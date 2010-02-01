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
* Description:  common layout config management
*
*/


// System includes
#include <peninputpluginutils.h>
#include <coemain.h>
#include <peninputctrlpool.h>

// User includes
#include "peninputlayoutconfig.h"
#include "peninputrangebarinfo.h"
#include <peninputlayoutvkb.h>
#include "peninputclientlayoutinfo.h"
#include "peninputshiftcapsInfo.h"


// Constants
const TInt KPeninputLayoutConfigInvalidId = -1;


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPeninputLayoutConfig::NewL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
EXPORT_C CPeninputLayoutConfig* CPeninputLayoutConfig::NewL( 
    CAknFepCtrlVkbLayout& aVkblayout, TResourceReader& aReader )
    {
    CPeninputLayoutConfig* self = new ( ELeave ) CPeninputLayoutConfig();
    CleanupStack::PushL( self );
    self->ConstructL( aVkblayout, aReader );
    CleanupStack::Pop( self );
    
    return self;
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutConfig::CPeninputLayoutConfig
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CPeninputLayoutConfig::CPeninputLayoutConfig()
    : iLanguageString( NULL ), iRangeBarInfo( NULL )
    {
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutConfig::ConstructL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputLayoutConfig::ConstructL( CAknFepCtrlVkbLayout& aVkblayout,
                                        TResourceReader& aReader )
    {
    TResourceReader reader;
    TInt count = 0;
    TInt i = 0;

    iLanguage = aReader.ReadInt16();
    iLanguageString = aReader.ReadHBufCL();
    
    if ( aReader.ReadInt16() != 0 )
        {
        iCaseSensitive = ETrue;
        }
    else
        {
        iCaseSensitive = EFalse;
        }

    iGuideLine = aReader.ReadInt16();
    iPositionTopLine = aReader.ReadInt16();
    iPositionBottomLine = aReader.ReadInt16();

    const TInt32 clientLayoutInfoListResId = aReader.ReadInt32();
    const TInt32 vkbLayoutInfoListResId = aReader.ReadInt32();
    const TInt32 rangeBarInfoResId = aReader.ReadInt32();
    const TInt32 shiftCapsInfoResId = aReader.ReadInt32();

    iRangeBarResId = aReader.ReadInt32();
    
    // Create client layout list
    if ( clientLayoutInfoListResId > 0 )
        {
        CCoeEnv::Static()->CreateResourceReaderLC( reader, clientLayoutInfoListResId );

        count = reader.ReadInt16();
        for ( i = 0; i < count; i++ )
            {
            CPeninputClientLayoutInfo* clientLayoutInfo = 
                CPeninputClientLayoutInfo::NewL( reader );
            iClientLayoutInfoList.Append( clientLayoutInfo );
            }
        // Pop and destroy reader
        CleanupStack::PopAndDestroy( 1 );            
        }
        
    // Create vkb layout list 
    if ( vkbLayoutInfoListResId > 0 )
        {     
        aVkblayout.SetResourceId( vkbLayoutInfoListResId );
        aVkblayout.ConstructFromResourceL();
        iVkbLayoutInfoList = aVkblayout.VkbLayoutInfoList();
        }
        
    // Create range bar
    if ( rangeBarInfoResId > 0 )
        {     
        CCoeEnv::Static()->CreateResourceReaderLC( reader, rangeBarInfoResId );        
        iRangeBarInfo = CPeninputRangeBarInfo::NewL( reader );
        // Pop and destroy reader
        CleanupStack::PopAndDestroy( 1 ); 
        }
        
    // Create shift & caps
    if ( shiftCapsInfoResId > 0 )
        {  
        CCoeEnv::Static()->CreateResourceReaderLC( reader, shiftCapsInfoResId );           
        count = reader.ReadInt16();
        for ( i = 0; i < count; i++ )
            {
            CPeninputShiftCapsInfo* shiftCapsInfo = 
                CPeninputShiftCapsInfo::NewL( reader );
            iShiftCapsInfoList.Append( shiftCapsInfo );
            }
        // Pop and destroy reader
        CleanupStack::PopAndDestroy( 1 );
        }
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutConfig::~CPeninputLayoutConfig
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C CPeninputLayoutConfig::~CPeninputLayoutConfig()
    {
    delete iLanguageString;
    delete iRangeBarInfo;
    iClientLayoutInfoList.ResetAndDestroy();
    iShiftCapsInfoList.ResetAndDestroy();
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutConfig::FindClientLayoutInfo
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C CPeninputClientLayoutInfo* 
    CPeninputLayoutConfig::FindClientLayoutInfo( TInt aLayoutId )
    {
    const TInt count = iClientLayoutInfoList.Count();

    for ( TInt i = 0; i < count; i++ )
        {
        if ( iClientLayoutInfoList[i]->LayoutID() == aLayoutId )
            {
            return iClientLayoutInfoList[i];
            }
        }

    return NULL;
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutConfig::FindVkbLayoutInfo
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C CPeninputVkbLayoutInfo*  
    CPeninputLayoutConfig::FindVkbLayoutInfo( TInt aLayoutId )
    {
    const TInt count = iVkbLayoutInfoList.Count();

    for ( TInt i = 0; i < count; i++ )
        {
        if ( iVkbLayoutInfoList[i]->LayoutID() == aLayoutId )
            {
            return iVkbLayoutInfoList[i];
            }
        }

    return NULL;
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutConfig::FindShiftCapsInfo
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C CPeninputShiftCapsInfo*  
    CPeninputLayoutConfig::FindShiftCapsInfo( TInt aRangeId )
    {
    const TInt count = iShiftCapsInfoList.Count();

    for ( TInt i = 0; i < count; i++ )
        {
        if ( iShiftCapsInfoList[i]->RangeId() == aRangeId )
            {
            return iShiftCapsInfoList[i];
            }
        }

    return NULL;
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutConfig::ShiftCapsSingleVkbLayoutId
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPeninputLayoutConfig::ShiftCapsSingleVkbLayoutId( 
    TInt aRangeId, TInt aShiftCapsSingleId )
    {
    CPeninputShiftCapsInfo* shiftCapsInfo = FindShiftCapsInfo( aRangeId );

    if ( !shiftCapsInfo )
        {
        return KPeninputLayoutConfigInvalidId;
        }
    else
        {
        CPeninputShiftCapsInfo::TPeninputShiftCapsSingle* shiftCapsSingle = 
            shiftCapsInfo->FindShiftCapsSingleById( aShiftCapsSingleId );
        if ( shiftCapsSingle )
            {
            return shiftCapsSingle->iVkbLayoutId;
            }
        else
            {
            return KPeninputLayoutConfigInvalidId;    
            }
        }
    }


// ---------------------------------------------------------------------------
// CPeninputLayoutConfig::FindVkbLayoutIdByCase
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPeninputLayoutConfig::ShiftCapsSingleVkbLayoutByCase( 
    TInt aRangeId, TInt aCase )
    {
    CPeninputShiftCapsInfo* shiftCapsInfo = FindShiftCapsInfo( aRangeId );

    if ( !shiftCapsInfo )
        {
        return KPeninputLayoutConfigInvalidId;
        }
    else
        {
        CPeninputShiftCapsInfo::TPeninputShiftCapsSingle* shiftCapsSingle = 
            shiftCapsInfo->FindShiftCapsSingleByCase( aCase );
        if ( shiftCapsSingle )
            {
            return shiftCapsSingle->iVkbLayoutId;
            }
        else
            {
            return KPeninputLayoutConfigInvalidId;    
            }
        }
    }
