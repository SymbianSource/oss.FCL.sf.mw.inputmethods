/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface of normal button and multidisplay button
*
*/


#ifndef C_CAKNFEPCTRLBUTTON_H
#define C_CAKNFEPCTRLBUTTON_H

// system includes
#include <peninputlayoutbasecontrol.h>
#include <peninputlayoutbutton.h>

// forward declarations
class TResourceReader;

// class declarations
/**
 *  Basic functionality for graphic buttons
 *  It provides creation from resource and can react according to SizeChanged Msg
 *
 *  @lib fepcommonctrls.lib
 *  @since S60 v3.2
 */
class CAknFepCtrlButton : public CButtonBase
    {
public: 
        
    /**
     * Two-phased constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout An instance of CFepUiLayout
     * @param aControlId The control id of input range button
     * @return The pointer point to CAknFepCtrlButton type object
     */
    IMPORT_C static CAknFepCtrlButton* NewL(CFepUiLayout* aUiLayout, TInt aControlId);
        
    /**
     * Two-phased constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout An instance of CFepUiLayout
     * @param aControlId The control id of input range button
     * @return The pointer point to CAknFepCtrlButton type object
     */
    IMPORT_C static CAknFepCtrlButton* NewLC(CFepUiLayout* aUiLayout, TInt aControlId);

    /**
     * Destructor.
     *
     * @since S60 v3.2
     */
    IMPORT_C virtual ~CAknFepCtrlButton();

    /**
     * This function creates the input range button from resource.
     *
     * @since S60 v3.2
     * @param aReader A Reader which is created against the AKN_FEP_BUTTON resource type
     * @return None
     */
    IMPORT_C void ConstructFromResourceL(TResourceReader& aReader);
    
    /**
     * This function creates the input range button from resource,
     * before calling this function, caller should set resource id of 
     * range button first
     * 
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void ConstructFromResourceL();

    /**
     * This function handles the SizeChanged event, it resizes all images and rects
     *
     * @since S60 v3.2
     * @param aRect New button extent
     * @param aIsReloadImages Whether resize images as well, 
     * if only the position of the button changed, no need to reset the images as well
     * @return Nothing
     */
    IMPORT_C void SizeChanged(TRect aRect, TBool aIsReloadImages = EFalse);

    /**
     * Draws UI
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void Draw();

    /**
     * Set active for this button
     *
     * @since S60 v3.2
     * @param aActiveFlag active the button or desactive it
     * @return None
     */
    IMPORT_C void SetActive(TBool aActiveFlag);

    /**
     * Set high light for the button
     *
     * @since S60 v3.2
     * @param aIsHighlight highlight the button or cancel high light it
     * @return None
     */
    IMPORT_C void SetHighlight(TBool aIsHighlight);
    
     /**
     * From CFepUiBaseCtrl
     * Handle pointer levae event. 
     * This will be called only when it's got pointer down event before and 
     * now pointer leaves without pointer up event
     *
     * @since S60 V4.0
     * @param aPoint current pointer position
     */
    virtual void HandlePointerLeave(const TPoint& aPoint);        

protected:

    /**
     * C++ default constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout An instance of CFepUiLayout
     * @param aControlId The control id of input range button
     */
    IMPORT_C CAknFepCtrlButton(CFepUiLayout* aUiLayout, TInt aControlId);
    
    void ResizeBitmaps(TSize aSize);
    
protected:

    /**
     * The first time construct flag of button
     */
    TBool iFirstTimeConstruct;

private:

    /**
     * Array of background bmp (not own)
     */
    RPointerArray<CFbsBitmap> iBmpList;
    
    /**
     * Array of background mask bmp (not own)
     */
    RPointerArray<CFbsBitmap> iMaskBmpList;

    /**
     * The highlight flag of the button
     */
    TBool iHighlight;
    };    

#endif // C_CAKNFEPCTRLBUTTON_H
            
// End Of File
