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
*       Provides global enumerations.
*
*/















#ifndef __AKN_FEP_GLOBAL_ENUMS_H__
#define __AKN_FEP_GLOBAL_ENUMS_H__
#include <eikon.hrh>

#define EPRCFind            0x0004     
#define ECangJie            0x0008      // Cang Jie mode for HK
#define EZhuyinFind         0x0010
#define EStrokeFind         0x0020
#define EPinyin             0x0040
#define EZhuyin             0x0080
#define EStroke             0x0100
#define ELatin              0x0200
#define ENumber             0x0400
#define EHiraganaKanji      0x0800      // Hiragana
#define EKatakana           0x1000      // Katakana
#define EHiragana           0x10000     // Hiragana Only
#define EHindi              0x20000     // Hindi only
#define ENativeNumber       0x40000
/* Teleca change begin, 15.04.2008 ssal */
#define EHangul				0x160000		// Korean 
/* Teleca change end, 15.04.2008 ssal */

// Additional case flags for GS.
#define ELatinText          0x2000
#define ELatinUpper         0x4000
#define ELatinLower         0x8000

// CangJie Mode
#define ECangJieNormal      0
#define ECangJieEasy        1
#define ECangJieAdvanced    2

enum
    {
    KFirstMode = EZhuyinFind,
    KLastMode = EHangul
    };

enum TKeyPressLength
    {
    EShortKeyPress,
    ELongKeyPress
    };

enum TAknFepManagerState
    {
    EAknFepStateNull,
    EAknFepStateInitial,
    EAknFepStateUIActive
    };

enum TUIState
    {
    ENull,
    EInitial,
    EEntry,
    ECandidate,
    EPredictiveCandidate,
    EPredictiveInput,
    EConversion,         // Japanese only
    EMultiCompletion,    // Japanese only
    ENextWord,           // Japanese only
    EQwerty,   // Qwerty mode is not supported.
    ECandidateRetroActive, // Japanese only
    EHalfQwerty,
    ECandidateSelection,
    EStrokeSequenceEdit,
    EKeySequenceEdit,
    EZhuyinSequenceEdit,
    ESpellingSelection,
    EMiniQwertyEdit,
    EZhuyinSpelling,
    EZhuyinCandidate
    };


enum TCase
    {
    EUpperCase,
    ELowerCase,
    EFnKeyLowerCase,      // Qwerty fn-key lower case input.    
    EFnKeyUpperCase       // Qwerty fn-key upper case input.
    };

enum    // cba/softkeys
    {
    EAknFepSoftkeyPrevious  = 0x4000,
    EAknFepSoftkeySpell,
    EAknFepSoftkeySymbol,
    EAknFepSoftkeyConvert,
    EAknFepSoftkeyCloseWindow,
    EAknFepSoftkeyCommit,           // Using Multi-tap input on Japanese input
    EAknFepSoftkeySelectCandidate,  // Using Predictive input on Japanese input
    EAknFepSoftkeyOtherModes,       // For Japanese, Open other modes list
    EAknFepSoftkeyPredictiveOn,     // For Japanese, Start Prediction
    EAknFepSoftkeyRetroActiveHalf,  // For Japanese, Change the retroactive candidates to half-width
    EAknFepSoftkeyRetroActiveFull,  // For Japanese, Change the retroactive candidates to full-width
    // internal command
    EAknFepAllCompletion,
    EAknFepSoftkeyStartCopy,
    EAknFepSoftkeyStartCut,
    EAknFepResourceChanged
    };

enum TWordMatchState
    {
    EWordMatchNone,
    EWordMatchFirst,
    EWordMatchLast,
    EWordMatchAfterLast,
    EWordMatchStd
    };

//ADD_JP
enum TWidthChar
    {
    ENoneWidthChar,
    EHalfWidthChar,
    EFullWidthChar
    };
    
enum 
	{
	EAknEditMenuCmdCopyText = 5100,
	EAknEditMenuCmdCutText = 5101,
	EAknEditMenuCmdInsertWord = 5102		
	};    
    
/**
 *  Plugin input mode.
 *
 *  @since S60 v3.2
 */
enum TPluginInputMode
    {
    EPluginInputModeNone = 0,
    EPluginInputModeHwr = 1,
    EPluginInputModeVkb = 2,
    EPluginInputModeFSQ = 4,
    EPluginInputModeItut = 8,
    EPluginInputModeFSc = 16,    // new one
    EPluginInputModeMiniItut = 32,
    EPluginInputModeFingerHwr = 64,
    EPluginInputModeAll = 127      // EPluginInputModeHwr | ... | EPluginInputModeFingerHwr|
    };
    
#define KLatinToDevanagariDigitsDelta 0x0936
#define KLatinToArabicIndicDigitsDelta 0x0630
#define KLatinToEasternArabicIndicDigitsDelta 0x06C0

// definition of special number mode
enum TCustomeNumericKeymap
    {
    ECustomNumericKeymapStart = EAknEditorReservedInternalUseKeymap,
    EKeymapFromResource 
    }; 
      
    
// Clear direction (using KAknFepClearDirection shared key)
enum TClearDirection
    {
    EClearDirectionLeft = 0,
    EClearDirectionRight = 1
    };

// Japanese qwerty specif flags
//  (using KAknFepJapaneseSpecialCharFlag shared key)
//      Japanese Qwerty Comma ON(Japanese)/OFF(Western)
//      Japanese Qwerty Period ON(Japanese)/OFF(Western)
//      Japanese Qwerty Width Of Space Full/Half width
enum TJapQwertySettingFlags
    {
    EJapQwertySettingNone = 0,
    EJapQwertySettingComma = 0x0001,
    EJapQwertySettingPeriod = 0x0002,
    EJapQwertySettingSpaceFullWidth = 0x0004,
    EJapTouchSettingHwrConversion = 0x0010,
    EJapTouchSettingVkbRomajiInput = 0x0020
    };

//  (using  shared key)
enum TJapaneseConversion
    {
    EJapaneseConversionOff = 0,
    EJapaneseConversionOn = 1
    };

enum TKeyboardStyle
    {
    EKeyboardStyleNone = 0x00,
    EKeyboardStyle12Key = 0x01,
    EKeyboardStyleQwerty = 0x02,
    EKeyboardStyleHalfQwerty = 0x04
    };

enum TPhysicalKeyboardLayout
    {
    EPhysicalKeyboardNone,
    EPhysicalKeyboard12key = 0x01,
    EPhysicalKeyboardQwerty4x12 = 0x02,
    EPhysicalKeyboardQwerty4x10 = 0x04,
    EPhysicalKeyboardQwerty3x11 = 0x08,
    EPhysicalKeyboardHalfQwerty = 0x10,   
    EPhysicalKeyboardCustomQwerty = 0x20   
    };

#endif

// End of file
