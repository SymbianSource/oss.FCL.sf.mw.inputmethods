/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Enum values of control id in fingerhwr hwr pluggin
*
*/

#ifndef PENINPUTFINGERHWRARCONTROLID_H
#define PENINPUTFINGERHWRARCONTROLID_H

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
	EHwrCtrlIdHandwringIndicator,
    EHwrCtrlIdInputContextField,
    EHwrCtrlIdTouchInputOption,
    
    EHwrCtrlIdCandidateList,
    EHwrCtrlIdNumCandidateList,
    EHwrCtrlIdBackspace,
    EHwrCtrlIdLanguageOption,
    EHwrCtrlIdInputLangSwitcher,
    EHwrCtrlIdVkbSwitcher,
    EHwrCtrlIdClose,

    
    EHwrCtrlIdSymbolTableVkb,
    EHwrCtrlIdSymbolTableVkbGroup,
	EHwrCtrlIdSctpad,
    EHwrCtrlIdSymbolButton,
    
    EHwrCtrlIdInputModeSwitcher,
    EHwrCtrlIdArrowLeft,
    EHwrCtrlIdArrowRight,
    EHwrCtrlIdArrowUp,
    EHwrCtrlIdArrowDown,
    
	EHwrCtrlIdBtnEnter,
	EHwrCtrlIdBtnSpace,
    EHwrCtrlId3Page1Btn,
    EHwrCtrlId3Page2Btn,
    EHwrCtrlId3Page3Btn,
	EHwrCtrlId2Page1Btn,
	EHwrCtrlId2Page2Btn
    };
#endif //PENINPUTFINGERHWRARCONTROLID_H

// End Of File
