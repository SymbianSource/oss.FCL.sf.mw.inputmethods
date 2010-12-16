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


#include <PtiEngine.h>
#include <coemain.h>
#include <AknUtils.h>
#include <peninputgeneralitutnew.rsg>
#include <peninputitutconfig_chinese.rsg>
#include <peninputitutconfig_latin.rsg>
#include <peninputitutconfig_korean.rsg>
#include <peninputitutconfig_hebrew.rsg>
#include <peninputitutconfig_farsi.rsg>
#include <AknFepGlobalEnums.h>
#include <eikon.hrh>
#include <bautils.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <peninputlayoutmultilineicf.h>
#include <AknSettingCache.h>
#include <AknFepInternalCRKeys.h>
#include <centralrepository.h>
#include <featmgr.h> 

#include "peninputgenericitutdatamgr.h"
#include "peninputgenericitutlayoutcontext.h"
#include "peninputgenericitutconverter.h"
#include "peninputgenericitutcustomization.h"
#include "peninputgenericitutwindowmanager.h"

#define LAF_MODIFY 1
_LIT(KItutCommonResFile, "z:\\resource\\plugins\\peninputgeneralitutnew.rsc");
_LIT(KConfigResourceFile, 
     "z:\\resource\\plugins\\peninputitutconfig_");
_LIT(KResourceFileExtName, ".rsc");

_LIT(KAvkonResourceFile, "z:\\resource\\avkon.rsc");
     
const TInt KOnePageCandidateCount = 40;
const TInt KMaxPredictCandCnt = 250;
const TInt KInvalidResId = -1;
const TInt KInvalidImMode = -1;
const TInt KInvalidIndex = -1;
const TUint16 KPlusChar = '+';

// For virtual ITUT candidate list in landscape mode. No reason
const TInt KCandsAdjustment = 7;


CGenericItutDataMgr* CGenericItutDataMgr::NewL(MItutLayoutContext* aLayoutContext, 
                                               CPtiEngine* aPtiEngine)
    {
    CGenericItutDataMgr* self = new (ELeave) CGenericItutDataMgr(aLayoutContext, aPtiEngine);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);//self
    return self;
    }

void CGenericItutDataMgr::ClearChnCandidates(TInt aCmd)
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

CGenericItutDataMgr::~CGenericItutDataMgr()
    {
    iSubscriberList.Close();  

    iMenuItemList.ResetAndDestroy();
    
    CCoeEnv::Static()->DeleteResourceFile(iAvkonResId);
        
    CCoeEnv::Static()->DeleteResourceFile(iCommonResId);
    
    // Remove the resource file of current input language
    if ( iConfigResId )
        {
        CCoeEnv::Static()->DeleteResourceFile( iConfigResId );
        }
    
    // Remove the resource file of some special controls
    if ( iSpecialResId )
        {
        CCoeEnv::Static()->DeleteResourceFile( iSpecialResId );
        }

    ClearChnCandidates(EItutCandidates);
    ClearChnCandidates(EItutPuncCandidates);

    iKeypadCellRects.Close();
    iKeypadCellRectsCn.Close();
    iKeypadCellRectsQuery.Close(); 
    iKeypadCellRectsForPrtWest.Close();
    delete iNumericKeymapData;
    }

void CGenericItutDataMgr::ConstructL()
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
    
    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC(reader, R_AVKON_TOUCH_INDICATOR_EDIT_MODE_MENU);
        
    const TInt count=reader.ReadInt16();
    for ( TInt ii=0; ii<count; ++ii )
        {
        TMenuItem* item = new (ELeave) TMenuItem();
        CleanupStack::PushL( item );
        item->iCommand = reader.ReadInt32();
        reader.ReadInt32();
        reader.ReadInt32();
        item->iText =  reader.ReadTPtrC();
        reader.ReadTPtrC();
        reader.ReadTPtrC();
        reader.ReadInt16();
        reader.ReadInt16();
        CleanupStack::Pop(); // pop first, since additem pushes again
        iMenuItemList.Append( item );
        reader.ReadInt32(); // extension link
        }        
    CleanupStack::PopAndDestroy(); //reader

    // There are some special controls in Chinese and Korean VITUT,
    // and these controls are hidden when changing to other input language, 
    // so related resource file should be in control environment at all times.
    // Otherwise crash happens when reading resource for them sometime. 
    // For example: If writing language is Chinese, enters spell mode,
    // then switches theme, crash happens. 
    if ( FeatureManager::FeatureSupported( KFeatureIdChinese ) ||
            FeatureManager::FeatureSupported( KFeatureIdKorean ) )
        {
        TFileName  resourceFilename( KConfigResourceFile ) ;
        if ( FeatureManager::FeatureSupported( KFeatureIdChinese ) )
            {
            // Chinese variant
            resourceFilename.Append( ResFileExtByScriptIdx( EChnExt ) );
            }
        else
            {
            // Korean variant
            resourceFilename.Append( ResFileExtByScriptIdx( EHangulExt ) );
            }
        resourceFilename.Append(KResourceFileExtName);
        
        BaflUtils::NearestLanguageFile( coeEnv->FsSession(), resourceFilename );
        iSpecialResId = coeEnv->AddResourceFileL( resourceFilename );        
        }
    }

CGenericItutDataMgr::TMenuItem* CGenericItutDataMgr::GetMenuItem(TInt aCommand)
    {
    for (TInt i=0;i<iMenuItemList.Count();i++)
        {
        if ( iMenuItemList[i]->iCommand == aCommand )
            return iMenuItemList[i];
        }
    return NULL;
    }
    
void CGenericItutDataMgr::AddSubscriber(MItutPropertySubscriber* aSubscriber)
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

void CGenericItutDataMgr::ReadLafInfoForPrtWest()
	{
	// Screen
	TRect rect;

	rect = AknLayoutScalable_Avkon::application_window(0).LayoutLine().Rect();

	TAknWindowLineLayout mainWnd, bottomWnd, bottomPane;
	TAknLayoutRect mainWndRect, bottomwndtrect, bottompanerect;

	mainWnd = AknLayoutScalable_Avkon::main_fep_vtchi_ss_pane(0).LayoutLine();
	mainWndRect.LayoutRect(rect, mainWnd);
	
	bottomWnd = AknLayoutScalable_Avkon::popup_fep_ituss_window(0).LayoutLine();    
	bottomwndtrect.LayoutRect(mainWndRect.Rect(), bottomWnd);

	bottomPane = AknLayoutScalable_Avkon::ituss_keypad_pane(0).LayoutLine();
	bottompanerect.LayoutRect(bottomwndtrect.Rect(), bottomPane);   
	
	iLayoutRectForPrtWest = mainWndRect.Rect();
	iLayoutOffsetForPrtWest = mainWndRect.Rect().iTl;

	// ICF
	TAknWindowLineLayout icflayout, icflayoutbg, icflayoutpane;
	TAknLayoutRect icfRect, icfRectBg, icfRectPane;
	
	icflayoutpane = AknLayoutScalable_Avkon::popup_fep_vtchi_icf_pane(0).LayoutLine();
	icfRectPane.LayoutRect( mainWndRect.Rect(), icflayoutpane );
	
	icflayoutbg = AknLayoutScalable_Avkon::bg_icf_pane_cp01(0).LayoutLine();
	icfRectBg.LayoutRect( icfRectPane.Rect(), icflayoutbg );
	
	icflayout = AknLayoutScalable_Avkon::vtchi_icf_list_pane(0).LayoutLine();
	icfRect.LayoutRect( icfRectBg.Rect(), icflayout );
	
	iIcfRectForPrtWest = icfRect.Rect();
	
	// icf indicator
	TAknWindowLineLayout icfIndiPaneWithText, icfIndiPaneWithoutText, 
		indiIconWithText, indiIconWithoutText;
	TAknLayoutRect icfIndiPaneRectWithText, icfIndiPaneRectWithoutText, 
		indiIconRectWithText, indiIconRectWithoutText;
	TAknTextLineLayout indiTextLayout;
	
	icfIndiPaneWithText = AknLayoutScalable_Avkon::icf_edit_indi_pane(1).LayoutLine();
	icfIndiPaneRectWithText.LayoutRect(icfRect.Rect(), icfIndiPaneWithText);
	iIndiPaneRectWithTextForPrtWest = icfIndiPaneRectWithText.Rect();
	
	icfIndiPaneWithoutText = AknLayoutScalable_Avkon::icf_edit_indi_pane(0).LayoutLine();
	icfIndiPaneRectWithoutText.LayoutRect(icfRect.Rect(), icfIndiPaneWithoutText);
	iIndiPaneRectWithoutTextForPrtWest = icfIndiPaneRectWithoutText.Rect();
	
	indiIconWithText = AknLayoutScalable_Avkon::icf_edit_indi_pane_g2(1).LayoutLine();
	indiIconRectWithText.LayoutRect( icfIndiPaneRectWithText.Rect(), indiIconWithText );
	iIndiIconRectWithTextForPrtWest = indiIconRectWithText.Rect();
	
	indiIconWithoutText = AknLayoutScalable_Avkon::icf_edit_indi_pane_g2(0).LayoutLine();
	indiIconRectWithoutText.LayoutRect( 
			icfIndiPaneRectWithoutText.Rect(), indiIconWithoutText );
	iIndiIconRectWithoutTextForPrtWest = indiIconRectWithoutText.Rect();
	
	indiTextLayout = AknLayoutScalable_Avkon::icf_edit_indi_pane_t1(0).LayoutLine();
	iIndiTextForPrtWest = indiTextLayout;
	
	// icf text
	TAknTextLineLayout icflefttext, icflefttext1; 
	icflefttext = AknLayoutScalable_Avkon::vtchi_icf_list_pane_t1(0, 0, 0).LayoutLine();
	icflefttext1 = AknLayoutScalable_Avkon::vtchi_icf_list_pane_t1(0, 0, 1).LayoutLine();

	iIcfTextAlignmentForPrtWest = icflefttext.iJ;
	iIcfTextLeftMarginForPrtWest = icflefttext.il;
	//iIcfTextRightMarginForPrtWest = icflefttext.ir;
	iIcfTextRightMarginForPrtWest = iIcfTextLeftMarginForPrtWest;
	    
	TAknLayoutText ctxt, ctxt1;
	ctxt.LayoutText( icfRect.Rect(), icflefttext );
	ctxt1.LayoutText( icfRect.Rect(), icflefttext1 );
	    
	iIcfTextTopMarginForPrtWest = ctxt.TextRect().iTl.iY - icfRect.Rect().iTl.iY;
	iIcfTextLineSpaceMarginForPrtWest = ctxt1.TextRect().iTl.iY - ctxt.TextRect().iBr.iY;
	iIcfTextHeightForPrtWest = ctxt.TextRect().Height();
	
	TInt maxRow = ( iIcfRectForPrtWest.Height() - iIcfTextTopMarginForPrtWest ) 
        / ( iIcfTextHeightForPrtWest + iIcfTextLineSpaceMarginForPrtWest );
	
	iIcfTextBottomMarginForPrtWest = 
        iIcfRectForPrtWest.Height() - ( iIcfTextHeightForPrtWest + iIcfTextLineSpaceMarginForPrtWest ) 
        * maxRow - iIcfTextTopMarginForPrtWest;
	    
	iIcfFontForPrtWest = const_cast<CFont*>(AknLayoutUtils::FontFromId(icflefttext.iFont, NULL));	
	
	// Keypad
	TAknWindowLineLayout itucell, ituinnercell;
	TAknLayoutRect itucellrect, ituinnercellrect;
   
	iKeypadCellRectsForPrtWest.Reset();

	itucell = AknLayoutScalable_Avkon::cell_ituss_key_pane(0).LayoutLine();           
	itucellrect.LayoutRect(bottompanerect.Rect(), itucell);
	ituinnercell = AknLayoutScalable_Avkon::bg_cell_ituss_key_g1(1).LayoutLine();
	
	iKeypadRectForPrtWest = bottompanerect.Rect();
	iKeypadRectForPrtWest.SetHeight(itucellrect.Rect().Height() * 4);
	iKeypadRectForPrtWest.Move(-iLayoutOffsetForPrtWest); 
	
	TRect cellrect = itucellrect.Rect();
	cellrect.Move(-iLayoutOffsetForPrtWest);
	cellrect.Move(-iKeypadRectForPrtWest.iTl);
	   
	// keypad is 4 rows, 3 cols
	for (TInt i = 0; i < 4; i++)
		{
		for (TInt j = 0; j < 3; j++)
			{
			TRect keyrect = cellrect;
			keyrect.Move(itucellrect.Rect().Width() * j, itucellrect.Rect().Height() * i);

			ituinnercellrect.LayoutRect(keyrect, ituinnercell);
			iKeypadCellRectsForPrtWest.Append(ituinnercellrect.Rect());
			
			// read shift icon rect
			if( i == 3 &&  j == 2 )
				{
				TAknWindowLineLayout shiftIcon =  AknLayoutScalable_Avkon::
								cell_ituss_key_pane_g2( 0 ).LayoutLine();
				TAknLayoutRect shiftIconRect;
				shiftIconRect.LayoutRect( keyrect, shiftIcon );                
				iShiftIconRectForPrtWest = shiftIconRect.Rect();          
				}    

			// read star icon rect
			if( i == 3 &&  j == 0 )
				{
				TAknWindowLineLayout starIcon =  AknLayoutScalable_Avkon::
								cell_ituss_key_pane_g2( 0 ).LayoutLine();
				TAknLayoutRect starIconRect;
				starIconRect.LayoutRect( keyrect, starIcon );                
				iStarIconRectForPrtWest = starIconRect.Rect();          
				}			
			
			}
		} 
	 

	// Num text row
	iVkBigNumTextForPrtWest = AknLayoutScalable_Avkon::cell_ituss_key_t1(4).LayoutLine(); 
	iVkNumTextForPrtWest = AknLayoutScalable_Avkon::cell_ituss_key_t1(1).LayoutLine();
	
	// Key text row 1                               
	iVkAlphaText1ForPrtWest = AknLayoutScalable_Avkon::cell_ituss_key_t2(1).LayoutLine();
	// Key text row 3
	iVkAlphaText3ForPrtWest = AknLayoutScalable_Avkon::cell_ituss_key_t4(1).LayoutLine();
	// Key text row 2                               
	iVkAlphaText2ForPrtWest = AknLayoutScalable_Avkon::cell_ituss_key_t3(1).LayoutLine();
	
	// Plus for Numberic Only keypad
	iVkTextPlusForNumbericOnlyForPrtWest = 
			AknLayoutScalable_Avkon::cell_ituss_key_t5(0).LayoutLine();
	
	// close button
	TAknWindowLineLayout funcbtn, funcbtnbg, funcbtninner;
	TAknLayoutRect funcrect, funcbgrect, funcinnerrect, arrowleftinnerrect, arrowrightinnerrect;
	TAknLayoutRect optioninnerrect, backspaceinnerrect;
	
	funcbtn = AknLayoutScalable_Avkon::cell_ituss_key_pane(1).LayoutLine();
	funcrect.LayoutRect(bottompanerect.Rect(), funcbtn);
	
	funcbtnbg = AknLayoutScalable_Avkon::bg_cell_ituss_key_g1(1).LayoutLine();
	funcbgrect.LayoutRect(funcrect.Rect(), funcbtnbg);
	
	funcbtninner = AknLayoutScalable_Avkon::cell_ituss_key_pane_g1(0).LayoutLine();
	funcinnerrect.LayoutRect(funcrect.Rect(), funcbtninner);
	
	TInt btnWidth = funcrect.Rect().Width();
	// close button
	iCloseRectForPrtWest = funcbgrect.Rect();
	iCloseRectForPrtWest.Move(0, funcrect.Rect().Height() * 4);
	iCloseRectForPrtWest.Move(-iLayoutOffsetForPrtWest);

	iCloseInnerRectForPrtWest = funcinnerrect.Rect();
	iCloseInnerRectForPrtWest.Move(0, funcrect.Rect().Height() * 4);
	iCloseInnerRectForPrtWest.Move(-iLayoutOffsetForPrtWest);
	
	// arrow left button
	iArrowLeftRectForPrtWest = iCloseRectForPrtWest;     
	iArrowLeftRectForPrtWest.Move(btnWidth, 0);
	arrowleftinnerrect.LayoutRect(iArrowLeftRectForPrtWest, funcbtninner);
	iArrowLeftInnerRectForPrtWest = arrowleftinnerrect.Rect();
	
	// option button
	iOptionsRectForPrtWest = iArrowLeftRectForPrtWest;
	iOptionsRectForPrtWest.Move(btnWidth, 0);    
	optioninnerrect.LayoutRect(iOptionsRectForPrtWest, funcbtninner);
	iOptionInnerRectForPrtWest = optioninnerrect.Rect();
	
	// arrow right button
	iArrowRightRectForPrtWest = iOptionsRectForPrtWest;
	iArrowRightRectForPrtWest.Move(btnWidth, 0);    
	arrowrightinnerrect.LayoutRect(iArrowRightRectForPrtWest, funcbtninner);
	iArrowRightInnerRectForPrtWest = arrowrightinnerrect.Rect();
	
	// backspace button
	iBackspaceRectForPrtWest = iArrowRightRectForPrtWest;
	iBackspaceRectForPrtWest.Move(btnWidth, 0);  
	backspaceinnerrect.LayoutRect(iBackspaceRectForPrtWest, funcbtninner);
	iBackspaceInnerRectForPrtWest = backspaceinnerrect.Rect();

	// preview popup window 
	TAknWindowLineLayout previewWnd, previewBackground, previewWndInner;
	TAknLayoutRect previewWndRect, previewBackgroundRect, previewWndInnerRect;
	TAknLayoutText previewWndText;
	previewWnd = AknLayoutScalable_Avkon::popup_fshwr2_char_preview_window(0).LayoutLine(); 
				
	previewWndRect.LayoutRect( iLayoutRectForPrtWest, previewWnd );
	previewBackground = AknLayoutScalable_Avkon::bg_popup_fep_char_preview_window_cp01().LayoutLine();
	previewBackgroundRect.LayoutRect( previewWndRect.Rect(), previewBackground );
	iPreviewWndRectForPrtWest = previewBackgroundRect.Rect();
	
	previewWndInner = AknLayoutScalable_Avkon::bg_popup_fep_char_preview_window_g9().LayoutLine();
	previewWndInnerRect.LayoutRect( previewBackgroundRect.Rect(), previewWndInner );
	
	iPreviewWndInnerRectForPrtWest = previewWndInnerRect.Rect();
	iPreviewWndTextForPrtWest = 
		AknLayoutScalable_Avkon::popup_fshwr2_char_preview_window_t1(0).LayoutLine();
	previewWndText.LayoutText(previewWndRect.Rect(), iPreviewWndTextForPrtWest);
	iBubbleFontForPrtWest = const_cast<CFont*>(previewWndText.Font());	
	
	// LAF for spell mode
	TAknWindowLineLayout spellwnd;
	TAknLayoutRect okBtnRect, cancelBtnRect, spellwndtrect;
	
	iSpellQueryPaneRectForPrtWest = mainWndRect.Rect();
	
    spellwnd = AknLayoutScalable_Avkon::popup_fep_ituss_window(1).LayoutLine();
    spellwndtrect.LayoutRect( mainWndRect.Rect(), spellwnd ); 
	
	TAknWindowLineLayout queryTopPane, queryICFPane, icfSpellIndiPane, 
		middleInnerBtn, middleBtn, bottomInnerBtn;
	TAknLayoutRect queryTopPaneRect, queryICFPaneRect, icfSpellIndiPaneRect, 
		bottomButton, bottomInnerRect, middleBtnRect;
	
	queryTopPane = AknLayoutScalable_Avkon::popup_fep_vtchi_icf_pane(1).LayoutLine();
	queryTopPaneRect.LayoutRect( mainWndRect.Rect(), queryTopPane );
	
	queryICFPane = AknLayoutScalable_Avkon::vtchi_query_pane(0).LayoutLine();
	queryICFPaneRect.LayoutRect( queryTopPaneRect.Rect(), queryICFPane );
	iSpellICFRectForPrtWest = queryICFPaneRect.Rect();
	
	// Middle pane
	TRect middleButtonPaneRect = spellwndtrect.Rect();
	TInt leftMargin = itucellrect.Rect().iTl.iX;
	middleButtonPaneRect.Move( leftMargin, 0 );
	
	middleBtn = AknLayoutScalable_Avkon::cell_ituss_key_pane(3).LayoutLine();
	middleBtnRect.LayoutRect( bottompanerect.Rect(), middleBtn );
	middleInnerBtn = AknLayoutScalable_Avkon::bg_cell_ituss_key_g1(5).LayoutLine();
	
	TRect cellMiddleRect;
	cellMiddleRect.iTl = middleButtonPaneRect.iTl;
	cellMiddleRect.SetHeight( middleBtnRect.Rect().Height());
	cellMiddleRect.SetWidth( middleBtnRect.Rect().Width());
	
	// OK button
	okBtnRect.LayoutRect( cellMiddleRect, middleInnerBtn );
	iOKRectForPrtWest = okBtnRect.Rect();
	
	// Cancel button
	cellMiddleRect.Move( middleBtnRect.Rect().Width(), 0 );
	
	cancelBtnRect.LayoutRect( cellMiddleRect, middleInnerBtn );
	iCancelRectForPrtWest = cancelBtnRect.Rect();
	
	iBtnOKTextForPrtWest = AknLayoutScalable_Avkon::ituss_sks_pane_t1(0).LayoutLine();
	iBtnCancelTextForPrtWest = AknLayoutScalable_Avkon::ituss_sks_pane_t2(0).LayoutLine();
	
	iBtnOKTextForPrtWest.ir = 7;
	iBtnCancelTextForPrtWest.ir = 7;
	
	iBtnCancelTextForPrtWest = iBtnOKTextForPrtWest;
	
	// Bottom pane
	TRect cellSpellRect;
	cellSpellRect.iTl.iX = middleButtonPaneRect.iTl.iX;
	cellSpellRect.iTl.iY = bottompanerect.Rect().iTl.iY + 
			itucellrect.Rect().Height() * 4;
	cellSpellRect.SetHeight( itucellrect.Rect().Height());
	cellSpellRect.SetWidth( itucellrect.Rect().Width());
		
	// Left
	bottomButton.LayoutRect( cellSpellRect, ituinnercell );
	iSpellArrowLeftRectForPrtWest = bottomButton.Rect();
	
	bottomInnerBtn = AknLayoutScalable_Avkon::cell_ituss_key_pane_g1(2).LayoutLine();
	bottomInnerRect.LayoutRect( bottomButton.Rect(), bottomInnerBtn );
	iSpellArrowLeftInnerRectForPrtWest = bottomInnerRect.Rect();
	
	// Right
	cellSpellRect.Move( itucellrect.Rect().Width(), 0 );
	bottomButton.LayoutRect( cellSpellRect, ituinnercell );
	iSpellArrowRightRectForPrtWest = bottomButton.Rect();
	
	bottomInnerRect.LayoutRect( bottomButton.Rect(), bottomInnerBtn );
	iSpellArrowRightInnerRectForPrtWest = bottomInnerRect.Rect();
	
	// BackSpace
	cellSpellRect.Move( itucellrect.Rect().Width(), 0 );
	bottomButton.LayoutRect( cellSpellRect, ituinnercell );
	iSpellICFBtnRectForPrtWest = bottomButton.Rect();
	
	bottomInnerRect.LayoutRect( bottomButton.Rect(), bottomInnerBtn );
	iSpellICFBtnInnerRectForPrtWest = bottomInnerRect.Rect();
	
	//iSpellText1ForPrtWest = AknLayoutScalable_Avkon::vtchi_query_pane_t1(0).LayoutLine();
	iSpellText1ForPrtWest = AknLayoutScalable_Avkon::vtchi_icf_list_pane_t1(0, 0, 0).LayoutLine();
	//iSpellText2ForPrtWest = AknLayoutScalable_Avkon::vtchi_query_pane_t2(0).LayoutLine();
	iSpellText2ForPrtWest = AknLayoutScalable_Avkon::vtchi_icf_list_pane_t1(0, 0, 1).LayoutLine();
	iSpellText3ForPrtWest = AknLayoutScalable_Avkon::vtchi_query_pane_t3(0).LayoutLine();
	
	iSpellIcfTextLeftMarginForPrtWest = iSpellText1ForPrtWest.il;
	iSpellIcfTextRightMarginForPrtWest = iSpellIcfTextLeftMarginForPrtWest;
	
	iSpellIcfFontForPrtWest = const_cast<CFont*>(
	        AknLayoutUtils::FontFromId(iSpellText1ForPrtWest.iFont, NULL));
	
	TAknLayoutText cSpelltxt1, cSpelltxt2;
	cSpelltxt1.LayoutText( queryICFPaneRect.Rect(), iSpellText1ForPrtWest );
	cSpelltxt2.LayoutText( queryICFPaneRect.Rect(), iSpellText2ForPrtWest );
	
	iSpellIcfTextTopMarginForPrtWest = 
			cSpelltxt1.TextRect().iTl.iY - queryICFPaneRect.Rect().iTl.iY;
	
	iSpellIcfTextLineSpaceMarginForPrtWest = 
			cSpelltxt2.TextRect().iTl.iY - cSpelltxt1.TextRect().iBr.iY;
	
	iSpellIcfTextHeightForPrtWest = cSpelltxt1.TextRect().Height();
	
	TInt maxSpellRow = ( iSpellICFRectForPrtWest.Height() - iSpellIcfTextTopMarginForPrtWest ) 
	        / ( iSpellIcfTextHeightForPrtWest + iSpellIcfTextLineSpaceMarginForPrtWest );
	
	iSpellIcfTextBottomMarginForPrtWest = 
			iSpellICFRectForPrtWest.Height() - 
			( iSpellIcfTextHeightForPrtWest + iSpellIcfTextLineSpaceMarginForPrtWest ) 
	        * maxSpellRow - iSpellIcfTextTopMarginForPrtWest;
	
	icfSpellIndiPane = AknLayoutScalable_Avkon::icf_edit_indi_pane(0).LayoutLine();
	icfSpellIndiPaneRect.LayoutRect(queryICFPaneRect.Rect(), icfSpellIndiPane);
	iSpellIndiPaneWithoutTextForPrtWest = icfSpellIndiPaneRect.Rect();
	
	TAknWindowLineLayout spellIndiIcon;
	TAknLayoutRect spellIndiIconRect;
	spellIndiIcon = AknLayoutScalable_Avkon::icf_edit_indi_pane_g2(0).LayoutLine();
	spellIndiIconRect.LayoutRect( icfSpellIndiPaneRect.Rect(), spellIndiIcon );
	iSpellIndiIconWithoutTextForPrtWest = spellIndiIconRect.Rect();
	}

void CGenericItutDataMgr::ReadLafInfo()
    {
    // Screen
    TRect rect, rectcn;
    AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EScreen, rect);
    iScreenSize = rect.Size();

    iLandScape = iScreenSize.iWidth < iScreenSize.iHeight ? EFalse : ETrue;

    ReadLafInfoForPrtWest();
    AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EApplicationWindow, rect);

    // finger Layout
    TAknWindowLineLayout vitulayout;
    TAknLayoutRect viturect;

    if (iLandScape)
        {
        vitulayout = AknLayoutScalable_Apps::popup_vitu2_window(1).LayoutLine();
        }
    else
        {
        vitulayout = AknLayoutScalable_Apps::popup_vitu2_window(0).LayoutLine();
        }
    
    viturect.LayoutRect(rect, vitulayout);

    iLayoutRect = viturect.Rect();
    iLayoutOffset = iLayoutRect.iTl;

    // ICF & Keypad
    TAknWindowLineLayout icflayoutlatin, icflayoutcn,itukeypad, itucell,icflayoutbgcn;
    TAknTextLineLayout icflefttext;    
    TAknTextLineLayout icflefttext1, icflefttext2, icflefttext3; 
    TAknLayoutRect icfrectlatin, icfrectcn, itukeypadrect, itucellrect,icfrectcnbg;

    if (iLandScape)
        {
        icflayoutlatin = AknLayoutScalable_Apps::field_vitu2_entry_pane(1).LayoutLine();
        icflayoutcn = AknLayoutScalable_Apps::field_vitu2_entry_pane(3).LayoutLine();
        }
    else
        {
        icflayoutlatin = AknLayoutScalable_Apps::field_vitu2_entry_pane(0).LayoutLine();
        icflayoutcn = AknLayoutScalable_Apps::field_vitu2_entry_pane(2).LayoutLine();
        }

    icfrectlatin.LayoutRect(viturect.Rect(), icflayoutlatin);
    icflayoutbgcn = AknLayoutScalable_Apps::bg_popup_call_pane_cp08(0).LayoutLine();
    icfrectcn.LayoutRect(viturect.Rect(), icflayoutcn);
    icfrectcnbg.LayoutRect(icfrectcn.Rect(),icflayoutbgcn);
	
    TAknLayoutScalableParameterLimits entryPaneVariety = 
                AknLayoutScalable_Apps::field_vitu2_entry_pane_t1_ParamLimits(2);
    // Calc the max row number
    TInt maxRow = entryPaneVariety.LastRow();
    
    // latin ICF layout
    icflefttext = AknLayoutScalable_Apps::field_vitu2_entry_pane_t1(2, 0, 0).LayoutLine();
    icflefttext1 = AknLayoutScalable_Apps::field_vitu2_entry_pane_t1(2, 0, 1).LayoutLine();
    icflefttext2 = AknLayoutScalable_Apps::field_vitu2_entry_pane_t1(2, 0, 2).LayoutLine();
    icflefttext3 = AknLayoutScalable_Apps::field_vitu2_entry_pane_t1(2, 0, maxRow).LayoutLine();

    iIcfTextAlignment = icflefttext.iJ;
    iIcfTextLeftMargin = icflefttext.il;
    iIcfTextRightMargin = icflefttext.ir;
    
    TAknLayoutText ctxt, ctxt1, ctxt2, ctxt3;
    ctxt.LayoutText( icfrectlatin.Rect(), icflefttext );
    ctxt1.LayoutText( icfrectlatin.Rect(), icflefttext1 );
    ctxt2.LayoutText( icfrectlatin.Rect(), icflefttext2 );
    ctxt3.LayoutText( icfrectlatin.Rect(), icflefttext3 );

    
    iIcfTextTopMargin = ctxt.TextRect().iTl.iY - icfrectlatin.Rect().iTl.iY;
    iIcfTextLineSpaceMargin = ctxt1.TextRect().iTl.iY - ctxt.TextRect().iBr.iY;
    iIcfTextBottomMargin = icfrectlatin.Rect().iBr.iY - ctxt3.TextRect().iBr.iY 
                                                      - iIcfTextLineSpaceMargin;  
    iIcfTextHeight = ctxt.TextRect().Height();
    
    iIcfFont = const_cast<CFont*>(AknLayoutUtils::FontFromId(icflefttext.iFont, NULL));
    
    // chinese ICF layout
   	entryPaneVariety = AknLayoutScalable_Apps::field_vitu2_entry_pane_t1_ParamLimits(0);
    // Calc the max row number
   	maxRow = entryPaneVariety.LastRow();
    icflefttext = AknLayoutScalable_Apps::field_vitu2_entry_pane_t1(0, 0, 0).LayoutLine();
    icflefttext1 = AknLayoutScalable_Apps::field_vitu2_entry_pane_t1(0, 0, 1).LayoutLine();
    icflefttext3 = AknLayoutScalable_Apps::field_vitu2_entry_pane_t1(0, 0, maxRow).LayoutLine();
    iIcfTextLeftMarginCn = icflefttext.il;
    iIcfTextRightMarginCn = icflefttext.ir;
    
    ctxt.LayoutText( icfrectcn.Rect(), icflefttext );
    ctxt1.LayoutText( icfrectcn.Rect(), icflefttext1 );
    ctxt3.LayoutText( icfrectcn.Rect(), icflefttext3 );
	
    TInt topOffset = icfrectcnbg.Rect().iTl.iY - icfrectcn.Rect().iTl.iY;
    iIcfTextTopMarginCn = ctxt.TextRect().iTl.iY - icfrectcn.Rect().iTl.iY - topOffset;
    iIcfTextLineSpaceMarginCn = ctxt1.TextRect().iTl.iY - ctxt.TextRect().iBr.iY;
    iIcfTextBottomMarginCn = icfrectcn.Rect().iBr.iY - ctxt3.TextRect().iBr.iY 
                                                     - iIcfTextLineSpaceMarginCn;
    iIcfTextHeightCn = ctxt.TextRect().Height();    
 	
    iIcfFontCn = const_cast<CFont*>(AknLayoutUtils::FontFromId(icflefttext.iFont, NULL));

    rect = icfrectlatin.Rect();
    rect.Move(-iLayoutOffset.iX, -iLayoutOffset.iY);
    iIcfWesRect = rect;
	
    rect = icfrectcnbg.Rect();
    rect.Move(-iLayoutOffset.iX, -iLayoutOffset.iY);
    iIcfChnRect = rect;
    //iIcfChnRect.iTl.iY += 10;
//#ifdef LAF_MODIFY
    //if(IsChinese())
//        {
//        iIcfChnRect.Move(0,-iIcfChnRect.iTl.iY + 14);
//        }
//#endif

    // Keypad
    TInt keypadvariety = iLandScape ? 1 : 0;
    itukeypad = AknLayoutScalable_Apps::grid_vitu2_itu_pane(keypadvariety).LayoutLine();
    itukeypadrect.LayoutRect(viturect.Rect(), itukeypad);
    iKeypadRect = itukeypadrect.Rect();
    iKeypadRect.Move(-iLayoutOffset.iX, -iLayoutOffset.iY);
    
    TInt cnParam  = iLandScape ? 3 : 2;    
    itukeypad = AknLayoutScalable_Apps::grid_vitu2_itu_pane(cnParam).LayoutLine();
    itukeypadrect.LayoutRect(viturect.Rect(), itukeypad);
    iKeypadRectCn = itukeypadrect.Rect();
    iKeypadRectCn.Move(-iLayoutOffset.iX, -iLayoutOffset.iY);

    TInt queryParam  = iLandScape ? 5 : 4;    
    itukeypad = AknLayoutScalable_Apps::grid_vitu2_itu_pane(queryParam).LayoutLine();
    itukeypadrect.LayoutRect(viturect.Rect(), itukeypad);
    iKeypadRectQuery = itukeypadrect.Rect();
    iKeypadRectQuery.Move(-iLayoutOffset.iX, -iLayoutOffset.iY);

    iKeypadCellRects.Reset();
    iKeypadCellRectsCn.Reset();
    iKeypadCellRectsQuery.Reset();

    // keypad is 4 rows, 3 cols
    for (TInt i = 0; i < 4; i++)
        {
        for (TInt j = 0; j < 3; j++)
            {
            itucell = AknLayoutScalable_Apps::cell_vitu2_itu_pane(keypadvariety, 
                                                                  j, 
                                                                  i).LayoutLine();
            itucellrect.LayoutRect(iKeypadRect, itucell);
            TRect cellrect = itucellrect.Rect();
            cellrect.Move(-iKeypadRect.iTl.iX, -iKeypadRect.iTl.iY);
            iKeypadCellRects.Append(cellrect);
            
            // read shift icon rect
            if( i == 3 &&  j == 2 )
            	{
            	TAknWindowLineLayout shiftIcon =  AknLayoutScalable_Apps::
            					cell_vitu2_itu_pane_g1( keypadvariety ).LayoutLine();
            	TAknLayoutRect shiftIconRect;
            	shiftIconRect.LayoutRect( cellrect, shiftIcon );				
            	iShiftIconRect = shiftIconRect.Rect();			
            	}
            itucellrect.LayoutRect(iKeypadRectCn, itucell);
            cellrect = itucellrect.Rect();
            cellrect.Move(-iKeypadRectCn.iTl.iX, -iKeypadRectCn.iTl.iY);
            iKeypadCellRectsCn.Append(cellrect);
           
            itucellrect.LayoutRect(iKeypadRectQuery, itucell);
            cellrect = itucellrect.Rect();
            cellrect.Move(-iKeypadRectQuery.iTl.iX, -iKeypadRectQuery.iTl.iY);
            iKeypadCellRectsQuery.Append(cellrect);
            }
        }

    iVkNumText = iLandScape ? AknLayoutScalable_Apps::cell_vitu2_itu_pane_t1(4).LayoutLine() :
    						  AknLayoutScalable_Apps::cell_vitu2_itu_pane_t1(0).LayoutLine(); 
	// Key text row 1   						  	
    iVkAlphaText1 = iLandScape ? AknLayoutScalable_Apps::cell_vitu2_itu_pane_t2(2).LayoutLine() :
    					AknLayoutScalable_Apps::cell_vitu2_itu_pane_t2(0).LayoutLine();
    iVkAlphaText3 = iLandScape ? AknLayoutScalable_Apps::cell_vitu2_itu_pane_t2(3).LayoutLine() :
    					AknLayoutScalable_Apps::cell_vitu2_itu_pane_t2(1).LayoutLine();
	iVkAlphaText3.iJ = 3;   

    iVkAlphaText1ForThai = iLandScape ? AknLayoutScalable_Apps::cell_vitu2_itu_pane_t3(6).LayoutLine() :
                            AknLayoutScalable_Apps::cell_vitu2_itu_pane_t2(7).LayoutLine();
    iVkAlphaText3ForThai = iLandScape ? AknLayoutScalable_Apps::cell_vitu2_itu_pane_t2(10).LayoutLine() :
                        AknLayoutScalable_Apps::cell_vitu2_itu_pane_t2(8).LayoutLine();
    iVkAlphaText3ForThai.iJ = 3;
					   					    					
    // Key text row 2    						  	
    iVkAlphaText2 = iLandScape ? AknLayoutScalable_Apps::cell_vitu2_itu_pane_t3(1).LayoutLine() :
    					AknLayoutScalable_Apps::cell_vitu2_itu_pane_t3(0).LayoutLine();
    iVkAlphaText2.iJ = 3;
    
    iVkAlphaText2ForThai = iLandScape ? AknLayoutScalable_Apps::cell_vitu2_itu_pane_t3(6).LayoutLine() :
                        AknLayoutScalable_Apps::cell_vitu2_itu_pane_t3(5).LayoutLine();
    iVkAlphaText2ForThai.iJ = 3;
    
    // backspace and switch button
    TAknWindowLineLayout funcbtn, funcclr, funcswitch, funcinner, funcinnercn, funcleft, funcright;
    TAknWindowLineLayout funcswitchcn, funcleftcn, funcrightcn, funcclrcn;
    TAknWindowLineLayout funcind, funcindcn, funcindinner, funcbtncn, funccell;
    TAknLayoutRect funcbtnrect, funcclrrect, funcswitchrect, funcbtncnrect;
    TAknLayoutRect funcindrect, funcindrectinner, funcindcnrect, funccellrect;
    TAknLayoutRect funcinnerrect, funcinnerrectcn, funcleftrect, funcrightrect;
    TAknLayoutRect funcclrcnrect, funcleftcnrect, funcrightcnrect, funcswitchcnrect;
  
    
    if (iLandScape)
        {
        funcbtn = AknLayoutScalable_Apps::grid_vitu2_function_pane(1).LayoutLine();
        funcbtncn = AknLayoutScalable_Apps::grid_vitu2_function_pane(3).LayoutLine();
        }
    else
        {
        funcbtn = AknLayoutScalable_Apps::grid_vitu2_function_pane(0).LayoutLine();
        funcbtncn = AknLayoutScalable_Apps::grid_vitu2_function_pane(2).LayoutLine();
        }

    funcclr = AknLayoutScalable_Apps::cell_vitu2_function_pane(0, 3, 0).LayoutLine();
    funcswitch = AknLayoutScalable_Apps::cell_vitu2_function_pane(0, 0, 0).LayoutLine();

    funcinner = AknLayoutScalable_Apps::cell_vitu2_function_pane_g1(0).LayoutLine();

    funcbtnrect.LayoutRect(viturect.Rect(), funcbtn);
    funcbtncnrect.LayoutRect(viturect.Rect(), funcbtncn);
    rect = funcbtnrect.Rect();
    rect.Move(-iLayoutOffset.iX, -iLayoutOffset.iY);
    funcclrrect.LayoutRect(rect, funcclr);
    funcswitchrect.LayoutRect(rect, funcswitch);

    // left and right button
    funcleft = AknLayoutScalable_Apps::cell_vitu2_function_pane(0, 1, 0).LayoutLine();
    funcright = AknLayoutScalable_Apps::cell_vitu2_function_pane(0, 2, 0).LayoutLine();
    funcleftrect.LayoutRect(rect, funcleft);
    funcrightrect.LayoutRect(rect, funcright);
    iArrowLeftRect 	= funcleftrect.Rect();
    iArrowRightRect	= funcrightrect.Rect();
    funcinnerrect.LayoutRect(iArrowLeftRect, funcinner);
    iArrowLeftInnerRect = funcinnerrect.Rect();
    funcinnerrect.LayoutRect(iArrowRightRect, funcinner);
    iArrowRightInnerRect = funcinnerrect.Rect();

    // funcinnerrect
    iBackspaceRect = funcclrrect.Rect();
    funcinnerrect.LayoutRect(iBackspaceRect, funcinner);
    iBackspaceInnerRect = funcinnerrect.Rect();
    iSwitchRect = funcswitchrect.Rect();
    funcinnerrect.LayoutRect( iSwitchRect, funcinner );
    iSwitchInnerRect = funcinnerrect.Rect();

    // Cursor button in Chinese input mode
    rect = funcbtncnrect.Rect();
    rect.Move(-iLayoutOffset.iX, -iLayoutOffset.iY);
    funcinnercn = AknLayoutScalable_Apps::cell_vitu2_function_pane_g1(1).LayoutLine();
    // 1. left arrow
    funccell = AknLayoutScalable_Apps::cell_vitu2_function_pane( 1, 1, 0 ).LayoutLine();
    funccellrect.LayoutRect( rect, funccell );
    iArrowLeftCnRect = funccellrect.Rect();
    funcinnerrectcn.LayoutRect( funccellrect.Rect(), funcinnercn );
    iArrowLeftInnerCnRect = funcinnerrectcn.Rect();
    
    // 2, up arrow
    funccell = AknLayoutScalable_Apps::cell_vitu2_function_pane( 1, 0, 0 ).LayoutLine();
    funccellrect.LayoutRect( rect, funccell );
    iArrowUpRect = funccellrect.Rect();
    funcinnerrectcn.LayoutRect( funccellrect.Rect(), funcinnercn );
    iArrowUpInnerRect = funcinnerrectcn.Rect();
    
    // 3. right arrow
    funccell = AknLayoutScalable_Apps::cell_vitu2_function_pane( 1, 2, 0 ).LayoutLine();
    funccellrect.LayoutRect( rect, funccell );
    iArrowRightCnRect = funccellrect.Rect();
    funcinnerrectcn.LayoutRect( funccellrect.Rect(), funcinnercn );
    iArrowRightInnerCnRect = funcinnerrectcn.Rect();
    
    // 4. down arrow
    funccell = AknLayoutScalable_Apps::cell_vitu2_function_pane( 1, 0, 1 ).LayoutLine();
    funccellrect.LayoutRect( rect, funccell );
    iArrowDownRect = funccellrect.Rect();
    funcinnerrectcn.LayoutRect( funccellrect.Rect(), funcinnercn );
    iArrowDownInnerRect = funcinnerrectcn.Rect();
#ifdef LAF_MODIFY    
    //indicator bar rect
    funccell = AknLayoutScalable_Apps::cell_vitu2_function_pane( 1, 1, 1 ).LayoutLine();
    funccellrect.LayoutRect( rect, funccell );
    iImIndiOuterRectCn = funccellrect.Rect();
    funccell = AknLayoutScalable_Apps::cell_vitu2_function_pane( 1, 2, 1 ).LayoutLine();
    funccellrect.LayoutRect(rect,funccell);
    iImIndiOuterRectCn.iBr = funccellrect.Rect().iBr;    

    iImIndicatorChnRect	= iImIndiOuterRectCn;
    iImIndicatorChnRect.Shrink( 24, 12 );

#endif
    
	// candidate list
    TAknWindowLineLayout candlist, candlistpane, charpane, listctrl;
    TAknLayoutRect candlistrect, candlistpanerect, charpanerect1, charpanerect2;
    TAknLayoutRect charpanerect3, listctrlrect;
	
    if (iLandScape)
        {
		candlist = AknLayoutScalable_Apps::popup_vitu2_dropdown_list_window(1).
										LayoutLine();        
        }
    else
        {
       	candlist = AknLayoutScalable_Apps::popup_vitu2_dropdown_list_window(0).
										LayoutLine();
        }
    candlistrect.LayoutRect( viturect.Rect(), candlist );
    rect = candlistrect.Rect();
    rect.Move(-iLayoutOffset.iX, -iLayoutOffset.iY);
    candlistpane = AknLayoutScalable_Apps::grid_vitu2_dropdown_list_pane().
										LayoutLine();
    candlistpanerect.LayoutRect( rect, candlistpane );
	
    charpane = AknLayoutScalable_Apps::cell_vitu2_dropdown_list_char_pane( 0 , 0 , 0 ).
										LayoutLine();
    charpanerect1.LayoutRect( candlistpanerect.Rect(), charpane );
    charpane = AknLayoutScalable_Apps::cell_vitu2_dropdown_list_char_pane( 0 , 1 , 0 ).
										LayoutLine();
    charpanerect2.LayoutRect( candlistpanerect.Rect(), charpane );
    charpane = AknLayoutScalable_Apps::cell_vitu2_dropdown_list_char_pane( 0 , 0 , 1 ).
										LayoutLine();
    charpanerect3.LayoutRect( candlistpanerect.Rect(), charpane );
    listctrl = AknLayoutScalable_Apps::cell_vitu2_dropdown_list_ctrl_pane( 0, 0, 0 ).
										LayoutLine();
    listctrlrect.LayoutRect( candlistpanerect.Rect(), listctrl );
									
    iCandsHorizontalMargin = charpanerect2.Rect().iTl.iX - charpanerect1.Rect().iBr.iX;
    iCandsVerticalMargin = charpanerect3.Rect().iTl.iY - charpanerect1.Rect().iBr.iY;
    iCandsNaviWidth = listctrlrect.Rect().Width();
	
    iCandsUnitWidth = charpanerect1.Rect().Width();
    iCandsUnitHeight = charpanerect1.Rect().Height() ;
    
    iCandsSpinBtnHeight = listctrlrect.Rect().Height();
    
    TInt gapValue = ( iScreenSize.iWidth - ( 
    		iCandsUnitWidth + iCandsHorizontalMargin + KCandsAdjustment ) * 6 ) / 2;
    if ( gapValue < 0 )
    	{
		gapValue = 0;
    	}
    
    iCandsSpellLTPos = charpanerect1.Rect().iTl;
    iCandsSpellLTPos.iX = gapValue;
    iCandsLTPos = charpanerect3.Rect().iTl;
    iCandsLTPos.iX = gapValue;
    iCandsPuncLTPos = iCandsSpellLTPos;
    
    TAknTextLineLayout candstxtlayout = 
                     AknLayoutScalable_Apps::cell_vitu2_dropdown_list_char_pane_t1().LayoutLine();
    TAknLayoutText candstxt;
    candstxt.LayoutText( charpanerect1.Rect(), candstxtlayout );
    iCandsFont = const_cast<CFont*>( AknLayoutUtils::FontFromId( candstxtlayout.iFont, NULL ) );
    iCandsTextMargin = candstxt.TextRect().iTl.iX - charpanerect1.Rect().iTl.iX;
    iCandsTextColor = candstxt.Color();
    
    //extra adjustment
    // No reason to add the iCandsTextMargin and KCandsAdjustment
    iCandsUnitWidth += KCandsAdjustment;
    iCandsNaviWidth += iCandsTextMargin;
    
    TPtrC ptr(reinterpret_cast<TText*>(&iLandScape), sizeof(iLandScape)/sizeof(TText));
    NotifySubscriber(MItutPropertySubscriber::EItutPropertyLandscape, ptr);

    TSize spellSize( iCandsUnitWidth * 6 + iCandsHorizontalMargin * 7, iCandsUnitHeight );
    iSpellRect = TRect( iCandsSpellLTPos, spellSize );
    iSpellFont = const_cast<CFont*>( AknLayoutUtils::FontFromId( candstxtlayout.iFont, NULL ) );
    iSpellTextColor = candstxt.Color();
    

    
    // Navi buttons
    TAknWindowLineLayout toppane, toppanecn, btnup, btndown, btnopt, btncls, btnback;
    TAknLayoutRect toppanerect, toppanerectcn, btnuprect, btndownrect, 
                   btnoptrect, btnoptrectcn, btnclsrect, btnclsrectcn, btnbackrectcn;
    TAknWindowLineLayout upinner, downinner, optinner, clsinner;
    TAknLayoutRect upinrect, downinrect, optinrect, optinrectcn, clsinrect, clsinrectcn;	
    if (iLandScape)
        {
        toppane = AknLayoutScalable_Apps::grid_vitu2_function_top_pane(1).LayoutLine();
        toppanecn = AknLayoutScalable_Apps::grid_vitu2_function_top_pane(3).LayoutLine();
        }
    else
        {
        toppane = AknLayoutScalable_Apps::grid_vitu2_function_top_pane(0).LayoutLine();
        toppanecn = AknLayoutScalable_Apps::grid_vitu2_function_top_pane(2).LayoutLine();
        }	
    toppanerect.LayoutRect( viturect.Rect(), toppane );
    toppanerectcn.LayoutRect(viturect.Rect(), toppanecn);
    rect = toppanerect.Rect();
    rectcn = toppanerectcn.Rect();
    rect.Move(-iLayoutOffset.iX, -iLayoutOffset.iY);
    rectcn.Move(-iLayoutOffset.iX, -iLayoutOffset.iY);
    
    // Chinese input mode
    funcswitchcn = AknLayoutScalable_Apps::cell_vitu2_function_top_pane( 0, 2, 0 ).LayoutLine();
    funcswitchcnrect.LayoutRect( rectcn, funcswitchcn );
    iSwitchCnRect = funcswitchcnrect.Rect();
    funcinnercn = AknLayoutScalable_Apps::cell_vitu2_function_top_pane_g1(0).LayoutLine();
    funcinnerrectcn.LayoutRect( funcswitchcnrect.Rect(), funcinnercn );
    iSwitchInnerCnRect = funcinnerrectcn.Rect();
    
    btnopt= AknLayoutScalable_Apps::cell_vitu2_function_top_pane( 0, 1, 0 ).LayoutLine();
    btnoptrect.LayoutRect( rect, btnopt );
    btnoptrectcn.LayoutRect(rectcn, btnopt);
    optinner = AknLayoutScalable_Apps::cell_vitu2_function_top_pane_g1(1).LayoutLine();
    optinrect.LayoutRect( btnoptrect.Rect(), optinner );
    optinner = AknLayoutScalable_Apps::cell_vitu2_function_top_pane_g1(0).LayoutLine();
    optinrectcn.LayoutRect(btnoptrectcn.Rect(), optinner);
    iOptionInnerRect = optinrect.Rect();
#ifdef LAF_MODIFY	
    iOptionInnerRectCn = optinrectcn.Rect();
#endif	
    btncls= AknLayoutScalable_Apps::cell_vitu2_function_top_pane( 0, 0, 0 ).LayoutLine();
    btnclsrect.LayoutRect( rect, btncls );
    btnclsrectcn.LayoutRect(rectcn, btncls);
    clsinner = AknLayoutScalable_Apps::cell_vitu2_function_top_pane_g1(1).LayoutLine();
    clsinrect.LayoutRect( btnclsrect.Rect(), clsinner );
    clsinner = AknLayoutScalable_Apps::cell_vitu2_function_top_pane_g1(0).LayoutLine();
    clsinrectcn.LayoutRect(btnclsrectcn.Rect(), clsinner);
    iCloseInnerRect = clsinrect.Rect();
#ifdef LAF_MODIFY	
    iCloseInnerRectCn = clsinrectcn.Rect();
#endif	
    btnback= AknLayoutScalable_Apps::cell_vitu2_function_top_pane( 0, 3, 0 ).LayoutLine();
    btnbackrectcn.LayoutRect(rectcn, btnback);
    iBackspaceCnRect = btnbackrectcn.Rect();
    funcinner = AknLayoutScalable_Apps::cell_vitu2_function_top_pane_g1(0).LayoutLine();
    funcinnerrect.LayoutRect( btnbackrectcn.Rect(), funcinner );
    iBackspaceInnerCnRect = funcinnerrect.Rect();
	
	
    iOptionsRect = btnoptrect.Rect();
    iOptionsRectCn = btnoptrectcn.Rect();
    iCloseRect = btnclsrect.Rect();
    iCloseRectCn = btnclsrectcn.Rect();

    // input mode indicator, should use laf data in the future
#ifndef LAF_MODIFY    
    funcindcn = AknLayoutScalable_Apps::field_vitu2_entry_pane_g1().LayoutLine();
    funcindcnrect.LayoutRect( icfrectcn.Rect(), funcindcn );
#endif    
    funcind = AknLayoutScalable_Apps::cell_vitu2_function_top_wide_pane(1).LayoutLine();
    funcindrect.LayoutRect(rect, funcind);
    
    funcindinner = AknLayoutScalable_Apps::cell_vitu2_function_top_wide_pane_g1(2).LayoutLine();
    funcindrectinner.LayoutRect(funcindrect.Rect(), funcindinner);
    //iBackspaceCnRect = funcindcnrect.Rect();

    iImIndicatorWesRect = funcindrectinner.Rect();	
    iImIndiOuterRect = funcindrect.Rect();
#ifndef LAF_MODIFY
    funcindinner = AknLayoutScalable_Apps::cell_vitu2_function_top_wide_pane_g1(1).LayoutLine();
    funcindrectinner.LayoutRect(funcindrect.Rect(), funcindinner);    
    iImIndicatorChnRect = funcindrectinner.Rect();
#endif
	// match selection list
	TAknWindowLineLayout matchlist, listpane, itempane1, itempane2, scrollpane;
	TAknLayoutRect matchlistrect, listpanerect, itempanerect1, itempanerect2, scrollpanerect;
	TAknWindowLineLayout arrowup, arrowdown, arrowupinner, arrowdowninner;
	TAknLayoutRect arrowuprect, arrowdownrect, arrowupinnerRect, arrowdowninnerRect;
	
	if( iLandScape )
		{
		matchlist = AknLayoutScalable_Apps::popup_vitu2_match_list_window(3).LayoutLine();
		listpane = AknLayoutScalable_Apps::list_vitu2_match_list_pane(1).LayoutLine();
		scrollpane = AknLayoutScalable_Apps::vitu2_page_scroll_pane(1).LayoutLine();
		}
	else
		{
		matchlist = AknLayoutScalable_Apps::popup_vitu2_match_list_window(2).LayoutLine();
		listpane = AknLayoutScalable_Apps::list_vitu2_match_list_pane(0).LayoutLine();
		scrollpane = AknLayoutScalable_Apps::vitu2_page_scroll_pane(0).LayoutLine();
		}
	
	matchlistrect.LayoutRect( viturect.Rect(), matchlist );
	rect = matchlistrect.Rect();
    rect.Move(-iLayoutOffset.iX, -iLayoutOffset.iY);	
    
	listpanerect.LayoutRect( rect, listpane );
	scrollpanerect.LayoutRect( rect, scrollpane );
	itempane1 = AknLayoutScalable_Apps::list_vitu2_match_list_item_pane( 0, 0, 0 ).LayoutLine();
	itempanerect1.LayoutRect( listpanerect.Rect(), itempane1 );
	itempane2 = AknLayoutScalable_Apps::list_vitu2_match_list_item_pane( 0, 0, 1 ).LayoutLine();
	itempanerect2.LayoutRect( listpanerect.Rect(), itempane2 );
	
	arrowupinner= AknLayoutScalable_Apps::vitu2_page_scroll_pane_g1().LayoutLine();
	arrowdowninner = AknLayoutScalable_Apps::vitu2_page_scroll_pane_g2().LayoutLine();
	arrowup = AknLayoutScalable_Apps::bg_button_pane_cp023().LayoutLine();
	arrowdown= AknLayoutScalable_Apps::bg_button_pane_cp024().LayoutLine();
	arrowuprect.LayoutRect( scrollpanerect.Rect(), arrowup );
	arrowdownrect.LayoutRect( scrollpanerect.Rect(), arrowdown );
	arrowupinnerRect.LayoutRect( scrollpanerect.Rect(), arrowupinner );
	arrowdowninnerRect.LayoutRect( scrollpanerect.Rect(), arrowdowninner );
	
	//iSelectionTl = rect.iTl;
	iSelectionHorMargin = listpanerect.Rect().iTl.iX;
	iSelectionVerMargin = listpanerect.Rect().iTl.iY;
	iSelectionTl = TPoint(iIcfWesRect.iTl.iX + 10, iIcfWesRect.iTl.iY + 10);
	iSelectionItemSize = itempanerect1.Rect().Size();
	iSelectionItemMargin = itempanerect1.Rect().iBr.iY - itempanerect2.Rect().iTl.iY;
	iSelectionNaviSize = arrowuprect.Rect().Size();
	iSelectionNaviInnerSize = arrowupinnerRect.Rect().Size();
	iSelectionTextLine = AknLayoutScalable_Apps::list_vitu2_match_list_item_pane_t1().LayoutLine();
    iSelectionPageTextLine = AknLayoutScalable_Apps::vitu2_page_scroll_pane_t1().LayoutLine();
	
	// read spell popup window laf
	TAknWindowLineLayout queryPane, btnOK, btnCancle, ICFArea, btnClr, btnClrInner;
	TAknWindowLineLayout editIndicator;
	TAknLayoutRect queryPaneRect, btnOKRect, btnCancleRect, ICFAreaRect, btnClrRect;
	TAknLayoutRect btnClrInnerRect, editIndicatorRect; 
	TAknTextLineLayout btnOKText, btnCancleText;
	if( iLandScape )
		{
		queryPane = AknLayoutScalable_Apps::popup_vitu2_query_window(1).LayoutLine();
		}
	else
		{
		queryPane = AknLayoutScalable_Apps::popup_vitu2_query_window(0).LayoutLine();	
		}
	queryPaneRect.LayoutRect( viturect.Rect(), queryPane );
	btnOK = AknLayoutScalable_Apps::bg_button_pane_cp016().LayoutLine();
	btnOKRect.LayoutRect( queryPaneRect.Rect(), btnOK );
	btnCancle = AknLayoutScalable_Apps::bg_button_pane_cp017().LayoutLine();
	btnCancleRect.LayoutRect( queryPaneRect.Rect(), btnCancle );
	btnOKText = AknLayoutScalable_Apps::popup_vitu2_query_window_t4(0).LayoutLine();
	btnCancleText = AknLayoutScalable_Apps::popup_vitu2_query_window_t5(0).LayoutLine();
	
	editIndicator = AknLayoutScalable_Apps::popup_vitu2_query_window_g1().LayoutLine();
	editIndicatorRect.LayoutRect( queryPaneRect.Rect(), editIndicator  );
	
	ICFArea = AknLayoutScalable_Apps::input_focus_pane_cp08().LayoutLine();
	ICFAreaRect.LayoutRect( queryPaneRect.Rect(), ICFArea  );
	
	btnClr = AknLayoutScalable_Apps::bg_button_pane_cp015().LayoutLine();
	btnClrRect.LayoutRect( queryPaneRect.Rect(), btnClr );
	btnClrInner = AknLayoutScalable_Apps::popup_vitu2_query_window_g2().LayoutLine();
	btnClrInnerRect.LayoutRect( queryPaneRect.Rect(), btnClrInner );
	
	iSpellQueryPaneRect = queryPaneRect.Rect();
	iSpellEditIndicator = editIndicatorRect.Rect();
    iOkRect = btnOKRect.Rect();
    iCancelRect = btnCancleRect.Rect();
    iSpellICFRect = ICFAreaRect.Rect();
    iSpellClrRect = btnClrRect.Rect();
    iSpellClrInnerRect = btnClrInnerRect.Rect();  
    
    //iBtnTextFormat  = iVkAlphaText1;//btnOKText;
	//iBtnTextFormat1 = iVkAlphaText1;//btnCancleText;
    iBtnTextFormat = AknLayoutScalable_Apps::popup_vitu2_query_button_pane_t1(0).LayoutLine();
    iBtnTextFormat1 = AknLayoutScalable_Apps::popup_vitu2_query_button_pane_t1(0).LayoutLine();
	
	
    // just test if there is size change of added ctrls
    iSendRect = iArrowLeftRect;
    iDisconnRect = iArrowRightRect;
    // end test    
    
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
    }
    
void CGenericItutDataMgr::SetTextAlignment()
    {
    CFepLayoutMultiLineIcf* icffield = static_cast<CFepLayoutMultiLineIcf*>(iLayoutContext->Control(ECtrlIdICF));
    
    if (icffield && InputLanguage() != ELangNone)
        {
        if ( IsPortraitWest())
            {
            TRAP_IGNORE(icffield->SetTextAlignmentL(
                    iIcfTextAlignmentForPrtWest, InputLanguage()));
            }
        else
            {
            TRAP_IGNORE(icffield->SetTextAlignmentL(
                    iIcfTextAlignment, InputLanguage()));
            }
        }
    }
    
void CGenericItutDataMgr::SetIcfTextAlignment(TInt aAlignment)
    {
    if ( IsPortraitWest())
        {
        iIcfTextAlignmentForPrtWest = aAlignment;
        }
    else
        {
        iIcfTextAlignment = aAlignment;
        }
    }

void CGenericItutDataMgr::SetLanguageL(TInt aLanguage)
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
            
            // Remove the resource file of previous input language.
            // The previous resource file should be removed first,
            // Because there are some same keypad ids in the resource file
            // of latin, Cyrillic, and etc.
            if ( iConfigResId )
                {
                coeEnv->DeleteResourceFile( iConfigResId );
                }

            // Add the resource file of current input language
            TFileName resourceConfigName( iResourceFilename );
            BaflUtils::NearestLanguageFile( coeEnv->FsSession(), resourceConfigName );
            iConfigResId = coeEnv->AddResourceFileL( resourceConfigName );
            
            if (IsChinese())
                {
                iLayoutContext->UiManager()->CreateChineseSpecificCtrlsIfNeededL();
                }
            
            if( IsKorean())
                {
                iLayoutContext->UiManager()->CreateKoreanSpecificCtrlsIfNeededL();
                }

            NotifyChangeControlLayout(MItutPropertySubscriber::EItutPropertyKeypadResourceId, 
                                      KeypadResourceId());  
            }
        }
    }

void CGenericItutDataMgr::SetCase(TInt aCase)
    {
    if (iCase != aCase || iInputMode == ELatin)
        {
        iCase = aCase;
        NotifyChangeControlLayout(MItutPropertySubscriber::EItutPropertyKeypadResourceId, 
                                  KeypadResourceId());  
        }
    }

void CGenericItutDataMgr::SetInputModeL(TInt aMode)
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
        case EHangul:
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




void CGenericItutDataMgr::SetChineseSpellFlag( TBool aFlag )
	{
	iIsChineseSpell = aFlag;
	}

TBool CGenericItutDataMgr::IsChineseSpellMode()
	{
	return iIsChineseSpell;
	}

TAny* CGenericItutDataMgr::RequestDataForPortraitWestUIAndChineseSpellUI(TInt aDataType)
    {
    switch ( aDataType )
        {
        case EScreenSize:
            {
            return &iScreenSize;
            }
        case ELayoutOffset:
            {
            return &iLayoutOffsetForPrtWest;
            }
        case ELayoutRect:
            {
            return &iLayoutRectForPrtWest;
            }
        case EIcfRect:
            {
            return &iIcfRectForPrtWest;
            }
        case EIcfFont:
            {
            if ( IsChineseSpellMode())
            	{
				return reinterpret_cast<TAny*>(iIcfFont);
            	}
            else
                {
                return reinterpret_cast<TAny*>(iIcfFontForPrtWest);
                }
            }
        case EBackspaceRect:
        case EItutPosClear:
            {
            return &iBackspaceRectForPrtWest;
            }
        case EKeypadRect:
            {
			if ( IsChineseSpellMode())
            	{
				return &iKeypadRect;
            	}
            else
                {
                return &iKeypadRectForPrtWest;
                }
            }               
        case EKeypadCellRects:
            {
            if ( IsChineseSpellMode())
            	{
				return &iKeypadCellRects;
            	}
            else
                {
                return &iKeypadCellRectsForPrtWest;
                }
            }
        case EKeypadLeftTextLine:
            {
            if ( IsChineseSpellMode())
                {
                return &iVkNumText;
                }
            else
                {
                // Modify new begin
                if ( iInputMode == ENumber || iInputMode == ENativeNumber )
                	{
					return &iVkBigNumTextForPrtWest;
                	}
                else
                	{
					return &iVkNumTextForPrtWest;
                	}
                // Modify new end
                }
            }
        case EKeypadRightTextLine1:
            {
            if ( IsChineseSpellMode())
                {
                return IsThai() ? &iVkAlphaText1ForThai : &iVkAlphaText1;
                }
            else
                {
				if ( iInputMode == ENumber || iInputMode == ENativeNumber )
					{
					return &iVkTextPlusForNumbericOnlyForPrtWest;
					}
				else
					{
					return &iVkAlphaText1ForPrtWest;
					}
                }
            }
        case EKeypadRightTextLine2:
            {
            if ( IsChineseSpellMode())
                {
                return IsThai() ? &iVkAlphaText2ForThai : &iVkAlphaText2;
                }
            else
                {
                return &iVkAlphaText2ForPrtWest;
                }
            }
        case EKeypadRightTextLine3:
            {
            if ( IsChineseSpellMode())
                {
                return IsThai() ? &iVkAlphaText3ForThai : &iVkAlphaText3;
                }
            else
                {
                return &iVkAlphaText3ForPrtWest;
                }
            }
        case EDropdownlistUnitWidth:
            {
            return &iCandsUnitWidth;
            }
        case EDropdownlistUnitHeight:
            {
            return &iCandsUnitHeight;
            }
        case EDropdownlistHorizontalMargin:
            {
            return &iCandsHorizontalMargin;
            }
        case EDropdownlistVerticalMargin:
            {
            return &iCandsVerticalMargin;
            }
        case EDropdownListNaviWidth:
            {
            return &iCandsNaviWidth;
            }
        case EDropdownListSpinBtnHeight:
            {
            return &iCandsSpinBtnHeight;
            }
        case EDropdownListSpellLTPos:
            {
            return &iCandsSpellLTPos;
            }
        case EDropdownListCandsLTPos:
            {
            return &iCandsLTPos;
            }
        case EDropdownListPuncLTPos:
            {
            return &iCandsPuncLTPos;
            }
        case EDropdownListFont:
            {
            return reinterpret_cast<TAny*>(iCandsFont);
            }
        case EDropdownListTextMargin:
            {
            return &iCandsTextMargin;
            }
        case EDropdownListTextColor:
            {
            return &iCandsTextColor;
            }
        case ESpellRect:
            {
            return &iSpellRect;
            }
        case ESpellFont:
            {
            return reinterpret_cast<TAny*>(iSpellIcfFontForPrtWest);
            }
        case ESpellTextColor:
            {
            return &iSpellTextColor;
            }
        case ELanguage:
            {
            return &iLanguage;
            }
        case EInputMode:
            {
            return &iInputMode;
            }
        case ECase:
            {
            return &iCase;
            }
        case EChnCandidates:
            {
            return &iCandidates;
            }
        case EChnPuncCandidates:
            {
            return &iPuncCandidates;
            }
        case EArrowLeftRect:
        case EItutPosArrowLeft:
            {
            return &iArrowLeftRectForPrtWest;
            }
        case EArrowRightRect:
        case EItutPosArrowRight:
            {
            return &iArrowRightRectForPrtWest;
            }
        case EArrowUpRect:
        case EItutPosArrowUp:
            {
            return &iArrowUpRect;
            }
        case EArrowDownRect:
        case EItutPosArrowDown:
            {
            return &iArrowDownRect;
            }
        case EItutPosSend:
            {
            return &iSendRect;
            }
        case EItutPosDisconn:
            {
            return &iDisconnRect;
            }
        case EMatchSelectionTlPos:
            {
            return &iSelectionTl;
            }
        case EMatchSelectionItemSize:
            {
            return &iSelectionItemSize;
            }
        case EMatchSelectionItemMargin:
            {
            return &iSelectionItemMargin;
            }
        case EMatchSelectionTextLine:
            {
            return &iSelectionTextLine;
            }
        case EOptionsRect:   
            {
            return &iOptionsRectForPrtWest;
            }           
        case ECloseRect:
            {
            return &iCloseRectForPrtWest;
            }          
        case ELeftInnerRect:
            {
            return &iArrowLeftInnerRectForPrtWest;
            }
        case ERightInnerRect:
            {
            return &iArrowRightInnerRectForPrtWest;
            }
        case EUpInnerRect:
            {
            return &iArrowUpInnerRect;
            }
        case EDownInnerRect:
            {
            return &iArrowDownInnerRect;
            }
        case EBackspaceInnerRect:
            {
            return &iBackspaceInnerRectForPrtWest;
            }
        case ECloseInnerRect:
            {
            return &iCloseInnerRectForPrtWest;
            }       
        case EOptionInnerRect:
            {
            return &iOptionInnerRectForPrtWest;
            }         
        case EItutPosOk:
            {
            if ( IsChineseSpellMode())
                {
                return &iOkRect;
                }
            else
                {
                return &iOKRectForPrtWest;
                }
            }
        case EItutPosCancel:
            {
            if ( IsChineseSpellMode())
                {
                return &iCancelRect;
                }
            else
                {
                return &iCancelRectForPrtWest;
                }
            }
        case EBtnTextLine:
            {
            if ( IsChineseSpellMode())
                {
                return &iBtnTextFormat;
                }
            else
                {
                return &iBtnOKTextForPrtWest;
                }
            }
        case ESpellTextCancle:
            {
            if ( IsChineseSpellMode())
                {
                return &iBtnTextFormat1;
                }
            else
                {
                return &iBtnCancelTextForPrtWest;
                }
            }
        case ESpellICFRect:
            {
            if ( IsChineseSpellMode())
                {
                return &iSpellICFRect;
                }
            else
                {
                return &iSpellICFRectForPrtWest;
                }
            }
        case ESpellClrRect:
            {
            if ( IsChineseSpellMode())
                {
                return &iSpellClrRect;
                }
            else
                {
                return &iSpellICFBtnRectForPrtWest;
                }
            }
        case ESpellClrInnerRect:
            {
            if ( IsChineseSpellMode())
                {
                return &iSpellClrInnerRect;
                }
            else
                {
                return &iSpellICFBtnInnerRectForPrtWest;
                }
            }
        case ESpellEditIndicatorRect:
            {
            return &iSpellEditIndicator;
            }
        case ESpellQueryPaneRect:
            {
            if ( IsChineseSpellMode())
                {
                return &iSpellQueryPaneRect;
                }
            else
                {
                return &iSpellQueryPaneRectForPrtWest;
                }
            }
        case ELatinOnly:
            {
            return &iLatinOnly;
            }
        case EPreviewBubbleRect:
            {
            return &iPreviewWndRectForPrtWest;
            }
        case EPreviewBubbleInnerRect:
            {
            return &iPreviewWndInnerRectForPrtWest;
            }   
        case EPreviewBubbleTextlayout:
            {
            return &iPreviewWndTextForPrtWest;
            }
        case EPreviewBubbleFont:
            {
            return reinterpret_cast<TAny*>(iBubbleFontForPrtWest);
            }
        case EImIndicatorRect:
            {
            if (iSpellMode)
                {
                return &iSpellEditIndicator;
                }
                 
            CalWesternIndicator();
            return &iCalindwesternRect;
            }
        case EImIndicatorOuterRect:
            {
            return &iImIndiOuterRect;
            }
        case EIndiPaneWithTextRect:
            {
            return &iIndiPaneRectWithTextForPrtWest;
            }
        case EIndiPaneWithoutTextRect:
            {
            return &iIndiPaneRectWithoutTextForPrtWest;
            }
        case EIndiIconWithTextRect:
            {
            return &iIndiIconRectWithTextForPrtWest;
            }
        case EIndiIconWithoutTextRect:
            {
            return &iIndiIconRectWithoutTextForPrtWest;
            }
        case EIndiTextLine:
            {
            return &iIndiTextForPrtWest;
            }
        case ESpellIndiPaneWithoutTextRect:
            {
            return &iSpellIndiPaneWithoutTextForPrtWest;
            }
        case ESpellIndiIconWithoutTextRect:
            {
            return &iSpellIndiIconWithoutTextForPrtWest;
            }
        case ESpellArrowLeft:
        	{
        	return &iSpellArrowLeftRectForPrtWest;
        	}
        case ESpellArrowLeftInner:
        	{
        	return &iSpellArrowLeftInnerRectForPrtWest;
        	}
        case ESpellArrowRight:
        	{
        	return &iSpellArrowRightRectForPrtWest;
        	}
        case ESpellArrowRightInner:
        	{
        	return &iSpellArrowRightInnerRectForPrtWest;
        	}
        default:
            break;
        }

    return NULL;
    }
TAny* CGenericItutDataMgr::RequestData(TInt aDataType)
    {
    if ( IsPortraitWest())
        {
        return RequestDataForPortraitWestUIAndChineseSpellUI( aDataType );
        }
    switch ( aDataType )
        {
        case EScreenSize:
            return &iScreenSize;
        case ELayoutOffset:
            return &iLayoutOffset;
        case ELayoutRect:
            return &iLayoutRect;
        case EIcfRect:
            return IsChinese() ? &iIcfChnRect : & iIcfWesRect;
        case EIcfFont:
            return IsChinese() ? reinterpret_cast<TAny*>(iIcfFontCn) : 
                                 reinterpret_cast<TAny*>(iIcfFont);
        case EBackspaceRect:
        case EItutPosClear:
            return IsChinese() ? &iBackspaceCnRect : &iBackspaceRect;
        case ESwitchRect:
        case EItutPosSwitchLayout:
            return IsChinese() ? &iSwitchCnRect : &iSwitchRect;
        case EImIndicatorRect:
            if (iSpellMode)
            	{
            	return &iSpellEditIndicator;
            	}
            	
            if (IsChinese())
            	{
            	return &iImIndicatorChnRect;
            	} 
            CalWesternIndicator();
            return &iCalindwesternRect;
       	case EImIndicatorOuterRect:
       	    if(IsChinese())
       	        {
       	        return &iImIndiOuterRectCn;
       	        }
       	    else
       	        {
       	        return &iImIndiOuterRect;
       	        }
        case EKeypadRect:
        	return IsChinese() ? &iKeypadRectCn : &iKeypadRect;	        	
        case EKeypadCellRects:
        	return IsChinese() ? &iKeypadCellRectsCn : &iKeypadCellRects;	
        case EKeypadLeftTextLine:
            return &iVkNumText;
        case EKeypadRightTextLine1:
            return IsThai() ? &iVkAlphaText1ForThai : &iVkAlphaText1;
        case EKeypadRightTextLine2:
            return IsThai() ? &iVkAlphaText2ForThai : &iVkAlphaText2;
        case EKeypadRightTextLine3:
            return IsThai() ? &iVkAlphaText3ForThai : &iVkAlphaText3;
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
        case ESpellRect:
            return &iSpellRect;
        case ESpellFont:
            return reinterpret_cast<TAny*>(iSpellFont);
        case ESpellTextColor:
            return &iSpellTextColor;
        case ELanguage:
            return &iLanguage;
        case EInputMode:
            return &iInputMode;
        case ECase:
            return &iCase;
        case EChnCandidates:
            return &iCandidates;
        case EChnPuncCandidates:
            return &iPuncCandidates;
        case EArrowLeftRect:
        case EItutPosArrowLeft:
            return IsChinese() ? &iArrowLeftCnRect : &iArrowLeftRect;
        case EArrowRightRect:
        case EItutPosArrowRight:
            return IsChinese() ? &iArrowRightCnRect : &iArrowRightRect;
        case EArrowUpRect:
        case EItutPosArrowUp:
            return &iArrowUpRect;
        case EArrowDownRect:
        case EItutPosArrowDown:
            return &iArrowDownRect;
        case EItutPosSend:
            return &iSendRect;
        case EItutPosDisconn:
            return &iDisconnRect;
        case EMatchSelectionTlPos:
            return &iSelectionTl;
        case EMatchSelectionItemSize:
            return &iSelectionItemSize;
        case EMatchSelectionItemMargin:
            return &iSelectionItemMargin;
        case EMatchSelectionTextLine:
            return &iSelectionTextLine;
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
       	case ESwitchInnerRect:
        	return IsChinese() ? &iSwitchInnerCnRect : &iSwitchInnerRect;
        case ELeftInnerRect:
        	return IsChinese() ? &iArrowLeftInnerCnRect : &iArrowLeftInnerRect;
        case ERightInnerRect:
        	return IsChinese() ? &iArrowRightInnerCnRect : &iArrowRightInnerRect;
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
        case EItutPosOk:
            return &iOkRect;
        case EItutPosCancel:
            return &iCancelRect;
        case EBtnTextLine:
            return &iBtnTextFormat;
        case ESpellTextCancle:
            return &iBtnTextFormat1;            
        case ESpellICFRect:
            return &iSpellICFRect;
        case ESpellClrRect:
            return &iSpellClrRect;
        case ESpellClrInnerRect:
            return &iSpellClrInnerRect;
        case ESpellEditIndicatorRect:
            return &iSpellEditIndicator;
        case ESpellQueryPaneRect:
            return &iSpellQueryPaneRect;       
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
        default:
            break;
        }

    return NULL;
    }

CGenericItutDataMgr::CGenericItutDataMgr(MItutLayoutContext* aLayoutContext, 
                                         CPtiEngine* aPtiEngine)
                                        :
                                         iPtiEngine( aPtiEngine ),
                                         iLanguage(ELangNone),
                                         iInputMode(KInvalidImMode),
                                         iCase(EAknEditorUpperCase),
                                         iIsChineseSpell( EFalse ),
                                         iLayoutContext(aLayoutContext),
                                         iCurrentScriptIdx(KInvalidIndex),
                                         iLandScape(EFalse),
                                         iSpellMode(EFalse)
    {
    }

void CGenericItutDataMgr::NotifySubscriber(MItutPropertySubscriber::TItutProperty aPropertyName, 
                                           const TDesC& aValue)
    {
    for (TInt i = 0; i < iSubscriberList.Count(); i++)
        {
        TRAP_IGNORE(iSubscriberList[i]->SetPropertyL(aPropertyName, aValue));
        }
    }

void CGenericItutDataMgr::NotifyChangeControlLayout(
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

TInt CGenericItutDataMgr::GetNextPageCandidateL(RPointerArray<HBufC>& aList)
    {
    if (iPtiEngine && (iPtiEngine->NextCandidatePage()))
        {
        TItutDataConverter::ConvertChnPhraseCandidateL(iPtiEngine->CandidatePage(), aList);

        return KErrNone;
        }

    return KErrNotSupported;
    }

TBool CGenericItutDataMgr::NextPageCandidateExist()
    {
    if (iPtiEngine)
        {
        return iPtiEngine->MoreCandidatePages();
        }

    return EFalse;    
    }

TInt CGenericItutDataMgr::KeypadResourceId()
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
        
    if ( IsKorean() )
        {
        return KoreanKeypadResourceId();
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
    
TInt CGenericItutDataMgr::ChineseKeypadResourceId()
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

TInt CGenericItutDataMgr::HindiKeypadResourceId()
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
    
TInt CGenericItutDataMgr::KoreanKeypadResourceId()
    {
    if ( iInputMode == EHangul )
        {
        return R_FINGER_INPUT_KEYPAD_KOREAN;
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
    
    
TInt CGenericItutDataMgr::GetConfigResFileName(TInt aLang)
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

TInt CGenericItutDataMgr::FindLang(const TInt* aArray, 
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

TBool CGenericItutDataMgr::IsChinese()
    {
    return (iLanguage == ELangPrcChinese || iLanguage == ELangHongKongChinese 
         || iLanguage == ELangTaiwanChinese);
    }

// tp teleca addon
TBool CGenericItutDataMgr::IsKorean()
    {
    return ( iLanguage == ELangKorean );
    }

TBool CGenericItutDataMgr::IsHindi()
    {
    return ( iLanguage == ELangHindi );
    }

TBool CGenericItutDataMgr::IsThai()
    {
    return ( iLanguage == ELangThai );
    }

TBool CGenericItutDataMgr::IsRtoLLanguage()
	{
	return ( iLanguage == ELangArabic ||
			 iLanguage == ELangHebrew ||
			 iLanguage == ELangFarsi  ||
			 iLanguage == ELangUrdu );	
	}
	
TBool CGenericItutDataMgr::IsRtoLLanguage( TInt aLanguage )
	{
	return ( aLanguage == ELangArabic ||
			 aLanguage == ELangHebrew ||
			 aLanguage == ELangFarsi  ||
			 aLanguage == ELangUrdu );	
	}

TBool CGenericItutDataMgr::IsLangDirectionSwitch() const
	{
	return iIsLangDirectionSwitch;	
	}
	
const TDesC& CGenericItutDataMgr::ResFileExtByScriptIdx(const TScriptExtIndex aIndex)
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
        case EHangulExt:
        	return KHangulExt;
        default:
            break;    
        }

    return KNullDesC;
    }
    
void CGenericItutDataMgr::SetUpdate(TBool aNeedUpdate)
	{
	iNeedUpdate = 	aNeedUpdate;
	}
	
TBool CGenericItutDataMgr::IsUpdate() const
	{
	return 	iNeedUpdate;
	}
	
void CGenericItutDataMgr::CalWesternIndicator()
	{
    TSize indSize(iImIndicatorWesRect.Width() * 5 / 6, iImIndicatorWesRect.Height() * 5 / 6);
    
    TInt offsetX = ( iImIndicatorWesRect.Width() - indSize.iWidth ) / 2;
    TInt offsetY = ( iImIndicatorWesRect.Height() - indSize.iHeight ) / 2;
    
    TInt x = iImIndicatorWesRect.iTl.iX + offsetX;
    TInt y = iImIndicatorWesRect.iTl.iY + offsetY;
    
    iCalindwesternRect.SetRect(TPoint(x,y) , indSize);	
	}

void CGenericItutDataMgr::SetNumericKeymap( const TInt aKeymap )
	{
	iNumericKeymap = aKeymap;
	}

void CGenericItutDataMgr::SetNumericKeymapData( TInt* aKeymapData )
	{
	delete iNumericKeymapData;
    TInt* len = (TInt*)((TUint8*)aKeymapData - 4);
    TPtrC16 keymapRes((const TUint16*)aKeymapData, *len/2);  
    iNumericKeymapData = keymapRes.Alloc();
	}

TBool CGenericItutDataMgr::IsExistPlusChar()
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
