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
* Description:  control pool
*
*/


// System includes
#include <peninputlayoutbasecontrol.h>

// User includes
#include "peninputctrlpool.h"


// ======== MEMBER FUNCTIONS ========

// ------------------------------------------------------------------------
// CPeninputCtrlPool::NewL
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C CPeninputCtrlPool* CPeninputCtrlPool::NewL()
    {
    CPeninputCtrlPool* self = NewLC();
    CleanupStack::Pop( self );

    return self;
    }
 
// ------------------------------------------------------------------------
// CPeninputCtrlPool::NewLC
// (other items were commented in a header)
// ------------------------------------------------------------------------
//    
EXPORT_C CPeninputCtrlPool* CPeninputCtrlPool::NewLC()
    {
    CPeninputCtrlPool* self = new ( ELeave ) CPeninputCtrlPool();
    CleanupStack::PushL( self );
    self->Construct();

    return self;
    }

// ------------------------------------------------------------------------
// CPeninputCtrlPool::ConstructL
// (other items were commented in a header)
// ------------------------------------------------------------------------
//   
EXPORT_C void CPeninputCtrlPool::Construct() 
    {
    }

// ------------------------------------------------------------------------
// CPeninputCtrlPool::~CPeninputCtrlPool
// (other items were commented in a header)
// ------------------------------------------------------------------------
//         
EXPORT_C CPeninputCtrlPool::~CPeninputCtrlPool()
    {
    iControlList.ResetAndDestroy();
    iControlList.Close();
    }

// ------------------------------------------------------------------------
// CPeninputCtrlPool::AddControl
// (other items were commented in a header)
// ------------------------------------------------------------------------
//    
EXPORT_C void CPeninputCtrlPool::AddControl( CFepUiBaseCtrl* aControl )
    {
    iControlList.Append( aControl );
    }

// ------------------------------------------------------------------------
// CPeninputCtrlPool::RemoveControl
// (other items were commented in a header)
// ------------------------------------------------------------------------
//     
EXPORT_C void CPeninputCtrlPool::RemoveControl( const TInt aControlID ) 
    {
    CFepUiBaseCtrl* ctrl = Control( aControlID );
    const TInt index = iControlList.Find( ctrl );
    delete ctrl;
    iControlList.Remove( index );
    }
    
// ------------------------------------------------------------------------
// CPeninputCtrlPool::Control
// (other items were commented in a header)
// ------------------------------------------------------------------------
//     
EXPORT_C CFepUiBaseCtrl* CPeninputCtrlPool::Control( 
    const TInt aControlID ) const
    {
    const TInt count = iControlList.Count();
    CFepUiBaseCtrl* ctrl = NULL;
    
    for ( TInt i = 0; i < count; i++ )
        {
        if ( iControlList[i]->ControlId() == aControlID )
            {
            ctrl = iControlList[i];
            break;
            }
        }

    return ctrl;
    }

// ------------------------------------------------------------------------
// CPeninputCtrlPool::ControlByIndex
// (other items were commented in a header)
// ------------------------------------------------------------------------
//   
EXPORT_C CFepUiBaseCtrl* CPeninputCtrlPool::ControlByIndex(
    const TInt aIndex ) const
    {
    CFepUiBaseCtrl* ctrl = NULL;
    const TInt count = iControlList.Count();

    if ( ( aIndex < count ) && ( aIndex >= 0 ) )
        {
        ctrl = iControlList[aIndex];
        }

    return ctrl;
    }

// ------------------------------------------------------------------------
// CPeninputCtrlPool::ControlCount
// (other items were commented in a header)
// ------------------------------------------------------------------------
// 
EXPORT_C TInt CPeninputCtrlPool::ControlCount() const
    {
    return iControlList.Count();
    }

// ------------------------------------------------------------------------
// CPeninputCtrlPool::Reset
// (other items were commented in a header)
// ------------------------------------------------------------------------
// 
EXPORT_C void CPeninputCtrlPool::Reset()
    {
    iControlList.ResetAndDestroy();
    }
