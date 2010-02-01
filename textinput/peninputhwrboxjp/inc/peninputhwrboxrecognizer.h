/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  peninput handwriting recognition class
*
*/


#ifndef C_PENINPUTHWRBOXRECOGNIZER_H
#define C_PENINPUTHWRBOXRECOGNIZER_H

//  INCLUDES
#include <eikon.hrh>  // TAknEditorNumericKeymap

class CPtiEngine;

/**
 *  CPeninputHwrBoxRecognizer
 *
 *  @lib peninputhwrboxjp.lib
 *  @since S60 v3.2
 */
class CPeninputHwrBoxRecognizer: public CBase
    {
    
public:

    /**
     * Symbian constructor.
     *
     * @since S60 v3.2
     * @return Pointer to created CPeninputHwrBoxRecognizer object
     */
    static CPeninputHwrBoxRecognizer* NewL(CPtiEngine* aPtiEngine);

    /**
     * destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CPeninputHwrBoxRecognizer();

    /**
     * Do recoginize by engine
     *
     * @since S60 v3.2
     * @param aTraceData The trace data.
     * @param aCandidates The candidates recognized.
     * @param aRangeStartPos The range index of candidates.
     * @return None
     */
    void DoRecognize(const RArray<TPoint>& aTraceData, RPointerArray<HBufC>& aCandidates, RArray<TInt>& aRangeStartPos);

    /**
     * Notify hwr engine to set primary range and auxiliary ranges
     *
     * @since S60 v3.2
     * @param aPermittedRanges The permitted ranges and the first range is primary range.
     * @return None
     */
    void SetRanges(const RArray<TInt>& aPermittedRanges);

    /**
     * Set case
     *
     * @since S60 v3.2
     * @param aCase The case
     * @return None
     */
    void SetCase(TInt aCase);

    /**
     * Set number mode
     *
     * @since S60 v3.2
     * @param aNumberMode Given number mode
     * @return None
     */
    void SetNumberMode(const TAknEditorNumericKeymap& aNumberMode);

    /**
     * Get stroke end mark from hwr engine
     *
     * @since S60 v3.2
     * @return Stroke end mark
     */
    TPoint StrokeEndMark() const ;

    /**
     * Set primary candidate num that hwr engine should return each time
     *
     * @since S60 v3.2
     * @param aNum The primary candidate num to be set
     * @return KErrNone if operation successfully,otherwise system wide error codes
     */
    TInt SetPrimaryCandidateNum(TInt aNum);

    /**
     * Set total candidate number that can be shown
     *
     * @since S60 v3.2
     * @param aNum The total candidate number that should be shown
     * @return KErrNone if operation successfully,otherwise system wide error codes
     */
    TInt SetCandidateNum(TInt aNum);

    /**
     * Set language
     *
     * @since S60 v3.2
     * @param aLanguage The language
     * @return None
     */
    void SetLanguageL(TInt aLanguage);

    /**
     * Set writing area size
     *
     * @since S60 v3.2
     * @param aSize Size of input area
     * @return None
     */
    void SetInputAreaSize( TSize aSize );

private:
    
    /**
     * C++ constructor
     *
     * @since S60 v3.2
     * @return None
     */
    CPeninputHwrBoxRecognizer();

    /**
     * Second phase constructor
     *
     * @since S60 v3.2
     * @return None
     */
    void ConstructL(CPtiEngine* aPtiEngine);

    /**
     * Notify hwr engine to set correct range by language
     *
     * @since S60 v3.2
     * @param aRange Current range need to be set
     * @param aRecognitionRange Recognition range need to be set for hwr engine
     * @return None
     */
    void SetRecognitionRange(const TInt aRange, TRecognitionRange& aRecognitionRange) const;

private: //data

    /**
     * Engine
     * Not Own
     */
    CPtiEngine* iPtiEngine;

    /**
     * Hwr recognizer (Not own)
     */
    MPtiHwrRecognizer* iRecognizer;

    /**
     * Total candidate number that can be shown
     */
    TInt iTotalCandidateNum;

    /**
     * Current case
     */
    TInt iCase;

    /**
     * Current language
     */
    TInt iLanguage;

    /**
     * Range count
     */
    TInt iRangeCount;

    /**
     * Premary range
     */
    TInt iPremaryRange;
    };

#endif   // C_PENINPUTHWRBOXRECOGNIZER_H

//End Of File
