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
*       Provides the CAknFepUICtrlCandidatePane methods.
*
*/












#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <uikon/eikdefmacros.h>
#endif
#include <eiklabel.h>
#include <AknsDrawUtils.h>
#include <AknBidiTextUtils.h> 
#include <AknsUtils.h>
#include <AknIconUtils.h>
#include <coecntrl.h>
#include <aknfepuictrls.mbg>
#include <skinlayout.cdl.h>
#include <aknlayoutscalable_avkon.cdl.h>

#include "AknFepUiInterfacePanic.h"
#include "AknFepUIAvkonCtrlCandidatePane.h"
#include "aknfepchineseuidataconv.h"
#include "aknfepuilayoutdatamgr.h"
#include "AknFepUiInputStateBase.h"
#include "AknFepUIAvkonCtrlCandidateCharacter.h"
#include "data_caging_path_literals.hrh"

const TInt CandidateCountLeft = 10;
const TInt DibertHeight = 480;
const TInt ShowAboveCharacter = -1;
const TInt ShowBellowCharacter = 1;

_LIT(KFepUiControlsBitmapFile, "aknfepuictrls.mbm");

CAknFepUICtrlCandidatePane* CAknFepUICtrlCandidatePane::NewL( RWindow& aParent, 
															  CAknFepUiLayoutDataMgr* aLafDataMgr, 
															  MAknFepChineseUiMgr* aMgr )
    {
    CAknFepUICtrlCandidatePane* self = new(ELeave) CAknFepUICtrlCandidatePane( aLafDataMgr, aMgr );
    CleanupStack::PushL(self);
    self->ConstructL(aParent);
    CleanupStack::Pop();
    return self;
    }

CAknFepUICtrlCandidatePane::~CAknFepUICtrlCandidatePane()
    {
    delete iCtrlScroll;
    delete iPreview;
    if(iCandidateArray)
    	{
    	iCandidateArray->Reset();
		delete iCandidateArray;
    	}
    iCandidateLabelArray.ResetAndDestroy();
    iCandidateModeArray.Reset();
    iPages.Reset();
    }

const TDesC& CAknFepUICtrlCandidatePane::TextBuffer() const
    {
    return iBuffer;
    }

void CAknFepUICtrlCandidatePane::SetCandidateBuffer(const TDesC& aBuffer)
    {
    TInt newLength = aBuffer.Length();
    TInt oldLength = iBuffer.Length();
    if(newLength > EMaxCandidates)
        {
        newLength = EMaxCandidates;
        }
    if(newLength - 1 < iSelected)
        {
        // as far as the client is concerned, they have removed the character that was selected
        // therefore we must now hide the selected candidate, although the highlighting
        // state remains unchanged
		iCandidateLabelArray[iSelected]->SetHighlight( EFalse );
        }
    else
        {
        if(iHighlighted)
            {
            // make sure that the candidate is visible, as the selection is now
            // within a visible range (it might have been hidden last time
            // the buffer was set)
			iCandidateLabelArray[iSelected]->SetHighlight( ETrue );
            }
        }

    TPtrC newText = aBuffer.Left(newLength);
    TPtr ptr = iBuffer.Des();
    if(ptr != newText)
        {
        for(TInt i = 0; i < newLength; i++)
            {
            TBool valueChanged = ETrue;
            TPtrC newChar = aBuffer.Mid(i, EOneCandidate);
            if(i < oldLength)
                {
                TPtrC oldChar = iBuffer.Mid(i, EOneCandidate);
                if(newChar == oldChar)
                    {
                    valueChanged = EFalse;
                    }
                }
            // only update and redraw label background if the value has changed
            if(valueChanged)
                {
                // we have already allocated enough memory for this
				CAknFepUICtrlCandidateCharacter* candidateLabel = iCandidateLabelArray[i];
                candidateLabel->SetText( newChar );
                }
            }
        ptr.Copy(newText);
        LayoutLabels();
        iVisibleCount = newLength;
        UpdateLabelVisibility();
        }
    }

CDesCArrayFlat* CAknFepUICtrlCandidatePane::CandidateArray(void)
    {
    return iCandidateArray;
    }

void CAknFepUICtrlCandidatePane::SetCandidateBuffer(void)
    {
    if(iPages.Count() == 0)
        {
        return;
        }
    TPage curPage = iPages[iCurDisplayPage];
    iVisibleCount = curPage.iEndIndex - curPage.iStartIndex + 1;

    if(iVisibleCount - 1 < iSelected)
        {
		iCandidateLabelArray[iSelected]->SetHighlight( EFalse );
        }
    else
        {
        if(iHighlighted)
            {
			iCandidateLabelArray[iSelected]->SetHighlight( ETrue );
            }
        }

    TBufC<EPhraseLength> buf;
    TPtr newText = buf.Des();

    for(TInt i = 0; i < iVisibleCount; i ++)
        {
		CAknFepUICtrlCandidateCharacter* candidateLabel = iCandidateLabelArray[i];
        const TDesC* txtLabel = candidateLabel->Text();
        newText = (*iCandidateArray)[curPage.iStartIndex + i];

        if (( txtLabel->Compare(newText) != 0 ))
            {
            // we have already allocated enough memory for this
            TRAP_IGNORE( candidateLabel->SetText( newText ));
            }

        if ( i < iVisibleCount )
            {
            // only redraw labels that are visible
            TRect rect = candidateLabel->Rect();
            Window().Invalidate( rect );
            }
        }
    LayoutLabels();
    DrawDeferred();
    UpdateLabelVisibility();
    }

void CAknFepUICtrlCandidatePane::SetHighlighted(TBool aHighlighted)
    {
    if(!COMPARE_BOOLS(iHighlighted, aHighlighted))
        {
        iHighlighted = aHighlighted;
        iCandidateLabelArray[iSelected]->SetHighlight( aHighlighted );
        }
    }

TBool CAknFepUICtrlCandidatePane::Highlighted() const
    {
    return iHighlighted;
    }

void CAknFepUICtrlCandidatePane::SetCandidateMode(TInt aIndex, TCandidateMode aMode)
    {
    __ASSERT_DEBUG(aIndex < EMaxCandidates, AknFepUiInterfacePanic(EAknFepUiInterfacePanicAttemptedSetModeOutsideMaxCandidates));
    __ASSERT_DEBUG(aIndex >= 0, AknFepUiInterfacePanic(EAknFepUiInterfacePanicAttemptedSetModeOutsideMaxCandidates));

    TCandidateMode oldMode = iCandidateModeArray[aIndex];
    if(oldMode != aMode)
        {
        iCandidateModeArray[aIndex] = aMode;
        
        // only do drawing if visible
        if(aIndex < iVisibleCount)
            {
            LayoutLabel(aIndex);
            if(iSelected == aIndex && iHighlighted)
                {
                }
            else
                {
				CAknFepUICtrlCandidateCharacter* candidateLabel = iCandidateLabelArray[aIndex];
                TRect rect = candidateLabel->Rect();
                Window().Invalidate(rect);
                }
            }
        }
    }

CAknFepUICtrlCandidatePane::TCandidateMode CAknFepUICtrlCandidatePane::CandidateMode(TInt aIndex)
    {
    return iCandidateModeArray[aIndex];
    }

TBool CAknFepUICtrlCandidatePane::IsShowHorizontalScrollArrows() const
    {
    return EFalse;
    }

void CAknFepUICtrlCandidatePane::ShowHorizontalScrollArrows(TBool /*aValue*/)
    {
    // Left & right arrow are not need, so do nothing.
    }

void CAknFepUICtrlCandidatePane::ShowUpScrollArrows(TBool aValue)
    {
	// call function of scroll pane to show/hide up arrow.
	iCtrlScroll->ShowUpScrollArrows( aValue );
    }
    
void CAknFepUICtrlCandidatePane::ShowDownScrollArrows(TBool aValue)
    {
	// call function of scroll pane to show/hide down arrow.
	iCtrlScroll->ShowDownScrollArrows( aValue );
    }
    
void CAknFepUICtrlCandidatePane::ShowLeftScrollArrows(TBool /*aValue*/)
    {
	// Left arrow is not need, so do nothing.
    }
        
void CAknFepUICtrlCandidatePane::ShowRightScrollArrows(TBool /*aValue*/)
    {
	// Right arrow is not need, so do nothing.
    }

TBool CAknFepUICtrlCandidatePane::IsShowVerticalScrollArrows() const
    {
	// call function of scroll pane to get scroll state
    return iCtrlScroll->IsShowVerticalScrollArrows();
    }

void CAknFepUICtrlCandidatePane::ShowVerticalScrollArrows(TBool aValue)
    {
	// call function of scroll pane to set scroll state
	iCtrlScroll->ShowVerticalScrollArrows( aValue );
    }

TInt CAknFepUICtrlCandidatePane::VisibleCandidateCount() const
    {
    return iVisibleCount;
    }

void CAknFepUICtrlCandidatePane::SetVisibleCandidateCount(TInt aCount)
    {
    __ASSERT_DEBUG(aCount > 0, AknFepUiInterfacePanic(EAknFepUiInterfacePanicNewVisibleCountExceedsRange));
    __ASSERT_DEBUG(aCount <= EMaxCandidates, AknFepUiInterfacePanic(EAknFepUiInterfacePanicNewVisibleCountExceedsRange));
    if(iVisibleCount != aCount)
        {
        if(aCount - 1 < iSelected)
            {
            // automatically move the selection to be at the end of the new
            // candidate buffer
            SelectIndex(aCount - 1); // ignore return code
            }
        iVisibleCount = aCount;
        UpdateLabelVisibility();
        }
    }

TBool CAknFepUICtrlCandidatePane::SelectFirstPhrase()
    {
    if(iVisibleCount >= 1)
        {
        return SelectIndex(0,EFalse);
        }
    else
        {
        return EFalse;
        }
    }

TBool CAknFepUICtrlCandidatePane::SelectFirst()
    {
    if(iBuffer.Length())
        {
        return SelectIndex(0);
        }
    else
        {
        return EFalse;
        }
    }

TBool CAknFepUICtrlCandidatePane::SelectLastPhrase()
    {
    if(iVisibleCount > 1)
        {
        return SelectIndex(iVisibleCount - 1);
        }
    else
        {
        return EFalse;
        }
    }

TBool CAknFepUICtrlCandidatePane::SelectLast()
    {
    TInt length = iBuffer.Length();
    if(length)
        {
        TInt index = length < iVisibleCount ? length : iVisibleCount;
        return SelectIndex(index - 1);
        }
    else
        {
        return EFalse;
        }
    }

TBool CAknFepUICtrlCandidatePane::SelectNext()
    {
    if(iSelected < iVisibleCount - 1)
        {
        return SelectIndex(iSelected + 1);
        }
    else if( iSelected == iVisibleCount - 1 )
    	{
		return SelectIndex(0);
    	}
    else
        {
        return EFalse;
        }
    }

TBool CAknFepUICtrlCandidatePane::SelectPrev()
    {
    if(iSelected > 0)
        {
        return SelectIndex(iSelected - 1);
        }
    else if( iSelected == 0 )
    	{
        return SelectIndex( iVisibleCount - 1 );
        }
    else
        {
        return EFalse;
        }
    }

TBool CAknFepUICtrlCandidatePane::SelectIndex(TInt aIndex, TBool /*selected*/ )
    {
    if( aIndex + 1 > iVisibleCount || aIndex < 0 )
        {
        return EFalse;
        }

    if ( iSelected != aIndex )
    	{
		iCandidateLabelArray[iSelected]->SetHighlight( EFalse );
    	}
    
    if ( iPointDown || iHighlighted )
    	{
		iCandidateLabelArray[aIndex]->SetHighlight( ETrue );
		
    	}
        iSelected = aIndex;

    return ETrue;
    }

TInt CAknFepUICtrlCandidatePane::SelectedIndex() const
    {
    return iSelected;
    }

TPtrC CAknFepUICtrlCandidatePane::CurrentPhraseCandidate() const
    {
    __ASSERT_DEBUG(iSelected >= 0, AknFepUiInterfacePanic(EAknFepUiInterfacePanicCurrentSelectionOutsideVisibleCandidates));
    if(iVisibleCount > iSelected)
        {
        return *(iCandidateLabelArray[iSelected]->Text());
        }
    else
        {
        return TPtrC();
        }
    }

TPtrC CAknFepUICtrlCandidatePane::CurrentCandidate() const
    {
    __ASSERT_DEBUG(iSelected >= 0, AknFepUiInterfacePanic(EAknFepUiInterfacePanicCurrentSelectionOutsideVisibleCandidates));
    if(iBuffer.Length() > iSelected)
        {
        return iBuffer.Mid(iSelected, EOneCandidate);
        }
    else
        {
        return TPtrC();
        }
    }

TInt CAknFepUICtrlCandidatePane::LabelTextWidthInPixels( TInt aIndex)
    {
    TAknLayoutText layoutText;
    TAknTextLineLayout candidatePaneLayoutText = 
        CAknFepChineseUIDataConv::AnyToTextLine(iLafDataMgr->RequestData( ECharacterTextLayout ));
       
    layoutText.LayoutText(Rect(), candidatePaneLayoutText);

    const CFont* font = layoutText.Font();

    TInt labelWidthInPixels = 0;
    if(font)
        {
        const TDesC* txtLabel = iCandidateLabelArray[aIndex]->Text();
        if(txtLabel->Length() != 0)
            {
            labelWidthInPixels = font->TextWidthInPixels(*txtLabel);
            }
        }
    return labelWidthInPixels;
    }
void CAknFepUICtrlCandidatePane::SetPaneLayout(CAknFepUICtrlContainerChinese::TPaneLayout aPaneLayout)
    {
    iPaneLayout = aPaneLayout;
    }
    
void CAknFepUICtrlCandidatePane::SplitPhraseCandidatesIntoPages()
    {
    iPages.Reset();
    iUnpagedIndex = 0;
    
    InternalSpilitPhraseCandidate();
    
    if( iPages.Count() == 0 )
        {
        iVisibleCount = 0;
        }
    else
        {
        iCurDisplayPage = 0;
        SetVisibleCandidateCount(iPages[0].iEndIndex - iPages[0].iStartIndex + 1);
        }
    }
    
void CAknFepUICtrlCandidatePane::NextCandidatePage(void)
    {
    if(iCurDisplayPage == (iPages.Count() - 1))
        {
        if( iUnpagedIndex < iCandidateArray->Count() )
            {
            //continue page
            InternalSpilitPhraseCandidate();
            iCurDisplayPage++;
            }
        else
            {
            iCurDisplayPage = 0;
            }
        }
    else
        {
        iCurDisplayPage ++;
        }
    // For fix bug of when the count of character in the last page is less than
    // the previous page of the last page. And then select the last character of 
    // the previous page of the last page, and select next page. First of the last 
    // page is not refreshed.
    iCandidateLabelArray[iSelected]->SetHighlight( EFalse );
    iSelected = 0;
    }

void CAknFepUICtrlCandidatePane::PreviousCandidatePage(void)
    {
    if(iCurDisplayPage == 0)
        {
        iCurDisplayPage = iPages.Count() - 1;
        }
    else
        {
        iCurDisplayPage --;
        }
    }

void CAknFepUICtrlCandidatePane::SizeChanged()
    {
    LayoutContainedControls();
    }

TInt CAknFepUICtrlCandidatePane::CountComponentControls() const
    {
    TInt count = 0;
    count += iCandidateLabelArray.Count();

    CCoeControl* controls[] =
        {
        iCtrlScroll,
        iPreview
        } ;

    for (TUint ii = 0 ; ii < (sizeof(controls) / sizeof(CCoeControl*)) ; ii++)
        {
        if(controls[ii])
            {
            count++ ;
            }   
        }
    return count ;
    }

CCoeControl* CAknFepUICtrlCandidatePane::ComponentControl(TInt aIndex) const
    {
    TInt candidateCount = iCandidateLabelArray.Count();

    if(aIndex < candidateCount)
        {
        // because this method is const, the const [] operator is selected
        // which means that the pointer is const, so have to cast away constness
        // - a possible alternative is to make the array mutable?
        return CONST_CAST(CAknFepUICtrlCandidateCharacter*, iCandidateLabelArray[aIndex]);
        }
    aIndex -= candidateCount;

    CCoeControl* controls[] =
        {
       // iCandidateSelected,
        iCtrlScroll,
        iPreview
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

void CAknFepUICtrlCandidatePane::ConstructL(RWindow& aParent)
    {
    SetContainerWindowL(aParent);

    TBuf<EOneCandidate> ordinal;
    for(TInt i = 0; i < EMaxCandidates; i++)
        {
        CAknFepUICtrlCandidateCharacter* candidateLabel = CAknFepUICtrlCandidateCharacter::NewL( aParent, this );
        CleanupStack::PushL(candidateLabel);
        candidateLabel->SetContainerWindowL(aParent);
        // ownership is passed, unless allocation is unsuccessful, in which case we still have it on the stack
        User::LeaveIfError(iCandidateLabelArray.Append(candidateLabel));
        CleanupStack::Pop(); // candidateLabel

        User::LeaveIfError(iCandidateModeArray.Append(ECandidateModeNormal));
        }

    iCandidateArray = new(ELeave) CDesCArrayFlat(1);
    
    // scroll part
    iCtrlScroll = CAknFepUICtrlCandidateScroll::NewL( aParent, this );
    iCtrlScroll->SetMopParent(this);
    
    // character preview
    iPreview = CAknFepPreview::NewL( aParent );
    }

CAknFepUICtrlCandidatePane::CAknFepUICtrlCandidatePane(CAknFepUiLayoutDataMgr* aLafDataMgr, MAknFepChineseUiMgr* aMgr )
    :
    iPaneLayout(CAknFepUICtrlContainerChinese::ELayoutCandidate),
    iHighlighted(EFalse),
    iCurDisplayPage(0),
    iChineseUiMgr(aMgr),
    iPointDown( EFalse )
    {
    iLafDataMgr = aLafDataMgr;
    }

void CAknFepUICtrlCandidatePane::LayoutContainedControls()
    {
    LayoutLabels();
    LayoutScroll();
    }

void CAknFepUICtrlCandidatePane::LayoutLabel(TInt aIndex)
    {
    __ASSERT_DEBUG(aIndex >= 0, AknFepUiInterfacePanic(EAknFepUiInterfacePanicAttemptedLayoutOutsideVisibleCount));
    __ASSERT_DEBUG(aIndex < iVisibleCount, AknFepUiInterfacePanic(EAknFepUiInterfacePanicAttemptedLayoutOutsideVisibleCount));
    TRect rect = Rect();
    CAknFepUICtrlCandidateCharacter* candidateLabel = iCandidateLabelArray[aIndex];
    TAknWindowLineLayout layoutCandidateItem = 
        CAknFepChineseUIDataConv::AnyToWindowLine( iLafDataMgr->RequestData(ECharacterLayout));
    TAknTextLineLayout candidateLayout = 
        CAknFepChineseUIDataConv::AnyToTextLine( iLafDataMgr->RequestData(ECharacterTextLayout));
    
    // layout candidate label
    TAknLayoutRect layoutRectCandidateItem;
    layoutRectCandidateItem.LayoutRect( rect,layoutCandidateItem );
    TRect rectCandidateItem = layoutRectCandidateItem.Rect();
    if ( aIndex > 0 )
	    {
	    rectCandidateItem.iTl.iX = iCandidateLabelArray[aIndex - 1]->Rect().iBr.iX;
        }

    TAknLayoutText layoutText;
    layoutText.LayoutText( rectCandidateItem, candidateLayout );
    TInt interval = candidateLayout.il + candidateLayout.ir;

    TInt width = LabelTextWidthInPixels( aIndex );
    TInt minwidth = layoutRectCandidateItem.Rect().Width() - interval ;    
    if ( width < minwidth )
    	{
		width = minwidth;
    	}    
    rectCandidateItem.SetWidth( width + interval );
    candidateLabel->SetRect( rectCandidateItem );

    // override colours if skin data present
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    TRgb candidateLabelTextColor = layoutText.Color();

    AknsUtils::GetCachedColor(skin, candidateLabelTextColor, 
                              KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG19);

    TRAP_IGNORE(
        candidateLabel->OverrideColorL(EColorLabelText, candidateLabelTextColor);
        );
    }

void CAknFepUICtrlCandidatePane::LayoutLabels()
    {
    for(TInt i = 0; i < iVisibleCount; i++)
        {
        LayoutLabel(i);
        }
    }
void CAknFepUICtrlCandidatePane::LayoutScroll()
    {
    TAknWindowLineLayout scrolllayout =  CAknFepChineseUIDataConv::AnyToWindowLine( iLafDataMgr->RequestData(EScrollLayout));
    AknLayoutUtils::LayoutControl( iCtrlScroll, Rect(), scrolllayout );
    }

void CAknFepUICtrlCandidatePane::UpdateLabelVisibility()
    {
    TInt number = 0;
    // only show the labels corresponding to candidates that have been set
    if(iPages.Count() != 0)
        {
        number = iPages[iCurDisplayPage].iEndIndex - iPages[iCurDisplayPage].iStartIndex + 1;
        }
    else
        {
        number = iBuffer.Length();
        }

    if(number > iVisibleCount)
        {
        number =  iVisibleCount;
        }
    for(TInt i = 0; i < EMaxCandidates; i++)
        {
        CAknFepUICtrlCandidateCharacter* candidateLabel = iCandidateLabelArray[i];
        if(i < number)
            {
            candidateLabel->MakeVisible(ETrue);
            }
        else
            {
            candidateLabel->MakeVisible(EFalse);
            }
        }
    }

void CAknFepUICtrlCandidatePane::ResetCandidateState()
    {
    iPages.Reset();
    }

void CAknFepUICtrlCandidatePane::DrawBackground(const TRect& aRect) const
    {
    // instead of drawing one blank rectangle over each of the 20 labels,
    // just draw one big rectangle covering the entire pane
    CWindowGc& gc = SystemGc();
    TRgb background = AKN_LAF_COLOR(0);
    gc.SetBrushColor(background);
    gc.Clear(aRect);
    }

void CAknFepUICtrlCandidatePane::InternalSpilitPhraseCandidate()
    {
    //__ASSERT_DEBUG(aStartIndex + aCount <= iCandidateArray->Count());
    
    TInt phraseCandidateNum = iCandidateArray->Count();
    if( phraseCandidateNum == iUnpagedIndex )
        {
        //nothing need to be paged
        return;
        }
        
    TPage curPage;
    TInt index = iUnpagedIndex;
    CAknFepUICtrlCandidateCharacter* candidateLabel;
    
    curPage.iStartIndex = index;
    curPage.iEndIndex = index;
    TInt candidateLeftNum = phraseCandidateNum - index > EMaxCandidates? 
                            EMaxCandidates : phraseCandidateNum - index;

    TAknWindowLineLayout layoutCandidateItem = 
        CAknFepChineseUIDataConv::AnyToWindowLine( iLafDataMgr->RequestData(ECharacterLayout));
    TAknWindowLineLayout candidateRect = 
        CAknFepChineseUIDataConv::AnyToWindowLine( iLafDataMgr->RequestData(EScrollLayout));
    
    TAknTextLineLayout candidateLayout = 
        CAknFepChineseUIDataConv::AnyToTextLine( iLafDataMgr->RequestData(ECharacterTextLayout));
    
    TInt interval = candidateLayout.il + candidateLayout.ir;
    // avoid accessing layout data member
    TAknLayoutRect layoutRect;
    layoutRect.LayoutRect(Rect(),layoutCandidateItem);
    TRect rectLayoutCandidateItem = layoutRect.Rect();
    TAknLayoutText layoutText;
    layoutText.LayoutText(rectLayoutCandidateItem,candidateLayout);
    TRect rectText = Rect();  
    TInt distance = rectText.iTl.iX - rectLayoutCandidateItem.iTl.iX + rectText.iBr.iX - rectLayoutCandidateItem.iBr.iX;
    rectLayoutCandidateItem.iBr.iX += distance - candidateRect.iW;

    TInt i = 0;     

    const TInt candidateWidth = rectLayoutCandidateItem.Width();
    for ( i = 0; i < candidateLeftNum; i ++ )
        {
        // Initialization
        candidateLabel = iCandidateLabelArray[i];
        // labelWidth in the font
        const CFont* font = layoutText.Font();
        TPtrC newText = (*iCandidateArray)[iUnpagedIndex + i];
        TInt labelWidth = font->TextWidthInPixels( newText );
        labelWidth = interval + ( labelWidth > layoutText.TextRect().Width() ? labelWidth : layoutText.TextRect().Width());
        if ( labelWidth > candidateWidth )
            {
            TBufC<EPhraseLength+10> buf1;
            TPtr text( buf1.Des() );
            
            AknBidiTextUtils::ConvertToVisualAndClip(newText, text, *font, 
                candidateWidth, candidateWidth,
                AknBidiTextUtils::EImplicit,0xFFFF);
            
            iCandidateArray->Delete( iUnpagedIndex + i );
            TRAP_IGNORE(
            {
            iCandidateArray->InsertL( iUnpagedIndex + i, text );
            candidateLabel->SetText(text);
            });
            	
            }
        else
            {
            TRAP_IGNORE( candidateLabel->SetText(newText));
            rectLayoutCandidateItem.iTl.iX += labelWidth;
            }
        if (rectLayoutCandidateItem.iTl.iX > rectLayoutCandidateItem.iBr.iX)
            {
            if (index>0)
                {
                curPage.iEndIndex = index - 1;
                }
            else
                {
                curPage.iEndIndex = index;
                }

            iPages.Append(curPage);
            break;
            }
        else
            {
            index ++;
            }
        
        if( i == EMaxCandidates -1 )
        	{
            curPage.iEndIndex = index - 1;
            iPages.Append(curPage);
            break;
        	}
        
        }
        
    if( i >= candidateLeftNum )
        {
        curPage.iEndIndex = index - 1;
        iPages.Append(curPage);
        }
        
    iUnpagedIndex = index;      
    }

TBool CAknFepUICtrlCandidatePane::IsFirstPage()
    {
    return iCurDisplayPage == 0;
    }

TBool CAknFepUICtrlCandidatePane::IsLastPage()
    {
	if( iPages.Count() == 0 )
		{
		return ETrue;
		}
	else if( iCandidateArray->Count() == iUnpagedIndex )
        {
        return iCurDisplayPage == iPages.Count() - 1;
        }
    return EFalse;
    }

void CAknFepUICtrlCandidatePane::SetInVisiable()
    {
	iCandidateLabelArray[iSelected]->SetHighlight( EFalse );
	TRAP_IGNORE( iPreview->HideL());
    for (TInt i = 0; i < EMaxCandidates; i ++)
        {
		CAknFepUICtrlCandidateCharacter* candidateLabel = iCandidateLabelArray[i];
        candidateLabel->MakeVisible(EFalse);
        }
    }

TBool CAknFepUICtrlCandidatePane::NeedGetNewCandidatePage()
    {
    return (iCandidateArray->Count() - iUnpagedIndex < CandidateCountLeft);

    }

void CAknFepUICtrlCandidatePane::Enable(TBool aEnable)
	{
	if (!aEnable)
		{
		for (TInt i=0; i<iVisibleCount; i++)
			{
			iCandidateLabelArray[i]->SetText( KNullDesC );
			}
		TRAP_IGNORE( HidePreviewL();)
		iCandidateArray->Reset();
		iVisibleCount = 0;
		DrawNow();
		}
	}

void CAknFepUICtrlCandidatePane::FepUIClosePressed()
	{
	// Click close to close container
	if ( iChineseUiMgr )
		{
		iChineseUiMgr->CloseContainer();
		}
	}

void CAknFepUICtrlCandidatePane::SetFepUiState( MAknFepUiStateCtrl* aFepUiState )
    {
	iFepUiState = aFepUiState;
    }

void CAknFepUICtrlCandidatePane::HidePreviewL()
	{
	if( iPreview && iPreview->IsVisible())
		{
		iPreview->HideL();
		}
	}
void CAknFepUICtrlCandidatePane::HandlePointerEventL( const TPointerEvent & aPointerEvent )
	{
	CCoeControl::HandlePointerEventL( aPointerEvent );
    // Press will pop up preview
	if ( aPointerEvent.iType == TPointerEvent::EButton1Down )
		{
		for ( TInt i = 0; i < iVisibleCount; i++ )
            {
	        if ( iCandidateLabelArray[i]->Rect().Contains( aPointerEvent.iPosition ))
		        {
				iPointDown = ETrue;
                SelectIndex( i );
	            const TDesC* txtLabel = iCandidateLabelArray[i]->Text();
	            TPoint point = iCandidateLabelArray[i]->PositionRelativeToScreen();
	            TRect rect( point, iCandidateLabelArray[i]->Size());
	            // if container is show on top, the preview should show bellow the character
	            // else the preview should show above the character.
	            TInt bottom = ShowAboveCharacter;
	            if ( point.iY < iCandidateLabelArray[i]->Rect().iBr.iY )
	                {
				    bottom = ShowBellowCharacter;
	                }
	            rect.Move( 0, (iCandidateLabelArray[i]->Size().iHeight ) * bottom );
	            iPreview->ShowL( rect, *txtLabel );
	            break;
	    	    }	        
            }
        }
	// Up will submit text
	else if ( aPointerEvent.iType == TPointerEvent::EButton1Up && iPointDown )
		{
		iPointDown = EFalse;
	    for ( TInt i = 0; i < iVisibleCount; i++ )
	        {
		    if ( iCandidateLabelArray[i]->Rect().Contains( aPointerEvent.iPosition ))
			    {
			    if ( iChineseUiMgr )
			        {
			        const TDesC* text = iCandidateLabelArray[i]->Text();	    
			        iChineseUiMgr->SubmitSelectedTextL( *text );			        
			        }
			    break;
			    }
	        }
	    if( iPreview->IsVisible())
	    	{
			iPreview->HideL();
	    	}
	    
	    if( !iHighlighted )
	    	{
			iCandidateLabelArray[iSelected]->SetHighlight( iHighlighted );
	    	}
		}
	// Drag out of candidate pane, hide preview
	else if ( aPointerEvent.iType == TPointerEvent::EDrag && iPointDown )
		{
	    TBool bContain = EFalse;
		for ( TInt i = 0; ( !bContain ) && ( i < iVisibleCount ); i++ )
		    {
			if ( iCandidateLabelArray[i]->Rect().Contains( aPointerEvent.iPosition ))
				{
				TBool bSameIndex = ( i == iSelected );
				SelectIndex( i );
				if ( !bSameIndex )
					{
					const TDesC* txtLabel = iCandidateLabelArray[i]->Text();
					TPoint point = iCandidateLabelArray[i]->PositionRelativeToScreen();
					TRect rect( point, iCandidateLabelArray[i]->Size());
					TInt bottom = ShowAboveCharacter;
					// If container is moved to top,
					// then pop up preview below the character
					if ( point.iY < iCandidateLabelArray[i]->Rect().iBr.iY )
						{
						bottom = ShowBellowCharacter;
						}
					rect.Move( 0, (iCandidateLabelArray[i]->Size().iHeight ) * bottom );
					iPreview->ShowL( rect, *txtLabel );
					}
				bContain = ETrue;
				break;
			    }
		    }
		if ( !bContain )
			{
			iPreview->HideL();
			iCandidateLabelArray[iSelected]->SetHighlight( iHighlighted );	
			if( !iHighlighted )
				{
				iSelected = 0;
				}
			}
		else if( bContain && !iPreview->IsVisible())
			{
			const TDesC* txtLabel = iCandidateLabelArray[iSelected]->Text();
			TPoint point = iCandidateLabelArray[iSelected]->PositionRelativeToScreen();
			TRect rect( point, iCandidateLabelArray[iSelected]->Size());
			TInt bottom = ShowAboveCharacter;
			// If container is moved to top,
			// then pop up preview below the character
			if ( point.iY < iCandidateLabelArray[iSelected]->Rect().iBr.iY )
				{
				bottom = ShowBellowCharacter;
				}
			rect.Move( 0, (iCandidateLabelArray[iSelected]->Size().iHeight ) * bottom );
			iPreview->ShowL( rect, *txtLabel );
			}
		}
	else
		{
		// do nothing
		}
	}

CAknFepPreview* CAknFepPreview::NewL( RWindowTreeNode& aParent )
	{
	CAknFepPreview* self = new(ELeave) CAknFepPreview;
    CleanupStack::PushL(self);
    self->ConstructL( aParent );
    CleanupStack::Pop();
    return self;
	}

CAknFepPreview::~CAknFepPreview()
	{
	delete iPreviewLabel;
	}

void CAknFepPreview::Draw( const TRect& /*aRect*/ ) const
	{
    CWindowGc& gc = SystemGc();
	
	MAknsSkinInstance* skin = AknsUtils::SkinInstance();
	TRect outerRect;
	TRect innerRect;
	CalculateFrameRects( outerRect, innerRect );
	gc.SetBrushColor( KRgbGray );
	// draw the whole frame background according to the containing context, which in this case
	// is the candidate pane, to avoid corner drawing mess and to cope with masked grid frame centre
	MAknsControlContext* cc = AknsDrawUtils::ControlContext(this);
	AknsDrawUtils::DrawFrame( skin, gc, outerRect, innerRect,
							  KAknsIIDQsnFrInputCharPreview, KAknsIIDDefault ); 	
	}

void CAknFepPreview::SizeChanged()
	{
    TRect rect = Rect();
    TAknTextLineLayout candidateLayout = AknLayoutScalable_Avkon::cell_hyb_candi_pane_t1(0);
    // Get preview rect and font. 
    TAknLayoutText layoutText;
    layoutText.LayoutText( rect, candidateLayout );
    TRect rectText = layoutText.TextRect();
    const CFont* font = layoutText.Font();

    iPreviewLabel->SetRect( rectText );
    iPreviewLabel->SetFont( font );
    // override with skin text color
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    
    TRgb labelTextColor;
    AknsUtils::GetCachedColor( skin, labelTextColor, 
                               KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG11 );

    TRAP_IGNORE( 
    		iPreviewLabel->OverrideColorL(EColorLabelText, labelTextColor);	 )
	}

void CAknFepPreview::ShowL( TRect& aRect, const TDesC& aCharacter )
	{
	SetRect( aRect );

	iPreviewLabel->SetTextL( aCharacter );
	MakeVisible( ETrue );
	RWindow& window = Window();
	window.SetOrdinalPosition( 0, ECoeWinPriorityAlwaysAtFront + 100 );
	DrawNow();
	}

void CAknFepPreview::HideL()
	{
	MakeVisible( EFalse );
	iPreviewLabel->SetTextL( KNullDesC );
	DrawNow();
	}

TInt CAknFepPreview::CountComponentControls() const
	{
    CCoeControl* controls[] = 
        {
        iPreviewLabel
        };
    TInt count = 0;
    for ( TUint ii = 0 ; ii < (sizeof(controls) / sizeof(CCoeControl*)); ii++ )
        {
        if ( controls[ii] )
            {
            count++;
            }   
        }

    return count ;
	}

CCoeControl* CAknFepPreview::ComponentControl(TInt aIndex) const
	{
    CCoeControl* controls[] = 
        {
        iPreviewLabel
        };

    for ( TUint ii = 0; (ii < sizeof(controls) / sizeof(CCoeControl*)); ii++ )
        {
        if ( controls[ii] && aIndex-- == 0 )
            {
            return controls[ii];
            }
        }
    // shouldn't be called while no components.
    return NULL ;
	}

CAknFepPreview::CAknFepPreview()
	{
	}

void CAknFepPreview::ConstructL( RWindowTreeNode& /*aParent */)
	{
	RWindowGroup& wg = CCoeEnv::Static()->RootWin();
	CreateWindowL( wg );
	RWindow& window = Window();
	iPreviewLabel = new( ELeave ) CEikLabel;
	iPreviewLabel->SetContainerWindowL( window );
	iPreviewLabel->SetLabelAlignment( ELayoutAlignCenter );	
	iPreviewLabel->SetMopParent( this );
	window.SetRequiredDisplayMode( EColor16MA );
	window.SetTransparencyAlphaChannel();
	window.SetOrdinalPosition( 0, ECoeWinPriorityAlwaysAtFront + 100 );
	window.SetFaded( EFalse, RWindowTreeNode::EFadeIncludeChildren ); 
	// Enable receive drag event.
	Window().PointerFilter( EPointerFilterDrag, 0 );
	MakeVisible( EFalse );
	ActivateL();
	}

void CAknFepPreview::CalculateFrameRects(TRect& aOuterRect, TRect& aInnerRect) const
	{
	TRect windowRect = Rect();

	TAknLayoutRect topLeft;
	topLeft.LayoutRect( windowRect, SkinLayout::Highlight_skin_placing__grid__Line_2());

	TAknLayoutRect bottomRight;
	bottomRight.LayoutRect( windowRect, SkinLayout::Highlight_skin_placing__grid__Line_5());

	aOuterRect = TRect( topLeft.Rect().iTl, bottomRight.Rect().iBr );
	aInnerRect = TRect( topLeft.Rect().iBr, bottomRight.Rect().iTl );
	}

CAknFepUICtrlCandidateScroll* CAknFepUICtrlCandidateScroll::NewL(
							RWindowTreeNode& aParent, MAknFepUIEventObserver* aObserver )
	{
	CAknFepUICtrlCandidateScroll* self = new(ELeave) CAknFepUICtrlCandidateScroll;
    CleanupStack::PushL(self);
    self->ConstructL( aParent, aObserver );
    CleanupStack::Pop();
    return self;
	}

CAknFepUICtrlCandidateScroll::~CAknFepUICtrlCandidateScroll()
	{
	// delete bitmaps
	delete iScrollCloseBitmap;
	delete iScrollCloseBitmapMask; 
    delete iNaviArrowBitmapUp;
    delete iNaviArrowBitmapUpMask;
    delete iNaviArrowBitmapDown;
    delete iNaviArrowBitmapDownMask;
	}

void CAknFepUICtrlCandidateScroll::SizeChanged()
	{
	LayoutIcon();
	}

void CAknFepUICtrlCandidateScroll::Draw( const TRect& /*aRect*/ ) const
	{
    CWindowGc& gc = SystemGc();
	MAknsSkinInstance* skin = AknsUtils::SkinInstance();

	// draw the whole frame background according to the containing context, which in this case
	// is the candidate pane, to avoid corner drawing mess and to cope with masked grid frame centre
	MAknsControlContext* cc = AknsDrawUtils::ControlContext(this);
	TRect rect = Rect();
	
	// Draw scroll background
	AknsDrawUtils::DrawBackground( skin, cc, this, gc,
								   rect.iTl, rect, KAknsDrawParamDefault );

	// Draw close Rects
	if ( iShowClose )
		{
		TRect outerRect;
		TRect innerRect;
		CalculateCloseRects( outerRect, innerRect );
		AknsDrawUtils::DrawFrame( skin, gc, outerRect, innerRect,
								  KAknsIIDQsnFrKeypadButtonFrNormal, KAknsIIDDefault ); 
		// Draw close bitmap
		iIndiFepClose.DrawImage( gc, 
								 iScrollCloseBitmap, 
								 iScrollCloseBitmapMask );
		}
	
	// Draw Up arrow & Down arrow
	if ( iShowVerticalScrollArrows )
	    {
	    if ( iShowUpScrollArrows )
	        {
	        iIndiFepArrowUp.DrawImage( gc,
	                                   iNaviArrowBitmapUp, 
	                                   iNaviArrowBitmapUpMask );
	        }
	            
	    if ( iShowDownScrollArrows )
	        {
	        iIndiFepArrowDown.DrawImage( gc, 
	                                     iNaviArrowBitmapDown, 
	                                     iNaviArrowBitmapDownMask );
	        }
	    }
	}

void CAknFepUICtrlCandidateScroll::HandlePointerEventL( const TPointerEvent & aPointerEvent )
	{
	// Close area is click will close the container
	if ( iObServer != NULL && iIndiFepClose.Rect().Contains( aPointerEvent.iPosition ))
		{
		iObServer->FepUIClosePressed();
		}
	}

void CAknFepUICtrlCandidateScroll::ShowUpScrollArrows(TBool aValue)
    {
	// Update the up arrow if needed
    if ( iShowUpScrollArrows != aValue )
        {
        iShowUpScrollArrows = aValue;
        TRect rect = iIndiFepArrowUp.Rect();
        Window().Invalidate(rect);
        }
    }
    
void CAknFepUICtrlCandidateScroll::ShowDownScrollArrows( TBool aValue )
    {
	// Update the down arrow if needed
    if ( iShowDownScrollArrows != aValue )
        {
        iShowDownScrollArrows = aValue;
        TRect rect = iIndiFepArrowDown.Rect();
        Window().Invalidate(rect);
        }        
    }

void CAknFepUICtrlCandidateScroll::ShowVerticalScrollArrows( TBool aValue )
    {
    // If the state is changed, the arrows will be redrawn in the new state.
    if(!COMPARE_BOOLS(iShowVerticalScrollArrows, aValue))
        {
        iShowVerticalScrollArrows = aValue;
        //TRect rect = iIndiFepArrowUp.Rect();
        DrawDeferred();
        //rect = iIndiFepArrowDown.Rect();
        //Window().Invalidate(rect);
        }
    }

TBool CAknFepUICtrlCandidateScroll::IsShowVerticalScrollArrows() const
    {
    return iShowVerticalScrollArrows;
    }

void CAknFepUICtrlCandidateScroll::ConstructL( RWindowTreeNode& aParent, 
											   MAknFepUIEventObserver* aObserver )
	{
    CreateWindowL( aParent );
    ConstructBitmapsL();    
    iObServer = aObserver; 
    
    TRect rect;
    AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EScreen, rect );
    // Dibert will not need close button
    iShowClose = rect.Height() == DibertHeight ? EFalse : ETrue;
    
	}

void CAknFepUICtrlCandidateScroll::ConstructBitmapsL()
	{
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();  
    
    TFileName bmpFile;
    bmpFile.Copy(_L("z:"));
    bmpFile.Append( KDC_BITMAP_DIR );
    bmpFile.Append( KFepUiControlsBitmapFile );  

    // Create close bitmap
    AknsUtils::CreateColorIconL( skin,
 							     KAknsIIDQgnGrafFepCandiListClose,
 							     KAknsIIDQsnIconColors,
 							     EAknsCIQsnIconColorsCG20,
 							     iScrollCloseBitmap,
 							     iScrollCloseBitmapMask,
 							     bmpFile,
 							     EMbmAknfepuictrlsQgn_indi_input_candi_list_close,
 							     EMbmAknfepuictrlsQgn_indi_input_candi_list_close_mask,
 							     AKN_LAF_COLOR( 0 )); 
    
    // Create Up bitmap
    AknsUtils::CreateColorIconL( skin,
                                 KAknsIIDQgnIndiFepArrowUp,
                                 KAknsIIDQsnIconColors,
                                 EAknsCIQsnIconColorsCG20,
                                 iNaviArrowBitmapUp,
                                 iNaviArrowBitmapUpMask,
                                 bmpFile,
                                 EMbmAknfepuictrlsQgn_indi_fep_arrow_up,
                                 EMbmAknfepuictrlsQgn_indi_fep_arrow_up_mask,
                                 AKN_LAF_COLOR( 0 ));
    
    // Create Down bitmap
    AknsUtils::CreateColorIconL( skin,
                                 KAknsIIDQgnIndiFepArrowDown,
                                 KAknsIIDQsnIconColors,
                                 EAknsCIQsnIconColorsCG20,
                                 iNaviArrowBitmapDown,
                                 iNaviArrowBitmapDownMask,
                                 bmpFile,
                                 EMbmAknfepuictrlsQgn_indi_fep_arrow_down,
                                 EMbmAknfepuictrlsQgn_indi_fep_arrow_down_mask,
                                 AKN_LAF_COLOR( 0 )); 
	}

CAknFepUICtrlCandidateScroll::CAknFepUICtrlCandidateScroll()
: iShowVerticalScrollArrows(EFalse),
  iShowUpScrollArrows(ETrue),
  iShowDownScrollArrows(ETrue),
  iShowClose(ETrue)
	{	
	}

void CAknFepUICtrlCandidateScroll::LayoutIcon()
	{
    TRect rect = Rect();
    // layout arrow up
    TAknWindowLineLayout arrowUpLayout = AknLayoutScalable_Avkon::cell_hyb_candi_scroll_pane_g1(0);
    iIndiFepArrowUp.LayoutRect( rect, arrowUpLayout );    
    AknIconUtils::SetSize(iNaviArrowBitmapUp, iIndiFepArrowUp.Rect().Size());
    
    // layout arrow down
    TAknWindowLineLayout arrowDownLayout = AknLayoutScalable_Avkon::cell_hyb_candi_scroll_pane_g2(0);
    iIndiFepArrowDown.LayoutRect( rect, arrowDownLayout );    
    AknIconUtils::SetSize( iNaviArrowBitmapDown, iIndiFepArrowDown.Rect().Size());
    
    // layout close
    TAknWindowLineLayout closeLayout = AknLayoutScalable_Avkon::cell_hyb_candi_scroll_pane_g3(0);
    iIndiFepClose.LayoutRect( rect, closeLayout );
    AknIconUtils::SetSize( iScrollCloseBitmap, iIndiFepClose.Rect().Size());   
    }

void CAknFepUICtrlCandidateScroll::CalculateCloseRects(TRect& aOuterRect, TRect& aInnerRect) const
	{
	TRect windowRect = iIndiFepClose.Rect();

	TAknLayoutRect topLeft;
	topLeft.LayoutRect( windowRect, SkinLayout::Highlight_skin_placing__grid__Line_2());

	TAknLayoutRect bottomRight;
	bottomRight.LayoutRect( windowRect, SkinLayout::Highlight_skin_placing__grid__Line_5());

	aOuterRect = TRect( topLeft.Rect().iTl, bottomRight.Rect().iBr );
	aInnerRect = TRect( topLeft.Rect().iBr, bottomRight.Rect().iTl );
	}

// End of file
