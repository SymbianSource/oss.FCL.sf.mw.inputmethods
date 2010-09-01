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
*       Provides the CAknFepUICtrlCandidateSelected methods.
*
*/











#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <uikon/eikdefmacros.h>
#endif
#include <eiklabel.h>
#include <AknUtils.h>
#include <AknsDrawUtils.h>
#include "AknFepUIAvkonCtrlCandidateSelected.h"
#include "AknFepUIAvkonCtrlPinyinPopup.h"
#include "AknFepUiInterfacePanic.h"
#include <skinlayout.cdl.h>

class CEikLabel;

CAknFepUICtrlCandidateSelected* CAknFepUICtrlCandidateSelected::NewL(RWindowTreeNode& aParent)
    {
    CAknFepUICtrlCandidateSelected* self = new(ELeave) CAknFepUICtrlCandidateSelected;
    CleanupStack::PushL(self);
    self->ConstructL(aParent);
    CleanupStack::Pop();
    return self;
    }

CAknFepUICtrlCandidateSelected::~CAknFepUICtrlCandidateSelected()
    {
    delete iOrdinalLabel;
    delete iCandidateLabel;
    }

void CAknFepUICtrlCandidateSelected::SetText(TPtrC aCharacter)
    {
    // make sure that we only copy as many characters as we can show
    TInt textLen = aCharacter.Length();
    TInt textLength = textLen > EMaxSelectedCandidateLength? EMaxSelectedCandidateLength:textLen;
    TPtrC newText = aCharacter.Left(textLength);
    TPtr ptr = iBuffer.Des();
    if(ptr != newText)
        {
        ptr.Copy(newText);
        // ignore any errors... we have guaranteed that the descriptor already has sufficient characters to avoid a realloc
        TRAP_IGNORE( iCandidateLabel->SetTextL(iBuffer)); 
        }
    }

void CAknFepUICtrlCandidateSelected::SetOrdinal(TInt aOrdinal)
    {
    __ASSERT_DEBUG(aOrdinal < CAknFepUICtrlPinyinPopup::EMaxPhraseCandidate, AknFepUiInterfacePanic(EAknFepUiInterfacePanicNewVisibleCountExceedsRange));
    __ASSERT_DEBUG(aOrdinal >= 0, AknFepUiInterfacePanic(EAknFepUiInterfacePanicNewVisibleCountExceedsRange));

    TBuf<2> buf;
    buf.Format(_L("%d"), aOrdinal);
    TRAP_IGNORE(iOrdinalLabel->SetTextL(buf));
    }

void CAknFepUICtrlCandidateSelected::SetCandidateMode(CAknFepUICtrlCandidatePane::TCandidateMode aMode)
    {
    if(iCandidateMode != aMode)
        {
        iCandidateMode = aMode;
        
        LayoutRects();
        LayoutContainedControls();
        DrawDeferred();
        }
    }

void CAknFepUICtrlCandidateSelected::SetLayout(CAknFepUICtrlContainerChinese::TPaneLayout aLayout)
    {
    iPaneLayout = aLayout;
    }

void CAknFepUICtrlCandidateSelected::ShowOrdinal(TBool aValue)
	{
	if(!COMPARE_BOOLS(iShowOrdinal,aValue))
		{
		iShowOrdinal = aValue;
		UpdateLabelVisibility();
		}
	}

void CAknFepUICtrlCandidateSelected::SizeChanged()
    {
    LayoutRects();
    LayoutContainedControls();
    }

TInt CAknFepUICtrlCandidateSelected::CountComponentControls() const
    {
    CCoeControl* controls[] = 
        {
        iOrdinalLabel,
        iCandidateLabel
        } ;

    TInt count = 0 ;
    for (TUint ii = 0 ; ii < (sizeof(controls) / sizeof(CCoeControl*)) ; ii++)
        if(controls[ii])
            count++ ;
    return count ;
    }

CCoeControl* CAknFepUICtrlCandidateSelected::ComponentControl(TInt aIndex) const 
    {
    CCoeControl* controls[] = 
        {
        iOrdinalLabel,
        iCandidateLabel
        } ;

    for (TUint ii = 0; (ii < sizeof(controls) / sizeof(CCoeControl*)) ; ii++)
        if (controls[ii] && aIndex-- == 0)
            return controls[ii] ;
    // shouldn't be called while no components.
    return NULL ;
    }

void CAknFepUICtrlCandidateSelected::Draw(const TRect& /*aRect*/) const
    {
    CWindowGc& gc = SystemGc();

    TBool skinnedDraw = EFalse;
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    TRect outerRect;
    TRect innerRect;
    CalculateFrameRects(outerRect, innerRect);

    // draw the whole frame background according to the containing context, which in this case
    // is the candidate pane, to avoid corner drawing mess and to cope with masked grid frame centre
    MAknsControlContext* cc = AknsDrawUtils::ControlContext(this);
    AknsDrawUtils::DrawBackground(skin, cc, this, gc, 
        outerRect.iTl, outerRect, KAknsDrawParamDefault);

    skinnedDraw = AknsDrawUtils::DrawFrame(skin, gc, outerRect, innerRect,
        KAknsIIDQsnFrGrid, KAknsIIDQsnFrGridCenter); 

    if(!skinnedDraw)
        {
        iRectShadow.DrawRect(gc);
        iRectHighlight.DrawRect(gc);
        }
    }

void CAknFepUICtrlCandidateSelected::ConstructL(RWindowTreeNode& aParent)
    {
    CreateWindowL(aParent);

    iOrdinalLabel = new(ELeave) CEikLabel;
    iOrdinalLabel->SetBufferReserveLengthL(EOneCandidate);
    iOrdinalLabel->SetContainerWindowL(*this);

    // force label to contain required number of characters from the outset
    iCandidateLabel = new(ELeave) CEikLabel;
    iCandidateLabel->SetBufferReserveLengthL(EMaxSelectedCandidateLength); // this will do a realloc now, so no need to later
    iCandidateLabel->SetContainerWindowL(*this);
    }

CAknFepUICtrlCandidateSelected::CAknFepUICtrlCandidateSelected()
    : 
    iCandidateMode(CAknFepUICtrlCandidatePane::ECandidateModeNormal),
    iPaneLayout(CAknFepUICtrlContainerChinese::ELayoutCandidate),
    iShowOrdinal(ETrue)
    {
    }

void CAknFepUICtrlCandidateSelected::LayoutRects()
    {
    TRect rect = Rect();
    iRectShadow.LayoutRect(rect, AKN_LAYOUT_WINDOW_Candidate_selection_highlight_Line_1(rect));
    iRectHighlight.LayoutRect(rect, AKN_LAYOUT_WINDOW_Candidate_selection_highlight_Line_2(rect));
    }

void CAknFepUICtrlCandidateSelected::LayoutContainedControls()
    {
    TRect rect = Rect();

    // layout candidate label
    TAknTextLineLayout candidateLayout = AKN_LAYOUT_TEXT_Chinese_universal_FEP_candidate_pane_texts_Line_1;
    TAknTextLineLayout ordinalLayout = AKN_LAYOUT_TEXT_Chinese_universal_FEP_candidate_pane_texts_Line_3; 
    candidateLayout.il = candidateLayout.il - ordinalLayout.il;

    TAknLayoutText layoutCandidate;
    layoutCandidate.LayoutText(rect,candidateLayout);
    
    const CFont* font = layoutCandidate.Font();
    TInt labelWidthInPixels = 0;
    if(font)
	    {
	    TInt bufLength = iBuffer.Length();
	    if(bufLength != 0)
		    {
		    TPtrC newChars = iBuffer.Left(bufLength);
		    labelWidthInPixels = font->TextWidthInPixels(newChars);
		    }
	    }
    TRect rectCandidate = layoutCandidate.TextRect();
    if(labelWidthInPixels == 0)
	    {
	    rectCandidate.iTl.iX = 0;
	    }
    rectCandidate.SetWidth(labelWidthInPixels);
    iCandidateLabel->SetRect(rectCandidate);
    iCandidateLabel->SetFont(font);
    
    // layout ordinal label
    TBufC16<1> buf;
    TPtr16 ptr = buf.Des();
    ptr = (*(iOrdinalLabel->Text())).Left(EOneCandidate);

    TAknLayoutText layoutOrdinal;
    ordinalLayout.il = 0;
	
    layoutOrdinal.LayoutText(rect,ordinalLayout);
    
    const CFont* fontOrdinal = layoutOrdinal.Font();
    
    TRect ordinalRect = layoutOrdinal.TextRect();

    // inserted in 2006.03.29
    TPoint ordinalPosition;
    // end inserting
    TInt ordinalWidth = fontOrdinal->TextWidthInPixels(ptr);
    
    /* removing in 2006.03.29
    ordinalRect.iTl.iX = rectCandidate.iTl.iX - ordinalWidth;
    ordinalRect.iTl.iY = rectCandidate.iTl.iY;

    ordinalRect.SetWidth(ordinalWidth);
    ordinalRect.SetHeight(fontOrdinal->HeightInPixels());
    */ // end removing

    // inserted in 2006.03.29
    ordinalPosition.iX = rectCandidate.iTl.iX - ordinalWidth;
    ordinalPosition.iY = rectCandidate.iTl.iY;

    ordinalRect = TRect(ordinalPosition,
                        TPoint(ordinalPosition.iX + ordinalRect.Width(),
                               ordinalPosition.iY + ordinalRect.Height()));
    // end inserting

    iOrdinalLabel->SetRect(ordinalRect);
    iOrdinalLabel->SetFont(fontOrdinal);

    // override with skin text color
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    TRgb labelTextColor;

    AknsUtils::GetCachedColor(skin, labelTextColor, 
                              KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG11);

    TRAP_IGNORE( 
	    iCandidateLabel->OverrideColorL(EColorLabelText, labelTextColor);
	    iOrdinalLabel->OverrideColorL(EColorLabelText, labelTextColor);
	    );
    }

void CAknFepUICtrlCandidateSelected::CalculateFrameRects(TRect& aOuterRect, TRect& aInnerRect) const
    {
    TRect windowRect = Rect();

    TAknLayoutRect topLeft;
    topLeft.LayoutRect(windowRect, SkinLayout::Highlight_skin_placing__grid__Line_2());

    TAknLayoutRect bottomRight;
    bottomRight.LayoutRect(windowRect, SkinLayout::Highlight_skin_placing__grid__Line_5());

    aOuterRect = TRect(topLeft.Rect().iTl, bottomRight.Rect().iBr);
    aInnerRect = TRect(topLeft.Rect().iBr, bottomRight.Rect().iTl);
    }

void CAknFepUICtrlCandidateSelected::UpdateLabelVisibility()
	{
	if(iShowOrdinal)
		{
		iOrdinalLabel->MakeVisible(ETrue);
		}
	else
		{
		iOrdinalLabel->MakeVisible(EFalse);
		}
	}

// End of file
