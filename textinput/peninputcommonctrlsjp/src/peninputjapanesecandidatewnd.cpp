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
* Description:  Candidate list
*
*/


// INCLUDE FILES
#include <AknsDrawUtils.h>
#include <aknconsts.h>  // KStatusPaneBackgroundColor
#include <skinlayout.cdl.h>  // SkinLayout
#include <aknlayoutscalable_avkon.cdl.h>  // AknLayoutScalable_Avkon
#include <peninputcommonlayoutglobalenum.h>  // EPeninputLayoutEventRange
#include <peninputeventjp.h>  // EPeninputLayoutEventSelectItem
#include <peninputlayout.h>
#include <AknsBasicBackgroundControlContext.h> 

#include "peninputeventjp.h"
#include "peninputjapanesecandidatewnd.h"
#include "peninputjapanesecandidatelistbox.h"
#include "peninputcontextfieldjp.h"

// CONSTANTS
const TInt KMaxTitleLength = 32;

const TInt KMaxListRows = 7;
const TInt KMinListRows = 3;
const TInt KMaxListColumns = 6;
const TInt KMinListColumns = 3;

//default value for long press timer
const TInt KLongPressInterval = 600000;

//default value for repeat timer
const TInt KRepeatInterval = 100000;

/**
*  CPeninputJapaneseCandidateWnd class.
*
*/

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPeninputJapaneseCandidateWnd::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPeninputJapaneseCandidateWnd* CPeninputJapaneseCandidateWnd::NewL(CFepUiLayout* aUiLayout,
                                                               TInt aControlId, const CCoeControl* aParent)
    {
    CPeninputJapaneseCandidateWnd* self =
        new(ELeave) CPeninputJapaneseCandidateWnd(aUiLayout, aControlId);
    CleanupStack::PushL(self);
    self->ConstructL(aParent);
    CleanupStack::Pop(self);    // self
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputJapaneseCandidateWnd::~CPeninputJapaneseCandidateWnd
// Destructor
// -----------------------------------------------------------------------------
//
CPeninputJapaneseCandidateWnd::~CPeninputJapaneseCandidateWnd()
    {
    delete iListBox;
    CancelScrollBarRepeat();
    delete iLongPressTimer;
    delete iRepeatTimer;
    if(iAppGc)
        {
        CCoeEnv::Static()->SwapSystemGc(iAppGc);
        }
    }


// ---------------------------------------------------------------------------
// CFepLayoutPopupWnd::SetRect
// Set the pop up window's rect
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputJapaneseCandidateWnd::SetRect(const TRect& aRect)
    {
    iRect = aRect;
    if (iListBox->Parent())
        {
        TRect rect(aRect);
        rect.iTl = TPoint(0,0);  // parent is owning a window.
        iListBox->Parent()->SetRect(rect);
        }
    }

// -----------------------------------------------------------------------------
// CPeninputJapaneseCandidateWnd::SetCurrentItemIndex
// Specify the cursor position of a pop-up list.
// -----------------------------------------------------------------------------
//
EXPORT_C void CPeninputJapaneseCandidateWnd::SetPosition(const TPoint& aPos)
    {
    iPos = aPos;
    }

// -----------------------------------------------------------------------------
// CPeninputJapaneseCandidateWnd::SetIcf
// -----------------------------------------------------------------------------
//
void CPeninputJapaneseCandidateWnd::SetIcf(CFepInputContextFieldJp* aInputContextField)
    {
    iInputContextField = aInputContextField;
    }

// -----------------------------------------------------------------------------
// CPeninputJapaneseCandidateWnd::ShowCandidateListL
// Show Pop-up list
// -----------------------------------------------------------------------------
//
EXPORT_C void CPeninputJapaneseCandidateWnd::ShowPopupL(CDesCArrayFlat* aItems, TInt aIndex)
    {
    if(!Hiden() || aItems->Count() <= 0)
        {
        return;
        }

    // Set listitems.
    CTextListBoxModel* model =
        static_cast<CTextListBoxModel*>(iListBox->Model());
    model->SetItemTextArray(aItems);
    model->SetOwnershipType(ELbmDoesNotOwnItemArray);

    iMaximumColumns = MaximumColumns(model);
    iMaximumRows    = MaximumRows(model);
    iListBox->SetMaximumColumns(iMaximumColumns);
    iListBox->SetMaximumRows(iMaximumRows);

    TSize min = iListBox->MinimumSize();

    TAknWindowLineLayout scrollLineLayout = AknLayoutScalable_Avkon::scroll_pane_cp30().LayoutLine();
    TInt atom = AknLayoutScalable_Avkon::list_single_choice_list_pane(0).LayoutLine().iH;
    TInt margin = atom/KPENINPUT_LAYOUT_VALUE_2;
    TInt rightMargin = atom/KPENINPUT_LAYOUT_VALUE_4;

    if (iInputContextField) // candidate popup
        {
        TRect IcfRect = iInputContextField->Rect();
        iPos.iX = IcfRect.iTl.iX + IcfRect.Size().iWidth/KPENINPUT_LAYOUT_VALUE_6;
        iPos.iY = IcfRect.iBr.iY;
        }
    else
        {
        iPos.iX -= min.iWidth + rightMargin;
        iPos.iY -= min.iHeight + margin*KPENINPUT_LAYOUT_VALUE_2;
        }
    TRect rect(TRect(iPos,Rect().Size()));

    rect.iBr.iX = rect.iTl.iX + min.iWidth;
    if (iMaximumRows < iListBox->Model()->NumberOfItems())  // scrollbar showing
        {
        rect.iBr.iX +=scrollLineLayout.iW;
        }

    // Title Font
    TAknTextLineLayout choiceTextLayout =
            AKN_LAYOUT_TEXT_Candidate_selection_list_texts_Line_1;
    const CFont* font = AknLayoutUtils::FontFromId(choiceTextLayout.iFont, NULL);
    TInt fontSize = font->AscentInPixels();
    TInt titleHeight = (iInputContextField)? (fontSize+KPENINPUT_LAYOUT_VALUE_2) : 0;

    rect.iBr.iY = rect.iTl.iY +  min.iHeight + titleHeight;

    TRect windowRect = rect;
    TAknLayoutRect topLeft;
    topLeft.LayoutRect(windowRect, SkinLayout::Popup_windows_skin_placing__frame_general__Line_2());
    iTopLeft = TSize(topLeft.Rect().Size().iWidth/2, topLeft.Rect().Size().iHeight/2);
    rect.iBr.iX += (iTopLeft.iWidth*2);
    rect.iBr.iY += (iTopLeft.iHeight*2);


    TRect listRect;
    listRect = TRect(TPoint(rect.iTl.iX + iTopLeft.iWidth
                            ,rect.iTl.iY + titleHeight + iTopLeft.iHeight)
                            ,min);

    SetRect(rect);
    iListBox->SetRect(listRect);
    iListBox->SetCurrentItemIndex(aIndex);

    MAknsControlContext *cc = AknsDrawUtils::ControlContext( iListBox );
    if (cc)
        {
        CAknsBasicBackgroundControlContext *bcc =
            (CAknsBasicBackgroundControlContext*)cc;
        bcc->SetBitmap(KAknsIIDQsnFrPopupCenter);
        listRect.iBr.iX = rect.iBr.iX - iTopLeft.iWidth;
        bcc->SetRect(listRect);
        }

    iListBox->MakeVisible(ETrue);  // after iListBox->SetRect()
    if (iListBox->Parent() && !iListBox->Parent()->IsBackedUp())
        {
        iListBox->Parent()->ActivateL();
        }
    else
        {
        iListBox->ActivateL();
        }
    ReportEvent(EEventShowCandidateWnd);
    if (!iAppGc)
        {
        iAppGc = CCoeEnv::Static()->SwapSystemGc(static_cast<CWindowGc*>(BitGc()));
        }
    Display(TPoint(0,0)); // set lock area, CapturePonter(ETrue)
    UiLayout()->UnLockArea(Rect(),this);  // free lock area
    CapturePointer(EFalse);
    }

// -----------------------------------------------------------------------------
// CPeninputJapaneseCandidateWnd::HideWindow
// Hide Pop-up list
// -----------------------------------------------------------------------------
//
EXPORT_C void CPeninputJapaneseCandidateWnd::HidePopup()
    {
    if(!Hiden())
        {
        ReportEvent(EEventHideCandidateWnd);
        if(iAppGc)
            {
            CCoeEnv::Static()->SwapSystemGc(iAppGc);
            iAppGc = NULL;
            }
        iListBox->MakeVisible(EFalse);   // before CloseWindow()
        CloseWindow();
        }
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::CurrentItemIndexOfCandidate
// Return a selected candidate item number currently
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPeninputJapaneseCandidateWnd::CurrentItemIndex() const
    {
    if (iListBox->IsVisible())
        {
        return iListBox->CurrentItemIndex();
        }
    return -1;      // Disable window
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::UpdateMaximumRows
// Return a selected candidate item number currently
// called by CPeninputLayoutWindowJp::SetControlsFont()
// -----------------------------------------------------------------------------
//
void CPeninputJapaneseCandidateWnd::UpdateMaximumRows(TInt aUnitHeight, TInt aUnitCount)
    {
    iListBox->SizeChanged();
    iListBox->SetMaximumColumns(iMaximumColumns);
    iFontMaximumRows = KMaxListRows;
    while (KMinListRows < iFontMaximumRows)
        {
        iListBox->SetMaximumRows(iFontMaximumRows);
        TSize min = iListBox->MinimumSize();
        if (min.iHeight <= aUnitHeight*(aUnitCount-1))
            {
            break;
            }
        -- iFontMaximumRows;
        }
    }

// -----------------------------------------------------------------------------
// CPeninputJapaneseCandidateWnd::MoveToL
// Move a current item in an appointed direction
// -----------------------------------------------------------------------------
//
void CPeninputJapaneseCandidateWnd::MoveToL(TPeninputUICtrlScroll aDirection)
    {
    if (iListBox->IsVisible())
        {
        TInt code = EKeyNull;

        switch(aDirection)
            {
            case EScrollUp:
                {
                code = EKeyUpArrow;
                }
                break;

            case EScrollDown:
                {
                code = EKeyDownArrow;
                }
                break;

            default:
                break;
            }
        if (EKeyNull != code)
            {
            TKeyEvent keyEvent;
            keyEvent.iCode = code;
            keyEvent.iScanCode = 0;
            keyEvent.iModifiers = 0;
            iListBox->OfferKeyEventL(keyEvent, EEventKey);
            if (!Hiden())
                {
                ReDraw();
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// CPeninputJapaneseCandidateWnd::HitTest
// Get item whose region contains the point.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CPeninputJapaneseCandidateWnd::HitTest(const TPoint& aPoint)
    {
    if( !Rect().Contains(aPoint) )
        {
        return -1;
        }
    TRect listRect = iListBox->Rect();
    if (listRect.iBr.iX >= aPoint.iX)
        {
        return 1;  // ListBox Side
        }
    if (iListBox->ScrollBarFrame()->GetScrollBarHandle(CEikScrollBar::EVertical)
        && iListBox->ScrollBarFrame()->GetScrollBarHandle(CEikScrollBar::EVertical)->Rect().Contains(aPoint))
        {
        return 0;     // ScrollBar
        }
    return -2;
    }

// ---------------------------------------------------------------------------
// CPeninputJapaneseCandidateWnd::HandlePointerDownEventL
// Handle pointer down event.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CFepUiBaseCtrl* CPeninputJapaneseCandidateWnd::HandlePointerDownEventL(const TPoint& aPoint)
    {
    if (Hiden())
        return NULL;
    CFepLayoutPopupWnd::HandlePointerDownEventL(aPoint);  // set pointer down
    TInt index = HitTest(aPoint);
    if (0 > index)
        {
        SetPointerDown(EFalse);
        if (iInputContextField) // candidate popup
            {
            return (-1 == index)? NULL:this;
            }
        else
            {
            HidePopup();
            return this;
            }
        }
    else
        {
        TPointerEvent pEvent;
        pEvent.iType = TPointerEvent::EButton1Down;
        pEvent.iPosition = aPoint;
        pEvent.iModifiers = 0;
        pEvent.iParentPosition = TPoint(0,0);
        SendPointerEventL(pEvent, index, EFalse);
        if (!index) // on ScrollBar
            {
            iScrollBarPointerEvent = pEvent;
            iScrollBarPointerEvent.iType = TPointerEvent::EButton1Up;
            iLongPressTimer->SetTimer(KLongPressInterval);
            }
        }
    return this;
    }

// ---------------------------------------------------------------------------
// CPeninputJapaneseCandidateWnd::HandlePointerMoveEventL
// Handle pointer move event.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CFepUiBaseCtrl* CPeninputJapaneseCandidateWnd::HandlePointerMoveEventL(const TPoint& aPoint)
    {
    CancelScrollBarRepeat();
    CFepLayoutPopupWnd::HandlePointerMoveEventL(aPoint);  // nop
    if( PointerDown() && !Hiden())
        {
        TInt index = HitTest(aPoint);
        TPointerEvent pEvent;
        pEvent.iType = TPointerEvent::EDrag;
        pEvent.iPosition = aPoint;
        pEvent.iModifiers = 0;
        pEvent.iParentPosition = TPoint(0,0);
        SendPointerEventL(pEvent, index, ETrue);  // if to ScrollBar, Draw.
        return this;
        }
    return NULL;
    }

// ---------------------------------------------------------------------------
// CPeninputJapaneseCandidateWnd::HandlePointerUpEventL
// Handle pointer up event.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CFepUiBaseCtrl* CPeninputJapaneseCandidateWnd::HandlePointerUpEventL(const TPoint& aPoint)
    {
    CancelScrollBarRepeat();
    if( PointerDown()  && !Hiden())
        {
        CFepLayoutPopupWnd::HandlePointerUpEventL(aPoint);  // pointer down off
        TInt index = HitTest(aPoint);
        TPointerEvent pEvent;
        pEvent.iType = TPointerEvent::EButton1Up;
        pEvent.iPosition = aPoint;
        pEvent.iModifiers = 0;
        pEvent.iParentPosition = TPoint(0,0);
        SendPointerEventL(pEvent, index, ETrue);  // if to ScrollBar, Draw.
        return this;
        }
    return NULL;
    }


// CallBack from ListBox
// ---------------------------------------------------------------------------
// CPeninputJapaneseCandidateWnd::HandleListBoxEventL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputJapaneseCandidateWnd::HandleListBoxEventL(
    CEikListBox* /*aListBox*/, TListBoxEvent aEventType)
    {
    switch (aEventType)
        {
        case EEventItemDraggingActioned: // Item Moved
            ReDraw();
            break;
        case EEventItemClicked:
            {
            if (iInputContextField) // candidate popup
                {
                ReportEvent(EPeninputLayoutEventSelectItem);
                }
            else
                {
                TInt currentItem = iListBox->CurrentItemIndex();
                TPtrC dataPtr;
                dataPtr.Set(reinterpret_cast<TUint16*>(&currentItem),sizeof(currentItem)/sizeof(TUint16));
                ReportEvent(EPeninputLayoutEventRange, dataPtr);  // Hwr:CharacterRange popup
                }
            }
            break;
        default:
           break;
        }
    }

// ---------------------------------------------------------
// Cancel timers for ScrollBar repeat
// ---------------------------------------------------------
//
void CPeninputJapaneseCandidateWnd::CancelScrollBarRepeat()
    {
    if (iLongPressTimer)
        {
        iLongPressTimer->Cancel();
        }
    if (iRepeatTimer)
        {
        iRepeatTimer->Cancel();
        }
    }

// ---------------------------------------------------------
// Time out event handler of both long press timer & repeat timer
// ---------------------------------------------------------
//
void CPeninputJapaneseCandidateWnd::HandleTimerOut(const CAknFepTimer* aTimer)
    {
    if (aTimer == iLongPressTimer)
        {
        iRepeatTimer->SetTimer(KRepeatInterval);
        }
    else
        {
        if (aTimer == iRepeatTimer)
            {
            if (TPointerEvent::EButton1Up == iScrollBarPointerEvent.iType)
                {
                TRAPD(err, SendPointerEventL(iScrollBarPointerEvent, 0, ETrue));
                if(KErrNone == err)
                    {
                    iScrollBarPointerEvent.iType = TPointerEvent::EButton1Down;
                    TRAP_IGNORE(SendPointerEventL(iScrollBarPointerEvent, 0, EFalse));
                    iScrollBarPointerEvent.iType = TPointerEvent::EButton1Up;
                    }
                iRepeatTimer->SetTimer(KRepeatInterval);
                }
            }
        }
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::ListBox()
// (other items were commented in a header).
// ---------------------------------------------------------------------------
CPeninputJapaneseCandidateListBox* CPeninputJapaneseCandidateWnd::ListBox() const
    {
    return iListBox;
    }

// ---------------------------------------------------------------------------
// CPeninputJapaneseCandidateWnd::OnDeActivate
// DeActivate popup window
// ---------------------------------------------------------------------------
//  
void CPeninputJapaneseCandidateWnd::OnDeActivate()	
    {
    ;// Do not CloseWindow;
    }

// ---------------------------------------------------------------------------
// Constructor
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPeninputJapaneseCandidateWnd::CPeninputJapaneseCandidateWnd(CFepUiLayout* aUiLayout,
                                                    TInt aControlId)
    :CFepLayoutPopupWnd(TSize(0,0), aUiLayout, aControlId),
    iMaximumColumns(KMinListColumns),
    iMaximumRows(KMinListRows),
    iFontMaximumRows(KMaxListRows)
    {
    }

// -----------------------------------------------------------------------------
// CPeninputJapaneseCandidateWnd::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPeninputJapaneseCandidateWnd::ConstructL(const CCoeControl* aParent)
    {
    CFepLayoutPopupWnd::ConstructL();
    iListBox = new(ELeave) CPeninputJapaneseCandidateListBox;
    iListBox->SetBorder(TGulBorder::ENone);
    iListBox->ConstructL(aParent, CEikListBox::ELeftDownInViewRect);
    iListBox->CreateScrollBarFrameL(ETrue, EFalse, EFalse);
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
                                CEikScrollBarFrame::EOff,
                                CEikScrollBarFrame::EAuto);
    iListBox->ItemDrawer()->SetGc(static_cast<CWindowGc*>(BitGc()));
    iListBox->ItemDrawer()->FormattedCellData()->SetSkinEnabledL( ETrue );
    iListBox->SetListBoxObserver(this);
    iLongPressTimer = CAknFepTimer::NewL(this);
    iRepeatTimer = CAknFepTimer::NewL(this);
    }

// -----------------------------------------------------------------------------
// CPeninputJapaneseCandidateWnd::MaximumColumns
// This function returns maximum columns to be able to display in listbox.
// -----------------------------------------------------------------------------
TInt CPeninputJapaneseCandidateWnd::MaximumColumns(
        const CTextListBoxModel* aModel) const
    {
    TInt maximumCharacters = 0;
    TInt numberOfItems = aModel->NumberOfItems();
    TInt i;
    for (i = 0; i < numberOfItems; i++)
        {
        TPtrC text = aModel->ItemText(i);
        TInt textLength = text.Length();
        if (maximumCharacters < textLength)
            {
            maximumCharacters = textLength;
            }
        }

    TInt maximumColumns = Min(KMaxListColumns, maximumCharacters);
    maximumColumns = Max(KMinListColumns, maximumColumns);

    return maximumColumns;
    }

// -----------------------------------------------------------------------------
// CPeninputJapaneseCandidateWnd::MaximumRows
// This function returns maximum rows to be able to display in listbox.
// -----------------------------------------------------------------------------
TInt CPeninputJapaneseCandidateWnd::MaximumRows(
        const CTextListBoxModel* aModel) const
    {
    TInt numberOfItems = aModel->NumberOfItems();
    if (iInputContextField) // candidate popup
        {
        numberOfItems = Max(KMinListRows, numberOfItems);
        }
    TInt maximumRows = Min(iFontMaximumRows, numberOfItems);

    return maximumRows;
    }

// -----------------------------------------------------------------------------
// CPeninputJapaneseCandidateWnd::Draw
// Called by the framework to draw the control
// -----------------------------------------------------------------------------
//
void CPeninputJapaneseCandidateWnd::Draw()
    {
    if(Hiden() || !Ready())
        {
        return;
        }
    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
    gc->Activate( MaskBitmapDevice() );
    gc->SetBrushStyle( CGraphicsContext::ESolidBrush );
    gc->SetBrushColor( MaskBkCol());//Non transparent at all
    gc->SetPenSize( PenSize());
    gc->SetPenColor( MaskBkCol() );
    gc->DrawRect(Rect());
    gc->Activate( BitmapDevice() );

    MAknsSkinInstance* skin = AknsUtils::SkinInstance();

    TRect outerRect = Rect();
    TRect innerRect = TRect(Rect().iTl + TPoint(iTopLeft.iWidth, iTopLeft.iHeight)
                            , Rect().iBr - TPoint(iTopLeft.iWidth, iTopLeft.iHeight));

    TBool skinnedDraw = AknsDrawUtils::DrawFrame( skin, *gc, outerRect, innerRect,
                                          KAknsIIDQsnFrPopup,KAknsIIDQsnFrPopupCenter);

    if (!skinnedDraw)
        {
        gc->SetPenStyle(CGraphicsContext::ENullPen);
        gc->SetBrushStyle(CGraphicsContext::ESolidBrush);
        gc->SetBrushColor(AKN_LAF_COLOR(KStatusPaneBackgroundColor));
        gc->DrawRect(Rect());
        }

    DrawTitle();
    if (iListBox->Parent())
        {
        iListBox->Parent()->DrawNow();
        }
    UpdateArea(iRect, EFalse);
    }

// ---------------------------------------------------------------------------
// CPeninputJapaneseCandidateWnd::ReDraw
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputJapaneseCandidateWnd::ReDraw()
    {
    if(Hiden() || !Ready())
        {
        return;
        }
    DrawTitle();
    UpdateArea(iRect,EFalse);
    }

// ---------------------------------------------------------------------------
// CPeninputJapaneseCandidateWnd::DrawTitle
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputJapaneseCandidateWnd::DrawTitle()
    {
    if (!iInputContextField)  // Hwr:CharacterRange popup
        {
        return;
        }
    CTextListBoxModel* model =
        static_cast<CTextListBoxModel*>(iListBox->Model());
    TInt numOfItem = model->NumberOfItems();
    TBuf<KMaxTitleLength> title;
    title.Format(_L("%d / %d"), iListBox->CurrentItemIndex() + 1, numOfItem);

    // Title Font
    TAknTextLineLayout choiceTextLayout =
            AKN_LAYOUT_TEXT_Candidate_selection_list_texts_Line_1;
    const CFont* font = AknLayoutUtils::FontFromId(choiceTextLayout.iFont, NULL);

    TRect rect(iListBox->Rect());
    TInt fontSize = font->AscentInPixels();
    rect.iBr.iY = rect.iTl.iY;
    rect.iTl.iY -= (fontSize + KPENINPUT_LAYOUT_VALUE_2);
    rect.iBr.iX = Rect().iBr.iX - iTopLeft.iWidth;
    TInt baseline = fontSize + 1;

    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
    TRect outerRect = Rect();
    TRect innerRect = TRect(Rect().iTl + TPoint(iTopLeft.iWidth, iTopLeft.iHeight)
                            , Rect().iBr - TPoint(iTopLeft.iWidth, iTopLeft.iHeight));

    gc->SetClippingRect(rect);
    TBool skinnedDraw = AknsDrawUtils::DrawFrame( skin, *gc, outerRect, innerRect,
                                          KAknsIIDQsnFrPopup,KAknsIIDQsnFrPopupCenter);
    gc->CancelClippingRect();
    if (!skinnedDraw)
        {
        gc->SetPenStyle(CGraphicsContext::ENullPen);
        gc->SetBrushStyle(CGraphicsContext::ESolidBrush);
        gc->SetBrushColor(AKN_LAF_COLOR(KStatusPaneBackgroundColor));
        gc->DrawRect(rect);
        }

    TRgb color( KRgbBlack );
    AknsUtils::GetCachedColor( AknsUtils::SkinInstance(),
                               color, KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG7 );
    gc->UseFont(font);
    gc->SetBrushStyle( CGraphicsContext::ENullBrush );
    gc->SetPenColor(color);
    gc->SetPenStyle(CGraphicsContext::ESolidPen);
    gc->DrawText(title, rect, baseline, CGraphicsContext::ERight, KPENINPUT_LAYOUT_VALUE_2);
    gc->DiscardFont();
    }

// ---------------------------------------------------------------------------
// CPeninputJapaneseCandidateWnd::SendPointerEventL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputJapaneseCandidateWnd::SendPointerEventL(const TPointerEvent& aPointerEvent, TInt aFlags, TBool aDraw)
    {
    if (1 == aFlags)  //  to ListBox
        {
        iListBox->HandlePointerEventL(aPointerEvent);
        }
    else if(iListBox->ScrollBarFrame()->GetScrollBarHandle(CEikScrollBar::EVertical))
        {  // on ScrollBar
        iListBox->ScrollBarFrame()->GetScrollBarHandle(CEikScrollBar::EVertical)->HandlePointerEventL(aPointerEvent);
        if (aDraw)
            {
            UpdateArea(iRect,EFalse);
            }
        }
    }
// End of File
