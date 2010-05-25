/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for button base and dragbar
*
*/

#include <coemain.h>
#include <barsread.h>
#include <AknsUtils.h>
#include <AknIconUtils.h>
#include <AknsDrawUtils.h>
#include <AknUtils.h>
#include <peninputlayout.h>
#include <AknBidiTextUtils.h>
#include <peninputcommonbutton.h>
#include <StringLoader.h>

#include "peninputscrollablelist.h"

const TInt KCtrlIdPageUp = 1;
const TInt KCtrlIdPageDown = 2;

const TInt KInvalidResId = -1;
const TInt KInvalidBmp = -1;


const TInt KDisplayTextLen = KMaxItemTextLength + KAknBidiExtraSpacePerLine;

EXPORT_C CFepLayoutScrollableList* CFepLayoutScrollableList::NewL(CFepUiLayout* aUiLayout,
                                                                  TInt aControlId,
                                                                  TSize aItemSize,
                                                                  TInt aHorizontalMargin,
                                                                  TInt aVerticalMargin,
                                                                  TSize aNaviSize,
                                                                  TSize aNaviInnerSize)
    {
    CFepLayoutScrollableList* self = NewLC(aUiLayout, 
                                           aControlId,
                                           aItemSize,
                                           aHorizontalMargin,
                                           aVerticalMargin,
                                           aNaviSize,
                                           aNaviInnerSize);
    CleanupStack::Pop(self);
    return self;
    }

EXPORT_C CFepLayoutScrollableList* CFepLayoutScrollableList::NewLC(CFepUiLayout* aUiLayout,
                                                                   TInt aControlId,
                                                                   TSize aItemSize,
                                                                   TInt aHorizontalMargin,
                                                                   TInt aVerticalMargin,
                                                                   TSize aNaviSize,
                                                                   TSize aNaviInnerSize)
    {
    CFepLayoutScrollableList* self = new(ELeave) CFepLayoutScrollableList(aUiLayout, 
                                                                          aControlId,
                                                                          aItemSize,
                                                                          aHorizontalMargin,
                                                                          aVerticalMargin,
                                                                          aNaviSize,
                                                                          aNaviInnerSize);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

EXPORT_C CFepLayoutScrollableList::~CFepLayoutScrollableList()
    {
    //iBmpList.ResetAndDestroy();
    //iBmpList.Close();

    //delete iPageInfoFormat;
    delete iPageInfo;
    iItemRects.Close();
    delete iSeperationBmp;
    delete iSeperationMaskBmp;
    }

EXPORT_C void CFepLayoutScrollableList::ConstructFromResourceL()
    {
    if (iResourceId == KInvalidResId)
        {
        User::Leave(KErrArgument);
        }
	//delete the old bmp res
	delete iSeperationBmp;
	delete iSeperationMaskBmp;
	iSeperationBmp = NULL;
	iSeperationMaskBmp = NULL;

    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC(reader, iResourceId);
    
    TPtrC bmpFileName = reader.ReadTPtrC();
    TInt imgMajorSkinId = reader.ReadInt32();
    
    TInt32 pageUpId = reader.ReadInt32();
    TInt32 pageDownId = reader.ReadInt32();
    TAknsItemID id;
   
    //read seperation line res
    const TInt16 bmpId = reader.ReadInt16();
    const TInt16 bmpMskId = reader.ReadInt16();
    // read skin item id
    const TInt skinitemid = reader.ReadInt16();
    id.Set(TInt(imgMajorSkinId), skinitemid);
    
    if (bmpId != KInvalidBmp)
        {
    	if (bmpMskId != KInvalidBmp)
    		{
		    AknsUtils::CreateColorIconL( UiLayout()->SkinInstance(),
						         id,
						         KAknsIIDQsnIconColors,
						         EAknsCIQsnIconColorsCG30,
						         iSeperationBmp,
						         iSeperationMaskBmp,
						         bmpFileName,
						         bmpId,
						         bmpMskId,
						         AKN_LAF_COLOR( 0 ) );    		                     
            }
        else
            {
    	    AknsUtils::CreateIconL(UiLayout()->SkinInstance(),
    	                           id,
    	                           iSeperationBmp,
    	                           bmpFileName,
    	                           bmpId);
            }
        }
    CleanupStack::PopAndDestroy(1); // reader
    
    TResourceReader pageUpReader;
    CCoeEnv::Static()->CreateResourceReaderLC(pageUpReader, pageUpId);
    iPageUp->ConstructFromResourceL(pageUpReader);
    CleanupStack::PopAndDestroy(1); // pageUpReader, imgreader

    TResourceReader pageDownReader;
    CCoeEnv::Static()->CreateResourceReaderLC(pageDownReader, pageDownId);
    iPageDown->ConstructFromResourceL(pageDownReader);
    CleanupStack::PopAndDestroy(1); // pageDownReader, imgreader
    }

EXPORT_C void CFepLayoutScrollableList::SizeChanged(TSize aItemSize,
                                                    TInt aHorizontalMargin,
                                                    TInt aVerticalMargin,
                                                    TSize aNaviSize,
                                                    TSize aNaviInnerSize)
    {
    iItemSize = aItemSize;
    iHorizontalMargin = aHorizontalMargin;
    iVerticalMargin = aVerticalMargin;
    iNaviSize = aNaviSize;
    iNaviInnerSize = aNaviInnerSize;

    }

EXPORT_C void CFepLayoutScrollableList::SetTextProperty(const CFont* aFont,
                                                        TRgb aFontColor)
    {
    iFont = aFont;
    iFontColor = aFontColor;

    iBaselineOffset = iItemSize.iHeight/2 + aFont->AscentInPixels()/2;
    }

EXPORT_C void CFepLayoutScrollableList::Draw()
    {
    if (Hiden() || !Ready() || Rect().Size() == TSize(0,0))
        {
        return;
        }

    // draw background of whole control
    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());

    //mask bitmaps
    TRect rect = Rect();
    
  	DrawOpaqueMaskBackground();
  	
    TRect innerRect = rect;
    innerRect.Shrink(4, 4);

    // ----- draw bitmaps -----
    gc->Activate(BitmapDevice());
    gc->Clear(rect);

	AknsDrawUtils::DrawFrame(UiLayout()->SkinInstance(), 
	                         *gc, 
	                         rect, 
	                         innerRect,
	                         iBgFrId,
	                         KAknsIIDDefault);

    // draw item
    for (TInt i = iCurrentPage * iOnePageItemCnt; 
         i < (iCurrentPage + 1) * iOnePageItemCnt && i < ItemArray().Count();
         i++)
        {
        DrawItem(i);
        }

    // draw scroll area
    if (iNaviBtnShown)
        {
        iPageUp->Draw();
        iPageDown->Draw();
        DrawPageInfo();
        }   
    }

EXPORT_C void CFepLayoutScrollableList::DrawPageInfo()
    {
    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
    //TPtr ptr = iPageInfo->Des();
    //FORMATPAGEINFO(ptr, *iPageInfoFormat, iCurrentPage + 1, iTotalPages);
    TRAP_IGNORE(UpdatePageInfoL( iCurrentPage + 1, iTotalPages ));
	
    if( iPageInfoLineSet )
        {
        TRgb color( KRgbBlack );  // sane default for nonskinned case

        TAknLayoutText textLayout;
        textLayout.LayoutText(iScrollRect, iPageInfoTextLine );
	    TAknsQsnTextColorsIndex clrIndex;
	    clrIndex = EAknsCIQsnTextColorsCG20;

	    if ( AknsUtils::AvkonSkinEnabled() )
	        {
	        AknsUtils::GetCachedColor( UiLayout()->SkinInstance(),
	                               color, KAknsIIDQsnTextColors, clrIndex );
	        }

	    textLayout.DrawText(*gc, iPageInfo->Des(), EFalse, color);
        }
    else
        {
        gc->UseFont(iFont);

        gc->SetBrushStyle(CGraphicsContext::ENullBrush);
        gc->SetBrushColor(KRgbBlack);
        gc->SetPenColor(iFontColor);
        gc->SetPenStyle(CGraphicsContext::ESolidPen);
        gc->SetPenSize(PenSize());


        gc->DrawText(iPageInfo->Des(),
                     iPageInfoRect,
                     iBaselineOffset,
                     CGraphicsContext::ECenter);

        gc->DiscardFont();
        }
    }

EXPORT_C TInt CFepLayoutScrollableList::HitTest(const TPoint& aPoint)
    {
    if( !Contains( aPoint ) )
        {
        return EListOutside;
        }
    for (TInt i = 0; i < iItemRects.Count(); i++)
        {
        if (iItemRects[i].Contains(aPoint))
            {
            if ((iCurrentPage * iOnePageItemCnt + i) < ItemArray().Count())
                return (iCurrentPage * iOnePageItemCnt + i);
            else 
                break;
            }
        }

    return EListInsideNoneListItem;    
    }
    
void CFepLayoutScrollableList::ScrollPageUp()
    {
    if (iCurrentPage >= 0 && iCurrentPage <= (iTotalPages - 1))
        {
        if (iCurrentPage == 0)
        	{
        	iCurrentPage = iTotalPages - 1;
        	}
        else
        	{
            iCurrentPage--;
        	}
        UpdateNaviButtonStates();
        Draw();
        UpdateArea( Rect(), EFalse);    
        }
        
#ifdef RD_TACTILE_FEEDBACK        
	UpdateFeedbackArea();
#endif // RD_TACTILE_FEEDBACK		
    }

void CFepLayoutScrollableList::ScrollPageDown()
    {
    if (iCurrentPage >= 0 && iCurrentPage <= (iTotalPages - 1))
        {
        if (iCurrentPage == iTotalPages - 1)
        	{
        	iCurrentPage = 0;
        	}
        else
        	{
            iCurrentPage++;
        	}
        UpdateNaviButtonStates();
        Draw();
        UpdateArea( Rect(), EFalse);    
        }
        
#ifdef RD_TACTILE_FEEDBACK
	UpdateFeedbackArea();
#endif // RD_TACTILE_FEEDBACK		
    }

EXPORT_C void CFepLayoutScrollableList::UpdateNaviButtonStates()
    {
    iPageUp->SetDimmed( iTotalPages == 1 );
    iPageDown->SetDimmed(iTotalPages == 1 );
    }
    
EXPORT_C void CFepLayoutScrollableList::CalcPageInfo()
    {
    iCurrentPage = 0;
    iTotalPages = (ItemArray().Count() + iOnePageItemCnt - 1) / iOnePageItemCnt;
    UpdateNaviButtonStates();
    }

EXPORT_C void CFepLayoutScrollableList::CalculateItemWidth()
    {
	TBuf<KDisplayTextLen> buf;
	CFont::TMeasureTextInput::TFlags flg = CFont::TMeasureTextInput::EFVisualOrder;
	if(iAlign == CGraphicsContext::ERight)
		flg = CFont::TMeasureTextInput::EFVisualOrderRightToLeft;
	
	const CFont* font = iFont;
	if(iTextLineSet)
		{
		TAknLayoutText textLayout;
		textLayout.LayoutText(TRect(0, 0, 1, 1), iTextLine);
		font = textLayout.Font();
		}
	ASSERT(font);
	
    iItemSize.iWidth = iInitItemWidth;
    const RPointerArray<SItem>& allItems = ItemArray();
    for(TInt i = 0; i < allItems.Count(); ++i)
        {
		TInt itemWidth = AknBidiTextUtils::MeasureTextBoundsWidth(
				*font, allItems[i]->iText, flg);
        if(itemWidth > iItemSize.iWidth)
            iItemSize.iWidth = itemWidth;
        }
     
    TPixelsTwipsAndRotation ptScreenSize;             
    CCoeEnv::Static()->ScreenDevice()->
                            GetDefaultScreenSizeAndRotation(ptScreenSize); 
    // calculate the length exclude the item
	TInt naviWidth = iNaviBtnShown? iNaviSize.iWidth : 0;                      
	TInt len = 4*iHorizontalMargin + naviWidth;
    if( iItemSize.iWidth + len > ptScreenSize.iPixelSize.iWidth)    
        {
        iItemSize.iWidth = ptScreenSize.iPixelSize.iWidth - len;
        }
    }

EXPORT_C void CFepLayoutScrollableList::DrawItem(TInt aItemIndex, TBool aItemActive)
    {
    // aItemIndex is relative to whole item array
    TBool isSpecial = ((aItemIndex == ItemArray().Count() - 1) && iSpecialItemOn);
    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
    TRect itemRect = iItemRects[aItemIndex % iOnePageItemCnt];
    TBool isFocus = aItemIndex == iCurFocusItem;
    
    if ( isSpecial )
        {
	    //mask bitmaps
		gc->Activate(MaskBitmapDevice());
	    gc->SetBrushStyle(CGraphicsContext::ESolidBrush);
	    gc->SetBrushColor(TRgb(KOpaqueColor));//Non transparent at all
	    gc->SetPenStyle(CGraphicsContext::ESolidPen);
	    gc->SetPenSize(PenSize());
	    gc->SetPenColor(TRgb(KOpaqueColor));
	    gc->DrawRect(itemRect);	    
	    // ----- draw bitmaps -----
	    gc->Activate(BitmapDevice());        
        
        TRect sepRect = itemRect;
        sepRect.iBr.iY = sepRect.iTl.iY + 2;
        sepRect.Move( 0, -4 );
    	SetListSeperationRect( sepRect );
        // draw background frame of special item		
		if( iSeperationBmp )
			{
			TRect srcRect( TPoint( 0, 0 ), iSeperationBmp->SizeInPixels() );

			if( iSeperationMaskBmp )
				{
				gc->BitBltMasked( iSeperationRect.iTl, 
								  iSeperationBmp, 
								  srcRect,
								  iSeperationMaskBmp,
								  ETrue);
								  
				}
			else
				{
				gc->BitBlt( iSeperationRect.iTl,
							iSeperationBmp,
							srcRect );
				}
			}
        }
    if( isFocus )
    	{
        //mask bitmaps
        gc->Activate(MaskBitmapDevice());
        gc->SetBrushStyle(CGraphicsContext::ESolidBrush);
        gc->SetBrushColor(TRgb(KOpaqueColor));//Non transparent at all
        gc->SetPenStyle(CGraphicsContext::ESolidPen);
        gc->SetPenSize(PenSize());
        gc->SetPenColor(TRgb(KOpaqueColor));
        gc->DrawRect(itemRect);
        
        // ----- draw bitmaps -----
        gc->Activate(BitmapDevice());   

	      // draw frame
        MAknsSkinInstance* skin = UiLayout()->SkinInstance();
	    TRect innerRect = itemRect;
	    innerRect.Shrink(7, 7);
        AknsDrawUtils::DrawFrame(skin, 
                                 *gc, 
	                             itemRect,
	                             innerRect,
                                 aItemActive ? iItemActiveFrId : iItemFrId,
                                 KAknsIIDDefault); 
        }
    
    CFepLayoutChoiceList::SItem* item = ItemArray()[aItemIndex];
    const CFont* font = iFont;
    TRgb color = iFontColor;
    TInt baseLine = iBaselineOffset;
    
    // draw text
    if (iTextLineSet)
        {
        TAknLayoutText textLayout;
        textLayout.LayoutText(itemRect, iTextLine);
        
	    TAknsQsnTextColorsIndex clrIndex;
	    clrIndex = isFocus ? EAknsCIQsnTextColorsCG10 : EAknsCIQsnTextColorsCG20;

	    color = KRgbBlack;
	    if (AknsUtils::AvkonSkinEnabled())
	        AknsUtils::GetCachedColor( UiLayout()->SkinInstance(),
	                               color, KAknsIIDQsnTextColors, clrIndex );
	    font = textLayout.Font();
	    itemRect = textLayout.TextRect();
	    baseLine = itemRect.Height() / 2 + font->AscentInPixels() / 2;
        }

    if(font == NULL)
    	return;
    
	gc->UseFont(font);
	gc->SetBrushStyle(CGraphicsContext::ENullBrush);
	gc->SetBrushColor(KRgbBlack);
	gc->SetPenColor(color);
	gc->SetPenStyle(CGraphicsContext::ESolidPen);
	gc->SetPenSize(PenSize());

	TBuf<KDisplayTextLen> buf;
	if ( iAlign == CGraphicsContext::ERight )
	    {
        AknBidiTextUtils::ConvertToVisualAndClip(item->iText, buf, *iFont,
                                         itemRect.Width(), itemRect.Width(),
                                         AknBidiTextUtils::ERightToLeft );
	    }
	else
	    {
        AknBidiTextUtils::ConvertToVisualAndClip(item->iText, buf, *iFont,
                                         itemRect.Width(), itemRect.Width());
	    }
	gc->DrawText(buf, itemRect, baseLine, iAlign);
	gc->DiscardFont();
    }

EXPORT_C void CFepLayoutScrollableList::ConstructL()
    {
    CFepLayoutPopupWnd::ConstructL();
    
    //create page up/down buttons
    iPageUp = CAknFepCtrlCommonButton::NewL(
                                     UiLayout(), 
                                     KCtrlIdPageUp,
                                     KAknsIIDQsnFrButtonNormal,
                                     KAknsIIDQsnFrButtonPressed,
                                     KAknsIIDQsnFrButtonInactive);
    iPageUp->AddEventObserver( this );
    iPageDown = CAknFepCtrlCommonButton::NewL(
                                     UiLayout(), 
                                     KCtrlIdPageDown,
                                     KAknsIIDQsnFrButtonNormal,
                                     KAknsIIDQsnFrButtonPressed,
                                     KAknsIIDQsnFrButtonInactive);
    iPageDown->AddEventObserver( this );
    AddControlL( iPageUp );
    AddControlL( iPageDown );
    }

EXPORT_C CFepLayoutScrollableList::CFepLayoutScrollableList(CFepUiLayout* aUiLayout,
                                                   TInt aControlId,
                                                   TSize aItemSize,
                                                   TInt aHorizontalMargin,
                                                   TInt aVerticalMargin,
                                                   TSize aNaviSize,
                                                   TSize aNaviInnerSize)
    :CFepLayoutChoiceList(aUiLayout, aControlId),
     iItemSize(aItemSize),
     iNaviSize(aNaviSize),
     iNaviInnerSize(aNaviInnerSize),
     iHorizontalMargin(aHorizontalMargin),
     iVerticalMargin(aVerticalMargin),     
     iSpecialItemOn(ETrue),
     iNaviBtnShown( ETrue ),
     iOnePageItemCnt(4)
    {
    iCurFocusItem = -1;
    iInitItemWidth = aItemSize.iWidth;
    }

EXPORT_C void CFepLayoutScrollableList::ReCalcLayout()
    {
    CalculateItemWidth();
    CalcPageInfo();
    iContentRect = TRect(TPoint(), TSize(2*iHorizontalMargin + iItemSize.iWidth, 
                       (iOnePageItemCnt + 1)*iVerticalMargin + iOnePageItemCnt*iItemSize.iHeight));

    if (iNaviBtnShown)
        {
		TInt nWidthPager = 2 * iHorizontalMargin + iNaviSize.iWidth;
		TInt nXposPager = iContentRect.Width();
		if (iAlign == CGraphicsContext::ERight)
			{
		    iContentRect.Move(nWidthPager, 0);
			nXposPager = 0;
			}
		iScrollRect = TRect(TPoint(nXposPager, 0), TSize(nWidthPager, iContentRect.Height()));
        iPageUpRect = TRect(TPoint(iScrollRect.iTl.iX + iHorizontalMargin, 
                                   iScrollRect.iTl.iY + iVerticalMargin),
                            iNaviSize);
                            
        TPoint innerTl ( iPageUpRect.iTl.iX + ( iNaviSize.iWidth - iNaviInnerSize.iWidth ) /2,
                         iPageUpRect.iTl.iY + ( iNaviSize.iHeight - iNaviInnerSize.iHeight ) /2 );
        iPageUp->SizeChanged( iPageUpRect, TRect( innerTl, iNaviInnerSize), ETrue );
        
        iPageDownRect = TRect(TPoint(iPageUpRect.iTl.iX, 
                                     iScrollRect.iBr.iY - iVerticalMargin - iNaviSize.iHeight), 
                              iNaviSize);

        innerTl = TPoint(iPageDownRect.iTl.iX + ( iNaviSize.iWidth - iNaviInnerSize.iWidth ) /2,
                         iPageDownRect.iTl.iY + ( iNaviSize.iHeight - iNaviInnerSize.iHeight ) /2 );
        iPageDown->SizeChanged( iPageDownRect, TRect(innerTl, iNaviInnerSize), ETrue );
        
        iPageInfoRect = TRect(TPoint(iScrollRect.iTl.iX, 
                              iScrollRect.iTl.iY + iScrollRect.Height()/2 - iNaviSize.iHeight/2),
                              iNaviSize);
        }
    else
    	{
		iScrollRect = TRect(0, 0, 0, 0);
    	}
    
    iItemRects.Reset();
    for (TInt i = 0; i < iOnePageItemCnt; i++)
        {
        iItemRects.Append(TRect(TPoint(iContentRect.iTl.iX + iHorizontalMargin, 
									   iContentRect.iTl.iY + (i + 1)* iVerticalMargin + 
                                       i * iItemSize.iHeight) , 
                                       iItemSize));
        }
    
    TRect ctrlRect = iContentRect;
    if (!iScrollRect.IsEmpty())
    	ctrlRect.BoundingRect(iScrollRect);
    SetRect(ctrlRect);
    UpdateNaviButtonStates();
    }

EXPORT_C void CFepLayoutScrollableList::HandleControlEvent(TInt aEventType, 
                                                  CFepUiBaseCtrl* aCtrl,
                                                  const TDesC& aEventData)
    {
    if( aEventType ==  EEventButtonUp )
        {
        if( aCtrl == iPageUp )
            {
            ScrollPageUp();
            }
        else if( aCtrl == iPageDown )
            {
            ScrollPageDown();
            }
        }
    }

EXPORT_C void CFepLayoutScrollableList::UpdatePageInfoL( TInt aCurrentPage, TInt aTotalPage )
	{
	if( iPageInfoResID == 0 )
		return;
	
	if (iPageInfo)
	    {
        delete iPageInfo;
        iPageInfo = NULL;
	    }
	CArrayFix< TInt >* aInts = new( ELeave ) CArrayFixFlat< TInt >(2);
	CleanupStack::PushL(aInts);

	aInts->AppendL( aCurrentPage );
	aInts->AppendL( aTotalPage );
	iPageInfo = StringLoader::LoadL( iPageInfoResID, *aInts );
	
	CleanupStack::PopAndDestroy(aInts);
	}
    
EXPORT_C void CFepLayoutScrollableList::SetImgFrId(TAknsItemID aBgFrId, 
                                                 TAknsItemID aNaviFrId,
                                                 TAknsItemID aNaviActiveFrId,
                                                 TAknsItemID aNaviDimFrId,
                                                 TAknsItemID aItemFrId,
                                                 TAknsItemID aItemActiveFrId)
    {
    iBgFrId = aBgFrId;
    iItemFrId = aItemFrId;
    iItemActiveFrId = aItemActiveFrId;
    iPageUp->SetImageFrameId( aNaviFrId, aNaviActiveFrId, aNaviDimFrId );
    iPageDown->SetImageFrameId( aNaviFrId, aNaviActiveFrId, aNaviDimFrId );
    }

EXPORT_C void CFepLayoutScrollableList::Move(const TPoint& aOffset)
    {
    CFepLayoutChoiceList::Move(aOffset);
    iContentRect.Move( aOffset );
    for (TInt i = 0; i < iOnePageItemCnt && i < iItemRects.Count(); i++)
        {
        iItemRects[i].Move( aOffset );
        }     
    iScrollRect.Move( aOffset );
    iPageInfoRect.Move( aOffset );   
    iSeperationRect.Move( aOffset );   
    }
EXPORT_C void CFepLayoutScrollableList::SetListSeperationRect( const TRect& aRect )
	{
	iSeperationRect = aRect;
	if( iSeperationBmp )
		{
		AknIconUtils::SetSize(iSeperationBmp, aRect.Size(), EAspectRatioNotPreserved  );				
		}
	if( iSeperationMaskBmp )
		{
		AknIconUtils::SetSize(iSeperationMaskBmp, aRect.Size(), EAspectRatioNotPreserved  );					
		}
	}    
	

                                       
EXPORT_C void CFepLayoutScrollableList::Hide(TBool aFlag)
	{
	CFepLayoutPopupWnd::Hide(aFlag);
	
#ifdef RD_TACTILE_FEEDBACK
	if (!aFlag)
		{
		iLastPageItemRect = iRect;
		
		UpdateFeedbackArea();
		
		if (!iPageUp->IsDimmed())
			{
			UiLayout()->RegisterFeedbackArea(reinterpret_cast<TInt>(iPageUp),iPageUp->Rect(),iPageUp->TactileFeedbackType());
			}

		if (!iPageDown->IsDimmed())
			{
			UiLayout()->RegisterFeedbackArea(reinterpret_cast<TInt>(iPageDown),iPageDown->Rect(),iPageDown->TactileFeedbackType());	
			}
		}
	else
	    {
        if (!iPageUp->IsDimmed())
            {
            UiLayout()->DeRegisterFeedbackArea(reinterpret_cast<TInt>(iPageUp),iPageUp->Rect());
            }

        if (!iPageDown->IsDimmed())
            {
            UiLayout()->DeRegisterFeedbackArea(reinterpret_cast<TInt>(iPageDown),
                                               iPageDown->Rect());
            }
	    }
#endif // RD_TACTILE_FEEDBACK	
	}
	
#ifdef RD_TACTILE_FEEDBACK	
void CFepLayoutScrollableList::UpdateFeedbackArea()
	{
	// calculate total height of all aviliable items in current page
	TInt curPageItemCount = 0;
    for (TInt i = iCurrentPage * iOnePageItemCnt; 
         i < (iCurrentPage + 1) * iOnePageItemCnt && i < ItemArray().Count();
         i++)
        {
        curPageItemCount++;
        }
        
	TRect itemRect(TPoint(iItemRects[0].iTl), 
	     TSize(iItemRects[0].Width(), iItemRects[0].Height() * curPageItemCount));
	
	UiLayout()->ChangeFeedbackArea(reinterpret_cast<TInt>(this),itemRect,iLastPageItemRect);        
	iLastPageItemRect = itemRect;
	}
#endif // RD_TACTILE_FEEDBACK	
// End Of File
