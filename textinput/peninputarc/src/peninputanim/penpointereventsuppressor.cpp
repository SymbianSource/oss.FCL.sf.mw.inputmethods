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

#include <penpointereventsuppressor.h>

const TInt KPenPointerEventSuppressorDefaultMaxTapDuration = 400000;	// 0.4 seconds
const TInt KPenPointerEventSuppressorDefaultMinInterDragInterval = 0;
const TInt KPenPointerEventSuppressorDefaultMovement = 6;

const TInt KPenPointerEventSuppressorDefaultMaxDownUpDuration = 400000; // 0.4 seconds
const TInt KPenPointerEventSuppressorDefaultDownUpMovement = 7;

CPenPointerEventSuppressor* CPenPointerEventSuppressor::NewL()
	{
	return new (ELeave) CPenPointerEventSuppressor;
	}

CPenPointerEventSuppressor::~CPenPointerEventSuppressor()
	{
	
	}

CPenPointerEventSuppressor::CPenPointerEventSuppressor()
: iMaxTapDuration( KPenPointerEventSuppressorDefaultMaxTapDuration ),
  iMinInterDragInterval( KPenPointerEventSuppressorDefaultMinInterDragInterval ),
  iTap( EFalse ),
  iMaxDownUpDuration( KPenPointerEventSuppressorDefaultMaxDownUpDuration )
  	{
  	// default move limit is 6 units, which seems to be a forgiving value for finger touch
  	iMaxTapMove.iWidth = KPenPointerEventSuppressorDefaultMovement;
	iMaxTapMove.iHeight = KPenPointerEventSuppressorDefaultMovement;
	
	iMaxDownUpMove.iWidth = KPenPointerEventSuppressorDefaultDownUpMovement;
	iMaxDownUpMove.iHeight = KPenPointerEventSuppressorDefaultDownUpMovement;
  	}

TBool CPenPointerEventSuppressor::SuppressPointerEvent( TPointerEvent& aPointerEvent )
	{
	switch ( aPointerEvent.iType )
		{
		case TPointerEvent::EButton1Down:
		    {
			iDownTime.HomeTime();
			iTap = ETrue;
			iDownPos = aPointerEvent.iPosition;
			iLastEventTime = iDownTime;
			break;
		    }		
		case TPointerEvent::EDrag:
			{
			TTime now;
			now.HomeTime();
			if (iTap)
				{
				TPoint delta = aPointerEvent.iPosition - iDownPos;
				if (Abs(delta.iX) > iMaxTapMove.iWidth ||
					Abs(delta.iY) > iMaxTapMove.iHeight ||
					now.MicroSecondsFrom(iDownTime) >= iMaxTapDuration)
					{
					// This touch action has gone outside the parameters of a tap
					// the drag event should be handled
					iTap = EFalse;
					iLastEventTime = now;
					return EFalse;
					}
				else
					{
					// still a tap, so suppress the drag event
					return ETrue;
					}
				}
			else if (now.MicroSecondsFrom(iLastEventTime) < iMinInterDragInterval)
				{
				// too soon since the last drag, suppress it
				return ETrue;
				}

			// this drag event should be handled
			iLastEventTime = now;
			break;
			}
		case TPointerEvent::EButton1Up:
            {
            TTime now;
            now.HomeTime();
            TPoint delta = aPointerEvent.iPosition - iDownPos;
            if ( now.MicroSecondsFrom( iDownTime ) < iMaxDownUpDuration 
                 && Abs( delta.iX ) < iMaxDownUpMove.iWidth 
                 && Abs( delta.iY ) < iMaxDownUpMove.iHeight )
                {
                //within maximum movement and timeout, so move to position of down
                aPointerEvent.iPosition = iDownPos;
                }
			iTap = EFalse;
			break;
		    }
		case TPointerEvent::ENullType:
		    break;
		default:
			break;
		}
	
	// all non-drag events should be handled
	return EFalse;
	}

void CPenPointerEventSuppressor::SetMaxTapDuration(
        TTimeIntervalMicroSeconds aDuration)
	{
	iMaxTapDuration = aDuration;
	}

void CPenPointerEventSuppressor::SetMaxTapMove(TSize aMoveLimits)
	{
	iMaxTapMove = aMoveLimits;
	}
	
void CPenPointerEventSuppressor::SetMinInterDragInterval(
        TTimeIntervalMicroSeconds aInterval)
	{
	iMinInterDragInterval = aInterval;
	}

void CPenPointerEventSuppressor::SetMaxDownUpMove( TSize aMaxDownUpMove ) 
    { 
    iMaxDownUpMove = aMaxDownUpMove; 
    }

void CPenPointerEventSuppressor::SetMaxDownUpDuration( TTimeIntervalMicroSeconds aDuration ) 
    {
    iMaxDownUpDuration = aDuration; 
    }
