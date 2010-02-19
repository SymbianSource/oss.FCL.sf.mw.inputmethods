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
* Description:  Implement of touch input mode switch
*
*/
 
  
#include <peninputlayoutchoicelist.h>
#include <peninputlayout.h>
#include <AknIconUtils.h>
#include <AknLayoutDef.h>
#include <AknsDrawUtils.h>
#include <AknUtils.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <barsread.h>
#include <AknsUtils.h>
#include <bautils.h>
#include <AknFepGlobalEnums.h> 
#include <peninputcommonchoicelist.h>


#define MIN_WORD_LENGTH  200 //temperarily added.
const TInt KExtraMargine = 10;
// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
EXPORT_C CPeninputCommonChoiceList::
			 CPeninputCommonChoiceList( CFepUiLayout* aUiLayout, 
	            						TInt aControlId,
	            						TAknsItemID aItemSkinID,
	    				  			    TAknsItemID aBackgroundSkinID,
	    				  			    ECommonChoiceListStyle aListStyle ) 
: CFepLayoutChoiceList( aUiLayout, aControlId )
    { 
    iItemSkinID =  aItemSkinID;
    iBackgroundSkinID = aBackgroundSkinID;
    iListStyle = aListStyle;
#ifdef RD_TACTILE_FEEDBACK     
    //Advanced Tactile feedback REQ417-47932
    if(aUiLayout)
    	{
    	if (aUiLayout->PenInputType() == EPluginInputModeFSQ||
    		aUiLayout->PenInputType() == EPluginInputModeVkb)
    		{
    		SetTactileFeedbackType(ETouchFeedbackBasicButton);
    		aUiLayout->ChangeFeedbackType(reinterpret_cast<TInt>(this),ETouchFeedbackBasicButton);
    		}
    	else
    		{
    		SetTactileFeedbackType(ETouchFeedbackSensitiveInput);
    		aUiLayout->ChangeFeedbackType(reinterpret_cast<TInt>(this),ETouchFeedbackSensitiveInput);
    		}
    	}       
#endif //RD_TACTILE_FEEDBACK 
    }

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CPeninputCommonChoiceList* CPeninputCommonChoiceList::
								NewL( CFepUiLayout* aUiLayout,
				    				  TInt aControlId, 
									  TAknsItemID aItemSkinID,
					  			      TAknsItemID aBackgroundSkinID,
					  			      ECommonChoiceListStyle aListStyle ) 
    {
    CPeninputCommonChoiceList *self = new (ELeave) 
        CPeninputCommonChoiceList( aUiLayout, 
        						   aControlId, 
        						   aItemSkinID,
        						   aBackgroundSkinID,
        						   aListStyle );
    
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();

    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CPeninputCommonChoiceList::~CPeninputCommonChoiceList()
    {
    iItemRects.Close();
    }

EXPORT_C void CPeninputCommonChoiceList::AddItemRectL( const TRect& aRect )
	{
	iItemRects.AppendL( aRect );	
	}
	
EXPORT_C void CPeninputCommonChoiceList::RemoveAllItemsRect()
	{
	iItemRects.Reset();	
	}
	
EXPORT_C void CPeninputCommonChoiceList::
								SetTextLayout( const TAknTextLineLayout& aTextLayout )
	{
	iTextLayout = aTextLayout;	
	}
	
TInt CPeninputCommonChoiceList::MaxWidthOfListItem()
	{
	if( iTextLayout.FontId() == 0 )
		return 0;
	
	TInt maxLength = 0;
	const CFont* font = AknLayoutUtils::FontFromId(iTextLayout.FontId(), NULL);
	ASSERT( font );
	
	for( TInt i = 0; i < ItemsCount(); i++ )
		{
		TInt length = font->TextWidthInPixels( ItemByIndex(i)->iText );
		maxLength  = length > maxLength ? length : maxLength;
		}
    maxLength = maxLength > MIN_WORD_LENGTH ? maxLength : MIN_WORD_LENGTH;		
	return maxLength;
	}
	
EXPORT_C void CPeninputCommonChoiceList::ReCalcLayout()
	{
	if( iItemRects.Count() == 0 )
	    {
	    return;
	    }
	    
	if( iListStyle == EAutoFitLength )
		{
		TInt itemLength = MaxWidthOfListItem() + iTextLayout.il + iTextLayout.ir;
		TInt offsetLength = 0;
		offsetLength = Rect().Width() - iItemRects[0].Width();	
		
		if( itemLength + offsetLength + KExtraMargine > UiLayout()->Rect().Width() )
		    { 
		    itemLength = UiLayout()->Rect().Width() - KExtraMargine - offsetLength;
		    }
		TRect rect;
		for( TInt i = 0; i < iItemRects.Count(); i++ )
			{
			rect = iItemRects[i];
			offsetLength = itemLength - rect.Width();	
			rect.SetRect(rect.iTl, TSize( itemLength, rect.Height() ) ); 
			iItemRects[i] = rect;
			}
		rect = Rect();
		rect.Resize( offsetLength, 0 );
		SetRect( rect );		
		}
	}
	
EXPORT_C void CPeninputCommonChoiceList::Draw()
	{
	if(!AbleToDraw())
    	return;    
    
    DrawBackground(); 

    //draw Item
    for(int ii = 0; ii < ItemsCount(); ++ii)
        {
        DrawItem(ii, CurrentFocusedIndex() == ii);
        }
	}
	
void CPeninputCommonChoiceList::DrawBackground()
	{
    DrawOpaqueMaskBackground( Rect() );	
	if( iBackgroundSkinID.iMajor != EAknsMajorNone && 
 		iBackgroundSkinID.iMinor != EAknsMinorNone )
 		{
	    TRect rtInnerRect( Rect() );
	    rtInnerRect.Shrink( 1, 1 );
	    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
	    
	    gc->Activate( BitmapDevice() ); 
	    gc->SetBrushColor( KRgbWhite );
	    gc->Clear( Rect() ); 
		AknsDrawUtils::DrawFrame( AknsUtils::SkinInstance(), 
	                     *gc, 
	                     Rect(), 
	                     rtInnerRect,
	                     iBackgroundSkinID,
	                     KAknsIIDDefault );       
 		}
	}
	
void CPeninputCommonChoiceList::DrawItem( TInt aIndex, TBool aIsFocus )
	{
    TRect rtFocusRect(iItemRects[aIndex]);
    rtFocusRect.Move( Rect().iTl );
	
	if( aIsFocus )
		{
        //draw focus bitmap
        TRect rtInnerRect( rtFocusRect );
        rtInnerRect.Shrink( 7, 7 );
        
       	if( iItemSkinID.iMajor != EAknsMajorNone && 
       	    iItemSkinID.iMinor != EAknsMinorNone)
       		{
       		CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
       		gc->Activate( BitmapDevice() ); 
			AknsDrawUtils::DrawFrame( AknsUtils::SkinInstance(), 
                         *gc, 
                         rtFocusRect, 
                         rtInnerRect,
                         iItemSkinID,
                         KAknsIIDDefault );       	
       		}	
	    
		}	
	
	// draw text
    TAknLayoutText textLayout;
    textLayout.LayoutText(rtFocusRect, iTextLayout);
    TRgb color( KRgbBlack );  // sane default for nonskinned case
    TAknsQsnTextColorsIndex clrIndex;
    clrIndex = aIsFocus ? EAknsCIQsnTextColorsCG10 : EAknsCIQsnTextColorsCG20;

    if ( AknsUtils::AvkonSkinEnabled() )
        {
        AknsUtils::GetCachedColor( AknsUtils::SkinInstance(),
                               color, KAknsIIDQsnTextColors, clrIndex );
        }

    textLayout.DrawText(*BitGc(), ItemByIndex(aIndex)->iText, ETrue, color);
	}

EXPORT_C void CPeninputCommonChoiceList::SetListColumnNumWithLafL( TInt aMaxCount)
	{
	if( iLastColumnNum == aMaxCount || aMaxCount < 1 )
		return;
	
	iLastColumnNum  = aMaxCount;
	RemoveAllItemsRect();	
	
    TAknWindowLineLayout appWnd = AknLayoutScalable_Avkon::application_window(0).LayoutLine();
    TAknWindowLineLayout mainPane = AknLayoutScalable_Avkon::main_pane(4).LayoutLine();
    TAknLayoutRect mainPaneRect;
    mainPaneRect.LayoutRect(appWnd.Rect(), mainPane);
    
    TAknWindowLineLayout menuWnd = AknLayoutScalable_Avkon::
    								popup_menu_window(13).LayoutLine();
    TAknWindowLineLayout choiceWnd = AknLayoutScalable_Avkon::
    								popup_submenu_window(aMaxCount - 1).LayoutLine();    								
    TAknLayoutRect choiceRect;
    choiceRect.LayoutRect(mainPaneRect.Rect(), choiceWnd);
    
	TRect rectChoiseList( 0, 0 , 
						  choiceRect.Rect().Width(),
						  choiceRect.Rect().Height());
	// set list window rect						  						  
	SetRect( rectChoiseList );
	
    TAknWindowLineLayout listPopupPane = AknLayoutScalable_Avkon::
    								listscroll_popup_sub_pane(0).LayoutLine();	
	TAknLayoutRect listPopupPaneRect; 
	listPopupPaneRect.LayoutRect(rectChoiseList, listPopupPane);								
    TAknWindowLineLayout listSubMenuPane = AknLayoutScalable_Avkon::
    								list_submenu_pane(1).LayoutLine();
    TAknLayoutRect listSubMenuPaneRect;
    listSubMenuPaneRect.LayoutRect(listPopupPaneRect.Rect(), listSubMenuPane);
    
    TAknWindowLineLayout listSinglePane; 
    TAknLayoutRect listSinglePaneRect;
    
    // set list item rect
    for( TInt i = 0; i < aMaxCount; i++ )
    	{
    	listSinglePane = AknLayoutScalable_Avkon::
    							list_single_popup_submenu_pane(i, 0, 0).LayoutLine();
		listSinglePaneRect.LayoutRect( listSubMenuPaneRect.Rect(), listSinglePane );
		iItemRects.AppendL(listSinglePaneRect.Rect());	
    	}
    
    // set list text layout	
   	SetTextLayout(AknLayoutScalable_Avkon::list_single_popup_submenu_pane_t1(0).LayoutLine());  	
	}

EXPORT_C void CPeninputCommonChoiceList::SetListSkinID( TAknsItemID aItemSkinID, 
												        TAknsItemID aBackgroundSkinID )	
	{
	iItemSkinID = aItemSkinID;
	iBackgroundSkinID = aBackgroundSkinID;
	}
EXPORT_C TInt CPeninputCommonChoiceList::HitTest(const TPoint& aPoint)
    {
    TPoint pPoint(aPoint.iX - Rect().iTl.iX,aPoint.iY - Rect().iTl.iY);
    TInt arrayCount = iItemRects.Count();
    TInt index = arrayCount;
    for(TInt i = 0; i < arrayCount; i++)
        {
        if(iItemRects[i].Contains(pPoint))
            {
            index = i;
            }
        }
    if( index >= arrayCount )
        {
        return EListOutside;
        }
    else
        {
        return index;
        }    
    }							 	
// End Of File
