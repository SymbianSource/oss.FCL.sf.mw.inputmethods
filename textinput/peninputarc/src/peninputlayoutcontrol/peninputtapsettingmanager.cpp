/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for CFepUiLayout
*
*/


#include "peninputtapsettingmanager.h"

#include <centralrepository.h>
#include <AknFepGlobalEnums.h>
#include <AknFepInternalCRKeys.h>

_LIT( KSeparatorComma, "," );

const TInt KTextBufferLength = 128;

const TInt KDefaultMovement = 5;
const TInt KDefaultTimeout = 200000;
const TInt KDefaultMargin = 9;

// ---------------------------------------------------------------------------
//  Symbian Constructor
// ---------------------------------------------------------------------------
// 
CPeninputTapSettingManager* CPeninputTapSettingManager::NewL()
    {
    CPeninputTapSettingManager* self = CPeninputTapSettingManager::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
//  Symbian Constructor
// ---------------------------------------------------------------------------
// 
CPeninputTapSettingManager* CPeninputTapSettingManager::NewLC()
    {
    CPeninputTapSettingManager* self = new ( ELeave ) CPeninputTapSettingManager();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
//  c++ destructor
// ---------------------------------------------------------------------------
// 
CPeninputTapSettingManager::~CPeninputTapSettingManager()
    {

    }

// ---------------------------------------------------------------------------
// Load all parameters of tap accuracy enhancement for specified layout
// ---------------------------------------------------------------------------
// 
void CPeninputTapSettingManager::Load( TInt aLayoutType )
    {
    TRAPD( err, LoadFromRepositoryL( aLayoutType ) );
    if ( err != KErrNone )
        {
        LoadDefault();
        }
    }

// ---------------------------------------------------------------------------
// Load all parameters of tap accuracy enhancement for specified layout
// ---------------------------------------------------------------------------
//
void CPeninputTapSettingManager::Load( TInt aLayoutType, const TDesC& aFileName )
    {
    TRAPD( err, LoadFromFileL( aLayoutType, aFileName ) );
    if ( err != KErrNone )
        {
        LoadDefault();
        }
    }

// ---------------------------------------------------------------------------
// Get configuration of PointerMove event suppressor
// ---------------------------------------------------------------------------
//
void CPeninputTapSettingManager::GetPointerMoveSuppressor( TSize& aMaxMovement, 
                                                           TInt& aTimeout )
    {
    aMaxMovement = iPointerMoveMaxMovement;
    aTimeout = iPointerMoveTimeout; 
    }

// ---------------------------------------------------------------------------
// Get configuration of PointerUp event suppressor
// ---------------------------------------------------------------------------
//
void CPeninputTapSettingManager::GetPointerUpSuppressor( TSize& aMaxMovement, 
                                                         TInt& aTimeout )
    {
    aMaxMovement = iPointerUpMaxMovement;
    aTimeout = iPointerUpTimeout;
    }

// ---------------------------------------------------------------------------
// Get extra response area of virtual key controls.
// ---------------------------------------------------------------------------
//
void CPeninputTapSettingManager::GetKeyExtResponseArea( TMargins& aMargins )
    {
    aMargins = iKeyMargins;
    }

// ---------------------------------------------------------------------------
// Get extra response area of button controls.
// ---------------------------------------------------------------------------
//
void CPeninputTapSettingManager::GetButtonExtResponseArea( TMargins& aMargins )
    {
    aMargins = iButtonMargins;
    }

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CPeninputTapSettingManager::CPeninputTapSettingManager()
    {
    
    }

// ---------------------------------------------------------------------------
// Symbian second-phase constructor.
// ---------------------------------------------------------------------------
//
void CPeninputTapSettingManager::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// Load all parameters of tap accuracy enhancement for specified layout.
// From Repository.
// ---------------------------------------------------------------------------
//
void CPeninputTapSettingManager::LoadFromRepositoryL( TInt aLayoutType )
    {
    TUint32 uidButtonExtMargins   = 0;
    TUint32 uidKeyCtrlExtMargins  = 0;
    TUint32 uidPointerMoveMaxMove = 0;
    TUint32 uidPointerMoveTimeout = 0;
    TUint32 uidPointerUpMaxMove   = 0;
    TUint32 uidPointerUpTimeout   = 0;

    switch( aLayoutType )
        {
        case EPluginInputModeFSQ:
            {
            uidButtonExtMargins   = KAknFepTapAccuracyFsqButtonExtMargins;
            uidKeyCtrlExtMargins  = KAknFepTapAccuracyFsqKeyCtrlExtMargins;
            uidPointerMoveMaxMove = KAknFepTapAccuracyFsqPointerMoveMaxMovement;
            uidPointerMoveTimeout = KAknFepTapAccuracyFsqPointerMoveTimeout;
            uidPointerUpMaxMove   = KAknFepTapAccuracyFsqPointerUpMaxMovement;
            uidPointerUpTimeout   = KAknFepTapAccuracyFsqPointerUpTimeout;
            }
            break;
        case EPluginInputModePortraitFSQ:
            {
            uidButtonExtMargins   = KAknFepTapAccuracyPFsqButtonExtMargins;
            uidKeyCtrlExtMargins  = KAknFepTapAccuracyPFsqKeyCtrlExtMargins;
            uidPointerMoveMaxMove = KAknFepTapAccuracyPFsqPointerMoveMaxMovement;
            uidPointerMoveTimeout = KAknFepTapAccuracyPFsqPointerMoveTimeout;
            uidPointerUpMaxMove   = KAknFepTapAccuracyPFsqPointerUpMaxMovement;
            uidPointerUpTimeout   = KAknFepTapAccuracyPFsqPointerUpTimeout;
            }
            break;
        case EPluginInputModeFingerHwr:
            {
            uidButtonExtMargins   = KAknFepTapAccuracyFhwrButtonExtMargins;
            uidKeyCtrlExtMargins  = KAknFepTapAccuracyFhwrKeyCtrlExtMargins;
            uidPointerMoveMaxMove = KAknFepTapAccuracyFhwrPointerMoveMaxMovement;
            uidPointerMoveTimeout = KAknFepTapAccuracyFhwrPointerMoveTimeout;
            uidPointerUpMaxMove   = KAknFepTapAccuracyFhwrPointerUpMaxMovement;
            uidPointerUpTimeout   = KAknFepTapAccuracyFhwrPointerUpTimeout;
            }
            break;            
        default:
            {
            uidButtonExtMargins   = KAknFepTapAccuracyDefaultButtonExtMargins;
            uidKeyCtrlExtMargins  = KAknFepTapAccuracyDefaultKeyCtrlExtMargins;
            uidPointerMoveMaxMove = KAknFepTapAccuracyDefaultPointerMoveMaxMovement;
            uidPointerMoveTimeout = KAknFepTapAccuracyDefaultPointerMoveTimeout;
            uidPointerUpMaxMove   = KAknFepTapAccuracyDefaultPointerUpMaxMovement;
            uidPointerUpTimeout   = KAknFepTapAccuracyDefaultPointerUpTimeout;
            }
        }
    
    CRepository* repository = CRepository::NewL( KCRUidAknFep );
    TBuf<KTextBufferLength> str;
    TInt num = 0;
    TPoint point;
    TMargins margins;
    
    //pointer move event
    repository->Get( uidPointerMoveTimeout, num );
    repository->Get( uidPointerMoveMaxMove, str );
    ParsePoints( point, str );
    
    iPointerMoveTimeout = num;
    iPointerMoveMaxMovement.iWidth = point.iX;
    iPointerMoveMaxMovement.iHeight = point.iY;
    
    //pointer up event
    repository->Get( uidPointerUpTimeout, num );
    repository->Get( uidPointerUpMaxMove, str );
    ParsePoints( point, str );
    
    iPointerUpTimeout = num;
    iPointerUpMaxMovement.iWidth = point.iX;
    iPointerUpMaxMovement.iHeight = point.iY;   
    
    //key control
    repository->Get( uidKeyCtrlExtMargins, str );
    ParseMargins( margins, str );
    iKeyMargins = margins;
    
    //button control
    repository->Get( uidButtonExtMargins, str );
    ParseMargins( margins, str );
    iButtonMargins = margins;
    
    delete repository;
    }

// ---------------------------------------------------------------------------
// Load all parameters of tap accuracy enhancement for specified layout.
// From file.
// ---------------------------------------------------------------------------
//
void CPeninputTapSettingManager::LoadFromFileL( TInt /*aLayoutType*/, 
                                                const TDesC& /*aFileName*/ )
    {
    //reserved
    }

// ---------------------------------------------------------------------------
// Load default parameters.
// ---------------------------------------------------------------------------
//
void CPeninputTapSettingManager::LoadDefault()
    {
    iPointerMoveMaxMovement = TSize( KDefaultMovement, KDefaultMovement );
    iPointerMoveTimeout     = KDefaultTimeout;
    
    iPointerUpMaxMovement   = TSize( KDefaultMovement, KDefaultMovement );
    iPointerUpTimeout       = KDefaultTimeout;
    
    iKeyMargins.iTop        = KDefaultMargin;
    iKeyMargins.iLeft       = KDefaultMargin;
    iKeyMargins.iBottom     = KDefaultMargin;
    iKeyMargins.iRight      = KDefaultMargin;
    
    iButtonMargins.iTop     = KDefaultMargin;
    iButtonMargins.iLeft    = KDefaultMargin;
    iButtonMargins.iBottom  = KDefaultMargin;
    iButtonMargins.iRight   = KDefaultMargin;
    }

// ---------------------------------------------------------------------------
// parse a TPoint from a comma separated values string.
// ---------------------------------------------------------------------------
//
TInt CPeninputTapSettingManager::ParsePoints( TPoint& aPoint, const TDesC& aText )
    {
    TPtrC tokenizer( aText );
    TInt pos = tokenizer.Find( KSeparatorComma );
    if ( pos == KErrNotFound )
        {
        TInt value = 0;
        TLex converter( tokenizer );
        TInt error = converter.Val( value );
        aPoint.iX = ( KErrNone == error ? value : 0 );
        aPoint.iY = aPoint.iX;
        return 0;
        }

    TInt value = 0;
   
    TLex converter( tokenizer.Left( pos ) );
    TInt error = converter.Val( value );
    aPoint.iX = ( KErrNone == error ? value : 0 );
    
    converter = TLex( tokenizer.Mid( pos + 1 ) );
    error = converter.Val( value );
    aPoint.iY = ( KErrNone == error ? value : 0 );
    
    return 0;
    }

// ---------------------------------------------------------------------------
// parse a TMargins from a comma separated values string.
// ---------------------------------------------------------------------------
//
TInt CPeninputTapSettingManager::ParseMargins( TMargins& aMargins, const TDesC& aText )
    {
    RArray<TInt> values;
    
    TPtrC tokenizer( aText );
    TInt pos = 0;
    while ( pos != KErrNotFound )
        {
        pos = tokenizer.Find( KSeparatorComma );
            
        TPtrC substr = ( pos == KErrNotFound ) ? tokenizer : tokenizer.Left( pos );
        TInt value = 0;
        TLex converter( substr );
        TInt error = converter.Val( value );
        values.Append( KErrNone == error ? value : 0 );
        
        if ( pos != KErrNotFound )
            {
            TPtrC right = tokenizer.Mid( pos + 1 );
            tokenizer.Set( right );
            }
        }
    
    TInt count = values.Count();
    
    //one value, applies to all sides
    if(  count == 1  )
        {
        aMargins.iTop    = values[0];
        aMargins.iLeft   = values[0];
        aMargins.iBottom = values[0];
        aMargins.iRight  = values[0];
        }
    
    //two values, first one specifies margins of top and bottom,
    //the second specifies left and right. 
    else if(  count == 2 )
        {
        aMargins.iTop    = values[0];
        aMargins.iLeft   = values[1];
        aMargins.iBottom = values[0];
        aMargins.iRight  = values[1];
        }

    //three values, first is top, second is the left and right, last is bottom. 
    else if(  count == 3 )
        {
        aMargins.iTop    = values[0];
        aMargins.iLeft   = values[1];
        aMargins.iBottom = values[1];
        aMargins.iRight  = values[2];
        }
    
    //four values, specify top,left,bottom and right respectively
    else if ( count >= 4 )
        {
        aMargins.iTop    = values[0];
        aMargins.iLeft   = values[1];
        aMargins.iBottom = values[2];
        aMargins.iRight  = values[3];
        }
    
    else
        {
        aMargins.iTop    = 0;
        aMargins.iLeft   = 0;
        aMargins.iBottom = 0;
        aMargins.iRight  = 0;
        }
    
    values.Close();
    return count;
    }

//end of file
