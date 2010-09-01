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
* Description:  common layout window for UI interface of VKB and HWR : japanese
*
*/


// System includes
#include <peninputcommonlayout.h>   // many includes
#include <peninputinputcontextfield.h>
#include <AknFepGlobalEnums.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <peninputrepeatbutton.h>   // CAknFepCtrlEventButton,CAknFepCtrlRepeatButton
#include <peninputdataconverter.h>

// User includes
#include "peninputeventjp.h"  // EPeninutWindowCtrlIdCandidatePopupWindow, ...
#include "peninputlayoutwindowjp.h"
#include "peninputjapaneseview.h"
#include "peninputcontextfieldjp.h"
#include "peninputjapanesecandidatewnd.h"
#include "peninputjapanesepredictivewnd.h"
#include "peninputjapanesecandidatelistbox.h"
#include "peninputjapanesepredictivelistbox.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPeninputLayoutWindowJp::~CPeninputLayoutWindowJp
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C CPeninputLayoutWindowJp::~CPeninputLayoutWindowJp()
    {
    delete iViewCandidate;
    delete iViewPrediction;
    delete iViewCharacterRange;
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindowJp::HandleControlEvent
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindowJp::HandleControlEvent(TInt aEventType,
                                          CFepUiBaseCtrl* aCtrl,
                                          const TDesC& aEventData)
    {
    TInt* data = ( TInt* ) aEventData.Ptr();
    switch (aEventType)
        {
        case EPeninputLayoutEventMovePosition:
            {
            TSize shadowSize = ShadowRect().Size();
            if (iOpened && shadowSize.iWidth && shadowSize.iHeight)  // check if ready.
                {
                TPoint tl = UiLayout()->Position();
                if ((tl.iX || tl.iY) &&
                    !CPeninputDataConverter::AnyToInt(iLayoutContext->RequestData(EAkninputDataTypeSizeChanging)) )
                    {
                    ChangeLayoutPosition(EPeninputPositionChangeTlJustify);
                    ICFDisableRegionUpdating();
                    }
                TInt style = *( ( TUint16* ) aEventData.Ptr() );
                if (EPeninputPositionChangeDataQuery != style)
                    {
                    style = EPeninputPositionChangeBrJustify;
                    }
                ChangeLayoutPosition(style);
                ICFDisableRegionUpdating();
                }
            }
            break;
        case EEventSetDimmedSctBtn:
            {
            CAknFepCtrlEventButton* sctBtn = static_cast<CAknFepCtrlEventButton*>(iCtrlPool->Control(EPeninutWindowCtrlIdSymbolBtn));
            if (sctBtn)
                {
                TBool dimmFlag = (*data)? ETrue : EFalse;
                sctBtn->SetDimmed(dimmFlag);
                }
            }
            break;
        default:
            CPeninputLayoutWindow::HandleControlEvent(aEventType,aCtrl,aEventData);
            break;
        }
    }

// --------------------------------------------------------------------------
// CPeninputLayoutWindowJp::CreateAllControls
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindowJp::CreateAllControlsL()
    {

    AddCandidateWindowL();

    // befor predictivewnd, handle pointer event
    CAknFepCtrlEventButton* upButton = AddButtonL(
                EPeninutWindowCtrlIdScrollUpBtn,
                EPeninputLayoutEventScrollUp,
                GetScrollUpBtnResId(),
                EKeySpace, // dummy
                ETrue);

    // befor predictivewnd, handle pointer event
    CAknFepCtrlEventButton* downButton = AddButtonL(
                EPeninutWindowCtrlIdScrollDownBtn,
                EPeninputLayoutEventScrollDown,
                GetScrollDownBtnResId(),
                EKeySpace, // dummy
                ETrue);

    AddPredictiveWindowL();

    iPredictiveWnd->SetButtons(upButton, downButton);

    AddCharacterRangeWindowL();  // Hwr only

    }

// --------------------------------------------------------------------------
// CPeninputLayoutWindowJp::SetControlsFont
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindowJp::SetControlsFont()
    {
    TAknTextLineLayout textPaneTextLayout;

    if (EPluginInputModeVkb == iLayoutContext->LayoutType())
        {
        textPaneTextLayout = 
            AknLayoutScalable_Avkon::fep_vkb_top_text_pane_t1().LayoutLine();
        }
    else
        {
        textPaneTextLayout = 
            AknLayoutScalable_Avkon::fep_hwr_top_text_pane_t1().LayoutLine();
        }

    CFepInputContextField* icf = static_cast<CFepInputContextField*>
        ( Control( EPeninutWindowCtrlIdInputContextField ) );

    icf->SetFont( AknLayoutUtils::FontFromId
        ( textPaneTextLayout.iFont, NULL ) );
    TInt UnitHeight = *( static_cast<TInt*>( iLayoutContext->RequestData
                    ( EPeninputDataTypeUnitHeight ) ) );
    TInt UnitWidth = *( static_cast<TInt*>( iLayoutContext->RequestData
                    ( EPeninputDataTypeUnitWidth ) ) );

    // call listbox->SizeChanged()
    iCandidateWnd->UpdateMaximumRows(
                              UnitHeight
                             ,(EPluginInputModeVkb == iLayoutContext->LayoutType())? KPENINPUT_LAYOUT_VKB_POPUP_MAX_ROW_COUNT:KPENINPUT_LAYOUT_HWR_POPUP_MAX_ROW_COUNT);

    if (iCharacterRangeWnd)
        {
        // call listbox->SizeChanged()
        iCharacterRangeWnd->UpdateMaximumRows(UnitHeight,KPENINPUT_LAYOUT_HWR_POPUP_MAX_ROW_COUNT);
        }

    iPredictiveWnd->SetLineCount(KPENINPUT_LAYOUT_PREDICTIONPANE_ROW_COUNT);

    // call listbox->SizeChanged()
    iPredictiveWnd->SizeChanged(Rect().Size().iWidth, UnitWidth, UnitHeight);
    }

// -----------------------------------------------------------------------------
// CPeninputLayoutWindowJp:::ConstructFromResourceL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindowJp::ConstructFromResourceL()
    {
    CPeninputLayoutWindow::ConstructFromResourceL();
    if (iCandidateWnd && iCandidateWnd->ListBox() && iCandidateWnd->ListBox()->ScrollBarFrame())
        {
        CAknDoubleSpanScrollBar* scrollBar =
            static_cast<CAknDoubleSpanScrollBar*>(iCandidateWnd->ListBox()->ScrollBarFrame()->GetScrollBarHandle(CEikScrollBar::EVertical));
        if (scrollBar)
            {
            scrollBar->HandleResourceChange(KAknsMessageSkinChange);
            }
        }
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindowJp::HandlePointerDownEventL
// Handle pointer down event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//      
EXPORT_C CFepUiBaseCtrl* CPeninputLayoutWindowJp::HandlePointerDownEventL(const TPoint& aPoint)
    {
    CFepUiBaseCtrl* ctrl = iCandidateWnd->HandlePointerDownEventL(aPoint);
    if (!ctrl && iCharacterRangeWnd)
        {
        ctrl = iCharacterRangeWnd->HandlePointerDownEventL(aPoint);
        }
    if (!ctrl)
        {
        ctrl = CAknFepCtrlBaseWindow::HandlePointerDownEventL(aPoint);
        }
    return ctrl;
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindowJp::HandlePointerUpEventL
// Handle pointer up event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C CFepUiBaseCtrl* CPeninputLayoutWindowJp::HandlePointerUpEventL(const TPoint& aPoint)
    {
    CFepUiBaseCtrl* ctrl = iCandidateWnd->HandlePointerUpEventL(aPoint);
    if (!ctrl && iCharacterRangeWnd)
        {
        ctrl = iCharacterRangeWnd->HandlePointerUpEventL(aPoint);
        }
    if (!ctrl)
        ctrl = CPeninputLayoutWindow::HandlePointerUpEventL(aPoint);
    return ctrl;
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindowJp::HandlePointerMoveEventL
// Handle pointer move event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CFepUiBaseCtrl* CPeninputLayoutWindowJp::HandlePointerMoveEventL(const TPoint& aPoint)
    {
    CFepUiBaseCtrl* ctrl = iCandidateWnd->HandlePointerMoveEventL(aPoint);
    if (!ctrl && iCharacterRangeWnd)
        {
        ctrl = iCharacterRangeWnd->HandlePointerMoveEventL(aPoint);
        }
    if (!ctrl)
        ctrl = CPeninputLayoutWindow::HandlePointerMoveEventL(aPoint);
    return ctrl;
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindowJp::CandidateWindow
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CPeninputJapaneseCandidateWnd* CPeninputLayoutWindowJp::CandidateWindow()
    {
    return iCandidateWnd;
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindowJp::PredictiveWindow
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CPeninputJapanesePredictiveWnd* CPeninputLayoutWindowJp::PredictiveWindow()
    {
    return iPredictiveWnd;
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindowJp::CharacterRangeWindow
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CPeninputJapaneseCandidateWnd* CPeninputLayoutWindowJp::CharacterRangeWindow()
    {
    return iCharacterRangeWnd;
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindowJp::InputContextField
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CFepInputContextFieldJp* CPeninputLayoutWindowJp::InputContextField()
    {
    return static_cast<CFepInputContextFieldJp*>(Control(EPeninutWindowCtrlIdInputContextField));
    }

// --------------------------------------------------------------------------
// CPeninputLayoutWindowJp::Opened
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindowJp::Opened()
    {
    iOpened = ETrue;
    }

// --------------------------------------------------------------------------
// CPeninputLayoutWindowJp::Closed
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindowJp::Closed()
    {
    iOpened = EFalse;
    }

// --------------------------------------------------------------------------
// CPeninputLayoutWindowJp::ICFDisableRegionUpdating
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindowJp::ICFDisableRegionUpdating()
    {
    CFepInputContextField* icf = static_cast<CFepInputContextField*>
        ( Control( EPeninutWindowCtrlIdInputContextField ) );

    icf->OnDeActivate();
    }

// --------------------------------------------------------------------------
// CPeninputLayoutWindowJp::ICFEnableRegionUpdating
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindowJp::ICFEnableRegionUpdating()
    {
    CFepInputContextField* icf = static_cast<CFepInputContextField*>
        ( Control( EPeninutWindowCtrlIdInputContextField ) );

    icf->OnActivate();
    }

// ------------------------------------------------------------------------
// CPeninputLayoutWindow::CPeninputLayoutWindow
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C CPeninputLayoutWindowJp::CPeninputLayoutWindowJp( 
    CFepUiLayout* aUiLayout, MPeninputLayoutContext* aLayoutContext )
    : CPeninputLayoutWindow( aUiLayout, aLayoutContext ),
    iOpened(EFalse)
    { 
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindowJp::AddButtonL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CAknFepCtrlEventButton* CPeninputLayoutWindowJp::AddButtonL(
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

    if ( button )
        {
        CleanupStack::PushL( button );
        // Read resource
        TResourceReader reader;

        CCoeEnv::Static()->CreateResourceReaderLC( reader, aResId );
        button->SetResourceId( aResId );
        button->ConstructFromResourceL();
        // Pop and destroy reader
        CleanupStack::PopAndDestroy( 1 );

        AddControlL(button);
        CleanupStack::Pop( button );
        }
    return button;
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindowJp::AddCandidateWindowL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputLayoutWindowJp::AddCandidateWindowL()
    {
    iViewCandidate = new(ELeave) CPeninputJapaneseView;
    iViewCandidate->ConstructL();
    iCandidateWnd = CPeninputJapaneseCandidateWnd::NewL(UiLayout(), EPeninutWindowCtrlIdCandidatePopupWindow, iViewCandidate);
    iCandidateWnd->AddEventObserver(UiLayout());
    AddControlL(iCandidateWnd);
    iCandidateWnd->SetParent(NULL);
    iViewCandidate->SetListBox(iCandidateWnd->ListBox());
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindowJp::AddPredictiveWindowL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputLayoutWindowJp::AddPredictiveWindowL()
    {
    iViewPrediction = new(ELeave) CPeninputJapaneseView;
    iViewPrediction->ConstructL();
    iPredictiveWnd = CPeninputJapanesePredictiveWnd::NewL(UiLayout(), EPeninutWindowCtrlIdPredictionPane, iViewPrediction);
    iPredictiveWnd->AddEventObserver(UiLayout());
    AddControlL(iPredictiveWnd);
    iViewPrediction->SetListBox(iPredictiveWnd->ListBox());
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindowJp::AddCandidateWindowL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputLayoutWindowJp::AddCharacterRangeWindowL()
    {
    if (EPluginInputModeVkb != iLayoutContext->LayoutType())
        {
        iViewCharacterRange = new(ELeave) CPeninputJapaneseView;
        iViewCharacterRange->ConstructL();
        iCharacterRangeWnd = CPeninputJapaneseCandidateWnd::NewL(UiLayout(), EPeninutWindowCtrlIdCharacterRangePopupWindow, iViewCharacterRange);
        iCharacterRangeWnd->AddEventObserver(UiLayout());
        AddControlL(iCharacterRangeWnd);
        iCharacterRangeWnd->SetParent(NULL);
        iViewCharacterRange->SetListBox(iCharacterRangeWnd->ListBox());
        }
    }
