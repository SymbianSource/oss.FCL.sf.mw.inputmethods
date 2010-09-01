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


#ifndef C_CAKNFEPCTRLMULTIMODEBUTTON_H
#define C_CAKNFEPCTRLMULTIMODEBUTTON_H

// system includes
#include <AknsConstants.h>
#include <peninputlayoutbasecontrol.h>
#include <peninputlayoutbutton.h>
//#include <peninputbutton.h>
#include <peninputcommonbutton.h>

// forward declarations
class TResourceReader;

struct TAknFepModeIndicator
    {
    /**
     * Range id of button
     */
    TInt iRangeId;
  
    /**
     * Real mode(case) of button
     */
    TInt iModeId;    
            
    /**
     * Permitted or not
     */
    TBool iPermitted;
    };

/**
 *  Basic functionality for multi display modes graphic buttons
 *  It provides creation from resource and can react according to SizeChanged Msg
 *
 *  @lib fepcommonctrls.lib
 *  @since Series 60 release 4.0
 */
class CAknFepCtrlMultiModeButton : public CAknFepCtrlCommonButton
    {
public:
        
    /**
     * Two-phased constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout An instance of CFepUiLayout
     * @param aControlId The control id of input range button
     * @return The pointer point to CAknFepCtrlMultiModeButton type object
     */
    IMPORT_C static CAknFepCtrlMultiModeButton* NewL(CFepUiLayout* aUiLayout, TInt aControlId,
                                    TAknsItemID aNormalID = KAknsIIDQsnFrFunctionButtonNormal,
                                    TAknsItemID aPressedID = KAknsIIDQsnFrFunctionButtonPressed,
                                    TAknsItemID aInactiveID = KAknsIIDQsnFrFunctionButtonInactive);
        
    /**
     * Two-phased constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout An instance of CFepUiLayout
     * @param aControlId The control id of input range button
     * @return The pointer point to CAknFepCtrlMultiModeButton type object
     */        
    IMPORT_C static CAknFepCtrlMultiModeButton* NewLC(CFepUiLayout* aUiLayout, TInt aControlId,
                                    TAknsItemID aNormalID = KAknsIIDQsnFrFunctionButtonNormal,
                                    TAknsItemID aPressedID = KAknsIIDQsnFrFunctionButtonPressed,
                                    TAknsItemID aInactiveID = KAknsIIDQsnFrFunctionButtonInactive);
      
    /**
     * Destructor.
     *
     * @since S60 v3.2
     */        
    IMPORT_C virtual ~CAknFepCtrlMultiModeButton();

    /**
     * This function creates the button from resource.
     * The function will leave if the count of display modes is invalid
     *
     * @since S60 v3.2
     * @param aReader A Reader which is created against the AKN_FEP_MULTIPLE_IMAGE_BUTTON resource type
     * @return None
     */
    IMPORT_C void ConstructFromResourceL(TResourceReader& aReader);
    
    /**
     * This function creates button from resource, before calling 
     * this function, caller should set resource id of button first
     * 
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void ConstructFromResourceL();

    /**
     * Add new display mode to button from resource reader
     *
     * @since S60 v3.2
     * @param aReader A Reader which is created against the AKN_FEP_BUTTON_IMAGE resource type
     * @return None
     */
    IMPORT_C void AddModeL(TResourceReader& aReader);

    /**
     * Set button to given display mode
     *
     * @since S60 v3.2
     * @param aType The image type of button, active or nonactive
     * @param aModeIndex The index of given display mode
     * @return None
     */
    IMPORT_C void SetCurrentModeL(const TButtonBmpType aType, const TInt aModeIndex);

    /**
     * Get the mode index of next display mode
     *
     * @since S60 v3.2
     * @return Next display mode of button
     */
    IMPORT_C TInt NextMode() const;

    /**
     * Set font color for text shown in button
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void SetFontColor(const TRgb aFontColor);

    /**
     * Set shadow font color for text shown in button
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void SetShadowFontColor(const TRgb aShadowFontColor);
    
    /**
     * This function handles the SizeChanged event, it resizes all images and rects
     *
     * @since S60 v3.2
     * @param aRect New button extent
     * @param aIsReloadImages Whether resize images as well, 
     * if only the position of the button changed, no need to reset the images as well
     * @return Nothing
     */
    IMPORT_C void SizeChanged(TRect aRect, TRect aInnerRect = TRect(),
                              TBool aIsReloadImages = EFalse);

    /**
     * This function used to set permitted mode(case) in given range
     *
     * @since S60 v3.2
     * @param aRangId Range id
     * @param aRealCaseId The real case id
     * @param aPermit ETrue means permit given range and case, otherwise EFalse
     * @return KErrNone if operation successfully, KErrNotSupported otherwise
     */
    IMPORT_C TInt SetPermittedModes(const TInt aRangeId, const TInt aRealCaseId, TBool aPermit);
    
    /**
     * This function used to set permitted range, all case in given range will be enable or not
     *
     * @since S60 v3.2
     * @param aRangId Range id
     * @param aPermit ETrue means permit given range, otherwise EFalse
     * @return KErrNone if operation successfully, KErrNotSupported otherwise
     */
    IMPORT_C TInt SetPermittedRange(const TInt aRangeId, TBool aPermit);
    
    /**
     * Enable all modes
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void EnableAllModes();
    
    /**
     * Disable all modes
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void DisableAllModes();
    
    IMPORT_C void DisableOneModes(const TInt aRangeId);
    
    /**
     * Judge if given range supported by this button
     *
     * @since S60 v3.2
     * @param aRange Given range id
     * @return ETrue if given range supported, otherwise EFalse
     */
    IMPORT_C TBool IsGivenRangeSupported(const TInt aRange);
    
    /**
     * Set range
     *
     * @since S60 v3.2
     * @param aRange The given range
     * @return KErrNone if operation successfully, otherwise KErrNotSupported
     */
    IMPORT_C TInt SetRange(const TInt aRange);
    
    /**
     * Get current active range
     *
     * @since S60 v3.2
     * @return Current active range if it is permitted, otherwise KErrNotSupported
     */
    IMPORT_C TInt CurrentRange() const;
    
    /**
     * Get range of given mode index
     *
     * @since S60 v3.2
     * @param aModeIndex Index of given mode
     * @return Range of given mode if mode exist, otherwise KErrNotSupported
     */
    IMPORT_C TInt RangeOfGivenMode(TInt aModeIndex) const;
    
    /**
     * Get real case by given mode index
     *
     * @since S60 v3.2
     * @param aModeIndex Index of given mode
     * @return Real case of given mode index if index valid, otherwise KErrNotFound
     */
    IMPORT_C TInt GetRealCaseByMode(TInt aModeIndex) const; 
    
    /**
     * Get range list
     *
     * @since S60 v3.2
     * @param aList Carry the range list on return
     * @return None
     */
    void GetRangeListL(RArray<TInt>& aList);

    /**
     * Get index of current display mode
     *
     * @since S60 v3.2
     * @return Index of current display mode of button
     */
    inline TInt CurrentMode() const;

    /**
     * Get counts of display modes
     *
     * @since S60 v3.2
     * @return Counts of display modes of button
     */
    inline TInt NumberOfModes() const;
    
    /**
     * Set font for text shown in button
     *
     * @since S60 v3.2
     * @param aFont The font to be set for button
     * @return None
     */
    inline void SetFont(const CFont* aFont);

    /**
     * Set font for text shown in button
     *
     * @since S60 v3.2
     * @param aShadowFont The font to be set for shadow text on button
     * @return None
     */
    inline void SetShadowFont(const CFont* aShadowFont);
    
protected:
        
    /**
     * C++ default constructor.
     */
    IMPORT_C CAknFepCtrlMultiModeButton(CFepUiLayout* aUiLayout, 
                                        TInt aControlId,
                                         TAknsItemID aNormalID = KAknsIIDQsnFrFunctionButtonNormal,
                                         TAknsItemID aPressedID = KAknsIIDQsnFrFunctionButtonPressed,
				                         TAknsItemID aInactiveID = KAknsIIDQsnFrFunctionButtonInactive);

    /**
     * Judge if image resource of button already exist
     *
     * @since S60 v3.2
     * @param aResId The image resource id
     * @return ETrue if image resource already exist, EFalse otherwise
     */
    IMPORT_C TBool ImageAlreadyExist(const TInt32 aResId);
    
    /**
     * Set bitmaps for button according to desired display mode
     *
     * @since S60 v3.2
     * @param aType The image type for button, can be active or nonactive etc.
     * @param aMode The desired display mode index
     * @return None
     */
    IMPORT_C void SetButtonBmpL(const TButtonBmpType aType , const TInt aMode);
    
    /**
     * Display text for button according to current display mode
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void DisplayText();
    
    /**
     * Draws UI
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void Draw();
    
    void ResizeBitmaps(TSize aSize);
    
private:

    /**
     * Duplicate bitmap
     *
     * @since S60 v3.2
     * @param aSourceBmp The source bitmap
     * @return Pointer to created bitmap
     */
    CFbsBitmap* DuplicateBitmapL(CFbsBitmap* aSourceBmp);

    /**
     * Transfer bitmap ownship from multimode button to outside.
     *
     * @since S60 v3.2
     * @param aRemoved The bitmap to be transfered
     * @return None
     */
    void TransferOwnership(CFbsBitmap* aRemoved);

private: // data
    /**
     * Array of foreground bitmaps in different modes
     */
     RPointerArray<CFbsBitmap> iForegroundBmpList;
    /**
     * Array of foreground mask bitmaps in different modes
     */
     RPointerArray<CFbsBitmap> iForegroundMaskBmpList;
    /**
     * Froground bitmap's position
     */
    TRect iForgroundBmpRect;
     
    
    /**
     * Array of image resource Id list
     */
    RArray<TInt32> iImageResIdList;
    
    /**
     * The index of current display mode
     */
    TInt iCurrentMode;
    
    /**
     * The counts of display modes of button
     */
    TInt iNumberOfModes;
    
    /**
     * Font of text shown in button
     */
    const CFont* iFont;

    /**
     * Font of shadow text shown in button
     */
    const CFont* iShadowFont;
    
    /**
     * Font color of text shown in button
     */
    TRgb iFontColor;

    /**
     * Shadow Font color of text shown in button
     */
    TRgb iShadowFontColor;

    /**
     * Array of text shown on button
     */
    RPointerArray<HBufC> iModeTextList;

    /**
     * Array of owned bitmap
     */
    //RPointerArray<CFbsBitmap> iOwnedBitmaps;
    
    /**
     * Array of modes
     */
    RPointerArray<TAknFepModeIndicator> iModesList; 

    /**
     * Current bitmap type, active or nonactive
     */
    TButtonBmpType iCurrentBmpType;
    };    

#include "peninputmultimodebutton.inl"

#endif // C_CAKNFEPCTRLMULTIMODEBUTTON_H

// End Of File
