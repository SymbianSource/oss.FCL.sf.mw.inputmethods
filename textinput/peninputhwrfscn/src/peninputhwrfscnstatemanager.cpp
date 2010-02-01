/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for chinese peninput full screen hwr
*
*/


#include "peninputhwrfscnlayout.h"
#include "peninputhwrfscnstatemanager.h"
#include "peninputhwrfscndatastore.h"
#include "peninputhwrfscnevent.h"
#include "peninputhwrtriggerstr.h"
#include "peninputhwrfscncandidatepanel.h"

// ============================ MEMBER FUNCTIONS ===============================
//                     CPeninputHwrfscnStateManager class
// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateManager::NewL()
// .
// -----------------------------------------------------------------------------
//
CPeninputHwrfscnStateManager* CPeninputHwrfscnStateManager::NewL( 
    CPeninputHwrfscnLayout* aLayout )
    {
    CPeninputHwrfscnStateManager* self = new( ELeave ) 
        CPeninputHwrfscnStateManager( aLayout );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateManager::CPeninputHwrfscnStateManager()
// .
// -----------------------------------------------------------------------------
//
CPeninputHwrfscnStateManager::CPeninputHwrfscnStateManager( 
    CPeninputHwrfscnLayout* aLayout )
    :iLayout( aLayout )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateManager::~CPeninputHwrfscnStateManager()
// .
// -----------------------------------------------------------------------------
//
CPeninputHwrfscnStateManager::~CPeninputHwrfscnStateManager()
    {
	for( int i = 0; i < EPeninputHwrfscnStateCount; ++i )
		{
		delete iStateHandlers[i];
		}
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateManager::CosntructL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnStateManager::ConstructL()
    {
    TInt i = 0;
    
    iStates[i] = EPeninputHwrfscnStateStandby;
    iStateHandlers[i] = CPeninputHwrfscnStateHandlerStandby::NewL( *this );

	iStates[++i] = EPeninputHwrfscnStateWriting;
    iStateHandlers[i] = CPeninputHwrfscnStateHandlerWriting::NewL( *this );
    
	iStates[++i] = EPeninputHwrfscnStateCandidateSelecting;
    iStateHandlers[i] = 
        CPeninputHwrfscnStateHandlerCandidateSelecting::NewL( *this );
	
	iStates[++i] = EPeninputHwrfscnStateHomophonicSelecting;
	iStateHandlers[i] = 
	    CPeninputHwrfscnStateHandlerHomophonicSelecting::NewL( *this );
	
	iStates[++i] = EPeninputHwrfscnStatePredictSelecting;
    iStateHandlers[i] = 
        CPeninputHwrfscnStateHandlerPredictSelecting::NewL( *this );
  
    iCurrentStateHandler = iStateHandlers[0];
    iCurrentState = EPeninputHwrfscnStateStandby;
    }
    
// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateManager::SetState()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputHwrfscnStateManager::SetState( TPeninputHwrfscnState aState )
    {
    if( iCurrentState == aState )        
        {
        return;    
        }
        
    CPeninputHwrfscnStateHandlerBase* newStateHandler = Find( aState );
    if( newStateHandler )
        {
        iCurrentState = aState;
        iCurrentStateHandler->OnExit();
        iCurrentStateHandler = newStateHandler;
        iCurrentStateHandler->OnEntry();
        }
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateManager::Find()
// .
// -----------------------------------------------------------------------------
//  
CPeninputHwrfscnStateHandlerBase* CPeninputHwrfscnStateManager::Find( 
    TPeninputHwrfscnState aState )
    {
    for( TInt i = 0; i < EPeninputHwrfscnStateCount; i++ )
        {
        if( iStates[i] == aState )
            {
            return iStateHandlers[i];
            }
        }
    return NULL;
    }
        
// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateManager::HandleCommand()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnStateManager::HandleCommand( 
    TInt /*aCmd*/, TUint8* /*aData*/ )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateManager::HandleEventL()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputHwrfscnStateManager::HandleEventL( TInt aEventType, 
    const TDesC& aEventData )
    {
    iCurrentStateHandler->HandleEventL( aEventType, aEventData );
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateManager::HwrLayout()
// .
// -----------------------------------------------------------------------------
//  
CPeninputHwrfscnLayout& CPeninputHwrfscnStateManager::HwrLayout()
    {
    return *iLayout;
    }  
       
// ============================ MEMBER FUNCTIONS ===============================
//                     CPeninputHwrfscnStateHandlerBase class
// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerBase::CPeninputHwrfscnStateHandlerBase()
// .
// -----------------------------------------------------------------------------
//
CPeninputHwrfscnStateHandlerBase::CPeninputHwrfscnStateHandlerBase( 
    CPeninputHwrfscnStateManager& aStateManager )
    :iStateManager( aStateManager ),
    iDataStore( aStateManager.HwrLayout().DataStore() )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerBase::~CPeninputHwrfscnStateHandlerBase()
// .
// -----------------------------------------------------------------------------
//
CPeninputHwrfscnStateHandlerBase::~CPeninputHwrfscnStateHandlerBase()
    {
    
    }  

// ============================ MEMBER FUNCTIONS ===============================
//                     CPeninputHwrfscnStateHandlerStandby class
// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerStandby::CPeninputHwrfscnStateHandlerStandby()
// .
// -----------------------------------------------------------------------------
// 
CPeninputHwrfscnStateHandlerStandby::CPeninputHwrfscnStateHandlerStandby( 
	CPeninputHwrfscnStateManager& aStateManager )
	:CPeninputHwrfscnStateHandlerBase( aStateManager )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerStandby::NewL()
// .
// -----------------------------------------------------------------------------
// 
CPeninputHwrfscnStateHandlerStandby* CPeninputHwrfscnStateHandlerStandby::NewL( 
	CPeninputHwrfscnStateManager& aStateManager )
    {
    CPeninputHwrfscnStateHandlerStandby* self = new( ELeave ) 
        CPeninputHwrfscnStateHandlerStandby( aStateManager );
    
    return self;
                                                                                
    }
    
// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerStandby::HandleCommand()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnStateHandlerStandby::
    HandleCommand( TInt /*aCmd*/, TUint8* /*aData*/ )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerStandby::HandleEventL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnStateHandlerStandby::HandleEventL( TInt aEventType, 
    const TDesC& /*aEventData*/ )
    {
    switch( aEventType )
        {
        case EHwrEventBeginWriting:
            {
            iStateManager.SetState( 
                CPeninputHwrfscnStateManager::EPeninputHwrfscnStateWriting );
            }
            break;
        
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerStandby::OnEntry()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnStateHandlerStandby::OnEntry()
    {
	iStateManager.HwrLayout().CandidatePanel().Show( EFalse );
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerStandby::OnExit()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnStateHandlerStandby::OnExit()
    {
    
    }

// ============================ MEMBER FUNCTIONS ===============================
//                     CPeninputHwrfscnStateHandlerWriting class
// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerWriting::CPeninputHwrfscnStateHandlerWriting()
// .
// -----------------------------------------------------------------------------
// 
CPeninputHwrfscnStateHandlerWriting::CPeninputHwrfscnStateHandlerWriting( 
	CPeninputHwrfscnStateManager& aStateManager )
	:CPeninputHwrfscnStateHandlerBase( aStateManager )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerWriting::NewL()
// .
// -----------------------------------------------------------------------------
// 
CPeninputHwrfscnStateHandlerWriting* CPeninputHwrfscnStateHandlerWriting::NewL( 
	CPeninputHwrfscnStateManager& aStateManager )
    {
    CPeninputHwrfscnStateHandlerWriting* self = new( ELeave ) 
        CPeninputHwrfscnStateHandlerWriting( aStateManager );
    
    return self;
                                                                                
    }
    
// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerWriting::HandleCommand()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnStateHandlerWriting::HandleCommand( 
    TInt /*aCmd*/, TUint8* /*aData*/ )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerWriting::HandleEventL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnStateHandlerWriting::HandleEventL( TInt aEventType, 
    const TDesC& aEventData )
    {
    switch( aEventType )
        {
        case EHwrEventEndWriting:
            {
            OnEndWritingL( aEventData );
            }
            break;
        
        case EHwrEventPointerOutsideWnd:
            {
            iStateManager.SetState( 
                CPeninputHwrfscnStateManager::EPeninputHwrfscnStateStandby );
            }
            break;
                
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerWriting::OnEndWriting()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnStateHandlerWriting::OnEndWritingL( const TDesC& aEventData )
	{
	const RArray<TPoint>* aTraceData = ( RArray<TPoint>* )aEventData.Ptr();
    iDataStore.DoRecognizeL( *aTraceData );
    RPointerArray<HBufC> candidates = iDataStore.Candidate();
    if ( candidates.Count() > 0 )
        {
        HBufC* defaultCandiate = candidates[0];
        if( iDataStore.IsSpecialDisplayChars( *defaultCandiate ) )
            {
            //allocte a new buf pointed by str
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
            iStateManager.SetState( 
                CPeninputHwrfscnStateManager::EPeninputHwrfscnStateStandby );
            }
        else
            {                                
            iStateManager.SetState( 
                CPeninputHwrfscnStateManager::
                EPeninputHwrfscnStateCandidateSelecting );  
            }
        }            			
	}

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerWriting::OnEntry()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnStateHandlerWriting::OnEntry()
    {
     
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerWriting::OnExit()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnStateHandlerWriting::OnExit()
    {
    
    }

// ============================ MEMBER FUNCTIONS ===============================
//                     CPeninputHwrfscnStateHandlerCandidateSelecting class
// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerCandidateSelecting::
//                              CPeninputHwrfscnStateHandlerCandidateSelecting()
// .
// -----------------------------------------------------------------------------
// 
CPeninputHwrfscnStateHandlerCandidateSelecting::
    CPeninputHwrfscnStateHandlerCandidateSelecting( 
	CPeninputHwrfscnStateManager& aStateManager )
	:CPeninputHwrfscnStateHandlerBase( aStateManager )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerCandidateSelecting::NewL()
// .
// -----------------------------------------------------------------------------
// 
CPeninputHwrfscnStateHandlerCandidateSelecting* 
    CPeninputHwrfscnStateHandlerCandidateSelecting::NewL( 
	CPeninputHwrfscnStateManager& aStateManager )
    {
    CPeninputHwrfscnStateHandlerCandidateSelecting* self = new( ELeave ) 
        CPeninputHwrfscnStateHandlerCandidateSelecting( aStateManager );
    
    return self;                                                                                
    }
    
// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerCandidateSelecting::HandleCommand()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnStateHandlerCandidateSelecting::HandleCommand( TInt /*aCmd*/, 
    TUint8* /*aData*/ )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerCandidateSelecting::HandleEventL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnStateHandlerCandidateSelecting::HandleEventL( TInt aEventType, 
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
            
        case EHwrEventPointerOutsideWnd:
        case EHwrEventBeginWriting: 
            {
            iStateManager.SetState( CPeninputHwrfscnStateManager::
                EPeninputHwrfscnStateStandby );
            }
            break;
            
        default:
            break;
        }
    
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerCandidateSelecting::OnSelectedCandidatesL()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputHwrfscnStateHandlerCandidateSelecting::OnSelectedCandidatesL( 
    const TDesC& aEventData )
    {
    //get the candidate index
    const TInt cellNo = aEventData[aEventData.Length()-1];
    if (!iDataStore.IsValid( cellNo ))
        {
        iStateManager.SetState( 
                        CPeninputHwrfscnStateManager::
                        EPeninputHwrfscnStateCandidateSelecting );
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
    
    //check if it is homophonic char when the chinese is primary range
    if( iDataStore.IsChinesePrimaryRange() 
       && iDataStore.IsChinese( cellNo ) 
       && iDataStore.IsHomephonicLanguage() )
        {               
        //get the homophonic candidates    
        iDataStore.DoHomophonicL( ptr );
        if( iDataStore.Candidate().Count() > 1 )
            {
            iStateManager.SetState( 
                CPeninputHwrfscnStateManager::
                EPeninputHwrfscnStateHomophonicSelecting );        
            }
        else//enter the predictive state
            {
            OnSelectedPredictivesL( aEventData, EFalse );
            }
        }
    else
        {
        OnSelectedPredictivesL( aEventData, EFalse );
        }
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerCandidateSelecting::OnSelectedCandidatesL()
// .
// -----------------------------------------------------------------------------
// 
void CPeninputHwrfscnStateHandlerCandidateSelecting::OnSelectedPredictivesL( 
    const TDesC& aEventData,
    TBool aAppend )
    {                                                        
    TPtrC ptr;
    ptr.Set( aEventData.Left( aEventData.Length() - 1 ) );
    // aAppend means we need append the prt to current trigger string and submit 
    // ptr to forground app    
    if( aAppend )
        {
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
        }
    else
        {
        iDataStore.TriggerStr()->Set( ptr );
        }
        
    iDataStore.DoPredictiveL();
    
    if( iDataStore.PredictiveCount() > 0 )
        {
        iStateManager.SetState( 
            CPeninputHwrfscnStateManager::
            EPeninputHwrfscnStatePredictSelecting );     
        }
    else
        {
        iStateManager.SetState( 
            CPeninputHwrfscnStateManager::EPeninputHwrfscnStateStandby );
        }
    
    }
        
// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerCandidateSelecting::OnEntry()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnStateHandlerCandidateSelecting::OnEntry()
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerCandidateSelecting::OnExit()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnStateHandlerCandidateSelecting::OnExit()
    {
    
    }

// ============================ MEMBER FUNCTIONS ===============================
//                     CPeninputHwrfscnStateHandlerHomophonicSelecting class
// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerHomophonicSelecting::
//                             CPeninputHwrfscnStateHandlerHomophonicSelecting()
// .
// -----------------------------------------------------------------------------
// 
CPeninputHwrfscnStateHandlerHomophonicSelecting::
    CPeninputHwrfscnStateHandlerHomophonicSelecting( 
	CPeninputHwrfscnStateManager& aStateManager )
	:CPeninputHwrfscnStateHandlerBase( aStateManager )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerHomophonicSelecting::NewL()
// .
// -----------------------------------------------------------------------------
// 
CPeninputHwrfscnStateHandlerHomophonicSelecting* 
    CPeninputHwrfscnStateHandlerHomophonicSelecting::NewL( 
	CPeninputHwrfscnStateManager& aStateManager )
    {
    CPeninputHwrfscnStateHandlerHomophonicSelecting* self = new( ELeave ) 
         CPeninputHwrfscnStateHandlerHomophonicSelecting( aStateManager );
    
    return self;
                                                                                
    }
    
// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerHomophonicSelecting::HandleCommand()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnStateHandlerHomophonicSelecting::HandleCommand( TInt /*aCmd*/, 
    TUint8* /*aData*/ )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerHomophonicSelecting::HandleEventL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnStateHandlerHomophonicSelecting::HandleEventL( TInt aEventType, 
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
            
        case EHwrEventPointerOutsideWnd:
        case EHwrEventBeginWriting: 
            {
            iStateManager.SetState( 
                CPeninputHwrfscnStateManager::EPeninputHwrfscnStateStandby );
            }
            break;
            
        default:
            break;
        }    
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerHomophonicSelecting::OnSelectedCandidatesL()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputHwrfscnStateHandlerHomophonicSelecting::OnSelectedCandidatesL( 
    const TDesC& aEventData )
    {
    TPtrC ptr;
    ptr.Set( ( aEventData.Left( aEventData.Length() - 1 ) ) );
    RPointerArray<HBufC> candidates = iDataStore.Candidate();
    HBufC* defaultCandidate = candidates[0];    
    iStateManager.HwrLayout().Replace( *defaultCandidate, ptr );    
    iDataStore.TriggerStr()->Set( ptr );
    iDataStore.DoPredictiveL();
    iStateManager.SetState( 
            CPeninputHwrfscnStateManager::EPeninputHwrfscnStatePredictSelecting );
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerHomophonicSelecting::OnSelectedCandidatesL()
// .
// -----------------------------------------------------------------------------
// 
void CPeninputHwrfscnStateHandlerHomophonicSelecting::OnSelectedPredictivesL( 
    const TDesC& aEventData )
    {                                                        
    TPtrC ptr;
    ptr.Set( aEventData.Left( aEventData.Length() - 1 ) );
    iStateManager.HwrLayout().SubmitStringToFep( ptr );
    
    iDataStore.TriggerStr()->Append( ptr );
    iDataStore.DoPredictiveL();
    if( iDataStore.PredictiveCount() > 0 )
        {
        iStateManager.SetState( 
             CPeninputHwrfscnStateManager::EPeninputHwrfscnStatePredictSelecting );     
        }
    else
        {
        iStateManager.SetState( 
            CPeninputHwrfscnStateManager::EPeninputHwrfscnStateStandby );
        }
        
    }
// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerHomophonicSelecting::OnEntry()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnStateHandlerHomophonicSelecting::OnEntry()
    {
     
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerHomophonicSelecting::OnExit()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnStateHandlerHomophonicSelecting::OnExit()
    {
    
    }

// ============================ MEMBER FUNCTIONS ===============================
//                     CPeninputHwrfscnStateHandlerPredictSelecting class
// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerPredictSelecting::
//                                CPeninputHwrfscnStateHandlerPredictSelecting()
// .
// -----------------------------------------------------------------------------
// 
CPeninputHwrfscnStateHandlerPredictSelecting::
    CPeninputHwrfscnStateHandlerPredictSelecting( 
	CPeninputHwrfscnStateManager& aStateManager )
	:CPeninputHwrfscnStateHandlerBase( aStateManager )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerPredictSelecting::NewL()
// .
// -----------------------------------------------------------------------------
// 
CPeninputHwrfscnStateHandlerPredictSelecting*  
    CPeninputHwrfscnStateHandlerPredictSelecting::NewL( 
	CPeninputHwrfscnStateManager& aStateManager )
    {
    CPeninputHwrfscnStateHandlerPredictSelecting* self = new( ELeave ) 
        CPeninputHwrfscnStateHandlerPredictSelecting( aStateManager );
    
    return self;
                                                                                
    }
    
// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerPredictSelecting::HandleCommand()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnStateHandlerPredictSelecting::HandleCommand( TInt /*aCmd*/, 
    TUint8* /*aData*/ )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerPredictSelecting::HandleEventL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnStateHandlerPredictSelecting::HandleEventL( TInt aEventType, 
    const TDesC& aEventData )
    {
    switch( aEventType )
        {
        case EHwrEventPredictiveSelected:
            {
            OnSelectedPredictivesL( aEventData );
            }
            break;
            
        case EHwrEventPointerOutsideWnd:
        case EHwrEventBeginWriting:  
            {
            iStateManager.SetState( 
                CPeninputHwrfscnStateManager::EPeninputHwrfscnStateStandby );
            }
            break;
            
        default:
            break;
        }    
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerPredictSelecting::OnSelectedCandidatesL()
// .
// -----------------------------------------------------------------------------
// 
void CPeninputHwrfscnStateHandlerPredictSelecting::OnSelectedPredictivesL( 
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
    if( iDataStore.PredictiveCount() > 0 )
        {
        iStateManager.SetState( 
            CPeninputHwrfscnStateManager::EPeninputHwrfscnStatePredictSelecting );     
        }
    else
        {
        iStateManager.SetState( 
            CPeninputHwrfscnStateManager::EPeninputHwrfscnStateStandby );
        }
        
    }
    
// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerPredictSelecting::OnEntry()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnStateHandlerPredictSelecting::OnEntry()
    {
    iStateManager.HwrLayout().CandidatePanel().ShowWritingCandidate( EFalse );
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnStateHandlerPredictSelecting::OnExit()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnStateHandlerPredictSelecting::OnExit()
    {
    
    }

//End of file
