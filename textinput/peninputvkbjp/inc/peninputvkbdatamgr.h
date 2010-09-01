/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface of vkb ui state
*
*/


#ifndef C_PENINPUTVKBDATAMGR_H
#define C_PENINPUTVKBDATAMGR_H

// System includes
#include <peninputdatamgr.h>

// User includes

// Forward decalaration
class MPeninputLayoutContext;
class CPtiEngine;

// class declarations
/**
 *  Data manager class
 *
 *  @lib peninputvkbjp.lib
 *  @since S60 v3.2
 */
class CPeninputVkbDataMgr: public CPeninputDataMgr
    {
public:
    /**
     * Create one CPeninputVkbDataMgr object
     *
     * @since S60 v3.2
     * @param aContext The layout context
     * @return CPeninputVkbDataMgr object.
     */
    static CPeninputVkbDataMgr* NewL(MPeninputLayoutContext* aContext);

    /**
     * Create one CPeninputVkbDataMgr object
     *
     * @since S60 v3.2
     * @param aContext The layout context
     * @return The CPeninputVkbDataMgr object.
     */
    static CPeninputVkbDataMgr* NewLC(MPeninputLayoutContext* aContext);

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None.
     */
    virtual ~CPeninputVkbDataMgr();

    /**
     * From CPeninputDataMgr
     * instesd of "void InitMore();"
     *
     * @since S60 v3.2
     * @return None
     */
    void InitMoreJpL();


    /**
     * Get PtiEngine
     *
     * @since S60 v3.2
     * @return ptiengine.
     */
    CPtiEngine* PtiEngine();

    /**
     * Get Predictive flag
     *
     * @since S60 v3.2
     * @return The predictive flag.
     */
    TBool Predictive() const;

    /**
     * Get full or half flag
     *
     * @since S60 v3.2
     * @return The full or half flag.
     */
    TInt FullHalf() const;

    /**
     * Get japanese flags
     *
     * @since S60 v3.2
     * @return japanese flags.
     */
    TInt JapaneseFlags() const;

    /**
     * Set permitted range
     *
     * @since S60 v3.2
     * @param aRange The new value
     * @return None.
     */
    void SetPermittedRange(TInt aRange);

    /**
     * Set ptiEngine InputMode by new range
     *
     * @since S60 v3.2
     * @param aRange The new Range
     * @return None.
     */
    void SetEngineInputMode(TInt aRange);

    /**
     * Set predictive
     *
     * @since S60 v3.2
     * @param aPredictive ETrue:show prediction pane   EFalse:not show
     * @return None.
     */
    void SetPredictive(TBool aPredictive);

    /**
     * Set full or half
     *
     * @since S60 v3.2
     * @param aFullHalf 0:half 1:full
     * @return None.
     */
    void SetFullHalf(TInt aFullHalf);

    /**
     * Set backspace or delete
     *
     * @since S60 v3.2
     * @param aClear 0:backspace 1:delete
     * @return None.
     */
    void SetClear(TInt aClear);

    /**
     * Set JapaneseFlags
     *
     * @since S60 v3.2
     * @param aJapaneseFlags
     * @return None.
     */
    void SetJapaneseFlags(TInt aJapaneseFlags);

    /**
     * kana : handle dakuten hanndakuten little
     *
     * @since S60 v3.2
     * @param aEventData The event data
     * @param aPreChar The previous character
     * @param aStatus ETrue:handled   EFalse:not handled
     * @param aSendText 1:Send Text
     */
    void HandleFunctionalVkbEventL(const TDesC& aEventData, TUint aPreChar, TBool& aStatus, TInt& aSendText);

    /**
     * half katakana : handle dakuten hanndakuten
     *
     * @since S60 v3.2
     * @param aEventData The event data(dakuten or hanndakuten)
     * @param aPreChar The previous character
     * @param aPrePreChar The previous previous character
     * @param aStatus ETrue:handled   EFalse:not handled
     * @param aSendText 1:Send Text
     */
    void HandleFunctionalVkbEventWithPrePreCharL(const TDesC& aEventData, TUint aPreChar, TUint aPrePreChar, TBool& aStatus, TInt& aSendText);

    /**
     * kana : handle touten,kuten
     *
     * @since S60 v3.2
     * @param anewChar initial/converted character
     * @return ETrue:converted   EFalse:not converted
     */
    TBool ConvertCommaFullStop(TUint& aNewChar) const;

    /**
     * Clear direction
     *
     * @since S60 v3.2
     * @return TInt 1:delete 0:backspace
     */
    TInt ClearDirection() const;

protected:

    /**
     * Constructor
     *
     * @since S60 v3.2
     * @param aContext The layout context
     * @return None.
     */
    CPeninputVkbDataMgr(MPeninputLayoutContext* aContext);

    /**
     * 2nd constructor function
     *
     * @since S60 v3.2
     * @return None.
     */
    void ConstructL();

private: // Data

    /**
     * The pti engine
     * Own
     */
    CPtiEngine* iPtiEngine;

    /**
     * prediction pane on/off
     */    
    TBool iPredictive;

    /**
     * 0:half 1:full
     */    
    TInt iFullHalf;

    /**
     * backspace or delete
     * 1:delete  0:Backspace
     */    
    TInt iClear;

    /**
     * Japanese Qwerty Comma ON(Japanese)/OFF(Western)
     * EPenInputJapaneseSettingQwertyComma        = 0x00000010,
     * Japanese Qwerty Period ON(Japanese)/OFF(Western)
     * EPenInputJapaneseSettingQwertyPeriod       = 0x00000020,
     * Japanese Qwerty Width Of Space ON(Full)/OFF(Half)
     * EPenInputJapaneseSettingQwertyWidthOfSpace = 0x00000040,
     */    
    TInt iJapaneseFlags;
    };


#endif // C_PENINPUTVKBDATAMGR_H

// End Of File
