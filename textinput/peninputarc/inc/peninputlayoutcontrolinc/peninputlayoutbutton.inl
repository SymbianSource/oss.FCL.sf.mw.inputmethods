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
* Description:  Inline function for button base and dragbar
*
*/


//class CButtonBase

// ---------------------------------------------------------------------------
// Is button dimmed?
// ---------------------------------------------------------------------------
//    
inline TBool CButtonBase::IsDimmed()
    {
    return iDimmed;
    }


// ---------------------------------------------------------------------------
// Set button bitmap stretchable status
// ---------------------------------------------------------------------------
//    
inline void CButtonBase::SetBmpStretchable(TBool aFlag)
    {
    iStretchable = aFlag;    
    }
    
// ---------------------------------------------------------------------------
// get button bitmap stretchable status
// ---------------------------------------------------------------------------
//    
inline TBool CButtonBase::BmpStretchable()
    {
    return iStretchable;    
    }
    
// ---------------------------------------------------------------------------
// set caption font color
// ---------------------------------------------------------------------------
//    
inline void CButtonBase::SetCaptionColor(const TRgb& aCol)
    {
    iCaptionColor = aCol;
    }
    
// ---------------------------------------------------------------------------
// set caption shadow font color
// ---------------------------------------------------------------------------
//       
inline void CButtonBase::SetCaptionShadowColor(const TRgb& aCol)    
    {
    iCaptionShadowColor = aCol;
    }
//class CDragBar
// ---------------------------------------------------------------------------
// Get dragbar draging status
// ---------------------------------------------------------------------------
//
inline TBool CDragBar::IsDragging()
    {
    return iIsDragging;
    }  

// ---------------------------------------------------------------------------
// Enable/Disable dragbar's draging feature
// ---------------------------------------------------------------------------
//
inline void CDragBar::EnableDragging(TBool aFlag)
    {
    iDraggingEnabled = aFlag;
    }

// ---------------------------------------------------------------------------
// Set the redraw interval while moving
// ---------------------------------------------------------------------------
//
inline void CDragBar::SetRedrawInterval(TTimeIntervalMicroSeconds32 aTime)
    {
    iDrawInterval = aTime;
    }
//end of class CDragBar

//end of class CButtonBase
