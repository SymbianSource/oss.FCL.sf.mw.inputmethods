/*
* Copyright (c) 2005-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  header file of pop up window
*
*/

#ifndef C_CFEPLAYOUTPOPUPWND_H
#define C_CFEPLAYOUTPOPUPWND_H

#include <peninputlayoutctrlgroup.h>
/**
 *  CButtonBase
 *
 *  Button base control. This class could be used directly by user.
 *
 *  @lib feplayoutcontrol.lib
 *  @since S60 V4.0
 */
class CFepLayoutPopupWnd : public CControlGroup
    {
public: 
    enum TDisplayPosition
        {
        EDispAutomatic,
        EDispBottomRight,
        EDispBottomLeft,
        EDispTopLeft,
        EDispTopRight
        }; 
    //constructor and destructor
    /**
     * Destructor.        
     *
     * @since S60 V4.0        
     */                  
    IMPORT_C virtual ~CFepLayoutPopupWnd();
    
    /**
     * Show pop up window  
     *
     * @since S60 V4.0        
     * @param aOffset Offset of the pop up window top left position to
     *				  the top left of layout
     */                                  
    IMPORT_C void Display(const TPoint& aOffset);
    /**
     * Show pop up window  
     *
     * @since S60 V4.0        
     * @param aOffsetRect The rect of the pop up window launch 
     */                                  
    IMPORT_C void Display( const TRect& aOffsetRect );

    /**
     * Show pop up window  
     *
     * @since S60 V4.0        
     * @param aOffsetRect The rect of the pop up window launch 
     */                                  
    IMPORT_C void Display( const TRect& aOffsetRect, TDisplayPosition aDisPostion );

    /**
     * Show pop up window  
     *
     * @since S60 V4.0        
     */                                  
    IMPORT_C void CloseWindow();
    
    //from base control CFepUiBaseCtrl
    /**
     * From CFepUiBaseCtrl
     * Set control's rectangle
     *
     * @since S60 V4.0
     * @param aRect The new control area
     */                                       
    IMPORT_C virtual void SetRect(const TRect& aRect);
    
    /**
     * From CFepUiBaseCtrl
     * DeActivate popup window
     *
     * @since S60 V4.0
     */ 
    IMPORT_C virtual void OnDeActivate();
    
    IMPORT_C void ReDrawRect(const TRect& aRect);
protected:
	
    /**
     * constructor.        
     *
     * @since S60 V4.0        
     * @param aSize The control size
     * @param aUiLayout Ui layout who contains this control.Ownership not transferred
     * @param aControlId control Id
     * @return An instance of CButtonBase class        
     */                               
    IMPORT_C CFepLayoutPopupWnd(const TSize& aSize,CFepUiLayout* aUiLayout,
                                                             TInt aControlId);
	
    /**
     * 2nd phase constructor.        
     *
     * @since S60 V4.0        
     */           	
	IMPORT_C void ConstructL(); 
	
	
	TDisplayPosition DeterminePosition(const TRect& aSrcRect);
	IMPORT_C virtual void AfterDisplayed();
private:

    /**
     * Do preparation for pop up window before displaying. 
     * E.g, calc and set the rect
     * It's called internally before displaying.
     *
     * @since S60 V4.0        
     */           	
	IMPORT_C virtual void OnDisplay(); 
private:    
	/*
	 * Flag tells whether the pop up window is shown
	 */
	TBool iIsShowing;
	
	/*
	 * Layout rect before pop up window displaying
	 */
	 TRect iPrevLayoutRect;	 
	 
	/*
	 * Old sprite position
	 */
	 TPoint iPrevLayoutPos;	 	 
	 
	/*
	 * Layout move offset
	 */
	 TPoint iLayoutMoveOff;	 	 
	 
	 
	/*
	 * The control which captures the pointer before
	 */	 
	 CFepUiBaseCtrl* iPreCaptureCtrl;
    /**
     * Reserved item1
     */
     TInt iReserved1;
     
    /**
     * Reserved item2
     */
     TInt iReserved2;	       	 
    };

#endif //C_CFEPLAYOUTPOPUPWND_H