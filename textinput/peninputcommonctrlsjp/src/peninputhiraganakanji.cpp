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
* Description:  peninputkanakanji implementation
*
*/


#include <PtiEngine.h>
#include <peninputlayout.h>
#include <PtiCompositionDataIF.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <peninputcommonlayoutglobalenum.h>  // EPeninputLayoutEventEnter
#include <aknfeppeninputenums.h>    //command from fep or IME to plugin. for vkb/hwr

#include "peninputeventjp.h"
#include "peninputhiraganakanji.h"
#include "peninputcontextfieldjp.h"
#include "peninputjapanesecandidatewnd.h"
#include "peninputjapanesepredictivewnd.h"

// CONSTANTS
const TUint16 KLittleCharCode = 0x5c0f; // little

//default value for long press timer
const TInt KLongPressInterval = 600000;
//default value for repeat timer
const TInt KRepeatInterval = 100000;

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
EXPORT_C CPeninputHiraganaKanji* CPeninputHiraganaKanji::NewL(CPtiEngine* aPtiEngine
                                                             , CFepUiLayout* aUiLayout
                                                             , CFepInputContextFieldJp* aInputContextField
                                                             , CPeninputJapaneseCandidateWnd* aCandidateWnd
                                                             , CPeninputJapanesePredictiveWnd* aPredictiveWnd)
    {
    if (aPtiEngine == NULL
         || aUiLayout == NULL
         || aInputContextField == NULL
         || aCandidateWnd == NULL
         || aPredictiveWnd == NULL
         )
        {
        User::Leave(KErrArgument);
        }

    CPeninputHiraganaKanji* self = new (ELeave) CPeninputHiraganaKanji(aPtiEngine
                                                                               , aUiLayout
                                                                               , aInputContextField
                                                                               , aCandidateWnd
                                                                               , aPredictiveWnd);

    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
EXPORT_C CPeninputHiraganaKanji::~CPeninputHiraganaKanji()
    {
    delete iCandidateArray;
    delete iChar;
    CancelRepeat();
    delete iLongPressTimer;
    delete iRepeatTimer;
    }

// ---------------------------------------------------------
// Handle events in
// ---------------------------------------------------------
EXPORT_C void CPeninputHiraganaKanji::HandleControlEventJpL(TInt aEventType, const TDesC& aEventData)
    {
    if(EStatusComplete == iStatus || EStatusCompleteAll == iStatus
          || EStatusCompleteAllfromUiLayout == iStatus) // wait for SetEditorTextL from Editor
        {
        return ;
        }
    switch (aEventType)
        {
        case EEventVirtualKeyUp:
            if (!aEventData.Length())
                {
                UpdateContextFieldL();
                }
            else
                {
                TUint16* unicode = (TUint16*) aEventData.Ptr();
                if (EStatusInitial == iStatus)
                    {
                    if (KLittleCharCode != *unicode)
                        {
                        iInputContextField->StartInlineL();
                        iStatus = (!iPredictiveWnd->Hiden())? EStatusPredictiveTransitoryInput : EStatusTransitoryInput;
                        }
                    }
                else if (EStatusPredictiveTransitoryInputWithNoChar == iStatus)
                    {
                    if (KLittleCharCode != *unicode)
                        {
                        iInputContextField->StartInlineL();
                        iStatus = EStatusPredictiveTransitoryInput;
                        }
                    }
                if (EStatusPredictiveTransitoryInput == iStatus || EStatusTransitoryInput == iStatus)
                    {
                    // different from editor
                    iStatus = (!iPredictiveWnd->Hiden())? EStatusPredictiveTransitoryInput : EStatusTransitoryInput;
                    HandleInsertL(aEventData);
                    }
                else if (EStatusConversion == iStatus
                       || EStatusCandidate == iStatus  // not take place
                       || EStatusChangeDivision == iStatus)
                    {
                    delete iChar;
                    iChar = NULL;
                    iChar = aEventData.AllocL();
                    CompleteAllL();
                    }
                }
            break;
        case EEventCompleteAllHiraganaKanji:    // CompleteALL:from UiLayout
            if (EStatusInitial != iStatus)
                {
                if (EStatusPredictiveTransitoryInputWithNoChar == iStatus)
                    {
                    iStatus = EStatusInitial;
                    UpdateContextFieldL();  // not leave
                    }
                else
                    {
                    iStatus = EStatusCompleteAllfromUiLayout;
                    CompleteAllL();
                    }
                }
            break;
        case EEventCompleteIfConvertingHiraganaKanji:    // Complete:from Hwr UiLayout
            if (EStatusConversion == iStatus
               || EStatusCandidate == iStatus  // not take place
               || EStatusChangeDivision == iStatus)
                {
                iStatus = EStatusCompleteAllfromUiLayout;
                CompleteAllL();
                }
            break;
        case EPeninputLayoutEventSelectItem:  // select from prediction/convert candidates by pointer
        case EPeninputLayoutEventEnter:
            if (EStatusPredictiveTransitoryInput == iStatus || EStatusPredictiveTransitoryInputWithNoChar == iStatus)
                {
                TInt curr = iPredictiveWnd->CurrentItemIndexOfCandidate();
                if (curr >= 0)
                    {
                    iPtiEngine->HandleCommandL(EPtiCommandUserActionCompleteCandidate, &curr);
                    CompleteSubL();  // not leave
                    }
                break;
                }
            if (EStatusCandidate == iStatus)
                {
                TInt curr = iCandidateWnd->CurrentItemIndex();
                TInt old = 0;
                iPtiEngine->HandleCommandL(EPtiCommandUserActionGetCurrentIndexOfCandidates, &old);
                while(curr > old)
                    {
                    ++old;
                    iPtiEngine->HandleCommandL(EPtiCommandUserActionConvert);
                    }
                while(curr < old)
                    {
                    --old;
                    iPtiEngine->HandleCommandL(EPtiCommandUserActionPrevious);
                    }
                }
            if (EStatusInitial != iStatus)
                {
                CompleteL();
                }
            break;
        case EPeninputLayoutEventBack:
            {
            switch(iStatus)
                {
                case EStatusPredictiveTransitoryInputWithNoChar:
                    {
                    iStatus = EStatusInitial;
                    UpdateContextFieldL();  // not leave
                    }
                    break;
                case EStatusTransitoryInput:
                case EStatusPredictiveTransitoryInput:
                    {
                    // different from editor
                    iStatus = (!iPredictiveWnd->Hiden())? EStatusPredictiveTransitoryInput : EStatusTransitoryInput;
                    HandleBackL();
                    }
                    break;
                case EStatusCandidate:
                    {
                    iStatus = EStatusConversion;
                    UpdateContextFieldL();
                    }
                    break;
                case EStatusConversion:
                case EStatusChangeDivision:
                    {
                    iPtiEngine->HandleCommandL(EPtiCommandUserActionCancel);
                    iStatus = EStatusTransitoryInput;
                    UpdateContextFieldL();
                    }
                    break;
                default:
                    break;
                }
            }
            break;
        case EEventStdKeyUpArrow:
        case EEventStdKeyDownArrow:
        case EPeninputLayoutEventSpace:
            if (EStatusInitial != iStatus)
                {
                if ((EStatusPredictiveTransitoryInput == iStatus || EStatusPredictiveTransitoryInputWithNoChar == iStatus)
                          && EPeninputLayoutEventSpace != aEventType)
                    {
                    iPredictiveWnd->MoveTo((EEventStdKeyUpArrow != aEventType)?
                                        CPeninputJapanesePredictiveWnd::EScrollDown : CPeninputJapanesePredictiveWnd::EScrollUp);
                    }
                else if (EStatusPredictiveTransitoryInputWithNoChar == iStatus && EPeninputLayoutEventSpace == aEventType)
                    {  // only by space button on vkb peninput
                    iStatus = EStatusInitial;
                    UpdateContextFieldL();
                    iUiLayout->SignalOwner(ESignalKeyEvent,aEventData);
                    }
                else
                    {
                    iPtiEngine->HandleCommandL(
                                 (EEventStdKeyUpArrow != aEventType)? EPtiCommandUserActionConvert : EPtiCommandUserActionPrevious);
                    if (EStatusCandidate == iStatus)   // not take place
                        {
                        iCandidateWnd->MoveToL((EEventStdKeyUpArrow != aEventType)?
                                                CPeninputJapaneseCandidateWnd::EScrollDown : CPeninputJapaneseCandidateWnd::EScrollUp);
                        }
                    else
                        {
                        iStatus = (EStatusConversion == iStatus)? EStatusCandidate : EStatusConversion;
                        }
                    UpdateContextFieldL();
                    }
                }
            break;
        case EPeninputLayoutEventTab:
            if (EStatusPredictiveTransitoryInputWithNoChar == iStatus)
                {
                iStatus = EStatusCompleteAll;   // wait for SetEditorTextL()
                iUiLayout->SignalOwner(ESignalKeyEvent,aEventData);
                }
            break;
        case EEventCancelHiraganaKanji:    // Cancel:from UiLayout   not used
            ClearTextL();
            break;
        case EEventStdKeyLeftArrow:
        case EEventStdKeyRightArrow:
            if (EStatusTransitoryInput == iStatus)
                {
                TInt pos = -1;
                iPtiEngine->HandleCommandL(
                             (EEventStdKeyLeftArrow == aEventType)? EPtiCommandUserActionLeft : EPtiCommandUserActionRight);
                const MPtiEngineCompositionDataInterface* data = iPtiEngine->CompositionData();
                if (data)
                    {
                    pos = data->CursorPosition();
                    }
                if(0 <= pos)
                    {
                    iInputContextField->SetCursorPosition(pos);
                    }
                }
            else if (EStatusPredictiveTransitoryInput == iStatus || EStatusPredictiveTransitoryInputWithNoChar == iStatus)
                {
                iPredictiveWnd->MoveTo((EEventStdKeyLeftArrow != aEventType)?
                                            CPeninputJapanesePredictiveWnd::EScrollRight : CPeninputJapanesePredictiveWnd::EScrollLeft);
                }
            else if (EStatusConversion == iStatus || EStatusCandidate == iStatus)
                {
                iStatus = EStatusChangeDivision;
                }
            if (EStatusChangeDivision == iStatus)
                {
                iPtiEngine->HandleCommandL(
                             (EEventStdKeyLeftArrow == aEventType)? EPtiCommandUserActionShorten : EPtiCommandUserActionLengthen);
                UpdateContextFieldL();
                }
            break;
        case EEventIcfPointerUp:  // from ContextField
            if (EStatusConversion == iStatus || EStatusCandidate == iStatus)
                {
                iPtiEngine->HandleCommandL(EPtiCommandUserActionShorten);
                iPtiEngine->HandleCommandL(EPtiCommandUserActionLengthen);
                iStatus = EStatusChangeDivision;
                UpdateContextFieldL();
                }
            else if (EStatusTransitoryInput == iStatus
                   || EStatusPredictiveTransitoryInput == iStatus
                   || EStatusPredictiveTransitoryInputWithNoChar == iStatus  // not take place
                   || EStatusChangeDivision == iStatus)
                {
                TInt pos = -1;
                TInt cursorPosition = *((TInt*)aEventData.Ptr());
                const MPtiEngineCompositionDataInterface* data = iPtiEngine->CompositionData();
                if (data)
                    {
                    pos = data->CursorPosition();
                    if (EStatusChangeDivision == iStatus)
                        {
                        if (data->NumberOfPhrase() > 0)
                            {
                            TPtrC8 clauseChar = data->LengthOfPhrase();
                            pos = clauseChar[0];
                            }
                        }
                    }
                if(0 <= pos)
                    {
                    TInt posDiff = cursorPosition - pos;
                    if(posDiff)
                        {
                        TPtiEngineCommand cmd;
                        if (EStatusChangeDivision == iStatus)
                            {
                            cmd = (posDiff < 0)? EPtiCommandUserActionShorten : EPtiCommandUserActionLengthen;
                            }
                        else
                            {
                            cmd = (posDiff < 0)? EPtiCommandUserActionLeft : EPtiCommandUserActionRight;
                            }
                        if (posDiff < 0)
                            {
                            posDiff = -posDiff;
                            }
                         while(posDiff > 0)
                            {
                            -- posDiff;
                            iPtiEngine->HandleCommandL(cmd);
                            }
                        }
                    if (EStatusChangeDivision == iStatus)
                        {
                        iPtiEngine->HandleCommandL(EPtiCommandUserActionConvert);
                        iStatus = EStatusConversion;
                        UpdateContextFieldL();
                        }
                    }
                }
            break;
		case EEventIcfCompleteAll:  // from ContextField
            {
            CompleteL();
            }
            break;
        case EEventICFSelectionChanged:  // from ContextField (when whithout transitory input)
            {
            }
            break;
        case EEventSetAppCursorSelection:  // from ContextField
            { // clear prediction pane
            if (EStatusPredictiveTransitoryInputWithNoChar == iStatus)
                {
                iStatus = EStatusInitial;
                UpdateContextFieldL();
                }
            }
            break;
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPeninputHiraganaKanji::HandleKeyEvent
// without implementation
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C TBool CPeninputHiraganaKanji::HandleKeyEvent(const TRawEvent& aData)
    {
    TBool ret = EFalse;
    iPendingEvent = 0;
    if(EStatusComplete == iStatus || EStatusCompleteAll == iStatus
          || EStatusCompleteAllfromUiLayout == iStatus) // wait for SetEditorTextL from Editor
        {
        return ETrue;
        }
    const MPtiEngineCompositionDataInterface* data = iPtiEngine->CompositionData();
    if (!data)
        {
        return ret;
        }
    TPtrC text = data->ConvertingString();
    if (text.Length() > 0 || EStatusPredictiveTransitoryInputWithNoChar == iStatus)
        {
        TInt key = (aData.ScanCode() & 0x000000FF);
        TInt evetType = EPeninputLayoutEventEnter;
        TInt repeat = 0;
        switch(key)
            {
            case EKeyUpArrow:
            case EStdKeyUpArrow:        /* 0x10 */
                {
                evetType = EEventStdKeyUpArrow;
                ret = ETrue;
                repeat = 1;
                }
                break;
            case EKeyDownArrow:
            case EStdKeyDownArrow:
                {
                evetType = EEventStdKeyDownArrow;
                ret = ETrue;
                repeat = 1;
                }
                break;
            case EKeyLeftArrow:
            case EStdKeyLeftArrow:      /* 0x0e */
                {
                evetType = EEventStdKeyLeftArrow;
                ret = ETrue;
                repeat = 1;
                }
                break;
            case EKeyRightArrow:
            case EStdKeyRightArrow:     /* 0x0f */
                {
                evetType = EEventStdKeyRightArrow;
                ret = ETrue;
                repeat = 1;
                }
                break;
            case EKeyOK:
            case EStdKeyDevice3:        /* 0xA7 */
            case EStdKeyEnter:          /* 0x03 */
            case EStdKeyNkpEnter:       /* 0x88 */
                {
                evetType = EPeninputLayoutEventEnter;
                ret = ETrue;
                }
                break;
            case EKeyBackspace:
            case EStdKeyBackspace:      /* 0x01 */
                {
                evetType = EPeninputLayoutEventBack;
                ret = ETrue;
                repeat = 1;
                }
                break;
            case EStdKeyEscape:         /* 0x04 */
                {
                evetType = EPeninputLayoutEventBack;
                ret = ETrue;
                }
                break;
            case EStdKeyTab:         /* 0x02 */
                {
                evetType = EPeninputLayoutEventTab;
                ret = ETrue;
                }
                break;
            case EStdKeySpace:         /* 0x05 */
                {
                if (EStatusPredictiveTransitoryInputWithNoChar == iStatus)
                    {
                    iStatus = EStatusInitial;
                    TRAP_IGNORE(UpdateContextFieldL());  // not Leave
                    }
                else
                    {
                    evetType = EPeninputLayoutEventSpace;
                    ret = ETrue;
                    }
                }
                break;
            default:
                break;
            }
        if(ret && TRawEvent::EKeyDown == aData.Type())
            {
            iPendingEvent = evetType;
            iRepeat = repeat;
            }
        if(TRawEvent::EKeyUp == aData.Type())
            {
            CancelRepeat();
            }
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CPeninputHiraganaKanji::HandlePendingEventL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
EXPORT_C TBool CPeninputHiraganaKanji::HandlePendingEventL()
    {
    if (iPendingEvent)
        {
        if (EPeninputLayoutEventBack == iPendingEvent)
            {
            iUiLayout->HandleControlEvent(EPeninputLayoutEventBack, reinterpret_cast<CFepUiBaseCtrl*>(this), KNullDesC);
            }
        else  // up/down/left/right arrow
            {
            HandleControlEventJpL(iPendingEvent, KNullDesC);
            }
        if (iRepeat)
            {
            iLongPressTimer->SetTimer(KLongPressInterval);
            }
        else
            {
            iPendingEvent = 0;
            }
        return ETrue;
        }
    return EFalse;
    }

// ---------------------------------------------------------------------------
// CPeninputHiraganaKanji::SetEditorTextL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
EXPORT_C void CPeninputHiraganaKanji::SetEditorTextL(const TFepInputContextFieldData& aData)
    {
    if(EStatusComplete == iStatus || EStatusCompleteAll == iStatus || EStatusCompleteAllfromUiLayout == iStatus)
        {
        iCandidateArray->Reset();
        iCandidateWnd->HidePopup();
        }
    if(iInputContextField)
        {
        iInputContextField->SetTextL(aData);
        }
    if(EStatusComplete == iStatus || EStatusCompleteAll == iStatus || EStatusCompleteAllfromUiLayout == iStatus)
        {
        if (EStatusCompleteAllfromUiLayout == iStatus)
            {
            iStatus = EStatusInitial;
            }
        else if (EStatusCompleteAll == iStatus)
            {
            if (!iPredictiveWnd->Hiden())
                {
                iStatus = EStatusPredictiveTransitoryInputWithNoChar;
                iUiLayout->HandleControlEvent(EEventHiraganaKanjiWithoutTransitoryChars, reinterpret_cast<CFepUiBaseCtrl*>(this), KNullDesC);
                }
            else
                {
                iStatus = EStatusInitial;
                }
            if (iChar)   // next input exists
                {
                UpdateContextFieldL();
                iUiLayout->HandleControlEvent(EEventVirtualKeyUp, reinterpret_cast<CFepUiBaseCtrl*>(this), *iChar);
                delete iChar;
                iChar = NULL;
                return;
                }
            }
        else
            {
            iStatus = EStatusConversion;
            
            if( iInputContextField != NULL )
                {
                iInputContextField->StartInlineL();
                }
            }
        UpdateContextFieldL();
        }
    }

// ---------------------------------------------------------
// Cancel timers for key repeat
// ---------------------------------------------------------
//
EXPORT_C void CPeninputHiraganaKanji::CancelRepeat()
    {
    if (iRepeat)
        {
        if (iLongPressTimer)
            {
            iLongPressTimer->Cancel();
            }
        if (iRepeatTimer)
            {
            iRepeatTimer->Cancel();
            }
        iPendingEvent = 0;
        iRepeat = 0;
        }
    iCandidateWnd->CancelScrollBarRepeat();
    iPredictiveWnd->CancelRepeat();
    }

// ---------------------------------------------------------
// Start timers for clear key repeat
// ---------------------------------------------------------
//
EXPORT_C void CPeninputHiraganaKanji::RepeatClearKeyStart()
    {
    if (!iRepeat)
        {
        iPendingEvent = EPeninputLayoutEventBack;
        iRepeat = 1;
        iLongPressTimer->SetTimer(KLongPressInterval);
        }
    }

// ---------------------------------------------------------
// Time out event handler of both long press timer & repeat timer
// ---------------------------------------------------------
//
void CPeninputHiraganaKanji::HandleTimerOut(const CAknFepTimer* aTimer)
    {
    if (aTimer == iLongPressTimer)
        {
        iRepeatTimer->SetTimer(KRepeatInterval);
        }
    else
        {
        if (aTimer == iRepeatTimer)
            {
            if (iPendingEvent)
                {
                if (EPeninputLayoutEventBack == iPendingEvent)
                    {
                    iUiLayout->HandleControlEvent(EPeninputLayoutEventBack, reinterpret_cast<CFepUiBaseCtrl*>(this), KNullDesC);
                    }
                else   // up/down/left/right arrow
                    {
                    TRAP_IGNORE(HandleControlEventJpL(iPendingEvent, KNullDesC));
                    }
                iRepeatTimer->SetTimer(KRepeatInterval);
                }
            }
        }
    }

// ---------------------------------------------------------
// C++ Constructor
// ---------------------------------------------------------
CPeninputHiraganaKanji::CPeninputHiraganaKanji(CPtiEngine* aPtiEngine
                                                                   , CFepUiLayout* aUiLayout
                                                                   , CFepInputContextFieldJp* aInputContextField
                                                                   , CPeninputJapaneseCandidateWnd* aCandidateWnd
                                                                   , CPeninputJapanesePredictiveWnd* aPredictiveWnd)
    : iPtiEngine(aPtiEngine),
      iUiLayout(aUiLayout),
      iInputContextField(aInputContextField),
      iStatus(EStatusInitial),
      iCandidateWnd(aCandidateWnd),
      iPredictiveWnd(aPredictiveWnd)
    {
    }

// ---------------------------------------------------------
// 2nd Constructor
// ---------------------------------------------------------
void CPeninputHiraganaKanji::ConstructL()
    {
    iCandidateArray = new(ELeave) CDesCArrayFlat(1);
    iCandidateWnd->SetIcf(iInputContextField);
    iLongPressTimer = CAknFepTimer::NewL(this);
    iRepeatTimer = CAknFepTimer::NewL(this);
    }

// ---------------------------------------------------------
// Insert a key
// ---------------------------------------------------------
void CPeninputHiraganaKanji::HandleInsertL(const TDesC& aEventData)
    {
    TUint16* unicode = (TUint16*)aEventData.Ptr();
    TUint ch;
    for (TInt i = 0; i < aEventData.Length(); i++)
        {
        ch = *(unicode + i);
        iPtiEngine->HandleCommandL(EPtiCommandAppendCharacter, &ch);
        }
    UpdateContextFieldL();
    }

// ---------------------------------------------------------
// Cancel From UiLayout   not used
// ---------------------------------------------------------
void CPeninputHiraganaKanji::ClearTextL()
    {
    iPtiEngine->ClearCurrentWord();
    CancelL();
    UpdateContextFieldL();
    }

// ---------------------------------------------------------
// complete
// ---------------------------------------------------------
void CPeninputHiraganaKanji::CompleteL()
    {
    iPtiEngine->HandleCommandL(EPtiCommandUserActionComplete);
    CompleteSubL();
    }

// ---------------------------------------------------------
// complete all
// ---------------------------------------------------------
void CPeninputHiraganaKanji::CompleteAllL()
    {
    iPtiEngine->HandleCommandL(EPtiCommandUserActionAllComplete);
    if (EStatusCompleteAllfromUiLayout == iStatus)
        {
        iPredictiveWnd->HidePopup();
        iCandidateArray->Reset();
        iCandidateWnd->HidePopup();
        }
    CompleteSubL();
    }

// ---------------------------------------------------------
// CompleteSubL
// ---------------------------------------------------------
void CPeninputHiraganaKanji::CompleteSubL()
    {
    const MPtiEngineCompositionDataInterface* data = iPtiEngine->CompositionData();
    if (!data)
        {
        return;
        }
    TPtrC completedString = data->CompletedString();
    TPtrC convertingString = data->ConvertingString();
    TInt sviStatus = iStatus;
    if (completedString.Length() > 0)
        {
        if (EStatusCompleteAllfromUiLayout != iStatus)
            {
            iStatus = (convertingString.Length())? EStatusComplete : EStatusCompleteAll;
            }
        if (EStatusPredictiveTransitoryInputWithNoChar != sviStatus)
            {
            if (EStatusComplete == iStatus)
                {
                iInputContextField->CommitInlineL();
                }
            else
                {
                iInputContextField->CompleteInlineL();
                }
            }
        iUiLayout->SignalOwner(ESignalKeyEvent,completedString);
        if (EStatusCompleteAllfromUiLayout == iStatus)
            {
            iUiLayout->HandleControlEvent(EEventWaitforEditorTextComing, reinterpret_cast<CFepUiBaseCtrl*>(this), KNullDesC);
            }
        }
    else
        {
        if (EStatusPredictiveTransitoryInputWithNoChar == iStatus
              || EStatusCompleteAllfromUiLayout == iStatus)
            {
            if (iStatus == EStatusCompleteAllfromUiLayout)  // exceptional case
                {
                iInputContextField->CancelInlineL();
                }
            iStatus = EStatusInitial;
            }
        UpdateContextFieldL();  // not leave
        }
    }

// ---------------------------------------------------------
// Handle back key
// iStatus
//     EStatusTransitoryInput:
//     EStatusPredictiveTransitoryInput:
// ---------------------------------------------------------
void CPeninputHiraganaKanji::HandleBackL()
    {
    const MPtiEngineCompositionDataInterface* data = iPtiEngine->CompositionData();
    if (!data)
        {
        return;
        }
    TPtrC text = data->ConvertingString();
    if (!text.Length())  // not take place
        {
        return;
        }
    iPtiEngine->DeleteKeyPress();
    const MPtiEngineCompositionDataInterface* dataAfter = iPtiEngine->CompositionData();
    if (!dataAfter)
        {
        return;
        }
    TPtrC textAfter = dataAfter->ConvertingString();
    if (!textAfter.Length())
        {
        CancelL();
        }
    UpdateContextFieldL();
    }

// ---------------------------------------------------------
// Update context field : ConvertingString
// ---------------------------------------------------------
//
void CPeninputHiraganaKanji::UpdateContextFieldL()
    {
    const MPtiEngineCompositionDataInterface* data = iPtiEngine->CompositionData();
    if (!data)
        {
        return;
        }
    TPtrC text = data->ConvertingString();
    if (EStatusPredictiveTransitoryInputWithNoChar == iStatus)
        {
        iPtiEngine->ClearCurrentWord();
        iInputContextField->SetCursorVisible( ETrue );
        // update popup window
        UpdateCandidateListL();
        return;
        }
    if (EStatusPredictiveTransitoryInput == iStatus || EStatusTransitoryInput == iStatus)
        {
        if (text.Length() > 0)
            {
            iInputContextField->UpdateInlineL(text, -1);
            TInt pos = data->CursorPosition();
            if(0 <= pos)
                {
                iInputContextField->SetCursorPosition(pos);
                }
            iInputContextField->SetCursorVisible( ETrue );
            // update popup window
            UpdateCandidateListL();
            return;
            }
        }
    if (EStatusConversion == iStatus || EStatusCandidate == iStatus || EStatusChangeDivision == iStatus)
        {
        if (text.Length() > 0 && data->NumberOfPhrase() > 0)
            {
            TPtrC8 clauseChar = data->LengthOfPhrase();
            // update inline on ContextField
            if ( EStatusChangeDivision == iStatus )
                {
                iInputContextField->UpdateInlineL(text, clauseChar[0], CFepInputContextFieldJp::ETransitoryInputAreaSwitching);
                }
            else
                {
                iInputContextField->UpdateInlineL(text, clauseChar[0]);
                }
            iInputContextField->SetCursorVisible(EFalse);
            UpdateCandidateListL();
            return;
            }
        }
    if (EStatusInitial != iStatus)    // can take place : at EEventVirtualKeyUp(data length == 0)
        {
        CancelL();
        }
    iPtiEngine->ClearCurrentWord();
    iInputContextField->SetCursorVisible( ETrue );
    iUiLayout->HandleControlEvent(EEventHiraganaKanjiWithTransitoryChars2Standbyjp, reinterpret_cast<CFepUiBaseCtrl*>(this), KNullDesC);
    UpdateCandidateListL();
    }

// ---------------------------------------------------------
// Update Predictive Popup
// ---------------------------------------------------------
void CPeninputHiraganaKanji::UpdateCandidateListL()
    {
    if (EStatusPredictiveTransitoryInput == iStatus || EStatusPredictiveTransitoryInputWithNoChar == iStatus || EStatusCandidate == iStatus)
        {
        TInt index = 0;
        iCandidateArray->Reset();
        iPtiEngine->GetCandidateListL(*iCandidateArray);
        if (EStatusCandidate == iStatus)
            {
            iPtiEngine->HandleCommandL(EPtiCommandUserActionGetCurrentIndexOfCandidates, &index);
            if (index >= iCandidateArray->Count())    // not take place
                {
                index = 0;
                }
            iPredictiveWnd->HidePopup();
            iCandidateWnd->ShowPopupL(iCandidateArray, index);
            // capture of candidate control removed. Trensitory Input Exists, then the capture flag should be on.
            //signal the owner that pointer needs be captured
                {
                TBool capture = ETrue;
                TPtrC data;
                data.Set((const TUint16 *)&capture,sizeof(TBool)/sizeof(TUint16));
                iUiLayout->LayoutOwner()->SignalOwner(ESignalCapturePointer,data);
                }
            }
        else
            {
            iCandidateWnd->HidePopup();
            iPredictiveWnd->ShowPopupL(iCandidateArray, index);
            }
        }
    else
        {
        iPredictiveWnd->HidePopup();
        iCandidateWnd->HidePopup();
        }
    }

// ---------------------------------------------------------
// Cancel Transitory Input
// ---------------------------------------------------------
void CPeninputHiraganaKanji::CancelL()
    {
    iStatus = EStatusInitial;
    iInputContextField->CancelInlineL();
    }

// End Of File
