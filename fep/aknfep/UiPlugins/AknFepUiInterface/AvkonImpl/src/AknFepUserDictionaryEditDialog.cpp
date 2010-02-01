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

#include <COEMAIN.H>
#include <EIKENV.H>
#include <eikclbd.h>
#include <akntitle.h>
#include "AknFepUserDictionaryEditDialog.h"

//For help:
#include <hlplch.h>
//#include <csxhelp\predinput.hlp.hrh>

//Uid of AknFep
const TUid KUidAknFep = {0x101FD65A};

CAknFepUserDictionaryEditDialog *CAknFepUserDictionaryEditDialog::NewL(
        TInt aOpenedItem,
        CArrayFixFlat<TInt> *aSelectedItems,
        MDesCArray *aArray)
    {
    CAknFepUserDictionaryEditDialog* self = CAknFepUserDictionaryEditDialog::NewLC(
            aOpenedItem, aSelectedItems, aArray );
    CleanupStack::Pop(self);
    return self;
    }

CAknFepUserDictionaryEditDialog *CAknFepUserDictionaryEditDialog::NewLC(
        TInt aOpenedItem,
        CArrayFixFlat<TInt> *aSelectedItems,
        MDesCArray *aArray)
    {
    CAknFepUserDictionaryEditDialog* self = new (ELeave) CAknFepUserDictionaryEditDialog(
            aOpenedItem, aSelectedItems, aArray );
    CleanupStack::PushL(self);
    self->ConstructL(R_UDB_EDIT_OPTIONS_MENUBAR);
    return self;
    }

CAknFepUserDictionaryEditDialog::CAknFepUserDictionaryEditDialog(
        TInt aOpenedItem,
        CArrayFixFlat<TInt> *aSelectedItems,
        MDesCArray *aArray )
    :CAknMarkableListDialog(aOpenedItem, aSelectedItems, aArray, R_UDB_EDIT_OPTIONS_MENUBAR, NULL, NULL), 
     iOpenedItem(aOpenedItem), iSelectedItems(aSelectedItems)
    {
    };

CAknFepUserDictionaryEditDialog::~CAknFepUserDictionaryEditDialog()
    {
    delete iOwnWordsModel;
    iOwnWordsModel = NULL;
    iOwnWordsListBox = NULL;
    };

void CAknFepUserDictionaryEditDialog::RunDlgLD()
    {
    CDesCArrayFlat* arrWords = new (ELeave) CDesCArrayFlat(1);
    CleanupStack::PushL( arrWords );
    //ownership of selectedItems is given to CAknFepUserDictionaryEditDialog
    CArrayFixFlat<TInt>* selectedItems = new (ELeave) CArrayFixFlat<TInt>(1);
    CleanupStack::PushL( selectedItems );
    
    TInt openedItem(0);
    CAknFepUserDictionaryEditDialog* dlg = CAknFepUserDictionaryEditDialog::NewL(
                        openedItem, selectedItems, arrWords);
    
    //Save old title pane and display title for user dictionary editing
    CCoeEnv* coeEnv = CCoeEnv::Static();
    CAknAppUi* appUi = static_cast<CAknAppUi*>(coeEnv->AppUi());

    CAknTitlePane* titlePane = 
        static_cast<CAknTitlePane*>( appUi->StatusPane()->ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );

    const TDesC* appTitle = titlePane->Text();
    HBufC* oldTitle = appTitle->AllocL();
    CleanupStack::PushL(oldTitle);
    HBufC* title = StringLoader::LoadLC( R_AKNFEP_PRED_USERDICT_EDITING_TITLE );
    titlePane->SetTextL( *title );
    CleanupStack::PopAndDestroy(title);

    dlg->ExecuteLD(R_USER_DICTIONARY_EDITING_DIALOG);
    
    //workaround to prevent application from crashing in case a 3rd party fep 
    //was installed while the dialog was active. 
    if( KUidAknFep != CCoeEnv::Static()->FepUid() )
        {
        User::Leave(KErrNone);
        }
    titlePane->SetTextL( *oldTitle );
    CleanupStack::PopAndDestroy( 3, arrWords );
    }


void CAknFepUserDictionaryEditDialog::PreLayoutDynInitL()
    {
    CAknMarkableListDialog::PreLayoutDynInitL();
    iOwnWordsListBox = (CEikColumnListBox*) ControlOrNull( ESelectionListControl );
    //Initialize the model for retrieving UDB entries
    iOwnWordsModel = CAknFepPredOwnWordsModel::NewL( iOwnWordsListBox->Model()->ItemTextArray() );
    iOwnWordsListBox->HandleItemAdditionL();
    
    //Set the empty listbox string
    HBufC* emptyListString_secondary = StringLoader::LoadLC( R_AKNFEP_PRED_USER_DICTIONARY_EMPTY );
    HBufC* emptyListString = StringLoader::LoadLC( R_AKNFEP_PRED_USER_DICTIONARY_HEADING_EMPTY );
    _LIT( KNewLine, "\n" );
    TInt newLength = emptyListString->Length() + KNewLine().Length() + emptyListString_secondary->Length();
    emptyListString = emptyListString->ReAllocL( newLength );
    CleanupStack::Pop(); //emptyListString could now be at a different location
    CleanupStack::PushL( emptyListString );
    TPtr ptr = emptyListString->Des();
    ptr.Append( KNewLine );
    ptr.Append( *emptyListString_secondary );

    iOwnWordsListBox->View()->SetListEmptyTextL( *emptyListString );
    CleanupStack::PopAndDestroy( 2, emptyListString_secondary );
    
    //Set the middle soft key:
    CEikButtonGroupContainer& cba = ButtonGroupContainer();
    iMSKPosition = cba.PositionById( EAknSoftkeyContextOptions );
    UpdateMSKLabelL();
    
    //For viewing extra long words:
    iOwnWordsListBox->ItemDrawer()->ColumnData()->EnableMarqueeL(ETrue);
    }

void CAknFepUserDictionaryEditDialog::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane *aMenuPane)
    {
    CAknMarkableListDialog::DynInitMenuPaneL( aResourceId, aMenuPane );
    
    if ( iOwnWordsListBox->SelectionIndexes()->Count() >= 1
            && R_UDB_EDIT_OPTIONS_MENUPANE == aResourceId )
        {
        //At least one item in the list is marked. 
        //Hide the "Edit" option.
        aMenuPane->SetItemDimmed( EAknFepPredDictionaryCmdEdit, ETrue);
        }
    
    if ( iOwnWordsListBox->Model()->NumberOfItems() <= 0 
            && R_UDB_EDIT_OPTIONS_MENUPANE == aResourceId )
        {
        //No items available for editing/deletion:
        aMenuPane->SetItemDimmed( EAknFepPredDictionaryCmdEdit, ETrue);
        aMenuPane->SetItemDimmed( EAknFepPredDictionaryCmdDelete, ETrue);
        }
    
    }

void CAknFepUserDictionaryEditDialog::ProcessCommandL( TInt aCommandId )
    {
    
    if ( MenuShowing() )
        {
        HideMenu();
        }
                
    switch(aCommandId)
        {
        case EAknFepPredDictionaryCmdEdit :
            {
            HandleEditCommandL();
            break; 
            }              
            
        case EAknFepPredDictionaryCmdAdd :
            {
            HandleAddNewCommandL();
            break; 
            }
            
        case EAknFepPredDictionaryCmdDelete : 
            {
            HandleDeleteCommandL();
            break; 
            }
                        
        case EAknCmdMark :
        case EAknCmdUnmark :
        case EAknMarkAll :
        case EAknUnmarkAll :
            {
            SelectionListProcessCommandL(aCommandId);
            break;
            }
        
        case EAknSoftkeyEdit :
            {
            if ( iOwnWordsListBox->Model()->NumberOfItems() <= 0 )
                {
                //No entries in the list. So only option available is "Add new"
                HandleAddNewCommandL();
                }
            else if ( iOwnWordsListBox->SelectionIndexes()->Count() <= 0 )
                {
                //Words available and none are marked. So selection => "Edit"
                HandleEditCommandL();
                }
            else
                {
                //Words are available and at least one is marked. 
                //So selection => "Options" menu is launched
                DisplayMenuL();
                }
            break;
            }
            
        case EAknCmdHelp:
            {
            CArrayFix<TCoeHelpContext>* helpContext = new (ELeave) CArrayFixFlat<TCoeHelpContext>( 1 );
            CleanupStack::PushL( helpContext );
            //THIS IS TEMPORARY AND NEEDS TO BE UPDATED WHEN HELP CONTENT IS AVAILABLE!!!
            TUid appuid = { 0x100058EC };
            _LIT( KAknFepPredUserDictHelp, "PREDINPUT_HLP_USERDICT" );
            helpContext->AppendL( TCoeHelpContext( appuid, KAknFepPredUserDictHelp() ) );
            HlpLauncher::LaunchHelpApplicationL( CEikonEnv::Static()->WsSession(), helpContext );
            CleanupStack::Pop( helpContext );  
            break;
            }
        
        case EAknCmdExit:
            {
            iAvkonAppUi->ProcessCommandL( aCommandId );
            break;
            }
        default: CAknMarkableListDialog::ProcessCommandL( aCommandId );
        }
    
    UpdateMSKLabelL();
    
    }

TKeyResponse CAknFepUserDictionaryEditDialog::OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType )
    {
    TKeyResponse response( EKeyWasNotConsumed );
    //Special handling required only for the clear/backspace/delete keys
    //AND only when find pane is empty AND there is at least one item in focus/selected
    if ( 
            ( aKeyEvent.iCode == EKeyBackspace 
                ||  aKeyEvent.iCode == EKeyDelete )
        &&  ( 
                FindBox()->TextLength() == 0 
                && iOwnWordsListBox->Model()->NumberOfItems() > 0 
                && (iOwnWordsListBox->SelectionIndexes()->Count() > 0 
                        || iOwnWordsListBox->CurrentItemIndex() >= 0 ) 
            )
        )
        {
        //Delete the item(s)
        HandleDeleteCommandL();
        response = EKeyWasConsumed;
        }
    else
        {
        response = CAknMarkableListDialog::OfferKeyEventL( aKeyEvent, aType );
        }
    
    //Update MSK only if key is not escape key (for exiting)
    if ( EKeyEscape != aKeyEvent.iCode )
        {
        UpdateMSKLabelL();
        }
    return response;
    }

TBool CAknFepUserDictionaryEditDialog::OkToExitL(TInt aButtonId)
    {
    if ( EAknSoftkeyEdit == aButtonId || EAknSoftkeyContextOptions == aButtonId )
        {
        CAknFepUserDictionaryEditDialog::ProcessCommandL( EAknSoftkeyEdit );
        return EFalse;
        }
    else
        {
        return CAknMarkableListDialog::OkToExitL( aButtonId );
        }
    }

void CAknFepUserDictionaryEditDialog::UpdateMSKLabelL()
    {
    if ( iOwnWordsListBox && iOwnWordsListBox->Model() )
        {
        CEikButtonGroupContainer& cba = ButtonGroupContainer();
        if ( iOwnWordsListBox->Model()->NumberOfItems() <= 0 )
            {
            //No entries in the list. So MSK is "Add new"
            HBufC* mskLabel = StringLoader::LoadLC( R_AKNFEP_PRED_MSK_ADD_NEW );
            cba.SetCommandL(iMSKPosition, EAknSoftkeyEdit, *mskLabel);    
            CleanupStack::PopAndDestroy( mskLabel );
            }
        else if ( iOwnWordsListBox->SelectionIndexes()->Count() > 0 )
            {
            //Words are available and at least one is marked. 
            //So on pressing MSK, "Options" menu is launched
            HBufC* mskLabel = StringLoader::LoadLC( R_AKNFEP_PRED_MSK_CONTEXT_OPTIONS );
            cba.SetCommandL(iMSKPosition, EAknSoftkeyContextOptions, *mskLabel);    
            CleanupStack::PopAndDestroy( mskLabel );
            }
        else
            {
            //Words available and none are marked. So MSK should be "Edit"
            HBufC* mskLabel = StringLoader::LoadLC( R_AKNFEP_PRED_MSK_EDIT );
            cba.SetCommandL(iMSKPosition, EAknSoftkeyEdit, *mskLabel);    
            CleanupStack::PopAndDestroy( mskLabel );
            }
        cba.DrawDeferred();
        }
    }

void CAknFepUserDictionaryEditDialog::HandleEditCommandL()
    {
    CDesCArrayFlat* wordArray = (CDesCArrayFlat*)iOwnWordsListBox->Model()->ItemTextArray();
    //First Get the current focussed word's index
    TInt selectedWordIndex = iOwnWordsListBox->CurrentItemIndex();
    //Edit option will be available only for the currently 
    //focussed item. 
    
    //Get the indexed word and remove seperator ("\t")
    TPtrC oldWord = ( (*wordArray)[selectedWordIndex] ).Mid(1);
    HBufC* newWord = HBufC::NewLC( KMaxUDBLength );
    //Copy the old word to newWord for the "Edit word" prompt
    newWord->Des().Copy( oldWord );
    TPtr newWordPtr(newWord->Des());
    HBufC* queryTitle = StringLoader::LoadLC( R_AKNFEP_PRED_EDIT_QUERY );
    if ( QueryWordL( *queryTitle, newWordPtr ) )
        {
        //Remove old word
        if ( KErrNone == iOwnWordsModel->RemoveSelectedWordL( selectedWordIndex ) )
            {
            iOwnWordsListBox->HandleItemRemovalL();
            //Now add the new word:
            selectedWordIndex = iOwnWordsModel->AddNewWordL( newWordPtr );
            if ( selectedWordIndex >= 0 )
                {
                iOwnWordsListBox->HandleItemAdditionL();
                if ( (iOwnWordsListBox->CurrentItemIndex() >= 0) && FindBox() && (FindBox()->TextLength() > 0) )
                    {
                    HandleAddNewIntoFilteredListL( selectedWordIndex );
                    }
                if ( selectedWordIndex >=0 )
                    {
                    iOwnWordsListBox->SetCurrentItemIndex( selectedWordIndex );
                    }
                iOwnWordsListBox->DrawNow();
                }
            }
        }
    
    CleanupStack::PopAndDestroy( 2, newWord );
    }

void CAknFepUserDictionaryEditDialog::HandleAddNewCommandL()
    {
    HBufC* newWord = HBufC::NewLC( KMaxUDBLength );
    TPtr newWordPtr(newWord->Des());
    //if there is some text in the find pane, include it in the query
    if ( FindBox()->TextLength() > 0 )
        {
        FindBox()->GetSearchText( newWordPtr );
        }
    HBufC* queryTitle = StringLoader::LoadLC( R_AKNFEP_PRED_ADD_QUERY );
    if ( QueryWordL( *queryTitle, newWordPtr ) )
        {
        TInt selectedWordIndex = iOwnWordsModel->AddNewWordL( newWordPtr );
        if ( selectedWordIndex >= 0 )
            {
            iOwnWordsListBox->HandleItemAdditionL();
            if ( (iOwnWordsListBox->CurrentItemIndex() >= 0) && FindBox() && (FindBox()->TextLength() > 0) )
                {
                HandleAddNewIntoFilteredListL( selectedWordIndex );
                }
            if ( selectedWordIndex >=0 )
                {
                iOwnWordsListBox->SetCurrentItemIndex( selectedWordIndex );
                }
            iOwnWordsListBox->DrawNow();
            }
        }
    CleanupStack::PopAndDestroy( 2, newWord );
    }
void CAknFepUserDictionaryEditDialog::HandleDeleteCommandL()
    {
    if ( iOwnWordsListBox->Model()->NumberOfItems() <= 0 )
        {
        //safety check - no words to delete so return from here
        return;
        }
    
    TInt updatedIndex(-1);
    TInt numOfMarkedItems = iOwnWordsListBox->SelectionIndexes()->Count();
    TInt currentIndex = CurrentItemIndex();
    //If multiple items are marked:
    if ( numOfMarkedItems > 1 )
        {
        //Load the confirmation string for multiple item deletion
        HBufC* prompt = StringLoader::LoadLC( 
                            R_AKNFEP_PRED_DELETE_MANY_QUERY, numOfMarkedItems );
        if ( ConfirmDeleteL( *prompt ) )
            {
            const CArrayFix<TInt>* selectedItems = SelectedItemsL();/*(CArrayFix<TInt>*)filteredModel->Filter()->SelectionIndexes();*/
            updatedIndex = iOwnWordsModel->RemoveSelectedWordsL( selectedItems );
            }
        CleanupStack::PopAndDestroy( prompt );
        }
    else
        {
        //Load the confirmation string for single item deletion
        HBufC* prompt = StringLoader::LoadLC( R_AKNFEP_PRED_DELETE_1_QUERY );
        if ( ConfirmDeleteL( *prompt ) )
            {
            //Deleting marked item?
            if ( numOfMarkedItems == 1 )
                {
                updatedIndex = iOwnWordsModel->RemoveSelectedWordL( SelectedItemsL()->At(0) );
                }
            //or deleting the currently focussed item
            else //just one entry to delete:
                {
                updatedIndex = iOwnWordsModel->RemoveSelectedWordL( currentIndex );
                }
            }
        CleanupStack::PopAndDestroy( prompt );
        }
    
    if ( updatedIndex == KErrNone )
        {
        iOwnWordsListBox->HandleItemRemovalL();
                
        CAknListBoxFilterItems* filter;
        TInt filteredNumOfItems(-1);
        if ( (iOwnWordsListBox->CurrentItemIndex() >= 0) && FindBox() && (FindBox()->TextLength() > 0) )
            {
            CAknFilteredTextListBoxModel* model = static_cast<CAknFilteredTextListBoxModel*>(iOwnWordsListBox->Model());
            filter = model->Filter();
            filter->HandleItemArrayChangeL();
            filter->UpdateCachedDataL();
            filteredNumOfItems = filter->FilteredNumberOfItems();
            }
        
        //retain the current selection position, unless
        //it refers to non-existant word. In this case, 
        //keep selection at the end of the list.
        TInt numOfItems = filteredNumOfItems>=0? filteredNumOfItems : iOwnWordsListBox->Model()->NumberOfItems();
                
        if ( currentIndex >= numOfItems && numOfItems > 0 )
            {       
            iOwnWordsListBox->SetCurrentItemIndex( numOfItems - 1 );
            }
        
        
        UpdateMSKLabelL();
        iOwnWordsListBox->DrawNow();
        }
    
    }

//Query for new word from user. Also used to edit existing word
TBool CAknFepUserDictionaryEditDialog::QueryWordL( const TDesC& aTitle, TDes& aNewWord )
    {
    CAknTextQueryDialog* dialog = CAknTextQueryDialog::NewL( aNewWord  );
    dialog->PrepareLC( R_AKNFEP_PRED_USER_DICTIONARY_WORD_QUERY );
    dialog->SetPromptL(aTitle);
    //last char space has to be reserved for null terminator
    dialog->SetMaxLength( KMaxUDBLength - 1 );
    if ( EAknSoftkeyOk ==  dialog->RunLD() )
        {
        return ETrue;
        }
    else
        {
        return EFalse;
        }
    }

TBool CAknFepUserDictionaryEditDialog::ConfirmDeleteL( const TDesC& aPrompt )
    {
    CAknQueryDialog* dialog = CAknQueryDialog::NewL();
    dialog->SetPromptL( aPrompt );
    if ( EAknSoftkeyYes ==  dialog->ExecuteLD( R_AKNFEP_PRED_DELETE_CONFIRMATION_QUERY ) )
        {
        return ETrue;
        }
    else
        {
        return EFalse;
        }
    }

TInt CAknFepUserDictionaryEditDialog::CurrentItemIndex()
    {
    if ( (iOwnWordsListBox->CurrentItemIndex() >= 0) && FindBox() && (FindBox()->TextLength() > 0) )
        {
        CAknFilteredTextListBoxModel* model = static_cast<CAknFilteredTextListBoxModel*>(iOwnWordsListBox->Model());
        CAknListBoxFilterItems* filter = model->Filter(); 
        return filter->FilteredItemIndex(iOwnWordsListBox->CurrentItemIndex());
        }           
    else
        {
        return iOwnWordsListBox->CurrentItemIndex();
        }
    }

const CArrayFix<TInt>* CAknFepUserDictionaryEditDialog::SelectedItemsL()
    {
    const CArrayFix<TInt>* selectedItems;
    if ( (iOwnWordsListBox->CurrentItemIndex() >= 0) && FindBox() && (FindBox()->TextLength() > 0) )
        {
        CAknFilteredTextListBoxModel* model = static_cast<CAknFilteredTextListBoxModel*>(iOwnWordsListBox->Model());
        CAknListBoxFilterItems* filter = model->Filter();
        filter->ResetFilteringL();
        selectedItems = filter->SelectionIndexes();
        }
    else
        {
        selectedItems = iOwnWordsListBox->SelectionIndexes();
        }
    TKeyArrayFix actNumKey(0,ECmpTInt);
    const_cast<CArrayFix<TInt>*>(selectedItems)->Sort(actNumKey);
    return selectedItems;
    }

void CAknFepUserDictionaryEditDialog::HandleAddNewIntoFilteredListL( TInt &aOriginalItemIndex )
    {
    CAknFilteredTextListBoxModel* model = static_cast<CAknFilteredTextListBoxModel*>(iOwnWordsListBox->Model());
    CAknListBoxFilterItems* filter = model->Filter();
    filter->HandleItemArrayChangeL();
    filter->UpdateCachedDataL();
    TInt filteredNumOfItems = filter->FilteredNumberOfItems();
    aOriginalItemIndex = filter->VisibleItemIndex( aOriginalItemIndex );
    }
