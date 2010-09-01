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
#include "peninputsplititutuimgrbase.h"
#include "peninputsplititutlayoutcontext.h"
#include "peninputsplititutdatamgr.h"
#include "peninputsplititutuistatebase.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------CSplitItutUiMgrBase----------------------------------------
// CSplitItutUiMgrBase::~CSplitItutUiMgrBase
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CSplitItutUiMgrBase::~CSplitItutUiMgrBase()
    {
    }

// ---------------------------------------------------------------------------
// CSplitItutUiMgrBase::HandleCommandL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TInt CSplitItutUiMgrBase::HandleCommandL(TInt /*aCmd*/, TUint8* /*aData*/)
    {
    return KErrNotSupported;
    }

// ---------------------------------------------------------------------------
// CSplitItutUiMgrBase::HandleCtrlEventL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TBool CSplitItutUiMgrBase::HandleCtrlEventL(TInt /*aEventType*/, 
                                              CFepUiBaseCtrl* /*aCtrl*/, 
                                              const TDesC& /*aEventData*/)
    {
    return EFalse;
    }

// ---------------------------------------------------------------------------
// CSplitItutUiMgrBase::CSplitItutUiMgrBase
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CSplitItutUiMgrBase::CSplitItutUiMgrBase(MSplitItutLayoutContext* aContext,
                                             CSplitItutDataMgr* aDataMgr,
                                             CSplitItutWindowManager* aUiManager)
                                            :iLayoutContext(aContext),
                                             iDataMgr(aDataMgr),
                                             iNumLongPressValid(EFalse),
                                             iClrConsume(EFalse),
                                             iUiMgr(aUiManager)
    {
    }

// ---------------------------------------------------------------------------
// CSplitItutUiMgrBase::ConstructL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CSplitItutUiMgrBase::ConstructL()
    {
    }

// ---------------------------------------------------------------------------
// CSplitItutUiMgrBase::HandleLongPressNumberKeyL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CSplitItutUiMgrBase::HandleLongPressNumberKeyL(TPtiKey /*aKey*/)
    {
    }
    
// End Of File
