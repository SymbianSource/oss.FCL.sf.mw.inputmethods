/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of vkb window class
*
*/

// System includes

// User includes
#include "peninputaknvkbpreviewbubblerenderer.h"
#include "peninputfloatbubble.h"

// Constants
// Modify warning
//const TInt KDefaultBubbleSize = 50;

// Member functions

// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
// 
EXPORT_C CPeninputAknVkbPreviewBubbleRenderer* CPeninputAknVkbPreviewBubbleRenderer::NewL()
    {
    CPeninputAknVkbPreviewBubbleRenderer* self = new( ELeave) CPeninputAknVkbPreviewBubbleRenderer;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor
// ---------------------------------------------------------------------------
//
CPeninputAknVkbPreviewBubbleRenderer::~CPeninputAknVkbPreviewBubbleRenderer()
    {
    delete iBubbleCtrlEx;
    
    iBubbleCtrlEx = NULL;
    }

// ---------------------------------------------------------------------------
// Attach this extension to a virtual keyboard object.
// ---------------------------------------------------------------------------
//
void CPeninputAknVkbPreviewBubbleRenderer::Attach()
    {
    iRefCount++;
    }

// ---------------------------------------------------------------------------
// Detach this extension from a virtual keyboard object.
// ---------------------------------------------------------------------------
//
void CPeninputAknVkbPreviewBubbleRenderer::Detach()
    {
    iRefCount--;
    if ( iRefCount < 1 ) 
        {
        delete this;
        }
    }

// ---------------------------------------------------------------------------
// Enable preview bubble.
// ---------------------------------------------------------------------------
//
void CPeninputAknVkbPreviewBubbleRenderer::ShowBubble( TBool aFlag )
    {
    if ( aFlag && ( NULL == iBubbleCtrlEx ) )
        {
        TRAP_IGNORE( iBubbleCtrlEx = CPeninputFloatingBubble::NewL() );
        }
    }

// ---------------------------------------------------------------------------
// Draw preview bubble.
// ---------------------------------------------------------------------------
//
void CPeninputAknVkbPreviewBubbleRenderer::DrawBubble( CVirtualKey* aKey,  
                                                       const TPoint& aVkbPos, 
                                                       const TPoint& aLayoutPos, 
                                                       const TSize& aBubbleSize  )
    {   
    if ( iBubbleCtrlEx )
        {
        TRect rect = aKey->Rect();
        rect.Move( aVkbPos.iX, aVkbPos.iY );
        rect.Move( aLayoutPos.iX, aLayoutPos.iY ); //now is in screen coord-sys
    
        TInt x = rect.Center().iX - aBubbleSize.iWidth / 2;
        TInt y = rect.iTl.iY - aBubbleSize.iHeight - 5;
    
        TRect rcBubble( TPoint(x,y), aBubbleSize );
        if( aKey->DisplayUnicode() && aKey->DisplayUnicode()->Length() != 0 )
            {
            iBubbleCtrlEx->SetText( *aKey->DisplayUnicode() );        
            }
        else
            {
            iBubbleCtrlEx->SetText( aKey->KeyUnicodes() );            
            }
        
        iBubbleCtrlEx->Show( rcBubble );
        iBubbleCtrlEx->DrawDeferred();
        }    
    }

// ---------------------------------------------------------------------------
// Clear preview bubble.
// ---------------------------------------------------------------------------
//
void CPeninputAknVkbPreviewBubbleRenderer::ClearBubble()
    {
    if ( iBubbleCtrlEx )
        {
        iBubbleCtrlEx->Hide();
        }    
    }

// ---------------------------------------------------------------------------
// Set icon and background of bubble.
// ---------------------------------------------------------------------------
//
void CPeninputAknVkbPreviewBubbleRenderer::SetBubbleBitmapParam( CFbsBitmap* /*aBmpId*/, 
                                                                 CFbsBitmap* /*aMaskBmpId*/, 
                                                                 TAknsItemID aBgSkinId )
    {
    if( iBubbleCtrlEx )
        {
        iBubbleCtrlEx->SetBackgroundImage( aBgSkinId );
        }    
    }

// ---------------------------------------------------------------------------
// Set text format
// ---------------------------------------------------------------------------
//
void CPeninputAknVkbPreviewBubbleRenderer::SetTextFormat( TAknTextLineLayout aTextFormat )
    {
    if( iBubbleCtrlEx )
        {
        iBubbleCtrlEx->SetTextFormat( aTextFormat );
        }    
    }

// ---------------------------------------------------------------------------
// Set margin of bubble
// ---------------------------------------------------------------------------
//
void CPeninputAknVkbPreviewBubbleRenderer::SetFrameDiff( TInt /*aLeftDiff*/, 
                                                         TInt /*aTopDiff*/, 
                                                         TInt /*aRightDiff*/,
                                                         TInt /*aBottomDiff*/ )
    {
    }

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CPeninputAknVkbPreviewBubbleRenderer::CPeninputAknVkbPreviewBubbleRenderer()
    {
    }

// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
void CPeninputAknVkbPreviewBubbleRenderer::ConstructL()
    {
    }
