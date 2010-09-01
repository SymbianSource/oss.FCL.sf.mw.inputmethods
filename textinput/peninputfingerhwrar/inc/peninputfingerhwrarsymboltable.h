/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  header of char range selector.
*
*/


#ifndef C_PENINPUTARABICFINGERHWRSYMBOLTABLE_H
#define C_PENINPUTARABICFINGERHWRSYMBOLTABLE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <e32cmn.h>

#include <peninputlayoutctrlgroup.h>

// FORWARD DECLARATIONS
class CAknFepCtrlEventButton;
class CMultiPageVirtualKeyboard;

// CLASS DECLARATION

/**
 *  class CPeninputArabicFingerHwrSymbolTable.
 *
 *  The symbol table control for arabic input 
 *
 *  @lib peninputfingerhwrar.lib
 *  @since Symbian TB9.2
 */
class CPeninputArabicFingerHwrSymbolTable : public CControlGroup
    {
public:
    /**
     * Symbian constructor
     * 
     * @since Symbian TB9.2
     * @param aUiLayout The layout
     * @param aId The control id
     * @return pointer of created object.
     */
    static CPeninputArabicFingerHwrSymbolTable* NewL( CFepUiLayout* aUiLayout, TInt aId );

    /**
     * Symbian constructor
     * 
     * @since Symbian TB9.2
     * @param aUiLayout The layout
     * @param aId The control id
     * @return pointer of created object.
     */    
    static CPeninputArabicFingerHwrSymbolTable* NewLC( CFepUiLayout* aUiLayout, TInt aId );

    /**
     * standard c++ destructor.
     *
     * @since Symbian TB9.2
     */    
    ~CPeninputArabicFingerHwrSymbolTable();
    
public:
	/**
	 * open the symbol table.
	 * 
	 * @since Symbian TB9.2
	 *
	 * @return None
	 */
	void OpenSymbolTable();
    
    /**
     * cancel the popup.
     * 
     * @since Symbian TB9.2
     * @return None
     * 
     */
    void CloseSymbolTable();
    
    /**
     * get visibility of popup.
     * @since Symbian TB9.2
     * @return ETrue if the popup is visible
     */
    TBool IsPopup();
    
	/**
	 * sizechanged
	 *
	 */
	void SizeChanged(const TRect aVirtualKeypadRect, const RArray<TRect> & aBtnRects,
                     const TInt aKeypadRow, const TInt aKeypadCol, TBool aIsLandscape = ETrue);
    
	/**
	 * Load VKB image
	 *
	 */
    void LoadVkbKeyImageL(TInt aResId, const TSize& aKeySize);
	
	/**
	 * Load virutal Key and set key rects.
	 *
	 */
	void LoadVirtualKeypadKeyL(const TInt aResId, const RArray<TRect>& aCellRects);
    
	/**
	 * Return keypad control
	 *
	 */
	inline CMultiPageVirtualKeyboard* KeyPad(){return iMutiPageKeypad;}
    
	/**
	 * Navigate page
	 *
	 */
	void NavigatePage(TInt aPageNo, TInt aPos);
    
	/**
	 * Construct from resource
	 * This funciton will be called when the resource has changed for example skin was changed
	 */
	void ConstructFromResourceL();	
	
protected: //virtuals from CControlGroup (CFepUiBaseCtrl)

    /**
     * From CControlGroup
     * Handle pointer down event
     *
     * @since Symbian TB9.2
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */   
    CFepUiBaseCtrl* HandlePointerDownEventL(const TPoint& aPoint);
    
    /**
     * From CControlGroup
     * Handle pointer up event
     *
     * @since Symbian TB9.2
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */           
    CFepUiBaseCtrl* HandlePointerUpEventL(const TPoint& aPoint);
    
    /**
     * From CControlGroup
     * Handle pointer move event
     *
     * @since S60 54.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */      
    CFepUiBaseCtrl* HandlePointerMoveEventL(const TPoint& aPoint);
    
    /**
     * C++ constructor
     * 
     * @since Symbian TB9.2
     * @param aFepUiLayout A Ui Layout environment (CFepUiLayout)
     * @param aControlId Control id
     * @return none.
     */
    CPeninputArabicFingerHwrSymbolTable(CFepUiLayout* aFepUiLayout, TInt aControlId);
    
    /**
     * Symbian second-phase constructor
     *
     * @since Symbian TB9.2
     * @return None
     */    
    void ConstructL();
    
	/**
     * create a multipate virtual keypad
     *
     * @since Symbian TB9.2
     * @return None
     */
	void CreateVirtualKeypadL();
	
	/**
	 * create the button control
	 *
	 */
	void CreateButtonGroupL();
	
    /**
     * EventButton creation helper.
     * 
     * @since Symbian TB9.2
     * @param aCtrlId button control id.
     * @param aResId resource id.
     * @param aEvent event id;
     * @param aUnicode a unicode value to be sent
     * @return pointer to created CAknFepCtrlEventButton obj.
     */       
    CAknFepCtrlEventButton* CreateEventBtnL( TInt aCtrlId, TInt32 aResId,
            TInt aEvent = 0xFFFF,TInt aUnicode=0 );
        
    /**
     * EventButton layout helper. Move button to specified rect.
     * 
     * @since Symbian TB9.2
     * @param aButton the button to move
     * @param aRect new rect of button
     * @param aXPadding horz padding of button icon.
     * @param aYPadding vert padding of button icon.
     * @param aReloadImages Specifies whether the images is to be reloaded.
     * @return None
     */     
    void MoveIconButton( CAknFepCtrlEventButton* aButton, const TRect& aRect, 
            TInt aXPadding=0, TInt aYPadding=0, TBool aReloadImages=ETrue );
	
	/**
	 * Show the visible page button
	 *
	 */
	void UpdatePageButtonsUi();
	
	/**
	 * Update the virtual key feedback
	 *
	 */
	void UpdateAllVirtualKeysFeedback();
	
	/**
	 * Draw group control
	 */
	void Draw();
	
	/**
	 * Load background from resource
	 *
	 */
	void LoadBackgroundFromResourceL(const TInt aResId);

private:
    /**
	 * multipage viritual keyboard
	 * not own.
	 */
	CMultiPageVirtualKeyboard* iMutiPageKeypad;
    
    /**
     * Sct page 0 selction button control.
     * Not own.
     */     
    CAknFepCtrlEventButton* i3Page1Btn;
    
    /**
     * Sct page 1 selction button control.
     * Not own.
     */     
    CAknFepCtrlEventButton* i3Page2Btn;
    
    /**
     * Sct page 2 selction button control.
     * Not own.
     */     
    CAknFepCtrlEventButton* i3Page3Btn;
	
	/**
     * Sct page 0 selction button control.
     * Not own.
     */     
    CAknFepCtrlEventButton* i2Page1Btn;
    
    /**
     * Sct page 1 selction button control.
     * Not own.
     */     
    CAknFepCtrlEventButton* i2Page2Btn;
    
    /**
     * visibility of popup list.
     */
    TBool iPopupVisible;
	
	/**
	 * store the layout mode
	 */
	TBool iIsLandscape;
    };

#endif // C_PENINPUTARABICFINGERHWRSYMBOLTABLE_H
