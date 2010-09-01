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
#include <AknUtils.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <barsread.h>
#include <AknsUtils.h>
#include <peninputmodeswitchutilsnew.rsg>
#include <bautils.h>
#include <AknFepGlobalEnums.h> 
#include <peninputlayoutinputmodechoice.h>
#include <AknFepInternalCRKeys.h>
#include <centralrepository.h>
#include <aknappui.h> 

// CONSTANT 
const TInt KInvalidImg = -1 ;
_LIT(KAknUtilsResourceFileName, "z:\\resource\\peninputmodeswitchutilsnew.rsc");
const TInt KCCMask(0x00000fff);


// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CPeninputLayoutInputmodelChoice::CPeninputLayoutInputmodelChoice(CFepUiLayout* aUiLayout, 
            TInt aControlId,TPluginInputMode aInputMode) 
: CPeninputCommonChoiceList( aUiLayout, aControlId, KAknsIIDNone, KAknsIIDNone, EAutoFitLength )
    { 
    iConstructed = EFalse;
    iUiLayout = aUiLayout;
    iCurPluginInputMode = aInputMode;
    iInputLanguage = ELangTest;
    iSecretEditor = EFalse;       
    }

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CPeninputLayoutInputmodelChoice* CPeninputLayoutInputmodelChoice::NewL(
    CFepUiLayout* aUiLayout,
    TInt aControlId, TPluginInputMode aInputMode)
    {
    CPeninputLayoutInputmodelChoice *self = new (ELeave) 
        CPeninputLayoutInputmodelChoice(aUiLayout, aControlId, aInputMode);
    
    CleanupStack::PushL(self);
    self->CustomConstructL();
    CleanupStack::Pop();

    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CPeninputLayoutInputmodelChoice::~CPeninputLayoutInputmodelChoice()
    {
    iInputModeItems.Close();
    delete iAknFepRepository;
    }

// ---------------------------------------------------------------------------
// Pop up touch input switch list
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutInputmodelChoice::PopUpSwitchListL( const TRect& aRtControl )
    {
    ResetListItemL();

    if (ItemsCount() > 0 && ItemByIndex(0))
        {
        Display(aRtControl);    
        }
    }

// ---------------------------------------------------------------------------
// Initialize internal control
// ---------------------------------------------------------------------------
//
void CPeninputLayoutInputmodelChoice::DoInitializeControlL()
    {
    if (iConstructed)
        {
        return;
        }
    iAknFepRepository = CRepository::NewL(KCRUidAknFep);
        
    SizeChanged();
    
    RFs fsSession;

    User::LeaveIfError(fsSession.Connect());
    CleanupClosePushL(fsSession);

    RResourceFile rsFile;
    
    TFileName resourceName(KAknUtilsResourceFileName);
    CCoeEnv* coeEnv = CCoeEnv::Static();
    BaflUtils::NearestLanguageFile(fsSession, resourceName);
    
    rsFile.OpenL(fsSession, resourceName);
    CleanupClosePushL(rsFile);
    
    // Create choice list
    TResourceReader reader;

    HBufC8* rBuffer = rsFile.AllocReadLC(PENINPUTMODE_SWITCH_CHOICELIST & KCCMask); 
    reader.SetBuffer(rBuffer);

    TPtrC bmpFileName = reader.ReadTPtrC();
    TInt32 imgMajorSkinId = reader.ReadInt32();

    if (CreateItemIconL(reader, &iPopupWndBgImg, bmpFileName, imgMajorSkinId))
        {
       SetBackgroundBitmapL(iPopupWndBgImg);
        }

    if (CreateItemIconL(reader, &iPopupWndFocusImg, bmpFileName, imgMajorSkinId))
        {
        SetFocusBmp(iPopupWndFocusImg);
        }
    	
    CleanupStack::PopAndDestroy(1); 
    
    // read name to list
    
    CreateChoiceItemL( reader, rsFile );
/*    CreateChoiceItemL(iInputModeItem[EPenInputHwr], EPluginInputModeHwr, 
                      R_PENINPUTMODE_NAMES, reader, rsFile);
                      
    CreateChoiceItemL(iInputModeItem[EPenInputVkb], EPluginInputModeVkb, 
                      R_PENINPUTMODE_VKB, reader, rsFile);
                      
    CreateChoiceItemL(iInputModeItem[EPenInputItut], EPluginInputModeItut, 
                      R_PENINPUTMODE_FINGERITUT, reader, rsFile);
                      
    CreateChoiceItemL(iInputModeItem[EPenInputFSc], EPluginInputModeFSc,
                      R_PENINPUTMODE_FULLHWR, reader, rsFile);
    
*/
    CleanupStack::PopAndDestroy(2);
    
    iConstructed = ETrue;  
    }

// ---------------------------------------------------------------------------
// Application size Changed
// ---------------------------------------------------------------------------
//
void CPeninputLayoutInputmodelChoice::SizeChanged()    
    {
    /*
    if ( Hiden() )
        {
        iChoiceList->CloseWindow();
        }
    */    
    TRect mainPaneRect = MainPaneRect();
        
    TAknWindowLineLayout choiceWnd = 
        AknLayoutScalable_Avkon::popup_choice_list_window(1).LayoutLine();
    
    TAknLayoutRect choiceRect;
    choiceRect.LayoutRect(mainPaneRect, choiceWnd);

   iItemRect = TRect(
        0, 0, choiceRect.Rect().Width(),
        AknLayoutScalable_Avkon::list_single_choice_list_pane(0).LayoutLine().iH);

    TAknTextLineLayout choiceTextLayout = 
        AknLayoutScalable_Avkon::list_single_choice_list_pane_t1(1).LayoutLine();

    const CFont* font = AknLayoutUtils::FontFromId(choiceTextLayout.iFont, NULL);
 
    SetFont(font);
    SetItemRect(iItemRect, iItemRect);

    const TInt leftMargin = choiceTextLayout.il;
   
    SetMarginAndBaseline(leftMargin,font->HeightInPixels());
    SetFontColor(AKN_LAF_COLOR_STATIC(choiceTextLayout.iC));

    if (iPopupWndBgImg)
    	{
    	AknIconUtils::SetSize(iPopupWndBgImg, 
    	                      TSize(iItemRect.Width(), iItemRect.Height() * 3), 
    	                      EAspectRatioNotPreserved);
    	}
    
    if (iPopupWndFocusImg)
    	{
    	AknIconUtils::SetSize(iPopupWndFocusImg, 
    	                      TSize(iItemRect.Width(), iItemRect.Height() * 3), 
    	                      EAspectRatioNotPreserved);
    	}

    iPopupItemSize = iItemRect.Size();    
    }    
    
// ---------------------------------------------------------------------------
// Symbian second-phase constructor
// ---------------------------------------------------------------------------
//
void CPeninputLayoutInputmodelChoice::CustomConstructL( )
    {
    //CFepLayoutChoiceList::NewL(iUiLayout, aControlId);
    // call base class construction
    this->ConstructL();
    
    DoInitializeControlL();
    }

// ---------------------------------------------------------------------------
// Insert choice item to input mode switch list
// ---------------------------------------------------------------------------
//
void CPeninputLayoutInputmodelChoice::CreateChoiceItemL( TResourceReader &aReader, 
                                                         RResourceFile& aRsFile )
    {
    HBufC8* rBuffer = aRsFile.AllocReadLC( R_PENINPUTMODE_NAMES & KCCMask );
    aReader.SetBuffer(rBuffer);
    
    TInt index;
    TInt number;
    number = aReader.ReadInt16();
    
    CFepLayoutChoiceList::SItem item;
    for (index = 0; index < number ; index++)
        {
        item.iCommand = aReader.ReadInt32();
        //TPtrC ptr = aReader.ReadTPtrC();
        HBufC* buf = aReader.ReadHBufCL();
        item.iText.Copy( buf->Des() );
        delete buf;
        iInputModeItems.AppendL( item );
        }

    CleanupStack::PopAndDestroy(1);
    }

// ---------------------------------------------------------------------------
// Create choice item icon
// ---------------------------------------------------------------------------
//
TBool CPeninputLayoutInputmodelChoice::CreateItemIconL(TResourceReader &aReader, 
    CFbsBitmap** aImg, TPtrC& aBmpFileName, TInt32 aMajorSkinId)
    {
    MAknsSkinInstance* skinInstance = UiLayout()->SkinInstance();
    
    const TInt bmpId = aReader.ReadInt16();
    const TInt bmpMaskId = aReader.ReadInt16();
    const TInt bmpSkinId = aReader.ReadInt16();

    TAknsItemID id;
    id.Set(TInt(aMajorSkinId), bmpSkinId);
    
    if (bmpId != KInvalidImg)
    	{
        AknsUtils::CreateIconL(skinInstance,
                               id,
                               *aImg,
                               aBmpFileName,
                               bmpId);
                                
        AknIconUtils::SetSize(*aImg, iPopupItemSize, EAspectRatioNotPreserved); 
    	
    	return ETrue;
    	}
    	
    return EFalse;
    }    

    
// ---------------------------------------------------------------------------
// Get main pane rect
// ---------------------------------------------------------------------------
//
TRect CPeninputLayoutInputmodelChoice::MainPaneRect()
    {
    TAknWindowLineLayout appWnd; 
    TAknWindowLineLayout mainPane;
    TAknLayoutRect mainPaneLayout;
             
    appWnd = AknLayoutScalable_Avkon::application_window( 0 ).LayoutLine();

    TInt id = appWnd.iW > appWnd.iH ? 4 : 1;
    
    mainPane = AknLayoutScalable_Avkon::main_pane(id).LayoutLine();
    mainPaneLayout.LayoutRect(appWnd.Rect(), mainPane);

    return mainPaneLayout.Rect();
    }


// ---------------------------------------------------------------------------
// Reset choice list item
// ---------------------------------------------------------------------------
//
void CPeninputLayoutInputmodelChoice::ResetListItemL()
    {
    TInt layouts = iUiLayout->GetEnabledPluginMode();
        
    ClearItemsL();
    
    for ( int i = 0; i < iInputModeItems.Count(); i++ )
        {
        if ( layouts & iInputModeItems[i].iCommand  )
            {
            InsertListItemL( iInputModeItems[i], iInputModeItems[i].iCommand );
            }
        }
       
    SetListColumnNumWithLafL( ItemsCount() );
    }

// ---------------------------------------------------------------------------
// Insert list item
// ---------------------------------------------------------------------------
//
void CPeninputLayoutInputmodelChoice::InsertListItemL( const CFepLayoutChoiceList::SItem& item,
     TInt aInputMode)
    {
    TInt value = 0;
    if (iAknFepRepository)
        {
        iAknFepRepository->Get(KAknFepInputTxtLang, value);
        
        iInputLanguage = TLanguage(value);
        }

    if (iSecretEditor && (aInputMode == EPluginInputModeHwr ||
        aInputMode == EPluginInputModeFSc || aInputMode == EPluginInputModeFingerHwr))
        {
        return;
        }

    // Added for <Single Click>. Do not insert current input mode to the list
    if (iUiLayout && (iUiLayout->PenInputType() == aInputMode))
    	{
    	return;
    	}
    
    // if current input mode is in choice list, add item to it
    // (!ItemByCommand(aInputMode))
    if(FindCommand(aInputMode) < 0)
        {
        AddItemL( item );
        }
    }

// ---------------------------------------------------------------------------
// Set current editor is secure
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutInputmodelChoice::SetSecretEditor(TBool aSecret)
    {
    iSecretEditor = aSecret;
    }    


// ---------------------------------------------------------------------------
// Get switch list rect
// ---------------------------------------------------------------------------
//
EXPORT_C TRect CPeninputLayoutInputmodelChoice::SwitchListRect()
    {
    return TRect(0,0,iItemRect.Width(), iItemRect.Height() * ItemsCount());
    }
    
// End Of File
