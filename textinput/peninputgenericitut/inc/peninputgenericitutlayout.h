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
* Description:  drop-down list control
*
*/


#ifndef C_GENERICITUTUILAYOUT_H
#define C_GENERICITUTUILAYOUT_H

#include <peninputlayout.h>
#include <peninputpluginutils.h>
#include <peninputlayoutmultilineicf.h>
#include <peninputinputcontextfield.h>

#include <peninputlayoutvkb.h>

#include "peninputgenericitutpropertysubscriber.h"
#include "peninputgenericitutlayoutcontext.h"

class CAknFepCtrlButton;
class CAknFepCtrlRepeatButton;
class CAknFepCtrlRawKeyButton;
class CGenericItutDataMgr;
class CFepCtrlDropdownList;
class CGenericItutUiMgrBase;
class CPeninputLayoutInputmodelChoice;
class CFepLayoutScrollableList;
class CAknFepCtrlLabel;
class CAknFepCtrlCommonButton;
class CAknFepCtrlCommonBgCtrl;
class CAknFepCtrlRepeatButtonEx;

class CGenericItutWindowManager;
class CGenericItutUiLayout : public CFepUiLayout,
                             public MItutLayoutContext
    {
public:
    static CGenericItutUiLayout* NewL(MLayoutOwner* aLayoutOwner,const TAny* aInitData);

    ~CGenericItutUiLayout();

    void HandleControlEvent(TInt aEventType, CFepUiBaseCtrl* aCtrl, const TDesC& aEventData);

    TInt OnAppEditorTextComing(const TFepInputContextFieldData& aData);

    TInt HandleCommand(TInt aCmd, TUint8* aData) ;

    TInt PenInputType(); 

    TInt SizeChanged(const TAny* pData);

    // from MItutLayoutContext
    CFepUiBaseCtrl* Control(TInt aCtrlId);
    
    CFepUiLayout* UiLayout();

    void SubmitText(const TDesC& aEventData);

    CGenericItutUiMgrBase* UiMgr();

    void OnDeActivate();
    
    void OnActivate();    

    inline CGenericItutDataMgr* DataMgr();

    inline TBool IsSecretEdtior();

    void EnterMatchSelectionState(TBool aOn);


    void HandleAppInfoChange(const TDesC& aInfo, TPeninputAppInfo aType);
    
    void ShowArrowBtn(TInt aShowFlag);
    
    void ApplyVariantLafDataL(TBool aResolutionChange = EFalse);
    
    CGenericItutWindowManager* UiManager();
    
    void SimulateRawEvent(TInt aScanCode, TRawEvent::TType aType ); 
    
    /**
     * Simulate raw event to window server immediately
     * simulating raw event command won't be scheduled by AO 
     * because it won't be added into CPeninputAnimObj::iAnimCmd.
     *
     */
    void SimulateImmeRawEvent( TInt aScanCode, TRawEvent::TType aType );
    
    void SetInputLanguageL(TInt aLanguage);  
    
    
    void SetLatinOnly( TBool aLatinOnly );
   
private:
    CGenericItutUiLayout(MLayoutOwner* aLayoutOwner);

    void ConstructL(const TAny* aInitData); 


    void CreateICFL();

    void CreateDropdownListL();



    TInt OnSkinChange();

    TBool PressOnCtrls(TPoint aPt);

    void SetInputModeL(TInt aMode);
    
    void SetCtrlRect(CFepUiBaseCtrl* aCtrl, TInt aRectIdx);
    
    CFepUiBaseCtrl* CurrentIndicator();
    
    void SetPromptTextL(TUint8* aData);
    
    CGenericItutUiMgrBase* ChineseUiManager();
   
private:

    CGenericItutDataMgr* iDataMgr;


    TBool iPenDownOnNaviPane;

    
    CGenericItutUiMgrBase* iChnUiMgr;
    CGenericItutUiMgrBase* iWesternUiMgr;
    CPeninputLayoutInputmodelChoice* iInputModeSwitch;
    
    CGenericItutUiMgrBase* iCurrentUiMgr;
    TBool iIsSecret;
    
    CGenericItutWindowManager* iWindowMgr;
    };

inline CGenericItutDataMgr* CGenericItutUiLayout::DataMgr()
    {
    return iDataMgr;
    }

inline TBool CGenericItutUiLayout::IsSecretEdtior()
    {
    return iIsSecret;
    }

#endif //C_GENERICITUTUILAYOUT_H
