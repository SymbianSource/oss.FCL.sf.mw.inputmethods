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
* Description:  states implementation
*
*/


//  INCLUDES
#include <aknfeppeninputenums.h>
#include <peninputdropdownlist.h>

#include "peninputhwrbxlayout.h"
#include "peninputhwrbxwnd.h"
#include "peninputhwrbxstate.h"
#include "peninputhwrevent.h"
#include "peninputhwrbxdatastore.h"
#include "peninputhwrtriggerstr.h"

//CAknFepHwrBxStateBase

// ---------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------
//
CAknFepHwrBxStateBase::CAknFepHwrBxStateBase(CAknFepHwrBxLayout* aLayout)
    {
    iLayout = aLayout;
    iWnd = NULL;
    iDataStore = NULL;
    
    if (iLayout)
        {
        iWnd = iLayout->Window();    
        iDataStore = iLayout->DataStore();
        }

    ASSERT(iLayout != NULL);
    ASSERT(iWnd != NULL);
    ASSERT(iDataStore != NULL);
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
CAknFepHwrBxStateBase::~CAknFepHwrBxStateBase()
    {
    }
    
// ---------------------------------------------------------
// Handle layout command event
// ---------------------------------------------------------
//
TInt CAknFepHwrBxStateBase::HandleCommandL(TInt /*aCmd*/, TUint8* /*aData*/)
    {
    return KErrNone;
    }

// ---------------------------------------------------------
// Handle raw event key
// ---------------------------------------------------------
//
TBool CAknFepHwrBxStateBase::HandleKeyEventL(const TRawEvent& /*aKeyEvent*/)
    {
    return EFalse;
    }

// ---------------------------------------------------------
// Handle raw event key
// ---------------------------------------------------------
//
TBool CAknFepHwrBxStateBase::HandleEventL(TInt /*aEventType*/, const TDesC& /*aEventData*/)
    {
    //not handled
    return EFalse;
    }

// ---------------------------------------------------------
// Doing something before entry a state
// ---------------------------------------------------------
//
void CAknFepHwrBxStateBase::OnEntry()
    {
    }

// ---------------------------------------------------------
// Doing something before exit a state
// ---------------------------------------------------------
//
void CAknFepHwrBxStateBase::OnExit()
    {
    }

//CAknFepHwrBxStateBase end

//CAknFepHwrBxStateCloseable

// ---------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------
//
CAknFepHwrBxStateCloseable::CAknFepHwrBxStateCloseable(CAknFepHwrBxLayout* aLayout)
: CAknFepHwrBxStateBase(aLayout)                          
    {
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
CAknFepHwrBxStateCloseable::~CAknFepHwrBxStateCloseable()
    {
    }

// ---------------------------------------------------------
// Handle ECmdPenInputWindowClose
// ---------------------------------------------------------
//
TInt CAknFepHwrBxStateCloseable::HandleCommandL(TInt aCmd, TUint8* /*aData*/)
    {
    if (ECmdPenInputWindowClose == aCmd)
        {
        //jump to init state
        CAknFepHwrBxStateInit* state = CAknFepHwrBxStateInit::NewLC(iLayout);
        iLayout->SetCurrentStateL(state);
        CleanupStack::Pop(state);
        
        //record the close mode: automatically
        state->SetAutoClosed(ETrue);
        }

    return KErrNone;
    }

//CAknFepHwrBxStateCloseable end
    
//CAknFepHwrBxStateInit    

// ---------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------
//
CAknFepHwrBxStateInit* CAknFepHwrBxStateInit::NewLC(CAknFepHwrBxLayout* aLayout)
    {
    CAknFepHwrBxStateInit* self = new (ELeave) CAknFepHwrBxStateInit(aLayout);
    CleanupStack::PushL(self);

    return self;
    }

// ---------------------------------------------------------
// C++ default constructor
// ---------------------------------------------------------
//
CAknFepHwrBxStateInit::CAknFepHwrBxStateInit(CAknFepHwrBxLayout* aLayout)
: CAknFepHwrBxStateBase(aLayout)
    {
    iAutoClosed = ETrue;    
    }

// ---------------------------------------------------------
// destructor
// ---------------------------------------------------------
//
CAknFepHwrBxStateInit::~CAknFepHwrBxStateInit()
    {
    }

// ---------------------------------------------------------
// Record last closed mode
// ---------------------------------------------------------
//
void CAknFepHwrBxStateInit::SetAutoClosed(TBool aAutoFlag)
    {
    iAutoClosed = aAutoFlag;
    }

// ---------------------------------------------------------
// Handle command
// ---------------------------------------------------------
//
TInt CAknFepHwrBxStateInit::HandleCommandL(TInt aCmd, TUint8* aData)
    {
    return KErrNone;
    }

// ---------------------------------------------------------
// Doing something before entry a state
// ---------------------------------------------------------
//
void CAknFepHwrBxStateInit::OnEntry()
    {
    iWnd->CancelWriting();
    iWnd->CancelRepeat();
    iWnd->Hide(ETrue);
    }

// ---------------------------------------------------------
// Doing something before exit a state
// ---------------------------------------------------------
//
void CAknFepHwrBxStateInit::OnExit()
    {
    iWnd->Hide(EFalse);
    }

//CAknFepHwrBxStateInit end

//CAknFepHwrBxStateStandby

// ---------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------
//
CAknFepHwrBxStateStandby* CAknFepHwrBxStateStandby::NewLC(CAknFepHwrBxLayout* aLayout)
    {
    CAknFepHwrBxStateStandby* self = new (ELeave) CAknFepHwrBxStateStandby(aLayout);
    CleanupStack::PushL(self);
    
    return self;
    }

// ---------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------
//
CAknFepHwrBxStateStandby* CAknFepHwrBxStateStandby::NewL(CAknFepHwrBxLayout* aLayout)
    {
    CAknFepHwrBxStateStandby* self = CAknFepHwrBxStateStandby::NewLC( aLayout );
    CleanupStack::Pop(self);
    
    return self;
    }

// C++ default constructor
// ---------------------------------------------------------
//
CAknFepHwrBxStateStandby::CAknFepHwrBxStateStandby(CAknFepHwrBxLayout* aLayout)
: CAknFepHwrBxStateCloseable(aLayout)
    {
    }


// ---------------------------------------------------------
// destructor
// ---------------------------------------------------------
//
CAknFepHwrBxStateStandby::~CAknFepHwrBxStateStandby()
    {
    }

// ---------------------------------------------------------
// Handle event
// ---------------------------------------------------------
//
TBool CAknFepHwrBxStateStandby::HandleEventL(TInt aEventType, const TDesC& /*aEventData*/)
    {
    TBool handled = ETrue;
    CAknFepHwrBxStateBase* state = NULL;
        
    switch (aEventType)
        {
        case EHwrEventBeginWriting:
            state = CAknFepHwrBxStateBeginWriting::NewLC(iLayout);
            iLayout->SetCurrentStateL(state);
            CleanupStack::Pop(state);//state
            break;
        case EHwrEventWindowClose:
            //jump to init state
            state = CAknFepHwrBxStateInit::NewLC(iLayout);
            iLayout->SetCurrentStateL(state);
            CleanupStack::Pop(state);

            //record the close mode: manully
            static_cast<CAknFepHwrBxStateInit*>(state)->SetAutoClosed(EFalse);
            handled = EFalse;
            break;        
        default:
            handled = EFalse;
            break;
        }
        
    return handled;
    }

// ---------------------------------------------------------
// Doing something before entry a state
// ---------------------------------------------------------
//
void CAknFepHwrBxStateStandby::OnEntry()
    {
    iDataStore->ClearCandidates();
    //cancel writing timer
    iWnd->CancelWriting();
    }

//CAknFepHwrBxStateStandby end

//CAknFepHwrBxStateBeginWriting

// ---------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------
//
CAknFepHwrBxStateBeginWriting* CAknFepHwrBxStateBeginWriting::NewLC(CAknFepHwrBxLayout* aLayout)
    {
    CAknFepHwrBxStateBeginWriting* self = new (ELeave) CAknFepHwrBxStateBeginWriting(aLayout);
    CleanupStack::PushL(self);
    
    return self;
    }

// ---------------------------------------------------------
// C++ default constructor
// ---------------------------------------------------------
//
CAknFepHwrBxStateBeginWriting::CAknFepHwrBxStateBeginWriting(CAknFepHwrBxLayout* aLayout)
: CAknFepHwrBxStateCloseable(aLayout)
    {
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
CAknFepHwrBxStateBeginWriting::~CAknFepHwrBxStateBeginWriting()
    {
    }

// ---------------------------------------------------------
// Handle event
// ---------------------------------------------------------
//
TBool CAknFepHwrBxStateBeginWriting::HandleEventL(TInt aEventType, const TDesC& aEventData)
    {
    CAknFepHwrBxStateBase* state = NULL;

    switch (aEventType)
        {
        case EHwrEventEndStroke:
            {
            const RArray<TPoint>* aTraceData = (RArray<TPoint>*)aEventData.Ptr();
            iDataStore->DoRecognizeL(*aTraceData);

            state = CAknFepHwrBxStateEndWriting::NewLC(iLayout);
            iLayout->SetCurrentStateL(state);
            CleanupStack::Pop(state);//state

            break;
            }
        default:
            break;
        }

    //All events are discarded.
    return ETrue;
    }
     
// ---------------------------------------------------------
// Doing something before entry a state
// ---------------------------------------------------------
//
void CAknFepHwrBxStateBeginWriting::OnEntry()
    {
    //disable draging
    //iWnd->SetDragEnable(EFalse);
    iWnd->DimInputContextField(ETrue);
    }
     
//CAknFepHwrBxStateBeginWriting end

//CAknFepHwrBxStateEndWriting

// ---------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------
//
CAknFepHwrBxStateEndWriting* CAknFepHwrBxStateEndWriting::NewLC(CAknFepHwrBxLayout* aLayout)
    {
    CAknFepHwrBxStateEndWriting* self = new (ELeave) CAknFepHwrBxStateEndWriting(aLayout);
    CleanupStack::PushL(self);
    
    return self;
    }

// ---------------------------------------------------------
// C++ default constructor
// ---------------------------------------------------------
//
CAknFepHwrBxStateEndWriting::CAknFepHwrBxStateEndWriting(CAknFepHwrBxLayout* aLayout)
: CAknFepHwrBxStateCloseable(aLayout)
    {
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
CAknFepHwrBxStateEndWriting::~CAknFepHwrBxStateEndWriting()
    {
    }

// ---------------------------------------------------------
// Handle event
// ---------------------------------------------------------
//
TBool CAknFepHwrBxStateEndWriting::HandleEventL(TInt aEventType, const TDesC& aEventData)
    {
    TBuf<KMaxCandidateLen> charCode;
    TBool handled = ETrue;
    CAknFepHwrBxStateBase* state = NULL;
        
    switch (aEventType)
        {
        case EHwrEventNewWriting:
            //submit the first char to fep
            iDataStore->GetCandidate(0, charCode);
            iLayout->SubmitL(charCode);

            iDataStore->ClearCandidates();
            // fall-through intended here
        case EHwrEventBeginWriting:
            state = CAknFepHwrBxStateBeginWriting::NewLC(iLayout);
            iLayout->SetCurrentStateL(state);
            CleanupStack::Pop(state);//state
            break;
        case EHwrEventCandidateSelected:
            OnCandidateSelectedL(aEventData);
            break;
        case EHwrEventKeyBack:
            // fall-through intended here
        case EHwrEventPointerOutsideWnd:
            state = CAknFepHwrBxStateStandby::NewLC(iLayout);
            iLayout->SetCurrentStateL(state);
            CleanupStack::Pop(state);//state
            
            break;
        case EHwrEventEndWriting:
            {
            iDataStore->GetCandidate(0, charCode);
            iLayout->SubmitL(charCode);

            TPtrC16 ptrBack;
            TPtrC16 ptrEnter;
            ptrBack.Set(&KBackRep, sizeof(KBackRep)/2);
            ptrEnter.Set(&KEnterRep, sizeof(KEnterRep)/2);

            //if char code is backspace or enter
            if (charCode.CompareC(ptrBack) == 0 || charCode.CompareC(ptrEnter) == 0)
                {
                state = CAknFepHwrBxStateStandby::NewLC(iLayout);
                iLayout->SetCurrentStateL(state);
                }
            else
                {
                state = CAknFepHwrBxStateCandidateSelecting::NewLC(iLayout);
                iLayout->SetCurrentStateL(state);
                }
        
            CleanupStack::Pop(state);//state
            break;
            }
        case EHwrEventWindowClose:
            iDataStore->GetCandidate(0, charCode);
            iLayout->SubmitL(charCode);
            
            //jump to init state
            state = CAknFepHwrBxStateInit::NewLC(iLayout);
            iLayout->SetCurrentStateL(state);
            CleanupStack::Pop(state);

            //record the close mode: manully
            static_cast<CAknFepHwrBxStateInit*>(state)->SetAutoClosed(EFalse);
            handled = EFalse;
            break;        
        default:
            iDataStore->GetCandidate(0, charCode);
            iLayout->SubmitL(charCode);
            
            state = CAknFepHwrBxStateStandby::NewLC(iLayout);
            iLayout->SetCurrentStateL(state);
            CleanupStack::Pop(state);//state
            
            handled = EFalse;
            break;
        }
        
    return handled;
    }
     
// ---------------------------------------------------------
// Handle raw event key
// ---------------------------------------------------------
//
TBool CAknFepHwrBxStateEndWriting::HandleKeyEventL(const TRawEvent& /*aKeyEvent*/)
    {
    //code is removed because of submit char to wrong editor when focus changed
    /*
    TBuf<KMaxCandidateLen> charCode;

    iDataStore->GetCandidate(0, charCode);
    iLayout->SubmitL(charCode);
    
    CAknFepHwrBxStateBase* state = CAknFepHwrBxStateStandby::NewLC(iLayout);
    iLayout->SetCurrentStateL(state);
    CleanupStack::Pop(state);//state
    */
    return EFalse;
    }
     
// ---------------------------------------------------------
// Handle EHwrEventCandidateSelected
// ---------------------------------------------------------
//
void CAknFepHwrBxStateEndWriting::OnCandidateSelectedL(const TDesC& aEventData)
    {
    CAknFepHwrBxStateBase* state = NULL;

    //cancel writing timer
    iWnd->CancelWriting();

    //There must be a charcode and length at least
    if (aEventData.Length() > 1)
        {
        //get the candidate index
        const TInt cellNo = aEventData[aEventData.Length()-1];
        
        //get and submit the candidate to fep
        TBuf<KMaxCandidateLen> charCode;
        iDataStore->GetCandidate(cellNo, charCode);
        iLayout->SubmitL(charCode);
        
        if (iDataStore->IsChinese(cellNo))
            {
            iDataStore->TriggerStr()->Set(aEventData.Left(aEventData.Length() - 1));
            iDataStore->DoPredictiveL();
            
            if (iDataStore->PredictiveCount()>0)
                {
                state = CAknFepHwrBxStatePredictiveSelecting::NewLC(iLayout);
                iLayout->SetCurrentStateL(state);
                }
            else
                {
                state = CAknFepHwrBxStateStandby::NewLC(iLayout);
                iLayout->SetCurrentStateL(state);
                }
            }
        else
            {
            state = CAknFepHwrBxStateStandby::NewLC(iLayout);
            iLayout->SetCurrentStateL(state);
            }
        CleanupStack::Pop(state);//state
        }
    }
    
// ---------------------------------------------------------
// Doing something before exit a state
// ---------------------------------------------------------
//
void CAknFepHwrBxStateEndWriting::OnExit()
    {
    //enable draging
    //iWnd->SetDragEnable(ETrue);
    iWnd->DimInputContextField(EFalse);
    }

    
//CAknFepHwrBxStateEndWriting end

//CAknFepHwrBxStateCandidateSelecting

// ---------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------
//
CAknFepHwrBxStateCandidateSelecting* CAknFepHwrBxStateCandidateSelecting::NewLC(
                                                                    CAknFepHwrBxLayout* aLayout)
    {
    CAknFepHwrBxStateCandidateSelecting* self = new (ELeave) 
                                                CAknFepHwrBxStateCandidateSelecting(aLayout);
    CleanupStack::PushL(self);
    
    return self;
    }

// ---------------------------------------------------------
// C++ default constructor
// ---------------------------------------------------------
//
CAknFepHwrBxStateCandidateSelecting::CAknFepHwrBxStateCandidateSelecting(
                                                                    CAknFepHwrBxLayout* aLayout)
: CAknFepHwrBxStateCloseable(aLayout)
    {
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
CAknFepHwrBxStateCandidateSelecting::~CAknFepHwrBxStateCandidateSelecting()
    {
    }

// ---------------------------------------------------------
// Handle event
// ---------------------------------------------------------
//
TBool CAknFepHwrBxStateCandidateSelecting::HandleEventL(TInt aEventType, const TDesC& aEventData)
    {
    TBool handled = ETrue;    
    CAknFepHwrBxStateBase* state = NULL;

    switch (aEventType)
        {
        case EHwrEventBeginWriting:
            iDataStore->ClearCandidates();
            state = CAknFepHwrBxStateBeginWriting::NewLC(iLayout);
            iLayout->SetCurrentStateL(state);
            CleanupStack::Pop(state);//state
            break;
        case EHwrEventCandidateSelected:
            OnCandidateSelectedL(aEventData);
            break;
        case EHwrEventWindowClose:
            //jump to init state
            state = CAknFepHwrBxStateInit::NewLC(iLayout);
            iLayout->SetCurrentStateL(state);
            CleanupStack::Pop(state);//state
            
            //record the close mode: manully
            static_cast<CAknFepHwrBxStateInit*>(state)->SetAutoClosed(EFalse);
            handled = EFalse;
            break;
        default:
            iDataStore->ClearCandidates();
            state = CAknFepHwrBxStateStandby::NewLC(iLayout);
            iLayout->SetCurrentStateL(state);
            CleanupStack::Pop(state);//state
            
            handled = EFalse;
            break;
        }
        
    return handled;
    }
     
// ---------------------------------------------------------
// Handle raw event key
// ---------------------------------------------------------
//
TBool CAknFepHwrBxStateCandidateSelecting::HandleKeyEventL(const TRawEvent& /*aKeyEvent*/)
    {
    CAknFepHwrBxStateBase* state = CAknFepHwrBxStateStandby::NewLC(iLayout);

    iLayout->SetCurrentStateL(state);
    CleanupStack::Pop(state);//state
    
    return EFalse;
    }
     
// ---------------------------------------------------------
// Doing something before entry a state
// ---------------------------------------------------------
//
void CAknFepHwrBxStateCandidateSelecting::OnEntry()
    {
    if (iDataStore->CandidateCount() > 0)
        {
        //hight the first
        iWnd->HighlightCell(0);
        }
    }

// ---------------------------------------------------------
// Handle EHwrEventCandidateSelected
// ---------------------------------------------------------
//
void CAknFepHwrBxStateCandidateSelecting::OnCandidateSelectedL(const TDesC& aEventData)
    {
    CAknFepHwrBxStateBase* state = NULL;

    if (aEventData.Length() > 1)
        {
        //get the candidate index
        const TInt cellNo = aEventData[aEventData.Length()-1];
        
        //not the first candidate, replace previouse char
        if (cellNo > 0)
            {
            //get the first candidate 
            TBuf<KMaxCandidateLen> oldCharCode;
            iDataStore->GetCandidate(0, oldCharCode);
            
            //get the new candidate 
            TBuf<KMaxCandidateLen> newCharCode;
            iDataStore->GetCandidate(cellNo, newCharCode);
            
            iLayout->TranslateCharCode( newCharCode );
            
            //delete the first candidate submitted and submit new
            iLayout->ReplaceL(oldCharCode, newCharCode);
            
            TPtrC16 ptrBack;
            TPtrC16 ptrEnter;
            ptrBack.Set(&KBackRep, sizeof(KBackRep)/2);
            ptrEnter.Set(&KEnterRep, sizeof(KEnterRep)/2);
            
            if ((newCharCode.Compare(ptrBack) == 0) || (newCharCode.Compare(ptrEnter) == 0))
            	{
                state = CAknFepHwrBxStateStandby::NewLC(iLayout);
                iLayout->SetCurrentStateL(state);
                CleanupStack::Pop(state);
                return;
            	}
            }

        if (iDataStore->IsChinese(cellNo))
            {
            iDataStore->TriggerStr()->Set(aEventData.Left(aEventData.Length()-1));

            iDataStore->DoPredictiveL();
            if (iDataStore->PredictiveCount() > 0)
                {
                state = CAknFepHwrBxStatePredictiveSelecting::NewLC(iLayout);
                iLayout->SetCurrentStateL(state);
                }
            else
                {
                state = CAknFepHwrBxStateStandby::NewLC(iLayout);
                iLayout->SetCurrentStateL(state);
                }
            }
        else
            {
            state = CAknFepHwrBxStateStandby::NewLC(iLayout);
            iLayout->SetCurrentStateL(state);
            }

        CleanupStack::Pop(state);//state
        }
    }

//CAknFepHwrBxStateCandidateSelecting end

//CAknFepHwrBxStatePredictiveSelecting

// ---------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------
//
CAknFepHwrBxStatePredictiveSelecting* CAknFepHwrBxStatePredictiveSelecting::NewLC(
                                                                    CAknFepHwrBxLayout* aLayout)
    {
    CAknFepHwrBxStatePredictiveSelecting* self = new (ELeave) 
                                              CAknFepHwrBxStatePredictiveSelecting(aLayout);
    CleanupStack::PushL(self);
    
    return self;
    }

// ---------------------------------------------------------
// C++ default constructor
// ---------------------------------------------------------
//
CAknFepHwrBxStatePredictiveSelecting::CAknFepHwrBxStatePredictiveSelecting(
                                                                    CAknFepHwrBxLayout* aLayout)
: CAknFepHwrBxStateCloseable(aLayout)
    {
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
CAknFepHwrBxStatePredictiveSelecting::~CAknFepHwrBxStatePredictiveSelecting()
    {
    }

// ---------------------------------------------------------
// Doing something before enter a state
// ---------------------------------------------------------
//
void CAknFepHwrBxStatePredictiveSelecting::OnEntry()
    {
    //fill the candidat list
    iDataStore->SetPredictives();
    }

// ---------------------------------------------------------
// Handle event
// ---------------------------------------------------------
//
TBool CAknFepHwrBxStatePredictiveSelecting::HandleEventL(TInt aEventType, const TDesC& aEventData)
    {
    TBool handled = ETrue;
    CAknFepHwrBxStateBase* state = NULL;
        
    switch (aEventType)
        {
        case EHwrEventBeginWriting:
            iDataStore->ClearCandidates();
            state = CAknFepHwrBxStateBeginWriting::NewLC(iLayout);
            iLayout->SetCurrentStateL(state);
            CleanupStack::Pop(state);//state
            break;
        case EHwrEventCandidateSelected:
            OnPredictiveSelectedL(aEventData);
            break;
        case EHwrEventKeyBack:
            OnKeyBackL();
            break;
        case EHwrEventWindowClose:
            //jump to init state
            state = CAknFepHwrBxStateInit::NewLC(iLayout);
            iLayout->SetCurrentStateL(state);
            CleanupStack::Pop(state);//state
            
            //record the close mode: manully
            static_cast<CAknFepHwrBxStateInit*>(state)->SetAutoClosed(EFalse);
            handled = EFalse;
            break;
       	case EHwrEventGetNextCandidatePage:
       	    {
       	    handled = EFalse;
       	    }
       	    break;
       	case EHwrEventCandidateExisted:
       	    {
            if (iDataStore->PredictiveCount() >= KMaxPredictCandCnt)
                {
                iWnd->DropdownList()->SetFlagCandidateExist(EFalse);
                }
            else
                {
                iWnd->DropdownList()->SetFlagCandidateExist(
                    iDataStore->PredictiveCandidateExisted());
                }          
            }
            break;
       	case EHwrEventGetPreviousCandidatePage:
       	    // actually, 'get previous page candidate' will do nothing
       	    // here handled is ETrue, so it will not be handled by layout either
       	    // this branch is just to avoid 'press arrow up' button be treated in 
       	    // 'default' branch
       	    break;
        default:
            iDataStore->ClearCandidates();
            state = CAknFepHwrBxStateStandby::NewLC(iLayout);
            iLayout->SetCurrentStateL(state);
            CleanupStack::Pop(state);//state
            
            handled = EFalse;
            break;
        }
        
    return handled;
    }
     
// ---------------------------------------------------------
// Handle raw event key
// ---------------------------------------------------------
//
TBool CAknFepHwrBxStatePredictiveSelecting::HandleKeyEventL(const TRawEvent& /*aKeyEvent*/)
    {
    //CAknFepHwrBxStateBase* state = CAknFepHwrBxStateStandby::NewLC(iLayout);

    //iLayout->SetCurrentStateL(state);
    //CleanupStack::Pop(state);//state
    
    return EFalse;
    }

// ---------------------------------------------------------
// Handle EHwrEventPredictiveSelected
// ---------------------------------------------------------
//
void CAknFepHwrBxStatePredictiveSelecting::OnPredictiveSelectedL(const TDesC& aEventData)
    {
    //There must be a charcode and length at least
    if (aEventData.Length() > 1)
        {
        //get the Predictive index
        const TInt cellNo = aEventData[aEventData.Length()-1];
        
        //get and submit the Predictive to fep
        TBuf<KMaxPredictiveLen> charCode;
        iDataStore->GetPredictive(cellNo, charCode);
        iLayout->SubmitL(charCode);
        
        iDataStore->TriggerStr()->Append(aEventData.Left(aEventData.Length()-1));
        iDataStore->DoPredictiveL();
        
        if (iDataStore->PredictiveCount() == 0)
            {
            iDataStore->TriggerStr()->SetLast();
            iDataStore->DoPredictiveL();
            }
            
        if (iDataStore->PredictiveCount() == 0)
            {
            CAknFepHwrBxStateBase* state = CAknFepHwrBxStateStandby::NewLC(iLayout);
            iLayout->SetCurrentStateL(state);
            CleanupStack::Pop(state);//state
            }
        }
    }

// ---------------------------------------------------------
// Handle EHwrEventKeyBack
// ---------------------------------------------------------
//
void CAknFepHwrBxStatePredictiveSelecting::OnKeyBackL()
    {
    CAknFepHwrBxStateBase* state = NULL;

    iLayout->Delete();
    
    //do predictive again
    iDataStore->TriggerStr()->DeleteLast();
    if (iDataStore->TriggerStr()->IsEmpty())
        {
        state = CAknFepHwrBxStateStandby::NewLC(iLayout);
        iLayout->SetCurrentStateL(state);
        CleanupStack::Pop(state);//state
        }
    else
        {
        iDataStore->DoPredictiveL();
        if (iDataStore->PredictiveCount() == 0)
            {
            iDataStore->TriggerStr()->SetLast();
            iDataStore->DoPredictiveL();
            }
        
        if (iDataStore->PredictiveCount() == 0)
            {
            state = CAknFepHwrBxStateStandby::NewLC(iLayout);
            iLayout->SetCurrentStateL(state);
            CleanupStack::Pop(state);//state
            }
        }
    }

//CAknFepHwrBxStatePredictiveSelecting end

// End Of File
