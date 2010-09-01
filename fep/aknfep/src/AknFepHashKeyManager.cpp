/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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












#include "AknFepHashKeyManager.h"
#include "AknFepManager.h"
#include "AknFepCaseManager.h"
#include "AknFepUiIndicInputManager.h"

#include <aknedsts.h>
#include <uikon.hrh>
#include <coedef.h>

#include <PtiEngine.h>

//move to class definition
#define EHashKeyTimeout 700000 // 0.7s

CAknFepHashKeyManager* CAknFepHashKeyManager::NewL(CAknFepManager& aFepMan, 
                                                   CAknFepCaseManager* aCaseMan)
    {
    CAknFepHashKeyManager* self = new(ELeave) CAknFepHashKeyManager(aFepMan, aCaseMan);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

CAknFepHashKeyManager::~CAknFepHashKeyManager()
    {
    delete iHashKeyTimer;
    }

TKeyResponse CAknFepHashKeyManager::HandleKeyEventL(TKeyPressLength aLength)
    {
    TKeyResponse response = EKeyWasNotConsumed;

    switch (iHashKeyStyle)
        {
        case EHashKeyStyleWestern:
            response = HandleKeyWesternL(aLength);
            break;
        case EHashKeyStyleChineseWithWestern:
            response = HandleKeyWesternWithChineseL(aLength);
            break;
        case EHashKeyStyleJapanese:
            response = HandleKeyJapaneseL(aLength);
            break;
        case EHashKeyStyleKoreanWithWestern:
            response = HandleKeyWesternWithKoreanL(aLength);
            break;
    default:
        break;
        }
    return response;
    }

void CAknFepHashKeyManager::SetMode(TInt aMode, TBool aWesternPredictive)
    {
    iMode = aMode;
    iWesternPredictive = aWesternPredictive;
    }

void CAknFepHashKeyManager::SetHashKeyStyle(THashKeyStyle aHashKeyStyle)
    {
    iHashKeyStyle = aHashKeyStyle;
    }

void CAknFepHashKeyManager::CancelHashKeyTimer()
    {
    if (iHashKeyTimer->IsActive())
        {
        iHashKeyTimer->Cancel();
        }
    }

CAknFepHashKeyManager::CAknFepHashKeyManager(CAknFepManager& aFepMan, CAknFepCaseManager* aCaseMan)
    :iHashKeyStyle(EHashKeyStyleWestern),
     iFepMan(aFepMan),
     iCaseMan(aCaseMan)
    {
    iLanguageChangedOrChangeFromNumberMode = EFalse;
    iIndicSelectionLoopIndex = -1; // Cannot use 'iFepMan.WesternPredictive(ELatin);' since FepMan is not completely constructed at the construction of this
    }

void CAknFepHashKeyManager::ConstructL()
    {   
    iHashKeyTimer = CPeriodic::NewL(CActive::EPriorityStandard);
    }

TInt CAknFepHashKeyManager::HashKeyTimerTimeoutCallback(TAny* aObj)
    {
    STATIC_CAST(CAknFepHashKeyManager*, aObj)->HashKeyTimerTimeout();
    return KErrNone;
    }

void CAknFepHashKeyManager::HashKeyTimerTimeout()
    {
    iHashKeyTimer->Cancel();
    }

void CAknFepHashKeyManager::RevertPredictiveTextSettingChangeL()
    {
    if ( !iFepMan.InputLanguageSupportsCaseChanges() && iFepMan.EditorState()&&
        !( iFepMan.EditorState()->Flags() & EAknEditorFlagNoT9) )
        {
        // Reverts predictive text settings change made with short key press.
        iFepMan.SetWesternPredictive( !iFepMan.WesternPredictive(ELatin) );
        iFepMan.TryChangeModeL(ELatin);
        }
    }

TKeyResponse CAknFepHashKeyManager::HandleKeyWesternL(TKeyPressLength aLength)
    {
    TKeyResponse response = EKeyWasNotConsumed;
    TBool phoneIdle = (iFepMan.EditorType() == CAknExtendedInputCapabilities::EPhoneNumberEditor); 
    // Don't handle hash key unless it comes from virtual keyboard.
    if( phoneIdle && !iFepMan.PluginInputMode() )
        {
        return response;
        }
    iLastModeNumber = (iMode == ENumber || iMode == ENativeNumber);

    if ( (iMode == ENumber || iMode == ENativeNumber) && !iFepMan.HashKeySelectionInUse())
        {
        if (aLength == ELongKeyPress)
            {
            // Clear the EExtendedFlagShortPressHashKey, only for non numeric only editors
            // so as to indicate we are in a long press of hash key event.
            // In case of numeric only editor, we should re-simulate hash and let application 
            // handle it.

            if ( !iFepMan.IsOnlyNumericPermitted() )
                {
                iFepMan.ClearExtendedFlag(CAknFepManager::EExtendedFlagShortPressHashKey);

                // Long press of hash key switches to alpha mode
                // if it is permitted mode in the editor.
                iFepMan.TryChangeToModeBeforeL();
                ResetIndicHashKeyStateL();
                }
            response = EKeyWasConsumed;
            }
        else if (!iFepMan.IsExtendedFlagSet(CAknFepManager::EExtendedFlagShortPressHashKey))
            {
            // Set the flag to indicate its a short press of key event
            iFepMan.SetExtendedFlag(CAknFepManager::EExtendedFlagShortPressHashKey);
            response = EKeyWasConsumed;
            }
        else
            {
            // We are not suppose to be here. Just a fail safe check to guard us against
            // erroneous situations.
            
            iFepMan.ClearExtendedFlag(CAknFepManager::EExtendedFlagShortPressHashKey);
            }
        }
    else
        {
        if ( aLength == EShortKeyPress )
            {
           	if (iFepMan.HashKeySelectionInUse())
             	{
            	// New style hash key loop when long hash-key press is used for text selection.												     			                	               	
#ifndef RD_INTELLIGENT_TEXT_INPUT
		        // To rip off suggested word completion when user wants to 
		        // switch to number or multitap mode
		        if(iFepMan.IsAutoCompleteOn())
		        	{
		            iFepMan.RemoveSuggestedCompletionL();	
		            }
#endif	//RD_INTELLIGENT_TEXT_INPUT
               	return HandleKeyWesternSelectionStyleL(aLength);  
               	}
               	
 			if ( iLastModeNumber )
            	{
            	// The key should not be consumed in number mode? FEP Manager may
            	// put special chars into the editor or the key  event 
            	// is passed to the editor.

	            // But now the event is consumed. Same happens also in 
    	        // Chinese languages because # key is reserved for 
        	    // other purpose.
            	response = EKeyWasConsumed;
           		 }               	
            
			if(IsIndicHashKeyStyleRequired())
				{
				return (SetIndicStateL());	
				}
            	// Traditional hash key loop
            if ( iFepMan.InputLanguageSupportsCaseChanges() )
                {
                if (iHashKeyTimer->IsActive())
                    {
                    iHashKeyTimer->Cancel();
                    iCaseMan->RevertCaseChange();
                    // Toggle predictive setting.
#ifndef RD_INTELLIGENT_TEXT_INPUT
		         // To rip off suggested word completion when user wants to 
		         // switch to number or multitap mode
		        if(iFepMan.IsAutoCompleteOn())
		        	{
		            iFepMan.RemoveSuggestedCompletionL();	
		            }

#endif	//RD_INTELLIGENT_TEXT_INPUT
                    iFepMan.SetCcpuFlag(CAknFepManager::ECcpuStataCommitPredictiveWord);
                    iFepMan.SetWesternPredictive( !iFepMan.WesternPredictive(ELatin) );
                    iFepMan.TryChangeModeL(ELatin);
                    }
                else
                    {
                    iCaseMan->ManualCaseUpdate();
                    iHashKeyTimer->Start(EHashKeyTimeout, EHashKeyTimeout,
                        TCallBack(HashKeyTimerTimeoutCallback, this));
                    }
                }
            else // Case changes are not allowed.
                {
                if (iHashKeyTimer->IsActive())
                    {
                    iHashKeyTimer->Cancel();
                    // Double press of hash key does not have functionality.
                    }
                else
                    {
                    // Toggle predictive setting.
#ifndef RD_INTELLIGENT_TEXT_INPUT
                    // AutoComplete - Fix - Begin
                    if(iFepMan.IsAutoCompleteOn())
		        	{
		            iFepMan.RemoveSuggestedCompletionL();	
		            }
		            // AutoComplete - Fix - End

#endif	//RD_INTELLIGENT_TEXT_INPUT
                    iFepMan.SetWesternPredictive( !iFepMan.WesternPredictive(ELatin) );
                    iFepMan.TryChangeModeL(ELatin);
                    iHashKeyTimer->Start(EHashKeyTimeout, EHashKeyTimeout,
                        TCallBack(HashKeyTimerTimeoutCallback, this));
                    }
                }
            response = EKeyWasConsumed;
            }
        else // Long key press of hash key
            {
            // Clear EExtendedFlagShortPressHashKey flag as soon as long press happens
            iFepMan.ClearExtendedFlag(CAknFepManager::EExtendedFlagShortPressHashKey);
            
            // Toggle number mode / alpha mode
            if (iLastModeNumber && !IsIndicHashKeyStyleRequired() && 
               !iFepMan.HashKeySelectionInUse())	// For Indic it is handled in SetIndicPreviousStateL()
                {
                // If short hash key press event is passed to the editor, it
                // needs to be removed here from the editor.

                iFepMan.TryChangeToModeBeforeL();
                }
            else
                {
                // Input mode is incremented.
                if (!iFepMan.HashKeySelectionInUse())
                	{
#ifndef RD_INTELLIGENT_TEXT_INPUT
                	// To rip off suggested word completion when user wants to switch to number mode
                	if(iFepMan.IsAutoCompleteOn())
                		{
                		iFepMan.RemoveSuggestedCompletionL();	
                		}

#endif	//RD_INTELLIGENT_TEXT_INPUT
						if(IsIndicHashKeyStyleRequired())
							{
							SetIndicPreviousStateL();	
							}					    	
						else
							{
							RevertPredictiveTextSettingChangeL();			
							}                	
                        if( iFepMan.IsSupportNativeNumber())
                            {
                            iFepMan.TryChangeModeL( ENativeNumber );  
                            }
                        else
                            {
                            iFepMan.TryChangeModeL( ENumber );    
                            }
                	}
				else if (!iFepMan.IsCcpuFlagSet(CAknFepManager::ECcpuStateEdwinInSelectionMode) &&
				         iFepMan.OkToActivateSelectionMode())							
					{											
					// User wants to select and not to change mode, so cancel the mode change
					// caused by initial hash key press.
					if(iLastModeNumber && !IsIndicHashKeyStyleRequired())
						{
						iFepMan.TryChangeToModeBeforeL();		
						}
					else
						{
						SetPreviousSelectionStyleModeL();		
						}
					iFepMan.SetCcpuFlag(CAknFepManager::ECcpuStateHashDown |
					                    CAknFepManager::ECcpuStateEdwinInSelectionMode |
					                    CAknFepManager::ECcpuStateHashKeyDeleteDone );
					 TKeyEvent ccpuStart = {EKeyF21, EStdKeyF21, 0, 0};
        			// to enable copy/paste support on cba. We simulate via CCoeEnv
        			// to avoid fep SimulateKeyEventL adding shift modifiers
        			CCoeEnv::Static()->SimulateKeyEventL(ccpuStart, EEventKey);					
					}                	
                }            
            response = EKeyWasConsumed;
            }
        }              
    return response;
    }


TKeyResponse CAknFepHashKeyManager::HandleKeyWesternWithChineseL(TKeyPressLength aLength)
    {
    TKeyResponse response = EKeyWasNotConsumed;
    if ( iFepMan.IsOnlyNumericPermitted() )
        {
        if ( aLength == ELongKeyPress )
            {
            response = EKeyWasConsumed;
            if ( iFepMan.HashKeySelectionInUse() )
                {
                TText prevCh = iFepMan.PreviousChar();
                if (prevCh == '#')
                    {
                    iFepMan.RemovePreviousCharacterL();             
                    }
                }
            
    		iFepMan.SetCcpuFlag( CAknFepManager::ECcpuStateHashDown | 
    		                    CAknFepManager::ECcpuStateEdwinInSelectionMode );
    		TKeyEvent ccpuStart = { EKeyF21, EStdKeyF21, 0, 0 };
    		// to enable copy/paste support on cba. We simulate via CCoeEnv
    		// to avoid fep SimulateKeyEventL adding shift modifiers
    		CCoeEnv::Static()->SimulateKeyEventL( ccpuStart, EEventKey );
            }
        //else let the FEP Man put special chars into the editor
        }
    else
        {
        if (aLength == EShortKeyPress)
            {
            if (iMode == ENumber || iMode == ENativeNumber)
                {
                iLastModeNumber = ETrue;
                }
            else
                {
                iLastModeNumber = EFalse;
                }            
            CAknEdwinState* editorState = iFepMan.EditorState();                    
            TInt permittedCases = editorState->PermittedCases();
            permittedCases = permittedCases == 0 ? 
                EAknEditorLowerCase | EAknEditorUpperCase : permittedCases;
            if ( iMode == ELatin && (permittedCases & EAknEditorTextCase))
                {
                // Latin case update is changed back to automatic if latin 
                // text case is allowed and the case is changed manually from #-key.
                iFepMan.ClearFlag(CAknFepManager::EFlagSupressAutoUpdate);
                }
            TInt currentCase = iCaseMan->CurrentCase();

            // Is current latin case last available case.
            TBool lastCase = ( ( !(permittedCases & EAknEditorTextCase) && 
                  (( currentCase == EAknEditorLowerCase) ||
                   ( currentCase == EAknEditorUpperCase && 
                   !(permittedCases & EAknEditorLowerCase)) ) ||
                ( editorState->Flags() & EAknEditorFlagFixedCase) ) ||
                ( editorState->Flags() & EAknEditorFlagForceTransparentFepModes) );
            
            if (iMode == ELatin && 
                !iFepMan.IsFlagSet(CAknFepManager::EFlagChangeInputMode) &&
                !lastCase )
                {
                // Input mode is still Latin. Only case is updated.
                if (permittedCases & EAknEditorTextCase) 
                    {
                    iCaseMan->ManualCaseUpdate();
                    }
                else
                    {
                    iCaseMan->SetCurrentCase(EAknEditorLowerCase);
                    }

                // Next time input mode is changed if any text is not entered before.
                iFepMan.SetFlag(CAknFepManager::EFlagChangeInputMode);
                }
            else
                {
                // Input mode is incremented.
                iFepMan.TryIncrementModeL(iMode);

                iFepMan.ClearFlag(CAknFepManager::EFlagChangeInputMode);

                if (iMode == ELatin) // New mode after Increment.
                    {
                    if (editorState->Flags() & EAknEditorFlagFixedCase) 
                        {
                        // Only one fixed case is available. Use it.
                        iCaseMan->ConfigureCaseStateFromEditorState();
                        iFepMan.SetFlag(CAknFepManager::EFlagChangeInputMode);
                        }
                    else if (editorState->Flags() & EAknEditorFlagForceTransparentFepModes) 
                        {
                        // Only lower case is used with find pane.
                        iCaseMan->SetCurrentCase(EAknEditorLowerCase);
                        iFepMan.SetFlag(CAknFepManager::EFlagSupressAutoUpdate);
                        iFepMan.SetFlag(CAknFepManager::EFlagChangeInputMode);
                        }
                    else if (permittedCases & EAknEditorTextCase)
                        {
                        iCaseMan->SetCurrentCase(EAknEditorTextCase);
                        iCaseMan->UpdateCase(ENullNaviEvent);
                        }
                    else
                        {
                        if (permittedCases & EAknEditorUpperCase)
                            {
                            iCaseMan->SetCurrentCase(EAknEditorUpperCase);
                            if ( !(permittedCases & EAknEditorLowerCase))
                                {
                                // We need to move to next input mode since only upper case 
                                // is permitted in Latin input mode.
                                iFepMan.SetFlag(CAknFepManager::EFlagChangeInputMode);
                                }
                            }
                        else
                            {
                            iCaseMan->SetCurrentCase(EAknEditorLowerCase);
                            iFepMan.SetFlag(CAknFepManager::EFlagChangeInputMode);
                            }
                        }
                    }
                }
            }
        else // Long keypress
            {
			if (!iFepMan.HashKeySelectionInUse())            
				{							
            	if (iLastModeNumber)
                	{
                	iFepMan.TryChangeToModeBeforeL();
                	}
            	else
                	{
                	if( iFepMan.IsSupportNativeNumber())
                	    {
                	    iFepMan.TryChangeModeL( ENativeNumber );
                	    }
                	else
                	    {
                	    iFepMan.TryChangeModeL(ENumber);
                	    }
                	}
				}
			else if (!iFepMan.IsCcpuFlagSet(CAknFepManager::ECcpuStateEdwinInSelectionMode) &&
			         iFepMan.OkToActivateSelectionMode())							
				{							
				// User wants to select and not to change mode, so cancel the mode change
				// caused by initial hash key press. 			
				MPtiLanguage* lang = iFepMan.PtiEngine()->CurrentLanguage();
				TInt inputLang = ELangTest;
				if (lang)
					{
					inputLang = lang->LanguageCode();	
					}
				const TInt inputMode = iFepMan.InputMode();				
				
				if ((inputLang == ELangPrcChinese && inputMode == EPinyin) ||
				    (inputLang == ELangTaiwanChinese && inputMode == EZhuyin) ||
				    (inputLang == ELangHongKongChinese && inputMode == EStroke))												
					{
					// Hash key selection was initiated from number mode,
					// return there.
					iFepMan.TryChangeModeL(ENumber);						
					}
				else
				    {
			   		iFepMan.TryChangeToModeBeforeL();							
					}
				   			                
				iFepMan.SetCcpuFlag(CAknFepManager::ECcpuStateHashDown | 
				                    CAknFepManager::ECcpuStateEdwinInSelectionMode );
				 TKeyEvent ccpuStart = {EKeyF21, EStdKeyF21, 0, 0};
        		// to enable copy/paste support on cba. We simulate via CCoeEnv
        		// to avoid fep SimulateKeyEventL adding shift modifiers
        		CCoeEnv::Static()->SimulateKeyEventL(ccpuStart, EEventKey);					
				}                		
            }
        response = EKeyWasConsumed;
        }
    return response;
    }


TKeyResponse CAknFepHashKeyManager::HandleKeyJapaneseL(TKeyPressLength aLength)
    {
    TKeyResponse response = EKeyWasNotConsumed;
    if (aLength == ELongKeyPress)
        {
        response = HandleLongKeypressJapaneseL();
        }
    else
        {
        response = HandleSortKeypressJapaneseL();
        }
    return response;
    }

TKeyResponse CAknFepHashKeyManager::HandleLongKeypressJapaneseL()
    {
    TKeyResponse response = EKeyWasConsumed;

    if (iFepMan.IsOnlyNumericPermitted())
        {
        if (iFepMan.HashKeySelectionInUse())
            {
            TText prevCh = iFepMan.PreviousChar();
            if (prevCh == '#')
                {
                iFepMan.RemovePreviousCharacterL();
                }
            }
        }

    if (iMode == EHiraganaKanji || iMode == ELatin)
        {
        // predictive input is turned on/off
        // Commit string on transitory input
        iFepMan.TryCloseUiL();

        if(iWesternPredictive)
            {
            iFepMan.TryChangePredictiveInputModeL(EFalse);
            }
        else
            {
            iFepMan.TryChangePredictiveInputModeL(ETrue);
            }
        iFepMan.SetCcpuFlag(CAknFepManager::ECcpuStateChangeToPredictionMode);
        }
    else if (iMode == EHiragana)
        {
        // nothing to do
        }
    else
        {
        // Input mode is incremented.
        iFepMan.TryIncrementModeL(iMode);
        if ( iMode == ELatin )
            {
            if (iCaseMan->IsAbleChangeCase())
                {
                CAknEdwinState* editorState = iFepMan.EditorState();
                TInt permittedCases = editorState->PermittedCases();
                if (permittedCases & EAknEditorTextCase)
                    {
                    iCaseMan->SetCurrentCase(EAknEditorTextCase);
                    iCaseMan->UpdateCase(ENullNaviEvent);
                    }
                else
                    {
                    if ( iFepMan.CaseBefore() )
                        {
                        iCaseMan->SetCurrentCase(iFepMan.CaseBefore());
                        }
                    else if (permittedCases & EAknEditorUpperCase)
                        {
                        iCaseMan->SetCurrentCase(EAknEditorUpperCase);
                        if ( !(permittedCases & EAknEditorLowerCase))
                            {
                            // We need to move to next input mode since only upper case 
                            // is permitted in Latin input mode.
                            iFepMan.SetFlag(CAknFepManager::EFlagChangeInputMode);
                            }
                        }
                    else
                        {
                        iCaseMan->SetCurrentCase(EAknEditorLowerCase);
                        iFepMan.SetFlag(CAknFepManager::EFlagChangeInputMode);
                        }
                    }
                }
            }
        iFepMan.SetCcpuFlag(CAknFepManager::ECcpuStateChangeToPredictionMode);
        }

    if (iFepMan.HashKeySelectionInUse()
     && !iFepMan.IsCcpuFlagSet(CAknFepManager::ECcpuStateEdwinInSelectionMode)
     && iFepMan.OkToActivateSelectionMode())
        {
        // User wants to select and not to change mode, so cancel the mode change
        // caused by initial hash key press.
        SetPreviousSelectionStyleModeL();

        iFepMan.SetCcpuFlag(CAknFepManager::ECcpuStateHashDown | 
                            CAknFepManager::ECcpuStateEdwinInSelectionMode );
         TKeyEvent ccpuStart = {EKeyF21, EStdKeyF21, 0, 0};
        // to enable copy/paste support on cba. We simulate via CCoeEnv
        // to avoid fep SimulateKeyEventL adding shift modifiers
        CCoeEnv::Static()->SimulateKeyEventL(ccpuStart, EEventKey);
        }

    return response;
    }

TKeyResponse CAknFepHashKeyManager::HandleSortKeypressJapaneseL()
    {
    TKeyResponse response = EKeyWasConsumed;
    if (iFepMan.IsOnlyNumericPermitted())
        {
        response = EKeyWasNotConsumed;
        }
    else if (iMode == EHiragana)
        {
        response = EKeyWasConsumed;
        }
    else if (iFepMan.HashKeySelectionInUse()
          && iPreviousSelection)
        {
        // User wants to select and not to change mode, so not change to input mode
        response = EKeyWasConsumed;
        iPreviousSelection = EFalse;
        }
    else
        {
        CancelHashKeyTimer();
        if ( iMode == ELatin && iCaseMan->IsAbleChangeCase())
            {
            // Input mode is incremented.
            iFepMan.TryIncrementModeL(iMode);
            if (iFepMan.IsQwerty())
                {
                if ( iMode == ELatin )
                    {
                    iFepMan.TryIncrementModeL(ENumber);
                    }
                }
            }
        else if ( iMode == ELatin && !iCaseMan->IsAbleChangeCase() && iFepMan.IsQwerty())
            {
            // In Qwerty input and not be able to change case mode,
            // Number mode must be skipped.
            iFepMan.TryIncrementModeL(ENumber);
            }
        else
            {
            // Input mode is incremented.
            iFepMan.TryIncrementModeL(iMode);
            if ( iMode == ELatin )
                {
                if (iCaseMan->IsAbleChangeCase())
                    {
                    CAknEdwinState* editorState = iFepMan.EditorState();
                    TInt permittedCases = editorState->PermittedCases();
                    if (permittedCases & EAknEditorTextCase)
                        {
                        iCaseMan->SetCurrentCase(EAknEditorTextCase);
                        iCaseMan->UpdateCase(ENullNaviEvent);
                        }
                    else
                        {
                        if ( iFepMan.CaseBefore() )
                            {
                            iCaseMan->SetCurrentCase(iFepMan.CaseBefore());
                            }
                        else if (permittedCases & EAknEditorUpperCase)
                            {
                            iCaseMan->SetCurrentCase(EAknEditorUpperCase);
                            if ( !(permittedCases & EAknEditorLowerCase))
                                {
                                // We need to move to next input mode since only upper case 
                                // is permitted in Latin input mode.
                                iFepMan.SetFlag(CAknFepManager::EFlagChangeInputMode);
                                }
                            }
                        else
                            {
                            iCaseMan->SetCurrentCase(EAknEditorLowerCase);
                            iFepMan.SetFlag(CAknFepManager::EFlagChangeInputMode);
                            }
                        }
                    }
                }
            }
        }

    return response;
    }
        
                
enum
	{
	EHashPredictionBit = 0x01,
    EHashTextCaseBit   = 0x02,
    EHashUpperCaseBit  = 0x04,
    EHashLowerCaseBit  = 0x08,
    EHashNumberModeBit = 0x10,    	    	    	
    };
        	
enum
	{
	EHashLatinLowerMode = EHashLowerCaseBit,
	EHashLatinUpperMode = EHashUpperCaseBit,
	EHashLatinTextMode  = EHashTextCaseBit,
	EHashPredLowerMode  = EHashLowerCaseBit | EHashPredictionBit,
	EHashPredUpperMode  = EHashUpperCaseBit | EHashPredictionBit,
	EHashPredTextMode   = EHashTextCaseBit | EHashPredictionBit		
	};        	
        	        
enum
	{
	EHashIndicMultitapMode,
	EHashIndicPredictiveMode,
#ifdef RD_HINDI_PHONETIC_INPUT	
	EHashIndicPhoneticLowerMode,
	EHashIndicPhoneticUpperMode,
#endif 	
	EHashIndicLatinTextMode,
	EHashIndicLatinLowerMode,
	EHashIndicLatinUpperMode,
	EHashNumberMode

	};        
// Here we enumerate possible mode sequences for selection style has key loop. 
// Number mode is handled as a special case. This only consumes 24 bytes.  
const TInt KNumModesInLoop = 4;  
const TUint8 allHashKeyLoops[] = 
	{
	EHashLatinLowerMode, EHashLatinUpperMode, EHashPredLowerMode, EHashPredUpperMode, 
	EHashLatinUpperMode, EHashLatinLowerMode, EHashPredUpperMode, EHashPredLowerMode, 
	EHashLatinTextMode,  EHashLatinLowerMode, EHashPredTextMode,  EHashPredLowerMode,
	EHashPredLowerMode, EHashPredUpperMode, EHashLatinLowerMode, EHashLatinUpperMode,
	EHashPredUpperMode, EHashPredLowerMode, EHashLatinUpperMode, EHashLatinLowerMode,
	EHashPredTextMode,  EHashPredLowerMode, EHashLatinTextMode,  EHashLatinLowerMode
	};

    
TKeyResponse CAknFepHashKeyManager::HandleKeyWesternSelectionStyleL(TKeyPressLength aLength)    
	{
    TKeyResponse response = EKeyWasNotConsumed;
	
	if (aLength == EShortKeyPress)
		{		
		if (iFepMan.IsOnlyNumericPermitted()) 
			{
			if (!iFepMan.IsExtendedFlagSet(CAknFepManager::EExtendedFlagShortPressHashKey))
			    {
			    // Set the flag to indicate its a short press of key event
			    iFepMan.SetExtendedFlag(CAknFepManager::EExtendedFlagShortPressHashKey);
			    response = EKeyWasConsumed;
			    }
            else
                {
			    iFepMan.ClearExtendedFlag(CAknFepManager::EExtendedFlagShortPressHashKey);
			    }
			return response;
			}
			
		if(IsIndicHashKeyStyleRequired())
			{
			return (SetIndicStateL());	
			}   			
   			
		if (!iSelectionLoop)
			{
			TInt index = 0;
			if (iFepMan.WesternPredictive(ELatin))
				{
				index += (3 * KNumModesInLoop);
				}
				
			switch (iCaseMan->CurrentCase())	
				{
				case EAknEditorUpperCase:
					 index += KNumModesInLoop;
					 break;				
				case EAknEditorTextCase:
					 index += (2 * KNumModesInLoop);
					 break;		
				default:
				     break;						 							 
				}				
				
			iSelectionLoop = &allHashKeyLoops[index];
			
			if (iMode == ENumber || iMode == ENativeNumber)				
				{
				iSelectionLoopIndex = -1;
				}
			}
			
		if (!iFepMan.InputLanguageSupportsCaseChanges())
			{
			// Jumping by 2 always ignores case change.
			iSelectionLoopIndex += 2;			
			}
		else 
			{
			iSelectionLoopIndex++;
			}
			
		if (iSelectionLoopIndex >= 0 && (iSelectionLoop[iSelectionLoopIndex] & EHashPredictionBit) != 0)
			{			
			MPtiLanguage* curLang = iFepMan.PtiEngine()->CurrentLanguage();
			if(!iFepMan.InputLanguageSupportsCaseChanges())
				{
				MCoeFepAwareTextEditor* edit = iFepMan.FepAwareTextEditor();
			    if(edit && edit->DocumentLengthForFep() > 0)
			        {
			        edit->GetCursorSelectionForFep(iPreviousIndicCursorPosition);
			        }
				
				}
			if ((curLang && curLang->HasInputMode(EPtiEnginePredictive) == EFalse) ||
			    (iFepMan.EditorState()->Flags() & EAknEditorFlagNoT9))			    			    			
				{						
				// If next one would be prediction, but prediction is not allowed,
				// skip modes.
				iSelectionLoopIndex += 2;				
				}
			}
							
		if (iSelectionLoopIndex >= KNumModesInLoop)
			{
			iSelectionLoopIndex = -1;
			}
			
		SetSelectionStyleModeL();			
					
		response = EKeyWasConsumed;					
		}
			
	return response;	
	}


void CAknFepHashKeyManager::SetPreviousSelectionStyleModeL()
	{
	if(IsIndicHashKeyStyleRequired())
		{
		SetIndicPreviousStateL();	
		return;
		}		
	if (iFepMan.IsOnlyNumericPermitted() || !iSelectionLoop)
		{
        // iSelectionLoop is not used in Japanese variant,
        // so, it keeps previous selection.
        if (!iSelectionLoop)
            {
            iPreviousSelection = ETrue;
            }
		return;		
		}
		
	if (!iFepMan.InputLanguageSupportsCaseChanges())
		{
		// Jumping by 2 always ignores case change.
		iSelectionLoopIndex -= 2;			
		if (iSelectionLoopIndex >= 0 && 
	       (iSelectionLoop[iSelectionLoopIndex] & EHashPredictionBit) == 0 )
			{
		
			MCoeFepAwareTextEditor* edit = iFepMan.FepAwareTextEditor();
		    if(edit && edit->DocumentLengthForFep() > 0)
		        {
		        edit->SetCursorSelectionForFepL(iPreviousIndicCursorPosition);
		        }
			}
		}
	else 
		{
		iSelectionLoopIndex--;
		}		
		
	if (iSelectionLoopIndex >= 0 && 
	    (iSelectionLoop[iSelectionLoopIndex] & EHashPredictionBit) != 0 &&
		(iFepMan.EditorState()->Flags() & EAknEditorFlagNoT9))			
		{
		// If next one would be prediction, but prediction is not allowed,
		// skip modes.
		iSelectionLoopIndex -= 2;				
		}
		
	if (iSelectionLoopIndex < -1)
		{
		// Jump over to the end of list
		iSelectionLoopIndex = KNumModesInLoop - 1;	
		}
		
	SetSelectionStyleModeL();		
	}
	
	
	
void CAknFepHashKeyManager::SetSelectionStyleModeL()
	{
	TBool wasPredictive = iFepMan.WesternPredictive();
	TBool willBePredictive = EFalse;
	
	if (iSelectionLoopIndex == -1)	
		{
		// Set number mode.
		if( iFepMan.IsSupportNativeNumber())
		    {
		    iFepMan.TryChangeModeL( ENativeNumber );
		    }
		else
		    {
		    iFepMan.TryChangeModeL(ENumber);
		    }
		}
	else
		{		
		willBePredictive = (iSelectionLoop[iSelectionLoopIndex] & EHashPredictionBit) != 0;		
		iFepMan.SetWesternPredictive(willBePredictive);
		if (iSelectionLoop[iSelectionLoopIndex] & EHashLowerCaseBit)	
			{
         	iCaseMan->SetCurrentCase(EAknEditorLowerCase, ETrue);			
			}									
		else if (iSelectionLoop[iSelectionLoopIndex] & EHashUpperCaseBit)			
			{	
			// Let case manager to decide whether we use text case or
			// upper case.
			CAknEdwinState* editorState = iFepMan.EditorState();
    		TInt permittedCases = EAknEditorAllCaseModes;
    		if(editorState)
    			permittedCases = editorState->PermittedCases();
    		if (!(permittedCases&EAknEditorTextCase ) || 
              iCaseMan->CapsLockRequired(EAknEditorLowerCase))    		
    			{    			    			
	   	        iCaseMan->SetCurrentCase(EAknEditorUpperCase, ETrue);			    			    			
    			}
			else
				{
	       	    iCaseMan->SetCurrentCase(EAknEditorTextCase, ETrue);							
				}    						
			}
		else
			{
       	    iCaseMan->SetCurrentCase(EAknEditorTextCase, ETrue);						
			} 
		iFepMan.SetFlag(CAknFepManager::EFlagSupressAutoUpdate);	
		if (!(wasPredictive && willBePredictive))
			{
			// Do not do this if this is just case change inside predictive mode,
			// otherwise active word will be committed.
			iFepMan.SetCcpuFlag(CAknFepManager::ECcpuStataCommitPredictiveWord);
			iFepMan.TryChangeModeL(ELatin);  									
			}
		iFepMan.ClearFlag(CAknFepManager::EFlagSupressAutoUpdate);		
		}			
	}
void CAknFepHashKeyManager::UpdateCaseForIndicHashLoop()	
	{
	switch (iCaseMan->CurrentCase())	
		{
		case EAknEditorUpperCase:
			 iIndicSelectionLoopIndex = EHashIndicLatinUpperMode;
			 break;				
		case EAknEditorTextCase:
			 iIndicSelectionLoopIndex = EHashIndicLatinTextMode;
			 break;	
		case EAknEditorLowerCase:
			 iIndicSelectionLoopIndex = EHashIndicLatinLowerMode;
			 break;	
		default:
		     break;						 							 
		}	
	}


TBool CAknFepHashKeyManager::IsIndicHashKeyStyleRequired()
	{
	if(TAknFepUiIndicInputManager::IsIndicLangauge(iFepMan.GetInputLanguageFromSharedDataInterface())
#ifdef RD_HINDI_PHONETIC_INPUT	
	   || TAknFepUiIndicInputManager :: IsIndicPhoneticLanguage(
             iFepMan.GetInputLanguageFromSharedDataInterface())
#endif	   
	   )
		{
		CAknEdwinState* editorState = iFepMan.EditorState();
		if (editorState && !(editorState->Flags() & EAknEditorFlagLatinInputModesOnly))
			{
			return ETrue;
			}
		}
	return EFalse;	
	}


TKeyResponse CAknFepHashKeyManager::SetIndicPreviousStateL()
	{
	TKeyResponse response = EKeyWasNotConsumed;
	if (iFepMan.IsOnlyNumericPermitted()) 
		{
		return response;
		}
	switch(iIndicSelectionLoopIndex)
		{
		case EHashIndicMultitapMode:
		case EHashIndicPredictiveMode:	// fall through
				if(iLanguageChangedOrChangeFromNumberMode)
    				{
			 		if(iFepMan.HashKeySelectionInUse())
    					{
    					iIndicSelectionLoopIndex = EHashNumberMode;
    					}
    				else
    					{
						iFepMan.ChangeInputLanguageL(ELangEnglish);
    					if(iIndicSelectionLoopIndex)	// if hindi predictive is on, turn off for english
					 		{
					 		iFepMan.SetWesternPredictive(EFalse);
					 		iFepMan.TryChangeModeL(ELatin);		        	
					 		}
    					UpdateCaseForIndicHashLoop();
    					}
    				}
    			else
    				{
				    // Reverts predictive text settings change made with short key press.
					if(iIndicSelectionLoopIndex)	// Meaning predictive state	
			    		{
						iIndicSelectionLoopIndex = EHashIndicMultitapMode;	// change to multitap
						MCoeFepAwareTextEditor* edit = iFepMan.FepAwareTextEditor();
					    if(edit && edit->DocumentLengthForFep() > 0)
					        {
					        edit->SetCursorSelectionForFepL(iPreviousIndicCursorPosition);
					        }
			    		}
			    	else
			    		{
						iIndicSelectionLoopIndex = EHashIndicPredictiveMode;	// change to predictive
			    		}
    				}
			break;
#ifdef RD_HINDI_PHONETIC_INPUT				
		case EHashIndicPhoneticLowerMode:
		case EHashIndicPhoneticUpperMode:
			if(iFepMan.WasLastKeyPressAHashKey())
	    		{
            	iFepMan.ChangeInputLanguageL(ELangEnglish);
	 		 	iFepMan.SetWesternPredictive(EFalse);
	 		 	iFepMan.TryChangeModeL(ELatin);
            	iLanguageChangedOrChangeFromNumberMode = ETrue;
	    		iIndicSelectionLoopIndex = EHashIndicLatinLowerMode;
	    		}
		    else
		    	{
		    	if(iIndicSelectionLoopIndex == EHashIndicPhoneticUpperMode)
		    		{
		    		iIndicSelectionLoopIndex = EHashIndicPhoneticLowerMode;		
		    		}
		    	else if(iIndicSelectionLoopIndex == EHashIndicPhoneticLowerMode)
		    		{
		    		iIndicSelectionLoopIndex = EHashIndicPhoneticUpperMode;				
		    		}
		    	}
			break;
#endif				
		case EHashIndicLatinTextMode:	// Text Case
		case EHashIndicLatinUpperMode: // Upper Case
		case EHashIndicLatinLowerMode: // Lower Case
			   	if(iLanguageChangedOrChangeFromNumberMode)
	    			{
		            iFepMan.ChangeInputLanguageL(iFepMan.GetInputLanguageFromSharedDataInterface());
	    			iIndicSelectionLoopIndex = iIndicPredictiveState;	
	    			}
    			else
	    			{
					if (iIndicSelectionLoopIndex == EHashIndicLatinLowerMode)
						{
						iIndicSelectionLoopIndex = EHashIndicLatinUpperMode;
						}
					else
						{
						iIndicSelectionLoopIndex = EHashIndicLatinLowerMode;	
						}
					iCaseMan->UpdateCase(ENullNaviEvent);
		    	    if(iCaseMan->CurrentCase() == EAknEditorTextCase)	
	                    {
	                    iIndicSelectionLoopIndex = EHashIndicLatinTextMode;
	                    }
	                else
	                    {
	                    iCaseMan->RevertCaseChange();
	                    } 	
	    			}
				break;
	
		case EHashNumberMode:
			if(iFepMan.HashKeySelectionInUse())
				{
				iFepMan.ChangeInputLanguageL(ELangEnglish);
		 		iFepMan.SetWesternPredictive(EFalse);
		 		iFepMan.TryChangeModeL(ELatin);		        	
				UpdateCaseForIndicHashLoop();		
				}
			break;		
		}
		SetIndicSelectionStateL();
		return EKeyWasConsumed;	
	}
    
void CAknFepHashKeyManager::ResetIndicHashKeyStateL()
	{
	if(IsIndicHashKeyStyleRequired())
		{
		TInt editorMode = iFepMan.EditorState()->CurrentInputMode();
		if(editorMode == EAknEditorNumericInputMode)
			{
			 iIndicSelectionLoopIndex = EHashNumberMode;
			}
		else
			{
			MPtiLanguage* curLang = iFepMan.PtiEngine()->CurrentLanguage();
			if(curLang && curLang->LanguageCode()!=ELangEnglish )
				{
#ifdef RD_HINDI_PHONETIC_INPUT					
				if(iFepMan.IsIndicPhoneticInputLanguage())
		    	 	{
		    	 	if(iCaseMan->CurrentCase() == EAknEditorUpperCase)
		    	 		iIndicSelectionLoopIndex = EHashIndicPhoneticUpperMode;
		    	 	else
		    	 		iIndicSelectionLoopIndex = EHashIndicPhoneticLowerMode;
		    	 	}
		    	 else
	    	 		{
#endif	    	 		
    			iIndicPredictiveState = iIndicSelectionLoopIndex = iFepMan.WesternPredictive(ELatin);
#ifdef RD_HINDI_PHONETIC_INPUT		    	 		
	    	 		}	
#endif	    	 		
				}
			else
				{
				UpdateCaseForIndicHashLoop();	
				}		
			}
		}
	}
    
TKeyResponse CAknFepHashKeyManager::SetIndicStateL()
	{
	TKeyResponse response = EKeyWasNotConsumed;
	
	if (iFepMan.IsOnlyNumericPermitted()) 
		{
		return response;
		}
	
	iLanguageChangedOrChangeFromNumberMode = EFalse;
    if(iIndicSelectionLoopIndex == -1)	
    	{
    	iIndicPredictiveState = iIndicSelectionLoopIndex = 
        	    (iFepMan.WesternPredictive(ELatin) && 
    	        (!( iFepMan.EditorState()->Flags() & EAknEditorFlagNoT9)));
    	// Cannot be done in constructor as FepMan is not completely initialized
    	}
	
	switch(iIndicSelectionLoopIndex)
		{
		case EHashIndicMultitapMode:
		case EHashIndicPredictiveMode:	//fall through
		
		    		iIndicPredictiveState = iIndicSelectionLoopIndex; // 0 for multitap and 1 for predictive	
					if(iFepMan.WasLastKeyPressAHashKey() || 
					    ( iFepMan.EditorState()->Flags() & EAknEditorFlagNoT9))
			    		{
		            	//iFepMan.ChangeInputLanguageL(ELangEnglish);
			 		 	iFepMan.SetWesternPredictive(EFalse);
			 		 	iFepMan.TryChangeModeL(ELatin);
		            	iLanguageChangedOrChangeFromNumberMode = ETrue;
			    		iIndicSelectionLoopIndex = EHashIndicLatinLowerMode;
			    		iCaseMan->UpdateCase(ENullNaviEvent);
			    	    if(iCaseMan->CurrentCase() == EAknEditorTextCase)	
		                    {
		                    iIndicSelectionLoopIndex = EHashIndicLatinTextMode;
		                    }
		                else
		                    {
		                        iCaseMan->RevertCaseChange();
		                    }    
			    		}
				    else
				    	{
				    	if(iIndicSelectionLoopIndex)	// Meaning predictive state	
				    		{
							iIndicSelectionLoopIndex = EHashIndicMultitapMode;	// change to multitap
				    		}
				    	else
				    		{
							if(!( iFepMan.EditorState()->Flags() & EAknEditorFlagNoT9))
							    {
							    iIndicSelectionLoopIndex = EHashIndicPredictiveMode;	// change to predictive
							    MCoeFepAwareTextEditor* edit = iFepMan.FepAwareTextEditor();
						        if(edit && edit->DocumentLengthForFep() > 0)
						            {
						            edit->GetCursorSelectionForFep(iPreviousIndicCursorPosition);
						            }
						        }
				    		}		
				    	}
			break;
#ifdef RD_HINDI_PHONETIC_INPUT				
		case EHashIndicPhoneticLowerMode:
		case EHashIndicPhoneticUpperMode:
			if(iFepMan.WasLastKeyPressAHashKey())
	    		{
            	//iFepMan.ChangeInputLanguageL(ELangEnglish);
	 		 	iFepMan.SetWesternPredictive(EFalse);
	 		 	iFepMan.TryChangeModeL(ELatin);
            	iLanguageChangedOrChangeFromNumberMode = ETrue;
	    		iIndicSelectionLoopIndex = EHashIndicLatinLowerMode;
	    		}
		    else
		    	{
		    	if(iIndicSelectionLoopIndex == EHashIndicPhoneticUpperMode)
		    		{
		    		iIndicSelectionLoopIndex = EHashIndicPhoneticLowerMode;		
		    		}
		    	else if(iIndicSelectionLoopIndex == EHashIndicPhoneticLowerMode)
		    		{
		    		iIndicSelectionLoopIndex = EHashIndicPhoneticUpperMode;				
		    		}
		    	}
			break;
#endif			
		case EHashIndicLatinTextMode:	// Text Case
		case EHashIndicLatinLowerMode: // Lower Case
		case EHashIndicLatinUpperMode: // Upper Case
				if(iFepMan.WasLastKeyPressAHashKey())
					{
					//iFepMan.ChangeInputLanguageL(iFepMan.GetInputLanguageFromSharedDataInterface());
			        iLanguageChangedOrChangeFromNumberMode = ETrue;
					if(iFepMan.HashKeySelectionInUse())
						{
						iIndicSelectionLoopIndex = EHashNumberMode;		// change to number
						}
					else
						{
#ifdef RD_HINDI_PHONETIC_INPUT						
						if(TAknFepUiIndicInputManager :: IsIndicPhoneticLanguage(
						      iFepMan.GetInputLanguageFromSharedDataInterface()))
							{
							iIndicSelectionLoopIndex = EHashIndicPhoneticLowerMode;	
							}
						else
							{
#endif							
							iIndicSelectionLoopIndex = iIndicPredictiveState; // change to hindi [multitap or predictive based on previous setting]	
#ifdef RD_HINDI_PHONETIC_INPUT	
							}
#endif							
						}	
					}
				else
					{
					switch (iCaseMan->CurrentCase())	
							{
							case EAknEditorUpperCase:
							case EAknEditorTextCase:	//fall through
								 iIndicSelectionLoopIndex = EHashIndicLatinLowerMode;
								 break;	
							case EAknEditorLowerCase:
								 iIndicSelectionLoopIndex = EHashIndicLatinUpperMode;
								 break;	
							default:
							     break;						 							 
							}	
					}
				break;
	
		case EHashNumberMode:
			if(iFepMan.HashKeySelectionInUse())
				{
			    iLanguageChangedOrChangeFromNumberMode = ETrue;
#ifdef RD_HINDI_PHONETIC_INPUT				    
			    if(TAknFepUiIndicInputManager :: IsIndicPhoneticLanguage(
			          iFepMan.GetInputLanguageFromSharedDataInterface()))
			    	{
			    	iIndicSelectionLoopIndex = EHashIndicPhoneticLowerMode;
			    	iFepMan.TryChangeModeL(ELatin);	
			    	}
				else
					{
#endif 					
					iIndicSelectionLoopIndex = iIndicPredictiveState; // change to hindi [multitap or predictive based on previous setting]
#ifdef RD_HINDI_PHONETIC_INPUT	
					}
#endif			
				}
			break;
			
		default:
				break;
		}

		SetIndicSelectionStateL();
		return EKeyWasConsumed;	
	}
	
	
	
	
void CAknFepHashKeyManager::SetIndicSelectionStateL()
	{
	switch(iIndicSelectionLoopIndex)
		{
		
		case EHashIndicMultitapMode:
			{
		 	 iFepMan.SetWesternPredictive(EFalse);
		 	 iFepMan.TryChangeModeL(EHindi);
			}
 			 break;	
 			 
		case EHashIndicPredictiveMode:
			 if(!( iFepMan.EditorState()->Flags() & EAknEditorFlagNoT9)) // make sure predictive is supported for editor
			 	{
			 	iFepMan.SetWesternPredictive(ETrue);
 			 	iFepMan.TryChangeModeL(EHindi);
			 	}
 			 break;
#ifdef RD_HINDI_PHONETIC_INPUT	
		case EHashIndicPhoneticLowerMode: 
			 iCaseMan->SetCurrentCase(EAknEditorLowerCase, ETrue);
			break;
			
 		case EHashIndicPhoneticUpperMode:
	  		 iCaseMan->SetCurrentCase(EAknEditorUpperCase, ETrue);
	  		break;
#endif	  			
		case EHashIndicLatinTextMode:
			 iCaseMan->SetCurrentCase(EAknEditorTextCase, ETrue);
			 break;
			 
		case EHashIndicLatinUpperMode:
			 iCaseMan->SetCurrentCase(EAknEditorUpperCase, ETrue);
			 break;
			 
		case EHashIndicLatinLowerMode:
			 iCaseMan->SetCurrentCase(EAknEditorLowerCase, ETrue);
			 break;	
	
		case EHashNumberMode:
		     if( iFepMan.IsSupportNativeNumber())
		        {
		        iFepMan.TryChangeModeL(ENativeNumber);
		        }
		     else
		        {
		        iFepMan.TryChangeModeL(ENumber);
		        }
			 break;
		default:
			 break;
		}
	}	

void CAknFepHashKeyManager::ResetPreviousSelectionStyleMode()
    {
    iPreviousSelection = EFalse;
    }
#ifdef RD_HINDI_PHONETIC_INPUT	
void CAknFepHashKeyManager::HandleSetIndicStateL()	
	{
	//calling SetIndicStateL only if currently it is in upper mode
	if(iIndicSelectionLoopIndex == EHashIndicPhoneticUpperMode)
	   SetIndicStateL();
	}
#endif

TKeyResponse CAknFepHashKeyManager::HandleKeyWesternWithKoreanL(TKeyPressLength aLength)
    {
    TKeyResponse response = EKeyWasNotConsumed;
    
    if(iMode == ELatin)
    	{
        iFepMan.SetWesternPredictive(EFalse);
        }

    if (iFepMan.IsOnlyNumericPermitted())
        {
        if (aLength == ELongKeyPress)
            {
		    TText prevCh = iFepMan.PreviousChar();
		    if (prevCh == '#')
		    	{
				iFepMan.RemovePreviousCharacterL();		    	
		    	}
	
            response = EKeyWasConsumed;
            }
        //else let the FEP Man put special chars into the editor
        }
    else
        {
        if (aLength == EShortKeyPress)
            {
		if (iMode == ENumber)
    			{
    			response = EKeyWasNotConsumed;
    			return response; 
				}   
            }
        else // Long keypress
            {             
  			if (iMode == ENumber)
                {
                iLastModeNumber = ETrue;
               	
               	TText prevCh = iFepMan.PreviousChar();
		    	if (prevCh == '#')
		    		{
					iFepMan.RemovePreviousCharacterL();		    	
		    		}                
                }
            else
                {
                iLastModeNumber = EFalse;
                }
                        
            CAknEdwinState* editorState = iFepMan.EditorState();                    
            TInt permittedCases = editorState->PermittedCases();
            permittedCases = permittedCases == 0 ? 
                EAknEditorLowerCase | EAknEditorUpperCase : permittedCases;
            if ( iMode == ELatin && (permittedCases & EAknEditorTextCase))
                {
                // Latin case update is changed back to automatic if latin 
                // text case is allowed and the case is changed manually from #-key.
                iFepMan.ClearFlag(CAknFepManager::EFlagSupressAutoUpdate);
                }
            TInt currentCase = iCaseMan->CurrentCase();

            // Is current latin case last available case.
            TBool lastCase = ( ( !(permittedCases & EAknEditorTextCase) && 
                  (( currentCase == EAknEditorLowerCase) ||
                   ( currentCase == EAknEditorUpperCase && 
                   !(permittedCases & EAknEditorLowerCase)) ) ||
                ( editorState->Flags() & EAknEditorFlagFixedCase) ) ||
                ( editorState->Flags() & EAknEditorFlagForceTransparentFepModes) );
            
            if (iMode == ELatin && 
                !iFepMan.IsFlagSet(CAknFepManager::EFlagChangeInputMode) &&
                !lastCase )
                {
                // Input mode is still Latin. Only case is updated.
                if (permittedCases & EAknEditorTextCase) 
                    {
                    iCaseMan->ManualCaseUpdate();
                    }
                else
                    {
                    iCaseMan->SetCurrentCase(EAknEditorLowerCase);
                    }

                // Next time input mode is changed if any text is not entered before.
                iFepMan.SetFlag(CAknFepManager::EFlagChangeInputMode);
                }
            else
                {
                // Do not change mode, if text selection is ongoing. 
                if(!iFepMan.IsCcpuFlagSet(CAknFepManager::ECcpuStateEdwinInSelectionMode))
                	{ 
	               	// Input mode is incremented.
	               	iFepMan.TryIncrementModeL(iMode);


	                iFepMan.ClearFlag(CAknFepManager::EFlagChangeInputMode);

	                if (iMode == ELatin) // New mode after Increment.
	                    {
	                    if (editorState->Flags() & EAknEditorFlagFixedCase) 
	                        {
	                        // Only one fixed case is available. Use it.
	                        iCaseMan->ConfigureCaseStateFromEditorState();
	                        iFepMan.SetFlag(CAknFepManager::EFlagChangeInputMode);
	                        }
	                    else if (editorState->Flags() & EAknEditorFlagForceTransparentFepModes) 
	                        {
	                        // Only lower case is used with find pane.
	                        iCaseMan->SetCurrentCase(EAknEditorLowerCase);
	                        iFepMan.SetFlag(CAknFepManager::EFlagSupressAutoUpdate);
	                        iFepMan.SetFlag(CAknFepManager::EFlagChangeInputMode);
	                        }
	                    else if (permittedCases & EAknEditorTextCase)
	                        {
	                        iCaseMan->SetCurrentCase(EAknEditorTextCase);
	                        iCaseMan->UpdateCase(ENullNaviEvent);
	                        }
	                    else
	                        {
	                        if (permittedCases & EAknEditorUpperCase)
	                            {
	                            iCaseMan->SetCurrentCase(EAknEditorUpperCase);
	                            if ( !(permittedCases & EAknEditorLowerCase))
	                                {
	                                // We need to move to next input mode since only upper case 
	                                // is permitted in Latin input mode.
	                                iFepMan.SetFlag(CAknFepManager::EFlagChangeInputMode);
	                                }
	                            }
	                        else
	                            {
	                            iCaseMan->SetCurrentCase(EAknEditorLowerCase);
	                            iFepMan.SetFlag(CAknFepManager::EFlagChangeInputMode);
	                            }
	                        }
	                    }
                    }
                }			
            }
        response = EKeyWasConsumed;
        }
    return response;
    }
// End of File
