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
* Description:  peninput common layout framework
*
*/


#ifndef C_CPENINPUTCOMMONLAYOUTEXT_H
#define C_CPENINPUTCOMMONLAYOUTEXT_H

// System includes
#include <centralrepository.h>
#include <peninputlayout.h>
#include <peninputlayoutplugininterface.h>

// User includes
#include "peninputlayoutcontext.h"

// Forward decalaration
class CPeninputDataMgr;
class CPeninputUiStateMgr;
class MPeninputUiStateMgr;
class CPeninputLayoutWindowExt;

/**
 *  Peninput common layout
 *
 *  @lib peninputcommonlayout.lib
 *  @since S60 v3.2
 */
class CPeninputCommonLayoutExt : public CFepUiLayout, 
                              public MPeninputLayoutContext
    {

public:

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C virtual ~CPeninputCommonLayoutExt();

    /**
     * Get data manager
     *
     * @since S60 v3.2
     * @return The pointer to MPeninputDataProvider
     */
    IMPORT_C MPeninputDataProvider* DataMgr() const;

    /**
     * Get layout window
     *
     * @since S60 v3.2
     * @return The pointer to CPeninputLayoutWindow object
     */
    IMPORT_C CPeninputLayoutWindowExt* LayoutWindow() const;

    /**
     * Get ui state manager
     *
     * @since S60 v3.2
     * @return The pointer to MPeninputUiStateMgr
     */
    IMPORT_C MPeninputUiStateMgr* UiStateMgr() const;

// From CFepUiLayout

    /**
     * From CFepUiLayout
     * Handle layout command event
     *
     * @since S60 v3.2
     * @param aCmd Command Id
     * @param aData Data for command
     * @return Errors when return value smaller than 0     
     */
    IMPORT_C TInt HandleCommand( TInt aCmd, TUint8* aData );

    /**
     * From CFepUiLayout
     * Inform layout the size changing event
     *
     * @since S60 v3.2
     * @param aData The event data
     * @return KErrNone if successfully size changing
     */
    IMPORT_C TInt SizeChanged( const TAny* aData );

    /**
     * From CFepUiLayout
     * Handle the event from control
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aCtrl The sender
     * @param aEventData The event data
     * @return None
     */
    IMPORT_C virtual void HandleControlEvent( TInt aEventType, 
                                              CFepUiBaseCtrl* aCtrl, 
                                              const TDesC& aEventData );

    /**
     * From CFepUiLayout
     * Handle editor text coming
     * Called by animation dll when app editor text is coming
     * If a ui layout has a editor area, it must implements 
     * this function to get the text
     *
     * @since S60 v3.2
     * @param aData Editor Text Coming data
     * @return KErrNone is operation succeeded
     */
    IMPORT_C TInt OnAppEditorTextComing( 
        const TFepInputContextFieldData& aData );

    /**
     * From CFepUiLayout 
     * Get the layout type, vkb or hwr. It's now similar to LayoutType which defines in 
     * another interface.
     * 
     * @since S60 v3.2
     * @return The layout type
     */
    IMPORT_C virtual TInt PenInputType();
    
    IMPORT_C void SetPositionFromOutside(const TPoint& aNewPos);
                                  
// From MKeyRawEventHandler

    /**
     * From CFepUiLayout
     * Handle raw event key
     *
     * @since S60 v3.2
     * @param aKeyEvent The key raw event data
     * @return ETrue if the key was handled, otherwise EFalse
     */
    IMPORT_C TBool HandleRawKeyEventL( const TRawEvent& aKeyEvent );

// From MPeninputLayoutContext

    /**
     * Send event to window
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aCtrl The sender
     * @param aEventData The event data
     * @return None
     */
    IMPORT_C void SendEventToWindow( TInt aEventType, 
                                     CFepUiBaseCtrl* aCtrl, 
                                     const TDesC& aEventData = KNullDesC );

    /**
     * Send event to layout
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aCtrl The sender
     * @param aEventData The event data
     * @return None
     */
    IMPORT_C void SendEventToLayout( TInt aEventType, 
                                     CFepUiBaseCtrl* aCtrl, 
                                     const TDesC& aEventData = KNullDesC );


    IMPORT_C TBool IsShiftPermitted();
    
    IMPORT_C TBool IsCapsLockPermitted();
    /**
     * Request data from data provider
     *
     * @since S60 v3.2
     * @param aDataType The data request 
     * @return The data you request
     */
    IMPORT_C TAny* RequestData( TPeninputDataType aDataType );

    /**
     * Set resource utils
     *
     * @since S60 v3.2
     * @param aDataType Data type
     * @param aData Data value
     * @return None
     */
    IMPORT_C void SetData( TPeninputDataType aDataType, TAny* aData );

    /**
     * Sendkey to fep
     *
     * @since S60 v3.2
     * @param aEventType Event type
     * @param aEventData Event data
     * @return None
     */
    IMPORT_C void Sendkey( TInt aEventType, 
                           const TDesC& aEventData = KNullDesC );

protected:

   /**
     * 2nd phase construction - constructor
     *
     * @since S60 v3.2
     * @param aLafEnv The LAF enviroment
     * @param aLayoutOwner The owner of layout
     * @return None
     */ 
    IMPORT_C CPeninputCommonLayoutExt( MLayoutOwner* aLayoutOwner );

    /**
     * 2nd phase construction - second constructor
     *
     * @since S60 v3.2
     * @param aInitData Init data of layout
     * @return None
     */
    IMPORT_C void ConstructL( const TAny* aInitData );

    /**
     * From CFepUiLayout
     * Test whether the position is valid
     * This is used to test the target position when draging controls 
     * to some place
     *
     * @since S60 v4.0
     * @param aRect The rect to be checked
     * @param aDragbar The dragbar being dragged
     * @param aVInfo The valid information in vertial dirtection. ETrue is 
     *        vertical moving is valid.It's only used when aRect is not 
     *        valid but moving in vertical direction is valid
     * @param aHInfo The valid information in horizontal dirtection
     *        ETrue is horizontal moving is valid.It's only used when aRect 
     *        is not valid but moving in vertical direction is valid
     * @param aFlag ETrue if whole layout is moving, otherwise, only 
     *        components moving
     * @return ETrue if aRect is a valid position. Otherwise, EFalse
     */       
    IMPORT_C TBool IsValidDestination( const TRect& aRect, 
                                       CDragBar* aDragBar, 
                                       TBool& aVInfo, 
                                       TBool& aHInfo, 
                                       TBool aFlag );

    /**
     * Init layout owner
     *
     * @since S60 v3.2
     * @return The rect of the layout
     */
    IMPORT_C virtual TRect Init();

    /**
     * Save a virtual keybroad key to the repository
     * 
     * @since S60 v3.2
     * @param aKey The key name in repository
     * @param aValue The unicode of key
     * @return None
     */
    IMPORT_C virtual void SaveKey( TInt aKey, TInt aValue );

    /**
     * Factory method for creation of layout window. The decision which 
     * layout window(VKB or HWR) is created will be postboned in subclasses 
     * of this class.
     *
     * @since S60 v3.2
     * @return None
     */
    virtual void CreateLayoutWindowL() = 0;

    /**
     * Factory method for creation resource config. It is enough at present 
     * to create and initialize resource config. Any necessary extension 
     * will be implemented in subclasses.
     * 
     * @since S60 v3.2
     * @return None
     */
    virtual void CreateDataMgrL( const TAny* aInitData ) = 0;

    /**
     * Get the layout type, vkb or hwr, child class must rewrite the function.
     * 
     * @since S60 v3.2
     * @return The layout type
     */
     virtual TInt LayoutType() = 0;

private: // New method
	
    /**
     * Change input language
     *
     * @since S60 v3.2
     * @param aLangID The language id you want to change to
     * @return None
     */
    void ChangeInputLanguage( TInt aLangID );

    /**
     * Change current range
     *
     * @since S60 v3.2
     * @param aRange The range need to set
     * @return None
     */
    void ChangeRange( TInt aRange );
    
    /**
     * Translate range for some languages
     *
     * @since S60 v3.2
     * @param aRange The range need to set
     * @return New range
     */    
    TInt TranslateRange(TInt aRange);
    
    /**
     * Set if enable switch to hwr
     *
     * @since S60 v3.2
     * @param aEnable The enable flag
     * @return None
     */ 
    void SetEnableSwitchToHwr(TBool aEnable);
    
    /**
     * Set if enable option button and language switch button
     *
     * @since S60 v3.2
     * @param aEnable The enable flag
     * @return None
     */ 
    void SetEnableSettingBtn(TBool aEnable);
    
protected:
	
    /**
     * The data manager
     * Own
     */
    CPeninputDataMgr* iDataMgr;
    
    /**
     * The ui state manager
     * Own
     */
    CPeninputUiStateMgr* iUiStateMgr;

    /**
     * The layout window
     * Own
     */    
    CPeninputLayoutWindowExt* iLayoutWindow;
    
    };

#endif // C_CPeninputCommonLayoutExt_H
