/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies).
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
class CPeninputSyncBitmapRotator;

// CLASS DECLARATION

/**
 *  class CPeninputArabicFingerHwrSymbolTable.
 *
 *  The symbol table control for arabic input 
 *
 *  @lib peninputfingerhwr.lib
 *  @since S60 v5.0
 */
class CPeninputArabicFingerHwrSymbolTable : public CControlGroup
    {
public:
    /**
     * Symbian constructor
     * 
     * @since S60 v5.0
     * @param aUiLayout The layout
     * @param aId The control id
     * @return pointer of created object.
     */
    static CPeninputArabicFingerHwrSymbolTable* NewL( CFepUiLayout* aUiLayout, TInt aId );

    /**
     * Symbian constructor
     * 
     * @since S60 v5.0
     * @param aUiLayout The layout
     * @param aId The control id
     * @return pointer of created object.
     */    
    static CPeninputArabicFingerHwrSymbolTable* NewLC( CFepUiLayout* aUiLayout, TInt aId );

    /**
     * standard c++ destructor.
     *
     * @since S60 v5.0
     */    
    ~CPeninputArabicFingerHwrSymbolTable();
    
public:
	/**
	 * open the symbol table.
	 * 
	 * @since S60 v5.0
	 *
	 * @return None
	 */
	void OpenSymbolTable();
    
    /**
     * cancel the popup.
     * 
     * @since S60 v5.0
     * @return None
     * 
     */
    void CloseSymbolTable();
    
    /**
     * get visibility of popup.
     * @since S60 v5.0
     * @return ETrue if the popup is visible
     */
    TBool IsPopup();
    
	/**
	 * sizechanged
	 *
	 */
	void SizeChanged(const TSize aKeypadSize, const TSize aButtonSize,
                     const TInt aKeypadRow, const TInt aKeypadCol, TBool aIsLandscape = ETrue);
    
    void LoadVkbKeyImageL(TInt aResId, const TSize& aKeySize);
	
	void LoadVirtualKeypadKeyL(const TInt aResId, const RArray<TRect>& aCellRects);
    
	inline CMultiPageVirtualKeyboard* KeyPad(){return iMutiPageKeypad;}
    
	void NavigatePage(TInt aPageNo, TInt aPos);
	
	void UpdatePageButtonsUi();
	
	void UpdateAllVirtualKeysFeedback(TInt aType);

protected: //virtuals from CControlGroup (CFepUiBaseCtrl)

    /**
     * From CControlGroup
     * Handle pointer down event
     *
     * @since S60 V5.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */   
    CFepUiBaseCtrl* HandlePointerDownEventL(const TPoint& aPoint);
    
    /**
     * From CControlGroup
     * Handle pointer up event
     *
     * @since S60 V5.0
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
     * Handle pointer levae event. 
     * This will be called only when it's got pointer down event before and 
     * now pointer leaves without pointer up event
     *
     * @since S60 V4.0
     * @param aPoint current pointer position
     */
    void HandlePointerLeave(const TPoint& aPoint);
    
    /**
     * C++ constructor
     * 
     * @since S60 v5.0
     * @param aFepUiLayout A Ui Layout environment (CFepUiLayout)
     * @param aControlId Control id
     * @return none.
     */
    CPeninputArabicFingerHwrSymbolTable(CFepUiLayout* aFepUiLayout, TInt aControlId);
    
    /**
     * Symbian second-phase constructor
     *
     * @since S60 v5.0
     * @return None
     */    
    void ConstructL();
    
	/**
     * create a multipate virtual keypad
     *
     * @since S60 v5.0
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
     * @since S60 v5.0
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
     * @since S60 v5.0
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
	 * create a new virtual key
	 *
	 */
	CVirtualKey* CreateVkbKeyL(TResourceReader& aReader, const TRect aKeyRect);
	
	/**
	 * update the virtal key
	 *
	 */
	void UpdateVkbKeyL( CVirtualKey* aVirtualKey, TResourceReader& aReader, 
	                    const TRect aKeyRect );
    void OnActivate();
private:
    /**
	 * multipage viritual keyboard
	 * not own.
	 */
	CMultiPageVirtualKeyboard* iMutiPageKeypad;
    
	/**
     * Enter button.
     * not own.
     */
    CAknFepCtrlEventButton* iEnterBtn;
    
    /**
     * space button.
     * not own.
     */
    CAknFepCtrlEventButton* iSpaceBtn;
    
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
	
	/**
	 * remember the pointer down contrl
	 */
	CFepUiBaseCtrl* iPenDownCtrl;
	
	/**
	 * check if the the pointer is moved out
	 */
	TBool iPointerMoveBack;
    };

#endif // C_PENINPUTARABICFINGERHWRSYMBOLTABLE_H
