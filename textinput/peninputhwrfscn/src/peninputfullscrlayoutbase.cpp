/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for layout base
*
*/

//SYSTEM INCLUDES
#include <e32math.h>

//FEP Includes
#include <peninputcmd.h>

//USER INCLUDES
#include "peninputfullscrlayoutbase.h"

// Test for Scroll bar laf read
#include "peninputhwrfscnlafmanager.h"


//CONST DEFINATION
const TInt KMaxBufferEventNum = 30;
const TInt KMinBufEventsCount = 2;

const TInt KAngle90 = 90;
const TInt KAngle180 = 180;
const TInt KAngle270 = 270;
const TInt KAngle360 = 360;
const TInt KPendownHoldTime = 800 * 1000;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPenInputFullScreenLayoutBase::CPenInputFullScreenLayoutBase
// Constructor
// ---------------------------------------------------------------------------
//
CPenInputFullScreenLayoutBase::
    CPenInputFullScreenLayoutBase( MLayoutOwner* aLayoutOwner )
    :CFepUiLayout( aLayoutOwner )
    {
    iEnableStrokeFilter = ETrue;
    }

// ---------------------------------------------------------------------------
// CPenInputFullScreenLayoutBase::~CPenInputFullScreenLayoutBase
// Destructor
// ---------------------------------------------------------------------------
//
CPenInputFullScreenLayoutBase::~CPenInputFullScreenLayoutBase()
    {
    delete iLongTapPeriodic;
    }

// ---------------------------------------------------------------------------
// CPenInputFullScreenLayoutBase::BaseConstructL
// Do basic contructor
// ---------------------------------------------------------------------------
//
void CPenInputFullScreenLayoutBase::BaseConstructL()
    {
    CFepUiLayout::BaseConstructL();

	iLongTapPeriodic = CPeriodic::NewL( CActive::EPriorityStandard );
    }

// ---------------------------------------------------------------------------
// CPenInputFullScreenLayoutBase::PeriodicCallBack
// .
// ---------------------------------------------------------------------------
//
TInt CPenInputFullScreenLayoutBase::PeriodicCallBack( TAny *aAnyPtr )
	{
	CPenInputFullScreenLayoutBase *pThis = 
	    static_cast<CPenInputFullScreenLayoutBase*>( aAnyPtr );
	return pThis->OnLongTapTimeOut();
	}

// ---------------------------------------------------------------------------
// CPenInputFullScreenLayoutBase::StartLongTapTimer
// .
// ---------------------------------------------------------------------------
//
void CPenInputFullScreenLayoutBase::StartLongTapTimer()
	{
	if ( !iLongTapPeriodic->IsActive() )
	    {
        iLongTapPeriodic->Start( KPendownHoldTime/*0.8s*/, 0, 
            TCallBack( PeriodicCallBack, this ) );	    
	    }
	}

// ---------------------------------------------------------------------------
// CPenInputFullScreenLayoutBase::StopLongTapTimer
// .
// ---------------------------------------------------------------------------
//
void CPenInputFullScreenLayoutBase::StopLongTapTimer()
	{
	iLongTapPeriodic->Cancel();
	}

// ---------------------------------------------------------------------------
// CPenInputFullScreenLayoutBase::RecordPointerEvent
// .
// ---------------------------------------------------------------------------
//
void CPenInputFullScreenLayoutBase::
    RecordPointerEvent( const TRawEvent &aEvent )
	{
	if( iEventBuf.Count() == KMaxBufferEventNum )
		{
		//replace the last pointer move event with new one;
		if( iEventBuf[KMaxBufferEventNum - 1].Type() == 
		    TRawEvent::EPointerMove )
			{
			iEventBuf[KMaxBufferEventNum - 1] = aEvent;
			}
		}
	else
		{
		iEventBuf.Append( aEvent );
		}
	}

// ---------------------------------------------------------------------------
// CPenInputFullScreenLayoutBase::SendBufferEventToLayoutL
// .
// ---------------------------------------------------------------------------
//
void CPenInputFullScreenLayoutBase::SendBufferEventToLayoutL()
	{
	TInt count = iEventBuf.Count();
    for( TInt i = 0; i < count; i++ )
	    {
	    CFepUiLayout::HandleEventL( ERawEvent, &iEventBuf[i] );
	    }

	iEventBuf.Reset();
	}

// ---------------------------------------------------------------------------
// CPenInputFullScreenLayoutBase::SendBufferEventToBackground
// .
// ---------------------------------------------------------------------------
//
void CPenInputFullScreenLayoutBase::SendBufferEventToBackground()
	{
	BeforeSendingEventsToBackground();
	TInt count = iEventBuf.Count();
    for( TInt i = 0; i < count; i++ )
        {
    	TPtrC data;
    	data.Set( reinterpret_cast<TUint16*>( &iEventBuf[i] ), 
    	sizeof( TRawEvent ) / sizeof( TUint16 ) );

        SignalOwner( ESignalSimulateEvent, data );
        }

    iEventBuf.Reset();
	}

// ---------------------------------------------------------------------------
// CPenInputFullScreenLayoutBase::CalcDiagonalDistance
// .
// ---------------------------------------------------------------------------
//
TReal CPenInputFullScreenLayoutBase::CalcDiagonalDistance()
	{
	//init left top right bottom
	TPoint leftTop = iEventBuf[0].Pos();
	TPoint rightBottom = iEventBuf[iEventBuf.Count() - 1].Pos();

	//find min left top and max right bottom
	TInt count = iEventBuf.Count();
	for( TInt i = 1; i < count - 1; ++i )
		{
		TPoint pt = iEventBuf[i].Pos();
		leftTop.iX = Min( leftTop.iX, pt.iX );
		leftTop.iY = Min( leftTop.iY, pt.iY );
		rightBottom.iX = Max( rightBottom.iX, pt.iX );
		rightBottom.iY = Max( rightBottom.iY, pt.iY );
		}

	//calculate width and height
	TInt nWidth = rightBottom.iX - leftTop.iX;
	TInt nHeight = rightBottom.iY - leftTop.iY;

	//calculate diagonaDistance accroding "Pythagorean proposition"
	TReal diagonaDistanceSquare = nWidth * nWidth + nHeight * nHeight;
	TReal diagonaDistance = 0;
	Math::Sqrt( diagonaDistance, diagonaDistanceSquare );

	return diagonaDistance;
	}

// ---------------------------------------------------------------------------
// CPenInputFullScreenLayoutBase::CalcAngle
// .
// ---------------------------------------------------------------------------
//
TInt CPenInputFullScreenLayoutBase::CalcAngle()
	{
	TPoint startPos = iEventBuf[0].Pos();
	TPoint endPos = iEventBuf[iEventBuf.Count() - 1].Pos();

    TReal x = endPos.iX - startPos.iX;
    TReal y = startPos.iY - endPos.iY;

    if ( ( endPos.iX - startPos.iX ) == 0 )
        {
        if ( y > 0 )
            {
            return KAngle90;
            }
        else
            {
            return KAngle270;
            }
        }

    TReal radians = 0;
    Math::ATan( radians, y, x );
    TReal angle = radians * KAngle180 / KPi;

    if ( x < 0 )
        {
        angle += KAngle180;
        }
    else if ( x > 0 )
        {
        angle += KAngle360;
        }

    if ( angle >= KAngle360 )
        {
        angle -= KAngle360;
        }

    return angle;
	}

// ---------------------------------------------------------------------------
// CPenInputFullScreenLayoutBase::IsStroke
// .
// ---------------------------------------------------------------------------
//
TBool CPenInputFullScreenLayoutBase::IsStroke()
	{
	if( iEventBuf.Count() < KMinBufEventsCount )
		{
		return EFalse;
		}

	//diagonal distance in pixel
	TReal n = CalcDiagonalDistance();

	//anti-clockwise angle of the two lines from origin to point.
	TInt m = CalcAngle();

	if( iEventBuf[iEventBuf.Count() - 1].Type() == TRawEvent::EButton1Up )
		{
		//if end with button up event then it's a quickly tap
		if( n > 14 )
			{
			return ETrue;
			}
		else if( n > 7.02 )
			{
			if( m < 80 || m > 260 )
				{
				return ETrue;
				}
			}
		}
	else
		{
		//else pen is moving
		if( n > 14 )
			{
			return ETrue;
			}
		}

	return EFalse;
	}

// ---------------------------------------------------------------------------
// CPenInputFullScreenLayoutBase::OnRawEvent_Button1Down
// .
// ---------------------------------------------------------------------------
//
TBool CPenInputFullScreenLayoutBase::
    OnRawEvent_Button1Down( const TRawEvent &aEvent )
	{
	//tap start from non-hwrbox
	if( RootControl()->NonHwrStartingPtRegion().Contains( aEvent.Pos() ) )
		{
		iPenState = EPenStateStroke;
		return EFalse;
		}
		
    // Just test scroll bar rect problem
	TRect scrollBarRect = PeninputHwrfscnLAFManager::ScrollBarRect();
	if ( scrollBarRect.Contains( aEvent.Pos() ) )
	    {
	    RecordPointerEvent( aEvent );
		iPenState = EPenStateLongTap;
		SendBufferEventToBackground();
		return ETrue;
	    }

	StartLongTapTimer();

	RecordPointerEvent( aEvent );

	iPenState = EPenStateUnknown;

	return ETrue;
	}

// ---------------------------------------------------------------------------
// CPenInputFullScreenLayoutBase::OnLongTapTimeOut
// .
// ---------------------------------------------------------------------------
//
TInt CPenInputFullScreenLayoutBase::OnLongTapTimeOut()
	{
	StopLongTapTimer();

	if( IsStroke() )
		{
		iPenState = EPenStateStroke;
		TRAP_IGNORE( SendBufferEventToLayoutL() );
		}
	else
		{
		iPenState = EPenStateLongTap;
		SendBufferEventToBackground();
		}

	return KErrNone;
	}

// ---------------------------------------------------------------------------
// CPenInputFullScreenLayoutBase::OnRawEvent_PointerMove
// .
// ---------------------------------------------------------------------------
//
TBool CPenInputFullScreenLayoutBase::
    OnRawEvent_PointerMove( const TRawEvent &aEvent )
	{
	if( iPenState == EPenStateStroke )
		{
		return EFalse;
		}

	RecordPointerEvent( aEvent );

	if( iPenState == EPenStateUnknown )
		{
		if( IsStroke() )
			{
			iPenState = EPenStateStroke;

	    	StopLongTapTimer();

			TRAP_IGNORE( SendBufferEventToLayoutL() );
			}
		}
	else if( iPenState == EPenStateLongTap )
		{
		SendBufferEventToBackground();
		}

	return ETrue;
	}

// ---------------------------------------------------------------------------
// CPenInputFullScreenLayoutBase::OnRawEvent_Button1Up
// .
// ---------------------------------------------------------------------------
//
TBool CPenInputFullScreenLayoutBase::
    OnRawEvent_Button1Up( const TRawEvent &aEvent )
	{
	StopLongTapTimer();

	if( iPenState == EPenStateStroke )
		{
		return EFalse;
		}

	RecordPointerEvent( aEvent );

	if( iPenState == EPenStateUnknown )
		{
		if( IsStroke() )
			{
			//send event to layout
			TRAP_IGNORE( SendBufferEventToLayoutL() );
			}
		else
			{
			//send event to background app
			SendBufferEventToBackground();
			}
		}
	else if( iPenState == EPenStateLongTap )
		{
		////send event to background app
		SendBufferEventToBackground();
		}

	return ETrue;
	}

// ---------------------------------------------------------------------------
// CPenInputFullScreenLayoutBase::HandleEventL
// .
// ---------------------------------------------------------------------------
//
TBool CPenInputFullScreenLayoutBase::
    HandleEventL( TEventType aType, const TAny* aData )
    {
    const TRawEvent &event = *( ( TRawEvent* ) aData );
    
    PreHandleEvent( aType, event );

	TBool handled = EFalse;

    //if pointer is captured, don't buffer the event.
    if( RootControl()->CtrlCapPointer() )
    	{
    	return CFepUiLayout::HandleEventL( aType, aData );
    	}

	if( iEnableStrokeFilter && aType == ERawEvent )
		{
		switch( event.Type() )
			{
			case TRawEvent::EButton1Down:
				{
				handled = OnRawEvent_Button1Down( event );
				break;
				}

			case TRawEvent::EButton1Up:
				{
				handled = OnRawEvent_Button1Up( event );
				break;
				}

			case TRawEvent::EPointerMove:
				{
				handled = OnRawEvent_PointerMove( event );
				break;
				}

			default:
				break;
			}
		}

    if( handled )
    	{
    	return ETrue;
    	}


	if( !iEnableStrokeFilter && aType == ERawEvent )
		{
		switch( event.Type() )
			{
			case TRawEvent::EButton1Down:
				{
				RecordPointerEvent( event );
				return EFalse;
				}
			case TRawEvent::EButton1Up:
				{
				RecordPointerEvent( event );
			    SendBufferEventToLayoutL();
			    return ETrue;
				}
			case TRawEvent::EPointerMove:
				{
				RecordPointerEvent( event );
				CFepUiBaseCtrl* hwrWnd = RootControl()->ControlByType(ECtrlTransparentHwrWnd);
				if(IsStroke() && hwrWnd)
            	    {
        	        hwrWnd->CapturePointer();
        	        SendBufferEventToLayoutL();
            	    }
				return ETrue;
				}
			default:
				break;
			}
		}
    iEventBuf.Reset();
   	return CFepUiLayout::HandleEventL( aType, aData );
    }

// ---------------------------------------------------------------------------
// CPenInputFullScreenLayoutBase::SemiTransparencyRequired
// .
// ---------------------------------------------------------------------------
//
TBool CPenInputFullScreenLayoutBase::SemiTransparencyRequired()
    {
    return EFalse;
    }

// ---------------------------------------------------------------------------
// CPenInputFullScreenLayoutBase::EnableStrokeFilter
// .
// ---------------------------------------------------------------------------
//
void CPenInputFullScreenLayoutBase::EnableStrokeFilter( TBool aEnable )
    {
    iEnableStrokeFilter = aEnable;
    }

// ---------------------------------------------------------------------------
// CPenInputFullScreenLayoutBase::StrokeFilter
// .
// ---------------------------------------------------------------------------
//
TBool CPenInputFullScreenLayoutBase::StrokeFilter() const
    {
    return iEnableStrokeFilter;
    }
// ---------------------------------------------------------------------------
// CPenInputFullScreenLayoutBase::PreHandleEvent
// .
// ---------------------------------------------------------------------------
//
void CPenInputFullScreenLayoutBase::
    PreHandleEvent( TEventType /*aType*/, const TRawEvent &/*aEvent*/ )
	{
	//do nothing
	}

// ---------------------------------------------------------------------------
// CPenInputFullScreenLayoutBase::BeforeSendingEventsToBackground
// .
// ---------------------------------------------------------------------------
//
void CPenInputFullScreenLayoutBase::BeforeSendingEventsToBackground()
    {
    //do noting
    }
//End of file
