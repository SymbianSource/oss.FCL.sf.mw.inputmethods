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
* Description:  header of number mode symbol table.
*
*/


#ifndef C_PENINPUTARABICFINGERHWRNUMSYMBOLTABLE_H
#define C_PENINPUTARABICFINGERHWRNUMSYMBOLTABLE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <e32cmn.h>

#include <peninputlayoutctrlgroup.h>

// FORWARD DECLARATIONS
class CAknFepCtrlEventButton;
class CVirtualKeyboard;

// CLASS DECLARATION

/**
 *  class CPeninputArabicFingerHwrNumSymbolTable.
 *
 *  The symbol table control for arabic input 
 *
 *  @lib peninputfingerhwrar.lib
 *  @since Symbian TB9.2
 */
class CPeninputArabicFingerHwrNumSymbolTable : public CControlGroup
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
    static CPeninputArabicFingerHwrNumSymbolTable* NewL( CFepUiLayout* aUiLayout, TInt aId );

    /**
     * Symbian constructor
     * 
     * @since Symbian TB9.2
     * @param aUiLayout The layout
     * @param aId The control id
     * @return pointer of created object.
     */    
    static CPeninputArabicFingerHwrNumSymbolTable* NewLC( CFepUiLayout* aUiLayout, TInt aId );

    /**
     * standard c++ destructor.
     *
     * @since Symbian TB9.2
     */    
    ~CPeninputArabicFingerHwrNumSymbolTable();
    
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
	void SizeChanged(const TRect& aVirtualKeypadRect);
    
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
	inline CVirtualKeyboard* KeyPad(){return iNumKeypad;}
    
	/**
	 * Construct from resource
	 * This funciton will be called when the resource has changed for example skin was changed
	 */
	void ConstructFromResourceL();	
	
    /**
     * accept editor's number mapping restriction.
     * 
     * @since S60 v5.0
     * @param aNumMapping a string contains numbers and related char. max length is 16.
     * @return none
     */
    void UpdateTableSymbol( const TDesC& aNumMapping );
    
    /*
     * set arabic number mode.
     * @since s60 v5.2
     * @param aIsNativeNumMode a flag to indicate whether it is native number mode.
     * @return none.
     */
    void SetNativeNumMode(const TBool aIsNativeNumMode);

    /**
     * Update the virtual keys rect.
     * @since s60 5.2.
     * @param aCellRects keypad cell rects.
     * @return None.
     */
    void UpdateAllVirtualKeysRect(const RArray<TRect> & aCellRects);
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
    CPeninputArabicFingerHwrNumSymbolTable(CFepUiLayout* aFepUiLayout, TInt aControlId);
    
    /**
     * Symbian second-phase constructor
     *
     * @since Symbian TB9.2
     * @return None
     */    
    void ConstructL();
    
	/**
     * create a virtual keypad
     *
     * @since Symbian TB9.2
     * @return None
     */
	void CreateVirtualKeypadL();
	
	/**
	 * Update the virtual key feedback
	 *
	 */
	void UpdateAllVirtualKeysFeedback();
	
	/**
	 * Draw group control
	 */
	void Draw();

private:
	/**
	 * Refresh the symbol table
	 *
	 */
	void RefreshNumSymbolTable();
		
	/**
	 * Load background from resource
	 *
	 */
	void LoadBackgroundFromResourceL(const TInt aResId);
    
	/*
	 * map latin number to arabic number.
	 */
	TInt16 MapLatinNumAccordingToNumMode(TInt16 aUnicode);
	
	/**
	 * Generate the char talbe according to the aNumMapping
	 *
	 */
	HBufC* GenerateCharTable(const TDesC& aNumMapping);
	
private:
    /**
	 * multipage viritual keyboard
	 * not own.
	 */
	CVirtualKeyboard* iNumKeypad;
    
    /**
     * visibility of popup list.
     */
    TBool iPopupVisible;
	
    /*
     * Default number mode.
     */
    TBool iIsNativeNumMode;
    };

#endif // C_PENINPUTARABICFINGERHWRNUMSYMBOLTABLE_H
