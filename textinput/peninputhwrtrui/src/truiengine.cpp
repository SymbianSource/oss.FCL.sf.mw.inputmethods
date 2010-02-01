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
* Description:  Implement of class CTruiShortcutsView
*
*/


#include <trui.rsg>
#include <e32const.h>
#include <PtiMHwrRecognizer.h>
#include <PtiLanguage.h>
#include <eikenv.h>
#include <AknFepInternalCRKeys.h>
#include <centralrepository.h>
#include <featmgr.h>
#include <PtiEngine.h>

#include "trui.hrh"
#include "truiengine.h"

#include "truilanguagerelation.h"

const TInt KRangeNamesArraySize = 5;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// construction
// ---------------------------------------------------------------------------
//
CTruiEngine::CTruiEngine( CEikonEnv* aEikonEnv ) : iEnv( aEikonEnv )
    {
    }

// ---------------------------------------------------------------------------
// construction
// ---------------------------------------------------------------------------
//
void CTruiEngine::ConstructL()
    {  
    CRepository* repository = CRepository::NewLC( KCRUidAknFep );
    // Get current input language
    TInt inputLanguage = 0;
    repository->Get( KAknFepInputTxtLang, inputLanguage );
    iInputLanguage = (TLanguage)inputLanguage;

    // Create PtiEngine
    iEngine = CPtiEngine::NewL();
    MPtiHwrRecognizer* hwr = iEngine->GetHwrRecognizerL( ELangEnglish );
    
    iPtiEngine = hwr->QueryUdmInterfaceL() ;    
    
    // Initialize a mapping table for language script
    RArray<TLanguageRelation> languageRelation;
    CleanupClosePushL( languageRelation );
    InitLanguageRelationL( R_TRUI_LANGUAGE_SCRIPT_SET, languageRelation );
    
    // Get all supported language script
    InitSupportedLanguageScriptL( iSupportedScript, iEngine, languageRelation );
    
    if ( iSupportedScript.Count() == 0 )
        {
        User::Leave( KErrNotSupported );
        }
    
    // Get currently active language script
    TInt langCode;
    repository->Get( KAknFepInputTxtLang, langCode );    
    TInt activeScript = GetActiveLanguageScript( langCode, languageRelation );
    
    // Destroy the mapping table.
    CleanupStack::PopAndDestroy( &languageRelation );
    CleanupStack::PopAndDestroy( repository );
    
    if ( activeScript != KErrNotFound )
        {
        SetLanguageScriptByIndexL( activeScript );
        }
    else
        {
        User::Leave( KErrNotSupported );
        }    
    }

// ---------------------------------------------------------------------------
// construction
// ---------------------------------------------------------------------------
//
CTruiEngine* CTruiEngine::NewL( CEikonEnv* aEikonEnv )
    {
    CTruiEngine* self = CTruiEngine::NewLC( aEikonEnv );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// construction
// ---------------------------------------------------------------------------
//
CTruiEngine* CTruiEngine::NewLC( CEikonEnv* aEikonEnv )
    {
    CTruiEngine* self = new ( ELeave ) CTruiEngine( aEikonEnv );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// destruction
// ---------------------------------------------------------------------------
//
CTruiEngine::~CTruiEngine()
    {
    iSupportedScript.Close();        
    if ( iRangeName )
        {
        iRangeName->Reset();
        delete iRangeName;        
        }
    iSupportedRange.Close();
    if ( iCharacterSet )
        {
        delete iCharacterSet;        
        }        
    if ( iShortcutList )
        {
        iShortcutList->Reset();
        delete iShortcutList;
        }
    delete iEngine;
    }

// ---------------------------------------------------------------------------
// Set current language script
// ---------------------------------------------------------------------------
//
void CTruiEngine::SetLanguageScriptByIndexL( TInt aIndex )
    {
    //Set script
    iCurrentScript = aIndex;
    
    //Delete previous iRangeName
    if ( iRangeName )
        {
        delete iRangeName;
        iRangeName = NULL;
        }
    //Reset iSupportedRange
    iSupportedRange.Reset();
    
    //Set character range
    HBufC* charRange;
    switch( iCurrentScript )
        {
        case EMainViewSubmenuWestern:
            {
            //There are 5 subranges under Western
            iRangeName = new (ELeave) CDesCArrayFlat( KRangeNamesArraySize ); 
            charRange = iEnv->AllocReadResourceLC( R_TRUI_MAINVIEW_CHAR_RANGE_LOWER );
            iRangeName->AppendL( *charRange );
            CleanupStack::PopAndDestroy( charRange );
            iSupportedRange.Append( ELatinLowerLetters );
            
            charRange = iEnv->AllocReadResourceLC( R_TRUI_MAINVIEW_CHAR_RANGE_CAPITAL );
            iRangeName->AppendL( *charRange );
            CleanupStack::PopAndDestroy( charRange );
            iSupportedRange.Append( ELatinUpperLetters );
            
            charRange = iEnv->AllocReadResourceLC( R_TRUI_MAINVIEW_CHAR_RANGE_ACCENT );
            iRangeName->AppendL( *charRange );
            CleanupStack::PopAndDestroy( charRange );
            iSupportedRange.Append( ELatinAccent );

            charRange = iEnv->AllocReadResourceLC( R_TRUI_MAINVIEW_CHAR_RANGE_DIGITS );
            iRangeName->AppendL( *charRange );
            CleanupStack::PopAndDestroy( charRange );
            iSupportedRange.Append( ELatinDigital );
            
            charRange = iEnv->AllocReadResourceLC( R_TRUI_MAINVIEW_CHAR_RANGE_SYMBOLS );
            iRangeName->AppendL( *charRange );
            CleanupStack::PopAndDestroy( charRange );
            iSupportedRange.Append( ELatinSymbol );
            
            iPtiEngine->SetLanguageScriptL( EPtiHwrScriptLatin );
            break;
            }
        case EMainViewSubmenuCyrillic:
            {
            //There are 5 subranges under Cyrillic
            iRangeName = new (ELeave) CDesCArrayFlat( KRangeNamesArraySize );
            charRange = iEnv->AllocReadResourceLC( R_TRUI_MAINVIEW_CYRILLIC_LOWER );
            iRangeName->AppendL( *charRange );
            CleanupStack::PopAndDestroy( charRange );
            iSupportedRange.Append( ECyrillicLowerLetters );
            
            charRange = iEnv->AllocReadResourceLC( R_TRUI_MAINVIEW_CYRILLIC_CAPITAL );
            iRangeName->AppendL( *charRange );
            CleanupStack::PopAndDestroy( charRange );
            iSupportedRange.Append( ECyrillicUpperLetters );

            charRange = iEnv->AllocReadResourceLC( R_TRUI_MAINVIEW_CHAR_RANGE_DIGITS );
            iRangeName->AppendL( *charRange );
            CleanupStack::PopAndDestroy( charRange );
            iSupportedRange.Append( ELatinDigital );            

            charRange = iEnv->AllocReadResourceLC( R_TRUI_MAINVIEW_CHAR_RANGE_SYMBOLS );
            iRangeName->AppendL( *charRange );
            CleanupStack::PopAndDestroy( charRange );
            iSupportedRange.Append( ELatinSymbol );
            
            iPtiEngine->SetLanguageScriptL( EPtiHwrScriptCyrillic );
            break;
            }
        case EMainViewSubmenuGreek:
            {
            //There are 5 subranges under Greek
            iRangeName = new (ELeave) CDesCArrayFlat( KRangeNamesArraySize );
            charRange = iEnv->AllocReadResourceLC( R_TRUI_MAINVIEW_GREEK_LOWER );
            iRangeName->AppendL( *charRange );
            CleanupStack::PopAndDestroy( charRange );
            iSupportedRange.Append( EGreekLowerLetters );
            
            charRange = iEnv->AllocReadResourceLC( R_TRUI_MAINVIEW_GREEK_CAPITAL );
            iRangeName->AppendL( *charRange );
            CleanupStack::PopAndDestroy( charRange );
            iSupportedRange.Append( EGreekUpperLetters );
            
            charRange = iEnv->AllocReadResourceLC( R_TRUI_MAINVIEW_CHAR_RANGE_DIGITS );
            iRangeName->AppendL( *charRange );
            CleanupStack::PopAndDestroy( charRange );
            iSupportedRange.Append( ELatinDigital );

            charRange = iEnv->AllocReadResourceLC( R_TRUI_MAINVIEW_CHAR_RANGE_SYMBOLS );
            iRangeName->AppendL( *charRange );
            CleanupStack::PopAndDestroy( charRange );
            iSupportedRange.Append( ELatinSymbol );
            
            iPtiEngine->SetLanguageScriptL( EPtiHwrScriptGreek );
            break;
            }
        case EMainViewSubmenuHebrew:
            {
            //There are 4 subranges under Hebrew
            iRangeName = new (ELeave) CDesCArrayFlat( KRangeNamesArraySize );
            charRange = iEnv->AllocReadResourceLC( R_TRUI_MAINVIEW_HEBREW_LETTERS );
            iRangeName->AppendL( *charRange );
            CleanupStack::PopAndDestroy( charRange );
            iSupportedRange.Append( EHebrewLetters );
   
            charRange = iEnv->AllocReadResourceLC( R_TRUI_MAINVIEW_CHAR_RANGE_DIGITS );
            iRangeName->AppendL( *charRange );
            CleanupStack::PopAndDestroy( charRange );
            iSupportedRange.Append( ELatinDigital );

            charRange = iEnv->AllocReadResourceLC( R_TRUI_MAINVIEW_CHAR_RANGE_SYMBOLS );
            iRangeName->AppendL( *charRange );
            CleanupStack::PopAndDestroy( charRange );
            iSupportedRange.Append( ELatinSymbol );
            
            iPtiEngine->SetLanguageScriptL( EPtiHwrScriptHebrew );
            break;
            }
        default:
            {
            User::Leave( KErrArgument );
            }
        } 
        // Set the first range as the default range
        SetCharacterRangeByIndexL( 0 );
    }
   
// ---------------------------------------------------------------------------
// Set current character range
// ---------------------------------------------------------------------------
// 
void CTruiEngine::SetCharacterRangeByIndexL( TInt aIndex )
    {
    iCurrentRange = iSupportedRange[aIndex];
    
    //Update character set by range
    TInt resourceId= 0;
    switch( iCurrentRange )
        {
        case ELatinLowerLetters:
            {
            resourceId = R_TRUI_CHARACTERSET_LATIN_LOWERLETTERS;
            break;
            }
        case ELatinUpperLetters:
            {
            resourceId = R_TRUI_CHARACTERSET_LATIN_UPPERLETTERS;
            break;
            }
        case ELatinAccent:
            {
            resourceId = R_TRUI_CHARACTERSET_LATIN_ACCENT;
            break;
            }
        case ELatinDigital:
            {
            resourceId = R_TRUI_CHARACTERSET_LATIN_DIGITAL;
            break;
            }
        case ELatinSymbol:
            {
            if ( FeatureManager::FeatureSupported(KFeatureIdChinese) )
                {
                resourceId = R_TRUI_CHARACTERSET_LATIN_SYMBOL_CHINESE;
                }
            else
                {
                resourceId = R_TRUI_CHARACTERSET_LATIN_SYMBOL;
                }                        
            break;
            }
        case ECyrillicLowerLetters:
            {
            // According to current input language,
            // Return different resource id.            
            resourceId = CyrillicLowerResourceId( iInputLanguage );
            break;
            }
        case ECyrillicUpperLetters:
            {
            resourceId = CyrillicUpperResourceId( iInputLanguage );
            break;
            }
        case ECyrillicAccent:
            {
            resourceId = R_TRUI_CHARACTERSET_CYRILLIC_ACCENT;
            break;
            }
        case EGreekLowerLetters:
            {
            resourceId = R_TRUI_CHARACTERSET_GREEK_LOWERLETTERS;
            break;
            }
        case EGreekUpperLetters:
            {
            resourceId = R_TRUI_CHARACTERSET_GREEK_UPPERLETTERS;
            break;
            }
        case EHebrewLetters:
            {
            resourceId = R_TRUI_CHARACTERSET_HEBREW_LETTERS;
            break;
            }
        default:
            {
            User::Leave( KErrArgument );
            }
        }
    if ( iCharacterSet )
        {
        delete iCharacterSet;
        iCharacterSet = NULL;
        }
    iCharacterSet = iEnv->AllocReadResourceL( resourceId );
    
    //Transfer character range to engine's subrange and store it in local
    SetSubrange();
    }
    
// ---------------------------------------------------------------------------
// Check if character has model
// ---------------------------------------------------------------------------
//
TBool CTruiEngine::CheckCharacterModel( const TDesC& aChar )
    {
    return iPtiEngine->CharacterModelExist( aChar );
    }

// ---------------------------------------------------------------------------
// Delete character model
// ---------------------------------------------------------------------------
//
TInt CTruiEngine::DeleteCharacterModel( const TDesC& aChar )
    {
    return iPtiEngine->DeleteCharacterModel( aChar );
    }

// ---------------------------------------------------------------------------
// Set character model
// ---------------------------------------------------------------------------
//
void CTruiEngine::SetCharacterModelL( const TDesC& aChar,
                                      RArray<TPoint>& aModel,
                                      TDes& aSimilarMsg )
    {
    iPtiEngine->SetCharacterModelL( aChar, aModel, iSubrange, aSimilarMsg );
    }

// ---------------------------------------------------------------------------
// Get character model
// ---------------------------------------------------------------------------
// 
void CTruiEngine::GetCharacterModelL( const TDesC& aChar, RArray<TPoint>& aModel )
    {
    iPtiEngine->GetCharacterModelL( aChar, aModel );     
    }

// ---------------------------------------------------------------------------
// Check if shortcut has model
// ---------------------------------------------------------------------------
//
TBool CTruiEngine::CheckShortcutModel( const TDesC& aText )
    {
    return iPtiEngine->CheckShortcutModel( aText );
    }
    
// ---------------------------------------------------------------------------
// Save shortcut text and its assigned character model into PTI Engine
// Meanwhile update shortcutlist
// ---------------------------------------------------------------------------
//
void CTruiEngine::SaveShortcutL( const TDesC& aText,
                                 RArray<TPoint>& aModel,
                                 TDes& aSimilarMsg )
    {
    iPtiEngine->SetShortcutModelL( aText, aModel, aSimilarMsg );    
    }

// ---------------------------------------------------------------------------
// Delete shortcut text and its assigned character model from PTI Engine
// Meanwhile update shortcutlist
// ---------------------------------------------------------------------------
//
TInt CTruiEngine::DeleteShortcut( const TDesC& aText )
    {
    return iPtiEngine->DeleteShortcutModel( aText );   
    }

// ---------------------------------------------------------------------------
// Set current edited shortcut
// ---------------------------------------------------------------------------
//
TInt CTruiEngine::SetShortcut( const TDesC& aText )
    {
    iShortcut = aText;
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// Update Shortcut list
// ---------------------------------------------------------------------------
//
void CTruiEngine::UpdateShortcutListL()
    {
    if ( iShortcutList )
        {
        iShortcutList->Reset();
        delete iShortcutList;
        iShortcutList = NULL;
        }
        
    RPointerArray<HBufC> list;
    CleanupClosePushL( list );    
    TRAPD( err, iPtiEngine->GetShortcutTextListL( list ) );   
    if ( err == KErrNone )
        {
        if ( list.Count() > 0 )
            {
            iShortcutList = new (ELeave) CDesCArrayFlat( list.Count() );
            for ( TInt i = 0; i < list.Count(); i++ )
                {
                iShortcutList->AppendL( *list[i]  );
                }
            iShortcutList->Sort( ECmpCollated );  
            }
        }
    list.ResetAndDestroy();               
    CleanupStack::PopAndDestroy( &list );    
    }

// ---------------------------------------------------------------------------
// From class MTruiEngine.
// change the text of an existing shortcut.
// ---------------------------------------------------------------------------
//
void CTruiEngine::ChangeShortcutTextL( const TDesC& aNewText )
    {
    iPtiEngine->ChangeShortcutTextL( iShortcut, aNewText );
    }

// ---------------------------------------------------------------------------
// Get guiding line pos
// ---------------------------------------------------------------------------
//
TInt CTruiEngine::GetGuidingLinePos( const TSize& aSize, TInt& aTop , TInt& aBottom )
    {
    return iPtiEngine->GetGuidingLinePos( aSize, aTop, aBottom );
    }

// ---------------------------------------------------------------------------
// Get the number of the maximum of shortcuts
// ---------------------------------------------------------------------------
//
TInt CTruiEngine::MaxShortCutLength()
    {
    return iPtiEngine->MaxShortCutLength();
    }

// ---------------------------------------------------------------------------
// Get trails assigned to the specified shortcut.
// ---------------------------------------------------------------------------
//
void CTruiEngine::GetShortcutModelL( const TDesC& aText, 
                                     RArray<TPoint>& aModel, 
                                     TUint& aUnicode )
    {
    iPtiEngine->GetShortcutModelL( aText, aModel, aUnicode );
    }

// ---------------------------------------------------------------------------
// Save preset shortcuts.
// ---------------------------------------------------------------------------
//
void CTruiEngine::SavePresetShortcutL( TUint aUnicode, const TDesC& aShortcut )
    {
    iPtiEngine->SavePresetShortcutL( aUnicode,  aShortcut );
    }

// ---------------------------------------------------------------------------
// Get trails assigned to a preset text.
// ---------------------------------------------------------------------------
//
void CTruiEngine::GetPresetSymbolByUnicodeL( TUint aUnicode, 
                                             RArray<TPoint>& aModel, 
                                             TDes& aShortcut )
    {
    iPtiEngine->GetPresetSymbolByUnicodeL( aUnicode, aModel, aShortcut );
    }
    
// ---------------------------------------------------------------------------
// Return all unicodes of preset text stored in PTI engine.
// ---------------------------------------------------------------------------
//
void CTruiEngine::GetAllPresetSymbolsL( RArray<TUint>& aPresets )
    {
    iPtiEngine->GetAllPresetSymbolsL( aPresets );
    }

// ---------------------------------------------------------------------------
// Transform character range to engine's subrange
// ---------------------------------------------------------------------------
//
void CTruiEngine::SetSubrange()
    {
    switch( iCurrentRange )
        {
        case ELatinLowerLetters:
        case ELatinUpperLetters:
        case ELatinAccent:
            {
            iSubrange = EPtiHwrRangeLanguage;
            break;
            }
        case ELatinDigital:
            {
            iSubrange = EPtiHwrRangeNumber;
            break;
            }
        case ELatinSymbol:
            {
            iSubrange = EPtiHwrRangeSymbol;
            break;
            }
        case ECyrillicLowerLetters:
        case ECyrillicUpperLetters:
        case ECyrillicAccent:
            {
            iSubrange = EPtiHwrRangeLanguage;
            break;
            }
        case EGreekLowerLetters:
        case EGreekUpperLetters:
            {
            iSubrange = EPtiHwrRangeLanguage;
            break;
            }
        case EHebrewLetters:
            {
            iSubrange = EPtiHwrRangeLanguage;
            break;
            }
        default:
            {
            iSubrange = EPtiHwrRangeAny;
            }
        }
    }

// ---------------------------------------------------------------------------
// Initialize the tabel used to map language to language script
// ---------------------------------------------------------------------------
//
void CTruiEngine::InitLanguageRelationL( TInt aResourceId, 
                               RArray<TLanguageRelation>& aLanguageRelation )
    {
    TResourceReader reader;
    iEnv->CreateResourceReaderLC( reader, aResourceId );
    TInt script_count = reader.ReadInt16();
    for ( TInt i = 0; i < script_count; i++ )
        {
        TInt scriptId = reader.ReadInt16();
        TInt lang_count = reader.ReadInt16();
        for ( TInt j = 0; j < lang_count; j++ )
            {
            TInt langCode = reader.ReadInt16();
            aLanguageRelation.Append( TLanguageRelation( langCode, scriptId ) );
            }
        }        
    CleanupStack::PopAndDestroy(); // reader    
    }
    
// ---------------------------------------------------------------------------
// Initialize the supported language script. 
// ---------------------------------------------------------------------------
//
void CTruiEngine::InitSupportedLanguageScriptL( RArray<TInt>& aSupportedScript,
                                                CPtiEngine* aEngine,
                          const RArray<TLanguageRelation>& aLanguageRelation )
    {
    // Get all supported input language.
    RArray<TInt> avaiableLanuage;
    CleanupClosePushL( avaiableLanuage );
    aEngine->GetAvailableLanguagesL( avaiableLanuage );
    avaiableLanuage.Sort();
           
    // Map language to language script utilizing TLanguageRelation
    for ( TInt i = 0; i < aLanguageRelation.Count(); i++ )
        {        
        TInt languageScriptId = aLanguageRelation[i].LanguageScriptId();
        if ( aSupportedScript.Find( languageScriptId ) == KErrNotFound )
            {
            TInt languageCode = aLanguageRelation[i].LanguageCode();
            if ( avaiableLanuage.FindInOrder( languageCode ) != KErrNotFound )
                {
                // Add language to aSupportedScript
                aSupportedScript.Append( languageScriptId );
                }
            }
        }
        
    CleanupStack::PopAndDestroy( &avaiableLanuage );
    }

// ---------------------------------------------------------------------------
// Initialize the supported language script. 
// ---------------------------------------------------------------------------
//    
TInt CTruiEngine::GetActiveLanguageScript( TInt aLanguageCode, 
                         const RArray<TLanguageRelation>& aLanguageRelation )
    {
    for ( TInt i = 0; i < aLanguageRelation.Count(); i++ )
        {
        if ( aLanguageCode == aLanguageRelation[i].LanguageCode() )
            {
            return aLanguageRelation[i].LanguageScriptId();
            }
        }    
    return KErrNotFound;
    }

// ---------------------------------------------------------------------------
// Get the resourc id of lower case of Cyrillic 
// according to the current input language.
// ---------------------------------------------------------------------------
//    
TInt CTruiEngine::CyrillicLowerResourceId( TLanguage aLanguage )
    {
    if ( aLanguage == ELangBulgarian )
        {
        return R_TRUI_CHARACTERSET_CYRILLIC_LOWERLETTERS_BULGARIAN;
        }
    else if ( aLanguage == ELangRussian )
        {
        return R_TRUI_CHARACTERSET_CYRILLIC_LOWERLETTERS_RUSSIAN;
        }
    else if ( aLanguage == ELangUkrainian )
        {
        return R_TRUI_CHARACTERSET_CYRILLIC_LOWERLETTERS_UKRAINE;
        }
    else
        {
        return R_TRUI_CHARACTERSET_CYRILLIC_LOWERLETTERS;
        }                
    }

// ---------------------------------------------------------------------------
// Get the resourc id of upper case of Cyrillic 
// according to the current input language.
// ---------------------------------------------------------------------------
// 
TInt CTruiEngine::CyrillicUpperResourceId( TLanguage aLanguage )
    {
    if ( aLanguage == ELangBulgarian )
        {
        return R_TRUI_CHARACTERSET_CYRILLIC_UPPERLETTERS_BULGARIAN;
        }
    else if ( aLanguage == ELangRussian )
        {
        return R_TRUI_CHARACTERSET_CYRILLIC_UPPERLETTERS_RUSSIAN;
        }
    else if ( aLanguage == ELangUkrainian )
        {
        return R_TRUI_CHARACTERSET_CYRILLIC_UPPERLETTERS_UKRAINE;
        }
    else
        {
        return R_TRUI_CHARACTERSET_CYRILLIC_UPPERLETTERS;
        }                    
    }

