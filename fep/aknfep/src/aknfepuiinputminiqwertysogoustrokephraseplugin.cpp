/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:           
 *      Provides the TAknFepInputMiniQwertySogouStrokePhrasePlugin definition.
 *
*/

// System includes
#include <e32cmn.h>
#include <PtiEngine.h>
#include <PtiDefs.h>
#include <avkon.rsg>
#include <aknfep.rsg>
#include <PtiUserDicEntry.h>
#include <aknnotewrappers.h> //CAknWarningNote 
// User includes
#include "AknFepUIManagerStateInterface.h"
#include "AknFepUICtrlInputPane.h"
#include "AknFepUICtrlPinyinPopup.h"        //Pinyin phrase
#include "aknfepuictrleditpane.h"           //pinyin phrase creation
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepManager.h"

#include "aknfepuiinputminiqwertysogoustrokephraseplugin.h"
#include "AknFepUiInputStateCandidateQwertyBaseChinesePhrase.h"

// Constant definition
const TInt KMaxShowKeystrokeCount = 255;
const TInt KMaxPhraseCreationCount = 10;
const TInt KMaxKeystrokeCount = 31;
const TInt16 KStrokeDelimiter = 0x2022;

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertySogouStrokePhrasePlugin::TAknFepInputMiniQwertySogouStrokePhrasePlugin
// C++ default constructor
// ---------------------------------------------------------------------------
//
TAknFepInputMiniQwertySogouStrokePhrasePlugin::TAknFepInputMiniQwertySogouStrokePhrasePlugin(
    MAknFepUIManagerStateInterface* aOwner,
    MAknFepUICtrlContainerChinese* aUIContainer,
    TUIState aState )
    : iOwner( aOwner ),
      iUIContainer( aUIContainer ),
      iState( aState ),
      iValid( EFalse )
    {
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertySogouStrokePhrasePlugin::operator=
// 
// ---------------------------------------------------------------------------
//
void TAknFepInputMiniQwertySogouStrokePhrasePlugin::operator=( const 
        TAknFepInputMiniQwertySogouStrokePhrasePlugin& aPlugin )
    {
    Mem::Copy( this, &aPlugin, sizeof(TAknFepInputMiniQwertySogouStrokePhrasePlugin));
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertySogouStrokePhrasePlugin::Enable
// Enable or disable the plugin.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertySogouStrokePhrasePlugin::Enable( TBool aValid )
    {
    iValid = aValid;
    }
        
// ---------------------------------------------------------
// TAknFepInputMiniQwertySogouStrokePhrasePlugin::IsEnable
// Get the state of the plugin.
// ---------------------------------------------------------
//
TBool TAknFepInputMiniQwertySogouStrokePhrasePlugin::IsEnable()
    {
    return iValid;
    }

// ---------------------------------------------------------------------------
// TAknFepInputMiniQwertySogouStrokePhrasePlugin::CommitInlineEEPL
// Commint text to EEP or editor
// ---------------------------------------------------------------------------
//
TBool TAknFepInputMiniQwertySogouStrokePhrasePlugin::CommitInlineEEPL(
    const TDesC& aDes )
    {
    TInt charCount = aDes.Length();
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    MAknFepUICtrlEditPane* editpane = iUIContainer->EditPaneWindow();
    CDesCArrayFlat* keyStoke = editpane->KeystrokeArray();
    RArray<TInt>* keycode = editpane->KeycodeArray();
    CDesCArrayFlat* phraseArray = editpane->PhraseArray();
    CDesCArrayFlat* phraseShowKeyStroke = editpane->PhraseShowKeyStrokeArray();
    RArray<TInt>* phraseKeycode = editpane->PhraseKeycodeArray();
    TBuf<KMaxKeystrokeCount> buf;
    TBuf<1> strokeDLT;
    strokeDLT.Append( KStrokeDelimiter );

    phraseArray->AppendL( aDes );

    TInt phraseCount = phraseArray->Count();
    TInt index = 0;
    TInt isFound = -1;
    
    TInt strokeDLTCount = 0;
    for ( TInt j = 0; j < keyStoke->Count(); j++ )
        {
        if ( keyStoke->MdcaPoint( j ) == strokeDLT )
            {
            strokeDLTCount++;
            if ( strokeDLTCount == charCount )
                {
                index = j;
                }
            isFound = 0;
            }
        }
    
    TInt phraseCharacterCount = 0;
    for ( TInt m = 0; m < phraseCount; m++ )
        {
        phraseCharacterCount += (*phraseArray)[m].Length();
        }

    //phrase creation state
    if ( phraseCount > 1 || ( 0 == isFound && charCount <= strokeDLTCount ))
        {
        editpane->SetPhraseCreationFlag( ETrue );

        // Tell sogou core which phrase is selected. It is used for phrase creation.
        TBuf<KMaxPhraseCreationCount> phraseSelected;
        phraseSelected.FillZ();
        if ( aDes.Length() <= KMaxPhraseCreationCount )
            {
            phraseSelected.Append( aDes );
            iOwner->PtiEngine()->HandleCommandL( EPtiCommandSelectElement, 
                    ( TAny* )( &phraseSelected ));
            }
        
        if ( 0 == isFound && index < keyStoke->Count() - 1 )
            {
            for ( TInt i = 0; i <= index; i++ )
                {
                TPtrC ptr = keyStoke->MdcaPoint( 0 );
                buf.Append( ptr );
                keyStoke->Delete( 0);
                phraseKeycode->AppendL( (*keycode)[0]);
                keycode->Remove( 0 );
                }
            phraseShowKeyStroke->AppendL( buf );
            editpane->SetCursorIndexOfKeystroke( 0 );
            editpane->DisableCursor();          
            return ETrue;
            }
        else
            {
            HBufC* phraseCreated = HBufC::NewLC( phraseCharacterCount );
            TPtr keyPtr = phraseCreated->Des();
            for ( TInt ii = 0; ii < phraseCount; ++ii )
                {
                TPtrC ptrZi = phraseArray->MdcaPoint( ii );
                keyPtr.Append( ptrZi );
                }

            AddPhrasePinyinToPti();

            if ( 0 != isFound || ( 0 == isFound && index == keyStoke->Count() - 1 ) )
                {
                editpane->ResetAllArray();
                }
            fepMan->NewTextL( phraseCreated->Des());
            fepMan->CommitInlineEditL();
            fepMan->PtiEngine()->CommitCurrentWord();
            CleanupStack::PopAndDestroy();
            return EFalse;
            }
        }
    else
        {
        HBufC* phraseCreated = HBufC::NewLC( phraseCharacterCount );
        TPtr keyPtr = phraseCreated->Des();
        for ( TInt ii = 0; ii < phraseCount; ++ii )
            {
            TPtrC ptrZi = phraseArray->MdcaPoint( ii );
            keyPtr.Append( ptrZi );
            }
        editpane->ResetAllArray();
        editpane->SetPhraseCreationFlag( EFalse );
        fepMan->NewTextL( phraseCreated->Des());
        fepMan->CommitInlineEditL();
        iOwner->PtiEngine()->SetPredictiveChineseChar( phraseCreated->Des());
        CleanupStack::PopAndDestroy();
        return EFalse;
        }
    }

// ---------------------------------------------------------
// TAknFepInputMiniQwertyPinyinPhraseBase::AddPhrasePinyinToPti
// Add phrase to DB.
// ---------------------------------------------------------
//
void TAknFepInputMiniQwertySogouStrokePhrasePlugin::AddPhrasePinyinToPti()
    {
    MAknFepUICtrlEditPane* editpane = iUIContainer->EditPaneWindow();
    CDesCArrayFlat* phraseShowKeyStroke = editpane->PhraseShowKeyStrokeArray();
    CDesCArrayFlat* keyStoke = editpane->KeystrokeArray();
    TBuf<KMaxShowKeystrokeCount> phraseCreatedStroke;
    phraseCreatedStroke.FillZ();
    for ( TInt i = 0; i < phraseShowKeyStroke->Count(); i++ )
        {
        TPtrC ptrPhraseKeyStroke = phraseShowKeyStroke->MdcaPoint(i);
        phraseCreatedStroke.Append(ptrPhraseKeyStroke);
        }
    for ( TInt j = 0; j < keyStoke->Count(); j++ )
        {
        TPtrC ptrKeyStroke = keyStoke->MdcaPoint(j);
        phraseCreatedStroke.Append(ptrKeyStroke);
        }
    //Add the phrase pinyin to the DB by PTI Engine
    TRAP_IGNORE( iOwner->PtiEngine()->HandleCommandL( 
            EPtiCommandSetPinyinForLearnWord, (TAny*)( &phraseCreatedStroke )));
    }

// End of file
