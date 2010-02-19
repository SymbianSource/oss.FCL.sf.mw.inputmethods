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
* Description:  peninput VKB data manager
*
*/

#ifndef PENINPUTVKBLAYOUT_H
#define PENINPUTVKBLAYOUT_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

#include <peninputcommonlayoutext.h>
#include "PtiDefs.h"

class CPtiEngine;

class CPeninputVkbKrLayout : public CPeninputCommonLayoutExt
    {

public:

    static CPeninputVkbKrLayout* NewL(
        MLayoutOwner* aLayoutOwner, 
        const TAny* aInitData,
        const TInt& aInputMode);

    virtual ~CPeninputVkbKrLayout();

private:

    CPeninputVkbKrLayout(
        MLayoutOwner* aLayoutOwner,
        const TInt& aInputMode);

    void ConstructL(
        const TAny* aInitData);

private: // from CFepUiLayout
    
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
    
    TInt OnResourceChange(TInt aType);     

private: // from CPeninputCommonLayout

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
     
     TInt SizeChanged( const TAny* aData );
     
     void HandleAppInfoChange(const TDesC& aInfo, TPeninputAppInfo aType);
    /* Teleca change begin, 21.05.2009 ssal */
    static TInt HideByteWarningBubble(TAny* aPointer);
    void HideByteWarningBubble();
	/* Teleca change end, 21.05.2009 ssal */ 
     
private: //data
    
    CPtiEngine* iPtiEngine;
    
    TInt iInputMode;

    TBool iInEditWordQueryDlg;
    /* Teleca change begin, 21.05.2009 ssal */
    CPeriodic* iInfoTimer;
    /* Teleca change end, 21.05.2009 ssal */   
    TPtiKeyboardType iKeyBoardType;
	TBool iIsOpenVkbKr;
    };

#endif // PENINPUTVKBLAYOUT_H
