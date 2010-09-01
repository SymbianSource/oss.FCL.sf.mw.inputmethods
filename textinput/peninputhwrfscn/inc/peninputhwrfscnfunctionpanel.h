/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for chinese peninput full screen hwr
*
*/


#ifndef __PENINPUTHWRFSCNFUNCTIONPANEL_H__ 
#define __PENINPUTHWRFSCNFUNCTIONPANEL_H__ 

#include "peninputhwrfscnbasepanel.h"
#include "peninputlayoutctrlgroup.h"
#include <peninputlayoutvkb.h>
#include <peninputvkbctrllayout.h>
#include "peninputeventbutton.h"
#include "peninputlayoutlatchbutton.h"

class CPeninputLayoutInputmodelChoice;
/**
* Class CPeninputHwrfscnFunctionPanel
*
* Define the function panel and contain vkb control and button group
*
* @lib peninputhwrfscn.lib
* @since S60 v5.0
*/
class CPeninputHwrfscnFunctionPanel : public CPeninputHwrfscnBasePanel
	{
public:
    /**
    * The factory function
    *
    * @since Series 60 5.0
    * @param aLayout The Full screen layout
    * @return Pointer to created CPeninputHwrfscnFunctionPanel object
    */
    static CPeninputHwrfscnFunctionPanel* NewL( CPeninputHwrfscnLayout* aLayout );

    /**
    * The destructor function
    *
    * @since Series 60 5.0
    */
    ~CPeninputHwrfscnFunctionPanel();

// From base class
    /**
    * From CPeninputHwrfscnBasePanel
    * Show controls
    *
    * @since Series 60 5.0
    * @param aVisible Show or hide
    */
	void Show( TBool aVisible );

    /**
    * From CPeninputHwrfscnBasePanel
    * Draw
    *
    * @since Series 60 5.0
    */
	virtual void Draw();

    /**
    * Handle control's event
    *
    * @since Series 60 5.0
    * @param aEventType Event type
    * @param aCtrl Event source
    * @param aEventData event data
    */
	virtual void HandleControlEvent( 
	    TInt aEventType, 
	    CFepUiBaseCtrl* aCtrl, 
	    const TDesC& aEventData );

    /**
    * Handle layout command
    *
    * @since Series 60 5.0
    * @param aCommand command type
    * @param aData Event data
    */
    virtual void HandleCommand( TInt aCommand, TUint8* aData );

    /**
    * From CPeninputHwrfscnBasePanel
    * Check if the property could be changed before changing it.
    *
    * @since S60 v5.0
    * @param aProperty One of @see TPeninputHwrfscnProperty changed
    * @return TBool ETrue if the property needed to be changed, EFalse otherwise
    */
    virtual TBool BeforePropertyChangedL( 
        MPeninputHwrfscnPropertySubscriber::
        TPeninputHwrfscnProperty aProptery );

	/**
    * From CPeninputHwrfscnBasePanel
	* Notify the property subscriber the property changed.
	*
	* @since S60 v5.0
	* @param aProperty One of @see TPeninputHwrfscnProperty changed
	*/
    virtual void PropertyChangedL( 
        MPeninputHwrfscnPropertySubscriber::
        TPeninputHwrfscnProperty aProptery );

    /**
    * From CPeninputHwrfscnBasePanel
    * Handle size changed.
    *
    * @since S60 v5.0
    */
    void SizeChanged();
	
    /**
    * 
    * Handle skin changed.
    *
    * @since S60 v5.0
    */    
    void HandleSkinChanged();

private:
   /**
    * The constructor
    *
    * @since Series 60 5.0
    * @param aLayout The layout pointer.
    */
    CPeninputHwrfscnFunctionPanel( CPeninputHwrfscnLayout* aLayout );

    /**
    * 2nd phase constructor
    *
    * @since Series 60 5.0
    */
    void ConstructL();

    /**
    * From CPeninputHwrfscnBasePanel
    * Handle laf (Look and feel) changed
    *
    * @since S60 v5.0
    */
    void LafChanged();

    /**
    * Create symbol table
    *
    * @since S60 v5.0
    */
    void CreateSymbolTableL();

    /**
    * Create more function button list
    *
    * @since S60 v5.0
    */
    void CreateMoreFuncBtnListL();

    /**
    * Add button to more function group
    *
    * @since S60 v5.0
    * @param aRect control rectangle
    * @param aCtrlId control ID
    * @param aResID resource ID
    */
    void AddBtnToMoreFuncGrpL( const TRect aRect, 
        TInt aCtrlId, 
        TInt aResID );

    /**
    * Handle control's button up event
    *
    * @since Series 60 5.0
    * @param aEventType Event type
    * @param aCtrl Event source
    * @param aEventData event data
    */
    void OnControlEvent_ButtonUp( TInt aEventType, 
        CFepUiBaseCtrl* aCtrl, 
		const TDesC& aEventData );

    /**
    * Dim keys when in number mode
    *
    * @since Series 60 5.0
    */
    void NumberModeDimKeysL();

	/**
	* Read LAF information
	*
	* @since Series 60 5.0
	*/
    void ReadLAFInfo();

	/**
	* Reset More function buttons size
	*
	* @since Series 60 5.0
	*/
	void ResetMoreFuncBtnsSize();

	/**
	* Reset symbol table size
	*
	* @since Series 60 5.0
	*/
	void ResetSymbolTableSize();

	/**
	* Get more function button group rectangle
	*
	* @since Series 60 5.0
	* @return button group rectangle
	*/
	TRect MoreFuncBtnGroupRect();
	
	/**
	* Get width switcher button rectangle
	*
	* @since Series 60 5.0
	* @return button group rectangle
	*/
	TRect WidthSwitcherBtnRect( );
	
	/**
	* Get space button rectangle
	*
	* @since Series 60 5.0
	* @return button group rectangle
	*/
	TRect SpaceBtnRect( );
	
	/**
	* Get enter button rectangle
	*
	* @since Series 60 5.0
	* @return button group rectangle
	*/
	TRect EnterBtnRect( );
	
    /**
    * Get Symbol table control rectangle
    *
    * @since Series 60 5.0
    * @return Symbol table rectangle
    */
    TRect SymbolTableRect();

private: //enum data
    /**
    * The more function group buttons index
    */
    enum TPeninputHwrfscnMoreFuncGrp
        {
        //This definition is used for more function btn
        ELangWidthFullHalf = 0,
        ELangWidthHalfFull,
        ESpaceBtn,
        EEnterBtn,
        EMoreFuncBtnCount
        };
    
     /**
     * The width state
     */
     enum TPeninputHwrfscnWidthState
         {
         EFullWidth = 0,
         EHalfWidth,
         };
           
private:

	/**
	* The VKB control
	* Not Own
	*/
    CPeninputVkbCtrlExt* iVkbCtrl;

	/**
	* The VKB layout
	* Own
	*/
    CAknFepCtrlVkbLayout* iVkbLayout;

	/**
	* More function button group
	* Not own
	*/
    CControlGroup* iMoreFuncBtnGroup;

	/**
	* Symbol table group
	* Not own
	*/
    CControlGroup* iSymbolTableGroup;
    
	/**
	* language width state
	*/
    TPeninputHwrfscnWidthState iLangWidthState;
    
	/**
	* Button width
	*/
	TInt iUnitWidth;

	/**
	* Button height
	*/
	TInt iUnitHeight;
    
    CFbsBitmap* iSvgLatched;
    CFbsBitmap* iSvgUnLatched;
	};

#endif	//__PENINPUTHWRFSCNFUNCTIONPANEL_H__ 