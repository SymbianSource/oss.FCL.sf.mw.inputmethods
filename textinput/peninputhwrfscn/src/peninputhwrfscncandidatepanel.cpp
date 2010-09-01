/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for chinese peninput full screen hwr
*
*/

// SYSTEM INCLUDES
#include <AknUtils.h>
#include <aknfeppeninputenums.h>

// RESOURCE INCLUDE
#include <peninputhwrfscreencn.rsg>

// USER INCLUDES
#include "peninputhwrfscnlayout.h"
#include "peninputhwrfscncandidatepanel.h"
#include "peninputhwrfscnevent.h"
#include "peninputhwrfscncontrolid.h"
#include "peninputhwrfscnlafmanager.h"

// CONSTANT DEFINITION HEADER
#include "peninputhwrfscnstoreconstants.h"

TBool GetLandScape()
	{
    TPixelsTwipsAndRotation ptSize;
    CCoeEnv::Static()->ScreenDevice()->GetDefaultScreenSizeAndRotation( ptSize );
    
    // set proper screen style FS HWR
    // if width less than height then landscape is EFalse, or is ETrue
    TBool isLandScape = ptSize.iPixelSize.iWidth > ptSize.iPixelSize.iHeight;
    	
    return isLandScape;
	}

// ============================ MEMBER FUNCTIONS ===============================
  
// -----------------------------------------------------------------------------
// CPeninputHwrfscnCandidatePanel::NewL()
// .
// -----------------------------------------------------------------------------
//
CPeninputHwrfscnCandidatePanel* CPeninputHwrfscnCandidatePanel::NewL( 
    CPeninputHwrfscnLayout* aLayout )
    {
    CPeninputHwrfscnCandidatePanel* self = new( ELeave ) 
        CPeninputHwrfscnCandidatePanel( aLayout );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnCandidatePanel::CPeninputHwrfscnCandidatePanel()
// .
// -----------------------------------------------------------------------------
//	
CPeninputHwrfscnCandidatePanel::CPeninputHwrfscnCandidatePanel( 
    CPeninputHwrfscnLayout* aLayout )
	:CPeninputHwrfscnBasePanel( aLayout ),
	iDataStore( aLayout->DataStore() )
    {
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnCandidatePanel::~CPeninputHwrfscnCandidatePanel()
// .
// -----------------------------------------------------------------------------
//
CPeninputHwrfscnCandidatePanel::~CPeninputHwrfscnCandidatePanel()
    {    
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnCandidatePanel::ConstructL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnCandidatePanel::ConstructL()
    {
    iPos.iX = 0;
    iPos.iY = 0;
    
    TBool isLandScape = GetLandScape();
    
    ReadLafInfo( isLandScape );
    
    // add candidate dropdown list
    AddCandidateDropdownListL();
    
    // add predictive dropdown list
    AddPredictiveDropdownListL();

    iDataStore.AddSubscriber( this );
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnCandidatePanel::Show()
// .
// -----------------------------------------------------------------------------
//       
void CPeninputHwrfscnCandidatePanel::Show( TBool aVisible )
    {
    if( !aVisible )
        {
        if( !iCandidateList->Hiden() )
        	{
        	iCandidateList->ResetAndClear();	
        	}
        	
        if( !iPredictList->Hiden() )
        	{
        	iPredictList->ResetAndClear();	
        	}
        }
        
    iCandidateList->Hide( !aVisible );
    iPredictList->Hide( !aVisible );
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnCandidatePanel::Draw()
// .
// -----------------------------------------------------------------------------
// 
void CPeninputHwrfscnCandidatePanel::Draw()
    {    
    }
        
// -----------------------------------------------------------------------------
// CPeninputHwrfscnCandidatePanel::HandleControlEvent()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnCandidatePanel::HandleControlEvent( TInt aEventType, 
    CFepUiBaseCtrl* aCtrl, 
	const TDesC& aEventData )
    {
    ReportEvent( aEventType, aCtrl, aEventData );
    }		                        
	    
// -----------------------------------------------------------------------------
// CPeninputHwrfscnCandidatePanel::HandleCommand()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnCandidatePanel::HandleCommand( TInt aCommand, 
    TUint8* aData )
    {
    if( aCommand == EHwrEventPointerOutsideWnd )
    	{
    	Show( EFalse );	
    	}
    else if( aCommand == EHwrEventCursorPosChanged )
        {
        TRect cursorRect = *( TRect* )aData;
        if( iPos.iY != cursorRect.iBr.iY )
            {
            MoveCandidateWindow( cursorRect );
            }
        }
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnCandidatePanel::ShowWritingCandidate()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnCandidatePanel::ShowWritingCandidate( TBool aVisible )
	{
	if( !aVisible )
		{
		if( !iCandidateList->Hiden() )
			{
			iCandidateList->ResetAndClear();	
			}
		}

    iCandidateList->Hide( !aVisible );
	}

// -----------------------------------------------------------------------------
// CPeninputHwrfscnCandidatePanel::BeforePropertyChangedL()
// .
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrfscnCandidatePanel::BeforePropertyChangedL(               
    MPeninputHwrfscnPropertySubscriber::TPeninputHwrfscnProperty /*aProptery*/ )
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnCandidatePanel::PropertyChangedL()
// .
// -----------------------------------------------------------------------------
//    
void CPeninputHwrfscnCandidatePanel::PropertyChangedL( 
	MPeninputHwrfscnPropertySubscriber::TPeninputHwrfscnProperty aProptery )
    {
    switch ( aProptery )
        {
        case MPeninputHwrfscnPropertySubscriber::EPeninputHwrfscnPropertyCandidates:
            {
            iCandidateList->Hide( EFalse );
            
            RPointerArray<HBufC> candidates = iDataStore.Candidate();
            TInt primaryRange = iDataStore.PrimaryRange();
            
            if ( primaryRange == ERangeNumber )
                {
                // display an un-expandable candidate list in number range
                iCandidateList->SetCandidatesL( candidates, 
                    CFepCtrlDropdownList::EListNoExpandable );
                }
            else// in non number range
                {
                // display an expandable candidate list
                iCandidateList->SetCandidatesL( candidates, 
                    CFepCtrlDropdownList::EListExpandable );
                }
            
            // set the first cell high light
            iCandidateList->SetHighlightCell( 0 );
            iLayout->LayoutOwner()->FlushUi();
            break;
            }
            
        case MPeninputHwrfscnPropertySubscriber::
            EPeninputHwrfscnPropertyHomephonicCandidates:
            {
            RPointerArray<HBufC> candidates = iDataStore.Candidate();
            
            if ( candidates.Count() <= KCandidateCountNumMode )
                {
                // display an un-expandable candidate list
                iCandidateList->SetCandidatesL( candidates, 
                     CFepCtrlDropdownList::EListNoExpandable );
                }
            else
                {
                // display an expandable candidate list
                iCandidateList->SetCandidatesL( candidates, 
                     CFepCtrlDropdownList::EListExpandable );    
                }
            
            iCandidateList->SetHighlightCell( 0 );
            iLayout->LayoutOwner()->FlushUi();
            break;
            }
            
        case MPeninputHwrfscnPropertySubscriber::
            EPeninputHwrfscnPropertyPredictives:
            {
            iPredictList->Hide( EFalse );
            
            TInt primaryRange = iDataStore.PrimaryRange();
            RPointerArray<HBufC> predicts = iDataStore.Predictive();            
            
            if ( primaryRange == ERangeNumber || 
                 primaryRange == ERangeEnglish )
                {
                // display an un-expandable predictive list
                iPredictList->SetCandidatesL( predicts, 
                    CFepCtrlDropdownList::EListNoExpandable );
                }
            else// in native range
                {
                // display an expandable predictive list
                iPredictList->SetCandidatesL( predicts, 
                    CFepCtrlDropdownList::EListExpandable );
                }
            iLayout->LayoutOwner()->FlushUi();
            break;
            }
            
       	case MPeninputHwrfscnPropertySubscriber::
       	    EPeninputHwrfscnPropertyAddPredictives:
       	    {
            RPointerArray<HBufC> predicts = iDataStore.Predictive();
            
            // add predict when pressing next page
            iPredictList->AddCandidatesL( predicts, 
                CFepCtrlDropdownList::EListExpandable );
       	    
       	    break;
       	    }
            
        default:
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnCandidatePanel::SizeChanged()
// .
// -----------------------------------------------------------------------------
//    	  
void CPeninputHwrfscnCandidatePanel::SizeChanged()
    {
    ReadLafInfo( GetLandScape() );
 
    // candidate dropdown list
    iCandidateList->SetFont( PeninputHwrfscnLAFManager::CandidateFont() );

    iCandidateList->SizeChanged( iUnitWidth,
        iUnitHeight,
        iUnitWidth,
        iPos,
        KCandidateCountPerRow,
        KCandidateRowCount );
                               
    // predictive dropdown list
    iPos.iY += iUnitHeight;
    iPredictList->SetFont( PeninputHwrfscnLAFManager::CandidateFont() );

    iPredictList->SizeChanged( iUnitWidth,
        iUnitHeight,
        iUnitWidth,
        iPos,
        KCandidateCountPerRow,
        KCandidateRowCount );
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnCandidatePanel::LafChanged()
// .
// -----------------------------------------------------------------------------
//	    
void CPeninputHwrfscnCandidatePanel::LafChanged()
    {
    
    } 
// -----------------------------------------------------------------------------
// CPeninputHwrfscnCandidatePanel::AddCandidateDropdownListL()
// .
// -----------------------------------------------------------------------------
//	    
void CPeninputHwrfscnCandidatePanel::AddCandidateDropdownListL()
    {
    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC( reader, 
         R_FSHWR_CANDIDATE_DROP_DOWN_LIST );

	const CFont *font = PeninputHwrfscnLAFManager::CandidateFont();
    TInt textMargin = ( iUnitHeight - font->HeightInPixels() ) / 2;

    // create the candidate list
    
    TRgb btnDownColor;
    PeninputHwrfscnLAFManager::CandidateBtnDownBgColor( btnDownColor );
    
    TDropdownListDrawInfo candiDrawInfo(KAknsIIDQsnFrFunctionButtonInactive, 
    									KAknsIIDQsnFrFunctionButtonNormal,
    								    KAknsIIDQsnFrFunctionButtonPressed,
    						            KAknsIIDQsnFrItutButtonCandiSideL,
    						            KAknsIIDQsnFrItutButtonCandiMiddle,
    						            KAknsIIDQsnFrItutButtonCandiSideR,
                                        KAknsIIDQsnFrItutButtonCandiPressedSideL,
                                        KAknsIIDQsnFrItutButtonCandiPressedMiddle,
                                        KAknsIIDQsnFrItutButtonCandiPressedSideR,
    						            TRgb(194, 221, 242),
    						            ETrue);

    
    iCandidateList = CFepCtrlDropdownList::NewL( iPos,
        iLayout,
        EHwrCtrlIdCandidateList,
        reader,                                            
        iUnitWidth,
        iUnitHeight,
        iUnitWidth,
        KCandidateCountPerRow,
        KCandidateRowCount,
        font,
        NULL,
        CGraphicsContext::ECenter,
        textMargin,
        KRgbBlack,       // border color 
        TRgb( 0xeeeeee ),  // content bg color
        KRgbWhite,       // navi bg color
        btnDownColor,
        KRgbRed );

    AddToRootControlL( iCandidateList );
    
    // set candidate list resource
    iCandidateList->SetResourceId( R_FSHWR_CANDIDATE_DROP_DOWN_LIST );
    
    // set the needed events
    iCandidateList->SetEventIdForCandidateSelected( EHwrEventCandidateSelected );
    
    iCandidateList->AddEventObserver( this );
    
    // hide the candidate list
    iCandidateList->Hide( ETrue );
    
    iCandidateList->SetDropdownListImgID(candiDrawInfo);
    
    CleanupStack::PopAndDestroy();
    }    
 
// -----------------------------------------------------------------------------
// CPeninputHwrfscnCandidatePanel::AddPredictiveDropdownListL()
// .
// -----------------------------------------------------------------------------
//	    
void CPeninputHwrfscnCandidatePanel::AddPredictiveDropdownListL()
    {
    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC( reader, 
        R_FSHWR_PREDICT_DROP_DOWN_LIST );
	
	const CFont *font = PeninputHwrfscnLAFManager::CandidateFont();
    TInt textMargin = ( iUnitHeight - font->HeightInPixels() ) / 2;

    // create the predictive list
    TRgb btnDownColor;
    PeninputHwrfscnLAFManager::CandidateBtnDownBgColor( btnDownColor );
    
    						            
    TDropdownListDrawInfo candiDrawInfo(KAknsIIDQsnFrFunctionButtonInactive, 
                                        KAknsIIDQsnFrFunctionButtonNormal,
                                        KAknsIIDQsnFrFunctionButtonPressed,
                                        KAknsIIDQsnFrItutButtonComposeSideL,
                                        KAknsIIDQsnFrItutButtonComposeMiddle,
                                        KAknsIIDQsnFrItutButtonComposeSideR,
                                        KAknsIIDQsnFrItutButtonComposePressedSideL,
                                        KAknsIIDQsnFrItutButtonComposePressedSideR,
                                        KAknsIIDQsnFrItutButtonComposePressedMiddle,
                                        TRgb(194, 221, 242),
                                        ETrue);     

    
    iPredictList = CFepCtrlDropdownList::NewL( iPos,
        iLayout,
        EHwrCtrlIdPredictiveList,
        reader,                                            
        iUnitWidth,
        iUnitHeight,
        iUnitWidth,
        KCandidateCountPerRow,
        KCandidateRowCount,
        font,
        NULL,
        CGraphicsContext::ECenter,
        textMargin,
        KRgbBlack,       // border color 
        TRgb( 0xeeeeee ),  // content bg color
        KRgbWhite,       // navi bg color
        btnDownColor,
        KRgbRed );

    AddToRootControlL( iPredictList );
    
    // set the predictive list resource
    iPredictList->SetResourceId( R_FSHWR_PREDICT_DROP_DOWN_LIST );
    
    // set the needed events
    iPredictList->SetEventIdForCandidateSelected( 
        EHwrEventPredictiveSelected );
    iPredictList->SetEventIdForNextPageCandidate( 
        EHwrEventGetNextCandidatePage );
    iPredictList->SetEventIdForPreviousPageCandidate( 
        EHwrEventGetPreviousCandidatePage );
    iPredictList->AddEventObserver( this );
    
    // hide the predictive list
    iPredictList->Hide( ETrue );
    
    iPredictList->SetDropdownListImgID(candiDrawInfo);
    
    CleanupStack::PopAndDestroy();
    }    
    
    
// -----------------------------------------------------------------------------
// CPeninputHwrfscnCandidatePanel::ReadLafInfo
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnCandidatePanel::ReadLafInfo( TBool /*aLandscapeStyle*/ )
	{
    // if size is changing, or unit size, shadow size have not been set yet
    TRect rect( PeninputHwrfscnLAFManager::CandidateUnitRect() );
    iUnitWidth = rect.Width();
    iUnitHeight = rect.Height();
 	}
 
// -----------------------------------------------------------------------------
// CPeninputHwrfscnCandidatePanel::MoveCandidateWindow
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnCandidatePanel::MoveCandidateWindow( 
    const TRect aCursorRect )
    {
    // clear the candidate window first
    TRect oldCandidateRect = iCandidateList->Rect();
    TRect oldPredictRect = iPredictList->Rect();
    ClearWindow();

	// calculate the extened list max height by primary range
    TInt extendedListHeight = MaxRowNum() * iUnitHeight;

    if( !CheckCursorPosition( aCursorRect ) )
        {
        // if the cursor position is not valid, set a default value
		iPos.iY = iLayout->ScreenSize().iHeight - extendedListHeight - 1;
        }
    else
        {
        // set the default candidate window position to be below the cursor
        iPos = aCursorRect.iBr;        
        }
    
    // calculate the bottom right Y of candidate window
    TInt brOfCanWindow = iPos.iY + extendedListHeight;   
    
    // get the screen extent
    TSize screenSize = iLayout->ScreenSize();
    
    // check if the extened window overflow
    if( brOfCanWindow > screenSize.iHeight )
        {
        // move the position to be above the cursor
        iPos.iY = aCursorRect.iTl.iY - 3 * iUnitHeight;
        
        brOfCanWindow = iPos.iY + extendedListHeight;
        
        // check if the extened window still overflow
        if( brOfCanWindow > screenSize.iHeight )
            {
            iPos.iY = screenSize.iHeight - extendedListHeight;
            }
        }

    // Set the candiate winodw x positon to be center of the screen width
    if(screenSize.iWidth < screenSize.iHeight)
        {
        iPos.iX = ( screenSize.iWidth - iUnitWidth * KCandidateCountNumMode ) / 2;
        }
    else
        {
        TRect mainPane = PeninputHwrfscnLAFManager::MainPaneRect();
        
        iPos.iX = ( mainPane.Width() - iUnitWidth * KCandidateCountNumMode ) / 2;
        }
    
    // set the candiate list position
    iCandidateList->SetTopLeftPoint( iPos );
    
    // set the predict list position 
    iPredictList->SetTopLeftPoint( TPoint( iPos.iX,iPos.iY+iUnitHeight ) );
    
    iLayout->UpdateArea( iCandidateList, oldCandidateRect, EFalse );
    iLayout->UpdateArea( iPredictList, oldPredictRect, EFalse );
    } 

// -----------------------------------------------------------------------------
// CPeninputHwrfscnCandidatePanel::ClearWindow
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnCandidatePanel::ClearWindow()
    {
    iCandidateList->Clear();
    iPredictList->Clear();
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnCandidatePanel::MaxRowNum
// -----------------------------------------------------------------------------
//
TInt CPeninputHwrfscnCandidatePanel::MaxRowNum()    
    {
    /*TInt primaryRange = iDataStore.PrimaryRange();
    
    if(primaryRange == ERangeNative)
        {
        return KMaxCandidateWinRowCount;
        }
    else if(primaryRange == ERangeNumber)
        {
        return KCandidateListCount;
        }        
    else if(primaryRange == ERangeEnglish)
        {
        return KCandidateRowCount;
        }*/
    
    return KMaxCandidateWinRowCount;
    }
    
// -----------------------------------------------------------------------------
// CPeninputHwrfscnCandidatePanel::CheckCursorPosition()
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrfscnCandidatePanel::CheckCursorPosition( 
    const TRect aCursorRect ) const
    {
    if( aCursorRect.iBr.iY < 0 || 
        aCursorRect.iBr.iY > iLayout->ScreenSize().iHeight )
        {
        return EFalse;
        }
    else
        {
        return ETrue;
        }
    }
    
//End of file
