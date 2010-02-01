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
 * Description: implementation smiley classes 
 *
 */

#include <AknIconUtils.h> 
#include <barsread.h>
#include <coemain.h>
#include <smiley.mbg>
#include <smiley.rsg>
#include "peninputlayoutsmiley.h"

//resource file
_LIT( KSmileyMifFile, "z:\\resource\\apps\\smiley.mif" );
_LIT( KSmileyResFile, "z:\\resource\\smiley.rsc" );

//string to calculate smiley width
_LIT( KSmileySizeTemplate, "\xf8e4" );

//invisible place holder for smiley character
const TUint16 KSmileyPlaceHolder = 0x3000;
const TInt KDefaultSmileySize = 24;

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
CPeninputSmileyImage* CPeninputSmileyImage::NewL( TInt aSmileyId, TInt aBitmapId, 
                                                  TInt aMaskId, const TSize& aImageSize )
    {
    CPeninputSmileyImage* self = CPeninputSmileyImage::NewLC( aSmileyId, aBitmapId, 
                                                              aMaskId, aImageSize );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
CPeninputSmileyImage* CPeninputSmileyImage::NewLC( TInt aSmileyId, TInt aBitmapId, 
                                                   TInt aMaskId, const TSize& aImageSize )
    {
    CPeninputSmileyImage* self = new ( ELeave ) CPeninputSmileyImage( aSmileyId, 
                                                   aBitmapId, aMaskId,aImageSize );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// c++ destructor
// ---------------------------------------------------------------------------
//
CPeninputSmileyImage::~CPeninputSmileyImage()
    {
    delete iImageBitmap;
    delete iMaskBitmap;
    }

// ---------------------------------------------------------------------------
// Create smiley image
// ---------------------------------------------------------------------------
//
void CPeninputSmileyImage::CreateImageL()
    {
    DestroyImage();
                
    TInt maskid = iMaskBitmapId != 0 ? iMaskBitmapId : iImageBitmapId;
    
    AknIconUtils::CreateIconL( iImageBitmap, iMaskBitmap, 
                               KSmileyMifFile, iImageBitmapId, maskid );
   
    AknIconUtils::SetSize( iImageBitmap, iImageSize );
    }

// ---------------------------------------------------------------------------
// Destroy smiley image
// ---------------------------------------------------------------------------
//
void CPeninputSmileyImage::DestroyImage()
    {
    if ( iImageBitmap )
        {
        delete iImageBitmap;
        iImageBitmap = NULL;
        }
    
    if ( iMaskBitmap )
        {
        delete iMaskBitmap;
        iMaskBitmap = NULL;
        }    
    }

// ---------------------------------------------------------------------------
// Get image bitmap of smiley
// ---------------------------------------------------------------------------
//
CFbsBitmap* CPeninputSmileyImage::ImageBitmap()
    {
    return iImageBitmap;
    }

// ---------------------------------------------------------------------------
// Get mask bitmap of smiley
// ---------------------------------------------------------------------------
//
CFbsBitmap* CPeninputSmileyImage::MaskBitmap()
    {
    return iMaskBitmap;
    }

// ---------------------------------------------------------------------------
// Get smiley id
// ---------------------------------------------------------------------------
//
TInt CPeninputSmileyImage::SmileyId()
    {
    return iSmileyId;
    }

// ---------------------------------------------------------------------------
// Get smiley size
// ---------------------------------------------------------------------------
//
TSize CPeninputSmileyImage::SmileyImageSize()
    {
    return iImageSize;
    }

// ---------------------------------------------------------------------------
// Set smiley size
// ---------------------------------------------------------------------------
//
void CPeninputSmileyImage::SetImageSize( const TSize& aSize )
    {
    if ( aSize != iImageSize )
        {
        iImageSize = aSize;
        if ( iImageBitmap )
            {
            AknIconUtils::SetSize( iImageBitmap, aSize );
            }     
        }
    }

// ---------------------------------------------------------------------------
// c++ constructor
// ---------------------------------------------------------------------------
//
CPeninputSmileyImage::CPeninputSmileyImage( TInt aSmileyId, TInt aBitmapId, 
                                            TInt aMaskId, const TSize& aImageSize ) 
    : iSmileyId( aSmileyId ) 
    , iImageBitmapId( aBitmapId ) 
    , iMaskBitmapId( aMaskId )
    , iImageSize( aImageSize )
    {
    }


// ---------------------------------------------------------------------------
// Symbian second-phase constructor
// ---------------------------------------------------------------------------
//
void CPeninputSmileyImage::ConstructL()
    {
    }

//////////////////////////////////////////////////////////////////////////////
//CPeninputSmileyManager

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
CPeninputSmileyManager* CPeninputSmileyManager::NewL()
    {
    CPeninputSmileyManager* self = CPeninputSmileyManager::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
CPeninputSmileyManager* CPeninputSmileyManager::NewLC()
    {
    CPeninputSmileyManager* self = new ( ELeave ) CPeninputSmileyManager;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor
// ---------------------------------------------------------------------------
//
CPeninputSmileyManager::~CPeninputSmileyManager()
    {
    iSmileyArray.ResetAndDestroy();
    iSmileyArray.Close();
    
    iSmileyHash.Close();
    }

// ---------------------------------------------------------------------------
// Get smiley object by id
// ---------------------------------------------------------------------------
//
CPeninputSmileyImage* CPeninputSmileyManager::SmileyImage( TInt aSmileyId )
    {
    TInt key = aSmileyId;
    CPeninputSmileyImage** ptr = iSmileyHash.Find( key );
    if ( ptr )
        {
        return *ptr;
        }
    return NULL;
    }

// ---------------------------------------------------------------------------
// Detech if Specified character is smiley
// ---------------------------------------------------------------------------
//
TBool CPeninputSmileyManager::IsSmileyCode( TUint16 aCharCode )
    {
    return ( aCharCode >= 0xf880 && aCharCode <= 0xf8e4 );
    }

// ---------------------------------------------------------------------------
// Get smiley template
// ---------------------------------------------------------------------------
//
const TDesC& CPeninputSmileyManager::SmileySizeTemplate()
    {
    return KSmileySizeTemplate;
    }

// ---------------------------------------------------------------------------
// Get invisible character as place holder
// ---------------------------------------------------------------------------
//
TUint16 CPeninputSmileyManager::SmileyPlaceHolderCharacter()
    {
    return KSmileyPlaceHolder;
    }

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CPeninputSmileyManager::CPeninputSmileyManager()
    {
    }

// ---------------------------------------------------------------------------
// Symbian second-phase constructor
// ---------------------------------------------------------------------------
//
void CPeninputSmileyManager::ConstructL()
    {
    TInt id = CCoeEnv::Static()->AddResourceFileL( KSmileyResFile );
    ReadSmileyInfoL( R_SMILEY_ICONS_INFO );
    CCoeEnv::Static()->DeleteResourceFile( id );

    PreLoadSmileyImageL();
    }

// ---------------------------------------------------------------------------
// Read smiley info from resource
// ---------------------------------------------------------------------------
//
void CPeninputSmileyManager::ReadSmileyInfoL( TInt aResourceId )
    {
    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC( reader, aResourceId );//resId

    TInt count = reader.ReadInt16();

    CPeninputSmileyImage* smiley = NULL;
    for ( TInt i = 0; i < count; i++ )
        {
        TUint16 animation = reader.ReadUint16(); // read animation
        
        TUint16 smileyId = reader.ReadUint16();
        
        TInt32 bitmapId = reader.ReadInt32();
        TInt32 maskId = reader.ReadInt32();
        TInt32 staticBitmapId = reader.ReadInt32();
        TInt32 staticMaskId = reader.ReadInt32();
        
        TPtrC smileycode = reader.ReadTPtrC(); // read string

        TSize size( KDefaultSmileySize, KDefaultSmileySize );
        if ( animation )
            {
            smiley = CPeninputSmileyImage::NewL( smileyId, staticBitmapId, 
                                                 staticMaskId, size );
            }
        else
            {
            smiley = CPeninputSmileyImage::NewL( smileyId, bitmapId, maskId,
                                                 size );            
            }
        iSmileyArray.Append( smiley );
        
        iSmileyHash.Insert( smileyId, smiley );
        }

    CleanupStack::PopAndDestroy(); // reader
    }

// ---------------------------------------------------------------------------
// Initalize all smily images
// ---------------------------------------------------------------------------
//
void CPeninputSmileyManager::PreLoadSmileyImageL()
    {
    for ( TInt i = 0; i < iSmileyArray.Count(); i++ )
        {
        iSmileyArray[i]->CreateImageL();
        }
    }
