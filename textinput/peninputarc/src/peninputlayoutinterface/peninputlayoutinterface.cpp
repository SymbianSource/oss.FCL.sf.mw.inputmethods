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
* Description:  Implementation for CFepUiLayoutInterface
*
*/


#include "peninputlayoutplugininterface.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CFepUiLayoutInterface::NewL
// Factory function
// ---------------------------------------------------------------------------
//
EXPORT_C CFepUiLayoutInterface* CFepUiLayoutInterface::NewL(TInt aInputMethodId)
    {
    // Set up the interface find for the default resolver.
    TUid id;
    id.iUid = aInputMethodId;
    TAny* pFunc = REComSession::CreateImplementationL(id, 
                                                    DestructorUidKeyOffset());
    return REINTERPRET_CAST(CFepUiLayoutInterface*,pFunc);
    }

// ---------------------------------------------------------------------------
// CFepUiLayoutInterface::CFepUiLayoutInterface
// Default constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CFepUiLayoutInterface::CFepUiLayoutInterface()
    {
    //do nothing
    }
// ---------------------------------------------------------------------------
// CFepUiLayoutInterface::~CFepUiLayoutInterface
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CFepUiLayoutInterface::~CFepUiLayoutInterface()
    {
    // Destroy any instance variables and then
    // inform the framework that this specific 
    // instance of the interface has been destroyed.
    REComSession::DestroyedImplementation(iDtor_ID_Key);
    }

// ---------------------------------------------------------------------------
// CFepUiLayoutInterface::SetDestructorKey
// Set destructor UID
// ---------------------------------------------------------------------------
//
EXPORT_C void CFepUiLayoutInterface::SetDestructorKey(TUid aKey)
     {
     iDtor_ID_Key = aKey;
     }

// ---------------------------------------------------------------------------
// CFepUiLayoutInterface::DestructorUidKeyOffset
// Get destructor Uid key offset
// ---------------------------------------------------------------------------
//     
TInt32 CFepUiLayoutInterface::DestructorUidKeyOffset()
    {    
    return _FOFF(CFepUiLayoutInterface,iDtor_ID_Key);
    }
    
 //end of file
