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


#ifndef C_GENERICITUTWINDOWMANAGER_H
#define C_GENERICITUTWINDOWMANAGER_H

#include <peninputlayout.h>
#include <peninputpluginutils.h>
#include <peninputlayoutmultilineicf.h>
#include <peninputinputcontextfield.h>

#include <peninputlayoutvkb.h>
#include "peninputgenericitutlayout.h"
#include "peninputgenericitutpropertysubscriber.h"
#include "peninputgenericitutlayoutcontext.h"

class CGenericItutWindow;

class CGenericItutWindowManager : public CBase,
                                  public MItutPropertySubscriber
    {
public:
    static CGenericItutWindowManager* NewL(CGenericItutUiLayout* aLayoutOwner,
                                           CGenericItutDataMgr* iDataMgr);

    ~CGenericItutWindowManager();

    void HandleCtrlEventL(TInt aEventType, CFepUiBaseCtrl* aCtrl, const TDesC& aEventData);

    TInt OnAppEditorTextComing(const TFepInputContextFieldData& aData);

    TBool HandleCommandL(TInt aCmd, TUint8* aData);

    void SetPropertyL( MItutPropertySubscriber::TItutProperty aPropertyName, 
                       const TDesC& aPropertyValue);

    TInt SizeChanged(TBool aIcfOverlap);

    CFepUiLayout* UiLayout();

    void SubmitText(const TDesC& aEventData);

    void ShowArrowBtn(TInt aShowFlag);
    
    void HandleAppInfoChangeL(const TDesC& aInfo); 
    
    void SimulateRawEvent(TInt aScanCode, TRawEvent::TType aType );

    /**
     * Simulate raw event to window server immediately
     * simulating raw event command won't be scheduled by AO 
     * because it won't be added into CPeninputAnimObj::iAnimCmd.
     *
     */
    void SimulateImmeRawEvent(TInt aScanCode, TRawEvent::TType aType );
    
    void SetPromptTextL(TUint8* aData);
    TInt OnSkinChange();
    
    void SetLanguageL(TInt aLanguage);
    
    void ResetIndicatorL(CFepUiBaseCtrl* aIndicator);  
    
    void ShowInputModeSwitchL();
    
    void SetLastRawKeyDown(TInt aScanCode, TBool aKeyDown,  CFepUiBaseCtrl* aCtrl);    

    void CreateChineseSpecificCtrlsIfNeededL();
    
    void ApplyVariantLafDataL(TBool aResolutionChange);

    CFepUiBaseCtrl* Control(TInt aCtrlId);
    
    void DimArrowKeys( TBool aDimArrow );
    
    void ShowBubble(TInt aShow);
    
    void OnActivate();
    
    inline CGenericItutWindow* Window();
    
    /* Teleca change begin, 18.05.2009 ssal */
    static TInt HideByteWarningBubble(TAny* aPointer);
    void HideByteWarningBubble();
    void DisplayWarningNote(const TDesC& aInfo);
	/* Teleca change end, 18.05.2009 ssal */
private:

    CGenericItutWindowManager(CGenericItutUiLayout* aLayoutOwner,
                              CGenericItutDataMgr* iDataMgr);

    void ConstructL(); 
    void UpdateIndicatorL(TUint8* aData);

    void ReportItemSelected(TInt aCmdId, TInt aIdx, TBool aCommit);

private:

    CGenericItutUiLayout* iLayoutOwner;
    
    CGenericItutWindow* iWindow;
    CGenericItutDataMgr* iDataMgr;
    
    TAknWindowLineLayout iBubbleSize;
    TAknTextLineLayout iBubbleTextLayout;
    
   
    TBool iRawKeyDown;  
    CFepUiBaseCtrl* iRawKeyCtrl;
    TInt iLastRawKeyDown;
    TBool iInEditWordQueryDlg;
    TBool iDimOperationBtn;
    
    /* Teleca change begin, 18.05.2009 ssal */
    CPeriodic* iInfoTimer;
    /* Teleca change end, 18.05.2009 ssal */
    };

inline CGenericItutWindow* CGenericItutWindowManager::Window()
	{
	return iWindow;	
	}
#endif // C_GENERICITUTWINDOWMANAGER_H
