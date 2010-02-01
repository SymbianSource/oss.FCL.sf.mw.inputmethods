/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for chinese peninput full screen controlbar background
*
*/

// INCLUDE

#include <aknlayoutscalable_apps.cdl.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <AknLayoutDef.h>
#include <AknUtils.h>
#include <AknsUtils.h>
#include <AknIconUtils.h>
#include <coemain.h>
#include <s32mem.h>
#include <barsread.h> 
#include <peninputhwrfscn.mbg>

#include "peninputhwrfscnbkgnd.h"

const TInt KInvalidImg = -1 ;
const TInt KInvalidResId = -1;

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
CPeninputHwrfscnBkgnd* CPeninputHwrfscnBkgnd::NewL( CFepUiLayout* aLayout, TInt aCtrlId )
    {
    CPeninputHwrfscnBkgnd* self = NewLC( aLayout, aCtrlId );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
CPeninputHwrfscnBkgnd* CPeninputHwrfscnBkgnd::NewLC( CFepUiLayout* aLayout, TInt aCtrlId )
    {
    CPeninputHwrfscnBkgnd* self = new(ELeave) CPeninputHwrfscnBkgnd( aLayout, aCtrlId );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// c++ destructor
// ---------------------------------------------------------------------------
//
CPeninputHwrfscnBkgnd::~CPeninputHwrfscnBkgnd()
    {
    
    }
    
// ---------------------------------------------------------------------------
// Draw window.
// ---------------------------------------------------------------------------
//
void CPeninputHwrfscnBkgnd::Draw()
    {
    if( !AbleToDraw() )
        {
        return;
        }
    
    CFbsBitGc* gc = static_cast<CFbsBitGc*>( BitGc() );
    gc->Activate( BitmapDevice() );
    gc->SetBrushStyle( CGraphicsContext::ESolidBrush );
    gc->SetBrushColor( BkColor() );
    gc->SetPenStyle(CGraphicsContext::ENullPen );
    gc->SetPenSize( TSize(0,0) );
    
    for ( TInt i = 0; i < iCtrlList.Count(); i++ )        
        {
        CFepUiBaseCtrl* ctrl = iCtrlList[i];
        
        if( !ctrl->Hiden() )
            {
            TRect rect = ctrl->Rect();
            if ( rect.Height() > 0 && rect.Width() > 0  )
                {
                if ( BackgroundBmp() )
                    {
                    gc->DrawBitmap( rect, BackgroundBmp() );
                    }
                else
                    {
                    gc->DrawRect( rect );
                    }
                }
            }
        }
    
    CControlGroup::Draw();
    }

// ---------------------------------------------------------------------------
// Construct control from resource.
// ---------------------------------------------------------------------------
//
void CPeninputHwrfscnBkgnd::ConstructFromResourceL()
    {
    CControlGroup::ConstructFromResourceL();
    
    if ( iResourceId == KInvalidResId )
        {
        User::Leave( KErrArgument );
        }    
    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC( reader, iResourceId );
    
    TPtrC bmpFileName = reader.ReadTPtrC();
    TInt32 imgMajorSkinId = reader.ReadInt32();

    TAknsItemID id;
    
    MAknsSkinInstance* skininstance = AknsUtils::SkinInstance();
    const TInt16 bmpId = reader.ReadInt16();
    const TInt16 bmpMskId = reader.ReadInt16();
    const TInt16 skinitemid = reader.ReadInt16();   
    id.Set( TInt(imgMajorSkinId), skinitemid );
    
    if ( bmpId != KInvalidImg )
        {
        CFbsBitmap* bmpImage = NULL;
        CFbsBitmap* bmpMask = NULL;
        if ( bmpMskId != KInvalidImg )
            {
            AknsUtils::CreateIconL( skininstance, 
                                    id,
                                    bmpImage,
                                    bmpMask,
                                    bmpFileName,
                                    bmpId,
                                    bmpMskId);
            }
        else
            {
            AknsUtils::CreateIconL( skininstance,
                                    id,
                                    bmpImage,
                                    bmpFileName,
                                    bmpId);
            }
        TSize size( 8, 8 );
        AknIconUtils::SetSize( bmpImage, size, EAspectRatioNotPreserved );
        AknIconUtils::SetSize( bmpMask, size, EAspectRatioNotPreserved );
        
        SetBackgroundBmp( bmpImage );
        SetBackgroundMaskBmp( bmpMask ); 
        }
    
    CleanupStack::PopAndDestroy(); // reader
    
    if ( !iFirstTimeConstruct )
        {
        Draw();
        }
    
    iFirstTimeConstruct = EFalse;
    }

// ---------------------------------------------------------------------------
// c++ constructor
// ---------------------------------------------------------------------------
//
CPeninputHwrfscnBkgnd::CPeninputHwrfscnBkgnd( CFepUiLayout* aLayout, TInt aCtrlId )
    : CControlGroup( aLayout, aCtrlId ), iFirstTimeConstruct( ETrue )
    {
    
    }

// ---------------------------------------------------------------------------
// Symbian second-phase constructor
// ---------------------------------------------------------------------------
//
void CPeninputHwrfscnBkgnd::ConstructL()
    {
    BaseConstructL();
    }
