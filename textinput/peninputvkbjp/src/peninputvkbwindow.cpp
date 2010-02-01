/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  vkb main window area implementation
*
*/


// system includes
#include <peninputvkbwindowjp.rsg>
#include <peninputvkbwindowconfiginfo_32.rsg>

#include <peninputcontextfieldjp.h>
#include <peninputcommonlayout.h>
#include <peninputrepeatbutton.h>
#include <peninputrangebar.h>
#include <peninputdataconverter.h>
#include <peninputclientlayoutinfo.h>
#include <AknIconUtils.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <peninputjapanesepredictivewnd.h>
#include <aknfeppeninputenums.h>    //command from fep or IME to plugin. for vkb/hwr
#include <peninputcommonlayoutglobalenum.h>  // EPeninputLayoutEventEnter
#include <peninputeventjp.h>

// user includes
#include "peninputvkb.hrh"      // EPeninputLayoutVkbEventResetShift
#include "peninputvkbwindow.h"
#include "peninputvkbctrlextjp.h"

// Constants
_LIT( KVkbWindowResourceFile,
      "z:\\resource\\plugins\\peninputvkbwindowjp.RSC" );
_LIT( KConfigurationResourceFile,
      "z:\\resource\\plugins\\peninputvkbwindowconfiginfo_32.RSC" );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPeninputVkbWindow::NewL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPeninputVkbWindow* CPeninputVkbWindow::NewL(CFepUiLayout* aUiLayout,
                                         MPeninputLayoutContext* aLayoutContext)
    {
    CPeninputVkbWindow* self = CPeninputVkbWindow::NewLC(aUiLayout, aLayoutContext);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CPeninputVkbWindow::NewLC
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPeninputVkbWindow* CPeninputVkbWindow::NewLC(CFepUiLayout* aUiLayout,
                                         MPeninputLayoutContext* aLayoutContext)
    {
    CPeninputVkbWindow* self = new (ELeave) CPeninputVkbWindow(aUiLayout, aLayoutContext);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CPeninputVkbWindow::~CPeninputVkbWindow
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPeninputVkbWindow::~CPeninputVkbWindow()
    {
    }

// ---------------------------------------------------------------------------
// CPeninputVkbWindow::HandleControlEvent
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputVkbWindow::HandleControlEvent(TInt aEventType,
                                          CFepUiBaseCtrl* aCtrl,
                                          const TDesC& aEventData)
    {

    TInt* data = (TInt*) aEventData.Ptr();
    switch (aEventType)
        {
        case EPeninputLayoutVkbEventResetShift:
            {
            HandleShiftBtnClicked();
            }
            break;
        case EPeninputLayoutEventSetPermittedRange:  // from datamgr
            {  // [Ref]CPeninputLayoutWindow::HandleControlEvent
            // Set range of range bar component
            CFepUiBaseCtrl* bar =
                iCtrlPool->Control( EPeninutWindowCtrlIdRangeBar );
            if ( bar )
                {
                CAknFepCtrlRangeBar* rangebar =
                    static_cast<CAknFepCtrlRangeBar*>( bar );
                rangebar->SetPermittedRanges( *data );
                }
            }
            break;
        default:
            //Call this function in base class
            CPeninputLayoutWindowJp::HandleControlEvent(aEventType,aCtrl,aEventData);
            break;
        }
    }

// --------------------------------------------------------------------------
// CPeninputVkbWindow::GetWindowConfigResId
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TInt CPeninputVkbWindow::GetWindowConfigResId()
    {
    return R_PENINPUT_LAYOUT_CONFIG_INFO;
    }

// --------------------------------------------------------------------------
// CPeninputVkbWindow::GetWindowResId
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TInt CPeninputVkbWindow::GetWindowResId()
    {
    return R_PENINPUT_LAYOUT_VKB_WINDOW;
    }

// --------------------------------------------------------------------------
// CPeninputVkbWindow::GetWindowConfigResFileName
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
const TDesC& CPeninputVkbWindow::GetWindowConfigResFileName(
    TInt /* aLangID */ )    // LangId : 32 fixed
    {
    return KConfigurationResourceFile();
    }

// --------------------------------------------------------------------------
// CPeninputVkbWindow::GetWindowResFileName
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
const TDesC& CPeninputVkbWindow::GetWindowResFileName()
    {
    return KVkbWindowResourceFile();
    }

// --------------------------------------------------------------------------
// CPeninputVkbWindow::ChangeUnitSize
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
const TRect CPeninputVkbWindow::ChangeUnitSize()
    {
    TAknWindowLineLayout appWnd;
    TAknWindowLineLayout mainPane;
    TAknWindowLineLayout wndLayout;
    TAknWindowLineLayout unitGrid;
    TAknLayoutRect mainPaneRect;
    TAknLayoutRect wndRect;
    TAknLayoutRect unitRect;

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

    mainPaneRect.LayoutRect( appWnd.Rect(), mainPane );

    TInt clientLayoutId = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeClientLayout ) );
    if (EPeninputClientLayoutLatin == clientLayoutId)
        {
        wndLayout = AknLayoutScalable_Avkon::popup_fep_vkb_window(0).LayoutLine();
        unitGrid = AknLayoutScalable_Avkon::aid_size_cell_vkb(0).LayoutLine();
        }
    else if (!iPredictiveWnd->Hiden())
        {
        wndLayout = AknLayoutScalable_Avkon::popup_fep_vkb_window(5).LayoutLine();
        unitGrid = AknLayoutScalable_Avkon::aid_size_cell_vkb(7).LayoutLine();
        }
    else
        {
        wndLayout = AknLayoutScalable_Avkon::popup_fep_vkb_window(4).LayoutLine();
        unitGrid = AknLayoutScalable_Avkon::aid_size_cell_vkb(6).LayoutLine();
        }
    wndRect.LayoutRect(mainPaneRect.Rect(), wndLayout);

    unitRect.LayoutRect(wndRect.Rect(), unitGrid);

    TRect vkbWndRect = TRect(0,
                             0,
                             wndRect.Rect().Width(),
                             wndRect.Rect().Height());

    TInt unitWidth = unitRect.Rect().Width();
    TInt unitHeight = unitRect.Rect().Height();

    iLayoutContext->SetData( EPeninputDataTypeUnitWidth, &unitWidth );
    iLayoutContext->SetData( EPeninputDataTypeUnitHeight, &unitHeight );

    return vkbWndRect;
    }

// --------------------------------------------------------------------------
// CPeninputVkbWindow::ChangeUnitSize
// (other items were commented in a header)
// --------------------------------------------------------------------------
void CPeninputVkbWindow::ChangeClientSize()
    {
    TAknTextLineLayout keypadText;

    TInt unitWidth = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeUnitWidth ) );
    TInt unitHeight = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeUnitHeight ) );

    keypadText =
        AknLayoutScalable_Avkon::cell_vkb_keypad_pane_t1().LayoutLine();

    // 1.Read data from scalable
    TInt unitCharWidth = unitWidth - 2 * keypadText.il;
    TInt unitCharHeight = unitHeight - 2 * keypadText.il;
    TPoint charLeftTop = TPoint( keypadText.il, keypadText.il );

    // Modify keypad font
    const CFont* font = NULL;
    font = AknLayoutUtils::FontFromId( keypadText.iFont, NULL);
    iVkbCtrl->SetFont( const_cast<CFont*> (font) );

    TInt clientLayoutId = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeClientLayout ) );
    CPeninputClientLayoutInfo* clientLayout =
        ConfigInfo()->FindClientLayoutInfo( clientLayoutId );

    if ( clientLayout )
        {
        // 2.Change self rect
        TInt width = clientLayout->Columns() * unitWidth;
        TInt height = clientLayout->Rows() * unitHeight;
        TRect rect = TRect( ClientPane()->Rect().iTl,
                            TSize( width, height ) );

        ClientPane()->SetRect( rect );
        // 3.Change the postion and size of controls in current layout
        ReorganizeControls( clientLayout->LayoutID(), EFalse );
        }

    // 4.Recalculate all keys in all vkblayouts
    RPointerArray<CPeninputVkbLayoutInfo>& vkbList =
        ConfigInfo()->VkbLayoutInfoList();
    const TInt count = vkbList.Count();

    for ( TInt i = 0; i < count; i++ )
        {
        vkbList[i]->ResetSize( unitWidth, unitHeight, unitCharWidth,
            unitCharHeight, charLeftTop, font );
        }

    TInt vkbLayoutid = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeVkbLayout ) );
    CPeninputVkbLayoutInfo* vkbLayout =
        ConfigInfo()->FindVkbLayoutInfo( vkbLayoutid );

    // Calculate current vkblayout according current layout
    if ( vkbLayout )
        {
        TRect rect = iVkbCtrl->Rect();

        rect.SetSize( vkbLayout->GetRealSize( unitWidth, unitHeight ) );
        iVkbCtrl->SetRect( rect );
        // Make the true draw
        UpdateArea( iVkbCtrl->Rect(), EFalse );
        }
    }

// --------------------------------------------------------------------------
// CPeninputVkbWindow::CreateAllControls
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputVkbWindow::CreateAllControlsL()
    {
    CPeninputLayoutWindowJp::CreateAllControlsL();

    if (iInputContextField)
        {
        HeaderPane()->RemoveControl(static_cast<CFepUiBaseCtrl*>(iInputContextField));
        iInputContextField = NULL;
        }
    AddContextFieldJpL();

    // Range bar
    AddRangeBarL();

    // Vkb control
    TAknTextLineLayout keypadText;

    keypadText =
        AknLayoutScalable_Avkon::cell_vkb_keypad_pane_t1().LayoutLine();
    iVkbCtrl = CPeninputVkbCtrlExtJp::NewL( TRect(), UiLayout(),
        EPeninutWindowCtrlIdVkbCtrl,
        AknLayoutUtils::FontFromId( keypadText.iFont, NULL ) );

    iVkbLayout->SetVkbKeyboard( iVkbCtrl );
    iCtrlPool->AddControl( iVkbCtrl );

    // Backspace button  3 Unit
    AddButtonL( EPeninutWindowCtrlIdClear1Btn,
                EPeninputLayoutEventBack,
                R_PENINPUT_LAYOUT_VKB_CLEAR_1,
                EKeyBackspace,
                ETrue );

    // Backspace button  1 Unit
    AddButtonL( EPeninutWindowCtrlIdClear2Btn,
                EPeninputLayoutEventBack,
                R_PENINPUT_LAYOUT_VKB_CLEAR_2,
                EKeyBackspace,
                ETrue );

    // Enter button
    AddButtonL( EPeninutWindowCtrlIdEnterBtn,
                EPeninputLayoutEventEnter,
                R_PENINPUT_LAYOUT_VKB_ENTER,
                EKeyEnter );

    // Tab button
    AddButtonL( EPeninutWindowCtrlIdTabBtn,
                EPeninputLayoutEventTab,
                R_PENINPUT_LAYOUT_VKB_TAB,
                EKeyTab );

    // Capslock button
    AddButtonL( EPeninutWindowCtrlIdCapslockBtn,
                EPeninputLayoutEventCapslock,
                R_PENINPUT_LAYOUT_VKB_CAPSLOCK );

    // Shift button
    AddButtonL( EPeninutWindowCtrlIdShiftBtn,
                EPeninputLayoutEventShift,
                R_PENINPUT_LAYOUT_VKB_SHIFT );

    // Space button
    AddButtonL( EPeninutWindowCtrlIdSpaceBtn,
                EPeninputLayoutEventSpace,
                R_PENINPUT_LAYOUT_VKB_SPACE,
                EKeySpace );

    // SCT button
    AddButtonL( EPeninutWindowCtrlIdSymbolBtn,
                EPeninputLayoutEventSct,
                R_PENINPUT_LAYOUT_VKB_SCT );

    // Switch to hwr button
    AddButtonL( EPeninutWindowCtrlIdSwitchToHwrBtn,
                EPeninputLayoutEventToHwr,
                R_PENINPUT_LAYOUT_VKB_HWR );
    for ( TInt i = 0; i < iCtrlPool->ControlCount(); i++ )
        {
        if ( iCtrlPool )
            {
            iCtrlPool->ControlByIndex( i )->AddEventObserver( UiLayout() );
            }
        }
    }

// --------------------------------------------------------------------------
// CPeninputVkbWindow::PopupChoiceList
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputVkbWindow::PopupChoiceList()
    {   // nop
    }

// --------------------------------------------------------------------------
// CPeninputVkbWindow::ReorganizeControls
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputVkbWindow::ReorganizeControls( TInt aClientLayoutId, TBool aNeedReset )
    {
    TInt unitWidth = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeUnitWidth ) );
    TInt unitHeight = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeUnitHeight ) );

    // First remove all controls in the client pane, but don't delete them
    CFepUiBaseCtrl* activCtrl = ClientPane()->ActiveControl();
    if (activCtrl)
        {    // we shuld remove active controls, because CControlGroup::Draw()  draw active controls.[for japanese]
        ClientPane()->RemoveControl(activCtrl);
        }
    ( const_cast<RPointerArray<CFepUiBaseCtrl>&>
        ( ClientPane()->ControlList() ) ).Reset();
    iVkbCtrl->SetRect( TRect( ClientPane()->Rect().iTl, TSize( 0, 0 ) ) );

    if (aNeedReset)
        {
        iVkbCtrl->Reset();
        }

    // Then add controls according to the configuration of clientlayout
    CPeninputClientLayoutInfo* clientLayout =
        ConfigInfo()->FindClientLayoutInfo( aClientLayoutId );
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
            rect.iTl = point + TPoint
                ( controlList[i]->BeginColumn() * unitWidth,
                  controlList[i]->BeginRow() * unitHeight
                );
            cols = controlList[i]->EndColumn()
                - controlList[i]->BeginColumn();
            rows = controlList[i]->EndRow() - controlList[i]->BeginRow();
            rect.SetSize( TSize( cols * unitWidth, rows * unitHeight ) );

            if ( ( ctrl->ControlType() | ECtrlButton ) == ECtrlButton )
                {
                ( static_cast<CAknFepCtrlButton*> (ctrl) )->SizeChanged
                    ( rect, ETrue );
                }
            else if ( ctrl->ControlId() == EPeninutWindowCtrlIdVkbCtrl )
                {
                ( static_cast<CPeninputVkbCtrlExt*> (ctrl) )->SizeChanged
                    ( unitWidth, unitHeight, rect.iTl );
                }
            else if ( ctrl->ControlId() == EPeninutWindowCtrlIdRangeBar )
                {
                rect = TRect( rect.iTl, TSize( unitWidth, unitHeight ) );
                ( static_cast<CAknFepCtrlRangeBar*> (ctrl) )->SizeChanged
                    ( rect );
                }
            AddNotOwnedControl( ctrl );
            }
        }
    }

// --------------------------------------------------------------------------
// CPeninputVkbWindow::GetScrollUpBtnResId
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TInt CPeninputVkbWindow::GetScrollUpBtnResId()
    {
    return R_PENINPUT_LAYOUT_VKB_SCROLL_UP;
    }

// --------------------------------------------------------------------------
// CPeninputVkbWindow::GetScrollDownBtnResId
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TInt CPeninputVkbWindow::GetScrollDownBtnResId()
    {
    return R_PENINPUT_LAYOUT_VKB_SCROLL_DOWN;
    }

// --------------------------------------------------------------------------
// CPeninputVkbWindow::DoCaseChange
// (other items were commented in a header)
// --------------------------------------------------------------------------
//[Ref]CPeninputGenericVkbWindow::DoCaseChange
// ECmdPenInputCase
void CPeninputVkbWindow::DoCaseChange( TInt aNewCase )
    {
    TInt curCase = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeCase ) );
    TInt curRange = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeCurrentRange ) );

    if (aNewCase != curCase && curRange == ERangeEnglish)
        {
        // Set shift and caplock button
        TInt shfit;
        TInt capslock;

        CPeninputDataConverter::ShiftCapslockByCase( aNewCase,
            shfit, capslock );

        iLayoutContext->SetData( EPeninputDataTypeShiftDown, &shfit );
        ChangeButtonStatus( shfit, EPeninutWindowCtrlIdShiftBtn );
        iLayoutContext->SetData( EPeninputDataTypeCapslockDown, &capslock );
        ChangeButtonStatus( capslock, EPeninutWindowCtrlIdCapslockBtn );
        iLayoutContext->SetData( EPeninputDataTypeCase, &aNewCase );

        TInt vkblayout = ConfigInfo()->ShiftCapsSingleVkbLayoutByCase
            ( curRange, aNewCase );

        ChangeVkbLayout( vkblayout );
        }
    }

// --------------------------------------------------------------------------
// CPeninputVkbWindow::DoRangeChange
// (other items were commented in a header)
// --------------------------------------------------------------------------
void CPeninputVkbWindow::DoRangeChange()
    {
    TInt currentRange = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeCurrentRange ) );
    switch (currentRange)
        {
        case ERangeHiraganaKanji:
        case ERangeHiraganaOnly:
            iCurrentJapanese = ERangeHiraganaKanji;
            break;
        case ERangeKatakana:
        case ERangeFullWidthKatakana:
            iCurrentJapanese = ERangeKatakana;
            break;
        default:
            {
            TInt permittedRange = CPeninputDataConverter::AnyToInt
                ( iLayoutContext->RequestData( EPeninputDataTypePermittedRange ) );
            if (permittedRange & (ERangeHiraganaKanji | ERangeKatakana | ERangeFullWidthKatakana))
                {
                if (iCurrentJapanese == ERangeHiraganaKanji
                        && !(permittedRange & (ERangeHiraganaKanji | ERangeHiraganaOnly)))
                    {
                    iCurrentJapanese = ERangeKatakana;
                    }
                else if (iCurrentJapanese == ERangeKatakana
                        && !(permittedRange & (ERangeKatakana | ERangeFullWidthKatakana)))
                    {
                    iCurrentJapanese = ERangeHiraganaKanji;
                    }
                }
            }
            break;
        }
    if ( iRangeBar )
        {
        TInt hideRange  = currentRange;
        switch (currentRange)
            {
            case ERangeFullWidthKatakana:
                hideRange = ERangeKatakana;
                break;
            case ERangeFullWidthEnglish:
                hideRange = ERangeEnglish;
                break;
            case ERangeFullWidthNumeric:
                hideRange = ERangeNumber;
                break;
            case ERangeHiraganaOnly:
                hideRange = ERangeHiraganaKanji;
                break;
            default:
                break;
            }
        iRangeBar->ActiveRange(hideRange);

        if (currentRange != ERangeHiraganaKanji
                && currentRange != ERangeHiraganaOnly
                && currentRange != ERangeFullWidthKatakana
                && currentRange != ERangeKatakana)
            {
            if (iCurrentJapanese == ERangeHiraganaKanji)
                {
                iRangeBar->HideRange(ERangeKatakana);
                }
            else
                {
                iRangeBar->HideRange(ERangeHiraganaKanji);
                }
            }
        else if (ERangeHiraganaOnly == currentRange)
            {
            iRangeBar->SetPermittedRanges(0);  // dimmed all buttons
            }
        }
    UiLayout()->Draw();  // after rangebar changed, we must draw all.
    UpdateArea(Rect(), EFalse);
    }

// ---------------------------------------------------------------------------
// CPeninputVkbWindow::SetDimmed
// Dim KeyBoard
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputVkbWindow::SetDimmed(TBool aDimFlag)
    {
    if (iVkbCtrl)
        {
        iVkbCtrl->SetDimmed(aDimFlag);
        if (aDimFlag)
            {
            iVkbCtrl->Draw();
            UpdateArea(iVkbCtrl->Rect(), EFalse);
            }
        }
    }

// -----------------------------------------------------------------------------
// CPeninputVkbWindow::Hide
// Hide the control pool and all it's controls
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPeninputVkbWindow::Hide(TBool aFlag)
    {
    CPeninputLayoutWindowJp::Hide(aFlag);

    // controls used/not used according to clientlauout
    CFepUiBaseCtrl* button = iCtrlPool->Control(EPeninutWindowCtrlIdCapslockBtn);
    if (button)
        {
        button->SetReady(!aFlag);
        }
    button = iCtrlPool->Control(EPeninutWindowCtrlIdShiftBtn);
    if (button)
        {
        button->SetReady(!aFlag);
        }
    button = iCtrlPool->Control(EPeninutWindowCtrlIdClear1Btn);
    if (button)
        {
        button->SetReady(!aFlag);
        }
    button = iCtrlPool->Control(EPeninutWindowCtrlIdClear2Btn);
    if (button)
        {
        button->SetReady(!aFlag);
        }
    }

// ---------------------------------------------------------------------------
// CPeninputVkbWindow::SetEnableSettingBtnJp
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//     
void CPeninputVkbWindow::SetEnableSettingBtnJp(TBool aEnable)
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

// -----------------------------------------------------------------------------
// CPeninputVkbWindow::SetSizeToRangeButtons
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPeninputVkbWindow::SetSizeToRangeButtons()
    {
    TInt currentRange = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeCurrentRange ) );
    if (!currentRange)   // CurrentRange is not yet set.
        {
        TInt unitWidth = CPeninputDataConverter::AnyToInt
            ( iLayoutContext->RequestData( EPeninputDataTypeUnitWidth ) );
        TInt unitHeight = CPeninputDataConverter::AnyToInt
            ( iLayoutContext->RequestData( EPeninputDataTypeUnitHeight ) );

        CFepUiBaseCtrl* ctrl = iCtrlPool->Control( EPeninutWindowCtrlIdRangeBar );
        TRect rect = TRect( TPoint(0,0), TSize( unitWidth, unitHeight ) );
        static_cast<CAknFepCtrlRangeBar*> (ctrl)->SizeChanged(rect);
        }
    }

// ---------------------------------------------------------------------------
// CPeninputVkbWindow::CPeninputVkbWindow
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPeninputVkbWindow::CPeninputVkbWindow(CFepUiLayout* aUiLayout,
                                         MPeninputLayoutContext* aLayoutContext)
    : CPeninputLayoutWindowJp( aUiLayout, aLayoutContext ),
    iCurrentJapanese(ERangeHiraganaKanji)
    {
    }

// --------------------------------------------------------------------------
// CPeninputVkbWindow::AddButtonL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CAknFepCtrlEventButton* CPeninputVkbWindow::AddButtonL(
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
        // Create the button from resource id
        button->SetResourceId( aResId );
        button->ConstructFromResourceL();

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
// CPeninputVkbWindow::AddRangeBarL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputVkbWindow::AddRangeBarL()
    {
    // Range bar
    iRangeBar = CAknFepCtrlRangeBar::NewL( UiLayout(),
        EPeninutWindowCtrlIdRangeBar,
        CAknFepCtrlRangeBar::EBtnGroupHorizontal );

    // Set event id
    iRangeBar->SetEventIdForRange( EPeninputLayoutEventRange );

    iCtrlPool->AddControl( iRangeBar );
    }

// ---------------------------------------------------------------------------
// CPeninputVkbWindow::SetWholeWindowRect
// [Ref]CPeninputLayoutWindow::SetWindowRect
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputVkbWindow::SetWholeWindowRect(const TRect& aRect)
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
// CPeninputVkbWindow::AddContextFieldJpL
// [ref]CPeninputLayoutWindow::AddContextFieldL( aResID )
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputVkbWindow::AddContextFieldJpL()
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

// End Of File
