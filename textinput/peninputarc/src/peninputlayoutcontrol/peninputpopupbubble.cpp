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
* Description:  Implementation for CPopupBubbleCtrl
*
*/


#include <AknIconUtils.h>
#include <AknsUtils.h>
#include <AknUtils.h>
#include <AknsDrawUtils.h> 

#include "peninputpopupbubble.h"
#include "peninputlayout.h"
#include <peninputpluginutils.h>
#include <imageconversion.h> 
#include "peninputcmd.h"

const TInt KShrinkSize = 10;
const TInt KDefaultTextColorIndex = EAknsCIQsnTextColorsCG60;
const TInt KAlpha = 200;

// ======== MEMBER FUNCTIONS ========

/*****************************************************************************/
// Implementation of Class CPopupBubbleWnd

// ---------------------------------------------------------------------------
// c++ constructor
// ---------------------------------------------------------------------------
//
CPopupBubbleWnd::CPopupBubbleWnd(CPopupBubbleCtrl& aParent)
: iParent(aParent),
iSupportTransparent(EFalse)
    {
    }

// ---------------------------------------------------------------------------
// c++ destructor
// ---------------------------------------------------------------------------
//
CPopupBubbleWnd::~CPopupBubbleWnd()
    {
    //delete bitmaps    
    iBitGc.ResetAndDestroy();
    iDevice.ResetAndDestroy();
    iBitmap.ResetAndDestroy();       
    }

// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
CPopupBubbleWnd* CPopupBubbleWnd::NewL(const TRect& aRect, CPopupBubbleCtrl& aParent)
    {
    CPopupBubbleWnd* self = CPopupBubbleWnd::NewLC(aRect, aParent);
    CleanupStack::Pop(); // self;
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
CPopupBubbleWnd* CPopupBubbleWnd::NewLC(const TRect& aRect, CPopupBubbleCtrl& aParent)
    {
    CPopupBubbleWnd* self = new ( ELeave ) CPopupBubbleWnd(aParent);
    CleanupStack::PushL( self );
    self->ConstructL(aRect);
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian second-phase constructor
// ---------------------------------------------------------------------------
//
void CPopupBubbleWnd::ConstructL(const TRect& aRect)
    {
    RWindowGroup& wg = CCoeEnv::Static()->RootWin();
    CreateWindowL( wg );
    SetComponentsToInheritVisibility();
    Window().SetRequiredDisplayMode( EColor16MA );
    TInt err = Window().SetTransparencyAlphaChannel();
    iSupportTransparent = ( KErrNone == err );
    Window().SetBackgroundColor( TRgb(0,0,0,0) );

    //create bitmaps
    for(TInt i = 0; i<ELastItem; i++)
        {
        CFbsBitmap* bitmap = new (ELeave) CFbsBitmap(); // Bitmap
        CleanupStack::PushL(bitmap);
        iBitmap.AppendL(bitmap); //take ownership
        CleanupStack::Pop(bitmap);
        iBitmap[i]->Create(aRect.Size(), EColor16MA);
        
        CFbsBitmapDevice* device = CFbsBitmapDevice::NewL(iBitmap[i]); //BitmapDevice
        CleanupStack::PushL(device);
        iDevice.AppendL(device); //take ownership
        CleanupStack::Pop(device);
        
        CFbsBitGc* bitgc = CFbsBitGc::NewL(); // GraphicsContext
        CleanupStack::PushL(bitgc);
        iBitGc.AppendL(bitgc); //take ownership
        CleanupStack::Pop(bitgc);
        }
    
    MakeVisible( ETrue );    
    SetRect(aRect);
    ActivateL();
    }

// ---------------------------------------------------------------------------
// Draw this control
// ---------------------------------------------------------------------------
//
void CPopupBubbleWnd::Draw(const TRect& aRect) const
    {    
    if ( !iParent.AbleToDraw() )
        {
        return;
        }
    
    CWindowGc& gc = SystemGc();

    if ( !SupportTransparent() )
        {
        gc.Clear( aRect ); 
        }    
    
    DrawTransparentBackground( aRect );

    if ( iParent.iForgroundBmp )
        {
        TRect innerRect = aRect;
        innerRect.Shrink( KShrinkSize, 0 );
        
        TRect iconRect;
        iconRect.iTl = TPoint( innerRect.iTl.iX + iParent.iIconOffset.iWidth, 
                innerRect.iTl.iY + iParent.iIconOffset.iHeight );
        iconRect.SetWidth( iParent.iIconSize.iWidth );
        iconRect.SetHeight( iParent.iIconSize.iHeight );
        
        if( iParent.iForgroundBmp->SizeInPixels() != iconRect.Size() )
            {
            AknIconUtils::SetSize( iParent.iForgroundBmp, iconRect.Size(), EAspectRatioNotPreserved );
            }
            
        TRect srcRect( TPoint( 0, 0 ), iParent.iForgroundBmp->SizeInPixels() );
        
        if( iParent.iForgroundBmpMask )
            {
            if( iParent.iForgroundBmpMask->SizeInPixels() != iconRect.Size() )
                {
                AknIconUtils::SetSize( iParent.iForgroundBmpMask, iconRect.Size(), EAspectRatioNotPreserved);
                }
                        
            gc.SetBrushStyle( CGraphicsContext::ENullBrush );
            gc.BitBltMasked( iconRect.iTl, 
                              iParent.iForgroundBmp, 
                              srcRect,
                              iParent.iForgroundBmpMask,
                              EFalse);
            }
        else
            {
            gc.BitBlt( iconRect.iTl,
                        iParent.iForgroundBmp,
                        srcRect );
            }            
        }
    
    if ( iParent.iText && iParent.iText->Length() > 0 )
        {
        gc.SetBrushStyle( CGraphicsContext::ENullBrush );
        TAknLayoutText textLayout;
        textLayout.LayoutText(Rect(), iParent.iTextFormat);
        TRgb color( KRgbBlack );  // sane default for nonskinned case
        if ( AknsUtils::AvkonSkinEnabled() )
            {
            AknsUtils::GetCachedColor( AknsUtils::SkinInstance(),
                                   color, KAknsIIDQsnTextColors, iParent.iTextColorIndex );
            }
         textLayout.DrawText(gc, *(iParent.iText), ETrue, color);
        }    
    }

// ---------------------------------------------------------------------------
// Resize this control
// ---------------------------------------------------------------------------
//
void CPopupBubbleWnd::SizeChanged()
    {
    TRect rect = Rect();        
    
    iRect[EMiddleItem] = rect;    
    
    iRect[EMiddleItem].Shrink( KShrinkSize, 0 );
      
    // horizon
    if( rect.iTl.iY == iRect[EMiddleItem].iTl.iY && rect.iBr.iY == iRect[EMiddleItem].iBr.iY )
        {
        iRect[ELeftItem] = TRect( rect.iTl, TPoint( iRect[EMiddleItem].iTl.iX, iRect[EMiddleItem].iBr.iY ) );
        iRect[ERightItem] = TRect( TPoint( iRect[EMiddleItem].iBr.iX, iRect[EMiddleItem].iTl.iY), rect.iBr );    
        }
    else if( rect.iTl.iX == iRect[EMiddleItem].iTl.iX &&  rect.iBr.iX == iRect[EMiddleItem].iBr.iX )
        {
        iRect[ELeftItem] = TRect( rect.iTl, TPoint( iRect[EMiddleItem].iBr.iX, iRect[EMiddleItem].iTl.iY ) );
        iRect[ERightItem] = TRect( TPoint( iRect[EMiddleItem].iTl.iX, iRect[EMiddleItem].iBr.iY), rect.iBr );            
        }     
    
    SizeImage( iParent.iFirstBmpId, iRect[ELeftItem], iBitmap[ELeftItem], iDevice[ELeftItem], iBitGc[ELeftItem] );
    SizeImage( iParent.iMiddleBmpId, iRect[EMiddleItem], iBitmap[EMiddleItem], iDevice[EMiddleItem], iBitGc[EMiddleItem] );
    SizeImage( iParent.iLastBmpId, iRect[ERightItem], iBitmap[ERightItem], iDevice[ERightItem], iBitGc[ERightItem] );
    }

// ---------------------------------------------------------------------------
// Handle resource change
// ---------------------------------------------------------------------------
//
void CPopupBubbleWnd::HandleResourceChange( TInt aType )
    {
    if ( aType == KAknsMessageSkinChange )
        {
        // When skin changes, we need to update local bitmaps
        SizeChanged();
        }
    }

// ---------------------------------------------------------------------------
// Size images used for drawing
// ---------------------------------------------------------------------------
//
void CPopupBubbleWnd::SizeImage( TAknsItemID aId, TRect aRect, CFbsBitmap* aBitmap, CFbsBitmapDevice* aDevice, CFbsBitGc* aBitGc )
    {     
    MAknsSkinInstance* instance = AknsUtils::SkinInstance();
    TAknsItemType type = instance->GetCachedItemData( aId )->Type();
    if ( type == EAknsITMaskedBitmap )
        {
        CAknsMaskedBitmapItemData* itemData =
                static_cast<CAknsMaskedBitmapItemData*> ( instance->GetCachedItemData( aId ) );

        AknIconUtils::SetSize( itemData->Bitmap(), aRect.Size(), EAspectRatioNotPreserved );
        AknIconUtils::SetSize( itemData->Mask(), aRect.Size(), EAspectRatioNotPreserved );

        TDisplayMode bitmapmode = itemData->Bitmap()->DisplayMode();
        TDisplayMode maskmode = itemData->Mask()->DisplayMode();

        aBitmap->Resize( aRect.Size() );
        aDevice->Resize( aRect.Size() );
        aBitGc->Activate( aDevice );

        TRect entireImage( TPoint(0, 0), itemData->Bitmap()->SizeInPixels() );
        if (itemData->Mask())
            {
            aBitGc->BitBltMasked( TPoint(0, 0), itemData->Bitmap(), entireImage,
                    itemData->Mask(), ETrue );
            }
        else
            {
            aBitGc->BitBlt( TPoint(0, 0), itemData->Bitmap() );
            }

        TInt size = aBitmap->DataSize();
        TUint32* data = aBitmap->DataAddress();
        TDisplayMode mode = aBitmap->DisplayMode();

        iBitmap[ETempItem]->Resize( aRect.Size() );
        iDevice[ETempItem]->Resize( aRect.Size() );
        iBitGc[ETempItem]->Activate( iDevice[ETempItem] );
        iBitGc[ETempItem]->BitBlt( TPoint(0, 0), itemData->Mask() );
        
        TUint32* mask_data = iBitmap[ETempItem]->DataAddress(); 
         
         if (mode == EColor16MA)
             {
             for (TInt i = 0; i < size/sizeof( TUint32 ); i++)
                 {
                 TRgb colour;
                 colour.SetInternal( *(mask_data + i) );
                 if (colour != TRgb( 0x00, 0x00, 0x00, 0xFF ))
                     {
                     colour.SetInternal( *(data + i) );
                     colour.SetAlpha( KAlpha ); //semi transparent
                     *(data + i) = colour.Internal();
                     }
                 else
                     {
                     colour.SetInternal( *(data + i) );
                     colour.SetAlpha( 0x00 ); //fully transparent
                     *(data + i) = colour.Internal();                   
                     }
                 }           
             }
        }
    } 

// ---------------------------------------------------------------------------
// Draw transparent background
// ---------------------------------------------------------------------------
//

void CPopupBubbleWnd::DrawTransparentBackground( const TRect& aRect ) const
{
    CWindowGc& gc = SystemGc();

    for( TInt i=0; i<ELastItem; i++ )
        {
        if ( i == ETempItem )//skip temporary bitmap
            {
            continue;
            }
        if ( iBitmap[i] )//draw other bitmaps
            {
            gc.BitBlt( iRect[i].iTl, iBitmap[i] );
            }
        }
}

/*****************************************************************************/
// Implementation of Class CPopupBubbleCtrl

// ---------------------------------------------------------------------------
// CPopupBubbleCtrl::NewL
// Symbian constructor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//               
EXPORT_C CPopupBubbleCtrl* CPopupBubbleCtrl::NewL( const TRect& aRect,
                               CFepUiLayout* aUiLayout,
                               TInt aCtrlId )
    {
    CPopupBubbleCtrl* ctrl = new (ELeave)CPopupBubbleCtrl( aRect,aUiLayout,aCtrlId );
    CleanupStack::PushL( ctrl );
    ctrl->BaseConstructL();
    
    CleanupStack::Pop( ctrl );
    return ctrl;
    }

// ---------------------------------------------------------------------------
// CPopupBubbleCtrl::CPopupBubbleCtrl
// C++  constructor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CPopupBubbleCtrl::CPopupBubbleCtrl( const TRect& aRect,
                         CFepUiLayout* aUiLayout,
                         TInt aCtrlId )
    :CFepUiBaseCtrl( aRect,aUiLayout,aCtrlId ),
     iTextColorIndex( KDefaultTextColorIndex )
    {
    SetControlType( ECtrlButton );
    SetHidenFlag( ETrue );
    }

// ---------------------------------------------------------------------------
// CPopupBubbleCtrl::CPopupBubbleCtrl
// C++  destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CPopupBubbleCtrl::~CPopupBubbleCtrl()
    {
    if ( iText )
        {
        delete iText;
        iText = NULL;
        }      
    if( iForgroundBmp )
        {
        delete iForgroundBmp;
        iForgroundBmp = NULL;
        }
    if( iForgroundBmpMask )
        {
        delete iForgroundBmpMask;
        iForgroundBmpMask = NULL;
        }        
    }

// ---------------------------------------------------------------------------
// CPopupBubbleCtrl::SetBitmapParamL
// Set bitmap parameters used fot this bubble
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CPopupBubbleCtrl::SetBitmapParamL(
                                    CFbsBitmap* aBmpId,
                                    CFbsBitmap* aMaskBmpId,     
                                 TAknsItemID aFirstBmpId,
                                 TAknsItemID aMiddleBmpId,
                                 TAknsItemID aLastBmpId)
    {
    if( iForgroundBmp )
        {
        delete iForgroundBmp;
        iForgroundBmp = NULL;
        }
    if( iForgroundBmpMask )
        {
        delete iForgroundBmpMask;
        iForgroundBmpMask = NULL;
        }    
        
    iForgroundBmp = aBmpId;
    iForgroundBmpMask = aMaskBmpId;
    
    iFirstBmpId = aFirstBmpId;
    iMiddleBmpId =  aMiddleBmpId;
    iLastBmpId = aLastBmpId;
    
    delete iWndControl;
    iWndControl = NULL;
    iWndControl = CPopupBubbleWnd::NewL( Rect(), *this );
    }

// ---------------------------------------------------------------------------
// CPopupBubbleCtrl::Hide
// Hide bubble control
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CPopupBubbleCtrl::Hide( TBool aFlag )
    {
    if ( iWndControl )
        {
        iWndControl->MakeVisible( !aFlag );
        }
    CFepUiBaseCtrl::Hide( aFlag );    
    }

// ---------------------------------------------------------------------------
// CPopupBubbleCtrl::Draw
// Draw bubble control
// ---------------------------------------------------------------------------
//
EXPORT_C void CPopupBubbleCtrl::Draw()
    {
    if( !AbleToDraw() )
        {
        return;
        }    
    
    if( iWndControl )
        {
        iWndControl->DrawDeferred();
        }
    }

// ---------------------------------------------------------------------------
// CPopupBubbleCtrl::Move
// Move bubble control
// ---------------------------------------------------------------------------
//
EXPORT_C void CPopupBubbleCtrl::Move( const TPoint& aOffset )
    {
    //leave empty
    }

// ---------------------------------------------------------------------------
// CPopupBubbleCtrl::SetTextFormat
// Set text format used for text drawing
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CPopupBubbleCtrl::SetTextFormat( TAknTextLineLayout aTextFormat )
    {
    iTextFormat = aTextFormat;                
    }

// ---------------------------------------------------------------------------
// CPopupBubbleCtrl::SetTextL
// Set bubble text
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CPopupBubbleCtrl::SetTextL( const TDesC& aText )
    {
    delete iText;
    iText = NULL;
    iText = aText.AllocL();
    }
