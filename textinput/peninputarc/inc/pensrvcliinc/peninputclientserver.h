/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Peninput server request commands and consts
*
*/



#ifndef _PENINPUTCLIENTSERVER_H
#define _PENINPUTCLIENTSERVER_H

//  INCLUDES
#include <e32base.h>
#include <frmtlay.h>

// CONSTANTS  
// A version must be specifyed when creating a session with the server
const TUint KPeninputServerMajorVersionNumber=0;
const TUint KPeninputServerMinorVersionNumber=1;
const TUint KPeninputServerBuildVersionNumber=1;

const TInt KMsgSlot0 = 0;       //IPC message slot 0
const TInt KMsgSlot1 = 1;       //IPC message slot 1
const TInt KMsgSlot2 = 2;       //IPC message slot 2
const TInt KMsgSlot3 = 3;       //IPC message slot 3

const TInt KOperationTimerOutValue = 50000;//100000 ; //500ms

// MACROS
// server name
_LIT(KPeninputServerName,"peninputserver");
_LIT(KPeninputServerSemaphoreName, "PeninputServerSemaphore");
_LIT(KPeninputServerExe, "z:\\sys\\bin\\peninputserver.exe");


// opcodes used in message passing between client and server
enum TPeninputServerRqst
    {
    EPeninputServerRequestMessageNotification,
    EPeninputServerCancelMessageNotification,
   
    EPeninputRequestHandleClientCommand,
    //EPeninputRequestHandleClientSignal,  //Notification command from client
    EPeninputRequestGetServerEventData,
    EPeninputRequestGetServerEventDataNumber,
    EPeninputRequestUiIsVisible,
    EPeninputRequestUiEditorMaxLength,
    EPeninputRequestUiGetUiPos,
    EPeninputRequestUiSetUiPos,
    EPeninputRequestUiSendAppEditorText,
    EPeninputServerRequestSetDisplayMode,
    EPeninputServerClearServerEvent,
    //EPeninputRequestLayoutSizeChanged,
    //EPeninputRequestLayoutSizeChangedWithData,
    EPeninputRequestResourceChanged,
    EPeninputRequestActivateLayout,
    EPeninputRequestSetUiLayoutId,
    EPeninputRequestSetUiLayoutIdWithData,
    //EPeninputRequestPrepareSimulateEvent,
    EPenInputRequestGetImePluginIdList,
    EPenInputRequestGetPenSupportLanguages,
    EPeninputRequestSetForeground,
    EPeninputRequestIsForeground,
    EPeninputRequestAddUiObserver,
    EPeninputRequestRemoveUiObserver,
    EPeninputRequestUiNotificationCompleted,
    //EPeninputRequestChangeUiObserverType,
    EPeninputRequestActivateLayoutInGlobalNotes,
    EPeninputRequestDisableLayout,
    EPeninputRequestDimLayout,
    EPeninputRequestUpdateAppInfo,
    EPeninputRequestRelinquishForeground,
    EPeninputRequestIsLayoutDimmed,
    EPeninputRequestServerThreadId,
    EPeninputRequestGetDisableLayout,
    EPeninputBackgroudDefaultOri,
    EPeninputRequestInternalDimLayout,
    EPeninputRequestDimResChangeLayout,
    EPeninputRequestSupportInputMode
    };

//server error code
enum TPeninputServerErrCode
    {
    KErrObserverNoHandler = -1005,   //observer does not have handler
    KErrObserverAlreadyActive = -1004,   //observer already active
    KErrInvalidCmdCase = -1003,  //invalid command 
    KErrNoLayout = -1002,	//current no layout is created    
    KErrInvalidLayoutId = -1001,	// no ui plugin for this layout id.
    KErrNoServer = -1000,  //no server
    KErrNotForegroundSession,
    KErrNotWrongFocusedWindowGroup //command from different focused window group will be ignore.
    };
#endif // _PENINPUTCLIENTSERVER_H
// End of File
