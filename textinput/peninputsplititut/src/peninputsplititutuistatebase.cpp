/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/
#include "peninputsplititutuistatebase.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CSplitItutUiStateBase::~CSplitItutUiStateBase
// ---------------------------------------------------------------------------
//
CSplitItutUiStateBase::~CSplitItutUiStateBase()
    {
    }

// ---------------------------------------------------------------------------
// CSplitItutUiStateBase::OnExit
// ---------------------------------------------------------------------------
//
void CSplitItutUiStateBase::OnExit()
    {
    }

// ---------------------------------------------------------------------------
// CSplitItutUiStateBase::OnEntryL
// ---------------------------------------------------------------------------
//
void CSplitItutUiStateBase::OnEntryL()
    {
    }

// ---------------------------------------------------------------------------
// CSplitItutUiStateBase::HandleCtrlEventL
// ---------------------------------------------------------------------------
//
TBool CSplitItutUiStateBase::HandleCtrlEventL(TInt /*aEventType*/, 
                                                CFepUiBaseCtrl* /*aCtrl*/, 
                                                const TDesC& /*aEventData*/)
    {
    return EFalse;
    }

// ---------------------------------------------------------------------------
// CSplitItutUiStateBase::CSplitItutUiStateBase
// ---------------------------------------------------------------------------
//
CSplitItutUiStateBase::CSplitItutUiStateBase(CSplitItutUiMgrBase* aOwner)
                                                :iOwner(aOwner)
    {
    }

// ---------------------------------------------------------------------------
// CSplitItutUiStateBase::ConstructL
// ---------------------------------------------------------------------------
//
void CSplitItutUiStateBase::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// CSplitItutUiStateBase::StateType
// ---------------------------------------------------------------------------
//
CSplitItutUiMgrBase::TUiState CSplitItutUiStateBase::StateType()
    {
    return CSplitItutUiMgrBase::EStateNull;
    }

// ---------------------------------------------------------------------------
// CSplitItutUiStateBase::HandleKeyL
// ---------------------------------------------------------------------------
//
TBool CSplitItutUiStateBase::HandleKeyL(TInt /*aCmd*/, TInt /*aKey*/)
    {
    return EFalse;
    }
    
// ---------------------------------------------------------------------------
// CSplitItutUiStateBase::HandleCommandL
// ---------------------------------------------------------------------------
//
TInt CSplitItutUiStateBase::HandleCommandL(TInt /*aCmd*/, TUint8* /*aData*/)
    {
    return KErrNotSupported;
    }
    
// End Of File
