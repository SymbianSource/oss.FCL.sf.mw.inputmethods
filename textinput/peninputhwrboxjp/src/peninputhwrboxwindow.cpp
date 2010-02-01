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
* Description:  Implementation of hwr window class
*
*/


// System includes
#include <peninputhwrwindowjp.rsg>
#include <peninputhwrwindowconfiginfo_32.rsg>
#include <peninputdataconverter.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <peninputcommonlayoutglobalenum.h>  // EPeninputLayoutEventEnter
#include <peninputclientlayoutinfo.h>
#include <peninputcontextfieldjp.h>
#include <peninputeventbutton.h>
#include <peninputrepeatbutton.h>
#include <peninputdropdownlist.h>
#include <peninputjapanesepredictivewnd.h>
#include <peninputjapanesecandidatewnd.h>
#include <AknsUtils.h>
#include <aknfeppeninputenums.h>    // ERangeEnglish, ...

// User includes
#include "peninputhwrboxlayout.h"
#include "peninputhwrboxwindow.h"
#include "peninputhwrboxgroup.h"
#include "peninputhwrboxdatamgr.h"
#include "peninputhwrboxjp.hrh"
#include "peninputhwrevent.h"

// Constants
_LIT( KHwrWindowResourceFile,
      "z:\\resource\\plugins\\peninputhwrwindowjp.RSC" );
_LIT( KConfigurationResourceFile,
      "z:\\resource\\plugins\\peninputhwrwindowconfiginfo_32.RSC" );
const TInt KWndTransparencyFactor = 180;

const TInt KBoxCount = 1;


const TInt KWriteBoxFrameBackColorMajor = EAknsMajorSkin;
const TInt KWriteBoxFrameBackColorGrp = EAknsMinorQsnOtherColors;
const TInt KWriteBoxFrameBackColorIdx = EAknsCIQsnOtherColorsCG9; 
const TUint32 KDefaultWriteBoxFrameColor = 0x000000;
const TUint32 KDefaultWriteBoxBackColor = 0xffffff;
// ======== MEMBER FUNCTIONS ========

// --------------------------------------------------------------------------
// CPeninputHwrBoxWindow::NewL
// (other items were commented in a header)
// --------------------------------------------------------------------------
CPeninputHwrBoxWindow* CPeninputHwrBoxWindow::NewL(
    CFepUiLayout* aUiLayout, MPeninputLayoutContext* aLayoutContext )
    {
    CPeninputHwrBoxWindow* self =
        new ( ELeave ) CPeninputHwrBoxWindow( aUiLayout, aLayoutContext );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// --------------------------------------------------------------------------
// CPeninputHwrBoxWindow::~CPeninputHwrBoxWindow
// (other items were commented in a header)
// --------------------------------------------------------------------------
CPeninputHwrBoxWindow::~CPeninputHwrBoxWindow()
    {
    }

// --------------------------------------------------------------------------
// CPeninputHwrBoxWindow::HandleControlEvent
// (other items were commented in a header)
// --------------------------------------------------------------------------
void CPeninputHwrBoxWindow::HandleControlEvent( TInt aEventType,
                                                    CFepUiBaseCtrl* aCtrl,
                                                    const TDesC& aEventData )
    {
    // Call this function in base class
    CPeninputLayoutWindowJp::HandleControlEvent( aEventType,
                                               aCtrl,
                                               aEventData );

    TInt* data = ( TInt* ) aEventData.Ptr();

    switch ( aEventType )
        {
        case EPeninputLayoutHwrEventPenColorChange:
            {
            iBoxGroup->SetBoxPenColor(*((TRgb*)data));
            }
            break;
        case EPeninputLayoutHwrEventPenSizeChange:
            {
            iBoxGroup->SetBoxPenSize(*((TSize*)data));
            }
            break;
        case EPeninputLayoutHwrEventWritingSpeedChange:
            {
            iBoxGroup->SetWritingSpeed(*data);

#ifdef _NOT_USING_DECUMA_MCR_

            // similar as S90
            iBoxGroup->SetStrokeDelay(90000);
#endif
            }
            break;
        case EPeninputLayoutHwrEventCandidates:
            {
            const TInt candiCount = static_cast<CPeninputHwrBoxLayout*>(UiLayout())->DataMgr(0)->CandidateCount();
            RArray<TInt> secondaryList;

            for (TInt i = 0; i < candiCount; i++)
                {
                if (i > candiCount - static_cast<CPeninputHwrBoxLayout*>(UiLayout())->DataMgr(0)->PermittedRangesCount())
                    {
                    //cell index start from 1
                    secondaryList.Append(i);
                    }
                }
            iDropdownList->SetSecondaryList(secondaryList);

            secondaryList.Close();

            RPointerArray<HBufC>* candidates = (RPointerArray<HBufC>*)aEventData.Ptr();
            if ( candiCount )
                {
                TRAP_IGNORE(iDropdownList->SetCandidatesL(*candidates, CFepCtrlDropdownList::EListExpandableWithoutIcon));
                }
            else
                {
                TRAP_IGNORE(iDropdownList->SetCandidatesL(*candidates, CFepCtrlDropdownList::EListNoExpandable));
                }
            }
            break;
        case EPeninputLayoutEventSwitchCharacterRange:
            if (iCharacterRangeWnd)
                {
                CFepUiBaseCtrl* rangeBtn = Control(EPeninutWindowCtrlIdSwitchToVkbBtn);
                if (rangeBtn)
                    {
                    TRect rect = rangeBtn->Rect();
                    TPoint point;
                    point.iX = rect.iTl.iX-6;
                    point.iY = rect.iBr.iY-6;
                    iCharacterRangeWnd->SetPosition(point);
                    }
                TInt index = static_cast<CPeninputHwrBoxLayout*>(UiLayout())->DataMgr(0)->CurrentCharacterRangeIndex();
                if (0 <= index && index <static_cast<CPeninputHwrBoxLayout*>(UiLayout())->DataMgr(0)->CharacterRangeArray()->Count())
                    {
                    TRAP_IGNORE(iCharacterRangeWnd->ShowPopupL(static_cast<CPeninputHwrBoxLayout*>(UiLayout())->DataMgr(0)->CharacterRangeArray(), index));
                    }
                }
            break;
        case EPeninputLayoutEventCloseCharacterRange:
            if (iCharacterRangeWnd)
                {
                iCharacterRangeWnd->HidePopup();
                }
            break;
        case EPeninputLayoutEventSetCharacterRangeText:  // from datamgr::SetPremaryRange
            {
            TInt count = static_cast<CPeninputHwrBoxLayout*>(UiLayout())->DataMgr(0)->CharacterRangeArray()->Count();
            TBool dimmFlag = (count < 2)? ETrue : EFalse;
            TInt resourceId = R_PENINPUT_LAYOUT_HWR_SWITCH_RANGE_ALL;
            switch(static_cast<CPeninputHwrBoxLayout*>(UiLayout())->DataMgr(0)->CharacterRangeId())
                {
                case EPeninutCharacterRangeIdKanaKanji:
                    resourceId = R_PENINPUT_LAYOUT_HWR_SWITCH_RANGE_KANA_KANJI;  // temporary
                    break;
                case EPeninutCharacterRangeIdLatin:
                    resourceId = R_PENINPUT_LAYOUT_HWR_SWITCH_RANGE_LATIN;
                    break;
                case EPeninutCharacterRangeIdNumber:
                    resourceId = R_PENINPUT_LAYOUT_HWR_SWITCH_RANGE_NUMERIC;
                    break;
                default:
                    break;
                }
            iCharacterRangeSwitchBtn->SetResourceId(resourceId);
            TRAP_IGNORE(iCharacterRangeSwitchBtn->ConstructFromResourceL());
            iCharacterRangeSwitchBtn->SetDimmed(dimmFlag);
            }
            break;
        case EPeninputLayoutEventSetConversion:
            iConvertBtn->SetDimmed(*((TBool*)data));
            break;
        case EPeninputLayoutHwrEventStrokeEndMark:
            {
            const TPoint endMark = *(TPoint*)(aEventData.Ptr());
            iBoxGroup->SetStrokeEndMark(endMark);
            }
            break;
        case EEventHwrStrokeStarted:
            {
            iDisableCaseChange = ETrue;
            }
            break;
        case EEventHwrCharacterTimerOut:
        case EEventHwrStrokeCanceled:
            {
            if (iDisableCaseChange)
                {
                iDisableCaseChange = EFalse;
                if (iCachedCase)
                    {
                    DoCaseChange(iCachedCase);
                    }
                }
            iCachedCase = 0;
            }
            break;
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxWindow:HandlePointerDownEventL
// Handle pointer down event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
CFepUiBaseCtrl* CPeninputHwrBoxWindow::HandlePointerDownEventL(const TPoint& aPoint)
    {
    if (!CandidateWindow()->Hiden()
        && iDropdownList->Rect().Contains(aPoint)
        && !CandidateWindow()->Rect().Contains(aPoint))
        {
        return this;
        }
    return CPeninputLayoutWindowJp::HandlePointerDownEventL(aPoint);
    }

// --------------------------------------------------------------------------
// CPeninputHwrBoxWindow::GetWindowConfigResId
// (other items were commented in a header)
// --------------------------------------------------------------------------
TInt CPeninputHwrBoxWindow::GetWindowConfigResId()
    {
    return R_PENINPUT_LAYOUT_CONFIG_INFO;
    }

// --------------------------------------------------------------------------
// CPeninputHwrBoxWindow::GetWindowResId
// (other items were commented in a header)
// --------------------------------------------------------------------------
TInt CPeninputHwrBoxWindow::GetWindowResId()
    {
    return R_PENINPUT_LAYOUT_HWR_WINDOW;
    }

// --------------------------------------------------------------------------
// CPeninputHwrBoxWindow::GetWindowConfigResFileName
// (other items were commented in a header)
// --------------------------------------------------------------------------
const TDesC& CPeninputHwrBoxWindow::GetWindowConfigResFileName(
    TInt /*aLangID*/ )    // LangId : 32 fixed
    {
    return KConfigurationResourceFile();
    }

// --------------------------------------------------------------------------
// CPeninputHwrBoxWindow::GetWindowResFileName
// (other items were commented in a header)
// --------------------------------------------------------------------------
const TDesC& CPeninputHwrBoxWindow::GetWindowResFileName()
    {
    return KHwrWindowResourceFile();
    }

// --------------------------------------------------------------------------
// CPeninputHwrBoxWindow::ChangeUnitSize
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
const TRect CPeninputHwrBoxWindow::ChangeUnitSize()
    {
    TAknWindowLineLayout appWnd;
    TAknWindowLineLayout mainPane;
    TAknWindowLineLayout wndLayout;
    TAknWindowLineLayout unitLayout;
    TAknLayoutRect mainPaneRect;
    TAknLayoutRect wndRect;
    TAknLayoutRect unitRect;

    // Get hwr window layout
    appWnd = AknLayoutScalable_Avkon::application_window( 0 ).LayoutLine();

    if (appWnd.iW > appWnd.iH)
        {
        // landscape
        mainPane = AknLayoutScalable_Avkon::main_pane(4).LayoutLine();
        }
    else
        {
        // portrait
        mainPane = AknLayoutScalable_Avkon::main_pane(1).LayoutLine();
        }
    if (!iPredictiveWnd->Hiden())
        {
        wndLayout = AknLayoutScalable_Avkon::popup_fep_hwr_window(7).LayoutLine();
        }
    else
        {
        wndLayout = AknLayoutScalable_Avkon::popup_fep_hwr_window(6).LayoutLine();
        }

    mainPaneRect.LayoutRect(appWnd.Rect(), mainPane);
    wndRect.LayoutRect(mainPaneRect.Rect(), wndLayout);

    unitLayout = AknLayoutScalable_Avkon::fep_hwr_aid_pane().LayoutLine();
    unitRect.LayoutRect(wndRect.Rect(), unitLayout);

    TRect hwrWndRect = TRect(0,
                             0,
                             wndRect.Rect().Width(),
                             wndRect.Rect().Height());

    TInt unitWidth = unitRect.Rect().Width();
    TInt unitHeight = unitRect.Rect().Height();

    iLayoutContext->SetData( EPeninputDataTypeUnitWidth, &unitWidth );
    iLayoutContext->SetData( EPeninputDataTypeUnitHeight, &unitHeight );

    return hwrWndRect;
    }

// --------------------------------------------------------------------------
// CPeninputHwrBoxWindow::ChangeClientSize
// (other items were commented in a header)
// --------------------------------------------------------------------------
void CPeninputHwrBoxWindow::ChangeClientSize()
    {
    TAknTextLineLayout languageSwitchText;
    TInt unitWidth = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeUnitWidth ) );
    TInt unitHeight = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeUnitHeight ) );

    TInt clientLayoutId = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeClientLayout ) );
    CPeninputClientLayoutInfo* clientLayout =
        ConfigInfo()->FindClientLayoutInfo( clientLayoutId );

    if ( clientLayout )
        {
        // Change self rect
        TInt width = clientLayout->Columns() * unitWidth;
        TInt height = clientLayout->Rows() * unitHeight;
        TRect rect = TRect( ClientPane()->Rect().iTl,
                            TSize( width, height ) );

        ClientPane()->SetRect( rect );
        // Change the postion and size of controls in current layout
        ReorganizeControls( clientLayout->LayoutID(), ETrue );
        }
    }

// --------------------------------------------------------------------------
// CPeninputHwrBoxWindow::CreateAllControlsL
// (other items were commented in a header)
// --------------------------------------------------------------------------
void CPeninputHwrBoxWindow::CreateAllControlsL()
    {
    CPeninputLayoutWindowJp::CreateAllControlsL();

    if (iInputContextField)
        {
        HeaderPane()->RemoveControl(static_cast<CFepUiBaseCtrl*>(iInputContextField));
        iInputContextField = NULL;
        }
    AddContextFieldJpL();

    // DropDown List
    AddDropdownListL();

    // Hwr box
    AddBoxGroupL();

    // convert button : use EventSpace
    iConvertBtn = AddButtonL( EPeninutWindowCtrlIdConvertToKanjiBtn,
                EPeninputLayoutEventSpace,
                R_PENINPUT_LAYOUT_HWR_CONVERT);
    iConvertBtn->SetDimmed(ETrue);

    // Backspace button
    AddButtonL( EPeninutWindowCtrlIdClear2Btn,
                EPeninputLayoutEventBack,
                R_PENINPUT_LAYOUT_HWR_BACKSPACE,
                EKeyBackspace,
                ETrue );

    // Enter button
    AddButtonL( EPeninutWindowCtrlIdEnterBtn,
                EPeninputLayoutEventEnter,
                R_PENINPUT_LAYOUT_HWR_ENTER,
                EKeyEnter );

    // character range switch button
    iCharacterRangeSwitchBtn = AddButtonL( EPeninutWindowCtrlIdInputLangSwitcherBtn,
                                 EPeninputLayoutEventSwitchCharacterRange,
                                 R_PENINPUT_LAYOUT_HWR_SWITCH_RANGE_ALL );

    // SCT button
    AddButtonL( EPeninutWindowCtrlIdSymbolBtn,
                EPeninputLayoutEventSct,
                R_PENINPUT_LAYOUT_HWR_SCT );

    // Switch to vkb button
    AddButtonL( EPeninutWindowCtrlIdSwitchToVkbBtn,
                EPeninputLayoutEventToVkb,
                R_PENINPUT_LAYOUT_HWR_VKB );

    for ( TInt i = 0; i < iCtrlPool->ControlCount(); i++ )
        {
        if ( iCtrlPool )
            {
            iCtrlPool->ControlByIndex( i )->AddEventObserver( UiLayout() );
            }
        }
    }

// --------------------------------------------------------------------------
// CPeninputHwrBoxWindow::SetControlsFont
// (other items were commented in a header)
// --------------------------------------------------------------------------
void CPeninputHwrBoxWindow::SetControlsFont()
    {
    CPeninputLayoutWindowJp::SetControlsFont();

    TAknTextLineLayout candTextLayout = AknLayoutScalable_Avkon::cell_hwr_candidate_pane_t1().LayoutLine();
    iDropdownList->SetFont(AknLayoutUtils::FontFromId(candTextLayout.iFont, NULL));
    }

// --------------------------------------------------------------------------
// CPeninputHwrBoxWindow::PopupChoiceList
// (other items were commented in a header)
// --------------------------------------------------------------------------
void CPeninputHwrBoxWindow::PopupChoiceList()
    {
    }

// --------------------------------------------------------------------------
// CPeninputHwrBoxWindow::ReorganizeControls
// (other items were commented in a header)
// --------------------------------------------------------------------------
void CPeninputHwrBoxWindow::ReorganizeControls( TInt aClientLayoutId, TBool /*aNeedReset*/ )
    {
    TInt unitWidth = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeUnitWidth ) );
    TInt unitHeight = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeUnitHeight ) );

    // First remove all controls in the client pane, but don't delete them
    ( const_cast<RPointerArray<CFepUiBaseCtrl>&>
        ( ClientPane()->ControlList() ) ).Reset();

    // Clear rect
    ClearRect( ClientPane()->Rect() );
    UpdateArea( ClientPane()->Rect(), EFalse );

    // Then add controls according to the configuration of clientlayout
    CPeninputClientLayoutInfo* clientLayout =
        ConfigInfo()->FindClientLayoutInfo( aClientLayoutId );

    // Get control list from client layout
    RPointerArray<CPeninputControlInfo>& controlList =
        clientLayout->ControlInfoList();
    const TInt count = controlList.Count();
    TInt controlID = 0;
    TInt cols = 0;
    TInt rows = 0;
    CFepUiBaseCtrl* ctrl = NULL;
    TRect rect = ClientPane()->Rect();

    // Set rect for client area
    rect.SetSize( TSize( clientLayout->Columns() * unitWidth,
                         clientLayout->Rows() * unitHeight ) );
    ClientPane()->SetRect( rect );

    // Set owner rect
    SetWholeWindowRect( ClientPane()->Rect() );

    TPoint point = rect.iTl;

    for ( TInt i = 0; i < count; i++ )
        {
        controlID = controlList[i]->ControlID();
        ctrl = iCtrlPool->Control( controlID );

        if ( ctrl )
            {
            rect.iTl = point
                + TPoint( controlList[i]->BeginColumn() * unitWidth,
                          controlList[i]->BeginRow() * unitHeight );
            cols = controlList[i]->EndColumn()
                - controlList[i]->BeginColumn();
            rows = controlList[i]->EndRow()
                - controlList[i]->BeginRow();
            rect.SetSize( TSize( cols * unitWidth, rows * unitHeight ) );

            if ( ( ctrl->ControlType() | ECtrlButton ) == ECtrlButton )
                {
                ( static_cast<CAknFepCtrlButton*> ( ctrl ) )->SizeChanged
                    ( rect, ETrue );
                }
            else if ( ctrl->ControlId() == EPeninutWindowCtrlIdHwrBox )
                {
                iBoxGroup->SizeChanged( rect );
                static_cast<CPeninputHwrBoxLayout*>(UiLayout())->DataMgr(0)->SetInputAreaSize(rect.Size());
                }
            else if ( ctrl->ControlId() == EPeninutWindowCtrlIdDropDownList )   // DropDownList
                {
                iDropdownList->SizeChanged(unitWidth, unitHeight, unitWidth, rect.iTl, cols-1, rows);
                }
            AddNotOwnedControl( ctrl );
            }
        }
    }

// --------------------------------------------------------------------------
// CPeninputHwrBoxWindow::GetScrollUpBtnResId
// (other items were commented in a header)
// --------------------------------------------------------------------------
TInt CPeninputHwrBoxWindow::GetScrollUpBtnResId()
    {
    return R_PENINPUT_LAYOUT_HWR_SCROLL_UP;
    }

// --------------------------------------------------------------------------
// CPeninputHwrBoxWindow::GetScrollDownBtnResId
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TInt CPeninputHwrBoxWindow::GetScrollDownBtnResId()
    {
    return R_PENINPUT_LAYOUT_HWR_SCROLL_DOWN;
    }

// --------------------------------------------------------------------------
// CPeninputHwrBoxWindow::DoCaseChange
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBoxWindow::DoCaseChange( TInt aNewCase )
    {
    if( iDisableCaseChange )
        {
        iCachedCase = aNewCase;
        return;
        }
    TInt* val = static_cast<TInt*>(iLayoutContext->RequestData(EPeninputDataTypePermittedRange));
    if ( *val & (ERangeEnglish | ERangeFullWidthEnglish))
        {
        iLayoutContext->SetData( EPeninputDataTypeCase, &aNewCase );
        }
    }

// ---------------------------------------------------------
// Highlight a dropdownlist cell
// ---------------------------------------------------------
//
void CPeninputHwrBoxWindow::HighlightCell(TInt aIndex)
    {
    iDropdownList->SetHighlightCell(aIndex);
    }

// ---------------------------------------------------------
// redraw dropdownlist
// ---------------------------------------------------------
//
void CPeninputHwrBoxWindow::DropdownListDraw()
    {
    iDropdownList->Draw();
    }

// ---------------------------------------------------------
// Cancel writing
// ---------------------------------------------------------
//
void CPeninputHwrBoxWindow::CancelWriting()
    {
    if (iBoxGroup)
        {
        iBoxGroup->CancelWriting();
        }
    }

// ---------------------------------------------------------
// Draw window and shadow
// ---------------------------------------------------------
//
void CPeninputHwrBoxWindow::DimInputContextField( TBool aFlag )
    {
    if ( iInputContextField )
        {
        iInputContextField->SetDimmed( aFlag );
        }
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxWindow::SetDimmed
// Dim hwrbox
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputHwrBoxWindow::SetDimmed(TBool aDimFlag)
    {
    if (iBoxGroup)
        {
        iBoxGroup->SetDimmed(aDimFlag);
        }
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxWindow::BoxGroup()
// hwrbox control
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPeninputHwrBoxGroup* CPeninputHwrBoxWindow::BoxGroup()
    {
    return iBoxGroup;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrBoxWindow::ConstructFromResourceL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPeninputHwrBoxWindow::ConstructFromResourceL()
    {
    CPeninputLayoutWindowJp::ConstructFromResourceL();
    SetHwBoxFrameBackColor();
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxWindow::SetEnableSettingBtnJp
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//     
void CPeninputHwrBoxWindow::SetEnableSettingBtnJp(TBool aEnable)
    {
    if ( aEnable )
        {
        iTouchInputOptionButton->SetDimmed(EFalse);
        }
    else
        {
        iTouchInputOptionButton->SetDimmed(ETrue);
        iTouchInputOptionButton->SetActive(EFalse);
        } 
    }

// --------------------------------------------------------------------------
// CPeninputHwrBoxWindow::CPeninputHwrBoxWindow
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputHwrBoxWindow::CPeninputHwrBoxWindow(
    CFepUiLayout* aUiLayout, MPeninputLayoutContext* aLayoutContext )
    : CPeninputLayoutWindowJp( aUiLayout, aLayoutContext ),
    iDisableCaseChange(EFalse)
    {
    }

// --------------------------------------------------------------------------
// CPeninputHwrBoxWindow::AddButtonL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CAknFepCtrlEventButton* CPeninputHwrBoxWindow::AddButtonL(
    const TInt aControlId, const TInt aEventId, const TInt aResId,
    const TInt aUnicode, const TBool aIsRepeat )
    {
    CAknFepCtrlEventButton* button = NULL;

    if ( aIsRepeat )
        {
        button = CAknFepCtrlRepeatButton::NewL( UiLayout(), aControlId,
                                                aEventId, aUnicode );
        }
    else
        {
        button = CAknFepCtrlEventButton::NewL( UiLayout(), aControlId,
                                               aEventId, aUnicode );
        }

    CleanupStack::PushL( button );
    if ( button )
        {
        // Read resource
        TResourceReader reader;

        CCoeEnv::Static()->CreateResourceReaderLC( reader, aResId );
        button->SetResourceId( aResId );
        button->ConstructFromResourceL();
        // Pop and destroy reader
        CleanupStack::PopAndDestroy( 1 );

        // Add into the control pool
        if ( iCtrlPool )
            {
            iCtrlPool->AddControl( button );
            }
        }
    CleanupStack::Pop( button );

    return button;
    }

// --------------------------------------------------------------------------
// CPeninputHwrBoxWindow::AddBoxGroupL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBoxWindow::AddBoxGroupL()
    {
    iBoxGroup = CPeninputHwrBoxGroup::NewL(UiLayout(), EPeninutWindowCtrlIdHwrBox, KBoxCount);
    iBoxGroup->SetTransparency(KWndTransparencyFactor, 100);

    if ( iCtrlPool )
        {
        iCtrlPool->AddControl( iBoxGroup );
        }
    SetHwBoxFrameBackColor();
    }

// --------------------------------------------------------------------------
// CPeninputHwrBoxWindow::SetHwBoxFrameBackColor
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBoxWindow::SetHwBoxFrameBackColor()
    {
    TRgb frameColor;
    TRgb backColor;
    TAknsItemID id;
    
    id.Set(KWriteBoxFrameBackColorMajor, KWriteBoxFrameBackColorGrp);

    TInt error = AknsUtils::GetCachedColor(AknsUtils::SkinInstance(),
                                           frameColor,
                                           id,
                                           KWriteBoxFrameBackColorIdx);

    if (error != KErrNone)
        {
        frameColor = TRgb(KDefaultWriteBoxFrameColor);
        backColor = TRgb(KDefaultWriteBoxBackColor);
        }
    else
        {
        backColor = frameColor;	
        }

    if (iBoxGroup)
        {
        iBoxGroup->SetBoxFrameColor(frameColor);
        iBoxGroup->SetBoxBackColor(backColor);
        }
    }

// ---------------------------------------------------------
// Add dropdown list to window
// ---------------------------------------------------------
//
void CPeninputHwrBoxWindow::AddDropdownListL()
    {
    ChangeUnitSize();
    TInt unitWidth = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeUnitWidth ) );
    TInt unitHeight = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeUnitHeight ) );

    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC(reader,R_PENINPUT_LAYOUT_HWR_DROP_DOWN_LIST);

    TAknTextLineLayout candTextLayout = AknLayoutScalable_Avkon::cell_hwr_candidate_pane_t1().LayoutLine();

    TInt textMargin = (unitHeight -
                      AknLayoutUtils::FontFromId(candTextLayout.iFont, NULL)->HeightInPixels())/2;

    iDropdownList = CFepCtrlDropdownList::NewL(TPoint(Rect().iTl.iX, Rect().iTl.iY + unitHeight),
                                               UiLayout(),
                                               EPeninutWindowCtrlIdDropDownList,
                                               reader,
                                               unitWidth,
                                               unitHeight,
                                               unitWidth,
                                               PENINPUT_LAYOUT_HWR_COL_COUNT-1,
                                               PENINPUT_LAYOUT_HWR_ROW_COUNT,
                                               AknLayoutUtils::FontFromId(candTextLayout.iFont, NULL),
                                               NULL,
                                               CGraphicsContext::ECenter,
                                               textMargin,
                                               KRgbBlack,       // border color
                                               TRgb(0xeeeeee),  // content bg color
                                               KRgbWhite,       // navi bg color
                                               AKN_LAF_COLOR_STATIC(candTextLayout.iC),
                                               KRgbRed);

    if ( iCtrlPool )
        {
        iCtrlPool->AddControl( iDropdownList );
        }
    iDropdownList->SetResourceId(R_PENINPUT_LAYOUT_HWR_DROP_DOWN_LIST);
    iDropdownList->SetEventIdForCandidateSelected(EPeninputLayoutHwrEventCandidateSelected);
    CleanupStack::PopAndDestroy(); // reader
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxWindow::SetWholeWindowRect
// [Ref]CPeninputLayoutWindow::SetWindowRect
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputHwrBoxWindow::SetWholeWindowRect(const TRect& aRect)
    {
    TRect rect = aRect;

    TInt unitHeight = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeUnitHeight ) );
    rect.iTl.iY = rect.iTl.iY - unitHeight;

    if (!iPredictiveWnd->Hiden())
        {
        TRect predictiveRect = aRect;
        predictiveRect.iTl.iY = predictiveRect.iBr.iY;
        predictiveRect.iBr.iY += iPredictiveWnd->Rect().Size().iHeight;
        iPredictiveWnd->SetRect(predictiveRect);
        rect.iBr.iY += iPredictiveWnd->Rect().Size().iHeight;
        }

    SetRect(rect);


    TRect shadowRect = TRect(TPoint(rect.iTl.iX - iShadowTlWidth,
                                    rect.iTl.iY - iShadowTlHeight),
                             TPoint(rect.iBr.iX + iShadowBrWidth,
                                    rect.iBr.iY + iShadowBrHeight));

    SetShadowRect(shadowRect);
    ICFDisableRegionUpdating();  // ICF::OnDeActivate()
    UiLayout()->SetShadowRect(shadowRect);
    ICFDisableRegionUpdating();  // ICF::OnDeActivate()

    HandleControlEvent(EPeninputLayoutEventMovePosition, NULL, KNullDesC);
    }

// ---------------------------------------------------------------------------
// CPeninputHwrBoxWindow::AddContextFieldJpL
// [ref]CPeninputLayoutWindow::AddContextFieldL( aResID )
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputHwrBoxWindow::AddContextFieldJpL()
    {
    // Get reference CControlGroup of headerPane
    CControlGroup* headerPane = HeaderPane();

    User::LeaveIfNull( headerPane );

    // Input context field
    iInputContextField = CFepInputContextFieldJp::NewL( TRect(),
        UiLayout(), EPeninutWindowCtrlIdInputContextField );

    // Read resource
    TResourceReader resReader;
    CCoeEnv::Static()->CreateResourceReaderLC( resReader, R_PENINPUT_LAYOUT_ICF );
    ReadIcfInfo( resReader );
    CleanupStack::PopAndDestroy( 1 );
    // Transfer to headerPane
    headerPane->AddControlL( iInputContextField );

    iInputContextField->SetFocus(ETrue);

    // Add layout as event observer
    iInputContextField->AddEventObserver( UiLayout() );
    }

