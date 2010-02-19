/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies).
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
#include <peninputfingerhwrarwnd.rsg>
#include <aknlayoutscalable_apps.cdl.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <peninputlayoutmultilineicf.h>
#include <peninputdropdownlist.h>
#include <peninputrepeatbutton.h>
#include <AknLayoutDef.h>
#include <AknUtils.h>
#include <AknsUtils.h>
#include <AknIconUtils.h>
#include <coemain.h>
#include <s32mem.h>
#include <peninputlayoutchoicelist.h>
#include <aknfeppeninputenums.h>
#include <AknFepGlobalEnums.h>
#include <peninputlayoutinputmodechoice.h>
#include <peninputrepeatbutton.h>
#include <peninputlayout.h>
#include <peninputlayoutbubblectrl.h>
#include <peninputlayoutvkb.h>

#include <peninputmultiimagebutton.h>
#include <peninputpluginutils.h>

#include "peninputfingerhwrarwnd.h"
#include "peninputfingerhwrarlafmanager.h"
#include "peninputfingerhwrarevent.h"
#include "peninputfingerhwrarcontrolid.h"
#include "peninputfingerhwrarstoreconstants.h"
#include "peninputfingerhwrarlayout.h"
#include "peninputfingerhwrardatastore.h"
#include "peninputlayouthwrwnd.h"
#include "peninputfingerhwrarsymboltable.h"
#include "peninputfingerhwrarindicator.h"

#include "peninputfingerhwrarmultipagevkb.h"
#include "hbufcarrayar.h"

const TUint32 KDefaultTextColor = 0x000000;
const TUint KDefaultFrameColor = 0x000000;
const TUint32 KDefaultWriteBoxBgColor = 0xdddddd;
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
		//iHandwritingIndicator->ConstructFromResourceL(R_AKN_FEP_HWR_INDICATOR_ICON);
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
		
		// Set guide line
		DrawGuideLine();
		
		//iHandwritingIndicator->BringToTop();
		iHandwritingIndicator->Draw();
		// update whole area
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
    iNumCandidateList->SizeChanged( iLafManager->CandidateUnitWidth(),
            iLafManager->CandidateUnitHeight(), 
            iLafManager->CandidateUnitWidth(), 
            iLafManager->CandidateLTPos(), 
            3/*KCandidateCountPerRow*/,
            1 );
    iContextField->SetTextMargin ( iLafManager->IcfLeftMargin(),
            iLafManager->IcfRightMargin(),
            iLafManager->IcfTopMargin(),
            iLafManager->IcfBottomMargin() );
    
    iContextField->SetLineSpace( iLafManager->IcfLineSpaceMargin() );                          
    iContextField->SizeChangedL( iLafManager->IcfRect(), 
            iLafManager->IcfTextHeight(),
            iLafManager->IcfFont()->FontMaxHeight(),
            iLafManager->IcfFont() );
    Draw();
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
        iContextField->ShowBubble( aInfo, iContextField->MsgBubbleCtrl()->Rect() );
        }
    else
        {
        iContextField->HideBubble();
        }    
    }

// ---------------------------------------------------------------------------
// accept editor's char ranges restriction.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::SetPermittedRanges( const TInt aPermittedRanges )
    {
    if(aPermittedRanges == 0)
        return;
    
    iPermittedRanges = aPermittedRanges;  
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
void CPeninputFingerHwrArWnd::SetBoxWritingSpeed( const TTimeIntervalMicroSeconds32& aCharDelay )
    {
    iWritingBox->SetCharacterDelay( aCharDelay );
    iWritingBox->SetStrokeDelay( 90000 );   
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
void CPeninputFingerHwrArWnd::ShowSctPage( TInt aPageNo, TInt aPos )
    {
    iSymbolTable->NavigatePage(aPageNo,aPos);
    UpdateAllVirtualKeysFeedback( EFingerHwrSymbolRange );
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
// retrieve the number candicate list control.
// ---------------------------------------------------------------------------
//
CFepCtrlDropdownList* CPeninputFingerHwrArWnd::NumCandidateList()
    {
    return iNumCandidateList;
    }

// ---------------------------------------------------------------------------
// Update layout
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::UpdateLayout( const TBool aIsStandby, const TBool aReloadCandicate)
    {
    TRAP_IGNORE( UpdateLayoutL( aIsStandby, aReloadCandicate) );
    }

// ---------------------------------------------------------------------------
// Update layout
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::UpdateLayoutL( const TBool aIsStandby, 
                                             const TBool aReloadCandicate)
    {
    // Set the candidates when dropdown list is shown
    if ( !aIsStandby )
        {
        typedef CFepCtrlDropdownList::TListType TDropListType;
        
        CPeninputFingerHwrArLayout* hwrLayout = NULL;
        hwrLayout = static_cast<CPeninputFingerHwrArLayout*>( UiLayout() );
        CPeninputFingerHwrArDataStore& datastore = hwrLayout->DataStore();
        
        const RPointerArray<HBufC>& candidates = datastore.Candidate();
        
                                                         
        TDropListType unexpandable = CFepCtrlDropdownList::EListExpandableMultiRowWithoutIconWithBubble;
        
        if ( datastore.PrimaryRange() == ERangeNative )
            {
            iCandidateList->Hide( EFalse );
            
            if ( aReloadCandicate )
                {
                iCandidateList->SetCandidatesL( candidates, unexpandable );
                }
            
            iCandidateList->SetHighlightCell( 0, datastore.Highlight() ); 
               
            }
        else
            {
            iNumCandidateList->Hide( EFalse );
            if ( aReloadCandicate )
                {
                iNumCandidateList->SetCandidatesL( candidates, unexpandable );
                }
            
            iNumCandidateList->SetHighlightCell( 0, datastore.Highlight() );         
            }
        }
    else
        {
		iCandidateList->ResetAndClear(CFepCtrlDropdownList::EListExpandableMultiRowWithoutIconWithBubble);
        iNumCandidateList->ResetAndClear(CFepCtrlDropdownList::EListExpandableMultiRowWithoutIconWithBubble);
        // hide all the dropdown list in standby state
        iCandidateList->Hide( ETrue );
        iNumCandidateList->Hide( ETrue );
        }            
    
    iArrowLeftBtn->Hide( !aIsStandby );
    iArrowRightBtn->Hide( !aIsStandby );
    iArrowUpBtn->Hide( !aIsStandby );
    iArrowDownBtn->Hide( !aIsStandby );
    }  

// ---------------------------------------------------------------------------
// dim arrow buttons.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::DimArrowKeys( TBool aDimArrow )
   {
   iArrowLeftBtn->SetDimmed( aDimArrow );   
   iArrowRightBtn->SetDimmed( aDimArrow );   
   iArrowUpBtn->SetDimmed( aDimArrow );   
   iArrowDownBtn->SetDimmed( aDimArrow ); 
   }

// ---------------------------------------------------------------------------
// retrieve char range of layout, including sct.
// ---------------------------------------------------------------------------
//
TInt CPeninputFingerHwrArWnd::CurrentCharRange()
    {
    return iCurCharRange;
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
	AddControlL(iHandwritingIndicator);
	
    //create icf 
    CreateContextFieldL();
    
    //create candidate lists
    CreateCandidateListL();
    
    //create number candidate lists.
    CreateNumCandidateListL();
    
    //create virtual sct pad
    CreateSymbolTableL();

    //create control buttons
    CreateButtonsL();   
    
    iCurCharRangeNoSct = EFingerHwrNativeRange;
    iCurCharRange      = EFingerHwrNativeRange;
    
	//ResetLayoutL();
    SizeChangedL();
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
// create candidate lists.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::CreateNumCandidateListL()
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
    TInt horizontalMargin = iLafManager->CandidateHorizontalMargin();
    TInt verticalMargin = iLafManager->CandidateVerticalMargin();

    iNumCandidateList = CFepCtrlDropdownList::NewL(UiLayout(),
            EHwrCtrlIdNumCandidateList,
            R_FSHWR_CANDIDATE_DROP_DOWN_LIST,                                            
            ltPosition,                                               
            font ,
            TSize(unitWidth,unitHeight),
            unitWidth,
            3,1,
            unitHeight,
            0,
            0,
            CFepCtrlDropdownList::ECandFromRToL,
            EFalse);            
    AddControlL( iNumCandidateList );
    iNumCandidateList->SetCellMargin( horizontalMargin, verticalMargin );
    iNumCandidateList->SetFont( font );
    
    // set the needed events
    iNumCandidateList->SetEventIdForCandidateSelected( EHwrEventCandidateSelected );
    iNumCandidateList->AddEventObserver( UiLayout() );
    
    // hide the candidate list
    iNumCandidateList->Hide( ETrue );
    iNumCandidateList->SetDropdownListImgID( candiDrawInfo );

    }
// ---------------------------------------------------------------------------
// create writing box.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::CreateWritingBoxL()
    {
    iWritingBox = CTransparentHwrWndExt::NewL( TRect( 0,0,0,0 ), UiLayout(), 
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
    iArrowUpBtn = CreateRepBtnL( EHwrCtrlIdArrowUp, R_AKN_FEP_HWR_ARROWUP, 
        EHwrEventNavKeyClicked, EKeyUpArrow );
    iArrowDownBtn = CreateRepBtnL( EHwrCtrlIdArrowDown, R_AKN_FEP_HWR_ARROWDOWN, 
        EHwrEventNavKeyClicked, EKeyDownArrow );
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
    AddControlL( iSymbolTable );
	iSymbolTable->AddEventObserver( UiLayout() );
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
// load virtual number pad images. 
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::LoadVkbKeyImageL( TInt aResId, const TSize& aKeySize )
    {
    iSymbolTable->LoadVkbKeyImageL(aResId,aKeySize);          
    }

// ---------------------------------------------------------------------------
// load virtual sct keys.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::LoadVirtualSctpadKeysL( const TInt aResId, 
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
	TRect indicatorRect;
	indicatorRect.iTl = rect.iTl;
	indicatorRect.Move(TPoint(10,10));
	
	indicatorRect.SetSize(TSize(50,30));
	
	iHandwritingIndicator->ConstructFromResourceL(R_AKN_FEP_HWR_INDICATOR_ICON);
	iHandwritingIndicator->SizeChanged(indicatorRect,ETrue);
	
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

    rect = iLafManager->CtrlRect( iArrowUpBtn->ControlId() );
    MoveIconButton( iArrowUpBtn, rect,  pdx, pdy, ETrue );
    
    rect = iLafManager->CtrlRect( iArrowLeftBtn->ControlId() );
    MoveIconButton( iArrowLeftBtn, rect,  pdx, pdy, ETrue );
    
    rect = iLafManager->CtrlRect( iArrowRightBtn->ControlId() );
    MoveIconButton( iArrowRightBtn,   rect,  pdx, pdy, ETrue );

    rect = iLafManager->CtrlRect( iArrowDownBtn->ControlId() );
    MoveIconButton( iArrowDownBtn,    rect,  pdx, pdy, ETrue );
    
    rect = iLafManager->CtrlRect( iOptionBtn->ControlId() );
    MoveIconButton( iOptionBtn, rect,  pdx, pdy, ETrue );
    
    // load vkb key image
    TSize keysize = iLafManager->VirtualSctpadCellSize().Size();
    LoadVkbKeyImageL(R_FINGER_HWR_SCTPAD_IMAGE, keysize );
    
    // get the key rect
    RArray<TRect> rects;
	CleanupClosePushL( rects );
    iLafManager->GetVirtualKeyRects( rects );
    
    // load keys
	if(iLafManager->IsLandscape())
	    {
	    LoadVirtualSctpadKeysL(R_ARABIC_FINGER_HWR_LANDSCAPE_SYMBOL_TABLE, rects);
		}
    else
	    {
		LoadVirtualSctpadKeysL(R_ARABIC_FINGER_HWR_PORTRAIT_SYMBOL_TABLE, rects);
		}
    CleanupStack::PopAndDestroy();//rects
    
	//move virtual Sctpad
	TInt rows = iLafManager->VirtualSctpadRowCount();
	TInt cols = iLafManager->VirtualSctpadColCount();		
	
	rect = iLafManager->CtrlRect(iSymbolTable->ControlId());
	iSymbolTable->SetRect(rect);
	
	TSize symButtonSize = iLafManager->SymbolGroupButtonSize();
	iSymbolTable->SizeChanged(TSize(),symButtonSize,rows,cols,iLafManager->IsLandscape());
    }


// ---------------------------------------------------------------------------
//  layout for chinese range standby.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::SwitchToStandbyView()
    {
	// hide following controls
    iCandidateList->Hide( ETrue );
    iNumCandidateList->Hide( ETrue );
    iSymbolTable->Hide( ETrue );
    
	// show following controls
    iArrowLeftBtn->Hide( EFalse );
    iArrowRightBtn->Hide( EFalse );
    iArrowUpBtn->Hide( EFalse );
    iArrowDownBtn->Hide( EFalse );
    iOptionBtn->Hide( EFalse );

    iSymbolTableBtn->Hide( EFalse );
	iSymbolTableBtn->SetHighlight( EFalse);
	
    UpdateAllVirtualKeysFeedback( EFingerHwrNativeRange );
	Draw();
    }

// ---------------------------------------------------------------------------
//  layout for symbol range standby.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::SwitchToSymbolTableView()
    {
	// show following controls
    iWritingBox->Hide( EFalse );
    iSymbolTable->Hide( EFalse );
    iOptionBtn->Hide( EFalse );
	
	// hide follwing controls
	iCandidateList->Hide( ETrue );
    iNumCandidateList->Hide( ETrue );
    iArrowLeftBtn->Hide( ETrue );
    iArrowRightBtn->Hide( ETrue );
    iArrowUpBtn->Hide( ETrue );
    iArrowDownBtn->Hide( ETrue );
    iOptionBtn->Hide( EFalse );    
    iSymbolTableBtn->SetHighlight( ETrue );

    TInt pageno = ( iCurCharRangeNoSct == EFingerHwrNativeRange ) ? 0 : 1;
    ShowSctPage( pageno, EPagePosPageNo );
    
    UpdateAllVirtualKeysFeedback( EFingerHwrSymbolRange );
    iSymbolTable->OpenSymbolTable();
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
            TInt dx = ( rcInner.Width() - rcInner.Height() ) / 2;
            rcInner.Move( dx, 0 );
            rcInner.SetWidth( rcInner.Height() );
        }
    else
        {
        TInt dy = ( rcInner.Height() - rcInner.Width() ) / 2;
        rcInner.Move( 0, dy );
        rcInner.SetHeight( rcInner.Width() );        
        }

    rcInner.Shrink( aXPadding, aYPadding );
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
    iContextField->SetBgImgSkinId( KAknsIIDQsnFrCall2Rect );
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
//  update feedback state of all virtual keys.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArWnd::UpdateAllVirtualKeysFeedback( TInt aType )
    {
    iSymbolTable->UpdateAllVirtualKeysFeedback(aType);
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
    	
        iNumCandidateList->ShowBubble(ETrue);

        // Set priview popup size
        iNumCandidateList->SetBubbleBitmapParam(NULL,NULL,KAknsIIDQsnFrInputCharPreview);
        iNumCandidateList->SetTextFormat(iLafManager->PreviewBubbleTextLayout());
        iNumCandidateList->SetBubbleTextFont(iLafManager->PreviewBubbleFont());
        iNumCandidateList->SetBubbleSize(TSize(outrect.Width(),outrect.Height()));  // Read laf
        iNumCandidateList->SetFrameDiff(innerrect.iTl.iX - outrect.iTl.iX,
                       innerrect.iTl.iY - outrect.iTl.iY,
                       outrect.iBr.iX - innerrect.iBr.iX,
                       outrect.iBr.iY - innerrect.iBr.iY);
 
        }
    else
        {
        iCandidateList->ShowBubble(EFalse);           
        iNumCandidateList->ShowBubble(EFalse);           
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
        iWritingBox->HideGuideLine(ETrue);
        }
    else
        {
        TInt style = CTransparentHwrWndExt::EGuideLineBottom;    
        iWritingBox->SetGuideLineStyle( style );
    
        // if size changing, or guide line pos has not been set
        CalculateGuideLinePos();
        
        iWritingBox->SetBottomGuideLinePosition(iGuideLineBottomTl, iGuideLineBottomBr);
                
        iWritingBox->RefreshUI(); 
        }
    }

// --------------------------------------------------------------------------
// CPeninputFingerHwrArWnd::OpenSymbolTable
// Open the symbol table
// --------------------------------------------------------------------------
//	
void CPeninputFingerHwrArWnd::OpenSymbolTable()
	{
    if(!iSymbolTable->IsPopup())
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
    return iSymbolTable->IsPopup();
    }

// --------------------------------------------------------------------------
// CPeninputFingerHwrArWnd::IsCandidateShowup
// Check if the candadiate list has been shown up
// --------------------------------------------------------------------------
//	
TBool CPeninputFingerHwrArWnd::IsCandidateShowup()
    {
	if(!iCandidateList->Hiden() || !iNumCandidateList->Hiden())
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
	if(!iHandwritingIndicator->Hiden())
	    {
		iHandwritingIndicator->Hide(ETrue);
	    Draw();
		}
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
	if(dataBeforeCursor!= KNullDesC)
	    {
		aCharBeforeCursor = dataBeforeCursor[0];
		return ETrue;
		}
	else
	    {
		return EFalse;
		}
	}
	
//  End Of File
