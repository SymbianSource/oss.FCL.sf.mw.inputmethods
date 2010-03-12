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


#ifndef C_SPLITITUTWINDOWMANAGER_H
#define C_SPLITITUTWINDOWMANAGER_H

#include <peninputlayout.h>
#include <peninputpluginutils.h>
#include <peninputinputcontextfield.h>

#include <peninputlayoutvkb.h>
#include "peninputsplititutlayout.h"
#include "peninputsplititutpropertysubscriber.h"
#include "peninputsplititutlayoutcontext.h"

class CSplitItutWindow;


/**
 *  class CSplitItutWindowManager.
 *
 *  Split itu-t window manager class.
 *
 *  @lib peninputsplititut.lib
 *  @since S60 v5.0
 */
class CSplitItutWindowManager : public CBase,
                                public MItutPropertySubscriber
    {
public:
    /** 
     * Symbian constructor.
     *
     * @since S60 5.0
     * 
     * @param aLayoutOwner The MLayoutOwner
     * @param iDataMgr The data mgr
     * @return Pointer to created CSplitItutWindowManager object
     */
    static CSplitItutWindowManager* NewL( CSplitItutUiLayout* aLayoutOwner,
                                          CSplitItutDataMgr* iDataMgr);

    /**
     * standard c++ destructor.
     *
     * @since S60 5.0
     * @return none
     */
    ~CSplitItutWindowManager();
    
    /**
     * Process text update
     *
     * @since S60 v5.0
     * @param aData context data
     * @return TInt
     */
    TInt OnAppEditorTextComing(const TFepInputContextFieldData& aData);
    
    /**
     * Handle control event
     *
     * @since Series 60 5.0
     * @param aEventType
     * @param aCtrl
     * @param aEventData
     * @return None    
     */
    void HandleCtrlEventL( TInt aEventType, 
                           CFepUiBaseCtrl* aCtrl, 
                           const TDesC& aEventData);

    /**
     * Handle command event
     *
     * @since Series 60 5.0
     * @param aCmd
     * @param aData
     * @return TBool    
     */
    TBool HandleCommandL(TInt aCmd, TUint8* aData);

    /**
     * From MItutPropertySubscriber
     * Set property
     *
     * @since Series 60 5.0
     * @param aPropertyName
     * @param aPropertyValue
     * @return None    
     */
    void SetPropertyL( MItutPropertySubscriber::TItutProperty aPropertyName, 
                       const TDesC& aPropertyValue);

    /**
     * handle size change
     *
     * @since Series 60 5.0
     * @return None    
     */
    TInt SizeChanged();

    /**
     * Get ui layout pointer
     *
     * @since Series 60 5.0
     * @return ui layout pointer    
     */
    CFepUiLayout* UiLayout();

    /**
     * submit text
     *
     * @since Series 60 5.0
     * @param aEventData
     * @return None    
     */
    void SubmitText(const TDesC& aEventData);

    /**
     * show/hide arrow keys
     *
     * @since Series 60 5.0
     * @param aShowFlag
     * @return None    
     */
    void ShowArrowBtn(TInt aShowFlag);
    
    /**
     * Handle application change info
     *
     * @since Series 60 5.0
     * @param aInfo
     * @return None    
     */
    void HandleAppInfoChangeL(const TDesC& aInfo); 
    
    /**
     * simulate raw event
     *
     * @since Series 60 5.0
     * @param aScanCode
     * @param aType
     * @return None    
     */
    void SimulateRawEvent(TInt aScanCode, TRawEvent::TType aType); 

    /**
     * handle skin change
     *
     * @since Series 60 5.0
     * @return None    
     */
    TInt OnSkinChange();
    
    /**
     * set last raw key down
     *
     * @since Series 60 5.0
     * @param aScanCode
     * @param aKeyDown
     * @param aCtrl
     * @return None    
     */
    void SetLastRawKeyDown(TInt aScanCode, TBool aKeyDown, CFepUiBaseCtrl* aCtrl);    

    /**
     * create chinese specific controls
     *
     * @since Series 60 5.0
     * @return None    
     */
    void CreateChineseSpecificCtrlsIfNeededL();
    
    /**
     * apply variant laf data
     *
     * @since Series 60 5.0
     * @param aResolutionChange
     * @return None    
     */
    void ApplyVariantLafDataL(TBool aResolutionChange);

    /** 
     * apply spell variant laf data
     *
     * @since S60 5.0
     * @return None
     */        
    void ApplyVariantLafDataForSpellL();
    
    /**
     * apply variant laf data
     *
     * @since Series 60 5.0
     * @param aResolutionChange
     * @return None    
     */
    CFepUiBaseCtrl* Control(TInt aCtrlId);
    
    /**
     * apply variant laf data
     *
     * @since Series 60 5.0
     * @param aResolutionChange
     * @return None    
     */
    void DimArrowKeys( TBool aDimArrow );
    
    /**
     * show/hide candidate preview bubble
     *
     * @since Series 60 5.0
     * @param aResolutionChange
     * @return None    
     */
    void ShowBubble(TInt aShow);
    
    /**
     * handle activate
     *
     * @since Series 60 5.0
     * @return None    
     */
    void OnActivate();
    
    /**
     * get split itut window
     *
     * @since Series 60 5.0
     * @return split itut window pointer    
     */
    inline CSplitItutWindow* Window();
    
private:
    /**
     * C++ constructor
     *
     * @since S60 5.0
     * @param aLayoutOwner The MLayoutOwner pointer.
     * @param iDataMgr The data mgr pointer
     * @return none
     */
    CSplitItutWindowManager( CSplitItutUiLayout* aLayoutOwner,
                             CSplitItutDataMgr* iDataMgr);

    /**
     * Symbian second-phase constructor
     *
     * @since S60 5.0
     * @return none
     */
    void ConstructL(); 
   
    /**
     * Set prompt text
     *
     * @since S60 5.0
     * paran aData
     * @return none
     */    
    void SetPromptTextL( TUint8* aData );

private:
   /**
     * The layout owner
     * Not own
     */
    CSplitItutUiLayout* iLayoutOwner;
    
   /**
     * The layout owner
     * Own
     */
    CSplitItutWindow* iWindow;
    
   /**
     * The data manager
     * Not own
     */
    CSplitItutDataMgr* iDataMgr;
    
   /**
     * The candidate preview bubble size
     */
    TAknWindowLineLayout iBubbleSize;
    
   /**
     * The candidate preview bubble text layout
     */
    TAknTextLineLayout iBubbleTextLayout;
    
   /**
     * The rawkey down flag
     */
    TBool iRawKeyDown;
    
   /**
     * The rawkey control
     * Not own
     */
    CFepUiBaseCtrl* iRawKeyCtrl;
    
   /**
     * The last rawkey down
     */    
    TInt iLastRawKeyDown;
    
   /**
     * The in editword query dialog flag
     */
    TBool iInEditWordQueryDlg;
    
   /**
     * The dim operation button flag
     */
    TBool iDimOperationBtn;
    };

// ---------------------------------------------------------------------------
// CSplitItutWindowManager::Window
// ---------------------------------------------------------------------------
//
inline CSplitItutWindow* CSplitItutWindowManager::Window()
	{
	return iWindow;	
	}
#endif // C_SPLITITUTWINDOWMANAGER_H
