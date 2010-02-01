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















#ifndef _PTI_MHWRRECOGNIZER_H
#define _PTI_MHWRRECOGNIZER_H

// INCLUDES
#include <badesca.h>
#include <PtiDefs.h>
#include <eikon.hrh>

// public sdk
enum TPtiHwrScriptType
    {
    EPtiHwrScriptBegin = ELangMaximum,
    EPtiHwrScriptLatin,
    EPtiHwrScriptCyrillic,
    EPtiHwrScriptGreek,
    EPtiHwrScriptHebrew,
    EPtiHwrScriptArabic,
    EPtiHwrScriptAny = ELangMaximum + 200   
    };

enum TPtiHwrRange
    {
    EPtiHwrRangeLanguage = 1,
    EPtiHwrRangeSymbol,
    EPtiHwrRangeNumber,
    EPtiHwrRangeAny = EPtiHwrRangeLanguage + 100
    };   
    
    
/**
 *  MTruiEngine class.
 *  The interface between HWR engine and TrainUI application
 */
class MTruiEngine
    {
public:
    /**
     * Set current trained character's Language Script
     *
     * @param aLanguageScript current trained character's Language Script
     */
    virtual void SetLanguageScriptL( TInt aLanguageScript ) = 0;
    
    /**
     * Check if character has model
     *
     * @param aChar The character which is checked
     * @return TBool True means this character has model
     *               False means this character doesn't has model yet
     */
    virtual TBool CharacterModelExist( const TDesC& aChar ) = 0;
    
    /**
     * Get character model
     *
     * @param aChar Get this character's model
     * @param aModel Array of points where the character's model is stored.
     * @return TInt Error value. If no error occurred, KErrNone is returned
     */
    virtual void GetCharacterModelL( const TDesC& aChar,
                                    RArray<TPoint>& aModel ) = 0;
    
    /**
     * Set character model
     *
     * @param aChar Set model to this character
     * @param aModel Array of points where the character's model is stored.
     * @aSubRange Shall set the value to TPtiHwrRange.
     * @aSimilarChar if not accepted by engine, store the conflict char
     * @return TInt Error value. If no error occurred, KErrNone is returned
     */
    virtual void SetCharacterModelL( const TDesC& aChar,
                                    RArray<TPoint>& aModel , TInt aSubRange, TDes& aSimilarChar ) = 0;
    
    /**
     * Delete character model
     *
     * @param aChar Delete this character's model
     * @return TInt Error value. If no error occurred, KErrNone is returned
     */
    virtual TInt DeleteCharacterModel( const TDesC& aChar ) = 0;   
    
    /**
     * Get text list of all the shortcuts
     *
     * @param aShortcutTextList The text list.
     * @return TInt Error value. If no error occurred, KErrNone is returned
     */
    virtual void GetShortcutTextListL( RPointerArray<HBufC>& aShortcutTextList ) = 0;
    
    /**
     * Check if shortcut has model
     *
     * @param aText The shortcut which is checked
     * @return TBool True means this shortcut has model
     *               False means this shortcut doesn't has model yet
     */
    virtual TBool CheckShortcutModel( const TDesC& aText ) = 0;
    
    /**
     * Get trails assigned to the specified shortcut.
     * 
     * @param aText The specified shortcut text
     * @param aModel Trails of the shortcut.
     * @param aUnicode The assigned preset text if it is a preset shortcut,
     *        otherwise, it is 0;     
     */
    virtual void GetShortcutModelL( const TDesC& aText, 
                            RArray<TPoint>& aModel, TUint& aUnicode ) = 0;
    
    /**
     * Set Shortcut model
     *
     * @param aText Set model to this Shortcut
     * @param aModel Array of points where the Shortcut's model is stored.
     * @param aSimilarText Store the conflict text if exist.
     * @return void.
     */
    virtual void SetShortcutModelL( const TDesC& aText,
                              RArray<TPoint>& aModel, TDes& aSimilarText ) = 0;
    
    /**
     * Delete Shortcut model
     *
     * @param aText Delete this Shortcut's model
     * @return TInt Error value. If no error occurred, KErrNone is returned
     */
    virtual TInt DeleteShortcutModel( const TDesC& aText ) = 0;
    
    /**
     * Change the text of an existing shortcut
     *
     * @param aOldText The text of an existing shortcut
     * @param aNewText The new text of the shortcut
     * @return none
     */
    virtual void ChangeShortcutTextL( const TDesC& aOldText,
                                     const TDesC& aNewText ) = 0;


    /**
     * Get guiding line pos
     * 
     * @aSize The are
     * @param aTop The vertical coordinate of top guiding line
     * @param aBottom The vertical coordinate of bottom guiding line
     * @return TInt Error value. If no error occurred, KErrNone is returned
     */
    virtual TInt GetGuidingLinePos( const TSize& aSize, TInt& aTop , TInt&  aBottom) = 0; 

    /**
     * Get the number of the maximum of shortcuts
     *
     * @return TInt The number of the maximum of shortcuts
     */
    virtual TInt MaxShortCutLength() = 0; 

    /**
     * Save preset shortcuts.
     * 
     * @param aUnicode The assigned preset text.
     * @param aShortcut The shortcut to be save.
     */
    virtual void SavePresetShortcutL( TUint aUnicode, const TDesC& aShortcut ) = 0;
    
    /**
     * Return all unicodes of preset text stored in PTI engine
     * 
     * @param aPresets All preset text's unicodes are stored in this array.     
     */
    virtual void GetAllPresetSymbolsL( RArray<TUint>& aPresets ) = 0;  
    
    /**
     * Get trails assigned to a preset text.
     * 
     * @param aUnicode The unicode of the preset text.
     * @param aModel Store trails of the preset text.
     * @param aShortcut Indicate whether the preset text has been assigned to a shortcut text.
     *        If KNullDesC, it is not in use.
     */
    virtual void GetPresetSymbolByUnicodeL( TUint aUnicode, RArray<TPoint>& aModel, TDes& aShortcut ) = 0;  
    };
    
    
/**
* MHwrRecognizer class.
* Interface definiton for Hwr.
*/
class MPtiHwrRecognizer
    {
public:
    // pure hwr interface
    /**
     * Set Hwr to given recognition range
     *
     * @param aRange Given recognition range
     * @return KErrNone means successfully, otherwise system wide error codes
     */
    virtual TInt SetRange(const TRecognitionRange& aRange) = 0;

    /**
     * Get current recognition range of hwr engine
     *
     * @return Current recognition range
     */
    virtual TRecognitionRange GetRange() = 0;

    /**
     * Appends an auxiliary range
     *
     * @param aRange The auxiliary range to be appended
     * @return KErrNone means successfully, otherwise system wide error codes
     */
    virtual TInt AddAuxiliaryRange (const TRecognitionRange& aRange) = 0;
    
    /**
     * Remove all auxiliary recognition ranges
     *
     * @return None
     */
    virtual void RemoveAllAuxRange () = 0;
    
    /**
     * Remove an specified auxiliary recognition range.
     *
     * @param aIndex The index of auxiliary range to be deleted
     * @return KErrNone means successfully, otherwise system wide error codes
     */
    virtual TInt RemoveAuxiliaryRange (TInt aIndex) = 0;
    
    /**
     * Set the number of primary candidate candidates the HWR engine should 
     * return each time
     *
     * @param aNumber The number of primary candidates that HWR engine should 
     * return each time
     * @return KErrNone means successfully, otherwise means aNumber is illegal
     */
    virtual TInt SetCandidateNum(TInt aNumber) = 0;

    /**
     * Retrieve the current primary candidate number need to be returned 
     * each time
     *
     * @return The number of primary candidate characters which engine returns 
     * each time
     */
    virtual TInt GetCandidateNum() const = 0;
    
    /**
     * Get the stroke end mark 
     *
     * @return Current stroke end mark
     */
    virtual TPoint StrokeEndMark() const = 0;
    
    /**
     * Get the character candidates from the HWR engine based on the input strokes 
     *
     * @param aTraceData The array of pen trace points, each stroke is ended by 
     * stroke end mark
     * @param aResult Candidate buffer array
     * @return KErrOutOfMem if out of memory, otherwise number of the primary 
     * candidate in the candidate list
     */
    virtual TInt Recognize(const RArray<TPoint>& aTraceData, RPointerArray<HBufC>& aResult) = 0;

    /**
     * Tell the HWR engine about the size of the writing area used to 
     * input character 
     *
     * @param aSize The size of writing area
     * @return KErrNone means operation succeed, otherwise system wide error codes
     */
    virtual TInt SetInputAreaSize(TSize& aSize) = 0;

    /**
     * Retrieve an ASCII string that describes the version information 
     * of the current HWR engine
     *
     * @param aVersion Contains the engine version string on return
     * @return KErrNone means operation succeed, otherwise system wide error codes
     */
    virtual TInt GetEngineVersion(TDes& aVersion) = 0;
    
    /**
     * Retrieve an ASCII string that describes the version information 
     * of the basic recognition dictionary
     *
     * @param aVersion Contains recognition dictionary version string on return
     * @return KErrNone means operation succeed, otherwise system wide error codes
     */
    virtual TInt GetDictVersion(TDes& aVersion) = 0;

    /**
     * Enable or disable word recognition
     *
     * @param aFlag The word recognition flag, ETrue if enable word recognition, 
     * otherwise EFalse
     * @return None
     */
    virtual void EnableWordRecognition(const TBool aFlag) = 0;
    
    /**
     * Specify whether small case or upper case letter will be the first one 
     * in candidate list when the first two letters are the same if case is ignored
     * 
     * @param aOrder EUpperFirst if user wants upper case letter in the first, 
     * otherwise ELowerFirst
     * @return None
     */
    virtual void SetFirstLetterOrder (const TLatinLetterOrder aOrder) = 0;

    /**
     * Set number mode
     * 
     * @param aNumberMode Given number mode
     * @return None
     */
    virtual void SetNumberMode(const TAknEditorNumericKeymap aNumberMode) = 0;
    
    /**
     * Set position of HWR top guideline
     * 
     * @param aPos Position to be set
     * @return None
     */
    virtual void GetTopGuideLinePos(TInt& aPos) = 0;

    /**
     * Set position of HWR bottom guideline
     * 
     * @param aPos Position to be set
     * @return None
     */
    virtual void GetBottomGuideLinePos(TInt& aPos) = 0;
    
    /**
     * Set permitted recognized character set
     * 
     * @param aSymbolSet Permitted recognized character set
     * @return None
     */
    virtual void SetPermittedSymbolSet(const TDesC& aSymbolSet) = 0;    

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
    virtual TInt RecognizeWithCharSet(const RArray<TPoint>& aTraceData, RPointerArray<HBufC>& aResult, const TDesC& aCharSet) = 0;
    
    /**
     * Set the number of each secondary candidates the HWR engine should 
     * return each time
     *
     * @param aNumber The number of each secondary candidates that HWR engine should 
     * return each time
     * @return KErrNone means successfully, otherwise means aNumber is illegal
     */
    virtual TInt SetAuxCandidateNum(TInt aNumber) = 0;
    
    /**
     * Retrieve the current each secondary candidate number need to be returned 
     * each time
     *
     * @return The number of secondary candidate characters which engine returns 
     * each time
     */
    virtual TInt GetAuxCandidateNum() const = 0;
    
    /**
     * Retrieve MTruiEngine interface pointer if the derived engine support UDM feature.
     *
     * @return The pointer of MTruiEngine if success.
     */
    virtual MTruiEngine* QueryUdmInterfaceL()  = 0;
    
    /**
     * Tell the engine that a new MCR session has been started.
     *
     * @return None.
     */
    virtual void McrBeginSessionL() = 0;
    
        /**
     * Get the character candidates from the HWR engine based on the input strokes by MCR
     *
     * @param aTraceData The array of pen trace points, each stroke is ended by 
     * stroke end mark
     * @param aResult Candidate buffer array
     * @return KErrOutOfMem if out of memory, otherwise number of the primary 
     * candidate in the candidate list
     */
    virtual void McrAddStrokesL(const RArray<TPoint>& aTraceData, RPointerArray<HBufC>& aResult) = 0;
    
    /**
     * Tell the engine that a new MCR session need to be ended, return the last result.
     *
     * @return None.
     */
    virtual void McrEndSessionL( RPointerArray<HBufC>& aResult ) = 0;
    /**
     * Tell the engine whether to use dictionary when doing reconition.
     * @param aFlag ETrue to enable recognition dictionary, and EFalse to diable.
     * @return KErrNone means operation succeed, otherwise system wide error codes
     */
    virtual TInt EnableRecognitionDictionary(const TBool aFlag) = 0;
    /**
     * Tell the HWR engine about the size of the touch screen 
     * 
     *
     * @param aSize The size of touch screen
     * @return KErrNone means operation succeed, otherwise system wide error codes
     */  
    virtual TInt SetScreenSize(TSize& aSize) = 0;   
    /**
     * Tell the HWR engine whether guideline is on.
     * 
     *
     * @param aFlag ETrue to enable guideline, and EFalse to diable.
     * @return KErrNone means operation succeed, otherwise system wide error codes
     */  
    virtual TInt EnableGuideline(const TBool aFlag) = 0;    
    };

    
#endif  //_PTI_MHWRRECOGNIZER_H