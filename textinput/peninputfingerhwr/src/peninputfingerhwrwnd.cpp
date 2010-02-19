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
* Description:  Implementation of main window of finger hwr.
*
*/

// INCLUDE
#include <peninputfingerhwrcn.rsg>
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

#include "peninputfingerhwrwnd.h"
#include "peninputfingerhwrlafmanager.h"
#include "peninputfingerhwrevent.h"
#include "peninputfingerhwrcontrolid.h"
#include "peninputfingerhwrstoreconstants.h"
#include "peninputfingerhwrlayout.h"
#include "peninputfingerhwrdatastore.h"
#include "peninputlayouthwrwnd.h"

#include "peninputmultipagevkb.h"
#include "peninputfingerhwrrangeselector.h"
#include "hbufcarray.h"

const TUint32 KDefaultTextColor = 0x000000;
const TUint KDefaultFrameColor = 0x000000;
const TUint32 KDefaultWriteBoxBgColor = 0xdddddd;
const TUint32 KDefaultWriteBoxFrameColor = 0x000000;

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrWnd* CPeninputFingerHwrWnd::NewL( CFepUiLayout* aFepUiLayout,
    TInt aControlId, TBool aLandscapeStyle )
    {
    CPeninputFingerHwrWnd* self = NewLC( aFepUiLayout, aControlId, aLandscapeStyle );
    CleanupStack::Pop( self );

    return self;
    }

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrWnd* CPeninputFingerHwrWnd::NewLC( CFepUiLayout* aFepUiLayout, 
    TInt aControlId, TBool aLandscapeStyle )
    {
    CPeninputFingerHwrWnd* self = new ( ELeave ) CPeninputFingerHwrWnd( aFepUiLayout, aControlId );
    CleanupStack::PushL( self );
    self->ConstructL( aLandscapeStyle );

    return self;
    }

// ---------------------------------------------------------------------------
// c++ destructor
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrWnd::~CPeninputFingerHwrWnd()
    {
    //delete by it's owner
    iContextField    = NULL;

#ifndef RD_INTELLIGENT_TEXT_INPUT    
    iFixEngSymbolList1 = NULL;
    iFixEngSymbolList2 = NULL;
#endif

    iCandidateList   = NULL;
    iPredictList     = NULL;
    iWritingBox      = NULL;
    
    iCloseBtn        = NULL;
    iOptionBtn       = NULL;
    iImeSwitchBtn    = NULL;
    iBackspaceBtn    = NULL;
    iArrowLeftBtn    = NULL; 
    iArrowRightBtn   = NULL;
    iArrowUpBtn      = NULL;
    iArrowDownBtn    = NULL; 
    iRangeSpecialBtn = NULL;
    iRangeSelector   = NULL;
    
    iSctPage0Btn = NULL;
    iSctPage1Btn = NULL;
    iSctPage2Btn = NULL;
    
    iVirtualNumpad = NULL;
    iVirtualSctpad = NULL;    
    iVirtualSctpadFix = NULL;
    iRangeSelector = NULL;
    iInputModeSwitch = NULL; 
    
    delete iLafManager;
    iLafManager = NULL;
    }

// ---------------------------------------------------------------------------
// Construct control from resource.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::ConstructFromResourceL()
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
    if ( iVirtualNumpad )
        {
        TSize keysize = iLafManager->VirtualNumpadCellSize().Size();
        LoadVkbKeyImageL( iVirtualNumpad, R_FINGER_HWR_NUMPAD_IMAGE, keysize );
        
        RArray<TRect> rects;
        CleanupClosePushL( rects );
        iLafManager->GetVirtualKeyRects( rects );
        LoadVirtualNumpadKeysL( R_FINGER_HWR_NUMPAD, rects );
        CleanupStack::PopAndDestroy(); //rects
        }
    
    if ( iVirtualSctpad )
        {
        TSize keysize = iLafManager->VirtualSctpadCellSize().Size();
        LoadVkbKeyImageL( iVirtualSctpad, R_FINGER_HWR_SCTPAD_IMAGE, keysize );
        
        RArray<TRect> rects;
        CleanupClosePushL( rects );
        iLafManager->GetVirtualKeyRects( rects );
        LoadVirtualSctpadKeysL( R_FINGER_HWR_SCTPAD, rects );
        CleanupStack::PopAndDestroy(); //rects
        }
    
    if ( iVirtualSctpadFix )
        {
        TSize keysize = iLafManager->FixVirtualSctpadCellSize().Size();
        LoadVkbKeyImageL( iVirtualSctpadFix, R_FINGER_HWR_SCTPAD_IMAGE, keysize );
        
        RArray<TRect> rects;
        CleanupClosePushL( rects );
        iLafManager->GetFixVirtualKeyRects( rects );
        LoadFixVirtualSctpadKeysL( R_FINGER_HWR_SCTPAD_FIX, rects );
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
void CPeninputFingerHwrWnd::Draw()
    { 
    DrawOpaqueMaskBackground( Rect() );

    if( BackgroundBmp() && BackgroundBmp()->SizeInPixels() != Rect().Size() )
        {
    	AknIconUtils::SetSize(BackgroundBmp(), Rect().Size(), EAspectRatioNotPreserved);
        }

	DrawBackground();    
         
    CControlGroup::Draw();
    
    UpdateArea( Rect(), EFalse );
    }

// ---------------------------------------------------------------------------
// Resize.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::SizeChangedL( TBool /*aLandscapeStyle*/ )
    {
    iLafManager->ReadLafInfo();
    ResetLayoutL();
  
    if ( iLafManager->IsLandscape() )
        {
        iPredictList->SetCandExpandType( CFepCtrlDropdownList::ECandExpandUpward );
        }
    else
        {
        iPredictList->SetCandExpandType( CFepCtrlDropdownList::ECandExpandDownward );
        }    
            
    iCandidateList->SizeChanged( iLafManager->CandidateUnitWidth(),
            iLafManager->CandidateUnitHeight(), 
            iLafManager->CandidateUnitWidth(), 
            iLafManager->CandidateLTPos(), 
            KCandidateCountPerRow,
            1 );
            
    iPredictList->SizeChanged( iLafManager->CandidateUnitWidth(),
            iLafManager->CandidateUnitHeight(),
            iLafManager->CandidateUnitWidth(), 
            iLafManager->PredictiveLTPos(),
            KPredictiveCountPerRow,
            KPredictiveRowCount );

    if ( iFixEngSymbolList1 )
        {
        iFixEngSymbolList1->SizeChanged( iLafManager->CandidateUnitWidth(),
                iLafManager->CandidateUnitHeight(), 
                iLafManager->CandidateUnitWidth(), 
                iLafManager->CandidateLTPos(), 
                KCandidateCountPerRow,
                1 );        
        }
    
    if ( iFixEngSymbolList2 )
        {
        iFixEngSymbolList2->SizeChanged( iLafManager->CandidateUnitWidth(),
                iLafManager->CandidateUnitHeight(), 
                iLafManager->CandidateUnitWidth(), 
                iLafManager->PredictiveLTPos(), 
                KPredictiveCountPerRow,
                1 );             
        }
    
    iContextField->SetTextMargin ( iLafManager->IcfLeftMargin(),
            iLafManager->IcfRightMargin(),
            iLafManager->IcfTopMargin(),
            iLafManager->IcfBottomMargin() );
    
    iContextField->SetLineSpace( iLafManager->IcfLineSpaceMargin() );                          
    iContextField->SizeChangedL( iLafManager->IcfRect(), 
            iLafManager->IcfTextHeight(),
            iLafManager->IcfFont()->FontMaxHeight(),
            iLafManager->IcfFont() );
}

// ---------------------------------------------------------------------------
// accept editor's text alignment.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::SetTextAlignmentL( TInt aAlignment, TInt aLangId )  
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
void CPeninputFingerHwrWnd::SetEditorTextL( const TFepInputContextFieldData& aData )
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
void CPeninputFingerHwrWnd::SetPromptTextL( TUint8* aPromoptData )
    {
    RDesReadStream readStream;

    TPtr8 countPtr( aPromoptData, 2*sizeof( TInt ), 2*sizeof( TInt ) );
    readStream.Open( countPtr );
    CleanupClosePushL( readStream );
    const TInt dataCount = readStream.ReadInt32L();
    const TInt textCount = readStream.ReadInt32L();
    CleanupStack::PopAndDestroy( &readStream );
    
    TPtr8 ptr( aPromoptData+2*sizeof( TInt ), dataCount+textCount, dataCount+textCount );            
    readStream.Open( ptr );
    CleanupClosePushL( readStream );
    
    HBufC8* dataBuf = HBufC8::NewLC( dataCount );
    TPtr8 dataBufPtr = dataBuf->Des();
    readStream.ReadL( dataBufPtr, dataCount );

    TFepPromptText* pIcfData = reinterpret_cast<TFepPromptText*>( const_cast<TUint8*>( dataBufPtr.Ptr() ) );

    HBufC* textBuf;
    if ( textCount > 0 )
        {
        textBuf = HBufC::NewLC( textCount/2 );
        TPtr textBufPtr = textBuf->Des();
        readStream.ReadL( textBufPtr, textCount/2 );
        iContextField->SetPromptTextL( textBuf->Des(), ETrue );
        CleanupStack::PopAndDestroy( textBuf );
        }
    else
        {
        iContextField->SetPromptTextL( KNullDesC, ETrue );
        }

    CleanupStack::PopAndDestroy( dataBuf );
    CleanupStack::PopAndDestroy( &readStream );        
    }
 
// ---------------------------------------------------------------------------
// Dim option button
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::SetEnableSettingBtn(const TBool aEnable)
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
void CPeninputFingerHwrWnd::SetBubbleTextL( const TDesC& aInfo )
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
void CPeninputFingerHwrWnd::SetPermittedRanges( const TInt aPermittedRanges )
    {
    if(aPermittedRanges == 0)
        return;
    
    iPermittedRanges = aPermittedRanges;
    
    iRangeSelector->SetPermittedRanges( aPermittedRanges );    
    }

// ---------------------------------------------------------------------------
// accept editor's number mapping restriction.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::SetNumericMapping( const TDesC& aNumMapping )
    {
    //format of aNumMapping is "0123456789******"
    
    //char offset in aNumMapping
    //cell 0 using aNumMapping[KNumOffsets[0]]
    //cell 1 using aNumMapping[KNumOffsets[1]]
    const TInt KNumOffsets[] = 
        {
         1,  2,  3, 15,
         4,  5,  6, 14,
         7,  8,  9, 13,
        10,  0, 11, 12
        };
    
    TInt maxMappingItemCount = sizeof(KNumOffsets)/sizeof(TInt);
    TInt mappingItemCount = aNumMapping.Length();
    
    //numberpad
    TInt keyCount = iVirtualNumpad->KeyArray().Count();
    for ( TInt i = 0; i < keyCount; i++ )
        {
        CVirtualKey* vk = iVirtualNumpad->KeyArray()[i];
        
        TInt offset = ( i < maxMappingItemCount ) ? KNumOffsets[i] : -1;
        
        if ( offset > -1  && offset < mappingItemCount )
            {
            TUint16 unicode = aNumMapping[offset];
            vk->SetKeyData( aNumMapping.Mid( offset,1 ) );
            vk->SetKeyScancode( unicode );
            vk->SetDimmed( EFalse );
            }
        else
            {
            vk->SetKeyData( KNullDesC );
            vk->SetKeyScancode( 0xFFFF );
            vk->SetDimmed( EFalse );
            }
        }
    
    //sctpad
    keyCount = iVirtualSctpad->KeyArray().Count();
    
    if ( iPermittedRanges == ERangeNumber )
        {
        for ( TInt i = 0; i < keyCount; i++ )
            {
            CVirtualKey* vk = iVirtualSctpad->KeyArray()[i];
            TInt pos = aNumMapping.Find( vk->KeyUnicodes() );
            vk->SetDimmed( pos == KErrNotFound );
            }
        }
    else
        {
        for ( TInt i = 0; i < keyCount; i++ )
            {
            CVirtualKey* vk = iVirtualSctpad->KeyArray()[i];
            vk->SetDimmed( EFalse );
            }        
        }
    
    //sync feedback
    UpdateAllVirtualKeysFeedback( CurrentCharRange() );
    }

// ---------------------------------------------------------------------------
// accept editor's input language.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::SetInputLanguage( const TInt aLanguageId )
    {
    // special punctuations
    const TInt KSpecialPuncPos[] = { 14, 15 };
    const TInt KSpecialChsPunc[] = { 0x300A, 0x300B };
    const TInt KSpecialChtPunc[] = { 0x300C, 0x300D };

    TBool isCht = ( ELangHongKongChinese == aLanguageId ) || 
                  ( ELangTaiwanChinese == aLanguageId );
    
    TInt specialPuncCount = sizeof(KSpecialPuncPos)/sizeof(TInt);
    
    
    for ( TInt i = 0; i < specialPuncCount; i++ )
        {
        TInt keypos = KSpecialPuncPos[i];        
        TInt charcode = isCht ? KSpecialChtPunc[i] : KSpecialChsPunc[i];

        CVirtualKey* vk = iVirtualSctpad->KeyArray()[keypos];
        if ( vk )
            {
            TBuf<1> buf;
            buf.Append( charcode );
            vk->SetKeyData( buf );
            vk->SetKeyScancode( charcode );
            }
        }
    }

// ---------------------------------------------------------------------------
// get strokes from hwr writing box.
// ---------------------------------------------------------------------------
//
const RArray<TPoint>& CPeninputFingerHwrWnd::StrokeList()
    {
    return iWritingBox->StrokeList();
    }

// ---------------------------------------------------------------------------
// set end mark of hwr
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::SetStrokeEndMark( const TPoint& aEndMark )
    {
    iWritingBox->SetStrokeEndMark( aEndMark );
    }

// ---------------------------------------------------------------------------
// set pen color of hwr writing.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::SetBoxPenColor( const TInt aPenColor )
    {
    iWritingBox->SetPenColor( aPenColor );
    }
    
// ---------------------------------------------------------------------------
// set pen size of hwr writing.
// ---------------------------------------------------------------------------
//  
void CPeninputFingerHwrWnd::SetBoxPenSize( const TSize aPenSize )
    {
    iWritingBox->SetPenSize( aPenSize );
    }

// ---------------------------------------------------------------------------
// set writing speed hwr writing.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::SetBoxWritingSpeed( const TTimeIntervalMicroSeconds32& aCharDelay )
    {
    iWritingBox->SetCharacterDelay( aCharDelay );
    iWritingBox->SetStrokeDelay( 90000 );   
    }

// ---------------------------------------------------------------------------
// retrieve rect of hwr writingbox.
// ---------------------------------------------------------------------------
//
TRect CPeninputFingerHwrWnd::WritingBoxRect()
    {
    return iWritingBox->Rect();
    }


// ---------------------------------------------------------------------------
// Cancel wrting
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::CancelWriting()
    {
    iWritingBox->CancelCharWriting();
    }

// ---------------------------------------------------------------------------
// Popup ime switch list.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::PopupImeSwitcher()
    {
    TRAP_IGNORE( iInputModeSwitch->PopUpSwitchListL( iImeSwitchBtn->Rect() ) );
    }


// ---------------------------------------------------------------------------
// show range selection list.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::ShowRangeDropList( TInt aDirect )
    {
    iRangeSelector->SetCurRange( iCurCharRangeNoSct, iCurCharRange == iCurCharRangeNoSct );
    iRangeSelector->Popup( aDirect );
    }

// ---------------------------------------------------------------------------
// cancel range selection.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::CancelRangeDropList()
    {
    iRangeSelector->CancelPopup();
    
    UpdateArea( Rect(), EFalse );
    }

// ---------------------------------------------------------------------------
// test whether the range selection list is visible.
// ---------------------------------------------------------------------------
//
TBool CPeninputFingerHwrWnd::RangeDropListVisible()
    {
    return iRangeSelector ? iRangeSelector->IsPopup() : EFalse;
    }


// ---------------------------------------------------------------------------
// Change SCT page.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::ShowSctPage( TInt aPageNo, TInt aPos )
    {
    switch ( aPos )
        {
        case EPagePosPageNo:
            {
            iVirtualSctpad->NavToPage( aPageNo );
            }
            break;
        default:
            break;
        }
    UpdateSctPageButtonsUi();
    UpdateAllVirtualKeysFeedback( EFingerHwrSymbolRange );
    }

// ---------------------------------------------------------------------------
// sync sct paging buttons.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::UpdateSctPageButtonsUi()
    {
    TInt curpage = iVirtualSctpad->CurPageIndex();
    TInt pagecount = iVirtualSctpad->PageCount();
    
    iSctPage0Btn->Hide( curpage != 0 );
    iSctPage1Btn->Hide( curpage != 1 );
    iSctPage2Btn->Hide( curpage != 2 );
    
    }

// ---------------------------------------------------------------------------
// retrieve the ICF control.
// ---------------------------------------------------------------------------
//
CFepLayoutMultiLineIcf* CPeninputFingerHwrWnd::Icf()
    {
    return iContextField;
    }

// ---------------------------------------------------------------------------
// retrieve the predictive List control. 
// ---------------------------------------------------------------------------
//
CFepCtrlDropdownList* CPeninputFingerHwrWnd::PredictiveList()
    {
    return iPredictList;
    }  

// ---------------------------------------------------------------------------
// retrieve the candicate list control.
// ---------------------------------------------------------------------------
//
CFepCtrlDropdownList* CPeninputFingerHwrWnd::CandidateList()
    {
    return iCandidateList;
    }

// ---------------------------------------------------------------------------
// cancel all popped list.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::CancelAllPopList()
    {
    if ( iRangeSelector->IsPopup() )
        {
        iRangeSelector->CancelPopup();
        }    
    }

// ---------------------------------------------------------------------------
// Update layout
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::UpdateLayout( const TBool aIsStandby, const TBool aReloadCandicate, const TBool aReloadPredictive )
    {
    TRAP_IGNORE( UpdateLayoutL( aIsStandby, aReloadCandicate, aReloadPredictive ) );
    }

// ---------------------------------------------------------------------------
// Update layout
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::UpdateLayoutL( const TBool aIsStandby, 
    const TBool aReloadCandicate, const TBool aReloadPredictive )
    {
    // Set the candidates when dropdown list is shown
    if ( !aIsStandby )
        {
        typedef CFepCtrlDropdownList::TListType TDropListType;
        
        CPeninputFingerHwrLayout* hwrLayout = NULL;
        hwrLayout = static_cast<CPeninputFingerHwrLayout*>( UiLayout() );
        CPeninputFingerHwrDataStore& datastore = hwrLayout->DataStore();
        
        const RPointerArray<HBufC>& candidates = datastore.Candidate();
        const RPointerArray<HBufC>& predictives = datastore.Predictive();
        
        TDropListType expandable = CFepCtrlDropdownList::EListExpandableWithBubble;
        TDropListType unexpandable = CFepCtrlDropdownList::EListNoExpandableWithBubble;
        
        if ( datastore.PrimaryRange() == ERangeNative )
            {
            iCandidateList->Hide( EFalse );
            iPredictList->Hide( EFalse );
            
            if ( aReloadCandicate )
                {
                iCandidateList->SetCandidatesL( candidates, unexpandable );
                }
            
            SetCandHighlightCell();

            // Re draw preview bubble after drawing highlight cell
            iCandidateList->ReDrawBubble();
            iPredictList->ReDrawBubble();
               
            if ( aReloadPredictive )
                {
                iPredictList->SetCandidatesL( predictives, expandable );
                }
            }
        else if ( datastore.PrimaryRange() == ERangeEnglish )
            {
#ifdef RD_INTELLIGENT_TEXT_INPUT
            iCandidateList->Hide(EFalse);
            iPredictList->Hide(EFalse);
            
            if ( aReloadCandicate )
                {
                iCandidateList->SetCandidatesL( candidates, unexpandable );
                }
            
            if ( aReloadPredictive )
                {
                TBool canExpand = datastore.PredictiveCount() > KCandidateCount;
                TDropListType type = canExpand ? expandable : unexpandable;
                iPredictList->SetCandidatesL( predictives, type );
                }
            
            SetCandHighlightCell();
                 
            // Re draw preview bubble after drawing highlight cell
            iCandidateList->ReDrawBubble();
            iPredictList->ReDrawBubble();
            
#else

            if(!iCandidateList->Hiden() && !iPredictList->Hiden())
                {
                if ( aReloadPredictive )
                    {
                    iCandidateList->SetCandidatesL( candidates, unexpandable );
                    iPredictList->SetCandidatesL( predictives, unexpandable );
                    }
                iPredictList->SetHighlightCell( 0 );  
                
                // Re draw preview bubble after drawing highlight cell
                iCandidateList->ReDrawBubble();
                iPredictList->ReDrawBubble();
                }
            
            if(!iFixEngSymbolList1->Hiden() && !iFixEngSymbolList2->Hiden())
                {
                iFixEngSymbolList1->SetCandidatesL( candidates, unexpandable );
                iFixEngSymbolList2->SetCandidatesL( predictives,unexpandable );              
                }
#endif            
            }
        }
    else
        {
        // hide all the dropdown list in standby state
        iCandidateList->Hide( ETrue );
        iPredictList->Hide( ETrue );
        
        //todo : fix EVWG-7U76JF, 2 lines
        iCandidateList->ClearBubble();
        iPredictList->ClearBubble();
        
#ifndef RD_INTELLIGENT_TEXT_INPUT
        iFixEngSymbolList1->Hide( ETrue );
        iFixEngSymbolList2->Hide( ETrue );
#endif

        }            
    
    iArrowLeftBtn->Hide( !aIsStandby );
    iArrowRightBtn->Hide( !aIsStandby );
    iArrowUpBtn->Hide( !aIsStandby );
    iArrowDownBtn->Hide( !aIsStandby );
    
    iOptionBtn->Hide( !aIsStandby );
    iImeSwitchBtn->Hide( !aIsStandby );
    }

// ---------------------------------------------------------------------------
// Set highlight candidate cell when expand another list.
// ---------------------------------------------------------------------------
// 
void CPeninputFingerHwrWnd::SetCandHighlightCell()
    {
    // if expand predictive list upward before charactor time out,
    // the candidate list will cover on expand predictive list.
    // To avoid this problem, add this function.
    CPeninputFingerHwrLayout* hwrLayout = NULL;
    hwrLayout = static_cast<CPeninputFingerHwrLayout*>( UiLayout() );
    CPeninputFingerHwrDataStore& datastore = hwrLayout->DataStore();    
    
    CFepUiBaseCtrl* capCtrl = RootControl()->CtrlCapPointer();
    
    if(!(capCtrl && capCtrl->ControlId() == EHwrCtrlIdPredictiveList && 
         iPredictList->CandExpandType() == CFepCtrlDropdownList::ECandExpandUpward)) 
        {
        // Set and draw highlight cell
        iCandidateList->SetHighlightCell( 0, datastore.Highlight() );
        }
    else
        {
        // Just set the highlight cell without draw
        iCandidateList->SetHighlightCell( 0, datastore.Highlight(), EFalse );
        }
    }
      
     
#ifndef RD_INTELLIGENT_TEXT_INPUT
// ---------------------------------------------------------------------------
// hide or show two eng-symbol candidate lists.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::HideTwoRowEngSymbolList( const TBool aIsHide )
    {
    iCandidateList->Hide( !aIsHide );
    iPredictList->Hide( !aIsHide );
    iFixEngSymbolList1->Hide( aIsHide );
    iFixEngSymbolList2->Hide( aIsHide );
    }
#endif

// ---------------------------------------------------------------------------
// dim arrow buttons.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::DimArrowKeys( TBool aDimArrow )
   {
   iArrowLeftBtn->SetDimmed( aDimArrow );   
   iArrowRightBtn->SetDimmed( aDimArrow );   
   iArrowUpBtn->SetDimmed( aDimArrow );   
   iArrowDownBtn->SetDimmed( aDimArrow ); 
   }

// ---------------------------------------------------------------------------
// change layout to specified char range.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::TryToChangeCharRange( TInt aCharRange )
    {
    //CancelPopup must be called before LayoutStandby????
    //if not, pointer is captured, unregister feedback area will failed.
    if ( iRangeSelector->IsPopup() )
        {
        iRangeSelector->CancelPopup();
        }
    
    if ( iCurCharRange != aCharRange )
        {
        switch ( aCharRange )
            {
            case EFingerHwrNativeRange:
                {
                iCurCharRange = EFingerHwrNativeRange;
                iCurCharRangeNoSct = EFingerHwrNativeRange;
                LayoutStandbyChinese();
                }
                break;
            case EFingerHwrEnglishRange:
                {
                iCurCharRange = EFingerHwrEnglishRange;
                iCurCharRangeNoSct = EFingerHwrEnglishRange;
                LayoutStandbyEnglish();
                }
                break;
            case EFingerHwrNumberRange:
                {
                iCurCharRange = EFingerHwrNumberRange;
                iCurCharRangeNoSct = EFingerHwrNumberRange;
                LayoutStandbyNumeric();
                }
                break;
            case EFingerHwrSymbolRange:
                {
                iCurCharRange = EFingerHwrSymbolRange;
                LayoutStandbySymbol();
                }
                break; 
            default:
                break;
            }
        TRAP_IGNORE(ReloadWritingGuideLinesL());
        }

    iRangeSelector->SetCurRange( iCurCharRangeNoSct, iCurCharRange == iCurCharRangeNoSct );
    }

// ---------------------------------------------------------------------------
// retrieve char range of layout, including sct.
// ---------------------------------------------------------------------------
//
TInt CPeninputFingerHwrWnd::CurrentCharRange()
    {
    return iCurCharRange;
    }

// ---------------------------------------------------------------------------
// restore the char range saved before sct.
// ---------------------------------------------------------------------------
//
TInt CPeninputFingerHwrWnd::RestoreNonSctLayout()
    {
    TInt rangeFlag = ERangeInvalid;
    
    switch ( iCurCharRangeNoSct )
        {
        case EFingerHwrNativeRange:
            {
            rangeFlag = ERangeNative;
            }
            break;
        case EFingerHwrEnglishRange:
            {
            rangeFlag = ERangeEnglish;
            }
            break;
        case EFingerHwrNumberRange:
            {
            rangeFlag = ERangeNumber;
            }
            break;
        default:
            {
            rangeFlag = ERangeInvalid;
            }
            break;
        }
    TryToChangeCharRange( iCurCharRangeNoSct );
    return rangeFlag;
    }

// ---------------------------------------------------------------------------
// c++ constructor
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrWnd::CPeninputFingerHwrWnd( 
    CFepUiLayout* aFepUiLayout, TInt aControlId )
    : CControlGroup( aFepUiLayout,aControlId ), iFirstTimeConstruct( ETrue )
    {
    iInEditWordQueryDlg = EFalse;
    }
    
// ---------------------------------------------------------------------------
// Symbian second-phase constructor
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::ConstructL( TBool /*aLandscapeStyle*/ )
    {
    BaseConstructL();    
    
    iLafManager = CPeninputFingerHwrLAFManager::NewL();
    iLafManager->ReadLafInfo();
    
    //crate writing 
    CreateWritingBoxL();
    
    //create icf 
    CreateContextFieldL();
    
    //create candidate lists
    CreateCandidateListL();
    
    //create predictive lists
    CreatePredictListL();
    
    //create virtual num pad
    CreateVirtualNumpadL();
    
    //create virtual sct pad
    CreateVirtualSctpadL();
    
    //create space and backspace virtual pad
    CreateFixVirtualSctpadL();

    //create control buttons
    CreateButtonsL();
    
    //create range selecor
    iRangeSelector = CPeninputFingerHwrRangeSelector::NewL( 
        UiLayout(), EHwrCtrlIdRangeSelector );
    AddControlL( iRangeSelector );
    
    //create ime selector
    iInputModeSwitch = CPeninputLayoutInputmodelChoice::NewL( 
        UiLayout(), EHwrCtrlIdInputModeSwitcher, 
        EPluginInputModeFingerHwr );
    
    iInputModeSwitch->SetListSkinID( KAknsIIDQsnFrList, KAknsIIDQsnFrPopupSub );
    UiLayout()->AddControlL( iInputModeSwitch );
    iInputModeSwitch->AddEventObserver( UiLayout() );    
    
    iCurCharRangeNoSct = EFingerHwrNativeRange;
    iCurCharRange      = EFingerHwrNativeRange;
    ResetLayoutL();
    
    LayoutStandbyChinese();
    }
    
// ---------------------------------------------------------------------------
// create icf.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::CreateContextFieldL()
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
void CPeninputFingerHwrWnd::CreateCandidateListL()
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
    iCandidateList = CFepCtrlDropdownList::NewL( UiLayout(),
            EHwrCtrlIdCandidateList, 
            R_FSHWR_CANDIDATE_DROP_DOWN_LIST, ltPosition, 
            TSize(unitWidth, unitHeight), KCandidateCountPerRow, 1 );  
            
    AddControlL( iCandidateList );

    const CFont *font = iLafManager->CandidateFont();
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
    

#ifndef RD_INTELLIGENT_TEXT_INPUT    
    iFixEngSymbolList1 = CFepCtrlDropdownList::NewL( UiLayout(),
                                                 EHwrCtrlIdFixEngSymbolList,
                                                 R_FSHWR_CANDIDATE_DROP_DOWN_LIST,
                                                 ltPosition,
                                                 TSize( unitWidth, unitHeight ),
                                                 KCandidateCountPerRow,
                                                 1 );
    
    AddControlL( iFixEngSymbolList1 );
    
    iFixEngSymbolList1->SetCellMargin( horizontalMargin, verticalMargin );
    iFixEngSymbolList1->SetFont( font );
    iFixEngSymbolList1->SetEventIdForCandidateSelected( EHwrEventCandidateSelected );
    iFixEngSymbolList1->AddEventObserver( UiLayout() );
    
    // hide the candidate list
    iFixEngSymbolList1->Hide( ETrue );
    iFixEngSymbolList1->SetDropdownListImgID( candiDrawInfo );
    
#endif
    }

// ---------------------------------------------------------------------------
// create predictive lists.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::CreatePredictListL()
    {                                       
    TDropdownListDrawInfo predicDrawInfo( 
            KAknsIIDQsnFrFunctionButtonInactive, 
            KAknsIIDQsnFrFunctionButtonNormal,
            KAknsIIDQsnFrFunctionButtonPressed,
            KAknsIIDQsnFrItutButtonComposeSideL,
            KAknsIIDQsnFrItutButtonComposeMiddle,
            KAknsIIDQsnFrItutButtonComposeSideR,
            KAknsIIDQsnFrItutButtonComposePressedSideL,
            KAknsIIDQsnFrItutButtonComposePressedSideR,
            KAknsIIDQsnFrItutButtonComposePressedMiddle,
            TRgb( 194, 221, 242 ),
            ETrue );     

    TInt unitWidth = iLafManager->CandidateUnitWidth();
    TInt unitHeight = iLafManager->CandidateUnitHeight();
    TPoint ltPosition = iLafManager->PredictiveLTPos();
    iPredictList = CFepCtrlDropdownList::NewL( UiLayout(),
                                               EHwrCtrlIdPredictiveList,
                                               R_FSHWR_PREDICT_DROP_DOWN_LIST,
                                               ltPosition,
                                               TSize( unitWidth, unitHeight ),
                                               KPredictiveCountPerRow,
                                               KPredictiveRowCount );  
  
    if ( iLafManager->IsLandscape() )
        {
        iPredictList->SetCandExpandType( CFepCtrlDropdownList::ECandExpandUpward );
        }
          
    AddControlL( iPredictList );
    
    const CFont *font = iLafManager->CandidateFont();
    TInt horizontalMargin = iLafManager->CandidateHorizontalMargin();
    TInt verticalMargin = iLafManager->CandidateVerticalMargin();
    iPredictList->SetCellMargin( horizontalMargin, verticalMargin );
    iPredictList->SetFont( font );
    
    // set the needed events
    iPredictList->SetEventIdForCandidateSelected( EHwrEventPredictiveSelected );
    iPredictList->SetEventIdForNextPageCandidate( EHwrEventGetNextCandidatePage );
    iPredictList->AddEventObserver( UiLayout() );
    
    // hide the predictive list
    iPredictList->Hide( ETrue );
    iPredictList->SetDropdownListImgID( predicDrawInfo );
    
    
#ifndef RD_INTELLIGENT_TEXT_INPUT
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
    
    iFixEngSymbolList2 = CFepCtrlDropdownList::NewL( UiLayout(),
                                                 EHwrCtrlIdFixEngSymbolList,
                                                 R_FSHWR_CANDIDATE_DROP_DOWN_LIST,
                                                 ltPosition,
                                                 TSize( unitWidth, unitHeight ),
                                                 KCandidateCountPerRow,
                                                 1 );
    
    AddControlL( iFixEngSymbolList2 );
    
    iFixEngSymbolList2->SetCellMargin( horizontalMargin, verticalMargin );
    iFixEngSymbolList2->SetFont( font );
    iFixEngSymbolList2->SetEventIdForCandidateSelected( EHwrEventCandidateSelected );
    iFixEngSymbolList2->AddEventObserver( UiLayout() );
    
    // hide the candidate list
    iFixEngSymbolList2->Hide( ETrue );
    iFixEngSymbolList2->SetDropdownListImgID( candiDrawInfo );
#endif  
    }
    
// ---------------------------------------------------------------------------
// create writing box.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::CreateWritingBoxL()
    {
    iWritingBox = CTransparentHwrWnd::NewL( TRect( 0,0,0,0 ), UiLayout(), 
                                            EHwrCtrlIdWritingBox, EFalse );
    
    AddControlL( iWritingBox );
    
    
    //set it to window
    iWritingBox->SetStrokeEndMark( TPoint( -1,0 ) );
    iWritingBox->AddEventObserver( UiLayout() );
    iWritingBox->EnableTraceOutsideWindow( EFalse );
    iWritingBox->SetWndTransparencyFactor( 0 );

    iWritingBox->InstallPenTraceDecoratorL( KNullDesC );
    }

// ---------------------------------------------------------------------------
// create all function buttons.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::CreateButtonsL()
    {
    iCloseBtn = CreateEventBtnL( EHwrCtrlIdClose, R_AKN_FEP_HWR_CLOSE );
    iOptionBtn = CreateEventBtnL( EHwrCtrlIdLanguageOption, R_AKN_FEP_HWR_OPTION );
    iImeSwitchBtn = CreateEventBtnL( EHwrCtrlIdVkbSwitcher, R_AKN_FEP_HWR_VKB );
    
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
    
    iRangeSpecialBtn = CreateEventBtnL( EHwrCtrlIdSymbolButton, R_AKN_FEP_HWR_RANGE_SPECIAL );
    
    iSctPage0Btn  = CreateEventBtnL( EHwrCtrlIdSctPage0Btn, R_AKN_FEP_HWR_SCT_PAGE_1 );
    iSctPage1Btn  = CreateEventBtnL( EHwrCtrlIdSctPage1Btn, R_AKN_FEP_HWR_SCT_PAGE_2 );
    iSctPage2Btn  = CreateEventBtnL( EHwrCtrlIdSctPage2Btn, R_AKN_FEP_HWR_SCT_PAGE_3 );
    }

// ---------------------------------------------------------------------------
// create virtual number pad.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::CreateVirtualNumpadL()
    {
    TFontSpec spec;

    iVirtualNumpad = CVirtualKeyboard::NewL( 
        TRect( 0,0,0,0 ),
        UiLayout(),
        EHwrCtrlIdNumpad,
        spec );
    AddControlL( iVirtualNumpad );
    iVirtualNumpad->AddEventObserver( UiLayout() );

    iVirtualNumpad->SetResourceId( KInvalidResId );
    
    // read keypad image info
    TSize keysize = iLafManager->VirtualNumpadCellSize().Size();
    
    
    LoadVkbKeyImageL( iVirtualNumpad, R_FINGER_HWR_NUMPAD_IMAGE, keysize );
    
    RArray<TRect> rects;
    CleanupClosePushL( rects );
    
    iLafManager->GetVirtualKeyRects( rects );
    LoadVirtualNumpadKeysL( R_FINGER_HWR_NUMPAD, rects );
    
    CleanupStack::PopAndDestroy(); //rects
    
    iVirtualNumpad->SetKeyTextColorGroup( EAknsCIQsnTextColorsCG68 );
    iVirtualNumpad->SetDrawOpaqueBackground( EFalse );
    }

// ---------------------------------------------------------------------------
// create virtual sct pad.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::CreateVirtualSctpadL()
    {
    TFontSpec spec;

    iVirtualSctpad = CMultiPageVirtualKeyboard::NewL( 
        iLafManager->CtrlRect(EHwrCtrlIdSctpad),
        UiLayout(),
        EHwrCtrlIdSctpad,
        spec );
    AddControlL( iVirtualSctpad );
    iVirtualSctpad->AddEventObserver( UiLayout() );
    
    iVirtualSctpad->SetResourceId( KInvalidResId );
    
    // read keypad image info
    TSize keysize = iLafManager->VirtualSctpadCellSize().Size();

    LoadVkbKeyImageL( iVirtualSctpad, R_FINGER_HWR_SCTPAD_IMAGE, keysize );
    
    RArray<TRect> rects;
    CleanupClosePushL( rects );
    
    iLafManager->GetVirtualKeyRects( rects );
    LoadVirtualSctpadKeysL( R_FINGER_HWR_SCTPAD, rects );
    
    CleanupStack::PopAndDestroy();//rects
    
    TInt rows = iLafManager->VirtualSctpadRowCount();
    TInt cols = iLafManager->VirtualSctpadColCount();
    iVirtualSctpad->UpdatePaging( rows, cols );
    
    iVirtualSctpad->SetKeyTextColorGroup( EAknsCIQsnTextColorsCG68 );
    iVirtualSctpad->SetDrawOpaqueBackground( EFalse );    
    }


// ---------------------------------------------------------------------------
// create virtual sct pad.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::CreateFixVirtualSctpadL()
    {
    TFontSpec spec;

    iVirtualSctpadFix = CVirtualKeyboard::NewL( 
        iLafManager->CtrlRect(EHwrCtrlIdSctpadFix),
        UiLayout(),
        EHwrCtrlIdSctpadFix,
        spec );
    AddControlL( iVirtualSctpadFix );
    iVirtualSctpadFix->AddEventObserver( UiLayout() );
    
    iVirtualSctpadFix->SetResourceId( KInvalidResId );
    
    // read keypad image info
    TSize keysize = iLafManager->FixVirtualSctpadCellSize().Size();

    LoadVkbKeyImageL( iVirtualSctpadFix, R_FINGER_HWR_SCTPAD_IMAGE, keysize );
    
    RArray<TRect> rects;
    CleanupClosePushL( rects );
    
    iLafManager->GetFixVirtualKeyRects( rects );
    LoadFixVirtualSctpadKeysL( R_FINGER_HWR_SCTPAD_FIX, rects );
    
    CleanupStack::PopAndDestroy();//rects
    
    iVirtualSctpadFix->SetKeyTextColorGroup( EAknsCIQsnTextColorsCG68 );
    iVirtualSctpadFix->SetDrawOpaqueBackground( EFalse );    
    }

// ---------------------------------------------------------------------------
// EventButton creation helper.
// ---------------------------------------------------------------------------
//
CAknFepCtrlEventButton* CPeninputFingerHwrWnd::CreateEventBtnL( TInt aCtrlId, 
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
CAknFepCtrlRepeatButton* CPeninputFingerHwrWnd::CreateRepBtnL( const TInt aCtrlId, 
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
void CPeninputFingerHwrWnd::LoadVkbKeyImageL( CVirtualKeyboard* aVkb, 
    TInt aResId, const TSize& aKeySize )
    {
    TResourceReader reader;    
    CCoeEnv::Static()->CreateResourceReaderLC( reader, aResId );      
    
    TPtrC bmpFileName = reader.ReadTPtrC();
    TInt32 imgMajorSkinId = reader.ReadInt32();
    TAknsItemID id;
    
    TSize keySize = aKeySize;
    
    for ( TInt index = 0; index <= EKeyBmpLastType ; index += 2 )
        { 
        // Get the image ids and mask ids from resource
        TInt bmpId = reader.ReadInt16(); 
        TInt bmpMskId = reader.ReadInt16();
        
        // read skin item id
        const TInt skinitemid = reader.ReadInt16();
        id.Set( TInt( imgMajorSkinId ), skinitemid );
        
        if ( bmpId != KInvalidImg )
            {
            CFbsBitmap* bmp = NULL;
            CFbsBitmap* maskbmp = NULL;

            if ( bmpMskId != KInvalidImg )
                {
                AknsUtils::CreateIconL( AknsUtils::SkinInstance(),
                   id, bmp, maskbmp, bmpFileName, bmpId, bmpMskId );
                
                // set maskbmp and size
                AknIconUtils::SetSize( maskbmp, keySize, EAspectRatioNotPreserved );
                aVkb->SetNonIrregularKeyBitmapL( 
                        TVirtualKeyBmpType( EKeyBmpNormal + index + 1 ), maskbmp );
                }
            else
                {
                AknsUtils::CreateIconL( AknsUtils::SkinInstance(), id, 
                    bmp, bmpFileName, bmpId );
                }
            // set bmp and size
            AknIconUtils::SetSize( bmp, keySize, EAspectRatioNotPreserved );
            aVkb->SetNonIrregularKeyBitmapL( 
                TVirtualKeyBmpType( EKeyBmpNormal + index ), bmp );
            }       
        }
    // Pop and destroy reader
    CleanupStack::PopAndDestroy( 1 );           
    }


// ---------------------------------------------------------------------------
// create virtual key.
// ---------------------------------------------------------------------------
//
CVirtualKey* CPeninputFingerHwrWnd::CreateVkbKeyL( TResourceReader& aReader, 
    const TRect aKeyRect )
    {
    CHBufCArray* keytexts = CHBufCArray::NewL();
    CleanupStack::PushL( keytexts );
    
    for ( TInt i = 0; i <= EPosLast; i++ )
        {
        HBufC* unicode = aReader.ReadHBufCL();
        keytexts->Array().AppendL( unicode );
        }

    TInt keyscancode = aReader.ReadInt16();

    HBufC* text = keytexts->Array()[0];
    
    CVirtualKey* vk = NULL;
    if ( text )
        {
        vk = CVirtualKey::NewL( *text, keyscancode, aKeyRect, aKeyRect, 0 );
        }
    else 
        {
        vk = CVirtualKey::NewL( KNullDesC, keyscancode, aKeyRect, aKeyRect, 0 );
        }

    CleanupStack::PopAndDestroy( keytexts ); //keytexts


    TRect innerrect = aKeyRect;
    innerrect.Shrink( TSize(10, 10) );
    vk->SetInnerRect( innerrect );
    
    return vk;
    }


// ---------------------------------------------------------------------------
// update virtual key info.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::UpdateVkbKeyL( CVirtualKey* aVirtualKey, 
    TResourceReader& aReader, const TRect aKeyRect )
    {
    CHBufCArray* keytexts = CHBufCArray::NewL();
    CleanupStack::PushL( keytexts );
    
    for ( TInt i = 0; i <= EPosLast; i++ )
        {
        HBufC* unicode = aReader.ReadHBufCL();
        keytexts->Array().AppendL( unicode );
        }

    TInt keyscancode = aReader.ReadInt16();

    HBufC* text = keytexts->Array()[0];
    if ( text )
        {
        aVirtualKey->SetKeyData( *text );
        }
    else
        {
        aVirtualKey->SetKeyData( KNullDesC );
        }
    
    aVirtualKey->SetKeyScancode( keyscancode );

    CleanupStack::PopAndDestroy( keytexts ); //keytexts


    TRect innerrect = aKeyRect;
    innerrect.Shrink( TSize(10,10) );
    aVirtualKey->SetInnerRect( innerrect );
   
    }

// ---------------------------------------------------------------------------
// load virtual number keys.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::LoadVirtualNumpadKeysL( 
    const TInt aResId, const RArray<TRect>& aCellRects )
    {
    iVirtualNumpad->SetResourceId( aResId );
    
    TAknTextLineLayout txtlayout = iLafManager->NumpadKeyTxtLayout();
    iVirtualNumpad->SetTextLineLayout( txtlayout );
    
    TInt fontid = txtlayout.FontId();
    const CFont* font = AknLayoutUtils::FontFromId( fontid );
    iVirtualNumpad->SetFont( font );
    

    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC( reader, aResId );


    // construct keys
    TInt resKeyCount = reader.ReadInt16();
    TInt existsKeyCount = iVirtualNumpad->KeyArray().Count();
    TInt rectCount = aCellRects.Count();
    
    for ( TInt i = 0; i < resKeyCount; i++ )
        {
        if ( i < existsKeyCount )
            {
            CVirtualKey* vk = iVirtualNumpad->KeyArray()[i];
            UpdateVkbKeyL( vk, reader, aCellRects[i%rectCount] );
            }
        else
            {
            CVirtualKey* vk = CreateVkbKeyL( reader, aCellRects[i%rectCount] );
            CleanupStack::PushL( vk );
            iVirtualNumpad->AddKeyL( vk );
            CleanupStack::Pop( vk );
            }
        }    
    CleanupStack::PopAndDestroy( 1 ); // reader
    
    iVirtualNumpad->Draw();
    iVirtualNumpad->UpdateArea( iVirtualNumpad->Rect() );
    }


// ---------------------------------------------------------------------------
// load virtual sct keys.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::LoadVirtualSctpadKeysL( const TInt aResId, 
    const RArray<TRect>& aCellRects )
    {
    iVirtualSctpad->SetResourceId( aResId );
    
    TAknTextLineLayout txtlayout = iLafManager->SctpadKeyTxtLayout();
    iVirtualSctpad->SetTextLineLayout( txtlayout );
    
    TInt fontid = txtlayout.FontId();
    const CFont* font = AknLayoutUtils::FontFromId( fontid );
    iVirtualSctpad->SetFont( font );
    

    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC( reader, aResId );

    // construct keys
    TInt resKeyCount = reader.ReadInt16();
    TInt existsKeyCount = iVirtualSctpad->KeyArray().Count();
    TInt rectCount = aCellRects.Count();
    
    for ( TInt i = 0; i < resKeyCount; i++ )
        {
        if ( i < existsKeyCount )
            {
            CVirtualKey* vk = iVirtualSctpad->KeyArray()[i];
            UpdateVkbKeyL( vk, reader, aCellRects[i%rectCount] );
            }
        else
            {
            CVirtualKey* vk = CreateVkbKeyL( reader, aCellRects[i%rectCount] );
            CleanupStack::PushL( vk );
            iVirtualSctpad->AddKeyL( vk );
            CleanupStack::Pop( vk );
            }
        }
    
    CleanupStack::PopAndDestroy( 1 ); // reader
    
    iVirtualSctpad->Draw();
    iVirtualSctpad->UpdateArea( iVirtualSctpad->Rect() );
    
    }

// ---------------------------------------------------------------------------
// load virtual sct keys.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::LoadFixVirtualSctpadKeysL( const TInt aResId, 
    const RArray<TRect>& aCellRects )
    {
    iVirtualSctpadFix->SetResourceId( aResId );
    
    TAknTextLineLayout txtlayout = iLafManager->FixSctpadKeyTxtLayout();
    iVirtualSctpadFix->SetTextLineLayout( txtlayout );
    
    TInt fontid = txtlayout.FontId();
    const CFont* font = AknLayoutUtils::FontFromId( fontid );
    iVirtualSctpadFix->SetFont( font );
    

    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC( reader, aResId );

    // construct keys
    TInt resKeyCount = reader.ReadInt16();
    TInt existsKeyCount = iVirtualSctpadFix->KeyArray().Count();
    TInt rectCount = aCellRects.Count();
    
    for ( TInt i = 0; i < resKeyCount; i++ )
        {
        if ( i < existsKeyCount )
            {
            CVirtualKey* vk = iVirtualSctpadFix->KeyArray()[i];
            UpdateVkbKeyL( vk, reader, aCellRects[i] );
            }
        else
            {
            CVirtualKey* vk = CreateVkbKeyL( reader, aCellRects[i] );
            CleanupStack::PushL( vk );
            iVirtualSctpadFix->AddKeyL( vk );
            CleanupStack::Pop( vk );
            }
        }
    
    CleanupStack::PopAndDestroy( 1 ); // reader
    
    iVirtualSctpadFix->Draw();
    iVirtualSctpadFix->UpdateArea( iVirtualSctpadFix->Rect() );
    
    }

// ---------------------------------------------------------------------------
//  relayout full ui, reset all controls position. 
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::ResetLayoutL()
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
    
    //Move Virtual Numpad
    if ( iVirtualNumpad )
        {
        rect = iLafManager->CtrlRect( iVirtualNumpad->ControlId() );
        iVirtualNumpad->SetRect( rect );

        const RPointerArray<CVirtualKey>& keys = iVirtualNumpad->KeyArray();
        RArray<TRect> rects;
        iLafManager->GetVirtualKeyRects( rects );
        
        const TInt count = keys.Count();
        for ( TInt i = 0; i < count; i++ )
            {
            CVirtualKey* vk = keys[i];
            vk->SetRect( rects[i] );
            }
        rects.Reset();
        rects.Close();
        }
    
    //move virtual Sctpad
    if ( iVirtualSctpad )
        {
        rect = iLafManager->CtrlRect( iVirtualSctpad->ControlId() );
        iVirtualSctpad->SetRect( rect );
        
        const RPointerArray<CVirtualKey>& keys = iVirtualSctpad->KeyArray();
        RArray<TRect> rects;
        iLafManager->GetVirtualKeyRects( rects );
        
        TInt n = rects.Count(); //num of cells in one page
        for ( TInt i = 0; i < keys.Count(); i++ )
            {
            CVirtualKey* vk = keys[i];
            vk->SetRect( rects[i%n] );
            }
        rects.Reset();
        rects.Close();
        }

    //move virtual Sctpad 
    if ( iVirtualSctpadFix )
        {
        rect = iLafManager->CtrlRect( iVirtualSctpadFix->ControlId() );
        iVirtualSctpadFix->SetRect( rect );
        
        const RPointerArray<CVirtualKey>& keys = iVirtualSctpadFix->KeyArray();
        RArray<TRect> rects;
        iLafManager->GetFixVirtualKeyRects( rects );
        
        TInt n = rects.Count(); //
        for ( TInt i = 0; i < keys.Count(); i++ )
            {
            CVirtualKey* vk = keys[i];
            vk->SetRect( rects[i] );
            }
        rects.Reset();
        rects.Close();
        }
    
    //Move buttons
    TSize siBtnPadding = iLafManager->ButtonInnerPadding();
    TInt pdx = siBtnPadding.iWidth;
    TInt pdy = siBtnPadding.iHeight;
    
    rect = iLafManager->CtrlRect( iCloseBtn->ControlId() );
    MoveIconButton( iCloseBtn, rect, pdx, pdy, ETrue );
   
    rect = iLafManager->CtrlRect( iRangeSpecialBtn->ControlId() );
    MoveIconButton( iRangeSpecialBtn, rect,  pdx, pdy, ETrue );
    
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
    
    rect = iLafManager->CtrlRect( iImeSwitchBtn->ControlId() );
    MoveIconButton( iImeSwitchBtn,    rect,  pdx, pdy, ETrue );
    
    rect = iLafManager->CtrlRect( iSctPage0Btn->ControlId() );
    MoveSwitchPageButton( iSctPage0Btn, rect,  pdx, pdy, ETrue );
    
    rect = iLafManager->CtrlRect( iSctPage1Btn->ControlId() );
    MoveSwitchPageButton( iSctPage1Btn, rect,  pdx, pdy, ETrue );

    rect = iLafManager->CtrlRect( iSctPage2Btn->ControlId() );
    MoveSwitchPageButton( iSctPage2Btn, rect,  pdx, pdy, ETrue );
    
    //save range buttons position
    iRectOfRangeBtn = iLafManager->CtrlRect( iRangeSelector->ControlId() ); 
    
    //move range selector
    iRangeSelector->SetRect( iRectOfRangeBtn );
    iRangeSelector->SetBaseRect( iRectOfRangeBtn );
    }


// ---------------------------------------------------------------------------
//  layout for chinese range standby.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::LayoutStandbyChinese()
    {
    
    iCandidateList->Hide( ETrue );
    iPredictList->Hide( ETrue );
    
#ifndef RD_INTELLIGENT_TEXT_INPUT
    iFixEngSymbolList1->Hide( ETrue );
    iFixEngSymbolList2->Hide( ETrue );
#endif

    iWritingBox->Hide( EFalse );
    iVirtualNumpad->Hide( ETrue );
    iVirtualSctpad->Hide( ETrue );
    iVirtualSctpadFix->Hide( ETrue );
    
    iArrowLeftBtn->Hide( EFalse );
    iArrowRightBtn->Hide( EFalse );
    iArrowUpBtn->Hide( EFalse );
    iArrowDownBtn->Hide( EFalse );
    iOptionBtn->Hide( EFalse );
    iImeSwitchBtn->Hide( EFalse );    
    
    iSctPage0Btn->Hide( ETrue );
    iSctPage1Btn->Hide( ETrue );
    iSctPage2Btn->Hide( ETrue );
    
    iRangeSpecialBtn->SetHighlight( EFalse );
    
    UpdateAllVirtualKeysFeedback( EFingerHwrNativeRange );
    }

// ---------------------------------------------------------------------------
//  layout for english range standby.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::LayoutStandbyEnglish()
    {
    iCandidateList->Hide( ETrue );
    iPredictList->Hide( ETrue );
    
#ifndef RD_INTELLIGENT_TEXT_INPUT
    iFixEngSymbolList1->Hide( ETrue );
    iFixEngSymbolList2->Hide( ETrue );
#endif
   
    iWritingBox->Hide( EFalse );
    iVirtualNumpad->Hide( ETrue );
    iVirtualSctpad->Hide( ETrue );
    iVirtualSctpadFix->Hide( ETrue );
    
    iArrowLeftBtn->Hide( EFalse );
    iArrowRightBtn->Hide( EFalse );
    iArrowUpBtn->Hide( EFalse );
    iArrowDownBtn->Hide( EFalse );
    iOptionBtn->Hide( EFalse );
    iImeSwitchBtn->Hide( EFalse );      

    iSctPage0Btn->Hide( ETrue );
    iSctPage1Btn->Hide( ETrue );
    iSctPage2Btn->Hide( ETrue );
    
    iRangeSpecialBtn->SetHighlight( EFalse );
    
    UpdateAllVirtualKeysFeedback( EFingerHwrEnglishRange );
    }

// ---------------------------------------------------------------------------
//  layout for number range standby.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::LayoutStandbyNumeric()
    {
    iCandidateList->Hide( ETrue );
    iPredictList->Hide( ETrue );
    
#ifndef RD_INTELLIGENT_TEXT_INPUT    
    iFixEngSymbolList1->Hide( ETrue );
    iFixEngSymbolList2->Hide( ETrue );
#endif
   
    iWritingBox->Hide( ETrue );
    iVirtualNumpad->Hide( EFalse );
    iVirtualSctpad->Hide( ETrue );
    iVirtualSctpadFix->Hide( ETrue );
    
    iArrowLeftBtn->Hide( EFalse );
    iArrowRightBtn->Hide( EFalse );
    iArrowUpBtn->Hide( EFalse );
    iArrowDownBtn->Hide( EFalse );
    iOptionBtn->Hide( EFalse );
    iImeSwitchBtn->Hide( EFalse );

    iSctPage0Btn->Hide( ETrue );
    iSctPage1Btn->Hide( ETrue );
    iSctPage2Btn->Hide( ETrue );
    
    iRangeSpecialBtn->SetHighlight( EFalse );
    
    UpdateAllVirtualKeysFeedback( EFingerHwrNumberRange );
    }

// ---------------------------------------------------------------------------
//  layout for symbol range standby.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::LayoutStandbySymbol()
    {
    iCandidateList->Hide( ETrue );
    iPredictList->Hide( ETrue );
    
#ifndef RD_INTELLIGENT_TEXT_INPUT
    iFixEngSymbolList1->Hide( ETrue );
    iFixEngSymbolList2->Hide( ETrue );
#endif
   
    iWritingBox->Hide( ETrue );
    iVirtualNumpad->Hide( ETrue );
    iVirtualSctpad->Hide( EFalse );
    iVirtualSctpadFix->Hide( EFalse );
    
    iArrowLeftBtn->Hide( ETrue );
    iArrowRightBtn->Hide( ETrue );
    iArrowUpBtn->Hide( ETrue );
    iArrowDownBtn->Hide( ETrue );
    iOptionBtn->Hide( EFalse );
    iImeSwitchBtn->Hide( EFalse );  
    
    iRangeSpecialBtn->SetHighlight( ETrue );

    TInt pageno = ( iCurCharRangeNoSct == EFingerHwrNativeRange ) ? 0 : 1;
    ShowSctPage( pageno, EPagePosPageNo );
    
    UpdateAllVirtualKeysFeedback( EFingerHwrSymbolRange );
    
    UiLayout()->RootControl()->ReDrawRect( iVirtualSctpadFix->Rect() );
    UpdateArea( iVirtualSctpadFix->Rect() );
    }


// ---------------------------------------------------------------------------
//  EventButton layout helper. Move button to specified rect.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::MoveIconButton( CAknFepCtrlEventButton* aButton, 
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
//  EventButton layout helper. 
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::MoveSwitchPageButton( CAknFepCtrlEventButton* aButton, 
    const TRect& aRect, TInt aXPadding, TInt aYPadding, TBool aReloadImages )
    {
    if ( !aButton )
        {
        return;
        }
    
    aButton->SetRect( aRect );
    TRect rcInner = aRect;

    rcInner.Shrink( aXPadding + 5, aYPadding );
    aButton->SizeChanged( aRect, rcInner, aReloadImages );
    }    


// ---------------------------------------------------------------------------
//  Read control's shadow info.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::ReadShadowInfoL( const TInt aResId )
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
void CPeninputFingerHwrWnd::ReadBackgroundInfoL( const TInt aResId )
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
void CPeninputFingerHwrWnd::ReadICFInfoL( const TInt aResId )
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
void CPeninputFingerHwrWnd::ReadWritingBoxInfoL( const TInt aResId )
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
    
    iWritingBoxBkColor = boxcolor;
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
    
    iWritingBoxFrameColor = boxframecolor;
    iWritingBox->SetFrameCol( boxframecolor );

    CleanupStack::PopAndDestroy(); // reader
    
    ReloadWritingGuideLinesL();
    }
    
// ---------------------------------------------------------------------------
//  update feedback state of all virtual keys.
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrWnd::UpdateAllVirtualKeysFeedback( TInt aType )
    {
    if ( !UiLayout()->SupportTactileFeedback() ) 
        {
        // return ; 
        // CFepUiLayout::OnActivate() is incorrect
        // SupportTactileFeedback return is false when touch is about to active
        }
    
    TBool isNumpad = ( aType == EFingerHwrNumberRange ) ? ETrue : EFalse;
    TBool isSctpad = ( aType == EFingerHwrSymbolRange ) ? ETrue : EFalse;
    
    //update numpad keys
    TInt keyCount = iVirtualNumpad->KeyArray().Count();
    for ( TInt i = 0; i < keyCount; i++ )
        {
        CVirtualKey* vk = iVirtualNumpad->KeyArray()[i];
        TBool enable = !vk->Dimmed() &&  isNumpad;
        iVirtualNumpad->EnableKeyFeedback( vk, enable );
        }
    
    //update sctpad keys
    keyCount = iVirtualSctpad->KeyArray().Count();
    TInt pageSize = iVirtualSctpad->PageSize();
    TInt curPage = iVirtualSctpad->CurPageIndex();
    for ( TInt i = 0; i < keyCount; i++ )
        {
        CVirtualKey* vk = iVirtualSctpad->KeyArray()[i];
        TInt page = i / pageSize;
        TBool enable = !vk->Dimmed() &&  isSctpad && ( page == curPage );
        iVirtualSctpad->EnableKeyFeedback( vk, enable );
        }
    }


void CPeninputFingerHwrWnd::ReloadWritingGuideLinesL()
    {
    TInt resid = -1;
    
    if( iCurCharRange == EFingerHwrNativeRange )
        {
        resid = R_FINGER_HWR_GUIDELINE_CHS;
        }
    else if( iCurCharRange == EFingerHwrEnglishRange )
        {
        resid = R_FINGER_HWR_GUIDELINE_ENG;
        }
    else 
        {
        return;
        }

    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC( reader, resid );

    TPtrC fileName = reader.ReadTPtrC();
    TInt32 majorSkinId = reader.ReadInt32();
    TInt16 indexOfColor = reader.ReadInt16();
    
    TInt16 imageid = reader.ReadInt16();
    TInt16 maskid = reader.ReadInt16();
    TInt16 skinid = reader.ReadInt16();
    
    TAknsItemID id;
    id.Set( TInt( majorSkinId ), skinid );
    
    CFbsBitmap* bmpImage = NULL;
    CFbsBitmap* bmpMask = NULL;
    
    //AknsUtils::CreateIconL(AknsUtils::SkinInstance(), id, bmpImage, bmpMask, bmpFileName, imageid,maskid);
    
    TRgb colorBkgnd = iWritingBoxBkColor;
    TRgb colorGuideLine = AKN_LAF_COLOR( 0 ); 
    if ( ETrue )
        {
        TRgb color1 = colorBkgnd;
        TRgb color2 = colorGuideLine;
        
        TInt k = 64;
        TInt brightness = Max( color1.Red(), color1.Green() );
        brightness = Max( brightness, color1.Blue() );
        
        color2 = ( brightness > 128 ) ? TRgb(0,0,0) : TRgb(255,255,255);

        TInt r = ( color1.Red()  * ( 256 - k ) + color2.Red()   * k ) / 256;
        TInt g = ( color1.Green()* ( 256 - k ) + color2.Green() * k ) / 256;
        TInt b = ( color1.Blue() * ( 256 - k ) + color2.Blue()  * k ) / 256;

        colorGuideLine = TRgb(r,g,b);
        }
    
    AknsUtils::CreateColorIconL( AknsUtils::SkinInstance(),
            id, KAknsIIDQsnIconColors, -1,
            bmpImage, bmpMask, fileName, imageid, maskid,
            colorGuideLine );
    
    CleanupStack::PushL( bmpImage ); //1
    CleanupStack::PushL( bmpMask );  //2
    
    TSize imgsize = iLafManager->CtrlRect(EHwrCtrlIdWritingBox).Size();
    
    AknIconUtils::SetSize( bmpImage, imgsize, EAspectRatioNotPreserved );
    AknIconUtils::SetSize( bmpMask, imgsize, EAspectRatioNotPreserved );
    
    //prepare to merge
    CFbsBitmap* bmpMerged = new(ELeave) CFbsBitmap();
    CleanupStack::PushL( bmpMerged );   //3
    bmpMerged->Create( imgsize, BitmapDevice()->DisplayMode() );
    
    CFbsBitmapDevice* dev=CFbsBitmapDevice::NewL( bmpMerged );
    CleanupStack::PushL( dev );         //4
    
    CFbsBitGc* gc = CFbsBitGc::NewL();
    CleanupStack::PushL( gc );          //5
    
    //merge image and mask to dest
    TRect rcImage( TPoint(0,0), imgsize );
    gc->Activate( dev );
    gc->SetPenStyle( CGraphicsContext::ENullPen );
    gc->SetBrushColor( colorBkgnd );
    gc->SetBrushStyle( CGraphicsContext::ESolidBrush );
    gc->DrawRect( rcImage );
    gc->SetBrushStyle( CGraphicsContext::ENullBrush );
    gc->BitBltMasked( TPoint(0,0), bmpImage, rcImage, bmpMask, EFalse );
    
    CleanupStack::PopAndDestroy( 2 ); //gc, dev
    
    iWritingBox->SetBackgroundBitmapL( bmpMerged );
    iWritingBox->SetBackgroundMaskBitmapL( bmpMask );
    
    CleanupStack::Pop( 2 );           //bmpMerged, bmpMask
    CleanupStack::PopAndDestroy( 1 ); //bmpImage
    CleanupStack::PopAndDestroy();    //reader
    
    iWritingBox->ReDrawRect( iWritingBox->Rect() );
    iWritingBox->UpdateArea( iWritingBox->Rect() );    
    }

void CPeninputFingerHwrWnd::ShowBubble(TInt aShow)
    {
    if (aShow > 0)
        {
    	TRect outrect,innerrect;          
        
        
        
        
        iCandidateList->ShowBubble(ETrue);
        iPredictList->ShowBubble(ETrue);
         
#ifndef RD_INTELLIGENT_TEXT_INPUT    
        iFixEngSymbolList1->ShowBubble(ETrue);
        iFixEngSymbolList2->ShowBubble(ETrue);
#endif
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

        iPredictList->SetBubbleBitmapParam(NULL,NULL,KAknsIIDQsnFrInputCharPreview);
    	iPredictList->SetTextFormat(iLafManager->PreviewBubbleTextLayout());
    	iPredictList->SetBubbleTextFont(iLafManager->PreviewBubbleFont());
    	iPredictList->SetBubbleSize(TSize(outrect.Width(),outrect.Height()));  // Read laf
    	iPredictList->SetFrameDiff(innerrect.iTl.iX - outrect.iTl.iX,
    	               innerrect.iTl.iY - outrect.iTl.iY,
    	               outrect.iBr.iX - innerrect.iBr.iX,
    	               outrect.iBr.iY - innerrect.iBr.iY);

#ifndef RD_INTELLIGENT_TEXT_INPUT    
        iFixEngSymbolList1->SetBubbleBitmapParam(NULL,NULL,KAknsIIDQsnFrInputCharPreview);
    	iFixEngSymbolList1->SetTextFormat(iLafManager->PreviewBubbleTextLayout());
    	iFixEngSymbolList1->SetBubbleTextFont(iLafManager->PreviewBubbleFont());
    	iFixEngSymbolList1->SetBubbleSize(TSize(outrect.Width(),outrect.Height()));  // Read laf
    	iFixEngSymbolList1->SetFrameDiff(innerrect.iTl.iX - outrect.iTl.iX,
    	               innerrect.iTl.iY - outrect.iTl.iY,
    	               outrect.iBr.iX - innerrect.iBr.iX,
    	               outrect.iBr.iY - innerrect.iBr.iY);

        iFixEngSymbolList2->SetBubbleBitmapParam(NULL,NULL,KAknsIIDQsnFrInputCharPreview);
    	iFixEngSymbolList2->SetTextFormat(iLafManager->PreviewBubbleTextLayout());
    	iFixEngSymbolList2->SetBubbleTextFont(iLafManager->PreviewBubbleFont());
    	iFixEngSymbolList2->SetBubbleSize(TSize(outrect.Width(),outrect.Height()));  // Read laf
    	iFixEngSymbolList2->SetFrameDiff(innerrect.iTl.iX - outrect.iTl.iX,
    	               innerrect.iTl.iY - outrect.iTl.iY,
    	               outrect.iBr.iX - innerrect.iBr.iX,
    	               outrect.iBr.iY - innerrect.iBr.iY);
#endif
 
        }
    else
        {
        iCandidateList->ShowBubble(EFalse);
        iPredictList->ShowBubble(EFalse);
#ifndef RD_INTELLIGENT_TEXT_INPUT    
        iFixEngSymbolList1->ShowBubble(EFalse);
        iFixEngSymbolList2->ShowBubble(EFalse);
#endif             
        }
    }    
//  End Of File
