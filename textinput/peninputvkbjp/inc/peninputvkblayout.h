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
* Description:  vkb layout
*
*/


#ifndef C_CPENINPUTVKBLAYOUT_H
#define C_CPENINPUTVKBLAYOUT_H

//  System includes
#include <peninputcommonlayout.h>

/** class forward decalaration */
class CPeninputVkbWindow;
class CPeninputHiraganaKanji;

/**
 *  peninputvkbjp layout
 *
 *  @lib peninputvkbjp.lib
 *  @since S60 v3.2
 */
class CPeninputVkbLayout : public CPeninputCommonLayout
    {

public:

    /**
     * Symbian constructor.
     *
     * @since S60 v3.2
     * @param aLafEnv The LAF enviroment
     * @param aInitData Init data for layout
     * @return Pointer to created CPeninputVkbLayout object
     */
    static CPeninputVkbLayout* NewL(MLayoutOwner* aLayoutOwner, const TAny* aInitData);

    /**
     * destructor.
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CPeninputVkbLayout();

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
     * Set current range
     *
     * @since S60 v3.2
     * @return None.
     */
    void DoSetCurrentRange();

private:

    /**
     * C++ default constructor
     *
     * @since S60 v3.2
     * @param aLafEnv The LAF enviroment
     * @return None
     */
    CPeninputVkbLayout(MLayoutOwner* aLayoutOwner);

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v3.2
     * @param aInitData Init data of layout
     * @return None
     */
    void ConstructL(const TAny* aInitData);

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
     * Translate range by fullhalf
     *
     * @since S60 v3.2
     * @param aRange The range need to set
     * @return New range
     */    
    TInt TranslateRangeHullHalf(TInt aRange) const;

    // async 
    static TInt KeyEventCallback(TAny* aThis);
    static TInt PointerEventCallback(TAny* aThis);
    void DoKeyEventL();
    void DoPointerEvent();

    /**
     * PointerEvent Capture or not.
     *
     * @since S60 V4.0
     * @param aFlag ETrue Capture on.
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

private: //data

    /**
     * The vkb window
     * Not Own
     */
    CPeninputVkbWindow* iVkbWindow;

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
     * The Original InputRange
     */
    TInt iRangeOrg;

    /**
     * Active object for calling @c KeyEventCallback asynchronously.
     * Own.
     */
    CAsyncCallBack* iKeyEventAsync;

    /**
     * Active object for calling @c PointerEventCallback asynchronously.
     * Own.
     */
    CAsyncCallBack* iPointerEventAsync;

    TRawEvent iKeyEvent;
    TRawEvent iPointerEventDown;
    TBool iCapturePointerEvent;
    };

#endif // C_CPENINPUTVKBLAYOUT_H

//End Of File
