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


#ifndef __PENINPUTHWRFSCNDATASTORE_H__
#define __PENINPUTHWRFSCNDATASTORE_H__

//SYSTEM INCLUDES
#include <e32base.h>
#include <e32def.h>
#include <w32std.h>
#include <eikon.hrh>
#include <AknUtils.h>

//FEP INCLUDES
#include <PtiEngine.h>
#include <peninputpluginutils.h>

//USER INCLUDES
#include "peninputhwrfscnpropertysubscriber.h"

//CONSTANTS
const TInt KLanguageStringLength = 10;

//FOWWARD CLASS
class CAknFepHwrTriggerStr;
class CRepository;
class CAknFepHwrEngine;
class CPtiEngine;

/**
 *  CPeninputHwrfscnDataStore
 *
 *  @lib peninputhwrfscn.lib
 *  @since S60 v5.0
 */
class CPeninputHwrfscnDataStore : public CBase
    {
public:

    /**
     * Symbian constructor.
     *
     * @since S60 v5.0
     * @return Pointer to created CPeninputHwrfscnDataStore object
     */
    static CPeninputHwrfscnDataStore* NewL( CPtiEngine* aPtiEngine );

    /**
     * destructor
     *
     * @since S60 v5.0
     * @return None
     */
    virtual ~CPeninputHwrfscnDataStore();

public:
    /**
    * The writing speed.
    *
    *@since Series 60 5.0
    */		
    enum TWritingSpeed
        {
        EWritingSpeedVeryFast = 1,
        EWritingSpeedFast,
        EWritingSpeedNormal,
        EWritingSpeedSlow,
        EWritingSpeedVerySlow
        };

public:
    /**
     * Is candidate in Chinese range
     *
     * @since S60 v5.0
     * @param aCandidateIndex The index the candidate in list.
     * @return ETrue if candidate is in Chinese range; otherwise EFalse
     */
    TBool IsChinese( TInt aCandidateIndex ) const;
    
    TBool IsValid( TInt aCandidateIndex ) const;    
    /**
     * Do recoginize by engine
     *
     * @since S60 v5.0
     * @param aTraceData The trace data.
     * @return None
     */            
    void DoRecognizeL( const RArray<TPoint>& aTraceData );

    /**
     * Do predictive using trigger string
     *
     * @since S60 v5.0
     * @return None
     */            
    void DoPredictiveL();
    
    /**
     * Do homophonic using trigger string
     *
     * @since S60 v5.0
     * @return None
     */            
    void DoHomophonicL(  const TDesC& aString  );
       
    /**
     * Get the predictive count
     *
     * @since S60 v5.0
     * @return The count of predictives
     */            
    TInt PredictiveCount() const;

    /**
     * Add property subscriber
     *
     * @since S60 v5.0
     * @param aSubscriber The property subscriber.
     * @return None
     */            
    void AddSubscriber( MPeninputHwrfscnPropertySubscriber* aSubscriber );

    /**
     * Get the trigger string
     *
     * @since S60 v5.0
     * @return Pointer to CAknFepHwrTriggerStr object
     */            
    CAknFepHwrTriggerStr* TriggerStr() const;

    /**
     * Set permitted ranges
     *
     * @since S60 v5.0
     * @param aPermittedRanges The permitted ranges.
     * @return None
     */            
    void SetPermittedRanges( TInt aPermittedRanges );
   
   /**
     * Get the permitted ranges count
     *
     * @since S60 v5.0
     * @return The permitted ranges array.
     */ 
    const RArray<TInt>& PermittedRanges() const;
   
    /**
     * Set Primary range
     *
     * @since S60 v5.0
     * @param aPrimaryRange The Primary range
     * @return None
     */            
    void SetPrimaryRange( TInt aPrimaryRange );
    
    /**
     * Get Primary range
     *
     * @since S60 v5.0
     * @return Primary range.
     */
    TInt PrimaryRange();
    
    /**
     * Check if the primary range is chinese
     *
     * @since S60 v5.0
     * @return ETrue if the primary range is chinese, EFalse if otherwise
     */
     TBool IsChinesePrimaryRange();
     
    /**
     * Set case
     *
     * @since S60 v5.0
     * @param aCase The case
     * @return None
     */            
    void SetCase( TInt aCase );
    
    /**
     * Set language
     *
     * @since S60 v5.0
     * @param aLanguage The language
     * @return None
     */            
    void SetLanguageL( TInt aLanguage );
    
    /**
     * Get language
     *
     * @since S60 v5.0
     * @return Current Language
     */            
    TInt CurLanguage();    
    
    /**
     * Set writting speed.
     *
     * @since S60 v5.0
     * @param aCharDelay The submit character timer in micro seconds
     * @return None
     */
    void SetWritingSpeed( TInt aWritingSpeed );
    
    /**
     * Get writting speed.
     *
     * @since S60 v5.0
     * @return The writing speed
     */
    TTimeIntervalMicroSeconds32 WritingSpeed();
    
    /**
     * Set trail drawing pen size.
     *
     * @since S60 v5.0
     * @param aSize New pen size
     * @return None
     */
    void SetBoxPenSize( const TSize& aSize );
    
    /**
     * Get trail drawing pen size.
     *
     * @since S60 v5.0
     * @return Pen size.
     */
    TSize PenSize();

    /**
     * Set trail drawing pen color.
     *
     * @since S60 v5.0
     * @param aCol New pen color
     * @return None
     */
    void SetBoxPenColor( const TRgb& aCol );
    
    /**
     * Get trail drawing pen color.
     *
     * @since S60 v5.0
     * @return The pen color
     */
    TRgb& PenColor();
    
    /**
     * Set the candidate list.
     *
     * @since S60 v5.0
     * @return None
     */
    void SetCandidatesL();

    /**
     * Set homephonic candidates.
     *
     *
     */
    void SetHomephonicCandidatesL();
   
    /**
     * Set the predictive list.
     *
     * @since S60 v5.0
     * @return None
     */ 
    void SetPredictives();
    
    /**
     * Add new predictives to list
     *
     * @since S60 v5.0
     * @return None
     */
    void AddPredictives();
    
    /**
     * Get the candidate.
     *
     * @since S60 v5.0
     * @param aIndex The index of candidate
     * @param aCharCode The result candidate
     * @return Candidate char code
     */
    const RPointerArray<HBufC>& Candidate();
    
    /**
     * Get the predictive.
     *
     * @since S60 v5.0
     * @param aIndex The index of predictive
     * @param aCharCode The result predictive
     * @return Predictive char code
     */
    const RPointerArray<HBufC>& Predictive();
    
    /**
     * Set number mode
     *
     * @since S60 v5.0
     * @param aNumberMode Given number mode
     * @return None
     */
    void SetNumberMode( const TAknEditorNumericKeymap aNumberMode );
    
    /**
     * Set number mode
     *
     * @since S60 v5.0
     * @param aCustomMode The customized number key mapping, 0-9
     *                    are not included.
     * @return None
     */
    void SetNumberModeL( const TDesC& aCustomMode );

    /**
     * Load and publish default repository values
     *
     * @since S60 v5.0
     * @return None
     */
    void LoadAndPublishDefaultL();

    /**
     * Set the screen mode.
     *
     * @since S60 v5.0
     * @param aRange The range
     * @return None
     */
    void SaveRange( TInt aRange );
    
    /**
     * Callback method to notify client about change.
     *
     * @since S60 v5.0
     * @param aPtr The callback data
     * @return None
     */
    static TInt HandleGSRepositoryCallBack( TAny* aPtr );

    /**
     * Set permitted ranges
     *
     * @since S60 v5.0
     * @param aPermittedRanges The permitted ranges.
     * @return None
     */            
    void SetPermittedCases( TInt aPermittedCases );
    
    /**
     * Get next predictive page
     *
     * @since S60 v5.0
     * @return None
     */            
    void GetNextPredictivePageL();
    
    /**
     * Judge if next predictive candidate exist
     *
     * @since S60 v5.0
     * @return ETrue if candidate exist, otherwise EFalse;
     */            
    TBool PredictiveCandidateExisted();
    
    /**
     * Set stroke end mark to stroke end mark control
     *
     * @since S60 v5.0
     * @return None
     */
    void SetStrokeEndMark();
    
    /**
     * Get the stroke end mark.
     *
     * @since S60 v5.0
     * @return TPoint
     */
    TPoint StrokeEndMark();
   
    /**
     * Get stroke end mark from control which handle it
     *
     * @since S60 v5.0
     * @return None
     */
    TPoint StrokeEndMarkFromControl();
    
    /**
     * Judge if current permitted range is only number range
     *
     * @since S60 v5.0
     * @return ETrue f current permitted range is only number range
     */
    TBool IsNumberOnly();
    
    /**
     * Judge if a language is TW language or PRC language
     *
     * @since S60 v5.0
     * @return ETrue if aLanguage is TW or PRC, otherwise EFalse
     */
    TBool IsHomephonicLanguage();

    /**
     * Get number keymapping string
     *
     * @since S60 v5.0
     * @param aKeyMappingId Keymapping id
     * @return Pointer to keymapping string 
     */    
    HBufC* KeyMappingStringL() const;
    
    /**
     * Check if the char is the special display char
     * 
     * @since S60 v5.0
     * @param aChar The checked char
     * @return TBool Wheater or not the input char is the special display char
     */
    TBool IsSpecialDisplayChars( const TDesC& aChar );
     
    /**
     * Check if the char is need to be sent to app directly which not need to be shown  in candidate window.
     * 
     * @since S60 v5.0
     * @param aChar The checked char
     * @return TBool Wheater or not the input char is the directly sent char
     */
    TBool IsDirectlySentCandidate( const TDesC& aChar );
    
    /**
     * Convert the special display char to the keycode.
     *
     * @since S60 v5.0
     * @param aChar The special display char.
     * @return HBufC* The converted keycode char.
     */
    HBufC* ConvertDisplayChars( const TDesC& aChar );

    /**
     * Get language show text
     *
     * @since S60 v5.0
     * @return TDesC* language show text.
     */
    const TDesC& LanguageShowText();
    
    /**
     * Get pen trail color
     *
     * @since S60 v5.0
     * @return TDesC* language show text.
     */    
    TInt GetPenTrailColor();
    TInt SetInputAreaSize(TSize& aSize);    
    TInt SetScreenSize(TSize& aSize);    
private:  

    /**
     * C++ constructor
     *
     * @since S60 v5.0
     * @return None
     */            
    CPeninputHwrfscnDataStore();
    
    /**
     * Second phase constructor
     *
     * @since S60 v5.0
     * @return None
     */
    void ConstructL( CPtiEngine* aPtiEngine );
   
   /**
     * Initialize the key mapping list
     *
     * @since S60 v5.0
     * @return None
     */  
   void InitKeyMappingListL();    

    /**
     * Notify subscriber
     *
     * @since S60 v5.0
     * @param aPropertyName The property name
     * @param aValue The new value
     * @return None
     */            
    void NotifySubscriber( MPeninputHwrfscnPropertySubscriber::
        TPeninputHwrfscnProperty aPropertyName );

    /**
     * Get new value from cenrep of aId.
     *
     * @since S60 v5.0
     * @param aWatcher The watcher to read
     * @param aId The Uid
     * @return None
     */
    TInt GetNewValue( CAknFepRepositoryWatcher* aWatcher, TInt aId );
    
    /**
     * Judge if a language is Chinese
     *
     * @since S60 v5.0
     * @param aLanguage Given language code
     * @return ETrue if aLanguage is Chinese, otherwise EFalse
     */
    TBool LanguageIsChinese( TInt aLanguage );
    
    /**
     * Set engine language
     *
     * @since S60 v5.0
     * @param aLanguage Given language code
     * @return None
     */
    void SetEngineLanguageL( TInt aLanguage );    
    
    /**
     * Set language show text
     *
     * @since S60 v5.0
     * @param aLanguage language code
     * @return None
     */
    void SetLanguageShowText( TInt aLanguage );
    
    /**
     * Get predictives when the primary range is chinese
     *
     * @since S60 v5.0
     *
     */
    void GetPredictivesOfPriRangeAsChineseL();
    
    /**
     * Get predictives when the primary range is number
     *
     * @since S60 v5.0
     *
     */
    void GetPredictivesOfPriRangeAsNumberL();
    
    /**
     * Get predictives when the primary range is english
     *
     * @since S60 v5.0
     */
    void GetPredictivesOfPriRangeAsEnglishL();
      
private:// data
    
    /**
     * Subscriber List
     */
    RPointerArray<MPeninputHwrfscnPropertySubscriber> iSubscriberList;
    
    /**
     * Trigger string (Own)
     */
    CAknFepHwrTriggerStr* iTriggerStr;
    
    /**
     * predictive list
     */
    RPointerArray<HBufC> iPredictives;

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
     * Permitted range in int format
     */
    TInt iIntRange;
    
    /**
     * Current case
     */
    TInt iCase;

    /**
     * Current language
     */
    TInt iLanguage;
    
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
     * The repository (Own);
     */
    CRepository* iRepositoryFep;
    
    /**
     * The repository (Own);
     */
    CRepository* iRepositorySetting;

    /**
     * The engine (Own);
     */
    CAknFepHwrEngine* iHwrEngine;

    /**
     * The repository watcher(Own);
     */    
    CAknFepRepositoryWatcher* iGSRepositoryWatcher;
    
    /**
     * The permited cases
     */
    TInt iPermittedCases;   
    
    /**
     * The storke control
     */
    MPeninputHwrfscnPropertySubscriber* iStrokeControl;
    
    /**
     * The current number key map
     */   
    TAknEditorNumericKeymap iCurrentNumberMode;
   
    /**
     * Keymapping id array
     */  
    RArray<TInt> iKeyMappingIdList;
    
    /**
     * Keymapping string array
     */          
    RPointerArray<HBufC> iKeyMappingStringList;

    /**
     * Language show text
     */
    TBuf<KLanguageStringLength> iLanguageShowText;
    
    /**
     * User defined key mapping (Own)
     */
    HBufC* iUserDefinedResource;
 };


#endif	//__PENINPUTHWRFSCNDATASTORE_H__