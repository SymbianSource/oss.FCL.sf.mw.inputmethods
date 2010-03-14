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
* Description:  HWR Engine
*
*/

#ifndef C_PENINPUTFINGERHWRENGINE_H
#define C_PENINPUTFINGERHWRENGINE_H

//  INCLUDES
#include <e32cmn.h>
#include <eikon.hrh>
#include <PtiEngine.h>

class CPeninputFingerHwrDataStore;

/**
 *  CAknFepHwrEngine
 *
 *  @lib peninputfingerhwr.lib
 *  @since S60 v3.2
 */
class CAknFepHwrEngine: public CBase
    {
public:

    /**
     * Symbian constructor.
     *
     * @since S60 v3.2
     * @return Pointer to created CAknFepHwrEngine object
     */
    static CAknFepHwrEngine* NewL( CPtiEngine* aPtiEngine, 
        CPeninputFingerHwrDataStore* aOwner );

    /**
     * destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CAknFepHwrEngine();

    /**
     * Do recoginize by engine
     *
     * @since S60 v3.2
     * @param aTraceData The trace data.
     * @param aCandidates The candidates recognized.
     * @param aRangeStartPos The range index of candidates.
     * @return None
     */            
    void DoRecognizeL( const RArray<TPoint>& aTraceData, RPointerArray<HBufC>& aCandidates );


    /**
     * Do predictive using trigger string
     *
     * @since S60 v3.2
     * @param aTriggerStr The trigger string to do predictive.
     * @param aPredictives The candidates predictived.
     * @param aNextPage Whether it is to get next page
     * @return None
     */            
    void DoPredictiveL( const TDesC& aTriggerStr, 
                        RPointerArray<HBufC>& aPredictives,
                        TBool aNextPage = EFalse );
     
#ifdef RD_INTELLIGENT_TEXT_INPUT     
    /**
     * Do Auto Complete using trigger string
     *
     * @since S60 v3.2
     * @param aTriggerStr The trigger string to do predictive.
     * @param aPredictives The candidates predictived.
     * @return None
     */
    void DoEngPredictiveL( const TDesC& aTriggerStr, 
                           RPointerArray<HBufC>& aPredictives);   
#endif
    
    /**
     * Notify hwr engine to set primary range and auxiliary ranges
     *
     * @since S60 v3.2
     * @param aPermittedRanges The permitted ranges and the first range is primary range.
     * @return None
     */
    void SetRanges( const RArray<TInt>& aPermittedRanges );
    
    /**
     * Notify hwr engine to set primary range and auxiliary ranges
     * 
     * @since S60 v3.2
     * @param aPermittedRanges The permitted ranges and the first range is primary range.
     * @return None
     */    
    void SetRangesL( const RArray<TInt>& aPermittedRanges );

    /**
     * Set case
     *
     * @since S60 v3.2
     * @param aCase The case
     * @return None
     */            
    void SetCase( const TInt aCase );

    /**
     * Set number mode
     *
     * @since S60 v3.2
     * @param aNumberMode Given number mode
     * @return None
     */
    void SetNumberMode( const TAknEditorNumericKeymap& aNumberMode );

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
    TInt SetPrimaryCandidateNum( const TInt aNum );

    /**
     * Set total candidate number that can be shown
     *
     * @since S60 v3.2
     * @param aNum The total candidate number that should be shown
     * @return None
     */
    void SetCandidatesMaxCount( const TInt aCount );       
    
    /**
     * Set language
     *
     * @since S60 v3.2
     * @param aLanguage The language
     * @return None
     */            
    void SetLanguageL( const TInt aLanguage );
    
    /**
     * Set custom key mapping
     *
     * @since S60 v3.2
     * @param aKeyMap The key mapping
     * @return None
     */            
    void SetCustomKeymapL( const TDesC& aKeyMap );
    
    /**
     * Clear custom key mapping
     *
     * @since S60 v3.2
     * @return None
     */            
    void ResetCustomKeyMap();
    
    /**
     * Do background construct
     *
     * @since S60 v3.2
     * @return None
     */            
    static TBool BackgroundTaskL( TAny* aPtr );
    
    /**
     * Set hand writing area size
     *
     * @since S60 v3.2
     * @param aSize The area size.
     * @return None
     */            
    TInt SetInputAreaSize(TSize& aSize);
    
    /**
     * Set hand screen size
     *
     * @since S60 v3.2
     * @param aSize The screen size.
     * @return None
     */            
    TInt SetScreenSize(TSize& aSize);
    
    void ResetKeyboardType();
    
    void SetKeyboardToQwerty();
    
    void GetKeyboardType();
    
private:
    
    /**
     * C++ constructor
     *
     * @since S60 v3.2
     * @return None
     */            
    CAknFepHwrEngine();

    /**
     * Second phase constructor
     *
     * @since S60 v3.2
     * @return None
     */
    void ConstructL( CPtiEngine* aPtiEngine, CPeninputFingerHwrDataStore* aOwner );
    
    /**
     * Notify hwr engine to set correct range by language
     *
     * @since S60 v3.2
     * @param aRange Current range need to be set
     * @param aRecognitionRange Recognition range need to be set for hwr engine
     * @return None
     */
    void SetRecognitionRange( const TInt aRange, TRecognitionRange& aRecognitionRange );
    
    /**
     * Do background construct
     *
     * @since S60 v3.2
     * @return None
     */            
    void DoIdleConstructL();
    
    /**
     * Convert the stroke end mark of point array to another point
     *
     * @since S60 v3.2
     * @param aTraceData Point array
     * @param aPnt1 Stroke end mark to be replaced
     * @param aPnt2 Stroke end mark to be used
     * @return None
     */
    void ConvertStrokeEndMark( RArray<TPoint>& aTraceData, TPoint aPnt1, TPoint aPnt2 );
     

private: //data

    /**
     * Engine
     * Ownership is determined by flag
     */
    CPtiEngine* iPtiEngine;
    
    /**
     * iPtiEngine ownership flag
     */
    TBool iOwnPtiEngine;

    /**
     * Hwr recognizer (Now own)
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

    /**
     * Custom key mapping
     */
    HBufC* iCustomKeymap;
    
    /**
     * Idle (own)
     */
    CIdle* iIdle;
    
    /**
     * Data store (Not own)
     */
    CPeninputFingerHwrDataStore* iOwner;
    
    /**
     * Set number mode flag
     */
    TBool iNeedSetNumberMode;
    
    /**
     * Set case flag
     */
    TBool iNeedSetCase;
    
    /**
     * Set range flag
     */
    TBool iNeedSetRange;
    
    /**
     * Permitted range array
     */
    RArray<TInt> iNeedPermittedRanges;
    
    /**
     * The number mode
     */
    TInt iNumberMode;
    
    TInt iKeyboardType;
    };

#endif //C_PENINPUTFINGERHWRENGINE_H

//End Of File
