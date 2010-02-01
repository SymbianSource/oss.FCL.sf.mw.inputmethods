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
#include "peninputgenericitutuistatebase.h"

CGenericItutUiStateBase::~CGenericItutUiStateBase()
    {
    }

void CGenericItutUiStateBase::OnExit()
    {
    }

void CGenericItutUiStateBase::OnEntryL()
    {
    }

TBool CGenericItutUiStateBase::HandleCtrlEventL(TInt /*aEventType*/, 
                                                CFepUiBaseCtrl* /*aCtrl*/, 
                                                const TDesC& /*aEventData*/)
    {
    return EFalse;
    }

CGenericItutUiStateBase::CGenericItutUiStateBase(CGenericItutUiMgrBase* aOwner)
                                                :iOwner(aOwner)
    {
    }

void CGenericItutUiStateBase::ConstructL()
    {
    }

CGenericItutUiMgrBase::TUiState CGenericItutUiStateBase::StateType()
    {
    return CGenericItutUiMgrBase::EStateNull;
    }

TBool CGenericItutUiStateBase::HandleKeyL(TInt /*aCmd*/, TInt /*aKey*/)
    {
    return EFalse;
    }
    
TInt CGenericItutUiStateBase::HandleCommandL(TInt /*aCmd*/, TUint8* /*aData*/)
    {
    return KErrNotSupported;
    }
    
// End Of File
