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
* Description:  trigger string implementation
*
*/


// INCLUDES
#include <aknlayoutscalable_avkon.cdl.h>
#include <AknUtils.h>
#include <AknsUtils.h>

// USER INCLUDE
#include "peninputhwrfscnlafmanager.h"

// CONSTANTS
const TUint32 KDefaultTextColor = 0x000000;
const TUint32 KDefaultShadowTextColor = 0x000000;
const TInt KCandidateNum = 10; 

// -----------------------------------------------------------------------------
// PeninputHwrfscnLAFManager::ControlBarLeftTopPosition()
// .
// -----------------------------------------------------------------------------
//
TPoint PeninputHwrfscnLAFManager::ControlBarLeftTopPosition()
    {
    TRect rect;
    AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EScreen, rect);
	TBool isLandscape = rect.Width() < rect.Height() ? EFalse : ETrue;
	
    TAknWindowLineLayout screenLayoutLine = 
		AknLayoutScalable_Avkon::Screen().LayoutLine();
		
		
	TAknWindowLineLayout areaTopLayoutLine = 
		AknLayoutScalable_Avkon::area_top_pane( 0 ).LayoutLine();
		
	TAknLayoutRect areaTopRect;
	areaTopRect.LayoutRect( screenLayoutLine.Rect(), areaTopLayoutLine );	
	
		
	TAknWindowLineLayout statusPaneLayoutLine = 
		isLandscape ?
		AknLayoutScalable_Avkon::status_pane( 0 ).LayoutLine() :
		AknLayoutScalable_Avkon::status_pane_g1( 0 ).LayoutLine();
		
	TAknLayoutRect statusPaneRect;
	statusPaneRect.LayoutRect( areaTopRect.Rect(), statusPaneLayoutLine );	
	
	
	TAknWindowLineLayout titlePaneLayoutLine = 
		AknLayoutScalable_Avkon::title_pane( 0 ).LayoutLine();
		
	TAknLayoutRect titlePaneRect;
	titlePaneRect.LayoutRect( statusPaneRect.Rect(), titlePaneLayoutLine );
	
	
    return titlePaneRect.Rect().iTl;
    }

// -----------------------------------------------------------------------------
// PeninputHwrfscnLAFManager::ControlBarUnitRect()
// .
// -----------------------------------------------------------------------------
//
TRect PeninputHwrfscnLAFManager::ControlBarUnitRect()
    {
	TRect mainPaneRect;
	AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EMainPane, mainPaneRect );
	
	TAknWindowComponentLayout controlBarWindowLayoutLine = 
		AknLayoutScalable_Avkon::
		popup_fep_china_hwr2_fs_control_window( 0 ).LayoutLine();
	TAknLayoutRect controlBarWindowRect;
	controlBarWindowRect.LayoutRect( mainPaneRect, controlBarWindowLayoutLine );

    TAknWindowComponentLayout controlBarGridLayoutLine = 
		AknLayoutScalable_Avkon::aid_fep_china_hwr2_fs_cell( 0 ).LayoutLine();
	TAknLayoutRect controlBarGridRect;
	controlBarGridRect.LayoutRect( controlBarWindowRect.Rect(), 
	    controlBarGridLayoutLine );

	return TRect( controlBarGridRect.Rect().iTl, 
				  TSize( CandidateUnitRect().Width(), 
				  		 CandidateUnitRect().Height() ) );
    }
	
// -----------------------------------------------------------------------------
// PeninputHwrfscnLAFManager::CandidateUnitRect()
// .
// -----------------------------------------------------------------------------
//
TRect PeninputHwrfscnLAFManager::CandidateUnitRect()
    {
    TRect rect;
    AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EScreen, rect);
    TInt width = rect.Width() < rect.Height() ? rect.Width() : rect.Height();    
    TRect candidateRect(TPoint(0,0), TSize(width/KCandidateNum,width/KCandidateNum));
    return candidateRect;
    }

// -----------------------------------------------------------------------------
// PeninputHwrfscnLAFManager::ScrollBarRect()
// .
// -----------------------------------------------------------------------------
//
TRect PeninputHwrfscnLAFManager::ScrollBarRect()
    {
	TRect mainPaneRect;
	AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EMainPane, mainPaneRect );
	
	TAknWindowComponentLayout listScrollPane = 
	    AknLayoutScalable_Avkon::listscroll_gen_pane( 0 ).LayoutLine();
	TAknLayoutRect listScrollRect;
	listScrollRect.LayoutRect( mainPaneRect, listScrollPane );
	
    TAknWindowComponentLayout scrollPane = 
        AknLayoutScalable_Avkon::scroll_pane().LayoutLine();
	TAknLayoutRect scrollRect;
	scrollRect.LayoutRect( listScrollRect.Rect(), scrollPane );
	
	return scrollRect.Rect();
    }

// -----------------------------------------------------------------------------
// PeninputHwrfscnLAFManager::CandidateFont()
// .
// -----------------------------------------------------------------------------
//    
const CFont* PeninputHwrfscnLAFManager::CandidateFont()
    {
    TAknTextLineLayout candTextLayout = 
            AknLayoutScalable_Avkon::cell_hwr_candidate_pane_t1().LayoutLine();
	
	return AknLayoutUtils::FontFromId( candTextLayout.iFont, NULL );
    }

// -----------------------------------------------------------------------------
// PeninputHwrfscnLAFManager::CandidateBtnDownBgColor()
// .
// -----------------------------------------------------------------------------
//    
void PeninputHwrfscnLAFManager::CandidateBtnDownBgColor( TRgb& aBtnDownColor )
    {
    TAknTextLineLayout candTextLayout = 
            AknLayoutScalable_Avkon::cell_hwr_candidate_pane_t1().LayoutLine();    
    
    aBtnDownColor = AKN_LAF_COLOR_STATIC( candTextLayout.iC );
    }

// -----------------------------------------------------------------------------
// PeninputHwrfscnLAFManager::GetLangTextColor()
// .
// -----------------------------------------------------------------------------
// 
void PeninputHwrfscnLAFManager::GetLangTextColor( TRgb& aTextColor )
    {
    TInt error = AknsUtils::GetCachedColor( AknsUtils::SkinInstance(),
        aTextColor,
        KAknsIIDQsnTextColors,
        EAknsCIQsnTextColorsCG59 );
    
    if ( error != KErrNone )
        {
    	aTextColor = TRgb(  KDefaultTextColor  );
        }
    }

// -----------------------------------------------------------------------------
// PeninputHwrfscnLAFManager::GetLangShadowTextColor()
// .
// -----------------------------------------------------------------------------
// 
void PeninputHwrfscnLAFManager::GetLangShadowTextColor( TRgb& aTextColor )
    {
    TInt error = AknsUtils::GetCachedColor( AknsUtils::SkinInstance(),
         aTextColor,
         KAknsIIDQsnTextColors,
         EAknsCIQsnTextColorsCG62 );
    
    if ( error != KErrNone )
        {
    	aTextColor = TRgb(  KDefaultShadowTextColor  );
        }
    }

// -----------------------------------------------------------------------------
// PeninputHwrfscnLAFManager::LangSwitchButtonFont()
// .
// -----------------------------------------------------------------------------
// 
const CFont* PeninputHwrfscnLAFManager::LangSwitchButtonFont()
    {
    TAknTextLineLayout langSwitch = 
        AknLayoutScalable_Avkon::cell_vkb_side_pane_t1().LayoutLine();
    
    return AknLayoutUtils::FontFromId( langSwitch.iFont, NULL );
    }

// -----------------------------------------------------------------------------
// PeninputHwrfscnLAFManager::LangSwitchButtonShadowFont()
// .
// -----------------------------------------------------------------------------
// 
const CFont* PeninputHwrfscnLAFManager::LangSwitchButtonShadowFont()
    {
    TAknTextLineLayout langSwitchShadow = 
        AknLayoutScalable_Avkon::cell_vkb_side_pane_t1_copy1().LayoutLine();
    
    return AknLayoutUtils::FontFromId( langSwitchShadow.iFont, NULL );
    }

// -----------------------------------------------------------------------------
// PeninputHwrfscnLAFManager::SymbolTableFont()
// .
// -----------------------------------------------------------------------------
//
const CFont* PeninputHwrfscnLAFManager::SymbolTableFont()            
    {
    TAknTextLineLayout keypadText =
		AknLayoutScalable_Avkon::cell_hwr_candidate_pane_t1().LayoutLine();
	
	return AknLayoutUtils::FontFromId(  keypadText.iFont, NULL  );
    }

// -----------------------------------------------------------------------------
// PeninputHwrfscnLAFManager::GetSymbolTableCharSizeOffset()
// .
// -----------------------------------------------------------------------------
//
TSize PeninputHwrfscnLAFManager::GetSymbolTableCharSizeOffset()
	{
	TAknTextLineLayout keypadText =
         AknLayoutScalable_Avkon::cell_hwr_candidate_pane_t1().LayoutLine();
    
    return TSize( keypadText.il,keypadText.il );	
	}

TRect PeninputHwrfscnLAFManager::MainPaneRect()
    {
    TRect mainPaneRect;
    AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EMainPane, mainPaneRect  );
    return mainPaneRect;
    }
      
// End Of File
