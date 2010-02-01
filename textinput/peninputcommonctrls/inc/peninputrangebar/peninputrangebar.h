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
* Description:  Control group of different input ranges
*
*/


#ifndef C_CAKNFEPCTRLRANGEBAR_H
#define C_CAKNFEPCTRLRANGEBAR_H

// system includes
#include <peninputlayoutctrlgroup.h>

// forward declarations
class CFbsBitmap;
class TResourceReader;
class CAknFepCtrlButton;
class CAknFepCtrlCommonButton;
// class declaration
/**
 *  This is the class definition for control group of different
 *  input ranges
 *
 *  @lib fepcommonctrls.lib
 *  @since S60 v3.2
 */
class CAknFepCtrlRangeBar : public CControlGroup , public MEventObserver
    {
public:

    /**
     * Style of range bar, can be horizontal or vertical
     */
    enum TButtonGroupStyle
        {
        EBtnGroupHorizontal,
        EBtnGroupVertical
        }; 

    /**
     * Action style of range bar, can be hide or highlight
    */
    enum TRangeBarActionStyle
        {
        ERangeBarActionStyleHide,     // all indicators action style are unitary: hide
        ERangeBarActionStyleHighLight, // all indicators action style are unitary: highlight
        ERangeBarUsingIndicatorStyle // every indicator can have different action style
        }; 

    /**
     * Action style of indicator, can be hide or visible
     * for using indicaotr action style, every indicator's position is fixed
     */
    enum TIndicatorActionStyle
        {
        EIndicatorActionStyleHide, // hide
        EIndicatorActionStyleHighLight, // visible and highlight
        EIndicatorActionStyleNormal // just like normal button press, highlight not latched
        };

    /**
     * Two-phased constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout An instance of CFepUiLayout
     * @param aUtils An instance of the resource environment
     * @param aControlId The control id of the Range Bar
     * @param aStyle The style of Range Bar, can be vertical or horizontal
     * @return The pointer point to CAknFepCtrlRangeBar type object
     */
    IMPORT_C static CAknFepCtrlRangeBar* NewL(CFepUiLayout* aUiLayout, 
                                              TInt aControlId , 
                                              TButtonGroupStyle aStyle);
    
    /**
     * Two-phased constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout An instance of CFepUiLayout
     * @param aUtils An instance of the resource environment
     * @param aControlId A control id of the Range Bar
     * @param aStyle the style of Range Bar, can be vertical or horizontal     
     * @return The pointer point to CAknFepCtrlRangeBar type object
     */
    IMPORT_C static CAknFepCtrlRangeBar* NewLC(CFepUiLayout* aUiLayout, 
                                               TInt aControlId , 
                                               TButtonGroupStyle aStyle);            

    /**
     * Destructor.
     *
     * @since S60 v3.2
     */
    IMPORT_C virtual ~CAknFepCtrlRangeBar();

    /**
     * This function creates the Range Bar from resource type AKN_FEP_INPUTRANGE_LIST.
     * The function will leave if the count of input ranges is invalid
     *
     * @since S60 v3.2
     * @param aReader A resource reader against AKN_FEP_INPUTMODE_LIST
     * @return None
     */
    IMPORT_C void ConstructFromResourceL(TResourceReader& aReader);

    /**
     * Refresh the curent Mode Control aera
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void RefreshUI();

    /**
     * Handles the SizeChanged event
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void SizeChanged(const TRect aRect);

    /**
     * Set active input range
     *
     * @since S60 v3.2
     * @param aRange A specified input range
     * @return ETrue if given input range set successfully, EFalse otherwise
     */
    IMPORT_C TBool SetRange(const TInt aRange);
        
    /**
     * Set mode of given range
     *
     * @since S60 v3.2
     * @param aRange A specified input range
     * @param aModeIndex The mode index in given range
     * @return None
     */
    IMPORT_C void SetCaseL(const TInt aRange, const TInt aModeIndex);
        
    /**
     * Set permitted input ranges
     *
     * @since S60 v3.2
     * @param aPermitted Used to decide which input ranges permitted
     * @return None
     */
    IMPORT_C void SetPermittedRanges(const TInt aPermitted);

    /**
     * Permit given range or not
     *
     * @since S60 v3.2
     * @param aRangeId The range id of given range
     * @param aPermit Used to enable given range or not
     * @return None
     */
    IMPORT_C void SetPermittedRange(const TInt aRangeId, TBool aPermit);

    /**
     * Permit given range & case or not
     *
     * @since S60 v3.2
     * @param aRangeId The range id of given range
     * @param aRealCaseId The real case id in given range
     * @param aPermit Used to enable given range & case or not
     * @return None
     */
    IMPORT_C void SetPermittedCase(const TInt aRangeId,
                                   const TInt aRealCaseId,
                                   TBool aPermit);

    /**
     * Hide given range, used when whole rangebar's actions tyle is 
     * hide or highlight
     *
     * @since S60 v3.2
     * @param aRange Given input range to be hide
     * @return KErrNone if the button represent given range hided successfully, 
     * otherwise system wide error codes
     */
    IMPORT_C void HideRange(const TInt aRange);

    /**
     * Hide given range, function only used when whole range bar's action
     * style is using indicator style
     *
     * @since S60 v3.2
     * @param aRange Given input range to be hide
     * @return None 
     */
    IMPORT_C void HideRangeUsingIndicatorStyle(const TInt aRange);

    /**
     * Show given range, used when whole rangebar's action style is 
     * hide or highlight
     *
     * @since S60 v3.2
     * @param aRange Given range to be shown
     * @return None
     */
    IMPORT_C void ShowRange(const TInt aRange);

    /**
     * Show given range, function only used when whole range bar's action
     * style is using indicator style
     *
     * @since S60 v3.2
     * @param aRange Given range to be shown
     * @return None
     */
    IMPORT_C void ShowRangeUsingIndicatorStyle(const TInt aRange);
        
    /**
     * Add new input range
     *
     * @since S60 v3.2
     * @param aReader Given resource reader for new input range
     * @return None
     */
    IMPORT_C void AddRangeL(TResourceReader& aReader);        
        
    /**
     * Get current real case of given Range
     *
     * @since S60 v3.2
     * @param aRange Given range
     * @return Current real case of given range
     */
    IMPORT_C TInt GetCase(TInt aRange) const;

    /**
     * Set font of range bar, this function should be called
     * by container of range bar
     *
     * @since S60 v3.2
     * @param aFont The font to be set for range bar
     * @return None
     */
    IMPORT_C void SetFont(const CFont* aFont);

    /**
     * Move range bar according to offset
     *
     * @since S60 v3.2
     * @param aOffset The moving offset
     * @return None
     */
    IMPORT_C void Move(const TPoint& aOffset);

    /**
     * Set event id which will be sent out when range button is click
     *
     * @since S60 v3.2
     * @param aEventId the event id
     * @return None
     */
    IMPORT_C void SetEventIdForRange(TInt aEventId);
    
    /**
     * Set event id which will be sent out when case button is click
     *
     * @since S60 v3.2
     * @param aEventId the event id
     * @return None
     */
    IMPORT_C void SetEventIdForCase(TInt aEventId);

    /**
     * Display all ranges whether it is IsHide
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void ShowAllRanges();
    
    /**
     * DrawRangeBar
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void Draw();

    /**
     * SetActionStyle
     *
     * @since S60 v3.2
     * @param aActionStyle action style
     * @return None
     */
    IMPORT_C void SetActionStyle(TRangeBarActionStyle aActionStyle);

    /**
     * ActiveRange
     *
     * @since S60 v3.2
     * @param aRange range to be activated
     * @return None
     */
    IMPORT_C void ActiveRange(TInt aRange);
        
    /**
     * Get the current input range
     *
     * @since S60 v3.2
     * @return Current Input Range
     */
    inline TInt InputRange() const;    

    /**
     * Set display style of range bar
     *
     * @since S60 v3.2
     * @param aNewStyle The style of range bar, can be vertical or horizontal
     * @return None
     */
    inline void SetStyle(const TButtonGroupStyle aNewStyle);        
        
    /**
     * Get display style of range bar
     *
     * @since S60 v3.2
     * @return Current display style of range bar
     */
    inline TInt Style() const;
 
    /**
     * Set layout rect of range bar, this function should be called
     * by container of range bar
     *
     * @since S60 v3.2
     * @param aRect The layout rect to be set for range bar
     * @return None
     */
    inline void SetCellRect(const TRect aRect);

    /**
     * Set display cell rects
     *
     * @since S60 v3.2 
     * @return None
     */
    IMPORT_C void SetDispalyCellRectsL( const RArray<TRect>& aRects,
    						  			const RArray<TRect>& aInnerRects ); 

    
protected:

    /**
     * From MEventObserver.
     * Handle event
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aCtrl The pointer points to control who sends event
     * @param aEventData The event data
     * @return None
     */
    IMPORT_C void HandleControlEvent(TInt aEventType, CFepUiBaseCtrl* aCtrl, 
                                     const TDesC& aEventData);
        
    /**
     * C++ default constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout An instance of CFepUiLayout
     * @param aControlId The control id of the Range Bar
     * @param aStyle The style of Range Bar, vertical or horizontal
     */
    IMPORT_C CAknFepCtrlRangeBar(CFepUiLayout* aUiLayout, TInt aControlId, 
                                 TButtonGroupStyle aStyle);

    /**
     * By default Symbian 2nd phase constructor is private.
     *
     * @since S60 v3.2
     * @param aUtils An instance of the resource environment
     * @return None
     */
    IMPORT_C void ConstructL();
    
    /**
     * handle pointer down event.
     *
     * @since S60 v3.2
     * @param aPoint 
     * @return control
     */
    virtual CFepUiBaseCtrl* HandlePointerDownEventL(const TPoint& aPoint);
public: // data

    /**
     * Position of the default indicator
     */
    TRect iIndicatorRect;
    
protected: // data

    /**
     * This class declares the range indicator
     * Purpose of this class is to save some information for each indicator
     *
     * @since S60 v3.2
     */  
    class TAknFepRangeIndicator
        {
    public: // data
        /**
         * Control type of button
         */
        TInt iCtrlType;
  
        /**
         * Id of the input range
         */
        TInt iInputRange;    
        
        /**
         * Type of input range, used to judge if it is mixed
         */
        TInt iRangeType;
            
        /**
         * control Id of button
         */
        TInt iCtrlId;
            
        /**
         * Is button that represent iInputRange hided
         */
        TBool iIsHide;
            
        /**
         * Pointer to the indicator button (Own)
         */
        CAknFepCtrlCommonButton* iIndicator;
        
        /**
         * Indicator action style,
         * value from TIndicatorActionStyle
         */
        TInt iActionStyle;

        /**
         * Indicator position index in cellrects list 
         * only meaningful when whole action style is 
         * ERangeBarUsingIndicatorStyle
         */
        TInt iPosIdx;
        }; 

private:

    /**
     * Set layout rect of whole range bar
     *
     * @since S60 v3.2
     * @return None
     */
    void SetWholeRect();
    
    /**
     * Get number of input ranges hided
     *
     * @since S60 v3.2 
     * @return Number of hided input ranges
     */
    TInt NumOfHidedRange();
    
    /**
     * Judge if button in given index has mixed range
     *
     * @since S60 v3.2 
     * @param aIndex Given index
     * @return ETrue if given index has mixed range, otherwise EFalse
     */
    TBool IsMixedRange(const TInt aIndex);
    
    /**
     * Report range or mode change event to observer
     *
     * @since S60 v3.2 
     * @param aEventId the event id
     * @param aEventData the event data
     * @return None
     */
    void ReportEventToObserver(const TInt aEventId, const TInt aEventData);
    
    /**
     * Set new position to sub controls
     *
     * @since S60 v3.2 
     * @return None
     */
    void SetIndicatorRect();
    
    /**
     * update new position to sub controls
     * this function used when indicator's rect get from cell rects list
     * and whole range bar's action style is highlight or hide
     *
     * @since S60 v3.2 
     * @return None
     */
    void UpdateIndicatorRect( TBool aUpdateImage );

    /**
     * update new position to sub controls
     * this function only used when indicator's rect get from cell rects list, 
     * and whole range bar's action style is using indicator style, every 
     * indicator's position is fixed
     *
     * @since S60 v3.2 
     * @return None
     */
    void UpdateIndicatorRectForUsingIndicatorStyle(TBool aUpdateImage);

    /**
     * Get indicator index by given range
     *
     * @since S60 v3.2 
     * @return Indicator index
     */
    TInt IndicatorIdxByRange(TInt aRange);

    /**
     * this function only used when indicator's rect get from cell rects list, 
     * and whole range bar's action style is using indicator style, and indicator
     * to be hiden is using hide action style
     *
     * @since S60 v3.2 
     * @param aIdx Indicator index to be hiden
     * @param aHide Whether hide or show indicator
     * @return None
     */
    void HideRangeForHidingIndicatorStyle(TInt aIdx, TBool aHide);

private: // data

    /**
     * Array of input range indicators
     */
    RPointerArray<TAknFepRangeIndicator> iIndicators;
        
    /**
     * Current active input range
     */
    TInt iInputRange;       
        
    /**
     * A rect used to erase the screen
     */
    TRect iEraseRect;
    
    /**
     * Style of range bar
     */
    TButtonGroupStyle iStyle;

    /**
     * Current font of range bar
     */
    const CFont* iFont;
    
    /**
     * event id for range button click
     */
    TInt iEventIdForRange;
    
    /**
     * event id for case button click
     */
    TInt iEventIdForCase;
    
    /**
     * Whether resize images as well, 
     * if only the position of the button changed, 
     * no need to reset the images as well
     */
    TBool iIsReloadImages;

    /**
     * action style
     */
    TRangeBarActionStyle iActionStyle;
    
    /**
     * First time construct flag
     */
    TBool iFirstTimeConstruct;
    
    /**
     * Rangebar cell rect
     */
    RArray<TRect> iCellRects;
    RArray<TRect> iCellInnerRects;
    TBool iCellChanged;
    };

#include "peninputrangebar.inl"

#endif // C_CAKNFEPCTRLRANGEBAR_H
            
// End Of File
