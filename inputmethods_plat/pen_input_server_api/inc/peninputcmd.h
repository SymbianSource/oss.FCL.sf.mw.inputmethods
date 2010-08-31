/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:               Peninput animation op codes and signal event
*
*/















#ifndef _PENINPUTCMD_H
#define _PENINPUTCMD_H
// AnimDLL commands
enum TPeninputOpcodes
    {
    EPeninputOpSystemCmdBase = 500,
    EPeninputOpFinishConstructionL,
    
    EPeninputOpActivate,    
    EPeninputOpDeactivate,
    //EPeninputOpIsLayoutVisible,
    //EPeninputOpSetLayoutId,
    EPeninputOpLayoutSizeChanged,
    //EPeninputOpCommand,
    //EPeninputOpSetLayoutIdWithData,
    EPeninputOpLayoutSizeChangedWithData,  
    //EPeninputOpServerDataNum,  
    //EHwGetSelectedChars,
    
    //EPeninputOpSendAppEditorText,
    //EPeninputOpGetEditorMaxLen,
    
    EPeninputOpSetLayoutPos,
    //EPeninputOpGetLayoutPos,
    
    //EPeninputOpAsyncCommand,
    //EPeninputOpHasUI,
    //EPeninputOpChangeDisplayMode,
    //EPeninputOpCloseAnim,
    EPeninputOpLayoutSizeChangedWithSize,
    EPeninputOpLayoutUpdateArea,
    //EPeninputOpReadyToGetEvent,
    EPeninputOpCapturePointer,
    EPeninputOpSimulateEvent,
#ifdef RD_TACTILE_FEEDBACK    
    EPeninputOpRegisterFeedbackArea, //register the tactile feedback area
    EPeninputOpDeRegisterFeedbackArea, //de-register the tactile feedback area
    EPeninputOpChangeFeedbackArea, //change the area
#endif // RD_TACTILE_FEEDBACK    
    EPeninputOpRegisterRawKeyArea,
    EPeninputOpDeRegisterRawKeyArea,
    EPeninputOpEnalbeSprite,
	EPeninputOpLayoutUpdateAreaRegion,
    EPeninputOpLastCommand ,
    EPeninputOpRequestDSAState,
	EPeninputOpChangeFeedbackType,
    EPeninputUserCmdBase = 1000

    };


enum TSignalEvent
    {
    //reserved event area for animation dll used only
    //
    ESignalCapturePointer = 1000,          //capture pointer event
    ESignalCancelEventBuffer,       //ask owner cancel buffering the event
    ESignalSimulateEvent,
    ESignalPenUiActivated,          //pen ui is going to open
    ESignalPenUiDeActivated,        //pen ui is going to close
    ESignalRegisterFeedbackArea,
    ESignalDeRegisterFeedbackArea,
    ESignalChangeFeedbackArea,
    ESignalDeRegisterAllFeedbackArea,
    ESignalRegisterRawKeyArea,    
    ESignalDeRegisterRawKeyArea,
    ESignalGetAllowedInputMode,
    ESignalPopupWnd,                //show pop up window in pen UI
    ESignalPopupWndClosed,          //The pop up window closed.
    
    ESignalServerExit,
	ESignalChangeFeedbackType,
    ESignalImmeSimulateEvent,
    ESignalServerReady = 1450,
    ESignalFepEventBase = 1500,    //Event base for Fep. All fep event
                                    //is re-directed to Fep.
    ESignalKeyEvent = 1501,        //hwr recongition selection event.
    ESignalNotifyUpdateCba,
    ESignalNotifyStoreMode,  
    ESignalHwNotifySctPopup, 

    ESignalSetAppEditorCursor,      //set the application editor cursor
    ESignalLaunchHelp,
    ESignalLaunchGenerlSetting,
    ESignalLaunchLanguageMenu,
    ESignalLaunchOptionMenu,
    ESignalLayoutClosed,
    ESignalLayoutICFLengthChanged,
    ESignalLayoutUIChanged,
    ESignalRegisterAnimClient,  //request to regist the animation client
    ESignalCommitInlineText,
    ESignalSelectMatchSelectionText, // param: index, commit (true or false?)
    ESignalNaviLeftMatchSelectionText, 
    ESignalNaviRightMatchSelectionText, 
    ESignalLaunchOptionsMenu,
    ESignalEnterSpellMode,
    ESignalExitSpellMode,
    ESignalLaunchSCT,
    ESignalEnterMatchSelection,
    ESignalGetEditMenuData,
    ESignalEditorMenuCommand,      
    ESignalCandidateIndex,
    ESignalCharacterWidth,
    ESignalJapanesePredictive,
    ESignalWesternPredictive,
    ESignalSyncIcfData,
    ESignalCharacterStart,
    ESignalShowCandidate,
    ESignalSelectCandidate,
    ESignalAcceptTooltip,
    ESignalStartInlineText,    
    ESignalCommitITIInlineText,    
    ESignalUserEventBase = 2000
    };

#endif //_PENINPUTCMD_H
// End of File
