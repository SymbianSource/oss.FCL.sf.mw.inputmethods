/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Panic definitions.
*
*/












#if !defined(__AKNFEPPANIC_H__)
#define __AKNFEPPANIC_H__

#include <e32std.h>

enum TAknFepPanic
    {
    EAknFepPanicCurrentSelectionOutsideVisibleCandidates_unused,
    EAknFepPanicAttemptedSelectionOutsideVisibleCandidates_unused,
    EAknFepPanicAttemptedSelectionOutsideMaxCandidates,
    EAknFepPanicAttemptedSetItemTextOutsideMaxCandidates_unused,
    EAknFepPanicAttemptedSetModeOutsideMaxCandidates_unused,
    EAknFepPanicAttemptedLayoutOutsideMaxCandidates,
    EAknFepPanicAttemptedLayoutOutsideVisibleCount_unused,
    EAknFepPanicCurrentSelectionFallsOutsideNewVisibleCount,
    EAknFepPanicCurrentSelectionFallsOutsideCandidates,
    EAknFepPanicNewVisibleCountExceedsRange_unused,
    EAknFepPanicCandidatesExceedVisibleCount,
    EAknFepPanicCandidatesEmpty,
    EAknFepPanicNonPermittedEditorMode,
    EAknFepPanicSizeChangedOutsideRange,
    EAknFepPanicNoPermittedEditorModesAvailable,
    EAknFepPanicNotInChina,
    EAknFepPanicNoHandleToFepAwareTextEditor,
    EAknFepPanicFindModeSavedToNonFindEditor,
    EAknFepPanicMoreCandidatesWithToneMarkThanWithout,
    EAknFepPanicSelectionMustBeZeroIfInInputPane,
    EAknFepPanicNotInsideInlineEdit,
    EAknFepPanicNoCaseManager,
    EAknFepPanicBadInputState,
    EAknFepPanicNoUiManager,
    EAknFepPanicNoCandidatePane_unused,
    EAknFepPanicCandidatePaneNotSupportScrollDirection_unused,
    EAknFepPanicNotSupportKey,
    EAknFepPanicBadCase,
    EAknFepPanicBadCursorNavigationState,
    EAknFepPanicCharPosOutOfRange,
    EAknFepPanicNotHandleHashKey,
    EAknFepPanicSetWindowShapeFail, // for two line UI container window creation
    EAknFepPanicIllegalNumberModeKeymapping,
    EAknFepPanicInvalidEditorState
#ifdef RD_HINDI_PHONETIC_INPUT	
    ,EAknFepPanicNoValidPtiEngine,
    EAknFepPanicNoValidFepManager
#endif
    };

GLDEF_C void AknFepPanic(TAknFepPanic aPanic);

#endif

// End of file
