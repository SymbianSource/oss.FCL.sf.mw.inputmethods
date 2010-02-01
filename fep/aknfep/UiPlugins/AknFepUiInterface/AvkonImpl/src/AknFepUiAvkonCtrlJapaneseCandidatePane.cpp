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
* Description:            Candidate list
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
#include <AknLayout.lag>
#include <AknFep.rsg>

#include <skinlayout.cdl.h>
#include <AknLayout2ScalableDef.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <layoutmetadata.cdl.h>

#include "AknFepUiInterfacePanic.h"
#include "AknFepUIAvkonCtrlJapaneseCandidateListbox.h"
#include "AknFepUIAvkonCtrlJapaneseCandidateHeadingPane.h"
#include "AknFepUIAvkonCtrlJapaneseCandidatePane.h"
#include "AknFepUIManagerJapanese.h"

// CONSTANTS
const TInt KMaxTitleLength = 32;

const TInt KMaxListRows = 7;
const TInt KMinListRows = 5;
const TInt KMaxListColumns = 9;
const TInt KMinListColumns = 3;
const TInt KLAFBaseListColumns = 3;
const TInt KLAFBaseListRows = 1;

const TInt KLAFBelowTextBaseline = 2;
const TInt KLAFAboveTextBaseline = 1;
const TInt KListboxMarge = 4;

/**
*  CAknFepUICtrlJapaneseCandidatePane class.
*
*/

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidatePane::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAknFepUICtrlJapaneseCandidatePane* CAknFepUICtrlJapaneseCandidatePane::NewL(CAknFepUIManagerJapanese* aUiMng)
    {
    CAknFepUICtrlJapaneseCandidatePane* self =
        new(ELeave) CAknFepUICtrlJapaneseCandidatePane(aUiMng);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);    // self
    return self;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidatePane::~CAknFepUICtrlJapaneseCandidatePane
// Destructor
// -----------------------------------------------------------------------------
//
CAknFepUICtrlJapaneseCandidatePane::~CAknFepUICtrlJapaneseCandidatePane()
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
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidatePane::ShowWindow
// Show Pop-up list
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidatePane::ShowWindow()
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
// CAknFepUICtrlJapaneseCandidatePane::CloseCandidateList
// Close Pop-up list
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidatePane::CloseCandidateList()
    {
    CancelPopup();
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidatePane::SetCurrentItemIndex
// Specify the cursor position of a pop-up list.
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidatePane::SetCurrentItemIndexL(
        TInt aItemIndex,
        TBool aDraw)
    {
    CTextListBoxModel* model =
        static_cast<CTextListBoxModel*>(ListBox()->Model());
    TInt numOfItem = model->NumberOfItems();
    TBuf<KMaxTitleLength> title;
    title.Format(_L("%d / %d"), aItemIndex + 1, numOfItem);
    SetTitleL(title);

    if (aDraw)
        {
        iListBox->SetCurrentItemIndexAndDraw(aItemIndex);
        iTitle->DrawNow();
        }
    else
        {
        iListBox->SetCurrentItemIndex(aItemIndex);
        }

    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidatePane::SetOffsetPosition
// Specify the display position of a pop-up list
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidatePane::SetOffsetPosition(TPoint aOffset)
    {
    iOffsetPosition = aOffset;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidatePane::SetWindowPosition
// Set the drawing position for candidate list popup window
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidatePane::SetWindowPosition(
        const TPoint& aLeftSideOfBaseLine, TInt aHeight, TInt aAscent)
    {
    // getting rectangle for main pane
    TRect screen =
        TRect(0, 0,
              AKN_LAYOUT_WINDOW_screen.iW, AKN_LAYOUT_WINDOW_screen.iH);
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
// CAknFepUICtrlJapaneseCandidatePane::ShowCandidateListL
// Show Pop-up list
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidatePane::ShowCandidateListL(
        CDesCArrayFlat* aItems, TInt aIndex, TInt aResourceID)
    {
    __ASSERT_DEBUG(aItems->Count() > 0,
                   AknFepUiInterfacePanic(EAknFepUiInterfacePanicCandidatesEmpty));

    // Set listitems.
    CTextListBoxModel* model =
        static_cast<CTextListBoxModel*>(ListBox()->Model());
    model->SetItemTextArray(aItems);
    model->SetOwnershipType(ELbmDoesNotOwnItemArray);

    iMaximumColumns = MaximumColumns(model);
    iMaximumRows    = MaximumRows(model);

    SetCurrentItemIndexL(aIndex, EFalse);
    SetMaximumHeight(iMaximumRows);

    CAknFepUICtrlJapaneseCandidateListbox* listBox =
        static_cast<CAknFepUICtrlJapaneseCandidateListbox*>(ListBox());

    listBox->SetMaximumColumns(iMaximumColumns);

    // Show popup list
    TBool notShowingPopup = model->NumberOfItems() == 0 && iWindowType < 0x80;
    if (!notShowingPopup)
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
    iPopoutCba->MakeVisible(ETrue);

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
// CAknFepUICtrlJapaneseCandidatePane::HideWindow
// Hide Pop-up list
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidatePane::HideWindow()
    {
    MakeVisible(EFalse);
    Window().SetPointerCapture(EFalse);

    iEikonEnv->RemoveFromStack(iPopoutCba);

    iPopoutCba->MakeVisible(EFalse);
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidatePane::CurrentItemIndexOfCandidate
// Return a selected candidate item number currently
// -----------------------------------------------------------------------------
//
TInt CAknFepUICtrlJapaneseCandidatePane::CurrentItemIndexOfCandidate()
    {
    return iListBox->CurrentItemIndex();
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidatePane::MoveItem
// Move a current item in an appointed direction
// -----------------------------------------------------------------------------
//
TInt CAknFepUICtrlJapaneseCandidatePane::MoveItem(
        TAknFepUICtrlScroll aDirection)
    {
    __ASSERT_DEBUG(iListBox->IsVisible(),
                   AknFepUiInterfacePanic(EAknFepUiInterfacePanicNoCandidatePane));

    TInt index = iListBox->CurrentItemIndex();

    CTextListBoxModel* model =
        static_cast<CTextListBoxModel*>(ListBox()->Model());
    const TInt numberOfItems = model->NumberOfItems();

    switch(aDirection)
        {
        case EScrollUp:
            {
            --index;
            if (index < 0)
                index = numberOfItems - 1;
            }
        break;

        case EScrollDown:
            {
            ++index;
            if (index >= numberOfItems)
                index = 0;
            }
        break;

        default:
            {
            //Panic
            AknFepUiInterfacePanic(EAknFepUiInterfacePanicCandidatePaneNotSupportScrollDirection);
            }
        break;
        }
    TRAPD(err, SetCurrentItemIndexL(index, ETrue));
    if (err != KErrNone)
        {
        HideWindow();
        index = 0;
        }
    return index;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidatePane::SetupWindowLayout
// Set up a window layout type
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidatePane::SetupWindowLayout(
        AknPopupLayouts::TAknPopupLayouts /*aType*/)
    {
    HandleSizeChanged(Layout(), Heading(), ListBox(), this);
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidatePane::AttemptExitL
// Set the title for the selection list
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidatePane::AttemptExitL(TBool aAccept)
    {
    if (iReturn) //Always not null unless ExecuteLD leaves
        *iReturn = aAccept;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidatePane::MopSupplyObject
// -----------------------------------------------------------------------------
//
TTypeUid::Ptr CAknFepUICtrlJapaneseCandidatePane::MopSupplyObject(TTypeUid aId)
    {
    SetMopParent( iEikonEnv->EikAppUi() );
    return CAknPopupList::MopSupplyObject(aId);
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapanesePredictivePane::CAknFepUICtrlJapanesePredictivePane
// Constructor
// -----------------------------------------------------------------------------
//
CAknFepUICtrlJapaneseCandidatePane::CAknFepUICtrlJapaneseCandidatePane(CAknFepUIManagerJapanese* aUiMng)
    : iUiMng(aUiMng)
    {
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidatePane::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidatePane::ConstructL()
    {
    // Create listbox.
    CAknFepUICtrlJapaneseCandidateListbox* list =
                new(ELeave) CAknFepUICtrlJapaneseCandidateListbox;
    // "iListBox" member variable is defined in CAknPopupList class.
    iListBox = list;
    CAknPopupList::ConstructL(
        iListBox, R_AKNFEP_SOFTKEYS_EMPTY_EMPTY_COMMIT, AknPopupLayouts::EMenuWindow);

    list->ConstructL(this, CEikListBox::ELeftDownInViewRect);
    list->CreateScrollBarFrameL(ETrue);
    list->ScrollBarFrame()->SetScrollBarVisibilityL(
                                CEikScrollBarFrame::EOff,
                                CEikScrollBarFrame::EAuto);
    iEikonEnv->RemoveFromStack(this);
    SetNonFocusing();
    MakeVisible(EFalse);
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidatePane::SetTitleL
// Set the title for the selection list
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidatePane::SetTitleL(const TDesC& aTitle)
    {
    if (!iTitle)
        {
        iTitle = new(ELeave) CAknFepUICtrlJapaneseCandidateHeadingPane;
        iTitle->SetContainerWindowL(*this);
        iTitle->ConstructL(aTitle);
        iTitle->SetMopParent(iListBox);
        }
    else
        {
        iTitle->SetTextL(aTitle);
        }
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidatePane::HandleSizeChanged
// The handler function for size change
// Original function is AknPopupLayouts::HandleSizeChanged()
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidatePane::HandleSizeChanged(
        TAknPopupWindowLayoutDef &aDef,
        CAknPopupHeadingPane *aHeading,
        CEikListBox *aListBox,
        CCoeControl *aWindowOwningControl)
    {
    if ( aHeading )
        {
        aHeading->SetLayout( CAknPopupHeadingPane::EListHeadingPane );
        aHeading->SetSkinFrameId(TAknsItemID());
        aHeading->SetSkinFrameCenterId(TAknsItemID());
        }

    TInt numofitems = aListBox->Model()->NumberOfItems();

    TInt minItems = iMaximumRows;
    TInt maxItems = KMaxListRows;

    if (aDef.iMaximumHeight)
        {
        maxItems = aDef.iMaximumHeight;
        }
    AknPopupLayouts::CheckRange(numofitems, minItems, maxItems);
    CalcPopupMenuWindow(aDef, iMainPaneRect, numofitems);
    SetupMenuPopupWindow(aDef, numofitems, aHeading != 0);

    CAknFepUICtrlJapaneseCandidateListbox* listbox =
        static_cast<CAknFepUICtrlJapaneseCandidateListbox*>(aListBox);
    listbox->SetMaximumColumns(iMaximumColumns);
    listbox->SetMaximumRows(iMaximumRows);
    SetMaximumHeight(iMaximumRows);

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
            bcc->SetParentPos(TPoint(0,0));
            }
        }
     }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidatePane::HandleDraw
// The handler function for Draw
// Original function is AknPopupLayouts::HandleDraw()
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidatePane::HandleDraw(CEikonEnv* /*aEikEnv*/,
                  CWindowGc& aGc,
                  const TAknPopupWindowLayoutDef& aDef,
                  CEikListBox* aListBox,
                  CAknPopupHeadingPane* aHeading)
    {
    TRect windowRect =
        AknPopupLayouts::MenuRect(const_cast<TAknPopupWindowLayoutDef&>(aDef));
    MAknsSkinInstance *skin = AknsUtils::SkinInstance();
    MAknsControlContext *cc = AknsDrawUtils::ControlContext( aListBox );
    TBool windowFrameDrawn = EFalse;
    if (!cc)
        {
        cc = aListBox->View()->ItemDrawer()->SkinBackgroundControlContext();
        }

    if (cc)
        {
		TRegionFix<2> dontDrawRegion;
		TRegionFix<8> drawRegion;
		if ( aListBox )
		    {
	    	dontDrawRegion.AddRect( aListBox->Rect() );
		    }
		if ( aHeading )
		    {
	    	dontDrawRegion.AddRect( aHeading->Rect() );
		    }
		drawRegion.AddRect( windowRect );
		drawRegion.SubRegion( dontDrawRegion );

		const TAknsItemID *popupBgId = &KAknsIIDQsnBgPopupBackground;
		TAknLayoutRect popupBgRect;
        popupBgRect.LayoutRect(windowRect, SkinLayout::Popup_windows_skin_placing__background_slice__Line_1(windowRect));
		AknsDrawUtils::DrawCachedImage(skin, aGc, popupBgRect.Rect(), *popupBgId);

		const TAknsItemID *sliceId = &KAknsIIDQsnBgSlicePopup;
		TAknLayoutRect sliceRect;
        sliceRect.LayoutRect(windowRect, SkinLayout::Popup_windows_skin_placing__background_slice__Line_2());
		AknsDrawUtils::DrawCachedImage(skin, aGc, sliceRect.Rect(), *sliceId);

        // drawing frame
        TAknLayoutRect topLeft;
        topLeft.LayoutRect(
            windowRect,
            ELayoutEmpty, 0,0, ELayoutEmpty, ELayoutEmpty, KListboxMarge, KListboxMarge);

        TAknLayoutRect bottomRight;
        bottomRight.LayoutRect(
            windowRect,
            ELayoutEmpty,
            ELayoutEmpty, ELayoutEmpty, 0, 0, KListboxMarge, KListboxMarge);

        TRect outerRect = TRect(topLeft.Rect().iTl, bottomRight.Rect().iBr);
        TRect innerRect = TRect(topLeft.Rect().iBr, bottomRight.Rect().iTl);
        aGc.SetPenStyle(CGraphicsContext::ENullPen);
        const TAknsItemID *frameId = &KAknsIIDQsnFrPopup;
        const TAknsItemID *frameCenterId = &KAknsIIDQsnFrPopupCenter;
		aGc.SetClippingRegion( drawRegion );
        windowFrameDrawn =
            AknsDrawUtils::DrawFrame(
                skin, aGc,
                outerRect, innerRect, *frameId, *frameCenterId);
		aGc.CancelClippingRegion();
        }

    if (!windowFrameDrawn)
        {
        DrawWindowShadow(
            aGc, aDef.iCover,
            aDef.iSecond, aDef.iFirst, aDef.iOutline, aDef.iInside);
        }
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidatePane::CalcPopupMenuWindow
// Set up the information on a pop-up window
// Original function is AknPopupLayouts::CalcPopupMenuWindow()
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidatePane::CalcPopupMenuWindow(
        TAknPopupWindowLayoutDef &aDef,const TRect& aRect, TInt /*num*/)
    {
    TAknLayoutRect popup_menu_window;

    TBool sizeDesideFlag = EFalse;
    TInt adjustOffsetX = 0;
    TInt adjustOffsetY = 0;
    TInt i;
    TAknWindowLineLayout lafwidth =
        AknLayoutScalable_Avkon::popup_fep_japan_candidate_window(iMaximumColumns - KLAFBaseListColumns).LayoutLine();

    // calculating the number of indication line again in order to be able to arrange under a plan position
    for (i = iMaximumRows; i >= KLAFBaseListRows && !sizeDesideFlag; i--)
        {
        TAknWindowLineLayout laf =
            AknLayoutScalable_Avkon::popup_fep_japan_candidate_window(i).LayoutLine();
        laf.il = 0;
        laf.it = 0;
        laf.iW = lafwidth.iW;
        popup_menu_window.LayoutRect(aRect, laf);
        // calculate a drawing position Y
        TInt drawY = iMainPaneRect.iTl.iY + iOffsetPosition.iY
                         + KLAFBelowTextBaseline + popup_menu_window.Rect().Height();
        if (drawY < iMainPaneRect.iBr.iY)
            {
            adjustOffsetY = (-1) * KLAFBelowTextBaseline;
            iMaximumRows = i;
            sizeDesideFlag = ETrue;
            }
        if (iMaximumRows >= KMinListRows && i == KMinListRows)
            {
            break;
            }
        }

    // if popup window cannot be arranged under a plan positon, it try whether popup is arranged top or not.
    //
    for (i = iMaximumRows; i >= KLAFBaseListRows && !sizeDesideFlag; i--)
        {
        TAknWindowLineLayout laf =
            AknLayoutScalable_Avkon::popup_fep_japan_candidate_window(i).LayoutLine();
        laf.il = 0;
        laf.it = 0;
        laf.iW = lafwidth.iW;
        popup_menu_window.LayoutRect(aRect, laf);
        // calculate a drawing position Y
        TInt drawY = iMainPaneRect.iTl.iY + iOffsetPosition.iY - KLAFAboveTextBaseline
                    - iSrcTextHeight - popup_menu_window.Rect().Height();
        if (drawY > iMainPaneRect.iTl.iY)
            {
            adjustOffsetY = iSrcTextHeight + KLAFAboveTextBaseline + popup_menu_window.Rect().Height();
            iMaximumRows = i;
            sizeDesideFlag = ETrue;
            }
        if (iMaximumRows >= KMinListRows && i == KMinListRows)
            {
            break;
            }
        }

    // In case popup window cannot be arrange both under and top,
    // popup is shown on the bottom on screen.
    if (!sizeDesideFlag)
        {
        TAknWindowLineLayout laf =
            AknLayoutScalable_Avkon::popup_fep_japan_candidate_window(KMinListRows).LayoutLine();
        laf.il = 0;
        laf.it = 0;
        laf.iW = lafwidth.iW;
        popup_menu_window.LayoutRect(aRect, laf);
        adjustOffsetY = iMainPaneRect.iTl.iY + iOffsetPosition.iY + KLAFBelowTextBaseline
                      + popup_menu_window.Rect().Height()
                      - iMainPaneRect.iBr.iY;
        iMaximumRows = KMinListRows;
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

    // getting a information of shadow in candidate list
    TRect window_rect = AknPopupLayouts::MenuRect(aDef);

    TAknWindowLineLayout laf1 =
        AKN_LAYOUT_WINDOW_Candidate_selection_pop_up_window_graphics_Line_1(
            aDef.iWindowRect);
    laf1.iW = TInt16(aDef.iWindowRect.Width());
    TAknWindowLineLayout laf2 =
        AKN_LAYOUT_WINDOW_Candidate_selection_pop_up_window_graphics_Line_2(
            aDef.iWindowRect);
    laf2.iW = TInt16(aDef.iWindowRect.Width() - 4);
    TAknWindowLineLayout laf3 =
        AKN_LAYOUT_WINDOW_Candidate_selection_pop_up_window_graphics_Line_3(
            aDef.iWindowRect);
    laf3.iW = TInt16(aDef.iWindowRect.Width() - 3);
    TAknWindowLineLayout laf4 =
        AKN_LAYOUT_WINDOW_Candidate_selection_pop_up_window_graphics_Line_4(
            aDef.iWindowRect);
    laf4.iW = TInt16(aDef.iWindowRect.Width() - 5);

    aDef.iCover.LayoutRect(window_rect, laf1);
    aDef.iSecond.LayoutRect(window_rect, laf2);
    aDef.iFirst.LayoutRect(window_rect, laf2);
    aDef.iOutline.LayoutRect(window_rect, laf3);
    aDef.iInside.LayoutRect(window_rect, laf4);
    aDef.iPopupMenuWindowOffset = TPoint(0, 0);
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidatePane::SetupMenuPopupWindow
// Set up the information on a pop-up window
// Original function is AknPopupLayouts::SetupMenuPopupWindow()
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidatePane::SetupMenuPopupWindow(
        TAknPopupWindowLayoutDef &aDef, TInt /*num*/, TBool /*heading*/)
    {
    AknPopupLayouts::SetupDefaults(aDef);
    TRect window_rect = AknPopupLayouts::MenuRect(aDef);

    // setting window size of listbox pane
    // Those arguments aren't used in it.
    TAknWindowLineLayout laf = AKN_LAYOUT_WINDOW_candidate_pane(0,0);
    TAknWindowLineLayout textWindowLayout =
        AKN_LAYOUT_WINDOW_list_single_popup_jap_candidate_pane(0,0);
    laf.ib = ELayoutEmpty;
    laf.iH = iMaximumRows * textWindowLayout.iH;
    aDef.iListRect.LayoutRect(window_rect, laf);

    // setting window size of heading pane
    //   calculating window size of heading pane based on a shadow of popup window
    //
    TAknTextLineLayout lafHead =
        AknLayoutScalable_Avkon::popup_fep_japan_candidate_window_t1().LayoutLine();

    TRect rectFirstShadow = aDef.iFirst.Rect();
    TRect rectListBox     = aDef.iListRect.Rect();
    laf.it = (TInt16)(rectFirstShadow.iTl.iY - window_rect.iTl.iY);
    laf.ib = ELayoutEmpty;
    laf.iH = (TInt16)(rectListBox.iTl.iY - rectFirstShadow.iTl.iY);
    aDef.iHeadingRect.LayoutRect(window_rect, laf);

    CAknFepUICtrlJapaneseCandidateHeadingPane* headingPane =
        static_cast<CAknFepUICtrlJapaneseCandidateHeadingPane*>(Heading());
    headingPane->SetParentRect(aDef.iWindowRect);
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidatePane::HandleSizeAndPositionOfComponents
// The handler function for size and a location
// Original function is AknPopupLayouts::HandleSizeAndPositionOfComponents()
//
//
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidatePane::HandleSizeAndPositionOfComponents(
        const TAknPopupWindowLayoutDef &aDef,
        CCoeControl *aContent,
        CAknPopupHeadingPane *aHeading)
    {
    TRect lbrect = aDef.iListRect.Rect();
    TRect headrect;

   if (AknLayoutUtils::ScalableLayoutInterfaceAvailable())
        {
        // set scroll size
        CAknFepUICtrlJapaneseCandidateListbox* listBox =
            static_cast<CAknFepUICtrlJapaneseCandidateListbox*>(aContent);
        CTextListBoxModel* model = static_cast<CTextListBoxModel*>(listBox->Model());
        const TInt items = model->NumberOfItems();
        if (items > listBox->MaximumRows())
            {
            TAknWindowLineLayout scrollllinelayout = AknLayoutScalable_Avkon::scroll_pane_cp30().LayoutLine();
            lbrect.iBr.iX -= scrollllinelayout.iW;
            }
        }

    // set listbox view rectangle
    aContent->SetRect(lbrect);
    if (aHeading)
        {
        headrect = aDef.iHeadingRect.Rect();
        aHeading->SetRect(headrect);
        }

    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidatePane::DrawWindowShadow
// Draw window shadow
//
// Original function is AknDrawWithSkins::DrawWindowShadow()
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidatePane::DrawWindowShadow(
        CWindowGc &aGc,
        const TAknLayoutRect &aCoverRect,
        const TAknLayoutRect &aSecondShadowRect,
        const TAknLayoutRect &aFirstShadowRect,
        const TAknLayoutRect &aOutlineFrameRect,
        const TAknLayoutRect &aInsideAreaRect)
    {
    MAknsSkinInstance *skin = AknsUtils::SkinInstance();

    TInt x2 = aFirstShadowRect.Rect().iTl.iX;
    TInt x3 = aSecondShadowRect.Rect().iTl.iX;

    TInt y2 = aFirstShadowRect.Rect().iTl.iY;
    TInt y3 = aSecondShadowRect.Rect().iTl.iY;

    TInt x4 = aOutlineFrameRect.Rect().iBr.iX;
    TInt x5 = aFirstShadowRect.Rect().iBr.iX;
    TInt x6 = aSecondShadowRect.Rect().iBr.iX;

    TInt y4 = aOutlineFrameRect.Rect().iBr.iY;
    TInt y5 = aFirstShadowRect.Rect().iBr.iY;
    TInt y6 = aSecondShadowRect.Rect().iBr.iY;

    aGc.SetBrushColor(aCoverRect.Color());
    AknsDrawUtils::Background(skin, NULL, NULL, aGc, aCoverRect.Rect());
    AknsDrawUtils::BackgroundBetweenRects(
        skin, NULL, NULL, aGc,
        TRect(x2,y2,x4-1,y4-1), aInsideAreaRect.Rect());

    aOutlineFrameRect.DrawOutLineRect(aGc);
    aGc.SetBrushColor(aFirstShadowRect.Color());
    aGc.Clear(TRect(x2,y4,x5,y5));
    aGc.Clear(TRect(x4,y2,x5,y4));
    aGc.SetBrushColor(aSecondShadowRect.Color());
    aGc.Clear(TRect(x3,y5,x6,y6));
    aGc.Clear(TRect(x5,y3,x6,y5));
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidatePane::MaximumColumns
// This function returns maximum columns to be able to display in listbox.
// -----------------------------------------------------------------------------
TInt CAknFepUICtrlJapaneseCandidatePane::MaximumColumns(
        CTextListBoxModel* aModel)
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
// CAknFepUICtrlJapaneseCandidatePane::MaximumRows
// This function returns maximum rows to be able to display in listbox.
// -----------------------------------------------------------------------------
TInt CAknFepUICtrlJapaneseCandidatePane::MaximumRows(
        CTextListBoxModel* aModel)
    {
    TInt numberOfItems = aModel->NumberOfItems();
    TInt maximumRows = Min(KMaxListRows, numberOfItems);

    return maximumRows;
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidatePane::Draw
// Called by the framework to draw the control
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidatePane::Draw(const TRect& /*aRect*/) const
    {
    CWindowGc& gc = SystemGc();
    gc.Clear();
    HandleDraw(iEikonEnv, gc, Layout(), iListBox, Heading());
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidatePane::OfferKeyEventL
// Handle key events. When a key event occurs
// -----------------------------------------------------------------------------
//
TKeyResponse CAknFepUICtrlJapaneseCandidatePane::OfferKeyEventL(
        const TKeyEvent& /*aKeyEvent*/,TEventCode /*aType*/)
    {
    return EKeyWasNotConsumed;
    }

void CAknFepUICtrlJapaneseCandidatePane::ShowWindowCbaL()
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

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidatePane::ProcessCommandL
// Processes events from the softkeys. Responds to EAknSoftkeyOk and EAknSoftkeyBack
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidatePane::ProcessCommandL(TInt aCommandId)
    {
    // Respond to softkey events
    switch (aCommandId)
        {
        case EAknFepSoftkeyCommit:
            iUiMng->HandleCommandL(aCommandId);
            break;
        case EAknFepSoftkeyRetroActiveHalf:
        case EAknFepSoftkeyRetroActiveFull:
            iUiMng->HandleCommandL(aCommandId);
            break;
        default:
            CAknPopupList::ProcessCommandL(aCommandId);
            break;
        }
    }

// -----------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidatePane::UpdateCbaL
// Changing the cba
// -----------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidatePane::UpdateCbaL(TInt aResourceID)
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

// CallBack from ListBox
// ---------------------------------------------------------------------------
// CAknFepUICtrlJapaneseCandidatePane::HandleListBoxEventL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepUICtrlJapaneseCandidatePane::HandleListBoxEventL(
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
