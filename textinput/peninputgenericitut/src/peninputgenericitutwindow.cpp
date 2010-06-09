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


#include <aknlayoutscalable_apps.cdl.h>
#include <peninputgeneralitutnew.rsg>
#include <peninputitutconfig_chinese.rsg>
#include <skinlayout.cdl.h>
#include <AknsDrawUtils.h>
#include <AknsUtils.h>
#include <AknUtils.h> 
#include <peninputrawkeybutton.h>
#include <peninputrepeatbutton.h>
#include <peninputcommonbutton.h>
#include "peninputrepeatbuttonex.h"
#include <AknFepGlobalEnums.h>
#include <aknfeppeninputenums.h>
#include <peninputdropdownlist.h>
#include <peninputlayoutchoicelist.h>
#include <peninputscrollablelist.h>
#include <s32mem.h>
#include <peninputlabel.h>
#include <peninputlayoutinputmodechoice.h>
#include <peninputlayoutbubblectrl.h>

#include "peninputgenericitutwindow.h"
#include "peninputgenericitutdatamgr.h"
#include "peninputgenericitutuimgrbase.h"
#include "peninputgenericitutuistatebase.h"
#include "peninputitutchnuimgr.h"
#include "peninputitutwesternuimgr.h"
#include "peninputgenericitutconverter.h"
#include "peninputcommonbgctrl.h"

_LIT(KBmpFileName, "z:\\resource\\apps\\peninputgenericitut.mbm");
_LIT(KEmptyString, "");

const TInt KImageMajorSkinId = EAknsMajorGeneric;
const TUint KDefaultSpellTextColor = 0;
const TUint KDefaultIcfTextColor = 0x000000;
const TUint KDefaultIcfFrameColor = 0x000000;

const TInt KSpaceUnicode = 32;
const TInt KButtonOneUnicode = 49;

CGenericItutWindow* CGenericItutWindow::NewL(CGenericItutWindowManager* aWindowMgr,
                                             CGenericItutUiLayout* aLayoutOwner,
                                             CGenericItutDataMgr* aDataMgr)
    {
    CGenericItutWindow* window = new (ELeave) CGenericItutWindow(aWindowMgr,aLayoutOwner,aDataMgr);

    CleanupStack::PushL(window);
    window->ConstructL();
    CleanupStack::Pop(window);

    return window;
    }

CGenericItutWindow::CGenericItutWindow(CGenericItutWindowManager* aWindowMgr, 
                                       CGenericItutUiLayout* aLayoutOwner,
                                       CGenericItutDataMgr* aDataMgr)
    : iDataMgr(aDataMgr),
      iLayoutOwner(aLayoutOwner),
      iWindowMgr(aWindowMgr),
      iIndiWithText( EFalse )
    {
    }

CGenericItutWindow::~CGenericItutWindow()
    {
    delete iBmpRotator;
    }

void CGenericItutWindow::SetPropertyL(MItutPropertySubscriber::TItutProperty aPropertyName, 
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

void CGenericItutWindow::CreateItutKeypadL()
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
    if ( iDataMgr->IsPortraitWest())
        {
    	CPenInputColorIcon* starIcon = CPenInputColorIcon::NewL( R_ITUT_KEYPAD_STAR_ICON );
        shiftIcon->ResizeL( iDataMgr->iShiftIconRectForPrtWest.Size());
        starIcon->ResizeL( iDataMgr->iStarIconRectForPrtWest.Size());
        iStandardItutKp->SetStarIcon( starIcon );
        }
    else
        {
        shiftIcon->ResizeL( iDataMgr->iShiftIconRect.Size());
        }
    iStandardItutKp->SetShiftIcon( shiftIcon );

    //set key to be 9 piece graphics
    iStandardItutKp->SetKeySkinId( EKeyBmpNormal, KAknsIIDQsnFrKeypadButtonFrNormal );
    iStandardItutKp->SetKeySkinId( EKeyBmpHighlight, KAknsIIDQsnFrKeypadButtonFrPressed );
    iStandardItutKp->SetKeySkinId( EKeyBmpDim, KAknsIIDQsnFrKeypadButtonFrInactive );
    }

void CGenericItutWindow::CreateLayoutIconL(TInt aMajorSkinId,
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

void CGenericItutWindow::ConstructItutKeypadFromResourceL(TInt aResId)
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

	//iLayoutOwner->RootControl()->Draw();
    }

void CGenericItutWindow::ChangeVirtualKeyInfoL(TResourceReader& aReader, 
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
         && ( !isThai ) && (curMode != EHangul) )
        {
        //This array is alloced when
        //HBufC* unicode = aReader.ReadHBufCL();
        //unicodesArr.AppendL(unicode);
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

CVirtualKey* CGenericItutWindow::CreateKeyL(TResourceReader& aReader,
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

void CGenericItutWindow::CreateBackGroundControlL()
	{
    iBackgroundCtrl = CAknFepCtrlCommonBgCtrl::NewL(
                                          iLayoutOwner,
                                          ECtrlIdBackground,
                                          KAknsIIDQsnFrPopup,
                                          KAknsIIDQsnFrPopupCenter);


	iBackgroundCtrl->SetRect(TItutDataConverter::AnyToRect(
					iLayoutOwner->DataMgr()->RequestData(ELayoutRect)));
					    
	AddControlL(iBackgroundCtrl);
	}


CAknFepCtrlCommonButton* CGenericItutWindow::CreateRawKeyButtonL(const TInt aControlId,
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
    
CAknFepCtrlCommonButton* CGenericItutWindow::CreateCommonButtonL(const TInt aControlId)
    {
    return CAknFepCtrlCommonButton::NewL(iLayoutOwner, 
    									 aControlId,
    									 KAknsIIDQsnFrFunctionButtonNormal,
										 KAknsIIDQsnFrFunctionButtonPressed,
										 KAknsIIDQsnFrFunctionButtonInactive );
     
    }    
    
CAknFepCtrlCommonButton* CGenericItutWindow::CreateRepeatButtonL(const TInt aControlId,
                                                                   const TInt aCommand)
    {
    return CAknFepCtrlRepeatButtonEx::NewL(iLayoutOwner,
                                           aControlId,
                                           KAknsIIDQsnFrFunctionButtonNormal,
										   KAknsIIDQsnFrFunctionButtonPressed,
										   KAknsIIDQsnFrFunctionButtonInactive,
                                           aCommand);
     
    }     

    
CFepUiBaseCtrl* CGenericItutWindow::CreateButtonL(TButtonType aType,
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
        case ERepeatButtonEx:
            {
            temp = CreateRepeatButtonL(aControlId, aCommand);    
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


void CGenericItutWindow::CreateAllButtonL()
    {
    //CFepUiBaseCtrl* temp = NULL;
    //create backgrand 
    CreateBackGroundControlL();
    
    //create backspace
    CreateButtonL(ERawKeyButton, ECtrlIdBackspace, EBackspaceRect, 
                  EBackspaceInnerRect,R_FINGER_RAWKEY_BACKSPACE);

    //create switch
    /*
    CreateButtonL(ECommonButton, ECtrlIdSwitch, ESwitchRect, 
                  ESwitchInnerRect, R_FINGER_LAYOUT_SWITCH);
                      
    //If open indicator menu, please use the following 2 lines
    temp = CreateButtonL(ECommonButton, ECtrlIdIndicator, KUnavailableID, 
                  KUnavailableID,KUnavailableID);
                  
    if (temp)
        {
        temp->Hide(ETrue);   
        }
    
    temp = NULL;
    */

    // create navi 
    CreateButtonL(ERawKeyButton, ECtrlIdArrowLeft, EArrowLeftRect, 
                  ELeftInnerRect,R_FINGER_RAWKEY_LEFTARROW);    
    
    CreateButtonL(ERawKeyButton, ECtrlIdArrowRight, EArrowRightRect, 
                  ERightInnerRect,R_FINGER_RAWKEY_RIGHTARROW); 

    CreateButtonL(ERepeatButtonEx, ECtrlIdArrowUp, EArrowUpRect, 
                  EUpInnerRect,R_PENINPUT_FINGER_CURSOR_UP); 

    CreateButtonL(ERepeatButtonEx, ECtrlIdArrowDown, EArrowDownRect, 
                  EDownInnerRect,R_PENINPUT_FINGER_CURSOR_DOWN);

    //create option 
    CreateButtonL(ECommonButton, ECtrlIdOptions, EOptionsRect, 
                  EOptionInnerRect, R_PENINPUT_FINGER_OPTIONS);    
    
    //Crease close
    CreateButtonL(ECommonButton, ECtrlIdClose, ECloseRect, 
                  ECloseInnerRect, R_PENINPUT_FINGER_CLOSE);  
                  
                  
    //CAknFepCtrlLabel* spellIndicator = CAknFepCtrlLabel::NewL(iLayoutOwner, ECtrlIdSpellIndicator);
    //AddControlL(spellIndicator);
    //spellIndicator->Hide(ETrue);

    }

void CGenericItutWindow::CreateDropdownListL()
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

    if (iDataMgr->IsLandScape())
        {
        iSpellCandsList->SetCandExpandType(CFepCtrlDropdownList::ECandExpandUpward);
        }

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

    if (iDataMgr->IsLandScape())
        {
        iCandsList->SetCandExpandType(CFepCtrlDropdownList::ECandExpandUpward);
        }

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

void CGenericItutWindow::ConstructIcfFromResourceL()
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
    TInt error = AknsUtils::GetCachedColor(skininstance, 
                                           icftextcolor, 
                                           colorid, 
                                           coloridx);

    if (error != KErrNone)
        {
        icftextcolor = TRgb(KDefaultIcfTextColor);
        }

    iICF->SetTextColorL(icftextcolor);    
    
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

    if (error != KErrNone)
        {
    	icfframecolor = TRgb(KDefaultIcfFrameColor);
        }

    iICF->SetBorderColor(icfframecolor);
    CleanupStack::PopAndDestroy( 1 ); // icfreader
    }

void CGenericItutWindow::CreateICFL()
    {
    CFont* icffont = TItutDataConverter::AnyToFont(iDataMgr->RequestData(EIcfFont));
    TRect rect = TItutDataConverter::AnyToRect(iDataMgr->RequestData(EIcfRect));
    iICF = CFepLayoutMultiLineIcf::NewL(rect, 
                                        iLayoutOwner, 
                                        ECtrlIdICF, 
                                        icffont->HeightInPixels(),
                                        icffont->FontMaxHeight(),
                                        icffont);

    iICF->SetFocus(ETrue);

    iICF->SetResourceId(R_FINGER_ICF);
    ConstructIcfFromResourceL();
    iICF->SetBgImgSkinId(KAknsIIDQgnGrafFepInputBg);
    //iDataMgr->SetTextAlignment();
    
    AddControlL(iICF);
    }

void CGenericItutWindow::AddEditorMenuL()
    {
    iEditorMenu = CPeninputCommonChoiceList::NewL( iLayoutOwner, 
                                        			ECtrlIdEditorMenu,
                                        			KAknsIIDQsnFrList,
                                        			KAknsIIDQsnFrPopupSub );
    
    AddControlL( iEditorMenu );
    }

void CGenericItutWindow::ShowEditorMenuL(TInt* aData)
    {
    if ( NULL == iEditorMenu )
        {
        return;
        }
    TInt count = *aData;
    iEditorMenu->ClearItemsL();
    for (TInt i=1;i<=count;i++ )
        {
        TInt menucommand = *(aData+i);
        CGenericItutDataMgr::TMenuItem* menuitem = iDataMgr->GetMenuItem(menucommand);
        if ( menuitem )
            {
            CFepLayoutChoiceList::SItem item;
            item.iCommand = menucommand;
            item.iText = menuitem->iText;
            iEditorMenu->AddItemL(item);                        
            }
        }
           
    if ( count > 0 )
        {
        iEditorMenu->SetListColumnNumWithLafL(count); 
        iEditorMenu->Display(Control(ECtrlIdIndicator)->Rect());
        }                
    }

void CGenericItutWindow::ConstructL()
    {
    iBmpRotator = CPeninputSyncBitmapRotator::NewL();
    CreateAllButtonL();
    CreateItutKeypadL();
    CreateICFL();
    CreateMatchSelectionCtrlL();
    
    /*
    AddEditorMenuL();
    
    iInputModeSwitch = CPeninputLayoutInputmodelChoice::NewL(
                                     iLayoutOwner,
                                     ECtrlIdInputSwitch,
                                     EPluginInputModeItut );
    iInputModeSwitch->SetListSkinID( KAknsIIDQsnFrList, KAknsIIDQsnFrPopupSub );
    AddControlL( iInputModeSwitch );
    */
    
   
    iBubbleSize = AknLayoutScalable_Apps::popup_char_count_window().LayoutLine();
    iBubbleTextLayout = AknLayoutScalable_Apps::popup_char_count_window_t1(0).LayoutLine();
    
    iICF->MsgBubbleCtrl()->SetTextFormat(iBubbleTextLayout);
    iICF->MsgBubbleCtrl()->SetTextColorIndex( EAknsCIQsnTextColorsCG67 );
    //Change the ID when ID into release
    iICF->MsgBubbleCtrl()->SetBitmapParam(NULL,
    									  NULL,
    									  KAknsIIDQsnFrInputPreviewSideL,
    									  KAknsIIDQsnFrInputPreviewMiddle,
    									  KAknsIIDQsnFrInputPreviewSideR);  
    iICF->SetMsgBubbleCtrlSize(TSize(iBubbleSize.iW,iBubbleSize.iH));
    iICF->MsgBubbleCtrl()->SetTextL( KEmptyString );
    /* Teleca change start, 18.05.2009 ssal */ 
    iICF->InfoBubbleCtrl()->SetTextFormat(iBubbleTextLayout); 
    iICF->InfoBubbleCtrl()->SetTextColorIndex( EAknsCIQsnTextColorsCG67 );
    iICF->InfoBubbleCtrl()->SetBitmapParam(NULL,
    									  NULL,
    									  KAknsIIDQsnFrInputPreviewSideL,
    									  KAknsIIDQsnFrInputPreviewMiddle,
    									  KAknsIIDQsnFrInputPreviewSideR);  
    iICF->SetInfoBubbleCtrlSize(TSize(iBubbleSize.iW,iBubbleSize.iH));
    }

void CGenericItutWindow::AddControlL(CFepUiBaseCtrl* aControl)
    {
    iLayoutOwner->AddControlL(aControl);   
    }

void CGenericItutWindow::CreateChineseSpecificCtrlsIfNeededL()
    {
    if (iChnCtrlInited)
        {
        return;
        }

    if( !Control( ECtrlIdSwitch ))
        {
        // create switch
        CreateButtonL(ECommonButton, ECtrlIdSwitch, ESwitchRect, 
                      ESwitchInnerRect, R_FINGER_LAYOUT_SWITCH);
        }

    if( !Control( ECtrlIdIndicator ) ) 
        {
        // create indicator button
        CFepUiBaseCtrl* indicatorBtn = CreateButtonL(ECommonButton, 
                ECtrlIdIndicator, KUnavailableID, 
                KUnavailableID,KUnavailableID);

        // create input case type menu
        AddEditorMenuL();
        }
    
    if( ! iInputModeSwitch )
    	{
		// create switch input mode menu
		iInputModeSwitch = CPeninputLayoutInputmodelChoice::NewL(
										 iLayoutOwner,
										 ECtrlIdInputSwitch,
										 EPluginInputModeItut );
		iInputModeSwitch->SetListSkinID( KAknsIIDQsnFrList, KAknsIIDQsnFrPopupSub );
		AddControlL( iInputModeSwitch );
    	}
    // create spell indicator
    CAknFepCtrlLabel* spellIndicator = CAknFepCtrlLabel::NewL(iLayoutOwner, ECtrlIdSpellIndicator);
    AddControlL(spellIndicator);
    spellIndicator->Hide(ETrue);
    
    
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

void CGenericItutWindow::ConstructSpellCtrlFromResourceL()
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

    CleanupStack::PushL( bmp );       
    CleanupStack::PushL( maskbmp );       

    if (bmp)
        {
        iSpell->SetBackgroundBitmapL(bmp);
        }
  
    CleanupStack::PopAndDestroy( maskbmp );       
    CleanupStack::Pop( bmp );

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


TRgb CGenericItutWindow::GetMatchListTextColor()
    {
    MAknsSkinInstance* skininstance = AknsUtils::SkinInstance();
    TRgb matchlistcolor;
    TInt error = AknsUtils::GetCachedColor(skininstance, 
                                           matchlistcolor, 
                                           KAknsIIDQsnTextColors,
                                           EAknsCIQsnTextColorsCG43);

    if (error != KErrNone)
        {
        matchlistcolor = TRgb(KDefaultIcfTextColor);
        }
    
    return matchlistcolor;
    }

void CGenericItutWindow::CreateMatchSelectionCtrlL()
    {
    // size and pos info of new match selection list should get from laf: later
    iMatchSelection = CFepLayoutScrollableList::NewL(iLayoutOwner, 
                                                     ECtrlIdMatchSelection,
                                                     iDataMgr->iSelectionItemSize,
                                                     iDataMgr->iSelectionHorMargin,
                                                     iDataMgr->iSelectionVerMargin,
                                                     iDataMgr->iSelectionNaviSize,
                                                     iDataMgr->iSelectionNaviInnerSize);
     
    iMatchSelection->SetImgFrId(KAknsIIDQsnFrPopupSub, 
                                KAknsIIDQsnFrFunctionButtonNormal,
								KAknsIIDQsnFrFunctionButtonPressed,
								KAknsIIDQsnFrFunctionButtonInactive,
                                KAknsIIDQsnFrList,
                                KAknsIIDQsnFrPopupSub);
    CFont* itemfont = TItutDataConverter::AnyToFont(iDataMgr->RequestData(EIcfFont));
    TRgb textcolor = GetMatchListTextColor();

    iMatchSelection->SetTextProperty(itemfont, textcolor);
    iMatchSelection->SetResourceId(R_FINGER_SELECTION_LIST);
    iMatchSelection->ConstructFromResourceL();
    iMatchSelection->SetTextLineLayout( iDataMgr->iSelectionTextLine );
    iMatchSelection->SetPageInfoTextLineLayout( iDataMgr->iSelectionPageTextLine );
    iMatchSelection->SetPageInfoResID( R_PENINPUT_SELECTION_LIST_PAGE_NUM );
    AddControlL(iMatchSelection);
    }

TInt CGenericItutWindow::OnAppEditorTextComing(const TFepInputContextFieldData& aData)
    {
    TRAPD(err, iICF->SetTextL(aData));

    // make sure if selection list on top of multiline icf,
    // it will not be hidden by icf
    if (!iMatchSelection->Hiden())
        {
        iMatchSelection->Draw();
        }

    return err;
    }

void CGenericItutWindow::SetIndiBubbleImageL( const TInt aImgID1,
                                              const TInt aMaskID1,
                                              const TInt aImgID2,
                                              const TInt aMaskID2 )
    {
    MAknsSkinInstance* skininstance = AknsUtils::SkinInstance();

    CFbsBitmap* bmp1 = NULL;
    CFbsBitmap* mask1 = NULL;
    
    TInt colorIndex = EAknsCIQsnIconColorsCG30;

    AknsUtils::CreateColorIconL(skininstance,
                                KAknsIIDQsnIconColors,
                                KAknsIIDQsnIconColors,
                                colorIndex,
                                bmp1,
                                mask1,
                                AknIconUtils::AvkonIconFileName(),
                                aImgID1,
                                aMaskID1,
                                KRgbGray);
    CleanupStack::PushL( bmp1 );
    CleanupStack::PushL( mask1 );
                                
    AknIconUtils::GetContentDimensions(bmp1, iIndicatorSize);

    CFbsBitmap* bmp2 = NULL;
    CFbsBitmap* mask2 = NULL;
    AknsUtils::CreateColorIconL(skininstance,
                                KAknsIIDQsnIconColors,
                                KAknsIIDQsnIconColors,
                                colorIndex,
                                bmp2,
                                mask2,
                                AknIconUtils::AvkonIconFileName(),
                                aImgID2,
                                aMaskID2,
                                KRgbGray);
 
    CleanupStack::PushL( bmp2 );
    CleanupStack::PushL( mask2 );
    
    AknIconUtils::GetContentDimensions(bmp2, iIndicatorTextSize);
    
    TRect boundRect;
    if ( iIndiWithText )
        {
        boundRect = TItutDataConverter::AnyToRect(
                iDataMgr->RequestData(EIndiIconWithTextRect));
        }
    else
        {
        boundRect = TItutDataConverter::AnyToRect(
                iDataMgr->RequestData(EIndiIconWithoutTextRect));
        }
    TRect imgrect, textrect;
    
    CalIndicatorRect(boundRect, imgrect, textrect, EIndiAlignCenter);
    AknIconUtils::SetSize(bmp1, imgrect.Size(), EAspectRatioNotPreserved);
    AknIconUtils::SetSize(mask1, imgrect.Size(), EAspectRatioNotPreserved);
    AknIconUtils::SetSize(bmp2, textrect.Size(), EAspectRatioNotPreserved);
    AknIconUtils::SetSize(mask2, textrect.Size(), EAspectRatioNotPreserved);

    CFbsBitmap* bmp3 = AknPenImageUtils::CombineTwoImagesL(bmp1, bmp2, bmp1->DisplayMode());
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
void CGenericItutWindow::SetIndicatorImageL(CFepUiBaseCtrl* aIndicator,
                                              const TInt aImgID1,
                                              const TInt aMaskID1,
                                              const TInt aImgID2,
                                              const TInt aMaskID2)
    {
    if ( NULL == aIndicator )
        {
        return;
        }
    MAknsSkinInstance* skininstance = AknsUtils::SkinInstance();

    CFbsBitmap* bmp1 = NULL;
    CFbsBitmap* mask1 = NULL;
    
    TInt colorIndex;
    if (iLayoutOwner->UiMgr()->CurrentState()->StateType() ==
            CGenericItutUiMgrBase::EStateSpelling)
		{
		colorIndex = EAknsCIQsnIconColorsCG10;
		}
    else
		{
		colorIndex = EAknsCIQsnIconColorsCG30;
		}
    AknsUtils::CreateColorIconL(skininstance,
                                KAknsIIDQsnIconColors,
                                KAknsIIDQsnIconColors,
                                colorIndex,
                                bmp1,
                                mask1,
                                AknIconUtils::AvkonIconFileName(),
                                aImgID1,
                                aMaskID1,
                                KRgbGray);
    CleanupStack::PushL( bmp1 );
    CleanupStack::PushL( mask1 );
                                
    AknIconUtils::GetContentDimensions(bmp1, iIndicatorSize);

    CFbsBitmap* bmp2 = NULL;
    CFbsBitmap* mask2 = NULL;
    AknsUtils::CreateColorIconL(skininstance,
                                KAknsIIDQsnIconColors,
                                KAknsIIDQsnIconColors,
                                colorIndex,
                                bmp2,
                                mask2,
                                AknIconUtils::AvkonIconFileName(),
                                aImgID2,
                                aMaskID2,
                                KRgbGray);
 
    CleanupStack::PushL( bmp2 );
    CleanupStack::PushL( mask2 );
    
    AknIconUtils::GetContentDimensions(bmp2, iIndicatorTextSize);

    TRect boundRect = TItutDataConverter::AnyToRect(iDataMgr->RequestData(EImIndicatorRect));
    TRect imgrect, textrect;

    if (aIndicator->ControlId() == ECtrlIdIndicator)
        {
        CalIndicatorRect(boundRect, imgrect, textrect, EIndiAlignCenter);
        AknIconUtils::SetSize(bmp1, imgrect.Size(), EAspectRatioNotPreserved);
        AknIconUtils::SetSize(mask1, imgrect.Size(), EAspectRatioNotPreserved);
        AknIconUtils::SetSize(bmp2, textrect.Size(), EAspectRatioNotPreserved);
        AknIconUtils::SetSize(mask2, textrect.Size(), EAspectRatioNotPreserved);

        CFbsBitmap* bmp3 = AknPenImageUtils::CombineTwoImagesL(bmp1, bmp2, EColor256);
        CFbsBitmap* mask3 = AknPenImageUtils::CombineTwoImagesL(mask1, mask2, EGray256);
        
        TInt bigger = imgrect.Height() > textrect.Height() ? imgrect.Height() : textrect.Height();
        TRect indirect(imgrect.iTl, TSize(imgrect.Width() + textrect.Width(), bigger));
        // button like indicator
        CAknFepCtrlCommonButton* indibtn = static_cast<CAknFepCtrlCommonButton*>(aIndicator);
        TRect outerrect = 
                       TItutDataConverter::AnyToRect(iDataMgr->RequestData(EImIndicatorOuterRect));
        /*if (IsChineseMode())
            {
            indirect.Move( TPoint(0 , 0));
            }
        else
            {
            indirect.Move(TPoint( -20 ,0));//
            }
        */
        indibtn->SetForgroundBmpRect(indirect);
        indibtn->SetForegroundBmpL(bmp3, mask3);
        indibtn->SetRect(outerrect);
        //indibtn->Hide(EFalse);
        }
    else
        {
        CalIndicatorRect(boundRect, imgrect, textrect, EIndiAlignRight);
        AknIconUtils::SetSize(bmp1, imgrect.Size(), EAspectRatioNotPreserved);
        AknIconUtils::SetSize(mask1, imgrect.Size(), EAspectRatioNotPreserved);
        AknIconUtils::SetSize(bmp2, textrect.Size(), EAspectRatioNotPreserved);
        AknIconUtils::SetSize(mask2, textrect.Size(), EAspectRatioNotPreserved);

        CFbsBitmap* bmp3 = AknPenImageUtils::CombineTwoImagesL(bmp1, bmp2, EColor256);
        CFbsBitmap* mask3 = AknPenImageUtils::CombineTwoImagesL(mask1, mask2, EGray256);
        
        TInt bigger = imgrect.Height() > textrect.Height() ? imgrect.Height() : textrect.Height();
        TRect indirect(imgrect.iTl, TSize(imgrect.Width() + textrect.Width(), bigger));
 		
        // label like indicator
        CAknFepCtrlLabel* indilabel = static_cast<CAknFepCtrlLabel*>(aIndicator);
        indilabel->SetBitmapL(bmp3);
        indilabel->SetMaskBitmapL(mask3);
        indilabel->SetRect(indirect);
        //indilabel->Hide(EFalse);
        }

    CleanupStack::PopAndDestroy( mask2 );
    CleanupStack::PopAndDestroy( bmp2 );
    CleanupStack::PopAndDestroy( mask1 );
    CleanupStack::PopAndDestroy( bmp1 );    
    }

void CGenericItutWindow::UpdateIndiBubbleL( TUint8* aData )
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

    if (indicatorData.iIndicatorImgID != 0 && 
        indicatorData.iIndicatorMaskID != 0 && 
        indicatorData.iIndicatorTextImgID != 0 &&
        indicatorData.iIndicatorTextMaskID != 0)
        {
        iDataMgr->SetIndicatorData(indicatorData);
        iImDimensionSet = ETrue;
        
        SetIndiBubbleImageL( indicatorData.iIndicatorImgID,
                             indicatorData.iIndicatorMaskID,
                             indicatorData.iIndicatorTextImgID,
                             indicatorData.iIndicatorTextMaskID);
        
        if ( iLayoutOwner->UiMgr()->CurrentState()->StateType() ==
            CGenericItutUiMgrBase::EStateSpelling )
            {
			TRect bubbleRect = TItutDataConverter::AnyToRect( 
					iDataMgr->RequestData( ESpellIndiPaneWithoutTextRect ));
			TRect iconRect = TItutDataConverter::AnyToRect( 
					iDataMgr->RequestData( ESpellIndiIconWithoutTextRect ));
			TSize offset;
	        offset.iHeight = iconRect.iTl.iY - bubbleRect.iTl.iY;
	        offset.iWidth = iconRect.iTl.iX - bubbleRect.iTl.iX;
			TSize size( iconRect.Width(), iconRect.Height());
			
			iICF->MsgBubbleCtrl()->SetRect( bubbleRect );
			iICF->MsgBubbleCtrl()->SetIconOffsetAndSize( offset, size );
            iICF->ShowBubble( KEmptyString, iICF->MsgBubbleCtrl()->Rect());
            }
        else
            {
            TBuf<100> text;
            iICF->MsgBubbleCtrl()->GetText( text );
            iICF->ShowBubble( text, iICF->MsgBubbleCtrl()->Rect());
            }
        }
    }

void CGenericItutWindow::UpdateIndicatorL(TUint8* aData)
    {
    if ( iLayoutOwner->UiMgr()->CurrentState()->StateType() == 
            CGenericItutUiMgrBase::EStateComposition ||
         iLayoutOwner->UiMgr()->CurrentState()->StateType() == 
            CGenericItutUiMgrBase::EStatePredict )
        {
        return;
        }
        
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

    if (indicatorData.iIndicatorImgID != 0 && 
        indicatorData.iIndicatorMaskID != 0 && 
        indicatorData.iIndicatorTextImgID != 0 &&
        indicatorData.iIndicatorTextMaskID != 0)
        {
        iDataMgr->SetIndicatorData(indicatorData);
        iImDimensionSet = ETrue;
        
        
        CFepUiBaseCtrl* temp = NULL;
        
        
        if (iLayoutOwner->UiMgr()->CurrentState()->StateType() ==
            CGenericItutUiMgrBase::EStateSpelling)
            {
            temp = Control(ECtrlIdSpellIndicator);
            if ( !temp )
                {
                return;
                }
            
            CFepUiBaseCtrl* indi = Control(ECtrlIdIndicator);
            if ( !indi )
                {
                return;
                }
            indi->Hide(ETrue);  
	        SetIndicatorImageL(temp, 
	                           indicatorData.iIndicatorImgID,
	                           indicatorData.iIndicatorMaskID,
	                           indicatorData.iIndicatorTextImgID,
	                           indicatorData.iIndicatorTextMaskID);
	        if( temp->Hiden() )               
	            {
	            temp->Hide( EFalse );    
	            }
	        else
	            {
	           	iLayoutOwner->RootControl()->ReDrawRect(temp->Rect()); 
	            iLayoutOwner->RootControl()->UpdateArea( temp->Rect() );
	            }
            }
        else
            {
            temp = Control(ECtrlIdIndicator); 
            
            CFepUiBaseCtrl* indi = Control(ECtrlIdSpellIndicator);
            if( indi )
            	{
				indi->Hide(ETrue);
            	}
            
            if( temp )
            	{
				SetIndicatorImageL(temp, 
								   indicatorData.iIndicatorImgID,
								   indicatorData.iIndicatorMaskID,
								   indicatorData.iIndicatorTextImgID,
								   indicatorData.iIndicatorTextMaskID);
				if( temp->Hiden() )               
					{
					temp->Hide( EFalse );    
					}
				else
					{
						temp->ClearRect( temp->Rect() );
					temp->Draw();
					temp->UpdateArea( temp->Rect() );
					iLayoutOwner->RootControl()->ReDrawRect( temp->Rect() );
					} 
            	}
            }
        }
    }

void CGenericItutWindow::CalIndicatorRect(const TRect& aBoundRect,
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
		
		/*	TInt n = (nTotalWidth - aBoundRect.Size().iWidth) / 2;
		imgSizeText.iWidth -= n;
		imgSizeIndi.iWidth -= n;
		imgSizeText.iHeight -= n / imgAspectText;
		imgSizeIndi.iHeight -= n / imgAspectIndi;
		*/
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

void CGenericItutWindow::SetPromptTextL(TUint8* aData)
    {
    RDesReadStream readStream;

    TPtr8 countPtr(aData, 2*sizeof(TInt), 2*sizeof(TInt));
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
    if (textCount > 0)
        {
        textBuf = HBufC::NewLC(textCount/2);
    	TPtr textBufPtr = textBuf->Des();
    	readStream.ReadL(textBufPtr, textCount/2);
         
        const HBufC* icfPromptText = iICF->PromptText();
        
        if (!icfPromptText || icfPromptText->Compare(textBuf->Des()) != 0 )
        	{
        	iICF->SetPromptTextL( textBuf->Des(), pIcfData->iCleanContent );	
        	}
        
        CleanupStack::PopAndDestroy(textBuf);
        }
    else
        {
        iICF->SetPromptTextL(KNullDesC, pIcfData->iCleanContent);
        }

    CleanupStack::PopAndDestroy(dataBuf);
	  CleanupStack::PopAndDestroy(&readStream);
    }

void CGenericItutWindow::SetSpellDisplayContentL( TUint8* aData )
	{
	TBool isInSpell = iDataMgr->IsSpellMode();
	if ( isInSpell )
		{
	    return;
		}
	
	RDesReadStream readStream;

	TPtr8 countPtr( aData, 3 * sizeof(TInt), 3 * sizeof(TInt));
	readStream.Open( countPtr );
	CleanupClosePushL( readStream );
	
	const TInt dataCount = readStream.ReadInt32L();
	const TInt icfTextCount = readStream.ReadInt32L();
	const TInt promptTextCount = readStream.ReadInt32L();
	CleanupStack::PopAndDestroy( &readStream );
	
	TPtr8 ptr( aData + 3 * sizeof(TInt), 
		dataCount + icfTextCount + promptTextCount, 
		dataCount + icfTextCount + promptTextCount );
	
    readStream.Open( ptr );
    CleanupClosePushL(readStream);
    
    HBufC8* dataBuf = HBufC8::NewLC( dataCount );
    TPtr8 dataBufPtr = dataBuf->Des();
    readStream.ReadL( dataBufPtr, dataCount );
    
    TFepSpellICFDisplayContent* pContent = 
    		reinterpret_cast<TFepSpellICFDisplayContent*>(
    				const_cast<TUint8*>(dataBufPtr.Ptr()));
    
    TFepInputContextFieldData icfData;
	icfData.iCmd = EPeninputICFInitial;
	icfData.iCurSel.iCursorPos = pContent->iCurPos;
	icfData.iCursorSelVisible = ETrue;
	icfData.iCursorVisibility = ETrue;
    		
    if ( icfTextCount > 0 )
    	{
		HBufC* textIcfBuf;
		textIcfBuf = HBufC::NewLC( icfTextCount/2 );
		TPtr textBufPtr = textIcfBuf->Des();
		readStream.ReadL( textBufPtr, icfTextCount/2 );
		
		icfData.iLength = icfTextCount;
		icfData.iText.Set( textIcfBuf->Des());
		
		iICF->SetTextL( icfData );
		
		CleanupStack::PopAndDestroy( textIcfBuf );
    	}
    else
    	{
		icfData.iLength = 0;
		icfData.iText.Set( KNullDesC );
		
		iICF->SetTextL( icfData );
    	}
    
    if ( promptTextCount > 0 )
    	{
		HBufC* textPromptBuf;
		textPromptBuf = HBufC::NewLC( promptTextCount/2 );
		TPtr promptBufPtr = textPromptBuf->Des();
		readStream.ReadL( promptBufPtr, promptTextCount/2 );
		
		iICF->SetPromptTextL( textPromptBuf->Des(), ETrue );	
		
		CleanupStack::PopAndDestroy( textPromptBuf );
    	}
    else
    	{
		iICF->SetPromptTextL( KNullDesC, ETrue );
    	}
    
    CleanupStack::PopAndDestroy( dataBuf );
	CleanupStack::PopAndDestroy( &readStream );
	}

TInt CGenericItutWindow::PenInputType()
    {
    return EPluginInputModeItut;
    }

void CGenericItutWindow::SetCtrlRect(CFepUiBaseCtrl* aCtrl, TInt aRectIdx)
    {
    aCtrl->SetRect(TItutDataConverter::AnyToRect(iDataMgr->RequestData(aRectIdx)));
    }

TInt CGenericItutWindow::SizeChanged(TBool aIcfOverlap)
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
   iMatchSelection->SetPageInfoTextLineLayout( iDataMgr->iSelectionPageTextLine );
   iMatchSelection->SizeChanged( 
        iDataMgr->iSelectionItemSize,
        iDataMgr->iSelectionHorMargin,
        iDataMgr->iSelectionVerMargin,
        iDataMgr->iSelectionNaviSize,
        iDataMgr->iSelectionNaviInnerSize);

    // should set textline when it is defined in laf
    CFont* itemfont = TItutDataConverter::AnyToFont(iDataMgr->RequestData(EIcfFont));
    TRgb textcolor = GetMatchListTextColor();
    iMatchSelection->SetTextProperty(
        TItutDataConverter::AnyToFont(iDataMgr->RequestData(EIcfFont)), GetMatchListTextColor());

	iBackgroundCtrl->SizeChanged( 
                         TItutDataConverter::AnyToRect(iDataMgr->RequestData( ELayoutRect ) ) );
	
    TRAP_IGNORE(ApplyVariantLafDataL(ETrue); 
                iLayoutOwner->HandleCommand(EItutExtCmdSizeChanged, NULL));
    
    
    if (iChnCtrlInited)
        {
        SetCtrlRect(iSpell, ESpellRect);
        iSpell->SetFont(TItutDataConverter::AnyToFont(iDataMgr->RequestData(ESpellFont)));

        ResizeCandidateList(iSpellCandsList, EDropdownListSpellLTPos, ETrue);
        ResizeCandidateList(iCandsList, EDropdownListCandsLTPos, ETrue);
        ResizeCandidateList(iPuncCandsList, EDropdownListPuncLTPos, EFalse);
        }
        
    if (aIcfOverlap && !iMatchSelection->Hiden())
        {
        iICF->SetOverLapState(ETrue, iMatchSelection->Rect());
        }

    return KErrNone;    
    }

TBool CGenericItutWindow::ControlSizeChanged(const TInt aControlId, 
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

void CGenericItutWindow::ApplyVariantLafDataL(TBool aResolutionChange)
    {
    if (aResolutionChange)
        {
        CAknFepCtrlCommonButton* indibutton = CommonButtonControl(ECtrlIdIndicator);

        SetIndicatorImageL(indibutton, 
                           iDataMgr->IndicatorData().iIndicatorImgID,
                           iDataMgr->IndicatorData().iIndicatorMaskID,
                           iDataMgr->IndicatorData().iIndicatorTextImgID,
                           iDataMgr->IndicatorData().iIndicatorTextMaskID);              
        }

    CFont* icffont = TItutDataConverter::AnyToFont(iDataMgr->RequestData(EIcfFont));
    iICF->Hide(ETrue);
    
    if( iDataMgr->IsChinese() )
    	{
    	iICF->SetTextMargin ( iDataMgr->iIcfTextLeftMarginCn,
    					  iDataMgr->iIcfTextRightMarginCn,
    					  iDataMgr->iIcfTextTopMarginCn,
    					  iDataMgr->iIcfTextBottomMarginCn );
    					  
	    iICF->SetLineSpace( iDataMgr->iIcfTextLineSpaceMarginCn );    					  
	 
	    iICF->SizeChangedL(TItutDataConverter::AnyToRect(iDataMgr->RequestData(EIcfRect)), 
	                      //icffont->HeightInPixels(),
	                      iDataMgr->iIcfTextHeightCn,
	                      icffont->FontMaxHeight(),
	                      icffont);	    		
    	}
    else
    	{
    	if ( iDataMgr->IsLandScape())
    	    {
    	    iICF->SetTextMargin ( iDataMgr->iIcfTextLeftMargin,
    	                          iDataMgr->iIcfTextRightMargin,
    	                          iDataMgr->iIcfTextTopMargin,
    	                          iDataMgr->iIcfTextBottomMargin );
    	                                  
            iICF->SetLineSpace( iDataMgr->iIcfTextLineSpaceMargin );                          
         
            iICF->SizeChangedL(TItutDataConverter::AnyToRect(iDataMgr->RequestData(EIcfRect)), 
                              iDataMgr->iIcfTextHeight,
                              icffont->FontMaxHeight(),
                              icffont);
    	    }
    	else
    	    {
    	    iICF->SetTextMargin ( iDataMgr->iIcfTextLeftMarginForPrtWest,
    	                          iDataMgr->iIcfTextRightMarginForPrtWest,
    	                          iDataMgr->iIcfTextTopMarginForPrtWest,
    	                          iDataMgr->iIcfTextBottomMarginForPrtWest );
    	                                  
            iICF->SetLineSpace( iDataMgr->iIcfTextLineSpaceMarginForPrtWest );                          
         
            iICF->SizeChangedL(TItutDataConverter::AnyToRect(iDataMgr->RequestData(EIcfRect)),
                              iDataMgr->iIcfTextHeightForPrtWest,
                              icffont->FontMaxHeight(),
                              icffont);
    	    }
    	}

    iICF->Hide(EFalse);    
    
    if ( iDataMgr->IsPortraitWest() && (!iDataMgr->IsChineseSpellMode()))
        {
        iICF->MsgBubbleCtrl()->SetTextL( KEmptyString );
        iIndiWithText = EFalse;
        IndiBubbleWithoutText();
        }
    if( iDataMgr->IsChinese() )
        {
        ControlSizeChanged(ECtrlIdArrowUp, EArrowUpRect, EUpInnerRect, ETrue);
        ControlSizeChanged(ECtrlIdArrowDown, EArrowDownRect, EDownInnerRect, ETrue);
        }
    
    TBool bSizeChanged = EFalse;       
    ControlSizeChanged(ECtrlIdArrowLeft, EArrowLeftRect, ELeftInnerRect, ETrue);
    ControlSizeChanged(ECtrlIdArrowRight, EArrowRightRect, ERightInnerRect, ETrue);
    if( iDataMgr->IsChinese() || iDataMgr->IsKorean() )
        {
        ControlSizeChanged(ECtrlIdSwitch, ESwitchRect, ESwitchInnerRect, ETrue);
        }
    bSizeChanged = ControlSizeChanged(ECtrlIdBackspace, EBackspaceRect, EBackspaceInnerRect, ETrue);
    ControlSizeChanged(ECtrlIdOptions, EOptionsRect, EOptionInnerRect, ETrue);
    ControlSizeChanged(ECtrlIdClose, ECloseRect, ECloseInnerRect, ETrue);
   						  	
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
    // Handle control res when language direction changing here.
    if( iDataMgr->IsLangDirectionSwitch() || 
    	( bSizeChanged && iDataMgr->IsRtoLLanguage() ) )
    	{
    	HandleButtonResOnLangDirChange( ECtrlIdBackspace );
    	iDataMgr->SetLangDirectionSwitch( EFalse );	
    	}   
    }

void CGenericItutWindow::ResizeCandidateList(CFepCtrlDropdownList* aList, 
                                             TInt aLTPosIdx, TBool aExpandable)
    {
    if (!aList)
        {
        return;   
        }
    
    TInt unitWidth = TItutDataConverter::AnyToInt(iDataMgr->RequestData(EDropdownlistUnitWidth));
    TInt unitHeight = TItutDataConverter::AnyToInt(iDataMgr->RequestData(EDropdownlistUnitHeight));
    TInt horizontalMargin = 
        TItutDataConverter::AnyToInt(iDataMgr->RequestData(EDropdownlistHorizontalMargin));
    TInt verticalMargin = 
        TItutDataConverter::AnyToInt(iDataMgr->RequestData(EDropdownlistVerticalMargin));
    //TInt naviWidth = TItutDataConverter::AnyToInt(iDataMgr->RequestData(EDropdownListNaviWidth));
    TInt naviWidth = TItutDataConverter::AnyToInt(iDataMgr->RequestData(EDropdownlistUnitWidth));
    CFont* listfont = TItutDataConverter::AnyToFont(iDataMgr->RequestData(EDropdownListFont));
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

void CGenericItutWindow::OnSkinChange()
    {
    CFont* itemfont = TItutDataConverter::AnyToFont(iDataMgr->RequestData(EIcfFont));
    TRgb textcolor = GetMatchListTextColor();
    iMatchSelection->SetTextProperty(itemfont, textcolor); 
    
    // when skin change all control with skin image should be reload.
    TRAP_IGNORE(ConstructKeyImageFromResourceL( R_ITUT_KEYPAD_BITMAP ));    
    
    // reconstruct shift icon when skin changed
    TRAP_IGNORE(iStandardItutKp->ShiftIcon()->ReConstructL());
    if ( iDataMgr->IsPortraitWest())
        {
        TRAP_IGNORE(iStandardItutKp->StarIcon()->ReConstructL());
        TRAP_IGNORE(iStandardItutKp->ShiftIcon()->ResizeL(iDataMgr->iShiftIconRectForPrtWest.Size()));
        TRAP_IGNORE(iStandardItutKp->StarIcon()->ResizeL(iDataMgr->iStarIconRectForPrtWest.Size()));
        }
    else
        {
        TRAP_IGNORE(iStandardItutKp->ShiftIcon()->ResizeL(iDataMgr->iShiftIconRect.Size()));
        }
    
    //Skin change will case the reconstuction of the button graphic.
    //When language is RToL, it should be mirrored again.
    if( iDataMgr->IsRtoLLanguage() )
    	{
    	HandleButtonResOnLangDirChange( ECtrlIdBackspace );	
    	}
    }


void CGenericItutWindow::CalculateFrameRects(const TRect aRect,
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
    
void CGenericItutWindow::EnterMatchSelectionState(TBool aOn)
    {
    TBuf<4> buf;
    buf.Append(reinterpret_cast<TText*>(&aOn), sizeof(TBool)/sizeof(TText));
    iLayoutOwner->SignalOwner(ESignalEnterMatchSelection, buf);
    }

void CGenericItutWindow::ResetIndicatorL(CFepUiBaseCtrl* aIndicator)
	{
	if ( NULL == aIndicator )
	    {
	    return;
	    }
    SetIndicatorImageL(aIndicator, 
                       iDataMgr->IndicatorData().iIndicatorImgID,
                       iDataMgr->IndicatorData().iIndicatorMaskID,
                       iDataMgr->IndicatorData().iIndicatorTextImgID,
                       iDataMgr->IndicatorData().iIndicatorTextMaskID);
	}

CAknFepCtrlCommonButton* CGenericItutWindow::CommonButtonControl(const TInt aControlId)
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

void CGenericItutWindow::DisplayInputmodeSwitchL()
    {
    CFepUiBaseCtrl* btn = Control(ECtrlIdSwitch);
    if ( NULL == btn )
        {
        return;
        }
    
    TRect rect = btn->Rect();
              
    TRAP_IGNORE( iInputModeSwitch->PopUpSwitchListL(rect) );        
    }

CFepUiBaseCtrl* CGenericItutWindow::Control(const TInt aCtrlId)
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

TBool CGenericItutWindow::IsChineseMode()
    {
    TInt immode = iDataMgr->InputMode();
    TBool chineseMode = EFalse;
    
    if (immode == EPinyin || immode == EStroke || immode == EZhuyin)
        {
        chineseMode = ETrue;
        }
    
    return chineseMode;    
    }

void CGenericItutWindow::ConstructKeyImageFromResourceL( TInt aKeyImageResID )
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
	
void CGenericItutWindow::HandleButtonResOnLangDirChange( TInt aControlId )
	{
    TRAP_IGNORE(HandleButtonResOnLangDirChangeL(aControlId));
    }
void CGenericItutWindow::HandleButtonResOnLangDirChangeL( TInt aControlId )
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

void CGenericItutWindow::SetHardKeyOneSymbol(TUint8* aData)
    {
    TRAP_IGNORE(SetHardKeyOneSymbolL(aData));
    }
void CGenericItutWindow::SetHardKeyOneSymbolL(TUint8* aData)
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

void CGenericItutWindow::SetUnicodesForHardKey1(CVirtualKey* aKey, const TDesC& aMapData)
    {
    TRAP_IGNORE(SetUnicodesForHardKey1L(aKey, aMapData));
    }
void CGenericItutWindow::SetUnicodesForHardKey1L(CVirtualKey* aKey, const TDesC& aMapData)
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

void CGenericItutWindow::IndiBubbleWithText()
    {
    if ( iICF )
        {
        TAknTextLineLayout textLine = TItutDataConverter::AnyToTextLine( 
                iDataMgr->RequestData( EIndiTextLine ));
        TRect bubbleRect = TItutDataConverter::AnyToRect( 
                iDataMgr->RequestData( EIndiPaneWithTextRect ));
        TRect iconRect = TItutDataConverter::AnyToRect( 
                iDataMgr->RequestData( EIndiIconWithTextRect ));
        TSize offset;
        offset.iHeight = iconRect.iTl.iY - bubbleRect.iTl.iY;
        offset.iWidth = iconRect.iTl.iX - bubbleRect.iTl.iX;
        TSize size( iconRect.Width(), iconRect.Height());
        
        iICF->MsgBubbleCtrl()->SetRect( bubbleRect );
        iICF->MsgBubbleCtrl()->SetIconOffsetAndSize( offset, size );
        iICF->MsgBubbleCtrl()->SetTextFormat( textLine );
        iICF->MsgBubbleCtrl()->SetTextColorIndex( EAknsCIQsnTextColorsCG67 );
        }
    }

void CGenericItutWindow::IndiBubbleWithoutText()
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

void CGenericItutWindow::MsgBubbleForChinese()
	{
	if ( iICF )
		{
		iBubbleSize = AknLayoutScalable_Apps::popup_char_count_window().LayoutLine();
	    iBubbleTextLayout = AknLayoutScalable_Apps::popup_char_count_window_t1(0).LayoutLine();
		
	    iICF->MsgBubbleCtrl()->SetTextFormat(iBubbleTextLayout);
	    iICF->MsgBubbleCtrl()->SetBitmapParam(NULL,
	    									  NULL,
	    									  KAknsIIDQsnFrInputPreviewSideL,
	    									  KAknsIIDQsnFrInputPreviewMiddle,
	    									  KAknsIIDQsnFrInputPreviewSideR);  
	    iICF->SetMsgBubbleCtrlSize(TSize(iBubbleSize.iW,iBubbleSize.iH));
	    TRAP_IGNORE(iICF->MsgBubbleCtrl()->SetTextL( KEmptyString ));
		}
	}
void CGenericItutWindow::SetIndiWithTextFlag( TBool aFlag )
    {
    iIndiWithText = aFlag;
    }
void CGenericItutWindow::CreateKoreanSpecificCtrlsIfNeededL()
    {
	if( !Control(ECtrlIdIndicator) )
		{
	    CreateButtonL(ECommonButton, ECtrlIdIndicator, KUnavailableID, 
					  KUnavailableID,KUnavailableID);
        // create input case type menu
	    AddEditorMenuL();
		}
	
	if( !Control(ECtrlIdSwitch))
		{
		CreateButtonL(ECommonButton, ECtrlIdSwitch, ESwitchRect, 
                    ESwitchInnerRect, R_FINGER_LAYOUT_SWITCH);
	
		}
	
	
	if( !iInputModeSwitch )
		{
		// create switch input mode menu
		iInputModeSwitch = CPeninputLayoutInputmodelChoice::NewL(
										 iLayoutOwner,
										 ECtrlIdInputSwitch,
										 EPluginInputModeItut );
		iInputModeSwitch->SetListSkinID( KAknsIIDQsnFrList, KAknsIIDQsnFrPopupSub );
		AddControlL( iInputModeSwitch );
		}
    }
// End Of File
