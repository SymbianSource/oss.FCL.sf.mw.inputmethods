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
* Description:            DB Engine
*
*/












//system include files
#include <aknfep.rsg>
#include <PtiUserDicEntry.h>
#include <PtiEngine.h>
#include <StringLoader.h> 

//user inlucde files
#include "dbmanagement.h"
#include "AknFepGlobalEnums.h"
#include "AknFepManager.h"
#include "AknFepSharedDataInterface.h"

// CONSTANTS
const TInt KInputPhraseMaxLength = 7;
const TInt KInvalidPhrase        = -103;
const TInt KPhraseExisted        = -104;
const TInt KPhraseNotExisted     = -105;
const TInt KInputPhraseOutRange  = -106;
const TInt KDBHasNoSpace         = -150;

// ---------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------
//
CDBManagement* CDBManagement::NewL( CAknFepManager* aFepManager )
    {
    CDBManagement* self = new( ELeave ) CDBManagement();
    CleanupStack::PushL( self );
    self->ConstructL(aFepManager);
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------
//
CDBManagement::CDBManagement()
    {
    }

// ---------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------
//
void CDBManagement::ConstructL( CAknFepManager* aFepManager )
    {
    iAknFepSharedDataInterface = CAknFepSharedDataInterface::NewL( aFepManager );
    iFepManager = aFepManager;
    iPtiEngineOfFep = iFepManager->PtiEngine();
    iPtiEngine = CPtiEngine::NewL(ETrue); 
    }

// ---------------------------------------------------------
// destructor
// ---------------------------------------------------------
//
CDBManagement::~CDBManagement()
    {
    delete iAknFepSharedDataInterface;
    delete iPtiEngine;
    }

// ---------------------------------------------------------
// Add phrase to DB.
// ---------------------------------------------------------
//
TBool CDBManagement::AddPhraseToDB( const TDesC& aPhraseAdd )
    {    
    TInt retCode;
    TPtiUserDictionaryEntry addUdbEntry(aPhraseAdd);
    TRAP_IGNORE( ActivateLanguageOfFepL() );
    //Add the phrase to the DB by PTI Engine
    iPtiEngineOfFep->AddUserDictionaryEntry( addUdbEntry );
    retCode = iPtiEngine->AddUserDictionaryEntry( addUdbEntry );
    if ( KErrNone != retCode )
        {
        GetDBErrMsg( retCode );
        return EFalse;
        }
    return ETrue;
    }

// ---------------------------------------------------------
// Edit phrase from DB
// ---------------------------------------------------------
//
TBool CDBManagement::EditPhraseFromDB(  const TDesC& aPhraseOld,
                                        const TDesC& aPhraseNew )
    {
    TBool retCode;
    //Add the new phrase to the DB by PTI Engine
    retCode = AddPhraseToDB( aPhraseNew );
    if ( !retCode )
        {
        GetDBErrMsg( retCode );
        return EFalse;
        }    
    retCode = RemovePhraseFromDB( aPhraseOld );
    if ( !retCode )
        {
        GetDBErrMsg( retCode );
        return EFalse;
        }
    return ETrue;
    }

// ---------------------------------------------------------
// Remove phrase from DB
// ---------------------------------------------------------
//
TBool CDBManagement::RemovePhraseFromDB(const TDesC& aPhraseRemove )
    {
    TInt retCode;
    TPtiUserDictionaryEntry removeUdbEntry( aPhraseRemove );
    //Remove the phrase from the DB by PTI Engine
    TRAP_IGNORE( ActivateLanguageOfFepL() );
    iPtiEngineOfFep->RemoveEntryFromUserDictionary( removeUdbEntry );
    retCode = iPtiEngine->RemoveEntryFromUserDictionary( removeUdbEntry );
    if( KErrNone != retCode )
        {
        GetDBErrMsg( retCode );
        return EFalse;
        }
    return ETrue;
    }

// ---------------------------------------------------------
// Get phrases from DB
// ---------------------------------------------------------
//
TBool CDBManagement::GetPhraseFromDBL( CDesCArray& aPhraseGet )
    {
    TPtiUserDictionaryEntry getUdbEntry;
    TInt retCode;
    TInt i = 0; 
    ActivateLanguage();

    retCode = iPtiEngine->GetUserDictionaryEntry(i, getUdbEntry);

    while (retCode == KErrNone)
        {
        aPhraseGet.AppendL(getUdbEntry.Word());
        i++;
        retCode = iPtiEngine->GetUserDictionaryEntry(i, getUdbEntry);
        }

    return ETrue;
    }

// ---------------------------------------------------------
// Get Error Message
// ---------------------------------------------------------
//
void CDBManagement::GetErrMsg( TDes& aErrMsg )
    {
    aErrMsg.Copy(iErrMsg);
    }

// ---------------------------------------------------------
// Check the input phrase
// ---------------------------------------------------------
//
TBool CDBManagement::InputPhraseCheck( TDes& aPhraseInput )
    {
    const TUint16 KChineseUnicodeMin = 0x4E00;
    const TUint16 KChineseUnicodeMax = 0x9FA5; 
    const TUint16 KSpaceUnicode      = 0x0020;
    const TUint16 KKeytroke2Unicode  = 0x4E28;
    const TUint16 KKeytroke3Unicode  = 0x4E3F;
    const TUint16 KKeytroke4Unicode  = 0x4E36;
    const TUint16 KKeytroke5Unicode  = 0x4E5B;
    TBuf<KInputPhraseMaxLength> inputText;
    aPhraseInput.TrimAll();
    TBool inValidFlg = EFalse;
    //the input phrase length more than the max length
    if ( aPhraseInput.Length() > KInputPhraseMaxLength )
        {
        return EFalse;
        }
    //check every character of input phrase
    for ( TInt i = 0; i < aPhraseInput.Length(); ++i )
        {
        if ( ( (TInt)aPhraseInput[i] >= KChineseUnicodeMin ) &&
            ( (TInt)aPhraseInput[i] <= KChineseUnicodeMax ) )
            {
            if ( ( KKeytroke2Unicode == (TInt)aPhraseInput[i] ) ||
                ( KKeytroke3Unicode == (TInt)aPhraseInput[i] ) ||
                ( KKeytroke4Unicode == (TInt)aPhraseInput[i] ) ||
                ( KKeytroke5Unicode == (TInt)aPhraseInput[i] ) )
                {
                inValidFlg = ETrue;
                }
            else
                {
                inputText.Append( aPhraseInput[i] );
                }
            }
        else
            {
            if ( KSpaceUnicode != (TInt)aPhraseInput[i] )
                {
                inValidFlg = ETrue;
                }            
            }
        }
    
    //out the chinese phrase.
    aPhraseInput = inputText;
    
    if ( inValidFlg )
        {
        StringLoader::Load( iErrMsg, R_AKNFEP_CHINESE_USER_DB_NOTE_ONLY_CHINESE_TEXT );
        return EFalse;
        }
    
    if ( aPhraseInput.Length() < 2 )
        {
        StringLoader::Load( iErrMsg, R_AKNFEP_CHINESE_USER_DB_NOTE_TOO_SHORT_TEXT );
        return EFalse;
        }
    
    return ETrue;
    }

// ---------------------------------------------------------
// Set language
// ---------------------------------------------------------
//
void CDBManagement::ActivateLanguageL()
    {
    TInt epocLanguageID = iAknFepSharedDataInterface->InputTextLanguage();
    iPtiEngine->ActivateLanguageL((TLanguage)epocLanguageID);
    switch(epocLanguageID)
        {
        case ELangHongKongChinese:
            iPtiEngine->SetInputMode( EPtiEngineStrokeByPhrase );
            break;
        case ELangTaiwanChinese:
            iPtiEngine->SetInputMode( EPtiEngineZhuyinByPhrase );
            break;
        case ELangPrcChinese:
            iPtiEngine->SetInputMode( EPtiEnginePinyinByPhrase );
            break;
        }   
    }

// ---------------------------------------------------------
// Set language
// ---------------------------------------------------------
//
void CDBManagement::ActivateLanguageOfFepL()
    {
    TInt epocLanguageID = iAknFepSharedDataInterface->InputTextLanguage();
    iPtiEngineOfFep->ActivateLanguageL((TLanguage)epocLanguageID);
    switch(epocLanguageID)
        {
        case ELangHongKongChinese:
            iPtiEngineOfFep->SetInputMode( EPtiEngineStroke );
            break;
        case ELangTaiwanChinese:
            iPtiEngineOfFep->SetInputMode( EPtiEngineZhuyinByPhrase );
            break;
        case ELangPrcChinese:
            iPtiEngineOfFep->SetInputMode( EPtiEnginePinyin );
            break;
        }   
    }

// ---------------------------------------------------------
// Set language
// ---------------------------------------------------------
//
void CDBManagement::ActivateLanguage()
    {
    TRAP_IGNORE( ActivateLanguageL() );  
    }

// ---------------------------------------------------------
// Get the ptiengine's error
// ---------------------------------------------------------
//
void CDBManagement::GetDBErrMsg( TInt aErrCode )
    {
    switch( aErrCode )
        {
        case KInvalidPhrase:
            StringLoader::Load( iErrMsg, R_AKNFEP_CHINESE_USER_DB_NOTE_INVALID_PHRASE_TEXT );
            break;
        case KPhraseExisted:
            StringLoader::Load( iErrMsg, R_AKNFEP_CHINESE_USER_DB_NOTE_PHRASE_EXISTED_TEXT );
            break;
        case KPhraseNotExisted:
            StringLoader::Load( iErrMsg, R_AKNFEP_CHINESE_USER_DB_NOTE_NOT_EXISTED_TEXT );
            break;
        case KInputPhraseOutRange:
            StringLoader::Load( iErrMsg, R_AKNFEP_CHINESE_USER_DB_PHRASE_TOO_LONG_TEXT );
            break;
        case KDBHasNoSpace:
            StringLoader::Load( iErrMsg, R_AKNFEP_CHINESE_USER_DB_NOTE_DB_HAVE_NO_SPACE_TEXT );
            break; 
        default:
            break;
        }
    }

//End of File
