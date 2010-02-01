/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            Implement method in dialog for user DB management
*
*/












#include <aknlists.h> // CAknSingleStyleListBox
#include <barsread.h> // TResource Reader
#include <eikclbd.h> // CColumnListBoxData
#include <aknsfld.h> //CAknSearchField
#include <AknQueryDialog.h> //CAknTextQueryDialog
#include <AknUtils.h> //AknSelectionService , CAknFilteredTextListBoxModel
#include <StringLoader.h> //StringLoader 
#include <aknnotewrappers.h> //CAknWarningNote 
#include <AknProgressDialog.h> //CAknProgressDialog
#include <AknDialog.h> //CAknDialog
#include <akncontext.h> //CAknContextPane 
#include <AknWaitDialog.h> //CAknWaitDialog 
#include <AknProgressDialog.h> //CAknProgressDialog
#include <eikprogi.h> //CEikProgressInfo 
#include <eikbtgpc.h> //CEikButtonGroupContainer 
#include <eikmenub.h> 
#include <charconv.h> //CCnvCharacterSetConverter
#include <f32file.h> //RFs
#include <eikedwin.h> //CEikEdwin 
#include <akntitle.h> //CAknTitlePane 
#include <AknFep.rsg>
#include "AknFep.hrh"
#include "aknfepuserdbdlg.h"
#include "dbmanagement.h"
#include "AknFepManagerUIInterface.h"
#include "AknFepManager.h"  
#include "aknfepuserdbquerydlg.h"

const TInt KListBoxItemMaxLength = 8;
const TInt KErrMsgMaxLength = 60;
const TInt KFindBoxInputMaxLength = 7;
const TInt KCDesCArraySize = 1;
_LIT( KItemStart, "\t" );
_LIT( KSearchTextEmpty, "" );

CAknFepUserdbDlg::CAknFepUserdbDlg( 
    TInt &aValue, CArrayFix< TInt> *aSelectedItems, MDesCArray *aArray,
    CPtiEngine* aPtiEngine, CAknFepManager* aOwner, MEikCommandObserver *aObserver) :
    CAknMarkableListDialog( aValue, aSelectedItems, aArray, aObserver ),
    iPtiEngine( aPtiEngine ),
    iOwner( aOwner ),
    iOldIndex( 0 ),
    iOldLanguage( 0 ),
    iOldCbaRes( 0 )
    {
    // No implementation required
    }

CAknFepUserdbDlg::~CAknFepUserdbDlg()
    {
    delete iDBEngine;
    iDBEngine = NULL;
    iPtiEngine = NULL;
    iOwner = NULL;  
    iQueryDlg = NULL;
    if ( iEikonEnv && iContextMenuBar )
        {
        iEikonEnv->EikAppUi()->RemoveFromStack( iContextMenuBar );
        delete iContextMenuBar;
        }
    }

CAknFepUserdbDlg* CAknFepUserdbDlg::NewL(
    TInt &aOpenedItem, CArrayFix< TInt> *aSelectedItems, MDesCArray *aArray,
    TInt aMenuBarResourceId, CPtiEngine* aPtiEngine, CAknFepManager* aOwner,
    MEikCommandObserver *aObserver )
    {
    CAknFepUserdbDlg* self = new( ELeave ) CAknFepUserdbDlg( aOpenedItem, 
        aSelectedItems, aArray, aPtiEngine, aOwner, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL( aMenuBarResourceId );
    CleanupStack::Pop( self ); // self;
    return self;
    }
	
void CAknFepUserdbDlg::ConstructL( TInt aMenuBarResourceId )
    {
    CAknSelectionListDialog::ConstructL( aMenuBarResourceId );
    this->PrepareLC( R_AKNFEP_LISTDLG_DIALOG );
    HBufC* message = StringLoader::LoadLC( R_AKNFEP_USER_DB_LIST_NO_ITEM_TEXT );
    ListBox()->View()->SetListEmptyTextL( *message );
    ConstructContextMenuL();
    
    //the DB engine init
    iDBEngine = CDBManagement::NewL( iOwner );
    CleanupStack::PopAndDestroy( message ); //message
    }

TBool CAknFepUserdbDlg::InitListBoxItemL()
    {
    iItems = new( ELeave ) CDesCArrayFlat( KCDesCArraySize );
    
    TBuf<KErrMsgMaxLength> errMsg;
    
    //get the phrases from the DB
    if ( !iDBEngine->GetPhraseFromDBL( *iItems ) )
        {
        iDBEngine->GetErrMsg( errMsg );
        //note fot err message
        CAknWarningNote* note = new( ELeave ) CAknWarningNote();
        note->SetTone( CAknNoteDialog::EWarningTone );
        note->ExecuteLD( errMsg );  
        }
    else
        {
        InsertIconL( iItems );
        }  
    CAknSingleStyleListBox* listBox=
                static_cast<CAknSingleStyleListBox*>( Control( ESelectionListControl ) );
    CTextListBoxModel* model = listBox->Model(); 
    if ( iItems && iItems->Count() != 0 )
        {
        model->SetItemTextArray( iItems );
        model->SetOwnershipType( ELbmOwnsItemArray );
        ListBox()->HandleItemAdditionL();
        return ETrue;
        }
    else
        {
        model->SetItemTextArray( iItems );
        model->SetOwnershipType( ELbmOwnsItemArray );
        return EFalse;
        }
    }

void CAknFepUserdbDlg::AddItemL()
    {
    CAknSingleStyleListBox* listbox =
        static_cast<CAknSingleStyleListBox*>( Control( ESelectionListControl ) );
    // New item text
    TBuf<KListBoxItemMaxLength> itemText;
    TBuf<KErrMsgMaxLength> errMsg;     
    if ( FindBox()->TextLength() != 0 )
        {
        FindBox()->GetSearchText( itemText );
        }
    FindBox()->SetFocus( EFalse );
    HBufC* prompt = StringLoader::LoadLC( R_AKNFEP_USER_DB_LIST_ADD_ITEM_TEXT );
    CAknFepUserDBQueryDlg* dialog = 
        CAknFepUserDBQueryDlg::NewL( iDBEngine, itemText, *prompt );
    iQueryDlg = dialog;
    CleanupStack::PopAndDestroy( prompt );   
    //Execute dialog, 
    //In the dialog it will check input and show waring not
    if ( dialog->ExecuteLD( R_AKNFEP_LISTBOX_DATA_QUERY_DIALOG ) )
        {
        TInt currentIndex = 0;
        CAknFilteredTextListBoxModel* model = 
            static_cast<CAknFilteredTextListBoxModel*>( ListBox()->Model() );
        CDesCArray* itemArray =
                static_cast<CDesC16ArrayFlat*>( model->ItemTextArray() );
        
        //Add '\t' at the beginning of itemText for find
        itemText.Insert( 0, KItemStart() );
        //
        TInt ret = itemArray->Find( itemText, currentIndex, ECmpCollated);
        if ( ret != 0 )
            {
            //add the phrase to db
            itemText.Trim();
            if ( !iDBEngine->AddPhraseToDB(  itemText ) )
            	{
            	iDBEngine->GetErrMsg( errMsg );
                //note fot err message
                CAknWarningNote* note = new( ELeave ) CAknWarningNote();
                note->SetTone( CAknNoteDialog::EWarningTone );
                note->ExecuteLD( errMsg );  
                // add by chen  
            	return;
            	}
            // Add '\t' in front of text
            itemText.Insert( 0, KItemStart() );
            // Insert item to the item array 
            if ( itemArray->Count() > 0 )
                {
                itemArray->InsertL( currentIndex, itemText );
                }
            else
                {
                itemArray->InsertL( 0, itemText );
                }        
            // Update the listbox
            listbox->HandleItemAdditionL();
            itemArray->Sort(ECmpCollated);
            
            FindBox()->SetSearchTextL( KSearchTextEmpty );
            ResetSearchFieldL();
            model->Filter()->HandleItemArrayChangeL();
            
            //Highlight the item you have just edited
            itemArray->Find( itemText, currentIndex, ECmpCollated );
            listbox->SetCurrentItemIndexAndDraw( currentIndex );

            }
        else
            {
            FindBox()->SetSearchTextL( KSearchTextEmpty );
            ResetSearchFieldL();
            listbox->SetCurrentItemIndexAndDraw( currentIndex );
            }
        
        model->Filter()->UpdateCachedDataL();
        UpdateListBoxL();
        }
    }

void CAknFepUserdbDlg::RemoveItemL()
    {       
    //error message
    TBuf<KErrMsgMaxLength> errMsg;
    TBuf<KListBoxItemMaxLength> removePhrase; 
    TInt oldIndex = ListBox()->View()->CurrentItemIndex();    
    CAknFilteredTextListBoxModel* model = 
        static_cast<CAknFilteredTextListBoxModel*>( ListBox()->Model() );
    CDesCArrayFlat* item =
        static_cast<CDesCArrayFlat*>( model->ItemTextArray() );
    model->Filter()->UpdateSelectionIndexesL();
    TInt markItems = model->Filter()->SelectionIndexes()->Count();    
    if ( markItems )
        {       
        HBufC * message = StringLoader::LoadLC(
            R_AKNFEP_DELETE_CONFIRMATION_TEXT, markItems,
            iEikonEnv ); //message
        CAknQueryDialog* delsomenote =
            CAknQueryDialog::NewL( CAknQueryDialog::EConfirmationTone );       
        delsomenote->SetPromptL( *message );
        if ( delsomenote->ExecuteLD( R_AKNFEP_DELETE_QUERY_DIALOG ) )
            {  
            iProgressDialog = new (ELeave) CAknProgressDialog(
                ( REINTERPRET_CAST( CEikDialog**, &iProgressDialog ) ), ETrue );
            iProgressDialog->PrepareLC( R_AKNFEP_WAIT_DIALOG );
            iProgressInfo = iProgressDialog->GetProgressInfoL();
            iProgressInfo->SetFinalValue( markItems ); 
            iProgressDialog->RunLD();           
            CArrayFix<TInt>* selectionArray = new( ELeave ) CArrayFixFlat<TInt>( markItems );
            CleanupStack::PushL( selectionArray ); //message, selectionArray          
            //loop for copy selection index
            for ( TInt idx = markItems; idx > 0; idx-- )
                {
                selectionArray->AppendL( model->Filter()->SelectionIndexes()->At( idx-1 ) );
                }           
            TKeyArrayFix* key= new (ELeave) TKeyArrayFix(0, ECmpTInt);
            CleanupStack::PushL( key ); //message, selectionArray, key        
            //sort selection array
            selectionArray->Sort( *key );           
            CleanupStack::PopAndDestroy( key ); //message, selectionArray 
            // loop throught the selected item indexes, and delete items
            for ( TInt idx = markItems; idx > 0; idx-- )
                {
                // get the index of the selected item
                TInt itemIndex = selectionArray->At( idx-1 );
                //romve the phrase form the db
	            removePhrase.Copy( item->MdcaPoint( itemIndex ) );
	            removePhrase.Trim();
	            if( !iDBEngine->RemovePhraseFromDB(  removePhrase ) )
	            	{
                    iDBEngine->GetErrMsg( errMsg );
                    iProgressDialog->ProcessFinishedL();
                    iProgressDialog = NULL;  
	                CleanupStack::PopAndDestroy( selectionArray ); //message    
	                CleanupStack::PopAndDestroy( message );//
	                //note fot err message
	                CAknWarningNote* errnote = new( ELeave ) CAknWarningNote();
	                errnote->SetTone( CAknNoteDialog::EWarningTone );
	                errnote->ExecuteLD( errMsg );
	            	return;
	            	}
                iProgressInfo->IncrementAndDraw(1);    
                //delte the item
                item->Delete( itemIndex );
                }            
            CleanupStack::PopAndDestroy( selectionArray ); //message            
            iProgressDialog->ProcessFinishedL();
            iProgressDialog = NULL;              

            // Update the listbox
            ResetSearchFieldL();
            ListBox()->HandleItemRemovalL();
            model->Filter()->HandleItemArrayChangeL();
            model->Filter()->SelectionIndexes()->Reset();
            ListBox()->View()->ClearSelection();
            }
        CleanupStack::PopAndDestroy( message );//
        }
    else
        {
        HBufC * message = StringLoader::LoadLC(
            R_AKNFEP_DELETE_CONFIRMATION_TEXT, 1, 
            iEikonEnv );
        CAknQueryDialog* delonenote =
            CAknQueryDialog::NewL( CAknQueryDialog::EConfirmationTone );
        delonenote->SetPromptL( *message );        
        if ( delonenote->ExecuteLD( R_AKNFEP_DELETE_QUERY_DIALOG ) )
            {    
            //remove from db
            removePhrase.Copy( model->ItemText( ListBox()->CurrentItemIndex() ) );
            removePhrase.Delete( 0, 1 );
            if( !iDBEngine->RemovePhraseFromDB( removePhrase ) )
                {
                iDBEngine->GetErrMsg( errMsg );
                //note fot err message
                CAknWarningNote* errnote = new( ELeave ) CAknWarningNote();
                errnote->ExecuteLD( errMsg );   
                CleanupStack::PopAndDestroy( message );
                return;
                }            
            TInt itemIndex = ListBox()->View()->CurrentItemIndex();           
            //get the actual index of list
            TInt actualIndex = FindActualIndex( itemIndex );            
            // Remove highlighted item from the listbox.
            item->Delete( actualIndex );  
            // Update the listbox
            ResetSearchFieldL();
            }
        CleanupStack::PopAndDestroy( message );
        }
    // If focused item is out of range, bring it forth
    if ( ( ( item->Count()-1 ) >= 0 ) && FindBox()->TextLength() == 0 )
        {
        if ( ( oldIndex > ( item->Count()-1 ) ))
            {
            ListBox()->SetCurrentItemIndexAndDraw( item->Count()-1 );
            }
        else
            {
            ListBox()->SetCurrentItemIndexAndDraw( oldIndex );
            }
        } 
    UpdateListBoxL();
    }

void CAknFepUserdbDlg::EditItemL()
    {
    CAknSingleStyleListBox* listbox = 
        static_cast<CAknSingleStyleListBox*>( Control( ESelectionListControl ) );        
       
    // If we don't have items, do nothing and return
    if ( listbox->Model()->NumberOfItems() < 1 )
        {
        return;
        }
    CTextListBoxModel* model = listbox->Model();
    // New item text
    TBuf<KListBoxItemMaxLength> itemText;
    TBuf<KErrMsgMaxLength> errMsg;
    TBuf<KListBoxItemMaxLength> itemOldText;
    itemText.Copy( model->ItemText(listbox->CurrentItemIndex () ) );
    itemText.Trim();
    itemOldText.Copy( model->ItemText( listbox->CurrentItemIndex() ) );
    itemOldText.Trim();    
    HBufC* prompt = StringLoader::LoadLC( R_AKNFEP_USER_DB_LIST_EDIT_ITEM_TEXT );
    CAknFepUserDBQueryDlg* dialog = 
        CAknFepUserDBQueryDlg::NewL( iDBEngine, itemText, *prompt );
    iQueryDlg = dialog;
    CleanupStack::PopAndDestroy( prompt );
    // Execute dialog
    if ( dialog->ExecuteLD( R_AKNFEP_LISTBOX_DATA_QUERY_DIALOG ) )
        {
        TInt currentIndex = 0;
        CDesCArray* itemArray = 
            static_cast<CDesC16ArrayFlat*>( model->ItemTextArray() );
        
        // Add '\t' in front of text for find
        itemText.Insert( 0, KItemStart() );
        //Find if the text is exist
        TInt ret = itemArray->Find( itemText, currentIndex, ECmpCollated );
        if ( ret != 0 )
            {
            //edit the phrase from db
            itemText.Trim();
            if(!iDBEngine->EditPhraseFromDB(  itemOldText, itemText ) )
            	{
            	iDBEngine->GetErrMsg( errMsg );
            
                //note fot err message
                CAknWarningNote* note = new( ELeave ) CAknWarningNote();
                note->ExecuteLD( errMsg );            	                	
                return;
                }
            currentIndex = ListBox()->CurrentItemIndex();
            //get the actual index of list
            TInt actualIndex = FindActualIndex( currentIndex );
            
            // Remove highlighted item from the listbox.
            itemArray->Delete( actualIndex );

            // Add '\t' in front of text
            itemText.Insert( 0, KItemStart() );
            
            // Insert item to the item array
            itemArray->InsertL( actualIndex, itemText );
            
            // Update the listbox
            listbox->HandleItemAdditionL();
            itemArray->Sort(ECmpCollated);
            
            if ( FindBox()->TextLength() != 0 )
                {
                FindBox()->SetSearchTextL( KSearchTextEmpty );
                ResetSearchFieldL();
                }
                        
            ListBox()->View()->ClearSelection();
            CAknFilteredTextListBoxModel* filterModel = 
                static_cast<CAknFilteredTextListBoxModel*>( ListBox()->Model() );
            filterModel->Filter()->HandleItemArrayChangeL();
            filterModel->Filter()->UpdateCachedDataL();
            
            //SetHightlight
            itemArray->Find( itemText, currentIndex, ECmpCollated );
            ListBox()->SetCurrentItemIndexAndDraw( currentIndex );
            }
        else
            {
            //get the actual index of list
            TInt actualIndex = FindActualIndex( listbox->CurrentItemIndex() );
            if ( currentIndex != actualIndex )
                {
                itemOldText.Copy( model->ItemText( listbox->CurrentItemIndex() ) );
                itemOldText.Trim();
                //Here need user db engine
                if( !iDBEngine->RemovePhraseFromDB( itemOldText ) )
                    {
                    iDBEngine->GetErrMsg( errMsg );
                
                    //note fot err message
                    CAknWarningNote* note = new( ELeave ) CAknWarningNote();
                    note->ExecuteLD( errMsg );                    
                    return;
                    }
                 
                currentIndex = ListBox()->CurrentItemIndex();
                
                //get the actual index of list
                actualIndex = FindActualIndex( currentIndex );
                
                // Remove highlighted item from the listbox.
                itemArray->Delete( actualIndex );
                
                listbox->HandleItemRemovalL();
                itemArray->Sort(ECmpCollated);
                
                if ( FindBox()->TextLength() != 0 )
                    {
                    FindBox()->SetSearchTextL( KSearchTextEmpty );
                    ResetSearchFieldL();
                    }
                
                ListBox()->View()->ClearSelection();
                CAknFilteredTextListBoxModel* filterModel = 
                    static_cast<CAknFilteredTextListBoxModel*>( ListBox()->Model() );
                filterModel->Filter()->HandleItemArrayChangeL();
                filterModel->Filter()->UpdateCachedDataL();

                //SetHightlight
                itemArray->Find( itemText, currentIndex, ECmpCollated );
                ListBox()->SetCurrentItemIndexAndDraw( currentIndex );
                }
            }
        UpdateListBoxL();
        }
    }

void CAknFepUserdbDlg::CmdMarkL( TInt aItemIndex, TBool aMark )
    {
    CAknSingleStyleListBox* listbox = 
        static_cast<CAknSingleStyleListBox*>( Control( ESelectionListControl ) );
    CTextListBoxModel* model = listbox->Model();
    TInt itemCount = model->NumberOfItems();
    CListBoxView* listBoxView = listbox->View();
    if ( aMark )
        {
        for ( TInt i = 0; i < itemCount; i++ )
            if ( aItemIndex == -1 || aItemIndex == i )
                listBoxView->SelectItemL( i );
        }
    else
        {
        for ( TInt i = 0; i < itemCount; i++ )
            {
            if ( aItemIndex == -1 || aItemIndex == i )
                {
                listBoxView->DeselectItem( i );
                }
            }
        }
    CAknFilteredTextListBoxModel* filterModel = 
        static_cast<CAknFilteredTextListBoxModel*>( ListBox()->Model() );
    filterModel->Filter()->UpdateCachedDataL();
    }

void CAknFepUserdbDlg::ChangeCurrentCbaL ( TInt aCbaResourceId )
    {
    if ( iOwner->IsEntryPhrase() || iOldCbaRes == aCbaResourceId )
        {
        return;
        }
    CEikButtonGroupContainer& cba = ButtonGroupContainer();
    cba.SetCommandSetL( aCbaResourceId );
    cba.AddCommandSetToStackL( aCbaResourceId );
    iOldCbaRes = aCbaResourceId;
    cba.DrawNow();
    }

void CAknFepUserdbDlg::ProcessCommandL( TInt aCommandId )
    {   
    HideMenu();
    
    iContextMenuBar->StopDisplayingMenuBar();
    
    switch ( aCommandId )
        {
        case EAknCmdUserDBAdd:
            {
            AddItemL();
            }
            break;
        case EAknCmdUserDBEdit:
            {
            EditItemL();
            }
            break;
        case EAknCmdUserDBDelete:
            {
            RemoveItemL();
            }
            break;
        case EAknCmdMark:
        case EAknMarkAll:
        case EAknCmdUnmark: 
        case EAknUnmarkAll:
            {
            AknSelectionService::HandleMarkableListProcessCommandL( aCommandId, ListBox() );
            UpdateListBoxL( aCommandId );
            }
            break;
        default:
            {
            break;
            }
        }
    }

TBool CAknFepUserdbDlg::OkToExitL( TInt aButtonId )
    {
    switch ( aButtonId )
        {
        case EAknSoftkeyOptions:
            {
            CAknSelectionListDialog::DisplayMenuL(); // display the menu
            }
            break;
        case EAknCmdUserDBAdd:
            {
            ProcessCommandL(EAknCmdUserDBAdd);
            }
            break;
        case EAknSoftkeyMark:
            {
            TInt ItemCount = ListBox()->Model()->NumberOfItems(); 
            TInt selectedItem = ListBox()->SelectionIndexes()->Count();
            if ( selectedItem < 1 && ItemCount > 0 )
                {
                //Show options-context-exit cba
                ChangeCurrentCbaL ( R_AKNFEP_SOFTKEYS_LIST_OPTIONS_EXIT_CONTEXT );
                }
            }
            break;
        case EAknSoftkeyUnmark:
            {
            TInt ItemCount = ListBox()->Model()->NumberOfItems(); 
            TInt selectedItem = ListBox()->SelectionIndexes()->Count();
            if ( selectedItem == 0 && ItemCount > 0 )
                {
                //Show options-context-exit cba
                ChangeCurrentCbaL ( R_AKNFEP_SOFTKEYS_LIST_OPTIONS_EXIT_CONTEXT );
                }
            }
            break;
        case EAknSoftkeyContextOptions:
            {
            TInt items = ListBox()->Model()->NumberOfItems();
            if ( items )
                {
                ShowContextMenuL();
                }
            }
            break;
        case EAknCmdMark:
        case EAknMarkAll:
        case EAknCmdUnmark: 
        case EAknUnmarkAll:
            {
            ProcessCommandL( aButtonId );
            }
            break;
        case EAknSoftkeyExit:
            {
            return ETrue;
            }
        default:
            {
            break;
            }
        }
    
    return EFalse;
    }

void CAknFepUserdbDlg::FocusChanged(TDrawNow aDrawNow)
	{
	if ( aDrawNow == EDrawNow )
		{
	    if ( ( FindBox()->IsVisible() && !iOwner->IsEntryPhrase()) )
	        {
	        FindBox()->SetFocus( ETrue );
	        }
		}
	}

void CAknFepUserdbDlg::DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane )
    {  
    if ( aResourceId == R_AKNFEP_USERDB_DLG_MENU )
        {
        const CArrayFix<TInt>* itemArray = ListBox()->SelectionIndexes();
        if ( itemArray->Count() != 0 )
            {
            aMenuPane->SetItemDimmed( EAknCmdUserDBAdd, ETrue );
            aMenuPane->SetItemDimmed( EAknCmdUserDBEdit, ETrue );
            }
        else
            {
            aMenuPane->SetItemDimmed( EAknCmdUserDBAdd, EFalse );
            aMenuPane->SetItemDimmed( EAknCmdUserDBEdit, EFalse );
            }
        }        
    if ( aResourceId == R_AKNFEP_MARK_UNMARK_MENUPANE )
        {
        TInt currentIndex = ListBox()->CurrentItemIndex();
        TInt selectedItem = ListBox()->SelectionIndexes()->Count();
        TInt listItems = ListBox()->Model()->NumberOfItems();
    
        // Display mark, mark all
        if ( selectedItem == 0 )
            {
            aMenuPane->SetItemDimmed( EAknCmdUnmark, ETrue );
            aMenuPane->SetItemDimmed( EAknUnmarkAll, ETrue );
            return;
            }
        // Display unmark, unmark all
        if ( selectedItem == listItems )
            {
            aMenuPane->SetItemDimmed( EAknCmdMark, ETrue );
            aMenuPane->SetItemDimmed( EAknMarkAll, ETrue );
            return;
            }
        TBool isMarked = EFalse;
        
        for ( TInt i = 0; i < selectedItem; i++ )
            {
            TInt selectedId = ListBox()->SelectionIndexes()->At( i );
            if ( selectedId == currentIndex )
                {
                isMarked = ETrue;
                }
            }

        if ( isMarked )
            {
            aMenuPane->SetItemDimmed( EAknCmdMark, ETrue );
            }
        else
            {
            aMenuPane->SetItemDimmed( EAknCmdUnmark, ETrue );
            }
        }
    }

TKeyResponse CAknFepUserdbDlg::OfferKeyEventL( const TKeyEvent& aKeyEvent, 
                                               TEventCode aType )
    {   
    TKeyResponse ret(EKeyWasNotConsumed);
    
    CAknFilteredTextListBoxModel* filterModel = 
        static_cast<CAknFilteredTextListBoxModel*>( ListBox()->Model() );
    
    TInt ItemCount = ListBox()->Model()->NumberOfItems();  
    
    ListBoxItemsChanged( ListBox() );
    
    if ( ret != EKeyWasConsumed && aKeyEvent.iCode == EKeyPhoneEnd )
        {
        TryExitL( EAknSoftkeyExit );
        return EKeyWasConsumed;
        }
    
    TBool claerKeyPressed = ( aType == EEventKey ) && ( aKeyEvent.iCode ==  EKeyBackspace );
    if( claerKeyPressed && ItemCount > 0 && FindBox()->TextLength() == 0 )
        {
        ProcessCommandL(EAknCmdUserDBDelete);
        UpdateListBoxL();
        ret =  EKeyWasConsumed;
        }
    else
        {
        if ( FindBox()->IsVisible() )
            {
            ret = CAknMarkableListDialog::OfferKeyEventL( aKeyEvent, aType );
            }
        else
            {
            ret =  ListBox()->OfferKeyEventL( aKeyEvent, aType );
            }
        }
       
    if ( ret != EKeyWasConsumed && aType == EEventKey &&
         aKeyEvent.iCode == EKeyOK )
        {
        iContextMenuBar->TryDisplayMenuBarL();
        ret = EKeyWasConsumed;
        }
            
    return ret;
    }

void CAknFepUserdbDlg::PostLayoutDynInitL()
    {
    CAknMarkableListDialog::PostLayoutDynInitL();
    FindBox()->Editor().SetMaxLength( KFindBoxInputMaxLength );
    ListBox()->SetObserver( this );
    ListBox()->AddItemChangeObserverL( this );
    
    InitListBoxItemL();
    UpdateListBoxL();

    return;
    }

void CAknFepUserdbDlg::ConstructContextMenuL()
    {
    CEikMenuBar* newMenuBar = new ( ELeave ) CEikMenuBar();
    CleanupStack::PushL( newMenuBar );
    newMenuBar->ConstructL
        ( this, NULL, R_AKNFEP_USERDB_DLG_CONTEXT_MENUBAR );
    iEikonEnv->EikAppUi()->AddToStackL( newMenuBar, ECoeStackPriorityMenu,
        ECoeStackFlagRefusesFocus );
    iContextMenuBar = newMenuBar;
    CleanupStack::Pop( newMenuBar );
    }

void CAknFepUserdbDlg::HandleListBoxEventL(CEikListBox* /*aListBox*/, TListBoxEvent aEventType)
    {
    CTextListBoxModel* model = static_cast<CTextListBoxModel*>( ListBox()->Model() );
    CDesCArrayFlat* itemArray =
            static_cast<CDesCArrayFlat*>( model->ItemTextArray() );
    TInt listItems = itemArray->Count();
    
    if ( !listItems )
        {
        return;
        }
    
    CAknSelectionListDialog::HandleListBoxEventL( ListBox(), aEventType );
    
    switch ( aEventType )
        {
        case EEventItemDoubleClicked:
            {
            TInt selectItems = ListBox()->SelectionIndexes()->Count();
            if ( selectItems )
                {
                iOldIndex = ListBox()->CurrentItemIndex();
                MarkOrUnmarkItemL();
                }
            else
                {
                ShowContextMenuL();
                }
            }
            break;
        default:
            break;
        }
    }


void CAknFepUserdbDlg::ResetSearchFieldL()
    {
    CAknFilteredTextListBoxModel* selectmodel = 
        static_cast<CAknFilteredTextListBoxModel*>( ListBox()->Model() );
    selectmodel->Filter()->HandleItemArrayChangeL(); 
    }

TInt CAknFepUserdbDlg::FindActualIndex( const TInt aItemIndex )
    {
    TInt itemIndex;
    CTextListBoxModel* model = 
        static_cast<CTextListBoxModel*>( ListBox()->Model() );
    CDesCArrayFlat* itemArray =
        static_cast<CDesCArrayFlat*>( model->ItemTextArray() );
    //get the real index of list
    TBuf<KListBoxItemMaxLength> itemText;
    itemText.Copy( model->ItemText(aItemIndex) );
    itemArray->Find( itemText, itemIndex, ECmpCollated );
    return itemIndex;
    }

void CAknFepUserdbDlg::ShowContextMenuL()
    {
    iContextMenuBar->SetMenuTitleResourceId( R_AKNFEP_USERDB_DLG_CONTEXT_MENUBAR );
    iContextMenuBar->SetMenuType( CEikMenuBar::EMenuContext );
    iContextMenuBar->StopDisplayingMenuBar();
    iContextMenuBar->TryDisplayMenuBarL();
    iContextMenuBar->SetMenuTitleResourceId( R_AKNFEP_USERDB_DLG_MENUBAR );
    iContextMenuBar->SetMenuType( CEikMenuBar::EMenuOptions );
    }

void CAknFepUserdbDlg::MarkOrUnmarkItemL()
    {
    TInt selectIndex = 0;
    TKeyArrayFix* key= new (ELeave) TKeyArrayFix( 0, ECmpTInt32 );
    CleanupStack::PushL( key );
    TInt index = ListBox()->CurrentItemIndex();
    const CArrayFix<TInt>* selectitemArray = ListBox()->SelectionIndexes();
    TInt ret = selectitemArray->Find( index, *key, selectIndex );
    if ( ret == 0)
        {
        ProcessCommandL( EAknCmdUnmark );
        }
    else
        {
        ProcessCommandL( EAknCmdMark );
        }
    CleanupStack::PopAndDestroy( key );
    }

void CAknFepUserdbDlg::InsertIconL( CDesC16Array* aDataArray )
    {
    TBuf<KListBoxItemMaxLength> itemText;
    for ( TInt i = 0; i < aDataArray->Count(); i++ )
        {
        itemText = aDataArray->MdcaPoint( i );
        itemText.Insert( 0, KItemStart );
        aDataArray->Delete( i );
        aDataArray->InsertL( i, itemText );
        }
    aDataArray->Sort( ECmpCollated );
    }

void CAknFepUserdbDlg::OnChangeLanguageL( TInt aInputLanguage )
    {
    if ( aInputLanguage != ELangPrcChinese &&
    	 aInputLanguage != ELangTaiwanChinese &&
    	 aInputLanguage != ELangHongKongChinese )
    	{
    	if ( iQueryDlg && iQueryDlg->IsVisible() )
    		{
    		delete iQueryDlg;
    		iQueryDlg = NULL;
    		}
    	this->TryExitL( EAknSoftkeyExit );
    	return;
    	}
    if ( iOldLanguage == aInputLanguage )
        {
        return;
        }
    iOldLanguage = aInputLanguage;
    
    CAknFilteredTextListBoxModel* model = 
        static_cast<CAknFilteredTextListBoxModel*>( ListBox()->Model() );
    CDesCArrayFlat* itemArray =
            static_cast<CDesCArrayFlat*>( model->ItemTextArray() );
    TBuf<KErrMsgMaxLength> errMsg;   

    AknSelectionService::HandleMarkableListProcessCommandL( EAknUnmarkAll, ListBox() );
    itemArray->Reset();
    
    //get the phrases from the DB
    if ( !iDBEngine->GetPhraseFromDBL( *itemArray ) )
        {
        iDBEngine->GetErrMsg( errMsg );
        //note fot err message
        CAknWarningNote* note = new( ELeave ) CAknWarningNote();
        note->SetTone( CAknNoteDialog::EWarningTone );
        note->ExecuteLD( errMsg );  
        }
    else
        {
        InsertIconL( itemArray ); 
        ListBox()->HandleItemAdditionL();
        ListBox()->ClearSelection();
        FindBox()->SetFocus( ETrue );
        FindBox()->SetFocusing( ETrue );
        FindBox()->MakeVisible( ETrue );
        }
    if ( itemArray->Count() == 0 )
        {
        FindBox()->SetSearchTextL( KSearchTextEmpty );
        FindBox()->SetFocus( EFalse );
        FindBox()->SetFocusing( EFalse );
        FindBox()->MakeVisible( EFalse );
        ListBox()->SetRect( Rect() );
        ListBox()->View()->DrawEmptyList( Rect() );
    
        //Show add-add-exit cba 
        ChangeCurrentCbaL( R_AKNFEP_SOFTKEYS_LIST_ADD_EXIT );
        }
    else
        {
        ListBox()->SetCurrentItemIndex( 0 );
        }
    }

void CAknFepUserdbDlg::ListBoxItemsChanged( CEikListBox* aListBox )
    {
    CAknFilteredTextListBoxModel* filterModel = 
        static_cast<CAknFilteredTextListBoxModel*>( aListBox->Model() );
    CDesCArrayFlat* itemArray =
            static_cast<CDesCArrayFlat*>( filterModel->ItemTextArray() );
    
    TInt ItemCount = aListBox->Model()->NumberOfItems(); 
    TInt selectedItem = filterModel->Filter()->SelectionIndexes()->Count();
    if ( ItemCount > 0 && selectedItem < 1 )
        {
        //Show options-context-exit cba
        TRAP_IGNORE ( ChangeCurrentCbaL( R_AKNFEP_SOFTKEYS_LIST_OPTIONS_EXIT_CONTEXT ) );
        }
    else
        {
        if ( ItemCount == 0)
            {
            //Show add-add-exit cba
            TRAP_IGNORE ( ChangeCurrentCbaL( R_AKNFEP_SOFTKEYS_LIST_ADD_EXIT ) );
            if ( itemArray->Count() == 0 && FindBox()->TextLength() == 0 )
                {
                FindBox()->SetFocus( EFalse );
                FindBox()->MakeVisible( EFalse );
                }
            
            if ( FindBox()->TextLength() == 0 )
                {
                ListBox()->SetRect( Rect() );
                ListBox()->View()->DrawEmptyList( Rect() );
                }
            }
        }
    }

void CAknFepUserdbDlg::UpdateListBoxL( TInt aCommandId )
	{
    // Update the listbox
    CAknFilteredTextListBoxModel* filterModel = 
        static_cast<CAknFilteredTextListBoxModel*>( ListBox()->Model() );
    filterModel->Filter()->UpdateCachedDataL();
    
    TInt selectedItem = ListBox()->SelectionIndexes()->Count();
    
    
    CAknFilteredTextListBoxModel* model = 
        static_cast<CAknFilteredTextListBoxModel*>( ListBox()->Model() );
    CDesCArrayFlat* itemArray =
            static_cast<CDesCArrayFlat*>( model->ItemTextArray() );
    TInt listItems = model->NumberOfItems();
    
    if ( listItems != 0 && selectedItem != 0)
        {
        TInt CurrentItemIndex = ListBox()->CurrentItemIndex();          
        if( ( aCommandId == EAknCmdMark || aCommandId == EAknMarkAll ) && 
        	 ListBox()->View()->ItemIsSelected(CurrentItemIndex))
        	{
        	//Show options-unmark-exit menu
        	ChangeCurrentCbaL( R_AKNFEP_SOFTKEYS_LIST_OPTIONS_EXIT_MARK );
        	}      
        if( ( aCommandId == EAknCmdUnmark || aCommandId == EAknUnmarkAll ) && 
             !ListBox()->View()->ItemIsSelected(CurrentItemIndex))
            {
             //Show options-mark-exit menu
            ChangeCurrentCbaL( R_AKNFEP_SOFTKEYS_LIST_OPTIONS_EXIT_UNMARK );      
            }
        }
    
    if ( listItems == 0 )
        {
        //Show add-add-exit cba       
        ChangeCurrentCbaL( R_AKNFEP_SOFTKEYS_LIST_ADD_EXIT ); 
        if ( itemArray->Count() == 0 && FindBox()->TextLength() == 0 )
            {
            FindBox()->SetFocus( EFalse );
            FindBox()->MakeVisible( EFalse );
            }
        
        if ( FindBox()->TextLength() == 0 )
            {
            ListBox()->SetRect( Rect() );
            ListBox()->View()->DrawEmptyList( Rect() );
            }
        }
    if( selectedItem == 0 && listItems != 0 )
        {
        FindBox()->MakeVisible( ETrue );
        FindBox()->SetFocus( ETrue );
        FindBox()->DrawNow();
        TRect findpaneRect(FindBox()->Rect());
        TRect clientRect = Rect();
        TSize size = TSize(clientRect.Width(),
            clientRect.Height() - findpaneRect.Height());
        ListBox()->SetRect( TRect(clientRect.iTl, size) );
        ListBox()->View()->Draw();
        //Show options-context-exit cba
        ChangeCurrentCbaL( R_AKNFEP_SOFTKEYS_LIST_OPTIONS_EXIT_CONTEXT );
        }
    
    if ( FindBox()->IsVisible() )
        {
        FindBox()->SetFocus( ETrue );
        FindBox()->MakeVisible( ETrue );
        }
	}

CCoeControl* CAknFepUserdbDlg::ComponentControl(TInt aIndex) const 
    { 
    if ( aIndex == CAknMarkableListDialog::CountComponentControls() ) 
        { 
        return ListBox(); 
        } 
    else 
        { 
        return CAknMarkableListDialog::ComponentControl(aIndex); 
        } 
    } 

TInt CAknFepUserdbDlg::CountComponentControls() const 
    { 
    if ( ListBox() )
	    {
	    return CAknMarkableListDialog::CountComponentControls() + 1; 
		}
    else
	    {
        return CAknMarkableListDialog::CountComponentControls();
        }
    }



void CAknFepUserdbDlg::SizeChanged()
    {
    //Change title text
    CEikStatusPane* statusPane = CEikonEnv::Static()->AppUiFactory()->StatusPane();
    CAknTitlePane* title = NULL;
    TRAP_IGNORE( title = static_cast< CAknTitlePane* >(
        statusPane->ControlL( TUid::Uid( EEikStatusPaneUidTitle) ) ) );

    //Load the new title text
    HBufC* titletext = StringLoader::LoadLC( R_AKNFEP_USER_DB_MANAGEMENT_TITLE );
    //Set title to the new text
    TRAP_IGNORE( title->SetTextL( *titletext ));
    CleanupStack::PopAndDestroy( titletext );
    }
// End of file
