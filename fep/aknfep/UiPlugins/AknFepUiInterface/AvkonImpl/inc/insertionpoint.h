/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the CInsertionPoint definition.
*
*/











#ifndef INSERTIONPOINT_H
#define INSERTIONPOINT_H

#include <e32base.h>	// For CActive, link against: euser.lib
#include <e32std.h>		// For RTimer, link against: euser.lib
#include <w32std.h>
#include "aknfepuilayoutdatamgr.h"

/**
 * The cursor observer interface.
 * 
 * @lib aknfepuiavkonplugin.dll
 * @since S60 v5.0
 */
class MRefreshObserver
    {
public:
    /**
     * When the cursor state is changed,it will notify its observer through this function.
     * 
     * @since S60 v5.0
     * @param None.
     * @return None.
     */
    virtual void Refresh( ) = 0;
    };

/**
 * The cursor class.
 * 
 * @lib aknfepuiavkonplugin.dll
 * @since S60 v5.0
 */

class CInsertionPoint : public CActive
    {
public:
    enum TInsertionPointState
        {
        EUninitialized,
        EInitialized,
        EError
        };

    /**
     * Cancel and destroy.
     * 
     * @since S60 v5.0.
     * @return none.
     */
    virtual ~CInsertionPoint( );

    /**
     * NewL function.
     * 
     * @since S60 v5.0.
     * @return none.
     */
     static CInsertionPoint* NewL( CAknFepUiLayoutDataMgr* aLafDataMgr );

    /**
     * NewLC funciton. 
     * 
     * @since S60 v5.0.
     * @return none. 
     */
     static CInsertionPoint* NewLC( CAknFepUiLayoutDataMgr* aLafDataMgr );

    /**
     * Set iRefreshObserver member.
     * 
     * @since S60 v5.0
     * @param aObserver. The observer you want to set.
     * @return none.
     */
     void SetRefreshObserver( MRefreshObserver* aObserver );

    /**
     * Set the position of cursor.
     * The max position of cursor is larger than length of visual text 
     * by 1 stride.
     * 
     * @since S60 5.0
     * @param aOffset. The cursor position which based on 0 index.
     * @return None.
     */
     void SetPosition( TInt aOffSet );

    /**
     * Set the rect where visual text will be displayed.
     * 
     * @since S60 v5.0
     * @param aRect. The rect you want to display the visual text in.
     * @return None.
     */
     void SetTextRect( TRect aRect );

    /**
     * Set the height of cursor.
     * 
     * @since S60 v5.0
     * @param aHeight. The cursor height you want to set.
     * @return.
     */
     void SetHeight();

    /**
     * Draw the cursor on the screen.
     * 
     * @since S60 v5.0
     * @param aGc. the gc you used to draw the cursor.
     * @return.
     */
     void Draw( CWindowGc& aGc );

    /**
     * Active the cursor into active state.
     * 
     * @since S60 v5.0
     * @param None.
     * @return None.
     */
     void Active();

    /**
     * Deactive the cursor into deactive state.
     * 
     * @since S60 v5.0
     * @param None.
     * return None.
     */
     void Deactive();

private:

    /**
     * Insertion Point.
     * 
     * @since S60 v5.0
     * @param None.
     */
    CInsertionPoint( CAknFepUiLayoutDataMgr* aLafDataMgr );

    /**
     * Two-phrase constructor.
     * 
     * @since S60 v5.0
     * @param None.
     * @return None.
     */
    void ConstructL();

    /**
     * From CActive.
     * The callback function used by CActiveScheduler.
     * 
     * @since S60 v5.0
     * @param None.
     * @return None.
     */
    void RunL();

    /**
     * From CActive.
     * How to cancel me.
     * 
     * @since S60 v5.0
     * @param None.
     * @return None.
     */
    void DoCancel();

    /**
     * From CActive.
     * Handle leaves from RunL().
     * 
     * @since S60 v5.0
     * @param None.
     * @return None.
     */
    TInt RunError( TInt aError );

    /**
     * Start the cursor with glint if actived or with gray color if deactived.
     * 
     * @since S60 v5.0
     * @param aDelay. After this time, the RunL should be called.
     * @return None.
     */
    void StartL( TTimeIntervalMicroSeconds32 aDelay );

private:
    // Data.
    /**
     * The state of CInsertionPoint object.
     */
    TInt iState;
    /**
     * The timer for changing the visibility of this cursor.
     */
    RTimer iTimer;
    /**
     * The rect that visual text must be displayed on.
     */
    TRect iTextRect;
    /**
     * the cursor rect which will be drawn onto the screen.
     */
    TRect iCursorRect;
    /**
     * Whether this cursor should be displayed.
     */
    TBool isVisible;
    /**
     * the length from the beginning of visual text to the cursor position.
     */
    TInt iOffSet;
    /**
     * the height of cursor.
     */
    TInt iHeight;
    /**
     * the BrushColor of active cursor.
     */
    TRgb iActiveBrushColor;
    /**
     * the PenColor of active cursor.
     */
    TRgb iActivePenColor;
    /**
     * the BrushColor of active cursor.
     */
    TRgb iInactiveBrushColor;
    /**
     * the PenColor of active cursor.
     */
    TRgb iInactivePenColor;
    /**
     * the width of cursor.
     */
    TInt iWidth;
    /**
     * Whether this cursor is actived.
     */
    TBool isActive;
    /**
     * The observer for caring of the cursor's change.
     * Not own.
     */
    MRefreshObserver* iRefreshObserver;
	CAknFepUiLayoutDataMgr* iLafDataMgr;
    };

#endif /* INSERTIONPOINT_H */
