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
*       Provides the CAknFepUICtrlCandidateCharacter methods.
*
*/











#include <AknUtils.h>
#include <AknsDrawUtils.h>
#include <eiklabel.h>
#include <skinlayout.cdl.h>
#include <aknlayoutscalable_avkon.cdl.h>

#include "AknFepUiInputStateBase.h"
#include "AknFepUIAvkonCtrlCandidateCharacter.h"
#include "AknFepUIAvkonCtrlPinyinPopup.h"
#include "AknFepUiInterfacePanic.h"
#include "AknFepUIAvkonCtrlCandidatePane.h"


CAknFepUICtrlCandidateCharacter* CAknFepUICtrlCandidateCharacter::NewL( RWindowTreeNode& aParent,
																		MAknFepUIEventObserver* aObserver )
    {
    CAknFepUICtrlCandidateCharacter* self = new(ELeave) CAknFepUICtrlCandidateCharacter;
    CleanupStack::PushL(self);
    self->ConstructL( aParent, aObserver );
    CleanupStack::Pop();
    return self;
    }

CAknFepUICtrlCandidateCharacter::~CAknFepUICtrlCandidateCharacter()
    {
    delete iCandidateLabel;
    }

const TDesC* CAknFepUICtrlCandidateCharacter::Text() const
	{
	return iCandidateLabel->Text();
	}

void CAknFepUICtrlCandidateCharacter::SetText( const TDesC& aCharacter )
    {
    // make sure that we only copy as many characters as we can show
    TInt textLen = aCharacter.Length();
    TInt textLength = textLen > EMaxSelectedCandidateLength? EMaxSelectedCandidateLength:textLen;
    TPtrC newText = aCharacter.Left( textLength );
    TPtr ptr = iBuffer.Des();
    if ( ptr.Compare( newText ) != 0 )
        {
        ptr.Copy( newText );
        
        // ignore any errors... we have guaranteed that the descriptor already has sufficient characters to avoid a realloc
        TRAP_IGNORE( iCandidateLabel->SetTextL( iBuffer )); 
        }
    }

void CAknFepUICtrlCandidateCharacter::SizeChanged()
    {
    LayoutRects();
    LayoutContainedControls();
    }

TInt CAknFepUICtrlCandidateCharacter::CountComponentControls() const
    {
    CCoeControl* controls[] = 
        {
        iCandidateLabel
        } ;

    TInt count = 0 ;
    for ( TUint ii = 0; ii < ( sizeof(controls) / sizeof(CCoeControl*)) ;ii++ )
    	{
        if(controls[ii])
        	{
            count++;
        	}
    	}
    return count ;
    }

CCoeControl* CAknFepUICtrlCandidateCharacter::ComponentControl( TInt aIndex ) const 
    {
    CCoeControl* controls[] = 
        {
        iCandidateLabel
        } ;

    for (TUint ii = 0; ( ii < sizeof(controls) / sizeof(CCoeControl*)); ii++ )
    	{
        if ( controls[ii] && aIndex-- == 0 )
        	{
            return controls[ii];
        	}
    	}
    // shouldn't be called while no components.
    return NULL ;
    }

void CAknFepUICtrlCandidateCharacter::Draw( const TRect& /*aRect*/ ) const
    {
    CWindowGc& gc = SystemGc();

    TBool skinnedDraw = EFalse;
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    TRect outerRect;
    TRect innerRect;
    CalculateFrameRects( outerRect, innerRect );

    // draw the whole frame background according to the containing context, which in this case
    // is the candidate pane, to avoid corner drawing mess and to cope with masked grid frame centre
    MAknsControlContext* cc = AknsDrawUtils::ControlContext(this);
    
    // Set highlight color to gray
    if( iIsHighligt )
    	{
        
        skinnedDraw = AknsDrawUtils::DrawFrame( skin, gc, outerRect, innerRect,
												KAknsIIDQsnFrKeypadButtonFrPressed, 
												KAknsIIDDefault ); 
    	}
    else
    	{  

		skinnedDraw = AknsDrawUtils::DrawFrame( skin, gc, outerRect, innerRect,
												KAknsIIDQsnFrKeypadButtonFrNormal, 
												KAknsIIDDefault );
    	}

    if ( !skinnedDraw )
        {
        iRectShadow.DrawRect( gc );
        iRectHighlight.DrawRect( gc );
        }
    }

void CAknFepUICtrlCandidateCharacter::ConstructL( RWindowTreeNode& aParent, MAknFepUIEventObserver* aObserver )
    {
    CreateWindowL( aParent );

    iCandidateLabel = new(ELeave) CEikLabel;
    iCandidateLabel->SetBufferReserveLengthL( EMaxSelectedCandidateLength ); // this will do a realloc now, so no need to later
    iCandidateLabel->SetContainerWindowL( *this );
	iCandidateLabel->SetLabelAlignment( ELayoutAlignCenter );
    }

CAknFepUICtrlCandidateCharacter::CAknFepUICtrlCandidateCharacter()
    {
    }

void CAknFepUICtrlCandidateCharacter::LayoutRects()
    {
    TRect rect = Rect();
    iRectShadow.LayoutRect( rect, AKN_LAYOUT_WINDOW_Candidate_selection_highlight_Line_1(rect));
    iRectHighlight.LayoutRect( rect, AKN_LAYOUT_WINDOW_Candidate_selection_highlight_Line_2(rect));
    iRectShadow.LayoutRect( rect, AknLayoutScalable_Avkon::cell_hyb_candi_pane_g1( 0 ));
    }


void CAknFepUICtrlCandidateCharacter::LayoutContainedControls()
    {
    TRect rect = Rect();
 
    TAknTextLineLayout candidateLayout = AknLayoutScalable_Avkon::cell_hyb_candi_pane_t1(0);
        
    TAknLayoutText layoutText;
    layoutText.LayoutText( rect, candidateLayout );
    TRect rectText = layoutText.TextRect();
    const CFont* font = layoutText.Font();

    iCandidateLabel->SetRect( rectText );
    iCandidateLabel->SetFont( font );

    // override with skin text color
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    TRgb labelTextColor;

    AknsUtils::GetCachedColor( skin, labelTextColor, 
                               KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG11 );

    TRAP_IGNORE( 
	    iCandidateLabel->OverrideColorL(EColorLabelText, labelTextColor);	   
	    );
    }

void CAknFepUICtrlCandidateCharacter::CalculateFrameRects(TRect& aOuterRect, TRect& aInnerRect) const
    {
    TRect windowRect = Rect();

    TAknLayoutRect topLeft;
    topLeft.LayoutRect( windowRect, SkinLayout::Highlight_skin_placing__grid__Line_2());

    TAknLayoutRect bottomRight;
    bottomRight.LayoutRect( windowRect, SkinLayout::Highlight_skin_placing__grid__Line_5());

    aOuterRect = TRect( topLeft.Rect().iTl, bottomRight.Rect().iBr );
    aInnerRect = TRect( topLeft.Rect().iBr, bottomRight.Rect().iTl );
    }

void CAknFepUICtrlCandidateCharacter::SetHighlight( TBool bHighlight )
	{
	iIsHighligt = bHighlight;
	DrawDeferred();
	}

// End of file
