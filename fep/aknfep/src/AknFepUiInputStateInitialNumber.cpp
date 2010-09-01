/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStateInitialNumber methods.
*
*/












#include "AknFepUiInputStateInitialNumber.h"
#include "AknFepUIManagerStateInterface.h"
#include "AknFepManagerUIInterface.h"
#include "AknFepManager.h"

#include <PtiEngine.h>
#include <PtiDefs.h>
#include <featmgr.h>                //FeatureManager

TAknFepInputStateInitialNumber::
TAknFepInputStateInitialNumber(MAknFepUIManagerStateInterface* aOwner)
    :TAknFepInputStateInitialMultitapBase(aOwner)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    ptiengine->SetInputMode(EPtiEngineNumeric);

    TRAP_IGNORE(ptiengine->SetExternalKeyMapL(EPtiEngineNumeric, EPtiKeyStar, 
                       (TDesC&)iOwner->NumberModeKeyMapping(), EPtiCaseLower))
    }

TBool TAknFepInputStateInitialNumber::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    
    if (aLength == ELongKeyPress)
        {
        switch (aKey)
            {
        case EKeyRightArrow:
        case EKeyLeftArrow:
        case EKeyDownArrow:
        case EKeyUpArrow:
        case EKeyBackspace:
            return EFalse;
       	case EPtiKeyStar:
       		 if (fepMan->EditSubmenuInUse() && fepMan->IsAbleToLaunchSCT())
       		 	{       		 
       		 	if (ptiengine->CurrentLanguage()->LanguageCode() == ELangKorean)
       		 		{
       		 		fepMan->LaunchSelectModeMenuL();      		 
       		 		}
				else
					{
				 	fepMan->LaunchSpecialCharacterTableL();       
					}
       		 	}       	     
        default:
            return ETrue;
            }
        }
   
    if(aKey == EPtiKeyStar)
        {
        TBool append = EFalse;
        if (aLength == EShortKeyPress)
            {
            if (fepMan->EditSubmenuInUse())
                {
                if (fepMan->IsOnlyNumericPermitted())   
                    {
                    append = ETrue;
                    }
                else
                    {
                    fepMan->ClearCcpuFlag(CAknFepManager::ECcpuStateIgnoreStarUp);
                    }   
                }               
            else
                {
                if (fepMan->IsAbleToLaunchSCT())            
                    {
                    fepMan->LaunchSpecialCharacterTableL();
                    }
                else
                    {           
                    append = ETrue;                         
                    }
                }
            }
        else
            {
            if (fepMan->EditSubmenuInUse() && fepMan->IsAbleToLaunchSCT())
                {                                       
                fepMan->LaunchSpecialCharacterTableL();             
                }
            }   
        if (append)
            {
#ifdef RD_INTELLIGENT_TEXT_INPUT            

	    // check editor is phonenumber editor or not.	    
            TBool phoneNumberEditor = (fepMan->EditorType() == CAknExtendedInputCapabilities::EPhoneNumberEditor);

            if( (fepMan->IsOnlyNumericPermitted()|| phoneNumberEditor ) && fepMan->GetNumericSCTResID())
            	{
            	TCallBack timerExpire(NumericResourceTimerTimeoutL, this);
            	fepMan->MultitapThroughSCTCharL(timerExpire); 
            	return ETrue;
            	}
           	else
           		{
           		NumericResourceKeyTimerTimeoutL();
           		}

#else           	
            iData = aKey;
            TPtrC text = ptiengine->AppendKeyPress((TPtiKey)aKey);

            // star key is inactive if keymapping is empty.
            if (text.Length() > 0 && text[0] != 0)
                {
                fepMan->NewCharacterL(text);                   
                
                // Commit immediately if there is only one character associated to the key.
                TBuf<CAknFepManager::EMaximumFepWordLength> mapdata;
                ptiengine->MappingDataForKey((TPtiKey)aKey, mapdata, ptiengine->Case());
                if (mapdata.Length() <= 1)
                    {
                    fepMan->CommitInlineEditL();
                    ptiengine->ClearCurrentWord();
                    iData = 0;                                          
                    }                   
	            } 
#endif
            }
 
        return ETrue;
        }

    if(iData)
        {
        fepMan->CommitInlineEditL();
        ptiengine->ClearCurrentWord();

        iData = 0;
        }
    return EFalse;
    }

#ifdef RD_INTELLIGENT_TEXT_INPUT

TInt TAknFepInputStateInitialNumber::NumericResourceTimerTimeoutL(TAny* aPtr)
    {
    reinterpret_cast<TAknFepInputStateInitialNumber*>(aPtr)->NumericResourceKeyTimerTimeoutL();
    return 1;
    }
TInt TAknFepInputStateInitialNumber::NumericResourceKeyTimerTimeoutL()
    {
    if(iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagInsideMultitapInlineEditingTransaction))
        {
        // this function is only called for number only editors 

        iOwner->FepMan()->CommitInlineEditL();
        }    
    return 1;
    }
    
#endif //RD_INTELLIGENT_TEXT_INPUT

// End of file
