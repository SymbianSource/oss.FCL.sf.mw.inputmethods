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
#ifndef C_CHNITUTUISTATEPREDICTBASE_H
#define C_CHNITUTUISTATEPREDICTBASE_H

#include "peninputgenericitutuistatebase.h"

class CChnItutUiStatePredict : public CGenericItutUiStateBase
    {
public:
    static CChnItutUiStatePredict* NewL(CGenericItutUiMgrBase* aOwner);

    ~CChnItutUiStatePredict();

    void OnExit();

    void OnEntryL();

    CGenericItutUiMgrBase::TUiState StateType();

    TBool HandleKeyL(TInt aCmd, TInt aKey);
    
    // handle control event
    TBool HandleCtrlEventL(TInt aEventType, CFepUiBaseCtrl* aCtrl, const TDesC& aEventData);

protected:
    CChnItutUiStatePredict(CGenericItutUiMgrBase* aOwner);
    
private:
    TBool IsKeyValid(TInt aKey);

    // from aTgt: remove overlap cand between aSrc and aTgt
    // aSrcStartIdx: from it to compare aSrc
    // aTgtStartIdx: from it to compare aTgt
    TInt RemoveDuplicateCand(const RPointerArray<HBufC>& aSrc,
                             RPointerArray<HBufC>& aTgt,
                             TInt aSrcStartIdx,
                             TInt aTgtStartIdx);

private:
    TInt iLastOverlapIdx;    
    };

#endif //C_CHNITUTUISTATEPREDICTBASE_H