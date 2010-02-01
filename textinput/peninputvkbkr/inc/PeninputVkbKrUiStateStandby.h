/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  peninput VKB data manager
*
*/

#ifndef PENINPUTVKBKRUISTATESTANDBY_H
#define PENINPUTVKBKRUISTATESTANDBY_H

#include "PeninputVkbKrUiStateBase.h"

class CPeninputVkbKrUiStateStandby : 
        public CPeninputVkbKrUiStateBase,
        public MPtiObserver
    {
public:
    
    static CPeninputVkbKrUiStateStandby* NewLC(
        MPeninputUiStateMgr* aUiStateMgr, 
        MPeninputLayoutContext* aContext,
        CPtiEngine& aPtiEngine);

    static CPeninputVkbKrUiStateStandby* NewL(
        MPeninputUiStateMgr* aUiStateMgr, 
        MPeninputLayoutContext* aContext,
        CPtiEngine& aPtiEngine);

    virtual ~CPeninputVkbKrUiStateStandby();

private:

    CPeninputVkbKrUiStateStandby(
        MPeninputUiStateMgr* aUiStateMgr, 
        MPeninputLayoutContext* aContext,
        CPtiEngine& aPtiEngine);

    void ConstructL();

private: // from CPeninputVkbKrUiStateBase 
    
    virtual TBool HandleKeyEventL(
        const TRawEvent& aData);
    
    virtual TBool HandleControlEvent(
        TInt aEventType,
        const TDesC& aEventData);
    
private:
    
    /**
    * This method is called when multitapping timer
    * expires. Related to EPtiEngineMultitaping input mode.
    */
    virtual void KeyTimerExpired();
    
    /**
    * This method is called when last item in prediction
    * candidate list is reached. Related to EPtiEnginePredictive
    * input mode.
    */
    virtual void LastWordInSelectionList();
    
    /**
    * This method is called when the first item in prediction
    * candidate list is reached. Related to EPtiEnginePredictive
    * input mode.
    */      
    virtual void FirstWordInSelectionList();

private:

    virtual TBool HandleVkbEvent(
        TInt aEventType, 
        const TDesC& aEventData );
    
    virtual TBool HandleBackEvent( 
        TInt aEventType, 
        const TDesC& aEventData );

    virtual TBool HandleEnterSpaceEvent( 
        TInt aEventType, 
        const TDesC& aEventData );

    virtual TBool HandleTabEvent( 
        TInt aEventType, 
        const TDesC& aEventData );
    
    TBool IsKoreanRange();
    
    TBool IsKoreanShiftLayout();
    
    TInt GetVkbLayout();
    
    TBool HandleKoreanKey(const TDesC& aEventData);
    
    void SendReplaceText(const TInt& aLenToReplace,const TDesC& aTextToReplace);
    
    void SendRangeEvent(const TInt& aRange,const TInt& aLayout);
    
    TBool CommitOrNot(TPtrC aData);
    
private:
    
    TBool iTextCommited;
    
    };

#endif // PENINPUTVKBKRUISTATESTANDBY_H
