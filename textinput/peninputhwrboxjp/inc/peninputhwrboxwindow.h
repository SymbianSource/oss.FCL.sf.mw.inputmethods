/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  peninput japanese hwr window layout class
*
*/


#ifndef C_PENINPUTHWRBOXWINDOW_H
#define C_PENINPUTHWRBOXWINDOW_H

// System includes
#include <peninputlayoutwindowjp.h>

// Forward declarations
class CFepUiBaseCtrl;
class CAknFepCtrlEventButton;
class CFepCtrlDropdownList;
class CPeninputHwrBoxGroup;

// Constants

/**
 *  Hwr layout window class
 *  This class define methods to deal with window layout and
 *  handle events sent to this window
 *
 *  @lib peninputhwrboxjp.lib
 *  @since S60 v3.2
 */
class CPeninputHwrBoxWindow : public CPeninputLayoutWindowJp
    {

public:

    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aUiLayout The plugin ui layout
     * @param aLayoutContext The hwr layout context
     * @return The pointer to CPeninputHwrBoxWindow object
     */
    static CPeninputHwrBoxWindow* NewL( CFepUiLayout* aUiLayout,
        MPeninputLayoutContext* aLayoutContext );

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CPeninputHwrBoxWindow();

// From base class CPeninputLayoutWindow

    /**
     * From CPeninputLayoutWindow
     * Handle control event
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aCtrl The control who sends the event
     * @param aEventData The event data
     * @return None
     */
    void HandleControlEvent( TInt aEventType,
                             CFepUiBaseCtrl* aCtrl,
                             const TDesC& aEventData );

    /**
     * Handle pointer down event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.
     */
    virtual CFepUiBaseCtrl* HandlePointerDownEventL(const TPoint& aPoint);

    /**
     * From CPeninputLayoutWindow
     * Get id of window config resource
     *
     * @since S60 v3.2
     * @return The id of window config resource
     */
    TInt GetWindowConfigResId();

    /**
     * From CPeninputLayoutWindow
     * Get id of window resource
     *
     * @since S60 v3.2
     * @return The id of window resource
     */
    TInt GetWindowResId();

    /**
     * From CPeninputLayoutWindow
     * Get file name of window config resource
     *
     * @since S60 v3.2
     * @param aLangID The id of language
     * @return The file name of window config resource
     */
    const TDesC& GetWindowConfigResFileName( TInt aLangID );

    /**
     * From CPeninputLayoutWindow
     * Get file name of window resource
     *
     * @since S60 v3.2
     * @return The file name of window resource
     */
    const TDesC& GetWindowResFileName();

    /**
     * From CPeninputLayoutWindow
     * Change unit size
     *
     * @since S60 v3.2
     * @return The window rect
     */
    const TRect ChangeUnitSize();

    /**
     * From CPeninputLayoutWindow
     * Change size of client area
     *
     * @since S60 v3.2
     * @param aLeftTopPoint The left top point of client area
     * @return None
     */
    virtual void ChangeClientSize();

    /**
     * From CPeninputLayoutWindow
     * Construct all controls specified in resource
     *
     * @since S60 v3.2
     * @return None
     */
    virtual void CreateAllControlsL();

    /**
     * Set control fonts. Fonts info comes from resource
     *
     * @since S60 v3.2
     * @return None
     */
    void SetControlsFont();

    /**
     * From CPeninputLayoutWindow
     * Pop up choice list
     *
     * @since S60 v3.2
     * @return None
     */
    void PopupChoiceList();

    /**
     * From CPeninputLayoutWindow
     * Re-organize all controls in the assigned client area layout
     *
     * @since S60 v3.2
     * @param aClientLayoutId The id of client area layout
     * @return None
     */
    void ReorganizeControls( TInt aClientLayoutId, TBool aNeedReset );

    /**
     * From CPeninputLayoutWindowJp
     * Get id of prediction pane scroll up button resource
     *
     * @since S60 v3.2
     * @return The id of prediction pane scroll up button resource
     */
    TInt GetScrollUpBtnResId();

    /**
     * From CPeninputLayoutWindowJp
     * Get id of prediction pane scroll down button resource
     *
     * @since S60 v3.2
     * @return The id of prediction pane scroll down button resource
     */
    TInt GetScrollDownBtnResId();

    /**
     * Do when case changed
     *
     * @since S60 v3.2
     * @param aNewCase The new case
     * @return None
     */
    void DoCaseChange( TInt aNewCase );

    /**
     * Highlight the dropdown list cell
     *
     * @since S60 v3.2
     * @param aIndex The cell index
     * @return None
     */
    void HighlightCell(TInt aIndex);

    /**
     * DropdownListDraw
     *
     * @since S60 v3.2
     * @return None
     */
    void DropdownListDraw();

    /**
     * Cancel wrting
     *
     * @since S60 v3.2
     * @return None
     */
    void CancelWriting();

    /**
     * Cancel repeat
     *
     * @since S60 v3.2
     * @param aFlag The dim flag
     * @return None
     */
    void DimInputContextField(TBool aFlag);

    /**
     * Dim hwrbox
     *
     * @since S60 V4.0
     * @param aFlag The dim status
     */
    void SetDimmed(TBool aDimFlag);

    /**
     * hwrbox control
     *
     * @since S60 V4.0
     */
    CPeninputHwrBoxGroup* BoxGroup();

    /** 
     * ConstructFromResourceL load settings from resource.
     * Before calling this function, the resource id should be set
     *
     * @since S60 v3.2
     * @return None
     */
    void ConstructFromResourceL();

    /**
     * Set if enable option button
     *
     * @since S60 v3.2
     * @param aEnable The enable flag
     * @return None
     */ 
    void SetEnableSettingBtnJp(TBool aEnable);

protected:

    /**
     * Constructor
     *
     * @since S60 v3.2
     * @param aUiLayout The plugin ui layout
     * @param aLayoutContext The hwr layout context
     * @return None
     */
    CPeninputHwrBoxWindow( CFepUiLayout* aUiLayout,
                               MPeninputLayoutContext* aLayoutContext );

private:

    /**
     * Add button to client area
     *
     * @since S60 v3.2
     * @param aControlId The control id
     * @param aEventId The control's event id
     * @param aResId The control's resource id
     * @param aUnicode The control's unicode
     * @param aIsRepeat The TBool indicating whether the
     *        control is repeated or not
     * @return The pointer to CAknFepCtrlEventButton object
     */
    CAknFepCtrlEventButton* AddButtonL( const TInt aControlId,
                                        const TInt aEventId,
                                        const TInt aResId,
                                        const TInt aUnicode = 0,
                                        const TBool aIsRepeat = EFalse );

    /**
     * Add hwr box to client area
     *
     * @since S60 v3.2
     * @return None
     */
    void AddBoxGroupL();

    /**
     * Set frame color of hwr write box
     *
     * @since S60 v3.2
     * @return None
     */
    void SetHwBoxFrameBackColor();

    /**
     * Add dropdown list to window
     *
     * @since S60 v3.2
     * @return None
     */
    void AddDropdownListL();

    /**
     * SetWholeWindowRect
     *
     * @since Series 60 3.2
     * @param aRect The rect of clientpane
     * @return None
     */
    void SetWholeWindowRect(const TRect& aRect);

    /**
     * Add context field into the window
     *
     * @since S60 v3.2
     * @return None
     */
    void AddContextFieldJpL();

private:

    /**
     * The drop down list control
     * Not own
     */
    CFepCtrlDropdownList* iDropdownList;

    /**
     * The handwriting box
     * Not own
     */
    CPeninputHwrBoxGroup* iBoxGroup;

    /**
     * The character range switch button
     * Not own
     */
    CAknFepCtrlEventButton* iCharacterRangeSwitchBtn;

    /**
     * The convert button
     * Not own
     */
    CAknFepCtrlEventButton* iConvertBtn;

    TBool iDisableCaseChange;
    TInt  iCachedCase;
    };

#endif // C_PENINPUTHWRBOXWINDOW_H
