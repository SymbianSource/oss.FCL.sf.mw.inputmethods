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
#include <AknsDrawUtils.h>

#include "peninputlayoutmicfcustomdraw.h"
#include "peninputlayoutmultilineicf.h"
#include "peninputlayoutsmiley.h"

CFepLayoutMIcfCustomDraw::CFepLayoutMIcfCustomDraw(CFepLayoutMultiLineIcfEditor* aParentControl)
    :iParentControl(aParentControl)
    {
    }

CFepLayoutMIcfCustomDraw::~CFepLayoutMIcfCustomDraw()
    {
    }

void CFepLayoutMIcfCustomDraw::DrawBackground(const TParam& aParam,
                                              const TRgb& /*aRgb*/,
                                              TRect& aDrawn) const
    {
    CFbsBitGc& gc = (CFbsBitGc&)(aParam.iGc);

    TRect drawrect = iParentControl->Rect();
    aDrawn = aParam.iDrawRect;
    gc.SetClippingRect(aDrawn);
    gc.Clear(drawrect);
    
    if (iParentControl->BgImgSkinId() != KAknsIIDNone)
        {
	    // set clipping rect, make sure text is not hide by drawframe
        
		AknsDrawUtils::DrawFrame(AknsUtils::SkinInstance(), 
		                         gc, 
		                         drawrect, 
		                         drawrect,
		                         iParentControl->BgImgSkinId(),
		                         KAknsIIDDefault);

        
        }
    else
    	{
    	iParentControl->DrawIcfBackground( gc, aDrawn );
    	}    
    	
    gc.CancelClippingRect();
    }

void CFepLayoutMIcfCustomDraw::DrawText( const TParam& aParam, const TLineInfo& aLineInfo, 
		                                 const TCharFormat& aFormat, const TDesC& aText, 
		                                 const TPoint& aTextOrigin, TInt aExtraPixels ) const
    {
    if ( !iParentControl->SmileyEnabled() )
        {
        MFormCustomDraw::DrawText( aParam, aLineInfo,  aFormat, aText, 
                                   aTextOrigin, aExtraPixels );
        return;
        }
    
    //get count of smileys
    TInt count = 0;
    for ( TInt i = 0; i < aText.Length(); i ++ )
        {
        if ( iParentControl->SmileyManager()->IsSmileyCode(aText[i] ) )
            {
            count++;
            break;
            }
        }
    
    if ( count < 1 )
        {
        //no smiley found, call default draw
        MFormCustomDraw::DrawText( aParam, aLineInfo,  aFormat, aText, 
                                   aTextOrigin, aExtraPixels );
        return;        
        }

    //custom draw   
    TInt topline = aParam.iDrawRect.iTl.iY;
    TInt bottomline = aParam.iDrawRect.iBr.iY;
    TInt baseline = aLineInfo.iBaseline;
    TPoint basepoint = aParam.iDrawRect.iTl;
    basepoint.iY = baseline;

    CFont* font = NULL;
    aParam.iMap.GetNearestFontInTwips( font, aFormat.iFontSpec );
    
    //cut off regions occupied by smiley characters.
    //smiley character is displayed as a rectangle if not supported by current font.
    RRegion rgn;
    TRAP_IGNORE( 
            iParentControl->CalcSmileyClipRegionL( 
                    rgn, aParam.iGc, *font, 
                    aText, basepoint, topline, 
                    bottomline ) 
            );
    
    //call default drawing first
    aParam.iGc.SetClippingRegion( rgn );
    
    MFormCustomDraw::DrawText( aParam, aLineInfo, aFormat, aText, 
                               aTextOrigin, aExtraPixels );
    
    aParam.iGc.CancelClippingRegion();
    
    rgn.Close();
    
    //fill in smiley icons
    TRAP_IGNORE( 
            iParentControl->CustomDrawSmileyL( 
                    aParam.iGc, *font, aText, 
                    basepoint, topline, bottomline ) 
            );
    
    aParam.iMap.ReleaseFont( font );
	}

void CFepLayoutMIcfCustomDraw::DrawText( const TParam& aParam, const TLineInfo& aLineInfo,
                                         const TCharFormat& aFormat, const TDesC& aText,
                                         const TInt aStart, const TInt aEnd, 
                                         const TPoint& aTextOrigin,TInt aExtraPixels) const
    {
    TPtrC text = aText.Mid( aStart, aEnd - aStart );
    DrawText( aParam, aLineInfo, aFormat, text, aTextOrigin, aExtraPixels );
    }
		       

TBool CFepLayoutMIcfCustomWrap::LineBreakPossible(TUint aPrevClass, TUint aNextClass, 
                                                  TBool aHaveSpaces) const
    {
    if( aHaveSpaces )
        {
        return ETrue;
        }
    return MFormCustomWrap::LineBreakPossible( aPrevClass, aNextClass, aHaveSpaces );
    }

TUint CFepLayoutMIcfCustomWrap::LineBreakClass(TUint aCode, TUint& aRangeStart,
                                               TUint& aRangeEnd) const
    {
    aRangeStart = aCode;
    aRangeEnd = aCode + 1;
 
    switch (aCode)
        {
        case 0x0020: // unicode for space
            return MTmCustom::EBkLineBreakClass;//ELineBreakClasses;
        default:
            break; // carry on
        }
        
    return MFormCustomWrap::LineBreakClass(aCode,aRangeStart,aRangeEnd);
     }

// End Of File
