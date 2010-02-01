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
* Description:  hwr layout
*
*/


#ifndef C_CPENINPUTHWRBOXLAYOUT_H
#define C_CPENINPUTHWRBOXLAYOUT_H

//  System includes
#include <peninputcommonlayout.h>

/** class forward decalaration */
class CPeninputHwrBoxWindow;
class CPeninputHiraganaKanji;
class CPeninputHwrBoxDataMgr;

#ifdef __PENINPUT_HWR_STROKE_LOG_ON
#include <flogger.h>
#endif

/**
 *  peninputhwrboxjp layout
 *
 *  @lib peninputhwrboxjp.lib
 *  @since S60 v3.2
 */
class CPeninputHwrBoxLayout : public CPeninputCommonLayout
    {

public:

    /**
     * Symbian constructor.
     *
     * @since S60 v3.2
     * @param aLafEnv The LAF enviroment
     * @param aInitData Init data for layout
     * @return Pointer to created CPeninputHwrBoxLayout object
     */
    static CPeninputHwrBoxLayout* NewL(MLayoutOwner* aLayoutOwner, const TAny* aInitData);

    /**
     * destructor.
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CPeninputHwrBoxLayout();

    /**
     * From MFepLayoutBase
     * HandleEventL. Handle UI event.
     * There are only 3 kinds of UI event right now: raw event,
     * pointer and pointer buffer event.
     *
     * @since S60 v4.0
     * @param aType The event type. See TEventType
     * @param aEventData The event data.
     * @return The control which processes the event
     */
    TBool HandleEventL(TEventType aType, const TAny* aEventData);

//from CFepUiLayout

    /**
     * From CFepUiLayout.
     * Handle layout command event
     *
     * @since S60 v3.2
     * @param aCmd Command Id.
     * @param aData Data for command.
     * @return Errors when return value small than 0.
     */
    TInt HandleCommand(TInt aCmd, TUint8* aData);

    /**
     * From CFepUiLayout.
     * Handle the event from control
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aCtrl  The sender
     * @param aEventData The event data.
     * @return None
     */
    void HandleControlEvent(TInt aEventType, CFepUiBaseCtrl* aCtrl, const TDesC& aEventData);

    /**
     * Handle the event for Pen from control
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aCtrl  The sender
     * @param aEventData The event data.
     * @return
     */
    TBool HandleControlEventPen(TInt aEventType, CFepUiBaseCtrl* aCtrl, const TDesC& aEventData);

    /**
     * From CFepUiLayout.
     * Handle editor text coming
     * Called by animation dll when app editor text is coming.
     * If a ui layout has a editor area, it must implements this function to get the text
     *
     * @since S60 v3.2
     * @param aTotalTextLen The total editor text len
     * @param aTextStartPos The position of the selection in whole editor text
     * @param aCursolSel The cursor selection in the editor
     * @param aText The text sent by fep.
     * @return KErrNone is operation succeeded.
     */
    TInt OnAppEditorTextComing( const TFepInputContextFieldData& aData);

    /**
     * Submit the candidate.
     *
     * @since S60 v3.2
     * @param aCharCode The char code
     * @return None
     */
    void SubmitL(const TDesC& aCharCode);

    /**
     * Delete the candidate.
     *
     * @since S60 v3.2
     * @param aChar The delete key code
     * @return None
     */
    void DeleteL(TInt aChar = EKeyBackspace);

    /**
     * From CFepUiLayout.
     * Handle raw event key
     *
     * @since S60 v3.2
     * @param aData the key raw event data
     * @return ETrue if the key was handled, otherwise EFalse
     */
    TBool HandleRawKeyEventL(const TRawEvent& aKeyEvent);

    /**
     * From CFepUiLayout.
     * Handle pointer down/up event while this event is outside of the layout
     * Do nothing in default implementation
     *
     * @since S60 v4.0
     * @param aEvent The raw event
     * @return EFalse if the event needs forward on. Otherwise ETrue.
     */
    TBool OnPointerEventOutsideLayout(const TRawEvent& aEvent);

    /**
     * Get DataMgr Pointer
     *
     * @since S60 v3.2
     * @return DataMgr Pointer
     */
    CPeninputHwrBoxDataMgr* DataMgr(TInt aDummy);

    /**
     * Get  HwrBox Window Pointer
     *
     * @since S60 v3.2
     * @return HwrBox Window Pointer
     */
    CPeninputHwrBoxWindow* HwrBoxWindow();

    void ToPenStandBy(TBool aNotStandBy = EFalse);

    void Initialize();

    void CompleteIfConvertingL();

    /**
     * clear last input char and goto PenStandBy
     *
     * @since S60 v3.2
     * @return None
     */
    void ClearInputToPenStandByL();

#ifdef __PENINPUT_HWR_STROKE_LOG_ON
    /**
     * write log "New"
     *
     * @since S60 V4.0
     */
    void WriteHwrStrokeLogNew();
#endif

private:

    /**
     * C++ default constructor
     *
     * @since S60 v3.2
     * @param aLafEnv The LAF enviroment
     * @return None
     */
    CPeninputHwrBoxLayout(MLayoutOwner* aLayoutOwner);

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v3.2
     * @param aInitData Init data of layout
     * @return None
     */
    void ConstructL(const TAny* aInitData);

    /**
     * Set current range
     *
     * @since S60 v3.2
     * @return None.
     */
    void DoSetCurrentRange();

    /**
     * CompleteAll HiraganaKanji
     *
     * @since S60 v3.2
     * @return None.
     */
    void HiraganaKanjiCompleteAll();

// From base class CPeninputCommonLayout

    /**
     * From CPeninputCommonLayout
     * Factory method for creation of layout window. The decision which
     * layout window(VKB or HWR) is created will be postboned in subclasses
     * of this class
     *
     * @since S60 v3.2
     * @return None
     */
    void CreateLayoutWindowL();

    /**
     * From CPeninputCommonLayout
     * Factory method for creation resource config. It is enough at present
     * to create and initialize resource config. Any necessary extension
     * will be implemented in subclasses
     *
     * @since S60 v3.2
     * @return None
     */
    void CreateDataMgrL( const TAny* aInitData );


    /**
     * From CPeninputCommonLayout
     * Get the layout type, VKB or HWR, child class must rewrite the function
     *
     * @since S60 v3.2
     * @return The layout type
     */
     TInt LayoutType();

    /**
     * SemiTransparencyRequired. Tell whether this layout require transparency.
     * This is to save memory used in mask bitmap. If transparency not deeded,
     * We use black-white bitmap.
     *
     * @since S60 v3.2
     * @return ETrue if layout require transparency, otherwise EFalse.
     */
    TBool SemiTransparencyRequired();

    /**
     * Translate range by fullhalf
     *
     * @since S60 v3.2
     * @param aRange The range need to set
     * @return New range
     */
    TInt TranslateRangeHullHalf(TInt aRange) const;

    // async
    static TInt KeyEventCallback(TAny* aThis);
    void DoKeyEventL();
    void DoPointerEvent();
    static TInt KeyEventClearCallback(TAny* aThis);
    static TInt PointerEventCallback(TAny* aThis);
    void DoKeyClearEventL();

    /**
     * PointerEvent Capture or not.
     *
     * @since S60 V4.0
     * @param aFlag ETrue if want to hide the control.
     */
    void SetCapturePointerEvent(TBool aFlag);

    /**
     * After Capture is rereased by fremawork, Capture On again
     *
     * @since S60 V4.0
     */
    void SetCapturePointerEventAgain();

    /**
     * HiraganaHandleControlEventJp
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aEventData The event data.
     * @return None
     */
    void HiraganaHandleControlEventJp(TInt aEventType, const TDesC& aEventData);

    /**
     * check if EStatusPredictiveTransitoryInputWithNoChar
     *
     * @since S60 v3.2
     * @return ETrue if EStatusPredictiveTransitoryInputWithNoChar, otherwise EFalse
     */
    TBool IsPredictiveWithNoChar() const;

#ifdef __PENINPUT_HWR_STROKE_LOG_ON
    /**
     * write log "Clear"
     *
     * @since S60 V4.0
     */
    void WriteHwrStrokeLogClear();

    /**
     * write log character range
     *
     * @since S60 V4.0
     */
    void WriteHwrStrokeLogRange();
#endif

private: //data
    /**
     * The ui state for pen manager
     * Own
     */
    CPeninputUiStateMgr* iUiStatePenMgr;

    /**
     * The hwr window
     * Not Own
     */
    CPeninputHwrBoxWindow* iHwrWindow;

    /**
     * The hiraganakanji
     * Own
     */
    CPeninputHiraganaKanji* iHiraganaKanji;

    /**
     * status:WaitforEditorTextComing
     */
    TInt iWaitforEditorTextComing;

    /**
     * Active object for calling @c KeyEventCallback asynchronously.
     * Own.
     */
    CAsyncCallBack* iKeyEventAsync;


    /**
     * Active object for calling @c KeyEventClearCallback asynchronously.
     * Own.
     */
    CAsyncCallBack* iKeyEventClearAsync;

    /**
     * Active object for calling @c PointerEventCallback asynchronously.
     * Own.
     */
    CAsyncCallBack* iPointerEventAsync;

    TRawEvent iKeyEvent;
    TRawEvent iPointerEventDown;

    TBool iCapturePointerEvent;

    /**
     * The last Submit Text
     * Own
     */
    HBufC* iSubmitText; // own

    /**
     * The Original InputRange
     */
    TInt iRangeOrg;

    /**
     * The dropdownlist expanded
     */
    TBool iExpanded;

    /**
     * The drop down list control
     * Not own
     */
    CFepUiBaseCtrl* iDropdownList;

#ifdef __PENINPUT_HWR_STROKE_LOG_ON
    RFileLogger iLogger;
    TInt iLogOff;
#endif
    };

#endif // C_CPENINPUTHWRBOXLAYOUT_H

//End Of File
