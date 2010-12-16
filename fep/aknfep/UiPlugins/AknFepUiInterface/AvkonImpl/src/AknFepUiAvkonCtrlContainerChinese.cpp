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
* Description:           
*       Provides the CAknFepUICtrlContainerChinese methods.
*
*/












#include <coemain.h>
#include <eikenv.h>
#include <eikappui.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <uikon/eikdefmacros.h>
#endif


#include <AknLayoutDef.h>
#include <AknLayout.lag>
#include <AknUtils.h>
#include <AknsDrawUtils.h>
#include <AknsFrameBackgroundControlContext.h>
#include <skinlayout.cdl.h>
#include <AknDef.h>

#include "AknFepUiInterfacePanic.h"
#include "AknFepUIAvkonCtrlContainerChinese.h"
#include "AknFepUIAvkonCtrlCandidatePane.h"
#include "AknFepUIAvkonCtrlInputPane.h"
#include "AknFepUIAvkonCtrlPinyinPopup.h"
#include "aknfepuictrlpinyineep.h"
#include "AknFepManager.h"
#include "aknfepchineseuidataconv.h"
#include "aknfepuilayoutdatamgr.h"

MAknFepUICtrlContainerChinese::~MAknFepUICtrlContainerChinese()
	{
	}
	
CAknFepUICtrlContainerChinese::CAknFepUICtrlContainerChinese()
    :
    iPaneLayout(ELayoutInput),
    iContainerPosition(TPoint(0,0)),
    isPCLPosed(EFalse)
    {

    }

CAknFepUICtrlContainerChinese* CAknFepUICtrlContainerChinese::NewL()
    {
    CAknFepUICtrlContainerChinese* self = new(ELeave) CAknFepUICtrlContainerChinese;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

MAknFepUICtrlInputPane* CAknFepUICtrlContainerChinese::InputPane() const
    {
    return iInputPane;
    }

MAknFepUICtrlCandidatePane* CAknFepUICtrlContainerChinese::CandidatePane() const
    {
    return iCandidatePane;
    }

MAknFepUICtrlPinyinPopup* CAknFepUICtrlContainerChinese::PinyinPopupWindow() const
    {
    return iPinyinPopupWindow;
    }

MAknFepUICtrlEditPane* CAknFepUICtrlContainerChinese::EditPaneWindow() const
    {
    return iEditPane;
    }

TBool CAknFepUICtrlContainerChinese::IsEnabled() const
    {
    return iEnabled;
    }

void CAknFepUICtrlContainerChinese::Enable(TBool aEnable)
    {
	if(!COMPARE_BOOLS(iEnabled, aEnable))
        {
        iEnabled = aEnable;
        if(aEnable)
            {
            Window().SetOrdinalPosition(0, ECoeWinPriorityFep); // right at the front
            // make sure we are not faded even if a query has appeared
            Window().SetFaded(EFalse, RWindowTreeNode::EFadeIncludeChildren); 
            }
        else
            {
            // make sure that the popup window is disabled as well
            iInputPane->MakeVisible(aEnable); 
            iPinyinPopupWindow->Enable(aEnable);
            iEditPane->Enable(aEnable);
            iCandidatePane->Enable(aEnable);
            }
        MakeVisible(aEnable);
        }
    if ( aEnable == EFalse )
        {
        if ( iFepMan )
            {
            iFepMan->PinyinPhraseCreation( EFalse );
            iFepMan->EntryPhrase( EFalse );
            iFepMan->EnableKeyStar( ETrue );
            iFepMan->PhraseCreation( EFalse );
            }
        PinyinPopupWindow()->ResetStrokeArray();
        PinyinPopupWindow()->SetLastResouce( 0 );
	    iPinyinPopupWindow->ResetInputLayoutMiniQwertyZhuyin();
#ifdef RD_INTELLIGENT_TEXT_INPUT
	    iEditPane->ResetAllArray();
	    iEditPane->SetEffictiveLength( 0 );
	    iEditPane->SetChangeState( EFalse );
#endif
        }
    }  
  
void CAknFepUICtrlContainerChinese::ConstructL()
    {
    CreateWindowL();
    SetNonFocusing();
    RWindow& window=Window();
    window.SetTransparencyAlphaChannel();
    window.SetOrdinalPosition(0, ECoeWinPriorityFep); // right at the front
    MakeVisible(EFalse);

    iEikonEnv->EikAppUi()->AddToStackL(this, ECoeStackPriorityDefault, ECoeStackFlagRefusesFocus);

    iLafDataMgr = CAknFepUiLayoutDataMgr::NewL();
    iInputPane = CAknFepUICtrlInputPane::NewL(window, CAknFepUICtrlInputPane::ELayoutPhraseInputPane, iLafDataMgr);
    iCandidatePane = CAknFepUICtrlCandidatePane::NewL(window, iLafDataMgr);
    iPinyinPopupWindow = CAknFepUICtrlPinyinPopup::NewL(window , iLafDataMgr);
    iEditPane = CAknFepUICtrlPinyinEEP::NewL( window, iLafDataMgr );
    iInputPane->SetMopParent(this);
    iCandidatePane->SetMopParent(this);
    iEditPane->SetMopParent(this);

    iInputPane->SetSizeChangedObserver(this);
    iPinyinPopupWindow->SetSizeChangedObserver(this);
    iPinyinPopupWindow->MakeVisible(EFalse);
    iEditPane->Enable(EFalse);

    iCandidatePane->SetPaneLayout(iPaneLayout); // set pane layout
    iCandidatePane->SetVisibleCandidateCount(iPaneLayout);
 	iHorizLine = 
 	    CAknFepChineseUIDataConv::AnyToLayoutRect(iLafDataMgr->RequestData(EHorizLine));	    
 	iEEPFirstHorizLine = 
 	    CAknFepChineseUIDataConv::AnyToLayoutRect(iLafDataMgr->RequestData(EEEPFirstHorizLine));
 	iEEPSecondHorizLine = 
 	    CAknFepChineseUIDataConv::AnyToLayoutRect(iLafDataMgr->RequestData(EEEPSecondHorizLine));
 	TInt index = ConvertLayoutToIndex();
    if ( index == 2 || index == 3 )
        {
        index = 0;
        }
    iInputPane->SetLayoutIndex(index);
    TRect outerRect = TRect();
    TRect innerRect = TRect();
    CalculateFrameRects(outerRect, innerRect);

    iBgContext = CAknsFrameBackgroundControlContext::NewL(KAknsIIDQsnFrPopup, outerRect, innerRect, ETrue);
    LayoutContainer();
    ActivateL();
    }

CAknFepUICtrlContainerChinese::~CAknFepUICtrlContainerChinese()
    {
    iEikonEnv->EikAppUi()->RemoveFromStack(this);
    delete iLafDataMgr;
    delete iInputPane;
    delete iCandidatePane;
    delete iPinyinPopupWindow;
    delete iEditPane;
    delete iBgContext;
    }

CCoeControl* CAknFepUICtrlContainerChinese::ComponentControl(TInt aIndex) const 
    {
    CCoeControl* controls[] = 
        {
        iInputPane,
        iCandidatePane,
        iEditPane,
        iPinyinPopupWindow
        } ;

    for (TUint ii = 0; (ii < sizeof(controls) / sizeof(CCoeControl*)) ; ii++)
        {
        if (controls[ii] && aIndex-- == 0)
            {
            return controls[ii] ;
            }
        }
    // shouldn't be called while no components.
    return NULL ;
    }

TInt CAknFepUICtrlContainerChinese::CountComponentControls() const
    {
    CCoeControl* controls[] = 
        {
        iInputPane,
        iCandidatePane,
        iEditPane,
        iPinyinPopupWindow
        } ;

    TInt count = 0 ;
    for (TUint ii = 0 ; ii < (sizeof(controls) / sizeof(CCoeControl*)) ; ii++)
        {
        if(controls[ii])
            {
            count++ ;
            }
        }
    return count ;
    }

TInt CAknFepUICtrlContainerChinese::ConvertLayoutToIndex() const
    {
    TInt layout = -1;
    // switch on the cases in reverse order, and count our way through
    // the possible layouts to determine the index into the layout
    // this means that it is easier to add another layout in between
    switch(iPaneLayout)
        {
        case ELayoutKeystroke: //index 3
            layout++;
        case ELayoutPhraseCreation: //index 2
            layout++;
	    case ELayoutCandidate: //index 1
		    layout++;
	    case ELayoutInput: //index 0
		    layout++;
        default:
            break;
        }
    return layout;
    }

void CAknFepUICtrlContainerChinese::LayoutFrames()
    {
    TRect outerRect;
    TRect innerRect;
    CalculateFrameRects(outerRect, innerRect);
    iBgContext->SetFrameRects(outerRect, innerRect);
    iBgContext->SetParentPos(PositionRelativeToScreen());
    }

void CAknFepUICtrlContainerChinese::CalculateFrameRects(TRect& aOuterRect, TRect& aInnerRect) const
    {
    TRect windowRect = Rect();
    TPoint lPoint(0,0);
    TPoint rPoint(0,0);
    TAknLayoutRect topLeft;
    topLeft.LayoutRect(windowRect, SkinLayout::Submenu_skin_placing_Line_2());

    TAknLayoutRect bottomRight;
    bottomRight.LayoutRect(windowRect, SkinLayout::Submenu_skin_placing_Line_5());
    lPoint.iX = bottomRight.Rect().iTl.iX;
    lPoint.iY = bottomRight.Rect().iTl.iY;
    rPoint.iX = bottomRight.Rect().iBr.iX;
    rPoint.iY = bottomRight.Rect().iBr.iY;

    aOuterRect = TRect(topLeft.Rect().iTl, bottomRight.Rect().iBr);
    aInnerRect = TRect(topLeft.Rect().iBr, bottomRight.Rect().iTl);
    }

void CAknFepUICtrlContainerChinese::Draw(const TRect& /*aRect*/) const
    {
    CWindowGc& gc = SystemGc();
    TBool skinnedDraw = EFalse;
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();

    TRect outerRect;
    TRect innerRect;
    CalculateFrameRects(outerRect, innerRect);

    skinnedDraw = AknsDrawUtils::DrawFrame(
	    skin, gc, 
	    outerRect, innerRect, 
	    KAknsIIDQsnFrPopupSub, KAknsIIDQsnFrPopupCenterSubmenu); 

    TRgb separatorColor = iHorizLine.Color();
    AknsUtils::GetCachedColor( skin, separatorColor,
          KAknsIIDQsnLineColors, EAknsCIQsnLineColorsCG8 );
    gc.SetBrushStyle( CGraphicsContext::ESolidBrush );
    gc.SetBrushColor( separatorColor );

    if(!skinnedDraw)
	    {
	    iRectCoverMainPaneInput.DrawRect(gc);
	    iRectFirstShadowInput.DrawRect(gc);
	    iRectOutlineFrameInput.DrawRect(gc);
	    iRectInsideAreaInput.DrawRect(gc);
	    }

    } 

void CAknFepUICtrlContainerChinese::SizeChanged()
    {
    iLafDataMgr->ReadLafInfo();
    LayoutRects();
    LayoutContainedControls();
    LayoutFrames();
    }

void CAknFepUICtrlContainerChinese::SetLayout(TPaneLayout aPaneLayout)
    {
    if(iPaneLayout != aPaneLayout)
        {
        iPaneLayout = aPaneLayout;
        SetControlVisibility();
        // set visible candidate length 
        iCandidatePane->SetPaneLayout(aPaneLayout);
        TInt index = ConvertLayoutToIndex();
        if ( index == 2 || index == 3)
            {
            index = 0;
            }
        iCandidatePane->SetVisibleCandidateCount(aPaneLayout - index);
        iInputPane->SetLayoutIndex( index );
        LayoutContainer();
        DrawDeferred();
        }
    }

void CAknFepUICtrlContainerChinese::FocusCandidatePane(TBool aValue)
    {
    iInputPane->SetHighlighted(!aValue);
    iCandidatePane->SetHighlighted(aValue);
    }

void CAknFepUICtrlContainerChinese::SelectionPopupEnabled(TBool /*aEnabled*/)
    {
    // ensure that the input pane is only visible if the popup is not enabled
    SetControlVisibility();
    }

TTypeUid::Ptr CAknFepUICtrlContainerChinese::MopSupplyObject(TTypeUid aId)
    {
    if (aId.iUid == MAknsControlContext::ETypeId)
        return ( MAknsControlContext::SupplyMopObject(aId, iBgContext));
    return CCoeControl::MopSupplyObject(aId);
    }

void CAknFepUICtrlContainerChinese::SelectionPopupSizeChanged(TInt /*aNumLines*/)
    {
    LayoutContainer();
    DrawDeferred();
    }

void CAknFepUICtrlContainerChinese::LayoutPinyinPopupWindow()
    {
    TRect popupRect(Position(), Size()); 
    TRect pinyinRect = popupRect;
    TAknWindowLineLayout entryHorizLine = 
        CAknFepChineseUIDataConv::AnyToWindowLine( iLafDataMgr->RequestData( EEntryHorizLineWindow ));
    TAknWindowLineLayout entryPaneLine = 
        CAknFepChineseUIDataConv::AnyToWindowLine( iLafDataMgr->RequestData( EEntryPaneWindowLine ));
    TAknWindowLineLayout eepFirstHorizLine = 
        CAknFepChineseUIDataConv::AnyToWindowLine( iLafDataMgr->RequestData( EEEPFirstHorizLineWindow ));
    TAknWindowLineLayout eepSecondHorizLine = 
        CAknFepChineseUIDataConv::AnyToWindowLine( iLafDataMgr->RequestData( EEEPSecondHorizLineWindow ));
    TAknLayoutRect layoutLineEntry, layoutLineEntryPane, layoutLineFirstHorizEEP, layoutLineSecondHorizEEP;
    TRect layoutLineRectEntry, layoutLineRectEntryPane, layoutLineRectFirstHorizEEP, layoutLineRectSecondHorizEEP;
    TInt index = ConvertLayoutToIndex();
    if ( 2 == index )
        {
        layoutLineFirstHorizEEP.LayoutRect( popupRect, eepFirstHorizLine );
        layoutLineRectFirstHorizEEP = layoutLineFirstHorizEEP.Rect();
        layoutLineSecondHorizEEP.LayoutRect( popupRect, eepSecondHorizLine );
        layoutLineRectSecondHorizEEP = layoutLineSecondHorizEEP.Rect();
        
        pinyinRect.iTl.iY = layoutLineRectFirstHorizEEP.iBr.iY;
        pinyinRect.iBr.iY = layoutLineRectSecondHorizEEP.iTl.iY;
        }
    else
        {
        layoutLineEntry.LayoutRect( popupRect, entryHorizLine );
        layoutLineRectEntry = layoutLineEntry.Rect();
        layoutLineEntryPane.LayoutRect( popupRect, entryPaneLine );
        layoutLineRectEntryPane = layoutLineEntryPane.Rect();
        pinyinRect.iTl.iY = layoutLineRectEntryPane.iTl.iY;
        pinyinRect.SetHeight(layoutLineRectEntry.iTl.iY - pinyinRect.iTl.iY);
        }
    iPinyinPopupWindow->SetRect(pinyinRect);
    }

void CAknFepUICtrlContainerChinese::LayoutPinyinEEPWindow()
    {
    TRect rect = Rect();  
    TAknWindowLineLayout EEPWindowLayout;
    if ( iPaneLayout == ELayoutPhraseCreation )
    	{
    	EEPWindowLayout =
    	    CAknFepChineseUIDataConv::AnyToWindowLine(iLafDataMgr->RequestData(EEEPWindowLayout));
    	}
    else
    	{
    	EEPWindowLayout =
    	    	    CAknFepChineseUIDataConv::AnyToWindowLine(iLafDataMgr->RequestData(EEntryPaneWindowLine));
    	}       
    AknLayoutUtils::LayoutControl(iEditPane, rect, EEPWindowLayout);
    }

void CAknFepUICtrlContainerChinese::SetControlVisibility()
    {
    TBool popupEnabled = iPinyinPopupWindow->IsEnabled();

    switch(iPaneLayout)
	    {
    case ELayoutCandidate:
        // predicative candidate state
        iInputPane->MakeVisible(EFalse);
        iPhraseCreationLaf = EFalse;
        break;
    case ELayoutInput:
        // input state
        iInputPane->MakeVisible(!popupEnabled);
        iPhraseCreationLaf = EFalse;
        break;
    case ELayoutPhraseCreation:
        if ( !iPhraseCreationLaf )
            {
            // input state
            iInputPane->MakeVisible( !popupEnabled );
            }
        else
            {
            iPhraseCreationLaf= EFalse;
            }
    default:
	    break;
	    }
    }

void CAknFepUICtrlContainerChinese::SetIsShownWithPopupWindows(TBool aValue)
    {
    if(!COMPARE_BOOLS(iIsShownWithPopupWindows,aValue))	
        { 
        iIsShownWithPopupWindows = aValue;
        // need to layout container first, so that the parent rect is the right size...
        LayoutContainer();
        // ... now we can layout the rects relative to the resized parent
        LayoutRects();
        // don't need to layout controls, only the rects are affected
        DrawDeferred();
        }
    }

void CAknFepUICtrlContainerChinese::SetContainerPosition(TPoint aCurPos, TInt aHeight)
    {
    TRect parentRect;
    AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane, parentRect);
    TInt windowHeight = 0;
    TInt index = ConvertLayoutToIndex();
    
    if ( index != 2 )
        {
        windowHeight = 
            CAknFepChineseUIDataConv::AnyToInt( iLafDataMgr->RequestData( EPopupHeightEntry ));
        isPCLPosed = EFalse;
        }
    else
        {
        windowHeight = CAknFepChineseUIDataConv::AnyToInt( iLafDataMgr->RequestData( EPopupHeightEEP ));
        isPCLPosed = ETrue;
        }
    // Laf comments: In case there is enough space on the main pane to show the window under the entry,
    // the window is placed 3 pixels below the text baseline of the entry. In case there is no space on 
    // the main pane to show the window fully under the entry, the window is placed 17(16 pixel font) or
    // 13(12 pixel font) pixels above the baseline of the entry.
    if(aCurPos.iY + 3 + windowHeight < parentRect.iBr.iY) 
        {
        iContainerPosition.iY = aCurPos.iY + 3; 
        }
    else
        {
        iContainerPosition.iY = aCurPos.iY - windowHeight - (aHeight + 1); 
        }
    if ((( iContainerPosition.iY + windowHeight ) > parentRect.iBr.iY ) ||
        ( iContainerPosition.iY < parentRect.iTl.iY ))
    {
    iContainerPosition.iY = parentRect.iBr.iY - ( parentRect.iBr.iY - 
                            parentRect.iTl.iY ) / 2 - windowHeight / 2;
    }
    LayoutContainer();
    }

void CAknFepUICtrlContainerChinese::ShowHorizontalScrollArrows(TBool aValue)
    {
    iCandidatePane->ShowHorizontalScrollArrows(aValue);
    }    
   
void CAknFepUICtrlContainerChinese::ShowVerticalScrollArrows(TBool aValue)
    {
    iCandidatePane->ShowVerticalScrollArrows(aValue);
    }

void CAknFepUICtrlContainerChinese::LayoutInputPane()
    {
    TRect rect = Rect();
    TAknWindowLineLayout entryPaneWindowLine = 
        CAknFepChineseUIDataConv::AnyToWindowLine( iLafDataMgr->RequestData( EEntryPaneWindowLine ));
    TInt index = ConvertLayoutToIndex();
    if ( index == 2)
        {
        TInt eepPopupHeight = CAknFepChineseUIDataConv::AnyToInt( iLafDataMgr->RequestData( EPopupHeightEEP ));
        TInt entryPopupHeight = CAknFepChineseUIDataConv::AnyToInt( iLafDataMgr->RequestData( EPopupHeightEntry ));
        rect.iTl.iY = rect.iTl.iY + eepPopupHeight - entryPopupHeight;
        }
    AknLayoutUtils::LayoutControl(iInputPane, rect, entryPaneWindowLine);
    }

void CAknFepUICtrlContainerChinese::LayoutContainedControls()
    {
    LayoutInputPane();
    // layout candidate pane
    TAknWindowLineLayout candidatePaneWindowLine = 
                CAknFepChineseUIDataConv::AnyToWindowLine( iLafDataMgr->RequestData( ECandidatePaneWindowLine ));
    TInt index = ConvertLayoutToIndex();
    if (index == 2)
        {
        if ( isPCLPosed )
            {
            iEditPane->Enable( ETrue );      
            AknLayoutUtils::LayoutControl( iCandidatePane, Rect(), candidatePaneWindowLine );
            LayoutPinyinPopupWindow();
            LayoutPinyinEEPWindow();
            }
        }
    else if ( index == 0 || index == 1 ) 
        {
        TRect rect = Rect();
        iEditPane->Enable(EFalse);
        AknLayoutUtils::LayoutControl(iCandidatePane, rect, candidatePaneWindowLine);
        LayoutPinyinPopupWindow();
        }
    else
        {
        iEditPane->Enable(ETrue);
        iPinyinPopupWindow->Enable(EFalse);
        AknLayoutUtils::LayoutControl(iCandidatePane, Rect(), candidatePaneWindowLine);
        LayoutPinyinEEPWindow();
        }
    }

void CAknFepUICtrlContainerChinese::LayoutContainer()
    {
    TInt layoutIndex = ConvertLayoutToIndex(); // 0 : input state layout, 1 : candidate state layout
    TRect rect;
    switch ( layoutIndex )
        {
        case 2:
            rect = CAknFepChineseUIDataConv::AnyToRect( iLafDataMgr->RequestData( EPopupRectEEP ));
            break;
        case 1:
            rect = CAknFepChineseUIDataConv::AnyToRect( iLafDataMgr->RequestData( EPopupRectCandatate ));
            break;
        default:
            rect = CAknFepChineseUIDataConv::AnyToRect( iLafDataMgr->RequestData( EPopupRectEntry ));
            break;
        }
    TInt rectHeight = rect.iBr.iY - rect.iTl.iY;
    rect.iTl.iY = iContainerPosition.iY;
    rect.iBr.iY = iContainerPosition.iY + rectHeight;
    SetRect(rect); 
    }

void CAknFepUICtrlContainerChinese::LayoutRects()
    {
    TInt index = ConvertLayoutToIndex();
    TRect rect = Rect();

    if ( index == 3 )
        {
        index = 0;
        }
    
    switch (index) // 0 : input state, 1: predicative candidate state
	    {
	    case 0:
		    iRectCoverMainPaneInput = CAknFepChineseUIDataConv::AnyToLayoutRect( iLafDataMgr->RequestData( ERectCoverMainPaneInputEntry ));
		    iRectFirstShadowInput= CAknFepChineseUIDataConv::AnyToLayoutRect( iLafDataMgr->RequestData( ERectFirstShadowInputEntry ));
		    iRectOutlineFrameInput= CAknFepChineseUIDataConv::AnyToLayoutRect( iLafDataMgr->RequestData( ERectOutlineFrameInputEntry ));
		    iRectInsideAreaInput= CAknFepChineseUIDataConv::AnyToLayoutRect( iLafDataMgr->RequestData( ERectInsideAreaInputEntry ));
		    break;
        case 1:
		    iRectCoverMainPaneInput = CAknFepChineseUIDataConv::AnyToLayoutRect( iLafDataMgr->RequestData( ERectCoverMainPaneInputCandidate ));
		    iRectFirstShadowInput= CAknFepChineseUIDataConv::AnyToLayoutRect( iLafDataMgr->RequestData( ERectFirstShadowInputCandidate ));
		    iRectOutlineFrameInput= CAknFepChineseUIDataConv::AnyToLayoutRect( iLafDataMgr->RequestData( ERectOutlineFrameInputCandidate ));
		    iRectInsideAreaInput= CAknFepChineseUIDataConv::AnyToLayoutRect( iLafDataMgr->RequestData( ERectInsideAreaInputCandidate ));
		    break;
        case 2:
		    iRectCoverMainPaneInput = CAknFepChineseUIDataConv::AnyToLayoutRect( iLafDataMgr->RequestData( ERectCoverMainPaneInputEEP));
		    iRectFirstShadowInput= CAknFepChineseUIDataConv::AnyToLayoutRect( iLafDataMgr->RequestData( ERectFirstShadowInputEEP ));
		    iRectOutlineFrameInput= CAknFepChineseUIDataConv::AnyToLayoutRect( iLafDataMgr->RequestData( ERectOutlineFrameInputEEP ));
		    iRectInsideAreaInput= CAknFepChineseUIDataConv::AnyToLayoutRect( iLafDataMgr->RequestData( ERectInsideAreaInputEEP ));
            break;
        default: 
            break;
	    }
    }

void CAknFepUICtrlContainerChinese::HandleResourceChange(TInt aType)
    {
    // maybe should be KEikDynamicLayoutVariantSwitch_APAC, need to have a try after it's ready.
    if(aType == KEikDynamicLayoutVariantSwitch)
        {
        LayoutContainer();
        }
    else
        {
        CCoeControl::HandleResourceChange(aType);
        }
    }
void CAknFepUICtrlContainerChinese::SetControlInVisible( TBool aVisible )
    {
    ShowVerticalScrollArrows( EFalse );
    if ( aVisible == EFalse )
        {
        iCandidatePane->SetInVisiable();
        }
    iPinyinPopupWindow->MakeVisible( EFalse );
    iInputPane->MakeVisible( EFalse );
    iPhraseCreationLaf= ETrue;
    }


TInt& CAknFepUICtrlContainerChinese::IsPhraseCreation()
    {
    return iPhraseCreation;
    }

void CAknFepUICtrlContainerChinese::PhraseCreation(  TBool aPhraseCreation )
    {
    if ( iPhraseCreation != aPhraseCreation )
        {
        iPhraseCreation = aPhraseCreation;
        }
    }

void CAknFepUICtrlContainerChinese::SetFepMan( MAknFepManagerUIInterface* aFepMan )
    {
    iFepMan = aFepMan;
    }
