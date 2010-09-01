/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  CTruiEngine class of TrainingUI
*
*/


#ifndef C_TRUIENGINE_H
#define C_TRUIENGINE_H

#include <e32base.h>
#include <badesca.h>

const TInt KShortcutMaxLength = 164;
class CPtiEngine;
class MTruiEngine;
class TLanguageRelation;
class CEikonEnv;
    
/**
 *  CTruiEngine view class.
 *
 */
class CTruiEngine : public CBase
    {
public:
    /** 
     * Two phase construction.
     * 
     * @return Pointer to CTruiEngine's instance
     */
    static CTruiEngine* NewL( CEikonEnv* aEikonEnv );

    /** 
     * Two phase construction.
     * 
     * @return Pointer to CTruiEngine's instance
     */
    static CTruiEngine* NewLC( CEikonEnv* aEikonEnv );   
        
    /**  
     * Destroy the object and release all memory objects
     */    
    virtual ~CTruiEngine();

    /**
     * Get the current input language
     *
     * @return Current input language
     */    
    inline TLanguage InputLanguage() const;
    
//Language Script
    /**
     * Set current language script
     *
     * @param aIndex The index of language script in the array named iSupportedScript
     */
    void SetLanguageScriptByIndexL( TInt aIndex );
    
    /**
     * Get name list of all supported language scripts
     *
     * @return CDesCArray* The name list
     */
    inline const CDesCArray* LanguageScriptName();
    
    /**
     * Get id of supported language script
     *
     * @return RArray<Tint> list of the script id
     */
    inline const RArray<TInt>& SupportedLanguageScript();
    
    /**
     * Get enum ID of the current language script
     *
     * @return  ID of the current language
     */
    inline TInt CurrentLanguageScript() const;
    
//Character Range
    /**
     * Set current character range
     *
     * @param aIndex The index of character range in the array named iSupportedRange
     */
    void SetCharacterRangeByIndexL( TInt aIndex );
    
    /**
     * Get name list of all supported character range
     *
     * @return CDesCArray* The name list
     */
    CDesCArray* GetCharacterRangeName();
    
    /**
     * Get enum ID of the current character range
     *
     * @return TInt ID of the character range
     */
    TInt GetCurrentCharacterRange();
    
    /**
     * Get enum ID of the supported character ranges
     *
     * @return Reference to the array saving IDs of the supported range
     */    
    inline const RArray<TInt>& GetCurrentSupportedRangesId();
    
    /**
     * Transform character range to engine's subrange
     */
    void SetSubrange();
    
//Character Set

    /**
     * Get current character set
     *
     * @return HBufC* The string contained character set
     */
    const HBufC* CharacterSet();
    
    /**
     * Check if character has model
     *
     * @param aLanguageScript Where the character is come from
     * @param aChar The character which is checked
     * @return TBool True means this character has model
     *               False means this character doesn't has model yet
     */
    TBool CheckCharacterModel( const TDesC& aChar );
    
    /**
     * Get character model
     *
     * @param aChar Get this character's model
     * @param aModel Array of points where the character's model is stored.     
     */
    void GetCharacterModelL( const TDesC& aChar, RArray<TPoint>& aModel );
    
    /**
     * Set character model
     *
     * @param aChar Set model to this character
     * @param aModel Array of points where the character's model is stored.
     * @param aSimilarMsg Text exists in the HWREngine with the similar model.
     * @return none, if the symbol model is invalid, leave with KErrAlreadyExists
     */
    void SetCharacterModelL( const TDesC& aChar,
                             RArray<TPoint>& aModel,
                             TDes& aSimilarMsg );
        
    /**
     * Delete character model
     *
     * @param aChar Delete this character's model
     * @return TInt Error value. If no error occurred, KErrNone is returned
     */
    TInt DeleteCharacterModel( const TDesC& aChar );
    
//Shortcut
    
    /**
     * Check if shortcut has model
     *
     * @param aText The shortcut which is checked
     * @return TBool True means this shortcut has model
     *               False means this shortcut doesn't has model yet
     */
    TBool CheckShortcutModel( const TDesC& aText );
        
    /**
     * Change the text of an existing shortcut
     *
     * @param aOldText The text of an existing shortcut
     * @param aNewText The new text of the shortcut     
     */
    void ChangeShortcutTextL( const TDesC& aNewText );    

    /**
     * Set current editing shortcut
     *
     * @param sShortcut text of the shortcut
     * @return TInt Error value. If no error occurred, KErrNone is returned
     */
    TInt SetShortcut( const TDesC& aText );
    
    /**
     * Get current edited shortcut
     *
     * @return TDes shortcut text
     */
    inline TDes& Shortcut();
    
    /**
     * Get all shortcuts from HWR engine
     *
     * @return Pointer to iShortcutList to contain all existing shortcut
     */
    inline CDesCArray* ShortcutTextList();
    
    /**
     * Save shortcut text and its assigned character model into PTI Engine
     *
     * @param aText Save this Shortcut's model
     * @param aModel Array of points where the Shortcut's model is stored.
     * @param aSimilarMsg Text exists in the HWREngine with the similar model.
     * @return none, if the symbol model is invalid, leave with KErrAlreadyExists
     */
    void SaveShortcutL( const TDesC& aText,
                        RArray<TPoint>& aModel,
                        TDes& aSimilarMsg );
    
    /**
     * Delete Shortcut model
     *
     * @param aText Delete this Shortcut's model
     * @return TInt Error value. If no error occurred, KErrNone is returned
     */
    TInt DeleteShortcut( const TDesC& aText );
    
    /**
     * Update Shortcut list
     *
     */
    void UpdateShortcutListL();
    
    /**
     * Set edit state
     *
     * @param aNew: ETrue, perform the function of adding new shortcut. 
     *              EFalse, perform the function of editting old shortcut.
     */
    inline void NewShortcut( TBool aNew );
    
    /**
     * Get edit state
     *
     */
    inline TBool NewShortcut();
    
    /**
     * Set if display shortcut wizard view.
     *
     * @param aIsDisplay ETrue: Display shortcut wizard view
     */
    inline void SetDisplayWizard( TBool aIsDisplay );

    /**
     * Get if display shortcut wizard view
     *
     * @return ETrue: Display shortcut wizard view
     */
    inline TBool DisplayWizard() const;
    
    /**
     * Set the current selected index
     *
     * @param aSelectedIndex currently selected index of shortcuts
     */
    inline void SetCurrentSelectedIndex( TInt aSelectedIndex );
        
    /**
     * Get the current selected index
     *
     * @return aSelectedIndex currently selected index of shortcuts
     */
    inline TInt CurrentSelectedIndex();

    /**
     * Get guiding line pos
     * 
     * @param aSize The size of WritingBox
     * @param aTop The vertical coordinate of top guiding line
     * @param aBottom The vertical coordinate of bottom guiding line
     * @return TInt Error value. If no error occurred, KErrNone is returned
     */
    TInt GetGuidingLinePos( const TSize& aSize, TInt& aTop , TInt&  aBottom); 

    /**
     * Get the number of the maximum of shortcuts
     *
     * @return TInt The number of the maximum of shortcuts
     */
    TInt MaxShortCutLength(); 
    
    /**
     * Get trails assigned to the specified shortcut.
     * 
     * @param aText The specified shortcut text
     * @param aModel Trails of the shortcut.
     * @param aUnicode The assigned preset text if it is a preset shortcut,
     *        otherwise, it is 0;     
     */
    void GetShortcutModelL( const TDesC& aText, RArray<TPoint>& aModel, TUint& aUnicode );
    
    /**
     * Save preset shortcuts.
     * 
     * @param aUnicode The assigned preset text.
     * @param aShortcut The shortcut to be save.
     */
    void SavePresetShortcutL( TUint aUnicode, const TDesC& aShortcut );

    /**
     * Get trails assigned to a preset text.
     * 
     * @param aUnicode The unicode of the preset text.
     * @param aModel Store trails of the preset text.
     * @param aShortcut Indicate whether the preset text has been assigned to a shortcut text.
     *        If KNullDesC, it is not in use.
     */
    void GetPresetSymbolByUnicodeL( TUint aUnicode, RArray<TPoint>& aModel, TDes& aShortcut );
    
    /**
     * Return all unicodes of preset text stored in PTI engine
     * 
     * @param aPresets All preset text's unicodes are stored in this array.     
     */
    void GetAllPresetSymbolsL( RArray<TUint>& aPresets );    
    
private:

    /** 
     * Perform the first phase of two phase construction.
     */
    CTruiEngine( CEikonEnv* aEikonEnv );
    
    /**
     * Perform the second phase construction of a CTruidemoAppUi object 
     */
    void ConstructL(); 
    
    /**
     * Initialize the table used to map language to language script
     * @param aResourceId ResourceId of Language script set.
     * @param aLanguageRelation Store the mappint relation.
     */
    void InitLanguageRelationL( TInt aResourceId, 
                            RArray<TLanguageRelation>& aLanguageRelation );
    
    /**
     * Initialize the supported language script.    
     * @param aSupportedScript Store the suppotred script.
     * @param aEngine PtiEngine from which get supported input language.
     * @param aLanguageRelation Mapping relation used to convert language 
     *                          into language script.     
     */
    void InitSupportedLanguageScriptL( RArray<TInt>& aSupportedScript,
                                       CPtiEngine* aEngine,
                              const RArray<TLanguageRelation>& aLanguageRelation );
    
    /**
     * Get the active language script from the active input language
     * @param aLanguageCode Currently active input language code.
     * @param aLanguageRelation Mapping relation used to convert language 
     *                          into language script.     
     */
    TInt GetActiveLanguageScript( TInt aLanguageCode, 
                           const RArray<TLanguageRelation>& aLanguageRelation );
                           
    /**
     * Get the resourc id of lower case of Cyrillic according to the current input language
     * @param aLanguage Currently input language.
     * @return resource id 
     */
    TInt CyrillicLowerResourceId( TLanguage aLanguage );
    
    /**
     * Get the resourc id of upper case of Cyrillic according to the current input language
     * @param aLanguage Currently input language.
     * @return resource id 
     */
    TInt CyrillicUpperResourceId( TLanguage aLanguage );

private:

    /**
     * Current edited shortcut
     */
    TBuf<KShortcutMaxLength> iShortcut;
    
    /**
     * Array of all supported language script
     * Own
     */
    RArray<TInt> iSupportedScript;
        
    /**
     * Current language script
     */
    TInt iCurrentScript;
    
    /**
     * Array of all supported character range
     * Own
     */
    RArray<TInt> iSupportedRange;
    
    /**
     * Array of the name of all supported character range
     * Own
     */
    CDesCArray* iRangeName;
    
    /**
     * Current character range
     */
    TInt iCurrentRange;
    
    /**
     * String of character set
     * Own
     */
    HBufC* iCharacterSet;
       
    /**
     * Array of the shortcut
     * Own
     */
    CDesCArray* iShortcutList;
    
    /**
     * Indicate if current edited shortcut is new created
     */
    TBool iNewShortcut;
    
    /**
     * The currently selected index of shortcuts
     */
    TInt iSelectedShortcut;
    
    /**
     * PtiEngine
     * Own
     */
    CPtiEngine* iEngine;
    
    /**
     * Interface trui used in PtiEngine
     * Not own
     */
    MTruiEngine* iPtiEngine;
    
    /**
     * Current subrange
     */
    TInt iSubrange;
    
    /**
     * Pointer to eikon enviroment.
     * Not own
     */
    CEikonEnv* iEnv;
    
    /**
     * Check if need to display shortcut wizard view
     */
    TBool iIsDisplay;
    
    /**
     * Current input language
     */    
    TLanguage iInputLanguage;
    };
    
#include "truiengine.inl"

#endif // C_TRUIENGINE_H

                    
