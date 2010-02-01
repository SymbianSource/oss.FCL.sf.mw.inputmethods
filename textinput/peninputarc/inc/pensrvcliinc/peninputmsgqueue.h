/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Peninput message queue header
*
*/


#ifndef __PENINPUT_MSGQUE__
#define __PENINPUT_MSGQUE__

const TInt KMaxEvent = 5;
_LIT(KMsgQueue,"raweventqueue");
_LIT(KMsgEventBufferQueue,"raweventbufqueue");
_LIT(KSimulatedMsgQueue,"simulateeventq");
_LIT(KMsgEventResponseQueue,"keyresponsequeue");

_LIT( KAnimKeyEventSemaphore, "AnimKeyResponseSemaphore");

class TRawEventBuffer
    {
    public:
        TInt iNum;
        TRawEvent iEvent[KMaxEvent+1];
    };
#endif