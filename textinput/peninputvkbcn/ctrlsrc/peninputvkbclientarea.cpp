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
* Description:  vkb client area implementation
*
*/


// system includes
#include <peninputvkbctrlext.h>
#include <peninputvkbctrllayout.h>
#include <peninputlayout.h>
#include <peninputlayoutrootctrl.h>
#include <peninputeventbutton.h>
#include <peninputdropdownlist.h>
#include <peninputmultimodebutton.h>
#include <aknfeppeninputenums.h>    //command from fep or IME to plugin
#include <peninputrangebar.h>
#include <peninputvkbcn.rsg>
#include <peninputrepeatbutton.h>
#include <AknLayoutDef.h>
#include <AknUtils.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <AknsUtils.h>
#include <peninputlayoutmultilineicf.h>
#include <peninputdragbutton.h>
#include <peninputpluginutils.h>

// user includes
#include "peninputvkbevent.h"
#include "peninputvkbdataprovider.h"
#include "peninputvkbclientarea.h"
#include "peninputvkbctrlpool.h"
#include "peninputvkblayoutpool.h"
#include "peninputvkbcompositionfield.h"
#include "peninputvkbImlayout.h"
#include "peninputvkbnumerickeymappingmgr.h"
#include "peninputvkbwindow.h"
#include "peninputvkbdatamgr.h"
#include "peninputvkbuistate.h"

// constant definition
const TInt KDropdownRowsForStroke = 3;
const TInt KDropdownRowsForOthers = 4;

const TUint16 KStrokeSeparator = 0x2022;
const TUint16 KPinyinSeparator = 0x0027;




// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::CAknFepVkbClientArea
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CAknFepVkbClientArea::CAknFepVkbClientArea(CFepUiLayout* aUiLayout, 
                                           TInt aControlId,
                                           MAknFepVkbDataProvider* aDataProvider,
                                           CPeninputCnGenericVkbLafMgr* aLafMgr)
    :CControlGroup(aUiLayout, aControlId, EFalse), 
     iDataProvider(aDataProvider),
     iTextPropertySet(EFalse),
     iDropdownPropertySet(EFalse),
     iLafMgr(aLafMgr)
    {
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::NewL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CAknFepVkbClientArea* CAknFepVkbClientArea::NewL(CFepUiLayout* aUiLayout, 
                                                 TInt aControlId,
                                                 MAknFepVkbDataProvider* aDataProvider,
                                                 CPeninputCnGenericVkbLafMgr* aLafMgr)
    {
    CAknFepVkbClientArea* self = CAknFepVkbClientArea::NewLC(aUiLayout, aControlId, 
                                                             aDataProvider, aLafMgr);
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::NewLC
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CAknFepVkbClientArea* CAknFepVkbClientArea::NewLC(CFepUiLayout* aUiLayout, 
                                                  TInt aControlId,
                                                  MAknFepVkbDataProvider* aDataProvider,
                                                  CPeninputCnGenericVkbLafMgr* aLafMgr)
    {
    CAknFepVkbClientArea* self = new (ELeave) CAknFepVkbClientArea(aUiLayout, aControlId, 
                                                                   aDataProvider, aLafMgr);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::SetTextProperty
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbClientArea::SetTextProperty()
	{
    TAknTextLineLayout compositionText;
    //compositionText = AknLayoutScalable_Avkon::cell_vkb_candidate_pane_t1().LayoutLine();
    compositionText = AknLayoutScalable_Avkon::cell_vkb2_top_candi_pane_t1(0).LayoutLine();

    iCompositionField->SetNormalColor(AKN_LAF_COLOR_STATIC(compositionText.iC));
    iCompositionField->SetWarningColor(KRgbRed);
    
    iCompositionField->SetFont(AknLayoutUtils::FontFromId(compositionText.iFont, NULL));
    
    iTextPropertySet = ETrue;
	}
    
// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::ConstructL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbClientArea::ConstructL()
    {
    BaseConstructL();
    
    iCtrlPool = CAknFepVkbCtrlPool::NewL();
    iVkbLayout = CAknFepCtrlVkbLayout::NewL();
    iLayoutPool = CAknFepLayoutPool::NewL( *iVkbLayout );    
    
    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC(reader, R_AKN_FEP_VKB_KEY_MAPPING_STRING_LIST);
    iKeyMappingMgr = CAknFepVkbNumericKepMappingMgr::NewL(reader);
    CleanupStack::PopAndDestroy(); // reader
 
    // 1.Create all control in the client, including all kinds of control in different layout
    // Composition field
    iCompositionField = CAknFepVkbCompositionField::NewL(KMaxLengthCompFieldElemCount,TRect(),
                                                         UiLayout(),
                                                         EAknFepVkbCtrlIdCompositionField, 
                                                         iDataProvider );
    iCompositionField->SetResourceId(R_AKN_FEP_VKB_COMPOSITION_FIELD);
    iCompositionField->ConstructFromResourceL();
    iCtrlPool->AddControl(iCompositionField);   
    AddButtonL(EAknFepVkbCtrlIdArrowUpBtn,
               EVkbEventUpArrow,
               R_AKN_FEP_VKB_ARROW_UP_BUTTON,
               EKeyUpArrow,
               ETrue);
    
    AddButtonL(EAknFepVkbCtrlIdArrowDownBtn,
               EVkbEventDownArrow,
               R_AKN_FEP_VKB_ARROW_DOWN_BUTTON,
               EKeyDownArrow,
               ETrue
               );
    
    AddButtonL(EAknFepVkbCtrlIdArrowLeftBtn,
               EVkbEventLeftArrow,
               R_AKN_FEP_VKB_ARROW_LEFT_BUTTON,
               EKeyLeftArrow,
               ETrue
               );
    
    AddButtonL(EAknFepVkbCtrlIdArrowRightBtn,
               EVkbEventRightArrow,
               R_AKN_FEP_VKB_ARROW_RIGHT_BUTTON,
               EKeyRightArrow,
               ETrue
               );
                                         
    // Vkb control   
    iVkbCtrl = CPeninputVkbCtrlExt::NewL(TRect(), 
                                       UiLayout(), 
                                       EAknFepVkbCtrlIdVkbCtrl, 
                                       NULL);
    iVkbCtrl->SetResourceId( R_PENINPUT_LAYOUT_VKB_KEYBOARD );
    iVkbCtrl->ConstructFromResourceL();                                  
    iVkbCtrl->SetKeyTextColorGroup( EAknsCIQsnTextColorsCG68 );                                   
    iVkbLayout->SetVkbKeyboard( iVkbCtrl );
    if (UiLayout()->PenInputType() == EPluginInputModeVkb)
        {
        iVkbCtrl->SetIrregularKeyBoard(ETrue);
        iVkbLayout->SetIrregularResourceId(R_AKN_FEP_VKBCN_IRREGULAR_BITMAP);
        iVkbLayout->ConstructFromIrregularResourceL();
        }
    else if (UiLayout()->PenInputType() == EPluginInputModeFSQ)
        {
        iVkbCtrl->SetIrregularKeyBoard(EFalse);
        iVkbLayout->SetNonIrregularResourceId(R_AKN_FEP_VKBCN_NONIRREGULAR_BITMAP);
        iVkbLayout->ConstructFromNonIrregularResourceL(); 
        
        //set key to be 9 piece graphics
        iVkbCtrl->SetKeySkinId( EKeyBmpNormal, KAknsIIDQsnFrKeypadButtonFrNormal );
        iVkbCtrl->SetKeySkinId( EKeyBmpHighlight, KAknsIIDQsnFrKeypadButtonFrPressed );
        iVkbCtrl->SetKeySkinId( EKeyBmpDim, KAknsIIDQsnFrKeypadButtonFrInactive );

        }
    iCtrlPool->AddControl(iVkbCtrl);
    
        // Add bubble support
    iVkbCtrl->ShowBubble(ETrue);    
    iVkbCtrl->SetGowithPointerDown(ETrue);
    iVkbCtrl->SetBubbleBitmapParam(NULL,NULL,KAknsIIDQsnFrInputCharPreview);  //Change the ID when ID into release


    AddButtonL(EAknFepVkbCtrlIdTouchInputOption, 
               EVkbEventTouchInputOption, 
               R_AKN_FEP_VKB_TOUCH_INPUT_OPTION);
    
    // Enter button          
    AddButtonL(EAknFepVkbCtrlIdEnterBtn,EVkbEventKeyEnter,R_AKN_FEP_VKB_ENTER, EKeyEnter);
    
    // Drop down list  
    AddDropdownListL();
    
    // Range bar
    AddRangeBarL();

    // Shift and capslock merged button
    CAknFepCtrlMultiModeButton* button = 
  		CAknFepCtrlMultiModeButton::NewLC( UiLayout(), EAknFepVkbCtrlIdShiftBtn );
    if ( button )
	    {
	    // Read resource
	    TResourceReader reader;
	    
	    CCoeEnv::Static()->CreateResourceReaderLC( reader,
	    	 			                           R_AKN_FEP_VKB_SHIFT_CAPSLOCK );  
	    button->SetResourceId( R_AKN_FEP_VKB_SHIFT_CAPSLOCK );      
	    button->ConstructFromResourceL();  
	    // Pop and destroy reader
	    CleanupStack::PopAndDestroy( 1 );
	    
	    // Add into the control pool                                                   
	    iCtrlPool->AddControl( button );
	    }
    CleanupStack::Pop( button );     
     
    // Space button          
    CAknFepCtrlEventButton* spaceButton = AddButtonL(
    										EAknFepVkbCtrlIdSpaceBtn,
    										EVkbEventKeySpace,
    										R_AKN_FEP_VKB_SPACE1, 
    										EKeySpace);
    // Create 3-pieces color icon for space button
	CPenInput3PiecesColorIcons* colorIcons = 
				CPenInput3PiecesColorIcons::NewL( R_AKN_FEP_VKB_SPACE_ICON1,
												  R_AKN_FEP_VKB_SPACE_ICON2,
												  R_AKN_FEP_VKB_SPACE_ICON3 );
	spaceButton->Set3PiecesColorIcons( colorIcons );
	
   //set background of space button to be 9 piece graphics
    if ( UiLayout()->PenInputType() == EPluginInputModeFSQ )
        {
        spaceButton->SetImageFrameId( KAknsIIDQsnFrKeypadButtonFrNormal, 
                                      KAknsIIDQsnFrKeypadButtonFrPressed,
                                      KAknsIIDQsnFrKeypadButtonFrInactive );
        }

    									
    // switch to hwr button  
    AddButtonL(EAknFepVkbCtrlIdSwitchToHwrBtn,EVkbEventHwr,R_AKN_FEP_VKB_HWR);   
    
    SetTextProperty();
    
    if( iCtrlPool != NULL )
        {
        for( TInt i=0; i < iCtrlPool->ControlCount(); i++ )
            {
            iCtrlPool->ControlByIndex(i)->AddEventObserver( UiLayout() );           
            }
        }
    }

// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::~CAknFepVkbClientArea
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CAknFepVkbClientArea::~CAknFepVkbClientArea()
    {
    delete iVkbLayout;
    delete iCtrlPool;
    delete iLayoutPool;
    delete iKeyMappingMgr;
    iRemainedPoolControlIDs.Close();
    }

// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::SizeChanged
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbClientArea::SizeChanged()
    {
    TAknTextLineLayout keypadText, compositionText, dropDownText;
    
    TInt btnWidth = iDataProvider->RequestData(EAknFepDataTypeUnitWidth);
    TInt btnHeight = iDataProvider->RequestData(EAknFepDataTypeUnitHeight);
    
    if (iDataProvider->SizeChanging() || (!iTextPropertySet))
    	{
    	SetTextProperty();
    	}
        
    // modify drop down list font
    if (iDataProvider->SizeChanging() || (!iDropdownPropertySet))
    	{
        //TAknTextLineLayout listText = 
        //      AknLayoutScalable_Avkon::cell_vkb_candidate_pane_t1().LayoutLine();
        TAknTextLineLayout listText = 
        		AknLayoutScalable_Avkon::cell_vkb2_top_candi_pane_t1(0).LayoutLine();
        iDropdownList->SetFont(AknLayoutUtils::FontFromId(listText.iFont, NULL));
        //iDropdownList->SetTextColor(AKN_LAF_COLOR_STATIC(listText.iC), ETrue);

        iDropdownList->SizeChanged(btnWidth, btnHeight, btnWidth);
    	}
    
    if ( iCurrentImLayout )
        {
        //3.Change the postion and size of controls in current layout
        ReorganizeControls(iCurrentImLayout, EFalse);
        }        

    //calculate current vkblayout according current imlayout
    if ( iCurrentVkbLayout )
        {
        //dimmed some keys if needed
        TRAP_IGNORE(DimKeys());    
        }
    }
        
// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::ChangeImLayout
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbClientArea::ChangeImLayout(TInt aImLayout, TBool aChangeVkb)
    {
    iDataProvider->SetData(EAknFepDataTypeIMLayout, aImLayout);
    static_cast<CAknFepVkbDataMgr*>(iDataProvider)->ResetUnitSize();
    
    TInt currentRange = iDataProvider->RequestData(EAknFepDataTypeCurrentRange);        

    CAknFepVkbImLayout* newImLayout = iLayoutPool->GetImLayout(aImLayout);

    if ( iCurrentImLayout != newImLayout)
        {
        iCurrentImLayout = newImLayout;
        if  ( currentRange == ERangeNative) 
            {
            iDropdownList->SetRowNum(aImLayout == EAknFepVkbImCnStroke ?
                                     KDropdownRowsForStroke : KDropdownRowsForOthers );
            }
        ReorganizeControls(iCurrentImLayout, ETrue);  
        }

    if ( iRangeBar )
        {
        iRangeBar->SetRange(currentRange);
        }
        
    //calculate current vkblayout according current imlayout
    if (aChangeVkb)
        {
         if ( ( currentRange == ERangeEnglish ) || ( currentRange == ERangeAccent ) )
            {
            // Change shift and capslock button status according to the current case
            TInt curCase = iDataProvider->RequestData( EAknFepDataTypeCase );
            TInt shfit;  
            TInt capslock;  
            
            switch ( curCase )
                {
                case ECaseUpper:
                    shfit = 0;
                    capslock = 1;
                    break;
                case ECaseText:
                    shfit = 1;
                    capslock = 0;
                    break;
                case ECaseInverseText:        
                    shfit = 1;
                    capslock = 1;
                    break;            
                case ECaseLower:        
                case ECaseInvalide:
                default:
                    shfit = 0;
                    capslock = 0;        
                    break;   
                }     

            iDataProvider->SetData( EAknFepDataTypeShfitDown, shfit );
            //ChangeButtonStatus( shfit, EAknFepVkbCtrlIdShiftBtn );
            iDataProvider->SetData( EAknFepDataTypeCapslockDown, capslock );
            //ChangeButtonStatus( capslock, EAknFepVkbCtrlIdCapslockBtn );                
            ChangeMergedButtonStatus(shfit, capslock);
            
            if ( ( shfit && capslock ) || ( !shfit && !capslock  ) )
                {
                ChangeVkbLayout();
                }
            else
                {
                TAknFepVkbLayout vkbLayout = ShiftCapsSingleVkbLayoutId( currentRange );
                ChangeVkbLayout( vkbLayout );
                }
            }
        else
            {
            iDataProvider->SetData( EAknFepDataTypeShfitDown, 0 );
            //ChangeButtonStatus( 0, EAknFepVkbCtrlIdShiftBtn );
            iDataProvider->SetData( EAknFepDataTypeCapslockDown, 0 );
            //ChangeButtonStatus( 0, EAknFepVkbCtrlIdCapslockBtn ); 
            ChangeMergedButtonStatus(0, 0);
 
            ChangeVkbLayout();
            }
        }
    }

// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::ShiftCapsSingleVkbLayoutId
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
TAknFepVkbLayout CAknFepVkbClientArea::ShiftCapsSingleVkbLayoutId( TInt aRange )
    {
    TInt defaultIMLayout = iDataProvider->RequestData(EAknFepDataTypeDefaultIMLayout);
    TInt curLanguage = iDataProvider->RequestData(EAknFepDataTypeInputLanguage);
    TAknFepVkbLayout vkbLayout = EAknFepVkbVkbLatinShift;
    TInt imLayout = iDataProvider->RequestData(EAknFepDataTypeIMLayout);
    
    switch (aRange)
        {
        case ERangeEnglish:
        	if (curLanguage == ELangTaiwanChinese && defaultIMLayout != EAknFepVkbImCnStroke)
        		{
        		vkbLayout = EAknFepVkbVkbLatinForZhuyinShift;
        		}
        	else 
        		{
        		vkbLayout = EAknFepVkbVkbLatinShift;
        		}
            break;
        case ERangeNumber:
        	if (curLanguage == ELangTaiwanChinese && defaultIMLayout != EAknFepVkbImCnStroke)
        		{
        		vkbLayout = EAknFepVkbVkbNumberForZhuyinShift;
        		}
        	else 
        		{
        		vkbLayout = EAknFepVkbVkbNumberShift;
        		}            
            break;
        case ERangeSymbol:
            vkbLayout = EAknFepVkbVkbSymbolShift;
            break;
        case ERangeAccent:
            {
            TInt accent = iDataProvider->RequestData(EAknFepDataTypeCurrentAccent);    
            switch (accent)
                {
                case EAknFepVkbVkbAccented1:
                    vkbLayout = EAknFepVkbVkbAccentedShfit1; 
                    break;
                case EAknFepVkbVkbAccented2:
                    vkbLayout = EAknFepVkbVkbAccentedShfit2; 
                    break;
                case EAknFepVkbVkbAccented3:
                    vkbLayout = EAknFepVkbVkbAccentedShfit3; 
                    break;
                default:
                    vkbLayout = EAknFepVkbVkbAccentedShfit1; 
                    break;                            
                }
            }
                                
            break;
        default:
            break;
        } 
        return vkbLayout;
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::ChangeVkbLayout
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbClientArea::ChangeVkbLayout(TInt aVkbLayoutId)
    {
    TAknFepVkbLayout vkbLayoutId;
    TInt range = iDataProvider->RequestData(EAknFepDataTypeCurrentRange);
    if ( aVkbLayoutId >= 0 )
        {
        CPeninputVkbLayoutInfo* tempLayout = iLayoutPool->GetVkbLayout(aVkbLayoutId); 
        if ( iCurrentVkbLayout == tempLayout)
            {
            return;
            }
        else
            {
            iCurrentVkbLayout = tempLayout;
            }
        vkbLayoutId = static_cast<TAknFepVkbLayout>(aVkbLayoutId);
        }
    else
        {
        
        vkbLayoutId = GetDefaultVkbLayout(range);
        iCurrentVkbLayout = iLayoutPool->GetVkbLayout(vkbLayoutId);
        }
    
    iVkbLayout->ChangeVkbLayout( vkbLayoutId );
    iVkbLayoutId = aVkbLayoutId;
    SetVkbLayoutSize(vkbLayoutId);
      
    iVkbCtrl->SetRect( TRect( iVkbBoardLeftTop, iLafMgr->KeypadRect().Size() ) );
    
    //dimmed some keys if needed
    TRAP_IGNORE(DimKeys());
    }
 
// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::SetVkbLayoutSize
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbClientArea::SetVkbLayoutSize(TInt aVkbLayoutId)
    {
    TInt currentRange = iDataProvider->RequestData(EAknFepDataTypeCurrentRange);
    TAknFepVkbLayout vkbLayoutid;
    
    if(aVkbLayoutId < 0)
        {
        vkbLayoutid = GetDefaultVkbLayout(currentRange);
        }
    else
        {
        vkbLayoutid = static_cast<TAknFepVkbLayout>(aVkbLayoutId);
        }

    CPeninputVkbLayoutInfo* vkbLayout = iLayoutPool->GetVkbLayout( vkbLayoutid );

    // Calculate current vkblayout according current layout
    if ( vkbLayout )
        {
        const TInt columns = vkbLayout->Columns();
        const TInt rows = vkbLayout->Rows();
        TRect keyRect;
        TRect charRect;
        
        iLafMgr->SetLayoutType(iLafMgr->LayoutTypeFromRowsColumns(rows,columns));
        
 		// Set priview popup size
		TRect outrect,innerrect;   
		iLafMgr->GetPreviewWndRect(outrect,innerrect);
		iVkbCtrl->SetTextFormat(iLafMgr->PreviewWndText());  //Read laf 
		iVkbCtrl->SetBubbleSize(TSize(outrect.Width(),outrect.Height()));  // Read laf
		iVkbCtrl->SetFrameDiff(innerrect.iTl.iX - outrect.iTl.iX,
		               innerrect.iTl.iY - outrect.iTl.iY,
		               outrect.iBr.iX - innerrect.iBr.iX,
		               outrect.iBr.iY - innerrect.iBr.iY);

        if(iDropdownList)
            {
            iDropdownList->SetBubbleBitmapParam(NULL,NULL,KAknsIIDQsnFrInputCharPreview);
            
    		iDropdownList->SetTextFormat(iLafMgr->PreviewWndText());  //Read laf
    		iDropdownList->SetBubbleTextFont(iLafMgr->PreviewFont()); 
    		iDropdownList->SetBubbleSize(TSize(outrect.Width(),outrect.Height()));  // Read laf
    		iDropdownList->SetFrameDiff(innerrect.iTl.iX - outrect.iTl.iX,
    		               innerrect.iTl.iY - outrect.iTl.iY,
    		               outrect.iBr.iX - innerrect.iBr.iX,
    		               outrect.iBr.iY - innerrect.iBr.iY);
            }

        for (TInt exter = 0; exter < rows; exter ++)
            {
            for (TInt inter = 0; inter < columns; inter ++ )
                {
                iLafMgr->GetKeyRect(inter,exter, keyRect, charRect);
                vkbLayout->SetKeyInfo(exter,inter,keyRect,charRect,iLafMgr->KeyTextFont());
                }
            }
        TSize curSize = vkbLayout->KeyInfoList()[0]->Key()->Rect().Size();

        if ( iVkbCtrl->IsIrregularKeyBoard() )
            {
            TSize size = 
                        iVkbCtrl->IrregularKeyBitmap(EKeyLeftDown,EKeyStateNormal)->SizeInPixels();
            if ( curSize != size )
                {
                for ( TInt i = 0; i <= EKeyRightUp; i++ )
                    {
                    for ( TInt j = 0; j <= EKeyStateDimmedMask; j++ )
                        {
                        AknIconUtils::SetSize( iVkbCtrl->IrregularKeyBitmap(
                            (TVirtualKeyIrregularKeyType)i,(TVirtualKeyIrregularKeyState)j), 
                             curSize, EAspectRatioNotPreserved ); 
                            
                        AknIconUtils::SetSize( iVkbCtrl->IrregularKeyBitmap(
                            (TVirtualKeyIrregularKeyType)i,(TVirtualKeyIrregularKeyState)j), 
                             curSize, EAspectRatioNotPreserved );
                        }
                    }
                }
            }
        else
        	{
        	TSize size = iVkbCtrl->NonIrregularKeyBitmap(EKeyBmpNormal)->SizeInPixels();
        	if( curSize != size )	
        		{
                for ( TInt i = 0; i <= EKeyBmpLastType; i++ )
                    {
                    if( iVkbCtrl->NonIrregularKeyBitmap((TVirtualKeyBmpType)i) )
                    	{
                        AknIconUtils::SetSize( 
                            iVkbCtrl->NonIrregularKeyBitmap((TVirtualKeyBmpType)i), curSize, 
                                                             EAspectRatioNotPreserved );
                    	}
                    }
        		}
        	}
        }
    }
    

// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::LoopVkbLayout
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbClientArea::LoopVkbLayout(TVkbShiftType aType, TBool aNotify)
    {
    TInt currentRange = iDataProvider->RequestData(EAknFepDataTypeCurrentRange);        
    
    if (currentRange!=ERangeNative)
        {
        TBool shiftDown = iDataProvider->RequestData(EAknFepDataTypeShfitDown); 
        TBool capslockDown = iDataProvider->RequestData(EAknFepDataTypeCapslockDown);
        
        if ( aType == EVkbShift)
            {
            if ((currentRange == ERangeEnglish) || (currentRange == ERangeAccent))
                {
                capslockDown = !capslockDown;
                iDataProvider->SetData( EAknFepDataTypeCapslockDown, capslockDown );
                ChangeMergedButtonStatus( shiftDown, capslockDown );   
                }
            }
        else if ( aType == EVkbCapslock )
            {
            if(static_cast<CAknFepVkbDataMgr*>(iDataProvider)->IsShiftPermitted())
                {
                if ((currentRange == ERangeEnglish) || (currentRange == ERangeAccent))
                    {
                    shiftDown = !shiftDown;
                    ChangeMergedButtonStatus( shiftDown, capslockDown );
                    iDataProvider->SetData( EAknFepDataTypeShfitDown, shiftDown );   
                    }
                
                }
            else
                {
                if(static_cast<CAknFepVkbDataMgr*>(iDataProvider)->IsCapsLockPermitted())
                    {
                    capslockDown = !capslockDown;
                    iDataProvider->SetData( EAknFepDataTypeCapslockDown, capslockDown );
                    ChangeMergedButtonStatus( shiftDown, capslockDown );   
                    }
                }
            }
        else  if( aType == EVkbBoth )
            {
            if ( shiftDown )
                {
                shiftDown = 0;
                }
            else
                {
                shiftDown = 1;
                }
            iDataProvider->SetData( EAknFepDataTypeShfitDown, shiftDown );
            
            if ((currentRange == ERangeEnglish) || (currentRange == ERangeAccent))
                {
                capslockDown = !capslockDown;
                iDataProvider->SetData( EAknFepDataTypeCapslockDown, capslockDown );
                ChangeMergedButtonStatus( shiftDown, capslockDown );   
                }           
            }
        else 
            {
            ChangeMergedButtonStatus( shiftDown, capslockDown );            
            }
         
        // Update indicator 
        TInt  caseid = CaseByShiftCapslockStatus();         
         
         
        if ( aNotify && (currentRange == ERangeEnglish || currentRange == ERangeAccent))
            {
            TBuf<4> buf;
            TInt fepcase = FepCaseByCaseId( caseid );
            iDataProvider->SetData( EAknFepDataTypeCase, fepcase );
            buf.Append(reinterpret_cast<TText*>(&fepcase), 4);
            UiLayout()->LayoutOwner()->SignalOwner( ESignalCaseMode, buf );            
            }

                
        if ( ( shiftDown && capslockDown ) || ( !shiftDown && !capslockDown  ) )
            {
            ChangeVkbLayout();
            }
        else
            {
            TAknFepVkbLayout vkbLayout = ShiftCapsSingleVkbLayoutId( currentRange );
            ChangeVkbLayout( vkbLayout );
            }
        }
    Draw();
    UpdateArea(Rect(), EFalse);    
    }

// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::FepCaseByCaseId
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CAknFepVkbClientArea::FepCaseByCaseId( TInt aCaseId )
    {
    switch ( aCaseId )
        {
        case 0:
            return ECaseLower;
        case 1:
            return ECaseText;
        case 2:
            return ECaseUpper;
        case 3:        
            return ECaseInverseText;
        default:
            return ECaseLower;
        }        
    }

// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::CaseByShiftCapslockStatus
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CAknFepVkbClientArea::CaseByShiftCapslockStatus()
    {
    TBool shiftDown = iDataProvider->RequestData(EAknFepDataTypeShfitDown); 
    TBool capslockDown = iDataProvider->RequestData(EAknFepDataTypeCapslockDown);
        
    return ( (TInt)capslockDown * 2 + (TInt)shiftDown );
    }

void CAknFepVkbClientArea::ReorganizeZhuYinControls( TBool aShowArrowKey )
    {
    if ( UiLayout()->PenInputType() != EPluginInputModeFSQ )
        {
        return;
        }

    CAknFepCtrlRepeatButton* arrowUpBtn = static_cast<CAknFepCtrlRepeatButton*>
        ( ControlById( EAknFepVkbCtrlIdArrowUpBtn ) );
    CAknFepCtrlRepeatButton* arrowDownBtn = static_cast<CAknFepCtrlRepeatButton*>
        ( ControlById( EAknFepVkbCtrlIdArrowDownBtn ) );
    CAknFepCtrlRepeatButton* arrowLeftBtn = static_cast<CAknFepCtrlRepeatButton*>
        ( ControlById( EAknFepVkbCtrlIdArrowLeftBtn ) );
    CAknFepCtrlRepeatButton* arrowRightBtn = static_cast<CAknFepCtrlRepeatButton*>
        ( ControlById( EAknFepVkbCtrlIdArrowRightBtn ) );
    CAknFepVkbCompositionField* compositionField = static_cast<CAknFepVkbCompositionField*>
        (ControlById( EAknFepVkbCtrlIdCompositionField));
    CFepCtrlDropdownList* dropdownList = static_cast<CFepCtrlDropdownList*>
        (ControlById( EAknFepVkbCtrlIdCandidateList)); 

    if ( iDataProvider->RequestData(EAknFepDataTypeIMLayout) == EAknFepVkbImCnZhuyin )
        {
        if ( aShowArrowKey )
            {    
            compositionField->Hide(ETrue);
            dropdownList->Hide(ETrue);
            arrowUpBtn->Hide(EFalse);
            arrowDownBtn->Hide(EFalse);
            arrowLeftBtn->Hide(EFalse); 
            arrowRightBtn->Hide(EFalse);
            }
        else
            {
            arrowUpBtn->Hide(ETrue);
            arrowDownBtn->Hide(ETrue);
            arrowLeftBtn->Hide(ETrue);
            arrowRightBtn->Hide(ETrue);
            compositionField->Hide(EFalse);
            dropdownList->Hide(EFalse); 
            }
        }
    else if( iDataProvider->RequestData(EAknFepDataTypeIMLayout) == EAknFepVkbImCnPinyin ||
             iDataProvider->RequestData(EAknFepDataTypeIMLayout) == EAknFepVkbImCnStroke ||
             iDataProvider->RequestData(EAknFepDataTypeIMLayout) == EAknFepVkbImCnCangjie )
        {
        compositionField->Hide(EFalse);
        dropdownList->Hide(EFalse); 
        }
    }

    
// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::ReorganizeControls
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbClientArea::ReorganizeControls(CAknFepVkbImLayout* aImLayout,
                                              TBool aImLayoutChanged)
    {
    // First remove all controls in the group, but don't delete them
    Reset(aImLayoutChanged);

    // Then add controls according to the configuration of clientlayout
    RPointerArray<CAknFepControlInfo>& controlList = aImLayout->ControlInfoList();
    
    TInt count = controlList.Count();
    TInt controlID = 0;

    
    CFepUiBaseCtrl* ctrl = NULL;   

    TInt range = iDataProvider->RequestData(EAknFepDataTypeCurrentRange);
    TInt curLanguage = iDataProvider->RequestData(EAknFepDataTypeInputLanguage);
    
    TInt vkbLayoutId = GetDefaultVkbLayout(range);
    CPeninputVkbLayoutInfo* vkbLayout = iLayoutPool->GetVkbLayout(vkbLayoutId);
    
    TInt vkbRows = vkbLayout->Rows();
    TInt vkbColumns = vkbLayout->Columns();
    iLafMgr->SetLayoutType(iLafMgr->LayoutTypeFromRowsColumns(vkbRows,vkbColumns));
    
    TPeninputCnCellType cellType = GetCellType(vkbColumns);
    iCellAidGap = iLafMgr->CellAidGap(cellType);
    
    TRect rect;
    TInt btnWidth = iDataProvider->RequestData(EAknFepDataTypeUnitWidth);
    TInt btnHeight = iDataProvider->RequestData(EAknFepDataTypeUnitHeight);
    TInt layoutType = iDataProvider->RequestData(EAknFepDataTypeIMLayout);
    
    TInt candidateHeight = 0;
    if ( layoutType == EAknFepVkbImCnZhuyin )
    	{
    	candidateHeight = btnHeight;
    	}
    else if (UiLayout()->PenInputType() != EPluginInputModeFSQ)
    	{
    	candidateHeight = iLafMgr->KeypadRect().Height() / vkbRows + 1;
    	}
    else if (UiLayout()->PenInputType() == EPluginInputModeFSQ)
        {
        candidateHeight = iLafMgr->KeypadRect().Height() / vkbRows;
        }
    	
    TSize size;
    TInt width = UiLayout()->PenInputType() == EPluginInputModeFSQ ?
    			 iLafMgr->EntirePaneRect().Width() : iLafMgr->KeypadRect().Width();
    if (range == ERangeNative)
    	{
		size = TSize( width,
                      iLafMgr->KeypadRect().Height() 
                      + ( aImLayout->Rows() - vkbRows - 1)*btnHeight
                      + candidateHeight);
    	}
   	else
   		{
		size = TSize( width,
		              iLafMgr->KeypadRect().Height() 
		              + ( aImLayout->Rows() - vkbRows)*btnHeight);
   		}
    
    if (UiLayout()->PenInputType() == EPluginInputModeFSQ )
        {
        TRect screenrect;
        AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EScreen, screenrect);

        size = screenrect.Size();
        }
    
    rect.SetSize( size );
    
    //Set owner rect
    SetWholeWindowRect(rect);
    
    TInt needJustify = iDataProvider->RequestData(EAknFepDataTypeNeedBRJustify);
    if (needJustify)
        {
        BottomRightJustify();
        iDataProvider->SetData(EAknFepDataTypeNeedBRJustify, EFalse);
        }

    TPoint point = Rect().iTl;
    TInt i = 0;
    // Copy ctrl ID from ctrl pool
    iRemainedPoolControlIDs.Reset();
    for( i = 0; i < iCtrlPool->ControlCount(); i++ )
    	{
    	TInt id = iCtrlPool->ControlByIndex(i)->ControlId();
    	iRemainedPoolControlIDs.Append(id);
    	}
    	
    for (i=0;i<count;i++)
        {
        controlID = controlList[i]->ControlID();
        ctrl = iCtrlPool->Control(controlID);

        if ( ctrl )
            {
            
            TRAP_IGNORE(AddControlL(ctrl));
            
	        // Remove control IDs added to the control group in the array
			TInt n = iRemainedPoolControlIDs.Find( controlID );
			if( n != KErrNotFound)
				iRemainedPoolControlIDs.Remove(n);            
            }        
        }
    
    if ( iCtrlList.Count() != count )
        {
        return;
        }
    TRect optionRect;
    TRect compositionRect;
    TRect innerRect;
    
    // Get composition area rect from laf
	TRect compositionAreaFromLaf = iLafMgr->compositionLayoutData();
	TInt GapValue = compositionAreaFromLaf.iTl.iX;
    
    if (range == ERangeNative)
        {
        if ( layoutType == EAknFepVkbImCnZhuyin )
            {
            // Option Btn
            optionRect.iTl = Rect().iTl;
            optionRect.SetSize(TSize(btnWidth, btnHeight));
            AknPenImageUtils::CalculateGraphicRect( optionRect, innerRect );
            static_cast<CAknFepCtrlEventButton*>
                (ControlById(EAknFepVkbCtrlIdTouchInputOption))->SizeChanged(optionRect, 
                                                                             innerRect, 
                                                                             ETrue);
            
            TRect arrowRect;
            TRect innerRect;
            
            if ( UiLayout()->PenInputType() == EPluginInputModeFSQ )
                {
                arrowRect.iTl = TPoint( optionRect.iBr.iX, optionRect.iTl.iY );
                arrowRect.SetSize(TSize(btnWidth + btnWidth, btnHeight));
                AknPenImageUtils::CalculateGraphicRect( arrowRect, innerRect );
                static_cast<CAknFepCtrlRepeatButton*>
                    ( ControlById( EAknFepVkbCtrlIdArrowUpBtn ) )->SizeChanged(arrowRect, 
                                                                               innerRect, 
                                                                               ETrue);
                    
                arrowRect.iTl = TPoint(arrowRect.iBr.iX, arrowRect.iTl.iY);
                arrowRect.SetSize(TSize(btnWidth + btnWidth, btnHeight));
                AknPenImageUtils::CalculateGraphicRect( arrowRect, innerRect );
                static_cast<CAknFepCtrlRepeatButton*>
                    ( ControlById( EAknFepVkbCtrlIdArrowDownBtn ) )->SizeChanged(arrowRect,
                                                                                 innerRect, 
                                                                                 ETrue);
                    
                arrowRect.iTl = TPoint(arrowRect.iBr.iX, arrowRect.iTl.iY);
                arrowRect.SetSize(TSize(btnWidth + btnWidth, btnHeight));
                AknPenImageUtils::CalculateGraphicRect( arrowRect, innerRect );
                static_cast<CAknFepCtrlRepeatButton*>
                    ( ControlById( EAknFepVkbCtrlIdArrowLeftBtn ) )->SizeChanged(arrowRect, 
                                                                                 innerRect, 
                                                                                 ETrue);
                    
                arrowRect.iTl = TPoint(arrowRect.iBr.iX, arrowRect.iTl.iY);
                arrowRect.SetSize(TSize(btnWidth + btnWidth, btnHeight));
                AknPenImageUtils::CalculateGraphicRect( arrowRect, innerRect );
                static_cast<CAknFepCtrlRepeatButton*>
                    ( ControlById( EAknFepVkbCtrlIdArrowRightBtn ) )->SizeChanged(arrowRect, 
                                                                                  innerRect, 
                                                                                  ETrue);
                }
            
            compositionRect.iTl = TPoint(optionRect.iBr.iX, optionRect.iTl.iY);
            compositionRect.SetSize(TSize(btnWidth * 3 + iCellAidGap - GapValue, 
            		candidateHeight));
            }
	    else
	        {
	        compositionRect.iTl = Rect().iTl;
	        // move the composition area rect with gap
			compositionRect.iTl.iX += GapValue;
			compositionRect.SetSize( 
					TSize( btnWidth * 3 + iCellAidGap - GapValue * 2, candidateHeight ));
	        }
        iCompositionField->SetReady(ETrue);
        iCompositionField->Hide(EFalse);
        iCompositionField->SizeChanged(compositionRect);    
        
        TRect candidateRect;
        candidateRect.iTl = TPoint(compositionRect.iBr.iX, compositionRect.iTl.iY);
        candidateRect.SetSize(TSize(btnWidth * 6, candidateHeight));        
             
        static_cast<CFepCtrlDropdownList*>(ControlById( EAknFepVkbCtrlIdCandidateList))->
        		SizeChanged(btnWidth, candidateHeight, btnWidth, candidateRect.iTl);    
        if( iDropdownList && iDropdownList->Hiden() )
        	iDropdownList->Hide(EFalse);      
        
        ReorganizeZhuYinControls( ETrue );
        }
    else
        {
        // Hide composition field when is not native range, since
        // Hide process also can deregister its tactile feedback
        // area, or else click button '3' when it is dimmed will 
        // cause tactile feedback, because '3' is just at the same
        // place in number range as the composition filed in the 
        // native range.
        if (iCompositionField)
            iCompositionField->Hide(ETrue);
                
        if( iDropdownList && !iDropdownList->Hiden() )
        	iDropdownList->Hide(ETrue);
        }
    iVkbBoardLeftTop = Rect().iTl;
    if (range == ERangeNative)
        {
        if( layoutType == EAknFepVkbImCnZhuyin )
            {
            iVkbBoardLeftTop = TPoint(optionRect.iTl.iX, optionRect.iBr.iY);
            }
        else
            {
             // move the gap
			compositionRect.iTl.iX -= GapValue;
            iVkbBoardLeftTop = TPoint(compositionRect.iTl.iX, compositionRect.iBr.iY);
            }
        }
    //TPoint vkbLeftTop = iVkbBoardLeftTop;
	iVkbBoardLeftTop.iX += UiLayout()->PenInputType() == EPluginInputModeFSQ ?
						   iLafMgr->KeypadRect().iTl.iX : 0;
	// After update LAF data for grid gap feature, the keyboard height is cut.
	// This modification affact FSQ window size.
	TInt yAxisOffset = UiLayout()->PenInputType() == EPluginInputModeFSQ ?
						   3 : 0;				   
	iVkbBoardLeftTop.iY += yAxisOffset;
						   	   						   
    TRect vkbRect = TRect( iVkbBoardLeftTop, iLafMgr->KeypadRect().Size() );
    CPeninputVkbCtrlExt* vkbCtrl = 
                        static_cast<CPeninputVkbCtrlExt*>(ControlById( EAknFepVkbCtrlIdVkbCtrl));
    vkbCtrl->SetRect( vkbRect );
    vkbCtrl->SetFont( iLafMgr->KeyTextFont() );
	vkbCtrl->SetTextLineLayout( iLafMgr->KeyTextLayout() );
    vkbCtrl->SetTextFormat( iLafMgr->KeyTextLayout() );  //Read laf 
    
    TInt tempHeightForFSQ = btnHeight;
    //if (UiLayout()->PenInputType() == EPluginInputModeFSQ )
    //	{
    //    tempHeightForFSQ = btnHeight + 3;	
    //	}
    
    TPoint rangeBarRefPoint = TPoint(vkbRect.iTl.iX, vkbRect.iBr.iY);
    //rangeBarRefPoint.iY += yAxisOffset;
    
    if (range != ERangeNative)
        {
        TRect shiftRect;
        shiftRect.iTl = TPoint(Rect().iTl.iX, vkbRect.iBr.iY);
        //shiftRect.iTl.iY += yAxisOffset; 
        shiftRect.SetSize(TSize(btnWidth, tempHeightForFSQ)); 
        AknPenImageUtils::CalculateGraphicRect( shiftRect, innerRect );       
        ControlById( EAknFepVkbCtrlIdShiftBtn)->SetReady(ETrue);    
        static_cast<CAknFepCtrlMultiModeButton*>(ControlById( EAknFepVkbCtrlIdShiftBtn))->
        					SizeChanged(shiftRect, innerRect, ETrue);    
        
        rangeBarRefPoint = TPoint(shiftRect.iBr.iX, shiftRect.iTl.iY);
        }
    
    TRect rangeBarRect;
    //rangeBarRect.iTl = TPoint(shiftRect.iBr.iX, shiftRect.iTl.iY);
    rangeBarRect.iTl = rangeBarRefPoint;
    rangeBarRect.SetSize(TSize(btnWidth, tempHeightForFSQ));
    static_cast<CAknFepCtrlRangeBar*>
                   (ControlById( EAknFepVkbCtrlIdRangeBar))->SizeChanged(rangeBarRect);
    rangeBarRect = 
                static_cast<CAknFepCtrlRangeBar*>(ControlById( EAknFepVkbCtrlIdRangeBar))->Rect();

    TRect spaceBtnRect;
    TInt spaceWidthCount = 4;
 /*   if( range == ERangeNumber && UiLayout()->PenInputType() == EPluginInputModeFSQ )
    	{
    	spaceWidthCount = 5;
    	}*/
    	
    if (range == ERangeNative || curLanguage == ELangTaiwanChinese)
        {
        spaceWidthCount =  (vkbLayoutId == EAknFepVkbVkbCnZhuyinInitial) ? 6 : 5;
        }
    
    TInt defaultIMLayout = iDataProvider->RequestData(EAknFepDataTypeDefaultIMLayout);
    if (curLanguage == ELangTaiwanChinese && 
        defaultIMLayout == EAknFepVkbImCnStroke && range != ERangeNative)
        {
        spaceWidthCount = 4;
        }
    
    spaceBtnRect.iTl = TPoint(rangeBarRect.iBr.iX, rangeBarRect.iTl.iY);
    spaceBtnRect.SetSize(TSize(btnWidth * spaceWidthCount + iCellAidGap, tempHeightForFSQ));
    //AknPenImageUtils::CalculateGraphicRect( spaceBtnRect, innerRect );
    innerRect = spaceBtnRect;
    TSize iconOffset = iLafMgr->GetSpaceIconOffset();
    innerRect.Shrink( iconOffset );
    
    static_cast<CAknFepCtrlEventButton*>
        (ControlById(EAknFepVkbCtrlIdSpaceBtn))->SizeChanged(spaceBtnRect, innerRect, ETrue);
        
    TRect spaceIconRect = innerRect;
    spaceIconRect.Shrink( iLafMgr->GetSpaceSideIconWidth(), 0);
	static_cast<CAknFepCtrlEventButton*>
        (ControlById(EAknFepVkbCtrlIdSpaceBtn))->SetDraw3PieceFrameInfo(
                            TDraw3PiecesFrame(KAknsIIDQgnIndiInputSpaceL,
													  KAknsIIDQgnIndiInputSpaceMiddle,
													  KAknsIIDQgnIndiInputSpaceR,
													  spaceIconRect));    
    
    TRect returnBtnRect;
    returnBtnRect.iTl = TPoint(spaceBtnRect.iBr.iX, spaceBtnRect.iTl.iY);
    returnBtnRect.SetSize(TSize(btnWidth, tempHeightForFSQ));
    AknPenImageUtils::CalculateGraphicRect( returnBtnRect, innerRect );
    static_cast<CAknFepCtrlEventButton*>
        (ControlById(EAknFepVkbCtrlIdEnterBtn))->SizeChanged(returnBtnRect, innerRect, ETrue);
        
    TRect inputModeSwitchBtnRect;
    inputModeSwitchBtnRect.iTl = TPoint(returnBtnRect.iBr.iX, returnBtnRect.iTl.iY);
    inputModeSwitchBtnRect.SetSize(TSize(btnWidth, tempHeightForFSQ));
    AknPenImageUtils::CalculateGraphicRect( inputModeSwitchBtnRect, innerRect );
    static_cast<CAknFepCtrlEventButton*>
        (ControlById(EAknFepVkbCtrlIdSwitchToHwrBtn))->SizeChanged(inputModeSwitchBtnRect, 
                                                                   innerRect, 
                                                                   ETrue);        
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::Reset
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbClientArea::Reset(TBool aImLayoutChanged)
    {
    iActiveCtrl = NULL;
    iCtrlList.Reset();

    if (aImLayoutChanged)
        {
        iVkbCtrl->Reset();
        }
    }

// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::HandleControlEvent
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbClientArea::HandleControlEvent(TInt aEventType,
                                              CFepUiBaseCtrl* aCtrl,
                                              const TDesC& aEventData)
    {
    TInt* data = (TInt*) aEventData.Ptr();
    TInt layoutType = iDataProvider->RequestData(EAknFepDataTypeIMLayout); 
    switch (aEventType)
        {
        case EVkbEventResetShift:
            LoopVkbLayout(EVkbCapslock, ETrue);
            break;

        case EVkbEventSetPermittedRange:
            iRangeBar->SetPermittedRanges(*data);
            break;
        case EVkbEventPositionChanged:
            {
            TInt style = *( ( TUint16* ) aEventData.Ptr() );
            CalculatePosition( style );
            }
            break;
                
        //Events need to send to composition field 
        case EVkbEventWindowClose:
        case EVkbEventCandidateSelected:
        case EVkbEventCompFieldAnalysisResponse:
        case EPeninputLayoutEventBack:
        case EVkbEventKeySpace:
        case EVkbEventKeyEnter:
        case EEventVirtualKeyUp:
        case EVkbEventLeftArrow:
        case EVkbEventRightArrow:
            {
            TInt imLayout = iDataProvider->RequestData(EAknFepDataTypeIMLayout);

            if ((imLayout == EAknFepVkbImCnStroke) && (aEventType == EVkbEventKeySpace))
            	{
                TBuf<1> buf;
                buf.Append(KStrokeSeparator);
                iCompositionField->HandleControlEvent(aEventType,aCtrl,buf);
            	}
            else if(((imLayout == EAknFepVkbImCnPinyin)) 
                    && (aEventType == EVkbEventKeySpace))
                {
                TBuf<1> buf;
                buf.Append(KPinyinSeparator);
                iCompositionField->HandleControlEvent(aEventType,aCtrl,buf);
                }
            else
                {
                iCompositionField->HandleControlEvent(aEventType,aCtrl,aEventData);	
                }
            }

            break;
        case EVkbEventCompFieldDataFlush:
            UiLayout()->SignalOwner(ESignalKeyEvent,aEventData);                         
            ClearCandidatesInList();
            ReorganizeZhuYinControls( ETrue ); 
            break;
            
        case EVkbEventClearContent:
            {
            TBool isChineseInputting = EFalse;
            TInt imLayout = iDataProvider->RequestData(EAknFepDataTypeIMLayout);
            if(imLayout == EAknFepVkbImCnPinyin ||
               imLayout == EAknFepVkbImCnZhuyin ||
               imLayout == EAknFepVkbImCnStroke ||
               imLayout == EAknFepVkbImCnCangjie)
                {
                isChineseInputting = ETrue;
                }
            
            if (isChineseInputting)
                {
                //Clear context in composition field
                iCompositionField->HandleControlEvent(aEventType,aCtrl,aEventData);
                }
            //Clear all candidates in the list if needed
            if ( layoutType!= EAknFepVkbImLatin )
                {
                if (isChineseInputting)
                    {
                    ClearCandidatesInList();
                    }
                ReorganizeZhuYinControls( ETrue ); 
                }
            }
            break;            
        case EVkbEventCompFieldNoChars:
            //Clear all candidates in the list
            ClearCandidatesInList();
            ReorganizeZhuYinControls( ETrue ); 
            break;
            
        case EEventControlFocusGained:
            if ( *data == ECompotisionFieldGetFocus )
                {
                iCompositionField->HandleControlEvent(aEventType,aCtrl,aEventData);
                }
            break;
        case EEventControlFocusLost:
            if ( ( aCtrl ) && ( aCtrl->ControlId() == EPeninputWindowCtrlIdMultiLineICF) )
                {
                iCompositionField->HandleControlEvent(aEventType,aCtrl,aEventData);
                }
            break;
        //Events sent to composition field are over
        
        case EVkbEventResetShiftCapslock:
//            ChangeButtonStatus(EFalse, EAknFepVkbCtrlIdShiftBtn);
//            ChangeButtonStatus(EFalse, EAknFepVkbCtrlIdCapslockBtn);
            ChangeMergedButtonStatus(0, 0);
            break;
        case EVkbEventCandidatesChanged:
            {
            RPointerArray<HBufC>* list = (RPointerArray<HBufC>*)aCtrl;
            if (list && iCompositionField->IsValidSpell() )
                {
                ReorganizeZhuYinControls( EFalse );
                if(UiLayout()->PenInputType() == EPluginInputModeFSQ)
                    {
                    TRAP_IGNORE(iDropdownList->SetCandidatesL(*list,
                                CFepCtrlDropdownList::EListExpandableWithBubble));
                    }
                else
                    {
                    TRAP_IGNORE(iDropdownList->SetCandidatesL(*list,
                                CFepCtrlDropdownList::EListExpandable));
                    }
                }               
            else
                {
                ClearCandidatesInList();    
				// Not display arrow keys when composition field get focus
                if(!(iCompositionField->IsFocused()))
                    {
                    ReorganizeZhuYinControls( ETrue );
                    }
                }
            }
            break;
       	case EVkbEventGetNextPageCandidate:
       	    {
            RPointerArray<HBufC>* list = (RPointerArray<HBufC>*)aCtrl;
            
            if (list)
                {
            	TRAP_IGNORE(iDropdownList->AddCandidatesL(*list,
                                                        CFepCtrlDropdownList::EListExpandable));
                }
       	    }
       	    break;
        case EVkbEventComposition2Standby:
            //Change vkb layout to standy ui state
            if ( layoutType == EAknFepVkbImCnPinyin)
                {
                ChangeVkbLayout(EAknFepVkbVkbCnPinyinInitial);
                }
            else if ( layoutType == EAknFepVkbImCnZhuyin)
                {
                ChangeVkbLayout(EAknFepVkbVkbCnZhuyinInitial);
                }           
            else if ( layoutType == EAknFepVkbImCnStroke || EAknFepVkbImCnCangjie)
                {
                iVkbCtrl->CancelDims();
                }       
            break;
        case EVkbEventStandby2Composition:
        	TRAP_IGNORE( HandleStandby2CompositionEventL() );
            break;
        default:
            break;    
        }      
    }

// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::CompositionFieldStringLength
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CAknFepVkbClientArea::CompositionFieldStringLength()
    {
    if ( iCurrentImLayout->FindControlInfo(EAknFepVkbCtrlIdCompositionField) )
        {
        if ( iCompositionField )
            {
            return iCompositionField->Length();
            }        
        }
        
    return 0;
    }

// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::GetDefaultVkbLayout
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TAknFepVkbLayout CAknFepVkbClientArea::GetDefaultVkbLayout(const TInt aRange)
    {
    TInt defaultIMLayout = iDataProvider->RequestData(EAknFepDataTypeDefaultIMLayout);
    
    TInt curLanguage = iDataProvider->RequestData(EAknFepDataTypeInputLanguage);
    TInt imLayout = iDataProvider->RequestData(EAknFepDataTypeIMLayout);
    switch (aRange)
        {
        case ERangeNative:
            {
            switch (imLayout)
                {
                case EAknFepVkbImCnPinyin:
                    return EAknFepVkbVkbCnPinyinInitial;
                case EAknFepVkbImCnZhuyin:
                    return EAknFepVkbVkbCnZhuyinInitial;
                case EAknFepVkbImCnStroke:
                    return EAknFepVkbVkbCnStroke;
                case EAknFepVkbImCnCangjie:
                    return EAknFepVkbVkbCnCangjie;                
                }
            }
        	if (curLanguage == ELangTaiwanChinese && defaultIMLayout != EAknFepVkbImCnStroke)
        		{
        		return EAknFepVkbVkbLatinForZhuyin;
        		}
        	else 
        		{
        		return EAknFepVkbVkbLatin;
        		}
        case ERangeEnglish:
        	if (curLanguage == ELangTaiwanChinese && defaultIMLayout != EAknFepVkbImCnStroke)
        		{
        		return EAknFepVkbVkbLatinForZhuyin;
        		}
        	else 
        		{
        		return EAknFepVkbVkbLatin;
        		}
        	    
        case ERangeNumber:
        	if (curLanguage == ELangTaiwanChinese && defaultIMLayout != EAknFepVkbImCnStroke)
        		{
        		return EAknFepVkbVkbNumberForZhuyin;
        		}
        	else 
        		{
        		return EAknFepVkbVkbNumber;
        		}        
        case ERangeSymbol:
            return EAknFepVkbVkbSymbol;
        case ERangeAccent:
            {
            TInt accent = iDataProvider->RequestData(EAknFepDataTypeCurrentAccent);    
            switch (accent)
                {
                case EAknFepVkbVkbAccented1:
                    return EAknFepVkbVkbAccented1;
                case EAknFepVkbVkbAccented2:
                    return EAknFepVkbVkbAccented2; 
                case EAknFepVkbVkbAccented3:
                    return EAknFepVkbVkbAccented3; 
                default:
                    return EAknFepVkbVkbAccented1;
                }    
            }
        default:
        	if (curLanguage == ELangTaiwanChinese && defaultIMLayout != EAknFepVkbImCnStroke)
        		{
        		return EAknFepVkbVkbLatinForZhuyin;
        		}
        	else 
        		{
        		return EAknFepVkbVkbLatin;
        		}
        }
    }
              
// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::SetWholeWindowRect
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbClientArea::SetWholeWindowRect(const TRect& aRect)
    {
    TRect rect = aRect;
    
    TInt btnHeight = iDataProvider->RequestData(EAknFepDataTypeUnitHeight);
    //rect.iTl.iY = rect.iTl.iY - iDataProvider->RequestData(EAknFepDataTypeUnitHeight) * 2;
    TInt imLayout = iDataProvider->RequestData(EAknFepDataTypeIMLayout);
    TInt titlebarHeight = iDataProvider->RequestData(EAknFepDataTypeTitleBarHeight); 

    rect.iTl.iY = aRect.iTl.iY - titlebarHeight;
    
    
    iTopParent->SetRect(rect);
    
    TPoint tl = TPoint( iTopParent->Rect().iTl.iX,
                        iTopParent->Rect().iTl.iY + titlebarHeight );
    
    //rect.SetSize(TSize((10 * iUnitWidth), iDataProvider->RequestData(EAknFepDataTypeUnitHeight) * 2 + 6 * iUnitHeight));

    SetRect( TRect( tl, iTopParent->Rect().iBr ) );
    

    }

// ---------------------------------------------------------------------------
// CAknFepVkbWindow::CalculatePosition
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbClientArea::CalculatePosition( TInt aJustifyMode )
    {
    switch ( aJustifyMode )
        {
        case EVkbPositionChangeBrJustify:
            BottomRightJustify();
            break;
            
        case EVkbPositionChangeDataQuery:
            //BottomRightJustify();
            DataQueryJustify();
            break;
        
        /*case EVkbPositionChangeNone:
            break;*/
            
        default:
            TopLeftJustify();
            break;;                    
        }        
    }

// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::TopLeftJustify
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//   
void CAknFepVkbClientArea::TopLeftJustify()
    {
    // Read position from datamgr 
    TRect newRect = TRect(UiLayout()->Position(),UiLayout()->Rect().Size());
    TPoint tl = UiLayout()->Position();     
               
    if (tl.iX < 0)
        {
        tl.iX = 0;
        }
    
    if (tl.iY < 0)
        {
        tl.iY = 0;  
        }
        
    newRect.iTl = tl;
    newRect.SetSize(iTopParent->Rect().Size());    
    
    if (newRect != UiLayout()->Rect())
        {
        //Write the new position into CenRep
        TInt x = 0;
        TInt y = 0;
        x = 0x0000FFFF & newRect.iBr.iX;
        y = 0xFFFF0000 & ( newRect.iBr.iY << 16 );      
        iDataProvider->SetData(EAknFepDataTypeWindowPosition,x | y);                    
        }
        
    //Set the layout position
    UiLayout()->SetRect(newRect);
    }

// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::BottomRightJustify
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
void CAknFepVkbClientArea::BottomRightJustify()
    {
    if(UiLayout()->PenInputType() == EPluginInputModeFSQ)
        {
        return;
        }    
    
    // Read position from datamgr 
    TRect newRect = TRect(UiLayout()->Position(),UiLayout()->Rect().Size());
    TPoint oldbr,br;
    TPoint tl;  
    TSize screenSize;
    
    TInt screenSizeData = iDataProvider->RequestData(EAknFepDataTypeScreenSize);
    screenSize.iWidth = screenSizeData & 0x0000FFFF;
    screenSize.iHeight = ( screenSizeData & 0xFFFF0000 ) >> 16; 
    
    TRect screenRect = TRect(TPoint(0,0),screenSize);
    
    TInt windowPosition = iDataProvider->RequestData(EAknFepDataTypeWindowPosition);

    oldbr.iX = windowPosition & 0x0000FFFF;
    oldbr.iY = ( windowPosition & 0xFFFF0000 ) >> 16;    
    
    br = oldbr;
        
    if ( !screenRect.Contains(br) )
        {
        if (screenSize.iWidth < br.iX)
            {
            br.iX = screenSize.iWidth;
            }
        if (screenSize.iHeight < br.iY)
            {
            br.iY = screenSize.iHeight;
            }
        }
    
    // Check the top left point is in screen
    tl.iX = br.iX - UiLayout()->Rect().Width();
    if (tl.iX < 0)
        {
        tl.iX = 0;
        }

    tl.iY = br.iY - UiLayout()->Rect().Height();        
    if (tl.iY < 0)
        {
        tl.iY = 0;  
        }
        
    if (UiLayout()->PenInputType() == EPluginInputModeFSQ)
        {
        tl.iX = 0;
        tl.iY = 0;
        }
    newRect.iTl = tl;
    newRect.SetSize(UiLayout()->Rect().Size());    
    
    if (oldbr != newRect.iBr)
        {
        //Write the new position into CenRep
        TInt x = 0;
        TInt y = 0;
        x = 0x0000FFFF & newRect.iBr.iX;
        y = 0xFFFF0000 & ( newRect.iBr.iY << 16 );
        iDataProvider->SetData(EAknFepDataTypeWindowPosition,x | y);

        //Set the layout position
        UiLayout()->SetRect(TRect(tl,newRect.Size()));
        }
        
    //Set the layout position
    UiLayout()->SetRect(newRect);
    }

// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::DataQueryJustify
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//   
void CAknFepVkbClientArea::DataQueryJustify()
    {
    TRect* dataQueryRect = (TRect*) (iDataProvider->RequestData(EAknFepDataTypeDataQueryRect));
    
    TRect rect = TRect(UiLayout()->Position(),UiLayout()->Rect().Size());
    
	CCoeEnv* coeEnv = CCoeEnv::Static();
    TPixelsTwipsAndRotation ptSize;     
    coeEnv->ScreenDevice()->GetDefaultScreenSizeAndRotation(ptSize);    

	// Landscape mode: move window to the br of the screen
    if (ptSize.iPixelSize.iWidth > ptSize.iPixelSize.iHeight)
    	{
    	rect.SetRect(ptSize.iPixelSize.iWidth - rect.Width(), 
    			     ptSize.iPixelSize.iHeight/2 - rect.Height()/2,
    			     ptSize.iPixelSize.iWidth, 
    			     ptSize.iPixelSize.iHeight/2 + rect.Height()/2);
    	}
	// Portrait mode: dodge the dataquery   	
    else if ( rect.Intersects(*dataQueryRect) )
        {
        TInt Y = dataQueryRect->iTl.iY;
        TInt height = rect.iBr.iY - rect.iTl.iY;
        rect.iTl.iY = Y - height;
        rect.iBr.iY = Y;
        if( rect.iTl.iY <0 )
            {
            rect.iTl.iY = 0;
            rect.iBr.iY = height;
            }
        }


    //Write the new position into CenRep
    TInt x = 0;
    TInt y = 0;
    x = 0x0000FFFF & rect.iBr.iX;
    y = 0xFFFF0000 & ( rect.iBr.iY << 16 );     
    iDataProvider->SetData(EAknFepDataTypeWindowPosition,x | y);           

    //Set the layout position
    UiLayout()->SetRect(rect);

    /*    
    TInt fontHeight = aData & 0x0000FFFF;
    
    TInt bottomY = ( aData & 0xFFFF0000 ) >> 16;
    
    if ( ( (bottomY - fontHeight) >= UiLayout()->Rect().iBr.iY )
        || ( (bottomY + fontHeight)  <= UiLayout()->Rect().iTl.iY ) )
        {
        return; 
        }
    else
        {
        tl = UiLayout()->Rect().iTl;
        br = UiLayout()->Rect().iBr;
        TInt middleY = UiLayout()->Rect().iTl.iY +  UiLayout()->Rect().Height() / 2;
        if ( middleY <= bottomY )
            {
            br.iY = bottomY - fontHeight;
            tl.iY = br.iY - UiLayout()->Rect().Height();
            if (tl.iY < 0) // Then skip to the bottom of the current editor
                {
                tl.iY = bottomY + fontHeight;
                br.iY = tl.iY + UiLayout()->Rect().Height();                                        
                }
            }
        else
            {
            tl.iY = bottomY + fontHeight;
            br.iY = tl.iY + UiLayout()->Rect().Height();
            if (br.iY > screenSize.iHeight)
                {
                br.iY = bottomY - fontHeight;
                tl.iY = br.iY - UiLayout()->Rect().Height();                        
                }
            }
            
        TInt x = 0;
        TInt y = 0;
        x = 0x0000FFFF & br.iX;
        y = 0xFFFF0000 & ( br.iY << 16 );       
        iDataProvider->SetData(EAknFepDataTypeWindowPosition,x | y);                                
        }
    */        
    }

        
// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::SetTopParent
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbClientArea::SetTopParent(CFepUiBaseCtrl* aParent)
    {
    iTopParent = aParent;
    }     


// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::AddDropdownListL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbClientArea::AddDropdownListL()
    {
    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC(reader,R_AKN_FEP_VKB_DROP_DOWN_LIST);
           
    if (!iDataProvider->UnitShadowSizeSet())
    	{
        iDataProvider->ReadUnitShadowSize();
    	}
    	
    
    TInt btnWidth = iDataProvider->RequestData(EAknFepDataTypeUnitWidth);
    TInt btnHeight = iDataProvider->RequestData(EAknFepDataTypeUnitHeight);
        	
    	
    //TAknTextLineLayout listText = 
    //    AknLayoutScalable_Avkon::cell_vkb_candidate_pane_t1().LayoutLine();
    TAknTextLineLayout listText = 
        AknLayoutScalable_Avkon::cell_vkb2_top_candi_pane_t1(0).LayoutLine();

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

    iDropdownList = CFepCtrlDropdownList::NewL(TPoint(0,btnWidth),
                                               UiLayout(),
                                               EAknFepVkbCtrlIdCandidateList,
                                               reader,                                            
                                               0,
                                               0,
                                               btnWidth,
                                               6,
                                               4,
                                               AknLayoutUtils::FontFromId(listText.iFont, NULL),
                                               NULL,
                                               CGraphicsContext::ECenter,
                                               3,
                                               KRgbBlack,       // border color 
                                               TRgb(0xeeeeee),  // content bg color
                                               KRgbWhite,       // navi bg color
                                               AKN_LAF_COLOR_STATIC(listText.iC),
                                               KRgbRed);

    iDropdownPropertySet = ETrue;                                               

    iDropdownList->SetResourceId(R_AKN_FEP_VKB_DROP_DOWN_LIST);
    iDropdownList->SetEventIdForCandidateSelected(EVkbEventCandidateSelected);
    iDropdownList->SetEventIdForNextPageCandidate(EVkbEventGetNextPageCandidate);
    iDropdownList->SetEventIdForPreviousPageCandidate(EVkbEventGetPreviousPageCandidate);
    iDropdownList->SetEventIdForCandidateExisted(EVkbEventQueryCandidateExisted);
	iDropdownList->SetDropdownListImgID(candiDrawInfo);
    //iDropdownList->AddEventObserver(UiLayout());
    CleanupStack::PopAndDestroy(); // reader
    if ( iCtrlPool )
        {
        iCtrlPool->AddControl(iDropdownList);    
        }
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::AddButtonL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CAknFepCtrlEventButton* CAknFepVkbClientArea::AddButtonL(const TInt aControlId,
                                                         const TInt aEventId,
                                                         const TInt aResId,
                                                         const TInt aUnicode,
                                                         const TBool aIsRepeat)
    {
    CAknFepCtrlEventButton* button = NULL;
    
    if ( aIsRepeat )
        {
        button = CAknFepCtrlRepeatButton::NewL( UiLayout(),
        										aControlId, 
        										aEventId, 
        										aUnicode,
        										KAknsIIDQsnFrFunctionButtonNormal,
        									  KAknsIIDQsnFrFunctionButtonPressed,
        									  KAknsIIDQsnFrFunctionButtonInactive);
        }
    else
        {
        button = CAknFepCtrlEventButton::NewL(UiLayout(),
        									  aControlId, 
        									  aEventId, 
        									  aUnicode,
    									      KAknsIIDQsnFrFunctionButtonNormal,
        									  KAknsIIDQsnFrFunctionButtonPressed,
        									  KAknsIIDQsnFrFunctionButtonInactive);
        }
    
    if ( button )
        {
        // Read resource
        TResourceReader reader;
        CCoeEnv::Static()->CreateResourceReaderLC(reader, aResId); 
        button->SetResourceId( aResId );    
        button->ConstructFromResourceL();  
        CleanupStack::PopAndDestroy(); // reader
        
        // Add into the control pool                                                   
        if ( iCtrlPool )
            {
            iCtrlPool->AddControl(button);    
            }    
        }
    return button;
    }    

// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::AddRangeBarL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbClientArea::AddRangeBarL()
    {
    // Range bar             
    iRangeBar = CAknFepCtrlRangeBar::NewL(UiLayout(),
                                          EAknFepVkbCtrlIdRangeBar,
                                          CAknFepCtrlRangeBar::EBtnGroupHorizontal);
                                          
    iRangeBar->SetActionStyle( CAknFepCtrlRangeBar::ERangeBarActionStyleHighLight );
    
    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC(reader, R_AKN_FEP_VKB_RANGE_LIST);
    iRangeBar->ConstructFromResourceL(reader);
    CleanupStack::PopAndDestroy(); // reader
    
    //Set event id    
    iRangeBar->SetEventIdForRange(EVkbEventSetRange);

    iCtrlPool->AddControl(iRangeBar);    
    }           
         
// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::ChangeMergedButtonStatus
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
void CAknFepVkbClientArea::ChangeMergedButtonStatus( 
											const TInt aIsShiftCase, 
    								 		const TInt aIsCapslockCase )
	{
	CAknFepCtrlMultiModeButton* button = 
		static_cast<CAknFepCtrlMultiModeButton*>
        (ControlById( EAknFepVkbCtrlIdShiftBtn ) );
    
    if( !button )
        {
        return;
        }
    switch ( aIsCapslockCase * 2 + aIsShiftCase )
        {
        case 1: // text case        
            {
            TRAP_IGNORE( button->SetCurrentModeL( EBtnBmpActive, 1 ) );			
	    	button->SetHighlight( ETrue );
            }
            break; 
        case 2: // upper case
            {
            TRAP_IGNORE( button->SetCurrentModeL( EBtnBmpActive, 2 ) );	
            button->SetHighlight( ETrue );
            }
            break;
        default:
            {
            TRAP_IGNORE( button->SetCurrentModeL( EBtnBmpActive, 0 ) );	
        	button->SetHighlight( EFalse );
            }
            break;
	    }
	}      
    
    
// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::CompositionField
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CFepLayoutEditAreaBase* CAknFepVkbClientArea::CompositionField()
    {
    if ( iCurrentImLayout && iCurrentImLayout->FindControlInfo(EAknFepVkbCtrlIdCompositionField) ) 
        {
        return iCompositionField;
        }
    return NULL;
    }

// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::AddControlL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbClientArea::AddControlL(CFepUiBaseCtrl* aControl)
    {
    //do nothing if we already has the control.
    if(KErrNotFound != iCtrlList.Find(aControl))
        return;
    
    if(aControl)
        {        
        aControl->SetReady( Ready() );
        iCtrlList.AppendL(aControl);   

        RRegion region(8);
        region.Copy(Region());   

        region.AddRect(aControl->Rect());
        SetRegion(region);
        }
    }    

// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::DimKeys
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbClientArea::DimKeys()
    {
    TRAP_IGNORE(DimKeysL());
    }

void CAknFepVkbClientArea::DimKeysL()
    {
    iVkbCtrl->CancelDims();
    TInt currentRange = iDataProvider->RequestData(EAknFepDataTypeCurrentRange);            
    TInt permitRange = iDataProvider->RequestData(EAknFepDataTypePermittedRange);            
    if ( (currentRange == ERangeNumber )
        && ( permitRange == ERangeNumber ) )
        {
        TInt keyMapping = iDataProvider->RequestData(EAknFepDataTypeNumericKeymap);
        HBufC* rs = NULL;
        if ( keyMapping == EKeymapFromResource )
            {
            HBufC* customRes = 
                           (HBufC*) iDataProvider->RequestData(EAknFepDataTypeUseDefinedResource);
            _LIT(KTenNumbers,"0123456789");
            rs = HBufC::NewL(KTenNumbers().Length() + customRes->Length());
            rs->Des().Copy(*customRes);
            rs->Des().Append(KTenNumbers);
            iVkbCtrl->DimKeys(rs);        
            }
        else
            {
            rs = iKeyMappingMgr->KeyMappingStringL(keyMapping);
            iVkbCtrl->DimKeys(rs);
            }
            
        delete rs;
        }    
    iVkbCtrl->Draw();    
    UpdateArea(iVkbCtrl->Rect(), EFalse);
    }

// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::ClearCandidatesInList
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbClientArea::ClearCandidatesInList()
    {
    RPointerArray<HBufC> candidates;
    if(UiLayout()->PenInputType() == EPluginInputModeFSQ)
        {
        TRAP_IGNORE(iDropdownList->SetCandidatesL(candidates,
            CFepCtrlDropdownList::EListNoExpandableWithBubble));
        }
    else
        {
        TRAP_IGNORE(iDropdownList->SetCandidatesL(candidates,
            CFepCtrlDropdownList::EListNoExpandable));
        }

    iDataProvider->SetData(EAknFepDataTypeCandidateCount,0);
    candidates.Close();            
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::NeedCreateWord
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool CAknFepVkbClientArea::NeedCreateWord()
	{
    return iCompositionField->NeedCreateWord();
	}
    
// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::SetNextCandidateExistFlag
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//  
void CAknFepVkbClientArea::SetNextCandidateExistFlag(TBool aFlag)
    {
	iDropdownList->SetFlagCandidateExist(aFlag);
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::ConstructFromResourceL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//  
void CAknFepVkbClientArea::ConstructFromResourceL()
    {                
    CControlGroup::ConstructFromResourceL();
    // Construct for rest controls not in the control group
    for(TInt i = 0; i < iRemainedPoolControlIDs.Count(); i++ )
    	{
    	iCtrlPool->Control( iRemainedPoolControlIDs[i] )->ConstructFromResourceL();	
    	}
    	
    if( iVkbLayout )
    	{
    	if( iVkbCtrl->IsIrregularKeyBoard() )    	
    		iVkbLayout->ConstructFromIrregularResourceL();
    	else
    		iVkbLayout->ConstructFromNonIrregularResourceL();    	
    	SetVkbLayoutSize( iVkbLayoutId );
    	}
//    iVkbLayout->ConstructFromResourceL();
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::SetEnableSwitchToHwr
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//     
void CAknFepVkbClientArea::SetEnableSwitchToHwr(TBool aEnable)
    {
    CAknFepCtrlEventButton* switchToHwrBtn = static_cast<CAknFepCtrlEventButton*>
        ( iCtrlPool->Control( EAknFepVkbCtrlIdSwitchToHwrBtn ) );

    if ( aEnable )    
        {
        switchToHwrBtn->SetDimmed(EFalse);
        }
    else
        {
        switchToHwrBtn->SetDimmed(ETrue);
        switchToHwrBtn->SetActive(EFalse);
        } 
    }

// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::ActiveCurrentRange
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//     
void CAknFepVkbClientArea::ActiveCurrentRange()
    {
    TInt curRange = iDataProvider->RequestData(EAknFepDataTypeCurrentRange); 
    
    if ( iRangeBar )
        {
        iRangeBar->SetRange(curRange);
        }  
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbClientArea::GetCreatedWordSpell
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//     
TPtrC CAknFepVkbClientArea::GetCreatedWordSpell()
    {
    return iCompositionField->GetCreatedWordSpell();
    }
    
TPeninputCnCellType CAknFepVkbClientArea::GetCellType(TInt aColumns)
	{
/*	TInt curRange = iDataProvider->RequestData(EAknFepDataTypeCurrentRange);  
    TBool zhuyinIm = iDataProvider->RequestData(EAknFepDataTypeIMLayout) == EAknFepVkbImCnZhuyin;
    TPeninputCnCellType cellType;
    
    if( UiLayout()->PenInputType() == EPluginInputModeFSQ )
    	{
    	cellType = ( aColumns == 10 )? EVkbCangjieAndPinying_FSQ : EVkbZhuyin_FSQ;
    	}
    else if( UiLayout()->PenInputType() == EPluginInputModeVkb )
    	{
    	if (curRange == ERangeEnglish)
    		{
    		cellType = EVkbLatin;
    		}
    	if (curRange == ERangeNumber)
    		{
    		cellType = EVkbNumber;	
    		}
    	if (curRange == ERangeAccent)
    		{
    		cellType = EVkbAccent;	
    		}      
    	else
    		{
    		cellType = ( zhuyinIm )? EVkbZhuyin : EVkbCangjieAndPinying;
    		}
    	}*/
	TInt curLanguage = iDataProvider->RequestData(EAknFepDataTypeInputLanguage);
	    	
    TPeninputCnCellType cellType = EVkbCangjieAndPinying;
    if( UiLayout()->PenInputType() == EPluginInputModeFSQ )
    	{
	    if (aColumns == 10)
	    	{
	    	cellType = EVkbCangjieAndPinying_FSQ;
	    	}
	    else if(aColumns == 11)
	    	{
	    	cellType = EVkbZhuyin_FSQ;
	    	}
    	}
	else 
		{
	    if (aColumns == 10)
	    	{
	    	cellType = EVkbCangjieAndPinying;
	    	}
	    else if(aColumns == 11)
	    	{
	    	cellType = EVkbZhuyin;
	    	}			
		}    	
    return cellType;
	}
	
void CAknFepVkbClientArea::ShowBubble(TBool aShow)
    {
    if (iVkbCtrl)
        {
        iVkbCtrl->ShowBubble(aShow);    
        }
    if(iDropdownList)
        {
        iDropdownList->ShowBubble(aShow);
        }
    }


void CAknFepVkbClientArea::DimArrowKeys( TBool aDimArrow )
    {
    static_cast<CAknFepCtrlRepeatButton*>
        ( iCtrlPool->Control( EAknFepVkbCtrlIdArrowUpBtn ) )->SetDimmed(aDimArrow);
    static_cast<CAknFepCtrlRepeatButton*>
        ( iCtrlPool->Control( EAknFepVkbCtrlIdArrowDownBtn ) )->SetDimmed(aDimArrow);
    static_cast<CAknFepCtrlRepeatButton*>
        ( iCtrlPool->Control( EAknFepVkbCtrlIdArrowLeftBtn ) )->SetDimmed(aDimArrow);
    static_cast<CAknFepCtrlRepeatButton*>
        ( iCtrlPool->Control( EAknFepVkbCtrlIdArrowRightBtn ) )->SetDimmed(aDimArrow);
    }
void CAknFepVkbClientArea::HandleStandby2CompositionEventL()
    {
    TInt layoutType = iDataProvider->RequestData(EAknFepDataTypeIMLayout); 
    //Change vkb layout to composition ui state
    ReorganizeZhuYinControls( EFalse );
           
    if ( layoutType == EAknFepVkbImCnPinyin)
        {
        ChangeVkbLayout(EAknFepVkbVkbCnPinyinComposition);
        }
    else if ( layoutType == EAknFepVkbImCnZhuyin)
        {
        ChangeVkbLayout(EAknFepVkbVkbCnZhuyinComposition);
        } 
    else if ( layoutType == EAknFepVkbImCnStroke || EAknFepVkbImCnCangjie)
        {
        HBufC* symbolNumSet = HBufC::NewLC( KLengthSymbolNeedToFilter );

        for (TInt index = 0; index < KLengthSymbolNeedToFilter; index ++)
            {
            symbolNumSet->Des().Append( KSymbolsNeedToFilter[index] );
            }
            
        iVkbCtrl->DimKeySet( symbolNumSet, ETrue );
        CleanupStack::PopAndDestroy( symbolNumSet );
        }
    }
// End Of File
