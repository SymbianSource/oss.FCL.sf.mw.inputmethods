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










/*
 * ============================================================================
 *  Name     : CAknFepUserDictionaryEditDialog 
 * ============================================================================
 */

#ifndef AKNFEPUSERDICTIONARYEDITDIALOG_H_
#define AKNFEPUSERDICTIONARYEDITDIALOG_H_

#include <aknselectionlist.h>
#include <aknfep.rsg>
#include <aknsfld.h>
#include <AknQueryDialog.h>
#include <avkon.hrh>
#include <AknsUtils.h>
#include <AknsConstants.h>
#include <aknconsts.h>
#include <aknlists.h>
#include <avkon.mbg>

#include <StringLoader.h>
#include <fbs.h>

#include "AknFepPredOwnWordsModel.h"

const TInt KMaxUDBLength = 64;

class CAknFepUserDictionaryEditDialog : public CAknMarkableListDialog
{

    public:
        /**
        * Function:     NewL
        *
        * Discussion:   Factory function used to create a new instance of
        *               CAknFepPredictiveSettingDialog
        *
        * Returns:      A newly created instance of CAknFepUserDictionaryEditDialog
        */
        static CAknFepUserDictionaryEditDialog* NewL(
                TInt aOpenedItem,
                CArrayFixFlat<TInt> *aSelectedItems,
                MDesCArray *aArray);

        /**
        * Function:     NewLC
        *
        * Discussion:   Factory function used to create a new instance of
        *               CAknFepPredictiveSettingDialog and leave a pointer to it on the
        *               cleanup stack.
        *
        * Params:       aMenuResId - menu resource id.
        * Params:       aConfirmationQueryResId - Confirmation Query resource id.
        * Returns:      A newly created instance of CAknFepUserDictionaryEditDialog
        */
        static CAknFepUserDictionaryEditDialog* NewLC(
                TInt aOpenedItem,
                CArrayFixFlat<TInt> *aSelectedItems,
                MDesCArray *aArray);
        
        // the static RunDlgLD launch function
        static void RunDlgLD();
        
        /**
        * Function:     ~CAknFepPredictiveSettingDialog
        *
        * Discussion:   Virtual destructor for CAknFepUserDictionaryEditDialog. Ensures that all
        *               allocated memory is returned to the heap before deletion
        *               of object.
        */
        virtual ~CAknFepUserDictionaryEditDialog();
        
    private:
        //virtual methods from CAknMarkableListDialog:
        void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane *aMenuPane);
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );
        TBool OkToExitL(TInt aButtonId);
        // the control initializer function
        void PreLayoutDynInitL();
                
        void ProcessCommandL( TInt aCommandId );
        
        //Constructor
        CAknFepUserDictionaryEditDialog(
                TInt aOpenedItem,
                CArrayFixFlat<TInt> *aSelectedItems,
                MDesCArray *aArray );
                
                
        
        //Own methods - 
        void UpdateMSKLabelL();
        
        //Query for new word from user. Also used to edit existing word
        TBool QueryWordL( const TDesC& aTitle, TDes& aNewWord );
        
        //Confirmation query for deletion
        TBool ConfirmDeleteL( const TDesC& aPrompt );
        
        //To handle different commands from the options menu or soft keys:
        void HandleEditCommandL();
        void HandleAddNewCommandL();
        void HandleDeleteCommandL();      
        
        //To get the correct index if a filtered list is being shown
        TInt CurrentItemIndex();
        const CArrayFix<TInt>* SelectedItemsL();     
        void HandleAddNewIntoFilteredListL( TInt &aOriginalItemIndex );
        
    private: //data
         
        CEikColumnListBox*  iOwnWordsListBox;
        TInt iOpenedItem;
        CArrayFixFlat<TInt>* iSelectedItems;
        CAknFepPredOwnWordsModel* iOwnWordsModel;
        TInt iMSKPosition;
                
};

#endif /*AKNFEPUSERDICTIONARYEDITDIALOG_H_*/
