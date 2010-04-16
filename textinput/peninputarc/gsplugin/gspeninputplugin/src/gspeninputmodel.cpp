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
* Description:  Data model for GSPenInputPlugin.
*
*/


#include <gspeninputpluginrsc.rsg>
#include <featmgr.h>
#include <hal.h>
#include <PtiEngine.h>
#include <AknFepGlobalEnums.h>
#include <PtiDefs.h>
#include <settingsinternalcrkeys.h>
#include <peninputpluginutils.h>
#include <AknFepInternalCRKeys.h>
#include <AvkonInternalCRKeys.h>
#include <aknfeppeninputenums.h>
#include <apgwgnam.h>
#include <eikapp.h>
#include <eikenv.h>
#include <eikappui.h>
#include <featmgr.h>
#include <peninputsettingappui.h>

#include "gspeninputmodel.h"
#include "gspeninputcontainer.h"
#include "truisupportedlanguageid.h"
#include "peninputpluginutils.h"

// CONSTANTS 
const TInt KTwipPerInch = 1440 * 1440;
const TInt KMidLeftBounday = 165;
const TInt KHigLeftBounday = 225;
const TInt KGSLanguageMask = 0x03FF;
const TInt KISOCodeLength = 2;
const TUid KUidtruiApp = { 0x2000B104 };
const TUid KUidPenInputSettingApp = { 0x2001959B };

_LIT(KLeftBracket, "(" );
_LIT(KRightBracket, ")" );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CGSRepositoryWatcher* CGSRepositoryWatcher::NewL(MGsCenRepNotifyHandlerCallback* aOwner,
                                                 CRepository* aRepository,
                                                 TUid aRepositoryUid )
    {
    CGSRepositoryWatcher* self = new (ELeave) CGSRepositoryWatcher(aOwner, aRepository, aRepositoryUid);

    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
CGSRepositoryWatcher::~CGSRepositoryWatcher()
    {
    if(iNotifyHandler)
        {
        iNotifyHandler->StopListening();
        delete iNotifyHandler;
        }
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CGSRepositoryWatcher::CGSRepositoryWatcher(MGsCenRepNotifyHandlerCallback* aOwner,
                                           CRepository* aRepository,
                                           TUid aRepositoryUid)
    :iOwner(aOwner), iRepository(aRepository), iRepositoryUid(aRepositoryUid)
    {
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
void CGSRepositoryWatcher::ConstructL()
    {
    iNotifyHandler = CCenRepNotifyHandler::NewL(*this, *iRepository);
    iNotifyHandler->StartListeningL();
    }

// ---------------------------------------------------------
// Notify call back function
// ---------------------------------------------------------
//
void CGSRepositoryWatcher::HandleNotifyInt(TUint32 aKey, TInt aNewValue)
    {
    if (iOwner)
        {
        iOwner->HandleNotifyInt(iRepositoryUid, aKey, aNewValue);
        }
    }

// ---------------------------------------------------------
// Notify call back function
// ---------------------------------------------------------
//
void CGSRepositoryWatcher::HandleNotifyGeneric(TUint32 aId)
    {
    if (iOwner)
        {
        iOwner->HandleNotifyInt(iRepositoryUid, aId, 0);
        } 
    }
        
// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CGSPenInputModel* CGSPenInputModel::NewL(MGsCenRepNotifyHandlerCallback* aOwner)
    {
    CGSPenInputModel* self = new (ELeave) CGSPenInputModel(aOwner);

    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CGSPenInputModel* CGSPenInputModel::NewL()
    {
    CGSPenInputModel* self = new (ELeave) CGSPenInputModel();

    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CGSPenInputModel::CGSPenInputModel(MGsCenRepNotifyHandlerCallback* aOwner)
    : iOwner(aOwner), iInputLanguage(ELangTest), iSpeed(0), 
      iWidth(0), iColor(0), 
      iLastChineseFindMethodItem(-1), 
      iGuideLine(ETrue), iIsRecognitionWithDictionary(ETrue), iInputMode(ELatin)
   {
   }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CGSPenInputModel::CGSPenInputModel()
    : iOwner(NULL), iInputLanguage(ELangTest), iSpeed(0), iWidth(0),  
      iColor(0),  
      iLastChineseFindMethodItem(-1), 
      iGuideLine(ETrue), iIsRecognitionWithDictionary(ETrue),iInputMode(ELatin)
   {
   }
// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
void CGSPenInputModel::ConstructL()
    {
    InitTrailWidthArrayL();    
    InitWritingSpeedArray();
    FeatureManager::InitializeLibL();
    iPtiEngine = CPtiEngine::NewL();
    InitializeCentralRepositoriesL();
    iConnectErr = iPenInputServer.Connect();
    }

// ---------------------------------------------------------
// Initializes writing speed array
// ---------------------------------------------------------
//
void CGSPenInputModel::InitWritingSpeedArray()
    {
    iWritingSpeedValue[0] = EPenInputWritingSpeedVeryFast;
    iWritingSpeedValue[1] = EPenInputWritingSpeedFast;
    iWritingSpeedValue[2] = EPenInputWritingSpeedNormal;
    iWritingSpeedValue[3] = EPenInputWritingSpeedSlow;
    iWritingSpeedValue[4] = EPenInputWritingSpeedVerySlow;
    }
 
// ---------------------------------------------------------
// Initializes pen trail width array
// ---------------------------------------------------------
//
void CGSPenInputModel::InitTrailWidthArrayL()
    {
    TInt twipX;
    TInt twipY;
    TInt pixelX;
    TInt pixelY;
    
    HAL::Get(HALData::EDisplayXTwips, twipX);
    HAL::Get(HALData::EDisplayYTwips, twipY);
    HAL::Get(HALData::EDisplayXPixels, pixelX);
    HAL::Get(HALData::EDisplayYPixels, pixelY);
    
    if (twipX * twipY <= 0 || pixelX * pixelY <= 0)
        {
        User::Leave(KErrNotSupported);  
        }
    
    TInt dpi = (pixelX * pixelY) / ((twipX * twipY) * KTwipPerInch);
    
    if (dpi < KMidLeftBounday)
        {
        dpi = EDPILow;
        }
    else if (dpi >= KMidLeftBounday && dpi < KHigLeftBounday)
        {
        dpi = EDPIMiddle;
        }
    else
        {
        dpi = EDPIHigh;
        }
        
    switch(dpi)
        {
        case EDPILow:
            {
            InitLowDPI();
            }
            break;
        case EDPIMiddle:
            {
            InitMiddleDPI();
            }
            break;
        case EDPIHigh:
            {
            InitHighDPI();
            }
            break;
        default:
            {
            InitLowDPI();    
            break;
            }          
        }
    }
    
// ---------------------------------------------------------
// Initializes low dpi array
// ---------------------------------------------------------
//
void CGSPenInputModel::InitLowDPI()
    {
    iTrailWidthValue[0] = 2;
    iTrailWidthValue[1] = 3;
    iTrailWidthValue[2] = 4;
    }
    
// ---------------------------------------------------------
// Initializes middle dpi array
// ---------------------------------------------------------
//
void CGSPenInputModel::InitMiddleDPI()
    {
    iTrailWidthValue[0] = 3;
    iTrailWidthValue[1] = 4;
    iTrailWidthValue[2] = 6;
    }

// ---------------------------------------------------------
// Initializes high dpi array
// ---------------------------------------------------------
//
void CGSPenInputModel::InitHighDPI()
    {
    iTrailWidthValue[0] = 3;
    iTrailWidthValue[1] = 4;
    iTrailWidthValue[2] = 8;
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
CGSPenInputModel::~CGSPenInputModel()
    {
    delete iPersonalizationWatcher;
    delete iPersonalizationRepository;

    delete iAknfepWatcher;
    delete iAknfepRepository;

    delete iAknWatcher;
    delete iAknRepository;

    FeatureManager::UnInitializeLib();
    delete iPtiEngine;
    
    iPenInputServer.Close();
    }

// ---------------------------------------------------------
// Initializes Repository
// ---------------------------------------------------------
//
void CGSPenInputModel::InitializeCentralRepositoriesL()
    {
    iPersonalizationRepository = CRepository::NewL(KCRUidPersonalizationSettings);
    iPersonalizationWatcher = CGSRepositoryWatcher::NewL(iOwner, iPersonalizationRepository,KCRUidPersonalizationSettings);
    
    iAknfepRepository = CRepository::NewL(KCRUidAknFep);
    iAknfepWatcher = CGSRepositoryWatcher::NewL(iOwner, iAknfepRepository,KCRUidAknFep);

    iAknRepository = CRepository::NewL(KCRUidAvkon);
    iAknWatcher = CGSRepositoryWatcher::NewL(iOwner, iAknRepository,KCRUidAvkon);
    }

// ---------------------------------------------------------
// Current input language name in list box item    
// ---------------------------------------------------------
//
void CGSPenInputModel::InputLanguage(TDes& aLanguage) 
    {
    iAknfepRepository->Get(KAknFepInputTxtLang, iInputLanguage);
    iPtiEngine->GetLocalizedLanguageName(iInputLanguage & KGSLanguageMask,
                                         aLanguage);
    }

// ---------------------------------------------------------
// Current input languageID 
// ---------------------------------------------------------
//
TInt CGSPenInputModel::InputLanguage() 
    {
    iAknfepRepository->Get(KAknFepInputTxtLang, iInputLanguage);
    return iInputLanguage; 
    }
    
// ---------------------------------------------------------
// Get languages from ptiengine    
// ---------------------------------------------------------
//
void CGSPenInputModel::GetLanguagesFromPtiL(TBool aISOCode, CDesCArray* aLanguageNames)
    {
    InputLanguage();    
    RArray<TInt> languageCodeArray;

    iPtiEngine->GetAvailableLanguagesL(languageCodeArray);
 
    const TInt count = languageCodeArray.Count();
    TInt langId = 0;
    HBufC* buffer = HBufC::NewLC(KGSBufSize);
    TBuf<KISOCodeLength>  isoCodeName;

    for (TInt index = 0; index < count; index ++)
        {
        langId = languageCodeArray[index];

        TPtr ptrToBuffer(buffer->Des());
        iPtiEngine->GetLocalizedLanguageName(langId & KGSLanguageMask,
                                             ptrToBuffer);
         
        //AdjustLanguageName(langId & KGSLanguageMask, ptrToBuffer); 
                                           
        if (aISOCode)
            {
            if (AknPenInputUtils::GetISOLanguageCode(TLanguage(langId), isoCodeName))
                {
                ptrToBuffer.Append(KLeftBracket);
                ptrToBuffer.Append(isoCodeName);
                ptrToBuffer.Append(KRightBracket);
                }
            }

        aLanguageNames->AppendL(ptrToBuffer);
        if (iInputLanguage == langId)
            {
            iLanguageIndex = index;   
            }

        ptrToBuffer.Zero();
        }
        
    CleanupStack::PopAndDestroy(buffer);
    languageCodeArray.Reset();
    languageCodeArray.Close(); 
    }

// ---------------------------------------------------------
// Get Popup Query languages from ptiengine    
// ---------------------------------------------------------
//
void CGSPenInputModel::GetPopupQuerylanguagesFromPtiL(CDesCArray* aLanguageNames)    
    {
    RArray<TInt> languageCodeArray;

    iPtiEngine->GetAvailableLanguagesL(languageCodeArray);

    // Item format should be "<icon_index>\t<item_text>"
    _LIT( KIconIndexSelected, "0\t" );
    _LIT( KIconIndexNotSelected, "\t" );// No icon
    
    MPtiLanguage* lang = NULL;
    TBuf<KISOCodeLength>  isoCodeName;
    
    for (TInt index = 0; index < languageCodeArray.Count(); index ++)
        {
        lang = iPtiEngine->GetLanguage(languageCodeArray[index]);
        if (!lang)
            {
            User::Leave(KErrCorrupt);
            }
            
        // Buffer should contain both tabulator string and localized name:
        HBufC* langStrBuf = HBufC::NewL(lang->LocalizedName().Length() + 7);
        CleanupStack::PushL(langStrBuf);
        
        TPtr langStrPtr = langStrBuf->Des();
        // If language is current language - use selected icon in lbx:
        if(lang->LanguageCode() == InputLanguage())
            {
            langStrPtr.Append(KIconIndexSelected);
            iLanguageIndex = index;
            }
        else
            {
            langStrPtr.Append(KIconIndexNotSelected);    
            }
            
        langStrPtr.Append(lang->LocalizedName());
        
        //AdjustLanguageName(lang->LanguageCode(), langStrPtr); 
        
        /*if (AknPenInputUtils::GetISOLanguageCode(TLanguage(lang->LanguageCode()), 
                                                 isoCodeName))
            {
            langStrPtr.Append(KLeftBracket);
            langStrPtr.Append(isoCodeName);
            langStrPtr.Append(KRightBracket);
            }*/
        aLanguageNames->AppendL(langStrPtr);
        
        CleanupStack::PopAndDestroy(langStrBuf);
        }

    languageCodeArray.Reset();
    languageCodeArray.Close();     
    }
    
// ---------------------------------------------------------
// Store input language to Repository
// ---------------------------------------------------------
//
void CGSPenInputModel::SetInputLanguageL()
    {
    RArray<TInt> languageCodeArray;
    iPtiEngine->GetAvailableLanguagesL(languageCodeArray);
 
    const TInt count = languageCodeArray.Count();
 
    if (iLanguageIndex <= count)
        {
        iInputLanguage = languageCodeArray[iLanguageIndex];
        iAknfepRepository->Set(KAknFepInputTxtLang, iInputLanguage);    
        }

    languageCodeArray.Reset();
    languageCodeArray.Close();    

    // Remember which chinese language variant was used 
    // for the chinese find method setting
    TUint key = ChineseFindMethodCRKey();
    if(key != iLastChineseFindMethodItem)
        {
        iAknRepository->Set(KAknAdaptiveSearchLastChinese, (int)key);
        iLastChineseFindMethodItem = key;
        }
    }
    
// ---------------------------------------------------------
// Current writing speed index in list box item
// ---------------------------------------------------------
//
TInt CGSPenInputModel::WritingSpeed() 
    {
    iPersonalizationRepository->Get(KSettingsWritingSpeed, iSpeed);

    for (TInt index = 0; index < EWritingSpeedArrayLength; index++)
        {
        if (iSpeed == iWritingSpeedValue[index])
            {
            return index;            
            }
        }

    iSpeed = iWritingSpeedValue[0];
    iPersonalizationRepository->Set(KSettingsWritingSpeed, iSpeed);
    
    return 0;
    }

// ---------------------------------------------------------
// DStore writing speed to Repository
// ---------------------------------------------------------
//
void CGSPenInputModel::SetWritingSpeed(const TInt aSpeed)
    {
    iSpeed = iWritingSpeedValue[aSpeed];
    iPersonalizationRepository->Set(KSettingsWritingSpeed, iSpeed);
    }

// ---------------------------------------------------------
// Store guid line to Repository
// ---------------------------------------------------------
//
void CGSPenInputModel::SetGuideLine(TInt aGuideLine)
    {
    iGuideLine = aGuideLine;
    iPersonalizationRepository->Set(KSettingsPenInputGuideLine, iGuideLine);
    }
     
// ---------------------------------------------------------
// Store recognition with dictionary to Repository
// ---------------------------------------------------------
//
void CGSPenInputModel::SetRecognitionWithDictionary(TInt aRecognitionWithDictionary)
    {
    iIsRecognitionWithDictionary = aRecognitionWithDictionary;
    iPersonalizationRepository->Set(KSettingRecognitionwithdictionary, iIsRecognitionWithDictionary);
    }
// ---------------------------------------------------------
// Get current guid line from Repository
// ---------------------------------------------------------
//
TInt CGSPenInputModel::GuideLine()
    {
    iPersonalizationRepository->Get(KSettingsPenInputGuideLine, iGuideLine);

    if (iGuideLine == 0 || iGuideLine == 1)
        {
        return iGuideLine;
        }
    iGuideLine = 1; //Set default value to repository key
    iPersonalizationRepository->Set(KSettingsPenInputGuideLine, iGuideLine);

    return iGuideLine;
    }

// ---------------------------------------------------------
// Get the item of recognition with dictionary from Repository
// ---------------------------------------------------------
//
TInt CGSPenInputModel::RecognitionWithDictionary()
    {
    iPersonalizationRepository->Get(KSettingRecognitionwithdictionary, iIsRecognitionWithDictionary);

    if (iIsRecognitionWithDictionary == 0 || iIsRecognitionWithDictionary == 1)
        {
        return iIsRecognitionWithDictionary;
        }
    iIsRecognitionWithDictionary = 1; //Set default value to repository key
    iPersonalizationRepository->Set(KSettingRecognitionwithdictionary, iIsRecognitionWithDictionary);

    return iIsRecognitionWithDictionary;
    }

// ---------------------------------------------------------
// Store input method for find to Repository
// ---------------------------------------------------------
//
void CGSPenInputModel::SetInputMethodForFind(TInt aInputMethodForFind)
    {
    iInputMethodForFind = aInputMethodForFind;
    iAknRepository->Set(KAknAvkonAdaptiveSearchEnabled, iInputMethodForFind);
    }
     
// ---------------------------------------------------------
// Get current input method for find from Repository
// ---------------------------------------------------------
//
TInt CGSPenInputModel::InputMethodForFind()
    {
    iAknRepository->Get(KAknAvkonAdaptiveSearchEnabled, iInputMethodForFind);

    if (iInputMethodForFind == 0 || iInputMethodForFind == 1)
        {
        return iInputMethodForFind;
        }

    // Set default value to qtn_ads_settings_adaptive_grid
    iInputMethodForFind = 1;
    iAknRepository->Set(KAknAvkonAdaptiveSearchEnabled, iInputMethodForFind);

    return iInputMethodForFind;
    }

// ---------------------------------------------------------
// Get chinese find method resource id
// ---------------------------------------------------------
//
TInt CGSPenInputModel::ChineseFindMethodResourceID()
    {
    TInt resource = -1;
    switch(ChineseFindMethodCRKey())
        {
        case KAknAdaptiveSearchChinesePRC:
            {
            resource = R_GS_CHINESEFINDMETHOD_SIMPLE_SETTING_PAGE_LBX;
            break;
            }
        case KAknAdaptiveSearchChineseTW:
            {
            resource = R_GS_CHINESEFINDMETHOD_TAIWAN_SETTING_PAGE_LBX;
            break;
            }
        case KAknAdaptiveSearchChineseHongkong:
            {
            resource = R_GS_CHINESEFINDMETHOD_HONGKONG_SETTING_PAGE_LBX;
            break;
            }
        }
    return resource;
    }

// ---------------------------------------------------------
// Get chinese find method cenrep key
// ---------------------------------------------------------
//
TUint CGSPenInputModel::ChineseFindMethodCRKey()
    {
    TUint key(0);
    // For the first time, update value from cenrep.
    // If key does not exist, set to 0. Then it is not queried again.
    if(iLastChineseFindMethodItem < 0)
        {
        if(iAknRepository->Get(KAknAdaptiveSearchLastChinese, iLastChineseFindMethodItem) 
           != KErrNone)
            {
            iLastChineseFindMethodItem = 0;
            }
        }

    // If input language is one of the chinese then show 
    // the setting item for that language
    if (iInputLanguage == ELangPrcChinese)
        { 
        key = KAknAdaptiveSearchChinesePRC;
        }
    else if (iInputLanguage == ELangTaiwanChinese)
        { 
        key = KAknAdaptiveSearchChineseTW;
        }
    else if (iInputLanguage == ELangHongKongChinese) 
        {
        key = KAknAdaptiveSearchChineseHongkong;
        }
    // If language is not chinese, and there is used chinese item, 
    // then show the last used setting item.
    else if (iLastChineseFindMethodItem > 0)
        {
        key = iLastChineseFindMethodItem;
        }
    return key;
    }

// ---------------------------------------------------------
// Store chinese find method to Repository
// ---------------------------------------------------------
//
void CGSPenInputModel::SetChineseFindMethod(TInt aChineseFindMethod)
    {
    TUint key = ChineseFindMethodCRKey();

    iChineseFindMethod = aChineseFindMethod;

    if(key > 0)
        {
        iAknRepository->Set(key, iChineseFindMethod);
        }
    }

// ---------------------------------------------------------
// Get current input method for find from Repository
// ---------------------------------------------------------
//
TInt CGSPenInputModel::ChineseFindMethod()
    {
    TUint key = ChineseFindMethodCRKey();

    if(key > 0)
        {
        if (iAknRepository->Get(key, iChineseFindMethod) == KErrNotFound)
            {
            // Set default value to repository key
            iChineseFindMethod = 0; 
            iAknRepository->Set(key, iChineseFindMethod);
            }
        }

    return iChineseFindMethod;
    }

// ---------------------------------------------------------
// Current pen trail width index in list box item
// ---------------------------------------------------------
//
TInt CGSPenInputModel::TrailWidth()  
    {
    iPersonalizationRepository->Get(KSettingsPenWidth, iWidth);

    for (TInt index = 0; index < ETrailWidthArrayLength; index++)
        {
        if (iWidth == iTrailWidthValue[index])
            {
            return index;
            }
        }
    
    iWidth = iTrailWidthValue[0];
    iPersonalizationRepository->Set(KSettingsPenWidth, iWidth);
    
    return 0;
    }

// ---------------------------------------------------------
// Store pen trail width to Repository
// ---------------------------------------------------------
//
void CGSPenInputModel::SetTrailWidth(const TInt aWidth)
    {
    iWidth = iTrailWidthValue[aWidth];    
    iPersonalizationRepository->Set(KSettingsPenWidth, iWidth);
    }

// ---------------------------------------------------------
// Current pen trail colour index in list box item
// ---------------------------------------------------------
//
TInt CGSPenInputModel::TrailColour() 
    {
    iPersonalizationRepository->Get(KSettingsPenColor, iColor);
    if (!AvailableTrailColor(TRgb(iColor)))
        {
        iColor = AknPenInputTrailColorUtils::GetTrailColorByTheme();
        }
        
    return iColor;
    }

// ---------------------------------------------------------
// Store pen trail colour to Repository
// ---------------------------------------------------------
//
void CGSPenInputModel::SetTrailColour(const TInt aColour)
    {
    iColor = aColour;
    iPersonalizationRepository->Set(KSettingsPenColor, aColour);
    }

// ---------------------------------------------------------
// Set Inpt mode based on current language
// ---------------------------------------------------------
//
void CGSPenInputModel::SetInputModeBySettingPage(const TInt aIndex)
    {
    TInt curInputMode = -1;

    MPtiLanguage* ptiLang = iPtiEngine->GetLanguage(iInputLanguage & KGSLanguageMask);    

    if (iInputLanguage == ELangPrcChinese)
        {
        if( ptiLang != NULL )
            {
            if (aIndex == 0 && ptiLang->HasInputMode(EPtiEnginePinyinByPhrase))
                {
                curInputMode = EPinyin;
                }
            else
                {
                curInputMode = EStroke;    
                }
            }

        if (curInputMode != -1)
            {
            iPersonalizationRepository->Set(KSettingsDefaultVkbPrc, curInputMode);
            iInputMode = curInputMode;   
            }
        }
    else if (iInputLanguage == ELangHongKongChinese) 
        {
        if( ptiLang != NULL )
            {
            if (aIndex == 0 && ptiLang->HasInputMode(EPtiEngineStrokeByPhrase))
                {
                curInputMode = EStroke;
                }
            else
                {
                curInputMode = ECangJie;   
                }
            }
            
        if (curInputMode != -1)
            {
            iPersonalizationRepository->Set(KSettingsDefaultVkbHk, curInputMode);
            iInputMode = curInputMode;   
            }
        }
    else if (iInputLanguage == ELangTaiwanChinese)
        { 
        if( ptiLang != NULL )
            {
            if (aIndex == 0 && ptiLang->HasInputMode(EPtiEngineZhuyinByPhrase))
                {
                curInputMode = EZhuyin;
                }
            else if (aIndex == 0 && ptiLang->HasInputMode(EPtiEngineStrokeByPhrase))
                {
                curInputMode = EStroke;
                }
            else if (aIndex == 0 && ptiLang->HasInputMode(EPtiEnginePinyinByPhrase))
                {
                curInputMode = EPinyin;
                }
            else if (aIndex == 1 && ptiLang->HasInputMode(EPtiEngineStrokeByPhrase))
                {
                curInputMode = EStroke;
                }
            else if (aIndex == 1 && ptiLang->HasInputMode(EPtiEnginePinyinByPhrase))
                {
                curInputMode = EPinyin;
                }
            else if (aIndex == 2)
                {
                curInputMode = EPinyin;            
                }
            }

        if (curInputMode != -1)
            {
            iPersonalizationRepository->Set(KSettingsDefaultVkbTw, curInputMode);
            iInputMode = curInputMode;   
            }
        }
    }

// ---------------------------------------------------------
// Get input mode index in setting page
// ---------------------------------------------------------
//
TInt CGSPenInputModel::InputModeInSettingPage()
    {
    TInt index = 0;
    TInt skipCount = 0;
    MPtiLanguage* ptiLang = iPtiEngine->GetLanguage(iInputLanguage & KGSLanguageMask);    
    
    if( ptiLang == NULL )
        {
        return index;
        }
    if (iInputLanguage == ELangPrcChinese)
        {
        if (iInputMode == EPinyin && ptiLang->HasInputMode(EPtiEnginePinyinByPhrase))
            {
            return index;
            }
            
        if (!ptiLang->HasInputMode(EPtiEnginePinyinByPhrase))
            {
            skipCount ++;
            }
            
        if (iInputMode == EStroke && ptiLang->HasInputMode(EPtiEngineStrokeByPhrase))
            {
            index ++;
            return index - skipCount;
            }
        }
    else if (iInputLanguage == ELangHongKongChinese) 
        {
        if (iInputMode == EStroke && ptiLang->HasInputMode(EPtiEngineStrokeByPhrase))
            {
            return index;
            }
            
        if (!ptiLang->HasInputMode(EPtiEngineStrokeByPhrase))
            {
            skipCount ++;
            }
            
         if ( iInputMode == ECangJie && (
              ptiLang->HasInputMode(EPtiEngineNormalCangjieQwerty)|| 
              ptiLang->HasInputMode(EPtiEngineNormalCangjieVkb) ) )
            {
            index ++;
            return index - skipCount;
            }
        }
    else if (iInputLanguage == ELangTaiwanChinese)
        { 
        if (iInputMode == EZhuyin && ptiLang->HasInputMode(EPtiEngineZhuyinByPhrase))
            {
            return index;
            }
            
        if (!ptiLang->HasInputMode(EPtiEngineZhuyinByPhrase))
            {
            skipCount ++;
            }
            
        if (iInputMode == EStroke && ptiLang->HasInputMode(EPtiEngineStrokeByPhrase))
            {
            index ++;
            return index - skipCount;
            }
        if (!ptiLang->HasInputMode(EPtiEngineStrokeByPhrase))
            {
            skipCount ++;
            }
        if (iInputMode == EPinyin && ptiLang->HasInputMode(EPtiEnginePinyinByPhrase))
            {
            index ++;
            return index - skipCount;
            } 
        }
        
    return index;
    }

// ---------------------------------------------------------
// Get input mode resource id
// ---------------------------------------------------------
//
TInt CGSPenInputModel::InputModeResourceID()
    {
    TInt resource = -1;
    if (iInputLanguage == ELangPrcChinese)
        { 
        resource = R_GS_OSK_SIMPLECHINESE_SETTING_PAGE_LBX;
        }
    else if (iInputLanguage == ELangHongKongChinese) 
        {  
        resource = R_GS_OSK_HONGKONGCHINESE_SETTING_PAGE_LBX;
        }
    else if (iInputLanguage == ELangTaiwanChinese)
        { 
        resource = R_GS_OSK_TAIWANCHINESE_SETTING_PAGE_LBX;
        }
        
    return resource;
    }

// ---------------------------------------------------------
// Reset input mode destription array
// ---------------------------------------------------------
//
void CGSPenInputModel::ResetInputModeArray(CDesCArrayFlat* aInputModeItems)
    {
    if ( !aInputModeItems )
        {
        return;
        }
    
    MPtiLanguage* ptiLang = iPtiEngine->GetLanguage(iInputLanguage & KGSLanguageMask);    
    if( ptiLang == NULL )
        {
        return;
        }
    switch(iInputLanguage)
        {
        case ELangPrcChinese:
            {            
            if(!ptiLang->HasInputMode(EPtiEngineStrokeByPhrase))
                {
                aInputModeItems->Delete(1);    
                }
            if(!ptiLang->HasInputMode(EPtiEnginePinyinByPhrase))
                {
                aInputModeItems->Delete(0);    
                }
            }
            break;
        case ELangHongKongChinese:
            {            
            if(!ptiLang->HasInputMode(EPtiEngineNormalCangjieVkb))
                {
                aInputModeItems->Delete(1);    
                }
            if(!ptiLang->HasInputMode(EPtiEngineStrokeByPhrase))
                {
                aInputModeItems->Delete(0);    
                }
            }
            break;
         case ELangTaiwanChinese:
            {            
            if(!ptiLang->HasInputMode(EPtiEnginePinyinByPhrase))
                {                
                aInputModeItems->Delete(2);                    
                }
            if(!ptiLang->HasInputMode(EPtiEngineStrokeByPhrase))
                {                
                aInputModeItems->Delete(1);                    
                }            
            if(!ptiLang->HasInputMode(EPtiEngineZhuyinByPhrase))
                {                
                aInputModeItems->Delete(0);                                    
                }
            }
            break;
         default:            
            break;
        }    
    }    

// ---------------------------------------------------------
// Reset input mode to default input mode when language changed 
// ---------------------------------------------------------
//
void CGSPenInputModel::ResetDefaultInputMode()    
    {
    switch(iInputLanguage)
        {
        case ELangPrcChinese:
            {
            if (InputMode() != EPinyin)
                {
                SetInputMode(EPinyin);
                }
            }
            break;
        case ELangHongKongChinese:
            {
            if (InputMode() != EStroke)
                {
                SetInputMode(EStroke);
                }
            }
            break;
         case ELangTaiwanChinese:
            {
            if (InputMode() != EZhuyin)
                {
                SetInputMode(EZhuyin);
                }
            }
            break;
         default:
            break;
        }        
    }

// ---------------------------------------------------------
// Reset Fep input mode based on current input language
// ---------------------------------------------------------
//
void CGSPenInputModel::ResetFepDefaultInputMode()
    {
    switch(iInputLanguage)
        {
        case ELangPrcChinese:
            {
            if (GetFepInputMode() != EPinyin)
                {
                SetFepInputMode(EPinyin);
                }
            }
            break;
        case ELangHongKongChinese:
            {
            if (GetFepInputMode() != EStroke)
                {
                SetFepInputMode(EStroke);
                }
            }
            break;
         case ELangTaiwanChinese:
            {
            if (GetFepInputMode() != EZhuyin)
                {
                SetFepInputMode(EZhuyin);
                }                
            }
            break;
         default:
            break;
        }        
    
    }

// ---------------------------------------------------------
// Set input mode to repository
// ---------------------------------------------------------
//
void CGSPenInputModel::SetInputMode(TInt aInputMode)
    {
    switch(iInputLanguage)
        {
        case ELangPrcChinese:
            {
            iPersonalizationRepository->Set(KSettingsDefaultVkbPrc, aInputMode);
            }
            break;
        case ELangHongKongChinese:
            {
            iPersonalizationRepository->Set(KSettingsDefaultVkbHk, aInputMode);
            }
            break;
         case ELangTaiwanChinese:
            {
            iPersonalizationRepository->Set(KSettingsDefaultVkbTw, aInputMode);
            }
            break;
         default:
            break;
        }
        
    iInputMode = aInputMode;   
    }
    
// ---------------------------------------------------------
// Get input mode from repository
// ---------------------------------------------------------
//
TInt CGSPenInputModel::InputMode()
    {
    switch(iInputLanguage)
        {
        case ELangPrcChinese:
            {
            iPersonalizationRepository->Get(KSettingsDefaultVkbPrc, iInputMode);
            }
            break;
        case ELangHongKongChinese:
            {
            iPersonalizationRepository->Get(KSettingsDefaultVkbHk, iInputMode);
            }
            break;
         case ELangTaiwanChinese:
            {
            iPersonalizationRepository->Get(KSettingsDefaultVkbTw, iInputMode);
            }
            break;
         default:
            break;
        }
     
    return iInputMode; 
    }
    
// ---------------------------------------------------------
// Set FEP default input mode when language changed 
// ---------------------------------------------------------
//
void CGSPenInputModel::SetFepInputMode(TInt aInputMode)
    {
    switch(iInputLanguage)
        {
        case ELangPrcChinese:
        case ELangHongKongChinese:
        case ELangTaiwanChinese:
            {
            iAknfepRepository->Set(KAknFepChineseInputMode, aInputMode);
            }
            break;
         default:
            break;
        }
    
    }
        
// ---------------------------------------------------------
// Get FEP default input mode when language changed
// ---------------------------------------------------------
//
TInt CGSPenInputModel::GetFepInputMode()
    {
    TInt inputMode = 0;
    switch(iInputLanguage)
        {
        case ELangPrcChinese:
        case ELangHongKongChinese:
        case ELangTaiwanChinese:
            {
            iAknfepRepository->Get(KAknFepChineseInputMode, inputMode);
            }
            break;
         default:
            break;
        }
        
    return inputMode;
    }

// ---------------------------------------------------------
// Adjust language name
// ---------------------------------------------------------
//
void CGSPenInputModel::AdjustLanguageName(TInt aLangId, TDes& aLangName)
    {
    switch(aLangId)
        {
        case ELangPrcChinese:
        case ELangHongKongChinese:
        case ELangTaiwanChinese:
            {
            TInt index = aLangName.Find(KLeftBracket);
            if( index != KErrNotFound)
                {
                aLangName.SetLength(index);
                }
            }
        }
    }

TBool CGSPenInputModel::IsSettingItemAvailable( TInt aItemId )
    {
    TBool res = EFalse;    
    switch( aItemId )
        {
        case EGSInputpenIdHwrTraining:
            {
            iIsHwrTrainingAviliable = EFalse;
            TInt supportMode = EPluginInputModeNone;
            if(KErrNone == iConnectErr)
                {
                supportMode = iPenInputServer.SupportInputMode();
                }
            TBool supportHWR = EFalse;
            if(supportMode & EPluginInputModeHwr ||
               supportMode & EPluginInputModeFSc ||
               supportMode & EPluginInputModeFingerHwr)
                {
                supportHWR = ETrue;
                }
              
            CEikonEnv* env = CEikonEnv::Static();
            TUid appUid = env->EikAppUi()
                          ->Application()->AppDllUid();
            if ( appUid == KUidPenInputSettingApp )
                {
                // if open from trui, don't show this item
                // if not, need to do further check.
                // if its parent group is trui                
                CPenInputSettingAppUi* appui 
                = static_cast<CPenInputSettingAppUi*>( env->EikAppUi() );
                TUid parentUid = AppUidFromWndGroupId( appui->ParentWindowGroupId() );
                if ( parentUid == KUidtruiApp )
                   {
                    iIsHwrTrainingAviliable = EFalse;                    
                    }
                else
                       {
                    iIsHwrTrainingAviliable = IsTruiSupportedLanguage();
                       }
                   }                
            else
                {
                iIsHwrTrainingAviliable = IsTruiSupportedLanguage();
                }
            res = ( !ChineseLanguage() && !JapaneseLanguage() && iIsHwrTrainingAviliable && supportHWR);
            }
            break;
        case EGSInputpenIdInputLanguage:
            {
            res = ETrue;
            }
            break;
            
        case EGSDefaultChineseOnscreenKeyboard:
            {
            // ChineseLanguage() && ( Support VKB or FSQ )
            res = ChineseLanguage() 
                   && ( FeatureManager::FeatureSupported
                        ( KFeatureIdVirtualKeyboardInput ) 
                        || FeatureManager::FeatureSupported
                     ( KFeatureIdVirtualFullscrQwertyInput ) );
            }                        
            break;
            
        case EGSInputpenIdGuidLine:
            {
            TInt supportMode = EPluginInputModeNone;
            if(KErrNone == iConnectErr)
                {
                supportMode = iPenInputServer.SupportInputMode();
                }
            
            TBool supportHWR = EFalse;
            if(supportMode & EPluginInputModeHwr ||
               supportMode & EPluginInputModeFSc ||
               supportMode & EPluginInputModeFingerHwr)
                {
                supportHWR = ETrue;
                }
            
            res = ( !ChineseLanguage() && !JapaneseLanguage() && supportHWR);
            }
            break;

        case EGSInputpenIdRecognitionWithDictionary:
            {
            TInt currentLanguage = InputLanguage(); // add this line
            if (currentLanguage == ELangArabic)
                {
                res = ETrue;
                }
            }              
            break;   
            
        case EGSInputpenIdWritingSpeed:
        case EGSInputpenIdPenTrailWidth:
        case EGSInputpenIdPenTrailColour:
            {
            TInt supportMode = EPluginInputModeNone;
            if(KErrNone == iConnectErr)
                {
                supportMode = iPenInputServer.SupportInputMode();
                }
               
            if(supportMode & EPluginInputModeHwr ||
               supportMode & EPluginInputModeFSc ||
               supportMode & EPluginInputModeFingerHwr)
                {
                res = ETrue;
                }
            }
            break;
            
         case EGSInputpenIdInputMethodForFind:
            {
            res = ETrue; 
            }
            break;   
            
        case EGSInputpenIdTouchScrCalib:
            {
            res = ETrue;
            }
            break;
            
        case EGSInputpenIdChineseFindMethod:
            {
            res = ChineseFindMethodCRKey() > 0;
            }
            break;
        }
    return res;
    }

// ---------------------------------------------------------
// Return current application's uid.
// ---------------------------------------------------------
//
TUid CGSPenInputModel::AppUidFromWndGroupId( TInt aWndGrpId )
    {
    TUid id = {0x00000000};
    TRAP_IGNORE(id = AppUidFromWndGroupIdL(aWndGrpId));        
    return id;
    }

TUid CGSPenInputModel::AppUidFromWndGroupIdL( TInt aWndGrpId )
    {
    RWsSession &ws = CCoeEnv::Static()->WsSession();    
    CApaWindowGroupName* wg = CApaWindowGroupName::NewLC(ws,aWndGrpId);
    TUid id = wg->AppUid();
    CleanupStack::PopAndDestroy(wg);    
    return id;    
    }

TBool CGSPenInputModel::IsTruiSupportedLanguage()
    {
    TInt currentLanguage = InputLanguage();
    TInt count = sizeof(KTruiSupportedLanguages)/sizeof(TInt);
    for(TInt i = 0; i < count; i++)
       {
       if( currentLanguage == KTruiSupportedLanguages[i] )
           {
           return ETrue;           
           }
       }    
    return EFalse;
    }

//  End Of File
