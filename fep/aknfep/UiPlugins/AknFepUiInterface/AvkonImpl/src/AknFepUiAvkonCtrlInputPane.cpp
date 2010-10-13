/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the CAknFepUICtrlInputPane methods.
*
*/












#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <uikon/eikdefmacros.h>
#endif
#include <gulfont.h> 
#include <eiklabel.h>
#include <AknUtils.h>
#include <AknsDrawUtils.h>
#include <skinlayout.cdl.h>

#include "AknFepUIAvkonCtrlInputPane.h"
#include "AknFepUIAvkonCtrlPinyinPopup.h"
#include "aknfepchineseuidataconv.h"
#include "aknfepuilayoutdatamgr.h"

CAknFepUICtrlInputPane* CAknFepUICtrlInputPane::NewL(RWindow& aParent, 
                                                     TInputPaneLayout aPaneLayout,
                                                     CAknFepUiLayoutDataMgr* aLafDataMgr )
    {
    CAknFepUICtrlInputPane* self = new(ELeave) CAknFepUICtrlInputPane(aPaneLayout,aLafDataMgr);
    CleanupStack::PushL(self);
    self->ConstructL(aParent);
    CleanupStack::Pop();
    return self;
    }

CAknFepUICtrlInputPane::~CAknFepUICtrlInputPane()
    {
    delete iBuffer;
    delete iLabel;
    delete iLabelToneMark;
    }

void CAknFepUICtrlInputPane::SetSizeChangedObserver(
    MAknFepUICtrlPinyinPopupSizeChangedObserver* aObserver)
    {
    iSizeChangedObserver = aObserver;
    }

void CAknFepUICtrlInputPane::SetText(const TDesC& aDes)
    {
    // make sure that we only copy as many characters as we can show
    if(iLayout == ELayoutPhraseInputPane)
        {
        TPtrC newText = aDes.Right(CalculateMaxTextLength(aDes));        
        TPtr ptr = iBuffer->Des();

        if(ptr != newText)
            {
            ptr.Copy(newText);
            UpdateLabelContents();

            if(iLayout == ELayoutPhraseInputPane) 
                {
                iSizeChangedObserver->SelectionPopupSizeChanged(CAknFepUICtrlPinyinPopup::EMaxCandidates + 1); // indicate input pane layout
                }
            DrawNow();
            }
        }
    else
        {
        TPtrC newText= aDes.Right(iMaxInputChars - EMaxToneMarkLength);
        TPtr ptr = iBuffer->Des();

        if(ptr != newText)
            {
            ptr.Copy(newText);
            UpdateLabelContents();

            if(iLayout == ELayoutPhraseInputPane) 
                {
                iSizeChangedObserver->SelectionPopupSizeChanged(CAknFepUICtrlPinyinPopup::EMaxCandidates + 1); // indicate input pane layout
                }
            DrawNow();
            }
        }
    }

void CAknFepUICtrlInputPane::GetText(TDes& aText) const
    {
    aText.Copy(*iBuffer);
    }

void CAknFepUICtrlInputPane::SetHighlighted(TBool aHighlighted)
    {
    if(!COMPARE_BOOLS(iHighlighted, aHighlighted))
        {
        iHighlighted = aHighlighted;
        // need to change highlight colors
        LayoutContainedControls();
        DrawDeferred();
        }
    }

TBool CAknFepUICtrlInputPane::Highlighted() const
    {
    return iHighlighted;
    }

void CAknFepUICtrlInputPane::SetToneMark(const TDesC& aToneMark)
    {
    TPtrC newText = aToneMark.Left(EMaxToneMarkLength);
    TPtr tonePtr = iToneMarkBuffer.Des();

    if(tonePtr != newText)
        {
        tonePtr.Copy(newText);
        UpdateLabelContents();
        if(iLayout == ELayoutPhraseInputPane)
            {
            iSizeChangedObserver->SelectionPopupSizeChanged(CAknFepUICtrlPinyinPopup::EMaxCandidates + 1);
            }
        DrawDeferred();
        }
    }

void CAknFepUICtrlInputPane::GetToneMark(TDes& aToneMark) const
    {
    TInt length = aToneMark.MaxLength();
    TPtrC toneMark = iToneMarkBuffer.Left(length);
    aToneMark.Copy(toneMark);
    }

void CAknFepUICtrlInputPane::SetToneMarkValidity(TBool aValid)
    {
    if(!COMPARE_BOOLS(iToneMarkValidity, aValid))
        {
        iToneMarkValidity = aValid;
        LayoutContainedControls();
        UpdateLabelContents();
        DrawDeferred();
        }
    }

void CAknFepUICtrlInputPane::SetLayoutIndex(TInt aIndex)
    {
    if(iLayoutIndex != aIndex)
        {
        iLayoutIndex = aIndex;
        LayoutContainedControls();
        }
    }

void CAknFepUICtrlInputPane::SetOverrideFontId(TInt aFontId)
    {
    if(iFontId != aFontId)
        {
        iFontId = aFontId;
        // need to update the layout, and then force the text to be formatted
        LayoutContainedControls();
        UpdateLabelContents();
        DrawDeferred();
        }
    }

void CAknFepUICtrlInputPane::MakeTextVisible(TBool aVisible)
    {
    if(aVisible)
        {
        UpdateLabelContents();
        }
    else
        {
        TRAP_IGNORE( 
            iLabel->SetTextL(KNullDesC);
            iLabelToneMark->SetTextL(KNullDesC);
            );
        }
    }

TInt CAknFepUICtrlInputPane::LabelTextWidthInPixels() const
    {
    TAknLayoutText layoutText = 
            CAknFepChineseUIDataConv::AnyToLayoutText( iLafDataMgr->RequestData( ELayoutInputPaneText ));
    const CFont* font = layoutText.Font();

    TInt labelWidthInPixels = 0;
    if(font)
        {
        TBufC<EMaxInputCharsInputPane> buf1;
        TPtr ptr1 = buf1.Des();
        GetText(ptr1);
        TBufC<EMaxToneMarkLength> buf2;
        TPtr ptr2 = buf2.Des();
        GetToneMark(ptr2);
        labelWidthInPixels = font->TextWidthInPixels(ptr1); 
        labelWidthInPixels += font->TextWidthInPixels(ptr2);
        }
    TInt maxWidth = layoutText.TextRect().Width(); 
    labelWidthInPixels = labelWidthInPixels > maxWidth? maxWidth:labelWidthInPixels;
    return labelWidthInPixels;
    }

void CAknFepUICtrlInputPane::SizeChanged()
    {
    LayoutContainedControls();
    }

TInt CAknFepUICtrlInputPane::CountComponentControls() const
    {
    CCoeControl* controls[] = 
        {
        iLabel,
        iLabelToneMark
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

CCoeControl* CAknFepUICtrlInputPane::ComponentControl(TInt aIndex) const 
    {
    CCoeControl* controls[] = 
        {
        iLabel,
        iLabelToneMark
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

void CAknFepUICtrlInputPane::Draw(const TRect& /*aRect*/) const
    {
    TBool skinnedDraw = EFalse;
    CWindowGc& gc = SystemGc();

    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    if(iHighlighted)
        {
        TRect outerRect;
        TRect innerRect;
        CalculateFrameRects(outerRect, innerRect);
        skinnedDraw = AknsDrawUtils::DrawFrame(
            skin, gc, outerRect, innerRect, KAknsIIDQsnFrList, KAknsIIDQsnFrListCenter); // or KAknsIIDDefault?
        }
    else
        {
        MAknsControlContext* cc = AknsDrawUtils::ControlContext(this);
        skinnedDraw = AknsDrawUtils::Background(skin, cc, this, gc, Rect());
        }

    if(!skinnedDraw)
        {
        if(iHighlighted)
            {
            iUnselectedStringHighlight.DrawRect(gc);
            }
        else
            {
            gc.SetBrushColor(AKN_LAF_COLOR_STATIC(0)); 
            gc.Clear(iUnselectedStringHighlight.Rect());
            }
        }
    }

void CAknFepUICtrlInputPane::ConstructL(RWindow& aParent)
    {
    SetContainerWindowL(aParent);
    iBuffer = HBufC::NewL(iMaxInputChars);

    // force label to contain required number of characters from the outset
    iLabel = new(ELeave) CEikLabel;
    iLabel->SetBufferReserveLengthL(iMaxInputChars); // this will do a realloc now, so no need to later
    iLabel->SetContainerWindowL(aParent);
    iLabel->SetMopParent(this);

    iLabelToneMark = new(ELeave) CEikLabel;
    iLabelToneMark->SetBufferReserveLengthL(EMaxToneMarkLength); // this will do a realloc now, so no need to later
    iLabelToneMark->SetContainerWindowL(aParent);
    iLabelToneMark->SetMopParent(this);

    }

CAknFepUICtrlInputPane::CAknFepUICtrlInputPane(TInputPaneLayout aPaneLayout, CAknFepUiLayoutDataMgr* aLafDataMgr)
    : 
    iHighlighted(EFalse),
    iLayout(aPaneLayout),
    iToneMarkValidity(ETrue),
    iFlags(0),
    iLafDataMgr(aLafDataMgr)
    {
    switch(iLayout)
        {
        case ELayoutPhraseInputPane:
            iMaxInputChars = EMaxInputCharsInputPane;
            break;
        case ELayoutPhrasePinyinPopupPane:
            iMaxInputChars = EMaxInputCharsPinyinPopup;
            break;
        default:
            iMaxInputChars = EMaxInputCharsInputPane;
            break;
        };
    }

void CAknFepUICtrlInputPane::UpdateLabelContents()
    {
    // add on the tone mark for the purposes of clipping
    TBufC<EMaxInputChars> combinedBuffer;
    TPtr combinedPtr = combinedBuffer.Des();
    combinedPtr.Copy(*iBuffer);
    combinedPtr.Append(iToneMarkBuffer);

    TRect rect = Rect();
    if(iLayout == ELayoutPhrasePinyinPopupPane)
        {
        TAknLayoutRect layoutLineRectEntryPane = 
                CAknFepChineseUIDataConv::AnyToLayoutRect( iLafDataMgr->RequestData( ELayoutLineRectEntryPane ));
        rect = layoutLineRectEntryPane.Rect();
        }
    // avoid accessing text layout member data
    TAknLayoutRect labelRect;
    
    TAknWindowLineLayout layoutEntryItem = 
            CAknFepChineseUIDataConv::AnyToWindowLine( iLafDataMgr->RequestData(ELayoutEntryItem));
    labelRect.LayoutRect(rect, layoutEntryItem);
    TAknLayoutText layoutText;
    TAknTextLineLayout entryTextLine = 
        CAknFepChineseUIDataConv::AnyToTextLine( iLafDataMgr->RequestData(EEntryTextLayout));
    layoutText.LayoutText(labelRect.Rect(),entryTextLine);
    TInt maxWidth = layoutText.TextRect().Width(); 
    
    TAknLayoutText layoutText1 = 
            CAknFepChineseUIDataConv::AnyToLayoutText( iLafDataMgr->RequestData( ELayoutInputPaneText ));
    const CFont* font = layoutText1.Font();

    TInt labelWidthInPixels = 0;
    if(font)
        {
        TBufC<EMaxInputCharsInputPane> buf1;
        TPtr ptr1 = buf1.Des();
        GetText(ptr1);
        TBufC<EMaxToneMarkLength> buf2;
        TPtr ptr2 = buf2.Des();
        GetToneMark(ptr2);
        labelWidthInPixels = font->TextWidthInPixels(ptr1); 
        labelWidthInPixels += font->TextWidthInPixels(ptr2);
        }
    
    if(labelWidthInPixels > maxWidth)
		{
		AknTextUtils::ClipToFit(
		combinedPtr,
		*(iLabel->Font()),
		maxWidth,
		AknTextUtils::EClipFromBeginning, 
		KDefaultClipWidth, 
		KNullDesC); 
		}

    // now remove the tone mark again, as it has a separate label
    TInt uncombinedLength = combinedPtr.Length() - iToneMarkBuffer.Length();
    if(uncombinedLength < 0)
        uncombinedLength = 0;
    combinedPtr.SetLength(uncombinedLength); // always shorter, so won't be bigger than max

    // ignore any errors... we have guaranteed that the descriptor already has sufficient characters to avoid a realloc
    TRAP_IGNORE( 
        iLabel->SetTextL(combinedPtr);
        iLabelToneMark->SetTextL(iToneMarkBuffer);
        );
    }

void CAknFepUICtrlInputPane::LayoutContainedControls()
    {
    TRect rect = Rect();
    if(iLayout == ELayoutPhraseInputPane)
		{
	    TAknWindowLineLayout layoutEntryItem = 
	            CAknFepChineseUIDataConv::AnyToWindowLine( iLafDataMgr->RequestData(ELayoutEntryItem));
        iUnselectedStringHighlight.LayoutRect(rect, layoutEntryItem);
        }
    else if(iLayout == ELayoutPhrasePinyinPopupPane) 
		{
        iUnselectedStringHighlight.LayoutRect(rect, AKN_LAYOUT_WINDOW_Input_highlight_Line_1(rect));
        }

    // layout spelling label    
    TAknTextLineLayout textLayout = 
        CAknFepChineseUIDataConv::AnyToTextLine( iLafDataMgr->RequestData(EEntryTextLayout));
    TAknLayoutText layoutText;
    TRect rectHighlight = iUnselectedStringHighlight.Rect();
    TRect textRect;
    
    layoutText.LayoutText(rectHighlight,textLayout);
    textRect = layoutText.TextRect();
    TRgb labelTextColor = layoutText.Color();

    TInt labelWidth = LabelTextWidthInPixels();
    if(textRect.Width() > labelWidth)
        {
        textRect.SetWidth(labelWidth);
        }
    iLabel->SetRect(textRect);
    const CFont* font = layoutText.Font();
    iLabel->SetFont(font);
    
    // layout tone mark label
    TRgb labelToneMarkTextColor = labelTextColor;
    TRect toneMarkRect;
    TPtrC ptr = *iBuffer;
    TInt toneMarkOffset = 0;

    toneMarkOffset = iLabel->CalcMinimumSize(ptr).iWidth;
    toneMarkRect.iTl.iX = textRect.iTl.iX + toneMarkOffset;
    toneMarkRect.iTl.iY = textRect.iTl.iY;
    toneMarkRect.iBr.iY = textRect.iBr.iY;
    toneMarkRect.SetWidth(labelWidth - toneMarkOffset);
    iLabelToneMark->SetRect(toneMarkRect);
    iLabelToneMark->SetFont(font);
    
    // once we have layed out the labels from LAF, override the colours from the skin if necessary
    // as the skin utils will only return RGB values, not colour ids.
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    TInt colorIndex = EAknsCIQsnTextColorsCG19;
    if (iHighlighted)
        {
        colorIndex = EAknsCIQsnTextColorsCG11;
        }
            
    AknsUtils::GetCachedColor(skin, labelTextColor, KAknsIIDQsnTextColors, colorIndex);

    colorIndex = iToneMarkValidity ? EAknsCIQsnTextColorsCG10: EAknsCIQsnTextColorsCG11; 
    AknsUtils::GetCachedColor(skin, labelToneMarkTextColor, KAknsIIDQsnTextColors, colorIndex);

    TRgb tonemarkColor;
    
    if (iToneMarkValidity)
    	{
    	tonemarkColor = labelToneMarkTextColor;
    	}
    else
    	{
    	tonemarkColor = KRgbGray;	
    	}	

    TRAP_IGNORE( 
        iLabel->OverrideColorL(EColorLabelText, labelTextColor);
        iLabelToneMark->OverrideColorL(EColorLabelText, tonemarkColor);
        );    
    }

void CAknFepUICtrlInputPane::CalculateFrameRects(TRect& aOuterRect, TRect& aInnerRect) const
    {
    TRect windowRect = Rect();
    TRect entryRect = 
        CAknFepChineseUIDataConv::AnyToRect( iLafDataMgr->RequestData(EEntryPaneRect));
    
    windowRect.SetHeight(entryRect.iBr.iY - entryRect.iTl.iY);
    
    TAknLayoutRect topLeft;
    topLeft.LayoutRect(windowRect,SkinLayout::Submenu_skin_placing_Line_2());

    TAknLayoutRect bottomRight;
    bottomRight.LayoutRect(windowRect, SkinLayout::Submenu_skin_placing_Line_5());

    aOuterRect = TRect(topLeft.Rect().iTl, bottomRight.Rect().iBr);
    aInnerRect = TRect(topLeft.Rect().iBr, bottomRight.Rect().iTl);
    if (aInnerRect.Width() <= 4)
        {
        aInnerRect.SetWidth(5);
        }
    }

TInt CAknFepUICtrlInputPane::CalculateMaxTextLength(const TDesC& aDes) const
    {
    TAknLayoutText layoutText = 
            CAknFepChineseUIDataConv::AnyToLayoutText( iLafDataMgr->RequestData( ELayoutInputPaneText ));
    TInt maxWidth = layoutText.TextRect().Width();
    TInt textWidth = 0;
    
    textWidth = aDes.Length();
    
    while (maxWidth < CalculateTextLength(aDes.Right(textWidth)))
        {
        textWidth--;
        }
    
    return textWidth;
    }

TInt CAknFepUICtrlInputPane::CalculateTextLength(const TDesC& aDes) const
    {
    TAknLayoutText layoutText = 
            CAknFepChineseUIDataConv::AnyToLayoutText( iLafDataMgr->RequestData( ELayoutInputPaneText ));
    const CFont* font = layoutText.Font();

    TInt labelWidthInPixels = 0;
    if(font)
        {
        labelWidthInPixels = font->TextWidthInPixels(aDes); 
        }
        
    return labelWidthInPixels;
    }

void CAknFepUICtrlInputPane::SetLayout( TInputPaneLayout aPaneLayout )
    {
    switch(aPaneLayout)
        {
        case ELayoutPhraseInputPane:
            iMaxInputChars = EMaxInputCharsInputPane;
            break;
        case ELayoutPhrasePinyinPopupPane:
            iMaxInputChars = EMaxInputCharsPinyinPopup;
            break;
        case ELayoutPhraseMiniQwertyZhuyin:
            iMaxInputChars = EMaxInputCharsZhuyinPopup;
            break;
        default:
            iMaxInputChars = EMaxInputCharsInputPane;
            break;
        };
    }
    
// End of file
