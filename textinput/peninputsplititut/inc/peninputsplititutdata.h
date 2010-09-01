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
* Description:
*
*/
#ifndef T_PENINPUTSPLITCHNITUTDATA_H
#define T_PENINPUTSPLITCHNITUTDATA_H

#include <peninputlayoutbasecontrol.h>
#include <aknfeppeninputenums.h>
#include <PtiDefs.h>
#include <e32keys.h>
#include <AknLayout2Def.h>


//#include "peninputsplititutcustomization.h"

const TInt KInvalidScanCode = -1;
const TInt KInvalidCtrlId = -1;

enum TItutDataType
    {
    EScreenSize,
    ELayoutOffset,
    ELayoutRect,
    EBackgroundRect,
    EBackspaceRect,
    EArrowLeftRect,
    EArrowRightRect,
    EArrowUpRect,
    EArrowDownRect,
    EOptionsRect,
    ECloseRect,
    EKeypadRect,
    EKeypadCellRects,
    EKeypadLeftTextLine,
    EKeypadRightTextLine1,
    EKeypadRightTextLine2,
    EKeypadRightTextLine3,
    EDropdownlistUnitWidth,
    EDropdownlistUnitHeight,
    EDropdownlistHorizontalMargin,
    EDropdownlistVerticalMargin,
    EDropdownListNaviWidth,
    EDropdownListSpinBtnHeight,
    EDropdownListSpellLTPos,
    EDropdownListCandsLTPos,
    EDropdownListPuncLTPos,
    EDropdownListFont,
    EDropdownListTextMargin,
    EDropdownListTextColor,
    ESpellRect,    // used to indicate stroke and zhuyin spelling
    ESpellFont,
    ESpellTextColor,
    ELanguage,
    EInputMode,
    ECase,
    EChnCandidates,
    EChnPuncCandidates,
    ELeftInnerRect,
    ERightInnerRect,
    EBackspaceInnerRect,
    EUpInnerRect,
    EDownInnerRect,
    ECloseInnerRect,
    EOptionInnerRect,
    ELatinOnly,
    EPreviewBubbleRect,
    EPreviewBubbleInnerRect,
    EPreviewBubbleFont,
    EPreviewBubbleTextlayout,
    ESpellICFRect,
    EBtnTextLine,
    EItutPosOk,
    EItutPosCancel,
    EIcfFont,
    ESpellEditIndicatorRect,   
    EImIndicatorRect,
    ESpellBackSpcae,
    ESpellBackSpcaeInner,    
    EIndiPaneWithoutTextRect, 
    EIndiIconWithoutTextRect,
    EIndiTextLine
    };

enum TControlId
    {
    ECtrlIdBackspace = 1000,
    ECtrlIdArrowLeft,
    ECtrlIdArrowRight,
    ECtrlIdArrowUp,
    ECtrlIdArrowDown,
    ECtrlIdOptions,
    ECtrlIdClose,
    ECtrlIdStdItut,
    ECtrlIdSpellICF,    // used to display spelling of stroke and zhuyin
    ECtrlIdSpellCandsList,
    ECtrlIdStdCandsList,
    ECtrlIdPuncCandsList,
    ECtrlIdBackground,  // used to display background
    ECtrlIdCancelWindow,
    ECtrlIdICF,
    ECtrlIdOkBtn,
    ECtrlIdCancelBtn,
    ECtrlIdLastStdCtrl = ECtrlIdCancelBtn+ 1
    };

enum TItutCommand
    {
    EItutCmdCandidateSelected = ECmdPenInputImeCmdBase + 1,
    EItutCmdGetNextCandidatePage,
    EItutCmdGetPreviousCandidatePage,
    EItutCmdCandidateExisted,
    EItutCmdPredictItemSelected,
    EItutCmdArrowUp,
    EItutCmdArrowDown,
    EItutExtCmdSizeChanged,
    EItutCmdCheckIcfEmpty
    };
    
enum TItutCandidatesIdx
    {
    EItutCandidates,
    EItutPuncCandidates
    };

enum TItutConfigurableKeys
    {
    EItutKeyArrowLeft,
    EItutKeyArrowRight,
    EItutKeyArrowUp,
    EItutKeyArrowDown,
    EItutKeyClear,
    EItutKeySwitchLayout,
    EItutKey1,
    EItutKey2,
    EItutKey3,
    EItutKey4,
    EItutKey5,
    EItutKey6,
    EItutKey7,
    EItutKey8,
    EItutKey9,
    EItutKeyStar,
    EItutKey0,
    EItutKeyHash
    };



class TItutKeyMapping
    {
public:
    TInt iKeyIdx;
    TInt iCtrlId;     // only valid when key is a button
    TInt iScanCode;   // only valid when key is a virtual key in keyboard
    };

const TItutKeyMapping ItutKeymap[] = 
    {
    {EItutKeyArrowLeft, ECtrlIdArrowLeft, KInvalidScanCode},
    {EItutKeyArrowRight, ECtrlIdArrowRight, KInvalidScanCode},
    {EItutKeyArrowUp, ECtrlIdArrowUp, KInvalidScanCode},
    {EItutKeyArrowDown, ECtrlIdArrowDown, KInvalidScanCode},
    {EItutKeyClear, ECtrlIdBackspace, KInvalidScanCode},
    {EItutKey1, KInvalidCtrlId, EPtiKey1},
    {EItutKey2, KInvalidCtrlId, EPtiKey2},
    {EItutKey3, KInvalidCtrlId, EPtiKey3},
    {EItutKey4, KInvalidCtrlId, EPtiKey4},
    {EItutKey5, KInvalidCtrlId, EPtiKey5},
    {EItutKey6, KInvalidCtrlId, EPtiKey6},
    {EItutKey7, KInvalidCtrlId, EPtiKey7},
    {EItutKey8, KInvalidCtrlId, EPtiKey8},
    {EItutKey9, KInvalidCtrlId, EPtiKey9},
    {EItutKeyStar, KInvalidCtrlId, EStdKeyNkpAsterisk},
    {EItutKey0, KInvalidCtrlId, EPtiKey0},
    {EItutKeyHash, KInvalidCtrlId, EStdKeyHash},
    };

const TInt LangsArr[] = 
    {
    1,   2,   3,   4,   5,   6,   7,   8,   9,   10, 
    11,  12,  13,  14,  15,  17,  18,  20,  21,  22,  
    23,  24,  25,  26,  27,  28,  39,  44,  45,  46,  
    47,  48,  49,  51,  59,  67,  68,  70,  76,  78,  
    79,  82,  83,  85,  91,  96,  102, 103, 401, 402,
    16,  42,  93,  54,  55,  57,  37,  50,  94,  33, 
    29,  30,  31,  58,  72,
    };

_LIT(KLatinExt, "latin");
_LIT(KCyrillicExt, "cyrillic");
_LIT(KGreekExt, "greek");
_LIT(KHebrewExt, "hebrew");
_LIT(KArabicExt, "arabic");
_LIT(KFarsiExt, "farsi");
_LIT(KUrduExt, "urdu");
_LIT(KThaiExt, "thai");
_LIT(KChnExt, "chinese");
_LIT(KDevanagiriExt, "devanagari");

enum TScriptExtIndex
    {
    ELatinExt,
    ECyrillicExt,
    EGreekExt,
    EHebrewExt,
    EArabicExt,
    EFarsiExt,
    EUrduExt,
    EThaiExt,
    EChnExt,
    EDevanagiriExt
    };

struct TScriptInfo
    {
    TBool iCaseSensitive;
    TInt iStartIdx;   // lang start index in langs
    TInt iEndIdx;     // lang end index in langs
    TScriptExtIndex iScriptExt;
    };
    
const TScriptInfo TScriptMapping[] = 
    {
    {ETrue,  0,  49, ELatinExt},
    {ETrue,  50, 52, ECyrillicExt},
    {ETrue,  53, 54, EGreekExt},
    {EFalse, 55, 55, EHebrewExt},
    {EFalse, 56, 56, EArabicExt},
    {EFalse, 57, 57, EFarsiExt},
    {EFalse, 58, 58, EUrduExt},
    {EFalse, 59, 59, EThaiExt},
    {ETrue, 60, 62, EChnExt}, // chinese also include abc
    {ETrue, 63, 64, EDevanagiriExt},
    };

#endif  // T_PENINPUTSPLITCHNITUTDATA_H

//End Of File