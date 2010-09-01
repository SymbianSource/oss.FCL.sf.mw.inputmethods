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

#ifndef C_CHNSPLITITUTUISTATESTANDBYBASE_H
#define C_CHNSPLITITUTUISTATESTANDBYBASE_H

#include "peninputsplititutuistatebase.h"

/**
 *  class CChnSplitItutUiStateStandby.
 *
 *  chinese split itu-t standby state
 *
 *  @lib peninputsplititut.lib
 *  @since S60 v5.0
 */
class CChnSplitItutUiStateStandby : public CSplitItutUiStateBase
    {
public:
    /** 
     * Symbian constructor.
     *
     * @since S60 5.0
     * 
     * @param aOwner
     * @return Pointer to created CChnSplitItutUiStateStandby object
     */
    static CChnSplitItutUiStateStandby* NewL(CSplitItutUiMgrBase* aOwner);

    /**
     * standard c++ destructor.
     *
     * @since S60 5.0
     * @return none
     */
    ~CChnSplitItutUiStateStandby();
    
    /**
     * From CSplitItutUiStateBase
     * exit current state
     *
     * @since S60 5.0
     * @return none
     */
    void OnExit();

    /**
     * From CSplitItutUiStateBase
     * exit current state
     *
     * @since S60 5.0
     * @return none
     */
    void OnEntryL();

    /**
     * From CSplitItutUiStateBase
     * exit current state
     *
     * @since S60 5.0
     * @return 
     */
    CSplitItutUiMgrBase::TUiState StateType();

    /**
     * From CSplitItutUiStateBase
     * exit current state
     *
     * @since S60 5.0
     * @return none
     */
    TBool HandleKeyL(TInt aCmd, TInt aKey);

protected:
    /**
     * C++ constructor
     *
     * @since S60 5.0
     * @param aOwner
     * @return none
     */
    CChnSplitItutUiStateStandby(CSplitItutUiMgrBase* aOwner);
    
private:
    /**
     * is valid key
     *
     * @since S60 5.0
     * @param aKey
     * @return none
     */
    TBool IsKeyValid(TInt aKey);
    };

#endif //C_CHNSPLITITUTUISTATESTANDBYBASE_H