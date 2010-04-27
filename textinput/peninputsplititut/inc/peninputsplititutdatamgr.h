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
* Description:  split itut data manager
*
*/


#ifndef C_SPLITITUTDATAMGR_H
#define C_SPLITITUTDATAMGR_H

#include <e32base.h>
#include <PtiDefs.h>
#include <AknLayout2Def.h>

#include "peninputsplititutpropertysubscriber.h"
#include "peninputsplititutdata.h"

class CPtiEngine;
class CFont;
class MSplitItutLayoutContext;
class CSplitItutWindowManager;

extern const TInt KInvalidResId;
extern const TInt KMaxPredictCandCnt;
extern const TInt KInvalidImMode;
const TInt KMaxFileLength = 80;

/**
* Class  CSplitItutDataMgr
*  
* Wrapper of L&F data.
*
* @lib peninputsplititut.lib
* @since S60 v5.0
*/

class CSplitItutDataMgr : public CBase
    {  
public:
    /**
     * Symbian constructor.
     *
     * @since S60 v5.0
     * @return Pointer to created object.
     */ 
    static CSplitItutDataMgr* NewL( MSplitItutLayoutContext* aLayoutContext, 
                                    CPtiEngine* aPtiEngine );

    /**
     * standard c++ destructor.
     *
     * @since S60 v5.0
     * @return None
     */ 
    ~CSplitItutDataMgr();

    /**
     * Add subscriber
     *
     * @since S60 v5.0
     * @param aSubscriber sub scriber
     * @return None
     */
    void AddSubscriber(MItutPropertySubscriber* aSubscriber);

    /**
     * read laf data.
     *
     * @since S60 v5.0
     * @return None
     */
    void ReadLafInfo();

    /**
     * Set input language
     *
     * @since S60 v5.0
     * @param aLanguage input language
     * @return None
     */
    void SetLanguageL(TInt aLanguage);

    /**
     * read laf data.
     *
     * @since S60 v5.0
     * @param aMode input mode  
     * @return None
     */
    void SetInputModeL(TInt aMode);

    /**
     * Set input case
     *
     * @since S60 v5.0
     * @param aCase input case  
     * @return None
     */
    void SetCase(TInt aCase);

    /**
     * Request data
     *
     * @since S60 v5.0
     * @param aDataType data type 
     * @return date
     */
    TAny* RequestData(TInt aDataType);

    /**
     * Get ptiengine pointer
     *
     * @since S60 v5.0
     * @return ptiengine
     */
    inline CPtiEngine* PtiEngine();

    /**
     * Notify change control layout
     *
     * @since S60 v5.0
     * @param aPropertyName property name
     * @param aCtrlResId control id  
     * @return None
     */
    void NotifyChangeControlLayout(MItutPropertySubscriber::TItutProperty aPropertyName, 
                                   TInt aCtrlResId);

    /**
     * Get input language
     *
     * @since S60 v5.0
     * @return language id
     */
    inline TInt InputLanguage();
    
    /**
     * Is valid language
     *
     * @since S60 v5.0
     * @param aLanguage language id    
     * @return TBool
     */
	inline TBool IsValidLanguage(TInt aLanguage);
	
    /**
     * Get input mode
     *
     * @since S60 v5.0
     * @return input mode
     */
    inline TInt InputMode();
    
    /**
     * Get text case
     *
     * @since S60 v5.0
     * @return text case
     */
    inline TInt TextCase();

    /**
     * Is exist plus char
     *
     * @since S60 v5.0
     * @return TBool
     */
    TBool IsExistPlusChar();
    
    /**
     * Set latin only flag
     *
     * @since S60 v5.0
     * @param aLatinOnly latin only flag
     * @return TBool
     */
    inline void SetLatinOnly( TBool aLatinOnly );

    /**
     * Is in Spell mode
     *
     * @since S60 v5.0
     * @return TBool
     */    
    inline TBool IsSpellMode();
    
    /**
     * Set Spell mode flag
     *
     * @since S60 v5.0
     * @param aSpellMode Spell mode flag
     * @return void
     */
    inline void SetSpellMode(TBool aSpellMode);
    
    /**
     * Set Indicator Data
     *
     * @since S60 v5.0
     * @param aIndicatorData Indicator Data
     * @return void
     */
    inline void SetIndicatorData(TFepIndicatorInfo &aIndicatorData);
    
    /**
     * Get Indicator Data
     *
     * @since S60 v5.0
     * @return TFepIndicatorInfo
     */
    inline TFepIndicatorInfo IndicatorData();
    
    /**
     * Set update flag
     *
     * @since S60 v5.0
     * @param aNeedUpdate need update flag
     * @return None
     */
	void SetUpdate(TBool aNeedUpdate);

    /**
     * get update flag
     *
     * @since S60 v5.0
     * @return update flag
     */
	TBool IsUpdate() const;

//    TInt GetNextPageCandidateL(RPointerArray<HBufC>& aList);

//    TBool NextPageCandidateExist();

//    void ClearChnCandidates(TInt aCmd);

    /**
     * get keypad resource id
     *
     * @since S60 v5.0
     * @return keypad resource id
     */
    TInt KeypadResourceId();
    
    /**
     * Is chinese
     *
     * @since S60 v5.0
     * @return TBool
     */
    TBool IsChinese();
    
    /**
     * Is Hindi
     *
     * @since S60 v5.0
     * @return TBool
     */
    TBool IsHindi();
    
    /**
     * Is Thai
     *
     * @since S60 v5.0
     * @return TBool
     */
    TBool IsThai();

    /**
     * Is right to left language
     *
     * @since S60 v5.0
     * @return TBool
     */
    TBool IsRtoLLanguage();
    
    /**
     * Is right to left language
     *
     * @since S60 v5.0
     * @param aLanguage language id
     * @return TBool
     */
    TBool IsRtoLLanguage( TInt aLanguage );
    
    /**
     * Is language direction switch
     *
     * @since S60 v5.0
     * @return TBool
     */
    TBool IsLangDirectionSwitch() const;
    
    /**
     * set language direction switch
     *
     * @since S60 v5.0
     * @param aIsChange change flag
     * @return None
     */
    inline void SetLangDirectionSwitch( TBool aIsChange );
    
    /**
     * set numeric keymap
     *
     * @since S60 v5.0
     * @param aKeymap
     * @return None
     */
    void SetNumericKeymap( const TInt aKeymap );
    
    /**
     * set numeric keymap data
     *
     * @since S60 v5.0
     * @param aKeymapData
     * @return None
     */
    void SetNumericKeymapData( TInt* aKeymapData );
    
    /**
     * is char flag changed
     *
     * @since S60 v5.0
     * @return TBool
     */
    inline TBool IsCharFlagChanged();
    
    /**
     * set numeric only
     *
     * @since S60 v5.0
     * @param aIsNumberOnly
     * @return None
     */
    inline void SetNumericOnly( TBool aIsNumberOnly );
    
    /**
     * get numeric only flag
     *
     * @since S60 v5.0
     * @return TBool
     */
    inline TBool IsNumericOnly() const;
	
	TRect iShiftIconRect;
	TRect iStarIconRect;
	
private:
    /**
     * C++ constructor
     *
     * @since S60 v5.0
     * @return None
     */
    CSplitItutDataMgr(MSplitItutLayoutContext* aLayoutContext, CPtiEngine* aPtiEngine );

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v5.0
     * @return None
     */
    void ConstructL();

    /**
     * Notify subscriber
     * 
     * @since S60 v5.0
     * @param aPropertyName
     * @param aValue
     * @return None
     */
    void NotifySubscriber( MItutPropertySubscriber::TItutProperty aPropertyName, 
                           const TDesC& aValue);

    /**
     * Get config resource file name
     * 
     * @since S60 v5.0
     * @param aLang Language id
     * @return None
     */
    TInt GetConfigResFileName(TInt aLang);
    
    /**
     * Find language in an array
     * 
     * @since S60 v5.0
     * @param aArray
     * @param aEndIdx
     * @param aLang
     * @return index in the array
     */
    TInt FindLang(const TInt* aArray, TInt aEndIdx, TInt aLang);
    
    /**
     * get resource file string
     * 
     * @since S60 v5.0
     * @param aIndex
     * @return resource file
     */
    static const TDesC& ResFileExtByScriptIdx(const TScriptExtIndex aIndex);
   
    /**
     * Get chinese keypad resource id
     * 
     * @since S60 v5.0
     * @return resource id
     */
    TInt ChineseKeypadResourceId();
    
    /**
     * Get chinese keypad resource id
     * 
     * @since S60 v5.0
     * @return resource id
     */
    TInt HindiKeypadResourceId();
public: 
    TInt iIcfTextLeftMargin;
	TInt iIcfTextRightMargin;
	TInt iIcfTextTopMargin;
	TInt iIcfTextBottomMargin;
	TInt iIcfTextLineSpaceMargin;
	TInt iIcfTextHeight;
    
private: // data member
    RPointerArray<MItutPropertySubscriber> iSubscriberList;
    CPtiEngine* iPtiEngine;
    TInt iLanguage;
    TInt iInputMode;
    TInt iCase;

    TSize iScreenSize;

    // offset of layout, compare to screen
    TPoint iLayoutOffset;
    TRect iLayoutRect;
    TRect iLayoutRectSpell;
    TRect iBackgroundRect;

    TRect iBackspaceRect;
    TRect iBackspaceInnerRect;
    TRect iBackspaceCnRect;
    TRect iBackspaceInnerCnRect;
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

    TRect iKeypadRect;  
    TRect iKeypadRectCn;
    TRect iKeypadRectQuery;
    RArray<TRect> iKeypadCellRects;
//    RArray<TRect> iKeypadCellRectsCn;
	
    // properties of candidate list
    /*
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
    */

    TAknTextLineLayout iVkBigNumTextForPrtWest;
    TAknTextLineLayout iVkAlphaText1;
    TAknTextLineLayout iVkAlphaText2;
    TAknTextLineLayout iVkAlphaText3;
    TAknTextLineLayout iVkNumText;
    TAknTextLineLayout iVkTextPlusForNumbericOnly;

    // resource id for generic itut resource file
    TInt iCommonResId;
    TInt iConfigResId;

    TInt iAvkonResId;
    
    // not own
    MSplitItutLayoutContext* iLayoutContext;

    // chinese related candidates
//    RPointerArray<HBufC> iCandidates;
//    RPointerArray<HBufC> iPuncCandidates;

    TRect iOptionsRect;
    TRect iCloseRect;
    TRect iCloseInnerRect;
    TRect iOptionInnerRect;
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

    TAknTextLineLayout iBtnTextFormat;
    TAknTextLineLayout iBtnTextFormat1; 
    

    TBool iNeedUpdate;
    TBool iLatinOnly;
	
	TBool iIsLangDirectionSwitch;    
	
	TInt iNumericKeymap;
	HBufC* iNumericKeymapData;
	TBool iExistPlusChar;
	TBool iPlusCharFlagChanged;
	TBool iNumberOnly;
	// For spell mode
	TBool iSpellMode;
	TRect iSpellICFRect;
	TRect iSpellEditIndicator;
	TRect iOkRect;
	TRect iCancelRect;
	CFont* iIcfFont;
	TInt iIcfTextAlignment;
	
	// Spell Back space rect
	TRect iSpellBackSpcace;
	TRect iSpellBackSpcaceInner;
	
	// Left&Right arrow
	TRect iSpellArrowLeftRect;
	TRect iSpellArrowLeftRectInner;
	TRect iSpellArrowRightRect;
	TRect iSpellArrowRightRectInner;
	
    //TRect iIndiPaneRectWithTextForPrtWest;
    TRect iIndiPaneRectWithoutTextForPrtWest;
    //TRect iIndiIconRectWithTextForPrtWest;
    TRect iIndiIconRectWithoutTextForPrtWest;
    TAknTextLineLayout iIndiTextForPrtWest;

	TAknTextLineLayout iSpellBtnTextFormat;
	TFepIndicatorInfo iIndicatorData;
    };

// ---------------------------------------------------------------------------
// CSplitItutDataMgr::PtiEngine
// ---------------------------------------------------------------------------
//
inline CPtiEngine* CSplitItutDataMgr::PtiEngine()
    {
    return iPtiEngine;
    }

// ---------------------------------------------------------------------------
// CSplitItutDataMgr::InputMode
// ---------------------------------------------------------------------------
//
inline TInt CSplitItutDataMgr::InputMode()
    {
    return iInputMode;
    }
    
// ---------------------------------------------------------------------------
// CSplitItutDataMgr::InputLanguage
// ---------------------------------------------------------------------------
//
inline TInt CSplitItutDataMgr::InputLanguage()
    {
    return iLanguage;
    }

// ---------------------------------------------------------------------------
// CSplitItutDataMgr::TextCase
// ---------------------------------------------------------------------------
//
inline TInt CSplitItutDataMgr::TextCase()
    {
    return iCase;
    }
    
// ---------------------------------------------------------------------------
// CSplitItutDataMgr::SetLatinOnly
// ---------------------------------------------------------------------------
//
inline void CSplitItutDataMgr::SetLatinOnly( TBool aLatinOnly )
    {
    iLatinOnly = aLatinOnly;
    } 

// ---------------------------------------------------------------------------
// CSplitItutDataMgr::IsSpellMode
// ---------------------------------------------------------------------------
//
inline TBool CSplitItutDataMgr::IsSpellMode()
    {
    return iSpellMode;
    }

// ---------------------------------------------------------------------------
// CSplitItutDataMgr::SetSpellMode
// ---------------------------------------------------------------------------
//
inline void CSplitItutDataMgr::SetSpellMode(TBool aSpellMode)
    {
    iSpellMode = aSpellMode;
    }
 
// ---------------------------------------------------------------------------
// CSplitItutDataMgr::SetIndicatorData
// ---------------------------------------------------------------------------
//
inline void CSplitItutDataMgr::SetIndicatorData(TFepIndicatorInfo &aIndicatorData)
    {
    iIndicatorData.iIndicatorImgID = aIndicatorData.iIndicatorImgID;
    iIndicatorData.iIndicatorMaskID = aIndicatorData.iIndicatorMaskID;
    iIndicatorData.iIndicatorTextImgID = aIndicatorData.iIndicatorTextImgID;
    iIndicatorData.iIndicatorTextMaskID = aIndicatorData.iIndicatorTextMaskID;
    }
 
// ---------------------------------------------------------------------------
// CSplitItutDataMgr::IndicatorData
// ---------------------------------------------------------------------------
//
inline TFepIndicatorInfo CSplitItutDataMgr::IndicatorData()
    {
    return iIndicatorData;
    } 


// ---------------------------------------------------------------------------
// CSplitItutDataMgr::IsValidLanguage
// ---------------------------------------------------------------------------
//
inline TBool CSplitItutDataMgr::IsValidLanguage(TInt aLanguage)
	{
	return (aLanguage != ELangTest && aLanguage != ELangNone);	
	}
	
// ---------------------------------------------------------------------------
// CSplitItutDataMgr::SetLangDirectionSwitch
// ---------------------------------------------------------------------------
//
inline void CSplitItutDataMgr::SetLangDirectionSwitch( TBool aIsChange )
	{
	iIsLangDirectionSwitch = aIsChange;
	}

// ---------------------------------------------------------------------------
// CSplitItutDataMgr::IsCharFlagChanged
// ---------------------------------------------------------------------------
//
inline TBool CSplitItutDataMgr::IsCharFlagChanged()
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

// ---------------------------------------------------------------------------
// CSplitItutDataMgr::SetNumericOnly
// ---------------------------------------------------------------------------
//
inline void CSplitItutDataMgr::SetNumericOnly( TBool aIsNumberOnly )
	{
	iNumberOnly = aIsNumberOnly;
	}

// ---------------------------------------------------------------------------
// CSplitItutDataMgr::IsNumericOnly
// ---------------------------------------------------------------------------
//
inline TBool CSplitItutDataMgr::IsNumericOnly() const
	{
	return iNumberOnly;
	}

#endif // C_SPLITITUTDATAMGR_H
