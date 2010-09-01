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
* Description:  peninput HWR data manager
*
*/


#ifndef C_PENINPUTHWRBOXDATAMGR_H
#define C_PENINPUTHWRBOXDATAMGR_H

// System includes
#include <peninputdatamgr.h>

const TInt KCharacterRangePopupMaxRow = 4;

// Forward decalaration
class MPeninputLayoutContext;
class CPeninputHwrBoxRecognizer;
class CPtiEngine;
/**
 *  Peninput HWR data management class
 *  This class manager the data of HWR
 *
 *  @lib peninputhwrboxjp.lib
 *  @since S60 v3.2
 */
class CPeninputHwrBoxDataMgr : public CPeninputDataMgr
    {
public:
    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aContext The layout context
     * @return The pointer to CPeninputHwrBoxDataMgr object
     */
    static CPeninputHwrBoxDataMgr* NewL(MPeninputLayoutContext* aContext);

    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aContext The layout context
     * @return The pointer to CPeninputHwrBoxDataMgr object
     */
    static CPeninputHwrBoxDataMgr* NewLC(MPeninputLayoutContext* aContext);

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CPeninputHwrBoxDataMgr();

    /**
     * Do recoginize by engine
     *
     * @since S60 v3.2
     * @param aTraceData The trace data.
     * @return None
     */
    void DoRecognize(const RArray<TPoint>& aTraceData);

    /**
     * Get the candidate count
     *
     * @since S60 v3.2
     * @return The count of candidate
     */
    TInt CandidateCount() const;

    /**
     * Set permitted ranges
     *
     * @since S60 v3.2
     * @param aPermittedRanges The permitted ranges.
     * @return None
     */
    void SetPermittedRanges(TInt aPermittedRanges);

    /**
     * Get the permitted ranges count
     *
     * @since S60 v3.2
     * @return The count of permitted ranges
     */
    TInt PermittedRangesCount() const;

    /**
     * Set premary range
     *
     * @since S60 v3.2
     * @param aPremaryRange The premary range
     * @return None
     */
    void SetPremaryRange(TInt aPremaryRange);

    /**
     * Set the candidate list.
     *
     * @since S60 v3.2
     * @return None
     */
    void SetCandidates();

    /**
     * Get WritingSpeed
     *
     * @since S60 v3.2
     * @return WritingSpeed.
     */
    TInt WritingSpeed() const;

    /**
     * Get PenSize
     *
     * @since S60 v3.2
     * @return PenSize.
     */
    TSize PenSize() const;

    /**
     * Get PenColor
     *
     * @since S60 v3.2
     * @return PenColor.
     */
    TRgb PenColor() const;

    /**
     * Get the candidate.
     *
     * @since S60 v3.2
     * @param aIndex The index of candidate
     * @param aCharCode The result candidate
     * @return Candidate char code
     */
    void GetCandidate(TInt aIndex, TDes& aCharCode) const;
    /**
     * Clear the candidate list.
     *
     * @since S60 v3.2
     * @return None
     */
    void ClearCandidates();

// From base class CPeninputDataMgr

    /**
     * From CPeninputDataMgr
     * Initialize the pen data
     * instesd of "void InitMore();"
     *
     * @since S60 v3.2
     * @return None
     */
    void InitMoreJpL();

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
     * Get Conversion flag
     *
     * @since S60 v3.2
     * @return The Conversion flag.
     */
    TBool Conversion() const;

    /**
     * Get The Character range popup text array
     *
     * @since S60 v3.2
     * @return ptiengine.
     */
    CDesCArrayFlat* CharacterRangeArray();

    /**
     * Get The Character range Id from popup index
     *
     * @since S60 v3.2
     * @return Character range Id.
     */
    TInt CharacterRangeId() const;

    /**
     * Get CharacterRange Popup index
     *
     * @since S60 v3.2
     * @param aIndex
     * @return Character Popup index.
     */
    TInt CurrentCharacterRangeIndex() const;

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
     * Set Conversion Flag
     *
     * @since S60 v3.2
     * @param aConversion ETrue:Convert EFalse:not Convert
     * @return None.
     */
    void SetConversion(TBool aConversion);

    /**
     * Set CharacterRange Popup index
     *
     * @since S60 v3.2
     * @param aIndex
     * @return None.
     */
    void SetCurrentCharacterRangeIndex(TInt aIndex);

    /**
     * From CPeninputDataMgr
     * Reset data contained in data manager
     *
     * @since S60 v3.2
     * @return None
     */
    void Reset();

    /**
     * Set writing area size
     *
     * @since S60 v3.2
     * @param aSize Size of input area
     * @return None
     */
    void SetInputAreaSize( const TSize& aSize );


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
     * @return None
     */
    CPeninputHwrBoxDataMgr( MPeninputLayoutContext* aContext );

    /**
     * 2nd constructor function
     *
     * @since S60 v3.2
     * @return None.
     */
    void ConstructL();

    /**
     * Transfer the pen's writing speed
     *
     * @since S60 v3.2
     * @param aWritingSpeed The value of pen's writing speed
     * @return TInt value to indicating writing speed
     */
    TInt TransferWritingSpeed( TInt aWritingSpeed ) const;

    /**
     * Notify HWR's engine to change the setting
     *
     * @since S60 v3.2
     * @param aType The key whose value will be set
     * @param aData The value will be set
     * @return None
     */
    void NotifyEngine( TPeninputDataType aType, TAny* aData );

    /**
     * Set Character range popup
     *
     * @since S60 v3.2
     * @param aPermittedRanges The permitted ranges.
     * @return None
     */
    void SetCharacterRangePopupL(TInt aPermittedRanges);

private:

    /**
     * candidate list
     */
    RPointerArray<HBufC> iCandidates;

    /**
     * The candidate range start index
     */
    RArray<TInt> iRangeStartPos;

    /**
     * Permitted ranges
     */
    RArray<TInt> iPermittedRanges;

    /**
     * The pti engine
     * Own
     */
    CPtiEngine* iPtiEngine;

    /**
     * The Recognizer (Own);
     */
    CPeninputHwrBoxRecognizer* iHwrBoxRecognizer;

    /**
     * Writing speed
     */
    TInt iWritingSpeed;

    /**
     * Pen size
     */
    TSize iPenSize;

    /**
     * Pen color
     */
    TRgb iPenColor;

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
     * Conversion on/off
     */
    TBool iConversion;

    /**
     * The Character range popup text array
     * Own
     */
    CDesCArrayFlat* iCharacterRangeArray;

    /**
     * The Character range popup rangeId array
     */
    TFixedArray<TInt, KCharacterRangePopupMaxRow> iCharacterRangeIdArray;

    /**
     * CharacterRange Popup index(default:0)
     */
    TInt iCurrentCharacterRangeIndex;
    };

#endif // C_PENINPUTHWRBOXDATAMGR_H
