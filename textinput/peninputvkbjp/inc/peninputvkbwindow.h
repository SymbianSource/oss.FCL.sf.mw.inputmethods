/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface of vkb ui state
*
*/


#ifndef C_PENINPUTVKBWINDOW_H
#define C_PENINPUTVKBWINDOW_H

// system includes
#include <peninputlayoutwindowjp.h>

/** class forward decalaration */

// Forward declarations
class CPeninputVkbCtrlExtJp;
class CAknFepCtrlRangeBar;
class CAknFepCtrlEventButton;

// class declarations
/**
 *  vkb window class
 *
 *  @lib peninputvkbjp.lib
 *  @since S60 v3.2
 */
class CPeninputVkbWindow : public CPeninputLayoutWindowJp
    {
public:

    /**
     * Create one CPeninputVkbWindow object
     *
     * @since S60 v3.2
     * @param aUiLayout The layout plugin
     * @param aControlId This control ID
     * @param aDataProvider The interface of data provider
     * @return The CPeninputVkbWindow object.
     */
    static CPeninputVkbWindow* NewL(CFepUiLayout* aUiLayout,
        MPeninputLayoutContext* aLayoutContext );

    /**
     * Create one CPeninputVkbWindow object
     *
     * @since S60 v3.2
     * @param aUiLayout The layout plugin
     * @param aControlId This control ID
     * @param aDataProvider The interface of data provider
     * @return The CPeninputVkbWindow object.
     */
    static CPeninputVkbWindow* NewLC(CFepUiLayout* aUiLayout,
        MPeninputLayoutContext* aLayoutContext );

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None.
     */
    virtual ~CPeninputVkbWindow();

    /**
     * from CPeninputLayoutWindow
     * Handle event
     * @since Series 60 3.2
     * @param aEventType The event type
     * @param aCtrl The control who sends the event
     * @param aEventData The event data
     * @return None
     */
    void HandleControlEvent(TInt aEventType, CFepUiBaseCtrl* aCtrl, const TDesC& aEventData);

    /**
     * from CPeninputLayoutWindow
     * Get id of window config resource
     *
     * @since S60 v3.2
     * @return The id of window config resource
     */
    virtual TInt GetWindowConfigResId();

    /**
     * from CPeninputLayoutWindow
     * Get id of window resource
     *
     * @since S60 v3.2
     * @return The id of window resource
     */
    virtual TInt GetWindowResId();

    /**
     * from CPeninputLayoutWindow
     * Get file name of window config resource
     *
     * @since S60 v3.2
     * @param aLangID The id of language
     * @return The file name of window config resource
     */
    virtual const TDesC&
        GetWindowConfigResFileName( TInt aLangID );

    /**
     * from CPeninputLayoutWindow
     * Get file name of window resource
     *
     * @since S60 v3.2
     * @return The file name of window resource
     */
    virtual const TDesC& GetWindowResFileName();

    /**
     * from CPeninputLayoutWindow
     * Change unit size
     *
     * @since S60 v3.2
     * @return The rect of whole window
     */
    virtual const TRect ChangeUnitSize();

    /**
     * from CPeninputLayoutWindow
     * Change size of client area
     *
     * @since S60 v3.2
     * @param aLeftTopPoint The left top point of client area
     * @return None
     */
    virtual void ChangeClientSize();

    /**
     * from CPeninputLayoutWindow
     * Create VKB and HWR common controls
     *
     * @since S60 v3.2
     * @return None
     */
    virtual void CreateAllControlsL();

    /**
     * from CPeninputLayoutWindow
     * Pop up choice list
     *
     * @since S60 v3.2
     * @return None
     */
    virtual void PopupChoiceList();

    /**
     * from CPeninputLayoutWindow
     * Re-organize all controls in the assigned client area layout
     *
     * @since S60 v3.2
     * @param aClientLayoutId The id of client area layout
     * @return None
     */
    virtual void ReorganizeControls( TInt aClientLayoutId, TBool aNeedReset );

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
    virtual void DoCaseChange( TInt aNewCase );

    /**
     * update rangebar when range changed
     *
     * @since S60 v3.2
     * @return None
     */
    void DoRangeChange();

    /**
     * Dim a key
     *
     * @since S60 V4.0
     * @param aFlag The dim status
     */
    void SetDimmed(TBool aDimFlag);

    /**
     * From CFepUiBaseCtrl,CControlGroup
     * Hide or shoe control. A hiden control will not handle the event.
     *
     * @since S60 V4.0
     * @param aFlag ETrue if want to hide the control.
     */
    virtual void Hide(TBool aFlag);

    /**
     * Set if enable option button
     *
     * @since S60 v3.2
     * @param aEnable The enable flag
     * @return None
     */ 
    void SetEnableSettingBtnJp(TBool aEnable);

    /**
     *
     * To Call CAknFepCtrlButton::ResizeBitmaps()
     * @since S60 v3.2
     * @return None
     */
    void SetSizeToRangeButtons();

protected:

    /**
     * Construct function
     *
     * @since S60 v3.2
     * @param aUiLayout The layout plugin
     * @param aControlId This control ID
     * @param aDataProvider The interface of data provider
     * @return None.
     */
    CPeninputVkbWindow(CFepUiLayout* aUiLayout,
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
    CAknFepCtrlEventButton* AddButtonL(const TInt aControlId,
                                       const TInt aEventId,
                                       const TInt aResId,
                                       const TInt aUnicode = 0,
                                       const TBool aIsRepeat = EFalse);

    /**
     * Add range bar into client area
     *
     * @since Series 60 3.2
     * @return None
     */
    void AddRangeBarL();

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

private: // data

    /**
     * The vkb extenting control for vkb area
     * Not own
     */
    CPeninputVkbCtrlExtJp* iVkbCtrl;

    /**
     * The range bar
     * Not own
     */
    CAknFepCtrlRangeBar* iRangeBar;

    /**
     * The current japanese
     * Hirakana/Katakana
     */
    TInt iCurrentJapanese;
    };

#endif // C_PENINPUTVKBWINDOW_H

// End Of File
