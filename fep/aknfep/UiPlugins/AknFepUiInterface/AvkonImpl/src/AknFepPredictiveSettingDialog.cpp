/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0""
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:          
*
*/










/*
 * ============================================================================
 *  Name     : CAknFepPredictiveSettingList from AknFepPredictiveSettingList 
 * ============================================================================
 */
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <uikon/eikctrlstatus.h>
#endif
#include <centralrepository.h>
#include <AknQueryDialog.h>
#include <AknFepInternalCRKeys.h>
#include <PtiDefs.h>
#include <e32property.h>
#include <AvkonInternalCRKeys.h>
#include <AknFepInternalPSKeys.h>
#include <AknFepGlobalEnums.h>
#include "AknFepPredictiveSettingDialog.h"
#include <aknnotewrappers.h>
#include <aknfep.rsg>
#include <hlplch.h>
#include <csxhelp/cp.hlp.hrh> 
#include <akntitle.h> //CAknTitlePane
#include <StringLoader.h>
#include <aknnavide.h>

CAknFepPredictiveSettingDialog *CAknFepPredictiveSettingDialog::NewL(TInt aMenuResId, TInt aConfirmationQueryResId, TInt aTitlePaneResId)
	{
	CAknFepPredictiveSettingDialog* self = CAknFepPredictiveSettingDialog::NewLC(aMenuResId, aConfirmationQueryResId, aTitlePaneResId);		
	CleanupStack::Pop(self);
	return self;
	}

CAknFepPredictiveSettingDialog *CAknFepPredictiveSettingDialog::NewLC(TInt aMenuResId, TInt aConfirmationQueryResId, TInt aTitlePaneResId)
	{
	CAknFepPredictiveSettingDialog* self = new (ELeave) CAknFepPredictiveSettingDialog(aConfirmationQueryResId, aTitlePaneResId);
	CleanupStack::PushL(self);
	self->ConstructL(aMenuResId);
    return self;
	}

void CAknFepPredictiveSettingDialog::ConstructL( TInt aMenuTitleResourceId )
	{
	CAknDialog::ConstructL(aMenuTitleResourceId);
	PrepareStatusPaneL();
	iItemCloseEventCheck = CIdle::NewL(CActive::EPriorityStandard);
	iSimuKey = EFalse;
	}

CAknFepPredictiveSettingDialog ::CAknFepPredictiveSettingDialog(TInt aConfirmationQueryResId, TInt aTitlePaneResId):
	iConfirmationQueryResId(aConfirmationQueryResId),
	iPredictiveSettingOffQueryNotShown(ETrue),
	iTitlePane(NULL),
	iNaviPane(NULL),
	iNaviDecoratedTabGroup(NULL),
	iLastTitle(NULL),
	iStatusTitle(NULL),
	iTitlePaneResId(aTitlePaneResId),
	iSettingItemInEditingState(EFalse)
	{
	}
	
CAknFepPredictiveSettingDialog::~CAknFepPredictiveSettingDialog()
	{
	TRAP_IGNORE(RestoreStatusPaneL());
	delete iSettingsData;

	delete iItemCloseEventCheck;
	iItemCloseEventCheck = NULL;
	if (iAknFepRepository)
		delete iAknFepRepository;
	
	delete iLastTitle;
	delete iStatusTitle;
	}


SEikControlInfo CAknFepPredictiveSettingDialog::CreateCustomControlL(TInt /*aControlType*/)
{
	
	// construct the data object the settings list will use
	iSettingsData = CAknFepPredictiveSettingData::NewL();
	
#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
	iOriginalKeyboardLayout = iSettingsData->OwnerKeyboardType();
#endif //__ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
	
	iAknFepRepository = CRepository::NewL(KCRUidAknFep);	
	ReadSettingsDataL();
		
	iSettingsList = CAknFepPredictiveSettingList::NewL(*iSettingsData, *this);
	
	SEikControlInfo controlInfo;
	controlInfo.iFlags = EEikControlHasEars;
	controlInfo.iTrailerTextId = 0;
	controlInfo.iControl = iSettingsList;
	STATIC_CAST(CAknFepPredictiveSettingList*, controlInfo.iControl)->SetContainerWindowL(*this);
	
	return controlInfo;
}


CAknFepPredictiveSettingDialog::TFormControlTypes CAknFepPredictiveSettingDialog::ConvertCustomControlTypeToBaseControlType(TInt /*aControlType*/) const
{
	return EPopfieldDerived;
}


TBool CAknFepPredictiveSettingDialog::OkToExitL(TInt aButtonId)
{
	TBool retValue = EFalse;
	switch(aButtonId)
	{
	case EAknSoftkeyBack:
		iSettingsList->StoreSettingsL();
		SaveSettingsDataL();
		retValue = ETrue;
		break;
	
	case EAknSoftkeyOptions:
		DisplayMenuL();
		break;
	
	case EAknSoftkeyChange:
        EditItemL(EFalse);
		break;
	}
	return retValue;
}

void CAknFepPredictiveSettingDialog::ProcessCommandL(TInt aCommandId)
	{
    if(iSettingItemInEditingState && aCommandId == EAknCmdExit)
        {
        // If we are currently editing the settings item then generate a cancel event
        RWsSession& wsSession = CEikonEnv::Static()->WsSession();
        TKeyEvent escapeEvent = {EKeyEscape, EStdKeyEscape, 0, 0};
        TRAP_IGNORE(CEikonEnv::Static()->SimulateKeyEventL(escapeEvent, EEventKey));
        wsSession.Flush();
		// After generating cancel event, start the callback function so the settings
		// page is dismissed.
        iItemCloseEventCheck->Start( TCallBack(EventCheckCallback, this) );
        return;
        }   	
	
	// This is TSW eror fixing.
	// This function is called by dialog framework, whenever menu item is 
	// selected. we should have to hide existing display menu first to maitain the
	// control stack .
	// If user try to stop displaying menu after creating any control
	// In that situation whenever application goes to background framework remove the
	// control from the stack when try to remove the menubar from control stack.
	CAknDialog::ProcessCommandL(aCommandId);
	
	switch(aCommandId)
		{
		case EAknFepCmdPredSettingChange:
			EditItemL(ETrue);
			break;
		case EAknCmdHelp:
			{
				
            const TInt KArrayGranularity = 3;
            CArrayFix<TCoeHelpContext>* contexts =
                    new (ELeave) CArrayFixFlat<TCoeHelpContext>( KArrayGranularity );
	    	CleanupStack::PushL ( contexts );
	    	TUid appuid = { 0x100058EC };
	    	contexts->AppendL( TCoeHelpContext( appuid, KPREDINPUT_HLP_INPUT ) );
	    	CleanupStack::Pop( contexts );
	    	HlpLauncher::LaunchHelpApplicationL( CEikonEnv::Static()->WsSession(), contexts );		
			}
			break;
		case EAknCmdExit:
			// Save the setting and exit application.
			iSettingsList->StoreSettingsL();
			SaveSettingsDataL();
			// Pass the EAknCmdExit command to Avkon 
			// Framework 
			iAvkonAppUi->ProcessCommandL(aCommandId);
			break;
		}
	}

void CAknFepPredictiveSettingDialog::ReadSettingsDataL()
    {
    TInt autoWordCompl = 0;
	TInt typingCorrection = 0;
	TInt numberCandidates = 0;
	TInt primaryCandidate = 0;
	TInt physicalKeyboards = 0;
    // Read CenRep 
	if( iAknFepRepository )
		{
		iAknFepRepository->Get(KAknFepPredTxtFlagExtension, iPredictiveTextFlag);
		iAknFepRepository->Get(KAknFepAutoCompleteFlag, autoWordCompl);
		iAknFepRepository->Get(KAknFepTypingCorrectionLevel, typingCorrection);
		iAknFepRepository->Get(KAknFepNumberCandidateFlag, numberCandidates);
		iAknFepRepository->Get(KAknFepPrimaryCandidateFlag, primaryCandidate);
		iAknFepRepository->Get(KAknFepPhysicalKeyboards, physicalKeyboards);
		}

    // Parse keyboard dependent settings for current keyboard from bitmask
	TPtiKeyboardType layout = iSettingsData->OwnerKeyboardType();	
	TInt autoTmp = autoWordCompl;
	
	// This part of code for error fixing:	    
	// IF ITI is off for QWERTY/ITU-T iPredictiveTextFlag = 0
	// IF ITI is on for ITU-T 		  iPredictiveTextFlag = 1
	// IF ITI is on for QWERTY 	  	  iPredictiveTextFlag = 2
	
	switch(layout)
	    {
	    case EPtiKeyboardNone:
	        break;
	  	case EPtiKeyboard12Key:
	        autoTmp &= EKeyboardStyle12Key;
	        iPredictiveTextFlag &= EKeyboardStyle12Key;
	        break;
	    case EPtiKeyboardHalfQwerty:
	        autoTmp &= EKeyboardStyleHalfQwerty;
	        iPredictiveTextFlag &= EKeyboardStyleHalfQwerty;
	        break;
	  	case EPtiKeyboardQwerty4x12:
	  	case EPtiKeyboardQwerty4x10:
	  	case EPtiKeyboardQwerty3x11:	  	
	  	case EPtiKeyboardCustomQwerty:
	        autoTmp &= EKeyboardStyleQwerty;
	        iPredictiveTextFlag &= EKeyboardStyleQwerty;
	        break;
	  	default:
	  	    break;
	    }
	
	//load setting data
	iSettingsData->LoadL(( autoTmp ? 1 : 0), typingCorrection, numberCandidates,
	           primaryCandidate, physicalKeyboards, autoWordCompl);
   
    }
    

void CAknFepPredictiveSettingDialog::SaveSettingsDataL() const
    {
    // Read setting data and fill CenRep.
	if( iAknFepRepository )
	  	{
	   	TInt autoFlag = 0;
	   	iAknFepRepository->Get(KAknFepAutoCompleteFlag, autoFlag);
	   	TInt predFlag = 0;
	   	iAknFepRepository->Get(KAknFepPredTxtFlagExtension, predFlag);

	   	// Parse keyboard dependent settings for current keyboard from bitmask
	   	TPtiKeyboardType layout = iSettingsData->OwnerKeyboardType();	   	
	   	switch(layout)
	   	    {
	   	    case EPtiKeyboardNone:
	   	        break;
	   	    case EPtiKeyboard12Key:
	   	        if (iSettingsData->AutoWordCompl())
	   	            {
	   	            autoFlag |= EKeyboardStyle12Key;
	   	            }
	   	        else
	   	            {
	   	            autoFlag &= ~EKeyboardStyle12Key;
	   	            }
	   	        break;
	   	    case EPtiKeyboardHalfQwerty:
	   	        if (iSettingsData->AutoWordCompl())
	   	            {
	   	            autoFlag |= EKeyboardStyleHalfQwerty;
	   	            }
	   	        else
	   	            {
	   	            autoFlag &= ~EKeyboardStyleHalfQwerty;
	   	            }
	   	    case EPtiKeyboardQwerty4x12:
	   	    case EPtiKeyboardQwerty4x10:
	   	    case EPtiKeyboardQwerty3x11:
	   	    
	   	    case EPtiKeyboardCustomQwerty:
	   	        if (iSettingsData->AutoWordCompl())
	   	            {
	   	            autoFlag |= EKeyboardStyleQwerty;
	   	            }
	   	        else
	   	            {
	   	            autoFlag &= ~EKeyboardStyleQwerty;
	   	            }
	   	        break;
	   	    default:
	   	        break;
	   	    }

	    if(iSettingsList->DeviceHasMultipleKeyboards())
	        {
	        iAknFepRepository->Set(KAknFepAutoCompleteFlag, iSettingsData->AWCBitmask());
	        }
	    else
	        {
	        iAknFepRepository->Set(KAknFepAutoCompleteFlag, autoFlag);            
	        }
	    iAknFepRepository->Set(KAknFepTypingCorrectionLevel, iSettingsData->TypingCorrection());
	    iAknFepRepository->Set(KAknFepNumberCandidateFlag, iSettingsData->NumberCandidates());
	    iAknFepRepository->Set(KAknFepPrimaryCandidateFlag, iSettingsData->PrimaryCandidate());
	    }
    }

void CAknFepPredictiveSettingDialog::SaveCurrentSettingsDataL() const
    {
    // Read setting data and fill CenRep.
   	if(iAknFepRepository)
      	{
      	if(EAutoWordCompletion == iSettingsList->ListBox()->CurrentItemIndex())
      		{
	   		TInt autoFlag = 0;
	   		iAknFepRepository->Get(KAknFepAutoCompleteFlag, autoFlag);
	   		TInt predFlag = 0;
	   		iAknFepRepository->Get(KAknFepPredTxtFlagExtension, predFlag);

	   		// Parse keyboard dependent settings for current keyboard from bitmask
	   		TPtiKeyboardType layout = iSettingsData->OwnerKeyboardType();
	   		switch(layout)
	   	    	{
		   	    case EPtiKeyboardNone:
		   	        break;
	   		    case EPtiKeyboard12Key:
	   	    	    if (iSettingsData->AutoWordCompl())
	   	        	    {
	   	            	autoFlag |= EKeyboardStyle12Key;
		   	            }
		   	        else
	   		            {
	   	    	        autoFlag &= ~EKeyboardStyle12Key;
	   	        	    }
		   	        break;
		   	    case EPtiKeyboardHalfQwerty:
	   		        if (iSettingsData->AutoWordCompl())
	   	    	        {
	   	        	    autoFlag |= EKeyboardStyleHalfQwerty;
	   	            	}
		   	        else
		   	            {
	   		            autoFlag &= ~EKeyboardStyleHalfQwerty;
	   	    	        }
		   	        break;
		   	    case EPtiKeyboardQwerty4x12:
	   		    case EPtiKeyboardQwerty4x10:
	   	    	case EPtiKeyboardQwerty3x11:
		   	    case EPtiKeyboardCustomQwerty:
	   		        if (iSettingsData->AutoWordCompl())
	   	    	        {
	   	        	    autoFlag |= EKeyboardStyleQwerty;
	   	            	}
		   	        else
		   	            {
	   		            autoFlag &= ~EKeyboardStyleQwerty;
	   	    	        }
		   	        break;
		   	    default:
	   		        break;
	   	    	}

		    if(iSettingsList->DeviceHasMultipleKeyboards())
	    	    {
	        	iAknFepRepository->Set(KAknFepAutoCompleteFlag, iSettingsData->AWCBitmask());
		        }
		    else
	    	    {
	        	iAknFepRepository->Set(KAknFepAutoCompleteFlag, autoFlag);            
		        }
      		}
        else if(ETypingCorrectionLevel == iSettingsList->ListBox()->CurrentItemIndex())
        	{
        	iAknFepRepository->Set(KAknFepTypingCorrectionLevel, iSettingsData->TypingCorrection());
        	}
        else if(ENumberCandidate  == iSettingsList->ListBox()->CurrentItemIndex())
        	{
        	iAknFepRepository->Set(KAknFepNumberCandidateFlag, iSettingsData->NumberCandidates());	
        	}
        else if(EPrimaryCandidate == iSettingsList->ListBox()->CurrentItemIndex())
        	{
        	iAknFepRepository->Set(KAknFepPrimaryCandidateFlag, iSettingsData->PrimaryCandidate());	
        	}
        }
    }


void CAknFepPredictiveSettingDialog::EditItemL(TBool aCalledFromMenu)
	{
	TInt prevSettings = CurrentSettings();
	
	// Set the flag as we are currently editing the settings item
	iSettingItemInEditingState = ETrue;

	iSettingsList->EditCurrentItemL(aCalledFromMenu);
	if(iSettingItemInEditingState == EFalse)
	    {
	    User::Leave(KErrNone);
	    }
	LaunchPredictiveSettingOffQueryIfNecessaryL(prevSettings);
	SaveCurrentSettingsDataL();

	//Reset the flag as the current item edit is over
	iSettingItemInEditingState = EFalse;	
	}

void CAknFepPredictiveSettingDialog::LaunchPredictiveSettingOffQueryIfNecessaryL(TInt prevSettings)
	{
	if(iPredictiveSettingOffQueryNotShown)
		{
   		if(iAknFepRepository)
   			{
   			iAknFepRepository->Get(KAknFepPredTxtFlagExtension,iPredictiveTextFlag);
   			}
   		TPtiKeyboardType layout = iSettingsData->OwnerKeyboardType();
       	TBool QueryNeeded = EFalse;
		if(iSettingsList->DeviceHasMultipleKeyboards())
			{
			TInt activateITIForKbd = 0;
       		if(EAutoWordCompletion == iSettingsList->ListBox()->CurrentItemIndex() && 
       			iSettingsData->AWCBitmask() != prevSettings & 0xF)
       			{
       			// if AWC is ON and Prev.Settings of AWC is not ON and ITI is OFF
       			if((iSettingsData->AWCBitmask() & EKeyboardStyle12Key) && 
       			   ((prevSettings & 0xF) & EKeyboardStyle12Key)!=1 &&
       			   (iPredictiveTextFlag & EKeyboardStyle12Key) == 0)
       				{
       				QueryNeeded = ETrue;
       				activateITIForKbd |= EKeyboardStyle12Key ;
       				if(layout == EPtiKeyboard12Key)
       					iPredictiveSettingOffQueryNotShown = EFalse;
       				}
       			// if AWC is ON and Prev.Settings of AWC is not ON and ITI is OFF		
       			if((iSettingsData->AWCBitmask() & EKeyboardStyleQwerty) && 
       			   ((prevSettings & 0xF) & EKeyboardStyleQwerty)!=2 &&
       			   (iPredictiveTextFlag & EKeyboardStyleQwerty) == 0)
       				{
       				QueryNeeded = ETrue;
       				activateITIForKbd |= EKeyboardStyleQwerty ;
       				if(layout == EKeyboardStyleQwerty)
       					iPredictiveSettingOffQueryNotShown = EFalse;
       				}
       			if((iSettingsData->AWCBitmask() & EKeyboardStyleHalfQwerty) && 
       			   ((prevSettings & 0xF) & EKeyboardStyleHalfQwerty)!=4 &&
       			   (iPredictiveTextFlag & EKeyboardStyleHalfQwerty) == 0)
       				{
       				QueryNeeded = ETrue;
       				activateITIForKbd |= EKeyboardStyleHalfQwerty ;
       				if(layout == EKeyboardStyleHalfQwerty)
       					iPredictiveSettingOffQueryNotShown = EFalse;
       				}
       			}
       		// if Prev.Setting of TypingCorrection != Curr.Settng of TypingCorrection and
       		// Current TypingCorrection != OFF and ITI is OFF	
       		else if(iSettingsData->iTypingCorrection != ((prevSettings >> 4)&0xF) &&
					ETypingCorrectionLevel == iSettingsList->ListBox()->CurrentItemIndex() && 
			        0 != iSettingsData->TypingCorrection() &&
			        (iPredictiveTextFlag & EKeyboardStyleQwerty) == 0)
				{
				QueryNeeded = ETrue;
				iPredictiveSettingOffQueryNotShown = EFalse;
				activateITIForKbd |= EKeyboardStyleQwerty ;
				}
			// if Prev.Setting of NumberCandidate != Curr.Settng of NumberCandidate and
       		// Current NumberCandidate is ON and ITI is OFF		
       		else if(iSettingsData->iNumberCandidates != ((prevSettings >> 8)&0xF) &&
       				ENumberCandidate == iSettingsList->ListBox()->CurrentItemIndex() && 
			        1 == iSettingsData->NumberCandidates() &&
			        (iPredictiveTextFlag & EKeyboardStyleQwerty) == 0)
       			{
       			QueryNeeded = ETrue;
       			iPredictiveSettingOffQueryNotShown = EFalse;
       			activateITIForKbd |= EKeyboardStyleQwerty ;
       			}
			
			if(QueryNeeded)
				{
				CAknQueryDialog* dlg = CAknQueryDialog::NewL();
				if(dlg->ExecuteLD(iConfirmationQueryResId))
					{
				    if(activateITIForKbd & EKeyboardStyle12Key)
				    	{
				    	iPredictiveTextFlag |= EKeyboardStyle12Key;
				    	}
		       	    	
		       	    if(activateITIForKbd & EKeyboardStyleQwerty)
		       	    	{
		       	    	iPredictiveTextFlag |= EKeyboardStyleQwerty;
		       	    	}
						if(activateITIForKbd & EKeyboardStyleHalfQwerty)
		       	    	{
		       	    	iPredictiveTextFlag |= EKeyboardStyleHalfQwerty;
		       	    	} 	
		       	    }
				}
			}
		else
			{
			TInt predFlag = 0;
			iAknFepRepository->Get(KAknFepPredTxtFlagExtension, predFlag);
			TInt autoFlag = 0;
       		iAknFepRepository->Get(KAknFepAutoCompleteFlag, autoFlag);

	       	switch(layout)
    	   	    {
       		    case EPtiKeyboardNone:
       	    	    break;
       	    	case EPtiKeyboard12Key:
       	    		autoFlag = ((autoFlag & EPtiKeyboard12Key)!=0);
	       	    	// if ITI is ON for QWERTY then iPredictiveTextFlag = 2; In that case we have to find out the ITI state for ITU-T
    	   	        if (!( iPredictiveTextFlag & EKeyboardStyle12Key ))
       		            {
       	    	        predFlag |= EKeyboardStyle12Key;
       	        	    }
       	    		
       	    		break;
       	    	case EPtiKeyboardHalfQwerty:
       	    		autoFlag = ((autoFlag & EKeyboardStyleHalfQwerty)!=0);
    	   	        if (!(iPredictiveTextFlag & EKeyboardStyleHalfQwerty ))
       		            {
       	    	        predFlag |= EKeyboardStyleHalfQwerty;
       	            	}
       	    		break;
				case EPtiKeyboardQwerty4x12:
       	    	case EPtiKeyboardQwerty4x10:
       	    	case EPtiKeyboardQwerty3x11:
       	    	case EPtiKeyboardCustomQwerty:
       	    		autoFlag = ((autoFlag & EKeyboardStyleQwerty)!=0);
	       	    	// if ITI is ON for ITU-T then iPredictiveTextFlag = 1; In that case we have to find out the ITI state for QWERTY
    	   	        if (!(iPredictiveTextFlag & EKeyboardStyleQwerty ))
       		            {
       	    	        predFlag |= EKeyboardStyleQwerty;
       	            	}
       	    		break;
       	    	default:
       	    		break;
    	   	    }

			
			if (predFlag != iPredictiveTextFlag)
			    {
			    switch(iSettingsList->ListBox()->CurrentItemIndex())
			    	{
			    	case EAutoWordCompletion:
			    		if(iSettingsData->iAutoWordCompl != autoFlag &&
			    			1 == iSettingsData->AutoWordCompl())
			    			{
			    			QueryNeeded = ETrue;	
			    			}
			    		break;
			    	case ETypingCorrectionLevel:
			    		if(iSettingsData->iTypingCorrection != ((prevSettings >> 4)&0xF) && 
			    			0 != iSettingsData->TypingCorrection())
			    			{
			    			QueryNeeded = ETrue;
			    			}
			    		break;
			    	case ENumberCandidate:
			    		if(iSettingsData->iNumberCandidates != ((prevSettings >> 8)&0xF) && 
			    			0 != iSettingsData->NumberCandidates())
			    			{
			    			QueryNeeded = ETrue;
			    			}
			    		break;
			    	}
				
				if(QueryNeeded)
					{
					CAknQueryDialog* dlg = CAknQueryDialog::NewL();
					if(dlg->ExecuteLD(iConfirmationQueryResId))
						iPredictiveTextFlag = predFlag;

					iPredictiveSettingOffQueryNotShown = EFalse;	
					}
				}
			}
		if(iAknFepRepository && QueryNeeded)
			{
			iAknFepRepository->Set(KAknFepPredTxtFlagExtension,iPredictiveTextFlag);
       		iAknFepRepository->Set(KAknFepPredTxtFlag, iPredictiveTextFlag != 0); // For backwards compatibility							
			}						
		}
	}
TInt CAknFepPredictiveSettingDialog::CurrentSettings()
	{
	TInt settings = 0;
	//settings are the values of first 16 bits
	//	settings & 0xF = iAWCBitmask
	//	(settings >> 4) & 0xF = iNumberCandidates
	//	(settings >> 8) & 0xF = iTypingCorrection
	//	(settings >> 12) & 0xF = iPrimaryCandidate
	settings = (iSettingsData->iPrimaryCandidate << 12) |
				(iSettingsData->iNumberCandidates << 8) |
				(iSettingsData->iTypingCorrection << 4) |
				(iSettingsData->AWCBitmask());
	return settings;				
	}

// This part of code for error fixing multiple layout.
void CAknFepPredictiveSettingDialog::HandleResourceChange(TInt aType)
	{
	
    CAknDialog::HandleResourceChange(aType);
 
    // Check for keyboard layout change
    // If layout is changed, load the setting information based on 
    // Layout.
    if(aType==KEikDynamicLayoutVariantSwitch)
        {
    
    	// Read the Predictive setting
         if(iAknFepRepository)
        	{
        	iAknFepRepository->Get(KAknFepPredTxtFlagExtension, iPredictiveTextFlag);
        	}
   
   		// Adjust predictive flag after layout switch
         TPtiKeyboardType keyboardLayout = iSettingsData->ActiveKeyboardType();
	//settings page not to be retained on change of keyboard layout for Touch Input.
#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
         
        if ( iOriginalKeyboardLayout != keyboardLayout )
            {
            if(iSettingItemInEditingState)
            	{
				if (!iSimuKey)
				    {
					// If we are currently editing the settings item then generate a cancel event
					// and start the scheduler so as to close the settings page 
					// when the blocking call completes. 
					RWsSession& wsSession = CEikonEnv::Static()->WsSession();
					TKeyEvent escapeEvent = {EKeyEscape, EStdKeyEscape, 0, 0};
					TRAP_IGNORE(CEikonEnv::Static()->SimulateKeyEventL(escapeEvent, EEventKey));
					iSimuKey = ETrue;
					wsSession.Flush();
					iItemCloseEventCheck->Start( TCallBack(EventCheckCallback, this) );
				    }
            	}
            else
	            {
    	        // In case the current settings item is not in editing state then directly 
    	        // close the settings page.
            	TRAP_IGNORE(TryExitL(EAknSoftkeyBack));	
    	        }
            return;
            }
#endif //__ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
     	switch(keyboardLayout)
   	    	{
	   	    case EPtiKeyboard12Key:
	   	    	// Dictionary is ON for ITU-T
	   	    	iPredictiveTextFlag	&= EKeyboardStyle12Key;
	   	    	break;
	   	    case EPtiKeyboardQwerty4x12:
	   	    case EPtiKeyboardQwerty4x10:
	   	    case EPtiKeyboardQwerty3x11:
	   	    case EPtiKeyboardHalfQwerty:
	   	    case EPtiKeyboardCustomQwerty:
	   	    	// 	// Dictionary is ON for QWERTY
	   	    	iPredictiveTextFlag &= EKeyboardStyleQwerty;
	   	    case EPtiKeyboardNone:
	   	    default:
	   	        break;
   	        }
        TRAP_IGNORE(SetTitlePaneL());
        }    
	}

TInt CAknFepPredictiveSettingDialog::EventCheckCallback(TAny* aObj)
	{
    TRAP_IGNORE(static_cast<CAknFepPredictiveSettingDialog*>(aObj)->AttemptToExitL());
    return KErrNone;	
	}
	
void CAknFepPredictiveSettingDialog::AttemptToExitL()
	{
	if(iItemCloseEventCheck->IsActive())
		{
		iItemCloseEventCheck->Cancel();	
		}
	// iSettingItemInEditingState sets before the blocking call of editing item state and 
	// gets reset after the blocking call.
    if (!iSettingItemInEditingState)
    	{
    	// In case we are here, then we need to close the settings item because of layout change
    	TRAP_IGNORE(TryExitL(EAknSoftkeyBack));	
    	}
    else
    	{
    	// Because of priority we hit the scheduler first and then the blocking call gets
    	// executed. To handle such a scenario, restart the scheduler.
    	iItemCloseEventCheck->Start( TCallBack(EventCheckCallback, this) );	
    	}
	}
	
void CAknFepPredictiveSettingDialog::PrepareStatusPaneL()
    {
    CCoeEnv* coeEnv = CCoeEnv::Static();
    CAknAppUi* appUi = static_cast<CAknAppUi*>(coeEnv->AppUi());
    CEikStatusPane* statusPane = appUi->StatusPane();
    
    iTitlePane = static_cast<CAknTitlePane*>( statusPane->ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );
    
    const TDesC* appTitle = iTitlePane->Text();
    iLastTitle = appTitle->AllocL();
    
    iStatusTitle = StringLoader::LoadL( iTitlePaneResId,coeEnv);
    SetTitlePaneL();
    
    iNaviPane = static_cast<CAknNavigationControlContainer*>(statusPane->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) ) );
    
    if(iNaviPane)
        {
        // Create a new tab Groub for Settings Page and push it on the Navigation Pane
        // This is done so as to avoid previous page Tab Group to be used
        iNaviDecoratedTabGroup = iNaviPane->CreateTabGroupL();
        if(iNaviDecoratedTabGroup)
            {
            // Tab Group visibility is set to false
            iNaviDecoratedTabGroup->MakeVisible(EFalse);
            iNaviPane->PushL(*iNaviDecoratedTabGroup);
            }
        } 
    }

void CAknFepPredictiveSettingDialog::SetTitlePaneL()
    {
    iTitlePane->SetTextL( *iStatusTitle );
    }

void CAknFepPredictiveSettingDialog::RestoreStatusPaneL()
    {
    iTitlePane->SetTextL( *iLastTitle );
    
    if(iNaviPane && iNaviDecoratedTabGroup)
        {
        // Pop the Tab Group created for the Settings page
        iNaviPane->Pop();
        delete iNaviDecoratedTabGroup;
        iNaviDecoratedTabGroup = NULL;
        }    
    }

void CAknFepPredictiveSettingDialog::HandlePointerEventL(const TPointerEvent& aPointerEvent)
    {
    if ( aPointerEvent.iType == TPointerEvent::EDrag )
        {
        iSettingsList->HandlePointerEventL(aPointerEvent);
        }
    else
        {
        CAknDialog::HandlePointerEventL(aPointerEvent);
        }
    }

/**
 *From MEikCommandObserver
 */
void CAknFepPredictiveSettingDialog::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane) 
    {
    if ( iAvkonAppUi->IsSingleClickCompatible() )
        {
        if ( R_PREDICTIVESETTINGDIALOG_MENU == aResourceId )
            {
            aMenuPane->SetItemDimmed(EAknFepCmdPredSettingChange, ETrue);
            }
        } 
    }

/**
 * From CAknDialog
 */
void CAknFepPredictiveSettingDialog::SizeChanged()
    {
    if( iSettingsList != NULL && iSettingsList->ListBox() != NULL )
    	{
        iSettingsList->ListBox()->EnableStretching(EFalse);
    	}
    CAknDialog::SizeChanged();
    }
