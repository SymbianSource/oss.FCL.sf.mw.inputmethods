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

#ifndef C_WESTERNSPLITITUTUISTATENONPREDICT_H
#define C_WESTERNSPLITITUTUISTATENONPREDICT_H

#include "peninputsplititutuistatebase.h"

class CFepLayoutMultiLineIcf;


/**
 *  class CWesternSplitItutUiStateNonPredict.
 *
 *  western split itut non-predict state class.
 *
 *  @lib peninputsplititut.lib
 *  @since S60 v5.0
 */
class CWesternSplitItutUiStateNonPredict : public CSplitItutUiStateBase
    {
public:
    /** 
     * Symbian constructor.
     *
     * @since S60 5.0
     * 
     * @param aOwner The MLayoutOwner
     * @return Pointer to created CWesternSplitItutUiStateNonPredict object
     */
    static CWesternSplitItutUiStateNonPredict* NewL(CSplitItutUiMgrBase* aOwner);

   /**
     * standard c++ destructor.
     *
     * @since S60 5.0
     * @return none
     */
    ~CWesternSplitItutUiStateNonPredict();

    /**
     * From CSplitItutUiStateBase.
     * exit current stat
     *
     * @since Series 60 5.0
     * @return None    
     */
    void OnExit();

    /**
     * From CSplitItutUiStateBase.
     * entry new stat
     *
     * @since Series 60 5.0
     * @return None    
     */
    void OnEntryL();

    /**
     * From CSplitItutUiStateBase.
     * get state type
     *
     * @since Series 60 5.0
     * @return state type    
     */
    CSplitItutUiMgrBase::TUiState StateType();

    /**
     * From CSplitItutUiStateBase.
     * handle command
     *
     * @since Series 60 5.0
     * @return Tint   
     */
    TInt HandleCommandL(TInt aCmd, TUint8* aData);

    /**
     * From CSplitItutUiStateBase.
     * handle control event
     *
     * @since Series 60 5.0
     * @return TBool  
     */
    TBool HandleCtrlEventL( TInt aEventType, 
                            CFepUiBaseCtrl* aCtrl, 
                            const TDesC& aEventData);

protected:
    /**
     * C++ constructor
     *
     * @since S60 5.0
     * @param aOwner The MLayoutOwner pointer.
     * @return none
     */
    CWesternSplitItutUiStateNonPredict(CSplitItutUiMgrBase* aOwner);

private:
    /**
     * is consume last keydown.
     */
    TBool iConsumeLastKeyDown;
    };

#endif //C_WESTERNSPLITITUTUISTATENONPREDICT_H