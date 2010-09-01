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
#ifndef C_CHNITUTUISTATEPINYINCOMPOSITION_H
#define C_CHNITUTUISTATEPINYINCOMPOSITION_H

#include "peninputgenericitutuistatebase.h"

class CChnItutUiStatePinyinComposition : public CGenericItutUiStateBase
    {
public:
    static CChnItutUiStatePinyinComposition* NewL(CGenericItutUiMgrBase* aOwner);

    ~CChnItutUiStatePinyinComposition();

    void OnExit();

    void OnEntryL();

    // handle control event
    TBool HandleCtrlEventL(TInt aEventType, CFepUiBaseCtrl* aCtrl, const TDesC& aEventData);

    CGenericItutUiMgrBase::TUiState StateType();

    TBool HandleKeyL(TInt aCmd, TInt aKey);

protected:
    CChnItutUiStatePinyinComposition(CGenericItutUiMgrBase* aOwner);

/*
private:
    // get nearest spell index, which is most close to last selected spell
    TInt GetNearestSpell(TInt aValidLastSpellLen);
*/

private:
    RPointerArray<HBufC> iSpellCands;
    //HBufC* iLastSelSpell;
    };

#endif //C_CHNITUTUISTATEPINYINCOMPOSITION_H