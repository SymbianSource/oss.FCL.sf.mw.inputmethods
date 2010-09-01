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
#ifndef C_CHNSPLITITUTUIMGR_H
#define C_CHNSPLITITUTUIMGR_H

#include "peninputsplititutuimgrbase.h"

class MSplitItutLayoutContext;

/**
 *  class CChnSplitItutUiMgr.
 *
 *  split itu-t chinese ui mgr.
 *
 *  @lib peninputsplititut.lib
 *  @since S60 v5.0
 */
class CChnSplitItutUiMgr : public CSplitItutUiMgrBase
    {
public:
    /** 
     * Symbian constructor.
     *
     * @since S60 5.0
     * 
     * @param aContext layout context
     * @param aDataMgr data mgr
     * @param aUiManager Ui mgr
     * @return split ui mgr
     */
    static CChnSplitItutUiMgr* NewL(MSplitItutLayoutContext* aContext, 
                               CSplitItutDataMgr* aDataMgr,
                               CSplitItutWindowManager* aUiManager);

    /**
     * standard c++ destructor.
     *
     * @since S60 5.0
     * @return none
     */
    ~CChnSplitItutUiMgr();

    /** 
     * handle command.
     *
     * @since S60 5.0
     * 
     * @param aCmd
     * @param aData
     * @return TInt
     */
    TInt HandleCommandL(TInt aCmd, TUint8* aData);

    /** 
     * handle control event.
     *
     * @since S60 5.0
     * 
     * @param aEventType
     * @param aCtrl
     * @param aEventData
     * @return TBool
     */
    TBool HandleCtrlEventL( TInt aEventType, 
                            CFepUiBaseCtrl* aCtrl, 
                            const TDesC& aEventData);

    /** 
     * set current state
     *
     * @since S60 5.0
     * 
     * @param aNewStateType
     * @return None
     */
    void SetCurrentStateL(TUiState aNewStateType);

    /** 
     * active ui
     *
     * @since S60 5.0
     * 
     * @return None
     */
    void ActivateUiL();

    /** 
     * handle long press number key
     *
     * @since S60 5.0
     * 
     * @param aKey
     * @return None
     */
    void HandleLongPressNumberKeyL(TPtiKey aKey);

    /** 
     * deactive ui
     *
     * @since S60 5.0
     * 
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
    CChnSplitItutUiMgr(MSplitItutLayoutContext* aContext,
                  CSplitItutDataMgr* aDataMgr,
                  CSplitItutWindowManager* aUiManager);
                  
    /**
     * Symbian second-phase constructor
     *
     * @since S60 5.0
     * @return none
     */
    void ConstructL();
    
private:
    
    /**
     * is chinese mode
     *
     * @since S60 5.0
     * @return TBool
     */
    TBool IsChineseMode();    

private:
    /**
     * standby state
     * Own
     */
    CSplitItutUiStateBase* iStandbyState;
    
    /**
     * pinyin component state
     * Own
     */
    CSplitItutUiStateBase* iPinyinComp;
    
    /**
     * stroke or zhuyin component state
     * Own
     */
    CSplitItutUiStateBase* iStoZhuComp;
    
    /**
     * predictive component state
     * Own
     */
    CSplitItutUiStateBase* iPredictState;
    
    /**
     * Normal state
     * Own
     */
    CSplitItutUiStateBase* iNormalState;
       
    };

#endif // C_CHNSPLITITUTUIMGR_H