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
* Description:  Container for gspeninputplugin.
*
*/


#ifndef C_GSPENINPUTCONTAINER_H
#define C_GSPENINPUTCONTAINER_H

// INCLUDES
#include <gsbasecontainer.h>
#include <gscommon.hrh>

#include "gscenrepnotifyhandlercallback.h"

// FORWARD DECLARATION
class CGSListBoxItemTextArray;
class CGSPenInputModel;
class CGulIcon;
class CAknNullService;

/**
 *  GSPenInputPlugin container class
 *
 *  This class is GSPenInputPlugin container class. It offers not only CCoeControl
 *  but also CGSBaseContainer behavior. General settings framework will use the class
 *  to handle list box.
 *
 *  @lib gspeninputplugin.dll
 *  @since S60 v3.2
 */
class CGSPenInputContainer : public CGSBaseContainer, 
                                public MGsCenRepNotifyHandlerCallback 
    {
    
public: 

    /**
     * C++ default constructor
     *
     * @since S60 v3.2
     * @return None
     */  
    CGSPenInputContainer();

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v3.2
     * @param aRect Listbox's rect.
     * @return None
     */          
    void ConstructL(const TRect& aRect);

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CGSPenInputContainer();

    /**
     * Updates listbox item's value.
     *
     * @since S60 v3.2
     * @param aItemId Item id which is updated.
     * @return None
     */
    void UpdateListBoxL(TInt aItemId);

    /**
     * Retrieves the currently selected listbox id
     *
     * @since S60 v3.2
     * @return Selected list box item id.
     */
    TInt CurrentSelectedId() const;

    /**
     * Display input language setting page
     *
     * @since S60 v3.2
     * @return None
     */
    void ShowInputLanguagePageL();

    /**
     * Display input mode setting page
     *
     * @since S60 v3.2
     * @return None
     */
    void ShowInputModePageL();

    /**
     * Display writing speed setting page
     *
     * @since S60 v3.2
     * @return None
     */
    void ShowWritingSpeedPageL();

    /**
     * Display guide line setting page
     *
     * @since S60 v3.2
     * @return None
     */
    void ShowGuideLinePageL();
    void ShowRecognitionWithDictionaryL();

    /**
     * Display pen trail width setting page
     *
     * @since S60 v3.2
     * @return None
     */
    void ShowTrailWidthPageL();
        
    /**
     * Display pen trail colour setting page
     *
     * @since S60 v3.2
     * @return None
     */
    void ShowTrailColourPageL();
    
    /**
     * Display "Touch screen calibration" setting page
     *
     * @since S60 v3.2
     * @return None
     */ 
    void StartTouchScreenCalibL();

    /**
     * Display "Input method for find" setting page
     *
     * @since S60 v5.0
     * @return None
     */ 
    void ShowInputMethodForFindPageL();
    
    /**
     * Display "Chinese input method" setting page
     *
     * @since S60 v5.0
     * @return None
     */ 
    void ShowChineseFindMethodPageL();
    
    /**
     * Get model instance
     *
     * @since S60 v3.2
     * @return None
     */ 
    CGSPenInputModel* Model();
    
    /**
     * Re-Construct List box
     *
     * @since S60 v3.2
     * @return None 
     */
    void ReConstructListBoxL();
    
    /**
     * Launch other application
     *
     * @aAppUid: The application's uid     
     */
    void LaunchAppL( TUid aAppUid );

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
    void MakeRecognitionWithDictionaryL();
    
    /**
     * From MGsCenRepNotifyHandlerCallback
     * This callback method is used to notify the client about
     * key changing
     * 
     * @param aRepositoryUid The repository uid for each key
     * @param aId Key id 
     * @param aNewValue New value
     * @return None
     */
    void HandleNotifyInt(TUid aRepositoryUid, TUint32 aId, TInt aNewValue);

protected:  
        
    /**
     * From CGSBaseContainer
     * Constructs listbox 
     *
     * @since S60 v3.2
     * @param aResLbxId Listbox source id
     * @return None
     */  
    void ConstructListBoxL(TInt aResLbxId);

private: 

    /**
     * Constructs listbox items
     *
     * @since S60 v3.2
     * @return None
     */  
    void CreateListBoxItemsL();

    /**
     * Create listbox hwr trui item 
     *
     * @since S60 v3.2
     * @return None
     */  
    void MakeHwrTruiItemL();
    
    /**
     * Create listbox input language item 
     *
     * @since S60 v3.2
     * @return None
     */  
    void MakeInputLanguageL();

    /**
     * Create  input mode item 
     *
     * @since S60 v3.2
     * @return None
     */  

    void MakeInputModeL();

    /**
     * Create listbox guide line item 
     *
     * @since S60 v3.2
     * @return None
     */  
    void MakeGuideItemL();
    
    /**
     * Create listbox writing speed item 
     *
     * @since S60 v3.2
     * @return None
     */  
    void MakeSpeedItemL();

    /**
     * Create listbox pen trail width item
     *
     * @since S60 v3.2
     * @return None
     */  
    void MakeTrailWidthItemL();

    /**
     * Create listbox pen trail colour item
     *
     * @since S60 v3.2
     * @return None
     */  
    void MakeTrailColourItemL();
    
    /**
     * Create listbox touch screen calibration item
     *
     * @since S60 v3.2
     * @return None
     */  
    void MakeTouchScrCalibItemL();
    
    /**
     * Create listbox input method for find item
     *
     * @since S60 v3.2
     * @return None
     */  
    void MakeInputMethodForFindItemL();
    
    /**
     * Create listbox chinese find method item
     * Uses appropriate method to create listbox item.
     *
     * @since S60 v5.0
     * @return None
     */  
    void MakeChineseFindMethodItemL();
    
    /**
     * Display "Chinese hongkong input method" setting page
     *
     * @since S60 v5.0
     * @return None
     */ 
    void MakeChineseFindMethodHongkongItemL();
    
    /**
     * Display "Chinese hongkong input method" setting page
     *
     * @since S60 v5.0
     * @return None
     */ 
    void ShowChineseFindMethodHongkongPageL();
    
    /**
     * Create a new icon
     *
     * @since S60 v3.2
     * @return Pointer to create new icom
     */  
    CGulIcon* CreateNewIconL();
    
    /**
     * Draw icon bitmap
     *
     * @since S60 v3.2
     * @param aRect Reference to main rect
     * @param aIcon Pointer to icon
     * @return None
     */  
    void DrawIconL(const TRect& aRect, CGulIcon* aIcon);
    
    /**
     * Create a icon for pen trail colour item 
     *
     * @since S60 v3.2
     * @return None
     */  
    void CreateIconL();  
    /**
     *  From CCoeControl
     *  Identify the help context so that the framework can look up
     *  the corresponding help topic
     *  @param aContext Returns the help context
     */    
    void GetHelpContext( TCoeHelpContext& aContext ) const;
    
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

private:// data
    
    /**
     * The icon array of list box item (Not own)
     */
    CArrayPtr<CGulIcon>* iIconArray;

    /**
     * The model (Own)
     */
    CGSPenInputModel* iModel;

    /**
     * The string array of list box item (Own)
     */
    CGSListBoxItemTextArray* iListboxItemArray;

    /**
     * The string array of writing speed items (Own)
     */
    CDesCArrayFlat* iWritingSpeedItems;

    /**
     * The string array of guide line items (Own)
     */
    CDesCArrayFlat* iGuideLineItems;
    /**
     * The string array of guide line items (Own)
     */
    CDesCArrayFlat* iRecognitionWithDictionaryItems;    

    /**
     * The string array of pen trail width items (Own)
     */
    CDesCArrayFlat* iPenTrailWidthItems;
    
    /**
     * The string array of input mode items (Own)
     */
    CDesCArrayFlat* iInputModeItems; 

    /**
     * The string array of method for find items (Own)
     */
    CDesCArrayFlat* iInputMethodForFindItems;

    /**
     * Resource id of chinese find method item array
     */
    TInt iChineseFindMethodResourceID;
    
    /**
     * The string array of chinese find method items (Own)
     */
    CDesCArrayFlat* iChineseFindMethodItems;
    
    /**
     * Current language is Chinese
     */
    TBool iChinese;
    
    /**
     * Current language is Japanese
     */
    TBool iJapanese;    
    
    /**
     * Launch application;
     */    
    CAknNullService* iNullService;
    };

#endif // C_GSPENINPUTCONTAINER_H

// End Of File
