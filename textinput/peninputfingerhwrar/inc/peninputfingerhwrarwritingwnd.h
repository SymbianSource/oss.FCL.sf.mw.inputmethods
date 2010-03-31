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
* Description:  CPeninputArabicFingerHwrWritingWnd
*
*/
#ifndef C_PENINPUTARABICFINGERHWRWRITINGWND_H
#define C_PENINPUTARABICFINGERHWRWRITINGWND_H

#include <peninputlayouthwrwnd.h>


/**
 *  CPeninputArabicFingerHwrWritingWnd
 *
 *  HWR control. This class could be used directly by user.
 *
 *  @lib peninputfingerhwrar.lib
 *  @since Symbian TB9.2
 */
class CPeninputArabicFingerHwrWritingWnd: public CTransparentHwrWndExt
    {
    
public: 
    //constructor and destructor
    /**
     * Destructor.        
     *
     * @since Symbian TB9.2       
     */                  
    virtual ~CPeninputArabicFingerHwrWritingWnd();

    /**
     * Factory function.        
     *
     * @since Symbian TB9.2       
     * @param aRect The rectangle area for this control
     * @param aUiLayout Ui layout who contains this control.Ownership not transferred
     * @param aControlId control Id
     * @return An instance of CButtonBase class        
     */                               
    static CPeninputArabicFingerHwrWritingWnd* NewL( const TRect& aRect,
                                                 CFepUiLayout* aUiLayout,
                                                 TInt aControlId,
                                                 TBool aFullScreenFlag=EFalse, 
                                                 TBool aShowGuideLine=ETrue );
	

                                  
    
	/**
	 * Install Indicator
	 */
	void InstallIndicator(CFepUiBaseCtrl* aIndicator);
    
    /**
     * Draw hwr control
     */	 
	virtual void Draw();               
    
	/**
     * Show indicator or not
     */
    void ShowIndicator(TBool aShowIndicator);

protected:
    /**
     * Default constructor.
     *
     * @since Symbian TB9.2
     * @param aRect The rectangle area for this control
     * @param aUiLayout Ui layout who contains this control.Ownership not transferred
     * @param aControlId control Id
     * @param aFullScreenFlag Fullscreen window flag. ETrue if it's fullscreen window.
     * @param aShowGuideLine Show guide line or not     
     * @return An instance of CPeninputArabicFingerHwrWritingWnd    
     */
    CPeninputArabicFingerHwrWritingWnd( const TRect& aRect, 
                                        CFepUiLayout* aUiLayout,
                                        TInt aControlId, 
                                        TBool aFullScreenFlag, 
                                        TBool aShowGuideLine );	
private:
    /**
	 * The indicator control
	 * Own
	 */
    CFepUiBaseCtrl* iIndicator;
	
	/**
	 * Show indicator or not
	 */
    TBool iIsShowingIndicator;    
    };

#endif // C_CAKNFEPCTRLLABEL_H
