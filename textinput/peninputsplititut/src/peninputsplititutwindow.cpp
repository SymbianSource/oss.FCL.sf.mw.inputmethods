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


#include <peninputsplititutnew.rsg>
#include <peninputsplititutconfig_chinese.rsg>
#include <skinlayout.cdl.h>
#include <AknsDrawUtils.h>
#include <AknsUtils.h>
#include <AknUtils.h> 
#include <peninputrawkeybutton.h>
#include <peninputrepeatbutton.h>
#include <peninputcommonbutton.h>
#include <AknFepGlobalEnums.h>
#include <aknfeppeninputenums.h>
#include <peninputdropdownlist.h>
#include <s32mem.h>
#include <peninputlabel.h>
#include <peninputlayoutbubblectrl.h>
#include <aknlayoutscalable_apps.cdl.h>

#include "peninputsplititutwindow.h"
#include "peninputsplititutdatamgr.h"
#include "peninputsplititutuimgrbase.h"
#include "peninputsplititutuistatebase.h"
#include "peninputsplititutchnuimgr.h"
#include "peninputsplititutwesternuimgr.h"
#include "peninputsplititutconverter.h"
#include "peninputcommonbgctrl.h"

_LIT(KBmpFileName, "z:\\resource\\apps\\peninputsplititut.mbm");
_LIT(KEmptyString, "");

const TInt KImageMajorSkinId = EAknsMajorGeneric;
const TUint KDefaultSpellTextColor = 0;

const TInt KSpaceUnicode = 32;
const TInt KButtonOneUnicode = 49;
const TInt KInvalidBmp = -1;

const TUint KDefaultIcfTextColor = 0x000000;
const TUint KDefaultIcfFrameColor = 0x000000;





// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CSplitItutWindow::NewL
// ---------------------------------------------------------------------------
//
CSplitItutWindow* CSplitItutWindow::NewL(CSplitItutWindowManager* aWindowMgr,
                                             CSplitItutUiLayout* aLayoutOwner,
                                             CSplitItutDataMgr* aDataMgr)
    {
    CSplitItutWindow* window = new (ELeave) CSplitItutWindow(aWindowMgr,aLayoutOwner,aDataMgr);

    CleanupStack::PushL(window);
    window->ConstructL();
    CleanupStack::Pop(window);

    return window;
    }

// ---------------------------------------------------------------------------
// CSplitItutWindow::CSplitItutWindow
// ---------------------------------------------------------------------------
//
CSplitItutWindow::CSplitItutWindow(CSplitItutWindowManager* aWindowMgr, 
                                       CSplitItutUiLayout* aLayoutOwner,
                                       CSplitItutDataMgr* aDataMgr)
    : iDataMgr(aDataMgr),
      iLayoutOwner(aLayoutOwner),
      iWindowMgr(aWindowMgr)
    {
    }

// ---------------------------------------------------------------------------
// CSplitItutWindow::~CSplitItutWindow
// ---------------------------------------------------------------------------
//
CSplitItutWindow::~CSplitItutWindow()
    {
    delete iBmpRotator;
    }


// ---------------------------------------------------------------------------
// CSplitItutWindow::SetPropertyL
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::SetPropertyL(MItutPropertySubscriber::TItutProperty aPropertyName, 
                                        const TDesC& aPropertyValue)
    {
    switch (aPropertyName)
        {
        case MItutPropertySubscriber::EItutPropertyKeypadResourceId:
        case MItutPropertySubscriber::EItutPropertyCandidateListResourceId:
            {
            TInt resId = TItutDataConverter::AnyToInt(const_cast<TUint16*>(aPropertyValue.Ptr()));
            
            if (resId == KInvalidResId)
                {
                return;
                }

            if (aPropertyName == MItutPropertySubscriber::EItutPropertyKeypadResourceId)
                {
                ConstructItutKeypadFromResourceL(resId);
                }
            else if (iCandsList)
                {
                iCandsList->ResetAndClear(CFepCtrlDropdownList::EListExpandable);
                iCandsList->SetResourceId(resId);
                iCandsList->ConstructFromResourceL();                  
                }
            }
            break;
        case MItutPropertySubscriber::EItutPropertyLandscape:
            {
            TBool landscape = *((TInt*)(aPropertyValue.Ptr()));

            if (iDataMgr->IsChinese())
                {
                if (landscape)
                    {
                    // expand upward
                    iSpellCandsList->SetCandExpandType(CFepCtrlDropdownList::ECandExpandUpward);
                    iCandsList->SetCandExpandType(CFepCtrlDropdownList::ECandExpandUpward);
                    }
                else
                    {
                    // portrait, expand downward
                    iSpellCandsList->SetCandExpandType();
                    iCandsList->SetCandExpandType();
                    }
                }
            }
            break;
        default:
            break;    
        }
    }

// ---------------------------------------------------------------------------
// CSplitItutWindow::CreateItutKeypadL
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::CreateItutKeypadL()
    {
    TRect keypadRect = TItutDataConverter::AnyToRect(iDataMgr->RequestData(EKeypadRect));

    TFontSpec spec;

    iStandardItutKp = CVirtualKeyboard::NewL(keypadRect,
                                             iLayoutOwner,
                                             ECtrlIdStdItut,
                                             spec);
    AddControlL(iStandardItutKp);

    iStandardItutKp->SetResourceId(KInvalidResId);
	
	// read keypad image info
	ConstructKeyImageFromResourceL( R_ITUT_KEYPAD_BITMAP );
	
    iStandardItutKp->SetKeyTextColorGroup( EAknsCIQsnTextColorsCG65 );
    iStandardItutKp->SetDrawOpaqueBackground(EFalse);
    
    // read key shift icon res
    CPenInputColorIcon* shiftIcon = CPenInputColorIcon::NewL( R_ITUT_KEYPAD_SHIFT_ICON );
    shiftIcon->ResizeL( iDataMgr->iShiftIconRect.Size() );
    iStandardItutKp->SetShiftIcon( shiftIcon );
    
    // read key star icon res
    CPenInputColorIcon* starIcon = CPenInputColorIcon::NewL( R_ITUT_KEYPAD_STAR_ICON );
    starIcon->ResizeL( iDataMgr->iStarIconRect.Size() );
    iStandardItutKp->SetStarIcon( starIcon );    
    
    //set key to be 9 piece graphics
    iStandardItutKp->SetKeySkinId( EKeyBmpNormal, KAknsIIDQsnFrKeypadButtonFrNormal );
    iStandardItutKp->SetKeySkinId( EKeyBmpHighlight, KAknsIIDQsnFrKeypadButtonFrPressed );
    iStandardItutKp->SetKeySkinId( EKeyBmpDim, KAknsIIDQsnFrKeypadButtonFrInactive );
    }

// ---------------------------------------------------------------------------
// CSplitItutWindow::CreateLayoutIconL
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::CreateLayoutIconL(TInt aMajorSkinId,
                                             TInt aMinorSkinId,
                                             const TDesC& aBmpFileName,
                                             TInt aBmpId,
                                             TInt aMaskBmpId,
                                             CFbsBitmap*& aBmp,
                                             CFbsBitmap*& aMaskBmp,
                                             TSize aSize)
    {
    TAknsItemID id;
    MAknsSkinInstance* skininstance = AknsUtils::SkinInstance();
    id.Set(TInt(aMajorSkinId), aMinorSkinId);

    if (aBmpId != KInvalidBmp)
        {
        if (aMaskBmpId != KInvalidBmp)
            {
            AknsUtils::CreateIconL(skininstance,
                                   id,
                                   aBmp,
                                   aMaskBmp,
                                   aBmpFileName,
                                   aBmpId,
                                   aMaskBmpId);
            AknIconUtils::SetSize(aMaskBmp, aSize, EAspectRatioNotPreserved);
            }
        else
            {
            AknsUtils::CreateIconL(skininstance,
                                   id,
                                   aBmp,
                                   aBmpFileName,
                                   aBmpId);
            }

        AknIconUtils::SetSize(aBmp, aSize, EAspectRatioNotPreserved);
        }
    }

// ---------------------------------------------------------------------------
// CSplitItutWindow::ConstructItutKeypadFromResourceL
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::ConstructItutKeypadFromResourceL(TInt aResId)
    {
    iStandardItutKp->SetResourceId(aResId);

    iStandardItutKp->SetTextLineLayout(
        TItutDataConverter::AnyToTextLine(iDataMgr->RequestData(EKeypadLeftTextLine)),
        EPosLeft);
    iStandardItutKp->SetTextLineLayout(
        TItutDataConverter::AnyToTextLine(iDataMgr->RequestData(EKeypadRightTextLine1)),
        EPosRight1);
    iStandardItutKp->SetTextLineLayout(
        TItutDataConverter::AnyToTextLine(iDataMgr->RequestData(EKeypadRightTextLine2)),
        EPosRight2);
    iStandardItutKp->SetTextLineLayout(
        TItutDataConverter::AnyToTextLine(iDataMgr->RequestData(EKeypadRightTextLine3)),
        EPosRight3);

    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC(reader, aResId);

    // construct keys
    TInt countkeys = reader.ReadInt16();

    RPointerArray<CVirtualKey>& keys = 
        const_cast<RPointerArray<CVirtualKey>&>(iStandardItutKp->KeyArray());

    if (keys.Count() == 0)
        {
        for (TInt i = 0; i < countkeys; i++)
            {
            RArray<TRect>& cellrects = 
                       TItutDataConverter::AnyToRectArray(iDataMgr->RequestData(EKeypadCellRects));

            CVirtualKey* vk = CreateKeyL(reader,
                                         cellrects[i]);
            CleanupStack::PushL(vk);
            iStandardItutKp->AddRawKeyL(vk);
            CleanupStack::Pop(vk);
            }
        }
    else
        {
        countkeys = keys.Count();
        for (TInt i = 0; i < countkeys; i++)
            {
            ChangeVirtualKeyInfoL(reader, keys[i]);
            }
        }

    CleanupStack::PopAndDestroy(1); // reader
	
	iStandardItutKp->Draw();
    iStandardItutKp->UpdateArea( iStandardItutKp->Rect() );
    }

// ---------------------------------------------------------------------------
// CSplitItutWindow::ChangeVirtualKeyInfoL
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::ChangeVirtualKeyInfoL( TResourceReader& aReader, 
                                              CVirtualKey* aKey)
    {

    TBuf<5> mapData;
    //Init Data
    for (TInt i = 0; i < 5; i++)
        {
        mapData.Append(0);
        }
    
    RPointerArray<HBufC> unicodesArr;
    for (TInt i = 0; i <= EPosLast; i++)
        {
        HBufC* unicode = aReader.ReadHBufCL();
        if (i == 0 && unicode != NULL)
            {
            mapData = unicode->Des();
            }
        unicodesArr.AppendL(unicode);
        }

    TInt keyscancode = aReader.ReadInt16();
    if (keyscancode != aKey->ScanCode())
        {
        return;
        }
    
    TInt curMode = TItutDataConverter::AnyToInt(
            iLayoutOwner->DataMgr()->RequestData(EInputMode));
    TBool isChinese = iLayoutOwner->DataMgr()->IsChinese();
    TBool isThai = iLayoutOwner->DataMgr()->IsThai();
    
    TInt latinOnly = TItutDataConverter::AnyToInt(
            iLayoutOwner->DataMgr()->RequestData(ELatinOnly));     
            
    //No symbol in Number range. Symbols under chinese are read from res 
    if ( curMode != ENumber && curMode != ENativeNumber 
         && iSymbolData.iSymbol1 != 0 && keyscancode == KButtonOneUnicode 
         && ( !isChinese || latinOnly )
         && ( !isThai ) )
        {
        //So we should delete it
        for (TInt i = 0; i <= EPosLast; i++)
            {
            delete unicodesArr[i];
            }
        SetUnicodesForHardKey1(aKey, mapData);
        }
    else
        {
        aKey->SetUnicodesL(unicodesArr);
        }

    unicodesArr.Close();
    }

// ---------------------------------------------------------------------------
// CSplitItutWindow::CreateKeyL
// ---------------------------------------------------------------------------
//
CVirtualKey* CSplitItutWindow::CreateKeyL(TResourceReader& aReader,
                                              TRect aKeyRect)
    {
    RPointerArray<HBufC> unicodesArr;
    for (TInt i = 0; i <= EPosLast; i++)
        {
        HBufC* unicode = aReader.ReadHBufCL();
        unicodesArr.AppendL(unicode);
        }

    TInt keyscancode = aReader.ReadInt16();

    CVirtualKey* vk = CVirtualKey::NewL(unicodesArr,
                                        keyscancode, 
                                        aKeyRect);
    unicodesArr.Close();
    CleanupStack::PushL(vk);
    
    TRect innerrect = aKeyRect;
    innerrect.Shrink(TSize(7, 7));
    vk->SetInnerRect(innerrect);

    CleanupStack::Pop(vk);

    return vk;
    }

// ---------------------------------------------------------------------------
// CSplitItutWindow::CreateBackGroundControlL
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::CreateBackGroundControlL()
	{
    iBackgroundCtrl = CAknFepCtrlCommonBgCtrl::NewL(
                                          iLayoutOwner,
                                          ECtrlIdBackground,
                                          KAknsIIDQsnFrPopup,
                                          KAknsIIDQsnFrPopupCenter);


	iBackgroundCtrl->SetRect(TItutDataConverter::AnyToRect(
					iLayoutOwner->DataMgr()->RequestData(EBackgroundRect)));
					    							
	AddControlL(iBackgroundCtrl);
	}


// ---------------------------------------------------------------------------
// CSplitItutWindow::CreateRawKeyButtonL
// ---------------------------------------------------------------------------
//
CAknFepCtrlCommonButton* CSplitItutWindow::CreateRawKeyButtonL(const TInt aControlId,
                                                        const TInt aCommand)
    {
    CAknFepCtrlCommonButton* temp = NULL;
    
    if (aCommand == KUnavailableID)
        {
        temp =  CAknFepCtrlRawKeyButton::NewL(iLayoutOwner,
                                             aControlId,
                                             KAknsIIDQsnFrFunctionButtonNormal,
											 KAknsIIDQsnFrFunctionButtonPressed,
											 KAknsIIDQsnFrFunctionButtonInactive,  
                                             EEventRawKeyDownEvent,
                                             EEventRawKeyUpEvent); 
        }
    else
        {
        temp =  CAknFepCtrlRawKeyButton::NewL(iLayoutOwner,
                                             aControlId,
                                             KAknsIIDQsnFrFunctionButtonNormal,
											 KAknsIIDQsnFrFunctionButtonPressed,
											 KAknsIIDQsnFrFunctionButtonInactive,  
                                             EEventRawKeyDownEvent,
                                             EEventRawKeyUpEvent,
                                             aCommand); 
        }
                                         
    return temp;       
    }
    
// ---------------------------------------------------------------------------
// CSplitItutWindow::CreateCommonButtonL
// ---------------------------------------------------------------------------
//
CAknFepCtrlCommonButton* CSplitItutWindow::CreateCommonButtonL(const TInt aControlId)
    {
    return CAknFepCtrlCommonButton::NewL(iLayoutOwner, 
    									 aControlId,
    									 KAknsIIDQsnFrFunctionButtonNormal,
										 KAknsIIDQsnFrFunctionButtonPressed,
										 KAknsIIDQsnFrFunctionButtonInactive );
     
    }    
  
// ---------------------------------------------------------------------------
// CSplitItutWindow::CreateRepeatButtonL
// ---------------------------------------------------------------------------
//
CAknFepCtrlCommonButton* CSplitItutWindow::CreateRepeatButtonL(const TInt /*aControlId*/,
                                                               const TInt /*aCommand*/)
    {
    return NULL;
     
    }     

    
// ---------------------------------------------------------------------------
// CSplitItutWindow::CreateButtonL
// ---------------------------------------------------------------------------
//
CFepUiBaseCtrl* CSplitItutWindow::CreateButtonL(TButtonType aType,
                                                  const TInt aControlId,
                                                  const TInt aRectId,
                                                  const TInt aInnerRectId,
                                                  const TInt aResourceId,
                                                  const TInt aCommand)
    {
    CAknFepCtrlCommonButton* temp = NULL;
    
    switch(aType)
        {
        case ERawKeyButton:
            {
            temp = CreateRawKeyButtonL(aControlId, aCommand);    
            }
        break;
        case ECommonButton:
            {
            temp = CreateCommonButtonL(aControlId);    
            }
        break;
        default:
            {
            User::Leave(KErrArgument);    
            }                
        }

    TRect btnrect;
    TRect btnInnerRect;
    
    if (aRectId != KUnavailableID &&
        aInnerRectId != KUnavailableID)
        {
        btnrect = TItutDataConverter::AnyToRect(iDataMgr->RequestData(aRectId));
        btnInnerRect = TItutDataConverter::AnyToRect(iDataMgr->RequestData(aInnerRectId));
           
        temp->SetRect(btnrect);
        temp->SetForgroundBmpRect(btnInnerRect);
        }
        
    if (aResourceId != KUnavailableID)
        {
        temp->SetResourceId(aResourceId);
        temp->ConstructFromResourceL();            
        }
      
    if (aRectId != KUnavailableID &&
        aInnerRectId != KUnavailableID)
        {
        temp->SizeChanged(temp->Rect(), btnInnerRect, ETrue);
        }        

    AddControlL(temp);
    
    return temp;
    }

// ---------------------------------------------------------------------------
// CSplitItutWindow::CreateAllButtonL
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::CreateAllButtonL()
    {
    //create backgrand 
    CreateBackGroundControlL();
    
    //create backspace
    CreateButtonL(ERawKeyButton, ECtrlIdBackspace, EBackspaceRect, 
                  EBackspaceInnerRect,R_FINGER_RAWKEY_BACKSPACE);

    // create navi 
    CreateButtonL(ERawKeyButton, ECtrlIdArrowLeft, EArrowLeftRect, 
                  ELeftInnerRect,R_FINGER_RAWKEY_LEFTARROW);    
    
    CreateButtonL(ERawKeyButton, ECtrlIdArrowRight, EArrowRightRect, 
                  ERightInnerRect,R_FINGER_RAWKEY_RIGHTARROW); 

    /*
    CreateButtonL(ERepeatButtonEx, ECtrlIdArrowUp, EArrowUpRect, 
                  EUpInnerRect,R_PENINPUT_FINGER_CURSOR_UP); 

    CreateButtonL(ERepeatButtonEx, ECtrlIdArrowDown, EArrowDownRect, 
                  EDownInnerRect,R_PENINPUT_FINGER_CURSOR_DOWN);
    */
    //create option 
    CreateButtonL(ECommonButton, ECtrlIdOptions, EOptionsRect, 
                  EOptionInnerRect, R_PENINPUT_FINGER_OPTIONS);    
    
    //Crease close
    CreateButtonL(ECommonButton, ECtrlIdClose, ECloseRect, 
                  ECloseInnerRect, R_PENINPUT_FINGER_CLOSE);                   

    }

// ---------------------------------------------------------------------------
// CSplitItutWindow::CreateDropdownListL
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::CreateDropdownListL()
    {
	TRect outrect,innerrect;
	outrect = TItutDataConverter::AnyToRect(iDataMgr->RequestData(EPreviewBubbleRect));  
	innerrect = TItutDataConverter::AnyToRect(iDataMgr->RequestData(EPreviewBubbleInnerRect));
    TAknTextLineLayout bubbleTextLayout = 
        TItutDataConverter::AnyToTextLine(iDataMgr->RequestData(EPreviewBubbleTextlayout));
    CFont* bubbleFont = TItutDataConverter::AnyToFont(iDataMgr->RequestData(EPreviewBubbleFont));
    
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
    						            
    TDropdownListDrawInfo compositionDrawInfo(KAknsIIDQsnFrFunctionButtonInactive, 
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
    TInt unitWidth =  TItutDataConverter::AnyToInt(iDataMgr->RequestData(EDropdownlistUnitWidth));
    TInt unitHeight = TItutDataConverter::AnyToInt(iDataMgr->RequestData(EDropdownlistUnitHeight));
    TInt horizontalMargin =  
        TItutDataConverter::AnyToInt(iDataMgr->RequestData(EDropdownlistHorizontalMargin));
    TInt verticalMargin =  
        TItutDataConverter::AnyToInt(iDataMgr->RequestData(EDropdownlistVerticalMargin));

    CFont* listfont = TItutDataConverter::AnyToFont(iDataMgr->RequestData(EDropdownListFont));
    TInt textmargin = TItutDataConverter::AnyToInt(iDataMgr->RequestData(EDropdownListTextMargin));
    TRgb textcolor = TItutDataConverter::AnyToRgb(iDataMgr->RequestData(EDropdownListTextColor));

    TPoint spellListLTPos = 
        TItutDataConverter::AnyToPoint(iDataMgr->RequestData(EDropdownListSpellLTPos));
    // create spelling drop list
    iSpellCandsList = CFepCtrlDropdownList::NewL(iLayoutOwner,
                                                 ECtrlIdSpellCandsList,
                                                 R_AKN_FEP_PINYIN_SPELLING_DROP_DOWN_LIST,
                                                 spellListLTPos,
                                                 TSize(unitWidth, unitHeight),
                                                 5,
                                                 3);


    iSpellCandsList->SetCellMargin(horizontalMargin, verticalMargin);
    iSpellCandsList->SetFont(listfont);
    AddControlL(iSpellCandsList);
    iSpellCandsList->SetEventIdForCandidateSelected(EItutCmdCandidateSelected);
    iSpellCandsList->SetCandTruncateType(CFepCtrlDropdownList::ECandTruncateFromBeginning);
    iSpellCandsList->SetFuzzyBoundry(10);
    iSpellCandsList->Hide(ETrue);
	iSpellCandsList->SetDropdownListImgID(compositionDrawInfo);

    TPoint candListLTPos = 
        TItutDataConverter::AnyToPoint(iDataMgr->RequestData(EDropdownListCandsLTPos));
    // create candidate drop list
    iCandsList = CFepCtrlDropdownList::NewL(iLayoutOwner,
                                            ECtrlIdStdCandsList,
                                            R_AKN_FEP_NORMAL_CAND_DROP_DOWN_LIST,
                                            candListLTPos,
                                            TSize(unitWidth, unitHeight),
                                            5,
                                            3);


    iCandsList->SetCellMargin(horizontalMargin, verticalMargin);
    iCandsList->SetFont(listfont);
    AddControlL(iCandsList);
    iCandsList->SetEventIdForCandidateSelected(EItutCmdCandidateSelected);
    iCandsList->SetEventIdForNextPageCandidate(EItutCmdGetNextCandidatePage);
    iCandsList->SetEventIdForCandidateExisted(EItutCmdCandidateExisted);
    iCandsList->SetCandTruncateType(CFepCtrlDropdownList::ECandTruncateAsEllipsis);        
    iCandsList->SetFuzzyBoundry(10);
    iCandsList->Hide(ETrue);
	iCandsList->SetDropdownListImgID(candiDrawInfo);

    TPoint puncListLTPos = 
        TItutDataConverter::AnyToPoint(iDataMgr->RequestData(EDropdownListPuncLTPos));
    // create punctuation drop list
    iPuncCandsList = CFepCtrlDropdownList::NewL(iLayoutOwner,
                                                ECtrlIdPuncCandsList,
                                                R_AKN_FEP_PUNC_DROP_DOWN_LIST,
                                                puncListLTPos,
                                                TSize(unitWidth, unitHeight),
                                                5,
                                                1);
    iPuncCandsList->SetCellMargin(horizontalMargin, verticalMargin);
    iPuncCandsList->SetFont(listfont);
    AddControlL(iPuncCandsList);
    iPuncCandsList->SetEventIdForCandidateSelected(EItutCmdCandidateSelected);
	iPuncCandsList->SetDropdownListImgID(candiDrawInfo);
    iPuncCandsList->Hide(ETrue);
    }

// ---------------------------------------------------------------------------
// CSplitItutWindow::ConstructIcfFromResourceL
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::ConstructIcfFromResourceL()
    {
    if (!iICF->ResourceId())
        {
        return;
        }
    
    TResourceReader icfreader;
    CCoeEnv::Static()->CreateResourceReaderLC(icfreader, iICF->ResourceId());
    TInt32 colorMajorSkinId = icfreader.ReadInt32();
    TInt skinitemid = icfreader.ReadInt16();
    TInt coloridx = icfreader.ReadInt16();

    TAknsItemID colorid;
    colorid.Set(TInt(colorMajorSkinId), skinitemid);

    TRgb icftextcolor;
    MAknsSkinInstance* skininstance = AknsUtils::SkinInstance();
    TInt error = AknsUtils::GetCachedColor( skininstance, 
                                            icftextcolor, 
                                            colorid, 
                                            coloridx );

    if ( error != KErrNone )
        {
        icftextcolor = TRgb( KDefaultIcfTextColor );
        }

    iICF->SetTextColorL( icftextcolor );    
    
    // Set highlight color
    TRgb icfhightlightcolor;
    error = AknsUtils::GetCachedColor( skininstance, 
                                       icfhightlightcolor, 
                                       KAknsIIDQsnHighlightColors,
                                       EAknsCIQsnHighlightColorsCG2 ); 
    if ( error == KErrNone ) 
        {
        iICF->SetTextSelColorL( icfhightlightcolor );
        }

    skinitemid = icfreader.ReadInt16();
    coloridx = icfreader.ReadInt16();
    
    colorid.Set(TInt(colorMajorSkinId), skinitemid);
    
    TRgb icfframecolor;
    error = AknsUtils::GetCachedColor(skininstance, 
                                      icfframecolor, 
                                      colorid, 
                                      coloridx);

    if ( error != KErrNone )
        {
    	icfframecolor = TRgb(KDefaultIcfFrameColor);
        }

    iICF->SetBorderColor( icfframecolor );
    CleanupStack::PopAndDestroy( 1 ); // icfreader
    }
 
 
// ---------------------------------------------------------------------------
// CSplitItutWindow::CreateICFL
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::CreateICFL()
    {
    CFont* icffont = TItutDataConverter::AnyToFont(iDataMgr->RequestData(EIcfFont));
    TRect rect = TItutDataConverter::AnyToRect(iDataMgr->RequestData(ESpellICFRect));
    
    // Create ICF
    iICF = CFepLayoutMultiLineIcf::NewL( rect, 
                                         iLayoutOwner, 
                                         ECtrlIdICF, 
                                         iDataMgr->iIcfTextHeight,
                                         icffont->FontMaxHeight(),
                                         icffont );

    iICF->SetFocus(ETrue);
    iICF->SetResourceId( R_SPLIT_ITUT_ICF );
    ConstructIcfFromResourceL();
    iICF->SetBgImgSkinId( KAknsIIDQgnGrafFepInputBg);
    
    AddControlL( iICF );
    
    // Set indicator parameters
    TAknWindowLineLayout iBubbleSize = AknLayoutScalable_Apps::popup_char_count_window().LayoutLine();
    TAknTextLineLayout iBubbleTextLayout = AknLayoutScalable_Apps::popup_char_count_window_t1(0).LayoutLine();
    
    iICF->MsgBubbleCtrl()->SetTextFormat(iBubbleTextLayout);
    iICF->MsgBubbleCtrl()->SetTextColorIndex( EAknsCIQsnTextColorsCG67 );
    
    //Change the ID when ID into release
    iICF->MsgBubbleCtrl()->SetBitmapParam( NULL,
    									   NULL,
    									   KAknsIIDQsnFrInputPreviewSideL,
    									   KAknsIIDQsnFrInputPreviewMiddle,
    									   KAknsIIDQsnFrInputPreviewSideR );  
    iICF->SetMsgBubbleCtrlSize(TSize(iBubbleSize.iW,iBubbleSize.iH));
    
    iICF->MsgBubbleCtrl()->SetTextL( KEmptyString );
    
    iICF->SetTextMargin( iDataMgr->iIcfTextLeftMargin,
            			 iDataMgr->iIcfTextRightMargin,
            			 iDataMgr->iIcfTextTopMargin,
            			 iDataMgr->iIcfTextBottomMargin );
            					  
    iICF->SetLineSpace( iDataMgr->iIcfTextLineSpaceMargin );    					  
        	 
    SetIndiBubble();
    iICF->Hide( ETrue );
    }
 
    
// ---------------------------------------------------------------------------
// CSplitItutWindow::ConstructL
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::ConstructL()
    {
    iBmpRotator = CPeninputSyncBitmapRotator::NewL();
    CreateAllButtonL();
    CreateItutKeypadL();
    CreateICFL();
    }

// ---------------------------------------------------------------------------
// CSplitItutWindow::AddControlL
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::AddControlL(CFepUiBaseCtrl* aControl)
    {
    iLayoutOwner->AddControlL(aControl);   
    }

// ---------------------------------------------------------------------------
// CSplitItutWindow::CreateChineseSpecificCtrlsIfNeededL
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::CreateChineseSpecificCtrlsIfNeededL()
    {
    if (iChnCtrlInited)
        {
        return;
        }

    CreateDropdownListL();
    // create spell control for stroke/zhuyin
    TRect rect = TItutDataConverter::AnyToRect(iDataMgr->RequestData(ESpellRect));
    iSpell = CFepInputContextField::NewL(rect, iLayoutOwner, ECtrlIdSpellICF);

    iSpell->SetResourceId(R_FINGER_SPELL);
    ConstructSpellCtrlFromResourceL();

    iSpell->SetFont(TItutDataConverter::AnyToFont(iDataMgr->RequestData(ESpellFont)));
    AddControlL(iSpell); 

    iSpell->Hide(ETrue);
    
  
    iChnCtrlInited = ETrue;
    }

// ---------------------------------------------------------------------------
// CSplitItutWindow::ConstructSpellCtrlFromResourceL
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::ConstructSpellCtrlFromResourceL()
    {
    if (!iSpell || !iSpell->ResourceId())
        {
        return;
        }

    TResourceReader spellreader;
    CCoeEnv::Static()->CreateResourceReaderLC(spellreader, iSpell->ResourceId());

    TInt bgimgResId = spellreader.ReadInt32();
    TResourceReader bgreader;
    CCoeEnv::Static()->CreateResourceReaderLC(bgreader, bgimgResId);

    TInt bmpid = bgreader.ReadInt16();
    TInt maskbmpid = bgreader.ReadInt16();
    TInt bmpskinid = bgreader.ReadInt16();

    CFbsBitmap* bmp = NULL;
    CFbsBitmap* maskbmp = NULL;
    
    CreateLayoutIconL(KImageMajorSkinId,
                      bmpskinid,
                      KBmpFileName,
                      bmpid,
                      maskbmpid,
                      bmp,
                      maskbmp,
                      iSpell->Rect().Size());

    if (bmp)
        {
        iSpell->SetBackgroundBitmapL(bmp);
        }
        
    delete maskbmp;

    CleanupStack::PopAndDestroy( 1 ); // bgreader

    TInt colorMajorSkinId = spellreader.ReadInt32();
    TInt skinitemid= spellreader.ReadInt16();
    TInt coloridx = spellreader.ReadInt16();

    TAknsItemID colorid;
    colorid.Set(TInt(colorMajorSkinId), skinitemid);

    TRgb spelltextcolor;
    MAknsSkinInstance* skininstance = AknsUtils::SkinInstance();
    TInt error = AknsUtils::GetCachedColor(skininstance, 
                                           spelltextcolor, 
                                           colorid, 
                                           coloridx);

    if (error != KErrNone)
        {
        spelltextcolor = TRgb(KDefaultSpellTextColor);
        }

    iSpell->SetTextColor(spelltextcolor);

    CleanupStack::PopAndDestroy( 1 ); // spellreader
    }


// ---------------------------------------------------------------------------
// CSplitItutWindow::PenInputType
// ---------------------------------------------------------------------------
//
TInt CSplitItutWindow::PenInputType()
    {
    return EPluginInputModeItut;
    }

// ---------------------------------------------------------------------------
// CSplitItutWindow::SetCtrlRect
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::SetCtrlRect(CFepUiBaseCtrl* aCtrl, TInt aRectIdx, const TBool aOffset )
    {
    TRect rect = TItutDataConverter::AnyToRect(iDataMgr->RequestData(aRectIdx));
    if(aOffset)
        {
        TPoint offset = TItutDataConverter::AnyToPoint(iDataMgr->RequestData(ELayoutOffset));
        rect.Move(offset);
        }
    aCtrl->SetRect(rect);
    }

// ---------------------------------------------------------------------------
// CSplitItutWindow::SizeChanged
// ---------------------------------------------------------------------------
//
TInt CSplitItutWindow::SizeChanged()
    {
    // resize all controls
    SetCtrlRect(iStandardItutKp, EKeypadRect);

    iStandardItutKp->SetTextLineLayout(
        TItutDataConverter::AnyToTextLine(iDataMgr->RequestData(EKeypadLeftTextLine)),
        EPosLeft);
    iStandardItutKp->SetTextLineLayout(
        TItutDataConverter::AnyToTextLine(iDataMgr->RequestData(EKeypadRightTextLine1)),
        EPosRight1);
    iStandardItutKp->SetTextLineLayout(
        TItutDataConverter::AnyToTextLine(iDataMgr->RequestData(EKeypadRightTextLine2)),
        EPosRight2);
    iStandardItutKp->SetTextLineLayout(
        TItutDataConverter::AnyToTextLine(iDataMgr->RequestData(EKeypadRightTextLine3)),
        EPosRight3);
    
    RPointerArray<CVirtualKey>& keys = 
        const_cast<RPointerArray<CVirtualKey>&>(iStandardItutKp->KeyArray());
    RArray<TRect>& cellRects = 
        TItutDataConverter::AnyToRectArray(iDataMgr->RequestData(EKeypadCellRects));

    for (TInt i = 0 ; i < keys.Count(); i++)
        {
        keys[i]->SetRect(cellRects[i]);
        TRect innerrect = cellRects[i];
        innerrect.Shrink(TSize(4, 4));
        keys[i]->SetInnerRect(innerrect);
        }
	// set key background image size
	TSize curSize = cellRects[0].Size();
	if( iStandardItutKp->NonIrregularKeyBitmap(EKeyBmpNormal) )
		{
		TSize size = iStandardItutKp->NonIrregularKeyBitmap(EKeyBmpNormal)->SizeInPixels();
		if( curSize != size )	
			{
	        for ( TInt i = 0; i <= EKeyBmpLastType; i++ )
	            {
	            if( iStandardItutKp->NonIrregularKeyBitmap((TVirtualKeyBmpType)i) )
	            	{
	                AknIconUtils::SetSize( 
                                   iStandardItutKp->NonIrregularKeyBitmap((TVirtualKeyBmpType)i), 
                                   curSize, EAspectRatioNotPreserved );                    		
	            	}
	            }	
			}
		}

    // should set textline when it is defined in laf
	iBackgroundCtrl->SizeChanged( 
                         TItutDataConverter::AnyToRect(iDataMgr->RequestData( EBackgroundRect ) ) );								
	
    TRAP_IGNORE(ApplyVariantLafDataL(ETrue); 
                /*iLayoutOwner->HandleCommand(EItutExtCmdSizeChanged, NULL)*/);
    
    
    if (iChnCtrlInited)
        {
        SetCtrlRect(iSpell, ESpellRect);
        iSpell->SetFont(TItutDataConverter::AnyToFont(iDataMgr->RequestData(ESpellFont)));

        ResizeCandidateList(iSpellCandsList, EDropdownListSpellLTPos, ETrue);
        ResizeCandidateList(iCandsList, EDropdownListCandsLTPos, ETrue);
        ResizeCandidateList(iPuncCandsList, EDropdownListPuncLTPos, EFalse);
        }

    return KErrNone;    
    }

// ---------------------------------------------------------------------------
// CSplitItutWindow::ControlSizeChanged
// ---------------------------------------------------------------------------
//
TBool CSplitItutWindow::ControlSizeChanged(const TInt aControlId, 
                                            const TInt aRect, 
                                            const TInt aInnerRect, 
                                            TBool aIsReloadImages)
    {
    CAknFepCtrlCommonButton* temp = CommonButtonControl(aControlId);
    if( !temp )
    	return EFalse;
    

    TRect rect;
    TRect innerRect;
    
    rect = TItutDataConverter::AnyToRect(iDataMgr->RequestData(aRect));
	innerRect = TItutDataConverter::AnyToRect(iDataMgr->RequestData(aInnerRect));
    
    TBool bSizedChanged = (rect.Size() != temp->Rect().Size()) && 
    	    (innerRect.Size() != temp->ForgroundBmpRect().Size());
    temp->SizeChanged(rect, innerRect, aIsReloadImages);   
      
    return bSizedChanged;
    }

// ---------------------------------------------------------------------------
// CSplitItutWindow::ApplyVariantLafDataL
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::ApplyVariantLafDataL(TBool /*aResolutionChange*/)
    {
    if( iDataMgr->IsChinese() )
        {
 //       ControlSizeChanged(ECtrlIdArrowUp, EArrowUpRect, EUpInnerRect, ETrue);
 //       ControlSizeChanged(ECtrlIdArrowDown, EArrowDownRect, EDownInnerRect, ETrue);
        }
    
    TBool bSizeChanged = EFalse;       
    ControlSizeChanged(ECtrlIdArrowLeft, EArrowLeftRect, ELeftInnerRect, ETrue);
    ControlSizeChanged(ECtrlIdArrowRight, EArrowRightRect, ERightInnerRect, ETrue);
    bSizeChanged = ControlSizeChanged(ECtrlIdBackspace, EBackspaceRect, EBackspaceInnerRect, ETrue);
    ControlSizeChanged(ECtrlIdOptions, EOptionsRect, EOptionInnerRect, ETrue);
    ControlSizeChanged(ECtrlIdClose, ECloseRect, ECloseInnerRect, ETrue);
   						  	
    // resize all controls
    SetCtrlRect(iStandardItutKp, EKeypadRect); 
    
    // Handle control res when language direction changing here.
    if( iDataMgr->IsLangDirectionSwitch() || 
    	( bSizeChanged && iDataMgr->IsRtoLLanguage() ) )
    	{
    	HandleButtonResOnLangDirChange( ECtrlIdBackspace );
    	iDataMgr->SetLangDirectionSwitch( EFalse );	
    	}   
    }
 
// ---------------------------------------------------------------------------
// CSplitItutWindow::ApplyVariantLafDataForSpellL
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::ApplyVariantLafDataForSpellL()
    {
    TBool bSizeChanged = EFalse;       
    ControlSizeChanged( ECtrlIdArrowLeft, EArrowLeftRect, ELeftInnerRect, ETrue );
    ControlSizeChanged( ECtrlIdArrowRight, EArrowRightRect, ERightInnerRect, ETrue );
    
    bSizeChanged = ControlSizeChanged( ECtrlIdBackspace, ESpellBackSpcae, 
    		                           ESpellBackSpcaeInner, ETrue );
   						  	
    // resize all controls
    SetCtrlRect( iBackgroundCtrl, EBackgroundRect );
    SetCtrlRect( iStandardItutKp, EKeypadRect, ETrue ); 
    
    // Handle control res when language direction changing here.
    if ( iDataMgr->IsLangDirectionSwitch() || 
    	( bSizeChanged && iDataMgr->IsRtoLLanguage()))
    	{
    	HandleButtonResOnLangDirChange( ECtrlIdBackspace );
    	iDataMgr->SetLangDirectionSwitch( EFalse );	
    	}
    
    CFont* icffont = TItutDataConverter::AnyToFont(iDataMgr->RequestData( EIcfFont ));
    iICF->Hide( ETrue ); 
    iICF->SizeChangedL( TItutDataConverter::AnyToRect( iDataMgr->RequestData( ESpellICFRect )), 
       	                iDataMgr->iIcfTextHeight,
    	                icffont->FontMaxHeight(),
    	                icffont );
    iICF->Hide( EFalse );
    }
    

// ---------------------------------------------------------------------------
// CSplitItutWindow::ResizeCandidateList
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::ResizeCandidateList(CFepCtrlDropdownList* aList, 
                                             TInt aLTPosIdx, TBool aExpandable)
    {
    if (!aList)
        {
        return;   
        }
    
    TInt unitWidth = TItutDataConverter::AnyToInt(
                        iDataMgr->RequestData(EDropdownlistUnitWidth));
    TInt unitHeight = TItutDataConverter::AnyToInt(
                        iDataMgr->RequestData(EDropdownlistUnitHeight));
    TInt horizontalMargin = 
        TItutDataConverter::AnyToInt(iDataMgr->RequestData(EDropdownlistHorizontalMargin));
    TInt verticalMargin = 
        TItutDataConverter::AnyToInt(iDataMgr->RequestData(EDropdownlistVerticalMargin));
    TInt naviWidth = TItutDataConverter::AnyToInt(
                        iDataMgr->RequestData(EDropdownlistUnitWidth));
    CFont* listfont = TItutDataConverter::AnyToFont(
                        iDataMgr->RequestData(EDropdownListFont));
    TPoint listLTPos = TItutDataConverter::AnyToPoint(iDataMgr->RequestData(aLTPosIdx));
    
    aList->SizeChanged(unitWidth, 
                       unitHeight,
                       naviWidth,
                       listLTPos);

    aList->SetCellMargin(horizontalMargin, verticalMargin);
    
    if (aExpandable)
        {
        TInt naviSpinBtnHeight = 
            TItutDataConverter::AnyToInt(iDataMgr->RequestData(EDropdownListSpinBtnHeight));
        
        aList->SetSpinBtnHeight(naviSpinBtnHeight);
        }
    }

// ---------------------------------------------------------------------------
// CSplitItutWindow::OnSkinChange
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::OnSkinChange()
    {
    // when skin change all control with skin image should be reload.
    TRAP_IGNORE(ConstructKeyImageFromResourceL( R_ITUT_KEYPAD_BITMAP ));    
    
    // reconstruct shift icon when skin changed
    TRAP_IGNORE(iStandardItutKp->ShiftIcon()->ReConstructL());
    TRAP_IGNORE(iStandardItutKp->ShiftIcon()->ResizeL(iDataMgr->iShiftIconRect.Size()));
    
    // reconstruct star icon when skin changed
    TRAP_IGNORE(iStandardItutKp->StarIcon()->ReConstructL());
    TRAP_IGNORE(iStandardItutKp->StarIcon()->ResizeL(iDataMgr->iStarIconRect.Size()));
    
    //Skin change will case the reconstuction of the button graphic.
    //When language is RToL, it should be mirrored again.
    if( iDataMgr->IsRtoLLanguage() )
    	{
    	HandleButtonResOnLangDirChange( ECtrlIdBackspace );	
    	}
    }


// ---------------------------------------------------------------------------
// CSplitItutWindow::CalculateFrameRects
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::CalculateFrameRects(const TRect aRect,
											 TRect& aOuterRect, 
											 TRect& aInnerRect )
    {
    TRect windowRect = aRect;
    
    TAknLayoutRect topLeft;
    topLeft.LayoutRect(windowRect, SkinLayout::Submenu_skin_placing_Line_2());

    TAknLayoutRect bottomRight;
    bottomRight.LayoutRect(windowRect, SkinLayout::Submenu_skin_placing_Line_5());

    aOuterRect = TRect(topLeft.Rect().iTl, bottomRight.Rect().iBr);
    aInnerRect = TRect(topLeft.Rect().iBr, bottomRight.Rect().iTl);
    }
    

// ---------------------------------------------------------------------------
// CSplitItutWindow::CommonButtonControl
// ---------------------------------------------------------------------------
//
CAknFepCtrlCommonButton* CSplitItutWindow::CommonButtonControl(const TInt aControlId)
    {
    CFepUiBaseCtrl* ctrl = NULL;
    ctrl = Control(aControlId);
    
    if (!ctrl)
        {
        return NULL;   
        }
        
    if (!ctrl->IsKindOfControl(ECtrlButton))
    
        {
        return NULL;   
        }
        
    return static_cast<CAknFepCtrlCommonButton*>(ctrl);
    }


// ---------------------------------------------------------------------------
// CSplitItutWindow::Control
// ---------------------------------------------------------------------------
//
CFepUiBaseCtrl* CSplitItutWindow::Control(const TInt aCtrlId)
    {
    CFepUiBaseCtrl* ctrl = NULL;
    RPointerArray<CFepUiBaseCtrl> todo;
    TInt current = 0;
    TBool found = EFalse;
    
    todo.Append(iLayoutOwner->RootControl());
    CControlGroup* temp;
    
    while (current < todo.Count() && !found)
        {
        ctrl = todo[current];
        
        if(ctrl->ControlId() == aCtrlId) //Find control
            {
            found = ETrue;     
            }
        else
            {
            if(ctrl->IsKindOfControl(ECtrlControlGroup))
                {
                temp = static_cast<CControlGroup*>(ctrl);
                for (int i = 0; i < temp->NumOfControls(); i++)
                    {
                    todo.Append(temp->At(i));
                    }
                }
                
            current++;
            }
        }
        
    todo.Close();
    
    return found ? ctrl : NULL;
    }      

// ---------------------------------------------------------------------------
// CSplitItutWindow::IsChineseMode
// ---------------------------------------------------------------------------
//
TBool CSplitItutWindow::IsChineseMode()
    {
    TInt immode = iDataMgr->InputMode();
    TBool chineseMode = EFalse;
    
    if (immode == EPinyin || immode == EStroke || immode == EZhuyin)
        {
        chineseMode = ETrue;
        }
    
    return chineseMode;    
    }

// ---------------------------------------------------------------------------
// CSplitItutWindow::ConstructKeyImageFromResourceL
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::ConstructKeyImageFromResourceL( TInt aKeyImageResID )
	{
    TResourceReader reader;    
    CCoeEnv::Static()->CreateResourceReaderLC( reader, aKeyImageResID );      
    
    TPtrC bmpFileName = reader.ReadTPtrC();
    TInt32 imgMajorSkinId = reader.ReadInt32();
    TAknsItemID id;
    
    RArray<TRect>& cellRects = 
                     TItutDataConverter::AnyToRectArray(iDataMgr->RequestData(EKeypadCellRects));
    TSize keySize = cellRects[0].Size();
    for (TInt index = 0; index <= EKeyBmpLastType ; index += 2)
        { 
        // Get the image ids and mask ids from resource
        TInt bmpId = reader.ReadInt16(); 
        TInt bmpMskId = reader.ReadInt16();
        
        // read skin item id
        const TInt skinitemid = reader.ReadInt16();
        id.Set(TInt(imgMajorSkinId), skinitemid);
        
        if (bmpId != KInvalidBmp)
            {
        	CFbsBitmap* bmp = NULL;
        	CFbsBitmap* maskbmp = NULL;

        	if (bmpMskId != KInvalidBmp)
        		{
        		AknsUtils::CreateIconL(AknsUtils::SkinInstance(),
        		                       id,
        		                       bmp,
        		                       maskbmp,
        		                       bmpFileName,
        		                       bmpId,
        		                       bmpMskId);
        		
       		    // set maskbmp and size
       		    AknIconUtils::SetSize( maskbmp, keySize, EAspectRatioNotPreserved );
                iStandardItutKp->SetNonIrregularKeyBitmapL(
                TVirtualKeyBmpType(EKeyBmpNormal + index + 1), maskbmp );
                }
            else
                {
        	    AknsUtils::CreateIconL(AknsUtils::SkinInstance(),
        	                           id,
        	                           bmp,
        	                           bmpFileName,
        	                           bmpId);
                }
            // set bmp and size
            AknIconUtils::SetSize( bmp, keySize, EAspectRatioNotPreserved );
           	iStandardItutKp->SetNonIrregularKeyBitmapL( 
                                                 TVirtualKeyBmpType(EKeyBmpNormal + index), bmp );
            }       
        }
    // Pop and destroy reader
    CleanupStack::PopAndDestroy( 1 );		
	}
	
// ---------------------------------------------------------------------------
// CSplitItutWindow::HandleButtonResOnLangDirChange
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::HandleButtonResOnLangDirChange( TInt aControlId )
	{
    TRAP_IGNORE(HandleButtonResOnLangDirChangeL(aControlId));
    }
    
// ---------------------------------------------------------------------------
// CSplitItutWindow::HandleButtonResOnLangDirChangeL
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::HandleButtonResOnLangDirChangeL( TInt aControlId )
	{
	CAknFepCtrlCommonButton* ctrl = CommonButtonControl(aControlId);
	if( ctrl && iBmpRotator)
		{
		TSyncRotationAngle rotChoice = CBitmapRotator::EMirrorVerticalAxis; 
		User::LeaveIfError( iBmpRotator->Rotate( *ctrl->ForgroundBmp() ,
								  				 rotChoice ) );
		User::LeaveIfError( iBmpRotator->Rotate( *ctrl->ForgroundBmpMask() ,
								  				 rotChoice ) );   								  
		}		
	}

// ---------------------------------------------------------------------------
// CSplitItutWindow::SetHardKeyOneSymbol
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::SetHardKeyOneSymbol(TUint8* aData)
    {
    TRAP_IGNORE(SetHardKeyOneSymbolL(aData));
    }
    
// ---------------------------------------------------------------------------
// CSplitItutWindow::SetHardKeyOneSymbolL
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::SetHardKeyOneSymbolL(TUint8* aData)
    {
    //Read stream
    RDesReadStream readStream;

    TPtr8 countPtr( aData, 3*sizeof(TInt), 3*sizeof(TInt) );            
    readStream.Open(countPtr);
    CleanupClosePushL(readStream);

    iSymbolData.iSymbol1 = readStream.ReadInt32L();
    iSymbolData.iSymbol2 = readStream.ReadInt32L();
    iSymbolData.iSymbol3 = readStream.ReadInt32L();
    
    CleanupStack::PopAndDestroy(&readStream);  
    }

// ---------------------------------------------------------------------------
// CSplitItutWindow::SetUnicodesForHardKey1
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::SetUnicodesForHardKey1(CVirtualKey* aKey, const TDesC& aMapData)
    {
    TRAP_IGNORE(SetUnicodesForHardKey1L(aKey, aMapData));
    }
    
// ---------------------------------------------------------------------------
// CSplitItutWindow::SetUnicodesForHardKey1L
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::SetUnicodesForHardKey1L(CVirtualKey* aKey, const TDesC& aMapData)
    {
    RPointerArray<HBufC> unicodesArr1;
    RArray<TInt> unicodesInt;
    
    unicodesInt.Append(iSymbolData.iSymbol1);
    unicodesInt.Append(iSymbolData.iSymbol2);
    unicodesInt.Append(iSymbolData.iSymbol3);
    
    TInt curMode = TItutDataConverter::AnyToInt(
            iLayoutOwner->DataMgr()->RequestData(EInputMode));
    
    TInt curLang = TItutDataConverter::AnyToInt(
            iLayoutOwner->DataMgr()->RequestData(ELanguage));    
    
    TInt latinOnly = TItutDataConverter::AnyToInt(
            iLayoutOwner->DataMgr()->RequestData(ELatinOnly));        
    
    HBufC* symbols = HBufC::NewL( 20 );
    for (TInt i = 0; i <= 2; i++)
        {
        symbols->Des().Append( unicodesInt[i] );
        if (EHindi == curMode ||
                (ELatin == curMode && curLang == ELangThai && !latinOnly))
            {
            continue;
            }
        else
            {
            symbols->Des().Append( KSpaceUnicode );
            }
        }
    
    HBufC* number = HBufC::NewL( 1 );
    number->Des().Append( aMapData );
    unicodesArr1.AppendL(number);
    unicodesArr1.AppendL(symbols);
    
    aKey->SetUnicodesL(unicodesArr1);

    unicodesArr1.Close();
    unicodesInt.Close();

    }

void CSplitItutWindow::UpdateIndiBubbleL( TUint8* aData )
    {  
    RDesReadStream readStream;
    TFepIndicatorInfo indicatorData;

    TPtr8 countPtr( aData, 4*sizeof(TInt), 4*sizeof(TInt) );            
    readStream.Open(countPtr);
    CleanupClosePushL(readStream);

    indicatorData.iIndicatorImgID = readStream.ReadInt32L();
    indicatorData.iIndicatorMaskID = readStream.ReadInt32L();
    indicatorData.iIndicatorTextImgID = readStream.ReadInt32L();
    indicatorData.iIndicatorTextMaskID = readStream.ReadInt32L();

    CleanupStack::PopAndDestroy(&readStream);

    if ( indicatorData.iIndicatorImgID != 0 && 
         indicatorData.iIndicatorMaskID != 0 && 
         indicatorData.iIndicatorTextImgID != 0 &&
         indicatorData.iIndicatorTextMaskID != 0)
        {
        iDataMgr->SetIndicatorData( indicatorData );
        iImDimensionSet = ETrue;

        SetIndiBubbleImageL( indicatorData.iIndicatorImgID,
                             indicatorData.iIndicatorMaskID,
                             indicatorData.iIndicatorTextImgID,
                             indicatorData.iIndicatorTextMaskID );
            
		TBuf<100> text;	
        iICF->MsgBubbleCtrl()->GetText( text );
        iICF->ShowBubble( text, iICF->MsgBubbleCtrl()->Rect());
        }
    }

// ---------------------------------------------------------------------------
// CSplitItutWindow::CalIndicatorRect
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::CalIndicatorRect(const TRect& aBoundRect,
                                          TRect& aRealRect1,
                                          TRect& aRealRect2,
                                          TIndicatorAlign aAlign) 
	{
	if (!iImDimensionSet)
		{
		return;
		}
	
	TInt imgAspectText = iIndicatorTextSize.iWidth / iIndicatorTextSize.iHeight;
	TInt imgAspectIndi = iIndicatorSize.iWidth / iIndicatorSize.iHeight;
    TSize imgSizeText( aBoundRect.Size().iHeight * imgAspectText, 
    			  	   aBoundRect.Size().iHeight );
	TSize imgSizeIndi( aBoundRect.Size().iHeight * imgAspectIndi, 
    			   			   aBoundRect.Size().iHeight );
	// check if the length of img > bound rect width
	TInt nTotalWidth = imgSizeText.iWidth + imgSizeIndi.iWidth;
	if( nTotalWidth > aBoundRect.Size().iWidth )
		{
		TReal nAspect = (TReal)imgSizeText.iWidth / nTotalWidth;
		imgSizeText.iWidth = aBoundRect.Size().iWidth * nAspect;
		imgSizeIndi.iWidth = aBoundRect.Size().iWidth - imgSizeText.iWidth;
		imgSizeText.iHeight = imgSizeText.iWidth / imgAspectText;
		// make sure the height of two rect is equal
		imgSizeIndi.iHeight = imgSizeText.iHeight;
		}
	if( aAlign == EIndiAlignRight )
		{
    	aRealRect2 = TRect(TPoint( aBoundRect.iBr.iX - imgSizeText.iWidth, aBoundRect.iTl.iY),
    			   	   imgSizeText);
		aRealRect1 = TRect(TPoint(aRealRect2.iTl.iX - imgSizeIndi.iWidth, aRealRect2.iTl.iY),
				       imgSizeIndi);
		}
	else if( aAlign == EIndiAlignCenter )
		{
		TInt offsetX = ( aBoundRect.Size().iWidth - imgSizeText.iWidth - imgSizeIndi.iWidth ) / 2;
		TInt offsetY = ( aBoundRect.Size().iHeight - imgSizeText.iHeight ) / 2;
    	aRealRect2 = TRect( TPoint( aBoundRect.iBr.iX - imgSizeText.iWidth - offsetX, 
    							   aBoundRect.iTl.iY + offsetY),
    			   	   	    imgSizeText );
		aRealRect1 = TRect( TPoint(aRealRect2.iTl.iX - imgSizeIndi.iWidth, aRealRect2.iTl.iY),
				       imgSizeIndi );
		}
	else if( aAlign == EIndiAlignLeft )
		{
		aRealRect1 = TRect( aBoundRect.iTl, imgSizeIndi );
		aRealRect2 = TRect( TPoint( aRealRect1.iBr.iX, aRealRect1.iTl.iY ), imgSizeText );
		}
	}

// ---------------------------------------------------------------------------
// CSplitItutWindow::SetPromptTextL
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::SetPromptTextL( TUint8* aData )
    {
    RDesReadStream readStream;

    TPtr8 countPtr( aData, 2*sizeof(TInt), 2*sizeof(TInt));
	readStream.Open(countPtr);
	CleanupClosePushL(readStream);
    const TInt dataCount = readStream.ReadInt32L();
	const TInt textCount = readStream.ReadInt32L();
    CleanupStack::PopAndDestroy(&readStream);
    
    TPtr8 ptr( aData+2*sizeof(TInt), dataCount+textCount, dataCount+textCount );            
	readStream.Open(ptr);
	CleanupClosePushL(readStream);
	
	HBufC8* dataBuf = HBufC8::NewLC(dataCount);
	TPtr8 dataBufPtr = dataBuf->Des();
	readStream.ReadL(dataBufPtr, dataCount);

    TFepPromptText* pIcfData = 
    		reinterpret_cast<TFepPromptText*>(const_cast<TUint8*>(dataBufPtr.Ptr()));

    HBufC* textBuf;
    if ( textCount > 0 )
        {
        textBuf = HBufC::NewLC( textCount/2 );
    	TPtr textBufPtr = textBuf->Des();
    	readStream.ReadL( textBufPtr, textCount/2 );
         
        const HBufC* icfPromptText = iICF->PromptText();
        
        if (!icfPromptText || icfPromptText->Compare( textBuf->Des()) != 0 )
        	{
        	iICF->SetPromptTextL( textBuf->Des(), pIcfData->iCleanContent );	
        	}
        CleanupStack::PopAndDestroy( textBuf );
        }
    else
        {
        iICF->SetPromptTextL( KNullDesC, pIcfData->iCleanContent );
        }

      CleanupStack::PopAndDestroy( dataBuf );
	  CleanupStack::PopAndDestroy( &readStream );
    }

// ---------------------------------------------------------------------------
// CSplitItutWindow::SetIndiBubbleImageL
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::SetIndiBubbleImageL( const TInt aImgID1,
                                            const TInt aMaskID1,
                                            const TInt aImgID2,
                                            const TInt aMaskID2 )
    {
    MAknsSkinInstance* skininstance = AknsUtils::SkinInstance();

    CFbsBitmap* bmp1 = NULL;
    CFbsBitmap* mask1 = NULL;
    
    TInt colorIndex = EAknsCIQsnIconColorsCG30;

    AknsUtils::CreateColorIconL( skininstance,
                                 KAknsIIDQsnIconColors,
                                 KAknsIIDQsnIconColors,
                                 colorIndex,
                                 bmp1,
                                 mask1,
                                 AknIconUtils::AvkonIconFileName(),
                                 aImgID1,
                                 aMaskID1,
                                 KRgbGray );
    CleanupStack::PushL( bmp1 );
    CleanupStack::PushL( mask1 );
                                
    AknIconUtils::GetContentDimensions( bmp1, iIndicatorSize );

    CFbsBitmap* bmp2 = NULL;
    CFbsBitmap* mask2 = NULL;
    AknsUtils::CreateColorIconL( skininstance,
                                 KAknsIIDQsnIconColors,
                                 KAknsIIDQsnIconColors,
                                 colorIndex,
                                 bmp2,
                                 mask2,
                                 AknIconUtils::AvkonIconFileName(),
                                 aImgID2,
                                 aMaskID2,
                                 KRgbGray );
 
    CleanupStack::PushL( bmp2 );
    CleanupStack::PushL( mask2 );
    
    AknIconUtils::GetContentDimensions( bmp2, iIndicatorTextSize );
    
    TRect boundRect;
    boundRect = TItutDataConverter::AnyToRect(
                iDataMgr->RequestData( EIndiIconWithoutTextRect ));
    
    TRect imgrect, textrect;
    
    CalIndicatorRect( boundRect, imgrect, textrect, EIndiAlignCenter );
    AknIconUtils::SetSize( bmp1, imgrect.Size(), EAspectRatioNotPreserved );
    AknIconUtils::SetSize( mask1, imgrect.Size(), EAspectRatioNotPreserved );
    AknIconUtils::SetSize( bmp2, textrect.Size(), EAspectRatioNotPreserved) ;
    AknIconUtils::SetSize( mask2, textrect.Size(), EAspectRatioNotPreserved );

    CFbsBitmap* bmp3 = AknPenImageUtils::CombineTwoImagesL(bmp1, bmp2, EColor256);
    CFbsBitmap* mask3 = AknPenImageUtils::CombineTwoImagesL(mask1, mask2, EGray256);
    
    iICF->MsgBubbleCtrl()->SetBitmapParam( bmp3, mask3, 
                    KAknsIIDQsnFrInputPreviewSideL,
                    KAknsIIDQsnFrInputPreviewMiddle,
                    KAknsIIDQsnFrInputPreviewSideR );
    
    CleanupStack::PopAndDestroy( mask2 );
    CleanupStack::PopAndDestroy( bmp2 );
    CleanupStack::PopAndDestroy( mask1 );
    CleanupStack::PopAndDestroy( bmp1 );
    }

// ---------------------------------------------------------------------------
// CSplitItutWindow::SetIndiBubble
// ---------------------------------------------------------------------------
//
void CSplitItutWindow::SetIndiBubble()
    {
    if ( iICF )
        {
        TRect bubbleRect = TItutDataConverter::AnyToRect( 
                iDataMgr->RequestData( EIndiPaneWithoutTextRect ));
        TRect iconRect = TItutDataConverter::AnyToRect( 
                iDataMgr->RequestData( EIndiIconWithoutTextRect ));
        
        TSize offset;
        offset.iHeight = iconRect.iTl.iY - bubbleRect.iTl.iY;
        offset.iWidth = iconRect.iTl.iX - bubbleRect.iTl.iX;
        TSize size( iconRect.Width(), iconRect.Height());
        
        iICF->MsgBubbleCtrl()->SetRect( bubbleRect );
        iICF->MsgBubbleCtrl()->SetIconOffsetAndSize( offset, size );
        }
    }

// End Of File
