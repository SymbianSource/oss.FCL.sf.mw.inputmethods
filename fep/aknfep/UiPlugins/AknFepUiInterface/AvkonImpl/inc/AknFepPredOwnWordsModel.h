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










#ifndef AKNFEPPREDOWNWORDSLISTBOX_H_
#define AKNFEPPREDOWNWORDSLISTBOX_H_

#include <e32base.h>
#include <eikclb.h>

//For accessing user dictionary through PtiEngine:
#include <PtiDefs.h>
#include <PtiEngine.h>
#include <PtiUserDicEntry.h>


class CAknFepPredOwnWordsModel : public CBase
    {
public:
    /**
    * Function:     NewL
    *
    * Discussion:   Factory function used to create a new instance of
    *               CAknFepPredictiveSettingDialog
    *
    * Params :      aTextArray - pointer to array of UDB entries  
    * Returns:      A newly created instance of CAknFepPredOwnWordsModel
    */
    static CAknFepPredOwnWordsModel* NewL( MDesCArray* aTextArray );

    /**
    * Function:     NewLC
    *
    * Discussion:   Factory function used to create a new instance of
    *               CAknFepPredictiveSettingDialog and leave a pointer to it on the
    *               cleanup stack.
    *
    * Params:       aTextArray - pointer to array of UDB entries
    * Returns:      A newly created instance of CAknFepUserDictionaryEditDialog
    */
    static CAknFepPredOwnWordsModel* NewLC( MDesCArray* aTextArray );
    
    //Destructor
    ~CAknFepPredOwnWordsModel();
    
    /**
    * Function:     RemoveSelectedWordL
    *
    * Discussion:   Removes existing word from UDB
    *
    * Params:       aWordIndex - index to the word to be removed 
    *                            in the list of UDB entries
    * Returns:      If successful, it returns the index of the word
    *               in the list of UDB entries which should be focussed. 
    *               Else, returns one of the system wide error codes.  
    */
    TInt RemoveSelectedWordL( TInt aWordIndex );
    
    /**
    * Function:     RemoveSelectedWordsL
    *
    * Discussion:   Removes existing word from UDB
    *
    * Params:       aSelectedItems - array of indices to the words to be 
    *                                removed from the list of UDB entries
    * Returns:      If successful, it returns KErrNone. 
    *               Else, returns one of the system wide error codes.  
    */
    TInt RemoveSelectedWordsL( const CArrayFix<TInt>* aSelectedItems );
    
    /**
    * Function:     AddNewWordL
    *
    * Discussion:   Adds the word to UDB
    *
    * Params:       aNewWord - new word to be added
    * Returns:      If successful, it returns the index of the newly added word
    *               in the list of UDB entries. 
    *               Else, returns one of the system wide error codes  
    */
    TInt AddNewWordL( TDesC& aNewWord );    
    
private:
    CAknFepPredOwnWordsModel();
    void ConstructL( MDesCArray* aTextArray );
    
    //Helper methods:
    /**
    * Function:     UpdateItemsL
    *
    * Discussion:   Refreshes the word list with the current UDB entries
    *
    * Params:       aTextArray
    * Returns:      no return value.  
    */
    void UpdateItemsL( MDesCArray* aTextArray );
    
    //to read user dictionary entries through PtiEngine
    void ReadUDBEntriesL();
    
    //to format the words for list box
    HBufC* FormListBoxEntryFromWordLC( const TDesC& aWord );
    
    //data:
    CPtiEngine* iPtiEngine;
    CDesCArrayFlat* iUDBWords;
    
    };
#endif /*AKNFEPPREDOWNWORDSLISTBOX_H_*/
