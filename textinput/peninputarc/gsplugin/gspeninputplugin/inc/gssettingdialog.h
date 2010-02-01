/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  gspeninputimple dialog header file.
*
*/


#ifndef C_GSSETTINGDIALOG_H
#define C_GSSETTINGDIALOG_H

// INCLUDES
#include <aknsettingitemlist.h>
#include <AknDialog.h>

#include "gscenrepnotifyhandlercallback.h"

// FORWARD CLASS DECLARATION
class CGSPenInputImple;
class CGSSettingItemList;
class CAknNullService;

// CONSTANTS
const TInt KItemTextSize = 50;

/**
 *  CGSColorSettingItem item class
 *
 *  This class is trail colour item class. It offers trail colour item in setting 
 *  list.Form this class, user can define item behavior and interface by themselves.
 *
 *  @lib gspeninputplugin.dll
 *  @since S60 v3.2
 */
class CGSColorSettingItem : public CAknSettingItem
    {
    
public:

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @param aIdentifier Resource id
     * @param aImpl CGSPenInputImple instance
     * @return None
     */
    CGSColorSettingItem(const TInt aIdentifier, CGSPenInputImple* aImpl);

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CGSColorSettingItem();

    /**
     * Create a mask for item mask
     *
     * @since S60 v3.2
     * @return None
     */
    void CreateMaskL();

// From CAknSettingItem

    /**
     * From CAknSettingItem
     * This launches the setting page 
     *
     * @since S60 v3.2
     * @param aCalledFromMenu Ignored in this class
     * @return None
     */
    void EditItemL(TBool aCalledFromMenu);

    /**
     * From CAknSettingItem
     * This method obtains the current value's setting text
     *
     * @since S60 v3.2
     * @return Reference to descriptor with the text
     */
    virtual const TDesC& SettingTextL();

protected:
    
// From CAknSettingItem

    /**
     * From CAknSettingItem
     * Method to obtain a new bitmap for the setting content
     *
     * @since S60 v3.2
     * @return Pointer to create CFbsBitmap object
     */
    CFbsBitmap* CreateBitmapL(); 
 
private:

    /**
     * The pen input implementation (Not own)
     */
    CGSPenInputImple* iImple;

    };

/**
 *  CGSTextSettingItem item class
 *
 *  This class is for text item class. It offers input lauguage, writing speed, 
 *  trail width items in setting list.Form this class, user can 
 *  define item behavior and interface by themselves.
 *
 *  @lib gspeninputplugin.dll
 *  @since S60 v3.2
 */
class CGSTextSettingItem : public CAknTextSettingItem 
    {

public:

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @param aItemResourceId Resource id
     * @param aText Text display in item
     * @param aImple CGSPenInputImple instance
     * @return None
     */
    CGSTextSettingItem(TInt aItemResourceId, 
                          TDes &aText, 
                          CGSPenInputImple* aImple,
                          CGSSettingItemList* aParent);

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CGSTextSettingItem();

// From CAknTextSettingItem

    /**
     * From CAknTextSettingItem
     * This method obtains the current value's setting text
     *
     * @since S60 v3.2
     * @return Reference to descriptor with the text
     */
    const TDesC& SettingTextL();

    /**
     * From CAknTextSettingItem
     * This launches the setting page 
     *
     * @since S60 v3.2
     * @param aCalledFromMenu Ignored in this class
     * @return None
     */
    void EditItemL(TBool aCalledFromMenu); 

private:

    /**
     * The pen input implementation (Not own)
     */
    CGSPenInputImple* iImple;

    /**
     * Item resource id 
     */
    TInt iItemId;

    /**
     * Input language item text
     */
    TBuf<KItemTextSize> iInputLanguage;

    /**
     * Writing speed item text
     */
    TBuf<KItemTextSize> iWritingSpeed;

    /**
     * Guide Line item text
     */
    TBuf<KItemTextSize> iGuideLine;
    /**
     * Guide Line item text
     */
    TBuf<KItemTextSize> iRecognitionWithDictionary;    
    
    
    /**
     * Trail Width item text
     */
    TBuf<KItemTextSize> iTrailWidth;

    /**
     * Input method for find item text
     */
    TBuf<KItemTextSize> iInputMethodForFind;

    /**
     * Chinese Input Find Method for find item text
     */
    RBuf iInputChineseFindMethod;
    //TBuf<KItemTextSize> iInputChineseFindMethod;

    /**
     * Input Mode item text
     */
    TBuf<KItemTextSize> iInputMode;
    
    /**
     * Input Mode item text (Not own)
     */    
    CGSSettingItemList* iSettingList;
 
    };

/**
 *  CGSSettingItemList list class
 *
 *  This class is for list box class. It offers list box, which contains input language,
 *  writing speed, trail width, and trail colour items in setting list.Form 
 *  CreateSettingItemL function user can create custom item which from above 
 *  CGSTextSettingItem and CGSColorSettingItem. 
 *
 *  @lib gspeninputplugin.dll
 *  @since S60 v3.2
 */
class CGSSettingItemList : public CAknSettingItemList
    {

public:

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @param aImple CGSPenInputImple instance
     * @return None
     */
    CGSSettingItemList(CGSPenInputImple* aImple);

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CGSSettingItemList();
    
    /**
     * Reset Setting List
     *
     * @since S60 v3.2
     * @return None
     */
    void ResetSettingListL();
    
    /**
    * From MEikListBoxObserver.
    * Handles the commands coming from the listbox
    * @param aListBox List box that initiated the event
    * @param aEventType Type of the event         
    */
    void HandleListBoxEventL( CEikListBox* aListBox, TListBoxEvent aEventType );
    
protected:

// From CAknSettingItemList

    /**
     * From CAknTextSettingItem
     * Framework method to create a setting item based upon the user id aSettingId
     *
     * @since S60 v3.2
     * @param identifier ID to use to determine the type of the setting item
     * @return A constructed (not 2nd-stage constructed) setting item
     */
    CAknSettingItem* CreateSettingItemL(TInt identifier);

private:

    /**
     * The pen input implementation (Not own)
     */
    CGSPenInputImple* iImple;

    /**
     * Input language item text
     */
    TBuf<KItemTextSize> iInputLanguage;

    /**
     * Writing speed item text
     */
    TBuf<KItemTextSize> iWritingSpeed;

    /**
     * Guide Line item text
     */
    TBuf<KItemTextSize> iGuideLine;    
    /**
     * Guide Line item text
     */
    TBuf<KItemTextSize> iRecognitionWithDictionary;    
    
    /**
     * Trail Width item text
     */
    TBuf<KItemTextSize> iTrailWidth;
    
    /**
     * Input method for find item text
     */
    TBuf<KItemTextSize> iInputMethodForFind;

    /**
     * Chinese Input Find Method for find item text
     */
    RBuf iInputChineseFindMethod;
    //TBuf<KItemTextSize> iInputChineseFindMethod;

    /**
     * Input Mode item text
     */
    TBuf<KItemTextSize> iInputMode;

    /**
     * Current language is chinese
     */
    TBool iChinese;  
    
    /**
     * Current language is Japanese
     */
    TBool iJapanese;        
    
   };

/**
 *  CGSSettingDialog dialog class
 *
 *  This class is for dialog class containing above list box. It offers dialog, 
 *  which contains a list box with input language, writing speed, trail width, 
 *  and trail colour items in setting list.
 *
 *  @lib gspeninputplugin.dll
 *  @since S60 v3.2
 */
class CGSSettingDialog : public CAknDialog,
                            public MGsCenRepNotifyHandlerCallback
    {

public:

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @param aImple CGSPenInputImple instance
     * @return None
     */
    CGSSettingDialog(CGSPenInputImple* aImple);

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CGSSettingDialog();

// From MEikDialogPageObserver via CEikDialog

    /**
     * From MEikDialogPageObserver
     * Framework method to create a control based upon control type
     *
     * @since S60 v3.2
     * @param aControlType Control resource id
     * @return Custom control info
     */
    virtual SEikControlInfo CreateCustomControlL(TInt aControlType);

// From MGsCenRepNotifyHandlerCallback
    
    /**
     * From MGsCenRepNotifyHandlerCallback
     * This callback method is used to notify the client about
     * key changing
     * 
     * @since S60 v3.2
     * @param aId Key id 
     * @param aNewValue New value
     * @return None
     */
    void HandleNotifyInt(TUint32 aId, TInt aNewValue);    
    
    /**
     * This callback method is used to notify the client about
     * key changing
     * 
     * @param aId Key id 
     * @param aNewValue New value
     * @param aRepositoryUid The repository uid for each key
     * @return None
     */
    void HandleNotifyInt(TUid aRepositoryUid, TUint32 aId, TInt aNewValue); 

// From MEikMenuObserver
    
    /**
     * From MEikMenuObserver
     * 
     * @since S60 v3.2
     * @param aResourceId resource id 
     * @param aMenuPane 
     * @return None
     */
    virtual void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane *aMenuPane);
    
protected:

// From CAknDialog

    /**
     * From CAknDialog
     * Handles a dialog button press for the specified button. 
     *
     * This function is invoked when the user presses a button in the
     * button panel. It is not called if the Cancel button is activated 
     * unless the @c EEikDialogFlagNotifyEsc flag has been set.
     *
     * If there is an Options key then pops up menu, otherwise exits.
     *
     * @param aButtonId The ID of the button that was activated.
     * @return @c ETrue to validate and exit the dialog, 
     *         @c EFalse to keep the dialog active. If @c ETrue, the 
     *         dialog will be destroyed automatically by @c ExecuteLD(). 
     *         The default implementation returns @c ETrue no matter which
     *         button is pressed.
     */
	virtual TBool OkToExitL( TInt aButtonId );
	
private:

    /**
     * Show setting page when user select item in listbox
     * 
     * @since S60 v3.2
     * @return None
     */
    void ShowSettingPageL();	

    /**
     * From MEikCommandObserver
     * Process menu selection event
     *
     * @since S60 v3.2
     * @param aCommandId command.
     * @return None
     */
    void ProcessCommandL(TInt aCommandId);
    
    /**
     * Handle notifier from KCRUidPersonalizationSettings in CRepository
     *
     * @param aId Key id
     * @param aNewValue New value     
     */
    void HandleNotifyIntFromPersonalizationL( TUint32 aId, TInt aNewValue );
    
    /**
     * Handle notifier from KCRUidAknFep in CRepository
     *
     * @param aId Key id
     * @param aNewValue New value     
     */
    void HandleNotifyIntFromAknFepL( TUint32 aId, TInt aNewValue );
    
    /**
     * Handle notifier from KCRUidAvkon in CRepository
     *
     * @param aId Key id
     * @param aNewValue New value     
     */
    void HandleNotifyIntFromAvkonL( TUint32 aId, TInt aNewValue );    
    
    /**
     * Launch other application
     *
     * @aAppUid: The application's uid     
     */    
    void LaunchAppL( TUid aAppUid );
     
private:

    /**
     * The pen input implementation (Not own)
     */
    CGSPenInputImple* iImple;

    /**
     * The setting list (Not own)
     */
    CGSSettingItemList* iSettingList;
    
    /**
     * Launch application;
     */    
    CAknNullService* iNullService;    
    };

#endif // C_GSSETTINGDIALOG_H

// End Of File
