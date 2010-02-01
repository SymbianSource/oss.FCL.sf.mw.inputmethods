// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//


#include <fepbconfig.h>
#include <fepbpanic.h>
#include <centralrepository.h>
#include <coedef.h>		// KUidFepFrameworkRepository
#include <s32strm.h>
#include <s32mem.h>
#include "FEPBPRIV.H"
#include <graphics/cone/coedefkeys.h>

LOCAL_C void Panic(TFepBasePanic aPanic)
	{
	User::Panic(KLitFepBasePanicText, aPanic);
	}
//
// TFepOnOrOffKeyData
//

EXPORT_C TBool TFepOnOrOffKeyData::operator==(const TFepOnOrOffKeyData& aAnother) const
	{
	return (iCharacterCodeForFoldedMatch==aAnother.iCharacterCodeForFoldedMatch) &&
		   (iModifierMask==aAnother.iModifierMask) &&
		   (iModifierValues==aAnother.iModifierValues);
	}

EXPORT_C TBool TFepOnOrOffKeyData::operator!=(const TFepOnOrOffKeyData& aAnother) const
	{
	return !operator==(aAnother);
	}

//
// CFepGenericGlobalSettings
//

/** Allocates and constructs a new generic FEP settings object. 

Reads the generic FEP data from the system settings. If the data is not 
present in the system settings, it is read from the default values specified.

@deprecated
@param aConeEnvironment The FEP's control environment.
@param aDefaultOnKeyData Default key event data for switching the FEP on.
@param aDefaultOffKeyData Default key event data for switching the FEP off.
@param aDefaultIsOn Whether the FEP is on or off by default.
@return Pointer to the newly created object. */
EXPORT_C CFepGenericGlobalSettings* CFepGenericGlobalSettings::NewL(CCoeEnv& aConeEnvironment, const TFepOnOrOffKeyData& aDefaultOnKeyData, const TFepOnOrOffKeyData& aDefaultOffKeyData, TBool aDefaultIsOn)
	{
	CFepGenericGlobalSettings* genericGlobalSettings=NewLC(aConeEnvironment, aDefaultOnKeyData, aDefaultOffKeyData, aDefaultIsOn);
	CleanupStack::Pop(); // genericGlobalSettings
	return genericGlobalSettings;
	}

/** Allocates and constructs a new generic FEP settings object. 

Reads the generic FEP data from the system settings. If the data is not 
present in the system settings, it is read from the default values specified. 
Leaves a pointer to the object on the cleanup stack.

@deprecated
@param aConeEnvironment The FEP's control environment.
@param aDefaultOnKeyData Default key event data for switching the FEP on.
@param aDefaultOffKeyData Default key event data for switching the FEP off.
@param aDefaultIsOn Whether the FEP is on or off by default.
@return Pointer to the newly created object. */
EXPORT_C CFepGenericGlobalSettings* CFepGenericGlobalSettings::NewLC(CCoeEnv& /*aConeEnvironment*/, const TFepOnOrOffKeyData& /*aDefaultOnKeyData*/, const TFepOnOrOffKeyData& /*aDefaultOffKeyData*/, TBool /*aDefaultIsOn*/)
	{
	CFepGenericGlobalSettings* genericGlobalSettings=new(ELeave) CFepGenericGlobalSettings;
	CleanupStack::PushL(genericGlobalSettings);
	genericGlobalSettings->ConstructL();
	return genericGlobalSettings;
	}

/** Allocates and constructs a new generic FEP settings object. 

Reads the generic FEP data from the system settings.

@return Pointer to the newly created object. */
EXPORT_C CFepGenericGlobalSettings* CFepGenericGlobalSettings::NewL()
	{
	CFepGenericGlobalSettings* const genericGlobalSettings=NewLC();
	CleanupStack::Pop(genericGlobalSettings);
	return genericGlobalSettings;
	}

/** Allocates and constructs a new generic FEP settings object. 

Reads the generic FEP data from the system settings. 
Leaves a pointer to the object on the cleanup stack.

@return Pointer to the newly created object. */
EXPORT_C CFepGenericGlobalSettings* CFepGenericGlobalSettings::NewLC()
	{
	CFepGenericGlobalSettings* const genericGlobalSettings=new(ELeave) CFepGenericGlobalSettings;
	CleanupStack::PushL(genericGlobalSettings);
	genericGlobalSettings->ConstructL();
	return genericGlobalSettings;
	}

/** Gets the key event data which turns the FEP on.

@return The key event data which turns the FEP on. */
EXPORT_C TFepOnOrOffKeyData CFepGenericGlobalSettings::OnKeyData() const
	{
	return iOnKeyData;
	}

/** Sets the key event data which turns the FEP on.

@param aOnKeyData The key event data which turns the FEP on. */
EXPORT_C void CFepGenericGlobalSettings::SetOnKeyData(const TFepOnOrOffKeyData& aOnKeyData)
	{
	iOnKeyData=aOnKeyData;
	iFlags|=EFlagStoreOnKeyData;
	}

/** Gets the key event data which turns the FEP off.

@return The key event data which turns the FEP off. */
EXPORT_C TFepOnOrOffKeyData CFepGenericGlobalSettings::OffKeyData() const
	{
	return iOffKeyData;
	}

/** Sets the key event data which turns the FEP off.

@param aOffKeyData The key event data which turns the FEP off. */
EXPORT_C void CFepGenericGlobalSettings::SetOffKeyData(const TFepOnOrOffKeyData& aOffKeyData)
	{
	iOffKeyData=aOffKeyData;
	iFlags|=EFlagStoreOffKeyData;
	}

/** Tests whether the FEP is on or off.

@return ETrue if the FEP is on, EFalse if the FEP is off. */
EXPORT_C TBool CFepGenericGlobalSettings::IsOn() const
	{
	return iFlags&EFlagIsOn;
	}

/** Sets a flag indicating whether the FEP is on or off.

@param aIsOn ETrue if the FEP is on, EFalse if the FEP is off. */
EXPORT_C void CFepGenericGlobalSettings::SetIsOn(TBool aIsOn)
	{
	if (aIsOn)
		iFlags|=EFlagIsOn;
	else
		iFlags&=~EFlagIsOn;

	iFlags|=EFlagStoreIsOn;
	}

/** Stores the local copies of the generic FEP data as the system settings. 

Causes all other running instances of the FEP to be updated with the new settings.
@capability WriteDeviceData		To prevent corruption of FEP settings.
*/
EXPORT_C void CFepGenericGlobalSettings::StoreChangesAndBroadcastL()
	{
	if (iFlags&(EFlagStoreIsOn|EFlagStoreOnKeyData|EFlagStoreOffKeyData))
		{
		CRepository* const repository=CRepository::NewLC(TUid::Uid(KUidFepFrameworkRepository));
		TInt error=KErrNone;
		do
			{
			User::LeaveIfError(repository->StartTransaction(CRepository::EConcurrentReadWriteTransaction));
			repository->CleanupCancelTransactionPushL();
			if (iFlags&EFlagStoreIsOn)
				WriteOnStateAndBroadcastL(*repository, iFlags&EFlagIsOn, ERepositoryKeyMask_DynamicSetting);

			if (iFlags&EFlagStoreOnKeyData)
				WriteOnOrOffKeyDataAndBroadcastL(*repository, iOnKeyData, ERepositoryKey_DynamicOnKeyData);

			if (iFlags&EFlagStoreOffKeyData)
				WriteOnOrOffKeyDataAndBroadcastL(*repository, iOffKeyData, ERepositoryKey_DynamicOffKeyData);

			CleanupStack::Pop(); // repository->CleanupCancelTransactionPushL
			TUint32 notUsed=0;
			error=repository->CommitTransaction(notUsed);
			} 
		while (error==KErrLocked);

		User::LeaveIfError(error);
		iFlags&=~(EFlagStoreIsOn|EFlagStoreOnKeyData|EFlagStoreOffKeyData);
		CleanupStack::PopAndDestroy(repository);
		}
	}

/** Reads the generic FEP data from the system settings. 

If the FEP data is not present in the system settings, then it is set from 
the default values (as passed to the NewL()).

This function is called during construction of the object. */
EXPORT_C void CFepGenericGlobalSettings::RefreshL()
	{
	TBool onState=EFalse;
	TFepOnOrOffKeyData onKeyData(0, 0, 0);
	TFepOnOrOffKeyData offKeyData(0, 0, 0);

	CRepository* const repository=CRepository::NewLC(TUid::Uid(KUidFepFrameworkRepository));
	TInt error=KErrNone;
	do
		{
		User::LeaveIfError(repository->StartTransaction(CRepository::EConcurrentReadWriteTransaction));
		repository->CleanupCancelTransactionPushL();
		ReadOnState(*repository, onState, &error);
		if (error==KErrNone)
			ReadOnOrOffKeyData(*repository, onKeyData, ERepositoryKeyMask_OnKeyData, &error);

		if (error==KErrNone)
			ReadOnOrOffKeyData(*repository, offKeyData, ERepositoryKeyMask_OffKeyData, &error);

		CleanupStack::PopAndDestroy(); // repository->CleanupCancelTransactionPushL
		} 
	while (error==KErrAbort);
	
	if (error!=KErrNotFound)
		User::LeaveIfError(error);

	CleanupStack::PopAndDestroy(repository);

	// everything leaving has now succeeded, so assign member-objects
	if (onState)
		iFlags|=EFlagIsOn;
	else
		iFlags&=~EFlagIsOn;

	iOnKeyData=onKeyData;
	iOffKeyData=offKeyData;
	iFlags&=~(EFlagStoreIsOn|EFlagStoreOnKeyData|EFlagStoreOffKeyData);
	}

/** @internalTechnology */
EXPORT_C void CFepGenericGlobalSettings::ReadOnState(CRepository& aRepository, TBool& aOnState, TInt* aError/*=NULL*/)
	{ // static
	TInt error=aRepository.Get(ERepositoryKey_DynamicOnState, aOnState);
	if (error!=KErrNone)
		error=aRepository.Get(ERepositoryKey_DefaultOnState, aOnState);

	if (aError!=NULL)
		*aError=error;
	}

/** @internalTechnology */
EXPORT_C void CFepGenericGlobalSettings::ReadOnOrOffKeyData(CRepository& aRepository, TFepOnOrOffKeyData& aOnOrOffKeyData, TUint32 aRepositoryKeyMask_OnOrOff, TInt* aError/*=NULL*/)
	{ // static
	TPckg<TFepOnOrOffKeyData> onOrOffKeyData(aOnOrOffKeyData);
	TInt error=aRepository.Get((ERepositoryKeyMask_DynamicSetting|aRepositoryKeyMask_OnOrOff), onOrOffKeyData);
	if (error!=KErrNone)
		error=aRepository.Get((ERepositoryKeyMask_DefaultSetting|aRepositoryKeyMask_OnOrOff), onOrOffKeyData);

	if (aError!=NULL)
		*aError=error;
	}

/** @internalTechnology */
EXPORT_C void CFepGenericGlobalSettings::WriteOnStateAndBroadcastL(CRepository& aRepository, TBool aOnState, TUint32 aRepositoryKeyMask_DefaultOrDynamic)
	{ // static
	const TUint32 repositoryKey=(aRepositoryKeyMask_DefaultOrDynamic|ERepositoryKeyMask_OnState);
	User::LeaveIfError(aRepository.Set(repositoryKey, aOnState));
	}

/** @internalTechnology */
EXPORT_C void CFepGenericGlobalSettings::WriteOnOrOffKeyDataAndBroadcastL(CRepository& aRepository, const TFepOnOrOffKeyData& aOnOrOffKeyData, TUint32 aRepositoryKey)
	{ // static
	const TPckgC<TFepOnOrOffKeyData> onOrOffKeyData(aOnOrOffKeyData);
	User::LeaveIfError(aRepository.Set(aRepositoryKey, onOrOffKeyData));
	}

CFepGenericGlobalSettings::CFepGenericGlobalSettings()
	:iFlags(0),
	 iOnKeyData(0, 0, 0),
	 iOffKeyData(0, 0, 0)
	{
	}

void CFepGenericGlobalSettings::ConstructL()
	{
	RefreshL();
	}


//
// MFepAttributeStorer
//

/** Reads all of the FEP's attributes from the system settings. 

Calls the implementation of ReadAttributeDataFromStreamL() for each attribute.

This function has identical behaviour to CCoeFep::ReadAllAttributesL().

@param aConeEnvironment Not used. */
EXPORT_C void MFepAttributeStorer::ReadAllAttributesL(CCoeEnv& /*aConeEnvironment*/)
	{
	CRepository* const repository=CRepository::NewLC(TUid::Uid(KUidFepSpecificSettingsRepository));
	RBuf8 buffer;
	CleanupClosePushL(buffer);
	buffer.CreateL(NCentralRepositoryConstants::KMaxBinaryLength);
	User::LeaveIfError(repository->StartTransaction(CRepository::EConcurrentReadWriteTransaction)); // a transaction ensures that a consistent set of values is read
	repository->CleanupCancelTransactionPushL();

	const TInt numberOfAttributes=NumberOfAttributes();
	for (TInt i=numberOfAttributes-1; i>=0; --i)
		{
		const TUid attributeUid(AttributeAtIndex(i));
		const TInt error=repository->Get(attributeUid.iUid, buffer);
		if (error==KErrAbort)
			{
			// cause the loop to start all over again, since some other process has done a commit whilst we've been inside this transaction
			i=numberOfAttributes;
			continue;
			}

		if (error==KErrNone)
			{
			RDesReadStream readStream(buffer);
			ReadAttributeDataFromStreamL(attributeUid, readStream);
			}
		}
	CleanupStack::PopAndDestroy(3, repository);
	}

/** Call this function after changing the value of an attribute that needs to be 
synchronised. 

Writes the new value to the system-wide settings and causes 
all other running instances of the FEP to be notified of the change.

A panic occurs if aAttributeUid is not one of the FEP's attributes.

@param aConeEnvironment The FEP's control environment.
@param aAttributeUid The UID of the attribute whose value has changed.
@capability WriteDeviceData */
EXPORT_C void MFepAttributeStorer::WriteAttributeDataAndBroadcastL(CCoeEnv& aConeEnvironment, TUid aAttributeUid)
	{
	TFixedArray<TUid, 1> arrayOfOneAttribute;
	arrayOfOneAttribute[0].iUid=aAttributeUid.iUid;
	WriteAttributeDataAndBroadcastL(aConeEnvironment, arrayOfOneAttribute.Array());
	}

/** Call this function after changing the value of multiple attributes that need 
to be synchronised. 

Writes the new values to the system settings, and causes all other running 
instances of the FEP to be notified of the change.

A panic occurs if any of the attributes contained in aAttributeUids are not 
among the FEP's attributes.

@param aConeEnvironment Not used.
@param aAttributeUids Array of UIDs of the attribute whose values have changed.
@capability WriteDeviceData		To prevent corruption of FEP settings.
*/
EXPORT_C void MFepAttributeStorer::WriteAttributeDataAndBroadcastL(CCoeEnv& /*aConeEnvironment*/, const TArray<TUid>& aAttributeUids)
	{
	CRepository* const repository=CRepository::NewLC(TUid::Uid(KUidFepSpecificSettingsRepository));
	RBuf8 buffer;
	CleanupClosePushL(buffer);
	buffer.CreateL(NCentralRepositoryConstants::KMaxBinaryLength);
	TInt error=KErrNone;
	do
		{
		User::LeaveIfError(repository->StartTransaction(CRepository::EConcurrentReadWriteTransaction));
		repository->CleanupCancelTransactionPushL();
		for (TInt i=aAttributeUids.Count()-1; i>=0; --i)
			{
			const TUid attributeUid(aAttributeUids[i]);
			__ASSERT_ALWAYS(NumberOfOccurrencesOfAttributeUid(attributeUid)==1, Panic(EPanicBadAttributeUid));
			RDesWriteStream writeStream(buffer);
			WriteAttributeDataToStreamL(attributeUid, writeStream);
			writeStream.CommitL();
			User::LeaveIfError(repository->Set(attributeUid.iUid, buffer));
			}
		CleanupStack::Pop(); // repository->CleanupCancelTransactionPushL
		TUint32 notUsed=0;
		error=repository->CommitTransaction(notUsed);
		} while (error==KErrLocked);
	User::LeaveIfError(error);
	CleanupStack::PopAndDestroy(2, repository);
	}

EXPORT_C void MFepAttributeStorer::MFepAttributeStorer_Reserved_1()
	{
	}

EXPORT_C void MFepAttributeStorer::MFepAttributeStorer_Reserved_2()
	{
	}

TInt MFepAttributeStorer::NumberOfOccurrencesOfAttributeUid(TUid aAttributeUid) const
	{
	TInt numberOfOccurrencesOfAttributeUid=0;
	for (TInt i=NumberOfAttributes()-1; i>=0; --i)
		{
		if (aAttributeUid==AttributeAtIndex(i))
			{
			++numberOfOccurrencesOfAttributeUid;
			}
		}
	return numberOfOccurrencesOfAttributeUid;
	}
