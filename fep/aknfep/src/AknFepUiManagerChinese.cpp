/*
* Copyright (c) 2003-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the CAknFepUIManagerChinese methods.
*
*/












// INCLUDE FILES
#include <e32svr.h> // RDebug stuff...
#include <e32keys.h>

#include "AknFepUiManagerBase.h"
#include "AknFepUiManagerChinese.h"
#include "AknFepManagerUIInterface.h"
#include "AknFepGlobalEnums.h"
#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUICtrlInputPane.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepUICtrlPinyinPopup.h"
#include "AknFepUiInputStateCandidate.h"
#include "AknFepUiInputStateCandidateChinese.h"
#include "AknFepUiInputStatePredictiveCandidate.h"
#include "AknFepUiInputStatePredictiveInput.h"
#include "AknFepUiInputStatePredictiveInputChinese.h"
#include "AknFepUiInputStateInitialChineseGeneric.h"
#include "AknFepUiInputStateInitialLatinMultitap.h"
#include "AknFepUiInputStateInitialNumber.h"
#include "AknFepUiInputStateEntryPinyin.h"
#include "AknFepUiInputStateEntryZhuyin.h"
#include "AknFepUiInputStateInitialZhuyinFind.h"
#include "AknFepUiInputStateInitialStrokeFind.h"
#include "AknFepUiInputStateEntryStroke.h"
#include "AknFepUiInputStateInitialWesternPredictive.h"
#include "AknFepUiInputStateEntryWesternPredictive.h"
#include "AknFepUiInputStatePredictiveCandidatePinyin.h"
#include "AknFepUiInputStatePredictiveCandidateStroke.h"
#include "AknFepCaseManager.h"
#include "AknFepPanic.h"

#include "AknFepUiCtrlContainerChinese.h"				// phrase input container
#include "AknFepUiInputStateEntryPinyinPhrase.h"		// phrase pinyin entry state class
#include "aknfepuiinputstateentrypinyinphrasecreation.h"   // pinyin phrase creation
#include "aknfepinputstatespellingpinyinphrasecreation.h"  // pinyin phrase creation
#include "aknfepinputstatecandidatepinyinphrasecreation.h" // pinyin phrase creation
#include "aknfepuiinputstateentryzhuyinphrasecreation.h"     // zhuyin phrase creation
#include "AknFepUiInputStateEntryZhuyinPhrase.h"		// phrase zhuyin entry state class
#include "AknFepUiInputStateEntryStrokePhrase.h"		// phrase Stroke entry state class
#include "AknFepUiInputStateCandidateChinesePhrase.h"          // phrase candidate state class
#include "aknfepuictrleditpane.h"           //phrase EEP control
#include "AknFepUiInputStateCandidatePinyinAndStrokePhrase.h"
#include "AknFepUiInputStatePredictiveInputPhrase.h"
#include "AknFepUiInputStatePredictiveInputChinesePhrase.h"
// Qwerty input
#include "AknFepUiInputStateInitialChineseGenericQwerty.h"
#include "AknFepUiInputStateEntryQwertyPinyin.h"
#include "AknFepUiInputStateEntryQwertyStroke.h"
#include "AknFepUiInputStateEntryQwertyCangJie.h"
#include "AknFepUiInputStateEntryQwertyZhuyin.h"
#include "AknFepUiInputStateCandidateQwertyZhuyin.h"
#include "AknFepUiInputStatePredictiveCandidateQwertyZhuyin.h"
#include "AknFepUiInputStatePredictiveInputQwerty.h"
#include "AknFepUiInputStatePredictiveInputQwertyPinyin.h"
#include "AknFepUiInputStatePredictiveInputQwertyZhuyin.h"
#include "AknFepUiInputStatePredictiveCandidatePinyinPhrase.h"
#include "AknFepUiInputStatePredictiveCandidateStrokePhrase.h"
#include "AknFepUiInputStatePredictiveCandidatePhrase.h"
#include "AknFepUiInputStatePredictiveInputQwertyStroke.h"
// Qwerty phrase input
#include "AknFepUiInputStateEntryQwertyStrokePhrase.h"
#include "AknFepUiInputStateEntryQwertyPinyinPhrase.h"
#include "AknFepUiInputStatePredictiveInputQwertyChinesePhrase.h"
#include "AknFepUiInputStatePredictiveInputQwertyPinyinPhrase.h"
#include "AknFepUiInputStateEntryQwertyZhuyinPhrase.h"
#include "AknFepUiInputStateCandidateQwertyZhuyinPhrase.h"
#include "AknFepUiInputStatePredictiveCandidateQwertyZhuyinPhrase.h"
#include "AknFepUiInputStatePredictiveInputQwertyZhuyinPhrase.h"
#include "AknFepUiInputStatePredictiveInputQwertyStrokePhrase.h"
#ifdef RD_INTELLIGENT_TEXT_INPUT        
#include "AknFepUIInputStateEntryMiniQwertyPinyinPhrase.h"
#include "AknFepUiInputStateCandidateMiniQwertyPinyinAndStrokePhrase.h"
#include "AknFepUiInputStatePredictiveInputMiniQwertyChinesePhrase.h"
#include "AknFepUiInputStatePredictiveCandidateMiniQwertyChinesePhrase.h"
#include "AknFepUiInputStateEntryMiniQwertyZhuyinPhrase.h"
#include "AknFepUiInputStateCandidateMiniQwertyZhuyinPhrase.h"
#include "aknfepuiinputstateminiqwertyzhuyinphrase.h"//phrase creation for mini qwerty zhuyin
#include "aknfepuiinputstateminiqwertystrokephrase.h"
#include "AknFepUIInputStateEntryMiniQwertyCangJie.h"
#include "aknfepuiinputstateentryhalfqwertyphrase.h"
#include "AknFepUiInputStateCandidateHalfQwertyChinesePhrase.h"
#include "AknFepUiInputStateInitialChineseGenericHalfQwerty.h"
#include "AknFepUiInputStatePredictiveCandidateHalfQwertyChinesePhrase.h"
#endif
// Qwerty Chinese find
#include "AknFepUiInputStatelStrokeFindQwerty.h"
#include "AknFepUiInputStateZhuyinFindQwerty.h"
#include <AknIndicatorContainer.h>  

#include <PtiEngine.h> 
#include <PtiDefs.h>

#include <avkon.rsg>        //phrase creation
#include <aknfep.rsg>

#include "aknfepuiinputminiqwertysogoupinyinphraseplugin.h"
#include "aknfepuiinputminiqwertysogoustrokephraseplugin.h"

// CONSTANTS
const TInt16 KStrokeHorizontalValue = 0x4e00; 
const TInt16 KStrokeVerticalValue = 0x4e28;
const TInt16 KStrokeDownToLeftValue = 0x4e3f;
const TInt16 KStrokeDownToRightValue = 0x4e36;
const TInt16 KStrokeBendingValue = 0x4e5b;
const TInt16 KStrokeQuestionValue = 0x003f;
const TInt16 KStrokeUnicode = 0x2461;
const TInt16 KZhuyinIndicator = 0x2462;
const TInt16 KPinyinIndicator = 0x2460;
//const TInt16 KStrokeIndicator = 0x2461;
const TInt16 KCanjieIndicator = 0x2463;
const TInt16 KChineseTone1 = 0x02c9;
const TInt16 KChineseTone2 = 0x02ca; 
const TInt16 KChineseTone3 = 0x02c7; 
const TInt16 KChineseTone4 = 0x02cb;
const TInt16 KChineseTone0 = 0x02d9;

const TUid KPtiSogouCoreUid = { 0x20031DD6 };

/**
*  CAknFepUIManagerChinese class.
* 
*/

//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// CAknFepUIManagerChinese::NewL
// 
// 
// ---------------------------------------------------------------------------
//
CAknFepUIManagerChinese* CAknFepUIManagerChinese::NewL(MAknFepManagerUIInterface* aFepMan, 
                                       CAknFepCaseManager* aCaseMan, TLanguage aLanguage)
    {   
    CAknFepUIManagerChinese* self = new(ELeave)CAknFepUIManagerChinese(aFepMan, aCaseMan);
    CleanupStack::PushL(self);
    self->ConstructL(aLanguage);
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerChinese::~CAknFepUIManagerChinese
// 
// 
// ---------------------------------------------------------------------------
//
CAknFepUIManagerChinese::~CAknFepUIManagerChinese()
    {
    delete iContainerPane;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerChinese::HandleKeyL
// 
// 
// ---------------------------------------------------------------------------
//
TBool CAknFepUIManagerChinese::HandleKeyL(TInt aKey, TKeyPressLength aLength, TEventCode aEventCode)
    {
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
#ifdef RD_INTELLIGENT_TEXT_INPUT    
    // In the initial state, use the base implementation which support long
    // press number entry on QWERTY. In other states the long press is not
    // handled.
    if ( (aLength == EShortKeyPress && iFepUiState.State() == EInitial) ||
         (aLength == ELongKeyPress && iInitialFepUIState == EInitial) )
        {
        return CAknFepUIManagerBase::HandleKeyL( aKey, aLength );
        }
    else if ( iFepMan->EditorHasFreeSpace() )
        {
#endif // RD_INTELLIGENT_TEXT_INPUT
#endif // __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
    TUIState currentState = iFepUiState.State();
    
    //when the state is EEntry and input mode is pinyin or zhuyin and akey = starkey
    //just handles EEventKey
    if( EPtiKeyStar == aKey && currentState == EEntry 
    		&& ( iFepMan->InputMode() == EPinyin || iFepMan->InputMode() == EZhuyin )
    		&& aEventCode != EEventKey )
    	{
    	return EFalse;
    	}
    if (aLength == EShortKeyPress)
        {
        iInitialFepUIState = currentState;
        }

    if(!iStatePtr->HandleKeyL(aKey, aLength))
        {
        if(currentState != iStatePtr->State()) //Check to see if we've changed state.
            {
            return iStatePtr->HandleKeyL(aKey, aLength);
            }
        return EFalse; // we've failed to handle it..
        }
    return ETrue; //we're ok.
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
#ifdef RD_INTELLIGENT_TEXT_INPUT
        }
    else
        {
        // FepManager must let some long press events to pass here even when
        // editor has no space, because long press may still work. But if
        // the long press was not handled, then the key event should not be
        // handled at all in this case.
        return EFalse;
        }
#endif // RD_INTELLIGENT_TEXT_INPUT
#endif // __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__    
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerChinese::CloseUI
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerChinese::CloseUI()
    {
    // update cba
    TRAP_IGNORE(FepMan()->UpdateCbaL(NULL));
    iContainerPane->EditPaneWindow()->SetCurrentCBAResID( 0 );
    
    if (iQwertyMode)
        {
#ifdef RD_INTELLIGENT_TEXT_INPUT 
        if ( EPtiKeyboardHalfQwerty == iPtiEngine->KeyboardType() )
            {
            ChangeState(EHalfQwerty);
            }
        else
#endif
            {
            ChangeState(EQwerty);
            }
        }
    else
        {
        ChangeState(EInitial);
        }
    if (iContainerPane)
        {
        iContainerPane->CandidatePane()->ResetCandidateState();
        iContainerPane->EditPaneWindow()->ResetAllArray();
        iContainerPane->Enable(EFalse);
        }
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerChinese::GetFormatOfFepInlineText
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerChinese::GetFormatOfFepInlineText(TCharFormat& aFormat, 
         TInt& aNumberOfCharactersWithSameFormat, TInt aPositionOfCharacter) const
    {
    if (iPredictive && iFepUiState.State() == EEntry)   // EInitial???
        {
        aFormat.iFontPresentation.iUnderline = EUnderlineOn;
        if (iFepMan->IsFlagSet(CAknFepManager::EFlagNoMatches))
            {
            aFormat.iFontPresentation.iHighlightStyle = 
                                   TFontPresentation::EFontHighlightNoMatchesIndicator;
            }
        TCursorSelection uncommit = iFepMan->UncommittedText();
        if (uncommit.Length()-aPositionOfCharacter>0)                   
            {
            aNumberOfCharactersWithSameFormat=uncommit.Length() - aPositionOfCharacter;
            }
        }
    }


// ---------------------------------------------------------------------------
// CAknFepUIManagerChinese::SupportLanguage
// 
// 
// ---------------------------------------------------------------------------
//
TInt CAknFepUIManagerChinese::SupportLanguage(TInt aMode) const
    {
    TInt lang = iLanguage;
    switch (aMode)
        {
        case ELatin:
            {
            lang = ELangEnglish;
            }
            break;
        case ENumber:
        case EPinyin:
        case EStroke:
        case ECangJie:
        case EStrokeFind:
        case EZhuyin:
        case EZhuyinFind:
        default:
            break;
        }
    return lang;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerChinese::SetEditorContext
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerChinese::SetEditorContext(TInt aContext)
    {
    if(iContainerPane)
        {
        iContainerPane->SetIsShownWithPopupWindows(
            aContext == CAknIndicatorContainer::EQueryEditorIndicators);
        }
   }

// ---------------------------------------------------------------------------
// CAknFepUIManagerChinese::ChangeState
// 
// 
// ---------------------------------------------------------------------------
//
TUIState CAknFepUIManagerChinese::ChangeState(TUIState aState)
    {
    // ask pti engine if phrase input is supported
    if(iPrevMode != iMode)
        {
        iPrevMode = iMode;
        ResetFlags();
        }
    if(!IsFlagSet(EQueryPtiEngineAlready))  
        {
        QueryPtiEngine();
        }
     
    // distinguish if Qwerty input in editing window state
    if(aState == EQwerty)
        {
        iQwertyMode = ETrue;
        }
    else if(aState == EInitial)
        {
        iQwertyMode = EFalse;
        }

    if(iQwertyMode)
        {
        ChangeStateQwerty(aState);
        }
    else
        {
        ChangeStatetItut(aState);
        }
        
    iPtiEngine->SetObserver(&iFepUiState);
#ifdef _DEBUG
    RDebug::Print(_L("UI Manager: New State: %d"), iFepUiState.State());
#endif
    // cba update: when input window exists, softkeys are not shown.
    TRAPD(err, iStatePtr->InitializeStateL());
    if(err != KErrNone)
        {
#ifdef _DEBUG
    RDebug::Print(_L("UI Manager: Initial Error: Mode = %d, State = %d"), 
                    iMode, iStatePtr->State());
#endif
        }
    return iStatePtr->State();
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerChinese::IsValidChineseInputKey
// 
// 
// ---------------------------------------------------------------------------
//
TBool CAknFepUIManagerChinese::IsValidChineseInputKey(TInt aKey) const
    {
    // This doesn't include checking for the 'tone' key, which is different...

    TBool response = ETrue;
    switch(aKey)
        {
    case EPtiKey0:
        if(((iMode == EPinyin) && (!IsFlagSet(ESupportPinyinPhrase) || 
            !iContainerPane->IsEnabled())) ||
           ((iMode == EStroke) && (!IsFlagSet(ESupportStrokePhrase) || 
            !iContainerPane->IsEnabled()))  || (iMode == EStrokeFind)) 
            {
            response = EFalse;
            }
        break;
    case EPtiKey1:
        if((iMode == EPinyin)&& (!IsFlagSet(ESupportPinyinPhrase) || !iContainerPane->IsEnabled()))
            {
            response = EFalse;
            }
        break;
    case EPtiKey2: //fall through
    case EPtiKey3:
    case EPtiKey4:
    case EPtiKey5:
    case EPtiKey6:
        break;  //always valid
    case EPtiKey7: //fall through
    case EPtiKey8:
    case EPtiKey9:
        if((iMode == EStroke) || (iMode == EStrokeFind))
            response = EFalse;
        break;
    default:
        response = EFalse;
        break;
        }
    return response;
    }
#ifdef RD_INTELLIGENT_TEXT_INPUT
// ---------------------------------------------------------------------------
// CAknFepUIManagerChinese::IsKeyMappedStroke
// 
// 
// ---------------------------------------------------------------------------
//

TBool  CAknFepUIManagerChinese::IsKeyMappedStroke(TInt aKey) const
    {
    TBuf<KMaxName> lowerdata;   
    iPtiEngine->MappingDataForKey((TPtiKey)aKey, lowerdata, EPtiCaseLower);  
    TInt StrokeUnicodePosition =0;
    
    if(lowerdata.Length()>0)
        {
        for(TInt i=0;i<lowerdata.Length();i++)
            {
            if(lowerdata[i]==KStrokeUnicode)
                {
                StrokeUnicodePosition = i+1;
                break;
                }
            }
        }
    if(lowerdata.Length() > StrokeUnicodePosition)
        {
        if (lowerdata[StrokeUnicodePosition] == KStrokeHorizontalValue || 
                lowerdata[StrokeUnicodePosition] == KStrokeVerticalValue || 
                lowerdata[StrokeUnicodePosition] == KStrokeDownToLeftValue || 
                lowerdata[StrokeUnicodePosition] == KStrokeDownToRightValue || 
                lowerdata[StrokeUnicodePosition] == KStrokeBendingValue ||
                lowerdata[StrokeUnicodePosition] == KStrokeQuestionValue)
            {
            return ETrue;
            }
        }
    
    return EFalse;
    }
#endif
TBool CAknFepUIManagerChinese::IsValidChineseInputSymbol(TInt aKey, TPtiEngineInputMode aMode) const
    {
    if( aMode == EPtiEnginePinyinPhraseQwerty)
        {
        return MapAgainst(aKey, aMode, KPinyinIndicator, EPtiCaseLower);
        }
    else if( aMode == EPtiEngineZhuyinPhraseQwerty)
        {
        return MapAgainst(aKey, aMode, KZhuyinIndicator, EPtiCaseLower);
        }
    else if( aMode == EPtiEngineNormalCangjieQwerty || 
             aMode == EPtiEngineEasyCangjieQwerty ||
             aMode == EPtiEngineAdvCangjieQwerty)
        {
        return MapAgainst(aKey, aMode, KCanjieIndicator, EPtiCaseLower);
        }
    return EFalse;
    }
TBool CAknFepUIManagerChinese::IsValidChineseToneMarkKey(TInt aKey, TPtiEngineInputMode aMode) const
    {
    if( MapAgainst(aKey, aMode, KChineseTone1, EPtiCaseLower))
        {
        return ETrue;
        }
    else if( MapAgainst(aKey, aMode, KChineseTone2, EPtiCaseLower))
        {
        return ETrue;
        }
    else if( MapAgainst(aKey, aMode, KChineseTone3, EPtiCaseLower))
        {
        return ETrue;
        }
    else if( MapAgainst(aKey, aMode, KChineseTone4, EPtiCaseLower))
        {
        return ETrue;
        }
    else if( MapAgainst(aKey, aMode, KChineseTone0, EPtiCaseLower))
        {
        return ETrue;
        }
    return EFalse;
    }
// ---------------------------------------------------------------------------
// CAknFepUIManagerChinese::IsValidChineseInputKeyQwerty
// 
// 
// ---------------------------------------------------------------------------
//
TBool CAknFepUIManagerChinese::IsValidChineseInputKeyQwerty(TInt aKey) const
    {
    TBool response = EFalse;
#ifdef RD_INTELLIGENT_TEXT_INPUT     
    
    TPtiKeyboardType keyboardType = EPtiKeyboardNone;
    if( iPtiEngine != NULL )        
        {
        keyboardType = iPtiEngine->KeyboardType();
        }
#endif

    if(iMode == EPinyin)
        {
        if( IsValidChineseInputSymbol(aKey, EPtiEnginePinyinPhraseQwerty) ||
            (IsFlagSet(ESupportPinyinPhrase) && aKey == EPtiKeyQwertyApostrophe 
             && State() != EInitial))
            {
            response = ETrue;
            }
#ifdef RD_INTELLIGENT_TEXT_INPUT          
        if ( State() == EInitial && EPtiKeyboardHalfQwerty == keyboardType &&
           ( aKey == EPtiKeyQwertyU || aKey == EPtiKeyQwertyI || aKey == EPtiKeyQwerty3 ))
            {
            response = EFalse;
            }
#endif
        }

    if(iMode == EStroke || iMode == EStrokeFind)
        {
#ifdef RD_INTELLIGENT_TEXT_INPUT        
        if(EPtiKeyboardQwerty4x10 == keyboardType ||EPtiKeyboardQwerty3x11 == keyboardType )
            {            
            if( IsKeyMappedStroke(aKey)|| EPtiKeyQwertySpace == aKey)
                {
                response = ETrue;
                if(aKey == EPtiKeyQwertySpace && (State() == EInitial || State() ==EPredictiveInput))
                    {
                    response = EFalse;
                    }
                }
            }
        else if( EPtiKeyboardHalfQwerty == keyboardType ||
           EPtiKeyboardCustomQwerty == keyboardType)
            {
            MPtiLanguage *lang = NULL;
            RArray < TPtiNumericKeyBinding > keybinding;
            TBool IsStrokeKey = EFalse;
            if(iPtiEngine)
            { 
            	  lang = iPtiEngine->CurrentLanguage();
            }
            if( iPtiEngine && lang )
                {
                TRAP_IGNORE(iPtiEngine->GetNumericModeKeysForQwertyL(
                						lang->LanguageCode(),
                				 		keybinding, keyboardType));
                TInt numericKeysCount = keybinding.Count();
                while ( numericKeysCount-- )
                    {
                    TPtiNumericKeyBinding numKeyBind = keybinding[numericKeysCount];
                    if ( (numKeyBind.iChar >= 0x31 && numKeyBind.iChar <= 0x36)&&(aKey == numKeyBind.iKey))
                        {
                           IsStrokeKey = ETrue;
                           break;              
                        }
                    }
                }
            keybinding.Reset();
            if(IsStrokeKey || (aKey == EPtiKeyQwertySpace))
                {
                if((aKey >= EPtiKeyQwertyA && aKey <= EPtiKeyQwertyZ) ||
                   (aKey >= EPtiKeyQwerty0 && aKey <= EPtiKeyQwerty6) ||		
                   (aKey == EPtiKeyQwertySpace))
                    {
                    response = ETrue;
                    if(aKey == EPtiKeyQwertySpace && 
                    	(State() == EInitial || State() ==EPredictiveInput))
                        {
                        response = EFalse;
                        }                  
                    }
                }               
            }
        else
            {
#endif
        switch(aKey)
            {
            case EPtiKeyQwertyQ:
            case EPtiKeyQwertyW:
            case EPtiKeyQwertyE:
            case EPtiKeyQwertyA:
            case EPtiKeyQwertyS:
            case EPtiKeyQwertyD:
            case EPtiKeyQwertyI:
            case EPtiKeyQwertyO:
            case EPtiKeyQwertyP:
            case EPtiKeyQwertyJ:
            case EPtiKeyQwertyK:
            case EPtiKeyQwertyL:
            case EPtiKeyQwertySpace:
                {
                response = ETrue;
                if(aKey == EPtiKeyQwertySpace && (State() == EInitial 
                || State() ==EPredictiveInput))
                    {
                    response = EFalse;
                    }
                }
                break;
            }
        }
#ifdef RD_INTELLIGENT_TEXT_INPUT        
        }
#endif
    
    if(iMode == ECangJie)
        {
        response = IsValidChineseInputSymbol(aKey, (TPtiEngineInputMode)iFepMan->CangJieMode());
        }
    
    if(iMode == EZhuyin || iMode == EZhuyinFind)
        {
#ifdef RD_INTELLIGENT_TEXT_INPUT        
       //     TInt ZhuyinUnicodeCur =0;
        if (  EPtiKeyboardHalfQwerty == keyboardType )
            {
            if((aKey >= EPtiKeyQwertyA && aKey <= EPtiKeyQwertyZ) || 
                    (aKey >= EPtiKey0 && aKey <= EPtiKey9) ||
                    aKey == EPtiKeyQwertySpace )
                {
                response = ETrue;        
                }  
            }
        else
            {
#endif
          if(IsValidChineseInputSymbol(aKey, (TPtiEngineInputMode)EPtiEngineZhuyinPhraseQwerty))
            {
            response = ETrue;        
            }
        }
#ifdef RD_INTELLIGENT_TEXT_INPUT        
        }
#endif        
#ifdef RD_INTELLIGENT_TEXT_INPUT        
    
    if( iPtiEngine != NULL && EPtiKeyboardQwerty4x12 == iPtiEngine->KeyboardType())
        {
#endif
    if(iMode == EZhuyin && IsFlagSet(ESupportZhuyinPhrase) && aKey == EPtiKeyQwertyApostrophe)
        {
        response = ETrue;
        }
#ifdef RD_INTELLIGENT_TEXT_INPUT        
        }
#endif
    return response;
    }

TBool CAknFepUIManagerChinese::IsQwertyZhuyinToneMarkKey(TInt aKey) const
    {
    // only return ETrue when qwerty and zhuyin symbol input supported for corresponding key
    TInt supportSCTToneMarks = EFalse;
    MPtiCoreInfo* ptiCoreInfo = iPtiEngine->CoreInfo(EPtiEngineZhuyin);
#ifdef RD_INTELLIGENT_TEXT_INPUT        
    TPtiKeyboardType keyboardType = iPtiEngine->KeyboardType();
#endif

    if(ptiCoreInfo)
        {
        supportSCTToneMarks = ptiCoreInfo->CapsBits() & ESupportSCTToneMarks;
        }

    ptiCoreInfo = iPtiEngine->CoreInfo(EPtiEngineZhuyinByPhrase);
    if(ptiCoreInfo)    
        {
        supportSCTToneMarks = 
                    supportSCTToneMarks || (ptiCoreInfo->CapsBits() & ESupportSCTToneMarks);
        }

    if(iQwertyMode && (iMode == EZhuyin) && supportSCTToneMarks)
        {
#ifdef RD_INTELLIGENT_TEXT_INPUT 
        return IsValidChineseToneMarkKey(aKey, EPtiEngineZhuyinPhraseQwerty);            
#endif        
        }
    else 
        {
        return EFalse;
        }
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerChinese::GetLatestDeliberateSelection
// 
// 
// ---------------------------------------------------------------------------
//
TPtr CAknFepUIManagerChinese::GetLatestDeliberateSelection() const
    {
    // need to cast around constness, as this is a const get but needs to call Des which isn't const
    typedef TBufC<MAknFepUICtrlInputPane::EMaxInputCharsPinyinPopupNotIncludingToneMark> pinyinBuffer;
    pinyinBuffer* bufPtr = CONST_CAST(pinyinBuffer*, &iLatestDeliberateSelection);
    return TPtr(bufPtr->Des());
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerChinese::FepUIState
// 
// 
// ---------------------------------------------------------------------------
//
TAknFepInputStateBase* CAknFepUIManagerChinese::FepUIState()
    {
    return &iFepUiState;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerChinese::UIContainer
// 
// 
// ---------------------------------------------------------------------------
//
MAknFepUICtrlContainerChinese* CAknFepUIManagerChinese::UIContainer() const
    {
    return iContainerPane;
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerChinese::IsValidShiftKeyPress
// 
// 
// ---------------------------------------------------------------------------
//
TBool CAknFepUIManagerChinese::IsValidShiftKeyPress() const
    {
    TUIState state = State();
    TInt mode = FepMan()->InputMode();
#ifdef RD_INTELLIGENT_TEXT_INPUT        
    TPtiKeyboardType kbdType = iFepMan->KeyboardLayout();
    if (EPtiKeyboardQwerty4x12 == kbdType)
        {
#endif
    if (mode == EPinyin || mode == EZhuyin || mode == EStroke || mode == ECangJie)
        {
        if (mode == EZhuyin && state == EPredictiveInput)
            {
            return EFalse;
            }
        if (mode == EZhuyin && state == EPredictiveCandidate)
            {
            return ETrue;
            }
        
        if (state == EEntry || state == ECandidate || state == EPredictiveCandidate)
            {
            return EFalse;
            }
        }
#ifdef RD_INTELLIGENT_TEXT_INPUT        
        }
    else if(EPtiKeyboardQwerty4x10 == kbdType ||
            EPtiKeyboardQwerty3x11 == kbdType ||
            EPtiKeyboardHalfQwerty == kbdType ||
            EPtiKeyboardCustomQwerty == kbdType)
        {
    	if (mode == EPinyin || mode == EZhuyin || mode == EStroke || mode == ECangJie)
            {
            if ( state == EEntry || state == ECandidate || /*state == EPredictiveCandidate ||*/ 
                state == EPredictiveInput || state == EMiniQwertyEdit || 
                state == EZhuyinSpelling || state == EZhuyinCandidate )
                {
                return EFalse;
                }         
            }
        }
#endif    
    
    return ETrue;
    }

#ifdef RD_INTELLIGENT_TEXT_INPUT        
TBool CAknFepUIManagerChinese::IsValidFnKeyPress() const
    {
    TUIState uiState = State();
    TInt mode = FepMan()->InputMode();
    if (mode == EPinyin || mode == EZhuyin || mode == EStroke || mode == ECangJie)
        {
        //if (uiState == EPredictiveCandidate || uiState == ECandidate)
        if (uiState == EEntry || 
        	uiState == ECandidate ||
        	uiState == EPredictiveCandidate || 
            uiState == EPredictiveInput ||
            uiState == EMiniQwertyEdit || uiState == EZhuyinSpelling ||
            uiState == EZhuyinCandidate )
            {
            return EFalse;
            }         
        }
    							   
    return ETrue;
    }

TBool CAknFepUIManagerChinese::IsValidLongChrKeyPress() const
    {
    TUIState state = State();
    TInt mode = FepMan()->InputMode();
    if ( mode == EPinyin || mode == EZhuyin || mode == EStroke )
        {
        if ( state == EEntry || state == EPredictiveInput )
            {
            return EFalse;
            }
        }
    return ETrue;
    }

#endif

// ---------------------------------------------------------------------------
// CAknFepUIManagerChinese::SetInputLanguageL
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerChinese::SetInputLanguageL(TLanguage aLanguage)
    {
    iLanguage = aLanguage;
    CloseUI();
    }


// ---------------------------------------------------------------------------
// CAknFepUIManagerChinese::CAknFepUIManagerChinese
// 
// 
// ---------------------------------------------------------------------------
//
CAknFepUIManagerChinese::CAknFepUIManagerChinese(MAknFepManagerUIInterface* aFepMan, 
                                                 CAknFepCaseManager* aCaseMan)
    : CAknFepUIManagerBase(aFepMan, aCaseMan),
      iFepUiState(NULL, NULL),
      iPrevMode(ENumber),
      iQwertyMode(ETrue)
    {
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerChinese::ConstructL
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerChinese::ConstructL(TLanguage aLanguage)
    {
    CAknFepUIManagerBase::ConstructL(aLanguage);
    iContainerPane = FepMan()->UiInterface()->NewAknFepUICtrlContainerChineseL();
    ResetFlags();  
    }

// ---------------------------------------------------------------------------
// CAknFepUIManagerChinese::QueryPtiEngine
// 
// 
// ---------------------------------------------------------------------------
//
void CAknFepUIManagerChinese::QueryPtiEngine(void)
    {
    MPtiLanguage* ptiLang = iPtiEngine->CurrentLanguage();

    // query ptiengine 
    if(ptiLang)
        {
        SetFlag(EQueryPtiEngineAlready);
        if(ptiLang->HasInputMode(EPtiEnginePinyinByPhrase))
            {
            SetFlag(ESupportPinyinPhrase);
            }
            
        if(ptiLang->HasInputMode(EPtiEngineZhuyinByPhrase))
            {
            SetFlag(ESupportZhuyinPhrase);
            }
            
        if (ptiLang->HasInputMode(EPtiEngineStrokeByPhrase))
            {
            SetFlag(ESupportStrokePhrase);
            }
        }
    }

void CAknFepUIManagerChinese::ChangeStatetItut(TUIState aState)
    {
    switch(aState)
        {
        case EInitial:
            {
            switch(iMode)
            {
            case EPinyin:
            case EZhuyin:
            case EStroke:
            case ECangJie: 
                iFepUiState = TAknFepInputStateInitialChineseGeneric(this, iContainerPane);
                break;
            case EZhuyinFind:
                iFepUiState = TAknFepInputStateInitialZhuyinFind(this, iContainerPane);
                break;
            case EStrokeFind:
                iFepUiState = TAknFepInputStateInitialStrokeFind(this, iContainerPane);
                break;
            case ELatin:
                {
                if (iPredictive)
                    {
                    iFepUiState = TAknFepInputStateInitialWesternPredictive(this);
                    }
                else
                    {
                    iFepUiState = TAknFepInputStateInitialLatinMultitap(this);
                    }
                }
                break;
            case ENumber:
                {
                // Map directly to the resources...
                iFepUiState = TAknFepInputStateInitialNumber(this);
                break;
                }
            default:
                AknFepPanic(EAknFepPanicBadInputState);
            }
            break;
            }
        case EEntry:
            {
            switch(iMode)
                {
                case EPinyin:
                    if(IsFlagSet(ESupportPinyinPhrase))
                        {
                        iFepUiState = TAknFepInputStateEntryPinyinPhrase(this,iContainerPane);
                        }
                    else 
                        {
                        iFepUiState = TAknFepInputStateEntryPinyin(this,iContainerPane);
                        }
                    break;
                case EZhuyin:
                    if(IsFlagSet(ESupportZhuyinPhrase))
                        {	
                        iFepUiState = TAknFepInputStateEntryZhuyinPhrase(this,iContainerPane);
                        }
                    else
                        {
                        iFepUiState = TAknFepInputStateEntryZhuyin(this,iContainerPane);
                        }
                    break;
                case EStroke:
                    if (IsFlagSet(ESupportStrokePhrase))
                        {
                        iFepUiState = TAknFepInputStateEntryStrokePhrase(this,iContainerPane);
                        }
                    else
                        {
                        iFepUiState = TAknFepInputStateEntryStroke(this,iContainerPane);
                        }
                    break;
                case ELatin:
                    iFepUiState = TAknFepInputStateEntryWesternPredictive(this);
                    break;
                default:
                    AknFepPanic(EAknFepPanicBadInputState);
                }
            }
            break;
        case ECandidate:
            {
            // modify for phrase creation
            FepMan()->EntryPhrase( ETrue );
            FepMan()->EnableKeyStar( EFalse );
            switch ( iMode )
                {
                case EPinyin:
                    {
                    if ( IsFlagSet( ESupportPinyinPhrase ) )
                        {
                        iFepUiState
                            = TAknFepUiInputStateCandidatePinyinAndStrokePhrase(
                                this, iContainerPane );
                        }
                    }
                    break;
                case EStroke:
                    {
                    if ( IsFlagSet( ESupportStrokePhrase ) )
                        {
                        iFepUiState
                            = TAknFepInputStateCandidateStrokePhrase(
                                this, iContainerPane );
                        }
                    }
                    break;
                case EZhuyin:
                    {
                    if ( IsFlagSet( ESupportZhuyinPhrase ) )
                        {
                        iFepUiState
                            = TAknFepInputStateCandidateChinesePhrase( this,
                                iContainerPane );
                        }
                    }
                    break;
                default:
                    {
                    iFepUiState = TAknFepInputStateCandidateChinese( this,
                        iContainerPane );
                    }
                }
            }
            break;
            
        case EPredictiveCandidate:
            {
            // add for phrase creation
            FepMan()->PhraseCreation( EFalse );
            FepMan()->PinyinPhraseCreation( EFalse );
            FepMan()->EntryPhrase( ETrue );
            FepMan()->EnableKeyStar( ETrue );
            TRAP_IGNORE( FepMan()->UpdateCbaL( R_AKNFEP_SOFTKEYS_PHRASE_CREATION_SELECT_CANCEL_SELECT ) );
            iContainerPane->PinyinPopupWindow()->ResetStrokeArray();
            switch(iMode)
                {
                case EPinyin:
                    if(IsFlagSet(ESupportPinyinPhrase))
                        {
                        iFepUiState = TAknFepInputStatePredictiveCandidatePinyinPhrase(
                                                                    this,iContainerPane);
                        }
                    else
                        {
                        iFepUiState = TAknFepInputStatePredictiveCandidatePinyin(
                                                             this,iContainerPane);     
                        }
                    break;
                case EStroke:
                    if (IsFlagSet(ESupportStrokePhrase))
                        {
                        iFepUiState = TAknFepInputStatePredictiveCandidateStrokePhrase(
                                                                   this,iContainerPane);     
                        }
                    else
                        {
                        iFepUiState = TAknFepInputStatePredictiveCandidateStroke(
                                                              this,iContainerPane);     
                        } 
                    break;
                case EZhuyin:
                        if(IsFlagSet(ESupportZhuyinPhrase))
                            {
                            iFepUiState = TAknFepInputStatePredictiveCandidateChinesePhrase(
                                                                        this,iContainerPane);
                            }
                        else
                            {
                            iFepUiState = TAknFepInputStatePredictiveCandidate(this,iContainerPane);
                            }
                    break;
                default:
                AknFepPanic(EAknFepPanicBadInputState);
                }
            }
            break;
        case EPredictiveInput:
            if((IsFlagSet(ESupportPinyinPhrase) || IsFlagSet(ESupportZhuyinPhrase) 
             || IsFlagSet(ESupportStrokePhrase)) 
               && (iMode != ECangJie))
                {
                iFepUiState = TAknFepInputStatePredictiveInputChinesePhrase(this, iContainerPane);
                }
            else
                {
                iFepUiState = TAknFepInputStatePredictiveInputChinese(this, iContainerPane);
                }
            break;
        // add for phrase creation
        case EStrokeSequenceEdit:
            if ( IsFlagSet( ESupportStrokePhrase ) && ( iMode != ECangJie ) )
                {
                FepMan()->EnableKeyStar( EFalse );
                iFepUiState = TAknFepInputStateEditStrokePhrase( this, iContainerPane ); 
                }
            break;
        // add for phrase creation
        case EKeySequenceEdit:
            if ( IsFlagSet( ESupportPinyinPhrase ) && ( iMode != ECangJie ) )
                {
                iFepUiState = TAknFepInputStateEntryPinyinPhraseCreation( this, iContainerPane );
                }
            break;
        // add for phrase creation
        case EZhuyinSequenceEdit:
            if ( IsFlagSet( ESupportZhuyinPhrase ) && ( iMode != ECangJie ) )
                {
                iFepUiState = TAknFepInputStateEntryZhuyinPhraseCreation( this, iContainerPane );
                }
            break;
        // add for phrase creation
        case ESpellingSelection:
            if ( IsFlagSet( ESupportPinyinPhrase ) && (iMode != ECangJie) )
                {
                FepMan()->EnableKeyStar( EFalse );
                iFepUiState = TAknFepInputStateSpellingPinyinPhraseCreation( this, iContainerPane );
                }
            break;
        // add for phrase creation
        case ECandidateSelection:
            if ( IsFlagSet (ESupportPinyinPhrase) && ( iMode != ECangJie ) )
                {
                FepMan()->EnableKeyStar( EFalse );
                iFepUiState = TAknFepInputStateCandidatePinyinPhraseCreation( this, iContainerPane );
                }
            break;
        default:
            AknFepPanic(EAknFepPanicBadInputState);
        }
    
    }
    
void CAknFepUIManagerChinese::ChangeStateQwerty(TUIState aState)
    {
#ifdef RD_INTELLIGENT_TEXT_INPUT        
    TPtiKeyboardType kbdType = iFepMan->KeyboardLayout();    
#endif    
    
    // Get current core id. It is used to judge whether sogou core is in use.
    TInt coreID = 0;
    TRAP_IGNORE( coreID = PtiEngine()->HandleCommandL( EPtiCommandGetCoreID ));

    switch(aState)
        {
        case EQwerty:
            if(iMode == EPinyin || iMode == EStroke || iMode == EZhuyin || iMode == ECangJie)
                {
                iFepUiState = TAknFepInputStateInitialChineseGenericQwerty(this, iMode);
                }
            else if(iMode == EStrokeFind)
                {
#ifdef RD_INTELLIGENT_TEXT_INPUT 
                if(EPtiKeyboardQwerty4x10 == kbdType ||
                   EPtiKeyboardQwerty3x11 == kbdType)
                    {
                    iFepUiState = TAknFepInputStateStrokeFindMiniQwerty(this,iMode);
                    }
                else
                    {
#endif //RD_INTELLIGENT_TEXT_INPUT                
                iFepUiState = TAknFepInputStateStrokeFindQwerty(this,iContainerPane);
#ifdef RD_INTELLIGENT_TEXT_INPUT 
                    }
#endif                
                }
            else if(iMode == EZhuyinFind)
                {
#ifdef RD_INTELLIGENT_TEXT_INPUT 
                if(EPtiKeyboardQwerty4x10 == kbdType ||
                   EPtiKeyboardQwerty3x11 == kbdType)
                    {
                    iFepUiState = TAknFepInputStateZhuyinFindMiniQwerty(this,iMode);
                    }
                else
                    {
#endif //RD_INTELLIGENT_TEXT_INPUT                 
                iFepUiState = TAknFepInputStateZhuyinFindQwerty(this,iContainerPane);
#ifdef RD_INTELLIGENT_TEXT_INPUT 
                    }
#endif  
                }
            break;
#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifdef __HALF_QWERTY_KEYPAD
        case EHalfQwerty: 
            if (iMode == EPinyin || iMode == EStroke || iMode == EZhuyin)
                {
                iFepUiState = TAknFepInputStateInitialChineseGenericHalfQwerty(
                        this, iMode);
                }
            else if(iMode ==EStrokeFind)
                {
                iFepUiState = TAknFepInputStateStrokeFindHalfQwerty(this,
                                            iMode);
                }    
            else if(iMode ==EZhuyinFind)
                {
                iFepUiState = TAknFepInputStateZhuyinFindHalfQwerty(this,
                                                            iMode);
                }
            break;
#endif
#endif
        case EEntry:
            {
            switch(iMode)
                {
                case EZhuyin:
                    if(IsFlagSet(ESupportZhuyinPhrase))
                        {
#ifdef RD_INTELLIGENT_TEXT_INPUT        
                        if ( EPtiKeyboardQwerty4x10 == kbdType ||
                             EPtiKeyboardQwerty3x11 == kbdType ||
                             EPtiKeyboardCustomQwerty == kbdType)
                           {
                           iFepUiState = TAknFepInputStateEntryMiniQwertyZhuyinPhrase(this, iContainerPane); 
                           }
                        else if(EPtiKeyboardHalfQwerty == kbdType)
                           {
                           iFepUiState = TAknFepInputStateEntryHalfQwertyZhuyinPhrase(this, iContainerPane);
                           }
                        else
                           {
#endif
                        iFepUiState = TAknFepInputStateEntryQwertyZhuyinPhrase(this, 
                                                                               iContainerPane);
#ifdef RD_INTELLIGENT_TEXT_INPUT        
                           }   
#endif
                        }
                    else
                        {
                        iFepUiState = TAknFepInputStateEntryQwertyZhuyin(this, iContainerPane);
                        }
                    
                    break;
                case EPinyin:
                    if(IsFlagSet(ESupportPinyinPhrase))
                        {
#ifdef RD_INTELLIGENT_TEXT_INPUT        
                        if ( EPtiKeyboardQwerty4x10 == kbdType ||
                             EPtiKeyboardQwerty3x11 == kbdType ||
                             EPtiKeyboardCustomQwerty == kbdType)                            
                            { 
                            iFepUiState = TAknFepInputStateEntryMiniQwertyPinyinPhrase(this, iContainerPane);
                            }
                        else if (EPtiKeyboardHalfQwerty == kbdType)
                            {
                            iFepUiState = TAknFepInputStateEntryHalfQwertyPinyinPhrase(this, iContainerPane);
                            }
                        else
                            {
#endif
                        iFepUiState = TAknFepInputStateEntryQwertyPinyinPhrase(
                                                           this, iContainerPane);
#ifdef RD_INTELLIGENT_TEXT_INPUT        
                            }
#endif
                        }
                    else
                        {
                        iFepUiState = TAknFepInputStateEntryQwertyPinyin(this, iContainerPane);
                        }
                    break;
                case EStroke:
                    if (IsFlagSet(ESupportStrokePhrase))
                        {
#ifdef RD_INTELLIGENT_TEXT_INPUT        
                        if ( EPtiKeyboardQwerty4x10 == kbdType ||
                             EPtiKeyboardQwerty3x11 == kbdType ||
                             EPtiKeyboardCustomQwerty == kbdType)                        	
                            {
                            iFepUiState = TAknFepInputStateEntryMiniQwertyStrokePhrase(
                                                                  this, iContainerPane);
                            // If sogou core is in use, set plugin to the state
                            // machine and enable the plugin.
                            if ( TUid::Uid( coreID ) == KPtiSogouCoreUid )
                                {
                                TAknFepInputMiniQwertySogouStrokePhrasePlugin plugin( this, iContainerPane, EEntry );
                                plugin.Enable( ETrue );
                                iFepUiState.SetStrokePlugin( plugin );
                                }
                        	}
                        else if (EPtiKeyboardHalfQwerty == kbdType)
                            {
                            iFepUiState= TAknFepInputStateEntryHalfQwertyStrokePhrase(this, iContainerPane);
                            }
                        else
 	                       {
#endif
                        	iFepUiState = TAknFepInputStateEntryQwertyStrokePhrase(
                                                             this, iContainerPane);
#ifdef RD_INTELLIGENT_TEXT_INPUT        
 	                       }	
#endif
                        }
                    else
                        {
                        iFepUiState = TAknFepInputStateEntryQwertyStroke(this, iContainerPane);
                        }
                    break;
                case ECangJie:
#ifdef RD_INTELLIGENT_TEXT_INPUT        
                        if ( EPtiKeyboardQwerty4x10 == kbdType ||
                             EPtiKeyboardQwerty3x11 == kbdType ||
                             EPtiKeyboardHalfQwerty == kbdType ||
                             EPtiKeyboardCustomQwerty == kbdType)                    	
                        {
                        iFepUiState = TAknFepInputStateEntryMiniQwertyCangJie(this, iContainerPane);                    	
                    	}
                    else
                    	{
#endif
                    	iFepUiState = TAknFepInputStateEntryQwertyCangJie(this, iContainerPane);
#ifdef RD_INTELLIGENT_TEXT_INPUT        
                    	}
#endif
                    break;
                default:
                    AknFepPanic(EAknFepPanicBadInputState);
                }
            }
            break;
        case ECandidate:
            if(iMode == EZhuyin)
                {
                if(IsFlagSet(ESupportZhuyinPhrase))
                    {
#ifdef RD_INTELLIGENT_TEXT_INPUT        
                        if ( EPtiKeyboardQwerty4x10 == kbdType ||
                             EPtiKeyboardQwerty3x11 == kbdType ||
                             EPtiKeyboardCustomQwerty == kbdType)                    	
                    	{
                    	iFepUiState = TAknFepUiInputStateCandidateMiniQwertyZhuyinPhrase(
                                                                  this, iContainerPane);
                    	}
#ifdef __HALF_QWERTY_KEYPAD
                        else if ( EPtiKeyboardHalfQwerty == kbdType )
                            {
                            iFepUiState = TAknFepUiInputStateCandidateHalfQwertyChinesePhrase(
                                                                                              this, iContainerPane);
                            }
#endif
                    else
         	            {
#endif
         	            iFepUiState = TAknFepInputStateCandidateQwertyZhuyinPhrase(
                                                            this, iContainerPane);
#ifdef RD_INTELLIGENT_TEXT_INPUT        
         	            }	
#endif
                    }
                else
                    {
                    iFepUiState = TAknFepInputStateCandidateQwertyZhuyin(this, iContainerPane);
                    }
                }
#ifdef RD_INTELLIGENT_TEXT_INPUT        
            else if(iMode == EPinyin)
                {
                if(IsFlagSet(ESupportPinyinPhrase))
                    {
                        if ( EPtiKeyboardQwerty4x10 == kbdType ||
                             EPtiKeyboardQwerty3x11 == kbdType ||
                             EPtiKeyboardCustomQwerty == kbdType)                    	
                        { 
                        iFepUiState = TAknFepUiInputStateCandidateMiniQwertyPinyinAndStrokePhrase(this, iContainerPane);
                        }
                        else if(EPtiKeyboardHalfQwerty == kbdType)
                        {
                        iFepUiState=TAknFepUiInputStateCandidateHalfQwertyChinesePhrase(
                                                                             this, iContainerPane);
                        }
                    }                 
                }
            else if(iMode == EStroke)
            	{
                 if (IsFlagSet(ESupportStrokePhrase))
                        {
                        if ( EPtiKeyboardQwerty4x10 == kbdType ||
                             EPtiKeyboardQwerty3x11 == kbdType ||
                             EPtiKeyboardCustomQwerty == kbdType)                    	
                        	{
                            iFepUiState = TAknFepUiInputStateCandidateMiniQwertyStrokePhrase(this, iContainerPane);
                            // If sogou core is in use, set plugin to the state
                            // machine and enable the plugin.
                            if ( TUid::Uid( coreID ) == KPtiSogouCoreUid )
                                {
                                TAknFepInputMiniQwertySogouStrokePhrasePlugin plugin( this, iContainerPane, ECandidate );
                                plugin.Enable( ETrue );
                                iFepUiState.SetStrokePlugin( plugin );
                                }                        
                        	}
                        else if ( EPtiKeyboardHalfQwerty == kbdType )
                            {
                            iFepUiState=TAknFepUiInputStateCandidateHalfQwertyChinesePhrase( this, iContainerPane);
                            }
                       }
            	}
	        else if(iMode == ECangJie)
	        	{
                if ( EPtiKeyboardQwerty4x10 == kbdType ||
                     EPtiKeyboardQwerty3x11 == kbdType ||
                     EPtiKeyboardHalfQwerty == kbdType ||
                     EPtiKeyboardCustomQwerty == kbdType)                    	
                {
                iFepUiState = TAknFepUiInputStateCandidateMiniQwertyPinyinAndStrokePhrase(
                                                                     this, iContainerPane);
                }
	        	}
#endif      
            break;
        case EPredictiveCandidate:
            if(iMode == EZhuyin)
                {
                if(IsFlagSet(ESupportZhuyinPhrase))
                    {
#ifdef RD_INTELLIGENT_TEXT_INPUT        
                    if ( EPtiKeyboardQwerty4x10 == kbdType ||
                         EPtiKeyboardQwerty3x11 == kbdType ||
                         EPtiKeyboardCustomQwerty == kbdType)                    	
                        {
                    	iFepUiState = TAknFepInputStatePredictiveCandidateMiniQwertyChinesePhrase(
                                                                          this, iContainerPane);
                    	}
#ifdef __HALF_QWERTY_KEYPAD
                    else if( EPtiKeyboardHalfQwerty == kbdType )
                        {
                        iFepUiState = TAknFepInputStatePredictiveCandidateHalfQwertyChinesePhrase(
                                                                           this, iContainerPane);
                        }
#endif
                    else
                    	{
#endif
                    	iFepUiState = TAknFepInputStatePredictiveCandidateQwertyZhuyinPhrase(
                                                                      this, iContainerPane);                    	
#ifdef RD_INTELLIGENT_TEXT_INPUT        
                        }	
#endif
                    }
                else
                    {
                    iFepUiState = TAknFepInputStatePredictiveCandidateQwertyZhuyin(
                                                              this, iContainerPane);
                    }
                
                }
#ifdef RD_INTELLIGENT_TEXT_INPUT        
            else if(iMode == EPinyin)
                {
                if(IsFlagSet(ESupportPinyinPhrase))
                    {
                    if ( EPtiKeyboardQwerty4x10 == kbdType ||
                         EPtiKeyboardQwerty3x11 == kbdType ||
                         EPtiKeyboardCustomQwerty == kbdType)                    	
                        {
                        iFepUiState = TAknFepInputStatePredictiveCandidateMiniQwertyChinesePhrase(
                                                                             this, iContainerPane);

                        // If sogou core is in use, set plugin to the state
                        // machine and enable the plugin.                        
                        if ( TUid::Uid( coreID ) == KPtiSogouCoreUid )
                            {
                            TAknFepInputMiniQwertySogouPinyinPhrasePlugin plugin( this, iContainerPane, EPredictiveCandidate );
                            plugin.Enable( ETrue );
                            iFepUiState.SetPlugin( plugin );
                            }                       
                        }
#ifdef __HALF_QWERTY_KEYPAD
                    else if( EPtiKeyboardHalfQwerty == kbdType )
                        {
                        iFepUiState = TAknFepInputStatePredictiveCandidateHalfQwertyChinesePhrase(
                                                                           this, iContainerPane);
                        }
#endif
                    }                 
                }
           else if(iMode == EStroke)
            	{
                 if (IsFlagSet(ESupportStrokePhrase))
                        {
                        if ( EPtiKeyboardQwerty4x10 == kbdType ||
                             EPtiKeyboardQwerty3x11 == kbdType ||
                             EPtiKeyboardCustomQwerty == kbdType)                    	
                        	{
                            iFepUiState = 
                                   TAknFepInputStatePredictiveCandidateMiniQwertyChinesePhrase(
                                                                          this, iContainerPane);
                            // If sogou core is in use, set plugin to the state
                            // machine and enable the plugin.
                            if ( TUid::Uid( coreID ) == KPtiSogouCoreUid )
                                {
                                TAknFepInputMiniQwertySogouStrokePhrasePlugin plugin( this, iContainerPane, EPredictiveCandidate );
                                plugin.Enable( ETrue );
                                iFepUiState.SetStrokePlugin( plugin );
                                }                   	
                        	}
#ifdef __HALF_QWERTY_KEYPAD
                    else if( EPtiKeyboardHalfQwerty == kbdType )
                        {
                        iFepUiState = TAknFepInputStatePredictiveCandidateHalfQwertyChinesePhrase(
                                                                           this, iContainerPane);
                        }
#endif
                       }
	
            	}
            else if(iMode == ECangJie)
            	{

                if ( EPtiKeyboardQwerty4x10 == kbdType ||
                     EPtiKeyboardQwerty3x11 == kbdType ||
                     EPtiKeyboardHalfQwerty == kbdType ||
                     EPtiKeyboardCustomQwerty == kbdType)                    	
                 {
                 iFepUiState = TAknFepInputStatePredictiveCandidateMiniQwertyChinesePhrase(
                                                                       this, iContainerPane);
                 }

            	}
#endif
            break;
        case EPredictiveInput:
            {
            switch(iMode)
                {
                case EPinyin:
                    if(IsFlagSet(ESupportPinyinPhrase))
                        {
#ifdef RD_INTELLIGENT_TEXT_INPUT        
                        if ( EPtiKeyboardQwerty4x10 == kbdType ||
                             EPtiKeyboardQwerty3x11 == kbdType ||
                             EPtiKeyboardHalfQwerty == kbdType ||
                             EPtiKeyboardCustomQwerty == kbdType)                    	
                            {
                            iFepUiState = TAknFepInputStatePredictiveInputMiniQwertyChinesePhrase/*TAknFepInputStatePredictiveInputQwertyPinyinPhrase*/(this, iContainerPane);
                            
                            // If sogou core is in use, set plugin to the state
                            // machine and enable the plugin.                            
                            if ( TUid::Uid( coreID ) == KPtiSogouCoreUid )
                                {
                                TAknFepInputMiniQwertySogouPinyinPhrasePlugin plugin( this, iContainerPane, EPredictiveInput );
                                plugin.Enable( ETrue );
                                iFepUiState.SetPlugin( plugin );
                                }                         
                            }
                        else
                            {
#endif
                            iFepUiState = TAknFepInputStatePredictiveInputQwertyPinyinPhrase(
                                                                        this, iContainerPane);
#ifdef RD_INTELLIGENT_TEXT_INPUT        
							}
#endif
                        }
                    else
                        {
                        iFepUiState = TAknFepInputStatePredictiveInputQwertyPinyin(
                                                              this, iContainerPane);
                        }
                    break;
                case EStroke:                
                    if(IsFlagSet(ESupportStrokePhrase))
                        {
#ifdef RD_INTELLIGENT_TEXT_INPUT        
                        if ( EPtiKeyboardQwerty4x10 == kbdType ||
                             EPtiKeyboardQwerty3x11 == kbdType ||
                             EPtiKeyboardHalfQwerty == kbdType ||
                             EPtiKeyboardCustomQwerty == kbdType)                    	
                      	   {
                           iFepUiState = TAknFepInputStatePredictiveInputMiniQwertyChinesePhrase(
                                                                             this, iContainerPane);
                           // If sogou core is in use, set plugin to the state
                           // machine and enable the plugin.
                           if ( TUid::Uid( coreID ) == KPtiSogouCoreUid )
                               {
                               TAknFepInputMiniQwertySogouStrokePhrasePlugin plugin( this, iContainerPane, EPredictiveInput );
                               plugin.Enable( ETrue );
                               iFepUiState.SetStrokePlugin( plugin );
                               }                      	   
                      	   }
                        else
         	               {
#endif
         	               iFepUiState = TAknFepInputStatePredictiveInputQwertyStrokePhrase(
                                                                     this, iContainerPane);                        	
#ifdef RD_INTELLIGENT_TEXT_INPUT        
         	               }
#endif
                        }
                    else
                        {
                        iFepUiState = TAknFepInputStatePredictiveInputQwertyStroke(
                                                              this, iContainerPane);
                        }
                    break;
                case ECangJie:
#ifdef RD_INTELLIGENT_TEXT_INPUT        
                    if ( EPtiKeyboardQwerty4x10 == kbdType ||
                         EPtiKeyboardQwerty3x11 == kbdType ||
                         EPtiKeyboardHalfQwerty == kbdType ||
                         EPtiKeyboardCustomQwerty == kbdType)                    	
                        {
                        iFepUiState = TAknFepInputStatePredictiveInputMiniQwertyChinesePhrase(
                                                                          this,iContainerPane);

                        }
                    else 
                    	{
#endif
                    	iFepUiState = TAknFepInputStatePredictiveInputQwerty(this, iContainerPane);
#ifdef RD_INTELLIGENT_TEXT_INPUT        
                    	}    
#endif
                    break;
                case EZhuyin:
                    if(IsFlagSet(ESupportZhuyinPhrase))
                        {
#ifdef RD_INTELLIGENT_TEXT_INPUT        
                        if ( EPtiKeyboardQwerty4x10 == kbdType ||
                             EPtiKeyboardQwerty3x11 == kbdType ||
                             EPtiKeyboardHalfQwerty == kbdType ||
                             EPtiKeyboardCustomQwerty == kbdType)                    	
                        	{
                            iFepUiState = TAknFepInputStatePredictiveInputMiniQwertyChinesePhrase/*TAknFepInputStatePredictiveInputQwertyPinyinPhrase*/(this, iContainerPane);
                        	}
                        else
                        	{
#endif
                        	iFepUiState = TAknFepInputStatePredictiveInputQwertyZhuyinPhrase(
                                                                       this, iContainerPane);
#ifdef RD_INTELLIGENT_TEXT_INPUT        
                        	}
#endif
                        }
                    else
                        {
                        iFepUiState = TAknFepInputStatePredictiveInputQwertyZhuyin(
                                                              this, iContainerPane);
                        }
                    break;  
                default:
                    AknFepPanic(EAknFepPanicBadInputState);
                }
            }
            break;
#ifdef RD_INTELLIGENT_TEXT_INPUT 
        case EMiniQwertyEdit:
            {
            switch(iMode)
                {
                case EZhuyin:
                    if(IsFlagSet(ESupportZhuyinPhrase))
                        {
#ifdef RD_INTELLIGENT_TEXT_INPUT        
                        if ( EPtiKeyboardQwerty4x10 == kbdType ||
                             EPtiKeyboardQwerty3x11 == kbdType ||
                             EPtiKeyboardHalfQwerty == kbdType ||
                             EPtiKeyboardCustomQwerty == kbdType)
                           {
                           iFepUiState = TAknFepInputStateEditingMiniQwertyZhuyinPhrase( this, iContainerPane );
                           }
#endif
                        }
                    break;
                case EPinyin:
                    if(IsFlagSet(ESupportPinyinPhrase))
                        {
#ifdef RD_INTELLIGENT_TEXT_INPUT        
                        if ( EPtiKeyboardQwerty4x10 == kbdType ||
                             EPtiKeyboardQwerty3x11 == kbdType ||
                             EPtiKeyboardHalfQwerty == kbdType ||
                             EPtiKeyboardCustomQwerty == kbdType)                            
                            { 
                            iFepUiState = 
                                TAknFepInputStateEditinigMiniQwertyPinyinPhrase( 
                                                             this,
                                                             iContainerPane );
                            }                            
#endif
                        }
                    break;
                case EStroke:
                    if (IsFlagSet(ESupportStrokePhrase))
                        {
#ifdef RD_INTELLIGENT_TEXT_INPUT        
                        if ( EPtiKeyboardQwerty4x10 == kbdType ||
                             EPtiKeyboardQwerty3x11 == kbdType ||
                             EPtiKeyboardHalfQwerty == kbdType ||
                             EPtiKeyboardCustomQwerty == kbdType)                           
                            {
                            iFepUiState = 
                                TAknFepInputStateEditingMiniQwertyStrokePhrase(
                                                              this, 
                                                              iContainerPane);

                            // If sogou core is in use, set plugin to the state
                            // machine and enable the plugin.
                            if ( TUid::Uid( coreID ) == KPtiSogouCoreUid )
                                {
                                TAknFepInputMiniQwertySogouStrokePhrasePlugin plugin( this, iContainerPane, EMiniQwertyEdit );
                                plugin.Enable( ETrue );
                                iFepUiState.SetStrokePlugin( plugin );
                                }                             
                            }
#endif
                        }
                    break;
                default:
                    break;
                }   
            }
            break;
        case EZhuyinSpelling:
            {
            iFepUiState = TAknFepInputStateSpellingMiniQwertyZhuyin( this, iContainerPane );
            }
            break;
        case EZhuyinCandidate:
            {
            iFepUiState = TAknFepInputStateCanindateSelectingMiniQwertyZhuyin( this, iContainerPane );
            }
            break;
#endif
        default:
            AknFepPanic(EAknFepPanicBadInputState);
    }
}
    
void CAknFepUIManagerChinese::UpdateCangJieState()
    {
    // Reset mode
    if (iMode == ECangJie)
        {
        ChangeStateQwerty( EQwerty );
        }
    }

TBool CAknFepUIManagerChinese::IsValidChineseSymbol(TInt aKey) const
	{
	TBool respond = EFalse;
	if((aKey >= EStdKeyComma && aKey <= EStdKeyNkpPlus)|| aKey == EPtiKeyStar )
		{
		respond = ETrue;
		}
	return respond;
	}
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
#ifdef RD_INTELLIGENT_TEXT_INPUT
// ---------------------------------------------------------------------------
// CAknFepUIManagerChinese::RollbackPreviousCharL
// 
// 
// ---------------------------------------------------------------------------
//
TBool CAknFepUIManagerChinese::RollbackPreviousCharL()
    {
    if ( iInputSeqLenDeltaWithLastKey > 0 )
        {
        HandleKeyL( EStdKeyBackspace, EShortKeyPress );
        return ETrue;
        }
    else
        {
        return CAknFepUIManagerBase::RollbackPreviousCharL();
        }
    }
#endif // RD_INTELLIGENT_TEXT_INPUT
#endif // __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
// End of file
