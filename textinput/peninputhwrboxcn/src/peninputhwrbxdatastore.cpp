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
* Description:  CAknFepHwrBxDataStore implementation
*
*/


#include <centralrepository.h>
#include <aknfeppeninputenums.h>
#include <AknFepGlobalEnums.h>

#include "peninputhwrbxdatastore.h"
#include <AknFepInternalCRKeys.h>
#include <settingsinternalcrkeys.h>
#include "peninputhwrtriggerstr.h"
#include "peninputhwrengine.h"
#include "peninputhwrboxgroup.h"
#include "peninputpluginutils.h"

const TInt KCandidateCount = 9;
const TInt KDefaultStrokeEndMarkX = -65536;
const TInt KDefaultStrokeEndMarkY = -65536;

_LIT(KCategory, "AKNFEPHWR");
_LIT( KCharSpace,  "\xF800" );
_LIT( KCharEnter, "\xF801" );
// ---------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------
//
CAknFepHwrBxDataStore* CAknFepHwrBxDataStore::NewL(CPtiEngine* aPtiEngine)
    {
    CAknFepHwrBxDataStore* self = new (ELeave) CAknFepHwrBxDataStore();
    
    CleanupStack::PushL(self);
    self->ConstructL(aPtiEngine);
    CleanupStack::Pop(self);//self

    return self;
    }

// ---------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------
//
CAknFepHwrBxDataStore::CAknFepHwrBxDataStore():iSizeChanging(EFalse)
    {
    iLanguage = ELangNone;
    }

// ---------------------------------------------------------
// destructor
// ---------------------------------------------------------
//
CAknFepHwrBxDataStore::~CAknFepHwrBxDataStore()
    {
    iSubscriberList.Close();
    
    iCandidates.ResetAndDestroy();
    iCandidates.Close();

    iPredictives.ResetAndDestroy();
    iPredictives.Close();
    
    iRangeStartPos.Close();
    
    iPermittedRanges.Close();

    delete iFepRepositoryWatcher;    
    delete iGSRepositoryWatcher;

    delete iRepositoryFep;
    delete iRepositorySetting;

    delete iTriggerStr;
    
    delete iHwrEngine;
    }

// ---------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::ConstructL(CPtiEngine* aPtiEngine)
    {
    iRepositoryFep = CRepository::NewL( KCRUidAknFep );
    iRepositorySetting = CRepository::NewL( KCRUidPersonalizationSettings );

    iFepRepositoryWatcher = CAknFepRepositoryWatcher::NewL(
        KCRUidPersonalizationSettings,
        TCallBack(HandleFepRepositoryCallBack, this),
        iRepositoryFep);

    iGSRepositoryWatcher = CAknFepRepositoryWatcher::NewL(
        KCRUidPersonalizationSettings,
        TCallBack(HandleGSRepositoryCallBack, this),
        iRepositorySetting);
    
    iTriggerStr = new (ELeave) CAknFepHwrTriggerStr();
    
    iHwrEngine = CAknFepHwrEngine::NewL(aPtiEngine, this);
    }

// ---------------------------------------------------------
// Is candidate in Chinese range
// ---------------------------------------------------------
//
TBool CAknFepHwrBxDataStore::IsChinese(TInt aCandidateIndex) const
    {
    TInt posIndex = ( iCandidates.Count()-1 ) - aCandidateIndex;
    TInt rangeCount = iPermittedRanges.Count();
    
    if ( rangeCount-1-posIndex > 0 )
        {
        return (iPermittedRanges[rangeCount-1-posIndex] == ERangeNative);    
        }
    else
        {
        return (iPermittedRanges[0] == ERangeNative);    
        }
    }
    
// ---------------------------------------------------------
// Do recoginize by engine
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::DoRecognizeL(const RArray<TPoint>& aTraceData)
    {
    if (iHwrEngine)
        {
        iHwrEngine->DoRecognizeL(aTraceData, iCandidates);
        }

    SetCandidates();
    }

// ---------------------------------------------------------
// Do predictive using trigger string
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::DoPredictiveL()
    {
    //because engine does not support trigger string
    //only the last char is used
    TPtrC string = TriggerStr()->GetString();

    if (iHwrEngine)
        {
        iHwrEngine->DoPredictiveL(string, iPredictives);
        }

    if (PredictiveCount() > 0)
        {
        SetPredictives();
        }
    }
    
// ---------------------------------------------------------
// Get the predictive count
// ---------------------------------------------------------
//
TInt CAknFepHwrBxDataStore::PredictiveCount() const
    {
    return iPredictives.Count();
    }

// ---------------------------------------------------------
// Get the candidate count
// ---------------------------------------------------------
//
TInt CAknFepHwrBxDataStore::CandidateCount() const
    {
    return iCandidates.Count();
    }

// ---------------------------------------------------------
// Add property subscriber
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::AddSubscriber(MAknFepHwrPropertySubscriber* aSubscriber)
    {
    if (aSubscriber)
        {
        const TInt index = iSubscriberList.Find(aSubscriber);
      
        if (index == KErrNotFound)
            {
            iSubscriberList.Append(aSubscriber);
            }
        }
    }

// ---------------------------------------------------------
// Get the trigger string
// ---------------------------------------------------------
//
CAknFepHwrTriggerStr* CAknFepHwrBxDataStore::TriggerStr() const
    {
    return iTriggerStr;
    }

// ---------------------------------------------------------
// Set permitted ranges
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::SetPermittedRanges(TInt aPermittedRanges)
    {
    iPermittedRanges.Reset();
    
    //order can not be changed
    if (aPermittedRanges & ERangeNative)
        {
        iPermittedRanges.Append(ERangeNative);
        }
    if (aPermittedRanges & ERangeEnglish)
        {
        iPermittedRanges.Append(ERangeEnglish);
        }
    if (aPermittedRanges & ERangeNumber)
        {
        iPermittedRanges.Append(ERangeNumber);
        }
    if (aPermittedRanges & ERangeSymbol)
        {
        iPermittedRanges.Append(ERangeSymbol);
        }
    
    iIntRange = aPermittedRanges;
    
    TPtrC ptr;
    ptr.Set(reinterpret_cast<TText*>(&iIntRange), sizeof(iIntRange));
    NotifySubscriber(MAknFepHwrPropertySubscriber::EAknFepHwrPropertyPermittedRange, ptr);        
    
    ptr.Set(reinterpret_cast<TText*>(&iPermittedRanges[0]), sizeof(iPermittedRanges[0]));
    NotifySubscriber(MAknFepHwrPropertySubscriber::EAknFepHwrPropertyRange, ptr);        
    
    // set primary and auxiliary ranges for hwr engine
    if (iHwrEngine)
        {
        iHwrEngine->SetRanges(iPermittedRanges);
        }
    }

// ---------------------------------------------------------
// Get the permitted ranges count
// ---------------------------------------------------------
//
TInt CAknFepHwrBxDataStore::PermittedRangesCount() const
    {
    return iPermittedRanges.Count();
    }

// ---------------------------------------------------------
// Set permitted cases
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::SetPermittedCases(TInt aPermittedCases)
    {
    iPermittedCases = aPermittedCases;
    TPtrC ptr;
    ptr.Set(reinterpret_cast<TText*>(&iPermittedCases), sizeof(iPermittedCases));
    NotifySubscriber(MAknFepHwrPropertySubscriber::EAknFepHwrPropertyPermittedCases, ptr);        
    }
    
TInt CAknFepHwrBxDataStore::PermittedCases()
    {
    return iPermittedCases;
    }
// ---------------------------------------------------------
// Set premary range
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::SetPremaryRange(TInt aPremaryRange)
    {
    const TInt oldIndex = iPermittedRanges.Find(aPremaryRange);

    if (oldIndex > 0)
        {
        iPermittedRanges.Sort();
        const TInt index = iPermittedRanges.Find(aPremaryRange);
        
        iPermittedRanges.Remove(index);
        iPermittedRanges.Insert(aPremaryRange, 0);
        
        TPtrC ptr;
        ptr.Set(reinterpret_cast<TText*>(&iPermittedRanges[0]), sizeof(iPermittedRanges[0]));
        NotifySubscriber(MAknFepHwrPropertySubscriber::EAknFepHwrPropertyRange, ptr);        
        
        // set primary and auxiliary ranges for hwr engine
        if (iHwrEngine)
            {
            iHwrEngine->SetRanges(iPermittedRanges);
            }
        }
    }

// ---------------------------------------------------------
// Set case
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::SetCase(TInt aCase)
    {              
    if ( iPermittedRanges.Count() != 0 && iPermittedRanges[0] != ERangeEnglish )
        {
        return;
        }
        
    iCase = aCase;

    TPtrC ptr;
    ptr.Set(reinterpret_cast<TText*>(&iCase), sizeof(iCase));
    NotifySubscriber(MAknFepHwrPropertySubscriber::EAknFepHwrPropertyCase, ptr);
    
    if (iHwrEngine)
        {
        iHwrEngine->SetCase(iCase);
        }
    }

// ---------------------------------------------------------
// Judge if a language is Chinese
// ---------------------------------------------------------
//
TBool CAknFepHwrBxDataStore::LanguageIsChinese(TInt aLanguage)
    {
	if ((aLanguage == ELangPrcChinese) ||
	    (aLanguage == ELangHongKongChinese) ||
	    (aLanguage == ELangTaiwanChinese))
	    {
		return ETrue;
	    }
	
	return EFalse;    
    }
// ---------------------------------------------------------
// Set real engine language
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::SetEngineLanguage(TInt aLanguage)
    {
    if (iHwrEngine)
        {
        TRAP_IGNORE(iHwrEngine->SetLanguageL(aLanguage));
        //iHwrEngine->SetPrimaryCandidateNum(KPremaryCandidateCount);
        iHwrEngine->SetCandidateNum(KCandidateCount);

        /*
        TPtrC ptr;            
        TPoint strokeEndMark = iHwrEngine->StrokeEndMark();
        ptr.Set(reinterpret_cast<TText*>(&strokeEndMark), sizeof(strokeEndMark));

        NotifySubscriber(MAknFepHwrPropertySubscriber::EAknFepHwrPropertyStrokeEndMark, ptr);
        */
        }
    }
    
// ---------------------------------------------------------
// Set stroke end mark
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::SetStrokeEndMark()
    {
    if (iHwrEngine)
        {
        TPtrC ptr;
        TPoint strokeEndMark = iHwrEngine->StrokeEndMark();
        ptr.Set(reinterpret_cast<TText*>(&strokeEndMark), sizeof(strokeEndMark));
        NotifySubscriber(MAknFepHwrPropertySubscriber::EAknFepHwrPropertyStrokeEndMark, ptr);
        }
    }
    
// ---------------------------------------------------------
// Set stroke end mark control
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::SetStrokeEndMarkControl(MAknFepHwrPropertySubscriber* aStrokeControl)
    {
    iStrokeControl = aStrokeControl;
    }
    
// ---------------------------------------------------------
// stroke end mark from control
// ---------------------------------------------------------
//
TPoint CAknFepHwrBxDataStore::StrokeEndMarkFromControl()
    {
	if (iStrokeControl)
	    {
		return iStrokeControl->StrokeEndMark();
	    }
	
	return TPoint(KDefaultStrokeEndMarkX, KDefaultStrokeEndMarkY);
    }

// ---------------------------------------------------------
// Set language
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::SetLanguageL(TInt aLanguage)
    {
    if( iLanguage == aLanguage )
        {
        return;
        }
        
    if (!LanguageIsChinese(aLanguage) && (aLanguage != ELangEnglish))
        {
        return;
        }

    if (aLanguage == ELangEnglish)
        {
        TLanguage displayLang = User::Language();
        
        // set real language to a Chinese
        if (!LanguageIsChinese(iLanguage))
            {
            // original language is not Chinese
            if (LanguageIsChinese(displayLang))
                {
            	iLanguage = displayLang;
                }
            else
                {
                iLanguage = ELangPrcChinese;	
                }
                
            SetEngineLanguage(iLanguage);
            }
        
        }
    else
        {
        // aLanguage is Chinese
        if (iLanguage != aLanguage)
            {
        	iLanguage = aLanguage;
            
            SetEngineLanguage(iLanguage);
            }
        }
        
    // make sure whole window is visible
    NotifySubscriber(MAknFepHwrPropertySubscriber::EAknFepHwrPropertySizeChanged, KNullDesC);
    }
    
// ---------------------------------------------------------
// Get language
// ---------------------------------------------------------
//
TInt CAknFepHwrBxDataStore::InputLanguage()
    {
    return iLanguage;
    }
    
// ---------------------------------------------------------
// Set writting speed
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::SetWritingSpeed(TInt aWritingSpeed)
    {
    TTimeIntervalMicroSeconds32 speed;
    switch (aWritingSpeed)
        {
        case EPenInputWritingSpeedVeryFast: 
            speed = EPenInputWritingCharDelayVeryFast;
            break;
            
        case EPenInputWritingSpeedFast: 
            speed = EPenInputWritingCharDelayFast;
            break;
            
        case EPenInputWritingSpeedNormal: 
            speed = EPenInputWritingCharDelayNormal;
            break;
            
        case EPenInputWritingSpeedSlow: 
            speed = EPenInputWritingCharDelaySlow;
            break;
            
        case EPenInputWritingSpeedVerySlow: 
            speed = EPenInputWritingCharDelayVerySlow;
            break;
            
        default:
            speed = EPenInputWritingCharDelayNormal;
            break;
        }
    
    TPtrC ptr;
    ptr.Set(reinterpret_cast<TText*>(&speed), sizeof(speed));
    NotifySubscriber(MAknFepHwrPropertySubscriber::EAknFepHwrPropertyWriteSpeed, ptr);        
    }

// ---------------------------------------------------------
// Set trail drawing pen size
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::SetBoxPenSize(const TSize& aSize)
    {
    iPenSize = aSize;

    TPtrC ptr;
    ptr.Set(reinterpret_cast<TText*>(&iPenSize), sizeof(iPenSize));
    NotifySubscriber(MAknFepHwrPropertySubscriber::EAknFepHwrPropertyPenSize, ptr);        
    }

// ---------------------------------------------------------
// Set trail drawing pen color
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::SetBoxPenColor(const TRgb& aCol)
    {
    iPenColor = aCol;

    TPtrC ptr;
    ptr.Set(reinterpret_cast<TText*>(&iPenColor), sizeof(iPenColor));
    NotifySubscriber(MAknFepHwrPropertySubscriber::EAknFepHwrPropertyPenColor, ptr);        
    }

// ---------------------------------------------------------
// Set the candidate list
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::SetCandidates()
    {
    ClearExtCandidate(); 
    TPtrC ptr;
    ptr.Set(reinterpret_cast<TText*>(&iCandidates), sizeof(iCandidates));
    NotifySubscriber(MAknFepHwrPropertySubscriber::EAknFepHwrPropertyCandidates, ptr);        
    }

// ---------------------------------------------------------
// Set the predictive list
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::SetPredictives()
    {
    TPtrC ptr;
    ptr.Set(reinterpret_cast<TText*>(&iPredictives), sizeof(iPredictives));
    NotifySubscriber(MAknFepHwrPropertySubscriber::EAknFepHwrPropertyPredictives, ptr);
    }
    
// ---------------------------------------------------------
// Add new predictives to list
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::AddPredictives()
    {
    TPtrC ptr;
    ptr.Set(reinterpret_cast<TText*>(&iPredictives), sizeof(iPredictives));
    NotifySubscriber(MAknFepHwrPropertySubscriber::EAknFepHwrPropertyAddPredictives, ptr);
    }

// ---------------------------------------------------------
// Get the candidate
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::GetCandidate(TInt aIndex, TDes& aCharCode) const
    {
    aCharCode.Zero();
    if (aIndex >=0 && aIndex < iCandidates.Count())
        {
        aCharCode = *iCandidates[aIndex];
        }
    }

// ---------------------------------------------------------
// Get the predictive
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::GetPredictive(TInt aIndex, TDes& aCharCode) const
    {
    aCharCode.Zero();
    if (aIndex >=0 && aIndex < iPredictives.Count())
        {
        aCharCode = *iPredictives[aIndex];
        }
    }

// ---------------------------------------------------------
// Clear the candidate list
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::ClearCandidates()
    {
    iCandidates.ResetAndDestroy();
    
    TPtrC ptr;
    ptr.Set(reinterpret_cast<TText*>(&iCandidates), sizeof(iCandidates));
    NotifySubscriber(MAknFepHwrPropertySubscriber::EAknFepHwrPropertyCandidates, ptr);        
    }

// ---------------------------------------------------------
// Notify subscriber
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::NotifySubscriber(
                              const MAknFepHwrPropertySubscriber::TAknFepHwrProperty aPropertyName,
                              const TDesC& aValue)
    {
    for (TInt i = 0; i < iSubscriberList.Count(); i++)
        {
        TRAP_IGNORE(iSubscriberList[i]->SetPropertyL(aPropertyName, aValue));
        }
    }

// ---------------------------------------------------------
// Callback method to notify repository change
// ---------------------------------------------------------
//
TInt CAknFepHwrBxDataStore::HandleGSRepositoryCallBack(TAny* aPtr)
    {
    CAknFepHwrBxDataStore *self = static_cast<CAknFepHwrBxDataStore*>(aPtr);
    
    CAknFepRepositoryWatcher* watcher = self->iGSRepositoryWatcher;
    if (watcher)
        {
        TInt newValue = self->GetNewValue(watcher, watcher->ChangedKey());
        
        switch (watcher->ChangedKey())
            {
            case KSettingsWritingSpeed:
                self->SetWritingSpeed(newValue);
                break;
            case KSettingsPenWidth:
                self->SetBoxPenSize(TSize(newValue,newValue));
                break;
            case KSettingsPenColor:
                self->SetBoxPenColor(newValue);
                break;
            default:
                break;
            }
        }
    return KErrNone;
    }

// ---------------------------------------------------------
// Callback method to notify repository change
// ---------------------------------------------------------
//
TInt CAknFepHwrBxDataStore::HandleFepRepositoryCallBack(TAny* aPtr)
    {
    CAknFepHwrBxDataStore *self = static_cast<CAknFepHwrBxDataStore*>(aPtr);
    
    CAknFepRepositoryWatcher* watcher = self->iFepRepositoryWatcher;
    if (watcher)
        {
        if (self->iSettingId == watcher->ChangedKey())
            {
            TPoint newValue = self->GetWindowPos();
            if (newValue != self->iWindowPosition)
                {
                self->NotifySubscriber(MAknFepHwrPropertySubscriber::EAknFepHwrPropertySizeChanged,
                                       KNullDesC);        
                self->iWindowPosition = newValue;
                }
            }
        }
    return KErrNone;
    }

// ---------------------------------------------------------
// Set number mode for hwr engine
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::SetNumberMode(const TAknEditorNumericKeymap aNumberMode)
    {
    if (iHwrEngine)
        {      
        iHwrEngine->SetNumberMode(aNumberMode);
        iHwrEngine->SetRanges(iPermittedRanges);
        }        
    }

// ---------------------------------------------------------
// Set number mode for hwr engine
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::SetNumberModeL(const TDesC& aCustomMode)
    {
    if (iHwrEngine)
        {
        iHwrEngine->SetCustomKeymapL(aCustomMode);
        }        
    }

TInt CAknFepHwrBxDataStore::GetPenTrailColor()
    {
    TInt newValue = 0; 
    iRepositorySetting->Get( KSettingsPenColor, newValue );
    
    if (!AknPenInputTrailColorUtils::CheckColorInColorTable(newValue))
        {
        return AknPenInputTrailColorUtils::GetTrailColorByTheme();
        }
    return newValue;
    }

// ---------------------------------------------------------
// LoadAndPublishDefault
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::LoadAndPublishDefaultL()
    {
    if (iRepositorySetting)
        {
        TInt newValue;

        iRepositorySetting->Get(KSettingsWritingSpeed, newValue);
        SetWritingSpeed(newValue);
        
        iRepositorySetting->Get(KSettingsPenWidth, newValue);
        TInt width = newValue;
        TInt height = newValue;
        SetBoxPenSize(TSize(width,height));

        newValue = GetPenTrailColor();
//        iRepositorySetting->Get(KSettingsPenColor, newValue);        
        SetBoxPenColor(newValue);
        
        iRepositorySetting->Get(KSettingsPenInputLang, newValue);
        SetLanguageL(newValue);
        }
    }

// ---------------------------------------------------------
// set screen mode
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::SetScreenMode(const TPixelsTwipsAndRotation& aScreenMode)
    {
    switch (aScreenMode.iRotation)
        {
        case CFbsBitGc::EGraphicsOrientationNormal:
            {
            iSettingId = KAknFepHwrWindowPosition0;
            break;
            }
        case CFbsBitGc::EGraphicsOrientationRotated90:
            {
            iSettingId = KAknFepHwrWindowPosition1;
            break;
            }
        case CFbsBitGc::EGraphicsOrientationRotated180:
            {
            iSettingId = KAknFepHwrWindowPosition2;
            break;
            }
        case CFbsBitGc::EGraphicsOrientationRotated270:
            {
            iSettingId = KAknFepHwrWindowPosition3;
            break;
            }
        default:
            User::Panic(KCategory, KErrArgument);
            break;
        }
    }

// ---------------------------------------------------------
// save window postion to repository
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::SaveWindowPos(const TPoint& aBr)
    {
    if (iRepositoryFep)
        {
        iWindowPosition = aBr;

        TInt newValue = aBr.iY;
        newValue = (newValue << 16) | (TInt16)aBr.iX;

        iRepositoryFep->Set(iSettingId, newValue);
        }
    }

// ---------------------------------------------------------
// save range to repository if english or chinese
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::SaveRange(TInt aRange)
    {
    if ( iRepositoryFep && (aRange == ERangeNative || aRange == ERangeEnglish) )
        {
        iRepositoryFep->Set(KAknFepLastUsedRange, aRange);
        }
    }

// ---------------------------------------------------------
// Get window position
// ---------------------------------------------------------
//
TPoint CAknFepHwrBxDataStore::GetWindowPos()
    {
    if (iRepositoryFep)
        {
        TInt pos;

        iRepositoryFep->Get(iSettingId, pos);
        
        TInt brX = (TInt16)(pos & 0x0000FFFF);
        TInt brY = (TInt16)( (pos & 0xFFFF0000) >>16);
        return (TPoint(brX, brY));
        }
    return TPoint(0,0);
    }

// ---------------------------------------------------------
// Get rep value
// ---------------------------------------------------------
//
TInt CAknFepHwrBxDataStore::GetNewValue(CAknFepRepositoryWatcher* aWatcher, TInt aId)
    {
    TInt newValue = -1;
    
    if (aWatcher == iFepRepositoryWatcher)
        {
        iRepositoryFep->Get(aId, newValue);
        }
    else if (aWatcher == iGSRepositoryWatcher)
        {
        iRepositorySetting->Get(aId, newValue);
        }
        
    return newValue;
    }

// ---------------------------------------------------------
// Get next predictive candidate page
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::GetNextPredictivePage()
    {
    if ( PredictiveCount()  >= KMaxPredictCandCnt ) 
        {
        return;
        }

    if (iHwrEngine)
        {
        TRAP_IGNORE(iHwrEngine->DoPredictiveL(KNullDesC, iPredictives, ETrue));
        }
                
    if (PredictiveCount() > 0)
        {
        if ( PredictiveCount() >= KMaxPredictCandCnt )
            {
            for (TInt i = KMaxPredictCandCnt; i < PredictiveCount(); i++)
                {
                delete iPredictives[KMaxPredictCandCnt];
                iPredictives.Remove(KMaxPredictCandCnt);
                }
            }
        
        AddPredictives();
        }
    }
    
TBool CAknFepHwrBxDataStore::PredictiveCandidateExisted()
    {
	if (iHwrEngine)
	    {
		return iHwrEngine->CandidateExisted();
	    }
	
	return EFalse;    
    }
    
// ---------------------------------------------------------
// Set flag to indicate whether size is changing
// ---------------------------------------------------------
//
void CAknFepHwrBxDataStore::SetSizeChanging(TBool aFlag)
	{
    iSizeChanging = aFlag;
	}

// ---------------------------------------------------------
// Get flag of whether size is changing
// ---------------------------------------------------------
//
TBool CAknFepHwrBxDataStore::SizeChanging()
	{
	return iSizeChanging;
	}


TBool CAknFepHwrBxDataStore::IsNumberOnly()
    {
    return ( (iPermittedRanges.Count()==1) && (iPermittedRanges[0]==ERangeNumber) );
    }

void CAknFepHwrBxDataStore::ClearExtCandidate()
    {
    if( IsNumberOnly() )
        {
        for( TInt val = 0; val < iCandidates.Count(); val++ )
            {
            if( *iCandidates[val] == KCharSpace 
                || *iCandidates[val] == KCharEnter  )
                {
                HBufC* candidate = iCandidates[val];
                iCandidates.Remove( val );
                delete candidate;
                } 
            }
        }
    }
TInt CAknFepHwrBxDataStore::SetInputAreaSize(TSize& aSize)
    {
    return iHwrEngine->SetInputAreaSize(aSize);    
    }    
TInt CAknFepHwrBxDataStore::SetScreenSize(TSize& aSize)
    {
    return iHwrEngine->SetScreenSize(aSize);    
    }    
// End Of File
