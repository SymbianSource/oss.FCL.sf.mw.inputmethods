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
* Description:            Define dialog for user DB management
*
*/












#ifndef C_CAKNFEPUSERDBDLG_H
#define C_CAKNFEPUSERDBDLG_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <aknselectionlist.h> //CAknMarkableListDialog
#include <AknProgressDialog.h> 
#include <eiklbo.h> // MEikListBoxObserver 
#include <eiklbx.h> //MListBoxItemChangeObserver

const TInt KMaxInputSize(7);
const TInt KMaxCharSize(7);

// CLASS DECLARATION
class CDBManagement;
class CPtiEngine;
class CAknFepManager;
class CAknProgressDialog;
class CEikProgressInfo;
class CAknFepUserDBQueryDlg;

/**
 *  Dialog for user DB management
 *  In this Dialog of user DB management, it has add,remove,
 *  edit and mark/unmark function
 *
 *  @code
 *   ?good_class_usage_example(s)
 *  @endcode
 *
 *  @lib ?library
 *  @since S60 v3.1
 */
class CAknFepUserdbDlg : public CAknMarkableListDialog,
                         public MListBoxItemChangeObserver 
    {
public:
    // Constructors and destructor

    /**
     * Destructor.
     */
    ~CAknFepUserdbDlg();

    /**
     * Two-phased constructor.
     * @param aOpenedItem Index of the item focusing on
     * @param aSelectedItems Selected of list items
     * @param aArray Content of list items
     * @param aMenuBarResourceId Menu bar id
     * @param aCommand Default value is zero
     */
    static CAknFepUserdbDlg* NewL( TInt &aOpenedItem,
            CArrayFix< TInt > *aSelectedItems, MDesCArray *aArray,
            TInt aMenuBarResourceId, CPtiEngine* aPtiEngine, CAknFepManager* aOwner,
            MEikCommandObserver *aObserver=0 );
    
    /**
     * Refresh user db view
     *
     * @since S60 v3.1
     * @param none
     * @return none
     */
    void OnChangeLanguageL( TInt aInputLanguage );

    /**
     * Refresh user db view when the screen size was changed.
     *
     * @since S60 vTB9.1
     * @param none
     * @return none
     */
    void SizeChanged();
	
	/**
     * Return control to application framework.
     *
     * @since S60 vTB9.2
     * @param TInt aIndex is the index of control
     * @return CCoeControl object pointer
     */
    CCoeControl* ComponentControl(TInt aIndex) const; 
	
	/**
     * Return number of controls in this dialog
     *
     * @since S60 vTB9.2
     * @param none
     * @return number of controls
     */
    TInt CountComponentControls() const;
	
private:

    /**
     * C++ constructor.
     * @param aOpenedItem Index of the item focusing on
     * @param aSelectedItems Selected of list items
     * @param aArray Content of list items
     * @param aCommand Default value is zero
     */
    CAknFepUserdbDlg( TInt &aValue, CArrayFix< TInt > *aSelectedItems,
            MDesCArray *aArray, CPtiEngine* aPtiEngine, CAknFepManager* aOwner,
            MEikCommandObserver *aObserver );

    /**
     * Symbian second-phase constructor.
     * @param aMenuBarResourceId Menu bar id
     */
    void ConstructL( TInt aMenuBarResourceId );
    
    /**
     * Construct Context Menu
     *
     * @since S60 v3.1
     * @param None
     * @return None 
     */
    void ConstructContextMenuL();
    
    /**
     * Init items in the list from user DB
     *
     * @since S60 v3.1
     * @param None
     * @return Whether there are any items in list, ETrue is have items 
     */
    TBool InitListBoxItemL( );
    
    /**
     * Add phrase to listbox and user DB
     *
     * @since S60 v3.1
     * @param none
     * @return none
     */
    void AddItemL();

    /**
     * Delete phrase in listbox and user DB
     *
     * @since S60 v3.1
     * @param none
     * @return none
     */
    void RemoveItemL();

    /**
     * Edit phrase in listbox and user DB
     *
     * @@since S60 v3.1
     * @param none
     * @return none
     */
    void EditItemL();

    /**
     * Mark/Unmark phrase in listbox and user DB
     *
     * @since S60 v3.1
     * @param aItemIndex item's index which would be mark or unmark
     * @return aMark set for the item mark
     */
    void CmdMarkL( TInt aItemIndex, TBool aMark );
    
    /**
     * Change the current cba to the specific cba
     *
     * @since S60 v3.1
     * @param aCbaResourceId cba id
     * @return none
     */
    void ChangeCurrentCbaL ( TInt aCbaResourceId );
    
    /**
     * Show context menu
     *
     * @since S60 v3.1
     * @param none
     * @return none
     */
    void ShowContextMenuL();
    
    /**
     * Set the Search Field blank
     *
     * @since S60 v3.1
     * @param none
     * @return none
     */
    void ResetSearchFieldL();
    
    /**
     * Find item's real Index in list
     *
     * @since S60 v3.1
     * @param aItemIndex Item's index in the view which want to find the actual index
     * @return The actual index of the item
     */
    TInt FindActualIndex( const TInt aItemIndex ); 
    /**
     * Mark or unmark item in list
     *
     * @since S60 v3.1
     * @param none
     * @return none
     */
    void MarkOrUnmarkItemL();
    
    /**
     * Add '\t' to item array
     *
     * @since S60 v3.1
     * @param aDataArray array which want to be inserted '\t'
     * @return none
     */
    void InsertIconL( CDesC16Array* aDataArray );
    
    /**
     * To update cba of dialg
     *
     * @since S60 v3.1
     * @param aCommandId command id from menu, 0 for command except mark
     * @return none
     */
    void UpdateListBoxL( TInt aCommandId = 0);
        
    //From base class CAknMarkableListDialog
    /**
     * From base class CAknMarkableListDialog
     * Deal the commands that have recevied
     * @param aCommandId Commands' Id
     * @return None
     */
    void ProcessCommandL( TInt aCommandId );
    
    /**
     * From base class CAknMarkableListDialog
     * Deal the softkey event
     * @param aButtonId softkey Id
     * @return ETrue to validate and exit the dialog, EFalse to keep the dialog active.
     */
    TBool OkToExitL( TInt aButtonId );
    
    /**
     * From base class CAknMarkableListDialog
     * Dynamically initialises a menu pane.
     * @param aResourceId Menu pane id
     * @param aMenuPane The in-memory representation of the menu pane.
     * @return None
     */
    void DynInitMenuPaneL( TInt aResourceId,CEikMenuPane* aMenuPane );
    
    void FocusChanged(TDrawNow aDrawNow);
    
    /**
     * From base class CAknMarkableListDialog
     * Initializes the dialog's controls after the 
     * dialog has been sized but before it has been activated
     * @param None
     * @return None
     */
    void PostLayoutDynInitL();
    
    /**
     * From base class CAknMarkableListDialog
     * @param aKeyEvent The key event.
     * @param aType The type of key event: EEventKey, EEventKeyUp or EEventKeyDown
     * @return Indicates whether or not the key event was used by this control
     */
    TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );

    //From base class MEikListBoxObserver
    /**
     * From base class MEikListBoxObserver
     * Observer callback from the listbox..
     * @param aListBox  ptr to listbox that is calling the method  
     * @param aEventType  type of event; defined by MEikListBoxObserver 
     * @return None
     */
    void HandleListBoxEventL(CEikListBox* aListBox, TListBoxEvent aEventType); 
    
    //From base class MListBoxItemChangeObserver
    /**
     * From base class MListBoxItemChangeObserver
     * Item change observer will be notified when list box items have been 
     * added or removed or the list box has been reset.
     * @param aListBox  ptr to listbox that is calling the method  
     * @return None
     */
    void ListBoxItemsChanged( CEikListBox* aListBox );
    
private: //data

    /**
     * Engine
     * Own
     */
	CDBManagement* iDBEngine;  
	
    /**
     * Progress dialog
     * Own, but doesn't need delete by myself
     */
	CAknProgressDialog*  iProgressDialog;
	
    /**
     * progress info
     * Own, but doesn't need delete by myself
     */
	CEikProgressInfo*    iProgressInfo;
	
    /**
     * Item array
     * Own, but doesn't need delete by myself,listbox would free it automatic
     */
	CDesC16Array* iItems;
	
    /**
     * Pti Engine
     * Not Own
     */
    CPtiEngine* iPtiEngine;
    
    /**
     * AknFepManager need by DBEngine
     * Not own
     */
	CAknFepManager* iOwner;
	
    /**
     * Context menu bar
     * Own
     */
	CEikMenuBar* iContextMenuBar;
    
    /**
     * Old highLight postion
     */
    TInt iOldIndex;

    /**
     * Old Language
     */
    TInt iOldLanguage;
    
    /**
     * Query for add and edit. Not Own.
     */
    CAknFepUserDBQueryDlg* iQueryDlg;
    
    /**
     * Old cba resouce
     */
    TInt iOldCbaRes;
    };

#endif // C_CAKNFEPUSERDBDLG_H
