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
* Description:  number keymapping manager
*
*/


// System includes
#include <eikon.hrh>

// User includes
#include "peninputnumerickeymappingmgr.h"

// Constants
_LIT( KNumberString, "0123456789" );
const TInt KMaxNumericString = 32;


// ======== MEMBER FUNCTIONS ========

// ------------------------------------------------------------------------
// CPeninputNumericKepMappingMgr::NewL
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C CPeninputNumericKepMappingMgr* CPeninputNumericKepMappingMgr::NewL(
    TResourceReader aReader )
    {
    CPeninputNumericKepMappingMgr* self = NewLC( aReader );
    CleanupStack::Pop( self );

    return self;        
    }
 
// ------------------------------------------------------------------------
// CPeninputNumericKepMappingMgr::NewLC
// (other items were commented in a header)
// ------------------------------------------------------------------------
//    
EXPORT_C CPeninputNumericKepMappingMgr* CPeninputNumericKepMappingMgr::NewLC(
    TResourceReader aReader )
    {
    CPeninputNumericKepMappingMgr* self = 
        new ( ELeave ) CPeninputNumericKepMappingMgr();
    CleanupStack::PushL( self );
    self->ConstructL( aReader );

    return self;        
    }

// ------------------------------------------------------------------------
// CPeninputNumericKepMappingMgr::ConstructL
// (other items were commented in a header)
// ------------------------------------------------------------------------
//   
void CPeninputNumericKepMappingMgr::ConstructL( TResourceReader aReader ) 
    {
    const TInt count = aReader.ReadInt16();
    
    for ( TInt i = 0; i < count; i++ )
        {
        iKeyMappingIdList.Append( aReader.ReadInt16() );
        TPtrC ptr = aReader.ReadTPtrC();
        iKeyMappingStringList.Append( ptr.AllocL() );
        }
    }

// ------------------------------------------------------------------------
// CPeninputNumericKepMappingMgr::~CPeninputNumericKepMappingMgr
// (other items were commented in a header)
// ------------------------------------------------------------------------
//         
EXPORT_C CPeninputNumericKepMappingMgr::~CPeninputNumericKepMappingMgr()    
    {
    iKeyMappingIdList.Reset();
    iKeyMappingIdList.Close();
    
    iKeyMappingStringList.ResetAndDestroy();
    iKeyMappingStringList.Close();
    }

// ------------------------------------------------------------------------
// CPeninputNumericKepMappingMgr::KeyMappingStringL
// (other items were commented in a header)
// ------------------------------------------------------------------------
//   
EXPORT_C HBufC* CPeninputNumericKepMappingMgr::KeyMappingStringL( 
    const TInt aKeyMappingId ) const
    {
    TLocale locale;
    TBuf<KMaxNumericString> rs = KNumberString();
    const TInt index = iKeyMappingIdList.Find( aKeyMappingId );
    
    if ( index != KErrNotFound )
        {
        HBufC* buf = iKeyMappingStringList[index];
        
        rs.Append( *buf );
        
        if ( ( aKeyMappingId == EAknEditorCalculatorNumberModeKeymap )
            || ( aKeyMappingId == EAknEditorConverterNumberModeKeymap ) )
            {
            rs.Append( locale.DecimalSeparator() ); 
            }       
        }
        
    return rs.AllocL();          
    }

// ------------------------------------------------------------------------
// CPeninputNumericKepMappingMgr::ArabicKeyMappingStringL
// (other items were commented in a header)
// ------------------------------------------------------------------------
//       
EXPORT_C HBufC* CPeninputNumericKepMappingMgr::KeyMappingStringL( 
    const TInt aKeyMappingId, const TInt aLang ) const
    {
    
    TLocale locale;
    TBuf<KMaxNumericString> rs;
    if( aLang == ELangArabic )
        {
        rs.Format( _L("%c%c%c%c%c%c%c%c%c%c"),
        0x0660, 0x0661, 0x0662,
        0x0663, 0x0664, 0x0665,
        0x0666, 0x0667, 0x0668, 0x0669 );
        }
    else if( aLang == ELangFarsi || aLang == ELangUrdu )
        {
        rs.Format( _L("%c%c%c%c%c%c%c%c%c%c"),
        0x06F0, 0x06F1, 0x06F2,
        0x06F3, 0x06F4, 0x06F5,
        0x06F6, 0x06F7, 0x06F8, 0x06F9 );
        }
    else if( aLang == ELangHindi )
        {
        rs.Format( _L("%c%c%c%c%c%c%c%c%c%c"),
        0x0966, 0x0967, 0x0968,
        0x0969, 0x096a, 0x096b,
        0x096c, 0x096d, 0x096e, 0x096f );
        }
    else if( aLang == ELangThai )
        {
        rs.Format( _L("%c%c%c%c%c%c%c%c%c%c"),
        0x0E50, 0x0E51, 0x0E52,
        0x0E53, 0x0E54, 0x0E55,
        0x0E56, 0x0E57, 0x0E58, 0x0E59 );        
        }
    else 
        {
        rs = KNumberString();   
        }
    
    const TInt index = iKeyMappingIdList.Find( aKeyMappingId );
    
    if ( index != KErrNotFound )
        {
        HBufC* buf = iKeyMappingStringList[index];
        
        rs.Append( *buf );
        
        if ( ( aKeyMappingId == EAknEditorCalculatorNumberModeKeymap )
            || ( aKeyMappingId == EAknEditorConverterNumberModeKeymap ) )
            {
            rs.Append( locale.DecimalSeparator() ); 
            }       
        }
        
    return rs.AllocL();          
    }

// ------------------------------------------------------------------------
// CPeninputNumericKepMappingMgr::Count
// (other items were commented in a header)
// ------------------------------------------------------------------------
// 
EXPORT_C TInt CPeninputNumericKepMappingMgr::Count() const
    {
    return iKeyMappingIdList.Count();
    }

// ------------------------------------------------------------------------
// CPeninputNumericKepMappingMgr::Reset
// (other items were commented in a header)    
// ------------------------------------------------------------------------
// 
EXPORT_C void CPeninputNumericKepMappingMgr::Reset()
    {
    iKeyMappingIdList.Reset();
    iKeyMappingStringList.ResetAndDestroy();
    }
