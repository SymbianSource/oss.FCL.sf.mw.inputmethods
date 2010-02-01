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
// FEPBPANIC.CPP
// 
//

#ifndef __FEPBPANIC_H__
#define __FEPBPANIC_H__

/** @internalComponent */
_LIT(KLitFepBasePanicText, "FEP-BASE");

/** @internalComponent */
enum TFepBasePanic
	{
	EPanicBadAttributeUid=1,
	EPanicNoAttributeUids,
	EPanicBadIndex1,
	EPanicBadIndex2,
	EPanicBadIndex3,
	EPanicBadIndex4,
	EPanicBadIndex5,
	EPanicKeyEventQueueIsAlreadySet1,
	EPanicKeyEventQueueIsAlreadySet2,
	EPanicKeyEventQueueIsAlreadySet3,
	EPanicKeyEventQueueIsAlreadySet4,
	EPanicUndefinedEventResponse1,
	EPanicUndefinedEventResponse2,
	EPanicUndefinedEventResponse3,
	EPanicNoDownUpFilter,
	EPanicWithDownUpFilter,
	EPanicAlreadyCalledBaseConstructL,
	EPanicInconsistentModifierMaskAndValues,
	EPanicBadFlag,
	EPanicAlreadyHandlingKeyEvent,
	EPanicNotHandlingKeyEvent,
	EPanicBadKeyResponse1,
	EPanicBadKeyResponse2,
	EPanicBadKeyResponse3,
	EPanicBadKeyResponse4,
	EPanicBadKeyResponse5,
	EPanicDeferredFunctionCallOutstanding,
	EPanicBadRecursionLevel,
	EPanicNotFullFileName1,
	EPanicNotFullFileName2,
	EPanicNotFullFileName3,
	EPanicNotFullFileName4,
	EPanicFileIsInRom1,
	EPanicFileIsInRom2,
	EPanicNegativeArrayLength,
	EPanicBadNumberOfCharacters1,
	EPanicBadNumberOfCharacters2,
	EPanicBadNumberOfCharacters3,
	EPanicCharacterNotInArrayOfFrequencyCountedCharactersOrderedByCharacterEncoding,
	EPanicNegativeArrayCount,
	EPanicNotOrderedByCharacterEncoding,
	EPanicCountedCharacterHasZeroOrNegativeFrequencyCount,
	EPanicInconsistentArrayCounts,
	EPanicBadSortType,
	EPanicNotInsideMutexProtectedCode1,
	EPanicNotInsideMutexProtectedCode2,
	EPanicSortConfusion,
	EPanicBinarySearchConfusion,
	EPanicBadResult1,
	EPanicBadResult2,
	EPanicUnexpectedError1,
	EPanicUnexpectedError2
	};

#endif // __FEPBPANIC_H__

