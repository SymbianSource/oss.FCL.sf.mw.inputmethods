/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef PENINPUTFINGERHWRCONTROLID_H
#define PENINPUTFINGERHWRCONTROLID_H

/**
 *  TPenInputFingerHwrCtrlId
 *
 *  defines the controlid of full screen hwr
 *
 *  @lib peninputfingerhwr.lib
 *  @since S60 v5.0
 */
enum TPenInputFingerHwrCtrlId
    {
    EHwrCtrlIdHwrWindow,
    EHwrCtrlIdWritingBox,
    EHwrCtrlIdInputContextField,
    EHwrCtrlIdTouchInputOption,
    
    EHwrCtrlIdFixEngSymbolList,
    EHwrCtrlIdCandidateList,
	EHwrCtrlIdPredictiveList,
    EHwrCtrlIdCandidatePrimary,
    EHwrCtrlIdCandidateSecondary,
    EHwrCtrlIdBackspace,
    EHwrCtrlIdMoreFuncGroup,
    EHwrCtrlIdLanguageOption,
//    EHwrCtrlIdSpace,
//    EHwrCtrlIdEnter,
    EHwrCtrlIdInputLangSwitcher,
    EHwrCtrlIdVkbSwitcher,
    EHwrCtrlIdClose,

    
    EHwrCtrlIdSymbolTableVkb,
    EHwrCtrlIdSymbolTableVkbGroup,
    
    EHwrCtrlIdRangeSelector,
    
    EHwrCtrlIdChineseButton,
    EHwrCtrlIdEnglishButton,
    EHwrCtrlIdSymbolButton,
    EHwrCtrlIdNumberButton,
    
    EHwrCtrlIdChineseMiButton,
    EHwrCtrlIdEnglishMiButton,
    EHwrCtrlIdNumberMiButton,
    EHwrCtrlIdSymbolMiButton,
    
    EHwrCtrlIdInputModeSwitcher,
    EHwrCtrlIdArrowLeft,
    EHwrCtrlIdArrowRight,
    EHwrCtrlIdArrowUp,
    EHwrCtrlIdArrowDown,
    
    EHwrCtrlIdNumpad,
    EHwrCtrlIdSctpad,
    EHwrCtrlIdSctpadFix,

    EHwrCtrlIdSctPage0Btn,
    EHwrCtrlIdSctPage1Btn,
    EHwrCtrlIdSctPage2Btn,
    };

#endif //PENINPUTFINGERHWRCONTROLID_H

// End Of File