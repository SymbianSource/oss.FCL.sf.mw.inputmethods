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
* Description:  header file of base class for Edwin-like editing area
*
*/


#ifndef C_CFEPLAYOUTEDITAREABASE_H
#define C_CFEPLAYOUTEDITAREABASE_H

#include <e32std.h>
#include <e32base.h>
#include <w32std.h>
#include <frmtlay.h>
#include <fepbase.h>
#include <fepitfr.h>
#include <eikdialg.h>
#include <eikenv.h>

#include "peninputlayoutbasecontrol.h"
#include <bidivisual.h>

// FORWARD DECLARATIONS
class CInsertionPoint;

const TInt KCursorSelectionGap = 4;

/**
 * Base class for lightweight Edwin-like editing area
 *
 * This is not an actual
 * control and it never actually gains focus, but it simulates it with its
 * own cursor. This is because the FEP must never take focus, because otherwise
 * it will mess up the functionality of the underlying application.
 *
 *  @lib feplayouteng.lib
 *  @since S60 v4.0
 */
class CFepLayoutEditAreaBase : public CFepUiBaseCtrl
    {
public:  
    // Constructors and destructor        
    /**
     * Destructor.
     *
     * @since S60 v4.0
     */
    IMPORT_C virtual ~CFepLayoutEditAreaBase();

    /**
     * Draws the text buffer into the given graphic context
     *
     * @since S60 v4.0
     * @param aGc The graphic context 
     * @param aRect Invalid rectangle. Edit area is redrawn only if 
     * the invalid rectangle intersects with the rectangle of the edit area
     */
     IMPORT_C virtual void DrawContent(CBitmapContext* aGc, const TRect& aRect);

    /**
     * Sets the position, where caret is in the buffer.
     *
     * @since S60 v4.0
     * @param aPositionOfInsertionPointInBuffer The position 
     */
     IMPORT_C void SetPositionOfInsertionPointInBuffer(
                            TInt& aPositionOfInsertionPointInBuffer);
    
    /**
     * Sets the position, where caret is in the buffer,
     * including the start of the selection.
     *
     * @since S60 v4.0
     * @param aPositionOfInsertionPointInBuffer The position  
     */
    void SetPositionOfInsertionPointInBuffer(
                                TInt& aPositionOfInsertionPointInBuffer, 
                                TInt& aPositionOfAnchorInBuffer);


    /**
     * Adjust cursor position when pointer down or move
     *
     * @since S60 v4.0
     * @param aPositionOfInsertionPointInBuffer The position  
     * @return None
     */
    IMPORT_C virtual void AdjustSelectedCompositionText(TInt& aPositionOfInsertionPointInBuffer);
    
    /**
     * Returns the cursor position relative to parent window
     *
     * @since S60 v4.0
     * @return Postition of cursor as pixels
     */
     IMPORT_C TPoint PositionOfInsertionPointOnWindow();

    /**
     * Returns the selected text.
     *
     * @since S60 v4.0
     * @return Selection range of the text
     */
    inline TCursorSelection Selection() const;

    /**
     * Sets the text color.
     *
     * @since S60 v4.0
     * @param aColor RGB-value of the text color
     */
    IMPORT_C void SetTextColor(const TRgb aColor);

    /**
     * Tests if the field has focus. 
     *
     * @since S60 v4.0
     * @return ETrue if it has focus, otherwise EFalse
     */
    IMPORT_C TBool IsFocused();

    IMPORT_C void SetTextAlignmentL(TInt aAlignment);

    /**
     * Sets the text to be displayed
     *
     * @since S60 v4.0
     * @param cursorSel Current cursor selection
     * @param aText The text to be set in fep ui
     * @return None
     */
    IMPORT_C void SetTextL( const TFepInputContextFieldData& aData );
            
    /**
     * Sets dim
     *
     * @since S60 v4.0
     * @param aDimFlag Dim flag, ETrue for dim
     * @return None
     */
    IMPORT_C void SetDimmed(TBool aDimFlag);

    /**
     * Get dim
     *
     * @since S60 v4.0
     * @return ETrue for dim otherwise EFlase
     */
    inline TBool IsDimmed() const;

    /**
     * Sets the cursor 
     *
     * @since S60 v4.0
     * @param cursorSel Current cursor selection
     * @return None
     */
    IMPORT_C void UpdateCursorSelection(const TCursorSelection& aCursorSel);
    
    /**
     * Update content when scrolling 
     *
     * @since S60 v4.0
     * @param cursorSel Current cursor selection
     * @return None
     */
    virtual void UpdateContent(const TCursorSelection& aCursorSel) = 0;

    /**
     * Set the font for text drawing
     *
     * @since S60 v4.0
     * @param aFontSpect font information
     */
    IMPORT_C void SetFont(const TFontSpec& aFontSpec);

    /**
     * Set the font for text drawing
     *
     * @since S60 v4.0
     * @param aFont The font to be used.Ownership not transferred
     */
    IMPORT_C void SetFont(const CFont* aFont);
    
    /**
     * Set editor focus
     *
     * @since S60 v4.0
     * @param aFlag current focus. Default value is ETrue
     */
    IMPORT_C void SetFocus(TBool aFlag = ETrue);    
    
    /**
     * Set cursor visibility
     *
     * @since S60 v4.0
     * @param aFlag ETrue for visisble, EFalse for invisible
     */
    IMPORT_C void SetCursorVisible(TBool aCursorVisibleFlag = ETrue, 
                                   TBool aCursorSelVisibleFlag = EFalse);    
    
    /**
     * Get the text length of buffer
     *
     * @since S60 v4.0
     * @return The buffer length
     */
    inline TInt TextLength() const;    

    /**
     * Get cursor visibility
     *
     * @since S60 v4.0
     * @return ETrue for visisble, EFalse for invisible
     */
    IMPORT_C TBool CursorVisible() const;    

    //from base class CFepUiBaseCtrl
    /**
     * From CFepUiBaseCtrl
     * Draw control
     *
     * @since S60 v4.0
     */                               
    IMPORT_C void Draw();

    /**
     * From CFepUiBaseCtrl
     * Sets the rectangle for the edit area.
     *
     * @since S60 v4.0
     * @param aRect The rectangle
     */
    IMPORT_C void SetRect(TRect aRect);

    /**
     * From CFepUiBaseCtrl
     * Move conntrol
     *
     * @since S60 V4.0
     * @param aOffset The offset to be moved
     */                
    IMPORT_C void Move(const TPoint& aOffset);
    
    /**
     * From CFepUiBaseCtrl
     * Handle pointer down event
     *
     * @since S60 4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                                        
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerDownEventL(const TPoint& aPoint);
    
    /**
     * From CFepUiBaseCtrl
     * Handle pointer move event
     *
     * @since S60 v4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                  
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerMoveEventL(const TPoint& aPoint);
    
    /**
     * From CFepUiBaseCtrl
     * Handle pointer up event
     *
     * @since S60 v4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                  
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerUpEventL(const TPoint& aPoint);
    
    /**
     * From CFepUiBaseCtrl
     * Update rect area
     *
     * @since S60 v4.0
     * @param aRect the rect to be flushed in screen
     * @param aUpdateFlag ETrue if full update.
     */
    //void UpdateArea(const TRect& aRect,TBool aUpdateFlag = EFalse);

    
    /**
     * From CFepUiBaseCtrl
     * Cancel pointer down event.
     * This will be called when a control has pointer down but pointer up 
     * event happens in other ways.
     *
     * @since S60 V4.0
     * @param aFocusedFlag ETrue if got focused, otherwise EFalse
     */          
    IMPORT_C virtual void CancelPointerDownL();
    
    /**
     * From CFepUiBaseCtrl    
     * Handle pointer levae event. 
     * This will be called only when it's got pointer down event before and 
     * now pointer leaves without pointer up event
     *
     * @since S60 V4.0
     * @param aPoint current pointer position
     */
    IMPORT_C virtual void HandlePointerLeave(const TPoint& aPoint);
    
    /**
     * From CFepUiBaseCtrl    
     * Handle pointer enter event. 
     * This will be called only when it's got pointer down event before and 
     * pointer leaves but now comes back without pointer up event.
     *
     * @since S60 V4.0
     * @param aPoint current pointer position     
     */
//    IMPORT_C virtual void HandlePointerEnter(const TPoint& aPoint);    

    /**
     * OnActivate. Called when the layout is going to be shown.
     *
     * @since S60 v4.0
     */        
    IMPORT_C void OnActivate();
    
    /**
     * OnDeActivate. Called when the layout is going to be hidden
     *
     * @since S60 v4.0
     */        
    IMPORT_C void OnDeActivate();
            
    /**
     * From CFepUiBaseCtrl
     * Call back function when layout starts dragging
     *
     * @since S60 V4.0
     */
    IMPORT_C void OnLayoutDraggingStart();

    /**
     * From CFepUiBaseCtrl    
     * Call back function when layout ends dragging
     *
     * @since S60 V4.0
     */
    IMPORT_C void OnLayoutDraggingEnd();

    /**
     * Get the editor max length. 
     * This is the max char length which this editor can holds
     *
     * @since S60 V4.0
     * @return The editor max length
     */
    IMPORT_C virtual TInt GetEditorFieldMaxLen();    
    
    /**
     * Update control's valid region when other control hiding or displaying.
     * This is usually used for HWR window to change it's writing area.
     *
     * @since S60 V4.0
     * @param aCtrl The control whose displaying status changed
     * @param bFlag ETrue if control is hiden, otherwise EFalse
     */        
    IMPORT_C virtual void UpdateValidRegion(CFepUiBaseCtrl* aCtrl,TBool aRemoveFlag);
    
    
    inline void SetTextIsSecret( TBool aIsSecret );
    
    inline TBool TextIsSecret() const;

protected:  // Methods
    
    /**
     * C++ default constructor.
     *
     * @since S60 v4.0
     * @param aRect Control area
     * @param aUiLayout The layout which contains this control
     * @param aControlId The control Id
     */    
    IMPORT_C CFepLayoutEditAreaBase(TRect aRect,CFepUiLayout* aUiLayout,TInt aControlId);
    
    /**
     * By default Symbian 2nd phase constructor is private.
     *
     * @since S60 v4.0
     * @param aVisibleCursor ETrue if visible cursor is required
     */
    IMPORT_C void BaseConstructL();
    
    /**
    * Handles taps, i.e. moves insertion point and selects text.
    * 
    * @since S60 v4.0
    * @param aPointerEvent Information about the pointer event.
    */
    //IMPORT_C virtual void HandlePointerEventL(const TPointerEvent& aPointerEvent);    

    /**
     * Prepares the field for losing focus.
     *
     * @since S60 v4.0
     */
    IMPORT_C virtual void PrepareForFocusLoss();

    /**
     * Prepares the field for gaining focus.
     *
     * @since S60 v4.0
     * @return The operation status. ETrue if successfully.
     */
    IMPORT_C virtual TBool PrepareForFocusGain();    

    /**
     * Returns the position in buffer for given x-coordinate
     *
     * @since S60 v4.0
     * @param aX X-coordinate
     * @return Position in buffer
     */
     IMPORT_C TInt PositionInDisplayText(TInt aX) const;

    /**
     * Calcuate display text
     *
     * @since S60 v4.0
     * @return None
     */
    void CalculateDisplayTextL();
    
    /**
     * Gives coordinates of the character relative to the parent
     * window.
     *
     * @since S60 v4.0
     * @param aCharacterIndex index of the character in iRenderBuffer
     * @return Position as TPoint relative to the parent window, 
     */
    IMPORT_C TPoint PositionInWindow(TInt aCharacterIndex);

    /** 
     * Scrolls selection to the left exposing more text 
     *
     * @since S60 v4.0
     */
    void ScrollLeft();

    /** 
     * Scrolls selection to the right exposing more text 
     *
     * @since S60 v4.0
     */
    void ScrollRight();

private:    // Methods

    /**
     * Draw selected text
     *
     * @since S60 v4.0
     */
    TRect DrawSelection();
    
    void UpdateText( const TFepInputContextFieldData& aData );
    
    static TInt UpdateSecretText( TAny* aEditArea );
    
    void IncreaseBufferL(TInt aLength);

    TInt PositionInVisualText(TInt aX);
    
    TPoint PositionInVisualWindow(TInt aCharacterIndex) const;
    
    TInt CursorDistance(TInt aX) const;
    
    TBidiText::TDirectionality CharDirection( TChar aChar ) const;
    
    void AdjustPostionInVisualText( const TInt cursorDistance, TInt& retVal );
    


    /**
     * Save current cursor state
     *
     * @since S60 v4.0
     * @param aCtrl The control which may affects the cursor shown state
     */    
    void SaveCursorState(CFepUiBaseCtrl* aCtrl);
    
    /**
     * Restore cursor state when overlapped area is removed.
     *
     * @since S60 v4.0
     * @param aCtrl The control which may affects the cursor shown state     
     */    
    void RestoreCursorState(CFepUiBaseCtrl* aCtrl);        

    /**
     * Restore cursor state when overlapped area is removed.
     *
     * @since S60 v4.0
     * @param aIndex The control index in the list which may affects the cursor shown state     
     */    
    void RestoreCursorState(TInt aIndex);        

    /**
     * Find whether the given control is one of the overlapped control.
     *
     * @since S60 v4.0
     * @return The index in the list. KErrNotFound if not fouond
     */    
    TInt FindOverlappedCtrl(CFepUiBaseCtrl* aCtrl);
    
    TInt TextWidth( TInt aStart, TInt aEnd );
    
protected:  // Data

    /**
     * cursor information for the selected txt
     */
    TCursorSelection iSelectedCompositionText;

    /** 
     * Corrected Rect of edit area. 
    *This have to be used when calculating width or does charachters fit 
     */
    TRect iCorrectedRect;

    /** 
     * Display text start position in the original text 
     */
    TInt iDisplayTextOffset;

    /**
     * Color of letters in the keyboard 
     */
    TRgb iTextColor;

    /**
     * The part of iBuffer that is shown in the edit area rectancle 
     */
    TPtrC iDisplayText;
        
    TPtrC iVisualText;
    
    TCursorSelection iDisplayTextCurSel;
        
    /** 
     * Flag to indicate that text is being scrolled 
     */
    TBool iScrolling;

    /** 
     * Flag telling whether the field has focus 
     */
    TBool iHasFocus;

    /**
    * Font spec for text drawing
    */
    TFontSpec iFontSpec;
    
    /**
     * Pointer down flag -- should be removed???
     */
    //TBool iPointerDown;
    
    /**
     * Buffer for text to display
     * Own
     */
    HBufC* iBuffer;
 
    HBufC* iVisualBuffer;
    
    /** 
     * the insertion point, i.e. caret 
     * Own
     */
    CInsertionPoint* iInsertionPoint;
    
    /**
     * Font for text drawing. Ownership depends on iFontOwnership flag.
     */
    CFont* iFont;

    /**
     * The control which captures the pointer before
     */  
    CFepUiBaseCtrl* iPreCaptureCtrl;

    /**
     *  Flag ownership flag. ETrue if CFont object is owned by this control.
     */
    TBool iFontOwnership;
    
    /*
     * Cursor visibility
     */
    TBool iCursorVisible;
    
    /*
     * Cursor selection
     */
    TBool iCursorSelVisible;
    
    /*
     * Editor is dimmed
     */
    TBool iDimmed;
    
    /*
     * Editor is in dragging state
     */
    TBool iInDragging;
    
    CPeriodic* iSecretTextTimer; 
    
    TCursorSelection iUncommittedText;
    
    TInt iRelativePos;
    
    TBool iTextIsSecret;

    TBidiText::TDirectionality iTextDirection;
    
    TBool iMfneEditor;
    
    TBool isCenter;
    
private:
    struct TOverlappedInfo
        {
        /*
         * The control overlapped with ICF cursor
         */
        CFepUiBaseCtrl* iCtrl;
        
        /*
         * keep the cursor on state.
         */            
        TBool iCursorStateBeforeOverlapped;         
        } ;
    
    /**
     * list of controls which overlapped with ICF
     */    
    RArray<TOverlappedInfo> iOverlappedCtrlList;

    /**
     * Reserved item1
     */
     TInt iReserved1;
     
    /**
     * Reserved item2
     */
     TInt iReserved2;    
    };

#include <peninputlayouteditareabase.inl>
#endif      // C_CFEPLAYOUTEDITAREABASE_H   
            
// End of File
