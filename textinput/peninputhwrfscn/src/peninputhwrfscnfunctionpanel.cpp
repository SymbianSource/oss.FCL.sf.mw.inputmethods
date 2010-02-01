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
#include <AknsUtils.h>
#include <aknfeppeninputenums.h>
#include <peninputlayout.h>
#include <AknFepGlobalEnums.h>
#include <aknlayoutscalable_avkon.cdl.h>

// RESOURCE INCLUDES
#include <peninputhwrfscreencn.rsg>
#include <peninputhwrfscn.mbg>
#include <peninputlayoutinputmodechoice.h>

// USER INCLUDES
#include "peninputhwrfscn.hrh"
#include "peninputhwrfscnlayout.h"
#include "peninputhwrfscnfunctionpanel.h"
#include "peninputhwrfscncontrolid.h"
#include "peninputhwrfscnevent.h"
#include "peninputhwrfscndatastore.h"
#include "peninputhwrfscnlafmanager.h"

#include "peninputhwrfscnbkgnd.h"

// CONSTANTS
//_LIT( KBMPFileName, "\\resource\\apps\\peninputhwrfscn.mbm" );
const TUint8 KSymbolTableColumnCount = 6;
const TUint8 KSymbolTableRowCount = 5;
const TInt KEnterWidthUintNum = 2;
const TInt KSpaceWidthUintNum = 2;
const TInt KWidthSwitcherBtnWidthUintNum = 2;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPeninputHwrfscnFunctionPanel::NewL()
// .
// -----------------------------------------------------------------------------
//
CPeninputHwrfscnFunctionPanel* CPeninputHwrfscnFunctionPanel::NewL( 
    CPeninputHwrfscnLayout* aLayout )
    {
    CPeninputHwrfscnFunctionPanel* self = 
               new ( ELeave ) CPeninputHwrfscnFunctionPanel( aLayout );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnFunctionPanel::CPeninputHwrfscnFunctionPanel()
// .
// -----------------------------------------------------------------------------
//
CPeninputHwrfscnFunctionPanel::CPeninputHwrfscnFunctionPanel( 
    CPeninputHwrfscnLayout* aLayout )
    :CPeninputHwrfscnBasePanel( aLayout )
    {
    // Initialze iPos
    iPos.SetXY( 0, 0 );

    // Initialize to be EFullWidth
    iLangWidthState = EFullWidth;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnFunctionPanel::~CPeninputHwrfscnFunctionPanel()
// .
// -----------------------------------------------------------------------------
//
CPeninputHwrfscnFunctionPanel::~CPeninputHwrfscnFunctionPanel()
    {
    delete iVkbLayout;  
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnFunctionPanel::Show()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnFunctionPanel::Show( TBool aVisible )
    {
	iVisible = aVisible;
	iSymbolTableGroup->Hide( !aVisible );
   	iMoreFuncBtnGroup->Hide( !aVisible );
	
	const RPointerArray<CFepUiBaseCtrl>& moreFuncBtnList = 
	    iMoreFuncBtnGroup->ControlList();

    for ( TInt i = 0; i < EMoreFuncBtnCount; i++ )
	    {
	    moreFuncBtnList[i]->Hide( !aVisible );
	    }
	    
   	iVkbCtrl->Hide( !aVisible );
    
    
    // Added for error when hide vkb calls for dimkey function
    if ( aVisible )
        {
        CAknFepCtrlCommonButton* butonFullHalf = static_cast<CAknFepCtrlCommonButton*>( 
        iMoreFuncBtnGroup->ControlById( EHwrCtrlIdLanguageWidthFullHalf ) );
        
        CAknFepCtrlCommonButton* butonHalfFull = static_cast<CAknFepCtrlCommonButton*>( 
        iMoreFuncBtnGroup->ControlById( EHwrCtrlIdLanguageWidthHalfFull ) );
        
            
        if ( iLayout->DataStore().PermittedRanges().Find( ERangeNative ) 
            == KErrNotFound )
            {
            iLangWidthState = EHalfWidth;

           	iVkbLayout->ChangeVkbLayout( EAknFepVkbSymbolHalfWidth );
            
            butonHalfFull->Hide(EFalse);
            butonFullHalf->Hide(ETrue);
            butonHalfFull->SetDimmed( ETrue );
            
            //butonFullHalf->SetHighlight( ETrue );
            
            //butonFullHalf->SetDimmed( ETrue );
            }
        else
            {
            if ( iLayout->DataStore().IsChinesePrimaryRange() )
                {
                iLangWidthState = EFullWidth;

               	iVkbLayout->ChangeVkbLayout( EAknFepVkbSymbolFullWidth );
                
                //butonFullHalf->SetHighlight( EFalse );
	            butonHalfFull->Hide(ETrue);
	            butonFullHalf->Hide(EFalse);
                }
            else
                {
                iLangWidthState = EHalfWidth;

               	iVkbLayout->ChangeVkbLayout( EAknFepVkbSymbolHalfWidth );
	            butonHalfFull->Hide(EFalse);
	            butonFullHalf->Hide(ETrue);                
                //butonFullHalf->SetHighlight( ETrue );
                }
            //butonFullHalf->SetDimmed( EFalse );
            butonHalfFull->SetDimmed( EFalse );
            }
            
        TVirtualKeyDrawInfo vkbBoardDrawInfo(KAknsIIDQsnFrItutButtonCandiSideL, 
									KAknsIIDQsnFrItutButtonCandiMiddle,
								    KAknsIIDQsnFrItutButtonCandiSideR,
						            KAknsIIDQsnFrItutButtonCandiSideL,
						            KAknsIIDQsnFrItutButtonCandiMiddle,
						            KAknsIIDQsnFrItutButtonCandiSideR,
                                    KAknsIIDQsnFrItutButtonCandiPressedSideL,
                                    KAknsIIDQsnFrItutButtonCandiPressedMiddle,
                                    KAknsIIDQsnFrItutButtonCandiPressedSideR,
						            TRgb(194, 221, 242),
						            ETrue);
        iVkbCtrl->SetVirtualKeyImgID(vkbBoardDrawInfo); 
            
        // reset rect and dim
		iVkbCtrl->SetRect( SymbolTableRect() );
		TRAP_IGNORE( NumberModeDimKeysL() );
		Draw();
        }
    // End
   
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnFunctionPanel::Draw()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnFunctionPanel::Draw()
    {
	iSymbolTableGroup->Draw();
    TRect rect = iSymbolTableGroup->Rect();
    iLayout->UpdateArea( iSymbolTableGroup, rect, EFalse,EFalse );

    iMoreFuncBtnGroup->Draw();
    rect = iMoreFuncBtnGroup->Rect();
    iLayout->UpdateArea( iMoreFuncBtnGroup, rect, EFalse,EFalse );
    }
// -----------------------------------------------------------------------------
// CPeninputHwrfscnFunctionPanel::HandleControlEvent()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnFunctionPanel::HandleControlEvent( 
    TInt aEventType, 
    CFepUiBaseCtrl* aCtrl, 
	const TDesC& aEventData )
    {
    switch( aEventType )
        {
        case EEventButtonUp:
            {
            OnControlEvent_ButtonUp( aEventType, aCtrl, aEventData );
	        }
            break;

        case EEventVirtualKeyUp:
			{
            Show( EFalse );
            const RPointerArray<CFepUiBaseCtrl>& FsFuncBtnList = 
                iMoreFuncBtnGroup->ControlList();            
            for (TInt i = 0; i < EMoreFuncBtnCount; i++)
                {
                FsFuncBtnList[i]->UpdateAreaImmed(FsFuncBtnList[i]->Rect(),
                        EFalse);
                }            
            ReportEvent( aEventType, aCtrl, aEventData );
    		}
            break;

        default:
        	break;
	    }
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnFunctionPanel::HandleCommand()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnFunctionPanel::HandleCommand( 
    TInt /*aCommand*/, TUint8* /*aData*/ )
    {
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnFunctionPanel::BeforePropertyChangedL()
// .
// -----------------------------------------------------------------------------
//
TBool CPeninputHwrfscnFunctionPanel::BeforePropertyChangedL( 
    MPeninputHwrfscnPropertySubscriber::TPeninputHwrfscnProperty /*aProptery*/ )
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnFunctionPanel::PropertyChangedL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnFunctionPanel::PropertyChangedL( 
	MPeninputHwrfscnPropertySubscriber::TPeninputHwrfscnProperty /*aProptery*/ )
    {
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnFunctionPanel::ConstructL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnFunctionPanel::ConstructL()
    {
    iLayout->DataStore().AddSubscriber( this );
    
    ReadLAFInfo();
    
    CreateSymbolTableL();

    CreateMoreFuncBtnListL();
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnFunctionPanel::SizeChanged()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnFunctionPanel::SizeChanged()
    {
    ReadLAFInfo();
    
	iMoreFuncBtnGroup->SetRect( MoreFuncBtnGroupRect() );
    iSymbolTableGroup->SetRect( SymbolTableRect() );

    ResetMoreFuncBtnsSize();
	ResetSymbolTableSize();
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnFunctionPanel::LafChanged()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnFunctionPanel::LafChanged()
    {
    ReadLAFInfo();
    ResetSymbolTableSize();
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnFunctionPanel::CreateSymbolTableL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnFunctionPanel::CreateSymbolTableL()
	{
    const CFont* font = PeninputHwrfscnLAFManager::SymbolTableFont();

    iVkbCtrl = CPeninputVkbCtrlExt::NewL( SymbolTableRect(), 
        iLayout, 
        EHwrCtrlIdSymbolTableVkb,
        font );
        
    //This modification is for EVWG-7VW2Z7, change another smaller layout font 
    TAknTextLineLayout keypadText =
		//AknLayoutScalable_Avkon::cell_hwr_fs_pane_t1().LayoutLine();  
        AknLayoutScalable_Avkon::cell_vkb_candidate_pane_t1().LayoutLine();
    //Modification for EVWG-7VW2Z7 end  
    iVkbCtrl->SetTextLineLayout(keypadText);
	iVkbCtrl->SetKeyTextColorGroup( EAknsCIQsnTextColorsCG60 );
	iVkbLayout = CAknFepCtrlVkbLayout::NewL();
    // Add VKB control to the VKB layout
    iVkbLayout->SetVkbKeyboard( iVkbCtrl );

    // Construct VKB layout from resource
    iVkbLayout->SetResourceId( R_AKN_FEP_VKB_VKB_LAYOUT_LIST );
    iVkbLayout->ConstructFromResourceL();
	
	iVkbCtrl->SetIrregularKeyBoard(EFalse);
	iVkbLayout->SetNonIrregularResourceId(R_AKN_FEP_VKB_NONIRREGULAR_BITMAP);
	iVkbLayout->ConstructFromNonIrregularResourceL();
    // Change the current VKB layout
    iVkbLayout->ChangeVkbLayout( EAknFepVkbSymbolFullWidth );
    
	TVirtualKeyDrawInfo vkbBoardDrawInfo(KAknsIIDQsnFrItutButtonCandiSideL, 
    									KAknsIIDQsnFrItutButtonCandiMiddle,
    								    KAknsIIDQsnFrItutButtonCandiSideR,
    						            KAknsIIDQsnFrItutButtonCandiSideL,
    						            KAknsIIDQsnFrItutButtonCandiMiddle,
    						            KAknsIIDQsnFrItutButtonCandiSideR,
                                        KAknsIIDQsnFrItutButtonCandiPressedSideL,
                                        KAknsIIDQsnFrItutButtonCandiPressedMiddle,
                                        KAknsIIDQsnFrItutButtonCandiPressedSideR,
    						            TRgb(194, 221, 242),
    						            ETrue);
    iVkbCtrl->SetVirtualKeyImgID(vkbBoardDrawInfo);

    ResetSymbolTableSize();
    iVkbCtrl->AddEventObserver( this );

    // Create a control group, and put vkb in it, to avoid the problem that, 
    // Root control will add all vkbs on the top of the control list.
    iSymbolTableGroup = CControlGroup::NewL( iLayout, 
        EHwrCtrlIdSymbolTableVkbGroup );
    AddToRootControlL( iSymbolTableGroup );

   	iSymbolTableGroup->AddControlL( iVkbCtrl );
    iSymbolTableGroup->SetActiveCtrl( iVkbCtrl );
	}

// -----------------------------------------------------------------------------
// CPeninputHwrfscnFunctionPanel::CreateMoreFuncBtnListL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnFunctionPanel::CreateMoreFuncBtnListL()
	{
    // Create container
    iMoreFuncBtnGroup = CPeninputHwrfscnBkgnd::NewL( iLayout, EHwrCtrlIdMoreFuncGroup );
    iMoreFuncBtnGroup->SetResourceId( R_PENINPUT_LAYOUT_WINDOW_BACKGROUND );
    iMoreFuncBtnGroup->ConstructFromResourceL();    
    
    AddToRootControlL( iMoreFuncBtnGroup );

    // Add btns to the container
    // Since the language width switch btn is a latched btn which diff with other btns, 
    // it should be created separately
    /*AddBtnToMoreFuncGrpL( WidthSwitcherBtnRect(),
        EHwrCtrlIdLanguageWidthFullHalf,
        EMbmPeninputhwrfscnQgn_indi_fep_button_full_half_width,
        EMbmPeninputhwrfscnQgn_indi_fep_button_full_half_width_mask,
        EMbmPeninputhwrfscnQgn_indi_fep_button_half_full_width,
        EMbmPeninputhwrfscnQgn_indi_fep_button_half_full_width_mask );*/

    CAknFepCtrlCommonButton* langFullHalfWidthBtn = 
        CAknFepCtrlCommonButton::NewL( iLayout, EHwrCtrlIdLanguageWidthFullHalf,
            								  KAknsIIDQsnFrFunctionButtonNormal,
        									  KAknsIIDQsnFrFunctionButtonPressed,
        									  KAknsIIDQsnFrFunctionButtonInactive );

	TResourceReader reader;
    
    CCoeEnv::Static()->CreateResourceReaderLC( reader,
    	 			   R_AKN_FEP_HWR_SCT_PAGE_1 );  
    langFullHalfWidthBtn->SetResourceId( R_AKN_FEP_HWR_SCT_PAGE_1 );      
    langFullHalfWidthBtn->ConstructFromResourceL(); 
    
    // Pop and destroy reader
    CleanupStack::PopAndDestroy( 1 );

    langFullHalfWidthBtn->AddEventObserver( this );
	iMoreFuncBtnGroup->AddControlL( langFullHalfWidthBtn );



    CAknFepCtrlCommonButton* langHalfFullWidthBtn = 
        CAknFepCtrlCommonButton::NewL( iLayout, EHwrCtrlIdLanguageWidthHalfFull,
            								  KAknsIIDQsnFrFunctionButtonNormal,
        									  KAknsIIDQsnFrFunctionButtonPressed,
        									  KAknsIIDQsnFrFunctionButtonInactive );

    
    CCoeEnv::Static()->CreateResourceReaderLC( reader,
    	 			   R_AKN_FEP_HWR_SCT_PAGE_2 );  
    langHalfFullWidthBtn->SetResourceId( R_AKN_FEP_HWR_SCT_PAGE_2 );      
    langHalfFullWidthBtn->ConstructFromResourceL(); 
    
    // Pop and destroy reader
    CleanupStack::PopAndDestroy( 1 );

    langHalfFullWidthBtn->AddEventObserver( this );
	iMoreFuncBtnGroup->AddControlL( langHalfFullWidthBtn );




   	AddBtnToMoreFuncGrpL( SpaceBtnRect(), 
   	    EHwrCtrlIdSpace, 
   	    R_AKN_FEP_HWR_SPACE );
	// Space button          
    CAknFepCtrlEventButton* spaceButton = 
    	static_cast<CAknFepCtrlEventButton*>(iMoreFuncBtnGroup->ControlById(EHwrCtrlIdSpace));
    // Create 3-pieces color icon for space button
	CPenInput3PiecesColorIcons* colorIcons = 
				CPenInput3PiecesColorIcons::NewL( R_AKN_FEP_VKB_SPACE_ICON1,
												  R_AKN_FEP_VKB_SPACE_ICON2,
												  R_AKN_FEP_VKB_SPACE_ICON3 );
	spaceButton->Set3PiecesColorIcons( colorIcons );

	
   	AddBtnToMoreFuncGrpL( EnterBtnRect(), 
   	    EHwrCtrlIdEnter, 
   	    R_AKN_FEP_HWR_RETURN );
	}

// -----------------------------------------------------------------------------
// CPeninputHwrfscnFunctionPanel::AddBtnToMoreFuncGrpL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnFunctionPanel::AddBtnToMoreFuncGrpL( 
    const TRect aRect, 
    TInt aCtrlId, 
    TInt aResID )
		{
		CAknFepCtrlEventButton* moreFuncBtn = 
		  CAknFepCtrlEventButton::NewL( iLayout, aCtrlId,
		  									0xFFFF,
											  0,
		      								  KAknsIIDQsnFrFunctionButtonNormal,
		  									  KAknsIIDQsnFrFunctionButtonPressed,
		  									  KAknsIIDQsnFrFunctionButtonInactive );
		moreFuncBtn->SetResourceId( aResID );
		moreFuncBtn->ConstructFromResourceL();
		TRect innerRect;
		AknPenImageUtils::CalculateGraphicRect( aRect, innerRect );   	
		moreFuncBtn->SizeChanged( aRect, innerRect, ETrue );
		moreFuncBtn->SetRect( aRect );
		moreFuncBtn->AddEventObserver( this );
		
		
		iMoreFuncBtnGroup->AddControlL( moreFuncBtn );
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnFunctionPanel::OnControlEvent_ButtonUp()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnFunctionPanel::OnControlEvent_ButtonUp( 
    TInt aEventType, 
    CFepUiBaseCtrl* aCtrl, 
	const TDesC& aEventData )
    {
    if( aCtrl->ControlId() == EHwrCtrlIdLanguageWidthFullHalf 
      || aCtrl->ControlId() == EHwrCtrlIdLanguageWidthHalfFull)
        {
        if( iLayout->DataStore().PermittedRanges().Find( ERangeNative ) != 
            KErrNotFound )
            {
            // Switch language width state
            iLangWidthState == EFullWidth? iLangWidthState = EHalfWidth:
                                           iLangWidthState = EFullWidth;
            
            // Show VKB layout corresponding to current language width state
            iLangWidthState == EFullWidth ? 
            	iVkbLayout->ChangeVkbLayout( EAknFepVkbSymbolFullWidth ) : 
            	iVkbLayout->ChangeVkbLayout( EAknFepVkbSymbolHalfWidth );
            
            TVirtualKeyDrawInfo vkbBoardDrawInfo(KAknsIIDQsnFrItutButtonCandiSideL, 
    									KAknsIIDQsnFrItutButtonCandiMiddle,
    								    KAknsIIDQsnFrItutButtonCandiSideR,
    						            KAknsIIDQsnFrItutButtonCandiSideL,
    						            KAknsIIDQsnFrItutButtonCandiMiddle,
    						            KAknsIIDQsnFrItutButtonCandiSideR,
                                        KAknsIIDQsnFrItutButtonCandiPressedSideL,
                                        KAknsIIDQsnFrItutButtonCandiPressedMiddle,
                                        KAknsIIDQsnFrItutButtonCandiPressedSideR,
    						            TRgb(194, 221, 242),
    						            ETrue);
            iVkbCtrl->SetVirtualKeyImgID(vkbBoardDrawInfo);
            
	        CAknFepCtrlCommonButton* butonFullHalf = static_cast<CAknFepCtrlCommonButton*>( 
	        iMoreFuncBtnGroup->ControlById( EHwrCtrlIdLanguageWidthFullHalf ) );
	        
	        CAknFepCtrlCommonButton* butonHalfFull = static_cast<CAknFepCtrlCommonButton*>( 
	        iMoreFuncBtnGroup->ControlById( EHwrCtrlIdLanguageWidthHalfFull ) );  
	        
            iLangWidthState == EFullWidth? butonFullHalf->Hide(EFalse):
                                           butonHalfFull->Hide(EFalse);
                                           
            iLangWidthState == EFullWidth? butonHalfFull->Hide(ETrue):
                                           butonFullHalf->Hide(ETrue);
            
            // reset rect and dim
	        iVkbCtrl->SetRect( SymbolTableRect() );
		    TRAP_IGNORE( NumberModeDimKeysL() );
		    iLayout->LayoutOwner()->UpdateArea( SymbolTableRect(), EFalse, EFalse );
            }
        }
    /*
    else if( aCtrl->ControlId() == EHwrCtrlIdVkbSwitcher )
        {
        TRect rect = aCtrl->Rect();   
        TRAP_IGNORE( iInputModeSwitch->PopUpSwitchListL( rect ) );        
        }
    */
    else
        {
        Show( EFalse );
        ReportEvent( aEventType, aCtrl, aEventData );
        }
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnFunctionPanel::NumberModeDimKeysL()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnFunctionPanel::NumberModeDimKeysL()
    {
   	iVkbCtrl->CancelDims();

    if( iLayout->DataStore().PermittedRanges().Count() == 1
        && iLayout->DataStore().PermittedRanges()[0] == ERangeNumber )
        {
        HBufC* rs = iLayout->DataStore().KeyMappingStringL();
        iVkbCtrl->DimKeys( rs );
        delete rs;
        rs = NULL;
        }

    iVkbCtrl->Draw();
    }


// -----------------------------------------------------------------------------
// CPeninputHwrfscnFunctionPanel::ReadLAFInfo()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnFunctionPanel::ReadLAFInfo()
    {
	TRect rect( PeninputHwrfscnLAFManager::ControlBarUnitRect() );
	iUnitWidth = rect.Width();
	iUnitHeight = rect.Height();
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnFunctionPanel::ResetMoreFuncBtnsSize()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnFunctionPanel::ResetMoreFuncBtnsSize()
	{
	const CFont* font = PeninputHwrfscnLAFManager::SymbolTableFont();
	
	RPointerArray<CFepUiBaseCtrl> moreFuncBtnList = 
	    iMoreFuncBtnGroup->ControlList();

    for ( int i = 0; i < EMoreFuncBtnCount; i++ )
	    {
	    CAknFepCtrlCommonButton* button = 
	        static_cast<CAknFepCtrlCommonButton*>( moreFuncBtnList[i] );
	    
        switch( button->ControlId() )
            {
            case EHwrCtrlIdLanguageWidthFullHalf:
            case EHwrCtrlIdLanguageWidthHalfFull:
                {
        	    TRect rect( WidthSwitcherBtnRect() );
				TRect innerRect;
				AknPenImageUtils::CalculateLongBtnGraphicRect( rect, innerRect );
				button->SizeChanged( rect, innerRect, ETrue );
        	    button->SetRect( rect );
    	        }
                break;

            case EHwrCtrlIdSpace:
    			{
        	    TRect rect( SpaceBtnRect() );
				TRect innerRect;
				innerRect = rect;
				innerRect.Shrink(10, 0);
        	    button->SizeChanged( rect, innerRect, ETrue );
			    TRect spaceIconRect = innerRect;
			    spaceIconRect.Shrink(10, 0);
				button->SetDraw3PieceFrameInfo(TDraw3PiecesFrame(KAknsIIDQgnIndiInputSpaceL,
																  KAknsIIDQgnIndiInputSpaceMiddle,
																  KAknsIIDQgnIndiInputSpaceR,
																  spaceIconRect));         	    
        	    button->SetRect( rect );
        	    button->SetFont( font );            
        		}
                break;
            
            case EHwrCtrlIdEnter:
                {
        	    TRect rect( EnterBtnRect() );
				TRect innerRect;
				AknPenImageUtils::CalculateGraphicRect( rect, innerRect );   	    
        	    button->SizeChanged( rect, innerRect, ETrue );
        	    button->SetRect( rect );
        	    button->SetFont( font );            
                }
                break;
            default:
            	break;
    	    }
	    }
	
	// Bitmap should be resized by the first time construct
	TSize bitMapSize( iUnitWidth, iUnitHeight );
	AknIconUtils::SetSize( iSvgLatched, bitMapSize, EAspectRatioNotPreserved );
	AknIconUtils::SetSize( iSvgUnLatched, bitMapSize, EAspectRatioNotPreserved );
    
	}

// -----------------------------------------------------------------------------
// CPeninputHwrfscnFunctionPanel::ResetSymbolTableSize()
// .
// -----------------------------------------------------------------------------
//
void CPeninputHwrfscnFunctionPanel::ResetSymbolTableSize()
    {
    const CFont* font = PeninputHwrfscnLAFManager::SymbolTableFont();
    
    TPoint charLeftTop( 0, 0 );
    
    TSize size( PeninputHwrfscnLAFManager::GetSymbolTableCharSizeOffset() );
    
    TInt unitCharWidth = iUnitWidth - 2*size.iWidth;
    TInt unitCharHeight = iUnitHeight - 2*size.iHeight;

    // Get all VKB layout information and resize them
    RPointerArray<CPeninputVkbLayoutInfo> vkbList = 
        iVkbLayout->VkbLayoutInfoList();
    
    TInt count = vkbList.Count();
    
    for ( TInt i = 0; i < count; i++ )
        {
        vkbList[i]->ResetSize( iUnitWidth, 
                               iUnitHeight, 
                               unitCharWidth, 
                               unitCharHeight, 
                               charLeftTop, 
                               font );
        }
	
  	TSize curSize = TSize( iUnitWidth, iUnitHeight );
    size = iVkbCtrl->NonIrregularKeyBitmap(EKeyBmpNormal)->SizeInPixels();
	if( curSize != size )	
		{
        for ( TInt i = 0; i <= EKeyBmpLastType; i++ )
            {
            if( iVkbCtrl->NonIrregularKeyBitmap((TVirtualKeyBmpType)i) )
            	{
                AknIconUtils::SetSize( iVkbCtrl->NonIrregularKeyBitmap((TVirtualKeyBmpType)i), 
                                       curSize, EAspectRatioNotPreserved );                    		
            	}
            }
		}	
	iVkbCtrl->SetRect( SymbolTableRect() );
    iVkbCtrl->SetFont( font );
    
    //This modification is for EVWG-7VW2Z7, change another smaller layout font 
    TAknTextLineLayout keypadText =
		//AknLayoutScalable_Avkon::cell_hwr_candidate_pane_t1().LayoutLine();
        AknLayoutScalable_Avkon::cell_vkb_candidate_pane_t1().LayoutLine();
    //Modification for EVWG-7VW2Z7 end
    iVkbCtrl->SetTextLineLayout(keypadText);
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnFunctionPanel::MoreFuncBtnGroupRect()
// .
// -----------------------------------------------------------------------------
//
TRect CPeninputHwrfscnFunctionPanel::MoreFuncBtnGroupRect()
	{
	TSize size( KSymbolTableColumnCount * iUnitWidth, iUnitHeight );
	
	return TRect( iPos, size );
	}

// -----------------------------------------------------------------------------
// CPeninputHwrfscnFunctionPanel::WidthSwitcherBtnRect()
// .
// -----------------------------------------------------------------------------
//
TRect CPeninputHwrfscnFunctionPanel::WidthSwitcherBtnRect( )
	{
	TPoint pos( iPos );
	
	TSize size( iUnitWidth * KWidthSwitcherBtnWidthUintNum, iUnitHeight );
	
	return TRect ( pos, size );
	}

// -----------------------------------------------------------------------------
// CPeninputHwrfscnFunctionPanel::SpaceBtnRect()
// .
// -----------------------------------------------------------------------------
//
TRect CPeninputHwrfscnFunctionPanel::SpaceBtnRect( )
	{
	TRect funcRect(WidthSwitcherBtnRect());
	
	TPoint pos( funcRect.iBr.iX, funcRect.iTl.iY );
	
	TSize size( iUnitWidth * KSpaceWidthUintNum, iUnitHeight );
	
	return TRect ( pos, size );
	}

// -----------------------------------------------------------------------------
// CPeninputHwrfscnFunctionPanel::EnterBtnRect()
// .
// -----------------------------------------------------------------------------
//
TRect CPeninputHwrfscnFunctionPanel::EnterBtnRect( )
	{
	TRect funcRect( SpaceBtnRect() );
	
	TPoint pos( funcRect.iBr.iX, funcRect.iTl.iY );
	
	TSize size( iUnitWidth * KEnterWidthUintNum, iUnitHeight );
	
	return TRect ( pos, size );
	}
		

// -----------------------------------------------------------------------------
// CPeninputHwrfscnFunctionPanel::SymbolTableRect()
// .
// -----------------------------------------------------------------------------
// 
TRect CPeninputHwrfscnFunctionPanel::SymbolTableRect()
	{
    TSize size( KSymbolTableColumnCount * iUnitWidth,
        KSymbolTableRowCount * iUnitHeight );
    
    return TRect( TPoint( iPos.iX, iPos.iY+iUnitHeight ), size );
	}
	
// -----------------------------------------------------------------------------
// CPeninputHwrfscnFunctionPanel::HandleSkinChanged()
// .
// -----------------------------------------------------------------------------
// 
void CPeninputHwrfscnFunctionPanel::HandleSkinChanged()
	{
	TRAP_IGNORE(iVkbLayout->ConstructFromNonIrregularResourceL());
	ResetSymbolTableSize();
	TInt newValue = iLayout->DataStore().GetPenTrailColor();
	iLayout->DataStore().SetBoxPenColor(TRgb(newValue));
	
	}

//End of file
