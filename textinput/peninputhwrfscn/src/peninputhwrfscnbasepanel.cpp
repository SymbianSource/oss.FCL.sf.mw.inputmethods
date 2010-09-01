/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for chinese peninput full screen hwr
*
*/


#include "peninputhwrfscnlayout.h"
#include "peninputhwrfscnbasepanel.h"

// ============================ MEMBER FUNCTIONS ===============================
  
// -----------------------------------------------------------------------------
// CPeninputHwrfscnBasePanel::~CPeninputHwrfscnBasePanel()
// .
// -----------------------------------------------------------------------------
//
CPeninputHwrfscnBasePanel::~CPeninputHwrfscnBasePanel()   
    {
    iEventObserverList.Reset();
    iEventObserverList.Close();
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnBasePanel::AddEventObserver()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnBasePanel::AddEventObserver( MEventObserver* aObserver )
    {
    if( !aObserver )
        return;
    //add only when it's not an observer 
    if( KErrNotFound == iEventObserverList.Find( aObserver ) )
        {
        iEventObserverList.Append( aObserver );
        }        
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnBasePanel::RemoveEventObserver()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnBasePanel::RemoveEventObserver( MEventObserver* aObserver )
    {
    TInt index = iEventObserverList.Find( aObserver );
    if( KErrNotFound != index )
        {
        iEventObserverList.Remove( index );
        }        
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnBasePanel::SetPostion()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnBasePanel::SetPostion( TPoint aPos )
    {
    iPos = aPos;
    SizeChanged();
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnBasePanel::IsVisible()
// .
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrfscnBasePanel::IsVisible()
    {
    return iVisible;
    }
        
// -----------------------------------------------------------------------------
// CPeninputHwrfscnBasePanel::AddToRootControl()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnBasePanel::AddToRootControlL( CFepUiBaseCtrl* aCtrl )
	{
	iLayout->AddControlL( aCtrl, EFalse );	
	}

// -----------------------------------------------------------------------------
// CPeninputHwrfscnBasePanel::CPeninputHwrfscnBasePanel()
// .
// -----------------------------------------------------------------------------
//	
CPeninputHwrfscnBasePanel::CPeninputHwrfscnBasePanel( 
    CPeninputHwrfscnLayout * aLayout )
    : iVisible( EFalse ),iLayout( aLayout )
    {
    }
    
// -----------------------------------------------------------------------------
// CPeninputHwrfscnBasePanel::ReportEvent()
// .
// -----------------------------------------------------------------------------
//	
void CPeninputHwrfscnBasePanel::ReportEvent( TInt aEventType, 
    CFepUiBaseCtrl* aCtrl, 
	const TDesC& aEventData )
    {
    for( TInt i = 0; i < iEventObserverList.Count(); i++ )
        {
        iEventObserverList[i]->HandleControlEvent( 
            aEventType, aCtrl, aEventData );
        }
    }

//end of file
