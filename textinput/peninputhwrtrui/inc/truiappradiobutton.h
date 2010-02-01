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
* Description:  radio button - control in Training UI application 
*
*/


#ifndef C_TRUIAPPRADIOBUTTON_H
#define C_TRUIAPPRADIOBUTTON_H

// includes 
#include <coecntrl.h>

// Class declaration
class MTruiRadioButtonObserver;

/**
 *  CTruiRadioButton is a new component for selecting own/preset model
 *  in shortcut edit view.  
 *  @lib avkon.lib
 */ 

class CTruiRadioButton: public CCoeControl, public MCoeControlObserver
    {
public:
    /* Flags for radion button status. */
    enum TRadioButtonStatus
        {
        ENonSelected = 0,
        ESelected
        };

    /* Flags for button type. */
    enum TSelectionType
        {
        ERadioButton = 0,
        ECheckBox        
        };
                
    /**
     * Two-phased constructor.
     * @param aParent the parent control for choice list
     * @param aTextArray the items' text that will be displayed in radion button.
     * @param aStatusArray the selected status for each radio button.
     * @param aSelectionType The type of the selection type, radio or checkbox
     * @return Pointer to CTruiRadioButton's instance
     */
    static CTruiRadioButton* NewL( CCoeControl* aParent,
                              TSelectionType aSelectionType,
                              MTruiRadioButtonObserver* aObserver = NULL );

    /**
     * Two-phased constructor.
     * @param aParent the parent control for choice list
     * @param aTextArray the items' text that will be displayed in radion button.
     * @param aStatusArray The selected status for each radio button.
     * @param aSelectionType The type of the selection type, radio or checkbox
     * @return Pointer to CTruiRadioButton's instance
     */
    static CTruiRadioButton* NewLC( CCoeControl* aParent, 
                              TSelectionType aSelectionType,
                              MTruiRadioButtonObserver* aObserver = NULL );
    /**
     * Destructor
     */
    ~CTruiRadioButton();
    
    /**
     * Convert texts into Label and update them in radio buttons     
     */
    void HandleItemAdditionL();
    
    /**
     * Add one item to radio button
     * @param aText The text of the radio button.
     * @param aStatus The current selected status of the radio button
     */
    void AddItem( const HBufC* aText, TRadioButtonStatus aStatus );
    
    /**
     * Return the status array
     */
    inline const RArray<TRadioButtonStatus>& StatusArray() const
        {
        return iStatusArray;
        };

    /**    
     * Handle pointer on radio button
     *
     * @param aIndex The index of the button group item.
     * @param aEventType The event type.
     */
    void HandlePointerRadioButtonL( TInt aIndex, TCoeEvent aEventType );
    
    /**    
     * Handle pointer on checkbox
     *
     * @param aIndex The index of the button group item.
     * @param aEventType The event type.
     */
    void HandlePointerCheckBox( TInt aIndex, TCoeEvent aEventType );
    
    inline void SetMultilineLayout( const TAknMultiLineTextLayout& aMultilineLayout )
        {
        iMultilineLayout = aMultilineLayout;
        };
        
// from CCoeControl
    /**
     * Return the control's minisize.
     */
	TSize MinimumSize();
    
private:

    /**
     * C++ Default Constructor 
     */
    CTruiRadioButton( MTruiRadioButtonObserver* aObserver, 
                      TSelectionType aSelectionType ); 
    
    /**
     * 2nd phase constructor
     * @param aParent the parent control for choice list
     * @param aTextArray the items' text that will be displayed in radion button.
     * @param aStatusArray the selected status for each radio button.
     */
    void ConstructL( CCoeControl* aParent );
                     
    /**    
     * Create radio button's bitmap.
     */
    void CreateBitmapForRadioButtonL();

    /**
     * Get cached color of skin for label control
     * 
     * @param aColor The color of Label will be set    
     */
    TInt GetCachedLabelTextColor( TRgb& aColor );
    
    /**
     * Load layouts for icons from LAF.
     *
     * @param aVarity Varity for LAF
     */
    void LoadIconsLayout( TInt aVarity );

    /**
     * Convert logical name to visual name for label when needed.
     *
     * @param aLabel The label is used to show text
     * @param aLogicalTxt The logical text which is needed to be converted.
     * @param aFont The font of label text
     * @param aLineWidth The line width of each text of label
     */
    void ConvertToVisualForEdwinL( CEikLabel* aLabel, const TDesC& aLogicalTxt, 
                                   const CFont& aFont, TInt aLineWidth );
    
    /**
     * Clip text of label to fit label width.
     *
     * @param aLabel The label is used to show text
     * @param aIndex The index of text in text array
     */
    void ClipToFitLabelL( CEikLabel* aLabel, TInt aIndex );    
    
// from MCoeControlObserver 
    /**
     * From MCoeControlObserver. Handles an event from an observed control.
     * @param aControl The control that sent the event.    
     * @param aEventType The event type.     
     */
    void HandleControlEventL( CCoeControl* aControl,TCoeEvent aEventType );
    
// from CCoeControl
    /**     
     * Responds to changes to the size and position of the contents of this control.
     */
    void SizeChanged();

    /**
     * Draws control to given area
     * @param aRect The rectangle that should be drawn by the control. 
     */
    void Draw( const TRect& aRect ) const;
    
    /**
     * Gets the number of controls contained in a compound control.
     * @return The number of component controls contained by this control. 
     */
    TInt CountComponentControls() const;

    /**
     * Gets an indexed component of a compound control.
     * @param aIndex The index of the control.
     * @return The component control with an index of aIndex.
     */
	CCoeControl* ComponentControl( TInt aIndex ) const;
	
    /**
     * Gets an indexed component of a compound control.
     * @param aIndex The index of the control.
     * @return The component control with an index of aIndex.
     */
    void HandlePointerEventL( const TPointerEvent& aPointerEvent );
    
    /**
     * From CoeControl.
     * Handles a change to the control's resources.
     *
     * @param aType A message UID value.
     */
    void HandleResourceChange( TInt aType );

private: // data

    /**
     * The control array store all sub-controls     
     * Own
     */
    CArrayPtrFlat<CCoeControl>* iControlArray;
    
    /**
     * The array to store the current status for each radio button
     * Own
     */
    RArray<TRadioButtonStatus> iStatusArray;    
    
    /**
     * The text array to store the text for each radio button
     * Own
     */
    RPointerArray<HBufC> iTextArray;
    
    /**
     * The bitmap for selected radio button.
     * Own
     */    
    CFbsBitmap* iRadioButtonSelectedBmp;

    /**
     * The bitmap mask for selected radio button.
     * Own
     */
    CFbsBitmap* iRadioButtonSelectedBmpm;

    /**
     * The bitmap for non-selected radio button.
     * Own
     */
    CFbsBitmap* iRadioButtonNonSelectedBmp;

    /**
     * The bitmap mask for non-selected radio button.
     * Own
     */
    CFbsBitmap* iRadioButtonNonSelectedBmpm;
    
    /**
     * The observer used to notify the current selected item changed
     * Not own
     */
    MTruiRadioButtonObserver* iObserver;
    
    /**
     * The observer used to notify the current selected item changed
     * Not own
     */
    TSelectionType iSelectionType;
    
    /**
     * Array to store TAknLayoutRect for icons
     * 
     * Own
     */
    RArray<TAknLayoutRect> iIconLayout;
    
    TBool iTactileSupported;        
    
    /**
     * MultilineLayout for edwin
     * 
     */
    TAknMultiLineTextLayout iMultilineLayout;    
    };
        
#endif // C_TRUIAPPRADIOBUTTON_H

