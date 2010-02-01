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
* Description:  Implementation of the common layout data manager
*
*/


#ifndef C_CPENINPUTDATAMGR_H
#define C_CPENINPUTDATAMGR_H

// System includes
#include <e32base.h>
#include <w32std.h>
#include <peninputpluginutils.h>
#include <coemain.h>

// User includes
#include "peninputdataprovider.h"

// Forward decalaration
class MPeninputLayoutContext;
class CRepository;

/**
 *  Data manager implementation
 *
 *  @lib peninputcommonlayout.lib
 *  @since S60 v3.2
 */
class CPeninputDataMgr : public CBase,
                         public MPeninputDataProvider
    {

public:

    /**
     * Create one CPeninputDataMgr object
     *
     * @since S60 v3.2
     * @param aContext The layout context
     * @return The pointer to CPeninputDataMgr object
     */
    IMPORT_C static CPeninputDataMgr* NewL( 
        MPeninputLayoutContext* aContext );

    /**
     * Create one CPeninputDataMgr object
     *
     * @since S60 v3.2
     * @param aContext The layout context
     * @return The pointer to CPeninputDataMgr object
     */
    IMPORT_C static CPeninputDataMgr* NewLC( 
        MPeninputLayoutContext* aContext );

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C virtual ~CPeninputDataMgr();

    /**
     * Other initialization
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C virtual void InitMore();

    /**
     * Call back function, which is called when some key changed
     *
     * @since S60 v3.2
     * @param aChangedKey The key whose value is changed
     * @return None
     */
    IMPORT_C virtual void HandleGSRepositoryChange( TInt aChangedKey );

    /**
     * Notify engine that some data changed
     *
     * @since S60 v3.2
     * @param aType The type of the data to be notified
     * @param aData The pointer to the notified data
     * @return None
     */
    IMPORT_C virtual void NotifyEngine( TPeninputDataType aType, 
                                        TAny* aData );
                                        
    IMPORT_C TBool IsShiftPermitted();
    IMPORT_C TBool IsCapsLockPermitted();

// From base class MPeninputDataProvider

    /**
     * From MPeninputDataProvider
     * Request data according to the data type
     *
     * @since S60 v3.2
     * @param aDataType The data type need to get
     * @return The pointer to TAny type required data
     */
    IMPORT_C TAny* RequestData( TPeninputDataType aDataType );

    /**
     * From MPeninputDataProvider
     * Set data according to the data type
     *
     * @since S60 v3.2
     * @param aDataType The data type need to set
     * @param aData The pointer to the new setting data
     * @return None
     */
    IMPORT_C void SetData( TPeninputDataType aDataType, TAny* aData );

    /**
     * From MPeninputDataProvider
     * Reset data contained in data manager
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C virtual void Reset();

    /**
     * From MPeninputDataProvider
     * Save data into AknFepRepository
     *
     * @since S60 v3.2
     * @param aKey The key name
     * @param aValue The key value
     * @return None
     */
    IMPORT_C virtual void SaveKey( TInt aKey, TInt aValue );

protected:

    /**
     * Constructor
     *
     * @since S60 v3.2
     * @param aContext The layout context
     * @return None.
     */
    IMPORT_C CPeninputDataMgr( MPeninputLayoutContext* aContext );

    /**
     * 2nd constructor function
     *
     * @since S60 v3.2
     * @return None.
     */
    IMPORT_C void ConstructL();

    /**
     * Handle notification from CenRep monitor
     *
     * @since S60 v3.2
     * @param aPtr The pointer to callback data
     * @return The result value.
     */
    IMPORT_C static TInt HandleGSRepositoryCallBack( TAny* aPtr );

private:

     /**
     * Set Screen Mode
     *
     * @since S60 v3.2
     * @param aScreenMode The reference of the screen mode data
     * @return None
     */
    void SetScreenMode( const TPixelsTwipsAndRotation& aScreenMode );

protected: // Data

    /**
     * The case
     */
    TInt iCase;

    /**
     * The peninput language
     */
    TInt iInputLanguage;

    /**
     * The permitted range
     */
    TInt iPermittedRange;

    /**
     * The permitted case
     */
    TInt iPermittedCase;
    
    /**
     * The current range
     */
    TInt iCurrentRange;

    /**
     * The client layout
     */
    TInt iClientLayout;

    /**
     * The vkb layout
     */
    TInt iVkbLayoutId;

    /**
     * The editor numeric key map
     */
    TInt iEditorNumericKeymap;

    /**
     * The editor numeric key map
     */
    TBool iEnableSwitchBtn;
    
    /**
     * The flag of shift down
     */
    TBool iShiftDown;

    /**
     * The flag of capslock down
     */
    TBool iCapslockDown;

    /**
     * The current accent
     */
    TInt iCurrentAccent;

    /**
     * The window position
     */
    TInt iWindowPosition;

    /**
     * The screen mode
     */
    TPixelsTwipsAndRotation iScreenMode;

    /**
     * The screen size
     */
    TSize iScreenSize;

    /**
     * The unit width
     */
    TInt iUnitWidth;

    /**
     * The unit height
     */
    TInt iUnitHeight;

    /**
     * The setting id for window postion
     */
    TInt iPositionSettingId;

    /**
     * The rectangle of data query dialog
     */
    TRect iDataQueryDlgRect;
    
    /**
     * The layout context
     * Not own
     */
    MPeninputLayoutContext* iContext;

    /**
     * The global setttings repository
     * Own
     */
    CRepository* iGSRepository;

    /**
     * The aknfep repository
     * Own
     */
    CRepository* iAknFepRepository;

    /**
     * The repository watcher, 
     * which will monitor change of the language and guide line
     * Own
     */
    CAknFepRepositoryWatcher* iGSRepositoryWatcher;

    /**
     * The user defined resource for number
     * Own
     */    
    HBufC* iUserDefinedResource;    
    
    /**
     * The reserve data
     * not Own
     */
    TAny* iReserve_1;
    
    /**
     * The position of the top guide line
     */
    TInt iGuideLineTop;
    
    /**
     * The position of the bottom guide line
     */
    TInt iGuideLineBottom;

    /**
     * Layout size changing flag
     */
    TBool iLayoutSizeChanging;
    
    /**
     * Indicate whether unit width has been set
     */
    TBool iUnitWidthSet;

    /**
     * Indicate whether unit height has been set
     */
    TBool iUnitHeightSet;

    /**
     * Indicate whether DeadKey has been latched
     */
    TBool iLatched;
    
     /**
     * Indicate whether unitsize has been changed
     */
    TBool iUnitSizeSet;
    
    TBool iAllowCaseChange;
    };

#endif // C_CPENINPUTDATAMGR_H
