/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Inline function for HWR window base and transparent window
*
*/


const TInt KFixedStartTime = 1000000; // 1s
const TInt KFixedStartAdjustTime = 100000; //100ms
const TInt KFixedIntervalTime = 300000; // 300ms
const TInt KFixedIntervalAdjustTime = 30000; // 30ms

// -----------------------------------------------------------------------------
// Get stroke list
// -----------------------------------------------------------------------------
//
inline const RArray<TPoint>& CHwrWndBase::StrokeList()
    {
    return iListOfPoints;
    }

// -----------------------------------------------------------------------------
// Set HWR recognizer
// -----------------------------------------------------------------------------
//    
/*inline void CHwrWndBase::SetRecognizer(MHwRecognizer* aRecognizer)
    {
    iRecognizer=aRecognizer;
    }    
    */
// -----------------------------------------------------------------------------
// Set stroke delay timer -- The interval for recognition
// -----------------------------------------------------------------------------
//
inline void CHwrWndBase::SetStrokeDelay(TTimeIntervalMicroSeconds32 aStrokeDelay)
    {
    iStrokeDelay = aStrokeDelay;
    }

// -----------------------------------------------------------------------------
// Set character timer
// -----------------------------------------------------------------------------
//    
inline void CHwrWndBase::SetCharacterDelay(TTimeIntervalMicroSeconds32 aCharDelay)
    {
    iCharDelay=aCharDelay;
    }

// -----------------------------------------------------------------------------
// Set stroke end mark
// -----------------------------------------------------------------------------
//    
inline void CHwrWndBase::SetStrokeEndMark (const TPoint& aPt)
    {
    iStrokeEndMark = aPt;
    }
    
// -----------------------------------------------------------------------------
// Get current candidate
// -----------------------------------------------------------------------------
//    
/*inline const RPointerArray<HBufC>& CHwrWndBase::CandidateData()
    {
    return iResult;
    }
*/

// -----------------------------------------------------------------------------
// Get stroke end mark
// -----------------------------------------------------------------------------
//        
inline const TPoint& CHwrWndBase::StrokeEndMark()
    {
    return iStrokeEndMark;
    }

// -----------------------------------------------------------------------------
// Get previous stroke point
// -----------------------------------------------------------------------------
//    
/*inline const TPoint& CHwrWndBase::PrevPoint()
    {
    return iPtPrev;
    }
*/
// -----------------------------------------------------------------------------
// Get dirty rect
// -----------------------------------------------------------------------------
//    
inline TRect& CHwrWndBase::DirtyRect()
    {
    return iDirtyRect;
    }

    
// -----------------------------------------------------------------------------
// get new trace flag
// -----------------------------------------------------------------------------
//
inline TBool CHwrWndBase::HasNewTrace()
    {
    return iHasNewTrace;
    }
    
    
// -----------------------------------------------------------------------------
// Set border corlor. Obsolete.
// -----------------------------------------------------------------------------
//    
inline void CHwrWndBase::SetFrameCol(const TRgb& aCol)
	{
	SetBorderColor(aCol);
	}
	
// -----------------------------------------------------------------------------
// get border corlor. Obsolete.
// -----------------------------------------------------------------------------
//    
inline const TRgb& CHwrWndBase::FrameCol()
	{
	return BorderColor();
	}
	
// -----------------------------------------------------------------------------
// Set clip region
// -----------------------------------------------------------------------------
//  	
inline void CTransparentHwrWnd::SetClipRegion()
    {
    if(iTraceOutsideWindowFlag) //do nothing if able to write outside of window
        return;
    CFbsBitGc* gc= static_cast<CFbsBitGc*> (BitGc());    
    gc->SetClippingRect(Rect());        
    }
    
// -----------------------------------------------------------------------------
// Cancel clip region
// -----------------------------------------------------------------------------
//      
inline void CTransparentHwrWnd::CancelClipRegion()
    {
    if(iTraceOutsideWindowFlag)//do nothing if able to write outside of window
        return;
    
    CFbsBitGc* gc= static_cast<CFbsBitGc*> (BitGc());    
    gc->CancelClippingRect();     
    }
    
// -----------------------------------------------------------------------------
// get transparency factor
// -----------------------------------------------------------------------------
// 
inline TUint8 CTransparentHwrWnd::TransparencyFactor()
    {
    return iTransparencyFactor;
    }
    
// -----------------------------------------------------------------------------
// enable/disable fade
// -----------------------------------------------------------------------------
// 
inline void CTransparentHwrWndExt::SetEnableFade( TBool aEnable )
    {
    iEnableFade = aEnable;
    }		
// -----------------------------------------------------------------------------
// set fading speed.
// -----------------------------------------------------------------------------
// 
inline void CTransparentHwrWndExt::SetFadingSpeed(TInt aFadingSpeed)
    {
    switch ( aFadingSpeed )
        {
        case 1:
            {
            iFadeBeginTime = KFixedStartTime - KFixedStartAdjustTime*2; // 800ms
            iFadeIntervalTime = KFixedIntervalTime - KFixedIntervalAdjustTime*2; // 240ms
            }
            break;
        case 2:
            {
            iFadeBeginTime = KFixedStartTime - KFixedStartAdjustTime; // 900ms
            iFadeIntervalTime = KFixedIntervalTime - KFixedIntervalAdjustTime; // 270ms
            }
            break;
        case 3:
            {
            iFadeBeginTime = KFixedStartTime; // 1s
            iFadeIntervalTime = KFixedIntervalTime; // 300ms
            }
            break;
        case 4:
            {
            iFadeBeginTime = KFixedStartTime + KFixedStartAdjustTime; // 1.2s
            iFadeIntervalTime = KFixedIntervalTime + KFixedIntervalAdjustTime; // 330ms
            }
            break;
        case 5:
            {
            iFadeBeginTime = KFixedStartTime + KFixedStartAdjustTime*2; // 1.4s
            iFadeIntervalTime = KFixedIntervalTime + KFixedIntervalAdjustTime*2; // 360ms
            }
            break;
        }    
    }
//end of file
     