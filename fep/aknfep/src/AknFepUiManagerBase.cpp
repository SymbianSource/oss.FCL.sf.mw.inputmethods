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
*       Provides the CAknFepUIManagerBase methods.
*
*/












// INCLUDE FILES
#include <e32keys.h>
#include <e32svr.h> // RDebug stuff...

#include "AknFepGlobalEnums.h"
#include "AknFepUiManagerBase.h"

// Western input
#include "AknFepUiInputStateInitialLatinMultitap.h"
#include "AknFepUiInputStateInitialNumber.h"
#include "AknFepUiInputStateInitialWesternPredictive.h"
#include "AknFepUiInputStateEntryWesternPredictive.h"

#include "AknFepCaseManager.h"
#include "AknFepPanic.h"

#include <aknfep.rsg>
#include <AknIndicatorContainer.h>  // CAknIndicatorContainer::TIndicatorContext
#include <PtiEngine.h> 
#include <PtiDefs.h>

// CONSTANTS
const TInt KNumberOfKeymapResources = 8;
const TInt KNumericKeymapResourceIds[KNumberOfKeymapResources] = 
    {
    R_AKNFEP_NUMBER_MODE_KEYMAP_STANDARD,
    R_AKNFEP_NUMBER_MODE_KEYMAP_PLAIN,
    R_AKNFEP_NUMBER_MODE_KEYMAP_CALCULATOR,
    R_AKNFEP_NUMBER_MODE_KEYMAP_CONVERTER,
    R_AKNFEP_NUMBER_MODE_KEYMAP_TO_FIELD,
    R_AKNFEP_NUMBER_MODE_KEYMAP_FIXED_DIALLING,
    R_AKNFEP_NUMBER_MODE_KEYMAP_SAT,
    R_AKNFEP_NUMBER_MODE_KEYMAP_SAT_HIDDEN
    };

#define EAknEditorNativeKeysNumberModeKeymap EAknEditorReservedInternalUseKeymap

/**
*  CAknFepUIManagerBase class.
* 
*/

//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::NewL
// 
// 
// ---------------------------------------------------------------------------
//
CAknFepUIManagerBase* CAknFepUIManagerBase::NewL(MAknFepManagerUIInterface* aFepMan, 
                                                 CAknFepCaseManager* aCaseMan, TLanguage aLanguage)
    {   
    CAknFepUIManagerBase* self = new(ELeave)CAknFepUIManagerBase(aFepMan, aCaseMan);
    CleanupStack::PushL(self);
    self->ConstructL(aLanguage);
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::~CAknFepUIManagerBase
// 
// 
// ---------------------------------------------------------------------------
//
CAknFepUIManagerBase::~CAknFepUIManagerBase()
    {
    CloseUI();
    delete iNumericKeymapBuffer;
    }

/**
 *  MAknFepManagerInterface class.
 * 
 */
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::SetMode
// 
// 
// ---------------------------------------------------------------------------
//
#ifdef RD_INTELLIGENT_TEXT_INPUT        
void CAknFepUIManagerBase::SetMode(TInt aMode, 
									TBool aPredictive, 
									TBool aQwertyInputMode, 
									TInt aKeyboardType)
#else
void CAknFepUIManagerBase::SetMode(TInt aMode, TBool aPredictive, TBool aQwertyInputMode)
#endif
    {
    iMode=aMode;
    iPredictive = aPredictive;
#ifdef RD_INTELLIGENT_TEXT_INPUT
    iQwertyInputMode = aQwertyInputMode; // predictive QWERTY changes (e.g. XT9)
#endif //RD_INTELLIGENT_TEXT_INPUT
    if (aQwertyInputMode 
#ifdef RD_INTELLIGENT_TEXT_INPUT
    && !aPredictive // predictive QWERTY changes (e.g. XT9): added second condition
#endif //RD_INTELLIGENT_TEXT_INPUT
	) 
        {
#ifdef RD_INTELLIGENT_TEXT_INPUT        
        if (aKeyboardType == EPtiKeyboardHalfQwerty)
            {
            ChangeState(EHalfQwerty);                        
            }
        else
            {
#endif            
        ChangeState(EQwerty);
#ifdef RD_INTELLIGENT_TEXT_INPUT                                
            }                       
#endif            
        }
    else
        {
        ChangeState(EInitial);
        }
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::SetCase
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerBase::SetCase(TCase aCase)
    {
#ifdef RD_INTELLIGENT_TEXT_INPUT
    TPtiTextCase newCase = EPtiCaseLower;
    
    TBool noResetCase = EFalse;
    
    switch( aCase )
        {
        case EUpperCase:
            {
            newCase = EPtiCaseUpper;
            }
            break;
        case ELowerCase:
            {
            newCase = EPtiCaseLower;
            }
            break;    
        case EFnKeyLowerCase:
            {
            newCase = EPtiCaseFnLower;
            }
            break;
            
        case EFnKeyUpperCase:
            {
            newCase = EPtiCaseFnUpper;
            noResetCase = ETrue;
            }
            break;
            
        default:
        break;
        }

    CAknFepFnKeyManager::TFnKeyState fnKeyState = iFepMan->FnKeyState();
#ifdef __REVERSE_FN_KEY_SUPPORTED
    // For Reverse Fn key mapping
    if(!iFepMan->IsReverseFnkeyInput())
    	{
#endif //__REVERSE_FN_KEY_SUPPORTED
    switch (fnKeyState)
        {
        case CAknFepFnKeyManager::EFnKeyLock:
        case CAknFepFnKeyManager::EFnKeyForced:
        case CAknFepFnKeyManager::EFnKeyDown:
            if( !noResetCase )
            newCase = EPtiCaseFnLower;
            break;
        case CAknFepFnKeyManager::EFnKeyNone:
        case CAknFepFnKeyManager::EFnKeyPressed:
        case CAknFepFnKeyManager::EFnKeyNext:
        case CAknFepFnKeyManager::EFnKeyPressedAgain:
        default:
            break;
        }
#ifdef __REVERSE_FN_KEY_SUPPORTED
        }
#endif //__REVERSE_FN_KEY_SUPPORTED
    PtiEngine()->SetCase( newCase );
#else 
    PtiEngine()->SetCase((aCase == EUpperCase)? EPtiCaseUpper : EPtiCaseLower);
#endif
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::SetNumberModeKeyMappingL
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerBase::SetNumberModeKeyMappingL(
     TAknEditorNumericKeymap aAknEditorNumericKeymap)
    {
    __ASSERT_DEBUG(
        (aAknEditorNumericKeymap >= 0 && aAknEditorNumericKeymap < KNumberOfKeymapResources) ||
        (aAknEditorNumericKeymap == EAknEditorAlphanumericNumberModeKeymap) ||
        (aAknEditorNumericKeymap == EAknEditorNativeKeysNumberModeKeymap), 
        AknFepPanic(EAknFepPanicIllegalNumberModeKeymapping));

#ifdef RD_INTELLIGENT_TEXT_INPUT        
    if (aAknEditorNumericKeymap == EAknEditorAlphanumericNumberModeKeymap && iNumericKeymapBuffer)
        {
        delete iNumericKeymapBuffer;
        iNumericKeymapBuffer = NULL;
        return;
        }
#endif
    
    // Fep does not handle keys when EAknEditorNativeKeysNumberModeKeymap is used.
    if (aAknEditorNumericKeymap != EAknEditorNativeKeysNumberModeKeymap && 
        aAknEditorNumericKeymap != EAknEditorAlphanumericNumberModeKeymap)
        {
        TInt keymapResourceId = KNumericKeymapResourceIds[aAknEditorNumericKeymap];
        if (iNumericKeymapResourceId != keymapResourceId || !iNumericKeymapBuffer)
            {
            delete iNumericKeymapBuffer;
            iNumericKeymapBuffer = NULL;
            iNumericKeymapBuffer = CCoeEnv::Static()->AllocReadResourceL( keymapResourceId );
            iNumericKeymapResourceId = keymapResourceId;
            }
        }
    }

const TDesC& CAknFepUIManagerBase::NumberModeKeyMapping() const
    {
    if (iNumericKeymapBuffer)
        {
        return *iNumericKeymapBuffer;
        }
    return KNullDesC;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::HandleKeyL
// 
// 
// ---------------------------------------------------------------------------
//
TBool CAknFepUIManagerBase::HandleKeyL(TInt aKey, TKeyPressLength aLength, TEventCode /*aEventCode*/)
    {
    if (!iStatePtr)
        {
        return EFalse;
        }
    
    TUIState currentState = iStatePtr->State();
    if (aLength == EShortKeyPress)
        {
        iInitialFepUIState = currentState;
        }
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__        
#ifdef RD_INTELLIGENT_TEXT_INPUT
    // Calculate and store the number of characters now and the delta with 
    // the last key press to be used when long press substitutes the initial
    // character.
    
    // This change is done for fixing the error :- Long keypress 
    // inputs the alphabet as well along with number/special character 
    // when candidate list is open.
    
    TInt curInputSeqLen = 0;
    TInt curEditorTextLen = 0;
    TInt curWordLen = 0;
    if (aLength == EShortKeyPress)
    	{
        curInputSeqLen = PtiEngine()->CurrentInputSequence().Length();
        curWordLen =  PtiEngine()->CurrentWord().Length();
        curEditorTextLen = FepMan()->DocumentLength();
    	iCaseBeforeLongKey = CaseMan()->CurrentCase();
    	}

    if ( aLength == ELongKeyPress )
        {
        if ( HandleLongKeyL( aKey ) )
            {
            return ETrue;
            }
        }
        
    TInt retVal = ETrue;
    TBool handledKey = iStatePtr->HandleKeyL(aKey, aLength);
    if(!handledKey)
        {
        if(currentState != iStatePtr->State()) //Check to see if we've changed state.
            {
            retVal = iStatePtr->HandleKeyL(aKey, aLength);
            }
        else
            {
            retVal = EFalse; // we've failed to handle it..
            }
        }
	
	// The delta is calculated after the short key press character is entered.
	// The check with EStdKeyF21 is done to fix the bug :
	// When prediction is off, keep shift key pressed down. 
	// Just before the editor enters the selection mode, 
	// do a long press of a number key. Along with the number 
	// even the alphabet is entered.
    if (aLength == EShortKeyPress && (handledKey || retVal) && (aKey!=EStdKeyF21))
   	    {
		// for the punctuation key, the autocompletion part is accepted, 
		// because of which the delta has to be calculated 
		// based on the current word length
    	if( aKey == EStdKeyFullStop && EPtiKeyboardHalfQwerty == FepMan()->KeyboardLayout())	
    	    {
    	    iInputSeqLenDeltaWithLastKey = PtiEngine()->CurrentWord().Length() - curWordLen;
    	    }
    	else
    	    {
    	    iInputSeqLenDeltaWithLastKey = PtiEngine()->CurrentInputSequence().Length() - curInputSeqLen;
    	    }
    	iEditorTextLenDeltaWithLastKey = FepMan()->DocumentLength() - curEditorTextLen;
    	// This change is done to fix the bug : Select the text
    	// using shift key. Do a long press of a key. Along 
    	// with the number even the alphabet is entered.
    	if(iEditorTextLenDeltaWithLastKey <= 0)
    		{
    		iEditorTextLenDeltaWithLastKey = iStatePtr->GetShortKeyPressTxtLength();
    		}
        }
    
    else if ( aLength == EShortKeyPress && 
            EPtiKeyboardHalfQwerty == FepMan()->KeyboardLayout() && 
            EFalse == retVal && PtiEngine()->IsValidKey((TPtiKey)aKey))
        {
        // reset the two deltas
        iInputSeqLenDeltaWithLastKey = 0;
        iEditorTextLenDeltaWithLastKey = 0;
        }

    return retVal;
#endif // RD_INTELLIGENT_TEXT_INPUT  
#else 

    if(!iStatePtr->HandleKeyL(aKey, aLength))
        {
        if(currentState != iStatePtr->State()) //Check to see if we've changed state.
            {
            return iStatePtr->HandleKeyL(aKey, aLength);
            }
        return EFalse; // we've failed to handle it..
        }
    return ETrue; //we're ok.
#endif // __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
    
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::HandleCommandL
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerBase::HandleCommandL(TInt aCommandId)
    {
    iStatePtr->HandleCommandL(aCommandId);
    }

void CAknFepUIManagerBase::HandleCommandL(TInt aCommandId, TInt /*aParam*/)
    {
    iStatePtr->HandleCommandL(aCommandId);
    }
// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::CloseUI
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerBase::CloseUI()
    {
    ChangeState(EInitial);
    }


// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::ExpireMultitapTimer
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerBase::ExpireMultitapTimer()
    {
    PtiEngine()->CancelTimerActivity();
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::IsValidNumericLongKeyPress
// 
// 
// ---------------------------------------------------------------------------
//
TBool CAknFepUIManagerBase::IsValidNumericLongKeyPress(TInt aKey) const
    {
    TBool response = EFalse;

    switch(aKey)
        {
        case EPtiKey0:
        case EPtiKey1:
        case EPtiKey2:
        case EPtiKey3:
        case EPtiKey4:
        case EPtiKey5:
        case EPtiKey6:
        case EPtiKey7:
        case EPtiKey8:
        case EPtiKey9:            
            if (iInitialFepUIState == EInitial || 
                iInitialFepUIState == EPredictiveCandidate)
                {
                response = ETrue;
                }
            break;
        default:
            break;
        }
    return response;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::AddTextToUserDictionaryL
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerBase::AddTextToUserDictionaryL(const TDesC& /*aText*/)
    {
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::GetFormatOfFepInlineText
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerBase::GetFormatOfFepInlineText(TCharFormat& /*aFormat*/, TInt& /*aNumberOfCharactersWithSameFormat*/, TInt /*aPositionOfCharacter*/) const
    {
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::SupportLanguage
// 
// 
// ---------------------------------------------------------------------------
//
TInt CAknFepUIManagerBase::SupportLanguage(TInt /*aMode*/) const
    {
    return iLanguage;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::SetInputLanguageL
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerBase::SetInputLanguageL(TLanguage aLanguage)
    {
    iLanguage = aLanguage;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::IsValidShiftKeyPress
// 
// 
// ---------------------------------------------------------------------------
//
TBool CAknFepUIManagerBase::IsValidShiftKeyPress() const
    {
    return ETrue;
    }

#ifdef RD_INTELLIGENT_TEXT_INPUT
TBool CAknFepUIManagerBase::IsValidFnKeyPress() const
    {
    return ETrue;
    }


TBool CAknFepUIManagerBase::IsValidLongChrKeyPress() const
    {
    return ETrue;
    }

#endif
// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::SetEditorContext
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerBase::SetEditorContext(TInt /*aContext*/)
    {
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::ActivateUI
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerBase::ActivateUI()
    {
    }

/**
 *  MAknFepUIManagerStateInterface class.
 * 
 */
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::State
// 
// 
// ---------------------------------------------------------------------------
//
TUIState CAknFepUIManagerBase::State() const
    {
    return iStatePtr->State();
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::ChangeState
// 
// 
// ---------------------------------------------------------------------------
//
TUIState CAknFepUIManagerBase::ChangeState(TUIState aState)
    {
//    iFepUiState = TAknFepInputStateBase(this);
#ifdef _DEBUG
    RDebug::Print(_L("UI Manager: New State: %d"), iStatePtr->State());
#endif
    return aState;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::FepMan
// 
// 
// ---------------------------------------------------------------------------
//
MAknFepManagerUIInterface* CAknFepUIManagerBase::FepMan() const
    {
    return iFepMan;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::PtiEngine
// 
// 
// ---------------------------------------------------------------------------
//
CPtiEngine* CAknFepUIManagerBase::PtiEngine() const
    {
    return iPtiEngine;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::IsValidChineseInputKey
// 
// 
// ---------------------------------------------------------------------------
//
TBool CAknFepUIManagerBase::IsValidChineseInputKey(TInt /*aKey*/) const
    {
    return ETrue;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::IsValidChineseInputKeyQwerty
// 
// 
// ---------------------------------------------------------------------------
//
TBool CAknFepUIManagerBase::IsValidChineseInputKeyQwerty(TInt /*aKey*/) const
    {
    return ETrue;
    }
    
// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::UpdateCangJieState
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerBase::UpdateCangJieState()
    {
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::IsValidChineseSymbol
// 
// 
// ---------------------------------------------------------------------------
//
TBool CAknFepUIManagerBase::IsValidChineseSymbol(TInt /*akey*/) const
	{
	return ETrue;
	}

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::IsQwertyZhuyinToneMarkKey
// 
// 
// ---------------------------------------------------------------------------
//
TBool CAknFepUIManagerBase::IsQwertyZhuyinToneMarkKey(TInt /*aKey*/) const
    {
    return ETrue;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::GetLanguage
// 
// 
// ---------------------------------------------------------------------------
//
TLanguage CAknFepUIManagerBase::GetLanguage() const
    {
    return iLanguage;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::CaseMan
// 
// 
// ---------------------------------------------------------------------------
//
CAknFepCaseManager* CAknFepUIManagerBase::CaseMan() const
    {
    return iCaseMan;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::GetLatestDeliberateSelection
// 
// 
// ---------------------------------------------------------------------------
//
TPtr CAknFepUIManagerBase::GetLatestDeliberateSelection() const
    {
    return TPtr(NULL, 0, 0);
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::ThaiSCTResourceId
// 
// 
// ---------------------------------------------------------------------------
//
TInt CAknFepUIManagerBase::ThaiSCTResourceId(TUint /*aPrewChar*/,TInt /*aKey*/)
    {
    return 0;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::CandidateArray
// 
// 
// ---------------------------------------------------------------------------
//
CDesCArrayFlat* CAknFepUIManagerBase::CandidateArray()
    {
    return NULL;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::IsPredictive
// 
// 
// ---------------------------------------------------------------------------
//
TBool CAknFepUIManagerBase::IsPredictive() const
    {
    return EFalse;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::SetInlineEditUnderlineVisibility
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerBase::SetInlineEditUnderlineVisibility(TBool /*aFlag*/)
    {
    }

/**
 *  CAknFepUIManagerBase class.
 * 
 */
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::CAknFepUIManagerBase
// 
// 
// ---------------------------------------------------------------------------
//
CAknFepUIManagerBase::CAknFepUIManagerBase(MAknFepManagerUIInterface* aFepMan, 
                                           CAknFepCaseManager* aCaseMan)
    :iMode(ENumber), 
    iFepMan(aFepMan),
    iCaseMan(aCaseMan)
    {
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::ConstructL
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerBase::ConstructL(TLanguage aLanguage)
    {   
    iPtiEngine = iFepMan->PtiEngine(); // Get PtiEngine
    iLanguage = aLanguage;
    iStatePtr = FepUIState();
    SetNumberModeKeyMappingL(EAknEditorStandardNumberModeKeymap);
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::FepUIState
// 
// 
// ---------------------------------------------------------------------------
//
TAknFepInputStateBase* CAknFepUIManagerBase::FepUIState()
    {
    return NULL;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::IsQwerty
// 
// 
// ---------------------------------------------------------------------------
//
TBool CAknFepUIManagerBase::IsQwerty() const
    {
    return EFalse;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::SetFepAwareEditorText
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerBase::SetFepAwareEditorText(const TFepInputContextFieldData& /*aIcfData*/)
    {
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::EditorMaxLength
// 
// 
// ---------------------------------------------------------------------------
//
TInt CAknFepUIManagerBase::EditorMaxLength()
    {
    return 0;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::SizeChanged
// 
// 
// ---------------------------------------------------------------------------
//
const TInt KResourceChanged =  1;
void CAknFepUIManagerBase::ResourceChanged(TInt /*aType*/)
    {
    if ( !iStatePtr )
        {
        return;
        }

    TRAP_IGNORE(iStatePtr->HandleCommandL(KResourceChanged));	
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::SetNextFepUI
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerBase::SetNextFepUI(MAknFepManagerInterface* /*aNextUi*/)
    {
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::SetStarKeyFlag
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerBase::SetStarKeyFlag( TBool aSet )
	{
	if ( iStatePtr )
        {
        iStatePtr->SetStarKeyFlag( aSet );
        }
	}
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
#ifdef RD_INTELLIGENT_TEXT_INPUT
// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::HandleLongKeyL
// 
// 
// ---------------------------------------------------------------------------
//
TBool CAknFepUIManagerBase::HandleLongKeyL( TInt aKey )
    {
#ifdef _DEBUG
    RDebug::Print( _L("HandleLongKeyL(%c)"), aKey );
#endif
    
    TBool consume = EFalse;
    
    if ( iQwertyInputMode &&                                         // Long press implementation for non-QWERTY modes is elsewhere
         iFepMan->LongPressNumberEntryOnQwerty() &&                  // Is long press number entry supported
         iFepMan->FnKeyState() == CAknFepFnKeyManager::EFnKeyNone && // Long press is handled differently only in non-Fn modes
         iFepMan->KeyMapsDifferentCharacterWithFn( (TPtiKey)aKey ) ) // Does the given key have a different character in Fn mode 
        {
        TBool charRemoved = RollbackPreviousCharL();
        
        // Continue only if there's space left in the editor field after the rollback.
        // In case of secret editor, the removal of the initial character is asynchronous.
        // In that case the editor may still seem to be filled up although we have already
        // queued the rollback event.
        if ( iFepMan->EditorHasFreeSpace() || charRemoved )
            {
            // Long key press load, if editor in textcase mode reload the 
            // text case mode, because shrot key press reset textcase mode.
            CaseMan()->SetCurrentCase(iCaseBeforeLongKey);
            
            iFepMan->SetFnKeyState( CAknFepFnKeyManager::EFnKeyNext );

            // Figure out the used case (upper/lower)
            TPtiTextCase ptiCase = EPtiCaseFnLower;
            
            if ( CaseMan()->CurrentCase() == EAknEditorTextCase ||
                 CaseMan()->CurrentCase() == EAknEditorUpperCase )
                {
                ptiCase = EPtiCaseFnUpper;
                }

            if ( iFepMan->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode) &&
                 CaseMan()->IsAbleChangeCase() )
                {
                // QWERTY shift mode => swap the case
                if ( ptiCase == EPtiCaseFnLower ) 
                    ptiCase = EPtiCaseFnUpper;
                else 
                    ptiCase = EPtiCaseFnLower;
                }

            // In case we are in uppercase mode but there's no characters mapped to that mode,
            // use the lower case instead.
            if ( ptiCase == EPtiCaseFnUpper )
                {
                TBuf<1> upperFnMapping;
                iPtiEngine->MappingDataForKey( (TPtiKey)aKey, upperFnMapping, EPtiCaseFnUpper );
                if ( !upperFnMapping.Length() )
                    {
                    ptiCase = EPtiCaseFnLower;
                    }
                }

            iPtiEngine->SetCase( ptiCase );
            }
        else
            {
            consume = ETrue;
            }
        }
    return consume;    
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::RollbackPreviousCharL
// 
// 
// ---------------------------------------------------------------------------
//
TBool CAknFepUIManagerBase::RollbackPreviousCharL()
    {
    TBool characterRemoved = ETrue;
    
    if( iFepMan->KeyboardLayout() == EPtiKeyboardHalfQwerty )
    	{
        if (iFepMan->IsFlagSet(CAknFepManager::EFlagNoMatches))
            {
            iFepMan->TryRemoveNoMatchesIndicatorL();
            iFepMan->UpdateCbaL(NULL);
            // make sure that the candidates 
            // will be updated the next time
            iPtiEngine->HandleCommandL(EPtiCommandSetFlagToUpdateCandidates);
            // Need to return as DeleteKeyPress should not be passed to ptiengine.
            // This is because ptiengine does not maintain the input sequence
            // if the key press does not increase the word length. So when no matches
            // character is (?) is deleted, no need to pass to ptiengine. This is to done
            // to solve the bug :enter characters till the no matches character is shown. Now press
            // backspace, last character is not deleted.
            return ETrue;
            }
    	}
    
    // It depends on the editor type and input mode, how the previous character
    // can be rolled back.
    if ( iPtiEngine->HandleCommandL(EPtiCommandDeadKeyWaiting) > 0 )
        {
        // The previous key entered a dead character
        iPtiEngine->HandleCommandL(EPtiCommandGetAndClearDeadKeyRootChar);
        }
    else if ( iInputSeqLenDeltaWithLastKey > 0 )
        {
        // The previous key entered chracter(s) to input sequence of PtiEngine
        for ( TInt i=0 ; i<iInputSeqLenDeltaWithLastKey ; ++i )
            {
            iPtiEngine->DeleteKeyPress();
            }
        if ( iFepMan->IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction) )
            {
            TPtrC curWord = iPtiEngine->CurrentWord();
            iFepMan->UpdateInlineEditL( curWord, curWord.Length() );
            }
        //a temporary solution to fix the error: long key press will input q1 instead of 1.
        //if the secret editor is a java secret editor, then do not simulate the key EF20 like avkon secret editor
        //&& !( iFepMan->IsJavaSecretEditor() ) was added    
		if ( ( iFepMan->IsSupportsSecretText() ) && !( iFepMan->IsJavaSecretEditor() ) )
        	{
            // Fep communicates with secret text editors using simulated events.
            iFepMan->SimulateKeyEventL(EKeyF20);
            iFepMan->ClearFlag( CAknFepManager::EFlagInsideMultitapInlineEditingTransaction);
            }    
    
        }
    else if ( iEditorTextLenDeltaWithLastKey > 0 )
        {
        // The previous key entered character(s) directly to the editor
        for ( TInt i=0 ; i<iEditorTextLenDeltaWithLastKey ; ++i )
            {
            //a temporary solution to fix the error: long key press will input q1 instead of 1.
            //if the secret editor is a java secret editor, then do not simulate the key EF20 like avkon secret editor
            //&& !( iFepMan->IsJavaSecretEditor() ) was added
            if ( ( iFepMan->IsSupportsSecretText() ) && !( iFepMan->IsJavaSecretEditor() ) )
                {
                // Fep communicates with secret text editors using simulated events.
                iFepMan->SimulateKeyEventL(EKeyF20);
                iFepMan->ClearFlag(CAknFepManager::EFlagInsideMultitapInlineEditingTransaction);
                }
            else
                {
                // In normal editors, the character can be removed directly
                iFepMan->RemovePreviousCharacterL();
                }
            }
        }
    else
        {
        // The previous key press produced no characters => do nothing
        characterRemoved = EFalse;
        }
        
    return characterRemoved;   
    }
#endif // RD_INTELLIGENT_TEXT_INPUT
#endif // __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__	

// add for phrase creation
MZhuyinKeyHandler* CAknFepUIManagerBase::ZhuyinKeyHandler()
    {
    return iFepMan->ZhuyinKeyHandler();
    }
//add for phrase creation
CAknFepZhuyinAnalyser* CAknFepUIManagerBase::ZhuyinAnalyser()
    {
    return iFepMan->ZhuyinAnalyser();
    }
TBool CAknFepUIManagerBase::MapAgainst(TInt aKey, TInt aMode, TInt16 aAgainst, TInt aCase) const
    {
    if(iPtiEngine)
        {
        iPtiEngine->SetInputMode((TPtiEngineInputMode)aMode);
        TBuf<KMaxName> data;
        iPtiEngine->MappingDataForKey((TPtiKey)aKey, data, (TPtiTextCase)aCase);
        if(data.Length() > 0)
            {
            return (data[0] == aAgainst);                
            }
        }
    return EFalse;
    }

// End of file
