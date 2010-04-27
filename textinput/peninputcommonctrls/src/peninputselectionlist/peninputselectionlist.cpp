/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
*
*/
#include <barsread.h>
#include <coemain.h>
#include <AknsItemID.h>
#include <AknsUtils.h>
#include <AknUtils.h>
#include <peninputlayout.h>
#include <skinlayout.cdl.h>
#include <AknsDrawUtils.h>

#include "peninputselectionlist.h"

// constant definition
const TInt KInvalidResId = -1;
const TInt KInvalidBmp = -1;
const TInt KInvalidItemIndex = -1;

EXPORT_C CPenInputSelectionList* CPenInputSelectionList::NewL(CFepUiLayout* aUiLayout,
                                                              TInt aControlId,
                                                              TPoint aTopLeftPoint,
                                                              TSize aItemSize,
                                                              TInt aItemHorizontalMargin)
    {
    CPenInputSelectionList* self = new (ELeave) CPenInputSelectionList(aUiLayout,
                                                                       aControlId,
                                                                       aTopLeftPoint,
                                                                       aItemSize,
                                                                       aItemHorizontalMargin);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

EXPORT_C CPenInputSelectionList::~CPenInputSelectionList()
    {
    iItemList.Reset();
    iItemList.Close();

    iBmpList.ResetAndDestroy();
    iBmpList.Close();
    }

void CPenInputSelectionList::RecalculateCtrlRect()
    {
    if (iItemList.Count() == 0)
        {
        SetRect(TRect(0, 0, 0, 0));
        }
    else
        {
        SetRect(TRect(iItemList[0].iRect.iTl,
                iItemList[iItemList.Count() - 1].iRect.iBr));
        }
    }

EXPORT_C void CPenInputSelectionList::SizeChanged(TPoint aTopLeftPoint,
                                                  TSize aItemSize,
                                                  TInt aItemHorizontalMargin)
    {
    iTopLeftPoint = aTopLeftPoint;
    iItemSize = aItemSize;
    iItemHorizontalMargin = aItemHorizontalMargin;

    for (TInt i = 0; i < iItemList.Count(); i++)
        {
        iItemList[i].iRect = TRect(TPoint(aTopLeftPoint.iX,
                                          aTopLeftPoint.iY + 
                                          i * (aItemHorizontalMargin + iItemSize.iHeight)),
                                   aItemSize);
        }

    RecalculateCtrlRect();
    }

EXPORT_C void CPenInputSelectionList::SetItemsL(const RPointerArray<HBufC>& aItemList)
    {
    Hide(ETrue);

    iItemList.Reset();

    for (TInt i = 0; i < aItemList.Count(); i++)
        {
        SItem item(TRect(TPoint(iTopLeftPoint.iX,
                               iTopLeftPoint.iY + i * (iItemHorizontalMargin + iItemSize.iHeight)),
                         iItemSize),
                   *aItemList[i]);
        iItemList.AppendL(item);
        }

    RecalculateCtrlRect();
    }

EXPORT_C void CPenInputSelectionList::AddItemL(const TDesC& aItem)
    {
    SItem item(TRect(TPoint(iTopLeftPoint.iX,
                            iTopLeftPoint.iY + iItemList.Count() * (iItemHorizontalMargin + 
                            iItemSize.iHeight)),iItemSize), aItem);
    iItemList.AppendL(item);

    RecalculateCtrlRect();
    }

EXPORT_C void  CPenInputSelectionList::RemoveItem(TInt aItemIndex)
    {
    if (aItemIndex < 0 || aItemIndex >= iItemList.Count())
        {
        return;
        }
    
    iItemList.Remove(aItemIndex);
    
    for (TInt i = aItemIndex; i < iItemList.Count(); i++)
        {
        iItemList[i].iRect.Move(-iItemHorizontalMargin, 0);
        }
    
    RecalculateCtrlRect();
    }

EXPORT_C void CPenInputSelectionList::RemoveAllItems()
    {
    iItemList.Reset();
    RecalculateCtrlRect();
    }

EXPORT_C void CPenInputSelectionList::ConstructFromResourceL()
    {
    if (iResourceId == KInvalidResId)
    	{
    	User::Leave(KErrArgument);
    	}
   
    iBmpList.ResetAndDestroy();
    
    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC(reader, iResourceId);

    TPtrC bmpFileName = reader.ReadTPtrC();
    TInt imgMajorSkinId = reader.ReadInt32();
    
    TAknsItemID id;
    
    for (TInt i = 0; i < EBmpLastType + 1; i+=2)
        {
        TInt bmpid = reader.ReadInt16();
        TInt maskid = reader.ReadInt16();
        TInt minorskinid = reader.ReadInt16();

        id.Set(imgMajorSkinId, minorskinid);

        CFbsBitmap* bmp = NULL;
        CFbsBitmap* maskbmp = NULL;

        if (bmpid != KInvalidBmp)
            {
            if (maskid != KInvalidBmp)
                {
        		AknsUtils::CreateIconL(UiLayout()->SkinInstance(),
        		                       id,
        		                       bmp,
        		                       maskbmp,
        		                       bmpFileName,
        		                       bmpid,
        		                       maskid);

                AknIconUtils::SetSize(maskbmp, iItemSize, EAspectRatioNotPreserved);
                }
            else
                {
        	    AknsUtils::CreateIconL(UiLayout()->SkinInstance(),
        	                           id,
        	                           bmp,
        	                           bmpFileName,
        	                           bmpid);
                }

            AknIconUtils::SetSize(bmp, iItemSize, EAspectRatioNotPreserved);
            }

        iBmpList.AppendL(bmp);
        iBmpList.AppendL(maskbmp);
        }

    CleanupStack::PopAndDestroy(); // reader
    }

EXPORT_C void CPenInputSelectionList::SetTextProperty(const CFont* aFont,
                                                      TRgb aFontColor)
    {
    iFont = aFont;
    iFontColor = aFontColor;

    iBaselineOffset = iItemSize.iHeight/2 + aFont->AscentInPixels()/2;
    }

TInt CPenInputSelectionList::HitTest(const TPoint& aPoint)
    {
    if (!Rect().Contains(aPoint))
        {
        return KErrGeneral;
        }
        
    for (TInt i = 0; i < iItemList.Count(); i++)
        {
        if (iItemList[i].iRect.Contains(aPoint))
            {
            return i;
            }
        }
    
    return KErrNotFound;    
    }

EXPORT_C CFepUiBaseCtrl* CPenInputSelectionList::HandlePointerDownEventL(const TPoint& aPoint)
    {
    CFepUiBaseCtrl::HandlePointerDownEventL(aPoint);

    TInt index = HitTest(aPoint);
    
    if (index >= 0)
        {
        if (index != iFocusItemIndex)
            {
            TInt prev = iFocusItemIndex;
            iFocusItemIndex = index;
            
            if (prev != KInvalidItemIndex)
                {
                DrawItem(prev);
                }
            
            DrawItem(iFocusItemIndex);
            }
        }
    else if (index == KErrGeneral)
        {
        SetPointerDown(EFalse);
        CloseWindow();
        }
    
    return this;    
    }

EXPORT_C CFepUiBaseCtrl* CPenInputSelectionList::HandlePointerUpEventL(const TPoint& aPoint)
    {
    CFepUiBaseCtrl::HandlePointerUpEventL(aPoint);
    
    TInt index = HitTest(aPoint);
    
    if (index >= 0)
        {
        CloseWindow();
        
        if (iFocusItemIndex != KInvalidItemIndex)
            {
            TPtrC ptr;
            ptr.Set(reinterpret_cast<TText*>(&iFocusItemIndex), sizeof(TInt)/sizeof(TText));
            ReportEvent(iEventId, ptr);
            }
        }
    
    iFocusItemIndex = KInvalidItemIndex;

    return this;    
    }
    
EXPORT_C CFepUiBaseCtrl* CPenInputSelectionList::HandlePointerMoveEventL(const TPoint& aPoint)
    {
    CFepUiBaseCtrl::HandlePointerMoveEventL(aPoint);

    if (PointerDown())
        {
        TInt index = HitTest(aPoint);

        if (index != iFocusItemIndex && index != KInvalidItemIndex)
            {
            TInt prev = iFocusItemIndex;
            iFocusItemIndex = index;
        
            if( prev != -1 )
                {
                DrawItem(prev);
                }

            DrawItem(iFocusItemIndex);
            }
        }

    return this;
    }

EXPORT_C void CPenInputSelectionList::Display()
    {
    //Hide(EFalse);
    BringToTop();
    Draw();
    }

EXPORT_C void CPenInputSelectionList::CloseWindow()
    {
    Hide(ETrue);
    RemoveAllItems();
    }

CPenInputSelectionList::CPenInputSelectionList(CFepUiLayout* aUiLayout,
                                               TInt aControlId,
                                               TPoint aTopLeftPoint,
                                               TSize aItemSize,
                                               TInt aItemHorizontalMargin)
    :CFepUiBaseCtrl(TRect(0, 0, 0, 0), aUiLayout, aControlId),
     iTopLeftPoint(aTopLeftPoint),
     iItemSize(aItemSize),
     iItemHorizontalMargin(aItemHorizontalMargin),
     iFocusItemIndex(KInvalidItemIndex),
     iFrId(KAknsIIDQsnFrPopupSub),
     iCenterFrId(KAknsIIDQsnFrPopupCenterSubmenu)
    {
    SetControlType(EUserCtrlBase | ECtrlBaseCtrl);
    }

void CPenInputSelectionList::ConstructL()
    {
    BaseConstructL();
    Hide(ETrue);
    }

void CPenInputSelectionList::Draw()
    {
	if(!AbleToDraw())
        return;

    for (TInt i = 0; i < iItemList.Count(); i++)
        {
        DrawItem(i);
        }
        
    UiLayout()->UpdateArea(this, this->Rect(), EFalse);
    }

void CPenInputSelectionList::DrawItem(TInt aItemIndex)
    {
    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());        
    
    //mask bitmaps
    gc->Activate( MaskBitmapDevice() );

    gc->SetBrushStyle( CGraphicsContext::ESolidBrush );
    gc->SetBrushColor(TRgb(KOpaqueColor));//Non transparent at all
    gc->SetPenStyle(CGraphicsContext::ESolidPen );
    gc->SetPenSize(PenSize());
    gc->SetPenColor(TRgb(KOpaqueColor) );
    gc->DrawRect(iItemList[aItemIndex].iRect);

    // ----- draw bitmaps -----
    gc->Activate(BitmapDevice());   

	// draw frame
	MAknsSkinInstance* skin = UiLayout()->SkinInstance();

    TRect outerRect;
    TRect innerRect;
    CalculateFrameRects( iItemList[aItemIndex].iRect, outerRect, innerRect);

    AknsDrawUtils::DrawFrame(
	    skin, *gc, 
	    outerRect, innerRect, 
	    iFrId, iCenterFrId); 

    // draw text
    if (iTextLineSet)
        {
        TAknLayoutText textLayout;
        textLayout.LayoutText(iItemList[aItemIndex].iRect, 
                              iTextLine);
        textLayout.DrawText(*gc, iItemList[aItemIndex].iText);
        }
    else if (iFont)
        {
        gc->UseFont(iFont);

        gc->SetBrushStyle(CGraphicsContext::ENullBrush);
        gc->SetBrushColor(KRgbBlack);
        gc->SetPenColor(iFontColor);
        gc->SetPenStyle(CGraphicsContext::ESolidPen);
        gc->SetPenSize(PenSize());
        
        gc->DrawText(iItemList[aItemIndex].iText,
                     iItemList[aItemIndex].iRect,
                     iBaselineOffset,
                     CGraphicsContext::ECenter);

        gc->DiscardFont();
        }    
    }
void CPenInputSelectionList::CalculateFrameRects( const TRect aRect,
												  TRect& aOuterRect, 
												  TRect& aInnerRect ) const
    {
    TRect windowRect = aRect;
    
    TAknLayoutRect topLeft;
    topLeft.LayoutRect(windowRect, SkinLayout::Submenu_skin_placing_Line_2());

    TAknLayoutRect bottomRight;
    bottomRight.LayoutRect(windowRect, SkinLayout::Submenu_skin_placing_Line_5());

    aOuterRect = TRect(topLeft.Rect().iTl, bottomRight.Rect().iBr);
    aInnerRect = TRect(topLeft.Rect().iBr, bottomRight.Rect().iTl);
    }
// End Of File
