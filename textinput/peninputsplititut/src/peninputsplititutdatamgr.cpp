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
* Description:  split itu-t data manager
*
*/


#include <PtiEngine.h>
#include <coemain.h>
#include <AknUtils.h>
#include <peninputsplititutnew.rsg>
#include <peninputsplititutconfig_chinese.rsg>
#include <peninputsplititutconfig_latin.rsg>
#include <peninputsplititutconfig_hebrew.rsg>
#include <peninputsplititutconfig_farsi.rsg>
#include <AknFepGlobalEnums.h>
#include <eikon.hrh>
#include <bautils.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <peninputlayoutmultilineicf.h>
#include <AknSettingCache.h>

#include "peninputsplititutdatamgr.h"
#include "peninputsplititutlayoutcontext.h"
#include "peninputsplititutconverter.h"
#include "peninputsplititutwindowmanager.h"

#define LAF_MODIFY 1
_LIT(KItutCommonResFile, "z:\\resource\\plugins\\peninputsplititutnew.rsc");
_LIT(KConfigResourceFile, 
     "z:\\resource\\plugins\\peninputsplititutconfig_");
_LIT(KResourceFileExtName, ".rsc");

_LIT(KAvkonResourceFile, "z:\\resource\\avkon.rsc");
     
const TInt KOnePageCandidateCount = 40;
const TInt KMaxPredictCandCnt = 250;
const TInt KInvalidResId = -1;
const TInt KInvalidImMode = -1;
const TInt KInvalidIndex = -1;
const TUint16 KPlusChar = '+';


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CSplitItutDataMgr::NewL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CSplitItutDataMgr* CSplitItutDataMgr::NewL(MSplitItutLayoutContext* aLayoutContext, 
                                               CPtiEngine* aPtiEngine)
    {
    CSplitItutDataMgr* self = new (ELeave) CSplitItutDataMgr(aLayoutContext, aPtiEngine);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);//self
    return self;
    }

/*
// ---------------------------------------------------------------------------
// CSplitItutDataMgr::ClearChnCandidates
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CSplitItutDataMgr::ClearChnCandidates(TInt aCmd)
    {
    switch (aCmd)
        {
        case EItutCandidates:
            {
            iCandidates.ResetAndDestroy();
            iCandidates.Close();
            }
            break;
        case EItutPuncCandidates:
            {
            iPuncCandidates.ResetAndDestroy();
            iPuncCandidates.Close();
            }
            break;
        default:
            break;
        }
    }
*/

// ---------CSplitItutDataMgr----------------------------------------
// CSplitItutDataMgr::~CSplitItutDataMgr
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CSplitItutDataMgr::~CSplitItutDataMgr()
    {
    iSubscriberList.Close();
    
    CCoeEnv::Static()->DeleteResourceFile(iAvkonResId);      
    CCoeEnv::Static()->DeleteResourceFile(iCommonResId);
    
    if (iConfigResId)
        {
        CCoeEnv::Static()->DeleteResourceFile(iConfigResId);
        }

//    ClearChnCandidates(EItutCandidates);
//    ClearChnCandidates(EItutPuncCandidates);

    iKeypadCellRects.Close();
//    iKeypadCellRectsCn.Close(); 
    delete iNumericKeymapData;
    }

// ---------------------------------------------------------------------------
// CSplitItutDataMgr::ConstructL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CSplitItutDataMgr::ConstructL()
    {
    // initialize layout info of controls and layout
    ReadLafInfo();    
    TFileName resourceCommonName(KItutCommonResFile);
    CCoeEnv* coeEnv = CCoeEnv::Static();
    BaflUtils::NearestLanguageFile(coeEnv->FsSession(), resourceCommonName);
    iCommonResId = coeEnv->AddResourceFileL(resourceCommonName);
    
    TFileName resourceAvkonName(KAvkonResourceFile);
    BaflUtils::NearestLanguageFile(coeEnv->FsSession(), resourceAvkonName);
    iAvkonResId = coeEnv->AddResourceFileL(resourceAvkonName);
    }

// ---------------------------------------------------------------------------
// CSplitItutDataMgr::ReadLafInfo
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CSplitItutDataMgr::ReadLafInfo()
    {
    // Screen
    TRect rect;
	rect = AknLayoutScalable_Avkon::application_window(0).LayoutLine().Rect();
    iScreenSize = rect.Size();

    // finger Layout
    TAknWindowLineLayout splitwnd, splitpane;
    TAknLayoutRect spliwndtrect, splitpanerect;

    // Get Split view rect
    splitwnd = AknLayoutScalable_Avkon::popup_fep_ituss_window(0).LayoutLine();    
    spliwndtrect.LayoutRect(rect, splitwnd);

    splitpane = AknLayoutScalable_Avkon::ituss_keypad_pane(0).LayoutLine();
    splitpanerect.LayoutRect(spliwndtrect.Rect(), splitpane);   
    
    iLayoutRect = spliwndtrect.Rect();
    // spell layout rect
    iLayoutRectSpell = rect;
    iLayoutOffset = spliwndtrect.Rect().iTl;
    iBackgroundRect = iLayoutRect;
    iBackgroundRect.Move(-iLayoutOffset);
    

    // Keypad
    TAknWindowLineLayout itucell, ituinnercell;
    TAknLayoutRect itucellrect, ituinnercellrect;
   
    iKeypadCellRects.Reset();
//    iKeypadCellRectsCn.Reset();

    itucell = AknLayoutScalable_Avkon::cell_ituss_key_pane(0).LayoutLine();           
    itucellrect.LayoutRect(splitpanerect.Rect(), itucell);
    ituinnercell = AknLayoutScalable_Avkon::bg_cell_ituss_key_g1(1).LayoutLine();
    
    iKeypadRect = splitpanerect.Rect();
    iKeypadRect.SetHeight(itucellrect.Rect().Height() * 4);
    iKeypadRect.Move(-iLayoutOffset); 
    
    TRect cellrect = itucellrect.Rect();
    cellrect.Move(-iLayoutOffset);
    cellrect.Move(-iKeypadRect.iTl);
       
    // keypad is 4 rows, 3 cols
    for (TInt i = 0; i < 4; i++)
        {
        for (TInt j = 0; j < 3; j++)
            {
            TRect keyrect = cellrect;
            keyrect.Move(itucellrect.Rect().Width() * j, itucellrect.Rect().Height() * i);

            ituinnercellrect.LayoutRect(keyrect, ituinnercell);
            iKeypadCellRects.Append(ituinnercellrect.Rect());
            
            // read shift icon rect
            if( i == 3 &&  j == 2 )
                {
                TAknWindowLineLayout shiftIcon =  AknLayoutScalable_Avkon::
                                cell_ituss_key_pane_g2( 0 ).LayoutLine();
                TAknLayoutRect shiftIconRect;
                shiftIconRect.LayoutRect( keyrect, shiftIcon );                
                iShiftIconRect = shiftIconRect.Rect();          
                }
            
            // read star icon rect
            if( i == 3 &&  j == 2 )
                {
                TAknWindowLineLayout starIcon =  AknLayoutScalable_Avkon::
                                cell_ituss_key_pane_g2( 0 ).LayoutLine();
                TAknLayoutRect starIconRect;
                starIconRect.LayoutRect( keyrect, starIcon );                
                iStarIconRect = starIconRect.Rect();          
                }            
            
            }
        } 
     

    iVkBigNumTextForPrtWest = AknLayoutScalable_Avkon::cell_ituss_key_t1(4).LayoutLine(); 
    
    iVkNumText = AknLayoutScalable_Avkon::cell_ituss_key_t1(1).LayoutLine(); 
    // Key text row 1                               
    iVkAlphaText1 = AknLayoutScalable_Avkon::cell_ituss_key_t2(1).LayoutLine();
    iVkAlphaText3 = AknLayoutScalable_Avkon::cell_ituss_key_t4(1).LayoutLine();                                                              
    // Key text row 2                               
    iVkAlphaText2 = AknLayoutScalable_Avkon::cell_ituss_key_t3(1).LayoutLine();
    
	// Plus for Numberic Only keypad
	iVkTextPlusForNumbericOnly = 
			AknLayoutScalable_Avkon::cell_ituss_key_t5(0).LayoutLine();
	
    // close button
    TAknWindowLineLayout funcbtn, funcbtnbg, funcbtninner;
    TAknLayoutRect funcrect, funcbgrect, funcinnerrect, arrowleftinnerrect, arrowrightinnerrect;
    TAknLayoutRect optioninnerrect, backspaceinnerrect;
    
    funcbtn = AknLayoutScalable_Avkon::cell_ituss_key_pane(1).LayoutLine();
    funcrect.LayoutRect(splitpanerect.Rect(), funcbtn);
    
    funcbtnbg = AknLayoutScalable_Avkon::bg_cell_ituss_key_g1(1).LayoutLine();
    funcbgrect.LayoutRect(funcrect.Rect(), funcbtnbg);
    
    funcbtninner = AknLayoutScalable_Avkon::cell_ituss_key_pane_g1(0).LayoutLine();
    funcinnerrect.LayoutRect(funcrect.Rect(), funcbtninner);
    
    TInt btnWidth = funcrect.Rect().Width();
    // close button
    iCloseRect = funcbgrect.Rect();
    iCloseRect.Move(0, funcrect.Rect().Height() * 4);
    iCloseRect.Move(-iLayoutOffset);

    iCloseInnerRect = funcinnerrect.Rect();
    iCloseInnerRect.Move(0, iKeypadRect.iBr.iY );
    iCloseInnerRect.Move(-iLayoutOffset);
    
    // arrow left button
    iArrowLeftRect = iCloseRect;     
    iArrowLeftRect.Move(btnWidth, 0);
    arrowleftinnerrect.LayoutRect(iArrowLeftRect, funcbtninner);
    iArrowLeftInnerRect = arrowleftinnerrect.Rect();
    
    // option button
    iOptionsRect = iArrowLeftRect;
    iOptionsRect.Move(btnWidth, 0);    
    optioninnerrect.LayoutRect(iOptionsRect, funcbtninner);
    iOptionInnerRect = optioninnerrect.Rect();
    
    // arrow right button
    iArrowRightRect = iOptionsRect;
    iArrowRightRect.Move(btnWidth, 0);    
    arrowrightinnerrect.LayoutRect(iArrowRightRect, funcbtninner);
    iArrowRightInnerRect = arrowrightinnerrect.Rect();
    
    // backspace button
    iBackspaceRect = iArrowRightRect;
    iBackspaceRect.Move(btnWidth, 0);  
    backspaceinnerrect.LayoutRect(iBackspaceRect, funcbtninner);
    iBackspaceInnerRect = backspaceinnerrect.Rect();


    // preview popup window 
    TAknWindowLineLayout previewWnd, previewBackground, previewWndInner;
    TAknLayoutRect previewWndRect, previewBackgroundRect, previewWndInnerRect;
    TAknLayoutText previewWndText;
    previewWnd = AknLayoutScalable_Avkon::popup_fshwr2_char_preview_window(0).LayoutLine(); 
                
    previewWndRect.LayoutRect( iLayoutRect, previewWnd );
    previewBackground = AknLayoutScalable_Avkon::bg_popup_fep_char_preview_window_cp01().LayoutLine();
    previewBackgroundRect.LayoutRect( previewWndRect.Rect(), previewBackground );
    iPreviewWndRect = previewBackgroundRect.Rect();
    
    previewWndInner = AknLayoutScalable_Avkon::bg_popup_fep_char_preview_window_g9().LayoutLine();
    previewWndInnerRect.LayoutRect( previewBackgroundRect.Rect(), previewWndInner );
    
    iPreviewWndInnerRect = previewWndInnerRect.Rect();
    iPreviewWndText = 
        AknLayoutScalable_Avkon::popup_fshwr2_char_preview_window_t1(0).LayoutLine();
    previewWndText.LayoutText(previewWndRect.Rect(), iPreviewWndText);
    iBubbleFont = const_cast<CFont*>(previewWndText.Font());
    
    // for spell window
    TAknWindowLineLayout spellwnd;
    TAknLayoutRect spellwndtrect;

    spellwnd = AknLayoutScalable_Avkon::popup_fep_ituss_window(1).LayoutLine();
    spellwndtrect.LayoutRect(rect, spellwnd);    
    
    // icf in spell mode
    TAknWindowLineLayout icfpane, focuspane, 
		bottomInnerBtn, middleInnerBtn, middleBtn;
    TAknLayoutRect icfpaneRect, focuspaneRect, 
		bottomButton, bottomInnerRect, middleBtnRect, okBtnRect, cancelBtnRect;
    
    icfpane = AknLayoutScalable_Avkon::popup_fep_vtchi_icf_pane(1).LayoutLine();
    icfpaneRect.LayoutRect(rect, icfpane);
    
    focuspane = AknLayoutScalable_Avkon::vtchi_query_pane(0).LayoutLine();
    focuspaneRect.LayoutRect(icfpaneRect.Rect(), focuspane);
    iSpellICFRect = focuspaneRect.Rect();
    
    // Middle pane
	TRect middleButtonPaneRect = spellwndtrect.Rect();
	TInt leftMargin = itucellrect.Rect().iTl.iX;
	middleButtonPaneRect.Move( leftMargin, 0 );
	
	middleBtn = AknLayoutScalable_Avkon::cell_ituss_key_pane(3).LayoutLine();
	middleBtnRect.LayoutRect( splitpanerect.Rect(), middleBtn );
	middleInnerBtn = AknLayoutScalable_Avkon::bg_cell_ituss_key_g1(5).LayoutLine();
	
	TRect cellMiddleRect;
	cellMiddleRect.iTl = middleButtonPaneRect.iTl;
	cellMiddleRect.SetHeight( middleBtnRect.Rect().Height());
	cellMiddleRect.SetWidth( middleBtnRect.Rect().Width());
	
	// OK button
	okBtnRect.LayoutRect( cellMiddleRect, middleInnerBtn );
    iOkRect = okBtnRect.Rect();
    
    // Cancel button
	cellMiddleRect.Move( middleBtnRect.Rect().Width(), 0 );
	
	cancelBtnRect.LayoutRect( cellMiddleRect, middleInnerBtn );
	iCancelRect = cancelBtnRect.Rect();
	
    iSpellBtnTextFormat = AknLayoutScalable_Avkon::ituss_sks_pane_t1().LayoutLine();
    iSpellBtnTextFormat.ir = 7;
    
    // Bottom pane
	TRect cellSpellRect;
	cellSpellRect.iTl.iX = middleButtonPaneRect.iTl.iX;
	cellSpellRect.iTl.iY = middleButtonPaneRect.iTl.iY + 
			itucellrect.Rect().Height() * 5 + 7;
	cellSpellRect.SetHeight( itucellrect.Rect().Height());
	cellSpellRect.SetWidth( itucellrect.Rect().Width());

	// Left
	bottomButton.LayoutRect( cellSpellRect, ituinnercell );
	iSpellArrowLeftRect = bottomButton.Rect();
	
	bottomInnerBtn = AknLayoutScalable_Avkon::cell_ituss_key_pane_g1(2).LayoutLine();
	bottomInnerRect.LayoutRect( bottomButton.Rect(), bottomInnerBtn );
	iSpellArrowLeftRectInner = bottomInnerRect.Rect();
	
	// Right
	cellSpellRect.Move( itucellrect.Rect().Width(), 0 );
	bottomButton.LayoutRect( cellSpellRect, ituinnercell );
	iSpellArrowRightRect = bottomButton.Rect();
	
	bottomInnerRect.LayoutRect( bottomButton.Rect(), bottomInnerBtn );
	iSpellArrowRightRectInner = bottomInnerRect.Rect();

    // backspace in spell mode	
	cellSpellRect.Move( itucellrect.Rect().Width(), 0 );
	bottomButton.LayoutRect( cellSpellRect, ituinnercell );
	iSpellBackSpcace = bottomButton.Rect();
	
	bottomInnerRect.LayoutRect( bottomButton.Rect(), bottomInnerBtn );
	iSpellBackSpcaceInner = bottomInnerRect.Rect();
    
	// icf indicator
	TAknWindowLineLayout icfIndiPaneWithoutText;
	TAknLayoutRect icfIndiPaneRectWithoutText;
	TAknTextLineLayout indiTextLayout;
	
	icfIndiPaneWithoutText = AknLayoutScalable_Avkon::icf_edit_indi_pane(0).LayoutLine();
	icfIndiPaneRectWithoutText.LayoutRect( focuspaneRect.Rect(), icfIndiPaneWithoutText );
	iIndiPaneRectWithoutTextForPrtWest = icfIndiPaneRectWithoutText.Rect();
	
	TAknWindowLineLayout spellIndiIcon;
	TAknLayoutRect spellIndiIconRect;
	spellIndiIcon = AknLayoutScalable_Avkon::icf_edit_indi_pane_g2(0).LayoutLine();
	spellIndiIconRect.LayoutRect( icfIndiPaneRectWithoutText.Rect(), spellIndiIcon );
	iIndiIconRectWithoutTextForPrtWest = spellIndiIconRect.Rect();
	
	indiTextLayout = AknLayoutScalable_Avkon::icf_edit_indi_pane_t1(0).LayoutLine();
	iIndiTextForPrtWest = indiTextLayout;  
    
	// ICF text line info
    TAknTextLineLayout icftextT1, icftextT2, icftextT3;    
    icftextT1 = AknLayoutScalable_Avkon::vtchi_query_pane_t1(0).LayoutLine();
    icftextT2 = AknLayoutScalable_Avkon::vtchi_query_pane_t2(0).LayoutLine();
    icftextT3 = AknLayoutScalable_Avkon::vtchi_query_pane_t3(0).LayoutLine();
    
    iIcfTextAlignment = icftextT1.iJ;
    iIcfTextLeftMargin = icftextT1.il;
    iIcfTextRightMargin = icftextT1.ir;

    TAknLayoutText ctxt1, ctxt2, ctxt3;
    ctxt1.LayoutText( focuspaneRect.Rect(), icftextT1 );
    ctxt2.LayoutText( focuspaneRect.Rect(), icftextT2 );
    ctxt3.LayoutText( focuspaneRect.Rect(), icftextT3 );

    iIcfTextTopMargin = ctxt1.TextRect().iTl.iY - focuspaneRect.Rect().iTl.iY;
    iIcfTextLineSpaceMargin = ctxt2.TextRect().iTl.iY - ctxt1.TextRect().iBr.iY;
    iIcfTextBottomMargin = focuspaneRect.Rect().iBr.iY - ctxt3.TextRect().iBr.iY 
                                                      - iIcfTextLineSpaceMargin;  
    iIcfTextHeight = ctxt1.TextRect().Height();
    
    iIcfFont = const_cast<CFont*>(AknLayoutUtils::FontFromId(icftextT1.iFont, NULL));
    }   

// ---------------------------------------------------------------------------
// CSplitItutDataMgr::ReadLafInfo
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CSplitItutDataMgr::AddSubscriber(MItutPropertySubscriber* aSubscriber)
    {
    if (aSubscriber)
        {
        const TInt index = iSubscriberList.Find(aSubscriber);

        if (index == KErrNotFound)
            {
            iSubscriberList.Append(aSubscriber);
            }
        }
    }
    
// ---------------------------------------------------------------------------
// CSplitItutDataMgr::ReadLafInfo
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CSplitItutDataMgr::SetLanguageL(TInt aLanguage)
    {    
    ASSERT( IsValidLanguage( aLanguage ) );
    	
    if (iLanguage != aLanguage)
        {	        
        if( IsValidLanguage( iLanguage ) )
        	iIsLangDirectionSwitch =  IsRtoLLanguage( aLanguage ) ^ IsRtoLLanguage( iLanguage );
        else        
        	//Check mirroring is needed when first setting language
        	iIsLangDirectionSwitch = IsRtoLLanguage( aLanguage );        	
        
        iLanguage = aLanguage;
        iInputMode = KInvalidImMode;
        
        //SetTextAlignment();

        TInt scriptIdx = GetConfigResFileName(aLanguage);

        if (scriptIdx != iCurrentScriptIdx)
            {
            iCurrentScriptIdx = scriptIdx;

            CCoeEnv* coeEnv = CCoeEnv::Static();
            
            if (iConfigResId)
                {
                coeEnv->DeleteResourceFile(iConfigResId);
                }

            TFileName resourceConfigName(iResourceFilename);
            BaflUtils::NearestLanguageFile(coeEnv->FsSession(), resourceConfigName);
            iConfigResId = coeEnv->AddResourceFileL(resourceConfigName);
            
            if (IsChinese())
                {
                iLayoutContext->UiManager()->CreateChineseSpecificCtrlsIfNeededL();
                }

            NotifyChangeControlLayout(MItutPropertySubscriber::EItutPropertyKeypadResourceId, 
                                      KeypadResourceId());  
            }
        }
    }
    
// ---------------------------------------------------------------------------
// CSplitItutDataMgr::ReadLafInfo
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CSplitItutDataMgr::SetCase(TInt aCase)
    {
    if (iCase != aCase || iInputMode == ELatin)
        {
        iCase = aCase;
        NotifyChangeControlLayout(MItutPropertySubscriber::EItutPropertyKeypadResourceId, 
                                  KeypadResourceId());  
        }
    }

// ---------------------------------------------------------------------------
// CSplitItutDataMgr::ReadLafInfo
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CSplitItutDataMgr::SetInputModeL(TInt aMode)
    {
    TInt engineMode = KInvalidImMode;

    switch (aMode)
        {
        case EPinyin:
            {
            engineMode = EPtiEnginePinyinByPhrase;
            }
            break;
        case EStroke:
            {
            engineMode = EPtiEngineStrokeByPhrase;
            }
            break;
        case EZhuyin:
            {
            engineMode = EPtiEngineZhuyinByPhrase;
            }
            break;
        case ELatin:
        case EHindi:
        case ENumber:
        case ENativeNumber:
            {
            iInputMode = aMode;
            NotifyChangeControlLayout(MItutPropertySubscriber::EItutPropertyKeypadResourceId, 
                                      KeypadResourceId());  
            }
            break;
        case EZhuyinFind:
            {
            iInputMode = EZhuyin; 
            
            return; 
            }
        case EStrokeFind:
            {
            iInputMode = EStroke;  
            
            return;
            }
        default:
            return;    
        }

    if (IsChinese())
        {
        TInt err = iPtiEngine->ActivateLanguageL(iLanguage, TPtiEngineInputMode(engineMode));
        if (err == KErrNone)
            {
            iPtiEngine->SetCandidatePageLength(KOnePageCandidateCount);
            iInputMode = aMode;
            }
        }
    }


// ---------------------------------------------------------------------------
// CSplitItutDataMgr::RequestData
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TAny* CSplitItutDataMgr::RequestData(TInt aDataType)
    {
    switch ( aDataType )
        {
        case EScreenSize:
            return &iScreenSize;
        case ELayoutOffset:
            return &iLayoutOffset;
        case ELayoutRect:
            return iSpellMode ? &iLayoutRectSpell : &iLayoutRect;
        case EBackgroundRect:
            return iSpellMode ? &iLayoutRectSpell : &iBackgroundRect;
        case EBackspaceRect:
            return IsChinese() ? &iBackspaceCnRect : &iBackspaceRect;
        case EKeypadRect:
            return &iKeypadRect;
       // 	return IsChinese() ? &iKeypadRectCn : &iKeypadRect;	        	
        case EKeypadCellRects:
            return &iKeypadCellRects;
       // 	return IsChinese() ? &iKeypadCellRectsCn : &iKeypadCellRects;	
        case EKeypadLeftTextLine:
        	if ( iInputMode == ENumber || iInputMode == ENativeNumber )
				{
				return &iVkBigNumTextForPrtWest;
				}
        	else
        		{
				return &iVkNumText;
        		}
        case EKeypadRightTextLine1:
			if ( iInputMode == ENumber || iInputMode == ENativeNumber )
				{
				return &iVkTextPlusForNumbericOnly;
				}
			else
				{
				return &iVkAlphaText1;
				}
        case EKeypadRightTextLine2:
            return &iVkAlphaText2;
        case EKeypadRightTextLine3:
            return &iVkAlphaText3;
        /*
        case EDropdownlistUnitWidth:
            return &iCandsUnitWidth;
        case EDropdownlistUnitHeight:
            return &iCandsUnitHeight;
        case EDropdownlistHorizontalMargin:
            return &iCandsHorizontalMargin;
        case EDropdownlistVerticalMargin:
            return &iCandsVerticalMargin;
        case EDropdownListNaviWidth:
            return &iCandsNaviWidth;
        case EDropdownListSpinBtnHeight:
            return &iCandsSpinBtnHeight;
        case EDropdownListSpellLTPos:
            return &iCandsSpellLTPos;
        case EDropdownListCandsLTPos:
            return &iCandsLTPos;
        case EDropdownListPuncLTPos:
            return &iCandsPuncLTPos;
        case EDropdownListFont:
            return reinterpret_cast<TAny*>(iCandsFont);
        case EDropdownListTextMargin:
            return &iCandsTextMargin;
        case EDropdownListTextColor:
            return &iCandsTextColor;
        */    

        case ELanguage:
            return &iLanguage;
        case EInputMode:
            return &iInputMode;
        case ECase:
            return &iCase;
        /*    
        case EChnCandidates:
            return &iCandidates;
        case EChnPuncCandidates:
            return &iPuncCandidates;
        */
        case EArrowLeftRect:
        	if( iSpellMode )
        		{
        	    return &iSpellArrowLeftRect;
        		}
        	else
        		{
                return IsChinese() ? &iArrowLeftCnRect : &iArrowLeftRect;
        		}
        case EArrowRightRect:
        	if( iSpellMode )
           		{
        	    return &iSpellArrowRightRect;
           		}
        	else
        		{
                return IsChinese() ? &iArrowRightCnRect : &iArrowRightRect;
        		}
        case EArrowUpRect:
//        case EItutPosArrowUp:
            return &iArrowUpRect;
        case EArrowDownRect:
//        case EItutPosArrowDown:
            return &iArrowDownRect;
        case EOptionsRect:
#ifdef LAF_MODIFY	
        	return IsChinese() ? &iOptionsRectCn : &iOptionsRect;
#else        	
            return &iOptionsRect;
#endif            
        case ECloseRect:
#ifdef LAF_MODIFY	
        	return IsChinese() ? &iCloseRectCn : &iCloseRect;
#else        	
            return &iCloseRect;
#endif            
        case ELeftInnerRect:
        	if( iSpellMode )
           		{
        	    return &iSpellArrowLeftRectInner;
           		}
        	else
        		{
        	    return IsChinese() ? &iArrowLeftInnerCnRect : &iArrowLeftInnerRect;
        		}
        case ERightInnerRect:
        	if( iSpellMode )
        		{
        	    return &iSpellArrowRightRectInner;
        	    }
        	else
        		{
        	    return IsChinese() ? &iArrowRightInnerCnRect : &iArrowRightInnerRect;
        		}
        case EBackspaceInnerRect:
        	return IsChinese() ? &iBackspaceInnerCnRect : &iBackspaceInnerRect;
       	case EUpInnerRect:
        	return &iArrowUpInnerRect;
        case EDownInnerRect:
        	return &iArrowDownInnerRect;
       	case ECloseInnerRect:
#ifdef LAF_MODIFY	       	
        	return IsChinese() ? &iCloseInnerRectCn : &iCloseInnerRect;
#else        	
        	return &iCloseInnerRect;
#endif        	
        case EOptionInnerRect:
#ifdef LAF_MODIFY	
        	return IsChinese() ? &iOptionInnerRectCn : &iOptionInnerRect;
#else        	
        	return &iOptionInnerRect;
#endif        
        case ELatinOnly:
            return &iLatinOnly;
        case EPreviewBubbleRect:
            return &iPreviewWndRect;
        case EPreviewBubbleInnerRect:
            return &iPreviewWndInnerRect;
        case EPreviewBubbleTextlayout:
            return &iPreviewWndText;
        case EPreviewBubbleFont:
            return reinterpret_cast<TAny*>(iBubbleFont);    
        case EItutPosOk:
            return &iOkRect;
        case EItutPosCancel:
            return &iCancelRect;
        case EBtnTextLine:
            return &iSpellBtnTextFormat;            
        case ESpellICFRect:
            return &iSpellICFRect;
        case ESpellEditIndicatorRect:
            return &iSpellEditIndicator;
        case EIcfFont:
            return reinterpret_cast<TAny*>(iIcfFont);      
        case EImIndicatorRect:
        	return &iSpellEditIndicator;
        case ESpellBackSpcae:
        	return &iSpellBackSpcace;
        case ESpellBackSpcaeInner:
        	return &iSpellBackSpcaceInner;
        case EIndiPaneWithoutTextRect:
            return &iIndiPaneRectWithoutTextForPrtWest;
        case EIndiIconWithoutTextRect:
            return &iIndiIconRectWithoutTextForPrtWest;
        case EIndiTextLine:
            return &iIndiTextForPrtWest;
        default:
            break;
        }

    return NULL;
    }

// ---------------------------------------------------------------------------
// CSplitItutDataMgr::CSplitItutDataMgr
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
CSplitItutDataMgr::CSplitItutDataMgr(MSplitItutLayoutContext* aLayoutContext, 
                                         CPtiEngine* aPtiEngine)
                                        :
                                         iPtiEngine( aPtiEngine ),
                                         iLanguage(ELangNone),
                                         iInputMode(KInvalidImMode),
                                         iCase(EAknEditorUpperCase),
                                         iConfigResId(0),
                                         iLayoutContext(aLayoutContext),
                                         iCurrentScriptIdx(KInvalidIndex),
                                         iSpellMode(EFalse)
    {
    }



// ---------------------------------------------------------------------------
// CSplitItutDataMgr::NotifySubscriber
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CSplitItutDataMgr::NotifySubscriber(MItutPropertySubscriber::TItutProperty aPropertyName, 
                                           const TDesC& aValue)
    {
    for (TInt i = 0; i < iSubscriberList.Count(); i++)
        {
        TRAP_IGNORE(iSubscriberList[i]->SetPropertyL(aPropertyName, aValue));
        }
    }

// ---------------------------------------------------------------------------
// CSplitItutDataMgr::NotifyChangeControlLayout
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CSplitItutDataMgr::NotifyChangeControlLayout(
    MItutPropertySubscriber::TItutProperty aPropertyName, TInt aCtrlResId)
    {
    TInt ctrlResId = KInvalidResId;

    switch (aPropertyName)
        {
        case MItutPropertySubscriber::EItutPropertyKeypadResourceId:
            {
            ctrlResId = iLayoutContext->Control(ECtrlIdStdItut)->ResourceId();
            }
            break;
        case MItutPropertySubscriber::EItutPropertyCandidateListResourceId:
            {
            ctrlResId = iLayoutContext->Control(ECtrlIdStdCandsList)->ResourceId();
            }
            break;
        default:
            break;
        }

    // since keypad resource divided by script, so maybe ctrlResId unchanged,
    // but it is actually changed in different script
    if ((ctrlResId != aCtrlResId) ||
        (aPropertyName == MItutPropertySubscriber::EItutPropertyKeypadResourceId))
        {
        TPtrC ptr(reinterpret_cast<TText*>(&aCtrlResId), sizeof(aCtrlResId)/sizeof(TText));

        NotifySubscriber(aPropertyName, ptr);
        }
    }

/*
TInt CSplitItutDataMgr::GetNextPageCandidateL(RPointerArray<HBufC>& aList)
    {
    if (iPtiEngine && (iPtiEngine->NextCandidatePage()))
        {
        TItutDataConverter::ConvertChnPhraseCandidateL(iPtiEngine->CandidatePage(), aList);

        return KErrNone;
        }

    return KErrNotSupported;
    }

TBool CSplitItutDataMgr::NextPageCandidateExist()
    {
    if (iPtiEngine)
        {
        return iPtiEngine->MoreCandidatePages();
        }

    return EFalse;    
    }
*/


// ---------------------------------------------------------------------------
// CSplitItutDataMgr::KeypadResourceId
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TInt CSplitItutDataMgr::KeypadResourceId()
    {
    if (iInputMode == ENumber )
        {
        if( IsExistPlusChar()  )
            return R_FINGER_INPUT_KEYPAD_NUMBER_WITH_PLUS;
        else
        	return R_FINGER_INPUT_KEYPAD_NUMBER;
        }
    if( iInputMode == ENativeNumber )
        {
        if( IsExistPlusChar() )
        	return R_FINGER_INPUT_KEYPAD_NATIVE_NUMBER_WITH_PLUS;
        else
        	return R_FINGER_INPUT_KEYPAD_NATIVE_NUMBER;
        }

    if ( iLatinOnly )
    	{
    	if ( iCase == EAknEditorTextCase || iCase == EAknEditorUpperCase )
    		{
    		return R_FINGER_INPUT_KEYPAD_LATINONLY_UPPERCASE;
    		}
        return R_FINGER_INPUT_KEYPAD_LATINONLY_LOWERCASE;
    	}

    if (!iLangCaseSensitive )
        {
        return R_FINGER_INPUT_KEYPAD_ABC;
        }
    if ( IsChinese() )
        {
        return ChineseKeypadResourceId();
        }
    if ( IsHindi() )
        {
        return HindiKeypadResourceId();
        }
        
    if (iCase == EAknEditorTextCase || iCase == EAknEditorUpperCase)
        {
        return R_FINGER_INPUT_KEYPAD_ABC_UPPERCASE;
        }
    else
        {
        return R_FINGER_INPUT_KEYPAD_ABC_LOWERCASE;
        }
    }
    
    
// ---------------------------------------------------------------------------
// CSplitItutDataMgr::ChineseKeypadResourceId
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TInt CSplitItutDataMgr::ChineseKeypadResourceId()
    {
    
    if (iInputMode == EZhuyin)
        {
        return R_FINGER_INPUT_KEYPAD_ZHUYIN_STANDBY_PREDICT;
        }
    if (iInputMode == EPinyin)
        {
        return R_FINGER_INPUT_KEYPAD_PINYIN_STANDBY_PREDICT;
        }
    if (iInputMode == EStroke)
        {
        TInt resID;
        if (InputLanguage() == ELangPrcChinese)
            {
            resID = R_FINGER_INPUT_KEYPAD_STROKE_STANDBY_PREDICT_PRC;
            }
        else
            {
            resID = R_FINGER_INPUT_KEYPAD_STROKE_STANDBY_PREDICT_TRA;
            }
        return resID;
        }
        
    if (iCase == EAknEditorTextCase || iCase == EAknEditorUpperCase)
        {
        return R_FINGER_INPUT_KEYPAD_ABC_UPPERCASE_CHN;
        }
    else
        {
        return R_FINGER_INPUT_KEYPAD_ABC_LOWERCASE_CHN;
        }
    }


// ---------------------------------------------------------------------------
// CSplitItutDataMgr::HindiKeypadResourceId
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TInt CSplitItutDataMgr::HindiKeypadResourceId()
    {
    if ( iInputMode == EHindi )
        {
        return R_FINGER_INPUT_KEYPAD_ABC;
        }
        
    if (iCase == EAknEditorTextCase || iCase == EAknEditorUpperCase)
        {
        return R_FINGER_INPUT_KEYPAD_LATINONLY_UPPERCASE;
        }
    else
        {
        return R_FINGER_INPUT_KEYPAD_LATINONLY_LOWERCASE;
        }
    }

// ---------------------------------------------------------------------------
// CSplitItutDataMgr::GetConfigResFileName
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TInt CSplitItutDataMgr::GetConfigResFileName(TInt aLang)
    {
    iResourceFilename.Zero();
    iResourceFilename = KConfigResourceFile();

    for (TInt i = 0; i < sizeof(TScriptMapping) / sizeof(TScriptMapping[0]); i++)
        {
        TScriptInfo script = TScriptMapping[i];

        if (FindLang(&(LangsArr[script.iStartIdx]), script.iEndIdx - script.iStartIdx, aLang) != 
            KErrNotFound)
            {
            iResourceFilename.Append(ResFileExtByScriptIdx(script.iScriptExt));
            iResourceFilename.Append(KResourceFileExtName);
            iLangCaseSensitive = script.iCaseSensitive;
            return i;
            }
        }

    // default resource file is latin
    iResourceFilename.Append(ResFileExtByScriptIdx(TScriptMapping[0].iScriptExt));
    iResourceFilename.Append(KResourceFileExtName);    
    iLangCaseSensitive = TScriptMapping[0].iCaseSensitive;
    return 0;
    }

// ---------------------------------------------------------------------------
// CSplitItutDataMgr::FindLang
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TInt CSplitItutDataMgr::FindLang(const TInt* aArray, 
                                   TInt aEndIdx,
                                   TInt aLang) 
    {
    for (TInt i = 0; i <= aEndIdx; i++)
        {
        if (aLang == *aArray)
            {
            return i;
            }

        aArray++;    
        }

    return KErrNotFound;
    }


// ---------------------------------------------------------------------------
// CSplitItutDataMgr::FindLang
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TBool CSplitItutDataMgr::IsChinese()
    {
    return (iLanguage == ELangPrcChinese || iLanguage == ELangHongKongChinese 
         || iLanguage == ELangTaiwanChinese);
    }

// ---------------------------------------------------------------------------
// CSplitItutDataMgr::IsHindi
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TBool CSplitItutDataMgr::IsHindi()
    {
    return ( iLanguage == ELangHindi );
    }

// ---------------------------------------------------------------------------
// CSplitItutDataMgr::IsThai
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TBool CSplitItutDataMgr::IsThai()
    {
    return ( iLanguage == ELangThai );
    }

// ---------------------------------------------------------------------------
// CSplitItutDataMgr::IsRtoLLanguage
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TBool CSplitItutDataMgr::IsRtoLLanguage()
	{
	return ( iLanguage == ELangArabic ||
			 iLanguage == ELangHebrew ||
			 iLanguage == ELangFarsi  ||
			 iLanguage == ELangUrdu );	
	}

// ---------------------------------------------------------------------------
// CSplitItutDataMgr::IsRtoLLanguage
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TBool CSplitItutDataMgr::IsRtoLLanguage( TInt aLanguage )
	{
	return ( aLanguage == ELangArabic ||
			 aLanguage == ELangHebrew ||
			 aLanguage == ELangFarsi  ||
			 aLanguage == ELangUrdu );	
	}

// ---------------------------------------------------------------------------
// CSplitItutDataMgr::IsLangDirectionSwitch
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TBool CSplitItutDataMgr::IsLangDirectionSwitch() const
	{
	return iIsLangDirectionSwitch;	
	}

// ---------------------------------------------------------------------------
// CSplitItutDataMgr::ResFileExtByScriptIdx
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
const TDesC& CSplitItutDataMgr::ResFileExtByScriptIdx(const TScriptExtIndex aIndex)
    {
    switch (aIndex)
        {
        case ELatinExt:
            return KLatinExt;
        case ECyrillicExt:
            return KCyrillicExt;
        case EGreekExt:
            return KGreekExt;
        case EHebrewExt:
            return KHebrewExt;
        case EArabicExt:
            return KArabicExt;
        case EFarsiExt:
            return KFarsiExt;
        case EUrduExt:
            return KUrduExt;
        case EThaiExt:
            return KThaiExt;
        case EChnExt:
            return KChnExt;
        case EDevanagiriExt:
        	return KDevanagiriExt;
        default:
            break;    
        }

    return KNullDesC;
    }

// ---------------------------------------------------------------------------
// CSplitItutDataMgr::SetUpdate
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CSplitItutDataMgr::SetUpdate(TBool aNeedUpdate)
	{
	iNeedUpdate = 	aNeedUpdate;
	}

// ---------------------------------------------------------------------------
// CSplitItutDataMgr::IsUpdate
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TBool CSplitItutDataMgr::IsUpdate() const
	{
	return 	iNeedUpdate;
	}

// ---------------------------------------------------------------------------
// CSplitItutDataMgr::SetNumericKeymap
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CSplitItutDataMgr::SetNumericKeymap( const TInt aKeymap )
	{
	iNumericKeymap = aKeymap;
	}

// ---------------------------------------------------------------------------
// CSplitItutDataMgr::SetNumericKeymapData
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CSplitItutDataMgr::SetNumericKeymapData( TInt* aKeymapData )
	{
	delete iNumericKeymapData;
    TInt* len = (TInt*)((TUint8*)aKeymapData - 4);
    TPtrC16 keymapRes((const TUint16*)aKeymapData, *len/2);  
    iNumericKeymapData = keymapRes.Alloc();
	}

// ---------------------------------------------------------------------------
// CSplitItutDataMgr::IsExistPlusChar
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TBool CSplitItutDataMgr::IsExistPlusChar()
    {
    if( !IsNumericOnly() )
    	{
    	return EFalse;
    	}
    if ( iNumericKeymap == EKeymapFromResource )
    	{
    	HBufC* keyRes = iNumericKeymapData;
    	if( keyRes != NULL && keyRes->Locate( TChar( KPlusChar ) ) != KErrNotFound )
    		{
    		return ETrue;
    		}
    	}
    else if ( iNumericKeymap == EAknEditorStandardNumberModeKeymap 
    		|| iNumericKeymap == EAknEditorConverterNumberModeKeymap
    		|| iNumericKeymap == EAknEditorCalculatorNumberModeKeymap 
    		|| iNumericKeymap == EAknEditorToFieldNumberModeKeymap 
    		|| iNumericKeymap == EAknEditorFixedDiallingNumberModeKeymap 
    		|| iNumericKeymap == EAknEditorSATNumberModeKeymap )
    	{
    	return ETrue;
    	}
    return EFalse;
    }

// End Of File
