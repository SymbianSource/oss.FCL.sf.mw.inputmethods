/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStateIndicPhoneticQwerty methods.
*
*/












#include "aknfepuiinputstateindicphoneticqwerty.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"           //MAknFepManagerUIInterface
#include "AknFepCaseManager.h"
#include "AknFepUiIndicInputManager.h"
#include "AknFepPanic.h"

#include <e32err.h>
#include <PtiEngine.h>
#include <PtiDefs.h>
#include <PtiKeyMappings.h>
#include <CentralRepository.h> 
#include <Languages.hrh>

TAknFepInputStateIndicPhoneticQwerty::
TAknFepInputStateIndicPhoneticQwerty(MAknFepUIManagerStateInterface* aOwner)
    :TAknFepInputStateQwerty(aOwner)
    {
    iState = EInitial;
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    ptiengine->ClearCurrentWord();
    TRAP_IGNORE(ptiengine->ActivateLanguageL(KLangHindiPhonetic,
                                             EPtiEngineInputModeIndicPhoneticQwerty));//KLangHindiPhonetic
    ptiengine->SetCase(EPtiCaseLower);
    }

TBool TAknFepInputStateIndicPhoneticQwerty::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    TBool ret = ETrue;
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    __ASSERT_DEBUG(ptiengine != NULL,AknFepPanic(EAknFepPanicNoValidPtiEngine));
    
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    __ASSERT_DEBUG(fepMan != NULL,AknFepPanic(EAknFepPanicNoValidFepManager));
         
	if(!IsQwertyKey(aKey)|| 
		fepMan->IsOnlyNumericPermitted()||
		fepMan->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed))
		{
		fepMan->CommitInlineEditL();
		ptiengine->HandleCommandL(EPtiCommandBreakSyllable, NULL );	
		ret = TAknFepInputStateQwerty::HandleKeyL(aKey, aLength);
		}
	else if (aKey == EPtiKeyQwertySpace ||aKey == EStdKeyEnter)
		{    
		TInt languageCode = 0;
    	MPtiLanguage* curlang = ptiengine->CurrentLanguage();
	    if (curlang)
	        {
	        languageCode = curlang->LanguageCode();
	        }

		// Space clears Explicite halant for North Indian Languages.
		if(IsToRemoveHalantForLanguage(languageCode))
			{
			RemoveHalantL(TLanguage(languageCode));
			fepMan->CommitInlineEditL();
			}
		ptiengine->HandleCommandL(EPtiCommandBreakSyllable, NULL );	
		ret = EFalse;
		}
	else
		{
		TBool isCharacter = ETrue;
	    if (!iData)
	        {
	        isCharacter = IsCharacterKey(aKey);
	        }
	        
		HandleCaseChange(aKey,isCharacter);
	    iData = aKey;

	    TBool keyEventHandledByFep = ETrue;
	    
		TPhoneticArg arg;
	    TBuf<CAknFepManager::EMaximumFepWordLength> destinationbuf;
	    destinationbuf.Zero();               
	    
		TInt errorcode = 0;   
	    
	    if(!((aKey == EPtiKeyStar) || 
	    	(aKey == EPtiKeyQwerty8 && fepMan->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode)) || 
	    	(aKey == EPtiKeyQwertySpace)))
	    	{
	        TPtrC string = ptiengine->AppendKeyPress((TPtiKey)aKey);
	        arg.iChar = string[0];
	        arg.iDest = & destinationbuf;
	        errorcode = ptiengine->HandleCommandL(EPtiCommandGetPhoneticText,REINTERPRET_CAST( TAny*, &arg));
	        }
	    else
	        {
	    	errorcode = ptiengine->HandleCommandL(EPtiCommandBreakSyllable, NULL );	
	    	}
	    	
	    switch(errorcode)
	    	{
	    	case KErrNone:
	    		{
	    		switch(arg.iState)
	    			{
	    			case EIndicSyllableStateInsideSyllable:
	    				{            				
	        			if (destinationbuf.Length())
				            {
				            fepMan->NewCharacterSequenceL(destinationbuf,
				                                          EIndicInputResponsePhoneticQwertyText);
				            }
	    				}
	    			break;
	    			case EIndicSyllableStateSyllableBroken:
	    				{
	  					fepMan->CommitInlineEditL();
	    				if (destinationbuf.Length())
				            {
				            fepMan->NewCharacterSequenceL(destinationbuf,
				                                          EIndicInputResponsePhoneticQwertyText);
				            }	
	    				}
	    			break;
	    			default:
	    			break;
	    			}
	    		}
	    	break;
	    	case KErrOverflow:
	    		{
				fepMan->NewCharacterSequenceL(destinationbuf,EIndicInputResponsePhoneticQwertyText);    		
	    		fepMan->CommitInlineEditL();
	        	ptiengine->HandleCommandL(EPtiCommandClearPhoneticBuffer,NULL);
	    		}
	    	break;
	    	default:
	    	break;
	    	}
	    ptiengine->ClearCurrentWord();
	    CAknFepCaseManager* caseMan = iOwner->CaseMan(); 
	    iOwner->CaseMan()->UpdateCase(ENullNaviEvent);
		}	

    return ret;
    }
    
TBool TAknFepInputStateIndicPhoneticQwerty::IsQwertyKey(const TInt aKey)
    {
    TBool response = EFalse;
    if ( (aKey >= EPtiKeyQwertyA && aKey <= EPtiKeyQwertyZ)||
        (aKey == EPtiKeyQwertyFullstop) ||
        (aKey == EPtiKeyStar || 
        (aKey == EPtiKeyQwerty8 && 
        iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode))) ||
        aKey == EPtiKeyQwertySpace || 
        aKey == EStdKeyEnter)
        {
        response = ETrue;
        }
    return response;
    }
    
void TAknFepInputStateIndicPhoneticQwerty::RemoveHalantL(TLanguage aLanguage)
	{
	MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    if(TAknFepUiIndicInputManager::IsCharVirama(fepMan->PreviousChar(), aLanguage))
    	{
    	fepMan->RemovePreviousCharacterL();	
    	}
	}
	
TBool TAknFepInputStateIndicPhoneticQwerty::IsToRemoveHalantForLanguage(TInt aLanguage)
	{
	TBool ret = EFalse;
	switch(aLanguage)
		{
		case KLangHindiPhonetic:
			ret = ETrue;
		break;
		}
	return ret;	
	}
	
	
// End of file
