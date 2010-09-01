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
#ifndef C_CHNITUTUIMGR_H
#define C_CHNITUTUIMGR_H

#include "peninputgenericitutuimgrbase.h"


class CChnItutUiMgr : public CGenericItutUiMgrBase
    {
public:
    static CChnItutUiMgr* NewL(MItutLayoutContext* aContext, 
                               CGenericItutDataMgr* aDataMgr,
                               CGenericItutWindowManager* aUiManager);

    ~CChnItutUiMgr();

    // called by ui layout
    TInt HandleCommandL(TInt aCmd, TUint8* aData);

    // handle control event
    TBool HandleCtrlEventL(TInt aEventType, CFepUiBaseCtrl* aCtrl, const TDesC& aEventData);

    void SetCurrentStateL(TUiState aNewStateType);

    void ActivateUiL();

    void HandleLongPressNumberKeyL(TPtiKey aKey);

    void DeactivateUI();

protected:
    CChnItutUiMgr(MItutLayoutContext* aContext,
                  CGenericItutDataMgr* aDataMgr,
                  CGenericItutWindowManager* aUiManager);

    void ConstructL();
    
private:
    
    TBool IsChineseMode();    

private:
    CGenericItutUiStateBase* iStandbyState;
    CGenericItutUiStateBase* iPinyinComp;
    CGenericItutUiStateBase* iStoZhuComp;
    CGenericItutUiStateBase* iPredictState;
    
    CGenericItutUiStateBase* iNormalState;
    CGenericItutUiStateBase* iMatchSelState;
    CGenericItutUiStateBase* iSpellState;    
    };

#endif // C_CHNITUTUIMGR_H