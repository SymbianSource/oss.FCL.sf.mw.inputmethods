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
* Description:  interface for pen input ui layout owner
*
*/



#ifndef M_FEPUILAYOUTOWNERINF_H
#define M_FEPUILAYOUTOWNERINF_H

#include <bitdev.h>

/**
 * Class MLayoutOwner.
 *
 * Interface for layout owner. 
 * A layout owner is responsible for displaying the layout.
 *
  * @since S60 V4.0
 */
class MLayoutOwner
    {
public:
    /**
     * Update a rectangle area
     *
     * @since S60 v4.0
     * @param aRect The rectangle area to be updated
     * @param aFullUpdateFlag Full update flag. Re-calculates the image due to background change. 
     * @param aImmedFlag Flags indicates whethere updating is executed immediately or buffered.
     */
    virtual void UpdateArea(const TRect& aRect,TBool aFullUpdateFlag, TBool aImmedFlag) = 0;

    /**
     * Set layout position. This should only be called after layout has change 
     * its position, i.e, the layout top left is the new position.
     *
     * @since S60 v4.0
     * @param aNewPos The new top left position of the layout
     * @return Return the old position of the layout
     */
    virtual TPoint SetPosition(const TPoint& aNewPos) = 0;

    /**
     * Get the Bitmap context for drawing.
     *
     * @since S60 v4.0
     * @return bitmap context
     */
    virtual CBitmapContext* BitmapContext () = 0;

    /**
     * Get the bitmap device
     *
     * @since S60 v4.0
     * @return The bitmap device for drawing
     */
    virtual CFbsBitmapDevice* BitmapDevice() = 0;

    /**
     * Get the mask bitmap device
     *
     * @since S60 v4.0
     * @return The mask bitmap device for drawing
     */
    virtual CFbsBitmapDevice* MaskBmpDevice() = 0;
    
    /**
     * Hide or show the layout
     *
     * @since S60 v4.0     
     * @param aHideFlat Flag to hide or show the layout.Hide layout if it's true.
     */
    virtual void Hide(TBool aHideFlag) = 0;
    
    /**
     * Signal owner that there is an event
     *
     * @since S60 v4.0     
     * @param aEventType The event type
     * @param aEventData The event data
     */    
    virtual void SignalOwner(TInt aEventType, const TDesC& aEventData) = 0;
    
    /**
     * Handle layout size change event
     *
     * @since S60 v4.0     
     * @param aSize The new layout size
     * @return standard symbian error code
     */       
    virtual TInt LayoutSizeChanged(const TSize& aSize, TBool aNeedUpdate = ETrue) = 0;
    
    /**
     * Get layout position and it's rect.
     * The position is relative to it's parent window or screen.
     * The size includes all invisible layout area as well as the shadow.
     *
     * @since S60 v4.0     
     * @return layout position.
     */       
    virtual TRect LayoutRect() = 0;    
		
    /**
     * Update UI layout immediatelly.
     *
     * @since S60 v5.0     
     * @return None.
     */       
    virtual void FlushUi() = 0;    
    };

#endif //M_FEPUILAYOUTOWNERINF_H

