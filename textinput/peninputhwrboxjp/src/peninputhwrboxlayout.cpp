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
* Description:  Implementation of the hwr layout
*
*/


// system includes
#include <peninputhiraganakanji.h>       // hiraganakanji
#include <peninputuistatemgr.h>    // use ui state mgr
#include <peninputdataconverter.h>
#include <aknfeppeninputenums.h>    //command from fep or IME to plugin. for vkb/hwr, range defination
#include <peninputcommonlayoutglobalenum.h>  // EPeninputLayoutEventEnter
#include <peninputeventjp.h>
#include <AknFepGlobalEnums.h> //define TWidthChar
#include <PtiEngine.h>
#include <peninputjapanesecandidatewnd.h>
#include <peninputjapanesepredictivewnd.h>

// user includes
#include "peninputhwrboxlayout.h"
#include "peninputhwrboxuistatetype.h"         // UI state
#include "peninputhwrboxwindow.h"        // use hwr window, which derived from base window
#include "peninputhwrboxdatamgr.h"       // use data mgr
#include "peninputhwrboxuistatestandby.h"
#include "peninputhwrboxuistatestandbyjp.h"
#include "peninputhwrboxuistatewithtransitorychars.h"
#include "peninputhwrboxuistatepenstandby.h"
#include "peninputhwrboxuistatepenbeginwriting.h"
#include "peninputhwrboxuistatepenendwriting.h"
#include "peninputhwrboxuistatepencandidateselecting.h"
#include "peninputhwrevent.h"
#include "peninputhwrboxgroup.h"

#ifdef __PENINPUT_HWR_STROKE_LOG_ON
#include <PeninputHwrStrokeLogConst.h>
#endif // __PENINPUT_HWR_STROKE_LOG_ON

// constant definition
const TInt KMaxCandidateLen = 128;
const TUint16 KBackRep = 0x2408;
const TUint16 KEnterRep = 0xF801;
const TUint16 KSpaceRep = 0xF800;
const TInt KDelay = 100000; //1/10 second
const TInt KPeninputBufLen = 16;
const TInt KPeninputBufLen8 = 8;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPeninputHwrBoxLayout::NewL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPeninputHwrBoxLayout* CPeninputHwrBoxLayout::NewL(MLayoutOwner* aLayoutOwner,
                                         const TAny* aInitData)
    {
    CPeninputHwrBoxLayout* self = new (ELeave) CPeninputHwrBoxLayout(aLayoutOwner);

    CleanupStack::PushL(self);
    self->ConstructL(aInitData);
    CleanupStack::Pop(self);

    return self;
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxLayout::~CPeninputHwrBoxLayout
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPeninputHwrBoxLayout::~CPeninputHwrBoxLayout()
    {
    delete iHiraganaKanji;
    delete iKeyEventAsync;
    delete iKeyEventClearAsync;
    delete iPointerEventAsync;
    delete iUiStatePenMgr;
    delete iSubmitText;
#ifdef __PENINPUT_HWR_STROKE_LOG_ON
    if (iLogger.LogValid())
        {
        iLogger.CloseLog();
        }
    iLogger.Close();
#endif // __PENINPUT_HWR_STROKE_LOG_ON
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxLayout::HandleEventL
// handle event from window server
// ---------------------------------------------------------------------------
//
TBool CPeninputHwrBoxLayout::HandleEventL(TEventType aType, const TAny* aData)
    {
    TRawEvent event=*((TRawEvent*)aData);
    CFepUiBaseCtrl* ctrl = NULL;
    if(ERawEvent == aType)
        {
        if (TRawEvent::EButton1Down == event.Type() && iHiraganaKanji)
            {
            iHiraganaKanji->CancelRepeat();
            }
        // DropDownList expanded and PointerDown on drawing area ---> cancel input and to StandBy
        if (iExpanded
            && TRawEvent::EButton1Down == event.Type()
            && iUiStatePenMgr->CurrentUiState() == iUiStatePenMgr->GetUiState(EPeninputHwrUiStatePenCandidateSelecting)
            && iDropdownList
            && iHwrWindow->BoxGroup())
            {
            TPoint point = event.Pos() - Position ();
            if (!iDropdownList->Rect().Contains(point)
                && iHwrWindow->BoxGroup()->Rect().Contains(point))
                {
                ClearInputToPenStandByL();
                return ETrue;
                }
            }
        if(TRawEvent::EButton1Up == event.Type())
            {
            ctrl = RootControl()->CtrlCapPointer();
            }
        }
    TBool handled = CPeninputCommonLayout::HandleEventL(aType,aData);
    // if dropdownlist expanded, capture off
    // as the dropdownlist is the 1st control in the clientpane controlgroup, capture is not need.
    if (TRawEvent::EButton1Up == event.Type() && !ctrl)
        {
        CFepUiBaseCtrl* capture = RootControl()->CtrlCapPointer();
        if (capture && capture->ControlId() == EPeninutWindowCtrlIdDropDownList)
           {  // dropdownlist expanded
           iDropdownList = capture;
           capture->CapturePointer(EFalse);
           SetCapturePointerEventAgain(); // capture for transitory input on
           HandleControlEvent(EPeninputLayoutHwrEventDropDownListExpanded, capture, KNullDesC);
           }
        }
    // if a control do capture off, capture for transitory input on.
    if (ctrl && !RootControl()->CtrlCapPointer())
        {
        SetCapturePointerEventAgain();
        }
    return handled;
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxLayout::HandleCommand
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CPeninputHwrBoxLayout::HandleCommand(TInt aCmd, TUint8* aData)
    {
    // Handle command from fep or IME to this layout
    TUint* data = (TUint*) aData;

    switch ( aCmd )
        {
        case ECmdPenInputPermittedRange:  // different from common layout
            // this command comes after ECmdPenInputJapaneseSetting
            if (iDataMgr)
                {
                // in SetData, SetPermittedRanges() and SetCharacterRangePopupLare() called.
                SetData(EPeninputDataTypePermittedRange, aData);
                }
            break;
        case ECmdPenInputLanguage:
            {
            CPeninputCommonLayout::HandleCommand(aCmd, aData);
            }
            break;
        case ECmdPenInputWindowOpen:
            if (iHwrWindow)
                {
                iHwrWindow->Opened();
                iLayoutWindow->Hide(EFalse);  // before Hide Check or Change, parent controlgroup should be Hide Off.
                TInt curRange = CPeninputDataConverter::AnyToInt(RequestData(EPeninputDataTypeCurrentRange));
                TBool currHide = iHwrWindow->PredictiveWindow()->Hiden();
                TBool newHide = ETrue;
                if (static_cast<CPeninputHwrBoxDataMgr*>(iDataMgr)->Predictive()
                              && static_cast<CPeninputHwrBoxDataMgr*>(iDataMgr)->Conversion()
                              &&  ERangeHiraganaKanji == curRange)
                    {
                    newHide = EFalse;
                    }
                if (newHide && !currHide || !newHide && currHide)
                    {
                    iHwrWindow->PredictiveWindow()->Hide(newHide);
                    iHwrWindow->SizeChanged(ETrue);
                    }
                CPeninputCommonLayout::HandleCommand( aCmd, aData );
                if (*data != EPenInputOpenManually && Position() == TPoint(0,0))
                    {  // position is not yet set.(English --> Japanese)
                    SendEventToWindow(EPeninputLayoutEventMovePosition, NULL, KNullDesC);
                    }
                iHwrWindow->ICFEnableRegionUpdating();  // ICF::OnActivate()
                }
            ToPenStandBy();
#ifdef __PENINPUT_HWR_STROKE_LOG_ON
    if (iLogger.Handle())
        {
        if (iLogger.LogValid())
            {
            iLogger.CloseLog();
            }
        // Generate file name based on date.
        TTime time;
        time.HomeTime();
        TDateTime datetime = time.DateTime();
        TBuf<KHwrStrokeLogFileNameMaxLen> name;
        name.Format(KHwrStrokeLogFileExt(),
            datetime.Year(), TInt(datetime.Month()+1), datetime.Day()+1,
            datetime.Hour(), datetime.Minute(), datetime.Second());
        // Open(or create) log file
        iLogger.CreateLog( KHwrStrokeLogDir(), name, EFileLoggingModeAppend);
        // Disable date and time
        iLogger.SetDateAndTime(EFalse, EFalse);
        // Write log
        iLogger.Write(KHwrStrokeLogOpen());
        WriteHwrStrokeLogRange();
        }
#endif // __PENINPUT_HWR_STROKE_LOG_ON
            break;
        case ECmdPenInputWindowClose:
            SendEventToWindow(EPeninputLayoutEventCloseCharacterRange, NULL, KNullDesC);
            HiraganaKanjiCompleteAll();
            CPeninputCommonLayout::HandleCommand( aCmd, aData );
            UiStateMgr()->SetCurrentUiState( EPeninputHwrUiStateNone );
            Initialize();
            SetCapturePointerEvent(EFalse);
            iHwrWindow->Closed();
#ifdef __PENINPUT_HWR_STROKE_LOG_ON
    if (iLogger.LogValid())
        {
        iLogger.Write(KHwrStrokeLogClose());
        iLogger.CloseLog();
        }
#endif // __PENINPUT_HWR_STROKE_LOG_ON
            break;
        case ECmdPenInputRange:  // different from common layout:control. ptiEngine.
            {
#ifdef __PENINPUT_HWR_STROKE_LOG_ON
    iLogOff = 1;
#endif
            // this command comes after ECmdPenInputJapaneseSetting, ECmdPenInputPermittedRange
            if (iLayoutWindow)
                {
                iHwrWindow->ICFDisableRegionUpdating();  // ICF::OnDeActivate()
                iLayoutWindow->Hide(EFalse);  // before Hide Check or Change, parent controlgroup should be Hide Off.
                }
            if (iDataMgr)
                {
                TUint range = TranslateRangeHullHalf(*data);
                TUint8* rangedata = reinterpret_cast<TUint8*>(&range);
                // SetData(SetPremaryRange) and SizeChangedForBaseWindow and ChangeClientLayout
                CPeninputCommonLayout::HandleCommand( aCmd, rangedata );
                iRangeOrg = range;
                TInt permittedRange = CPeninputDataConverter::AnyToInt(RequestData(EPeninputDataTypePermittedRange));
                if (permittedRange & (ERangeHiraganaKanji | ERangeKatakana | ERangeFullWidthKatakana))
                    {
                    TInt index = 0;
                    if (range == ERangeEnglish || range == ERangeFullWidthEnglish)
                        {
                        index = 2;  // see CPeninputHwrBoxDataMgr::SetCharacterRangePopupL()
                        }
                    if (range == ERangeNumber || range == ERangeFullWidthNumeric)
                        {
                        index = (permittedRange & (ERangeEnglish | ERangeFullWidthEnglish))? 3:2;
                                                 // see CPeninputHwrBoxDataMgr::SetCharacterRangePopupL()
                        }
                    if (index)
                        {
                        TPtrC dataPtr;
                        dataPtr.Set(reinterpret_cast<TUint16*>(&index),sizeof(TInt)/sizeof(TUint16));
                        HandleControlEvent(EPeninputLayoutEventRange,NULL,dataPtr);  // call DoSetCurrentRange()
                        }
                    else
                        {
                        DoSetCurrentRange();
                        }
                    }
                else
                    {
                    DoSetCurrentRange();
                    }
                }
            }
#ifdef __PENINPUT_HWR_STROKE_LOG_ON
    iLogOff = 0;
#endif
            break;
        case ECmdPenInputJapaneseSetting:  // Japanese setting information
            // this command comes before ECmdPenInputPermittedRange, ECmdPenInputRange
            if (iDataMgr)
                {
                iDataMgr->Reset();
                TUint val = *data;
                TInt sctDimmFlag = (val & EPenInputJapaneseSettingLunchSCT)? 0: 1;
                TPtr ptr((TUint16*)&sctDimmFlag, sizeof(TInt)/sizeof(TUint16));
                SendEventToWindow(EEventSetDimmedSctBtn, NULL, ptr);
                static_cast<CPeninputHwrBoxDataMgr*>(iDataMgr)->SetFullHalf(
                                (val & EPenInputJapaneseSettingCharacterWidth)? 1: 0);
                static_cast<CPeninputHwrBoxDataMgr*>(iDataMgr)->SetPredictive(
                                (val & EPenInputJapaneseSettingPredictive)? ETrue: EFalse);
                static_cast<CPeninputHwrBoxDataMgr*>(iDataMgr)->SetConversion(
                                (val & EPenInputJapaneseSettingConversion)? ETrue: EFalse);
                static_cast<CPeninputHwrBoxDataMgr*>(iDataMgr)->SetClear(
                                (val & EPenInputJapaneseSettingDeletingDirection)? 1: 0);
                }
            break;
        case ECmdPenInputEnableSettingBtn:
            iHwrWindow->SetEnableSettingBtnJp(*data);
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
// CPeninputHwrBoxLayout::HandleControlEvent
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputHwrBoxLayout::HandleControlEvent(TInt aEventType, CFepUiBaseCtrl* aCtrl, const TDesC& aEventData)
    {
    // Handle event from control
    TInt* data = (TInt*)aEventData.Ptr();
    TBool handled = EFalse;

    if (EEventDraggingEnd == aEventType)
        {
        CPeninputCommonLayout::HandleControlEvent(aEventType, aCtrl, aEventData);
        }
#ifdef __PENINPUT_HWR_STROKE_LOG_ON
    if (EPeninputLayoutEventBack == aEventType)
        {
        WriteHwrStrokeLogClear();
        }
    if (EPeninputLayoutHwrEventCandidateSelected == aEventType && iLogger.LogValid())
        {
        if (aEventData.Length() > 1)
            {
                    //get the candidate index
            const TInt cellNo = aEventData[aEventData.Length()-1] + 1;
            iLogger.WriteFormat(KHwrStrokeLogSelected(), cellNo);
            }
        }
#endif // __PENINPUT_HWR_STROKE_LOG_ON

    if (iUiStatePenMgr != NULL)
        {
        handled = HandleControlEventPen(aEventType, aCtrl, aEventData);
        }
    if (handled)
        {
        return;
        }

    //deal with the event from all controls in layout
    switch (aEventType)
        {
        case EPeninputLayoutEventClose:
        case EPeninputLayoutEventOption:
        case EPeninputLayoutEventToVkb:
            HiraganaKanjiCompleteAll();  // call ToPenStandBy()
            SetCapturePointerEvent(EFalse);
            if (EPeninputLayoutEventClose == aEventType)
                {
                Initialize();  //jump to init state
                }
            if (EPeninputLayoutEventToVkb == aEventType)
                {
                TBuf<KPeninputBufLen8> buf;
                TBool switchByMode = ETrue;
                TInt mode = EPluginInputModeVkb;
                CPeninputDataConverter::CombinationToDesc( switchByMode, 
                                                                 mode, buf );
                SignalOwner( ESignalLayoutUIChanged, buf );
                }
            else
                {
                CPeninputCommonLayout::HandleControlEvent(aEventType, aCtrl, aEventData);
                }
            break;
        case EPeninputLayoutEventSct:
            HiraganaKanjiCompleteAll();
            SetCapturePointerEvent(EFalse);
            SignalOwner(ESignalHwNotifySctPopup);
            break;
        case EPeninputLayoutEventSwitchLanguage: // nop
            break;

        case EPeninputLayoutEventRange:  // from character range popup or when accept ECmdPenInputRange command
            {
            TInt permittedRange = CPeninputDataConverter::AnyToInt(RequestData(EPeninputDataTypePermittedRange));
            TInt currentRange = CPeninputDataConverter::AnyToInt( RequestData( EPeninputDataTypeCurrentRange ) );
            TInt range = currentRange;
            TInt deActive = 0;   // 0:none 1:ICF 2:display off
            static_cast<CPeninputHwrBoxDataMgr*>(iDataMgr)->SetCurrentCharacterRangeIndex(*data);
            switch(static_cast<CPeninputHwrBoxDataMgr*>(iDataMgr)->CharacterRangeId())
                {
                case EPeninutCharacterRangeIdAll:
                    if (permittedRange & (ERangeHiraganaKanji | ERangeKatakana | ERangeFullWidthKatakana))
                        {
                        if (!(range & (ERangeHiraganaKanji | ERangeKatakana | ERangeFullWidthKatakana)))
                            {
                            range = (permittedRange & ERangeHiraganaKanji)? ERangeHiraganaKanji : ERangeKatakana;
                            }
                        }
                    break;
                case EPeninutCharacterRangeIdKanaKanji:
                    permittedRange &= (ERangeHiraganaKanji | ERangeKatakana | ERangeFullWidthKatakana);
                    if (!(permittedRange & range))
                        {
                        range = (permittedRange & ERangeHiraganaKanji)? ERangeHiraganaKanji : ERangeKatakana;
                        }
                    break;
                case EPeninutCharacterRangeIdLatin:
                    permittedRange &= (ERangeEnglish | ERangeFullWidthEnglish);
                    range = ERangeEnglish;
                    break;
                case EPeninutCharacterRangeIdNumber:
                    permittedRange &= (ERangeNumber | ERangeFullWidthNumeric);
                    range = ERangeNumber;
                    break;
                default:
                    break;
                }
            static_cast<CPeninputHwrBoxDataMgr*>(iDataMgr)->SetPermittedRanges(permittedRange);
            range = TranslateRangeHullHalf(range);
            if (!(range & permittedRange))
                {
                range = iRangeOrg;
                }
            if (aCtrl)  // from character range popup
                {
                TBool currHide = iHwrWindow->PredictiveWindow()->Hiden();
                TBool newHide = ETrue;
                if (static_cast<CPeninputHwrBoxDataMgr*>(iDataMgr)->Predictive()
                                  && static_cast<CPeninputHwrBoxDataMgr*>(iDataMgr)->Conversion()
                                  &&  ERangeHiraganaKanji == range)
                    {
                    newHide = EFalse;
                    }
                if (newHide && !currHide || !newHide && currHide)
                    {
                    LayoutOwner()->Hide(ETrue);   // display off
                    deActive = 2;
                    iHwrWindow->PredictiveWindow()->Hide(newHide);
                    iHwrWindow->SizeChanged(ETrue);
                    }
                else
                    {
                    iHwrWindow->ICFDisableRegionUpdating();  // ICF::OnDeActivate()
                    deActive = 1;
                    }
                iHwrWindow->CharacterRangeWindow()->HidePopup();
                }
            if (aCtrl && range != currentRange)  // from character range popup
                {
                // SetData(SetPremaryRange) and Sendkey( ESignalRange,...)
                TPtrC rangeptr((TUint16*)(&range), sizeof(TInt)/sizeof(TUint16));
                CPeninputCommonLayout::HandleControlEvent(aEventType, aCtrl, rangeptr);
                if (ERangeEnglish != range && ERangeNumber != range)
                    {
                    TInt native = ERangeNative;
                    TBuf<KPeninputBufLen> buf;
                    CPeninputDataConverter::CombinationToDesc( native, range, buf );
                    Sendkey( ESignalRange, buf );
                    }
                }
            else
                {
                SetData(EPeninputDataTypeCurrentRange, &range);
                }
            DoSetCurrentRange();
            if (deActive == 2)
                {
                LayoutOwner()->Hide(EFalse);   // display on
                }
            else if (deActive == 1)
                {
                iHwrWindow->ICFEnableRegionUpdating();  // ICF Cursor On
                }
            }
            break;

        // from HiraganaKanji
        case EEventHiraganaKanjiWithTransitoryChars2Standbyjp:
            if (UiStateMgr() && UiStateMgr()->CurrentUiState() == UiStateMgr()->GetUiState(EPeninputHwrUiStateWithTransitoryChars))
                {
                UiStateMgr()->SetCurrentUiState(EPeninputHwrUiStateStandbyjp);
                SetCapturePointerEvent(EFalse);
                ToPenStandBy();
                }
            break;
        // from HiraganaKanji
        case EEventHiraganaKanjiWithoutTransitoryChars:
            if (UiStateMgr() && UiStateMgr()->CurrentUiState() == UiStateMgr()->GetUiState(EPeninputHwrUiStateWithTransitoryChars))
                {
                SetCapturePointerEvent(EFalse);
                }
            break;

        case EEventVirtualKeyUp:
            if ( UiStateMgr()->CurrentUiState()
            && !static_cast<CPeninputHwrBoxUiState*>(UiStateMgr()->CurrentUiState())->HandleControlEvent(aEventType,aEventData) )
                {
                SetCapturePointerEvent(ETrue);
                HiraganaHandleControlEventJp(aEventType, aEventData);
                }
            break;
        case EPeninputLayoutEventBack:
            {
            TInt clear = EKeyBackspace;
            TPtrC clearptr((TUint16*)(&clear), 1);
            if ( UiStateMgr()->CurrentUiState()
            && !static_cast<CPeninputHwrBoxUiState*>(UiStateMgr()->CurrentUiState())->HandleControlEvent(aEventType,clearptr) )
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
            && !static_cast<CPeninputHwrBoxUiState*>(UiStateMgr()->CurrentUiState())->HandleControlEvent(aEventType,aEventData) )
                {
                TRAP_IGNORE(iHiraganaKanji->HandleControlEventJpL(aEventType,aEventData));
                }
            break;
        case EPeninputLayoutEventSpace:   // convert
            if ( UiStateMgr()->CurrentUiState()
            && !static_cast<CPeninputHwrBoxUiState*>(UiStateMgr()->CurrentUiState())->HandleControlEvent(aEventType, aEventData) )
                {
                HiraganaHandleControlEventJp(aEventType, aEventData);
                }
            break;
        case EEventIcfPointerUp:  // from ContextField
        case EEventIcfCompleteAll:  // from ContextField
            TRAP_IGNORE(iHiraganaKanji->HandleControlEventJpL(aEventType,aEventData));
            break;
        case EEventShowCandidateWnd:   // CandidateWnd On
            {
            iHwrWindow->SetDimmed(ETrue);
            }
            break;
        case EEventHideCandidateWnd:   // CandidateWnd Off
            {
            iHwrWindow->SetDimmed(EFalse);
            }
            break;
        case EEventWaitforEditorTextComing:   // may use, someday.
            {
            iWaitforEditorTextComing = 1;
            }
            break;
        case EEventDraggingEnd:
            break;
        case EPeninputLayoutEventSwitchCharacterRange:
            if (static_cast<CPeninputHwrBoxDataMgr*>(iDataMgr)->CharacterRangeArray()->Count() > 1)
                {
                HiraganaKanjiCompleteAll();
                SendEventToWindow(aEventType, aCtrl, aEventData);
                }
            break;
        case EEventICFSelectionChanged:  // from ContextField (when whithout transitory input)
            if (iHiraganaKanji)
                {  // update saved data
                TRAP_IGNORE(iHiraganaKanji->HandleControlEventJpL(aEventType,aEventData));
                }
            break;
        case EEventSetAppCursorSelection:  // from ContextField
            if (UiStateMgr() && UiStateMgr()->CurrentUiState() == UiStateMgr()->GetUiState(EPeninputHwrUiStateWithTransitoryChars))
                {
                TRAP_IGNORE(iHiraganaKanji->HandleControlEventJpL(aEventType,aEventData));
                }
            break;
        case EPeninputLayoutHwrEventDropDownListExpanded:
#ifdef __PENINPUT_HWR_STROKE_LOG_ON
    if (iLogger.LogValid())
        {
        iLogger.Write(KHwrStrokeLogExpand());
        }
#endif // __PENINPUT_HWR_STROKE_LOG_ON
            CPeninputCommonLayout::HandleControlEvent(aEventType, aCtrl, aEventData);
            iExpanded = ETrue;
            break;
        default:
            CPeninputCommonLayout::HandleControlEvent(aEventType, aCtrl, aEventData);
            break;
        }
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxLayout::HandleControlEventPen
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool CPeninputHwrBoxLayout::HandleControlEventPen(TInt aEventType, CFepUiBaseCtrl* /*aCtrl*/, const TDesC& aEventData)
    {
    TBool handled = EFalse;
    CPeninputHwrBoxUiState* current = static_cast<CPeninputHwrBoxUiState*>(iUiStatePenMgr->CurrentUiState());
    TBuf<KMaxCandidateLen> charCode;
    //filter un-related events
    if (aEventType != EEventSetAppCursorSelection
        && aEventType != EPeninputLayoutEventClose
        && aEventType != EPeninputLayoutEventBack
        && aEventType != EPeninputLayoutEventEnter
        && (aEventType != EPeninputLayoutEventSpace || !iCapturePointerEvent)
        && aEventType != EPeninputLayoutEventRange
        && aEventType != EPeninputLayoutEventOption
        && aEventType != EPeninputLayoutEventSct
        && aEventType != EPeninputLayoutEventSwitchCharacterRange
        && aEventType != EEventDraggingEnd
        && (aEventType >= EPeninputLayoutHwrEventLast || aEventType < EPeninputLayoutHwrEventBeginWriting))
        {
        return handled;
        }
    TRAPD(err, current->HandleControlEventPenL(aEventType, aEventData, *this, charCode, handled));
    if (KErrNone != err)
        {   // close peninput
        HiraganaKanjiCompleteAll();  // call ToPenStandBy()
        SetCapturePointerEvent(EFalse);
        Initialize();  //jump to init state
        CPeninputCommonLayout::HandleControlEvent(EPeninputLayoutEventClose, NULL, KNullDesC);
        handled = ETrue;
        }
    return handled;
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxLayout::OnAppEditorTextComing
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CPeninputHwrBoxLayout::OnAppEditorTextComing(const TFepInputContextFieldData& aData)
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

// ---------------------------------------------------------
// Submit the char code.
// ---------------------------------------------------------
//
void CPeninputHwrBoxLayout::SubmitL(const TDesC& aCharCode)
    {
    TInt len = 0;
    if (iSubmitText && !(*iSubmitText).CompareC(aCharCode)
        || !UiStateMgr()
        || !UiStateMgr()->CurrentUiState())
        {
        return;
        }
    if (iSubmitText)
        {
        len = iSubmitText->Length();
        if (len > 1 
            && UiStateMgr()->CurrentUiState() == UiStateMgr()->GetUiState(EPeninputHwrUiStateWithTransitoryChars))
            {
            len = 1;
            }
        }
    delete iSubmitText;
    iSubmitText = NULL;
    iSubmitText = aCharCode.AllocL();

    TInt eventType = EEventVirtualKeyUp;
    TPtr charcode(iSubmitText->Des());

    //if charcode is enter or back or space, translate them back
    TPtrC16 ptr;
    ptr.Set(&KBackRep, sizeof(KBackRep)/sizeof(TUint16));

    if (charcode.CompareC(ptr) == 0)
        {
        charcode.Zero();
        charcode.Append((TChar)EKeyBackspace);
        eventType = EPeninputLayoutEventBack;
        }
    else
        {
        ptr.Set(&KSpaceRep, sizeof(KSpaceRep)/sizeof(TUint16));
        if (charcode.CompareC(ptr) == 0)
            {
            charcode.Zero();
            charcode.Append((TChar)EKeySpace);
            eventType = EPeninputLayoutEventSpace;
            }
        else
            {
            ptr.Set(&KEnterRep, sizeof(KEnterRep)/sizeof(TUint16));
            if (charcode.CompareC(ptr) == 0)
                {
                charcode.Zero();
                charcode.Append((TChar)EKeyEnter);
                eventType = EPeninputLayoutEventEnter;
                }
            }
        }
    if (len > 0 && 
        (EEventVirtualKeyUp != eventType
                  || UiStateMgr()->CurrentUiState() != UiStateMgr()->GetUiState(EPeninputHwrUiStateStandby)))
        {
        for (TInt i = 0; i < len; i++)
            {
            DeleteL(EKeyDelete);
            }
        }

    if (len > 0 && 
        (EEventVirtualKeyUp == eventType
                  && UiStateMgr()->CurrentUiState() == UiStateMgr()->GetUiState(EPeninputHwrUiStateStandby)))
        {
        // len == 1 or 2, charcode.Length() == 1 or 2.
        TInt diff = charcode.Length() - len;
        if (diff > 0)
            {
            HBufC* p = HBufC::NewL(charcode.Length() + 2);
            p->Des().Append((TUint16*)&len, 2);
            p->Des().Append(charcode.Left(len));
            Sendkey( ESignalReplaceTextJp, *p );
            delete p;
            Sendkey( ESignalKeyEvent, charcode.Right(diff));
            }
        else
            {
            while (len > charcode.Length())
                {
                DeleteL(EKeyDelete);
                --len;
                }
            HBufC* p = HBufC::NewL(charcode.Length() + 2);
            p->Des().Append((TUint16*)&len, 2);
            p->Des().Append(charcode);
            Sendkey( ESignalReplaceTextJp, *p );
            delete p;
            }
        }
    else if ( !static_cast<CPeninputHwrBoxUiState*>(UiStateMgr()->CurrentUiState())->HandleControlEvent(eventType,charcode) )
        {
        SetCapturePointerEvent(ETrue);
        iHiraganaKanji->HandleControlEventJpL(eventType, charcode);
        }
    if (EEventVirtualKeyUp != eventType)
        {
        delete iSubmitText;
        iSubmitText = NULL;
        }
    }

// ---------------------------------------------------------
// Delete the previous charcode
// ---------------------------------------------------------
//
void CPeninputHwrBoxLayout::DeleteL(TInt aChar/*  = EKeyBackspace*/)
    {
        //delete the last char
    const TInt len = sizeof(aChar);
    TBuf<len> aBufBck;
    aBufBck.Append((TChar)aChar);

    if (UiStateMgr() && UiStateMgr()->CurrentUiState() == UiStateMgr()->GetUiState(EPeninputHwrUiStateWithTransitoryChars))
        {
        static_cast<CPeninputHwrBoxDataMgr*>(iDataMgr)->PtiEngine()->HandleCommandL(EPtiCommandBackSpaceByForce);
        }
    else
        {
        SignalOwner(ESignalKeyEvent, aBufBck);
        }
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxLayout::HandleRawKeyEventL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool CPeninputHwrBoxLayout::HandleRawKeyEventL(const TRawEvent& aKeyEvent)
    {
    TBool rs = EFalse;
    if (iHiraganaKanji)
        {
        iHiraganaKanji->CancelRepeat();
        }
    if (iUiStatePenMgr)
        {
        CPeninputUiStateBase* current = iUiStatePenMgr->CurrentUiState();
        if (iUiStatePenMgr->GetUiState(EPeninputHwrUiStatePenBeginWriting) == current)
            {  // temporary
            return ETrue;
            }
        if (iUiStatePenMgr->GetUiState(EPeninputHwrUiStatePenEndWriting) == current
            || iUiStatePenMgr->GetUiState(EPeninputHwrUiStatePenCandidateSelecting) == current)
            {
            TInt key = (aKeyEvent.ScanCode() & 0x000000FF);
            switch(key)
                {  // Handle clear key
                case EKeyBackspace:
                case EStdKeyBackspace:      /* 0x01 */
                case EStdKeyEscape:         /* 0x04 */
                    {
                    // if TRawEvent::EKeyDown, return at once.
                    if (TRawEvent::EKeyDown == aKeyEvent.Type())
                        {
                        iKeyEventClearAsync->CallBack();
                        }
                    return ETrue;
                    }
                default:
                    ToPenStandBy();
                    break;
                }
            }
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
                return UiStateMgr()->GetUiState(EPeninputHwrUiStateStandby)->HandleKeyEventL(aKeyEvent);
                }
            rs = iHiraganaKanji->HandleKeyEvent(aKeyEvent);
            }
        }
    if (!iWaitforEditorTextComing && TRawEvent::EKeyDown == aKeyEvent.Type() && TRawEvent::ENone == iKeyEvent.Type())
        {
        if (!rs)
            {
            SendEventToWindow(EPeninputLayoutEventCloseCharacterRange, NULL, KNullDesC);
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
// CPeninputHwrBoxLayout::OnPointerEventOutsideLayout
// Call back when the pointer down/up event happends outside of the layout
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool CPeninputHwrBoxLayout::OnPointerEventOutsideLayout(const TRawEvent& aEvent)
    {
    if (TRawEvent::EButton1Down == aEvent.Type()) // this if is not necessary. only EButton1Down.
        {
        HandleControlEventPen(EPeninputLayoutHwrEventPointerOutsideWnd, NULL, KNullDesC);
        SendEventToWindow(EPeninputLayoutEventCloseCharacterRange, NULL, KNullDesC);
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
// CPeninputHwrBoxLayout::DataMgr
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputHwrBoxDataMgr* CPeninputHwrBoxLayout::DataMgr(TInt /*aDummy*/)
    {
    return static_cast<CPeninputHwrBoxDataMgr*>(iDataMgr);
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxLayout::HwrBoxWindow
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputHwrBoxWindow* CPeninputHwrBoxLayout::HwrBoxWindow()
    {
    return iHwrWindow;
    }

void CPeninputHwrBoxLayout::ToPenStandBy(TBool aNotStandBy/* = EFalse*/)
    {
    if (iUiStatePenMgr)
        {
        CPeninputUiStateBase* current = iUiStatePenMgr->CurrentUiState();
        if (iUiStatePenMgr->GetUiState(EPeninputHwrUiStatePenEndWriting) == current
            || iUiStatePenMgr->GetUiState(EPeninputHwrUiStatePenCandidateSelecting) == current)
            {
            static_cast<CPeninputHwrBoxDataMgr*>(iDataMgr)->ClearCandidates();
#ifdef __PENINPUT_HWR_STROKE_LOG_ON
            if (iUiStatePenMgr->GetUiState(EPeninputHwrUiStatePenEndWriting) == current && !aNotStandBy)
                {
                WriteHwrStrokeLogNew();
                }
#endif // __PENINPUT_HWR_STROKE_LOG_ON
            }
        if (!aNotStandBy)
            {
            iUiStatePenMgr->SetCurrentUiState(EPeninputHwrUiStatePenStandBy);
            iHwrWindow->CancelWriting();   // Standby::OnEntry()
            iHwrWindow->DimInputContextField(EFalse);
            }
        delete iSubmitText;
        iSubmitText = NULL;
        iExpanded = EFalse;
        }
    }

void CPeninputHwrBoxLayout::Initialize()
    {
    if (iUiStatePenMgr)
        {
#ifdef __PENINPUT_HWR_STROKE_LOG_ON
        CPeninputUiStateBase* current = iUiStatePenMgr->CurrentUiState();
        if (iUiStatePenMgr->GetUiState(EPeninputHwrUiStatePenEndWriting) == current
            || iUiStatePenMgr->GetUiState(EPeninputHwrUiStatePenBeginWriting) == current)
            {
            WriteHwrStrokeLogNew();
            }
#endif // __PENINPUT_HWR_STROKE_LOG_ON
        iUiStatePenMgr->SetCurrentUiState(EPeninputHwrUiStateNone);
        }
    if (iHwrWindow)
        {
        iHwrWindow->CancelWriting();
        iHwrWindow->Hide(ETrue);
        iExpanded = EFalse;
        }
    }

void CPeninputHwrBoxLayout::CompleteIfConvertingL()
    {
    if (iCapturePointerEvent    // Transitory Input Exists
        && UiStateMgr() && UiStateMgr()->CurrentUiState() == UiStateMgr()->GetUiState(EPeninputHwrUiStateWithTransitoryChars))
        {
        iWaitforEditorTextComing = 0;
        if (iHiraganaKanji)
            {
            iHiraganaKanji->HandleControlEventJpL(EEventCompleteIfConvertingHiraganaKanji, KNullDesC);
            }
        if (iWaitforEditorTextComing)   // the status of hiraganakanji was converting  ---> complete,but should not capture off
            {
            UiStateMgr()->SetCurrentUiState(EPeninputHwrUiStateStandbyjp);
            iCapturePointerEvent = EFalse;
            }
        }
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxLayout::ClearInputToPenStandByL()
// clear last input char and goto PenStandBy
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputHwrBoxLayout::ClearInputToPenStandByL()
    {
    if (iSubmitText)
        {
        TInt len = iSubmitText->Length();
        if (len > 1 &&
            UiStateMgr() && UiStateMgr()->CurrentUiState() == UiStateMgr()->GetUiState(EPeninputHwrUiStateWithTransitoryChars))
            {
            len = 1;
            }
        for (TInt i = 0; i < len; i++)
            {
            DeleteL(EKeyDelete);
            }
        }
    ToPenStandBy();
    if (UiStateMgr() && UiStateMgr()->CurrentUiState() == UiStateMgr()->GetUiState(EPeninputHwrUiStateWithTransitoryChars))
        {
        iHiraganaKanji->HandleControlEventJpL(EEventVirtualKeyUp,KNullDesC);
        }
    }

#ifdef __PENINPUT_HWR_STROKE_LOG_ON
// ---------------------------------------------------------------------------
// CPeninputHwrBoxLayout::WriteHwrStrokeLogNew
// write log "New"
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputHwrBoxLayout::WriteHwrStrokeLogNew()
    {
    if (iLogger.LogValid())
        {
        iLogger.Write(KHwrStrokeLogNew());
        }
    }
#endif // __PENINPUT_HWR_STROKE_LOG_ON

// ---------------------------------------------------------------------------
// CPeninputHwrBoxLayout::CPeninputHwrBoxLayout
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPeninputHwrBoxLayout::CPeninputHwrBoxLayout(MLayoutOwner* aLayoutOwner)
    : CPeninputCommonLayout( aLayoutOwner ),
    iCapturePointerEvent(EFalse),
    iExpanded(EFalse)
    {
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxLayout::ConstructL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputHwrBoxLayout::ConstructL(const TAny* aInitData)
    {
    CPeninputCommonLayout::ConstructL( aInitData );

#ifdef __PENINPUT_HWR_STROKE_LOG_ON
    User::LeaveIfError( iLogger.Connect() );
    // Make directory
    RFs fs;
    if (fs.Connect() == KErrNone)
        {
        TInt ret = fs.MkDirAll(KHwrStrokeLogBaseDir());
        fs.Close();
        }
#endif // __PENINPUT_HWR_STROKE_LOG_ON

    iUiStatePenMgr = CPeninputUiStateMgr::NewL( this );

    CPeninputUiStateBase* initialState =
        CPeninputUiStateBase::NewL( UiStateMgr(), this );
    UiStateMgr()->AddUiState( initialState, EPeninputHwrUiStateNone );

    CPeninputHwrBoxUiStateStandby* standbyState =
        CPeninputHwrBoxUiStateStandby::NewL( UiStateMgr(), this );
    UiStateMgr()->AddUiState( standbyState, EPeninputHwrUiStateStandby );

    CPeninputHwrBoxUiStateStandbyjp* standbyjpState =
        CPeninputHwrBoxUiStateStandbyjp::NewL( UiStateMgr(), this );
    UiStateMgr()->AddUiState( standbyjpState, EPeninputHwrUiStateStandbyjp );

    CPeninputHwrBoxUiStateWithTransitoryChars* transitoryState =
        CPeninputHwrBoxUiStateWithTransitoryChars::NewL( UiStateMgr(), this );
    UiStateMgr()->AddUiState( transitoryState, EPeninputHwrUiStateWithTransitoryChars );

    UiStateMgr()->SetCurrentUiState( initialState );

    CPeninputUiStateBase* penInitialState =
        CPeninputHwrBoxUiState::NewL( iUiStatePenMgr, this );

    iUiStatePenMgr->AddUiState( penInitialState, EPeninputHwrUiStateNone );
    CPeninputHwrBoxUiStatePenStandby* penStandbyState =
        CPeninputHwrBoxUiStatePenStandby::NewL( iUiStatePenMgr, this );
    iUiStatePenMgr->AddUiState( penStandbyState, EPeninputHwrUiStatePenStandBy );

    CPeninputHwrBoxUiStatePenBeginWriting* penBeginState =
        CPeninputHwrBoxUiStatePenBeginWriting::NewL( iUiStatePenMgr, this );
    iUiStatePenMgr->AddUiState( penBeginState, EPeninputHwrUiStatePenBeginWriting );

    CPeninputHwrBoxUiStatePenEndWriting* penEndState =
        CPeninputHwrBoxUiStatePenEndWriting::NewL( iUiStatePenMgr, this );
    iUiStatePenMgr->AddUiState( penEndState, EPeninputHwrUiStatePenEndWriting );

    CPeninputHwrBoxUiStatePenCandidateSelecting* penCandidateSelectingState =
        CPeninputHwrBoxUiStatePenCandidateSelecting::NewL( iUiStatePenMgr, this );
    iUiStatePenMgr->AddUiState( penCandidateSelectingState, EPeninputHwrUiStatePenCandidateSelecting );

    iUiStatePenMgr->SetCurrentUiState( penInitialState );

    iHwrWindow = static_cast<CPeninputHwrBoxWindow*>(iLayoutWindow);
    Initialize();
    {
    TInt penSpeed = static_cast<CPeninputHwrBoxDataMgr*>(iDataMgr)->WritingSpeed();
    TPtr ptr((TUint16*)&penSpeed,sizeof(TInt)/sizeof(TUint16));
    SendEventToWindow(EPeninputLayoutHwrEventWritingSpeedChange, NULL, ptr);
    }
    {
    TSize penSize = static_cast<CPeninputHwrBoxDataMgr*>(iDataMgr)->PenSize();
    TPtr ptr((TUint16*)&penSize,sizeof(TSize)/sizeof(TUint16));
    SendEventToWindow(EPeninputLayoutHwrEventPenSizeChange, NULL, ptr);
    }
    {
    TRgb penColor = static_cast<CPeninputHwrBoxDataMgr*>(iDataMgr)->PenColor();
    TPtr ptr((TUint16*)&penColor,sizeof(TRgb)/sizeof(TUint16));
    SendEventToWindow(EPeninputLayoutHwrEventPenColorChange, NULL, ptr);
    }
    //create hiraganakanji
    iHiraganaKanji = CPeninputHiraganaKanji::NewL(static_cast<CPeninputHwrBoxDataMgr*>(iDataMgr)->PtiEngine()
                                                       , this
                                                       , iHwrWindow->InputContextField()
                                                       , iHwrWindow->CandidateWindow()
                                                       , iHwrWindow->PredictiveWindow());
    TCallBack cb(KeyEventCallback, this);
    TCallBack cbpointer(PointerEventCallback, this);
    iKeyEventAsync = new(ELeave) CAsyncCallBack(cb, CActive::EPriorityHigh);
    iPointerEventAsync = new(ELeave) CAsyncCallBack(cbpointer, CActive::EPriorityStandard);
    TCallBack cbClear(KeyEventClearCallback, this);
    iKeyEventClearAsync = new(ELeave) CAsyncCallBack(cbClear, CActive::EPriorityHigh);

    iKeyEvent.Set(TRawEvent::ENone,0);
    iPointerEventDown.Set(TRawEvent::ENone,0);
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxLayout::DoSetCurrentRange
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputHwrBoxLayout::DoSetCurrentRange()
    {
    TInt currentRange = CPeninputDataConverter::AnyToInt( RequestData( EPeninputDataTypeCurrentRange ) );
    static_cast<CPeninputHwrBoxDataMgr*>(iDataMgr)->SetEngineInputMode(currentRange); // PtiEngine:SetInputMode()

    if ( currentRange == ERangeHiraganaKanji && static_cast<CPeninputHwrBoxDataMgr*>(iDataMgr)->Conversion())
        {
        UiStateMgr()->SetCurrentUiState(EPeninputHwrUiStateStandbyjp);
        }
    else
        {
        UiStateMgr()->SetCurrentUiState(EPeninputHwrUiStateStandby);
        }
   SetCapturePointerEvent(EFalse);
   ToPenStandBy();
#ifdef __PENINPUT_HWR_STROKE_LOG_ON
   WriteHwrStrokeLogRange();
#endif // __PENINPUT_HWR_STROKE_LOG_ON
   }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxLayout::CompleteAllHiraganaKanji
// ---------------------------------------------------------------------------
//
void CPeninputHwrBoxLayout::HiraganaKanjiCompleteAll()
    {
    ToPenStandBy();
    if( UiStateMgr() && UiStateMgr()->CurrentUiState() == UiStateMgr()->GetUiState(EPeninputHwrUiStateWithTransitoryChars))
        {
        if (iHiraganaKanji)
            {  // CompleteALL
            TRAP_IGNORE(iHiraganaKanji->HandleControlEventJpL(EEventCompleteAllHiraganaKanji, KNullDesC));
            }
        }
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxLayout::CreateLayoutWindowL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputHwrBoxLayout::CreateLayoutWindowL()
    {
    iLayoutWindow = CPeninputHwrBoxWindow::NewL( this, this );
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxLayout::CreateDataMgrL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputHwrBoxLayout::CreateDataMgrL( const TAny* /*aInitData*/ )
    {
    iDataMgr = CPeninputHwrBoxDataMgr::NewL( this );
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxLayout::LayoutType
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TInt CPeninputHwrBoxLayout::LayoutType()
    {
    return EPluginInputModeHwr;
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxLayout::SemiTransparencyRequired
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TBool CPeninputHwrBoxLayout::SemiTransparencyRequired()
    {
    return ETrue;
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxLayout::TranslateRangeHullHalf
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TInt CPeninputHwrBoxLayout::TranslateRangeHullHalf(TInt aRange) const
    {
    TInt newRange = aRange;
    if (static_cast<CPeninputHwrBoxDataMgr*>(iDataMgr)->FullHalf()) // Full
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

TInt CPeninputHwrBoxLayout::KeyEventCallback(TAny* aThis)
    {
    if (aThis)
        {
        TRAPD(err,static_cast<CPeninputHwrBoxLayout*>(aThis)->DoKeyEventL());
        if (KErrNone != err)
            {
            static_cast<CPeninputHwrBoxLayout*>(aThis)->HandleControlEvent(EPeninputLayoutEventClose, NULL, KNullDesC);
            }
        return err;
        }
    return 0;
    }

TInt CPeninputHwrBoxLayout::PointerEventCallback(TAny* aThis)
    {
    if (aThis)
        {
        static_cast<CPeninputHwrBoxLayout*>(aThis)->DoPointerEvent();
        }
    return 0;
    }

void CPeninputHwrBoxLayout::DoKeyEventL()
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

void CPeninputHwrBoxLayout::DoPointerEvent()
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

TInt CPeninputHwrBoxLayout::KeyEventClearCallback(TAny* aThis)
    {
    if (aThis)
        {
        TRAPD(err,static_cast<CPeninputHwrBoxLayout*>(aThis)->DoKeyClearEventL());
        if (KErrNone != err)
            {
            static_cast<CPeninputHwrBoxLayout*>(aThis)->HandleControlEvent(EPeninputLayoutEventClose, NULL, KNullDesC);
            }
        return err;
        }
    return 0;
    }

void CPeninputHwrBoxLayout::DoKeyClearEventL()
    {
    ClearInputToPenStandByL();
#ifdef __PENINPUT_HWR_STROKE_LOG_ON
    WriteHwrStrokeLogClear();
#endif // __PENINPUT_HWR_STROKE_LOG_ON
    if (iHiraganaKanji)
        {
        iHiraganaKanji->RepeatClearKeyStart();
        }
    }

void CPeninputHwrBoxLayout::SetCapturePointerEvent(TBool aFlag)
    {
    if (iCapturePointerEvent && !aFlag || !iCapturePointerEvent && aFlag)
        {
        TBool dimmFlag = (aFlag)? EFalse : ETrue;
        TPtr ptr((TUint16*)&dimmFlag,sizeof(TBool)/sizeof(TUint16));
        SendEventToWindow(EPeninputLayoutEventSetConversion, NULL, ptr);
        //signal the owner that pointer needs not be captured
        TBool capture = aFlag;
        TPtrC captureData;
        captureData.Set((const TUint16*)&capture,sizeof(TBool)/sizeof(TUint16));
        SignalOwner(ESignalCapturePointer,captureData);
        iCapturePointerEvent = aFlag;
        }
    }

void CPeninputHwrBoxLayout::SetCapturePointerEventAgain()
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

void CPeninputHwrBoxLayout::HiraganaHandleControlEventJp(TInt aEventType, const TDesC& aEventData)
    {
    if (iHiraganaKanji)
        {
        TRAPD(err, iHiraganaKanji->HandleControlEventJpL(aEventType,aEventData));
        if (KErrNone != err)
            {   // close peninput
            HiraganaKanjiCompleteAll();  // call ToPenStandBy()
            SetCapturePointerEvent(EFalse);
            Initialize();  //jump to init state
            CPeninputCommonLayout::HandleControlEvent(EPeninputLayoutEventClose, NULL, KNullDesC);
            }
        }
    }

TBool CPeninputHwrBoxLayout::IsPredictiveWithNoChar() const
    {
    if (!iCapturePointerEvent    // without transitory chars
    && UiStateMgr()->CurrentUiState() == UiStateMgr()->GetUiState(EPeninputHwrUiStateWithTransitoryChars))
        {
        return ETrue;
        }
    return EFalse;
    }

#ifdef __PENINPUT_HWR_STROKE_LOG_ON
// ---------------------------------------------------------------------------
// CPeninputHwrBoxLayout::WriteHwrStrokeLogClear
// write log "Clear"
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputHwrBoxLayout::WriteHwrStrokeLogClear()
    {
    if (iLogger.LogValid())
        {
        iLogger.Write(KHwrStrokeLogClear());
        }
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxLayout::WriteHwrStrokeLogRange
// write log characterrange
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputHwrBoxLayout::WriteHwrStrokeLogRange()
    {
    if (iLogger.LogValid() && !iLogOff)
        {
        TBuf<KHwrStrokeLogCharacterRangeLen> characterRange;
        switch(static_cast<CPeninputHwrBoxDataMgr*>(iDataMgr)->CharacterRangeId())
            {
            case EPeninutCharacterRangeIdKanaKanji:
                characterRange.Append(KHwrStrokeLogRangeKanaKanji());
                break;
            case EPeninutCharacterRangeIdLatin:
                characterRange.Append(KHwrStrokeLogRangeLatin());
                break;
            case EPeninutCharacterRangeIdNumber:
                characterRange.Append(KHwrStrokeLogRangeNumber());
                break;
            default:
                characterRange.Append(KHwrStrokeLogRangeAllTypes());
                break;
            }
        iLogger.WriteFormat(KHwrStrokeLogCharacterRange(), &characterRange);
        }
    }
#endif // __PENINPUT_HWR_STROKE_LOG_ON
// End Of File
