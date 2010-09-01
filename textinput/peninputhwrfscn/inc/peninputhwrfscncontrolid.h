/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Enum values of control id in boxbase hwr pluggin
*
*/


#ifndef T_PENINPUTHWRFSCNCONTROLID_H
#define T_PENINPUTHWRFSCNCONTROLID_H

/**
 *  TPenInputHwrfscnCtrlId
 *
 *  defines the controlid of full screen hwr
 *
 *  @lib peninputhwrboxcn.lib
 *  @since S60 v5.0
 */
enum TPenInputHwrfscnCtrlId
    {
    EHwrCtrlIdHwrWindow,
    EHwrCtrlIdInputContextField,
    EHwrCtrlIdTouchInputOption,
    
    EHwrCtrlIdCandidateList,
	EHwrCtrlIdPredictiveList,
    EHwrCtrlIdCandidatePrimary,
    EHwrCtrlIdCandidateSecondary,
    
    EHwrCtrlIdControlBar,
    EHwrCtrlIdMoreFunc,
    EHwrCtrlIdBackspace,
    EHwrCtrlIdMoreFuncGroup,
    EHwrCtrlIdLanguageWidthFullHalf,
    EHwrCtrlIdLanguageWidthHalfFull,
    EHwrCtrlIdLanguageOption,
    EHwrCtrlIdSpace,
    EHwrCtrlIdEnter,
    EHwrCtrlIdInputLangSwitcher,
    EHwrCtrlIdVkbSwitcher,
    
    // add by jiawenjuan
    EHwrCtrlIdClose,

    
    EHwrCtrlIdSymbolTableVkb,
    EHwrCtrlIdSymbolTableVkbGroup,
    
    EHwrCtrlIdRangeBar,
    EHwrCtrlIdChineseButton,
    EHwrCtrlIdEnglishButton,
    EHwrCtrlIdSymbolButton,
    EHwrCtrlIdNumberButton,
    
    EHwrCtrlIdCoverRangeBar,
    EHwrCtrlIdCoverChineseButton,
    EHwrCtrlIdCoverEnglishButton,
    EHwrCtrlIdCoverSymbolButton,
    EHwrCtrlIdCoverNumberButton,
    
    EHwrCtrlIdInputModeSwitcher,
    EHwrCtrlIdBkgndWnd1,
    EHwrCtrlIdBkgndWnd2
    };

#endif //T_PENINPUTHWRFSCNCONTROLID_H

// End Of File
