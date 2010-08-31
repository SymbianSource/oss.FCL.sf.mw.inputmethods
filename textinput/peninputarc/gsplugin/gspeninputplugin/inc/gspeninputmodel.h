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
* Description:  Model for gspeninputplugin.
*
*/


#ifndef  C_GSPENINPUTMODEL_H
#define  C_GSPENINPUTMODEL_H

// INCLUDES
#include <backgroundimage.h>
#include <badesca.h>
#include <bldvariant.hrh>
#include <centralrepository.h>
#include <e32base.h>
#include <generalsettingsvariant.hrh>
#include <e32property.h>
#include <cenrepnotifyhandler.h>
#include <peninputclient.h>

#include "gspeninputplugin.hrh"

_LIT(HLP_GS_PEN_INPUT, "CP_HLP_PEN_INPUT"); // temp fix for helps integration build break replace KCP_HLP_PEN_INPUT in cp.hlp.hrh;

// FORWARD DECLARATION
class CGSPenInputContainer;
class CPtiEngine;
class MGsCenRepNotifyHandlerCallback;

// CONSTANTS
const TInt KGSBufSize = 128;

/**
 *  GSPenInputPlugin Repository Watcher class
 *
 *  This class provides a callback function for watchering repository key. 
 *
 *  @lib gspeninputplugin.dll
 *  @since S60 v3.2
 */
class CGSRepositoryWatcher : public CBase, public MCenRepNotifyHandlerCallback
    {
    
public:

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @param aCallBack Call back function 
     * @param aRepository Repository for watchering
     * @param aRepositoryUid The repository for each key
     * @return Pointer to created CGSRepositoryWatcher object
     */
    static CGSRepositoryWatcher* NewL(MGsCenRepNotifyHandlerCallback* aOwner,
                                         CRepository* aRepository,
                                         TUid aRepositoryUid );

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CGSRepositoryWatcher();

// From MCenRepNotifyHandlerCallback
 
    /**
     * From MCenRepNotifyHandlerCallback
     * Notify call back function
     *
     * @since S60 v3.2
     * @param aKey Repository UID  
     * @param aNewValue Changed value
     * @return None
     */
    void HandleNotifyInt(TUint32 aKey, TInt aNewValue);

    void HandleNotifyGeneric(TUint32 aId);
    
private:
    
    /**
     * C++ default constructor
     *
     * @since S60 v3.2
     * @param aCallBack Call back function 
     * @param aRepository Repository for watchering
     * @param aRepositoryUid The reposotory uid for each key
     * @return None
     */          
    CGSRepositoryWatcher(MGsCenRepNotifyHandlerCallback* aOwner,
                            CRepository* aRepository,
                            TUid aRepositoryUid );

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v3.2
     * @return None
     */          
    void ConstructL();

private:

    /**
     * Owner pointer for call back 
     */
    MGsCenRepNotifyHandlerCallback* iOwner;

    /**
     * The Repository for watchering (Not own)
     */
    CRepository* iRepository;

    /**
     * The handle of Repository notify (Own)
     */
    CCenRepNotifyHandler* iNotifyHandler;
    
    /**
     * The reposotory uid
     */
    TUid iRepositoryUid;
    
    };
    
/**
 *  GSPenInputPlugin model class
 *
 *  This class is GSPenInputPlugin model class. It handle GSPenInputPlugin
 *  data and store value to Repository.
 *
 *  @lib gspeninputplugin.dll
 *  @since S60 v3.2
 */
class CGSPenInputModel : public CBase
    {
   
public:

    /**  DPI type for pen trail width */
    enum TDPIType 
        {
        EDPILow = 150, // low DPI    
        EDPIMiddle = 200, // middle DPI
        EDPIHigh = 250 // high DPI       
        };
    
    /**  List box item array length */        
    enum TPenInputArray
        {
        EWritingSpeedArrayLength = 5, 
        ETrailWidthArrayLength = 3, 
        };

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @param aOwner Pointer to owner 
     * @return Pointer to created CGSPenInputModel object
     */
    static CGSPenInputModel* NewL(MGsCenRepNotifyHandlerCallback* aOwner);

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @return Pointer to created CGSPenInputModel object
     */
    static CGSPenInputModel* NewL();

    /**
     * Set owner for call back
     *
     * @since S60 v3.2
     * @param aOwner Pointer to owner  
     * @return None
     */
    inline void SetOwner(MGsCenRepNotifyHandlerCallback* aOwner);

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CGSPenInputModel();

    /**
     * Get current input language index in list box item
     *
     * @since S60 v3.2
     * @return The index of input language setting page 
     */
    inline TInt InputLanguageIndex() const;

    /**
     * Set current input language index in list box item
     *
     * @since S60 v3.2
     * @param aIndex Language index in input language setting page
     * @return None 
     */
    inline void SetInputLanguageIndex(TInt aIndex);

    /**
     * Current input language name in list box item
     *
     * @since S60 v3.2
     * @param aLanguage Reference to current input language name
     * @return None
     */
    void InputLanguage(TDes& aLanguage);

    /**
     * Current input language ID
     *
     * @since S60 v3.2
     * @return Language ID
     */
    TInt InputLanguage(); 
    
    /**
     * Get languages from ptiengine    
     *
     * @since S60 v3.2
     * @param aLanguageNames Poniter to language name array from ptiengine
     * @param aISOCode ETrue language name with ISO code.
     * @return None
     */
    void GetLanguagesFromPtiL(TBool aISOCode,CDesCArray* aLanguageNames);
    
    /**
     * Get Popup Query languages from ptiengine    
     *
     * @since S60 v3.2
     * @param aLanguageNames Poniter to language name array from ptiengine
     * @return None
     */
    void GetPopupQuerylanguagesFromPtiL(CDesCArray* aLanguageNames);    

    /**
     * Store input language to Repository
     *
     * @since S60 v3.2
     * @return None 
     */
    void SetInputLanguageL();
    
    /**
     * Current writing speed index in list box item
     *
     * @since S60 v3.2
     * @return None 
     */
    TInt WritingSpeed();

    /**
     * Store writing speed to Repository
     *
     * @since S60 v3.2
     * @param aSpeed Index in list box item     
     * @return None 
     */
    void SetWritingSpeed(const TInt aSpeed);

    /**
     * Store guid line to Repository
     *
     * @since S60 v3.2
     * @return ETrue for guid line on; otherwise EFalse
     */
    void SetGuideLine(TInt aGuideLine);
    void SetRecognitionWithDictionary(TInt aRecognitionWithDictionary);

    /**
     * Get current guide line from Repository
     *
     * @since S60 v3.2
     * @return ETrue for guid line on; otherwise EFalse
     */
    TInt GuideLine();
    
    TInt RecognitionWithDictionary();

    /**
     * Store input method for find to Repository
     *
     * @since S60 v5.0
     */
    void SetInputMethodForFind(TInt aInputMethodForFind);

    /**
     * Get current input method for find from Repository
     *
     * @since S60 v5.0
     */
    TInt InputMethodForFind();

    /**
     * Get chinese find method resource id
     *
     * @since S60 v5.0
     */
    TInt ChineseFindMethodResourceID();

    /**
     * Store chinese find method to Repository
     *
     * @since S60 v5.0
     */
    void SetChineseFindMethod(TInt aChineseFindMethod);

    /**
     * Get chinese find method from Repository
     *
     * @since S60 v5.0
     */
    TInt ChineseFindMethod();

    /**
     * Current pen trail width index in list box item
     *
     * @since S60 v3.2
     * @return The index of writing speed item 
     */
    TInt TrailWidth();

    /**
     * Store pen trail width to Repository
     *
     * @since S60 v3.2
     * @param aWidth Index in list box item     
     * @return None 
     */
    void SetTrailWidth(const TInt aWidth);
        
    /**
     * Current pen trail colour index in list box item
     *
     * @since S60 v3.2
     * @return The index of trail colour item 
     */
    TInt TrailColour();

    /**
     * Store pen trail colour to Repository
     *
     * @since S60 v3.2
     * @param aColour Index in list box item     
     * @return None 
     */
    void SetTrailColour(const TInt aColour);

    /**
     * Set input mode.
     *
     * @since S60 v3.2
     * @param aIndex Item idex of setting page.
     * @return None
     */
    void SetInputModeBySettingPage(const TInt aIndex);    
    
    /**
     * Get input mode index in setting page
     *
     * @since S60 v3.2
     * @return None
     */
    TInt InputModeInSettingPage();

    /**
     * Get input mode resource id
     *
     * @since S60 v3.2
     * @return None
     */
    TInt InputModeResourceID();

    /**
     * Current language is chinese language
     *
     * @since S60 v3.2
     * @return ETrue for chinese language; otherwise EFalse
     */
    inline TBool ChineseLanguage();
    
    /**
     * Current language is chinese and Japanese language
     *
     * @since S60 v3.2
     * @return ETrue for chinese language; otherwise EFalse
     */
    inline TBool JapaneseLanguage();
    
    /**
     * Reset input mode destription array
     *
     * @since S60 v3.2
     * @param aIndex Item idex of setting page.  
     * @return None
     */
    void ResetInputModeArray(CDesCArrayFlat* aInputModeItems);
    
    /**
     * Reset input mode to default input mode when language changed 
     *
     * @since S60 v3.2
     * @return None
     */
    void ResetDefaultInputMode();
    
    /**
     * Set FEP default input mode when language changed 
     *
     * @since S60 v3.2
     * @param aInputMode Default input mode.     
     * @return None
     */
    void SetFepInputMode(TInt aInputMode);
        
    /**
     * Get FEP default input mode when language changed 
     *
     * @since S60 v3.2
     * @return Default input mode
     */
    TInt GetFepInputMode();
    
    /**
     * Get input mode from repository
     *
     * @since S60 v3.2
     * @return Current input mode
     */          
    TInt InputMode();    
    
    /**
     * Reset Fep input mode based on current input language
     *
     * @since S60 v3.2
     * @return None
     */          
    void ResetFepDefaultInputMode();
    
    TBool IsSettingItemAvailable( TInt aItemId );
    
private: 

    /**
     * C++ default constructor
     *
     * @since S60 v3.2
     * @param aOwner Pointer to owner       
     * @return None
     */  
    CGSPenInputModel(MGsCenRepNotifyHandlerCallback* aOwner);
    
    /**
     * C++ default constructor
     *
     * @since S60 v3.2
     * @return None
     */  
    CGSPenInputModel();

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v3.2
     * @return None
     */          
    void ConstructL();

    /**
     * Initializes Repository
     *
     * @since S60 v3.2
     * @return None
     */          
    void InitializeCentralRepositoriesL();

    /**
     * Initializes pen trail width array
     *
     * @since S60 v3.2
     * @return None
     */          
    void InitTrailWidthArrayL();

    /**
     * Initializes writing speed array
     *
     * @since S60 v3.2
     * @return None
     */          
    void InitWritingSpeedArray();

    /**
     * Initializes low dpi array
     *
     * @since S60 v3.2
     * @return None
     */          
    void InitLowDPI();

    /**
     * Initializes middle dpi array
     *
     * @since S60 v3.2
     * @return None
     */          
    void InitMiddleDPI();

    /**
     * Initializes high dpi array
     *
     * @since S60 v3.2
     * @return None
     */          
    void InitHighDPI();
    
    /**
     * Set input mode to repository
     *
     * @since S60 v3.2
     * @param aInputMode Input Mode
     * @return None
     */          
    void SetInputMode(TInt aInputMode);
    
    /**
     * The trail color got from key is available
     *
     * @since S60 v3.2
     * @return ETrue for Available; otherwise EFalse
     */
     inline TBool AvailableTrailColor(TRgb color);         
    
    /**
     * Adjust language name
     *
     * @since S60 v3.2
     * @param aLangId Language Id
     * @param aLangName Input/output of language name
     * @return None
     */          
    void AdjustLanguageName(TInt aLangId, TDes& aLangName);

    /**
     * Get chinese find method cenrep key id
     *
     * @since S60 v5.0
     */
    TUint ChineseFindMethodCRKey();

    /**
     * Return current application's uid
     *
     * @param aWndGrpId Current application's window group id
     * @since S60 v5.0
     */
    TUid AppUidFromWndGroupId( TInt aWndGrpId );
    
    TUid AppUidFromWndGroupIdL( TInt aWndGrpId );
    TBool IsTruiSupportedLanguage();

private: 

    /**
     * Owner pointer for call back 
     */
    MGsCenRepNotifyHandlerCallback* iOwner;

    /**
     * The personalization repository (Own)
     */
    CRepository* iPersonalizationRepository;

    /**
     * The fep repository (Own)
     */
    CRepository* iAknfepRepository;

    /**
     * The avkon repository (Own)
     */
    CRepository* iAknRepository;

    /**
     * The input language index in setting page 
     */
    TInt iLanguageIndex;

    /**
     * The input language for storing in repository 
     */
    TInt iInputLanguage;

    /**
     * The writhing speed for storing in repository  
     */
    TInt iSpeed;

    /**
     * The pen input width for storing in repository 
     */
    TInt iWidth;

    /**
     * The pen color for storing in repository 
     */
    TInt iColor;
    
    /**
     * The input method for find for storing in repository 
     */
    TInt iInputMethodForFind;    

    /**
     * The input method for find for storing in repository 
     */
    TInt iChineseFindMethod;

    /**
     * The last used chinese find method item,
     * -1 == not queried from repo
     * 0 == not used
     * >0 == cenrep key id
     */
    TInt iLastChineseFindMethodItem;

    /**
     * The guid line for storing in repository 
     */
    TInt iGuideLine;    
    /**
     * Indicate HwrTraining is aviliable or not
     */
    TInt iIsRecognitionWithDictionary;      
    
    /**
     * The default input mode 
     */
    TInt iInputMode;    

    /**
     * The icon array of list box item 
     */
    TInt iTrailWidthValue[ETrailWidthArrayLength];

    /**
     * The icon array of list box item 
     */
    TInt iWritingSpeedValue[EWritingSpeedArrayLength];

    /**
     * The personalization watcher (Own)
     */
    CGSRepositoryWatcher* iPersonalizationWatcher;    


    /**
     * The Aknfep watcher (Own)
     */
    CGSRepositoryWatcher* iAknfepWatcher;    

    /**
     * The Akn watcher (Own)
     */
    CGSRepositoryWatcher* iAknWatcher;
    
    /**
    * Pti Engine (Own)
    */
    CPtiEngine* iPtiEngine;
    
    /**
     * Indicate HwrTraining is aviliable or not
     */
    TBool iIsHwrTrainingAviliable;
    
    /**
     * PeninputServer Client
     */
    RPeninputServer iPenInputServer;
    
    /**
     * PeninputServer connect error
     */
    TInt iConnectErr;
    
    };
    
#include "gspeninputmodel.inl"    

#endif // C_GSPENINPUTMODEL_H

// End Of File
