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

#include "peninputhwrfscnwritingbox.h"
#include "peninputhwrfscnlayout.h"
#include "peninputhwrfscndatastore.h"
#include "peninputhwrfscnevent.h"

// ============================ MEMBER FUNCTIONS ===============================
  
// -----------------------------------------------------------------------------
// CPeninputHwrfscnWritingBox::NewL()
// .
// -----------------------------------------------------------------------------
//
CPeninputHwrfscnWritingBox* CPeninputHwrfscnWritingBox::NewL( 
    CPeninputHwrfscnLayout* aLayout )
    {
    CPeninputHwrfscnWritingBox* self = new( ELeave ) 
        CPeninputHwrfscnWritingBox( aLayout );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnWritingBox::~CPeninputHwrfscnWritingBox()
// .
// -----------------------------------------------------------------------------
//
CPeninputHwrfscnWritingBox::~CPeninputHwrfscnWritingBox()
    {
    iStrokeList.Reset();
    iStrokeList.Close();
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnWritingBox::Show()
// .
// -----------------------------------------------------------------------------
//       
void CPeninputHwrfscnWritingBox::Show( TBool /*aVisible*/ )
    {    
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnWritingBox::Draw()
// .
// -----------------------------------------------------------------------------
// 
void CPeninputHwrfscnWritingBox::Draw()
    {
    iHwrWnd->Draw();
    }
    
// -----------------------------------------------------------------------------
// CPeninputHwrfscnWritingBox::HandleControlEvent()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnWritingBox::HandleControlEvent( TInt aEventType, 
    CFepUiBaseCtrl* /*aCtrl*/, 
	const TDesC& /*aEventData*/ )
    {
    switch( aEventType )
        {
        case EEventHwrCharacterTimerOut:
            {
            OnStrokeCharacterTimerOut();
            iLayout->EnableStrokeFilter( ETrue );
            }
            break;
            
        case EEventHwrStrokeStarted:
            {
            OnStrokeStarted();
            iLayout->EnableStrokeFilter( EFalse );
            }
            break;
            
        case EEventHwrStrokeFinished:
            {
            OnStrokeFinished();		
            }
            break;
            
        default: 
            break;       
        }
    }		                        

// -----------------------------------------------------------------------------
// CPeninputHwrfscnWritingBox::HandleCommand()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnWritingBox::HandleCommand( TInt /*aCommand*/, 
    TUint8* /*aData*/ )
    {
    
    }    

// -----------------------------------------------------------------------------
// CPeninputHwrfscnWritingBox::BeforePropertyChangedL()
// .
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrfscnWritingBox::BeforePropertyChangedL(            
    MPeninputHwrfscnPropertySubscriber::TPeninputHwrfscnProperty /*aProptery*/ )
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnWritingBox::PropertyChangedL()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputHwrfscnWritingBox::PropertyChangedL( 
	    MPeninputHwrfscnPropertySubscriber::TPeninputHwrfscnProperty aProptery )
    {
      switch( aProptery )
        {
        case EPeninputHwrfscnPropertyWriteSpeed:
            {
            TTimeIntervalMicroSeconds32 speed = 
                iLayout->DataStore().WritingSpeed();
            SetWritingSpeed( speed );
            }
            break;
            
        case EPeninputHwrfscnPropertyPenSize:
            {
            TSize penSize = iLayout->DataStore().PenSize();
            SetPenSize( penSize );
            }
            break;
            
        case EPeninputHwrfscnPropertyPenColor:
            {
            TRgb& color = iLayout->DataStore().PenColor();
            SetPenColor( color );
            }
            break;
            
        default:
            break;
        }
    }	    

// -----------------------------------------------------------------------------
// CPeninputHwrfscnWritingBox::CPeninputHwrfscnWritingBox()
// .
// -----------------------------------------------------------------------------
//	
CPeninputHwrfscnWritingBox::CPeninputHwrfscnWritingBox( 
    CPeninputHwrfscnLayout* aLayout )
    :CPeninputHwrfscnBasePanel( aLayout )
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnWritingBox::ConstructL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnWritingBox::ConstructL()
    {
    TRect hwrRect( TPoint( 0, 0 ), iLayout->ScreenSize() );
    
    //create full screen hwr window
    iHwrWnd = CTransparentHwrWnd::NewL( hwrRect, iLayout, 
        ECtrlTransparentHwrWnd, ETrue );
    AddToRootControlL( iHwrWnd );
    
    //add this to datastore subscriber list
    iLayout->DataStore().AddSubscriber( this );
    
    //load the default hwr settings: pen size, speed, color
    iLayout->DataStore().LoadAndPublishDefaultL();
    
    //get stroke end mark
    TPoint strokeEndMark = iLayout->DataStore().StrokeEndMark();

    //set it to window
    iHwrWnd->SetStrokeEndMark( strokeEndMark );
    iHwrWnd->AddEventObserver( this );
    iHwrWnd->EnableTraceOutsideWindow( ETrue );
    
    Draw();
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnWritingBox::SizeChanged()
// .
// -----------------------------------------------------------------------------
//    	  
void CPeninputHwrfscnWritingBox::SizeChanged()
    {
    TRect hwrRect( TPoint( 0, 0 ), iLayout->ScreenSize() );
    iHwrWnd->SetRect( hwrRect ); 
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnWritingBox::LafChanged()
// .
// -----------------------------------------------------------------------------
//	    
void CPeninputHwrfscnWritingBox::LafChanged()
    {
    
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnWritingBox::OnStrokeCharacterTimerOut()
// .
// -----------------------------------------------------------------------------
//	
void CPeninputHwrfscnWritingBox::OnStrokeCharacterTimerOut()
    {
    // Get stroke list is called by event handler
    TPtrC ptr;
    ptr.Set( reinterpret_cast<TText*>( &iStrokeList ), 
        sizeof( &iStrokeList ) );
    ReportEvent( EHwrEventEndWriting, iHwrWnd, ptr );
    } 

// -----------------------------------------------------------------------------
// CPeninputHwrfscnWritingBox::OnStrokeStarted()
// .
// -----------------------------------------------------------------------------
//	    
void CPeninputHwrfscnWritingBox::OnStrokeStarted()
    {
    ReportEvent( EHwrEventBeginWriting, iHwrWnd, KNullDesC );
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnWritingBox::OnStrokeStarted()
// .
// -----------------------------------------------------------------------------
//	
void CPeninputHwrfscnWritingBox::OnStrokeFinished()
    {
    // copy data from iHwrWnd's stroke list
    iStrokeList.Reset();
    RArray<TPoint> dataList = iHwrWnd->StrokeList();
    for( TInt i = 0; i < dataList.Count(); i++ )
        {
        iStrokeList.Append( dataList[i] );
        }
    }
    
// -----------------------------------------------------------------------------
// CPeninputHwrfscnWritingBox::CancelCharWriting()
// .
// -----------------------------------------------------------------------------
//	
void CPeninputHwrfscnWritingBox::CancelCharWriting()
    {
    iLayout->EnableStrokeFilter( ETrue );
    iHwrWnd->CancelCharWriting();
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnWritingBox::SetWritingSpeed()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputHwrfscnWritingBox::SetWritingSpeed( 
    const TTimeIntervalMicroSeconds32& aCharDelay )
    {
    iHwrWnd->SetCharacterDelay( aCharDelay );
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnWritingBox::SetBoxPenSize()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputHwrfscnWritingBox::SetPenSize( const TSize aSize )
    {
    iHwrWnd->SetPenSize( aSize );   
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnWritingBox::SetBoxPenColor()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputHwrfscnWritingBox::SetPenColor( const TRgb aCol )
    {
    iHwrWnd->SetPenColor( aCol );
    }

//End of file
