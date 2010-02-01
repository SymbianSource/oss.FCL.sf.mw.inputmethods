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


#ifndef __PENINPUTHWRFSCNCANDIDATEPANEL_H__
#define __PENINPUTHWRFSCNCANDIDATEPANEL_H__

#include "peninputhwrfscnbasepanel.h"
#include "peninputhwrfscnstatemanager.h"
#include "peninputhwrfscndatastore.h"
#include "peninputdropdownlist.h"

/**
* Class CPeninputHwrfscnCandidatePanel
*
* Define the candidate panel and contain the candidate window
*
* @lib peninputhwrfscn.lib
* @since S60 v5.0
*/
class CPeninputHwrfscnCandidatePanel : public CPeninputHwrfscnBasePanel
	{
public:
    /**
	* The factory function
	*
	* @since Series 60 5.0
	* @param aLayout The Full screen layout
	* @return Pointer to created CPeninputHwrfscnCandidatePanel object
	*/	    
	static CPeninputHwrfscnCandidatePanel* NewL( CPeninputHwrfscnLayout* aLayout );
   
    /**
    * The destructor function
    *
    * @since Series 60 5.0
    */	    
    ~CPeninputHwrfscnCandidatePanel();

public:	
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
    * From CPeninputHwrfscnBasePanel
    * Handle control's event
    *
    * @since Series 60 5.0
    * @param aEventType Event type
    * @param aCtrl Event source
    * @param aEventData event data
    */
	virtual void HandleControlEvent( TInt aEventType, CFepUiBaseCtrl* aCtrl, 
        const TDesC& aEventData );
    
    /**
    * From CPeninputHwrfscnBasePanel
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
    * From CPeninputHwrfscnBasePanel
    * Handle laf (Look and feel) changed
    *
    * @since S60 v5.0
    */
	void LafChanged();
	
    /**
    * Show or Hide writing candidate dropdown list
    * 
    * @since S60 v5.0
    * @param aVisible Show or hide
    */
	void ShowWritingCandidate( TBool aVisible );
	
private:
   /**
    * The constructor
    *
    * @since S60 5.0
    * @param aLayout The layout pointer.
    */
	CPeninputHwrfscnCandidatePanel( CPeninputHwrfscnLayout* aLayout );
	
    /**
    * 2nd phase constructor
    *
    * @since S60 5.0
    */
	void ConstructL();
	
    /**
    * Create writing candidate dropdown list
    *
    * @since S60 5.0
    */
    void AddCandidateDropdownListL();
    
    /**
    * Create predictive candidate dropdown list
    *
    * @since S60 5.0
    */
    void AddPredictiveDropdownListL();	    
	    
    /**
    * Read LAF information
    *
    * @since S60 5.0
    * @param aLandscapeStyle Landscape style
    */
	void ReadLafInfo( TBool aLandscapeStyle );

    /**
    * Move the candidate window when cursor moving
    *
    * @since S60 5.0
    * @param aCursorRect The cursor rect
    */
    void MoveCandidateWindow( const TRect aCursorRect );
    
    /**
    * Clear the candidate window
    *
    * @since S60 5.0
    */
    void ClearWindow();
    
    /**
    * Get the max row number according to different primary range
    *
    * @since S60 5.0
    * @return TInt the max row number
    */
    TInt MaxRowNum();
    
    /**
    * Check the validity of the cursor position 
    *
    * @since S60 5.0
    * @param aCursorRect The cursor rect
    * @return TBool the cursor position is valid or not
    */
    TBool CheckCursorPosition( const TRect aCursorRect ) const;
      
private:
	/**
	* The writing candidate pointer
	* Own
	*/
	CFepCtrlDropdownList *iCandidateList;
	
    /**
	* The predictive candidate pointer
	* Own
    */
    CFepCtrlDropdownList *iPredictList;
    
    /**
    * The data store object
    */
    CPeninputHwrfscnDataStore& iDataStore;
    
    /**
     * The unit width
     */
    TInt iUnitWidth;

    /**
     * The unit height
     */
    TInt iUnitHeight;
	};

#endif	//__PENINPUTHWRFSCNCANDIDATEPANEL_H__