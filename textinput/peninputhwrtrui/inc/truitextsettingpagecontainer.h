/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  CTruiTextSettingPageContainer class of TrainingUI
*
*/


#ifndef C_TRUITEXTSETTINGPAGECONTAINER_H
#define C_TRUITEXTSETTINGPAGECONTAINER_H

#include <akntextsettingpage.h>
#include <AknPreviewPopUpObserver.h>

class CTruiHwrBox;
class CAknPreviewPopUpController;

class CTruiTextSettingPageContainer : public CAknTextSettingPage,
                                      public MAknPreviewPopUpObserver
    {
public:

    /**
     * Two-phased constructor.
     * @param aResourceId Resource Id of CAknTextSettingPage
     * @param aText Reference to text for editing
     * @param aEnableHwrBox Indicate whether to open HWR box to preview model
     *        assigned to the shortcut text
     * @param aTextSettingPageFlags option flags for miscellaneous things
     * @return Pointer to the component control
     */
    static CTruiTextSettingPageContainer* NewL( TInt aResourceId, 
                                                TDes& aText, 
                                                const TRect& aRect,
                                                TBool aEnableHwrBox = EFalse, 
                                                TInt aTextSettingPageFlags = 0 );
    
    /**
     * Two-phased constructor.
     * @param aResourceId Resource Id of CAknTextSettingPage
     * @param aText Reference to text for editing
     * @param aEnableHwrBox Indicate whether to open HWR box to preview model
     *        assigned to the shortcut text
     * @param aTextSettingPageFlags option flags for miscellaneous things
     * @return Pointer to the component control
     */
    static CTruiTextSettingPageContainer* NewLC( TInt aResourceId, 
                                                 TDes& aText, 
                                                 const TRect& aRect,
                                                 TBool aEnableHwrBox = EFalse,
                                                 TInt aTextSettingPageFlags = 0 );

    /**
     * Destructor.
     */
    virtual ~CTruiTextSettingPageContainer();

private:    
    /**
    * Constructor.
	* @param aSettingPageResourceId	Setting Page to use (if present)
	* @param aText						Reference to text for editing
	* @param aTextSettingPageFlags		option flags for miscellaneous things    
    */
    CTruiTextSettingPageContainer( TInt aResourceID, TDes& aText, TInt aTextSettingPageFlags = 0 );

   /**
    * Perform the second phase construction of a CTruiMainViewContainer object.
    *
    * @param aEnableHwrBox Indicate whether to open HWR box to preview model
    *        assigned to the shortcut text    
    */
    void ConstructL( const TRect& aRect, TBool aEnableHwrBox = EFalse );      
    
    /**
     * From CoeControl.
     * Responds to changes to the size and position of the contents of this control.
     */
    void SizeChanged();
    
    /**
     * From CAknSettingPage
     * Processes events from the softkeys.     
     *
     * @param	aCommandId	Event Id from the soft-key
     */
	void ProcessCommandL( TInt aCommandId );
	
// from MAknPreviewPopUpObserver
    /**    
    * Called by the preview popup when an appropriate event takes place. 
    * @param aController Pointer to the sender of the event.
    * @param aEvent Event type.
    */
    void HandlePreviewPopUpEventL(
          CAknPreviewPopUpController* aController, TPreviewPopUpEvent aEvent );

private: // data    
    /**
     * Pointer to HWR box.
     * Own
     */
    CTruiHwrBox* iHwrBox;
    
    /**
     * Pointer to preview popup used to show tooltip
     * Own
     */
    CAknPreviewPopUpController* iPopupController;
    };
    
#endif // C_TRUITEXTSETTINGPAGECONTAINER_H