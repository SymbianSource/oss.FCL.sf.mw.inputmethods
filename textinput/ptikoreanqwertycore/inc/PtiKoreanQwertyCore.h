/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/


#ifndef __PTIKOREANQWERTYCORE_H__
#define __PTIKOREANQWERTYCORE_H__

#include <e32base.h>	// CBase
#include <e32std.h>	 // TBuf

#include <PtiCore.h>
#include <osscombination.h>

class COssHangulInputContext;
class CRepository;
class CActiveCallback;

class CPtiKoreanQwertyCore : public CPtiCore
    {
public:
    static CPtiKoreanQwertyCore* NewLC(
        CPtiEngine* aOwner, 
        TDes* aTextBuffer);
    
    static CPtiKoreanQwertyCore* NewL(
        CPtiEngine* aOwner, 
        TDes* aTextBuffer);
    
    virtual ~CPtiKoreanQwertyCore();

private:
    
    CPtiKoreanQwertyCore(
        CPtiEngine* aOwner, 
        TDes* aTextBuffer);
    
    void ConstructL();
    
private: // from CPtiCore

    /**
    * Initialized core object. This method is called by PtiEngine framework when core object
    * is loaded.        
    *
    * @since 2.6
    * @param aOwner Pointer to onwing PtiEngine framework instance.
    * @param aTextBuffer A pointer to PtiEngine main text buffer.
    * @param aLanguageList A language list to be filled with language bindings to this core
    *                      onbject
    * @param aUseDefautUserDictionary A boolean value indicating whether default user dictionary shoulkd be used
    *                                 (in case this core object supports user dictionaries).
    */  
    virtual void InitializeL(
        CPtiEngine* aOwner, 
        TDes* aTextBuffer, 
        CArrayPtrFlat<CPtiCoreLanguage>* aLanguageList,
        TBool aUseDefautUserDictionary);
    
    /**
    * Removes languages associated to this core object from given language list. Languages
    * not associated to this core abject are not touched.
    * 
    * @since 2.6
    * @param aLanguageList A language list to be cleaned.
    */
    virtual void DropLanguages(CArrayPtrFlat<CPtiCoreLanguage>* aLanguageList);
    
    /**
    * Opens resources for given language and itinialized them.
    *
    * @since 2.6
    * @param aLanguage Symbian OS language code a language to be initialized.
    * @return KErrNone or a system wide error code.
    */
    virtual TInt OpenLanguageL(CPtiCoreLanguage *aLanguage);
    
    /**
    * Closes currently activate language and frees resources associated to it.
    *
    * @since 2.6
    * @return KErrNone of system wide error ocde.
    */
    virtual TInt CloseLanguageL();
    
    /**
    * Returns the contents of text buffer.
    * 
    * @since 2.6
    * @return A constant pointer to text buffer.
    */  
    virtual TPtrC GetCurrentWord() const;
    
    /**
    * Commits current word. Commiting means that user has added current
    * word as part of text. Frequency information and other related attributes
    * can then be updated.
    *
    * @since 2.6
    * @return KErrNone or a system wide error code.
    */
    virtual TInt CommitCurrentWord();       
    
    /**
    * Returns a pointer core info structure. Core info struct contains useful 
    * information about core object and its features.
    *
    */              
    virtual MPtiCoreInfo* GetCoreInfo();
            
    /**
    * Turns reoreding feature on or off. Reordering means that prediction engine
    * maintains frequency information for used words and modifys the order of candidate list
    * based on that information. This feature may not be supported by all prediction engines.
    * 
    * @since 2.6
    * @param aStatus New status for reorderinf feature.
    * @return KErrNone or a system wide error code.
    */
    virtual TInt SetReordering(TBool aStatus);
    
    /**
    * This is the single most important core API method, most of the text input functionality
    * will be utilize this method. Call to this method indicates core object that a key was
    * pressed and it should act accordingly and fill the output buffer.
    * 
    * @since 2.6
    * @param aKey Enum value for pressed key.
    */
    virtual void AppendKeyPress(TPtiKey aKey);
    
    /**
    * Indicates core object that it should perform 'delete key press'-operation.
    *
    * @since 2.6
    */
    virtual void DeleteKeyPress();
    
    /**
    * Call to this method notifys core object that input buffer was cleared.
    *
    * @since 2.6
    */
    virtual void ClearBuffer();
    
    /**
    * Sets current input sequence. The end result of this operation must be same as 
    * if user had entered keys in input array as individual key presses.
    *
    * @since 2.6
    * @param aInput An array containing sequence of key presses (type TPtiKey).
    * @return KErrNone or a system wide error code.
    */
    virtual TInt SetCurrentInputSequence(TPtrC8 aInput);
    
    /**
    * Converts given string from one character set to another. Possible conversion types
    * are defined in PtiDefs.h
    *
    * @since 2.6
    * @param aType        Conversion type.
    * @param aInput       Pointer to input string to be converted.
    * @param aInputLength Length of input buffer.
    * @param aOutput      pointer to output buffer.
    * @return KErrNone or a system wide error code.
    */
    virtual TInt Convert(
        TPtiCharConversion aType,
        TAny* aInput,
        TInt aInputLength,
        TAny* aOutput);

    /**
    * Re-defines keymappings for given key / input mode / case combination. Previous 
    * keymapping will be deleted and new mappings will be used instead.
    *
    * @since 2.6
    * @param aMode Input mode for new key mapping.
    * @param aKey  A key to be replaced.
    * @param aKeyMap New keymapping data for aKey.
    * @param aCase   Text case to be replaced.
    * @return KErrNone or system wide error code.
    */
    virtual TInt SetExternalKeyMapL(
        TPtiEngineInputMode aMode,
        TPtiKey aKey,
        TDesC& aKeyMap,
        TPtiTextCase aCase);
    /**
    * Returns alternate spelling for given unicode characters. Possible conversion
    * types are defined in header PtiDefs.h.
    *                                       
    * @sinec 2.6
    * @aInput A character to be converted.
    * @aOutput Resulting alternate spelling will be stored here.
    * @aType   Spelling conversion type.
    * @return  KErrNone or system wide error code.
    */                              
    virtual TInt GetSpelling(TUint16 aInput, TDes& aOutput, TPtiSpelling aType);
    
    /**
    * Cancels all on-going core obejct timner activity.
    *
    * @since 2.6
    * @return KErrNone or system wide error code.
    */
    virtual TInt CancelTimerActivity();
    
    /**
    * A generic command handling method for extending PtiEngine API without adding
    * new base class methods. It is recommended to use this only in force major
    * situations, where there is no other way.
    * 
    * @since 2.6
    * @param aCommand A Command id value. Platform wide command ids are defined in PtiDefs.h.
    * @param aParams  TAny pointer for passing any required parameters.
    * @return An integer containing possible return value related to command.
    */
    virtual TInt HandleCommandL(
        TPtiEngineCommand aCommand, 
        TAny* aParams);

//
// Predictive latin API
//  
private:    
    /**
    * Returns number of candidates for currently entered key sequence.
    *
    * @since 2.6
    * @return Number of prediction candidates for currently entered key sequence.
    */
    virtual TInt NumberOfCandidates();
    
    /**     
    * Returns a list of candidate words for currently entered key sequence.
    *
    * @since 2.6
    * @param aList A list to be filled with candidate words.
    * @return Number of candidate works in aList.
    */
    virtual TInt GetCandidateListL(CDesCArray& aList);
    
    /**
    * Returns next word candidate list. Next word candidates are words
    * potentially related to currently selected candidate. Next word prediction
    * feature must supported by the engine and turned on.
    *
    * @since 2.6
    */
    virtual TInt GetNextWordCandidateListL(CDesCArray& aList);
    
    /**
    * Returns first word in list of candidate words for current input sequence.
    * 
    * @since 2.6
    * @param aResult A descriptor where resulting word will be stored.
    * @return KErrnNone or system wide error code.
    */      
    virtual TInt GetFirstCandidate(TDes& aResult);
    
    /**
    * Returns next word in list of candidate words for current input sequence.
    * 
    * @since 2.6
    * @param aResult A descriptor where resulting word will be stored.
    * @return KErrnNone or system wide error code.
    */                      
    virtual TInt GetNextCandidate(TDes& aResult);
    
    /**
    * Returns previous word in list of candidate words for current input sequence.
    * 
    * @since 2.6
    * @param aResult A descriptor where resulting word will be stored.
    * @return KErrnNone or system wide error code.
    */                              
    virtual TInt GetPreviousCandidate(TDes& aResult);

//
// User dictionary API
//
private:
    virtual TInt AddUserDictionaryEntry(
        MPtiUserDictionaryEntry& aEntry, 
        CPtiUserDictionary* aDict);
    
    virtual TInt RemoveUserDictionaryEntry(
        MPtiUserDictionaryEntry& aEntry, 
        CPtiUserDictionary* aDict);
    
    virtual TInt NumberOfEntriesInUserDictionary(
        CPtiUserDictionary* aDict);
    
    virtual TInt GetUserDictionaryEntry(
        TInt aIndex, 
        MPtiUserDictionaryEntry& aResult, 
        CPtiUserDictionary* aDict);
    
    virtual CPtiUserDictionary* CreateEmptyUserDictionaryL(TInt aMaxSize);
    
    virtual TInt AttachUserDictionary(CPtiUserDictionary* aDict);
    
    virtual TBool DetachUserDictionary(CPtiUserDictionary* aDict);      

//
// Chinese API
//
private:
    
    virtual TPtrC GetCandidatePage();
    
    virtual TBool NextCandidatePage();
    
    virtual TBool PreviousCandidatePage();
    
    virtual TBool MoreCandidatePages();
    
    virtual void SetCandidatePageLength(TInt aLength);
    
    virtual TPtrC GetPhoneticSpelling(TInt aIndex) const; 
    
    virtual TInt PhoneticSpellingCount() const; 
    
    virtual TBool SelectPhoneticSpelling(TInt aIndex);
    
    virtual TInt SelectedPhoneticSpelling() const; 
    
    virtual TBool ToneMark(TText& aToneMark) const;
    
    virtual TBool IsToneMarkValidForSpelling();
    
    virtual TBool SetPredictiveChineseChar(const TDesC& aChar);
    
    virtual TBool SetToneMark(TInt aToneMark);


//
// Japanese API
//
private:
    
    virtual TPtrC ReadingTextL();
    
    virtual MPtiEngineCompositionDataInterface* CompositionData();

//
// Chinese API
//
private:
    
    virtual HBufC* GetCandidatesByInputString(
        const TDesC& aInputString, 
        RPointerArray<HBufC>& aList, 
        const TBool aIsPredictive);
    
    void UpdateFromRepositoryL();
    
    void UpdateDblConsonentL(
        const TInt& aCenRepId,
        const TOssCombinationItem& aCombination);
    
public:
    
    static TInt CBSettingChanged(void* aSelf);
    
private:
    
    CPtiEngine* iOwner;
    
    TDes* iTextBuffer;
    
    TBool iKoreanLanOpen;
    
    COssHangulInputContext* iHic;
    
    COssCombination* iCombination;
    
    TBool iLowerCase;
    
    CRepository* iRepository;
    
    CActiveCallback* iCRNotifier;
    
    };

#endif  // __PTIKOREANQWERTYCORE_H__

