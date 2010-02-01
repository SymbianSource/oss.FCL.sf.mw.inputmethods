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

#include <aknfepuictrls.mbg>
#include "AknFepUiInterfacePanic.h"
#include "AknFepUIAvkonCtrlCandidatePane.h"
#include "AknFepUIAvkonCtrlCandidateSelected.h"
#include "aknfepchineseuidataconv.h"
#include "aknfepuilayoutdatamgr.h"


#include "data_caging_path_literals.hrh"

const TInt CandidateCountLeft = 10;

_LIT(KFepUiControlsBitmapFile, "aknfepuictrls.mbm");

CAknFepUICtrlCandidatePane* CAknFepUICtrlCandidatePane::NewL(RWindow& aParent, CAknFepUiLayoutDataMgr* aLafDataMgr)
    {
    CAknFepUICtrlCandidatePane* self = new(ELeave) CAknFepUICtrlCandidatePane( aLafDataMgr );
    CleanupStack::PushL(self);
    self->ConstructL(aParent);
    CleanupStack::Pop();
    return self;
    }

CAknFepUICtrlCandidatePane::~CAknFepUICtrlCandidatePane()
    {
    delete iCandidateSelected;
    delete iNaviArrowBitmapLeft;
    delete iNaviArrowBitmapLeftMask;
    delete iNaviArrowBitmapRight;
    delete iNaviArrowBitmapRightMask;
    delete iNaviArrowBitmapUp;
    delete iNaviArrowBitmapUpMask;
    delete iNaviArrowBitmapDown;
    delete iNaviArrowBitmapDownMask;
    if(iCandidateArray)
    	{
    	iCandidateArray->Reset();
		delete iCandidateArray;
    	}
    iCandidateLabelArray.ResetAndDestroy();
    iOrdinalLabelArray.ResetAndDestroy();
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
        iCandidateSelected->MakeVisible(EFalse);
        }
    else
        {
        if(iHighlighted)
            {
            // make sure that the candidate is visible, as the selection is now
            // within a visible range (it might have been hidden last time
            // the buffer was set)
            iCandidateSelected->MakeVisible(ETrue);
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
                CEikLabel* candidateLabel = iCandidateLabelArray[i];
                TRAP_IGNORE(candidateLabel->SetTextL(newChar));

                // need to update the character if it is in the current selected candidate
                if(i == iSelected)
                    {
                    iCandidateSelected->SetText(newChar);
                    iCandidateSelected->SetOrdinal(i + 1);
                    DrawDeferred();
                    }
                if(i < iVisibleCount)
                    {
                    // only redraw characters that are visible
                    TRect rect = candidateLabel->Rect();
                    Window().Invalidate(rect);
                    }
                }
            }
        ptr.Copy(newText);
        LayoutLabels();
        LayoutCandidateSelected();
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
        iCandidateSelected->MakeVisible(EFalse);
        }
    else
        {
        if(iHighlighted)
            {
            iCandidateSelected->MakeVisible(ETrue);
            }
        }

    TBufC<EPhraseLength> buf;
    TPtr newText = buf.Des();

    for(TInt i = 0; i < iVisibleCount; i ++)
        {
        CEikLabel* candidateLabel = iCandidateLabelArray[i];
        const TDesC* txtLabel = candidateLabel->Text();
        newText = (*iCandidateArray)[curPage.iStartIndex + i];

        if((txtLabel->Compare(newText) != 0) )
            {
            // we have already allocated enough memory for this
            TRAP_IGNORE( candidateLabel->SetTextL(newText));
            }
            
            // need to update the character if it is in the current selected candidate
            if(i == iSelected)
                {
                iCandidateSelected->SetText(newText);
                iCandidateSelected->SetOrdinal(i + 1);
                DrawDeferred();
                }
            if(i < iVisibleCount)
                {
                // only redraw labels that are visible
                TRect rect = candidateLabel->Rect();
                Window().Invalidate(rect);
            }
        }
    LayoutLabels();
    LayoutCandidateSelected();
    DrawDeferred();
    UpdateLabelVisibility();
    }

void CAknFepUICtrlCandidatePane::SetHighlighted(TBool aHighlighted)
    {
    if(!COMPARE_BOOLS(iHighlighted, aHighlighted))
        {
        iHighlighted = aHighlighted;
        iCandidateSelected->MakeVisible(aHighlighted);

        iCandidateSelected->DrawDeferred();
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
        if(iSelected == aIndex)
            {
            LayoutCandidateSelected();
            }
        // only do drawing if visible
        if(aIndex < iVisibleCount)
            {
            LayoutLabel(aIndex);
            if(iSelected == aIndex && iHighlighted)
                {
                iCandidateSelected->DrawDeferred();
                }
            else
                {
                CEikLabel* candidateLabel = iCandidateLabelArray[aIndex];
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
    // If the state is changed, the arrows will be redrawn in the new state.
    //    if(!COMPARE_BOOLS(iShowHorizontalScrollArrows, aValue))
    //        {
    //        iShowHorizontalScrollArrows = aValue;
    //        TRect rect = iIndiFepArrowLeft.Rect();
    //        Window().Invalidate(rect);
    //        rect = iIndiFepArrowRight.Rect();
    //        Window().Invalidate(rect);
    //        }
    }

void CAknFepUICtrlCandidatePane::ShowUpScrollArrows(TBool aValue)
    {
    if (iShowUpScrollArrows != aValue)
        {
        iShowUpScrollArrows = aValue;
        TRect rect = iIndiFepArrowUp.Rect();
        Window().Invalidate(rect);
        }
    }
    
void CAknFepUICtrlCandidatePane::ShowDownScrollArrows(TBool aValue)
    {
    if (iShowDownScrollArrows != aValue)
        {
        iShowDownScrollArrows = aValue;
        TRect rect = iIndiFepArrowDown.Rect();
        Window().Invalidate(rect);
        }
        
    }
    
void CAknFepUICtrlCandidatePane::ShowLeftScrollArrows(TBool /*aValue*/)
    {
    //    if (iShowLeftScrollArrows != aValue)
    //        {
    //        iShowLeftScrollArrows = aValue;
    //        TRect rect = iIndiFepArrowLeft.Rect();
    //        Window().Invalidate(rect);
    //        }
    }
        
void CAknFepUICtrlCandidatePane::ShowRightScrollArrows(TBool /*aValue*/)
    {
    //    if (iShowRightScrollArrows != aValue)
    //        {
    //        iShowRightScrollArrows = aValue;
    //        TRect rect = iIndiFepArrowRight.Rect();
    //        Window().Invalidate(rect);
    //        }
    }

TBool CAknFepUICtrlCandidatePane::IsShowVerticalScrollArrows() const
    {
    return iShowVerticalScrollArrows;
    }

void CAknFepUICtrlCandidatePane::ShowVerticalScrollArrows(TBool aValue)
    {
    // If the state is changed, the arrows will be redrawn in the new state.
    if(!COMPARE_BOOLS(iShowVerticalScrollArrows, aValue))
        {
        iShowVerticalScrollArrows = aValue;
        TRect rect = iIndiFepArrowUp.Rect();
        Window().Invalidate(rect);
        rect = iIndiFepArrowDown.Rect();
        Window().Invalidate(rect);
        }
    }

void CAknFepUICtrlCandidatePane::ShowCandidateOrdinals(TBool aValue)
    {
    if(!COMPARE_BOOLS(iShowCandidateOrdinals, aValue))
        {
        iShowCandidateOrdinals = aValue;
        iCandidateSelected->ShowOrdinal(aValue);
        UpdateLabelVisibility();
        }
    }

TBool CAknFepUICtrlCandidatePane::IsShowCandidateOrdinals() const
    {
    return iShowCandidateOrdinals;
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
    else
        {
        return EFalse;
        }
    }

TBool CAknFepUICtrlCandidatePane::SelectIndex(TInt aIndex, TBool selected)
    {
    if(aIndex + 1 > iVisibleCount || aIndex < 0)
        {
        return EFalse;
        }
    if(iSelected != aIndex)
        {
        if(iSelected + 1 > iVisibleCount && iHighlighted)
            {
            // candidate selection is currently hidden, as the current 
            // selection is outside the range of valid candidates, therefore 
            // show it again
            iCandidateSelected->MakeVisible(ETrue);
            }
        iSelected = aIndex;
        // move and repopulate the selected candidate control
        const TDesC* txtLabel = iCandidateLabelArray[aIndex]->Text();
        iCandidateSelected->SetText(*txtLabel);
        iCandidateSelected->SetOrdinal(iSelected + 1);
        
        LayoutCandidateSelected();
        if( selected )
           {

           DrawDeferred();
           }
        
		}
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
        CAknFepChineseUIDataConv::AnyToTextLine(iLafDataMgr->RequestData( ECandidatePaneTextLine ));
       
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
    LayoutRects();
    LayoutContainedControls();
    }

TInt CAknFepUICtrlCandidatePane::CountComponentControls() const
    {
    TInt count = 0;
    count += iCandidateLabelArray.Count();
    count += iOrdinalLabelArray.Count();

    CCoeControl* controls[] =
        {
        iCandidateSelected
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
    TInt ordinalCount = iOrdinalLabelArray.Count();

    if(aIndex < candidateCount)
        {
        // because this method is const, the const [] operator is selected
        // which means that the pointer is const, so have to cast away constness
        // - a possible alternative is to make the array mutable?
        return CONST_CAST(CEikLabel*, iCandidateLabelArray[aIndex]);
        }
    aIndex -= candidateCount;

    if(aIndex < ordinalCount)
        {
        // because this method is const, the const [] operator is selected
        // which means that the pointer is const, so have to cast away constness
        // - a possible alternative is to make the array mutable?
        return CONST_CAST(CEikLabel*, iOrdinalLabelArray[aIndex]);
        }
    aIndex -= ordinalCount;

    CCoeControl* controls[] =
        {
        iCandidateSelected
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

void CAknFepUICtrlCandidatePane::Draw(const TRect& /* aRect */) const
    {
    CWindowGc& gc = SystemGc();

    if(iShowHorizontalScrollArrows)
        {
        if (iShowLeftScrollArrows)
            {
            iIndiFepArrowLeft.DrawImage(gc, 
                                        iNaviArrowBitmapLeft, 
                                        iNaviArrowBitmapLeftMask);     
            }
        
        if (iShowRightScrollArrows)
            {
            iIndiFepArrowRight.DrawImage(gc, 
                                         iNaviArrowBitmapRight, 
                                         iNaviArrowBitmapRightMask);
            }
        }
    if(iShowVerticalScrollArrows)
        {
        if (iShowUpScrollArrows)
            {
            iIndiFepArrowUp.DrawImage(gc,
                                      iNaviArrowBitmapUp, 
                                      iNaviArrowBitmapUpMask);
            }
            
        if (iShowDownScrollArrows)
            {
            iIndiFepArrowDown.DrawImage(gc, 
                                        iNaviArrowBitmapDown, 
                                        iNaviArrowBitmapDownMask);
            }
        }
    }

void CAknFepUICtrlCandidatePane::ConstructL(RWindow& aParent)
    {
    SetContainerWindowL(aParent);
    ConstructBitmapsL();

    TBuf<EOneCandidate> ordinal;
    for(TInt i = 0; i < EMaxCandidates; i++)
        {
        CEikLabel* candidateLabel = new(ELeave) CEikLabel;
        CleanupStack::PushL(candidateLabel);
        candidateLabel->SetContainerWindowL(aParent);
        candidateLabel->SetBufferReserveLengthL(EPhraseLength);
        // ownership is passed, unless allocation is unsuccessful, in which case we still have it on the stack
        User::LeaveIfError(iCandidateLabelArray.Append(candidateLabel));
        CleanupStack::Pop(); // candidateLabel

        CEikLabel* ordinalLabel = new(ELeave) CEikLabel;
        CleanupStack::PushL(ordinalLabel);
        ordinalLabel->SetContainerWindowL(aParent);
        ordinalLabel->SetBufferReserveLengthL(EOneCandidate);
        ordinal.Num((i+1)%10); // digits are [1, 2,,, 9, 0] - only want the last digit in the case of 10!
        ordinalLabel->SetTextL(ordinal);
        // ownership is passed, unless allocation is unsuccessful, in which case we still have it on the stack
        User::LeaveIfError(iOrdinalLabelArray.Append(ordinalLabel));
        CleanupStack::Pop(); // ordinalLabel

        User::LeaveIfError(iCandidateModeArray.Append(ECandidateModeNormal));
        }

    iCandidateSelected = CAknFepUICtrlCandidateSelected::NewL(aParent);
    iCandidateSelected->MakeVisible(iHighlighted);
    iCandidateSelected->SetMopParent(this);
    iCandidateArray = new(ELeave) CDesCArrayFlat(1);
    }

void CAknFepUICtrlCandidatePane::ConstructBitmapsL()
    {    
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();  
    
    TFileName bmpFile;
    bmpFile.Copy(_L("z:"));
    bmpFile.Append(KDC_BITMAP_DIR);
    bmpFile.Append(KFepUiControlsBitmapFile);  

    AknsUtils::CreateColorIconL( skin,
                                 KAknsIIDQgnIndiFepArrowLeft,
                                 KAknsIIDQsnIconColors,
                                 EAknsCIQsnIconColorsCG20,
                                 iNaviArrowBitmapLeft,
                                 iNaviArrowBitmapLeftMask,
                                 bmpFile,
                                 EMbmAknfepuictrlsQgn_indi_fep_arrow_left,
                                 EMbmAknfepuictrlsQgn_indi_fep_arrow_left_mask,
                                 AKN_LAF_COLOR( 0 )
                             );    
    AknsUtils::CreateColorIconL( skin,
                                 KAknsIIDQgnIndiFepArrowRight,
                                 KAknsIIDQsnIconColors,
                                 EAknsCIQsnIconColorsCG20,
                                 iNaviArrowBitmapRight,
                                 iNaviArrowBitmapRightMask,
                                 bmpFile,
                                 EMbmAknfepuictrlsQgn_indi_fep_arrow_right,
                                 EMbmAknfepuictrlsQgn_indi_fep_arrow_right_mask,
                                 AKN_LAF_COLOR( 0 )
                             );    

    AknsUtils::CreateColorIconL( skin,
                                 KAknsIIDQgnIndiFepArrowUp,
                                 KAknsIIDQsnIconColors,
                                 EAknsCIQsnIconColorsCG20,
                                 iNaviArrowBitmapUp,
                                 iNaviArrowBitmapUpMask,
                                 bmpFile,
                                 EMbmAknfepuictrlsQgn_indi_fep_arrow_up,
                                 EMbmAknfepuictrlsQgn_indi_fep_arrow_up_mask,
                                 AKN_LAF_COLOR( 0 )
                             );    
    AknsUtils::CreateColorIconL( skin,
                                 KAknsIIDQgnIndiFepArrowDown,
                                 KAknsIIDQsnIconColors,
                                 EAknsCIQsnIconColorsCG20,
                                 iNaviArrowBitmapDown,
                                 iNaviArrowBitmapDownMask,
                                 bmpFile,
                                 EMbmAknfepuictrlsQgn_indi_fep_arrow_down,
                                 EMbmAknfepuictrlsQgn_indi_fep_arrow_down_mask,
                                 AKN_LAF_COLOR( 0 )
                             );    
    }

CAknFepUICtrlCandidatePane::CAknFepUICtrlCandidatePane(CAknFepUiLayoutDataMgr* aLafDataMgr)
    :
    iPaneLayout(CAknFepUICtrlContainerChinese::ELayoutCandidate),
    iHighlighted(EFalse),
    iShowHorizontalScrollArrows(EFalse),
    iShowVerticalScrollArrows(EFalse),
    iShowUpScrollArrows(ETrue),
    iShowDownScrollArrows(ETrue),
    iShowLeftScrollArrows(ETrue),
    iShowRightScrollArrows(ETrue),
    iShowCandidateOrdinals(ETrue),
    iCurDisplayPage(0),
    iOrdinalLableWidth(0)
    {
    iLafDataMgr = aLafDataMgr;
    }

void CAknFepUICtrlCandidatePane::LayoutRects()
    {
    TRect rect = Rect();
    
    TAknWindowLineLayout candidateWindowLine1 = 
      CAknFepChineseUIDataConv::AnyToWindowLine( iLafDataMgr->RequestData(ECandidateWindowLine1));
    TAknWindowLineLayout candidateWindowLine2 = 
      CAknFepChineseUIDataConv::AnyToWindowLine( iLafDataMgr->RequestData(ECandidateWindowLine2));
    TAknWindowLineLayout candidateWindowLine3 = 
      CAknFepChineseUIDataConv::AnyToWindowLine( iLafDataMgr->RequestData(ECandidateWindowLine3));
    TAknWindowLineLayout candidateWindowLine4 = 
      CAknFepChineseUIDataConv::AnyToWindowLine( iLafDataMgr->RequestData(ECandidateWindowLine4));

    iIndiFepArrowLeft.LayoutRect(rect, candidateWindowLine1); 
    AknIconUtils::SetSize(iNaviArrowBitmapLeft, iIndiFepArrowLeft.Rect().Size());

    iIndiFepArrowRight.LayoutRect(rect, candidateWindowLine2); 
    AknIconUtils::SetSize(iNaviArrowBitmapRight, iIndiFepArrowRight.Rect().Size());

    iIndiFepArrowUp.LayoutRect(rect, candidateWindowLine3);
    AknIconUtils::SetSize(iNaviArrowBitmapUp, iIndiFepArrowUp.Rect().Size());

    iIndiFepArrowDown.LayoutRect(rect, candidateWindowLine4);
    AknIconUtils::SetSize( iNaviArrowBitmapDown, iIndiFepArrowDown.Rect().Size() );
    }

void CAknFepUICtrlCandidatePane::LayoutContainedControls()
    {
    LayoutLabels();
    LayoutCandidateSelected();
    }

void CAknFepUICtrlCandidatePane::LayoutLabel(TInt aIndex)
    {
    __ASSERT_DEBUG(aIndex >= 0, AknFepUiInterfacePanic(EAknFepUiInterfacePanicAttemptedLayoutOutsideVisibleCount));
    __ASSERT_DEBUG(aIndex < iVisibleCount, AknFepUiInterfacePanic(EAknFepUiInterfacePanicAttemptedLayoutOutsideVisibleCount));
    TRect rect = Rect();
    CEikLabel* candidateLabel = iCandidateLabelArray[aIndex];
    CEikLabel* ordinalLabel = iOrdinalLabelArray[aIndex];
    TAknWindowLineLayout layoutCandidateItem = 
        CAknFepChineseUIDataConv::AnyToWindowLine( iLafDataMgr->RequestData(ELayoutCandidateItem));
    TAknTextLineLayout candidateLayout = 
        CAknFepChineseUIDataConv::AnyToTextLine( iLafDataMgr->RequestData(ECandidatePaneTextLine));
    
    // layout candidate label
    TAknLayoutRect layoutRectCandidateItem;
    layoutRectCandidateItem.LayoutRect(rect,layoutCandidateItem);
    TRect rectCandidateItem = layoutRectCandidateItem.Rect();
    
    if(aIndex > 0)
	    {
	    rectCandidateItem.iTl.iX = iCandidateLabelArray[aIndex - 1]->Rect().iBr.iX;
        }
    TAknLayoutText layoutText;
    layoutText.LayoutText(rectCandidateItem,candidateLayout);
    TRect rectText = layoutText.TextRect();
    rectText.SetWidth(LabelTextWidthInPixels(aIndex));

    candidateLabel->SetRect(rectText);
    candidateLabel->SetFont(layoutText.Font());

    // layout ordinal label
    TAknTextLineLayout ordinalLayout = 
        CAknFepChineseUIDataConv::AnyToTextLine( iLafDataMgr->RequestData(EOrdinalCandidateTextLayout));
    TBufC16<1> buf;
    TAknLayoutText layoutOrdinal;
    TPtr16 ptr = buf.Des();
    ptr = (*(ordinalLabel->Text())).Left(EOneCandidate);
    
    layoutOrdinal.LayoutText(rectCandidateItem,ordinalLayout);
    
    const CFont* font = layoutOrdinal.Font();
    
    TRect rectOrdinal = layoutOrdinal.TextRect();
    // align ordinal label and candidate label 
    
    /* removed in 2006.03.29
    rectOrdinal.iTl.iY = rectText.iTl.iY;
    rectOrdinal.SetWidth(font->TextWidthInPixels(ptr));
    rectOrdinal.SetHeight(font->HeightInPixels());
    */
    
    // inserted in 2006.03.29
    TPoint ordinalPosition;
    TInt ordinalWidth = font->TextWidthInPixels(ptr);
    ordinalPosition.iX = rectText.iTl.iX - ordinalWidth;
    ordinalPosition.iY = rectText.iTl.iY;
    
    rectOrdinal = TRect(ordinalPosition,
                        TPoint(ordinalPosition.iX + rectOrdinal.Width(),
                               ordinalPosition.iY + rectOrdinal.Height()));
    // end inserting                               

    ordinalLabel->SetRect(rectOrdinal);
    ordinalLabel->SetFont(font);
    iOrdinalLableWidth = rectOrdinal.Width();

    // override colours if skin data present
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    TRgb candidateLabelTextColor = layoutText.Color();
    TRgb ordinalLabelTextColor = layoutOrdinal.Color();

    AknsUtils::GetCachedColor(skin, candidateLabelTextColor, 
                              KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG19);

    AknsUtils::GetCachedColor(skin, ordinalLabelTextColor,
        KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG19);

    TRAP_IGNORE(
        candidateLabel->OverrideColorL(EColorLabelText, candidateLabelTextColor);
        ordinalLabel->OverrideColorL(EColorLabelText, ordinalLabelTextColor);
        );
    }

void CAknFepUICtrlCandidatePane::LayoutLabels()
    {
    for(TInt i = 0; i < iVisibleCount; i++)
        {
        LayoutLabel(i);
        }
    }

void CAknFepUICtrlCandidatePane::LayoutCandidateSelected()
    {
    TCandidateMode candidateMode = iCandidateModeArray[iSelected];
    iCandidateSelected->SetCandidateMode(candidateMode);
    iCandidateSelected->SetLayout(iPaneLayout);

    TRect ordinalRect = iOrdinalLabelArray[iSelected]->Rect();

    TAknLayoutRect layoutRect;
    TAknWindowLineLayout layoutCandidateItem = 
        CAknFepChineseUIDataConv::AnyToWindowLine( iLafDataMgr->RequestData(ELayoutCandidateItem));
    layoutRect.LayoutRect(Rect(),layoutCandidateItem);
    
    TRect rectLayoutCandidateItem = layoutRect.Rect();
    rectLayoutCandidateItem.iTl.iX = ordinalRect.iTl.iX;

    TAknTextLineLayout candidateLayout = 
        CAknFepChineseUIDataConv::AnyToTextLine( iLafDataMgr->RequestData(ECandidatePaneTextLine));
    TAknTextLineLayout ordinalLayout = 
        CAknFepChineseUIDataConv::AnyToTextLine( iLafDataMgr->RequestData(EOrdinalCandidateTextLayout));
    rectLayoutCandidateItem.SetWidth(iCandidateLabelArray[iSelected]->Rect().Width() + candidateLayout.il - ordinalLayout.il);

    rectLayoutCandidateItem.SetHeight(rectLayoutCandidateItem.Height() - 1);
    
    iCandidateSelected->SetRect(rectLayoutCandidateItem);
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
        CEikLabel* candidateLabel = iCandidateLabelArray[i];
        CEikLabel* ordinalLabel = iOrdinalLabelArray[i];
        if(i < number)
            {
            candidateLabel->MakeVisible(ETrue);
            ordinalLabel->MakeVisible(iShowCandidateOrdinals); // only show if needed
            }
        else
            {
            candidateLabel->MakeVisible(EFalse);
            ordinalLabel->MakeVisible(EFalse); // don't show it as there is no character to go with it
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
    CEikLabel* candidateLabel;
    
    curPage.iStartIndex = index;
    curPage.iEndIndex = index;
    TInt candidateLeftNum = phraseCandidateNum - index > EMaxCandidates? 
                            EMaxCandidates : phraseCandidateNum - index;

    TAknWindowLineLayout layoutCandidateItem = 
        CAknFepChineseUIDataConv::AnyToWindowLine( iLafDataMgr->RequestData(ELayoutCandidateItem));
    TAknTextLineLayout candidateLayout = 
        CAknFepChineseUIDataConv::AnyToTextLine( iLafDataMgr->RequestData(ECandidatePaneTextLine));
    
    // avoid accessing layout data member
    TAknLayoutRect layoutRect;
    layoutRect.LayoutRect(Rect(),layoutCandidateItem);
    TRect rectLayoutCandidateItem = layoutRect.Rect();

    TAknLayoutText layoutText;
    layoutText.LayoutText(rectLayoutCandidateItem,candidateLayout);
    TRect rectText = layoutText.TextRect();
            
    TInt distance = rectText.iTl.iX - rectLayoutCandidateItem.iTl.iX + rectLayoutCandidateItem.iBr.iX - rectText.iBr.iX;
    rectLayoutCandidateItem.iTl.iX += distance;

    TInt width = 0;
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
            candidateLabel->SetTextL(text);
            });
            	
            }
        else
            {
            TRAP_IGNORE( candidateLabel->SetTextL(newText));
            width = LabelTextWidthInPixels(i)+ iOrdinalLableWidth; 
            rectLayoutCandidateItem.iTl.iX += width;
            }
        if (rectLayoutCandidateItem.iTl.iX >= rectLayoutCandidateItem.iBr.iX)
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
    if( iCandidateArray->Count() == iUnpagedIndex )
        {
        return iCurDisplayPage == iPages.Count() - 1;
        }
    return EFalse;
    }

void CAknFepUICtrlCandidatePane::SetInVisiable()
    {
    for (TInt i = 0; i < EMaxCandidates; i ++)
        {
        CEikLabel* candidateLabel = iCandidateLabelArray[i];
        CEikLabel* ordinalLabel = iOrdinalLabelArray[i];
        candidateLabel->MakeVisible(EFalse);
        ordinalLabel->MakeVisible(EFalse);
        }
    iCandidateSelected->MakeVisible(EFalse);
    }

TBool CAknFepUICtrlCandidatePane::NeedGetNewCandidatePage()
    {
    return (iCandidateArray->Count() - iUnpagedIndex < CandidateCountLeft);

    }

//Bug Fixing for ESSG-7P5CSZ
void CAknFepUICtrlCandidatePane::Enable(TBool aEnable)
	{
	if (!aEnable)
		{
		for (TInt i=0; i<iVisibleCount; i++)
			{
			TRAP_IGNORE(iCandidateLabelArray[i]->SetTextL(KNullDesC));
			}
		iCandidateArray->Reset();
		iVisibleCount = 0;
		DrawNow();
		}
	}
// End of file
