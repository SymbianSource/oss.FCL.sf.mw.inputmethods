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

#ifndef C_CHNITUTUISTATESTANDBYBASE_H
#define C_CHNITUTUISTATESTANDBYBASE_H

#include "peninputgenericitutuistatebase.h"

class CChnItutUiStateStandby : public CGenericItutUiStateBase
    {
public:
    static CChnItutUiStateStandby* NewL(CGenericItutUiMgrBase* aOwner);

    ~CChnItutUiStateStandby();

    void OnExit();

    void OnEntryL();

    CGenericItutUiMgrBase::TUiState StateType();

    TBool HandleKeyL(TInt aCmd, TInt aKey);

protected:
    CChnItutUiStateStandby(CGenericItutUiMgrBase* aOwner);
    
private:

    TBool IsKeyValid(TInt aKey);
    };

#endif //C_CHNITUTUISTATESTANDBYBASE_H