/*
* Copyright (c) 2009-2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Implementation of main window of finger hwr.
*
*/

// INCLUDE
#include <coemain.h>
#include <s32mem.h>

#include <aknlayoutscalable_apps.cdl.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <AknLayoutDef.h>
#include <AknUtils.h>
#include <AknsUtils.h>
#include <AknIconUtils.h>
#include <AknFepGlobalEnums.h>
#include <aknfeppeninputenums.h>

#include <peninputfingerhwrarwnd.rsg>
#include <peninputlayoutmultilineicf.h>
#include <peninputdropdownlist.h>
#include <peninputrepeatbutton.h>
#include <peninputlayoutbubblectrl.h>

// USER INCLUDES
#include "peninputfingerhwrarwnd.h"
#include "peninputfingerhwrarlafmanager.h"
#include "peninputfingerhwrarevent.h"
#include "peninputfingerhwrarcontrolid.h"
#include "peninputfingerhwrarstoreconstants.h"
#include "peninputfingerhwrarmultipagevkb.h"
#include "peninputfingerhwrarlayout.h"
#include "peninputfingerhwrardatastore.h"
#include "peninputfingerhwrarsymboltable.h"
#include "peninputfingerhwrarnumsymboltable.h"
#include "peninputfingerhwrarindicator.h"
#include "peninputfingerhwrarwritingwnd.h"

const TUint32 KDefaultTextColor          = 0x000000;
const TUint   KDefaultFrameColor         = 0x000000;
const TUint32 KDefaultWriteBoxBgColor    = 0xdddddd;
const TUint32 KDefaultWriteBoxFrameColor = 0x000000;

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrArWnd* CPeninputFingerHwrArWnd::NewL( CFepUiLayout* aFepUiLayout,
    TInt aControlId, TBool aLandscapeStyle )
    {
    CPeninputFingerHwrArWnd* self = NewLC( aFepUiLayout, aControlId, aLandscapeStyle );
    CleanupStack::Pop( self );

    return self;
    }

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrArWnd* CPeninputFingerHwrArWnd::NewLC( CFepUiLayout* aFepUiLayout, 
    TInt aControlId, TBool aLandscapeStyle )
    {
    CPeninputFingerHwrArWnd* self = new ( ELeave ) CPeninputFingerHwrArWnd( aFepUiLayout, aControlId );
    CleanupStack::PushL( self );
    self->ConstructL( aLandscapeStyle );

    return self;
    }

// ---------------------------------------------------------------------------
// c++ destructor
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrArWnd::~CPeninputFingerHwrArWnd()
    {
    #ifdef RD_TACTILE_FEEDBACK
    if(UiLayout() && iWritingBox)
        {
		UiLayout()->DeRegisterFeedbackArea(reinterpret_cast<TInt>(iWritingBox),
	                                 iWritingBox->Rect());
		}
	#endif								 
    delete iLafManager;
    }

// ---------------------------------------------------------------------------
// Construct control from resource.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::ConstructFromResourceL()
    {
    if ( iResourceId == KInvalidResId )
        {
        User::Leave( KErrArgument );
        }

    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC( reader, iResourceId );

    TInt supportskin = reader.ReadInt16();
    
    if ( iFirstTimeConstruct && ( supportskin != KNotSupportSkin ) )
        {
        UiLayout()->AddControlL( this );
        }
    
    // read shadow resources
    TInt32 shadow = reader.ReadInt32();
    ReadShadowInfoL( shadow );
    
    // read icf resource
    TInt32 icf = reader.ReadInt32();
    ReadICFInfoL( icf );
     

    if ( iFirstTimeConstruct )
        {
        const TInt32 boxwndResId = reader.ReadInt32();
        const TInt32 bgImageResId = reader.ReadInt32();
  
        ReadWritingBoxInfoL( boxwndResId );
        ReadBackgroundInfoL( bgImageResId );
        }
    else
        {
        CControlGroup::ConstructFromResourceL();

        const TInt32 boxwndResId = reader.ReadInt32();
        const TInt32 bgImageResId = reader.ReadInt32();
        ReadWritingBoxInfoL( boxwndResId );
        ReadBackgroundInfoL( bgImageResId );
        }    

    CleanupStack::PopAndDestroy(); // reader

    // read keypad image info
    if ( iNumSymbolTable )
        {
        TSize keysize = iLafManager->VirtualNumpadCellSize().Size();
        iNumSymbolTable->LoadVkbKeyImageL(R_FINGER_HWR_NUMPAD_IMAGE, keysize );
        
        RArray<TRect> rects;
        CleanupClosePushL( rects );
        iLafManager->GetNumVirtualKeyRects( rects );
        LoadNumSymbolVirtualKeysL( R_FINGER_HWR_NUMPAD, rects );
        CleanupStack::PopAndDestroy(); //rects
        }
    
    if ( iSymbolTable )
        {
        TSize keysize = iLafManager->VirtualSctpadCellSize().Size();
        iSymbolTable->LoadVkbKeyImageL(R_FINGER_HWR_SCTPAD_IMAGE, keysize );
        
        RArray<TRect> rects;
        CleanupClosePushL( rects );
        iLafManager->GetVirtualKeyRects( rects );
        if(iLafManager->IsLandscape())
            {
            LoadSymbolVirtualKeysL(R_ARABIC_FINGER_HWR_LANDSCAPE_SYMBOL_TABLE, rects);
            }
        else
            {
            LoadSymbolVirtualKeysL(R_ARABIC_FINGER_HWR_PORTRAIT_SYMBOL_TABLE, rects);
            }        
        CleanupStack::PopAndDestroy(); //rects
        }
    
    if (!iFirstTimeConstruct)
    	{
    	Draw();
    	}
    
    iFirstTimeConstruct = EFalse;  
    }


// ---------------------------------------------------------------------------
// Draw window.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::Draw()
    {
    if(AbleToDraw())
        {
		DrawOpaqueMaskBackground( Rect() );

		if( BackgroundBmp() && BackgroundBmp()->SizeInPixels() != Rect().Size() )
			{
			AknIconUtils::SetSize(BackgroundBmp(), Rect().Size(), EAspectRatioNotPreserved);
			}

		DrawBackground();    
			 
		CControlGroup::Draw();
		
		// Update whole area
		UpdateArea( Rect(), EFalse );
		}
    }

// ---------------------------------------------------------------------------
// Resize.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::SizeChangedL()
    {
    iLafManager->ReadLafInfo();
    ResetLayoutL();
  
            
    iCandidateList->SizeChanged( iLafManager->CandidateUnitWidth(),
            iLafManager->CandidateUnitHeight(), 
            iLafManager->CandidateUnitWidth(), 
            iLafManager->CandidateLTPos(), 
            3/*KCandidateCountPerRow*/,
            2 );
    iContextField->SetTextMargin ( iLafManager->IcfLeftMargin(),
            iLafManager->IcfRightMargin(),
            iLafManager->IcfTopMargin(),
            iLafManager->IcfBottomMargin() );
    
    iContextField->SetLineSpace( iLafManager->IcfLineSpaceMargin() );                          
    iContextField->SizeChangedL( iLafManager->IcfRect(), 
            iLafManager->IcfTextHeight(),
            iLafManager->IcfFont()->FontMaxHeight(),
            iLafManager->IcfFont() );
    
    CalculateGuideLinePos();
    Draw();
}

// ---------------------------------------------------------------------------
// Handle window open
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::HandleWindowOpenCommandL()
    {
    CalculateGuideLinePos();
    }

// ---------------------------------------------------------------------------
// accept editor's text alignment.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::SetTextAlignmentL( TInt aAlignment, TInt aLangId )  
    {
    if ( iContextField )
        {
        iContextField->SetTextAlignmentL( aAlignment, aLangId );
        }
    }

// ---------------------------------------------------------------------------
// accept editor's text
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::SetEditorTextL( const TFepInputContextFieldData& aData )
    {
    if ( iContextField )
        {
        iContextField->SetTextL( aData );
        }
    }

// ---------------------------------------------------------------------------
// accept editor's promopt txt.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::SetPromptTextL( TUint8* aPromoptData )
    {
    RDesReadStream readStream;

    TPtr8 countPtr(aPromoptData, 2*sizeof(TInt), 2*sizeof(TInt));
    readStream.Open(countPtr);
    CleanupClosePushL(readStream);
    const TInt dataCount = readStream.ReadInt32L();
    const TInt textCount = readStream.ReadInt32L();
    CleanupStack::PopAndDestroy(&readStream);
    
    TPtr8 ptr( aPromoptData+2*sizeof(TInt), dataCount+textCount, dataCount+textCount );            
    readStream.Open(ptr);
    CleanupClosePushL(readStream);
    
    HBufC8* dataBuf = HBufC8::NewLC(dataCount);
    TPtr8 dataBufPtr = dataBuf->Des();
    readStream.ReadL(dataBufPtr, dataCount);

    TFepPromptText* pIcfData = 
    reinterpret_cast<TFepPromptText*>(const_cast<TUint8*>(dataBufPtr.Ptr()));

    HBufC* textBuf;
    if (textCount > 0)
        {
        textBuf = HBufC::NewLC(textCount/2);
        TPtr textBufPtr = textBuf->Des();
        readStream.ReadL(textBufPtr, textCount/2);
         
        const HBufC* icfPromptText = iContextField->PromptText();
        
        if (!icfPromptText || icfPromptText->Compare(textBuf->Des()) != 0 )
            {
            iContextField->SetPromptTextL( textBuf->Des(), pIcfData->iCleanContent );    
            }
        
        CleanupStack::PopAndDestroy(textBuf);
        }
    else
        {
        iContextField->SetPromptTextL(KNullDesC, pIcfData->iCleanContent);
        }

    CleanupStack::PopAndDestroy(dataBuf);
    CleanupStack::PopAndDestroy(&readStream);      
    }
 
// ---------------------------------------------------------------------------
// Dim option button
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::SetEnableSettingBtn(const TBool aEnable)
    {
    if ( aEnable )    
        {
        iOptionBtn->SetDimmed(EFalse);
        }
    else
        {
        iOptionBtn->SetDimmed(ETrue);
        iOptionBtn->SetActive(EFalse);
        }
    
    }

// ---------------------------------------------------------------------------
// accept editor's bubble text.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::SetBubbleTextL( const TDesC& aInfo )
    {
    if ( aInfo.Length() > 0 )
        {
        TRect rect(iLafManager->IndicatorBubbleRect());
        iContextField->MsgBubbleCtrl()->SetRect(rect);
//        iContextField->ShowBubble( aInfo, iContextField->MsgBubbleCtrl()->Rect());
        iContextField->ShowBubble( aInfo, rect);
        }
    else
        {
        iContextField->HideBubble();
        }    
    }

// ---------------------------------------------------------------------------
// get strokes from hwr writing box.
// ---------------------------------------------------------------------------
//
const RArray<TPoint>& CPeninputFingerHwrArWnd::StrokeList()
    {
    return iWritingBox->StrokeList();
    }

// ---------------------------------------------------------------------------
// set end mark of hwr
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::SetStrokeEndMark( const TPoint& aEndMark )
    {
    iWritingBox->SetStrokeEndMark( aEndMark );
    }

// ---------------------------------------------------------------------------
// set guide line on or off.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::SetGuideLineOn(const TBool aGuideLineOn)
    {
    iGuideLineOn = aGuideLineOn;
	DrawGuideLine();
    }

// ---------------------------------------------------------------------------
// set pen color of hwr writing.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::SetBoxPenColor( const TInt aPenColor )
    {
    iWritingBox->SetPenColor( aPenColor );
    }
    
// ---------------------------------------------------------------------------
// set pen size of hwr writing.
// ---------------------------------------------------------------------------
//  
void CPeninputFingerHwrArWnd::SetBoxPenSize( const TSize aPenSize )
    {
    iWritingBox->SetPenSize( aPenSize );
    }

// ---------------------------------------------------------------------------
// set writing speed hwr writing.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::SetBoxWritingSpeed( TTimeIntervalMicroSeconds32& aCharDelay )
    {
    iWritingBox->SetCharacterDelay( aCharDelay );
    TInt strokeDelay = aCharDelay.Int()/5;
    TTimeIntervalMicroSeconds32 sDelay(strokeDelay);
    iWritingBox->SetStrokeDelay( sDelay);   
    }

// ---------------------------------------------------------------------------
// retrieve rect of hwr writingbox.
// ---------------------------------------------------------------------------
//
TRect CPeninputFingerHwrArWnd::WritingBoxRect()
    {
    return iWritingBox->Rect();
    }


// ---------------------------------------------------------------------------
// Cancel wrting
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::CancelWriting()
    {
    iWritingBox->CancelCharWriting();
    }

// ---------------------------------------------------------------------------
// Change SCT page.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::ShowSymbolPage( TInt aPageNo, TInt aPos )
    {
    iSymbolTable->NavigatePage(aPageNo,aPos);
    }

// ---------------------------------------------------------------------------
// retrieve the ICF control.
// ---------------------------------------------------------------------------
//
CFepLayoutMultiLineIcf* CPeninputFingerHwrArWnd::Icf()
    {
    return iContextField;
    }


// ---------------------------------------------------------------------------
// retrieve the candicate list control.
// ---------------------------------------------------------------------------
//
CFepCtrlDropdownList* CPeninputFingerHwrArWnd::CandidateList()
    {
    return iCandidateList;
    }

// ---------------------------------------------------------------------------
// Open the candidate list
// ---------------------------------------------------------------------------
//	
void CPeninputFingerHwrArWnd::OpenCandidateList()
    {
    CPeninputFingerHwrArLayout* hwrLayout = NULL;
    hwrLayout = static_cast<CPeninputFingerHwrArLayout*>( UiLayout() );
    CPeninputFingerHwrArDataStore& datastore = hwrLayout->DataStore();
    
    const RPointerArray<HBufC>& candidates = datastore.Candidate();
    
    if(candidates.Count() > 0)
        {
        CFepCtrlDropdownList::TListType unexpandable = CFepCtrlDropdownList::EListExpandableMultiRowWithoutIconWithBubble;
    
        iCandidateList->Hide( EFalse );
        TRAP_IGNORE(iCandidateList->SetCandidatesL( candidates, unexpandable ));
        iCandidateList->SetHighlightCell( 0, datastore.Highlight() ); 		   
        iArrowLeftBtn->Hide(ETrue);
        iArrowRightBtn->Hide(ETrue);
        iSpaceBtn->Hide(ETrue);
        iEnterBtn->Hide(ETrue);
        }	
	}

// ---------------------------------------------------------------------------
// Close the candidate list
// ---------------------------------------------------------------------------
//	
void CPeninputFingerHwrArWnd::CloseCandidateList()
    {
	iCandidateList->ResetAndClear(CFepCtrlDropdownList::EListExpandableMultiRowWithoutIconWithBubble);
	// hide all the dropdown list in standby state
	iCandidateList->Hide( ETrue );
    iArrowLeftBtn->Hide(EFalse);
    iArrowRightBtn->Hide(EFalse);
    iSpaceBtn->Hide(EFalse);
    iEnterBtn->Hide(EFalse);
	}

// ---------------------------------------------------------------------------
// dim arrow buttons.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::DimArrowKeys( TBool aDimArrow )
   {
   iArrowLeftBtn->SetDimmed( aDimArrow );   
   iArrowRightBtn->SetDimmed( aDimArrow );   
   iSpaceBtn->SetDimmed( aDimArrow );   
   iEnterBtn->SetDimmed( aDimArrow ); 
   }

// ---------------------------------------------------------------------------
// c++ constructor
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrArWnd::CPeninputFingerHwrArWnd( 
    CFepUiLayout* aFepUiLayout, TInt aControlId )
    : CControlGroup( aFepUiLayout,aControlId ), iFirstTimeConstruct( ETrue )
    {
    }
    
// ---------------------------------------------------------------------------
// Symbian second-phase constructor
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::ConstructL( TBool /*aLandscapeStyle*/ )
    {
	BaseConstructL();    
    
    iLafManager = CPeninputFingerHwrArLafManager::NewL();
    iLafManager->ReadLafInfo();
	
    //crate writing 
    CreateWritingBoxL();
	
	//create the handwriting indicator
	iHandwritingIndicator = CPeninputArabicFingerHwrIndicator::NewL(UiLayout(),EHwrCtrlIdHandwringIndicator);
	iWritingBox->InstallIndicator(iHandwritingIndicator);
	iWritingBox->ShowIndicator(ETrue);
	
    //create icf 
    CreateContextFieldL();
    
    //create candidate lists
    CreateCandidateListL();
    
    //create virtual sct pad
    CreateSymbolTableL();
    
    //create number mode virtual sct pad
    CreateNumSymbolTableL();

    //create control buttons
    CreateButtonsL();   
    
	//set controls postion and extent
    SizeChangedL();
	
	#ifdef RD_TACTILE_FEEDBACK
	if (UiLayout()->SupportTactileFeedback())
		{
	    UiLayout()->RegisterFeedbackArea(reinterpret_cast<TInt>(iWritingBox),
	                                     iWritingBox->Rect(),
	                                     iWritingBox->TactileFeedbackType());
		}
    #endif
	
	//switch to standby view
    SwitchToStandbyView();
    }
    
// ---------------------------------------------------------------------------
// create icf.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::CreateContextFieldL()
    {    
    const CFont* icffont =iLafManager->IcfFont(); 
    
    iContextField = CFepLayoutMultiLineIcf::NewL( 
        iLafManager->IcfRect(), 
        UiLayout(), 
        EHwrCtrlIdInputContextField, 
        icffont->HeightInPixels(),
        icffont->FontMaxHeight(),
        icffont );

    iContextField->SetTextMargin ( iLafManager->IcfLeftMargin(),
                      iLafManager->IcfRightMargin(),
                      iLafManager->IcfTopMargin(),
                      iLafManager->IcfBottomMargin() );
                      
    iContextField->SetLineSpace( iLafManager->IcfLineSpaceMargin() );                         
 
    iContextField->SizeChangedL( iLafManager->IcfRect(), 
                      iLafManager->IcfTextHeight(),
                      iLafManager->IcfFont()->FontMaxHeight(),
                      iLafManager->IcfFont() );

    TAknWindowLineLayout bubbleLineLayout = AknLayoutScalable_Apps::popup_char_count_window().LayoutLine();
    TAknTextLineLayout bubbleTextLayout = AknLayoutScalable_Apps::popup_char_count_window_t1( 0 ).LayoutLine();
    
    iContextField->MsgBubbleCtrl()->SetTextFormat( bubbleTextLayout );
    iContextField->MsgBubbleCtrl()->SetTextColorIndex( EAknsCIQsnTextColorsCG67 );
    
    //Change the ID when ID into release
    iContextField->MsgBubbleCtrl()->SetBitmapParam( NULL,
                                          NULL,
                                          KAknsIIDQsnFrInputPreviewSideL,
                                          KAknsIIDQsnFrInputPreviewMiddle,
                                          KAknsIIDQsnFrInputPreviewSideR );  
    iContextField->SetMsgBubbleCtrlSize( TSize( bubbleLineLayout.iW,bubbleLineLayout.iH ) );    
    iContextField->MsgBubbleCtrl()->SetTextL(KNullDesC);
    
    iContextField->SetLineSeparatorAfterPrompt(ETrue);
    
    //transfer to headerPane
    AddControlL( iContextField );
    
    //add layout as event observer
    iContextField->AddEventObserver( UiLayout() );
    iContextField->SetFocus();

    }

// ---------------------------------------------------------------------------
// create candidate lists.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::CreateCandidateListL()
    {  
    TDropdownListDrawInfo candiDrawInfo( 
            KAknsIIDQsnFrFunctionButtonInactive, 
            KAknsIIDQsnFrFunctionButtonNormal,
            KAknsIIDQsnFrFunctionButtonPressed,
            KAknsIIDQsnFrItutButtonCandiSideL,
            KAknsIIDQsnFrItutButtonCandiMiddle,
            KAknsIIDQsnFrItutButtonCandiSideR,
            KAknsIIDQsnFrItutButtonCandiPressedSideL,
            KAknsIIDQsnFrItutButtonCandiPressedMiddle,
            KAknsIIDQsnFrItutButtonCandiPressedSideR,
            TRgb( 194, 221, 242 ),
            ETrue );

    TInt unitWidth = iLafManager->CandidateUnitWidth();
    TInt unitHeight = iLafManager->CandidateUnitHeight();
    TPoint ltPosition = iLafManager->CandidateLTPos();
    const CFont *font = iLafManager->CandidateFont();
    iCandidateList = CFepCtrlDropdownList::NewL(UiLayout(),
            EHwrCtrlIdCandidateList,
            R_FSHWR_CANDIDATE_DROP_DOWN_LIST,                                            
            ltPosition,                                               
            font ,
            TSize(unitWidth,unitHeight),
            unitWidth,
            3,2,
            unitHeight,
            0,
            0,
            CFepCtrlDropdownList::ECandFromRToL,
            EFalse);            
    AddControlL( iCandidateList );

    TInt horizontalMargin = iLafManager->CandidateHorizontalMargin();
    TInt verticalMargin = iLafManager->CandidateVerticalMargin();
    iCandidateList->SetCellMargin( horizontalMargin, verticalMargin );
    iCandidateList->SetFont( font );
    
    // set the needed events
    iCandidateList->SetEventIdForCandidateSelected( EHwrEventCandidateSelected );
    iCandidateList->AddEventObserver( UiLayout() );
    
    // hide the candidate list
    iCandidateList->Hide( ETrue );
    iCandidateList->SetDropdownListImgID( candiDrawInfo );
    }
    
// ---------------------------------------------------------------------------
// create writing box.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::CreateWritingBoxL()
    {
    iWritingBox = CPeninputArabicFingerHwrWritingWnd::NewL( TRect( 0,0,0,0 ), UiLayout(), 
                                            EHwrCtrlIdWritingBox, EFalse );
    
    AddControlL( iWritingBox );
    
    
    //set it to window
    iWritingBox->SetStrokeEndMark( TPoint( -1,0 ) );
    iWritingBox->AddEventObserver( UiLayout() );
    iWritingBox->EnableTraceOutsideWindow( EFalse );
    iWritingBox->SetWndTransparencyFactor( 0 );
    iWritingBox->InstallPenTraceDecoratorL( KNullDesC,EFalse);
    }

// ---------------------------------------------------------------------------
// create all function buttons.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::CreateButtonsL()
    {
    iCloseBtn = CreateEventBtnL( EHwrCtrlIdClose, R_AKN_FEP_HWR_CLOSE );
    iOptionBtn = CreateEventBtnL( EHwrCtrlIdLanguageOption, R_AKN_FEP_HWR_OPTION );
    
    iBackspaceBtn = CreateRepBtnL( EHwrCtrlIdBackspace, R_AKN_FEP_HWR_BACK_SPACE, 
        EHwrEventKeyBack, EKeyBackspace );
    		
    iArrowLeftBtn = CreateRepBtnL( EHwrCtrlIdArrowLeft, R_AKN_FEP_HWR_ARROWLEFT, 
        EHwrEventNavKeyClicked, EKeyLeftArrow );
    iArrowRightBtn = CreateRepBtnL( EHwrCtrlIdArrowRight, R_AKN_FEP_HWR_ARROWRIGHT, 
        EHwrEventNavKeyClicked, EKeyRightArrow );
    iEnterBtn = CreateEventBtnL(EHwrCtrlIdBtnEnter,R_AKN_FEP_HWR_ENTER);
    iSpaceBtn = CreateEventBtnL(EHwrCtrlIdBtnSpace,R_AKN_FEP_HWR_SPACE);
    // Create 3-pieces color icon for space button
    CPenInput3PiecesColorIcons* colorIcons = 
                CPenInput3PiecesColorIcons::NewL( R_AKN_FEP_VKB_SPACE_ICON1,
                                                  R_AKN_FEP_VKB_SPACE_ICON2,
                                                  R_AKN_FEP_VKB_SPACE_ICON3 );
    iSpaceBtn->Set3PiecesColorIcons( colorIcons );                                                
	iSymbolTableBtn = CreateEventBtnL( EHwrCtrlIdSymbolButton, R_AKN_FEP_HWR_SYMBOL_TABLE_BUTTON );	
    }

// ---------------------------------------------------------------------------
// create virtual sct pad.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::CreateSymbolTableL()
    {
    iSymbolTable = CPeninputArabicFingerHwrSymbolTable::NewL(UiLayout(),EHwrCtrlIdSymbolTableVkbGroup);
	iSymbolTable->Hide(ETrue);
	iSymbolTable->SetResourceId(R_FINGERHWR_ARABIC_SYMBOLTABLE);
	iSymbolTable->ConstructFromResourceL();
    AddControlL( iSymbolTable );
	iSymbolTable->AddEventObserver( UiLayout() );
    }

// ---------------------------------------------------------------------------
// create virtual number mode sct pad.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::CreateNumSymbolTableL()
    {
    iNumSymbolTable = CPeninputArabicFingerHwrNumSymbolTable::NewL(UiLayout(),EHwrCtrlIdNumSymbolTableVkbGroup);
    iNumSymbolTable->Hide(ETrue);
    iNumSymbolTable->SetResourceId(R_FINGERHWR_ARABIC_NUMBERTABLE);
    iNumSymbolTable->ConstructFromResourceL();
    AddControlL( iNumSymbolTable );
    iNumSymbolTable->AddEventObserver( UiLayout() );
    }

// ---------------------------------------------------------------------------
// EventButton creation helper.
// ---------------------------------------------------------------------------
//
CAknFepCtrlEventButton* CPeninputFingerHwrArWnd::CreateEventBtnL( TInt aCtrlId, 
    TInt32 aResId, TInt aEvent/*= 0xFFFF*/,TInt aUnicode/*=0*/ )
    {
    CAknFepCtrlEventButton* button = CAknFepCtrlEventButton::NewL( 
        UiLayout(), aCtrlId, aEvent, aUnicode,
        KAknsIIDQsnFrFunctionButtonNormal,
        KAknsIIDQsnFrFunctionButtonPressed,
        KAknsIIDQsnFrFunctionButtonInactive );
    
    button->SetResourceId( aResId );
    button->ConstructFromResourceL();
    button->AddEventObserver( UiLayout() );        
    AddControlL( button );
    return button;
    }

// ---------------------------------------------------------------------------
// RepeatButton creation helper.
// ---------------------------------------------------------------------------
//
CAknFepCtrlRepeatButton* CPeninputFingerHwrArWnd::CreateRepBtnL( const TInt aCtrlId, 
    TInt32 aResId, const TInt aEvent, const TInt aUnicode )
    {
    CAknFepCtrlRepeatButton* button = CAknFepCtrlRepeatButton::NewL( 
        UiLayout(), aCtrlId, aEvent, aUnicode,
        KAknsIIDQsnFrFunctionButtonNormal,
        KAknsIIDQsnFrFunctionButtonPressed,
        KAknsIIDQsnFrFunctionButtonInactive );
    
    button->SetResourceId( aResId );
    button->ConstructFromResourceL( );
    button->AddEventObserver( UiLayout() );
    AddControlL( button );
    
    return button;
    }

// ---------------------------------------------------------------------------
// load virtual sct keys.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::LoadSymbolVirtualKeysL( const TInt aResId, 
    const RArray<TRect>& aCellRects )
    {
    TAknTextLineLayout txtlayout = iLafManager->SctpadKeyTxtLayout();
    iSymbolTable->KeyPad()->SetTextLineLayout( txtlayout );
    
    TInt fontid = txtlayout.FontId();
    const CFont* font = AknLayoutUtils::FontFromId( fontid );
    iSymbolTable->KeyPad()->SetFont( font );
    
    iSymbolTable->LoadVirtualKeypadKeyL(aResId,aCellRects);
    }

// ---------------------------------------------------------------------------
// load virtual number mode sct keys.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::LoadNumSymbolVirtualKeysL( const TInt aResId, 
    const RArray<TRect>& aCellRects, TBool aReload)
    {
    TAknTextLineLayout txtlayout = iLafManager->NumpadKeyTxtLayout();
    iNumSymbolTable->KeyPad()->SetTextLineLayout( txtlayout );
    
    TInt fontid = txtlayout.FontId();
    const CFont* font = AknLayoutUtils::FontFromId( fontid );
    iNumSymbolTable->KeyPad()->SetFont( font );
    if(aReload)
        {
        iNumSymbolTable->LoadVirtualKeypadKeyL(aResId,aCellRects);
        }
    else
        {
        iNumSymbolTable->UpdateAllVirtualKeysRect(aCellRects);
        }
    }

// ---------------------------------------------------------------------------
//  relayout full ui, reset all controls position. 
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::ResetLayoutL()
    {
    TRect rcScreen = iLafManager->LayoutRect();
    SetRect( rcScreen );
   
    TRect rect;
    
    //Move ICF
    rect = iLafManager->CtrlRect( iContextField->ControlId() );
    iContextField->SetRect( rect );

    //Move Writing box
    rect = iLafManager->CtrlRect( iWritingBox->ControlId() );
    iWritingBox->SetRect( rect );
		
	//resize indicator
	iHandwritingIndicator->ConstructFromResourceL(R_AKN_FEP_HWR_INDICATOR_ICON);
	iHandwritingIndicator->SizeChanged(iLafManager->GetIndicatorRect(),ETrue);
	
    //Move buttons
    TSize siBtnPadding = iLafManager->ButtonInnerPadding();
    TInt pdx = siBtnPadding.iWidth;
    TInt pdy = siBtnPadding.iHeight;
    
    rect = iLafManager->CtrlRect( iCloseBtn->ControlId() );
    MoveIconButton( iCloseBtn, rect, pdx, pdy, ETrue );
   
    rect = iLafManager->CtrlRect( iSymbolTableBtn->ControlId() );
    MoveIconButton( iSymbolTableBtn, rect,  pdx, pdy, ETrue );
    
    rect = iLafManager->CtrlRect( iBackspaceBtn->ControlId() );
    MoveIconButton( iBackspaceBtn, rect,  pdx, pdy, ETrue );
    
    rect = iLafManager->CtrlRect( iOptionBtn->ControlId() );
    MoveIconButton( iOptionBtn, rect,  pdx, pdy, ETrue );
    
	pdx = iLafManager->GetArrowPaddingSize().iWidth;
	pdy = iLafManager->GetArrowPaddingSize().iHeight;
	
    rect = iLafManager->CtrlRect( iArrowLeftBtn->ControlId() );
    MoveIconButton( iArrowLeftBtn, rect, pdx, pdy, ETrue );
    
    rect = iLafManager->CtrlRect( iArrowRightBtn->ControlId() );
    MoveIconButton( iArrowRightBtn, rect, pdx, pdy, ETrue );

    rect = iLafManager->CtrlRect( iSpaceBtn->ControlId() );
    MoveIconButton( iSpaceBtn, rect, pdx, pdy, ETrue );

    TRect pieceInnerRect = rect;
    pieceInnerRect.Shrink(pdx*2,pdy);
    
    // that's a not good algrithm in funciton AknPenInputDrawUtils::Draw3PiecesColorIcon for drawing 3Pieces icons 
    // as when the  outter rect's height was not equal to the inter rect's height, the target rects the left icons and right icons 
    // would be bitblited onto are set to be Rect(0.0.0.0.0) [[which should not be that behavior]].
    // Please theck the codes: AknPenInputDrawUtils::Draw3PiecesColorIcon: this function should be Updated.
    iSpaceBtn->SetDraw3PieceFrameInfo(
                                      TDraw3PiecesFrame(KAknsIIDQgnIndiInputSpaceL,
                                      KAknsIIDQgnIndiInputSpaceMiddle,
                                      KAknsIIDQgnIndiInputSpaceR,
                                      pieceInnerRect));
    
    rect = iLafManager->CtrlRect( iEnterBtn->ControlId() );
    MoveIconButton( iEnterBtn, rect, pdx, pdy, ETrue );

    // load vkb key image
    TSize keysize = iLafManager->VirtualSctpadCellSize().Size();
    iSymbolTable->LoadVkbKeyImageL(R_FINGER_HWR_SCTPAD_IMAGE, keysize );
    
    // get the key rect
    RArray<TRect> rects;
	CleanupClosePushL( rects );
    iLafManager->GetVirtualKeyRects( rects );
	
    // load keys
	if(iLafManager->IsLandscape())
	    {
	    LoadSymbolVirtualKeysL(R_ARABIC_FINGER_HWR_LANDSCAPE_SYMBOL_TABLE, rects);
		}
    else
	    {
		LoadSymbolVirtualKeysL(R_ARABIC_FINGER_HWR_PORTRAIT_SYMBOL_TABLE, rects);
		}
		
    CleanupStack::PopAndDestroy();//rects
    
	//move virtual Sctpad
	TInt rows = iLafManager->VirtualSctpadRowCount();
	TInt cols = iLafManager->VirtualSctpadColCount();		
	
	rect = iLafManager->CtrlRect(iSymbolTable->ControlId());
	iSymbolTable->SetRect(rect);
	
	iSymbolTable->SizeChanged(iLafManager->GetVirtualKeyRect(),
	                          iLafManager->GetSymBtnArray(), rows, cols,
							  iLafManager->IsLandscape());
	//number mode symbol table
    // load number mode vkb key image
    TSize numkeysize = iLafManager->VirtualNumpadCellSize().Size();
    iNumSymbolTable->LoadVkbKeyImageL(R_FINGER_HWR_NUMPAD_IMAGE, numkeysize );
    
    // get the key rect
    RArray<TRect> numrects;
    CleanupClosePushL( numrects );
    iLafManager->GetNumVirtualKeyRects( numrects );
    
    // load keys
    LoadNumSymbolVirtualKeysL(R_FINGER_HWR_NUMPAD, numrects,EFalse);
        
    CleanupStack::PopAndDestroy();//rects
    
    //move virtual Sctpad
    TInt numpadrows = iLafManager->VirtualNumSctpadRowCount();
    TInt numpadcols = iLafManager->VirtualNumSctpadColCount();       
    
    rect = iLafManager->CtrlRect(iNumSymbolTable->ControlId());
    iNumSymbolTable->SetRect(rect);
    
    iNumSymbolTable->SizeChanged(iLafManager->GetVirtualNumKeyRect());
    }


// ---------------------------------------------------------------------------
//  layout for chinese range standby.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::SwitchToStandbyView()
    {
	// hide following controls
    iCandidateList->Hide( ETrue );
    iSymbolTable->Hide( ETrue);
    iNumSymbolTable->Hide(ETrue);
    
	// show following controls
    iArrowLeftBtn->Hide( EFalse );
    iArrowRightBtn->Hide( EFalse );
    iSpaceBtn->Hide( EFalse );
    iEnterBtn->Hide( EFalse );
    iOptionBtn->Hide( EFalse );
    iSymbolTableBtn->SetHighlight( EFalse);
    iContextField->SetReady(ETrue);
	Draw();
    }

// ---------------------------------------------------------------------------
//  layout for symbol range standby.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::SwitchToSymbolTableView()
    {
    CPeninputFingerHwrArLayout* hwrLayout = NULL;
    hwrLayout = static_cast<CPeninputFingerHwrArLayout*>( UiLayout() );    
    CPeninputFingerHwrArDataStore& datastore = hwrLayout->DataStore();
    TBool IsNumberOnly = datastore.IsNumberOnlyMode();
	// show following controls
    iWritingBox->Hide( EFalse );
    if(IsNumberOnly)
        {
        iSymbolTable->Hide( ETrue );
        iNumSymbolTable->Hide(EFalse);
        }
    else
        {
        iSymbolTable->Hide( EFalse );
        iNumSymbolTable->Hide(ETrue);
        }
    iOptionBtn->Hide( EFalse );
	
	// hide follwing controls
	iCandidateList->Hide( ETrue );
    iArrowLeftBtn->Hide( ETrue );
    iArrowRightBtn->Hide( ETrue );
    iSpaceBtn->Hide( ETrue );
    iEnterBtn->Hide( ETrue );
    
	// set the symboltable button highlighted
    iSymbolTableBtn->SetHighlight( ETrue );
	
	// avoid overlap refresh problem after opening symbil table
	// so set this control to be not ready
    iContextField->SetReady(EFalse);
    
    if(IsNumberOnly)
        {
        iNumSymbolTable->OpenSymbolTable();
        iNumSymbolTable->BringToTop();    
        }
    else
        {
        iSymbolTable->OpenSymbolTable();
        iSymbolTable->BringToTop();
        }
    }


// ---------------------------------------------------------------------------
//  EventButton layout helper. Move button to specified rect.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::MoveIconButton( CAknFepCtrlEventButton* aButton, 
    const TRect& aRect, TInt aXPadding, TInt aYPadding, TBool aReloadImages )
    {
    if ( !aButton )
        {
        return;
        }
    
    aButton->SetRect( aRect );
    TRect rcInner = aRect;
    if (  rcInner.Width()> rcInner.Height() )
        {
            TInt dx = ( rcInner.Width() - rcInner.Height() ) / 4;
            rcInner.Move( dx, 0 );
            rcInner.SetWidth( rcInner.Height() + 2*dx);
        }
    else
        {
        TInt dy = ( rcInner.Height() - rcInner.Width() ) / 4;
        rcInner.Move( 0, dy );
        rcInner.SetHeight( rcInner.Width() + 2*dy);        
        }
    rcInner.Shrink( aXPadding, aYPadding );
    if(aButton->ControlId() == EHwrCtrlIdBtnSpace)
        {
        // config 3 pieces icon info       
        TRect middleIconRect = rcInner;
        middleIconRect.Shrink(2,2);
        ( static_cast<CAknFepCtrlCommonButton*> (aButton) )->SetDraw3PieceFrameInfo(
                                          TDraw3PiecesFrame(KAknsIIDQgnIndiInputSpaceL,
                                          KAknsIIDQgnIndiInputSpaceMiddle,
                                          KAknsIIDQgnIndiInputSpaceR,
                                          middleIconRect));                     
        }
    aButton->SizeChanged( aRect, rcInner, aReloadImages );
    }
    
// ---------------------------------------------------------------------------
//  Read control's shadow info.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::ReadShadowInfoL( const TInt aResId )
    {
    TResourceReader resReader;
    CCoeEnv::Static()->CreateResourceReaderLC( resReader, aResId );
     
    TPtrC bmpFileName = resReader.ReadTPtrC();
    TInt32 imgMajorSkinId = resReader.ReadInt32();

    TAknsItemID id;
    TInt skinitemid;
    
    MAknsSkinInstance* skininstance = AknsUtils::SkinInstance();

    for ( TInt i = 0; i < KShadowBmpBr + 1; i++ )
    	{
        const TInt16 bmpId = resReader.ReadInt16();
        const TInt16 bmpMskId = resReader.ReadInt16();
        skinitemid = resReader.ReadInt16();
        
        id.Set( TInt( imgMajorSkinId ), skinitemid );
    	
        if ( bmpId != KInvalidImg )
        	{
            CFbsBitmap* shadowImg = NULL;
            CFbsBitmap* shadowMaskImg = NULL;

            if ( bmpMskId != KInvalidImg )
                {
                AknsUtils::CreateIconL( skininstance, 
                                       id,
                                       shadowImg,
                                       shadowMaskImg,
                                       bmpFileName,
                                       bmpId,
                                       bmpMskId );
                }
            else
                {
                AknsUtils::CreateIconL( skininstance,
                                       id,
                                       shadowImg,
                                       bmpFileName,
                                       bmpId );
                }
            
            SetShadowBmp( shadowImg, shadowMaskImg, TShadowBitmapIndex( i ) );
        	}
    	}
    CleanupStack::PopAndDestroy( 1 ); // reader
    
    }
    
// ---------------------------------------------------------------------------
//  Read control's background info.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::ReadBackgroundInfoL( const TInt aResId )
    {
    if ( aResId == 0 )
        return;
    
    if ( BackgroundBmp() )
        {
        CFbsBitmap* bkBmp = BackgroundBmp();
        delete bkBmp;   
        SetBackgroundBmp( NULL );
        }
    if ( BkMaskBmp() )
        {
        CFbsBitmap* bkMaskBmp = BkMaskBmp();
        delete bkMaskBmp;
        SetBackgroundMaskBmp( NULL );
        }    

    TResourceReader resReader;
    CCoeEnv::Static()->CreateResourceReaderLC( resReader, aResId ); 
     
    TPtrC bmpFileName = resReader.ReadTPtrC();
    TInt32 imgMajorSkinId = resReader.ReadInt32();

    TAknsItemID id;
    TInt skinitemid;
    
    MAknsSkinInstance* skininstance = AknsUtils::SkinInstance();
    const TInt16 bmpId = resReader.ReadInt16();
    const TInt16 bmpMskId = resReader.ReadInt16();
    skinitemid = resReader.ReadInt16();   
    id.Set( TInt( imgMajorSkinId ), skinitemid );
    
    if ( bmpId != KInvalidImg )
        {
        CFbsBitmap* backgroundImg = NULL;
        CFbsBitmap* backgroundMaskImg = NULL;
        if ( bmpMskId != KInvalidImg )
            {
            AknsUtils::CreateIconL( skininstance, id, backgroundImg,
                    backgroundMaskImg, bmpFileName, bmpId, bmpMskId );
            }
        else
            {
            AknsUtils::CreateIconL( skininstance, id, backgroundImg,
                    bmpFileName, bmpId );
            }
       
        SetBackgroundBmp( backgroundImg );
        SetBackgroundMaskBmp( backgroundMaskImg );           
        }
    CleanupStack::PopAndDestroy( 1 ); //reader
    }

// ---------------------------------------------------------------------------
//  read icf appearance info.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::ReadICFInfoL( const TInt aResId )
    {
    if ( aResId == 0 )
        {
        return;
        }

    TResourceReader icfreader;
    CCoeEnv::Static()->CreateResourceReaderLC( icfreader, aResId );
         
    TInt32 colorMajorSkinId = icfreader.ReadInt32();
    TInt skinitemid = icfreader.ReadInt16();
    TInt coloridx = icfreader.ReadInt16();

    TAknsItemID colorid;
    colorid.Set( TInt( colorMajorSkinId ), skinitemid );

    TRgb icftextcolor;
    MAknsSkinInstance* skininstance = AknsUtils::SkinInstance();
    TInt error = AknsUtils::GetCachedColor( skininstance, icftextcolor,
            colorid, coloridx );

    if ( error != KErrNone )
        {
        icftextcolor = TRgb( KDefaultTextColor );
        }

    iContextField->SetTextColorL( icftextcolor );    
    
    // Set highlight color
    TRgb icfhightlightcolor;
    error = AknsUtils::GetCachedColor( skininstance, 
                                       icfhightlightcolor, 
                                       KAknsIIDQsnHighlightColors,
                                       EAknsCIQsnHighlightColorsCG2 ); 
    if ( error == KErrNone ) 
        {
        iContextField->SetTextSelColorL( icfhightlightcolor );
        }

    skinitemid = icfreader.ReadInt16();
    coloridx = icfreader.ReadInt16();
    
    colorid.Set( TInt( colorMajorSkinId ), skinitemid );
    
    TRgb icfframecolor;
    error = AknsUtils::GetCachedColor( skininstance, icfframecolor, 
            colorid, coloridx );

    if ( error != KErrNone )
        {
    	icfframecolor = TRgb( KDefaultFrameColor );
        }

    iContextField->SetBorderColor( icfframecolor );
    iContextField->SetBgImgSkinId( KAknsIIDQgnGrafFepInputBg );
    CleanupStack::PopAndDestroy( 1 ); // icfreader       
    }

// ---------------------------------------------------------------------------
//  read writingbox appearance info.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::ReadWritingBoxInfoL( const TInt aResId )
    {
    if ( aResId == 0 )
	{
	return;
	}

    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC( reader, aResId );

    // get write box bg color
    TInt32 colorMajorSkinId = reader.ReadInt32();
    TInt skinitemid = reader.ReadInt16();
    TInt coloridx = reader.ReadInt16();

    TAknsItemID colorid;
    colorid.Set( TInt( colorMajorSkinId ), skinitemid );

    TRgb boxcolor;
    MAknsSkinInstance* skininstance = AknsUtils::SkinInstance();
    TInt error = AknsUtils::GetCachedColor( skininstance, 
                                           boxcolor, 
                                           colorid, 
                                           coloridx );

    if ( error != KErrNone )
        {
        boxcolor = TRgb( KDefaultWriteBoxBgColor );
        }
    
    iWritingBox->SetBkColor( boxcolor );

    skinitemid = reader.ReadInt16();
    coloridx = reader.ReadInt16();
    
    colorid.Set( TInt( colorMajorSkinId ), skinitemid );
    
    TRgb boxframecolor;
    error = AknsUtils::GetCachedColor( skininstance, 
                                      boxframecolor, 
                                      colorid, 
                                      coloridx );

    if ( error != KErrNone )
        {
    	boxframecolor = TRgb( KDefaultWriteBoxFrameColor );
        }
		
    iWritingBox->SetFrameCol( boxframecolor );

    CleanupStack::PopAndDestroy(); // reader
    }

// ---------------------------------------------------------------------------
//  show bublble or not
// ---------------------------------------------------------------------------
//	
void CPeninputFingerHwrArWnd::ShowBubble(TInt aShow)
    {
    if (aShow > 0)
        {
        TRect outrect,innerrect;                  
        iCandidateList->ShowBubble(ETrue);
    
        // Set priview popup size
        iCandidateList->SetBubbleBitmapParam(NULL,NULL,KAknsIIDQsnFrInputCharPreview);
    
        outrect = iLafManager->PreviewBubbleRect();  
        innerrect = iLafManager->PreviewBubbleInnerRect();
        iCandidateList->SetTextFormat(iLafManager->PreviewBubbleTextLayout());
        iCandidateList->SetBubbleTextFont(iLafManager->PreviewBubbleFont());
        iCandidateList->SetBubbleSize(TSize(outrect.Width(),outrect.Height()));  // Read laf
        iCandidateList->SetFrameDiff(innerrect.iTl.iX - outrect.iTl.iX,
                       innerrect.iTl.iY - outrect.iTl.iY,
                       outrect.iBr.iX - innerrect.iBr.iX,
                       outrect.iBr.iY - innerrect.iBr.iY);
        
        }
    else
        {
        iCandidateList->ShowBubble(EFalse);           
        }
    }

// --------------------------------------------------------------------------
// CPeninputHwrBxAbWnd::CalculateGuideLinePos
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::CalculateGuideLinePos()
      {
      TRect rect = iWritingBox->Rect();
      TInt leftrightmargin = rect.Size().iWidth / 10;
      
      CPeninputFingerHwrArLayout* hwrLayout = NULL;
      hwrLayout = static_cast<CPeninputFingerHwrArLayout*>( UiLayout() );
      
      TInt bottommargin = 0;
      hwrLayout->DataStore().GetBottomGuideLinePos(bottommargin);

      iGuideLineBottomTl.iX = rect.iTl.iX + leftrightmargin;
      iGuideLineBottomTl.iY = rect.iTl.iY + bottommargin;
      
      iGuideLineBottomBr.iX = rect.iBr.iX - leftrightmargin;
      iGuideLineBottomBr.iY = rect.iTl.iY + bottommargin;
      
      iWritingBox->SetBottomGuideLinePosition(iGuideLineBottomTl, iGuideLineBottomBr);
      }

// --------------------------------------------------------------------------
// CPeninputFingerHwrArWnd::DrawGuideLine
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::DrawGuideLine()
    {
    if(iGuideLineOn)
        {
        TInt style = CTransparentHwrWndExt::EGuideLineBottom;    
        iWritingBox->SetGuideLineStyle( style );
    
        // if size changing, or guide line pos has not been set
        CalculateGuideLinePos();
        
        iWritingBox->HideGuideLine(ETrue);
        }
    else
        {
        iWritingBox->HideGuideLine(EFalse);
        }
		
	iWritingBox->RefreshUI();	
    }

// --------------------------------------------------------------------------
// CPeninputFingerHwrArWnd::OpenSymbolTable
// Open the symbol table
// --------------------------------------------------------------------------
//	
void CPeninputFingerHwrArWnd::OpenSymbolTable()
	{
    if(!iSymbolTable->IsPopup() && !iNumSymbolTable->IsPopup())
    	{
    	SwitchToSymbolTableView();  
        }
    }

// --------------------------------------------------------------------------
// CPeninputFingerHwrArWnd::CloseSymbolTable
// Close the symbol table
// --------------------------------------------------------------------------
//	
void CPeninputFingerHwrArWnd::CloseSymbolTable()
	{
	if(iSymbolTable->IsPopup())
		{
	    iSymbolTable->CloseSymbolTable();
	    iSymbolTable->BringToBack();
	    SwitchToStandbyView();		
	    }
	else if(iNumSymbolTable->IsPopup())
	    {
        iNumSymbolTable->CloseSymbolTable();
        iNumSymbolTable->BringToBack();
        SwitchToStandbyView();      	    
	    }
    }

// --------------------------------------------------------------------------
// CPeninputFingerHwrArWnd::IsSymbolTableShowingUp
// Check if the symbol table has been shown up
// --------------------------------------------------------------------------
//	    
TBool CPeninputFingerHwrArWnd::IsSymbolTableShowingUp()
	{
    TBool ret = iSymbolTable->IsPopup() | iNumSymbolTable->IsPopup();  
    return ret;
    }

// --------------------------------------------------------------------------
// CPeninputFingerHwrArWnd::IsCandidateShowup
// Check if the candadiate list has been shown up
// --------------------------------------------------------------------------
//	
TBool CPeninputFingerHwrArWnd::IsCandidateShowup()
    {
	if(!iCandidateList->Hiden())
	    {
		return ETrue;
		}
	return EFalse;	
	}

// --------------------------------------------------------------------------
// CPeninputFingerHwrArWnd::IsCandidateShowup
// Check if the candadiate list has been shown up
// --------------------------------------------------------------------------
//	
void CPeninputFingerHwrArWnd::HideIndicator()
    {
    iWritingBox->ShowIndicator(EFalse);
	}

// --------------------------------------------------------------------------
// CPeninputFingerHwrArWnd::GetCharBeforeCursor
// Get the char by its position in ICF editor
// --------------------------------------------------------------------------
//	
TBool CPeninputFingerHwrArWnd::GetCharBeforeCursor(TInt aCharPos, TUint16& aCharBeforeCursor)
    {
	TBuf<1> dataBeforeCursor;
	iContextField->ExtractText( dataBeforeCursor, aCharPos, 1 );
	
	TBool isFound = EFalse;
	if(dataBeforeCursor!= KNullDesC)
	    {
		aCharBeforeCursor = dataBeforeCursor[0];
		isFound = ETrue;
		}
	
	return isFound;
	}

// ---------------------------------------------------------------------------
// accept editor's number mapping restriction.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::SetNumericMapping( const TDesC& aNumMapping )
    {
    iNumSymbolTable->UpdateTableSymbol(aNumMapping);
    }
// ---------------------------------------------------------------------------
//  set native number mode on or off.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::SetNativeNumMode(const TBool aIsNativeNumMode)
    {
    iNumSymbolTable->SetNativeNumMode(aIsNativeNumMode);
    }
// ---------------------------------------------------------------------------
//  set icf language.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::SetIcfLanguage( TInt aLang )
    {
    iContextField->SetLanguageId(aLang);
    }

//  End Of File
