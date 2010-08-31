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
* Description:  Implementation for gspeninputimple
*
*/


#include <eikenv.h>
#include <eikappui.h>   
#include <aknradiobuttonsettingpage.h>
#include <AknColourSelectionGrid.h> 
#include <gspeninputpluginrsc.rsg> 
#include <StringLoader.h>
#include <f32file.h> 
#include <bautils.h>
#include <avkon.mbg>
#include <aknlistquerydialog.h>
#include <aknconsts.h>
#include <eikspane.h> 
#include <akntitle.h> 
#include <akntoolbar.h>
#include <eikapp.h> 
#include <AknNullService.h>

#ifdef RD_UI_TRANSITION_EFFECTS_PHASE2
#include <gfxtranseffect/gfxtranseffect.h>
#include <akntransitionutils.h>
#include <avkondomainpskeys.h>
#include <e32property.h>

#endif

#include "gspeninputplugin.h"
#include "gspeninputimple.h"
#include "gspeninputmodel.h"
#include "gssettingdialog.h"

#define iAvkonAppUi ((CAknAppUi*)CEikonEnv::Static()->EikAppUi())

const TInt KTitlePaneImage = 0;
const TInt KTitlePaneText = 1;

const TUid KUidPenInputSettingApp = { 0x2001959B };

#define KMaxCmdLen 16

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------
// Constructor 
// ---------------------------------------------------------
//
CGSPenInputImple* CGSPenInputImple::NewL()
    {
    CGSPenInputImple* self = new (ELeave) CGSPenInputImple();

    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }
 
// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
CGSPenInputImple::~CGSPenInputImple()
    {
    iResourceLoader.Close();
    delete iModel;
    delete iWritingSpeedItems;
    delete iPenTrailWidthItems;
    delete iInputModeItems;
    delete iInputMethodForFindItems;
    delete iGuideLineItems;    
    delete iRecognitionWithDictionary;
    delete iDialog;
    delete iTitleTextBak;
    delete iChineseFindMethodItems;
    delete iNullService;
    }
        
// ---------------------------------------------------------
// Get model instance        
// ---------------------------------------------------------
//
CGSPenInputModel* CGSPenInputImple::Model()
    {
    return iModel;  
    }
    
// ---------------------------------------------------------
// Current writing speed name 
// ---------------------------------------------------------
//
void CGSPenInputImple::WritingSpeed(TDes& aWritingSpeed)
    {
    const TInt writingSpeed = iModel->WritingSpeed();
    const TInt count = iWritingSpeedItems->Count();

    for (TInt index = 0; index < count; ++index)
        {
        if (index == writingSpeed)
            {
            aWritingSpeed = (*iWritingSpeedItems)[index];
            break;
            }
        }

    if (!aWritingSpeed.Length() && 
        iWritingSpeedItems->Count() > 0)
        {
        aWritingSpeed = (*iWritingSpeedItems)[0];
        }
    }
    
// ---------------------------------------------------------
// Current trail width name 
// ---------------------------------------------------------
//
void CGSPenInputImple::TrailWidth(TDes& aTrailWidth)
    {
    const TInt trailwidth = iModel->TrailWidth();
    const TInt count = iPenTrailWidthItems->Count();

    for (TInt index = 0; index < count; ++index)
        {
        if (index == trailwidth)
            {
            aTrailWidth = (*iPenTrailWidthItems)[index];
            break;
            }
        }

    if (!aTrailWidth.Length() && iPenTrailWidthItems->Count() > 0)
        {
        aTrailWidth = (*iPenTrailWidthItems)[0];
        }
    }
  
// ---------------------------------------------------------
// Current guide line name 
// ---------------------------------------------------------
//
void CGSPenInputImple::GuideLine(TDes& aGuideLine)
    {
    const TInt guideLine = iModel->GuideLine();
    
    aGuideLine = (*iGuideLineItems)[guideLine == 0 ? 1:0];
    }
    
// ---------------------------------------------------------
// Current input method for find name 
// ---------------------------------------------------------
//
void CGSPenInputImple::InputMethodForFind(TDes& aInputMethodForFind)
    {
    const TInt inputmethodforfind = iModel->InputMethodForFind();
    
    aInputMethodForFind = (*iInputMethodForFindItems)[inputmethodforfind == 0 ? 0:1];
    }

// ---------------------------------------------------------
// Current chinese find method item text
// ---------------------------------------------------------
//
void CGSPenInputImple::GetCurrentChineseFineMethodItem
                                ( TDes& aChineseFindMethod )
    {   
    const TInt chineseFindMethod = iModel->ChineseFindMethod(); 
    if ( iChineseFindMethodItems 
         && chineseFindMethod < iChineseFindMethodItems->Count() )
        {        
        aChineseFindMethod 
          = (*iChineseFindMethodItems)[chineseFindMethod];
        }
    else
        {
        aChineseFindMethod = KNullDesC;
        }
    }

// ---------------------------------------------------------
// Load correct items for chinese find methods
// by different chinese languages
// ---------------------------------------------------------
//
void CGSPenInputImple::LoadChineseFindMethodItemsL()
    {
    TInt resourceID = iModel->ChineseFindMethodResourceID();
    delete iChineseFindMethodItems;
    iChineseFindMethodItems = NULL;    
    if ( resourceID != KErrNotFound )
        {        
        iChineseFindMethodItems 
            = CEikonEnv::Static()->ReadDesC16ArrayResourceL
                ( resourceID );        
        }
    }


// ---------------------------------------------------------
// Load correct items for default on screen vbk items
// ---------------------------------------------------------
//     
 void CGSPenInputImple::LoadDefaultOnScreenVkbItemsL()
    {
    TInt resourceID = iModel->InputModeResourceID();
    // Reload input mode items
    delete iInputModeItems;
    iInputModeItems = NULL;    
    if ( resourceID != KErrNotFound )
        {
        iInputModeItems 
            = CEikonEnv::Static()->ReadDesC16ArrayResourceL
                ( resourceID );
        // Reload current input mode
        iModel->InputMode();
        }
    }

// ---------------------------------------------------------
// Recognition with dictionary
// ---------------------------------------------------------
//
void CGSPenInputImple::RecognitionWithDictionary(TDes& aRecognitionWithDictionary)
    {
    const TInt recognitionWithDictionary = iModel->RecognitionWithDictionary();
    
    aRecognitionWithDictionary = (*iRecognitionWithDictionary)[recognitionWithDictionary == 0 ? 1:0];
    }


// ---------------------------------------------------------
// Current input mode name
// ---------------------------------------------------------
//
void CGSPenInputImple::InputModeL(TDes& aInputMode)
    {
    if (!iModel->ChineseLanguage())
        {
        return;
        }

    delete iInputModeItems;
    iInputModeItems = NULL;
    
    if (iModel->InputModeResourceID() != -1)
        {
        iInputModeItems = CCoeEnv::Static()->ReadDesC16ArrayResourceL(
                                   iModel->InputModeResourceID());
        iModel->ResetInputModeArray(iInputModeItems);	
        }
    else
        {
        aInputMode = KNullDesC;
        return;
        }
        
    // Update iInputMode from CenRep
    iModel->InputMode();
    const TInt index = iModel->InputModeInSettingPage();
    
    if ( iInputModeItems &&  index < iInputModeItems->Count() )
        {
        aInputMode = (*iInputModeItems)[index];
        }
    else
        {
        aInputMode = KNullDesC;
        }    
    }
    
// ---------------------------------------------------------
// Dispaly input pen main view
// ---------------------------------------------------------
//
void CGSPenInputImple::ShowMainViewL()
    {
    TUid appUid = iAvkonAppUi->Application()->AppDllUid();
        // if open from trui, use dialog
        // otherwise, use peninput setting application
        // Launch peninputsetting application as embedded.
        if ( iNullService )
            {
            delete iNullService;
            iNullService = NULL;
            }
        iNullService = CAknNullService::NewL
                      ( KUidPenInputSettingApp, NULL );
    }

// ---------------------------------------------------------
// Display "Pen input language" setting page
// ---------------------------------------------------------
//
TBool CGSPenInputImple::ShowInputLanguageSettingPageL()
	{
	TBool  ret(EFalse);
    CDesCArrayFlat* languageItems = new(ELeave)CDesCArrayFlat(10);
    CleanupStack::PushL(languageItems);
    
    iModel->GetLanguagesFromPtiL(EFalse, languageItems);

    TInt currentItem = iModel->InputLanguageIndex();

    CAknRadioButtonSettingPage* dlg = new (ELeave) CAknRadioButtonSettingPage(
                                      R_GS_INPUTLANGUAGE_TEXT_SETTING_PAGE, 
                                      currentItem,
                                      languageItems);

    CleanupStack::PushL(dlg);
    
    if (dlg->ExecuteLD(CAknSettingPage::EUpdateWhenChanged))
        {
        if (currentItem != iModel->InputLanguageIndex())
            {
            iModel->SetInputLanguageIndex(currentItem);
            iModel->SetInputLanguageL();
            if (iModel->ChineseLanguage())
                {
                iModel->ResetFepDefaultInputMode();
                }
            ret = ETrue;
            }        
        }

    CleanupStack::Pop(dlg);
    CleanupStack::PopAndDestroy(languageItems);

    return ret; 	
	}

// ---------------------------------------------------------
// Display pop up list setting page
// ---------------------------------------------------------
//
TBool CGSPenInputImple::ShowInputLanguagePageL()
    {
    TBool ret(EFalse);
    CDesCArrayFlat* languagesList = new (ELeave) CDesCArrayFlat(10);
    CleanupStack::PushL(languagesList);
    
    iModel->GetPopupQuerylanguagesFromPtiL(languagesList);
    
    TInt currentItem = iModel->InputLanguageIndex();
    CAknListQueryDialog* dlg = new (ELeave) CAknListQueryDialog(&currentItem);
    
    dlg->PrepareLC(R_GS_LANGUAGES_LIST_QUERY);
    
    CAknListQueryControl* listQueryControl = 
        static_cast<CAknListQueryControl*> (dlg->Control(EListQueryControl));
        
#ifdef RD_UI_TRANSITION_EFFECTS_PHASE2
		// Stops transition
		GfxTransEffect::Deregister(dlg);
#endif        
    CTextListBoxModel* model = listQueryControl->Listbox()->Model();
    
    // Add icons to the listbox:
    CArrayPtr<CGulIcon>* icons = new( ELeave ) CArrayPtrFlat<CGulIcon>(10);
    CleanupStack::PushL(icons);
    
    LoadIconL(icons, EMbmAvkonQgn_prop_sub_current, 
               EMbmAvkonQgn_prop_sub_current_mask);

    listQueryControl->Listbox()->ItemDrawer()->FormattedCellData()
        ->SetIconArrayL(icons); // transfer to list query

    CleanupStack::Pop(icons);
    
    // Activate selected language:
    model->SetOwnershipType(ELbmOwnsItemArray);
    model->SetItemTextArray(languagesList); // transfer to list query
    listQueryControl->Listbox()->SetCurrentItemIndex(currentItem);
    // Needed because not using icon in cell.
    listQueryControl->Listbox()->ItemDrawer()->FormattedCellData()
        ->SetNotAlwaysDrawnSubCellL(0, ETrue);
    
    // Fire up the dialog
    TInt returnValue = dlg->RunLD();

    if(returnValue == EAknSoftkeyOk)
        {
        if(currentItem != iModel->InputLanguageIndex())
            {
            iModel->SetInputLanguageIndex(currentItem);
            iModel->SetInputLanguageL();
            if (iModel->ChineseLanguage())
                {
                iModel->ResetFepDefaultInputMode();
                }
            ret = ETrue;
            }        
        }

    CleanupStack::Pop(languagesList);
        
    return ret;
    }

// ---------------------------------------------------------
// Display "inputmode" setting page
// ---------------------------------------------------------
//
TBool CGSPenInputImple::ShowInputModePageL()
    {
    if (!iModel->ChineseLanguage())
        {
        return EFalse;
        }
        
    delete iInputModeItems;
    iInputModeItems = NULL;
    
    if (iModel->InputModeResourceID() != -1)
        {
        iInputModeItems = CCoeEnv::Static()->ReadDesC16ArrayResourceL(
                                   iModel->InputModeResourceID());
        iModel->ResetInputModeArray(iInputModeItems);	
        }
    else
        {
        return EFalse;
        }
           
    TBool ret = EFalse;
    TInt currentItem = iModel->InputModeInSettingPage();    
    if ( currentItem < iInputModeItems->Count() )
        {
        CAknRadioButtonSettingPage* dlg = new (ELeave) CAknRadioButtonSettingPage(
                                          R_GS_VKB_DEFAULTCHINESE_OSK_TEXT_SETTING_PAGE, 
                                          currentItem, 
                                          iInputModeItems);

        CleanupStack::PushL(dlg);                
        if (dlg->ExecuteLD(CAknSettingPage::EUpdateWhenChanged))
            {
            if(currentItem != iModel->InputModeInSettingPage())
                {
                iModel->SetInputModeBySettingPage(currentItem);
                ret = ETrue;
                }
            }
     
        CleanupStack::Pop(dlg);        
        }    
    return ret;     
    }
    
// ---------------------------------------------------------
// Display "Writing speed" setting page
// ---------------------------------------------------------
//
TBool CGSPenInputImple::ShowWritingSpeedPageL()
    {
    TInt currentItem = iModel->WritingSpeed();

    CAknRadioButtonSettingPage* dlg = new (ELeave) CAknRadioButtonSettingPage(
                                      R_GS_WRITINGSPEED_TEXT_SETTING_PAGE, 
                                      currentItem, 
                                      iWritingSpeedItems);

    CleanupStack::PushL(dlg);
    
    TBool ret = EFalse;
    if (dlg->ExecuteLD(CAknSettingPage::EUpdateWhenChanged))
        {
        if(currentItem != iModel->WritingSpeed())
            {
            iModel->SetWritingSpeed(currentItem);
            ret = ETrue;
            }
        }
 
    CleanupStack::Pop(dlg);  
    
    return ret; 
    }

// ---------------------------------------------------------
// Display "Guide Line" setting page
// ---------------------------------------------------------
//
TBool CGSPenInputImple::ShowGuideLinePageL()
    {
    TInt currentItem = iModel->GuideLine();
    
    if (currentItem == 1)
        {
        currentItem = 0;
        }
    else
        {
        currentItem = 1;
        }

    CAknRadioButtonSettingPage* dlg = new (ELeave) CAknRadioButtonSettingPage(
                                      R_GS_GUIDLINE_TEXT_SETTING_PAGE, 
                                      currentItem, 
                                      iGuideLineItems);

    CleanupStack::PushL(dlg);
    
    TBool ret = EFalse;
    if (dlg->ExecuteLD(CAknSettingPage::EUpdateWhenChanged))
        {
        if(currentItem == iModel->GuideLine())
            {
            iModel->SetGuideLine(currentItem == 0? 1:0);
            ret = ETrue;
            }
        }
 
    CleanupStack::Pop(dlg);  
    
    return ret; 
    }

// ---------------------------------------------------------
// Display "Recognition with dictionary" setting page
// ---------------------------------------------------------
//
TBool CGSPenInputImple::ShowRecognitionWithDictionaryL()
    {
    TInt currentItem = iModel->RecognitionWithDictionary();
    
    if (currentItem == 1)
        {
        currentItem = 0;
        }
    else
        {
        currentItem = 1;
        }

    CAknRadioButtonSettingPage* dlg = new (ELeave) CAknRadioButtonSettingPage(
            R_GS_RECOGNITIONWITHDICTIONARY_TEXT_SETTING_PAGE, 
                                      currentItem, 
                                      iRecognitionWithDictionary);

    CleanupStack::PushL(dlg);
    
    TBool ret = EFalse;
    if (dlg->ExecuteLD(CAknSettingPage::EUpdateWhenChanged))
        {
        if(currentItem == iModel->RecognitionWithDictionary())
            {
            iModel->SetRecognitionWithDictionary(currentItem == 0? 1:0);
            ret = ETrue;
            }
        }
 
    CleanupStack::Pop(dlg);  
    
    return ret; 
    }
   
// ---------------------------------------------------------
// Display "Pen trail width" setting page
// ---------------------------------------------------------
//
TBool CGSPenInputImple::ShowTrailWidthPageL()
    {
    TInt currentItem = iModel->TrailWidth();

    CAknRadioButtonSettingPage* dlg = new (ELeave) CAknRadioButtonSettingPage(
                                      R_GS_TRAILWIDTH_TEXT_SETTING_PAGE, 
                                      currentItem, 
                                      iPenTrailWidthItems);

    CleanupStack::PushL(dlg);
    
    TBool ret = EFalse;
    if (dlg->ExecuteLD(CAknSettingPage::EUpdateWhenChanged))
        {
        if(currentItem != iModel->TrailWidth())
            {
            iModel->SetTrailWidth(currentItem);
            ret = ETrue;
            }     
        }
    
    CleanupStack::Pop(dlg);     
    
    return ret;
    }

// ---------------------------------------------------------
// Display "Pen trail colour" setting page
// ---------------------------------------------------------
//
TBool CGSPenInputImple::ShowTrailColourPageL()
    {
    CArrayFixFlat<TRgb>* colours = new (ELeave) CArrayFixFlat<TRgb>(8);
    CleanupStack::PushL(colours);

    colours->AppendL(KRgbBlack);
    colours->AppendL(KRgbDarkGray);
    colours->AppendL(KRgbDarkRed);
    colours->AppendL(KRgbDarkGreen);
    colours->AppendL(KRgbDarkYellow);
    colours->AppendL(KRgbDarkBlue);
    colours->AppendL(KRgbDarkMagenta);
    colours->AppendL(KRgbDarkCyan);
    colours->AppendL(KRgbRed);
    colours->AppendL(KRgbGreen);
    colours->AppendL(KRgbYellow);
    colours->AppendL(KRgbBlue);
    colours->AppendL(KRgbMagenta);
    colours->AppendL(KRgbCyan);
    colours->AppendL(KRgbGray);
    colours->AppendL(KRgbWhite);

    TBool noneChosen = EFalse; 
    TRgb colourChosen = TRgb(iModel->TrailColour());
                            
    CAknColourSelectionGrid* dlg = CAknColourSelectionGrid::NewL(colours, 
                                                                 EFalse, 
                                                                 noneChosen, 
                                                                 colourChosen);
    CleanupStack::PushL(dlg);
    
    TBool ret = EFalse;
    if (dlg->ExecuteLD())
        {
        iModel->SetTrailColour(colourChosen.Value());
        ret = ETrue;
        }
    
    CleanupStack::Pop(dlg);      
    CleanupStack::PopAndDestroy(colours);    
    
    return ret;
    }

// ---------------------------------------------------------
// Display "Input method for find" setting page
// ---------------------------------------------------------
//
TBool CGSPenInputImple::ShowInputMethodForFindPageL()
    {
    TInt currentItem = iModel->InputMethodForFind();

    CAknRadioButtonSettingPage* dlg = new (ELeave) CAknRadioButtonSettingPage(
                                      R_GS_INPUTMETHODFORFIND_TEXT_SETTING_PAGE, 
                                      currentItem, 
                                      iInputMethodForFindItems);

    CleanupStack::PushL(dlg);
    TBool ret = EFalse;
    
    if (dlg->ExecuteLD(CAknSettingPage::EUpdateWhenChanged))
        {
        if(currentItem != iModel->InputMethodForFind())
            {
            iModel->SetInputMethodForFind(currentItem);
            ret = ETrue;
            }
        }
 
    CleanupStack::Pop(dlg);
    return ret;
    }

// ---------------------------------------------------------
// Display "Chinese Find Method" setting page
// ---------------------------------------------------------
//
TBool CGSPenInputImple::ShowChineseFindMethodPageL()
    {
    TInt currentItem = iModel->ChineseFindMethod();
    TBool ret = EFalse;
    if ( !iChineseFindMethodItems )
        {
        // In that case, don't open setting page
        return EFalse;
        }
    CAknRadioButtonSettingPage* dlg 
                 = new (ELeave) CAknRadioButtonSettingPage(
                   R_GS_CHINESEFINDMETHOD_TEXT_SETTING_PAGE,
                   currentItem,
                   iChineseFindMethodItems);
    CleanupStack::PushL(dlg);
    
    if (dlg->ExecuteLD(CAknSettingPage::EUpdateWhenChanged))
        {
        if(currentItem != iModel->ChineseFindMethod())
            {
            iModel->SetChineseFindMethod(currentItem);
            ret = ETrue;
            }
        }
    CleanupStack::Pop(dlg);       
    return ret;
    }

// ---------------------------------------------------------
// Start touch screen calibration
// ---------------------------------------------------------
//
void CGSPenInputImple::StartTouchScreenCalibL()
    {
    _LIT( KTouchScreenCalibFileName, "z:TouchScreenCalib.exe" );
    TInt err = KErrNone;
 
    TParse parse;
    parse.Set( KTouchScreenCalibFileName, 
                   &KDC_PROGRAMS_DIR, NULL );
    TFileName fileName( parse.FullName() );
 
    RProcess process;
    err = process.Create( fileName, KNullDesC );
    User::LeaveIfError(err);
 
    CApaCommandLine* commandLine = CApaCommandLine::NewLC();
    commandLine->SetDocumentNameL( KNullDesC );
    commandLine->SetExecutableNameL( fileName );
    commandLine->SetProcessEnvironmentL( process );
    CleanupStack::PopAndDestroy( commandLine );
 
    process.Resume();
    process.Close();
    }
    
// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CGSPenInputImple::CGSPenInputImple()
    : iResourceLoader(*(CCoeEnv::Static()))
    {
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
void CGSPenInputImple::ConstructL()
    {
    iModel = CGSPenInputModel::NewL( this );
    iModel->SetOwner(NULL);
    
    iModel->InputLanguage();
    iModel->InputMode();
    iModel->TrailColour();
    iModel->TrailWidth();
    iModel->InputMethodForFind();
    iModel->GuideLine();
    iModel->RecognitionWithDictionary();
    
    OpenResourceFileL(KGSInputPenPluginResourceFileName);
    iWritingSpeedItems = CCoeEnv::Static()->ReadDesC16ArrayResourceL(
                                            R_GS_WRITINGSPEED_SETTING_PAGE_LBX);
    iPenTrailWidthItems = CCoeEnv::Static()->ReadDesC16ArrayResourceL(
                                             R_GS_PENTRAIL_SETTING_PAGE_LBX);

    iGuideLineItems = CCoeEnv::Static()->ReadDesC16ArrayResourceL(
                                             R_GS_GUIDLINE_SETTING_PAGE_LBX); 

    iInputMethodForFindItems = CCoeEnv::Static()->ReadDesC16ArrayResourceL(
                                             R_GS_INPUTMETHODFORFIND_SETTING_PAGE_LBX);                                             
    iRecognitionWithDictionary = CCoeEnv::Static()->ReadDesC16ArrayResourceL(
            R_GS_RECOGNIZEWITHDICTIONARY_SETTING_PAGE_LBX);     
    
    if (iModel->InputModeResourceID() != -1)
        {
        iInputModeItems = CCoeEnv::Static()->ReadDesC16ArrayResourceL(
                                   iModel->InputModeResourceID());
        }                                             
    }

void CGSPenInputImple::HandleNotifyInt(TUint32 aId, TInt aNewValue)
    {
    if ( iDialog )
        {
        iDialog->HandleNotifyInt( aId, aNewValue );
        }
    }

void CGSPenInputImple::HandleNotifyInt(TUid aRepositoryUid, TUint32 aId, TInt aNewValue)
    {
    if ( iDialog )
        {
        iDialog->HandleNotifyInt( aRepositoryUid, aId, aNewValue );
        }    
    }
    
// ---------------------------------------------------------
// Opens the nearest localized resourcefile using aResourceLoader   
// ---------------------------------------------------------
//    
void CGSPenInputImple::OpenResourceFileL(const TDesC& aResourceFileName)
    {
    RFs fsSession;
    User::LeaveIfError(fsSession.Connect()); 
    CleanupClosePushL(fsSession);
    
    // Find the resource file:
    TParse parse;
    parse.Set(aResourceFileName, &KDC_RESOURCE_FILES_DIR, NULL);
    TFileName fileName(parse.FullName());

    // Get language of resource file:
    BaflUtils::NearestLanguageFile(fsSession, fileName);

    // Open resource file:
    iResourceLoader.OpenL(fileName);

    // If leave occurs before this, close is called automatically when the
    // thread exits.
    fsSession.Close();
    
    CleanupStack::PopAndDestroy(); //fsSession
    }    

// ---------------------------------------------------------
// Load language list popup query dialog icons from resource
// ---------------------------------------------------------
//    
void CGSPenInputImple::LoadIconL(CArrayPtr<CGulIcon>* aIcons, 
                               TInt aBitmapId, 
                               TInt aMaskId)
    {
    CGulIcon* icon = CGulIcon::NewLC();
    CFbsBitmap* bitmap = 0;
    CFbsBitmap* mask = 0;
    
    AknIconUtils::CreateIconL(bitmap, mask, KAvkonBitmapFile, aBitmapId, aMaskId);
    icon->SetBitmap(bitmap);
    icon->SetMask(mask);
    aIcons->AppendL(icon);
    
    CleanupStack::Pop(icon);
    }

void CGSPenInputImple::PrepareScreenFurnitureL( TBool& aToolbarShow )
    {
    // Get application UI StatusPane
    CEikStatusPane* statusPane = 
        static_cast<CAknAppUi*>(CEikonEnv::Static()->EikAppUi())->StatusPane();
    // Get application UI TitlePane
    CAknTitlePane* titlePane = 
        static_cast<CAknTitlePane*>(statusPane->ControlL(TUid::Uid(EEikStatusPaneUidTitle)));

    CCoeControl* akncontrol = static_cast<CCoeControl*>(titlePane);
    CCoeControl* imageControl = akncontrol->ComponentControl(KTitlePaneImage);
    CCoeControl* textControl = akncontrol->ComponentControl(KTitlePaneText);
    
    if (imageControl &&
        textControl &&
        imageControl != textControl)
        imageControl->MakeVisible(EFalse);

    // Backup title pane text    
    if ( iTitleTextBak )
        {
        delete iTitleTextBak;
        iTitleTextBak = NULL;
        }    
    if ( titlePane->Text() && titlePane->Text()->Length() > 0 )
        {
        iTitleTextBak = HBufC::NewL( titlePane->Text()->Length() );
        TPtr titlePtr = iTitleTextBak->Des();
        titlePtr.Copy( *( titlePane->Text() ) );
        }
    
    // Get GSPlugin title
    HBufC* result = StringLoader::LoadL(R_GS_INPUTPEN_PLUGIN_CAPTION);
    titlePane->SetText( result );
    
    
    //Hide navibar
    CAknNavigationControlContainer* naviContainer = 
                static_cast<CAknNavigationControlContainer*>(
                                 statusPane->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) ) );
    if( naviContainer )
        {
        naviContainer->PushDefaultL();
        }

    //Hide Toolbar
    CAknToolbar* toolBar = iAvkonAppUi->CurrentFixedToolbar();
    if( toolBar && toolBar->IsShown() )
        {
        aToolbarShow = ETrue;
        toolBar->SetToolbarVisibility( EFalse );
        }
        
    // Backup status pane
    isSpVisibleBak = statusPane->IsVisible();
    // Show status pane
    if ( !isSpVisibleBak )
        {
        statusPane->MakeVisible( ETrue );
        }
    }

void CGSPenInputImple::RestoreScreenFurnitureL(TBool aShowToolbar )
    {
    CEikStatusPane* statusPane = 
        static_cast<CAknAppUi*>(CEikonEnv::Static()->EikAppUi())->StatusPane();
    // Get application UI TitlePane
    CAknTitlePane* titlePane = 
        static_cast<CAknTitlePane*>(statusPane->ControlL(TUid::Uid(EEikStatusPaneUidTitle)));
        
    // Reset the title
    if ( iTitleTextBak )
        {
        titlePane->SetTextL( *iTitleTextBak );
        delete iTitleTextBak;
        iTitleTextBak = NULL;
        }
    else
        {
        titlePane->SetTextL( KNullDesC );
        }
    
    CCoeControl* akncontrol = static_cast<CCoeControl*>(titlePane);
    CCoeControl* imageControl = akncontrol->ComponentControl(KTitlePaneImage);
    CCoeControl* textControl = akncontrol->ComponentControl(KTitlePaneText);
    
    if (imageControl &&
        textControl &&
        imageControl != textControl)
        imageControl->MakeVisible(ETrue);


    //Restore Toolbar
    CAknToolbar* toolBar = iAvkonAppUi->CurrentFixedToolbar();
    if( toolBar && aShowToolbar )
        {
        toolBar->SetToolbarVisibility( ETrue );
        }
    
    //Restore navibar
    CAknNavigationControlContainer* naviContainer = 
                static_cast<CAknNavigationControlContainer*>(
                                 statusPane->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) ) );
    if( naviContainer )
        {
        naviContainer->Pop();
        }
    }

// ---------------------------------------------------------
// Restore caller's status pane visibility.
// ---------------------------------------------------------
//
void CGSPenInputImple::RestoreStatusPaneVisibility()
    {
    CEikStatusPane* statusPane = 
        static_cast<CAknAppUi*>
        (CEikonEnv::Static()->EikAppUi())->StatusPane();
    
    // restore status pane
    if ( isSpVisibleBak != statusPane->IsVisible() )
        {
        statusPane->MakeVisible( isSpVisibleBak );
        }
    }

// End Of File
