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
#include "peninputgenericitutuimgrbase.h"
#include "peninputgenericitutlayoutcontext.h"
#include "peninputgenericitutdatamgr.h"
#include "peninputgenericitutuistatebase.h"

CGenericItutUiMgrBase::~CGenericItutUiMgrBase()
    {
    }

TInt CGenericItutUiMgrBase::HandleCommandL(TInt /*aCmd*/, TUint8* /*aData*/)
    {
    return KErrNotSupported;
    }

TBool CGenericItutUiMgrBase::HandleCtrlEventL(TInt /*aEventType*/, 
                                              CFepUiBaseCtrl* /*aCtrl*/, 
                                              const TDesC& /*aEventData*/)
    {
    return EFalse;
    }

CGenericItutUiMgrBase::CGenericItutUiMgrBase(MItutLayoutContext* aContext,
                                             CGenericItutDataMgr* aDataMgr,
                                             CGenericItutWindowManager* aUiManager)
                                            :iLayoutContext(aContext),
                                             iDataMgr(aDataMgr),
                                             iNumLongPressValid(EFalse),
                                             iClrConsume(EFalse),
                                             iUiMgr(aUiManager)
    {
    }

void CGenericItutUiMgrBase::ConstructL()
    {
    }

void CGenericItutUiMgrBase::HandleLongPressNumberKeyL(TPtiKey /*aKey*/)
    {
    }
    
    

// End Of File
