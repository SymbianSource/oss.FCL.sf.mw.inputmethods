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












#if !defined(__AknFepUiInterfacePanic_h__)
#define __AknFepUiInterfacePanic_h__

#include <e32std.h>

enum TAknFepUiInterfacePanic
    {
    EAknFepUiInterfacePanicCurrentSelectionOutsideVisibleCandidates,
    EAknFepUiInterfacePanicAttemptedSelectionOutsideVisibleCandidates,
    EAknFepUiInterfacePanicAttemptedSetItemTextOutsideMaxCandidates,
    EAknFepUiInterfacePanicAttemptedSetModeOutsideMaxCandidates,
    EAknFepUiInterfacePanicAttemptedLayoutOutsideVisibleCount,
    EAknFepUiInterfacePanicNewVisibleCountExceedsRange,
    EAknFepUiInterfacePanicCandidatesEmpty,
    EAknFepUiInterfacePanicNoCandidatePane,
    EAknFepUiInterfacePanicCandidatePaneNotSupportScrollDirection,
    EAvkonImplNotSupported
    };

GLREF_C void AknFepUiInterfacePanic(TAknFepUiInterfacePanic aPanic);

#endif

// End of file
