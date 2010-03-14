/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Implementation for arabic peninput finger hwr
*
*/

#include <peninputcmd.h>

#include "peninputfingerhwrarlayout.h"
#include "peninputfingerhwrarwnd.h"
#include "peninputfingerhwrarstatemanager.h"
#include "peninputfingerhwrardatastore.h"
#include "peninputfingerhwrarevent.h"


// ============================ MEMBER FUNCTIONS ===============================
//                     CPeninputFingerHwrArStateHandlerBase class
// -----------------------------------------------------------------------------
// CPeninputFingerHwrArStateHandlerBase::CPeninputFingerHwrArStateHandlerBase()
// .
// -----------------------------------------------------------------------------
//
CPeninputFingerHwrArStateHandlerBase::CPeninputFingerHwrArStateHandlerBase( 
CPeninputFingerHwrArStateManagerBase& aStateManager )
    :iStateManager( aStateManager ),
    iDataStore( aStateManager.HwrLayout().DataStore() )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrArStateHandlerBase::~CPeninputFingerHwrArStateHandlerBase()
// .
// -----------------------------------------------------------------------------
//
CPeninputFingerHwrArStateHandlerBase::~CPeninputFingerHwrArStateHandlerBase()
    {
    
    }


// ============================ MEMBER FUNCTIONS ===============================
//                     CPeninputFingerHwrStateManager class
// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManager::CPeninputFingerHwrStateManager()
// .
// -----------------------------------------------------------------------------
//
CPeninputFingerHwrArStateManagerBase::CPeninputFingerHwrArStateManagerBase( 
    CPeninputFingerHwrArLayout* aLayout )
    :iLayout(aLayout)
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManager::~CPeninputFingerHwrStateManager()
// .
// -----------------------------------------------------------------------------
//
CPeninputFingerHwrArStateManagerBase::~CPeninputFingerHwrArStateManagerBase()
    {
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateManager::CosntructL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrArStateManagerBase::BaseConstructL()
    {

    }
    
// -----------------------------------------------------------------------------
// CPeninputFingerHwrArStateManagerBase::SetState()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputFingerHwrArStateManagerBase::SetState( TInt /*aState*/ )
    {

    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrArStateManagerBase::IsStandbyState()
// .
// -----------------------------------------------------------------------------
//
TBool CPeninputFingerHwrArStateManagerBase::IsStandbyState()
    {
    if(iCurrentState == 0)
        {
        return ETrue;
        }

    return EFalse;
    }


// -----------------------------------------------------------------------------
// CPeninputFingerHwrArStateManagerBase::HandleEventL()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputFingerHwrArStateManagerBase::HandleEventL( const TInt /*aEventType*/,
    const TDesC& /*aEventData*/)
    {

    }

CPeninputFingerHwrArLayout& CPeninputFingerHwrArStateManagerBase::HwrLayout()
    {
    return *iLayout;
    }

 
// ============================ MEMBER FUNCTIONS ===============================
//                     CPeninputFingerHwrStateManager class
// -----------------------------------------------------------------------------
// CPeninputFingerHwrArStateManager::NewL()
// .
// -----------------------------------------------------------------------------
//
CPeninputFingerHwrArStateManager* CPeninputFingerHwrArStateManager::NewL( 
    CPeninputFingerHwrArLayout* aLayout )
    {
    CPeninputFingerHwrArStateManager* self = new( ELeave ) 
        CPeninputFingerHwrArStateManager( aLayout );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrArStateManager::CPeninputFingerHwrStateManager()
// .
// -----------------------------------------------------------------------------
//
CPeninputFingerHwrArStateManager::CPeninputFingerHwrArStateManager( 
    CPeninputFingerHwrArLayout* aLayout )
    :CPeninputFingerHwrArStateManagerBase( aLayout )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrArStateManager::~CPeninputFingerHwrStateManager()
// .
// -----------------------------------------------------------------------------
//
CPeninputFingerHwrArStateManager::~CPeninputFingerHwrArStateManager()
    {
	for( int i = 0; i < EPeninputFingerHwrArStateCount; ++i )
		{
		delete iStateHandlers[i];
		}
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrArStateManager::CosntructL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrArStateManager::ConstructL()
    {
    BaseConstructL();
    
    TInt i = 0;
    
    iStates[i] = EPeninputFingerHwrArStateStandby;
    iStateHandlers[i] = CPeninputFingerHwrArStateStandby::NewL( *this );

	iStates[++i] = EPeninputFingerHwrArStateWriting;
    iStateHandlers[i] = CPeninputFingerHwrArStateWriting::NewL( *this );
    
	iStates[++i] = EPeninputFingerHwrArStateCandidateSelecting;
    iStateHandlers[i] = 
        CPeninputFingerHwrArStateCandidateSelecting::NewL( *this );
	
//	iStates[++i] = EPeninputFingerHwrArStatePredictSelecting;
//    iStateHandlers[i] = 
//        CPeninputFingerHwrArStatePredictSelecting::NewL( *this );              
//  
    iCurrentStateHandler = iStateHandlers[0];
    iCurrentState = EPeninputFingerHwrArStateStandby;
    }
    
// -----------------------------------------------------------------------------
// CPeninputFingerHwrArStateManager::SetState()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputFingerHwrArStateManager::SetState( TInt aState )
    {
    if( iCurrentState == aState )        
        {
        return;    
        }
        
    CPeninputFingerHwrArStateHandlerBase* newStateHandler = Find( (TPeninputFingerHwrArState)aState );
    if( newStateHandler )
        {
        iCurrentState = (TPeninputFingerHwrArState)aState;
        iCurrentStateHandler->OnExit();
        iCurrentStateHandler = newStateHandler;
        iCurrentStateHandler->OnEntry();
        }
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrArStateManager::Find()
// .
// -----------------------------------------------------------------------------
//  
CPeninputFingerHwrArStateHandlerBase* CPeninputFingerHwrArStateManager::Find( 
    TPeninputFingerHwrArState aState )
    {
    for( TInt i = 0; i < EPeninputFingerHwrArStateCount; i++ )
        {
        if( iStates[i] == aState )
            {
            return iStateHandlers[i];
            }
        }
    return NULL;
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrArStateManager::HandleEventL()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputFingerHwrArStateManager::HandleEventL( const TInt aEventType, 
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
CPeninputFingerHwrArStateStandby::CPeninputFingerHwrArStateStandby( 
	CPeninputFingerHwrArStateManager& aStateManager )
	:CPeninputFingerHwrArStateHandlerBase( aStateManager )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateHandlerStandby::NewL()
// .
// -----------------------------------------------------------------------------
// 
CPeninputFingerHwrArStateStandby* CPeninputFingerHwrArStateStandby::NewL( 
	CPeninputFingerHwrArStateManager& aStateManager )
    {
    CPeninputFingerHwrArStateStandby* self = new( ELeave ) 
        CPeninputFingerHwrArStateStandby( aStateManager );
    
    return self;
                                                                                
    }
    

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateHandlerStandby::HandleEventL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrArStateStandby::HandleEventL( const TInt aEventType, 
    const TDesC& /*aEventData*/ )
    {
    switch( aEventType )
        {
        case EEventHwrStrokeStarted:
            {
            iStateManager.SetState( 
                CPeninputFingerHwrArStateManager::EPeninputFingerHwrArStateWriting );
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
void CPeninputFingerHwrArStateStandby::OnEntry()
    {
//    iDataStore.TriggerStr()->DeleteAll();
    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateHandlerStandby::OnExit()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrArStateStandby::OnExit()
    {

    }

// ============================ MEMBER FUNCTIONS ===============================
//                     CPeninputFingerHwrStateHandlerWriting class
// -----------------------------------------------------------------------------
// CPeninputFingerHwrArStateWriting::CPeninputFingerHwrStateHandlerWriting()
// .
// -----------------------------------------------------------------------------
// 
CPeninputFingerHwrArStateWriting::CPeninputFingerHwrArStateWriting( 
	CPeninputFingerHwrArStateManager& aStateManager )
	:CPeninputFingerHwrArStateHandlerBase( aStateManager )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrArStateWriting::NewL()
// .
// -----------------------------------------------------------------------------
// 
CPeninputFingerHwrArStateWriting* CPeninputFingerHwrArStateWriting::NewL( 
	CPeninputFingerHwrArStateManager& aStateManager )
    {
    CPeninputFingerHwrArStateWriting* self = new( ELeave ) 
        CPeninputFingerHwrArStateWriting( aStateManager );
    
    return self;
                                                                                
    }
    

// -----------------------------------------------------------------------------
// CPeninputFingerHwrArStateWriting::HandleEventL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrArStateWriting::HandleEventL( const TInt aEventType, 
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
//            OnSelectedPredictivesL(aEventData);
            }
            break;                     
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrArStateWriting::OnEndStrokeL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrArStateWriting::OnEndStrokeL( const TDesC& aEventData )
	{
	const RArray<TPoint>* aTraceData = ( RArray<TPoint>* )aEventData.Ptr();
    iDataStore.DoRecognizeL( *aTraceData );
	}


// -----------------------------------------------------------------------------
// CPeninputFingerHwrArStateWriting::OnEndWriting()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrArStateWriting::OnEndWritingL( const TDesC& /*aEventData*/ )
	{
    // record the default cell highlight flag.
    iDataStore.SetHighlight(ETrue);
    // This is a workarround to clear the recognizer.
    RArray<TPoint> stroke_array;
    stroke_array.Reset(); 
    // Call recognize with a empty array of stroke is the condition
    // to clear the context>
    iDataStore.DoRecognizeL(stroke_array );      
    
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
            iStateManager.SetState( CPeninputFingerHwrArStateManager::
                EPeninputFingerHwrArStateStandby );
            }
        else
            {
            
            iStateManager.SetState( CPeninputFingerHwrArStateManager::
                EPeninputFingerHwrArStateCandidateSelecting );  
            }        
        
        } 
	}

// -----------------------------------------------------------------------------
// CPeninputFingerHwrArStateWriting::OnSelectedCandidatesL()
// .
// -----------------------------------------------------------------------------
//	
void CPeninputFingerHwrArStateWriting::OnSelectedCandidatesL( 
    const TDesC& aEventData )
    {
    const TInt cellNo = aEventData[aEventData.Length() - 1];
    if (!iDataStore.IsValidCandidate( cellNo ))
        {
        iStateManager.SetState(CPeninputFingerHwrArStateManager::
                        EPeninputFingerHwrArStateStandby );
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
        
    iDataStore.SetArabicSymbolL();
    
    // record the default cell highlight flag
    iDataStore.SetHighlight(EFalse);
    
//    iStateManager.SetState( 
//        CPeninputFingerHwrArStateManager::
//        EPeninputFingerHwrArStateStandby );     
    }
    

	

// -----------------------------------------------------------------------------
// CPeninputFingerHwrArStateWriting::OnEntry()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrArStateWriting::OnEntry()
    {
     
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrArStateWriting::OnExit()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrArStateWriting::OnExit()
    {
    
    }

// ============================ MEMBER FUNCTIONS ===============================
//                     CPeninputFingerHwrStateHandlerCandidateSelecting class
// -----------------------------------------------------------------------------
// CPeninputFingerHwrArStateCandidateSelecting::
//                              CPeninputFingerHwrArStateCandidateSelecting()
// .
// -----------------------------------------------------------------------------
// 
CPeninputFingerHwrArStateCandidateSelecting::
    CPeninputFingerHwrArStateCandidateSelecting( 
	CPeninputFingerHwrArStateManager& aStateManager )
	:CPeninputFingerHwrArStateHandlerBase( aStateManager )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrArStateCandidateSelecting::NewL()
// .
// -----------------------------------------------------------------------------
// 
CPeninputFingerHwrArStateCandidateSelecting* 
    CPeninputFingerHwrArStateCandidateSelecting::NewL( 
	CPeninputFingerHwrArStateManager& aStateManager )
    {
    CPeninputFingerHwrArStateCandidateSelecting* self = new( ELeave ) 
        CPeninputFingerHwrArStateCandidateSelecting( aStateManager );
    
    return self;                                                                                
    }


// -----------------------------------------------------------------------------
// CPeninputFingerHwrArStateCandidateSelecting::HandleEventL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrArStateCandidateSelecting::HandleEventL( const TInt aEventType, 
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
//            OnSelectedPredictivesL( aEventData );
            }
            break;
        case EHwrEventKeyBack:
            {
            OnClickBackSpaceL();
            }
            break;
        case EEventHwrStrokeStarted:
            {
            iStateManager.SetState( CPeninputFingerHwrArStateManager::
                EPeninputFingerHwrArStateWriting );
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
void CPeninputFingerHwrArStateCandidateSelecting::OnSelectedCandidatesL( 
    const TDesC& aEventData )
    {
    //get the candidate index
    const TInt cellNo = aEventData[aEventData.Length()-1];
    if (!iDataStore.IsValidCandidate( cellNo ))
        {
        iStateManager.SetState( CPeninputFingerHwrArStateManager::
                        EPeninputFingerHwrArStateStandby );
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
    iDataStore.SetArabicSymbolL();
//    iStateManager.SetState( CPeninputFingerHwrArStateManager::
//            EPeninputFingerHwrArStateStandby );
    
    }


// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateHandlerCandidateSelecting::OnClickBackSpaceL()
// .
// -----------------------------------------------------------------------------
// 
void CPeninputFingerHwrArStateCandidateSelecting::OnClickBackSpaceL()
    {
    iDataStore.SetHighlight(EFalse);                                                        
    iDataStore.SetArabicSymbolL();
    iStateManager.SetState( CPeninputFingerHwrArStateManager::
            EPeninputFingerHwrArStateStandby );
    }
    
    
        
// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateHandlerCandidateSelecting::OnEntry()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrArStateCandidateSelecting::OnEntry()
    {
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrStateHandlerCandidateSelecting::OnExit()
// .
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrArStateCandidateSelecting::OnExit()
    {
    
    }
//End of file
