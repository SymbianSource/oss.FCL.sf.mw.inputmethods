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
* Description:  header for finger hwr UiLayout
*
*/

#ifndef C_PENINPUTFINGERHWRLAYOUT_H
#define C_PENINPUTFINGERHWRLAYOUT_H

// INCLUDES
#include "peninputpluginutils.h"
#include "peninputlayout.h"
#include "peninputfingerhwrstoreconstants.h"
#include "peninputfingerhwrcontrolid.h"

// FORWARD DECLARATIONS
class CPeninputFingerHwrStateManagerBase;
class CPeninputFingerHwrDataStore;
class CRepository;
class CPeninputFingerHwrWnd;

// class DECLARATIONS

/**
 *  class CPeninputHwrfscnLayout.
 *
 *  Chinese full screen handwriting ui layout class.
 *
 *  @lib peninputfingerhwr.lib
 *  @since S60 v5.0
 */
class CPeninputFingerHwrLayout : public CFepUiLayout
    {
public:
    /**
     * The writing speed.
     *
     *@since S60 v5.0
     */
    enum TWritingSpeed
        {
        EWritingSpeedVeryFast = 1,
        EWritingSpeedFast,
        EWritingSpeedNormal,
        EWritingSpeedSlow,
        EWritingSpeedVerySlow
        };
public:
    /** 
     * Symbian constructor.
     *
     * @since S60 5.0
     * 
     * @param aLayoutOwner The MLayoutOwner
     * @param aInitData Init data for layout
     * @return Pointer to created CPeninputHwrfscnLayout object
     */
    static CPeninputFingerHwrLayout* NewL(MLayoutOwner* aLayoutOwner,
            const TAny* aInitData);

public:
    //from CFepUiLayout
    /**
     * From CFepUiLayout.
     * Handle layout command event
     *
     * @since Series 60 5.0
     * @param aCmd Command Id.
     * @param aData Data for command.
     * @return Errors when return value small than 0.    
     */
    TInt HandleCommand(const TInt aCmd, TUint8* aData);
    
    /**
     * From CFepUiLayout.
     * Handle layout command event
     *
     * @since Series 60 5.0
     * @param aCmd Command Id.
     * @param aData Data for command.
     * @return Errors when return value small than 0.    
     */
    TInt HandleCommandL(const TInt aCmd, TUint8* aData);

    /**
     * From CFepUiLayout.
     * Inform layout the size changing event
     *
     * @since Series 60 5.0
     * @param pData The event data.
     * @return KErrNone if successfully size changing. 
     */
    TInt SizeChanged(const TAny* pData);

    /**
     * From CFepUiLayout.
     * Handle editor text coming
     * Called by animation dll when app editor text is coming.
     * If a ui layout has a editor area, it must implements this function to get the text
     *
     * @since Series 60 5.0
     * @param aData The fep input context field data
     * @return KErrNone is operation succeeded.    
     */
    TInt OnAppEditorTextComing(const TFepInputContextFieldData& aData);

    /**
     * From CFepUiLayout.
     * Handle application info change
     *
     * @since Series 60 5.0
     * @param aInfo The information.
     * @param aType The information type.
     * @return none
     */
    void HandleAppInfoChange(const TDesC& aInfo, TPeninputAppInfo aType);

    
    /**
     * From CFepUiLayout
     * Called by owner when the system resource changed event.
     *
     * @since S60 v4.0
     * @param aType the event type
     * @return KErrNone if successfully size changing. 
     */
    TInt OnResourceChange(TInt aType);
    
    /**
     * From MPenUiLayoutBase
     * Return the peninput ui type
     *
     * @since Series 60 5.0
     * @return The ui type @see TPenInputUiType
     */
    TInt PenInputType();

    /**
     * From MFepLayoutBase        
     * Called by owner when the layout is going to be hidden
     *
     * @since S60 5.0
     * @return none
     */
    void OnDeActivate();
    
public:
    //from MEventObserver;    
    /**
     * From MEventObserver 
     * handle control event.
     * 
     * @since Series 60 5.0
     * @param aEventType The event type
     * @param aCtrl The control who sends the event
     * @param aEventData The event data
     * @return none
     */
    void HandleControlEvent(TInt aEventType, CFepUiBaseCtrl* aCtrl,
            const TDesC& aEventData);
    void HandleControlEventL(TInt aEventType, CFepUiBaseCtrl* aCtrl,
            const TDesC& aEventData);

public:

    /**
     * Get the data stroe object.
     * 
     * @since Series 60 5.0
     * @return The CPeninputHwrfscnDataStore reference
     */
    CPeninputFingerHwrDataStore& DataStore();

    /**
     * Replace the char before the cursor in forground app with a new char.
     *
     * @since S60 5.0
     * @param aOldCharCode The old char need to be replaced
     * @param aNewCharCode The new char for replacing the old char
     * @return none
     */
    void Replace(const TDesC& aOldCharCode, const TDesC& aNewCharCode,
            const TBool aIsPart = EFalse);

    /**
     * Submit string to layout owner.
     *
     * @since S60 5.0
     * @param aCharCode The buffer needed to be submitted.
     * @return none
     */
    void SubmitStringToFep(const TDesC& aCharCode);

    /**
     * Submit char to layout owner.
     *
     * @since S60 5.0
     * @param aCharCode The buffer needed to be submitted.
     * @return none
     */    
    void SubmitCharToFep(const TInt aCharCode);

#ifdef RD_INTELLIGENT_TEXT_INPUT
    /**
     * Submit char to layout owner.
     *
     * @since S60 5.0
     * @param aSetPosFlag The buffer needed to be submitted.
     * @return none
     */    
    void SubmitStartPosToFep(TBool aSetPosFlag);
#endif

    /**
     * repository callback for settings.
     *
     * @since S60 5.0
     * @param aPtr callback data.
     * @return error code.
     */
    static TInt HandleGSRepositoryCallBack(TAny* aPtr);
    
    /**
     * load default settings.
     * 
     * @since S60 5.0
     * @param aSetPosFlag The buffer needed to be submitted.
     * @return none
     */
    void LoadAndPublishDefaultL();
    

    /**
     * sync stroke end mark of writingbox with hwr engine.
     * 
     * @param aEndMark a TPoint object as new stroke end mark.
     * @return none   
     */
    void SyncHwrStrokeEndMark(const TPoint&  aEndMark);

#ifndef RD_INTELLIGENT_TEXT_INPUT 
    /**
     * hide or show fixed english symbol list. 
     * 
     * @since S60 5.0
     * @param aIsHide ETrue to hide lists, EFalse to show lists.
     * @return none
     */
    void HideTwoRowEngSymbolList(const TBool aIsHide);
#endif

private:
    /**
     * C++ constructor
     *
     * @since S60 5.0
     * @param aLayoutOwner The MLayoutOwner pointer.
     * @return none
     */
    CPeninputFingerHwrLayout(MLayoutOwner* aLayoutOwner);

    /**
     * standard c++ destructor.
     *
     * @since S60 5.0
     * @return none
     */
    ~CPeninputFingerHwrLayout();

    /**
     * Symbian second-phase constructor
     *
     * @since S60 5.0
     * @param aInitData The init data passed by layout engine.
     * @return none
     */
    void ConstructL(const TAny* aInitData);


    /**
     * Create the hwr box window
     *
     * @since S60 5.0
     * @return none
     */
    void CreateHwrWindowL();

    /**
     * Create the state manager
     *
     * @since S60 5.0
     * @return none
     */
    void CreateStateManagerL();

    
private:
    /**
     * handler of button down event.
     *  
     * @since S60 5.0
     * @param aEventType event type id.
     * @param aCtrl event src.
     * @param aEventData event data.
     * @return none
     */
    void OnCtrlButtonDownL(TInt aEventType, CFepUiBaseCtrl* aCtrl,  
            const TDesC& aEventData);

    /**
     * handler of button up event.
     *  
     * @since S60 5.0
     * @param aEventType event type id.
     * @param aCtrl event src.
     * @param aEventData event data.
     * @return none
     */
    void OnCtrlButtonUpL(TInt aEventType, CFepUiBaseCtrl* aCtrl, 
            const TDesC& aEventData);

    /**
     * handler of rep-button clicked event.
     *  
     * @since S60 5.0
     * @param aCtrl event src.
     * @param aData event data.
     * @return none
     */    
    void OnRepButtonClickedL(CFepUiBaseCtrl* aCtrl, const TDesC& aData);

    /**
     * clicked event handler of the BACKSPACE button.
     *  
     * @since S60 5.0
     * @return none
     */     
    void OnBackspaceClickedL();
    
    /**
     * handler of virtual key down event.
     *  
     * @since S60 5.0
     * @param aCtrl event src.
     * @param aData event data.
     * @return none
     */ 
    void OnVirtualKeyDownL(CFepUiBaseCtrl* aCtrl, const TDesC& aData);
    
    /**
     * handler of virtual key up event.
     *  
     * @since S60 5.0
     * @param aCtrl event src.
     * @param aData event data.
     * @return none
     */     
    void OnVirtualKeyUpL(CFepUiBaseCtrl* aCtrl, const TDesC& aData);
    
    /**
     * handler of candidate list event.
     *  
     * @since S60 5.0
     * @param aCtrl event src.
     * @param aData event data.
     * @return none
     */     
    void OnCandidateSelectedL(CFepUiBaseCtrl* aCtrl, const TDesC& aData);
    
    /**
     * handler of predictive list event.
     *  
     * @since S60 5.0
     * @param aCtrl event src.
     * @param aData event data.
     * @return none
     */     
    void OnPredictSelectedL(CFepUiBaseCtrl* aCtrl, const TDesC& aData);



    /**
     * clicked event handler of icf.
     *  
     * @since S60 5.0
     * @return none
     */
    void OnIcfClicked();
    
    /**
     * handler of ime selector event.
     *  
     * @since S60 5.0
     * @param aEventType event type id.
     * @param aCtrl event src.
     * @param aEventData event data.
     * @return none
     */    
    void OnImeSelected(TInt aEventType, CFepUiBaseCtrl* aCtrl, 
            const TDesC& aEventData);

    /**
     * handler of StrokeStarted event.
     *  
     * @since S60 5.0
     * @return none
     */
    void OnHwrStrokeStartedL();

    /**
     * handler of StrokeFinished event.
     *  
     * @since S60 5.0
     * @return none
     */    
    void OnHwrStrokeFinishedL();
    
    /**
     * handler of CharacterFinished event.
     *  
     * @since S60 5.0
     * @return none
     */    
    void OnHwrCharacterFinishedL();


    /**
     * handler of NumMapping changed event.
     *  
     * @since S60 5.0
     * @return none
     */
    void OnNumMappingChangedL();
    
    /**
     * Handle the candidate list events.
     *
     * @since S60 5.0
     * @param aEventType The event type
     * @param aEventData The event data 
     * @return none   
     */
    void OnCandidateWndEventL();


private:
    /**
     * Notify the layout owner the primary has changed.
     *
     * @since S60 v5.0
     * @param aPrimaryRange The range which will be changed to
     * @return none
     */
    void NotifyPrimaryRangeChanged(TInt aPrimaryRange);

    /**
     * get value from repository
     * 
     * @since S60 v5.0
     * @param aWatcher repository reader.
     * @param aId value id.
     * @return value
     */
    TInt GetNewValue(CAknFepRepositoryWatcher* aWatcher, const TInt aId);
    
    /**
     * get hwr trail color from repository
     * 
     * @since S60 v5.0
     * @return color value.
     */    
    TInt GetPenTrailColor();
    

    /**
     * set hwr writing speed.
     * 
     * @since S60 v5.0
     * @param aWritingSpeed speed
     * @return none
     */    
    void SetWritingSpeed(const TInt aWritingSpeed);
    
    /**
     * set hwr writing pen width.
     * 
     * @since S60 v5.0
     * @param aWritingSpeed speed
     * @return none
     */     
    void SetBoxPenSize(const TSize aPenSize);
    
    /**
     * set hwr writing pen color.
     * 
     * @since S60 v5.0
     * @param aWritingSpeed speed
     * @return none
     */    
    void SetBoxPenColor(const TInt aPenColor);

    /**
     * restore the range before sct popup. 
     * 
     * @since S60 v5.0
     * @return none    
     */
    void RestoreRange();

    /**
     * get current state manager. 
     * 
     * @since S60 v5.0
     * @return current state manager  
     */
    CPeninputFingerHwrStateManagerBase* CurStateManager();
    
    /**
     * change current state manager to standby state.
     * 
     * @since S60 v5.0
     * @return none 
     */
    void ChangeCurStateManagerToStandby();

private:
    
    /**
     * main window.
     * not own
     */
    CPeninputFingerHwrWnd* iHwrWnd;
    
    /**
     * The data store
     * Own
     */
    CPeninputFingerHwrDataStore* iDataStore;

    /**
     * The state manager
     * Own
     */
    RPointerArray<CPeninputFingerHwrStateManagerBase> iStateMgrArray;

    /**
     * The resource Id
     * 
     */
    TInt iResId;

    /**
     * Repository watcher
     */
    CAknFepRepositoryWatcher* iGSRepositoryWatcher;
    
    /**
     * Repository object.
     */
    CRepository* iRepositorySetting;
    
    /**
     * Last overlap index.
     */
    TInt iLastOverlapIdx;
    
    /**
     * Open Finger HWR flag
     */
    TBool iIsOpenFingerHWR;
    };
#endif //C_PENINPUTFINGERHWRLAYOUT_H