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
#ifndef C_WESTERNITUTUIMGR_H
#define C_WESTERNITUTUIMGR_H

#include "peninputgenericitutuimgrbase.h"

class CWesternItutUiMgr : public CGenericItutUiMgrBase
    {
public:
    static CWesternItutUiMgr* NewL(MItutLayoutContext* aContext, 
                                   CGenericItutDataMgr* aDataMgr,
                                   CGenericItutWindowManager* aUiManager);

    ~CWesternItutUiMgr();

    // called by ui layout
    TInt HandleCommandL(TInt aCmd, TUint8* aData);

    // handle control event
    TBool HandleCtrlEventL(TInt aEventType, CFepUiBaseCtrl* aCtrl, const TDesC& aEventData);

    void SetCurrentStateL(TUiState aNewState);

    void ActivateUiL();

    void DeactivateUI();

protected:

    CWesternItutUiMgr(MItutLayoutContext* aContext,
                      CGenericItutDataMgr* aDataMgr,
                      CGenericItutWindowManager* aUiManager);

    void ConstructL();
    
private:
    CGenericItutUiStateBase* iNormalState;
    CGenericItutUiStateBase* iCustomerState;
    CGenericItutUiStateBase* iMatchSelState;
    CGenericItutUiStateBase* iSpellState;
    };

#endif // C_WESTERNITUTUIMGR_H