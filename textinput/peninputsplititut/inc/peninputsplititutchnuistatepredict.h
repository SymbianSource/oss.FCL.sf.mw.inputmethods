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
#ifndef C_CHNSPLITITUTUISTATEPREDICTBASE_H
#define C_CHNSPLITITUTUISTATEPREDICTBASE_H

#include "peninputsplititutuistatebase.h"

/**
 *  class CChnSplitItutUiStatePredict.
 *
 *  chinese split itu-t predict state
 *
 *  @lib peninputsplititut.lib
 *  @since S60 v5.0
 */
class CChnSplitItutUiStatePredict : public CSplitItutUiStateBase
    {
public:
    /** 
     * Symbian constructor.
     *
     * @since S60 5.0
     * 
     * @param aOwner
     * @return Pointer to created CChnSplitItutUiStatePredict object
     */
    static CChnSplitItutUiStatePredict* NewL(CSplitItutUiMgrBase* aOwner);

    /**
     * standard c++ destructor.
     *
     * @since S60 5.0
     * @return none
     */
    ~CChnSplitItutUiStatePredict();

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
     * handle key event
     *
     * @since S60 5.0
     * @param aCmd
     * @param aKey
     * @return none
     */
    TBool HandleKeyL(TInt aCmd, TInt aKey);
    
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
    TBool HandleCtrlEventL( TInt aEventType, 
                            CFepUiBaseCtrl* aCtrl, 
                            const TDesC& aEventData);

protected:
    /**
     * C++ constructor
     *
     * @since S60 5.0
     * @param aOwner
     * @return none
     */
    CChnSplitItutUiStatePredict(CSplitItutUiMgrBase* aOwner);
    
private:
    /**
     * is valid key
     *
     * @since S60 5.0
     * @param aKey
     * @return none
     */
    TBool IsKeyValid(TInt aKey);

    /**
     * remove duplicate candidate
     *
     * @since S60 5.0
     * @param aSrc
     * @param aTgt
     * @param aSrcStartIdx
     * @param aTgtStartIdx
     * @return none
     */
    TInt RemoveDuplicateCand(const RPointerArray<HBufC>& aSrc,
                             RPointerArray<HBufC>& aTgt,
                             TInt aSrcStartIdx,
                             TInt aTgtStartIdx);

private:

    /**
     * last over lap index
     */
    TInt iLastOverlapIdx;    
    };

#endif //C_CHNSPLITITUTUISTATEPREDICTBASE_H