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
 * Description:  peninput generic vkb window layout class
 *
 */

#ifndef C_PENINPUTSPLITQWERTYWINDOW_H
#define C_PENINPUTSPLITQWERTYWINDOW_H

// System includes
#include <peninputlayoutwindowext.h>
#include <peninputuistatemgrinterface.h>
#include <AknLayoutDef.h>

// Forward declarations
class CAknFepCtrlEventButton;
class CAknFepCtrlDragButton;
class CAknFepCtrlRepeatButton;
class CAknFepCtrlCommonButton;
class CPeninputCommonChoiceList;
class CPeninputSplitQwertyLafMgr;
class CPeninputSyncBitmapRotator;
class CRepository;
class CFepLayoutScrollableList;

// Constants
const TInt KMaxFileLength = 80;
const TInt KMaxRangeLabelLength = 50;

/**
 * class TAllRangeLabels
 */
class TAllRangeLabels
    {
public:
    TBuf<KMaxRangeLabelLength> iLabelNativeChar;
    TBuf<KMaxRangeLabelLength> iLabelNativeNumber;
    TBuf<KMaxRangeLabelLength> iLabelLatinChar;
    TBuf<KMaxRangeLabelLength> iLabelLatinNumber;
    TBuf<KMaxRangeLabelLength> iLabelSymbol;
    TBuf<KMaxRangeLabelLength> iLabelAccent;
    };

/**
 * class CPeninputSplitQwertyWindow
 * Main window of IME Split-Qwerty 
 *
 *  @lib peninputsplitqwerty.lib
 *  @since S60 v5.0
 */
class CPeninputSplitQwertyWindow : public CPeninputLayoutWindowExt
    {
public:
    /**
     * Symbian constructor
     *
     * @since S60 v5.0
     * @param aUiLayout The plugin ui layout
     * @param aLayoutContext The vkb layout context
     * @return The pointer to CPeninputSplitQwertyWindow object
     */
    static CPeninputSplitQwertyWindow* NewL( CFepUiLayout* aUiLayout,
                                             MPeninputLayoutContext* aLayoutContext,
                                             MPeninputUiStateMgr* aUiStateMgr );

    /**
     * C++ destructor
     *
     * @since S60 v5.0
     * @return None
     */
    virtual ~CPeninputSplitQwertyWindow();

public: //from base class CPeninputLayoutWindowExt
    /**
     * From CPeninputLayoutWindowExt
     * Get id of window config resource
     *
     * @since S60 v5.0
     * @return The id of window config resource
     */
    TInt GetWindowConfigResId();

    /**
     * From CPeninputLayoutWindowExt
     * Get id of window resource
     *
     * @since S60 v5.0
     * @return The id of window resource
     */
    TInt GetWindowResId();

    /**
     * From CPeninputLayoutWindowExt
     * Get resource id of numeric keymapping
     *
     * @since S60 v5.0
     * @return The id of window resource
     */
    TInt GetNumKeymappingResId();

    /**
     * From CPeninputLayoutWindowExt
     * Get file name of window config resource
     *
     * @since S60 v5.0
     * @param aLangID The id of language
     * @return The file name of window config resource
     */
    const TDesC& GetWindowConfigResFileName( TInt aLangID );

    /**
     * From CPeninputLayoutWindow
     * Get file name of window resource
     *
     * @since S60 v5.0
     * @return The file name of window resource
     */
    const TDesC& GetWindowResFileName();

    /**
     * From CPeninputLayoutWindowExt
     * Change unit size
     *
     * @since S60 v5.0
     * @return The window rect
     */
    const TRect ChangeUnitSize();

    /**
     * From CPeninputLayoutWindowExt
     * Change size of client area
     *
     * @since S60 v5.0
     * @return None
     */
    void ChangeClientSize();

    /**
     * From CPeninputLayoutWindowExt
     * Construct all controls specified in resource
     *
     * @since S60 v5.0
     * @return None
     */
    void CreateAllControlsL();

    /**
     * From CPeninputLayoutWindowExt
     * Set control fonts. Fonts info comes from resource
     *
     * @since S60 v5.0
     * @return None
     */
    void SetControlsFont();

    /**
     * From CPeninputLayoutWindowExt
     * Pop up choice list
     *
     * @since S60 v5.0
     * @return None
     */
    void PopupChoiceList();

    /**
     * From CPeninputLayoutWindowExt
     * Re-organize all controls in the assigned client area layout
     *
     * @since S60 v5.0
     * @param aClientLayoutId The id of client area layout
     * @return None
     */
    void ReorganizeControls( TInt aClientLayoutId, TBool aNeedReset );

    /**
     * From CPeninputLayoutWindowExt
     * Do when case changed
     *
     * @since S60 v5.0
     * @param aNewCase The new case
     * @return None
     */
    void DoCaseChange( TInt aNewCase );

public:
    /**
     * From CPeninputLayoutWindowExt
     * Handle control event 
     *
     * @since S60 v5.0
     * @param aEventType The event type
     * @param aCtrl The control who sends the event
     * @param aEventData The event data
     * @return None
     */
    void HandleControlEvent( TInt aEventType, CFepUiBaseCtrl* aCtrl,
                             const TDesC& aEventData );

public:
    /**
     * ConstructFromResourceL load settings from resource.
     * Before calling this function, the resource id should be set
     *
     * @since S60 v5.0
     * @return None
     */
    void ConstructFromResourceL();

    /**
     * Callback function to do background construction
     * 
     *
     * @since S60 v5.0
     * @return None
     */
    static TInt BackgroundTaskL( TAny* aPtr );

    /**
     * Set current language
     *
     * @since S60 v5.0
     * @param aLang Current language
     * @return None
     */
    void SetSwitchlistLanguage( TInt aLang );

    /**
     * Set current editor is secret
     *
     * @since S60 v5.0
     * @param aSecret Editor is secret
     * @return None
     */
    void SetSwitchlistSecretFlag( TBool aSecret );

    /**
     * Dim the arrow keys on qwerty when the editor is findpane or secret editor
     * 
     * @since S60 v5.0
     * @param aSecret Editor is secret
     * @return None
     */
    void DimArrowKeys( TBool aDimArrow );

    /**
     * Handle accented char event
     *
     * @since S60 v5.0
     * @param aEventType The event type
     * @param aEventData The event data
     * @param aDeadKey The DeadKey char buffer
     * @param anewCharBuf The new Accented buffer
     * @return None
     */
    TBool HandleAccentCharEvent( TInt aEventType, const TDesC& aEventData,
                                 const TDesC& aDeadKey, HBufC* anewCharBuf );

    /**
     * Handle virtual key latched event
     *
     * @since S60 v5.0
     * @param aEventType The event type
     * @param aCtrl The controler
     * @param aEventData Event data
     * @return None
     */
    void HandleVirtualKeyLatchedEvent( TInt aEventType, CFepUiBaseCtrl* aCtrl, 
                                       const TDesC& aEventData );

    /**
     * Handle virtual key up event
     *
     * @since S60 v5.0
     * @param aEventType The event type
     * @param aCtrl The controler
     * @param aEventData Event data
     * @return None
     */
    TBool HandleVirtualKeyUpEvent( TInt aEventType, CFepUiBaseCtrl* aCtrl,
                                   const TDesC& aEventData );

    /**
     * Submit dead key
     *
     * @param abuf Dead key buffer 
     * @return None
     */   
    void SubmitDeadKeyL( const TDesC& abuf );

    /*
     * Set dead key
     *
     * @since S60 v5.0
     * @return None
     */
    void SetDeadKey();

    /**
     * Unlatch a DeadKey by the key buffer
     *
     * @since S60 v5.0
     * @param abuf The key buffer which will be unlatched
     * @return None
     */
    void UnLatchDeadKey( const TDesC& abuf = KNullDesC );

    /**
     * Update laf data
     * 
     * @since S60 v5.0
     * @return None
     */
    void UpdateLafData();

    /*
     * Load accent labels for range list
     *
     * @since S60 v5.0
     * @param aLang current language id
     * @return None
     */
    void ConstructAccentListL( TInt aLangId );

    /*
     * Load range labels for range list
     *
     * @since S60 v5.0
     * @param aLangId current language id
     * @return None
     */
    void ConstructRangeLabelListL( TInt aLangId );

    /**
     * Show or hide preview bubble
     * 
     * @since S60 v5.0
     * @param aShow ETrue if preview bubble is going to be Shown
     * @return None
     */    
    void ShowBubble( TInt aShow );

    /**
     * Update state of range button
     * 
     * @since S60 v5.0
     * @return None
     */
    void UpdateRangeCtrlsL();

    /**
     * Update layout position and size
     * 
     * @since S60 v5.0
     * @return None
     */
    void UpdateLayoutPosAndSize();


protected: //from base class CPeninputLayoutWindowExt
    /**
     * Set laf layout type
     * 
     * @since S60 v5.0
     * @param aRows row count 
     * @param aColumns column count
     * @return None
     */
    virtual void SetLafLayOut( TInt aRows, TInt aColumns );

    /**
     * Create custom control
     * 
     * @since S60 v5.0
     * @return ETrue if success
     */
    virtual TBool CreateCustomControlL( TInt16 aControlId, TInt32 aImageId );

    /**
     * Add custom control to this group
     * 
     * @since S60 v5.0
     * @param aCtrl a control
     * @return None
     */
    virtual void AddCustomControlGroupL( CFepUiBaseCtrl* aCtrl );

    /**
     * Set layout size
     * 
     * @since S60 v5.0
     * @return None
     */
    void SetVkbLayoutSize();

private:
    /**
     * C++ constructor
     *
     * @since S60 v5.0
     * @param aUiLayout The plugin ui layout
     * @param aLayoutContext The vkb layout context
     * @return None
     */
    CPeninputSplitQwertyWindow( CFepUiLayout* aUiLayout,
                                MPeninputLayoutContext* aLayoutContext,
                                MPeninputUiStateMgr* aUiStateMgr );

    /**
     * Symbian constructor
     * 
     * @since S60 v5.0
     * @return None
     */
    void ConstructL();
    
private:
    /**
     * Add range bar into client area
     *
     * @since S60 v5.0
     * @return None
     */
    void AddRangeBarL();

    /**
     * Add button to client area
     *
     * @since S60 v5.0
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
     * Add popup window to display accented char list
     *
     * @since S60 v5.0
     * @return None
     */
    void AddPopupWindowL();

    /**
     * Reorgnize position and size of function buttons
     *
     * @since S60 v5.0
     * @return None
     */
    void ReorgnizeTitleBar();

    /**
     * Show range list
     *
     * @since S60 v5.0
     * @return None
     */
    void PopupRangeListL();

    /**
     * Get resource info of accented chars popup window
     *
     * @since S60 v5.0
     * @return None
     */
    void GetPopupWndInfoFromResL( TResourceReader reader, const TRect& aRect );

    /**
     * Background constructor
     *
     * @since S60 v5.0
     * @return None
     */
    void DoIdleConstructL();

    /**
     * Simulate raw event
     *
     * @since S60 v5.0
     * @param aScanCode scan code of key
     * @param aType event type
     * @return None
     */
    void SimulateRawEvent( TInt aScanCode, TRawEvent::TType aType );

    inline TBool IsRtoLLanguage( TInt aLanguage );
    inline TBool IsValidLanguage( TInt aLanguage );

    /**
     * Handle size changed for range list
     *
     * @since S60 v5.0
     * @return None
     */
    void HandlePopupSizeChange();

    /**
     * Handle dead key event
     *
     * @since S60 v5.0
     * @param aEventType event type
     * @param aEventData event data
     * @return ETrue if event is handled
     */
    TBool HandleDeadKeyL( TInt aEventType, const TDesC& aEventData );

    /**
     * Handle resource of button(base on language direction)
     *
     * @since S60 v5.0
     * @param aControlId id of button control
     * @return None
     */
    void HandleButtonResOnLangDirChange( TInt aControlId );
    void HandleButtonResOnLangDirChangeL( TInt aControlId );

    /**
     * Utils to get an event button
     * 
     * @since S60 v5.0
     * @param aCtrlId id of button control
     * @return pointer to an event button
     */
    inline CAknFepCtrlEventButton* EventButtonCtrl( TInt aCtrlId );
    
    /**
     * Utils to get an common button
     * 
     * @since S60 v5.0
     * @param aCtrlId id of button control
     * @return pointer to an common button
     */
    inline CAknFepCtrlCommonButton* CommonButtonCtrl( TInt aCtrlId );
    
    /**
     * Utils to get context value 
     * 
     * @since S60 v5.0
     * @param aDataIndex index of context value
     * @return conext value
     */
    TInt IntContext( TPeninputDataType aDataIndex );
    
    /**
     * Handle range list event
     *
     * @since S60 v5.0
     * @return None
     */
    void HandleRangeListSelectedL( CFepUiBaseCtrl* aCtrl, const TDesC& aData );

  
    /**
     * Get resource ids of range button icon
     * 
     * @since S60 v5.0
     * @param aLangId current language id
     * @param aAlphaRange variable to receive icon resource id for alpha range
     * @param aOtherRange variable to receive icon resource id for other ranges
     * @return None
     */
    void GetRangeIconResource( TInt aLangId, TInt& aAlphaRange, TInt& aOtherRange  );
    
   
    /**
     * Prepare range list items
     * 
     * @since S60 v5.0
     * @param aRangeItems items in range list
     * @return None
     */
    void PrepareRangeListItems( RArray<CFepLayoutChoiceList::SItem>& aRangeItems );
    
    /**
     * Called when short press occurs on range button.
     *
     * @since S60 v3.2
     * @return None
     */
    void HandleRangeButtonShortPress();
    
private: // Data

    /**
     * The vkb extenting control for vkb area
     * Not own
     */
    CPeninputVkbCtrlExt* iVkbCtrl;

    /**
     * button ctrl: left arrow
     * Not own
     */
    CAknFepCtrlRepeatButton* iArrowLeftButton;

    /**
     * button ctrl: right arrow
     * Not own
     */
    CAknFepCtrlRepeatButton* iArrowRightButton;

    /**
     * button ctrl: range
     * Not own
     */
    CAknFepCtrlCommonButton* iRangeButton;

    /**
     * Range list
     * Not own
     */
    CPeninputCommonChoiceList* iPopupWnd;

    /**
     * The resource file name
     */
    TBuf<KMaxFileLength> iResourceFilename;

    /**
     * The background bitmap of popup window
     * Not own
     */
    CFbsBitmap* iPopupWndBgImg;

    /**
     * The focus bitmap of popup window
     * Not own
     */
    CFbsBitmap* iPopupWndFocusImg;

    /**
     * The UI state manager
     * Not own
     */
    MPeninputUiStateMgr* iUiStateMgr;

    /**
     * Application window rect
     */
    TRect iAppWndRect;

    /**
     * AO for background task
     * Own
     */
    CIdle* iIdle;

    /**
     * Whether popup window has been initied
     */
    TBool iPopupInited;

    /**
     * Popup window item rect
     */
    TRect iPopupItemRect;

    /**
     * Choice text layout
     */
    TAknTextLineLayout iChoiceTextLayout;

    /**
     * Whether popup window text property has been set
     */
    TBool iPopupSet;

    /**
     * Array of unit size
     * Own
     */
    RArray<TSize> iUnits;

    /**
     * Laf data manager
     * Own
     */
    CPeninputSplitQwertyLafMgr* iLafMgr;

    /**
     * The new DeadKey buffer
     */
    TBuf<16> iNewDeadKeyBuf;

    /**
     * The old DeadKey buffer
     */
    TBuf<16> iOldDeadKeyBuf;

    /**
     * Window layout of preview bubble 
     */
    TAknWindowLineLayout iBubbleSize;
    
    /**
     * Text layout of preview bubble 
     */
    TAknTextLineLayout iBubbleTextLayout;
    
    /**
     * Flag for first construction
     */
    TBool iFirstConstruct;

    /**
     * Writing language
     */
    TInt iLanguage;

    /**
     * Bitmap rotator
     * Own
     */
    CPeninputSyncBitmapRotator* iBmpRotator;

    /**
     * Accent commonds for range list
     * Own
     */
    RArray<CFepLayoutChoiceList::SItem> iAccentCmdList;

    /**
     * Range labels for range list
     */
    TAllRangeLabels iRangeLabels;

    /**
     * Indicates whether range button short press is handled.
     */
    TBool iHandleRangeShortPress;
    };

inline TBool CPeninputSplitQwertyWindow::IsRtoLLanguage( TInt aLanguage )
    {
    return ( aLanguage == ELangArabic || aLanguage == ELangHebrew
            || aLanguage == ELangFarsi || aLanguage == ELangUrdu );
    }

inline TBool CPeninputSplitQwertyWindow::IsValidLanguage( TInt aLanguage )
    {
    return ( aLanguage != ELangTest && aLanguage != ELangNone );
    }

inline CAknFepCtrlEventButton* CPeninputSplitQwertyWindow::EventButtonCtrl(
                                                                TInt aCtrlId )
    {
    return static_cast<CAknFepCtrlEventButton*> ( Control( aCtrlId ) );
    }

inline CAknFepCtrlCommonButton* CPeninputSplitQwertyWindow::CommonButtonCtrl(
                                                                TInt aCtrlId )
    {
    return static_cast<CAknFepCtrlCommonButton*> ( Control( aCtrlId ) );
    }

#endif // C_PENINPUTSPLITQWERTYWINDOW_H
