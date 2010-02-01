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
* Description:  Predictive list
*
*/


// INCLUDE FILES
#include <AknsDrawUtils.h>
#include <aknconsts.h> // KStatusPaneBackgroundColor
#include <aknlayoutscalable_avkon.cdl.h> // AknLayoutScalable_Avkon
#include <peninputeventjp.h>  // EPeninputLayoutEventSelectItem
#include <peninputrepeatbutton.h>   // CAknFepCtrlEventButton,CAknFepCtrlRepeatButton
#include <peninputlayout.h>
#include <AknsBasicBackgroundControlContext.h> 

#include "peninputeventjp.h"
#include "peninputjapanesepredictivewnd.h"
#include "peninputjapanesepredictivelistbox.h"
#include "peninputjapanesepredictivecelllistboxdata.h"
#include "peninputjapanesepredictiveListboxitemdrawer.h"
#include "peninputlayoutwindowjp.h"

// CONSTANTS
const TInt KMaxListColumns = 24;  // for vkb
const TInt KSpaceCharCode = ' ';

/**
*  CPeninputJapanesePredictiveWnd class.
*
*/

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPeninputJapanesePredictiveWnd* CPeninputJapanesePredictiveWnd::NewL(CFepUiLayout* aUiLayout,
                                                               TInt aControlId, const CCoeControl* aParent)
    {
    CPeninputJapanesePredictiveWnd* self =
        new(ELeave) CPeninputJapanesePredictiveWnd(aUiLayout, aControlId);
    CleanupStack::PushL(self);
    self->ConstructL(aParent);
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::~CPeninputJapanesePredictiveWnd
// Destructor
// -----------------------------------------------------------------------------
//
CPeninputJapanesePredictiveWnd::~CPeninputJapanesePredictiveWnd()
    {
    delete iListBox;
    if (iLineInfoArray)
        {
        iLineInfoArray->ResetAndDestroy();
        }
    delete iLineInfoArray;
    delete iListTextArray;
    }

// ---------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::SetRect
// Set the prediction pane's rect
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputJapanesePredictiveWnd::SetRect(const TRect& aRect)
    {
    if (!iUnitWidth)
        {
        return;  // not ready
        }
    iRect = aRect;
    CFepUiBaseCtrl::UpdateValidRegion(this, ETrue);  // for getting pointer event
    TRect rect(aRect);
    TAknWindowLineLayout textWindowLayout =
        AknLayoutScalable_Avkon::list_single_choice_list_pane(0).LayoutLine();
    TInt topMargin = (iUnitHeight > textWindowLayout.iH)? (iUnitHeight - textWindowLayout.iH) : KPENINPUT_LAYOUT_VALUE_2;
    rect.Shrink(KPENINPUT_LAYOUT_VALUE_4, topMargin);

    rect.iBr.iX -=iUnitWidth;
    iListBox->SetRect(rect);  // call iListBox->SizeChaged()
    if (iScrollUpBtn && iScrollDownBtn)
        {
        TRect upBtn(TPoint(iRect.iBr.iX -iUnitWidth , iRect.iTl.iY), TSize(iUnitWidth, iUnitHeight));
        TRect downBtn(TPoint(iRect.iBr.iX - iUnitWidth, iRect.iTl.iY + iUnitHeight), TSize(iUnitWidth, iUnitHeight));
        iScrollUpBtn->SetRect(upBtn);
        iScrollDownBtn->SetRect(downBtn);
        }

    // instead of MakeVisible(EFalse)
    if (iListBox->ScrollBarFrame()->GetScrollBarHandle(CEikScrollBar::EVertical))
        {
        TRect sbRect(TPoint(rect.iBr.iX, Rect().iTl.iY)
            , TSize(iListBox->ScrollBarFrame()->GetScrollBarHandle(CEikScrollBar::EVertical)->Rect().Size().iHeight,0));

        iListBox->ScrollBarFrame()->GetScrollBarHandle(CEikScrollBar::EVertical)->SetRect(sbRect);
        }
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::Hide
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPeninputJapanesePredictiveWnd::Hide(TBool aFlag)
    {
    if (iScrollUpBtn && iScrollDownBtn)
        {
        CPeninputLayoutWindowJp* window = static_cast<CPeninputLayoutWindowJp*>(iScrollUpBtn->ParentCtrl());
        if (window)
            {
            window->ICFDisableRegionUpdating();  // ICF::OnDeActivate()
            }
        if (aFlag || !iScrollUpBtn->IsDimmed())
            {
            iScrollUpBtn->Hide(aFlag);
            iScrollDownBtn->Hide(aFlag);
            }
        else
            {
            iScrollUpBtn->Hide(ETrue);
            iScrollDownBtn->Hide(ETrue);
            }
        }
    CFepLayoutPopupWnd::Hide(aFlag);
    }
// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::SetButtons
// -----------------------------------------------------------------------------
//
void CPeninputJapanesePredictiveWnd::SetButtons(CAknFepCtrlEventButton* aScrollUpBtn
                                              , CAknFepCtrlEventButton* aScrollDownBtn)
    {
    if (aScrollUpBtn && aScrollDownBtn)
        {
        iScrollUpBtn = aScrollUpBtn;
        iScrollDownBtn = aScrollDownBtn;
        iScrollUpBtn->AddEventObserver(this);
        iScrollDownBtn->AddEventObserver(this);
        ActivateButtons(EFalse);
        }
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::SetCurrentItemIndex
// Specify the cursor position of a pop-up list.
// -----------------------------------------------------------------------------
//
void CPeninputJapanesePredictiveWnd::SetCurrentItemIndex(TInt aItemIndex) const
    {
    CPeninputJapanesePredictiveListBoxModel* model =
        static_cast<CPeninputJapanesePredictiveListBoxModel*>(iListBox->Model());
    const TInt maximumItems = model->MaximumItems();

    if (aItemIndex > maximumItems)
        {
        aItemIndex = maximumItems;
        }

    iListBox->SetCurrentCandidateNo(aItemIndex, EFalse);
    }

// ---------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::HandleControlEvent
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputJapanesePredictiveWnd::HandleControlEvent(TInt aEventType,
                                          CFepUiBaseCtrl* /*aCtrl*/,
                                          const TDesC& /*aEventData*/)
    {
    TPointerEvent pEvent;
    pEvent.iType = TPointerEvent::EButton1Down;
    pEvent.iModifiers = 0;
    pEvent.iParentPosition = TPoint(0,0);
    switch (aEventType)
        {
        case EPeninputLayoutEventScrollUp:
            if (iListBox->ScrollBarFrame()->GetScrollBarHandle(CEikScrollBar::EVertical))
                {
                TRAP_IGNORE(iListBox->HandleScrollEventL(iListBox->ScrollBarFrame()->GetScrollBarHandle(CEikScrollBar::EVertical),EEikScrollUp));
                ReDraw();
                }
            break;
        case EPeninputLayoutEventScrollDown:
            if (iListBox->ScrollBarFrame()->GetScrollBarHandle(CEikScrollBar::EVertical))
                {
                TRAP_IGNORE(iListBox->HandleScrollEventL(iListBox->ScrollBarFrame()->GetScrollBarHandle(CEikScrollBar::EVertical),EEikScrollDown));
                ReDraw();
                }
            break;
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::ShowPredictiveListL
// popup is shown always.this method draws lists.
// -----------------------------------------------------------------------------
//
void CPeninputJapanesePredictiveWnd::ShowPopupL(const CDesCArrayFlat* aItems, TInt aIndex)
    {
    if(Hiden())
        {
        return;
        }
    if(aItems->Count() <= 0)
        {
        HidePopup();  // clear lists
        return;
        }

    if (iLineInfoArray)
        {
        iLineInfoArray->ResetAndDestroy();
        }
    else
        {
        iLineInfoArray = new(ELeave) CPredictivePopupLineInfoArray(1);
        }

    TAknTextLineLayout textLineLayout =
        AknLayoutScalable_Avkon::fep_vkb_top_text_pane_t1().LayoutLine();

    TInt allowDrawWidth = Rect().Size().iWidth - iUnitWidth-KPENINPUT_LAYOUT_VALUE_16;

    // Setup iLineInfoArray
    CreatePredictivePopupLineInfoArrayL(aItems, allowDrawWidth, textLineLayout.iFont);

    // Create iListTextArray
    CreateListTextArrayL(*iLineInfoArray);

    // Set listitems.
    CPeninputJapanesePredictiveListBoxModel* model =
        static_cast<CPeninputJapanesePredictiveListBoxModel*>(iListBox->Model());
    model->SetMaximumItems(aItems->Count());
    model->SetLineInfoArray(iLineInfoArray);
    model->SetItemTextArray(iListTextArray);
    model->SetOwnershipType(ELbmDoesNotOwnItemArray);

    SetCurrentItemIndex(aIndex);
    SetRect(Rect());
    iListBox->ActivateL();
    TBool dimm = EFalse;
    if (iLineInfoArray->Count() <= iListBox->BottomItemIndex() + 1)
        {
        dimm = ETrue;
        }
    if (iListBox->ScrollBarFrame()->GetScrollBarHandle(CEikScrollBar::EVertical))
        {
        iListBox->ScrollBarFrame()->GetScrollBarHandle(CEikScrollBar::EVertical)->SetDimmed(dimm);
        }
    ActivateButtons(!dimm);
    if (dimm)
        {
        ClearList();
        }
    iListBox->Draw();
    if (iScrollUpBtn && iScrollDownBtn && !iScrollUpBtn->IsDimmed())
        {
        iScrollUpBtn->Draw();
        iScrollDownBtn->Draw();
        }
    UpdateArea(iRect, EFalse);
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::HideWindow
// popup is shown always.this method clear lists.
// -----------------------------------------------------------------------------
//
void CPeninputJapanesePredictiveWnd::HidePopup()
    {
    if(!Hiden())
        {
        // Reset listitems.
        if (iLineInfoArray)
            {
            iLineInfoArray->ResetAndDestroy();
            }
        ActivateButtons(EFalse);
        ClearList();
        }
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::CurrentItemIndexOfCandidate
// Return a selected candidate item number currently
// -----------------------------------------------------------------------------
//
TInt CPeninputJapanesePredictiveWnd::CurrentItemIndexOfCandidate() const
    {
    if (iListBox->IsVisible())
        {
        return iListBox->CurrentCandidateNo();
        }
    return -1;      // Disable window
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::MoveTo
// Move a current item in an appointed direction
// -----------------------------------------------------------------------------
//
void CPeninputJapanesePredictiveWnd::MoveTo(TPeninputUICtrlScroll aDirection)
    {
    if (iListBox->IsVisible())
        {
        TBool action = ETrue;

        switch(aDirection)
            {
            case EScrollLeft:
                {
                iListBox->MovePreviousItem();
                }
                break;

            case EScrollRight:
                {
                iListBox->MoveNextItem();
                }
                break;
            case EScrollUp:
                {
                iListBox->MovePreviousLineItem();
                }
                break;

            case EScrollDown:
                {
                iListBox->MoveNextLineItem();
                }
                break;

            default:
                {
                action = EFalse;
                }
                break;
            }
        if (action)
            {
            if (!Hiden())
                {
                ReDraw();
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::SizeChanged
// called by CPeninputLayoutWindowJp::SetControlsFont()
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputJapanesePredictiveWnd::SizeChanged(TInt aWidth, TInt aUnitWidth, TInt aUnitHeight)
    {
    if (aWidth > iUnitWidth)
        {
        iUnitWidth = aUnitWidth;
        iUnitHeight = aUnitHeight;
        TRect newRect(Rect().iTl ,TSize(aWidth,iUnitHeight*KPENINPUT_LAYOUT_PREDICTIONPANE_ROW_COUNT));
        SetRect(newRect);   // call iListBox->SetRect(), call iListBox->SizeChanged()
        iScrollUpBtn->SizeChanged(iScrollUpBtn->Rect(), iScrollUpBtn->Rect(), ETrue);
        iScrollDownBtn->SizeChanged(iScrollDownBtn->Rect(), iScrollDownBtn->Rect(), ETrue);
        }
    }

// ---------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::HitTest
// Get item whose region contains the point.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CPeninputJapanesePredictiveWnd::HitTest(const TPoint& aPoint)
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
        return 0;     // ScrollBar
    }

// ---------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::HandlePointerDownEventL
// Handle pointer down event.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CFepUiBaseCtrl* CPeninputJapanesePredictiveWnd::HandlePointerDownEventL(const TPoint& aPoint)
    {
    CFepLayoutPopupWnd::HandlePointerDownEventL(aPoint);  // set pointer down
    if (iLineInfoArray && iLineInfoArray->Count() > 0)
        {
        TInt index = HitTest(aPoint);
        TPointerEvent pEvent;
        pEvent.iType = TPointerEvent::EButton1Down;
        pEvent.iPosition = aPoint;
        pEvent.iModifiers = 0;
        pEvent.iParentPosition = TPoint(0,0);
        if (1 == index)  // on ListBox
            {
            TInt oldItem = iListBox->CurrentItemIndex();
            iListBox->HandlePointerEventL(pEvent);
            TInt currentItem = iListBox->CurrentItemIndex();
            if (0 <= currentItem)
                {
                iListBox->SetCurrentCandidateNo(currentItem, aPoint-iListBox->Rect().iTl, ETrue);
                if (oldItem != currentItem)
                    {
                    TInt topItem = iListBox->TopItemIndex();
                    if (topItem <= oldItem && topItem + KPENINPUT_LAYOUT_PREDICTIONPANE_ROW_COUNT> oldItem)
                        {
                        iListBox->SetCurrentItemIndexAndDraw(oldItem);
                        iListBox->SetCurrentItemIndex(currentItem);
                        }
                    }
                ReDraw();
                }
            }
        }
    return this;
    }

// ---------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::HandlePointerMoveEventL
// Handle pointer move event.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CFepUiBaseCtrl* CPeninputJapanesePredictiveWnd::HandlePointerMoveEventL(const TPoint& aPoint)
    {
    CFepLayoutPopupWnd::HandlePointerMoveEventL(aPoint);  // nop
    if( PointerDown() && iLineInfoArray && iLineInfoArray->Count() > 0)
        {
        TInt index = HitTest(aPoint);
        TPointerEvent pEvent;
        pEvent.iType = TPointerEvent::EDrag;
        pEvent.iPosition = aPoint;
        pEvent.iModifiers = 0;
        pEvent.iParentPosition = TPoint(0,0);
        if (1 == index)
            {
            TInt oldItem = iListBox->CurrentItemIndex();
            iListBox->HandlePointerEventL(pEvent);
            TInt currentItem = iListBox->CurrentItemIndex();
            if (0 <= currentItem)
                {
                iListBox->SetCurrentCandidateNo(currentItem, aPoint-iListBox->Rect().iTl, ETrue);
                if (oldItem != currentItem)
                    {
                    iListBox->SetCurrentItemIndexAndDraw(oldItem);
                    iListBox->SetCurrentItemIndex(currentItem);
                    }
                ReDraw();
                }
            }
        }
    return this;
    }

// ---------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::HandlePointerUpEventL
// Handle pointer up event.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CFepUiBaseCtrl* CPeninputJapanesePredictiveWnd::HandlePointerUpEventL(const TPoint& aPoint)
    {
    TBool down = PointerDown();
    CFepLayoutPopupWnd::HandlePointerUpEventL(aPoint);  // pointer down off
    TInt index = HitTest(aPoint);
    if (down && iLineInfoArray && iLineInfoArray->Count() > 0)
        {
        TPointerEvent pEvent;
        pEvent.iType = TPointerEvent::EButton1Up;
        pEvent.iPosition = aPoint;
        pEvent.iModifiers = 0;
        pEvent.iParentPosition = TPoint(0,0);
        if (1 == index)  // onListBox
            {
            iListBox->HandlePointerEventL(pEvent);
            TInt currentItem = iListBox->CurrentItemIndex();
            if (0 <= currentItem)
                {
                iListBox->SetCurrentCandidateNo(currentItem, aPoint-iListBox->Rect().iTl, ETrue);
                }
            ReportEvent(EPeninputLayoutEventSelectItem);
            }
        }
    return this;
    }

// CallBack from ListBox
// ---------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::HandleListBoxEventL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputJapanesePredictiveWnd::HandleListBoxEventL(
    CEikListBox* /*aListBox*/, TListBoxEvent /*aEventType*/)
    {
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::SetLineCount
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPeninputJapanesePredictiveWnd::SetLineCount(TInt aLineCount)
    {
    iListBox->SetLineCount(aLineCount);
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::ReDraw
// (other items were commented in a header).
// ---------------------------------------------------------------------------
void CPeninputJapanesePredictiveWnd::ReDraw()
    {
    if(Hiden() || !Ready())
        {
        return;
        }
    UpdateArea(iRect, EFalse);
    }

// ---------------------------------------------------------
// Cancel repeat
// ---------------------------------------------------------
//
void CPeninputJapanesePredictiveWnd::CancelRepeat()
    {
    if (iScrollUpBtn)
        {
        static_cast<CAknFepCtrlRepeatButton*>(iScrollUpBtn)->CancelRepeat();
        }
    if (iScrollDownBtn)
        {
        static_cast<CAknFepCtrlRepeatButton*>(iScrollDownBtn)->CancelRepeat();
        }
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::ListBox()
// (other items were commented in a header).
// ---------------------------------------------------------------------------
CPeninputJapanesePredictiveListBox* CPeninputJapanesePredictiveWnd::ListBox() const
    {
    return iListBox;
    }

// ---------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::OnDeActivate
// DeActivate popup window
// ---------------------------------------------------------------------------
//  
void CPeninputJapanesePredictiveWnd::OnDeActivate()	
    {
    ;// Do not CloseWindow;
    }

// -----------------------------------------------------------------------------
// Constructor
//
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CPeninputJapanesePredictiveWnd::CPeninputJapanesePredictiveWnd(CFepUiLayout* aUiLayout,
                                                    TInt aControlId)
    :CFepLayoutPopupWnd(TSize(0,0), aUiLayout, aControlId)
    {
    SetControlType(ECtrlBackgroundCtrl|ControlType()); // should not be ActiveCtrl of ControlGroup
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPeninputJapanesePredictiveWnd::ConstructL(const CCoeControl* aParent)
    {
    CFepLayoutPopupWnd::ConstructL();
    iListBox = new(ELeave) CPeninputJapanesePredictiveListBox;
    iListBox->SetBorder(TGulBorder::ENone);
    iListBox->ConstructL(aParent, CEikListBox::ELeftDownInViewRect);
    iListBox->CreateScrollBarFrameL(ETrue, EFalse, EFalse);
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
                                CEikScrollBarFrame::EOff,
                                CEikScrollBarFrame::EAuto);
    iListBox->ItemDrawer()->SetGc(static_cast<CWindowGc*>(BitGc()));
    iListBox->ItemDrawer()->FormattedCellData()->SetSkinEnabledL( ETrue );
    static_cast<CPeninputJapanesePredictiveListBoxItemDrawer*>(iListBox->ItemDrawer())->SetBitmapDevice(BitmapDevice());
    static_cast<CPeninputJapanesePredictiveCellListBoxData*>(iListBox->ItemDrawer()->FormattedCellData())->SetWnd(this);
    iLineInfoArray = NULL;
    iListTextArray = NULL;
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::CreatePredictivePopupLineInfoArrayL
// Create a array with line information in candidate list
// -----------------------------------------------------------------------------
void CPeninputJapanesePredictiveWnd::CreatePredictivePopupLineInfoArrayL(
        const CDesCArrayFlat* aItems,
        const TInt aMaxDisplayWidthInPixel,
        const TInt aFontId)
    {

    // getting font object from font id
    const CFont* font = AknLayoutUtils::FontFromId(aFontId);

    TInt displayWidthInPixel = aMaxDisplayWidthInPixel;
    TInt elementStartXPos = 0;

    TBufC<KMaxListColumns*2> bufferOfLine;
    TPtr lineText(bufferOfLine.Des());
    lineText.SetLength(0);

    CPredictivePopupElementArray* elementArray = NULL;

    const TInt widthOfSpaceInPixel = font->CharWidthInPixels(KSpaceCharCode);
    const TInt numberOfItem = aItems->Count();
    TInt i;
    for (i = 0; i < numberOfItem; i++)
        {
        TPtrC itemText = aItems->MdcaPoint(i);
        TInt itemTextLength = itemText.Length();

        // getting a number of characters within width
        TInt count = font->TextCount(itemText, displayWidthInPixel);

        // If a candidate cannot occupy a line, it is
        // continued in next line
        if (count < itemTextLength && elementArray)
            {
            PredictivePopupAppendLineL(lineText, elementArray);
            CleanupStack::Pop(elementArray);    // elementArray
            elementArray = NULL;
            lineText.SetLength(0);

            displayWidthInPixel = aMaxDisplayWidthInPixel;
            elementStartXPos = 0;
            count = font->TextCount(itemText, displayWidthInPixel);
            }

        FOREVER
            {
            if (!elementArray)
                {
                elementArray = new(ELeave) CPredictivePopupElementArray(1);
                CleanupStack::PushL(elementArray);
                }

            TPtrC elementText = itemText.Left(count);
            TInt  elementWidthInPixels = font->TextWidthInPixels(elementText);

            // setting a item string to line string
            lineText.Append(elementText);
            displayWidthInPixel -= elementWidthInPixels;

            TPredictivePopupElement& element = elementArray->ExtendL();
            element.iStartXPos = elementStartXPos;
            elementStartXPos += elementWidthInPixels;
            element.iEndXPos   = elementStartXPos;
            element.iCandidateNo = i;

            // creating a string which removes a item string
            itemTextLength -= count;
            if (itemTextLength > 0)
                {
                // If this line is full, add this line and create a new line.
                PredictivePopupAppendLineL(lineText, elementArray);
                CleanupStack::Pop(elementArray);    // elementArray
                elementArray = NULL;
                lineText.SetLength(0);

                itemText.Set(itemText.Right(itemTextLength));
                displayWidthInPixel = aMaxDisplayWidthInPixel;
                elementStartXPos = 0;

                // Calculating a number of characters which be able to shown in the width and
                // cutting the string
                count = font->TextCount(itemText, displayWidthInPixel);
                }
            else
                {
                break;
                }
            }

        if (displayWidthInPixel > widthOfSpaceInPixel)
            {
            // adding space if it is possible to add space
            lineText.Append(KSpaceCharCode);
            displayWidthInPixel -= widthOfSpaceInPixel;
            elementStartXPos    += widthOfSpaceInPixel;
            }
        else
            {
            // adding a line information if it isn't possible to add space
            PredictivePopupAppendLineL(lineText, elementArray);
            CleanupStack::Pop(elementArray);    // elementArray
            elementArray = NULL;
            lineText.SetLength(0);

            displayWidthInPixel = aMaxDisplayWidthInPixel - 1;
            elementStartXPos = 0;
            }
        }

    if (lineText.Length() > 0 && elementArray)
        {
        PredictivePopupAppendLineL(lineText, elementArray);
        CleanupStack::Pop(elementArray);    // elementArray
        }

    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::PredictivePopupAppendLineL
// Append to array with line information
// -----------------------------------------------------------------------------
//
void CPeninputJapanesePredictiveWnd::PredictivePopupAppendLineL(
        const TPtr& aLineText,
        CPredictivePopupElementArray* aElementArray)
    {
    HBufC* lineTextBuf = HBufC::NewL(aLineText.Length());
    CleanupStack::PushL(lineTextBuf);
    TPtr lineTextBufPtr = lineTextBuf->Des();
    lineTextBufPtr = aLineText;

    CPredictivePopupLineInfo* lineInfo = new(ELeave) CPredictivePopupLineInfo;
    CleanupStack::PushL(lineInfo);
    iLineInfoArray->AppendL(lineInfo);
    CleanupStack::Pop(lineInfo);
    lineInfo->iLineText = lineTextBuf;
    CleanupStack::Pop(lineTextBuf);
    lineInfo->iElementsOnLine = aElementArray;
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::CreateListTextArrayL
// Create arrary with string to show in a line
// -----------------------------------------------------------------------------
//
void CPeninputJapanesePredictiveWnd::CreateListTextArrayL(const CPredictivePopupLineInfoArray& aLineArray)
    {
    CDesCArrayFlat* textArray = new(ELeave) CDesCArrayFlat(1);
    CleanupStack::PushL(textArray);

    const TInt numberOfItem = aLineArray.Count();
    TInt i;
    for (i = 0; i < numberOfItem; i++)
        {
        CPredictivePopupLineInfo* lineInfo = aLineArray.At(i);
        textArray->AppendL(*lineInfo->iLineText);
        }

    CleanupStack::Pop(textArray);
    delete iListTextArray;
    iListTextArray = NULL;
    iListTextArray = textArray;
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::Draw
// Called by the framework to draw the control
// -----------------------------------------------------------------------------
//
void CPeninputJapanesePredictiveWnd::Draw()
    {
    if(Hiden() || !Ready())
        {
        return;
        }
    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
    ClearList();
    if (iLineInfoArray && iLineInfoArray->Count() > 0)
        {
        iListBox->Draw();
        if (iScrollUpBtn && iScrollDownBtn && !iScrollUpBtn->IsDimmed())
            {
            iScrollUpBtn->Draw();
            iScrollDownBtn->Draw();
            }
        }
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::Clear
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPeninputJapanesePredictiveWnd::ClearList()
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

    MAknsControlContext *cc = AknsDrawUtils::ControlContext( iListBox );
    if (cc)
        {
        CAknsBasicBackgroundControlContext *bcc =
            (CAknsBasicBackgroundControlContext*)cc;
        bcc->SetBitmap(KAknsIIDQsnFrPopupCenter);
        bcc->SetRect(Rect());
        }

    MAknsSkinInstance* skin = AknsUtils::SkinInstance();

    TBool skinnedDraw = AknsDrawUtils::DrawFrame( skin, *gc, Rect(), Rect(),
                                          KAknsIIDQsnFrPopupCenter,KAknsIIDQsnFrPopupCenter);

    if (!skinnedDraw)
        {
        gc->SetPenStyle(CGraphicsContext::ENullPen);
        gc->SetBrushStyle(CGraphicsContext::ESolidBrush);
            gc->SetBrushColor(AKN_LAF_COLOR(KStatusPaneBackgroundColor));
        gc->DrawRect(Rect());
        }
    UpdateArea(iRect,EFalse);
    }

// -----------------------------------------------------------------------------
// CPeninputJapanesePredictiveWnd::ActivateButtons
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPeninputJapanesePredictiveWnd::ActivateButtons(TBool aActive) const
    {
    if (iScrollUpBtn && iScrollDownBtn)
        {
        CPeninputLayoutWindowJp* window = static_cast<CPeninputLayoutWindowJp*>(iScrollUpBtn->ParentCtrl());
        if (window)
            {
            window->ICFDisableRegionUpdating();  // ICF::OnDeActivate()
            }
        iScrollUpBtn->SetDimmed(!aActive);
        iScrollDownBtn->SetDimmed(!aActive);
        iScrollUpBtn->Hide(!aActive);
        iScrollDownBtn->Hide(!aActive);
        window->ICFEnableRegionUpdating();  // ICF::OnActivate()
        }
    }
// End of File
