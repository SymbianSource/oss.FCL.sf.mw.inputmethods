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
* Description:               Pen input related enums
*
*/














#ifndef AKNFEPPENINPUTENUMS_H
#define AKNFEPPENINPUTENUMS_H

#include <peninputcmd.h>
/**
 *TAknFepPenInputCmd 
 *The command is sent from FEP to IME plugin
 *Some commands have a TInt parameter
*/
enum TAknFepPenInputCmd
    {
    ECmdPenInputBase,
    /**
     * Set allowed input range
     * Parameter is input permitted mode
     */
    ECmdPenInputPermittedRange, 
    
    /**
     * Parameter is TAknEditorNumericKeymap
     */
    ECmdPenInputEditorNumericKeyMap,
    
    /**
     * Parameter is the case
     */
    ECmdPenInputCase,
    
    /**
     * Parameter is editor input language
     */
    ECmdPenInputLanguage,
    
    /**
     * Parameter is CAknEdwinState*, the command is only 
     * sent to IME plugin. 
     */
    ECmdPenInputEditorState,
    
    /**
     * When current editor is data query, UI needs adjust position
     * The command is sent to IME plugin then goes to UI layout
     *
     * Parameter is, 
     *  to IME plugin, TRect* means data query dialog rect
     *  to UI layout, a Desc contains TRect 
     */
    ECmdPenInputSetWindowPos,
    
    /**
     * Parameter is TAknFepPenInputOpenMode, indicating open method
     */
    ECmdPenInputWindowOpen,
    
    /**
     * No parameter
     */
    ECmdPenInputWindowClose,

    /**
     * Parameter is a TBool to indicate whether change case is allowed
     */
    ECmdPenInputAllowChangeCase,

    /**
     * Parameter is TInt, case mode
     */
    ECmdPenInputCaseMode,

    /**
     * Parameter is TInt, primary range
     */
    ECmdPenInputRange,

    ECmdPenInputPermittedCase,
    ECmdPenInputPluginInputMode, 
    
    ECmdPenInputEnableSettingBtn, 
    
    ECmdPenInputEditorCustomNumericKeyMap,
    
    ECmdPenInputSuggestRange,

    ECmdPenInputIsSecretText,
    
    ECmdPenInputIsNumberGrouping,
    
    /**
     * Parameter is a TBool to indicate whether dim arrow keys
     * in FindPane Editor
     */
    ECmdPenInputDimArrowKeys,
    
    ECmdPenInputSendEditorTextAndCurPos,
    
    /**
     * Japanese setting information for peninput.
     * The following setting information is included as parameter.
     *  Deleting direction  - 1st bit BIT ON(right side) / OFF(left side)
     *  Japanese predictive - 2nd bit BIT ON(predictive on) / OFF(predictive off)
     *  Japanese conversion - 3rd bit BIT ON(conversion on) / OFF(conversion off)
     *  Character width     - 4th bit BIT ON(full-width) / OFF(half-width)
     *  Japanese Qwerty Comma  - 5th bit BIT ON(Japanese)/OFF(Western)
     *  Japanese Qwerty Period - 6th bit BIT ON(Japanese)/OFF(Western)
     *  Japanese Qwerty Width Of Space - 7th bit BIT ON(Full)/OFF(Half)
     *  Lunch SCT           - 8th bit BIT ON(allow)/OFF(not allow)
     */
    ECmdPenInputJapaneseSetting,

    ECmdPenInputJapaneseSetSubRange,

    ECmdPenInputJapaneseGetModeOfSubRange,

    ECmdPenInputFingerInputMode,
    ECmdPenInputFingerOpenSCT,    //1020
    ECmdPenInputFingerCloseSCT,
    ECmdPenInputDimKey,
    ECmdPenInputHideKey,
    ECmdFingerShowKey,
    ECmdPenInputFingerHideLayout,
    ECmdPenInputFingerEvent,
    ECmdPenInputFingerLongKeyPress, //1028
    ECmdPenInputFingerKeyPress,     //1029
    ECmdPenInputFingerMatchSelection,//1030
    ECmdPenInputFingerMatchList,
    ECmdPenInputFingerMatchIndicator,
    ECmdPenInputFingerSpelling,
    ECmdPenInputFingerShowSpellEntry,
    ECmdPenInputSetTextAlignment,
    ECmdPenInputSetPromptText,
    ECmdPenInputNoFreeSpace, 
    ECmdPenInputDeActive,    
    ECmdPenInputSendEditMenuData,
    ECmdPenInputActive, 
    ECmdPenInputCharacterPreview,  
    ECmdPenInputEnableIndicatorButton,        
    ECmdPenInputSymbolOfHardKeyOne,
    ECmdPeninputAvkonFepState,
    ECmdPeninputCandidateList,
    ECmdPeninputCandidateIndex,
    ECmdPenInputFingerKutenQuery,
    ECmdPenInputInEditWordQueryDlg,
    ECmdPenInputRotation,
    ECmdPenInputPopupTooltip,
    ECmdPenInputHideTooltip,
    ECmdPenInputPopupCandidateList,
    ECmdPenInputHideCandidateList,
    ECmdPeninputITIStatus,
    ECmdPenInputImeCmdBase = 2000
    };

enum TAknFepPenInputSignal
    {
    ESignalPenInput = ESignalUserEventBase,
    /**
     * When plugin UI change case mode, notify FEP
     * Parameter is case mode.
     */
    ESignalCaseMode,
    ESignalRange,
    ESignalMessage,
    ESignalReplaceText,
    ESignalReplaceTextJp,
	ESignalReplacePartText
    };

enum TAknFepPenInputRange
    {
    ERangeInvalid = 0,
    ERangeNative  = 0x01,
    ERangeEnglish = 0x02,
    ERangeNumber  = 0x04,
    ERangeSymbol  = 0x08,
    ERangeAccent =  0x10
    };

/**
 *TAknFepPenInputJapaneseSubRange
 * for Japanese ERangeNative
 */
enum TAknFepPenInputJapaneseSubRange
    {
    ERangeHiraganaKanji                 = 0x20,     // for Japanese variant
    ERangeKatakana                      = 0x40,     // for Japanese variant
    ERangeFullWidthKatakana             = 0x80,     // for Japanese variant
    ERangeFullWidthEnglish              = 0x100,    // for Japanese variant
    ERangeFullWidthNumeric              = 0x200,    // for Japanese variant
    ERangeHiraganaOnly                  = 0x400     // for Japanese variant
    };

enum TAknFepPenInputCaseMode
    {
    ECaseInvalide = 0,
    //ABC
    ECaseUpper = 1, 
    //abc
    ECaseLower = 2,
    //Abc
    ECaseText = 4,
    //aBC
    ECaseInverseText = 8
    };
      
      
enum TAknFepPenInputOpenMode
    {
    //Pen input service is opened automatically by program internal logic
    EPenInputOpenAutomatically,
    
    //Pen input service is opened by clicking touch input icon
    EPenInputOpenManually 
    };
    
/**
 *TAknFepPenInputJapaneseSetting
 * for ECmdPenInputJapaneseSetting command
 */
enum TAknFepPenInputJapaneseSetting
    {
    // Deleting direction ON(right side) / OFF(left side)
    EPenInputJapaneseSettingDeletingDirection  = 0x00000001,
    // Japanese predictive ON(predictive on) / OFF(predictive off)
    EPenInputJapaneseSettingPredictive         = 0x00000002,
    // Japanese HWR conversion ON(conversion on) / OFF(conversion off)
    EPenInputJapaneseSettingConversion         = 0x00000004,
    // Character width ON(full-width) / OFF(half-width)
    EPenInputJapaneseSettingCharacterWidth     = 0x00000008,
    // Japanese Qwerty Comma ON(Japanese)/OFF(Western)
    EPenInputJapaneseSettingQwertyComma        = 0x00000010,
    // Japanese Qwerty Period ON(Japanese)/OFF(Western)
    EPenInputJapaneseSettingQwertyPeriod       = 0x00000020,
    // Japanese Qwerty Width Of Space ON(Full)/OFF(Half)
    EPenInputJapaneseSettingQwertyWidthOfSpace = 0x00000040,
    // Lunch SCT ON(allow)/OFF(not allow)
    EPenInputJapaneseSettingLunchSCT           = 0x00000080,
    // Hiragana Romaji input methods ON(Romaji)/OFF(Kana)
    EPenInputJapaneseSettingRomajiInput        = 0x00000100
    };
    
/**
 * TAknFepPenInputSettingWritingSpeed
 * for pen input general setting and hwr input methods
 */
enum TAknFepPenInputSettingWritingSpeed
    {
    EPenInputWritingSpeedVeryFast = 1,
    EPenInputWritingSpeedFast,
    EPenInputWritingSpeedNormal,
    EPenInputWritingSpeedSlow,
    EPenInputWritingSpeedVerySlow
    };
    
enum TAknFepPenInputWritingCharDelay
    {
    EPenInputWritingCharDelayVeryFast = 300*1000,
    EPenInputWritingCharDelayFast = 400*1000,
    EPenInputWritingCharDelayNormal = 550*1000,
    EPenInputWritingCharDelaySlow = 700*1000,
    EPenInputWritingCharDelayVerySlow = 1000*1000
    };
    
#endif // AKNFEPPENINPUTENUMS_H

// End Of File
