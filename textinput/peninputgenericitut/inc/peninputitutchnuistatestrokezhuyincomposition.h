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
#ifndef C_CHNITUTUISTATESTROKEZHUYINCOMPOSITION_H
#define C_CHNITUTUISTATESTROKEZHUYINCOMPOSITION_H

#include "peninputgenericitutuistatebase.h"

class CChnItutUiStateStrokeZhuyinComposition : public CGenericItutUiStateBase
    {
public:
    static CChnItutUiStateStrokeZhuyinComposition* NewL(CGenericItutUiMgrBase* aOwner);

    ~CChnItutUiStateStrokeZhuyinComposition();

    void OnExit();

    void OnEntryL();

    CGenericItutUiMgrBase::TUiState StateType();

    TBool HandleKeyL(TInt aCmd, TInt aKey);

    // handle control event
    TBool HandleCtrlEventL(TInt aEventType, CFepUiBaseCtrl* aCtrl, const TDesC& aEventData);

protected:
    CChnItutUiStateStrokeZhuyinComposition(CGenericItutUiMgrBase* aOwner);
    };

#endif //C_CHNITUTUISTATESTROKEZHUYINCOMPOSITION_H