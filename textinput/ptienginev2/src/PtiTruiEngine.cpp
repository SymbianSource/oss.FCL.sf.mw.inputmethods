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
* Description:   Implement of class CTruiPtiEngine
*
*/



#include <bautils.h>

#include "PtiTruiEngine.h"
#include "PtiSymbolList.h"


// ===================== Constant Definition =======================
_LIT( KSymbolPath, "c:\\PredicH\\TrainUISymbol.dat" );    
_LIT( KShortCutPath, "c:\\PredicH\\TrainUiShortcut.dat" ); 
_LIT( KPresetPath, "c:\\PredicH\\TrainUiPreset.dat");  
_LIT( KPresetRomPath, "z:\\resource\\TrainUiPreset.dat" );	
	
// ======== MEMBER FUNCTIONS ========

CTruiPtiEngine::CTruiPtiEngine()
    {
    iScript = EPtiHwrScriptAny;
    iHl = 0;
    iBl = 0;
    }

void CTruiPtiEngine::ConstructL( CHwrRecognizer* aHwrEngine )
    {
    // Load the file
    iSymboList.Reset();
    iHwrEngine = aHwrEngine;
    User::LeaveIfError( iFsSession.Connect() );
    }


CTruiPtiEngine* CTruiPtiEngine::NewL( CHwrRecognizer* aHwrEngine )
    {
    CTruiPtiEngine* self = CTruiPtiEngine::NewLC( aHwrEngine );
    CleanupStack::Pop( self );
    return self;
    }

CTruiPtiEngine* CTruiPtiEngine::NewLC( CHwrRecognizer* aHwrEngine )
    {
    CTruiPtiEngine* self = new( ELeave ) CTruiPtiEngine;
    CleanupStack::PushL( self );
    self->ConstructL( aHwrEngine );
    return self;
    }

CTruiPtiEngine::~CTruiPtiEngine()
    {
    iSymboList.DeleteAll();
    delete iPreSetList;
    iPreSetList = NULL;
    iFsSession.Close();
    }

// ---------------------------------------------------------------------------
// From class MTruiEngine.
//  Set current trained character's Language Script.
// ---------------------------------------------------------------------------
//
void CTruiPtiEngine::SetLanguageScriptL( TInt aLanguageScript )
    {
    iScript = aLanguageScript;
    
    LoadUdmL( EUdmSymbol );
    LoadUdmL( EUdmShortcut );
    }
        
// ---------------------------------------------------------------------------
// From class MTruiEngine.
// Check if character has model.
// ---------------------------------------------------------------------------
//
TBool CTruiPtiEngine::CharacterModelExist( const TDesC& aChar )
    {
    TRAPD( err, LoadUdmL( EUdmSymbol ) );
    if ( err != KErrNone )
        {
        return err;
        }

    THwrUdmRange range = { iScript, EPtiHwrRangeAny  };
    return iSymboList[EUdmSymbol]->CheckSymbolModel( aChar, range );
    }

// ---------------------------------------------------------------------------
// From class MTruiEngine.
// Get character model.
// ---------------------------------------------------------------------------
//
void CTruiPtiEngine::GetCharacterModelL( const TDesC& aChar, RArray<TPoint>& aModel )
    {
    LoadUdmL( EUdmSymbol );    
    THwrUdmRange range = { iScript, EPtiHwrRangeAny  };
    TUint presetCode = 0;
    iSymboList[EUdmSymbol]->GetSymbolModelL( aChar, aModel, presetCode,range );
    }
	
	
// ---------------------------------------------------------------------------
// From class MTruiEngine.
// Set character model.
// ---------------------------------------------------------------------------
//
void CTruiPtiEngine::SetCharacterModelL( const TDesC& aChar, RArray<TPoint>& aModel,  TInt aSubRange, TDes& aSimilarChar )
    {
    THwrUdmRange script = { iScript, aSubRange };
    User::LeaveIfError( iHwrEngine->SymbolModelValid( aChar, aModel, script, aSimilarChar ) );
    
    LoadUdmL( EUdmSymbol );
    THwrUdmRange range = { iScript, aSubRange };
    iSymboList[EUdmSymbol]->SetSymbolModelL( aChar, aModel, iHl,iBl,range );
    }
    
// ---------------------------------------------------------------------------
// From class MTruiEngine.
// Delete character model.
// ---------------------------------------------------------------------------
//
TInt CTruiPtiEngine::DeleteCharacterModel( const TDesC& aChar )
    {
   TRAPD( errUdm, LoadUdmL( EUdmSymbol ) );
    if ( errUdm != KErrNone )
        {
        return errUdm;
        }

    THwrUdmRange range = { iScript, EPtiHwrRangeAny  };
    TRAPD( err,iSymboList[EUdmSymbol]->DeleteSymbolModelL( aChar,  range ) );
    return err;
    }

// ---------------------------------------------------------------------------
// From class MTruiEngine.
// Get text list of all the shortcuts.
// ---------------------------------------------------------------------------
//
void CTruiPtiEngine::GetShortcutTextListL( RPointerArray<HBufC>& aShortcutTextList )
    {
    LoadUdmL( EUdmShortcut );       
    THwrUdmRange range = { EPtiHwrScriptAny, EPtiHwrRangeAny  };
    iSymboList[EUdmShortcut]->GetModelTextListL( aShortcutTextList, range );
    }

// ---------------------------------------------------------------------------
// From class MTruiEngine.
// Check if shortcut has model.
// ---------------------------------------------------------------------------
//
TBool CTruiPtiEngine::CheckShortcutModel( const TDesC& aText )
    {
    TRAPD( err, LoadUdmL( EUdmShortcut ) );    
    if ( err != KErrNone )
        return EFalse;
    
    THwrUdmRange range = { EPtiHwrScriptAny, EPtiHwrRangeAny };
    return iSymboList[EUdmShortcut]->CheckSymbolModel( aText, range );
    }

// ---------------------------------------------------------------------------
// From class MTruiEngine.
// Get Shortcut model.
// ---------------------------------------------------------------------------
//
void CTruiPtiEngine::GetShortcutModelL( const TDesC& aText, RArray<TPoint>& aModel,TUint& aUnicode )
    {
    LoadUdmL( EUdmShortcut );    
    THwrUdmRange range = { EPtiHwrScriptAny, EPtiHwrRangeAny  };
    iSymboList[EUdmShortcut]->GetSymbolModelL( aText, aModel, aUnicode, range );
    }

// ---------------------------------------------------------------------------
// From class MTruiEngine.
// Set Shortcut model.
// ---------------------------------------------------------------------------
void CTruiPtiEngine::SetShortcutModelL( const TDesC& aText, RArray<TPoint>& aModel, TDes& aSimilarText  )
    {
    THwrUdmRange script = { iScript, EPtiHwrRangeAny };
    User::LeaveIfError( iHwrEngine->SymbolModelValid( aText, aModel, script, aSimilarText ) );
    
    LoadUdmL( EUdmShortcut );
    THwrUdmRange range = { EPtiHwrScriptAny, EPtiHwrRangeAny  };
    return iSymboList[EUdmShortcut]->SetSymbolModelL( aText, aModel, iHl, iBl, range );
    }
    
// ---------------------------------------------------------------------------
// From class MTruiEngine.
// Delete Shortcut model.
// ---------------------------------------------------------------------------
TInt CTruiPtiEngine::DeleteShortcutModel( const TDesC& aText )
    {
    TRAPD( errUdm, LoadUdmL( EUdmShortcut ) );
    if ( errUdm != KErrNone )
        {
        return errUdm;
        }
    
    THwrUdmRange range = { EPtiHwrScriptAny, EPtiHwrRangeAny  };
    TRAPD( err,iSymboList[EUdmShortcut]->DeleteSymbolModelL( aText, range ) );
    return err;
    }
        
// ---------------------------------------------------------------------------
// From class MTruiEngine.
// hange the text of an existing shortcut.
// ---------------------------------------------------------------------------
//
void CTruiPtiEngine::ChangeShortcutTextL( const TDesC& aOldText,
                                         const TDesC& aNewText )
    {
    LoadUdmL( EUdmShortcut );
        
    THwrUdmRange range = { EPtiHwrScriptAny, EPtiHwrRangeAny  };
    TRAP_IGNORE( iSymboList[EUdmShortcut]->ChangeSymbolTextL( aOldText, aNewText, range ) );
    }

// ---------------------------------------------------------------------------
// Get guiding line pos
// ---------------------------------------------------------------------------
//
TInt CTruiPtiEngine::GetGuidingLinePos(  const TSize& aSize, TInt& aTop , TInt&  aBottom)
    {
    iHl = aTop = aSize.iHeight * 65 / 135;
    iBl = aBottom = aSize.iHeight * 88 / 135;
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// Get the number of the maximum of shortcuts
// ---------------------------------------------------------------------------
//
TInt CTruiPtiEngine::MaxShortCutLength()
    {
    return 10;
    }


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CTruiPtiEngine::SavePresetShortcutL( TUint aUnicode, const TDesC& aShortcut ) 
    {
    LoadUdmL( EUdmShortcut );
    iSymboList[EUdmShortcut]->SavePresetShortcutL( aUnicode, aShortcut ) ;
    }
    
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CTruiPtiEngine::GetAllPresetSymbolsL( RArray<TUint>& aPresets ) 
    {
    LoadUdmL( EUdmShortcut );
    iSymboList[EUdmShortcut]->GetAllPresetSymbolsL( aPresets ) ;
    }
    
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CTruiPtiEngine::GetPresetSymbolByUnicodeL( TUint aUnicode, RArray<TPoint>& aModel, TDes& aShortcut ) 
    {
    LoadUdmL( EUdmShortcut );
    iSymboList[EUdmShortcut]->GetPresetSymbolByUnicodeL( aUnicode, aModel, aShortcut ) ;
    }

void CTruiPtiEngine::LoadUdmL( TUdmType aType )
    {
    if ( iSymboList[aType] )
        {
        return;
        }
    
    iSymboList[EUdmSymbol] =  CSymbolList::NewL( KSymbolPath(), NULL  );    
    iSymboList[EUdmSymbol]->ExternalizeL();
    
    // well , EUdmPreset belong to shourtcut
    if ( !iPreSetList )
        {
        // copy the data from rom if not exist on KPresetPath
        TInt retCode = BaflUtils::CopyFile( iFsSession, KPresetRomPath(), KPresetPath(), 0 );
        TUint attPresetDataFile = 0;
        iFsSession.Att( KPresetPath, attPresetDataFile );
        if ( attPresetDataFile & KEntryAttReadOnly )
        	{
        	attPresetDataFile &= ~KEntryAttReadOnly;
        	iFsSession.SetAtt( KPresetPath(), attPresetDataFile, KEntryAttReadOnly );
        	}
        if ( KErrNone == retCode || KErrAlreadyExists == retCode )
            {
            iPreSetList = CSymbolList::NewL( KPresetPath(), NULL ); 
            }
        }   
    iSymboList[EUdmShortcut] =  CSymbolList::NewL( KShortCutPath(), iPreSetList ); 
    }
    
void CTruiPtiEngine::UnLoadUdm(  TUdmType aType )
    {
    if ( iSymboList[aType] )
        {
        delete iSymboList[aType];
        iSymboList[aType] = NULL;
        }
    }

void CTruiPtiEngine::GetModelIndexListL( TUdmType aType, RArray<TInt>& aList, const THwrUdmRange& aRange)
    {
    CSymbolList* temp = iSymboList[aType];
    TBool tempB = !iSymboList[aType];
    LoadUdmL( aType );

    THwrUdmRange range = aRange;
    ConvertUdmRange( range );
    iSymboList[aType]->GetModelIndexListL( aList, range );
    }


     
void CTruiPtiEngine::GetSymbolModelL(  TUdmType aType,TInt aIndex, TPtrC& aSymbolName,RArray<TPoint>& aModel, TInt& aHelpLine, TInt& aBaseLine )
    {
    LoadUdmL( aType );
    
    iSymboList[aType]->GetSymbolDataRef( aIndex, aSymbolName, aModel, aHelpLine, aBaseLine);
    }

void CTruiPtiEngine::ConvertUdmRange( THwrUdmRange& aRange )
    {
    switch ( aRange.iScript )
        {
        case ELangEnglish:
        case ELangAustralian:
        case ELangNewZealand:
        case ELangInternationalEnglish:
        case ELangSouthAfricanEnglish:
        case ELangCanadianEnglish:
        case ELangAmerican:
        case ELangSwissFrench:
        case ELangBelgianFrench:
        case ELangInternationalFrench:
        case ELangFrench:
        case ELangCanadianFrench:
        case ELangGerman:
        case ELangSwissGerman:
        case ELangSpanish:
        case ELangLatinAmericanSpanish:
        case ELangSwissItalian:
        case ELangItalian:
        case ELangFinlandSwedish:
        case ELangSwedish:
        case ELangFinnish:
//        case ELangAlbanian:
        case ELangDanish:
        case ELangDutch:
        case ELangIcelandic:
        case ELangNorwegian:
        case ELangPortuguese:
        case ELangIndonesian:
        case ELangMalay:
        case ELangTagalog:
        case ELangCroatian:
        case ELangCzech:
        case ELangHungarian:
        case ELangPolish:
        case ELangRomanian:
        case ELangSlovak:
        case ELangSlovenian:
//        case ELangSerbian:
        case ELangEstonian:
        case ELangLatvian:
        case ELangLithuanian:
        case ELangTurkish:
            aRange.iScript = EPtiHwrScriptLatin;
            break;
        case ELangGreek:
        case ELangCyprusGreek:
            aRange.iScript = EPtiHwrScriptGreek;
            break; 
        case ELangBelarussian:
        case ELangRussian:
        case ELangUkrainian:
        case ELangBulgarian:
            aRange.iScript = EPtiHwrScriptCyrillic;
            break;
        case ELangHebrew:
            aRange.iScript = EPtiHwrScriptHebrew;
            break;
        case ELangArabic:
            aRange.iScript = EPtiHwrScriptArabic;
            break;
        default:
            break;
        }
    }

// end of line
