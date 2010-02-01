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
* Description:  header file of latchable button
*
*/

#ifndef C_CFEPLAYOUTLATCHBUTTON_H
#define C_CFEPLAYOUTLATCHBUTTON_H

#include "peninputlayoutbutton.h"
/**
 *  CLatchButton
 *
 *  Latchable button base control. It has two states: latched and unlatched
 *
 *  @lib feplayoutcontrol.lib
 *  @since S60 V4.0
 */
class CFepLayoutLatchButton: public CButtonBase
	{
public: 
    //constructor and destructor
    /**
     * Destructor.        
     *
     * @since S60 V4.0        
     */                  
    IMPORT_C virtual ~CFepLayoutLatchButton();

    /**
     * Factory function.        
     *
     * @since S60 V4.0        
     * @param aRect The rectangle area for this control
     * @param aUiLayout Ui layout who contains this control.Ownership not transferred
     * @param aControlId control Id
     * @return An instance of CButtonBase class        
     */                               
    IMPORT_C static CFepLayoutLatchButton* NewL(const TRect& aRect,
    								CFepUiLayout* aUiLayout,TInt aControlId);
    								
    /**
     * Set bitmap for button latched status
     *
     * @since S60 V4.0        
     * @param aBmp The bitmap for latched button. Ownership transferred to button
     * @param aMaskBmp The mask bitmap. Ownership transferred to button
     */
	IMPORT_C void SetLatchedBitmap(CFbsBitmap* aBmp,CFbsBitmap* aMaskBmp = NULL);

    /**
     * Set bitmap for button unlatched status
     *
     * @since S60 V4.0        
     * @param aBmp The bitmap for unlatched button. Ownership transferred to button
     * @param aMaskBmp The mask bitmap. Ownership transferred to button
     */
	IMPORT_C void SetUnLatchedBitmap(CFbsBitmap* aBmp,CFbsBitmap* aMaskBmp = NULL);

    /**
     * set button latch state
     *
     * @since S60 V4.0        
     */                                                      

	inline void SetLatched(TBool aFlag);

    /**
     * get button latch state
     *
     * @since S60 V4.0        
     * @return The button latch state
     */                                                      	
	inline TBool Latched();
	
    /**
     * From CFepUiBaseCtrl
     * Handle pointer levae event. 
     * This will be called only when it's got pointer down event before and 
     * now pointer leaves without pointer up event
     *
     * @since S60 V4.0
     * @param aPoint current pointer position
     */
    IMPORT_C void HandlePointerLeave(const TPoint& aPoint);
    
    /**
     * From CFepUiBaseCtrl
     * Handle pointer enter event. 
     * This will be called only when it's got pointer down event before and 
     * pointer leaves but now comes back without pointer up event.
     *
     * @since S60 V4.0
     * @param aPoint current pointer position     
     */
    IMPORT_C void HandlePointerEnter(const TPoint& aPoint);            
	
    //from base class CFepUiBaseCtrl
    /**
     * From CFepUiBaseCtrl
     * Draw control
     *
     * @since S60 V4.0
     */                               
    IMPORT_C virtual void Draw();    	
protected:
    /**
     * Constructor
     *
     * @since S60 V4.0
     * @param aRect The rectangle area for this control
     * @param aUiLayout Ui layout who contains this control.Ownership not transferred
     * @param aControlId control Id
     */           
    IMPORT_C CFepLayoutLatchButton(const TRect& aRect,
    
    								CFepUiLayout* aUiLayout,TInt aControlId);

    /**
     * From CFepUiBaseCtrl
     * Handle pointer down event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                                        
    IMPORT_C CFepUiBaseCtrl* HandlePointerDownEventL(const TPoint& aPoint);
    
    /**
     * From CButtonBase
     * Handle pointer up event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                  
    IMPORT_C CFepUiBaseCtrl* HandlePointerUpEventL(const TPoint& aPoint);
    
private:
    /**
     * Change latch state when pen up/down
     *
     * @since S60 V4.0
     */                  
	void ChangeState();   
private:		
	//iPressed is not used in CButtonBase, need removed in the future.
    /**
     * button latch state
     * Own.
     */	
	TBool iLatched;
	
    /**
     * bitmap for latched state
     * Own.
     */
    CFbsBitmap* iLatchedBmp; 
    
    /**
     * mask bitmap for latched state
     * Own.
     */
    CFbsBitmap* iLatchedMaskBmp; 
    
    /**
     * bitmap for unlatched state
     * Own.
     */
    CFbsBitmap* iUnLatchedBmp; 
    
    /**
     * mask bitmap for unlatched state
     * Own.
     */
    CFbsBitmap* iUnLatchedMaskBmp; 		
    
    /**
     * Reserved item1
     */
     TInt iReserved1;
     
    /**
     * Reserved item2
     */
     TInt iReserved2;	           		
	};

#include "peninputlayoutlatchbutton.inl"
#endif //C_CFEPLAYOUTLATCHBUTTON_H
