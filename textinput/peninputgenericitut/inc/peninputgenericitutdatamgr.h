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
* Description:  drop-down list control
*
*/


#ifndef C_GENERICITUTDATAMGR_H
#define C_GENERICITUTDATAMGR_H

#include <e32base.h>
#include <PtiDefs.h>
#include <AknLayout2Def.h>

#include "peninputgenericitutpropertysubscriber.h"
#include "peninputgenericitutdata.h"

class CPtiEngine;
class CFont;
class MItutLayoutContext;
class CGenericItutWindowManager;

extern const TInt KInvalidResId;
extern const TInt KMaxPredictCandCnt;
extern const TInt KInvalidImMode;
const TInt KMaxFileLength = 80;


class CGenericItutDataMgr : public CBase
    {
public:    
    /**
     * struct SItem
     * Choice list item data structure.
     * 
     */
    struct TMenuItem
        {
        /*
         * Command of choice list item.
         */
        TInt iCommand;

        /*
         * Title of choice list item.
         */
        TBuf<100> iText;
        };    
public:
    static CGenericItutDataMgr* NewL(MItutLayoutContext* aLayoutContext, CPtiEngine* aPtiEngine );

    ~CGenericItutDataMgr();

    void AddSubscriber(MItutPropertySubscriber* aSubscriber);

    // should be called by owner when initialize data manager, and when SizeChanged
    void ReadLafInfo();

    void SetLanguageL(TInt aLanguage);

    void SetInputModeL(TInt aMode);

    void SetCase(TInt aCase);

    TAny* RequestData(TInt aDataType);
    TAny* RequestDataForPortraitWestUIAndChineseSpellUI(TInt aDataType);

    inline CPtiEngine* PtiEngine();

    // if there is need to change keypad layout, aKeypadResId: new keypad resource id
    void NotifyChangeControlLayout(MItutPropertySubscriber::TItutProperty aPropertyName, 
                                   TInt aCtrlResId);

    inline TInt InputLanguage();
	inline TBool IsValidLanguage(TInt aLanguage);
    inline TInt InputMode();
    
    inline TInt TextCase();

    inline TBool IsLandScape();
    
    inline TBool IsSpellMode();

    TBool IsExistPlusChar();
    inline void SetSpellMode(TBool aSpellMode);
    
    inline void SetIndicatorData(TFepIndicatorInfo &aIndicatorData);
    inline TFepIndicatorInfo IndicatorData();
    

    inline void SetLatinOnly( TBool aLatinOnly );

    inline TBool IsPortraitWest();
    
	void SetUpdate(TBool aNeedUpdate);
	TBool IsUpdate() const;
    // when user press navi-next button on dropdownlist, there is need to 
    // retrieve next page candidate
    TInt GetNextPageCandidateL(RPointerArray<HBufC>& aList);

    TBool NextPageCandidateExist();

    void ClearChnCandidates(TInt aCmd);

    TInt KeypadResourceId();
    
    TBool IsChinese();
    TBool IsKorean(); // tp teleca addon
    TBool IsHindi();
    TBool IsThai();

    // Is Right to Left language
    TBool IsRtoLLanguage();
    TBool IsRtoLLanguage( TInt aLanguage );    
    TBool IsLangDirectionSwitch() const;
    inline void SetLangDirectionSwitch( TBool aIsChange );
        
    void SetTextAlignment();
    
    void SetIcfTextAlignment(TInt aAlignment);
    
    void SetNumericKeymap( const TInt aKeymap );
    
    void SetNumericKeymapData( TInt* aKeymapData );
    
    inline TBool IsCharFlagChanged();
    
    inline void SetNumericOnly( TBool aIsNumberOnly );
    
    inline TBool IsNumericOnly() const;
	
    TMenuItem* GetMenuItem(TInt aCommand); 
    void SetChineseSpellFlag( TBool aFlag );
    
    TBool IsChineseSpellMode();
    	
	TPoint iSelectionTl;
    TSize iSelectionItemSize;
    TInt iSelectionItemMargin;
    TInt iSelectionHorMargin;
    TInt iSelectionVerMargin;
    TSize iSelectionNaviSize;
    TSize iSelectionNaviInnerSize;
    TAknTextLineLayout iSelectionTextLine;
    TAknTextLineLayout iSelectionPageTextLine;
    
    TInt iIcfTextLeftMargin;
	TInt iIcfTextRightMargin;
	TInt iIcfTextTopMargin;
	TInt iIcfTextBottomMargin;
	TInt iIcfTextLineSpaceMargin;
	TInt iIcfTextHeight;
	
	TInt iIcfTextLeftMarginCn;
	TInt iIcfTextRightMarginCn;
	TInt iIcfTextTopMarginCn;
	TInt iIcfTextBottomMarginCn;
	TInt iIcfTextLineSpaceMarginCn;
	TInt iIcfTextHeightCn;
	
	TRect iShiftIconRect;
	TInt iIcfTextLeftMarginForPrtWest;
    TInt iIcfTextRightMarginForPrtWest;
    TInt iIcfTextTopMarginForPrtWest;
    TInt iIcfTextBottomMarginForPrtWest;
    TInt iIcfTextLineSpaceMarginForPrtWest;
    TInt iIcfTextHeightForPrtWest;
    TRect iShiftIconRectForPrtWest;
    TRect iStarIconRectForPrtWest;
    TInt iSpellIcfTextHeightForPrtWest;
    
	TInt iSpellIcfTextLeftMarginForPrtWest;
    TInt iSpellIcfTextRightMarginForPrtWest;
    TInt iSpellIcfTextTopMarginForPrtWest;
    TInt iSpellIcfTextBottomMarginForPrtWest;
    TInt iSpellIcfTextLineSpaceMarginForPrtWest;
private:
    CGenericItutDataMgr(MItutLayoutContext* aLayoutContext, CPtiEngine* aPtiEngine );

    void ConstructL();

    void NotifySubscriber( MItutPropertySubscriber::TItutProperty aPropertyName, 
                           const TDesC& aValue);

    // return current script index
    TInt GetConfigResFileName(TInt aLang);
    
    // return index in aArray
    TInt FindLang(const TInt* aArray, TInt aEndIdx, TInt aLang);
    
    static const TDesC& ResFileExtByScriptIdx(const TScriptExtIndex aIndex);

    // whether both of aMode1 & aMode2 are chinese mode, or both of them are western mode
   
    void CalWesternIndicator();
    
    TInt ChineseKeypadResourceId();
    
    TInt HindiKeypadResourceId();
    TInt KoreanKeypadResourceId();
    void ReadLafInfoForPrtWest();
    
private: // data member
    RPointerArray<MItutPropertySubscriber> iSubscriberList;
    CPtiEngine* iPtiEngine;
    TInt iLanguage;
    TInt iInputMode;
    TInt iCase;
    TBool iIsChineseSpell;

    TSize iScreenSize;

    // offset of layout, compare to screen
    TPoint iLayoutOffset;
    TRect iLayoutRect;

    TRect iIcfWesRect;
    TRect iIcfChnRect;
    CFont* iIcfFont;
    CFont* iIcfFontCn;
    TInt iIcfTextAlignment;

    TRect iBackspaceRect;
    TRect iBackspaceInnerRect;
    TRect iBackspaceCnRect;
    TRect iBackspaceInnerCnRect;
    TRect iSwitchRect;
    TRect iSwitchInnerRect;
    TRect iSwitchCnRect;
    TRect iSwitchInnerCnRect;
    TRect iArrowLeftRect;
    TRect iArrowLeftInnerRect;
    TRect iArrowLeftCnRect;
    TRect iArrowLeftInnerCnRect;
    TRect iArrowRightRect;
    TRect iArrowRightInnerRect;
    TRect iArrowRightCnRect;
    TRect iArrowRightInnerCnRect;
    TRect iArrowUpRect;
    TRect iArrowDownRect;  
    TRect iArrowUpInnerRect;
    TRect iArrowDownInnerRect;
    

    TRect iImIndicatorWesRect;
    TRect iCalindwesternRect;
    TRect iImIndiOuterRect;
    TRect iImIndiOuterRectCn;
    TRect iImIndicatorChnRect;

    TRect iSendRect;
    TRect iDisconnRect;

    TRect iKeypadRect;  
    TRect iKeypadRectCn;
    TRect iKeypadRectQuery;
    RArray<TRect> iKeypadCellRects;
    RArray<TRect> iKeypadCellRectsCn;
    RArray<TRect> iKeypadCellRectsQuery;
	
    // properties of candidate list
    TInt iCandsUnitWidth;
    TInt iCandsUnitHeight;
    TInt iCandsHorizontalMargin;
    TInt iCandsVerticalMargin;
    TInt iCandsNaviWidth;
    TInt iCandsSpinBtnHeight;    
    TPoint iCandsSpellLTPos;
    TPoint iCandsLTPos;
    TPoint iCandsPuncLTPos;
    CFont* iCandsFont;
    TInt iCandsTextMargin;
    TRgb iCandsTextColor;    



    TRect iSpellRect;
    CFont* iSpellFont;
    TRgb iSpellTextColor;

    TAknTextLineLayout iVkAlphaText1;
    TAknTextLineLayout iVkAlphaText2;
    TAknTextLineLayout iVkAlphaText3;
    TAknTextLineLayout iVkNumText;
    
    // Layout for Thai
    TAknTextLineLayout iVkAlphaText1ForThai;
    TAknTextLineLayout iVkAlphaText2ForThai;
    TAknTextLineLayout iVkAlphaText3ForThai;

    // resource id for generic itut resource file
    TInt iCommonResId;
    RArray<TInt> iConfigResIds;

    TInt iAvkonResId;
    
    // not own
    MItutLayoutContext* iLayoutContext;

    // chinese related candidates
    RPointerArray<HBufC> iCandidates;
    RPointerArray<HBufC> iPuncCandidates;

    TRect iOptionsRect;
    TRect iCloseRect;
    TRect iCloseInnerRect;
    TRect iOptionInnerRect;
    TRect iSpellIndiRect;
    TRect iOptionsRectCn;
    TRect iCloseRectCn;    
    TRect iCloseInnerRectCn;
    TRect iOptionInnerRectCn;
    
    // preview bubble
    TRect iPreviewWndRect;
    TRect iPreviewWndInnerRect;
    CFont* iBubbleFont;
	TAknTextLineLayout iPreviewWndText;

    TBuf<KMaxFileLength> iResourceFilename;
    TBool iLangCaseSensitive;
    TInt iCurrentScriptIdx;

    TBool iLandScape;
    TBool iSpellMode;
    
    // spell popup window data
    TRect iSpellQueryPaneRect;
    TRect iSpellICFRect;
    TRect iSpellClrRect;
    TRect iSpellClrInnerRect;
    TRect iOkRect;
    TRect iCancelRect;
    TRect iSpellEditIndicator;
    TAknTextLineLayout iBtnTextFormat;
    TAknTextLineLayout iBtnTextFormat1; 
    TFepIndicatorInfo iIndicatorData; 
    
    TRect iSpellQueryPaneRectForPrtWest;
    TRect iOKRectForPrtWest;
    TRect iCancelRectForPrtWest;
    TAknTextLineLayout iBtnOKTextForPrtWest;
    TAknTextLineLayout iBtnCancelTextForPrtWest;
    TRect iSpellICFRectForPrtWest;
    TRect iSpellICFBtnRectForPrtWest;
    TRect iSpellICFBtnInnerRectForPrtWest;
    TAknTextLineLayout iSpellText1ForPrtWest;
    TAknTextLineLayout iSpellText2ForPrtWest;
    TAknTextLineLayout iSpellText3ForPrtWest;
    TRect iSpellIndiPaneWithoutTextForPrtWest;
    TRect iSpellIndiIconWithoutTextForPrtWest;
    CFont* iSpellIcfFontForPrtWest;
    TRect iSpellArrowLeftRectForPrtWest;
    TRect iSpellArrowLeftInnerRectForPrtWest;
    TRect iSpellArrowRightRectForPrtWest;
    TRect iSpellArrowRightInnerRectForPrtWest;
    RPointerArray<TMenuItem> iMenuItemList;

    TBool iNeedUpdate;
    TBool iLatinOnly;
	
	TBool iIsLangDirectionSwitch;    
	
	TInt iNumericKeymap;
	HBufC* iNumericKeymapData;
	TBool iExistPlusChar;
	TBool iPlusCharFlagChanged;
	TBool iNumberOnly;

	TRect iLayoutRectForPrtWest;
	TPoint iLayoutOffsetForPrtWest;
	RArray<TRect> iKeypadCellRectsForPrtWest;
	TRect iKeypadRectForPrtWest;
    TAknTextLineLayout iVkAlphaText1ForPrtWest;
    TAknTextLineLayout iVkAlphaText2ForPrtWest;
    TAknTextLineLayout iVkAlphaText3ForPrtWest;
    TAknTextLineLayout iVkNumTextForPrtWest;
    TAknTextLineLayout iVkBigNumTextForPrtWest;
    TAknTextLineLayout iVkTextPlusForNumbericOnlyForPrtWest;
    TRect iCloseRectForPrtWest;
    TRect iCloseInnerRectForPrtWest;
    TRect iArrowLeftRectForPrtWest;
    TRect iArrowLeftInnerRectForPrtWest;
    TRect iOptionsRectForPrtWest;
    TRect iOptionInnerRectForPrtWest;
    TRect iArrowRightRectForPrtWest;
    TRect iArrowRightInnerRectForPrtWest;
    TRect iBackspaceRectForPrtWest;
    TRect iBackspaceInnerRectForPrtWest;
    TRect iPreviewWndRectForPrtWest;
    TRect iPreviewWndInnerRectForPrtWest;
    TAknTextLineLayout iPreviewWndTextForPrtWest;
    CFont* iBubbleFontForPrtWest;
    TRect iIcfRectForPrtWest;
    CFont* iIcfFontForPrtWest;
    TInt iIcfTextAlignmentForPrtWest;
    
    TRect iIndiPaneRectWithTextForPrtWest;
    TRect iIndiPaneRectWithoutTextForPrtWest;
    TRect iIndiIconRectWithTextForPrtWest;
    TRect iIndiIconRectWithoutTextForPrtWest;
    TAknTextLineLayout iIndiTextForPrtWest;
    };

inline CPtiEngine* CGenericItutDataMgr::PtiEngine()
    {
    return iPtiEngine;
    }
    
inline TInt CGenericItutDataMgr::InputMode()
    {
    return iInputMode;
    }
    
inline TInt CGenericItutDataMgr::InputLanguage()
    {
    return iLanguage;
    }

inline TInt CGenericItutDataMgr::TextCase()
    {
    return iCase;
    }

inline TBool CGenericItutDataMgr::IsLandScape()
    {
    return iLandScape;
    }

inline TBool CGenericItutDataMgr::IsSpellMode()
    {
    return iSpellMode;
    }

inline void CGenericItutDataMgr::SetSpellMode(TBool aSpellMode)
    {
    iSpellMode = aSpellMode;
    }
    
inline void CGenericItutDataMgr::SetIndicatorData(TFepIndicatorInfo &aIndicatorData)
    {
    iIndicatorData.iIndicatorImgID = aIndicatorData.iIndicatorImgID;
    iIndicatorData.iIndicatorMaskID = aIndicatorData.iIndicatorMaskID;
    iIndicatorData.iIndicatorTextImgID = aIndicatorData.iIndicatorTextImgID;
    iIndicatorData.iIndicatorTextMaskID = aIndicatorData.iIndicatorTextMaskID;
    }
    
inline TFepIndicatorInfo CGenericItutDataMgr::IndicatorData()
    {
    return iIndicatorData;
    } 
    

inline void CGenericItutDataMgr::SetLatinOnly( TBool aLatinOnly )
    {
    iLatinOnly = aLatinOnly;
    } 

inline TBool CGenericItutDataMgr::IsPortraitWest()
    {
    return !iLandScape && !IsChinese() && !IsKorean();
    }
inline TBool CGenericItutDataMgr::IsValidLanguage(TInt aLanguage)
	{
	return (aLanguage != ELangTest && aLanguage != ELangNone);	
	}
	
inline void CGenericItutDataMgr::SetLangDirectionSwitch( TBool aIsChange )
	{
	iIsLangDirectionSwitch = aIsChange;
	}

inline TBool CGenericItutDataMgr::IsCharFlagChanged()
    {
    if( iExistPlusChar != IsExistPlusChar() )
    	{
    	iExistPlusChar = IsExistPlusChar();
        iPlusCharFlagChanged = ETrue;
    	}
    else
    	{
        iPlusCharFlagChanged = EFalse;
    	}
    return iPlusCharFlagChanged;
    }

inline void CGenericItutDataMgr::SetNumericOnly( TBool aIsNumberOnly )
	{
	iNumberOnly = aIsNumberOnly;
	}

inline TBool CGenericItutDataMgr::IsNumericOnly() const
	{
	return iNumberOnly;
	}

#endif // C_GENERICITUTDATAMGR_H
