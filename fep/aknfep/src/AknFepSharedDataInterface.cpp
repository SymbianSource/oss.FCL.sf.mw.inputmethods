/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*       CAknFepSharedDataInterface class
*
*/












#include <w32std.h>
#include <eikdef.h>
#include <eikenv.h>
#include <AknUtils.h>

#include "AknFepSharedDataInterface.h"
#include "AknFepManager.h"

#include <centralrepository.h>
#include <AknFepInternalCRKeys.h>
#ifdef RD_INTELLIGENT_TEXT_INPUT
#include <AknFepInternalPSKeys.h>
#include <AknFepGlobalEnums.h>
#include <PtiDefs.h>
#endif
#include <AvkonInternalCRKeys.h>    // KAknQwertyInputModeActive
#include <settingsinternalcrkeys.h> //Pen related settings
#include <sensorplugindomaincrkeys.h> // KCRUidSensorSettings

// TODO: include CommonengineInternalCRKeys.h
const TUid KCRUidCommonEngineKeys = {0x1020503B};
const TUint32 KGSDisplayTxtLang = 0x00000001;

const TInt KJapHwrConversion = 0x0010;

#include <AknDef.h>

CAknFepSharedDataInterface* CAknFepSharedDataInterface::NewL(CAknFepManager* aFepManager)
    {
    CAknFepSharedDataInterface* self = new (ELeave) CAknFepSharedDataInterface(aFepManager);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(); //self
    return self;
    }


CAknFepSharedDataInterface::~CAknFepSharedDataInterface()
    {
    delete iCommonEngineRepository;
    delete iAknFepRepositoryWatcher;
    delete iAknFepLocaleRepositoryWatcher;
    delete iAknFepRepository;
    delete iGsPenSettings;
    delete iLocaleRepository;
    delete iSensorRepository;

    if (iQwertyModeStatusSubscriber)
        {
        iQwertyModeStatusSubscriber->StopSubscribe();
        }
#ifdef RD_INTELLIGENT_TEXT_INPUT
    if (iKeyboardLayoutStatusSubscriber)
        {
        iKeyboardLayoutStatusSubscriber->StopSubscribe();
        }
    iKeyboardLayoutStatusProperty.Close();
    delete iKeyboardLayoutStatusSubscriber;    
#endif
    iQwertyModeStatusProperty.Close();
    delete iQwertyModeStatusSubscriber;
    }


TInt CAknFepSharedDataInterface::PredictiveTextOn()
    {
    TInt value = 0;
    if (iAknFepRepository)
        {
#ifdef RD_INTELLIGENT_TEXT_INPUT
        TPtiKeyboardType layout = (TPtiKeyboardType)KeyboardLayout();
        TInt temp = 0;
        iAknFepRepository->Get(KAknFepPredTxtFlagExtension, temp);
        switch(layout)
            {
            case EPtiKeyboardNone:
                break;
          	case EPtiKeyboard12Key:
                temp &= EKeyboardStyle12Key;
                break;
            case EPtiKeyboardHalfQwerty:
                temp &= EKeyboardStyleHalfQwerty;
                break;                
          	case EPtiKeyboardQwerty4x12:
          	case EPtiKeyboardQwerty4x10:
          	case EPtiKeyboardQwerty3x11:          	
          	case EPtiKeyboardCustomQwerty:
                temp &= EKeyboardStyleQwerty;
                break;
          	default:
          	    break;
            }
        value = ( temp ? 1 : 0);
#else
        iAknFepRepository->Get(KAknFepPredTxtFlag, value);
#endif
        }
    return value;
    }

TInt CAknFepSharedDataInterface::PredictiveTextJapaneseOn()
    {
    TInt value = 0;
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepJapanesePredTxtFlag, value);
        }
    return value;
    }

TInt CAknFepSharedDataInterface::InputMode()
    {
    TUint value = 0;
    if (iAknFepRepository)
        {
   		/* Teleca change begin, 03.07.2008 ssal 
   			original code:
		TPtrC filteredPtr(0,0);
		iAknFepRepository->Get(KAknFepChineseInputMode, value);*/
		_LIT(Kx, "x");
		// This conversion is needed because KAknFepChineseInputMode cenrep key original type was 16bit int.
		// now type is changed to string, so that it can accommodate bigger values like EHangul 0x16000. 
		TBuf<CAknFepManager::EMaximumFepWordLength> conversion;
        iAknFepRepository->Get(KAknFepChineseInputMode, conversion);
       
       	TInt len = conversion.Find(Kx);
       	TLex lex;
       	
       	if(len)
       		{
       		TPtrC ptr = conversion.Mid(len +1);
       		lex.Assign(ptr);
       		}
		else
			{
			lex.Assign(conversion);
			}
		
		lex.Val(value, EHex);
		/* Teleca change end, 04.06.2008 ssal */
        }
    return value;
    }

void CAknFepSharedDataInterface::SetInputMode(TInt aInputMode)
    {
    if (iAknFepRepository)
        {
        /* Teleca change begin, 03.07.2008 ssal 
			original code:
        iAknFepRepository->Set(KAknFepChineseInputMode, aInputMode);*/
        
        // Please see the explanation on CAknFepSharedDataInterface::InputMode() method
		TBuf<CAknFepManager::EMaximumFepWordLength> conversion;
		conversion.Num(aInputMode, EHex);
		
        iAknFepRepository->Set(KAknFepChineseInputMode, conversion);
      	/* Teleca change end, 04.06.2008 ssal */
        }
    }

TInt CAknFepSharedDataInterface::CangJieMode()
    {
    TInt value = 0;
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepCangJieMode, value);
        }
    return value;
    }
    
void CAknFepSharedDataInterface::SetCangJieMode(TInt aNewCangJieMode)
    {
    if (iAknFepRepository)
        {
        iAknFepRepository->Set(KAknFepCangJieMode, aNewCangJieMode);
        }
    }

TInt CAknFepSharedDataInterface::InputTextLanguage()
    {
    TInt value = 0;
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepInputTxtLang, value);
        }
    return TLanguage(value);
    }

void CAknFepSharedDataInterface::SetInputTextLanguage(TInt aValue)
    {
    if (iAknFepRepository)
        {
        iAknFepRepository->Set(KAknFepInputTxtLang, aValue);
        }

    // now inform all open apps of the switch
    TWsEvent event;
    event.SetType(KEikInputLanguageChange);
    CEikonEnv::Static()->WsSession().SendEventToAllWindowGroups(event);
    }

TInt CAknFepSharedDataInterface::DisplayLanguage()
    {
    TInt value = 0;
    if (iCommonEngineRepository)
        {
        iCommonEngineRepository->Get(KGSDisplayTxtLang, value);
        }
    return TLanguage(value);
    }

void CAknFepSharedDataInterface::SetPredictiveTextOn()
    {
    if (iAknFepRepository)
        {
#ifdef RD_INTELLIGENT_TEXT_INPUT
        TPtiKeyboardType layout = (TPtiKeyboardType)KeyboardLayout();
        TInt value = 0;
        iAknFepRepository->Get(KAknFepPredTxtFlagExtension, value);
        switch(layout)
            {
            case EPtiKeyboardNone:
                // No keyboard active, so don't change anything
                break;
          	case EPtiKeyboard12Key:
                value |= EKeyboardStyle12Key;
                break;
            case EPtiKeyboardHalfQwerty:    
                value |= EKeyboardStyleHalfQwerty;
                break;             
          	case EPtiKeyboardQwerty4x12:
          	case EPtiKeyboardQwerty4x10:
          	case EPtiKeyboardQwerty3x11:
          	case EPtiKeyboardCustomQwerty:
                value |= EKeyboardStyleQwerty;
                break;
          	default:
          	    break;
            }
        iAknFepRepository->Set(KAknFepPredTxtFlagExtension, value);
        // For backwards compatibility
        if (value & EKeyboardStyle12Key)
            {
            iAknFepRepository->Set(KAknFepPredTxtFlag, 1);
            }
#else
        iAknFepRepository->Set(KAknFepPredTxtFlag, 1);
#endif
        }
    }


void CAknFepSharedDataInterface::ResetPredictiveTextOn()
    {
    if (iAknFepRepository)
        {
#ifdef RD_INTELLIGENT_TEXT_INPUT
        TPtiKeyboardType layout = (TPtiKeyboardType)KeyboardLayout();
        TInt value = 0;
        iAknFepRepository->Get(KAknFepPredTxtFlagExtension, value);
        switch(layout)
            {
            case EPtiKeyboardNone:
                // No keyboard active, so don't change anything
                break;
          	case EPtiKeyboard12Key:
                value &= ~EKeyboardStyle12Key;
                break;
          	case EPtiKeyboardHalfQwerty:
                value &= ~EKeyboardStyleHalfQwerty;
                break;
          	case EPtiKeyboardQwerty4x12:
          	case EPtiKeyboardQwerty4x10:
          	case EPtiKeyboardQwerty3x11:
          	case EPtiKeyboardCustomQwerty:
                value &= ~EKeyboardStyleQwerty;
                break;
          	default:
          	    break;
            }
        iAknFepRepository->Set(KAknFepPredTxtFlagExtension, value);
        // For backwards compatibility
        if (!(value & EKeyboardStyle12Key))
            {
            iAknFepRepository->Set(KAknFepPredTxtFlag, 0);
            }
#else
        iAknFepRepository->Set(KAknFepPredTxtFlag, 0);
#endif
        }
    }

void CAknFepSharedDataInterface::SetJapanesePredictiveTextOn()
    {
    if (iAknFepRepository)
        {
        iAknFepRepository->Set(KAknFepJapanesePredTxtFlag, 1);
        }
    }


void CAknFepSharedDataInterface::ResetJapanesePredictiveTextOn()
    {
    if (iAknFepRepository)
        {
        iAknFepRepository->Set(KAknFepJapanesePredTxtFlag, 0);
        }
    }

TInt CAknFepSharedDataInterface::PluginInputMode() const
    {
    TInt value = 0;
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepLastUsedInput, value);
        }
    return value;
    }

void CAknFepSharedDataInterface::SetPluginInputMode(TInt aDevice)
    {
    if (iAknFepRepository)
        {
        iAknFepRepository->Set(KAknFepLastUsedInput, aDevice);
        }
    }

TInt CAknFepSharedDataInterface::PluginUILanguage() const
    {
    TInt value = 0;
    if (iAknFepRepository)
        {
        //iAknFepRepository->Create(KAknFepLastUsedUILanguage, 0); //??
        iAknFepRepository->Get(KAknFepLastUsedUILanguage, value);
        }
    return value;
    }

void CAknFepSharedDataInterface::SetPluginUILanguage(TInt aLanguage)
    {
    if (iAknFepRepository)
        {
        iAknFepRepository->Set(KAknFepLastUsedUILanguage, aLanguage);
        }
    }

void CAknFepSharedDataInterface::SetDefaultNumberMode( TInt aMode, TInt aValue )
    {
    if (iLocaleRepository)
        {
        iLocaleRepository->Set(aMode, aValue);
        }
    }


TInt CAknFepSharedDataInterface::PenInputLanguage() const
    {
    TInt value = 0;
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepInputTxtLang, value);
        }
    return value;
    }

TInt CAknFepSharedDataInterface::ScrollLatinPredictiveCandidates() const
    {
    return iSharedDataScrollLatinPredictive;
    }

TInt CAknFepSharedDataInterface::SpaceWithScrollRight() const
    {
    return iSharedDataSpaceWithScrollRight;
    }

TInt CAknFepSharedDataInterface::EntersWithScrollDown() const
    {
    return iSharedDataEnterWithScrollDown;
    }

TInt CAknFepSharedDataInterface::QwertyInputMode() //const
    {
    TInt value = 0;
    iQwertyModeStatusProperty.Get(value);
    return value;
    }

#ifdef RD_INTELLIGENT_TEXT_INPUT
TInt CAknFepSharedDataInterface::KeyboardLayout() //const
    {
    TInt value = 0;
    iKeyboardLayoutStatusProperty.Get(value);
    return value;
    }

#ifdef FF_DUAL_LANGUAGE_SUPPORT
TInt CAknFepSharedDataInterface::InputTextLanguageSecondary()
    {
    TInt value = 0;
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepInputTxtLangSecondary, value);
        }
    return TLanguage(value);
    }

void CAknFepSharedDataInterface::SetInputTextLanguageSecondary(TInt aValue)
    {
    if (iAknFepRepository)
        {
        iAknFepRepository->Set(KAknFepInputTxtLangSecondary, aValue);
        }

    // now inform all open apps of the switch
    // same event as primary input language change
    TWsEvent event;
    event.SetType(KEikInputLanguageChange);
    CEikonEnv::Static()->WsSession().SendEventToAllWindowGroups(event);
    }

#endif //FF_DUAL_LANGUAGE_SUPPORT
#endif

CAknFepSharedDataInterface::CAknFepSharedDataInterface(CAknFepManager* aFepManager)
    {
    iFepManager = aFepManager;
    }


void CAknFepSharedDataInterface::ConstructL()
    {
    TInt ret = KErrNone;

    TRAP(ret, iCommonEngineRepository = CRepository::NewL(KCRUidCommonEngineKeys));

    TRAP(ret, iAknFepRepository = CRepository::NewL(KCRUidAknFep));
    if (ret == KErrNone)
        {
        iAknFepRepositoryWatcher = CAknFepRepositoryWatcher::NewL(
            KCRUidAknFep,
            TCallBack(HandleRepositoryCallBack, this),
            iAknFepRepository);
        HandleHashKeySelectionChange();//we need read proactively before value changed,and we must sure the iAknFepRepository has been constructed
        }
	
	TRAP(ret, iLocaleRepository = CRepository::NewL( KCRUidLocaleSettings ));

    if (ret == KErrNone)
        {
        iAknFepLocaleRepositoryWatcher = CAknFepRepositoryWatcher::NewL(
            KCRUidLocaleSettings,
            TCallBack(HandleLocaleRepositoryCallBack, this),
            iLocaleRepository);
        }

    User::LeaveIfError(iQwertyModeStatusProperty.Attach(KCRUidAvkon, KAknQwertyInputModeActive));
#ifdef RD_INTELLIGENT_TEXT_INPUT
    //Attach the P&S property to the keyboardlayout and subscribe to the P&S framework
    // for any keyboard layout change.
    User::LeaveIfError(iKeyboardLayoutStatusProperty.Attach(KCRUidAvkon, KAknKeyBoardLayout));    
    iKeyboardLayoutStatusSubscriber = new (ELeave) CSubscriber(
                                         TCallBack(HandleKeyboardLayoutChangeNotification, this), 
                                         iKeyboardLayoutStatusProperty);    
    iKeyboardLayoutStatusSubscriber->SubscribeL();
#endif
    iQwertyModeStatusSubscriber = new (ELeave) CSubscriber(
    								TCallBack(QwertyModeChangeNotification, this), 
    								iQwertyModeStatusProperty);
    iQwertyModeStatusSubscriber->SubscribeL();

    // Read read-only shared data settings.
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepScrollLatinPredictive, iSharedDataScrollLatinPredictive);
        iAknFepRepository->Get(KAknFepSpaceWithScrollRight,  iSharedDataSpaceWithScrollRight);
        iAknFepRepository->Get(KAknFepEnterWithScrollDown,   iSharedDataEnterWithScrollDown);
        }
        
    TRAP(ret, iGsPenSettings = CRepository::NewL(KCRUidPersonalizationSettings));
    
    TRAP(ret, iSensorRepository = CRepository::NewL( KCRUidSensorSettings ) );
    }

TInt CAknFepSharedDataInterface::HashKeySelectionNotification(TAny* aObj)
    {
    if (aObj)
        {
        static_cast<CAknFepSharedDataInterface*>(aObj)->HandleHashKeySelectionChange();
        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
    }
    
void CAknFepSharedDataInterface::HandleHashKeySelectionChange()
    {
    TInt value(0);
    
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepHashKeySelection, value);
        }
    
    iHashKeySelectionInUse = value;
    }


TInt CAknFepSharedDataInterface::InputModeChangeGSNotification(TAny* aObj)
    {
    if (aObj)
        {
        static_cast<CAknFepSharedDataInterface*>(aObj)->HandleInputModeGSChange();
        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
    }

void CAknFepSharedDataInterface::HandleInputModeGSChange()
    {
    iFepManager->SetFlag(CAknFepManager::EFlagNewSharedDataInputMode);
    }

TInt CAknFepSharedDataInterface::InputTextLangGSNotification(TAny* aObj)
    {
    if (aObj)
        {
        static_cast<CAknFepSharedDataInterface*>(aObj)->HandleInputTextLanguageGSChange();
        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
    }

void CAknFepSharedDataInterface::HandleInputTextLanguageGSChange()
    {
    iFepManager->SetFlag(CAknFepManager::EFlagNewSharedDataInputLanguage);
    ESubVariantFlag subVariant = AknLayoutUtils::SubVariant();
    if((AknLayoutUtils::Variant() == EApacVariant)
        && ((subVariant == EPrcSubVariant)
        || (subVariant == EHongKongSubVariant)
        || (subVariant == ETaiwanSubVariant)))
        {
        iFepManager->SetFlag(CAknFepManager::EFlagNewSharedDataInputMode);
        }
    }


TInt CAknFepSharedDataInterface::PredictiveTextSettingChangeGSNotification(TAny* aObj)
    {
    if (aObj)
        {
        static_cast<CAknFepSharedDataInterface*>(aObj)->HandlePredictiveTextSettingGSChange();
        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
    }

TInt CAknFepSharedDataInterface::JapanesePredictiveTextSettingChangeGSNotification(TAny* aObj)
    {
    if (aObj)
        {
        static_cast<CAknFepSharedDataInterface*>(aObj)->HandleJapanesePredictiveTextSettingGSChange();
        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
    }

TInt CAknFepSharedDataInterface::QwertyModeChangeNotification(TAny* aObj)
    {
    if (aObj)
        {
        static_cast<CAknFepSharedDataInterface*>(aObj)->HandleQwertyModeChangeNotification();
        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
    }

void CAknFepSharedDataInterface::HandlePredictiveTextSettingGSChange()
    {
    TInt value = 0;
#ifdef RD_INTELLIGENT_TEXT_INPUT
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepPredTxtFlagExtension, value);
        }
    TPtiKeyboardType layout = (TPtiKeyboardType)KeyboardLayout();
    switch(layout)
        {
        case EPtiKeyboardNone:
            break;
      	case EPtiKeyboard12Key:
            value &= EKeyboardStyle12Key;
            break;
        case EPtiKeyboardHalfQwerty:
            value &= EKeyboardStyleHalfQwerty;
            break;            
      	case EPtiKeyboardQwerty4x12:
      	case EPtiKeyboardQwerty4x10:
      	case EPtiKeyboardQwerty3x11:      	
      	case EPtiKeyboardCustomQwerty:
            value &= EKeyboardStyleQwerty;
            break;
      	default:
      	    break;
        }
    if (value > 0)
    	{
    	value = 1;
    	}
#else
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepPredTxtFlag, value);
        }
#endif
    iFepManager->SetWesternPredictive(value);
    }

void CAknFepSharedDataInterface::HandleJapanesePredictiveTextSettingGSChange()
    {
    TInt value = 0;
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepJapanesePredTxtFlag, value);
        }
    iFepManager->SetJapanesePredictive(value);
    }

void CAknFepSharedDataInterface::HandleQwertyModeChangeNotification()
    {
    TInt value = 0;
    iQwertyModeStatusProperty.Get(value);
    iFepManager->SetQwertyMode(value);
    }

TInt CAknFepSharedDataInterface::NumberModeChangesGSNotification()
    {
    iFepManager->NumberModeChangeGSNotification();
    return KErrNone;
    }
#ifdef RD_INTELLIGENT_TEXT_INPUT

//
//Intensionally not called methods
// JapanesePredictiveTextSettingChangeGSNotification()
// MultiTapTimerChangeGSNotification()
// JapaneseQwertyFlagsChangeGSNotification()
// PredictiveTextAutoCompleteSettingChangeGSNotification() - Autocompletion feature
// 
TInt CAknFepSharedDataInterface::HandleGenericRepositoryChange(TAny* aPtr)
    {
    CAknFepSharedDataInterface *self = STATIC_CAST(CAknFepSharedDataInterface*, aPtr);
    TInt error = KErrNone;
    if(self->iAknFepRepositoryWatcher->ChangedKey() == KAknFepChineseInputMode)
        {
        error = InputModeChangeGSNotification(aPtr);
        }
    if(self->iAknFepRepositoryWatcher->ChangedKey() == KAknFepInputTxtLang)
        {
        error = InputTextLangGSNotification(aPtr);
        }
    if(self->iAknFepRepositoryWatcher->ChangedKey() == KAknFepPredTxtFlag)
        {
        error = PredictiveTextSettingChangeGSNotification(aPtr);
        }
    if(self->iAknFepRepositoryWatcher->ChangedKey() == KAknFepAutoCompleteFlag)
        {
        error = PredictiveTextAutoCompleteSettingChangeGSNotification(aPtr);
        }
    if(self->iAknFepRepositoryWatcher->ChangedKey() == KAknFepTypingCorrectionLevel)
        {
        error = PredictiveTextTypingCorrectionLevelChangeNotification(aPtr);
        }
    if(self->iAknFepRepositoryWatcher->ChangedKey() == KAknFepNumberCandidateFlag)
        {
        error = PredictiveTextNumberCandidateSettingChangeNotification(aPtr);
        }
    if(self->iAknFepRepositoryWatcher->ChangedKey() == KAknFepPrimaryCandidateFlag)
        {
        error = PredictiveTextPrimaryCandidateChangeNotification(aPtr);
        }

    return error;
    }
#endif
TInt CAknFepSharedDataInterface::HandleRepositoryCallBack(TAny* aPtr)
    {
    CAknFepSharedDataInterface *self = STATIC_CAST(CAknFepSharedDataInterface*, aPtr);
#ifdef RD_INTELLIGENT_TEXT_INPUT    
    if(self->iAknFepRepositoryWatcher->IsGenericCenrepChange())
        {
        return HandleGenericRepositoryChange(aPtr);
        }
    else
#endif        
    if (self->iAknFepRepositoryWatcher)
        {
		 if (self->iAknFepRepositoryWatcher->ChangedKey() == KAknFepHashKeySelection)
        	{
        	return HashKeySelectionNotification(aPtr);
        	}
        if (self->iAknFepRepositoryWatcher->ChangedKey() == KAknFepChineseInputMode)
            {
            return InputModeChangeGSNotification(aPtr);
            }
        else if (self->iAknFepRepositoryWatcher->ChangedKey() == KAknFepInputTxtLang)
            {
            return InputTextLangGSNotification(aPtr);
            }
#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifdef FF_DUAL_LANGUAGE_SUPPORT
        else if (self->iAknFepRepositoryWatcher->ChangedKey() == KAknFepInputTxtLangSecondary)
            {
            return InputTextLangGSNotification(aPtr);
            }
#endif //FF_DUAL_LANGUAGE_SUPPORT
#endif
        else if (self->iAknFepRepositoryWatcher->ChangedKey() == KAknFepPredTxtFlag)
            {
            return PredictiveTextSettingChangeGSNotification(aPtr);
            }
        else if (self->iAknFepRepositoryWatcher->ChangedKey() == KAknFepJapanesePredTxtFlag)
            {
            return JapanesePredictiveTextSettingChangeGSNotification(aPtr);
            }
        else if (self->iAknFepRepositoryWatcher->ChangedKey() == KAknFepMultiTapTimer)
            {
            return MultiTapTimerChangeGSNotification(aPtr);
            }
        else if (self->iAknFepRepositoryWatcher->ChangedKey() == KAknFepJapaneseSpecialCharFlag)
            {
            return JapaneseQwertyFlagsChangeGSNotification(aPtr);
            }
#ifndef RD_INTELLIGENT_TEXT_INPUT
        else if (self->iAknFepRepositoryWatcher->ChangedKey() == KAknFepAutoCompleteFlag)
            {
            return PredictiveTextAutoCompleteSettingChangeGSNotification(aPtr);
            }          
#endif	//RD_INTELLIGENT_TEXT_INPUT
#ifdef RD_INTELLIGENT_TEXT_INPUT
        else if (self->iAknFepRepositoryWatcher->ChangedKey() == KAknFepAutoCompleteFlag)
            {
            return PredictiveTextAutoCompleteSettingChangeGSNotification(aPtr);
            }          
        else if (self->iAknFepRepositoryWatcher->ChangedKey() == KAknFepTypingCorrectionLevel)
            {
            return PredictiveTextTypingCorrectionLevelChangeNotification(aPtr);
            }          
        else if (self->iAknFepRepositoryWatcher->ChangedKey() == KAknFepNumberCandidateFlag)
            {
            return PredictiveTextNumberCandidateSettingChangeNotification(aPtr);
            }
        else if (self->iAknFepRepositoryWatcher->ChangedKey() == KAknFepPrimaryCandidateFlag)
            {
            return PredictiveTextPrimaryCandidateChangeNotification(aPtr);
            }
        else if (self->iAknFepRepositoryWatcher->ChangedKey() == KAknFepPredTxtFlagExtension)
            {
            return PredictiveTextSettingChangeGSNotification(aPtr);
            }          
#endif
        }

    return KErrNone;
    }
    
TInt CAknFepSharedDataInterface::HandleLocaleRepositoryCallBack(TAny* aPtr)    
	{
	CAknFepSharedDataInterface *self = STATIC_CAST(CAknFepSharedDataInterface*, aPtr);

	if(self->iAknFepLocaleRepositoryWatcher)
	    {
        if (self->iAknFepLocaleRepositoryWatcher->ChangedKey() == KSettingsDefaultNumberMode
        ||  self->iAknFepLocaleRepositoryWatcher->ChangedKey() == KSettingsIndicDefaultNumberMode)
		    {
		    return self->NumberModeChangesGSNotification();
		    }
	    }    
	return KErrNone;	
	}

CAknFepSharedDataInterface::CSubscriber::CSubscriber(TCallBack aCallBack, RProperty& aProperty)
    :
    CActive(EPriorityNormal), iCallBack(aCallBack), iProperty(aProperty)
    {
    CActiveScheduler::Add(this);
    }

CAknFepSharedDataInterface::CSubscriber::~CSubscriber()
    {
    Cancel();
    }

void CAknFepSharedDataInterface::CSubscriber::SubscribeL()
    {
    if (!IsActive())
        {
        iProperty.Subscribe(iStatus);
        SetActive();
        }
    }

void CAknFepSharedDataInterface::CSubscriber::StopSubscribe()
    {
    Cancel();
    }

void CAknFepSharedDataInterface::CSubscriber::RunL()
    {
    if (iStatus.Int() == KErrNone)
        {
        iCallBack.CallBack();
        SubscribeL();
        }
    }

void CAknFepSharedDataInterface::CSubscriber::DoCancel()
    {
    iProperty.Cancel();
    }
    
    
TInt CAknFepSharedDataInterface::EditSubmenuInUse() const
    {
    TInt value = 0;				
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepHashKeySelection, value);
        }       
    
    return value;    
    }
    
    
    
TInt CAknFepSharedDataInterface::HashKeySelectionInUse()
    {
    // This is never used in qwerty modes.
    if (QwertyInputMode())
    	{
    	return 0;
    	}
    	    	
    TInt value = 0;     	    		    	
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepHashKeySelection, value);
        }        
 
    return iHashKeySelectionInUse;
    }   
    

CAknFepRepositoryWatcher* CAknFepRepositoryWatcher::NewL(
    const TUid aUid,
    const TUint32 aKey,
    CCenRepNotifyHandler::TCenRepKeyType aKeyType,
    TCallBack aCallBack,
    CRepository* aRepository)
    {
    CAknFepRepositoryWatcher* self = new(ELeave) CAknFepRepositoryWatcher(aUid, aKey, aCallBack, 
                                                                          aRepository);

    CleanupStack::PushL(self);
    self->ConstructL(aKeyType);
    CleanupStack::Pop(self);

    return self;
    }

CAknFepRepositoryWatcher* CAknFepRepositoryWatcher::NewL(
    const TUid aUid,
    TCallBack aCallBack,
    CRepository* aRepository)
    {
    CAknFepRepositoryWatcher* self = new(ELeave) CAknFepRepositoryWatcher(aUid, 
                           NCentralRepositoryConstants::KInvalidNotificationId, 
                           aCallBack, aRepository);

    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }

CAknFepRepositoryWatcher::~CAknFepRepositoryWatcher()
    {
    if( iNotifyHandler )
        {
        iNotifyHandler->StopListening();
        delete iNotifyHandler;
        iNotifyHandler = NULL;
        }
    }

CAknFepRepositoryWatcher::CAknFepRepositoryWatcher(
    const TUid aUid,
    const TUint32 aKey,
    TCallBack aCallBack,
    CRepository* aRepository)
    :
    iUid(aUid), iKey(aKey), iCallBack(aCallBack), iRepository(aRepository)
#ifdef RD_INTELLIGENT_TEXT_INPUT    
    , iIsGeneric (EFalse)
#endif    
    {
    }

void CAknFepRepositoryWatcher::ConstructL(CCenRepNotifyHandler::TCenRepKeyType aKeyType)
    {
    iNotifyHandler = CCenRepNotifyHandler::NewL(*this, *iRepository, aKeyType, iKey);
    iNotifyHandler->StartListeningL();
    }

void CAknFepRepositoryWatcher::ConstructL()
    {
    iNotifyHandler = CCenRepNotifyHandler::NewL(*this, *iRepository);
    iNotifyHandler->StartListeningL();
    }

TUint32 CAknFepRepositoryWatcher::ChangedKey()
    {
    return iChangedKey;
    }

void CAknFepRepositoryWatcher::HandleNotifyInt(TUint32 aKey, TInt aNewValue)
    {
#ifdef _DEBUG
    RDebug::Print(_L("AknFep: HandleNotifyInt aKey=%d, aNewValue=%d"), aKey, aNewValue);
#endif
    iChangedKey = aKey;
#ifdef RD_INTELLIGENT_TEXT_INPUT    
    iIsGeneric = EFalse;
#endif    
    iCallBack.CallBack();
    iChangedKey = NCentralRepositoryConstants::KInvalidNotificationId;
    }

void CAknFepRepositoryWatcher::HandleNotifyError(TUint32 /*aKey*/, TInt /*aError*/, CCenRepNotifyHandler* /*aHandler*/)
    {
    }

/*
#ifdef RD_INTELLIGENT_TEXT_INPUT
void CAknFepRepositoryWatcher::HandleNotifyGeneric(TUint32 aId)
    {
#ifdef _DEBUG
    RDebug::Print(_L("AknFep: HandleNotifyGeneric aId=%d"), aId);
#endif  
    iChangedKey = aId;
    iIsGeneric = ETrue;
    iCallBack.CallBack();      
    iChangedKey = NCentralRepositoryConstants::KInvalidNotificationId;
    }
#else*/

void CAknFepRepositoryWatcher::HandleNotifyGeneric(TUint32 aId)
    {
#ifdef _DEBUG
    RDebug::Print(_L("AknFep: HandleNotifyGeneric aId=%d"), aId);
#endif  
    iChangedKey = aId;
    iCallBack.CallBack();
    iChangedKey = NCentralRepositoryConstants::KInvalidNotificationId;  
    }
//#endif

/**
 * Multitap timer value. 
 * Replaces the constant KMultiTapTimeoutMS in Japanese variant.
 *
 * @since 3.0
 * @return Value of KAknFepMultiTapTimer setting.
 */
TInt CAknFepSharedDataInterface::MultiTapTimer() const
    {
    TInt value = 1000000;
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepMultiTapTimer, value);
        }
    return value;
    }
    
/**
 * Change notification for Multitap timer. 
 *
 * @since 3.0
 * @return return KErrNone if aObj isn't NULL
 *         return KErrArgument if aObj is NULL
 */
TInt CAknFepSharedDataInterface::MultiTapTimerChangeGSNotification(TAny* aObj)
    {
    if (aObj)
        {
        static_cast<CAknFepSharedDataInterface*>(aObj)->HandleMultiTapTimerChangeGSNotification();
        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
    }

/**
 * Handle change notification for Multitap timer. 
 *
 * @since 3.0
 */
void CAknFepSharedDataInterface::HandleMultiTapTimerChangeGSNotification()
    {
    TInt value = MultiTapTimer();
    iFepManager->SetMultiTapTimer(value);
    }

/**
 * Japanese Qwerty Flags value. 
 * Replaces the constant KAknFepJapaneseSpecialCharFlag in Japanese variant.
 *
 * @since 3.0
 * @return Value of KAknFepJapaneseSpecialCharFlag setting.
 */
TInt CAknFepSharedDataInterface::JapaneseQwertyFlags() const
    {
    TInt value = 0x000F;
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepJapaneseSpecialCharFlag, value);
        }
    return value;
    }
    
/**
 * Change notification for Japanese Qwerty Flags. 
 *
 * @since 3.0
 * @return return KErrNone if aObj isn't NULL
 *         return KErrArgument if aObj is NULL
 */
TInt CAknFepSharedDataInterface::JapaneseQwertyFlagsChangeGSNotification(TAny* aObj)
    {
    if (aObj)
        {
        static_cast<CAknFepSharedDataInterface*>(aObj)->HandleJapaneseQwertyFlagsChangeGSNotification();
        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
    }

/**
 * Handle change notification for Japanese Qwerty Flags. 
 *
 * @since 3.0
 */
void CAknFepSharedDataInterface::HandleJapaneseQwertyFlagsChangeGSNotification()
    {
    TInt value = JapaneseQwertyFlags();
    iFepManager->SetJapaneseQwertyFlags(value);
    }

/**
 * Returns a value of current clear direction.
 *
 * @since 3.2
 * @return Value of KAknFepClearDirection setting.
 */
TInt CAknFepSharedDataInterface::ClearDirection() const
    {
    TInt value = EClearDirectionLeft;
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepClearDirection, value);
        }
    return value;
    }

/**
 * Set a value of current clear direction.
 *
 * @since 3.2
 * @param aValue new value set to Clear Direction shared data item which
 *               represents the direction currently being used
 */
void CAknFepSharedDataInterface::SetClearDirection(TInt aValue)
    {
    if (iAknFepRepository)
        {
        iAknFepRepository->Set(KAknFepClearDirection, aValue);
        }
    }
#ifdef RD_INTELLIGENT_TEXT_INPUT
TInt CAknFepSharedDataInterface::HandleKeyboardLayoutChangeNotification(TAny* aObj)
    {
    if (aObj)
        {
        static_cast<CAknFepSharedDataInterface*>(aObj)->HandleKeyboardLayoutChange();
        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
    }
#endif
    
    
void CAknFepSharedDataInterface::SetPredictiveTextAutoCompleteOn()
    {
    if (iAknFepRepository)
        {
#ifdef RD_INTELLIGENT_TEXT_INPUT
        //Set autocomplete flag only for the current keyboard layout type. Otherwise,
        //by simply setting to "1", we los information on autocompletion settings
        //of other keyboard types.
        TPtiKeyboardType currentKeyboardLayoutlayout = (TPtiKeyboardType)KeyboardLayout();
        TInt autocompletionSetting = 0;
        iAknFepRepository->Get(KAknFepAutoCompleteFlag, autocompletionSetting);
        switch(currentKeyboardLayoutlayout)
            {
            case EPtiKeyboardNone:
                break;
            case EPtiKeyboard12Key:
                autocompletionSetting |= EKeyboardStyle12Key;
                break;
            case EPtiKeyboardQwerty4x12:
            case EPtiKeyboardQwerty4x10:
            case EPtiKeyboardQwerty3x11:
            case EPtiKeyboardHalfQwerty:
            case EPtiKeyboardCustomQwerty:
                autocompletionSetting |= EKeyboardStyleQwerty;
                break;
            default:
                break;
            }
        iAknFepRepository->Set(KAknFepAutoCompleteFlag, autocompletionSetting);
#else
        iAknFepRepository->Set(KAknFepAutoCompleteFlag, 1);
#endif
        }
    }    


void CAknFepSharedDataInterface::ResetPredictiveTextAutoCompleteOn()
    {
    if (iAknFepRepository)
        {
#ifdef RD_INTELLIGENT_TEXT_INPUT
        //Reset autocomplete flag only for the current keyboard layout type. Otherwise,
        //by simply setting to "0", we lose information on autocompletion settings
        //of other keyboard types.
        TPtiKeyboardType currentKeyboardLayoutlayout = (TPtiKeyboardType)KeyboardLayout();
        TInt autocompletionSetting = 0;
        iAknFepRepository->Get(KAknFepAutoCompleteFlag, autocompletionSetting);
        switch(currentKeyboardLayoutlayout)
            {
            case EPtiKeyboardNone:
                break;
            case EPtiKeyboard12Key:
                autocompletionSetting &= ~EKeyboardStyle12Key;
                break;
            case EPtiKeyboardQwerty4x12:
            case EPtiKeyboardQwerty4x10:
            case EPtiKeyboardQwerty3x11:
            case EPtiKeyboardHalfQwerty:
            case EPtiKeyboardCustomQwerty:
                autocompletionSetting &= ~EKeyboardStyleQwerty;
                break;
            default:
                break;
            }
        iAknFepRepository->Set(KAknFepAutoCompleteFlag, autocompletionSetting);
#else
        iAknFepRepository->Set(KAknFepAutoCompleteFlag, 0);
#endif
        }
    }
    
#ifndef RD_INTELLIGENT_TEXT_INPUT
void CAknFepSharedDataInterface::HandlePredictiveTextAutoCompleteSettingGSChange()
    {
    TInt value = 0;
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepAutoCompleteFlag, value);
        }
    iFepManager->SetAutoCompletionState(value);
    }
    
    
TInt CAknFepSharedDataInterface::PredictiveTextAutoCompleteSettingChangeGSNotification(TAny* aObj)
    {
    if (aObj)
        {
        static_cast<CAknFepSharedDataInterface*>(aObj)->HandlePredictiveTextAutoCompleteSettingGSChange();
        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
    }    

#endif	//RD_INTELLIGENT_TEXT_INPUT

#ifdef RD_INTELLIGENT_TEXT_INPUT
void CAknFepSharedDataInterface::SetFnKeyState(TInt aState)
    {
    RProperty::Set(KPSUidAknFep,KAknFepFnKeyState, aState);
    }
    
TInt CAknFepSharedDataInterface::GetFnKeyState()
    {
    TInt state = 0;
    RProperty::Get(KPSUidAknFep,KAknFepFnKeyState, state);
    return state;
    }
void CAknFepSharedDataInterface::HandleKeyboardLayoutChange()
    {
    TInt value = 0;
    TPtiKeyboardType keyboardType;
    iKeyboardLayoutStatusProperty.Get(value);
    keyboardType = (TPtiKeyboardType)value;
    iFepManager->SetKeyboardLayout(keyboardType);
    iFepManager->SetFnKeyMappingState();
#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
    iFepManager->StopDisplayingMenuBar();
#endif //__ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
    }
TInt CAknFepSharedDataInterface::PredictiveTextAutoCompleteOn()
    {
    TInt value = 0;
    if (iAknFepRepository)
        {
        TPtiKeyboardType layout = (TPtiKeyboardType)KeyboardLayout();
        TInt temp = 0;
        iAknFepRepository->Get(KAknFepAutoCompleteFlag, temp);
        switch(layout)
            {
            case EPtiKeyboardNone:
                break;
          	case EPtiKeyboard12Key:
                temp &= EKeyboardStyle12Key;
                break;
            case EPtiKeyboardHalfQwerty:
                temp &= EKeyboardStyleHalfQwerty;
                break;
          	case EPtiKeyboardQwerty4x12:
          	case EPtiKeyboardQwerty4x10:
          	case EPtiKeyboardQwerty3x11:          	
          	case EPtiKeyboardCustomQwerty:
                temp &= EKeyboardStyleQwerty;
                break;
          	default:
          	    break;
            }
        value = ( temp ? 1 : 0);
        }
    return value;
    }

void CAknFepSharedDataInterface::HandlePredictiveTextAutoCompleteSettingGSChange()
    {
    TInt value = 0;
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepAutoCompleteFlag, value);
        }
    TPtiKeyboardType layout = (TPtiKeyboardType)KeyboardLayout();
    switch(layout)
        {
        case EPtiKeyboardNone:
            break;
      	case EPtiKeyboard12Key:
            value &= EKeyboardStyle12Key;
            break;
        case EPtiKeyboardHalfQwerty:    
            value &= EKeyboardStyleHalfQwerty;
            break;
      	case EPtiKeyboardQwerty4x12:
      	case EPtiKeyboardQwerty4x10:
      	case EPtiKeyboardQwerty3x11:      	
      	case EPtiKeyboardCustomQwerty:
            value &= EKeyboardStyleQwerty;
            break;
      	default:
      	    break;
        }
    if (value > 0)
    	{
    	value = 1;
    	}
    iFepManager->SetAutoCompletionState(value);
    }
    
    
TInt CAknFepSharedDataInterface::PredictiveTextAutoCompleteSettingChangeGSNotification(TAny* aObj)
    {
    if (aObj) 
        {
        static_cast<CAknFepSharedDataInterface*>(aObj)->HandlePredictiveTextAutoCompleteSettingGSChange();
        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
    }    
/**
 * Queries the value of PredictiveText Typing Correction Level from shared data
 *
 * @return Typing Correction Level
*/
TInt CAknFepSharedDataInterface::PredictiveTextTypingCorrectionLevel()
	{
	TInt value = 0;
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepTypingCorrectionLevel, value);
        }
    return value;
	}


/**
 * Sets the value of PredictiveText Typing Correction Level state to aCorrectionLevel in shared data 
*/    
void CAknFepSharedDataInterface::SetPredictiveTextTypingCorrectionLevel(TInt aCorrectionLevel)
	{
	if (iAknFepRepository)
        {
        iAknFepRepository->Set(KAknFepTypingCorrectionLevel, aCorrectionLevel);
        }
	}

void CAknFepSharedDataInterface::HandlePredictiveTextTypingCorrectionLevelChange()
    {
    TInt value = 0;
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepTypingCorrectionLevel, value);
        }
    iFepManager->SetTypingCorrectionLevel(value);
    }

 /**
 * Handles a change in predictive text Typing Correction Level setting.
*/    
TInt CAknFepSharedDataInterface::PredictiveTextTypingCorrectionLevelChangeNotification(TAny* aObj)
	{
	if (aObj) 
        {
        static_cast<CAknFepSharedDataInterface*>(aObj)->HandlePredictiveTextTypingCorrectionLevelChange();
        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
	}

/**
 * Queries the value of PredictiveText Number Candidate Shown state to be On from shared data
 *
 * @return 1 for shown or 0 for not shown
*/
TInt CAknFepSharedDataInterface::PredictiveTextNumberCandidateShown()
	{
	TInt value = 0;
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepNumberCandidateFlag, value);
        }
    return value;
	}


/**
 * Sets the value of PredictiveText Number Candidate Shown state to 1 in shared data - this
 * is used to represent predictive text Number Candidate Shown being shown.
*/    
void CAknFepSharedDataInterface::SetPredictiveTextNumberCandidateShown()
	{
	if (iAknFepRepository)
        {
        iAknFepRepository->Set(KAknFepNumberCandidateFlag, 1);
        }
	}


/**
 * Sets the value of PredictiveText Number Candidate Shown state to 0 in shared data - this
 * is used to represent predictive text Number Candidate Shown being not shown.
*/    
void CAknFepSharedDataInterface::ResetPredictiveTextNumberCandidateShown()
	{
	if (iAknFepRepository)
        {
        iAknFepRepository->Set(KAknFepNumberCandidateFlag, 0);
        }
	}

void CAknFepSharedDataInterface::HandlePredictiveTextNumberCandidateSettingChange()
    {
    TInt value = 0;
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepNumberCandidateFlag, value);
        }
    iFepManager->SetNumberCandidateState(value);
    }

/**
 * Handles a change in predictive text Number Candidate Shown setting.
*/    
TInt CAknFepSharedDataInterface::PredictiveTextNumberCandidateSettingChangeNotification(TAny* aObj)
	{
	if (aObj) 
        {
        static_cast<CAknFepSharedDataInterface*>(aObj)->HandlePredictiveTextNumberCandidateSettingChange();
        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
	}
	
/**
* Queries the value of PredictiveText Primary candidate setting from shared data
*
* @return Primary candidate setting(0 for Suggested or 1 for Exact)
*/
TInt CAknFepSharedDataInterface::PredictiveTextPrimaryCandidate()
	{
	TInt value = 0;
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepPrimaryCandidateFlag, value);
        }
    return value;
	}


/**
* Sets the value of PredictiveText Primary candidate setting state to aPrimaryCandidate in shared data 
*/    
void CAknFepSharedDataInterface::SetPredictiveTextPrimaryCandidate(TInt aPrimaryCandidate)
	{
	if (iAknFepRepository)
        {
        iAknFepRepository->Set(KAknFepPrimaryCandidateFlag, aPrimaryCandidate);
        }
	}
/**
* Handles a change in predictive text Primary candidate setting.
*/   
void CAknFepSharedDataInterface::HandlePredictiveTextPrimaryCandidateChange()
    {
    TInt value = 0;
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepPrimaryCandidateFlag, value);
        }
    iFepManager->SetPrimaryCandidate(value);
    }

/**
* Handles a change in predictive text Typing Correction Level setting.
*/    
TInt CAknFepSharedDataInterface::PredictiveTextPrimaryCandidateChangeNotification(TAny* aObj)
	{
	if (aObj) 
        {
        static_cast<CAknFepSharedDataInterface*>(aObj)->HandlePredictiveTextPrimaryCandidateChange();
        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
	}
#else	// RD_INTELLIGENT_TEXT_INPUT

TInt CAknFepSharedDataInterface::PredictiveTextAutoCompleteOn()
    {
    TInt value = 0;
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepAutoCompleteFlag, value);
        }
    return value;
    }
#endif    
/**
 * Returns a value of HWR conversion on/off.
 *
 * @since 3.2
 * @return Value of KAknFepJapaneseHwrConversion setting.
 */
TInt CAknFepSharedDataInterface::JapaneseHwrConversion() const
    {
    TInt value = 0;
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepJapaneseSpecialCharFlag, value);
        }
    return (value & KJapHwrConversion)?
                EJapaneseConversionOn : EJapaneseConversionOff;
    }

/**
 * Set a value of HWR conversion on/off.
 *
 * @since 3.2
 * @param aValue new value set to HWR conversion on/off shared data item
 */
void CAknFepSharedDataInterface::SetJapaneseHwrConversion(TInt aValue)
    {
    if (iAknFepRepository)
        {
        TInt value;
        iAknFepRepository->Get(KAknFepJapaneseSpecialCharFlag, value);
        if (aValue == EJapaneseConversionOn)
            {
            value |= KJapHwrConversion;
            }
        else
            {
            value &= ~(KJapHwrConversion);
            }
        iAknFepRepository->Set(KAknFepJapaneseSpecialCharFlag, value);
        }
    }
    
TInt CAknFepSharedDataInterface::FepShowFsqPreviewStatus()
    {
    TInt value = 0;
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepShowFsqPreview, value);
        }
    return value;
    }
    
    
void CAknFepSharedDataInterface::SetFepShowFsqPreviewStatus(TInt aValue)
    {
    if (iAknFepRepository)
        {
        iAknFepRepository->Set(KAknFepShowFsqPreview, aValue);
        }
    } 
    
TInt CAknFepSharedDataInterface::FepShowVkbPreviewStatus()
    {
    TInt value = 0;
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepShowVkbPreview, value);
        }
    return value;
    }
    
    
void CAknFepSharedDataInterface::SetFepShowVkbPreviewStatus(TInt aValue)
    {
    if (iAknFepRepository)
        {
        iAknFepRepository->Set(KAknFepShowVkbPreview, aValue);
        }
    } 
    
TInt CAknFepSharedDataInterface::FepLastUseVkbModeForLandscape()
    {
    TInt value = 4;
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepLastUseVkbMode, value);
        }
    return value;
    }
    
    
void CAknFepSharedDataInterface::SetFepLastUseVkbModeForLandscape(TInt aValue)
    {
    if (iAknFepRepository)
        {
        iAknFepRepository->Set(KAknFepLastUseVkbMode, aValue);
        }
    }           

TBool CAknFepSharedDataInterface::AutoRotateEnabled()
	{
	TInt sensorState( 0 );    

    if (iSensorRepository)
        {
        iSensorRepository->Get( KSenSettingsSensorsOn, sensorState );
        }
    if ( sensorState )
        {
        TInt turnCtrl = 0;
        iSensorRepository->Get( KSenSettingsTurnCtrl, turnCtrl );
        turnCtrl = turnCtrl >> 2;
            
        if ( turnCtrl ) 
            {
            return ETrue;
            }
        }
    
    return EFalse;
	}       

#ifdef	RD_INTELLIGENT_TEXT_INPUT
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
/**
* Queries if numbers (and other characters found behind Fn) can be entered with long key press on QWERTY.
*/
TBool CAknFepSharedDataInterface::LongPressNumberEntryOnQwerty() const
    {
	TInt value = 0;
      if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepLongPressNumberEntryOnQwerty, value);
        }
    return value;
    }

/**
* Queries if copy-pasting with shift+softkeys is available also on QWERTY.
*/
TBool CAknFepSharedDataInterface::ShiftCopyPastingOnQwerty() const
    {
	TInt value = 0;
      if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepShiftCopyPasteOnQwerty, value);
        }
    return value;
    }
#endif // __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__	
#endif	// RD_INTELLIGENT_TEXT_INPUT

#ifdef  RD_SCALABLE_UI_V2
// ---------------------------------------------------------------------------
// Get a value of current default on screen vkb.
// It depends on input lanage is PRC/HW/TW Chinese.
// ---------------------------------------------------------------------------
//  
TInt CAknFepSharedDataInterface::DefaultOnScreenVKB()
    {
    TInt value = 0;
    switch( PenInputLanguage() )
        {
        case ELangPrcChinese:
            {
            iGsPenSettings->Get( KSettingsDefaultVkbPrc, value );
            }
            break;
        case ELangHongKongChinese:
            {
            iGsPenSettings->Get( KSettingsDefaultVkbHk, value );
            }
            break;
         case ELangTaiwanChinese:
            {
            iGsPenSettings->Get( KSettingsDefaultVkbTw, value );
            }
            break;
         default:
            break;
        }
    return value;
    };
#endif

// End of File
