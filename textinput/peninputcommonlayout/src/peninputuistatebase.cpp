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
* Description:  Implementation of the ui state base
*
*/


// User includes
#include <peninputlayoutvkb.h>
#include <AknFepGlobalEnums.h>      // Define EPluginInputModeHwr
#include <peninputdataconverter.h>
#include "peninputuistatebase.h"
#include "peninputlayoutcontext.h"
#include "peninputcmd.h"
#include "peninputcommonlayoutglobalenum.h"

const TInt KMaxKeyLength = 10;

// ======== MEMBER FUNCTIONS ========

// --------------------------------------------------------------------------
// CPeninputUiStateBase::CPeninputUiStateBase
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C CPeninputUiStateBase::CPeninputUiStateBase( 
    MPeninputUiStateMgr* aUiStateMgr, MPeninputLayoutContext* aContext )
    : iUiStateMgr( aUiStateMgr ), iContext( aContext )
    {
    }
    
// --------------------------------------------------------------------------
// CPeninputUiStateBase::NewL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C CPeninputUiStateBase* CPeninputUiStateBase::NewL( 
    MPeninputUiStateMgr* aUiStateMgr, MPeninputLayoutContext* aContext )
    {
    CPeninputUiStateBase* self = new ( ELeave ) 
        CPeninputUiStateBase( aUiStateMgr, aContext );
    CleanupStack::PushL( self );
    self->Construct(); 
    CleanupStack::Pop( self );
    
    return self;
    }  
    
// --------------------------------------------------------------------------
// CPeninputUiStateBase::ConstructL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C void CPeninputUiStateBase::Construct()
    {
    } 
          
// --------------------------------------------------------------------------
// CPeninputUiStateBase::~CPeninputUiStateBase
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C CPeninputUiStateBase::~CPeninputUiStateBase()
    {    
    }        

// --------------------------------------------------------------------------
// CPeninputUiStateBase::HandleKeyEventL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C TBool CPeninputUiStateBase::HandleKeyEventL( 
    const TRawEvent& /*aData*/ )   
    {
    return EFalse;
    }    

// --------------------------------------------------------------------------
// CPeninputUiStateBase::HandleControlEvent
// (other items were commented in a header).
// --------------------------------------------------------------------------
//
EXPORT_C TBool CPeninputUiStateBase::HandleControlEvent( TInt /*aEventType*/, 
    const TDesC& /*aEventData*/ )
    {
    return EFalse;   
    }

// --------------------------------------------------------------------------
// CPeninputUiStateBase::SendKey
// (other items were commented in a header).
// --------------------------------------------------------------------------
//
EXPORT_C TBool CPeninputUiStateBase::SendKey( const TDesC& aEventData )
    {
    TInt len = aEventData.Length();
    
    if ( iContext->LayoutType() == EPluginInputModeHwr )
        {
        iContext->Sendkey(ESignalKeyEvent,aEventData);
        }
    else
        {
        if ( len == 1)
            {
            // If it is enter, tab, space, back key etc.
            TPtrC ptr = aEventData.Left(1);
            iContext->Sendkey(ESignalKeyEvent,ptr);            
            }
        else if ( len > 1)
            {
            // If it is virtual key
            TVirtualKeyEventData* data = (TVirtualKeyEventData* ) aEventData.Ptr();
            
            TInt lang = CPeninputDataConverter::AnyToInt
                ( iContext->RequestData( EPeninputDataTypeInputLanguage ) );    
            
            TBuf<KMaxKeyLength> trueKeySeries;
                
            if ( lang == ELangArabic )
                {
                TInt len = data->iKeyData.Length();
                for ( TInt i=0;i < len; i++ ) 
                    {
                    trueKeySeries.Append(data->iKeyData[len-i-1]);
                    }        
                } 
            else
                {
                trueKeySeries.Append( data->iKeyData );
                }
                    
            iContext->Sendkey( ESignalKeyEvent, trueKeySeries );             
            }            
        }
        
    return ETrue; 
    }
