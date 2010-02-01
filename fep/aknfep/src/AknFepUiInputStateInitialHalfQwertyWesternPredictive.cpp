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
*       Provides the TAknFepInputStateInitialHalfQwertyWesternPredictive methods.
*
*/












#include "AknFepUiInputStateInitialHalfQwertyWesternPredictive.h"
#include "AknFepUIManagerStateInterface.h"      //MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"           //MAknFepManagerUIInterface
#include "AknFepManager.h"
#include "AknFepCaseManager.h"                  //CAknFepCaseManager

#include <PtiDefs.h>
#include <PtiEngine.h>

// TODO: Remove this include
#include <AvkonInternalCRKeys.h>
static const TInt KKeyMappingsLength = 32;
TAknFepUiInputStateInitialHalfQwertyWesternPredictive::TAknFepUiInputStateInitialHalfQwertyWesternPredictive(MAknFepUIManagerStateInterface* aOwner)
    //:TAknFepInputStateBase(aOwner),
    :TAknFepInputStateInitialMultitapBase(aOwner)
    {
    iData = 0;
    CPtiEngine* ptiengine = iOwner->PtiEngine();
	// this is already being done in the base constructor
	// should not be done again
//    TPtiTextCase caseMode = ptiengine->Case();
//    ptiengine->ClearCurrentWord();
//    ptiengine->SetCase(caseMode);
    ptiengine->SetInputMode( EPtiEngineHalfQwerty );
    }

TBool TAknFepUiInputStateInitialHalfQwertyWesternPredictive::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    
    TBool retVal = EFalse;
    TBuf<KKeyMappingsLength> mappedCharacters;
    if (iData && (iData != aKey) && fepMan->IsFlagSet(CAknFepManager::EFlagInsideMultitapInlineEditingTransaction))
        {
        iOwner->PtiEngine()->CancelTimerActivity();        
        fepMan->CommitInlineEditL();      
        iOwner->PtiEngine()->ClearCurrentWord();        
        fepMan->ClearFlag(CAknFepManager::EFlagInsideMultitapInlineEditingTransaction);             
        iData = 0;
        // if the current fn key state is FnNext, then clear it.
        if( !fepMan->IsFlagSet( CAknFepManager::EFlagLongShiftKeyPress ) )
            {
            if( CAknFepFnKeyManager::EFnKeyNext == fepMan->FnKeyState())
                {
                fepMan->SetFnKeyState( CAknFepFnKeyManager::EFnKeyNone );
                }
            }
        }
    HandleShiftState(aKey);
    if((fepMan->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed))&& (aKey !=EStdKeyBackspace) && (aKey!=EKeyBackspace))
        {
        if ( fepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction) )
            {
            fepMan->TryRemoveNoMatchesIndicatorL();
            }      
        if (fepMan->IsAutoCompleteOn())
            {
            TInt tailLength = 0;
            iOwner->PtiEngine()->HandleCommandL( EPtiCommandGetAutoCompletionTailLength, &tailLength );
            if(tailLength > 0)
                {
				// To remove the autocompletion part, the below function is used. 
				// DeleteKeyPress was used earlier. This is removed to fix the bug :
				// enter characters to show autocompletion part. Now press a 
				// character so that the no matches character is shown. Now press
                // backspace key, last character is not deleted.
                fepMan->RemoveSuggestedAdvanceCompletionL();                
                return ETrue;
                }
            }
        // for long key press do not go to multitap base
        if( EShortKeyPress == aLength )
        	{
        	TAknFepInputStateInitialMultitapBase::HandleKeyL(aKey, aLength);
        	}        	
        else
        	{		
        	mappedCharacters.Zero();	
	        iOwner->PtiEngine()->MappingDataForKey((TPtiKey)aKey
	        					, mappedCharacters, iOwner->PtiEngine()->Case());
	        fepMan->NewCharacterL(mappedCharacters);
	        fepMan->CommitInlineEditL();        
        	}
     // cancel the multitap timer if only one character is mapped to it   
		mappedCharacters.Zero();
		
        iOwner->PtiEngine()->MappingDataForKey((TPtiKey)aKey, mappedCharacters, iOwner->PtiEngine()->Case());
        
        if( mappedCharacters.Length() < 2 )
            {
            iOwner->PtiEngine()->CancelTimerActivity(); 
            }
            
        iData = aKey;
        fepMan->ClearFlag(CAknFepManager::EFlagNoActionDuringChrKeyPress);
        return ETrue;
        }
	// for long press of punct key, a stream of characters mapped to 
	// that case should be shown both in predictive and non-predictive mode	
    if ( aLength == ELongKeyPress && aKey == EStdKeyFullStop )    
    	{
    	// Long press should always input fnlower character
    	iOwner->PtiEngine()->SetCase( EPtiCaseFnLower);
    	
    	mappedCharacters.Zero();
        iOwner->PtiEngine()->MappingDataForKey((TPtiKey)aKey
        					, mappedCharacters, iOwner->PtiEngine()->Case());
        fepMan->NewCharacterL(mappedCharacters);
        fepMan->CommitInlineEditL();        
        return ETrue;
   		}
    switch (aKey)
        {
        case EStdKeyLeftShift:
        case EStdKeyRightShift:
        case EStdKeyLeftFunc:
            {
            // Handle shift key seperately only if fn state is set.
            if( CAknFepFnKeyManager::EFnKeyPressed == fepMan->FnKeyState() ||
                      CAknFepFnKeyManager::EFnKeyDown == fepMan->FnKeyState() ||
                      CAknFepFnKeyManager::EFnKeyForced == fepMan->FnKeyState() || 
                      CAknFepFnKeyManager::EFnKeyNext == fepMan->FnKeyState() || 
                      CAknFepFnKeyManager::EFnKeyPressedAgain == fepMan->FnKeyState() ||
                      CAknFepFnKeyManager::EFnKeyLock == fepMan->FnKeyState())
                {
                fepMan->PtiEngine()->SetCase( EPtiCaseFnLower );
                TAknFepInputStateInitialMultitapBase::HandleKeyL(aKey, aLength);
				// cancel the multitap timer if only one character is mapped to it   
				mappedCharacters.Zero();
        		iOwner->PtiEngine()->MappingDataForKey((TPtiKey)aKey, mappedCharacters, iOwner->PtiEngine()->Case());
        
        		if( mappedCharacters.Length() < 2 )
            		{
            		iOwner->PtiEngine()->CancelTimerActivity(); 
            		}
                }
            }
            break;
            
        case EStdKeyNull:
            // Take no action if simulated key ends up here
            break;
            
        case EStdKeySpace: //fall through
            {
        	_LIT(KSpace," "); 
        	if( fepMan->FnKeyState() == CAknFepFnKeyManager::EFnKeyNone)               
        		{
				fepMan->StartInlineEditL();
	            fepMan->UpdateInlineEditL(KSpace,0);
	            fepMan->CommitInlineEditL();
	        	return ETrue;        		
        		}
        	else
	        	{
	            // Most keys activate the entry state and are handled there
	            fepMan->TryCloseUiL(); //expires the multitap timer
	            iOwner->ChangeState(EEntry);
	            fepMan->SendUIActivatedNotification();
	            }
        	break;	
	        }
        case EStdKeyEnter:
        case EStdKeyTab:
        case EStdKeyBackspace:
        case EStdKeyLeftArrow:
        case EStdKeyRightArrow:
        case EStdKeyUpArrow:
        case EStdKeyDownArrow:
            {
            // Navigation keys do not activate the entry state. Instead they are handled by the editor directly.
            
            // Asyncronous case update is needed after the editor has handled the key
            fepMan->SimulateKeyEventL(EKeyF19); 
            if (((aKey == EStdKeyBackspace) || (aKey == EKeyBackspace)) &&  (fepMan->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed)))
                fepMan->ClearFlag(CAknFepManager::EFlagNoActionDuringChrKeyPress);
            }
            break;
        default:        
            {
            // Most keys activate the entry state and are handled there
            fepMan->TryCloseUiL(); //expires the multitap timer
            iOwner->ChangeState(EEntry);
            fepMan->SendUIActivatedNotification();
            }
        }
    
    if ( aKey == EStdKeyEnter)
        {
        iData = 0;
        return 0;
        }
    if( !iOwner->PtiEngine()->IsValidKey((TPtiKey)aKey))
    	{
    	iData = 0;
        fepMan->ClearFlag(CAknFepManager::EFlagInsideMultitapInlineEditingTransaction);        
        fepMan->PtiEngine()->CancelTimerActivity();
                
        return retVal;
    	}
    
    iData = aKey;
    
    return retVal;
    }

void TAknFepUiInputStateInitialHalfQwertyWesternPredictive::KeyTimerExpired()
    {
    if (iData)
        {
        MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
        CAknFepCaseManager* caseMan = iOwner->CaseMan();
        
        TAknFepInputStateInitialMultitapBase::KeyTimerExpired();
        
        caseMan->UpdateCase(ENullNaviEvent); 
        fepMan->ClearFlag( CAknFepManager::EFlagInsideMultitapInlineEditingTransaction );
        
        iData = 0;
        
        // if the current fn key state is FnNext, then clear it.
        if( !fepMan->IsFlagSet( CAknFepManager::EFlagLongShiftKeyPress ) )
            {
            if( CAknFepFnKeyManager::EFnKeyNext == fepMan->FnKeyState())
                {
                // This cannot be done in fn manager. It should happen only
                // on timer expiry -> for shift key multitapping in fn mode 
                // when prediction is ON.
                fepMan->SetFnKeyState( CAknFepFnKeyManager::EFnKeyNone );
                }
            }
        }
    }
void TAknFepUiInputStateInitialHalfQwertyWesternPredictive::HandleShiftState( TInt aKey )
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    CAknFepCaseManager* caseMan = iOwner->CaseMan();
    CAknFepFnKeyManager::TFnKeyState fnKeyState  = fepMan->FnKeyState();
    TPtiTextCase shiftedCase;
    if(fnKeyState  != CAknFepFnKeyManager::EFnKeyNone)
        {
        shiftedCase = EPtiCaseFnLower;
        }
    else
        {
        shiftedCase= EPtiCaseLower; 
        }
    
    // Find out if the key is alphabetic
    TBuf<KKeyMappingsLength> keyMappings; 
    ptiengine->MappingDataForKey( (TPtiKey)aKey, keyMappings, EPtiCaseUpper );   
    TBool isAlpha = ( keyMappings.Length() && TChar(keyMappings[0]).IsAlpha() );
    
    if (isAlpha && (fnKeyState  == CAknFepFnKeyManager::EFnKeyNone))
        {
        switch ( caseMan->CurrentCase() )
            {
            case EAknEditorTextCase:
                {
                // Alphabetic characters are always capitalized in "Abc" mode 
                // unless the shift is pressed
                if ( !fepMan->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode) )
                    {
                    shiftedCase = EPtiCaseUpper;
                    }          
                }
            break;
            
            case EAknEditorLowerCase:
                {
                // Pressing shift enters uppercae letters in the "abc" mode
                if ( fepMan->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode) )
                    {
                    shiftedCase = EPtiCaseUpper;
                    }                
                }
            break;
            
            case EAknEditorUpperCase:
                {
                // In the "ABC" mode (capslock mode) the letters are uppercased
                // unless the shift is pressed
                if ( !fepMan->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode) )
                    {
                    shiftedCase = EPtiCaseUpper;
                    }                
                }
            break;
                
            default:
            break;
            }

        
        }
    else
        {
        // non-alphabetic keys are shifted if and only if shift is pressed, no matter what is the input state
        if ( fepMan->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode) && (fnKeyState  == CAknFepFnKeyManager::EFnKeyNone))
            {
            shiftedCase = EPtiCaseUpper;
            }
        else if ( fepMan->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode) && (fnKeyState  != CAknFepFnKeyManager::EFnKeyNone))
            {
            shiftedCase = EPtiCaseFnUpper;
            }    
        }

    
    // Handle the Chr key state
    if ( fepMan->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed) )
        {
        // Chr key is pressed down. Chr mode is used.
        if (shiftedCase == EPtiCaseLower)
            {
            shiftedCase = EPtiCaseChrLower;
            }
        else if (shiftedCase == EPtiCaseUpper)
            {
            shiftedCase = EPtiCaseChrUpper;
            }
        }
    
    ptiengine->SetCase(shiftedCase);
    
    fepMan->ClearFlag(CAknFepManager::EFlagNoActionDuringShiftKeyPress);
    }// End of file
