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
#ifndef C_CHNSPLITITUTUISTATEPINYINCOMPOSITION_H
#define C_CHNSPLITITUTUISTATEPINYINCOMPOSITION_H

#include "peninputsplititutuistatebase.h"

/**
 *  class CChnSplitItutUiStatePinyinComposition.
 *
 *  chinese split itu-t predict state
 *
 *  @lib peninputsplititut.lib
 *  @since S60 v5.0
 */
class CChnSplitItutUiStatePinyinComposition : public CSplitItutUiStateBase
    {
public:
    /** 
     * Symbian constructor.
     *
     * @since S60 5.0
     * 
     * @param aOwner
     * @return Pointer to created CChnSplitItutUiStatePinyinComposition object
     */
    static CChnSplitItutUiStatePinyinComposition* NewL(CSplitItutUiMgrBase* aOwner);

    /**
     * standard c++ destructor.
     *
     * @since S60 5.0
     * @return none
     */
    ~CChnSplitItutUiStatePinyinComposition();

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
     * handle control event
     *
     * @since S60 5.0
     * @param aEventType
     * @param aCtrl
     * @param aEventData
     * @return none
     */
    TBool HandleCtrlEventL(TInt aEventType, CFepUiBaseCtrl* aCtrl, const TDesC& aEventData);

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
     * handle key event
     *
     * @since S60 5.0
     * @param aCmd
     * @param aKey
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
    CChnSplitItutUiStatePinyinComposition(CSplitItutUiMgrBase* aOwner);

private:
    /**
     * spell candidate array
     */
    RPointerArray<HBufC> iSpellCands;
    };

#endif //C_CHNSPLITITUTUISTATEPINYINCOMPOSITION_H