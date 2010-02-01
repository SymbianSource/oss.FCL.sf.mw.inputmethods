/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:               Predective text input engine hwr recognizer interface.
*
*/















#ifndef _PTI_HWRRECOGNIZER_H
#define _PTI_HWRRECOGNIZER_H

// INCLUDES
#include "PtiMHwrRecognizer.h"
 
// internal definition
enum TUdmType
    {
    EUdmSymbol = 1,
    EUdmShortcut,
    EUdmMax
    };
 struct THwrUdmRange
    {
    TInt iScript;
    TInt iRange;
    };
class CTruiPtiEngine;




/**
* CHwrRecognizer class.
* Interface definiton for Hwr.
*/
class CHwrRecognizer : 
    public CBase,
    public MPtiHwrRecognizer
    {
public:
    /**
     * Two phase constructor, create instance of implementation by 
     * implementation uid.
     *
     * @param aImpId The given implementation uid
     */
    IMPORT_C static CHwrRecognizer* NewL(TInt aImpId);
    
    /**
     * Two phase constructor, create instance of implementation by
     * given language.
     *
     * @param aLanguage The language should be support by implementation instance
     * @param aHwrImplUid Carray the implementation uid on return
     */
    IMPORT_C static CHwrRecognizer* NewL(const TLanguage aLanguage, TUid& aHwrImplUid);
    
    /**
     * Destructor
     */
    IMPORT_C virtual ~CHwrRecognizer();

    /**
     * Get implementation uid list by supplied resolution parameters
     *
     * @param aMatch Resolution parameters
     * @param aImpIdList The array carry the implementation uid list
     * @return None
     */
    IMPORT_C static void ListImplementationsL(const TLanguage aLanguage, RArray<TUid>& aImpIdList);
    
    /**
     * Get supported language list
     *
     * @param aResult Carry the supported language list on return
     * @return None
     */
    IMPORT_C static void ListAvailableLanguagesL(RArray<TInt>& aResult);
    
    /**
     * Get supported language list of given hwr implementation instance specified by given 
     * implementation uid
     *
     * @param aImpId The given implementation uid
     * @param aResult Carry the supported language list on return
     * @return None
     */
    IMPORT_C static void ListGivenAvailableLanguagesL(TUid aImpId, RArray<TInt>& aResult);
    
    // from MPtiHwrRecognizer
    /**
     * Set Hwr to given recognition range
     *
     * @param aRange Given recognition range
     * @return KErrNone means successfully, otherwise system wide error codes
     */
    IMPORT_C TInt SetRange(const TRecognitionRange& aRange);

    /**
     * Get current recognition range of hwr engine
     *
     * @return Current recognition range
     */
    IMPORT_C TRecognitionRange GetRange();

    /**
     * Appends an auxiliary range
     *
     * @param aRange The auxiliary range to be appended
     * @return KErrNone means successfully, otherwise system wide error codes
     */
    IMPORT_C TInt AddAuxiliaryRange (const TRecognitionRange& aRange);
    
    /**
     * Remove all auxiliary recognition ranges
     *
     * @return None
     */
    IMPORT_C void RemoveAllAuxRange ();
    
    /**
     * Remove an specified auxiliary recognition range.
     *
     * @param aIndex The index of auxiliary range to be deleted
     * @return KErrNone means successfully, otherwise system wide error codes
     */
    IMPORT_C TInt RemoveAuxiliaryRange (TInt aIndex);
    
    /**
     * Set the number of primary candidate candidates the HWR engine should 
     * return each time
     *
     * @param aNumber The number of primary candidates that HWR engine should 
     * return each time
     * @return KErrNone means successfully, otherwise means aNumber is illegal
     */
    IMPORT_C TInt SetCandidateNum(TInt aNumber);

    /**
     * Retrieve the current primary candidate number need to be returned 
     * each time
     *
     * @return The number of primary candidate characters which engine returns 
     * each time
     */
    IMPORT_C TInt GetCandidateNum() const;
    
    /**
     * Get the stroke end mark 
     *
     * @return Current stroke end mark
     */
    IMPORT_C TPoint StrokeEndMark () const;
    
    /**
     * Get the character candidates from the HWR engine based on the input strokes 
     *
     * @param aTraceData The array of pen trace points, each stroke is ended by 
     * stroke end mark
     * @param aResult Candidate buffer array
     * @return KErrOutOfMem if out of memory, otherwise number of the primary 
     * candidate in the candidate list
     */
    IMPORT_C TInt Recognize(const RArray<TPoint>& aTraceData, RPointerArray<HBufC>& aResult);

    /**
     * Tell the HWR engine about the size of the writing area used to 
     * input character 
     *
     * @param aSize The size of writing area
     * @return KErrNone means operation succeed, otherwise system wide error codes
     */
    IMPORT_C TInt SetInputAreaSize(TSize& aSize);

    /**
     * Retrieve an ASCII string that describes the version information 
     * of the current HWR engine
     *
     * @param aVersion Contains the engine version string on return
     * @return KErrNone means operation succeed, otherwise system wide error codes
     */
    IMPORT_C TInt GetEngineVersion(TDes& aVersion);
    
    /**
     * Retrieve an ASCII string that describes the version information 
     * of the basic recognition dictionary
     *
     * @param aVersion Contains recognition dictionary version string on return
     * @return KErrNone means operation succeed, otherwise system wide error codes
     */
    IMPORT_C TInt GetDictVersion(TDes& aVersion);

    /**
     * Enable or disable word recognition
     *
     * @param aFlag The word recognition flag, ETrue if enable word recognition, 
     * otherwise EFalse
     * @return None
     */
    IMPORT_C void EnableWordRecognition(const TBool aFlag);
    
    /**
     * Specify whether small case or upper case letter will be the first one 
     * in candidate list when the first two letters are the same if case is ignored
     * 
     * @param aOrder EUpperFirst if user wants upper case letter in the first, 
     * otherwise ELowerFirst
     * @return None
     */
    IMPORT_C void SetFirstLetterOrder (const TLatinLetterOrder aOrder);

    /**
     * Set number mode
     * 
     * @param aNumberMode Given number mode
     * @return None
     */
    IMPORT_C void SetNumberMode(const TAknEditorNumericKeymap aNumberMode);

    /**
     * Set position of HWR top guideline
     * 
     * @param aPos Position to be set
     * @return None
     */
    IMPORT_C void GetTopGuideLinePos(TInt& aPos);

    /**
     * Set position of HWR bottom guideline
     * 
     * @param aPos Position to be set
     * @return None
     */
    IMPORT_C void GetBottomGuideLinePos(TInt& aPos);
    
    /**
     * Set permitted recognized character set
     * 
     * @param aPos Position to be set
     * @return None
     */
    IMPORT_C void SetPermittedSymbolSet(const TDesC& aSymbolSet);
    
    /**
     * Get the character candidates from the HWR engine based on the input strokes 
     *
     * @param aTraceData The array of pen trace points, each stroke is ended by 
     * stroke end mark
     * @param aResult Candidate buffer array
     * @param aCharSet The small charset that primary range candidate should come from
     * @return KErrOutOfMem if out of memory, otherwise number of the primary 
     * candidate in the candidate list
     */
    IMPORT_C TInt RecognizeWithCharSet(const RArray<TPoint>& aTraceData, RPointerArray<HBufC>& aResult, const TDesC& aCharSet);
    
    /**
     * Set the number of each secondary candidates the HWR engine should 
     * return each time
     *
     * @param aNumber The number of each secondary candidates that HWR engine should 
     * return each time
     * @return KErrNone means successfully, otherwise means aNumber is illegal
     */
    IMPORT_C TInt SetAuxCandidateNum(TInt aNumber);
    
    /**
     * Retrieve the current each secondary candidate number need to be returned 
     * each time
     *
     * @return The number of secondary candidate characters which engine returns 
     * each time
     */
    IMPORT_C TInt GetAuxCandidateNum() const;
    
    /**
     * Retrieve MTruiEngine interface pointer if the derived engine support UDM feature.
     *
     * @return The pointer of MTruiEngine if success.
     */
    IMPORT_C MTruiEngine* QueryUdmInterfaceL();
    
    /**
     * Tell the engine that a new MCR session has been started.
     *
     * @return None.
     */
    IMPORT_C void McrBeginSessionL();
    
        /**
     * Get the character candidates from the HWR engine based on the input strokes by MCR
     *
     * @param aTraceData The array of pen trace points, each stroke is ended by 
     * stroke end mark
     * @param aResult Candidate buffer array
     * @return KErrOutOfMem if out of memory, otherwise number of the primary 
     * candidate in the candidate list
     */
    IMPORT_C void McrAddStrokesL(const RArray<TPoint>& aTraceData, RPointerArray<HBufC>& aResult);
    
    
    /**
     * Tell the engine that a new MCR session need to be ended, return the last result.
     *
     * @return None.
     */
    IMPORT_C void McrEndSessionL( RPointerArray<HBufC>& aResult );

    /**
     * Check wether the new symbol model is valid.
     *
     * @param aSymbolName The name of the symbol name.
     * @param aStrokes Store the strokes for the model.
     * @param aScriptType The script that the model shall belongs to  . 
     * @param aSimilarSymbol Store the characters recognized by aStrokes
     * @return KErrNone if the model is accpeted by the engine, otherwise err codes
     */
    IMPORT_C virtual TInt SymbolModelValid(  const TDesC& aSymbolName, const RArray<TPoint>& aStrokes, THwrUdmRange aScriptType, TDes& aSimilarSymbol );
    /**
     * Tell the engine whether to use dictionary when doing reconition.
     * @param aFlag ETrue to enable recognition dictionary, and EFalse to diable.
     * @return KErrNone means operation succeed, otherwise system wide error codes
     */
    IMPORT_C TInt EnableRecognitionDictionary(const TBool aFlag);  
    /**
     * Tell the HWR engine about the size of the touch screen 
     * 
     *
     * @param aSize The size of touch screen
     * @return KErrNone means operation succeed, otherwise system wide error codes
     */  
    IMPORT_C TInt SetScreenSize(TSize& aSize);       
    /**
     * Tell the HWR engine whether guideline is on.
     * 
     *
     * @param aFlag ETrue to enable guideline, and EFalse to diable.
     * @return KErrNone means operation succeed, otherwise system wide error codes
     */  
    IMPORT_C TInt EnableGuideline(const TBool aFlag);    
protected:
    
    /**
     * Retrieve whether the derived engine support UDM feature.
     *
     * @return ETrue if support.
     */
    IMPORT_C virtual  TBool IsSupportUdm() const;
    
    /**
     * Load the aType models to memory.
     *
     * @param aType Shall set with the value TUdmType 
     * @return none
     */
    IMPORT_C   void LoadUdmL( TUdmType aType );
    
    /**
     * Free the momory of the aType models
     *
     * @param aType Shall set with the value TUdmType 
     * @return none
     */
    IMPORT_C   void UnLoadUdmL( TUdmType aType );
    
    /**
     * Get the filterd models by THwrUdmRange, return the indexes.
     *
     * @param aType Shall set with the value TUdmType 
     * @param aList Store the indexes that fulfil the aRange
     * @param aRange Sepcifiy the rules that the modles shall belong to.
     * @return none
     */
    IMPORT_C   void GetModelIndexListL(TUdmType aType, RArray<TInt>& aList, const THwrUdmRange& aRange );
    
    /**
     * Get the filterd models by THwrUdmRange, return the indexes.
     *
     * @param aType Shall set with the value TUdmType 
     * @param aIndex The index of the model.
     * @param aSymbolName Sepcifiy the rules that the modles shall belong to.
     * @param aModel Store the strokes for the model.
     * @param aHelpLine Store the helpline. 
     * @param aBaseLine Store the baseline.
     * @return none
     */
    IMPORT_C   void GetSymbolModelL(TUdmType aType, TInt aIndex, TPtrC& aSymbolName,RArray<TPoint>& aModel, TInt& aHelpLine, TInt& aBaseLine );
    
private:

    /**
     * Utitlity function, parse a string to range values
     * 
     * @param aRangeData The string to convert
     * @param aLowerValue The lower bound value.
     * @param aUpperValue The upper bound value.
     * @param aRadix The number system representation for the integer
     * @since S60 v3.2
     *
     * @return None
     */
    void static GetRanges(const TDesC8& aRangeData, TInt& aLowerValue, TInt& aUpperValue);
    
    /**
     * Utitlity function, extract the languages and append them to array
     * 
     * @param aImplementationData The string to extract.
     * @param aResult The result array that need to append.
     * @since S60 v3.2
     *
     * @return None.
     */
    void static AppendMatchedLanguagesL(const TDesC8& aImplementationData, RArray<TInt>& aResult);
    
    /**
     * Utitlity function, indicates whether the value is included
     * in a string.
     * 
     * @param aImplementationData The string to test.
     * @param aValue The value to test.
     * @since S60 v3.2
     *
     * @return ETrue if match, otherwise EFalse.
     */
    TBool static Match(const TDesC8& aImplementationData, TInt aValue);

private:

    /**
     * instance identifier key
     */
    TUid iDtor_ID_Key;
    
    CTruiPtiEngine* iUdmEngine;
    
    
    };
    
/**
 * This class declares the hwr recognizer
 * Purpose of this class is to save some information of active hwr recognizer
 *
 * @since S60 v4.0
 */  
class CPtiHwrRecognizer : public CBase
    {
public: // data

    /**
     * Destructor
     */
    ~CPtiHwrRecognizer();

    /**
     * The pointer points to current active hwr implementation
     */
    CHwrRecognizer* iHwrRecognizer;
            
    /**
     * The languages list that current active hwr implementation supports
     */
    RArray<TInt> iHwrLanguagesList;
	    
    /**
     * The implementation uid of current active hwr implementation
     */
    TUid iHwrImpId;
    };

#endif //  _PTI_HWRRECOGNIZER_H