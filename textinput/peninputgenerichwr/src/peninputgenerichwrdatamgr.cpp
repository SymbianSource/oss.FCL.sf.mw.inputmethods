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
* Description:  peninput hwr data manager
*
*/


// System includes
#include <peninputdataconverter.h>
#include <centralrepository.h>
#include <settingsinternalcrkeys.h>
#include <peninputlayoutcontext.h>
#include <peninputcommonlayoutglobalenum.h>

// User includes
#include "peninputgenerichwr.hrh"
#include "peninputgenerichwrdatamgr.h"
#include "peninputgenericrecognizer.h"
#include "AknsUtils.h"
#include "peninputpluginutils.h"

// Constants
const TInt KGuideLineOn = 1;
enum
    {
    EHwrWritingSpeedVeryFast    = 300 * 1000,
    EHwrWritingSpeedFast        = 400 * 1000,
    EHwrWritingSpeedNormal      = 550 * 1000,
    EHwrWritingSpeedSlow        = 700 * 1000,
    EHwrWritingSpeedVerySlow    = 1000 * 1000
    };


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPeninputGenericHwrDataMgr::NewL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//    
CPeninputGenericHwrDataMgr* CPeninputGenericHwrDataMgr::NewL( 
    MPeninputLayoutContext* aContext ,CPtiEngine* aPtiEngine)
    {
    CPeninputGenericHwrDataMgr* self = NewLC( aContext ,aPtiEngine );
    CleanupStack::Pop( self );
      
    return self;
    }
    
// ---------------------------------------------------------------------------
// CPeninputGenericHwrDataMgr::NewLC
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//  
CPeninputGenericHwrDataMgr* CPeninputGenericHwrDataMgr::NewLC( 
    MPeninputLayoutContext* aContext ,CPtiEngine* aPtiEngine)
    {
    CPeninputGenericHwrDataMgr* self = 
        new ( ELeave ) CPeninputGenericHwrDataMgr( aContext );
    self->iPtiEngine = aPtiEngine;
    CleanupStack::PushL( self );
    self->ConstructL(); 

    return self;
    }

// ---------------------------------------------------------------------------
// CPeninputGenericHwrDataMgr::CPeninputGenericHwrDataMgr
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//  
CPeninputGenericHwrDataMgr::CPeninputGenericHwrDataMgr( 
    MPeninputLayoutContext* aContext ) 
    : CPeninputDataMgr( aContext )
    {
    }

// ---------------------------------------------------------------------------
// CPeninputGenericHwrDataMgr::~CPeninputGenericHwrDataMgr
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//      
CPeninputGenericHwrDataMgr::~CPeninputGenericHwrDataMgr()
    {
    if( iOwnPtiEngine )
        {
        delete iPtiEngine;
        }
    delete iPenData.iRecognizer; 
    }

// ---------------------------------------------------------------------------
// CPeninputGenericHwrDataMgr::GetPenTrailColor
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//      
TRgb CPeninputGenericHwrDataMgr::GetPenTrailColor()
    {
    TInt newValue = 0; 
    
    iGSRepository->Get( KSettingsPenColor, newValue );
    
    if (!AknPenInputTrailColorUtils::CheckColorInColorTable(newValue))
        {
        newValue = AknPenInputTrailColorUtils::GetTrailColorByTheme();
        }
    return TRgb(newValue);
    }
// ---------------------------------------------------------------------------
// CPeninputGenericHwrDataMgr::InitMore
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//  
void CPeninputGenericHwrDataMgr::InitMore()
    {
    // Read pen data
    if ( iGSRepository )
        {
        TInt newValue;                
        // Read guide line on/off flag, don't remove these code
        newValue = KGuideLineOn;
        iPenData.iGuideLineOn = iGSRepository->Get( KSettingsPenInputGuideLine, newValue );
        iPenData.iGuideLineOn = newValue;
        
        iPenData.iPenColor = GetPenTrailColor();
        
        newValue = 1;
        iGSRepository->Get( KSettingsPenWidth, newValue );        
        iPenData.iPenSize = TSize( newValue, newValue );
        
        newValue = 2;
        iGSRepository->Get( KSettingsWritingSpeed, newValue );        
        iPenData.iFadingSpeed = newValue;
        iPenData.iWritingSpeed = TransferWritingSpeed( newValue );
        }
    
    if( !iPtiEngine )    
        {
        TRAP_IGNORE( iPtiEngine = CPtiEngine::NewL( ETrue ));
        iOwnPtiEngine = ETrue; 
        }
        
    TRAP_IGNORE( iPenData.iRecognizer = CPeninputGenericRecognizer::NewL(iPtiEngine) );
    iPenData.iRecognizer->EnableWordRecognition( ETrue );    
    iReserve_1 = &iPenData;
    }

// ---------------------------------------------------------------------------
// CPeninputGenericHwrDataMgr::HandleGSRepositoryChange
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//  
void CPeninputGenericHwrDataMgr::HandleGSRepositoryChange( TInt aChangedKey )
    {
    TInt newValue = 0;
    
    switch ( aChangedKey )
        {
        case KSettingsPenColor:
        	  {
            iGSRepository->Get( KSettingsPenColor, newValue ); 
            iPenData.iPenColor = newValue;           
            iContext->SendEventToWindow( EPeninputLayoutHwrEventPenColorChange, 
                NULL ); 
            }
            break;
        case KSettingsPenWidth:
        	  {
            iGSRepository->Get( KSettingsPenWidth, newValue );        
            iPenData.iPenSize = TSize( newValue, newValue );            
            iContext->SendEventToWindow( EPeninputLayoutHwrEventPenSizeChange, 
                NULL );
            }
            break; 
        case KSettingsWritingSpeed:
        	  {
            iGSRepository->Get( KSettingsWritingSpeed, newValue );                    
            iPenData.iFadingSpeed = newValue;
            iPenData.iWritingSpeed = TransferWritingSpeed( newValue );
            iContext->SendEventToWindow
                ( EPeninputLayoutHwrEventWritingSpeedChange, NULL );
            }
            break;
        case KSettingsPenInputGuideLine:
        	  {
            iGSRepository->Get( KSettingsPenInputGuideLine, newValue ); 
            if ( newValue != iPenData.iGuideLineOn )
                {
                iPenData.iGuideLineOn = newValue;
                iContext->SendEventToWindow
                    ( EPeninputLayoutHwrEventGuideLineChanged, NULL );
                } 
            //inform engine about then change in guideline.
            if(iPenData.iRecognizer != NULL)
                {
                iPenData.iRecognizer->EnableGuideline(iPenData.iGuideLineOn == 1? ETrue : EFalse);
                }
            }
            break;
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// CPeninputGenericHwrDataMgr::Reset
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//  
void CPeninputGenericHwrDataMgr::Reset()
    {
    }

// ---------------------------------------------------------------------------
// CPeninputGenericHwrDataMgr::TransferWritingSpeed
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//  
TInt CPeninputGenericHwrDataMgr::TransferWritingSpeed( TInt aWritingSpeed )
    {
    TInt speed;
    
    switch ( aWritingSpeed )
        {
        case 1: 
            {
            speed = EHwrWritingSpeedVeryFast;
            }
            break;
        case 2: 
            {
            speed = EHwrWritingSpeedFast;
            }
            break;
        case 3: 
            {
            speed = EHwrWritingSpeedNormal;
            }
            break;
        case 4: 
            {
            speed = EHwrWritingSpeedSlow;
            }
            break;
        case 5:
            { 
            speed = EHwrWritingSpeedVerySlow;
            }
            break;
        default:
            {
            speed = EHwrWritingSpeedNormal;
            }
            break;
        }
        
    return speed;    
    }

// ---------------------------------------------------------------------------
// CPeninputGenericHwrDataMgr::SetData
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
void CPeninputGenericHwrDataMgr::SetData( TPeninputDataType aDataType, 
                                         TAny* aData )
    {
    switch ( aDataType )
        {
        case EAkninputDataTypeInputArea:
            {
            iInputArea = *(TSize*)aData;
            }   
            break;
         default:
            {
            CPeninputDataMgr::SetData( aDataType, aData );
            }
        }
    NotifyEngine( aDataType, aData ) ;    
    }

 
 // ---------------------------------------------------------------------------
// CPeninputGenericHwrDataMgr::NotifyEngine
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
TAny* CPeninputGenericHwrDataMgr::RequestData( TPeninputDataType aDataType )
    {
    switch ( aDataType )
        {
        case EAkninputDataTypeGuideLineTop: 
            {
            iPenData.iRecognizer->GetTopGuideLinePos( iHelpLine );
            return &iHelpLine;
            }
        case EAkninputDataTypeGuideLineBottom:
            {
            iPenData.iRecognizer->GetBottomGuideLinePos( iBaseLine );
            return &iBaseLine;
            }
        case EPeninputDataTypePenTailColor:
            {
            iPenData.iPenColor = GetPenTrailColor();
            return &iPenData.iPenColor;
            }
            
         default:
            {
            return CPeninputDataMgr::RequestData( aDataType );
            }
        }
    }
    
// ---------------------------------------------------------------------------
// CPeninputGenericHwrDataMgr::NotifyEngine
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//      
void CPeninputGenericHwrDataMgr::NotifyEngine( TPeninputDataType aType, 
                                               TAny* aData )
    {
    // Call the function in base class, which will store data to CenRep
    CPeninputDataMgr::NotifyEngine( aType, aData );
      
    TInt* data = ( TInt* ) aData;
    
    // Notify engine, when language, range, case changed
    switch ( aType )
        {
        case EPeninputDataTypeCase: 
            {
            iPenData.iRecognizer->SetCase( *data );
            }
            break;
        case EPeninputDataTypeInputLanguage: 
            {
            iPenData.iRecognizer->SetLanguage( *data );
            }
            break;
        case EPeninputDataTypeCurrentRange: 
            {
            iPenData.iRecognizer->SetMode( *data );
            }
            break;
        case EPeninputDataTypeNumericKeymap:
            {
            iPenData.iRecognizer->SetNumberMode
                ( ( TAknEditorNumericKeymap )*data );
            }
            break;     
        case EAknFepDataTypeUseDefinedResource:
            {
            HBufC* res = (HBufC*)RequestData(EAknFepDataTypeUseDefinedResource);
            iPenData.iRecognizer->SetPermittedCharSet(*res);
            }
            break;
        case EAkninputDataTypeInputArea:
            {
            iPenData.iRecognizer->SetInputAreaSize( *(TSize*)data );
            }   
        break;
        default:
            break;
        }    
    }
//End of file
