/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of mutli-page virtual keyboard control.
*
*/


#ifndef C_FINGERHWRARMULTIPAGEVIRTUALKEYBOARD_H
#define C_FINGERHWRARMULTIPAGEVIRTUALKEYBOARD_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <peninputlayoutvkb.h>

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
 *  class CMultiPageVirtualKeyboard.
 * 
 *  multi-page virtual keybord.
 * 
 *  @lib peninputfingerhwrar.lib
 *  @since S60 v5.0
 */

class CMultiPageVirtualKeyboard : public CVirtualKeyboard
    {
public:

    /** 
     * Symbian constructor.
     *
     * @since S60 V5.0     
     * @param aRect The keyboard position relative to layout
     * @param aUiLayout The layout contains the keyboard. Owner ship not transferred.
     * @param aControlId Id for this keyboard control.
     * @param aFontSpec The font used for key drawing.
     * @return Pointer of created object.
     */    

    static CMultiPageVirtualKeyboard* NewL( const TRect& aRect, 
            CFepUiLayout* aUiLayout, TInt aControlId, 
            const TFontSpec& aFontSpec, TBool aIrregular = EFalse );
    /**
     * Symbian constructor.
     *
     * @since S60 V5.0     
     * @param aRect The keyboard position relative to layout
     * @param aUiLayout The layout contains the keyboard. Owner ship not transferred.
     * @param aControlId Id for this keyboard control
     * @param aFont The font used for key drawing.
     * @return Pointer of created object.
     */    
    static CMultiPageVirtualKeyboard* NewL( const TRect& aRect, 
            CFepUiLayout* aUiLayout,TInt aControlId,
            CFont* aFont, TBool aIrregular = EFalse );
    

    /**
     * standard c++ destructor.
     *
     * @since S60 v5.0
     */    
    ~CMultiPageVirtualKeyboard();
    
    
    /**
     * get current page index.
     * 
     * @since S60 v5.0
     * @return page index.
     */
    TInt CurPageIndex();
    
    /**
     * get page count.
     * 
     * @since S60 v5.0
     * @return page count.
     */    
    TInt PageCount();
    
    /**
     * The page size
     * 
     */
    TInt PageSize();
    
    /**
     * move to next page.
     * 
     * @since S60 v5.0
     * @return none.
     */      
    void NavToNextPage();
   
    /**
     * move to previous page.
     * 
     * @since S60 v5.0
     * @return none.
     */     
    void NavToPrevPage();
    
    /**
     * move to first page.
     * 
     * @since S60 v5.0
     * @return none.
     */     
    void NavToFirstPage();
    
    /**
     * move to last page.
     * 
     * @since S60 v5.0
     * @return none.
     */ 
    void NavToLastPage();
    
    /**
     * move to specified page.
     * 
     * @since S60 v5.0
     * @param aPageIndex page index.
     * @return none.
     */ 
    void NavToPage( TInt aPageIndex );

    /**
     * update paging.
     * 
     * @since S60 v5.0
     * @param aRows Specifies row count.
     * @param aCols Specifies column count.
     * @return none.
     */     
    void UpdatePaging( TInt aRows, TInt aCols );
    
private:

    /**
     * Stand C++ Constructor
     * 
     * @since S60 V5.0     
     * @param aRect The keyboard position relative to layout
     * @param aUiLayout The layout contains the keyboard. Owner ship not transferred.
     * @param aControlId Id for this keyboard control
     * @param aFontSpect The font used for key drawing
     */
    CMultiPageVirtualKeyboard(const TRect& aRect,CFepUiLayout* aUiLayout,
            TInt aControlId, const TFontSpec& aFontSpec, 
            TBool aIrregular = EFalse);

    /**
     * Symbian 2nd phase constructor
     * 
     * @since S60 V5.0 
     */
    void ConstructL();
    
private: //datas
    /**
	 * The current page index
	 */
    TInt iCurPageIndex;
	
	/**
	 * The page size
	 */
    TInt iPageSize;
	
	/**
	 * The page count
	 */
    TInt iPageCount;
    
    };

#endif // C_FINGERHWRARMULTIPAGEVIRTUALKEYBOARD_H

//  End Of File
