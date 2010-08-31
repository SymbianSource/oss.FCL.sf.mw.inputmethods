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


#ifndef C_CFEPUICURSOR_H
#define C_CFEPUICURSOR_H

#include <w32std.h>
#include <peninputlayoutminterface.h>
#include <peninputlayoutbasecontrol.h>

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
 * Class CFepUiCursor
 *
 * Insertion point i.e. the cursor for edit area of the vkb-control.
 * 
 * @lib feplayouteng.lib
 * @since S60 v4.0
 */
NONSHARABLE_CLASS(CFepUiCursor) : public CFepUiBaseCtrl  , MTimerHandler
    {
public:
    
    /**
     * Static constructor.
     *
     * @since S60 v4.0
     * @param aOwner The layout owner
     * @param aHeight A height of the cursor
     * @param aBitmapDevice The bitmap device.
     * @param aGc The graphic context.
     */
    /*static CFepUiCursor* NewL(CFepUiBaseCtrl* aOwner, 
                                 CFbsBitmapDevice* aBitmapDevice, 
                                 CBitmapContext* aGc,
                                 TInt aHeight);
    
      */
    /**
     * Destructor.
     *
     * @since S60 v4.0
     */
    virtual ~CFepUiCursor();

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
    IMPORT_C TPoint Position();    

    /**
     * Turns the cursor on and off
     *
     * @since S60 v4.0
     * @param aOn ETrue -> cursor on, EFalse -> cursor off
     */
    IMPORT_C void SetOn(TBool aOn, TBool aImmediately = ETrue);

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
     //IMPORT_C void Draw(CBitmapContext* aGc, TBool aReset = EFalse);        
     
    IMPORT_C void ReDraw(TBool aReset = EFalse);             
    //from base class CFepUiBaseCtrl
    /**
     * From CFepUiBaseCtrl
     * Draw control
     *
     * @since S60 V4.0
     */                               
     virtual void Draw();    
     
     
    /**
     * Update control's valid region when other control hiding or displaying.
     * This is usually used for HWR window to change it's writing area.
     *
     * @since S60 V4.0
     * @param aCtrl The control whose displaying status changed
     * @param bFlag ETrue if control is hiden, otherwise EFalse
     */        
    virtual void UpdateValidRegion(CFepUiBaseCtrl* aCtrl,TBool aRemoveFlag);
     
     inline void SetOwner(CFepUiBaseCtrl* aOwner);
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
    
    IMPORT_C void DelayCursorShown();
    
	/**
     * cancel delay process
     */       
    void CancelDelayCursorShown();
	
    //from base class MTimerHandler
    /**
     * HandleTimerOut. Handling time out event
     * @since S60 v4.0
     * @param aTimeType The timer type
     */                
    void HandleTimerOut(TInt /*aTimeType*/);
    
    virtual TInt OrderPos();
    
    inline TRect CursorRect();
    
private: //Methods
    // Constructors and destructor
    
    /**
     * Factory function.        
     *
     * @since S60 V5.0        
     * @param aRect The rectangle area for this control
     * @param aUiLayout Ui layout who contains this control.Ownership not transferred
     * @param aId The control id
     * @return An instance of CFepUiCursor class        
     */                               
    static CFepUiCursor* NewL(const TRect& aRect,CFepUiLayout* aUiLayout,TInt aId);

    /**
    * C++ default constructor.
    * 
    * @since S60 v4.0
    * @param aOwner The editor, who is the owner of insertion pointer
    * @param aHeight height of insertionpoint
     * @param aBitmapDevice The bitmap device.
     * @param aGc The graphic context.
     */
/*    CFepUiCursor(CFepUiBaseCtrl* aOwner,
                    CFbsBitmapDevice* aBitmapDevice,
                    CBitmapContext* aGc,
                    TInt aHeight);
  */
    CFepUiCursor(const TRect& aRect,CFepUiLayout* aUiLayout,TInt aId);


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
    

    void ResetValidClipRegion();
    
    void UpdateCursorArea(const TRect& aRect, TBool aFlag);
    
    TInt OrderPosL();
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
friend class CFepUiLayoutRootCtrl;
    
    };



inline TBool CFepUiCursor::IsOn()
    {
    return iIsOn;
    }
    
inline TBool CFepUiCursor::IsVisible()
    {
    return iIsVisible;
    }

inline void CFepUiCursor::SetVisible(TBool aVisible)
    {
    iIsVisible = aVisible;
    }

inline TInt CFepUiCursor::Height()
    {
    return iHeight;
    }

inline void CFepUiCursor::SetOwner(CFepUiBaseCtrl* aOwner)
    {
    iEditor = aOwner;
    }

inline TRect CFepUiCursor::CursorRect()
    {
    return iCursorRect;
    }
    
#endif //C_CFEPUICURSOR_H

