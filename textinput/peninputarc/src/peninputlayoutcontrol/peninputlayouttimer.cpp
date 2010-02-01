/*
* Copyright (c) 2005-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for timer used in layout
*
*/


#include "peninputlayouttimer.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLayoutTimer::NewL
// two-phase constructor
// ---------------------------------------------------------------------------
//
CLayoutTimer* CLayoutTimer::NewL(MTimerHandler* aTimerHandler,TTimeType aMode,
                                                    TBool aAutoFlag)
    {
    CLayoutTimer *self = new(ELeave) CLayoutTimer(aTimerHandler,aMode,aAutoFlag);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }


// ---------------------------------------------------------------------------
// CLayoutTimer::CLayoutTimer
// Default constructor
// ---------------------------------------------------------------------------
//
CLayoutTimer::CLayoutTimer(MTimerHandler* aTimerHandler,TTimeType aMode,
                            TBool aAutoFlag)
                           : CActive(0),iType(aMode),
                           iAutoRepeat(aAutoFlag),iTimerHandler(aTimerHandler)
    {
    }

// ---------------------------------------------------------------------------
// CLayoutTimer::ConstructL
// 2nd phase constructor
// ---------------------------------------------------------------------------
//
void CLayoutTimer::ConstructL()
    {
    TInt err = iTimer.CreateLocal();
    User::LeaveIfError(err);
    CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------------------------
// CLayoutTimer::~CLayoutTimer
// Destructor
// ---------------------------------------------------------------------------
//
CLayoutTimer::~CLayoutTimer()
    {
    Cancel();
    iTimer.Close();
    }

// ---------------------------------------------------------------------------
// CLayoutTimer::RunL
// called when time out
// ---------------------------------------------------------------------------
//
void CLayoutTimer::RunL()
    {
    iTimerHandler->HandleTimerOut (iType);
    if(iAutoRepeat)
        {
        iTimer.After(iStatus,iInterval);
        SetActive();        
        }
    }

// ---------------------------------------------------------------------------
// CLayoutTimer::RunError
// called if RunL leaves
// ---------------------------------------------------------------------------
//
TInt CLayoutTimer::RunError(TInt /*aError*/)
    {
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CLayoutTimer::DoCancel
// called when stroker timer has been cancelled
// ---------------------------------------------------------------------------
//
void CLayoutTimer::DoCancel()
    {
    iTimer.Cancel();
    }

// ---------------------------------------------------------------------------
// CLayoutTimer::SetTimer
// Set the timer and activate it
// ---------------------------------------------------------------------------
//
void CLayoutTimer::SetTimer (TTimeIntervalMicroSeconds32 aDelay)
    {
    Cancel();
    iInterval = aDelay;
    iTimer.After(iStatus,aDelay);
    SetActive();
    }
//end of file
