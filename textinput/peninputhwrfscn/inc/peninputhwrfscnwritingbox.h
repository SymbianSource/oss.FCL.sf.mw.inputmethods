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


#ifndef __CPENINPUTHWRFSCNWRITINGBOX_H__
#define __CPENINPUTHWRFSCNWRITINGBOX_H__

//USER INCLUDES
#include "peninputhwrfscnbasepanel.h"
#include "peninputlayouthwrwnd.h"

/**
* Class CPeninputHwrfscnWritingBox
*
* Inherited from CPeninputHwrfscnBasePanel and wrap a transparent hwr window.
* Handle the hwr writing events.
*
* @lib peninputhwrfscn.lib
* @since S60 v5.0
*/
class CPeninputHwrfscnWritingBox : public CPeninputHwrfscnBasePanel
	{
public:
    /**
    * factory function
    *
    * @since S60 v5.0
    * @param aLayout The CPeninputHwrfscnLayout pointer.
    */
    static CPeninputHwrfscnWritingBox* NewL(  CPeninputHwrfscnLayout* aLayout );
    
    /**
    * Destructor
    *
    * @since S60 v5.0
    */
    ~CPeninputHwrfscnWritingBox();
   
public:	
	/**
	* Make visible
	*
	* @since S60 v5.0
	* @param aVisible ETrue will make it visible or EFalse otherwise
	*/
	void Show( TBool aVisible );
	
	/**
	* Draw control
	*
	* @since S60 v5.0
	*/
	virtual void Draw();
	
	/**
	* Inherited from MEventObserver
	* 
	* @since S60 v5.0
    * @param aEventType The event type
    * @param aCtrl The control who sends the event
    * @param aEventData The event data 		
	*
	*/
	virtual void HandleControlEvent( TInt aEventType, CFepUiBaseCtrl* aCtrl, 
	     const TDesC& aEventData );
    
    /**
    * from CPeninputHwrfscnBasePanel
    *
    * @since S60 v5.0
    * @param aCmd Command Id
    * @param aData Data for command
    */
    virtual void HandleCommand( TInt aCommand, TUint8* aData );
    
    /**
    * from MPeninputHwrfscnPropertySubscriber
    * Check if the property could be changed before changing it.
    
    * @since S60 v5.0
    * @param aProperty One of @see TPeninputHwrfscnProperty changed
    * @return TBool ETrue if the property needed to be changed, EFalse otherwise
    */
    virtual TBool BeforePropertyChangedL(               
        MPeninputHwrfscnPropertySubscriber::TPeninputHwrfscnProperty aProptery );
    
    /**
    * from MPeninputHwrfscnPropertySubscriber
    * Notify the property subscriber the property changed
    *
    * @since S60 v5.0
    * @param aProperty One of @see TPeninputHwrfscnProperty changed
    */
    virtual void PropertyChangedL( 
        MPeninputHwrfscnPropertySubscriber::TPeninputHwrfscnProperty aProptery );

    /**
    * Size changed
    *
    * @since S60 v5.0
    */
    void SizeChanged();
    
    /**
    * Laf changed
    *
    * @since S60 v5.0
    */
    void LafChanged();		
    
    /**
    * Cancel the writing. It will stops the timer and clear all strokes
    *
    * @since S60 v5.0
    */
    void CancelCharWriting();
    
private:
    /**
    * The constructor.
    * 
    * @since S60 v5.0
    * @param aLayout The CPeninputHwrfscnLayout pointer.
    */
    CPeninputHwrfscnWritingBox( CPeninputHwrfscnLayout* aLayout );
    
    /**
    * 2nd constructor.
    *
    * @since S60 v5.0
    */
    void ConstructL();
    	
private:
    /**
    * Handle hwr writng timer out event
    *
    * @since S60 v5.0
    */
    void OnStrokeCharacterTimerOut();
    
    /**
    * Handle hwr string writing event
    *
    * @since S60 v5.0
    */
    void OnStrokeStarted();
    
    /**
    * Handle hwr storke finished event
    *
    * @since S60 v5.0
    */
    void OnStrokeFinished();
    
    /**
     * Set FSHWR writting speed.
     *
     * @since S60 v5.0
     * @param aCharDelay The submit character timer in micro seconds
     * @return None
     */
    void SetWritingSpeed( const TTimeIntervalMicroSeconds32& aCharDelay );
    
    /**
     * Set FSHWR trail drawing pen size.
     *
     * @since S60 v5.0
     * @param aSize New pen size
     * @return None
     */
    void SetPenSize( const TSize aSize );
    
    /**
     * Set FSHWR trail drawing pen color.
     *
     * @since S60 v5.0
     * @param aCol New pen color
     * @return None
     */
    void SetPenColor( const TRgb aCol );
    
private:
    /**
    * The hwr window
    *
    */
    CTransparentHwrWnd* iHwrWnd;
    
    /**
    * The stored stroke list each time storke finished
    *
    */
    RArray<TPoint> iStrokeList;
    };
	
#endif	//__CPENINPUTHWRFSCNWRITINGBOX_H__