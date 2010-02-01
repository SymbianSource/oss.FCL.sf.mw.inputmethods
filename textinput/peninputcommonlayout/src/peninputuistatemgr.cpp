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
* Description:  Implementation of the ui state machine manager
*
*/


// User includes
#include "peninputuistatemgr.h"
#include "peninputlayoutcontext.h"
#include "peninputuistatebase.h"


// ======== MEMBER FUNCTIONS ========

// --------------------------------------------------------------------------
// CPeninputUiStateMgr::CPeninputUiStateMgr
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputUiStateMgr::CPeninputUiStateMgr( MPeninputLayoutContext* aContext )
    : iContext( aContext )
    {
    }

// --------------------------------------------------------------------------
// CPeninputUiStateMgr::NewL
// (other items were commented in a header).
// --------------------------------------------------------------------------
//
EXPORT_C CPeninputUiStateMgr* CPeninputUiStateMgr::NewL( 
    MPeninputLayoutContext* aContext )
    {
    CPeninputUiStateMgr* self = NewLC( aContext );
    CleanupStack::Pop( self );

    return self;
    }
    
// --------------------------------------------------------------------------
// CPeninputUiStateMgr::NewLC
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C CPeninputUiStateMgr* CPeninputUiStateMgr::NewLC( 
    MPeninputLayoutContext* aContext )
    {
    CPeninputUiStateMgr* self = 
        new ( ELeave ) CPeninputUiStateMgr( aContext );
    CleanupStack::PushL( self );
    self->Construct(); 
    
    return self;
    }
      
// --------------------------------------------------------------------------
// CPeninputUiStateMgr::~CPeninputUiStateMgr
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C CPeninputUiStateMgr::~CPeninputUiStateMgr()
    {
    iUiStateTypeList.Reset();
    iUiStateTypeList.Close();
    
    iUiStateList.ResetAndDestroy();
    iUiStateList.Close();
    }
    
// --------------------------------------------------------------------------
// CPeninputUiStateMgr::CurrentUiState
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C CPeninputUiStateBase* CPeninputUiStateMgr::CurrentUiState()
    {
    return iCurrentState;
    }

// --------------------------------------------------------------------------
// CPeninputUiStateMgr::GetUiState
// (other items were commented in a header).
// --------------------------------------------------------------------------
EXPORT_C CPeninputUiStateBase* CPeninputUiStateMgr::GetUiState( 
    TInt aUiStateType )                                            
    {
    CPeninputUiStateBase* uiState = NULL;
  
    for ( TInt i = 0; i < iUiStateTypeList.Count(); i++ )
        {
        if ( iUiStateTypeList[i] == aUiStateType )
            {
            uiState = iUiStateList[i];
            break;
            }
        }
    
    return uiState; 
    }
    
// --------------------------------------------------------------------------
// CPeninputUiStateMgr::SetCurrentUiState
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C void CPeninputUiStateMgr::SetCurrentUiState( 
    CPeninputUiStateBase* aUiState )
    {
    iCurrentState = aUiState;
    }

// --------------------------------------------------------------------------
// CPeninputUiStateMgr::SetCurrentUiState
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C void CPeninputUiStateMgr::SetCurrentUiState( TInt aUiStateType )
    {
    iCurrentState = GetUiState( aUiStateType );
    }

// --------------------------------------------------------------------------
// CPeninputUiStateMgr::AddUiState
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C void CPeninputUiStateMgr::AddUiState( CPeninputUiStateBase* aUiState,
                                               TInt aUiStateType )
    {
    iUiStateTypeList.Append( aUiStateType );
    iUiStateList.Append( aUiState );  
    }
    
// --------------------------------------------------------------------------
// CPeninputUiStateMgr::ConstructL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputUiStateMgr::Construct()
    {
    }
