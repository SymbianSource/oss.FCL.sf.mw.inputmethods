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
* Description:  Implementation of the common layout data manager
*
*/


// System includes
#include <aknfeppeninputenums.h>
#include <PtiEngine.h>
#include <peninputpluginutils.h>   // Use resource utils,laf env,rep notifier
#include <centralrepository.h>
#include <AknFepInternalCRKeys.h>
#include <settingsinternalcrkeys.h>

// User includes
#include "peninputdatamgr.h"
#include "peninputdataprovider.h"
#include "peninputlayoutcontext.h"


// ======== MEMBER FUNCTIONS ========

// --------------------------------------------------------------------------
// CPeninputDataMgr::NewL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//           
EXPORT_C CPeninputDataMgr* CPeninputDataMgr::NewL( 
    MPeninputLayoutContext* aContext )
    {
    CPeninputDataMgr* self = NewLC( aContext );
    CleanupStack::Pop( self );

    return self;
    }

// --------------------------------------------------------------------------
// CPeninputDataMgr::NewLC
// (other items were commented in a header)
// --------------------------------------------------------------------------
//           
EXPORT_C CPeninputDataMgr* CPeninputDataMgr::NewLC( 
    MPeninputLayoutContext* aContext )
    {
    CPeninputDataMgr* self = new ( ELeave ) CPeninputDataMgr( aContext );
    CleanupStack::PushL( self );
    self->ConstructL(); 

    return self;        
    } 

// --------------------------------------------------------------------------
// CPeninputDataMgr::~CPeninputDataMgr
// (other items were commented in a header)
// --------------------------------------------------------------------------
//           
EXPORT_C CPeninputDataMgr::~CPeninputDataMgr()
    {    
    delete iGSRepositoryWatcher;
    delete iGSRepository;
    delete iAknFepRepository;
    delete iUserDefinedResource;
    } 

// --------------------------------------------------------------------------
// CPeninputDataMgr::CPeninputDataMgr
// (other items were commented in a header).
// --------------------------------------------------------------------------
//      
EXPORT_C CPeninputDataMgr::CPeninputDataMgr( 
    MPeninputLayoutContext* aContext ) 
    : iContext(aContext),
      iUnitWidthSet(EFalse),
      iUnitHeightSet(EFalse)
    {
    }

// --------------------------------------------------------------------------
// CPeninputDataMgr::ConstructL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C void CPeninputDataMgr::ConstructL()
    {
    TInt ret = 0;
    
    // Construct aknfep CenRep
    TRAP( ret, iAknFepRepository = CRepository::NewL( KCRUidAknFep ) );

    // Construct GS CenRep
    TRAP( ret, iGSRepository = 
          CRepository::NewL( KCRUidPersonalizationSettings ) );

    if ( iGSRepository )
        {
        // Read Language
        iGSRepository->Get( KSettingsPenInputLang, iInputLanguage );       
        }
    
    // Add GS Watcher
    if ( ret == KErrNone )
        {
        iGSRepositoryWatcher = CAknFepRepositoryWatcher::NewL( 
            KCRUidPersonalizationSettings, 
            TCallBack( HandleGSRepositoryCallBack, this ),
            iGSRepository );
        }

    // Call init function, which is virutal function
    InitMore();
    }

// ---------------------------------------------------------------------------
// CPeninputDataMgr::IsShiftPermitted
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
EXPORT_C TBool CPeninputDataMgr::IsShiftPermitted()
    {
    if( !iAllowCaseChange && iCurrentRange == ERangeNative )
        {
        return EFalse;
        }
        
    return (iPermittedCase & ECaseText) ||
           (iPermittedCase & ECaseInverseText);
    }
    
// ---------------------------------------------------------------------------
// CPeninputDataMgr::IsCapsLockPermitted
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//  
EXPORT_C TBool CPeninputDataMgr::IsCapsLockPermitted()
    {
    if( !iAllowCaseChange && iCurrentRange == ERangeNative )
        {
        return EFalse;
        }
        
    return iPermittedCase & ECaseUpper;
    }
    
// --------------------------------------------------------------------------
// CPeninputDataMgr::SetData
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C void CPeninputDataMgr::SetData( TPeninputDataType aDataType, 
                                         TAny* aData )
    {
    switch ( aDataType )
        {
        case EPeninputDataTypeCase:
            {
            iCase = *( ( TInt* ) aData );
            }
            break;
        case EPeninputDataTypeInputLanguage:
            {
            iInputLanguage = *( ( TInt* ) aData );
            }
            break;
        case EPeninputDataTypePermittedRange:
            {
            iPermittedRange = *( ( TInt* ) aData );
            }
            break;
        case EAkninputDataTypeAllowCaseChange:
            {
            iAllowCaseChange = *((TBool*)aData);    
            }
            break;
        case EPeninputDataTypePermittedCase:
            {
            iPermittedCase = *( (TInt* ) aData );
            }
            break;
        case EPeninputDataTypeCurrentRange:
            {
            iCurrentRange = *( ( TInt* ) aData );
            }
            break;
        case EPeninputDataTypeClientLayout:
            {
            iClientLayout = *( ( TInt* ) aData );
            }
            break;
        case EPeninputDataTypeVkbLayout:
            {
            iVkbLayoutId = *( ( TInt* ) aData );
            }
            break;
        case EPeninputDataTypeNumericKeymap:
            {
            iEditorNumericKeymap = *( ( TInt* ) aData );
            }
            break;
        case EPeninputDataTypeShiftDown:
            {
            iShiftDown = *( ( TInt* ) aData );
            }
            break;
        case EPeninputDataTypeCapslockDown:
            {
            iCapslockDown = *( ( TInt* ) aData );
            }
            break;
        case EPeninputDataTypeCurrentAccent:
            {
            iCurrentAccent = *( ( TInt* ) aData );
            }
            break;
        case EPeninputDataTypeScreenMode:
            {
            SetScreenMode( *( ( TPixelsTwipsAndRotation* ) aData ) );
            }
            break;
        case EPeninputDataTypeScreenSize:
            {
            iScreenSize = *( ( TSize* ) aData );  
            }
            break;
        case EPeninputDataTypeWindowPosition:
            {
            iWindowPosition = *( ( TInt* ) aData );
            }
            break;
        case EPeninputDataTypeUnitWidth:
            {
            iUnitWidth = *( ( TInt* ) aData );
            iUnitWidthSet = ETrue;
            }
            break;
        case EPeninputDataTypeUnitHeight:
            {
            iUnitHeight = *( ( TInt* ) aData );
            iUnitHeightSet = ETrue;
            }
            break;
        case EPeninputDataTypeDataQueryDlgRect:
            {
            iDataQueryDlgRect = *( ( TRect* ) aData );
            }
            break;        
        case EAknFepDataTypeUseDefinedResource:
            {
            delete iUserDefinedResource;
            TInt* len = (TInt*)((TUint8*)aData - 4);
            TPtrC16 keymapRes((const TUint16*)aData, *len/2);  
            iUserDefinedResource = keymapRes.Alloc();
            }
            break;
        case EPeninputDataTypeReserve_1:
            {
            iReserve_1 = aData;
            }
            break;
        case EAkninputDataTypeSizeChanging:
            {
            iLayoutSizeChanging =  *((TBool*)aData);	
            }
            break;
        case EAkninputDataTypeLatchedSet:
            {
            iLatched = *((TBool*)aData);
            }
            break;
        default:
            break;
        }
        
    NotifyEngine( aDataType, aData ) ;
    } 

    
// --------------------------------------------------------------------------
// CPeninputDataMgr::RequestData
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C TAny* CPeninputDataMgr::RequestData( TPeninputDataType aDataType )  
    {
    switch ( aDataType )
        {
        case EPeninputDataTypeCase:
            return &iCase;
        case EPeninputDataTypeInputLanguage:
            return &iInputLanguage;
        case EPeninputDataTypePermittedRange:
            return &iPermittedRange;
        case EPeninputDataTypePermittedCase:
            return &iPermittedCase;
        case EPeninputDataTypeCurrentRange:
            return &iCurrentRange;
        case EPeninputDataTypeClientLayout:
            return &iClientLayout;
        case EPeninputDataTypeVkbLayout:
            return &iVkbLayoutId;
        case EPeninputDataTypeNumericKeymap:
            return &iEditorNumericKeymap;
        case EPeninputDataTypeShiftDown:
            return &iShiftDown;
        case EPeninputDataTypeCapslockDown:
            return &iCapslockDown;
        case EPeninputDataTypeCurrentAccent:
            return &iCurrentAccent;
        case EPeninputDataTypeScreenMode:
            return &iScreenMode;
        case EPeninputDataTypeScreenSize:
            return &iScreenSize;        
        case EPeninputDataTypeWindowPosition:
            return &iWindowPosition;
        case EPeninputDataTypePositionSettingId:
            return &iPositionSettingId;
        case EPeninputDataTypeUnitWidth:
            return &iUnitWidth;
        case EPeninputDataTypeUnitHeight:
            return &iUnitHeight;
        case EPeninputDataTypeDataQueryDlgRect:
            return &iDataQueryDlgRect;
        case EAknFepDataTypeUseDefinedResource:
            return iUserDefinedResource;
        case EPeninputDataTypeReserve_1:
            return iReserve_1;
        case EAkninputDataTypeGuideLineTop:
            return &iGuideLineTop;
        case EAkninputDataTypeGuideLineBottom:
            return &iGuideLineBottom;
        case EAkninputDataTypeSizeChanging:
            return &iLayoutSizeChanging;
        case EAkninputDataTypeLatchedSet:
            return &iLatched;
        case EAkninputDataTypeUnitSizeSet:
             {
             iUnitSizeSet = (iUnitWidthSet & iUnitHeightSet);
             return &iUnitSizeSet;	
             }
        default:
            break;
        }

    return NULL;
    }

// --------------------------------------------------------------------------
// CPeninputDataMgr::Reset
// (other items were commented in a header)
// --------------------------------------------------------------------------
//           
EXPORT_C void CPeninputDataMgr::Reset()
    { 
    }        

// --------------------------------------------------------------------------
// CPeninputDataMgr::HandleGSRepositoryCallBack
// (other items were commented in a header)
// --------------------------------------------------------------------------
// 
EXPORT_C TInt CPeninputDataMgr::HandleGSRepositoryCallBack( TAny* aPtr )
    {
    CPeninputDataMgr* self = static_cast<CPeninputDataMgr*>( aPtr );
    
    CAknFepRepositoryWatcher* watcher = self->iGSRepositoryWatcher;
    if ( watcher )
        {        
        self->HandleGSRepositoryChange( watcher->ChangedKey() );
        }

    return KErrNone;
    }

// --------------------------------------------------------------------------
// CPeninputDataMgr::SaveKey
// (other items were commented in a header)
// --------------------------------------------------------------------------
// 
EXPORT_C void CPeninputDataMgr::SaveKey( TInt aKey, TInt aValue )
    {
    if ( iAknFepRepository )
        {
        iAknFepRepository->Set( aKey, aValue );
        }
    }

// --------------------------------------------------------------------------
// CPeninputDataMgr::SetScreenMode
// (other items were commented in a header).
// --------------------------------------------------------------------------
// 
void CPeninputDataMgr::SetScreenMode(
    const TPixelsTwipsAndRotation& aScreenMode )
    {
    switch ( aScreenMode.iRotation )
        {
        case CFbsBitGc::EGraphicsOrientationNormal:
            {
            iPositionSettingId = KAknFepHwrWindowPosition0;
            }
            break;
        case CFbsBitGc::EGraphicsOrientationRotated90:
            {
            iPositionSettingId = KAknFepHwrWindowPosition1;
            }
            break;
        case CFbsBitGc::EGraphicsOrientationRotated180:
            {
            iPositionSettingId = KAknFepHwrWindowPosition2;
            }
            break;
        case CFbsBitGc::EGraphicsOrientationRotated270:
            {
            iPositionSettingId = KAknFepHwrWindowPosition3;
            }
            break;
        default:
            break;
        }
        
    iScreenSize = aScreenMode.iPixelSize;    
    iScreenMode = aScreenMode;
    if ( iAknFepRepository )
        {
        iAknFepRepository->Get( iPositionSettingId, iWindowPosition );        
        }               
    }         

// --------------------------------------------------------------------------
// CPeninputDataMgr::InitMore
// (other items were commented in a header)
// --------------------------------------------------------------------------
//   
EXPORT_C void CPeninputDataMgr::InitMore()
    {
    }

// --------------------------------------------------------------------------
// CPeninputDataMgr::HandleGSRepositoryChange
// (other items were commented in a header)
// --------------------------------------------------------------------------
// 
EXPORT_C void CPeninputDataMgr::HandleGSRepositoryChange( TInt /*aChangedKey*/ )
  {
  }

// --------------------------------------------------------------------------
// CPeninputDataMgr::NotifyEngine
// (other items were commented in a header).
// --------------------------------------------------------------------------
// 
EXPORT_C void CPeninputDataMgr::NotifyEngine( TPeninputDataType aType, 
                                              TAny*  aData )
    {
    TInt data = *( (TInt*) aData );
    switch ( aType )
        {
        case EPeninputDataTypeCurrentRange:
            if (iAknFepRepository)
                {
                if ( ( data == ERangeEnglish ) || ( data == ERangeNative ) )
                    {
                    iAknFepRepository->Set(KAknFepLastUsedRange, data );
                    }                
                }          
            break;
        case EPeninputDataTypeWindowPosition:
            if (iAknFepRepository)
                {
                iAknFepRepository->Set(iPositionSettingId, data );
                }
            break;
        default:
            break;
        }
    }

