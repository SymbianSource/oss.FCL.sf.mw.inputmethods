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
* Description:  Implementation of the vkb layout
*
*/


// system includes
#include <AknFepGlobalEnums.h> // by w45wang for the temp
#include <peninputjapanesepredictivewnd.h>
#include <peninputhiraganakanji.h>       // hiraganakanji
#include <peninputuistatemgr.h>    // use ui state mgr
#include <peninputdataconverter.h>
#include <aknfeppeninputenums.h>    //command from fep or IME to plugin. for vkb/hwr
#include <peninputcommonlayoutglobalenum.h>  // EPeninputLayoutEventEnter
#include <txtetext.h>  // CEditableText::EParagraphDelimiter

// user includes
#include "peninputvkblayout.h"
#include "peninputvkbuistatetype.h"         // UI state
#include "peninputvkb.hrh"            // control ID and range defination
#include "peninputvkbwindow.h"        // use vkb window, which derived from base window
#include "peninputvkbdatamgr.h"       // use data mgr
#include "peninputvkbuistatestandby.h"
#include "peninputvkbuistatestandbyjp.h"
#include "peninputvkbuistatewithtransitorychars.h"
#include "peninputcontextfieldjp.h"

// constant definition
const TInt KPeninputVkbFullSpaceCode = 0x3000;
const TInt KDelay = 100000; //1/10 second
const TInt KPeninputBufLen = 16;
const TUint KHalfDakuten = 0xff9e;
const TUint KHalfHanDakuten = 0xff9f;
const TUint KFullDakuten = 0x309b;
const TUint KFullHanDakuten = 0x309c;
const TUint KLittle = 0x5c0f;

const TInt KModeSwitchBufLen = 8; // by w45wang for the temp

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPeninputVkbLayout::NewL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPeninputVkbLayout* CPeninputVkbLayout::NewL(MLayoutOwner* aLayoutOwner,
                                         const TAny* aInitData)
    {
    CPeninputVkbLayout* self = new (ELeave) CPeninputVkbLayout(aLayoutOwner);

    CleanupStack::PushL(self);
    self->ConstructL(aInitData);
    CleanupStack::Pop(self);

    return self;
    }

// ---------------------------------------------------------------------------
// CPeninputVkbLayout::~CPeninputVkbLayout
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPeninputVkbLayout::~CPeninputVkbLayout()
    {
    delete iHiraganaKanji;
    delete iKeyEventAsync;
    delete iPointerEventAsync;
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxLayout::HandleEventL
// handle event from window server
// ---------------------------------------------------------------------------
//
TBool CPeninputVkbLayout::HandleEventL(TEventType aType, const TAny* aData)
    {
    TRawEvent event=*((TRawEvent*)aData);
    if(ERawEvent == aType)
        {
        if (TRawEvent::EButton1Down == event.Type() && iHiraganaKanji)
            {
            iHiraganaKanji->CancelRepeat();
            }
        }
    return CPeninputCommonLayout::HandleEventL(aType,aData);
    }

// ---------------------------------------------------------------------------
// CPeninputVkbLayout::HandleCommand
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CPeninputVkbLayout::HandleCommand(TInt aCmd, TUint8* aData)
    {
    // Handle command from fep or IME to this layout
    TUint* data = (TUint*) aData;

    switch ( aCmd )
        {
        case ECmdPenInputPermittedRange:  // different from common layout
            // this command comes after ECmdPenInputJapaneseSetting
            if (iDataMgr)
                {
                static_cast<CPeninputVkbDataMgr*>(iDataMgr)->SetPermittedRange(*data);
                }
            break;
        case ECmdPenInputLanguage:
            {
            CPeninputCommonLayout::HandleCommand(aCmd, aData);
            }
            break;
        case ECmdPenInputWindowOpen:
            if (iVkbWindow)
                {
                iVkbWindow->Opened();
                TInt curRange = CPeninputDataConverter::AnyToInt(RequestData(EPeninputDataTypeCurrentRange));
                TBool currHide = iVkbWindow->PredictiveWindow()->Hiden();
                TBool newHide = ETrue;
                if (static_cast<CPeninputVkbDataMgr*>(iDataMgr)->Predictive()
                              &&  ERangeHiraganaKanji == curRange)
                    {
                    newHide = EFalse;
                    }
                if (newHide && !currHide || !newHide && currHide)
                    {
                    iVkbWindow->PredictiveWindow()->Hide(newHide);
                    }
                CPeninputCommonLayout::HandleCommand( aCmd, aData );
                if (*data != EPenInputOpenManually && Position() == TPoint(0,0))
                    {  // position is not yet set.(English --> Japanese)
                    SendEventToWindow(EPeninputLayoutEventMovePosition, NULL, KNullDesC);
                    }
                iWaitforEditorTextComing = 0;
                iVkbWindow->ICFEnableRegionUpdating();  // ICF::OnActivate()
                }
            break;
        case ECmdPenInputWindowClose:
            {
            HiraganaKanjiCompleteAll();
            CPeninputCommonLayout::HandleCommand( aCmd, aData );
            UiStateMgr()->SetCurrentUiState( EPeninputVkbUiStateNone );
            SetCapturePointerEvent(EFalse);
            iVkbWindow->Closed();
            iWaitforEditorTextComing = 0;
            }
            break;
        case ECmdPenInputRange:  // different from common layout:control ptiEngine
            // this command comes after ECmdPenInputJapaneseSetting, ECmdPenInputPermittedRange
            {
            iRangeOrg = TranslateRangeHullHalf(*data);
            if (iLayoutWindow)
                {
                iVkbWindow->ICFDisableRegionUpdating();  // ICF::OnDeActivate()
                iVkbWindow->SetSizeToRangeButtons();  // To Call CAknFepCtrlButton::ResizeBitmaps()
                TBool currHide = iVkbWindow->PredictiveWindow()->Hiden();
                TBool newHide = ETrue;
                if (static_cast<CPeninputVkbDataMgr*>(iDataMgr)->Predictive()
                              &&  ERangeHiraganaKanji == iRangeOrg)
                    {
                    newHide = EFalse;
                    }
                if (newHide && !currHide || !newHide && currHide)
                    {
                    LayoutOwner()->Hide( ETrue );
                    }
                }
            TUint range = iRangeOrg;
            TUint8* rangedata = reinterpret_cast<TUint8*>(&range);
            CPeninputCommonLayout::HandleCommand( aCmd, rangedata );
            DoSetCurrentRange();
            }
            break;
        case ECmdPenInputJapaneseSetting:  // Japanese setting information
            // this command comes before ECmdPenInputPermittedRange, ECmdPenInputRange
            {
            TUint val = *data;
            TInt sctDimmFlag = (val & EPenInputJapaneseSettingLunchSCT)? 0: 1;
            TPtr ptr((TUint16*)&sctDimmFlag, sizeof(TInt)/sizeof(TUint16));
            SendEventToWindow(EEventSetDimmedSctBtn, NULL, ptr);
            static_cast<CPeninputVkbDataMgr*>(iDataMgr)->SetFullHalf(
                            (val & EPenInputJapaneseSettingCharacterWidth)? 1: 0);
            static_cast<CPeninputVkbDataMgr*>(iDataMgr)->SetPredictive(
                            (val & EPenInputJapaneseSettingPredictive)? ETrue: EFalse);
            static_cast<CPeninputVkbDataMgr*>(iDataMgr)->SetClear(
                            (val & EPenInputJapaneseSettingDeletingDirection)? 1: 0);
            static_cast<CPeninputVkbDataMgr*>(iDataMgr)->SetJapaneseFlags(val);
            }
            break;
        case ECmdPenInputEnableSettingBtn:
            iVkbWindow->SetEnableSettingBtnJp(*data);
            break;
        case ECmdPenInputSendEditorTextAndCurPos:
            CFepUiLayout::HandleCommand( aCmd, aData );
            break;

        default:
            CPeninputCommonLayout::HandleCommand( aCmd, aData );
            break;
        }
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CPeninputVkbLayout::HandleControlEvent
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputVkbLayout::HandleControlEvent(TInt aEventType, CFepUiBaseCtrl* aCtrl, const TDesC& aEventData)
    {
    // Handle event from control
    TInt* data = (TInt*)aEventData.Ptr();

    //deal with the event from all controls in layout
    switch (aEventType)
        {
        case EPeninputLayoutEventClose:
        case EPeninputLayoutEventOption:
        case EPeninputLayoutEventToHwr:
            {
            HiraganaKanjiCompleteAll();
            SetCapturePointerEvent(EFalse);
            if (EPeninputLayoutEventToHwr == aEventType)
                {
                // by w45wang for the temp
                TBool switchByMode = ETrue;
                TInt mode = EPluginInputModeHwr;
                TBuf<KModeSwitchBufLen> buf;
                CPeninputDataConverter::CombinationToDesc( switchByMode, mode, buf );
                SignalOwner( ESignalLayoutUIChanged, buf );
                // end
                }
            else
                {
                CPeninputCommonLayout::HandleControlEvent(aEventType, aCtrl, aEventData);
                }
            iWaitforEditorTextComing = 0;
            }
            break;
        case EPeninputLayoutEventSct:
            HiraganaKanjiCompleteAll();
            SetCapturePointerEvent(EFalse);
            SignalOwner(ESignalHwNotifySctPopup);
            break;
        case EPeninputLayoutEventSwitchLanguage:  // nop
            break;

        case EPeninputLayoutEventRange:  // different from common layout:control ptiEngine
            {
            HiraganaKanjiCompleteAll();
            TInt permittedRange = CPeninputDataConverter::AnyToInt(RequestData(EPeninputDataTypePermittedRange));
            TInt range = TranslateRangeHullHalf(*data);
            if (!(range & permittedRange))
                {
                range = iRangeOrg;
                }
            TPtrC rangeptr((TUint16*)(&range), sizeof(TInt)/sizeof(TUint16));
            TBool currHide = iVkbWindow->PredictiveWindow()->Hiden();
            TBool newHide = ETrue;
            if (static_cast<CPeninputVkbDataMgr*>(iDataMgr)->Predictive()
                              &&  ERangeHiraganaKanji == *data)
                {
                newHide = EFalse;
                }
            if (newHide && !currHide || !newHide && currHide)
                {
                LayoutOwner()->Hide(ETrue);   // display off
                iVkbWindow->PredictiveWindow()->Hide(newHide);
                }
            else
                {
                iVkbWindow->ICFDisableRegionUpdating();  // ICF::OnDeActivate()
                }
            CPeninputCommonLayout::HandleControlEvent(aEventType, aCtrl, rangeptr);
            if (ERangeEnglish != range && ERangeNumber != range)
                {
                TInt native = ERangeNative;
                TBuf<KPeninputBufLen> buf;
                CPeninputDataConverter::CombinationToDesc( native, range, buf );
                Sendkey( ESignalRange, buf );
                }
            DoSetCurrentRange();
            if (newHide && !currHide || !newHide && currHide)
                {
                LayoutOwner()->Hide(EFalse);   // display on
                }
            else
                {
                iVkbWindow->ICFEnableRegionUpdating();  // ICF Cursor On
                }
            }
            break;

        // from HiraganaKanji
        case EEventHiraganaKanjiWithTransitoryChars2Standbyjp:
            if (UiStateMgr() && UiStateMgr()->CurrentUiState() == UiStateMgr()->GetUiState(EPeninputVkbUiStateWithTransitoryChars))
                {
                UiStateMgr()->SetCurrentUiState(EPeninputVkbUiStateStandbyjp);
                SetCapturePointerEvent(EFalse);
                }
            break;
        // from HiraganaKanji
        case EEventHiraganaKanjiWithoutTransitoryChars:
            if (UiStateMgr() && UiStateMgr()->CurrentUiState() == UiStateMgr()->GetUiState(EPeninputVkbUiStateWithTransitoryChars))
                {
                SetCapturePointerEvent(EFalse);
                }
            break;

        case EEventVirtualKeyUp:
            {
            //  convert kutenn/touten code
            TUint newChar = *((TUint16*)data);  // case : aEventData length == 1
            TInt len = aEventData.Length();
            if (1 != len)
                {
                TVirtualKeyEventData* dataPtr = (TVirtualKeyEventData* ) aEventData.Ptr();
                newChar = dataPtr->iKeyEvent.iCode;
                }
            TPtrC eventData(reinterpret_cast<TUint16*>(&newChar), sizeof(TUint16)/sizeof(TUint16));
            TBool ret = static_cast<CPeninputVkbDataMgr*>(iDataMgr)->ConvertCommaFullStop(newChar);

            // Kana : dakuten,handakuten,little
            if (!ret && UiStateMgr()->CurrentUiState() == UiStateMgr()->GetUiState(EPeninputVkbUiStateStandby) )
                {
                TChar preChar = iVkbWindow->InputContextField()->CharacterOnCursor();
                if (preChar == TChar(CEditableText::EParagraphDelimiter))
                    {
                    preChar = TChar( KNoChar );
                    }
                if (preChar != TChar( KNoChar ))
                    {
                    if (iWaitforEditorTextComing > 0)
                        {
                        -- iWaitforEditorTextComing;
                        break;
                        }
                    TBool status = EFalse;
                    TInt currentRange = CPeninputDataConverter::AnyToInt( RequestData( EPeninputDataTypeCurrentRange ) );
                    if (ERangeKatakana == currentRange && (KHalfDakuten == newChar || KHalfHanDakuten == newChar || KLittle == newChar))
                        {
                        TChar prePreChar = iVkbWindow->InputContextField()->CharacterAroundCursor(-1);
                        if (prePreChar == TChar(CEditableText::EParagraphDelimiter))
                            {
                            prePreChar = TChar( KNoChar );
                            }
                        TRAP_IGNORE(static_cast<CPeninputVkbDataMgr*>(iDataMgr)->HandleFunctionalVkbEventWithPrePreCharL(eventData, TUint(preChar), TUint(prePreChar)
                                                                                                                         , status, iWaitforEditorTextComing))
                        }
                    else if (KFullDakuten == newChar || KFullHanDakuten == newChar || KLittle == newChar)
                        {
                        TRAP_IGNORE(static_cast<CPeninputVkbDataMgr*>(iDataMgr)->HandleFunctionalVkbEventL(eventData, TUint(preChar)
                                                                                                           , status, iWaitforEditorTextComing))
                        }
                    if (iWaitforEditorTextComing)
                        {
                        iWaitforEditorTextComing = 5;
                        }
                    if (status)
                        {
                        break;
                        }
                    }
                else    // if the editor stirng is empty, ignore little
                    {
                    TUint16* text = (TUint16*)eventData.Ptr();  // always aEventData length == 1
                    if (TUint16(0x5c0f) == *text)
                        {
                        iWaitforEditorTextComing = 0;
                        break;
                        }
                    }
                }
            iWaitforEditorTextComing = 0;
            if ( UiStateMgr()->CurrentUiState()
            && !static_cast<CPeninputVkbUiState*>(UiStateMgr()->CurrentUiState())->HandleControlEvent(aEventType,eventData) )
                {
                SetCapturePointerEvent(ETrue);
                HiraganaHandleControlEventJp(aEventType, eventData);
                }
            }
            break;
        case EPeninputLayoutEventBack:
            {
            //  update clear code : backspace/delete
            // 1:delete 0:backspace
            TInt clear = EKeyBackspace;
            TPtrC clearptr((TUint16*)(&clear), 1);
            if ( UiStateMgr()->CurrentUiState()
            && !static_cast<CPeninputVkbUiState*>(UiStateMgr()->CurrentUiState())->HandleControlEvent(aEventType,clearptr) )
                {
                HiraganaHandleControlEventJp(aEventType, clearptr);
                }
            }
            break;
        case EPeninputLayoutEventEnter:
        case EPeninputLayoutEventSelectItem:  // select from prediction/convert candidates by pointer
        case EPeninputLayoutEventTab:
            if (EPeninputLayoutEventEnter == aEventType && IsPredictiveWithNoChar())
                {    // without transitory chars and next word prediction candidates are offered
                     // Enter button should clear Prediction and enter a newline
                TRAP_IGNORE(iHiraganaKanji->HandleControlEventJpL(EPeninputLayoutEventBack,KNullDesC));
                }
            if ( UiStateMgr()->CurrentUiState()
            && !static_cast<CPeninputVkbUiState*>(UiStateMgr()->CurrentUiState())->HandleControlEvent(aEventType,aEventData) )
                {
                TRAP_IGNORE(iHiraganaKanji->HandleControlEventJpL(aEventType,aEventData));
                }
            break;
        case EPeninputLayoutEventSpace:
            {
            //  update space code : full/half
            TInt space = *data;
            TPtrC spaceptr((TUint16*)(&space), 1);
            if (static_cast<CPeninputVkbDataMgr*>(iDataMgr)->JapaneseFlags() & EPenInputJapaneseSettingQwertyWidthOfSpace)
                {
                TInt currentRange = CPeninputDataConverter::AnyToInt
                      ( RequestData( EPeninputDataTypeCurrentRange ) );
                TInt fullWidth = (ERangeHiraganaKanji
                           | ERangeFullWidthKatakana
                           | ERangeFullWidthEnglish
                           | ERangeFullWidthNumeric
                           | ERangeHiraganaOnly );
                if ( currentRange & fullWidth)
                    {
                    space = KPeninputVkbFullSpaceCode;
                    }
                }
            if ( UiStateMgr()->CurrentUiState()
            && !static_cast<CPeninputVkbUiState*>(UiStateMgr()->CurrentUiState())->HandleControlEvent(aEventType,spaceptr) )
                {
                HiraganaHandleControlEventJp(aEventType, spaceptr);
                }
            }
            break;
        case EEventIcfPointerUp:  // from ContextField
        case EEventIcfCompleteAll:  // from ContextField
            {
            TRAP_IGNORE(iHiraganaKanji->HandleControlEventJpL(aEventType,aEventData));
            SetCapturePointerEventAgain();
            }
            break;
        case EEventSizeChanged:   // PredictiveWnd On/Off resize and redraw
            if (iVkbWindow && static_cast<CFepUiBaseCtrl*>(iVkbWindow->PredictiveWindow()) == aCtrl)
                {
                iVkbWindow->SizeChanged(ETrue);
                }
            else
                {
                CPeninputCommonLayout::HandleControlEvent(aEventType, aCtrl, aEventData);
                }
            break;
        case EEventShowCandidateWnd:   // CandidateWnd On
            {
            iVkbWindow->SetDimmed(ETrue);
            }
            break;
        case EEventHideCandidateWnd:   // CandidateWnd Off
            {
            iVkbWindow->SetDimmed(EFalse);
            }
            break;
        case EEventWaitforEditorTextComing:   // may use, someday.
            {
            iWaitforEditorTextComing = 1;
            }
            break;
        case EEventDraggingEnd:
            {
            CPeninputCommonLayout::HandleControlEvent(aEventType, aCtrl, aEventData);
            SetCapturePointerEventAgain();
            }
            break;
        case EEventICFSelectionChanged:  // from ContextField (when whithout transitory input)
            if (iHiraganaKanji)
                {  // update saved data
                TRAP_IGNORE(iHiraganaKanji->HandleControlEventJpL(aEventType,aEventData));
                }
            break;
        case EEventSetAppCursorSelection:  // from ContextField
            if (UiStateMgr() && UiStateMgr()->CurrentUiState() == UiStateMgr()->GetUiState(EPeninputVkbUiStateWithTransitoryChars))
                {
                TRAP_IGNORE(iHiraganaKanji->HandleControlEventJpL(aEventType,aEventData));
                }
            break;
        default:
            CPeninputCommonLayout::HandleControlEvent(aEventType, aCtrl, aEventData);
            break;
        }
    }

// ---------------------------------------------------------------------------
// CPeninputVkbLayout::OnAppEditorTextComing
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CPeninputVkbLayout::OnAppEditorTextComing(const TFepInputContextFieldData& aData)
    {
    iWaitforEditorTextComing = 0;
    // Handle edit text coming
    if (iHiraganaKanji)
        {
        TRAPD(err, iHiraganaKanji->SetEditorTextL(aData));
        if (KErrNone != err)
            {
            HandleControlEvent(EPeninputLayoutEventClose, NULL, KNullDesC);
            }
        return err;
        }
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CPeninputVkbLayout::HandleRawKeyEventL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool CPeninputVkbLayout::HandleRawKeyEventL(const TRawEvent& aKeyEvent)
    {
    TBool rs = EFalse;
    if (iHiraganaKanji)
        {
        iHiraganaKanji->CancelRepeat();
        }
    if (UiStateMgr() && UiStateMgr()->CurrentUiState())
        {
        rs = UiStateMgr()->CurrentUiState()->HandleKeyEventL(aKeyEvent);
        if (rs && iHiraganaKanji)
            {
            if (EStdKeyEnter == (aKeyEvent.ScanCode() & 0x000000FF) && IsPredictiveWithNoChar())
                {    // without transitory chars and next word prediction candidates are offered
                     // Enter button should clear Prediction and enter a newline
                TRAP_IGNORE(iHiraganaKanji->HandleControlEventJpL(EPeninputLayoutEventBack,KNullDesC));
                return UiStateMgr()->GetUiState(EPeninputVkbUiStateStandby)->HandleKeyEventL(aKeyEvent);
                }
            rs = iHiraganaKanji->HandleKeyEvent(aKeyEvent);
            }
        }
    if (!iWaitforEditorTextComing && TRawEvent::EKeyDown == aKeyEvent.Type() && TRawEvent::ENone == iKeyEvent.Type())
        {
        if (!rs)  // state == standby or state == standbyjp or key that hiraganakanji do not handle.
            {
            HiraganaKanjiCompleteAll();
            if (iWaitforEditorTextComing)  // complete done.
                {
                rs = ETrue;
                iKeyEvent = aKeyEvent;
                iKeyEventAsync->CallBack();       // for key that hiraganakanji do not handle
                }
            }
        else if (iHiraganaKanji)
            {
            iKeyEventAsync->CallBack();   // for pending event
            }
        }
    return rs;
    }

// ---------------------------------------------------------------------------
// CPeninputVkbLayout::OnPointerEventOutsideLayout
// Call back when the pointer down/up event happends outside of the layout
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool CPeninputVkbLayout::OnPointerEventOutsideLayout(const TRawEvent& aEvent)
    {
    if (TRawEvent::EButton1Down == aEvent.Type()) // this if is not necessary. only EButton1Down.
        {
        HiraganaKanjiCompleteAll();
        if (iCapturePointerEvent)
            {
            iPointerEventDown = aEvent;
            SetCapturePointerEvent(EFalse);
            iPointerEventAsync->CallBack();
            }
        }
    return EFalse;
    }

// ---------------------------------------------------------------------------
// CPeninputVkbLayout::DoSetCurrentRange
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputVkbLayout::DoSetCurrentRange()
    {
    TInt currentRange = CPeninputDataConverter::AnyToInt( RequestData( EPeninputDataTypeCurrentRange ) );

    static_cast<CPeninputVkbDataMgr*>(iDataMgr)->SetEngineInputMode(currentRange); // PtiEngine:SetInputMode()

    if (iVkbWindow)
        {
        iVkbWindow->DoRangeChange();  // RangeBar
        }

    if ( currentRange == ERangeHiraganaKanji )
        {
        UiStateMgr()->SetCurrentUiState(EPeninputVkbUiStateStandbyjp);
        }
    else
        {
        UiStateMgr()->SetCurrentUiState(EPeninputVkbUiStateStandby);
        }
   SetCapturePointerEvent(EFalse);
   }

// ---------------------------------------------------------------------------
// CPeninputVkbLayout::CPeninputVkbLayout
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPeninputVkbLayout::CPeninputVkbLayout(MLayoutOwner* aLayoutOwner)
    : CPeninputCommonLayout( aLayoutOwner ),iCapturePointerEvent(EFalse)
    {
    }

// ---------------------------------------------------------------------------
// CPeninputVkbLayout::ConstructL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputVkbLayout::ConstructL(const TAny* aInitData)
    {
    CPeninputCommonLayout::ConstructL( aInitData );
    CPeninputUiStateBase* initialState =
        CPeninputUiStateBase::NewL( UiStateMgr(), this );
    UiStateMgr()->AddUiState( initialState, EPeninputVkbUiStateNone );

    CPeninputVkbUiStateStandby* standbyState =
        CPeninputVkbUiStateStandby::NewL( UiStateMgr(), this );
    UiStateMgr()->AddUiState( standbyState, EPeninputVkbUiStateStandby );

    CPeninputVkbUiStateStandbyjp* standbyjpState =
        CPeninputVkbUiStateStandbyjp::NewL( UiStateMgr(), this );
    UiStateMgr()->AddUiState( standbyjpState, EPeninputVkbUiStateStandbyjp );

    CPeninputVkbUiStateWithTransitoryChars* transitoryState =
        CPeninputVkbUiStateWithTransitoryChars::NewL( UiStateMgr(), this );
    UiStateMgr()->AddUiState( transitoryState, EPeninputVkbUiStateWithTransitoryChars );

    UiStateMgr()->SetCurrentUiState( initialState );
    iVkbWindow = static_cast<CPeninputVkbWindow*>(iLayoutWindow);
    //create hiraganakanji
    iHiraganaKanji = CPeninputHiraganaKanji::NewL(static_cast<CPeninputVkbDataMgr*>(iDataMgr)->PtiEngine()
                                                       , this
                                                       , iVkbWindow->InputContextField()
                                                       , iVkbWindow->CandidateWindow()
                                                       , iVkbWindow->PredictiveWindow());
    TCallBack cb(KeyEventCallback, this);
    TCallBack cbpointer(PointerEventCallback, this);
    iKeyEventAsync = new(ELeave) CAsyncCallBack(cb, CActive::EPriorityStandard);
    iPointerEventAsync = new(ELeave) CAsyncCallBack(cbpointer, CActive::EPriorityStandard);
    iKeyEvent.Set(TRawEvent::ENone,0);
    iPointerEventDown.Set(TRawEvent::ENone,0);
    }

// ---------------------------------------------------------------------------
// CPeninputVkbLayout::CompleteAllHiraganaKanji
// ---------------------------------------------------------------------------
//
void CPeninputVkbLayout::HiraganaKanjiCompleteAll()
    {
    if( UiStateMgr() && UiStateMgr()->CurrentUiState() == UiStateMgr()->GetUiState(EPeninputVkbUiStateWithTransitoryChars))
        {
        if (iHiraganaKanji)
            {  // CompleteALL
            TRAP_IGNORE(iHiraganaKanji->HandleControlEventJpL(EEventCompleteAllHiraganaKanji, KNullDesC));
            }
        }
    }

// ---------------------------------------------------------------------------
// CPeninputGenericVkbLayout::CreateLayoutWindowL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputVkbLayout::CreateLayoutWindowL()
    {
    iLayoutWindow = CPeninputVkbWindow::NewL( this, this );
    }

// ---------------------------------------------------------------------------
// CPeninputGenericVkbLayout::CreateDataMgrL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputVkbLayout::CreateDataMgrL( const TAny* /*aInitData*/ )
    {
    iDataMgr = CPeninputVkbDataMgr::NewL( this );
    }

// ---------------------------------------------------------------------------
// CPeninputVkbLayout::LayoutType
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TInt CPeninputVkbLayout::LayoutType()
    {
    return EPluginInputModeVkb;
    }


// ---------------------------------------------------------------------------
// CPeninputVkbLayout::TranslateRangeHullHalf
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TInt CPeninputVkbLayout::TranslateRangeHullHalf(TInt aRange) const
    {
    TInt newRange = aRange;
    if (static_cast<CPeninputVkbDataMgr*>(iDataMgr)->FullHalf()) // Full
        {
        switch (aRange)
            {
            case ERangeEnglish:
                newRange = ERangeFullWidthEnglish;
                break;
            case ERangeNumber:
                 newRange = ERangeFullWidthNumeric;
                 break;
            case ERangeKatakana:
                newRange = ERangeFullWidthKatakana;
                break;
            default:
                break;
            }
        }
    else   // half
        {
        switch (aRange)
            {
            case ERangeFullWidthEnglish:
                newRange = ERangeEnglish;
                break;
            case ERangeFullWidthNumeric:
                newRange = ERangeNumber;
                break;
            case ERangeFullWidthKatakana:
                newRange = ERangeKatakana;
                break;
            default:
                break;
            }
        }
    return newRange;
    }

TInt CPeninputVkbLayout::KeyEventCallback(TAny* aThis)
    {
    if (aThis)
        {
        TRAPD(err,static_cast<CPeninputVkbLayout*>(aThis)->DoKeyEventL());
        if (KErrNone != err)
            {
            static_cast<CPeninputVkbLayout*>(aThis)->HandleControlEvent(EPeninputLayoutEventClose, NULL, KNullDesC);
            }
        return err;
        }
    return 0;
    }

TInt CPeninputVkbLayout::PointerEventCallback(TAny* aThis)
    {
    if (aThis)
        {
        static_cast<CPeninputVkbLayout*>(aThis)->DoPointerEvent();
        }
    return 0;
    }

void CPeninputVkbLayout::DoKeyEventL()
    {
    TBool ret = iHiraganaKanji->HandlePendingEventL();
    if (!ret && TRawEvent::ENone != iKeyEvent.Type())
        {
        iKeyEvent.Set(TRawEvent::EKeyUp,iKeyEvent.ScanCode());
        CCoeEnv::Static()->WsSession().SimulateRawEvent(iKeyEvent);
        iKeyEvent.Set(TRawEvent::EKeyDown,iKeyEvent.ScanCode());
        CCoeEnv::Static()->WsSession().SimulateRawEvent(iKeyEvent);
        iKeyEvent.Set(TRawEvent::EKeyUp,iKeyEvent.ScanCode());
        CCoeEnv::Static()->WsSession().SimulateRawEvent(iKeyEvent);
        iKeyEvent.Set(TRawEvent::ENone,0);
        }
    }

void CPeninputVkbLayout::DoPointerEvent()
    {
    if (TRawEvent::ENone != iPointerEventDown.Type())
        {
        User::After(KDelay);
        CCoeEnv::Static()->WsSession().SimulateRawEvent(iPointerEventDown);
        iPointerEventDown.Set(TRawEvent::EButton1Up,iPointerEventDown.Pos().iX,iPointerEventDown.Pos().iY);
        CCoeEnv::Static()->WsSession().SimulateRawEvent(iPointerEventDown);
        iPointerEventDown.Set(TRawEvent::ENone,0);
        }
    }

void CPeninputVkbLayout::SetCapturePointerEvent(TBool aFlag)
    {
    if (iCapturePointerEvent && !aFlag || !iCapturePointerEvent && aFlag)
        {
        //signal the owner that pointer needs not be captured
        TBool capture = aFlag;
        TPtrC captureData;
        captureData.Set((const TUint16*)&capture,sizeof(TBool)/sizeof(TUint16));
        SignalOwner(ESignalCapturePointer,captureData);
        iCapturePointerEvent = aFlag;
        }
    }

void CPeninputVkbLayout::SetCapturePointerEventAgain()
    {
    if (iCapturePointerEvent)
        {
        //signal the owner that pointer needs not be captured
        TBool capture = ETrue;
        TPtrC captureData;
        captureData.Set((const TUint16*)&capture,sizeof(TBool)/sizeof(TUint16));
        SignalOwner(ESignalCapturePointer,captureData);
        }
    }

void CPeninputVkbLayout::HiraganaHandleControlEventJp(TInt aEventType, const TDesC& aEventData)
    {
    if (iHiraganaKanji)
        {   // close peninput
        TRAPD(err, iHiraganaKanji->HandleControlEventJpL(aEventType,aEventData));
        if (KErrNone != err)
            {
            HiraganaKanjiCompleteAll();
            SetCapturePointerEvent(EFalse);
            CPeninputCommonLayout::HandleControlEvent(EPeninputLayoutEventClose, NULL, KNullDesC);
            }
        }
    }

TBool CPeninputVkbLayout::IsPredictiveWithNoChar() const
    {
    if (!iCapturePointerEvent    // without transitory chars
    && UiStateMgr()->CurrentUiState() == UiStateMgr()->GetUiState(EPeninputVkbUiStateWithTransitoryChars))
        {
        return ETrue;
        }
    return EFalse;
    }
// End Of File
