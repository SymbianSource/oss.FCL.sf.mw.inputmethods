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
* Description:  header file of layout interface
*
*/


#ifndef C_CINSERTIONPOINT_H
#define C_CINSERTIONPOINT_H

#include <w32std.h>
#include <peninputlayoutminterface.h>

class CFepUiBaseCtrl;
class CLayoutTimer;

#ifndef __CURSOR_CONST__
#define __CURSOR_CONST__
/** Cursor blink timer interval */
const TInt KCursorBlinkPerioid = 500000;
/** Stop Cursor blink timer */
/** Cursor width*/
const TInt KCursorWidth = 3;
/** Cursor position offset */
const TInt KCursorPosCorrectionX = -1;
/** Cursor position offset */
const TInt KCursorPosCorrectionY = 0;
#endif //__CURSOR_CONST__

/**
 * Class CInsertionPoint
 *
 * Insertion point i.e. the cursor for edit area of the vkb-control.
 * 
 * @lib feplayouteng.lib
 * @since S60 v4.0
 */
NONSHARABLE_CLASS(CInsertionPoint) : public CBase, MTimerHandler
    {
public:
    // Constructors and destructor
    /**
     * Static constructor.
     *
     * @since S60 v4.0
     * @param aOwner The layout owner
     * @param aHeight A height of the cursor
     * @param aBitmapDevice The bitmap device.
     * @param aGc The graphic context.
     */
    IMPORT_C static CInsertionPoint* NewL(CFepUiBaseCtrl* aOwner, 
                                 CFbsBitmapDevice* aBitmapDevice, 
                                 CBitmapContext* aGc,
                                 TInt aHeight);
    
    /**
     * Destructor.
     *
     * @since S60 v4.0
     */
    virtual ~CInsertionPoint();

    /**
     * Sets the new position for cursor.
     *
     * @since S60 v4.0
     * @param aPosition New position
     */
     IMPORT_C void SetPosition(const TPoint& aPosition);
    
    /**
     * get the cursor position.
     *
     * @since S60 v4.0
     * @return The cursor position
     */
    TPoint Position();    

    /**
     * Turns the cursor on and off
     *
     * @since S60 v4.0
     * @param aOn ETrue -> cursor on, EFalse -> cursor off
     */
    IMPORT_C void SetOn(TBool aOn);

    /**
     * Sets caret height.
     *
     * @since S60 v4.0
     * @param aHeight new caret height in pixels
     */
    IMPORT_C void SetHeight(TInt aHeight);
    
    /**
     * Draw the cursor 
     * Draw the cursor by inverting colors in the cursor rectangle
     *
     * @since S60 v4.0
     * @param aGc Window graphics context to draw the cursor into
     * @param aReset ETrue to set the initial cusor state to invisiable, 
     *        which is used in case editor clear itself before redraw.
     */
     IMPORT_C void Draw(CBitmapContext* aGc, TBool aReset = EFalse);        
    
     inline TBool IsOn();
     
     inline TBool IsVisible();

    /**
     * Turns the cursor shown and off
     *
     * @since S60 v4.0
     * @param aOn ETrue -> cursor shown, EFalse -> cursor hide
     */
    inline void SetVisible(TBool aVisible);
    
    /**
     * Get the height of caret
     *
     * @since S60 v4.0
     * @return Height of caret
     */
    inline TInt Height();

    IMPORT_C void DelayCursorWhileDraging();
private: //Methods
    /**
    * C++ default constructor.
    * 
    * @since S60 v4.0
    * @param aOwner The editor, who is the owner of insertion pointer
    * @param aHeight height of insertionpoint
     * @param aBitmapDevice The bitmap device.
     * @param aGc The graphic context.
     */
    CInsertionPoint(CFepUiBaseCtrl* aOwner,
                    CFbsBitmapDevice* aBitmapDevice,
                    CBitmapContext* aGc,
                    TInt aHeight);

    /**
     * 2nd phase constructor.
     * 
     * @since S60 v4.0
     */
    void ConstructL();
                
    /**
     * Invalidates InsertionPoint's rect
     * 
     * @since S60 v4.0
     */
    void InvalidateInsertionPoint();
    
    /**
     * Timer's callBack function that handles cursor blinking
     * 
     * @since S60 v4.0
     */
    static TInt CursorBlinkCallBack(TAny *aPtr);
   
    //from base class MTimerHandler
    /**
     * HandleTimerOut. Handling time out event
     * @since S60 v4.0
     * @param aTimeType The timer type
     */                
    void HandleTimerOut(TInt /*aTimeType*/);
private:    // Data

    /** 
     * Position of the cursor 
     */
    TPoint iPosition;

    /**
     * Height of the cursor
     */
    TInt iHeight;

    /**
     * Boolean value telling is the cursor on
     */
    TBool iIsOn;     
    
    /**
     * Boolean value telling is the cursor currently visible 
     */
    TBool iIsVisible;         
    
    /**
     * Active object which uses a timer to handle cursor blinking 
     */
    CPeriodic* iCursorBlinkingTimer;  
    
    /**
     * Cursor rect
     */
    TRect iCursorRect;            

    /** 
     * owner of this insertion point
     * Not own
     */
    CFepUiBaseCtrl* iEditor;

    /** 
     * bitmap device for drawing
     * Not own
     */    
    CFbsBitmapDevice* iBitmapDevice;
    
    /** 
     * graphic context
     * Not own
     */    
    CBitmapContext* iGc;
    
    /**
     * The timer to restore the cursor while pen is dragging.
     */
    CLayoutTimer* iRestoreCursorTimer;
    
    //flag tells whether the cursor is temprary disabled
    TBool iCursorTempDisabled;
    };

inline TBool CInsertionPoint::IsOn()
    {
    return iIsOn;
    }
    
inline TBool CInsertionPoint::IsVisible()
    {
    return iIsVisible;
    }

inline void CInsertionPoint::SetVisible(TBool aVisible)
    {
    iIsVisible = aVisible;
    }

inline TInt CInsertionPoint::Height()
    {
    return iHeight;
    }

#endif //C_CINSERTIONPOINT_H

