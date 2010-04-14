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
* Description:  Implementation for chinese peninput finger hwr
*
*/
#include <peninputcmd.h>

#include "peninputfingerhwrlayout.h"
#include "peninputfingerhwrwnd.h"
#include "peninputfingerhwrstatemanager.h"
#include "peninputfingerhwrdatastore.h"
#include "peninputfingerhwrevent.h"
#include "peninputfingerhwrtriggerstr.h"


// ============================ MEMBER FUNCTIONS ===============================
//                     CPeninputFingerHwrStateHandlerBase class
// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateHandlerBase::CPeninputFingerHwrStateHandlerBase()
// .
// -----------------------------------------------------------------------------
//
CPeninputFingerHwrStateHandlerBase::CPeninputFingerHwrStateHandlerBase( 
    CPeninputFingerHwrStateManagerBase& aStateManager )
    :iStateManager( aStateManager ),
    iDataStore( aStateManager.HwrLayout().DataStore() )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateHandlerBase::~CPeninputFingerHwrStateHandlerBase()
// .
// -----------------------------------------------------------------------------
//
CPeninputFingerHwrStateHandlerBase::~CPeninputFingerHwrStateHandlerBase()
    {
    
    }


// ============================ MEMBER FUNCTIONS ===============================
//                     CPeninputFingerHwrStateManager class
// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManager::CPeninputFingerHwrStateManager()
// .
// -----------------------------------------------------------------------------
//
CPeninputFingerHwrStateManagerBase::CPeninputFingerHwrStateManagerBase( 
    CPeninputFingerHwrLayout* aLayout )
    :iLayout(aLayout)
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManager::~CPeninputFingerHwrStateManager()
// .
// -----------------------------------------------------------------------------
//
CPeninputFingerHwrStateManagerBase::~CPeninputFingerHwrStateManagerBase()
    {
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManager::CosntructL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrStateManagerBase::BaseConstructL()
    {

    }
    
// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManagerBase::SetState()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputFingerHwrStateManagerBase::SetState( TInt /*aState*/ )
    {

    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManagerBase::IsStandbyState()
// .
// -----------------------------------------------------------------------------
//
TBool CPeninputFingerHwrStateManagerBase::IsStandbyState()
    {
    if(iCurrentState == 0)
        {
        return ETrue;
        }

    return EFalse;
    }


// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManagerBase::HandleEventL()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputFingerHwrStateManagerBase::HandleEventL( const TInt /*aEventType*/,
    const TDesC& /*aEventData*/)
    {

    }

CPeninputFingerHwrLayout& CPeninputFingerHwrStateManagerBase::HwrLayout()
    {
    return *iLayout;
    }

 
// ============================ MEMBER FUNCTIONS ===============================
//                     CPeninputFingerHwrStateManager class
// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManagerCn::NewL()
// .
// -----------------------------------------------------------------------------
//
CPeninputFingerHwrStateManagerCn* CPeninputFingerHwrStateManagerCn::NewL( 
    CPeninputFingerHwrLayout* aLayout )
    {
    CPeninputFingerHwrStateManagerCn* self = new( ELeave ) 
        CPeninputFingerHwrStateManagerCn( aLayout );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManagerCn::CPeninputFingerHwrStateManager()
// .
// -----------------------------------------------------------------------------
//
CPeninputFingerHwrStateManagerCn::CPeninputFingerHwrStateManagerCn( 
    CPeninputFingerHwrLayout* aLayout )
    :CPeninputFingerHwrStateManagerBase( aLayout )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManagerCn::~CPeninputFingerHwrStateManager()
// .
// -----------------------------------------------------------------------------
//
CPeninputFingerHwrStateManagerCn::~CPeninputFingerHwrStateManagerCn()
    {
	for( int i = 0; i < EPeninputFingerHwrStateCountCn; ++i )
		{
		delete iStateHandlers[i];
		}
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManagerCn::CosntructL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrStateManagerCn::ConstructL()
    {
    BaseConstructL();
    
    TInt i = 0;
    
    iStates[i] = EPeninputFingerHwrStateStandbyCn;
    iStateHandlers[i] = CPeninputFingerHwrChineseStateStandby::NewL( *this );

	iStates[++i] = EPeninputFingerHwrStateWritingCn;
    iStateHandlers[i] = CPeninputFingerHwrChineseStateWriting::NewL( *this );
    
	iStates[++i] = EPeninputFingerHwrStateCandidateSelectingCn;
    iStateHandlers[i] = 
        CPeninputFingerHwrChineseStateCandidateSelecting::NewL( *this );
	
	iStates[++i] = EPeninputFingerHwrStatePredictSelectingCn;
    iStateHandlers[i] = 
        CPeninputFingerHwrChineseStatePredictSelecting::NewL( *this );              
  
    iCurrentStateHandler = iStateHandlers[0];
    iCurrentState = EPeninputFingerHwrStateStandbyCn;
    }
    
// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManagerCn::SetState()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputFingerHwrStateManagerCn::SetState( TInt aState )
    {
    if( iCurrentState == aState )        
        {
        return;    
        }
        
    CPeninputFingerHwrStateHandlerBase* newStateHandler = Find( (TPeninputFingerHwrStateCn)aState );
    if( newStateHandler )
        {
        iCurrentState = (TPeninputFingerHwrStateCn)aState;
        iCurrentStateHandler->OnExit();
        iCurrentStateHandler = newStateHandler;
        iCurrentStateHandler->OnEntry();
        }
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManagerCn::Find()
// .
// -----------------------------------------------------------------------------
//  
CPeninputFingerHwrStateHandlerBase* CPeninputFingerHwrStateManagerCn::Find( 
    TPeninputFingerHwrStateCn aState )
    {
    for( TInt i = 0; i < EPeninputFingerHwrStateCountCn; i++ )
        {
        if( iStates[i] == aState )
            {
            return iStateHandlers[i];
            }
        }
    return NULL;
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManagerCn::HandleEventL()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputFingerHwrStateManagerCn::HandleEventL( const TInt aEventType, 
    const TDesC& aEventData )
    {
    iCurrentStateHandler->HandleEventL( aEventType, aEventData );
    }


// ============================ MEMBER FUNCTIONS ===============================
//                     CPeninputFingerHwrStateManagerEn class
// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManagerEn::NewL()
// .
// -----------------------------------------------------------------------------
//
CPeninputFingerHwrStateManagerEn* CPeninputFingerHwrStateManagerEn::NewL( 
    CPeninputFingerHwrLayout* aLayout )
    {
    CPeninputFingerHwrStateManagerEn* self = new( ELeave ) 
        CPeninputFingerHwrStateManagerEn( aLayout );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManagerEn::CPeninputFingerHwrStateManager()
// .
// -----------------------------------------------------------------------------
//
CPeninputFingerHwrStateManagerEn::CPeninputFingerHwrStateManagerEn( 
    CPeninputFingerHwrLayout* aLayout )
    :CPeninputFingerHwrStateManagerBase( aLayout )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManagerEn::~CPeninputFingerHwrStateManager()
// .
// -----------------------------------------------------------------------------
//
CPeninputFingerHwrStateManagerEn::~CPeninputFingerHwrStateManagerEn()
    {
	for( int i = 0; i < EPeninputFingerHwrStateCountEn; ++i )
		{
		delete iStateHandlers[i];
		}
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManagerEn::CosntructL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrStateManagerEn::ConstructL()
    {
    BaseConstructL();
    
    TInt i = 0;
    
    iStates[i] = EPeninputFingerHwrStateStandbyEn;
    iStateHandlers[i] = CPeninputFingerHwrEnglishStateStandby::NewL( *this );

	iStates[++i] = EPeninputFingerHwrStateWritingEn;
    iStateHandlers[i] = CPeninputFingerHwrEnglishStateWriting::NewL( *this );
    
	iStates[++i] = EPeninputFingerHwrStateCandidateSelectingEn;
    iStateHandlers[i] = 
        CPeninputFingerHwrEnglishStateCandidateSelecting::NewL( *this );
	
#ifdef RD_INTELLIGENT_TEXT_INPUT	
	iStates[++i] = EPeninputFingerHwrStatePredictSelectingEn;
    iStateHandlers[i] = 
        CPeninputFingerHwrEnglishStatePredictSelecting::NewL( *this );
#endif // RD_INTELLIGENT_TEXT_INPUT
  
    iCurrentStateHandler = iStateHandlers[0];
    iCurrentState = EPeninputFingerHwrStateStandbyEn;
    }
    
// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManagerEn::SetState()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputFingerHwrStateManagerEn::SetState( TInt aState )
    {
    if( iCurrentState == aState )        
        {
        return;    
        }
        
    CPeninputFingerHwrStateHandlerBase* newStateHandler = 
            Find((TPeninputFingerHwrStateEn)aState);
    if( newStateHandler )
        {
        iCurrentState = (TPeninputFingerHwrStateEn)aState;
        iCurrentStateHandler->OnExit();
        iCurrentStateHandler = newStateHandler;
        iCurrentStateHandler->OnEntry();
        }
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManagerEn::Find()
// .
// -----------------------------------------------------------------------------
//  
CPeninputFingerHwrStateHandlerBase* CPeninputFingerHwrStateManagerEn::Find( 
    TPeninputFingerHwrStateEn aState )
    {
    for( TInt i = 0; i < EPeninputFingerHwrStateCountEn; i++ )
        {
        if( iStates[i] == aState )
            {
            return iStateHandlers[i];
            }
        }
    return NULL;
    }
        

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManagerEn::HandleEventL()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputFingerHwrStateManagerEn::HandleEventL( const TInt aEventType, 
    const TDesC& aEventData )
    {
    iCurrentStateHandler->HandleEventL( aEventType, aEventData );
    }


// ============================ MEMBER FUNCTIONS ===============================
//                     CPeninputFingerHwrStateManagerNum class
// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManagerNum::NewL()
// .
// -----------------------------------------------------------------------------
//
CPeninputFingerHwrStateManagerNum* CPeninputFingerHwrStateManagerNum::NewL( 
    CPeninputFingerHwrLayout* aLayout )
    {
    CPeninputFingerHwrStateManagerNum* self = new( ELeave ) 
        CPeninputFingerHwrStateManagerNum( aLayout );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManagerNum::CPeninputFingerHwrStateManagerNum()
// .
// -----------------------------------------------------------------------------
//
CPeninputFingerHwrStateManagerNum::CPeninputFingerHwrStateManagerNum( 
    CPeninputFingerHwrLayout* aLayout )
    :CPeninputFingerHwrStateManagerBase( aLayout )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManagerNum::~CPeninputFingerHwrStateManagerNum()
// .
// -----------------------------------------------------------------------------
//
CPeninputFingerHwrStateManagerNum::~CPeninputFingerHwrStateManagerNum()
    {
	for( int i = 0; i < EPeninputFingerHwrStateCountNum; ++i )
		{
		delete iStateHandlers[i];
		}
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManagerNum::CosntructL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrStateManagerNum::ConstructL()
    {
    BaseConstructL();
    
    TInt i = 0;
    
    iStates[i] = EPeninputFingerHwrStateStandbyNum;
    iStateHandlers[i] = CPeninputFingerHwrNumericStateStandby::NewL( *this );
  
    iCurrentStateHandler = iStateHandlers[0];
    iCurrentState = EPeninputFingerHwrStateStandbyNum;
    }
    
// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManagerNum::SetState()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputFingerHwrStateManagerNum::SetState( TInt aState )
    {
    if( iCurrentState == aState )        
        {
        return;    
        }
        
    CPeninputFingerHwrStateHandlerBase* newStateHandler = 
            Find((TPeninputFingerHwrStateNum)aState );
    if( newStateHandler )
        {
        iCurrentState = (TPeninputFingerHwrStateNum)aState;
        iCurrentStateHandler->OnExit();
        iCurrentStateHandler = newStateHandler;
        iCurrentStateHandler->OnEntry();
        }
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManagerNum::Find()
// .
// -----------------------------------------------------------------------------
//  
CPeninputFingerHwrStateHandlerBase* CPeninputFingerHwrStateManagerNum::Find( 
    TPeninputFingerHwrStateNum aState )
    {
    for( TInt i = 0; i < EPeninputFingerHwrStateCountNum; i++ )
        {
        if( iStates[i] == aState )
            {
            return iStateHandlers[i];
            }
        }
    return NULL;
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManagerNum::HandleEventL()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputFingerHwrStateManagerNum::HandleEventL( const TInt aEventType, 
    const TDesC& aEventData )
    {
    iCurrentStateHandler->HandleEventL( aEventType, aEventData );
    }    
    

// ============================ MEMBER FUNCTIONS ===============================
//                     CPeninputFingerHwrStateHandlerStandby class
// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateHandlerStandby::CPeninputFingerHwrStateHandlerStandby()
// .
// -----------------------------------------------------------------------------
// 
CPeninputFingerHwrChineseStateStandby::CPeninputFingerHwrChineseStateStandby( 
	CPeninputFingerHwrStateManagerCn& aStateManager )
	:CPeninputFingerHwrStateHandlerBase( aStateManager )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateHandlerStandby::NewL()
// .
// -----------------------------------------------------------------------------
// 
CPeninputFingerHwrChineseStateStandby* CPeninputFingerHwrChineseStateStandby::NewL( 
	CPeninputFingerHwrStateManagerCn& aStateManager )
    {
    CPeninputFingerHwrChineseStateStandby* self = new( ELeave ) 
        CPeninputFingerHwrChineseStateStandby( aStateManager );
    
    return self;
                                                                                
    }
    

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateHandlerStandby::HandleEventL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrChineseStateStandby::HandleEventL( const TInt aEventType, 
    const TDesC& /*aEventData*/ )
    {
    switch( aEventType )
        {
        case EEventHwrStrokeStarted:
            {
            iStateManager.SetState( 
                CPeninputFingerHwrStateManagerCn::EPeninputFingerHwrStateWritingCn );
            }
            break;
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateHandlerStandby::OnEntry()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrChineseStateStandby::OnEntry()
    {
    iDataStore.TriggerStr()->DeleteAll();
    
#ifdef RD_INTELLIGENT_TEXT_INPUT
    // Set start position flag to false and disable part relace function
    iStateManager.HwrLayout().SubmitStartPosToFep(EFalse);
#endif // RD_INTELLIGENT_TEXT_INPUT    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateHandlerStandby::OnExit()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrChineseStateStandby::OnExit()
    {

    }

// ============================ MEMBER FUNCTIONS ===============================
//                     CPeninputFingerHwrStateHandlerWriting class
// -----------------------------------------------------------------------------
// CPeninputFingerHwrChineseStateWriting::CPeninputFingerHwrStateHandlerWriting()
// .
// -----------------------------------------------------------------------------
// 
CPeninputFingerHwrChineseStateWriting::CPeninputFingerHwrChineseStateWriting( 
	CPeninputFingerHwrStateManagerCn& aStateManager )
	:CPeninputFingerHwrStateHandlerBase( aStateManager )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrChineseStateWriting::NewL()
// .
// -----------------------------------------------------------------------------
// 
CPeninputFingerHwrChineseStateWriting* CPeninputFingerHwrChineseStateWriting::NewL( 
	CPeninputFingerHwrStateManagerCn& aStateManager )
    {
    CPeninputFingerHwrChineseStateWriting* self = new( ELeave ) 
        CPeninputFingerHwrChineseStateWriting( aStateManager );
    
    return self;
                                                                                
    }
    

// -----------------------------------------------------------------------------
// CPeninputFingerHwrChineseStateWriting::HandleEventL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrChineseStateWriting::HandleEventL( const TInt aEventType, 
    const TDesC& aEventData )
    {
    switch( aEventType )
        {
        case EEventHwrStrokeFinished:
            {
            OnEndStrokeL(aEventData);
            }
            break;
        case EEventHwrCharacterTimerOut:
            {
            OnEndWritingL( aEventData );
            }
            break;
        case EHwrEventCandidateSelected:
            {
            OnSelectedCandidatesL(aEventData);
            }    
            break;
        case EHwrEventPredictiveSelected:
            {
            OnSelectedPredictivesL(aEventData);
            }
            break;
        case EHwrEventKeyBack:
            {
            OnClickBackSpaceL();
            }
            break;
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrChineseStateWriting::OnEndStrokeL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrChineseStateWriting::OnEndStrokeL( const TDesC& aEventData )
	{
	const RArray<TPoint>* aTraceData = ( RArray<TPoint>* )aEventData.Ptr();
    iDataStore.DoChineseRecognizeL( *aTraceData );
    
    if(iDataStore.PredictiveCount() <= 0)
        {
        iDataStore.SetFixChinesePredictiveL();
        }
	}


// -----------------------------------------------------------------------------
// CPeninputFingerHwrChineseStateWriting::OnEndWriting()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrChineseStateWriting::OnEndWritingL( const TDesC& /*aEventData*/ )
	{
    // record the default cell highlight flag.
    iDataStore.SetHighlight(ETrue);
    
    RPointerArray<HBufC> candidates = iDataStore.Candidate();
    if ( candidates.Count() > 0 )
        {
        HBufC* defaultCandiate = candidates[0];
        
        if( iDataStore.IsSpecialDisplayChars( *defaultCandiate ) )
            {
            HBufC* realCandidate = iDataStore.ConvertDisplayChars( *defaultCandiate );
            iStateManager.HwrLayout().SubmitStringToFep( *realCandidate );
            delete realCandidate;
            }
        else
            {
            iStateManager.HwrLayout().SubmitStringToFep( *defaultCandiate );
            }
            
        //check if the default candidate is need to be sent directly
        if( iDataStore.IsDirectlySentCandidate( *defaultCandiate ) )
            {
            iStateManager.SetState( CPeninputFingerHwrStateManagerCn::
                EPeninputFingerHwrStateStandbyCn );
            }
        else
            {
            if(iDataStore.PredictiveCount() <= 0)
                {
                iDataStore.TriggerStr()->DeleteAll();
                iDataStore.SetFixChinesePredictiveL();
                }            
            
            iStateManager.SetState( CPeninputFingerHwrStateManagerCn::
                EPeninputFingerHwrStateCandidateSelectingCn );  
            }        
        
        } 
	}
	
// -----------------------------------------------------------------------------
// CPeninputFingerHwrChineseStateWriting::OnClickBackSpaceL()
// .
// -----------------------------------------------------------------------------
//	
void CPeninputFingerHwrChineseStateWriting::OnClickBackSpaceL()
    {
    iDataStore.SetHighlight(EFalse);                                                        
    iDataStore.TriggerStr()->DeleteLast();
    iDataStore.SetChineseSymbolL();
    iDataStore.SetFixChinesePredictiveL();
    iStateManager.SetState( CPeninputFingerHwrStateManagerCn::
            EPeninputFingerHwrStatePredictSelectingCn );
    }
	

// -----------------------------------------------------------------------------
// CPeninputFingerHwrChineseStateWriting::OnSelectedCandidatesL()
// .
// -----------------------------------------------------------------------------
//	
void CPeninputFingerHwrChineseStateWriting::OnSelectedCandidatesL( 
    const TDesC& aEventData )
    {
    const TInt cellNo = aEventData[aEventData.Length() - 1];
    if (!iDataStore.IsValidCandidate( cellNo ))
        {
        iStateManager.SetState(CPeninputFingerHwrStateManagerCn::
                        EPeninputFingerHwrStateStandbyCn );
        return;
        }    
    
    TPtrC ptr;
    ptr.Set(( aEventData.Left( aEventData.Length() - 1 ) ) );
    RPointerArray<HBufC> candidates = iDataStore.Candidate();

    //handle special display char
    if( iDataStore.IsSpecialDisplayChars( ptr ) )
        {
        HBufC* realCandidate = iDataStore.ConvertDisplayChars( ptr );
        iStateManager.HwrLayout().SubmitStringToFep( ptr );
        delete realCandidate;
        }
    else
        {
        iStateManager.HwrLayout().SubmitStringToFep( ptr );         
        }        
        
    iDataStore.TriggerStr()->Set( ptr );     
        
    iDataStore.DoPredictiveL();
    iDataStore.SetChineseSymbolL();
    
    // record the default cell highlight flag
    iDataStore.SetHighlight(EFalse);
    
    if( iDataStore.PredictiveCount() <= 0 )
        {
        iDataStore.TriggerStr()->DeleteAll();
        iDataStore.SetFixChinesePredictiveL();
        }
    
    iStateManager.SetState( 
        CPeninputFingerHwrStateManagerCn::
        EPeninputFingerHwrStatePredictSelectingCn );     
    }
    
// -----------------------------------------------------------------------------
// CPeninputFingerHwrChineseStateWriting::OnSelectedPredictivesL()
// .
// -----------------------------------------------------------------------------
//	
void CPeninputFingerHwrChineseStateWriting::OnSelectedPredictivesL( 
    const TDesC& aEventData )
    {
    // 1. submit the default candidate
    RPointerArray<HBufC> candidates = iDataStore.Candidate();
    HBufC* defaultCandidate = candidates[0];
       
    if( iDataStore.IsSpecialDisplayChars( *defaultCandidate ) )
        {
        // convert special display chars
        HBufC* realCandidate = iDataStore.ConvertDisplayChars( *defaultCandidate );
        if( defaultCandidate->Compare( KDisplayBackspace ) != 0 )
            {
            iStateManager.HwrLayout().SubmitStringToFep( realCandidate->Des());
            }
        else
            {
            // to delete the last char in ICF
            iStateManager.HwrLayout().Replace(*defaultCandidate, *realCandidate);
            }    
        delete realCandidate;
        }
    else
        {
        iStateManager.HwrLayout().SubmitStringToFep( *defaultCandidate );
        }
       
    // 2. submit the selected prediction
    TPtrC ptr;
    ptr.Set( aEventData.Left( aEventData.Length() - 1 ) );   
    iStateManager.HwrLayout().SubmitStringToFep( ptr );
    
    // 3. update trigger string
    iDataStore.TriggerStr()->Append( ptr );    
    
    // 4. update predictive   
    iDataStore.DoPredictiveL();
    iDataStore.SetChineseSymbolL();
    iDataStore.SetHighlight(EFalse);
    
    if( iDataStore.PredictiveCount() <= 0 )
        {
        iDataStore.TriggerStr()->DeleteAll();
        iDataStore.SetFixChinesePredictiveL();   
        }
    
    iStateManager.SetState( 
        CPeninputFingerHwrStateManagerCn::
        EPeninputFingerHwrStatePredictSelectingCn );     
    }
	

// -----------------------------------------------------------------------------
// CPeninputFingerHwrChineseStateWriting::OnEntry()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrChineseStateWriting::OnEntry()
    {
     
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrChineseStateWriting::OnExit()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrChineseStateWriting::OnExit()
    {
    
    }

// ============================ MEMBER FUNCTIONS ===============================
//                     CPeninputFingerHwrStateHandlerCandidateSelecting class
// -----------------------------------------------------------------------------
// CPeninputFingerHwrChineseStateCandidateSelecting::
//                              CPeninputFingerHwrChineseStateCandidateSelecting()
// .
// -----------------------------------------------------------------------------
// 
CPeninputFingerHwrChineseStateCandidateSelecting::
    CPeninputFingerHwrChineseStateCandidateSelecting( 
	CPeninputFingerHwrStateManagerCn& aStateManager )
	:CPeninputFingerHwrStateHandlerBase( aStateManager )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrChineseStateCandidateSelecting::NewL()
// .
// -----------------------------------------------------------------------------
// 
CPeninputFingerHwrChineseStateCandidateSelecting* 
    CPeninputFingerHwrChineseStateCandidateSelecting::NewL( 
	CPeninputFingerHwrStateManagerCn& aStateManager )
    {
    CPeninputFingerHwrChineseStateCandidateSelecting* self = new( ELeave ) 
        CPeninputFingerHwrChineseStateCandidateSelecting( aStateManager );
    
    return self;                                                                                
    }


// -----------------------------------------------------------------------------
// CPeninputFingerHwrChineseStateCandidateSelecting::HandleEventL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrChineseStateCandidateSelecting::HandleEventL( const TInt aEventType, 
    const TDesC& aEventData )
    {
    switch( aEventType )
        {
        case EHwrEventCandidateSelected:
            {
            OnSelectedCandidatesL( aEventData );
            }
            break;
        case EHwrEventPredictiveSelected:
            {
            OnSelectedPredictivesL( aEventData );
            }
            break;
        case EHwrEventKeyBack:
            {
            OnClickBackSpaceL();
            }
            break;
        case EEventHwrStrokeStarted:
            {
            iStateManager.SetState( CPeninputFingerHwrStateManagerCn::
                EPeninputFingerHwrStateWritingCn );
            }
            break;                     
        default:
            break;
        }
    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateHandlerCandidateSelecting::OnSelectedCandidatesL()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputFingerHwrChineseStateCandidateSelecting::OnSelectedCandidatesL( 
    const TDesC& aEventData )
    {
    //get the candidate index
    const TInt cellNo = aEventData[aEventData.Length()-1];
    if (!iDataStore.IsValidCandidate( cellNo ))
        {
        iStateManager.SetState( CPeninputFingerHwrStateManagerCn::
                        EPeninputFingerHwrStateStandbyCn );
        return;
        }    
    
    TPtrC ptr;
    ptr.Set( ( aEventData.Left( aEventData.Length() - 1 ) ) );
    RPointerArray<HBufC> candidates = iDataStore.Candidate();
    //the selected char is not same as default
    //replace the default candidate
    HBufC* defaultCandidate = candidates[0];
    if( *defaultCandidate != ptr )
        {
        if( iDataStore.IsSpecialDisplayChars( ptr ) )
            {
            HBufC* realCandidate = iDataStore.ConvertDisplayChars( ptr );
            iStateManager.HwrLayout().Replace( *defaultCandidate, 
                *realCandidate );
            delete realCandidate;
            }
        else
            {
            iStateManager.HwrLayout().Replace( *defaultCandidate, ptr );         
            }        
        }
    OnSelectedPredictivesL( aEventData, EFalse );
    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateHandlerCandidateSelecting::OnSelectedCandidatesL()
// .
// -----------------------------------------------------------------------------
// 
void CPeninputFingerHwrChineseStateCandidateSelecting::OnSelectedPredictivesL( 
    const TDesC& aEventData,
    TBool aAppend )
    {                                                        
    TPtrC ptr;
    ptr.Set( aEventData.Left( aEventData.Length() - 1 ) );
    // aAppend means we need append the prt to current trigger string and submit 
    // ptr to forground app    
    if( aAppend )
        {
        iStateManager.HwrLayout().SubmitStringToFep( ptr );
        iDataStore.TriggerStr()->Append( ptr );    
        }
    else
        {
        iDataStore.TriggerStr()->Set( ptr );
        }
        
    iDataStore.DoPredictiveL();
    iDataStore.SetChineseSymbolL();
    iDataStore.SetHighlight(EFalse);
    
    if( iDataStore.PredictiveCount() <= 0 )
        {
        iDataStore.TriggerStr()->DeleteAll();
        iDataStore.SetFixChinesePredictiveL();   
        }
    
    iStateManager.SetState( 
        CPeninputFingerHwrStateManagerCn::
        EPeninputFingerHwrStatePredictSelectingCn );     
    
    }
    
// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateHandlerCandidateSelecting::OnClickBackSpaceL()
// .
// -----------------------------------------------------------------------------
// 
void CPeninputFingerHwrChineseStateCandidateSelecting::OnClickBackSpaceL()
    {
    iDataStore.SetHighlight(EFalse);                                                        
    iDataStore.TriggerStr()->DeleteLast();
    iDataStore.SetChineseSymbolL();
    iDataStore.SetFixChinesePredictiveL();
    iStateManager.SetState( CPeninputFingerHwrStateManagerCn::
            EPeninputFingerHwrStatePredictSelectingCn );
    }
    
    
        
// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateHandlerCandidateSelecting::OnEntry()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrChineseStateCandidateSelecting::OnEntry()
    {
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateHandlerCandidateSelecting::OnExit()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrChineseStateCandidateSelecting::OnExit()
    {
    
    }


// ============================ MEMBER FUNCTIONS ===============================
//                     CPeninputFingerHwrStateHandlerPredictSelecting class
// -----------------------------------------------------------------------------
// CPeninputFingerHwrChineseStatePredictSelecting::
//                                CPeninputFingerHwrStateHandlerPredictSelecting()
// .
// -----------------------------------------------------------------------------
// 
CPeninputFingerHwrChineseStatePredictSelecting::
    CPeninputFingerHwrChineseStatePredictSelecting( 
	CPeninputFingerHwrStateManagerCn& aStateManager )
	:CPeninputFingerHwrStateHandlerBase( aStateManager )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrChineseStatePredictSelecting::NewL()
// .
// -----------------------------------------------------------------------------
// 
CPeninputFingerHwrChineseStatePredictSelecting*  
    CPeninputFingerHwrChineseStatePredictSelecting::NewL( 
	CPeninputFingerHwrStateManagerCn& aStateManager )
    {
    CPeninputFingerHwrChineseStatePredictSelecting* self = new( ELeave ) 
        CPeninputFingerHwrChineseStatePredictSelecting( aStateManager );
    
    return self;
                                                                                
    }


// -----------------------------------------------------------------------------
// CPeninputFingerHwrChineseStatePredictSelecting::HandleEventL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrChineseStatePredictSelecting::HandleEventL( const TInt aEventType, 
    const TDesC& aEventData )
    {
    switch( aEventType )
        {
        case EHwrEventCandidateSelected:
            {
            OnSelectedCandidatesL(aEventData);
            }
            break;
        case EHwrEventPredictiveSelected:
            {
            OnSelectedPredictivesL( aEventData );
            }
            break;
        case EHwrEventKeyBack:
            {
            OnClickBackSpaceL();
            }
            break;
        case EEventHwrStrokeStarted:
            {
            iStateManager.SetState( CPeninputFingerHwrStateManagerCn::
                EPeninputFingerHwrStateWritingCn );
            }
            break;            
        default:
            break;
        }    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrChineseStatePredictSelecting::OnSelectedCandidatesL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrChineseStatePredictSelecting::OnSelectedCandidatesL( 
    const TDesC& aEventData )
    {
    TPtrC ptr;
    ptr.Set( aEventData.Left( aEventData.Length() - 1 ) );
    iStateManager.HwrLayout().SubmitStringToFep( ptr );
    
    // When select offen-use symbol, clear trigger string
    iDataStore.TriggerStr()->DeleteAll();
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrChineseStatePredictSelecting::OnSelectedCandidatesL()
// .
// -----------------------------------------------------------------------------
// 
void CPeninputFingerHwrChineseStatePredictSelecting::OnSelectedPredictivesL( 
    const TDesC& aEventData )
    {                                                        
    TPtrC ptr;
    ptr.Set( aEventData.Left( aEventData.Length() - 1 ) );
    if( iDataStore.IsSpecialDisplayChars( ptr ) )
        {
        HBufC* str = iDataStore.ConvertDisplayChars( ptr );
        iStateManager.HwrLayout().SubmitStringToFep( *str );
        delete str;
        }
    else
        {
        iStateManager.HwrLayout().SubmitStringToFep( ptr );
        }
    
    iDataStore.TriggerStr()->Append( ptr );
    iDataStore.DoPredictiveL();
    if( iDataStore.PredictiveCount() <= 0 )
        {
        iDataStore.TriggerStr()->DeleteAll();
        iDataStore.SetFixChinesePredictiveL();     
        }        
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrChineseStatePredictSelecting::OnClickBackSpaceL()
// .
// -----------------------------------------------------------------------------
// 
void CPeninputFingerHwrChineseStatePredictSelecting::OnClickBackSpaceL()
    {
    iDataStore.TriggerStr()->DeleteAll();
    iStateManager.SetState(CPeninputFingerHwrStateManagerCn::EPeninputFingerHwrStateStandbyCn);
    }

   
// -----------------------------------------------------------------------------
// CPeninputFingerHwrChineseStatePredictSelecting::OnEntry()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrChineseStatePredictSelecting::OnEntry()
    {

    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrChineseStatePredictSelecting::OnExit()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrChineseStatePredictSelecting::OnExit()
    {
    
    }




// ============================ MEMBER FUNCTIONS ===============================
//                     CPeninputFingerHwrEnglishStateStandby class
// -----------------------------------------------------------------------------
// CPeninputFingerHwrEnglishStateStandby::CPeninputFingerHwrEnglishStateStandby()
// .
// -----------------------------------------------------------------------------
// 
CPeninputFingerHwrEnglishStateStandby::CPeninputFingerHwrEnglishStateStandby( 
	CPeninputFingerHwrStateManagerEn& aStateManager )
	:CPeninputFingerHwrStateHandlerBase( aStateManager )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrEnglishStateStandby::NewL()
// .
// -----------------------------------------------------------------------------
// 
CPeninputFingerHwrEnglishStateStandby* CPeninputFingerHwrEnglishStateStandby::NewL( 
	CPeninputFingerHwrStateManagerEn& aStateManager )
    {
    CPeninputFingerHwrEnglishStateStandby* self = new( ELeave ) 
        CPeninputFingerHwrEnglishStateStandby( aStateManager );
    
    return self;                                                                           
    }
    

// -----------------------------------------------------------------------------
// CPeninputFingerHwrEnglishStateStandby::HandleEventL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrEnglishStateStandby::HandleEventL( const TInt aEventType, 
    const TDesC& /*aEventData*/ )
    {
    switch( aEventType )
        {
        case EEventHwrStrokeStarted:
            {
            iStateManager.SetState( CPeninputFingerHwrStateManagerEn::
                EPeninputFingerHwrStateWritingEn );
            }
            break;
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrEnglishStateStandby::OnEntry()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrEnglishStateStandby::OnEntry()
    {
    iDataStore.TriggerStr()->DeleteAll();    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrEnglishStateStandby::OnExit()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrEnglishStateStandby::OnExit()
    {
    }


// ============================ MEMBER FUNCTIONS ===============================
//                     CPeninputFingerHwrEnglishStateWriting class
// -----------------------------------------------------------------------------
// CPeninputFingerHwrEnglishStateWriting::CPeninputFingerHwrEnglishStateWriting()
// .
// -----------------------------------------------------------------------------
// 
CPeninputFingerHwrEnglishStateWriting::CPeninputFingerHwrEnglishStateWriting( 
	CPeninputFingerHwrStateManagerEn& aStateManager )
	:CPeninputFingerHwrStateHandlerBase( aStateManager )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrEnglishStateWriting::NewL()
// .
// -----------------------------------------------------------------------------
// 
CPeninputFingerHwrEnglishStateWriting* CPeninputFingerHwrEnglishStateWriting::NewL( 
	CPeninputFingerHwrStateManagerEn& aStateManager )
    {
    CPeninputFingerHwrEnglishStateWriting* self = new( ELeave ) 
        CPeninputFingerHwrEnglishStateWriting( aStateManager );
    
    return self;                                                                            
    }
    

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateHandlerWriting::HandleEventL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrEnglishStateWriting::HandleEventL( const TInt aEventType, 
    const TDesC& aEventData )
    {
    switch( aEventType )
        {
        case EEventHwrStrokeFinished:
            {
            OnEndStrokeL( aEventData );           
            }
            break;       
        case EEventHwrCharacterTimerOut:
            {
            OnEndWritingL( aEventData );
            }
            break;
        case EHwrEventCandidateSelected:
            {
            OnSelectedCandidatesL(aEventData);
            }    
            break;             
        case EHwrEventPredictiveSelected:
            {
            OnSelectedPredictivesL(aEventData);
            }
            break;    
        case EHwrEventKeyBack:
            {
            OnClickBackSpaceL();
            }
            break;
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrEnglishStateWriting::OnEndStrokeL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrEnglishStateWriting::OnEndStrokeL( const TDesC& aEventData )
	{
	const RArray<TPoint>* aTraceData = ( RArray<TPoint>* )aEventData.Ptr();
    iDataStore.DoEngRecognizeL( *aTraceData );
	}

// -----------------------------------------------------------------------------
// CPeninputFingerHwrEnglishStateWriting::OnEndWriting()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrEnglishStateWriting::OnEndWritingL( const TDesC& aEventData )
	{
	const RArray<TPoint>* aTraceData = ( RArray<TPoint>* )aEventData.Ptr();
    
    // set the default cell highlight flag
    iDataStore.SetHighlight(ETrue);
    
#ifdef RD_INTELLIGENT_TEXT_INPUT
    // the first dropdown list will show writing candidate
    RPointerArray<HBufC> candidates = iDataStore.Candidate();
#else
    // the second dropdown list will show writing canidate
    RPointerArray<HBufC> candidates = iDataStore.Predictive();
#endif //RD_INTELLIGENT_TEXT_INPUT
    
    if ( candidates.Count() > 0 )
        {
        HBufC* defaultCandiate = candidates[0];
        
        if( iDataStore.IsSpecialDisplayChars( *defaultCandiate ) )
            {
            HBufC* realCandidate = iDataStore.ConvertDisplayChars( *defaultCandiate );
            iStateManager.HwrLayout().SubmitStringToFep( *realCandidate );
            delete realCandidate;
            }
        else
            {
            iStateManager.HwrLayout().SubmitStringToFep( *defaultCandiate );
            }
        //check if the default candidate is need to be sent directly
        if( iDataStore.IsDirectlySentCandidate( *defaultCandiate ) )
            {               
            iStateManager.SetState( CPeninputFingerHwrStateManagerEn::
                EPeninputFingerHwrStateStandbyEn );
            }
        else
            {                                
            iStateManager.SetState( CPeninputFingerHwrStateManagerEn::
                EPeninputFingerHwrStateCandidateSelectingEn );  
            }        
        
        }    
	}

// -----------------------------------------------------------------------------
// CPeninputFingerHwrEnglishStateWriting::OnClickBackSpaceL()
// .
// -----------------------------------------------------------------------------
//	
void CPeninputFingerHwrEnglishStateWriting::OnClickBackSpaceL()
    {
#ifdef RD_INTELLIGENT_TEXT_INPUT
    // delete the last char in trigger string
    iDataStore.TriggerStr()->DeleteLast();
    
    // Get auto complete candidate from engine 
    // by the new trigger string
    iDataStore.DoPredictiveL();
    
    // Show fix symbol in the first dropdown list
    iDataStore.SetFixEnglishSymbolL(ERowOne);
    iDataStore.SetHighlight(EFalse);        
    
    iStateManager.SetState( 
            CPeninputFingerHwrStateManagerEn::EPeninputFingerHwrStatePredictSelectingEn );       
#else
    // Show two row fix symbol
    iDataStore.SetFixEnglishSymbolL(ERowTwo);
    iStateManager.HwrLayout().HideTwoRowEngSymbolList(EFalse);
#endif
    }


	
// -----------------------------------------------------------------------------
// CPeninputFingerHwrEnglishStateWriting::OnSelectedCandidatesL()
// .
// -----------------------------------------------------------------------------
//	
void CPeninputFingerHwrEnglishStateWriting::OnSelectedCandidatesL( 
    const TDesC& aEventData )
    {
    TPtrC ptr;
    ptr.Set( ( aEventData.Left( aEventData.Length() - 1 ) ) );   
    
#ifdef RD_INTELLIGENT_TEXT_INPUT
    RPointerArray<HBufC> candidates = iDataStore.Candidate();
    HBufC* defaultCandidate = candidates[0];

    if( *defaultCandidate != ptr )
        {
        if( iDataStore.IsSpecialDisplayChars( ptr ) )
            {
            HBufC* realCandidate = iDataStore.ConvertDisplayChars( ptr );
            iStateManager.HwrLayout().SubmitStringToFep(realCandidate->Des());

            delete realCandidate;
            }
        else
            {
            iStateManager.HwrLayout().SubmitStringToFep(ptr);        
            }        
        
        // Set the trigger string in different condition
        if(!iDataStore.IsEnglishLetter(ptr))
            {
            iStateManager.HwrLayout().SubmitStartPosToFep(ETrue);            
            iDataStore.TriggerStr()->Set(ptr);
            }
        else
            {
            iDataStore.TriggerStr()->DeleteLast();
            iDataStore.TriggerStr()->Append(ptr);
            }
        
        // according to new trigger string to get auto complete candidate.
        iDataStore.DoPredictiveL();
        
        }
    // Set the first dropdown list content
    iDataStore.SetFixEnglishSymbolL();
    
    // Not highlight the default writing candidate.
    iDataStore.SetHighlight(EFalse);
    
    iStateManager.SetState( CPeninputFingerHwrStateManagerEn::
    EPeninputFingerHwrStatePredictSelectingEn );         
        
#else
    if( iDataStore.IsSpecialDisplayChars( ptr ) )
        {
        HBufC* realCandidate = iDataStore.ConvertDisplayChars( ptr );
        iStateManager.HwrLayout().SubmitStringToFep( realCandidate->Des() );
        delete realCandidate;
        }    
    else
        {
        iStateManager.HwrLayout().SubmitStringToFep( ptr );
        }
    
    // Set two dropdown list content and show them
    iDataStore.SetFixEnglishSymbolL(ERowTwo);
    iStateManager.HwrLayout().HideTwoRowEngSymbolList(EFalse);
    
    iStateManager.SetState( 
        CPeninputFingerHwrStateManagerEn::
        EPeninputFingerHwrStateCandidateSelectingEn );
#endif //  RD_INTELLIGENT_TEXT_INPUT
    
    }	

// -----------------------------------------------------------------------------
// CPeninputFingerHwrEnglishStateWriting::OnSelectedPredictivesL()
// .
// -----------------------------------------------------------------------------
//	
void CPeninputFingerHwrEnglishStateWriting::OnSelectedPredictivesL(
    const TDesC& aEventData )
    {    
    TPtrC ptr;
    ptr.Set( ( aEventData.Left( aEventData.Length() - 1 ) ) );
    
#ifdef RD_INTELLIGENT_TEXT_INPUT
    TPtrC string = iDataStore.TriggerStr()->GetString();
    // replace all the trigger string that has been submitted into ICF
    iStateManager.HwrLayout().Replace(string, ptr);
    iStateManager.HwrLayout().SubmitStringToFep(KDisplaySpace);
    
    // Set the first dropdown list content and not hightlight the
    // first cell
    iDataStore.SetFixEnglishSymbolL();
    iDataStore.SetHighlight(EFalse);
    
    iDataStore.ClearPredictive();
    iDataStore.TriggerStr()->DeleteAll();
    
    iStateManager.SetState( CPeninputFingerHwrStateManagerEn::
        EPeninputFingerHwrStatePredictSelectingEn );

#else
    RPointerArray<HBufC> candidates = iDataStore.Predictive();

    //handle special display char
    if( iDataStore.IsSpecialDisplayChars( ptr ) )
        {
        HBufC* realCandidate = iDataStore.ConvertDisplayChars( ptr );
        iStateManager.HwrLayout().SubmitStringToFep( ptr );
        delete realCandidate;
        }
    else
        {
        iStateManager.HwrLayout().SubmitStringToFep( ptr );         
        } 
    
    // Set the two dropdown list content
    iDataStore.SetFixEnglishSymbolL(ERowTwo);
    iStateManager.HwrLayout().HideTwoRowEngSymbolList(EFalse);
    
    iStateManager.SetState( 
        CPeninputFingerHwrStateManagerEn::
        EPeninputFingerHwrStateCandidateSelectingEn );     
#endif
    }



// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateHandlerWriting::OnEntry()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrEnglishStateWriting::OnEntry()
    {
     
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateHandlerWriting::OnExit()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrEnglishStateWriting::OnExit()
    {
    
    }


// ============================ MEMBER FUNCTIONS ===============================
//                     CPeninputFingerHwrEnglishStateCandidateSelecting class
// -----------------------------------------------------------------------------
// CPeninputFingerHwrEnglishStateCandidateSelecting::
//                              CPeninputFingerHwrEnglishStateCandidateSelecting()
// .
// -----------------------------------------------------------------------------
// 
CPeninputFingerHwrEnglishStateCandidateSelecting::
    CPeninputFingerHwrEnglishStateCandidateSelecting( 
	CPeninputFingerHwrStateManagerEn& aStateManager )
	:CPeninputFingerHwrStateHandlerBase( aStateManager )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrEnglishStateCandidateSelecting::NewL()
// .
// -----------------------------------------------------------------------------
// 
CPeninputFingerHwrEnglishStateCandidateSelecting* 
    CPeninputFingerHwrEnglishStateCandidateSelecting::NewL( 
	CPeninputFingerHwrStateManagerEn& aStateManager )
    {
    CPeninputFingerHwrEnglishStateCandidateSelecting* self = new( ELeave ) 
        CPeninputFingerHwrEnglishStateCandidateSelecting( aStateManager );
    
    return self;                                                                                
    }
    

// -----------------------------------------------------------------------------
// CPeninputFingerHwrEnglishStateCandidateSelecting::HandleEventL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrEnglishStateCandidateSelecting::HandleEventL( const TInt aEventType, 
    const TDesC& aEventData )
    {
    switch( aEventType )
        {
        case EHwrEventCandidateSelected:
            {
            OnSelectedCandidatesL( aEventData );
            }
            break;     
        case EHwrEventPredictiveSelected:
            {
            OnSelectedPredictivesL( aEventData );
            }
            break;
        case EHwrEventKeyBack:
            {
            OnClickBackSpaceL();
            }
            break;            
        case EEventHwrStrokeStarted: 
            {
            iStateManager.SetState( CPeninputFingerHwrStateManagerEn::
                EPeninputFingerHwrStateWritingEn );
            }
            break;
        default:
            break;
        }
    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateHandlerCandidateSelecting::OnSelectedCandidatesL()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputFingerHwrEnglishStateCandidateSelecting::OnSelectedCandidatesL( 
    const TDesC& aEventData )
    {
    TPtrC ptr;
    ptr.Set( aEventData.Left( aEventData.Length() - 1 ) );

#ifdef RD_INTELLIGENT_TEXT_INPUT
    RPointerArray<HBufC> candidates = iDataStore.Candidate();
    HBufC* defaultCandidate = candidates[0];

    if( *defaultCandidate != ptr )
        {
        if( iDataStore.IsSpecialDisplayChars( ptr ) )
            {
            HBufC* realCandidate = iDataStore.ConvertDisplayChars( ptr );
            iStateManager.HwrLayout().Replace( *defaultCandidate, 
                *realCandidate, ETrue );
            delete realCandidate;
            }
        else
            {
            iStateManager.HwrLayout().Replace( *defaultCandidate, ptr, ETrue );         
            }        
        
        // update trigger string in different condition
        if(!iDataStore.IsEnglishLetter(ptr))
            {
            iStateManager.HwrLayout().SubmitStartPosToFep(ETrue);            
            iDataStore.TriggerStr()->Set(ptr);
            }
        else
            {
            iDataStore.TriggerStr()->DeleteLast();
            iDataStore.TriggerStr()->Append(ptr);
            }
        
        iDataStore.DoPredictiveL();
        
        }
    // Set the first dropdown list content and not highlight 
    // the first cell
    iDataStore.SetFixEnglishSymbolL();
    iDataStore.SetHighlight(EFalse);
    
    iStateManager.SetState( CPeninputFingerHwrStateManagerEn::
    EPeninputFingerHwrStatePredictSelectingEn );         
        

#else
    if( iDataStore.IsSpecialDisplayChars( ptr ) )
        {
        HBufC* realCandidate = iDataStore.ConvertDisplayChars( ptr );
        iStateManager.HwrLayout().SubmitStringToFep( realCandidate->Des() );
        delete realCandidate;
        }
    else
        {
        iStateManager.HwrLayout().SubmitStringToFep( ptr );
        }
    
    iDataStore.SetFixEnglishSymbolL(ERowTwo);
    iStateManager.HwrLayout().HideTwoRowEngSymbolList(EFalse);
    
#endif     
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrEnglishStateCandidateSelecting::OnSelectedPredictivesL()
// .
// -----------------------------------------------------------------------------
// 
void CPeninputFingerHwrEnglishStateCandidateSelecting::OnSelectedPredictivesL( 
    const TDesC& aEventData)
    {                                                        
    TPtrC ptr;
    ptr.Set(( aEventData.Left( aEventData.Length() - 1 )));
    
#ifdef RD_INTELLIGENT_TEXT_INPUT
    // for auto complete, replace all the trigger string    
    TPtrC string = iDataStore.TriggerStr()->GetString();
    iStateManager.HwrLayout().Replace(string, ptr);
    iStateManager.HwrLayout().SubmitStringToFep(KDisplaySpace);
    iDataStore.SetFixEnglishSymbolL();
    iDataStore.SetHighlight(EFalse);
    
    iDataStore.ClearPredictive();
    iDataStore.TriggerStr()->DeleteAll();
    
    iStateManager.SetState( CPeninputFingerHwrStateManagerEn::
        EPeninputFingerHwrStatePredictSelectingEn );    
#else
    // for normal English recognition, the second dropdownlist show candidates
    RPointerArray<HBufC> candidates = iDataStore.Candidate();
    HBufC* defaultCandidate = candidates[0];
    
    if( *defaultCandidate != ptr )
        {
        //handle special display char
        if( iDataStore.IsSpecialDisplayChars( ptr ) )
            {
            HBufC* realCandidate = iDataStore.ConvertDisplayChars( ptr );
            iStateManager.HwrLayout().Replace( *defaultCandidate, 
                *realCandidate );
            delete realCandidate;
            }
        else
            {
            iStateManager.HwrLayout().Replace( *defaultCandidate, ptr );         
            }
        }
             
     iDataStore.SetFixEnglishSymbolL(ERowTwo);
     iStateManager.HwrLayout().HideTwoRowEngSymbolList(EFalse);         
#endif // RD_INTELLIGENT_TEXT_INPUT

    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrEnglishStateCandidateSelecting::OnClickBackSpaceL()
// .
// -----------------------------------------------------------------------------
// 
void CPeninputFingerHwrEnglishStateCandidateSelecting::OnClickBackSpaceL()
    {
#ifdef RD_INTELLIGENT_TEXT_INPUT
    // delete the last char in trigger string
    iDataStore.TriggerStr()->DeleteLast();
    
    // Get auto complete candidate from engine 
    // by the new trigger string
    iDataStore.DoPredictiveL();
    
    // Show fix symbol in the first dropdown list
    iDataStore.SetFixEnglishSymbolL(ERowOne);
    iDataStore.SetHighlight(EFalse);        
    
    iStateManager.SetState( 
            CPeninputFingerHwrStateManagerEn::EPeninputFingerHwrStatePredictSelectingEn );       
#else
    // Show two row fix symbol
    iDataStore.SetFixEnglishSymbolL(ERowTwo);
    iStateManager.HwrLayout().HideTwoRowEngSymbolList(EFalse);
#endif
    }
  
      
// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateHandlerCandidateSelecting::OnEntry()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrEnglishStateCandidateSelecting::OnEntry()
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateHandlerCandidateSelecting::OnExit()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrEnglishStateCandidateSelecting::OnExit()
    {
    
    }

#ifdef RD_INTELLIGENT_TEXT_INPUT

// ============================ MEMBER FUNCTIONS ===============================
//                     CPeninputFingerHwrEnglishStatePredictSelecting class
// -----------------------------------------------------------------------------
// CPeninputFingerHwrEnglishStatePredictSelecting::
//                                CPeninputFingerHwrEnglishStatePredictSelecting()
// .
// -----------------------------------------------------------------------------
// 
CPeninputFingerHwrEnglishStatePredictSelecting::
    CPeninputFingerHwrEnglishStatePredictSelecting( 
	CPeninputFingerHwrStateManagerEn& aStateManager )
	:CPeninputFingerHwrStateHandlerBase( aStateManager )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrEnglishStatePredictSelecting::NewL()
// .
// -----------------------------------------------------------------------------
// 
CPeninputFingerHwrEnglishStatePredictSelecting*  
    CPeninputFingerHwrEnglishStatePredictSelecting::NewL( 
	CPeninputFingerHwrStateManagerEn& aStateManager )
    {
    CPeninputFingerHwrEnglishStatePredictSelecting* self = new( ELeave ) 
        CPeninputFingerHwrEnglishStatePredictSelecting( aStateManager );
    
    return self;                                                                      
    }
    

// -----------------------------------------------------------------------------
// CPeninputFingerHwrEnglishStatePredictSelecting::HandleEventL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrEnglishStatePredictSelecting::HandleEventL( const TInt aEventType, 
    const TDesC& aEventData )
    {
    switch( aEventType )
        {
        case EHwrEventCandidateSelected:
            {
            OnSelectedCandidatesL(aEventData);
            }
            break;
        case EHwrEventPredictiveSelected:
            {
            OnSelectedPredictivesL( aEventData );
            }
            break;
        case EHwrEventKeyBack:
            {            
            OnClickBackSpaceL();   
            }
            break;   
        case EEventHwrStrokeStarted:  
            {
            iStateManager.SetState( 
                CPeninputFingerHwrStateManagerEn::EPeninputFingerHwrStateWritingEn );
            }
            break;
        default:
            break;
        }    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrEnglishStatePredictSelecting::OnSelectedCandidatesL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrEnglishStatePredictSelecting::OnSelectedCandidatesL( 
    const TDesC& aEventData )
    {
    TPtrC ptr;
    ptr.Set(( aEventData.Left( aEventData.Length() - 1 )));      
    iStateManager.HwrLayout().SubmitStringToFep( ptr );
    iStateManager.HwrLayout().SubmitStartPosToFep(EFalse);
    
    
    // in fact, just get an empty auto complete candidate
    // because the prt is punc.
    iDataStore.TriggerStr()->Set(ptr);
    iDataStore.DoPredictiveL();        
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrEnglishStatePredictSelecting::OnSelectedPredictivesL()
// .
// -----------------------------------------------------------------------------
// 
void CPeninputFingerHwrEnglishStatePredictSelecting::OnSelectedPredictivesL( 
    const TDesC& aEventData )
    {
    TPtrC ptr;
    ptr.Set(( aEventData.Left( aEventData.Length() - 1 )));

    TPtrC string = iDataStore.TriggerStr()->GetString();
    // replace all the trigger string that have been submitted into ICF.
    iStateManager.HwrLayout().Replace(string, ptr);
    iStateManager.HwrLayout().SubmitStringToFep(KDisplaySpace);
    iDataStore.ClearPredictive();
    iDataStore.TriggerStr()->DeleteAll();   
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrEnglishStatePredictSelecting::OnClickBackSpaceL()
// .
// -----------------------------------------------------------------------------
// 
void CPeninputFingerHwrEnglishStatePredictSelecting::OnClickBackSpaceL()
    {
    // delete the last char in trigger string and get auto complete candidate
    iDataStore.TriggerStr()->DeleteLast();
    iDataStore.DoPredictiveL();
    
    if( iDataStore.PredictiveCount() <= 0 )
        {
        iDataStore.TriggerStr()->DeleteAll();     
        }    
    }    
    
// -----------------------------------------------------------------------------
// CPeninputFingerHwrEnglishStatePredictSelecting::OnEntry()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrEnglishStatePredictSelecting::OnEntry()
    {

    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrEnglishStatePredictSelecting::OnExit()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrEnglishStatePredictSelecting::OnExit()
    {
    
    }

#endif // RD_INTELLIGENT_TEXT_INPUT


// ============================ MEMBER FUNCTIONS ===============================
//                     CPeninputFingerHwrEnglishStateStandby class
// -----------------------------------------------------------------------------
// CPeninputFingerHwrNumericStateStandby::CPeninputFingerHwrNumericStateStandby()
// .
// -----------------------------------------------------------------------------
// 
CPeninputFingerHwrNumericStateStandby::CPeninputFingerHwrNumericStateStandby( 
	CPeninputFingerHwrStateManagerNum& aStateManager )
	:CPeninputFingerHwrStateHandlerBase( aStateManager )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrNumericStateStandby::NewL()
// .
// -----------------------------------------------------------------------------
// 
CPeninputFingerHwrNumericStateStandby* CPeninputFingerHwrNumericStateStandby::NewL( 
	CPeninputFingerHwrStateManagerNum& aStateManager )
    {
    CPeninputFingerHwrNumericStateStandby* self = new( ELeave ) 
        CPeninputFingerHwrNumericStateStandby( aStateManager );
    
    return self;
                                                                                
    }
    

// -----------------------------------------------------------------------------
// CPeninputFingerHwrNumericStateStandby::HandleEventL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrNumericStateStandby::HandleEventL( const TInt /*aEventType*/, 
    const TDesC& /*aEventData*/ )
    {
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrNumericStateStandby::OnEntry()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrNumericStateStandby::OnEntry()
    {
    iDataStore.TriggerStr()->DeleteAll();
    
#ifdef RD_INTELLIGENT_TEXT_INPUT 
    iStateManager.HwrLayout().SubmitStartPosToFep(EFalse);
#endif // RD_INTELLIGENT_TEXT_INPUT    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrNumericStateStandby::OnExit()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrNumericStateStandby::OnExit()
    {
    
    }

//End of file
