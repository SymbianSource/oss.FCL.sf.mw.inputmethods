/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for PeninputLayoutControl
*
*/

//#include <e32base.h>
#include <AknIconUtils.h>
#include <w32std.h>
#include <AknsDrawUtils.h>
#include <eikenv.h>
#include <AknsConstants.h>
#include <AknUtils.h>
#include <featmgr.h>

#include "peninputlayoutvirtualkeyctrl.h"
#include "peninputlayoutvkb.h"
#include "peninputlayout.h"
#include "peninputpluginutils.h"
#include "peninputcmd.h"

_LIT( KKeyShiftCharacter, "\x2191");
_LIT( KKeyStarCharacter, "\x002a");

const TInt KDefaultKeyMargin = 10;

// ---------------------------------------------------------------------------
// CVirtualKeyCtrl::NewL
// Static factory creator
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CVirtualKeyCtrl* CVirtualKeyCtrl::NewL(CVirtualKeyboard* aKeyboard,
                                                        CVirtualKey* aKeyInfo)
    {
    if(!aKeyInfo || !aKeyboard)
        return 0;
    
    CVirtualKeyCtrl* keyCtrl = new(ELeave) CVirtualKeyCtrl(aKeyboard,aKeyInfo);
    CleanupStack::PushL(keyCtrl);
    keyCtrl->ConstructL();
    CleanupStack::Pop();    
    return keyCtrl;    
    }

// ---------------------------------------------------------------------------
// CVirtualKeyCtrl::CVirtualKeyCtrl
// constuctor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
CVirtualKeyCtrl::CVirtualKeyCtrl(CVirtualKeyboard* aKeyboard, CVirtualKey* aKeyInfo)
                                 :CFepUiBaseCtrl(aKeyInfo->Rect(), aKeyboard->UiLayout(),-1),
                                  iKeyInfo(aKeyInfo), iKeyboard(aKeyboard)
    {
    SetControlType(ECtrlVirtualKey);    
    //virtual key postion is kept as relative to key board, so we must change the 
    //key control's position relative to layout.
    Move(aKeyboard->Rect().iTl);
    
#ifdef RD_TACTILE_FEEDBACK     
    //register the key feedback area
    //UiLayout()->RegisterFeedbackArea(reinterpret_cast<TInt>(this),Rect());    
#endif // RD_TACTILE_FEEDBACK 
    
    aKeyInfo->iKeyCtrl = this;
    iScanCode = aKeyInfo->ScanCode();
    }

// ---------------------------------------------------------------------------
// CVirtualKeyCtrl::~CVirtualKeyCtrl
// destructor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
CVirtualKeyCtrl::~CVirtualKeyCtrl()
    {
    //Never uncomment the following statement. When this control is deleted,
    //iKeyInfo may be already invalid!!!
    //iKeyInfo->iKeyCtrl = 0;
    
#ifdef RD_TACTILE_FEEDBACK     
    UiLayout()->DeRegisterFeedbackArea(reinterpret_cast<TInt>(this),Rect());            
#endif // RD_TACTILE_FEEDBACK     
    }
    
// ---------------------------------------------------------------------------
// CVirtualKeyCtrl::ConsructL
// 2nd phase constuctor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//        
void CVirtualKeyCtrl::ConstructL()
    {
    BaseConstructL();
    
    //tap accuracy enhancement
    if ( FeatureManager::FeatureSupported( KFeatureIdFfCapacitiveDisplay ))
        {
        TMargins margins;
        UiLayout()->GetKeyExtResponseArea( margins );
        TPoint topLeftMargin( margins.iLeft, margins.iTop );
        TSize bottomRightMargin( margins.iRight, margins.iBottom );
        EnableExtResponseArea( ETrue, TRect( topLeftMargin,bottomRightMargin ) );
        }
    }

// ---------------------------------------------------------------------------
// CVirtualKeyCtrl::DrawKeyText
// Draw key text
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//        
void CVirtualKeyCtrl::DrawKeyText(CFbsBitGc* aGc)  
    {
    TBool textlineset = EFalse;

    // if text line set
    for (TInt i = 0; i <= EPosLast; i++)
        {
        if (iKeyboard->TextLineLayoutSet(TVirtualKeyTextPos(i)))
            {
            textlineset = ETrue;

            if (iKeyInfo->KeyUnicodes(TVirtualKeyTextPos(i)) != KNullDesC)
                {
                TAknLayoutText textLayout;
                textLayout.LayoutText(GetRect(), 
                                      iKeyboard->TextLineLayout(TVirtualKeyTextPos(i)));
				TRgb color( KRgbBlack );  // sane default for nonskinned case			    
			    if ( AknsUtils::AvkonSkinEnabled() )
			        {
			        AknsUtils::GetCachedColor( UiLayout()->SkinInstance(),
				                               color, 
				                               KAknsIIDQsnTextColors, 
				                               iKeyboard->KeyTextColorGroup() );
			        }
			    
			    // replace the shift text to shift icon			        
				if( iKeyboard->ShiftIcon() &&
					iKeyInfo->KeyUnicodes(TVirtualKeyTextPos(i)) == KKeyShiftCharacter )
					{
					//CFbsBitGc* gc = GetGc();//static_cast<CFbsBitGc*>(BitGc());
					AknPenInputDrawUtils::DrawColorIcon( iKeyboard->ShiftIcon(),
														 *aGc,
														 textLayout.TextRect() );	
					}
				else if(iKeyboard->StarIcon() &&
						iKeyInfo->KeyUnicodes(TVirtualKeyTextPos(i)) == KKeyStarCharacter )
					{
					// Get the size of the icon
					TSize starIconSize = iKeyboard->StarIcon()->Bitmap()->SizeInPixels();
					// Get the rect of draw icon area
					TRect drawIconRect = textLayout.TextRect();
					// When the size of icon is different with the size of draw icon area,
					// because the icon is drew from the left top coordinate of the draw
					// icon area, so the icon will not be drew in the center. In this case,
					// we need to adjust the top left coordinate of draw icon rect to 
					// make sure that the icon will be drew in the center of the key
					if ( starIconSize != drawIconRect.Size())
                        {
						// Adjust the top left coordinate of draw icon rect to make sure
						// that the icon will be drew in the center of the key
						drawIconRect.iTl.iX += 
							( drawIconRect.Width() -  starIconSize.iWidth ) / 2;
						drawIconRect.iTl.iY += 
							( drawIconRect.Height() - starIconSize.iHeight ) / 2;
                        }
                
					// Draw the icon
					AknPenInputDrawUtils::DrawColorIcon( iKeyboard->StarIcon(),
														 *aGc,
														 drawIconRect );		
					}
				else				    
				    {
				    textLayout.DrawText(*aGc, iKeyInfo->KeyUnicodes(TVirtualKeyTextPos(i)), 
                                    EFalse, color);
				    }
                }
            }
        }

    if (!textlineset)
        {
		TAknLayoutText textLayout;
		textLayout.LayoutText(GetRect(), iKeyboard->iVKBTextLineLayout);
	    
	    //CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());    
	    
	    //charRect.Move(keyboardRect.iTl);
	    //if(iKeyInfo->Dimmed())
	    //    {        
	    //    gc->SetFadingParameters(iKeyboard->iFadingParamBlack, iKeyboard->iFadingParamWhite);
	    //    gc->SetFaded(ETrue);            
	    //    }
		TRgb color( KRgbBlack );  // sane default for nonskinned case			    
	    if ( AknsUtils::AvkonSkinEnabled() || 
	    	 iKeyboard->KeyTextColorGroup() != INVALID_COLOR_GROUP )
	        {
	        AknsUtils::GetCachedColor( UiLayout()->SkinInstance(),
		                               color, 
		                               KAknsIIDQsnTextColors, 
		                               iKeyboard->KeyTextColorGroup() );
	        }
	    else
    		{
			color = iKeyboard->iFontColor;
    		}
    		
    	if( iKeyInfo->Dimmed() )
			{			
			color = KRgbGray;	
			}	
    	if( iKeyInfo->DisplayUnicode() && iKeyInfo->DisplayUnicode()->Length() != 0)
    		{
    		textLayout.DrawText(*aGc, *iKeyInfo->DisplayUnicode(), EFalse, color);		
    		}
    	else
    		{
    		textLayout.DrawText(*aGc, iKeyInfo->KeyUnicodes(), EFalse, color);		
    		}		
    		
	    //if(iKeyInfo->Dimmed())
	    //    gc->SetFaded(EFalse);            
	    
    	}
    }

// ---------------------------------------------------------------------------
// CVirtualKeyCtrl::DrawDimKey
// Draw dimmed key
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//        
void CVirtualKeyCtrl::DrawDimKey()
    {
    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());    
    TRect rect = Rect();
    TRect bmpRect(TPoint(0,0),rect.Size());    
    gc->Activate( BitmapDevice() );
                
    
    if(iVirtualKeyDrawInfo.iVKDimLeftImgID.iMajor && 
       iVirtualKeyDrawInfo.iVKDimMiddleImgID.iMajor &&
       iVirtualKeyDrawInfo.iVKDimRightImgID.iMajor)
        {
        TRect innerRect = Rect();
		innerRect.Shrink( 4, 0 );
		    
		AknPenInputDrawUtils::Draw3PiecesFrame(UiLayout()->SkinInstance(),
								   *gc,
								   Rect(),
								   innerRect,
								   iVirtualKeyDrawInfo.iVKDimLeftImgID,
								   iVirtualKeyDrawInfo.iVKDimMiddleImgID,
								   iVirtualKeyDrawInfo.iVKDimRightImgID);
        }
    else if (iKeyboard->IsIrregularKeyBoard())
        {
        DrawIrregularKey(EKeyStateDimmed, EKeyStateDimmedMask);
        }
    else if (iKeyboard->KeySkinId(EKeyBmpDim) != KAknsIIDNone)
        {
        TRect innerrect = rect;
        innerrect.Shrink( KDefaultKeyMargin, KDefaultKeyMargin );
        
        AknsDrawUtils::DrawFrame(UiLayout()->SkinInstance(),
                                 *gc,
                                 rect,
                                 innerrect,
                                 iKeyboard->KeySkinId(EKeyBmpDim),
                                 KAknsIIDDefault);
        }
    else if( iKeyboard->NonIrregularKeyBitmap( EKeyBmpDim ) )
    	{
        TRect srcRect(TPoint(0,0), iKeyboard->NonIrregularKeyBitmap( EKeyBmpDim )->SizeInPixels());
        DrawBmp(rect,
                srcRect,
                iKeyboard->NonIrregularKeyBitmap( EKeyBmpDim ),
                iKeyboard->NonIrregularKeyBitmap( EkeyBmpDimMask ),
                ETrue); 
    	}
    else if (iKeyboard->iDimKeyBmp)
        {
        TRect srcRect(TPoint(0,0),iKeyboard->iDimKeyBmp->SizeInPixels());
        DrawBmp(rect,srcRect,iKeyboard->iDimKeyBmp,
                                            iKeyboard->iDimKeyMaskBmp,ETrue);
        }    

    else
        {
        gc->SetFaded(ETrue);

        //use if keboard has background bitmap            
        if (iKeyboard->BackgroundBmp())
            {
            TRect srcRect = Rect();
            srcRect.Move(-iKeyboard->Rect().iTl);
            DrawBmp(Rect(),srcRect,iKeyboard->BackgroundBmp(),iKeyboard->BkMaskBmp());
            }
        else
            {
            //self draw background if background bitmap
            gc->SetBrushColor( BkColor());
            gc->SetPenColor( BkColor() );
            gc->SetBrushStyle( CGraphicsContext::ESolidBrush );                
            gc->DrawRect(rect);		                      
            }
        }
        
    gc->SetPenColor(KRgbBlack);
    gc->SetBrushStyle( CGraphicsContext::ENullBrush );    
    //Draw text again.
    DrawKeyText(gc); 
    }

// ---------------------------------------------------------------------------
// CVirtualKeyCtrl::DrawBmp
// Draw bitmap
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//        
void CVirtualKeyCtrl::DrawBmp(const TRect& aDestRect,const TRect& aSourceRect,
                                  const CFbsBitmap* aBmp,CFbsBitmap* aMask,
                                  TBool aInvertFlag)
    {
    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
    DrawBmp(gc,aDestRect,aSourceRect,aBmp,aMask,aInvertFlag);					     
    }

// ---------------------------------------------------------------------------
// CVirtualKeyCtrl::DrawBmp
// Draw bitmap
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//        
void CVirtualKeyCtrl::DrawBmp(CFbsBitGc* aGc, const TRect& aDestRect,const TRect& aSourceRect,
                                  const CFbsBitmap* aBmp,CFbsBitmap* aMask,
                                  TBool aInvertFlag)
    {
    if(aMask)
        {
        //TRect bmpRect(TPoint(0,0),aMask->SizeInPixels());
        aGc->DrawBitmapMasked(aDestRect,aBmp,aSourceRect,aMask,aInvertFlag);        
        }
    else
        {
        aGc->DrawBitmap(aDestRect,aBmp,aSourceRect);
        }                            
    }
    
// ---------------------------------------------------------------------------
// CVirtualKeyCtrl::DrawNormalStateKey
// Draw key in normal state
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//            
void CVirtualKeyCtrl::DrawNormalStateKey()
    {    
    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
 
    TRect rect = Rect();    
    TRect bmpRect(TPoint(0,0),rect.Size());
    
    if(iVirtualKeyDrawInfo.iVKLeftImgID.iMajor && 
       iVirtualKeyDrawInfo.iVKMiddleImgID.iMajor &&
       iVirtualKeyDrawInfo.iVKRightImgID.iMajor)
        {
        TRect innerRect = Rect();
		innerRect.Shrink( 4, 0 );
		    
		AknPenInputDrawUtils::Draw3PiecesFrame(UiLayout()->SkinInstance(),
								   *gc,
								   Rect(),
								   innerRect,
								   iVirtualKeyDrawInfo.iVKLeftImgID,
								   iVirtualKeyDrawInfo.iVKMiddleImgID,
								   iVirtualKeyDrawInfo.iVKRightImgID);
        }
    else if (iKeyboard->IsIrregularKeyBoard())
        {
        DrawIrregularKey(EKeyStateNormal, EKeyStateNormalMask);
        }
    else if (iKeyboard->KeySkinId(EKeyBmpNormal) != KAknsIIDNone)
        {
        TRect innerrect = rect;
        innerrect.Shrink( KDefaultKeyMargin, KDefaultKeyMargin );
        TBool bHasDrawn = EFalse;
        if(UiLayout()->NotDrawToLayoutDevice())
            {
            /*TBool ret = iKeyboard->PrepareKeyBmp(iKeyboard->NormalKeyBmp(),
                                    iKeyboard->NormalKeyDev(),
                                    rect,innerrect,
                                    iKeyboard->KeySkinId(EKeyBmpNormal), 
                                    KAknsIIDDefault,Rect());*/
            TBool ret = iKeyboard->PrepareNormalKeyBmp(rect,innerrect,Rect());                                    
            if(ret)
                {
                gc->BitBlt(rect.iTl,iKeyboard->NormalKeyBmp());
                bHasDrawn = ETrue;
                }
            }
        
        if(!bHasDrawn)
            {
	        AknsDrawUtils::DrawFrame(UiLayout()->SkinInstance(), 
	                                 *gc, 
	                                 rect, 
	                                 innerrect,
	                                 iKeyboard->KeySkinId(EKeyBmpNormal), 
	                                 KAknsIIDDefault); 
            }
        }
    else if( iKeyboard->NonIrregularKeyBitmap( EKeyBmpNormal ) )
    	{
        TRect srcRect(TPoint(0,0), 
                      iKeyboard->NonIrregularKeyBitmap( EKeyBmpNormal )->SizeInPixels());
        DrawBmp(rect,
                srcRect,
                iKeyboard->NonIrregularKeyBitmap( EKeyBmpNormal ),
                iKeyboard->NonIrregularKeyBitmap( EKeyBmpNormalMask ),
                ETrue); 
    	}
     
    else
        {
        gc->SetFaded(EFalse);

        //use if keboard has background bitmap            
        if (iKeyboard->BackgroundBmp())
            {
            TRect srcRect = Rect();
            srcRect.Move(-iKeyboard->Rect().iTl);
            DrawBmp(Rect(),srcRect,iKeyboard->BackgroundBmp(),iKeyboard->BkMaskBmp());
            }
        else
            {
            //self draw background if background bitmap
            gc->SetBrushColor( BkColor());
            gc->SetPenColor( BkColor() );
            gc->SetBrushStyle( CGraphicsContext::ESolidBrush );                
            gc->DrawRect(rect);		                      
            }
        }        
        
    gc->SetPenColor( KRgbBlack );
    gc->SetBrushStyle( CGraphicsContext::ENullBrush );    
    gc->SetFaded(EFalse);            
    DrawKeyText(gc);        
    }

CFbsBitGc* CVirtualKeyCtrl::GetGc()
    {
    if(UiLayout()->NotDrawToLayoutDevice())
        {
        //draw to highligh bitmap
        if(Keyboard()->Bitmap()->SizeInPixels() != Rect().Size())
            {
            Keyboard()->Bitmap()->Resize(Rect().Size());
            Keyboard()->HighlightDev()->Resize( Rect().Size());
                //gc must be adjusted
            Keyboard()->HighlightGc()->Activate(Keyboard()->HighlightDev());
            Keyboard()->HighlightGc()->Resized();
            }
        return Keyboard()->HighlightGc();
        }
    else
        return static_cast<CFbsBitGc*>(BitGc());
    }

// ---------------------------------------------------------------------------
// CVirtualKeyCtrl::DrawHighlightKey
// Draw key in highlight state
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//                
void CVirtualKeyCtrl::DrawHighlightKey()
    {    
    CFbsBitGc* gc = GetGc();
    if(UiLayout()->NotDrawToLayoutDevice())
        gc->Activate( Keyboard()->HighlightDev() );
    TRect rect = GetRect();

    TPoint bmpPos = rect.iTl;
    TRect bmpRect(TPoint(0,0),rect.Size());
   
    if(iVirtualKeyDrawInfo.iVKPressedLeftImgID.iMajor && 
       iVirtualKeyDrawInfo.iVKPressedMiddleImgID.iMajor &&
       iVirtualKeyDrawInfo.iVKPressedRightImgID.iMajor)
        {
        TRect innerRect = GetRect();
		innerRect.Shrink( 4, 0 );
		    
		AknPenInputDrawUtils::Draw3PiecesFrame(UiLayout()->SkinInstance(),
								   *gc,
								   GetRect(),
								   innerRect,
								   iVirtualKeyDrawInfo.iVKPressedLeftImgID,
								   iVirtualKeyDrawInfo.iVKPressedMiddleImgID,
								   iVirtualKeyDrawInfo.iVKPressedRightImgID);

        }
    else if (iKeyboard->IsIrregularKeyBoard())
        {
        DrawIrregularKey(EKeyStatePressed, EKeyStatePressedMask);
        }

    //do we have highlight background bmp for this key?
    else if (iKeyboard->KeySkinId(EKeyBmpHighlight) != KAknsIIDNone)
        {
        TRect innerrect = rect;

        innerrect.Shrink( KDefaultKeyMargin, KDefaultKeyMargin );
        TBool bHasDrawn = EFalse;
        if(UiLayout()->NotDrawToLayoutDevice())
            {
            /*TBool ret = iKeyboard->PrepareKeyBmp(iKeyboard->HighightKeyBmp(),
                                    iKeyboard->HighlightKeyDev(),
                                    rect,innerrect,
                                    iKeyboard->KeySkinId(EKeyBmpHighlight), 
                                    KAknsIIDDefault,Rect());*/
            TBool ret = iKeyboard->PrepareHighlightKeyBmp(rect,innerrect,Rect());                                    
            if(ret)
                {
                gc->BitBlt(rect.iTl,iKeyboard->HighightKeyBmp());
                bHasDrawn = ETrue;
                }
            }
        
        if(!bHasDrawn)
            {        
	        AknsDrawUtils::DrawFrame(UiLayout()->SkinInstance(), 
	                                 *gc, 
	                                 rect, 
	                                 innerrect,
	                                 iKeyboard->KeySkinId(EKeyBmpHighlight), 
	                                 KAknsIIDDefault); 
            }
        }
    else if( iKeyboard->NonIrregularKeyBitmap( EKeyBmpHighlight ) )
    	{
        TRect srcRect(TPoint(0,0), 
                      iKeyboard->NonIrregularKeyBitmap( EKeyBmpHighlight )->SizeInPixels());
        DrawBmp(gc, rect,
                srcRect,
                iKeyboard->NonIrregularKeyBitmap( EKeyBmpHighlight ),
                iKeyboard->NonIrregularKeyBitmap( EKeyBmpHighlightMask ),
                ETrue); 
    	}
    else if(iKeyInfo->HighlightBmpIndex() >= 0 && 
                 iKeyInfo->HighlightBmpIndex() < iKeyboard->iKeyHighLightBmpList.Count())
        {
        gc->DrawBitmap(rect,iKeyboard->iKeyHighLightBmpList[iKeyInfo->HighlightBmpIndex()]);
        }
    else //using fading
        {
        gc->SetFadingParameters(iKeyboard->iFadingParamBlack, iKeyboard->iFadingParamWhite);
        gc->SetFaded(ETrue);            

        if(iKeyboard->BackgroundBmp())//use keyboard background bitmap
            {
            gc->BitBlt(bmpPos, iKeyboard->BackgroundBmp(), iKeyInfo->Rect());                
            }
        else //draw background if not background bitmap
            {              
            gc->SetBrushColor( BkColor());
            gc->SetPenColor( BkColor() );
            gc->SetBrushStyle( CGraphicsContext::ESolidBrush );                
            gc->DrawRect(rect);
            }
        }                    
 
    gc->SetPenColor( KRgbBlack );

    gc->SetBrushStyle( CGraphicsContext::ENullBrush );    
    gc->SetFaded(EFalse);       
    if(UiLayout()->NotDrawToLayoutDevice())
        {
		DrawKeyText(Keyboard()->HighlightGc());
        UpdateChangedArea(ETrue);
		}
    else
        {
		DrawKeyText(gc);
		}   

    }
    
// ---------------------------------------------------------------------------
// CVirtualKeyCtrl::Draw
// Draw key
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//                    
void CVirtualKeyCtrl::Draw()
    {
    if(!AbleToDraw())
    	return;    
    
    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
    
    //mask bitmaps
  	DrawOpaqueMaskBackground();  
        
    // ----- draw bitmaps -----
    gc->Activate( BitmapDevice() );                
    
/*    if (iScanCode == -1 && !iKeyInfo->Dimmed() && iKeyboard)
    	{
    	iKeyboard->DimKey(iKeyInfo, ETrue);
    	}*/

    if(iKeyInfo->Dimmed())
        {
        DrawDimKey();
        return;
        }
    
    if(iKeyInfo->Latched()||(PointerDown()&&!iKeyInfo->IsLatchKey()))
        {        
        DrawHighlightKey();
        
// remove DrawBubble() into HandlePointerDownEventL()
// to avoid showing more than one bubble at the same time.        
//        iKeyboard->DrawBubble(iKeyInfo);
        }
    else
        {                
        DrawNormalStateKey();
        }
    }
    
// ---------------------------------------------------------------------------
// CVirtualKeyCtrl::HandlePointerDownEventL
// Handle pointer down event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//                    
CFepUiBaseCtrl* CVirtualKeyCtrl::HandlePointerDownEventL(const TPoint& aPoint)
    {
    //do nothing if key is dimmed.
    if(iKeyInfo->Dimmed())
        return this;
    
    CFepUiBaseCtrl::HandlePointerDownEventL(aPoint);
    
    //draw key new state
    iKeyboard->DrawBubble(iKeyInfo);
    if(UiLayout()->NotDrawToLayoutDevice())
        {
        if( !(iKeyInfo->Dimmed()) && (iKeyInfo->Latched() || (PointerDown()&&!iKeyInfo->IsLatchKey())) )
            {        
            DrawHighlightKey();
            }
		else
		    {
			ReDraw();
			}	
        }
    else
        {
        ReDraw();
        UpdateArea( Rect() );
        }         	
    UpdateArea( Rect() );

    if(iKeyInfo->IsLatchKey())
        {
        // Only set latch status, not generate event for latch key
        iKeyInfo->SetLatched(!iKeyInfo->Latched());       
        }
    else
        {
        ReportKeyEvent(GetKeyEventType(EVirtualKeyControlDown));
        }     
 
    return this;
    }

// ---------------------------------------------------------------------------
// CVirtualKeyCtrl::HandlePointerUpEventL
// Handle pointer up event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CFepUiBaseCtrl* CVirtualKeyCtrl::HandlePointerUpEventL(const TPoint& aPoint)
    {
    if(!PointerDown() || iKeyInfo->Dimmed())
        {
        return this;
        }
    CFepUiBaseCtrl::HandlePointerUpEventL(aPoint);
    
    iKeyboard->ClearBubble(ETrue);
    
	TControlEvent  eventType = GetKeyEventType(EVirtualKeyControlUp);
    if(iKeyInfo->IsLatchKey())
        {
        // When key up, latch key will generate event. 
        // When key down, latch key won't generate event.
        eventType = iKeyInfo->Latched() ? EEventVirtualKeyLatched : EEventVirtualKeyUnLatched;
        }     
    if(UiLayout()->NotDrawToLayoutDevice())
        {
        //no need to draw again, just remove the highlight bitmap
        UpdateChangedArea(EFalse);
		Draw();
        }
    else
        {
        ReDraw();
        UpdateAreaImmed( Rect() );
        }
    ReportKeyEvent(eventType); 
    
    
    //Advanced tactile feedback REQ 417-47932
	#ifdef RD_TACTILE_FEEDBACK
    if (UiLayout()->SupportTactileFeedback())
    	{
   		UiLayout()->DoTactileFeedback(ETouchFeedbackSensitiveInput, ETrue, EFalse);
        }
	#endif // RD_TACTILE_FEEDBACK 
    return this;
    }
	
// ---------------------------------------------------------------------------
// CVirtualKeyCtrl::HandlePointerEnter
// Handle pointer enter event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CVirtualKeyCtrl::HandlePointerEnter(const TPoint& aPoint)
    {
    //do nothing if key is dimmed.
    if(iKeyInfo->Dimmed())
        return;
    
    CFepUiBaseCtrl::HandlePointerEnter(aPoint);
    if(iKeyInfo->IsLatchKey())
        {
        iKeyInfo->SetLatched(!iKeyInfo->Latched());       
        }
        
    iKeyboard->DrawBubble(iKeyInfo);    
    ReDraw();
    if(UiLayout()->NotDrawToLayoutDevice())
        {
        UpdateChangedArea(ETrue);
        }
    else
        UpdateArea( Rect() );
      
    }

// ---------------------------------------------------------------------------
// CVirtualKeyCtrl::HandlePointerLeave
// Handle pointer left event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CVirtualKeyCtrl::HandlePointerLeave(const TPoint& aPoint)
    {    
    //do nothing if key is dimmed.
    if(iKeyInfo->Dimmed())
        return;
    
    CFepUiBaseCtrl::HandlePointerLeave(aPoint);
    if(iKeyInfo->IsLatchKey())
        {
        iKeyInfo->SetLatched(!iKeyInfo->Latched());       
        }
        
    iKeyboard->ClearBubble(ETrue);
    if(UiLayout()->NotDrawToLayoutDevice())
        {
        UpdateChangedArea(EFalse);
        }
    else
        {
        ReDraw();
        UpdateArea( Rect() );
        }
	
#ifdef RD_TACTILE_FEEDBACK
    if ( iKeyboard->GowithPointerDown() )
		{
	    
		if (UiLayout()->SupportTactileFeedback())
			{
			if (!Hiden())
				{				
				UiLayout()->DoTactileFeedback(ETouchFeedbackCharacterInputButton, ETrue, EFalse);				
				}
			}
		}
#endif //RD_TACTILE_FEEDBACK
    }

void CVirtualKeyCtrl::CancelPointerDownL()
	{
	CFepUiBaseCtrl::CancelPointerDownL();
	iKeyboard->ClearBubble(ETrue);
    if(UiLayout()->NotDrawToLayoutDevice())
        {
        UpdateChangedArea(EFalse);
        }
    else
        {	
        ReDraw();
        UpdateArea( Rect() );
        }
	}
// ---------------------------------------------------------------------------
// CVirtualKeyCtrl::ReDraw
// Redraw and updates the key
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
void CVirtualKeyCtrl::ReDraw()
    {
    if(UiLayout()->NotDrawToLayoutDevice())
        {
        if(iKeyInfo->Latched()||(PointerDown()&&!iKeyInfo->IsLatchKey()))
            {        
            DrawHighlightKey();
            UpdateChangedArea(ETrue);
            }
		else
            {
			Draw();
			UpdateArea(Rect(), EFalse);
			}		
        }
    else
        {
        Draw();        
        UpdateArea(Rect(), EFalse);
        }    
    }
    
// ---------------------------------------------------------------------------
// CVirtualKeyCtrl::ReportKeyEvent
// Report current key event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//   
void CVirtualKeyCtrl::ReportKeyEvent(TControlEvent aEventType)
    {
    if (iKeyInfo->ScanCode() == 0xffffffff ||
        iKeyInfo->ScanCode() == 0xffff)
    	{
    	return;	
    	}

    TVirtualKeyEventData eventData;

    eventData.iKeyEvent.iCode = iKeyInfo->KeyCode();
    eventData.iKeyEvent.iScanCode = iKeyInfo->ScanCode();
    
    TInt unicodeidx;

    if (iKeyInfo->UnicodesListEmpty(unicodeidx))
        {
        eventData.iKeyData.Set(iKeyInfo->KeyUnicodes());
        }
    else
        {
        eventData.iKeyData.Set(iKeyInfo->KeyUnicodes(TVirtualKeyTextPos(unicodeidx)));
        }    
    eventData.iKey = iKeyInfo;
    TPtrC eventDataPtr;
    eventDataPtr.Set(reinterpret_cast<TUint16*>(&eventData),sizeof(eventData)/sizeof(TUint16));
    
    iKeyboard->ReportEvent(aEventType,eventDataPtr);	    
    }
    
// ---------------------------------------------------------------------------
// CVirtualKeyCtrl::GetKeyEventType
// Convert event type
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//       
TControlEvent CVirtualKeyCtrl::GetKeyEventType(TVirtualKeyEventType aEvent)
    {
    TControlEvent event = EEventVirtualKeyUp;
    switch(aEvent)
        {
        case EVirtualKeyControlUp:
            event = EEventVirtualKeyUp;
            break;
        case EVirtualKeyControlDown:
            event = EEventVirtualKeyDown;
            break;
        default:
            __ASSERT_DEBUG(EFalse, User::Panic(_L("peninputserver"), EUiLayoutBadEvent));    
        }
    return event;
    }

TBool CVirtualKeyCtrl::Contains(const TPoint& aPt)
    {
    if  (!CFepUiBaseCtrl::Contains(aPt))   // OR:  if (!iRect.Contains(aPt))
        {
        return EFalse;
        }
    
    if (iKeyboard->IsIrregularKeyBoard())
        {
        return MaskBmpContains(aPt);    
        }
        
    return ETrue; 
    }

TBool CVirtualKeyCtrl::MaskBmpContains(const TPoint& aPt)
    {
    // judge if mask bitmap contains a point
    TRgb ptColor;

    // map aPt to maskbmp's point
    TPoint ptInBmp = TPoint(aPt.iX - iRect.iTl.iX, aPt.iY - iRect.iTl.iY);

    CFbsBitmap* maskbmp = MaskBmp();
    if (!maskbmp)
        {
        return EFalse;
        }

       //  before following call, when mask bitmap created, you must call IMPORT_C static void DisableCompression( CFbsBitmap* aBitmap ); in akniconutils.h to make sure following call not crash
   maskbmp->GetPixel(ptColor, ptInBmp);    

    return (ptColor != KRgbBlack);
    }

CFbsBitmap* CVirtualKeyCtrl::MaskBmp()  // there may be other method to get mask bitmap
    {
    if (iKeyInfo->Dimmed())
        {
        return iKeyboard->IrregularKeyBitmap(iKeyInfo->IrregualrKeyType(),EKeyStateDimmedMask);
        }

    if (PointerDown() || iKeyInfo->Latched())
        {
        return iKeyboard->IrregularKeyBitmap(iKeyInfo->IrregualrKeyType(),EKeyStatePressedMask);
        }
    return iKeyboard->IrregularKeyBitmap(iKeyInfo->IrregualrKeyType(),EKeyStateNormalMask);
    }
 
 void CVirtualKeyCtrl::DrawIrregularKey(TVirtualKeyIrregularKeyState aKeyState, 
                                        TVirtualKeyIrregularKeyState aKeyStateMask)
    {
    TRect rect = Rect();
        
    if( rect.IsEmpty()	)    
        {
        return;
        }
    TSize bitmapSize = 
             iKeyboard->IrregularKeyBitmap(iKeyInfo->IrregualrKeyType(),aKeyState)->SizeInPixels();
    
    
    TRect srcRect(TPoint(0,0), bitmapSize);

    DrawBmp(rect,
            srcRect,
            iKeyboard->IrregularKeyBitmap(iKeyInfo->IrregualrKeyType(),aKeyState),
            iKeyboard->IrregularKeyBitmap(iKeyInfo->IrregualrKeyType(),aKeyStateMask),
            ETrue);

    }
 
void CVirtualKeyCtrl::SetVirtualKeyImgID(TVirtualKeyDrawInfo aVirtualKeyDrawInfo)
    {
    iVirtualKeyDrawInfo = aVirtualKeyDrawInfo;
    }
    
//implementation for class CVirtualRawKeyCtrl
// ---------------------------------------------------------------------------
// CVirtualRawKeyCtrl::NewL
// Factory function
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
CVirtualRawKeyCtrl* CVirtualRawKeyCtrl::NewL(CVirtualKeyboard* aKeyboard,
                                                          CVirtualKey* aKeyInfo)
    {
    if(!aKeyInfo)
        return 0;
    
    CVirtualRawKeyCtrl* keyCtrl = 
                            new(ELeave) CVirtualRawKeyCtrl(aKeyboard,aKeyInfo);
    CleanupStack::PushL(keyCtrl);
    keyCtrl->ConstructL();
    CleanupStack::Pop();    
    return keyCtrl;        
    }
   
// ---------------------------------------------------------------------------
// CVirtualRawKeyCtrl::CVirtualRawKeyCtrl
// Constructor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
CVirtualRawKeyCtrl::CVirtualRawKeyCtrl(CVirtualKeyboard* aKeyboard,
                                                        CVirtualKey* aKeyInfo)
                                      :CVirtualKeyCtrl(aKeyboard,aKeyInfo)
    {
    SetControlType(ECtrlVirtualItutKey);
    }
    
// ---------------------------------------------------------------------------
// CVirtualRawKeyCtrl::GetKeyEventType
// Convert key event type
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//         
TControlEvent CVirtualRawKeyCtrl::GetKeyEventType(TVirtualKeyEventType aEvent)
    {
    TControlEvent event = EEventVirtualKeyUp;
    switch(aEvent)
        {
            case EVirtualKeyControlUp:
                event = EEventRawKeyUpEvent;
                break;
            case EVirtualKeyControlDown:
                event = EEventRawKeyDownEvent;
                break;
            default:
                __ASSERT_DEBUG(EFalse, User::Panic(_L("peninputserver"), EUiLayoutBadEvent));    
        }
    return event;
    }
    
// ---------------------------------------------------------------------------
// CVirtualRawKeyCtrl::CancelPointerDownL
// Handle cancel pointer down event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//             
void CVirtualRawKeyCtrl::CancelPointerDownL()
    {
    CFepUiBaseCtrl::CancelPointerDownL();

    //simulates key up event
    ReportKeyEvent(EEventRawKeyUpEvent);
    }
    
// ---------------------------------------------------------------------------
// CVirtualRawKeyCtrl::HandlePointerLeave
// Handle pointer left event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CVirtualRawKeyCtrl::HandlePointerLeave(const TPoint& aPoint)
    {    
    if(UiLayout()->NotDrawToLayoutDevice())
        {
        UpdateChangedArea(EFalse);
        }
    TRawEvent event;
    event.Set(TRawEvent::EButton1Up,aPoint.iX,aPoint.iY);
    
    SimulateRawEvent(event);
    
	// Advanced tactile feedback REQ 417-47932
#ifdef RD_TACTILE_FEEDBACK
	if (UiLayout()->SupportTactileFeedback())
		{		
		if (!VirtualKey()->Dimmed() && !Hiden())
			{
			UiLayout()->DoTactileFeedback(ETouchFeedbackSensitiveInput, ETrue, EFalse);
			}
        }
#endif //RD_TACTILE_FEEDBACK     
	}

TRect CVirtualKeyCtrl::GetRect()
    {
    TRect r(Rect());
    if(UiLayout()->NotDrawToLayoutDevice())
        {
		if(iKeyInfo->Latched()||(PointerDown()&&!iKeyInfo->IsLatchKey()))
		    {
			r.Move(-r.iTl.iX, -r.iTl.iY);
			}
        }
    return r;
    }
	
void CVirtualKeyCtrl::UpdateChangedArea(TBool aFlag)
    {
    struct SData
        {
    	TUint32 ctrl;
        TBool flag;
        CFbsBitmap* bmp;
        TRect pos;
        } data;
        
    data.ctrl = (TUint32)this;
    data.flag = aFlag;
    data.bmp = Keyboard()->Bitmap();//aFlag ? Keyboard()->iBitmap : 0;
    data.pos = Rect();
    TPtrC ptr;
    ptr.Set(reinterpret_cast<const TUint16*>(&data),sizeof(data)/sizeof(TUint16));
    
    UiLayout()->SignalOwner(ESignalUpdateChangedArea,ptr);
    }
//end of file
