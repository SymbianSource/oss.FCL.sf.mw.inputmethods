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
*       Provides the CAknFepUIManagerJapanese methods.
*
*/












// INCLUDE FILES
#include <e32svr.h> // RDebug stuff...
#include <txtfrmat.h>
#include <e32keys.h>

#include "AknFepGlobalEnums.h"
#include "AknFepUiManagerBase.h"
#include "AknFepUIManagerJapanese.h"
#include "AknFepManagerInterface.h"         // MAknFepManagerInterface
#include "AknFepUIManagerStateInterface.h"  // MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"
#include "AknFepGlobalEnums.h"
#include "AknFepCaseManager.h"

#include "AknFepUiInputStateInitialHiraganaKanji.h"
#include "AknFepUiInputStateInitialHiragana.h"
#include "AknFepUiInputStateInitialKatakana.h"
#include "AknFepUiInputStateInitialLatinJapanese.h"
#include "AknFepUiInputStateInitialFullNumber.h"
#include "AknFepUiInputStateEntryJapaneseMultitap.h"
#include "AknFepUiInputStateEntryHiraganaKanji.h"
#include "AknFepUiInputStateEntryKatakana.h"
#include "AknFepUiInputStateEntryLatinJapanese.h"
#include "AknFepUiInputStateConversionHiraganaKanji.h"
#include "AknFepUiInputStateCandidateHiraganaKanji.h"
#include "AknFepUiInputStateCompletionHiraganaKanji.h"
#include "AknFepUiInputStateCompletionPredictiveHiraganaKanji.h"
#include "AknFepUiInputStateCompletionJapaneseMultitap.h"
#include "AknFepUiInputStateCompletionKatakana.h"
#include "AknFepUiInputStateCompletionLatinJapanese.h"
#include "AknFepUiInputStatePredictiveInputJapanese.h"
#include "AknFepUiInputStateNextWordJapanesePredictive.h"
#include "AknFepUICtrlContainerJapanese.h"
#include "AknFepPanic.h"
#include "AknFepUiInputStateCandidateRetroActive.h"

#include <AknIndicatorContainer.h>  // CAknIndicatorContainer::TIndicatorContext
#include <PtiEngine.h>
#include <PtiCompositionDataIF.h>   // MPtiEngineCompositionDataInterface
#include <PtiDefs.h>                // keys

// CONSTANTS

// LOCAL FUNCTION PROTOTYPES
static TBool IsNumericKey(TInt aKey);
static TBool IsQwertyKey(TInt aKey);

//============================ LOCAL FUNCTIONS ==============================

// -----------------------------------------------------------------------------
// IsNumericKey
//
// Returns: ETrue is returned if aKey is number key
//
// -----------------------------------------------------------------------------
//
static TBool IsNumericKey(const TInt aKey)
    {
    TBool response = EFalse;
    if (aKey >= EPtiKeyQwerty0 && aKey <= EPtiKeyQwerty9)
        {
        response = ETrue;
        }
    return response;
    }

// -----------------------------------------------------------------------------
// IsQwertyKey
//
// Returns: ETrue is returned if aKey is qwerty key
//
// -----------------------------------------------------------------------------
//
static TBool IsQwertyKey(const TInt aKey)
    {
    TBool response = EFalse;
    if ( (aKey >= EPtiKeyQwertyA && aKey <= EPtiKeyQwertyZ) ||
        IsNumericKey( aKey ) ||
        aKey == EStdKeySpace ||
        aKey == EPtiKeyQwertyPlus ||
        aKey == EPtiKeyQwertyMinus ||
        aKey == EPtiKeyQwertyComma ||
        aKey == EPtiKeyQwertySemicolon ||
        aKey == EPtiKeyQwertyFullstop ||
        aKey == EPtiKeyQwertyHash ||
        aKey == EPtiKeyQwertySlash ||
        aKey == EPtiKeyQwertyApostrophe)
        {
        response = ETrue;
        }
    return response;
    }

/**
 *  CAknFepUIManagerJapanese class.
 *
 */
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// CAknFepUIManagerJapanese::NewL
//
//
// ---------------------------------------------------------------------------
//
CAknFepUIManagerJapanese* CAknFepUIManagerJapanese::NewL(MAknFepManagerUIInterface* aFepMan,
                                                         CAknFepCaseManager* aCaseMan,
                                                         TLanguage aLanguage)
    {
    CAknFepUIManagerJapanese* self = new(ELeave)CAknFepUIManagerJapanese(aFepMan, aCaseMan);
    CleanupStack::PushL(self);
    self->ConstructL(aLanguage);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerJapanese::~CAknFepUIManagerJapanese
//
//
// ---------------------------------------------------------------------------
//
CAknFepUIManagerJapanese::~CAknFepUIManagerJapanese()
    {
    delete iCandidateArray;
    delete iContainerPane;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerJapanese::SetCase
//
//
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerJapanese::SetCase(TCase aCase)
    {
    CAknFepUIManagerBase::SetCase(aCase);

    if (iMode != ENumber)
        {
        if (!iCaseMan->IsAbleChangeCase())
            {
            if (iCaseMan->CurrentCase() == EAknEditorLowerCase)
                {
                TRAP_IGNORE(iPtiEngine->HandleCommandL(EPtiCommandUseLatinCaseLowerOnlyKeyMap));
                }
            else if (iCaseMan->CurrentCase() == EAknEditorUpperCase)
                {
                TRAP_IGNORE(iPtiEngine->HandleCommandL(EPtiCommandUseLatinCaseUpperOnlyKeyMap));
                }
            }
        else
            {
            TRAP_IGNORE(iPtiEngine->HandleCommandL(EPtiCommandUseLatinDefaultKeyMap));
            }
        }
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerJapanese::HandleKeyL
//
//
// ---------------------------------------------------------------------------
//
TBool CAknFepUIManagerJapanese::HandleKeyL(TInt aKey, TKeyPressLength aLength, TEventCode /*aEventCode*/)
    {
    TUIState currentState = iFepUiState.State();
    if (aLength == EShortKeyPress)
        {
        iInitialFepUIState = currentState;
        }

    // If current input mode is Number or Hiragana,
    // numeric long keypress doesn't operate.
    if (iMode == ENumber || iMode == EHiragana)
        {
        if (IsNumericKey(aKey) && aLength == ELongKeyPress)
            {
            return ETrue;
            }
        }

    if (iMode == EHiragana)
        {
        if( aKey == EPtiKeyStar && aLength == ELongKeyPress)
            {
            return ETrue;
            }
        }
    // CBA keypress is valid when input state is Initial, Entry
    // Predictive input or NextWord.
    switch (aKey)
        {
        case EKeyCBA1:          // ITU-T left softkey
        case EKeyCBA2:          // ITU-T right softkey
        case EStdKeyDevice0:    // Qwerty left softkey (=0xa4)
        case EStdKeyDevice1:    // Qwerty right softkey(=0xa5)
            // In case of Latin and Number, all characters must be committed.
            if (iMode != EHiraganaKanji)
                {
                CloseUI();
                }
            switch (iStatePtr->State())
                {
                case EInitial:
                case EEntry:
                case EPredictiveInput:
                case ENextWord:
                    return EFalse;
                default:
                    return ETrue;
                }
            //break;  not needed
        default:
            // move to next line
            break;
        }

    TPtiTextCase oldPtiCaseMode = iPtiEngine->Case();
    TPtiTextCase caseMode = EPtiCaseLower;
    TPtiTextCase shiftedCase = caseMode;
    if (IsQwerty())
        {
        caseMode = EPtiCaseLower;
        if (!FepMan()->IsOnlyNumericPermitted())
            {
            caseMode = oldPtiCaseMode;
            if (iMode & (EHiraganaKanji|EHiragana|EKatakana))
                {
                if (CaseMan()->CurrentCase() == EAknEditorTextCase
                 && caseMode == EPtiCaseUpper
                 && iStatePtr->State() == EInitial)
                    {
                    iPtiEngine->SetCase(EPtiCaseLower);
                    caseMode = EPtiCaseLower;
                    oldPtiCaseMode = EPtiCaseLower;
                    }
                }
            else if (iMode == ELatin)
                {
                switch (CaseMan()->CurrentCase())
                    {
                    case EAknEditorUpperCase:
                    case EAknEditorTextCase:
                        caseMode = EPtiCaseUpper;
                        break;
                    case EAknEditorLowerCase:
                    default:
                        caseMode = EPtiCaseLower;
                        break;
                    }
                }
            }
        shiftedCase = caseMode;
        if ( FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyShiftMode) )
            {
            // while pressing Shift key
            if ( aKey >= EPtiKeyQwertyA && aKey <= EPtiKeyQwertyZ )
                {
                shiftedCase = (caseMode != EPtiCaseUpper)? EPtiCaseUpper : EPtiCaseLower;
                }
            else
                {
                // except alpha key
                shiftedCase = EPtiCaseUpper;
                }
            }
        else if (!(aKey >= EPtiKeyQwertyA && aKey <= EPtiKeyQwertyZ))
            {
            // out of A-Z key
            shiftedCase = EPtiCaseLower;
            }

        if (shiftedCase != oldPtiCaseMode)
            {
            iPtiEngine->SetCase(shiftedCase);
            }
        }

    TBool ret = iStatePtr->HandleKeyL(aKey, aLength);
    // If the state was changed by keypress event and not handle keypress event,
    // changed state handle it keypress event.
    while (!ret && currentState != iFepUiState.State())
        {
        // If editor was filled by characters,
        // don't handle no more numeric keypress or qwerty keypress
        if (iStatePtr->State() == EInitial
         && !FepMan()->EditorHasFreeSpace())
            if (!IsQwerty() && IsNumericKey(aKey))
                {
                return ETrue;
                }
            else if (IsQwerty() && IsQwertyKey(aKey))
                {
                return ETrue;
                }

        if ( (IsQwerty())
          && (iPtiEngine->Case() != shiftedCase) )
            {
            iPtiEngine->SetCase(shiftedCase);
            }

        currentState = iFepUiState.State();
        ret = iFepUiState.HandleKeyL(aKey, aLength);
        }

    if (IsQwerty())
        {
        if (oldPtiCaseMode != iPtiEngine->Case())
            {
            iPtiEngine->SetCase(oldPtiCaseMode);
            }
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerJapanese::CloseUI
//
//
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerJapanese::CloseUI()
    {
    if (iStatePtr->State() != EInitial)
        {
        iStatePtr->CloseUI();
        if (iStatePtr->State() == EMultiCompletion)
            {
            ChangeState(EInitial);
            }
        else
            {
            // Commit Uncommited string
            if (iStatePtr->State() != ENextWord)
                {
                ChangeState(EMultiCompletion);
                }
            TRAP_IGNORE(iStatePtr->HandleCommandL(EAknFepAllCompletion));
            }
        }
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerJapanese::GetFormatOfFepInlineText
//
//
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerJapanese::GetFormatOfFepInlineText(TCharFormat& aFormat,
                                                        TInt& aNumberOfCharactersWithSameFormat,
                                                        TInt aPositionOfCharacter) const
    {
    TRgb highlightColor1 = AKN_LAF_COLOR_STATIC(244);
    TRgb highlightColor2 = AKN_LAF_COLOR_STATIC(210);
    TRgb textColor = AKN_LAF_COLOR_STATIC(0);
    if ((iFepUiState.State() == EEntry)
     || (iFepUiState.State() == EConversion)
     || (iFepUiState.State() == ECandidate)
     || (iFepUiState.State() == ECandidateRetroActive)
     || (iFepUiState.State() == EPredictiveInput))
        {
        const MPtiEngineCompositionDataInterface* data = PtiEngine()->CompositionData();
        TInt length = 0;
        TInt clauseCount = 0;
        TInt preLength = 0;
        TPtrC8 clauseChar = data->LengthOfPhrase();
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
#ifdef RD_INTELLIGENT_TEXT_INPUT
        if ( clauseChar.Length() )
           {
#endif //RD_INTELLIGENT_TEXT_INPUT
#endif // __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__        
        for (; length < aPositionOfCharacter; ++clauseCount)
            {
            preLength = length;
            length += clauseChar[clauseCount];
            }
        if (length == aPositionOfCharacter)
            aNumberOfCharactersWithSameFormat = clauseChar[clauseCount];
        else
            {
            --clauseCount;
            aNumberOfCharactersWithSameFormat = clauseChar[clauseCount]
                                                + preLength
                                                - aPositionOfCharacter;
            }
        switch (data->AttributeOfPhrase()[clauseCount])
            {
            //Input phase
            case MPtiEngineCompositionDataInterface::EPtiAttributeTransitoryInput:
                {
                if (iFlagInlineEditUnderline)
                    {
                    aFormat.iFontPresentation.iHighlightColor = highlightColor1;
                    aFormat.iFontPresentation.iHighlightStyle = TFontPresentation::
                                                                EFontHighlightNormal;
                    }
                else
                    {
                    }
                }
                break;
            // Conversion phase(no target phrase)
            case MPtiEngineCompositionDataInterface::EPtiAttributeNonTargetConvert:
                {
                aFormat.iFontPresentation.iHighlightColor = highlightColor1;
                aFormat.iFontPresentation.iHighlightStyle = TFontPresentation::
                                                            EFontHighlightNormal;
                }
                break;
            // Conversion phase(target Phrase)
            case MPtiEngineCompositionDataInterface::EPtiAttributeTargetConvert:
                {
                aFormat.iFontPresentation.iTextColor = textColor;
                aFormat.iFontPresentation.iHighlightColor = highlightColor2;
                aFormat.iFontPresentation.iHighlightStyle = TFontPresentation::
                                                            EFontHighlightNormal;
                }
                break;
            default:
                break;
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
#ifdef RD_INTELLIGENT_TEXT_INPUT
						}
#endif // RD_INTELLIGENT_TEXT_INPUT
#endif // __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__              
            }
        }
        else if (iFepUiState.State() == EMultiCompletion)
            {
            const MPtiEngineCompositionDataInterface* data = PtiEngine()->CompositionData();
            TInt length = data->CompletedString().Length();
            // Panic in case (aNumberOfCharactersWithSameFormat < 1)
            if( !(length > 0) )
               {
               length = 1;
               }

            aNumberOfCharactersWithSameFormat = length;
        }
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerJapanese::IsValidShiftKeyPress
//
//
// ---------------------------------------------------------------------------
//
TBool CAknFepUIManagerJapanese::IsValidShiftKeyPress() const
    {
    TBool ret = ETrue;
    switch (iMode)
        {
        case EHiraganaKanji:
            // ITU-T has to check Edit-key(= Shift-key). If the state is Initial
            // or NextWord, Edit-key is valid, the other states are invalid.
            // However, in Qwerty input, shift-key is used for changing Case mode,
            // so, shift-key is always valid even if the statue is anything.
            if (!iQwerty)
                {
                if (iStatePtr->State() == EInitial || iStatePtr->State()== ENextWord)
                    {
                    ret = ETrue;
                    }
                else
                    {
                    ret = EFalse;
                    }
                }
            break;
        case EHiragana:
            ret = EFalse;
            break;
        case EKatakana:
        default:
            break;
        }

    return ret;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerJapanese::ChangeState
//
//
// ---------------------------------------------------------------------------
//
TUIState CAknFepUIManagerJapanese::ChangeState(TUIState aState)
    {
    switch(aState)
        {
        case EInitial:
            {
            switch(iMode)
                {
                case EHiragana:
                    iFepUiState = TAknFepUiInputStateInitialHiragana(this, UIContainer());
                    break;
                case EHiraganaKanji:
                    iFepUiState = TAknFepUiInputStateInitialHiraganaKanji(this, UIContainer());
                    break;
                case EKatakana:
                    iFepUiState = TAknFepUiInputStateInitialKatakana(this, UIContainer());
                    break;
                case ELatin:
                    {
                    iFepUiState = TAknFepUiInputStateInitialLatinJapanese(this, UIContainer());
                    }
                    break;
                case ENumber:
                    iFepUiState = TAknFepUiInputStateInitialFullNumber(this, UIContainer());
                    break;
                default:
                    AknFepPanic(EAknFepPanicBadInputState);
                    break;
                }
            break;
            }
        case EEntry:
            {
            switch(iMode)
                {
                case EHiraganaKanji:
                   iFepUiState = TAknFepUiInputStateEntryHiraganaKanji(this, UIContainer());
                   break;
                case EHiragana:
                case EKatakana:
                   iFepUiState = TAknFepUiInputStateEntryKatakana(this, UIContainer());
                   break;
                case ELatin:
                   iFepUiState = TAknFepUiInputStateEntryLatinJapanese(this, UIContainer());
                   break;
                case ENumber:
                   iFepUiState = TAknFepUiInputStateEntryJapaneseMultitap(this, UIContainer());
                   break;
                default:
                    AknFepPanic(EAknFepPanicBadInputState);
                    break;
                }
            }
            break;
        case ECandidate:
            iFepUiState = TAknFepUiInputStateCandidateHiraganaKanji(this, UIContainer());
            break;
        case EPredictiveInput:
            iFepUiState = TAknFepUiInputStatePredictiveInputJapanese(this, UIContainer());
            break;
        case EConversion:
            iFepUiState = TAknFepUiInputStateConversionHiraganaKanji(this, UIContainer());
            break;
        case EMultiCompletion: // Only Japanese version uses this flag.
            switch(iMode)
                {
                case EHiraganaKanji:
                    if(iPredictive)
                        {
                        if(iFepUiState.State() == EPredictiveInput ||
                           iFepUiState.State() == ENextWord)
                            {
                            iFepUiState = TAknFepUiInputStateCompletionPredictiveHiraganaKanji(this, UIContainer());
                            }
                        else
                            {
                            iFepUiState = TAknFepUiInputStateCompletionHiraganaKanji(this, UIContainer());
                            }
                        }
                    else
                        {
                        iFepUiState = TAknFepUiInputStateCompletionHiraganaKanji(this, UIContainer());
                        }
                    break;
                case EHiragana:
                case EKatakana:
                        {
                        iFepUiState = TAknFepUiInputStateCompletionKatakana(this, UIContainer());
                        }
                    break;
                case ENumber:
                    iFepUiState = TAknFepUiInputStateCompletionJapaneseMultitap(this, UIContainer());
                    break;
                case ELatin:
                    iFepUiState = TAknFepUiInputStateCompletionLatinJapanese(this, UIContainer());
                    break;
                default:
                    AknFepPanic(EAknFepPanicBadInputState);
                    break;
                }
                break;
        case ENextWord: // Only Japanese version uses this flag
            {
            switch(iMode)
                {
                case EHiraganaKanji:
                    if(iPredictive)
                        {
                        iFepUiState = TAknFepUiInputStateNextWordJapanesePredictive(this, UIContainer());
                        break;
                        }
                default:
                    AknFepPanic(EAknFepPanicBadInputState);
                    break;
                }
            }
            break;
        case ECandidateRetroActive:
            {
            switch(iMode)
                {
                case EHiraganaKanji:
                    iFepUiState = TAknFepUiInputStateCandidateRetroActive(this, UIContainer());
                    break;
                default:
                    AknFepPanic(EAknFepPanicBadInputState);
                    break;
                }
            }
            break;
        default:
            AknFepPanic(EAknFepPanicBadInputState);
        }

    PtiEngine()->SetObserver(&iFepUiState);

#ifdef _DEBUG
    RDebug::Print(_L("UI Manager: New State: %d"), iFepUiState.State());
#endif
    TRAPD(err, iStatePtr->InitializeStateL());
    if (err != KErrNone)
        {
#ifdef _DEBUG
        RDebug::Print(_L("UI Manager: Setup Error: Mode = %d, State = %d"),
                      iMode, iStatePtr->State());
#endif
        CloseUI();
        }
    return iStatePtr->State();
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerJapanese::CandidateArray
//
//
// ---------------------------------------------------------------------------
//
CDesCArrayFlat* CAknFepUIManagerJapanese::CandidateArray()
    {
    return iCandidateArray;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerJapanese::IsPredictive
//
//
// ---------------------------------------------------------------------------
//
TBool CAknFepUIManagerJapanese::IsPredictive() const
   {
   return iPredictive;
   }


// ---------------------------------------------------------------------------
// CAknFepUIManagerJapanese::SetInlineEditUnderlineVisibility
//
//
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerJapanese::SetInlineEditUnderlineVisibility(TBool aFlag)
    {
    iFlagInlineEditUnderline = aFlag;
    }


// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::SetMode
//
//
// ---------------------------------------------------------------------------
//
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
#ifdef RD_INTELLIGENT_TEXT_INPUT
void CAknFepUIManagerJapanese::SetMode(TInt aMode, TBool aPredictive, TBool aQwertyInputMode, TInt aKeyboardType)
#else
void CAknFepUIManagerJapanese::SetMode(TInt aMode, TBool aPredictive, TBool aQwertyInputMode)
#endif // RD_INTELLIGENT_TEXT_INPUT
#else
void CAknFepUIManagerJapanese::SetMode(TInt aMode, TBool aPredictive, TBool aQwertyInputMode)
#endif // __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
    {
    iMode=aMode;
    iPredictive = aPredictive;
    iQwerty = aQwertyInputMode;
    ChangeState(EInitial);
    }


// ---------------------------------------------------------------------------
// CAknFepUIManagerJapanese::FepUIState
//
//
// ---------------------------------------------------------------------------
//
TAknFepInputStateBase* CAknFepUIManagerJapanese::FepUIState()
    {
    return &iFepUiState;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerJapanese::UIContainer
//
//
// ---------------------------------------------------------------------------
//
MAknFepUICtrlContainerJapanese* CAknFepUIManagerJapanese::UIContainer() const
    {
    return iContainerPane;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerJapanese::CAknFepUIManagerJapanese
//
//
// ---------------------------------------------------------------------------
//
CAknFepUIManagerJapanese::CAknFepUIManagerJapanese(MAknFepManagerUIInterface* aFepMan,
                                                   CAknFepCaseManager* aCaseMan)
    : CAknFepUIManagerBase(aFepMan, aCaseMan),
      iFepUiState(NULL,NULL),
      iFlagInlineEditUnderline(ETrue)
    {
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerJapanese::ConstructL
//
//
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerJapanese::ConstructL(TLanguage aLanguage)
    {
    CAknFepUIManagerBase::ConstructL(aLanguage);

    PtiEngine()->ActivateLanguageL(aLanguage, EPtiEngineInputModeNone);

    iCandidateArray = new(ELeave) CDesCArrayFlat(1);
    iContainerPane  = FepMan()->UiInterface()->NewAknFepUICtrlContainerJapaneseL(this);
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerJapanese::IsQwerty
//
//
// ---------------------------------------------------------------------------
//
TBool CAknFepUIManagerJapanese::IsQwerty() const
    {
    return iQwerty;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerJapanese::HandleCommandL
//
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerJapanese::HandleCommandL(TInt aCommandId)
    {
    switch (aCommandId)
        {
        case EAknFepSoftkeyCommit:
        case EAknFepSoftkeySelectCandidate:
            {
            TUIState currentState = iFepUiState.State();
            iStatePtr->HandleCommandL(aCommandId);
            if (currentState != iFepUiState.State())
                {
                if (iFepUiState.State() == EMultiCompletion)
                    {
                    iFepUiState.HandleKeyL(EKeyOK, EShortKeyPress);
                    }
                }
            }
            break;
        case EAknFepSoftkeyPredictiveOn:
            {
            MAknFepManagerUIInterface* fepMan = FepMan();
            CPtiEngine* ptiEngine = PtiEngine();

            // Update of inline editing characters
            TPtiEngineInputMode inputmode = (IsQwerty())?
                        EPtiEngineHiraganaKanjiPredictiveQwerty
                        : EPtiEngineHiraganaKanjiPredictive;
            ptiEngine->SetInputMode(inputmode);
            const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
            TPtrC text = data->CompletedString();
            TPtrC text2 = data->ConvertingString();
            if (text.Length() >0)
                {
                ChangeState(EMultiCompletion);
                fepMan->CommitInlineEditL(text, text.Length());
                if (fepMan->EditorHasFreeSpace())
                    {
                    ChangeState(ENextWord);
                    }
                else
                    {
                    CloseUI();
                    ChangeState(EInitial);
                    }
                }
            else if (text2.Length() > 0)
                {
                // Change to Predictive State
                ChangeState(EPredictiveInput);
                iPredictive = ETrue;
                iStatePtr->HandleCommandL(aCommandId);
                }
            }
            break;
        default:
            iStatePtr->HandleCommandL(aCommandId);
            break;
        }
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerBase::ResourceChanged
// This API is called when FEP control handles the changing resource event.
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerJapanese::ResourceChanged(TInt /*aType*/)
    {
    TRAP_IGNORE( iStatePtr->HandleCommandL(EAknFepResourceChanged));
    }

// End of file
