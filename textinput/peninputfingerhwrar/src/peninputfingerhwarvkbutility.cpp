/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Implementation of virtual key ctrl utility for arabic hwr.
*
*/

// INCLUDE
#include <e32std.h>
#include <barsread.h>
#include <coemain.h>

#include "peninputfingerhwarvkbutility.h"
#include "peninputlayoutvkb.h"
#include "peninputpluginutils.h"
#include "hbufcarrayar.h"
#include "peninputfingerhwrarstoreconstants.h"

// ---------------------------------------------------------------------------
// Load virtual keys image
// ---------------------------------------------------------------------------
//
void PeninputFingerHwrArVkbUtility::LoadVirtualKeypadKeyL(CVirtualKeyboard& aVkbCtrl, const TInt aResId, const RArray<TRect>& aCellRects)        
    {
    aVkbCtrl.SetResourceId(aResId);
	
	TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC( reader, aResId );

    // construct keys
    TInt resKeyCount = reader.ReadInt16();
    TInt existsKeyCount = aVkbCtrl.KeyArray().Count();
    TInt rectCount = aCellRects.Count();
    
    for ( TInt i = 0; i < resKeyCount; i++ )
        {
        if ( i < existsKeyCount )
            {
            CVirtualKey* vk = aVkbCtrl.KeyArray()[i];
            UpdateVkbKeyL( vk, reader, aCellRects[i%rectCount] );
            }
        else
            {
            CVirtualKey* vk = CreateVkbKeyL( reader, aCellRects[i%rectCount] );
            CleanupStack::PushL( vk );
            aVkbCtrl.AddKeyL( vk );
            
            CleanupStack::Pop( vk );
            }
        }
    
    CleanupStack::PopAndDestroy( 1 ); // reader
    
    aVkbCtrl.Draw();
    aVkbCtrl.UpdateArea( aVkbCtrl.Rect() );
    }

// ---------------------------------------------------------------------------
// Create the virtual key
// ---------------------------------------------------------------------------
//
CVirtualKey* PeninputFingerHwrArVkbUtility::CreateVkbKeyL(TResourceReader& aReader,const TRect& aKeyRect)
	{
	CHBufCArray* keytexts = CHBufCArray::NewL();
    CleanupStack::PushL( keytexts );
    
    for ( TInt i = 0; i <= EPosLast; i++ )
        {
        HBufC* unicode = aReader.ReadHBufCL();
        keytexts->Array().AppendL( unicode );
        }

    TInt keyscancode = aReader.ReadInt16();

    HBufC* text = keytexts->Array()[0];
    
    CVirtualKey* vk = NULL;
    if ( text )
        {
        vk = CVirtualKey::NewL( *text, keyscancode, aKeyRect, aKeyRect, 0 );
        }
    else 
        {
        vk = CVirtualKey::NewL( KNullDesC, keyscancode, aKeyRect, aKeyRect, 0 );
        }

    CleanupStack::PopAndDestroy( keytexts ); //keytexts

    
    TRect innerrect = aKeyRect;
    innerrect.Shrink( TSize(10, 10) );
    vk->SetInnerRect( innerrect );
    
    return vk;
	}

// ---------------------------------------------------------------------------
// Load virtual keys image
// ---------------------------------------------------------------------------
//	
void PeninputFingerHwrArVkbUtility::LoadVkbKeyImageL(CVirtualKeyboard& aVkbCtrl, TInt aResId, const TSize& aKeySize)
    {
	TResourceReader reader;    
    CCoeEnv::Static()->CreateResourceReaderLC( reader, aResId );      
    
    TPtrC bmpFileName = reader.ReadTPtrC();
    TInt32 imgMajorSkinId = reader.ReadInt32();
    TAknsItemID id;
    
    TSize keySize = aKeySize;
    
    for ( TInt index = 0; index <= EKeyBmpLastType ; index += 2 )
        { 
        // Get the image ids and mask ids from resource
        TInt bmpId = reader.ReadInt16(); 
        TInt bmpMskId = reader.ReadInt16();
        
        // read skin item id
        const TInt skinitemid = reader.ReadInt16();
        id.Set( TInt( imgMajorSkinId ), skinitemid );
        
        if ( bmpId != KInvalidImg )
            {
            CFbsBitmap* bmp = NULL;
            CFbsBitmap* maskbmp = NULL;

            if ( bmpMskId != KInvalidImg )
                {
                AknsUtils::CreateIconL( AknsUtils::SkinInstance(),
                   id, bmp, maskbmp, bmpFileName, bmpId, bmpMskId );
                
                // set maskbmp and size
                AknIconUtils::SetSize( maskbmp, keySize, EAspectRatioNotPreserved );
                aVkbCtrl.SetNonIrregularKeyBitmapL(TVirtualKeyBmpType(EKeyBmpNormal + index + 1 ),maskbmp);
                }
            else
                {
                AknsUtils::CreateIconL( AknsUtils::SkinInstance(), id, 
                    bmp, bmpFileName, bmpId );
                }
            // set bmp and size
            AknIconUtils::SetSize( bmp, keySize, EAspectRatioNotPreserved );
            aVkbCtrl.SetNonIrregularKeyBitmapL(TVirtualKeyBmpType( EKeyBmpNormal + index ),bmp);
            }       
        }
    // Pop and destroy reader
    CleanupStack::PopAndDestroy( 1 );        
	}

// ---------------------------------------------------------------------------
// update virtual key info.
// ---------------------------------------------------------------------------
//
void PeninputFingerHwrArVkbUtility::UpdateVkbKeyL(CVirtualKey* aVirtualKey, TResourceReader& aReader, const TRect& aKeyRect)
    {
    CHBufCArray* keytexts = CHBufCArray::NewL();
    CleanupStack::PushL( keytexts );
    
    for ( TInt i = 0; i <= EPosLast; i++ )
        {
        HBufC* unicode = aReader.ReadHBufCL();
        keytexts->Array().AppendL( unicode );
        }

    TInt keyscancode = aReader.ReadInt16();

    HBufC* text = keytexts->Array()[0];
    if ( text )
        {
        aVirtualKey->SetKeyData( *text );
        }
    else
        {
        aVirtualKey->SetKeyData( KNullDesC );
        }
    
    aVirtualKey->SetKeyScancode( keyscancode );

    CleanupStack::PopAndDestroy( keytexts ); //keytexts

    aVirtualKey->SetRect(aKeyRect);
    TRect innerrect = aKeyRect;
    innerrect.Shrink( TSize(2,2) );
    aVirtualKey->SetInnerRect( innerrect );
    }
          
//  End Of File
