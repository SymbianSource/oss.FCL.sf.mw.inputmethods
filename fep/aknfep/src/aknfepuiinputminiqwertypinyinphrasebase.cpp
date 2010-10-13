/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
 *      Provides the TAknFepInputStateStrokePhraseCreationBase definition.
 *
*/












// System includes
#include <e32cmn.h>
#include <PtiEngine.h>
#include <PtiDefs.h>
#include <avkon.rsg>
#include <aknfep.rsg>
#include <PtiUserDicEntry.h>
#include <aknnotewrappers.h> //CAknWarningNote 
// User includes
#include "AknFepUIManagerStateInterface.h"
#include "AknFepUICtrlInputPane.h"
#include "AknFepUICtrlPinyinPopup.h"        //Pinyin phrase
#include "aknfepuictrleditpane.h"           //pinyin phrase creation
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepManager.h"
#include "aknfepuiinputminiqwertypinyinphrasebase.h"

// Constant definition
const TInt KMaxPhraseCount = 50;
const TInt KMaxPhraseCreationCount = 7;
const TInt KMaxSpellLength = 7;//max pinyin length for one chinese Zi
const TInt KMaxKeystrokeCount = 31;
const TInt KInvalidToneMark = -1;
const TUint16 KAutoDLT = 0x002E;
const TUint16 KManualDLT = 0x0027;
const TUint16 KPYa = 0x0061;
const TUint16 KPYb = 0x0062;
const TUint16 KPYc = 0x0063;
const TUint16 KPYd = 0x0064;
const TUint16 KPYe = 0x0065;
const TUint16 KPYf = 0x0066;
const TUint16 KPYg = 0x0067;
const TUint16 KPYh = 0x0068;
const TUint16 KPYi = 0x0069;
const TUint16 KPYj = 0x006A;
const TUint16 KPYk = 0x006B;
const TUint16 KPYl = 0x006C;
const TUint16 KPYm = 0x006D;
const TUint16 KPYn = 0x006E;
const TUint16 KPYo = 0x006F;
const TUint16 KPYp = 0x0070;
const TUint16 KPYq = 0x0071;
const TUint16 KPYr = 0x0072;
const TUint16 KPYs = 0x0073;
const TUint16 KPYt = 0x0074;
const TUint16 KPYu = 0x0075;
const TUint16 KPYv = 0x0076;
const TUint16 KPYw = 0x0077;
const TUint16 KPYx = 0x0078;
const TUint16 KPYy = 0x0079;
const TUint16 KPYz = 0x007A;

_LIT( KMaxPhraseNote,"\x8BCD\x7EC4\x6700\x957F\x4E3A\x4E03\x5B57" );

const TMiniQwertyKeyMap StrokeMap[] =
    {
    {KPYa, EPtiKeyQwertyA},
    {KPYb, EPtiKeyQwertyB},
    {KPYc, EPtiKeyQwertyC},
    {KPYd, EPtiKeyQwertyD},
    {KPYe, EPtiKeyQwertyE},
    {KPYf, EPtiKeyQwertyF},
    {KPYg, EPtiKeyQwertyG},
    {KPYh, EPtiKeyQwertyH},
    {KPYi, EPtiKeyQwertyI},
    {KPYj, EPtiKeyQwertyJ},
    {KPYk, EPtiKeyQwertyK},
    {KPYl, EPtiKeyQwertyL},
    {KPYm, EPtiKeyQwertyM},
    {KPYn, EPtiKeyQwertyN},
    {KPYo, EPtiKeyQwertyO},
    {KPYp, EPtiKeyQwertyP},
    {KPYq, EPtiKeyQwertyQ},
    {KPYr, EPtiKeyQwertyR},
    {KPYs, EPtiKeyQwertyS},
    {KPYt, EPtiKeyQwertyT},
    {KPYu, EPtiKeyQwertyU},
    {KPYv, EPtiKeyQwertyV},
    {KPYw, EPtiKeyQwertyW},
    {KPYx, EPtiKeyQwertyX},
    {KPYy, EPtiKeyQwertyY},
    {KPYz, EPtiKeyQwertyZ},
    {KManualDLT, EPtiKeyQwertySpace}
    };

const TMiniQwertyToneMap PinyinToneMap[] =
    {
    {0x02C9, 1},
    {0x02CA, 2},
    {0x02C7, 3},
    {0x02CB, 4},
    {0x02D9, 5}
    };

// ---------------------------------------------------------------------------
// TAknFepInputStateStrokePhraseCreationBase::TAknFepInputStateStrokePhraseCreationBase
// C++ default constructor
// ---------------------------------------------------------------------------
//
TAknFepInputMiniQwertyPinyinPhraseBase::TAknFepInputMiniQwertyPinyinPhraseBase(
    MAknFepUIManagerStateInterface* aOwner,
    MAknFepUICtrlContainerChinese* aUIContainer) :
    TAknFepInputStateCandidateQwertyBaseChinesePhrase(aOwner, aUIContainer)
    {
    if (iOwner->PtiEngine()->InputMode() != EPtiEnginePinyinPhraseQwerty)
        {
        iOwner->PtiEngine()->SetInputMode(EPtiEnginePinyinPhraseQwerty);
        }
    iOwner->PtiEngine()->SetCandidatePageLength(KMaxPhraseCount);
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() && uiContainer->CandidatePane()->IsFirstPage());
    uiContainer->ShowVerticalScrollArrows(multiplePages);
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::HandleKeyL
// Handle system key press event.
// ---------------------------------------------------------------------------
//
TBool TAknFepInputMiniQwertyPinyinPhraseBase::HandleKeyL(TInt aKey,
    TKeyPressLength aLength)
    {
    //
    switch (aKey)
        {
        case EKeyLeftShift:
        case EKeyLeftCtrl:
        case EKeyRightCtrl:
        case EStdKeyDevice1:// CBA2    
            {
            iOwner->FepMan()->TryCloseUiL();
            break;
            }
        case EKeyRightFunc:
            {
            iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
            break;
            }
        case EStdKeyLeftFunc: //tone mark.
            {
            HandlToneMarkL();
            break;
            }
        case EStdKeyBackspace:
            {
            if (CheckKeyNeedRepeat(aLength) )
                {
                HandleKeyBackspaceL(aLength);
                }
            else
                {
                iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
                }
            break;
            }
        case EStdKeyEnter://Enter keypress.
            {
            HandlEnterKeyL();
            break;
            }
        case EStdKeyDevice0:// CBA1
        case EStdKeyDevice3:// CBASelect
            {
            HandleCommitL();
            break;
            }
        case EStdKeyUpArrow:
            {
            iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
            break;
            }
        case EStdKeyDownArrow:
            {
            if (EMiniQwertyEdit == iState)
                {
                if (CheckFirstGroupStroke() )
                    {
                    iOwner->ChangeState(EEntry);
                    }
                }
            else
                {
                iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
                }
            break;
            }
        case EStdKeyLeftArrow:
            {
            if (EMiniQwertyEdit == iState)
                {
                UIContainer()->EditPaneWindow()->SetAutoChangeStateFlag( EManualChangeState );
                MoveCursorLeft();
                }
            else
                {
                iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
                }
            break;
            }
        case EStdKeyRightArrow:
            {
            if (EMiniQwertyEdit == iState)
                {
                UIContainer()->EditPaneWindow()->SetAutoChangeStateFlag( EManualChangeState );
                MoveCursorRight();
                }
            else
                {
                iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
                }
            break;
            }
        case EStdKeySpace:
            {
            if (EShortKeyPress == aLength)
                {
                HandlSpaceKeyL();
                break;
                }
            break;
            }
        default:
            {
            if (CAknFepFnKeyManager::EFnKeyNext == iOwner->FepMan()->FnKeyState() || CAknFepFnKeyManager::EFnKeyLock
                    == iOwner->FepMan()->FnKeyState()
                    || ( CAknFepFnKeyManager::EFnKeyDown == iOwner->FepMan()->FnKeyState() && iState != ECandidate ))
                {
                iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
                return ETrue;
                }
            else
                if (iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagShiftKeyDepressed)
                        || iOwner->FepMan()->IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed))
                    {
                    iOwner->FepMan()->TryCloseUiL();
                    return EFalse;
                    }
                else
                    if ( (EShortKeyPress == aLength )
                            && (iOwner->IsValidChineseInputKeyQwerty(aKey) ))
                        {
                        if (iState == ECandidate)
                            {
                            HandleKeyFromCandidateL(aKey, aLength);
                            }
                        else
                            {
                            HandleDefaultKeyL(aKey);
                            }
                        }
                    else
                        {
                        iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
                        }
            break;
            }
        }

    return ETrue;
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::RefreshUI
// Update input window.
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::RefreshUI(TBool aRefresh)
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    // get cursor position
    if (aRefresh)
        {
        TPoint baseLine;
        TInt height(0);
        TInt ascent(0);
        TRAPD(ret,iOwner->FepMan()->GetScreenCoordinatesL(baseLine,height,ascent))
        ;
        if (ret == KErrNone)
            {
            uiContainer->SetContainerPosition(baseLine, height);
            }
        }
    if ( (uiContainer->EditPaneWindow()->KeystrokeArray()->Count() > 0 ) || 
        (uiContainer->EditPaneWindow()->PhraseArray()->Count() > 0 ) )
        {
        ShowInfoOnEEPPane();
        SetWarningColor();
        ShowInfoOnCandidatePane();
        UpdateIndicator();
        TRAP_IGNORE( ChangeCbaL() );
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::CheckFirstGroupStroke
// check the first group stroke of keystroke.
// ---------------------------------------------------------------------------
//
TBool TAknFepInputMiniQwertyPinyinPhraseBase::CheckFirstGroupStroke()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* showKeystroke = editPane->ShowKeystrokeArray();
    TInt stringBeforeLength = 0;
    TInt stringAfterLength = 0;
    TInt keyCode = 0;
    TInt toneMark = 0;
    if ( 0 == showKeystroke->Count() )
        {
        return EFalse;
        }

    if (KInvalidToneMark != ToneMark(showKeystroke->MdcaPoint( 0) ) )
        {
        return EFalse;
        }

    ClearPtiEngineKeystroke();
    for (TInt i = 0; i < showKeystroke->Count(); ++i)
        {
        if (CheckSpellingDLT(showKeystroke->MdcaPoint(i) ) )
            {
            if ( 0 == i)
                {
                return EFalse;
                }
            else
                {
                return ETrue;
                }
            }
        toneMark = ToneMark(showKeystroke->MdcaPoint(i) );
        if (KInvalidToneMark != toneMark)
            {
            for (TInt j = 0; j < toneMark; ++j)
                {
                ptiengine->IncrementToneMark(ETrue);
                TPtrC ptr0 = getCurrentLeastDLTSpell();
                if (ptr0.Length() > 0)
                    {
                    if ( 0
                            == ptr0.Mid( ptr0.Length() - 1, 1 ).Compare(showKeystroke->MdcaPoint(i) ) )
                        {
                        return ETrue;
                        }
                    }
                }
            TPtrC ptr = getCurrentLeastDLTSpell();
            if ( 0 == ptr.Mid( ptr.Length() - 1, 1 ).Compare(showKeystroke->MdcaPoint(i) ) )
                {
                return ETrue;
                }
            else
                {
                return EFalse;
                }
            }
        GetKeystrokeCode(keyCode, showKeystroke->MdcaPoint(i) );
        stringBeforeLength = getCurrentLeastDLTSpell().Length();
        ptiengine->AppendKeyPress((TPtiKey)keyCode).Length();
        stringAfterLength = getCurrentLeastDLTSpell().Length();
        //the keystroke is invalid.
        if (stringBeforeLength == stringAfterLength)
            {
            return EFalse;
            }
        }
    return ETrue;
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::HandlEnterKey
// Handle Enter key press
// ---------------------------------------------------------------------------
//
TInt TAknFepInputMiniQwertyPinyinPhraseBase::GetCursorPos()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* showKeystroke = editPane->ShowKeystrokeArray();
    CDesCArrayFlat* keystroke = editPane->KeystrokeArray();
    TInt phraseCount = editPane->PhraseArray()->Count();
    TInt index = editPane->GetCursorIndexOfKeystroke();
    TInt pos = 0;
    TInt count = 0;
    TBuf<1> autoDLT;
    autoDLT.Append(KAutoDLT);

    if ( 0 == index)
        {
        pos = phraseCount;
        }
    else
        if (keystroke->Count() == index)
            {
            pos = phraseCount + showKeystroke->Count();
            }
        else
            {
            for (TInt i = 0; i < showKeystroke->Count(); ++i)
                {
                if ( 0 != showKeystroke->MdcaPoint( i ).Compare(autoDLT) )
                    {
                    count = count + 1;
                    }
                if (count == index)
                    {
                    pos = phraseCount + i + 1;
                    break;
                    }
                }
            }
    return pos;
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::HandlEnterKeyL
// Handle Enter key press
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::HandlEnterKeyL()
    {
    TBuf<KMaxKeystrokeCount> currentText;
    MAknFepUICtrlEditPane* editpane = UIContainer()->EditPaneWindow();
    CDesCArrayFlat* showkeyStoke = editpane->ShowKeystrokeArray();
    CDesCArrayFlat* phraseArray = editpane->PhraseArray();
    TInt phraseCount = phraseArray->Count();
    TInt keyStrokeCount = showkeyStoke->Count();
    TInt i = 0;
    TBuf<1> autoDLT;
    autoDLT.Append(KAutoDLT);

    for (i = 0; i < phraseCount; i++)
        {
        currentText.Append(phraseArray->MdcaPoint(i) );
        }
    for (i = 0; i < keyStrokeCount; i++)
        {
        currentText.Append(showkeyStoke->MdcaPoint(i) );
        }

    while (currentText.Find(autoDLT)> 0)
        {
        currentText.Replace(currentText.Find(autoDLT), 1, KNullDesC);
        }

    if (currentText.Length() )
        {
        MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
        fepMan->NewTextL(currentText);
        fepMan->CommitInlineEditL();
        if (keyStrokeCount == 0 && currentText.Length() > 1)
            {
            AddPhraseToDB(currentText);
            }

        UIContainer()->EditPaneWindow()->ResetAllArray();
        UIContainer()->EditPaneWindow()->SetPhraseCreationFlag(EFalse);

        if (fepMan->IsFlagSet(CAknFepManager::EFlagEditorFull))
            {
            fepMan->ClearFlag(CAknFepManager::EFlagEditorFull);
            iOwner->FepMan()->TryCloseUiL();
            }
        else
            {
            iOwner->FepMan()->TryCloseUiL();
            }
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::HandlToneMarkL
// Handle tone mark key press.
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::HandlSpaceKeyL()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystrokeArray = editPane->KeystrokeArray();
    TInt phraseCount = editPane->PhraseArray()->Count();
    TInt showKeystrokeCount = editPane->ShowKeystrokeArray()->Count();
    TInt index = editPane->GetCursorIndexOfKeystroke();
    TBuf<1> manualDLT;
    TBuf<1> autoDLT;
    manualDLT.Append( KManualDLT );
    autoDLT.Append( KAutoDLT );
    
    TBool firstGroup = CheckFirstGroupStroke();

    keystrokeArray->InsertL(index, manualDLT);
    editPane->SetCursorIndexOfKeystroke(index + 1);
    AnalyseL();
    
    if ( editPane->ShowKeystrokeArray()->Count() > KMaxKeystrokeCount )
        {
        keystrokeArray->Delete( index );
        editPane->SetCursorIndexOfKeystroke( index );
        iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
        AnalyseL();
        return;
        }
    
    //Auto change from EMiniQwertyEdit state to EEntry state. 
    if (EMiniQwertyEdit == iState)
        {
        if ( ( !firstGroup ) && CheckFirstGroupStroke() )
            {
            if ( EAutoChangeStateFromCandidate == editPane->GetAutoChangeStateFlag() )
                {
                RefreshUI();
                editPane->SetChangeState(ETrue);
                iOwner->ChangeState(ECandidate);
                }
            else if ( EAutoChangeStateFromInput == editPane->GetAutoChangeStateFlag() )
                {
                editPane->SetChangeState(ETrue);
                iOwner->ChangeState(EEntry);
                }
            else
                {
                RefreshUI();
                }
            return;
            }
        }

    //Auto change from EEntry state to EMiniQwertyEdit state.
    if (EEntry == iState)
        {
        if ( !CheckFirstGroupStroke() )
            {
            editPane->SetAutoChangeStateFlag(EAutoChangeStateFromInput);
            editPane->SetChangeState(ETrue);
            iOwner->ChangeState(EMiniQwertyEdit);
            return;
            }
        }

    RefreshUI();
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::HandlToneMarkL
// Handle tone mark key press.
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::HandlToneMarkL()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    TBool firstGroup = CheckFirstGroupStroke();
    AddToneMarkL();
    AnalyseL();
    //Auto change from EMiniQwertyEdit state to EEntry state. 
    if (EMiniQwertyEdit == iState)
        {
        if ( ( !firstGroup ) && CheckFirstGroupStroke() )
            {
            if ( EAutoChangeStateFromCandidate == editPane->GetAutoChangeStateFlag() )
                {
                RefreshUI();
                editPane->SetChangeState(ETrue);
                iOwner->ChangeState(ECandidate);
                }
            else if ( EAutoChangeStateFromInput == editPane->GetAutoChangeStateFlag() )
                {
                editPane->SetChangeState(ETrue);
                iOwner->ChangeState(EEntry);
                }
            else
                {
                RefreshUI();
                }
            return;
            }
        }

    //Auto change from EEntry state to EMiniQwertyEdit state.
    if (EEntry == iState)
        {
        if ( !CheckFirstGroupStroke() )
            {
            editPane->SetAutoChangeStateFlag(EAutoChangeStateFromInput);
            editPane->SetChangeState(ETrue);
            iOwner->ChangeState(EMiniQwertyEdit);
            return;
            }
        }
    RefreshUI();
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::CheckManualDelimiter
// Check the delimiter that wether is manual delimiter.
// ---------------------------------------------------------------------------
//
TBool TAknFepInputMiniQwertyPinyinPhraseBase::CheckManualDelimiter(
    const TInt aCount, const TDesC& aKeystroke)
    {
    TInt keyCount = 0;
    for (TInt i = 0; i < aKeystroke.Length(); ++i)
        {
        if (KManualDLT != aKeystroke[i])
            {
            keyCount = keyCount + 1;
            }
        if (aCount == keyCount)
            {
            break;
            }
        }

    if (keyCount != aCount)
        {
        return EFalse;
        }

    if (aKeystroke.Length() == keyCount)
        {
        return EFalse;
        }

    if (KManualDLT != aKeystroke[keyCount])
        {
        return EFalse;
        }

    return ETrue;
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::AnalyseSpellingL
// Analyse Pinyin spelling.
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::AnalyseSpellingL(
    const TDesC& aKeystroke, const TDesC& aSpelling,
    CDesCArrayFlat* aShowKeystroke)
    {
    TInt count = 0;
    TBuf<1> manualDLT;
    TBuf<1> autoDLT;
    manualDLT.Append(KManualDLT);
    autoDLT.Append(KAutoDLT);

    if ( 0 == aSpelling.Compare(aKeystroke) )
        {
        for (TInt i = 0; i < aKeystroke.Length(); ++i)
            {
            aShowKeystroke->AppendL(aKeystroke.Mid(i, 1) );
            }
        return;
        }

    for (TInt i = 0; i < aSpelling.Length(); ++i)
        {
        if (KManualDLT == aSpelling[i])
            {
            //Manual delimiter
            if (CheckManualDelimiter(count, aKeystroke) )
                {
                aShowKeystroke->AppendL(manualDLT);
                }
            else
                {
                aShowKeystroke->AppendL(autoDLT);
                }
            }
        else
            {
            aShowKeystroke->AppendL(aSpelling.Mid(i, 1) );
            count = count + 1;
            }
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::AnalyseSpellingAddToneMarkL
// Analyse spelling after user press tone mark.
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::AnalyseSpellingAddToneMarkL(
                                               TInt aI,
                                               TInt aToneMarkCount,
                                               CDesCArrayFlat* aKeystroke,
                                               CDesCArrayFlat* aShowKeystroke )
    {
    TBuf<1> atuoDLT;
    TBuf<1> manualDLT;
    atuoDLT.Append(KAutoDLT);
    manualDLT.Append(KManualDLT);
    if ( CheckSpellingDLT( aKeystroke->MdcaPoint( aI - 1 ) ) || 
        ( KInvalidToneMark != ToneMark( aKeystroke->MdcaPoint( aI - 1 ) ) ) )
        {
        aShowKeystroke->AppendL( aKeystroke->MdcaPoint( aI ) ); 
        }
    else
        {
        if ( CheckSpellingAddToneMarkToPTIL( aToneMarkCount ) )
            {
            aShowKeystroke->AppendL( aKeystroke->MdcaPoint( aI - 1 ) ); 
            aShowKeystroke->AppendL( aKeystroke->MdcaPoint( aI ) );
            }
        else
            {
            if ( aShowKeystroke->Count() == 0 )
                {
                aShowKeystroke->AppendL( aKeystroke->MdcaPoint( aI - 1 ) ); 
                aShowKeystroke->AppendL( aKeystroke->MdcaPoint( aI ) );
                }
            else
                {
                if ( ( !CheckSpellingDLT(aShowKeystroke->MdcaPoint( 
                    aShowKeystroke->Count() - 1 ) ) ) && 
                    ( KInvalidToneMark == ToneMark( aShowKeystroke->
                        MdcaPoint( aShowKeystroke->Count() - 1 ) ) ) )
                    {
                    aShowKeystroke->AppendL( atuoDLT );
                    }
                aShowKeystroke->AppendL( aKeystroke->MdcaPoint( aI - 1 ) ); 
                aShowKeystroke->AppendL( aKeystroke->MdcaPoint( aI ) );
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::AnalyseSpellingAddKeyL
// Analyse spelling after user press key that is not tone mark.
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::AnalyseSpellingAddKeyL(
                                            TInt& aI,
                                            TInt aDelimterPos,
                                            CDesCArrayFlat* aKeystroke,
                                            CDesCArrayFlat* aShowKeystroke )
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    TBuf<1> atuoDLT;
    TBuf<1> manualDLT;
    atuoDLT.Append(KAutoDLT);
    manualDLT.Append(KManualDLT);
    if ( 0 == aI )
        {
        if ( aI + 1 < aKeystroke->Count() )
            {
            if ( ( KInvalidToneMark != ToneMark( aKeystroke->MdcaPoint( aI + 1 ) ) ) ||
                ( CheckSpellingDLT( aKeystroke->MdcaPoint( aI + 1 ) ) ) )
                {
                aShowKeystroke->AppendL( aKeystroke->MdcaPoint( aI ) );
                aShowKeystroke->AppendL( aKeystroke->MdcaPoint( aI + 1 ) );
                aI = aI + 1;
                }
            else
                {
                aShowKeystroke->AppendL( aKeystroke->MdcaPoint( aI ) );
                aShowKeystroke->AppendL( atuoDLT );
                }
            }
        else
            {
            aShowKeystroke->AppendL( aKeystroke->MdcaPoint( aI ) );
            }
        }
    else if ( aI + 1 == aKeystroke->Count() )
        {
        if ( ( 1 == aDelimterPos ) || ( -1 == aDelimterPos ) )
            {
            if ( ( !CheckSpellingDLT( aShowKeystroke->
                    MdcaPoint( aShowKeystroke->Count() - 1 ) ) &&
                ( KInvalidToneMark == ToneMark( aShowKeystroke->
                    MdcaPoint( aShowKeystroke->Count() - 1 ) ) ) ) )
                {
                aShowKeystroke->AppendL( atuoDLT );
                }
            aShowKeystroke->AppendL( aKeystroke->MdcaPoint( aI ) );
            }
        else
            {
            for ( TInt ii = 0; ii < aDelimterPos - 1; ++ii )
                {
                aShowKeystroke->Delete( aShowKeystroke->Count() - 1 );
                }
            if ( ( !CheckSpellingDLT( aShowKeystroke->
                    MdcaPoint( aShowKeystroke->Count() - 1 ) ) ) &&
                ( KInvalidToneMark == ToneMark( aShowKeystroke->MdcaPoint( 
                    aShowKeystroke->Count() - 1 ) ) ) )
                {
                aShowKeystroke->AppendL( atuoDLT );
                }
            aI = aI - aDelimterPos;
            }
        }
    else
        {
        if ( -1 == aDelimterPos )
            {
            if ( ( !CheckSpellingDLT( aShowKeystroke->MdcaPoint( 
                    aShowKeystroke->Count() - 1 ) ) ) &&
                ( KInvalidToneMark == ToneMark( aShowKeystroke->MdcaPoint( 
                    aShowKeystroke->Count() - 1 ) ) ) )
                {
                aShowKeystroke->AppendL( atuoDLT );
                }
            if ( 0 == getCurrentLeastDLTSpell().Length() )
                {
                aShowKeystroke->AppendL( aKeystroke->MdcaPoint( aI ) );
                if ( KInvalidToneMark != ToneMark( aKeystroke->MdcaPoint( aI + 1 ) ) ) 
                    {
                    aShowKeystroke->AppendL( aKeystroke->MdcaPoint( aI + 1 ) );
                    aI = aI + 1;                            
                    }
                }
            else
                {
                aI = aI - 1;
                }
            }
        else
            {
            for ( TInt ii = 0; ii < aDelimterPos - 1; ++ii )
                {
                aShowKeystroke->Delete( aShowKeystroke->Count() - 1 );
                }
            if ( ( !CheckSpellingDLT( aShowKeystroke->MdcaPoint( 
                    aShowKeystroke->Count() - 1 ) ) ) &&
                ( KInvalidToneMark == ToneMark( aShowKeystroke->MdcaPoint( 
                    aShowKeystroke->Count() - 1 ) ) ) )
                {
                aShowKeystroke->AppendL( atuoDLT );
                }
            aI = aI - aDelimterPos;
            }
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::AnalyseL
// Analyse key storke.
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::AnalyseL()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    //user pressed keystroke sequence
    CDesCArrayFlat* keystroke = editPane->KeystrokeArray();
    //stroke to be shown
    CDesCArrayFlat* showKeystroke = editPane->ShowKeystrokeArray();
    TInt start = 0;
    TInt toneMarkCount = KInvalidToneMark;
    TInt delimiterPosition = 0;
    showKeystroke->Reset();
    TBuf<1> atuoDLT;
    TBuf<1> manualDLT;
    atuoDLT.Append(KAutoDLT);
    manualDLT.Append(KManualDLT);
    TBuf<KMaxKeystrokeCount> keyTmp;

    if ( keystroke->Count() == 0 )
        {
        return;
        }

    //if the first keystroke is delimiter or ToneMark, analyse frome the next one
    if ( CheckSpellingDLT( keystroke->MdcaPoint( 0 ) ) || 
        ( KInvalidToneMark != ToneMark( keystroke->MdcaPoint( 0 ) ) ) )
        {
        showKeystroke->AppendL(keystroke->MdcaPoint( 0 ) );
        start = 1;
        }
    
    ClearPtiEngineKeystroke();
    
    for ( TInt i = start; i < keystroke->Count(); ++i )
        {
        toneMarkCount = ToneMark( keystroke->MdcaPoint( i ) );
        // current keystroke is delimiter
        if ( CheckSpellingDLT( keystroke->MdcaPoint( i ) ) )
            {
            showKeystroke->AppendL(keystroke->MdcaPoint( i ) );
            ClearPtiEngineKeystroke();
            }
        // current keystroke is tone mark
        else if ( KInvalidToneMark != toneMarkCount )
            {
            AnalyseSpellingAddToneMarkL( i, toneMarkCount, keystroke, showKeystroke );
            ClearPtiEngineKeystroke();
            }
        // current keystroke is A - Z
        else
            {
            delimiterPosition = CheckSpellingAddKeyToPTIL( keystroke->MdcaPoint( i ) );
            if ( 0 == delimiterPosition )
                {
                if ( ( 0 != i ) && 
                    ( 1 == this->getCurrentLeastDLTSpell().Length() ) )
                    {
                    if ( ( !CheckSpellingDLT( showKeystroke->
                        MdcaPoint( showKeystroke->Count() - 1 ) ) ) && 
                        ( KInvalidToneMark == ToneMark( showKeystroke->
                            MdcaPoint( showKeystroke->Count() - 1 ) ) ) )
                        {
                        showKeystroke->AppendL( atuoDLT );
                        }
                    }
                if ( i + 1 < keystroke->Count() ) 
                    {
                    if ( KInvalidToneMark == ToneMark( keystroke->MdcaPoint( i + 1 ) ) )
                        {
                        showKeystroke->AppendL( keystroke->MdcaPoint( i ) );
                        }
                    }
                else
                    {
                    showKeystroke->AppendL( keystroke->MdcaPoint( i ) );
                    }
                }
            else
                {
                AnalyseSpellingAddKeyL( i, delimiterPosition, keystroke, showKeystroke );
                ClearPtiEngineKeystroke();
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::HandleCommitL
// Handle CBA1 or select event.
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::HandleCommitL()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    TInt keyCount = editPane->KeystrokeArray()->Count();
    TInt showKeyCount = editPane->ShowKeystrokeArray()->Count();
    CDesCArrayFlat* phrase = editPane->PhraseArray();
    TInt cdtCount = uiContainer->CandidatePane()->CandidateArray()->Count();
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();

    if (EMiniQwertyEdit == iState)
        {
        if ( ( showKeyCount > 0 ) && (cdtCount > 0 ) )
            {
            UIContainer()->EditPaneWindow()->SetChangeState(ETrue);
            iOwner->ChangeState(ECandidate);
            return;
            }

        if ( keyCount > 0 )
            {
            iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
            return;
            }
        }

    if ( ( 0 == keyCount ) && (phrase->Count() <= 1 ) )
        {
        iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
        return;
        }

    if ( ( 0 == keyCount ) && (phrase->Count() > 1 ) )
        {
        TBuf<KMaxPhraseCreationCount> phraseCreated;
        for (TInt i = 0; ( (i < phrase->Count() ) && (i
                < KMaxPhraseCreationCount ) ); ++i )
            {
            phraseCreated.Append(phrase->MdcaPoint( i ) );
            AddPhraseToDB(phraseCreated);
            }
        fepMan->NewTextL(phraseCreated);
        fepMan->CommitInlineEditL();
        if ( !UIContainer()->EditPaneWindow()->GetPhraseCreationFlag() )
            {
            UIContainer()->EditPaneWindow()->SetPhraseCreationFlag(EFalse);
            iOwner->PtiEngine()->SetPredictiveChineseChar(phraseCreated);
#ifdef RD_INTELLIGENT_TEXT_INPUT
            UIContainer()->EditPaneWindow()->SetChangeState(ETrue);
            if (EPtiKeyboardQwerty4x10 == iOwner->FepMan()->KeyboardLayout() ||
                 EPtiKeyboardQwerty3x11 == iOwner->FepMan()->KeyboardLayout() )
                {
                iOwner->ChangeState(EPredictiveCandidate);
                }
#endif
            }
        else
            {
            UIContainer()->EditPaneWindow()->SetPhraseCreationFlag(EFalse);
            iOwner->FepMan()->TryCloseUiL();
            }
        return;
        }

    TPtrC text = UIContainer()->CandidatePane()->CurrentPhraseCandidate();
    if (text.Length() == 0)
        {
        return;
        }

    if ( !CommitInlineEEPL(text) )
        {
        DoActionAfterCommit();
        }
    else
        {
        if ( !CheckFirstGroupStroke() )
            {
            if ( iState == EEntry )
                {
                editPane->SetAutoChangeStateFlag(EAutoChangeStateFromInput);
                }
            else
                {
                editPane->SetAutoChangeStateFlag(EAutoChangeStateFromCandidate);
                }
            iOwner->ChangeState(EMiniQwertyEdit);
            return;
            }
        else
            {
            if ( iState == EEntry )
                {
                UIContainer()->EditPaneWindow()->SetChangeState( ETrue );
                iOwner->ChangeState( ECandidate );
                }
            }
        AnalyseL();
        RefreshUI();
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::HandleKeyBackspaceL
// Handle KeyBackspace event.
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::HandleKeyBackspaceL(
    TKeyPressLength aLength)
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    TInt index = editPane->GetCursorIndexOfKeystroke();
    CDesCArrayFlat* keystroke = editPane->KeystrokeArray();
    TInt phraseCount = editPane->PhraseArray()->Count();
    TBool firstGroup = CheckFirstGroupStroke();

    if ( ( 0 == index ) && (phraseCount != 0 ) )
        {
        RevertPhraseToKeystrokeL();
        }
    else
        if ( ( 0 == index ) && (keystroke->Count() > 0 ) )
            {
            iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
            }
        else
            {
            if ( !DeleteKeystrokeL() )
                {
                iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
                }
            }

    //if the keystroke is null and phrase is null then close input window.
    if ( ( 0 == phraseCount ) && (keystroke->Count() == 0 ))
        {
        iOwner->FepMan()->TryCloseUiL(); //no more keys, close the UI.
        if (aLength == ELongKeyPress)
            {
            iOwner->FepMan()->SetLongClearAfterCloseUI(ETrue);
            }
        return;
        }

    AnalyseL();
    //Auto change from EMiniQwertyEdit state to EEntry state. 
    if (EMiniQwertyEdit == iState)
        {
        if ( ( !firstGroup ) && CheckFirstGroupStroke() )
            {
            if ( EAutoChangeStateFromCandidate == editPane->GetAutoChangeStateFlag() )
                {
                RefreshUI();
                editPane->SetChangeState(ETrue);
                iOwner->ChangeState(ECandidate);
                }
            else if ( EAutoChangeStateFromInput == editPane->GetAutoChangeStateFlag() )
                {
                editPane->SetChangeState(ETrue);
                iOwner->ChangeState(EEntry);
                }
            else
                {
                RefreshUI();
                }
            return;
            }
        }

    //Auto change from EEntry state to EMiniQwertyEdit state.
    if (EEntry == iState)
        {
        if ( (firstGroup ) && ( !CheckFirstGroupStroke() ))
            {
            editPane->SetChangeState(EFalse);
            editPane->SetAutoChangeStateFlag(EAutoChangeStateFromInput);
            iOwner->ChangeState(EMiniQwertyEdit);
            return;
            }
        }

    RefreshUI();
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::HandleDefaultKeyL
// Handle default key press event.
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::HandleDefaultKeyL(TInt aKey)
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    TBool firstGroup = CheckFirstGroupStroke();

    if ( !AddKeystrokeL(aKey) )
        {
        iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
        return;
        }

    AnalyseL();

    //Auto change from EMiniQwertyEdit state to EEntry state. 
    if (EMiniQwertyEdit == iState)
        {
        if ( ( !firstGroup ) && CheckFirstGroupStroke() )
            {
            if ( EAutoChangeStateFromCandidate == editPane->GetAutoChangeStateFlag() )
                {
                RefreshUI();
                editPane->SetChangeState(ETrue);
                iOwner->ChangeState(ECandidate);
                }
            else if ( EAutoChangeStateFromInput == editPane->GetAutoChangeStateFlag() )
                {
                editPane->SetChangeState(ETrue);
                iOwner->ChangeState(EEntry);
                }
            else
                {
                RefreshUI();
                }
            return;
            }
        }

    //Auto change from EEntry state to EMiniQwertyEdit state.
    if (EEntry == iState)
        {
        if ( !CheckFirstGroupStroke() )
            {
            editPane->SetAutoChangeStateFlag(EAutoChangeStateFromInput);
            editPane->SetChangeState(ETrue);
            iOwner->ChangeState(EMiniQwertyEdit);
            return;
            }
        }

    RefreshUI();
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::ShowInfoOnEEP
// Show the showinfo on the EEP ctrl.
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::ShowInfoOnEEPPane()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* showKeystroke = editPane->ShowKeystrokeArray();
    CDesCArrayFlat* keystrokeArray = editPane->KeystrokeArray();
    CDesCArrayFlat* phrase = editPane->PhraseArray();
    TBuf<KMaxKeystrokeCount> showInfo;
    TBuf<KMaxKeystrokeCount> keyBuf;
    for (TInt i = 0; i < phrase->Count(); ++i)
        {
        showInfo.Append(phrase->MdcaPoint(i) );
        }

    for (TInt ii = 0; ii < showKeystroke->Count(); ++ii)
        {
        showInfo.Append(showKeystroke->MdcaPoint(ii) );
        }

    for (TInt j = 0; j < keystrokeArray->Count(); ++j)
        {
        keyBuf.Append(keystrokeArray->MdcaPoint(j) );
        }

    editPane->SetTextForZhuyin(showInfo, GetCursorPos(), phrase->Count(),
        keyBuf, editPane->GetCursorIndexOfKeystroke() );

    if (iState == ECandidate)
        {
        editPane->DisableCursor();
        }
    else
        if (iState == EEntry)
            {
            editPane->EnableCursor();
            editPane->DeactiveCursor();
            }
        else
            if (iState == EMiniQwertyEdit)
                {
                editPane->EnableCursor();
                editPane->ActiveCursor();
                }
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::ShowInfoOnEEP
// Show the showinfo on the EEP ctrl.
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::ShowInfoOnCandidatePane()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    CDesCArrayFlat* phrase = uiContainer->CandidatePane()->CandidateArray();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    if (KMaxPhraseCreationCount == editPane->PhraseArray()->Count() )
        {
        phrase->Reset();
        TRAP_IGNORE( phrase->AppendL( KMaxPhraseNote ) );
        uiContainer->CandidatePane()->SplitPhraseCandidatesIntoPages();
        uiContainer->CandidatePane()->SetCandidateBuffer();
        uiContainer->Enable(ETrue);
        uiContainer->ShowHorizontalScrollArrows(EFalse);
        uiContainer->ShowVerticalScrollArrows(EFalse);
        }

    TRAP_IGNORE( GetCandidateL() );
    if (phrase->Count() > 0)
        {
        // layout candidate and fill candidate buffer for first display page
        uiContainer->CandidatePane()->SplitPhraseCandidatesIntoPages();
        uiContainer->CandidatePane()->SetCandidateBuffer();
        uiContainer->Enable(ETrue);
        TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() && uiContainer->CandidatePane()->IsFirstPage());
        uiContainer->ShowVerticalScrollArrows(multiplePages);
        if (iState == EMiniQwertyEdit)
            {
            UIContainer()->FocusCandidatePane(EFalse);
            }
        else
            {
            UIContainer()->FocusCandidatePane(ETrue);
            UIContainer()->CandidatePane()->SelectFirstPhrase();
            }
        }
    else
        {
        uiContainer->CandidatePane()->SplitPhraseCandidatesIntoPages();
        uiContainer->CandidatePane()->SetCandidateBuffer();
        uiContainer->Enable(ETrue);
        uiContainer->ShowHorizontalScrollArrows(EFalse);
        uiContainer->ShowVerticalScrollArrows(EFalse);
        uiContainer->SetControlInVisible( EFalse );
        }
    }
//-------------------------------------------------------------------------------
// Purpose: to adjust the scancode to make pinyin works
// Pinyin symbol inheritantly accepts A-Z, any input out of this range will make it crash.
// Add this function to turn some alienated scancode to A-Z.
//
//---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::MapKey(TInt& aKey)
    {    
    if(iOwner && iOwner->PtiEngine())
        {
        iOwner->PtiEngine()->SetInputMode(EPtiEnginePinyinPhraseQwerty);
        TBuf<KMaxName> upperdata;
        iOwner->PtiEngine()->MappingDataForKey((TPtiKey)aKey, upperdata, EPtiCaseUpper);
        if(upperdata.Length() > 0)
            {
            aKey = upperdata[0];
            }
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::GetShowKeystroke
//Handle horizontal navigation.
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::GetShowKeystroke(TInt aKey,
    TDes& aKeystroke)
    {
    TInt key = aKey;
    MapKey(key);   
    TInt count = sizeof(StrokeMap )/sizeof(StrokeMap[0] );
    for (TInt i = 0; i < count; i++)
        {
        if (key == StrokeMap[i].iKeyCode)
            {
            aKeystroke.Append(StrokeMap[i].iValue);
            break;
            }
        }
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::HandleHorizontalNavigation
//Handle horizontal navigation.
// ---------------------------------------------------------------------------
//
TBool TAknFepInputMiniQwertyPinyinPhraseBase::HandleHorizontalNavigation(
    TInt aKey)
    {
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();
    TBool response = EFalse;

    // Do navigation...
    if (aKey == EStdKeyLeftArrow)
        {
        if (!candidatePane->SelectPrev())
            {
            candidatePane->SetCandidateBuffer();
            candidatePane->SelectLastPhrase();
            }
        response = ETrue;
        }
    else
        if (aKey == EStdKeyRightArrow)
            {
            if (!candidatePane->SelectNext())
                {
                candidatePane->SetCandidateBuffer();
                candidatePane->SelectFirstPhrase();
                }
            response = ETrue;
            }

    if (response)
        {
        UpdateIndicator();
        }

    return response;
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::UpdateIndicator
// Update Indicator.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::UpdateIndicator()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();

    if (candidatePane->IsFirstPage())
        {
        uiContainer->CandidatePane()->ShowUpScrollArrows(EFalse);
        }
    else
        {
        uiContainer->CandidatePane()->ShowUpScrollArrows(ETrue);
        }

    if (candidatePane->IsLastPage())
        {
        uiContainer->CandidatePane()->ShowDownScrollArrows(EFalse);
        }
    else
        {
        uiContainer->CandidatePane()->ShowDownScrollArrows(ETrue);
        }
#ifdef RD_INTELLIGENT_TEXT_INPUT
    if (iOwner->FepMan()->KeyboardLayout() == EPtiKeyboardQwerty4x10)
        {
        if (candidatePane->VisibleCandidateCount()==1)
            {
            uiContainer->CandidatePane()->ShowLeftScrollArrows(EFalse);
            uiContainer->CandidatePane()->ShowRightScrollArrows(EFalse);
            }
        else
            {
            uiContainer->CandidatePane()->ShowLeftScrollArrows(ETrue);
            uiContainer->CandidatePane()->ShowRightScrollArrows(ETrue);
            }
        }
    else
        {
#endif
        if (candidatePane->SelectedIndex() == 0
                && candidatePane->IsFirstPage() )
            {
            uiContainer->CandidatePane()->ShowLeftScrollArrows(EFalse);
            }
        else
            {
            uiContainer->CandidatePane()->ShowLeftScrollArrows(ETrue);
            }

        if (candidatePane->IsLastPage() && (candidatePane->SelectedIndex()
                == candidatePane->VisibleCandidateCount() - 1))
            {
            uiContainer->CandidatePane()->ShowRightScrollArrows(EFalse);
            }
        else
            {
            uiContainer->CandidatePane()->ShowRightScrollArrows(ETrue);
            }
        }
#ifdef RD_INTELLIGENT_TEXT_INPUT
    }
#endif    

// ---------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::DeleteMoreKeystrokeL
// Delete more keystroke than max leng.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::DeleteMoreKeystrokeL()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystroke = editPane->KeystrokeArray();
    CDesCArrayFlat* showKeystroke = editPane->ShowKeystrokeArray();
    TInt index = editPane->GetCursorIndexOfKeystroke();
    TBuf<1> autoDLT;
    autoDLT.Append( KAutoDLT );
    AnalyseL();
    // the count keystroke more than max length.
    TInt count = showKeystroke->Count() - KMaxKeystrokeCount; 
    
    for ( TInt i = 0; i < count; ++i )
        {
        if ( 0 != showKeystroke->MdcaPoint( showKeystroke->Count() - 1 ).Compare( autoDLT ) )
            {
            showKeystroke->Delete( showKeystroke->Count() - 1 );
            keystroke->Delete( keystroke->Count() - 1 );
            }
        }
    
    if ( index > keystroke->Count() )
        {
        editPane->SetCursorIndexOfKeystroke( keystroke->Count() );
        }    
    }
// ---------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::DeleteKeystrokeL
// Delete current keystroke.
// ---------------------------------------------------------
//
TBool TAknFepInputMiniQwertyPinyinPhraseBase::DeleteKeystrokeL()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystroke = editPane->KeystrokeArray();
    TInt index = editPane->GetCursorIndexOfKeystroke();
    TInt showKeyCount = editPane->ShowKeystrokeArray()->Count();
    
    if ( ( 0 == index ) || (keystroke->Count() == 0 ))
        {
        return EFalse;
        }
    if ( 1 == index )
        {
        editPane->Reset();
        }
    if (index >= keystroke->Count() )
        {
        keystroke->Delete(keystroke->Count() - 1);
        editPane->SetCursorIndexOfKeystroke(keystroke->Count() );
        }
    else
        {
        keystroke->Delete(index - 1);
        editPane->SetCursorIndexOfKeystroke(index - 1);
        }
    
    if ( showKeyCount >= KMaxKeystrokeCount - 1 )
        {
        DeleteMoreKeystrokeL();
        }
    
    return ETrue;
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::AddPhraseToDB
// Add phrase to DB.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::AddPhraseToDB(
    const TDesC& aPhraseAdd)
    {
    TPtiUserDictionaryEntry addUdbEntry(aPhraseAdd);
    //Add the phrase to the DB by PTI Engine
    iOwner->PtiEngine()->AddUserDictionaryEntry(addUdbEntry);
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::RefreshCandidate
// Refresh candidate
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::RefreshCandidate()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    uiContainer->CandidatePane()->SetCandidateBuffer();
    uiContainer->Enable(ETrue);

    TBool multiplePages = !(uiContainer->CandidatePane()->IsLastPage() && 
            uiContainer->CandidatePane()->IsFirstPage());
    uiContainer->ShowVerticalScrollArrows(multiplePages);

    uiContainer->CandidatePane()->SelectFirstPhrase();
    UpdateIndicator();
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::CheckKeyNeedRepeat
//  Check key whether need repeat.
// ---------------------------------------------------------------------------
//
TBool TAknFepInputMiniQwertyPinyinPhraseBase::CheckKeyNeedRepeat(
    TKeyPressLength aLength)
    {
    MAknFepUICtrlEditPane* editPane = UIContainer()->EditPaneWindow();
    if (editPane->IsChangeState() && aLength == ELongKeyPress)
        {
        return EFalse;
        }
    else
        {
        editPane->SetChangeState(EFalse);
        }
    return ETrue;
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::CommitInlineEEPL
// Commint text to EEP or editor
// ---------------------------------------------------------------------------
//
TBool TAknFepInputMiniQwertyPinyinPhraseBase::CommitInlineEEPL(
    const TDesC& aDes)
    {
    TInt charCount = aDes.Length();
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    MAknFepUICtrlEditPane* editpane = UIContainer()->EditPaneWindow();
    CDesCArrayFlat* showkeyStoke = editpane->ShowKeystrokeArray();
    CDesCArrayFlat* keyStoke = editpane->KeystrokeArray();
    CDesCArrayFlat* phraseArray = editpane->PhraseArray();
    CDesCArrayFlat* phraseShowKeyStroke = editpane->PhraseShowKeyStrokeArray();
    TBuf<KMaxKeystrokeCount> buf;
    TBuf<1> autoDLT;
    autoDLT.Append(KAutoDLT);

    if (phraseArray->Count() == 0)
        {
        editpane->SetPhraseCreationFlag(EFalse);
        }
    phraseArray->AppendL(aDes);

    TInt delCount = 0;
    for (TInt i = 0; i < showkeyStoke->Count() && charCount
            && keyStoke->Count() != 0; i++)
        {
        TPtrC showptr = showkeyStoke->MdcaPoint(i);
        TPtrC ptr = keyStoke->MdcaPoint( 0);
        if (CheckSpellingDLT(showptr) || (KInvalidToneMark
                != ToneMark(showptr) ))
            {
            charCount--;
            }
        if (showptr.Compare(autoDLT) != 0)
            {
            buf.Append(ptr);
            keyStoke->Delete( 0);
            }
        delCount++;
        }
    
	for (TInt j = 0; j < delCount; j++)
		{
		showkeyStoke->Delete( 0);
		}
	phraseShowKeyStroke->AppendL(buf);
	
    TInt phraseCount = phraseArray->Count();
    if (phraseCount > KMaxPhraseCreationCount)
        {
        phraseCount = KMaxPhraseCreationCount;
        }

    if (keyStoke->Count() == 0 || phraseCount == KMaxPhraseCreationCount)
        {
		//here,we pass pinyin-spelling to db together with phrase.
		TBuf<KMaxPhraseCreationCount> phraseCreated;
		phraseCreated.FillZ();
		TBuf<(1+KMaxSpellLength)*KMaxPhraseCreationCount> phraseCreatedWithPinYin;//(Zi+pinyin)* max_Zi
		phraseCreatedWithPinYin.FillZ();
		for (TInt ii = 0; ii < phraseCount; ++ii)
			{
			TPtrC ptrZi = phraseArray->MdcaPoint(ii);
			phraseCreatedWithPinYin.Append(ptrZi);
			phraseCreated.Append(ptrZi);
			
			TPtrC ptrPinYin = phraseShowKeyStroke->MdcaPoint(ii);//it mabe include KManualDLT,but no matter
			phraseCreatedWithPinYin.Append(ptrPinYin);
			
			TInt zeroTail = (1+KMaxSpellLength)-(ptrZi.Length()+ptrPinYin.Length());
			phraseCreatedWithPinYin.AppendFill(0,zeroTail);
			
			}
		fepMan->NewTextL(phraseCreated);
		fepMan->CommitInlineEditL();
		iOwner->PtiEngine()->SetPredictiveChineseChar(phraseCreated);
		AddPhraseToDB(phraseCreatedWithPinYin);
		return EFalse;
        }

    

    editpane->SetCursorIndexOfKeystroke( 0);
    editpane->DisableCursor();
    editpane->SetPhraseCreationFlag(ETrue);
    return ETrue;
    }
// ---------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::ClearPtiEngineKeystroke
// clear the ptiengine keystroke;
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::ClearPtiEngineKeystroke()
    {
    iOwner->PtiEngine()->ClearCurrentWord();
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::AddKeystrokeL
// Add keystroke.
// ---------------------------------------------------------
//
TBool TAknFepInputMiniQwertyPinyinPhraseBase::AddKeystrokeL(TInt aKey)
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystrokeArray = editPane->KeystrokeArray();
    TInt phraseCount = editPane->PhraseArray()->Count();
    TInt showKeystrokeCount = editPane->ShowKeystrokeArray()->Count();
    TInt index = editPane->GetCursorIndexOfKeystroke();
    TBuf<1> keystroke;
    
    if ( phraseCount + showKeystrokeCount >= KMaxKeystrokeCount )
        {
        return EFalse;
        }

    GetShowKeystroke(aKey, keystroke);
	//Add this condition to avoid crash in case keystroke is empty.
    if(keystroke.Length() == 0)
        {
        return EFalse;
        }
    if ( index >= keystrokeArray->Count() )
        {
        keystrokeArray->AppendL(keystroke);
        editPane->SetCursorIndexOfKeystroke(keystrokeArray->Count() );
        }
    else
        {
        keystrokeArray->InsertL(index, keystroke);
        editPane->SetCursorIndexOfKeystroke(index + 1);
        }
    
    if ( phraseCount + showKeystrokeCount >= KMaxKeystrokeCount - 1 )
        {
        AnalyseL();
        if ( phraseCount + editPane->ShowKeystrokeArray()->Count() <= KMaxKeystrokeCount )
            {
            return ETrue;
            }
        else
            {
            keystrokeArray->Delete( index );
            editPane->SetCursorIndexOfKeystroke( index );
            AnalyseL();
            return EFalse;
            }
        }
        

    return ETrue;
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::AddToneMarkL
// Add tone mark.
// ---------------------------------------------------------
//
TBool TAknFepInputMiniQwertyPinyinPhraseBase::AddToneMarkL()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystrokeArray = editPane->KeystrokeArray();
    TInt phraseCount = editPane->PhraseArray()->Count();
    TInt showKeystrokeCount = editPane->ShowKeystrokeArray()->Count();
    TInt index = editPane->GetCursorIndexOfKeystroke();
    TInt i = 0;
    const TInt MaxToneMark = 4;
    if ( phraseCount + showKeystrokeCount >= KMaxKeystrokeCount )
        {
        if ( 0 == index )
            {
            return EFalse;
            }
        else
            {
            if ( KInvalidToneMark == ToneMark( keystrokeArray->MdcaPoint( index - 1 ) ) )
                {
                return EFalse;
                }
            }
        }

    TBuf<1> toneMark;

    if ( 0 == index )
        {
        toneMark.Append(PinyinToneMap[0].iValue);
        keystrokeArray->InsertL( 0, toneMark);
        editPane->SetCursorIndexOfKeystroke( 1);
        return ETrue;
        }
    
    TInt num = sizeof(PinyinToneMap )/( sizeof(PinyinToneMap[0]));
    for ( i = 0; i < num; ++i )
        {
        if (PinyinToneMap[i].iValue == keystrokeArray->MdcaPoint( index - 1 )[0])
            {
            break;
            }
        }

    if ( i > MaxToneMark )
        {
        toneMark.Append(PinyinToneMap[0].iValue);
        keystrokeArray->InsertL(index, toneMark);
        editPane->SetCursorIndexOfKeystroke( index + 1 );
        }
    else
        {
        if ( MaxToneMark == i )
            {
            toneMark.Append(PinyinToneMap[0].iValue);
            }
        else
            {
            toneMark.Append(PinyinToneMap[ i + 1 ].iValue);
            }
        keystrokeArray->Delete(index - 1);
        keystrokeArray->InsertL(index - 1, toneMark);
        }

    if ( phraseCount + showKeystrokeCount >= KMaxKeystrokeCount - 1 )
        {
        AnalyseL();
        if ( phraseCount + editPane->ShowKeystrokeArray()->Count() > KMaxKeystrokeCount )
            {
            if ( i > MaxToneMark )
                {
                keystrokeArray->Delete( index );
                editPane->SetCursorIndexOfKeystroke( index );
                }
            else
                {
                toneMark.Zero();
                toneMark.Append( PinyinToneMap[i].iValue );
                keystrokeArray->Delete( index - 1 );
                keystrokeArray->InsertL( index - 1, toneMark );
                }
            AnalyseL();
            return EFalse;
            }
        }
        
    return ETrue;
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::GetCandidateL
// Get the candidate info.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::GetCandidateL()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* phraseCandidates = 
        uiContainer->CandidatePane()->CandidateArray();
    CDesCArrayFlat* keystroke = editPane->KeystrokeArray();
    CDesCArrayFlat* showKeystroke = editPane->ShowKeystrokeArray();
    TInt phraseCount = editPane->PhraseArray()->Count();
    phraseCandidates->Reset();
    TBuf<KMaxKeystrokeCount> buf;
    TBuf<1> delimiter;
    delimiter.Append( KManualDLT );
    TInt keyCode;
    TInt count = KInvalidToneMark;
    ClearPtiEngineKeystroke();
    
    if ( showKeystroke->Count() == 0 )
        {
        return;
        }

    if ( CheckAllGroupStroke() && ( phraseCount == 0 ) )
        {
        getCurrentLeastDLTSpell();
        ptiengine->GetChinesePhraseCandidatesL( *phraseCandidates ); 
        return;
        }
    
    if ( CheckFirstGroupStroke() )
        {
        ClearPtiEngineKeystroke();
        for ( TInt i = 0; i < showKeystroke->Count(); ++i )
            {
            count = ToneMark( keystroke->MdcaPoint( i ) );
            if ( CheckSpellingDLT( showKeystroke->MdcaPoint( i ) ) )
                {
                break;
                }
            else if ( KInvalidToneMark != count )
                {
                for ( TInt j = 0; j < count; ++j )
                    {
                    ptiengine->IncrementToneMark( ETrue );
                    }
                break;
                }
            else
                {
                GetKeystrokeCode( keyCode, keystroke->MdcaPoint( i ) );
                ptiengine->AppendKeyPress((TPtiKey)keyCode);
                }
            }
        }
    else
        {
        return;
        }
    getCurrentLeastDLTSpell();
    ptiengine->GetChinesePhraseCandidatesL( *phraseCandidates );
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::GetKeystrokeCode
// get Code of the key stroke. 
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::GetKeystrokeCode(TInt& aKey,
    const TDesC& aKeystroke)
    {
    TInt count = sizeof(StrokeMap )/sizeof(StrokeMap[0] );
    for (TInt i = 0; i < count; i++)
        {
        TBuf<1> buf;
        buf.Append(StrokeMap[i].iValue);
        if (0 == aKeystroke.Compare(buf))
            {
            aKey = StrokeMap[i].iKeyCode;
            break;
            }
        }
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::RevertPhraseToKeystrokeL
// Revert the phrase to keystroke.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::RevertPhraseToKeystrokeL()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystrokeArray = editPane->KeystrokeArray();
    CDesCArrayFlat* shownKeystrokeArray = editPane->ShowKeystrokeArray();
    CDesCArrayFlat* phrase = editPane->PhraseArray();
    CDesCArrayFlat* phraseStroke = editPane->PhraseShowKeyStrokeArray();
    TBuf<KMaxKeystrokeCount> buf;
    if ( (phrase->Count() == 0 ) || (phraseStroke->Count() == 0 ))
        {
        return;
        }

    buf.Append(phraseStroke->MdcaPoint(phraseStroke->Count() - 1) );
    for (TInt i = buf.Length() - 1; i >= 0; --i)
        {
        keystrokeArray->InsertL( 0, buf.Mid(i, 1) );
        }
    phrase->Delete(phrase->Count() - 1);
    phraseStroke->Delete(phraseStroke->Count() - 1);
    AnalyseL();
    TInt showCount = editPane->ShowKeystrokeArray()->Count();
    TInt leng = phrase->Count() + showCount;
    TInt deleteCount = 0;
    if (leng > KMaxKeystrokeCount)
        {
        for (TInt i = 0; i < leng - KMaxKeystrokeCount; ++i)
            {
            if (KAutoDLT != shownKeystrokeArray->MdcaPoint( shownKeystrokeArray->Count() - 1 )[0])
                {
                deleteCount = deleteCount + 1;
                }
            shownKeystrokeArray->Delete(shownKeystrokeArray->Count() - 1);
            }
        if (KAutoDLT == shownKeystrokeArray->MdcaPoint( shownKeystrokeArray->Count() - 1 )[0])
            {
            shownKeystrokeArray->Delete(shownKeystrokeArray->Count() - 1);
            }
        }
    for (TInt i = 0; i < deleteCount; ++i)
        {
        keystrokeArray->Delete(keystrokeArray->Count() - 1);
        }

    editPane->SetCursorIndexOfKeystroke(buf.Length() );
    keystrokeArray->Compress();
    phrase->Compress();
    phraseStroke->Compress();

    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::SetWarningColor
// Set warning color.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::SetWarningColor(
                                           TInt aCount,
                                           TInt aIndex,
                                           TInt aI,
                                           TInt aPhraseCount,
                                           TInt aToneMark,
                                           TBool aValid,
                                           CDesCArrayFlat* aShowKeystroke
                                           )
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlEditPane* editPane = UIContainer()->EditPaneWindow();
    if ( ( !aValid ) || CheckSpellingDLT(aShowKeystroke->MdcaPoint(aI
            - 1) ) || (KInvalidToneMark
            != ToneMark( aShowKeystroke->MdcaPoint( aI - 1 ) ) ))
        {
        editPane->SetHighlight(aPhraseCount + aIndex, aPhraseCount
                + aIndex + aCount - 1 );
        }
    else
        {
        TPtrC ptr1 = getCurrentLeastDLTSpell();
        for (TInt j = 0; j < aToneMark; ++j)
            {
            ptiengine->IncrementToneMark(ETrue);
            TPtrC ptr0 = getCurrentLeastDLTSpell();
            if (ptr0.Length() > 0)
                {
                if ( 0 == ptr0.Mid( ptr0.Length() - 1, 1 ).
                Compare(aShowKeystroke->MdcaPoint( aI ) ) )
                    {
                    break;
                    }
                }
            }
        TPtrC ptr2 = getCurrentLeastDLTSpell();
        if ( ( ( 1 + ptr1.Length() ) != ptr2.Length() ) || ( 0
                != ptr2.Mid( ptr2.Length() - 1, 1 ).
                CompareC( aShowKeystroke->MdcaPoint( aI ) ) ))
            {
            editPane->SetHighlight( aPhraseCount + aIndex, aPhraseCount
                    + aIndex + aCount - 1);
            }
        }
    ClearPtiEngineKeystroke();
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::SetWarningColor
// Set warning color.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::SetWarningColor()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlEditPane* editPane = UIContainer()->EditPaneWindow();
    CDesCArrayFlat* showKeystroke = editPane->ShowKeystrokeArray();
    TInt phraseCount = editPane->PhraseArray()->Count();
    TBool valid = ETrue;
    TInt index = 0;
    TInt count = 0;
    TInt stringBeforeLength = 0;
    TInt stringAfterLength = 0;
    TInt start = 0;
    TInt keyCode = 0;
    TInt toneMark = KInvalidToneMark;

    if (showKeystroke->Count() == 0)
        {
        return;
        }

    if (CheckAllGroupStroke() )
        {
        return;
        }
    ClearPtiEngineKeystroke();
    if (CheckSpellingDLT(showKeystroke->MdcaPoint( 0) ) || (KInvalidToneMark
            != ToneMark(showKeystroke->MdcaPoint( 0) ) ))
        {
        start = 1;
        editPane->SetHighlight(phraseCount, phraseCount);
        index = 1;
        }

    for ( TInt i = start; i < showKeystroke->Count(); ++i )
        {
        count = count + 1;
        // tone mark
        toneMark = ToneMark(showKeystroke->MdcaPoint(i) );
        if ( KInvalidToneMark != toneMark )
            {
            SetWarningColor( count, 
                             index, 
                             i, 
                             phraseCount, 
                             toneMark, 
                             valid, 
                             showKeystroke );
            index = i + 1;
            count = 0;
            valid = ETrue;
            }
        else
            if ( CheckSpellingDLT(showKeystroke->MdcaPoint(i) ) )
                {
                if ( ( !valid )
                        || CheckSpellingDLT(showKeystroke->MdcaPoint(i - 1) )
                        || (KInvalidToneMark
                                != ToneMark(showKeystroke->MdcaPoint(i - 1) ) ))
                    {
                    editPane->SetHighlight(phraseCount + index, phraseCount
                            + index + count - 1);
                    }
                ClearPtiEngineKeystroke();
                index = i + 1;
                count = 0;
                valid = ETrue;
                }
            else
                {
                if ( valid )
                    {
                    GetKeystrokeCode(keyCode, showKeystroke->MdcaPoint(i) );
                    stringBeforeLength = getCurrentLeastDLTSpell().Length();
                    ptiengine->AppendKeyPress((TPtiKey)keyCode).Length();
                    stringAfterLength = getCurrentLeastDLTSpell().Length();
                    if ( stringBeforeLength == stringAfterLength )//the keystroke is invalid
                        {
                        ClearPtiEngineKeystroke();
                        valid = EFalse;
                        }
                    }
                }

        if ( ( showKeystroke->Count() == i + 1 ) && ( !valid ) )
            {
            editPane->SetHighlight(
                                phraseCount + index, 
                                phraseCount + index + count - 1 );
            ClearPtiEngineKeystroke();
            }
        }
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::SetWarningColor
// Set warning color.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::ChangeCbaL()
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan( );
    MAknFepUICtrlEditPane* editPane = UIContainer()->EditPaneWindow( );
    TInt candidateCount = UIContainer()->CandidatePane()->CandidateArray()->Count( );
    TInt keyStrokeCount = editPane->KeystrokeArray()->Count( );
    TInt phraseCount = editPane->PhraseArray()->Count( );
    TInt currentCBAResId = editPane->GetCurrentCBAResID( );
    if ( (candidateCount == 0 && keyStrokeCount != 0 ) || (phraseCount == 1
        && keyStrokeCount == 0 ) )
        {
        if ( R_AKNFEP_SOFTKEYS_EMPTY_CANCEL_EMPTY != currentCBAResId )
            {
            fepMan->UpdateCbaL( R_AKNFEP_SOFTKEYS_EMPTY_CANCEL_EMPTY );
            editPane->SetCurrentCBAResID( R_AKNFEP_SOFTKEYS_EMPTY_CANCEL_EMPTY );
            }

        }
    else if ( keyStrokeCount == 0 )
        {
        if ( R_AKNFEP_SOFTKEYS_DONE_CANCEL_DONE != currentCBAResId )
            {
            fepMan->UpdateCbaL( R_AKNFEP_SOFTKEYS_DONE_CANCEL_DONE );
            editPane->SetCurrentCBAResID( R_AKNFEP_SOFTKEYS_DONE_CANCEL_DONE );
            }
        }
    else if ( R_AKNFEP_SOFTKEYS_OK_SELECT_CANCEL != currentCBAResId )
        {
        fepMan->UpdateCbaL( R_AKNFEP_SOFTKEYS_OK_SELECT_CANCEL );
        editPane->SetCurrentCBAResID( R_AKNFEP_SOFTKEYS_OK_SELECT_CANCEL );
        }
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::MoveCursorLeft
// Move the cursor to Left.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::MoveCursorLeft()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* showKeyStoke = editPane->ShowKeystrokeArray();
    TInt showKeyStokeCount = editPane->ShowKeystrokeArray()->Count();
    TInt indexOfKeystroke = editPane->GetCursorIndexOfKeystroke();
    TInt cursorPos = editPane->GetCursor() - editPane->PhraseArray()->Count();

    TBuf<1> autoDLT;
    autoDLT.Append(KAutoDLT);
    const TInt offset = 2;
    if (cursorPos <= 0)
        {
        cursorPos = 0;
        }
    else
        if (cursorPos >= showKeyStokeCount)
            {
            cursorPos = showKeyStokeCount;
            }

    if (cursorPos == 0)
        {
        editPane->SetCursorIndexOfKeystroke(editPane->KeystrokeArray()->Count() );
        ShowInfoOnEEPPane();
        SetWarningColor();
        }
    else
        if (cursorPos == 1)
            {
            editPane->SetCursorIndexOfKeystroke( 0);
            ShowInfoOnEEPPane();
            SetWarningColor();
            }
        else
            {
            if (showKeyStoke->MdcaPoint( cursorPos - 2 ).Compare(autoDLT) == 0)
                {
                editPane->MoveCursorLeft(offset);
                editPane->SetCursorIndexOfKeystroke(indexOfKeystroke - 1);
                }
            else
                {
                editPane->MoveCursorLeft();
                editPane->SetCursorIndexOfKeystroke(indexOfKeystroke - 1);
                }
            }
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::MoveCursorRight
// Move the cursor to Right.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::MoveCursorRight()
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* showKeyStoke = editPane->ShowKeystrokeArray();
    TInt showKeyStokeCount = editPane->ShowKeystrokeArray()->Count();
    TInt indexOfKeystroke = editPane->GetCursorIndexOfKeystroke();
    TInt cursorPos = editPane->GetCursor() - editPane->PhraseArray()->Count();
    TBuf<1> autoDLT;
    autoDLT.Append(KAutoDLT);
    const TInt offset = 2;

    if (cursorPos <= 0)
        {
        cursorPos = 0;
        }
    else
        if (cursorPos >= showKeyStokeCount)
            {
            cursorPos = showKeyStokeCount;
            }

    if (cursorPos == showKeyStokeCount)
        {
        editPane->SetCursorIndexOfKeystroke( 0);
        ShowInfoOnEEPPane();
        SetWarningColor();
        }
    else
        if (cursorPos == showKeyStokeCount - 1)
            {
            editPane->MoveCursorRight();
            editPane->SetCursorIndexOfKeystroke(editPane->KeystrokeArray()->Count() );
            ShowInfoOnEEPPane();
            SetWarningColor();
            }
        else
            {
            if (showKeyStoke->MdcaPoint( cursorPos ).Compare(autoDLT) == 0)
                {
                editPane->MoveCursorRight(offset);
                editPane->SetCursorIndexOfKeystroke(indexOfKeystroke + 1);
                }
            else
                {
                editPane->MoveCursorRight();
                editPane->SetCursorIndexOfKeystroke(indexOfKeystroke + 1);
                }
            }
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::CheckSpecialKey
// Check the special keystroke( i,u,v ).
// ---------------------------------------------------------
//
TBool TAknFepInputMiniQwertyPinyinPhraseBase::CheckSpecialKey(const TInt aKey)
    {
    if ( (aKey == EPtiKeyQwertyI ) || (aKey == EPtiKeyQwertyU ) || (aKey
            == EPtiKeyQwertyV ))
        {
        return ETrue;
        }
    return EFalse;
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::CheckSpellingDLT
// Check the delimiter of spelling.
// ---------------------------------------------------------
//
TBool TAknFepInputMiniQwertyPinyinPhraseBase::CheckSpellingDLT(
    const TDesC& aKeystroke)
    {
    if ( aKeystroke.Length() == 0 )
        {
        return EFalse;
        }
    
    if ( ( KAutoDLT == aKeystroke[0] ) || ( KManualDLT == aKeystroke[0] ) )
        {
        return ETrue;
        }
    
    return EFalse;
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::ToneMark
// Get pinyin tone mark.
// ---------------------------------------------------------
//
TInt TAknFepInputMiniQwertyPinyinPhraseBase::ToneMark(const TDesC& aKeystroke)
    {
    TInt numOfToneMark = sizeof (PinyinToneMap)/sizeof(PinyinToneMap[0]);
    for (TInt i = 0; i < numOfToneMark; i++)
        {
        if (PinyinToneMap[i].iValue == aKeystroke[0])
            return PinyinToneMap[i].iInputTone;
        }
    return KInvalidToneMark;
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::ToneMark
// Get pinyin tone mark.
// ---------------------------------------------------------
//
TBool TAknFepInputMiniQwertyPinyinPhraseBase::CheckSpellingAddToneMarkToPTIL(
                                                        const TInt aToneMark )
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    TBuf<1> toneMark;
    TBuf<1> delimiter;
    
    toneMark.Append( PinyinToneMap[aToneMark - 1].iValue );
    delimiter.Append( KManualDLT );
    
    for ( TInt i = 0; i < aToneMark; ++i )
        {
        ptiengine->IncrementToneMark(ETrue);
        TPtrC ptr = getCurrentLeastDLTSpell();
        if ( ( 0 == ptr.Mid( ptr.Length() - 1, 1 ).Compare( toneMark ) ) &&
            ( KErrNotFound == ptr.Find( delimiter ) )
            )
            {
            return ETrue;
            }
        }

    
    return EFalse;
    }
TPtrC TAknFepInputMiniQwertyPinyinPhraseBase::getCurrentLeastDLTSpell()
	{
	CPtiEngine* ptiengine = iOwner->PtiEngine();
	CDesC16ArrayFlat* allSpell = new CDesC16ArrayFlat(1);
	TPtrC  currentSpell;
	TBuf<1> delimiter;
	TBuf<KMaxKeystrokeCount> preDltPosizeArr;
	TBuf<KMaxKeystrokeCount> curDltPosizeArr;
	preDltPosizeArr.FillZ(KMaxKeystrokeCount);
	preDltPosizeArr.SetLength(KMaxKeystrokeCount);
	curDltPosizeArr.FillZ(KMaxKeystrokeCount);
	curDltPosizeArr.SetLength(KMaxKeystrokeCount);
	
	TInt index = 1;
	delimiter.Append( KManualDLT );
	TRAP_IGNORE( ptiengine->GetPhoneticSpellingsL(*allSpell));
	TInt allCount = ptiengine->PhoneticSpellingCount();
	
	
	for( TInt i = 0; i < allCount; i++)
		{
		
		currentSpell.Set(allSpell->MdcaPoint(i));
		TInt dltIndex = 0;
		curDltPosizeArr.FillZ(KMaxKeystrokeCount);
		curDltPosizeArr.SetLength(KMaxKeystrokeCount);
		TInt preIndex = 0;
		for (TInt j = 0; j < currentSpell.Length(); j++)
			{
		
			if (currentSpell.Mid(j, 1).Compare(delimiter) == 0 )
				{	
				curDltPosizeArr[dltIndex] = j - preIndex - 1;
				preIndex = j;
				dltIndex++;
				}
			else if ((j+1) == currentSpell.Length())
				{
				curDltPosizeArr[dltIndex] = j - preIndex;
				}
			
			}
		
		for (TInt k = 0; k < KMaxKeystrokeCount; k++)
			{
			
			if (preDltPosizeArr[k] < curDltPosizeArr[k] )
				{
				preDltPosizeArr.Copy(curDltPosizeArr);
				index = i + 1;
				break;
				}
			else if (preDltPosizeArr[k] > curDltPosizeArr[k] )
				{
				break;
				}
			}
			
		}
	
	if (allSpell)
		{
		delete allSpell;
		}
	ptiengine->SelectPhoneticSpelling( index );
	return ptiengine->GetPhoneticSpelling(index);
	
	}
// ---------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::CheckSpellingAddKeyToPTIL
// Check the spelling after add key to the PTI.
// ---------------------------------------------------------
//
TInt TAknFepInputMiniQwertyPinyinPhraseBase::CheckSpellingAddKeyToPTIL(
                                                     const TDesC& aKeystroke )
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    
    TBuf<1> delimiter;
    TInt keyCode = 0;
    TInt stringBeforeLength = 0;
    TInt stringAfterLength = 0;
    TInt delimiterIndex = 0;
    delimiter.Append( KManualDLT );
    
    GetKeystrokeCode( keyCode, aKeystroke );
    //get the string before append this keystroke
    TPtrC ptr = getCurrentLeastDLTSpell();
    stringBeforeLength = ptr.Length();
    //get the string after append this keystroke
    ptiengine->AppendKeyPress((TPtiKey)keyCode);
    TPtrC ptr1 = getCurrentLeastDLTSpell();
    stringAfterLength = ptr1.Length();
    
    if ( stringBeforeLength != stringAfterLength )
        {
        TPtrC ptr = getCurrentLeastDLTSpell();
        if ( KErrNotFound == ptr.Find( delimiter ) )
            {
            return 0;
            }
        else
            {
            delimiterIndex = ptr.Length() - ptr.Find( delimiter ) - 1;
            return delimiterIndex;
            }
        }
    else
        {
        return -1;
        }
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::HandleKeyFromCandidateL
// Handle key from candidate.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::HandleKeyFromCandidateL(
    TInt aKey, TKeyPressLength /*aLength*/)
    {
    TInt index = 0;
#ifdef RD_INTELLIGENT_TEXT_INPUT
    // it may be one of the 'valid' numbers..
    TPtiKeyboardType keyboardtype = iOwner->FepMan()->KeyboardLayout();
    if (EPtiKeyboardQwerty4x10 == keyboardtype || EPtiKeyboardQwerty3x11
            == keyboardtype || EPtiKeyboardHalfQwerty == keyboardtype
            || EPtiKeyboardCustomQwerty == keyboardtype)
        {
        MPtiLanguage *lang= NULL;
        RArray <TPtiNumericKeyBinding> keybinding;
        TBool validnumkey = EFalse;
        CPtiEngine* ptiengine = iOwner->PtiEngine();
        
        if( ptiengine != NULL )
            {
            lang = ptiengine->CurrentLanguage();
            }
        if (ptiengine && lang)
            {
            TRAP_IGNORE(ptiengine->GetNumericModeKeysForQwertyL(lang->LanguageCode(), keybinding, keyboardtype));
            TInt numericKeysCount = keybinding.Count();
            while (numericKeysCount--)
                {
                TPtiNumericKeyBinding numKeyBind =
                        keybinding[numericKeysCount];
                if ( (numKeyBind.iChar >= 0x31 && numKeyBind.iChar <= 0x36)
                        &&(aKey == numKeyBind.iKey))
                    {
                    index = numKeyBind.iChar - EPtiKey0 -1;
                    validnumkey = ETrue;
                    break;
                    }
                }
            }
        keybinding.Reset();
        keybinding.Close();
        if ( !validnumkey)
            {
            iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
            return;
            }
        }
    else
        {
#endif
        index = MapKeyToIndex(aKey);
#ifdef RD_INTELLIGENT_TEXT_INPUT
        }
#endif
    MAknFepUICtrlCandidatePane* candidatePane = UIContainer()->CandidatePane();

    if (candidatePane->SelectIndex(index) || aKey == EStdKeyDevice3 || aKey
            == EStdKeyEnter)
        {
        TPtrC text = candidatePane->CurrentPhraseCandidate();
        if (text.Length() )
            {
            if ( !CommitInlineEEPL(text) )
                {
                DoActionAfterCommit();
                }
            else
                {
                if (CheckFirstGroupStroke() )
                    {
                    RefreshUI(EFalse);
                    UIContainer()->CandidatePane()->SelectFirstPhrase();
                    }
                else
                    {
                    UIContainer()->EditPaneWindow()->SetAutoChangeStateFlag(EAutoChangeStateFromCandidate);
                    iOwner->ChangeState(EMiniQwertyEdit);
                    }
                }
            }
        }
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::CheckAllGroupStroke
// check the all group stroke of keystroke.
// ---------------------------------------------------------
//
TBool TAknFepInputMiniQwertyPinyinPhraseBase::CheckAllGroupStroke()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlEditPane* editPane = uiContainer->EditPaneWindow();
    CDesCArrayFlat* keystroke = editPane->KeystrokeArray();
    TInt phraseCount = editPane->PhraseArray()->Count();
    TInt keyCode = 0;
    TInt stringBeforeLength = 0;
    TInt stringAfterLength = 0;
    TInt toneMarkCount = KInvalidToneMark;

    ClearPtiEngineKeystroke();

    //If there is phrase on the entry pane then the all keystroke is invalid.
    if ( phraseCount > 0 )
        {
        return EFalse;
        }

    //If the first keystroke is delimiter or tone mark 
    //then all keystroke is invalid.
    if ( ( keystroke->Count() == 0 )
            || (CheckSpellingDLT(keystroke->MdcaPoint( 0 ) ) )
            || (KInvalidToneMark != ToneMark(keystroke->MdcaPoint( 0 ) ) ) )
        {
        return EFalse;
        }

    for (TInt i = 0; i < keystroke->Count(); ++i )
        {
        toneMarkCount = ToneMark(keystroke->MdcaPoint( i ) );
        if ( KInvalidToneMark != toneMarkCount )
            {
            if ( (CheckSpellingDLT(keystroke->MdcaPoint( i - 1 ) ) )
                    || (KInvalidToneMark != ToneMark(keystroke->MdcaPoint( i - 1 ) ) ) )
                {
                return EFalse;
                }
            for (TInt j =0; j < toneMarkCount; ++j)
                {
                ptiengine->IncrementToneMark(ETrue);
                TPtrC ptr1 = getCurrentLeastDLTSpell();
                if (ptr1.Length() > 0)
                    {
                    if ( 0 == ptr1.Mid( ptr1.Length() - 1 ).Compare(keystroke->MdcaPoint( i ) ) )
                        {
                        break;
                        }
                    }
                }
            TPtrC ptr = getCurrentLeastDLTSpell();
            if ( 0 != ptr.Mid( ptr.Length() - 1, 1 ).Compare(keystroke->MdcaPoint( i ) ) )
                {
                return EFalse;
                }
            }
        else
            {
            if ( (CheckSpellingDLT(keystroke->MdcaPoint( i ) ) )
                    && (KInvalidToneMark != ToneMark(keystroke->MdcaPoint( i - 1 ) ) ) )
                {
                return EFalse;
                }
            GetKeystrokeCode(keyCode, keystroke->MdcaPoint(i) );
            stringBeforeLength = getCurrentLeastDLTSpell().Length();
            ptiengine->AppendKeyPress((TPtiKey)keyCode).Length();
            stringAfterLength = getCurrentLeastDLTSpell().Length();
            if ( stringBeforeLength == stringAfterLength )
                {
                return EFalse;
                }
            }
        }

    return ETrue;
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::DoActionAfterCommit
// Do action afer commit, change state to predictive or not.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::DoActionAfterCommit()
    {
    TRAP_IGNORE( DoActionAfterCommitL() );
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::DoActionAfterCommitL
// Do action afer commit, change state to predictive or not.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::DoActionAfterCommitL()
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    if (fepMan->IsFlagSet(CAknFepManager::EFlagEditorFull) )
        {
        UIContainer()->EditPaneWindow()->SetPhraseCreationFlag(EFalse);
        fepMan->ClearFlag(CAknFepManager::EFlagEditorFull);
        UIContainer()->EditPaneWindow()->SetPhraseCreationFlag(EFalse);
        if (UIContainer()->EditPaneWindow()->KeystrokeArray()->Count() != 0)
            {
            UIContainer()->EditPaneWindow()->ResetAllArray();
            UIContainer()->EditPaneWindow()->SetPhraseCreationFlag(EFalse);
            CAknWarningNote* errnote = new( ELeave ) CAknWarningNote();
            errnote->SetTone(CAknNoteDialog::EWarningTone);
            errnote->ExecuteLD(KMaxPhraseNote);
            }
        iOwner->FepMan()->TryCloseUiL();
        }
    else
        {
        if (UIContainer()->EditPaneWindow()->KeystrokeArray()->Count() != 0)
            {
            UIContainer()->EditPaneWindow()->ResetAllArray();
            UIContainer()->EditPaneWindow()->SetPhraseCreationFlag(EFalse);
            CAknWarningNote* errnote = new( ELeave ) CAknWarningNote();
            errnote->SetTone(CAknNoteDialog::EWarningTone);
            errnote->ExecuteLD(KMaxPhraseNote);
            }
        else
            {
            UIContainer()->EditPaneWindow()->ResetAllArray();
            if ( !UIContainer()->EditPaneWindow()->GetPhraseCreationFlag() )
                {
                UIContainer()->EditPaneWindow()->SetPhraseCreationFlag(EFalse);
                UIContainer()->EditPaneWindow()->SetChangeState(ETrue);
                iOwner->ChangeState(EPredictiveCandidate);
                }
            else
                {
                UIContainer()->EditPaneWindow()->SetPhraseCreationFlag(EFalse);
                iOwner->FepMan()->TryCloseUiL();
                }
            }
        }
    }
// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertyPinyinPhraseBase::HandleCommandL( TInt aCommandId )
    {
    switch ( aCommandId )
        {
        // Handle the event frome command.
        case EAknSoftkeySelect:
            //case (TUint16)EAknSoftkeySelect: //the Selected in soft CBA
            HandleCommitL( );
            break;
        default:
            TAknFepInputStateChineseBase::HandleCommandL( aCommandId );
            break;
        }
    }
// End of file
