/*
* Copyright (c) 2004-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Predective text input engine core local methods.
*
*/


#include <ecom/ecom.h>
#include <PtiCore.h>
#include "PtiUserDictionary.h"

const TUid KPtiCoreInterfaceUid = { 0x101F87FC };

//
// LOCAL METHODS
//

void Cleanup( TAny* aAny )
	{
	RImplInfoPtrArray* implArray = 
		reinterpret_cast< RImplInfoPtrArray*> ( aAny );
	implArray->ResetAndDestroy();
	implArray->Close();
	}

//
// CPtiCore
//
	
// ---------------------------------------------------------------------------
// CPtiCore::~CPtiCore
// 
// ---------------------------------------------------------------------------
// 	
EXPORT_C CPtiCore::~CPtiCore()
	{
	REComSession::DestroyedImplementation(TUid::Uid(iDtorKeyId));
	}

	
// ---------------------------------------------------------------------------
// CPtiCore::CreateCoreL
// 
// ---------------------------------------------------------------------------
// 	
EXPORT_C CPtiCore* CPtiCore::CreateCoreL(TUid aCoreUid)
	{
	TUid dtorIDKey;
	CPtiCore* iface = (CPtiCore*)REComSession::CreateImplementationL(aCoreUid, dtorIDKey);		
	if (iface)
		{
		iface->SetDestructorKeyId(dtorIDKey.iUid);
		}
	return iface;
	}	

	
// ---------------------------------------------------------------------------
// CPtiCore::ListCoresLC
// 
// ---------------------------------------------------------------------------
// 	
EXPORT_C CArrayFix<TInt>* CPtiCore::ListCoresLC()
	{
	RImplInfoPtrArray infoArray;
	TInt i;

	CArrayFixFlat<TInt>* result = new (ELeave) CArrayFixFlat<TInt>(KDefaultCoresGran);
	CleanupStack::PushL( result );

	CleanupStack::PushL( TCleanupItem( Cleanup, &infoArray ) );
	REComSession::ListImplementationsL(KPtiCoreInterfaceUid, infoArray);

	for (i = 0; i < infoArray.Count(); i++)
		{
		result->AppendL(infoArray[i]->ImplementationUid().iUid);
		}

	CleanupStack::PopAndDestroy(); // infoArray 

	return result;
	}

	
// ---------------------------------------------------------------------------
// CPtiCore::SetCurrentInputSequence
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiCore::SetCurrentInputSequence(TPtrC8 /*aInput*/)
	{
	return KErrNotSupported;
	}

	
// ---------------------------------------------------------------------------
// CPtiCore::Convert
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiCore::Convert(TPtiCharConversion /*aType*/,
								TAny* /*aInput*/,
							    TInt /*aInputLength*/,
							    TAny* /*aOutput*/)
	{
	return KErrNotSupported;	
	}


// ---------------------------------------------------------------------------
// CPtiCore::GetSpelling
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiCore::GetSpelling(TUint16 /*aInput*/,
			                        TDes& /*aOutput*/,
						            TPtiSpelling /*aType*/)
	{
	return KErrNotSupported;		
	}


// ---------------------------------------------------------------------------
// CPtiCore::SetExternalKeyMapL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiCore::SetExternalKeyMapL(TPtiEngineInputMode /*aMode*/,
	                                       TPtiKey /*aKey*/,
							               TDesC& /*aKeyMap*/,
								           TPtiTextCase /*aCase*/)
	{
	return KErrNotSupported;	
	}


// ---------------------------------------------------------------------------
// CPtiCore::CommitCurrentWord
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiCore::CommitCurrentWord()
	{
	return KErrNotSupported;
	}


// ---------------------------------------------------------------------------
// CPtiCore::GetCurrentWord
// 
// ---------------------------------------------------------------------------
//	
EXPORT_C TPtrC CPtiCore::GetCurrentWord() const
	{
	return TPtrC();
	}


// ---------------------------------------------------------------------------
// CPtiCore::NumberOfCandidates
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiCore::NumberOfCandidates()
	{
	return 0;
	}


// ---------------------------------------------------------------------------
// CPtiCore::GetCandidateListL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiCore::GetCandidateListL(CDesCArray& /*aList*/)
	{
	return KErrNotSupported;
	}


// ---------------------------------------------------------------------------
// CPtiCore::GetNextWordCandidateListL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiCore::GetNextWordCandidateListL(CDesCArray& /*aList*/)
	{
	return KErrNotSupported;
	}


// ---------------------------------------------------------------------------
// CPtiCore::GetFirstCandidate
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiCore::GetFirstCandidate(TDes& /*aResult*/)
	{
	return KErrNotSupported;
	}


// ---------------------------------------------------------------------------
// CPtiCore::GetNextCandidate
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiCore::GetNextCandidate(TDes& /*aResult*/)
	{
	return KErrNotSupported;
	}


// ---------------------------------------------------------------------------
// CPtiCore::GetPreviousCandidate
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiCore::GetPreviousCandidate(TDes& /*aResult*/)
	{
	return KErrNotSupported;
	}

	
// ---------------------------------------------------------------------------
// CPtiCore::AddUserDictionaryEntry
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiCore::AddUserDictionaryEntry(MPtiUserDictionaryEntry& /*aEntry*/, CPtiUserDictionary* /*aDict*/)
	{
	return KErrNotSupported;
	}


// ---------------------------------------------------------------------------
// CPtiCore::RemoveUserDictionaryEntry
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiCore::RemoveUserDictionaryEntry(MPtiUserDictionaryEntry& /*aEntry*/, CPtiUserDictionary* /*aDict*/)
	{
	return KErrNotSupported;
	}


// ---------------------------------------------------------------------------
// CPtiCore::NumberOfEntriesInUserDictionary
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiCore::NumberOfEntriesInUserDictionary(CPtiUserDictionary* /*aDict*/)
	{
	return 0;
	}


// ---------------------------------------------------------------------------
// CPtiCore::GetUserDictionaryEntry
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiCore::GetUserDictionaryEntry(TInt /*aIndex*/, MPtiUserDictionaryEntry& /*aResult*/, CPtiUserDictionary* /*aDict*/)
	{
	return KErrNotSupported;
	}


// ---------------------------------------------------------------------------
// CPtiCore::CreateEmptyUserDictionaryL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPtiUserDictionary* CPtiCore::CreateEmptyUserDictionaryL(TInt aMaxSize)
	{
	return CPtiUserDictionary::NewL(aMaxSize);		
	}


// ---------------------------------------------------------------------------
// CPtiCore::AttachUserDictionary
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiCore::AttachUserDictionary(CPtiUserDictionary* /*aDict*/)
	{
	return KErrNotSupported;
	}


// ---------------------------------------------------------------------------
// CPtiCore::DetachUserDictionary
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPtiCore::DetachUserDictionary(CPtiUserDictionary* /*aDict*/)		
	{
	return EFalse;
	}


// ---------------------------------------------------------------------------
// CPtiCore::GetCandidatePage
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TPtrC CPtiCore::GetCandidatePage()
	{
	return TPtrC();
	}


// ---------------------------------------------------------------------------
// CPtiCore::NextCandidatePage
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPtiCore::NextCandidatePage()
	{
	return EFalse;
	}


// ---------------------------------------------------------------------------
// CPtiCore::PreviousCandidatePage
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPtiCore::PreviousCandidatePage()
	{
	return EFalse;
	}


// ---------------------------------------------------------------------------
// CPtiCore::MoreCandidatePages
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPtiCore::MoreCandidatePages()
	{
	return EFalse;
	}


// ---------------------------------------------------------------------------
// CPtiCore::SetCandidatePageLength
// 
// ---------------------------------------------------------------------------
//
EXPORT_C void CPtiCore::SetCandidatePageLength(TInt /*aLength*/)
	{
	}


// ---------------------------------------------------------------------------
// CPtiCore::GetPhoneticSpelling
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TPtrC CPtiCore::GetPhoneticSpelling(TInt /*aIndex*/) const
 	{
	return TPtrC();
	}


// ---------------------------------------------------------------------------
// CPtiCore::PhoneticSpellingCount
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiCore::PhoneticSpellingCount() const
	{
	return 0;
	}


// ---------------------------------------------------------------------------
// CPtiCore::SelectPhoneticSpelling
// 
// ---------------------------------------------------------------------------
//	
EXPORT_C TBool CPtiCore::SelectPhoneticSpelling(TInt /*aIndex*/)
	{
	return EFalse;
	}


// ---------------------------------------------------------------------------
// CPtiCore::SelectedPhoneticSpelling
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiCore::SelectedPhoneticSpelling() const
 	{
	return KErrNotSupported;
	}


// ---------------------------------------------------------------------------
// CPtiCore::ToneMark
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPtiCore::ToneMark(TText& aToneMark) const
	{
	aToneMark = 0;
	return EFalse;
	}


// ---------------------------------------------------------------------------
// CPtiCore::IsToneMarkValidForSpelling
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPtiCore::IsToneMarkValidForSpelling()
	{
	return EFalse;
	}


// ---------------------------------------------------------------------------
// CPtiCore::SetPredictiveChineseChar
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPtiCore::SetPredictiveChineseChar(const TDesC& /*aChar*/)
	{
	return EFalse;
	}


// ---------------------------------------------------------------------------
// CPtiCore::SetToneMark
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPtiCore::SetToneMark(TInt /*aToneMark*/)
	{
	return EFalse;
	}


// ---------------------------------------------------------------------------
// CPtiCore::ReadingTextL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TPtrC CPtiCore::ReadingTextL()
	{
	return TPtrC();
	}


// ---------------------------------------------------------------------------
// CPtiCore::CompositionData
// 
// ---------------------------------------------------------------------------
//
EXPORT_C MPtiEngineCompositionDataInterface* CPtiCore::CompositionData()
	{
	return NULL;
	}


// ---------------------------------------------------------------------------
// CPtiCore::GetCandidatesByInputString
// 
// ---------------------------------------------------------------------------
//	
EXPORT_C HBufC* CPtiCore::GetCandidatesByInputString(const TDesC& /*aInputString*/, 
                                                     RPointerArray<HBufC>& /*aList*/, 
                                                     const TBool /*aIsPredictive*/)
    {
    return NULL;
    }
    
// End of file
