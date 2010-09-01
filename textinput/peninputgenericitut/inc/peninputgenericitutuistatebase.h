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
* Description:  drop-down list control
*
*/


#ifndef C_GENERICITUTUISTATEBASE_H
#define C_GENERICITUTUISTATEBASE_H

#include <e32base.h>
#include <PtiDefs.h>

#include "peninputgenericitutdata.h"
#include "peninputgenericitutuimgrbase.h"

class MItutLayoutContext;
class CGenericItutDataMgr;
class CFepUiBaseCtrl;

class CGenericItutUiStateBase : public CBase
    {
public:

    virtual ~CGenericItutUiStateBase();

    virtual void OnExit() =  0;

    virtual void OnEntryL() = 0;

    // handle control event
    virtual TBool HandleCtrlEventL( TInt aEventType, CFepUiBaseCtrl* aCtrl, 
                                    const TDesC& aEventData);

    virtual CGenericItutUiMgrBase::TUiState StateType() = 0;

    virtual TBool HandleKeyL(TInt aCmd, TInt aKey);
    
    virtual TInt HandleCommandL(TInt aCmd, TUint8* aData);

protected:
    CGenericItutUiStateBase(CGenericItutUiMgrBase* aOwner);

    virtual void ConstructL();

protected: 
    // not own
    CGenericItutUiMgrBase* iOwner;
    };

#endif //C_GENERICITUTUISTATEBASE_H
