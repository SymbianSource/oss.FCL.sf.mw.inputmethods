/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  vkb control extension
*
*/


// System includes
#include <peninputvkbctrllayout.h>   // CPeninputVkbLayoutInfo

// user includes
#include "peninputvkbctrlextjp.h"

// ======== MEMBER FUNCTIONS ========

// --------------------------------------------------------------------------
// CPeninputVkbCtrlExtJp::NewL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputVkbCtrlExtJp* CPeninputVkbCtrlExtJp::NewL( const TRect& aRect,
    CFepUiLayout* aUiLayout, TInt aControlId, const TFontSpec& aFontSpec )
    {
    CPeninputVkbCtrlExtJp* self = new ( ELeave ) CPeninputVkbCtrlExtJp
        ( aRect, aUiLayout, aControlId, aFontSpec );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// --------------------------------------------------------------------------
// CPeninputVkbCtrlExtJp::NewL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputVkbCtrlExtJp* CPeninputVkbCtrlExtJp::NewL( const TRect& aRect,
    CFepUiLayout* aUiLayout, TInt aControlId, const CFont* aFont )
    {
    TFontSpec fontSpec;

    CPeninputVkbCtrlExtJp* vkb = new (ELeave)
        CPeninputVkbCtrlExtJp( aRect, aUiLayout, aControlId, fontSpec );
    CleanupStack::PushL( vkb );
    vkb->ConstructL();
    vkb->SetFont( aFont );
    CleanupStack::Pop( vkb );

    return vkb;
    }

// --------------------------------------------------------------------------
// CPeninputVkbCtrlExtJp::~CPeninputVkbCtrlExtJp
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputVkbCtrlExtJp::~CPeninputVkbCtrlExtJp()
    {
    }

// ---------------------------------------------------------------------------
// CPeninputVkbCtrlExtJp::SetDimmed
// Dim KeyBoard
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputVkbCtrlExtJp::SetDimmed(TBool aDimFlag) const
    {
    TInt count = iVkbLayoutInfo->KeyInfoList().Count();
    for (TInt ii=0; ii < count; ii++)
        {
        iVkbLayoutInfo->KeyInfoList()[ii]->Key()->SetDimmed(aDimFlag);
        }
    }

// --------------------------------------------------------------------------
// CPeninputVkbCtrlExtJp::CPeninputVkbCtrlExtJp
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputVkbCtrlExtJp::CPeninputVkbCtrlExtJp( const TRect& aRect,
    CFepUiLayout* aUiLayout, TInt aControlId, const TFontSpec& aFontSpec )
    : CPeninputVkbCtrlExt( aRect, aUiLayout, aControlId, aFontSpec )
    {
    }

// End Of File
