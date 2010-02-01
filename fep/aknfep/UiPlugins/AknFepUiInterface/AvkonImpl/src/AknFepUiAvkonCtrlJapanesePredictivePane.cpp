/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            Predictive candidate list
*
*/












// INCLUDE FILES
#include <eikdef.h>
#include <aknViewAppUi.h>
#include <avkon.hrh>
#include <aknPopup.h>
#include <aknlists.h>
#include <aknPopupHeadingPane.h>
#include <AknsDrawUtils.h>
#include <AknsBasicBackgroundControlContext.h>
#include <AknFep.rsg>
#include <barsread.h>
#include <eikmop.h>
#include <AknLayout.lag>
#include <aknlayoutscalable_avkon.cdl.h>

#include "AknFepUiInterfacePanic.h"
#include "AknFepGlobalEnums.h"
#include "AknFepUIAvkonCtrlJapanesePredictiveListbox.h"
#include "AknFepUIAvkonCtrlJapanesePredictivePane.h"
#include "AknFepUIManagerJapanese.h"

// CONSTANTS
const TInt KMaxListColumns = 30;
const TInt KLAFBaseListRows = 1;
const TInt KLAFBelowTextBaseline = 2;
const TInt KLAFAboveTextBaseline = 1;
const TInt KSpaceCharCode = ' ';
const TInt KListboxFlameMarge = 4;
const TInt KScrollRightMarge = 5;
const TInt KListboxMarge = 3;

/**
*  CAknFepUICtrlJapanesePredictivePane class.
*
*/

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAknFepUICtrlJapanesePredictivePane* CAknFepUICtrlJapanesePredictivePane::NewL(CAknFepUIManagerJapanese* aUiMng)
    {
    CAknFepUICtrlJapanesePredictivePane* self =
        new(ELeave)CAknFepUICtrlJapanesePredictivePane(aUiMng);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);    // self
    return self;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::~CAknFepUICtrlJapanesePredictivePane
// Destructor
// -----------------------------------------------------------------------------
//
CAknFepUICtrlJapanesePredictivePane::~CAknFepUICtrlJapanesePredictivePane()
    {
    if (IsVisible())
        {
        HideWindow();
        }

    if (iListBox)
        {
        delete iListBox;
        iListBox = NULL;
        }

    if (iLineInfoArray)
        {
        iLineInfoArray->ResetAndDestroy();
        delete iLineInfoArray;
        iLineInfoArray = NULL;
        }

    if (iListTextArray)
        {
        delete iListTextArray;
        iListTextArray = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::ShowWindow
// Show Pop-up list
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictivePane::ShowWindow()
    {
    RWindow& window=Window();
    //Set ordinal position to top
    window.SetOrdinalPosition(0,ECoeWinPriorityFep);
    window.SetFaded(EFalse, RWindowTreeNode::EFadeIncludeChildren);
    window.SetPointerCapture(ETrue);
    TRAP_IGNORE( ActivateL());
    MakeVisible(ETrue);
    DrawNow();
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::ClosePredictiveList
// Close Pop-up list
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictivePane::ClosePredictiveList()
    {
    CancelPopup();
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::SetCurrentItemIndex
// Specify the cursor position of a pop-up list.
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictivePane::SetCurrentItemIndex(TInt aItemIndex)
    {
    CAknFepUICtrlJapanesePredictiveListBoxModel* model =
        static_cast<CAknFepUICtrlJapanesePredictiveListBoxModel*>(ListBox()->Model());
    const TInt maximumItems = model->MaximumItems();

    if (aItemIndex > maximumItems)
        {
        aItemIndex = maximumItems;
        }

    CAknFepUICtrlJapanesePredictiveListbox* listBox =
        static_cast<CAknFepUICtrlJapanesePredictiveListbox*>(ListBox());

    listBox->SetCurrentCandidateNo(aItemIndex, EFalse);
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::SetOffsetPosition
// Specify the display position of a pop-up list
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictivePane::SetOffsetPosition(TPoint aOffset)
    {
    iOffsetPosition = aOffset;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::SetWindowPosition
// Set position to show candidate list
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictivePane::SetWindowPosition(
                                                const TPoint& aLeftSideOfBaseLine,
                                                TInt aHeight,
                                                TInt aAscent)
    {
    // getting rectangle of main pane
    TRect screen =
        TRect(0, 0, AKN_LAYOUT_WINDOW_screen.iW, AKN_LAYOUT_WINDOW_screen.iH);
    TAknLayoutRect main_pane;
    main_pane.LayoutRect(screen, AKN_LAYOUT_WINDOW_main_pane(screen, 0, 1, 1));
    iMainPaneRect = main_pane.Rect();

    TInt leftPos = aLeftSideOfBaseLine.iX;
    if (leftPos < 0)
        {
        leftPos = iOffsetPosition.iX;
        }

    TPoint windowOffset(
        leftPos,
        aLeftSideOfBaseLine.iY + aHeight - aAscent - iMainPaneRect.iTl.iY);
    SetOffsetPosition(windowOffset);

    iSrcTextHeight = aHeight;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::ShowCandidateListL
// Show Pop-up list
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictivePane::ShowCandidateListL(
        CDesCArrayFlat* aItems, TInt aIndex, TInt aResourceID)
    {
    __ASSERT_DEBUG(aItems->Count() > 0,
                   AknFepUiInterfacePanic(EAknFepUiInterfacePanicCandidatesEmpty));

    if (iLineInfoArray)
        {
        iLineInfoArray->ResetAndDestroy();
        }
    delete iLineInfoArray;
    delete iListTextArray;
    iLineInfoArray = NULL;
    iListTextArray = NULL;
    // getting listbox width by a pixel unit
    TAknWindowLineLayout laf =
        AKN_LAYOUT_WINDOW_popup_fep_japan_predictive_window(0, 0);
    TAknTextLineLayout textLineLayout =
        AKN_LAYOUT_TEXT_Predictive_candidate_selection_list_texts_Line_1(0,0);
    TAknWindowLineLayout scrollLineLayout = AknLayoutScalable_Avkon::scroll_pane_cp29().LayoutLine();
    TAknLayoutText text;
    text.LayoutText(laf.Rect(), textLineLayout);
    TInt allowDrawWidth = text.TextRect().Width() - scrollLineLayout.iW;

    iLineInfoArray = CreatePredictivePopupLineInfoArrayL(
        aItems, allowDrawWidth, textLineLayout.iFont);
    iListTextArray = CreateListTextArrayL(iLineInfoArray);

    // Set listitems.
    CAknFepUICtrlJapanesePredictiveListBoxModel* model =
        static_cast<CAknFepUICtrlJapanesePredictiveListBoxModel*>(ListBox()->Model());
    model->SetMaximumItems(aItems->Count());
    model->SetLineInfoArray(iLineInfoArray);
    model->SetItemTextArray(iListTextArray);
    model->SetOwnershipType(ELbmDoesNotOwnItemArray);

    iMaximumRows = MaximumRows(model);

    SetCurrentItemIndex(aIndex);
    SetMaximumHeight(iMaximumRows);

    // Show popup list
    TBool notShowingPopup =
        model->NumberOfItems() == 0 && iWindowType < 0x80;
    if (notShowingPopup)
        {
        const TSize screenSize=iCoeEnv->ScreenDevice()->SizeInPixels();
        iPopoutCba->SetBoundingRect(TRect(screenSize));
        }

    // Create a CBA for use with the popup
    if (aResourceID)
        {
        UpdateCbaL(aResourceID);
        }

    SetupWindowLayout(iWindowType);

    iListBox->SetListBoxObserver(this);

    if (iMarkable)
        {
        // If markable, set the popup to observe event change events.
        // Call HandleControlEvent to set the CBA appropriate to the 1st item
        iListBox->SetObserver(this);
        HandleControlEventL(iListBox, EEventStateChanged);
        }

    if (IsVisible())
        {
        DrawDeferred();
        }
    else
        {
        ShowWindow();
        ActivateL();
        ShowWindowCbaL();
        }
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::HideWindow
// Hide Pop-up list
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictivePane::HideWindow()
    {
    MakeVisible(EFalse);
    Window().SetPointerCapture(EFalse);

    iEikonEnv->RemoveFromStack(iPopoutCba);

    iPopoutCba->MakeVisible(EFalse);

    iCurrentResource = 0;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::CurrentItemIndexOfCandidate
// Return a selected candidate item number currently
// -----------------------------------------------------------------------------
//
TInt CAknFepUICtrlJapanesePredictivePane::CurrentItemIndexOfCandidate()
    {
    if (IsVisible())
        {
        CAknFepUICtrlJapanesePredictiveListbox* listBox =
            static_cast<CAknFepUICtrlJapanesePredictiveListbox*>(ListBox());
        return listBox->CurrentCandidateNo();
        }
    return -1;      // Disable window
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::MoveItem
// Move a current item in an appointed direction
// -----------------------------------------------------------------------------
//
TInt CAknFepUICtrlJapanesePredictivePane::MoveItem(
        TAknFepUICtrlScroll aDirection)
    {
    __ASSERT_DEBUG(iListBox->IsVisible(),
                   AknFepUiInterfacePanic(EAknFepUiInterfacePanicNoCandidatePane));

    TInt ret = 0;

    switch(aDirection)
        {
        case EScrollLeft:
            ret = MovePreviousItem();
            break;

        case EScrollRight:
            ret = MoveNextItem();
            break;

        case EScrollUp:
            ret = MovePreviousLineItem();
            break;

        case EScrollDown:
            ret = MoveNextLineItem();
            break;

        default:
        //Panic
            AknFepUiInterfacePanic(EAknFepUiInterfacePanicCandidatePaneNotSupportScrollDirection);
            break;
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::SetupWindowLayout
// Set up a window layout type
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictivePane::SetupWindowLayout(
        AknPopupLayouts::TAknPopupLayouts /*aType*/)
    {
    HandleSizeChanged(Layout(), NULL, ListBox(), this);
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::AttemptExitL
// Set the title for the selection list
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictivePane::AttemptExitL(TBool aAccept)
    {
    if (iReturn) //Always not null unless ExecuteLD leaves
        *iReturn = aAccept;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::MopSupplyObject
// -----------------------------------------------------------------------------
//
TTypeUid::Ptr CAknFepUICtrlJapanesePredictivePane::MopSupplyObject(TTypeUid aId)
    {
    SetMopParent( iEikonEnv->EikAppUi() );
    return CAknPopupList::MopSupplyObject(aId);
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::CAknFepUICtrlJapanesePredictivePane
// Constructor
// -----------------------------------------------------------------------------
//
CAknFepUICtrlJapanesePredictivePane::CAknFepUICtrlJapanesePredictivePane(CAknFepUIManagerJapanese* aUiMng)
    : iUiMng(aUiMng)
    {
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictivePane::ConstructL()
    {
    // Create listbox
    CAknFepUICtrlJapanesePredictiveListbox* list =
                new(ELeave) CAknFepUICtrlJapanesePredictiveListbox;
    // "iListBox" member variable is defined in CAknPopupList class.
    iListBox = list;
    CAknPopupList::ConstructL(
        list, R_AKNFEP_SOFTKEYS_CONVERT_OTHER_SELECT, AknPopupLayouts::EMenuWindow);

    list->ConstructL(this, CEikListBox::ELeftDownInViewRect);
    list->CreateScrollBarFrameL(ETrue);
    list->ScrollBarFrame()->SetScrollBarVisibilityL(
                                CEikScrollBarFrame::EOff,
                                CEikScrollBarFrame::EAuto);

    SetNonFocusing();
    MakeVisible(EFalse);
    iLineInfoArray = NULL;
    iListTextArray = NULL;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::CreatePredictivePopupLineInfoArrayL
// Create a array with line information in candidate list
// -----------------------------------------------------------------------------
CPredictivePopupLineInfoArray*
CAknFepUICtrlJapanesePredictivePane::CreatePredictivePopupLineInfoArrayL(
        CDesCArrayFlat* aItems,
        const TInt aMaxDisplayWidthInPixel,
        const TInt aFontId)
    {
    CPredictivePopupLineInfoArray* lineArray =
        new(ELeave) CPredictivePopupLineInfoArray(1);
    CleanupStack::PushL(lineArray);

    // getting font object from font id
    const CFont* font = AknLayoutUtils::FontFromId(aFontId);

    TInt displayWidthInPixel = aMaxDisplayWidthInPixel;
    TInt elementStartXPos = 0;

    TBuf<KMaxListColumns*2> bufferOfLine;
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
            PredictivePopupAppendLineL(lineArray, bufferOfLine, elementArray);
            CleanupStack::Pop(elementArray);    // elementArray
            elementArray = NULL;
            bufferOfLine.SetLength(0);

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
            bufferOfLine.Append(elementText);
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
                PredictivePopupAppendLineL(lineArray, bufferOfLine, elementArray);
                CleanupStack::Pop(elementArray);    // elementArray
                elementArray = NULL;
                bufferOfLine.SetLength(0);

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
            bufferOfLine.Append(KSpaceCharCode);
            displayWidthInPixel -= widthOfSpaceInPixel;
            elementStartXPos    += widthOfSpaceInPixel;
            }
        else
            {
            // adding a line information if it isn't possible to add space
            PredictivePopupAppendLineL(lineArray, bufferOfLine, elementArray);
            CleanupStack::Pop(elementArray);    // elementArray
            elementArray = NULL;
            bufferOfLine.SetLength(0);

            displayWidthInPixel = aMaxDisplayWidthInPixel - 1;
            elementStartXPos = 0;
            }
        }

    if (bufferOfLine.Length() > 0 && elementArray)
        {
        PredictivePopupAppendLineL(lineArray, bufferOfLine, elementArray);
        CleanupStack::Pop(elementArray);    // elementArray
        }

    CleanupStack::Pop(lineArray);    // lineArray

    return lineArray;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::PredictivePopupAppendLineL
// Append to array with line information
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictivePane::PredictivePopupAppendLineL(
        CPredictivePopupLineInfoArray* aLineArray,
        const TDesC& aLineText,
        CPredictivePopupElementArray* aElementArray)
    {
    HBufC* lineTextBuf = HBufC::NewL(aLineText.Length());
    CleanupStack::PushL(lineTextBuf);
    TPtr lineTextBufPtr = lineTextBuf->Des();
    lineTextBufPtr.Copy(aLineText);

    CPredictivePopupLineInfo* lineInfo = new(ELeave) CPredictivePopupLineInfo;
    CleanupStack::PushL(lineInfo);

    aLineArray->AppendL(lineInfo);
    CleanupStack::Pop(2, lineTextBuf); // lineTextBuf, lineInfo

    // After that set to line information items,
    // becasue the following member variables are deleted
    // in the destractor of CPredictivePopupLineInfo if leave occurs.
    lineInfo->iLineText = lineTextBuf;
    lineInfo->iElementsOnLine = aElementArray;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::CreateListTextArrayL
// Create arrary with string to show in a line
// -----------------------------------------------------------------------------
//
CDesCArrayFlat* CAknFepUICtrlJapanesePredictivePane::CreateListTextArrayL(
        CPredictivePopupLineInfoArray* aLineArray)
    {
    CDesCArrayFlat* textArray = new(ELeave) CDesCArrayFlat(1);
    CleanupStack::PushL(textArray);

    const TInt numberOfItem = aLineArray->Count();
    TInt i;
    for (i = 0; i < numberOfItem; i++)
        {
        CPredictivePopupLineInfo* lineInfo = aLineArray->At(i);
        textArray->AppendL(*lineInfo->iLineText);
        }

    CleanupStack::Pop(textArray);    // textArray

    return textArray;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::HandleSizeChanged
// The handler function for size change
//
// Original function is AknPopupLayouts::HandleSizeChanged()
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictivePane::HandleSizeChanged(
        TAknPopupWindowLayoutDef& aDef,
        CAknPopupHeadingPane* aHeading,
        CEikListBox* aListBox,
        CCoeControl* aWindowOwningControl)
    {
    TInt numofitems = aListBox->Model()->NumberOfItems();

    TInt minItems = KJapPredictiveVisibleListRowsMin;
    TInt maxItems = KJapPredictiveVisibleListRowsMax;

    if (aDef.iMaximumHeight)
        {
        maxItems = aDef.iMaximumHeight;
        }
    AknPopupLayouts::CheckRange(numofitems, minItems, maxItems);
    CalcPopupMenuWindow(aDef, iMainPaneRect, numofitems);
    SetupMenuPopupWindow(aDef, numofitems, aHeading != 0);

    aWindowOwningControl->SetRect(AknPopupLayouts::WindowRect(aDef));
    HandleSizeAndPositionOfComponents(aDef, aListBox, aHeading);

    TRect windowRect = AknPopupLayouts::WindowRect(aDef);
    MAknsControlContext *cc = AknsDrawUtils::ControlContext( aListBox );
    TBool defaultContext = EFalse;
    if (!cc)
        {
        cc = aListBox->View()->ItemDrawer()->SkinBackgroundControlContext();
        defaultContext = ETrue;
        }

    if (cc)
        {
        CAknsBasicBackgroundControlContext *bcc =
            (CAknsBasicBackgroundControlContext*)cc;
        TAknLayoutRect popupBgRect;

        popupBgRect.LayoutRect(
            windowRect,
            ELayoutEmpty,
            0, 0, ELayoutEmpty, ELayoutEmpty,
            windowRect.Size().iWidth, windowRect.Size().iHeight);

        bcc->SetBitmap(KAknsIIDQsnFrPopupCenter);

        if (defaultContext)
            {
            bcc->SetRect(popupBgRect.Rect());
            }

        bcc->SetParentPos(aWindowOwningControl->PositionRelativeToScreen());

        if (defaultContext)
            {
            bcc->SetParentPos(TPoint(0, 0));
            }
        }
     }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::HandleDraw
// The handler function for Draw
//
// Original function is AknPopupLayouts::HandleDraw()
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictivePane::HandleDraw(CEikonEnv* /*aEikEnv*/,
                  CWindowGc& aGc,
                  const TAknPopupWindowLayoutDef& aDef,
                  CEikListBox* aListBox,
                  CAknPopupHeadingPane* /*aHeading*/)
    {
    aGc.Clear();

    TRect windowRect =
        AknPopupLayouts::MenuRect(const_cast<TAknPopupWindowLayoutDef&>(aDef));
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    MAknsControlContext* cc = AknsDrawUtils::ControlContext(aListBox);
    TBool windowFrameDrawn = EFalse;
    if (!cc)
        {
        cc = aListBox->View()->ItemDrawer()->SkinBackgroundControlContext();
        }

    if (cc)
        {
        // drawing frame
        TAknLayoutRect topLeft;
        topLeft.LayoutRect(
            windowRect,
            ELayoutEmpty,
            0, 0, ELayoutEmpty, ELayoutEmpty, KListboxFlameMarge, KListboxFlameMarge);

        TAknLayoutRect bottomRight;
        bottomRight.LayoutRect(
            windowRect,
            ELayoutEmpty,
            ELayoutEmpty, ELayoutEmpty, 0, 0, KListboxFlameMarge, KListboxFlameMarge);

        TRect outerRect = TRect(topLeft.Rect().iTl, bottomRight.Rect().iBr);
        TRect innerRect = TRect(topLeft.Rect().iBr, bottomRight.Rect().iTl);
        aGc.SetPenStyle(CGraphicsContext::ENullPen);
        const TAknsItemID *frameId = &KAknsIIDQsnFrPopupSub;
        const TAknsItemID *frameCenterId = &KAknsIIDQsnFrPopupCenter;
        windowFrameDrawn = AknsDrawUtils::DrawFrame(
                skin, aGc, outerRect, innerRect, *frameId, *frameCenterId);
        }

    if (!windowFrameDrawn)
        {
        DrawWindowShadow(
            aGc,
            aDef.iCover, aDef.iSecond, aDef.iFirst, aDef.iOutline, aDef.iInside);
        }
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::CalcPopupMenuWindow
// Set up the information on a pop-up window
//
// Original function is AknPopupLayouts::CalcPopupMenuWindow()
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictivePane::CalcPopupMenuWindow(
        TAknPopupWindowLayoutDef &aDef,const TRect& aRect, TInt /*aNum*/)
    {
    TAknLayoutRect popup_menu_window;

    TBool sizeDesideFlag = EFalse;
    TInt adjustOffsetX = 0;
    TInt adjustOffsetY = 0;

    TAknWindowLineLayout laf =
        AKN_LAYOUT_WINDOW_popup_fep_japan_predictive_window(
            0, iMaximumRows - KJapPredictiveVisibleListRowsMin);
    laf.il = 0;
    laf.it = 0;
    TAknWindowLineLayout maxlaf =
        AKN_LAYOUT_WINDOW_popup_fep_japan_predictive_window(
            0, KJapPredictiveVisibleListRowsMax - KJapPredictiveVisibleListRowsMin);
    laf.il = 0;
    maxlaf.it = 0;

    popup_menu_window.LayoutRect(aRect, laf);

    // checking whether list can be drawn under the position which popup window is dispalyed
    if (iMainPaneRect.iTl.iY
        + iOffsetPosition.iY + KLAFBelowTextBaseline
        + maxlaf.iH < iMainPaneRect.iBr.iY)
        {
        // deciding the position which popup window is dispalyed
        adjustOffsetY = (-1) * KLAFBelowTextBaseline;
        sizeDesideFlag = ETrue;
        }

        // checking whether list can be drawn top the position which popup window is dispalyed
    else if (iMainPaneRect.iTl.iY
            + iOffsetPosition.iY - KLAFAboveTextBaseline
            - iSrcTextHeight
            - popup_menu_window.Rect().Height() > iMainPaneRect.iTl.iY)
        {
        // deciding the position which popup window is dispalyed
        adjustOffsetY = iSrcTextHeight + KLAFAboveTextBaseline + popup_menu_window.Rect().Height();
        sizeDesideFlag = ETrue;
        }

    // display the bottom of window in case the displaying position isn't decided
    //
    if (!sizeDesideFlag)
        {
        TAknWindowLineLayout laf =
            AKN_LAYOUT_WINDOW_popup_fep_japan_predictive_window(
                0, KJapPredictiveVisibleListRowsMax - KLAFBaseListRows);
        laf.il = 0;
        laf.it = 0;
        popup_menu_window.LayoutRect(aRect, laf);
        adjustOffsetY =
            iMainPaneRect.iTl.iY
            + iOffsetPosition.iY + KLAFBelowTextBaseline
            + popup_menu_window.Rect().Height() - iMainPaneRect.iBr.iY;

        iMaximumRows = KJapPredictiveVisibleListRowsMin;
        sizeDesideFlag = ETrue;
        }

    adjustOffsetX =
        iMainPaneRect.iTl.iX
        + iOffsetPosition.iX
        + popup_menu_window.Rect().Width() - iMainPaneRect.iBr.iX;
    adjustOffsetX = (adjustOffsetX > 0) ? adjustOffsetX : 0;

    TPoint adjustOffset(-adjustOffsetX, -adjustOffsetY);

    aDef.iWindowRect = popup_menu_window.Rect();
    aDef.iWindowRect.Move(iOffsetPosition);
    aDef.iWindowRect.Move(adjustOffset);

    TRect window_rect = AknPopupLayouts::MenuRect(aDef);
    aDef.iCover.LayoutRect(
        window_rect,
        AKN_LAYOUT_WINDOW_Predictive_candidate_selection_popup_window_graphics_Line_1(aDef.iWindowRect));
    aDef.iSecond.LayoutRect(
        window_rect,
        AKN_LAYOUT_WINDOW_Predictive_candidate_selection_popup_window_graphics_Line_2(aDef.iWindowRect));
    aDef.iFirst.LayoutRect(
        window_rect,
        AKN_LAYOUT_WINDOW_Predictive_candidate_selection_popup_window_graphics_Line_2(aDef.iWindowRect));
    aDef.iOutline.LayoutRect(
        window_rect,
        AKN_LAYOUT_WINDOW_Predictive_candidate_selection_popup_window_graphics_Line_3(aDef.iWindowRect));
    aDef.iInside.LayoutRect(
        window_rect,
        AKN_LAYOUT_WINDOW_Predictive_candidate_selection_popup_window_graphics_Line_4(aDef.iWindowRect));
    aDef.iPopupMenuWindowOffset = TPoint(0, 0);
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::SetupMenuPopupWindow
// Set up the information on a pop-up window
//
// Original function is AknPopupLayouts::SetupMenuPopupWindow()
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictivePane::SetupMenuPopupWindow(TAknPopupWindowLayoutDef &aDef, TInt /*num*/, TBool /*heading*/)
    {
    AknPopupLayouts::SetupDefaults(aDef);
    TRect window_rect = AknPopupLayouts::MenuRect(aDef);

    // setting size of listbox pane
    TAknWindowLineLayout laf =
        AKN_LAYOUT_WINDOW_popup_fep_japan_predictive_window(0, iMaximumRows - KLAFBaseListRows);

    laf.il = KListboxMarge;
    laf.it = KListboxMarge;
    laf.iW -= KListboxMarge*2;
    laf.iH = laf.iH - KListboxMarge*2 - (laf.iH-KListboxMarge*2)%iMaximumRows;
    aDef.iListRect.LayoutRect(window_rect, laf);
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::HandleSizeAndPositionOfComponents
// The handler function for size and a location
//
// Original function is AknPopupLayouts::HandleSizeAndPositionOfComponents()
//
//
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictivePane::HandleSizeAndPositionOfComponents(
        const TAknPopupWindowLayoutDef &aDef,
        CCoeControl *aContent,
        CAknPopupHeadingPane* /*aHeading*/)
    {
    TRect lbrect = aDef.iListRect.Rect();

   if (AknLayoutUtils::ScalableLayoutInterfaceAvailable())
        {
        // set scroll size
        CAknFepUICtrlJapanesePredictiveListbox* listBox =
            static_cast<CAknFepUICtrlJapanesePredictiveListbox*>(aContent);
        CAknFepUICtrlJapanesePredictiveListBoxModel* model =
            static_cast<CAknFepUICtrlJapanesePredictiveListBoxModel*>(listBox->Model());
        TInt maxRows = model->LineInfoArray()->Count();
        if (maxRows > KJapPredictiveVisibleListRowsMax)
            {
            TAknWindowLineLayout scrollLineLayout = AknLayoutScalable_Avkon::scroll_pane_cp29().LayoutLine();
            lbrect.iBr.iX -= (scrollLineLayout.iW + KScrollRightMarge - KListboxMarge);
            }
        }

    aContent->SetRect(lbrect);
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::DrawWindowShadow
// Draw window shadow
//
// Original function is AknDrawWithSkins::DrawWindowShadow()
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictivePane::DrawWindowShadow(
        CWindowGc& aGc,
        const TAknLayoutRect& aCoverRect,
        const TAknLayoutRect& /*aSecondShadowRect*/,
        const TAknLayoutRect& aFirstShadowRect,
        const TAknLayoutRect& aOutlineFrameRect,
        const TAknLayoutRect& aInsideAreaRect)
    {
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();

    // draw rectangle
    aGc.SetBrushColor(aCoverRect.Color());
    AknsDrawUtils::Background(skin, NULL, NULL, aGc, aCoverRect.Rect());

    aFirstShadowRect.DrawOutLineRect(aGc);

    aOutlineFrameRect.DrawOutLineRect(aGc);

    aGc.SetBrushStyle(CGraphicsContext::ESolidBrush);
    aGc.SetBrushColor(aInsideAreaRect.Color());
    AknsDrawUtils::Background(skin, NULL, NULL, aGc, aInsideAreaRect.Rect());
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::MaximumRows
// calculate maximum columns from items of listbox
// -----------------------------------------------------------------------------
//
TInt CAknFepUICtrlJapanesePredictivePane::MaximumRows(
        CTextListBoxModel* aModel)
    {
    CAknFepUICtrlJapanesePredictiveListBoxModel* model =
        static_cast<CAknFepUICtrlJapanesePredictiveListBoxModel*>(aModel);
    TInt maxRows = model->LineInfoArray()->Count();
    TInt maxVisibleRows = Min(KJapPredictiveVisibleListRowsMax, maxRows);
    maxVisibleRows = Max(KJapPredictiveVisibleListRowsMin, maxVisibleRows);
    return maxVisibleRows;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::Draw
// Called by the framework to draw the control
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictivePane::Draw(const TRect& /*aRect*/) const
    {
    CWindowGc& gc = SystemGc();
    gc.Clear();
    HandleDraw(iEikonEnv, gc, Layout(), iListBox, Heading());
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::OfferKeyEventL
// Handle key events. When a key event occurs
// -----------------------------------------------------------------------------
//
TKeyResponse CAknFepUICtrlJapanesePredictivePane::OfferKeyEventL(
        const TKeyEvent& /*aKeyEvent*/,TEventCode /*aType*/)
    {
    return EKeyWasNotConsumed;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::MovePreviousItem
// selecting a next candidate
// -----------------------------------------------------------------------------
//
TInt CAknFepUICtrlJapanesePredictivePane::MovePreviousItem()
    {
    CAknFepUICtrlJapanesePredictiveListbox* listBox =
        static_cast<CAknFepUICtrlJapanesePredictiveListbox*>(ListBox());

    return listBox->MovePreviousItem();
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::MoveNextItem
// selecting a next candidate
// -----------------------------------------------------------------------------
//
TInt CAknFepUICtrlJapanesePredictivePane::MoveNextItem()
    {
    CAknFepUICtrlJapanesePredictiveListbox* listBox =
        static_cast<CAknFepUICtrlJapanesePredictiveListbox*>(ListBox());

    return listBox->MoveNextItem();
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::MovePreviousLineItem
// selecting a previous line candidate
// -----------------------------------------------------------------------------
//
TInt CAknFepUICtrlJapanesePredictivePane::MovePreviousLineItem()
    {
    CAknFepUICtrlJapanesePredictiveListbox* listBox =
        static_cast<CAknFepUICtrlJapanesePredictiveListbox*>(ListBox());

    return listBox->MovePreviousLineItem();
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::MoveNextLineItem
// selecting a next line candidate
// -----------------------------------------------------------------------------
//
TInt CAknFepUICtrlJapanesePredictivePane::MoveNextLineItem()
    {
    CAknFepUICtrlJapanesePredictiveListbox* listBox =
        static_cast<CAknFepUICtrlJapanesePredictiveListbox*>(ListBox());

    return listBox->MoveNextLineItem();
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::UpdateCbaL
// Changing the cba
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictivePane::UpdateCbaL(TInt aResourceID)
    {
    if (iCurrentResource == aResourceID)
        {
        return;
        }

    // store resource id
    iPopoutCba->SetCommandSetL( aResourceID );
    iCurrentResource = aResourceID;
    iPopoutCba->DrawDeferred();
    }

/**
 * Processes events from the softkeys. Responds to EAknSoftkeyOk and EAknSoftkeyBack
 * to accept or cancel the pop-up.
 *
 * @param   aCommandId  Event Id from the soft-key
 */
void CAknFepUICtrlJapanesePredictivePane::ProcessCommandL(TInt aCommandId)
    {
    // Respond to softkey events
    switch (aCommandId)
        {
        case EAknFepSoftkeyConvert:
        case EAknFepSoftkeyCloseWindow:
        case EAknFepSoftkeyCommit:
        case EAknFepSoftkeySelectCandidate:
        case EAknFepSoftkeyOtherModes:
            iUiMng->HandleCommandL(aCommandId);
            break;
        default:
            CAknPopupList::ProcessCommandL(aCommandId);
            break;
        }
    }

void CAknFepUICtrlJapanesePredictivePane::ShowWindowCbaL()
    {
    RDrawableWindow* cbaWindow = iPopoutCba->ButtonGroup()->AsControl()->DrawableWindow();
    cbaWindow->SetOrdinalPosition(0,ECoeWinPriorityFep);
    cbaWindow->SetFaded(EFalse, RWindowTreeNode::EFadeWindowOnly);
    cbaWindow->SetNonFading(ETrue);
    iEikonEnv->EikAppUi()->AddToStackL(iPopoutCba,
                                                 ECoeStackPriorityFep,
                                                 ECoeStackFlagRefusesFocus);

    iPopoutCba->ActivateL();
    iPopoutCba->MakeVisible(ETrue);
    iPopoutCba->DrawDeferred(); // to update the cba
    }

// ---------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::HandleListBoxEventL
// CallBack from ListBox
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepUICtrlJapanesePredictivePane::HandleListBoxEventL(
    CEikListBox* /*aListBox*/, TListBoxEvent aEventType)
    {
    switch (aEventType)
        {
        case EEventItemDraggingActioned: // Item Moved
            DrawNow();
            break;
        case EEventItemClicked:
            {
            TKeyEvent ccpuStart = {EKeyOK, EStdKeyDevice3, 0, 0};
            iEikonEnv->SimulateKeyEventL(ccpuStart, EEventKey);
            }
            break;
        default:
           break;
        }
    }

// End of File
