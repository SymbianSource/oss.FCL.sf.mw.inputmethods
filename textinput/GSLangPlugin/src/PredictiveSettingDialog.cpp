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
 *  Name     : CGSPredictiveSettingList from AknFepPredictiveSettingList 
 * ============================================================================
 */
#include <centralrepository.h>
#include <AknQueryDialog.h>
#include <AknFepInternalCRKeys.h>
#include <PtiDefs.h>
#include <e32property.h>
#include <AvkonInternalCRKeys.h>
#include <AknFepGlobalEnums.h>
#include "PredictiveSettingDialog.h"
#include <aknnotewrappers.h>
#include <aknfep.rsg>
#include <hlplch.h> // HlpLauncher
#include <csxhelp/cp.hlp.hrh> 
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <uikon/eikctrlstatus.h>
#endif

CGSPredictiveSettingDialog *CGSPredictiveSettingDialog::NewL(TInt aMenuResId, TInt aConfirmationQueryResId)
	{
	CGSPredictiveSettingDialog* self = CGSPredictiveSettingDialog::NewLC(aMenuResId, aConfirmationQueryResId);		
	CleanupStack::Pop(self);
	return self;
	}

CGSPredictiveSettingDialog *CGSPredictiveSettingDialog::NewLC(TInt aMenuResId, TInt aConfirmationQueryResId)
	{
	CGSPredictiveSettingDialog* self = new (ELeave) CGSPredictiveSettingDialog(aConfirmationQueryResId);
	CleanupStack::PushL(self);
	self->ConstructL(aMenuResId);
    return self;
	}

void CGSPredictiveSettingDialog::ConstructL( TInt aMenuTitleResourceId )
	{
	CAknDialog::ConstructL(aMenuTitleResourceId);

#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__	
	iItemCloseEventCheck = CIdle::NewL(CActive::EPriorityStandard);
#endif	

	}

CGSPredictiveSettingDialog ::CGSPredictiveSettingDialog(TInt aConfirmationQueryResId):
	iConfirmationQueryResId(aConfirmationQueryResId),
	iPredictiveSettingOffQueryNotShown(ETrue)
#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__	
	,iSettingItemInEditingState(EFalse)
#endif //__ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__		
	{
#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__	
	RProperty::Get(KCRUidAvkon, KAknKeyBoardLayout, iOriginalKeyboardLayout);
#endif //__ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
	}
	
CGSPredictiveSettingDialog::~CGSPredictiveSettingDialog()
	{
	delete iSettingsData;

#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__	
	delete iItemCloseEventCheck;
	iItemCloseEventCheck = NULL;
#endif
		
	}


SEikControlInfo CGSPredictiveSettingDialog::CreateCustomControlL(TInt /*aControlType*/)
{
	
	// construct the data object the settings list will use
	iSettingsData = CGSPredictiveSettingData::NewL();
	ReadSettingsDataL();
		
	iSettingsList = CGSPredictiveSettingList::NewL(*iSettingsData, *this);
	
	SEikControlInfo controlInfo;
	controlInfo.iFlags = EEikControlHasEars;
	controlInfo.iTrailerTextId = 0;
	controlInfo.iControl = iSettingsList;
	STATIC_CAST(CGSPredictiveSettingList*, controlInfo.iControl)->SetContainerWindowL(*this);
	
	return controlInfo;
}


CGSPredictiveSettingDialog::TFormControlTypes CGSPredictiveSettingDialog::ConvertCustomControlTypeToBaseControlType(TInt /*aControlType*/) const
{
	return EPopfieldDerived;
}


TBool CGSPredictiveSettingDialog::OkToExitL(TInt aButtonId)
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

void CGSPredictiveSettingDialog::ProcessCommandL(TInt aCommandId)
	{
	// This is MT Error fixing.
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

// Reads Input Settings data from CenRep.
void CGSPredictiveSettingDialog::ReadSettingsDataL()
    {
    TInt autoWordCompl = 0;
	TInt typingCorrection = 0;
	TInt numberCandidates = 0;
	TInt primaryCandidate = 0;
	TInt physicalKeyboards = 0;
    // Read CenRep 
    TInt ret = KErrNone;
    CRepository* aknFepRepository = NULL;
    TRAP(ret, aknFepRepository = CRepository::NewL(KCRUidAknFep));
    if (ret == KErrNone)
        {
         if(aknFepRepository)
        	{
        	aknFepRepository->Get(KAknFepPredTxtFlagExtension, iPredictiveTextFlag);
        	aknFepRepository->Get(KAknFepAutoCompleteFlag, autoWordCompl);
        	aknFepRepository->Get(KAknFepTypingCorrectionLevel, typingCorrection);
        	aknFepRepository->Get(KAknFepNumberCandidateFlag, numberCandidates);
        	aknFepRepository->Get(KAknFepPrimaryCandidateFlag, primaryCandidate);
        	aknFepRepository->Get(KAknFepPhysicalKeyboards, physicalKeyboards);
        	delete aknFepRepository;
        	}
        }

    // Parse keyboard dependent settings for current keyboard from bitmask
	TInt keyboardLayout = 0;
	RProperty::Get(KCRUidAvkon, KAknKeyBoardLayout, keyboardLayout);
	TPtiKeyboardType layout = (TPtiKeyboardType)keyboardLayout;
	TInt autoTmp = autoWordCompl;
	
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
	    	    
	// The Value of iPredictiveTextFlag should be as mentioned below according to keyboard 
	// layout and ITI settings	
	// IF ITI is off for QWERTY/ITU-T iPredictiveTextFlag = 0
	// IF ITI is on for ITU-T         iPredictiveTextFlag = 1
	// IF ITI is on for QWERTY        iPredictiveTextFlag = 2
	
	//load setting data
	iSettingsData->LoadL(( autoTmp ? 1 : 0), typingCorrection, numberCandidates,
	           primaryCandidate, physicalKeyboards, autoWordCompl);
   
    }
    

void CGSPredictiveSettingDialog::SaveSettingsDataL() const
    {
    // Read setting data and fill CenRep.
    TInt ret = KErrNone;
    CRepository* AknFepRepository = NULL;
    TRAP(ret, AknFepRepository = CRepository::NewL(KCRUidAknFep));
    if (ret == KErrNone)
    	{
    	if(AknFepRepository)
      	{
       	TInt autoFlag = 0;
       	AknFepRepository->Get(KAknFepAutoCompleteFlag, autoFlag);
       	TInt predFlag = 0;
       	AknFepRepository->Get(KAknFepPredTxtFlagExtension, predFlag);

       	// Parse keyboard dependent settings for current keyboard from bitmask
       	TInt keyboardLayout = 0;
       	RProperty::Get(KCRUidAvkon, KAknKeyBoardLayout, keyboardLayout);
       	TPtiKeyboardType layout = (TPtiKeyboardType)keyboardLayout;
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
            AknFepRepository->Set(KAknFepAutoCompleteFlag, iSettingsData->AWCBitmask());
            }
        else
            {
            AknFepRepository->Set(KAknFepAutoCompleteFlag, autoFlag);            
            }
        AknFepRepository->Set(KAknFepTypingCorrectionLevel, iSettingsData->TypingCorrection());
        AknFepRepository->Set(KAknFepNumberCandidateFlag, iSettingsData->NumberCandidates());
        AknFepRepository->Set(KAknFepPrimaryCandidateFlag, iSettingsData->PrimaryCandidate());
        delete AknFepRepository;
        }
      }
    }

void CGSPredictiveSettingDialog::SaveCurrentSettingsDataL() const
    {
    // Read setting data and fill CenRep.
    TInt ret = KErrNone;
    CRepository* AknFepRepository = NULL;
    TRAP(ret, AknFepRepository = CRepository::NewL(KCRUidAknFep));
    if (ret == KErrNone)
    	{
    	if(AknFepRepository)
      	{
      	if(EAutoWordCompletion == iSettingsList->ListBox()->CurrentItemIndex())
      		{
       		TInt autoFlag = 0;
       		AknFepRepository->Get(KAknFepAutoCompleteFlag, autoFlag);
       		TInt predFlag = 0;
       		AknFepRepository->Get(KAknFepPredTxtFlagExtension, predFlag);

       		// Parse keyboard dependent settings for current keyboard from bitmask
       		TInt keyboardLayout = 0;
       		RProperty::Get(KCRUidAvkon, KAknKeyBoardLayout, keyboardLayout);
       		TPtiKeyboardType layout = (TPtiKeyboardType)keyboardLayout;
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

	        //AknFepRepository->Set(KAknFepPredTxtFlagExtension, predFlag);
    	    //AknFepRepository->Set(KAknFepPredTxtFlag, iPredictiveTextFlag != 0); // For backwards compatibility
        	if(iSettingsList->DeviceHasMultipleKeyboards())
            	{
	            AknFepRepository->Set(KAknFepAutoCompleteFlag, iSettingsData->AWCBitmask());
    	        }
        	else
            	{
	            AknFepRepository->Set(KAknFepAutoCompleteFlag, autoFlag);            
    	        }
        	}
        	else if(ETypingCorrectionLevel == iSettingsList->ListBox()->CurrentItemIndex())
        		{
        		AknFepRepository->Set(KAknFepTypingCorrectionLevel, iSettingsData->TypingCorrection());
        		}
        	else if(ENumberCandidate  == iSettingsList->ListBox()->CurrentItemIndex())
        		{
        		AknFepRepository->Set(KAknFepNumberCandidateFlag, iSettingsData->NumberCandidates());	
        		}
        	else if(EPrimaryCandidate == iSettingsList->ListBox()->CurrentItemIndex())
        		{
        		AknFepRepository->Set(KAknFepPrimaryCandidateFlag, iSettingsData->PrimaryCandidate());	
        		}
        	delete AknFepRepository;
        	}
      	}
    }

void CGSPredictiveSettingDialog::EditItemL(TBool aCalledFromMenu)
{
    TInt prevSettings = CurrentSettings();
#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__	
	// Set the flag as we are currently editing the settings item
	iSettingItemInEditingState = ETrue;
#endif //__ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
    
	iSettingsList->EditCurrentItemL(aCalledFromMenu);
	LaunchPredictiveSettingOffQueryIfNecessaryL(prevSettings);
	SaveCurrentSettingsDataL();

#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__	
	//Reset the flag as the current item edit is over
	iSettingItemInEditingState = EFalse;
#endif //__ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
		
}

void CGSPredictiveSettingDialog::LaunchPredictiveSettingOffQueryIfNecessaryL(TInt aPrevSettings)
	{
	if(iPredictiveSettingOffQueryNotShown)
		{
		CRepository* AknFepRepository = NULL;
		AknFepRepository = CRepository::NewL(KCRUidAknFep);
		if(AknFepRepository)
			{
		    CleanupStack::PushL(AknFepRepository);
		    AknFepRepository->Get(KAknFepPredTxtFlagExtension,iPredictiveTextFlag);
		    }
		    
		TInt keyboardLayout = 0;
		RProperty::Get(KCRUidAvkon, KAknKeyBoardLayout, keyboardLayout);
		TPtiKeyboardType layout = (TPtiKeyboardType)keyboardLayout;
		TBool QueryNeeded = EFalse;

		if(iSettingsList->DeviceHasMultipleKeyboards())
		    {
			TInt activateITIForKbd = 0;
			if(EAutoWordCompletion == iSettingsList->ListBox()->CurrentItemIndex() && 
			   iSettingsData->AWCBitmask() != aPrevSettings & 0xF)
			    {
			    // if AWC is ON and Prev.Settings of AWC is not ON and ITI is OFF
			    if((iSettingsData->AWCBitmask() & EKeyboardStyle12Key) && 
			       ((aPrevSettings & 0xF) & EKeyboardStyle12Key)!=1 &&
			        (iPredictiveTextFlag & EKeyboardStyle12Key) == 0)
			         {
			         QueryNeeded = ETrue;
			         activateITIForKbd |= EKeyboardStyle12Key ;
			         if(layout == EPtiKeyboard12Key)
			             iPredictiveSettingOffQueryNotShown = EFalse;
			         }
			    // if AWC is ON and Prev.Settings of AWC is not ON and ITI is OFF        
			    if((iSettingsData->AWCBitmask() & EKeyboardStyleQwerty) && 
			       ((aPrevSettings & 0xF) & EKeyboardStyleQwerty)!=2 &&
			        (iPredictiveTextFlag & EKeyboardStyleQwerty) == 0)
			         {
			         QueryNeeded = ETrue;
			         activateITIForKbd |= EKeyboardStyleQwerty ;
			         if(layout == EKeyboardStyleQwerty)
			             iPredictiveSettingOffQueryNotShown = EFalse;
			         }
			     if((iSettingsData->AWCBitmask() & EKeyboardStyleHalfQwerty) && 
			       ((aPrevSettings & 0xF) & EKeyboardStyleHalfQwerty)!=4 &&
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
			else if(iSettingsData->iTypingCorrection != ((aPrevSettings >> 4)&0xF) &&
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
			else if(iSettingsData->iNumberCandidates != ((aPrevSettings >> 8)&0xF) &&
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
			AknFepRepository->Get(KAknFepPredTxtFlagExtension, predFlag);
			TInt autoFlag = 0;
       		AknFepRepository->Get(KAknFepAutoCompleteFlag, autoFlag);

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
    	   	        if (!( iPredictiveTextFlag & EKeyboardStyleHalfQwerty))
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
			    		if(iSettingsData->iTypingCorrection != ((aPrevSettings >> 4)&0xF) && 
			    			0 != iSettingsData->TypingCorrection())
			    			{
			    			QueryNeeded = ETrue;
			    			}
			    		break;
			    	case ENumberCandidate:
			    		if(iSettingsData->iNumberCandidates != ((aPrevSettings >> 8)&0xF) && 
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
		if(AknFepRepository)
			{
			if(QueryNeeded)
				{
				AknFepRepository->Set(KAknFepPredTxtFlagExtension,iPredictiveTextFlag);
        		AknFepRepository->Set(KAknFepPredTxtFlag, iPredictiveTextFlag != 0); // For backwards compatibility							
				}
			CleanupStack::Pop(AknFepRepository);
			delete AknFepRepository;
			}			
		}
	}
TInt CGSPredictiveSettingDialog::CurrentSettings()
    {
    TInt settings = 0;
    //settings are the values of first 16 bits
    //  settings & 0xF = iAWCBitmask
    //  (settings >> 4) & 0xF = iNumberCandidates
    //  (settings >> 8) & 0xF = iTypingCorrection
    //  (settings >> 12) & 0xF = iPrimaryCandidate
    settings = (iSettingsData->iPrimaryCandidate << 12) |
                (iSettingsData->iNumberCandidates << 8) |
                (iSettingsData->iTypingCorrection << 4) |
                (iSettingsData->AWCBitmask());
    return settings;                
    }

// This part of code for error fixing multiple layout. Whenever you switch from one keyboard 
// Layout to another keyboard layout HandleResourceChange is getting called and loads the 
// Setting information specific to keyboard layout
void CGSPredictiveSettingDialog::HandleResourceChange(TInt aType)
    {
    
    CAknDialog::HandleResourceChange(aType);
 
    // Check for keyboard layout change
    // If layout is changed, load the setting information based on 
    // Layout.
    if(aType==KEikDynamicLayoutVariantSwitch)
        {   
        TInt keyboardLayout = EPtiKeyboardNone;
        RProperty::Get(KCRUidAvkon, KAknKeyBoardLayout, keyboardLayout);
#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
        if(iOriginalKeyboardLayout != keyboardLayout)
            {
            if(iSettingItemInEditingState)
            	{
            	// If we are currently editing the settings item then generate a cancel event
            	// and start the scheduler so as to close the settings page 
            	// when the blocking call completes. 
            	RWsSession& wsSession = CEikonEnv::Static()->WsSession();
            	TKeyEvent escapeEvent = {EKeyEscape, EStdKeyEscape, 0, 0};
            	TRAP_IGNORE(CEikonEnv::Static()->SimulateKeyEventL(escapeEvent, EEventKey));
            	wsSession.Flush();
            	iItemCloseEventCheck->Start( TCallBack(EventCheckCallback, this) );
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
        // Read the Predictive setting
        TInt ret = KErrNone;
        CRepository* AknFepRepository = NULL;
        TRAP(ret, AknFepRepository = CRepository::NewL(KCRUidAknFep));
        if(ret == KErrNone)
        {
          AknFepRepository->Get(KAknFepPredTxtFlagExtension,iPredictiveTextFlag);
   
        // Adjust predictive flag after layout switch
        TInt keyboardLayout = EPtiKeyboardNone;
        RProperty::Get(KCRUidAvkon, KAknKeyBoardLayout, keyboardLayout);
        switch(keyboardLayout)
            {
            case EPtiKeyboard12Key:
                // Dictionary is ON for ITU-T
                iPredictiveTextFlag &= EKeyboardStyle12Key;
                break;
            case EPtiKeyboardHalfQwerty:    
                iPredictiveTextFlag &= EKeyboardStyleHalfQwerty;
                break;
            case EPtiKeyboardQwerty4x12:
            case EPtiKeyboardQwerty4x10:
            case EPtiKeyboardQwerty3x11:            
            case EPtiKeyboardCustomQwerty:
                // Dictionary is ON for QWERTY
                iPredictiveTextFlag &= EKeyboardStyleQwerty;
            case EPtiKeyboardNone:
            default:
                break;
            }
        delete AknFepRepository;
        }
        }        
    }
#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
TInt CGSPredictiveSettingDialog::EventCheckCallback(TAny* aObj)
	{
    TRAP_IGNORE(static_cast<CGSPredictiveSettingDialog*>(aObj)->AttemptToExitL());
    return KErrNone;	
	}
	
void CGSPredictiveSettingDialog::AttemptToExitL()
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
#endif //__ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__

void CGSPredictiveSettingDialog::HandlePointerEventL(const TPointerEvent& aPointerEvent)
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
void CGSPredictiveSettingDialog::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane) 
    {
    if ( iAvkonAppUi->IsSingleClickCompatible() )
        {
        if ( R_PREDICTIVESETTINGDIALOG_MENU == aResourceId )
            {
            aMenuPane->SetItemDimmed(EAknFepCmdPredSettingChange, ETrue);
            }
        } 
    }
