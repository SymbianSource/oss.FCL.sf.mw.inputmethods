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
* Description:  gspeninputimple header file.
*
*/


#ifndef C_GSPENINPUTIMPLE_H
#define C_GSPENINPUTIMPLE_H

// INCLUDES
#include <ConeResLoader.h>

#include "peninputgsinterface.h"
#include "gscenrepnotifyhandlercallback.h"

// FORWARD DECLARATION
class CGSPenInputModel;
class CGSSettingDialog;
class CAknNullService;

/**
 *  gspeninputimple interface class
 *
 *  This class inherit from CGSPenInputInterface and implement all 
 *  its pure virtual function. 
 *
 *  @lib gspeninputplugin.dll
 *  @since S60 v3.2
 */
class CGSPenInputImple : public CPenInputGSInterface, 
                         public MGsCenRepNotifyHandlerCallback 
    {

public: 

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @return Pointer to created CGSPenInputImple object
     */
    static CGSPenInputImple* NewL();

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CGSPenInputImple();
        
    /**
     * Get model instance
     *
     * @since S60 v3.2
     * @return None
     */ 
    CGSPenInputModel* Model();

    /**
     * Current writing speed name 
     *
     * @since S60 v3.2
     * @param aWritingSpeed Reference to current input language name
     * @return None
     */
    void WritingSpeed(TDes& aWritingSpeed);
    
    /**
     * Current guide line name 
     *
     * @since S60 v3.2
     * @param aGuideLine Reference to current guide line name
     * @return None
     */
    void GuideLine(TDes& aGuideLine);    
    
    /**
     * Current guide line name 
     *
     * @since S60 v3.2
     * @param aGuideLine Reference to current guide line name
     * @return None
     */
    void RecognitionWithDictionary(TDes& aRecognitionWithDictionary);        
    
    /**
     * Current trail width name 
     *
     * @since S60 v3.2
     * @param aWritingSpeed Reference to current input language name
     * @return None
     */
    void TrailWidth(TDes& aTrailWidth);

    /**
     * Current input mode name 
     *
     * @since S60 v3.2
     * @param aInputMode Reference to current input mode name
     * @return None
     */
    void InputModeL(TDes& aInputMode);
    
    /**
     * Current input method for find name 
     *
     * @since S60 v5.0
     * @param aInputMethodForFind Reference to current input method for find name
     * @return None
     */
    void InputMethodForFind(TDes& aInputMethodForFind);
    
    /**
     * Display "Pen input language" setting page
     *
     * @since S60 v3.2
     * @return ETrue for setting change; otherwise EFalse
     */
    TBool ShowInputLanguageSettingPageL();    
    
    /**
     * Current chinese find method item text
     *     
     * @return aChineseFindMethod, chinese find method resource item text     
     */     
    void GetCurrentChineseFineMethodItem( TDes& aChineseFindMethod );

    /**
     * Load correct items for chinese find methods
     * by different chinese languages
     * 
     * @return None
     */     
     void LoadChineseFindMethodItemsL();

    /**
     * Load correct items for default on screen vbk items
     * 
     * @return None
     */     
     void LoadDefaultOnScreenVkbItemsL();
    
// From CGSPenInputInterface

    /**
     * From CGSPenInputInterface
     * Dispaly input pen settings main view
     *
     * @since S60 v3.2
     * @return None.
     */
    void ShowMainViewL();

    /**
     * From CGSPenInputInterface
     * Display pop up list setting page
     *
     * @since S60 v3.2
     * @return ETrue for setting change; otherwise EFalse
     */
    TBool ShowInputLanguagePageL();

    /**
     * From CGSPenInputInterface
     * Display "Input Mode" setting page
     *
     * @since S60 v3.2
     * @return ETrue for setting change; otherwise EFalse
     */
    TBool ShowInputModePageL();

    /**
     * From CGSPenInputInterface
     * Display "Writing speed" setting page
     *
     * @since S60 v3.2
     * @return ETrue for setting change; otherwise EFalse
     */
    TBool ShowWritingSpeedPageL();
    
    /**
     * From CGSPenInputInterface
     * Display "Guide Line" setting page
     *
     * @since S60 v3.2
     * @return ETrue for setting change; otherwise EFalse
     */
    TBool ShowGuideLinePageL();      
    TBool ShowRecognitionWithDictionaryL();
    

    /**
     * From CGSPenInputInterface
     * Display "Pen trail width" setting page
     *
     * @since S60 v3.2
     * @return ETrue for setting change; otherwise EFalse
     */
    TBool ShowTrailWidthPageL();

    /**
     * From CGSPenInputInterface
     * Display "Pen trail colour" setting page
     *
     * @since S60 v3.2
     * @return ETrue for setting change; otherwise EFalse
     */ 
    TBool ShowTrailColourPageL();

    /**
     * From CGSPenInputInterface
     * Display "Input method for find" setting page
     *
     * @since S60 v5.0
     * @return ETrue for setting change; otherwise EFalse
     */
    TBool ShowInputMethodForFindPageL();  

    /**
     * From CGSPenInputInterface
     * Display "Chinese Find Method" setting page
     *
     * @since S60 v5.0
     * @return ETrue for setting change; otherwise EFalse
     */
    TBool ShowChineseFindMethodPageL();
        
    /**
     * From CGSPenInputInterface
     * Display "Touch screen calibration" setting page
     *
     * @since S60 v3.2
     * @return None
     */ 
    void StartTouchScreenCalibL();

// From MGsCenRepNotifyHandlerCallback
    void HandleNotifyInt(TUint32 aId, TInt aNewValue );

    /**
     * This callback method is used to notify the client about
     * key changing
     * 
     * @param aRepositoryUid The repository uid for each key
     * @param aId Key id 
     * @param aNewValue New value
     * @return None
     */
    void HandleNotifyInt(TUid aRepositoryUid, TUint32 aId, TInt aNewValue);
    
    /**
     * Restore caller's status pane visibility.
     */
    void RestoreStatusPaneVisibility();
    
    
protected:

    /**
     * C++ default constructor
     *
     * @since S60 v3.2
     * @return None
     */  
    CGSPenInputImple();

private:

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v3.2
     * @return None
     */          
     void ConstructL();

    /**
     * Opens the nearest localized resourcefile using aResourceLoader
     *
     * @since S60 v3.2
     * @param aResourceFileName Drive and name of resource file in format
     *                          <path>:<rsc_file_name>
     * @return None
     */
     void OpenResourceFileL(const TDesC& aResourceFileName);
   
    /**
     * Load language list popup query dialog icons from resource
     *
     * @since S60 v3.2
     * @param aIcons Pointer to display icone
     * @param aBitmapId Bitmap resource id
     * @param aMaskId Mask resource id
     * @return None
     */
     void LoadIconL(CArrayPtr<CGulIcon>* aIcons, TInt aBitmapId, TInt aMaskId);    

     void PrepareScreenFurnitureL( TBool& aToolbarShow );

     void RestoreScreenFurnitureL( TBool aShowToolbar );
   
private:
    
     /**
     * The model (Own)
     */
    CGSPenInputModel* iModel;
    
    CGSSettingDialog* iDialog;
    
    /**
    * Resource loader
    */
    RConeResourceLoader iResourceLoader;

    /**
     * The string of writing speed items (Own)
     */
    CDesCArrayFlat* iWritingSpeedItems;

    /**
     * The string of guid line items (Own)
     */
    CDesCArrayFlat* iGuideLineItems;
    /**
     * The string of guid line items (Own)
     */
    CDesCArrayFlat* iRecognitionWithDictionary;    

    /**
     * The string of pen trail width items (Own)
     */
    CDesCArrayFlat* iPenTrailWidthItems;
    
    /**
     * The string array of input mode items (Own)
     */
    CDesCArrayFlat* iInputModeItems;    
    
    /**
     * The string of input method for find items (Own)
     */
    CDesCArrayFlat* iInputMethodForFindItems;

    /**
     * The string of input method for chinese find method items (Own)
     */
    CDesCArrayFlat* iChineseFindMethodItems;
    
    /**
     * Variable to store the orign status pane visible
     */
	TBool isSpVisibleBak;

    /**
     * Variable to store the orign title pane text
     */
	HBufC* iTitleTextBak;
	
    /**
     * Launch application;
     */    
    CAknNullService* iNullService;
    };

#endif // C_GSPENINPUTIMPLE_H

// End Of File
