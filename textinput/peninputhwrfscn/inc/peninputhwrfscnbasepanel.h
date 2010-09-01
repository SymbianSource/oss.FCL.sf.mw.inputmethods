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


#ifndef __PENINPUTHWRFSCNBASEPANEL_H__
#define __PENINPUTHWRFSCNBASEPANEL_H__

//SYSTEM INCLUDES
#include <peninputlayoutbasecontrol.h>
#include <e32base.h>
#include <e32cmn.h>

//USER INCLUDES
#include "peninputhwrfscnpropertysubscriber.h"

//FORWARD CLASS
class CPeninputHwrfscnLayout;

/**
* Class CPeninputHwrfscnBasePanel
*
* The control container
* @lib peninputhwrfscn.lib
* @since S60 v5.0
*/
class CPeninputHwrfscnBasePanel : public CBase,
    public MEventObserver,
    public MPeninputHwrfscnPropertySubscriber
	{
public: //struct
    /**
    * The destructor
    *
    * @since S60 v5.0
    */
	virtual ~CPeninputHwrfscnBasePanel();
	
public:	
	/**
	* Make control visible
	*
	* @since S60 v5.0
	* @param aVisible ETrue if make it visible or EFlase otherwise
	*/
	virtual void Show( TBool aVisible ) = 0;
	
	/**
	* Draw controls in this panel
	*
	* @since S60 v5.0
	*/
	virtual void Draw() = 0;
    
    /**
    * Handler command
    *
    * @since S60 v5.0
    * @param aCmd Command Id
    * @param aData Data for command
    */
    virtual void HandleCommand( TInt aCommand, TUint8* aData ) = 0;
    
    /**
    * Handle size changed.
    *
    * @since S60 v5.0
    */
    virtual void SizeChanged() = 0;	
    
    /**
    * Handle laf (Look and feel) changed
    *
    * @since S60 v5.0
    */
    virtual void LafChanged() = 0;
    
    /**
    * Add its panel's observer
    *
    * @since S60 v5.0
    * @param aObserver The event observer to be added on observer list
    */
    void AddEventObserver( MEventObserver* aObserver );
    
    /**
    * Remove its panel's observer
    *
    * @since S60 v5.0
    * @param aObserver The event observer to be removed from observer list
    */
    void RemoveEventObserver( MEventObserver* aObserver );
    
    /**
    * Set panel's position
    *
    * @since S60 v5.0
    * @param aPos The postion to be set
    */
    void SetPostion(TPoint aPos);
    
    /**
    * Check if the panel is visible now
    *
    * @since S60 v5.0
    * @return TBool ETrue if the panel is visible now or EFalse otherwise
    */
    TBool IsVisible();
    
protected: //base construct
    /**
    * Constructor
    *
    * @since S60 v5.0
    * @param aLayout The fs hwr layout pointer
    */
    CPeninputHwrfscnBasePanel( CPeninputHwrfscnLayout* aLayout );
    
    /**
    * Add the control to root control
    *
    * @since S60 v5.0
    * @param aCtrl The control to be added onto rootcontrol
    */
    void AddToRootControlL( CFepUiBaseCtrl* aCtrl );
    
    /**
    * Report events to panel's observers
    *
    * @since S60 v5.0
    * @param aEventType The event type            
    * @param aCtrl The control who sends the event    
    * @param aEventData The event data                
    */
    void ReportEvent( TInt aEventType, CFepUiBaseCtrl* aCtrl, 
	     const TDesC& aEventData );

protected:
    /**
    * Visible or not
    *
    */	    
    TBool iVisible;
    
    /**
    * The current postion
    */
    TPoint iPos;
	
	/**
	* The hwr layout pointer
	* Not own
	*/
	CPeninputHwrfscnLayout *iLayout;
	
private:
	/**
	* The observer list
	*/
	RPointerArray<MEventObserver> iEventObserverList;
};

#endif	//__PENINPUTHWRFSCNBASEPANEL_H__