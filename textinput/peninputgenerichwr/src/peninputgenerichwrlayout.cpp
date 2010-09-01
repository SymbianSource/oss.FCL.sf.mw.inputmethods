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
* Description:  generic HWR layout
*
*/


// System includes
#include <aknfeppeninputenums.h>    // Command from fep or IME to plugin
#include <AknFepGlobalEnums.h>      // Define EPluginInputModeHwr
#include <peninputcmd.h>            // Use global signal
#include <peninputpluginutils.h>    // Use resource utils and laf env
#include <peninputlayoutchoicelist.h>
#include <peninputlayoutbutton.h>
#include <peninputdragbutton.h>
#include <peninputcommonlayoutglobalenum.h>

// User includes
#include "peninputgenerichwr.hrh"
#include "peninputuistatemgrinterface.h"
#include "peninputgenerichwrlayout.h"
#include "peninputgenerichwrwindow.h"
#include "peninputgenerichwrdatamgr.h"
#include "peninputgenerichwruistatestandby.h"
#include "peninputgenerichwruistatepopup.h"


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPeninputGenericHwrLayoutImp::NewL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//    
CPeninputGenericHwrLayoutImp* CPeninputGenericHwrLayoutImp::NewL( 
    TAny* /*aInitParams*/ )
    {
    CPeninputGenericHwrLayoutImp* self = 
        new ( ELeave ) CPeninputGenericHwrLayoutImp();
    CleanupStack::PushL( self );
    self->Construct(); 
    CleanupStack::Pop( self );

    return self;
    }

// ---------------------------------------------------------------------------
// CPeninputGenericHwrLayoutImp::~CPeninputGenericHwrLayoutImp
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
CPeninputGenericHwrLayoutImp::~CPeninputGenericHwrLayoutImp()
    {
    }

// ---------------------------------------------------------------------------
// CPeninputGenericHwrLayoutImp::CPeninputGenericHwrLayoutImp
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
CPeninputGenericHwrLayoutImp::CPeninputGenericHwrLayoutImp()
    {
    }

// ---------------------------------------------------------------------------
// CPeninputGenericHwrLayoutImp::ConstructL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
void CPeninputGenericHwrLayoutImp::Construct()
    {
    }

// ---------------------------------------------------------------------------
// CPeninputGenericHwrLayoutImp::CreateFepUiLayoutL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
CFepUiLayout* CPeninputGenericHwrLayoutImp::CreateFepUiLayoutL(
    MLayoutOwner* aLayoutOwner, const TAny* aData )
    {
    CPeninputGenericHwrLayout* uiLayout = 
        CPeninputGenericHwrLayout::NewL( aLayoutOwner, aData );

    return uiLayout;
    }

// ---------------------------------------------------------------------------
// CPeninputGenericHwrLayout::NewL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
CPeninputGenericHwrLayout* CPeninputGenericHwrLayout::NewL( 
    MLayoutOwner* aLayoutOwner, 
    const TAny* aInitData )
    {
    CPeninputGenericHwrLayout* self = 
        new ( ELeave ) CPeninputGenericHwrLayout( aLayoutOwner );
    CleanupStack::PushL( self );
    self->ConstructL( aInitData );
    CleanupStack::Pop( self );

    return self;  
    }

// ---------------------------------------------------------------------------
// CPeninputGenericHwrLayout::CPeninputGenericHwrLayout
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
CPeninputGenericHwrLayout::CPeninputGenericHwrLayout(
    MLayoutOwner* aLayoutOwner )
    : CPeninputCommonLayout( aLayoutOwner )
    {
    }

// ---------------------------------------------------------------------------
// CPeninputGenericHwrLayout::~CPeninputGenericHwrLayout
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
CPeninputGenericHwrLayout::~CPeninputGenericHwrLayout()
    {    
    }        

// ---------------------------------------------------------------------------
// CPeninputGenericHwrLayout::ConstructL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
void CPeninputGenericHwrLayout::ConstructL( const TAny *aInitData )
    {
    CPeninputCommonLayout::ConstructL( aInitData ); 

	
    CPeninputUiStateBase* initialState = 
        CPeninputUiStateBase::NewL( UiStateMgr(), this );
    CPeninputUiStateHwrStandby* standbyState = 
        CPeninputUiStateHwrStandby::NewL( UiStateMgr(), this );
    CPeninputUiStateHwrPopup* popupState = 
        CPeninputUiStateHwrPopup::NewL( UiStateMgr(), this );

    UiStateMgr()->AddUiState( initialState, EPeninputHwrUiStateNone );
    UiStateMgr()->AddUiState( standbyState, EPeninputHwrUiStateStandby );
    UiStateMgr()->AddUiState( popupState, EPeninputHwrUiStatePopup );
    UiStateMgr()->SetCurrentUiState( initialState );
    }

// ---------------------------------------------------------------------------
// CPeninputGenericHwrLayout::HandleCommand
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
TInt CPeninputGenericHwrLayout::HandleCommand( TInt aCmd, TUint8* aData )
    {
    CPeninputCommonLayout::HandleCommand( aCmd, aData );
    
    if ( aCmd == ECmdPenInputWindowOpen )
        {
        UiStateMgr()->SetCurrentUiState( EPeninputHwrUiStateStandby );
        }
    else if ( aCmd == ECmdPenInputWindowClose )
        {
        UiStateMgr()->SetCurrentUiState( EPeninputHwrUiStateNone );
        }
    else if( aCmd == ECmdPenInputLanguage )
        {
        ((CPeninputGenericHwrWindow*)iLayoutWindow)->OnLanguageChange();
        }
    return KErrNone;    
    }

// ---------------------------------------------------------------------------
// CPeninputGenericHwrLayout::CreateLayoutWindowL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
void CPeninputGenericHwrLayout::CreateLayoutWindowL()
    {
    iLayoutWindow = CPeninputGenericHwrWindow::NewL( this, this );
    }
 
// ---------------------------------------------------------------------------
// CPeninputGenericHwrLayout::CreateDataMgrL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//      
void CPeninputGenericHwrLayout::CreateDataMgrL( const TAny* aInitData )
    {
    TPeninputLayoutData* layoutData = (TPeninputLayoutData*)aInitData;

    iDataMgr = CPeninputGenericHwrDataMgr::NewL( this, (*layoutData).iPtiEngine );
    }    

// ---------------------------------------------------------------------------
// CPeninputGenericHwrLayout::LayoutType
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
TInt CPeninputGenericHwrLayout::LayoutType()
    {
    return EPluginInputModeHwr;
    }

// ---------------------------------------------------------------------------
// CPeninputGenericHwrLayout::SemiTransparencyRequired
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
TBool CPeninputGenericHwrLayout::SemiTransparencyRequired()
    {
    return ETrue;
    }

//End Of File
