/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:               Internal Central Repository keys.
 *
*/















#ifndef AKNFEP_INTERNAL_CR_KEYS_H
#define AKNFEP_INTERNAL_CR_KEYS_H

const TUid KCRUidAknFep = { 0x101F876D };

/**
 * If this feature is enabled, new line can be entered by pressing scroll down.
 * Value is either integer 1 (enabled) or 0 (disabled).
 * Default value: 1 in Japanese variant. 0 in other variants.
 */
const TUint32 KAknFepEnterWithScrollDown   = 0x00000001;

/**
 * If this feature is enabled, Latin predictive input candidates are selected
 * using scroll keys. If the feature is disabled, they are selected using
 * softkeys. It is recommended to enable this feature in Japanese variant,
 * because in Japanese input, predictive candidates are always selected using
 * scroll keys.
 * Value is either integer 1 (enabled) or 0 (disabled).
 * Default value: 1 in Japanese variant. 0 in other variants.
 */
const TUint32 KAknFepScrollLatinPredictive = 0x00000002;

/**
 * If this feature is enabled, space can be entered by pressing scroll right.
 * Value is either integer 1 (enabled) or 0 (disabled).
 * Default value: 1 in Japanese and Thai variant. 0 in other variants.
 */
const TUint32 KAknFepSpaceWithScrollRight  = 0x00000003;

/**
 * Chinese Input Mode, integer value.
 *
 * Used values:
 *
 * 0x10  (Pinyin)
 * 0x20  (Zhuyin)
 * 0x40  (Stroke)
 * 0x80  (Latin Upper Case)
 * 0x100 (Latin Lower Case)
 *
 * Default value: 0
 */
const TUint32 KAknFepChineseInputMode      = 0x00000004;

/**
 * Input Text Language, integer value.
 *
 * Possible values:
 * 0 (automatic)
 * valid Symbian OS language code
 *
 * Default value: 0
 */
const TUint32 KAknFepInputTxtLang          = 0x00000005;

/**
 * This key is required for the Japanese predictive text input feature.
 * General settings uses it to store the user selection for Japanese
 * predictive text input.
 *
 * Default value: 0
 */
const TUint32 KAknFepJapanesePredTxtFlag   = 0x00000006;

/**
 * Predictive Text Input On/Off for ITU-T keypad.
 *
 * Possible values:
 * 0: off (default value)
 * 1: on
 */
const TUint32 KAknFepPredTxtFlag           = 0x00000007;

/**
 * Multitap timer value.
 * Replaces the constant KMultiTapTimeoutMS in Japanese variant.
 *
 * Default value: 1000000 in Japanese variant. 0 in other variants.
 */
const TUint32 KAknFepMultiTapTimer         = 0x00000008;

/**
 * Chinese CangJie input method for S60 and QWERTY.
 *
 * Possible values:
 * 0 (ECangJieNormal)
 * 1 (ECangJieEasy)
 * 2 (ECangJieAdvanced)
 */
const TUint32 KAknFepCangJieMode           = 0x00000009;

/**
 * Test mode key used in emulator.
 */
const TUint32 KAknFepTestModeKey           = 0x0000000A;

/**
 * This key is required for the Japanese special character flag. General 
 * settings uses it to store the user selection for Japanese special 
 * character settings.
 * - Comma: Japanese comma = 0x01
 *      Western comma != 0x01
 * - Period: Japanese period = 0x02
 *      Western period != 0x02
 * - Width of Space: Full width = 0x04
 *      Half width != 0x04
 * - Width of punctuation marks and symbols:
 *      Full width = 0x08
 *      Half width != 0x08
 */
const TUint32 KAknFepJapaneseSpecialCharFlag = 0x0000000B;

/**
 * Indicates whether the hash key is used in selections in lists and editors. 
 * Needed because making Edit key optional in the device. 
 * Possible values are: 1 (hash key selection in use) or 0 (hash key 
 * selection not in use).
 *
 * Default value: 0
 */
const TUint32 KAknFepHashKeySelection      = 0x0000000C;

/**
 * Indicates whether "edit text" submenu should be added to application 
 * options-menu by FEP.
 */
const TUint32 KAknFepEditTextSubmenu       = 0x0000000D;

/*
 * The bottom right point of the hwr window in the orientation mode
 * EGraphicsOrientationNormal.
 */
const TUint32 KAknFepHwrWindowPosition0 = 0x0000000E;

/**
 * The bottom right point of the hwr window in the orientation mode
 * EGraphicsOrientationRotated90.
 */	
const TUint32 KAknFepHwrWindowPosition1 = 0x0000000F;
	

/**
 * The bottom right point of the hwr window in the orientation mode
 * EGraphicsOrientationRotated180.
 */
const TUint32 KAknFepHwrWindowPosition2 = 0x00000010;
	
/**
 * The bottom right point of the hwr window in the orientation mode 
 * EGraphicsOrientationRotated270.
 */
const TUint32 KAknFepHwrWindowPosition3 = 0x00000011;
	
/**
 * The last used alpha recognition range.
 */
const TUint32 KAknFepLastUsedRange = 0x00000012;
	
/**
 *  Information whether HWR or VKB input method was used last.
 */
const TUint32 KAknFepLastUsedInput = 0x00000013;

/**
 *  Tells which UI language was active when FEP was last initialized.
 */
const TUint32 KAknFepLastUsedUILanguage = 0x00000014;

/**
 * This key is required for the clear direction.
 * Clear direction value Left side (Backspace function) /
 * Right side (Delete function).
 * Possible values are: (defined in AknFepGlobalEnums.h)
 * EClearDirectionLeft(0)  remove a character on the left side
 * EClearDirectionRight(1) remove a character on the right side
 */
const TUint32 KAknFepClearDirection = 0x00000015;

/**
 * Predictive Text Auto Word Completion On/Off.
 *
 * A bit vector indicating which input types have auto completion on.
 *
 * Possible values:
 * 0: off
 * 1: on (default value)
 *
 * (bitmask values in EKeyboardStyle)
 *
 * 0x01 = ITU-T (12-key) predictive input
 * 0x02 = Qwerty predictive input
 */
const TUint32 KAknFepAutoCompleteFlag = 0x00000016;

/**
 * Predictive Text Typing Correction Level.
 *
 * Possible values:
 * 0: off 
 * 1: medium (default value)
 * 2: high
 */
const TUint32 KAknFepTypingCorrectionLevel = 0x00000017;

/**
 * Predictive Text Number Candidate Shown/Not shown.
 *
 * Possible values:
 * 0: Not Shown 
 * 1: Shown (default value)
 */
const TUint32 KAknFepNumberCandidateFlag = 0x00000018;

/**
 * Predictive Text Primay Candidate Exact/Suggested.
 *
 * Possible values:
 * 0: Suggested (default value)
 * 1: Exact 
 */
const TUint32 KAknFepPrimaryCandidateFlag = 0x00000019;

/** 
 * Available physical keyboards. 
 * A bit vector indicating which physical keyboards are connected to the device.
 * Possible values (combination of these):
 * 0x01 = 12-key ITU-T,
 * 0x02 = 4x12 qwerty,
 * 0x04 = 4x10 qwerty,
 * 0x08 = 3x11 qwerty,
 * 0x10 = half qwerty,
 * 0x20 = custom qwerty
 */
const TUint32 KAknFepPhysicalKeyboards = 0x0000001A;

/**
 * Predictive Text Input On/Off.
 *
 * A bit vector indicating which input types have prediction on.
 *
 * Possible values:
 * 0: off
 * 1: on (default value)
 *
 * (bitmask values in EKeyboardStyle)
 *
 * 0x01 = ITU-T (12-key) predictive input (should match KAknFepPredTxtFlag)
 * 0x02 = Qwerty predictive input
 */
const TUint32 KAknFepPredTxtFlagExtension = 0x0000001B;

/**
 * Secondary Input Text Language, integer value.
 *
 * Possible values:
 * 0 (none)
 * valid Symbian OS language code
 *
 * Default value: 0
 */
const TUint32 KAknFepInputTxtLangSecondary = 0x0000001C;

/**
 * Indicate fullscreen qwerty(big one) key preview popup On/Off.
 *
 * Possible values:
 * 0: off
 * 1: on (default value)
 */
const TUint32 KAknFepShowFsqPreview = 0x0000001D;

/**
 * Indicate vkb (small one) preview popup On/Off.
 *
 * Possible values:
 * 0: off 
 * 1: on(default value)
 */
const TUint32 KAknFepShowVkbPreview  = 0x0000001E;

/**
 * the value stores the preferred virtual 
 * qwerty mode in landscape, there are two modes, 
 * FSQ the bigger one or VKB the smaller one. 
 * The mode values are from touch input types,  
 * EPluginInputModeHwr = 1, 
 * EPluginInputModeVkb = 2,
 * EPluginInputModeFSQ = 4,
 * EPluginInputModeItut = 8,
 * EPluginInputModeFSc = 16.
 * And only EPluginInputModeVkb and EPluginInputModeFSQ 
 * are valid here, and the default value is FSQ 4.
 */
const TUint32 KAknFepLastUseVkbMode = 0x0000001F;

/**
 * Long press number entry feature on (hybrid) QWERTY keyboard. When activated, long press of 
 * keyboard button can be used to enter the character normally mapped behind Fn modifier on that key. 
 * This setting has no effect if the keyboard does not have Fn key. Possible values: 0 (off), 1 (on)
 */
const TUint32 KAknFepLongPressNumberEntryOnQwerty = 0x00000021;
/**
 * Copy-pasting with shift + softkeys on QWERTY. When activated, text can be copied by holding 
 * shift depressed and pressing left softkey and pasted by holding shift depressed and pressing 
 * right softkey. Similar funtionality is anyway available on ITU-T keypads with the edit key; 
 * this setting affects only QWERTY keyboards. Possible values: 0 (off), 1 (on)
 */
const TUint32 KAknFepShiftCopyPasteOnQwerty = 0x00000022;

/*
 * This key is used for arabic finger hwr to set or get the default arabic finger hwr orientation.
 * 0 is for portrait and 1 is for landscape.
 */
const TUint32 KAknFepDefaultArabicFingerHwrOrientation = 0x00000023;

/**
 * This key is required for the Korean Combine Word flag. Korean
 * peninputplugin uses it to store the user selection for Korean Combine 
 * Words settings.
 * Korean Combining Version is 0x00000001,
 * Korean ConsonantSsangKiyeok combining stage is 0x00000002,
 * Korean ConsonantSsangTikeut combining stage is 0x00000004,
 * Korean ConsonantSsangPieup combining stage is 0x00000008,
 * Korean ConsonantSsangSios combining stage is 0x00000010,
 * Korean ConsonantSsangCieuc combining stage is 0x00000020,
 * Korean ConsonantYaPlusI combining stage is 0x00000040,
 * Korean ConsonantYuPlusI combining stage is 0x00000080,
 * Korean Combining Word Setting On is 0x00000100
 *
 */
const TUint32 KAknFepKoreanCombineWordFlag = 0x00000024;              

/*
 * This key is used for arabic finger hwr to set or get the default number mode.
 * in number table.
 * 0 is for latin number and 1 is for native number.
 */
const TUint32 KAknFepDefaultArabicNumberMode = 0x00000025;

/**
 * This key is used for storing the last used input mode in portrait orientation.
 * With adding portrait FSQ, users have two choices for portrait input mode,
 * ITU-T and portrait FSQ.  The last used input mode in portrait orientation is
 * kept to have the input mode if the phone changes the orientation to landscape
 * and back to portrait.
 * 
 * Only EPluginInputModeItut(8) and EPluginInputModePortraitFSQ(128) are valid,
 * and the default value is EPluginInputModeItut.
 */
const TUint32 KAknFepLastUsedPortraitInput = 0x00000026;

/**
 * following keys are used for tap accuracy enhancement, default input mode
 */
const TUint32 KAknFepTapAccuracyDefaultButtonExtMargins         = 0x27;
const TUint32 KAknFepTapAccuracyDefaultKeyCtrlExtMargins        = 0x28;
const TUint32 KAknFepTapAccuracyDefaultPointerMoveMaxMovement   = 0x29;
const TUint32 KAknFepTapAccuracyDefaultPointerMoveTimeout       = 0x2a;
const TUint32 KAknFepTapAccuracyDefaultPointerUpMaxMovement     = 0x2b;
const TUint32 KAknFepTapAccuracyDefaultPointerUpTimeout         = 0x2c;

/**
 * following keys are used for tap accuracy enhancement, FSQ
 */
const TUint32 KAknFepTapAccuracyFsqButtonExtMargins             = 0x2d;
const TUint32 KAknFepTapAccuracyFsqKeyCtrlExtMargins            = 0x2e;
const TUint32 KAknFepTapAccuracyFsqPointerMoveMaxMovement       = 0x2f;
const TUint32 KAknFepTapAccuracyFsqPointerMoveTimeout           = 0x30;
const TUint32 KAknFepTapAccuracyFsqPointerUpMaxMovement         = 0x31;
const TUint32 KAknFepTapAccuracyFsqPointerUpTimeout             = 0x32;

/**
 * following keys are used for tap accuracy enhancement, Portrait FSQ
 */
const TUint32 KAknFepTapAccuracyPFsqButtonExtMargins            = 0x33;
const TUint32 KAknFepTapAccuracyPFsqKeyCtrlExtMargins           = 0x34;
const TUint32 KAknFepTapAccuracyPFsqPointerMoveMaxMovement      = 0x35;
const TUint32 KAknFepTapAccuracyPFsqPointerMoveTimeout          = 0x36;
const TUint32 KAknFepTapAccuracyPFsqPointerUpMaxMovement        = 0x37;
const TUint32 KAknFepTapAccuracyPFsqPointerUpTimeout            = 0x38;

/**
 * following keys are used for tap accuracy enhancement, Finger HWR
 */
const TUint32 KAknFepTapAccuracyFhwrButtonExtMargins            = 0x39;
const TUint32 KAknFepTapAccuracyFhwrKeyCtrlExtMargins           = 0x3a;
const TUint32 KAknFepTapAccuracyFhwrPointerMoveMaxMovement      = 0x3b;
const TUint32 KAknFepTapAccuracyFhwrPointerMoveTimeout          = 0x3c;
const TUint32 KAknFepTapAccuracyFhwrPointerUpMaxMovement        = 0x3d;
const TUint32 KAknFepTapAccuracyFhwrPointerUpTimeout            = 0x3e;

#endif

// End of file
