/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Inline methods for CPtiEngineImpl
*
*/



#include "PtiCore.h"
#include "PtiLanguage.h"

// ---------------------------------------------------------------------------
// CPtiEngineImpl::IsCoreForInputMode
// 
// ---------------------------------------------------------------------------
// 
inline TBool CPtiEngineImpl::IsCoreForInputMode(TPtiEngineInputMode aMode) const
	{
	if ((iCurrentLanguage) && (iCurrentLanguage->GetCore(aMode)))
		{
		return ETrue;
		}

	return EFalse;
	}

// ---------------------------------------------------------------------------
// CPtiEngineImpl::IsCore
// 
// ---------------------------------------------------------------------------
// 
inline TBool CPtiEngineImpl::IsCore() const
	{
	if ((iCurrentLanguage) && (iCurrentLanguage->GetCore(iInputMode)))
		{
		return ETrue;
		}

	return EFalse;
	}

// ---------------------------------------------------------------------------
// CPtiEngineImpl::Core
// 
// ---------------------------------------------------------------------------
//    
inline CPtiCore* CPtiEngineImpl::Core() const
	{
	return static_cast<CPtiCore*>(iCurrentLanguage->GetCore(iInputMode));
	}

// ---------------------------------------------------------------------------
// CPtiEngineImpl::CoreForInputMode
// 
// ---------------------------------------------------------------------------
//    
inline CPtiCore* CPtiEngineImpl::CoreForInputMode(TPtiEngineInputMode aMode) const
	{
	return static_cast<CPtiCore*>(iCurrentLanguage->GetCore(aMode));
	}

// ---------------------------------------------------------------------------
// CPtiEngineImpl::CurrentLanguage
// 
// ---------------------------------------------------------------------------
//    
inline MPtiLanguage* CPtiEngineImpl::CurrentLanguage() 
	{
	return iCurrentLanguage;
	}

// ---------------------------------------------------------------------------
// CPtiEngineImpl::CurrentWord
// 
// ---------------------------------------------------------------------------
//    
inline TPtrC CPtiEngineImpl::CurrentWord()
	{
	return iTextBuffer;	
	}

// ---------------------------------------------------------------------------
// CPtiEngineImpl::InputMode
// 
// ---------------------------------------------------------------------------
//    
inline TPtiEngineInputMode CPtiEngineImpl::InputMode() const
	{
	return iInputMode;
	}

// ---------------------------------------------------------------------------
// CPtiEngineImpl::SetCase
// 
// ---------------------------------------------------------------------------
//    
inline void CPtiEngineImpl::SetCase(TPtiTextCase aCase)
	{
	iCase = aCase;
	}

// ---------------------------------------------------------------------------
// CPtiEngineImpl::Case
// 
// ---------------------------------------------------------------------------
//    
inline TPtiTextCase CPtiEngineImpl::Case() const
	{
	return iCase;
	}

// ---------------------------------------------------------------------------
// CPtiEngineImpl::NumberOfLanguages
// 
// ---------------------------------------------------------------------------
//    
inline TInt CPtiEngineImpl::NumberOfLanguages() const
	{
	// iLanguages is always available if CPtiEngineImpl::NewL succeeded,
	// so this can be done safely.
	return iLanguages->Count() - 1;  // Remove Numeric from count.
	}

// ---------------------------------------------------------------------------
// CPtiEngineImpl::LastEnteredKey
// 
// ---------------------------------------------------------------------------
//    
inline TPtiKey CPtiEngineImpl::LastEnteredKey() const
	{
	if (iInputSequence.Length() > 0)
		{
		return (TPtiKey)iInputSequence[iInputSequence.Length() - 1];
		}
	return EPtiKeyNone;
	}

// ---------------------------------------------------------------------------
// CPtiEngineImpl::CurrentInputSequence
// 
// ---------------------------------------------------------------------------
//    
inline TPtrC8 CPtiEngineImpl::CurrentInputSequence() const
	{
	return TPtrC8(iInputSequence);
	}

// ---------------------------------------------------------------------------
// CPtiEngineImpl::SetObserver
// 
// ---------------------------------------------------------------------------
//    
inline void CPtiEngineImpl::SetObserver(MPtiObserver* aObserver)
	{
	iObserver = aObserver;
	}

// ---------------------------------------------------------------------------
// CPtiEngineImpl::Observer
// 
// ---------------------------------------------------------------------------
//    
inline MPtiObserver* CPtiEngineImpl::Observer()
	{
	return iObserver;
	}

// ---------------------------------------------------------------------------
// CPtiEngineImpl::ClearBuffers
// 
// ---------------------------------------------------------------------------
//    
inline void CPtiEngineImpl::ClearBuffers()
	{
	iInputSequence.Zero();
	iTextBuffer.Zero();
	iCaseBuffer.Zero();
	}

// ---------------------------------------------------------------------------
// CPtiEngineImpl::IsChinese
// 
// ---------------------------------------------------------------------------
//    
inline TBool CPtiEngineImpl::IsChinese() const
	{
	if ((iCurrentLanguage) && ((iCurrentLanguage->LanguageCode() == ELangPrcChinese) ||
		(iCurrentLanguage->LanguageCode() == ELangTaiwanChinese) ||
		(iCurrentLanguage->LanguageCode() == ELangHongKongChinese)))
		{
		return ETrue;
		}

	return EFalse;
	}

// ---------------------------------------------------------------------------
// CPtiEngineImpl::IsToneMarkOrSeparator
// 
// ---------------------------------------------------------------------------
//    
inline TBool CPtiEngineImpl::IsToneMarkOrSeparator(TInt aChar) const
	{
	if (iInputMode == EPtiEngineZhuyinByPhrase || 
	        iInputMode == EPtiEngineZhuyinPhraseHalfQwerty)
		{
		if ((aChar == KPtiEngineChineseSeparator) ||
			(aChar == KPtiZhuyinTone0) ||
			(aChar == KPtiZhuyinTone1) ||
			(aChar == KPtiZhuyinTone2) ||
			(aChar == KPtiZhuyinTone3) ||
			(aChar == KPtiZhuyinTone4))
			{
			return ETrue;
			}
		}
	else
		{
		if ((aChar == KPtiEngineChineseSeparator) ||
			(aChar == KPtiPinyinTone0) ||
			(aChar == KPtiPinyinTone1) ||
			(aChar == KPtiPinyinTone2) ||
			(aChar == KPtiPinyinTone3) ||
			(aChar == KPtiPinyinTone4))
			{
			return ETrue;
			}
		}
		
	return EFalse;
	}

// ---------------------------------------------------------------------------
// CPtiEngineImpl::SupportsUserDictionaries
// 
// ---------------------------------------------------------------------------
//    
inline TBool CPtiEngineImpl::SupportsUserDictionaries(CPtiCore* aCore)
	{
	if (aCore)
		{
		MPtiCoreInfo *info = aCore->GetCoreInfo();
		if (info &&	(info->CapsBits() & ESupportUserDictionaries))
			{
			return ETrue;
			}
		}

	return EFalse;
	}
			
// ---------------------------------------------------------------------------
// CPtiEngineImpl::IsModeChineseQwerty
// 
// ---------------------------------------------------------------------------
// 		
inline TBool CPtiEngineImpl::IsModeChineseQwerty(TPtiEngineInputMode aMode) const
	{
	if ((aMode == EPtiEnginePinyinQwerty) ||	
		(aMode == EPtiEnginePinyinPhraseQwerty) ||
		(aMode == EPtiEngineStrokeQwerty) ||
		(aMode == EPtiEngineStrokePhraseQwerty) ||		
		(aMode == EPtiEngineZhuyinQwerty) ||
		(aMode == EPtiEngineZhuyinPhraseQwerty) ||
		(aMode == EPtiEngineNormalCangjieQwerty) ||
        (aMode == EPtiEngineEasyCangjieQwerty) ||
        (aMode == EPtiEngineInputModeZhuyinMiniQwertyFind)||
		(aMode == EPtiEngineAdvCangjieQwerty)||
		(aMode == EPtiEnginePinyinPhraseHalfQwerty)||
		(aMode == EPtiEngineStrokePhraseHalfQwerty)||
		(aMode == EPtiEngineZhuyinPhraseHalfQwerty))  				
		{
		return ETrue;	
		}
		
	return EFalse;	
	}
		
// ---------------------------------------------------------------------------
// CPtiEngineImpl::AllowCapitalize
// 
// ---------------------------------------------------------------------------
// 		
inline TBool CPtiEngineImpl::AllowCapitalize(TUint16 aChar) const
	{
	if (iInputMode == EPtiEngineQwerty)
		{
		// Following lower case characters are behind shift in some 
		// keymappings and should not be capitalized by PtiEngine.
		if (Case() == EPtiCaseUpper &&
		   (aChar == 0x00e9 || aChar == 0x00f9 || aChar == 0x00ec))		
			{
			return EFalse;
			}				
		}

	return ETrue;	
	}	
	

// ---------------------------------------------------------------------------
// CPtiEngineImpl::CaseSequence
// 
// ---------------------------------------------------------------------------
// 		
inline TPtrC8 CPtiEngineImpl::CaseSequence()
    {
    return iCaseBuffer;	    
    }
		
// End of file
