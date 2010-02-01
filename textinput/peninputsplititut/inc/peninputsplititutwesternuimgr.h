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
#ifndef C_WESTERNSPLITITUTUIMGR_H
#define C_WESTERNSPLITITUTUIMGR_H

#include "peninputsplititutuimgrbase.h"

/**
 *  class CWesternSplitItutUiMgr.
 *
 *  western split itut ui manager class.
 *
 *  @lib peninputsplititut.lib
 *  @since S60 v5.0
 */
class CWesternSplitItutUiMgr : public CSplitItutUiMgrBase
    {
public:
    /** 
     * Symbian constructor.
     *
     * @since S60 5.0
     * 
     * @param aContext
     * @param aDataMgr
     * @param aUiManager
     * @return Pointer to created CWesternSplitItutUiMgr object
     */
    static CWesternSplitItutUiMgr* NewL(MSplitItutLayoutContext* aContext, 
                                   CSplitItutDataMgr* aDataMgr,
                                   CSplitItutWindowManager* aUiManager);

    /**
     * standard c++ destructor.
     *
     * @since S60 5.0
     * @return none
     */
    ~CWesternSplitItutUiMgr();

    /**
     * From CSplitItutUiMgrBase
     * handle command
     *
     * @since S60 5.0
     * @param aCmd
     * @param aData
     * @return TInt
     */
    TInt HandleCommandL(TInt aCmd, TUint8* aData);

    /**
     * From CSplitItutUiMgrBase
     * handle control event
     *
     * @since S60 5.0
     * @param aEventType
     * @param aCtrl
     * @param aEventData
     * @return TBool
     */
    TBool HandleCtrlEventL( TInt aEventType, 
                            CFepUiBaseCtrl* aCtrl, 
                            const TDesC& aEventData);

    /**
     * From CSplitItutUiMgrBase
     * Set current state
     *
     * @since S60 5.0
     * @param aNewState
     * @return TInt
     */
    void SetCurrentStateL(TUiState aNewState);

    /**
     * From CSplitItutUiMgrBase
     * Active ui
     *
     * @since S60 5.0
     * @return None
     */
    void ActivateUiL();

    /**
     * From CSplitItutUiMgrBase
     * deactive ui
     *
     * @since S60 5.0
     * @return None
     */
    void DeactivateUI();

protected:
    /**
     * C++ constructor
     *
     * @since S60 5.0
     * @param aContext
     * @param aDataMgr
     * @param aUiManager
     * @return none
     */
    CWesternSplitItutUiMgr( MSplitItutLayoutContext* aContext,
                            CSplitItutDataMgr* aDataMgr,
                            CSplitItutWindowManager* aUiManager);

    /**
     * standard c++ destructor.
     *
     * @since S60 5.0
     * @return none
     */
    void ConstructL();
    
private:
    /**
     * normal state.
     * own
     */
    CSplitItutUiStateBase* iNormalState;
    };

#endif // C_WESTERNSPLITITUTUIMGR_H