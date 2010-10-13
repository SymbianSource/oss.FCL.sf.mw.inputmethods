/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the CAknFepUICtrlPinyinPopup methods.
*
*/












#include <gdi.h>
#include <eiklabel.h>
#include <AknsDrawUtils.h>
#include <AknsUtils.h>
#include <AknIconUtils.h>

#include <AknsFrameBackgroundControlContext.h>
#include <aknfepuictrls.mbg>

#include "AknFepUiInterfacePanic.h"
#include "AknFepUIAvkonCtrlPinyinPopup.h"
#include "AknFepUIAvkonCtrlInputPane.h"
#include "aknfepchineseuidataconv.h"
#include "aknfepuilayoutdatamgr.h"
#include <skinlayout.cdl.h>

#include "data_caging_path_literals.hrh"

_LIT(KFepUiControlsBitmapFile, "aknfepuictrls.mbm");

CAknFepUICtrlPinyinPopup* CAknFepUICtrlPinyinPopup::NewL(RWindow& aParent, CAknFepUiLayoutDataMgr* aLafDataMgr )
    {
    CAknFepUICtrlPinyinPopup* self = new(ELeave) CAknFepUICtrlPinyinPopup( aLafDataMgr );
    CleanupStack::PushL(self);
    self->ConstructL(aParent);
    CleanupStack::Pop();
    return self;
    }

CAknFepUICtrlPinyinPopup::~CAknFepUICtrlPinyinPopup()
    {
    iInputPaneArray.ResetAndDestroy();
    iPages.Reset();
    delete iSpellingArray;
    delete iSpellingIndexArray;
    delete iNaviArrowBitmapLeft;
    delete iNaviArrowBitmapLeftMask;
    delete iNaviArrowBitmapRight;
    delete iNaviArrowBitmapRightMask;
    delete iBgContext;
    delete iKeystrokeArray;
    delete iInEffectKeystrokeArray;
    delete iShowKeystrokeArray;
    delete iOptimizeSpelling;
    delete iTempSpelling;
    delete iChooseChineseCharacterArray;
    delete iChooseChineseCharacterArraySpelling;
    delete iChooseChineseCharacterArrayKeystroke;
    
    delete iKeystrokeArrayForStroke;
    delete iInEffectKeystrokeArrayForStroke;
    delete iShowKeystrokeArrayForStroke;
    delete iChooseChineseCharacterArrayForStroke;
    delete iChooseChineseCharacterArrayKeystrokeForStroke;
    
    delete iZhuyinSymbols;
    delete iZhuyinShowSymbols;
    delete iFirstValidZhuyinGrp;
    delete iChineseCharsDisped;
    delete iZhuyinSymbolsForChineseCharsDisped;
    }

void CAknFepUICtrlPinyinPopup::SetSizeChangedObserver(
    MAknFepUICtrlPinyinPopupSizeChangedObserver* aObserver)
    {
    iSizeChangedObserver = aObserver;
    }

TBool CAknFepUICtrlPinyinPopup::IsEnabled() const
    {
    return iEnabled;
    }

void CAknFepUICtrlPinyinPopup::Enable(TBool aEnable)
    {
    // fix the refresh bug.
    TBool redraw = ( aEnable || iEnabled != aEnable );
    
    iEnabled = aEnable;
    if(aEnable)
        {
        Window().SetOrdinalPosition(0, ECoeWinPriorityFep); // right at the front
        }
    
    iSizeChangedObserver->SelectionPopupEnabled(aEnable);
    if (!aEnable)
    	{
        for(TInt i = 0; i < iVisibleCount; i++)
            {
            CAknFepUICtrlInputPane* inputPane = iInputPaneArray[i];
            inputPane->SetText(KNullDesC);
            }   
        iVisibleCount = 0;
    	}
    MakeVisible(aEnable);
    
    // fix the refresh bug.
    if( redraw ) 
        { 
        DrawDeferred(); 
        } 
    }

void CAknFepUICtrlPinyinPopup::SetVisibleSelectionCount(TInt aCount)
    {
    __ASSERT_DEBUG(aCount <= EMaxPhraseCandidate, AknFepUiInterfacePanic(EAknFepUiInterfacePanicNewVisibleCountExceedsRange));
    __ASSERT_DEBUG(aCount >= 1, AknFepUiInterfacePanic(EAknFepUiInterfacePanicNewVisibleCountExceedsRange));

    if(iVisibleCount != aCount)
        {
        iVisibleCount = aCount;
        TInt oldSelected = iSelected;

        if (iVisibleCount > 1) //the layout does not support a visible count of 1
            {
            if(aCount - 1 < oldSelected)
                {
                // automatically move the selection to be at the end of the new
                // candidate buffer, by forcing the one that will become the new 
                // highlighted one to be layout out highlighted before we are resized 
                // (note that briefly, there will be two items highlighted, but at least
                // there will be no flicker)
                iSelected = aCount - 1;
                LayoutInputPane(aCount - 1);        
                }

            // note that the old highlighted one isn't shown any more, so we don't need to lay it out again to hide its highlight
            SetInputPaneVisibility();
            iSizeChangedObserver->SelectionPopupSizeChanged(iVisibleCount); 
            DrawDeferred();
            }
        }
    }

void CAknFepUICtrlPinyinPopup::SetVisibleCount(TInt aCount)
    {
    if(iVisibleCount != aCount)
        {
        iVisibleCount = aCount;
        }
    }

TInt CAknFepUICtrlPinyinPopup::VisibleSelectionCount() const
    {
    return iVisibleCount;
    }

CDesCArrayFlat* CAknFepUICtrlPinyinPopup::PhoneticSpellingArray(void)
    {
    return iSpellingArray;
    }

CDesCArrayFlat* CAknFepUICtrlPinyinPopup::PhoneticSpellingIndexArray(void)
    {
    return iSpellingIndexArray;
    }

void CAknFepUICtrlPinyinPopup::SplitPhraseSpellingIntoPages(void)
    {
    iPages.Reset(); 
    TPage curPage;
    TInt spellingNum = iSpellingArray->Count();
    if(spellingNum == 0)
        {
        iVisibleCount = 0;
        return;
        }

    TInt index = 0;
    TBool beAppend = EFalse;
    
    while(index < spellingNum)
        {
        beAppend = EFalse;
        curPage.iStartIndex = index;
        curPage.iEndIndex = index;
    
        TInt spellingLeftNum = spellingNum - index > EMaxPhraseCandidate? EMaxPhraseCandidate:spellingNum - index;
        for(TInt j = 0; j < spellingLeftNum; j ++)  
            {
            iInputPaneArray[j]->SetText((*iSpellingArray)[index + j]);
            } 
        
        TAknWindowLineLayout layoutEntryItem = 
                CAknFepChineseUIDataConv::AnyToWindowLine( iLafDataMgr->RequestData(ELayoutEntryItem));
        TAknTextLineLayout entryTextLine = 
            CAknFepChineseUIDataConv::AnyToTextLine( iLafDataMgr->RequestData(EEntryTextLayout));
        TInt layoutLinePrevLeft = 0;
        TInt layoutLinePrevWidth = 0;

        TAknLayoutRect layoutRectEntryItem;
        layoutRectEntryItem.LayoutRect(Rect(),layoutEntryItem);
        TRect rectEntryItem = layoutRectEntryItem.Rect();
    
        TAknLayoutText layoutText;
        layoutText.LayoutText(rectEntryItem,entryTextLine);
        TRect rectText = layoutText.TextRect();
        
        TInt rightLimit = iIndiFepArrowRight.Rect().iTl.iX;
        // value is 3 for 176*208 resolution
        TInt distance = rectText.iTl.iX - rectEntryItem.iTl.iX + rectEntryItem.iBr.iX - rectText.iBr.iX;
        
        for(TInt i = 0;i < spellingLeftNum;i ++)
            {
            if(i > 0)
                {
                rectText.iTl.iX = TInt16(layoutLinePrevLeft + layoutLinePrevWidth);
                }
            rectText.SetWidth(iInputPaneArray[i]->LabelTextWidthInPixels() + distance);

            // if label's layout out of screen, forms another display page
            // > change >= change for phrase creation.
            if((rectText.iTl.iX >= rightLimit) || ((rectText.iTl.iX + rectText.Width()) >= rightLimit))
                {
                curPage.iEndIndex = index - 1;
                iPages.Append(curPage);
                beAppend = ETrue;
                break;
                }
            
            layoutLinePrevLeft = rectText.iTl.iX;
            layoutLinePrevWidth = rectText.Width();

            index ++;
            
            if( i == EMaxPhraseCandidate -1 )
            	{
                curPage.iEndIndex = index - 1;
                iPages.Append(curPage);
                beAppend = ETrue;
                break;
            	}
            
            }    
        }
    
    if( beAppend == EFalse)
    	{
    	curPage.iEndIndex = index - 1;
    	iPages.Append(curPage);
    	}
    iCurDisplayPage = 0;
    SetVisibleSelectionCount(iPages[0].iEndIndex - iPages[0].iStartIndex + 1);
    }

void CAknFepUICtrlPinyinPopup::SplitSpellingIntoPages(TInt aCount)
    {
    iPages.Reset(); 
    TPage curPage;
    if(aCount == 0)
        {
        return;
        }
    TInt index = 0;
    TBool beAppend = EFalse;
    
    while(index < aCount)
        {
        beAppend = EFalse;
        curPage.iStartIndex = index;
        curPage.iEndIndex = index;
        TInt spellingLeftNum = aCount - index > EMaxCandidates? EMaxCandidates:aCount - index;
 
        TAknWindowLineLayout layoutEntryItem = 
                CAknFepChineseUIDataConv::AnyToWindowLine( iLafDataMgr->RequestData(ELayoutEntryItem));
        TAknTextLineLayout entryTextLine = 
            CAknFepChineseUIDataConv::AnyToTextLine( iLafDataMgr->RequestData(EEntryTextLayout));
        TInt layoutLinePrevLeft = 0;
        TInt layoutLinePrevWidth = 0;

        TAknLayoutRect layoutRectEntryItem;
        layoutRectEntryItem.LayoutRect(Rect(),layoutEntryItem);
        TRect rectEntryItem = layoutRectEntryItem.Rect();
    
        TAknLayoutText layoutText;
        layoutText.LayoutText(rectEntryItem,entryTextLine);
        TRect rectText = layoutText.TextRect();
        
        TInt rightLimit = rectEntryItem.iTl.iX + rectEntryItem.Width();
        // value is 3 for 176*208 resolution
        TInt distance = rectText.iTl.iX - rectEntryItem.iTl.iX + rectEntryItem.iBr.iX - rectText.iBr.iX;
        for(TInt i = 0;i < spellingLeftNum;i ++)
            {
            if(i > 0)
                {
                rectText.iTl.iX = TInt16(layoutLinePrevLeft + layoutLinePrevWidth);
                }
            rectText.SetWidth(iInputPaneArray[i]->LabelTextWidthInPixels() + distance);

            // if label's layout out of screen, forms another display page
            if((rectText.iTl.iX >= rightLimit) || ((rectText.iTl.iX + rectText.Width()) >= rightLimit))
                {
                curPage.iEndIndex = index - 1;
                iPages.Append(curPage);
                beAppend = ETrue;
                break;
                }
            layoutLinePrevLeft = rectText.iTl.iX;
            layoutLinePrevWidth = rectText.Width();
            index ++;
            
            if( i == EMaxPhraseCandidate -1 )
            	{
            	curPage.iEndIndex = index - 1;
                iPages.Append(curPage);
                beAppend = ETrue;
                break;
            	}
            
            }
        }
    
    if(beAppend == EFalse)
    	{
        curPage.iEndIndex = index - 1;
        iPages.Append(curPage);
    	}
    
    iCurDisplayPage = 0;
    SetVisibleSelectionCount(iPages[0].iEndIndex - iPages[0].iStartIndex + 1);
    SelectIndex(0);
    }

void CAknFepUICtrlPinyinPopup::SetDisplayPage(TInt aSelection)
    {
    if(aSelection >= iVisibleCount)
        {
        for(TInt i = 0 ; i < iPages.Count(); i ++)
            {
            if((aSelection >= iPages[i].iStartIndex) && (aSelection <= iPages[i].iEndIndex))
                {
                iCurDisplayPage = i;
                SetVisibleSelectionCount(iPages[i].iEndIndex - iPages[i].iStartIndex + 1);
                SelectIndex(aSelection - iPages[i].iStartIndex);
                break;
                }
            }
        }
    else
        {
        SelectIndex(aSelection);
        }
    UpdateInputPaneVisibility();
    }

TInt CAknFepUICtrlPinyinPopup::CurrentPageStartIndex(void)
    {
    return iPages[iCurDisplayPage].iStartIndex;
    }

CDesCArrayFlat* CAknFepUICtrlPinyinPopup::KeystrokeArray()
	{
	return iKeystrokeArray;
	}

CDesCArrayFlat* CAknFepUICtrlPinyinPopup::InEffectKeystrokeArray()
	{
	return iInEffectKeystrokeArray;
	}

CDesCArrayFlat* CAknFepUICtrlPinyinPopup::ShowKeystrokeArray()
	{
	return iShowKeystrokeArray;
	}

CDesCArrayFlat* CAknFepUICtrlPinyinPopup::OptimizeSpelling()
	{
	return iOptimizeSpelling;
	}

CDesCArrayFlat* CAknFepUICtrlPinyinPopup::TempSpelling()
    {
    return iTempSpelling;
    }

CDesCArrayFlat* CAknFepUICtrlPinyinPopup::ChooseChineseCharacterArray()
	{
	return iChooseChineseCharacterArray;
	}
CDesCArrayFlat* CAknFepUICtrlPinyinPopup::ChooseChineseCharacterArraySpelling()
	{
	return iChooseChineseCharacterArraySpelling;
	}
CDesCArrayFlat* CAknFepUICtrlPinyinPopup::ChooseChineseCharacterArrayKeystroke()
	{
	return iChooseChineseCharacterArrayKeystroke;
	}

TBool CAknFepUICtrlPinyinPopup::GetTonemarkState()
	{
	return iTonemarkState;
	}

void CAknFepUICtrlPinyinPopup::SetTonemarkState( TBool aState )
	{
	iTonemarkState = aState;
	}

void CAknFepUICtrlPinyinPopup::UpdateInputPaneVisibility()
    {
    TInt number = 0;
    if(iPages.Count() != 0)
        {
        number = iPages[iCurDisplayPage].iEndIndex - iPages[iCurDisplayPage].iStartIndex + 1;
        }
    if(number > iVisibleCount)
        {
        number =  iVisibleCount;
        }
    for(TInt i = 0; i < EMaxPhraseCandidate; i++)
        {
        CAknFepUICtrlInputPane* inputPane = iInputPaneArray[i];
    
        if(i < number)
            {
            inputPane->MakeVisible(ETrue);
            }
        else
            {
            inputPane->MakeVisible(EFalse);
            }
        }
    }

TBool CAknFepUICtrlPinyinPopup::SetItemText(TInt aIndex, const TDesC& aText)
    {
    __ASSERT_DEBUG(aIndex < EMaxPhraseCandidate, AknFepUiInterfacePanic(EAknFepUiInterfacePanicAttemptedSetItemTextOutsideMaxCandidates));
    __ASSERT_DEBUG(aIndex >= 0, AknFepUiInterfacePanic(EAknFepUiInterfacePanicAttemptedSetItemTextOutsideMaxCandidates));

    if(aIndex > iVisibleCount)
        {
        return EFalse;
        }

    CAknFepUICtrlInputPane* inputPane = iInputPaneArray[aIndex];
    inputPane->SetText(aText);
    return ETrue;
    }

TBool CAknFepUICtrlPinyinPopup::SetPhraseItemTexts()
    {
    for(TInt i = 0; i < iVisibleCount; i ++)
        {
        TPtrC spelling = (*iSpellingArray)[i + iPages[iCurDisplayPage].iStartIndex];  // our index is zero based, engine index is one based
        SetItemText(i, spelling);
        }
    return ETrue;
    }

TBool CAknFepUICtrlPinyinPopup::GetItemText(TInt aIndex, TDes& aText) const
    {
    __ASSERT_DEBUG(aIndex < EMaxCandidates, AknFepUiInterfacePanic(EAknFepUiInterfacePanicAttemptedSetItemTextOutsideMaxCandidates));
    __ASSERT_DEBUG(aIndex >= 0, AknFepUiInterfacePanic(EAknFepUiInterfacePanicAttemptedSetItemTextOutsideMaxCandidates));

    const CAknFepUICtrlInputPane* inputPane = iInputPaneArray[aIndex];
    inputPane->GetText(aText);

    return ETrue;
    }

void CAknFepUICtrlPinyinPopup::SetToneMark(const TDesC& aToneMark)
    {
    for(TInt i = 0; i < EMaxPhraseCandidate; i++)
        {
        CAknFepUICtrlInputPane* inputPane = iInputPaneArray[i];
        inputPane->SetToneMark(aToneMark);
        }
    }

void CAknFepUICtrlPinyinPopup::SetToneMark(TInt aIndex, const TDesC& aToneMark)
    {
    CAknFepUICtrlInputPane* inputPane = iInputPaneArray[aIndex];
    inputPane->SetToneMark(aToneMark);

    LayoutInputPane(aIndex);
    }

void CAknFepUICtrlPinyinPopup::GetToneMark(TInt aIndex, TDes& aToneMark) const
    {
    const CAknFepUICtrlInputPane* inputPane = iInputPaneArray[aIndex];
    inputPane->GetToneMark(aToneMark);
    }

void CAknFepUICtrlPinyinPopup::SetItemToneMarkValidity(TInt aIndex, TBool aValid)
    {
    CAknFepUICtrlInputPane* inputPane = iInputPaneArray[aIndex];
    inputPane->SetToneMarkValidity(aValid);
    }



TBool CAknFepUICtrlPinyinPopup::SelectNextPhrase()
    {
    TInt maxIndex = iVisibleCount  - 1;
    TInt newIndex = iSelected;
    
    if((iSelected == maxIndex) )
        {
        // is last page 
        if(iCurDisplayPage == (iPages.Count() - 1))
            {
            iCurDisplayPage = 0;
            }
        else
            {
            iCurDisplayPage ++;
            }
        SetVisibleSelectionCount(iPages[iCurDisplayPage].iEndIndex - iPages[iCurDisplayPage].iStartIndex + 1);
        SetFlag(EDispPageChanged);
        newIndex = 0;
        }
    else if(iSelected < maxIndex)
        {
        newIndex ++;
        }
    else if(iSelected > maxIndex)
        {
        return EFalse;
        }
    return SelectIndex(newIndex);
    }


TBool CAknFepUICtrlPinyinPopup::SelectPrevPhrase()
    {
    TInt maxIndex = iVisibleCount - 1;
    TInt newIndex = iSelected;
    if(iSelected == 0)
        {
        // first page
        if(iCurDisplayPage == 0)
            {
            iCurDisplayPage = iPages.Count() - 1;
            }
        else
            {
            iCurDisplayPage --;
            }
        // loop to the end
        SetVisibleSelectionCount(iPages[iCurDisplayPage].iEndIndex - iPages[iCurDisplayPage].iStartIndex + 1);
        SetFlag(EDispPageChanged);
        newIndex = iVisibleCount - 1 ;
        }
    else if(iSelected >  0)
        {
        newIndex--;
        }
    else if(iSelected > maxIndex)
        {
        return EFalse;
        }
    return SelectIndex(newIndex);
    }

TBool CAknFepUICtrlPinyinPopup::SelectIndex(TInt aIndex)
    {
    __ASSERT_DEBUG(aIndex < iVisibleCount, AknFepUiInterfacePanic(EAknFepUiInterfacePanicAttemptedSelectionOutsideVisibleCandidates));
    __ASSERT_DEBUG(aIndex >= 0, AknFepUiInterfacePanic(EAknFepUiInterfacePanicAttemptedSelectionOutsideVisibleCandidates));
    // If the value of the index changes, the control is redrawn to show the new position.
    if(iSelected != aIndex)
        {
        // only want to layout the two controls between which the selection is moving
        TInt oldSelected = iSelected;
        iSelected = aIndex;

        LayoutInputPane(oldSelected);
        LayoutInputPane(iSelected);
        }
    return ETrue;
    }

TInt CAknFepUICtrlPinyinPopup::CurrentSelection() const
    {
    if(iPages.Count() != 0)
        {
        return iPages[iCurDisplayPage].iStartIndex + iSelected;
        }
    else
        {
        return 0;
        }
    }

TInt CAknFepUICtrlPinyinPopup::CurrentVisibleSelection() const
    {
    return iSelected;
    }

void CAknFepUICtrlPinyinPopup::PopupSizeChanged()
	{
	SizeChanged();
	}
	
void CAknFepUICtrlPinyinPopup::SizeChanged()
    {
    LayoutRects();
    LayoutContainedControls();
    LayoutFrames();
    }

TInt CAknFepUICtrlPinyinPopup::CountComponentControls() const
    {
    TInt count = 0;
    count += iInputPaneArray.Count();
    
    CCoeControl* controls[] = 
        {
        0
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

CCoeControl* CAknFepUICtrlPinyinPopup::ComponentControl(TInt aIndex) const 
    {
    TInt candidateCount = iInputPaneArray.Count();

    if(aIndex < candidateCount)
        {
        // because this method is const, the const [] operator is selected
        // which means that the pointer is const, so have to cast away constness 
        // - a possible alternative is to make the array mutable
        return CONST_CAST(CAknFepUICtrlInputPane*, iInputPaneArray[aIndex]);
        }
    aIndex -= candidateCount;

    CCoeControl* controls[] = 
        {
        0
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

void CAknFepUICtrlPinyinPopup::Draw(const TRect& /*aRect*/) const
    {
    CWindowGc& gc = SystemGc();
    TBool skinnedDraw = EFalse;
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();

    TRect outerRect;
    TRect innerRect;
    CalculateFrameRects(outerRect, innerRect);
    
    gc.Clear(outerRect);
    TRect tmpRect;
    tmpRect.iTl.iX = innerRect.iTl.iX;
    tmpRect.iTl.iY = outerRect.iTl.iY;
    tmpRect.SetWidth(innerRect.Width());
    tmpRect.SetHeight(outerRect.Height());
    skinnedDraw = AknsDrawUtils::DrawFrame(skin, gc, outerRect, tmpRect,
        KAknsIIDQsnFrPopupSub, KAknsIIDQsnFrPopupCenterSubmenu);
	    
    // draw the rectangles
    if(!skinnedDraw)
        {
        iRectCoverMainPaneInput.DrawRect(gc);
        iRectFirstShadowInput.DrawRect(gc);
        iRectOutlineFrameInput.DrawRect(gc);
        iRectInsideAreaInput.DrawRect(gc);
        }

    if((iVisibleCount >= 1) && (iPages.Count() > 1))
        {
        if(iCurDisplayPage != 0)
            {
            iIndiFepArrowLeft.DrawImage(gc, iNaviArrowBitmapLeft, iNaviArrowBitmapLeftMask);
            }

        if (iPages.Count() > iCurDisplayPage + 1)
            {
            iIndiFepArrowRight.DrawImage(gc, iNaviArrowBitmapRight, iNaviArrowBitmapRightMask);
            }
	    }
    }

TTypeUid::Ptr CAknFepUICtrlPinyinPopup::MopSupplyObject(TTypeUid aId)
    {
    if (aId.iUid == MAknsControlContext::ETypeId)
        return ( MAknsControlContext::SupplyMopObject(aId, iBgContext));
    return CCoeControl::MopSupplyObject(aId);
    }

void CAknFepUICtrlPinyinPopup::ConstructL(RWindow& /*aParent*/)
    {
    CreateWindowL();
    ConstructBitmapsL();
    RWindow& window = Window();

    for(TInt i = 0; i < EMaxPhraseCandidate; i++)
        {
        CAknFepUICtrlInputPane* inputPane = 
            CAknFepUICtrlInputPane::NewL(window, CAknFepUICtrlInputPane::ELayoutPhrasePinyinPopupPane, iLafDataMgr);
        inputPane->SetMopParent(this);
        CleanupStack::PushL(inputPane);
        // ownership is passed, unless allocation is unsuccessful, in which case we still have it on the stack
        User::LeaveIfError(iInputPaneArray.Append(inputPane));
        CleanupStack::Pop(); // inputPane
        }

    iSpellingArray = new(ELeave)CDesCArrayFlat(1);
    iSpellingIndexArray = new(ELeave)CDesCArrayFlat(1);
    iKeystrokeArray = new(ELeave)CDesCArrayFlat(1);
    iShowKeystrokeArray = new(ELeave)CDesCArrayFlat(1);
    iOptimizeSpelling = new(ELeave)CDesCArrayFlat(1);
    iTempSpelling = new(ELeave)CDesCArrayFlat(1);
    iInEffectKeystrokeArray = new(ELeave)CDesCArrayFlat(1);
    iChooseChineseCharacterArray = new(ELeave)CDesCArrayFlat(1);
    iChooseChineseCharacterArrayKeystroke = new(ELeave)CDesCArrayFlat(1);
    iChooseChineseCharacterArraySpelling = new(ELeave)CDesCArrayFlat(1);
    //For stroke
    iKeystrokeArrayForStroke = new(ELeave)CDesCArrayFlat(1);
    iInEffectKeystrokeArrayForStroke = new(ELeave)CDesCArrayFlat(1);
    iShowKeystrokeArrayForStroke = new(ELeave)CDesCArrayFlat(1);
    iChooseChineseCharacterArrayForStroke = new(ELeave)CDesCArrayFlat(1);
    iChooseChineseCharacterArrayKeystrokeForStroke = new(ELeave)CDesCArrayFlat(1);
    
    iZhuyinSymbols = new(ELeave)CDesCArrayFlat(1);
    iZhuyinShowSymbols = new(ELeave)CDesCArrayFlat(1);
    iFirstValidZhuyinGrp = new(ELeave)CDesCArrayFlat(1);
    iChineseCharsDisped = new(ELeave)CDesCArrayFlat(1);
    iZhuyinSymbolsForChineseCharsDisped = new(ELeave)CDesCArrayFlat(1);
    
    SetNonFocusing();
    window.SetOrdinalPosition(0, ECoeWinPriorityFep); // right at the front
    window.SetNonFading(ETrue);

    TRect outerRect = TRect();
    TRect innerRect = TRect();
    CalculateFrameRects(outerRect, innerRect);

    iBgContext = CAknsFrameBackgroundControlContext::NewL(KAknsIIDQsnFrPopupSub, outerRect, innerRect, ETrue);
    iLastResouce = 0;
    MakeVisible(EFalse);
    }

void CAknFepUICtrlPinyinPopup::ConstructBitmapsL()
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
    }

CAknFepUICtrlPinyinPopup::CAknFepUICtrlPinyinPopup( CAknFepUiLayoutDataMgr* aLafDataMgr )
    : 
    iSelected(0),
    iEnabled(EFalse),
    iVisibleCount(CAknFepUICtrlPinyinPopup::EMaxPhraseCandidate),
    iCurDisplayPage(0),
    iTonemarkState(1),
    iStrokeState(0),
    iHighLightFlag( 0 ),
    iFlags(0),    
    iValidKeystrokeCount( 0 ),
    iPhraseCreation(0),
    iPhraseCreationEditingState( 0 ),
    iState(1),    
    iChangeStateFromEditToSpelling( 0 ),
    iValidKeystrokeChanged( 0 )
    {
    iLafDataMgr = aLafDataMgr;
    iEEPContent.Zero();
    }

void CAknFepUICtrlPinyinPopup::LayoutRects()
    {
    // left arrow
    iIndiFepArrowLeft = 
        CAknFepChineseUIDataConv::AnyToLayoutRect( iLafDataMgr->RequestData( EIndiFepArrowLeft ));
    AknIconUtils::SetSize( iNaviArrowBitmapLeft, iIndiFepArrowLeft.Rect().Size());

    // right arrow
    iIndiFepArrowRight = 
        CAknFepChineseUIDataConv::AnyToLayoutRect( iLafDataMgr->RequestData( EIndiFepArrowRight ));
    AknIconUtils::SetSize( iNaviArrowBitmapRight, iIndiFepArrowRight.Rect().Size());

    // cover main pane
    iRectCoverMainPaneInput = 
        CAknFepChineseUIDataConv::AnyToLayoutRect( iLafDataMgr->RequestData( ERectCoverMainPaneInput ));
    // first shadow
    iRectFirstShadowInput = 
        CAknFepChineseUIDataConv::AnyToLayoutRect( iLafDataMgr->RequestData( ERectFirstShadowInput ));
    // outline frame
    iRectOutlineFrameInput = 
        CAknFepChineseUIDataConv::AnyToLayoutRect( iLafDataMgr->RequestData( ERectOutlineFrameInput ));
    // inside area
    iRectInsideAreaInput = 
        CAknFepChineseUIDataConv::AnyToLayoutRect( iLafDataMgr->RequestData( ERectInsideAreaInput ));
    }

void CAknFepUICtrlPinyinPopup::LayoutContainedControls()
    {
    LayoutInputPanes();
    }

void CAknFepUICtrlPinyinPopup::LayoutInputPanes()
    {
    for(TInt i = 0; i < EMaxPhraseCandidate; i++)
        {
        LayoutInputPane(i);
        }
    }

void CAknFepUICtrlPinyinPopup::LayoutInputPane(TInt aIndex)
    {
    TRect rect = Rect();

    CAknFepUICtrlInputPane* inputPane = iInputPaneArray[aIndex];
    TAknWindowLineLayout layoutEntryItem = 
            CAknFepChineseUIDataConv::AnyToWindowLine( iLafDataMgr->RequestData(ELayoutEntryItem));
    TAknTextLineLayout layoutLineText = 
        CAknFepChineseUIDataConv::AnyToTextLine( iLafDataMgr->RequestData(EEntryTextLayout));
    TAknLayoutRect layoutRectEntryItem;
    layoutRectEntryItem.LayoutRect(rect,layoutEntryItem);
    TRect rectEntryItem = layoutRectEntryItem.Rect();
    
    TAknLayoutText layoutText;
    layoutText.LayoutText(rectEntryItem,layoutLineText);
    TRect rectText = layoutText.TextRect();
    
    // value is 3 for 176*208 resolution
    TInt distance = rectText.iTl.iX - rectEntryItem.iTl.iX + rectEntryItem.iBr.iX - rectText.iBr.iX;
    
    if( aIndex > 0)
        {
        CAknFepUICtrlInputPane* inputPanePrev = iInputPaneArray[aIndex - 1];
        rectText.iTl.iX = inputPanePrev->Rect().iBr.iX;
        }
    rectText.SetWidth(inputPane->LabelTextWidthInPixels() + distance);
    rectText.SetHeight(rect.Height());

    inputPane->SetRect(rectText);
    inputPane->SetHighlighted(aIndex == iSelected);
    }

void CAknFepUICtrlPinyinPopup::SetInputPaneVisibility()
    {
    TInt number = iVisibleCount;
    CWindowGc& gc = SystemGc();
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    MAknsControlContext* cc = AknsDrawUtils::ControlContext(this);
    TBool skinnedDraw = AknsDrawUtils::Background(skin, cc, this, gc, Rect(),KAknsDrawParamPrepareOnly);
 
    for(TInt i = 0; i < EMaxPhraseCandidate; i++)
        {
        CAknFepUICtrlInputPane* inputPane = iInputPaneArray[i];
 
        //TBool skinnedDraw = AknsDrawUtils::HasBitmapBackground(skin, cc);
        if(skinnedDraw)
            {
            // show input panes, when skinned, as they must draw background context
            // to avoid (the one that is almost off-screen) flickering behind
            // however, need to hide the text so that is doesn't show
            inputPane->MakeTextVisible(i < number);
            }
        else
            {
            // if not skinned, must hide the offscreen input panes otherwise they 
            // will draw over the border rectangle
            inputPane->MakeVisible(i < number);
            }
        }
    }

void CAknFepUICtrlPinyinPopup::LayoutFrames()
    {
    TRect outerRect;
    TRect innerRect;
    CalculateFrameRects(outerRect, innerRect);
    iBgContext->SetFrameRects(outerRect, innerRect);
    iBgContext->SetParentPos(PositionRelativeToScreen());
    }

void CAknFepUICtrlPinyinPopup::CalculateFrameRects(TRect& aOuterRect, TRect& aInnerRect) const
    {
    TRect windowRect = 
        CAknFepChineseUIDataConv::AnyToRect( iLafDataMgr->RequestData( EPopupRectEntry ));
    TInt rectHeight = windowRect.iBr.iY - windowRect.iTl.iY;
    TInt rectWidth = windowRect.Width();
    windowRect.iTl.iX = 0;
    windowRect.iTl.iY = 0;
    windowRect.SetHeight(rectHeight);
    windowRect.SetWidth(rectWidth);

    TAknLayoutRect topLeft;
    topLeft.LayoutRect(windowRect, SkinLayout::Submenu_skin_placing_Line_2());

    TAknLayoutRect bottomRight;
    bottomRight.LayoutRect(windowRect, SkinLayout::Submenu_skin_placing_Line_5());
    
    aOuterRect = TRect(topLeft.Rect().iTl, bottomRight.Rect().iBr);
    aInnerRect = TRect(topLeft.Rect().iBr, bottomRight.Rect().iTl);
    }

void CAknFepUICtrlPinyinPopup::SetNoneHighlighted()
    {
    TInt number = iVisibleCount;
    CWindowGc& gc = SystemGc();
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    MAknsControlContext* cc = AknsDrawUtils::ControlContext(this);
    TBool skinnedDraw = AknsDrawUtils::Background(skin, cc, this, gc, Rect(),KAknsDrawParamPrepareOnly);
 
    for(TInt i = 0; i < EMaxPhraseCandidate; i++)
        {
        CAknFepUICtrlInputPane* inputPane = iInputPaneArray[i];
 
        //TBool skinnedDraw = AknsDrawUtils::HasBitmapBackground(skin, cc);
        if(skinnedDraw)
            {
            // show input panes, when skinned, as they must draw background context
            // to avoid (the one that is almost off-screen) flickering behind
            // however, need to hide the text so that is doesn't show
            inputPane->SetHighlighted( !(i < number) );
            }
        }
    }
CDesCArrayFlat* CAknFepUICtrlPinyinPopup::KeystrokeArrayForStroke()
    {
    return iKeystrokeArrayForStroke;
    }

CDesCArrayFlat* CAknFepUICtrlPinyinPopup::InEffectKeystrokeArrayForStroke()
    {
    return iInEffectKeystrokeArrayForStroke;
    }

CDesCArrayFlat* CAknFepUICtrlPinyinPopup::ShowKeystrokeArrayForStroke()
    {
    return iShowKeystrokeArrayForStroke;
    }

CDesCArrayFlat* CAknFepUICtrlPinyinPopup::ChooseChineseCharacterArrayForStroke()
    {
    return iChooseChineseCharacterArrayForStroke;
    }

CDesCArrayFlat* CAknFepUICtrlPinyinPopup::ChooseChineseCharacterArrayKeystrokeForStroke()
    {
    return iChooseChineseCharacterArrayKeystrokeForStroke;
    }

CDesCArrayFlat* CAknFepUICtrlPinyinPopup::ZhuyinSymbols()
    {
    return this->iZhuyinSymbols;
    }

CDesCArrayFlat* CAknFepUICtrlPinyinPopup::ZhuyinShowSymbols()
    {
    return this->iZhuyinShowSymbols;
    }

CDesCArrayFlat* CAknFepUICtrlPinyinPopup::FirstValidZhuyinGrp()
    {
    return this->iFirstValidZhuyinGrp;
    }

void CAknFepUICtrlPinyinPopup::SetEEPContent ( const TDes& aDes )
    {
    iEEPContent = aDes;
    }

TDes& CAknFepUICtrlPinyinPopup::GetEEPContent ( )
    {
    return iEEPContent;
    }

void CAknFepUICtrlPinyinPopup::SetEEPContentValid ( TBool aValid )
    {
    iEEPContentValid = aValid;
    }

TBool CAknFepUICtrlPinyinPopup::GetEEPContentValid ( )
    {
    return iEEPContentValid;
    }

CDesCArrayFlat* CAknFepUICtrlPinyinPopup::ChineseCharsDisped()
    {
    return this->iChineseCharsDisped;
    }

CDesCArrayFlat* CAknFepUICtrlPinyinPopup::ZhuyinSymbolsForChineseCharsDisped()
    {
    return this->iZhuyinSymbolsForChineseCharsDisped;
    }

void CAknFepUICtrlPinyinPopup::ResetStrokeArray()
    {
    //reset pinyin array
    iSpellingArray->Reset();
    iSpellingArray->Compress();
    iSpellingIndexArray->Reset();
    iSpellingIndexArray->Compress();
    iKeystrokeArray->Reset();
    iKeystrokeArray->Compress();
    iInEffectKeystrokeArray->Reset();
    iInEffectKeystrokeArray->Compress();
    iShowKeystrokeArray->Reset();
    iShowKeystrokeArray->Compress();
    iChooseChineseCharacterArray->Reset();
    iChooseChineseCharacterArray->Compress();
    iChooseChineseCharacterArrayKeystroke->Reset();
    iChooseChineseCharacterArrayKeystroke->Compress();
    iChooseChineseCharacterArraySpelling->Reset();
    iChooseChineseCharacterArraySpelling->Compress();
    iOptimizeSpelling->Reset();
    iOptimizeSpelling->Compress();
    iTempSpelling->Reset();
    iTempSpelling->Compress();

    //reset stroke array
    iKeystrokeArrayForStroke->Reset();
    iKeystrokeArrayForStroke->Compress();
    iInEffectKeystrokeArrayForStroke->Reset();
    iInEffectKeystrokeArrayForStroke->Compress();
    iShowKeystrokeArrayForStroke->Reset();
    iShowKeystrokeArrayForStroke->Compress();
    iChooseChineseCharacterArrayForStroke->Reset();
    iChooseChineseCharacterArrayForStroke->Compress();
    iChooseChineseCharacterArrayKeystrokeForStroke->Reset();
    iChooseChineseCharacterArrayKeystrokeForStroke->Compress();
    
    //reset zhuyin array
    iZhuyinSymbols->Reset();
    iFirstValidZhuyinGrp->Reset();
    iChineseCharsDisped->Reset();
    iZhuyinSymbolsForChineseCharsDisped->Reset();
    
    iZhuyinSymbols->Compress();
    iFirstValidZhuyinGrp->Compress();
    iChineseCharsDisped->Compress();
    iZhuyinSymbolsForChineseCharsDisped->Compress();
    
    iLastResouce = 0;
    iValidKeystrokeCount = 0;
    iPhraseCreation = 0;
    iPhraseCreationEditingState = 0;
    iHighLightFlag = 0;
    iChangeStateFromEditToSpelling = 0;
    iValidKeystrokeChanged = 0;
    
    }

void CAknFepUICtrlPinyinPopup::SetChangeState( TBool aChangeState )
    {
    iChangeState =  aChangeState ;
    }

TBool CAknFepUICtrlPinyinPopup::IsChangeStatet() const
    {
    return iChangeState;
    }

void CAknFepUICtrlPinyinPopup::SetState( TInt aState )
    {
    iState = aState;
    }

TInt CAknFepUICtrlPinyinPopup::GetState()
    {
    return iState;
    }

void CAknFepUICtrlPinyinPopup::SetPhraseCreationState( TBool aState )
    {
    iPhraseCreation = aState;
    }

TBool CAknFepUICtrlPinyinPopup::GetPhraseCreationState()
    {
    return iPhraseCreation;
    }

void CAknFepUICtrlPinyinPopup::SetPhraseCreationEditingState( TBool aState )
    {
    iPhraseCreationEditingState = aState;
    }

TBool CAknFepUICtrlPinyinPopup::GetPhraseCreationEditingState()
    {
    return iPhraseCreationEditingState;
    }

// ---------------------------------------------------------
// Set flag whether from candidate state change state.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinPopup::SetFromCandidateChangeStateFlag( TBool aFlag )
    {
    iHighLightFlag = aFlag;
    }

// ---------------------------------------------------------
// Get flag whether from candidate state change state.
// ---------------------------------------------------------
//
TBool CAknFepUICtrlPinyinPopup::GetFromCandidateChangeStateFlag()
    {
    return iHighLightFlag;
    }

// ---------------------------------------------------------
// Set count of valid keystroke.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinPopup::SetValidKeystrokeCount( TInt aCount )
	{
	iValidKeystrokeCount = aCount;
	}

// ---------------------------------------------------------
// Set count of valid keystroke.
// ---------------------------------------------------------
//
TInt CAknFepUICtrlPinyinPopup::GetValidKeystrokeCount()
	{
	return iValidKeystrokeCount;
	}

// ---------------------------------------------------------
// Set last cba resouce id.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinPopup::SetLastResouce( TInt aResouceId )
    {
    iLastResouce = aResouceId;
    }

// ---------------------------------------------------------
// Get last cba resouce id.
// ---------------------------------------------------------
//
TInt CAknFepUICtrlPinyinPopup::GetLastResouce()
    {
    return iLastResouce;
    }

// ---------------------------------------------------------
// Set change state from Edit to Spelling flag.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinPopup::SetFromEditToSpellingFlag( TBool aFlag )
    {
    iChangeStateFromEditToSpelling = aFlag;
    }

// ---------------------------------------------------------
// Get change state from Edit to Spelling flag.
// ---------------------------------------------------------
//
TInt CAknFepUICtrlPinyinPopup::GetFromEditToSpellingFlag()
    {
    return iChangeStateFromEditToSpelling;
    }

// ---------------------------------------------------------
// Set valid keystroke change flag.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinPopup::SetValidKeystrokeChange( TBool aFlag )
    {
    iValidKeystrokeChanged = aFlag;
    }

// ---------------------------------------------------------
// Get valid keystroke change flag.
// ---------------------------------------------------------
//
TBool CAknFepUICtrlPinyinPopup::GetValidKeystrokeChange()
    {
    return iValidKeystrokeChanged;
    }

// ---------------------------------------------------------
// Set PreStateIsPredict flag.
// ---------------------------------------------------------
//
void CAknFepUICtrlPinyinPopup::SetPreStateIsPredictive( TBool aFlag )
    {
    iPreStateIsPredictive = aFlag;
    }

// ---------------------------------------------------------
// Set PreStateIsPredict flag.
// ---------------------------------------------------------
//
TBool CAknFepUICtrlPinyinPopup::GetPreStateIsPredictive()
    {
    return iPreStateIsPredictive;
    }

void CAknFepUICtrlPinyinPopup::SetInputLayoutMiniQwertyZhuyin()
    {
    for(TInt i = 0; i < EMaxPhraseCandidate; i++)
        {
        iInputPaneArray[i]->SetLayout( CAknFepUICtrlInputPane::ELayoutPhraseMiniQwertyZhuyin );
        }
    }

void CAknFepUICtrlPinyinPopup::ResetInputLayoutMiniQwertyZhuyin()
    {
    for(TInt i = 0; i < EMaxPhraseCandidate; i++)
        {
        iInputPaneArray[i]->SetLayout( CAknFepUICtrlInputPane::ELayoutPhrasePinyinPopupPane );
        }
    }
// End of file
