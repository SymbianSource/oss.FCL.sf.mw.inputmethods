/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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


#include <AknIconUtils.h>
#include <AknsUtils.h>
#include <AknUtils.h>
#include <AknsDrawUtils.h> 
#include "peninputlayout.h"

#include "peninputlayoutbubblectrl.h"
#include "peninputlayoutvirtualkeyctrl.h"
#include "peninputlayoutvkb.h"
#include "peninputpluginutils.h"

const TInt KFadingParamBlack = 128;
const TInt KFadingParamWhite = 192;
const TInt KDefaultBubbleSize = 50;

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CVirtualKeyboard

// ---------------------------------------------------------------------------
// CVirtualKeyboard::NewL
// Factory function
// ---------------------------------------------------------------------------
//
EXPORT_C CVirtualKeyboard* CVirtualKeyboard::NewL(const TRect& aRect, 
                                     CFepUiLayout* aUiLayout,TInt aControlId,
                                     const TFontSpec& aFontSpec,
                                     TBool aIrregular)
    {
    CVirtualKeyboard* vkb = new(ELeave) CVirtualKeyboard(aRect,aUiLayout,
                                                        aControlId,aFontSpec,
                                                        aIrregular);
    CleanupStack::PushL(vkb);
    vkb->ConstructL();
    vkb->SetRect(aRect);
    CleanupStack::Pop();
    return vkb;
    }

// ---------------------------------------------------------------------------
// CVirtualKeyboard::NewL
// Factory function
// ---------------------------------------------------------------------------
//
EXPORT_C CVirtualKeyboard* CVirtualKeyboard::NewL(const TRect& aRect, 
                                      CFepUiLayout* aUiLayout,TInt aControlId,
                                      CFont* aFont,
                                      TBool aIrregular)
    {
    TFontSpec fontSpec;
    fontSpec.iHeight = 0;
    CVirtualKeyboard* vkb = new(ELeave) CVirtualKeyboard(aRect,aUiLayout,
                                                        aControlId,fontSpec,
                                                        aIrregular);
    vkb->SetFont(aFont);                                                        
    CleanupStack::PushL(vkb);
    vkb->ConstructL();
    vkb->SetRect(aRect);
    CleanupStack::Pop();
    return vkb;
    }


// ---------------------------------------------------------------------------
// CVirtualKeyboard::CVirtualKeyboard
// Default constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CVirtualKeyboard::CVirtualKeyboard(const TRect& /*aRect*/,
                                CFepUiLayout* aUiLayout, TInt aControlId,
                                const TFontSpec& aFontSpec,
                                TBool aIrregular):
                                CControlGroup(aUiLayout,aControlId),
                                iFadingParamBlack(KFadingParamBlack),
                                iFadingParamWhite(KFadingParamWhite),
                                iKeyNormalSkinId(KAknsIIDNone),
                                iKeyHighlightSkinId(KAknsIIDNone),
                                iKeyDimSkinId(KAknsIIDNone),
                                iNeedDrawBackground(ETrue),
                                iIrregularKey(aIrregular),
                                iBubbleCtrl(NULL),
                                iBubbleVerticalMargin(0),                                
                                iKeyTextColorGroup(INVALID_COLOR_GROUP)
    {
    iFontSpec = aFontSpec;   
    SetControlType(ECtrlVirtualKeyboard);
    }
    
// ---------------------------------------------------------------------------
// CVirtualKeyboard::~CVirtualKeyboard
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CVirtualKeyboard::~CVirtualKeyboard()
    {
    if ( iBubbleCtrl )
        delete iBubbleCtrl;
    
    //free keys
    iKeyArray.ResetAndDestroy();
    iKeyArray.Close();
    
    //key highlight bitmap
    iKeyHighLightBmpList.ResetAndDestroy();
    iKeyHighLightBmpList.Close();
    
    iKeyHighLightMaskBmpList.ResetAndDestroy();
    iKeyHighLightMaskBmpList.Close();
    
    delete iDimKeyBmp;
    
    delete iDimKeyMaskBmp;
        
    //font
    if(iFontOwnership && iFont)
        {
        BitmapDevice()->ReleaseFont(iFont);
        iFont = NULL;    
        }
    iUnUsedRawKeyCtrlPool.ResetAndDestroy();
    iUnUsedRawKeyCtrlPool.Close();
    
    iUnUsedKeyCtrlPool.ResetAndDestroy();
    iUnUsedKeyCtrlPool.Close();    

    iTextLineLayout.Close();
    iTextLineSet.Close();
    
    iNonIrregularKeyBmpList.ResetAndDestroy();
    iNonIrregularKeyBmpList.Close();
    
    DestroyIrregularBmps();
    
    delete iShiftIcon;
    }

// ---------------------------------------------------------------------------
// CVirtualKeyboard::ConstructL
// 2nd phase constructor
// ---------------------------------------------------------------------------
//
EXPORT_C void CVirtualKeyboard::ConstructL ()
    {
    CControlGroup::ConstructL();
    if(BitmapDevice() && iFontSpec.iHeight)
        {
        if (KErrNone != BitmapDevice()->GetNearestFontInPixels(iFont,iFontSpec))
            {
            iFont = NULL;
            }
        else
            {
            iFontOwnership =  ETrue;
            }
        }

    for (TInt i = 0; i <= EPosLast; i++)
        {
        iTextLineLayout.AppendL(TAknTextLineLayout());
        iTextLineSet.AppendL(EFalse);
        }
        
    if (iIrregularKey)
        {
        ConstructIrregularBmps();    
        }        
    }

// ---------------------------------------------------------------------------
// CVirtualKeyboard::AddKeyL
// Add a key to key board
// ---------------------------------------------------------------------------
//
EXPORT_C void CVirtualKeyboard::AddKeyL(CVirtualKey* aKey)
    {
    if(aKey)
        {
        iKeyArray.AppendL(aKey);
        
        //create key control
        CVirtualKeyCtrl* ctrl = CreateKeyCtrlL(aKey,EFalse);//CVirtualKeyCtrl::NewL(this,aKey);
#ifdef RD_TACTILE_FEEDBACK 
        //Advanced Tactile feedback REQ417-47932
        ctrl->SetTactileFeedbackType(ETouchFeedbackSensitiveKeypad);
        UiLayout()->RegisterFeedbackArea(reinterpret_cast<TInt>(ctrl),ctrl->Rect(),ETouchFeedbackSensitiveKeypad);
#endif	  
        CleanupStack::PushL(ctrl);
        AddKeyControlL(ctrl);
        CleanupStack::Pop();
        if (aKey->ScanCode() == -1)
            {
            DimKey(aKey, ETrue);
            }
        }        
    }

// ---------------------------------------------------------------------------
// CVirtualKeyboard::AddRawKeyL
// Add a raw key to key board
// ---------------------------------------------------------------------------
//
EXPORT_C void CVirtualKeyboard::AddRawKeyL(CVirtualKey* aKey)
    {
    if(aKey)
        {
        iKeyArray.AppendL(aKey);
        
        //create key control
        //CVirtualRawKeyCtrl* ctrl = CVirtualRawKeyCtrl::NewL(this,aKey);
        CVirtualRawKeyCtrl* ctrl = static_cast<CVirtualRawKeyCtrl*>(CreateKeyCtrlL(aKey,ETrue));
#ifdef RD_TACTILE_FEEDBACK 
        //Advanced Tactile feedback REQ417-47932
        ctrl->SetTactileFeedbackType(ETouchFeedbackSensitiveKeypad);
        UiLayout()->RegisterFeedbackArea(reinterpret_cast<TInt>(ctrl),ctrl->Rect(),ETouchFeedbackSensitiveKeypad);            
        
#endif	        
        CleanupStack::PushL(ctrl);
        AddKeyControlL(ctrl);
        CleanupStack::Pop();
        if (aKey->ScanCode() == -1)
            {
            DimKey(aKey, ETrue);
            }        
        }        
    }

// ---------------------------------------------------------------------------
// CVirtualKeyboard::RemoveKey
// Remove a key from keyboard
// ---------------------------------------------------------------------------
//
EXPORT_C void CVirtualKeyboard::RemoveKey(CVirtualKey* aKey)
    {
    if(!aKey)
        return;
    TInt idx = iKeyArray.Find(aKey);
    if(KErrNotFound != idx)
        {				
        iKeyArray.Remove(idx);
		CVirtualKeyCtrl* ctrl = FindCtrlForKey(aKey);
		
#ifdef RD_TACTILE_FEEDBACK 
        UiLayout()->DeRegisterFeedbackArea(reinterpret_cast<TInt>(ctrl),ctrl->Rect());            
#endif		
        if(iActiveCtrl == ctrl)
            iActiveCtrl = 0;            
		
        RemoveKeyControl(ctrl);
        aKey->iKeyCtrl = 0;
        }
        
    }

// ---------------------------------------------------------------------------
// CVirtualKeyboard::FindCtrlForKey
// Find the key control which has the virtual key information.
// ---------------------------------------------------------------------------
//    
CVirtualKeyCtrl* CVirtualKeyboard::FindCtrlForKey(const CVirtualKey* aKey)
    { 
    for(TInt i = 0; i < ControlList().Count(); ++i)
        {
        CVirtualKeyCtrl* keyCtrl = static_cast<CVirtualKeyCtrl*>(ControlList()[i]);
        if(keyCtrl->iKeyInfo == aKey)
            return keyCtrl;
        }	      
    return 0;

    }
// ---------------------------------------------------------------------------
// CVirtualKeyboard::CreateKeyL
// Add a key to keyboard
// ---------------------------------------------------------------------------
//
EXPORT_C void CVirtualKeyboard::CreateKeyL(TUint aCode, TUint aScanCode, 
                    const TRect& aRect, const TRect& aDrawRect, TInt aBaseline,
                    TInt aHighlightBmpIdx,TInt aHighlightMaskBmpId)
    {
    CVirtualKey* key = CVirtualKey::NewL(aCode,aScanCode,aRect,aDrawRect,
                                aBaseline,aHighlightBmpIdx,aHighlightMaskBmpId);
    CleanupStack::PushL(key);
    AddKeyL(key);
    CleanupStack::Pop(key);  
    }

// ---------------------------------------------------------------------------
// CVirtualKeyboard::CreateKeyL
// Add a key to keyboard
// ---------------------------------------------------------------------------
//
EXPORT_C void CVirtualKeyboard::CreateKeyL(const TDesC& aCode, TUint aScanCode, 
                    const TRect& aRect, const TRect& aDrawRect, TInt aBaseline,
                    TInt aHighlightBmpIdx,TInt aHighlightMaskBmpId)
    {
    CVirtualKey* key = CVirtualKey::NewL(aCode,aScanCode,aRect,aDrawRect,
                                aBaseline,aHighlightBmpIdx,aHighlightMaskBmpId);
    CleanupStack::PushL(key);
    AddKeyL(key);    
    //key->SetKeyboard(this);  
    //key->iKeyboard = this;
    CleanupStack::Pop(key);  
    }


// ---------------------------------------------------------------------------
// CVirtualKeyboard::CreateRawKeyL
// Add a key to keyboard
// ---------------------------------------------------------------------------
//
EXPORT_C void CVirtualKeyboard::CreateRawKeyL(TUint aCode, TUint aScanCode, 
                    const TRect& aRect, const TRect& aDrawRect, TInt aBaseline,
                    TInt aHighlightBmpIdx,TInt aHighlightMaskBmpId)
    {
    CVirtualKey* key = CVirtualKey::NewL(aCode,aScanCode,aRect,aDrawRect,
                                aBaseline,aHighlightBmpIdx,aHighlightMaskBmpId);
    CleanupStack::PushL(key);
    AddRawKeyL(key);
    CleanupStack::Pop(key);  
    }

// ---------------------------------------------------------------------------
// CVirtualKeyboard::CreateRawKeyL
// Add a key to keyboard
// ---------------------------------------------------------------------------
//
EXPORT_C void CVirtualKeyboard::CreateRawKeyL(const TDesC& aCode, TUint aScanCode, 
                    const TRect& aRect, const TRect& aDrawRect, TInt aBaseline,
                    TInt aHighlightBmpIdx,TInt aHighlightMaskBmpId)
    {
    CVirtualKey* key = CVirtualKey::NewL(aCode,aScanCode,aRect,aDrawRect,
                                aBaseline,aHighlightBmpIdx,aHighlightMaskBmpId);
    CleanupStack::PushL(key);
    AddRawKeyL(key);    
    CleanupStack::Pop(key);
    }

// ---------------------------------------------------------------------------
// CVirtualKeyboard::CreateRawKeyL
// Add a key to keyboard
// ---------------------------------------------------------------------------
//
EXPORT_C void CVirtualKeyboard::CreateRawKeyL(RPointerArray<HBufC>& aUnicodesList,
                                              TInt aScanCode,
                                              const TRect& aRect)
    {
    CVirtualKey* key = CVirtualKey::NewL(aUnicodesList,
                                         aScanCode,
                                         aRect);
    CleanupStack::PushL(key);
    AddRawKeyL(key);    
    CleanupStack::Pop(key);
    }

// CVirtualKeyboard::Draw
// Draw virtual keyboard
// ---------------------------------------------------------------------------
// 
EXPORT_C void CVirtualKeyboard::Draw()
    {
    if(!AbleToDraw())
    	return;    
    const TRect& keyboardRect = Rect();

    if (iNeedDrawBackground)
        {
        CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());

        //mask bitmaps
      if (!iIrregularKey)
	      {
	      DrawOpaqueMaskBackground();  	
	      }

        // ----- draw bitmaps -----
        gc->Activate( BitmapDevice() );

        if(!BackgroundBmp())
            {
            // ----- draw bitmaps -----

            gc->SetBrushColor( BkColor());
            gc->SetPenColor( KRgbBlack );
            gc->SetPenStyle(CGraphicsContext::ESolidPen);
           if (!iIrregularKey)
      	      {
              gc->DrawRect(keyboardRect);
              }
            }
        else
            {
            // ----- draw bitmaps -----        
            if(BkMaskBmp())
                {
                TRect bmpRect(TPoint(0,0),BkMaskBmp()->SizeInPixels());
                if (!iIrregularKey)
	         	    {
	                gc->DrawBitmapMasked(keyboardRect,BackgroundBmp(),bmpRect,BkMaskBmp(),EFalse);
	                }
                }
            else        
                {
         	    if (!iIrregularKey)
	        	    {
	                gc->DrawBitmap(keyboardRect,BackgroundBmp());
	                }
                }
            }    
       
        gc->SetBrushStyle( CGraphicsContext::ENullBrush );       
        }
    
    //draw each key control
    for(TInt i = 0; i < ControlList().Count(); ++i)
        {
        CVirtualKeyCtrl* ctrl = static_cast<CVirtualKeyCtrl*>(ControlList()[i]);
        ctrl->Draw();
        }
    }
    
void CVirtualKeyboard::ClearBubble(TBool /*aSetZeroSize*/)
    {
    if ( iBubbleCtrl )
        {
        iBubbleCtrl->Close();
        }
    }

void CVirtualKeyboard::DrawBubble(CVirtualKey* aKey)
    {
    
    if ( aKey && !aKey->Dimmed() && iBubbleCtrl && iIsShowBubble)
        {
        TRect rect = iBubbleCtrl->Rect();
        TRect parentWndRect = Rect();       
        TInt x1 = aKey->Rect().iTl.iX - ( iBubbleSize.iWidth - aKey->Rect().Width() ) / 2;
        TInt x2 = aKey->Rect().iBr.iX + ( iBubbleSize.iWidth - aKey->Rect().Width() ) / 2;
        if ( x1 < 0 )
            {
            rect.iTl.iX = parentWndRect.iTl.iX;     
            rect.iBr.iX = iBubbleSize.iWidth; 
            }
        else if ( x2 > parentWndRect.iBr.iX )
            {
            rect.iTl.iX = parentWndRect.iBr.iX - iBubbleSize.iWidth;     
            rect.iBr.iX = parentWndRect.iBr.iX;                 
            }
        else
            {
            rect.iTl.iX = x1;     
            rect.iBr.iX = x2;                 
            }
        
        rect.iTl.iY = iBubbleVerticalMargin + aKey->Rect().iTl.iY - 
                      iBubbleSize.iHeight + Rect().iTl.iY;
        rect.iBr.iY = iBubbleVerticalMargin + aKey->Rect().iTl.iY + Rect().iTl.iY;
		
		if( aKey->DisplayUnicode() && aKey->DisplayUnicode()->Length() != 0 )
			{
			TRAP_IGNORE(iBubbleCtrl->SetTextL(*aKey->DisplayUnicode()));		
			}
        else
        	{
        	TRAP_IGNORE(iBubbleCtrl->SetTextL(aKey->KeyUnicodes()));			
        	}
        iBubbleCtrl->Popup(rect);
        }
    }

EXPORT_C void CVirtualKeyboard::ShowBubble(TBool aFlag)
    {  
    TBool flag = iBubbleCtrl ? ETrue : EFalse;
    iIsShowBubble = aFlag;
    if ( flag != aFlag )
        {
        if(!flag)
            {
            TRAP_IGNORE(iBubbleCtrl = CBubbleCtrl::NewL(TRect(0,0,0,0),UiLayout(),-0xFFFF));
            iBubbleSize = TSize(KDefaultBubbleSize,KDefaultBubbleSize);
            }
        }
    }
    
EXPORT_C TBool CVirtualKeyboard::GowithPointerDown()
    {
    return iGowithPointerDown;
    }

EXPORT_C void CVirtualKeyboard::SetGowithPointerDown(TBool aFlag)
    {
    iGowithPointerDown = aFlag;
    }
        
EXPORT_C void CVirtualKeyboard::SetBubbleBitmapParam(CFbsBitmap* aBmpId,
                                                     CFbsBitmap* aMaskBmpId ,
                                                     TAknsItemID aBgSkinId )
    {
    if ( iBubbleCtrl )
        {
        iBubbleCtrl->SetBitmapParam(aBmpId,aMaskBmpId,aBgSkinId);
        }
    }
        
EXPORT_C void CVirtualKeyboard::SetBubbleSize(const TSize& aSize)
    {
    iBubbleSize = aSize;
    }
    
EXPORT_C void CVirtualKeyboard::SetBubbleVerticalMargin(TInt aMargin)
    {
    iBubbleVerticalMargin = aMargin;    
    }

EXPORT_C void CVirtualKeyboard::SetTextFormat(TAknTextLineLayout aTextFormat)
    {
    if ( iBubbleCtrl )
        {
        iBubbleCtrl->SetTextFormat(aTextFormat);    
        }
    }

EXPORT_C void CVirtualKeyboard::SetFrameDiff(TInt aLeftDiff, TInt aTopDiff,
                                             TInt aRightDiff,TInt aBottomDiff)
    {
    if ( iBubbleCtrl )
        {
        iBubbleCtrl->SetFrameDiff(aLeftDiff,aTopDiff,aRightDiff,aBottomDiff);    
        }        
    }
       
EXPORT_C CFepUiBaseCtrl* CVirtualKeyboard::HandlePointerMoveEventL(const TPoint& aPt)
    {
    if(iWndControl)
        {
        return CFepUiBaseCtrl::HandlePointerMoveEventL(aPt);
        }
        
    if( !iGowithPointerDown )        
        {
        return CControlGroup::HandlePointerMoveEventL( aPt );
        }
        
    CFepUiBaseCtrl* ctrl = ControlForPoints(aPt);

    if( !iRect.Contains(aPt) ) // not belong our group
        return NULL; // Check: This will not happen!
    
    if( !PointerDown() )
        {
        return NULL;
        }
    if(ctrl != iCtrlWithPointerDown) 
        {
        if ( iBubbleCtrl && iIsShowBubble ) 
            {
            //iBubbleCtrl->Freeze();
            }
        if( iCtrlWithPointerDown )
            {
            iCtrlWithPointerDown->HandlePointerLeave(aPt);
            }
        
    	iCtrlWithPointerDown = ctrl;
    	
        if( ctrl )
            {
            iCtrlWithPointerDown->HandlePointerEnter(aPt);
            }
            
        if ( iBubbleCtrl && iIsShowBubble ) 
            {
            //iBubbleCtrl->UnFreeze();
            }
        }
    else
        {
        if(ctrl)
            ctrl->HandlePointerMoveEventL(aPt);
        }
    
    return this;
    }
	
// ---------------------------------------------------------------------------
// CVirtualKeyboard::DrawKey
// Draw a key
// ---------------------------------------------------------------------------
// 
void CVirtualKeyboard::DrawKey(const CVirtualKey* aKey)
    {
    if (Hiden())
        {
        return;
        }

    CVirtualKeyCtrl* keyCtrl = FindCtrlForKey(aKey);
    if(keyCtrl)
        {
        keyCtrl->Draw();
        }

    TRect rect(Rect().iTl+aKey->Rect().iTl,aKey->Rect().Size());

    UpdateArea(rect, EFalse);
    }
    
// ---------------------------------------------------------------------------
// CVirtualKeyboard::SetFont
// Set vkb font
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//        
EXPORT_C void CVirtualKeyboard::SetFont(const TFontSpec& aFontSpec)
    {
    iFontSpec = aFontSpec;
    //release old font
    if(iFontOwnership && iFont)
        {
        BitmapDevice()->ReleaseFont(iFont);
        iFont = NULL;    
        }    
    iFontOwnership = ETrue;    
    if(BitmapDevice())
        {
        if (KErrNone != BitmapDevice()->GetNearestFontInPixels(iFont,iFontSpec))
            iFont = NULL;
        }      
    }        

// ---------------------------------------------------------------------------
// CVirtualKeyboard::SetFont
// Set VKB font
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//        
EXPORT_C void CVirtualKeyboard::SetFont(const CFont* aFont)
    {
    if(iFontOwnership && iFont)
        {
        BitmapDevice()->ReleaseFont(iFont);
        }    
    iFontOwnership = EFalse;    
    iFont = const_cast<CFont*>(aFont);
    if(iFont)
        iFontSpec = iFont->FontSpecInTwips();
    }        


// ---------------------------------------------------------------------------
// CVirtualKeyboard::SetFontColor
// Set VKB key font color
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//        
EXPORT_C void CVirtualKeyboard::SetFontColor( const TRgb aFontColor )
    {    
    iFontColor = aFontColor;
    }   
    
// ---------------------------------------------------------------------------
// CVirtualKeyboard::AddKeyHighLightBitmapL
// Add a key highlight bmp
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CVirtualKeyboard::AddKeyHighLightBitmapL(CFbsBitmap* aBmp)
    {
    iKeyHighLightBmpList.AppendL(aBmp);
    }
    
// ---------------------------------------------------------------------------
// CVirtualKeyboard::AddKeyHighLightMaskBitmapL
// Add a key highlight mask bmp
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CVirtualKeyboard::AddKeyHighLightMaskBitmapL(CFbsBitmap* aBmp)
    {
    iKeyHighLightMaskBmpList.AppendL(aBmp);
    }    
  
// ---------------------------------------------------------------------------
// CVirtualKeyboard::RemoveKeyHighLightBitmap
// Remove a key highlight bmp
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CVirtualKeyboard::RemoveKeyHighLightBitmap(TInt aIndex)
    {
    __ASSERT_DEBUG(aIndex >= 0 && aIndex < iKeyHighLightBmpList.Count(), 
                   User::Panic(_L("peninputserver"), EUiGeneralIndexError));
    iKeyHighLightBmpList.Remove(aIndex);                    
    }
    	 
// ---------------------------------------------------------------------------
// CVirtualKeyboard::RemoveKeyHighLightMaskBitmap
// Remove a key highlight mask bmp
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CVirtualKeyboard::RemoveKeyHighLightMaskBitmap(TInt aIndex)
    {
    __ASSERT_DEBUG(aIndex >= 0 && aIndex < iKeyHighLightMaskBmpList.Count(), 
                   User::Panic(_L("peninputserver"), EUiGeneralIndexError));
    iKeyHighLightMaskBmpList.Remove(aIndex);                        
    }    
    
    
// ---------------------------------------------------------------------------
// CVirtualKeyboard::DimKey
// Dim a key
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//        
EXPORT_C void CVirtualKeyboard::DimKey(CVirtualKey* aKey, TBool aFlag)
    {
    if(!aKey)
        return;
    if(aFlag == aKey->Dimmed() )
        return;
    
    if (!aFlag && aKey->ScanCode() == -1)
        {
        return;
        }
    aKey->SetDimmed(aFlag);
    DrawKey(aKey);
    
#ifdef RD_TACTILE_FEEDBACK 
    CVirtualKeyCtrl* keyCtrl = FindCtrlForKey(aKey);
  
    if(keyCtrl->IsKindOfControl(ECtrlTactileFeedback))
        {        
        if(aFlag)
            UiLayout()->DeRegisterFeedbackArea(reinterpret_cast<TInt>(keyCtrl),keyCtrl->Rect());
        else
            UiLayout()->RegisterFeedbackArea(reinterpret_cast<TInt>(keyCtrl),keyCtrl->Rect(),keyCtrl->TactileFeedbackType());
        }
#endif
    }    

EXPORT_C void CVirtualKeyboard::EnableKeyFeedback( CVirtualKey* aKey, TBool aEnable )
    {
#ifdef RD_TACTILE_FEEDBACK 
    CVirtualKeyCtrl* keyCtrl = FindCtrlForKey(aKey);
  
    if ( keyCtrl && keyCtrl->IsKindOfControl(ECtrlTactileFeedback) )
        {
        TInt id = reinterpret_cast<TInt>(keyCtrl);
        UiLayout()->DeRegisterFeedbackArea(id,keyCtrl->Rect());
        if( aEnable )
            {
            UiLayout()->RegisterFeedbackArea(id,keyCtrl->Rect(),keyCtrl->TactileFeedbackType());
            }
        }
#endif    
    }

void CVirtualKeyboard::AddKeyControlL(CVirtualKeyCtrl* aKeyCtrl)
    {
    //do nothing if we already has the control.
    if(!aKeyCtrl || KErrNotFound != iCtrlList.Find(aKeyCtrl))
        return;   
    
    iOwnedCtrlList.AppendL(aKeyCtrl);

    iCtrlList.AppendL(aKeyCtrl);     
    aKeyCtrl->iKeyboard = this;
    //aKeyCtrl->Move(iRect.iTl);  
    }


void CVirtualKeyboard::RemoveKeyControl(CVirtualKeyCtrl* aKeyCtrl)
    {
    if(!aKeyCtrl)
        return;
    TInt idx = iOwnedCtrlList.Find(aKeyCtrl);
    if(KErrNotFound != idx)
        {
        iOwnedCtrlList.Remove(idx);
        }
    idx = iCtrlList.Find(aKeyCtrl);
    if(KErrNotFound != idx)
        {
        iCtrlList.Remove(idx);
        }
    //aKeyCtrl can't be deleted. This is for VKB fix: VKB will dynamically
    //remove current keys during layout handling pointer event. Thus
    //CControlGroup::SetActiveCtrl will panic.
    //delete aKeyCtrl;        
    TInt err;
    aKeyCtrl->SetReady(EFalse); //disable the control    
    if(aKeyCtrl->IsKindOfControl(ECtrlVirtualItutKey))
        {
        err = iUnUsedRawKeyCtrlPool.Append(aKeyCtrl);
        }
    else        
        {
        err = iUnUsedKeyCtrlPool.Append(aKeyCtrl);
        }
    if(KErrNone != err)
        delete aKeyCtrl;        
    }
// ---------------------------------------------------------------------------
// CVirtualKeyboard::SetRect
// Set the control rect
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CVirtualKeyboard::SetRect(const TRect& aRect)
    {
    if(aRect == Rect())
        return;    
    TPoint offset = aRect.iTl - iRect.iTl;    
    CControlGroup::SetRect(aRect);
    //move all virtual keys
    for(TInt i = 0; i < ControlList().Count(); ++i)
        {
        ControlList()[i]->Move(offset);        
        }
    }


// ---------------------------------------------------------------------------
// CVirtualKeyboard::CreateKeyCtrlL
// Create a control
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
CVirtualKeyCtrl* CVirtualKeyboard::CreateKeyCtrlL(CVirtualKey* aKeyInfo,
                                                            TBool aRawKeyFlag)
    {
    RPointerArray<CVirtualKeyCtrl> &unUsedCtrlPool = aRawKeyFlag ?
                		 iUnUsedRawKeyCtrlPool : iUnUsedKeyCtrlPool;
    for(TInt i = 0 ; i < unUsedCtrlPool .Count(); ++i)
        {
        if(unUsedCtrlPool [i]->iKeyInfo->ScanCode() == aKeyInfo->ScanCode())
            {
            CVirtualKeyCtrl* ctrl = unUsedCtrlPool [i];            
            unUsedCtrlPool.Remove(i);            
            ctrl->iKeyInfo = aKeyInfo;
            TRect rect = aKeyInfo->Rect();
            rect.Move(Rect().iTl);
            ctrl->SetRect(rect);            
            aKeyInfo->iKeyCtrl = ctrl;
            ctrl->SetReady(ETrue); //enable the control    
            return ctrl;
            }
        }
    if(aRawKeyFlag)
        return CVirtualRawKeyCtrl::NewL(this,aKeyInfo);
    else
        return CVirtualKeyCtrl::NewL(this,aKeyInfo);
    }
    
void CVirtualKeyboard::ConstructIrregularBmps()
    {
    ConstructOneIrregularBmp(iKeyLeftDownBmpList);    

    ConstructOneIrregularBmp(iKeyMiddleUpBmpList);    

    ConstructOneIrregularBmp(iKeyMiddleDownBmpList);    

    ConstructOneIrregularBmp(iKeyRightDownBmpList);    

    ConstructOneIrregularBmp(iKeyRightUpBmpList);    
    }
    
void CVirtualKeyboard::ConstructOneIrregularBmp(RPointerArray<CFbsBitmap>& iBmpList)    
    {
    if (iBmpList.Count() != 0)
        {
        return;
        }
    
    
    for(TInt index = 0; index <= EKeyStateDimmedMask ; index ++)
        {
        iBmpList.Append(NULL);
        }
    }
    
void CVirtualKeyboard::DestroyIrregularBmps()
    {
    iKeyLeftDownBmpList.ResetAndDestroy();    
    iKeyLeftDownBmpList.Close();

    iKeyMiddleUpBmpList.ResetAndDestroy();    
    iKeyMiddleUpBmpList.Close();

    iKeyMiddleDownBmpList.ResetAndDestroy();    
    iKeyMiddleDownBmpList.Close();

    iKeyRightDownBmpList.ResetAndDestroy();    
    iKeyRightDownBmpList.Close();

    iKeyRightUpBmpList.ResetAndDestroy();    
    iKeyRightUpBmpList.Close();
    }
    
EXPORT_C void CVirtualKeyboard:: AddIrregularKeyBitmapL(TVirtualKeyIrregularKeyType aKeyType,
                                     TVirtualKeyIrregularKeyState aKeyState,
                                     CFbsBitmap* aBmp)
    {
    switch(aKeyType)
        {
        case EKeyLeftDown:
            {
            delete iKeyLeftDownBmpList[aKeyState];
            iKeyLeftDownBmpList[aKeyState] = aBmp;
            }
        break;
        case EKeyMiddleUp:
            {
            delete iKeyMiddleUpBmpList[aKeyState];
            iKeyMiddleUpBmpList[aKeyState] = aBmp;
            }
        break;
        case EKeyMiddleDown:
            {
            delete iKeyMiddleDownBmpList[aKeyState];
            iKeyMiddleDownBmpList[aKeyState] = aBmp;
            }
        break;
        case EKeyRightDown:
            {
            delete iKeyRightDownBmpList[aKeyState];
            iKeyRightDownBmpList[aKeyState] = aBmp;
            }
        break;
        case EKeyRightUp:
            {
            delete iKeyRightUpBmpList[aKeyState];
            iKeyRightUpBmpList[aKeyState] = aBmp;
            }
        break;
        default:
            {
            User::Leave(KErrNotFound);
            }        
        }
    }
    
EXPORT_C void CVirtualKeyboard::RemoveIrregularKeyBitmapL(TVirtualKeyIrregularKeyType aKeyType,
                                                          TVirtualKeyIrregularKeyState aKeyState)
    {
    switch(aKeyType)
        {
        case EKeyLeftDown:
            {
            delete iKeyLeftDownBmpList[aKeyState];
            iKeyLeftDownBmpList[aKeyState] = NULL;
            }
        break;
        case EKeyMiddleUp:
            {
            delete iKeyMiddleUpBmpList[aKeyState];
            iKeyMiddleUpBmpList[aKeyState] = NULL;
            }
        break;
        case EKeyMiddleDown:
            {
            delete iKeyMiddleDownBmpList[aKeyState];
            iKeyMiddleDownBmpList[aKeyState] = NULL;
            }
        break;
        case EKeyRightDown:
            {
            delete iKeyRightDownBmpList[aKeyState];
            iKeyRightDownBmpList[aKeyState] = NULL;
            }
        break;
        case EKeyRightUp:
            {
            delete iKeyRightUpBmpList[aKeyState];
            iKeyRightUpBmpList[aKeyState] = NULL;
            }
        break;
        default:
            {
            User::Leave(KErrNotFound);
            }
        }
    }
    
EXPORT_C CFbsBitmap* CVirtualKeyboard::IrregularKeyBitmap(TVirtualKeyIrregularKeyType aKeyType,
                                        TVirtualKeyIrregularKeyState aKeyState)    
    {
    CFbsBitmap* ret;
    switch(aKeyType)
        {
        case EKeyLeftDown:
            {
            ret = iKeyLeftDownBmpList[aKeyState];
            }
        break;
        case EKeyMiddleUp:
            {
            ret = iKeyMiddleUpBmpList[aKeyState];
            }
        break;
        case EKeyMiddleDown:
            {
            ret = iKeyMiddleDownBmpList[aKeyState];
            }
        break;
        case EKeyRightDown:
            {
            ret = iKeyRightDownBmpList[aKeyState];
            }
        break;
        case EKeyRightUp:
            {
            ret = iKeyRightUpBmpList[aKeyState];
            }
        break;
        default:
            {
            ret = NULL;
            }
        }
    
    return ret;
    } 
    
EXPORT_C TBool CVirtualKeyboard::IsIrregularKeyBoard()
    {
    return iIrregularKey;
    }
                                                                                          
EXPORT_C void CVirtualKeyboard::SetIrregularKeyBoard(TBool aIrregular)
    {
    iIrregularKey = aIrregular;

    if (iIrregularKey)
        {
        ConstructIrregularBmps();    
        }
    }
    
EXPORT_C void CVirtualKeyboard::SetNonIrregularKeyBitmapL(TVirtualKeyBmpType aBmpType, 
                                                          CFbsBitmap* aBmp)
 	{
 	TInt i = iNonIrregularKeyBmpList.Count();
 	for( ; i < EKeyBmpLastType + 1; i++)
 		{
 		iNonIrregularKeyBmpList.Append(NULL);
 		}
 	delete iNonIrregularKeyBmpList[aBmpType];
 	iNonIrregularKeyBmpList[aBmpType] = aBmp;
 	}
 	
EXPORT_C CFbsBitmap* CVirtualKeyboard::NonIrregularKeyBitmap(TVirtualKeyBmpType aBmpType)
	{
	if( iNonIrregularKeyBmpList.Count() > aBmpType )
	    {
    	return 	iNonIrregularKeyBmpList[aBmpType];
	    }
	return NULL;
	}
	
EXPORT_C void CVirtualKeyboard::SetVirtualKeyImgID(TVirtualKeyDrawInfo aVirtualKeyDrawInfo)
	{
	for(TInt i = 0; i < ControlList().Count(); ++i)
        {
        CVirtualKeyCtrl* ctrl = static_cast<CVirtualKeyCtrl*>(ControlList()[i]);
        ctrl->SetVirtualKeyImgID(aVirtualKeyDrawInfo);
        }
	}	

EXPORT_C void CVirtualKeyboard::SetShiftIcon( CPenInputColorIcon* aIcon )
	{
	delete iShiftIcon;
	iShiftIcon = aIcon;	
	}
/*
EXPORT_C CFepUiBaseCtrl* CVirtualKeyboard::HandlePointerDownEventL(const TPoint& aPoint)
    {
    return CControlGroup::HandlePointerDownEventL(aPoint);
    }
    
EXPORT_C CFepUiBaseCtrl* CVirtualKeyboard::HandlePointerUpEventL(const TPoint& aPoint)
    {
    return CControlGroup::HandlePointerUpEventL(aPoint);
    }

EXPORT_C CFepUiBaseCtrl* CVirtualKeyboard::HandlePointerMoveEventL(const TPoint& aPoint)
    {
    return CControlGroup::HandlePointerMoveEventL(aPoint);
    }
    */				 
//end of implementation of class CVirtualKeyboard
    
// Implementation of Class CVirtualKey

// ---------------------------------------------------------------------------
// CVirtualKey::CVirtualKey
// default constructor
// ---------------------------------------------------------------------------
//
CVirtualKey::CVirtualKey(TUint aCode, TInt aScanCode, 
                         const TRect& aRect, TInt aHighlightBmpIdx, TInt aHighlightMaskBmpId)
                                  : 
                                  iRect(aRect),
                                  iHighlightBmpIndex(aHighlightBmpIdx),
                                  iHighlightMaskBmpIndex(aHighlightMaskBmpId),
                                  iIsLatchKey(EFalse),iLatched(EFalse)
    {
    iCode = aCode;
    iScanCode = aScanCode;
    }
    
// ---------------------------------------------------------------------------
// CVirtualKey::CVirtualKey
// default constructor
// ---------------------------------------------------------------------------
//
CVirtualKey::CVirtualKey(TInt aScanCode,
                         const TRect& aRect)
                         : iRect(aRect),
                          iHighlightBmpIndex(-1),
                          iHighlightMaskBmpIndex(-1),
                          iIsLatchKey(EFalse),iLatched(EFalse)
    {
    iScanCode = aScanCode;
    }                         

// ---------------------------------------------------------------------------
// CVirtualKey::NewL
// factory function
// ---------------------------------------------------------------------------
//
EXPORT_C CVirtualKey* CVirtualKey::NewL(TUint aCode, TInt aScanCode,
                                 const TRect& aRect, const TRect& aCharRect, 
                                 TInt aBaseLine, TInt aHighlightBmpIdx,
                                 TInt aHighlightMaskBmpId)
    {
    TBuf<1> buf;
    buf.Append(aCode);
    return CVirtualKey::NewL(buf,aScanCode,aRect,aCharRect,aBaseLine,
                                aHighlightBmpIdx,aHighlightMaskBmpId);
    }

// ---------------------------------------------------------------------------
// CVirtualKey::NewL
// factory fuction
// ---------------------------------------------------------------------------
//
EXPORT_C CVirtualKey* CVirtualKey::NewL(const TDesC& aCodeData, TInt aScanCode,
                                    const TRect& aRect, const TRect& /*aCharRect*/, 
                                    TInt /*aBaseLine*/, TInt aHighlightBmpIdx,
                                    TInt aHighlightMaskBmpId) 
    {
    TUint code = 0;
    
    if(aCodeData.Length())
        code = aCodeData[0];
    
    CVirtualKey* key = new(ELeave) CVirtualKey(code,aScanCode,aRect,
                                                aHighlightBmpIdx,
                                                aHighlightMaskBmpId);
    CleanupStack::PushL(key);
    
    if (code == 0xffffffff||
        code == 0xffff)
    	{
    	key->ConstructL(KNullDesC());	
    	}
    else
    	{
    	key->ConstructL(aCodeData);	
    	}
    
    CleanupStack::Pop(key);
    return key;
    }
    
// ---------------------------------------------------------------------------
// CVirtualKey::NewL
// factory fuction
// ---------------------------------------------------------------------------
//
EXPORT_C CVirtualKey* CVirtualKey::NewL(RPointerArray<HBufC>& aUnicodesList,
                                        TInt aScanCode,
                                        const TRect& aRect)
    {
    CVirtualKey* key = new (ELeave) CVirtualKey(aScanCode,
                                                aRect);
    CleanupStack::PushL(key);
    key->ConstructL(aUnicodesList);
    CleanupStack::Pop(key);
    return key;
    }

// ---------------------------------------------------------------------------
// CVirtualKey::ConstructL()
// 2nd constructor
// ---------------------------------------------------------------------------
//
void CVirtualKey::ConstructL(const TDesC& aCodeData)
    {
    iCodeData = aCodeData.AllocL();
    }

// ---------------------------------------------------------------------------
// CVirtualKey::ConstructL()
// 2nd constructor
// ---------------------------------------------------------------------------
//
void CVirtualKey::ConstructL(const RPointerArray<HBufC>& aUnicodesList)
    {
    iCodeData = KNullDesC().AllocL();

    for (TInt i = 0; i <= EPosLast; i++)
        {
        if (i <= aUnicodesList.Count() - 1)
            {
            iUnicodesList.AppendL(aUnicodesList[i]);
            }
        else
            {
            iUnicodesList.AppendL(NULL);
            }
        }
    }

// CVirtualKey::~CVirtualKey
// default destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CVirtualKey::~CVirtualKey()
    {    
    delete iCodeData;
    delete iDisplayCodeData;
    iUnicodesList.ResetAndDestroy();
    iUnicodesList.Close();
    }


// ---------------------------------------------------------------------------
// set latch state
// ---------------------------------------------------------------------------
//           
EXPORT_C void CVirtualKey::SetLatched(TBool aFlag)
    {    
    if(!iIsLatchKey || iLatched == aFlag)
        return;

    iLatched = aFlag;
    if(iKeyCtrl)
        {
        //find key        
        //iKeyboard->DrawKey(this);
        iKeyCtrl->Draw();
        }
    }

// ---------------------------------------------------------------------------
//set key rect
// ---------------------------------------------------------------------------
//
EXPORT_C void CVirtualKey::SetRect(const TRect& aRect)
    {
    if(iRect == aRect)
        return;
    //if(iKeyboard)
    //    iKeyboard->ChangeFeedbackArea(reinterpret_cast<TInt>(this),aRect,iRect);
    iRect = aRect;    

    // CVirtualKey's rect is not absoluate position, but relative to keyboard,
    // so need to move CVirtualKeyCtrl
    if(iKeyCtrl)
        {
        iKeyCtrl->SetRect(iRect);
        iKeyCtrl->Move(iKeyCtrl->Keyboard()->Rect().iTl);
        }
    }

EXPORT_C void CVirtualKey::SetUnicodesL(RPointerArray<HBufC>& aUnicodesList)
    {
    iUnicodesList.ResetAndDestroy();

    for (TInt i = 0; i <= EPosLast; i++)
        {
        if (i <= aUnicodesList.Count() - 1)
            {
            iUnicodesList.AppendL(aUnicodesList[i]);
            }
        else
            {
            iUnicodesList.AppendL(NULL);
            }
        }
    }
    
EXPORT_C TVirtualKeyIrregularKeyType CVirtualKey::IrregualrKeyType()
    {
    return iIrregularKeyType; 
    }     

EXPORT_C void CVirtualKey::SetIrregualrKeyType(TVirtualKeyIrregularKeyType aKeyType)
    {
    iIrregularKeyType = aKeyType;
    }    

// ---------------------------------------------------------------------------
// CVirtualKey::KeyUnicodes
//Return key unicodes in a specific position -- support for multi language
// ---------------------------------------------------------------------------
const TDesC& CVirtualKey::KeyUnicodes(TVirtualKeyTextPos aPos) const
    {
    if (aPos >= 0 && aPos <= EPosLast && iUnicodesList[aPos])
        {
        return *(iUnicodesList[aPos]); 
        }

    return KNullDesC;    
    }

TBool CVirtualKey::UnicodesListEmpty(TInt& aIdx)
    {
    if (iUnicodesList.Count() == 0)
        {
        return ETrue;
        }

    TBool empty = ETrue;

    for (TInt i = 0; i <= EPosLast; i++)
        {
        if (iUnicodesList[i])
            {
            empty = EFalse;
            aIdx = i;
            break;
            }
        }
    
    return empty;    
    }

EXPORT_C void CVirtualKey::SetDisplayUnicodeL( const TDesC& aDspUnicode )
	{
	delete iDisplayCodeData;
	iDisplayCodeData = NULL;
	iDisplayCodeData = aDspUnicode.AllocL();
	}

EXPORT_C void CVirtualKey::HideKeyCtrl( TBool aHide )
    {
    if ( iKeyCtrl )
        {
        iKeyCtrl->Hide( aHide );
        }
    }
//end of implementation of Class CVirtualKey    
