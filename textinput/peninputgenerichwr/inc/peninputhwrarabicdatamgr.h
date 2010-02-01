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
* Description:  peninput HWR data manager
*
*/


#ifndef C_PENINPUTHWRARABICDATAMGR_H
#define C_PENINPUTHWRARABICDATAMGR_H

// System includes
#include <e32base.h>
#include <w32std.h>
#include <peninputdatamgr.h>

// Forward decalaration
class MPeninputLayoutContext;
class CPeninputHwrBxAbEngine;
class CPtiEngine;
/**
 *  Peninput HWR data management class
 *  This class manager the data of HWR
 *  
 *  @lib peninputgenerichwr.lib
 *  @since S60 v3.2
 */
class CPeninputHwrBxAbDataMgr : public CPeninputDataMgr
    {

public:
  
    /**
     * The HWR-pen's data struct
     */
    struct THandWritingPenData
        {
        TInt iGuideLineOn;
        TSize iPenSize;
        TRgb iPenColor;
        TInt iWritingSpeed;
        TInt iFadingSpeed;
        CPeninputHwrBxAbEngine* iRecognizer; 
        };
    
    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aContext The layout context
     * @return The pointer to CPeninputHwrBxAbDataMgr object
     */
    static CPeninputHwrBxAbDataMgr* NewL( 
        MPeninputLayoutContext* aContext ,CPtiEngine* aPtiEngine);
    
    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aContext The layout context
     * @return The pointer to CPeninputHwrBxAbDataMgr object
     */
    static CPeninputHwrBxAbDataMgr* NewLC(
        MPeninputLayoutContext* aContext ,CPtiEngine* aPtiEngine);

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CPeninputHwrBxAbDataMgr();

// From base class CPeninputDataMgr

    /**
     * From CPeninputDataMgr
     * Initialize the pen data
     *
     * @since S60 v3.2
     * @return None
     */
    void InitMore();

    /**
     * From CPeninputDataMgr
     * Call back function, which is called when some key changed
     *
     * @since S60 v3.2
     * @param aChangedKey The key whose value is changed
     * @return None
     */
    void HandleGSRepositoryChange( TInt aChangedKey );

    /**
     * From CPeninputDataMgr
     * Reset data contained in data manager
     *
     * @since S60 v3.2
     * @return None
     */
    void Reset();
    
    /**
     * From CPeninputDataMgr
     * get pen color according to the change of theme
     *
     * @since S60 v3.2
     * @return None
     */    
    TRgb GetPenTrailColor();
  
protected:

    /**
     * Constructor
     *
     * @since S60 v3.2
     * @param aContext The layout context
     * @return None
     */
    CPeninputHwrBxAbDataMgr( MPeninputLayoutContext* aContext );
    
    /**
     * Transfer the pen's writing speed
     *
     * @since S60 v3.2
     * @param aWritingSpeed The value of pen's writing speed
     * @return TInt value to indicating writing speed
     */
    TInt TransferWritingSpeed( TInt aWritingSpeed );
    
    /**
     * Notify HWR's engine to change the setting
     *
     * @since S60 v3.2
     * @param aType The key whose value will be set
     * @param aData The value will be set
     * @return None
     */
    void NotifyEngine( TPeninputDataType aType, TAny* aData );
    
    TAny* RequestData( TPeninputDataType aDataType );
    
    void SetData( TPeninputDataType aDataType, TAny* aData );
    
private:
    
    /**
     * The HWR-pen's data
     */
    THandWritingPenData iPenData;
     
    /**
     * The pti engine
     * Ownership is deternimed by iOwnPtiEngine
     */    
    CPtiEngine* iPtiEngine;
    
    /**
     * The pti engine ownership flag
     */    
    TBool iOwnPtiEngine;
    
    TSize    iInputArea;
    
    TInt iHelpLine;
    
    TInt iBaseLine;
    };

#endif // C_PENINPUTGENERICHWRDATAMGR_H
