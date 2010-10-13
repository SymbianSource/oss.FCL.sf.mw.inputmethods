/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for button base and dragbar
*
*/

#include <s32mem.h> 
#include <AknsConstants.h>
#include <AknUtils.h>
#include <AknsDrawUtils.h>
#include <AknBidiTextUtils.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <barsread.h>
#include <peninputserver.rsg>
#include <StringLoader.h> 
#include <peninputcmdparam.h>

#include "aknfeppeninputenums.h"
#include "penuicandidatewnd.h"
#include "penuiwndeventobserver.h"

/**
 *  The capture type of pointing on canidate list.
 */
enum TCandWndCaptureType
    {
    ECandWndCapNone,
    ECandWndCapItem,
    ECandWndCapPageUp,
    ECandWndCapPageDown,
    ECandWndCapOther,
    ECandWndCapOut
    };

const TInt KInvalidBmp = -1 ;
const TInt KInvalidColorGroup = -1;

const TInt KOnePageItemCount = 4;
const TInt KTextRectMargin = 4;
const TInt KDisplayTextLen = KMaxItemTextLength + KAknBidiExtraSpacePerLine;

const TInt KPaneHorMarginCount = 2;
const TInt KTotalHorMarginCount = 4;

const TInt KTransparency[256] =
    { 
    0, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 7, 7, 7,
    8, 8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 11, 11, 12, 12, 12, 13, 13, 13,
    14, 14, 14, 14, 15, 15, 15, 16, 16, 16, 17, 17, 17, 17, 18, 18, 18, 19,
    19, 19, 20, 20, 20, 20, 21, 21, 21, 22, 22, 22, 23, 23, 23, 23, 24, 24,
    24, 25, 25, 25, 26, 26, 26, 26, 27, 27, 27, 28, 28, 28, 29, 29, 29, 29,
    30, 30, 30, 31, 31, 31, 32, 32, 32, 32, 33, 33, 33, 34, 34, 34, 35, 35,
    35, 35, 36, 36, 36, 37, 37, 37, 38, 38, 38, 38, 39, 39, 39, 40, 40, 40,
    41, 41, 41, 41, 42, 42, 42, 43, 43, 43, 44, 44, 44, 44, 45, 45, 45, 46,
    46, 46, 47, 47, 47, 47, 48, 48, 48, 49, 49, 49, 50, 50, 50, 50, 51, 51,
    51, 52, 52, 52, 53, 53, 53, 53, 54, 54, 54, 55, 55, 55, 56, 56, 56, 56,
    57, 57, 57, 58, 58, 58, 59, 59, 59, 59, 60, 60, 60, 61, 61, 61, 62, 62,
    62, 62, 63, 63, 63, 64, 64, 64, 65, 65, 65, 65, 66, 66, 66, 67, 67, 67,
    68, 68, 68, 68, 69, 69, 69, 70, 70, 70, 71, 71, 71, 71, 72, 72, 72, 73,
    73, 73, 74, 74, 74, 74, 75, 75, 75, 76, 76, 76, 77, 77 
    };

const TInt KServerBufferSize = 8;

const TSize KInnerRectOffset = TSize( 4, 4 );
const TSize KHighlightInnerRectOffset = TSize( 7, 7 );
const TSize KPageButtonInnerRectOffset = TSize( 10, 10 );

const TInt KSeparatorLineHeight = 2;
const TInt KSeparatorLineYOffset = -4;

const TInt KLafLandscapeCandidateIndex = 3;
const TInt KLafPortraitCandidateIndex = 2;

const TInt KHalfNumber = 2;
const TInt KItemSelIntSize = 2;
const TInt KPageInfoIntSize = 2;

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CPenUiCandidateWnd* CPenUiCandidateWnd::NewL( MPenUiWndEventObserver* aObserver )
    {
    CPenUiCandidateWnd* self = CPenUiCandidateWnd::NewLC( aObserver );
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CPenUiCandidateWnd* CPenUiCandidateWnd::NewLC( MPenUiWndEventObserver* aObserver )
    {
    CPenUiCandidateWnd* self = new (ELeave) CPenUiCandidateWnd( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// Destructor.
// ---------------------------------------------------------------------------
//
CPenUiCandidateWnd::~CPenUiCandidateWnd()
    {
    iItemRects.Close();
    iItemList.ResetAndDestroy();
    
    DeleteAllBitmaps();
    
    delete iPageInfo;
    }

// ---------------------------------------------------------------------------
// Handle raw events of pen input.
// ---------------------------------------------------------------------------
//
TBool CPenUiCandidateWnd::HandleRawEventL( const TAny* aEventData )
    {
    if( !IsVisible() && iPointerArea == ECandWndCapNone )
        {
        return EFalse;
        }
    
    TRawEvent event = *((TRawEvent*)aEventData);
    switch( event.Type() )                
        {
        case TRawEvent::EPointerMove:
            {
            TPoint point = event.Pos() - PositionRelativeToScreen(); 
            HandlePointerMoveEvent( point );
            return ETrue;
            }
        case TRawEvent::EButton1Down:
            {
            TPoint point = event.Pos() - PositionRelativeToScreen(); 
            HandlePointerDownEvent( point );
            return ETrue;
            }
        case TRawEvent::EButton1Up:
            {
            HandlePointerUpEventL();
            return ETrue;
            }
        default:
            break;                    
        }    
    return EFalse;
    }

// --------------------------------------------------------------------------
// Show candidate list
// --------------------------------------------------------------------------
//
void CPenUiCandidateWnd::ShowCandidateListL( const CDesCArray* aItemArray,
                                             TInt /*aActiveIndex*/ )
    {
    // Set the current item array.
    SetItemArrayL( aItemArray );
    
    // Set current page to the first page
    iCurrentPage = 0;
    
    // Calculate total page
    iTotalPages = ( iItemList.Count() + KOnePageItemCount - 1) / KOnePageItemCount;
    
    // Set the current page info text
    GetPageInfoBufL();
    
    // Clear focused item by default. 
    iFocusItem = KErrNotFound;

    // Capture all pointers from full screen.
    SetCapturePointer( ETrue );
    
    // Calculate the displaying rect.
    ReCalcLayout();

    if( !IsVisible() )
        {
        MakeVisible( ETrue );
        }
    
    DrawDeferred();
    }

// ---------------------------------------------------------------------------
// Hide candidate list.
// ---------------------------------------------------------------------------
//
void CPenUiCandidateWnd::HideCandidateList()
    {
    if ( IsVisible())
        {
        MakeVisible( EFalse );
        SetCapturePointer( EFalse );
        }
    }

// ---------------------------------------------------------------------------
// Handles a change to the control's resources. 
// ---------------------------------------------------------------------------
//
void CPenUiCandidateWnd::HandleResourceChange( TInt aType )
    {
    CCoeControl::HandleResourceChange( aType );
    HideCandidateList();
    switch ( aType )
        {
        case KEikDynamicLayoutVariantSwitch:
            {
            ReadLafInfo();
            }
            break;
        case KAknsMessageSkinChange:
            {
            TRAP_IGNORE( HandleSkinChangeL() );
            }
            break;
        default:
            break;
        }
    }

// ---------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that might leave.
// ---------------------------------------------------------------------------
//
CPenUiCandidateWnd::CPenUiCandidateWnd( MPenUiWndEventObserver* aObserver ) 
                        : iNaviBtnShown( ETrue ), 
                          iObserver( aObserver )
    {
    }

// ---------------------------------------------------------------------------
// 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void CPenUiCandidateWnd::ConstructL()
    {
    CreateWindowL();
    
    SetImgFrId( KAknsIIDQsnFrPopupSub, 
                KAknsIIDQsnFrFunctionButtonNormal,
                KAknsIIDQsnFrFunctionButtonPressed,
                KAknsIIDQsnFrFunctionButtonInactive,                
                KAknsIIDQsnFrList );

    ConstructFromResourceL();
    
    ReadLafInfo();
    
    SetBitmapSize(); 
     
    CreateDimmedMaskL( iPageUpDimBmpMask, iPageUpBmpMask );
    CreateDimmedMaskL( iPageDownDimBmpMask, iPageDownBmpMask );
    
    ActivateL();
    
    MakeVisible( EFalse );
    }

// ---------------------------------------------------------------------------
// Set all kinds of frame ids.
// ---------------------------------------------------------------------------
//
void CPenUiCandidateWnd::SetImgFrId( TAknsItemID aBgFrId, 
                                     TAknsItemID aNaviFrId,
                                     TAknsItemID aNaviActiveFrId,
                                     TAknsItemID aNaviDimFrId,                                    
                                     TAknsItemID aItemActiveFrId )
    {
    iBgFrId = aBgFrId;    
    iItemActiveFrId = aItemActiveFrId;
    
    // Page button
    iNaviFrId = aNaviFrId;
    iNaviActiveFrId = aNaviActiveFrId;
    iNaviDimFrId = aNaviDimFrId;
    }
 
// ---------------------------------------------------------------------------
// Construct candidate list from resource.
// ---------------------------------------------------------------------------
//
void CPenUiCandidateWnd::ConstructFromResourceL()
    {
    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC( reader, 
                                               R_PENINPUT_SERVER_CANDIDATE_LIST );
    
    // Get bitmap file name
    TPtrC bmpFileName = reader.ReadTPtrC();
    
    // Get major skin id
    TInt imgMajorSkinId = reader.ReadInt32();
    
    // Get up arrow bitmap resource id
    TInt32 pageUpId = reader.ReadInt32();
    
    // Get down arrow bitmap resource id
    TInt32 pageDownId = reader.ReadInt32();         
    
    // Get seperation line bitmap id and mask id
    const TInt16 bmpId = reader.ReadInt16();
    const TInt16 bmpMskId = reader.ReadInt16();
    
    // Get seperation line skin item id
    TAknsItemID id;
    const TInt skinitemid = reader.ReadInt16();
    id.Set(TInt(imgMajorSkinId), skinitemid);
    
    // Get seperation line bitmap and mask bitmap
    if (bmpId != KInvalidBmp)
        {
        if (bmpMskId != KInvalidBmp)
            {
            AknsUtils::CreateColorIconL( AknsUtils::SkinInstance(),
                                         id,
                                         KAknsIIDQsnIconColors,
                                         EAknsCIQsnIconColorsCG30,
                                         iSeperationBmp,
                                         iSeperationMaskBmp,
                                         bmpFileName,
                                         bmpId,
                                         bmpMskId,
                                         AKN_LAF_COLOR( 0 ) );                                 
            }
        else
            {
            AknsUtils::CreateIconL( AknsUtils::SkinInstance(),
                                    id,
                                    iSeperationBmp,
                                    bmpFileName,
                                    bmpId );
            }
        }
    
    CleanupStack::PopAndDestroy(); // reader
    
    // Construct page up button from resource
    TResourceReader pageUpReader;
    CCoeEnv::Static()->CreateResourceReaderLC( pageUpReader, pageUpId );
    ConstructPageInfoFromResourceL ( pageUpReader, ETrue );
    CleanupStack::PopAndDestroy(); // pageUpReader

    // Construct page down button from resource
    TResourceReader pageDownReader;
    CCoeEnv::Static()->CreateResourceReaderLC( pageDownReader, pageDownId );
    ConstructPageInfoFromResourceL( pageDownReader, EFalse );
    CleanupStack::PopAndDestroy(); // pageDownReader
    }

// -----------------------------------------------------------------------------
// Construct up / down arrow buttons from resource.
// -----------------------------------------------------------------------------
//
void CPenUiCandidateWnd::ConstructPageInfoFromResourceL( TResourceReader& aReader, 
                                                         TBool aUpDown )
    {
    // Get bimap file name of arrow buttons
    TPtrC bmpFileName = aReader.ReadTPtrC();
    
    // Get major skin id
    TInt imgMajorSkinId = aReader.ReadInt32();
    
    // Get color index
    TInt colorIndex = aReader.ReadInt16();    
    
    // Get the image ids and mask ids from resource
    TInt bmpId = aReader.ReadInt16(); 
    TInt bmpMskId = aReader.ReadInt16();
    
    // Get skin item id
    const TInt skinitemid = aReader.ReadInt16();
    TAknsItemID id;
    id.Set( imgMajorSkinId, skinitemid );
    
    // Get bitmaps
    CFbsBitmap* forgroundBmp = NULL;
    CFbsBitmap* forgroundBmpMask = NULL;
    if ( bmpId != KInvalidBmp )
        {
        if (bmpMskId != KInvalidBmp)
            {
            if( colorIndex == KInvalidColorGroup )
                {
                AknsUtils::CreateIconL( AknsUtils::SkinInstance(),
                                        id,
                                        forgroundBmp,
                                        forgroundBmpMask,
                                        bmpFileName,
                                        bmpId,
                                        bmpMskId );
                }
            else
                {
                AknsUtils::CreateColorIconL( AknsUtils::SkinInstance(),
                                             id,
                                             KAknsIIDQsnIconColors,
                                             colorIndex,
                                             forgroundBmp,
                                             forgroundBmpMask,
                                             bmpFileName,
                                             bmpId,
                                             bmpMskId,
                                             AKN_LAF_COLOR( 0 ) );       
                }
            }
        else
            {
            AknsUtils::CreateIconL( AknsUtils::SkinInstance(),
                                    id,
                                    forgroundBmp,
                                    bmpFileName,
                                    bmpId );
            }
        }
    
    if ( aUpDown )
        {
        iPageUpBmp = forgroundBmp;
        iPageUpBmpMask = forgroundBmpMask;
        }
    else
        {
        iPageDownBmp = forgroundBmp;
        iPageDownBmpMask = forgroundBmpMask;
        }
    }

// ---------------------------------------------------------------------------
// Read LAF data for candidate list
// ---------------------------------------------------------------------------
//        
void CPenUiCandidateWnd::ReadLafInfo()
    {
    // Screen
    TRect rect, rectcn;
    AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EScreen, rect);
    TBool isLandScape = rect.Size().iWidth < rect.Size().iHeight ? EFalse : ETrue;
    
    TAknWindowLineLayout appWnd; 
    appWnd = AknLayoutScalable_Avkon::application_window(0).LayoutLine();
    
    TAknWindowLineLayout parentWnd;
    TAknWindowLineLayout candidateList;
    TAknWindowLineLayout listpane;
    TAknWindowLineLayout scrollpane;
    if (isLandScape)
        {
        parentWnd = AknLayoutScalable_Apps::popup_vitu2_window(1).LayoutLine();
        candidateList = AknLayoutScalable_Apps::popup_vitu2_match_list_window
                                                ( KLafLandscapeCandidateIndex )
                                                .LayoutLine();
        listpane = AknLayoutScalable_Apps::list_vitu2_match_list_pane(1).LayoutLine();
        scrollpane = AknLayoutScalable_Apps::vitu2_page_scroll_pane(1).LayoutLine();
        }
    else
        {
        parentWnd = AknLayoutScalable_Apps::popup_vitu2_window(0).LayoutLine();
        candidateList = AknLayoutScalable_Apps::popup_vitu2_match_list_window
                                                ( KLafPortraitCandidateIndex )
                                                .LayoutLine();
        listpane = AknLayoutScalable_Apps::list_vitu2_match_list_pane(0).LayoutLine();
        scrollpane = AknLayoutScalable_Apps::vitu2_page_scroll_pane(0).LayoutLine();
        }
    
    TAknLayoutRect parentRectLayout;
    parentRectLayout.LayoutRect( appWnd.Rect(), parentWnd );
    TRect parentRect = parentRectLayout.Rect();
    
    // candidate list
    TAknLayoutRect candidateListRect;
    candidateListRect.LayoutRect( parentRect, candidateList );    
 
    // list pane, its parent is candidate list
    TAknLayoutRect listpaneRect;
    listpaneRect.LayoutRect( candidateListRect.Rect(), listpane );
    
    // scroll pane, its parent is candidate list
    TAknLayoutRect scrollRect;
    scrollRect.LayoutRect( candidateListRect.Rect(), scrollpane );
    
    // Item pane, its parent is list pane
    TAknWindowLineLayout itemPane 
          = AknLayoutScalable_Apps::list_vitu2_match_list_item_pane( 0, 0, 0 )
            .LayoutLine();
    TAknLayoutRect itemPaneRect;
    itemPaneRect.LayoutRect( listpaneRect.Rect(), itemPane );

    iLafData.iHorMargin = listpaneRect.Rect().iTl.iX;    
    iLafData.iVerMargin = listpaneRect.Rect().iTl.iY;        
    iLafData.iItemSize = itemPaneRect.Rect().Size();
    
    // Up button image. its parent is scroll pane
    TAknWindowLineLayout arrowUpImagePane
                    = AknLayoutScalable_Apps::vitu2_page_scroll_pane_g1()
                      .LayoutLine();
    TAknLayoutRect arrowUpImageRect;
    arrowUpImageRect.LayoutRect( scrollRect.Rect(), arrowUpImagePane );
    // Up button, its parent is scroll pane
    TAknWindowLineLayout arrowUpPane
              = AknLayoutScalable_Apps::bg_button_pane_cp023().LayoutLine();
    TAknLayoutRect arrowUpRect;
    arrowUpRect.LayoutRect( scrollRect.Rect(), arrowUpPane );
    
    // Down button image, its parent is scroll pane
    TAknWindowLineLayout arrowDownImagePane
            = AknLayoutScalable_Apps::vitu2_page_scroll_pane_g2().LayoutLine();
    TAknLayoutRect arrowDownImageRect;
    arrowDownImageRect.LayoutRect( scrollRect.Rect(), arrowDownImagePane );
    
    // Down button, its parent is scroll pane
    TAknWindowLineLayout arrowDownPane 
                = AknLayoutScalable_Apps::bg_button_pane_cp024().LayoutLine();
    TAknLayoutRect arrowDownRect;
    arrowDownRect.LayoutRect( scrollRect.Rect(), arrowDownPane );
    
    iLafData.iNaviSize = arrowUpRect.Rect().Size();
    iLafData.iNaviInnerSize = arrowUpImageRect.Rect().Size();
    iLafData.iTextLine 
               = AknLayoutScalable_Apps::list_vitu2_match_list_item_pane_t1()
                 .LayoutLine();
    iLafData.iPageTextLine 
               = AknLayoutScalable_Apps::vitu2_page_scroll_pane_t1()
                 .LayoutLine();
            
    TAknLayoutText list_item_pane_t1_layout_text;
    list_item_pane_t1_layout_text.LayoutText( itemPaneRect.Rect(), 
                                              iLafData.iTextLine );
    iLafData.iFont = list_item_pane_t1_layout_text.Font();
    }

// ---------------------------------------------------------------------------
// Set size for bitmaps
// ---------------------------------------------------------------------------
//
void CPenUiCandidateWnd::SetBitmapSize()
    {
    if ( iPageUpBmp )
        {
        AknIconUtils::SetSize( iPageUpBmp, 
                               iLafData.iNaviSize, 
                               EAspectRatioNotPreserved );        
        }
    
    if ( iPageUpBmpMask )
        {
        AknIconUtils::SetSize( iPageUpBmpMask, 
                               iLafData.iNaviSize, 
                               EAspectRatioNotPreserved );        
        }
    
    if ( iPageDownBmp )
        {
        AknIconUtils::SetSize( iPageDownBmp, 
                               iLafData.iNaviSize, 
                               EAspectRatioNotPreserved );
        }
    
    if ( iPageDownBmpMask )
        {
        AknIconUtils::SetSize( iPageDownBmpMask,
                               iLafData.iNaviSize, 
                               EAspectRatioNotPreserved );
        }
    
    if ( iPageUpDimBmpMask )
        {
        AknIconUtils::SetSize( iPageUpDimBmpMask, 
                               iLafData.iNaviSize, 
                               EAspectRatioNotPreserved );
        }
    
    if ( iPageDownDimBmpMask )
        {
        AknIconUtils::SetSize( iPageDownBmpMask,
                               iLafData.iNaviSize, 
                               EAspectRatioNotPreserved );    
        }    
    }

// ---------------------------------------------------------------------------
// Create mask bitmap for dimmed icons
// ---------------------------------------------------------------------------
//    
void CPenUiCandidateWnd::CreateDimmedMaskL( CFbsBitmap*& aDimmedMask,
                                            const CFbsBitmap* aMask )
    {
    if (aMask && aMask->DisplayMode() == EGray256 )
        {
        if( NULL != aDimmedMask )
            {
            delete aDimmedMask;
            aDimmedMask = NULL;
            }

        aDimmedMask = new (ELeave) CFbsBitmap;
        User::LeaveIfError( aDimmedMask->Create( aMask->SizeInPixels(), EGray256 ) ); 
        CleanupStack::PushL( aDimmedMask );
        CFbsBitmapDevice* bitmapDevice = CFbsBitmapDevice::NewL( aDimmedMask );
        CleanupStack::PushL( bitmapDevice );
        CFbsBitGc* bitGc( NULL );
        User::LeaveIfError( bitmapDevice->CreateContext( bitGc ) );
        CleanupStack::PushL( bitGc );
        bitGc->SetPenStyle( CGraphicsContext::ESolidPen );
        bitGc->BitBlt( TPoint( 0, 0 ), aMask );
        aDimmedMask->LockHeap();
        TInt w = aMask->SizeInPixels().iWidth; 
        TInt h = aMask->SizeInPixels().iHeight;
        TInt dataStride = aMask->DataStride() - w; 
        unsigned char* address = (unsigned char *)aDimmedMask->DataAddress();  
        for ( TInt i = 0; i < h; ++i )
            {
            for ( TInt j = 0; j < w; ++j )
                {
                *address = KTransparency[ *address ];
                ++address;
                }
            address += dataStride;         
            }
        aDimmedMask->UnlockHeap();
        CleanupStack::PopAndDestroy( bitGc ); // bitGc
        CleanupStack::PopAndDestroy( bitmapDevice ); // bitmapDevice
        CleanupStack::Pop( aDimmedMask ); // aDimmedMask
        }
    }

// --------------------------------------------------------------------------
// Capture all poninters of full screen when candidate list is opened.
// --------------------------------------------------------------------------
//
void CPenUiCandidateWnd::SetCapturePointer( TBool aFlag )
    {
    if ( iObserver )
        {
        TBuf<KServerBufferSize> buf;
        buf.Append( ( const TUint16* )( &aFlag ), sizeof( TBool ) / sizeof( TUint16 ) );
        buf.Append( ( const TUint16* )( this ), sizeof( TInt ) / sizeof( TUint16 ) );
        iObserver->SignalOwner( ESignalCapturePointer, buf );
        }
    }

// ---------------------------------------------------------------------------
// Get page info and transfer it to descritor.
// ---------------------------------------------------------------------------
//
void CPenUiCandidateWnd::GetPageInfoBufL()
    {
    if( NULL != iPageInfo )
        {
        delete iPageInfo;
        iPageInfo = NULL;
        }    
    CArrayFix<TInt>* pageNumbers = 
            new ( ELeave ) CArrayFixFlat<TInt>( KPageInfoIntSize );
    CleanupStack::PushL( pageNumbers );
    pageNumbers->AppendL( iCurrentPage + 1 );
    pageNumbers->AppendL( iTotalPages );
    iPageInfo = StringLoader::LoadL( R_PENINPUT_SERVER_CANDIDATE_LIST_PAGE_NUM, 
                                     *pageNumbers );
    CleanupStack::PopAndDestroy( pageNumbers ); // pageNumbers    
    }

// ---------------------------------------------------------------------------
// Set the text item array to candidate list.
// ---------------------------------------------------------------------------
//
void CPenUiCandidateWnd::SetItemArrayL( const CDesCArray* aItemArray )
    {    
    if ( aItemArray )
        {
        iItemList.ResetAndDestroy();
        for ( TInt i = 0; i < aItemArray->Count(); i++ )
            {
            CPenUiCandidateWnd::TPenUiCandidateItem* item 
                      = new (ELeave) CPenUiCandidateWnd::TPenUiCandidateItem;
            item->iCommand = ECandItemCmdItemSelected;
            if ( (*aItemArray)[i].Length() <= item->iText.MaxLength() )
                {
                item->iText.Copy( (*aItemArray)[i] );
                }
            else
                {
                // Given longer than maxlength, display the part of less than max
                item->iText.Copy( (*aItemArray)[i].Left( item->iText.MaxLength() ) );
                }            
            CleanupStack::PushL(item);
            iItemList.AppendL( item );
            CleanupStack::Pop( item ); // item
            }
        AddSpellItemL();
        }
    }

// ---------------------------------------------------------------------------
// Add spell item
// ---------------------------------------------------------------------------
//
void CPenUiCandidateWnd::AddSpellItemL()
    {
    if( iSpellEnabled && iItemList.Count() != 0 )
        {
        HBufC* spellText = StringLoader::LoadLC( R_PENINPUT_SERVER_FINGER_SPELL );    
        CPenUiCandidateWnd::TPenUiCandidateItem* item 
                    = new (ELeave) CPenUiCandidateWnd::TPenUiCandidateItem;
        item->iCommand = ECandItemCmdEnterSpellMode;
        item->iText.Copy( *spellText );        
        CleanupStack::PushL( item );
        iItemList.AppendL( item );
        CleanupStack::Pop( item ); // item        
        CleanupStack::PopAndDestroy( spellText ); // spellText
        }    
    }

// ---------------------------------------------------------------------------
// Calculate the displaying rect of candidate.
// ---------------------------------------------------------------------------
//
void CPenUiCandidateWnd::ReCalcLayout()
    {
    // Dynamically adjust width 
    TInt width = WidthToFitText();

    // Calculate the height of candidate
    TInt height = ( KOnePageItemCount + 1 ) * iLafData.iVerMargin 
                    + KOnePageItemCount * iLafData.iItemSize.iHeight;
    
    // Calulate the rect of candidate
    TRect rtCtrl( iInitRect.iTl, TSize( width, height ) );
    rtCtrl = RectToFitScreen( rtCtrl );
        
    SetRect( rtCtrl );    
    }

// ---------------------------------------------------------------------------
// Return the dynamical width for displaying text.
// ---------------------------------------------------------------------------
//
TInt CPenUiCandidateWnd::WidthToFitText()
    {
    if ( iLafData.iFont )
        {
        // Calculate the maximum of width among all text to be shown    
        iItemWidth = iTextWidthEnabled ? iLafData.iItemSize.iWidth : 0;    
        const CFont* font = iLafData.iFont;        
        CFont::TMeasureTextInput::TFlags flg 
                                   = CFont::TMeasureTextInput::EFVisualOrder;
        if( iAlign == CGraphicsContext::ERight )
            {
            flg = CFont::TMeasureTextInput::EFVisualOrderRightToLeft;
            }       
        for( TInt i = 0; i < iItemList.Count(); ++i )
            {
            TInt itemWidth = AknBidiTextUtils::MeasureTextBoundsWidth(
                                            *font, iItemList[i]->iText, flg );
            if(itemWidth > iItemWidth)
                {
                iItemWidth = itemWidth;
                }
            }
        }
    
    // Make sure the total width ( text item + navi width + all horiezental margin )
    // not to be larger than the screen width.
    TInt naviWidth = iNaviBtnShown ? iLafData.iNaviSize.iWidth : 0;
    TInt totalHorMargin = KTotalHorMarginCount * iLafData.iHorMargin;
    TInt totalWidth = iItemWidth + totalHorMargin + naviWidth;       
    TPixelsTwipsAndRotation ptScreenSize;             
    CCoeEnv::Static()->ScreenDevice()->
                       GetDefaultScreenSizeAndRotation( ptScreenSize ); 
    if( totalWidth > ptScreenSize.iPixelSize.iWidth )    
        {
        iItemWidth = ptScreenSize.iPixelSize.iWidth - naviWidth - totalHorMargin;
        totalWidth = iItemWidth + totalHorMargin + naviWidth;
        }
    return totalWidth;
    }
  
// ---------------------------------------------------------------------------
// Return the rect to fit the screen size.
// ---------------------------------------------------------------------------
//
TRect CPenUiCandidateWnd::RectToFitScreen( const TRect& aRect )
    {
    TPixelsTwipsAndRotation ptScreenSize;             
    CCoeEnv::Static()->ScreenDevice()->
                                GetDefaultScreenSizeAndRotation( ptScreenSize) ;    
    TRect rtScreen ( TPoint( 0, 0 ), ptScreenSize.iPixelSize );
    if( aRect.Width() > rtScreen.Width() || aRect.Height() > rtScreen.Height() )
        {
        // If aRect is larger than the screen area, it is no need to adjust.
        return aRect;
        }
       
    TRect rect = aRect;
    if( rect.iBr.iY > rtScreen.iBr.iY )
        {
        // There is no enough space below the underline word. 
        // If there is no enough space at the right and enough space 
        // at the left, candidate list pops up on the left.
        // Otherwise, candidate list pops up on the right.
        TInt verOffset = rtScreen.iBr.iY - rect.iBr.iY;
        if ( ( iInitRect.iBr.iX + rect.Width() > rtScreen.iBr.iX )
                && ( rect.iTl.iX - rect.Width() > rtScreen.iTl.iX ) )
            {
            rect.Move( -rect.Width(), verOffset );
            }
        else
            {
            rect.Move( iInitRect.iBr.iX - rect.iTl.iX, verOffset );
            }
        }
    if( rect.iBr.iX > rtScreen.iBr.iX )
        {
        rect.Move( rtScreen.iBr.iX - rect.iBr.iX , 0 );
        }
    if( rect.iTl.iX < rtScreen.iTl.iX )
        {
        rect.Move( rtScreen.iTl.iX - rect.iTl.iX , 0 );
        }        
    if( rect.iTl.iY < rtScreen.iTl.iY )
        {
        rect.Move( 0, rtScreen.iTl.iY - rect.iTl.iY );
        }
    return rect;
    }

// ---------------------------------------------------------------------------
// Draw each item.
// ---------------------------------------------------------------------------
//
void CPenUiCandidateWnd::DrawItem( CWindowGc* aGc, TInt aIndex ) const
    {
    if ( aIndex < 0 || aIndex >= iItemRects.Count() )
        {
        return;
        }
    
    const CFont* font = iLafData.iFont;
    if ( !font )
        {
        return;   
        }
    
    TInt itemIndex = aIndex + iCurrentPage * iItemRects.Count();
    if( itemIndex >= iItemList.Count() )
        {
        return;
        }
    
    // Get text rect
    TRect textRect = iItemRects[ aIndex ];
    textRect.SetWidth( iItemWidth );
    textRect.Move( iLafData.iHorMargin, 0 );
    
    // Draw seperation
    if ( ( itemIndex == iItemList.Count() - 1) && iSpellEnabled )
         {
         // Calculate seperation bmp rect
         TPoint separatorTl = textRect.iTl;
         separatorTl.iY += KSeparatorLineYOffset;  
         
         // Draw bitmap
         if( iSeperationBmp )
             {
             TRect srcRect( TPoint( 0, 0 ), iSeperationBmp->SizeInPixels() );
             if( iSeperationMaskBmp )
                 {
                 aGc->BitBltMasked( separatorTl, 
                                  iSeperationBmp, 
                                  srcRect,
                                  iSeperationMaskBmp,
                                  ETrue );
                 }
             else
                 {
                 aGc->BitBlt( separatorTl,
                            iSeperationBmp,
                            srcRect );
                 }
             }
         }     

    TAknsQsnTextColorsIndex clrIndex;
    clrIndex = ( itemIndex == iFocusItem ) ? EAknsCIQsnTextColorsCG10 
                                           : EAknsCIQsnTextColorsCG20;     
    TRgb color = KRgbBlack;
    if (AknsUtils::AvkonSkinEnabled())
         {
         AknsUtils::GetCachedColor( AknsUtils::SkinInstance(),
                                    color, 
                                    KAknsIIDQsnTextColors, 
                                    clrIndex );
         }
    
    TBuf<KDisplayTextLen> buf;
    if ( iAlign == CGraphicsContext::ERight )
        {
        AknBidiTextUtils::ConvertToVisualAndClip( iItemList[itemIndex]->iText, 
                                                  buf, 
                                                  *font,
                                                  textRect.Width(), 
                                                  textRect.Width(),
                                                  AknBidiTextUtils::ERightToLeft );
        }
    else
        {
        AknBidiTextUtils::ConvertToVisualAndClip( iItemList[itemIndex]->iText, 
                                                  buf, 
                                                  *font,
                                                  textRect.Width(), 
                                                  textRect.Width());
        }
    
    if ( textRect.Height() - font->FontMaxHeight() < KTextRectMargin )
        {
        // Make sure the height of the area for drawing text is larger than 
        // the max height of font by 4 pixels at least.
        // The piece of code is used to avoid clipping some characters, like 'g'.
        textRect.SetHeight( font->FontMaxHeight() + KTextRectMargin );
        }
    
    // Draw highlight item
    if ( itemIndex == iFocusItem )
        {                 
        MAknsSkinInstance* skin = AknsUtils::SkinInstance();
        TRect hightLightRect = iItemRects[ aIndex ];
        TRect innerHightlightRect = hightLightRect;
        innerHightlightRect.Shrink( KHighlightInnerRectOffset );
        AknsDrawUtils::DrawFrame( skin, 
                                  *aGc, 
                                  hightLightRect,
                                  innerHightlightRect,
                                  iItemActiveFrId,
                                  KAknsIIDDefault ); 
        }
    
    // Draw text
    aGc->UseFont( font );
    aGc->SetPenColor( color );
    TInt baseLine = textRect.Height() / KHalfNumber 
                    + font->AscentInPixels() / KHalfNumber;
    aGc->DrawText( buf, textRect, baseLine, iAlign );
    aGc->DiscardFont();    
    }

// ---------------------------------------------------------------------------
// Draw page down / up arrow button.
// ---------------------------------------------------------------------------
//
void CPenUiCandidateWnd::DrawPageButton( CWindowGc* aGc, TBool aUpDown ) const
    {
    TRect buttonRect;
    CFbsBitmap* buttonBmp = NULL;
    CFbsBitmap* buttonBmpMask = NULL;
    CFbsBitmap* buttonDimBmpMask = NULL;
    if ( aUpDown )
        {
        buttonBmp = iPageUpBmp;
        buttonBmpMask = iPageUpBmpMask;
        buttonDimBmpMask = iPageUpDimBmpMask;
        buttonRect = iPageUpRect;        
        }
    else
        {
        buttonBmp = iPageDownBmp;
        buttonBmpMask = iPageDownBmpMask;
        buttonDimBmpMask = iPageDownDimBmpMask;
        buttonRect = iPageDownRect;
        }
    
    // Inner rect
    TRect innerRect = buttonRect;
    innerRect.Shrink( KPageButtonInnerRectOffset );
        
    // Draw arrow buttons
    CFbsBitmap* bmpMask = NULL;    
    if( iTotalPages == 1 ) 
        {
        // Draw dim page buttons.
        AknsDrawUtils::DrawFrame( AknsUtils::SkinInstance(), 
                                  *aGc, 
                                  buttonRect, 
                                  innerRect,
                                  iNaviDimFrId,
                                  KAknsIIDDefault );

        bmpMask = buttonDimBmpMask; 
        }
    else
        {
        // Draw normal state or pressed state
        TBool isActive = ( aUpDown && iPointerArea == ECandWndCapPageUp ) 
                         || ( !aUpDown && iPointerArea == ECandWndCapPageDown ); 
        TAknsItemID ImgID = isActive ? iNaviActiveFrId : iNaviFrId;
        AknsDrawUtils::DrawFrame( AknsUtils::SkinInstance(), 
                                  *aGc, 
                                  buttonRect, 
                                  innerRect,
                                  ImgID,
                                  KAknsIIDDefault ); 
        
        bmpMask = buttonBmpMask;                            
        }
    
    // Draw forground
    if( buttonBmp )
        {
        TRect srcRect( TPoint( 0, 0 ), buttonBmp->SizeInPixels() );
        if( bmpMask )
            {
            aGc->BitBltMasked( buttonRect.iTl, buttonBmp, srcRect,
                               bmpMask, EFalse );
            }
        else
            {
            aGc->BitBlt( buttonRect.iTl, buttonBmp, srcRect );    
            }
        }
    }

// ---------------------------------------------------------------------------
// Draw page into text.
// ---------------------------------------------------------------------------
//
void CPenUiCandidateWnd::DrawPageInfo( CWindowGc* aGc ) const
    {
    if ( iPageInfo->Length() == 0)
        {
        return;    
        }
    
    TRgb color( KRgbBlack );  // sane default for nonskinned case
    TAknsQsnTextColorsIndex clrIndex;
    clrIndex = EAknsCIQsnTextColorsCG20;
    if ( AknsUtils::AvkonSkinEnabled() )
        {
        AknsUtils::GetCachedColor( AknsUtils::SkinInstance(),
                                   color, 
                                   KAknsIIDQsnTextColors, 
                                   clrIndex );
        }

    TAknLayoutText textLayout;
    textLayout.LayoutText( iScrollRect, iLafData.iPageTextLine );
    textLayout.DrawText( *aGc, iPageInfo->Des(), EFalse, color );
    }

// ---------------------------------------------------------------------------
// Handle pointer down event. 
// ---------------------------------------------------------------------------
//
void CPenUiCandidateWnd::HandlePointerDownEvent( const TPoint& aPoint )
    {
    if ( Rect().Contains( aPoint ) )
        {               
        CheckPointerEvent ( aPoint );
        }
    else
        {
        iPointerArea = ECandWndCapOut;
        
        HideCandidateList();
        }    
    }

// ---------------------------------------------------------------------------
// Handle pointer move event. 
// ---------------------------------------------------------------------------
//
void CPenUiCandidateWnd::HandlePointerMoveEvent(const TPoint& aPoint)
    {
    if( !IsVisible() || iPointerArea == ECandWndCapNone )
        {
        return;
        }
        
    if ( !Rect().Contains( aPoint ) )
        {
        iPointerArea = ECandWndCapOut;
        return;
        }
    
    CheckPointerEvent ( aPoint );
    }

// ---------------------------------------------------------------------------
// Handle pointer up event. 
// ---------------------------------------------------------------------------
//
void CPenUiCandidateWnd::HandlePointerUpEventL()
    {
    if( !IsVisible() )
        {
        iPointerArea = ECandWndCapNone;
        return;
        }
    
    TBool needReDraw = EFalse ;
    switch ( iPointerArea )
        {
        case ECandWndCapItem:
            {            
            if ( iFocusItem != KErrNotFound 
                    && iFocusItem < iItemList.Count() )
                {                        
                ReportItemSelectedEventL();
                HideCandidateList();
                }
            }
            break;
        case ECandWndCapPageUp:
            {
            iCurrentPage = ( iCurrentPage == 0 ) ? iTotalPages 
                                                 : iCurrentPage;
            iCurrentPage --;            
            GetPageInfoBufL();
            
            needReDraw = ETrue;
            }
            break;
        case ECandWndCapPageDown:
            {
            iCurrentPage++;
            iCurrentPage = ( iCurrentPage == iTotalPages ) ? 0 
                                                           : iCurrentPage;
            GetPageInfoBufL();
            
            needReDraw = ETrue;
            }
            break;
        default:
            break;
        }
    
    iPointerArea = ECandWndCapNone;
    if ( needReDraw )
        {
        DrawDeferred();
        }
    }

// ---------------------------------------------------------------------------
// Check pointer area of pointer event. 
// ---------------------------------------------------------------------------
//
void CPenUiCandidateWnd::CheckPointerEvent( const TPoint& aPoint )
    {
    if( iListRect.Contains( aPoint ) )
        {
        TInt focus = ItemIndexByPoint( aPoint );
        if ( focus != KErrNotFound )
            {
            iPointerArea = ECandWndCapItem;
        
            // Enter text item
            if ( focus != iFocusItem  )
                {
                iFocusItem = focus;
                DrawDeferred();
                }
            }
        else
            {
            iPointerArea = ECandWndCapOther;
            }
        }
    else 
        {
        if ( iTotalPages > 1 )
            {
            if ( iPageUpRect.Contains( aPoint) )
                {
                // Enter page up button 
                if ( iPointerArea != ECandWndCapPageUp )
                    {
                    iPointerArea = ECandWndCapPageUp;
                    DrawDeferred();
                    }
                }
            else if ( iPageDownRect.Contains( aPoint ) )
                {
                // Enter page down button
                if ( iPointerArea != ECandWndCapPageDown )
                    {
                    iPointerArea = ECandWndCapPageDown;
                    DrawDeferred();
                    }
                }
            else
                {
                iPointerArea = ECandWndCapOther;
                }
            }
        else
            {
            iPointerArea = ECandWndCapOther;
            }
        }    
    }

// ---------------------------------------------------------------------------
// Return the index of pointed text item. 
// ---------------------------------------------------------------------------
//
TInt CPenUiCandidateWnd::ItemIndexByPoint( const TPoint& aPoint )
    {
    TInt ret = KErrNotFound;
    for ( TInt i = 0; i < iItemRects.Count(); i++ )
        {
        if ( iItemRects[ i ].Contains( aPoint ) )
            {
            ret = i  + iCurrentPage * iItemRects.Count();
            if ( ret >= iItemList.Count() ) 
                {
                ret = KErrNotFound;
                }
            break;
            }
        }
    return ret;
    }

// ---------------------------------------------------------------------------
// Report item selected event to ui layout. 
// ---------------------------------------------------------------------------
//
void CPenUiCandidateWnd::ReportItemSelectedEventL()
    {
    if ( iObserver && iFocusItem != KErrNotFound 
            && iFocusItem < iItemList.Count() )
        {
        // Report success select item
        TInt command = iItemList[ iFocusItem ]->iCommand;
        
        HBufC8* buf8 = HBufC8::NewLC( KItemSelIntSize * sizeof( TInt ) );
        TPtr8 buf8Ptr = buf8->Des();
        
        RDesWriteStream writeStream;
        CleanupClosePushL(writeStream);
        writeStream.Open( buf8Ptr ); 
        
        writeStream.WriteInt32L( command );
        writeStream.WriteInt32L( iFocusItem );
        
        writeStream.CommitL();                 
        CleanupStack::PopAndDestroy( &writeStream );
        iObserver->HandleCommand( ECmdPeninputSelectServerCandidate, 
                                  reinterpret_cast< TUint8* >( &buf8Ptr ) );
        
        CleanupStack::PopAndDestroy( buf8 );
        }
    }

// ---------------------------------------------------------------------------
// Delete all bitmaps. 
// ---------------------------------------------------------------------------
//
void CPenUiCandidateWnd::DeleteAllBitmaps()
    {
    delete iPageUpBmp;
    iPageUpBmp = NULL;
    
    delete iPageUpBmpMask;
    iPageUpBmpMask = NULL;
    
    delete iPageUpDimBmpMask;
    iPageUpDimBmpMask = NULL;
    
    delete iPageDownBmp;
    iPageDownBmp = NULL;
    
    delete iPageDownBmpMask;
    iPageDownBmpMask = NULL;
    
    delete iPageDownDimBmpMask;
    iPageDownDimBmpMask = NULL;
    
    delete iSeperationBmp;
    iSeperationBmp = NULL;
    
    delete iSeperationMaskBmp;
    iSeperationMaskBmp = NULL;
    }

// ---------------------------------------------------------------------------
// Handle skin change. 
// ---------------------------------------------------------------------------
//
void CPenUiCandidateWnd::HandleSkinChangeL()
    {
    // Reconstruct all bitmaps as these bitmaps are based on current skin. 
    DeleteAllBitmaps();
    
    ConstructFromResourceL();
    
    SetBitmapSize(); 
     
    CreateDimmedMaskL( iPageUpDimBmpMask, iPageUpBmpMask );
    CreateDimmedMaskL( iPageDownDimBmpMask, iPageDownBmpMask );
    
    // Set separator line size if the spell feature is enabled.
    if ( iSpellEnabled && iItemRects.Count() > 0 )
        {
        if( NULL != iSeperationBmp )
            {
            AknIconUtils::SetSize( iSeperationBmp, iSeparatorSize, 
                                   EAspectRatioNotPreserved ); 
            }
            
        if( NULL != iSeperationMaskBmp )
            {
            AknIconUtils::SetSize( iSeperationMaskBmp, iSeparatorSize, 
                                   EAspectRatioNotPreserved );
            }        
        }
    }

// ---------------------------------------------------------------------------
// Responds to changes to the size and position of the contents of this control.
// ---------------------------------------------------------------------------
//
void CPenUiCandidateWnd::SizeChanged()
    {
    TInt paneHorMargin = KPaneHorMarginCount * iLafData.iHorMargin;    
    
    // Calculate list pane rect
    iListRect = Rect();    
    iListRect.SetWidth( iItemWidth + paneHorMargin );        
    
    if( iNaviBtnShown )
        {
        // scroll pane width = naviButton width + 2 horMargin width 
        TInt scorllPaneWidth = iLafData.iNaviSize.iWidth + paneHorMargin;        
        TInt scrollPaneTlX = iListRect.Width();
        if ( iAlign == CGraphicsContext::ERight )
            {
            // If the layout is RTL, move list pane to the rightmost,
            // move scroll pane to the leftmost.
            iListRect.Move( scorllPaneWidth, 0 );
            scrollPaneTlX = 0;
            }
        
        // calculate scroll pane rect
        iScrollRect = TRect( TPoint( scrollPaneTlX, iListRect.iTl.iY ),
                             TSize( scorllPaneWidth, iListRect.Height() ) );
        
        // calculate up button rect
        TPoint tlPageUp( iScrollRect.iTl.iX + iLafData.iHorMargin, 
                         iScrollRect.iTl.iY + iLafData.iVerMargin );
        iPageUpRect = TRect( tlPageUp, iLafData.iNaviSize );
        
        // calculate down button rect
        TPoint tlPageDown( iPageUpRect.iTl.iX, 
                           iScrollRect.iBr.iY - iLafData.iVerMargin
                           - iLafData.iNaviSize.iHeight );
        iPageDownRect = TRect( tlPageDown, iLafData.iNaviSize );
        }
    
    // Calculate each the area of each text, not including the margin
    iItemRects.Reset();
    for ( TInt i = 0; i < KOnePageItemCount; i++ )
        {
        TPoint point( iListRect.iTl.iX , iListRect.iTl.iY + 
                      ( i + 1 ) * iLafData.iVerMargin + 
                      i * iLafData.iItemSize.iHeight );
        
        TSize size( iListRect.Width(), iLafData.iItemSize.iHeight );        
        iItemRects.Append( TRect( point, size ) );
        }

    // Calculate separator line rect if the spell feature is enabled.
    if ( iSpellEnabled && iItemRects.Count() > 0 )
        {
        TSize newSize ( iItemWidth, KSeparatorLineHeight );
        if ( newSize != iSeparatorSize )
            {
            // Resize seperation bitmaps
            if( iSeperationBmp )
                {
                AknIconUtils::SetSize( iSeperationBmp, newSize, 
                                        EAspectRatioNotPreserved  );                
                }
            
            if( iSeperationMaskBmp )
                {
                AknIconUtils::SetSize( iSeperationMaskBmp, newSize, 
                                        EAspectRatioNotPreserved  );                    
                }        

            iSeparatorSize = newSize;
            }        
        } 
    }

// ---------------------------------------------------------------------------
// Draw candidate control.
// ---------------------------------------------------------------------------
//
void CPenUiCandidateWnd::Draw( const TRect& /*aRect*/ ) const
    {
    if ( !IsVisible() || Rect().Size() == TSize(0,0))
        {
        return;
        }

    // Draw background of whole control
    CWindowGc& gc = SystemGc();       
    TRect listRect = Rect();
    gc.Clear( listRect );

    TRect innerRect = listRect;
    innerRect.Shrink( KInnerRectOffset );
    AknsDrawUtils::DrawFrame( AknsUtils::SkinInstance(), 
                              gc, 
                              listRect, 
                              innerRect,
                              iBgFrId,
                              KAknsIIDDefault );

    // Draw item
    for ( TInt i = 0; i < iItemRects.Count(); i++ )
        {
        DrawItem( &gc, i );
        }

    // Draw scroll area
    if (iNaviBtnShown)
        {
        DrawPageButton( &gc, ETrue );
        DrawPageButton( &gc, EFalse );
        DrawPageInfo( &gc );
        }  
    }

// End Of File
