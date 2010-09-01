/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
*
*/










#include "AknFepPredOwnWordsModel.h"


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAknFepPredOwnWordsModel::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAknFepPredOwnWordsModel* CAknFepPredOwnWordsModel::NewL( MDesCArray* aTextArray )
    {
    CAknFepPredOwnWordsModel* self = CAknFepPredOwnWordsModel::NewLC( aTextArray );
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CAknFepPredOwnWordsModel::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAknFepPredOwnWordsModel* CAknFepPredOwnWordsModel::NewLC( MDesCArray* aTextArray )
    {
    CAknFepPredOwnWordsModel* self = new ( ELeave ) CAknFepPredOwnWordsModel();
    CleanupStack::PushL(self);
    self->ConstructL( aTextArray );
    return self;
    }
// -----------------------------------------------------------------------------
// CAknFepPredOwnWordsModel::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAknFepPredOwnWordsModel::ConstructL( MDesCArray* aTextArray )
    {
    iPtiEngine = CPtiEngine::NewL(ETrue); 
    //for our purposes(adding/removing UDB entries), 
    //we are not dependent on language. Hence using English here
    //as it should be available on all variants/devices.
    iPtiEngine->ActivateLanguageL( ELangEnglish, EPtiEnginePredictive );
    
    //Now read the UDB entires into the text array
    UpdateItemsL( aTextArray );
    }
// -----------------------------------------------------------------------------
// CAknFepPredOwnWordsModel::~CAknFepPredOwnWordsModel()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CAknFepPredOwnWordsModel::CAknFepPredOwnWordsModel()
    {
    // No implementation required
    }

// -----------------------------------------------------------------------------
// CAknFepPredOwnWordsModel::~CAknFepPredOwnWordsModel()
// Destructor.
// -----------------------------------------------------------------------------
//
CAknFepPredOwnWordsModel::~CAknFepPredOwnWordsModel()
    {
    delete iPtiEngine;
    }

// -----------------------------------------------------------------------------
// CAknFepPredOwnWordsModel::UpdateItemsL()
// Destructor.
// -----------------------------------------------------------------------------
//
void CAknFepPredOwnWordsModel::UpdateItemsL( MDesCArray* aTextArray )
    {
    iUDBWords = (CDesCArrayFlat*)aTextArray;
    ReadUDBEntriesL();
    }

// -----------------------------------------------------------------------------
//  Read all the user dictionary words through PtiEngine
// -----------------------------------------------------------------------------
//
void CAknFepPredOwnWordsModel::ReadUDBEntriesL()
    {
    if (NULL == iUDBWords)
        {
        //iUDBWords is not created here. We take referrence of item array
        //from listbox parameter passed to the MAknFepPredUserDictEditingModel
        //methods. So this is an error condition - list box is not 
        //completely/properly initialized
        User::Leave(KErrCorrupt);
        }
    
    TInt numOfUDBEntries = iPtiEngine->NumberOfEntriesInUserDictionary();
    for (TInt i(0) ; i < numOfUDBEntries ; ++i)
        {
        TPtiUserDictionaryEntry udbEntry;
        TInt getEntryResult = iPtiEngine->GetUserDictionaryEntry( i, udbEntry);
        if ( KErrNone == getEntryResult )
            {
            HBufC *listEntry = FormListBoxEntryFromWordLC( udbEntry.Word() ); 
            iUDBWords->AppendL( *listEntry );
            CleanupStack::PopAndDestroy( listEntry );
            }
        else if ( KErrNotFound != getEntryResult )
            {
            //As this is not an empty slot, this could be an error condition
            User::Leave(KErrCorrupt);
            }   
        }
    iUDBWords->Sort();
    }

// -----------------------------------------------------------------------------
//  Helper method for listbox string formatting
// -----------------------------------------------------------------------------
//
HBufC* CAknFepPredOwnWordsModel::FormListBoxEntryFromWordLC( const TDesC& aWord )
    {
    _LIT( KListItemPrefix, "\t" );
    TInt length = aWord.Length();
    //add 2 to length of word to store the tab seperator
    HBufC* listItem = HBufC::NewLC( length + 2 );
    listItem->Des().Copy( KListItemPrefix() ); 
    listItem->Des().Append( aWord );
    return ( listItem );
    }


//Adding new word
TInt CAknFepPredOwnWordsModel::AddNewWordL( TDesC& aNewWord )
    {
    TPtiUserDictionaryEntry udbEntry;
    udbEntry.SetWord( aNewWord );
    TInt status = iPtiEngine->AddUserDictionaryEntry( udbEntry );
    if (KErrNone == status || KErrAlreadyExists == status)
        {
        HBufC *listEntry = FormListBoxEntryFromWordLC( aNewWord ); 
        //If the word already exists, just move focus to it.
        //If it is a new word, add it to the list box items too.
        if ( KErrNone == status )
            {
            iUDBWords->AppendL( *listEntry );
            iUDBWords->Sort();
            }
        TInt selectedWordIndex(-1);
        iUDBWords->Find( *listEntry, selectedWordIndex, ECmpNormal );
        CleanupStack::PopAndDestroy( listEntry );
        return selectedWordIndex;
        }
    else
        {
        return KErrGeneral;
        }
    }

//Removing existing word
TInt CAknFepPredOwnWordsModel::RemoveSelectedWordL( TInt aWordIndex )
    {
    TPtiUserDictionaryEntry udbEntry;
    TPtrC word = ( (*iUDBWords)[aWordIndex] ).Mid(1);
    udbEntry.SetWord( word );
    if ( KErrNone == iPtiEngine->RemoveEntryFromUserDictionary( udbEntry ) )
        {
        iUDBWords->Delete( aWordIndex );
        return KErrNone;
        }
    else
        {
        return KErrGeneral;
        }
    }

//Only to be called if it is known for sure that at least one item is marked/selected
TInt CAknFepPredOwnWordsModel::RemoveSelectedWordsL( const CArrayFix<TInt>* aSelectedItems )
    {
    TPtiUserDictionaryEntry udbEntry;
    TInt numOfSelections = aSelectedItems->Count();
    /*TKeyArrayFix actNumKey(0,ECmpTInt);
    aSelectedItems->Sort( actNumKey );
    */
    //loop backwards so we don't disturb the indices after deletion
    for (TInt i(numOfSelections-1); i >= 0 ; --i )
        {
        TInt selectedItem = aSelectedItems->At(i);
        TPtrC word = ( (*iUDBWords)[selectedItem] ).Mid(1);
        udbEntry.SetWord( word );
        if ( KErrNone == iPtiEngine->RemoveEntryFromUserDictionary( udbEntry ) )
            {
            iUDBWords->Delete( selectedItem );
            }
        else
            {
            return KErrGeneral;
            }
        }
    
    return KErrNone;
    }

