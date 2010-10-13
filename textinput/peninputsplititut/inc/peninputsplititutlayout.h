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
* Description:  split itu-t layout
*
*/


#ifndef C_SPLITITUTUILAYOUT_H
#define C_SPLITITUTUILAYOUT_H

#include <peninputlayout.h>
#include <peninputpluginutils.h>
//#include <peninputinputcontextfield.h>

#include <peninputlayoutvkb.h>

#include "peninputsplititutpropertysubscriber.h"
#include "peninputsplititutlayoutcontext.h"

class CAknFepCtrlButton;
class CAknFepCtrlRepeatButton;
class CAknFepCtrlRawKeyButton;
class CSplitItutDataMgr;
//class CFepCtrlDropdownList;
class CSplitItutUiMgrBase;
class CAknFepCtrlCommonButton;
class CAknFepCtrlCommonBgCtrl;
class CAknFepCtrlRepeatButtonEx;

class CSplitItutWindowManager;
class CSplitItutUiLayout : public CFepUiLayout,
                           public MSplitItutLayoutContext
    {
public:
    /**
     * Symbian constructor.
     *
     * @since S60 v5.0
     * @return Pointer to created object.
     */
    static CSplitItutUiLayout* NewL(MLayoutOwner* aLayoutOwner,const TAny* aInitData);

    /**
     * standard c++ destructor.
     *
     * @since S60 v5.0
     * @return None
     */ 
    ~CSplitItutUiLayout();

    /**
     * Process text update
     *
     * @since S60 v5.0
     * @param aData context data
     * @return TInt
     */ 
    TInt OnAppEditorTextComing(const TFepInputContextFieldData& aData);
    
    /**
     * handle control event
     *
     * @since S60 v5.0
     * @param aEventType event type
     * @param aCtrl control pointer
     * @param aEventData event data
     * @return None
     */
    void HandleControlEvent(TInt aEventType, CFepUiBaseCtrl* aCtrl, const TDesC& aEventData);

    /**
     * handle command
     *
     * @since S60 v5.0
     * @param aCmd command
     * @param aData 
     * @return TInt
     */
    TInt HandleCommand(TInt aCmd, TUint8* aData) ;

    /**
     * get pen input type
     *
     * @since S60 v5.0
     * @return pen input type
     */
    TInt PenInputType(); 

    /**
     * handle size change
     *
     * @since S60 v5.0
     * @param pData
     * @return TInt
     */
    TInt SizeChanged(const TAny* pData);

    /**
     * get control pointer
     *
     * @since S60 v5.0
     * @param aCtrlId
     * @return control pointer
     */
    CFepUiBaseCtrl* Control(TInt aCtrlId);
   
    /**
     * get UI layout
     *
     * @since S60 v5.0
     * @return UI layout
     */
    CFepUiLayout* UiLayout();

    /**
     * get submit text
     *
     * @since S60 v5.0
     * @param aEventData
     * @return None
     */
    void SubmitText(const TDesC& aEventData);

    /**
     * get UI layout
     *
     * @since S60 v5.0
     * @return UI manager
     */
    CSplitItutUiMgrBase* UiMgr();

    /**
     * handle deactivate
     *
     * @since S60 v5.0
     * @return None
     */
    void OnDeActivate();
    
    /**
     * handle activate
     *
     * @since S60 v5.0
     * @return None
     */
    void OnActivate();    

    /**
     * get data manager
     *
     * @since S60 v5.0
     * @return data mgr
     */
    inline CSplitItutDataMgr* DataMgr();

    /**
     * get data manager
     *
     * @since S60 v5.0
     * @return data mgr
     */
    inline TBool IsSecretEdtior();

    /**
     * handle app info change
     *
     * @since S60 v5.0
     * @return None
     */
    void HandleAppInfoChange(const TDesC& aInfo, TPeninputAppInfo aType);
    
    /**
     * show/hide arrow button
     *
     * @since S60 v5.0
     * @param aShowFlag
     * @return None
     */
    void ShowArrowBtn(TInt aShowFlag);
 
    /**
     * apply laf data
     *
     * @since S60 v5.0
     * @param aResolutionChange
     * @return None
     */
    void ApplyVariantLafDataL(TBool aResolutionChange = EFalse);
    /**
     * apply spell modelaf data
     *
     * @since S60 v5.0
     * @return None
     */
    void ApplyVariantLafDataForSpellL();
    
    
    /**
     * get ui manager
     *
     * @since S60 v5.0
     * @return ui mgr
     */
    CSplitItutWindowManager* UiManager();
    
    /**
     * apply laf data
     *
     * @since S60 v5.0
     * @param aScanCode
     * @param aType
     * @return None
     */
    void SimulateRawEvent(TInt aScanCode, TRawEvent::TType aType); 
    
    /**
     * set input language
     *
     * @since S60 v5.0
     * @param aLanguage
     * @return None
     */
    void SetInputLanguageL(TInt aLanguage);  
    
    /**
     * set latin only flag
     *
     * @since S60 v5.0
     * @param aLatinOnly
     * @return None
     */
    void SetLatinOnly( TBool aLatinOnly );
   
private:
    /**
     * C++ constructor
     *
     * @since S60 v5.0
     * @return None
     */
    CSplitItutUiLayout(MLayoutOwner* aLayoutOwner);

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v5.0
     * @return None
     */
    void ConstructL(const TAny* aInitData); 

    /**
     * create dropdownlist
     *
     * @since S60 v5.0
     * @return None
     */
//    void CreateDropdownListL();

    /**
     * create dropdownlist
     *
     * @since S60 v5.0
     * @return None
     */
    TInt OnSkinChange();

    /**
     * is press on control
     *
     * @since S60 v5.0
     * @param aPt
     * @return TBool
     */
    TBool PressOnCtrls(TPoint aPt);

    /**
     * set input mode
     *
     * @since S60 v5.0
     * @param aMode
     * @return None
     */
    void SetInputModeL(TInt aMode);
    
    /**
     * set input mode
     *
     * @since S60 v5.0
     * @param aMode
     * @return None
     */
    void SetCtrlRect(CFepUiBaseCtrl* aCtrl, TInt aRectIdx);

    /**
     * get chinese ui mgr
     *
     * @since S60 v5.0
     * @return chinese ui mgr
     */
    CSplitItutUiMgrBase* ChineseUiManager();
    
    /**
     * Handle show candidate list command
     *
     *     
     * @aData pointer is sent from FEP, point to a TPtrC
     * @return None
     */    
    void HandleShowMatchListCmdL( TUint8* aData );    

    /**
     * Read text stored in a block of memory into HBufC
     *
     *     
     * @aData aStartPtr the starting address of a block of memory with a string in it
     * @aLength The length of the string
     * @return pointer of a HBufC storing the string in it
     */ 
    HBufC* ReadTextInfoHBufCL( TUint16* aStartPtr, TInt aLength );
    
    /**
     * Handle select candidate list item command
     *
     *     
     * @aData pointer is sent from FEP, point to a TPtrC
     * @return None
     */    
    void HandleSelectServerCandItemL( TUint8* aData );
    
private:
    /**
     * data manager.
     * own
     */
    CSplitItutDataMgr* iDataMgr;

    /**
     * Chinese UI manager.
     * own
     */
    CSplitItutUiMgrBase* iChnUiMgr;
    
    /**
     * Western UI manager.
     * own
     */
    CSplitItutUiMgrBase* iWesternUiMgr;
    
    /**
     * Current UI manager.
     * own
     */
    CSplitItutUiMgrBase* iCurrentUiMgr;
    
    /**
     * is secret flag.
     */
    TBool iIsSecret;
    
    /**
     * window manager.
     * own
     */
    CSplitItutWindowManager* iWindowMgr;
    };

// ---------------------------------------------------------------------------
// CSplitItutUiLayout::DataMgr
// ---------------------------------------------------------------------------
//
inline CSplitItutDataMgr* CSplitItutUiLayout::DataMgr()
    {
    return iDataMgr;
    }

// ---------------------------------------------------------------------------
// CSplitItutUiLayout::IsSecretEdtior
// ---------------------------------------------------------------------------
//
inline TBool CSplitItutUiLayout::IsSecretEdtior()
    {
    return iIsSecret;
    }

#endif //C_SPLITITUTUILAYOUT_H
