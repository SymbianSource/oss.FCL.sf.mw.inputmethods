/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Declares document for CpeninputsettingAppView
*
*/

#ifndef __PENINPUTSETTING_APPVIEW_H__
#define __PENINPUTSETTING_APPVIEW_H__


#include <coecntrl.h>

/*! 
  @class CpeninputsettingAppView
  
  @discussion An instance of the Application View object for the peninputsetting 
  example application
  */
class CpeninputsettingAppView : public CCoeControl
    {
public:

/*!
  @function NewL
   
  @discussion Create a CpeninputsettingAppView object, which will draw itself to aRect
  @param aRect the rectangle this view will be drawn to
  @result a pointer to the created instance of CpeninputsettingAppView
  */
    static CpeninputsettingAppView* NewL(const TRect& aRect);

/*!
  @function NewLC
   
  @discussion Create a CpeninputsettingAppView object, which will draw itself to aRect
  @param aRect the rectangle this view will be drawn to
  @result a pointer to the created instance of CpeninputsettingAppView
  */
    static CpeninputsettingAppView* NewLC(const TRect& aRect);


/*!
  @function ~CpeninputsettingAppView
  
  @discussion Destroy the object and release all memory objects
  */
     ~CpeninputsettingAppView();


public:  // from CCoeControl
/*!
  @function Draw
  
  @discussion Draw this CpeninputsettingAppView to the screen
  @param aRect the rectangle of this view that needs updating
  */
    void Draw(const TRect& aRect) const;
  

private:

/*!
  @function ConstructL
  
  @discussion  Perform the second phase construction of a CpeninputsettingAppView object
  @param aRect the rectangle this view will be drawn to
  */
    void ConstructL(const TRect& aRect);

/*!
  @function CpeninputsettingAppView
  
  @discussion Perform the first phase of two phase construction 
  */
    CpeninputsettingAppView();
    };


#endif // __PENINPUTSETTING_APPVIEW_H__
