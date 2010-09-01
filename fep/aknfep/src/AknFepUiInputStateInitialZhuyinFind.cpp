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
*       Provides the TAknFepInputStateInitialZhuyinFind methods.
*
*/












#include "AknFepUiInputStateInitialZhuyinFind.h"
#include "AknFepUIManagerStateInterface.h"

#include <PtiEngine.h>

TAknFepInputStateInitialZhuyinFind::TAknFepInputStateInitialZhuyinFind(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateInitialChineseMultitapBase(aOwner, aUIContainer)
    {
    }

// End of file
