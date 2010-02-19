/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Wrapper of HBufC Array.
*
*/


// INCLUDE
#include "hbufcarrayar.h"

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
CHBufCArray* CHBufCArray::NewL()
    {
    CHBufCArray* self = CHBufCArray::NewLC();
    CleanupStack::Pop( self ); // self;
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian Constructor.
// ---------------------------------------------------------------------------
//
CHBufCArray* CHBufCArray::NewLC()
    {
    CHBufCArray* self = new (ELeave) CHBufCArray();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// c++ destructor
// ---------------------------------------------------------------------------
//
CHBufCArray::~CHBufCArray()
    {
    iHBufCArray.ResetAndDestroy();
    iHBufCArray.Close();
    }

// ---------------------------------------------------------------------------
// get array.
// ---------------------------------------------------------------------------
//
RPointerArray<HBufC>& CHBufCArray::Array()
    {
    return iHBufCArray;
    }

// ---------------------------------------------------------------------------
// c++ constructor.
// ---------------------------------------------------------------------------
//
CHBufCArray::CHBufCArray()
    {
    }

// ---------------------------------------------------------------------------
// Symbian second-phase constructor.
// ---------------------------------------------------------------------------
//
void CHBufCArray::ConstructL()
    {
    }

// End Of File
