/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  CPeninputArabicFingerHwrIndicator
*
*/
#ifndef C_PENINPUTARABICFINGERHWRINDICATOR_H
#define C_PENINPUTARABICFINGERHWRINDICATOR_H

#include <peninputlayoutbasecontrol.h>
/**
 *  CPeninputArabicFingerHwrIndicator
 *
 *  Label control. This class could be used directly by user.
 *
 *  @lib peninputfingerhwrar.lib
 *  @since S60 V5.0
 */
class CPeninputArabicFingerHwrIndicator: public CFepUiBaseCtrl
    {
    
public: 
    //constructor and destructor
    /**
     * Destructor.        
     *
     * @since S60 V4.0        
     */                  
    virtual ~CPeninputArabicFingerHwrIndicator();

    /**
     * Factory function.        
     *
     * @since S60 V4.0        
     * @param aRect The rectangle area for this control
     * @param aUiLayout Ui layout who contains this control.Ownership not transferred
     * @param aControlId control Id
     * @return An instance of CButtonBase class        
     */                               
    static CPeninputArabicFingerHwrIndicator* NewL(CFepUiLayout* aUiLayout,TInt aControlId);
	
	void ConstructFromResourceL( TInt aResID );

    /**
     * Set bitmap for given status .        
     *
     * @since S60 V4.0        
     * @param aBmp The bitmap. Ownership transferred to button
     * @param aType The bitmap type used for
     */                                 
    void SetBitmapL(CFbsBitmap* aBmp);

    /**
     * Set mask bitmap for given status.        
     *
     * @since S60 V4.0        
     * @param aBmp The bitmap. Ownership transferred to button
     * @param aType The bitmap type used for
     */                                 
    void SetMaskBitmapL(CFbsBitmap* aBmp);

    //from base class CFepUiBaseCtrl
    /**
     * From CFepUiBaseCtrl
     * Draw control
     *
     * @since S60 V4.0
     */                               
    virtual void Draw();    

    /**
     * Label size changed
     *
     * @since S60 v3.2
     * @param aNewRect New rect for label
     * @param aIsReloadImages resize image     
     * @return None
     */
    virtual void SizeChanged(TRect aNewRect, TBool aIsReloadImages);
    
protected:   

    /**
     * Constructor
     *
     * @since S60 V4.0
     * @param aRect The rectangle area for this control
     * @param aUiLayout Ui layout who contains this control.Ownership not transferred
     * @param aControlId control Id
     */           
    CPeninputArabicFingerHwrIndicator(CFepUiLayout* aUiLayout,TInt aControlId);

    /**
     * Bitmaps size changed
     *
     * @since S60 v3.2
     * @param aSize New size for bitmaps
     * @return None
     */
    void ResizeBitmaps(TSize aSize);            

private:    

    /**
     * bitmap for dimmed state
     * Own.
     */
    CFbsBitmap* iBmp;   
         
    /**
     * mask bitmap for dim stute
     * Own.
     */
    CFbsBitmap* iMaskBmp;
    };

#endif // C_CAKNFEPCTRLLABEL_H