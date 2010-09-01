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
* Description:  Data store
*
*/


#ifndef C_CAKNFEPHWRBXDATASTORE_H
#define C_CAKNFEPHWRBXDATASTORE_H

//  INCLUDES
#include <e32base.h>
#include <e32def.h>
#include <w32std.h>
#include <gdi.h>
#include <PtiEngine.h>
#include <eikon.hrh>

#include "peninputhwrpropertysubscriber.h"
#include <peninputpluginutils.h>

// constant definition
const TInt KMaxPredictiveLen = 128;
const TInt KMaxCandidateLen = 128;

const TInt KMaxPredictCandCnt = 250;


//const TUint16 KBackRep = 0x2408;
// convert displayable backspace
const TUint16 KBackRep = 0x2190;
const TUint16 KEnterRep = 0xF801;
const TUint16 KSpaceRep = 0xF800;
const TUint16 KEnterCode = 0x2029;

class CAknFepHwrTriggerStr;
class CRepository;
class CAknFepHwrEngine;
class CPtiEngine;

/**
 *  CAknFepHwrBxDataStore
 *
 *  @lib peninputhwrboxcn.lib
 *  @since S60 v3.2
 */
class CAknFepHwrBxDataStore : public CBase
    {
public:

    /**
     * Symbian constructor.
     *
     * @since S60 v3.2
     * @return Pointer to created CAknFepHwrBxDataStore object
     */
    static CAknFepHwrBxDataStore* NewL(CPtiEngine* aPtiEngine);

    /**
     * destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CAknFepHwrBxDataStore();

    /**
     * Is candidate in Chinese range
     *
     * @since S60 v3.2
     * @param aCandidateIndex The index the candidate in list.
     * @return ETrue if candidate is in Chinese range; otherwise EFalse
     */
    TBool IsChinese(TInt aCandidateIndex) const;
    
    /**
     * Do recoginize by engine
     *
     * @since S60 v3.2
     * @param aTraceData The trace data.
     * @return None
     */            
    void DoRecognizeL(const RArray<TPoint>& aTraceData);

    /**
     * Do predictive using trigger string
     *
     * @since S60 v3.2
     * @return None
     */            
    void DoPredictiveL();
    
    /**
     * Get the predictive count
     *
     * @since S60 v3.2
     * @return The count of predictives
     */            
    TInt PredictiveCount() const;

    /**
     * Get the candidate count
     *
     * @since S60 v3.2
     * @return The count of candidate
     */            
    TInt CandidateCount() const;

    /**
     * Add property subscriber
     *
     * @since S60 v3.2
     * @param aSubscriber The property subscriber.
     * @return None
     */            
    void AddSubscriber(MAknFepHwrPropertySubscriber* aSubscriber);

    /**
     * Get the trigger string
     *
     * @since S60 v3.2
     * @return Pointer to CAknFepHwrTriggerStr object
     */            
    CAknFepHwrTriggerStr* TriggerStr() const;

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
     * Set case
     *
     * @since S60 v3.2
     * @param aCase The case
     * @return None
     */            
    void SetCase(TInt aCase);
    
    /**
     * Set language
     *
     * @since S60 v3.2
     * @param aLanguage The language
     * @return None
     */            
    void SetLanguageL(TInt aLanguage);
    
    /**
     * Get language
     *
     * @since S60 v3.2
     * @return Input Language
     */            
    TInt InputLanguage();    
    
    /**
     * Set writting speed.
     *
     * @since S60 v3.2
     * @param aCharDelay The submit character timer in micro seconds
     * @return None
     */
    void SetWritingSpeed(TInt aWritingSpeed);

    /**
     * Set trail drawing pen size.
     *
     * @since S60 v3.2
     * @param aSize New pen size
     * @return None
     */
    void SetBoxPenSize(const TSize& aSize);

    /**
     * Set trail drawing pen color.
     *
     * @since S60 v3.2
     * @param aCol New pen color
     * @return None
     */
    void SetBoxPenColor(const TRgb& aCol);

    /**
     * Set the candidate list.
     *
     * @since S60 v3.2
     * @return None
     */
    void SetCandidates();

    /**
     * Set the predictive list.
     *
     * @since S60 v3.2
     * @return None
     */
    void SetPredictives();
    
    /**
     * Add new predictives to list
     *
     * @since S60 v3.2
     * @return None
     */
    void AddPredictives();

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
     * Get the predictive.
     *
     * @since S60 v3.2
     * @param aIndex The index of predictive
     * @param aCharCode The result predictive
     * @return Predictive char code
     */
    void GetPredictive(TInt aIndex, TDes& aCharCode) const;

    /**
     * Clear the candidate list.
     *
     * @since S60 v3.2
     * @return None
     */
    void ClearCandidates();
    
    /**
     * Set number mode
     *
     * @since S60 v3.2
     * @param aNumberMode Given number mode
     * @return None
     */
    void SetNumberMode(const TAknEditorNumericKeymap aNumberMode);
    
    /**
     * Set number mode
     *
     * @since S60 v3.2
     * @param aCustomMode The customized number key mapping, 0-9
     *                    are not included.
     * @return None
     */
    void SetNumberModeL(const TDesC& aCustomMode);

    /**
     * Load and publish default repository values
     *
     * @since S60 v3.2
     * @return None
     */
    void LoadAndPublishDefaultL();

    /**
     * Set the screen mode.
     *
     * @since S60 v3.2
     * @return None
     */
     void SetScreenMode(const TPixelsTwipsAndRotation& aScreenMode);

    /**
     * Set the screen mode.
     *
     * @since S60 v3.2
     * @param aBr The Bottom right point
     * @return None
     */
    void SaveWindowPos(const TPoint& aBr);

    /**
     * Set the screen mode.
     *
     * @since S60 v3.2
     * @param aRange The range
     * @return None
     */
    void SaveRange(TInt aRange);

    /**
     * Get window position
     *
     * @since S60 v3.2
     * @return The bottom-right point of layout from repository
     */            
    TPoint GetWindowPos(); 
       
    /**
     * Callback method to notify client about change.
     *
     * @since S60 v3.2
     * @param aPtr The callback data
     * @param aSetting The setting Uid
     * @return None
     */
    static TInt HandleFepRepositoryCallBack(TAny* aPtr);
    
    /**
     * Callback method to notify client about change.
     *
     * @since S60 v3.2
     * @param aPtr The callback data
     * @return None
     */
    static TInt HandleGSRepositoryCallBack(TAny* aPtr);

    /**
     * Set permitted ranges
     *
     * @since S60 v3.2
     * @param aPermittedRanges The permitted ranges.
     * @return None
     */            
    void SetPermittedCases(TInt aPermittedCases);
    TInt PermittedCases();
    
    /**
     * Get next predictive page
     *
     * @since S60 v3.2
     * @return None
     */            
    void GetNextPredictivePage();
    
    /**
     * Judge if next predictive candidate exist
     *
     * @since S60 v3.2
     * @return ETrue if candidate exist, otherwise EFalse;
     */            
    TBool PredictiveCandidateExisted();
    
    /**
     * Set stroke end mark to stroke end mark control
     *
     * @since S60 v3.2
     * @return None
     */
    void SetStrokeEndMark();
    
    /**
     * Set control which handle stroke end mark info
     *
     * @since S60 v3.2
     * @aStrokeControl The control that handle stroke end mark info
     * @return None
     */
    void SetStrokeEndMarkControl(MAknFepHwrPropertySubscriber* aStrokeControl);
    
    /**
     * Get stroke end mark from control which handle it
     *
     * @since S60 v3.2
     * @return None
     */
    TPoint StrokeEndMarkFromControl();
    
    /**
     * Judge if current permitted range is only number range
     *
     * @since S60 v3.2
     * @return ETrue f current permitted range is only number range
     */
    TBool IsNumberOnly();

    /**
     * Set flag to indicate whether size is changing
     *
     * @since S60 v3.2
     * @param aFlag ETrue means size is changing, otherwise EFalse
     * @return None
     */            
    void SetSizeChanging(TBool aFlag);
    
    /**
     * Get flag of whether size is changing
     *
     * @since S60 v3.2
     * @return ETrue if size is changing, otherwise EFalse
     */            
    TBool SizeChanging();
    
    /**
     * Get pen trail color
     *
     * @since S60 v3.2
     */        
    TInt GetPenTrailColor();
    TInt SetInputAreaSize(TSize& aSize);
    TInt SetScreenSize(TSize& aSize);
private:  

    /**
     * C++ constructor
     *
     * @since S60 v3.2
     * @return None
     */            
    CAknFepHwrBxDataStore();
    
    /**
     * Second phase constructor
     *
     * @since S60 v3.2
     * @return None
     */
    void ConstructL(CPtiEngine* aPtiEngine);

    /**
     * Notify subscriber
     *
     * @since S60 v3.2
     * @param aPropertyName The property name
     * @param aValue The new value
     * @return None
     */            
    void NotifySubscriber(MAknFepHwrPropertySubscriber::TAknFepHwrProperty aPropertyName, 
                          const TDesC& aValue);

    /**
     * Get new value from cenrep of aId.
     *
     * @since S60 v3.2
     * @param aWatcher The watcher to read
     * @param aId The Uid
     * @return None
     */
    TInt GetNewValue(CAknFepRepositoryWatcher* aWatcher, TInt aId);
    
    /**
     * Judge if a language is Chinese
     *
     * @since S60 v3.2
     * @param aLanguage Given language code
     * @return ETrue if aLanguage is Chinese, otherwise EFalse
     */
    TBool LanguageIsChinese(TInt aLanguage);
    
    /**
     * Set engine language
     *
     * @since S60 v3.2
     * @param aLanguage Given language code
     * @return None
     */
    void SetEngineLanguage(TInt aLanguage);

    void ClearExtCandidate();
private:// data
    
    /**
     * Subscriber List
     */
    RPointerArray<MAknFepHwrPropertySubscriber> iSubscriberList;
    
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
     * The screen mode setting id;
     */
    TUint32 iSettingId;

    /**
     * The repository watcher(Own);
     */
    CAknFepRepositoryWatcher* iFepRepositoryWatcher;

    /**
     * The repository watcher(Own);
     */    
    CAknFepRepositoryWatcher* iGSRepositoryWatcher;
    
    /**
     * The Bottom-right position of layout;
     */    
    TPoint iWindowPosition;
    
    TInt iPermittedCases;
    
    MAknFepHwrPropertySubscriber* iStrokeControl;

    TBool iSizeChanging;
 };

#endif // C_CAKNFEPHWRBXDATASTORE_H

// End Of File
