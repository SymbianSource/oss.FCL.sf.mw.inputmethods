/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:           
*       Provides the CAknFepManager methods.
*
*/











#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <uikon/eikdefmacros.h>
#endif
#include "AknFepManager.h"
#include "AknFepManagerInterface.h" // MAknFepManagerInterface
#include "aknfepindicator.h"
#include "AknFepPanic.h"
#include "AknFepKeyCatcher.h"
#include "AknFepGlobalEnums.h"
#include "AknFepCaseManager.h"
#include "AknFepHashKeyManager.h"
#include "AknFepSharedDataInterface.h"
#include "AknFepInlineTextDecorator.h"
#include "AknFepUiManagerBase.h"
#include "AknFepUiManagerChinese.h"
#include "AknFepUiManagerWestern.h"

#include "AknFepPluginManager.h"
#include "AknFepUiIndicInputManager.h"
#include "AknFepUiIndicEnums.h"
#include <AknFepUIStylusPopupMenu.h>
#include "aknfepuispellinputpane.h"

#include <bautils.h>
#include <coeaui.h>                 //CCoeAppUi
#include <aknedsts.h>               //CAknEdwinState
#include <uikon.hrh>                //keys
#include <aknenv.h>                 //CAknEnv
#include <PtiEngine.h>              //CPtiEngine
#include <AknJapaneseReading.h>     //CAknJapaneseReading
#include <featmgr.h>                //FeatureManager
#include <e32base.h>                //CPeriodic
#include <PtiDefs.h>                //keys
#include <fepipext.h>               //MCoeFepSpecificExtensions

#include <aknsoundsystem.h>
#include <avkon.rsg>
#include <avkon.hrh>

#include <tagma.h>
#include <frmtview.h>
#include <aknedformaccessor.h>

#include <aknfep.rsg>
#include <avkon.mbg>

#include "aknconsts.h"
#include <aknfeppeninputenums.h>
#include <eikproc.h>                // User Dictionary
#include <eikapp.h>                 // User Dictionary

#include <StringLoader.h>

#include <centralrepository.h>
#include <AknFepInternalCRKeys.h>
#include <settingsinternalcrkeys.h>
#include <AknUtils.h>
#include <AknCustomCursorSupport.h>
#include <PtiKeyMappings.h>
#include <aknfeppeninputimeplugin.h>
#include <peninputcmd.h>

#include <AknSettingCache.h>
#include <peninputgsinterface.h>
#include <aknextendedinputcapabilities.h>
#include <AvkonInternalCRKeys.h>
#include <hlplch.h>
#include <aknfepdocumentnavigation.h>

#include <eikcolib.h>
#include <akntoolbar.h>

#include <eikfrlbd.h>
#include <eikccpu.h>
#include <aknappui.h>
#include <apgwgnam.h>
#ifdef RD_INTELLIGENT_TEXT_INPUT
#include "AknFepUiWordPopupContent.h"
#include "AknFepFnKeyManager.h"
#include <csxhelp/cp.hlp.hrh> 
#endif //RD_INTELLIGENT_TEXT_INPUT

#ifdef RD_TACTILE_FEEDBACK
#include <touchfeedback.h>
#endif //RD_TACTILE_FEEDBACK
#include "AknFepThaiSCTSelector.h"

//the follow 10 filse include for phrase creation
#include <bamdesca.h>
#include <eikspane.h> //CEikStatusPane 
#include <akntitle.h> //CAknTitlePane 
#include <akntoolbar.h> //CAknToolBar 
#include <aknappui.h>
#include "AknFep.hrh"
#include "aknfepuserdbdlg.h"
#include "aknfepzhuyinkeyhandler.h"
#include "AknFepZhuyinAnalyser.h"
#include "AknFepZhuyinAnalyserDb.h"

#include <aknnavi.h> 
#include <aknnavide.h> 

#include <eikedwin.h>
#include <aknCharMap.h>

#include <AknFepInternalPSKeys.h>
#include <e32property.h>
#include <AknDef.h>

const TInt KSelectItemSize(10);
_LIT(KAknFepResourceFileName, "z:\\resource\\fep\\aknfep.rsc");

#define text_to_field_mode_hash_key ';'
#define iAvkonAppUi ((CAknAppUi*)CEikonEnv::Static()->EikAppUi())
#define __SHIFT_KEY_LOOP

// EAknEditorNativeKeysNumberModeKeymap is used for example in MFNE editors.
#define EAknEditorNativeKeysNumberModeKeymap EAknEditorReservedInternalUseKeymap

// UID of user dictionary application
const TUid KUidUserDictApp = {0x101F8645};

const TText KAknFEPLineFeedSymbol = 0x21B2;
const TText KAknFEPMirroredLineFeedSymbol = 0x21B3;
const TText KAknFEPZeroKey = 0x30;

// inline text decoration potentially adds 2 directional markers for every "real" character. Buffer needs to be 3 x predictive engine bufsize
const TInt KMaxInlineEditSize( 3 * CAknFepManager::EMaximumFepWordLength );

// Maximum number of chunks to navigate throught attempting to get to the visual end of a selection.
// Since chunks are arranged only within a line, this should be more than the number of chunks likely in
// a single line of text
const TInt KMaxChunkWalkIterations(20);

// This command is is used for setting empty CBA.
const TInt KNullCommandId = 0;

_LIT_SECURE_ID(KPhoneSecureId, 0x100058b3);

#ifdef RD_HINDI_PHONETIC_INPUT

const TUint KQuestionMark = 0x003F;

#endif

// Ccpu key events. 
/*As part of build warnings commenting this perticular code it might require when touch is enabled.
const TKeyEvent KEditorCopyEvent = {3, 67, 161, 0};  // Ctrl + C
const TKeyEvent KEditorCutEvent  = {24, 88, 161, 0}; // Ctrl + X
*/

const TInt KTouchInputPreviewOn = 1;
const TInt KTouchInputPreviewOff = 0;

#ifdef  RD_SCALABLE_UI_V2
const TKeyEvent KAknCcpuCopyEvent = {EKeyF18, EEikCmdEditCopy, EModifierCtrl, 1};  // Copy event for AknCcpuSupport
const TKeyEvent KAknCcpuCutEvent  = {EKeyF18, EEikCmdEditCut, EModifierCtrl, 1};   // Cut event for AknCcpuSupport

const TInt KMaxMenuSize = 28;
const TInt KEditorMenuPermitedSend[KMaxMenuSize] = 
    {         
    EJapanFepCmdModeHiragana,
    EJapanFepCmdModeKatakana,
    EJapanFepCmdEditModeAlpha,
    EJapanFepCmdEditModeNumber,
    EJapanFepCmdModeFullWidth,
    EJapanFepCmdModeHalfWidth,
    EChinFepCmdModePinyin,
    EChinFepCmdModeZhuyin,
    EChinFepCmdModeStroke,
    EChinFepCmdModePinyinPhrase,
    EChinFepCmdModeZhuyinPhrase,
    EChinFepCmdModeStrokePhrase,    
    EAknCmdT9PredictiveT9Off,
    EAknCmdMultitapPredictiveT9On,
    EJapanFepCmdMultitapPredictiveT9On,
    EChinFepCmdModeZhuyinFind,
    EChinFepCmdModeStrokeFind,
    EAknCmdEditModeAlpha,
    EAknCmdEditModeLatinText,
    EChinFepCmdModeLatinUpper,
    EChinFepCmdModeLatinLower,
    EAknCmdEditModeNumber,
    EChinFepCmdModeNumber,
    EAknCmdEditModeArabicIndicNumber,
    EAknCmdEditModeEasternArabicIndicNumber,
    EAknCmdEditModeIndicNumber,
    EAknCmdEditModeKorean,
    EAknCmdEditModeEnglish
    };   
     
#endif // RD_SCALABLE_UI_V2 


// Number modes used by Arabic or Hindi.
enum TNonLatinNumberModes
    { 
    ENbrModeLatin,
    ENbrModeArabic = 1,
    ENbrModeIndic = 1
    };

// Number mode type
enum TNonLatinNumberModeType
    {
    ENbrModeTypeArabic,
    ENbrModeTypeIndic,
    ENbrModeTypeEasternArabic //for Urdu & Farsi languages
    };
    
// The passwd timeout is same as defined on CEikSecretEditor.
const TInt KSyncPasswdTimeout = 1000000;

#ifdef RD_SCALABLE_UI_V2
TUid AppUidFromWndGroupId(TInt aWndGrpId);
extern TUid GetCurAppUid();
extern TBool IsGlobalNotesApp(TUid& aUid);
TInt GetForegroundTaskAppWgId();
#endif
#ifdef RD_INTELLIGENT_TEXT_INPUT
const TInt KWordToFocusInCandidateList = 1;
#endif

LOCAL_C TInt RemapVietnameseAccentedCharacter(TUint16 aChr);

//
// Class TTextDirectionalInfo
//
/**
* The implementation inializes with well-defined "null" state
*/
CAknFepManager::TTextDirectionalInfo::TTextDirectionalInfo() : 
iDirectionalStatus(EUnknown), iBlockEnvironment(EUndefined)
    {}

void CAknFepManager::TTextDirectionalInfo::SetDirectionFromChunk( 
               const CTmTextLayout::TTmChunkDescription& aChunk )
    {
    if ( aChunk.iStart >= 0 ) // Condition for valid block. Does nothing otherwise
        iDirectionalStatus = aChunk.iRightToLeft ? ERightToLeft : ELeftToRight;
    }


void TFepLanguageTranslation::ReadFromResource(TResourceReader& aReader)
    {
    iName.Set(aReader.ReadTPtrC());
    iLanguageId = aReader.ReadInt16();
    iCommandId = aReader.ReadInt16();
    }


CAknFepManager::CAknFepManager(CCoeEnv& aConeEnvironment)
    :CCoeFep(aConeEnvironment),
    iAsyncResouceChanged(EActivePriorityDefault),
    iPreviousEditingState(EStateNone),
    iInputCapabilities(TCoeInputCapabilities::ENone),
    iModeBefore(ELatin),
    #ifdef RD_INTELLIGENT_TEXT_INPUT
    iKeyPressedDuringShift(EFalse),
#endif
    iCharWidth(EHalfWidthChar),
    iAsyncOwnSimulateKey(EActivePriorityDefault),
    iLongThai0Key(EFalse),
    iEditCharsPtr(0,0),
    iComposChrFlag(0),
    iIsUserdbdlgActive(0),
    iLongClearAfterCloseUI(EFalse),
    // phrase creation
    iPhraseCreation(0),
    iPinyinPhraseCreation(0),
    iEntryPhrase(0)
    

    {
    }

void CAknFepManager::ConstructL(const CCoeFepParameters& aFepParameters)
    {
    BaseConstructL(aFepParameters);
    iUiInterface = CAknFepUIInterface::NewL();
    iIndicator = iUiInterface->NewIndicatorL();
    iPreviousIndicator = CAknEnv::Static()->SwapEditingStateIndicator(iIndicator);

    // Set up resources
    TFileName resourceName(KAknFepResourceFileName);
    CCoeEnv* coeEnv = CCoeEnv::Static();
    BaflUtils::NearestLanguageFile(coeEnv->FsSession(), resourceName);
    iResId = coeEnv->AddResourceFileL(resourceName);

    // Save Japanese Feature support to a member variable
    iFeatureSupportedJapanese = FeatureManager::FeatureSupported(KFeatureIdJapanese);

    iUiLanguage = User::Language();
    
    // Shared Data
    iSharedDataInterface = CAknFepSharedDataInterface::NewL(this);

    TInt previUiLanguage = iSharedDataInterface->PluginUILanguage();

    if (iUiLanguage != previUiLanguage)
        {
        
        // if UI language has changed we need to set text input language according to UI language
        iSharedDataInterface->SetPluginUILanguage(iUiLanguage);
        iSharedDataInterface->SetInputTextLanguage(0);  // signals that it needs to be updated
        }

    if (!iSharedDataInterface->InputMode()) //not initialised yet - first time device is switched on
        {
        switch (iUiLanguage)
            {
            case ELangPrcChinese:
                iSharedDataInterface->SetInputMode(EPinyin);
                break;
            case ELangTaiwanChinese:
                iSharedDataInterface->SetInputMode(EZhuyin);
                break;
            case ELangHongKongChinese:
                iSharedDataInterface->SetInputMode(EStroke);
                iSharedDataInterface->SetCangJieMode(ECangJieNormal);
                break;
            case ELangJapanese:
                iSharedDataInterface->SetInputMode(EHiraganaKanji);
                break;
            case ELangKorean:
                iSharedDataInterface->SetInputMode(EHangul);
                break;
            case ELangHindi:
                iSharedDataInterface->SetInputMode(EHindi);
                break;
            default:
                iSharedDataInterface->SetInputMode(ELatin);
                break;
            }
        }

    iUiInterface->SetFepMenuObserver(this);
   
    TInt inputLanguage = iSharedDataInterface->InputTextLanguage();

    // A process instantiating Fep may miss ECapabilityWriteDeviceData capability, 
    // which is needed for updating locale repository.
    // A workaround is to delay change up to a process that has the capability
    TSecurityInfo info;
    info.Set(RProcess());

    TBool hasCapability = info.iCaps.HasCapability(ECapabilityWriteDeviceData);
    
    if (!inputLanguage && hasCapability)
        {
        // set correct input language
        inputLanguage = SubstituteSublanguageId(iUiLanguage);
        
        // If Ptiengine is not initialize, initialize it
        if (!iPtiEngine)
            {
            iPtiEngine = CPtiEngine::NewL(ETrue);    
            }         
        // If still not found, default to english.
        if (!iPtiEngine->GetLanguage(inputLanguage))
            {
            inputLanguage = ELangEnglish;   
            }
        
        // Set the current input language
        iSharedDataInterface->SetInputTextLanguage(inputLanguage);
        
        // update numeric input
        if ( iUiLanguage == ELangArabic ) 
            {
            SetDefaultNumberMode( ENbrModeArabic, 
                                  ENbrModeTypeArabic );
            }            
        else if ( ( iUiLanguage == ELangUrdu  ) || 
                  ( iUiLanguage == ELangFarsi  ) )
            {
            SetDefaultNumberMode( ENbrModeArabic, 
                                  ENbrModeTypeEasternArabic );
            }
		else if ( TAknFepUiIndicInputManager::IsScriptDevanagari( iUiLanguage ) )
            {
            SetDefaultNumberMode( ENbrModeLatin, 
                                  ENbrModeTypeIndic );
            }
        else
            {           
            SetDefaultNumberMode( ENbrModeLatin, ENbrModeTypeArabic );
            }
        }
        
    iSctEditChars = HBufC::NewL( 10 );
    
#ifdef RD_INTELLIGENT_TEXT_INPUT
    // Predictive QWERTY (XT9) changes ---->
    	if(CCoeEnv::Static()->AppUi())
				iExactWordPopupContent = iUiInterface->CreateWordPopupContentL();
    // Predictive QWERTY (XT9) changes <----
#endif //RD_INTELLIGENT_TEXT_INPUT
    // If the following line is uncommented, the FEP is fully constructed
    // for all applications at application startup.
    // ConstructFullyL();
    iShiftKeypressMonitor = CPeriodic::NewL(CActive::EPriorityStandard);
#ifdef RD_INTELLIGENT_TEXT_INPUT
    iFnKeyManager = CAknFepFnKeyManager::NewL( *this,iSharedDataInterface );
#endif
    TRAPD(result,iKeyCatcher = CAknFepKeyCatcher::NewL(*this));
    if(result == KErrNone)
        {
        iKeyCatcher->ActivateL();
        }
    // the follow 3 objects for phrase creation
    iZhuyinKeyHandler = CZhuyinKeyHandler::NewL();
    iZhuyinAnalyserDb = CZhuyinAnalyserDbFeed::NewL(EPinyinAnalyserTypeZhuyin);
    iZhuyinAnalyser = CAknFepZhuyinAnalyser::NewL(iZhuyinAnalyserDb);
    iAknFepThaiSCTSelector = CAknFepThaiSCTSelector::NewL();
    iHybridAplphaChangedToAlphanumeric = EFalse;
    iLastFocusedEditor = NULL;
    iFepAwareDialogParentEditor = NULL;
	
    // Fep manager should have both read and write privilege.
    // Known case: Auto-completion cannot be changed in setting dialog.
    RProperty::Define( KPSUidAknFep, KAknFepSettingDialogState, RProperty::EInt );
    }

void CAknFepManager::ConstructFullyL()
    {
    if (!iPtiEngine)
        {
        iPtiEngine = CPtiEngine::NewL(ETrue);    
        } 
    
    if ( !(iPtiEngine->GetLanguage(iSharedDataInterface->InputTextLanguage())) )
        {
        // If input language cenrep value is set to language unknown to PtiEngine,
        // try to reset based on UI-language. 
        TInt inputLang = SubstituteSublanguageId(iUiLanguage);
        
        // If still not found, default to english.
        if (!iPtiEngine->GetLanguage(inputLang))
            {
            inputLang = ELangEnglish;   
            }
        iSharedDataInterface->SetInputTextLanguage(inputLang);      
        }      
#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifdef FF_DUAL_LANGUAGE_SUPPORT
    if (iPtiEngine->GetLanguage(iSharedDataInterface->InputTextLanguageSecondary()) == NULL)
        {
        // If secondary input language is unknown to PtiEngine, set it to none
        iSharedDataInterface->SetInputTextLanguageSecondary(0);
        }
#endif //FF_DUAL_LANGUAGE_SUPPORT
#endif
        
    if(NULL == iKeyCatcher)
        {
        iKeyCatcher = CAknFepKeyCatcher::NewL(*this);    
        iKeyCatcher->ActivateL();
        }
    
    iConcatenationTimer=CPeriodic::NewL(CActive::EPriorityStandard);
    iPostEventCheck=CIdle::NewL(CActive::EPriorityStandard);
    iChrKeypressMonitor = CPeriodic::NewL(CActive::EPriorityStandard);

    CCoeEnv* coeEnv = CCoeEnv::Static();
    TInt inputLanguage = iSharedDataInterface->InputTextLanguage();
#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifdef FF_DUAL_LANGUAGE_SUPPORT
    TInt inputLanguageSecondary = iSharedDataInterface->InputTextLanguageSecondary();
#endif //FF_DUAL_LANGUAGE_SUPPORT
#endif
    SetWesternPredictive(iSharedDataInterface->PredictiveTextOn());
#ifndef RD_INTELLIGENT_TEXT_INPUT
    iIsAutoCompleteOn = iSharedDataInterface->PredictiveTextAutoCompleteOn(); // AutoCompletion disabled by Default 
#endif	//RD_INTELLIGENT_TEXT_INPUT
#ifdef RD_INTELLIGENT_TEXT_INPUT
    iIsAutoCompleteOn = iSharedDataInterface->PredictiveTextAutoCompleteOn(); // AutoCompletion disabled by Default 
    iTypingCorrectionLevel = iSharedDataInterface->PredictiveTextTypingCorrectionLevel();
    iIsNumberCandidateShown = iSharedDataInterface->PredictiveTextNumberCandidateShown();
    TInt primaryCandidateValue = iSharedDataInterface->PredictiveTextPrimaryCandidate();
    if( primaryCandidateValue == 0)
		iPrimaryCandidate = ETrue;
	else if(primaryCandidateValue == 1)
		iPrimaryCandidate = EFalse; 
#endif // RD_INTELLIGENT_TEXT_INPUT
    iJapanesePredictive = iSharedDataInterface->PredictiveTextJapaneseOn();
    iQwertyInputMode = iSharedDataInterface->QwertyInputMode();
    iMultiTapTimer = iSharedDataInterface->MultiTapTimer();
    iJapaneseQwertyFlags = iSharedDataInterface->JapaneseQwertyFlags();
#ifdef RD_INTELLIGENT_TEXT_INPUT
    if(!iFnKeyManager)
        {
        iFnKeyManager = CAknFepFnKeyManager::NewL( *this,iSharedDataInterface );
        }    
#endif
    iCaseMan = CAknFepCaseManager::NewL(*this);
    iLangMan = CAknFepLanguageManager::NewL( this, iCaseMan, iSharedDataInterface );
    iPtiEngine->ActivateLanguageL(inputLanguage);
#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifdef FF_DUAL_LANGUAGE_SUPPORT
    iPtiEngine->SetSecondaryInputL(inputLanguageSecondary);
#endif //FF_DUAL_LANGUAGE_SUPPORT
#endif
    iLangMan->SetInputLanguageL(inputLanguage);
    SetInputLanguageCapabilities(inputLanguage);
    iCurrentFepUI = iLangMan->GetFepUI(ELatin, iCharWidth, IsPredictive(ELatin));

    iLastKeyPressedWasHashKey = EFalse;
    iDeviceSupportsRtlLanguage = DeviceSupportsRTLLanguageL();

    TResourceReader reader;
    // get character from resource to represent an 'enter' character
    coeEnv->CreateResourceReaderLC(reader, R_AVKON_SCT_SPACE_SYMBOL);
    iSpaceCharacterCode = reader.ReadUint16();
    coeEnv->CreateResourceReaderLC(reader, R_AVKON_SCT_FULLWIDTHSPACE_SYMBOL);
    iFullWidthSpaceCharacterCode = reader.ReadUint16();
    CleanupStack::PopAndDestroy(2); //reader

    //initialise states
    iMode = EAknEditorNullInputMode; //Force a sync of mode...
    SyncStates(EAknFepStateNull);
    ResetFlags();
    ResetExtendedFlags();

    ReadHashKeyLoopL();
    iHashKeyMan = CAknFepHashKeyManager::NewL(*this, iCaseMan);
    SetHashKeyStyle();
    iInlineTextDecorator = new (ELeave) CAknFepInlineTextDecorator;
    if (IsFeatureSupportedJapanese())
        {
        CAknJapaneseReading::CreateL(iPtiEngine);
        }
    // setting call back
    TCallBack callback(DoSimulateKey, this);
    iAsyncOwnSimulateKey.Set(callback);
    
    TCallBack resourcechangedcallback(HandleResourceChangedCallback,this);
    iAsyncResouceChanged.Set(resourcechangedcallback);
#ifdef RD_SCALABLE_UI_V2
    iFepPluginManager = CAknFepPluginManager::NewL
        ( *this, *iSharedDataInterface, *iLangMan, *iCaseMan);
    iNotifyPlugin = ETrue;
#endif //RD_SCALABLE_UI_V2

    iFepFullyConstructed = ETrue;
#ifdef RD_INTELLIGENT_TEXT_INPUT
    iIsFnKeyMapped = IsFnKeyMappedL();
    iKeyboardType = (TPtiKeyboardType)iSharedDataInterface->KeyboardLayout();
#endif
    
    iFnKeypressMonitor = CPeriodic::NewL(CActive::EPriorityStandard);
    }

CAknFepManager::~CAknFepManager()
    {
#ifdef RD_PF_SEC_APPARC
    delete iNullService;
#else
    //Embedding - removing created document
    if ( iEmbedded )
        {
        CEikonEnv::Static()->Process()->DestroyDocument(iEmbedded); // User dictionary
        iEmbedded = NULL;
        }
#endif
    if (IsFlagSet(EFlagInsideInlineEditingTransaction))
        {
        SetFlag(CAknFepManager::EFlagSupressAutoUpdate);
        TRAPD(err, CommitInlineEditL())
        if (err)
            {
            CleanUpFep();
            }
        }
        
    if ( EditorState() )
        {
        EditorState()->SetObserver(NULL);    
        }        
    if (iLastFocusedEditor)
        {
        iLastFocusedEditor->SetObserver(NULL);
        iLastFocusedEditor = NULL;
        } 
    if (iFepAwareDialogParentEditor)
        {
        iFepAwareDialogParentEditor->SetObserver(NULL);
        iFepAwareDialogParentEditor = NULL;
        } 
    
    delete iSharedDataInterface;
    MAknEditingStateIndicator* indicator = CAknEnv::Static()->EditingStateIndicator();
    if (indicator == iIndicator)
        {
        CAknEnv::Static()->SwapEditingStateIndicator(NULL);
        }

        
    delete iIndicator;
    iIndicator = NULL;
    CCoeEnv::Static()->DeleteResourceFile(iResId);

    // Everything that is constructed in ConstructFullyL() method
    // needs to be deleted in CommonDestroyFep() method.
    CommonDestroyFep();

	if (iUiInterface)
	    iUiInterface->SetFepMenuObserver(NULL);

    delete iPtiEngine;
    delete iSctEditChars;

#ifdef RD_SCALABLE_UI_V2
    if ( FepAwareTextEditor() )
        {
    		UnregisterObserver();
        }
    delete iFepPluginManager;
    delete iStylusCcpuMenu;        

    delete iSpellInitText;        
#endif // RD_SCALABLE_UI_V2
    delete iUiInterface;
#ifdef RD_INTELLIGENT_TEXT_INPUT
    // Predictive QWERTY (XT9) changes ---->
    delete iExactWordPopupContent;
    iExactWordPopupContent = 0;
    delete iCandidatePopup;
    iCandidatePopup = 0;
    delete iResourceString;
    iResourceString = 0;
    // Predictive QWERTY (XT9) changes <----
#endif //RD_INTELLIGENT_TEXT_INPUT
    // the follow objects for phrase creation
      delete iZhuyinKeyHandler;
      delete iZhuyinAnalyser;
    delete iAknFepThaiSCTSelector;
    }

void CAknFepManager::CommonDestroyFep()
    {
    iFepFullyConstructed = EFalse;

    // This method is called both from destructor and CleanUpFep methods.
    iAsyncOwnSimulateKey.Cancel();
    iAsyncResouceChanged.Cancel();
    delete iShiftKeypressMonitor;
    iShiftKeypressMonitor = NULL;
    delete iConcatenationTimer;
    iConcatenationTimer = NULL;
    delete iChrKeypressMonitor;
    iChrKeypressMonitor = NULL;
    delete iLangMan;
    iLangMan = NULL;
    iCurrentFepUI = NULL;
    delete iKeyCatcher;
    iKeyCatcher = NULL;

    delete iPostEventCheck;
    iPostEventCheck = NULL;
    delete iHashKeyMan;
    iHashKeyMan = NULL;
    delete iCaseMan;
    iCaseMan = NULL;
    delete iInlineTextDecorator;
    iInlineTextDecorator = NULL;
    iHashKeyLoop.Close();
#ifdef RD_INTELLIGENT_TEXT_INPUT
    if(iFnKeyManager)
        {
        delete iFnKeyManager;
        iFnKeyManager = NULL;
        }
    if( iNumericResourceTimer )        
        {
        delete iNumericResourceTimer;
        iNumericResourceTimer = NULL;    
        }
#endif
    delete iFnKeypressMonitor;
    iFnKeypressMonitor = NULL;
    }

#ifdef RD_INTELLIGENT_TEXT_INPUT
void CAknFepManager::SetKeyboardLayout(TPtiKeyboardType aType)
    {
    	
    if(iQwertyInputMode && iFnKeyManager 
    		&& (iFnKeyManager->FnKeyState()== CAknFepFnKeyManager::EFnKeyLock
		||iFnKeyManager->FnKeyState()== CAknFepFnKeyManager::EFnKeyNext)) 
		{
        	iMode = ENumber;
		}
	
	if(iFnKeyManager)        
		{
		iFnKeyManager->ClearFnKeyState();
		}
    	
    if(iPtiEngine)
        {
        iPtiEngine->SetKeyboardType(aType);
        }    
    iKeyboardType = aType;        
    }

TPtiKeyboardType CAknFepManager::KeyboardLayout() const
    {
    return iKeyboardType;
    }
#endif
TBool CAknFepManager::TryCloseUiL()
    {
#ifdef RD_HINDI_PHONETIC_INPUT        
    if ((iSharedDataInterface->InputTextLanguage() == KLangHindiPhonetic) && 
         IsFlagSet(CAknFepManager::EFlagNoMatches))
        {
        ClearFlag(CAknFepManager::EFlagNoMatches);
    	if (PreviousChar() == KQuestionMark)
    		RemovePreviousCharacterL();
    	}
#endif    
    if (iFepManState == EAknFepStateUIActive)
        {
        // In Japanese input,
        // commit inline editing at only the western predictive mode.
        if (!IsFeatureSupportedJapanese() || WesternPredictive())
            {
            if (IsFlagSet(EFlagInsideInlineEditingTransaction))
                {
                TryRemoveNoMatchesIndicatorL();
                UpdateCbaL(NULL);
                }
                
             	

//#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
#ifdef RD_INTELLIGENT_TEXT_INPUT
            if ( iFepPluginManager && 
             	(iFepPluginManager->PluginInputMode() == EPluginInputModeNone))
            	{
            	RemoveSuggestedAdvanceCompletionL();
            	}            
#endif // RD_INTELLIGENT_TEXT_INPUT              
//#endif // __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
                
#ifdef RD_SCALABLE_UI_V2 
            
            if ( iFepPluginManager != NULL && 
                 iFepPluginManager->BeforeSpell() )
                {
                iFepPluginManager->SetBeforeSpell(EFalse);
                }
            else
#endif  
          
                {
                CommitInlineEditL();    
                }
            }
        if (IsCcpuFlagSet(ECcpuStataCommitPredictiveWord))            
            {
            // Word commiting must be done after CommitInlineEditL call (because
            // of Japanese ReadingTextL funtionality), hence the delayed commit here.
            iPtiEngine->CommitCurrentWord();
            ClearCcpuFlag(ECcpuStataCommitPredictiveWord);
            }            
        InternalFepUI()->CloseUI();
        SyncStates(EAknFepStateInitial);
        if (iMode == EPinyin || iMode == EZhuyin || iMode == EStroke || iMode == ECangJie)
            {
            SetCursorType(EActive);
            if ( EditorState() )
            	{
            	EditorState()->SetFlags( EditorState()->Flags() & ~EAknEditorFlagChinesePopup );
            	}            
            }

        // EFalse in case of latin input in Japanese variant
        if (IsFeatureSupportedJapanese() 
         && iMode == ELatin && !WesternPredictive())
            {
            return EFalse;
            }

        return ETrue;
        }
    else
        {
    	if (IsKoreanInputLanguage())
        	{
        	if (IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction))
        		{
        	   	CommitInlineEditL();
        		PtiEngine()->ClearCurrentWord();
        		}
        	}
    	
        //in initial state characters should either:
        //1. not be consumed by the FEP.
        //2. be committed immediately by the Input State.
        //3. be multitap chars, in which case they may need to be committed
        // here. It is best to let the engine deal with this.
        if ( IsFlagSet( EFlagInsideInlineEditingTransaction ) )
            {
            InternalFepUI()->ExpireMultitapTimer();
            }
        return EFalse;
        }
    }

void CAknFepManager::SendUIActivatedNotification()
    {
    SyncStates(EAknFepStateUIActive);
    }

void CAknFepManager::SendUIDeActivatedNotification()
    {
    SyncStates(EAknFepStateInitial);
    }

void CAknFepManager::PlaySound(TAvkonSystemSID aSound) const
    {
    if (SoundSystem())
        {
        SoundSystem()->PlaySound(aSound);
        }
    }
    
void CAknFepManager::SetLongClearAfterCloseUI(TBool aLongClear)
    {
    iLongClearAfterCloseUI = aLongClear;
    }

TKeyResponse CAknFepManager::HandleKeyEventL(TUint aCode, TKeyPressLength aLength,TEventCode aEventCode)
    {
    TKeyResponse response = EKeyWasNotConsumed;

    TInt documentLength = -1;
    if (iInputCapabilities.FepAwareTextEditor())
        {
        documentLength = iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep();
        }

    if (iFepFullyConstructed && iConcatenationTimer->IsActive())
        {
        if (!( aCode == EKeyRightArrow ||
             aCode == EKeyLeftArrow ||
             aCode == EKeyUpArrow ||
             aCode == EKeyDownArrow ))
            {
            iConcatenationTimer->Cancel();
            }
        }

#ifdef RD_INTELLIGENT_TEXT_INPUT    
    
    if(iKeyBackSpaceHit && aCode != EKeyBackspace)
    	iKeyBackSpaceHit = 0;
    
    if (aCode != EPtiKeyStar && iNumericResourceTimer)
        {
        // return with EKeyWasConsumed if any of the arrow keys are pressed with 
        // iNumericResourceTimer is active
        if(NumericResourceMultiTapTimerTimeoutL()&&
           (aCode == EKeyRightArrow || aCode == EKeyLeftArrow || aCode == EKeyUpArrow || aCode ==EKeyDownArrow))
            return EKeyWasConsumed;
        }
#endif



    // This method is implemented independent of all modes, so it can be called by anyone, anywhere
    if ( BidiCursorRequired() )
        {
        response = AttemptCursorFlipAtAmbiguousPointL( aCode );
        if ( response == EKeyWasConsumed )
            {
            return response;
            }
        }

    if ( aCode != EPtiKeyHash 
#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifdef __HALF_QWERTY_KEYPAD
#ifdef __SHIFT_KEY_LOOP		
    		&& ( aCode != EPtiKeyQwertyLeftShift 
    		|| EPtiKeyboardHalfQwerty != KeyboardLayout() )
#endif //__SHIFT_KEY_LOOP
#endif //__HALF_QWERTY_KEYPAD
#endif //RD_INTELLIGENT_TEXT_INPUT   		
    		)
    
        {
        
        SetLastPressedHashKeyStatus(EFalse);
        
        if(iHashKeyMan)
            {
            iHashKeyMan->CancelHashKeyTimer();
            if (aCode != EKeyOK)
                {
                iHashKeyMan->ClearSelectionModeHashKeyLoop();
                }
            }
        }
    // Always commit the text before handling arrow keys
    if (iMode == EHangul && IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction))
    	{
    	if ( aCode == EKeyRightArrow ||
             aCode == EKeyLeftArrow ||
             aCode == EKeyUpArrow ||
             aCode == EKeyDownArrow )
    		{
    		CommitInlineEditL();
       		iPtiEngine->ClearCurrentWord();
    		}
        }
    TBool isPassNext = IsFlagSet(EFlagPassNextKey);
   
    switch (aCode)
        {
        case EKeyEscape:

#ifdef RD_SCALABLE_UI_V2     
        if( iFepFullyConstructed && iFepPluginManager)
            {
            iFepPluginManager->ResetMenuState(EFalse);            
            }
#endif         
            if (TryCloseUiL())
                {
                response = EKeyWasConsumed;
                // phrase creation
                PhraseCreation(EFalse);
                // phrase creation
                PinyinPhraseCreation(EFalse);
                }
            break;
#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifdef __HALF_QWERTY_KEYPAD
#ifdef __SHIFT_KEY_LOOP
        case EPtiKeyQwertyLeftShift:
        if( EPtiKeyboardHalfQwerty == KeyboardLayout() )
        	{
			response = HandleHashKeyL(aLength);
            (aLength == ELongKeyPress)? SetLastPressedHashKeyStatus(EFalse):SetLastPressedHashKeyStatus(ETrue);
			}
            break;			
#endif //__SHIFT_KEY_LOOP       	
#endif //__HALF_QWERTY_KEYPAD	
#endif //RD_INTELLIGENT_TEXT_INPUT
        case EPtiKeyHash:
            if(InputMode() == EHangul)
            	{
				if (aLength == EShortKeyPress)
					{
					FepUI()->HandleKeyL(aCode, aLength);
					response = EKeyWasConsumed;
					}
				else
					{
               	 	//PhraseCreation(EFalse);
            		response = HandleHashKeyL(aLength);
            		(aLength == ELongKeyPress)? SetLastPressedHashKeyStatus(EFalse):
                                        		SetLastPressedHashKeyStatus(ETrue);
					}
				}
     	     else if (InputMode()!= EZhuyin)
                {
                if (IsPinyinPhraseCreation() )
                    {
                    ClearCcpuFlag( ECcpuStateHashDown );
                    response = EKeyWasConsumed;
                    }
                else
                    {
                    response = HandleHashKeyL(aLength);
                    (aLength == ELongKeyPress)
                        ? SetLastPressedHashKeyStatus(EFalse)
                            : SetLastPressedHashKeyStatus(ETrue);
                    }
                }
            else
                {
                PhraseCreation(EFalse);
                response = HandleHashKeyL(aLength);
                (aLength == ELongKeyPress)
                    ? SetLastPressedHashKeyStatus(EFalse)
                        : SetLastPressedHashKeyStatus(ETrue);
                }
            break;
        case EKeyCBA1:
            if(Japanese())
                {
                if (FepUI()->HandleKeyL(aCode, aLength))
                    {
                    response = EKeyWasConsumed;
                    }
                break;
                }
            // phrase creation the key CBA2 need to solve.
            if ( IsEntryPhrase() )
                {
                if ( FepUI()->HandleKeyL( aCode, aLength ) )
                    {
                    response = EKeyWasConsumed;
                    break;
                    }
                }

            if (!WesternPredictive())
                {
                if (TryCloseUiL())
                    {
                    response = EKeyWasConsumed;
                    }
                }            
            break;
        case EKeyCBA2:
            if(Japanese())
                {
                if (FepUI()->HandleKeyL(aCode, aLength))
                    {
                    response = EKeyWasConsumed;
                    }
                break;
                }
            if (!WesternPredictive())
                {
                if (TryCloseUiL())
                    {
                    // phrase creation the key CBA2 need to solve.
                    PhraseCreation( EFalse );
                    response = EKeyWasConsumed;
                    }
                }
     		 else
                {
                if ( IsFlagSet( EFlagInsideInlineEditingTransaction ) )
                    {
                    InternalFepUI()->ExpireMultitapTimer();
                    }
                }
            // phrase creation the key CBA2 need to solve.
            EntryPhrase( EFalse );
            break;
        case EPtiKeyStar:
            if (IsFlagSet(EFlagPassNextKey))
                {
                ClearFlag(EFlagPassNextKey);
                }
            else if ( !IsFepAwareTextEditor() )
                {
                // The editor is MFNE or other non FEP aware text editor.
                // Pass the key to the editor to handle.
                response = EKeyWasNotConsumed;
                }
            else if (IsFlagSet(EFlagShiftKeyDepressed))
                {
                response = EKeyWasConsumed;
                }
            else 
                {
#ifdef RD_SCALABLE_UI_V2                
                iStarKeyPressed = ETrue;
#endif // RD_SCALABLE_UI_V2                    
                if (FepUI()->HandleKeyL(aCode, aLength, aEventCode))
                    {
                    response = EKeyWasConsumed;
                    }
#ifdef RD_SCALABLE_UI_V2                
                iStarKeyPressed = EFalse;
#endif // RD_SCALABLE_UI_V2                     
                }
            break;
        case EKeyRightArrow:
            if (TryHandleArrowRightEventL(documentLength))
                {
                response = EKeyWasConsumed;
                break;
                }
            // fall through
        case EKeyLeftArrow:
            if (aCode == EKeyLeftArrow && TryHandleArrowLeftEventL(documentLength))
                {
                response = EKeyWasConsumed;
                break;
                }
                
            if (IsCcpuFlagSet(ECcpuStateCopy | ECcpuStateCut))    
                {
                response = EKeyWasNotConsumed;
                break;
                }            
            else if (IsCcpuFlagSet(ECcpuStateStartCopy | ECcpuStateStartCut))    
                {
                TryHandleCommonArrowAndBackspaceFunctionalityL(aCode, response);                
                break;
                }               

            if (iLanguageCapabilities.iInputLanguageCode == ELangJapanese && iMode != ELatin)
                {
                if(FepUI()->HandleKeyL(aCode, aLength))
                    {
                    response = EKeyWasConsumed;
                    }
                }
            else
                {
                if (WesternPredictive())
                    {
                    response = HandleWesternPredictiveArrowKeyL(aCode, aLength);
                    }
                else
                    {
                    if (iFepManState == EAknFepStateUIActive)
                        {
                        FepUI()->HandleKeyL(aCode, aLength);
                        response = EKeyWasConsumed;
                        }
                    else
                        {
                        /*
						TCursorSelection currentEditorSelection(0,0);
						if(iInputCapabilities.FepAwareTextEditor())
							{
							iInputCapabilities.FepAwareTextEditor()->
								GetCursorSelectionForFep(currentEditorSelection);
							}
                        
						if(!IsMfneEditor() &&
								(PluginInputMode() == EPluginInputModeItut ||
								 PluginInputMode() == EPluginInputModeFSQ)  &&
								 currentEditorSelection.Length() != 0)
							{
							response = EKeyWasConsumed;
							break;
							}
						*/

                        response = HandleNaviEventOutsideInlineEditL(aCode, aLength);
                        if (response == EKeyWasConsumed)
                            {
                            iCaseMan->UpdateCase(ENullNaviEvent);
                            }
                        else if (aCode == EKeyLeftArrow && !IsFlagSet(EFlagShiftKeyDepressed))
                            {
                            iCaseMan->UpdateCase(ELeftNaviEvent);
                            }
                        else if (!IsFlagSet(EFlagShiftKeyDepressed))
                            {
                            iCaseMan->UpdateCase(ERightNaviEvent);
                            }
                        }
                    }
                }
            break;
        case EKeyDownArrow:
       	    if( iWesternPredictive && IsFlagSet(EFlagInsideInlineEditingTransaction) && 
       	    	( EPtiKeyboard12Key == KeyboardLayout() ) && ( IsFlagSet(EFlagShiftKeyDepressed) ||
       	    	  HashKeySelectionInUse()&& IsCcpuFlagSet(ECcpuStateHashDown) ) )
            	{
 				iPtiEngine->CommitCurrentWord();
            	CommitInlineEditL();
            	response = EKeyWasConsumed;
            	break;
            	}

          if (TryHandleArrowDownEventL(documentLength) || IsFlagSet(EFlagNoMatches))
                {
                response = EKeyWasConsumed;
                break;
                }
                
        case EKeyUpArrow:
            if (iLanguageCapabilities.iInputLanguageCode == ELangJapanese && iMode != ELatin)
                {
                if ( (iAknEditorFlags & EAknEditorFlagNoLRNavigation) &&
                    IsFlagSet(EFlagShiftKeyDepressed) )
                    {
                    // Up or down key is pressed during shift key press.
                    ClearFlag(EFlagNoActionDuringShiftKeyPress);
                    return EKeyWasNotConsumed;
                    }

                if(FepUI()->HandleKeyL(aCode, aLength))
                    {
                    response = EKeyWasConsumed;
                    }
                else
                    {
                    TryHandleCommonArrowAndBackspaceFunctionalityL(aCode, response);
                    }
                }
            else
                {
                if (iLanguageCapabilities.iInputLanguageCode == ELangThai &&
                    iFepManState == EAknFepStateUIActive &&
                    WesternPredictive() )
                    {
                    if (FepUI()->HandleKeyL(aCode, aLength))
                        {
                        ClearFlag(EFlagChangeInputMode);
                        response = EKeyWasConsumed;
                        }
                    break;
                    }
                else if ( (iAknEditorFlags & EAknEditorFlagNoLRNavigation) &&
                    IsFlagSet(EFlagShiftKeyDepressed) )
                    {
                    // Up or down key is pressed during shift key press.
                    ClearFlag(EFlagNoActionDuringShiftKeyPress);
                    return EKeyWasNotConsumed;
                    }
                if (WesternPredictive())
                    {
                    // For customizabal of Down/Up scroll keypress action.
                    // If it need to custom it, the function of HandleKeyL returns EFalse.
#ifdef RD_INTELLIGENT_TEXT_INPUT
                    // Predictive QWERTY (XT9) changes ---->
                    if ( IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction) )
                        {
                        TInt wordIndexToSelect = 0;
                        
                        if(iQwertyInputMode && EPtiKeyboardHalfQwerty != KeyboardLayout())
                        	{
                        	if(!iPrimaryCandidate)
                            iPtiEngine->HandleCommandL( EPtiCommandUserActionGetIndexOfSecondaryCandidate, 
        							&wordIndexToSelect );
                            
                        	if( iQwertyInputMode 
                        	    && IsExactWordPopUpShown()
                        	    && aCode == EKeyUpArrow )
	                        	{
		                        iExactWordPopupContent->HidePopUp();
                            
                                // When press up, if FSQ is opened, 
                                // also need to check and close tooltip on it.
		                        SendEventsToPluginManL( EPluginHideTooltip );
		                                                                                    
	                        	iPtiEngine->HandleCommandL( 
                                           EPtiCommandUserActionSetCurrentIndexOfCandidates, 
	                        								 &wordIndexToSelect );
	                        	iPtiEngine->HandleCommandL( EPtiCommandLockCurrentCandidate );
	                        	/* Update the Candidate List in case of Exact Typing as best predicted word is 
	                             selected from the tooltip to the editor */
	                        	if(!iPrimaryCandidate)
	                        	    iPtiEngine->HandleCommandL(EPtiCommandSetFlagToUpdateCandidates);
	                        	    
	                        	TPtrC selectedWord = iPtiEngine->CurrentWord();
	                        	UpdateInlineEditL( selectedWord, selectedWord.Length() ); 
	                        	response = EKeyWasConsumed;
		                     
	                        	}
	                        else
	                        	{
		                        LaunchCandidatePopupListL( wordIndexToSelect );
		                        response = EKeyWasConsumed;
	                        	}
                        	}
                        else
                        	{
                        	if (!IsFlagSet(EFlagInsideMultitapInlineEditingTransaction))
                        	    {
                        	    TInt lastCandidateIndex = iPtiEngine->NumberOfCandidates() -1;
                           	    if( EPtiKeyboard12Key == KeyboardLayout() && ( IsFlagSet(EFlagShiftKeyDepressed) ||
                           	    		  HashKeySelectionInUse()&& IsCcpuFlagSet(ECcpuStateHashDown) ) )
    	                        	{
    	                            if (response != EKeyWasConsumed)
    	                                {
    	                                response = HandleWesternPredictiveArrowKeyL(aCode, aLength);
    	                                }
    	                        	}
                           	    else if(aCode == EKeyUpArrow && lastCandidateIndex >= 0)
                        	        {
                        	        // Get last word on list
                        	        LaunchCandidatePopupListL( lastCandidateIndex );
                        	        response = EKeyWasConsumed;
                        	        }
                        	    else
                        	        {
                        	        // Get next word on list (wrap around if at end)
                        	        iPtiEngine->HandleCommandL( EPtiCommandUserActionGetCurrentIndexOfCandidates, 
            							&wordIndexToSelect );
                        	        wordIndexToSelect < lastCandidateIndex ? wordIndexToSelect++ : wordIndexToSelect = 0;
                        	        LaunchCandidatePopupListL( wordIndexToSelect );
                        	        response = EKeyWasConsumed;
                        	        }                        	                        	    
                        	    }
                        	else
                        	    response = EKeyWasConsumed;
                        	}
                        }
                    else
                        {                       
	                    // Predictive QWERTY (XT9) changes <----
#endif //RD_INTELLIGENT_TEXT_INPUT
                    if(iFepManState == EAknFepStateUIActive &&
                        FepUI()->HandleKeyL(aCode, aLength))
                        {
                        response = EKeyWasConsumed;
#ifdef RD_INTELLIGENT_TEXT_INPUT

                        // When press up, if FSQ is opened, 
                        // also need to check and close tooltip on it.
                        SendEventsToPluginManL( EPluginHideTooltip );

	                    // Predictive QWERTY (XT9) changes ---->
	                    // Exact word popup is hidden when the exact word is selected
	                    iExactWordPopupContent->HidePopUp();
	                    // Predictive QWERTY (XT9) changes <----
#endif //RD_INTELLIGENT_TEXT_INPUT
                        }
                    // normal operation of  Down/Up scroll keypress action.
                    if (response != EKeyWasConsumed)
                        {
                        response = HandleWesternPredictiveArrowKeyL(aCode, aLength);
                        }
#ifdef RD_INTELLIGENT_TEXT_INPUT
                        } // Predictive QWERTY (XT9) changes
#endif //RD_INTELLIGENT_TEXT_INPUT
                    }
                else
                    {
                    if (iFepManState == EAknFepStateUIActive)
                        {
                        FepUI()->HandleKeyL(aCode, aLength);
                        response = EKeyWasConsumed;
                        }
                    else
                        {
                        TryHandleCommonArrowAndBackspaceFunctionalityL(aCode, response);
                        }
                    }
                }
            break;
        case EKeyBackspace:
            if (iLanguageCapabilities.iInputLanguageCode == ELangJapanese &&
                !(iMode == ELatin || iMode == ENumber))
                {
                if (FepUI()->HandleKeyL(aCode, aLength))
                    {
                    response = EKeyWasConsumed;
                    }
                }
            else if (iLanguageCapabilities.iInputLanguageCode == ELangVietnamese)
                {
                TCursorSelection cursorSelection;
                if ( !IsFepAwareTextEditor() )                                           
                    {
                    response = EKeyWasNotConsumed;
                    }
                else
                    {
                    iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(cursorSelection); 
                    if (cursorSelection.Length() != 0)      
                        {
                        response = EKeyWasNotConsumed;
                        }
#ifdef RD_SCALABLE_UI_V2
                    else if ( !documentLength 
                        && ( iFepPluginManager->PluginInputMode() == EPluginInputModeItut
                        || iFepPluginManager->PluginInputMode() == EPluginInputModeVkb ) )
                        {
                        response = EKeyWasConsumed;
                        }
#endif   
										else 
                        {
                        if (FepUI()->HandleKeyL(aCode, aLength))
                            {
                            response = EKeyWasConsumed;
                            }
                        else
                            {
                            if( WesternPredictive() )
                                {
                                // We need to start the autoconcatination timer
                                SimulateKeyEventL(EKeyF24);
                                }
                            }
                        }
                    }
                
                }
            else if (IsKoreanInputLanguage() && iMode == EHangul)
                {
                if (FepUI()->HandleKeyL(aCode, aLength))
                    {
                    response = EKeyWasConsumed;
                    }
                }
            else
                {
                if (iFepManState == EAknFepStateInitial)
                    {
#ifdef RD_SCALABLE_UI_V2
                     if ( iFepPluginManager->PluginInputMode() == EPluginInputModeItut )
                        {
                        if ( iFepPluginManager->CurrentFepInputUI() && 
                            FepUI()->HandleKeyL(aCode, aLength))
                            {
                            response = EKeyWasConsumed;
                            break;
                            }
                        }
                     if ( !documentLength 
                        && ( iFepPluginManager->PluginInputMode() == EPluginInputModeItut
                        || iFepPluginManager->PluginInputMode() == EPluginInputModeVkb ) )
                        {
                        response = EKeyWasConsumed;
                        break;

                        }
#endif                        
                    if (IsFlagSet(EFlagLineFeedCharacter))
                        {
                        // restore cursor visibility
                        if (IsFlagSet(EFlagInsideMultitapInlineEditingTransaction))
                            {
                            iInputCapabilities.FepAwareTextEditor()->SetInlineEditingCursorVisibilityL(ETrue);        
                            }
                        
                        iInputCapabilities.FepAwareTextEditor()->CancelFepInlineEdit();
                        ClearFlag(EFlagInsideMultitapInlineEditingTransaction | 
                                  EFlagInsideInlineEditingTransaction | 
                                  EFlagLineFeedCharacter);
                        response =  EKeyWasConsumed;
                        }
                    else
                        {
                        response = HandleNaviEventOutsideInlineEditL(aCode, aLength);
                        if (WesternPredictive() && response == EKeyWasNotConsumed)
                            {
                            // if we're passing a backspace to the editor simulate any old keypress
                            // to catch and check the state of play after the deletion
                            SimulateKeyEventL(EKeyF24);
                            }
                        if (response == EKeyWasNotConsumed)
                            {
                            if(iMode == EZhuyinFind)
                                {
                                SetFlag(EFlagSupressAutoUpdate);
                                }
                            if (iLongClearAfterCloseUI &&
                                aLength == ELongKeyPress)
                                {
                                response = EKeyWasConsumed;
                                }

                            iCaseMan->UpdateCase(EBackspaceEvent);
                            }
#ifdef RD_INTELLIGENT_TEXT_INPUT
                        if(IsAutoCompleteOn())
                            {
                            TInt tailLength = 0;
                            iPtiEngine->HandleCommandL( EPtiCommandGetAutoCompletionTailLength, 
                                                        &tailLength );
                            if(tailLength > 0)
                                {
                                RemoveSuggestedAdvanceCompletionL();
                                }
                            }
#endif
                        }
                    
                    // Clear the star key flag.
                    if( FepUI() )
                        {
                        FepUI()->SetStarKeyFlag( EFalse );
                        }
                    }
#ifdef RD_INTELLIGENT_TEXT_INPUT
                else if (IsAutoCompleteOn())
                    {
                      if(FepUI()->HandleKeyL(aCode, aLength))
                        {
                        if( EPtiKeyboardHalfQwerty == KeyboardLayout()
                        		&& iWesternPredictive
                        		&& iCaseMan->CurrentCase() != EAknEditorUpperCase )
                            {
                        	ClearFlag(CAknFepManager::EFlagChangeInputMode);
                            }
                        TInt tailLength = 0;
                        iPtiEngine->HandleCommandL( EPtiCommandGetAutoCompletionTailLength, &tailLength );
                        if(tailLength > 0)
                            {
                            RemoveSuggestedAdvanceCompletionL();
                            }
                        response = EKeyWasConsumed;
                        }
                    }
                else if(FepUI()->HandleKeyL(aCode, aLength))
                    {
                    response = EKeyWasConsumed;
                    }
                SendEventsToPluginManL( EPluginHideTooltip );
                iExactWordPopupContent->HidePopUp();
#else
                else if (FepUI()->HandleKeyL(aCode, aLength))
                    {
                    response = EKeyWasConsumed;
                    }                
#endif //RD_INTELLIGENT_TEXT_INPUT
                }
            break;
        case EKeyDelete:
#ifdef RD_INTELLIGENT_TEXT_INPUT
			if(IsAutoCompleteOn())
				{
				TInt tailLength = 0;
				iPtiEngine->HandleCommandL( EPtiCommandGetAutoCompletionTailLength, 
												&tailLength );
				if(tailLength > 0)
					{
					RemoveSuggestedAdvanceCompletionL();
				    }
				}

            // When press delete key, if FSQ is opened, 
            // also need to check and close tooltip on it.
			SendEventsToPluginManL( EPluginHideTooltip );

		    iExactWordPopupContent->HidePopUp();
        	if ( IsFlagSet( EFlagInsideInlineEditingTransaction ) )
            	{
            	TryRemoveNoMatchesIndicatorL();
            	UpdateCbaL(NULL);
            	CommitInlineEditL();
    	        } 
#endif //RD_INTELLIGENT_TEXT_INPUT        
        	break;            
        case EKeyF19:  // Fep simulated event to asynchronously update the case.
            iCaseMan->UpdateCase(ENullNaviEvent);
            response=EKeyWasConsumed;
            ClearExtendedFlag(EExtendedFlagOkKeyPressOnCandidatePopup );
            break;
        case EKeyF22:
            if (WesternPredictive())
                {
                iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(iUncommittedText);
                if (CursorInsideWord())
                    {
                    MoveCursorToEndOfWordL();
                    }
                if (iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep())
                    {
                    iCaseMan->UpdateCase(ENullNaviEvent);
                    }
                }
            else if (iMode == ELatin)
                {
                iCaseMan->UpdateCase(ENullNaviEvent);
                }
            ClearCcpuFlag(ECcpuStateIgnoreStarUp);    
            response=EKeyWasConsumed;
            break;
        case EKeyF23:
            if (WesternPredictive())
                {
                iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(iUncommittedText);
                if (CursorInsideWord())
                    {
                    MoveCursorToStartOfWordL();
                    }
                if (iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep())
                    {
                    iCaseMan->UpdateCase(ENullNaviEvent);
                    }
                }
            else if (iMode == ELatin)
                {
                iCaseMan->UpdateCase(ENullNaviEvent);
                }
            ClearCcpuFlag(ECcpuStateIgnoreStarUp);    
            response=EKeyWasConsumed;
            break;
        case EKeyF24:
            if (WesternPredictive() && CursorInsideWord())
                {
                // word concatenation - start the timer; add concatenated word
                // to UDB on timer expiry
                iConcatenationTimer->Start(EWordConcatenationTimeout, EWordConcatenationTimeout,
                    TCallBack(WordConcatenationTimerTimeoutCallback, this));
                }
            response=EKeyWasConsumed;
            break;
        case EPtiKey0: //fall through
        case EPtiKey1:
        case EPtiKey2:
        case EPtiKey3:
        case EPtiKey4:
        case EPtiKey5:
        case EPtiKey6:
        case EPtiKey7:
        case EPtiKey8:
        case EPtiKey9:
            #ifdef RD_SCALABLE_UI_V2     
                if ( aLength == ELongKeyPress && !isPassNext && (aCode == EPtiKey0) )
                    {
                    if( !IsQwerty() && 
                        !IsFlagSet(CAknFepManager::EFlagPassNextKey)&&
                        iFepPluginManager->PluginInputMode() != EPluginInputModeItut )
                        {
                        iFepPluginManager->ClosePluginInputModeL( ETrue );
                        }
                    }
            #endif
             if (IsFlagSet(EFlagLineFeedCharacter) && aCode != EPtiKey0)
                {
                // There is line feed character waiting. Pospone this event and handle
                // line feed first.
                CancelInlineEdit();
                 if (!(iAknEditorFlags & EAknEditorFlagFindPane) || iQwertyInputMode)
                 {
                  SimulateKeyEventL(EKeyEnter);
                  SimulateKeyEventL(EKeyF19); // Asyncronous case update    
                 }
                SimulateKeyEventL((TPtiKey)aCode);                  
                ClearFlag(EFlagLineFeedCharacter);
                return EKeyWasConsumed;                 
                }                
            if (iLanguageCapabilities.iInputLanguageCode == ELangJapanese && iMode != ELatin)
                {
                if (EditorHasFreeSpace() || iFepManState == EAknFepStateUIActive)
                    {
                    if (FepUI()->HandleKeyL(aCode, aLength))
                        {
                        ClearFlag(EFlagChangeInputMode);
                        response = EKeyWasConsumed;
                        }
                    // response is EKeyWasNotConsumed
                    }
                }
            else
                {
                //pass extra key received when doing secret text edit
                if (IsFlagSet(EFlagPassNextKey))
                    {
                    ClearFlag(EFlagPassNextKey);
                    }
                else if (IsFlagSet(EFlagShiftKeyDepressed) || IsCcpuFlagSet(ECcpuStateHashDown))
                    {
                    response = EKeyWasConsumed;
                    }
                    
                // The key event is passed to the editor to handle in case of
                // MFNE and other non FEP aware editors (response = EKeyWasNotConsumed).
                else if ( IsFepAwareTextEditor() )
                    {
                    if ( ( iMode != ENumber && iMode != ENativeNumber ) ||
                        ( ( iMode == ENumber || iMode == ENativeNumber )  && aLength == ELongKeyPress))
                        {
                        // Be default the key event is handled by FEP for FEP aware editors.
                        response = EKeyWasConsumed;
                        }

                    // New character can be entered to the editor if there is
                    // space left in the editor. Editor can be modified also
                    // is the last character is multitapped or last character
                    // in inline state is replaced with a digit.
                    /*Hindi*/
                    //This condition is added for Hindi language in case of multitapping of EKey1 to get 
                    //a numeral value "1" because of a clasical case of Halant and ZWS getting added 
                    //before numeral 1 which may affect incase of limited space in editor   
                    




                    // Is the language Indic?
                    TBool isIndicLanguage = TAknFepUiIndicInputManager::IsIndicLangauge(
                                               TLanguage(iLanguageCapabilities.iInputLanguageCode)); 

                    // Is the state in Multitap of the Inline Text   
                    TBool  isFlagInsideMultitapInlineEditingTransaction = 
                                                 IsFlagSet(EFlagInsideMultitapInlineEditingTransaction);

                    // Is the state of the text inline when long pressing key event occurs
                    TBool  isInlineEditorTransactionLongKeyPress = IsFlagSet(EFlagInsideInlineEditingTransaction)
                                                               && ( aLength == ELongKeyPress );

                    // Is the state of the text inline when current mode is Koeran.
                    TBool isInlineEditorTranscationKoreanMode = IsFlagSet(EFlagInsideInlineEditingTransaction)
                                                                && ( iMode == EHangul ); 

                    // Don't check the free space of the editor 
                    // when current mode is Hangul and current editor state is inline state.
                    if ( EditorHasFreeSpace()
                        || (!WesternPredictive() && !EditorHasFreeSpace() && isIndicLanguage )
                        || isFlagInsideMultitapInlineEditingTransaction
                        || isInlineEditorTransactionLongKeyPress  
                        || isInlineEditorTranscationKoreanMode )
                        {
                        ClearFlag(EFlagEditorFull);

                        if (aLength == ELongKeyPress && 
                           (iMode != ELatin && iMode != ENumber && iMode != ENativeNumber && iMode != EStrokeFind && iMode != EHangul) &&
                            FepUI()->IsValidNumericLongKeyPress(aCode))
                            {
                            if (iFepManState == EAknFepStateInitial) // Multitapping
                                {
#ifdef RD_SCALABLE_UI_V2
                                 if (iFepFullyConstructed && 
                                     iFepPluginManager->CurrentFepInputUI() )
                                     {
                                     if (iFepPluginManager->CurrentFepInputUI()->HandleKeyL(aCode, 
                                                                                         aLength))
                                         {
                                         response = EKeyWasConsumed;
                                         break;
                                         }
                                     }
#endif        
                                // Current multitabbed character is replaced with a number.
                                TChar ch(aCode);
                                TBuf<1> buf;
                                buf.Append(ch);
                                NewCharacterL(buf);
                                CommitInlineEditL();
                                iPtiEngine->ClearCurrentWord();
                                }
                            else
                                {
                                // Long number key press is given to the editor.
                                TryCloseUiL();
                                response = EKeyWasNotConsumed;
                                }
                            }
                        else if (FepUI()->HandleKeyL(aCode, aLength))
                            {
        	            	if ( (iMode == EPinyin || iMode == EZhuyin || iMode == EStroke || iMode == ECangJie)
        	            			&& iFepManState == EAknFepStateUIActive )
        	            		{
        	            		if ( EditorState() )
        	            			{
        	            			EditorState()->SetFlags( EditorState()->Flags() | EAknEditorFlagChinesePopup );
        	            			}       	            		
        	            		}
                            ClearFlag(EFlagChangeInputMode);
                            // for japanese variant
                            if (IsFeatureSupportedJapanese()
                             && iMode == ENumber)
                                {
                                response = EKeyWasConsumed;
                                }
                            }
                        else
                            {
                            //By default, pass long keypress to an editor for inserting number.
                            response = EKeyWasNotConsumed;
                            //No need to handle long key press in dialer
                            if ( RProcess().SecureId().iId != KPhoneSecureId )
                                {

                                if ( iLanguageCapabilities.iLocalDigitType == EDigitTypeArabicIndic )
                                    {
                                    SimulateKeyEventL( aCode + KLatinToArabicIndicDigitsDelta );
                                    response = EKeyWasConsumed;
                                    }
                                else if( iLanguageCapabilities.iLocalDigitType == EDigitTypeEasternArabicIndic )    
                                    {
                                    SimulateKeyEventL( aCode + KLatinToEasternArabicIndicDigitsDelta );
                                    response = EKeyWasConsumed;
                                    }
                                /*Hindi*/    
                                else if ( iLanguageCapabilities.iLocalDigitType == EDigitTypeDevanagari 
                                            || iPreviousEditingState == EDevanagariIndicNumeric )                           
                                    {
                                    
                                    SimulateKeyEventL( aCode + KLatinToDevanagariDigitsDelta );
                                    
                                    response = EKeyWasConsumed;
                                    }
                                }
                            }
                        }
                    }                   
                }
            break;
        case EKeyOK:
            {
            if (iUiInterface->SoftkeysExist() && EditSubmenuInUse()) 
                {                           
                if (iUiInterface->SoftkeyCommandId(ELeftSoftkeyIndex) == EAknFepSoftkeyStartCopy)
                    {                                     
                    ProcessCommandL(EAknFepSoftkeyStartCopy);
                    response = EKeyWasConsumed;             
                    break;
                    }
                else if (iUiInterface->SoftkeyCommandId(ELeftSoftkeyIndex) == EAknFepSoftkeyStartCut)
                    {
                    ProcessCommandL(EAknFepSoftkeyStartCut);
                    response = EKeyWasConsumed;             
                    break;              
                    }
                else if (iUiInterface->SoftkeyCommandId(ELeftSoftkeyIndex) == EEikCmdEditCopy)
                    {                                     
                    ProcessCommandL(EEikCmdEditCopy);
                    response = EKeyWasConsumed;             
                    break;
                    }
                else if (iUiInterface->SoftkeyCommandId(ELeftSoftkeyIndex) == EEikCmdEditCut)
                    {
                    ProcessCommandL(EEikCmdEditCut);
                    response = EKeyWasConsumed;             
                    break;              
                    }                                                                               
                }
            
            if ( (iAknEditorFlags & EAknEditorFlagNoLRNavigation) &&
                IsFlagSet(EFlagShiftKeyDepressed) )
                {
                // Selection key is pressed during shift key press.
                ClearFlag(EFlagNoActionDuringShiftKeyPress);
                return EKeyWasNotConsumed;
                }
            if (iLanguageCapabilities.iInputLanguageCode == ELangJapanese && iMode != ELatin)
                {
                if (FepUI()->HandleKeyL(aCode, aLength))
                    {
                    response = EKeyWasConsumed;
                    }
                }
            else
                {
                if (IsFlagSet(EFlagShiftKeyDepressed) && 
                  !(iAknEditorFlags & EAknEditorFlagSupressShiftMenu))
                    {
                    response = EKeyWasConsumed;
                    }
                else if (iFepManState == EAknFepStateUIActive)
                    {
                    if (FepUI()->HandleKeyL(aCode, aLength))
                        {
                        response = EKeyWasConsumed;
                        }
                    }
                else if (iMode == ELatin && !WesternPredictive() && 
                         IsFlagSet(EFlagInsideMultitapInlineEditingTransaction))                    
                    {
                    CommitInlineEditL();
                    }
#ifdef RD_INTELLIGENT_TEXT_INPUT
			    // Predictive QWERTY (XT9) changes ---->
			    // Accept current candidate (including possible auto-completed part) and insert space after the word
			    // if selection key pressed while inline editing (on Qwerty only!)
			    // this is not the expected behavior for half qwerty
			    
				TBool isQwertyMode = iQwertyInputMode && 
								!( EPtiKeyboardHalfQwerty == KeyboardLayout() );
			    if ( response == EKeyWasNotConsumed && iWesternPredictive && isQwertyMode && 
			         IsFlagSet(EFlagInsideInlineEditingTransaction) 
			         // For addition of ITI features on FSQ. 
			         // If FSQ is opened with ITI features activated, don't commit as HW's action
			         && !( iFepPluginManager && iFepPluginManager->EnableITIOnFSQ() ) )
			        {
			        
					// If the primary candidate setting is exact typing, The tooltip/bubble always shows the best prediction.
				    // Word on the typing line is accepted into editor on selection key event..  
					                  
			        iPtiEngine->CommitCurrentWord();
			        CommitInlineEditL();
                    TryCloseUiL();
                    if(!static_cast<TChar>(NextChar()).IsSpace())
	                    {
	                    SimulateKeyEventL(EKeySpace);
	                    }
                    else 
	                	{
						TCursorSelection sel;
            			iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(sel);
            			sel.iCursorPos++;
            			sel.iAnchorPos++;
            			iInputCapabilities.FepAwareTextEditor()->SetCursorSelectionForFepL(sel);	                		
	                	}    
                    SimulateKeyEventL(EKeyF19); // asynchronous case update
                
			        response = EKeyWasConsumed;
			        }
			    // Predictive QWERTY (XT9) changes <----
#endif //RD_INTELLIGENT_TEXT_INPUT
                }
            }
            break;
#ifdef RD_INTELLIGENT_TEXT_INPUT
        	case EKeyPhoneEnd:
	        	{
	        	// Suggested word should get committed in exact typing
				if ( iWesternPredictive &&
			    	 IsFlagSet(EFlagInsideInlineEditingTransaction) )
			        {
						
					// If the primary candidate setting is exact typing, The tooltip/bubble always shows the best prediction.
					// Word on the typing line is accepted into editor on end key event..
						
			        iPtiEngine->CommitCurrentWord();
					CommitInlineEditL();
	            	TryCloseUiL();
	            	ClearFlag(CAknFepManager::EFlagNoMatches);
	            	SimulateKeyEventL(EKeyF19); // asynchronous case update
			        }
	        	}
			break;
#endif //RD_INTELLIGENT_TEXT_INPUT
        default:
            break;
        }
    SchedulePostEventCheckL(aCode);

#ifdef RD_SCALABLE_UI_V2     
    if ( !isPassNext )
        {
        SendEventsToPluginManL( EPluginKeyEvent, aCode );
        }
#endif
        
    return response;
    }
    
#ifdef RD_INTELLIGENT_TEXT_INPUT
void CAknFepManager::HandleShiftHalfQwertyChineseL(TEventCode aEventCode)
    {
    if (aEventCode == EEventKeyDown)
        {
        ClearCcpuFlag(ECcpuStateCbaShown);		
        SetFlag(EFlagShiftKeyDepressed | EFlagNoActionDuringShiftKeyPress);
        ClearFlag(EFlagNoActionDuringChrKeyPress);
        SetFlag(EFlagQwertyShiftMode);
        // Long press of shift key timer is activated.
        iShiftKeypressMonitor->Start(EShiftKeyRepeatDelay,
            EShiftKeyRepeatDelay, TCallBack(ResetShiftKeyMonitorCallback,
                this) );
        }
    else if (aEventCode == EEventKeyUp)
        {
        ClearFlag(EFlagShiftKeyDepressed
            |EFlagNoActionDuringShiftKeyPress);
        ClearFlag(EFlagQwertyShiftMode);
        iShiftKeypressMonitor->Cancel();

        if ( (iKeyPressedDuringShift)|| IsFlagSet(EFlagLongShiftKeyPress) )
            {
            iKeyPressedDuringShift = EFalse;
            ClearFlag( EFlagLongShiftKeyPress );
            if ( iFnKeyManager->FnKeyState() )
                {
                iFnKeyManager->SetFnKeyState( CAknFepFnKeyManager::EFnKeyNone );
                }
            UpdateIndicators();
            }
        else
            {
            CAknEdwinState* editorState = EditorState();
            TInt permittedCases = editorState->PermittedCases();
            permittedCases = permittedCases == 0 ?
            EAknEditorLowerCase | EAknEditorUpperCase : permittedCases;

            if (iMode == ELatin && (permittedCases & EAknEditorTextCase))
                {
                // Latin case update is changed back to automatic if latin 
                // text case is allowed and the case is changed manually from #-key.
                ClearFlag(CAknFepManager::EFlagSupressAutoUpdate);
                }
            TInt currentCase = iCaseMan->CurrentCase();

            // Is current latin case last available case.
            TBool lastCase = ( ( !(permittedCases & EAknEditorTextCase)
                    && ((currentCase == EAknEditorLowerCase)
                        || (currentCase == EAknEditorUpperCase
                            && !(permittedCases
                                & EAknEditorLowerCase)) )
                    || (editorState->Flags() & EAknEditorFlagFixedCase) )
                || (editorState->Flags()
                    & EAknEditorFlagForceTransparentFepModes) );

            if (iMode == ELatin
                && !IsFlagSet(CAknFepManager::EFlagChangeInputMode)
                && !lastCase)
                {
                if (iWesternPredictive)
                    {
                    // Input mode is still Latin. Only case is updated.
                    if (permittedCases & EAknEditorTextCase)
                        {
                        iCaseMan->ManualCaseUpdate();
                        }
                    else
                        {
                        iCaseMan->SetCurrentCase(EAknEditorLowerCase);
                        }
                    }
                else
                    {
                    if(iCaseMan->CurrentCase() == EAknEditorUpperCase)
                        {
                        iCaseMan->SetCurrentCase(EAknEditorLowerCase);
                        }
                    else
                        {
                        iCaseMan->SetCurrentCase(EAknEditorUpperCase);
                        }
                    }

                // Next time input mode is changed if any text is not entered before.
                SetFlag(CAknFepManager::EFlagChangeInputMode);
                }
            else
                {
                // Input mode is incremented.
                SetChangeModeByShiftAndSpace( EFalse );
                TryIncrementChineseModeForQwertyL(iMode);

                ClearFlag(CAknFepManager::EFlagChangeInputMode);

                if ( iMode == ELatin && iWesternPredictive ) // New mode after Increment.

                    {
                    if (editorState->Flags() & EAknEditorFlagFixedCase)
                        {
                        // Only one fixed case is available. Use it.
                        iCaseMan->ConfigureCaseStateFromEditorState();
                        SetFlag(CAknFepManager::EFlagChangeInputMode);
                        }
                    else if (editorState->Flags()
                        & EAknEditorFlagForceTransparentFepModes)
                        {
                        // Only lower case is used with find pane.
                        iCaseMan->SetCurrentCase(EAknEditorLowerCase);
                        SetFlag(CAknFepManager::EFlagSupressAutoUpdate);
                        SetFlag(CAknFepManager::EFlagChangeInputMode);
                        }
                    else if (permittedCases & EAknEditorTextCase)
                        {
                        iCaseMan->SetCurrentCase(EAknEditorTextCase);
                        iCaseMan->UpdateCase(ENullNaviEvent);
                        }
                    else
                        {
                        if (permittedCases & EAknEditorUpperCase)
                            {
                            iCaseMan->SetCurrentCase(EAknEditorUpperCase);
                            if ( !(permittedCases
                                    & EAknEditorLowerCase))
                                {
                                // We need to move to next input mode since only upper case 
                                // is permitted in Latin input mode.
                                SetFlag(CAknFepManager::EFlagChangeInputMode);
                                }
                            }
                        else
                            {
                            iCaseMan->SetCurrentCase(EAknEditorLowerCase);
                            SetFlag(CAknFepManager::EFlagChangeInputMode);
                            }
                        }
                    }
                }
            }
        }
    }

void CAknFepManager::HandleShiftQwertyChineseL(TEventCode aEventCode)
    {
    if ( IsFlagSet( EFlagQwertyChrKeyDepressed ) && 
    	 !IsFeatureSupportedJapanese() && 
    	 iCurrentFepUI->IsValidShiftKeyPress())
    	{
        ClearFlag(EFlagQwertyChrKeyDepressed | EFlagNoActionDuringChrKeyPress);
        LaunchLanguagesPopupListL();
        return;
    	}
    
    TInt language = iSharedDataInterface->InputTextLanguage();

    if (aEventCode == EEventKeyUp)
        {
        ClearCcpuFlag(ECcpuStateCbaShown);
        if (FepUI())
            {
            FepUI()->HandleKeyL(EKeyLeftShift, EShortKeyPress);
            }
        TBool validShift = iCurrentFepUI->IsValidShiftKeyPress();
                
        if (IsFlagSet(EFlagLongShiftKeyPress) ||iKeyPressedDuringShift)
            {
            if (iKeyPressedDuringShift || IsFlagSet(EFlagQwertyShiftMode))
                {
                iShiftKeypressMonitor->Cancel();
                iKeyPressedDuringShift = EFalse;
                ClearFlag(EFlagQwertyShiftMode);
                }
            }
        else
            if (IsFlagSet(EFlagQwertyShiftMode))
                {
                if ( iMode != ELatin && validShift )
                    {
                    iModeBefore = iMode;
                    TryChangeModeL(ELatin);
                    ClearFlag( EFlagQwertyShiftMode );
                    }
                }
            else
                {
                if (iMode == ELatin)
                    {
                    TBool isWsPr= EFalse;
                    if (WesternPredictive())
                        {
                        SetWesternPredictive(EFalse);
                        isWsPr = ETrue;
                        }
                    TInt Modetemp = iModeBefore;
                    TInt sharedDataMode = iSharedDataInterface->InputMode();
                    if (Modetemp == EStroke && 
                    	iQwertyInputMode && 
                    	sharedDataMode == ECangJie)
                    	{
                    	Modetemp = ECangJie;
                    	}

                    TryChangeModeL(Modetemp);
                    if (isWsPr)
                        {
                        SetWesternPredictive(ETrue);
                        }
                    }
                }

        ClearFlag(EFlagShiftKeyDepressed);

        if (IsFlagSet(EFlagLongShiftKeyPress)
                && iMode == ELatin
                && IsFlagSet(EFlagQwertyShiftMode))//latin shift ->abc
            {
            ClearFlag(EFlagQwertyShiftMode);
            }

        if (IsFlagSet(EFlagLongShiftKeyPress)) //long keypress up
            {
            ClearFlag(EFlagLongShiftKeyPress
                    |EFlagNoActionDuringShiftKeyPress);
            }
        else // short keypress up
            {
            iShiftKeypressMonitor->Cancel();
            }

        ClearFlag(EFlagNoActionDuringChrKeyPress);
        ClearFlag(EFlagNoActionDuringShiftKeyPress);
        if ( validShift )
            {
            UpdateIndicators();
            }
        else
            {
            ClearFlag(EFlagQwertyShiftMode);
            }
        }
    else
        if (aEventCode == EEventKeyDown)
            {

            SetFlag(EFlagShiftKeyDepressed | EFlagNoActionDuringShiftKeyPress);
            ClearFlag(EFlagNoActionDuringChrKeyPress);

            if (IsFlagSet(EFlagQwertyShiftMode))
                {
                ClearFlag(EFlagQwertyShiftMode);
                }
            else
                {
                SetFlag(EFlagQwertyShiftMode);
                if ( iCurrentFepUI->IsValidShiftKeyPress() 
                		|| iMode == ELatin )
                    {
                    UpdateIndicators();
                    }
   	            // If the iShiftKeypressMonitor is actived now, cancel it first.
	              iShiftKeypressMonitor->Cancel();
                // Long press of shift key timer is activated.
                iShiftKeypressMonitor->Start(EShiftKeyRepeatDelay,
                        EShiftKeyRepeatDelay, TCallBack(
                                ResetShiftKeyMonitorCallback, this));
                }
            }
    }
#endif //RD_INTELLIGENT_TEXT_INPUT
TKeyResponse CAknFepManager::HandleShiftKeyEventL(TEventCode aEventCode)
    {
    TKeyResponse response = EKeyWasNotConsumed;
#ifdef RD_INTELLIGENT_TEXT_INPUT
	// GQF clarification 
	// In case irrespective Fn/shift key Text case should not be changed.
	// Like Shift +Fn + Shift should not toggle the text case of the editor.
	// Only Consecutive press of shift key should change the text case
	if( iQwertyInputMode && (EEventKeyDown== aEventCode) && EPtiKeyboardHalfQwerty != KeyboardLayout())
		{
		if( (iFnKeyManager->FnKeyState() == CAknFepFnKeyManager::EFnKeyNext) && 
			IsFlagSet(EFlagQwertyShiftMode))
			{
			iFnKeyManager->ClearFnKeyState();
			iPtiEngine->SetCase((TPtiTextCase)EPtiCaseLower);
			ClearFlag(EFlagQwertyShiftMode);
			}
		}
    TInt language = iSharedDataInterface->InputTextLanguage();
#endif    
    
    if (iQwertyInputMode)
        {
        if ( !iInputCapabilities.FepAwareTextEditor() )
            {
            response = EKeyWasNotConsumed;
            }
#ifdef RD_INTELLIGENT_TEXT_INPUT
        else if (language == ELangTaiwanChinese||
                    language == ELangHongKongChinese ||
                    language == ELangPrcChinese)
                {
                if ( ( EPtiKeyboardHalfQwerty == KeyboardLayout() && FepUI() ) &&
                        iPtiEngine &&
                        ( EPtiCaseFnUpper == iPtiEngine->Case() || EPtiCaseFnLower == iPtiEngine->Case() ) &&
                        ( aEventCode == EEventKeyDown || aEventCode == EEventKeyUp ) )
                    {
                    // Ignore the KeyUp event
                    if( aEventCode == EEventKeyDown )
                        {
                        FepUI()->HandleKeyL( EPtiKeyQwertyLeftShift, 
                                                     EShortKeyPress );
                        }
                    if( aEventCode == EEventKeyUp )
                        {
			            ClearCcpuFlag(ECcpuStateCbaShown);
                        }						
                    }
                else
                    {
                    if ((iAknEditorFlags & EAknEditorFlagFindPane)&& (CAknFepFnKeyManager::EFnKeyNone ==FnKeyState()))
                        {
                        if (aEventCode == EEventKeyUp)
                            {
				            ClearCcpuFlag(ECcpuStateCbaShown);							
                            if ( iKeyPressedDuringShift )
                                {
                                iKeyPressedDuringShift = EFalse;
                                }
                            else
                                {
                                TryIncrementModeChineseQwertyL( iMode );
                                }
                            ClearFlag( EFlagShiftKeyDepressed | EFlagNoActionDuringShiftKeyPress );
                            }
                        else if(aEventCode == EEventKeyDown)
                            {
                            SetFlag(EFlagShiftKeyDepressed | EFlagNoActionDuringShiftKeyPress);
                            ClearFlag(EFlagNoActionDuringChrKeyPress);
                            }
                        return EKeyWasConsumed;
                        }
                    if (EPtiKeyboardHalfQwerty == KeyboardLayout())
                        {
                        HandleShiftHalfQwertyChineseL(aEventCode);
                        }
                    else
                        {
                        HandleShiftQwertyChineseL(aEventCode);
                        }
                    }
                }
#endif // ITI flag
        else if (aEventCode == EEventKeyDown)
            {
#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifdef __HALF_QWERTY_KEYPAD
            if( ( EPtiKeyboardHalfQwerty == KeyboardLayout() && FepUI() ) &&
                iPtiEngine && EditorHasFreeSpace())
                {
		if (IsFlagSet(CAknFepManager::EFlagInsideMultitapInlineEditingTransaction))
                            {
                            iPtiEngine->CancelTimerActivity();
                            }
                if( EPtiCaseFnUpper == iPtiEngine->Case() || EPtiCaseFnLower == iPtiEngine->Case() )
                    {
                    FepUI()->HandleKeyL( EPtiKeyQwertyLeftShift, 
                                                 EShortKeyPress );
					iFnCharInsertedForShift = ETrue;                                                    
                    }                 
                }
#endif // Half Qwerty flag
#endif // ITI flag

            // Shift key is pressed down.
            SetFlag(EFlagShiftKeyDepressed | EFlagNoActionDuringShiftKeyPress);
            ClearFlag(EFlagNoActionDuringChrKeyPress);
            if ( IsFlagSet(EFlagQwertyShiftMode) && IsFlagSet(EFlagNoActionDuringShiftKeyPress) )
                {
                ClearFlag(EFlagQwertyShiftMode | EFlagNoActionDuringShiftKeyPress);
                // If long shift key press flag is set, then do not do manual case update.
                if (!IsFlagSet(EFlagLongShiftKeyPress))
                    {
                    iCaseMan->ManualCaseUpdate();
                    }
                }
            else
                {
                if (!(iCurrentFepUI->IsValidShiftKeyPress()))
                    {
                    return EKeyWasConsumed;
                    }
#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifdef __HALF_QWERTY_KEYPAD                    
                if(!iFnCharInsertedForShift)
#endif //RD_INTELLIGENT_TEXT_INPUT
#endif //__HALF_QWERTY_KEYPAD                
                	{
                	SetFlag(EFlagQwertyShiftMode);
                	}
					
                if(iShiftKeypressMonitor->IsActive())
                	iShiftKeypressMonitor->Cancel();               	
                // Long press of shift key timer is activated.
                iShiftKeypressMonitor->Start(EShiftKeyRepeatDelay, EShiftKeyRepeatDelay, 
                    TCallBack(ResetShiftKeyMonitorCallback, this));
#ifdef RD_INTELLIGENT_TEXT_INPUT
                // Shift kkey hack for GQF implementation. Need to check 
                // if it has any side effect.  
                if((iFnKeyManager->FnKeyState() != CAknFepFnKeyManager::EFnKeyLock)
                    && (iFnKeyManager->FnKeyState() != CAknFepFnKeyManager::EFnKeyForced)
                    && (iFnKeyManager->FnKeyState() != CAknFepFnKeyManager::EFnKeyDown)
                    && (iFnKeyManager->FnKeyState() != CAknFepFnKeyManager::EFnKeyNext))
#endif
                UpdateIndicators();
                }
            }
        else if (aEventCode == EEventKeyUp)
            {
#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifdef __HALF_QWERTY_KEYPAD            
            if( ( EPtiKeyboardHalfQwerty == KeyboardLayout() && FepUI() ) &&
                iPtiEngine )
                {
                ClearCcpuFlag(ECcpuStateHashDown
                      | ECcpuStateHashKeyDeleteDone
                      | ECcpuStateChangeToPredictionMode);
                if (IsCcpuFlagSet(ECcpuStateEdwinInSelectionMode))
                    {
                    SetCcpuFlag(ECcpuStateSelectionEventPosted);
                    ClearCcpuFlag(ECcpuStateEdwinInSelectionMode);
                    SetFlag(EFlagLongShiftKeyPress);

                    TKeyEvent ccpuStart = {0, EStdKeyLeftShift, 0, 0};
                    CCoeEnv::Static()->SimulateKeyEventL(ccpuStart, aEventCode);
                    }
                  if( CAknFepFnKeyManager::EFnKeyNone != FnKeyState())
                      {
                      ClearFlag(EFlagQwertyShiftMode);
                      }
                //  iFnCharInsertedForShift = EFalse;    
                  }
                
#endif // Half Qwerty flag
#endif // ITI flag

            ClearFlag(EFlagShiftKeyDepressed);
            ClearCcpuFlag(ECcpuStateCbaShown);
            if ( !IsFlagSet(EFlagNoActionDuringShiftKeyPress) ||
                (IsFlagSet(EFlagNoActionDuringShiftKeyPress) && IsFlagSet(EFlagLongShiftKeyPress)) )
                {
                ClearFlag(EFlagQwertyShiftMode);
                }
#ifdef RD_INTELLIGENT_TEXT_INPUT 
#ifdef __HALF_QWERTY_KEYPAD				          
            if( IsFlagSet(EFlagQwertyChrKeyDepressed) && IsFlagSet(EFlagNoActionDuringShiftKeyPress))
                {
#ifdef FF_DUAL_LANGUAGE_SUPPORT                    
                 // Currently dual language feature would work correctly for ITUT and Half Qwerty products 
				 // Hence the dual language settings should also be displayed only for ITUT and Halfqwerty products
                if(iKeyboardType == EPtiKeyboardHalfQwerty )
                    LaunchDualLanguageSettingDialogL();
                else                     
#endif //FF_DUAL_LANGUAGE_SUPPORT
                    LaunchLanguagesPopupListL();
                ClearFlag(EFlagQwertyChrKeyDepressed|EFlagNoActionDuringChrKeyPress|EFlagNoActionDuringShiftKeyPress);                
                }
               
#ifdef __SHIFT_KEY_LOOP                                
            if( KeyboardLayout() == EPtiKeyboardHalfQwerty &&
            // looping does not happen for long key press
                !IsFlagSet(EFlagLongShiftKeyPress) &&     
            // looping does not happen in functionized state                
                iFnKeyManager->FnKeyState() == CAknFepFnKeyManager::EFnKeyNone &&
            // looping happens only when no action 
            // has happened when the shift key has been pressed     
                 IsFlagSet(EFlagNoActionDuringShiftKeyPress) && !iFnCharInsertedForShift )
                {
                HandleKeyEventL( EStdKeyLeftShift, EShortKeyPress );
                ClearFlag(EFlagQwertyShiftMode);
                }
        	iFnCharInsertedForShift = EFalse;        
#endif //__SHIFT_KEY_LOOP       
#endif //__HALF_QWERTY_KEYPAD      
#endif //RD_INTELLIGENT_TEXT_INPUT   
            if (IsFlagSet(EFlagLongShiftKeyPress))  //long keypress up
                {
                ClearFlag(EFlagLongShiftKeyPress);
                }
            else                                    // short keypress up
                {
                iShiftKeypressMonitor->Cancel();
#ifdef RD_INTELLIGENT_TEXT_INPUT 
#ifdef __HALF_QWERTY_KEYPAD               
#ifdef __SHIFT_KEY_LOOP
				// Cancel multitap timer
                
                if( iPtiEngine != NULL )
                    {
				    iPtiEngine->CancelTimerActivity();
                    }
#endif //__SHIFT_KEY_LOOP       
#endif //__HALF_QWERTY_KEYPAD      
#endif //RD_INTELLIGENT_TEXT_INPUT
                }
            ClearFlag( EFlagNoActionDuringShiftKeyPress );
#ifdef RD_INTELLIGENT_TEXT_INPUT
            // Shift kkey hack for GQF implementation. Need to check 
                // if it has any side effect.  
            if((iFnKeyManager->FnKeyState() != CAknFepFnKeyManager::EFnKeyLock)
                && (iFnKeyManager->FnKeyState() != CAknFepFnKeyManager::EFnKeyForced)
                && (iFnKeyManager->FnKeyState() != CAknFepFnKeyManager::EFnKeyDown)
                &&(iFnKeyManager->FnKeyState() != CAknFepFnKeyManager::EFnKeyNext))
#endif
            UpdateIndicators();
            }
        }
    else
        {
        if ( !iInputCapabilities.FepAwareTextEditor() && (!iCandidatePopup))
            {
            response = EKeyWasNotConsumed;
            }        
        else if (aEventCode == EEventKeyDown)
            {
            if (Japanese() && iInputCapabilities.FepAwareTextEditor())
                {
                if (!(iCurrentFepUI->IsValidShiftKeyPress()))
                    {
                    return EKeyWasConsumed;
                    }
                TryCloseUiL();
                }
            
            // Shift key is pressed down but no any other key is pressed.
            ClearCcpuFlag(ECcpuSupressEditMenuFromShiftUp);
            SetFlag(EFlagShiftKeyDepressed | EFlagNoActionDuringShiftKeyPress);
            if ( !(iAknEditorFlags & EAknEditorFlagNoLRNavigation))
                {
                // Cancel the timer first if active.
                // EEventKeyUp is not sent always for shift key during debugging on emulator.
                iShiftKeypressMonitor->Cancel();
                // Long press of shift key timer is activated.
                iShiftKeypressMonitor->Start(EShiftKeyRepeatDelay, EShiftKeyRepeatDelay,
                    TCallBack(ResetShiftKeyMonitorCallback, this));
                }
            if ( IsFlagSet(EFlagInsideInlineEditingTransaction) &&
                WesternPredictive() && iFepManState == EAknFepStateUIActive )
                {
                TryRemoveNoMatchesIndicatorL();
                UpdateCbaL(NULL);
                }
            }
        else if (aEventCode == EEventKeyUp)
            {
            ClearFlag(EFlagShiftKeyDepressed);
            ClearCcpuFlag(ECcpuStateCbaShown);
            if (IsFlagSet(EFlagLongShiftKeyPress))  //long keypress up
                {
                ClearFlag(EFlagLongShiftKeyPress | EFlagNoActionDuringShiftKeyPress);
                // Shift long key press timer started, in between release of
                // Shift key should cancel active long shift key timer. 
                iShiftKeypressMonitor->Cancel();
                if (!iInputCapabilities.FepAwareTextEditor())
                    {
                    TKeyEvent ccpuStart = {0, EStdKeyLeftShift, 0, 0};
                    CCoeEnv::Static()->SimulateKeyEventL(ccpuStart, EEventKeyUp);                    
                    }
                }
            else                                    // short keypress up
                {
                iShiftKeypressMonitor->Cancel();
                if (!(iAknEditorFlags & EAknEditorFlagSupressShiftMenu) &&
                    IsFlagSet(EFlagNoActionDuringShiftKeyPress) && //iCurrentFepUI->IsValidShiftKeyPress() &&
                    !IsCcpuFlagSet(ECcpuSupressEditMenuFromShiftUp))
                    {
                    // Shift key is released before any other key is pressed.
                    // Edit menu is launched if it is allowed.
                    ClearFlag(EFlagNoActionDuringShiftKeyPress);
                    //Check if pointer event drag is not there before launch of edit menu.
                    if( !IsExtendedFlagSet(EExtendedFlagPointerEventTypeEDrag))
                      	{
                    	LaunchSelectModeMenuL();
                    	}
                   
                    if (iInputCapabilities.FepAwareTextEditor())
                        {
                        response = EKeyWasConsumed;    
                        }
                    }
                }
            }
        }
#ifdef RD_INTELLIGENT_TEXT_INPUT
    if (aEventCode == EEventKeyUp && 
        EPtiKeyboardQwerty4x10 == KeyboardLayout() && FepUI())
	    {
	    FepUI()->HandleKeyL(EKeyLeftShift, EShortKeyPress);
	    }
#endif	    
    return response;
    }

TKeyResponse CAknFepManager::HandleThai0KeyEvent(TEventCode aEventCode, TKeyPressLength aLength, 
                                                 TBool& aThai0KeyHandling)
    {
    TKeyResponse response = EKeyWasNotConsumed;
    aThai0KeyHandling = EFalse;
    if(iQwertyInputMode)
        {
        return response;    
        }
   
    if ( iLanguageCapabilities.iInputLanguageCode == ELangThai && 
         iMode != ENumber && iMode != ENativeNumber )
        {
        if (!IsPredictive())
            {
            if (aEventCode != EEventKeyUp)
                {
                response = EKeyWasConsumed;
                if ( aLength == ELongKeyPress)
                    {
                    if (!iLongThai0Key)
                        {
                        iLongThai0Key = ETrue;
                        response = EKeyWasNotConsumed; // 0 to editor
                        aThai0KeyHandling = ETrue;
                        }
                    }
                }

            else if (aEventCode == EEventKeyUp)
                {
                if (iLongThai0Key)
                    {
                    iLongThai0Key = EFalse;
                    response = EKeyWasConsumed;
                    }
                else
                    {
                    response = EKeyWasNotConsumed; // Show Thai 0 key SCT
                    aThai0KeyHandling = ETrue;
                    }
                }
            }
        }
    return response;
    }

TBool CAknFepManager::HandleQwertyChrKeyEventL(TEventCode aEventCode)
    {
    TBool response = EFalse;
    if ( !iQwertyInputMode || !iInputCapabilities.FepAwareTextEditor() )
        {
        return response;
        }
#ifdef RD_INTELLIGENT_TEXT_INPUT
    if(IsFlagSet(EFlagShiftKeyDepressed))
        {
        iKeyPressedDuringShift = ETrue;
        }
#ifdef __HALF_QWERTY_KEYPAD
    
        if( (aEventCode == EEventKeyUp) )
            {
            ClearFlag(EFlagQwertyChrKeyDepressed);
            }
        // Handle shift for Half Qwerty seperately
        if( ( EPtiKeyboardHalfQwerty == KeyboardLayout() && FepUI() ) &&
            iPtiEngine /*&&             
            ( aEventCode == EEventKeyDown || aEventCode == EEventKeyUp ) */ && !IsFlagSet(EFlagShiftKeyDepressed))
            {		

			if( HandleChrKeyForHalfQwertyL(aEventCode) )
				{
				return ETrue;
				}							
            }
#endif // Half Qwerty flag 
        // This part of code for error fixing.
        // whenever fep is Fnupper state, press of "CHR" key
        // always launch SCT table.
        if( (iFnKeyManager->FnKeyState() != CAknFepFnKeyManager::EFnKeyNone)&&
            (IsFlagSet(EFlagQwertyShiftMode) || IsFlagSet(EFlagLongShiftKeyPress)))
            {
            ClearFlag(EFlagNoActionDuringShiftKeyPress | EFlagLongShiftKeyPress | EFlagShiftKeyDepressed | EFlagQwertyShiftMode);
            iShiftKeypressMonitor->Cancel();  
            SetFlag(EFlagQwertyChrKeyDepressed | EFlagNoActionDuringChrKeyPress);
            return response;
            }
           

#endif // ITI flag
    if (aEventCode == EEventKeyDown)
        {
        SetFlag(EFlagQwertyChrKeyDepressed | EFlagNoActionDuringChrKeyPress);
        response = EFalse;

        if (IsFeatureSupportedJapanese() || 
            ( IsChineseInputLanguage() 
#ifdef RD_INTELLIGENT_TEXT_INPUT        
            && KeyboardLayout() == EPtiKeyboardHalfQwerty
#endif //RD_INTELLIGENT_TEXT_INPUT
            ) )
            {
            // start timer for Chr key long pressing
            iChrKeypressMonitor->Start(EChrKeyRepeatDelay, EChrKeyRepeatDelay, 
                TCallBack(HandleChrKeyMonitorCallback, this));
            }
        }
    else if (aEventCode == EEventKeyUp)
        {
        if (iChrKeypressMonitor->IsActive())
            {
            iChrKeypressMonitor->Cancel();
            }

        if (IsFlagSet(EFlagShiftKeyDepressed) && IsFlagSet(EFlagNoActionDuringChrKeyPress)
#ifdef RD_INTELLIGENT_TEXT_INPUT
		 	||
            IsFlagSet(EFlagQwertyShiftMode)
#endif            	
			)
            {
            ClearFlag(EFlagNoActionDuringShiftKeyPress | EFlagLongShiftKeyPress | 
                      EFlagShiftKeyDepressed | EFlagQwertyShiftMode |EFlagQwertyChrKeyDepressed);
            iShiftKeypressMonitor->Cancel();
#ifdef RD_INTELLIGENT_TEXT_INPUT
            iKeyPressedDuringShift = EFalse;
#endif                 
            UpdateIndicators();
            // In Japanese variant, the selecting input language is unnecessary.
#ifdef RD_INTELLIGENT_TEXT_INPUT
            if (EPtiKeyboardQwerty4x10 == KeyboardLayout() || EPtiKeyboardHalfQwerty == KeyboardLayout())
                {
            	if (!IsFeatureSupportedJapanese() && iCurrentFepUI->IsValidShiftKeyPress())
	                {
#ifdef FF_DUAL_LANGUAGE_SUPPORT	                
					// Currently dual language feature would work correctly for ITUT and Half Qwerty products 
	                // Hence the dual language settings should also be displayed only for ITUT and Halfqwerty products
	                if(iKeyboardType == EPtiKeyboard12Key || iKeyboardType == EPtiKeyboardHalfQwerty )
                        LaunchDualLanguageSettingDialogL();
                    else                      
#endif //FF_DUAL_LANGUAGE_SUPPORT
                    LaunchLanguagesPopupListL();
	                }
                }
            else
            	{
#endif            	
            if (!IsFeatureSupportedJapanese() && iCurrentFepUI->IsValidShiftKeyPress())
                {
                LaunchLanguagesPopupListL();
	                }
#ifdef RD_INTELLIGENT_TEXT_INPUT	                
                }
#endif                
            }
        else if (IsFlagSet(EFlagNoActionDuringChrKeyPress))
            {
            // pass key to Chinese UI manager in Chinese qwerty input for sct table
            if(IsChineseInputLanguage() && iMode != ELatin)
                {
                FepUI()->HandleKeyL(EStdKeyLeftFunc, EShortKeyPress);
                ClearFlag( EFlagQwertyChrKeyDepressed | EFlagNoActionDuringChrKeyPress );
                }
            else if (Japanese())
                {
                if (iMode != EHiragana)
                    {
                    // Commit characters
#ifdef RD_INTELLIGENT_TEXT_INPUT                    
                	// Set the iSupressCursorMoveToEndChrKeyPressed when cursor is in 
					// between two words and SCT is launched
                    iSupressCursorMoveToEndChrKeyPressed = ETrue;
#endif //RD_INTELLIGENT_TEXT_INPUT                    
                    HandleChangeInFocus();
                    LaunchSpecialCharacterTableL();
                    }
                }
            else if ( IsAbleToLaunchSCT() )
                {                
#ifdef RD_INTELLIGENT_TEXT_INPUT                
               	// Set the iSupressCursorMoveToEndChrKeyPressed when cursor is in 
				// between two words and SCT is launched
                iSupressCursorMoveToEndChrKeyPressed = ETrue;
#endif //RD_INTELLIGENT_TEXT_INPUT                
                HandleChangeInFocus();
                LaunchSpecialCharacterTableL();
                }
            else if(IsFlagSet(EFlagQwertyChrKeyDepressed))
            	{
					 ClearFlag(EFlagQwertyChrKeyDepressed);
            	}
            }
        else
            {
            FepUI()->ExpireMultitapTimer();
            ClearFlag(EFlagQwertyChrKeyDepressed | EFlagNoActionDuringChrKeyPress);
            
            response = ETrue;
            } 
        }
    return response;
    }
    
#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifdef __HALF_QWERTY_KEYPAD    
TKeyResponse CAknFepManager::HandleChrKeyForHalfQwertyL( TEventCode aEventCode)
	{
	if ((IsFlagSet(CAknFepManager::EFlagInsideMultitapInlineEditingTransaction)) && !(EPtiCaseFnUpper == iPtiEngine->Case() || EPtiCaseFnLower == iPtiEngine->Case()))
        {
        iPtiEngine->CancelTimerActivity();
        }
	// in functionized mode the chr key is mapped to specific chars
	// and should therefore be handled by the states
	if ( EPtiCaseFnUpper == iPtiEngine->Case() || EPtiCaseFnLower == iPtiEngine->Case() || IsReverseFnkeyInput())
		{		
		if( aEventCode == EEventKeyDown && ( EditorHasFreeSpace() ||
    		  IsFlagSet( EFlagInsideMultitapInlineEditingTransaction )))
			{
			FepUI()->HandleKeyL( EStdKeyLeftFunc, 
										 EShortKeyPress );			
			}
		return EKeyWasConsumed;
		}
	// in non-functionized predictive mode,
	// a long key press of the chr key should produce the SCT
	// a short key press should produce the candidate list
	if ( ( iWesternPredictive || IsChineseInputLanguage() ) /*&& 
	   ( iMode != EPinyin && iMode != EStroke && iMode != EZhuyin )*/ )
		{						
		
		if(!iChrKeypressMonitor->IsActive())
			{
			// this monitors long key presses of chr key
			// the framework does not give us the iRepeats info 
			// for chr key for some reason
			iChrKeypressMonitor->Start(EHalfQwertyChrKeyRepeatDelay, EHalfQwertyChrKeyRepeatDelay, 
                TCallBack(HandleChrKeyMonitorCallback, this));			
			}

		if( aEventCode != EEventKeyUp )
			{
			SetFlag( EFlagQwertyChrKeyDepressed | EFlagNoActionDuringChrKeyPress);
			return EKeyWasConsumed;
			}

		if ( aEventCode == EEventKeyUp && IsFlagSet(EFlagNoActionDuringChrKeyPress) )
			{
			if(!iChrLongKeyPress )
				{							
				// if we have come here it's a short press
				// the flag EFlagNoActionDuringChrKeyPress is cleared in the timer callback
				if(!IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction) || IsFlagSet(CAknFepManager::EFlagInsideMultitapInlineEditingTransaction))
					{
					// when not in inline editing mode,
					// chr key should produce the SCT
					if ( !IsChineseInputLanguage() )
						{
						LaunchSpecialCharacterTableL();	
						}
					else
						{
						FepUI()->HandleKeyL( EStdKeyLeftFunc, 
								 EShortKeyPress );
						}
					}				
				ClearFlag( EFlagQwertyChrKeyDepressed | EFlagNoActionDuringChrKeyPress);			

				iChrKeypressMonitor->Cancel();			

				TInt wordToFocus = KWordToFocusInCandidateList;
				TInt numberOfCandidates = iPtiEngine->NumberOfCandidates();
				if (numberOfCandidates > 1)
				{
				wordToFocus = KWordToFocusInCandidateList;
				}
				LaunchCandidatePopupListL(wordToFocus);

				return EKeyWasConsumed;
				}
			else 
				{

		    	
		    	// Cancel the timer    	
		    	iChrKeypressMonitor->Cancel();
		    	
		    	// commit the characters
	        	HandleChangeInFocus();

		    	// Clear the chr key depressed flag
		    	ClearFlag( EFlagQwertyChrKeyDepressed | EFlagNoActionDuringChrKeyPress);
		        // launch the special character table
		        iChrLongKeyPress = EFalse;
		        
		        if ( IsAbleToLaunchSCT() )
		            {
		            LaunchSpecialCharacterTableL();
		            }

		        return EKeyWasConsumed;
				}	
			}
		else
			{
			return EKeyWasNotConsumed;
			}
		}
	return EKeyWasNotConsumed;
	}			
#endif //__HALF_QWERTY_KEYPAD 
#endif //RD_INTELLIGENT_TEXT_INPUT				

TKeyResponse CAknFepManager::HandleQwertyControlKeyEventL(const TKeyEvent& aKeyEvent, 
                                                          TEventCode aEventCode)
    {
    TKeyResponse response = EKeyWasConsumed;
#ifdef RD_INTELLIGENT_TEXT_INPUT
     if (EPtiKeyboardQwerty4x10 == KeyboardLayout())
     {
     	if( iCurrentFepUI && !iCurrentFepUI->IsValidShiftKeyPress() && aEventCode == EEventKeyUp )
     	{
     	    FepUI()->HandleKeyL(EKeyLeftCtrl, EShortKeyPress);    
     		return EKeyWasConsumed;
     	}
     }
#endif     

    // need to ignore [Ctrl] + the following simbol keys
    // because FEP doesn't consume [Ctrl] combination keys.
    if (Japanese() && aEventCode == EEventKey)
        {
        TBool comsume = EFalse;
        switch (aKeyEvent.iScanCode)
            {
            case EStdKeyNkpPlus:
            case EPtiKeyQwertyPlus:
            case EPtiKeyQwertyMinus:
            case EPtiKeyQwertyComma:
            case EPtiKeyQwertySemicolon:
            case EPtiKeyQwertyFullstop:
            case EPtiKeyQwertyHash:
            case EPtiKeyQwertySlash:
            case EPtiKeyQwertyApostrophe:
            case EPtiKeyQwertySpace:
                comsume = ETrue;
                break;
            default:
                break;
            }
        if (comsume)
            {
            return EKeyWasConsumed;
            }
        }

#ifdef RD_SCALABLE_UI_V2
    if (aKeyEvent.iModifiers & EModifierCtrl
    && (aKeyEvent.iScanCode == EPtiKeyQwertyC || aKeyEvent.iScanCode == EPtiKeyQwertyV || 
        aKeyEvent.iScanCode == EPtiKeyQwertyX))
        {
        SendEventsToPluginManL( EPluginSyncFepAwareText );
        // Let copy / cut / paste events to flow through.
        response = EKeyWasNotConsumed;
        }          
#endif         
#ifdef RD_INTELLIGENT_TEXT_INPUT
    // Predictive QWERTY (XT9) changes ---->
    // Ctrl combinations which affect the editor state, must commit the current inline edit.
    // Otherwise the currently active inline word might be lost.
    if ( iWesternPredictive && iQwertyInputMode && IsFlagSet(EFlagInsideInlineEditingTransaction) )
        {
        switch (aKeyEvent.iScanCode)
            {
            case EStdKeyUpArrow:
            case EStdKeyDownArrow:
            case EStdKeyLeftArrow:
            case EStdKeyRightArrow:
            case EPtiKeyQwertyC:
            case EPtiKeyQwertyV:
            case EPtiKeyQwertyX:
            case EPtiKeyQwertyA:
                iPtiEngine->CommitCurrentWord();
                TryCloseUiL();        
                break;
            default:
                break;
            }
        }
    // Predictive QWERTY (XT9) changes <----
#endif //RD_INTELLIGENT_TEXT_INPUT

    if (aEventCode != EEventKeyUp)
        {
        return EKeyWasNotConsumed;
        }
    
    if(iQwertyInputMode)
        {

#ifdef RD_INTELLIGENT_TEXT_INPUT
        // Query ptiEngine to know if the scancode maps to EPtiKeyQwertyC, 
        // EPtiKeyQwertyV or EPtiKeyQwertyX.
        // we should not always rely on scancode as products may change it, while
        // still having the 'C', 'V' and 'X' mappings
        
        TBuf<32> mapData;              
        iPtiEngine->MappingDataForKey((TPtiKey)aKeyEvent.iScanCode, mapData, EPtiCaseUpper); 
        
        if( mapData.Length() > 0 )    
        	{
        	if( mapData[0] == EPtiKeyQwertyC || mapData[0] == EPtiKeyQwertyV ||
        	mapData[0] == EPtiKeyQwertyX )
        		{
        		// Let copy / cut / paste events to flow through.
            	response = EKeyWasNotConsumed;
        		}
        	}
#else     
        if (aKeyEvent.iScanCode == EPtiKeyQwertyC || aKeyEvent.iScanCode == EPtiKeyQwertyV ||
            aKeyEvent.iScanCode == EPtiKeyQwertyX)
            {
            // Let copy / cut / paste events to flow through.
            response = EKeyWasNotConsumed;
            }
#endif
            
            
        if (aKeyEvent.iScanCode == EStdKeyRightShift || aKeyEvent.iScanCode == EStdKeyLeftShift
#ifdef RD_INTELLIGENT_TEXT_INPUT
			||
            IsFlagSet(EFlagQwertyShiftMode)
#endif //RD_INTELLIGENT_TEXT_INPUT
			)
            {
            if(!iInputCapabilities.FepAwareTextEditor())
                {
                response = EKeyWasNotConsumed;
                }
            else
                {
                SendEventsToPluginManL( EPluginCloseMode, EFalse );
                if ( iMode != ENumber && iMode!= ENativeNumber )
                    {
                    iModeBefore = iMode;
                    }
#ifdef RD_HINDI_PHONETIC_INPUT  
                           
                if(iSharedDataInterface->InputTextLanguage() == KLangHindiPhonetic)    
                    {
                    TryChangePhoneticModeL();   
                    }
                else
                    {
#endif                  
                    if((!(IsOnlyNumericPermitted() || 
                         (iAknEditorFlags & EAknEditorFlagLatinInputModesOnly))) &&
                       TAknFepUiIndicInputManager::IsIndicLangauge(
                         (TLanguage)iSharedDataInterface->InputTextLanguage()))
                        {
                        if(iLanguageCapabilities.iInputLanguageCode == ELangEnglish)
                            {
                            ChangeInputLanguageL(iSharedDataInterface->InputTextLanguage());
                            }
                        else
                            {
                            ChangeInputLanguageL(ELangEnglish); 
                            }   
                        }
                    else
                        {
                        TryIncrementModeChineseQwertyL(iMode);  
                        }  
#ifdef RD_HINDI_PHONETIC_INPUT        
                    }   
#endif
                }
            ClearFlag(EFlagShiftKeyDepressed);
            if ( !IsFlagSet(EFlagNoActionDuringShiftKeyPress) ||
                (IsFlagSet(EFlagNoActionDuringShiftKeyPress) && IsFlagSet(EFlagLongShiftKeyPress)) )
                {
                ClearFlag(EFlagQwertyShiftMode);
                }
            if (IsFlagSet(EFlagLongShiftKeyPress))  //long keypress up
                {
                ClearFlag(EFlagLongShiftKeyPress);
                }
            else                                    // short keypress up
                {
                iShiftKeypressMonitor->Cancel();
                }
            ClearFlag( EFlagNoActionDuringShiftKeyPress );
            UpdateIndicators();
            }
        else if(aKeyEvent.iScanCode == EStdKeySpace) // switch between last used Chinese input mode and English
            {
#ifdef RD_INTELLIGENT_TEXT_INPUT
            // Predictive QWERTY (XT9) changes ---->
            // Toggle the predictive mode with ctrl+space
            if ( (iPermittedInputModes & EAknEditorTextInputMode) &&
                 !(iAknEditorFlags & EAknEditorFlagNoT9) &&
                 iMode == ELatin &&
                 iLanguageCapabilities.iSupportsWesternQwertyPredictive )
                {
                if (iWesternPredictive)
                    {
                    //ProcessCommandL(EAknCmdT9PredictiveT9Off);
                    SendEventsToPluginManL( EPluginCloseMode, EFalse );
                    //HandleChangeInFocus();                    
                    if (iMode != ELatin)
                        {
                        TryChangeModeL(ELatin);
                        }
                    else
                        {
#ifdef RD_INTELLIGENT_TEXT_INPUT
                        RemoveSuggestedAdvanceCompletionL();
#endif // RD_INTELLIGENT_TEXT_INPUT
                        CommitInlineEditL();
                        }
                    SetCcpuFlag(ECcpuStataCommitPredictiveWord);
                    SetWesternPredictive(EFalse);
                    HandleChangeInFocus();
                    }
                else
                    {
                    //ProcessCommandL(EAknCmdMultitapPredictiveT9On);
                    SendEventsToPluginManL( EPluginCloseMode, EFalse );
                    //HandleChangeInFocus();
                    SetWesternPredictive(ETrue);
                    TryCloseUiL();
                    TryChangeModeL(ELatin);
     				}              
                // Reset the function key state,if predictive mode is cahnged.
				if (iFnKeyManager)
					{
					iFnKeyManager->ClearFnKeyState();
					}
                }
            else // Predictive QWERTY (XT9) changes <----
                {
#endif //RD_INTELLIGENT_TEXT_INPUT
            SendEventsToPluginManL( EPluginCloseMode, EFalse );
            if(iMode != iModeBefore && iMode != ENumber && iMode != ENativeNumber )
                {
                TInt mode = iMode;
                if(iMode != ELatin && iModeBefore != ELatin) // more than one Chinese input modes
                    {
                    TryChangeModeL(ELatin);
                    }
                else
                    {
                    TryChangeToModeBeforeL();
                    }
                iModeBefore = mode; // deposit previous input mode
                }
#ifdef RD_INTELLIGENT_TEXT_INPUT
            }
#endif //RD_INTELLIGENT_TEXT_INPUT
            } 
#ifdef RD_INTELLIGENT_TEXT_INPUT //// Predictive QWERTY (XT9) changes: open the edit menu with Fn+space ---->
        else if (aKeyEvent.iScanCode == EStdKeyLeftFunc)
            {
            LaunchSelectModeMenuL();
            } // Predictive QWERTY (XT9) changes <----
#endif //RD_INTELLIGENT_TEXT_INPUT
        }       

    if (IsCcpuFlagSet(ECcpuStatePosted))           
        {
#ifdef RD_INTELLIGENT_TEXT_INPUT
        // Query ptiEngine to know if the scancode maps to EPtiKeyQwertyC, 
        // EPtiKeyQwertyV or EPtiKeyQwertyX.
        // we should not always rely on scancode as products may change it, while
        // still having the 'C', 'V' and 'X' mappings
        
        TBuf<32> mapData;              
        iPtiEngine->MappingDataForKey((TPtiKey)aKeyEvent.iScanCode, mapData, EPtiCaseUpper); 
        
        if( mapData.Length() > 0 )    
        	{
        	if( mapData[0] == EPtiKeyQwertyC || mapData[0] == EPtiKeyQwertyX )
        		{
	            // Let copy / cut events to flow through.
	            response = EKeyWasNotConsumed;
	            }
        	}
#else
        if (aKeyEvent.iScanCode == EPtiKeyQwertyC || aKeyEvent.iScanCode == EPtiKeyQwertyX)          
            {
            // Let copy / cut events to flow through.
            response = EKeyWasNotConsumed;
            }        
#endif   
        ResetCcpuFlags();               
        }

/*    if (EPtiKeyboardQwerty4x10 == KeyboardLayout())
    	{
    	FepUI()->HandleKeyL(EKeyLeftCtrl, EShortKeyPress);    
   	}*/

    //The default value of response is EKeyWasConsumed. So in some case, fep don¡¯t 
    //do anything, but it return Consumed.
    //This make app miss the event EEventKeyUp. 
    //As per the logic of the function, the combine about ctrl key including:
    //       Ctrl key + C; Ctrl key + V; Ctrl + X
    //       Ctrl key + Shift key
    //       Ctrl key + space
    //       Ctrl key + Function key
    //In above cases, it return EKeyWasConsumed, it is correct.
    //The function for Ctrl Key as pressing the only one key ctrl at one time is to 
    //do something if the flag EFlagQwertyShiftMode is set
    //In this case, it return EKeyWasConsumed, it is correct.
    //In other case, it should return EKeyWasNotConsumed
    if( ( aKeyEvent.iScanCode == EStdKeyRightCtrl || aKeyEvent.iScanCode==EStdKeyLeftCtrl )
        && !IsFlagSet(EFlagQwertyShiftMode) )
        {
        response = EKeyWasNotConsumed;
        }

    return response;
    }

TBool CAknFepManager::HandleQwertyKeyEventL(const TKeyEvent& aKeyEvent, TKeyResponse& aResponse)
    {
    TBool keyHandled = EFalse;
    aResponse = EKeyWasNotConsumed;
#ifdef RD_INTELLIGENT_TEXT_INPUT 
    if(iKeyBackSpaceHit && (EKeyBackspace != aKeyEvent.iCode || EStdKeyBackspace != aKeyEvent.iScanCode))
        	iKeyBackSpaceHit = 0;
        
    if(IsMfneEditor())
        {
        // multi-field editors need the key event to be sent to them directly
        // therefore not consuming it here
        return EFalse;
        }

    if(IsFlagSet(EFlagShiftKeyDepressed))
        {
        iKeyPressedDuringShift = ETrue; 
        }  
  // This part of code for error fixing
    // After short pressing of Fn/shift, if user press enter key, delete key
    // it will reset the shift key and Fn key state.
    if( EKeyEnter == aKeyEvent.iCode || EStdKeyEnter == aKeyEvent.iScanCode|| 
        EKeyBackspace == aKeyEvent.iCode || EStdKeyBackspace == aKeyEvent.iScanCode)
        {
		// if the shift key is still depressed, 
		// don't clear the qwerty shift mode
        if(!IsFlagSet(EFlagShiftKeyDepressed))
        	ClearFlag(EFlagQwertyShiftMode);     
        if( iFnKeyManager->FnKeyState() == CAknFepFnKeyManager::EFnKeyNext )
            {
            iFnKeyManager->ClearFnKeyState();
            }
        }
#endif    
    
    if ( iQwertyInputMode && 
         ( EditorHasFreeSpace() || 
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__         
#ifdef RD_INTELLIGENT_TEXT_INPUT 
           KeyEventWillReplaceCharacter( aKeyEvent ) ||
#endif // RD_INTELLIGENT_TEXT_INPUT
#endif // __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__         
           ( Japanese() && iFepManState == EAknFepStateUIActive ) ) )
        {
        if (IsFlagSet(EFlagPassNextKey))
            {
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__            
#ifdef RD_INTELLIGENT_TEXT_INPUT
            // Don't clear the pass next flag yet on the simulated "rollback key"
            // used with secret editors, the actual key code to be entered to the
            // editor will follow next.
            if ( aKeyEvent.iCode != EKeyF20 )
#endif // RD_INTELLIGENT_TEXT_INPUT          
#endif // __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__          
            ClearFlag(EFlagPassNextKey);
            return ETrue;   
            }        
                 
        TInt keyScanCode = aKeyEvent.iScanCode;
#if defined(__WINS__)
        if ( keyScanCode == EStdKeyNkpPlus )
            {
            // Workaround solution for the qwerty '+' key in emulator environment.
            // I did not find a way how to to configure epoc_352x416_Qwerty.ini to send '+' keyevent.
            keyScanCode = 0x2b;  // '+' key.
            }
#endif                 
        TBool phoneIdle = (EditorType() == CAknExtendedInputCapabilities::EPhoneNumberEditor);        
#ifdef RD_INTELLIGENT_TEXT_INPUT
        TInt keyLayout =iSharedDataInterface->KeyboardLayout();
        if (IsOnlyNumericPermitted() && phoneIdle &&                
#ifdef __REVERSE_FN_KEY_SUPPORTED
            !iIsReverseFnkeyInput &&
#endif
            (keyLayout != EPtiKeyboardHalfQwerty ))                
#else
        if (IsOnlyNumericPermitted() && phoneIdle)                
#endif
            {
            return ETrue;          
            }
        //if this SpecialNumeric editor, do not consume event.    
        if (IsOnlyNumericPermitted() && IsSpecialNumericEditor())
        	{
        	return EFalse;
        	}    
      
        
#ifdef FF_HOME_SCREEN_EASY_DIALING
#ifdef RD_INTELLIGENT_TEXT_INPUT
        
        // This piece of code part of error fixing
        // The Fep will insert the char, if Phone application launch the
        // Phone number acquire editor in idle mode
        // Specially Fep does not maintain Fn key state for non Fepware editor.
        // So, Intermediate state of FnKey like FnNext will not work
        // Hold on Fn key modify the modifier of of key event.
        const TBool fnDown = (aKeyEvent.iModifiers & EModifierRightFunc);        
        if( phoneIdle && fnDown && iFnKeyManager )
            {
            iFnKeyManager->SetFnKeyState(CAknFepFnKeyManager::EFnKeyDown);
            SetCase((TCase)EFnKeyLowerCase);
            }
#endif  // RD_INTELLIGENT_TEXT_INPUT
#endif  // FF_HOME_SCREEN_EASY_DIALING
        
       
        const TBool shifted = (aKeyEvent.iModifiers & (EModifierLeftShift | 
                               EModifierRightShift | EModifierShift));
        
        // This is needed for forcing shif state if this is the first key press in
        // phone idle.    
        if (shifted && phoneIdle)                    
            {
            SetFlag(EFlagQwertyShiftMode);
            }                           

            
        if (aKeyEvent.iRepeats == 1)
            {
            iKeyRepeat = ELongKeyPress;
            }
        else
            {
            iKeyRepeat = EShortKeyPress;
            }
                                  
        if ( IsFlagSet(EFlagShiftKeyDepressed) &&
            IsChineseInputLanguage() &&
            keyScanCode == EPtiKeyQwertySpace) 
            {
            SendEventsToPluginManL( EPluginCloseMode, EFalse );
            ClearFlag(EFlagQwertyShiftMode);
            ClearFlag(EFlagLongShiftKeyPress);
            SetChangeModeByShiftAndSpace( ETrue );
            TryIncrementChineseModeForQwertyL(iMode);
            keyHandled = ETrue;
            }
#ifdef RD_INTELLIGENT_TEXT_INPUT
		else
            {
	            if(iKeyRepeat == ELongKeyPress && iPtiEngine->CurrentWord().Length() == EMaximumFepWordLength)
	            {
	            	keyHandled = ETrue;
	            }
	            else
	            {
	            	keyHandled = FepUI()->HandleKeyL(keyScanCode, iKeyRepeat);     
					if( EPtiKeyboardHalfQwerty == KeyboardLayout() 
	            			&& iWesternPredictive
	            			&& iCaseMan->CurrentCase() != EAknEditorUpperCase )
	            		{
	            		ClearFlag(CAknFepManager::EFlagChangeInputMode);
	            		}
	            	if ( keyHandled && iFepManState == EAknFepStateUIActive 
	            			&& (iMode == EPinyin || iMode == EZhuyin || iMode == EStroke || iMode == ECangJie) )
	            		{
	            		if ( EditorState() )
	            			{
		            		EditorState()->SetFlags( EditorState()->Flags() | EAknEditorFlagChinesePopup );
	            			}
	            		}
	            }
            }
#else
        if (IsChineseInputLanguage() && keyScanCode == EStdKeyBackspace)
            {
            keyHandled = FepUI()->HandleKeyL(keyScanCode, iKeyRepeat);
            }
            else
            {
            keyHandled = FepUI()->HandleKeyL(keyScanCode, EShortKeyPress);      
            }
#endif //RD_INTELLIGENT_TEXT_INPUT
        
        if (keyHandled)
            {
            aResponse = EKeyWasConsumed;
#ifdef RD_INTELLIGENT_TEXT_INPUT
            // Predictive QWERTY changes ---->
            if ( iLangMan->IsSplitView() || ( iFepPluginManager && !iFepPluginManager->EnableITIOnFSQ()))
                {
                ShowExactWordPopupIfNecessaryL();
                }
            // Predictive QWERTY changes <----
            }
#endif //RD_INTELLIGENT_TEXT_INPUT

            // The indicators should be updated only if the key was handled. Otherwise
            // the shift-copy-pasting will mess up the indicators.
#ifdef RD_INTELLIGENT_TEXT_INPUT
    TInt language = iSharedDataInterface->InputTextLanguage();
    TBool IsChineseInput = language == ELangTaiwanChinese||
                           language == ELangHongKongChinese ||
                           language == ELangPrcChinese;
#endif      
       if ( !shifted  )
            {           
#ifdef RD_INTELLIGENT_TEXT_INPUT     
            if(IsChineseInput && (iKeyboardType!= EPtiKeyboardHalfQwerty)) 
                {
                ClearFlag(EFlagQwertyShiftMode);
                UpdateIndicators();      
                }
            else if( keyHandled )
                {
#endif                 
                ClearFlag(EFlagQwertyShiftMode);
                UpdateIndicators();
                
#ifdef RD_INTELLIGENT_TEXT_INPUT
                }
#endif
            } 
              
#ifdef RD_SCALABLE_UI_V2
    if ( ( (keyScanCode >= EPtiKeyQwertyA && keyScanCode <= EPtiKeyQwertyZ) ||
         (keyScanCode >= EPtiKeyQwerty0 && keyScanCode <= EPtiKeyQwerty9) ||
         (keyScanCode == EStdKeyEnter) ||
         (keyScanCode == EPtiKeyQwertyPlus) ||
         (keyScanCode == EPtiKeyQwertyMinus) ||
         (keyScanCode == EPtiKeyQwertyComma) ||
         (keyScanCode == EPtiKeyQwertySemicolon) ||
         (keyScanCode == EPtiKeyQwertyFullstop)  ||
         (keyScanCode == EPtiKeyQwertyHash)  ||
         (keyScanCode == EPtiKeyQwertySlash) ||
         (keyScanCode == EPtiKeyQwertyApostrophe) )
         && !( iFepPluginManager && iFepPluginManager->EnableITIOnFSQ() ) )          
        {
        //SendEventsToPluginManL( EPluginCloseMode, EFalse );
        //iFepPluginManager->ClosePluginInputModeL(ETrue);
        }
    else if( keyHandled )
        {
        //for QWERTY, number only, the number is send to editor
        //bypass FEP
        SendEventsToPluginManL( EPluginSyncFepAwareText ); 
        //iFepPluginManager->SyncFepAwareText();
        }
#endif
        }
        
#ifdef RD_INTELLIGENT_TEXT_INPUT    

        TInt lang = iSharedDataInterface->InputTextLanguage();

        if(lang == ELangTaiwanChinese||
           lang == ELangHongKongChinese ||
           lang == ELangPrcChinese)    
            {        
            if( CurrentInputLangCode() == ELangEnglish 
                && IsFlagSet(EFlagQwertyShiftMode)
                && !IsFlagSet(EFlagLongShiftKeyPress))
                {
                ClearFlag(EFlagQwertyShiftMode); 
                UpdateIndicators();  
                }
            }
#endif // RD_INTELLIGENT_TEXT_INPUT  
    iKeyRepeat = EShortKeyPress;        
    return keyHandled;
    }



void CAknFepManager::CancelTransaction()
    {
    if (iInputCapabilities.FepAwareTextEditor() && IsFlagSet(EFlagInsideInlineEditingTransaction))
        {
        CancelInlineEdit();
        }
    }

void CAknFepManager::IsOnHasChangedState()
    {
    }

void CAknFepManager::OfferKeyEventL(TEventResponse& /*aEventResponse*/, 
                                    const TKeyEvent& /*aKeyEvent*/, 
                                    TEventCode /*aEventCode*/)
    {
    }

void CAknFepManager::OfferPointerEventL(TEventResponse& /*aEventResponse*/, 
                                        const TPointerEvent& /*aPointerEvent*/, 
                                        const CCoeControl* /*aWindowOwningControl*/)
    {
    }

void CAknFepManager::OfferPointerBufferReadyEventL(TEventResponse& /*aEventResponse*/, 
                                                   const CCoeControl* /*aWindowOwningControl*/)
    {
    }

TInt CAknFepManager::NumberOfAttributes() const
    {
    return 0;
    }

TUid CAknFepManager::AttributeAtIndex(TInt /*aIndex*/) const
    {
    return KNullUid;
    }

void CAknFepManager::WriteAttributeDataToStreamL(TUid /*aAttributeUid*/, RWriteStream& /*aStream*/) const
    {
    }

void CAknFepManager::ReadAttributeDataFromStreamL(TUid /*aAttributeUid*/, RReadStream& /*aStream*/)
    {
    }

void CAknFepManager::HandleGainingForeground()
    {
    if (iFepFullyConstructed)
        {
        ClearFlag(EFlagRemoveMatchesMenuItem);
        ClearCcpuFlag(ECcpuStateHashDown | ECcpuStateSelectionEventPosted | 
                      ECcpuStateEdwinInSelectionMode);
        TRAPD(err, iPtiEngine->HandleCommandL(EPtiCommandReloadLearningInfo));
        if (err != KErrNone)
            {
            CleanUpFep();
            }
        }

#ifdef RD_SCALABLE_UI_V2        
    CCoeControl* focusCtrl = static_cast<const CCoeAppUi*>(CCoeEnv::Static()->AppUi())->TopFocusedControl(); 
    iGainForeground = ETrue;
    /*if( iLoseForeAndGainFocus )
        {
        SendEventsToPluginManL( EPluginFocusChanged, iGainForeground );
        }*/
    if(focusCtrl && focusCtrl->IsFocused())
        {
        if(GetForegroundTaskAppWgId() == CCoeEnv::Static()->RootWin().Identifier())
            //SendEventsToPluginManL( EPluginFocusChanged, iGainForeground );
            TRAP_IGNORE(SendEventsToPluginManL( EPluginForegroundChanged, ETrue));            
        }
    else
        {
        if (iFepPluginManager) 
            {
            iFepPluginManager->HandleiDimGainForeground(ETrue);   
            }            
        }
#endif // RD_SCALABLE_UI_V2        
    }

void CAknFepManager::HandleLosingForeground()
    {
    if (iFepFullyConstructed)
        {
        // Forget the shift state on FEP switch (i.e. when application goes to background)
        ClearFlag(EFlagQwertyShiftMode | EFlagShiftKeyDepressed | 
                  EFlagLongShiftKeyPress | EFlagNoActionDuringShiftKeyPress);
        }	
    if (Japanese())
        {
        TRAP_IGNORE(TryCloseUiL());
        }
    if (IsChineseInputLanguage())
        {
        TRAP_IGNORE(TryCloseUiL());
        }
        
#ifdef RD_SCALABLE_UI_V2     
    iGainForeground = EFalse;
    //SendEventsToPluginManL( EPluginFocusChanged, iGainForeground );
    TRAP_IGNORE(SendEventsToPluginManL( EPluginForegroundChanged, EFalse));            
#endif // RD_SCALABLE_UI_V2    
    }

void CAknFepManager::HandleChangeInFocus()
    {
    TRAPD(err, HandleChangeInFocusL());
    if (err != KErrNone)
        {
        CleanUpFep();
        }
    }

void CAknFepManager::HandleChangeInFocusForSettingFep()
	{
	HandleChangeInFocus();
	}

void CAknFepManager::HandleDestructionOfFocusedItem()
    {
    if (iInputCapabilities.FepAwareTextEditor())
        {
        iFocusedItemDestroy = ETrue;
        if ( iFepManState == EAknFepStateUIActive )
            {            
            TRAP_IGNORE(SendEventsToPluginManL( EPluginFocusItemDestroy ));  
            if (Japanese() && iAknEditorFlags & EAknEditorFlagFindPane)
                {
                // In case of Find pane while inputting japanese characters,
                // No update or commit characters.
                iPtiEngine->ClearCurrentWord();
                }

        #ifdef RD_SCALABLE_UI_V2
            // For addition of ITI features on FSQ, 
            // need to restore some values stored before opening FSQ
            // When go into this section, 
            // touch ui won't be closed by ClosePluginInputUiL,
            // but need to restore fep state here            
            if ( iFepPluginManager )
            	{
                TRAP_IGNORE(iFepPluginManager->ResetItiStateL());
            	}
        #endif
            // Close UI
            if ( FepUI())
                {
                FepUI()->CloseUI();
                }
            }
        else
            {            
            TRAP_IGNORE(SendEventsToPluginManL( EPluginFocusItemDestroy, ETrue ));  
            }
        iFocusedItemDestroy = EFalse;
        }

    if ( !(IsFlagSet(EFlagForegroundUIComponentVisible) ||
        IsFlagSet(EFlagMenuPaneVisible)) )
        {
        iLanguageCapabilities.iNumericEditorDigitType = EDigitTypeUnknown;
        //UpdateLocalDigitMode();
        }
    if(IsFlagSet(EFlagForegroundUIComponentVisible))
        {
        ClearFlag(EFlagForegroundUIComponentVisible);
        }

    ClearFlag( EFlagInsideInlineEditingTransaction |
        EFlagInsideMultitapInlineEditingTransaction |
        EFlagLineFeedCharacter | EFlagNoMatches );

    iInputCapabilities = NULL;
    }

void CAknFepManager::HandleAknEdwinStateEventL(CAknEdwinState* aAknEdwinState, 
                                               EAknEdwinStateEvent aEventType)
    {
    if(aEventType == EAknEdwinDestroy)
    	{
		// Reset the last focus editor when destroy it.
        if ( aAknEdwinState == iLastFocusedEditor )
            {
            iLastFocusedEditor->SetObserver( NULL );
            iLastFocusedEditor = NULL;
            }
        
        // Whenever the editor is destroyed, we destroy all the UI components we launched
        if(iFepAwareDialogParentEditor == NULL || 
        		iFepAwareDialogParentEditor == aAknEdwinState)
        	{
            // EExtendedFlagEdwinEditorDestroyed flag is used to leave once the dialog is closed.
            SetExtendedFlag(EExtendedFlagEdwinEditorDestroyed);
            // Delete any candidate popup is launched
            if(iCandidatePopup)
                {
                delete iCandidatePopup;
                iCandidatePopup = NULL;
                }        
            // Delete anu UI components launched by Fep.
            iUiInterface->DeleteDialogs();
        	}
        
        if ( iFepPluginManager )
        	{
			// Check whether the destroyed editor is the editor that stored in fep
			// according to the editor state
			if ( aAknEdwinState == iFepPluginManager->EdwinState())
				{
				// Set the editor pointer to NULL to avoid wild pointer exist
				iFepPluginManager->ClearDestroyedEditorPointer();
				}
        	}
    	}
    if ( IsFepAwareTextEditor() )
        {
        CAknEdwinState* editorState = EditorState();
        if ( editorState != aAknEdwinState || !iFepFullyConstructed )
            {
            // FEP state update is not needed because editor state of non-focused
            // editor was changed.
            return;
            }

        switch (aEventType)
            {
            case EAknEdwinStateInputModeUpdate:
                {
                TUint currentInputMode = editorState->CurrentInputMode();
                if ( EditorModeFromFepMode(iMode) != currentInputMode )
                    {
                    if (IsFlagSet(EFlagInsideInlineEditingTransaction))
                        {
                        TryRemoveNoMatchesIndicatorL();
                        }
                    UpdateCbaL(NULL);
                    TryCloseUiL();
                    ConfigureFEPFromEditorStateL();
                    }
                }
                break;
            case EAknEdwinStateCaseModeUpdate:
                {
                TInt currentCaseMode = editorState->CurrentCase();
                if ( currentCaseMode != iCaseMan->CurrentCase() )
                    {
                    iCaseMan->SetCurrentCase(currentCaseMode);
                    }
                }
                break;
            case EAknEdwinStateLocalLanguageUpdate:
                {
                TLanguage localLanguage = ELangTest;
                if (GetLocalLanguage( localLanguage ) )
                    {
                    iLanguageCapabilities.iLocalInputLanguageInUse = ETrue;
                    if ( localLanguage != iLanguageCapabilities.iInputLanguageCode )
                        {
                        ChangeInputLanguageL(localLanguage);
                        }
                    }
                }
                break;
            case EAknEdwinStateFlagsUpdate:
                {
                TInt oldEditorFlags = iAknEditorFlags;
                if(iAknEditorFlags != editorState->Flags())
                    {
                    iAknEditorFlags = editorState->Flags();
                    UpdateLocalDigitMode();
                    }
                if ( iAknEditorFlags != oldEditorFlags )
                    {
                    if ( (iAknEditorFlags & EAknEditorFlagNoT9) != 
                         (oldEditorFlags & EAknEditorFlagNoT9))
                        {
                        if (IsFlagSet(EFlagInsideInlineEditingTransaction))
                            {
                            TryRemoveNoMatchesIndicatorL();
                            }
                        UpdateCbaL(NULL);
                        TryCloseUiL();
                        ConfigureFEPFromEditorStateL();
                        }

                    if ( (iAknEditorFlags & EAknEditorFlagNoEditIndicators) != 
                         (oldEditorFlags & EAknEditorFlagNoEditIndicators))
                        {
                        // We only need to update indicators when EAknEditorFlagNoEditIndicators was modified.
                        // Current Fep state is not lost.
                        UpdateIndicators();
                        }

                    if ( (iAknEditorFlags & EAknEditorFlagFixedCase) != 
                         (oldEditorFlags & EAknEditorFlagFixedCase))
                        {
                        // We only need to update EFlagSupressAutoUpdate flag when the
                        // EAknEditorFlagFixedCase was modified.
                        // Current Fep state is not lost.
                        if (iAknEditorFlags & EAknEditorFlagFixedCase)
                            {
                            SetFlag(EFlagSupressAutoUpdate);
                            }
                        else
                            {
                            ClearFlag(EFlagSupressAutoUpdate);
                            }
                        }
                    }
                }
                break;
            case EAknEdwinStateEventStateUpdate:
                {
                // Feps state is reseted and new state is fetched completelly from editor state.
                if (IsFlagSet(EFlagInsideInlineEditingTransaction))
                    {
                    TryRemoveNoMatchesIndicatorL();
                    }
                UpdateCbaL(NULL);
                TryCloseUiL();
                ConfigureFEPFromEditorStateL();
                }
                break;           
            case EAknSyncEdwinState:
#ifdef RD_SCALABLE_UI_V2
                 if ( !IsCcpuFlagSet(ECcpuStateIgnoreNextEdwinSyncEvent) && iFepPluginManager &&
                     iFepPluginManager->PluginInputMode() == EPluginInputModeNone )
#else
                 if ( !IsCcpuFlagSet(ECcpuStateIgnoreNextEdwinSyncEvent) )
#endif                     
                     {
                     HandleChangeInFocusL();                        
                     }
                 else
                    {
                    ClearCcpuFlag(ECcpuStateIgnoreNextEdwinSyncEvent);
                    }
                 break;  
                 
#ifdef RD_SCALABLE_UI_V2
            case EAknActivatePenInputRequest:
                SendEventsToPluginManL( EPluginEditorActivate );
                break;
            case EAknClosePenInputRequest:
                if ( iFepPluginManager )
                    {
                    iFepPluginManager->ClosePluginInputModeL( EFalse );
                    }
                break;
            case EAknEdwinStatePromptUpdate:
                SendEventsToPluginManL( EPluginPromptChanged ); 
                break;
#endif // RD_SCALABLE_UI_V2                
            case EAknCursorPositionChanged:
#ifdef RD_SCALABLE_UI_V2  
                SendEventsToPluginManL( EPluginSyncFepAwareText );   
                HandleCopyCutStateL();    
#endif
                // whenever focus cahnged happen, internally
                // editor treat cursorposition change.
                // In between, if editor context not in inline edit state.
                // Try to update the case editor gets the focus, and editor
                // context come in inline state.                
				iCaseMan->UpdateCase( ENullNaviEvent );
                break;

            default:
                break;
            }
        }
#ifdef RD_SCALABLE_UI_V2
    else
        {
        // Close touch input when inMenu is opened. When inMenu is opened, 
        // editor loses focus, and inMenu gets focus. After touch input has 
        // been force closed, menu state should be restored and last focused
        // window ,which launches touch input, should be removed.
        if( aEventType == EAknClosePenInputRequest  
                && iFepPluginManager  
                && iFepPluginManager->IsInMenuOpen() )
            {
            CCoeEnv* env = CCoeEnv::Static();
            TKeyEvent keyEvent = {EKeyCBA2, EStdKeyNull, 0, 0};
            env->SimulateKeyEventL(keyEvent,EEventKey);            
            iFepPluginManager->ResetMenuState();            
            iFepPluginManager->RemoveLastFocusedWinFromOpenList();
            iFepPluginManager->ClosePluginInputModeL( EFalse );
            }
        }
#endif // RD_SCALABLE_UI_V2  
    }


CAknExtendedInputCapabilities::TEditorType CAknFepManager::EditorType() const
    {
    MObjectProvider* mop = iInputCapabilities.ObjectProvider();

    if ( mop )
        {
        CAknExtendedInputCapabilities* extendedInputCapabilities = 
        mop->MopGetObject( extendedInputCapabilities );

        if ( extendedInputCapabilities && iInputCapabilities.FepAwareTextEditor() ) 
            {
            TInt type = extendedInputCapabilities->EditorType();                                     
            return static_cast<CAknExtendedInputCapabilities::TEditorType>( type );
            }
        }
        
    return CAknExtendedInputCapabilities::EUndefined;
    }

TUint CAknFepManager::MIDPConstraint() const
	{
	TUint ConstraintValue=0xFFFFFFFF;
	MObjectProvider* mop = iInputCapabilities.ObjectProvider();
	
	if ( mop )
		{
		CAknExtendedInputCapabilities* extendedInputCapabilities = 
        mop->MopGetObject( extendedInputCapabilities );
		if ( extendedInputCapabilities && iInputCapabilities.FepAwareTextEditor() ) 
			{
			ConstraintValue = extendedInputCapabilities->MIDPConstrainst();
			}
		}
		return ConstraintValue;
	}
    
TBool CAknFepManager::IsSpecialNumericEditor()
	{
	TBool JavaNumericEditor = EFalse;
	TUint ConstraintValue = MIDPConstraint();
	if((ConstraintValue & ESplConstraintMask) == ESplNumeric )
		JavaNumericEditor = ETrue;
	
	return JavaNumericEditor;
	
	}
#ifdef RD_SCALABLE_UI_V2
void CAknFepManager::HandleInputCapabilitiesEventL( TInt aEvent, TAny* /*aParams*/ )
    {
    switch (aEvent)
        {
        case CAknExtendedInputCapabilities::MAknEventObserver::EActivatePenInputRequest:
            SendEventsToPluginManL( EPluginEditorActivate );
            break;
        case CAknExtendedInputCapabilities::MAknEventObserver::EClosePenInputRequest:
            if ( iFepPluginManager )
                {
                iFepPluginManager->ClosePluginInputModeL( EFalse );
                }
            break;
        case CAknExtendedInputCapabilities::MAknEventObserver::EPointerEventReceived:

/* 
#ifdef RD_TACTILE_FEEDBACK
            CAknExtendedInputCapabilities::
                MAknEventObserver::TPointerEventReceivedParams* params = 
                    static_cast<CAknExtendedInputCapabilities::
                MAknEventObserver::TPointerEventReceivedParams*>(aParams);
            if (params->iPointerEvent.iType == TPointerEvent::EButton1Down)            
                {
                
                MTouchFeedback* feedback = MTouchFeedback::Instance();
                if (feedback && iFepPluginManager && !iFepPluginManager->VKBIsOpened())
                    {
                    feedback->InstantFeedback( ETouchFeedbackBasic );
                    }
                }
#endif // RD_TACTILE_FEEDBACK
*/
            if( IsFlagSet( EFlagInsideInlineEditingTransaction ) )
                {
                if (IsFeatureSupportedJapanese())
			        {
			        TryCloseUiL();
			        }
			    else
			        {
#ifdef RD_INTELLIGENT_TEXT_INPUT			        
			        iPtiEngine->CommitCurrentWord();
					CommitInlineEditL();
			        TryCloseUiL();

#else
					CommitInlineEditL();
#endif			        
			        }
                }
            break;      
        case CAknExtendedInputCapabilities::MAknEventObserver::EControlContentUpdatedInternally:
            SendEventsToPluginManL( EPluginSyncFepAwareText, EPluginSyncAll );
            break;
        case CAknExtendedInputCapabilities::MAknEventObserver::EOpenStylusMenuCcpu:
            // User has highlighted text and editor requests stylus ccpu popup menu
            // to be displayed.        
            LaunchStylusCcpuMenuL(iClickPoint);
            break;
        }    
    }
    
void CAknFepManager::SubmitInlineTextL( const TDesC& aData )
    {
    if(TextIsValidInEditor(aData))
    	{
    	StartInlineEditL(aData);
    	CommitInlineEditL();
    	}
    }
    
// for japanese  
void CAknFepManager::NotifyJapaneseSetting()
    {
	TInt param = 0;
    // Deleting direction ON(right side) / OFF(left side)
    if (iSharedDataInterface->ClearDirection() == EClearDirectionRight)
        {
        param |= EPenInputJapaneseSettingDeletingDirection;
        }
    // Japanese predictive ON(predictive on) / OFF(predictive off)
    if (iJapanesePredictive)
        {
        param |= EPenInputJapaneseSettingPredictive;
        }
    // Japanese HWR conversion ON / OFF
    if (iSharedDataInterface->JapaneseHwrConversion() == EJapaneseConversionOn)
        {
        param |= EPenInputJapaneseSettingConversion;
        }
    // Japanese HWR conversion ON / OFF
    if (iCharWidth == EFullWidthChar)
        {
        param |= EPenInputJapaneseSettingCharacterWidth;
        }
    // Japanese qwerty setting flag
    TInt qwertflag = iSharedDataInterface->JapaneseQwertyFlags();
    // Qwerty Comma
    if (qwertflag & EJapQwertySettingComma)
        {
        param |= EPenInputJapaneseSettingQwertyComma;
        }
    // Qwerty Period
    if (qwertflag & EJapQwertySettingPeriod)
        {
        param |= EPenInputJapaneseSettingQwertyPeriod;
        }
    // Qwerty Width Of Space
    if (qwertflag & EJapQwertySettingSpaceFullWidth)
        {
        param |= EPenInputJapaneseSettingQwertyWidthOfSpace;
        }
    // Lunch SCT
    if (IsAbleToLaunchSCT())
        {
        param |= EPenInputJapaneseSettingLunchSCT;
        }
#ifdef RD_SCALABLE_UI_V2
    // Set Japanese setting
    if(iFepPluginManager->CurrentPluginInputFepUI())    
        {
        TRAP_IGNORE(iFepPluginManager->CurrentPluginInputFepUI()->HandleCommandL(
                                             ECmdPenInputJapaneseSetting, param));
        }
#endif //RD_SCALABLE_UI_V2    
    }

TBool CAknFepManager::FullyFepAwareTextEditor() const
    {
    if ( iInputCapabilities.FepAwareTextEditor() )
        {
        if ( EditorType() != CAknExtendedInputCapabilities::EMFNEBased )
            {
            return ETrue;
            }
        }
        
    return EFalse;
    }

TBool CAknFepManager::SemiFepAwareTextEditor( TBool aAtLeast ) const
    {
    if ( aAtLeast )
        {
        switch ( EditorType() )
            {
            case CAknExtendedInputCapabilities::EEdwinBased:
            case CAknExtendedInputCapabilities::EMFNEBased:
                return ETrue;
                
            default:
                return EFalse;
            }
        }
    else
        {
        return EditorType() == CAknExtendedInputCapabilities::EMFNEBased;
        }
    }
    
void CAknFepManager::ProcessEditorMenuCommand(TInt aCommand)
    {
    TRAP_IGNORE(ProcessCommandL(aCommand));    
    }
    
TInt CAknFepManager::GetPermittedEditorMenu(TBool aOnlyCount)
    {
    TInt count = 0;
    TRAP_IGNORE(count = GetPermittedEditorMenuL(aOnlyCount));
    return count;
    }

TInt CAknFepManager::GetPermittedEditorMenuL(TBool aOnlyCount)
    {
    
    CAknEdwinState* editorState = NULL;
    if (iInputCapabilities.FepAwareTextEditor())
        {
        editorState = EditorState();
        }
    
    CAknFepUiInterfaceMenuBar* menuBar = NULL;

	RPointerArray<CEikMenuPaneItem> editorMenuItemList;	
    
    CEikMenuPane* editorMenuPane = new (ELeave) CEikMenuPane((MEikMenuObserver*)(this));
    CleanupDeletePushL( editorMenuPane );
    
    editorMenuPane->SetItemArrayOwnedExternally(EFalse);
    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC(reader, R_AVKON_TOUCH_INDICATOR_EDIT_MODE_MENU);//
        
    const TInt menuCount=reader.ReadInt16();
    for ( TInt ii=0; ii<menuCount; ++ii )
        {
        CEikMenuPaneItem* item = new(ELeave) CEikMenuPaneItem();
        CleanupStack::PushL( item );
        item->iData.iCommandId = reader.ReadInt32();
        item->iData.iCascadeId = reader.ReadInt32();
        item->iData.iFlags = reader.ReadInt32();
        TPtrC txtptr = reader.ReadTPtrC();
        item->SetScaleableTextL( txtptr );
        TPtrC extratxtptr = reader.ReadTPtrC();
        item->iData.iExtraText = extratxtptr;
        TPtrC bitmapFile = reader.ReadTPtrC();
        TInt bitmapId = reader.ReadInt16();
        TInt bitmapMaskId = reader.ReadInt16();
        CleanupStack::Pop(); // pop first, since additem pushes again
        editorMenuPane->AddMenuItemL( item->iData );
        editorMenuItemList.Append(item);
        reader.ReadInt32(); // extension link
        }
        
    CleanupStack::PopAndDestroy(); //reader 

            
    if (editorState)
        {
        menuBar = iUiInterface->EditorStateMenuBar(editorState);
        }
    
    TInt count = 0;            
    
    if ( menuBar )
        {
        CAknFepUiInterfaceMenuPane* menuPane = menuBar->MenuPane();

        CEikMenuPane* oldMenuPane = menuPane->GetMenuPane();
   
        menuPane->SetMenuPane(editorMenuPane);             
             
        DynInitTouchMenuPaneL(menuPane);
        
        menuPane->SetMenuPane(oldMenuPane);
                

        for (TInt ii = 0; ii < KMaxMenuSize; ii++)
            {
            CEikMenuPaneItem::SData& itemdata = 
                        editorMenuPane->ItemData(KEditorMenuPermitedSend[ii]);
            if ( !(itemdata.iFlags & EEikMenuItemDimmed ) )
                {
                count = count + 1;
                iEditorCommandList[count] = itemdata.iCommandId; 
                }
            }
        iEditorCommandList[0] = count;
        if (!aOnlyCount && iFepPluginManager->CurrentPluginInputFepUI())
            {
            iFepPluginManager->CurrentPluginInputFepUI()->HandleCommandL(
                        ECmdPenInputSendEditMenuData, reinterpret_cast<TInt>(iEditorCommandList));
            }
        }
    
    editorMenuItemList.ResetAndDestroy();
	CleanupStack::PopAndDestroy(editorMenuPane);
	
    return count;
    }
#endif // RD_SCALABLE_UI_V2

void CAknFepManager::ExitPluginSpellModeByOk()
    {
    TRAP_IGNORE(ExitPluginSpellModeByOkL());
    }

void CAknFepManager::ExitPluginSpellModeByOkL()
    {
#ifdef RD_SCALABLE_UI_V2        		    
    if (iFepPluginManager->IsSpellVisible())
        {
        CommitInlineEditL();
        HBufC* spell = iFepPluginManager->SpellTextInput();
        
        iFepPluginManager->SetITUTSpellingStateL(EFalse); 
        iFepPluginManager->HideSpellEditor();        
        
        if (spell)
            {
            CleanupStack::PushL(spell);
            TPtr text = spell->Des();
            FepUI()->AddTextToUserDictionaryL(text);
            InsertTextFromDialogL(text, iFepPluginManager->CursorSelection());
            CleanupStack::PopAndDestroy(spell);
            }
        
        UpdateCbaL(NULL); 
        if (iFepPluginManager->CaseUpdatesSupressed())
            {
            SetFlag(EFlagSupressAutoUpdate);
            }
        else
            {
            ClearFlag(EFlagSupressAutoUpdate);
            }
        ClearFlag(EFlagSupressAutoUpdateAtEditorStart);
        }
#endif       		    
    }

void CAknFepManager::ExitPluginSpellModeByCancel()
    {
/*    
#ifdef RD_SCALABLE_UI_V2
    if (iFepPluginManager->IsSpellVisible())
        {
        iFepPluginManager->SetITUTSpellingStateL(EFalse); 
        iFepPluginManager->DestroySpellEditor();
        UpdateCbaL(NULL);
        HandleChangeInFocus();
        if (iFepPluginManager->CursorSelection().Length())
            {
            EditorState()->SetInlineEditSpan(iFepPluginManager->CursorSelection());
            ConfigureFEPFromEditorStateL();
            }
        TransferFepStateToEditorL();

        if (iFepPluginManager->CaseUpdatesSupressed())
            {
            SetFlag(EFlagSupressAutoUpdate);
            }
        else
            {
            ClearFlag(EFlagSupressAutoUpdate);
            }
        ClearFlag(EFlagSupressAutoUpdateAtEditorStart);
        iFepPluginManager->SetBeforeSpell(ETrue);
        }
#endif
*/
#ifdef RD_SCALABLE_UI_V2
    if (iFepPluginManager->IsSpellVisible())
        {
        TRAP_IGNORE(iFepPluginManager->SetITUTSpellingStateL(EFalse)); 
        iFepPluginManager->HideSpellEditor();        

        TRAP_IGNORE(UpdateCbaL(NULL)); 
        if (iFepPluginManager->CaseUpdatesSupressed())
            {
            SetFlag(EFlagSupressAutoUpdate);
            }
        else
            {
            ClearFlag(EFlagSupressAutoUpdate);
            }
        ClearFlag(EFlagSupressAutoUpdateAtEditorStart);
        }
#endif       		
    }

void CAknFepManager::ProcessCommandL(TInt aCommandId)
    {
    //ProcessCommandL() is called before HandleChangeInFocus() for the editor to insert the
    //number into, so have to force a call
    
    //Process pen input menu if avaliable
#ifdef RD_SCALABLE_UI_V2

     if (iFepPluginManager && 
        (iFepPluginManager->PluginInputMode() == EPluginInputModeItut)) 
       	{
       	switch(aCommandId)
       		{
       		case EAknSoftkeyOptions:
       		    {
       		    LaunchSelectModeMenuL();
       		    }
       		    return;
       		case EAknSoftkeyOk:
       		    {
                iFepPluginManager->ClosePluginInputModeL(ETrue);
       		    }
       		    return;
       		default:
       			{
       			break;
       			}
       		}
       	}

    SendEventsToPluginManL( EPluginMenuCmd, aCommandId );

#endif //RD_SCALABLE_UI_V2
    MAknFepManagerInterface* currentFepUI=NULL;
    switch (aCommandId)
        {
        // Edit mode menu commands
        //The soft CBA event from touch screen. 
        case EAknSoftkeyCancel:
      //  case (TUint16)EAknSoftkeyCancel:
        case EAknSoftkeySelect:
     //   case (TUint16)EAknSoftkeySelect:
        currentFepUI = FepUI();
        if (currentFepUI)
            currentFepUI->HandleCommandL( aCommandId );
            break;
        // --- commands for Japanese ---
        case EJapanFepCmdModeHiragana:
            SendEventsToPluginManL( EPluginCloseMode, EFalse );
            HandleChangeInFocus();
            TryChangeModeL(EHiraganaKanji);
            break;
        case EJapanFepCmdModeKatakana:
            SendEventsToPluginManL( EPluginCloseMode, EFalse );
            HandleChangeInFocus();
            TryChangeModeL(EKatakana);
            break;
        case EAknCmdEditInsertPictograph:
            SetStopProcessFocus(ETrue);            
            HandleChangeInFocus();
            LaunchPictographCharacterTableL();
            SetStopProcessFocus(EFalse);            
            break;
        case EJapanFepCmdModeFullWidth:
        case EPenInputJpCmdFullWidth:
        case EJapanFepCmdModeHalfWidth:
        case EPenInputJpCmdHalfWidth:
            {
            SendEventsToPluginManL( EPluginCloseMode, EFalse );
            HandleChangeInFocus();
            if (aCommandId == EJapanFepCmdModeFullWidth
             || aCommandId == EPenInputJpCmdFullWidth)
                {
                iCharWidth = EFullWidthChar;
                }
            else
                {
                iCharWidth = EHalfWidthChar;
                }
            UpdateIndicators();
            TryChangeModeL(iMode);
#ifdef RD_SCALABLE_UI_V2
            if (aCommandId == EPenInputJpCmdFullWidth
             || aCommandId == EPenInputJpCmdHalfWidth)
                {
                // Re-open pen input window
                SendEventsToPluginManL( EPluginActivate );  
                }
#endif //RD_SCALABLE_UI_V2
            }
            break;
        case EJapanFepCmdModeUserDic:
        case EPenInputJpCmdSaveWord:
            {
            SendEventsToPluginManL( EPluginCloseMode, EFalse );
            // User dictionary
#ifdef RD_PF_SEC_APPARC
            // Launch settings app
            if ( iNullService )
                {
                delete iNullService;
                iNullService = NULL;
                }
            iNullService = CAknNullService::NewL( KUidUserDictApp, this );
#else
            CEikProcess* process = CEikonEnv::Static()->Process();
            if (iEmbedded)
                {
                process->DestroyDocument(iEmbedded);
                }
#if (defined(SYMBIAN_SUPPORT_UI_FRAMEWORKS_V1) || defined(__SERIES60_27__) || defined(__SERIES60_28__)) 
            iEmbedded = process->AddNewDocumentL(_L("User Dictionary"), KUidUserDictApp);
#else
            iEmbedded = process->AddNewDocumentL(KUidUserDictApp);
#endif
            iEmbedded->NewDocumentL();
            iEmbedded->EditL(NULL);
#endif //RD_PF_SEC_APPARC
            }
            break;
        case EPenInputJpCmdKutenCode:
        case EJapanFepCmdModeKutenCodeInput:
            {
            SendEventsToPluginManL( EPluginCloseMode, EFalse );
            LaunchKutenCodeQueryL();
#ifdef RD_SCALABLE_UI_V2
            if (aCommandId == EPenInputJpCmdKutenCode)
                {
                // Re-open pen input window
                SendEventsToPluginManL( EPluginActivate );
                }
#endif //RD_SCALABLE_UI_V2
            }
            break;
        case EAknCmdEditClearDirectionLeft:
        case EAknCmdEditClearDirectionRight:
            {
            TInt value = EClearDirectionLeft;
            if (aCommandId == EAknCmdEditClearDirectionRight)
                {
                value = EClearDirectionRight;
                }
            iSharedDataInterface->SetClearDirection(value);
            }
            break;
        case EAknFepSoftkeyCloseWindow:
            if(IsChineseInputLanguage())
                {
                TryCloseUiL();
                }                        
            if (iCcpuMode != ECcpuStateNone)
                {
                ResetCcpuFlags();

                // reset select mode indicators
                iIndicator->SetCopyMode(EFalse);
                // update view                
                iPreviousEditingState = EStateNone;
                UpdateIndicators();

                iUiInterface->DeleteSoftkeys();                              
                break;
                }
        case EAknFepSoftkeyCommit:
        case EAknFepSoftkeySelectCandidate:
        case EAknFepSoftkeyConvert:
        case EAknFepSoftkeyOtherModes:
        case EAknFepSoftkeyRetroActiveHalf:
        case EAknFepSoftkeyRetroActiveFull:
            {
            FepUI()->HandleCommandL(aCommandId);
            }
            break;
        case EAknFepSoftkeyPredictiveOn:
            {
            iJapanesePredictive = ETrue;
            iSharedDataInterface->SetJapanesePredictiveTextOn();
            iHashKeyMan->SetMode(iMode, ETrue);
            UpdateIndicators();
            FepUI()->HandleCommandL(aCommandId);
            }
            break;
        case EJapanFepCmdModePredictiveOn:
            {
            SendEventsToPluginManL( EPluginCloseMode, EFalse );
            HandleChangeInFocus();
            if (!iJapanesePredictive)
                {
                LaunchConfirmationNoteL(R_AVKON_JAPANESE_PREDICTIVE_ACTIVATED_NOTE);
                }
            iJapanesePredictive = ETrue;
            TryChangeModeL(EHiraganaKanji);
            iSharedDataInterface->SetJapanesePredictiveTextOn();
            }
            break;
        case EJapanFepCmdModePredictiveOff:
            {
            SendEventsToPluginManL( EPluginCloseMode, EFalse );
            HandleChangeInFocus();
            if (iJapanesePredictive)
                {
                LaunchConfirmationNoteL(R_AVKON_JAPANESE_PREDICTIVE_DEACTIVATED_NOTE);
                }
            iJapanesePredictive = EFalse;
            TryChangeModeL(EHiraganaKanji);
            iSharedDataInterface->ResetJapanesePredictiveTextOn();
            }
            break;
#ifdef RD_SCALABLE_UI_V2
        case EPenInputJpCmdPredictiveOn:
            {
            iJapanesePredictive = ETrue;
            iSharedDataInterface->SetJapanesePredictiveTextOn();
            if (iFepPluginManager->PluginInputMode() == EPluginInputModeHwr)
                {
                iSharedDataInterface->SetJapaneseHwrConversion(EJapaneseConversionOn);
                }
            }
            break;
        case EPenInputJpCmdPredictiveOff:
            {
            iJapanesePredictive = EFalse;
            iSharedDataInterface->ResetJapanesePredictiveTextOn();
            }
            break;
        case EPenInputJpCmdConversionOn:
            {
            iSharedDataInterface->SetJapaneseHwrConversion(EJapaneseConversionOn);
            }
            break;
        case EPenInputJpCmdConversionOff:
            {
            iSharedDataInterface->SetJapaneseHwrConversion(EJapaneseConversionOff);
            }
            break;
#endif //RD_SCALABLE_UI_V2

        // --- commands for Chinese ---
        case EChinFepCmdModePinyin:
        case EChinFepCmdModePinyinPhrase:
            SendEventsToPluginManL( EPluginCloseMode, EFalse );
            HandleChangeInFocus();
            TryChangeModeL(EPinyin);
            break;
        case EChinFepCmdModeZhuyin:
        case EChinFepCmdModeZhuyinPhrase:
            SendEventsToPluginManL( EPluginCloseMode, EFalse );
            HandleChangeInFocus();
            TryChangeModeL(EZhuyin);
            break;
        case EChinFepCmdModeStroke:
        case EChinFepCmdModeStrokePhrase:  
            SendEventsToPluginManL( EPluginCloseMode, EFalse );
            HandleChangeInFocus();
            TryChangeModeL(EStroke);
            break;
        case EChinFepCmdModeCangJie:
            SendEventsToPluginManL( EPluginCloseMode, EFalse );
            HandleChangeInFocus();
            TryChangeModeL(ECangJie);
            break;
        case EChinFepCmdModeCangJieOption:
            // Launch CangJie option dialog
            {
            SetStopProcessFocus(ETrue);
            SendEventsToPluginManL( EPluginCloseMode, EFalse );
            TInt newMode = LaunchCangJieOptionDlgL();
            SetStopProcessFocus(EFalse);
            UpdateCangJieState( newMode );
            }
            break;
        case EChinFepCmdModeLatinUpper:
            SendEventsToPluginManL( EPluginCloseMode, EFalse );
            HandleChangeInFocus();
            TryChangeModeL(ELatinUpper);
            // delete it for support auto update to text-case
            //SetFlag(EFlagSupressAutoUpdate);
            iCaseMan->SetCurrentCase(EAknEditorUpperCase);
            break;
        case EAknCmdEditModeKorean:
            SendEventsToPluginManL( EPluginCloseMode, EFalse );
            HandleChangeInFocus();
            TryChangeModeL(EHangul);
            // delete it for support auto update to text-case
            //SetFlag(EFlagSupressAutoUpdate);
            iCaseMan->SetCurrentCase(EAknEditorUpperCase);
            break;
        case EChinFepCmdModeLatinLower:
            SendEventsToPluginManL( EPluginCloseMode, EFalse );
            HandleChangeInFocus();
            TryChangeModeL(ELatinLower);
            // delete it for support auto update to text-case
            //SetFlag(EFlagSupressAutoUpdate);
            iCaseMan->SetCurrentCase(EAknEditorLowerCase);
            break;
        case EAknCmdEditModeNumber:
            //if current input is arabic finger hwr, then change the default 
            //number mode.
            if(iSharedDataInterface->InputTextLanguage() == ELangArabic 
               && iFepPluginManager->PluginInputMode() == EPluginInputModeFingerHwr)
                {
                iSharedDataInterface->SetDefaultArabicNumberMode(0);
                HandleChangeInFocus();
                SendEventsToPluginManL(EPluginArabicNumModeChanged, 0);                
                break;
                }
        case EJapanFepCmdEditModeNumber:
        case EChinFepCmdModeNumber:
            SendEventsToPluginManL( EPluginCloseMode, EFalse );
            HandleChangeInFocus();
            if ( iLanguageCapabilities.iArabicIndicDigitsAllowed 
                || iLanguageCapabilities.iEasternArabicIndicDigitsAllowed 
                || iLanguageCapabilities.iIndicDigitsAllowed )
                {
                iLanguageCapabilities.iLocalDigitType = EDigitTypeWestern;
                }
            TryChangeModeL(ENumber);
            break;
        case EAknCmdEditModeArabicIndicNumber:
            //if current input is arabic finger hwr, then change the default 
            //number mode.
            if(iSharedDataInterface->InputTextLanguage() == ELangArabic 
               && iFepPluginManager->PluginInputMode() == EPluginInputModeFingerHwr)
                {
                iSharedDataInterface->SetDefaultArabicNumberMode(1);
                SendEventsToPluginManL(EPluginArabicNumModeChanged, 1);
                HandleChangeInFocus();
                }
            else
                {
                SendEventsToPluginManL( EPluginCloseMode, EFalse );
                HandleChangeInFocus();
                iLanguageCapabilities.iLocalDigitType = EDigitTypeArabicIndic;
                TryChangeModeL(ENativeNumber);
                }
            break;
            /*For Eastern Arabic Numeric*/
        case EAknCmdEditModeEasternArabicIndicNumber:
            SendEventsToPluginManL( EPluginCloseMode, EFalse );
            HandleChangeInFocus();
            iLanguageCapabilities.iLocalDigitType = EDigitTypeEasternArabicIndic;
            TryChangeModeL(ENativeNumber);
            break;
            /*for Hindi*/
        case EAknCmdEditModeIndicNumber:
            HandleChangeInFocus();
            iLanguageCapabilities.iLocalDigitType = EDigitTypeDevanagari;
            TryChangeModeL(ENativeNumber);
            break;
        case EChinFepCmdInstructions:
            SetStopProcessFocus(ETrue);            
            HandleChangeInFocus();
            LaunchHelpTextQueryL();
            SetStopProcessFocus(EFalse);            
            break;
        case EChinFepCmdModeZhuyinFind:
            SendEventsToPluginManL( EPluginCloseMode, EFalse );
            HandleChangeInFocus();
            TryChangeModeL(EZhuyinFind);
            break;
        case EChinFepCmdModeStrokeFind:
            SendEventsToPluginManL( EPluginCloseMode, EFalse );
            HandleChangeInFocus();
            TryChangeModeL(EStrokeFind);
            break;

        // commands for western or common
        case EJapanFepCmdMultitapPredictiveT9On:
        case EAknCmdMultitapPredictiveT9On:
#ifdef RD_INTELLIGENT_TEXT_INPUT
        case EAknFepCmdPredActivate:
#endif //RD_INTELLIGENT_TEXT_INPUT
            SetStopProcessFocus(ETrue, EFalse);
            HandleChangeInFocus();
#ifdef RD_SCALABLE_UI_V2     
        if( iFepFullyConstructed && iFepPluginManager)
            {
            iFepPluginManager->SetMenuState();            
            }
#endif
            if ( !iWesternPredictive )
                {
                LaunchConfirmationNoteL(R_AVKON_T9_ACTIVATED_NOTE);
                }
            SetWesternPredictive(ETrue);
            SetStopProcessFocus(EFalse);
            TryChangeModeL(ELatin);
            break;
        case EAknCmdT9PredictiveT9Off:
#ifdef RD_INTELLIGENT_TEXT_INPUT
        case EAknFepCmdPredDeactivate:
#endif //RD_INTELLIGENT_TEXT_INPUT
        SetStopProcessFocus(ETrue, EFalse);
            HandleChangeInFocus();
#ifdef RD_SCALABLE_UI_V2     
        if( iFepFullyConstructed && iFepPluginManager) 
            {
            iFepPluginManager->SetMenuState();            
            }
#endif   
            if (iMode != ELatin)
                {
                TryChangeModeL(ELatin);
                }
            else
                {
#ifdef RD_INTELLIGENT_TEXT_INPUT
                RemoveSuggestedAdvanceCompletionL();
#endif // RD_INTELLIGENT_TEXT_INPUT                	
                CommitInlineEditL();
                }
			if ( iWesternPredictive )
                {
                LaunchConfirmationNoteL(R_AVKON_T9_DEACTIVATED_NOTE);
                }
            SetWesternPredictive(EFalse);
            SetStopProcessFocus(EFalse);            
            break;
        case EAknCmdEditModeEnglish:
        case EAknCmdEditModeAlpha:
        case EJapanFepCmdEditModeAlpha:
            SendEventsToPluginManL( EPluginCloseMode, EFalse );
            HandleChangeInFocus();            
            TryChangeModeL(ELatin);
            iCaseMan->UpdateCase( ENullNaviEvent );
            ClearFlag(EFlagSupressAutoUpdate);
            break;
        case EAknCmdEditModeLatinText:
            SendEventsToPluginManL( EPluginCloseMode, EFalse );
            HandleChangeInFocus();
            TryChangeModeL(ELatinText);
            ClearFlag(EFlagSupressAutoUpdate);
            iCaseMan->SetCurrentCase(EAknEditorTextCase);
            break;
        case EAknCmdEditInsertSymbol:
        case EAknCmdEditInsertSymbolJp:
        case EAknFepSoftkeySymbol:
        case EAknCmdEditInsertSmiley:
            SetCcpuFlag(ECcpuStateSupressCursorMoveToEnd);
#ifdef RD_SCALABLE_UI_V2            
     if (iFepPluginManager && 
        (iFepPluginManager->PluginInputMode() == EPluginInputModeItut ||
         iFepPluginManager->PluginInputMode() == EPluginInputModeFSQ ||
         iFepPluginManager->PluginInputMode() == EPluginInputModeFingerHwr || 
         iFepPluginManager->PluginInputMode() == EPluginInputModePortraitFSQ ) )
        {
        iFepPluginManager->SetMenuState();     
        }

#endif //RD_SCALABLE_UI_V2             
            SetStopProcessFocus(ETrue, EFalse);
            HandleChangeInFocus();
            if (aCommandId == EAknFepSoftkeySymbol)
                {               
                SetCcpuFlag(ECcpuStateCbaSymbol);
                }
            // Show Thai specific SCT with Thai vowels and tonemarks
            if ( (iLanguageCapabilities.iInputLanguageCode == ELangThai ) &&
                 (iMode!=ENumber && iMode != ENativeNumber ) )
                {
				TInt resourceId = 0;
				if (iAknFepThaiSCTSelector)
					{
					resourceId = iAknFepThaiSCTSelector->ThaiSCRResourceId(PreviousChar(),EPtiKeyStar);
					}
				LaunchSpecialCharacterTableL(resourceId, 
											 aCommandId==EAknCmdEditInsertSymbol, 
											 aCommandId==EAknCmdEditInsertSmiley);                  
                }
            else
            {
                LaunchSpecialCharacterTableL(0, 
                                             aCommandId==EAknCmdEditInsertSymbol, 
                                             aCommandId==EAknCmdEditInsertSmiley);
            }
            SetStopProcessFocus(EFalse);
            ClearCcpuFlag(ECcpuStateCbaSymbol);
            break;
        case EAknFepSoftkeyPrevious:
            FepUI()->HandleCommandL(aCommandId);
            break;
        case EAknFepSoftkeySpell:
            LaunchEditWordQueryL();
            break;
        case EAknCmdT9PredictiveInsertWord:
            SetStopProcessFocus(ETrue, EFalse);
            HandleChangeInFocus();
            LaunchInsertWordQueryL(KNullDesC, TCursorSelection(0,0));
            SetStopProcessFocus(EFalse);
#ifdef RD_SCALABLE_UI_V2     
        if( iFepFullyConstructed && iFepPluginManager)
            {
            iFepPluginManager->ResetMenuState();            
            }
#endif             
            break;
        case EAknCmdT9PredictiveEditWord:
            SetStopProcessFocus( ETrue, EFalse );
            HandleChangeInFocus();
#ifdef RD_INTELLIGENT_TEXT_INPUT
                RemoveSuggestedAdvanceCompletionL();
#endif // RD_INTELLIGENT_TEXT_INPUT   
            LaunchEditWordQueryL();
            SetStopProcessFocus(EFalse);
#ifdef RD_SCALABLE_UI_V2     
            if( iFepFullyConstructed && iFepPluginManager )
                {
                iFepPluginManager->ResetMenuState();            
                }
#endif 
            break;
        case EAknCmdT9PredictiveMatches:
            SetStopProcessFocus(ETrue,EFalse);
            LaunchMatchesPopupListL();
            SetStopProcessFocus(EFalse);
            break;
#ifndef RD_INTELLIGENT_TEXT_INPUT
        case EAknCmdPredictiveAutoWord:
             if ( !iIsAutoCompleteOn )
                {
                SetStopProcessFocus(ETrue);
                LaunchConfirmationNoteL(R_AVKON_PREDICTIVE_AUTOCOMPLETION_ACTIVATED_NOTE);
                SetStopProcessFocus(EFalse);
                }
             iIsAutoCompleteOn = ETrue;      
         	 iSharedDataInterface->SetPredictiveTextAutoCompleteOn();  			                 	 
#ifdef  RD_SCALABLE_UI_V2       	 
            if(iFepPluginManager->PluginInputMode() == EPluginInputModeItut)
                {
                SendEventsToPluginManL( EPluginCloseMode, EFalse );
                HandleChangeInFocus();
                }
#endif
	 		 UpdateIndicators();
        	 break;
        case EAknCmdPredictiveNormal:
             if ( iIsAutoCompleteOn )
                {
                SetStopProcessFocus(ETrue);
                LaunchConfirmationNoteL(R_AVKON_PREDICTIVE_AUTOCOMPLETION_DEACTIVATED_NOTE);
                SetStopProcessFocus(EFalse);
                }
         	 iIsAutoCompleteOn = EFalse;
         	 iSharedDataInterface->ResetPredictiveTextAutoCompleteOn();        	 
        	 UpdateIndicators();
        	 break;
#endif
        case EAknCmdInputLanguage:
#ifdef RD_INTELLIGENT_TEXT_INPUT
        case EAknFepCmdPredInputLanguage:
#endif //RD_INTELLIGENT_TEXT_INPUT
            SetStopProcessFocus(ETrue, EFalse);
            HandleChangeInFocus();
#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifdef FF_DUAL_LANGUAGE_SUPPORT 
            	// Currently dual language feature would work correctly for ITUT and Half Qwerty products 
                // Hence the dual language settings should also be displayed only for ITUT and Halfqwerty products
                if(iKeyboardType == EPtiKeyboard12Key || iKeyboardType == EPtiKeyboardHalfQwerty )
                    LaunchDualLanguageSettingDialogL();
                else 
					{ 
					LaunchLanguagesPopupListL(aCommandId == EAknCmdInputLanguage);		            
		            iStopProcessFocus = EFalse; 
					}             
#else // FF_DUAL_LANGUAGE_SUPPORT
            LaunchLanguagesPopupListL(aCommandId == EAknCmdInputLanguage);
            //HandleChangeInFocus();
            iStopProcessFocus = EFalse;
#endif // FF_DUAL_LANGUAGE_SUPPORT
#else            
            LaunchLanguagesPopupListL(aCommandId == EAknCmdInputLanguage);
            HandleChangeInFocus();
            iStopProcessFocus = EFalse;
#endif
            break;
        case EAknEditMenuCmdCutText:
             StartCcpuModeL(EFalse);
             if (WesternPredictive())
                {                
                // Remove underlining from predicted word when focus is re-gained.
                SetCcpuFlag(ECcpuStateUncommitWhenFocused);                 
                }                                     
             SetCcpuFlag(ECcpuStateIgnoreNextFocusChange);                            
             SetCcpuFlag(ECcpuStateEdwinInSelectionMode);                       
             break;            
        case EAknEditMenuCmdCopyText:                               
             StartCcpuModeL(ETrue);
             if (WesternPredictive())
                {                
                // Remove underlining from predicted word when focus is re-gained.
                SetCcpuFlag(ECcpuStateUncommitWhenFocused);                 
                }                        
             SetCcpuFlag(ECcpuStateIgnoreNextFocusChange);               
             SetCcpuFlag(ECcpuStateEdwinInSelectionMode);                
             break;
        case EAknFepSoftkeyStartCopy:
        case EAknFepSoftkeyStartCut:        
             {
             ResetCcpuFlags();
             if (aCommandId == EAknFepSoftkeyStartCopy)
                {
                SetCcpuFlag(ECcpuStateCopy);                
                }
             else
                {
                SetCcpuFlag(ECcpuStateCut);
                }
                
             __ASSERT_DEBUG(iUiInterface->SoftkeysExist(),
                            AknFepPanic(EAknFepPanicNoHandleToFepAwareTextEditor));
                 
            TBool update = UpdateCBALabelL(ELeftSoftkeyIndex, KNullCommandId, R_TEXT_SOFTKEY_EMPTY);
            update |= UpdateCBALabelL(ERightSoftkeyIndex, EAknFepSoftkeyCloseWindow, 
                                      R_AKNFEP_SOFTKEY_CCPU_CANCEL);
            update |= UpdateCBALabelL( EMiddleSoftkeyIndex, KNullCommandId, R_TEXT_SOFTKEY_EMPTY ); 
            if (update)
                {
                iUiInterface->DrawSoftkeysNow();
                }                           
             }
             break;   
        case EEikCmdEditCopy:
        case EEikCmdEditCut:
            {
#ifdef RD_SCALABLE_UI_V2            
     if (iFepPluginManager && 
        (iFepPluginManager->PluginInputMode() == EPluginInputModeItut ||
         iFepPluginManager->PluginInputMode() == EPluginInputModeFSQ ||
         iFepPluginManager->PluginInputMode() == EPluginInputModeFingerHwr || 
         iFepPluginManager->PluginInputMode() == EPluginInputModePortraitFSQ ) )
        {
        iFepPluginManager->SetMenuState(EFalse);     
        }

#endif //RD_SCALABLE_UI_V2             
            SetStopProcessFocus(ETrue, EFalse);
            HandleCopyCutEventL(aCommandId);
            SetStopProcessFocus(EFalse);            
            }
            break;
        case EEikCmdEditPaste:
           	{
           	// We have a hindi character in clipboard which can form ligature with the existing text
           	// if pasted. While the input language is English, the cursor aligmment check would not happen.
           	// When selecting Options->Paste, the Fep context at this point is for the menu.
           	// iInputCapabilities for editor context is retrived from CCoeEnv and is restored back
           	// after doing the operation, else FepAwareTextEditor will be true in menu context.
           	if ( (!BidiCursorRequired()) && (!WesternPredictive()) )
           		{
	        	TCoeInputCapabilities origInputCapabilities = iInputCapabilities;
	        
	        	CCoeEnv* coeEnv = CCoeEnv::Static();
	        	iInputCapabilities = static_cast<const CCoeAppUi*>(coeEnv->AppUi())->InputCapabilities();
        	
        		if( !(iInputCapabilities.SupportsSecretText()) )		        
		        	{
		    	    DoCursorDirectionCheckL();
			        }	
    	       	iInputCapabilities = origInputCapabilities;
	           	}
           	}     
        	break;
            
        case EAknCmdEditMenuSctSelected:
            {
            SetCcpuFlag(ECcpuStateSupressCursorMoveToEnd);
            HandleChangeInFocus();  // to re-establish contact with editor

            TBuf<ESingleCharacter> charAsDesc(ESingleCharacter);
            for ( TInt ii = 0; ii < iSctEditChars->Length(); ii++)
                {
                charAsDesc[0]=(TText) (*iSctEditChars)[ii];
                if (charAsDesc[0] == TText(0x000A) // 0x000A is line feed
                 || charAsDesc[0] == CEditableText::EParagraphDelimiter) 
                    {
                    // This is line feed character. Post it to editor and let it decide
                    // if it is valid.
                    if (!(iAknEditorFlags & EAknEditorFlagFindPane) && iSctEditChars->Length() == 1)                              
                        {
                        SimulateKeyEventL(EKeyEnter);
                        SimulateKeyEventL(EKeyF19); // Asyncronous case update                                                                                  
                        }
                    }                                                        
                else
                    {
                    if (CharIsValidInEditor(charAsDesc[0]))
                    {
                    TCursorSelection cursorSelection(0,0);
                    if ((iAknEditorFlags & EAknEditorFlagMTAutoOverwrite) && 
                                    iMode == ELatin && !WesternPredictive())
                        {
                        TInt edSize = iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep();
                        iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(iUncommittedText);
                        if (iUncommittedText.iCursorPos < edSize && iUncommittedText.Length() == 0)
                            {
                            cursorSelection = TCursorSelection(iUncommittedText.iCursorPos + 1, 
                                                               iUncommittedText.iAnchorPos);
                            }
                        }
                        
                    if (EditorHasFreeSpace())
                        {
                        InsertTextFromDialogL(charAsDesc, cursorSelection);
                        }
                                           
                    }
                  } 
                 
                }
            } 
            break;
        case EAknCmdTaskSwapper:
            SendEventsToPluginManL( EPluginFaseSwap, ETrue );
            break;
            
        case EAknCmdEditMenuClose:
            {
            // Handle destruction of menu
            StopDisplayingMenuBar();     
            }            
        break;
#ifdef RD_INTELLIGENT_TEXT_INPUT
        // Predictive QWERTY (XT9) changes ---->
         case EAknFepCmdPredMatches:
         {
         	HandleChangeInFocus();  // to re-establish contact with editor
	        // Focus current active word in list ---->
	        TInt wordToFocus;
	        iPtiEngine->HandleCommandL( EPtiCommandUserActionGetCurrentIndexOfCandidates, &wordToFocus );
	        iSListLaunchedFromMenu = ETrue;
	        TKeyEvent candLaunch = {EKeyDownArrow, EStdKeyDownArrow, 0, 0};
			CEikonEnv::Static()->WsSession().SimulateKeyEvent(candLaunch);
	        //LaunchCandidatePopupListL( wordToFocus );
	       }
	        break;
        case EAknFepCmdPredSettings:
        	LaunchPredictiveSettingDialogL();
        	break;
        /*
	    This code is the part of fixing TSW Error 
	    "Matches" and "insert word" options are not available under Edit Menu.
	    This is For Insert Word Functionality*/	
        case EAknEditMenuCmdInsertWord:
        	HandleChangeInFocus();
            LaunchInsertWordQueryL(KNullDesC, TCursorSelection(0,0));
        	break;
#ifdef __USER_DICTIONARY_EDITING__ 
        case EAknFepCmdPredEditUserDictionary:
            if ( IsAutoCompleteOn() )
                {
                HandleChangeInFocus();
                RemoveSuggestedAdvanceCompletionL();
                }
            UiInterface()->LaunchUserDictEditDialogL();
            break;
#endif //__USER_DICTIONARY_EDITING__ 
        
        case EAknFepCmdPredHelp:
	        // Help ID:PREDINPUT_HLP_INPUT
			{					
            const TInt KArrayGranularity = 3;
            CArrayFix<TCoeHelpContext>* contexts =
                    new (ELeave) CArrayFixFlat<TCoeHelpContext>( KArrayGranularity );
	    	CleanupStack::PushL ( contexts );
	    	TUid appuid = { 0x100058EC };
	    	contexts->AppendL( TCoeHelpContext( appuid, KPREDINPUT_HLP_INPUT ) );
	    	CleanupStack::Pop( contexts );
	    	HlpLauncher::LaunchHelpApplicationL( CEikonEnv::Static()->WsSession(), contexts );			
			}
	        break;
       // Predictive QWERTY (XT9) changes <----
#endif //RD_INTELLIGENT_TEXT_INPUT
        // add for phrase creation user db view.
        case EAknCmdUserDBDlg:
            {
            HandleChangeInFocus();
            LaunchUserDBDlgL();
           }            
        break;
        case EPenInputCmdPreviewOn:
            {
            iSharedDataInterface->SetFepShowVkbPreviewStatus(KTouchInputPreviewOn);
            SendEventsToPluginManL(EPluginPreview, KTouchInputPreviewOn);  
            }
        break;
        case EPenInputCmdPreviewOff:
            {
            iSharedDataInterface->SetFepShowVkbPreviewStatus(KTouchInputPreviewOff);
            SendEventsToPluginManL(EPluginPreview, KTouchInputPreviewOff);    
            }
        break;
        case EPenInputCmdRecognitionWithDictionary:
            {
            SetStopProcessFocus(ETrue);
            HandleChangeInFocus();
            LaunchRecognitionWithDictionaryPopupListL();
            HandleChangeInFocus();
            iStopProcessFocus = EFalse;
            }
            break;
        case EPenInputCmdHwrInputToPortrait:
            {
            SendEventsToPluginManL( EPluginCloseMode, EFalse );
            HandleChangeInFocus();                        
            SendEventsToPluginManL( EPluginSwitchToPortrait);
            }
            break;
        case EPenInputCmdHwrInputToLandscape:
            {
            SendEventsToPluginManL( EPluginCloseMode, EFalse );
            HandleChangeInFocus();                        
            SendEventsToPluginManL( EPluginSwitchToLandscape);
            }
            break;
        /*case EPenInputCmdSwitchToVkeyBasedInput:
            {
            SendEventsToPluginManL( EPluginCloseMode, EFalse );
            HandleChangeInFocus();            
            SendEventsToPluginManL( EPluginSwitchMode);
            }
            break;*/
        case EPenInputCmdWritingSpeed:
            {
            SetStopProcessFocus(ETrue,EFalse);
            LaunchWritingSpeedPopupListL();
			SetStopProcessFocus(EFalse);
            }
            break;
        case EPenInputCmdGuidingLine:
            {
            SetStopProcessFocus(ETrue,EFalse);
            LaunchGuidingLinePopupListL();
			SetStopProcessFocus(EFalse);
            }
            break;
        default:
            break;
        }
        
        
    StopDisplayingMenuBar();
    }

void CAknFepManager::SetEmphasis(CBase* /*aMenuControl*/,TBool aEmphasis)
    {
    if (aEmphasis)
        { // opening main menu pane
        SetFlag(EFlagMenuPaneVisible);
        if (IsFlagSet(EFlagInsideInlineEditingTransaction))
            {
            SetFlag(EFlagInlineEditInBackground);
            }
        }
    else
        {
		// The pointer to be cleared when the callback is received on leaving the focus from the menu pane.
#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
        iOptionsMenuBar = NULL;
#endif
        ClearFlag(EFlagMenuPaneVisible | EFlagRemoveMatchesMenuItem | EFlagInlineEditInBackground);
        }
    }

void CAknFepManager::DynInitMenuBarL(TInt aResourceId, CAknFepUiInterfaceMenuBar* aMenuBar)
    {
    if (iFepFullyConstructed && IsFlagSet(EFlagLaunchEditMenu))
        {
        aMenuBar->ReplaceAllMenuPanes(R_AVKON_EDIT_MODE_MENU);
        }
        
#ifdef RD_SCALABLE_UI_V2        
    if (iFepFullyConstructed &&
        aResourceId == R_AVKON_PENINPUT_OPTION_MENU_BAR)
        {
        aMenuBar->ReplaceAllMenuPanes(R_AVKON_TOUCH_TOUCHINPUT_MENU);
        }
#endif //RD_SCALABLE_UI_V2        
        
    }

#ifdef RD_SCALABLE_UI_V2
void CAknFepManager::DynInitTouchMenuPaneL(CAknFepUiInterfaceMenuPane* aMenuPane)
    {
    if ( !iFepFullyConstructed )
        {
        return;
        }


    if (iFepManState != EAknFepStateNull &&
        iInputCapabilities.FepAwareTextEditor() &&
        !(iAknEditorFlags & EAknEditorFlagFindPane))
        {
        // 'Input language' menu item on option menu isn't supported in Japanese variant.
        if (!IsFeatureSupportedJapanese())
            {
            // Insert 'Input language' item to incoming menu if editor is active on screen and
            // menu contains 'Help' or 'Exit' item.
            if (!IsOnlyNumericPermitted())
                {
                TInt helpIndex;
                if (aMenuPane->MenuItemExists(EAknCmdHelp, helpIndex) ||
                    aMenuPane->MenuItemExists(EAknCmdExit, helpIndex))
                    {
                    AddInputLanguageItemL(aMenuPane, helpIndex);
                    }
                }
            }
        }

    TInt index;
    if(aMenuPane->MenuItemExists(EAknCmdEditItemPlaceHolder, index))
	    {
		aMenuPane->DeleteMenuItem(EAknCmdEditItemPlaceHolder);	   
	    }

    ClearFlag(EFlagLaunchEditMenu);
            
    DimInputmodeTouchMenuItems(aMenuPane);
    }
#endif //RD_SCALABLE_UI_V2
void CAknFepManager::DynInitMenuPaneL(TInt aResourceId, CAknFepUiInterfaceMenuPane* aMenuPane)
    {
    if ( !iFepFullyConstructed )
        {
        return;
        }

#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
    CAknEdwinState* editorState = NULL;
    if (iInputCapabilities.FepAwareTextEditor())
        {
        editorState = EditorState();
        }

    if (editorState)
        {
        iOptionsMenuBar = iUiInterface->EditorStateMenuBar(editorState);
        }
#endif    
    
    SetCcpuFlag(ECcpuStateIgnoreNextEdwinSyncEvent);

    if (iFepManState != EAknFepStateNull &&
        iInputCapabilities.FepAwareTextEditor() &&
        !(iAknEditorFlags & EAknEditorFlagFindPane))
        {
        // 'Input language' menu item on option menu isn't supported in Japanese variant.
        if (!IsFeatureSupportedJapanese())
            {
            // Insert 'Input language' item to incoming menu if editor is active on screen and
            // menu contains 'Help' or 'Exit' item.
            //Also, no need for writing language/ITI options in telephony's dialer
            if ( (RProcess().SecureId().iId != KPhoneSecureId) && !IsOnlyNumericPermitted() )
                {
#ifdef RD_INTELLIGENT_TEXT_INPUT                
                //This method gets called multiple times - first before the help and exit
                //commands are populated and finally after these items are added.
                //So if we have already added the input options menu and/or writing language menu,
                //remove it. It will get added again to the correct place with the below code.
                //No need to handle following menu types here.
                if ( !( R_AVKON_EDIT_MODE_MENU == aResourceId || 
                        R_AKNFEP_PRED_INPUT_OPTIONS_SUBMENU == aResourceId ||
                        R_AVKON_PREDICTIVE_TEXT_MENU_T9 == aResourceId ||
                        R_AVKON_INPUT_MODE_SUB_MENU == aResourceId ||
                        R_AKNFEP_EDIT_SUBMENU == aResourceId ||
                        R_AVKON_TOUCH_TOUCHINPUT_MENU == aResourceId ) 
                // Add this condition for adding "input options" into the option menu of FSQ.
                        || ( R_AVKON_TOUCH_TOUCHINPUT_MENU == aResourceId
                             && iFepPluginManager
                             && iFepPluginManager->IsSupportITIOnFSQ() )
                        )                        
                    {
                    FindAndRemoveInputOptionsMenuItemL( aMenuPane );
                    FindAndRemoveEditSubMenuItemL (aMenuPane);
                    AddInputOptionsMenuItemL( aMenuPane );
                    }                                	        	
#else
                TInt helpIndex;
		if ( aMenuPane->MenuItemExists(EAknCmdHelp, helpIndex) ||
                     aMenuPane->MenuItemExists(EAknCmdExit, helpIndex) )
                    {
                    AddInputLanguageItemL(aMenuPane, helpIndex);
                    }
                       	
#endif //RD_INTELLIGENT_TEXT_INPUT
                }
            }
#ifdef RD_INTELLIGENT_TEXT_INPUT
        if ( IsChineseInputLanguage() && 
                ( iKeyboardType == EPtiKeyboardQwerty4x10 ||
                iKeyboardType == EPtiKeyboardQwerty3x11 ) )
            {
            // the follow added for phrase creation
            TInt iptlanguage;
            if ( !iIsUserdbdlgActive )
                {
                aMenuPane->MenuItemExists( EAknCmdInputLanguage, iptlanguage );
                AddUserDBDlgItemL( aMenuPane, iptlanguage );
                }
            }
#endif

        if (!(aResourceId == R_AVKON_EDIT_MODE_MENU ||
            aResourceId == R_AVKON_TOUCH_TOUCHINPUT_MENU ||
            aResourceId == R_AVKON_TOUCH_INDICATOR_EDIT_MODE_MENU))
            {
            AddEditSubmenuL(aMenuPane);
            }
        }
		
		//HWR Hindi, Training application is not available,
        //Disable Menu option
        if(aResourceId == R_AVKON_TOUCH_TOUCHINPUT_MENU && 
        	iLanguageCapabilities.iInputLanguageCode == ELangHindi)
        	{
        	aMenuPane->SetItemDimmed(EPenInputCmdHwrTraining, ETrue);
        	}
    TInt index;
    if(aMenuPane->MenuItemExists(EAknCmdEditItemPlaceHolder, index))
	    {
		aMenuPane->DeleteMenuItem(EAknCmdEditItemPlaceHolder);	   
	    }
#ifdef RD_SCALABLE_UI_V2
    TInt oldPermitModes = -1;
    if (R_AVKON_TOUCH_TOUCHINPUT_MENU == aResourceId )
        {
        oldPermitModes = iPermittedInputModes;
        
        if( iFepPluginManager != NULL )
            {
            iPermittedInputModes = iFepPluginManager->PreviousPermitMode();
            }
        }
    if( iFepPluginManager != NULL )
        {
        iFepPluginManager->InitMenuPaneL( iRememberEditorState, aMenuPane, aResourceId );
        }
    if (oldPermitModes != -1)
        {
        iPermittedInputModes = oldPermitModes;    
        }
#endif //  RD_SCALABLE_UI_V2      
    switch (aResourceId)
        {
        case R_AVKON_TOUCHINPUT_PREVIEW:
            {
            InitPreviewMenuPane(aMenuPane);     
            } 
        break;
        case R_AVKON_EDIT_MODE_MENU:
        case R_AVKON_TOUCH_INDICATOR_EDIT_MODE_MENU:
            {
            ClearFlag(EFlagLaunchEditMenu);
            
   		  //call comes here when edit key or '*' is pressed and displays avkon edit menu            
		  //moving only the essential condition checks in EditSubmenuInUse() here
		  //no need of checking iSharedDataInterface->EditSubmenuInUse() because for devices with edit key
		  //this would return 0 and would not populate CUT and COPY option

        	TBool tempFlag = ETrue;
	        if ( ( ( RProcess().SecureId().iId == KPhoneSecureId ) && (iQwertyInputMode) ) ||
	             ( ExtendedInputCapabilities() & 
                 CAknExtendedInputCapabilities::EForceEditSubmenuStatusFlagOff)
	        	  
	           )
	        	{
	        	tempFlag = EFalse;         		
	        	}

            if (tempFlag)
                {
                TInt posit(0);
                    
                if (!(iAknEditorFlags & EAknEditorFlagFindPane) &&
                	iInputCapabilities.FepAwareTextEditor() &&
                    aMenuPane->MenuItemExists(EEikCmdEditCut, posit) &&
                    !IsOnlyNumericPermitted() 
                    )
                    { 
                    if (iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep() > 0 )
                    	{
						if (iRememberEditorState && iRememberEditorState->CcpuState() &&
							!iRememberEditorState->CcpuState()->CcpuCanCopy()&& 
							PluginInputMode() != EPluginInputModeItut )                      
							{                                                       
							TBuf<KMaxFileName> titleStr;
							StringLoader::Load(titleStr, R_AKNFEP_OPTIONS_EDIT_MENU_CUT_TEXT);                          
																
							CAknFepUiInterfaceMenuPane::SItemData itemData;
						
							itemData.iCommandId = EAknEditMenuCmdCutText;
							itemData.iCascadeId = 0;                        
							if (titleStr.Length())
								{
								itemData.iText.Copy(titleStr.Mid(1, titleStr.Length() - 1));                                                
								}
							itemData.iFlags = 0;                        
							aMenuPane->InsertMenuItemL(itemData, posit);            
											
							titleStr.Zero();                                            
							itemData.iText.Zero();
							StringLoader::Load(titleStr, R_AKNFEP_OPTIONS_EDIT_MENU_COPY_TEXT);
							
							itemData.iCommandId = EAknEditMenuCmdCopyText;                      
							if (titleStr.Length())
								{
								itemData.iText.Copy(titleStr.Mid(1, titleStr.Length() - 1));                                                
								}
							aMenuPane->InsertMenuItemL(itemData, posit);                                                    
							}
						else //Hack for browser in case there are some text already selected
							{
							 if (iRememberEditorState && iRememberEditorState->CcpuState() )
								 {
								  if(iRememberEditorState->CcpuState()->CcpuCanPaste())
									  aMenuPane->SetItemDimmed(EEikCmdEditPaste,EFalse);
								  if(iRememberEditorState->CcpuState()->CcpuCanCopy())
									  aMenuPane->SetItemDimmed(EEikCmdEditCopy,EFalse);
								  if(iRememberEditorState->CcpuState()->CcpuCanCut())
									  aMenuPane->SetItemDimmed(EEikCmdEditCut,EFalse);
								 }
							 }
                    	}
                    //if the document size is 0 and there are items in clipboard then dislay Paste item
                    if ( iRememberEditorState && iRememberEditorState->CcpuState() &&
                         iRememberEditorState->CcpuState()->CcpuCanPaste())
                        aMenuPane->SetItemDimmed(EEikCmdEditPaste,EFalse);
                    
                    } 
                
                }                       
            
            DimInputmodeMenuItems(aMenuPane);
#ifdef RD_INTELLIGENT_TEXT_INPUT
            //Is this an edit menu launched for a western language?
            if ( R_AVKON_EDIT_MODE_MENU == aResourceId && !IsChineseInputLanguage() 
                    && ELangJapanese != iLanguageCapabilities.iInputLanguageCode )
                {
                //If this is edit menu (launched from '*' or edit key) and predictive input is allowed,
                    //we need to remove "Input language" and add "Input options" menu item. 
                if (!IsOnlyNumericPermitted() && IsLanguageSupportPrediction() && !(iAknEditorFlags & EAknEditorFlagNoT9))
                    {
                    TInt menuitemPos(0);
                    if(aMenuPane->MenuItemExists(EAknCmdEditInsertSmiley,menuitemPos)) // insert after smiley if exist
                        {
                        AddPredictiveModeEditMenuL(aMenuPane,menuitemPos+1);
                        }
                    else if(aMenuPane->MenuItemExists(EAknCmdEditInsertSymbol,menuitemPos)) // insert after smybol if exist
                        {
                        AddPredictiveModeEditMenuL(aMenuPane,menuitemPos+1);
                        }
                    }
                }
#endif //RD_INTELLIGENT_TEXT_INPUT

            }
            break;
                        
        case R_AVKON_PREDICTIVE_TEXT_MENU_T9:
            {
            if( (iPtiEngine->NumberOfCandidates() <= 1) && !IsFlagSet(EFlagRemoveMatchesMenuItem ) )
			   {
			   SetFlag( EFlagRemoveMatchesMenuItem );
			   }

            if (iMode != ELatin)
                {
                aMenuPane->SetItemDimmed(EAknCmdT9PredictiveInsertWord, ETrue);
                }
            if (!IsFlagSet(EFlagInlineEditInBackground))
                {
                aMenuPane->SetItemDimmed(EAknCmdT9PredictiveMatches, ETrue);
                aMenuPane->SetItemDimmed(EAknCmdT9PredictiveEditWord, ETrue);
                }
            else if (IsFlagSet(EFlagRemoveMatchesMenuItem))
                {
                aMenuPane->SetItemDimmed(EAknCmdT9PredictiveMatches, ETrue);
                }
            break;
            }
        case R_AVKON_INPUT_MODE_SUB_MENU:
            {
            if (IsChineseInputLanguage())
                {
                DoChineseSubMenu(aMenuPane);
                }
            break;
        case R_AKNFEP_EDIT_SUBMENU:
            DisableEditSubmenuItems(aMenuPane);
            break;
            }
#ifdef RD_INTELLIGENT_TEXT_INPUT
            // Predictive QWERTY (XT9) changes ---->
        case R_AKNFEP_PRED_INPUT_OPTIONS_SUBMENU:
        	{
            	// This part of code fixing TSW Errors
            	// "Edit word" option is not available under edit menu in ITU-T mode.
            	// "Insert word" Should not be available QWERTY mode.
          	if (IsKoreanInputLanguage() || 
          			CurrentInputLangCode() == ELangPrcChinese || 
          			CurrentInputLangCode() == ELangTaiwanChinese || 
          			CurrentInputLangCode() == ELangHongKongChinese  )
          		{
          		// No text prediction for korean.
          		aMenuPane->SetItemDimmed(EAknFepCmdPredActivate, ETrue );
          		aMenuPane->SetItemDimmed(EAknFepCmdPredDeactivate,ETrue );
                aMenuPane->SetItemDimmed( EAknFepCmdPredSettings, ETrue );
          		}
          	else 
                {
                aMenuPane->SetItemDimmed( EAknFepCmdPredSettings, EFalse );
                if(iSharedDataInterface->PredictiveTextOn())
                    {
                    aMenuPane->SetItemDimmed( EAknFepCmdPredActivate, ETrue );					
                    }
                else
                    {
                    aMenuPane->SetItemDimmed(EAknFepCmdPredDeactivate,ETrue ); 
                    }	
                }
        	} 
          break;
            
#endif //RD_INTELLIGENT_TEXT_INPUT
        default:
            break;
        }
#ifdef RD_INTELLIGENT_TEXT_INPUT
	/*
    This code is the part of fixing TSW Error Edit Menu : 
    "Matches" and "insert word" options are not available under Edit Menu.
    This is To make visible/invisible the Insert Word in Optios menu & Edit Menu*/
    iIsLastResourceEditMenu = aResourceId;
#endif  
    }

#ifdef RD_INTELLIGENT_TEXT_INPUT
void CAknFepManager::FindAndRemoveInputOptionsMenuItemL( CAknFepUiInterfaceMenuPane* aMenuPane )
    {
    //Logic : 
    //Loop through each item in menu pane and check if the data
    //CEikMenuPaneItem::SData::iCascadeId == R_AKNFEP_PRED_INPUT_OPTIONS_SUBMENU
    //or if CEikMenuPaneItem::SData::iCommandId == EAknCmdInputLanguage
    //If so, delete that item.
    TInt numOfMenuItems = aMenuPane->NumberOfItemsInPane();
    CEikMenuPane* menuPane = aMenuPane->GetMenuPane();
    for (TInt index(0); index < numOfMenuItems ; ++index )
        {
        CEikMenuPaneItem::SData itemData = menuPane->ItemDataByIndexL( index );
        if ( itemData.iCascadeId == R_AKNFEP_PRED_INPUT_OPTIONS_SUBMENU ||
             itemData.iCommandId == EAknCmdInputLanguage ||
             itemData.iCascadeId == R_AVKON_INPUT_MODE_SUB_MENU ||
           ( IsChineseInputLanguage() && iSharedDataInterface->QwertyInputMode() && 
             itemData.iCommandId == EChinFepCmdModeCangJieOption ) )
            { 
            menuPane->DeleteBetweenMenuItems( index, index );
            //Assumption - there can only be one such item in the menu pane
            //Hence we can return without going through the remaining items
            numOfMenuItems = aMenuPane->NumberOfItemsInPane();
            index--;
            }
        }
    }

void CAknFepManager::FindAndRemoveEditSubMenuItemL( CAknFepUiInterfaceMenuPane* aMenuPane )
    {
    //Logic : 
    //Loop through each item in menu pane and check if the data
    //CEikMenuPaneItem::SData::iCascadeId == R_AKNFEP_EDIT_SUBMENU
    //If so, delete that item.
    TInt numOfMenuItems = aMenuPane->NumberOfItemsInPane();
    CEikMenuPane* menuPane = aMenuPane->GetMenuPane();
    for (TInt index(0); index < numOfMenuItems ; ++index )
        {
        CEikMenuPaneItem::SData itemData = menuPane->ItemDataByIndexL( index );
        if ( itemData.iCascadeId == R_AKNFEP_EDIT_SUBMENU )
            {
            menuPane->DeleteBetweenMenuItems( index, index );
            //Assumption - there can only be one such item in the menu pane
            //Hence we can return without going through the remaining items
            return;
            }
        }
    }

void CAknFepManager::AddInputOptionsMenuItemL( CAknFepUiInterfaceMenuPane* aMenuPane )
    {
    //Logic:
    //Appropriate location is :
    //  if Avkon Help command exists, then immediately above it
    //  else if Avkon Exit command exists, then immediately above it
    //  else if there are 3 or more items in the Options menu, then 3rd from last
    //  else it is the last item. 
    TInt posToInsertItem( 0 );
    //MenuItemExists() will return the index of the item in 2nd parameter
    //But if item is not found, it is returning the total number of items.
    if ( ( aMenuPane->MenuItemExists(EAknCmdHelp, posToInsertItem) ) ||
         ( aMenuPane->MenuItemExists(EAknCmdExit, posToInsertItem) ) )
        {
        //do nothing - we already have posToInsertItem with required value 
        }
    else 
        {
        TInt numOfMenuItems = aMenuPane->NumberOfItemsInPane();
        if ( numOfMenuItems > 2 )
            {
            //We should add the new item as third from last, after ignoring
            //any dimmed items in the menu pane
            TInt numOfUnDimmedItems(0);
            //loop from bottom for better performance 
            for (TInt index(numOfMenuItems-1); index >= 0 ; --index )
                {
                CEikMenuPaneItem::SData itemData = aMenuPane->GetMenuPane()->ItemDataByIndexL( index );
                if ( !(itemData.iFlags & EEikMenuItemDimmed) )
                    {
                    ++numOfUnDimmedItems;
                    }
                if ( numOfUnDimmedItems == 2 )
                    {
                    //We have our position - save it and break from this loop
                    if ( index > 1 )
                        {
                        posToInsertItem = index;
                        }
                    else 
                        {
                        //There are two or less items on this menu that will be displayed
                        //Hence, push ITI options to the end of the list:
                        posToInsertItem = numOfMenuItems;
                        }
                    break;
                    }
                }
            }
        else 
            {
            posToInsertItem = numOfMenuItems;
            }
        }
    TInt inputOptionsPos = 0;
    TInt pastePos = 0;
    
    if ( aMenuPane->MenuItemExists( EEikCmdEditPaste, pastePos ) )
        {
        CEikMenuPaneItem::SData& itemData = aMenuPane->GetMenuPane()->ItemDataByIndexL( pastePos );
        if ( !( itemData.iFlags & EEikMenuItemDimmed ) )
        	{
            inputOptionsPos = pastePos + 1; 
        	}
        }
   
    //Now we have the position at which we need to insert the menu item. 
    if (
#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
    iQwertyInputMode &&
#endif   		
    IsLanguageSupportPrediction() && !(iAknEditorFlags & EAknEditorFlagNoT9))
        {
        AddPredictiveModeOptionsL( aMenuPane, inputOptionsPos );
        }
    else
        {
        AddInputLanguageItemL(aMenuPane, posToInsertItem);
        }
    }

// -----------------------------------------------------------------------------
// To check if the menu with the CascadeId exist in menupane
// -----------------------------------------------------------------------------
TBool CAknFepManager::MenuItemExistL( CAknFepUiInterfaceMenuPane* aMenuPane, 
									 TInt aCascadeId, TInt &aPosition )
    {
    TInt numOfMenuItems = aMenuPane->NumberOfItemsInPane();
    CEikMenuPane* menuPane = aMenuPane->GetMenuPane();

    TInt index;
    for( index = 0; index < numOfMenuItems ; ++index )
        {
        CEikMenuPaneItem::SData& itemData = menuPane->ItemDataByIndexL( index );
        if( itemData.iCascadeId == aCascadeId )
            {
            aPosition = index;
            return ETrue;
            }
        }
    return EFalse;
    }

#endif // RD_INTELLIGENT_TEXT_INPUT


void CAknFepManager::AddEditSubmenuL(CAknFepUiInterfaceMenuPane* aMenuPane)
    {
    // Adds edit-submenu to options menu when required contidions are met.
    // Edit-submenu is added if:
    //  1. There is active editor on screen
    //  2. Editor contains text or there is text in clipboard.
    //  3. Editor is not in Find Pane.
    //
    // Since menu will have focus, we also need to remeber certain values
    // to be able to access undelying editor attributes.
    
    CAknEdwinState* editorState = EditorState();
    iEditorCcpuStatus = 0;        
    if (editorState && editorState->CcpuState())
        {
        iEditorCcpuStatus = EditorCcpuStatus(editorState);    
        }
    else
        {
        // The editor doesn't have ccpu-capability, doen't display the menu.
        return; 
        }            
    TInt index;
    if (aMenuPane->MenuItemExists(EAknCmdEditTextMenu, index))
        {
        return;    
        }
        
    iRememberLength = iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep();      
    
    if (iRememberLength > 0 || (iEditorCcpuStatus & ECcpuStatusFlagCanPaste))
        {
		if (aMenuPane->MenuItemExists(EAknCmdEditItemPlaceHolder, index) ||
#ifdef RD_INTELLIGENT_TEXT_INPUT
            MenuItemExistL( aMenuPane, R_AKNFEP_PRED_INPUT_OPTIONS_SUBMENU, index ) ||
#endif //RD_INTELLIGENT_TEXT_INPUT 
            aMenuPane->MenuItemExists(EAknCmdInputLanguage, index) ||
		    aMenuPane->MenuItemExists(EAknCmdHelp, index) || 
            aMenuPane->MenuItemExists(EAknCmdExit, index))  
            {
            TBuf<KMaxFileName> titleStr;
            StringLoader::Load(titleStr, R_AKNFEP_OPTIONS_EDIT_SUBMENU_TITLE);          
        
            CAknFepUiInterfaceMenuPane::SItemData mData;
                            
            mData.iCommandId = EAknCmdEditTextMenu;
            mData.iCascadeId = R_AKNFEP_EDIT_SUBMENU;  
            mData.iFlags = 0;
            mData.iText.Copy(titleStr.Mid(1, titleStr.Length() - 1).Left(
               CAknFepUiInterfaceMenuPane::SItemData::ENominalTextLength));
                            
            aMenuPane->InsertMenuItemL(mData, index);  

 			if(aMenuPane->MenuItemExists(EAknCmdEditItemPlaceHolder, index))
    			{
    			    aMenuPane->DeleteMenuItem(EAknCmdEditItemPlaceHolder);
    			   
    			}
            // Remember editor state, because there is no access to it while
            // menu is on screen.
            iRememberEditorState = editorState;              
            }                                           
        }               
    }

    
void CAknFepManager::DisableEditSubmenuItems(CAknFepUiInterfaceMenuPane* aMenuPane)
    {
    aMenuPane->SetItemDimmed(EAknEditMenuCmdCopyText, ETrue); 
    aMenuPane->SetItemDimmed(EEikCmdEditCopy, ETrue);
    aMenuPane->SetItemDimmed(EAknEditMenuCmdCutText, ETrue);  
    aMenuPane->SetItemDimmed(EEikCmdEditCut, ETrue);
    aMenuPane->SetItemDimmed(EEikCmdEditPaste, ETrue);                                          
    
    if (iRememberLength > 0)
        {           
        if (iEditorCcpuStatus & ECcpuStatusFlagCanCopy)
            {
            aMenuPane->SetItemDimmed(EEikCmdEditCopy, EFalse);              
            }
        else
            {
            aMenuPane->SetItemDimmed(EAknEditMenuCmdCopyText, EFalse);                                      
            }   
                        
        if (iEditorCcpuStatus & ECcpuStatusFlagCanCut)
            {
            aMenuPane->SetItemDimmed(EEikCmdEditCut, EFalse);               
            }
        else
            {
            aMenuPane->SetItemDimmed(EAknEditMenuCmdCutText, EFalse);                                   
            }               
        }
                        
    if (iEditorCcpuStatus & ECcpuStatusFlagCanPaste)            
        {
        aMenuPane->SetItemDimmed(EEikCmdEditPaste, EFalse);             
        }   
    }
    

void CAknFepManager::StartCcpuModeL(TBool aCopyMode)
    {
    ResetCcpuFlags();
    if (!iUiInterface->SoftkeysExist())
        {
        iUiInterface->CreateSoftkeys(R_AVKON_SOFTKEYS_EMPTY, this);
        }
        
    TBool update = EFalse;
    if (aCopyMode)
        {
        SetCcpuFlag(ECcpuStateStartCopy);
        update = UpdateCBALabelL(ELeftSoftkeyIndex, EAknFepSoftkeyStartCopy, 
                                 R_AKNFEP_SOFTKEY_CCPU_START);      
        update |= UpdateCBALabelL( EMiddleSoftkeyIndex, EAknFepSoftkeyStartCopy, 
                                   R_AVKON_SOFTKEY_SELECT_TEXT ); 
        }
    else
        {
        SetCcpuFlag(ECcpuStateStartCut);
        update = UpdateCBALabelL(ELeftSoftkeyIndex, EAknFepSoftkeyStartCut, 
                                 R_AKNFEP_SOFTKEY_CCPU_START);               
        update |= UpdateCBALabelL( EMiddleSoftkeyIndex, EAknFepSoftkeyStartCut, 
                                   R_AVKON_SOFTKEY_SELECT_TEXT ); 
        }
        
    update |= UpdateCBALabelL(ERightSoftkeyIndex, EAknFepSoftkeyCloseWindow,
                              R_AKNFEP_SOFTKEY_CCPU_CANCEL);
        
    if (update)
        {
        iUiInterface->DrawSoftkeysNow();
        }           

    iIndicator->SetCopyMode(ETrue);
    UpdateIndicators(); // updates status of indicators
    }


TBool CAknFepManager::OkToActivateSelectionMode() const
    {
    if (iFepManState != EAknFepStateNull && 
        iInputCapabilities.FepAwareTextEditor() &&
        !(iAknEditorFlags & EAknEditorFlagFindPane))
        {               
        return ETrue;       
        }
    
    return EFalse;  
    }


TBool CAknFepManager::HandleCcpuModeKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aEventCode,
                                             TKeyResponse& aRetCode, TBool aLongPressFlag)
    {
    TKeyPressLength length = aKeyEvent.iRepeats ? ELongKeyPress : EShortKeyPress;
    TBool indicLang = TAknFepUiIndicInputManager :: IsIndicLangauge(TLanguage(
                                     iLanguageCapabilities.iInputLanguageCode))
#ifdef RD_HINDI_PHONETIC_INPUT
    || TAknFepUiIndicInputManager :: IsIndicPhoneticLanguage(TLanguage(
                                     iLanguageCapabilities.iInputLanguageCode))
#endif
    ;
    if(aKeyEvent.iCode == EKeyEscape)
        return EFalse;
    if (aEventCode == EEventKey && aKeyEvent.iCode == '*')
        {
        iStarScan = aKeyEvent.iScanCode;
        }

    if ((aKeyEvent.iScanCode == EStdKeyDevice0)
     || (aKeyEvent.iScanCode == EStdKeyDevice1))
        {
        if (HashKeySelectionInUse()
         && IsCcpuFlagSet(ECcpuStateHashDown))
            {
            if (IsCcpuFlagSet(ECcpuStateChangeToPredictionMode))
                {
                ClearCcpuFlag(ECcpuStateChangeToPredictionMode);
                if (iModeBefore == ELatin
                 || iModeBefore == EHiraganaKanji)
                    {
                    TBool preFlag = IsPredictive();
                    TryChangePredictiveInputModeL(!preFlag);
                    }
                else
                    {
                    TryChangeToModeBeforeL();
                    }
                }
            }
        // Always let sofkeys fall through.
        return EFalse;      
        }
                
    if (IsCcpuFlagSet(ECcpuStateSelectionEventPosted))
        {
        ClearCcpuFlag(ECcpuStateSelectionEventPosted);
        return EFalse;  
        }       
                        
    if (HashKeySelectionInUse())
        {       
        if (aKeyEvent.iScanCode == EStdKeyHash && aEventCode == EEventKeyDown
            && iInputCapabilities.FepAwareTextEditor())
            {
            SetCcpuFlag(ECcpuStateHashDown);
            iHashKeyMan->ResetPreviousSelectionStyleMode();
            return EFalse;
            }
            
        if (!IsOnlyNumericPermitted())
            {
            if (IsFeatureSupportedJapanese())
                {
                if (FepUI()->IsValidShiftKeyPress()
                 && aKeyEvent.iScanCode == iStarScan
                 && !iQwertyInputMode
                 && !(iMode == EKatakana && iFepManState == EAknFepStateUIActive))
                    {
                    if ( aEventCode == EEventKeyUp && !aLongPressFlag
                     && !(iMode == ELatin && WesternPredictive()
                          && iFepManState == EAknFepStateUIActive))
                        {
                        if (!IsCcpuFlagSet(ECcpuStateIgnoreStarUp))
                            {
                            LaunchSpecialCharacterTableL();
                            }
                        return EFalse;
                        }
                    else if ( aEventCode == EEventKey
                           && !aLongPressFlag
                           && length == ELongKeyPress )
                        {
                        LaunchSelectModeMenuL();
                        aRetCode = EKeyWasConsumed;
                        return ETrue;
                        }
                    }
                }
            else
                {
                if ((aKeyEvent.iScanCode == iStarScan && aEventCode == EEventKeyUp &&
                    !aLongPressFlag && !iQwertyInputMode) &&
                (!indicLang || WesternPredictive() || (iMode == ENumber || iMode == ENativeNumber) ))
                {
                // Disables *-key on korean multitapping mode
                if(IsKoreanInputLanguage() && iMode == EHangul)
                	{
                	SetCcpuFlag(ECcpuStateIgnoreStarUp);	
                	}
				
                if (!IsCcpuFlagSet(ECcpuStateIgnoreStarUp))
                    {
#ifdef RD_SCALABLE_UI_V2                    
                    if (iFepPluginManager->CurrentFepInputUI())
                        {
                        if (iFepPluginManager->CurrentFepInputUI()->HandleKeyL(EStdKeyNull, EShortKeyPress))
                            {
                            return EFalse;
                            }
                        }
#endif                        
                    LaunchSelectModeMenuL();
                    aRetCode = EKeyWasConsumed;
                    return ETrue;
                    }
                return EFalse;
                }
            else if((aKeyEvent.iScanCode == iStarScan && aEventCode == EEventKey
                && !aLongPressFlag && !iQwertyInputMode)
                && (indicLang)
                && (! WesternPredictive())
                && (length == ELongKeyPress)
                && (iMode != ENumber && iMode != ENativeNumber ))            
                {
                    LaunchSelectModeMenuL();
                    return EFalse;
                    }
                }
            }
        }
        
    if (IsCcpuFlagSet(ECcpuStateHashDown))
        {
        if (aEventCode == EEventKey)
            {                   
            if (aKeyEvent.iScanCode == EStdKeyDownArrow
                || aKeyEvent.iScanCode == EStdKeyRightArrow
                || aKeyEvent.iScanCode == EStdKeyLeftArrow
                || aKeyEvent.iScanCode == EStdKeyUpArrow)
                {
				if (IsFlagSet(EFlagInsideInlineEditingTransaction) && iMode == EHangul) 
					{
					CommitInlineEditL();
       				iPtiEngine->ClearCurrentWord();
					}
										
                if ((IsCcpuFlagSet(ECcpuStateStartCopy)) ||
                    (IsCcpuFlagSet(ECcpuStateStartCut)))
                    {
                    aRetCode = EKeyWasConsumed;             
                    return ETrue;   
                    }
                
                if (!IsCcpuFlagSet(ECcpuStateEdwinInSelectionMode))
                    {
                    // User wants to select and not to change mode, so cancel the mode change
                    // caused by initial hash key press.                                    
                    if(!(iAknEditorFlags & EAknEditorFlagFindPane))
                        {                               
                        iHashKeyMan->SetPreviousSelectionStyleModeL();                                                   
                        SetCcpuFlag(ECcpuStateEdwinInSelectionMode);
                        TKeyEvent ccpuStart = {EKeyF21, EStdKeyF21, 0, 0};
                        // to enable copy/paste support on cba. We simulate via CCoeEnv
                        // to avoid fep SimulateKeyEventL adding shift modifiers
                        CCoeEnv::Static()->SimulateKeyEventL(ccpuStart, EEventKey); 
                        }
                    }

                // If prediction is changed while pressing hash-key,
                // it go back previous prediction state.
                if (IsCcpuFlagSet(ECcpuStateChangeToPredictionMode))
                    {
                    ClearCcpuFlag(ECcpuStateChangeToPredictionMode);
                    if (iModeBefore == ELatin
                     || iModeBefore == EHiraganaKanji)
                        {
                        TBool preFlag = IsPredictive();
                        TryChangePredictiveInputModeL(!preFlag);
                        }
                    else
                        {
                        TryChangeToModeBeforeL();
                        }
                    }
                    
                if (WesternPredictive() && IsFlagSet(EFlagInsideInlineEditingTransaction))                    
                    {      
                    // This will commit inline edit if it is active for predictive mode.                              
                    return EFalse;
                    }

                aRetCode = EKeyWasNotConsumed;
                return ETrue;
                }
            else if (aKeyEvent.iScanCode == EStdKeyBackspace)   
                {                               
                if (!(aKeyEvent.iModifiers & EStdKeyLeftShift))
                    {                                   
                    // Simulate edit key + clear key functionality via hash key in no-edit-key device.              
                    TKeyEvent ccpuStart = {127, EStdKeyBackspace, EStdKeyLeftShift, 0};
                    CCoeEnv::Static()->SimulateKeyEventL(ccpuStart, aEventCode);                    
                    
                    aRetCode = EKeyWasConsumed;             
                    return ETrue;                                       
                    }
                else if (!IsCcpuFlagSet(ECcpuStateHashKeyDeleteDone))
                    {
                    // Hash key + clear key was pressed, but hash key was released before
                    // changing to selection mode. Need to return to previous mode, because
                    // user didn't want to change input mode.                       
                    iHashKeyMan->SetPreviousSelectionStyleModeL();
                    SetCcpuFlag(ECcpuStateHashKeyDeleteDone);
                    }

                // If prediction is changed while pressing hash-key,
                // it go back previous prediction state.
                if (IsCcpuFlagSet(ECcpuStateChangeToPredictionMode))
                    {
                    ClearCcpuFlag(ECcpuStateChangeToPredictionMode);
                    if (iModeBefore == ELatin
                     || iModeBefore == EHiraganaKanji)
                        {
                        TBool preFlag = IsPredictive();
                        TryChangePredictiveInputModeL(!preFlag);
                        }
                    else
                        {
                        TryChangeToModeBeforeL();
                        }
                    }

                }
            }
        else if (aKeyEvent.iScanCode == EStdKeyHash && aEventCode == EEventKeyUp)
            {
            ClearCcpuFlag(ECcpuStateHashDown
                          | ECcpuStateHashKeyDeleteDone
                          | ECcpuStateChangeToPredictionMode);
            if (IsCcpuFlagSet(ECcpuStateEdwinInSelectionMode))
                {
                SetCcpuFlag(ECcpuStateSelectionEventPosted);
                ClearCcpuFlag(ECcpuStateEdwinInSelectionMode);
                SetFlag(EFlagLongShiftKeyPress);

                TKeyEvent ccpuStart = {0, EStdKeyLeftShift, 0, 0};
                CCoeEnv::Static()->SimulateKeyEventL(ccpuStart, aEventCode);
                }

            return EFalse;
            }
        } 
    
    if (IsCcpuFlagSet(ECcpuStateStartCopy | ECcpuStateStartCut))    
        {
        if (aKeyEvent.iScanCode == EStdKeyDevice3 && aEventCode == EEventKey)
            {
            aRetCode = EKeyWasConsumed;                         
            if (IsCcpuFlagSet(ECcpuStateStartCopy))
                {
                ProcessCommandL(EAknFepSoftkeyStartCopy);
                }
            else
                {
                ProcessCommandL(EAknFepSoftkeyStartCut);                
                }                   
            return ETrue;               
            }       
        else if (aKeyEvent.iScanCode < EStdKeyLeftArrow ||
                 aKeyEvent.iScanCode > EStdKeyDownArrow) 
            {       
            aRetCode = EKeyWasConsumed;             
            return ETrue;   
            }
        }       
    else if (IsCcpuFlagSet(ECcpuStateCopy | ECcpuStateCut))
        {       
        if ((aKeyEvent.iScanCode >= EStdKeyLeftArrow) && (aKeyEvent.iScanCode <= EStdKeyDownArrow)) 
            {   
            if (!(aKeyEvent.iModifiers & EModifierShift))
                {
                TKeyEvent ccpuStart = aKeyEvent;
                ccpuStart.iModifiers |= EModifierShift;
                ccpuStart.iModifiers |= EModifierRightShift;
                CCoeEnv::Static()->SimulateKeyEventL(ccpuStart, aEventCode);        
                
                aRetCode = EKeyWasConsumed;
                return ETrue;                                                                                                       
                }  
            else
                {
                aRetCode = EKeyWasNotConsumed;              
                return ETrue;
                }               
            }
        
        TCursorSelection cursorSelection;
        if( iInputCapabilities.FepAwareTextEditor() )
        	{
			iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(cursorSelection); 
			if( (aEventCode ==  EEventKeyUp) && (cursorSelection.Length() == 0) 
				&& ((aKeyEvent.iScanCode ==  EStdKeyBackspace) || (aKeyEvent.iCode ==  EKeyBackspace)
				|| (aKeyEvent.iScanCode ==  EStdKeyDelete) || (aKeyEvent.iCode ==  EKeyDelete))) 
				{
				ClearCcpuFlag(ECcpuStateButton);
				}
			}
        if (aKeyEvent.iScanCode != EStdKeyBackspace && aKeyEvent.iCode != EKeyOK)   
            {                           
            aRetCode = EKeyWasConsumed;
            return ETrue;                                                       
            }
        else
            {
            if (iInputCapabilities.FepAwareTextEditor())            
                {
                TCursorSelection cursorSelection;                               
                iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(cursorSelection); 
                if (cursorSelection.Length() == 0)      
                    {
                    aRetCode = EKeyWasConsumed;
                    return ETrue;                                                                           
                    }
                else if (aKeyEvent.iScanCode == EStdKeyBackspace)
                    {
                    // Selection will be removed, handle cba button state accordingly                    
                    if (iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep() ==
                        cursorSelection.Length())
                        {
                        // Whole text was selected and will be removed. Cancel the ccpu mode.
                        ProcessCommandL(EAknFepSoftkeyCloseWindow);
                        }
                    else    
                        {
                        // Part of the text will be removed. Set Cba-buttons to initial ccpu-mode status.                        
                        __ASSERT_DEBUG(iUiInterface->SoftkeysExist(),
                                    AknFepPanic(EAknFepPanicNoHandleToFepAwareTextEditor));
                 
                        TBool update = UpdateCBALabelL(ELeftSoftkeyIndex, KNullCommandId, R_TEXT_SOFTKEY_EMPTY);
                        update |= UpdateCBALabelL(ERightSoftkeyIndex, EAknFepSoftkeyCloseWindow, R_AKNFEP_SOFTKEY_CCPU_CANCEL);
                        update |= UpdateCBALabelL( EMiddleSoftkeyIndex, KNullCommandId, R_TEXT_SOFTKEY_EMPTY ); 
                        if (update)
                            {
                            iUiInterface->DrawSoftkeysNow();
                            }                                                                     
                        }
                    }                    
                }
            }                                   
        }
        
    return EFalse;  
    }
    
    

void CAknFepManager::AddInputLanguageItemL(CAknFepUiInterfaceMenuPane* aMenuPane, TInt aIndex)
    {
    CAknFepUiInterfaceMenuPane::SItemData mData;
    
    TBuf<KMaxFileName> langStr;
    TInt Inputlangindex;
    if(!aMenuPane->MenuItemExists(EAknCmdInputLanguage, Inputlangindex))
	    {    
    StringLoader::Load(langStr, R_AKNFEP_OPTIONS_INPUT_LANGUAGE);
    
    mData.iCommandId = EAknCmdInputLanguage;
    mData.iCascadeId = 0;  
    mData.iFlags = 0;
    mData.iText.Copy(langStr.Mid(1, langStr.Length() - 1).Left(CAknFepUiInterfaceMenuPane::SItemData::ENominalTextLength));
    
    aMenuPane->InsertMenuItemL(mData, aIndex); 
	    }
    
    // Chinese input mode switching from the option menu
    if (IsChineseInputLanguage() && iSharedDataInterface->QwertyInputMode())
        {
        // Add Chinese language items
        langStr.Zero();
        
        StringLoader::Load(langStr, R_AKNFEP_OPTIONS_INPUT_MODE_CHI);
        
        mData.iCommandId = EAknCmdInputMethod;
        mData.iCascadeId = R_AVKON_INPUT_MODE_SUB_MENU;
        mData.iFlags = 0;
        mData.iText.Copy(langStr.Mid(1, langStr.Length() - 1).Left(CAknFepUiInterfaceMenuPane::SItemData::ENominalTextLength));
        
        aMenuPane->InsertMenuItemL(mData, ++aIndex);
        
        // Check if the current engine has CangJie feature or not, if not
        // then CangJie mode becomes invisible, similar impl in IsInputModeAvailable
        // and DoChineseSubMenu        
        TBool isCangJieSupported = EFalse;
        MPtiCoreInfo* ptiCoreInfo = iPtiEngine->CoreInfo(EPtiEngineNormalCangjieQwerty);
        if ( ptiCoreInfo )
            {
            isCangJieSupported = ptiCoreInfo->CapsBits() & ESupportCangjieInput;
            }
        
        // Add Hong Kong CangJie option, option available only in HK variant and qwerty mode
        if ( iLanguageCapabilities.iInputLanguageCode == 
             ELangHongKongChinese && isCangJieSupported )
            {
            langStr.Zero();
            
            StringLoader::Load(langStr, R_AKNFEP_OPTIONS_CANGJIE_MODE);
            
            mData.iCommandId = EChinFepCmdModeCangJieOption;
            mData.iCascadeId = 0;
            mData.iFlags = 0;
            mData.iText.Copy(langStr.Mid(1, langStr.Length() - 1).Left(CAknFepUiInterfaceMenuPane::SItemData::ENominalTextLength));
            
            aMenuPane->InsertMenuItemL(mData, ++aIndex);          
            }
        }
    }

#ifdef RD_INTELLIGENT_TEXT_INPUT
// Predictive QWERTY (XT9) changes ---->
void CAknFepManager::AddPredictiveModeOptionsL(CAknFepUiInterfaceMenuPane* aMenuPane, 
                                               TInt aIndex ) const
    {
    CAknFepUiInterfaceMenuPane::SItemData mData;
    
    HBufC* inputMenuStr = StringLoader::LoadLC(R_AKNFEP_PRED_OPTIONS_INPUT_OPTIONS);
    
    mData.iCommandId = 0;
    mData.iCascadeId = R_AKNFEP_PRED_INPUT_OPTIONS_SUBMENU;  
    mData.iFlags = 0;
    mData.iText.Copy(inputMenuStr->Left(CAknFepUiInterfaceMenuPane::SItemData::ENominalTextLength));
    
    CleanupStack::PopAndDestroy(inputMenuStr);
    
    aMenuPane->InsertMenuItemL(mData, aIndex);
       
    // Chinese input mode switching from the option menu
    if (IsChineseInputLanguage() && iSharedDataInterface->QwertyInputMode())
        {
        // Add Chinese language items
        TBuf<KMaxFileName> langStr;
        
        langStr.Zero();
        
        StringLoader::Load(langStr, R_AKNFEP_OPTIONS_INPUT_MODE_CHI);
        
        mData.iCommandId = EAknCmdInputMethod;
        mData.iCascadeId = R_AVKON_INPUT_MODE_SUB_MENU;
        mData.iFlags = 0;
        mData.iText.Copy(langStr.Mid(1, langStr.Length() - 1).Left(CAknFepUiInterfaceMenuPane::SItemData::ENominalTextLength));
        
        aMenuPane->InsertMenuItemL(mData, ++aIndex);
        
        // Check if the current engine has CangJie feature or not, if not
        // then CangJie mode becomes invisible, similar impl in IsInputModeAvailable
        // and DoChineseSubMenu        
        TBool isCangJieSupported = EFalse;
        MPtiCoreInfo* ptiCoreInfo = iPtiEngine->CoreInfo(EPtiEngineNormalCangjieQwerty);
        if ( ptiCoreInfo != NULL)
            {
            isCangJieSupported = ptiCoreInfo->CapsBits() & ESupportCangjieInput;
            }
#ifdef __HALF_QWERTY_KEYPAD
        isCangJieSupported = isCangJieSupported && (iKeyboardType != EPtiKeyboardHalfQwerty);
#endif //__HALF_QWERTY_KEYPAD
        
        // Add Hong Kong CangJie option, option available only in HK variant and qwerty mode
        if ( iLanguageCapabilities.iInputLanguageCode == ELangHongKongChinese && isCangJieSupported )
            {
            langStr.Zero();
            
            StringLoader::Load(langStr, R_AKNFEP_OPTIONS_CANGJIE_MODE);
            
            mData.iCommandId = EChinFepCmdModeCangJieOption;
            mData.iCascadeId = 0;
            mData.iFlags = 0;
            mData.iText.Copy(langStr.Mid(1, langStr.Length() - 1).Left(CAknFepUiInterfaceMenuPane::SItemData::ENominalTextLength));
            
            aMenuPane->InsertMenuItemL(mData, ++aIndex);          
            }
        }
    }

void CAknFepManager::AddPredictiveModeEditMenuL(CAknFepUiInterfaceMenuPane* aMenuPane, 
                                                TInt aIndex ) const
	{
	// Remove Predictive Edit menu option if XT9 options are there --->
	aMenuPane->SetItemDimmed(EAknCmdPredictiveTextCascade, ETrue);
    aMenuPane->SetItemDimmed(EAknCmdMultitapPredictiveT9On, ETrue);
	// Remove Predictive Edit menu option if XT9 options are there <---
	//if current writing language supports prediction then display input options settings --> 
    //if current writing language does not supports prediction then display only input language 
    //option to modify the writing language, when user wants to modify when edit query is launched. <--
     TBool isLanguagugeSupportsPrediction = EFalse;
     switch(iKeyboardType)
     {
		 case EPtiKeyboardHalfQwerty:
#ifdef __HALF_QWERTY_KEYPAD		 
		 		isLanguagugeSupportsPrediction = (iQwertyInputMode) & (iLanguageCapabilities.iSupportsWesternHalfQwertyPredictive); 
#endif   //__HALF_QWERTY_KEYPAD
		    break;
     case EPtiKeyboardQwerty4x12:
     case EPtiKeyboardQwerty4x10:  
     case EPtiKeyboardQwerty3x11:
     case EPtiKeyboardCustomQwerty:
		 		isLanguagugeSupportsPrediction = (iQwertyInputMode) & (iLanguageCapabilities.iSupportsWesternQwertyPredictive);
		    break;
     case EPtiKeyboardNone:
     case EPtiKeyboard12Key:
		 		isLanguagugeSupportsPrediction = (!iQwertyInputMode) & (iLanguageCapabilities.iSupportsWesternPredictive);
		    break;
		 default:
		 		isLanguagugeSupportsPrediction = (!iQwertyInputMode) & (iLanguageCapabilities.iSupportsWesternPredictive);
		    break;    
		 }
     if (isLanguagugeSupportsPrediction && !(iAknEditorFlags & EAknEditorFlagNoT9))
         {
         aMenuPane->SetItemDimmed(EAknCmdInputLanguage, ETrue);           
         AddPredictiveModeOptionsL(aMenuPane, aIndex);
         }
	}
// Predictive QWERTY (XT9) changes <----
#endif //RD_INTELLIGENT_TEXT_INPUT
void CAknFepManager::DoChineseSubMenu(CAknFepUiInterfaceMenuPane* aMenuPane)
    {
    // ZhuyinFind & StrokeFind ONLY available for FindEditors
    if (iAknEditorFlags & EAknEditorFlagForceTransparentFepModes)
        {
        if (iMode == EZhuyinFind)
            {
            aMenuPane->SetItemDimmed(EChinFepCmdModeZhuyinFind, ETrue);
            }
        
        if (iMode == EStrokeFind)
            {
            aMenuPane->SetItemDimmed(EChinFepCmdModeStrokeFind, ETrue);
            }        
        }
    else
        {
        aMenuPane->SetItemDimmed(EChinFepCmdModeZhuyinFind, ETrue);
        aMenuPane->SetItemDimmed(EChinFepCmdModeStrokeFind, ETrue);
        }
    
    // Customize menu item for Chinese mainland
    if (iLanguageCapabilities.iInputLanguageCode == ELangPrcChinese)
        {
        aMenuPane->SetItemDimmed(EChinFepCmdModeCangJie, ETrue);
        aMenuPane->SetItemDimmed(EChinFepCmdModeZhuyin, ETrue);
        aMenuPane->SetItemDimmed(EChinFepCmdModeZhuyinFind, ETrue);
        
        if (iMode == EPinyin)
            {
            aMenuPane->SetItemDimmed(EChinFepCmdModePinyin, ETrue);
            }
        else if (iMode == EStroke)
            {
            aMenuPane->SetItemDimmed(EChinFepCmdModeStroke, ETrue);
            }
        else if (iMode == ELatin)
            {
            aMenuPane->SetItemDimmed(EAknCmdEditModeLatinText, ETrue);
            }
        else
            return;
        }
    // Customize menu item for HongKong
    else if (iLanguageCapabilities.iInputLanguageCode == ELangHongKongChinese)
        {
        aMenuPane->SetItemDimmed(EChinFepCmdModePinyin, ETrue);
        aMenuPane->SetItemDimmed(EChinFepCmdModeZhuyin, ETrue);
        aMenuPane->SetItemDimmed(EChinFepCmdModeZhuyinFind, ETrue);     
        
        TBool isCangJieSupported = EFalse;
#ifdef RD_INTELLIGENT_TEXT_INPUT
        if ( EPtiKeyboardQwerty3x11 == KeyboardLayout() ||
             EPtiKeyboardQwerty4x10 == KeyboardLayout())
        	{
        	isCangJieSupported = ETrue;
        	}
#endif //RD_INTELLIGENT_TEXT_INPUT
#ifdef RD_INTELLIGENT_TEXT_INPUT      
#ifdef __HALF_QWERTY_KEYPAD
        isCangJieSupported = isCangJieSupported && (iKeyboardType != EPtiKeyboardHalfQwerty);
#endif //__HALF_QWERTY_KEYPAD
#endif //RD_INTELLIGENT_TEXT_INPUT
        
        // CangJie is only for qwerty mode
        if ( !iSharedDataInterface->QwertyInputMode() || !isCangJieSupported )
            aMenuPane->SetItemDimmed(EChinFepCmdModeCangJie, ETrue);
        
        if (iMode == EStroke)
            {
            aMenuPane->SetItemDimmed(EChinFepCmdModeStroke, ETrue);              
            }
        else if (iMode == ECangJie)
            {
            aMenuPane->SetItemDimmed(EChinFepCmdModeCangJie, ETrue);
            }
        else if (iMode == ELatin)
            {
            aMenuPane->SetItemDimmed(EAknCmdEditModeLatinText, ETrue);
            }
        else
            return;                
        }
    // Customize menu item for TaiWan
    else if (iLanguageCapabilities.iInputLanguageCode == ELangTaiwanChinese)
        {
        aMenuPane->SetItemDimmed(EChinFepCmdModePinyin, ETrue);
        aMenuPane->SetItemDimmed(EChinFepCmdModeCangJie, ETrue);
        
        if (iMode == EStroke)
            {
            aMenuPane->SetItemDimmed(EChinFepCmdModeStroke, ETrue);
            }
        else if (iMode == EZhuyin)
            {
            aMenuPane->SetItemDimmed(EChinFepCmdModeZhuyin, ETrue);
            }
        else if (iMode == ELatin)
            {
            aMenuPane->SetItemDimmed(EAknCmdEditModeLatinText, ETrue);
            }
        else
            return;                 
        }
    else
        return;    
    }
#ifdef RD_SCALABLE_UI_V2
       
void CAknFepManager::DoWesternTouchMenu(CAknFepUiInterfaceMenuPane* aMenuPane)
    {
    const TInt inputLanguage = iLanguageCapabilities.iInputLanguageCode;
    // if mode is already hangul or language is not korean dim korean mode menu item
    TInt index;
        
    if( iMode == EHangul || inputLanguage != ELangKorean || !(IsModePermitted(EHangul)))
        {
        if ( aMenuPane->MenuItemExists( EAknCmdEditModeKorean, index) )
            {
            aMenuPane->SetItemDimmed(EAknCmdEditModeKorean, ETrue);
            }
        }
    else
        {
        if ( aMenuPane->MenuItemExists( EAknCmdEditModeKorean, index ) )
            {
            aMenuPane->SetItemDimmed(EAknCmdEditModeKorean, EFalse);
            }
        }
    // remove Chinese specific items from menu
    aMenuPane->SetItemDimmed(EAknCmdEditModeAlpha, ETrue);    
    
    if (aMenuPane->MenuItemExists(EJapanFepCmdModeUserDic, index))
        {
        aMenuPane->SetItemDimmed(EJapanFepCmdModeUserDic, ETrue);
        }

    // T9 stuff
    if (WesternPredictive())
        {
        aMenuPane->SetItemDimmed(EAknCmdPredictiveTextCascade, EFalse);
        if(iPtiEngine->CurrentLanguage()->HasInputMode(EPtiEngineWordCompletion))
        	{
        	// Disable Autoword Completion feature for Touch Input
        	#ifndef RD_SCALABLE_UI_V2  
        	aMenuPane->SetItemDimmed(EAknCmdPredictiveModeCascade, EFalse);	
        	#endif // RD_SCALABLE_UI_V2
        	}
        	
        if (aMenuPane->MenuItemExists(EAknCmdT9PredictiveT9Off, index)) 
            {
            aMenuPane->SetItemDimmed(EAknCmdT9PredictiveT9Off, EFalse);
            }          	
        }
    else
        {
        if ( (iPermittedInputModes & EAknEditorTextInputMode) &&
            !(iAknEditorFlags & EAknEditorFlagNoT9) &&
            iMode == ELatin &&
            !IsKoreanInputLanguage() &&
            iLanguageCapabilities.iSupportsWesternPredictive)
            {
            aMenuPane->SetItemDimmed(EAknCmdMultitapPredictiveT9On, EFalse);
            }
        }

    if (  ( iMode == ENumber || iMode == ENativeNumber ) &&
        (iPermittedInputModes & (EAknEditorTextInputMode | EAknEditorSecretAlphaInputMode)) )
        {
        aMenuPane->SetItemDimmed(EAknCmdEditModeAlpha, EFalse); 
        aMenuPane->SetItemDimmed(EAknCmdEditModeLatinText, ETrue);
        aMenuPane->SetItemDimmed(EChinFepCmdModeLatinUpper, ETrue);
        aMenuPane->SetItemDimmed(EChinFepCmdModeLatinLower, ETrue);
        }

    if ( IsModePermitted(ENumber) )
        {
        if (( iMode != ENumber ||
            iMode == ENumber && iLanguageCapabilities.iArabicIndicDigitsAllowed &&
            !(iAknEditorFlags & EAknEditorFlagLatinInputModesOnly) &&
            (iLanguageCapabilities.iLocalDigitType == EDigitTypeArabicIndic)) 
            || ( iMode != ENumber ||
            iMode == ENumber && iLanguageCapabilities.iEasternArabicIndicDigitsAllowed &&
            !(iAknEditorFlags & EAknEditorFlagLatinInputModesOnly) &&
            (iLanguageCapabilities.iLocalDigitType == EDigitTypeEasternArabicIndic))
            ||( iMode != ENumber ||
            iMode == ENumber && iLanguageCapabilities.iIndicDigitsAllowed &&
            !(iAknEditorFlags & EAknEditorFlagLatinInputModesOnly) &&
            (iLanguageCapabilities.iLocalDigitType == EDigitTypeDevanagari)) &&
            (!(iPermittedInputModes==EAknEditorNumericInputMode))) // hide the menu if number editor
            
            {
            aMenuPane->SetItemDimmed(EAknCmdEditModeNumber, EFalse);
            }
        }
    
    if ( IsModePermitted( ENativeNumber ) )
        {
       if ( iLanguageCapabilities.iArabicIndicDigitsAllowed &&
            !(iAknEditorFlags & EAknEditorFlagLatinInputModesOnly) &&
            iMode != ENativeNumber )
            {
            aMenuPane->SetItemDimmed(EAknCmdEditModeArabicIndicNumber, EFalse);
            }
        else if ( iLanguageCapabilities.iEasternArabicIndicDigitsAllowed &&
            !(iAknEditorFlags & EAknEditorFlagLatinInputModesOnly) &&
            iMode != ENativeNumber )
            {
            aMenuPane->SetItemDimmed(EAknCmdEditModeEasternArabicIndicNumber, EFalse);
            }   
        else if( iLanguageCapabilities.iIndicDigitsAllowed && 
            !( iAknEditorFlags & EAknEditorFlagLatinInputModesOnly ) &&       

            iMode != ENativeNumber )
            {
            aMenuPane->SetItemDimmed(EAknCmdEditModeIndicNumber, EFalse);   
            }
        }

    if (IsAbleToLaunchSCT())
        {
        if (aMenuPane->MenuItemExists(EAknCmdEditInsertSymbol, index)) 
            {
            aMenuPane->SetItemDimmed(EAknCmdEditInsertSymbol, EFalse);
            }
        }

    if ( iMode != ENumber && iMode != ENativeNumber && 
         iPtiEngine->NumberOfLanguages() > 1 &&
        !(iAknEditorFlags & EAknEditorFlagLatinInputModesOnly))
        {
        aMenuPane->SetItemDimmed(EAknCmdInputLanguage, EFalse);
        }
        
    if (iPermittedInputModes == EAknEditorNumericInputMode)
        {
        aMenuPane->SetItemDimmed(EAknCmdEditModeLatinText, ETrue);
        aMenuPane->SetItemDimmed(EChinFepCmdModeLatinUpper, ETrue);
        aMenuPane->SetItemDimmed(EChinFepCmdModeLatinLower, ETrue);
        }
    else if ( (iMode == ELatin || iMode == ENumber || iMode == ENativeNumber ) &&
        (iPermittedInputModes & EAknEditorSecretAlphaInputMode) &&
        !(iPermittedInputModes & EAknEditorTextInputMode))
        {
        // Text case mode is not available in secret editors.
        aMenuPane->SetItemDimmed(EAknCmdEditModeLatinText, ETrue);
        }
        
    if (iAknEditorFlags & EAknEditorFlagForceTransparentFepModes)
        {
        aMenuPane->SetItemDimmed(EAknCmdEditModeLatinText, ETrue);
        aMenuPane->SetItemDimmed(EChinFepCmdModeLatinUpper, ETrue);
        }   
        
    if (iCaseMan->CurrentCase() == EAknEditorUpperCase)
        {
        aMenuPane->SetItemDimmed(EChinFepCmdModeLatinUpper, ETrue);   
        }
    else if (iCaseMan->CurrentCase() == EAknEditorLowerCase)
        {
        aMenuPane->SetItemDimmed(EChinFepCmdModeLatinLower, ETrue);   
        }
        
    aMenuPane->SetItemDimmed(EAknCmdEditModeLatinText, ETrue);    
    
    if (!(iAknEditorFlags & EAknEditorFlagLatinInputModesOnly) 
    && iFepPluginManager->IsNonLatinLanguage(TLanguage(iSharedDataInterface->InputTextLanguage())))
        {
        aMenuPane->SetItemDimmed(EAknCmdEditModeLatinText, ETrue);
        aMenuPane->SetItemDimmed(EChinFepCmdModeLatinUpper, ETrue);
        aMenuPane->SetItemDimmed(EChinFepCmdModeLatinLower, ETrue);
        }             
    if( iMode == EHangul )
        {
        //aMenuPane->SetItemDimmed(EAknCmdEditModeLatinText, EFalse);
        //aMenuPane->SetItemDimmed(EAknCmdEditModeAlpha, EFalse);
        
        aMenuPane->SetItemDimmed(EAknCmdEditModeEnglish, EFalse);
        aMenuPane->SetItemDimmed(EChinFepCmdModeLatinUpper, ETrue);
        aMenuPane->SetItemDimmed(EChinFepCmdModeLatinLower, ETrue);
        
        }
    else
        {
        aMenuPane->SetItemDimmed(EAknCmdEditModeEnglish, ETrue);
        }
    }    

void CAknFepManager::DoChineseTouchMenu(CAknFepUiInterfaceMenuPane* aMenuPane)
    {
    TInt index;
    if (aMenuPane->MenuItemExists(EAknCmdEditInsertSymbol, index)
        && aMenuPane->MenuItemExists(EJapanFepCmdModeUserDic, index)) 
        {
        aMenuPane->SetItemDimmed(EJapanFepCmdModeUserDic, ETrue);    
        }

    if (WesternPredictive())
        {
        aMenuPane->SetItemDimmed(EAknCmdPredictiveTextCascade, EFalse);
        if(iPtiEngine->CurrentLanguage()->HasInputMode(EPtiEngineWordCompletion))
        	{
        	// Disable Autoword Completion feature for Touch Input
        	#ifndef RD_SCALABLE_UI_V2  
        	aMenuPane->SetItemDimmed(EAknCmdPredictiveModeCascade, EFalse);	
        	#endif // RD_SCALABLE_UI_V2
        	}
        	
        if (aMenuPane->MenuItemExists(EAknCmdT9PredictiveT9Off, index)) 
            {
            aMenuPane->SetItemDimmed(EAknCmdT9PredictiveT9Off, EFalse);
            }             	
        }
    else
        {
        if ( (iPermittedInputModes & EAknEditorTextInputMode) &&
            !(iAknEditorFlags & EAknEditorFlagNoT9) &&
            iMode == ELatin)
            {
            aMenuPane->SetItemDimmed(EAknCmdMultitapPredictiveT9On, EFalse);
            }
        }
    if (IsAbleToLaunchSCT() &&
        aMenuPane->MenuItemExists(EAknCmdEditInsertSymbol, index))
        {
        aMenuPane->SetItemDimmed(EAknCmdEditInsertSymbol, EFalse);
        }
    if( (iFepPluginManager && (iFepPluginManager->PluginInputMode() != EPluginInputModeItut ) ) 
            && aMenuPane->MenuItemExists(EChinFepCmdInstructions, index) )  
        {
        aMenuPane->SetItemDimmed(EChinFepCmdInstructions, ETrue);
        }
    else if ( !(iPermittedInputModes == EAknEditorNumericInputMode ||
        iAknEditorFlags & EAknEditorFlagLatinInputModesOnly) )
        {
        if (aMenuPane->MenuItemExists(EAknCmdEditInsertSymbol, index)) 
            {
            aMenuPane->SetItemDimmed(EChinFepCmdInstructions, EFalse);
            }
        }

    if ( iMode != ENumber && iPtiEngine->NumberOfLanguages() > 1  &&
        !(iAknEditorFlags & EAknEditorFlagLatinInputModesOnly))
        {
        aMenuPane->SetItemDimmed(EAknCmdInputLanguage, EFalse);
        }

    if (iPermittedInputModes == EAknEditorNumericInputMode)
        {
        aMenuPane->SetItemDimmed(EAknCmdEditModeLatinText, ETrue);
        aMenuPane->SetItemDimmed(EChinFepCmdModeLatinUpper, ETrue);
        aMenuPane->SetItemDimmed(EChinFepCmdModeLatinLower, ETrue);
        }
    else if ( (iMode == ELatin || iMode == ENumber) &&
        (iPermittedInputModes & EAknEditorSecretAlphaInputMode) &&
        !(iPermittedInputModes & EAknEditorTextInputMode))
        {
        // Text case mode is not available in secret editors.
        aMenuPane->SetItemDimmed(EAknCmdEditModeLatinText, ETrue);
        }
    if (iAknEditorFlags & EAknEditorFlagForceTransparentFepModes)
        {
        aMenuPane->SetItemDimmed(EAknCmdEditModeLatinText, ETrue);
        aMenuPane->SetItemDimmed(EChinFepCmdModeLatinUpper, ETrue);
        }

    if ( IsModePermitted(ENumber) && iMode != ENumber )
        {
        //aMenuPane->SetItemDimmed(EChinFepCmdModeNumber, EFalse);  // original statement
        aMenuPane->SetItemDimmed(EAknCmdEditModeNumber, EFalse);
        // end modifying
        }
    }
TBool CAknFepManager::IsChineseInputMode( TInt aMode )
    {
    return aMode == EPRCFind || aMode == ECangJie || aMode == EZhuyinFind
           || aMode == EStrokeFind || aMode == EPinyin || aMode == EZhuyin
           || aMode == EStroke;    
    }

#endif //RD_SCALABLE_UI_V2
void CAknFepManager::DoWesternMenu(CAknFepUiInterfaceMenuPane* aMenuPane)
    {
    // remove Chinese specific items from menu
    aMenuPane->SetItemDimmed(EChinFepCmdModeLatinUpper, ETrue);
    aMenuPane->SetItemDimmed(EChinFepCmdModeLatinLower, ETrue);
    aMenuPane->SetItemDimmed(EAknCmdEditModeLatinText, ETrue);
    const TInt inputLanguage = iLanguageCapabilities.iInputLanguageCode;
    
    
    TInt pos=-1;
    
    if( aMenuPane->MenuItemExists( EAknCmdEditModeKorean, pos ) )
        {
        // if mode is already hangul or language is not korean dim korean mode menu item
        if( iMode == EHangul || inputLanguage != ELangKorean || !(IsModePermitted(EHangul)))
            {
            
            aMenuPane->SetItemDimmed(EAknCmdEditModeKorean, ETrue);
            }
        else
            {
            aMenuPane->SetItemDimmed(EAknCmdEditModeKorean, EFalse);
            }
        }
    
    TInt index;
    
    if (aMenuPane->MenuItemExists(EJapanFepCmdModeUserDic, index))
        {
        aMenuPane->SetItemDimmed(EJapanFepCmdModeUserDic, ETrue);
        }

    // T9 stuff
    if (WesternPredictive())
        {
        aMenuPane->SetItemDimmed(EAknCmdPredictiveTextCascade, EFalse);
#ifndef RD_INTELLIGENT_TEXT_INPUT
        if(iPtiEngine->CurrentLanguage()->HasInputMode(EPtiEngineWordCompletion))
        	{
        	// Disable Autoword Completion feature for Touch Input
        	#ifndef RD_SCALABLE_UI_V2  
        	aMenuPane->SetItemDimmed(EAknCmdPredictiveModeCascade, EFalse);	
        	#endif // RD_SCALABLE_UI_V2
        	}
#endif  //RD_INTELLIGENT_TEXT_INPUT 
        }
    else
        {
        if ( (iPermittedInputModes & EAknEditorTextInputMode) &&
            !(iAknEditorFlags & EAknEditorFlagNoT9) &&
            iMode == ELatin &&
			!IsKoreanInputLanguage() &&
#ifdef RD_INTELLIGENT_TEXT_INPUT
            // Predictive QWERTY (XT9) changes ---->
            ((
#endif //RD_INTELLIGENT_TEXT_INPUT
            iLanguageCapabilities.iSupportsWesternPredictive && !IsKoreanInputLanguage()
#ifdef RD_INTELLIGENT_TEXT_INPUT
			&& !iQwertyInputMode) ||
			(iLanguageCapabilities.iSupportsWesternQwertyPredictive && iQwertyInputMode)
			)
            // Predictive QWERTY (XT9) changes <----
#endif //RD_INTELLIGENT_TEXT_INPUT
           )
            {
            aMenuPane->SetItemDimmed(EAknCmdMultitapPredictiveT9On, EFalse);
            }
        }

    if ( ( iMode == ENumber || iMode == ENativeNumber || iMode == EHangul ) && 
        (iPermittedInputModes & (EAknEditorTextInputMode | EAknEditorSecretAlphaInputMode)) )
        {
        aMenuPane->SetItemDimmed(EAknCmdEditModeAlpha, EFalse);
        }

    if ( IsModePermitted(ENumber) )
        {
        if ( iMode != ENumber ) // hide the menu if number editor
            {
            aMenuPane->SetItemDimmed(EAknCmdEditModeNumber, EFalse);
            }
#ifdef RD_INTELLIGENT_TEXT_INPUT
        // Predictive QWERTY (XT9) changes ---->
        // There's no need for numeric mode when full qwerty keyboard is in use (like in MAXI)
        if (iQwertyInputMode)
            {
            aMenuPane->SetItemDimmed(EAknCmdEditModeNumber, ETrue);
            }
        // Predictive QWERTY (XT9) changes <----
#endif //RD_INTELLIGENT_TEXT_INPUT
        }
        
    if ( IsModePermitted(ENativeNumber) && iMode != ENativeNumber)
        {
        if ( iLanguageCapabilities.iArabicIndicDigitsAllowed ) // hide the menu if number editor)
            {
            aMenuPane->SetItemDimmed(EAknCmdEditModeArabicIndicNumber, EFalse);
            }
        else if ( iLanguageCapabilities.iEasternArabicIndicDigitsAllowed ) // hide the menu if number editor)
            {
            aMenuPane->SetItemDimmed(EAknCmdEditModeEasternArabicIndicNumber, EFalse);
            }    
        else if( iLanguageCapabilities.iIndicDigitsAllowed ) // hide the menu if number editor)
            {
            aMenuPane->SetItemDimmed(EAknCmdEditModeIndicNumber, EFalse);   
            }
        }

    if (IsAbleToLaunchSCT())
        {
        if (aMenuPane->MenuItemExists(EAknCmdEditInsertSymbol, index)) 
            {
            aMenuPane->SetItemDimmed(EAknCmdEditInsertSymbol, EFalse);
            }
        }
    TInt writtingLanguage = 0;
    if ( iMode != ENumber && iMode != ENativeNumber && iPtiEngine->NumberOfLanguages() > 1 &&
        !(iAknEditorFlags & EAknEditorFlagLatinInputModesOnly)
        && aMenuPane->MenuItemExists( EAknCmdInputLanguage, writtingLanguage ) )
        {
        aMenuPane->SetItemDimmed(EAknCmdInputLanguage, EFalse);
        }
    }

void CAknFepManager::DoChineseMenu(CAknFepUiInterfaceMenuPane* aMenuPane)
    {
    TInt index;
    if (aMenuPane->MenuItemExists(EAknCmdEditInsertSymbol, index)
       && aMenuPane->MenuItemExists(EJapanFepCmdModeUserDic, index)) 
        {
        aMenuPane->SetItemDimmed(EJapanFepCmdModeUserDic, ETrue);    
        }

    if (WesternPredictive())
        {
        aMenuPane->SetItemDimmed(EAknCmdPredictiveTextCascade, EFalse);
#ifndef RD_INTELLIGENT_TEXT_INPUT
        if(iPtiEngine->CurrentLanguage()->HasInputMode(EPtiEngineWordCompletion))
        	{
        	// Disable Autoword Completion feature for Touch Input
        	#ifndef RD_SCALABLE_UI_V2  
        	aMenuPane->SetItemDimmed(EAknCmdPredictiveModeCascade, EFalse);	
        	#endif // RD_SCALABLE_UI_V2
        	}
#endif //RD_INTELLIGENT_TEXT_INPUT
        }
    else
        {
        if ( (iPermittedInputModes & EAknEditorTextInputMode) &&
            !(iAknEditorFlags & EAknEditorFlagNoT9) &&
            iMode == ELatin)
            {
            aMenuPane->SetItemDimmed(EAknCmdMultitapPredictiveT9On, EFalse);
            }
        }
    if (IsAbleToLaunchSCT() &&
        aMenuPane->MenuItemExists(EAknCmdEditInsertSymbol, index))
        {
        aMenuPane->SetItemDimmed(EAknCmdEditInsertSymbol, EFalse);
        }
    if( iQwertyInputMode && aMenuPane->MenuItemExists(EChinFepCmdInstructions, index) )
        {
        aMenuPane->SetItemDimmed(EChinFepCmdInstructions, ETrue);
        }
    else if ( !(iPermittedInputModes == EAknEditorNumericInputMode ||
        iAknEditorFlags & EAknEditorFlagLatinInputModesOnly) )
        {
        if (aMenuPane->MenuItemExists(EAknCmdEditInsertSymbol, index)
            && aMenuPane->MenuItemExists(EChinFepCmdInstructions, index)) 
            {
            aMenuPane->SetItemDimmed(EChinFepCmdInstructions, EFalse);
            }
        }

    if ( (!IsOnlyNumericPermitted()) && iPtiEngine->NumberOfLanguages() > 1  &&
        !(iAknEditorFlags & EAknEditorFlagLatinInputModesOnly))
        {
        aMenuPane->SetItemDimmed(EAknCmdInputLanguage, EFalse);
        // the follow added for phrase creation
        TInt iptlanguage;
        if ( !iIsUserdbdlgActive 
#ifdef RD_INTELLIGENT_TEXT_INPUT 
        && EPtiKeyboardHalfQwerty != KeyboardLayout() 
#endif
        && IsChineseInputLanguage() )
            {
            aMenuPane->MenuItemExists( EAknCmdInputLanguage, iptlanguage );
            MPtiLanguage* ptilang = iPtiEngine->CurrentLanguage();
            TInt inputLanguage = (ptilang)? ptilang->LanguageCode() : 0;          
            if (iQwertyInputMode && (inputLanguage == ELangPrcChinese ||
            	inputLanguage == ELangHongKongChinese ||
            	inputLanguage == ELangTaiwanChinese))
            	{
            	TRAP_IGNORE(AddUserDBDlgItemL( aMenuPane, iptlanguage ));
            	}
            }
        }

    if (iPermittedInputModes == EAknEditorNumericInputMode)
        {
        aMenuPane->SetItemDimmed(EAknCmdEditModeLatinText, ETrue);
        aMenuPane->SetItemDimmed(EChinFepCmdModeLatinUpper, ETrue);
        aMenuPane->SetItemDimmed(EChinFepCmdModeLatinLower, ETrue);
        }
    else if ( (iMode == ELatin || iMode == ENumber) &&
        (iPermittedInputModes & EAknEditorSecretAlphaInputMode) &&
        !(iPermittedInputModes & EAknEditorTextInputMode))
        {
        // Text case mode is not available in secret editors.
        aMenuPane->SetItemDimmed(EAknCmdEditModeLatinText, ETrue);
        }
    if (iAknEditorFlags & EAknEditorFlagForceTransparentFepModes)
        {
        aMenuPane->SetItemDimmed(EAknCmdEditModeLatinText, ETrue);
        aMenuPane->SetItemDimmed(EChinFepCmdModeLatinUpper, ETrue);
        }

    if ( IsModePermitted(ENumber) && iMode != ENumber )
        {
        //aMenuPane->SetItemDimmed(EChinFepCmdModeNumber, EFalse);  // original statement
        aMenuPane->SetItemDimmed(EAknCmdEditModeNumber, EFalse);
        // end modifying
        }
    }

void CAknFepManager::DoJapaneseMenu(CAknFepUiInterfaceMenuPane* aMenuPane)
    {
    // remove Chinese specific items from menu
    TInt index;
    if (aMenuPane->MenuItemExists(EAknCmdEditInsertSymbol, index))
        {
        aMenuPane->SetItemDimmed(EAknCmdEditInsertSymbol, ETrue);
        }
        
    aMenuPane->SetItemDimmed(EChinFepCmdModeLatinUpper, ETrue);
    aMenuPane->SetItemDimmed(EChinFepCmdModeLatinLower, ETrue);
    aMenuPane->SetItemDimmed(EAknCmdEditModeLatinText, ETrue);

    if (IsAbleToLaunchSCT()
     && !iSharedDataInterface->QwertyInputMode())
        {
        aMenuPane->SetItemDimmed(EAknCmdEditInsertSymbolJp, EFalse);
        }

    if (IsAbleToLaunchPCT()
     && !iSharedDataInterface->QwertyInputMode())
        {
        aMenuPane->SetItemDimmed(EAknCmdEditInsertPictograph, EFalse);
        }

    if (!(iPermittedInputModes & EAknEditorHiraganaKanjiInputMode) &&
        aMenuPane->MenuItemExists(EJapanFepCmdModeUserDic, index)
        )
        {
        aMenuPane->SetItemDimmed(EJapanFepCmdModeUserDic, ETrue);
        }

    if (iPermittedInputModes == (EAknEditorSecretAlphaInputMode | EAknEditorNumericInputMode))
        {
        if (iMode == ELatin)
            {
            aMenuPane->SetItemDimmed(EJapanFepCmdEditModeNumber, EFalse);
            }
        else if (iMode == ENumber)
            {
            aMenuPane->SetItemDimmed(EJapanFepCmdEditModeAlpha, EFalse);
            }
        }
    else
        {
        // If the fep is running on the Janapnse User Dictionary Application,
        // "User Dictionary" in editting options menu is hidden.
        if (CEikonEnv::Static()->EikAppUi()->Application())
            {
            if (CEikonEnv::Static()->EikAppUi()->Application()->AppDllUid() == KUidUserDictApp)
                {
                aMenuPane->SetItemDimmed(EJapanFepCmdModeUserDic, ETrue);
                }
            }

        if (iPermittedInputModes & EAknEditorHiraganaKanjiInputMode)
            {
            aMenuPane->SetItemDimmed(EJapanFepCmdModeKutenCodeInput, EFalse);
            }

        TInt value = iSharedDataInterface->ClearDirection();
        if (value == EClearDirectionRight)
            {
            aMenuPane->SetItemDimmed(EAknCmdEditClearDirectionLeft, EFalse);
            }
        else
            {
            aMenuPane->SetItemDimmed(EAknCmdEditClearDirectionRight, EFalse);
            }

        if (!iSharedDataInterface->QwertyInputMode())
            {
            if (IsPredictive(ELatin))
                {
                aMenuPane->SetItemDimmed(EJapanFepCmdPredictiveTextCascade, EFalse);
                }
            else if ((iPermittedInputModes & EAknEditorHalfWidthTextInputMode)
                  && !(iAknEditorFlags & EAknEditorFlagNoT9))
                {
                aMenuPane->SetItemDimmed(EJapanFepCmdMultitapPredictiveT9On, EFalse);
                }
            }

        if (IsPredictive(EHiraganaKanji))
            {
            aMenuPane->SetItemDimmed(EJapanFepCmdModePredictiveOff, EFalse);
            }
        else if((iPermittedInputModes & EAknEditorHiraganaKanjiInputMode)
             && !(iAknEditorFlags & EAknEditorFlagNoT9))
            {
            aMenuPane->SetItemDimmed(EJapanFepCmdModePredictiveOn, EFalse);
            }

        if (iMode != EHiraganaKanji && iPermittedInputModes & EAknEditorHiraganaKanjiInputMode)
            {
            aMenuPane->SetItemDimmed(EJapanFepCmdModeHiragana, EFalse);
            }

        if (iCharWidth == EHalfWidthChar)
            {
            if (iPermittedInputModes & EAknEditorKatakanaInputMode && iMode != EKatakana)
                {
                aMenuPane->SetItemDimmed(EJapanFepCmdModeKatakana, EFalse);
                }

            if (iMode != ELatin && (iPermittedInputModes & EAknEditorHalfWidthTextInputMode
                || iPermittedInputModes & EAknEditorSecretAlphaInputMode))
                {
                aMenuPane->SetItemDimmed(EJapanFepCmdEditModeAlpha, EFalse);
                }

            if (iMode != ENumber
             && iPermittedInputModes & EAknEditorNumericInputMode
             && !iSharedDataInterface->QwertyInputMode())
                {
                aMenuPane->SetItemDimmed(EJapanFepCmdEditModeNumber, EFalse);
                }

            if (iMode == EKatakana && iPermittedInputModes & EAknEditorFullWidthKatakanaInputMode)
                {
                aMenuPane->SetItemDimmed(EJapanFepCmdModeFullWidth, EFalse);
                }

            if (iMode == ELatin && !iWesternPredictive 
             && iPermittedInputModes & EAknEditorFullWidthTextInputMode)
                {
                aMenuPane->SetItemDimmed(EJapanFepCmdModeFullWidth, EFalse);
                }

            if (iMode == ENumber
             && iPermittedInputModes & EAknEditorFullWidthNumericInputMode)
                {
                aMenuPane->SetItemDimmed(EJapanFepCmdModeFullWidth, EFalse);
                }
            }
        else // Full width Character
            {
            if (iMode != EKatakana && iPermittedInputModes & EAknEditorFullWidthKatakanaInputMode)
                {
                aMenuPane->SetItemDimmed(EJapanFepCmdModeKatakana, EFalse);
                }

            if (iMode != ELatin && iPermittedInputModes & EAknEditorFullWidthTextInputMode)
                {
                aMenuPane->SetItemDimmed(EJapanFepCmdEditModeAlpha, EFalse);
                }

            if (iMode != ENumber
             && iPermittedInputModes & EAknEditorFullWidthNumericInputMode
             && !iSharedDataInterface->QwertyInputMode())
                {
                aMenuPane->SetItemDimmed(EJapanFepCmdEditModeNumber, EFalse);
                }

            if (iMode == EKatakana && iPermittedInputModes & EAknEditorKatakanaInputMode)
                {
                aMenuPane->SetItemDimmed(EJapanFepCmdModeHalfWidth, EFalse);
                }

            if (iMode == ELatin && iPermittedInputModes & EAknEditorFullWidthTextInputMode)
                {
                aMenuPane->SetItemDimmed(EJapanFepCmdModeHalfWidth, EFalse);
                }

            if (iMode == ENumber
             && iPermittedInputModes & EAknEditorFullWidthNumericInputMode)
                {
                aMenuPane->SetItemDimmed(EJapanFepCmdModeHalfWidth, EFalse);
                }
            }
        }
    }

void CAknFepManager::HandlePointerEventInInlineTextL(TPointerEvent::TType /*aType*/, TUint /*aModifiers*/, TInt /*aPositionInInlineText*/)
    {
    }

void CAknFepManager::GetFormatOfFepInlineText(TCharFormat& aFormat, 
                                              TInt& aNumberOfCharactersWithSameFormat, 
                                              TInt aPositionOfCharacter) const
    {
    if ( FepUI() )
        {
        FepUI()->GetFormatOfFepInlineText(aFormat, aNumberOfCharactersWithSameFormat, 
                                          aPositionOfCharacter);
        }
    }

void CAknFepManager::NumberModeChangeGSNotification()
	{
	iLanguageCapabilities.iNumericEditorDigitType = EDigitTypeUnknown;
	UpdateNumericEditorDigitType();	
	UpdateLocalDigitMode();
	}

void CAknFepManager::HandleChangeInFocusL()
    {
    // Closing SCT has done focus change, so here just reset the flag.
    // dim state changed, need fetch it again.
    SendEventsToPluginManL(EPluginEnableFetchDimState);

    //The CCPU flag ECcpuStateShiftkeyWasPressedBeforeLosingFocus suggests that the candidate list was closed
    //with a long key press of shift key. If so then, FEP needs to set the EFlagShiftKeyDepressed for the CCPU mode to work 
    //correctly. The state of the shift state is copied to local variable isToSetShiftKeyStateDown because as a function of change
    //of focus the FEP clears all the CCPU flags which would reset ECcpuStateShiftkeyWasPressedBeforeLosingFocus also.
    TBool isToSetShiftKeyStateDown = IsCcpuFlagSet(ECcpuStateShiftkeyWasPressedBeforeLosingFocus);
    if (!Japanese() || iFepManState != EAknFepStateUIActive)
        {
        if (IsCcpuFlagSet(ECcpuStateIgnoreNextFocusChange))
            {
            // The focus event was caused by menu selection, don't clear all ccpu flags.
            ClearCcpuFlag(ECcpuStateIgnoreNextFocusChange);
            }
        else
            {
            if((HashKeySelectionInUse() && IsCcpuFlagSet(ECcpuStateHashDown) 
             && IsCcpuFlagSet(ECcpuStateEdwinInSelectionMode)))
                {
                ClearCcpuFlag(ECcpuStateHashDown);              
                SetCcpuFlag(ECcpuStateSelectionEventPosted);
                ClearCcpuFlag(ECcpuStateEdwinInSelectionMode);
                SetFlag(EFlagLongShiftKeyPress);                
                TKeyEvent ccpuStart = {0, EStdKeyLeftShift, 0, 0};
                CCoeEnv::Static()->SimulateKeyEventL(ccpuStart, EEventKeyUp);                                       
                }   
            else
                {
                TBool isToSupressCursorMove = IsCcpuFlagSet(ECcpuStateSupressCursorMoveToEnd);
                TBool isCommitPredictiveWord = IsCcpuFlagSet(ECcpuStataCommitPredictiveWord);
                CancelCcpuMode();               
                if(isToSupressCursorMove)
                    SetCcpuFlag(ECcpuStateSupressCursorMoveToEnd);
                if(isCommitPredictiveWord)
                    SetCcpuFlag(ECcpuStataCommitPredictiveWord);                
                }    
            
            }
        SetCcpuFlag(ECcpuSupressEditMenuFromShiftUp);        
        iPreviousEditingState = EStateNone;
        iEditIndicatorAtLastUpdate = NULL;
        ClearFlag( EFlagBidiCursorIsInitialized );
#ifndef RD_INTELLIGENT_TEXT_INPUT
// removing the no match indicator is not necessary for ITI 
        if (IsFlagSet(EFlagInsideInlineEditingTransaction))
            {
            TryRemoveNoMatchesIndicatorL();
            }
#endif             
        UpdateCbaL(NULL);
                    
        if (iFepFullyConstructed)
            {
            SetCcpuFlag(ECcpuStateLosingFocus); // CommitInlineEditL needs to now we're losing focus (for cursor handling):           
            CancelAllTimerActivity();
            FepUI()->ExpireMultitapTimer();
            ClearCcpuFlag(ECcpuStateLosingFocus); // Cleared right away to make sure nothing else is affected.              
            }
        // Forget shift key and chr key press if application or UI component is
        // changed during shiftkey press.
        ClearFlag(EFlagQwertyChrKeyDepressed);
        //If candidate list was closed by long keypress of shift key, set the flag EFlagShiftKeyDepressed for CCPU mode
        //to work correctly once it re-focuses on the editor.
        if(isToSetShiftKeyStateDown)
            {
            SetFlag(EFlagShiftKeyDepressed);
            ClearCcpuFlag(ECcpuStateShiftkeyWasPressedBeforeLosingFocus);
            }
        else
            {
            ClearFlag(EFlagShiftKeyDepressed);
            }
        if ( iFepFullyConstructed && IsFepAwareTextEditor() )
            {
            //save state of old item
            TransferFepStateToEditorL();

            if ( WesternPredictive() )
                {                                            
#ifndef RD_INTELLIGENT_TEXT_INPUT
                // To rip off suggested word completion when there is a change in focus
                if(IsAutoCompleteOn())
                	{
                	RemoveSuggestedCompletionL();

                	}
#endif //RD_INTELLIGENT_TEXT_INPUT
                SetCcpuFlag(ECcpuStateLosingFocus);    // CommitInlineEditL needs to now we're losing focus (for cursor handling):                
                CommitInlineEditL(); 
                ClearCcpuFlag(ECcpuStateLosingFocus);  // Cleared right away to make sure nothing else is affected.                              
                }
            else
                {
                TryCloseUiL();
                }
                                 
            iRememberLength = iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep();
            if (iLastFocusedEditor && !IsExtendedFlagSet(EExtendedFlagFepAwareDialogLaunched))
                {
                iLastFocusedEditor->SetObserver(NULL);
                iLastFocusedEditor = NULL;
                }            
			// When editor launches a non-fep aware dialog, then we
			// should not set the observer to Null. Because if the dialog is 
			// destroyed we will not know and the dialog will remain orphaned.                                     
			// Also we need to know the fep editor pointer.
            iLastFocusedEditor = EditorState();        
            iEditorCcpuStatus = EditorCcpuStatus(iLastFocusedEditor); 
            //iLastFocusedEditor->SetObserver(NULL);    
#ifdef RD_SCALABLE_UI_V2            
            UnregisterObserver();
#endif // RD_SCALABLE_UI_V2 
            }

        if (iPermittedInputModes & EAknEditorSecretAlphaInputMode &&
            !(iPermittedInputModes & EAknEditorTextInputMode))
            {
            // For password editors.                
            ClearFlag(EFlagSupressAutoUpdate);
            } 

        // Losing focus is over, from this point on we are gaining focus.
        ClearCcpuFlag(ECcpuStateLosingFocus);   

#ifdef RD_INTELLIGENT_TEXT_INPUT
        if(iFnKeyManager)
                iFnKeyManager->ClearFnKeyState();
                
        if(iResourceString)
          {
          delete iResourceString;
          iResourceString = NULL;
          }
#endif // RD_INTELLIGENT_TEXT_INPUT
        
        // get input capabilities from newly focused item
        CCoeEnv* coeEnv = CCoeEnv::Static();
        iInputCapabilities = static_cast<const CCoeAppUi*>(coeEnv->AppUi())->InputCapabilities();
        SetQwertyModeToInputcapbility();
        if ( IsFepAwareTextEditor() )
            {
            // Now at the launch of non-fep aware dialogs we do not set the editor observer to NULL,
            // and retain the editor pointer in iLastFocusedEditor. 
            // So in case the editor is back in focus, we set iLastFocusedEditor to NULL
            // and set the editor observer again.
            
			// At launch of fep aware dialog on a fep aware editor say spell query launched on messaging editor,
            // we have the flag EExtendedFlagFepAwareDialogLaunched set. 
			// So in this scenario we retain the observer for both the editor as we need to close the spell query
            // on close of messaging editor. 
            // Specific scenario is explained, the same is applicable for all general scenarios, for any
            // fep aware dialog launched on any other fep aware editor.
            if (iLastFocusedEditor && !IsExtendedFlagSet(EExtendedFlagFepAwareDialogLaunched))
                {
                iLastFocusedEditor->SetObserver(NULL);
                iLastFocusedEditor = NULL;
                }
            if( EditorState() )
                {
            	EditorState()->SetObserver(this);
                }
            ConfigureFEPFromEditorStateL();
#ifdef RD_INTELLIGENT_TEXT_INPUT	
			if( PtiEngine() )
				{
	            if(!IsAutoCompleteOn())
	                {
					TInt tailLength = 0;
		            TRAP_IGNORE( PtiEngine()->HandleCommandL( 
									EPtiCommandGetAutoCompletionTailLength,
									&tailLength ));	
					if(tailLength)
						{					
						RemoveSuggestedAdvanceCompletionL();
						}
	                }	
				}
#endif
            if (!iWesternPredictive && iMode == ELatin)
                {
                // It is possible to cause situation (by ill-acting client
                // app) where focus handling goes out of sync and there actually
                // is uncomitted inline edit operation going on in to-be-focused
                // editor. Multitapping mode doesn't like that, so we make sure here
                // that it won't happen.
                CancelInlineEdit();                         
                }             
            AdjustCursorTypeForCurrentPosition();
            
            if (IsCcpuFlagSet(ECcpuStateUncommitWhenFocused))
                {
                // One of the ccpu editing options mode was started from
                // options menu. If there is active predictive word it
                // needs to be deactivated.
                ClearCcpuFlag(ECcpuStateUncommitWhenFocused);               
                DeactivatePredicitveWordAndMoveCursorL();                        
                }
#ifdef RD_SCALABLE_UI_V2                   
            RegisterObserver();
#endif // RD_SCALABLE_UI_V2            		
            }
        else // no FepAwareTextEditor
            {
#ifdef RD_SCALABLE_UI_V2
            // register to receive TInputCapabilitiesEvent events
            if ( SemiFepAwareTextEditor() )
                {
                RegisterObserver();
                TLanguage localLanguage = ELangTest;
                if (GetLocalLanguage( localLanguage ) )
                    {
                    iLanguageCapabilities.iLocalInputLanguageInUse = ETrue;
                    if ( localLanguage != iLanguageCapabilities.iInputLanguageCode )
                        {
                        ChangeInputLanguageL(localLanguage);
                        }
                    }
                else if (iLanguageCapabilities.iLocalInputLanguageInUse)
                    {
                    iLanguageCapabilities.iLocalInputLanguageInUse = EFalse;
                    ChangeInputLanguageL(iSharedDataInterface->InputTextLanguage());
                    }

                if (IsFlagSet(EFlagNewSharedDataInputLanguage) )
                    {
                    //Global mode or input language has been changed in general settings
                    if ( !iLanguageCapabilities.iLocalInputLanguageInUse)
                        {
                        ChangeInputLanguageL(iSharedDataInterface->InputTextLanguage());
                        }
                    ClearFlag(EFlagNewSharedDataInputLanguage);
                    }
                }
#endif // RD_SCALABLE_UI_V2
            // Check for non Edwin derived editors (eg mfne, tel no editor etc)
            if (iInputCapabilities.SupportsWesternNumericIntegerPositive() || 
                iInputCapabilities.SupportsWesternNumericIntegerNegative() || 
                iInputCapabilities.SupportsWesternNumericReal() )
                {
                SyncStates(EAknFepStateInitial);
                iPermittedInputModes = EAknEditorNumericInputMode;
                iAknEditorNumericKeymap = EAknEditorNativeKeysNumberModeKeymap;
                iAknEditorFlags = 0;
                iCharWidth = EHalfWidthChar;
                UpdateNumericEditorDigitType();
                
                if ( IsMfneEditor() ) 
                    {
                    UpdateLocalDigitMode();
                    }
                
                if( IsInputModeAvailable(ENativeNumber) &&
                    ( iLanguageCapabilities.iLocalDigitType == EDigitTypeArabicIndic ||
                      iLanguageCapabilities.iLocalDigitType == EDigitTypeEasternArabicIndic ||
                      iLanguageCapabilities.iLocalDigitType == EDigitTypeDevanagari ) )
                    {
                    TryChangeModeL( ENativeNumber );
                    }
                else
                    {
                    TryChangeModeL( ENumber );
                    }
                }
            else //we don't have a valid editor
                {
                SyncStates(EAknFepStateNull);
                
                // Reset the qwerty shift case mode when we do not have a valis editor.
                ClearFlag(IsFlagSet( EFlagQwertyShiftMode ));
                if (!IsFlagSet(EFlagForegroundUIComponentVisible | EFlagMenuPaneVisible) &&
                    iIndicator)
                    {
                    iIndicator->SetState(EStateNone);
                    iHybridAplphaChangedToAlphanumeric = EFalse; 
                    }
                ClearFlag(EFlagForegroundUIComponentVisible);
                iPermittedInputModes = EAknEditorNullInputMode;
                
#ifdef RD_SCALABLE_UI_V2                
                RegisterObserver();               
#endif // RD_SCALABLE_UI_V2                      
                }            
            }
        }
    else if (Japanese() && iFepManState == EAknFepStateUIActive)
        {
        if (iInputCapabilities.FepAwareTextEditor())
            {
            TryCloseUiL();
            }
        }
    
#ifdef RD_SCALABLE_UI_V2
        {
        /*
        if( iGainForeground )
            {
            iLoseForeAndGainFocus = EFalse;
            }
        else
            {
            iLoseForeAndGainFocus = ETrue;
            }
        SendEventsToPluginManL( EPluginFocusChanged, iGainForeground );
        */
        CCoeControl* focusCtrl = static_cast<const CCoeAppUi*>(CCoeEnv::Static()->AppUi())->TopFocusedControl();         
       CAknAppUi* aui = static_cast<CAknAppUi*>(CEikonEnv::Static()->AppUi());
       TBool bForeGround = aui->IsForeground();
       if(iGainForeground && ! bForeGround)
        {
        iGainForeground = EFalse;
        }
        
        if(iGainForeground && iNotifyPlugin)
            {            
            SendEventsToPluginManL( EPluginFocusChanged, 
                                    iGainForeground && focusCtrl!=0 && focusCtrl->IsFocused());        
            }
        iNotifyPlugin = ETrue;
        }
#endif // RD_SCALABLE_UI_V2
        if( PtiEngine() && FepAwareTextEditor() )
            {
            if( FepAwareTextEditor()->DocumentLengthForFep() == 0 &&
                IsFlagSet(EFlagInsideInlineEditingTransaction)&&
                iFepManState == EAknFepStateUIActive )
              {
              PtiEngine()->ClearCurrentWord();                
              TryCloseUiL();                
              }
            }
#ifdef RD_INTELLIGENT_TEXT_INPUT
    // Predictive QWERTY (XT9) changes ---->
    ShowExactWordPopupIfNecessaryL();
    // Predictive QWERTY (XT9) changes <----
#endif //RD_INTELLIGENT_TEXT_INPUT
    }

void CAknFepManager::CleanUpFep()
    {
    if (iFepFullyConstructed)
        {
        CancelAllTimerActivity();
        iInputCapabilities.FepAwareTextEditor()->CancelFepInlineEdit();
        }
    else
        {
        CommonDestroyFep();
        }
    }

#ifdef RD_SCALABLE_UI_V2
void CAknFepManager::UnregisterObserver()
    {

		MObjectProvider* mop = iInputCapabilities.ObjectProvider();
    
    	if ( mop )
    		{
    		CAknExtendedInputCapabilities* extendedInputCapabilities = 
    		mop->MopGetObject( extendedInputCapabilities );

    		if ( extendedInputCapabilities ) 
    			{
    			extendedInputCapabilities->UnregisterObserver( this );
    			}
    		}
    }

void CAknFepManager::RegisterObserver()
    {

	MObjectProvider* mop = iInputCapabilities.ObjectProvider();

	if ( mop )
		{
		CAknExtendedInputCapabilities* extendedInputCapabilities = 
		mop->MopGetObject( extendedInputCapabilities );

		if ( extendedInputCapabilities ) 
			{
			extendedInputCapabilities->RegisterObserver( this );
			}
		}
    }    
 
#endif // RD_SCALABLE_UI_V2    
    
void CAknFepManager::SimulateKeyEventL(TUint aKeyCode, TBool aActiveObj)
    {
    if (aActiveObj)
        {
        iSimulateKey = aKeyCode;
        iAsyncOwnSimulateKey.CallBack();
        }
    else
        {
        CArrayFix<TUint>* simCharArray = new(ELeave) CArrayFixFlat<TUint>(1);
        CleanupStack::PushL(simCharArray);
        simCharArray->AppendL(aKeyCode);
        SimulateKeyEventsL(simCharArray->Array());
        CleanupStack::PopAndDestroy(); // simCharArray
        }
    }

TInt CAknFepManager::ResetShiftKeyMonitorCallback(TAny* aObj)
    {
    TRAPD(err, static_cast<CAknFepManager*>(aObj)->ResetShiftKeyMonitorL());
    if (err)
        {
        static_cast<CAknFepManager*>(aObj)->CleanUpFep();
        return KErrDied;
        }
    return KErrNone;
    }

void CAknFepManager::ResetShiftKeyMonitorL()
    {
    TBool phoneIdle = (EditorType() == CAknExtendedInputCapabilities::EPhoneNumberEditor); 
    if(phoneIdle)
    	{
    	iShiftKeypressMonitor->Cancel();
    	return;
    	}
    
    SetFlag(EFlagLongShiftKeyPress);
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__    
#ifdef RD_INTELLIGENT_TEXT_INPUT
    // Copy-paste with shift+SKs is always available on ITU-T. On QWERTY,
    // it's available if activated in the Central Repository.
    if ( (!iQwertyInputMode || iSharedDataInterface->ShiftCopyPastingOnQwerty()) && 
         !(iAknEditorFlags & EAknEditorFlagFindPane) )
#endif // RD_INTELLIGENT_TEXT_INPUT        
#else    
    if (!iQwertyInputMode && !(iAknEditorFlags & EAknEditorFlagFindPane))
#endif // __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
        {
        if (HashKeySelectionInUse() && IsCcpuFlagSet(ECcpuStateHashDown)) 
            {
            SetCcpuFlag(ECcpuStateEdwinInSelectionMode);
            iHashKeyMan->SetPreviousSelectionStyleModeL();
            }
    	if(iCandidatePopup)
    		{	// if candidate list is launched and then a long key press of shift key is done, destroy the candidate list first
				iCandidatePopup->AttemptExitL(EFalse);
				HandleChangeInFocusL();
    		}
        if(!IsCcpuFlagSet(ECcpuStateCbaShown))
			{       
	        TKeyEvent ccpuStart = {EKeyF21, EStdKeyF21, 0, 0};
    	    // to enable copy/paste support on cba. We simulate via CCoeEnv
        	// to avoid fep SimulateKeyEventL adding shift modifiers
	        CCoeEnv::Static()->SimulateKeyEventL(ccpuStart, EEventKey);
			SetCcpuFlag(ECcpuStateCbaShown);
			}
        }
#ifdef RD_INTELLIGENT_TEXT_INPUT 
#ifdef __HALF_QWERTY_KEYPAD   
	// we should not enter the copy-paste mode in find pane editors
    if( EPtiKeyboardHalfQwerty == KeyboardLayout() && !(iAknEditorFlags & EAknEditorFlagFindPane))
        {
        CAknFepFnKeyManager::TFnKeyState fnState = FnKeyState();
        
        if( CAknFepFnKeyManager::EFnKeyNone != fnState 
        		|| iFnCharInsertedForShift )
            {
            TText prevCh = PreviousChar();
            if (prevCh == '#' || prevCh == '/')
                {
                RemovePreviousCharacterL();             
                }
            // reset the fn state if it is FnNext
            if( fnState == CAknFepFnKeyManager::EFnKeyNext )
                {
                SetFnKeyState( CAknFepFnKeyManager::EFnKeyNone );
                }
            }
        SetCcpuFlag( CAknFepManager::ECcpuStateEdwinInSelectionMode );
        if(!IsCcpuFlagSet(ECcpuStateCbaShown))
            {		
	        TKeyEvent ccpuStart = {EKeyF21, EStdKeyF21, 0, 0};
         
    	    // to enable copy/paste support on cba. We simulate via CCoeEnv
        	// to avoid fep SimulateKeyEventL adding shift modifiers
        	CCoeEnv::Static()->SimulateKeyEventL(ccpuStart, EEventKey);    
            SetCcpuFlag(ECcpuStateCbaShown); 			                                      
			}
        }  
#endif //__HALF_QWERTY_KEYPAD
		          
#endif//RD_INTELLIGENT_TEXT_INPUT        
    iShiftKeypressMonitor->Cancel();
    }

void CAknFepManager::CancelAllTimerActivity()
    {
    if (iShiftKeypressMonitor->IsActive())
        {
        iShiftKeypressMonitor->Cancel();
        }
    if (iConcatenationTimer->IsActive())
        {
        iConcatenationTimer->Cancel();
        }
    if (iChrKeypressMonitor->IsActive())
        {
        iChrKeypressMonitor->Cancel();
        }
    if (iPostEventCheck->IsActive())
        {
        iPostEventCheck->Cancel();
        }
    if ( iHashKeyMan )
        {
        iHashKeyMan->CancelHashKeyTimer();
        }
#ifdef RD_INTELLIGENT_TEXT_INPUT        
    if( iNumericResourceTimer )
        {
        TRAP_IGNORE( NumericResourceMultiTapTimerTimeoutL());
        }
#endif
    DeactivateFnkeyPressMonitor();
    }

TBool CAknFepManager::IsModePermitted(TInt aMode, TWidthChar aWidth) const
    {
    if (aWidth == ENoneWidthChar)
        {
        aWidth = iCharWidth;
        }

    TBool isModePermitted = ETrue;
    const TInt inputLanguage = iLanguageCapabilities.iInputLanguageCode;    
    
    if ( aMode == ENativeNumber && !IsInputModeAvailable( aMode ) )
        {
        if ( inputLanguage == ELangThai && iFepPluginManager && 
               iFepPluginManager->PluginInputMode() != EPluginInputModeItut &&
               !iQwertyInputMode)
               {
               return ETrue;
               }        
        return EFalse;
        }
    
    TUint editorMode = EditorMode(aMode, aWidth);

    //Support Boxcn default Chinese inputmode when defaul inputmode is Cangjie Hongkong
    //only if Chinese input is permitted in the editor
    if (!( (iVariantPermittedModes | ELatinText | ELatinUpper | ELatinLower | EHiragana) & aMode) )
        {
        //the mode is never valid in this variant
        isModePermitted = EFalse;
        }
    else if( aMode == ECangJie && !(iAknEditorFlags & EAknEditorFlagLatinInputModesOnly))
		{
		if (iQwertyInputMode || iFepPluginManager->PluginInputMode() == EPluginInputModeVkb
				|| iFepPluginManager->PluginInputMode() == EPluginInputModeFSQ)
			{
			isModePermitted = ETrue;
			}
		else
			{
			isModePermitted = EFalse;
			}
		}
    else if (!(iPermittedInputModes & editorMode))
        {
        isModePermitted = EFalse;
        }
    else if (((aMode == EPinyin) || (aMode == EZhuyin) || (aMode == EStroke) || (aMode == ECangJie)
        ||   (aMode == EHiraganaKanji) || (aMode == EKatakana) || ( aMode == EHangul )
        || (((aMode == ELatin) || (aMode == ENumber) || iMode == ENativeNumber ) && (iCharWidth == EFullWidthChar)))
        && (iAknEditorFlags & EAknEditorFlagLatinInputModesOnly))
        {
        //the editor does not allow Chinese modes
        isModePermitted = EFalse;
        }
    else if (((aMode == EStrokeFind) || (aMode == EZhuyinFind))
        && (!(iAknEditorFlags & EAknEditorFlagForceTransparentFepModes)))
        {
        //StrokeFind and ZhuyinFind modes are only valid in a chinese find pane
        isModePermitted = EFalse;
        }
    else
        {
        TUint editorsWhichAllowThisMode;
        if (aMode == ENumber || aMode == ENativeNumber )
            {
            //The FEP's ENumber mode has a direct mapping to EAknEditorNumericInputMode
            // or EAknEditorFullWidthNumericInputMode
            editorsWhichAllowThisMode = EAknEditorNumericInputMode;
            if (aWidth == EFullWidthChar)
                {
                editorsWhichAllowThisMode = EAknEditorFullWidthNumericInputMode;
                }
            }
        else if (aMode == EHiragana)
            {
            editorsWhichAllowThisMode = EAknEditorHiraganaInputMode;
            }
        else if (aMode == EKatakana || (aMode == ELatin && aWidth == EFullWidthChar))
            {
            editorsWhichAllowThisMode = EditorMode(aMode, aWidth);
            }
        else if (aMode == EHiraganaKanji)
            {
            // HiraganaKanji mode allows all input mode.
            editorsWhichAllowThisMode = EAknEditorAllInputModes;
            }
        else if (aMode == EHangul)
            {
            editorsWhichAllowThisMode =(EAknEditorTextInputMode | EAknEditorSecretAlphaInputMode | EAknEditorHalfWidthTextInputMode);     
            }          
        else
            {
            //if the editor allows Text Input or Secret Alpha Mode,
            //we infer that Chinese Modes are valid
            editorsWhichAllowThisMode =(EAknEditorTextInputMode | 
                                        EAknEditorSecretAlphaInputMode | 
                                        EAknEditorHalfWidthTextInputMode);
            }
        isModePermitted = iPermittedInputModes & editorsWhichAllowThisMode;
        }

    return isModePermitted;
    }

TInt CAknFepManager::SyncStates(TAknFepManagerState aState)
    {
    TInt ret = KErrNone;
    
    if (!iFepFullyConstructed)
        {
        if (aState == EAknFepStateNull)
            {
            return KErrNone;
            }
                       
        TRAP(ret, ConstructFullyL());
        }
    if (iFepFullyConstructed)
        {
        iFepManState = aState;
        iKeyCatcher->SetState(aState);
        }
   
    return ret;    
    }

TUint CAknFepManager::EditorMode(TInt aMode, TWidthChar aWidth) const
    {
    TInt editorMode = 0;
    switch(aMode)
        {
        case EPinyin:
        case EZhuyin:
        case EStroke:
        case ECangJie:
        case EPRCFind:
        case EZhuyinFind:
        case EStrokeFind:
        case EHangul:
            {
            editorMode = EAknEditorTextInputMode;
            }
            break;
        case EHindi:
        case ELatin:
        case ELatinText:
        case ELatinUpper:
        case ELatinLower:
            {
            if (iPermittedInputModes & EAknEditorTextInputMode)
                {
                 editorMode = EAknEditorTextInputMode;
                }
            else if (iPermittedInputModes & EAknEditorHalfWidthTextInputMode)
                {
                if (IsFeatureSupportedJapanese())
                    {
                    editorMode = EAknEditorHalfWidthTextInputMode;
                    }
                else
                    {
                    editorMode = EAknEditorTextInputMode;
                    }
                }
            else
                {
                editorMode = EAknEditorSecretAlphaInputMode;
                }
            if (aWidth == EFullWidthChar)
                {
                editorMode = EAknEditorFullWidthTextInputMode;
                }
            }
            break;
        case EHiragana:
            {
            editorMode = EAknEditorHiraganaInputMode;
            }
            break;
        case EHiraganaKanji:
            {
            editorMode = EAknEditorHiraganaKanjiInputMode;
            }
            break;
        case EKatakana:
            {
            editorMode = EAknEditorKatakanaInputMode;
            if (aWidth == EFullWidthChar)
                {
                editorMode = EAknEditorFullWidthKatakanaInputMode;
                }
            }
            break;
        case ENumber:
        case ENativeNumber:
            {
            editorMode = EAknEditorNumericInputMode;
            if (aWidth == EFullWidthChar)
                {
                editorMode = EAknEditorFullWidthNumericInputMode;
                }
            }
            break;
        default:
            break;
        }

    return editorMode;
    }

void CAknFepManager::LaunchPenSupportMenuL()
    {
    if (HashKeySelectionInUse() && IsCcpuFlagSet(ECcpuStateHashDown))
        {
        return;
        }
#ifdef RD_INTELLIGENT_TEXT_INPUT
	if(iCandidatePopup)        
		{
		iCandidatePopup->AttemptExitL(EFalse);
		HandleChangeInFocusL();
		}
#endif		
    
    if (IsFlagSet(EFlagInsideInlineEditingTransaction))
        {
        TryRemoveNoMatchesIndicatorL();
		/*
	    This code is the part of fixing TSW Error Edit Menu : 
	    "Matches" and "insert word" options are not available under Edit Menu.
	    When we launch Edit Menu.the word which in Inline Editing in the BackGorund 
	    Should not Commit*/        
#ifndef RD_INTELLIGENT_TEXT_INPUT
        CommitInlineEditL();
#endif
		/*
	    This code is the part of fixing TSW ErrorEdit Menu : 
	    "Matches" and "insert word" options are not available under Edit Menu.
	    This Flag is useful for deciding whether "Matches" Menu Item can Visible 
	    or not*/
#ifdef RD_INTELLIGENT_TEXT_INPUT        
        SetFlag(EFlagInlineEditInBackground);
#endif
		/*
	    This code is the part of fixing TSW Error  Edit Menu : 
	    "Matches" and "insert word" options are not available under Edit Menu.*/
#ifndef RD_INTELLIGENT_TEXT_INPUT        
        ClearFlag(EFlagInsideInlineEditingTransaction);
#endif        
        }
    
    CAknEdwinState* editorState = NULL;
    if (iInputCapabilities.FepAwareTextEditor())
        {
        editorState = EditorState();
        }
    
    iEditMenuBar = NULL;
    if (editorState)
        {
        iEditMenuBar = iUiInterface->EditorStateMenuBar(editorState);
        }
    
    if (iEditMenuBar && 
        !iEditMenuBar->IsDisplayed())
        {
        
        iPenSupportMenu = ETrue;
       // SetFlag(EFlagLaunchEditMenu);
        CAknFepUiInterfaceMenuPane* menuPane = iEditMenuBar->MenuPane();
        
        CEikMenuPane* oldMenuPane = menuPane->GetMenuPane();        
        if (!oldMenuPane)
            {
            menuPane = NULL;    
            }
        
        iRememberEditorState = EditorState();
  
        iEditMenuBar->SetMenuType(CAknFepUiInterfaceMenuBar::EMenuEdit);        
        // It is unnecessary to add ROW SCT in Editting option menu.
        iEditMenuBar->TryDisplayMenuBarL();
        if (menuPane && menuPane->NumberOfItemsInPane() == 0)
            {
            // Edit menu do not contain any items.
            iEditMenuBar->StopDisplayingMenuBar();
            }
        else
            {
            //adjust menu position to the front of dialog to
            //avoid input capability is wrongly got
            CCoeEnv* coeEnv = CCoeEnv::Static();
            CCoeAppUi* appUi = (CCoeAppUi*)(coeEnv->AppUi());
            if( appUi->IsDisplayingDialog() )
                {
                iEditMenuBar->RemoveFromStack();
                iEditMenuBar->AddToStackL(ECoeStackPriorityDialog + 1, ECoeStackFlagStandard);                              
                }
            }
            
        iEditMenuBar->SetMenuType(CAknFepUiInterfaceMenuBar::EMenuOptions);
        }
        iPenSupportMenu = EFalse;
    }

void CAknFepManager::LaunchSelectModeMenuL()
    {
    if (HashKeySelectionInUse() && IsCcpuFlagSet(ECcpuStateHashDown))
        {
        return;
        }
#ifdef RD_INTELLIGENT_TEXT_INPUT
	if(iCandidatePopup)        
		{
		iCandidatePopup->AttemptExitL(EFalse);
		HandleChangeInFocusL();
		}
#endif		
    
    if (IsFlagSet(EFlagInsideInlineEditingTransaction))
        {
        TryRemoveNoMatchesIndicatorL();
		/*
	    This code is the part of fixing TSW Error :  Edit Menu : 
	    "Matches" and "insert word" options are not available under Edit Menu.
	    When we launch Edit Menu.the word which in Inline Editing in the BackGorund 
	    Should not Commit*/        
#ifndef RD_INTELLIGENT_TEXT_INPUT
        CommitInlineEditL();
#endif
		/*
	    This code is the part of fixing TSW Error :  Edit Menu : 
	    "Matches" and "insert word" options are not available under Edit Menu.
	    This Flag is useful for deciding whether "Matches" Menu Item can Visible 
	    or not*/
#ifdef RD_INTELLIGENT_TEXT_INPUT        
        SetFlag(EFlagInlineEditInBackground);
#endif
		/*
	    This code is the part of fixing TSW Error :  Edit Menu : 
	    "Matches" and "insert word" options are not available under Edit Menu.*/
#ifndef RD_INTELLIGENT_TEXT_INPUT        
        ClearFlag(EFlagInsideInlineEditingTransaction);
#endif        
        }
    
    CAknEdwinState* editorState = NULL;
    if (iInputCapabilities.FepAwareTextEditor())
        {
        editorState = EditorState();
        }
    
    iEditMenuBar = NULL;
    if (editorState)
        {
        iEditMenuBar = iUiInterface->EditorStateMenuBar(editorState);
        }
    
    if (iEditMenuBar && 
        !iEditMenuBar->IsDisplayed())
        {
       // SetFlag(EFlagLaunchEditMenu);
        CAknFepUiInterfaceMenuPane* menuPane = iEditMenuBar->MenuPane();
        CEikMenuPane* oldMenuPane = menuPane->GetMenuPane();        
        if (!oldMenuPane)
            {
            menuPane = NULL;    
            }        
         if(menuPane)
        	SetFlag(EFlagLaunchEditMenu);
        
        iRememberEditorState = EditorState();
        
        // It is unnecessary to add ROW SCT in Editting option menu.
        if (!IsFeatureSupportedJapanese())
            {
            iEditCharsPtr.Copy(KNullDesC);
            iEditCharsPtr.Set( iSctEditChars->Des() );
            
            // get the SCT resource id from editor default settings
            TInt currentEditorSCTResId = GetCurrentEditorSCTResId();
            TInt charResourceId = ENoCharacters;
            if ( currentEditorSCTResId == EDefaultNumericCharMapResId)
                {
                charResourceId = NumericModeSCTResourceId();
                if ( charResourceId != ENoCharacters && IsAbleToLaunchSCT()&& 
                        EditorType() != CAknExtendedInputCapabilities::EEikSecretEditorBased)
                    {
                    
                    if( menuPane != NULL )
                        {
                        menuPane->ConstructMenuSctRowL( iEditCharsPtr, charResourceId );
                        }
                    }
                else
                    {
                    currentEditorSCTResId = charResourceId;
                    }   
                }
            else
                {
                TInt sctChars = EAknSCTChiChineseInputModes;
                if (iCharWidth == EFullWidthChar)
                    {
                    sctChars = EAknSCTFullCase;
                    }
                else
                    {
                    if (iMode == ELatin)
                        {
                        if ( FeatureManager::FeatureSupported(KFeatureIdChinese) )
                            {
                            sctChars = EAknSCTChiLatinInputModes;
                            }
                        else
                            {
                            if (iQwertyInputMode)
                                {
                                sctChars = EAknSCTQwerty;
                                }
                            else
                                {
                                sctChars = EAknSCTUpperCase;
                                if (iCaseMan->CurrentCase() == EAknEditorLowerCase)
                                    {
                                    sctChars = EAknSCTLowerCase;
                                    }
                                }
                            }
                        }
                    else if (iMode == ENumber || iMode == ENativeNumber)
                        {
                        sctChars = EAknSCTNumeric;
                        if ( ((iAknEditorFlags & EAknEditorFlagUseSCTNumericCharmap) ||
                            (iAknEditorNumericKeymap == EAknEditorAlphanumericNumberModeKeymap)) &&
                            !IsOnlyHalfWidthCharacterPermitted() )
                            {
                            sctChars = EAknSCTLowerCase;
                            }
                        }
                    //delete for phrase creation
//                    else if (iMode == EStroke && iFepManState == EAknFepStateUIActive)
//                    	{
//                    	return;
//                    	}
                    else if (iMode == EHiraganaKanji)
                        {
                        sctChars = EAknSCTFullCase;
                        }
                    else if(iMode == EKatakana)
                        {
                        sctChars = EAknSCTHalfCase;
                        }
                    }
                    
                if (!(ExtendedInputCapabilities() & CAknExtendedInputCapabilities::EDisableSCT))                    
                    {
                    if( menuPane && (EditorType() != CAknExtendedInputCapabilities::EEikSecretEditorBased))
                        {
                        if(IsAbleToLaunchSmiley())
                            {
                            sctChars |= EAknCharMapEmotionUse;
                            }
                        
                        menuPane->ConstructMenuSctRowFromDialogL( sctChars, iEditCharsPtr, 
                                                                  currentEditorSCTResId );                    
                        }
                    }
                }
            }
            iEditMenuBar->SetMenuType(CAknFepUiInterfaceMenuBar::EMenuEdit);
            iFepPluginManager->SetMenuState( ETrue ); 
            iEditMenuBar->TryDisplayMenuBarL();
            if (menuPane && menuPane->NumberOfItemsInPane() == 0)
                {
                // Edit menu do not contain any items.
                iEditMenuBar->StopDisplayingMenuBar();
                iFepPluginManager->ResetMenuState( ETrue );
                }
            else
                {
                //adjust menu position to the front of dialog to
                //avoid input capability is wrongly got
                CCoeEnv* coeEnv = CCoeEnv::Static();
                CCoeAppUi* appUi = (CCoeAppUi*)(coeEnv->AppUi());
                if( appUi->IsDisplayingDialog() )
                    {
                    iEditMenuBar->RemoveFromStack();
                    // Set the MenuBar not focused, the later add to stack operation could unfocus other control correctly
                    iEditMenuBar->SetFocus(EFalse);
                    iEditMenuBar->AddToStackL(ECoeStackPriorityDialog + 1, ECoeStackFlagStandard);                              
                    }
                }
                
            iEditMenuBar->SetMenuType(CAknFepUiInterfaceMenuBar::EMenuOptions);
        }
    }

void CAknFepManager::LaunchMatchesPopupListL()
    {
    TInt noOfMatches = iPtiEngine->NumberOfCandidates();
    if (noOfMatches > 1)    // 1 => only current match available, 0 => not in inline edit
        {
        // Create a list to pass to the dialog
        CDesCArrayFlat* matchesList=new(ELeave)CDesCArrayFlat(1);
        CleanupStack::PushL(matchesList);
        iPtiEngine->GetCandidateListL(*matchesList);
        TBuf<EMaximumFepWordLength> matchListDesc;
        TInt matchListItemCount = matchesList->MdcaCount();
        TInt currentIndex = ResolveCurrentCandidateListIndex(matchesList);      
        for ( TInt ii=0; ii < matchListItemCount ; ii++)
            {   
            matchListDesc = matchesList->MdcaPoint(ii);
            TChar tempChar = matchListDesc[0];
            if(tempChar.IsDigit())
                {
                AknTextUtils::ConvertDigitsTo(matchListDesc,iLanguageCapabilities.iLocalDigitType);
                matchesList->Delete(ii);
                matchesList->InsertL(ii,matchListDesc);
                }
            }

        // add 'Spell' to the bottom of the list
        TResourceReader reader;
        CCoeEnv* coeEnv = CCoeEnv::Static();
        coeEnv->CreateResourceReaderLC(reader, R_AVKON_T9_MATCHES_QUERY_SPELL_ITEM);
        matchesList->AppendL(reader.ReadTPtrC());
        CleanupStack::PopAndDestroy(); // reader

        TInt index = 0;
        SetFlag(EFlagForegroundUIComponentVisible);

        TUid fepUid = CCoeEnv::Static()->FepUid();
        ClearExtendedFlag(EExtendedFlagEdwinEditorDestroyed);
		SendEventsToPluginManL(EPluginEnablePriorityChangeOnOriChange,EFalse);
        // Fire up the dialog
		iMatchesListLaunched = ETrue;
        TInt returnValue = iUiInterface->LaunchListPopupL(R_AVKON_T9_MATCHES_QUERY, index, 
                                                          currentIndex, matchesList, NULL);
        iMatchesListLaunched = EFalse;
        PrepareFepAfterDialogExitL(fepUid);
		SendEventsToPluginManL(EPluginEnablePriorityChangeOnOriChange,ETrue);
        if(returnValue == EAknSoftkeyOk)
            {// Selected item 'index' from the matches array
            HandleChangeInFocusL();
            TBuf<EMaximumFepWordLength> newText;
            // Get current match from editor and show it as a default text in 'insert word' query.
            iInputCapabilities.FepAwareTextEditor()->GetEditorContentForFep(newText, 
                             iUncommittedText.iAnchorPos, iUncommittedText.Length());
            
            if (index==noOfMatches) // => 'spell' item at the bottom of the list
                {
                LaunchInsertWordQueryL(newText, iUncommittedText);
                #ifdef RD_SCALABLE_UI_V2
                if( iFepFullyConstructed && iFepPluginManager )
                    {
                    iFepPluginManager->ResetMenuState();
                    }
                #endif
                }
            else
                { // insert word at index into text
                TBuf<EMaximumFepWordLength> modifiedMatch;                
                modifiedMatch.Copy(matchesList->MdcaPoint(index));
                TChar first(newText[0]);
                if (first.IsUpper())
                    {
                    TChar converted(modifiedMatch[0]);
                    converted.UpperCase();
                    modifiedMatch[0] = converted;
                    }
                InsertTextFromDialogL(modifiedMatch, iUncommittedText);
                }
            }
        CleanupStack::PopAndDestroy(); // matchesList
        }
    }

#ifdef RD_INTELLIGENT_TEXT_INPUT
// Predictive QWERTY (XT9) changes ---->
void CAknFepManager::LaunchCandidatePopupListL( TInt aFocusedIndex )
    {
    iKeyBackSpaceHit = 1;
    //CPtiEngine* ptiEngine = iPtiEngine;
    
    // Addition for ITI features on FSQ.
    // Before open candidate list, if FSQ is opened, 
    // also need to check and close tooltip on it.
    SendEventsToPluginManL( EPluginHideTooltip );
    
    // Before open candidate list, if touch ui is opened, 
    // also need to close touch ui firstly.
    if ( iFepPluginManager->PluginInputMode() != EPluginInputModeNone )
        {
        iFepPluginManager->ClosePluginInputUiL( ETrue );
        }              

    iExactWordPopupContent->HidePopUp();
        
    TBool isShiftKeyDepressedAfterCandidateListLaunched = EFalse;
    // Retrieve active index for QWERTY keypad when Best Prediction is Set
    TInt activeIndex = KErrNotFound;
                
    if(iPrimaryCandidate && IsQwerty() && ( KeyboardLayout() != EPtiKeyboardHalfQwerty))
        {           
        iPtiEngine->HandleCommandL( EPtiCommandUserActionGetCurrentIndexOfCandidates, &activeIndex );        
        }
            
    TInt noOfMatches = iPtiEngine->NumberOfCandidates();
    if (noOfMatches > 0)
        {
        //! The compact candidate popup that opens with when arrow down pressed while inline editing
        //MAknFepCandidatePopup *candidatePopup;
        
        //Predictive QWERTY (XT9) changes for new Architecture ---->
        iCandidatePopup = UiInterface()->NewCandidatePopupL(*this);
        //Predictive QWERTY (XT9) changes for new Architecture <----
            
        // Determine the place where to spawn the candidate popup.
        TPoint  inlineEditorTl;
        TPoint  inlineEditorBr;
        TInt    height;
        TInt    ascent;
        TInt    documentOffset = iPtiEngine->CurrentWord().Length();
        
        //  Popup Position was not right for RTL ---->
        if(iLanguageCapabilities.iRightToLeftLanguage)
        	documentOffset = 0;
        //  Popup Position was not right for RTL <----
        GetScreenCoordinatesL(inlineEditorTl, height, ascent, documentOffset);
        
        inlineEditorTl.iY -= height;
        GetScreenCoordinatesL(inlineEditorBr, height, ascent);
        
        // Focus next word in list ---->
        // To fix the bug Candidate list opens with the highlight on 3rd candidate when Exact typing is use.
        // Show the popup.
        TInt selectedIdx = aFocusedIndex;
        TKeyEvent lastKeyEvent;
        
        if(selectedIdx == KErrNotFound)
            {
            if (noOfMatches > 1)
                {
                selectedIdx = KWordToFocusInCandidateList;
                }
            // Focus next word in list <----
            if(iSListLaunchedFromMenu)
                {
                iPtiEngine->HandleCommandL( EPtiCommandUserActionGetCurrentIndexOfCandidates, &selectedIdx );
                iSListLaunchedFromMenu = EFalse;
                }
            }
        TUid fepUid = CCoeEnv::Static()->FepUid();
        ClearExtendedFlag(EExtendedFlagEdwinEditorDestroyed);
        TInt popupCmd = iCandidatePopup->ExecutePopupL( TRect(inlineEditorTl, inlineEditorBr),
                                                        selectedIdx, lastKeyEvent, iLanguageCapabilities.iRightToLeftLanguage,
                                                        (TInt)KeyboardLayout() );
        PrepareFepAfterDialogExitL(fepUid);      
        
        if((lastKeyEvent.iScanCode == EStdKeyDevice3 ) || 
        		((lastKeyEvent.iScanCode == KAknFEPZeroKey)&& ((KeyboardLayout()== EPtiKeyboard12Key)||(KeyboardLayout()==EPtiKeyboardHalfQwerty))) 
        		|| lastKeyEvent.iCode == EKeySpace )
        	{
        	SetExtendedFlag(EExtendedFlagOkKeyPressOnCandidatePopup);
        	}
       
     
        TBool needToExecute(EFalse);        
        //The candidate list was closed by long key press of shift key
        isShiftKeyDepressedAfterCandidateListLaunched = IsFlagSet(EFlagShiftKeyDepressed) && (KeyboardLayout()== EPtiKeyboard12Key);
        if (popupCmd != EAknSoftkeyCancel)
            {
            HandleChangeInFocusL();
                              
            if (popupCmd == EAknFepSoftkeySpell)
                {                               
                // For QWERTY Input Mode
                if (iQwertyInputMode && EPtiKeyboardHalfQwerty != iKeyboardType) 
                    {   
                    TInt activeIdx = KErrNotFound;
                    TInt secondaryIdx = KErrNotFound;
                                    
                    iPtiEngine->HandleCommandL( EPtiCommandUserActionGetCurrentIndexOfCandidates, 
                                                &activeIdx );
                    iPtiEngine->HandleCommandL( EPtiCommandUserActionGetIndexOfSecondaryCandidate, 
                                                &secondaryIdx );
                    
                    if(activeIdx != secondaryIdx)
                        {                        
                        iPtiEngine->HandleCommandL( 
                                 EPtiCommandUserActionSetCurrentIndexOfCandidates, &secondaryIdx );                         
                        }
                    else
                        {
                        secondaryIdx = 0;
                        iPtiEngine->HandleCommandL( 
                                 EPtiCommandUserActionSetCurrentIndexOfCandidates, &secondaryIdx );  
                        }
                    TPtrC selectedWord = iPtiEngine->CurrentWord();
                    if(!IsFlagSet(EFlagInsideInlineEditingTransaction))
                        {
                        StartInlineEditingWithSelectedWord(selectedWord);
                        }
                    UpdateInlineEditL( selectedWord, selectedWord.Length() );
                        
                    } 
                // For ITUT Keypad, remove autocompletion part before launching Query Dialog
                else
                    {
                    RemoveSuggestedAdvanceCompletionL();
                    }
                
                // delete the candidate popup before launching the edit word dialog
                // as the candidate list has already been closed.
                if(iCandidatePopup)
                    {
                    delete iCandidatePopup;
                    iCandidatePopup = NULL;
                    }
                
                LaunchEditWordQueryL(); 
                }           
            
            // Close with space or selection key: Commit and add space or move cursor ahead one
            // character if next character already is space
            else if ( ( lastKeyEvent.iScanCode == EStdKeyDevice3 || lastKeyEvent.iScanCode == KAknFEPZeroKey )
            		 && ( KeyboardLayout() == EPtiKeyboard12Key || KeyboardLayout() == EPtiKeyboardHalfQwerty ) )
                {
                iPtiEngine->HandleCommandL( EPtiCommandUserActionSetCurrentIndexOfCandidates, 
                                            &selectedIdx ); 
                TPtrC selectedWord = iPtiEngine->CurrentWord();
                if(!IsFlagSet(EFlagInsideInlineEditingTransaction))
                    {
                     StartInlineEditingWithSelectedWord(selectedWord);
                    }
                UpdateInlineEditL( selectedWord, selectedWord.Length() );
                iPtiEngine->CommitCurrentWord();
                CommitInlineEditL();
				#ifdef RD_INTELLIGENT_TEXT_INPUT    
				    iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(iUncommittedText);
				#endif    

                AddOneSpaceOrMoveCursorL();
                }
            // Close with right arrow commits the word, does not add space
            else if ( lastKeyEvent.iScanCode == EStdKeyRightArrow )
            	{
                // Change of UI Spec
                /*
                 * The following codde change is a part of bug fix for improper 
                 * behavior of right arrow key when the candidate list is open. The
                 * right arrow key according to the UI specs should not the selected 
                 * candicdate and cursor should be end of the word.
                 */
                iPtiEngine->HandleCommandL( EPtiCommandUserActionSetCurrentIndexOfCandidates, 
                                            &selectedIdx );               
                TPtrC selectedWord = iPtiEngine->CurrentWord();
                if(!IsFlagSet(EFlagInsideInlineEditingTransaction))
                    {
                     StartInlineEditingWithSelectedWord(selectedWord);
                    }
                UpdateInlineEditL( selectedWord, selectedWord.Length() );  //selection.iAnchorPosition
                iPtiEngine->CommitCurrentWord();
                CommitInlineEditL();
                #ifdef RD_INTELLIGENT_TEXT_INPUT    
				    iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(iUncommittedText);
				#endif  

                SimulateKeyEventL(EKeyF19);
                }
            else if(popupCmd == EAknSoftkeyOk)
                {
                iPtiEngine->HandleCommandL( EPtiCommandUserActionSetCurrentIndexOfCandidates, 
                                            &selectedIdx ); 
                TPtrC selectedWord = iPtiEngine->CurrentWord();
                if(!IsFlagSet(EFlagInsideInlineEditingTransaction))
                    {
                     StartInlineEditingWithSelectedWord(selectedWord);
                    }
                UpdateInlineEditL( selectedWord, selectedWord.Length() );
                iPtiEngine->CommitCurrentWord();
                CommitInlineEditL();
                AddOneSpaceOrMoveCursorL();
                }
            else
                {
                iPtiEngine->HandleCommandL( EPtiCommandUserActionSetCurrentIndexOfCandidates, 
                                            &selectedIdx ); 
                TPtrC selectedWord = iPtiEngine->CurrentWord();
                if(!IsFlagSet(EFlagInsideInlineEditingTransaction))
                    {
                     StartInlineEditingWithSelectedWord(selectedWord);
                    }
                UpdateInlineEditL( selectedWord, selectedWord.Length() );
                needToExecute = ETrue;
                }
            }
        else if (popupCmd == EAknSoftkeyCancel && iPrimaryCandidate && IsQwerty() 
				             && ( KeyboardLayout() != EPtiKeyboardHalfQwerty ) )
            {
            iPtiEngine->HandleCommandL( EPtiCommandUserActionSetIndexOfActiveCandidate, &activeIndex );             
            iKeyBackSpaceHit = 0;
            }            
		else if(lastKeyEvent.iCode == EKeyPhoneEnd )
			{
			HandleChangeInFocusL();
			CCoeEnv::Static()->SimulateKeyEventL( lastKeyEvent, EEventKey );
			}		
	    else if (popupCmd == EAknSoftkeyCancel && IsQwerty() && ( KeyboardLayout() != EPtiKeyboardHalfQwerty ) ) 
           iKeyBackSpaceHit = 0;
		
        // If the dialog is not closed by pressing softkeys, escape, or right arrow
        // then the pressed key is handled also by Fep. This way the user can keep typing when the popup is open.
      
        //=============== Error fixing and simplify the condition ==========================================
        // The following piece of code should be excuted, if focus word
        // in the candidate list is accept, otherwise word sholud remain as it is.
        // If user does not accept the word,popupCmd always be EAknSoftkeyCancel.
        // Insetead of earlier lengthy condition, makes it simple.
           
        if( needToExecute )
           {
            const TBool shifted = (lastKeyEvent.iModifiers & (EModifierLeftShift | 
                                   EModifierRightShift | EModifierShift));
            const TBool chrDown = (lastKeyEvent.iModifiers & (EModifierLeftFunc));                       
            
            const TBool fnDown = (lastKeyEvent.iModifiers & (EModifierRightFunc));                       
            if(fnDown  ) 
                iFnKeyManager->SetFnKeyState(CAknFepFnKeyManager::EFnKeyDown)  ;     

         	if (shifted)
          		{
          		SetFlag(EFlagQwertyShiftMode|EFlagNoActionDuringShiftKeyPress);
          		}
            else
                {
                ClearFlag(EFlagQwertyShiftMode|EFlagNoActionDuringShiftKeyPress);
                }
            
            if (chrDown)
                {
                SetFlag(EFlagQwertyChrKeyDepressed|EFlagNoActionDuringChrKeyPress);
                }
            else
                {
                ClearFlag(EFlagQwertyChrKeyDepressed|EFlagNoActionDuringChrKeyPress);
                }
            if ( lastKeyEvent.iScanCode == EStdKeyDevice3 )
                {
                CCoeEnv::Static()->SimulateKeyEventL( lastKeyEvent, EEventKey );
                }
            if (! ( lastKeyEvent.iScanCode == EStdKeyDevice3 || 
                    lastKeyEvent.iScanCode == EStdKeyRightArrow ) )
                {
                /*
                 * The following code change is a part of bug fix for improper 
                 * behavior of any keypress when the candidate list is open. The
                 * keypress according to the UI specs should lock the selected 
                 * candidate to editor and the inserted letter is appended to it. 
                 */                
                iPtiEngine->HandleCommandL( EPtiCommandLockCurrentCandidate);
                TPtrC selectedWord = iPtiEngine->CurrentWord();
                if(!IsFlagSet(EFlagInsideInlineEditingTransaction))
                    {
                     StartInlineEditingWithSelectedWord(selectedWord);
                    }
                UpdateInlineEditL( selectedWord, selectedWord.Length() );  //selection.iAnchorPosition
                // For QWERTY input mode, scanCode is used
			#ifdef __HALF_QWERTY_KEYPAD
                if( KeyboardLayout() == EPtiKeyboardHalfQwerty)
                    {
                    iHalfQwertyLastKeyEvent = lastKeyEvent;
                    iAsyncOwnSimulateKey.CallBack();                    
                    }
                else 
			#endif //__HALF_QWERTY_KEYPAD
                if (iQwertyInputMode)
                    {
                    CCoeEnv::Static()->SimulateKeyEventL( lastKeyEvent, EEventKey );
                    }
                // For ITUT input Mode, keyCode is used
                else
                    {
                    SimulateKeyEventL( lastKeyEvent.iCode);
                    }
                }
            }
            
        ShowExactWordPopupIfNecessaryL();
        if(iCandidatePopup)
            {
            delete iCandidatePopup;
            iCandidatePopup = NULL;
            }
        }
    //The flag ECcpuStateShiftkeyWasPressedBeforeLosingFocus suggests that the candidate list was closed with a 
    //long key press of edit key (shift key). There are multiple calls to HandleChangedInFocusL() one manually and the other  through
    //CONE focus change notifier. We want the flag to be set for CONE's focus handler.
    if(isShiftKeyDepressedAfterCandidateListLaunched)
        {
        SetCcpuFlag(ECcpuStateShiftkeyWasPressedBeforeLosingFocus);
        }
    }
void CAknFepManager::StartInlineEditingWithSelectedWord(TDesC& aTextToUncommit)
    {
    if(iConcatenationTimer->IsActive())
    	iConcatenationTimer->Cancel();
	
    TInt docLenFep=iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep();
                if (iUncommittedText.iCursorPos>docLenFep)
                    iUncommittedText.iCursorPos=docLenFep;
                if (iUncommittedText.iAnchorPos>docLenFep)
                    iUncommittedText.iAnchorPos=docLenFep;
                        // Activates western predictive UI to an inline editing state.
                FepUI()->ActivateUI();
                SyncStates(EAknFepStateUIActive);
                  // Predictive QWERTY (XT9) changes <----
                TRAP_IGNORE(StartInlineEditL(iUncommittedText, aTextToUncommit,
                        aTextToUncommit.Length(), EFalse));

    }
void CAknFepManager::GetCandidatesWithIndexL(CDesCArray* aArray,
        TInt& aActiveIdx, TInt& aSecondaryIdx)
    {
#ifndef RD_INTELLIGENT_TEXT_INPUT
	return;
#endif
    aActiveIdx = -1;
    aSecondaryIdx = -1;
    iPtiEngine->HandleCommandL(
            EPtiCommandUserActionGetCurrentIndexOfCandidates, &aActiveIdx);

    if (iKeyBackSpaceHit)
        aSecondaryIdx = aActiveIdx;
    else
        iPtiEngine->HandleCommandL(
                EPtiCommandUserActionGetIndexOfSecondaryCandidate,
                &aSecondaryIdx);

    if (aArray == NULL || aActiveIdx == aSecondaryIdx)
        return;

    iPtiEngine->GetCandidateListL(*aArray);
    iPtiEngine->HandleCommandL(
            EPtiCommandUserActionGetCurrentIndexOfCandidates, &aActiveIdx);
    iPtiEngine->HandleCommandL(
            EPtiCommandUserActionGetIndexOfSecondaryCandidate, &aSecondaryIdx);
    }

void CAknFepManager::ShowExactWordPopupIfNecessaryL()
    {
    // Ok, I have to do it here, same logics are in
    // TryPopExactWordInICFL and ShowExactWordPopupIfNecessaryL.
    // the code follows the old code's logic: this is a
    // equivalent of the OLD ShowExactWordPopupIfNecessaryL; but
    // now I can call TryPopExactWordInICFL and 
    // TryPopExactWordInOtherPlaceL directly after extracting them from
    // the OLD ShowExactWordPopupIfNecessaryL.
    iExactWordPopupContent->HidePopUp();
//    SendEventsToPluginManL(EPluginHideTooltip);
    if (!iWesternPredictive || !IsFlagSet(EFlagInsideInlineEditingTransaction))
        return;

    if (EPtiKeyboardHalfQwerty == KeyboardLayout())
        {
        if (IsFlagSet(CAknFepManager::EFlagNoMatches))
            UpdateCbaL(R_AKNFEP_SOFTKEYS_SPELL_EMPTY);
        return;
        }

    //TryPopExactWordInICFL();
    if (iFepPluginManager && !iFepPluginManager->IsTooltipOpenOnFSQ())
        TryPopExactWordInOtherPlaceL();
    }

void CAknFepManager::TryPopExactWordInICFL()
    {
#ifndef RD_INTELLIGENT_TEXT_INPUT
	return;
#endif
	
    // Before open tooltip,  
    // also need to check and close tooltip on it.
//    SendEventsToPluginManL(EPluginHideTooltip);
    if (!iWesternPredictive || !IsFlagSet(EFlagInsideInlineEditingTransaction))
        {
        SendEventsToPluginManL(EPluginHideTooltip);
        return;
        }

    TInt activeIdx, secondaryIdx;
    GetCandidatesWithIndexL(NULL, activeIdx, secondaryIdx);
    if (activeIdx == secondaryIdx)
        {
        SendEventsToPluginManL(EPluginHideTooltip);
        return;
        }

    CDesCArray* candidates = new (ELeave) CDesCArrayFlat(16);
    CleanupStack::PushL(candidates);
    GetCandidatesWithIndexL(candidates, activeIdx, secondaryIdx);
    CleanupStack::PopAndDestroy(candidates);
    if (activeIdx == secondaryIdx)
        {
        SendEventsToPluginManL(EPluginHideTooltip);
        return;
        }

    if (iFepPluginManager)
        SendEventsToPluginManL(EPluginShowTooltip, secondaryIdx);
    }

void CAknFepManager::TryPopExactWordInOtherPlaceL()
    {
#ifndef RD_INTELLIGENT_TEXT_INPUT
	return;
#endif
	
    iExactWordPopupContent->HidePopUp();
    if (!iWesternPredictive || !IsFlagSet(EFlagInsideInlineEditingTransaction))
        return;
    // In the proactive mode this popup is used to show the exact input if it differs from the
    // default candidate shown inline.
    // In the reactive mode it is used to show the best guess candidate if it differs from the
    // exact word shown inline.

    TBool popupBelowInline = EFalse; // ETrue -> popup is above inline editor
    TInt activeIdx, secondaryIdx;
    GetCandidatesWithIndexL(NULL, activeIdx, secondaryIdx);
    if (activeIdx == secondaryIdx)
        return;

    CDesCArray* candidates = new (ELeave) CDesCArrayFlat(16);
    CleanupStack::PushL(candidates);
    GetCandidatesWithIndexL(candidates, activeIdx, secondaryIdx);

    // In case the active index and the secondary index is same, 
    // then we do not need to display the popup
    if (activeIdx == secondaryIdx)
        {
        CleanupStack::PopAndDestroy(candidates);
        return;
        }

    TPtrC secondaryWord = (*candidates)[secondaryIdx];
    iExactWordPopupContent->SetTextL(secondaryWord);
    iExactWordPopupContent->SetArrowDirection(
            MAknFepUiWordPopupContent::EUpwards);
    CleanupStack::PopAndDestroy(candidates);

    TPoint popupTopRight;
    TInt height;
    TInt ascent;
    TInt documentOffset = iPtiEngine->CurrentWord().Length();

    GetScreenCoordinatesL(popupTopRight, height, ascent, documentOffset);
    iExactWordPopupContent->UpdateContentSize();

    TBool rightToLeftLang = IsRightToLeftParagraph(DocPos());
    if (!rightToLeftLang)
        {
        popupTopRight.iX += iExactWordPopupContent->Size().iWidth;
        }

    if (popupBelowInline)
        {
        popupTopRight.iY += ascent / 2;
        }
    else
        {
        popupTopRight.iY -= height + iExactWordPopupContent->Size().iHeight;
        }

    iExactWordPopupContent->SetPosition(popupTopRight);
    iExactWordPopupContent->ShowPopUp();
    }

void CAknFepManager::LaunchPredictiveSettingDialogL()
	{
#ifdef RD_SCALABLE_UI_V2	
    // Addtion of ITI features on FSQ.
    if ( iFepPluginManager 
         && iFepPluginManager->IsSupportITIOnFSQ() )
        {
        // Hide touch input ui firstly,
        // otherwise, touch ui can hide status pane.        
        // Let CAknFepPluginManager know this setting dialog will be opened.
        iFepPluginManager->ITISettingDialogOpen( ETrue );
        iFepPluginManager->ClosePluginInputUiL( ETrue );
        SendEventsToPluginManL( ELostForeground );
        }
#endif // RD_SCALABLE_UI_V2

	// Hide fixed toolbar just before launching predictive settings dialog
	CEikAppUiFactory * appUiFactory = NULL;
	appUiFactory = static_cast<CEikAppUiFactory*>(CEikonEnv::Static()->AppUiFactory());
	
	CAknToolbar * fixedToolbar = NULL;
	if(appUiFactory)
	    fixedToolbar = appUiFactory->CurrentFixedToolbar();
	
	// If fixedtoolbar is not there in the current editor, we don't need to hide it before launching settings dialog 
	// and unhide it after settings dialog is done.
	if( fixedToolbar && !( fixedToolbar->IsShown() && fixedToolbar->CountComponentControls()>0 ) )
	    fixedToolbar = NULL;
	
	if(fixedToolbar)
		{
	  fixedToolbar->SetDimmed(ETrue);
	  fixedToolbar->DrawDeferred();
	  }
    TUid fepUid = CCoeEnv::Static()->FepUid();
    ClearExtendedFlag(EExtendedFlagEdwinEditorDestroyed);
		
    if ( iFepPluginManager && iFepPluginManager->IsSupportITIOnFSQ() )
        {
	    RProperty::Set( KPSUidAknFep, KAknFepSettingDialogState, 2 );
	    }
    else
        {
	    RProperty::Set( KPSUidAknFep, KAknFepSettingDialogState, 1 );
		}
    
	UiInterface()->LaunchPredictiveSettingDialogL(R_PREDICTIVESETTING_DIALOG,
												  R_PREDICTIVESETTINGDIALOG_MENUBAR,
												  R_PREDICTIVETEXTOFF_CONFIRMATION_QUERY,
												  R_AKNFEP_PRED_INPUT_SETTINGS_TITLE);
    PrepareFepAfterDialogExitL(fepUid);	
	RProperty::Set(KPSUidAknFep,KAknFepSettingDialogState,0);
#ifdef RD_SCALABLE_UI_V2	
    // // Addtion of ITI features on FSQ.    
    if ( iFepPluginManager 
         && iFepPluginManager->IsSupportITIOnFSQ() )
        {        
        // Let CAknFepPluginManager know this setting dialog will be opened.
        iFepPluginManager->ITISettingDialogOpen( EFalse );        
        }
#endif // RD_SCALABLE_UI_V2

	// Enables fixed toolbar after predictive settings dialog is closed
	if(fixedToolbar)
		{
	  fixedToolbar->SetDimmed(EFalse);
	  fixedToolbar->DrawDeferred();
	  }
	}
#ifdef FF_DUAL_LANGUAGE_SUPPORT
void CAknFepManager::LaunchWritingLanguageSettingDialogL()
    {
    TUid fepUid = CCoeEnv::Static()->FepUid();
    ClearExtendedFlag(EExtendedFlagEdwinEditorDestroyed);
    UiInterface()->LaunchWritingLanguageSettingDialogL(R_INPUTLANGUAGE_DIALOG, R_PREDICTIVESETTINGDIALOG_MENUBAR, R_INPUT_LANGUAGE_SETTINGS_TITLE);
    PrepareFepAfterDialogExitL(fepUid);	
	}

void  CAknFepManager:: LaunchDualLanguageSettingDialogL()
    {
    /* In Chinese language variant,if there are more than one languages supporting dual language feature,
    display writing language setting view in which Primary and Secondary Languages can be selected from the list.
    
    In Chinese language variant,if there is only one language supporting dual language feature, 
    no need to display the Writing language view which contains Primary and Secondary Language. 
    Instead we can display the Writing language list directly to the user;from which the user can select the writing language*/

    if( FeatureManager::FeatureSupported( KFeatureIdChinese ) || FeatureManager::FeatureSupported( KFeatureIdJapanese ))
        {                        
        CArrayFixFlat<TInt>* languages = new( ELeave )CArrayFixFlat<TInt>( ELanguageArrayGranularity );
        CleanupStack::PushL( languages );
        iPtiEngine->GetAvailableLanguagesL( languages );
        
        TInt count = languages->Count();   
        TInt noLangSupportWesternPred = count;
        for (TInt index = 0; index < count; index++)
            {      
            TInt langCode = languages->At(index);            
            
            if (langCode == ELangTaiwanChinese ||
               langCode == ELangHongKongChinese ||
               langCode == ELangPrcChinese ||
               langCode == ELangJapanese ||
               langCode == ELangKorean )
                {
                noLangSupportWesternPred--;
                }            
            }                         
        CleanupStack::PopAndDestroy(languages);    
        
        if( noLangSupportWesternPred > 1 )
            {             
            SetFlag(CAknFepManager::EFlagNewSharedDataInputMode);
            LaunchWritingLanguageSettingDialogL();            
            if(IsFlagSet(CAknFepManager::EFlagNewSharedDataInputLanguage))
                {
                ClearFlag(CAknFepManager::EFlagNewSharedDataInputLanguage);
                HandleChangeInFocusL();
                ChangeInputLanguageL(iSharedDataInterface->InputTextLanguage());
                }
            }
        else
            LaunchLanguagesPopupListL();                                                
        }   
    else
        {
        LaunchWritingLanguageSettingDialogL();        
        if(IsFlagSet(CAknFepManager::EFlagNewSharedDataInputLanguage))
            {
            ClearFlag(CAknFepManager::EFlagNewSharedDataInputLanguage);
            HandleChangeInFocusL();
            ChangeInputLanguageL(iSharedDataInterface->InputTextLanguage());
            }
        } 
        
    }
#endif //FF_DUAL_LANGUAGE_SUPPORT
// Predictive QWERTY (XT9) changes <----
#endif //RD_INTELLIGENT_TEXT_INPUT
void CAknFepManager::LaunchInsertWordQueryL(const TDesC& aInitialText, 
                                            TCursorSelection aTextSpanToReplace)
    {
    TInt recourceId = R_AVKON_INSERT_WORD_QUERY;
    LaunchFepQueryDialogL(recourceId, aInitialText, aTextSpanToReplace);
    }

void CAknFepManager::LaunchEditWordQueryL()
    {

    TBuf<EMaximumFepWordLength> newText;
#ifndef RD_INTELLIGENT_TEXT_INPUT
	if(IsAutoCompleteOn())
  		{
        RemoveSuggestedCompletionL();
        }
#endif //RD_INTELLIGENT_TEXT_INPUT
    iInputCapabilities.FepAwareTextEditor()->GetEditorContentForFep(newText, 
                                                                    iUncommittedText.iAnchorPos, 
                                                                    iUncommittedText.Length());

    TInt recourceId = R_AVKON_INSERT_WORD_QUERY;
    LaunchFepQueryDialogL(recourceId, newText, iUncommittedText);
    }

void CAknFepManager::LaunchFepQueryDialogL(TInt aResourceId, const TDesC& aInitialText, 
                                           TCursorSelection aTextSpanToReplace)
    {
    HBufC* textBuf = HBufC::NewLC(EQueryBufferLength);  // buffer for inserted text
    TPtr text = textBuf->Des();

    if (aInitialText.Length() <= EQueryBufferLength)
        text.Copy(aInitialText);
    else // truncate the incoming text to EQueryBufferLength before copying
        {
        TPtrC truncatedText = aInitialText.Left(EQueryBufferLength);
        text.Copy(truncatedText);
        }

    if (IsFlagSet(EFlagInsideInlineEditingTransaction))
        {
        TryRemoveNoMatchesIndicatorL();
        CommitInlineEditL();
        }
    UpdateCbaL(NULL);

    TInt underlyingCase = iCaseMan->CurrentCase();
    TBool isCaseUpdatesSupressed = IsFlagSet(EFlagSupressAutoUpdateAtEditorStart);

#ifndef RD_INTELLIGENT_TEXT_INPUT
    if (aInitialText.Length())
        {
        iCaseMan->SetCurrentCase(EAknEditorLowerCase);
        if (aInitialText.Length() == 1)
            {
            if (STATIC_CAST(TChar, aInitialText[0]).IsUpper())
                iCaseMan->SetCurrentCase(EAknEditorTextCase);
            }
        else // length of initial text > 1
            {
            if (STATIC_CAST(TChar, aInitialText[0]).IsUpper())
                {
                if (STATIC_CAST(TChar, aInitialText[1]).IsLower())
                    iCaseMan->SetCurrentCase(EAknEditorTextCase);
                else // 2nd character is upper case
                    iCaseMan->SetCurrentCase(EAknEditorUpperCase);
                }
            }
        }
#endif // RD_INTELLIGENT_TEXT_INPUT

    if (iCaseMan->CurrentCase() == EAknEditorLowerCase 
     || iCaseMan->CurrentCase() == EAknEditorUpperCase)
        {
        SetFlag(EFlagSupressAutoUpdateAtEditorStart);
        }

    TransferFepStateToEditorL();

    SetFlag(EFlagForegroundUIComponentVisible);
    
    TInt textQueryEditorFlag = 0;
    if ((aResourceId == R_AVKON_INSERT_WORD_QUERY ||
         aResourceId == R_AVKON_EDIT_WORD_QUERY ) &&
         FeatureManager::FeatureSupported( KFeatureIdChinese ))
    	{
	    textQueryEditorFlag = EAknEditorFlagLatinInputModesOnly;
    	}
#ifdef RD_SCALABLE_UI_V2     
    if (iFepFullyConstructed && 
        iFepPluginManager->PluginInputMode() == EPluginInputModeItut )
   // if ( iFepFullyConstructed && 
    //    (( iFepPluginManager->PluginInputMode() == EPluginInputModeItut ) && IsChineseInputLanguage())) 
        {
        iFepPluginManager->SpellText(textBuf);
        iFepPluginManager->DisplaySpellEditor(textQueryEditorFlag, aInitialText, 
                                              TCursorSelection(aInitialText.Length(), 0));

        delete iSpellInitText;
        iSpellInitText = NULL;
	    iSpellInitText = aInitialText.AllocL();
	    iSpellInitCurSel = aTextSpanToReplace;

        iFepPluginManager->SetCaseUpdatesSupressed(isCaseUpdatesSupressed);
		//add to avoid text update late when open spell.
        //iFepPluginManager->SetITUTSpellingStateL(ETrue);    
        iFepPluginManager->SetCursorSelection(aTextSpanToReplace);
        CleanupStack::PopAndDestroy();
        
        return;
        }
#endif    
    
    if(iFepPluginManager)
        {
        iFepPluginManager->SetInEditWordQueryFlag(ETrue);
        }

    PrepareFepForFepAwareDialogLaunch();
    TUid fepUid = CCoeEnv::Static()->FepUid();
    ClearExtendedFlag(EExtendedFlagEdwinEditorDestroyed);
    TInt id = iUiInterface->TextQueryDialogL(text, *EditorState(), 
                                             textQueryEditorFlag, aResourceId);
    PrepareFepAfterFepAwareDialogExitL(fepUid);        
    PrepareFepAfterDialogExitL(fepUid);

    if(iFepPluginManager)
        {
        iFepPluginManager->SetInEditWordQueryFlag(EFalse);
        }                                             
    if (id == EAknSoftkeyOk)
        {
        // Insert the character into the edwin.
        InsertTextFromDialogL(text, aTextSpanToReplace);
        // Add the word to the user dictionary (except for single character)  after it has been added to the editor.
        // FepUI of the main editor is used. Different FepUI is used in multitap
        // query and predictive mode 'main' editor at least in Japanese variant.
#ifdef RD_INTELLIGENT_TEXT_INPUT
        if(text.Length() > 1)
            FepUI()->AddTextToUserDictionaryL(text);
#else
        FepUI()->AddTextToUserDictionaryL(text); 
#endif // RD_INTELLIGENT_TEXT_INPUT

        }
    else // restore original case and inline text
        {
        HandleChangeInFocus();
        if (aTextSpanToReplace.Length())
            {
            EditorState()->SetInlineEditSpan(aTextSpanToReplace);
            ConfigureFEPFromEditorStateL();
            }
        iCaseMan->SetCurrentCase(underlyingCase);
        TransferFepStateToEditorL();
        }

    if (isCaseUpdatesSupressed)
        {
        SetFlag(EFlagSupressAutoUpdate);
        }
    else
        {
        ClearFlag(EFlagSupressAutoUpdate);
        }
    ClearFlag(EFlagSupressAutoUpdateAtEditorStart);
    CleanupStack::PopAndDestroy(); // text
    }

void CAknFepManager::InsertTextFromDialogL(const TDesC& aTextFromDialog, 
                                           TCursorSelection aExtentOfTextToReplace)
    {
    SetCcpuFlag(ECcpuStateSupressCursorMoveToEnd);
    // ensure we're back on the correct editor
 
#ifdef RD_SCALABLE_UI_V2   
    iNotifyPlugin = EFalse; // not really focus change
    HandleChangeInFocus();
    iNotifyPlugin = ETrue;
#else
    HandleChangeInFocus();
#endif    
    // need to commit if the editor was previously performing an inline edit
    if (IsFlagSet(EFlagInsideInlineEditingTransaction))
        CommitInlineEditL();

    TInt addedTextLength=0;

    TCursorSelection currentSelection;
// In case input capabilities goes null leaving with an error code and allowing application to handle..
    if (NULL == iInputCapabilities.FepAwareTextEditor())
        User::Leave(KErrCorrupt);
    iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(currentSelection);
    // Prepare for the edit
    TInt docLength = iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep();
    TInt maxLength = iInputCapabilities.FepAwareTextEditor()->DocumentMaximumLengthForFep();

    if (aExtentOfTextToReplace.Length())
        {
        if (currentSelection.Length())
            // Want to 'cancel' the selection so move cursor & anchor to the end of
            // the selection and then to the end of the current word
            {
            TInt endOfSelection=currentSelection.HigherPos();
            iUncommittedText.SetSelection(endOfSelection, endOfSelection);
            if (CursorInsideWord())
                MoveCursorToEndOfWordL();
            }
        iUncommittedText = aExtentOfTextToReplace;
        iInputCapabilities.FepAwareTextEditor()->SetCursorSelectionForFepL(iUncommittedText);
        addedTextLength = aTextFromDialog.Length();
        if (maxLength != 0)
            {
            addedTextLength = Min(addedTextLength,
                (maxLength - docLength + iUncommittedText.Length()));
            }
        }
    else
        {
        addedTextLength = aTextFromDialog.Length();

        //maxLength of 0 means the editor has no limit
        if (maxLength != 0)
            {
            addedTextLength = Min(addedTextLength,
                                  (maxLength - docLength + currentSelection.Length()));
            }
        if (currentSelection.Length())
            {
            iUncommittedText.SetSelection(currentSelection.HigherPos(), 
                                          currentSelection.LowerPos());
            iInputCapabilities.FepAwareTextEditor()->SetCursorSelectionForFepL(iUncommittedText);
            }
        else // do this in order for editor to get the correct number of chars to format from GetFormatOfFepInlineText()
            iUncommittedText.iCursorPos+=addedTextLength;
        }
    // if necessary trim text to fit into available space in the editor & perform the edit
    TPtrC textToEditor = aTextFromDialog.Left(addedTextLength);

    StartInlineEditL(textToEditor);
    CommitInlineEditL();
#ifdef RD_INTELLIGENT_TEXT_INPUT    
    iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(iUncommittedText);
#endif    

    EditorState()->SetInlineEditSpan(iUncommittedText);
    }

void CAknFepManager::LaunchKutenCodeQueryL()
    {
    TInt code;
    TUid fepUid = CCoeEnv::Static()->FepUid();
    ClearExtendedFlag(EExtendedFlagEdwinEditorDestroyed);
    UiInterface()->JapaneseKutenQueryDialogL(R_AKNFEPUICTRLJAPANESEKUTEN_QUERY, code, 0);

    PrepareFepAfterDialogExitL(fepUid);

    HandleChangeInFocus();  // to re-establish contact with editor under KutenCode Query

    if (code > 0 && EditorHasFreeSpace())
        {
        TBuf<ESingleCharacter> text;
        text.Append((TText)code);
        SetFlag(EFlagInsideInlineEditingTransaction);
        iInputCapabilities.FepAwareTextEditor()->StartFepInlineEditL(text, ESingleCharacter, 
                                                                     ETrue, NULL, *this, *this);
        CommitInlineEditL();
        }
    }

TInt CAknFepManager::NumericModeSCTResourceId() const
    {
    TInt charMapResId = EDefaultNumericCharMapResId;
    TBool phoneIdle = (EditorType() == CAknExtendedInputCapabilities::EPhoneNumberEditor);            
    
    if( phoneIdle )
        {
        charMapResId=R_AKNFEP_SCT_NUMERIC_MODE_CHARS_STANDARD;
        return charMapResId;
        }

    switch (iAknEditorNumericKeymap)
        {
        case EAknEditorStandardNumberModeKeymap:
            charMapResId = R_AKNFEP_SCT_NUMERIC_MODE_CHARS_STANDARD;
            break;
        case EAknEditorCalculatorNumberModeKeymap:
            charMapResId = R_AKNFEP_SCT_NUMERIC_MODE_CHARS_CALCULATOR;
            break;
        case EAknEditorConverterNumberModeKeymap:
            charMapResId = R_AKNFEP_SCT_NUMERIC_MODE_CHARS_CONVERTER;
            break;
        case EAknEditorFixedDiallingNumberModeKeymap:
            charMapResId = R_AKNFEP_SCT_NUMERIC_MODE_CHARS_FIXED_DIALLING;
            break;
        case EAknEditorSATNumberModeKeymap:
            charMapResId = R_AKNFEP_SCT_NUMERIC_MODE_CHARS_SAT;
            break;
        case EAknEditorToFieldNumberModeKeymap:
            charMapResId = R_AKNFEP_SCT_NUMERIC_MODE_CHARS_TO_FIELD;
            break;
        case EAknEditorPlainNumberModeKeymap:
            charMapResId = R_AKNFEP_SCT_NUMERIC_MODE_CHARS_PLAIN;
            break;
        case EAknEditorNativeKeysNumberModeKeymap:
            //currently this is not in use
            break;
        case EAknEditorSATHiddenNumberModeKeymap:
            charMapResId = R_AKNFEP_SCT_NUMERIC_MODE_CHARS_SAT_HIDDEN;
            break;
        default:
            //We reach here because there is no valid SCT resource ID set
            //by the current editor. So best we can do now is set the 
            //default mode for numeric editors.
            charMapResId = R_AKNFEP_SCT_NUMERIC_MODE_CHARS_STANDARD;
            break;
        }
    if(IsHybridAplhaEditor() && !IsHybridAlphaModeChangedtoAplhanumeric())
        {
        charMapResId = R_AKNFEP_SCT_NUMERIC_MODE_CHARS_STANDARD;
        }
    return charMapResId;
    }


TBool CAknFepManager::HashKeyModeChangeBlockedInSearchField() const
    {
    if ((iAknEditorFlags & EAknEditorFlagFindPane) && 
         iSharedDataInterface->HashKeySelectionInUse() &&                
         !IsChineseInputLanguage() &&
         !IsKoreanInputLanguage() &&
         iLanguageCapabilities.iInputLanguageCode != ELangVietnamese &&
         !FeatureManager::FeatureSupported(KFeatureIdJapanese))
         {
         if (iMode == ELatin || iMode == ENumber || iMode == ENativeNumber )
            {
            return ETrue;               
            }                               
         }
             
    return EFalse;         
    }

    
TKeyResponse CAknFepManager::HandleHashKeyL(TKeyPressLength aLength)
    {
    TKeyResponse response = EKeyWasNotConsumed;
    if (IsFlagSet(EFlagPassNextKey))
        {
        ClearFlag(EFlagPassNextKey);
        }
    else if (IsFlagSet(EFlagShiftKeyDepressed))
        {
        response = EKeyWasConsumed;
        }
    else if (HashKeyModeChangeBlockedInSearchField())      
        {
        // Do not allow case changes for latin languages in search field
        // if hash key selection is active.
        response = EKeyWasConsumed;
        }
    else
        {
        if ( IsFlagSet(EFlagInsideMultitapInlineEditingTransaction) )
            {
#ifdef RD_HINDI_PHONETIC_INPUT	                    
		if(iSharedDataInterface->InputTextLanguage() != KLangHindiPhonetic)    
#endif            
            FepUI()->ExpireMultitapTimer();
            // For Japanese, send backspace key event in case hash key
            // is pressed while secret text input.
           if (IsFeatureSupportedJapanese())
                {
                if (iInputCapabilities.SupportsSecretText())
                    {
                    SimulateKeyEventL(EKeyF20);          //backspace
                    }
                }
#ifdef RD_HINDI_PHONETIC_INPUT	                    
		if(iSharedDataInterface->InputTextLanguage() != KLangHindiPhonetic)    
#endif            
                ClearFlag(EFlagInsideMultitapInlineEditingTransaction);
            iCaseMan->UpdateCase(ENullNaviEvent);
            }
        if (aLength == EShortKeyPress)
            {
            if ( iMode != ENumber && iMode != ENativeNumber )
                {
                iModeBefore = iMode;
                if (iModeBefore == ELatin)
                    {
                    iCaseBefore = iCaseMan->CurrentCase();
                    }
                }
            }

       // always keep previous input mode for Japanese input
       if (IsFeatureSupportedJapanese())
            {
            iModeBefore = iMode;
            if (iModeBefore == ELatin)
                {
                iCaseBefore = iCaseMan->CurrentCase();
                }
            }
       if (iMode == EHangul && aLength == ELongKeyPress)
       		{       
       		if (IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction))
                {
                TPtrC text = iPtiEngine->CurrentWord();
                if (text.Length() > 0)
            		{
            		CancelInlineEdit();
            		StartInlineEditL();
            		UpdateInlineEditL(iPtiEngine->PreviousCandidate(), iPtiEngine->PreviousCandidate().Length());
            		CommitInlineEditL();
            		}
            	else
            		{
            		CancelInlineEdit();          		
            		}
            	iPtiEngine->ClearCurrentWord();
                }
       		}
		
        response = iHashKeyMan->HandleKeyEventL(aLength);
        if (response == EKeyWasNotConsumed)
            {
            switch (iAknEditorNumericKeymap)
                {
            case EAknEditorStandardNumberModeKeymap: //fall through
            case EAknEditorFixedDiallingNumberModeKeymap:
            case EAknEditorSATNumberModeKeymap:
            case EAknEditorSATHiddenNumberModeKeymap:
            case EAknEditorAlphanumericNumberModeKeymap:
                if (!EditorHasFreeSpace())
                    {
                    response=EKeyWasConsumed;
                    }
                break;
            case EAknEditorCalculatorNumberModeKeymap: //fall through
            case EAknEditorConverterNumberModeKeymap:
                SimulateKeyEventL(CurrentDecimalSeparator());
                if (EditorHasFreeSpace())
                    {
                    SetFlag(EFlagCharacterAdded);
                    }
                response=EKeyWasConsumed;
                break;
            case EAknEditorToFieldNumberModeKeymap:
                SimulateKeyEventL(text_to_field_mode_hash_key);
                if (EditorHasFreeSpace())
                    {
                    SetFlag(EFlagCharacterAdded);
                    }
                response=EKeyWasConsumed;
                break;
            case EAknEditorPlainNumberModeKeymap:
                response=EKeyWasConsumed;
                break;
            case EAknEditorNativeKeysNumberModeKeymap:
                break;
            default:
                break;
                }
            }
        }
    return response;
    }

TInt CAknFepManager::EditorNumericKeymap() const
    {
    return iAknEditorNumericKeymap;
    }


TBool CAknFepManager::HashKeySelectionInUse() const
    {       
	if (ExtendedInputCapabilities() 
    & CAknExtendedInputCapabilities::EForceHashKeySelectionStatusFlagOff)
	    {
	    return EFalse;
	    }
        
    return iSharedDataInterface->HashKeySelectionInUse();           
    }     
  

TBool CAknFepManager::EditSubmenuInUse() const
	{
	if (RProcess().SecureId().iId == KPhoneSecureId)
		{
		if (iQwertyInputMode)
			{
			return EFalse;				
			}
		} 
			
	if (ExtendedInputCapabilities() & CAknExtendedInputCapabilities::EForceEditSubmenuStatusFlagOff)
	    {
	    return EFalse;
	    }			
			
	return iSharedDataInterface->EditSubmenuInUse();			
	} 


void CAknFepManager::DimEditMenuModeItems(CAknFepUiInterfaceMenuPane* aMenuPane)
    {
    // current item
    DimMenuItem(aMenuPane, iMode);

    // dim menu items for Chinese input modes that aren't valid in the current editor
    // or the device subvariant
    for (TInt mode = KFirstMode; mode <= KLastMode; mode = mode << 1)
        {
        if (!IsModePermitted(mode))
            {
            DimMenuItem(aMenuPane, mode);
            }
        }
    
    // Switch menu whether input language is supported related chinese phrase    
    MPtiLanguage* ptilang = iPtiEngine->GetLanguage(
                iLanguageCapabilities.iInputLanguageCode ); 
    if ( ptilang )
        {
        if( iMode != EPinyin && IsModePermitted( EPinyin ) )
            {
            if( ptilang->HasInputMode( EPtiEnginePinyinByPhrase ) ) 
                {
                aMenuPane->SetItemDimmed( EChinFepCmdModePinyin, ETrue );
                aMenuPane->SetItemDimmed( EChinFepCmdModePinyinPhrase, EFalse );
                }
            }
        
        if( iMode != EZhuyin && IsModePermitted( EZhuyin ) )
            {
            if( ptilang->HasInputMode( EPtiEngineZhuyinByPhrase ) ) 
                {
                aMenuPane->SetItemDimmed( EChinFepCmdModeZhuyin, ETrue );
                aMenuPane->SetItemDimmed( EChinFepCmdModeZhuyinPhrase, EFalse );
                }
            }
            
        if( iMode != EStroke && IsModePermitted( EStroke ) )
            {
            if( ptilang->HasInputMode( EPtiEngineStrokeByPhrase ) ) 
                {
                aMenuPane->SetItemDimmed( EChinFepCmdModeStroke, ETrue );
                aMenuPane->SetItemDimmed( EChinFepCmdModeStrokePhrase, EFalse );
                }
            }
        }
    
    if (IsAbleToLaunchSmiley())
        {
        TInt index;
        if (aMenuPane->MenuItemExists(EAknCmdEditInsertSmiley, index))
            {
            aMenuPane->SetItemDimmed(EAknCmdEditInsertSmiley, EFalse);
            }
        }        
    }

void CAknFepManager::DimMenuItem(CAknFepUiInterfaceMenuPane* aMenuPane, TInt aMode)
    {
    TInt dimmedMenuItem = 0;
    switch (aMode)
        {
        case EPinyin:
            dimmedMenuItem = EChinFepCmdModePinyin;
            break;
        case EZhuyin:
            dimmedMenuItem = EChinFepCmdModeZhuyin;
            break;
        case EStroke:
            dimmedMenuItem = EChinFepCmdModeStroke;
            break;
        case ECangJie:
            dimmedMenuItem = EChinFepCmdModeCangJie;
            break;
        case EZhuyinFind:
            dimmedMenuItem = EChinFepCmdModeZhuyinFind;
            break;
        case EStrokeFind:
            dimmedMenuItem = EChinFepCmdModeStrokeFind;
            break;
        case ELatin:
            if ( iLanguageCapabilities.iInputLanguageCode != ELangJapanese)
                {
                // According Katja's info, dim the current case item in the 
                // indicator menu.
                if ( iCaseMan->CurrentCase() == EAknEditorUpperCase )
                    {
                    dimmedMenuItem = EChinFepCmdModeLatinUpper;
                    }
                else if(iCaseMan->CurrentCase() == EAknEditorLowerCase)
                    {
                    dimmedMenuItem = EChinFepCmdModeLatinLower;
                    }
                else if(iCaseMan->CurrentCase() == EAknEditorTextCase)
                    {
                    dimmedMenuItem = EAknCmdEditModeLatinText;
                    }
                }
            break;
        case ENativeNumber:
            {
            aMenuPane->SetItemDimmed(EAknCmdEditModeArabicIndicNumber, ETrue);
            aMenuPane->SetItemDimmed(EAknCmdEditModeEasternArabicIndicNumber, ETrue);
            aMenuPane->SetItemDimmed(EAknCmdEditModeIndicNumber, ETrue);
            }
            break;
        default:
            break;
        }
    TInt index;
    if (dimmedMenuItem && aMenuPane->MenuItemExists(dimmedMenuItem, index))
        {
        aMenuPane->SetItemDimmed(dimmedMenuItem, ETrue);
        }
    }

void CAknFepManager::LaunchConfirmationNoteL(TInt aResourceId)
    {
    SetFlag(EFlagForegroundUIComponentVisible);
	iUiInterface->LaunchConfirmationNoteL(aResourceId);
    }

void CAknFepManager::NewCharacterL(const TDesC& aChar)
    {
    if ( aChar.Length() == 0 )
    	{
    	return;
    	}
    TChar ch = aChar[0];

    TBool validNewCandidate = CharIsValidInEditor(ch);
    // Check if the character is valid for the editor.
    if ( !validNewCandidate )
        {
        TChar firstNewCandidate = ch;
        TBool goneThroughAllCandidates(EFalse);
        TPtrC8 sequence = iPtiEngine->CurrentInputSequence();

        if ( sequence.Length() )
            {
            TPtiKey key = (TPtiKey)sequence[0];
                        
            TBuf<20> mapData;              
            iPtiEngine->MappingDataForKey(key, mapData, iPtiEngine->Case()); 
            // If the character was not coming from ptiengine mappings, fail instantly.              
            if (mapData.Locate(firstNewCandidate) != KErrNotFound)
                {                
                // If there is an ivalid character in chr-loop, try to jump to next valid one.
                for (TInt jj = 0; jj < mapData.Length(); jj++)
                    {
                    TPtrC text = iPtiEngine->AppendKeyPress(key);
				    if ( text.Length() == 0 )
				    	{
				    	continue;
				    	}
                    ch = text[0];
                    goneThroughAllCandidates = (ch == firstNewCandidate);                    
                    validNewCandidate = CharIsValidInEditor(ch);
                    if (validNewCandidate || goneThroughAllCandidates)
                        {
                        break;
                        }                                        
                    }
                }            
            }
        }
    if (!validNewCandidate)
        {
        // No valid characters at all.
        CancelInlineEdit();
        return;
        }

    if ( (ch == KAknFEPLineFeedSymbol ||
          ch == KAknFEPMirroredLineFeedSymbol) &&
        (EditorHasFreeSpace() || IsFlagSet(EFlagLastCharacterInEditor)))
        {
        SetFlag(EFlagLineFeedCharacter);
        }
    else
        {
        ClearFlag(EFlagLineFeedCharacter);
        }
        
    if ( (ch == EKeySpace) &&
        (EditorHasFreeSpace() || IsFlagSet(EFlagLastCharacterInEditor)))
        {
        SetFlag(EFlagSpaceCharacter);
        }
    else
        {
        ClearFlag(EFlagSpaceCharacter);
        }


    if (iInputCapabilities.SupportsSecretText())
        {
        if (IsFlagSet(EFlagInsideMultitapInlineEditingTransaction))
            {
            SimulateKeyEventL(EKeyF20); // to delete previous char in secret text editor
            }
        //we receive an extra key press if we simulate a key that is also a device key
        if (ch.IsDigit() || TUint(ch) == EStarKeyUnicodeValue || TUint(ch) == EHashKeyUnicodeValue)
            {
            SetFlag(EFlagPassNextKey);
            }
        if ( ch.IsDigit() && iLanguageCapabilities.iLocalDigitType == EDigitTypeArabicIndic )
            {
            ClearFlag(EFlagPassNextKey);
            SimulateKeyEventL(ch + (TChar)KLatinToArabicIndicDigitsDelta ); 
            }
        else if ( ch.IsDigit() 
               && iLanguageCapabilities.iLocalDigitType == EDigitTypeEasternArabicIndic )
            {
            ClearFlag(EFlagPassNextKey);
            if(iLanguageCapabilities.iInputLanguageCode == ELangUrdu 
            || iLanguageCapabilities.iInputLanguageCode == ELangFarsi)
                {
                SimulateKeyEventL(ch + (TChar)KLatinToEasternArabicIndicDigitsDelta );  
                }
            }    
        else if ( ch.IsDigit() && iLanguageCapabilities.iLocalDigitType == EDigitTypeDevanagari )
            {
            ClearFlag(EFlagPassNextKey);
            SimulateKeyEventL(ch + (TChar)KLatinToDevanagariDigitsDelta);
            }
        else
            {
            SimulateKeyEventL(ch);
            }
        SetFlag(EFlagInsideMultitapInlineEditingTransaction | EFlagInsideInlineEditingTransaction);
        }
    else if (EditorHasFreeSpace() || IsFlagSet(EFlagInsideMultitapInlineEditingTransaction))
        {
        TBuf<1> buf;
        if ( ch.IsDigit() && iLanguageCapabilities.iLocalDigitType == EDigitTypeArabicIndic )
            {
            buf.Append( ch + (TChar)KLatinToArabicIndicDigitsDelta );   
            }
        else if ( ch.IsDigit() 
               && iLanguageCapabilities.iLocalDigitType == EDigitTypeEasternArabicIndic )
            {
            if(iLanguageCapabilities.iInputLanguageCode == ELangUrdu 
            || iLanguageCapabilities.iInputLanguageCode == ELangFarsi)
                {
                buf.Append( ch + (TChar)KLatinToEasternArabicIndicDigitsDelta );                    
                }
            }    
        else if ( ch.IsDigit() && iLanguageCapabilities.iLocalDigitType == EDigitTypeDevanagari )
            {
            buf.Append( ch + (TChar)KLatinToDevanagariDigitsDelta );    
            }
        else
            {
            buf.Append( ch );
            }

        if (iFepManState == EAknFepStateInitial) //multitapping
            {
            if (!IsFlagSet(EFlagInsideMultitapInlineEditingTransaction))
                {
                TInt edSize = iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep();
                iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(iUncommittedText);
                if (iAknEditorFlags & EAknEditorFlagMTAutoOverwrite && iMode == ELatin 
                    && !WesternPredictive()
                    && iUncommittedText.iCursorPos < edSize 
                    && iUncommittedText.Length() == 0)
                    {
                    // If the cursor is at the beginning or in the middle of the text,
                    // existing characters are replaced with new ones in latin multitap input.
                    iUncommittedText.iCursorPos++;
                    StartInlineEditL(iUncommittedText, buf, ESingleCharacter, EFalse);
                    SetFlag(EFlagInsideMultitapInlineEditingTransaction);
                    UpdateInlineEditL(buf, ESingleCharacter);
                    }
                else
                    {
                    StartInlineEditL(buf);
                    SetFlag(EFlagInsideMultitapInlineEditingTransaction);
                    }
                }
            else
                {
                UpdateInlineEditL(buf, ESingleCharacter);
                }

            if(!EditorHasFreeSpace())
                {
                SetFlag(EFlagLastCharacterInEditor);
                }
            }
        else
            {
            iInputCapabilities.FepAwareTextEditor()->StartFepInlineEditL(buf, ESingleCharacter, 
                                                                        ETrue, NULL, *this, *this);
            }
        SetFlag(EFlagInsideInlineEditingTransaction);
        }
    }

void CAknFepManager::NewTextL(const TDesC& aText)
    {
    ClearFlag(EFlagLineFeedCharacter);
    TBool unlimit = EFalse;
    TInt freeSpace = EditorFreeSpace(unlimit);

    if (IsFlagSet(EFlagInsideMultitapInlineEditingTransaction) ||
        unlimit || freeSpace > 0 ) //
        {
        if( unlimit || freeSpace > aText.Length() )
            {
            iInputCapabilities.FepAwareTextEditor()->StartFepInlineEditL(aText, aText.Length(), ETrue, 
                                                                                 NULL, *this, *this);
            }
        else  
            {
            iInputCapabilities.FepAwareTextEditor()->StartFepInlineEditL(aText.Left(freeSpace), freeSpace, ETrue, 
                                                                                 NULL, *this, *this);
            }
        
        SetFlag(EFlagInsideInlineEditingTransaction);
        }
    }


void CAknFepManager::NewCharacterSequenceL(const TDesC& aText, TIndicInputResponse aResponse)
    {
    TChar zws((ZERO_WIDTH_SPACE));
    TChar virama((TAknFepUiIndicInputManager::Virama(TLanguage(
                    iLanguageCapabilities.iInputLanguageCode))));
    TBool ret = ETrue;
	TBuf<CAknFepManager::EMaximumFepWordLength> buf;
    buf.Zero();
    TBool isTextLayoutPresent = (AknFepDocumentNavigation() || TextLayout()) ? ETrue : EFalse;
    switch( aResponse )
        {
        case EIndicInputResponseNone:
            {
            NewCharacterL(aText);
            }
            break;
        case EIndicInputResponseNumber:
            {
            NewCharacterL(aText);
            CommitInlineEditL();
            }
            break;
        case EIndicInputResponseZWSandCharacter:
            {
            if (! iInputCapabilities.SupportsSecretText() && isTextLayoutPresent)
                {
                buf.Append(zws);
                NewCharacterL(buf);
                CommitInlineEditL();
                }
            NewCharacterL(aText);           
            }
            break;
        case EIndicInputResponseInsertZWS:
            {
            buf.Append(virama);
            if(isTextLayoutPresent)
                buf.Append(zws);
            ret = EFalse;           
            }
            break;
        case EIndicInputResponseInsertZWSandLigature:
            {
            buf.Append(virama);
            if(isTextLayoutPresent)
                buf.Append(zws);
            if ( iInputCapabilities.SupportsSecretText() )
                {
                buf.Append(aText);  
                }
            ret = EFalse;           
            }
            break;

        case EIndicInputResponseInsertRepha:
            {
            MCoeFepAwareTextEditor* fepAwareTextEditor = 
                iInputCapabilities.FepAwareTextEditor();

            if( fepAwareTextEditor )
                {
                TInt nextCharPos = 0;
                TCursorSelection curSel;
            
                fepAwareTextEditor->GetCursorSelectionForFep( curSel );
                
                // Insert Repha before the desired syllable.
                TBool leftFlag = GetNextVisualLeftCharacter( nextCharPos );
                if( leftFlag )
                    {
                    curSel.iCursorPos = nextCharPos;
                    curSel.iAnchorPos = nextCharPos;
                    
                    fepAwareTextEditor->SetCursorSelectionForFepL( curSel );
                                                
                    TAknFepUiIndicInputManager::GetRepha( buf, 
                        TLanguage( iLanguageCapabilities.iInputLanguageCode ) );

                    ret = EFalse;
                    }
                }
            break;
            }

        case EIndicInputResponseInsertRakar:
            {
            TAknFepUiIndicInputManager::GetRakar( buf, 
                TLanguage( iLanguageCapabilities.iInputLanguageCode ) );
            ret = EFalse;
            }
            break;
#ifdef RD_HINDI_PHONETIC_INPUT      
    	case EIndicInputResponsePhoneticMultitapText:
    		{
    		buf.Append(aText);
			ret = EFalse;	
    		}
    		break;
        case EIndicInputResponsePhoneticQwertyText:
            {
            buf.Append(aText);
            ret = EFalse;   
            }
            break;
#endif          

        case EIndicInputResponseInsertDirectLigature:
            {
            // In key-0, the new line character preceeds the Ksha.
            // First clear the new line flag.
            ClearFlag( EFlagLineFeedCharacter );
            
            buf.Append( aText );
            ret = EFalse;
            }
            break;

        case EIndicInputResponseInsertViramaZWSandDirectLigature:
            {
            // This case handles the insertion of Virama when star key
            // is pressed.

            // In key-0, the new line character preceeds the Ksha.
            // First clear the new line flag.
            ClearFlag( EFlagLineFeedCharacter );

            buf.Append(virama);
            if(isTextLayoutPresent)
                buf.Append(zws);
            if ( iInputCapabilities.SupportsSecretText() )
                {
                buf.Append(aText);  
                }
            ret = EFalse;
            }
            break;

        case EIndicInputResponseZWSandDirectLigature:
            {
            if (! iInputCapabilities.SupportsSecretText() && isTextLayoutPresent)
                {
                buf.Append(zws);
                NewCharacterL(buf);
                CommitInlineEditL();
                }
            buf.Zero();
            buf.Append( aText );
            ret = EFalse;
            }
            break;

#ifdef RD_MARATHI
		case EIndicInputResponseInsertEyeLashRa:
			{
			TAknFepUiIndicInputManager::GetEyeLashRa( buf, 
				(TLanguage)iLanguageCapabilities.iInputLanguageCode );
			ret = EFalse;
    		}
			break;

		case EIndicInputResponseInsertChandraA:
			{
			TAknFepUiIndicInputManager::GetChandraA( buf );
			ret = EFalse;
			}
			break;
#endif // RD_MARATHI

        default:
            break;
        }
        
    if(ret)
        return;
    
    if ( iInputCapabilities.SupportsSecretText() )
        {
        if ( IsFlagSet(EFlagInsideMultitapInlineEditingTransaction) )
            {
            SimulateKeyEventL(EKeyF20); // to delete previous char in secret text editor
            }
        for( TInt counter=0; counter<buf.Length(); counter++ )
            {
            if( (buf[counter] != ZERO_WIDTH_SPACE) )
                SimulateKeyEventL(buf[counter]);    
            }
        SetFlag(EFlagInsideMultitapInlineEditingTransaction | EFlagInsideInlineEditingTransaction);
        }
    else if ( EditorHasFreeSpace() || IsFlagSet(EFlagInsideMultitapInlineEditingTransaction) )
        {
        if ( iFepManState == EAknFepStateInitial ) //multitapping
            {
            if ( !IsFlagSet(EFlagInsideMultitapInlineEditingTransaction) )
                {
                TInt edSize = iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep();
                iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(iUncommittedText);
                if (iAknEditorFlags & EAknEditorFlagMTAutoOverwrite && iMode == ELatin 
                    && !WesternPredictive()
                    && iUncommittedText.iCursorPos < edSize 
                    && iUncommittedText.Length() == 0)
                    {
                    // If the cursor is at the beginning or in the middle of the text,
                    // existing characters are replaced with new ones in latin multitap input.
                    iUncommittedText.iCursorPos++;
                    StartInlineEditL(iUncommittedText, buf, ESingleCharacter, EFalse);
                    }
                else
                    {
                    StartInlineEditL();
                    }
                SetFlag(EFlagInsideMultitapInlineEditingTransaction);
                }
            UpdateInlineEditL(buf, buf.Length());

            if( !EditorHasFreeSpace() )
                {
                SetFlag(EFlagLastCharacterInEditor);
                }
            }
        SetFlag(EFlagInsideInlineEditingTransaction);

        // Check if text needs to be committed   		
        if( ( TAknFepUiIndicInputManager::IsToCommitCharSeq( aResponse ) ) )
            {
            CommitInlineEditL();    
            }
        if( aResponse == EIndicInputResponseInsertZWSandLigature )
            {
            NewCharacterL(aText);   
            }    
        else if( aResponse == EIndicInputResponseInsertViramaZWSandDirectLigature )
            {
            // Doing this here because we first commit the virama 
            // and ZWS and keep the ligature in the inline buffer.
            NewLigatureL( aText );
            }
        }
    }

void CAknFepManager::CommitInlineEditL()
    {   
    if ( !IsFlagSet( CAknFepManager::EFlagInsideInlineEditingTransaction ) )
        {
        return;
        }
#ifdef RD_INTELLIGENT_TEXT_INPUT
    // Before commit inline, check and close 
    // tooltip on FSQ first.
    SendEventsToPluginManL( EPluginHideTooltip );

    // Predictive QWERTY (XT9) changes ---->
    // Exact word popup is hidden when the inline editor is closed
    iExactWordPopupContent->HidePopUp();
    // Predictive QWERTY (XT9) changes <----
#endif //RD_INTELLIGENT_TEXT_INPUT
    if (iInputCapabilities.FepAwareTextEditor())
			{
	    if  (IsFlagSet(EFlagLineFeedCharacter))
	        {
	        if (iInputCapabilities.SupportsSecretText())
	            {
	            SimulateKeyEventL(EKeyF20); // backspace to delete previous char in secret text editor
	            SimulateKeyEventL(EKeyEnter);
	            SimulateKeyEventL(EKeyF19); // Asyncronous case update
	            }
	        else
	            {
	            __ASSERT_DEBUG(iInputCapabilities.FepAwareTextEditor(),
	                           AknFepPanic(EAknFepPanicNoHandleToFepAwareTextEditor));
	            iInputCapabilities.FepAwareTextEditor()->SetInlineEditingCursorVisibilityL(ETrue);
	            iInputCapabilities.FepAwareTextEditor()->CancelFepInlineEdit();
	            
	            // Multitapping enter event is not posted for find pane.
	            // It would never accept it anyway but the event causes problems in gallery's find.
	            // Allow real enter from qwerty keyboard though.
	            if (!(iAknEditorFlags & EAknEditorFlagFindPane) || iQwertyInputMode)
	                {                
	                SimulateKeyEventL(EKeyEnter);
	                SimulateKeyEventL(EKeyF19); // Asyncronous case update
	                }            
	            }
	        }
	    else if (IsFlagSet(EFlagSpaceCharacter | EFlagFullWidthSpaceCharacter))
	        {
	        TBuf<ESingleCharacter> charAsDesc(ESingleCharacter);
	
	        if (IsFlagSet(EFlagSpaceCharacter))
	            {
	            charAsDesc[0] = TText(0x0020);      // 0x0020 is space
	            }
	        else
	            {
	            charAsDesc[0] = TText(0x3000);      // 0x3000 is full-width space
	            }
	
	        if (iInputCapabilities.SupportsSecretText())
	            {
	            SimulateKeyEventL(EKeyF20); // backspace to delete previous char in secret text editor
	            SimulateKeyEventL(charAsDesc[0]);
	            SimulateKeyEventL(EKeyF19); // Asyncronous case update
	            }
	        else
	            {
	            iInputCapabilities.FepAwareTextEditor()->UpdateFepInlineTextL(charAsDesc, 1);
	            iInputCapabilities.FepAwareTextEditor()->SetInlineEditingCursorVisibilityL(ETrue);
	            CCoeEnv* coeEnv = CCoeEnv::Static();
	            iInputCapabilities.FepAwareTextEditor()->CommitFepInlineEditL(*coeEnv);
	            iCaseMan->UpdateCase(ENullNaviEvent);
	            if (!EditorHasFreeSpace())
	                {
	                SetFlag(EFlagEditorFull);
	                }
	            else
	            	{
	            	ClearFlag(EFlagEditorFull);
	            	}
	            }
	        }
	    else if (IsFlagSet(EFlagInsideInlineEditingTransaction) 
	         && !iInputCapabilities.SupportsSecretText())
	        {
	        // Chinese languages seem not to support this
	        if ( !IsChineseInputLanguage() &&
	            (iUncommittedText.Length() == 0 || 
	                    (
	#ifdef RD_INTELLIGENT_TEXT_INPUT
	                    !IsAutoCompleteOn() &&
	#endif //RD_INTELLIGENT_TEXT_INPUT                    
	                    !EditorSupportsNeutralProtection()
	                    ) 
	            )
	           )
	                    
	            { // empty insertions need no resolution.
	            /*Hindi*/
	            if( (TAknFepUiIndicInputManager :: IsIndicLangauge(
	                      TLanguage(iLanguageCapabilities.iInputLanguageCode))) 
	#ifdef RD_HINDI_PHONETIC_INPUT
	                || ( TAknFepUiIndicInputManager :: IsIndicPhoneticLanguage(
	                      TLanguage(iLanguageCapabilities.iInputLanguageCode)) )
	#endif
	            )
	                {
	                // NOTE! Need to get cursor visibility information from some where.
	                CommitInlineEditWithoutResolutionL( *(iInputCapabilities.FepAwareTextEditor()), 
	                                                    ETrue, EFalse); 
	                }
	            else
	                {
	                // NOTE! Need to get cursor visibility information from some where.
	                CommitInlineEditWithoutResolutionL( *(iInputCapabilities.FepAwareTextEditor()), 
	                                                    ETrue);
	                }
	            }
	        else
	            {
	
	            //note secret text editors do not use the MCoeFepAwareTextEditor interface,
	            //we only need to clear the flags to 'commit' a character
	
	            __ASSERT_DEBUG(iInputCapabilities.FepAwareTextEditor(),
	                           AknFepPanic(EAknFepPanicNoHandleToFepAwareTextEditor));
	            CCoeEnv* coeEnv = CCoeEnv::Static();
	
	            TBool showCursor = ETrue;
	            if (IsFlagSet(EFlagInsideMultitapInlineEditingTransaction) ||
	                IsCcpuFlagSet(ECcpuStateLosingFocus))
	                {
	                showCursor = EFalse;
	                }
                else
                    {
                    if(IsKoreanInputLanguage( ) && !IsQwerty() &&
                    		IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction))
                        {
                        showCursor = EFalse;
                        }
                    }

	            iInputCapabilities.FepAwareTextEditor()->SetInlineEditingCursorVisibilityL(showCursor);
	            ClearFlag(EFlagInsideInlineEditingTransaction);
	            iInputCapabilities.FepAwareTextEditor()->CommitFepInlineEditL(*coeEnv);
	
	            // Arabic & Hebrew required functionality added ---->
	            if (IsFlagSet(EFlagInsideMultitapInlineEditingTransaction)) // Condition added for AknFep.
	                {
	                // 3 * as each original character can potentially take 2 markers
	                HBufC* decoratedTextBuf = HBufC::NewLC(KMaxInlineEditSize + 2); // +2 to accommodate previous and next charcaters
	                TPtr decoratedText = decoratedTextBuf->Des();
	                // Resolve in a slightly wider range than the new text. Attempt to expand 1 character in each direction
	                TCursorSelection resolveSpan(iUncommittedText.HigherPos(), 
	                                             iUncommittedText.LowerPos());
	                TInt decCursor = 0;
	
	                // Gets the CPlainText object from the focused editor if it exists.
	                CPlainText* plainText = PlainText();
	                TBool containsTextField = EFalse;
	                if (resolveSpan.iAnchorPos > 0)
	                    {
	                    if (plainText)
	                        {
	                        // Check if surrounding document position contains text field.
	                        containsTextField = plainText->FindFields(resolveSpan.iAnchorPos - 1);
	                        }
	                    TBuf<ESingleCharacter> chDes;
	                    iInputCapabilities.FepAwareTextEditor()->GetEditorContentForFep( chDes, 
	                                              resolveSpan.iAnchorPos - 1, ESingleCharacter);
	                    if ( chDes.Length() != 0 && chDes[0] != CEditableText::EPictureCharacter && !containsTextField)
	                        {
	                        // Resolve span is not extended if surrounding character is picture character or it
	                        // contains text field. Text field data would be lost when FEP replaces existing editor
	                        // content with new text buffer content that does not have text field data.
	                        // ResolveDecoratedText() call does not handle these special cases which can has some 
	                        // effects to Arabic & Hebrew text rendering.
	                        resolveSpan.iAnchorPos--;
	                        }
	                    }
	                if (resolveSpan.iCursorPos < 
	                    iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep())
	                    {
	                    containsTextField = EFalse;
	                    if (plainText)
	                        {
	                        // Check if surrounding document position contains text fields.
	                        containsTextField = plainText->FindFields(resolveSpan.iCursorPos);
	                        }
	                    TBuf<ESingleCharacter> chDes;
	                    iInputCapabilities.FepAwareTextEditor()->GetEditorContentForFep( chDes, 
	                                                  resolveSpan.iCursorPos, ESingleCharacter);
	                    if ( chDes.Length() != 0 && chDes[0] != CEditableText::EPictureCharacter && !containsTextField)
	                        {
	                        // Resolve span is not extended if surrounding character is picture character or it
	                        // contains text field. Text field data would be lost when FEP replaces existing editor
	                        // content with new text buffer content that does not have text field data.
	                        // ResolveDecoratedText() call does not handle these special cases which can has some 
	                        // effect to Arabic & Hebrew text rendering.
	                        resolveSpan.iCursorPos++;
	                        decCursor++;
	                        }
	                    }
	
	                // resolveSpan has been widened. Now go get the text for that span.
	                iInputCapabilities.FepAwareTextEditor()->GetEditorContentForFep(decoratedText, 
	                                                 resolveSpan.iAnchorPos, resolveSpan.Length());
	
	                if ( decoratedText.Length() != resolveSpan.Length() )
	                    {
	                    // This assert checks is we have failed to retrieve enough text.  Should not happen
	                    // __ASSERT_DEBUG( decoratedText.Length() == resolveSpan.Length(),  Panic(EEikPanicT9CharPosOutOfRange) );
	                    // Abondon the resolution.
	
	                    iInputCapabilities.FepAwareTextEditor()->SetInlineEditingCursorVisibilityL( ETrue );
	                    }
	                else
	                    {
	                    TInt lowerOffset = iUncommittedText.LowerPos() - resolveSpan.LowerPos();
	
	                    TBool needsResolving = iInlineTextDecorator->ResolveDecoratedText(decoratedText,
	                        TCursorSelection(lowerOffset, lowerOffset + iUncommittedText.Length()));
	
	                    if ( needsResolving )
	                        {                      
	                        TBool setToTrue=EFalse;
	                        iInputCapabilities.FepAwareTextEditor()->Extension1()->StartFepInlineEditL(
	                            setToTrue,
	                            resolveSpan,
	                            decoratedText,
	                            decoratedText.Length() - decCursor,
	                            EFalse,
	                            0, // MFormCustomDrawer*
	                            *this,
	                            *this);
	
	                        iInputCapabilities.FepAwareTextEditor()->SetInlineEditingCursorVisibilityL( ETrue );
	                        if (IsFeatureSupportedJapanese())
	                            {
	                            // need to call clear buffer of PtiEngine for Japanese varint
	                            // before calling second CommitFepInlineEditL().
	                            // because the committed string is set as reading string twice
	                            iPtiEngine->ClearCurrentWord();
	                            }
	                        iInputCapabilities.FepAwareTextEditor()->CommitFepInlineEditL(*coeEnv);
	                        }
	                    else
	                        {
                            // Line cursor is made invisible here.  There is no line cursor in S60 LAF, even though there is
                            // an Edwin flag which govern this.  See CEikEdwin::SetCursorVisibilityL(TBool)
                            // Cursor is not set if this CommitInlineEditL call was prompted when losing focus.
	                        MAknFepDocumentNavigation* docNavi = AknFepDocumentNavigation();
                            
                            // Use FepAwareTextEditor to judge, not use TextView()
                            if ( (docNavi || FepAwareTextEditor()) && !IsCcpuFlagSet(ECcpuStateLosingFocus) )
                                {
                                if (docNavi)
                                    {
                                    docNavi->SetCursorVisibilityL( TCursor::EFCursorInvisible, 
                                                                 TCursor::EFCursorFlashing );
                                    }
                                else 
                                    {
                                    //Use Edwin (FepAwareTextEditor) to set cursor visibility
                                    FepAwareTextEditor()->SetInlineEditingCursorVisibilityL( ETrue );
                                    }
                                }
	                        }
	                    }
	                // iMatchState=EWordMatchFirst;
	
	                CleanupStack::PopAndDestroy(); // decoratedTextBuf
	                }
	            if (iInputCapabilities.FepAwareTextEditor())
	                {
	                iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(iUncommittedText);   
	                }
	            
	            iCaseMan->UpdateCase(ENullNaviEvent);
	            // <---- Arabic & Hebrew required functionality added
	            if (!EditorHasFreeSpace())
	                {
	                SetFlag(EFlagEditorFull);
	                }
	            else
	            	{
	            	ClearFlag(EFlagEditorFull);
	            	}
	            }
	        }
	    else if (IsFlagSet(EFlagInsideInlineEditingTransaction) 
	          && iInputCapabilities.SupportsSecretText())
	        {
	        CCoeEnv* coeEnv = CCoeEnv::Static();
	        iInputCapabilities.FepAwareTextEditor()->CommitFepInlineEditL(*coeEnv);
	        iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(iUncommittedText);
	        
	        if (iQwertyInputMode)
	            {
	            // Case may need to updated automatically in case of qwerty secret text editor
	            // beause of the sticky shift feature.
	            if (iCaseMan->CurrentCase() == EAknEditorLowerCase)
	                {
	                SetCase(ELowerCase);            
	                }
	            else
	                {
	                SetCase(EUpperCase);                        
	                }               
	            UpdateIndicators();         
	            }
	        }
			}
    
//    SendEventsToPluginManL( EPluginSyncFepAwareText, EPluginSyncUpdatedText );
      
    ClearFlag(EFlagInsideMultitapInlineEditingTransaction | EFlagInsideInlineEditingTransaction |
        EFlagLineFeedCharacter | EFlagLastCharacterInEditor | EFlagSpaceCharacter |
        EFlagFullWidthSpaceCharacter);
        
    // No-edit_key devices: Inline editing ending, allow edit-menu again.    
    ClearCcpuFlag(ECcpuStateIgnoreStarUp | ECcpuStateNewPredictiveWord);        
#ifdef RD_INTELLIGENT_TEXT_INPUT
    iUncommittedText.iCursorPos=0;
    iUncommittedText.iAnchorPos=0;
#endif 
    if (!iInputCapabilities.FepAwareTextEditor())
        {
        // In case while updating the inline edit, 3rd party app removes the focus out of editor before the transaction
        // completes, we return with an error.
        User::Leave(KErrCorrupt);
        }

    }

void CAknFepManager::CommitInlineEditL(const TDesC& aText, TInt aUncommitedTextChange)
    {
    if (iMode == EHiraganaKanji)
        {
        // When there is not enough editing space to enter the selected candidate,
        // the error tone is played.
        if ((iFreeSpaceOfEditor > 0) && (iFreeSpaceOfEditor < aText.Length()))
            {
            PlaySound(EAvkonSIDErrorTone);
            }
        }

    //maxLength of 0 means the editor has no limit
    TInt maxEdSize = iInputCapabilities.FepAwareTextEditor()->DocumentMaximumLengthForFep();
    if ( maxEdSize != 0 && iFreeSpaceOfEditor < aText.Length())
        {
        TPtrC charbuf = aText.Left(iFreeSpaceOfEditor);
        TInt uncommitedTextChange = charbuf.Length();
        UpdateInlineEditL(charbuf, uncommitedTextChange);
        }
    else
        {
        UpdateInlineEditL(aText, aUncommitedTextChange);
        }

    ClearFlag(EFlagInsideMultitapInlineEditingTransaction);
    CommitInlineEditL();
    }

void CAknFepManager::CancelInlineEdit()
    {
    if( !EditorState()->CurrentInlineEditSpan().Length() )
    	{
    	ClearFlag(EFlagInsideInlineEditingTransaction);
    	if(IsFlagSet(CAknFepManager::EFlagNoMatches))
    		{
    		ClearFlag(CAknFepManager::EFlagNoMatches); 
    		}
    	}
    if (IsFlagSet(EFlagInsideInlineEditingTransaction))
        {
        TRAP_IGNORE(TryRemoveNoMatchesIndicatorL());

        if (iInputCapabilities.SupportsSecretText())
            {
            TRAP_IGNORE(SimulateKeyEventL(EKeyF20)); // backspace to delete previous char in secret text editor
            }
        else if (IsFeatureSupportedJapanese()
              && iFepManState == EAknFepStateUIActive)
            {
            FepUI()->CloseUI();
            FepUI()->ExpireMultitapTimer();
            iPtiEngine->ClearCurrentWord();
            SyncStates(EAknFepStateInitial);
            }
        }

    iUncommittedText.iCursorPos=0;
    
    iUncommittedText.iAnchorPos=0;
    
    iInputCapabilities.FepAwareTextEditor()->CancelFepInlineEdit();
    ClearFlag(EFlagInsideMultitapInlineEditingTransaction | EFlagInsideInlineEditingTransaction 
              | EFlagLineFeedCharacter | EFlagSpaceCharacter | EFlagFullWidthSpaceCharacter);
    ClearCcpuFlag(ECcpuStateNewPredictiveWord);    
    }

void CAknFepManager::GetScreenCoordinatesL(TPoint& aLeftSideOfBaseLine, TInt& aHeight, 
                                           TInt& aAscent, TInt aDocumentOffset)
    {
    TCursorSelection cursorSelection;
    iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(cursorSelection);
    TInt documentPosition = cursorSelection.LowerPos()  - aDocumentOffset;
    if (documentPosition < 0)
        documentPosition = 0;
    iInputCapabilities.FepAwareTextEditor()->GetScreenCoordinatesForFepL(aLeftSideOfBaseLine, 
                                                          aHeight, aAscent, documentPosition);
    }

void CAknFepManager::GetScreenCoordinatesL(TPoint& aLeftSideOfBaseLine, 
                                           TInt& aHeight, TInt& aAscent)
    {
    TCursorSelection cursorSelection;
    MCoeFepAwareTextEditor* fepAwareTextEditor = iInputCapabilities.FepAwareTextEditor();

    fepAwareTextEditor->GetCursorSelectionForFep(cursorSelection);
    fepAwareTextEditor->GetScreenCoordinatesForFepL(aLeftSideOfBaseLine, aHeight, aAscent, 
                                                    cursorSelection.LowerPos());
    }

TBool CAknFepManager::EditorHasFreeSpace( TInt aRequiredNumberOfCharacter ) const
    {
    TBool unlimit = EFalse;
    TInt freeSpace = 0;
#ifdef RD_INTELLIGENT_TEXT_INPUT
	if(WesternPredictive())
	    {
	    freeSpace = EditorFreeSpaceForAutoWordCompletion(unlimit);	
	    }
	else
		{
#endif //RD_INTELLIGENT_TEXT_INPUT
    	freeSpace = EditorFreeSpace(unlimit);			
#ifdef RD_INTELLIGENT_TEXT_INPUT
		}
#endif //RD_INTELLIGENT_TEXT_INPUT
    if( unlimit || freeSpace > aRequiredNumberOfCharacter )
        {
        return ETrue;
        }
    return EFalse;
    }
    
// -----------------------------------------------------------------------------
// check whether the current editor is a Java secret editor
// ----------------------------------------------------------------------------- 
TBool CAknFepManager::IsJavaSecretEditor()
    {
    TBool JavaSecretEditor = EFalse;
    
    TUint ConstraintValue = 0;
    MObjectProvider* mop = iInputCapabilities.ObjectProvider();
    
    if ( mop )
        {
        CAknExtendedInputCapabilities* extendedInputCapabilities = 
        mop->MopGetObject( extendedInputCapabilities );
        if ( extendedInputCapabilities && iInputCapabilities.FepAwareTextEditor() ) 
            {
            ConstraintValue = extendedInputCapabilities->MIDPConstrainst();
            }
        }
    if( ConstraintValue & 0x10000 ) 
        {
        JavaSecretEditor = ETrue;
        }
    
    return JavaSecretEditor;
    }       

TBool CAknFepManager::IsEditorHasFreeSpace() const
    {
    TBool unlimit = EFalse;
    TInt freeSpace = 0;
#ifdef RD_INTELLIGENT_TEXT_INPUT
	if(WesternPredictive())
	    {
	    freeSpace = EditorFreeSpaceForAutoWordCompletion(unlimit);	
	    }
	else
		{
#endif //RD_INTELLIGENT_TEXT_INPUT
    	freeSpace = EditorFreeSpace(unlimit);			
#ifdef RD_INTELLIGENT_TEXT_INPUT
		}
#endif //RD_INTELLIGENT_TEXT_INPUT
    if( unlimit || freeSpace > 0 )
        {
        return ETrue;
        }
    return EFalse;
    }
    
TInt CAknFepManager::EditorFreeSpace(TBool& aUnlimit, 
    TBool isToCountUncommittedTextLength /*= EFalse */) const
    {
    aUnlimit = EFalse;

    if (!iInputCapabilities.FepAwareTextEditor())
        {
        //no handle to MCoeFepAwareTextEditor in non Edwin derived editors
        return 0;
        }

    //maxLength of 0 means the editor has no limit
    TInt maxEdSize = iInputCapabilities.FepAwareTextEditor()->DocumentMaximumLengthForFep();
    TCursorSelection currentSelection;
    iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(currentSelection);

    aUnlimit = (maxEdSize == 0 || ((iAknEditorFlags & EAknEditorFlagMTAutoOverwrite) &&
                                   (currentSelection.iCursorPos >= 0 && 
                                    currentSelection.iCursorPos < maxEdSize) ));
    if(aUnlimit)
        {
        return 0;
        }

    TInt edSize = iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep();

    if( isToCountUncommittedTextLength )
        {
        return ( maxEdSize - edSize + 
            currentSelection.Length() + UncommittedText().Length() );
        }
    
    return (maxEdSize - edSize + currentSelection.Length());

    }

#ifdef RD_INTELLIGENT_TEXT_INPUT
TInt CAknFepManager::EditorFreeSpaceForAutoWordCompletion(TBool& aUnlimit, 
    TBool /* isToCountUncommittedTextLength = EFalse*/ ) const
    {
    aUnlimit = EFalse;

    if (!iInputCapabilities.FepAwareTextEditor())
        {
        //no handle to MCoeFepAwareTextEditor in non Edwin derived editors
        return 0;
        }

    //maxLength of 0 means the editor has no limit
    TInt maxEdSize = iInputCapabilities.FepAwareTextEditor()->DocumentMaximumLengthForFep();
    TInt edSize = iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep();
    TInt keySequenceLength = iPtiEngine->CurrentInputSequence().Length();
    TCursorSelection currentSelectionOrUncommitted;    
	iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(currentSelectionOrUncommitted);

    aUnlimit = (maxEdSize == 0 || ((iAknEditorFlags & EAknEditorFlagMTAutoOverwrite) &&
                                   (currentSelectionOrUncommitted.iCursorPos >= 0 && 
                                    currentSelectionOrUncommitted.iCursorPos < maxEdSize) ));
    if(aUnlimit)
        {
        return 0;
        }
    else
        {

        if ( !iQwertyInputMode )
            {
            return ( maxEdSize - edSize + currentSelectionOrUncommitted.Length() );
            }
        else
            {
            // In the predictive QWERTY mode, the keySequenceLength doesn't necessarily have anything
            // to do with the amount of characters on screen. At least entering accented characters with
            // Chr-multitapping will increase the key sequence length although number of characters 
            // doesn't change (this is either bug or feature of ZiCore). Ask the auto-completion
            // length directly from PtiEngine instead of trying to calculate it on our own.
            TInt tailLength = 0;
            TRAP_IGNORE(iPtiEngine->HandleCommandL( EPtiCommandGetAutoCompletionTailLength, &tailLength ));
            if( tailLength < 0 )
             {
             tailLength = 0;
             }
            return (maxEdSize - edSize + currentSelectionOrUncommitted.Length() + tailLength);
            }
        }
    }
#endif
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
#ifdef RD_INTELLIGENT_TEXT_INPUT  

TInt CAknFepManager::DocumentLength() const
    {
    TInt ret = 0;
    if ( iInputCapabilities.FepAwareTextEditor() )
        {
        ret = iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep();
        }
    return ret;
    }
    
TBool CAknFepManager::KeyEventWillReplaceCharacter( const TKeyEvent& aKeyEvent )
    {
    // The function is currently used only in QWERTY mode. It might not work correctly
    // with ITU-T input.
    TBool ret = EFalse;
    if ( IsFlagSet(EFlagInsideMultitapInlineEditingTransaction) )
        {
        // Multitapping
        ret = ETrue;
        }
    else if ( aKeyEvent.iRepeats &&
              LongPressNumberEntryOnQwerty() &&
              FnKeyState() == CAknFepFnKeyManager::EFnKeyNone &&
              KeyMapsDifferentCharacterWithFn( (TPtiKey)aKeyEvent.iScanCode ) )
        {
        // QWERTY key long press producing another charcter
        ret = ETrue;
        }
    return ret;    
    }
#endif
#endif
TChar CAknFepManager::CurrentDecimalSeparator() const
    {
    TLocale tLoc;
    return tLoc.DecimalSeparator();
    }

void CAknFepManager::ConfigureFEPFromEditorStateL()
    {
    User::LeaveIfError(SyncStates(EAknFepStateInitial));

    CAknEdwinState* editorState = EditorState();
    TInt editorMode = editorState->CurrentInputMode();
    TInt sharedDataMode = iSharedDataInterface->InputMode();
    TBool latinCaseSet = EFalse;
    iPermittedInputModes = editorState->PermittedInputModes();
    TInt defaultInputMode = editorState->DefaultInputMode();
    if(iAknEditorFlags != editorState->Flags())
        {
        iAknEditorFlags = editorState->Flags();
        UpdateLocalDigitMode();
        }
    iAknEditorNumericKeymap = editorState->NumericKeymap();

    TLanguage localLanguage = ELangTest;
    if (GetLocalLanguage( localLanguage ) )
        {
        iLanguageCapabilities.iLocalInputLanguageInUse = ETrue;
        if ( localLanguage != iLanguageCapabilities.iInputLanguageCode )
            {
            ChangeInputLanguageL(localLanguage);
            }
        }
    else if (iLanguageCapabilities.iLocalInputLanguageInUse)
        {
        ChangeInputLanguageL(iSharedDataInterface->InputTextLanguage());
        iLanguageCapabilities.iLocalInputLanguageInUse = EFalse;
        ClearFlag(EFlagNewSharedDataInputMode);
		}

    if (IsFlagSet(EFlagNewSharedDataInputLanguage) )
        {
        //Global mode or input language has been changed in general settings
        if ( !iLanguageCapabilities.iLocalInputLanguageInUse)
            {
            ChangeInputLanguageL(iSharedDataInterface->InputTextLanguage());
            }
        ClearFlag(EFlagNewSharedDataInputLanguage);
        }

    if (IsFeatureSupportedJapanese())
        {
        // set Japanese predicitve ON/OFF
        if (iAknEditorFlags & EAknEditorFlagNoT9)
            {
            iJapanesePredictive = EFalse;
            }
        else
            {
            // Is it supported Japanese predictive input
            if (HasJapanesePredictionInputMode())
                {
                iJapanesePredictive = iSharedDataInterface->PredictiveTextJapaneseOn();
                }
            else
                {
                // if Predictive Japanese input isn't supported in PtiEngine, set to OFF.
                iJapanesePredictive = EFalse;
                iSharedDataInterface->ResetJapanesePredictiveTextOn();
                }
            }

        // In Japanese variant, EAknEditorTextInputMode of allow input mode
        // means all input mode.
        if (iPermittedInputModes & EAknEditorTextInputMode)
            {
            iPermittedInputModes |= (EAknEditorKatakanaInputMode |
                                    EAknEditorHalfWidthTextInputMode |
                                    EAknEditorFullWidthTextInputMode |
                                    EAknEditorFullWidthNumericInputMode |
                                    EAknEditorFullWidthKatakanaInputMode |
                                    EAknEditorHiraganaKanjiInputMode |
                                    EAknEditorHiraganaInputMode);
            }

        if (iAknEditorFlags & EAknEditorFlagLatinInputModesOnly)
            {
            if (iPermittedInputModes == EAknEditorNumericInputMode)
                {
                iPermittedInputModes = EAknEditorNumericInputMode;
                }
            else if (iPermittedInputModes & EAknEditorNumericInputMode)
                {
                iPermittedInputModes = (EAknEditorHalfWidthTextInputMode 
                                      | EAknEditorNumericInputMode);
                }
            else
                {
                iPermittedInputModes = EAknEditorHalfWidthTextInputMode;
                }
            }

        if (editorMode)
            {
            editorMode = ConfigureFepModeFromEditorMode(editorMode);
            }
        else // for new editors
            {
            iCharWidth = EHalfWidthChar;
            sharedDataMode = EHiraganaKanji;
            // First input mode is changed to Latin from Hiragana/Kanji
            // if display language is English.
            TInt dispLang = iSharedDataInterface->DisplayLanguage();
            // ELangTest means Automatic as display language in GS
            if (dispLang == ELangTest)
                {
                // UiLanguage isn't Japanese, Latin is set.
                if (iUiLanguage != ELangJapanese)
                    {
                    sharedDataMode = ELatin;
                    }
                }
            else if (dispLang != ELangJapanese)
                {
                // Display language isn't Japanese, Latin is set.
                sharedDataMode = ELatin;
                }

            if (!(iPermittedInputModes & EAknEditorHiraganaKanjiInputMode))
                {
                if (IsOnlyFullWidthCharacterPermitted())
                    {
                    iCharWidth = EFullWidthChar;
                    }
                else if (IsOnlyHalfWidthCharacterPermitted())
                    {
                    iCharWidth = EHalfWidthChar;
                    }

                if (iPermittedInputModes & (EAknEditorKatakanaInputMode |
                                        EAknEditorFullWidthKatakanaInputMode))
                    {
                    // In case setting Latin as above, Latin is used preferentially.
                    if (sharedDataMode == ELatin
                     && (iPermittedInputModes & 
                           (EAknEditorHalfWidthTextInputMode | EAknEditorFullWidthTextInputMode)))
                        {
                        sharedDataMode = ELatin;
                        }
                    else
                        {
                        sharedDataMode = EKatakana;
                        }
                    }
                else if (iPermittedInputModes & (EAknEditorHalfWidthTextInputMode |
                                             EAknEditorFullWidthTextInputMode))
                    {
                    sharedDataMode = ELatin;
                    }
                else if (iPermittedInputModes & (EAknEditorNumericInputMode |
                                                 EAknEditorFullWidthNumericInputMode))
                    {
                    sharedDataMode = ENumber;
                    }
                // First input mode is changed to latin from katakana
                // if display language is English.
                if (!(dispLang == ELangTest || dispLang == ELangJapanese))
                    {
                    if ((iPermittedInputModes &  EAknEditorKatakanaInputMode) &&
                        (iPermittedInputModes & EAknEditorHalfWidthTextInputMode))
                        {
                        sharedDataMode = ELatin;
                        }

                    if ((iPermittedInputModes &  EAknEditorKatakanaInputMode) &&
                        (iPermittedInputModes & EAknEditorFullWidthTextInputMode))
                        {
                        sharedDataMode = ELatin;
                        }
                    }
                }
            }
        }
    else // for other variants (western or chinese)
        {
        if (!(iPermittedInputModes & (EAknEditorTextInputMode | EAknEditorNumericInputMode 
            | EAknEditorSecretAlphaInputMode)))
            {
            // Any latin input mode is not permitted by the editor.
            // For compatibility permitted japanese input modes are checked and
            // corresponding latin input modes are allowed.
            if ( iPermittedInputModes & ( EAknEditorKatakanaInputMode |
                                    EAknEditorHalfWidthTextInputMode |
                                    EAknEditorFullWidthTextInputMode |
                                    EAknEditorFullWidthKatakanaInputMode |
                                    EAknEditorHiraganaKanjiInputMode |
                                    EAknEditorHiraganaInputMode
                                    | EAknEditorHangulInputMode ) )
                {
                iPermittedInputModes |= EAknEditorTextInputMode;
                }
            if ( iPermittedInputModes & EAknEditorFullWidthNumericInputMode )
                {
                iPermittedInputModes |= EAknEditorNumericInputMode;
                }

            if ( !(defaultInputMode & (EAknEditorTextInputMode | EAknEditorNumericInputMode)))
                {
                if ( defaultInputMode & ( EAknEditorKatakanaInputMode |
                                    EAknEditorHalfWidthTextInputMode|
                                    EAknEditorFullWidthTextInputMode |
                                    EAknEditorFullWidthKatakanaInputMode |
                                    EAknEditorHiraganaKanjiInputMode |
                                    EAknEditorHiraganaInputMode
                                    | EAknEditorHangulInputMode ) )
                    {
                    defaultInputMode = EAknEditorTextInputMode;
                    }
                else if (defaultInputMode & EAknEditorFullWidthNumericInputMode)
                    {
                    defaultInputMode = EAknEditorNumericInputMode;
                    }
                }
            }
        }

    // object provider items
    iEditMenuBar = NULL;
    editorState->SetMenu();
    editorState->SetCba();

#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
    iOptionsMenuBar = NULL;
#endif    

    SetWesternPredictive(iSharedDataInterface->PredictiveTextOn());
    //check that we don't have a Chinese find mode saved to an editor that is
    //not a Chinese find pane
    __ASSERT_DEBUG( !((editorMode == EZhuyinFind || editorMode == EStrokeFind) &&
        !(iAknEditorFlags & EAknEditorFlagForceTransparentFepModes))
        , AknFepPanic(EAknFepPanicFindModeSavedToNonFindEditor));

    ReadHashKeyLoopL();

    UpdateNumericEditorDigitType();
    if ( IsMfneEditor() ) 
        {
        UpdateLocalDigitMode();
        }
    
    if (editorMode) //should be an editor that has been used before
        {
        if (IsFlagSet(EFlagNewSharedDataInputMode) && (IsModePermitted(sharedDataMode)) && !IsKoreanInputLanguage()
                && (!(iAknEditorFlags & EAknEditorFlagForceTransparentFepModes)))
            {
            
                {
                //change to global mode, except in Find Pane, where we ignore
                //GS setting
                TryChangeModeL(sharedDataMode);
                ClearFlag(EFlagNewSharedDataInputMode);
                latinCaseSet = sharedDataMode & (ELatinUpper | ELatinLower);
                if (sharedDataMode == ELatinText)
                    {
                    editorState->SetFlags( editorState->Flags() & ~EFlagSupressAutoUpdate );
                    }
                }
            }

        // Next two cases are apps/UI intitialinsg their editors in the
        // with the modes defined in uikon.hrh, change these to an appropriate
        // FEP mode
        else if (editorMode == EAknEditorNumericInputMode)
            {
            iCharWidth = EHalfWidthChar;
            if ( (iMode == ENumber || iMode == ENativeNumber) && 
                 IsModePermitted(iMode))
                {
                TryChangeModeL( iMode );
                }
            else if ( IsInputModeAvailable(ENativeNumber) &&
                 ( iLanguageCapabilities.iLocalDigitType == EDigitTypeEasternArabicIndic || 
                   iLanguageCapabilities.iLocalDigitType == EDigitTypeDevanagari ||
                   iLanguageCapabilities.iLocalDigitType == EDigitTypeArabicIndic ) )
                {
                TryChangeModeL( ENativeNumber );
                }
            else
                {
                TryChangeModeL( ENumber );
                }
            }
        else if (editorMode == EAknEditorSecretAlphaInputMode)
            {
            //this is safe default as it is currently allowed in all FEPs
            SetWesternPredictive(EFalse);
            TryChangeModeL(ELatin);
            }
        else if (editorMode == EAknEditorTextInputMode)
            {        
            if (IsKoreanInputLanguage())
            	{
                if( iMode == EAknEditorNullInputMode )
            		{
            		SetWesternPredictive(EFalse);
         			TryChangeModeL(EHangul);
            		}
            	else
            		{
            		TryChangeModeL(ELatin);
            		}
            	}
            else
            	{
            	//this is safe default as it is currently allowed in all FEPs
            	TryChangeModeL(ELatin); 
            	}
            }
        else if ( editorMode == EAknEditorHangulInputMode )
        	{
            // Choose EHangul as the current fep mode 
            // if the editor mode stored in editor state is EAknEditorHangulInputMode. 
            TryChangeModeL(EHangul);        
        	}        
        else if (!iStrokeUsedInQWERTY)
        	{
        	if (editorMode == EStroke && sharedDataMode == ECangJie)
        		{
        		TryChangeModeL(sharedDataMode);
        		}
        	else
        		{
        		TryChangeModeL(editorMode);
        		}
        	}
        else  //normal case, use locally stored mode
            {
            if (editorMode == ECangJie && sharedDataMode == ECangJie && iQwertyInputMode)
            	{
            	TryChangeModeL(EStroke);
            	}
            else
            	{
            	TryChangeModeL(editorMode);
            	}
            }
        }
    else if ((iAknEditorFlags & EAknEditorFlagForceTransparentFepModes) 
              && !IsFeatureSupportedJapanese())  //new find pane
        {
        // the default chinese find mode is the first mode in the hash key loop, except PrcChinese
        if( iLanguageCapabilities.iInputLanguageCode == ELangPrcChinese)
            {
            // Default searching language shall be ELatin
            // Changed from EPinyin to ELatin here
            TryChangeModeL(ELatin);
            }
        else
            {
            TryChangeModeL(iHashKeyLoop[0]); 
            }
        }
    else    //new editor
        {
        if (defaultInputMode == EAknEditorNumericInputMode)
            {
            //numeric mode is not saved to/retreived from shared data
            iCharWidth = EHalfWidthChar;
            
            if( IsInputModeAvailable(ENativeNumber) && 
                ( iLanguageCapabilities.iLocalDigitType == EDigitTypeArabicIndic ||
                  iLanguageCapabilities.iLocalDigitType == EDigitTypeEasternArabicIndic ||
                  iLanguageCapabilities.iLocalDigitType == EDigitTypeDevanagari ) )
                {
                TryChangeModeL( ENativeNumber );
                }
            else
                {
                TryChangeModeL( ENumber );
                }
            }
        else if ((defaultInputMode == EAknEditorTextInputMode) &&
                 ((iAknEditorFlags & EAknEditorFlagLatinInputModesOnly) ||
                 (iAknEditorFlags & EAknEditorFlagNoT9)))
            {
            //Chinese Modes or Western Predictive mode not allowed.
            //See if we are ok going to the current shared data mode,
            //otherwise go for a safe default
            if (TryChangeModeL(sharedDataMode))
                {
                latinCaseSet = sharedDataMode & (ELatinUpper | ELatinLower);
                }
            else
                {
                TryChangeModeL(ELatin);
                }
            }
        else if (defaultInputMode == EAknEditorHalfWidthTextInputMode)
            {
            iCharWidth = EHalfWidthChar;
            TryChangeModeL(ELatin);
            }
        else if (defaultInputMode == EAknEditorHiraganaInputMode)
            {
            TryChangeModeL(EHiragana);
            }
        else if (  iLanguageCapabilities.iInputLanguageCode == ELangJapanese
              && (defaultInputMode == EAknEditorKatakanaInputMode
              || defaultInputMode == EAknEditorFullWidthKatakanaInputMode))
            {
            iCharWidth = (defaultInputMode == EAknEditorKatakanaInputMode)?
                                EHalfWidthChar : EFullWidthChar;
            TryChangeModeL(EKatakana);
            }
        else if (defaultInputMode == EAknEditorFullWidthTextInputMode)
            {
            iCharWidth = EFullWidthChar;
            TryChangeModeL(ELatin);
            }
        else if (defaultInputMode == EAknEditorFullWidthNumericInputMode)
            {
            iCharWidth = EFullWidthChar;
            if( IsInputModeAvailable(ENativeNumber) && 
                ( iLanguageCapabilities.iLocalDigitType == EDigitTypeArabicIndic ||
                  iLanguageCapabilities.iLocalDigitType == EDigitTypeEasternArabicIndic ||
                  iLanguageCapabilities.iLocalDigitType == EDigitTypeDevanagari ) )
                {
                TryChangeModeL( ENativeNumber );
                }
            else
                {
                TryChangeModeL( ENumber );
                }
            }
        else if (defaultInputMode == EAknEditorTextInputMode 
            && iLanguageCapabilities.iInputLanguageCode == ELangHindi )
            {
            TryChangeModeL( EHindi );
            }
        else // Default input mode is EAknEditorTextInputMode
            {
            // Check possibility move this section on else if
            if (IsKoreanInputLanguage())
            	{
            	TryChangeModeL(EHangul);
            	}
            else if (TryChangeModeL(sharedDataMode))
                {
                latinCaseSet = sharedDataMode & (ELatinUpper | ELatinLower);
                }
            else
                {
                TryChangeModeL(ELatin);
                }
                
            }
        }

    if (!iMode)
        {
        // If mode change wasn't successful, try latin mode as fallback.
        TryChangeModeL(ELatin);             
        }

    // if also fallback fails, panic.
    __ASSERT_DEBUG(iMode, AknFepPanic(EAknFepPanicNoPermittedEditorModesAvailable));

    if (iCaseMan && !latinCaseSet)
        {
        iCaseMan->ConfigureCaseStateFromEditorState();
        if ( (editorState->Flags() & EFlagSupressAutoUpdate) ||
            editorMode == EAknEditorSecretAlphaInputMode )
            {
            SetFlag(EFlagSupressAutoUpdate);
            }
        else
            {
            ClearFlag(EFlagSupressAutoUpdate);
            }
        }
    if ((iAknEditorFlags & EAknEditorFlagForceTransparentFepModes) &&
        IsChineseInputLanguage() && iCaseMan != NULL )
        {
        SetFlag(EFlagSupressAutoUpdate);
        iCaseMan->SetCurrentCase(EAknEditorLowerCase);
        }

    UpdateEditorContext();

    // Check if this editor was previously inline editing
    if ( WesternPredictive() && editorState->CurrentInlineEditSpan().Length())
        { // restore the fep to an inline editing state
        iUncommittedText = editorState->CurrentInlineEditSpan();
        TInt docLenFep=iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep();
        if (iUncommittedText.iCursorPos>docLenFep)
            iUncommittedText.iCursorPos=docLenFep;
        if (iUncommittedText.iAnchorPos>docLenFep)
            iUncommittedText.iAnchorPos=docLenFep;

        TBuf<EMaximumFepWordLength> textToUncommit;
        iInputCapabilities.FepAwareTextEditor()->
            GetEditorContentForFep(textToUncommit, iUncommittedText.iAnchorPos, 
                                   Min(iUncommittedText.Length(), EMaximumFepWordLength));

        // Activates western predictive UI to an inline editing state.
        FepUI()->ActivateUI();
        SyncStates(EAknFepStateUIActive);
        TInt err = iPtiEngine->SetCurrentWord(textToUncommit);
#ifdef RD_INTELLIGENT_TEXT_INPUT
        // Predictive QWERTY (XT9) changes (TEST) ---->
        // The PtiXt9Core may choose to ignore the SetCurrentInputSequence() call.
        // Make sure our text buffer is in sync with the one in PtiEngine.
        if(KErrNone != err)
        	textToUncommit = iPtiEngine->CurrentWord();
        TInt maxEdSize = iInputCapabilities.FepAwareTextEditor()->DocumentMaximumLengthForFep();
        if((maxEdSize > 0) && (iUncommittedText.iAnchorPos + textToUncommit.Length() > maxEdSize))
        {
        	textToUncommit.SetLength(maxEdSize - iUncommittedText.iAnchorPos);
        	iUncommittedText.iCursorPos = maxEdSize;
        }	
        // Predictive QWERTY (XT9) changes <----
#endif //RD_INTELLIGENT_TEXT_INPUT
        StartInlineEditL(iUncommittedText, textToUncommit, textToUncommit.Length(), EFalse);
       
        // Make sure editor character case in sync
        if( iUncommittedText.iAnchorPos == 0 )
        	iCaseMan->UpdateCase(ENullNaviEvent);
        
        }

    if (editorState != iPreviousEditorState)
        {
        // we have changed editor, time to update local digits mode
        UpdateLocalDigitMode(); 
        iPreviousEditorState = editorState;
        }

    if(IsExtendedFlagSet(EExtendedFlagShiftReleasedOnPopup) && !(iAknEditorFlags & EAknEditorFlagFindPane) )
        {
        // If in FEP context, shift is released on a non fep aware dialog
        // like "text copied to clipboard", then release the shift 
        // when the editor comes in focus by simulating shift up event.
        ClearExtendedFlag(EExtendedFlagShiftReleasedOnPopup);
        TKeyEvent shiftUp = {0, EStdKeyLeftShift, 0, 0};
        CCoeEnv::Static()->SimulateKeyEventL(shiftUp, EEventKeyUp);     
        }
    else if (IsFlagSet(EFlagLongShiftKeyPress) && !(iAknEditorFlags & EAknEditorFlagFindPane))
        {
        // Find pane editor does not require any shift simulation
        
        // After the "text copied" note and if the shift key was not released
        // then simulate shift down event.
        TKeyEvent shiftDown = {0, EStdKeyLeftShift, 0, 0};
        CCoeEnv::Static()->SimulateKeyEventL(shiftDown, EEventKeyDown);      
        if((!iCandidatePopup) && (KeyboardLayout() != EPtiKeyboard12Key ))
        ResetShiftKeyMonitorL();
        }
#ifdef RD_INTELLIGENT_TEXT_INPUT
    TransferFepStateToEditorL(EFalse); // ensure editor has up to date state
#else
    TransferFepStateToEditorL(); // ensure editor has up to date state
#endif   
    
    iHashKeyMan->ResetIndicHashKeyStateL();
    ClearExtendedFlag(EExtendedFlagShortPressHashKey);
    }

TInt CAknFepManager::ConfigureFepModeFromEditorMode(TInt aEditorMode)
    {
    TInt fepMode(aEditorMode);

    switch (aEditorMode)
        {
        case EAknEditorNumericInputMode:
            if( IsInputModeAvailable(ENativeNumber) && 
                ( iLanguageCapabilities.iLocalDigitType == EDigitTypeArabicIndic ||
                  iLanguageCapabilities.iLocalDigitType == EDigitTypeEasternArabicIndic ||
                  iLanguageCapabilities.iLocalDigitType == EDigitTypeDevanagari ) )
                {
                fepMode = ENativeNumber;
                }
            else
                {
                fepMode = ENumber;
                }
            iCharWidth = EHalfWidthChar;
            break;
        case EAknEditorSecretAlphaInputMode:
        case EAknEditorTextInputMode:
        case EAknEditorHalfWidthTextInputMode:
            fepMode = ELatin;
            iCharWidth = EHalfWidthChar;
            break;
        case EAknEditorKatakanaInputMode:
            fepMode = EKatakana;
            iCharWidth = EHalfWidthChar;
            break;
        case EAknEditorFullWidthTextInputMode:
            fepMode = ELatin;
            iCharWidth = EFullWidthChar;
            break;
        case EAknEditorFullWidthKatakanaInputMode:
            fepMode = EKatakana;
            iCharWidth = EFullWidthChar;
            break;
        case EAknEditorFullWidthNumericInputMode:
            if( IsInputModeAvailable(ENativeNumber) && 
                ( iLanguageCapabilities.iLocalDigitType == EDigitTypeArabicIndic ||
                  iLanguageCapabilities.iLocalDigitType == EDigitTypeEasternArabicIndic ||
                  iLanguageCapabilities.iLocalDigitType == EDigitTypeDevanagari ) )
                {
                fepMode = ENativeNumber;
                }
            else
                {
                fepMode = ENumber;
                }
            iCharWidth = EFullWidthChar;
            break;
        case EAknEditorHiraganaKanjiInputMode:
            fepMode = EHiraganaKanji;
            break;
        case EAknEditorHiraganaInputMode:
            fepMode = EHiragana;
            break;
        default:
            break;
        }
    return fepMode;
    }

void CAknFepManager::LaunchHelpTextQueryL()
    {
    TUid fepUid = CCoeEnv::Static()->FepUid();  
    ClearExtendedFlag(EExtendedFlagEdwinEditorDestroyed);
    iUiInterface->LaunchHelpTextQueryL(R_AKNFEP_HELP_TEXT);
    PrepareFepAfterDialogExitL(fepUid);
    }

void CAknFepManager::UpdateEditorContext() const
    {
    if(EditingStateIndicator())
        {
        iUiInterface->UpdateEditorContext(*EditingStateIndicator(), *FepUI());
        }
    }

TBool CAknFepManager::TextIsValidInEditor(const TDesC& aText)
    {
    TBool ret = ETrue;
    TUint length = aText.Length();
    TCoeInputCapabilities::MCoeFepSpecificExtensions* ipCapExt = 
                           iInputCapabilities.FepSpecificExtensions(TUid::Uid(EAknFepUid));
    if (ipCapExt)
        {
        while(length--)
            {
            if (!ipCapExt->IsValidCharacter(aText[length]))
                {
                ret = EFalse;
                }
            }
        }
    return ret;
    }

TBool CAknFepManager::CharIsValidInEditor(TChar aChar)
    {
    TBool ret = ETrue;
    TCoeInputCapabilities::MCoeFepSpecificExtensions* ipCapExt = 
                           iInputCapabilities.FepSpecificExtensions(TUid::Uid(EAknFepUid));
    if (ipCapExt)
        {
        if (!ipCapExt->IsValidCharacter(aChar))
            {
            ret = EFalse;
            }
        }
    return ret;
    }

void CAknFepManager::SetCursorType(TChineseFepCursorType aType)
    {
    TBool setToTrue = ETrue;
    TTextCursor newCursor;

    newCursor.iType=TTextCursor::ETypeRectangle;
    if (aType == EPassive)
        {
        newCursor.iType=TTextCursor::ETypeHollowRectangle;
        }
    if ( IsFepAwareTextEditor() )
        {
        iInputCapabilities.FepAwareTextEditor()->Extension1()->SetCursorType(setToTrue, newCursor);
        }
    }

MAknEditingStateIndicator* CAknFepManager::EditingStateIndicator() const
    {
    MAknEditingStateIndicator* indicator = NULL;

    // First ask indicator from controls owning the editor.
    // For example CAknQueryControl and CAknSearchField implement
    // their own indicators.

    MObjectProvider* objectProvider = NULL;

    // First try through CAknEdwinState object.
    // It exists with CEikEdwin-derived editors.
    
		MCoeFepAwareTextEditor* editor = NULL;
		MCoeFepAwareTextEditor_Extension1* ext1 = NULL;
		CAknEdwinState* state = NULL;

    editor = iInputCapabilities.FepAwareTextEditor();
    
    if(editor)
    	ext1 = editor->Extension1() ;
    if(ext1)
      state = (CAknEdwinState*)ext1->State( KNullUid ) ;
      
    if ( IsFepAwareTextEditor() && ( editor  ) && ( ext1) && ( state ))
        {
        objectProvider = state->ObjectProvider();
        }

    // If not found, try to get object provider from input capabilities.
    // It is set there with CEikMfne-derived editors.

    if ( !objectProvider )
        {
        objectProvider = iInputCapabilities.ObjectProvider();
        }

    if ( objectProvider )
        {
        indicator = objectProvider->MopGetObject( indicator );
        }

    // If no indicator was supplied, get one from CAknEnv.
    // By default, this one is FEP's own indicator.

    if ( !indicator )
        {
        indicator = CAknEnv::Static()->EditingStateIndicator();
        if (indicator != iIndicator)
            {
            CAknEnv::Static()->SwapEditingStateIndicator(iIndicator);
            indicator = iIndicator;
            }
        }

    return indicator;
    }

TBool CAknFepManager::TryHandleArrowRightEventL(TInt aDokumentLength)
    {
    TBool ret = EFalse;
    TBool thai = (iLanguageCapabilities.iInputLanguageCode == ELangThai);
    if (  iInputCapabilities.FepAwareTextEditor() &&
          (iSharedDataInterface->SpaceWithScrollRight() || thai ) &&
          (!(iAknEditorFlags & EAknEditorFlagNoLRNavigation)) &&
          iFepManState == EAknFepStateInitial &&
          (!IsFlagSet(EFlagInsideInlineEditingTransaction) ) &&
          (!IsFlagSet(EFlagShiftKeyDepressed)) &&
          iMode != EHiragana )          //  Not supported in only Hiragana input mode
        {
        TInt cursorPos = DocPos().iPos;
        TCursorSelection cursorSel;
        iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(cursorSel);
        if (( cursorPos == aDokumentLength)
            && !cursorSel.Length()
            && (IsFeatureSupportedJapanese() || (PreviousChar() != 0x0020)) )
            {
            // set to half-width space or full-width space by current input mode.
            //    0x0020 is unicode for half-width space
            //    0x3000 is unicode for full-width space
            TUint code(0);
            switch (iMode)
                {
                case EHiraganaKanji:
                    code = 0x3000;
                    break;
                case ELatin:
                    code = (!WesternPredictive() && iCharWidth == EFullWidthChar)?
                            0x3000 : 0x0020;
                    break;
                case EHiragana:
                    // Not supported in only Hiragana input mode
                    break;
                case EHangul:
                	code = 0x0020;
                	break;
                case ENumber:
                case ENativeNumber:
                case EKatakana:
                default:
                    code = (iCharWidth == EFullWidthChar)? 0x3000 : 0x0020;
                    break;
                }
            if (code)
                {
                SimulateKeyEventL(code); // add a space
                SimulateKeyEventL(EKeyF19); // Asyncronous case update
                ret = ETrue;
                }
            }
        }
    return ret;
    }

TBool CAknFepManager::TryHandleArrowLeftEventL(TInt aDokumentLength)
    {
    TBool ret = EFalse;
    if (iInputCapabilities.FepAwareTextEditor() &&
        iSharedDataInterface->SpaceWithScrollRight() &&
        (!(iAknEditorFlags & EAknEditorFlagNoLRNavigation)) &&
        iFepManState == EAknFepStateInitial &&
        (!IsFlagSet(EFlagInsideInlineEditingTransaction) ) &&
        (!IsFlagSet(EFlagShiftKeyDepressed)) &&
        iMode != EHiragana )          //  Not supported in only Hiragana input mode
        {
        TInt cursorPos = DocPos().iPos;
        TCursorSelection cursorSel;
        iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(cursorSel);
        TText previousChar = PreviousChar();
        if (( cursorPos == aDokumentLength)
            && !cursorSel.Length()
            && (IsFeatureSupportedJapanese()
            && (previousChar == 0x3000 || previousChar == 0x0020) )) // full width or half-width space.
            {
            SimulateKeyEventL(EKeyBackspace);
            SimulateKeyEventL(EKeyF19); // Asyncronous case update
            ret = ETrue;
            }
        }
    return ret;
    }

TBool CAknFepManager::TryHandleArrowDownEventL(TInt aDokumentLength)
    {
    TBool ret = EFalse;
#ifdef RD_INTELLIGENT_TEXT_INPUT
    // Predictive QWERTY (XT9) changes ---->
    if ( iWesternPredictive &&
         IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction) && !(iAknEditorFlags & EAknEditorFlagNoT9) 
         && !IsFlagSet(EFlagInsideMultitapInlineEditingTransaction) && !IsFlagSet(EFlagNoMatches))
        {
        LaunchCandidatePopupListL();
        ret = ETrue;
        }
    // Predictive QWERTY (XT9) changes <----
    else 
#endif //RD_INTELLIGENT_TEXT_INPUT
    if  ( ( iAknEditorFlags & EAknEditorFlagAllowEntersWithScrollDown ) &&
          iInputCapabilities.FepAwareTextEditor() &&
          ( iSharedDataInterface->EntersWithScrollDown() ) &&
          iFepManState == EAknFepStateInitial &&
          !IsFlagSet(EFlagInsideInlineEditingTransaction) &&
          !IsFlagSet(EFlagShiftKeyDepressed) )
        {
        TInt cursorPos = DocPos().iPos;
        TCursorSelection cursorSel;
        iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(cursorSel);
        if ( cursorPos == aDokumentLength &&
            !cursorSel.Length() )
            {
            SimulateKeyEventL(EKeyEnter); // add line feed
            SimulateKeyEventL(EKeyF19); // Asyncronous case update
            ret = ETrue;
            }
        }
    return ret;
    }

TUint CAknFepManager::EditorModeFromFepMode(TInt aFepMode)
    {
    TUint editorMode(aFepMode);
    switch (aFepMode)
        {
        case ELatin:
            if (iPermittedInputModes & EAknEditorTextInputMode)
                {
                editorMode = EAknEditorTextInputMode;
                if (IsFeatureSupportedJapanese())
                    {
                    if (iCharWidth == EHalfWidthChar 
                    && (iPermittedInputModes & EAknEditorHalfWidthTextInputMode))
                        {
                        editorMode = EAknEditorHalfWidthTextInputMode;
                        }
                    else if (iCharWidth == EFullWidthChar 
                         && (iPermittedInputModes & EAknEditorFullWidthTextInputMode))
                        {
                        editorMode = EAknEditorFullWidthTextInputMode;
                        }
                    }
                }
            else if (iPermittedInputModes & EAknEditorHalfWidthTextInputMode)
                {
                editorMode = EAknEditorTextInputMode;
                if (IsFeatureSupportedJapanese())
                    {
                    editorMode = EAknEditorHalfWidthTextInputMode;
                    if (iCharWidth == EFullWidthChar 
                     &&(iPermittedInputModes & EAknEditorFullWidthTextInputMode))
                        {
                        editorMode = EAknEditorFullWidthTextInputMode;
                        }
                    }
                }
            else if (iPermittedInputModes & EAknEditorFullWidthTextInputMode)
                {
                editorMode = EAknEditorTextInputMode;
                if (IsFeatureSupportedJapanese())
                    {
                    editorMode = EAknEditorFullWidthTextInputMode;
                    }
                }
            else
                {
                editorMode = EAknEditorSecretAlphaInputMode;
                }
            break;
        case ENumber:
        case ENativeNumber:
            editorMode = EAknEditorNumericInputMode;
            if (iCharWidth == EFullWidthChar 
            && (iPermittedInputModes & EAknEditorFullWidthNumericInputMode))
                {
                editorMode = EAknEditorFullWidthNumericInputMode;
                }
            break;
    	case EHangul: 
       		editorMode = EAknEditorHangulInputMode;       	
       		break;
        default:
            if (IsFeatureSupportedJapanese())
                {
                editorMode = EditorMode(aFepMode, iCharWidth);
                }
            break;
        }
    return editorMode;
    }

CAknKeySoundSystem* CAknFepManager::SoundSystem() const
    {
    CCoeEnv* coeEnv = CCoeEnv::Static();
    if (coeEnv->AppUi())
        {
        return iAvkonAppUiBase->KeySounds();
        }
    return NULL;
    }

void CAknFepManager::SetHashKeyStyle()
    {
    CAknFepHashKeyManager::THashKeyStyle style = CAknFepHashKeyManager::EHashKeyStyleWestern;

    switch (iLanguageCapabilities.iInputLanguageCode)
        {
        case ELangPrcChinese:
        case ELangHongKongChinese:
        case ELangTaiwanChinese:
            style = CAknFepHashKeyManager::EHashKeyStyleChineseWithWestern;
            break;
        case ELangJapanese:
            style = CAknFepHashKeyManager::EHashKeyStyleJapanese;
            break;
        case ELangKorean:
        	style = CAknFepHashKeyManager::EHashKeyStyleKoreanWithWestern;
        	break;
        default:
            break;
        }

    iHashKeyMan->SetHashKeyStyle( style );
    }

void CAknFepManager::ReadHashKeyLoopL()
    {
    iHashKeyLoop.Reset();

    CCoeEnv* coeEnv = CCoeEnv::Static();
    TResourceReader reader;
    coeEnv->CreateResourceReaderLC(reader, R_HASH_KEY_LOOP_ALL_INPUT_MODES);

    iVariantPermittedModes = 0;
    const TInt count = reader.ReadInt16();
    for (TInt ii=0; ii<count; ii++)
        {
        TInt mode = reader.ReadInt32();
        if ( IsInputModeAvailable(mode) )
            {
            if ( mode == EPRCFind)
                {
                mode = ELatin;
                }
            iHashKeyLoop.Append(mode);
            iVariantPermittedModes |= mode;
            }
        }
    CleanupStack::PopAndDestroy(); //reader
    }

TBool CAknFepManager::IsValidInLineCharacter(TChar aCharacter) const
    {
    TBool validInlineCharacter = ETrue;
    TChar::TCategory category = aCharacter.GetCategory();
    TChar::TBdCategory bdCategory = aCharacter.GetBdCategory();
    switch (iLanguageCapabilities.iInputLanguageCode)
        {
        case ELangArabic:
            {
            // Needs more specific category
            validInlineCharacter = ( (category & TChar::ELetterOtherGroup) &&
                (bdCategory == TChar::ERightToLeftArabic) );
            break;
            }
        case ELangHebrew:
            {
            // Needs more specific category
            validInlineCharacter = ( (category & TChar::ELetterOtherGroup) &&
                (bdCategory == TChar::ERightToLeft) );
            break;
            }
        case ELangFarsi:
        case ELangUrdu:
            {
            // Needs more specific category
            validInlineCharacter = ( (category & TChar::ELetterOtherGroup) &&
                (bdCategory == TChar::ERightToLeftArabic) );
            break;
            }
            /*Hindi*/            
        case ELangHindi:
#ifdef RD_MARATHI
			/* Same for Marathi */
        case ELangMarathi:
#endif
            {
            // Needs more specific category
            validInlineCharacter = IsValidInlineIndicCharacter(aCharacter);
            
            break;
            }
        case ELangVietnamese:
            {
            // For full screen qwerty, validInlineCharacter is true
            if( !( iFepPluginManager && iFepPluginManager->EnableITIOnFSQ() ) )
                {
                validInlineCharacter = (aCharacter.IsAlpha() &&
                                        !((category & TChar::ELetterOtherGroup))) ||
                                        ((category == TChar::EMarkGroup) && 
                                        (bdCategory == TChar::ENonSpacingMark));
                }
            break;
            }
        case ELangThai:
            {
            validInlineCharacter = (!(aCharacter.IsSpace() || (aCharacter == 0x2029))) &&
                                   ((aCharacter.IsAlpha() && 
                                    !(category & TChar::ELetterOtherGroup)) ||
                                   ((category & TChar::EMarkGroup) && 
                                    (bdCategory == TChar::ENonSpacingMark)) ||
                                   ((category & TChar::ELetterModifierGroup) && 
                                    (bdCategory == TChar::ELeftToRight)) ||
                                   ((category & TChar::ESeparatorGroup ) && 
                                    (bdCategory == TChar::EWhitespace))||
                                   ((category & TChar::EMcCategory) && 
                                    (bdCategory == TChar::ELeftToRight)));
            break;                                    
            }
        case ELangTaiwanChinese:
        case ELangHongKongChinese:
        case ELangPrcChinese:
        case ELangEnglish:
            {                           
#ifdef RD_INTELLIGENT_TEXT_INPUT   
            // for qwerty special characters should not break the word
            if( iQwertyInputMode && ( KeyboardLayout() !=EPtiKeyboardHalfQwerty ))
                {
                validInlineCharacter = (!( aCharacter.IsSpace() || (aCharacter == 0x2029) || 
                                        ( aCharacter.IsAlpha() && 
                                         (category & TChar::ELetterOtherGroup))||
                                        ((category & TChar::EMarkGroup) && 
                                         (bdCategory == TChar::ENonSpacingMark)) ||
                        				((category & TChar::ELetterModifierGroup) && 
                        		 		 (bdCategory == TChar::ELeftToRight)) ||
                        				((category & TChar::ESeparatorGroup ) && 
                        		 		 (bdCategory == TChar::EWhitespace))));
                }            
            else                
#endif      // whereas for ITUT and Half Qwerty they should
                {
                validInlineCharacter = ( aCharacter.IsAlpha() && !(category & TChar::ELetterOtherGroup) );
                }            
            break;
            }
        default:
            {
#ifdef RD_INTELLIGENT_TEXT_INPUT            
			// for qwerty special characters should not break the word
			
			if( iQwertyInputMode && ( KeyboardLayout() !=EPtiKeyboardHalfQwerty ) )
				{
				validInlineCharacter = (!(aCharacter.IsSpace() || (aCharacter == 0x2029)));	
				}
			else	            
#endif		// whereas for ITUT and Half Qwerty they should
				{
				validInlineCharacter = ( aCharacter.IsAlpha() && !(category & TChar::ELetterOtherGroup) );
				}
			

            break;
            }
        }
	
	TUint16 key = 0;
	if(validInlineCharacter)
		{
		validInlineCharacter = EFalse;
        ConvertCharToKey(aCharacter, key);
		if(0 != key)
			{
			validInlineCharacter = ETrue;
			}
		}
		
    return validInlineCharacter;
    }
TBool CAknFepManager::IsValidInlineIndicCharacter(TChar aCharacter) const
    {
    TBool validInlineCharacter = ETrue;
    TChar::TCategory category = aCharacter.GetCategory();
    TChar::TBdCategory bdCategory = aCharacter.GetBdCategory();
    
    if(!((category == TChar::EPoCategory) || aCharacter == 0x0950 || aCharacter == 0x093D))
        {
        validInlineCharacter = ((( (category & TChar::ELetterOtherGroup) 
                                      && (bdCategory == TChar::ELeftToRight) )
                                  ||( (category & TChar::EMcCategory) 
                                      && (bdCategory == TChar::ELeftToRight) )
                                  ||( (category & TChar::EMarkGroup) 
                                      && (bdCategory == TChar::ENonSpacingMark)) )
                                 &&( ! ( aCharacter.IsAlpha() 
                                      && !(category & TChar::ELetterOtherGroup) )));
        }
    else 
        {
        validInlineCharacter = EFalse;
        }
    return  validInlineCharacter;
    }

TInt CAknFepManager::WordConcatenationTimerTimeoutCallback(TAny* aObj)
    {
    TRAPD(err, static_cast<CAknFepManager*>(aObj)->WordConcatenationTimerTimeoutL());
    if (err)
        {
        static_cast<CAknFepManager*>(aObj)->CleanUpFep();
        return KErrNoMemory;
        }
    return KErrNone;
    }

#ifdef RD_INTELLIGENT_TEXT_INPUT
    
TBool CAknFepManager::IsHybridAplhaEditor() const
    {
    TBool phoneIdle = (EditorType() == CAknExtendedInputCapabilities::EHybridAlphaNumericEditor);
    return phoneIdle;
    }

TBool CAknFepManager::IsHybridAlphaModeChangedtoAplhanumeric() const
    {
    return iHybridAplphaChangedToAlphanumeric;
    }

TBool CAknFepManager::IsPhoneNumberEditor() const
    {
    TBool phoneEditor = (EditorType() == CAknExtendedInputCapabilities::EPhoneNumberEditor);
    return phoneEditor;
    }
CAknFepFnKeyManager::TFnKeyState CAknFepManager::FnKeyState()
    {
    return iFnKeyManager->FnKeyState();
    }
void CAknFepManager::SetFnKeyState(CAknFepFnKeyManager::TFnKeyState aState)
    {
    iFnKeyManager->SetFnKeyState(aState);
    }
#ifdef __REVERSE_FN_KEY_SUPPORTED
TBool CAknFepManager::IsReverseFnkeyInput()
    {
    return iIsReverseFnkeyInput;
    }
void CAknFepManager::SetReverseFnkeyInputMode(TBool iIsOn)
	{
	iIsReverseFnkeyInput = iIsOn;
	}
TBool CAknFepManager::IsValidCharInNumericEditorL( TChar aChar ) const
    {
    if(!EditorState())
        {
    	return EFalse;    	
        }
    TBool validChar = EFalse;
    if( aChar.IsDigit() )
        {
        return ETrue;
        }
    TBool phoneIdle = (EditorType() == CAknExtendedInputCapabilities::EPhoneNumberEditor);            
    if( (phoneIdle || (IsHybridAplhaEditor() && !IsHybridAlphaModeChangedtoAplhanumeric()))
	 	&& aChar == '#')    
    	{
    	return ETrue;
    	}
	if( aChar == '.' && EAknEditorConverterNumberModeKeymap == iAknEditorNumericKeymap)
        {
        return ETrue;
        }	
    if( IsAllowedKeymappingForNumberMode(aChar) )
    	{
    	return ETrue;
    	}
    	
    TInt resourceSCTId = EditorState()->SpecialCharacterTableResourceId();  
    // No SCT table use default SCT
    if( resourceSCTId == KErrNotFound || !resourceSCTId || 
            (IsHybridAplhaEditor()&& !IsHybridAlphaModeChangedtoAplhanumeric()))
        {
        resourceSCTId = NumericModeSCTResourceId();
        }   
    
    if( resourceSCTId )
        {
        TResourceReader reader;
        CEikonEnv::Static()->CreateResourceReaderLC(reader,resourceSCTId);
        reader.ReadInt32(); // skip sct id    
        TPtrC chars=reader.ReadTPtrC(); // read special char        
        validChar = ( chars.Locate(aChar)>=0 );                      
        CleanupStack::PopAndDestroy();
        }
    return validChar;
    
    }
TInt CAknFepManager::GetNumericSCTResID()
    {
    if(!EditorState())
      	{
        return NumericModeSCTResourceId();
    	  }
    TInt resourceSCTId = EditorState()->SpecialCharacterTableResourceId();
    if(resourceSCTId == KErrNotFound || !resourceSCTId )
        {
        resourceSCTId = NumericModeSCTResourceId();
        }
    return resourceSCTId;
    }

TBool CAknFepManager::IsValidCharForNumericInFnReverseL(TInt aKey, TPtiTextCase aCase) 
    {
    if(!EditorState())
        {
    	return EFalse;    	
        }
    TBuf<32> keyMaps;
    iPtiEngine->MappingDataForKey( (TPtiKey)aKey, keyMaps, aCase);  
    
    TBool validKey(EFalse);   
    if( keyMaps.Length()<= 0 )
        {
        return validKey;
        }
    TChar chr =keyMaps[0];
    if( chr.IsDigit() )
        {
        return ETrue;
        }

    TInt resourceSCTId = EditorState()->SpecialCharacterTableResourceId();  
    // No SCT table use default SCT
    if( resourceSCTId == KErrNotFound || !resourceSCTId )
        {
        resourceSCTId = NumericModeSCTResourceId();
        }   
    
    if( resourceSCTId )
        {
        TResourceReader reader;
        CEikonEnv::Static()->CreateResourceReaderLC(reader,resourceSCTId);
        reader.ReadInt32(); // skip sct id    
        TPtrC chars=reader.ReadTPtrC(); // read special char
        
        validKey = (chars.Locate(chr)>=0 );
        if(chr.IsAlpha())
            {
            chr.UpperCase();
            validKey |= (chars.Locate(chr)>=0 ) ;
            }                 
            
        CleanupStack::PopAndDestroy();
        }
         
    return validKey;
    
    }
#endif //__REVERSE_FN_KEY_SUPPORTED
void CAknFepManager::AddOneSpaceOrMoveCursorL()
	{
	if(static_cast<TChar>(NextChar()).IsSpace())
		{
		TCursorSelection sel;
		iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(sel);
		sel.iCursorPos++;
		sel.iAnchorPos++;
		iInputCapabilities.FepAwareTextEditor()->SetCursorSelectionForFepL(sel);	                		
		}
	else if(IsEditorHasFreeSpace())
    	{
        SimulateKeyEventL(EKeySpace);
    	}
	SimulateKeyEventL(EKeyF19);
	}
#endif
TBool CAknFepManager::IsOnlyNumericPermitted() const
    {
    if (IsFeatureSupportedJapanese())
        {
        return !(iPermittedInputModes & ~(EAknEditorNumericInputMode 
               | EAknEditorFullWidthNumericInputMode) );
        }
    else
        {
        // Only non-japanese modes are considered.
        return !( iPermittedInputModes & 
                  (EAknEditorTextInputMode | EAknEditorSecretAlphaInputMode) );
        }
    }

void CAknFepManager::TryIncrementModeL(TInt aCurrentMode)
    {
    //check that we are currently in the correct mode
    __ASSERT_DEBUG(IsModePermitted(iMode),
                   AknFepPanic(EAknFepPanicNonPermittedEditorMode));
    __ASSERT_DEBUG(iInputCapabilities.FepAwareTextEditor(),
                   AknFepPanic(EAknFepPanicNoHandleToFepAwareTextEditor));

    const TInt lastModePos = iHashKeyLoop.Count()-1 ;
    const TInt currentModePos = iHashKeyLoop.Find(aCurrentMode);
    TInt newModePos = currentModePos;

    __ASSERT_DEBUG(currentModePos != KErrNotFound ,
                   AknFepPanic(EAknFepPanicNonPermittedEditorMode));

    do  {
        if (newModePos == lastModePos)
            {
            //we've got to the end of the hash key loop, go back to the beginning
            newModePos = 0;
            }
        else
            {
            newModePos++;
            }

        if (TryChangeModeL(iHashKeyLoop[newModePos]))
            {
            return; //succeeded to move to next available mode
            }
        }
        while (newModePos != currentModePos);
    }

void CAknFepManager::TryIncrementModeChineseQwertyL(TInt aCurrentMode)
    {
    __ASSERT_DEBUG(IsModePermitted(aCurrentMode),AknFepPanic(EAknFepPanicNonPermittedEditorMode));

    const TInt lastModePos = iHashKeyLoop.Count() - 1 ;
    const TInt currentModePos = iHashKeyLoop.Find(aCurrentMode);
    TInt newModePos = currentModePos;

    __ASSERT_DEBUG(currentModePos != KErrNotFound ,
                   AknFepPanic(EAknFepPanicNonPermittedEditorMode));

    do  
        {
        if (newModePos == lastModePos)
            {
            //we've got to the end of the hash key loop, go back to the beginning
            newModePos = 0;
            }
        else
            {
            newModePos++;
            }
        }while(iHashKeyLoop[newModePos] == ENumber && !IsOnlyNumericPermitted()); // no number mode in Chinese qwerty input, except those editors which only support number
            
    SendEventsToPluginManL( EPluginCloseMode, EFalse );
    if(TryChangeModeL(iHashKeyLoop[newModePos]))
        {
        return; //succeeded to move to next available mode
        }
    }

void CAknFepManager::TryIncrementChineseModeForQwertyL( TInt aCurrentMode )
    {
    __ASSERT_DEBUG(IsModePermitted(aCurrentMode),AknFepPanic(EAknFepPanicNonPermittedEditorMode));

    const TInt lastModePos = iHashKeyLoop.Count() - 1;
    const TInt currentModePos = iHashKeyLoop.Find( aCurrentMode );
    TInt newModePos = currentModePos;

    __ASSERT_DEBUG(currentModePos != KErrNotFound ,
        AknFepPanic(EAknFepPanicNonPermittedEditorMode));

    do
        {
        if ( newModePos == lastModePos )
            {
            //we've got to the end of the hash key loop, go back to the beginning
            newModePos = 0;
            }
        else
            {
            newModePos++;
            }
        }
    while ( (iHashKeyLoop[newModePos] == ENumber
        && !IsOnlyNumericPermitted()) || (iHashKeyLoop[newModePos] == ELatin
        && IsChangeModeByShiftAndSpace() ) ); // no number mode and latin in Chinese qwerty input, except those editors which only support number

    SendEventsToPluginManL( EPluginCloseMode, EFalse );
    if ( TryChangeModeL( iHashKeyLoop[newModePos] ) )
        {
        return; //succeeded to move to next available mode
        }
    }

void CAknFepManager::TryChangeToSharedDataModeL()
    {
    TInt sharedDataMode = iSharedDataInterface->InputMode();
    TryChangeModeL(sharedDataMode);
    }

void CAknFepManager::TryChangeToModeBeforeL()
    {
    if (TryChangeModeL(iModeBefore))
        {
        if (iModeBefore==ELatin)
            {
            if ( iCaseBefore )
                {
                iCaseMan->SetCurrentCase(iCaseBefore);
                }
            else
                {
                iCaseMan->UpdateCase(ENullNaviEvent);
                }
            }
        }
    }

#ifdef RD_HINDI_PHONETIC_INPUT    

TBool CAknFepManager::IsIndicPhoneticInputLanguage() const
    {
    MPtiLanguage* ptilang = iPtiEngine->CurrentLanguage();
    TInt currentPtiLang = (ptilang)? ptilang->LanguageCode() : 0;
    return (TAknFepUiIndicInputManager::IsIndicPhoneticLanguage(TLanguage(currentPtiLang)));
    }

void CAknFepManager::TryChangePhoneticModeL()
    {
    MPtiLanguage* ptilang = iPtiEngine->CurrentLanguage();
    TInt currentPtiLang = (ptilang)? ptilang->LanguageCode() : 0;
    TInt lang = 0;
    
    (TAknFepUiIndicInputManager :: IsIndicPhoneticLanguage(TLanguage(currentPtiLang)))? 
    lang = ELangEnglish : lang = iLanguageCapabilities.iInputLanguageCode;
    
    iPtiEngine->ActivateLanguageL(lang);
    iLangMan->SetInputLanguageL(lang);
    InternalFepUI()->SetMode(iMode, EFalse, iQwertyInputMode);  
    iCaseMan->UpdateCase(ENullNaviEvent);
    UpdateIndicators();
    CommitInlineEditL();
    }
    
TInt CAknFepManager::SetPhoneticIndicator(TLanguage aInputLanguage)
    {
    TInt newState = 0;
    TInt currentCase = iCaseMan->CurrentCase();

    switch(aInputLanguage)
        {
        case KLangHindiPhonetic: 
            if(currentCase == EAknEditorUpperCase)  
                {
                newState = EIndicatorStateHindiPhoneticUpper;   
                }
            else
                {
                newState = EIndicatorStateHindiPhoneticLower;   
                }
        break;
        default:
        break;
        }
    return newState;        
    }
#endif
TBool CAknFepManager::TryChangeModeL(TInt aMode)
    {
    CAknEdwinState* editorState = EditorState();
    TInt sharedDataMode = iSharedDataInterface->InputMode();

    if (aMode == EStroke && iQwertyInputMode && sharedDataMode == ECangJie)
    	{
    	iEditorStateStrokeUsed = editorState;
    	iStrokeUsedInQWERTY = ETrue;
    	}
    else if (iEditorStateStrokeUsed == editorState)
    	{
    	iStrokeUsedInQWERTY = EFalse;
    	}
    
    if (aMode == ECangJie && !iQwertyInputMode && 
    		(iFepPluginManager->PluginInputMode() != EPluginInputModeVkb
			|| iFepPluginManager->PluginInputMode() != EPluginInputModeFSQ))
        {
        aMode = EStroke;
        }
    
    if (aMode == ELatinText ||aMode == ELatinUpper || aMode == ELatinLower)
        {
        if (TryChangeModeL(ELatin))
            {
            if ( !(iAknEditorFlags & EAknEditorFlagFixedCase) )
                {
                if (aMode == ELatinText)
                    {
                    ClearFlag(EFlagSupressAutoUpdate);
                    }
                else if (aMode == ELatinUpper)
                    {
                    //Modify for allow text case after entering a dot and space
                    ClearFlag(EFlagSupressAutoUpdate);
                    //Modify for allow text case after entering a dot and space
                    iCaseMan->SetCurrentCase(EAknEditorUpperCase);
                    }
                else //lower
                    {
                    //Modify for allow text case after entering a dot and space
                    ClearFlag(EFlagSupressAutoUpdate);
                    //Modify for allow text case after entering a dot and space
                    iCaseMan->SetCurrentCase(EAknEditorLowerCase);
                    }
                UpdateIndicators();
                }
            return ETrue;
            }
        return EFalse;
        }

    if ( iQwertyInputMode && ( aMode == ENumber || aMode == ENativeNumber ) && 
         !IsOnlyNumericPermitted() 
         // Add this condition to exclude all touch inputmethods. 
         // When change to number range on touch inputmethods, fn key state can't be locked.          
         && iFepPluginManager 
         && iFepPluginManager->PluginInputMode() == EPluginInputModeNone )
        {
        if (TryChangeModeL(ELatin))
            {
#ifdef RD_INTELLIGENT_TEXT_INPUT
            // If editor should be launched in FnLock state, because in case of QWERTY keypad
            // Current numeric mode converted to alphanumeric mode.
            if( iFnKeyManager )
                {
                iFnKeyManager->SetFnKeyState(CAknFepFnKeyManager::EFnKeyLock);
                SetCase((TCase)EFnKeyLowerCase);
                }
#endif // RD_INTELLIGENT_TEXT_INPUT
            return ETrue;
            }
        }

    if (IsModePermitted(aMode))
        {
        if (aMode == ELatin)
            {
            if (iAknEditorFlags & EAknEditorFlagNoT9)
                {
                SetWesternPredictive(EFalse);
                }

            if (iAknEditorFlags & EAknEditorFlagFixedCase)
                {
                iCaseMan->ConfigureCaseStateFromEditorState();
                }
            }
#ifdef RD_INTELLIGENT_TEXT_INPUT        
        if (!iQwertyInputMode && WesternPredictive() && iFepManState == EAknFepStateUIActive)
#else
        if (WesternPredictive() && iFepManState == EAknFepStateUIActive)
#endif            
            {
            TryRemoveNoMatchesIndicatorL();
            UpdateCbaL(NULL);
            }
        TryCloseUiL();

        // getting a new ui manager object corresponded in aMode.
        MAknFepManagerInterface* fepUI = iLangMan->GetFepUI(aMode, iCharWidth, 
                                                            IsPredictive(aMode));
        // getting the currect language id from ptiengine.
        MPtiLanguage* ptilang = iPtiEngine->CurrentLanguage();
        TInt currentPtiLang = (ptilang)? ptilang->LanguageCode() : 0;
        // getting the supported language id in current ui manager.
        TInt currentUiLang = fepUI->SupportLanguage(aMode);

        // changing language id both ptiengine and ui manager
        // 1. check the currect ui manager for input mode
        // 2. check the current ptiengine and ui manager
        if (iCurrentFepUI != fepUI
         || currentPtiLang != currentUiLang)
            {
            iCurrentFepUI = fepUI;
            TInt err = iPtiEngine->ActivateLanguageL(currentUiLang);
            if (err != KErrNone)
                {
#ifdef _DEBUG
                RDebug::Print(_L("ActivateLanguageL error return code=[%d] language id=[%d]"),
                                err, iLanguageCapabilities.iInputLanguageCode);
#endif
                ChangeInputLanguageL(ELangEnglish);
                return ETrue;
                }
            }
#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifdef FF_DUAL_LANGUAGE_SUPPORT
        // Change secondary input
        iPtiEngine->SetSecondaryInputL(iSharedDataInterface->InputTextLanguageSecondary());
#endif //FF_DUAL_LANGUAGE_SUPPORT
#endif

//#ifndef RD_INTELLIGENT_TEXT_INPUT 
            TBuf<50> mapData;
            //Init Data
            for (TInt i = 0; i < 50; i++)
                {
                mapData.Append(0);
                }

            //Backup original input mode
            TPtiEngineInputMode oriMode = iPtiEngine->InputMode();
            
            //Change current input mode, since MappingDataForKey I/F only worked
            //when in EPtiEngineMultitapping mode
            iPtiEngine->SetInputMode(EPtiEngineMultitapping);
            iPtiEngine->MappingDataForKey(EPtiKey1, mapData, iPtiEngine->Case());
            
            //Restore
            iPtiEngine->SetInputMode(oriMode);

            //Get first three symbols
			if(mapData.Length() > 0)
				{
	        	iSymbolData.iSymbol1 = mapData[0];
	            
				if (mapData.Length() > 1)
	            	iSymbolData.iSymbol2 = mapData[1];
	        	if (mapData.Length() > 2)
	        		iSymbolData.iSymbol3 = mapData[2];
				}
            
            //Write data to stream
            if (iFepPluginManager && iFepPluginManager->CurrentPluginInputFepUI())
                {
                iFepPluginManager->CurrentPluginInputFepUI()->HandleCommandL(
                            ECmdPenInputSymbolOfHardKeyOne, reinterpret_cast<TInt>(&iSymbolData));
                }
//#endif                
        iMode = aMode;
        
#ifdef RD_SCALABLE_UI_V2 
        if ( IsChineseInputMode( aMode ) )
            {
            iLastChineseInputMode = aMode;
            }        
#endif // RD_SCALABLE_UI_V2
        if(iMode == ENumber || iMode == ENativeNumber )
            {
#ifdef RD_INTELLIGENT_TEXT_INPUT 
            ClearFlag(CAknFepManager::EFlagNewSharedDataInputLanguage);
            ClearFlag(CAknFepManager::EFlagNewSharedDataInputMode);
#endif //  RD_INTELLIGENT_TEXT_INPUT  
            InternalFepUI()->SetNumberModeKeyMappingL(iAknEditorNumericKeymap);
            }
#ifdef RD_INTELLIGENT_TEXT_INPUT            
        InternalFepUI()->SetMode(iMode, IsPredictive(), iQwertyInputMode, KeyboardLayout());
#else
        InternalFepUI()->SetMode(iMode, IsPredictive(), iQwertyInputMode);
#endif        
        iHashKeyMan->SetMode(iMode, IsPredictive());
        // ensure editor is aware of new fep mode
        TCursorSelection curSel;
        if ( IsFepAwareTextEditor() )
            {
            EditorState()->SetCurrentInputMode(EditorModeFromFepMode(aMode));
            iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(curSel);
            AdjustCursorTypeForCurrentPosition();
            }

        // when cursor is in between two words and SCT is launched and
        // cancel key is pressed, dont move the cursor at the end of
        // second word.     
         if (WesternPredictive(aMode) && !curSel.Length()
            && CursorInsideWord() && !IsCcpuFlagSet(ECcpuStateSupressCursorMoveToEnd)
#ifdef RD_INTELLIGENT_TEXT_INPUT            
            && (!iSupressCursorMoveToEndChrKeyPressed)
#endif //RD_INTELLIGENT_TEXT_INPUT       
            )
            {
            MoveCursorToEndOfWordL();
            }
        else
            {
#ifdef RD_INTELLIGENT_TEXT_INPUT              
            iSupressCursorMoveToEndChrKeyPressed = EFalse;
#endif //RD_INTELLIGENT_TEXT_INPUT             
            ClearCcpuFlag(ECcpuStateSupressCursorMoveToEnd);
            }

#ifdef RD_INTELLIGENT_TEXT_INPUT
        // Ensure that autocompletion state is updated
        if (WesternPredictive())
        	{
        	SetAutoCompletionState(iSharedDataInterface->PredictiveTextAutoCompleteOn());
        	}
#endif
        // Set multitap timer to PtiEngine for Japanese variant only
        if (IsFeatureSupportedJapanese())
            {
            TInt timeout = (iInputCapabilities.SupportsSecretText())? 
            					KSyncPasswdTimeout : iMultiTapTimer;
            iPtiEngine->HandleCommandL(EPtiCommandSetMultitapTimer, &timeout);
            iPtiEngine->HandleCommandL(EPtiCommandSetJapaneseQwertyFlags, &iJapaneseQwertyFlags);
            TInt clear = iSharedDataInterface->ClearDirection();
            iPtiEngine->HandleCommandL(EPtiCommandSetClearFunction, &clear);
            // Send PtiEngine the command whether Pictograph is allowed in Candidate list
            TInt allow = IsAbleToLaunchPCT();
#ifndef RD_INTELLIGENT_TEXT_INPUT
            iPtiEngine->HandleCommandL(EPtiCommandAllowPictographCandidate, &allow);
#endif //RD_INTELLIGENT_TEXT_INPUT
            }

        UpdateIndicators(); // moved to bottom to avoid extra indicator updates

        return ETrue;
        }
    return EFalse;
    }

void CAknFepManager::ChangeInputLanguageL(TInt aInputLanguage)
    {
    	if(iLangMan)
    	{
    	iLangMan->SetInputLanguageL(aInputLanguage);

    	SetInputLanguageCapabilities(aInputLanguage);

    	ReadHashKeyLoopL();
		// we need to make sure that we commit the word to ptiengine, 
		// so that the word gets added to the UWD 
    	SetCcpuFlag(ECcpuStataCommitPredictiveWord);
    	// Addition for ITI on FSQ
    	// When FSQ is opened with the ITI-supported input language, 
    	// if switch to another language which doesn't support ITI, such as Korean,
    	// need to restore the previous configuration on FEP
        if ( iFepPluginManager && !iFepPluginManager->IsSupportITIOnFSQ() )
        	{    	
        	iFepPluginManager->ResetItiStateL();
        	}    	
    	if ( !TryChangeModeL(NewInputModeAfterLanguageChange()) )
	        {
	        if ( !TryChangeModeL(ELatin) )
	            {
	            // If the editor does not support text input mode, we try number mode.
	            if( IsInputModeAvailable(ENativeNumber) && 
	                ( iLanguageCapabilities.iLocalDigitType == EDigitTypeArabicIndic ||
	                  iLanguageCapabilities.iLocalDigitType == EDigitTypeEasternArabicIndic ||
	                  iLanguageCapabilities.iLocalDigitType == EDigitTypeDevanagari ) )
	                {
	                TryChangeModeL(ENativeNumber);;
	                }
	            else
	                {
	                TryChangeModeL(ENumber);
	                }
	            }
	        }
  
        if (!iLanguageCapabilities.iLocalInputLanguageInUse)
            {
            switch (iSharedDataInterface->InputTextLanguage())
                {
                case ELangPrcChinese:
                    {
                    iSharedDataInterface->SetInputMode(EPinyin);
                    SetFlag(EFlagNewSharedDataInputMode);
                    break;
                    }
            	case ELangTaiwanChinese:
                    {
                    iSharedDataInterface->SetInputMode(EZhuyin);
                    SetFlag(EFlagNewSharedDataInputMode);
                    break;
                    }
                case ELangHongKongChinese:
                    {
                    iSharedDataInterface->SetInputMode(EStroke);
                    SetFlag(EFlagNewSharedDataInputMode);
                    break;
                    }
                default:
                    break;
                }
            }

    	SetHashKeyStyle();
    	UpdateEditorContext();
  		}
    //add notify to phrase creation udbmanagement view
    
    if(iUserdbdlg)
	    {
	    iUserdbdlg->OnChangeLanguageL( aInputLanguage );	
	    if ( aInputLanguage != ELangPrcChinese &&
	        	 aInputLanguage != ELangTaiwanChinese &&
	        	 aInputLanguage != ELangHongKongChinese )
	    	{
	    	iUserdbdlg = NULL;
	    	}
	    }
    }

void CAknFepManager::RemovePreviousCharacterL()
    {
    if (IsFlagSet(EFlagInsideInlineEditingTransaction))
        {
        CommitInlineEditL();
        }

    // Set cursor span on previous character
    iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(iUncommittedText);
    if (iUncommittedText.iAnchorPos > 0)
        {
        iUncommittedText.iAnchorPos--;
        }
    

    // get text to replace
    TBuf<ESingleCharacter> charToReplace;
    iInputCapabilities.FepAwareTextEditor()->GetEditorContentForFep(charToReplace, 
                                                                    iUncommittedText.iAnchorPos, 
                                                                    ESingleCharacter);

    // delete the character with an inline edit
    StartInlineEditL(iUncommittedText, charToReplace, ESingleCharacter, EFalse);
    UpdateInlineEditL(KNullDesC, 0);
    CommitInlineEditL();
    }

void CAknFepManager::RemoveZWSCharacterL( TBool aIsViramaInputted, 
                                            TBool aIsInMultitappingHalant, 
                                            TBool aIsCharModifier, 
                                            TBool aIsLigaturePresent /*= EFalse*/)
    {
    CTextLayout* textLayout = TextLayout();
    MAknFepDocumentNavigation* docNavi = AknFepDocumentNavigation();
    if(!textLayout && !docNavi)
        return;

    if( aIsLigaturePresent )
        {
        TTmDocPosSpec pos = DocPos();
        TTmPosInfo2 info;
        TTmLineInfo lineInfo;
        if (docNavi)
        	{
        	docNavi->FindDocPos(pos, info, lineInfo);
        	}
        else
        	{
        	textLayout->TagmaTextLayout().FindDocPos(pos, info, lineInfo);      
        	}

        if( IsZWSCharacterPresent( ETrue ) )
            {
            TInt commitPoint = pos.iPos - 2;
            if( IsFlagSet( EFlagInsideInlineEditingTransaction ) )
                {
                CommitInlineEditL();
                }

            TCursorSelection sel( commitPoint, commitPoint );
            iInputCapabilities.FepAwareTextEditor()->SetCursorSelectionForFepL(sel);
            }
        }
    else
        {
        if ( IsFlagSet(EFlagInsideInlineEditingTransaction) && IsZWSCharacterPresent())
            {
            CommitInlineEditL();
            }
        }

    TTmDocPosSpec pos = DocPos();
    pos.iType = TTmDocPosSpec::ETrailing;
    TTmPosInfo2 info;
    TTmLineInfo lineInfo;
    if ( ! iInputCapabilities.SupportsSecretText() )
        {
        if (docNavi)
        	{
        	docNavi->FindDocPos(pos, info, lineInfo);
        	}
        else
        	{
        	textLayout->TagmaTextLayout().FindDocPos(pos, info, lineInfo);
        	}
        TBuf<2> previousCharacter;
        TBuf<1> zwsPrevChar;
        previousCharacter.FillZ();
        zwsPrevChar.FillZ();
        TBool found = EFalse;

        TCursorSelection curSel;
        if ( IsFepAwareTextEditor() )
            {
            iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(curSel);
            TInt lowerpos = curSel.LowerPos();
            if( lowerpos > 0)
                {
                if(lowerpos > 1)
                iInputCapabilities.FepAwareTextEditor()->
                    GetEditorContentForFep(previousCharacter, lowerpos - 2, 2);
                else
                iInputCapabilities.FepAwareTextEditor()->
                    GetEditorContentForFep(previousCharacter, lowerpos - 1, 1);
                    
                if( lowerpos > 1 )
                    {
                    if(previousCharacter[1] == ZERO_WIDTH_SPACE)
                        {
                        zwsPrevChar.Append(previousCharacter[1]);
                        if( aIsInMultitappingHalant )
                            {
                            curSel.iAnchorPos = lowerpos - 2;
                            }
                        else
                            {
                            curSel.iAnchorPos = lowerpos - 1;
                            }
                        found = ETrue;      
                        }
                    else if( previousCharacter[0] == ZERO_WIDTH_SPACE )
                        {
                        zwsPrevChar.Append(previousCharacter[0]);
                        curSel.iAnchorPos = lowerpos - 2;
                        curSel.iCursorPos = lowerpos - 1;                       
                        found = ETrue;
                        }   
                    }
                else
                    {
                    if(previousCharacter[0] == ZERO_WIDTH_SPACE)
                        {
                        zwsPrevChar.Append(previousCharacter[0]);
                        if( aIsInMultitappingHalant )
                            {
                            curSel.iAnchorPos = lowerpos - 2;
                            }
                        else
                            {
                            curSel.iAnchorPos = lowerpos - 1;
                            }
                        found = ETrue;    
                        }   
                    }    
                
                }
            }
        if( found )
            {
            StartInlineEditL(curSel, zwsPrevChar, ESingleCharacter, ETrue);
            UpdateInlineEditL(KNullDesC, 0);
            CommitInlineEditL();    
            if ( !aIsInMultitappingHalant && ! iInputCapabilities.SupportsSecretText() )
                {
                if ( aIsViramaInputted )
                    {
                    TTmDocPosSpec pos = DocPos();
                    TTmPosInfo2 infotemp;
                    if (docNavi)
                    	{
                    	docNavi->FindDocPos(pos, info, lineInfo);      
                    	}
                    else
                    	{
                    	textLayout->TagmaTextLayout().FindDocPos(pos, info, lineInfo);      
                    	}
                    if(NextChar() && TAknFepUiIndicInputManager::IsCharBaseConsonant(NextChar(), 
                                           TLanguage(iLanguageCapabilities.iInputLanguageCode)))
	                    {
	                    TInt nextPos;                  
	                    if (docNavi)
	                    	{
	                    	nextPos = docNavi->FindNextPos(pos.iPos);
	                    	}
	                    else
	                    	{
		                    nextPos = textLayout->TagmaTextLayout().FindNextPos(pos.iPos);                 
	                    	}
	                    info.iDocPos.iPos  = nextPos;   
	                    }
                    else if((TAknFepUiIndicInputManager::IsCharAnVowel(NextChar(), 
                             TLanguage(iLanguageCapabilities.iInputLanguageCode)))
                         &&(aIsCharModifier && TAknFepUiIndicInputManager::IsCharVirama(
                             PreviousChar(), TLanguage(iLanguageCapabilities.iInputLanguageCode))))
                        {
                        bool posFound;
		                if (docNavi)
		                	{
		                	posFound = docNavi->GetNextVisualCursorPos(pos, infotemp, EFalse);      
		                	}
		                else
		                	{
		                	posFound = textLayout->TagmaTextLayout().GetNextVisualCursorPos(pos, 
                                                                                      infotemp, 
                                                                                      EFalse);      
		                	}
                        
                    	if (posFound)
                            {
                            info.iDocPos.iPos = infotemp.iDocPos.iPos;
                            }
                        }
                    TCursorSelection sel(info.iDocPos.iPos ,info.iDocPos.iPos );
                    iInputCapabilities.FepAwareTextEditor()->SetCursorSelectionForFepL(sel);    
                    }
                else
                    {
                    TCursorSelection sel(info.iDocPos.iPos - 1,info.iDocPos.iPos - 1);
                    iInputCapabilities.FepAwareTextEditor()->SetCursorSelectionForFepL(sel);        
                    }   
                
                SendEventsToPluginManL( EPluginSyncFepAwareText );
                }
            }
        }
    }

void  CAknFepManager::TryChangePredictiveInputModeL(TBool aFlag)
    {
    if(Japanese() && iMode != ELatin)
        {
        if(aFlag)
            {
            if (!(iAknEditorFlags & EAknEditorFlagNoT9)
               && HasJapanesePredictionInputMode())
                {
                iJapanesePredictive = aFlag;
                iSharedDataInterface->SetJapanesePredictiveTextOn();
                }
            }
        else
            {
            iJapanesePredictive = aFlag;
            iSharedDataInterface->ResetJapanesePredictiveTextOn();
            }
        }
    else
        {
        SetWesternPredictive(aFlag);
        }
    TryChangeModeL(iMode);
    }

MCoeFepAwareTextEditor* CAknFepManager::FepAwareTextEditor() const
    {
    return iInputCapabilities.FepAwareTextEditor();
    }

TBool CAknFepManager::IsFepAwareTextEditor() const
    {
#ifdef RD_SCALABLE_UI_V2   
    return FullyFepAwareTextEditor();
#else
		return ( iInputCapabilities.FepAwareTextEditor() ? ETrue : EFalse );
#endif    
    }
    
TCursorSelection CAknFepManager::UncommittedText() const
    {
    return iUncommittedText;
    }

void CAknFepManager::SetCase(TCase aCase)
    {
    // For QWERTY only keypad, extended input state like FnLock is applicable.
    // In case of ITU-T keypad, this flag should not be part of editor flag.
    // In case of numeric only editor Only FnReverse state is valid and editor will be on EFnReverse
    // State if FnKey is hold on state.
    
    //TODO: In case of other functionized input state like (FnNext), it should be stored in editor
    
#ifdef  RD_INTELLIGENT_TEXT_INPUT
    if(EditorState())
        {
        if( iFnKeyManager &&  ( iFnKeyManager->FnKeyState()== CAknFepFnKeyManager::EFnKeyLock ) )
            {
            EditorState()->SetFlags( EditorState()->Flags() | EAknEditorFlagFnLock );
            }
        else
            {
            EditorState()->SetFlags( EditorState()->Flags() & ~EAknEditorFlagFnLock );       
            }
        }
#endif

    InternalFepUI()->SetCase(aCase);
    }

CAknEdwinState* CAknFepManager::EditorState() const
    {
    if ( IsFepAwareTextEditor() )
        {
        MCoeFepAwareTextEditor* fepAvareTextEditor = iInputCapabilities.FepAwareTextEditor();
        
        if ( fepAvareTextEditor && fepAvareTextEditor->Extension1() )
            {
            return static_cast<CAknEdwinState*>( fepAvareTextEditor->Extension1()->State( KNullUid ) );
            }
        }
    return NULL;
    }

 void CAknFepManager::SetPredictiveIndicatorState(TAknEditingState& aState)
	{
	
	TInt currentCase = iCaseMan->CurrentCase();
#ifndef RD_INTELLIGENT_TEXT_INPUT
    if(iIsAutoCompleteOn && iLanguageCapabilities.iInputLanguageCode != ELangThai)
    	{
       	if (iLanguageCapabilities.iInputLanguageCode == ELangArabic)
        	aState=EAutoCompleteArabic;
       	
       	else if (iLanguageCapabilities.iInputLanguageCode == ELangHebrew)
           		 aState=EAutoCompleteHebrew;
       	
        else if (iLanguageCapabilities.iInputLanguageCode == ELangFarsi ||
            	 iLanguageCapabilities.iInputLanguageCode == ELangUrdu)
            	 aState=EAutoCompleteFarsiAndUrdu;
             	/*Hindi*/
        else if ( (iLanguageCapabilities.iInputLanguageCode == ELangHindi) || (iLanguageCapabilities.iInputLanguageCode == ELangMarathi))
        		  aState = EAutoCompleteHindi;
        
        else if(iLanguageCapabilities.iInputLanguageCode == ELangVietnamese)
            	{
            	switch (currentCase)
                	{
                	case EAknEditorUpperCase:
                    	aState =  EAutoCompleteUpperVietnamese;
                    	break;
                	case EAknEditorLowerCase:
                    	aState =  EAutoCompleteLowerVietnamese;
                    	break;
                    case EAknEditorTextCase:
                        aState = EAutoCompleteShiftedVietnamese;
                        break;
                    default:
                        break;
                    }    
                }
        else
                {
                switch (currentCase)
                    {
                    case EAknEditorUpperCase:
                        aState =  EAutoCompleteUpper;
                        break;
                    case EAknEditorLowerCase:
                        aState =  EAutoCompleteLower;
                        break;
                    case EAknEditorTextCase:
                        aState = EAutoCompleteShifted;
                        break;
                    default:
                        break;
                    }
                }
            
    	}
    else
#endif //RD_INTELLIGENT_TEXT_INPUT
#ifdef RD_INTELLIGENT_TEXT_INPUT
		if(iFnKeyManager->FnKeyState() != CAknFepFnKeyManager::EFnKeyNone)
			{
			aState = EFnKeyPressed;
			if(iFnKeyManager->FnKeyState() == CAknFepFnKeyManager::EFnKeyLock)
	            aState = EFnKeyLocked;    
			}
        else
#endif
        {
    	if (iLanguageCapabilities.iInputLanguageCode == ELangArabic)
      	 	 aState = ET9Arabic;
    	
   		else if (iLanguageCapabilities.iInputLanguageCode == ELangHebrew)
        	     aState = ET9Hebrew;
   		
    	else if (iLanguageCapabilities.iInputLanguageCode == ELangFarsi ||
        	     iLanguageCapabilities.iInputLanguageCode == ELangUrdu)
        		 aState = ET9FarsiAndUrdu;
        		 
    	else if (iLanguageCapabilities.iInputLanguageCode == ELangThai)
        		 aState = ET9Thai;
    	
    	/*Hindi*/
#ifdef RD_MARATHI
                        /*Marathi*/
                        else if ( iLanguageCapabilities.iInputLanguageCode == ELangMarathi )
                        	aState = EIndicatorStatePredictiveMarathi;
#endif // RD_MARATHI
    	
    	else if(iLanguageCapabilities.iInputLanguageCode == ELangVietnamese)
        	{
        	switch (currentCase)
            	{
            	case EAknEditorUpperCase:
                	aState =  ET9UpperVietnamese;
                	break;
            	case EAknEditorLowerCase:
                	aState =  ET9LowerVietnamese;
                	break;
                case EAknEditorTextCase:
                    aState = ET9ShiftedVietnamese;
                    break;
                default:
                    break;
                }    
            }
        
        else if(iLanguageCapabilities.iInputLanguageCode == ELangGreek)
            {
            switch (currentCase)
                {
                case EAknEditorUpperCase:
                    aState =  ET9UpperGreek;
                    break;
                case EAknEditorLowerCase:
                    aState =  ET9LowerGreek;
                    break;
                case EAknEditorTextCase:
                    aState = ET9ShiftedGreek;
                    break;
                default:
                    break;
                }    
            }
        
        //Cyrillic alphabet is used for Russian, Bulgarian and 
        //Ukranian languages
        else if(iLanguageCapabilities.iInputLanguageCode == ELangRussian ||
                iLanguageCapabilities.iInputLanguageCode == ELangBulgarian ||
                iLanguageCapabilities.iInputLanguageCode == ELangUkrainian )
            {
            switch (currentCase)
                {
                case EAknEditorUpperCase:
                    aState =  ET9UpperCyrillic;
                    break;
                case EAknEditorLowerCase:
                    aState =  ET9LowerCyrillic;
                    break;
                case EAknEditorTextCase:
                    aState = ET9ShiftedCyrillic;
                    break;
                default:
                    break;
                }    
            }
        else
            {
            switch (currentCase)
                {
                case EAknEditorUpperCase:
                    aState = ET9Upper;
                    break;
                case EAknEditorLowerCase:
                    aState = ET9Lower;
                    break;
                case EAknEditorTextCase:
                    aState = ET9Shifted;
                    break;
                default:
                    break;
                }
            }
        
    	}
	}
void CAknFepManager::UpdateIndicators()
    { 
    if(ELongKeyPress == iKeyRepeat)
        return;	
	// If it is fepware editor update the Indicators
	// otherwise not need excute remainig part of code.
	if( !iInputCapabilities.FepAwareTextEditor() || !iFepFullyConstructed ) 
		{
        //Ensure, If no idicator is require, make indicator state None 
        if (!IsFlagSet(EFlagForegroundUIComponentVisible | EFlagMenuPaneVisible))
            iIndicator->SetState(EStateNone);
		return;
		}

    TAknEditingState newState = EStateNone;
    MPtiLanguage* ptiLang = PtiEngine()->CurrentLanguage();    
    MAknEditingStateIndicator* editingStateIndicator = EditingStateIndicator();

    TBool findPaneIndicator(
        iAknEditorFlags & EAknEditorFlagNoEditIndicators &&
        iAknEditorFlags & EAknEditorFlagForceTransparentFepModes &&
        editingStateIndicator != (MAknEditingStateIndicator*)iIndicator );

#ifdef RD_SCALABLE_UI_V2
    TBool fingerItutIndicator = ( iFepPluginManager && 
                                  iFepPluginManager->PluginInputMode() == EPluginInputModeItut );
    // Portrait FSQ is added.
    TBool fingerFSQIndicator = ( iFepPluginManager && 
            ( iFepPluginManager->PluginInputMode() == EPluginInputModeFSQ ||
              iFepPluginManager->PluginInputMode() == EPluginInputModePortraitFSQ ) );
#else
    TBool fingerItutIndicator = EFalse;
    TBool fingerFSQIndicator = EFalse;
    
#endif
    if ( IsHybridAplhaEditor() && !iHybridAplphaChangedToAlphanumeric )
            {
            newState = EHybridModeLatin;
            }    
    //else if (!(iAknEditorFlags & EAknEditorFlagNoEditIndicators) 
    //    || findPaneIndicator || fingerItutIndicator)
    else if (!(iAknEditorFlags & EAknEditorFlagNoEditIndicators) 
        || findPaneIndicator || fingerItutIndicator || fingerFSQIndicator)
        {
        
#ifdef RD_SCALABLE_UI_V2
        if ( ( IsFlagSet(EFlagQwertyShiftMode)  || IsFlagSet(EFlagLongShiftKeyPress)) &&
             ((iFepPluginManager && iFepPluginManager->PluginInputMode() == EPluginInputModeVkb) ||
        #ifndef RD_INTELLIGENT_TEXT_INPUT
			      iQwertyInputMode
      	#else	       								
		           ( iQwertyInputMode && (IsFlagSet(EFlagQwertyShiftMode) ||IsFlagSet(EFlagLongShiftKeyPress))
		           && (iFnKeyManager->FnKeyState() != CAknFepFnKeyManager::EFnKeyNext)
		           && (iFnKeyManager->FnKeyState() != CAknFepFnKeyManager::EFnKeyLock)
		           && (iFnKeyManager->FnKeyState() != CAknFepFnKeyManager::EFnKeyForced)
		           )
		#endif
             ))
#else
        if ( iQwertyInputMode && IsFlagSet(EFlagQwertyShiftMode))
    
#endif        
            {
            newState = EQwertyShift;

#ifdef RD_INTELLIGENT_TEXT_INPUT 
			// To keep the shift key indicator in         
            // Check if fn/shift both key are pressed 
            if((iFnKeyManager->FnKeyState() != CAknFepFnKeyManager::EFnKeyNone) &&
            	(IsFlagSet(EFlagQwertyShiftMode) || IsFlagSet(EFlagLongShiftKeyPress)))
	            {
	            newState = EFnKeyPressed;	
	            }
#endif            
            
            }
        else
            {
            switch (iMode)
                {
                case EHiraganaKanji:
                    if (IsPredictive())
                        {
                        newState = EIndicatorStateJapanesePredictive;
                        }
                    else
                        {
                        newState = EIndicatorStateHiraganaKanji;
                        }
                    break;
                case EKatakana:
                    if (iCharWidth == EHalfWidthChar)
                        {
                        newState = EIndicatorStateHalfKatakana;
                        }
                    else if (iCharWidth == EFullWidthChar)
                        {
                        newState = EIndicatorStateFullKatakana;
                        }
                    break;
                case EHiragana:
                    {
                    newState = EIndicatorStateHiragana;
                    }
                    break;
                case EPinyin:
                    if (ptiLang->HasInputMode(EPtiEnginePinyinByPhrase))
                        {
                        newState = EIndicatorStatePinyinPhrase;
                        }                        
                    else
                        {
                        newState = EIndicatorStatePinyin;
                        }                        
#ifdef RD_INTELLIGENT_TEXT_INPUT
                    if ( iFnKeyManager->FnKeyState()
                        == CAknFepFnKeyManager::EFnKeyLock )
                        newState = EFnKeyLocked;
                    else if ( iFnKeyManager->FnKeyState()
                        != CAknFepFnKeyManager::EFnKeyNone )
                        newState = EFnKeyPressed;
#endif
                    break;
                case EZhuyin:
                    if (ptiLang->HasInputMode(EPtiEngineZhuyinByPhrase))
                        {
                        newState = EIndicatorStateZhuyinPhrase;
                        }                        
                    else
                        {
                        newState = EIndicatorStateZhuyin;
                        }                        
#ifdef RD_INTELLIGENT_TEXT_INPUT
                    if ( iFnKeyManager->FnKeyState()
                        == CAknFepFnKeyManager::EFnKeyLock )
                        newState = EFnKeyLocked;
                    else if ( iFnKeyManager->FnKeyState()
                        != CAknFepFnKeyManager::EFnKeyNone )
                        newState = EFnKeyPressed;
#endif
                    break;
                case EStroke:
                    {
                    TBool flag = ptiLang->HasInputMode(EPtiEngineStrokeByPhrase);
                    if (iLanguageCapabilities.iInputLanguageCode == ELangPrcChinese)
                        {
                        if (flag)
                            {
                            newState = EIndicatorStateStrokePhrase;
                            }                            
                        else
                            {
                            newState = EIndicatorStateStroke;
                            }                            
                        }
                    else
                        {
                        if (flag)
                            {
                            newState = EIndicatorStateStrokeTradPhrase;
                            }                            
                        else
                            {
                            newState = EIndicatorStateStrokeTrad;
                            }                            
                        }
                        }
#ifdef RD_INTELLIGENT_TEXT_INPUT
                    if ( iFnKeyManager->FnKeyState()
                        == CAknFepFnKeyManager::EFnKeyLock )
                        newState = EFnKeyLocked;
                    else if ( iFnKeyManager->FnKeyState()
                        != CAknFepFnKeyManager::EFnKeyNone )
                        newState = EFnKeyPressed;
#endif
                    break;
                case ECangJie:
                    newState = EIndicatorStateCangJie;
#ifdef RD_INTELLIGENT_TEXT_INPUT
                    if ( iFnKeyManager->FnKeyState()
                        == CAknFepFnKeyManager::EFnKeyLock )
                        newState = EFnKeyLocked;
                    else if ( iFnKeyManager->FnKeyState()
                        != CAknFepFnKeyManager::EFnKeyNone )
                        newState = EFnKeyPressed;
#endif
                    break;
                case EZhuyinFind:
                    newState = EIndicatorStateZhuyinFind;
                    break;
                case EStrokeFind:
                    newState = EIndicatorStateStrokeFind;
                    break;
                case ENumber:
                    {
                    newState = ENumeric;
                    //iLanguageCapabilities.iLocalDigitType = EDigitTypeWestern;
                    }
                    break;
                case ENativeNumber:
                    {
                    UpdateNumberIndicator( newState );
                    }
                    break;
                case EHangul:
                	{
                	newState = EIndicatorStateHangul;            	              	
                	}
                	break;
                case ELatin:
                    {
                    UpdateLatinIndicator( newState );
                    }
                    break;
                case EHindi:
                    {
                    UpdateHindiIndicator( newState );
                    }
                    break;
                default:
                    break;
           }
        }
    }

    if ( iFepPluginManager && iFepPluginManager->PluginInputMode() == EPluginInputModeVkb )
        {
        newState = iFepPluginManager->VKBIndicatorState( newState );
        }

    // Update indicators only if the state has been changed, or if the indicator has swapped
    if ((iPreviousEditingState != newState) || (editingStateIndicator != iEditIndicatorAtLastUpdate))
        {
        if (editingStateIndicator)
            {
            // because current avkon have not find chinese phrase indicator,
            // so change back to no-phrase
           if (findPaneIndicator || iAknEditorFlags & EAknEditorFlagFindPane )
                {
                switch(newState)
                    {
                    case EIndicatorStateStrokePhrase:
                        {
                        newState = EIndicatorStateStroke; 
                        }
                    break;
                    case EIndicatorStateStrokeTradPhrase:
                        {
                        newState = EIndicatorStateStrokeTrad; 
                        }
                    break;
                    case EIndicatorStatePinyinPhrase:
                        {
                        newState = EIndicatorStatePinyin; 
                        }
                    break;
                    case EIndicatorStateZhuyinPhrase:
                        {
                        newState = EIndicatorStateZhuyin; 
                        }
                    break;
                    default:
                        {
                        break;
                        }
                    }
                }
           // Add This condition for  Phonebook, ReTe, PF52.50_2008_wk32: 
           // Navigation bar disappears after tapping find pane and then returning back to Names list view.
           // Portrait FSQ is added.
           if (!(editingStateIndicator == (MAknEditingStateIndicator*)iIndicator &&
               ( iFepPluginManager && 
                    ( iFepPluginManager->PluginInputMode() == EPluginInputModeItut 
                    || iFepPluginManager->PluginInputMode() == EPluginInputModeFSQ 
                    || iFepPluginManager->PluginInputMode() == EPluginInputModePortraitFSQ ) ) &&
               IsFindPaneEditor()))
               {
               editingStateIndicator->SetState(newState);
               }

            // Ensure that no indicator is shown in navi pane when other
            // indicator is changed in use.
            if ( editingStateIndicator != (MAknEditingStateIndicator*)iIndicator &&
                 iPreviousEditingState == EStateNone &&
                 (!IsFlagSet(EFlagForegroundUIComponentVisible | EFlagMenuPaneVisible)) )

                {
                iIndicator->SetState( EStateNone );
                }

            iPreviousEditingState = newState;
            iEditIndicatorAtLastUpdate=editingStateIndicator;
            }
        }
    }

void CAknFepManager::UpdateNumberIndicator( TAknEditingState& aNewState )
    {
    
 // The fix is made only for Hindi
    UpdateNumericEditorDigitType();
    TBool supportsWesternNumericIntegerOrReal =
        ( iInputCapabilities.SupportsWesternNumericIntegerPositive() ||
        iInputCapabilities.SupportsWesternNumericIntegerNegative() ||
        iInputCapabilities.SupportsWesternNumericReal() );

#ifdef RD_SCALABLE_UI_V2                        
        iFepPluginManager->SetPluginNativeRange( ETrue );
#endif // RD_SCALABLE_UI_V2 
        // In usual case digit mode is Arabic-Indic digits if local digit mode is
        // Arabic-Indic and current input language is Arabic.
        if ( (!supportsWesternNumericIntegerOrReal &&
            iLanguageCapabilities.iLocalDigitType == EDigitTypeArabicIndic /* &&
            iLanguageCapabilities.iInputLanguageCode == ELangArabic*/
            )
            ||
            // But digit mode is Arabic-Indic digit in MFNE and real number
            // editors if UI language is Arabic and global digit setting is
            // Arabic-Indic digits.
            ( !(iAknEditorFlags & EAknEditorFlagLatinInputModesOnly) &&
            iLanguageCapabilities.iNumericEditorDigitType == EDigitTypeArabicIndic && // AknTextUtils::DigitModeQuery(AknTextUtils::EDigitModeShownToUser) &&
            supportsWesternNumericIntegerOrReal &&
            iUiLanguage == ELangArabic
            )
            )
            {
            aNewState=EArabicIndicNumeric;
                        
            }
        else if ( (!supportsWesternNumericIntegerOrReal &&
            iLanguageCapabilities.iLocalDigitType == EDigitTypeEasternArabicIndic /* &&
            iLanguageCapabilities.iInputLanguageCode == ELangUrdu,Farasi*/
            )
            ||
            // But digit mode is Arabic-Indic digit in MFNE and real number
            // editors if UI language is Arabic and global digit setting is
            // Arabic-Indic digits.
            ( !(iAknEditorFlags & EAknEditorFlagLatinInputModesOnly) &&
            iLanguageCapabilities.iNumericEditorDigitType == EDigitTypeEasternArabicIndic && // AknTextUtils::DigitModeQuery(AknTextUtils::EDigitModeShownToUser) &&
            supportsWesternNumericIntegerOrReal &&
            (iUiLanguage == ELangUrdu || iUiLanguage == ELangFarsi)))
            {
            aNewState=EArabicIndicNumeric;                            
            }    
        /* Devanagari Scripts */
        else if ( (!supportsWesternNumericIntegerOrReal &&
                iLanguageCapabilities.iLocalDigitType == EDigitTypeDevanagari //&&
                //iLanguageCapabilities.iInputLanguageCode == ELangHindi
                )
                ||
                ( !(iAknEditorFlags & EAknEditorFlagLatinInputModesOnly) &&
                iLanguageCapabilities.iNumericEditorDigitType == EDigitTypeDevanagari && // AknTextUtils::DigitModeQuery(AknTextUtils::EDigitModeShownToUser) &&
                supportsWesternNumericIntegerOrReal &&
                TAknFepUiIndicInputManager::IsScriptDevanagari( iUiLanguage )
                )
                )
            {
            aNewState= EDevanagariIndicNumeric;                            
            }
        else
        {
        if ( iLanguageCapabilities.iInputLanguageCode == ELangArabic )
            {
            iLanguageCapabilities.iLocalDigitType = EDigitTypeArabicIndic;
            aNewState=EArabicIndicNumeric;
            }
        else if ( iLanguageCapabilities.iInputLanguageCode == ELangFarsi 
               || iLanguageCapabilities.iInputLanguageCode == ELangUrdu )
            {
            iLanguageCapabilities.iLocalDigitType = EDigitTypeEasternArabicIndic;
            aNewState=EArabicIndicNumeric;
            }    
        else if ( iLanguageCapabilities.iInputLanguageCode == ELangHindi )
            {
            //iLanguageCapabilities.iLocalDigitType = EDigitTypeDevanagari;
            aNewState= EDevanagariIndicNumeric;
            }
        }
    }
    
void CAknFepManager::UpdateLatinIndicator( TAknEditingState& aNewState )
    {  
    if (IsPredictive())
        {
        //AutoComplete - Begin
        SetPredictiveIndicatorState( aNewState );
        //AutoComplete - End

        return;           
        }

    TInt currentCase = iCaseMan->CurrentCase();
#ifdef RD_INTELLIGENT_TEXT_INPUT
	if(iFnKeyManager->FnKeyState() == CAknFepFnKeyManager::EFnKeyLock)
		aNewState = EFnKeyLocked;
    else if(iFnKeyManager->FnKeyState() != CAknFepFnKeyManager::EFnKeyNone)
		aNewState = EFnKeyPressed;
	else    
#endif //RD_INTELLIGENT_TEXT_INPUT
		{
#ifdef RD_SCALABLE_UI_V2                        
    if( iFepPluginManager &&
        ( !iFepPluginManager->SyncWithPluginRange() || iFepPluginManager->PluginNativeRange() ) )
        {
        iFepPluginManager->SetPluginNativeRange( ETrue );
#endif // RD_SCALABLE_UI_V2 
        switch ( iLanguageCapabilities.iInputLanguageCode )
            {
            case ELangArabic:
                {
                aNewState=EMultitapArabic; 
                }
                return;
            case ELangHebrew:
                {
                aNewState=EMultitapHebrew;                              
                }
                return;
            case ELangFarsi:
            case ELangUrdu:
                {
                aNewState=EMultitapFarsiAndUrdu;                                 
                }
                return;
            case ELangThai:
                {
                aNewState = EMultitapThai;                               
                }
                return;
                
#ifdef RD_MARATHI
            case ELangMarathi:
                {
                aNewState = EIndicatorStateMultitapMarathi;                                
                }
            	return;
#endif // RD_MARATHI

#ifdef RD_HINDI_PHONETIC_INPUT  
            case KLangHindiPhonetic:
                {                            
                TInt currentPtiLang = (ptiLang)? ptiLang->LanguageCode() : 0;
                
                if(TAknFepUiIndicInputManager::IsIndicPhoneticLanguage(TLanguage(currentPtiLang)))
                    {
                    aNewState = TAknEditingState(SetPhoneticIndicator(TLanguage(currentPtiLang)));
                    }
                else
                    {
                     switch ( currentCase )
                        {
                        case EAknEditorUpperCase:
                            aNewState=EMultitapUpper;
                            if (iCharWidth == EFullWidthChar)
                                {
                                aNewState=EIndicatorStateFullLatinUpper;
                                }
                            break;
                        case EAknEditorLowerCase:
                            aNewState=EMultitapLower;
                            if (iCharWidth == EFullWidthChar)
                                {
                                aNewState=EIndicatorStateFullLatinLower;
                                }
                            break;
                        case EAknEditorTextCase:
                            aNewState=EMultitapShifted;
                            if (iCharWidth == EFullWidthChar)
                                {
                                aNewState=EIndicatorStateFullLatinText;
                                }
                            break;
                        default:
                            break;
                        }
                    }       
                
                }
            return;
#endif
            case ELangVietnamese:
                {                          
                switch (currentCase)
                    {
                    case EAknEditorUpperCase:
                        aNewState =  EMultitapUpperVietnamese;
                        break;
                    case EAknEditorLowerCase:
                        aNewState =  EMultitapLowerVietnamese;
                        break;
                    case EAknEditorTextCase:
                        aNewState = EMultitapShiftedVietnamese;
                        break;
                    default:
                        break;
                    }
				return;	  
                }
                
            case ELangGreek:
                {
                switch (currentCase)
                    {
                    case EAknEditorUpperCase:
                        aNewState =  EMultitapUpperGreek;
                        break;
                    case EAknEditorLowerCase:
                        aNewState =  EMultitapLowerGreek;
                        break;
                    case EAknEditorTextCase:
                        aNewState = EMultitapShiftedGreek;
                        break;
                    default:
                        break;
                    }
				return;  
                }
            
            //Cyrillic alphabet is used for Russian, Bulgarian and 
            //Ukranian languages. So fall through for all these languages -
            case ELangRussian:
            case ELangBulgarian:
            case ELangUkrainian:
                {
                switch (currentCase)
                    {
                    case EAknEditorUpperCase:
                        aNewState =  EMultitapUpperCyrillic;
                        break;
                    case EAknEditorLowerCase:
                        aNewState =  EMultitapLowerCyrillic;
                        break;
                    case EAknEditorTextCase:
                        aNewState = EMultitapShiftedCyrillic;
                        break;
                    default:
                        break;
                    } 
				return; 
                }
            }
#ifdef RD_SCALABLE_UI_V2     
        }
        // if plugin is not native range
    
    if(iFepPluginManager && iLanguageCapabilities.iInputLanguageCode != ELangRussian &&
       iLanguageCapabilities.iInputLanguageCode != ELangBulgarian &&
       iLanguageCapabilities.iInputLanguageCode != ELangUkrainian &&
       iLanguageCapabilities.iInputLanguageCode != ELangGreek )
        {
        iFepPluginManager->SetPluginNativeRange( EFalse );
        }
    
#endif // RD_SCALABLE_UI_V2                                 
		}
#ifdef RD_INTELLIGENT_TEXT_INPUT		
	if(FnKeyState() == CAknFepFnKeyManager::EFnKeyNone)
#endif //RD_INTELLIGENT_TEXT_INPUT		
	    {
        switch (currentCase)
        {
        case EAknEditorUpperCase:
            aNewState=EMultitapUpper;
            if (iCharWidth == EFullWidthChar)
                {
                aNewState=EIndicatorStateFullLatinUpper;
                }
            break;
        case EAknEditorLowerCase:
            aNewState=EMultitapLower;
            if (iCharWidth == EFullWidthChar)
                {
                aNewState=EIndicatorStateFullLatinLower;
                }
            break;
        case EAknEditorTextCase:
            aNewState=EMultitapShifted;
            if (iCharWidth == EFullWidthChar)
                {
                aNewState=EIndicatorStateFullLatinText;
                }
            break;
        default:
            break;
        }
        }
    }
void CAknFepManager::UpdateHindiIndicator( TAknEditingState& aNewState )
    {
    if ( IsPredictive() )
        {
        if( iIsAutoCompleteOn )
            aNewState = EAutoCompleteHindi; 
        else
            aNewState = EIndicatorStatePredictiveHindi;
        }
    else
        {
        aNewState = EIndicatorStateMultitapHindi;
        }
    }

TInt CAknFepManager::CurrentInputLangCode()
	{
	ASSERT(iPtiEngine);
	
	MPtiLanguage* lang = iPtiEngine->CurrentLanguage();
	ASSERT(lang);
	
	return lang->LanguageCode();
	}

TBool CAknFepManager::GetIndicatorImgID(TInt& aIndicatorImgID, TInt& aIndicatorTextID)
    {
    if (iPreviousEditingState == EStateNone)
        {
        UpdateIndicators();
        if (iPreviousEditingState == EStateNone)
            {
            return EFalse;    
            }
        }
 
	TBool predictiveMode = QueryPredictiveState(iPreviousEditingState);
	TBool predictiveAutoCompleteMode = EFalse;
	if(!predictiveMode)
		{
		predictiveAutoCompleteMode = QueryPredictiveAutoCompleteState(iPreviousEditingState);		
		}
    
    if (predictiveMode)
        {
        aIndicatorImgID = EAknNaviPaneEditorIndicatorT9;
        }
    else if (predictiveAutoCompleteMode)
        {
        aIndicatorImgID = EAknNaviPaneEditorIndicatorAutoComplete;   
        }
    else 
        {
        aIndicatorImgID = EAknNaviPaneEditorIndicatorQuery;
        }
   
    aIndicatorTextID = EvaluateState(iPreviousEditingState);
    
    return ETrue;
    }
    
TInt CAknFepManager::EvaluateState(const TAknEditingState aState)
{
	TInt result = 0;
	switch(aState)
	{
		case EArabicIndicNumeric:
  			 result = EAknNaviPaneEditorIndicatorArabicIndicNumberCase;
  			 break;
  			 
		case ET9Thai:
		case EMultitapThai:
#ifndef RD_INTELLIGENT_TEXT_INPUT
		case EAutoCompleteThai:
#endif
			result = EAknNaviPaneEditorIndicatorThai;
			break;				
			
		case EIndicatorStatePredictiveHindi:
		case EIndicatorStateMultitapHindi:
#ifndef RD_INTELLIGENT_TEXT_INPUT
		case EAutoCompleteHindi:
#endif
#ifdef RD_MARATHI
		case EIndicatorStateMultitapMarathi:
		case EIndicatorStatePredictiveMarathi:
#endif // RD_MARATHI
			result = EAknNaviPaneEditorIndicatorDevanagariCase;
			break;	
			
		case ET9Arabic:
		case EMultitapArabic:
#ifndef RD_INTELLIGENT_TEXT_INPUT
		case EAutoCompleteArabic:
#endif
			result = EAknNaviPaneEditorIndicatorArabicCase;
			break;	

		case ET9Hebrew:
		case EMultitapHebrew:
#ifndef RD_INTELLIGENT_TEXT_INPUT
		case EAutoCompleteHebrew:
#endif
			result = EAknNaviPaneEditorIndicatorHebrewCase;
			break;	
			
		case ET9FarsiAndUrdu:
		case EMultitapFarsiAndUrdu:
#ifndef RD_INTELLIGENT_TEXT_INPUT
		case EAutoCompleteFarsiAndUrdu:
#endif
			result = EAknNaviPaneEditorIndicatorFarsiAndUrduCase;
			break;					


		case ET9Upper:
		case EMultitapUpper:
#ifndef RD_INTELLIGENT_TEXT_INPUT
		case EAutoCompleteUpper:
#endif
			result = EAknNaviPaneEditorIndicatorUpperCase;	
			break;

		case ET9Lower:
		case EMultitapLower:
#ifndef RD_INTELLIGENT_TEXT_INPUT
		case EAutoCompleteLower:
#endif
			result = EAknNaviPaneEditorIndicatorLowerCase;	
			break;

		case ET9Shifted:
		case EMultitapShifted:
#ifndef RD_INTELLIGENT_TEXT_INPUT
		case EAutoCompleteShifted:
#endif
			result = EAknNaviPaneEditorIndicatorTextCase;	
			break;
			
		case ENumeric:
			result = EAknNaviPaneEditorIndicatorNumberCase;	
			break;		
  			 
  			 	
		case EIndicatorStatePinyin:
			result = EAknNaviPaneEditorIndicatorPinyin;
			break;
		case EIndicatorStatePinyinPhrase:
			result = EAknNaviPaneEditorIndicatorPinyinPhrase;
			break;
			
		case EIndicatorStateZhuyin:
			 result = EAknNaviPaneEditorIndicatorZhuyin;
			break;
		case EIndicatorStateZhuyinPhrase:
			 result = EAknNaviPaneEditorIndicatorZhuyinPhrase;
			break;

		case EIndicatorStateZhuyinFind:
			result = EAknNaviPaneEditorIndicatorZhuyinFind;
			break;

		case EDevanagariIndicNumeric :
			result = EAknNaviPaneEditorIndicatorDevanagariIndicNumberCase;
			break;
		
		case EIndicatorStateHiraganaKanji:
		case EIndicatorStateJapanesePredictive:
			result = EAknNaviPaneEditorIndicatorJapaneseHiraganaKanji;
			break;

		case EIndicatorStateFullKatakana:
			result = EAknNaviPaneEditorIndicatorJapaneseFullKatakana;
			break;

		case EIndicatorStateHalfKatakana:
			result = EAknNaviPaneEditorIndicatorJapaneseHalfKatakana;
			break;

		case EIndicatorStateFullLatinText:
			result = EAknNaviPaneEditorIndicatorJapaneseFullTextAlphabet;
			break;

		case EIndicatorStateFullLatinUpper:
			result = EAknNaviPaneEditorIndicatorJapaneseFullUpperAlphabet;
			break; 

		case EIndicatorStateFullLatinLower:
			result = EAknNaviPaneEditorIndicatorJapaneseFullLowerAlphabet;
			break;

		case EIndicatorStateFullNumeric:
			result = EAknNaviPaneEditorIndicatorJapaneseFullNumeric;
			break;

		case EIndicatorStateHiragana:
			result = EAknNaviPaneEditorIndicatorJapaneseHiragana;
			break;

		case EIndicatorStateStroke:
			result = EAknNaviPaneEditorIndicatorStroke;
			break;
		case EIndicatorStateStrokePhrase:
			result = EAknNaviPaneEditorIndicatorStrokePhrase;
			break;

		case EIndicatorStateStrokeTrad:
			result = EAknNaviPaneEditorIndicatorStrokeTrad;
			break;
		case EIndicatorStateStrokeTradPhrase:
			result = EAknNaviPaneEditorIndicatorStrokeTradPhrase;
			break;

		case EQwertyShift:
			result = EAknNaviPaneEditorIndicatorQwertyShift;
			break;

		case EIndicatorStateStrokeFind:
			result = EAknNaviPaneEditorIndicatorStrokeFind;
			break;

		case EIndicatorStateCangJie:
			result = EAknNaviPaneEditorIndicatorCangJie;
			break;
		
		case ET9UpperVietnamese:
		case EMultitapUpperVietnamese:
#ifndef RD_INTELLIGENT_TEXT_INPUT
		case EAutoCompleteUpperVietnamese:
#endif
			result = EAknNaviPaneEditorIndicatorVietnameseUpperCase;
			break;			
			
		case ET9LowerVietnamese:
		case EMultitapLowerVietnamese:
#ifndef RD_INTELLIGENT_TEXT_INPUT
		case EAutoCompleteLowerVietnamese:
#endif
			result = EAknNaviPaneEditorIndicatorVietnameseLowerCase;
			break;	
			
		case ET9ShiftedVietnamese:
		case EMultitapShiftedVietnamese:
#ifndef RD_INTELLIGENT_TEXT_INPUT
		case EAutoCompleteShiftedVietnamese:
#endif
			result = EAknNaviPaneEditorIndicatorVietnameseTextCase;
			break;
#ifdef RD_HINDI_PHONETIC_INPUT	
		case EIndicatorStateHindiPhoneticUpper:
			result = EAknNaviPaneEditorIndicatorDevanagariPhoneticUpperCase;
			break;
		case EIndicatorStateHindiPhoneticLower:
			result = EAknNaviPaneEditorIndicatorDevanagariPhoneticLowerCase;
			break;
#endif
        case ET9UpperGreek:
        case EMultitapUpperGreek:
            result = EAknNaviPaneEditorIndicatorGreekUpperCase;
            break;
            
        case ET9LowerGreek:
        case EMultitapLowerGreek:
            result = EAknNaviPaneEditorIndicatorGreekLowerCase;
            break;
        
        case ET9ShiftedGreek:
        case EMultitapShiftedGreek:
            result = EAknNaviPaneEditorIndicatorGreekTextCase;
            break;
            
        case ET9UpperCyrillic:
        case EMultitapUpperCyrillic:
            result = EAknNaviPaneEditorIndicatorCyrillicUpperCase;
            break;
            
        case ET9LowerCyrillic:
        case EMultitapLowerCyrillic:
            result = EAknNaviPaneEditorIndicatorCyrillicLowerCase;
            break;
        
        case ET9ShiftedCyrillic:
        case EMultitapShiftedCyrillic:
            result = EAknNaviPaneEditorIndicatorCyrillicTextCase;
            break;
            
        case EHybridModeLatin:
            result = EAknNaviPaneEditorIndicatorHybridLatin;
            break;
        case EIndicatorStateHangul:
            result = EAknNaviPaneEditorIndicatorKoreanHangul;
            break;
        
	}
	return result;

}    

TBool CAknFepManager::IsAbleToLaunchSCT() const
    {
    TBool ableToLaunchSCT = ETrue;

    if (ExtendedInputCapabilities() & CAknExtendedInputCapabilities::EDisableSCT && 
        PluginInputMode() == EPluginInputModeNone )
        {
        // The editor doesn't allow SCT.
        return EFalse;
        }
    
    if (iMode == ENumber || iMode == ENativeNumber)
        {
        if (iPermittedInputModes == EAknEditorNumericInputMode || !NumericModeSCTResourceId()) 
            {
            // SCT is not launched if the number mode is the only input mode or
            // if there is not keymap resource for the current numeric keymap mode.
            ableToLaunchSCT = EFalse;
            }
        if (iAknEditorFlags & EAknEditorFlagUseSCTNumericCharmap ||
            ( (iAknEditorNumericKeymap == EAknEditorAlphanumericNumberModeKeymap) &&
              (iPermittedInputModes != EAknEditorNumericInputMode) ))
            {
            // SCT is launched in the number mode if requested by editor flag
            // EAknEditorFlagUseSCTNumericCharmap or if also text input modes are
            // allowed and the keymap is EAknEditorAlphanumericNumberModeKeymap.
            ableToLaunchSCT = ETrue;
            }      
        }
    if (!(EditorHasFreeSpace() && EditorState() 
        && EditorState()->SpecialCharacterTableResourceId()))
        {
        ableToLaunchSCT = EFalse;
        }
    
    return ableToLaunchSCT;
    }

TBool CAknFepManager::QueryPredictiveState(const TAknEditingState aState)
 { 
    return  (( aState == EIndicatorStatePinyin) ||
             ( aState == EIndicatorStatePinyinPhrase) ||
             ( aState == EIndicatorStateZhuyin) ||
             ( aState == EIndicatorStateZhuyinPhrase) ||
             ( aState == EIndicatorStateZhuyinFind) ||
             ( aState == EIndicatorStateStrokeFind) ||
             ( aState == EIndicatorStateStroke) || 
             ( aState == EIndicatorStateStrokePhrase) ||
			 ( aState == EIndicatorStateCangJie) ||
             ( aState == EIndicatorStateStrokeTrad) || 
             ( aState == EIndicatorStateStrokeTradPhrase) ||
             ( aState == ET9Upper) ||
             ( aState == ET9Shifted) ||
             ( aState == ET9Lower) ||
             ( aState == ET9Arabic) ||
             ( aState == ET9Hebrew) ||
             ( aState == ET9FarsiAndUrdu) ||
             ( aState == ET9Thai) ||
             ( aState == EIndicatorStatePredictiveHindi) ||
#ifdef RD_MARATHI
             ( aState == EIndicatorStatePredictiveMarathi) ||
#endif // RD_MARATHI
             ( aState == EIndicatorStateJapanesePredictive) ||
             ( aState == ET9UpperVietnamese) ||
             ( aState == ET9LowerVietnamese) ||
             ( aState == ET9ShiftedVietnamese) ||
             ( aState == ET9UpperGreek) ||
             ( aState == ET9LowerGreek) ||
             ( aState == ET9ShiftedGreek) ||
             ( aState == ET9UpperCyrillic) ||
             ( aState == ET9LowerCyrillic) ||
             ( aState == ET9ShiftedCyrillic) );  
 }

 TBool CAknFepManager::QueryPredictiveAutoCompleteState(TAknEditingState aState)
 {
 	 	        
    return (( aState == EAutoCompleteUpper ) ||  
     		( aState == EAutoCompleteShifted )  ||
     		( aState == EAutoCompleteLower ) || 
     		( aState == EAutoCompleteHindi ) ||
     		( aState == EAutoCompleteArabic ) ||
     		( aState == EAutoCompleteHebrew ) ||
     		( aState == EAutoCompleteFarsiAndUrdu ) ||
     		( aState == EAutoCompleteThai ) ||
     		( aState == EAutoCompleteUpperVietnamese ) ||
     		( aState == EAutoCompleteLowerVietnamese ) ||
     		( aState == EAutoCompleteShiftedVietnamese ));
 }

void CAknFepManager::LaunchSpecialCharacterTableL(TInt aResourceId, TBool aLaunchedByTouchWin, TBool aSmileyFirst)
    {
#ifdef RD_INTELLIGENT_TEXT_INPUT    
    // Set the iSupressCursorMoveToEndChrKeyPressed when cursor is in 
	// between two words and SCT is launched
    iSupressCursorMoveToEndChrKeyPressed = ETrue;
#endif //RD_INTELLIGENT_TEXT_INPUT    
#ifdef RD_SCALABLE_UI_V2
    if ( iStarKeyPressed && iFepPluginManager && 
         iFepPluginManager->PluginInputMode() == EPluginInputModeItut )
            {
            aLaunchedByTouchWin = ETrue;
            }
    iCancelPopupInQwerty = aLaunchedByTouchWin;
#endif //RD_SCALABLE_UI_V2
    DoLaunchSctAndPctL(aResourceId, aSmileyFirst ? EShowSmileyFirst : EShowSctFirst );
#ifdef RD_SCALABLE_UI_V2
    iCancelPopupInQwerty = EFalse;
#endif //RD_SCALABLE_UI_V2    
#ifdef RD_INTELLIGENT_TEXT_INPUT    
    iSupressCursorMoveToEndChrKeyPressed = ETrue;  
#endif //RD_INTELLIGENT_TEXT_INPUT     
    }

TBool CAknFepManager::IsAbleToLaunchPCT() const
    {
    TBool isAbleToLaunch = EFalse;
    if (FeatureManager::FeatureSupported(KFeatureIdJapanesePicto)
     && (iAknEditorFlags & EAknEditorFlagEnablePictographInput))
        {
        isAbleToLaunch = ETrue;
        }
    return isAbleToLaunch;
    }

TBool CAknFepManager::IsAbleToLaunchSmiley() const
    {
	if(iLanguageCapabilities.iRightToLeftLanguage)
		{
	       return EFalse;
		}
	
    TBool ableToLaunchSmiley(EFalse);
    
    CAknEdwinState* edwinState = EditorState();
    if ( edwinState && EditorHasFreeSpace() )
        {
        CAknEdwinFormAccessor* formAccessor = edwinState->FormAccessor();
        if(formAccessor)
            {
            CEikEdwin *edwin = static_cast<CEikEdwin*>(formAccessor->FormClientControl());
            if(edwin)
                {
                ableToLaunchSmiley = ((edwin->UserFlags()&CEikEdwin::EAvkonEnableSmileySupport) != 0);
                }
            }
        }

    return ableToLaunchSmiley;
    }

TInt CAknFepManager::GetCurrentEditorSCTResId() const
    {
    if(!EditorState())
        {
    	return NumericModeSCTResourceId();    	
        }
    TInt currentEditorSCTResId = EditorState()->SpecialCharacterTableResourceId();
    if ( FeatureManager::FeatureSupported(KFeatureIdChinese) )
        {        
        if (currentEditorSCTResId == R_AVKON_SPECIAL_CHARACTER_TABLE_DIALOG)
            {
            if (iMode == EZhuyin)
            	{
            	currentEditorSCTResId = 
                                  R_AVKON_SPECIAL_CHARACTER_TABLE_DIALOG_FOR_ZHUYIN_SYMBOL_INPUT;
        	    }
            else
            	{
            	currentEditorSCTResId = R_AVKON_SPECIAL_CHARACTER_TABLE_DIALOG_CHINESE;
        	    }        
            }
        }

    if ( FeatureManager::FeatureSupported(KFeatureIdKorean) )
        {
        // Use the special sct resource file for Korean variant.
        if (currentEditorSCTResId == R_AVKON_SPECIAL_CHARACTER_TABLE_DIALOG)
            {
            currentEditorSCTResId = R_AVKON_SPECIAL_CHARACTER_TABLE_DIALOG_KOREAN;
            }
        }
    
    if (currentEditorSCTResId == EDefaultSCTResourceId) // -1 forces the default in SCT resource definition
        {
        if (iAknEditorFlags & EAknEditorFlagLatinInputModesOnly)
            {
            currentEditorSCTResId = R_AVKON_SPECIAL_CHARACTER_TABLE_DIALOG_LATIN_ONLY;
            }        
        else	
            {        		        	
            // only ITU-T and when Bopomofo symbol input mode is supported, we use new sct table
            TInt supportSCTToneMarks = EFalse;
            MPtiCoreInfo* ptiCoreInfo = iPtiEngine->CoreInfo(EPtiEngineZhuyin);
            
            if(ptiCoreInfo)
                {
                supportSCTToneMarks = ptiCoreInfo->CapsBits() & ESupportSCTToneMarks;
                }
            
            ptiCoreInfo = iPtiEngine->CoreInfo(EPtiEngineZhuyinByPhrase);
            if(ptiCoreInfo)    
                {
                supportSCTToneMarks = supportSCTToneMarks || 
                                      (ptiCoreInfo->CapsBits() & ESupportSCTToneMarks);
                }
            
            if(!iQwertyInputMode && (iMode == EZhuyin) && supportSCTToneMarks)
                {
                currentEditorSCTResId = 
                                  R_AVKON_SPECIAL_CHARACTER_TABLE_DIALOG_FOR_ZHUYIN_SYMBOL_INPUT;
                }
            else
                {
                if ( FeatureManager::FeatureSupported(KFeatureIdChinese) )
                    {    
                    currentEditorSCTResId = R_AVKON_SPECIAL_CHARACTER_TABLE_DIALOG_CHINESE;
                    }                
                else if( FeatureManager::FeatureSupported(KFeatureIdKorean) )
                    {    
                    // Use the special sct resource file for Korean variant.
                    currentEditorSCTResId = R_AVKON_SPECIAL_CHARACTER_TABLE_DIALOG_KOREAN;
                    }               
                else
                    {                    
                    currentEditorSCTResId = R_AVKON_SPECIAL_CHARACTER_TABLE_DIALOG;
                    }
                }
            }
        }  
    return currentEditorSCTResId;         
    }

void CAknFepManager::LaunchPictographCharacterTableL()
    {
    DoLaunchSctAndPctL(NULL, EShowPctFirst);
    }


void CAknFepManager::UpdateInlineEditL(const TDesC& aNewInlineText, 
                                       TInt aPositionOfInsertionPointInInlineText)
    {
    if ( ! IsFlagSet ( EFlagInsideInlineEditingTransaction ) )
        {
        return;
        }
    // Japanese implementation needs to be merged with functionality of other languages. Now they are separate.
    if (iLanguageCapabilities.iInputLanguageCode == ELangJapanese)
        {
        iUncommittedText.iCursorPos = iUncommittedText.iAnchorPos + 
                                      aPositionOfInsertionPointInInlineText;

        if (aNewInlineText.Length() == 1)
            {
            if (aNewInlineText[0] == KAknFEPLineFeedSymbol ||
                aNewInlineText[0] == KAknFEPMirroredLineFeedSymbol)
                {
                SetFlag(EFlagLineFeedCharacter);
                ClearFlag(EFlagSpaceCharacter | EFlagFullWidthSpaceCharacter);
                }
            else if (aNewInlineText[0] == iSpaceCharacterCode)
                {
                SetFlag(EFlagSpaceCharacter);
                ClearFlag(EFlagLineFeedCharacter | EFlagFullWidthSpaceCharacter);
                }
            else if (aNewInlineText[0] == iFullWidthSpaceCharacterCode)
                {
                SetFlag(EFlagFullWidthSpaceCharacter);
                ClearFlag(EFlagLineFeedCharacter | EFlagSpaceCharacter);
                }
            else
                {
                ClearFlag(EFlagLineFeedCharacter 
                          | EFlagSpaceCharacter 
                          | EFlagFullWidthSpaceCharacter);
                }
            if (iInputCapabilities.SupportsSecretText())
                {
                TBuf<1> buf;
                buf.Append(aNewInlineText[0]);
                NewCharacterL(buf);
                }
            else
                {
                SetFlag(EFlagInsideMultitapInlineEditingTransaction);
                iInputCapabilities.FepAwareTextEditor()->UpdateFepInlineTextL(aNewInlineText, 
                                                  aPositionOfInsertionPointInInlineText);
                }
            }
        else
            {
            ClearFlag(EFlagLineFeedCharacter | EFlagSpaceCharacter | EFlagFullWidthSpaceCharacter);
            iInputCapabilities.FepAwareTextEditor()->UpdateFepInlineTextL(aNewInlineText, 
                                                   aPositionOfInsertionPointInInlineText);
            }
#ifdef _DEBUG
        RDebug::Print(_L("UpdateInlineEditL text=[%S] len=[%d] CurPos=[%d] AncPos=[%d] change=[%d]"),
            &aNewInlineText, iUncommittedText.Length(),
            iUncommittedText.iCursorPos, iUncommittedText.iAnchorPos,
            aPositionOfInsertionPointInInlineText);
#endif
        }
    else
        {
        	TInt curPos = iUncommittedText.iAnchorPos + aNewInlineText.Length();
        	
        	TInt deltaCur = curPos - iUncommittedText.iCursorPos;
        	
        	TInt maxEdSize = iInputCapabilities.FepAwareTextEditor()->DocumentMaximumLengthForFep();
        	
        	TInt docLength = iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep();
        	
        	HBufC* newInlineText=NULL;
	        	
        	TBool syncLater = ETrue;
			// we have to restrict to a length that is permissible in the editor
        	// this situation comes when we select the max length condition is reached 
        	// in the editor and we navigate to one of the words in the middle 
        	// and select a longer word from the candidate list        	
        	

        	if( (maxEdSize > 0) && ( deltaCur > 0 ) 
        							&& (( docLength + deltaCur) > maxEdSize ) )
        	{
        		newInlineText= aNewInlineText.Left( aNewInlineText.Length() - (docLength + deltaCur - maxEdSize )).AllocLC();
        		aPositionOfInsertionPointInInlineText=newInlineText->Length();
        	}
        	else if((maxEdSize > 0) && (curPos > maxEdSize))
        	{
        		newInlineText= aNewInlineText.Left(maxEdSize - iUncommittedText.iAnchorPos).AllocLC();
        		aPositionOfInsertionPointInInlineText=newInlineText->Length();
        	}        	
        	else
        	{
        		newInlineText=aNewInlineText.AllocLC();
        		syncLater = EFalse;        	
        	} 
	// The Below code changes are made to sync iTextBuffer and the last word in the editor.When the 
	// cursor position reaches more than the max length of editor, tail length is modified according 
	// to truncated last word and Autocompletion bit length (Element count) value to display last 
	// word in the editor properly. 			       	        	        	
    #ifdef RD_INTELLIGENT_TEXT_INPUT        	 	
			if(syncLater)
        	    {       	    
    	        TBuf<CAknFepManager::EMaximumFepWordLength> currentText = iPtiEngine->CurrentWord();
    	        currentText.SetLength(newInlineText->Length());        	        
    	        iPtiEngine->HandleCommandL( EPtiCommandSetCursorCrossedMaxLength );
    	        iPtiEngine->SetCurrentWord(currentText); 
    	        TransferFepStateToEditorL();
    	        iPtiEngine->HandleCommandL( EPtiCommandResetCursorCrossedMaxLength );
        	    }
	#endif 	
        	// CleanupStack::PushL(TCleanupItem(ClearFepStateOnLeave,this));
        	iUncommittedText.iCursorPos = iUncommittedText.iAnchorPos + aPositionOfInsertionPointInInlineText;

		HBufC* decoratedInlineEdit = HBufC::NewLC(aNewInlineText.Length()*3);
		TPtr bufferPtr(decoratedInlineEdit->Des());

        if ( EditorSupportsNeutralProtection() )
            {
        		if ( newInlineText->Length() > 0 )
                {
                iInlineTextDecorator->DecorateInlineText(
        			newInlineText->Des(),
                    bufferPtr,
                    aPositionOfInsertionPointInInlineText,
                    static_cast<TChar>(PreviousChar()),
                    IsRightToLeftParagraph(DocPos() ),
                    ( CurrentInputDirectionality() == TBidiText::ERightToLeft ) );
                }

            // Because the length of the new inline edit may bear no relation to the old inline edit,
            // the passed desired position of the cursor (which is hidden in S60 anyway) in the new
            // inline edit is restricted to its Length(). The input value of parameter
            // aPositionOfInsertionPointInInlineText is ignored, and its modified value
            // from the DecorateInlineText routine is also ignored.
            
			aPositionOfInsertionPointInInlineText = decoratedInlineEdit->Length();

			iUncommittedText.iCursorPos = iUncommittedText.iAnchorPos + aPositionOfInsertionPointInInlineText;
			
            iInputCapabilities.FepAwareTextEditor()->UpdateFepInlineTextL(
                bufferPtr, aPositionOfInsertionPointInInlineText);
            	
            iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(iUncommittedText);
            iUncommittedText.iAnchorPos-=decoratedInlineEdit->Length();
        	}

        	else
        	{
        		// Even without the issue of decoration, the length of the new inline edit may bear no
        		// relation to the old inline edit. It is always set to length (the end) of the new inline edit
        		aPositionOfInsertionPointInInlineText = newInlineText->Length();

        		iInputCapabilities.FepAwareTextEditor()->UpdateFepInlineTextL(
        			newInlineText->Des(), aPositionOfInsertionPointInInlineText);

        		// This is needed here too because we should not insist that the new text is the same length
        		// as the old, and so the iUncommitted text should be reset.
        		iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(iUncommittedText);
        		iUncommittedText.iAnchorPos-=newInlineText->Length();
        	}       

        	
        CleanupStack::PopAndDestroy(decoratedInlineEdit);  
        SendEventsToPluginManL( EPluginSyncFepAwareText, EPluginSyncUpdatedText );
        CleanupStack::PopAndDestroy(newInlineText);
        //Send info to engine to set EPtiXt9FlagAutoCompletionSetOnToOff flag as false 
        TInt value = 0;
        TRAP_IGNORE(iPtiEngine->HandleCommandL( EPtiCommandAutoCompletionOnToOff,&value));			
        }  
    }

void CAknFepManager::StartInlineEditL()
    {
    if(iConcatenationTimer->IsActive())
    	iConcatenationTimer->Cancel();
	
    //currently western only, would need to sort out flags if this were to be used
    //in multitap modes
    switch (iMode)
        {
        case ELatin:
        case EKatakana:
        case EHiragana:
            iInputCapabilities.FepAwareTextEditor()->StartFepInlineEditL(KNullDesC, 0, EFalse, 
                                                                         NULL, *this, *this);
            iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(iUncommittedText);
            break;
        default:
            iInputCapabilities.FepAwareTextEditor()->StartFepInlineEditL(KNullDesC, 0, EFalse, 
                                                                         NULL, *this, *this);
            break;
        }
#ifdef _DEBUG
    RDebug::Print(_L("StartInlineEditL len=[%d] CurPos=[%d] AncPos=[%d]"),
        iUncommittedText.Length(),
        iUncommittedText.iCursorPos, iUncommittedText.iAnchorPos );
#endif

    // have to calculate the number of free space on current EDITOR after StartFepInlineEditL().
    TInt maxEdSize = iInputCapabilities.FepAwareTextEditor()->DocumentMaximumLengthForFep();
    TInt edSize = iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep();
    iFreeSpaceOfEditor = maxEdSize - edSize;

    SetFlag(EFlagInsideInlineEditingTransaction);
    }

void CAknFepManager::StartInlineEditL(const TDesC& aText)
    {
    if(iConcatenationTimer->IsActive())
    	iConcatenationTimer->Cancel();
	
    TInt textLength = aText.Length();

    // CleanupStack::PushL(TCleanupItem(ClearFepStateOnLeave,this));
    
	HBufC* decoratedInlineEdit = HBufC::NewLC(aText.Length()*3);
	TPtr bufferPtr(decoratedInlineEdit->Des());

    // decorate the inline edit
    if( aText.Length() > 0 && EditorSupportsNeutralProtection() )
        {
        iInlineTextDecorator->DecorateInlineText(
            aText,
            bufferPtr,
            textLength, // aPositionOfInsertionPointInInlineText,
            static_cast<TChar>(PreviousChar()),
            IsRightToLeftParagraph( DocPos() ),
            ( CurrentInputDirectionality() == TBidiText::ERightToLeft ) );
        }
    else
        {

        bufferPtr.Copy(aText);
        }
    
    // Before starting editor inline, we cancel editor inline if by any chance editor ends up in inline edit and fep does not
        TBool phoneIdle = (EditorType() == CAknExtendedInputCapabilities::EPhoneNumberEditor); 
        
        if(phoneIdle)
            {
            iInputCapabilities.FepAwareTextEditor()->CancelFepInlineEdit();
            }
    
    // operates at the current insertion point or selection
    iInputCapabilities.FepAwareTextEditor()->StartFepInlineEditL(

        bufferPtr,
        textLength, // aPositionOfInsertionPointInInlineText,
        EFalse,  // Cursor visibility
        0, // MFormCustomDrawer*
        *this,
        *this);

    // CleanupStack::Pop(); // cleanupitem for FEP state.

    if (!iInputCapabilities.FepAwareTextEditor())
        {
        // In case while starting the inline edit, 3rd party app removes the focus out of editor before the transaction
        // completes, we return with an error.
        User::Leave(KErrCorrupt);
        }
    // cursor span needs to be updated following potential modification
    // of the inline edit content before it was started
    iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(iUncommittedText);
	iUncommittedText.iAnchorPos-=decoratedInlineEdit->Length();
	CleanupStack::PopAndDestroy(decoratedInlineEdit);

    SetFlag(EFlagInsideInlineEditingTransaction);
    }

void CAknFepManager::StartInlineEditL(TCursorSelection aCursorSelection, 
                                      const TDesC& aInitialInlineText, 
                                      TInt aPositionOfInsertionPointInInlineText, 
                                      TBool aCursorVisibility)
    {
    	
		if(iConcatenationTimer->IsActive())
    	iConcatenationTimer->Cancel();
	
    TBool setToTrue=EFalse;

    // setToTrue parameter forces use of selection hiding inline edit editor API -
    // this method also sets the cursor selection to the second parameter
    // CleanupStack::PushL(TCleanupItem(ClearFepStateOnLeave,this));
    iInputCapabilities.FepAwareTextEditor()->Extension1()->StartFepInlineEditL(
        setToTrue,
        aCursorSelection,
        aInitialInlineText,
        aPositionOfInsertionPointInInlineText,
        aCursorVisibility,
        0, // MFormCustomDrawer* ?
        *this,
        *this);
    // CleanupStack::Pop(); // cleanupitem for FEP state.
    SetFlag(EFlagInsideInlineEditingTransaction);
    //This will suppress Edit submenu from poping up
    SetCcpuFlag(ECcpuStateIgnoreStarUp);
    }

TBool CAknFepManager::CloseUiIfWordDeletedL()
    {
    if (iUncommittedText.Length()==0)
        {
        // word has been totally deleted
        ClearFlag(EFlagCompoundWord);
        TryCloseUiL();
        return ETrue;
        }
    return EFalse;
    }

TBool CAknFepManager::TryGetTextToUncommitL(TDes& aText, TInt aCode, TBool& aComsumeKey)
    {
    TWesternNaviEvent naviEvent = ENullNaviEvent;
    if (aCode == EKeyLeftArrow)
        {
        naviEvent = ELeftNaviEvent;
        }
    else if (aCode == EKeyRightArrow)
        {
        naviEvent = ERightNaviEvent;
        }
    else if (aCode == EKeyBackspace)
        {
        naviEvent = EBackspaceEvent;
        }

    TInt nextCharPos = 0;  // Character ordinal in logical memory (0 - n-1)
    TBool hindibackspace = 0 ;  
    TBool nextPositionFound = EFalse;
    iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(iUncommittedText);
    if ( naviEvent == ERightNaviEvent )
        {
        nextPositionFound = GetNextVisualRightCharacter( nextCharPos );
        if( (TAknFepUiIndicInputManager :: IsIndicLangauge(TLanguage(iLanguageCapabilities.iInputLanguageCode))) || 
                (TLanguage(iLanguageCapabilities.iInputLanguageCode) == ELangThai))
            {
            FindStartOfWord(nextCharPos);   
                        
            if(nextCharPos < iUncommittedText.LowerPos())
                {
                nextCharPos = iUncommittedText.LowerPos();
                }
            }
        }
    else if ( naviEvent == ELeftNaviEvent )
        {
        nextPositionFound = GetNextVisualLeftCharacter( nextCharPos );
        if( nextPositionFound && (TAknFepUiIndicInputManager :: IsIndicLangauge(TLanguage(iLanguageCapabilities.iInputLanguageCode))) ||
                (nextPositionFound && (TLanguage(iLanguageCapabilities.iInputLanguageCode) == ELangThai)))
            {
            FindEndOfWord(nextCharPos);
            if(nextCharPos > iUncommittedText.HigherPos())
                {
                nextCharPos = iUncommittedText.HigherPos();
                }
            }
            
        }
    else if ( naviEvent == EBackspaceEvent )
        {
        if (iLanguageCapabilities.iRightToLeftLanguage)
            {
            nextPositionFound = GetNextVisualRightCharacter( nextCharPos );
            }
        else
            {
            if( (TAknFepUiIndicInputManager :: IsIndicLangauge(TLanguage(iLanguageCapabilities.iInputLanguageCode))) ||
                    (TLanguage(iLanguageCapabilities.iInputLanguageCode) == ELangThai))
                {
                TCursorSelection curSel;
                iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(curSel);
                nextCharPos = curSel.iCursorPos;        
                hindibackspace = ETrue;
                }
            else
                {
            nextPositionFound = GetNextVisualLeftCharacter( nextCharPos );
                }   
            }
        }

    if ( !nextPositionFound && !hindibackspace )
        {
        aComsumeKey = EFalse;
        return EFalse;
        }

    TInt anchorPos = 0;
    TInt cursorPos = 0;
    TTmDocPosSpec pos = DocPos();
    CTmTextLayout::TTmChunkDescription leftChunk;
    CTmTextLayout::TTmChunkDescription rightChunk;
    FindAdjacentChunks(pos, leftChunk, rightChunk);

    if (( naviEvent == ELeftNaviEvent && leftChunk.iRightToLeft ) ||
        ( naviEvent == ERightNaviEvent && !rightChunk.iRightToLeft ))
        {
        anchorPos = nextCharPos;
        cursorPos = nextCharPos;
        FindEndOfWord(cursorPos);
        }
    else
        {
        if( (TAknFepUiIndicInputManager :: IsIndicLangauge(TLanguage(iLanguageCapabilities.iInputLanguageCode))) ||
                (TLanguage(iLanguageCapabilities.iInputLanguageCode) == ELangThai))
            {
            cursorPos = nextCharPos;
            }
        else
            {
            cursorPos = nextCharPos + 1;    
            }
        anchorPos = nextCharPos;
        FindStartOfWord(anchorPos);
        }
        
#ifdef RD_INTELLIGENT_TEXT_INPUT    
	TCursorSelection curSel;
    iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(curSel);
    // if selection happens dont consume the event pass it to editor
    if((naviEvent == EBackspaceEvent)
            && (!IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction))
            && (curSel.iAnchorPos != curSel.iCursorPos))
        {
        aComsumeKey = EFalse;
        return EFalse;
        }
#endif

    iUncommittedText.SetSelection(cursorPos, anchorPos);

    if (iUncommittedText.Length() <= EMaximumFepWordLength)
        {
        iInputCapabilities.FepAwareTextEditor()->
            GetEditorContentForFep(aText, iUncommittedText.iAnchorPos, iUncommittedText.Length());
        aComsumeKey = ETrue;
        return ETrue;
        }
    else
        {
        if (naviEvent == EBackspaceEvent)
            {
            // delete the last character
            cursorPos--;
            iUncommittedText.SetSelection(cursorPos, cursorPos);
            aComsumeKey = EFalse;
            }
        else
            {
            TInt newCursorPosition= (pos.iPos == cursorPos) ? anchorPos : cursorPos;
            iUncommittedText.SetSelection(newCursorPosition, newCursorPosition);
            iInputCapabilities.FepAwareTextEditor()->SetCursorSelectionForFepL(iUncommittedText);
            aComsumeKey = ETrue;
            }
        // The word is longer than EMaximumFepWordLength. Ui is deactivated.
        TryCloseUiL();
        return EFalse;
        }
    }


TBool CAknFepManager::TryGetTextToUncommitL(TDes& aText, TBool& aComsumeKey)
    {
    TInt nextCharRightPos = 0;  // Character ordinal in logical memory (0 - n-1)
    TInt nextCharLeftPos = 0; 
    TBool nextRightPosFound = EFalse;
    TBool nextLeftPosFound = EFalse;
    
    nextRightPosFound = GetNextVisualRightCharacter( nextCharRightPos );
    if( (TAknFepUiIndicInputManager :: IsIndicLangauge(TLanguage(iLanguageCapabilities.iInputLanguageCode))) || 
            (TLanguage(iLanguageCapabilities.iInputLanguageCode) == ELangThai))
        {
        FindStartOfWord(nextCharRightPos);   
                    
        if(nextCharRightPos < iUncommittedText.LowerPos())
            {
            nextCharRightPos = iUncommittedText.LowerPos();
            }
        }
    
    
    nextLeftPosFound = GetNextVisualLeftCharacter( nextCharLeftPos );
    if( nextLeftPosFound && (TAknFepUiIndicInputManager :: IsIndicLangauge(TLanguage(iLanguageCapabilities.iInputLanguageCode))) ||
            (nextLeftPosFound && (TLanguage(iLanguageCapabilities.iInputLanguageCode) == ELangThai)))
        {
        FindEndOfWord(nextCharLeftPos);
        if(nextCharLeftPos > iUncommittedText.HigherPos())
            {
            nextCharLeftPos = iUncommittedText.HigherPos();
            }
        }
            



    if ( !nextRightPosFound && !nextLeftPosFound )
        {
        aComsumeKey = EFalse;
        return EFalse;
        }

    TInt anchorPos = 0;
    TInt cursorPos = 0;

    
    
    TTmDocPosSpec pos = DocPos();
    CTmTextLayout::TTmChunkDescription leftChunk;
    CTmTextLayout::TTmChunkDescription rightChunk;
    FindAdjacentChunks(pos, leftChunk, rightChunk);

    /*
    if( (TAknFepUiIndicInputManager :: IsIndicLangauge(TLanguage(iLanguageCapabilities.iInputLanguageCode))) ||
            (TLanguage(iLanguageCapabilities.iInputLanguageCode) == ELangThai))
        {
        cursorPos = pos.iPos;
        }
    else
        {
        cursorPos = pos.iPos + 1;    
        }
    */
    
    anchorPos = pos.iPos;
    cursorPos = pos.iPos;
    FindEndOfWord(cursorPos);          
    FindStartOfWord(anchorPos);
    
    
    if(cursorPos == pos.iPos || anchorPos == pos.iPos)
        {
        iUncommittedText.SetSelection(pos.iPos, pos.iPos);
        aComsumeKey = ETrue;
        return EFalse;
        }
    else
        {
        iUncommittedText.SetSelection(cursorPos, anchorPos);
        }  
    
    if (iUncommittedText.Length() <= EMaximumFepWordLength)
        {
        iInputCapabilities.FepAwareTextEditor()->
            GetEditorContentForFep(aText, iUncommittedText.iAnchorPos, iUncommittedText.Length());
        aComsumeKey = ETrue;
        return ETrue;
        }
    else
        {
        /*
        TInt newCursorPosition= (pos.iPos == cursorPos) ? anchorPos : cursorPos;
        iUncommittedText.SetSelection(newCursorPosition, newCursorPosition);
        iInputCapabilities.FepAwareTextEditor()->SetCursorSelectionForFepL(iUncommittedText);
        */
        aComsumeKey = ETrue;
        // The word is longer than EMaximumFepWordLength. Ui is deactivated.
        TryCloseUiL();
        return EFalse;
        }
    }

void CAknFepManager::UpdateCbaL(TInt aResourceId)
    {
    if ( !IsFepAwareTextEditor() )
        {
        return;
        }

    if (IsCcpuFlagSet(ECcpuStateStartCopy | ECcpuStateCopy | ECcpuStateStartCut | ECcpuStateCut))
        {
        return;
        }
    
    if((iAknEditorFlags & EAknEditorFlagFindPane)&& IsChineseInputLanguage()&& aResourceId != NULL)
        {
        return;
        }

	iUiInterface->UpdateSoftkeysL(aResourceId, EditorState(), this, 
                                iSharedDataInterface->InputMode());
    }


TBool CAknFepManager::TryRemoveNoMatchesIndicatorL()
    {
    __ASSERT_DEBUG(IsFlagSet(EFlagInsideInlineEditingTransaction), 
                   AknFepPanic(EAknFepPanicNotInsideInlineEdit));

    if (IsFlagSet(CAknFepManager::EFlagNoMatches))
        {
        ClearFlag(CAknFepManager::EFlagNoMatches); 
        TBuf<EMaximumFepWordLength> newText;
        iInputCapabilities.FepAwareTextEditor()->GetEditorContentForFep(newText, 
                                                                      iUncommittedText.iAnchorPos,
                                                                      iUncommittedText.Length());
        iInputCapabilities.FepAwareTextEditor()->UpdateFepInlineTextL(newText, 
                                                                      iUncommittedText.Length());
        SendEventsToPluginManL( EPluginSyncFepAwareText );
        return ETrue;
        }
    return EFalse;
    }

TBool CAknFepManager::IsMoreGoodWordsComing(TInt aNewWordLength) const
    {
    return (aNewWordLength == iUncommittedText.Length() + 1);
    }
#ifndef RD_INTELLIGENT_TEXT_INPUT
TBool CAknFepManager::IsMoreGoodAutoCompleteWordsComing(TInt aInputMode,TInt aPreviousWordLength, 
                                                        TInt aNewWordLength) const
	{
	return ((aInputMode == EPtiEngineWordCompletion) && (aPreviousWordLength != aNewWordLength ));	
	}
#endif

void CAknFepManager::TryStartCompoundWord()
    {
    // check if this word constitutes a 'compound' word
    // i.e. is either adjacent char is an alphabetic characeter
    TBuf<ESingleCharacter> ch;
    // check the previous character
    if(iUncommittedText.iAnchorPos>0)
        {
        iInputCapabilities.FepAwareTextEditor()->GetEditorContentForFep(ch, 
                                                                     iUncommittedText.iAnchorPos-1,
                                                                     ESingleCharacter);
        if (ch.Length()>0 && STATIC_CAST(TChar, IsValidInLineCharacter(ch[0])))
            {
            SetFlag(EFlagCompoundWord);
            }
        }

    // check the next character
    TInt edSize = iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep();
    if (!IsFlagSet(EFlagCompoundWord) && iUncommittedText.iAnchorPos < edSize)
        {
        iInputCapabilities.FepAwareTextEditor()->GetEditorContentForFep(ch, 
                                                                     iUncommittedText.iAnchorPos,
                                                                     ESingleCharacter);
        if (ch.Length()>0 && STATIC_CAST(TChar, IsValidInLineCharacter(ch[0])))
            {
            SetFlag(EFlagCompoundWord);
            }
        }
    }

void CAknFepManager::AddCompoundWordToUdbL()
    {
    __ASSERT_DEBUG(WesternPredictive() , AknFepPanic(EAknFepPanicNonPermittedEditorMode));

    HBufC* compoundWordBuf = 
                   HBufC::NewLC(iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep()); // buffer for text
    TPtr compoundWord=compoundWordBuf->Des();

    iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(iUncommittedText);

    // reset cursor span on word
    FindStartOfWord(iUncommittedText.iAnchorPos);
    FindEndOfWord(iUncommittedText.iCursorPos);

    iInputCapabilities.FepAwareTextEditor()->GetEditorContentForFep(compoundWord, 
                                                                    iUncommittedText.iAnchorPos, 
                                                                    iUncommittedText.Length());
    FepUI()->AddTextToUserDictionaryL(compoundWord);
    CleanupStack::PopAndDestroy();  // compoundWordBuf
    }

void CAknFepManager::SetWesternPredictive( const TBool aWesternPredictive )
    {
    const TInt previousValue = iWesternPredictive;
    iWesternPredictive = aWesternPredictive && !(iAknEditorFlags & EAknEditorFlagNoT9) &&
    // Predictive QWERTY (XT9) changes ---->
        (   
#ifdef RD_INTELLIGENT_TEXT_INPUT
        (
#endif //RD_INTELLIGENT_TEXT_INPUT
		iLanguageCapabilities.iSupportsWesternPredictive 
#ifdef RD_INTELLIGENT_TEXT_INPUT
        && !iQwertyInputMode)
         || (iLanguageCapabilities.iSupportsWesternQwertyPredictive && iQwertyInputMode) 
#ifdef __HALF_QWERTY_KEYPAD
         || (iLanguageCapabilities.iSupportsWesternHalfQwertyPredictive && iQwertyInputMode) 
#endif //__HALF_QWERTY_KEYPAD
#endif //RD_INTELLIGENT_TEXT_INPUT
		);
    // Predictive QWERTY (XT9) changes <----
    if ( (previousValue != iWesternPredictive) && !(iAknEditorFlags & EAknEditorFlagNoT9) )
        {
        if (iWesternPredictive)
            {
            iSharedDataInterface->SetPredictiveTextOn();
            }
        else
            {
            iSharedDataInterface->ResetPredictiveTextOn();
            }
        if ( iFepPluginManager && iFepPluginManager->IsSupportITIOnFSQ() )
        	{
            TRAP_IGNORE(SendEventsToPluginManL( EPluginUpdatePenInputITIStatus ));
        	}
        }
    }

void CAknFepManager::SetWesternAutoComplete( const TBool aWesternAutoComplete )
    {
    const TInt previousValue = iIsAutoCompleteOn;
    iIsAutoCompleteOn = aWesternAutoComplete && !(iAknEditorFlags & EAknEditorFlagNoT9);
    if ( (previousValue != iIsAutoCompleteOn) && !(iAknEditorFlags & EAknEditorFlagNoT9) )
        {
        if (iIsAutoCompleteOn)
            {
            iSharedDataInterface->SetPredictiveTextAutoCompleteOn();
            }
        else
            {
            iSharedDataInterface->ResetPredictiveTextAutoCompleteOn();
            }
        }
    }

void CAknFepManager::SetJapanesePredictive(const TBool aJapanesePredictive)
    {
    if (iJapanesePredictive != aJapanesePredictive)
        {
        iJapanesePredictive = aJapanesePredictive;
        }
    if (iJapanesePredictive & iSharedDataInterface->PredictiveTextJapaneseOn()
     || !iJapanesePredictive & !iSharedDataInterface->PredictiveTextJapaneseOn())
        {
        if(iJapanesePredictive)
            {
            iSharedDataInterface->SetJapanesePredictiveTextOn();
            }
        else
            {
            iSharedDataInterface->ResetJapanesePredictiveTextOn();
            }
        }
    }

void CAknFepManager::SetQwertyMode(const TBool aQwertyInputMode)
    {
#ifdef RD_SCALABLE_UI_V2
    if( aQwertyInputMode && iFepPluginManager && 
        iSharedDataInterface->PluginInputMode() != EPluginInputModeNone
        // Add this condition for addtion of ITI features on FSQ.
        // iSharedDataInterface->QwertyInputMode() == 1, means that 
        // hw keyboard is slided, then need to close touch inputmethods.
        && iSharedDataInterface->QwertyInputMode() )
        {
        iFepPluginManager->ResetMenuState(EFalse);        
        ExitPluginSpellModeByCancel();
        TRAP_IGNORE(iFepPluginManager->ClosePluginInputModeL( ETrue ));
        if ( iCancelPopupInQwerty )
            {
            TRAP_IGNORE( SimulateKeyEventL( EKeyCBA2 ) );
            }
        }
    SetQwertyModeToInputcapbility();
#endif//RD_SCALABLE_UI_V2
    if (!COMPARE_BOOLS(aQwertyInputMode, iQwertyInputMode))
        {
#ifndef RD_INTELLIGENT_TEXT_INPUT
        if(IsAutoCompleteOn())
        	{
            TRAP_IGNORE(RemoveSuggestedCompletionL());
            }
#endif //RD_INTELLIGENT_TEXT_INPUT
        iQwertyInputMode = aQwertyInputMode;
        if (!iQwertyInputMode)
            {
#ifdef RD_INTELLIGENT_TEXT_INPUT
            SetTypingCorrectionLevel(EPtiErrorCorrectionLevelHigh);
            SetNumberCandidateState(ETrue); // ETrue
            SetPrimaryCandidate(EFalse); // Best prediction
#endif            
            ClearFlag(EFlagQwertyShiftMode | EFlagNoActionDuringChrKeyPress);
            }
#ifdef RD_INTELLIGENT_TEXT_INPUT
        else
	        {
	        iSharedDataInterface->HandlePredictiveTextNumberCandidateSettingChange();
	        iSharedDataInterface->HandlePredictiveTextTypingCorrectionLevelChange();
	        iSharedDataInterface->HandlePredictiveTextPrimaryCandidateChange();	
	        }
#endif        

        if ( iFepManState != EAknFepStateNull && IsFepAwareTextEditor() )
            {
#ifdef RD_INTELLIGENT_TEXT_INPUT

			   // In case of layout remove the auto completion part
			   // Before transfer fep state to editor state.        
               if(IsAutoCompleteOn())
            	{
#ifdef RD_SCALABLE_UI_V2            	
                if( iFepPluginManager 
                        && iFepPluginManager->IsSupportITIOnFSQ()
                        && !iQwertyInputMode )
                    {
                    TRAP_IGNORE( iPtiEngine->HandleCommandL( EPtiCommandLockCurrentCandidate ) );
                    }
                else
#endif //RD_SCALABLE_UI_V2                	
                    {
                    TInt tailLength = 0;
                    TRAP_IGNORE(iPtiEngine->HandleCommandL( EPtiCommandGetAutoCompletionTailLength, &tailLength ));
                    if(tailLength > 0)
                        {
                        TRAP_IGNORE(
                            {
                            RemoveSuggestedAdvanceCompletionL();
                            iPtiEngine->HandleCommandL( EPtiCommandLockCurrentCandidate );
                            });                       
                        }
                    }
            	}
#endif 
            if ( !iFocusedItemDestroy )
            	{            	
                TRAP_IGNORE(
                    {
                    TransferFepStateToEditorL();
                    TryCloseUiL();
                    ConfigureFEPFromEditorStateL();
                    });                
            	}            
            }
        }
#ifdef RD_INTELLIGENT_TEXT_INPUT
    if(iFnKeyManager)
        iFnKeyManager->SetQwertyInputMode(aQwertyInputMode);
#endif
    }

TBool CAknFepManager::WesternPredictive(TInt aMode) const
    {
    TInt mode = iMode;
    if ( aMode )
        {
        mode = aMode;
        }
    return (mode == ELatin && iWesternPredictive &&
    // Predictive QWERTY (XT9) changes ---->
      
#ifdef RD_INTELLIGENT_TEXT_INPUT
        ((
#endif //RD_INTELLIGENT_TEXT_INPUT
		iLanguageCapabilities.iSupportsWesternPredictive 
#ifdef RD_INTELLIGENT_TEXT_INPUT
        && !iQwertyInputMode)
         || (iLanguageCapabilities.iSupportsWesternQwertyPredictive && iQwertyInputMode)
#ifdef __HALF_QWERTY_KEYPAD
		|| (iLanguageCapabilities.iSupportsWesternHalfQwertyPredictive && iQwertyInputMode)
#endif //__HALF_QWERTY_KEYPAD
		)
#endif //RD_INTELLIGENT_TEXT_INPUT
		);
    // Predictive QWERTY (XT9) changes <----
    }

TBool CAknFepManager::IsPredictive(TInt aMode) const
    {
    TBool isPredictive = EFalse;
    TInt mode = iMode;
    if ( aMode )
        {
        mode = aMode;
        }
    if (mode == EHiraganaKanji )
        {
        if ((iPermittedInputModes & EAknEditorHiraganaKanjiInputMode)
         && iJapanesePredictive
         && !(iAknEditorFlags & EAknEditorFlagNoT9)
         && HasJapanesePredictionInputMode())
            {
            isPredictive = ETrue;
            // Japanese predictive input is available on both Qwerty and not Qwerty
            return isPredictive;
            }
        }
    else if (mode == ELatin)
        {
        if ((iPermittedInputModes & (EAknEditorTextInputMode | EAknEditorHalfWidthTextInputMode))
          && !(iAknEditorFlags & EAknEditorFlagNoT9)
          && iWesternPredictive && 
          (iLanguageCapabilities.iSupportsWesternPredictive
#ifdef RD_INTELLIGENT_TEXT_INPUT
           ||iLanguageCapabilities.iSupportsWesternQwertyPredictive 
#ifdef __HALF_QWERTY_KEYPAD           
           ||iLanguageCapabilities.iSupportsWesternHalfQwertyPredictive
#endif //__HALF_QWERTY_KEYPAD           
#endif //RD_INTELLIGENT_TEXT_INPUT
		)) // Predictive QWERTY (XT9) changes
            {
            isPredictive = ETrue;
            }
        }
#ifdef RD_SCALABLE_UI_V2
	#ifdef RD_INTELLIGENT_TEXT_INPUT
	    return isPredictive; //&& (iFepPluginManager->PluginInputMode() == EPluginInputModeNone);
	#else
	    return isPredictive && !iQwertyInputMode 
             && ((iFepPluginManager->PluginInputMode() == EPluginInputModeItut) 
	    			|| (iFepPluginManager->PluginInputMode() == EPluginInputModeNone));
	#endif //RD_INTELLIGENT_TEXT_INPUT
#else
	#ifdef RD_INTELLIGENT_TEXT_INPUT
	    return isPredictive;
	#else
	    return isPredictive && !iQwertyInputMode;
	#endif //RD_INTELLIGENT_TEXT_INPUT
#endif //RD_SCALABLE_UI_V2
    }

TBool CAknFepManager::Japanese() const
    {
    TBool rtn = EFalse;
    switch (iMode)
        {
        case EHiraganaKanji:
        case EKatakana:
        case EHiragana:
            rtn = ETrue;
            break;
        case ELatin:
            if ((!WesternPredictive()) && iCharWidth == EFullWidthChar)
                {
                rtn = ETrue;
                }
            break;
        case ENumber:
            if (iCharWidth == EFullWidthChar)
                {
                rtn = ETrue;
                }
            break;
        default:
            break;
        }
    return rtn;
    }

TBool CAknFepManager::IsOnlyFullWidthCharacterPermitted() const
    {
    CAknEdwinState* editorState = EditorState();
    TUint permittedInputModes = editorState->PermittedInputModes();
    TUint fullWidthPermitted = (EAknEditorFullWidthTextInputMode
                              | EAknEditorFullWidthNumericInputMode
                              | EAknEditorFullWidthKatakanaInputMode);
    TUint fullWidthNotPermitted = ~fullWidthPermitted;
    TBool isfullchar = ETrue;
    if (permittedInputModes & fullWidthNotPermitted)
        {
        isfullchar = EFalse;
        }

    if (!(permittedInputModes & fullWidthPermitted))
        {
        isfullchar = EFalse;
        }

    return isfullchar;
    }

TBool CAknFepManager::IsOnlyHalfWidthCharacterPermitted() const
    {
    CAknEdwinState* editorState = EditorState();
    TUint permittedInputModes;
    if( editorState )
        {
        permittedInputModes= editorState->PermittedInputModes();
        }
    else
        {
        permittedInputModes = EAknEditorNumericInputMode;
        }
    TUint halfWidthNotPermitted = (EAknEditorFullWidthTextInputMode
                                 | EAknEditorFullWidthNumericInputMode
                                 | EAknEditorFullWidthKatakanaInputMode
                                 | EAknEditorHiraganaKanjiInputMode
                                 | EAknEditorHiraganaInputMode);

    TUint halfWidthPermitted  = ~halfWidthNotPermitted;
    TBool ishalfchar = ETrue;
    if (permittedInputModes & halfWidthNotPermitted)
        {
        ishalfchar = EFalse;
        }

    if (!(permittedInputModes & halfWidthPermitted))
        {
        ishalfchar = EFalse;
        }
    if (permittedInputModes & EAknEditorTextInputMode)
        {
        ishalfchar = EFalse;
        }
    if (editorState && (editorState->Flags() & EAknEditorFlagLatinInputModesOnly))
        {
        ishalfchar = ETrue;
        }
    return ishalfchar;
    }

TBool CAknFepManager::IsHalfAndFullKatakanaPermitted() const
    {
    CAknEdwinState* editorState = EditorState();
    TUint permittedInputModes = editorState->PermittedInputModes();
    TUint katakanaInputMode = (EAknEditorKatakanaInputMode
                              | EAknEditorFullWidthKatakanaInputMode);
    TBool iskatakana = EFalse;
    if ((permittedInputModes & katakanaInputMode) == katakanaInputMode)
        {
        iskatakana = ETrue;
        }
    return iskatakana;
    }

TBool CAknFepManager::IsHalfAndFullLatinPermitted() const
    {
    CAknEdwinState* editorState = EditorState();
    TUint permittedInputModes = editorState->PermittedInputModes();
    TUint latinInputMode = (EAknEditorHalfWidthTextInputMode
                              | EAknEditorFullWidthTextInputMode);
    TBool islatin = EFalse;

    if ((permittedInputModes & latinInputMode) == latinInputMode)
        {
        islatin = ETrue;
        }
    return islatin;
    }

TBool CAknFepManager::IsHalfAndFullNumberPermitted() const
    {
    CAknEdwinState* editorState = EditorState();
    TUint permittedInputModes = editorState->PermittedInputModes();
    TUint numInputMode = (EAknEditorNumericInputMode
                              | EAknEditorFullWidthNumericInputMode);
    TBool isnum = EFalse;
    if ((permittedInputModes & numInputMode) == numInputMode)
        {
        isnum = ETrue;
        }

    return isnum;
    }

TWidthChar CAknFepManager::CharacterWidth() const
    {
    return iCharWidth;
    }

CPtiEngine* CAknFepManager::PtiEngine() const
    {
    return iPtiEngine;
    }

void CAknFepManager::SetInlineEditingCursorVisibilityL(TBool aCursorVisibility)
    {
#ifdef RD_SCALABLE_UI_V2
    if ( SemiFepAwareTextEditor( ETrue ) )
#else
    if (iInputCapabilities.FepAwareTextEditor())
#endif // RD_SCALABLE_UI_V2
        {
        iInputCapabilities.FepAwareTextEditor()->SetInlineEditingCursorVisibilityL(aCursorVisibility);
        }
    }

TInt CAknFepManager::InputMode() const
    {
    return iMode;
    }
    
TInt CAknFepManager::CangJieMode() const
    {
    TInt shareMode = iSharedDataInterface->CangJieMode();
    TInt cangjieMode = shareMode + EPtiEngineNormalCangjieQwerty;
    return cangjieMode;
    }    

TBool CAknFepManager::InputLanguageSupportsCaseChanges() const
    {
    return iLanguageCapabilities.iSupportsCase;
    }

TBool CAknFepManager::IsAbleScrollLatinPredictive() const
    {
    return iSharedDataInterface->ScrollLatinPredictiveCandidates();
    }

TText CAknFepManager::PreviousChar( TBool /*aContextSensitive*/ )
    {
    TBuf<ESingleCharacter> previousCharacter(ESingleCharacter);
    previousCharacter.FillZ();

    TCursorSelection curSel;
    if ( IsFepAwareTextEditor() )
        {
        iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(curSel);
        TInt lowerpos = curSel.LowerPos();
        if(lowerpos > 0)
            {
            iInputCapabilities.FepAwareTextEditor()->
                GetEditorContentForFep(previousCharacter, lowerpos-1, ESingleCharacter);
            if (previousCharacter.Length())
                return previousCharacter[0];
            }
        }
    return NULL;
    }

TBool CAknFepManager::IsZWSCharacterPresent( TBool aLigaturePresent /* = EFalse*/ )
    {
    TBuf<3> previousCharacter;
    previousCharacter.Zero();
    TBool found = EFalse;
    previousCharacter.FillZ();

    TCursorSelection curSel;
    if ( IsFepAwareTextEditor() )
        {
        MCoeFepAwareTextEditor* fepAwareTextEditor = 
            iInputCapabilities.FepAwareTextEditor();
        fepAwareTextEditor->GetCursorSelectionForFep(curSel);
        TInt lowerpos = curSel.LowerPos();
        if( lowerpos > 1 )
            {
            if( aLigaturePresent && lowerpos > 3 )
                {
                fepAwareTextEditor->
                    GetEditorContentForFep(previousCharacter, 
                                            lowerpos - 4, 2);
                }
            else
                {
                fepAwareTextEditor->
                    GetEditorContentForFep(previousCharacter, 
                                            lowerpos - 2, 2);
                }
            if( previousCharacter[0] == ZERO_WIDTH_SPACE
                     || previousCharacter[1] == ZERO_WIDTH_SPACE )
                {
                found = ETrue;
                }   
            }
        }
    return found;
    }

TText CAknFepManager::NextChar()    
    {
    TBuf<ESingleCharacter> nextCharacter(ESingleCharacter);
    nextCharacter.FillZ();

    TCursorSelection curSel;
    if ( IsFepAwareTextEditor() )
        {
        iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(curSel);
        TInt higherpos = curSel.HigherPos();
        TInt maxLength = iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep();
        if ( higherpos >= 0 && higherpos <= maxLength && maxLength >= 1 ) 
            {
            iInputCapabilities.FepAwareTextEditor()->
                GetEditorContentForFep(nextCharacter, higherpos, ESingleCharacter);
            if( nextCharacter.Length() )    
                return nextCharacter[0];
            }
        }
    return NULL;    
    }
    
void CAknFepManager::FindStartOfWord(TInt& aAnchorPos) const
    {
    TBuf<ESingleCharacter> characterBuffer;
    FOREVER
        {
        if (aAnchorPos<=0)
            {
            break; // start of editor
            }
        iInputCapabilities.FepAwareTextEditor()->GetEditorContentForFep(characterBuffer, 
                                                                        aAnchorPos-1, 
                                                                        ESingleCharacter);
        if (!IsValidInLineCharacter(characterBuffer[0]))
            {
            break;
            }
        aAnchorPos--;
        }
    }

void CAknFepManager::FindEndOfWord(TInt& aCursorPos) const
    {
    TBuf<ESingleCharacter> characterBuffer;
    FOREVER
        {
        if (aCursorPos>=iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep())
            {
            break; // end of editor
            }
        iInputCapabilities.FepAwareTextEditor()->GetEditorContentForFep(characterBuffer, 
                                                                        aCursorPos, 
                                                                        ESingleCharacter);
        if (!IsValidInLineCharacter(characterBuffer[0]))
            {
            break;
            }
        aCursorPos++;
        }
    }

TBool CAknFepManager::CursorInsideWord()
    {
    TBuf<ESingleCharacter> characterBuffer;
    TBool insideWord=ETrue;

    iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(iUncommittedText);
    TInt cursorPos=iUncommittedText.iCursorPos;

    if (cursorPos)
        {
        // check character before cursor
        iInputCapabilities.FepAwareTextEditor()->GetEditorContentForFep(characterBuffer, 
                                                                        cursorPos-1, 
                                                                        ESingleCharacter);
        if (characterBuffer.Length())
			{
            if (!IsValidInLineCharacter(characterBuffer[0]))
                {
                insideWord=EFalse;
                }
			}	
        }
    else // at start of editor
        {
        insideWord=EFalse;
        }

    if (insideWord && cursorPos<iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep())
        {
        // put character after cursor into buffer
        iInputCapabilities.FepAwareTextEditor()->GetEditorContentForFep(characterBuffer, cursorPos,
                                                                        ESingleCharacter);
        if (characterBuffer.Length())
			{
            if (!IsValidInLineCharacter(characterBuffer[0]))
                {
                insideWord=EFalse;
                }
			}	
        }
    else // at end of editor
        {
        insideWord=EFalse;
        }

    return insideWord;
    }

void CAknFepManager::MoveCursorToStartOfWordL()
    {
    TInt anchorPos=iUncommittedText.iAnchorPos;
    FindStartOfWord(anchorPos);
    iUncommittedText.SetSelection(anchorPos, anchorPos);
    iInputCapabilities.FepAwareTextEditor()->SetCursorSelectionForFepL(iUncommittedText);
    }

void CAknFepManager::MoveCursorToEndOfWordL()
    {
    TInt cursorPos=iUncommittedText.iCursorPos;
    
    if(!IsExtendedFlagSet(EExtendedFlagOkKeyPressOnCandidatePopup))    
    	FindEndOfWord(cursorPos);
    iUncommittedText.SetSelection(cursorPos, cursorPos);
    iInputCapabilities.FepAwareTextEditor()->SetCursorSelectionForFepL(iUncommittedText);
    }

TKeyResponse CAknFepManager::HandleNaviEventOutsideInlineEditL(TUint aCode, 
                                                               TKeyPressLength aLength)
    {
    TKeyResponse response = EKeyWasNotConsumed;
    if ( iFepPluginManager && iFepPluginManager->PluginInputMode() == EPluginInputModeFingerHwr 
    	 && aCode == EKeyBackspace )
    	{
        return response;
    	}
    
    if (TryHandleCommonArrowAndBackspaceFunctionalityL(aCode, response))
        {
        return response;
        }
    
    // You will not understand the flowing lines, 
    // for it is just a "hole stem" than normal logic.
    //
    // Let me explain: 
    // In HWR, 4x12 keyboard is used, and predictive should be disabled for it.
    // unfortunately, the existed codes enable predictive in some special case.
    // when you switch from FSQ to HWR, with ITI on, you will get that.
    // then if you press arrow key, 
    // the code blow of "HandlePredictiveNaviEventOutsideInlineEditL" will be called.
    // which is writen for FSQ, not HWR, that finally cause navi-event being consumed.
    // No navigation action being performed.
    //
    // It is not a complete fix, just for simplicity. 
    if (iFepPluginManager && 
        iFepPluginManager->PluginInputMode() == EPluginInputModeFingerHwr)
        {
        return EKeyWasNotConsumed;
        }

    if (WesternPredictive())
        {
        response = HandlePredictiveNaviEventOutsideInlineEditL(aCode, aLength);
        }

    return response;
    }

TKeyResponse CAknFepManager::HandleWesternPredictiveArrowKeyL(TInt aCode, TKeyPressLength aLength)
    {
    TKeyResponse response = EKeyWasNotConsumed;
    
    if (IsFlagSet(EFlagLineFeedCharacter))
        {
        // This event will stop the multitapping timer for enter chr.
        // just return.
        return EKeyWasConsumed;                
        }

	TCursorSelection selection;
    iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(selection);
    if(!IsMfneEditor() &&
         ( iFepPluginManager->PluginInputMode() == EPluginInputModeItut) &&
         selection.Length() > 0)
        {
        // If open virtual ITUT and highlight some chars
        return EKeyWasConsumed;
        }        
    if (aCode == EKeyUpArrow || aCode == EKeyDownArrow)
        {
        ClearFlag(EFlagCompoundWord);
        if (IsFlagSet(EFlagInsideInlineEditingTransaction))
            {
            // moving out of an inline edit
            TryRemoveNoMatchesIndicatorL();
            UpdateCbaL(NULL);
            // To rip off suggested word completion when user presses UP / DOWN arrow keys
            if(IsAutoCompleteOn())
           		{
#ifndef RD_INTELLIGENT_TEXT_INPUT
           		
           		RemoveSuggestedCompletionL();
           		
#else
           		TInt tailLength = 0;
           		iPtiEngine->HandleCommandL( EPtiCommandGetAutoCompletionTailLength, 
										&tailLength );
           		if(tailLength > 0)
           			{
           			RemoveSuggestedAdvanceCompletionL();
           			return EKeyWasConsumed;
           			}
#endif          	
            	}
            if (IsFlagSet(EFlagShiftKeyDepressed) || IsCcpuFlagSet(ECcpuStateHashDown))
                {
                CommitInlineEditL();
                TryCloseUiL();
                SimulateKeyEventL(EKeyF19);  // Asyncronous case update
                // pass to editor for text selection.
                response=EKeyWasNotConsumed;
                }
            }
		if ((iInputCapabilities.FepAwareTextEditor() && !TryHandleTextSelectedNaviEventL(aCode, response)) && !IsFlagSet(EFlagShiftKeyDepressed))
            {
            if (IsFlagSet(EFlagInsideInlineEditingTransaction))
                {
                ClearFlag(CAknFepManager::EFlagNoMatches);
                TryRemoveNoMatchesIndicatorL();
                CommitInlineEditL();
                }
            UpdateCbaL(NULL);

            // simulate a designated key event to catch and check if we're inside a word
            if (aCode == EKeyDownArrow)
                {
                SimulateKeyEventL(EKeyF22);
                }
            else
                {
                SimulateKeyEventL(EKeyF23);
                }
            // pass to editor for text selection.
            response=EKeyWasNotConsumed;
            }
        else if (IsFlagSet(EFlagShiftKeyDepressed) || IsCcpuFlagSet(ECcpuStateHashDown))
            {
            // Need to update case indicator if text selection is made upwards.
            SimulateKeyEventL(EKeyF19);
            }
        }
    else // right or left
        {
        if (IsFlagSet(EFlagInsideInlineEditingTransaction))
            {
            // moving out of an inline edit
#ifndef RD_INTELLIGENT_TEXT_INPUT
                if(IsAutoCompleteOn())
                	{
                	if((aCode == EKeyLeftArrow && 
                	    		CurrentInputDirectionality() == TBidiText::ELeftToRight) || 
                	    (aCode == EKeyRightArrow && 
                	    		CurrentInputDirectionality() == TBidiText::ERightToLeft))
                		{
                		RemoveSuggestedCompletionL();	
                		}
                	}
#else
            if(IsAutoCompleteOn())
            	{
                TInt tailLength = 0;
            	iPtiEngine->HandleCommandL( EPtiCommandGetAutoCompletionTailLength, &tailLength );
            	// In case the inline editing is ongoing and there is a word completion
                // currently shown, the first backwards tap will remove the suggested 
                // completion. 
                if(tailLength > 0)
	                {
	                if (((CurrentInputDirectionality()==TBidiText::ERightToLeft) && aCode == EKeyLeftArrow) 
	                	|| ((CurrentInputDirectionality()==TBidiText::ELeftToRight) && aCode == EKeyRightArrow ))
	                    {
					    iPtiEngine->HandleCommandL( EPtiCommandLockCurrentCandidate);
					    TPtrC selectedWord = iPtiEngine->CurrentWord();
                        UpdateInlineEditL( selectedWord, selectedWord.Length() );  //selection.iAnchorPosition
                        iPtiEngine->CommitCurrentWord();
                        TryCloseUiL();
                        // Asyncronous case update after the editor has handled the key
                		SimulateKeyEventL(EKeyF19); 
	                    response=EKeyWasConsumed;
	                    }
	                else if(((CurrentInputDirectionality()== TBidiText::ERightToLeft ) && aCode == EKeyRightArrow) 
	                	|| ((CurrentInputDirectionality()== TBidiText::ELeftToRight) && aCode == EKeyLeftArrow )) 
		                {
	                    RemoveSuggestedAdvanceCompletionL();
	                    response=EKeyWasConsumed;
		                } 
			           return response;   	
	                }
	                else if (!IsQwerty())
		                {
		                iPtiEngine->CommitCurrentWord();
		                }
				
            	}
#endif // RD_INTELLIGENT_TEXT_INPUT
            TryRemoveNoMatchesIndicatorL();
            UpdateCbaL(NULL);
            if (IsFlagSet(EFlagShiftKeyDepressed) || IsCcpuFlagSet(ECcpuStateHashDown))
                {
#ifndef RD_INTELLIGENT_TEXT_INPUT
                 if(IsAutoCompleteOn()) // To rip off suggested completion when shift key is pressed 
                 	{
                 	RemoveSuggestedCompletionL();
                	}
#endif //RD_INTELLIGENT_TEXT_INPUT
                CommitInlineEditL();
                TryCloseUiL();
                SimulateKeyEventL(EKeyF19);  // Asyncronous case update
                // pass to editor for text selection.
                response=EKeyWasNotConsumed;
                }
            else
                {
                TWesternNaviEvent naviEvent = ELeftNaviEvent;
                if (aCode == EKeyRightArrow)
                    {
                    naviEvent = ERightNaviEvent;
                    }
                // Find the visual left/right end of the current word
                TTmDocPosSpec pos;
                GetAvkonDefinedVisualLimitsOfSelection(
                    iUncommittedText, naviEvent, pos, NULL );

                if (IsCcpuFlagSet(ECcpuStateNewPredictiveWord))
                    {
                    // This is new predictive word which is being committed with arrow
                    // key. Force ::TryCloseUiL to commit PtiEngie word to keep
                    // frequency information in sync.
                    SetCcpuFlag(ECcpuStataCommitPredictiveWord);
                    }

                TryCloseUiL();

                // Position should be dis-ambiguated based on last navigation and direction of blocks.
                AdjustPosSpecForAmbiguityAfterLeftOrRightNavigation( pos, naviEvent );

                SetCursorPositionL( pos );
                if (CursorInsideWord() && naviEvent == ERightNaviEvent)
                {
                    MoveCursorToEndOfWordL();
                }

                iCaseMan->UpdateCase(ENullNaviEvent);
                response=EKeyWasConsumed;
                }
            }
        else
            {
#ifndef RD_INTELLIGENT_TEXT_INPUT            
            if ( iLanguageCapabilities.iInputLanguageCode == ELangThai ) // Without this, t9 goes inside words
                {                               // That is not allowed in Thai
                response = EKeyWasNotConsumed;
                }
            else
#endif //RD_INTELLIGENT_TEXT_INPUT
            response = HandleNaviEventOutsideInlineEditL(aCode, aLength);
            if (response == EKeyWasNotConsumed)
                {
                // moving over a space
                if (aCode == EKeyLeftArrow && !IsFlagSet(EFlagShiftKeyDepressed))
                    {
                    iCaseMan->UpdateCase(ELeftNaviEvent);
                    }
                else if (!IsFlagSet(EFlagShiftKeyDepressed)) // EKeyRightArrow
                    {
                    iCaseMan->UpdateCase(ERightNaviEvent);
                    }
                }
            else
                iCaseMan->UpdateCase(ENullNaviEvent);
            }
        }
    return response;
    }

TBool CAknFepManager::TryHandleTextSelectedNaviEventL(TInt aCode, TKeyResponse& response)
    {
    iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(iUncommittedText);

    if (!IsFlagSet(EFlagShiftKeyDepressed) && iUncommittedText.Length())
        {
        if (aCode == EKeyBackspace)
            {
            iCaseMan->UpdateCase(EBackspaceEvent);
            response = EKeyWasNotConsumed;
            }
        else
            {
            TInt newCursorPos = iUncommittedText.HigherPos(); //default, right or down
            if (aCode == EKeyLeftArrow || aCode == EKeyUpArrow)
                {
                newCursorPos = iUncommittedText.LowerPos();
                }
            iUncommittedText.SetSelection(newCursorPos, newCursorPos);
            iInputCapabilities.FepAwareTextEditor()->SetCursorSelectionForFepL(iUncommittedText);

            if (WesternPredictive())
                {
                if (CursorInsideWord())
                    {
                    if (aCode == EKeyLeftArrow || aCode == EKeyUpArrow)
                        {
                        MoveCursorToStartOfWordL();
                        }
                    else //right or down
                        {
                        MoveCursorToEndOfWordL();
                        }
                    }
                iCaseMan->UpdateCase(ENullNaviEvent);
                }
            response = EKeyWasConsumed;
            }
        return ETrue;
        }
    return EFalse;
    }

TBool CAknFepManager::TryHandleCommonArrowAndBackspaceFunctionalityL(TInt aCode, 
                                                                     TKeyResponse& aResponse)
    {
    TBool keyHandled = ETrue;
    if ( !IsFepAwareTextEditor() )
        {
        aResponse=EKeyWasNotConsumed;
        return keyHandled;
        }
#ifdef RD_HINDI_PHONETIC_INPUT        
	if(iLanguageCapabilities.iInputLanguageCode == KLangHindiPhonetic)
		{
		if (aCode == EKeyBackspace)
        	{
        	CommitInlineEditL();
        	FepUI()->HandleKeyL(aCode, EShortKeyPress);
        	}
		if(IsFlagSet(CAknFepManager::EFlagNoMatches)) 
			{
			if(PreviousChar() == 0x003F && aCode != EKeyBackspace)
				{
				RemovePreviousCharacterL();		
				}
			ClearFlag(CAknFepManager::EFlagNoMatches);
			FepUI()->ExpireMultitapTimer();
			}
		iPtiEngine->HandleCommandL(EPtiCommandBreakSyllable, NULL );
		}
#endif		
    if (IsFlagSet(EFlagInsideMultitapInlineEditingTransaction))
        {
        FepUI()->ExpireMultitapTimer();

        if (aCode == EKeyBackspace)
            {
            aResponse = EKeyWasNotConsumed; // let editor handle the move
            }
        else
            {
            aResponse = EKeyWasConsumed;
            }
        }
    else
        {
        //selecting text
        if (IsFlagSet(EFlagShiftKeyDepressed) || IsCcpuFlagSet(ECcpuStateCopy | ECcpuStateCut))
            {
            if (iShiftKeypressMonitor->IsActive())
                {
                ResetShiftKeyMonitorL();    //don't want edit menu
                }
            ClearFlag(EFlagQwertyShiftMode);
            SimulateKeyEventL(EKeyF19);  // Asyncronous case update
            aResponse = EKeyWasNotConsumed;
            return keyHandled;
            }

        TCursorSelection currentEditorSelection;
        iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(currentEditorSelection);

        TWesternNaviEvent naviEvent = ENullNaviEvent;

        // Translate key code to internal navi-event
        switch( aCode )
            {
            case EKeyUpArrow:
                naviEvent = EUpNaviEvent;
                break;
            case EKeyDownArrow:
                naviEvent = EDownNaviEvent;
                break;
            case EKeyRightArrow:
                naviEvent = ERightNaviEvent;
                break;
            case EKeyLeftArrow:
                naviEvent = ELeftNaviEvent;
                break;
            default: // just leave it as NULL
                break;
            }

        if (currentEditorSelection.Length())
            {
            // we have text selected - set cursor to start/end of current selection
            TTmDocPosSpec newPos;
            aResponse = NavigateFromSelectionL( currentEditorSelection, naviEvent, newPos );
            
            // You will not understand the following lines, 
            // for it is just a "hole stem" than normal logic.
            //
            // Let me explain: 
            // In HWR, 4x12 keyboard is used, and predictive should be disabled for it.
            // unfortunately, the existing codes enable predictive in some special case.
            // when you switch from FSQ to HWR, with ITI on, you will get that.
            // then if you select some words and press arrow key, 
            // the if bratch of the code blow will be excuted.
            // which is writen for FSQ, not HWR, 
            // MoveCursorToEndOfWordL or MoveCursorToStartOfWordL is called unexpected
            //
            // It is not a complete fix, just for simplicity.
            TBool isFingerHwr = ( iFepPluginManager != NULL ) && 
								( iFepPluginManager->PluginInputMode() == EPluginInputModeFingerHwr );

            if ( aCode != EKeyBackspace && !isFingerHwr && WesternPredictive() )
                {
                if (CursorInsideWord())
                    {
                    TBool rightToleftParagraph = IsRightToLeftParagraph(newPos);
                    if ( (rightToleftParagraph && naviEvent == ELeftNaviEvent) ||
                         (!rightToleftParagraph && naviEvent == ERightNaviEvent) )
                        {
                        MoveCursorToEndOfWordL();
                        }
                    else
                        {
                        MoveCursorToStartOfWordL();
                        }
                    }

                }
            }
        else if (!(EditorState()->Flags() & EAknEditorFlagNoLRNavigation))
            {
            if ((naviEvent != ENullNaviEvent) && HandleLoopingInNaviEventL(naviEvent))
                {
                aResponse=EKeyWasConsumed;
                }
            else
                {
                if (iMode == ELatin)
                    {
                    // normal operation of Down/Up scroll keypress action.
                    switch( aCode )
                        {
                        case EKeyUpArrow:
                        case EKeyDownArrow:
                            SimulateKeyEventL(EKeyF22);
                            break;
                        case EKeyRightArrow:
                        case EKeyLeftArrow:
                            {
                            /*Hindi*/
                            if( (TAknFepUiIndicInputManager :: IsIndicLangauge(
                                      TLanguage(iLanguageCapabilities.iInputLanguageCode))) 
                                 && IsZWSCharacterPresent() )
                                {
                                RemoveZWSCharacterL(EKeyLeftArrow); 
                                }
                            }
                            break;
                        default: // nothing
                            break;
                        }
                    }
                keyHandled = EFalse;
                }
            }
        }

    return keyHandled;
    }

/**
* Routine handled navigation and also backspace events at "beginning" of document
*
*/
TBool CAknFepManager::HandleLoopingInNaviEventL(TWesternNaviEvent aNaviEvent)
    {
    TBool looping = EFalse;
    TBool eventHandled = EFalse;
    TTmDocPosSpec newCursorPos;

    // This routine may handle Left, Right and Backspace and nothing else
    // Note that Up and Down cannot do any sort of document looping in FEP
    if ( aNaviEvent != ELeftNaviEvent
        && aNaviEvent != ERightNaviEvent
        && aNaviEvent != EBackspaceEvent )
        return eventHandled; // not consumed

    TTmDocPosSpec pos = DocPos();
    TBool rightToLeftParagraph = IsRightToLeftParagraph(pos);

    TWesternNaviEvent incrementingNaviEvent = 
                                          rightToLeftParagraph ? ELeftNaviEvent : ERightNaviEvent;
    TWesternNaviEvent decrementingNaviEvent = 
                                          rightToLeftParagraph ? ERightNaviEvent : ELeftNaviEvent;

    if (aNaviEvent == decrementingNaviEvent)
        {
        // Abandon early if we are not in the first line.
        if ( InFirstLineAndFormatted( pos ) )
            {
            TTmDocPosSpec startPos;
            GetVisualDocStart( startPos );
             if( AtVisualStart( pos, startPos ) )
                {
                TTmDocPosSpec endPos;
                if ( !GetVisualDocEnd( endPos ) )
                    {
                    // End of document is not formatted. Value returned is just pos at DocLength.
                    // So actually move the cursor there (this is guranteed to format the last line)
                    // and the end End position is found a 2nd time.
                    newCursorPos = endPos;
                    SetCursorPositionL( newCursorPos );
                    GetVisualDocEnd( endPos );
                    }
                newCursorPos = endPos;
                looping = ETrue;
                }
            }
        }
    else if (aNaviEvent == incrementingNaviEvent )
        {
        if ( InLastLineAndFormatted( pos ) )
            {
            TTmDocPosSpec endPos;
            GetVisualDocEnd( endPos );
            if( AtVisualEnd( pos, endPos, 
                             iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep() ) )
                {
                TTmDocPosSpec startPos;
                if ( !GetVisualDocStart( startPos ) )
                    {
                    // Document was not formatted at docPos(0).  So actually move the cursor
                    // to that line at least, and then ask for the start pos again.
                    newCursorPos = startPos;
                    SetCursorPositionL( newCursorPos );
                    GetVisualDocStart( startPos );
                    }
                newCursorPos = startPos;
                looping = ETrue;
                }
            }
        }
    else if ( aNaviEvent == EBackspaceEvent )
        {
        // Deletion is always logical, so the condition is simply to test the logical pos.
        if ( pos.iPos ==0 )
            {
            eventHandled = ETrue;
            }
        }

    if (looping)
        {
        // This takes language issues into account, adjusting the cursor position:
        AdjustPosSpecForInputLanguageAfterDocumentLoop( newCursorPos, aNaviEvent, 
                                                        (aNaviEvent == incrementingNaviEvent) );
        SetCursorPositionL( newCursorPos );
        eventHandled = ETrue;
        }
    return eventHandled;
    }

void CAknFepManager::TransferFepStateToEditorL( TBool aFnLockSync )
    {
    // If editor in Fnlock state, need to set actual input mode
    // because, in case of qwerty keypad alphanumeric editor there is no numeric mode
    // numeric mode handle as FnLock
#ifdef RD_INTELLIGENT_TEXT_INPUT

   if( aFnLockSync && iQwertyInputMode && iFnKeyManager && iFnKeyManager->FnKeyState()==CAknFepFnKeyManager::EFnKeyLock)
        {
        EditorState()->SetCurrentInputMode(EditorModeFromFepMode(ENumber));
        // Indicator should not be reset, if menu is displaying from editor.
        if(!IsFlagSet(EFlagForegroundUIComponentVisible | EFlagMenuPaneVisible))
            iFnKeyManager->ClearFnKeyState(); 
        }
    else
        {
#endif
        EditorState()->SetCurrentInputMode(EditorModeFromFepMode(iMode));
        
#ifdef RD_INTELLIGENT_TEXT_INPUT        
        }
   if(iFnKeyManager && iFnKeyManager->FnKeyState()!=CAknFepFnKeyManager::EFnKeyLock)
       iFnKeyManager->ClearFnKeyState();       

#endif

    
    if ((iMode == ELatin && iWesternPredictive &&
        IsFlagSet(EFlagInsideInlineEditingTransaction)) 
#ifdef RD_INTELLIGENT_TEXT_INPUT
        ||(IsLanguageSupportPrediction() && IsFlagSet(EFlagInsideInlineEditingTransaction))
#endif //RD_INTELLIGENT_TEXT_INPUT
		)
        {
        EditorState()->SetInlineEditSpan(iUncommittedText);
#ifdef RD_INTELLIGENT_TEXT_INPUT
        if (IsFlagSet(CAknFepManager::EFlagNoMatches))
#endif //RD_INTELLIGENT_TEXT_INPUT
        TryRemoveNoMatchesIndicatorL();
        UpdateCbaL(NULL);
        }
    else
        {
        EditorState()->SetInlineEditSpan(TCursorSelection(0,0));
        }
    EditorState()->SetCurrentCaseWithNoInitialisation(iCaseMan->CurrentCase());
    if ( IsFlagSet( EFlagSupressAutoUpdate ) )
        {
        EditorState()->SetFlags( EditorState()->Flags() | EFlagSupressAutoUpdate );
        }
    else
        {
        EditorState()->SetFlags( EditorState()->Flags() & ~EFlagSupressAutoUpdate );
        }
    SetQwertyModeToInputcapbility();
    }

MAknFepManagerInterface* CAknFepManager::FepUI() const
    {
#ifdef RD_SCALABLE_UI_V2
    if ( iFepFullyConstructed && ( iFepPluginManager->PluginInputMode() != EPluginInputModeNone ) )
        {
        if (iFepPluginManager->PluginInputMode() == EPluginInputModeItut)
            {
            return iFepPluginManager->CurrentFepInputUI() ?
                iFepPluginManager->CurrentFepInputUI() : iCurrentFepUI;
            }
        if ( iFepPluginManager->EnableITIOnFSQ() )
            {
            return iCurrentFepUI;
            }
        return iFepPluginManager->CurrentPluginInputFepUI();
        }
#endif        
    return iCurrentFepUI;
    }

void CAknFepManager::LaunchLanguagesPopupListL(TBool aLaunchedByTouchWin)
    {
#ifdef RD_SCALABLE_UI_V2
    if( iFepPluginManager->PluginInputMode() != EPluginInputModeNone &&
        iFepPluginManager->PluginInputMode() != EPluginInputModeItut
        // Add this condition for correction of dim features on FSQ
        && iFepPluginManager->PluginInputMode() != EPluginInputModeFSQ
        && iFepPluginManager->PluginInputMode() != EPluginInputModePortraitFSQ )
        {
        iFepPluginManager->LaunchPenInputLanguageSelectionL(aLaunchedByTouchWin);
        return;
        }
#endif //RD_SCALABLE_UI_V2
       
    CArrayFixFlat<TInt>* languages = 
        new( ELeave )CArrayFixFlat<TInt>( ELanguageArrayGranularity );
    CleanupStack::PushL( languages );
    iPtiEngine->GetAvailableLanguagesL( languages );

    // Create a list of languages for the dialog
    CDesCArrayFlat* languagesList = 
        new( ELeave )CDesCArrayFlat( ELanguageArrayGranularity );
    CleanupStack::PushL( languagesList );

    // Item format should be "<icon_index>\t<item_text>"
    _LIT( KIconIndexSelected,    "0\t" );
    _LIT( KIconIndexNotSelected, "\t" );// No icon
    
    MPtiLanguage* lang = NULL;
    TInt  currentLanguage = iSharedDataInterface->InputTextLanguage();
    TInt selectedLanguageIndex = 0;
    
    for (TInt ii=0; ii < languages->Count(); ii++)
        {
        lang = iPtiEngine->GetLanguage( ( *languages )[ii] );
        if (!lang)
            {
            User::Leave( KErrCorrupt );
            }
        // Buffer should contain both tabulator string and localized name:
        HBufC* langStrBuf = HBufC::NewL( lang->LocalizedName().Length() + 3 );
        CleanupStack::PushL( langStrBuf );
        
        TPtr langStrPtr = langStrBuf->Des();

        // If language is current language - use selected icon in lbx:
        if( lang->LanguageCode() == currentLanguage )
            {
            langStrPtr.Append( KIconIndexSelected );
            selectedLanguageIndex = ii;
            }
        else
            {
            langStrPtr.Append( KIconIndexNotSelected );    
            }
        langStrPtr.Append( lang->LocalizedName() );
        languagesList->AppendL( langStrPtr );
        CleanupStack::PopAndDestroy( langStrBuf );
        }

    // Add icons to the listbox:
    CArrayPtr<CGulIcon>* icons = new( ELeave ) CArrayPtrFlat<CGulIcon>(10);
    CleanupStack::PushL( icons );
    LoadIconL( icons, EMbmAvkonQgn_prop_sub_current, 
               EMbmAvkonQgn_prop_sub_current_mask );

    TInt index = 0;
    SetFlag( EFlagForegroundUIComponentVisible );
    CleanupStack::Pop( icons );	// iUiInterface->LaunchListPopupL takes ownership immediately
#ifdef RD_SCALABLE_UI_V2 
    if( iFepFullyConstructed && iFepPluginManager && !iFepPluginManager->IsInMenuOpen() )
            {
            iFepPluginManager->SetMenuState();            
            }
#endif              
    // Fire up the dialog
#ifdef RD_SCALABLE_UI_V2
    iCancelPopupInQwerty = aLaunchedByTouchWin;
#endif //RD_SCALABLE_UI_V2
    TUid fepUid = CCoeEnv::Static()->FepUid();
    ClearExtendedFlag(EExtendedFlagEdwinEditorDestroyed);
	SendEventsToPluginManL(EPluginEnablePriorityChangeOnOriChange,EFalse);
    TInt returnValue = iUiInterface->LaunchListPopupL(R_AVKON_T9_LANGUAGES_QUERY, index, 
                                                      selectedLanguageIndex, languagesList, icons);
#ifdef RD_SCALABLE_UI_V2
    iCancelPopupInQwerty = EFalse;
#endif //RD_SCALABLE_UI_V2    
    PrepareFepAfterDialogExitL(fepUid);
    CleanupStack::PopAndDestroy( languagesList );

    if(returnValue == EAknSoftkeyOk)
        {
        // Selected item index from the languages array
        HandleChangeInFocusL();

        // The user has changed language and the local language is not used any more.
        iLanguageCapabilities.iLocalInputLanguageInUse = EFalse;
        if (EditorState())
            {
            EditorState()->SetLocalLanguage( ELangTest );
            }

        TBool westernPredictiveInUse = WesternPredictive();

// Truncate autocompletion portion on change of writing language
#ifdef RD_INTELLIGENT_TEXT_INPUT
		RemoveSuggestedAdvanceCompletionL();
#endif // RD_INTELLIGENT_TEXT_INPUT   
        //Hindi Change Bug Fix
        iSharedDataInterface->SetInputTextLanguage(languages->At(index));
        CAknEnv::Static()->SettingCache().Update(KEikInputLanguageChange);
        SetActiveInputLanguage( languages->At(index) );
        ChangeInputLanguageL(languages->At(index));           
        SetFlag( EFlagNewSharedDataInputLanguage | EFlagNewSharedDataInputMode );
	  	// This pice of code TSW Fute error fixing.
        // Display the confirmation msg if the dictionary is not available for
        // Current language.
#ifdef RD_INTELLIGENT_TEXT_INPUT         
        // Display the confirmation msg only for Western languages
        if (!IsChineseInputLanguage())
        	{
	        if (!IsLanguageSupportPrediction())
	        	{
	        	LaunchConfirmationNoteL(R_AVKON_T9_NOTE_NO_AVAILABLE);
	        	}     
        	}
#endif
        
          
        }
	SendEventsToPluginManL(EPluginEnablePriorityChangeOnOriChange,ETrue);	
    CleanupStack::PopAndDestroy( languages );
    }

void CAknFepManager::LaunchRecognitionWithDictionaryPopupListL()
    {
#ifdef RD_SCALABLE_UI_V2
    if( iFepPluginManager && (iFepPluginManager->PluginInputMode() == EPluginInputModeHwr) )
        {
        iFepPluginManager->LaunchPenInputRecognitionWithDictionarySelectionL();
        return;
        }
#endif //RD_SCALABLE_UI_V2
    }
void CAknFepManager::LaunchWritingSpeedPopupListL()
    {
#ifdef RD_SCALABLE_UI_V2
    if( iFepPluginManager && (iFepPluginManager->PluginInputMode() == EPluginInputModeFingerHwr) )
        {
        iFepPluginManager->LaunchPenInputWritingSpeedSelectionL();
        return;
        }
#endif //RD_SCALABLE_UI_V2
    }
void CAknFepManager::LaunchGuidingLinePopupListL()
    {
#ifdef RD_SCALABLE_UI_V2
    if( iFepPluginManager && (iFepPluginManager->PluginInputMode() == EPluginInputModeFingerHwr) )
        {
        iFepPluginManager->LaunchPenInputGuidingLineSelectionL();
        return;
        }
#endif //RD_SCALABLE_UI_V2
    }
void CAknFepManager::LoadIconL( CArrayPtr<CGulIcon>* aIcons, 
                                TInt aBitmapId, 
                                TInt aMaskId )
    {
    CGulIcon* icon = CGulIcon::NewLC();
    CFbsBitmap* bitmap = 0;
    CFbsBitmap* mask = 0;
    AknIconUtils::CreateIconL(  bitmap, mask, KAvkonBitmapFile, aBitmapId, aMaskId );
    icon->SetBitmap( bitmap );
    icon->SetMask( mask );
    aIcons->AppendL( icon );
    CleanupStack::Pop( icon );
    }


TBool CAknFepManager::IsInputModeAvailable(TInt aMode) const
    {
    TBool isAvailable = ETrue;
    const TInt inputLanguage = iLanguageCapabilities.iInputLanguageCode;
    switch (aMode)
        {
        case ELatin:
            isAvailable = !( ( inputLanguage == ELangPrcChinese ) &&
                (iAknEditorFlags & EAknEditorFlagForceTransparentFepModes));
            break;
        case EPinyin:
            isAvailable = ( inputLanguage == ELangPrcChinese );
            break;
        case EStroke:
            isAvailable = ( inputLanguage == ELangPrcChinese || 
                inputLanguage == ELangHongKongChinese || 
                inputLanguage == ELangTaiwanChinese);
            break;
        case ECangJie:
            {
            isAvailable = ( ( inputLanguage == ELangHongKongChinese ) &&
                          iQwertyInputMode );
#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifdef __HALF_QWERTY_KEYPAD            
            isAvailable = isAvailable && (EPtiKeyboardHalfQwerty != KeyboardLayout()); 
#endif //__HALF_QWERTY_KEYPAD
#endif //RD_INTELLIGENT_TEXT_INPUT
            }
            
            break;
        case EZhuyin:
            isAvailable = ( inputLanguage == ELangTaiwanChinese );
            break;
        case EPRCFind:
            isAvailable = ( ( inputLanguage == ELangPrcChinese ) &&
                (iAknEditorFlags & EAknEditorFlagForceTransparentFepModes));
            break;
        case EZhuyinFind:
            isAvailable = ( ( inputLanguage == ELangTaiwanChinese ) &&
                (iAknEditorFlags & EAknEditorFlagForceTransparentFepModes));
            break;
        case EStrokeFind:
            isAvailable = ( ( inputLanguage == ELangHongKongChinese ) &&
                (iAknEditorFlags & EAknEditorFlagForceTransparentFepModes));
            break;
        case EHiraganaKanji:
            isAvailable = (inputLanguage == ELangJapanese);
            break;
        case EKatakana:
            isAvailable = (inputLanguage == ELangJapanese );
            break;
        case EHindi:
            isAvailable = (inputLanguage == ELangHindi );
            break;
        case EHangul:
            isAvailable = (inputLanguage == ELangKorean );
            break;
        case ENativeNumber:
            isAvailable = (inputLanguage == ELangHindi 
                || inputLanguage == ELangArabic 
                || inputLanguage == ELangUrdu
                || inputLanguage == ELangFarsi/*
                || ( inputLanguage == ELangThai && iFepPluginManager && 
                     iFepPluginManager->PluginInputMode() != EPluginInputModeItut &&
                     iFepPluginManager->PluginInputMode() != EPluginInputModeNone)*/ );
        default:
            break;
        }
    return isAvailable;
    }

TInt CAknFepManager::NewInputModeAfterLanguageChange() const
    {
    TInt inputMode = 0;
    switch (iLanguageCapabilities.iInputLanguageCode)
        {
        case ELangPrcChinese:
            if ( iAknEditorFlags & EAknEditorFlagForceTransparentFepModes )
                {
                inputMode = ELatin; // EPRCFind from hashkey loop
                }
            else
                {
                inputMode = EPinyin;
                }
            break;
        case ELangHongKongChinese:
            if ( iAknEditorFlags & EAknEditorFlagForceTransparentFepModes )
                {
                inputMode = EStrokeFind;
                }
            else
                {
                TInt sharedDataMode = iSharedDataInterface->InputMode();
	            if (iQwertyInputMode && 
	            	sharedDataMode == ECangJie)
	            	{
	            	inputMode = ECangJie;
	            	}
	            else
	            	{
	            	inputMode = EStroke;
	            	}
                }
            break;
        case ELangTaiwanChinese:
            if ( iAknEditorFlags & EAknEditorFlagForceTransparentFepModes )
                {
                inputMode = EZhuyinFind;
                }
            else
                {
                inputMode = EZhuyin;
                }
            break;
        case ELangJapanese:
            inputMode = EHiraganaKanji;
            break;
        case ELangKorean:
            inputMode = EHangul;
            break;
        default:
            inputMode = ELatin;
            break;
        }
    return inputMode;
    }

void CAknFepManager::SetInputLanguageCapabilities(const TInt aInputLanguage)
    {
    iLanguageCapabilities.iInputLanguageCode = aInputLanguage;

    if ( aInputLanguage == ELangArabic || aInputLanguage == ELangHebrew || 
         aInputLanguage == ELangFarsi  || aInputLanguage == ELangUrdu )
        {
        iLanguageCapabilities.iSupportsCase = EFalse;
        iLanguageCapabilities.iRightToLeftLanguage = ETrue;
        SetCursorTypeForInputDirection(EFalse);
        }
    else
        {
        /*Hindi*/
        if ( aInputLanguage == ELangThai 
        || (TAknFepUiIndicInputManager :: IsIndicLangauge(TLanguage(aInputLanguage)))
        ) 
            {
            iLanguageCapabilities.iSupportsCase = EFalse;
            }
        else
            {
            iLanguageCapabilities.iSupportsCase = ETrue;
            }
        iLanguageCapabilities.iRightToLeftLanguage = EFalse;
        SetCursorTypeForInputDirection(ETrue);
        }

    TLanguage uiLanguage = User::Language();
    iLanguageCapabilities.iArabicIndicDigitsAllowed = 
             ( aInputLanguage == ELangArabic || uiLanguage == ELangArabic);
    iLanguageCapabilities.iEasternArabicIndicDigitsAllowed = ( 
        ( aInputLanguage == ELangUrdu || uiLanguage == ELangUrdu ) ||
        ( aInputLanguage == ELangFarsi|| uiLanguage == ELangFarsi ) );
    /*Hindi*/
    iLanguageCapabilities.iIndicDigitsAllowed =  
             ( (TAknFepUiIndicInputManager :: IsIndicDigitsSupported(TLanguage(aInputLanguage))) );
    UpdateLocalDigitMode();

    MAknFepManagerInterface* fepUI = iLangMan->GetFepUI(ELatin, iCharWidth, ETrue);
    MPtiLanguage* ptiLanguage = iPtiEngine->GetLanguage(fepUI->SupportLanguage(ELatin));
    if (ptiLanguage) // can be NULL in some uncommon situations
        {
        iLanguageCapabilities.iSupportsWesternPredictive = 
                                           ptiLanguage->HasInputMode(EPtiEnginePredictive);
        // tp teleca change Korean lang does not support predictive
        // and we want to disable predictive even with local editors
        if(/*IsKoreanInputLanguage()*/GetInputLanguageFromSharedDataInterface() == ELangKorean )
            {
            iLanguageCapabilities.iSupportsWesternPredictive = EFalse;
            }
        
        
        // tp teleca change end
#ifdef RD_INTELLIGENT_TEXT_INPUT
        // Predictive QWERTY (XT9) changes ---->
        iLanguageCapabilities.iSupportsWesternQwertyPredictive = 
                                           ptiLanguage->HasInputMode(EPtiEngineQwertyPredictive );
        if ( GetInputLanguageFromSharedDataInterface() == ELangKorean )
            {
            iLanguageCapabilities.iSupportsWesternQwertyPredictive = EFalse;            
            }
        
        // Predictive QWERTY (XT9) changes <----
#ifdef __HALF_QWERTY_KEYPAD
		iLanguageCapabilities.iSupportsWesternHalfQwertyPredictive = ptiLanguage->HasInputMode(EPtiEngineHalfQwertyPredictive );
#endif         
#endif //RD_INTELLIGENT_TEXT_INPUT
        }

    if (!iLanguageCapabilities.iSupportsWesternPredictive 
#ifdef RD_INTELLIGENT_TEXT_INPUT
        && !iLanguageCapabilities.iSupportsWesternQwertyPredictive 
        // Predictive QWERTY changes
#ifdef __HALF_QWERTY_KEYPAD
		&& !iLanguageCapabilities.iSupportsWesternHalfQwertyPredictive 
#endif //__HALF_QWERTY_KEYPAD        
#endif //RD_INTELLIGENT_TEXT_INPUT
        ) // Predictive QWERTY changes
        {
        SetWesternPredictive(EFalse);
        }
    }

TInt CAknFepManager::SubstituteSublanguageId(const TInt aUiLanguage) const
    {
    return DefaultInputLanguageFromUILanguage(aUiLanguage);
    }

TTmDocPosSpec CAknFepManager::DocPos()
    {
    TTmDocPosSpec docPos = TTmDocPosSpec();
    TTmDocPos rawDocPos;
    TBool gotPos = EFalse;
    MAknFepDocumentNavigation* docNavi = AknFepDocumentNavigation();
    if (docNavi)
    	{
        docNavi->GetDocPos(rawDocPos);
        gotPos = ETrue;
    	}
    else
    	{
        // Use Edwin (FepAwareTextEditor() ) to get the position of document
        if ( FepAwareTextEditor() )
            {
            TCursorSelection cursorSel( 0, 0 );
            // Get the postion of document
            FepAwareTextEditor()->GetCursorSelectionForFep( cursorSel );

            // Set the leading edge of the document.
            rawDocPos.iPos = cursorSel.iCursorPos;
            CAknEdwinState* state( EditorState() );
            rawDocPos.iLeadingEdge = ( state && 
                state->Flags() & EAknEditorFlagCursorLedingEdge );                
            gotPos = ETrue;
            }
    	}
    if (gotPos)
    	{
        docPos.iPos = rawDocPos.iPos;
        TTmDocPosSpec::TType docPosType = 
                      rawDocPos.iLeadingEdge ? TTmDocPosSpec::ELeading : TTmDocPosSpec::ETrailing;
        docPos.iType = docPosType;
        }
    return docPos;
    }

/**
* This method needs EditorState() to exist
*
*/
CTextView* CAknFepManager::TextView() const
    {
    if (EditorState())
        {
        CAknEdwinFormAccessor* formAccessor = EditorState()->FormAccessor();
        if (formAccessor)
            {
            return formAccessor->GetTextView();
            }
        }
    return NULL;
    }

CTextLayout* CAknFepManager::TextLayout() const
    {
    if( EditorState() )
        {
        CAknEdwinFormAccessor* formAccessor = EditorState()->FormAccessor();
        if (formAccessor)
            {
            return formAccessor->GetTextLayout();
            }
        }
    return NULL;
    }
    
CPlainText* CAknFepManager::PlainText() const
    {
    return iUiInterface->PlainText(EditorState());
    }

/**
* Checks if the paragraph containing aPos is Right to left.
* This routine will return nonsense if
*   - TextLayout() does not return a pointer
*   - the position passed is not in the formatted region
*
* Note, the return value from FindDocPos is thrown away
*/
TBool CAknFepManager::IsRightToLeftParagraph(TTmDocPosSpec aPos) const
    {
    TTmPosInfo2 posInfo;
    TTmLineInfo lineInfo;
    MAknFepDocumentNavigation* docNavi = AknFepDocumentNavigation();
    if (docNavi)
    	{
    	docNavi->FindDocPos(aPos, posInfo, lineInfo);
    	}
	else
		{
	    CTextLayout* textLayout = TextLayout();
	    if (textLayout)
	        {
	        textLayout->TagmaTextLayout().FindDocPos(aPos, posInfo, lineInfo);
	        }
		}
		
    return lineInfo.iFlags & TTmLineInfo::EParRightToLeft;
    }


void CAknFepManager::FindAdjacentChunks(const TTmDocPosSpec& aPos,
                                        CTmTextLayout::TTmChunkDescription& aLeft, 
                                        CTmTextLayout::TTmChunkDescription& aRight) const
    {
    aLeft.iStart = -1;
    aLeft.iEnd = -1;
    aLeft.iRightToLeft = EFalse;
    aRight.iStart = -1;
    aRight.iEnd = -1;
    aRight.iRightToLeft = EFalse;

    MAknFepDocumentNavigation* docNavi = AknFepDocumentNavigation();
    if (docNavi)
    	{
    	docNavi->FindAdjacentChunks(aPos, aLeft, aRight);
    	}
    else
    	{
    CTextLayout* textLayout = TextLayout();
    if (textLayout)
        {
        textLayout->TagmaTextLayout().FindAdjacentChunks(aPos, aLeft, aRight);
        }
    	}
    }


TBool CAknFepManager::GetNextVisualRightCharacter( TInt& aPosition )
    {
    TTmDocPosSpec pos = DocPos();
    TTmPosInfo2 info;
    TBool nextPositionFound = EFalse;
    MAknFepDocumentNavigation* docNavi = AknFepDocumentNavigation();
    if (docNavi)
    	{
    	nextPositionFound = docNavi->GetNextVisualCursorPos(pos, info, EFalse);
    	}
    else
    	{
	    CTextLayout* textLayout = TextLayout();
	    if (textLayout)
	        {
	        nextPositionFound = textLayout->TagmaTextLayout().GetNextVisualCursorPos(pos, 
                                                                                   info, 
                                                                                   EFalse);
	        }
        }
    if (nextPositionFound)
        {
        aPosition = info.iDocPos.iPos;

        CTmTextLayout::TTmChunkDescription leftChunk;
        CTmTextLayout::TTmChunkDescription rightChunk;
        FindAdjacentChunks(pos, leftChunk, rightChunk);

        if ( !rightChunk.iRightToLeft )
            {
            aPosition -= 1;
            }
        }
    return nextPositionFound;
    }

TBool CAknFepManager::GetNextVisualLeftCharacter( TInt& aPosition )
    {
    TTmDocPosSpec pos = DocPos();
    TTmPosInfo2 info;
    TBool nextPositionFound = EFalse;
    MAknFepDocumentNavigation* docNavi = AknFepDocumentNavigation();
    if (docNavi)
    	{
    	nextPositionFound = docNavi->GetNextVisualCursorPos(pos, info, ETrue);
    	}
    else
    	{
	    CTextLayout* textLayout = TextLayout();
	    if (textLayout)
	        {
	        nextPositionFound = TextLayout()->TagmaTextLayout().GetNextVisualCursorPos(pos, 
                                                                                     info, 
                                                                                     ETrue);
	        }
        }
    if (nextPositionFound)
        {
        aPosition = info.iDocPos.iPos;
        CTmTextLayout::TTmChunkDescription leftChunk;
        CTmTextLayout::TTmChunkDescription rightChunk;
        FindAdjacentChunks(pos, leftChunk, rightChunk);

        if ( leftChunk.iRightToLeft )
            {
            aPosition -= 1;
            }
        if (iLanguageCapabilities.iInputLanguageCode == ELangVietnamese)
            {
            aPosition = pos.iPos -1;
            }
        }
    return nextPositionFound;
    }

void CAknFepManager::FindVisualRightOfWord(TInt& aPosition)
    {
    TTmDocPosSpec pos = DocPos();
    CTmTextLayout::TTmChunkDescription leftChunk;
    CTmTextLayout::TTmChunkDescription rightChunk;
    FindAdjacentChunks(pos, leftChunk, rightChunk);

    if ( rightChunk.iRightToLeft )
        {
        FindStartOfWord(aPosition);
        }
    else
        {
        FindEndOfWord(aPosition);
        }
    }

void CAknFepManager::FindVisualLeftOfWord(TInt& aPosition)
    {
    TTmDocPosSpec pos = DocPos();
    CTmTextLayout::TTmChunkDescription leftChunk;
    CTmTextLayout::TTmChunkDescription rightChunk;
    FindAdjacentChunks(pos, leftChunk, rightChunk);

    if ( leftChunk.iRightToLeft)
        {
        FindEndOfWord(aPosition);
        }
    else
        {
        FindStartOfWord(aPosition);
        }
    }



TBool CAknFepManager::GetVisualDocStart( TTmDocPosSpec& aPos) const
    {
    // Set up the initial try as leading to the first logical character
    // It is a requirement that this position is at least in the first line
    TTmDocPosSpec pos(0, TTmDocPosSpec::ELeading);

    // Need to record if we are in aleft to right paragraph or not
    TBool leftToRightParagraph = !IsRightToLeftParagraph(pos);

    TBool success(EFalse);
    CTextLayout* textLayout = TextLayout();
    TTmDocPosSpec docPos;
    if (AknFepDocumentNavigation() || textLayout)
        {
//      This was written because Tagma's LineExtreme() method did not seem to work
        success = GetAknFepLineExtreme( pos, !leftToRightParagraph, docPos );
        }

    if (success)
        aPos = docPos;
    else // Possible cleanup; action for when there is insufficient information
        {
        aPos.iPos = 0;
        aPos.iType = TTmDocPosSpec::ELeading;
        }

    return success;
    }

TBool CAknFepManager::GetVisualDocEnd( TTmDocPosSpec& aPos) const
    {
    // Set EAknEditorFlagNeedWholeTextData to Editor State
    // EAknEditorFlagNeedWholeTextData means needing to get whole document data to editor
    UpdateEditorStateFlags( EAknEditorFlagNeedWholeTextData );

    TInt docLength = iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep();

    // Set up the initial try as trailing after the last logical character
    // It is a requirement that this position is at least in the last line
    TTmDocPosSpec pos(docLength, TTmDocPosSpec::ETrailing);

    // Need to record if we are in a left to right paragraph or not
    TBool leftToRightParagraph = !IsRightToLeftParagraph(pos);

    TBool success(EFalse);
    CTextLayout* textLayout = TextLayout();
    TTmDocPosSpec docPos;
    if (AknFepDocumentNavigation() || textLayout)
        {
        // This was written because Tagma's LineExtreme() method did not seem to work
        success = GetAknFepLineExtreme( pos, leftToRightParagraph, docPos );
        }

    if (success)
        aPos = docPos;
    else // Possible cleanup; action for when there is insufficient information
        {
        aPos.iPos = docLength;
        aPos.iType = TTmDocPosSpec::ETrailing;
        }
        
    // Remove EAknEditorFlagNeedWholeTextData fromEditor State
    UpdateEditorStateFlags( EAknEditorFlagNeedWholeTextData, EFalse );
    return success;
    }


TBool CAknFepManager::GetAknFepLineExtreme( const TTmDocPosSpec& aPos, TBool aToRight,
        TTmDocPosSpec& aExtreme) const
    {
    TBool success(EFalse);

    TTmPosInfo2 posInfo;
    TTmLineInfo lineInfo;
    TBool found = EFalse;
    MAknFepDocumentNavigation* docNavi = AknFepDocumentNavigation();
    if (docNavi)
    	{
    	found = docNavi->FindDocPos(aPos, posInfo, lineInfo);
    	}
    else
    	{
	    CTextLayout* textLayout = TextLayout();
	    if ( textLayout )
	        {
	        found = textLayout->TagmaTextLayout().FindDocPos(aPos, posInfo, lineInfo);
	        }
    	}

    if ( found )
        {
        // Make a selection of the line info.  Cursor first, Anchar 2nd:
        TCursorSelection theWholeLine( lineInfo.iEnd-1, lineInfo.iStart );

        TWesternNaviEvent event;
        if ( aToRight )
            event = ERightNaviEvent;
        else
            event = ELeftNaviEvent;

        GetAvkonDefinedVisualLimitsOfSelection( theWholeLine, event, aExtreme, NULL );
        success = ETrue;
        }

    return success;
    }


/**
* This routine analyses a selection (Does not have to be a real selection, but the datastructure
* is used as input) to find its visual start or end, according to the passed parameter
* The algo used is:
* - First find the top line of the selection.
* - Then find the bottom line of the selection.
* - Look at the two lines' directionality (from the paragraph they belong to) and decide what the overall
* directionality of the selection is.  Thus right/left can be translated to increment/decrement.
* (Note that Up is always decrement; down is always increment). So that allows the key actions to be
* correctly grouped, and allows it to be determined which keys act from the top or the bottom line
* (Decrement always acts from the top line; Increment always acts from the bottom line)
*
* - When the keys are handled, the action is again translated back to right/left according to the key-
* grouping and whether you are acting from the top line or from the bottom line.  Eg, decrement in RTL
* line is visually to the Right.
* - the appropriate "ExtremePos" API is called passing in the position on the line in which we are acting
*
*/
void CAknFepManager::GetAvkonDefinedVisualLimitsOfSelection(
    const TCursorSelection& aSelection,
    TWesternNaviEvent aEvent,
    TTmDocPosSpec& aLimitPos,
    TWesternNaviEvent* aEffectiveRightOrLeftEvent ) const
    {
    // in case an irrelvant event is sent, initialize aLimitPos to something safe
    aLimitPos.iPos = 0;
    aLimitPos.iType = TTmDocPosSpec::ELeading;

    TTmDocPosSpec lowLogicalPos( aSelection.LowerPos(), TTmDocPosSpec::ELeading );
    TTmDocPosSpec highLogicalPos( aSelection.HigherPos(), TTmDocPosSpec::ETrailing );

    // this records what the resulting Right/left direction (for optional return)
    TWesternNaviEvent effectiveEvent = aEvent;

    // Get line and paragraph of the lower limit of the selection:
    TTmPosInfo2 posInfo;
    TTmLineInfo lineInfo;
    TBool lowIsRightToLeft;
    TBool highIsRightToLeft;
    TBool layoutExists = EFalse;
    
    MAknFepDocumentNavigation* docNavi = AknFepDocumentNavigation();
    if (docNavi)
    	{
    	docNavi->FindDocPos(lowLogicalPos, posInfo, lineInfo);
        lowIsRightToLeft = lineInfo.iFlags & TTmLineInfo::EParRightToLeft;
    	docNavi->FindDocPos(highLogicalPos, posInfo, lineInfo);
        highIsRightToLeft = lineInfo.iFlags & TTmLineInfo::EParRightToLeft;
        layoutExists = ETrue;
    	}
    else
    	{
	    CTextLayout* textLayout = TextLayout();
	    if (textLayout)
	        {
	        textLayout->TagmaTextLayout().FindDocPos(lowLogicalPos, posInfo, lineInfo);
	        lowIsRightToLeft = lineInfo.iFlags & TTmLineInfo::EParRightToLeft;
	        textLayout->TagmaTextLayout().FindDocPos(highLogicalPos, posInfo, lineInfo);
	        highIsRightToLeft = lineInfo.iFlags & TTmLineInfo::EParRightToLeft;
	        layoutExists = ETrue;
	        }
    	}
        
    if (layoutExists)
    	{
        // Decide on the net directionality. This if looks a bit laboured, but it
        // exhibits the Averell policy.
        TBool netDirectionIsRightToLeft(EFalse);
        if ( lowIsRightToLeft == highIsRightToLeft )
            netDirectionIsRightToLeft = lowIsRightToLeft;
        else
            netDirectionIsRightToLeft = lowIsRightToLeft;// ambiguous - Take first paragraph

        if ( netDirectionIsRightToLeft )
            switch ( aEvent )
            {
            case ELeftNaviEvent:
            case EDownNaviEvent:
                effectiveEvent = ELeftNaviEvent;
                aLimitPos = LeftVisualExtremePos( highLogicalPos, lowLogicalPos.iPos, 
                                                  highLogicalPos.iPos );
                break;
            case ERightNaviEvent:
            case EUpNaviEvent:
                effectiveEvent = ERightNaviEvent;
                aLimitPos = RightVisualExtremePos(lowLogicalPos, lowLogicalPos.iPos, 
                                                  highLogicalPos.iPos );
                break;
            default:
            // No action. Only the events above are handled.
                break;
            }
        else
            switch (aEvent)
            {
            case ELeftNaviEvent:
            case EUpNaviEvent:
                effectiveEvent = ELeftNaviEvent;
                aLimitPos = LeftVisualExtremePos( lowLogicalPos, lowLogicalPos.iPos, 
                                                  highLogicalPos.iPos);
                break;
            case ERightNaviEvent:
            case EDownNaviEvent:
                effectiveEvent = ERightNaviEvent;
                aLimitPos = RightVisualExtremePos( highLogicalPos, lowLogicalPos.iPos, 
                                                   highLogicalPos.iPos );
                break;
            default:
            // No action. Only the events above are handled.
                break;
            }
        }

    else
        {
        // No text layout: Left or Up naviEvent gives low end; Right or down give high end
        switch ( aEvent )
            {
            case ELeftNaviEvent:
            case EUpNaviEvent:
                effectiveEvent = ELeftNaviEvent;
                aLimitPos = lowLogicalPos;
                break;
            case ERightNaviEvent:
            case EDownNaviEvent:
                effectiveEvent = ERightNaviEvent;
                aLimitPos = highLogicalPos;
                break;
            default:
            // No action. Only the events above are handled.
                break;
            }
        }
    // return the effective direction (Right or Left)
    if ( aEffectiveRightOrLeftEvent )
        *aEffectiveRightOrLeftEvent = effectiveEvent;

    }


TTmDocPosSpec CAknFepManager::LeftVisualExtremePos( const TTmDocPosSpec& aStartPos, 
                                                    TInt aMinPos, TInt aMaxPos ) const
    {
    // Start at start and find adjacent left chunks repeatedly until we find no more chunks, or our max or min
    // is within a chunk
    TTmDocPosSpec currentPos = aStartPos;
    CTextLayout* textLayout = TextLayout();
    if (AknFepDocumentNavigation() || textLayout)
        {
        CTmTextLayout::TTmChunkDescription leftChunk;
        CTmTextLayout::TTmChunkDescription rightChunk;

        TInt iterations(0);
        TBool endFound(EFalse);

        while( !endFound && iterations < KMaxChunkWalkIterations )
            {
            iterations++;
            FindAdjacentChunks(currentPos, leftChunk, rightChunk);
            // Try to move to left end of left chunk
            endFound = LeftEndOfChunkWithinRange( leftChunk, aMinPos, aMaxPos, currentPos );
            }
        }
    return currentPos;
    }

TTmDocPosSpec CAknFepManager::RightVisualExtremePos( const TTmDocPosSpec& aStartPos, 
                                                     TInt aMinPos, TInt aMaxPos ) const
    {
    // Start at start and find adjacent right chunks repeatedly until we find no more chunks, or our max or min
    // is within a chunk
    TTmDocPosSpec currentPos = aStartPos;
    CTextLayout* textLayout = TextLayout();
    if (AknFepDocumentNavigation() || textLayout)
        {
        CTmTextLayout::TTmChunkDescription leftChunk;
        CTmTextLayout::TTmChunkDescription rightChunk;

        TInt iterations(0);
        TBool endFound(EFalse);

        while( !endFound && iterations < KMaxChunkWalkIterations )
            {
            iterations++;
            FindAdjacentChunks(currentPos, leftChunk, rightChunk);
            // Try to move to left end of left chunk
            endFound = RightEndOfChunkWithinRange( rightChunk, aMinPos, aMaxPos, currentPos );
            }
        }
    return currentPos;
    }

TBool CAknFepManager::LeftEndOfChunkWithinRange( const CTmTextLayout::TTmChunkDescription& aChunk,
                                         TInt aMinPos, TInt aMaxPos, TTmDocPosSpec& aNewPos) const
    {
    TBool constrained = EFalse;
    TInt leftEnd;
    if ( !ChunkIsValid( aChunk ) )
        {
        constrained = ETrue; // leave the aNewPos as it is
        }
    else if ( aChunk.iStart >= iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep() )
        {
        constrained = ETrue; // That chunk was a ficticious one:
        }
    else if ( aChunk.iEnd <= aMinPos || aChunk.iStart >= aMaxPos )
        {
        constrained = ETrue; // That chunk found does not actually overlap with valid range. Don't go there
        }
    else
        {
        if ( aChunk.iRightToLeft )
            {
            leftEnd =  aChunk.iEnd;
            if ( leftEnd >= aMaxPos )
                {
                constrained = ETrue;
                aNewPos.iPos = aMaxPos;
                }
            else
                {
                aNewPos.iPos = leftEnd;
                }
            aNewPos.iType = TTmDocPosSpec::ETrailing;
            }
        else
            {
            leftEnd = aChunk.iStart;
            if ( leftEnd <= aMinPos )
                {
                constrained = ETrue;
                aNewPos.iPos = aMinPos;
                }
            else
                {
                aNewPos.iPos = leftEnd;
                }
            aNewPos.iType = TTmDocPosSpec::ELeading;
            }
        }
    return constrained;
    }

TBool CAknFepManager::RightEndOfChunkWithinRange( const CTmTextLayout::TTmChunkDescription& aChunk,
                                         TInt aMinPos, TInt aMaxPos, TTmDocPosSpec& aNewPos) const
    {
    TBool constrained = EFalse;
    TInt rightEnd;
    if ( !ChunkIsValid( aChunk ) )
        {
        constrained = ETrue; // leave the aNewPos as it is
        }
    else if ( aChunk.iStart >= iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep() )
        {
        constrained = ETrue; // That chunk was a ficticious one:
        }
    else if ( aChunk.iEnd <= aMinPos || aChunk.iStart >= aMaxPos )
        {
        constrained = ETrue; // That chunk found does not actually overlap with valid range. Don't go there
        }
    else
        {
        if ( aChunk.iRightToLeft )
            {
            rightEnd =  aChunk.iStart;
            if ( rightEnd <= aMinPos )
                {
                constrained = ETrue;
                aNewPos.iPos = aMinPos;
                }
            else
                {
                aNewPos.iPos = rightEnd;
                }
            aNewPos.iType = TTmDocPosSpec::ELeading;
            }
        else
            {
            rightEnd = aChunk.iEnd;
            if ( rightEnd >= aMaxPos )
                {
                constrained = ETrue;
                aNewPos.iPos = aMaxPos;
                }
            else
                {
                aNewPos.iPos = rightEnd;
                }
            aNewPos.iType = TTmDocPosSpec::ETrailing;
            }
        }
    return constrained;
    }

/**
* This method handles the attachment to the chunk being navigated toward.
* Should only be used for Right and Left navigation.
* The actual direction of that chunk does not need to be looked at. Even if
* is the same direction as the one being moved from, you still attach to it, as it may not
* be logically contiguous.
*/
void CAknFepManager::AdjustPosSpecForAmbiguityAfterLeftOrRightNavigation( TTmDocPosSpec& aPos, 
                                                              TWesternNaviEvent aEvent ) const
    {

    CTmTextLayout::TTmChunkDescription leftChunk;
    CTmTextLayout::TTmChunkDescription rightChunk;
    FindAdjacentChunks(aPos, leftChunk, rightChunk);

    TBool inputDirectionIsRTL = ( CurrentInputDirectionality() == TBidiText::ERightToLeft );

    switch (aEvent)
        {
        case ELeftNaviEvent:
            {
            // Do not attach to the left chunk if its directionality is not the same as aDirection
            if ( ChunkIsValid( leftChunk ) )
                if ( COMPARE_BOOLS(inputDirectionIsRTL, leftChunk.iRightToLeft) )
                    AttachToLeftChunk( aPos, leftChunk );
            break;
            }
        case ERightNaviEvent:
            {
            if ( ChunkIsValid( rightChunk ) )
                if ( COMPARE_BOOLS(inputDirectionIsRTL, rightChunk.iRightToLeft) )
                    AttachToRightChunk( aPos, rightChunk );
            break;
            }
        default:
            break;
        }

    }


void CAknFepManager::AttachToRightChunk( TTmDocPosSpec& aPos, 
                                         const CTmTextLayout::TTmChunkDescription& aRightChunk )
    {
    if ( aRightChunk.iRightToLeft )
        {
        aPos.iPos = aRightChunk.iEnd;
        aPos.iType = TTmDocPosSpec::ETrailing;
        }
    else
        {
        aPos.iPos = aRightChunk.iStart;
        aPos.iType = TTmDocPosSpec::ELeading;
        }
    }

void CAknFepManager::AttachToLeftChunk( TTmDocPosSpec& aPos, 
                                        const CTmTextLayout::TTmChunkDescription& aLeftChunk )
    {
    if ( aLeftChunk.iRightToLeft )
        {
        aPos.iPos = aLeftChunk.iStart;
        aPos.iType = TTmDocPosSpec::ELeading;
        }
    else
        {
        aPos.iPos = aLeftChunk.iEnd;
        aPos.iType = TTmDocPosSpec::ETrailing;
        }
    }

/**
* This routine ensures that the cursor position is not attached to a chunk of opposite direction.
* For end of document, the normal routine can be used.
* For the start, usually do nothing, but there is the possibility of forcing position 0/Trailing, when
* the edge chunk and the input directionality are opposite.
*/
void CAknFepManager::AdjustPosSpecForInputLanguageAfterDocumentLoop( TTmDocPosSpec& aPos, 
                                     TWesternNaviEvent /*aNaviEvent*/, TBool aLoopToTop ) const
    {

    // Looping based on a Left event should adjust the cursor position as if that point had
    // been reached by navigation to that same visual position outwards from the body
    // Define this as the effective navigation that has just happened:
    TWesternNaviEvent effectiveEvent(ELeftNaviEvent);

    // Rule for this effective navigation is
    // move to top of document : if paradirection is LTR then treat it as a LEFT navi
    // move to top of document : if paradirection is RTL then treat it as RIGHT navi
    // move to bottom of document : if paradirection is LTR then treat is as RIGHT navi
    // move to bottom of document : if paradirection is RTL then treat as LEFT navi
    // However, NOTE that decrementing events leads to looping to the END! (and vice versa)
    TBool newParaIsRTL = IsRightToLeftParagraph(aPos);
    if ( aLoopToTop && newParaIsRTL ) // move to top; RTL
        effectiveEvent = ERightNaviEvent;
    else if ( !aLoopToTop && !newParaIsRTL ) // move to bottom; LTR
        effectiveEvent = ERightNaviEvent;

    if ( aLoopToTop )
        {
        //  There is no ficticious block of the paragraph direction at -1 to 0.  So we have to
        // behave as if there is one:
        //
        CTmTextLayout::TTmChunkDescription leftChunk;
        CTmTextLayout::TTmChunkDescription rightChunk;
        CTmTextLayout::TTmChunkDescription& edgeChunk = leftChunk;

        FindAdjacentChunks(aPos, leftChunk, rightChunk);

        // Get input language direction
        TBool inputDirectionIsRTL = ( CurrentInputDirectionality() == TBidiText::ERightToLeft );
        if ( effectiveEvent == ELeftNaviEvent )
            edgeChunk = leftChunk;
        else
            edgeChunk = rightChunk;

        // Check edgechunk for its direction
        if ( !COMPARE_BOOLS( edgeChunk.iRightToLeft, inputDirectionIsRTL ) )
            {
            // Input language disagrees with the chunk that we are on,
            aPos.iPos = 0;
            aPos.iType = TTmDocPosSpec::ETrailing;
            }

        }
    else // Looping to the end of document
        {
        // There IS a ficticious block at the doc end, so use the usual routine.
        AdjustPosSpecForAmbiguityAfterLeftOrRightNavigation( aPos, effectiveEvent );
        }

    }

TBool CAknFepManager::ChunkIsValid( const CTmTextLayout::TTmChunkDescription& aChunk )
    {
    TBool isValid(ETrue);

    const TInt KNotValid = -1;
    if ( aChunk.iRightToLeft == KNotValid )
        isValid = EFalse;
    else if (aChunk.iStart == KNotValid )
        isValid = EFalse;
    else if (aChunk.iEnd == KNotValid )
        isValid = EFalse;

#ifdef _DEBUG
    if (isValid)
        __ASSERT_DEBUG( aChunk.iStart <= aChunk.iEnd, 
                        AknFepPanic( EAknFepPanicBadCursorNavigationState ) );
#endif

    return isValid;
    }
/**
* Finds the current input direction. This is determined as best as it can using
* - T9Interface as the preferred source of information
* - shared data (via AknTextUtils) if that fails
* - localLanguage overrides the above sources of information
* - when fep is in numeric mode it returns LTR
* - if FEP is in an irelevant mode, then LTR is returned
*
* @return TDirectionality::ELeftToRight for LTR alpha mode or numeric mode
*/
TBidiText::TDirectionality CAknFepManager::CurrentInputDirectionality() const
    {
    TBidiText::TDirectionality directionality(TBidiText::ELeftToRight);

    // See if we are in a mode where the directionality is defined:
    TBool checkForRTLValid(EFalse);

    switch (iMode )
        {
        case ELatin:
        case ENumber:
            checkForRTLValid = ETrue;
            break;
        default:
            break;
        }

    if ( checkForRTLValid )
        {
            MPtiLanguage* ptilang = iPtiEngine->CurrentLanguage(); // No need to get language from PtiEngine ?
            TInt inputLanguage = (ptilang)? ptilang->LanguageCode() : 0;
            if ( inputLanguage )
                {
                directionality = TBidiText::ScriptDirectionality( (TLanguage)inputLanguage );
                checkForRTLValid = EFalse;
                }
        }

    // if no T9 interface information is available, then go for other sources
    // Note that you have to check for local language in this case (belt-and-braces)
    if ( checkForRTLValid )
        {
        TLanguage localLanguage = ELangTest;
        if ( GetLocalLanguage( localLanguage ) )
            directionality = TBidiText::ScriptDirectionality( localLanguage );
        else
            directionality = AknTextUtils::CurrentScriptDirectionality();
        }

    return directionality;
    }


TBool CAknFepManager::GetLocalLanguage( TLanguage& aLanguage ) const
    {
    TBool result(EFalse);
    // Check for a local language override
    if ( IsFepAwareTextEditor() )
        {
        // For ITU-T keyboard we always use english in latin-only editors.               
        // In case of qwerty keyboard, some languages can be used in latin
        // only-editors, some require that english is used instead. That
        // is decided here.
        const TInt globalLang = iSharedDataInterface->InputTextLanguage();        
        TBool allowTest = 
#ifndef RD_INTELLIGENT_TEXT_INPUT
                           !iQwertyInputMode ||
#endif
                           globalLang == ELangPrcChinese ||
                           globalLang == ELangHongKongChinese ||
                           globalLang == ELangTaiwanChinese ||
                           globalLang == ELangThai ||
                           globalLang == ELangGreek ||
                           globalLang == ELangArabic ||
                           globalLang == ELangFarsi ||                           
                           globalLang == ELangUrdu ||
                           globalLang == ELangHebrew ||
                           globalLang == ELangRussian ||
                           globalLang == ELangUkrainian ||
                           globalLang == ELangHindi ||
#ifdef RD_MARATHI
                           globalLang == ELangMarathi ||
#endif // RD_MARATHI

#ifdef RD_HINDI_PHONETIC_INPUT  
                           globalLang == KLangHindiPhonetic ||
#endif                           
                           globalLang == ELangBulgarian || 
                           globalLang == ELangVietnamese;
                                          
        if (allowTest && iAknEditorFlags & EAknEditorFlagLatinInputModesOnly &&
            iLanguageCapabilities.iInputLanguageCode != ELangJapanese)
            {
            aLanguage = ELangEnglish;               
            }
        else
            {
            aLanguage = EditorState()->LocalLanguage();
            
            // Change local language to ELangEnglish if application, e.g. Search Widget,
            // sets local language to ELangEnglish_Taiwan, ELangEnglish_HongKong 
            // and ELangEnglish_Prc. 
            if( aLanguage == ELangEnglish_Taiwan || 
                    aLanguage == ELangEnglish_HongKong || 
                    aLanguage == ELangEnglish_Prc )
                {
                aLanguage = ELangEnglish;
                }
            }
            
        if ( aLanguage != ELangTest )
            {   
            MPtiLanguage* languageObject = NULL;
            languageObject = iPtiEngine->GetLanguage(aLanguage);
            if (languageObject)  // Check if the input language is supported.
                {                
                result = ETrue;
                }
            else
                {
                aLanguage = ELangTest;
                }
            }
        }
    return result;
    }


TBool CAknFepManager::GetExposedDirectionOfText( TInt aPos, TBool aForward, 
                                                 TBool& aIsRightToLeft ) const
    {
    TBool hasStrongCharacter(EFalse);

    // Access the plain text from the editor, getting a little bit at a time.

    TInt docLen = iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep();

    // Init with defaults for aForward = ETrue
    TInt limit = docLen;
    TInt increment = 1;
    if ( !aForward )
        {
        limit = -1; // Loop does not execute if == limit
        increment = -1;
        }

    for ( TInt index = aPos; index != limit ; index += increment )
        {
        TChar ch = CharAt(index);
        if ( iInlineTextDecorator->CharacterIsStrongRightToLeft( (TText)ch ) )
            {
            hasStrongCharacter = ETrue;
            aIsRightToLeft = ETrue;
            break;
            }
        else if ( iInlineTextDecorator->CharacterIsStrongLeftToRight( (TText)ch ) )
            {
            hasStrongCharacter = ETrue;
            aIsRightToLeft = EFalse;
            break;
            }
        }

    return hasStrongCharacter;
    }

TBool CAknFepManager::GetExposedDirectionOfTextInDescriptor( const TDesC& aText, TBool aForward, 
                                                             TBool& aIsRightToLeft ) const
    {
    TBool hasStrongCharacter(EFalse);

    TInt textLen = aText.Length();

    TInt start = 0;
    TInt limit = textLen;
    TInt increment = 1;
    if ( !aForward )
        {
        start = textLen-1;
        limit = -1; // Loop does not execute if == limit
        increment = -1;
        }

    for ( TInt index = start; index != limit ; index += increment )
        {
        if ( iInlineTextDecorator->CharacterIsStrongRightToLeft( (TText)aText[index] ) )
            {
            hasStrongCharacter = ETrue;
            aIsRightToLeft = ETrue;
            break;
            }
        else if (  iInlineTextDecorator->CharacterIsStrongLeftToRight( (TText)aText[index] ) )
            {
            hasStrongCharacter = ETrue;
            aIsRightToLeft = EFalse;
            break;
            }
        }

    return hasStrongCharacter;
    }

TChar CAknFepManager::CharAt( TInt aPos ) const
    {
    TBuf<ESingleCharacter> chDes;
#ifdef RD_SCALABLE_UI_V2
    if ( SemiFepAwareTextEditor( ETrue ) )
#else
    if ( iInputCapabilities.FepAwareTextEditor() )
#endif // RD_SCALABLE_UI_V2    
        iInputCapabilities.FepAwareTextEditor()->GetEditorContentForFep( chDes, aPos, 
                                                                         ESingleCharacter);
    if( chDes.Length() > 0 )
        return chDes[0];
    else
        return TChar(0);
    }


/**
* Navigate off of selected text using an Up/Down/Right/Left navigation event
*
* The code takes account of the paragraph directionality that the passed selection is
* found in.
*
* The new cursor position is resolved for ambuguity and set using SetCursorPosition and
* so it has the same side-effects:
*   iUncommittedText is set to the zero-width "cursor" state with the new cursor position
*   iUncommittedText is set into the FepAwareTextEditor state
*   iCaseManager is updated with a ENullNaviEvent
*   the doc pos is set in TextView object if present
*/
TKeyResponse CAknFepManager::NavigateFromSelectionL(
            const TCursorSelection& aSelection,
            TWesternNaviEvent aNaviEvent,
            TTmDocPosSpec& aPos)
    {
    if (IsCcpuFlagSet(ECcpuStateCopy | ECcpuStateCut | ECcpuStateHashDown))
        {
        return EKeyWasNotConsumed;
        }
        
    switch( aNaviEvent )
        {
        case EUpNaviEvent:
        case EDownNaviEvent:
        case ERightNaviEvent:
        case ELeftNaviEvent:
            // Carry on. Handle this event
            break;
        default: // just leave it as NULL
            return EKeyWasNotConsumed;
        }
    // Find the visual end of the selection
    TWesternNaviEvent effectiveEvent; // used to return either Right or Left event from next call:
    GetAvkonDefinedVisualLimitsOfSelection( aSelection, aNaviEvent, aPos, &effectiveEvent );
    AdjustPosSpecForAmbiguityAfterLeftOrRightNavigation( aPos, effectiveEvent );

    // Set this as the current position
    SetCursorPositionL( aPos );

    return EKeyWasConsumed;
    }


void CAknFepManager::SetCursorPositionL( const TTmDocPosSpec& aNewCursorPos, TBool aDragSelectOn )
    {
    if ( !iInputCapabilities.FepAwareTextEditor() )
        return;

    TInt newAnchorPos = aNewCursorPos.iPos;

    if ( aDragSelectOn )
        {
        TCursorSelection selection;
        iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(selection);
        if ( selection.Length() > 0 ) // there is a selection. Keep the anchor
            newAnchorPos = selection.iAnchorPos;
        }

    iUncommittedText.SetSelection( aNewCursorPos.iPos, newAnchorPos );

    iInputCapabilities.FepAwareTextEditor()->SetCursorSelectionForFepL(iUncommittedText);
    iCaseMan->UpdateCase(ENullNaviEvent);

    // This is able to set the directional attachment of the cursor in a way that the FepAwareTextEditor interface cannot
    MAknFepDocumentNavigation* docNavi = AknFepDocumentNavigation();
    if (docNavi)
    	{
    	docNavi->SetDocPosL(aNewCursorPos, aDragSelectOn);
    	}
    else
    	{
        CAknEdwinFormAccessor* formAccessor = EditorState()->FormAccessor();
        if (formAccessor)
            {
            CEikEdwin *edwin = static_cast<CEikEdwin *>( formAccessor->FormClientControl() );
            if (edwin)
                edwin->SetCursorPosL( aNewCursorPos.iPos, aDragSelectOn );
            }
        }
    }



TBool CAknFepManager::InFirstLineAndFormatted( const TTmDocPosSpec& aPos ) const
    {
    return DocPosInFirstOrLastLineAndFormatted( aPos, ETrue);
    }

TBool CAknFepManager::InLastLineAndFormatted( const TTmDocPosSpec& aPos ) const
    {
    return DocPosInFirstOrLastLineAndFormatted( aPos, EFalse );
    }

TBool CAknFepManager::DocPosInFirstOrLastLineAndFormatted( const TTmDocPosSpec& aPos, 
                                                           TBool aCheckFirstLine ) const
    {
    TBool success(EFalse);
    TTmLineInfo lineInfo;
    MAknFepDocumentNavigation* docNavi = AknFepDocumentNavigation();
    if (docNavi)
    	{
	    success = docNavi->DocPosToLine( aPos, lineInfo);
    	}
    else
    	{
	    CTextLayout* textLayout = TextLayout();
	    if ( textLayout )
	        {
	        success = textLayout->TagmaTextLayout().DocPosToLine( aPos, lineInfo);
	        }
    	}
    	
        if ( success )
            {
            if ( aCheckFirstLine )
                success = (lineInfo.iLineNumber == 0 );
            else
                {
                // Current position has yielded line information.  But how to tell if it is the last line?
                // Use the fact that the docpos of the last character will be in the last line.
                // Also account for the fact that these APIs report a character at docLength itself, a
                // ficticious end of paragraph character.  So use a >= docLength
                success = (lineInfo.iEnd >= 
                           iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep()  );
                }
            }
        
    return success;
    }

TBool CAknFepManager::AtVisualStart( const TTmDocPosSpec& aCurrentPos, 
                                     const TTmDocPosSpec& aLimitPos ) const
    {
    TBool positionsAreTheSame(EFalse);
    if ( aCurrentPos.iPos == aLimitPos.iPos && aCurrentPos.iType == aLimitPos.iType )
        positionsAreTheSame = ETrue;
    else if ( aCurrentPos.iPos == 0 && aCurrentPos.iType == TTmDocPosSpec::ETrailing )
        positionsAreTheSame = ETrue;

    return positionsAreTheSame;
    }

TBool CAknFepManager::AtVisualEnd( const TTmDocPosSpec& aCurrentPos, 
                                   const TTmDocPosSpec& aLimitPos, TInt aDocLength ) const
    {
    TBool positionsAreTheSame(EFalse);
    if ( aCurrentPos.iPos == aLimitPos.iPos && aCurrentPos.iType == aLimitPos.iType )
        positionsAreTheSame = ETrue;
    else if ( aCurrentPos.iPos == aDocLength && aCurrentPos.iType == TTmDocPosSpec::ELeading )
        positionsAreTheSame = ETrue;
    else if ( aCurrentPos.iPos > aDocLength )
        positionsAreTheSame = ETrue;

    return positionsAreTheSame;
    }


void CAknFepManager::SetCursorType(TBool aIsLeftToRight)
    {
    if ( !BidiCursorRequired() )
        return;

    // Optimization to prevent excessive Wserv communication:
    if ( IsFlagSet( EFlagBidiCursorIsInitialized )
        && COMPARE_BOOLS( !aIsLeftToRight, IsFlagSet( EFlagCursorPointsRightToLeft ) ) )
        return;

    SetFlag( EFlagBidiCursorIsInitialized );

    MCoeFepAwareTextEditor* editor = iInputCapabilities.FepAwareTextEditor();
    if (editor && editor->Extension1())
        {
        TCharFormat format;
        TInt docPos = DocPos().iPos;
        editor->GetFormatForFep(format, docPos);
        
        // Use Avkon routine to make all the layout decisions. It returns a TTextCursor object
        TTextCursor newCursor;
        AknCustomCursorSupport::GetBidiTextCursorFromFontSpec( format.iFontSpec, 
                                                               !aIsLeftToRight, newCursor );
        TBool setToTrue(ETrue);
        editor->Extension1()->SetCursorType(setToTrue, newCursor);
       
        // Keep flag in step for optimization
        if ( aIsLeftToRight )
            ClearFlag( EFlagCursorPointsRightToLeft );
        else
            SetFlag( EFlagCursorPointsRightToLeft );
        }
    }

TBool CAknFepManager::DeviceSupportsRTLLanguageL()
    {
    CArrayFixFlat<TInt>* languages = new (ELeave) CArrayFixFlat<TInt>(ELanguageArrayGranularity);
    CleanupStack::PushL(languages);
    iPtiEngine->GetAvailableLanguagesL(languages);

    TBool bidiCursorSuppressed(EFalse);
#if defined(__WINS__)
    bidiCursorSuppressed = ETrue;
    // Test (Disabled) mode (fake) UID, key and its value
    const TInt KTestModeValue = 1; // 1 = show Bidi cursor

    CRepository* repository = NULL;
    TRAPD(ret, repository = CRepository::NewL(KCRUidAknFep));
    if (ret == KErrNone)
        {
        TInt modeValue = 0;
        if (repository->Get(KAknFepTestModeKey, modeValue) == KErrNone)
            {
            if (modeValue == KTestModeValue)
                {
                bidiCursorSuppressed = EFalse; // show bidi cursor
                }
            }
        }
    delete repository;
#endif

    TBool result = EFalse;
    if (!bidiCursorSuppressed)
        {
        for (TInt jj = 0; jj < languages->Count(); jj++)
            {
            if (languages->At(jj) == ELangArabic ||
                languages->At(jj) == ELangHebrew ||
                languages->At(jj) == ELangFarsi  ||
                languages->At(jj) == ELangUrdu )
                // + others...
                {
                result = ETrue;
                break;
                }
            }
        }

    CleanupStack::PopAndDestroy();  // languages
    return result;
    }

void CAknFepManager::CalculateEditorDigitType(TDigitType& aDestination)
    {
    // When current range is latin number mode
    if ( iMode == ENumber )
        {
        // Set the Local Digit Type flag to EDigitTypeWestern
        aDestination = EDigitTypeWestern;
        return;
        }

    TBool ASCIIDigits = EFalse;
    MObjectProvider* mop = iInputCapabilities.ObjectProvider();

    if ( mop )
        {
        CAknExtendedInputCapabilities* extendedInputCapabilities = 
        mop->MopGetObject( extendedInputCapabilities );

        if ( extendedInputCapabilities ) 
            {
            if ( extendedInputCapabilities->SupportsCapabilities(
                CAknExtendedInputCapabilities::ESupportsOnlyASCIIDigits ) )
                {
                // If this is true, then the inputcaps wants only ascii
                // digits -> you can, when the editor is seen as numeric,
                // be sure that only latin indicator is then wanted.
                ASCIIDigits = ETrue;
                }
            }
        }

    if (iAknEditorFlags & EAknEditorFlagLatinInputModesOnly) 
        {
        // use of only latin characters implies that numbers are also western
        ASCIIDigits = ETrue;
        }

    AknTextUtils::TDigitModeQueryType editorType = AknTextUtils::EDigitModeEditorDefault;
    TBool numberOnlyEditor = iPermittedInputModes == EAknEditorNumericInputMode;
    if ( numberOnlyEditor )
        {
        editorType = AknTextUtils::EDigitModeNumberEditor;
        if (ASCIIDigits)
            {
            editorType = AknTextUtils::EDigitModeLatinNumberEditor;
            }
        }
        
    if (iLanguageCapabilities.iArabicIndicDigitsAllowed &&
            (AknTextUtils::DigitModeQuery(editorType) || iMode == ENativeNumber) && 
        !ASCIIDigits)
        {
        aDestination = EDigitTypeArabicIndic;
        }
    else if (iLanguageCapabilities.iEasternArabicIndicDigitsAllowed &&
            (AknTextUtils::DigitModeQuery(editorType) || iMode == ENativeNumber) && 
        !ASCIIDigits)
        {
        aDestination = EDigitTypeEasternArabicIndic;
        }    
    /*Hindi*/    
    else if ( iLanguageCapabilities.iIndicDigitsAllowed &&
            (AknTextUtils::DigitModeQuery(editorType) || iMode == ENativeNumber) && 
        !ASCIIDigits)    
        {
        aDestination = EDigitTypeDevanagari;    
        }
    else
        {
        aDestination = EDigitTypeWestern;
        }
    }


void CAknFepManager::UpdateNumericEditorDigitType()
    {
    if (iLanguageCapabilities.iNumericEditorDigitType == EDigitTypeUnknown)
        {
        CalculateEditorDigitType(iLanguageCapabilities.iNumericEditorDigitType);
        }
    }

void CAknFepManager::UpdateLocalDigitMode()
    {
    TInt oldLocalDigitType = iLanguageCapabilities.iLocalDigitType;
    CalculateEditorDigitType(iLanguageCapabilities.iLocalDigitType);
    
    if (oldLocalDigitType != iLanguageCapabilities.iLocalDigitType && 
        iCaseMan) // happens in destructor   
        {
        UpdateIndicators(); 
        }
    }

/**
* This routine should be called if the flag is set that says that the last navigation was to an
* ambiguous point.
* The routine checks to see if the event passed is
* - Right or Left Arrow
* - Would be the event to "jump" across the ambiguity. That is
*       - if at the "left" side of a LTR | RTL point, you need a Right arrow
*       - if at the "right" side of a LTR | RTL point, you need a Left arrow.
*       - if at the "left" side of a RTL | LTR point, you need a Right arrow
*       - if at the "right" side of a RTL | LTR point, you need a Left arrow.
*
*/
// TKeyResponse CAknFepManager::AttemptCursorFlipAtAmbiguousPointL( const TKeyEvent& aKeyEvent, TEventCode aEventCode )
TKeyResponse CAknFepManager::AttemptCursorFlipAtAmbiguousPointL( const TUint aCode )
    {
    TKeyResponse response = EKeyWasNotConsumed;

    if ( IsFlagSet(EFlagAtDirectionallyAmbiguousPoint)  // only perform if already at ambi-point?
        && !IsFlagSet(EFlagInsideInlineEditingTransaction) ) // do not flip if inside inline edit
        {
        if ( aCode == EKeyRightArrow || aCode == EKeyLeftArrow )
            {
            // Never use TextView() to get editor data, 
            // use FepAwareTextEditor() condition instead of TextView()
            if ( iInputCapabilities.FepAwareTextEditor() && EditorState() 
            && AknFepDocumentNavigation() )
                {
                TTmDocPosSpec docPos = DocPos();
                TTextDirectionalInfo status = LocalTextDirectionalStatus( docPos );

                CTmTextLayout::TTmChunkDescription leftChunk;
                CTmTextLayout::TTmChunkDescription rightChunk;
                FindAdjacentChunks(docPos, leftChunk, rightChunk);

                if ( status.iBlockEnvironment == 
                     TTextDirectionalInfo::EAtRightEndOfBlockFacingOppositeDirectionalBlock
                    && aCode == EKeyRightArrow )
                    {
                    response = EKeyWasConsumed;
                    // Set cursor to new directionality ( opposite to current )
                    SetCursorTypeForTextDirection( status.iDirectionalStatus == 
                                                   TTextDirectionalInfo::ERightToLeft );
                    // Must actually change the docpos!
                    AttachToRightChunk( docPos, rightChunk );
                    SetCursorPositionL( docPos, ETrue ); // Drag any existing selection
                    }
                else if (  status.iBlockEnvironment == 
                           TTextDirectionalInfo::EAtLeftEndOfBlockFacingOppositeDirectionalBlock
                    && aCode == EKeyLeftArrow )
                    {
                    response = EKeyWasConsumed;
                    // Effectively this toggles the direction of the cursor
                    SetCursorTypeForTextDirection( status.iDirectionalStatus == 
                                                   TTextDirectionalInfo::ERightToLeft );
                    // Must actually change the docpos!
                    AttachToLeftChunk( docPos, leftChunk );
                    SetCursorPositionL( docPos, ETrue ); // Drag any existing selection
                    }
                }
            }
        }
    return response;
    }


TInt CAknFepManager::PostEventCheckCallback(TAny* aObj)
    {
    TRAP_IGNORE(static_cast<CAknFepManager*>(aObj)->DoCursorDirectionCheckL());
    return KErrNone;
    }

TBool CAknFepManager::BidiCursorRequired() const
    {
    return iDeviceSupportsRtlLanguage;
    }

void CAknFepManager::DoCursorDirectionCheckL()
    {
   iPostEventCheck->Cancel();
    if(BidiCursorRequired()) 
        {
        AdjustCursorTypeForCurrentPosition();
        }

    if ( !BidiCursorRequired() 
    || (TAknFepUiIndicInputManager :: IsIndicLangauge(TLanguage(iLanguageCapabilities.iInputLanguageCode)) ))
            {
		/*Hindi*/
    	if( ( !WesternPredictive() ) && 
			( TAknFepUiIndicInputManager::IsIndicLangauge( 
					TLanguage( iLanguageCapabilities.iInputLanguageCode ) ) ) && 
			( ( TAknFepUiIndicInputManager::IsCharVirama( 
					PreviousChar( ETrue ), TLanguage( iLanguageCapabilities.iInputLanguageCode ) ) ) 
#ifdef RD_MARATHI
			|| ( IsEyeLashRaPresentL() && 
			TAknFepUiIndicInputManager::IsCharVirama(
					PreviousToPreviousChar( ETrue ), TLanguage( iLanguageCapabilities.iInputLanguageCode ) ) )
#endif
			)
    		&& !( iInputCapabilities.SupportsSecretText() )
    		)
			{
			TTmDocPosSpec pos = DocPos();
			pos.iType = TTmDocPosSpec::ETrailing;
		    TTmPosInfo2 info;
			TTmLineInfo lineInfo;
			TBool findAvailable = EFalse;
		    MAknFepDocumentNavigation* docNavi = AknFepDocumentNavigation();
		    if (docNavi)
		    	{
			    docNavi->FindDocPos(pos, info, lineInfo);
			    findAvailable = ETrue;
		    	}
		    else
		    	{
				CTextLayout* textLayout = TextLayout();
	    		if ( textLayout )
	    			{
					textLayout->TagmaTextLayout().FindDocPos(pos, info, lineInfo);	    					    
					findAvailable = ETrue;
	    			}
		    	}
		    if (findAvailable)
		    	{
				TCursorSelection sel(info.iDocPos.iPos  ,info.iDocPos.iPos );
				iInputCapabilities.FepAwareTextEditor()->SetCursorSelectionForFepL(sel);
		    	}
			}		
		}	
    }

void CAknFepManager::AdjustCursorTypeForCurrentPosition()
    {
    
    // Never use TextView() to get editor data, 
    // use FepAwareTextEditor() condition instead of TextView()
    if ( iInputCapabilities.FepAwareTextEditor() && EditorState() 
    && AknFepDocumentNavigation())
        AdjustCursorTypeForPosition( DocPos() );
    }

void CAknFepManager::SchedulePostEventCheckL( TUint aCode )
    {
    
    TBool isToPostEvent   = EFalse;    
    TBool isIndicLanguage = TAknFepUiIndicInputManager :: 
                            IsIndicLangauge(TLanguage(iLanguageCapabilities.iInputLanguageCode));
    
    if ( BidiCursorRequired() )
        {
        switch ( aCode )
            {
            // Navigation cases:
            case EKeyDownArrow:
            case EKeyUpArrow:            
            case EPtiKey1:
            case EPtiKey2:
            case EPtiKey3:
            case EPtiKey4:
            case EPtiKey5:
            case EPtiKey6:
            case EPtiKey7:
            case EPtiKey8:
            case EPtiKey9:
            case EPtiKey0:
                 {
                    isToPostEvent = ETrue;
                 }
                 break;
            case EKeyBackspace:
            case EKeyDelete:
                {
                    if(!isIndicLanguage)
                    {
                        isToPostEvent = ETrue;
                    }
                }
                break;
            case EKeyLeftArrow:
            case EKeyRightArrow:
                {
                    if(!isIndicLanguage)
                    {
                        isToPostEvent = ETrue;
                    }
                }
                break;                
            default:
                break;
            }

        // If Repha has been inserted then do not post event.
        if( isIndicLanguage )
            {
            if( aCode == EPtiKey1 && ( !iQwertyInputMode ) )
                {
                if( IsRephaPresent() 
#ifdef RD_MARATHI
                || IsEyeLashRaPresentL()
#endif
                )
                    {
                    isToPostEvent = EFalse;
                    }
                }
            }
        }
              
        if(isIndicLanguage) // iQwertyInputMode check is added so as to ensure that the event is not posted for Qwerty mode. 
        {
            if((aCode == EKeyBackspace || aCode == EKeyDelete) && (!iQwertyInputMode))
            {
                isToPostEvent = ETrue;
            }           
        } 
        
        if(isToPostEvent)
        {
            if ( iPostEventCheck->IsActive() )
                iPostEventCheck->Cancel();
            iPostEventCheck->Start( TCallBack(PostEventCheckCallback, this) );
        }                   
    }

void CAknFepManager::AdjustCursorTypeForPosition( const TTmDocPosSpec& aDocPos )
    {

    TTextDirectionalInfo dirInfo = LocalTextDirectionalStatus( aDocPos );
    // default clearing of this flag
    ClearFlag( EFlagAtDirectionallyAmbiguousPoint );

    switch ( dirInfo.iBlockEnvironment )
        {
        case TTextDirectionalInfo::EUndefined:
            // Set according to input direction
            SetCursorTypeForTextDirection( CurrentInputDirectionality() == 
                                           TBidiText::ELeftToRight );
            break; // do nothing

        // All unambiguous cases
        case TTextDirectionalInfo::EInsideBlock:
        case TTextDirectionalInfo::EAtRightEndOfLine:
        case TTextDirectionalInfo::EAtLeftEndOfLine:
        case TTextDirectionalInfo::EAtRightEndOfBlockFacingSameDirectionBlock:
        case TTextDirectionalInfo::EAtLeftEndOfBlockFacingSameDirectionBlock:
            SetCursorTypeForTextDirection( dirInfo.iDirectionalStatus == 
                                           TTextDirectionalInfo::ELeftToRight );
            break;

        // Ambiguous. Navigation has resulted in an ambiguous visual position. Set cursor according
        // to current direction
        case TTextDirectionalInfo::EAtRightEndOfBlockFacingOppositeDirectionalBlock:
        case TTextDirectionalInfo::EAtLeftEndOfBlockFacingOppositeDirectionalBlock:
            SetCursorTypeForTextDirection( dirInfo.iDirectionalStatus == 
                                           TTextDirectionalInfo::ELeftToRight );
            SetFlag( EFlagAtDirectionallyAmbiguousPoint );
            break;
        default:
            // Do nothing
            break;
        }
    }

void CAknFepManager::SetCursorTypeForTextDirection( TBool aLeftToRight )
    {
    SetCursorType( aLeftToRight );
    }

void CAknFepManager::SetCursorTypeForInputDirection( TBool /*aLeftToRight*/ )
    {
    // Do nothing
    }

CAknFepManager::TTextDirectionalInfo CAknFepManager::
                               LocalTextDirectionalStatus( const TTmDocPosSpec& aDocPos ) const
    {
    TTextDirectionalInfo directionalInfo;

    MCoeFepAwareTextEditor* fepAwareTextEditor = iInputCapabilities.FepAwareTextEditor();
    if ( !fepAwareTextEditor )
        return directionalInfo;

    if ( aDocPos.iPos < 0 ) // somehow, we are before the first character
        directionalInfo.iDirectionalStatus = TTextDirectionalInfo::ENotInDocument;
    else if ( aDocPos.iPos > fepAwareTextEditor->DocumentLengthForFep() ) // (iPos = docLen still is within document)
        directionalInfo.iDirectionalStatus = TTextDirectionalInfo::ENotInDocument;
    else if ( fepAwareTextEditor->DocumentLengthForFep() == 0 ) // zero length; no document
        directionalInfo.iDirectionalStatus = TTextDirectionalInfo::ENotInDocument;

    if ( directionalInfo.iDirectionalStatus == TTextDirectionalInfo::EUnknown )
        {
        CTmTextLayout::TTmChunkDescription leftChunk;
        CTmTextLayout::TTmChunkDescription rightChunk;
        FindAdjacentChunks(aDocPos, leftChunk, rightChunk);

    // Cases:
    // position is within a chunk (chunks are continuous in logical index and same directionality.
    //  ->Return directionality
    // position is betweeen two chunks of opposite directionality
    //  -> return simple ambiguity
    // position is between two chnks of same directionality, but logical index is not contiguous
    //  -> return complex ambiguity ; return directionality

        if ( leftChunk.iStart == -1 && rightChunk.iStart == -1 ) // 2 chunks invalid; direction stays EUnkonwn
            {
            directionalInfo.iDirectionalStatus = TTextDirectionalInfo::ENoDirectionalChunks;
            }
        else if ( leftChunk.iStart == -1 )  // no left block
            {
            directionalInfo.iBlockEnvironment = TTextDirectionalInfo::EAtLeftEndOfLine;
            directionalInfo.SetDirectionFromChunk( rightChunk );
            }
        else if ( rightChunk.iStart == -1 ) // no right block
            {
            directionalInfo.iBlockEnvironment = TTextDirectionalInfo::EAtRightEndOfLine;
            directionalInfo.SetDirectionFromChunk( leftChunk );
            }
        // Both chunks are the same direction.
        // We are either nicely inside a block or the two blocks are at +/- 2, 4, etc levels
        else if ( COMPARE_BOOLS( leftChunk.iRightToLeft, rightChunk.iRightToLeft ) )
            {
            // Can set direction from either chunk; use left one
            directionalInfo.SetDirectionFromChunk( leftChunk );

            if (leftChunk.iRightToLeft )
                {
                if ( leftChunk.iStart == rightChunk.iEnd ) // chunks are continguous
                    directionalInfo.iBlockEnvironment = TTextDirectionalInfo::EInsideBlock;
                else // Chunks are separated by 2, 4 etc. levels
                    directionalInfo.iBlockEnvironment = ( aDocPos.iPos == leftChunk.iStart ) ?
                        TTextDirectionalInfo::EAtRightEndOfBlockFacingSameDirectionBlock :
                        TTextDirectionalInfo::EAtLeftEndOfBlockFacingSameDirectionBlock;
                }
            else
                {
                if ( leftChunk.iEnd == rightChunk.iStart ) // chunks are continguous
                    directionalInfo.iBlockEnvironment = TTextDirectionalInfo::EInsideBlock;
                else // Chunks are separated by 2, 4 etc. levels
                    directionalInfo.iBlockEnvironment = ( aDocPos.iPos == leftChunk.iEnd ) ?
                        TTextDirectionalInfo::EAtRightEndOfBlockFacingSameDirectionBlock :
                        TTextDirectionalInfo::EAtLeftEndOfBlockFacingSameDirectionBlock;
                }
            }
        else // Blocks are in opposite direction. Have to deal with all the values of the iType enum
            {
            TInt charPos;
            switch( aDocPos.iType )
                {
                case TTmDocPosSpec::ETrailing: // position follows a block End
                    {
                    charPos = aDocPos.iPos - 1; //  attachment is to character[iPos-1] - now find it
                    if ( leftChunk.iStart <= charPos && charPos < leftChunk.iEnd ) // pos is in left chunk
                        {
                        directionalInfo.SetDirectionFromChunk( leftChunk );
                        directionalInfo.iBlockEnvironment = 
                            TTextDirectionalInfo::EAtRightEndOfBlockFacingOppositeDirectionalBlock;
                        }
                    else if( rightChunk.iStart <= charPos && charPos < rightChunk.iEnd ) // pos is in right chunk
                        {
                        directionalInfo.SetDirectionFromChunk( rightChunk );
                        directionalInfo.iBlockEnvironment = 
                            TTextDirectionalInfo::EAtLeftEndOfBlockFacingOppositeDirectionalBlock;
                        }
                    else // Probably impossible situation.  Supply reasonable defaults rather than panicking
                        {
                        directionalInfo.iDirectionalStatus = TTextDirectionalInfo::ELeftToRight;
                        directionalInfo.iBlockEnvironment = 
                            TTextDirectionalInfo::EAtLeftEndOfBlockFacingOppositeDirectionalBlock;
                        }
                    break;
                    }
                case TTmDocPosSpec::ELeading: // position precedes a block start
                    {
                    charPos = aDocPos.iPos; //  attachment is to character[iPos] - now find it
                    if ( leftChunk.iStart <= charPos && charPos < leftChunk.iEnd ) // pos is in left chunk
                        {
                        directionalInfo.SetDirectionFromChunk( leftChunk );
                        directionalInfo.iBlockEnvironment = 
                            TTextDirectionalInfo::EAtRightEndOfBlockFacingOppositeDirectionalBlock;
                        }
                    else if( rightChunk.iStart <= charPos && charPos < rightChunk.iEnd ) // pos is in right chunk
                        {
                        directionalInfo.SetDirectionFromChunk( rightChunk );
                        directionalInfo.iBlockEnvironment = 
                            TTextDirectionalInfo::EAtLeftEndOfBlockFacingOppositeDirectionalBlock;
                        }
                    else // Probably impossible situation.  Supply reasonable defaults rather than panicking
                        {
                        directionalInfo.iDirectionalStatus = TTextDirectionalInfo::ELeftToRight;
                        directionalInfo.iBlockEnvironment = 
                            TTextDirectionalInfo::EAtLeftEndOfBlockFacingOppositeDirectionalBlock;
                        }
                    break;
                    }
                case TTmDocPosSpec::ELeftToRight: // Is attached to a Left to right block
                    {
                    directionalInfo.iDirectionalStatus = TTextDirectionalInfo::ELeftToRight;
                    // If left chunk is RTL, then we are NOT attached to that left block
                    // (ie. we are at left end of the right block)
                    directionalInfo.iBlockEnvironment = leftChunk.iRightToLeft ?
                        TTextDirectionalInfo::EAtLeftEndOfBlockFacingOppositeDirectionalBlock : // must be in right chunk then (left end of it)
                        TTextDirectionalInfo::EAtRightEndOfBlockFacingOppositeDirectionalBlock; // else in left chunk (right end of it)
                    break;
                    }
                case TTmDocPosSpec::ERightToLeft:
                    {
                    directionalInfo.iDirectionalStatus = TTextDirectionalInfo::ERightToLeft;
                    // If left chunk if RTL, then we are attached to that left block (ie. at right end of it)
                    directionalInfo.iBlockEnvironment = leftChunk.iRightToLeft ? // is it in left chunk?
                        TTextDirectionalInfo::EAtRightEndOfBlockFacingOppositeDirectionalBlock : // right end of left chunk
                        TTextDirectionalInfo::EAtLeftEndOfBlockFacingOppositeDirectionalBlock;
                    break;
                    }
                }

            }
        }
    return directionalInfo;
    }


/**
* Redecoration is carried out if a neutral is exposed by the deletion to a directionality
* different to that which is was already marked.
*
* The routine leaves iUncommittedText again set to the (possibly new) cursor position.
*
*/
void CAknFepManager::RedecorateAfterDeletionL( const TDesC& aTextThatWasDeleted  )
    {
    if (!EditorSupportsNeutralProtection())
        return;

    if ( aTextThatWasDeleted.Length() == 0 )
        return;

    TBool protect = ETrue;

    // Set up iUncommittedText with the current cursor position so that PreviousChar will work:
    iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(iUncommittedText);

    TTmDocPosSpec docPos = DocPos();
    TInt docLen = iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep();

    TInt previousPos = docPos.iPos;
    // Trailing iType actually points to character at index iPos - 1
    if( docPos.iType == TTmDocPosSpec::ETrailing )
        previousPos = previousPos -= 1;
    TInt nextPos = previousPos + 1;

    // Perform no neutral protection if the beginning of the text is deleted.
    // Note that there MAY be protection occuring if the delete is from the end so there is
    // no corresponding test on DocLength. That is handled later where the paragraph direction
    // is used for the "forward" directionality.
    if ( previousPos < 0 )
        return;

    // Up to 2 protection characters, one RTL and one LTR
    TBuf<2> textToInsert;

    TBool adjacentPrecedingIsNeutral(EFalse);
    TBool adjacentTrailingIsNeutral(EFalse);

    // See if either preceding or next is neutral
    adjacentPrecedingIsNeutral = iInlineTextDecorator->CharacterIsNeutral( CharAt(previousPos) );
    if ( nextPos < docLen )
        adjacentTrailingIsNeutral = iInlineTextDecorator->CharacterIsNeutral( CharAt(nextPos) );

    if ( !adjacentPrecedingIsNeutral && !adjacentTrailingIsNeutral )
        protect = EFalse;

    // Inits actually correspond to LTR, but they are only used if the bools indicating
    // strong directionality found get set
    TBool directionPrecedingDeleteIsRTL(EFalse);
    TBool directionTrailingDeleteIsRTL(EFalse);
    if ( protect )
        {
        TBool strongPreceding = GetExposedDirectionOfText( previousPos, EFalse, 
                                                           directionPrecedingDeleteIsRTL );
        TBool strongTrailing = GetExposedDirectionOfText( nextPos, ETrue, 
                                                          directionTrailingDeleteIsRTL );
        if (!strongTrailing)
            {
            TBool isRTLPara = IsRightToLeftParagraph(docPos);
            directionTrailingDeleteIsRTL = isRTLPara;
            strongTrailing = ETrue;
            }
        if ( !strongPreceding || !strongTrailing) // Must have strong in both directions
            protect = EFalse;
        else if ( COMPARE_BOOLS( directionPrecedingDeleteIsRTL, directionTrailingDeleteIsRTL ) ) // Must differ
            protect = EFalse;
        }

    TBool forwardProtection(EFalse);

    if ( protect )
        {
        protect = EFalse;
        TBool deletedWasRTL;

        // Check for and do reverse protection
        if ( adjacentPrecedingIsNeutral )
            {
            TBool deletedTextIsStrong = GetExposedDirectionOfTextInDescriptor( aTextThatWasDeleted, 
                                                                               ETrue, 
                                                                               deletedWasRTL ); // search forward into deleted stuff
            if ( deletedTextIsStrong && (directionPrecedingDeleteIsRTL == deletedWasRTL))
                {
                protect = ETrue;
                forwardProtection = EFalse;
                if ( deletedWasRTL )
                    textToInsert.Append( CAknFepInlineTextDecorator::ERightToLeftMark );
                else
                    textToInsert.Append( CAknFepInlineTextDecorator::ELeftToRightMark );
                }
            }

        // Check for and do forward protection
        // Note it is possible to have both forward and reverse redecoration.
        if ( adjacentTrailingIsNeutral )
            {
            TBool deletedTextIsStrong = GetExposedDirectionOfTextInDescriptor( aTextThatWasDeleted, 
                                                                               EFalse, 
                                                                               deletedWasRTL ); // search backward in deleted stuff
            if (deletedTextIsStrong && (directionTrailingDeleteIsRTL == deletedWasRTL) )
                {
                protect = ETrue;
                forwardProtection = ETrue;
                if ( deletedWasRTL )
                    textToInsert.Append( CAknFepInlineTextDecorator::ERightToLeftMark );
                else
                    textToInsert.Append( CAknFepInlineTextDecorator::ELeftToRightMark );
                }
            }
        }


    if ( protect )
        {
        StartInlineEditL(textToInsert);
        CommitInlineEditWithoutResolutionL(*(iInputCapabilities.FepAwareTextEditor()), ETrue);
        // iUncommittedText is now set to correct value by the Commitxxxx method
        // Document position is adjusted to as to keep the cursor stable.
        // Make a docPos. If reverse protection, then the position should trail
        TTmDocPosSpec docPos( iUncommittedText.LowerPos() , TTmDocPosSpec::ETrailing );
        // But if there has been forward Protection, then we have to back up the cursor by 1
        // and lead. (This is OK too, if there is simultaneous reversse and forward protection)
        if ( forwardProtection )
            {
            docPos.iPos = Max(docPos.iPos - 1, 0 );
            docPos.iType = TTmDocPosSpec::ELeading;
            }
        SetCursorPositionL( docPos );
        }
    }

TBool CAknFepManager::EditorSupportsNeutralProtection()
    {
    TBool ret = ETrue;
    if ( !IsFepAwareTextEditor() )
        ret = EFalse;

    else if (iInputCapabilities.SupportsSecretText())
        ret = EFalse;

    else if ( iAknEditorFlags & EAknEditorFlagLatinInputModesOnly )
        ret = EFalse;

    else if (iPermittedInputModes == EAknEditorNumericInputMode)
        ret = EFalse;

    else if ( !CharIsValidInEditor( CAknFepInlineTextDecorator::ERightToLeftMark ) )
        ret = EFalse;

    return ret;
    }

void CAknFepManager::CommitInlineEditWithoutResolutionL(MCoeFepAwareTextEditor& aFepAwareTextEditor, 
                                                        TBool aCursorVisibility , TBool aClearPti )
    {
    aFepAwareTextEditor.SetInlineEditingCursorVisibilityL(aCursorVisibility);
    /*
    On any call to CommitFepInlineEditL, the mode will change
    even when a leave event occurs.
    */
    ClearFlag(EFlagInsideInlineEditingTransaction | EFlagInsideMultitapInlineEditingTransaction);
    ClearCcpuFlag(ECcpuStateNewPredictiveWord);
    
    if(aClearPti)
        {
        iPtiEngine->CommitCurrentWord();
        }
    CCoeEnv* coeEnv = CCoeEnv::Static();
    aFepAwareTextEditor.CommitFepInlineEditL(*coeEnv);
    aFepAwareTextEditor.GetCursorSelectionForFep(iUncommittedText);
    // iMatchState=EWordMatchFirst;
    iCaseMan->UpdateCase(ENullNaviEvent);
    }

void CAknFepManager::WordConcatenationTimerTimeoutL()
    {
    iConcatenationTimer->Cancel();

    // add concatenated word to the UDB (a concatenated word is a compound word)
    AddCompoundWordToUdbL();

#ifdef RD_INTELLIGENT_TEXT_INPUT
	MoveCursorToEndOfWordL();
#endif
    }


TKeyResponse CAknFepManager::HandlePredictiveNaviEventOutsideInlineEditL(TUint aCode, 
                                                                         TKeyPressLength aLength)
    {
    TKeyResponse response = EKeyWasNotConsumed;

    TWesternNaviEvent naviEvent = ENullNaviEvent;
    if (aCode == EKeyLeftArrow)
        {
        naviEvent = ELeftNaviEvent;
        }
    else if (aCode == EKeyRightArrow)
        {
        naviEvent = ERightNaviEvent;
        }
    else if (aCode == EKeyBackspace)
        {
        naviEvent = EBackspaceEvent;
        }

    TInt nextCharPos = 0;  // Character ordinal in logical memory (0 - n-1)
    TBool nextPositionFound = EFalse;
    if ( naviEvent == ERightNaviEvent )
        {
        nextPositionFound = GetNextVisualRightCharacter( nextCharPos );
        }
    else if ( naviEvent == ELeftNaviEvent )
        {
        nextPositionFound = GetNextVisualLeftCharacter( nextCharPos );
        }
    else if ( naviEvent == EBackspaceEvent )
        {
        if (iLanguageCapabilities.iRightToLeftLanguage)
            {
            TChar character = PreviousChar();
            TChar::TBdCategory bdCategory = character.GetBdCategory();
            // In case last character in buffer is number or Latin characters, needs to delete 
            // character from left.
            if (TChar::EEuropeanNumber == bdCategory || TChar::EArabicNumber == bdCategory ||
                TChar::ELeftToRight == bdCategory )
                {
                nextPositionFound = GetNextVisualLeftCharacter( nextCharPos );
                }
            else
                {
                nextPositionFound = GetNextVisualRightCharacter( nextCharPos );
                }
            }
        else
            {
            nextPositionFound = GetNextVisualLeftCharacter( nextCharPos );
            }
        }

    if ( !nextPositionFound )
        {
        response = EKeyWasNotConsumed;
        }
    else // not looping - lets see what to do
        {
        TBuf<ESingleCharacter> characterBuffer;
        iInputCapabilities.FepAwareTextEditor()->GetEditorContentForFep(characterBuffer, 
                                                                        nextCharPos, 
                                                                        ESingleCharacter);

        if ( !IsValidInLineCharacter( STATIC_CAST(TChar, characterBuffer[0])) )
            {
            // if the character is a non-alpha character; let it go
            response=EKeyWasNotConsumed;
            if (IsFlagSet(EFlagCompoundWord))
                {
                if  (STATIC_CAST(TChar, characterBuffer[0]).IsSpace())
                    // we are navigating over a space so add any current compond word to the udb
                    AddCompoundWordToUdbL();
                ClearFlag(EFlagCompoundWord);
                }
        //allow to popup editsubmenu        
        ClearCcpuFlag(ECcpuStateIgnoreStarUp);
            }
        else
            {
            if (FepUI()->HandleKeyL(aCode, aLength))
                {
                response=EKeyWasConsumed;
                }
            }
        }
    return response;
    }

/**
 * Call back function:
 */
TInt CAknFepManager::DoSimulateKey(TAny* aThisPtr)
    {
    CAknFepManager* myobj = static_cast<CAknFepManager*>(aThisPtr);
    TRAPD(error, myobj->HandleOwnedSimulateKeyL());
    return error;
    }

void CAknFepManager::HandleOwnedSimulateKeyL()
    {
#ifdef RD_INTELLIGENT_TEXT_INPUT         
#ifdef __HALF_QWERTY_KEYPAD
        if( KeyboardLayout() == EPtiKeyboardHalfQwerty )
            {            
            CCoeEnv::Static()->SimulateKeyEventL( iHalfQwertyLastKeyEvent, EEventKey );
            }        
        else
#endif //__HALF_QWERTY_KEYPAD        
#endif //RD_INTELLIGENT_TEXT_INPUT
    if (iSimulateKey > 0)
        {
        HandleKeyEventL(iSimulateKey, EShortKeyPress);
        iSimulateKey = 0;
        }
    }

/**
 * This method is that the label of CBA is updated by aTextResId.
 * @param aPosition   CBA position, either ELeftSoftkeyIndex or ERightSoftkeyIndex
 * @param aCommandId  command id for softkey
 * @param aTextResId  resource id to set string on label.
 * @return TBool The return value is ETrue in case it is necessary to update CBA.
 */
TBool CAknFepManager::UpdateCBALabelL(TInt aPosition, TInt aCommandId, TInt aTextResId)
    {
    return iUiInterface->UpdateSoftkeyLabelL(aPosition, aCommandId, aTextResId);
    }

void CAknFepManager::DoLaunchSctAndPctL(TInt aResourceId, TShowSctMode aShowSctMode)
    {
    TBool replace = EFalse;
    TBool disableRecentItemRow = EFalse;
    
    if (ExtendedInputCapabilities() & CAknExtendedInputCapabilities::EDisableSCT)
        {
        // The editor doesn't allow SCT.
        return;
        }
    if ( EditorType() == CAknExtendedInputCapabilities::EEikSecretEditorBased )
        {
        
        disableRecentItemRow = ETrue;
        }    
    
    if (IsCcpuFlagSet(ECcpuStateCbaSymbol) && 
        iMode == ELatin && WesternPredictive(ELatin) && 
        iPtiEngine->LastEnteredKey() == EPtiKey1)
        {
        // User is entering special character as result of pressing key '1' and
        // cycling through candidates until left softkey turns into "Symbol". In
        // that case we want to replace last cahracter instead of adding new one.
        replace = ETrue;
        }
    
    //Commit characters in latin multitap as some editors may call CancelFepTransaction()
    //which will be received before the focus change notification.
    if (IsFeatureSupportedJapanese())
        {
        TryCloseUiL();
        }
    else
        {
#ifdef RD_INTELLIGENT_TEXT_INPUT
        if(!(WesternPredictive()))
#endif
        CommitInlineEditL();
        }

    if (!EditorHasFreeSpace())
        {
        return;
        }
    
    // get the SCT resource id from editor default settings
    TInt currentEditorSCTResId = GetCurrentEditorSCTResId();
    TInt charMap = aResourceId;
    if ( FeatureManager::FeatureSupported(KFeatureIdChinese) )
        {        
        if (charMap == R_AVKON_SPECIAL_CHARACTER_TABLE_DIALOG)
            {
            charMap = R_AVKON_SPECIAL_CHARACTER_TABLE_DIALOG_CHINESE;
            }
        }
    else if ( FeatureManager::FeatureSupported(KFeatureIdKorean) )
        {     
        // Use the special sct resource file for Korean variant.
        if (charMap == R_AVKON_SPECIAL_CHARACTER_TABLE_DIALOG)
            {
            charMap = R_AVKON_SPECIAL_CHARACTER_TABLE_DIALOG_KOREAN;
            }
        }
    if (!charMap)
        {
        // override numeric keymap if necessary. flag is for non standard charmaps
        TInt standardNumericCharMap = EDefaultNumericCharMapResId;
        if ((iMode == ENumber || iMode == ENativeNumber) &&
            !(iAknEditorFlags & EAknEditorFlagUseSCTNumericCharmap) &&
            (iCharWidth == EHalfWidthChar) &&
            ( (iAknEditorNumericKeymap != EAknEditorAlphanumericNumberModeKeymap) ||
              (iPermittedInputModes == EAknEditorNumericInputMode) ))
            {
            standardNumericCharMap = NumericModeSCTResourceId();
            }
        charMap = standardNumericCharMap;
        }
        
    if ( charMap == ENoCharacters && !IsAbleToLaunchSCT())
        {
    	currentEditorSCTResId = charMap;
        }
    if(R_AKNFEP_SCT_NUMERIC_MODE_CHARS_PLAIN == charMap)
        {
        TBool isEmpty = ETrue;
        TRAP_IGNORE(isEmpty = GetSctLengthL(charMap));
        if(isEmpty)
           {
           return;
           }
        }

    if (currentEditorSCTResId)
        {
        TBuf<32> specialChars;

        TInt sctChars = EAknSCTChiChineseInputModes;
        TInt afterSctChars = EAknSCTHalfCase;

        if (iCharWidth == EFullWidthChar)
            {
            sctChars = EAknSCTFullCase;
            if (iMode == ELatin)
                {
                afterSctChars = EAknSCTUpperCase;
                if (iCaseMan->CurrentCase() == EAknEditorLowerCase)
                    {
                    afterSctChars = EAknSCTLowerCase;
                    }
                }
            else if (iMode == ENumber||iMode == ENativeNumber)
                {
                afterSctChars = EAknSCTLowerCase;
                }
            }
        else
            {
            if (iMode == ELatin)
                {
                if ( FeatureManager::FeatureSupported(KFeatureIdChinese) )
                    {
                    sctChars = EAknSCTChiLatinInputModes;
                    }
                else
                    {
                    if (iQwertyInputMode)
                        {
                        sctChars = EAknSCTQwerty;
                        afterSctChars = EAknSCTFullCase;                        
                        }
                    else
                        {                                           
                        sctChars = EAknSCTUpperCase;
                        afterSctChars = EAknSCTFullCase;
                        if (iCaseMan->CurrentCase() == EAknEditorLowerCase)
                            {
                            sctChars = EAknSCTLowerCase;
                            }
                        }
                    }
                }
            else if (iMode == ENumber||iMode == ENativeNumber)
                {
                sctChars = EAknSCTNumeric;
                if ( ((iAknEditorFlags & EAknEditorFlagUseSCTNumericCharmap) ||
                      (iAknEditorNumericKeymap == EAknEditorAlphanumericNumberModeKeymap)) &&
                      !IsOnlyHalfWidthCharacterPermitted() )
                    {
                    sctChars = EAknSCTLowerCase;
                    afterSctChars = EAknSCTFullCase;
                    }
                }
            else if (iMode == EHiraganaKanji)
                {
                sctChars = EAknSCTFullCase;
                }
            else if(iMode == EKatakana)
                {
                sctChars = EAknSCTHalfCase;
                afterSctChars = EAknSCTFullCase;
                }
            }

        SetFlag(EFlagForegroundUIComponentVisible);

        TBool showAnotherTable = EFalse;
        if (IsFeatureSupportedJapanese())
            {
            if (sctChars == EAknSCTFullCase)
                {
                if (!IsOnlyFullWidthCharacterPermitted())
                    {
                    showAnotherTable = ETrue;
                    }
                }
            else if (sctChars == EAknSCTHalfCase || sctChars == EAknSCTUpperCase 
                  || sctChars == EAknSCTLowerCase)
                {
                if (!IsOnlyHalfWidthCharacterPermitted() 
                 && !(iAknEditorFlags & EAknEditorFlagLatinInputModesOnly))
                    {
                    showAnotherTable = ETrue;
                    }
                }
            }
#ifdef RD_SCALABLE_UI_V2     
        if( iFepFullyConstructed && !iFepPluginManager->IsGlobleNotes())
            {
             if (iFepPluginManager->PluginInputMode() == EPluginInputModeItut ||
                 iFepPluginManager->PluginInputMode() == EPluginInputModeFSQ ||
                 iFepPluginManager->PluginInputMode() == EPluginInputModeFingerHwr ||
                 iFepPluginManager->PluginInputMode() == EPluginInputModePortraitFSQ )

                {
                SetStopProcessFocus(ETrue, EFalse);
                iFepPluginManager->SetMenuState();            
                }                      
             else if (!iFepPluginManager->IsGlobleNotes())
                {
                iFepPluginManager->ClosePluginInputUiL( ETrue );
                }
            }
#endif  

        TBool lockNumericKeys = ( iPtiEngine->IsQwertyBasedMode(iPtiEngine->InputMode())) 
        #ifdef RD_INTELLIGENT_TEXT_INPUT        
                            || ( iPtiEngine->InputMode() == EPtiEngineHalfQwerty ) ||
                            ( iPtiEngine->InputMode() == EPtiEngineHalfQwertyPredictive ) 
        #endif                            
                            ;
        TUid fepUid = CCoeEnv::Static()->FepUid();

        if(IsAbleToLaunchSmiley())
            {
            if(aShowSctMode == EShowSmileyFirst)
                {
                sctChars |= EAknCharMapEmotionFirst;
                }
            else
                {
                sctChars |= EAknCharMapEmotionUse;
                }
            }
        ClearExtendedFlag(EExtendedFlagEdwinEditorDestroyed);
        
        if ( iFepPluginManager && iFepPluginManager->IsSpellVisible() )
        	{
            iFepPluginManager->SetLaunchSCTInSpell( ETrue );            
        	}        	
        SendEventsToPluginManL(EPluginEnablePriorityChangeOnOriChange,EFalse);
		if (iUiInterface->CharMapDialogL(
			sctChars, 
			specialChars, 
			charMap,
			lockNumericKeys,
            aShowSctMode==EShowPctFirst,
			!IsAbleToLaunchPCT(),
			showAnotherTable,
			afterSctChars,
			currentEditorSCTResId,
			disableRecentItemRow))
            {
            PrepareFepAfterDialogExitL(fepUid);
            SetCcpuFlag(ECcpuStateSupressCursorMoveToEnd);
			// after closing SCT, need change dim state.
            SendEventsToPluginManL(EPluginEnableFetchDimState);
#ifdef RD_SCALABLE_UI_V2 
            iNotifyPlugin = EFalse;
            if( iFepFullyConstructed && iFepPluginManager)
                {
                 if (iFepPluginManager->PluginInputMode() == EPluginInputModeItut ||
                     iFepPluginManager->PluginInputMode() == EPluginInputModeFSQ ||
                     iFepPluginManager->PluginInputMode() == EPluginInputModeFingerHwr ||
                     iFepPluginManager->PluginInputMode() == EPluginInputModePortraitFSQ )
                    {
                    SetStopProcessFocus(EFalse);
                    }                      
                 else
                    {
                    HandleChangeInFocus();
                    }
                }
            else
                {
                HandleChangeInFocus();
                }   
            // Set the flag, not change focus for next.
            iNotifyPlugin = ETrue;
#endif            

            //Removes auto-complition part if SCT is launched and any special character is selected 
#ifdef RD_INTELLIGENT_TEXT_INPUT
            if( specialChars.Length() > 0 )
                {
            	RemoveSuggestedAdvanceCompletionL();
                }
#endif // RD_INTELLIGENT_TEXT_INPUT
            TBuf<ESingleCharacter> charAsDesc(ESingleCharacter);
            for ( TInt ii = 0; ii < specialChars.Length(); ii++)
                {
                charAsDesc[0]=(TText)specialChars[ii];

                if (iInputCapabilities.SupportsSecretText())
                    {
                    TChar ch = charAsDesc[0];
                    if (ch == TText(0x000A) || ch == TText(CEditableText::EParagraphDelimiter))
                        // 0x000A is ascii, EParagraphDelimiter is unicode - future
                        // proofs against sct changing to unicode
                        ch = EKeyEnter;
                    if (CharIsValidInEditor(ch))
                        {
                        //we receive an extra key press if we simulate a key that is also a device key
                        if (ch.IsDigit() || TUint(ch) == EStarKeyUnicodeValue 
                         || TUint(ch) == EHashKeyUnicodeValue)
                            SetFlag(EFlagPassNextKey);
                        SimulateKeyEventL(ch);
                        }
                    }
                else
                    {
                    if (charAsDesc[0] == TText(0x000A)) // 0x000A is line feed
                        {
                        charAsDesc[0] = CEditableText::EParagraphDelimiter;
                        }
                        
                    if (CharIsValidInEditor(charAsDesc[0]))
                        {
                        TCursorSelection cursorSelection(0,0);
                        if ((iAknEditorFlags & EAknEditorFlagMTAutoOverwrite) && iMode == ELatin 
                          && !WesternPredictive())
                            {
                            TInt edSize = 
                                   iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep();
                            iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(iUncommittedText);
                            if (iUncommittedText.iCursorPos < edSize 
                             && iUncommittedText.Length() == 0)
                                {
                                cursorSelection = TCursorSelection(iUncommittedText.iCursorPos + 1,
                                                                   iUncommittedText.iAnchorPos);
                                }
                            }
                        if (EditorState())
                            {
                            if (charAsDesc[0] == CEditableText::EParagraphDelimiter)
                                {
                                // This is line feed character. Post it to editor and let it decide
                                // if it is valid.
                                 if (!(iAknEditorFlags & EAknEditorFlagFindPane) 
                                    && specialChars.Length() == 1)
                                    { 
#ifdef RD_INTELLIGENT_TEXT_INPUT        
                                    CommitInlineEditL();                                    
#endif
                                   
                                    SimulateKeyEventL(EKeyEnter);   
                                    SimulateKeyEventL(EKeyF19); // Asyncronous case update                                                                                              
                                    }
                                }                                                        
                            else
                                {                                                                                                                                   
                                if (replace)
                                    {
                                    TKeyEvent delKey = {8, EStdKeyBackspace, 1, 0};        
                                    CCoeEnv::Static()->SimulateKeyEventL(delKey, EEventKey);                                                                
                                    }                               
//There may be several char to be inserted, need disable Focus change to avoid multi-focus change.
#ifdef RD_SCALABLE_UI_V2 
#endif       
                                InsertTextFromDialogL(charAsDesc, cursorSelection);
                                }
                            // Clear dead key, vowel sequence and vietnamese tone mark states.                         
                            iPtiEngine->HandleCommandL(EPtiCommandGetAndClearLastVietnameseChar, 
                                                       NULL);
                            iPtiEngine->HandleCommandL(EPtiCommandClearVowelSeq, 
                                                       NULL);
                            iPtiEngine->HandleCommandL(EPtiCommandGetAndClearDeadKeyRootChar, 
                                                       NULL);                            
                            }
                        }
                    }
                }
            }
        else
            {
            iClosePeninputUi = EFalse;
            iStopProcessFocus = EFalse;                   
            }		   
        PrepareFepAfterDialogExitL(fepUid);
		SendEventsToPluginManL(EPluginEnablePriorityChangeOnOriChange,ETrue);
        }
    }

/**
 * Queries supportting of SecretText
 *
 * @since 2.6
 * @return ETrue if SecretText was supported
 */
TBool CAknFepManager::IsSupportsSecretText() const
    {
    TBool bSecret = iInputCapabilities.SupportsSecretText();
    if( !bSecret )
        {
        //test java editor
        MObjectProvider* mop = iInputCapabilities.ObjectProvider();            	
        if ( mop )
           {
            CAknExtendedInputCapabilities* extendedInputCapabilities;
            extendedInputCapabilities = mop->MopGetObject( extendedInputCapabilities );
            if ( extendedInputCapabilities ) 
                {
                TUint constraints = extendedInputCapabilities->MIDPConstrainst();
                if( constraints & 0x10000 )
                    {
                    bSecret = ETrue;
                    }
                }
            }
        }
    
    return bSecret;
    }

void CAknFepManager::SendEventsToPluginManL( TInt aEventType, TInt aEventData )
    {
#ifdef RD_SCALABLE_UI_V2
    if ( iFepFullyConstructed )
        {
        iFepPluginManager->HandleEventsFromFepL( aEventType, aEventData );
        }
#endif //RD_SCALABLE_UI_V2
    }
    
void CAknFepManager::UpdateCangJieState(TInt aNewMode)
    {
    CAknFepUIManagerChinese* fepUiMgr = STATIC_CAST(CAknFepUIManagerChinese*, 
                                             iLangMan->GetFepUI(ECangJie, ENoneWidthChar, EFalse));
    
    if (aNewMode >= 0)
        {
        if ( fepUiMgr )
            {
            fepUiMgr->UpdateCangJieState();
            }
        
        // Store CangJie settings into the sharedata
        iSharedDataInterface->SetCangJieMode(aNewMode);            
        }
    }

TInt CAknFepManager::LaunchCangJieOptionDlgL()
    {
    // Get the CangJie level from the GS
    TInt currentLevel = iSharedDataInterface->CangJieMode();
    
    if ((currentLevel != ECangJieNormal) 
    && (currentLevel != ECangJieEasy) 
    && (currentLevel != ECangJieAdvanced))
        return KErrGeneral;

    // Launch the CangJie setting menu
    CDesCArrayFlat* items = CCoeEnv::Static()->ReadDesC16ArrayResourceL(R_AVKON_CANGJIE_OPTIONS_LBX);
    CleanupStack::PushL(items);

    TUid fepUid = CCoeEnv::Static()->FepUid();   
    ClearExtendedFlag(EExtendedFlagEdwinEditorDestroyed);
	TInt ret = iUiInterface->LaunchCangJieOptionDlgL(
        R_AVKON_CANGJIE_MODE_OPTIONS_SETTING_PAGE,
        currentLevel,
        items
        );
    PrepareFepAfterDialogExitL(fepUid);

    CleanupStack::PopAndDestroy(items);

    return ret ? currentLevel : KErrGeneral;
    }

GLDEF_C void AknFepPanic(TAknFepPanic aPanic)
    {
    User::Panic(_L("AKNFEP"),aPanic);
    }

/**
 * Set Multitap timer value. 
 *
 * @since 3.0
 * @param Value of KAknFepMultiTapTimer setting
 */
void CAknFepManager::SetMultiTapTimer(const TInt aMultiTapTimer)
    {
    if (iMultiTapTimer != aMultiTapTimer)
        {
        iMultiTapTimer = aMultiTapTimer;
        }
    }

/**
 * Set Japanese Qwerty Flags value. 
 *
 * @since 3.0
 * @param Value of KAknFepJapaneseSpecialCharFlag setting
 */
void CAknFepManager::SetJapaneseQwertyFlags(const TInt aJapaneseQwertyFlags)
    {
    if (iJapaneseQwertyFlags != aJapaneseQwertyFlags)
        {
        iJapaneseQwertyFlags = aJapaneseQwertyFlags;
        }
    }

/**
 * Handle the Chr Key Monitor
 * for Japanese variant only.
 *
 * @since 3.0
 * @return KErrNone if succeed, KErrDied if failed
 */
TInt CAknFepManager::HandleChrKeyMonitorCallback(TAny* aObj)
    {
    TRAPD(err, static_cast<CAknFepManager*>(aObj)->HandleChrKeyMonitorL());
    if (err)
        {
        static_cast<CAknFepManager*>(aObj)->CleanUpFep();
        return KErrDied;
        }
    return KErrNone;
    }

/**
 * Handle the Chr Key Monitor
 * for Japanese variant only.
 *
 * @since 3.0
 */
void CAknFepManager::HandleChrKeyMonitorL()
    {
#ifdef RD_INTELLIGENT_TEXT_INPUT        
#ifdef __HALF_QWERTY_KEYPAD        
    if ( IsFlagSet(EFlagQwertyChrKeyDepressed) && 
    		  IsFlagSet(EFlagNoActionDuringChrKeyPress) && 
    		  (EPtiKeyboardHalfQwerty == KeyboardLayout() ) && 
    		  ( iWesternPredictive || iMode == EPinyin || 
    		     iMode == EStroke || iMode == EZhuyin ) )   
    	{
    	// Clear the chr key depressed flag
    	TBool validchr = FepUI()->IsValidLongChrKeyPress();
    	
    	if(!validchr || !IsAbleToLaunchSCT() )
    		{
			  FepUI()->HandleKeyL( EStdKeyLeftFunc, EShortKeyPress );    		
			  ClearFlag( EFlagQwertyChrKeyDepressed | EFlagNoActionDuringChrKeyPress);
    		}
    	else
    		{
    		// will launch SCT later
    		iChrLongKeyPress = ETrue;	
    		}	
        
    	}
    	else
#endif   
#endif
	 if (IsFlagSet(EFlagQwertyChrKeyDepressed) && IsFlagSet(EFlagNoActionDuringChrKeyPress)
     && iCurrentFepUI->IsValidShiftKeyPress())
        {
        // Timer cancel
        iChrKeypressMonitor->Cancel();
        // Commit characters
        HandleChangeInFocus();
        // Clear flags
        ClearFlag(EFlagQwertyChrKeyDepressed | EFlagNoActionDuringChrKeyPress);
        // Open editting menu
        LaunchSelectModeMenuL();
        }
    }


// -----------------------------------------------------------------------------
// CAknFepManager::SetCursorSelectionL
// Set editor cursor selection.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepManager::SetCursorSelectionL(const TCursorSelection& /*aCurSel*/, TBool /*aSyncCursor*/)
    {
    }

 TInt CAknFepManager::HandleResourceChangedCallback(TAny* aObj)
	{
	TRAPD(err, static_cast<CAknFepManager*>(aObj)->ResourceChangedTimeroutL());
    if (err)
        {
        static_cast<CAknFepManager*>(aObj)->CleanUpFep();
        return KErrNoMemory;
        }
    return KErrNone;
	}

void CAknFepManager::ResourceChangedTimeroutL()
    {
    if(iCurrentFepUI)
	    {
	    iCurrentFepUI->ResourceChanged(1);	
	    }   
    }


// -----------------------------------------------------------------------------
// pen related inplementation
// -----------------------------------------------------------------------------
#ifdef RD_SCALABLE_UI_V2

// -----------------------------------------------------------------------------
// CAknFepManager::HandleLayoutChange
// Update plugin UI case mode.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepManager::HandleResourceChange(TInt aType)
    {
    if(iCurrentFepUI)
    	{
        iAsyncResouceChanged.CallBack();
    	}
#ifdef RD_INTELLIGENT_TEXT_INPUT
    
    if(aType == KEikDynamicLayoutVariantSwitch)        
        {		
		if ( iAvkonAppUi->IsDisplayingDialog() && iMatchesListLaunched )
			{
			 iUiInterface->DeleteDialogs();
			}
        
        //When there is a change of layout, the inline text position changes and hence the candidate 
        //list position also needs to change accordingly.
        //We need the editor to be brought back to focus, this is because we need the position of
        //the inline text.
        if( iCandidatePopup && iPtiEngine )
        	{
			iCandidatePopup->UnFocus();
			//Need to get the editor state back after it is brought back to focus.
			TRAP_IGNORE( HandleChangeInFocusL()); 
			
			// Add candidate list control stack to get the layout change event
			// after dialog control......
			TRAP_IGNORE( iCandidatePopup->SetFocusAddStackReducePriorityL() );
        	}        
        }
#endif		
    TRAP_IGNORE(SendEventsToPluginManL( EPluginResourceChanged, aType ));
    }   
void CAknFepManager::HandlePointerEventL(const TPointerEvent& aPointerEvent)
    {
    if (IsFlagSet(CAknFepManager::EFlagNoMatches))
        {
        ClearFlag(CAknFepManager::EFlagNoMatches); 
        }
   
    if(aPointerEvent.iType == TPointerEvent::EDrag)
    	{
    	SetExtendedFlag(EExtendedFlagPointerEventTypeEDrag);
    	}
    if (aPointerEvent.iType == TPointerEvent::EButton1Up)
        {
    	ClearExtendedFlag(EExtendedFlagPointerEventTypeEDrag);

        // Remember last up-click in case it is needed for displaying
        // stylus ccpu-menu.
        iClickPoint = aPointerEvent.iPosition;
        }    
    }
 
// -----------------------------------------------------------------------------
// CAknFepManager::HandleSelModeArrowKeyEventL
// Set text selection when tap on the arrow keys.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepManager::HandleSelModeArrowKeyEventL(const TKeyEvent& aKeyEvent, 
                                                  TEventCode aEventCode,
                                                  TKeyResponse& aRetCode)
    {
    aRetCode = EKeyWasNotConsumed;
    TBool used = EFalse;

    // only arrow keys are potentially handled
    if ( ( aKeyEvent.iScanCode == EStdKeyLeftArrow 
            || aKeyEvent.iScanCode == EStdKeyRightArrow )
            && iInputCapabilities.FepAwareTextEditor() )
        {
        TInt inputMode = PluginInputMode();
        TBool isArabicFingerHwr = ( inputMode == EPluginInputModeFingerHwr &&
                GetInputLanguageFromSharedDataInterface() == ELangArabic );
        TCursorSelection currentEditorSelection( 0, 0 );

        iInputCapabilities.FepAwareTextEditor()->
                GetCursorSelectionForFep( currentEditorSelection );
        
        if ( !IsMfneEditor() && ( inputMode == EPluginInputModeItut ||
                inputMode == EPluginInputModeFSQ  || 
                inputMode == EPluginInputModePortraitFSQ  || 
                isArabicFingerHwr )
                && currentEditorSelection.Length() != 0 )
            {
            used = ETrue;

            if ( !( aKeyEvent.iModifiers & EModifierShift ) )
                {
                TKeyEvent ccpuStart = aKeyEvent;
                ccpuStart.iModifiers |= EModifierShift;
                ccpuStart.iModifiers |= EModifierRightShift;
                CCoeEnv::Static()->SimulateKeyEventL( ccpuStart, aEventCode );        
                aRetCode = EKeyWasConsumed;
                }               
            }
        }
    
    return used;
    }

#else // !RD_SCALABLE_UI_V2
// -----------------------------------------------------------------------------
// CAknFepManager::HandleResourceChange
// Update plugin UI case mode.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepManager::HandleResourceChange(TInt aType)
    {
    if (iFepFullyConstructed && Japanese() )
        {
        // Japaneseinput should handle the event of changing layout or screen.
        FepUI()->ResourceChanged(aType);
        }
#ifdef RD_INTELLIGENT_TEXT_INPUT
    if(iCandidatePopup && iPtiEngine && (aType == KEikDynamicLayoutVariantSwitch))        
        {
        //When there is a change of layout, the inline text position changes and hence the candidate 
        //list position also needs to change accordingly.
        //We need the editor to be brought back to focus, this is because we need the position of
        //the inline text.
        iCandidatePopup->UnFocus();
        //Need to get the editor state back after it is brought back to focus.
        TRAP_IGNORE(HandleChangeInFocusL());
        //Can not do anything if FEP is not constructed fully
        if(iFepFullyConstructed)
            {
            //Now we read the position of the inline text
            TPoint  inlineEditorTl;
            TPoint  inlineEditorBr;
            TInt    height;
            TInt    ascent;
            TInt    documentOffset = iPtiEngine->CurrentWord().Length();
            //  Popup Position for RTL
            if(iLanguageCapabilities.iRightToLeftLanguage)
                documentOffset = 0;
            //  Popup Position for RTL
            TRAP_IGNORE( GetScreenCoordinatesL( inlineEditorTl, height, ascent, documentOffset ));
            inlineEditorTl.iY -= height;
            TRAP_IGNORE( GetScreenCoordinatesL( inlineEditorBr, height, ascent ));
            
            //Let the candidate list know about a change of resource and update the
            //inline text rectangle.
            //Now show the candidate list at the re-calculated position
            iCandidatePopup->ShowAtNewPosition(TRect(inlineEditorTl,inlineEditorBr));
            }
        }
#endif		
    }

#endif //RD_SCALABLE_UI_V2    

TDigitType CAknFepManager::LocalDigit()
    {
    return iLanguageCapabilities.iLocalDigitType;
    }
    
/**
* Returns ETrue if arabic indic digit setting is on. 
*
* @since 3.0
*/
TBool CAknFepManager::ArabicIndicDigitsInUse() const
    {
    return (iLanguageCapabilities.iLocalDigitType == EDigitTypeArabicIndic);
    }
       
TBool CAknFepManager::EasternArabicIndicDigitsInUse() const
    {
    return (iLanguageCapabilities.iLocalDigitType == EDigitTypeEasternArabicIndic);
    }
    
/**
 *  Sets delayed commit on (predicitve word will be commited in 
 *  next call to TryCloseUiL. This is needed because of Japanese ReadingTextL).
 *
 * @since 3.1
 */
void CAknFepManager::SetDelayedCommit()
    {
    iCcpuMode |= ECcpuStataCommitPredictiveWord;
    }

// -----------------------------------------------------------------------------
// CAknFepManager::SetDefaultNumberMode
//
// Set default number mode for non-latin languagues
// 
// Copied from CGSLangModel::SetDefaultNumberMode
// -----------------------------------------------------------------------------
//
void CAknFepManager::SetDefaultNumberMode( TInt aMode, TInt aNbrModeType )
    {
    //As aMode comes from the UI, it can either be 0 or 1. This needs
    //special handling for Indic mode as it has been defined as an enum
    //constant and has a value of 2. This applies to any values defined
    //in the enum and have a value > 1.
    if ( aNbrModeType == ENbrModeTypeArabic ||
         aNbrModeType == ENbrModeTypeEasternArabic )
        {
        iSharedDataInterface->SetDefaultNumberMode( KSettingsDefaultNumberMode, aMode );
        }
    else
        {
        iSharedDataInterface->SetDefaultNumberMode( KSettingsIndicDefaultNumberMode, aMode );
        }

    TLocale locale;
    if ( aMode == ENbrModeLatin )
        {
        locale.SetDigitType( EDigitTypeWestern );
        }
    else
        {
        //if aMode != EGSNbrModeLatin, then it should be either latin or arabic. However
        //as EGSNbrModeArabic and EGsNbrModeIndic both have a value = 1, we can't use
        //that constant for below comparison. Hence, need to depend on the 2nd param.
        switch ( aNbrModeType )
            {
            case ENbrModeTypeArabic:
                locale.SetDigitType( EDigitTypeArabicIndic );
                break;
            case ENbrModeTypeIndic:
                locale.SetDigitType( EDigitTypeDevanagari );
                break;
            case ENbrModeTypeEasternArabic:
                locale.SetDigitType( EDigitTypeEasternArabicIndic );
                break;
            default:
                break;
            }
        }

    locale.Set();
    }

// -----------------------------------------------------------------------------
// CAknFepManager::ResolveCurrentCandidateListIndex
//
// Calculates correct selection index for matches popup list.
// -----------------------------------------------------------------------------
//
TInt CAknFepManager::ResolveCurrentCandidateListIndex(CDesCArrayFlat* aCandList)
    {
    if (iPtiEngine && aCandList)
        {           
        TBuf<EMaximumFepWordLength> currentCand;
    
        currentCand.Copy(iPtiEngine->CurrentWord());    
        for (TInt i = 0; i < aCandList->Count(); i++)
            {
            if (currentCand == aCandList->MdcaPoint(i))
                {
                if (i == aCandList->Count() - 1)
                    {
                    return 0;
                    }
                return i + 1;
                }
            }
        }

    return 0;
    }

void CAknFepManager::AlignLogicalAndVisualCursorL( TTmDocPosSpec::TType aType, 
                                                    TBool aToLeft )
    {
    if( !( iInputCapabilities.SupportsSecretText() ) )
        {
        if( IsFlagSet( EFlagInsideInlineEditingTransaction ) && 
            IsFlagSet( EFlagInsideMultitapInlineEditingTransaction ) )
            {
            CommitInlineEditL();
            }

        MCoeFepAwareTextEditor* fepAwareTextEditor = 
            iInputCapabilities.FepAwareTextEditor();

	    MAknFepDocumentNavigation* docNavi = AknFepDocumentNavigation();
        if( (docNavi || TextLayout()) && fepAwareTextEditor )
            {
            TTmDocPosSpec pos = DocPos();
            pos.iType = aType;
            TTmPosInfo2 info;
            if (docNavi)
            	{
	            docNavi->GetNextVisualCursorPos( pos, info, aToLeft );
            	}
            else
            	{
 	            TextLayout()->TagmaTextLayout().GetNextVisualCursorPos( pos, info, aToLeft );
            	}
            TCursorSelection curSel( info.iDocPos.iPos, info.iDocPos.iPos );
            fepAwareTextEditor->SetCursorSelectionForFepL( curSel );
            }
        }
    }

void CAknFepManager::RemoveRephaCharacterL()
    {
    MCoeFepAwareTextEditor* fepAwareTextEditor = 
        iInputCapabilities.FepAwareTextEditor();
    
    if( fepAwareTextEditor )
        {
        TInt previousCharPos = 0;
        TBool leftFlag = GetNextVisualLeftCharacter( previousCharPos );
        
        if( !leftFlag )
            {
            return;
            }
        
        TCursorSelection curSel;
        fepAwareTextEditor->GetCursorSelectionForFep( curSel );
            
        TInt lowerPos = curSel.LowerPos();
        if( lowerPos > 1 )
            {
            const TInt rephaLength( 2 );
            TBuf<rephaLength> previousCharacter;
            previousCharacter.FillZ();
            
            fepAwareTextEditor->
                GetEditorContentForFep( previousCharacter, 
                                        previousCharPos, rephaLength );
            
            if (rephaLength == previousCharacter.Length())
                {
                if( previousCharacter[0] == TAknFepUiIndicInputManager::RaCharacter( 
                        TLanguage( iLanguageCapabilities.iInputLanguageCode ) ) &&
                    previousCharacter[1] == TAknFepUiIndicInputManager::Virama( 
                        TLanguage( iLanguageCapabilities.iInputLanguageCode ) ) )
                    {
                    curSel.iAnchorPos = previousCharPos;
                    curSel.iCursorPos = previousCharPos + rephaLength;
                    
                    // Remove the repha
                    
                    if( IsFlagSet( EFlagInsideInlineEditingTransaction ) && 
                        IsFlagSet( EFlagInsideMultitapInlineEditingTransaction ) )
                        {
                        CommitInlineEditL();
                        }
                    StartInlineEditL( curSel, previousCharacter, rephaLength, ETrue );
                    UpdateInlineEditL( KNullDesC, 0 );
                    CommitInlineEditL();
    
                    // Go to the visual right position of current ligature as the 
                    // repha has been deleted.
                    AlignLogicalAndVisualCursorL( TTmDocPosSpec::ELeading, EFalse );
                    }
                }
            }
        }
    }

TBool CAknFepManager::IsRephaPresent()
    {
    TBool result = EFalse;
    
    MCoeFepAwareTextEditor* fepAwareTextEditor = 
        iInputCapabilities.FepAwareTextEditor();

    if( fepAwareTextEditor )
        {
        TCursorSelection curSel;
        
        fepAwareTextEditor->GetCursorSelectionForFep( curSel );
        
        TInt previousCharPos = 0;
        TBool leftFlag = GetNextVisualLeftCharacter( previousCharPos );
        
        if( !leftFlag )
            {
            return result;
            }

        TInt lowerPos = curSel.LowerPos();
        if( lowerPos > 1 )
            {
            const TInt rephaLength( 2 );
            TBuf<rephaLength> previousCharacter;
            previousCharacter.FillZ();
            
            fepAwareTextEditor->
                GetEditorContentForFep( previousCharacter, 
                                        previousCharPos, rephaLength );

            // Verify whether the length of the string is equal to the length 
            // of repha before doing further checks.
            if( previousCharacter.Length() == rephaLength )
                {               
                if( previousCharacter[0] == TAknFepUiIndicInputManager::RaCharacter( 
                        TLanguage( iLanguageCapabilities.iInputLanguageCode ) ) &&
                    previousCharacter[1] == TAknFepUiIndicInputManager::Virama( 
                        TLanguage( iLanguageCapabilities.iInputLanguageCode ) ) )
                    {
                    result = ETrue;
                    }
                }
            }
        }
        
    return result;
    }
    
TText CAknFepManager::PreviousToPreviousChar( TBool aContextSensitive )
    {
    TBuf<ESingleCharacter> prevToPrevChar( ESingleCharacter );
    prevToPrevChar.FillZ();

    if( !aContextSensitive )
        {
        TTmDocPosSpec pos = DocPos();
        
        // Pos=0 means it is before the first character and there is no previous character
        if (pos.iPos > 0 ) 
            {
            if( iInputCapabilities.FepAwareTextEditor() )
                {
                iInputCapabilities.FepAwareTextEditor()->
                    GetEditorContentForFep( prevToPrevChar, pos.iPos - 1, ESingleCharacter );
                }
            }
        }
    else
        {
        TCursorSelection curSel;
        
        MCoeFepAwareTextEditor* fepAwareTextEditor = 
            iInputCapabilities.FepAwareTextEditor();
        
        if( fepAwareTextEditor )
            {
            fepAwareTextEditor->GetCursorSelectionForFep( curSel );
            TInt lowerpos = curSel.LowerPos();

            if( lowerpos > 1 )
                {
                fepAwareTextEditor->
                    GetEditorContentForFep( prevToPrevChar, lowerpos - 2, ESingleCharacter );
                }
            }
        }
    if (prevToPrevChar.Length())
        return prevToPrevChar[0];
    else
        return NULL;
    }

void CAknFepManager::RemoveRakarCharacterL()
    {
    MCoeFepAwareTextEditor* fepAwareTextEditor = 
        iInputCapabilities.FepAwareTextEditor();
    
    if( fepAwareTextEditor )
        {
        TCursorSelection curSel;
            
        fepAwareTextEditor->GetCursorSelectionForFep( curSel );
        
        TInt lowerpos = curSel.LowerPos();

        if( lowerpos > 1 )
            {
            const TInt rakarLength( 2 );
            TBuf<rakarLength> buffer;
            buffer.FillZ();

            fepAwareTextEditor->
                GetEditorContentForFep( buffer, lowerpos - rakarLength, rakarLength );
            
            if (rakarLength == buffer.Length())
                {
                if( buffer[0] == TAknFepUiIndicInputManager::Virama( 
                        TLanguage( iLanguageCapabilities.iInputLanguageCode ) ) &&
                    buffer[1] == TAknFepUiIndicInputManager::RaCharacter( 
                        TLanguage( iLanguageCapabilities.iInputLanguageCode ) ) )
                    {
                    curSel.iAnchorPos = lowerpos - rakarLength;
                    curSel.iCursorPos = lowerpos;
                    
                    // Remove the repha
                    
                    if( IsFlagSet( EFlagInsideInlineEditingTransaction ) && 
                        IsFlagSet( EFlagInsideMultitapInlineEditingTransaction ) )
                        {
                        CommitInlineEditL();
                        }
                    StartInlineEditL( curSel, buffer, rakarLength, ETrue );
                    UpdateInlineEditL( KNullDesC, 0 );
                    CommitInlineEditL();
                    }
                }
            }
        }
    }

/**
 * Returns the status of predictive input.
 *
 *  @since 3.2
 *  @return ETrue is available predictive input.
 */
TBool CAknFepManager::IsAbleToChangePrediction() const
    {
    TBool ret = EFalse;
    if (!(iAknEditorFlags & EAknEditorFlagNoT9))
        {
        if (iMode == EHiraganaKanji)
            {
            if (HasJapanesePredictionInputMode())
                {
                ret = ETrue;
                }
            }
        else
            {
            ret = ETrue;
            }
        }
    return ret;
    }

TLanguage CAknFepManager :: GetInputLanguageFromSharedDataInterface()
{
    return ((TLanguage)(iSharedDataInterface->InputTextLanguage()));
}
    
    
void CAknFepManager::DeactivatePredicitveWordAndMoveCursorL()
    {
    if (WesternPredictive())
        {           
        TWesternNaviEvent naviEvent = ERightNaviEvent;
    
        if (iLanguageCapabilities.iRightToLeftLanguage)
            {
            naviEvent = ELeftNaviEvent;     
            }
#ifdef RD_INTELLIGENT_TEXT_INPUT         
		// This function is called at time of Cut/Copy command
		// TryCloseUiL will remove the auto completion part
		// To adjust the visual cursor position commit the text with
		// auto completion part.  
		if( IsAutoCompleteOn()) 
			{
			TInt tailLength = 0;
			iPtiEngine->HandleCommandL( EPtiCommandGetAutoCompletionTailLength, &tailLength );

			if( tailLength > 0)
				{
				// Sync the key sequence and editor text.	
				iPtiEngine->HandleCommandL( EPtiCommandLockCurrentCandidate );			
				}
			}
#endif
    
        // Find the visual left/right end of the current word
        TTmDocPosSpec pos;
        GetAvkonDefinedVisualLimitsOfSelection(
               iUncommittedText, naviEvent, pos, NULL );
               
        TryCloseUiL();
    
        // Position should be dis-ambiguated based on last navigation and direction of blocks.
        AdjustPosSpecForAmbiguityAfterLeftOrRightNavigation( pos, naviEvent );

        SetCursorPositionL( pos );

        iCaseMan->UpdateCase(ENullNaviEvent);       
        }
    }


// -----------------------------------------------------------------------------
// CAknFepManager::EditorCcpuStatus
//
// Returns the Ccpu status of given editor state as a flag combination.
// -----------------------------------------------------------------------------
//
TInt CAknFepManager::EditorCcpuStatus(CAknEdwinState* aEdwinState)
    {
    TInt ret = 0;
                                            
    if (aEdwinState && aEdwinState->CcpuState())
        {
        if (aEdwinState->CcpuState()->CcpuCanCopy())
            {
            ret |=  ECcpuStatusFlagCanCopy;
            }
        if (aEdwinState->CcpuState()->CcpuCanCut()) 
            {
            ret |=  ECcpuStatusFlagCanCut;
            }
        if (aEdwinState->CcpuState()->CcpuCanPaste())               
            {
            ret |=  ECcpuStatusFlagCanPaste;                    
            }
        }                 
    return ret; 
    }
    
void CAknFepManager::NewLigatureL( const TDesC& aText )
    {
    if( !iInputCapabilities.SupportsSecretText() && 
        ( EditorHasFreeSpace() || 
            IsFlagSet( EFlagInsideMultitapInlineEditingTransaction ) ) )
        {
        if ( iFepManState == EAknFepStateInitial ) //multitapping
            {
            if ( !IsFlagSet( EFlagInsideMultitapInlineEditingTransaction ) )
                {
                ClearFlag( EFlagLineFeedCharacter );
                StartInlineEditL( aText );
                SetFlag( EFlagInsideMultitapInlineEditingTransaction );
                SetFlag( EFlagInsideInlineEditingTransaction );
                }
            }
        }
    }



// -----------------------------------------------------------------------------
// CAknFepManager::CancelCcpuMode
//
// Cancels the copy / cut mode initiated from editing options -submenu.
// -----------------------------------------------------------------------------
//
void CAknFepManager::CancelCcpuMode()
    {
    iIndicator->SetCopyMode(EFalse);
    if (IsCcpuFlagSet(ECcpuStateStartCopy | ECcpuStateCopy | ECcpuStateStartCut | ECcpuStateCut))
        {
        iUiInterface->DeleteSoftkeys();    
        }
    
    ResetCcpuFlags();                       
    }

// -----------------------------------------------------------------------------
// CAknFepManager::CancelShiftKeyTimer
//
// Cancels shift key timer.
// -----------------------------------------------------------------------------
//
void CAknFepManager::CancelShiftKeyTimer()
    {
    ClearFlag(EFlagShiftKeyDepressed);
    if (iShiftKeypressMonitor)
        {
        iShiftKeypressMonitor->Cancel();    
        }
    }

void CAknFepManager::RemoveTextFromEditorL( TInt aNumOfCharacters, 
                                            TInt aPos /*= 0*/,
                                            TBool aIsToAlignCursor /* = ETrue */)
	{
    if( IsFlagSet( EFlagInsideInlineEditingTransaction ) && 
        IsFlagSet( EFlagInsideMultitapInlineEditingTransaction ) )
        {
        // Update the inline buffer with NULL descriptor.
        // Inline buffer can be only at position 0, i.e. 
        // where the cursor stands.
        if( aNumOfCharacters == UncommittedText().Length()
             && aPos == 0 )
            {
            UpdateInlineEditL( KNullDesC, 0 );
            }
        CommitInlineEditL();
        }
    else
        {
        MCoeFepAwareTextEditor* fepAwareTextEditor = 
            iInputCapabilities.FepAwareTextEditor();

        if( fepAwareTextEditor && aPos >= 0 )
            {
            // Get the previous cursor position.
            TCursorSelection cursorSelPrevious;
            fepAwareTextEditor->GetCursorSelectionForFep( cursorSelPrevious );

            // Place the cursor at the desired position.
            TCursorSelection sel( cursorSelPrevious.iCursorPos - aPos, 
                            cursorSelPrevious.iCursorPos - aPos );
        	fepAwareTextEditor->SetCursorSelectionForFepL( sel );

            // Set cursor span on previous N characters
            if ( sel.iAnchorPos >= aNumOfCharacters )
                {
                sel.iAnchorPos -= aNumOfCharacters;

                // get text to clear
                TBuf<EMaximumFepWordLength> bufToClear;
                fepAwareTextEditor->GetEditorContentForFep( bufToClear, sel.iAnchorPos, 
                                                            aNumOfCharacters );

                // delete the character with an inline edit
                StartInlineEditL( sel, bufToClear, aNumOfCharacters, EFalse );
                UpdateInlineEditL( KNullDesC, 0 );
                CommitInlineEditL();
                
                if( aIsToAlignCursor )
                    {
                    // Move the cursor to the original position considering the 
                    // aNumOfCharacters
                    cursorSelPrevious.iCursorPos -= aNumOfCharacters;
                    cursorSelPrevious.iAnchorPos = cursorSelPrevious.iCursorPos;
                    fepAwareTextEditor->SetCursorSelectionForFepL( cursorSelPrevious );
                    }
                }        
            }        
        }
    }

#ifdef RD_MARATHI

TBool CAknFepManager::IsChandraAPresentL()
    {
    TBool result = EFalse;

    MCoeFepAwareTextEditor* fepAwareTextEditor = 
		iInputCapabilities.FepAwareTextEditor();

	if( fepAwareTextEditor )
		{
		TCursorSelection curSel;
		
		fepAwareTextEditor->GetCursorSelectionForFep( curSel );
		
		TInt lowerpos = curSel.LowerPos();

        TInt chandraALength = 2;
        TBuf<2> buf;
	    TAknFepUiIndicInputManager::GetChandraA( buf );

		if( lowerpos > ( chandraALength - 1 ) )
			{
			TBuf<2>previousCharacter;

			fepAwareTextEditor->GetEditorContentForFep( 
                previousCharacter, lowerpos-chandraALength, chandraALength );
			
			if( buf.CompareC( previousCharacter ) == 0 )
			    {
			    // Chandra-A found
			    result = ETrue;
			    }
			}
		}
    return result;
	}

TBool CAknFepManager::IsEyeLashRaPresentL()
    {
    TBool result = EFalse;

    MCoeFepAwareTextEditor* fepAwareTextEditor = 
		iInputCapabilities.FepAwareTextEditor();

	if( fepAwareTextEditor )
		{
		TCursorSelection curSel;
		
		fepAwareTextEditor->GetCursorSelectionForFep( curSel );
		
		TInt lowerpos = curSel.LowerPos();

        
        TBuf<3> buf;
	    TAknFepUiIndicInputManager::GetEyeLashRa( buf, 
	                                    (TLanguage)iLanguageCapabilities.iInputLanguageCode );
        TInt eyeLashRaLength = buf.Length();

		if( ( lowerpos - eyeLashRaLength ) >= 0 )
			{
			TBuf<3>previousCharacter;

			fepAwareTextEditor->GetEditorContentForFep( 
                previousCharacter, lowerpos-eyeLashRaLength, eyeLashRaLength );
			
			if( previousCharacter.Length() >=2 && 
			    buf.CompareC( previousCharacter ) == 0 )
			    {
			    // EyeLashRa found
			    result = ETrue;
			    }
			}
		}
    return result;
	}

#endif // RD_MARATHI
#ifndef RD_INTELLIGENT_TEXT_INPUT
void CAknFepManager::RemoveSuggestedCompletionL()
{	
	if(iPtiEngine->InputMode() == EPtiEngineWordCompletion && 
	   IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction))
		{
			TBuf<CAknFepManager::EMaximumFepWordLength> currentText = iPtiEngine->CurrentWord();
			if(currentText.Length() > iPtiEngine->CurrentInputSequence().Length())
				{
				currentText.SetLength(iPtiEngine->CurrentInputSequence().Length());
				iPtiEngine->SetCurrentWord(currentText);
	    		UpdateInlineEditL(currentText,currentText.Length()); 
   	            TransferFepStateToEditorL();
				}
			
		}
}
#endif //RD_INTELLIGENT_TEXT_INPUT


// -----------------------------------------------------------------------------
// CAknFepManager::ExtendedInputCapabilities
//
// Returns the extended input bapabilities bitvector.
// -----------------------------------------------------------------------------
//
TUint CAknFepManager::ExtendedInputCapabilities() const
    {
    MObjectProvider* mop = iInputCapabilities.ObjectProvider();

    if ( mop && iInputCapabilities.FepAwareTextEditor() )//IsFepAwareTextEditor()
		{
		CAknExtendedInputCapabilities* extendedInputCapabilities = 
		mop->MopGetObject( extendedInputCapabilities );

		if ( extendedInputCapabilities ) 
			{
			return extendedInputCapabilities->Capabilities();
			}
		} 
		
    return 0;		          
    }


#ifdef RD_SCALABLE_UI_V2         
// -----------------------------------------------------------------------------
// CAknFepManager::LaunchStylusCcpuMenuL
//
// Displays stylus ccpu-menu.
// -----------------------------------------------------------------------------
//    
void CAknFepManager::LaunchStylusCcpuMenuL(TPoint &aClickPoint)
    {
    const TInt KItemCopy  = 0x01;
    const TInt KItemCut   = 0x02;
    const TInt KItemPaste = 0x04;
    
    delete iStylusCcpuMenu;
    iStylusCcpuMenu = NULL;    
     
    // It is assume that this method is called only if there is selected
    // text in suitable editor, ie. there will be at least "copy"-item available.
    TInt items = KItemCopy;    
        
    CAknEdwinState* editorState = EditorState();
    if (editorState && editorState->CcpuState())
        {              
        if (editorState->CcpuState()->CcpuCanCut())
            {
            items |= KItemCut;
            }
        if (editorState->CcpuState()->CcpuCanPaste())
            {
            items |= KItemPaste;
            }                    
        }        
    
    if (items)
        {
        TBuf<CAknFepUiInterfaceMenuPane::SItemData::ENominalTextLength> itemStr;          
        iStylusCcpuMenu = iUiInterface->NewStylusPopUpMenuL(this, aClickPoint);     
        
        if (items & KItemCopy)
            {
            StringLoader::Load(itemStr, R_AKNFEP_SOFTKEY_COPY);            
            iStylusCcpuMenu->AddMenuItemL(itemStr.Mid(1, itemStr.Length() - 1).Left(CAknFepUiInterfaceMenuPane::SItemData::ENominalTextLength), EEikCmdEditCopy);            
            }
        if (items & KItemCut)
            {                    
            itemStr.Zero();
            StringLoader::Load(itemStr, R_AKNFEP_SOFTKEY_CUT);            
            iStylusCcpuMenu->AddMenuItemL(itemStr.Mid(1, itemStr.Length() - 1).Left(CAknFepUiInterfaceMenuPane::SItemData::ENominalTextLength), EEikCmdEditCut);
            }
        if (items & KItemPaste)
            {
            itemStr.Zero();
            StringLoader::Load(itemStr, R_AKNFEP_STYLUS_CCPU_MENU_PASTE);               
            iStylusCcpuMenu->AddMenuItemL(itemStr.Mid(1, itemStr.Length() - 1).Left(CAknFepUiInterfaceMenuPane::SItemData::ENominalTextLength), EEikCmdEditPaste);                
            }
        
        SetCcpuFlag(ECcpuStateCopy); // This forces ProcessCommandL to handle the events.
        iStylusCcpuMenu->SetPosition(aClickPoint);    
        iStylusCcpuMenu->ShowMenu();       
        }
    }
#endif    

#ifdef RD_INTELLIGENT_TEXT_INPUT
 TBool CAknFepManager::IsFnKeyMappedL(TPtiKeyboardType aKeyboardType)
    {
    MPtiLanguage *lang = NULL;
    RArray < TPtiNumericKeyBinding > keybinding;
    TBool ret = EFalse;
    if(iPtiEngine)
        {
        lang = iPtiEngine->CurrentLanguage();
        if(lang)
        {
        iPtiEngine->GetNumericModeKeysForQwertyL(lang->LanguageCode(), keybinding, aKeyboardType);
        TInt numericKeysCount = keybinding.Count();
        while ( numericKeysCount-- )
            {
            TPtiNumericKeyBinding numKeyBind = keybinding[numericKeysCount];
            if ( numKeyBind.iCase == EPtiCaseFnLower || numKeyBind.iCase == EPtiCaseFnUpper)
                {
                ret = ETrue;
                break;
                    }
                }
            }
        }
    keybinding.Reset();
    return ret;
    }
TBool CAknFepManager::IsFnKeyMapped()
    {
    return iIsFnKeyMapped;
    }
    
void CAknFepManager::SetFnKeyMappingState()
    {
    TRAP_IGNORE(iIsFnKeyMapped = IsFnKeyMappedL());
    }

TBool CAknFepManager::KeyMapsDifferentCharacterWithFn( TPtiKey aKey ) const
    {
    // we are interested only about the first mapped character => buffer length 1 is adequate
    TBuf<1> lowerMapping; 
    TBuf<1> upperMapping;
    TBuf<1> lowerFnMapping; 
    TBuf<1> upperFnMapping;
    iPtiEngine->MappingDataForKey( aKey, lowerMapping, EPtiCaseLower );
    iPtiEngine->MappingDataForKey( aKey, upperMapping, EPtiCaseUpper );
    iPtiEngine->MappingDataForKey( aKey, lowerFnMapping, EPtiCaseFnLower );
    iPtiEngine->MappingDataForKey( aKey, upperFnMapping, EPtiCaseFnUpper );
    
    if ( lowerFnMapping.Length() )
        {
        if ( !lowerMapping.Length() ||              // < key maps character only with Fn
             lowerMapping[0] != lowerFnMapping[0] ) // < Fn changes the character of the key
            {
            return ETrue;
            }
        }
    else if ( upperFnMapping.Length() )
        {
        if ( !upperMapping.Length() ||              // < key maps character only with Fn
             upperMapping[0] != upperFnMapping[0] ) // < Fn changes the character of the key
            {
            return ETrue;
            }
        }
    
    return EFalse; // The key doesn't have symbol behind Fn or the symbol is the same as without Fn
    }
#endif //RD_INTELLIGENT_TEXT_INPUT
MAknFepDocumentNavigation* CAknFepManager::AknFepDocumentNavigation() const
	{
	MAknFepDocumentNavigation* navi = NULL;
	
    MObjectProvider* mop = iInputCapabilities.ObjectProvider();
    if ( mop )
        {
        mop->MopGetObject( navi );
        }
    
    return navi;
	}

CAknFepUIInterface* CAknFepManager::UiInterface()
	{
	return iUiInterface;
	}

void CAknFepManager::AlignLogicalAndVisualCursorL()
    {
    TBool isIndicLang = 
        TAknFepUiIndicInputManager::
            IsIndicLangauge( TLanguage( iLanguageCapabilities.iInputLanguageCode ) );

    if( isIndicLang )
        {
        TTmDocPosSpec pos = DocPos();
        pos.iType = TTmDocPosSpec::ETrailing;
        TTmPosInfo2 info;
        TTmLineInfo lineInfo;

        TBool findAvailable = EFalse;

        MAknFepDocumentNavigation* docNavi = AknFepDocumentNavigation();

        if ( docNavi )
            {
            docNavi->FindDocPos( pos, info, lineInfo );
            findAvailable = ETrue;
            }
        else
            {
            CTextLayout* textLayout = TextLayout();
            if ( textLayout )
                {
                textLayout->TagmaTextLayout().FindDocPos( pos, info, lineInfo );	    					    
                findAvailable = ETrue;
                }
            }

        if ( findAvailable )
            {
            TCursorSelection sel( info.iDocPos.iPos, info.iDocPos.iPos );
            iInputCapabilities.FepAwareTextEditor()->SetCursorSelectionForFepL(sel);
            }
        }
    }
#ifdef RD_INTELLIGENT_TEXT_INPUT
TKeyResponse CAknFepManager::HandleFnKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aEventCode )
    {
     TKeyResponse ret; 
     
#ifdef __HALF_QWERTY_KEYPAD  
     if ( IsFlagSet( EFlagShiftKeyDepressed ) && 
        ( EPtiKeyboardHalfQwerty == KeyboardLayout() )&& 
        ( aEventCode != EEventKeyUp ) )
         {
         return EKeyWasNotConsumed;
         }
#endif
     
     if (!iCurrentFepUI->IsValidFnKeyPress() && EStdKeyRightFunc == aKeyEvent.iScanCode)
     	{
     	TBool Uiret = ETrue;
     	if (EEventKeyDown == aEventCode )
     		{
      		Uiret = FepUI()->HandleKeyL(EKeyRightFunc, EShortKeyPress);     		
     		}
     		
     	if (Uiret)
     		{
     		ret = EKeyWasConsumed;
     		}
        else
        	{
     	    ret = iFnKeyManager->HandleFnKeyEventL( aKeyEvent, aEventCode, 
                                            	    iInputCapabilities ) ;     	        	
        	}	 
     	}
     else
     	{
		//Handle Fn key and consider long press case
		if( EStdKeyRightFunc == aKeyEvent.iScanCode )
			{
			switch(aEventCode)
				{
				case EEventKeyDown:
					{
					  //if current state of fn key is EFnKeyNone or EFnKeyNext..., EFnKeyLock will be set;
					  //otherwise, EFnKeyNone will be set.
				      if ( iFnKeyManager->FnKeyState() ==  CAknFepFnKeyManager::EFnKeyLock )
				    	  {
			                ClearExtendedFlag( EExtendedFlagFnKeyNeedLockMode );
				    	  }
				      else
				    	  {
			                SetExtendedFlag( EExtendedFlagFnKeyNeedLockMode );
				    	  }
					  ActivateFnkeyPressMonitor();
					}
					break;
				case EEventKeyUp:
					{
					if(!iFnKeypressMonitor->IsActive())
					   {
					    //when long pressing event occurrs,
                        //event has been already handled in HandleFnKeyPressMonitorCallback()
						if ( !IsExtendedFlagSet ( EExtendedFlagFnKeyNeedLockMode ))
							{
							 iFnKeyManager->SetFnKeyState(CAknFepFnKeyManager::EFnKeyNone);
							}
						return EKeyWasConsumed;
					   }
					//when short pressing event occurs, 
                    //only deactive fn Key press monitor and normal event handle flow will work
					DeactivateFnkeyPressMonitor();
					}
					break;
				default:
					break;	
				}			  
			}
     	ret = iFnKeyManager->HandleFnKeyEventL( aKeyEvent, aEventCode, 
                                            	iInputCapabilities ) ;     	
     	}
     if(IsHybridAplhaEditor() && !iHybridAplphaChangedToAlphanumeric)
         {
         if(FnKeyState() != CAknFepFnKeyManager::EFnKeyLock)
            iHybridAplphaChangedToAlphanumeric = ETrue;
         }

     return ret;
    }
#endif //RD_INTELLIGENT_TEXT_INPUT

// -----------------------------------------------------------------------------
// CAknFepManager::HasJapanesePredictionInputMode
// Has Japanese prediction input mode in PtiEngine
// -----------------------------------------------------------------------------
//
TBool CAknFepManager::HasJapanesePredictionInputMode() const
    {
    // Is it supported Japanese predictive input
    TBool ret = EFalse;
    MPtiLanguage* ptilang = iPtiEngine->GetLanguage(ELangJapanese);
    if ( ptilang
     &&  ( (!iQwertyInputMode && ptilang->HasInputMode(EPtiEngineHiraganaKanjiPredictive))
         ||(iQwertyInputMode && ptilang->HasInputMode(EPtiEngineHiraganaKanjiPredictiveQwerty)) ) )
        {
        ret = ETrue;
        }
    return ret;
    }
#ifdef RD_HINDI_PHONETIC_INPUT
void CAknFepManager::HandleIndicCaseL()
	{
		iHashKeyMan->HandleSetIndicStateL();
	}
#endif

// -----------------------------------------------------------------------------
// CAknFepManager::IsCursorAtEndOfWord
// 
// -----------------------------------------------------------------------------
//    
TBool CAknFepManager::IsCursorAtEndOfWord()
    {   
    iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(iUncommittedText);
    TInt origPos = iUncommittedText.iCursorPos; 
   
    FindEndOfWord(iUncommittedText.iCursorPos);
       
    if (origPos != iUncommittedText.iCursorPos)
        {
        return EFalse;
        }
        
    return ETrue;        
    }

#ifdef RD_INTELLIGENT_TEXT_INPUT
TInt CAknFepManager::GetCurrentCase()
    {
    TCase returnCase;
    
    switch( iCaseMan->CurrentCase() )
        {
        case EAknEditorUpperCase:
        case EAknEditorTextCase:
            {
            returnCase = EUpperCase;
            }
            break;

        case EAknEditorLowerCase:
            {
            returnCase = ELowerCase;
            }
            break;
        
        default:
            returnCase = ELowerCase;
            break;
        }
    return returnCase;
    }
// Predictive QWERTY changes (XT9) ---->

void CAknFepManager::GetCandidatesL( CDesCArray& aArray, TInt& aActiveIdx )
    {
    iPtiEngine->GetCandidateListL(aArray);
    iPtiEngine->HandleCommandL(
            EPtiCommandUserActionGetCurrentIndexOfCandidates, &aActiveIdx);
    }

void CAknFepManager::GetUpdateCandidatePositionL(TRect& aRect)
    {
     if(iCandidatePopup && iFepFullyConstructed && iPtiEngine)
        {
        //Now we read the position of the inline text
        TPoint  inlineEditorTl;
        TPoint  inlineEditorBr;
        TInt    height;
        TInt    ascent;
        TInt    documentOffset = iPtiEngine->CurrentWord().Length();
        //  Popup Position for RTL
        if(iLanguageCapabilities.iRightToLeftLanguage)
            documentOffset = 0;
        //  Popup Position for RTL
        GetScreenCoordinatesL(inlineEditorTl, height, ascent, documentOffset);
        inlineEditorTl.iY -= height;
        GetScreenCoordinatesL(inlineEditorBr, height, ascent);
        aRect.iTl = inlineEditorTl;
        aRect.iBr = inlineEditorBr;
        }
    }

TBool CAknFepManager::IsRightToLeftLanguage()
	{
	return iLanguageCapabilities.iRightToLeftLanguage ;
	}

/**
* Setter for the advanced predictive typing correction.
* @param    aLevel   The level to be set.
*/
void CAknFepManager::SetTypingCorrectionLevel(TInt aLevel)
	{
	iTypingCorrectionLevel = aLevel;
    
	if(iFepFullyConstructed)
		{
		TPtiErrorCorrectionLevel correctionLevel = (TPtiErrorCorrectionLevel)aLevel;
		TRAP_IGNORE(iPtiEngine->HandleCommandL( EPtiCommandSetCorrectionLevel, &correctionLevel ));	
		}
	}
	
/**
* Resetter for the advanced predictive Number Candidate setting.
* @param    aValue   The value to be set.
*/
void CAknFepManager::SetNumberCandidateState(TInt aValue)
	{
	if(aValue == 0)
		iIsNumberCandidateShown = EFalse;
	else if(aValue == 1)
		iIsNumberCandidateShown = ETrue;
	
	if(iFepFullyConstructed)
		{
		TRAP_IGNORE(iPtiEngine->HandleCommandL( EPtiCommandSetNumberCandidateStatus, 
		                                        &iIsNumberCandidateShown ));
		}
	}
	
void CAknFepManager::RemoveSuggestedAdvanceCompletionL()
    {	
    if( IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction) &&
        iPtiEngine->CurrentWord().Length() )
        {
        TRAP_IGNORE(iPtiEngine->HandleCommandL( EPtiCommandUserActionShorten ));
        TPtrC currentText = iPtiEngine->CurrentWord();
        UpdateInlineEditL( currentText, currentText.Length() );
        }
    }
/**
* Setes the current state from GS to the local state
*/    
   
void CAknFepManager::SetAutoCompletionState(TInt aValue) 
	{
	if(aValue == 0)
		{
		iIsAutoCompleteOn = EFalse;
		}
	else if(aValue == 1)
		{
		iIsAutoCompleteOn = ETrue;
		}
	 
	if(iFepFullyConstructed)
		{
		if(iIsAutoCompleteOn)
			{
			TRAP_IGNORE(iPtiEngine->HandleCommandL( EPtiCommandEnableAutoCompletion  ));
			}
		else
			{
			TInt tailLength = 0;
			TRAP_IGNORE(iPtiEngine->HandleCommandL( EPtiCommandGetAutoCompletionTailLength, &tailLength ));
			if(tailLength > 0)
				{
				// send an update to engine to signify change in autocompletion from on to off while tail exists
				// This will set EPtiXt9FlagAutoCompletionSetOnToOff flag as true in engine
				TInt value = 1;
				TRAP_IGNORE(iPtiEngine->HandleCommandL( EPtiCommandAutoCompletionOnToOff,&value));				
				}
			
			
			TRAP_IGNORE(iPtiEngine->HandleCommandL( EPtiCommandDisableAutoCompletion ));
			}
		}
	}
	
/**
* Sets the primary candidate from GS to the local state
*/    
   
void CAknFepManager::SetPrimaryCandidate(TInt aValue) 
	{
	if(aValue == 0)
		iPrimaryCandidate = ETrue;
	else if(aValue == 1)
		iPrimaryCandidate = EFalse;    
	if(iFepFullyConstructed)
		{
		TRAP_IGNORE(iPtiEngine->HandleCommandL( EPtiCommandSetProactiveCorrection, 
		                                        &iPrimaryCandidate ));
		}
	}
#endif //RD_INTELLIGENT_TEXT_INPUT
    
CAknFepPluginManager* CAknFepManager::PluginUIManager()
    {
#ifdef RD_SCALABLE_UI_V2

    return iFepPluginManager;
#else
    return NULL;        
#endif //RD_SCALABLE_UI_V2     
    }

// add for phrase creation
MZhuyinKeyHandler* CAknFepManager::ZhuyinKeyHandler()
    {
    return iZhuyinKeyHandler;
    }
// add for phrase creation
CAknFepZhuyinAnalyser* CAknFepManager::ZhuyinAnalyser()
    {
    return iZhuyinAnalyser;
    }

#ifdef RD_SCALABLE_UI_V2
void CAknFepManager::UpdateTouchCaseMode()
	{
    if (iFepFullyConstructed && 
        iFepPluginManager 
        // case should be updated even if current input mode is none because iPreviousCoseMode
        // still need to be updated.
        /*&& 
        iFepPluginManager->PluginInputMode() != EPluginInputModeNone*/)
    	{
    	iFepPluginManager->UpdateCaseMode();	
    	}
	}
#endif// RD_SCALABLE_UI_V2

void CAknFepManager::SetStopProcessFocus(TBool aStop, TBool aClose)
    {
    iClosePeninputUi = aClose;
    iStopProcessFocus = aStop;
    if (!aStop)
        {
        HandleChangeInFocus();
        }
    }
    
TBool CAknFepManager::StopProcessFocus()
    {
    return iStopProcessFocus;    
    }
    
TBool CAknFepManager::CloseUiOnFocusChange()
    {
    return iClosePeninputUi;    
    }
void CAknFepManager::HandleCopyCutStateL()    
{
    if (IsCcpuFlagSet(ECcpuStateCopy | ECcpuStateCut))
                {
                TCursorSelection selection;
                iInputCapabilities.FepAwareTextEditor()->GetCursorSelectionForFep(selection);
                TBool update;
                if (selection.Length() > 0)
                    {
                    if (!IsCcpuFlagSet(ECcpuStateButton))
                        {
                        __ASSERT_DEBUG(iUiInterface->SoftkeysExist(),
                                       AknFepPanic(EAknFepPanicNoHandleToFepAwareTextEditor));                         
                        if (IsCcpuFlagSet(ECcpuStateCopy))                      
                            {
                            update = UpdateCBALabelL(ELeftSoftkeyIndex, EEikCmdEditCopy, R_AKNFEP_SOFTKEY_COPY);
                            update |= UpdateCBALabelL( EMiddleSoftkeyIndex, EEikCmdEditCopy, R_AVKON_SOFTKEY_SELECT_TEXT ); 
                            }
                        else
                            {
                            update = UpdateCBALabelL(ELeftSoftkeyIndex, EEikCmdEditCut, R_AKNFEP_SOFTKEY_CUT);                          
                            update |= UpdateCBALabelL( EMiddleSoftkeyIndex, EEikCmdEditCut, R_AVKON_SOFTKEY_SELECT_TEXT ); 
                            }
                        if (update)
                            {
                            iUiInterface->DrawSoftkeysNow();
                            }               
                        SetCcpuFlag(ECcpuStateButton);                                      
                        }                   
                    }   
                else if (IsCcpuFlagSet(ECcpuStateButton))                        
                    {                   
                    ClearCcpuFlag(ECcpuStateButton);
                    update = EFalse;
                    update = UpdateCBALabelL(ELeftSoftkeyIndex, KNullCommandId, R_TEXT_SOFTKEY_EMPTY);
                    update |= UpdateCBALabelL( EMiddleSoftkeyIndex, KNullCommandId, R_TEXT_SOFTKEY_EMPTY );
                    if (update)
                        {
                        iUiInterface->DrawSoftkeysNow();
                        }               
                    }
                }    
}    
#ifdef RD_SCALABLE_UI_V2
TInt GetForegroundTaskAppWgId()
    {
    TApaTaskList tList(CCoeEnv::Static()->WsSession());
    
    TApaTask task = tList.FindByPos(0);
    return task.WgId();
    }

void CAknFepManager::DimInputmodeTouchMenuItems(CAknFepUiInterfaceMenuPane* aMenuPane)
    {
    DimEditMenuModeItems(aMenuPane);

    if ( IsChineseInputLanguage() )
        {
        DoChineseTouchMenu(aMenuPane);
        }
    else if ( iLanguageCapabilities.iInputLanguageCode == ELangJapanese)
        {
        DoJapaneseMenu(aMenuPane);
        }
    else
        {
        DoWesternTouchMenu(aMenuPane);
        }
    }
#endif

void CAknFepManager::DimInputmodeMenuItems(CAknFepUiInterfaceMenuPane* aMenuPane)
    {
    DimEditMenuModeItems(aMenuPane);

    if ( IsChineseInputLanguage() )
        {
        DoChineseMenu(aMenuPane);
        }
    else if ( iLanguageCapabilities.iInputLanguageCode == ELangJapanese)
        {
        DoJapaneseMenu(aMenuPane);
        }
    else
        {
        DoWesternMenu(aMenuPane);
        }
    }

void CAknFepManager::HandleCopyCutEventL(TInt aCommandId)
    {
     if (iCcpuMode != ECcpuStateNone)
        {                   
        ResetCcpuFlags();                   
        iUiInterface->DeleteSoftkeys();
                    
        SetCcpuFlag(ECcpuStatePosted);  
                                         
#ifdef RD_SCALABLE_UI_V2                    
        if (aCommandId == EEikCmdEditCopy)
            {                     
            // Send copy event to AknCcpuSupport.
            CCoeEnv::Static()->SimulateKeyEventL(KAknCcpuCopyEvent, EEventKey);
            }
        else
            {
            // Send cut event to AknCcpuSupport.
            CCoeEnv::Static()->SimulateKeyEventL(KAknCcpuCutEvent, EEventKey);
            }                               
#else         
/*As part of build warnings commenting this perticular code it might require when touch is enabled.        
        if (aCommandId == EEikCmdEditCopy)
            {                                          
            // Send copy-event to editor.
            CCoeEnv::Static()->SimulateKeyEventL(KEditorCopyEvent, EEventKey);
            }
        else 
            {                       
            // send cut-event to editor.         
            CCoeEnv::Static()->SimulateKeyEventL(KEditorCutEvent, EEventKey);
            } 
			*/            
#endif // RD_SCALABLE_UI_V2 

        if (iInputCapabilities.FepAwareTextEditor())                                
            {
            // update indicators only if this is editable component.
            iIndicator->SetCopyMode(EFalse);
            UpdateIndicators();                                                           
            }
#ifdef  RD_SCALABLE_UI_V2       	 
        HandleChangeInFocus();
#endif
        
        }

    }

TInt CAknFepManager::FepShowVkbPreviewStatus()
    {
    return iSharedDataInterface->FepShowVkbPreviewStatus();    
    }

void CAknFepManager::InitPreviewMenuPane(CAknFepUiInterfaceMenuPane* aMenuPane)
    {
    if (FepShowVkbPreviewStatus() > 0)
        {
        aMenuPane->GetMenuPane()->SetItemButtonState(EPenInputCmdPreviewOn, 
                                                     EEikMenuItemSymbolOn );
        }
    else
        {
        aMenuPane->GetMenuPane()->SetItemButtonState(EPenInputCmdPreviewOff, 
                                                     EEikMenuItemSymbolOn );
        }
    }

void CAknFepManager::SynCCPSoftKey()
    {
    TRAP_IGNORE(HandleCopyCutStateL());   
    }

TInt CAknFepManager::PluginInputMode() const
{
#ifdef RD_SCALABLE_UI_V2
    return iFepPluginManager ? iFepPluginManager->PluginInputMode() : EPluginInputModeNone;
#else
    return EPluginInputModeNone;
#endif 
}

void CAknFepManager::HandleEndKeyL()
    {
#ifdef RD_SCALABLE_UI_V2 

    if (iFepPluginManager)
        {
        return;    
        }

    RWsSession &ws = CCoeEnv::Static()->WsSession();
    //TInt wgId =ws.GetFocusWindowGroup();
    CApaWindowGroupName* wg = CApaWindowGroupName::NewLC(ws,
        CCoeEnv::Static()->WsSession().GetFocusWindowGroup());

    TUid id = wg->AppUid();
    CleanupStack::PopAndDestroy(wg); 

    if (0x10207218 == id.iUid ||
        0x10281EF2 == id.iUid)
        {
        RPeninputServer iPenInputServer;

        TInt err = iPenInputServer.Connect();
		if(err == KErrNone)
			{
			iPenInputServer.SetForeground(ETrue);
			iPenInputServer.ActivateLayout( EFalse ); 
			iPenInputServer.HandleCommand(ECmdPenInputWindowClose);
			
			iPenInputServer.ClearServerEvent();
			iPenInputServer.LoseForeground();
			iPenInputServer.Close();
			}
        }
#endif //RD_SCALABLE_UI_V2         
    }
void CAknFepManager::NeedDisableKeySound(const TKeyEvent& aKeyEvent, TEventCode aEventCode)
    {
#ifdef RD_SCALABLE_UI_V2    
    TInt keyCode = aKeyEvent.iScanCode;
    TBool interestKey = EFalse;
    if ( keyCode == EPtiKey0 || 
         keyCode == EPtiKey1 ||
         keyCode == EPtiKey2 ||
         keyCode == EPtiKey3 ||
         keyCode == EPtiKey4 ||
         keyCode == EPtiKey5 ||
         keyCode == EPtiKey6 ||
         keyCode == EPtiKey7 ||
         keyCode == EPtiKey8 ||
         keyCode == EPtiKey9 ||
         keyCode == EStdKeyDownArrow ||
         keyCode == EStdKeyRightArrow ||
         keyCode == EStdKeyLeftArrow ||
         keyCode == EStdKeyUpArrow ||
         keyCode == EStdKeyBackspace
         )
        {
        interestKey = ETrue;
        }
    if( interestKey && iFepPluginManager && 
        iFepPluginManager->PluginInputMode() == EPluginInputModeItut)
        {
        if( aEventCode == EEventKeyDown )
            {
            static_cast<CAknAppUi*>(CEikonEnv::Static()->EikAppUi())->
                 DisableNextKeySound( keyCode ); 
            }
        else if( aEventCode == EEventKey )
            {
			#ifdef RD_TACTILE_FEEDBACK
            MTouchFeedback::Instance()->
                InstantFeedback(ETouchFeedbackSensitive);
			#endif
            }
        }
#endif     
    }

#ifdef RD_INTELLIGENT_TEXT_INPUT
void CAknFepManager::SetComposChrFlag( TBool aFlag )
    {
    iComposChrFlag = aFlag;
    }
TBool CAknFepManager::GetComposChrFlag( )
    {
    return iComposChrFlag;
    }
#endif

// add for phrase creation User db view
void CAknFepManager::AddUserDBDlgItemL( CAknFepUiInterfaceMenuPane* aMenuPane, TInt aIndex )
    {
    CAknFepUiInterfaceMenuPane::SItemData mData;

    TBuf<KMaxFileName> langStr;
    TInt Inputlangindex;
    if (!aMenuPane->MenuItemExists(EAknCmdUserDBDlg, Inputlangindex))
        {
        StringLoader::Load(langStr, R_AKNFEP_OPTIONS_USER_DB_DLG);

        mData.iCommandId = EAknCmdUserDBDlg;
        mData.iCascadeId = 0;
        mData.iFlags = 0;
        mData.iText.Copy(langStr.Mid(1, langStr.Length() - 1).Left(CAknFepUiInterfaceMenuPane::SItemData::ENominalTextLength));

        aMenuPane->InsertMenuItemL(mData, aIndex);
        }
    }
// add for phrase creation
void CAknFepManager::LaunchUserDBDlgL( )
    {
    TInt itemIndex = 0;
    
    //Change title text
    CEikStatusPane* statusPane = CEikonEnv::Static()->AppUiFactory()->StatusPane();
    CAknTitlePane* title = static_cast< CAknTitlePane* >(
        statusPane->ControlL( TUid::Uid( EEikStatusPaneUidTitle) ) );

    //Save the old title tet
    HBufC* Oldtitletext = title->Text()->AllocLC();
    //Load the new title text
    HBufC* titletext = StringLoader::LoadLC( R_AKNFEP_USER_DB_MANAGEMENT_TITLE );
    //Set title to the new text
    title->SetTextL( *titletext );

    CDesCArray* items = new( ELeave ) CDesCArrayFlat( KSelectItemSize );
    CleanupStack::PushL( items );
    CArrayFix<TInt>* selectedItems = new( ELeave ) CArrayFixFlat<TInt>( KSelectItemSize );
    CleanupStack::PushL( selectedItems );
    
    CAknToolbar* toolbar = iAvkonAppUi->CurrentFixedToolbar();
    
    if( toolbar && toolbar->IsShown() && toolbar->CountComponentControls()>0 )
        {
        toolbar->SetDimmed( ETrue );
        toolbar->DrawDeferred();
        }
    
    TUid naviPaneUid;
	naviPaneUid.iUid = EEikStatusPaneUidNavi;

	CEikStatusPaneBase::TPaneCapabilities subPane =
			statusPane->PaneCapabilities(naviPaneUid);
	// if we can access the navigation pane
	if (subPane.IsPresent() && subPane.IsAppOwned())
		{
		CAknNavigationControlContainer
				* naviPane =
						(CAknNavigationControlContainer *) statusPane->ControlL(naviPaneUid);
		iTapGroup = naviPane->CreateTabGroupL();
		iTapGroup->MakeVisible( EFalse );
		naviPane->PushL( *iTapGroup );
		}
    
    iUserdbdlg = CAknFepUserdbDlg::NewL( itemIndex,
        selectedItems, items, R_AKNFEP_USERDB_DLG_MENUBAR, iPtiEngine, this );
    
    iIsUserdbdlgActive = 1;
    
    iUserdbdlg->RunLD();
    
    iIsUserdbdlgActive = 0;
    
	// if we can access the navigation pane
	if (subPane.IsPresent() && subPane.IsAppOwned())
		{

		CAknNavigationControlContainer
				* naviPane =
						(CAknNavigationControlContainer *) statusPane->ControlL(naviPaneUid);
		naviPane->Pop();
		delete iTapGroup;
		iTapGroup = NULL;
		}
	
    //Set the title to old text
    title->SetTextL( *Oldtitletext );
    
    if( toolbar )
        {
        toolbar->SetDimmed( EFalse );
        toolbar->DrawNow();
        }
    
    CleanupStack::PopAndDestroy( selectedItems );
    CleanupStack::PopAndDestroy( items );
    CleanupStack::PopAndDestroy( titletext );
    CleanupStack::PopAndDestroy( Oldtitletext );
    iUserdbdlg = NULL;
    }
// add for phrase creation
TBool CAknFepManager::IsPinyinPhraseCreation() const
    {
    return iPinyinPhraseCreation;
    }

void CAknFepManager::PinyinPhraseCreation(  TBool aPinyinPhraseCreation )
    {
    if ( iPinyinPhraseCreation != aPinyinPhraseCreation )
        {
        iPinyinPhraseCreation = aPinyinPhraseCreation;
        }
    }
// add for phrase creation
TBool CAknFepManager::IsPhraseCreation() const
    {
    return iPhraseCreation;
    }
// add for phrase creation
void CAknFepManager::PhraseCreation(  TBool aPhraseCreation )
    {
    if ( iPhraseCreation != aPhraseCreation )
        {
        iPhraseCreation = aPhraseCreation;
        }
    }
// add for phrase creation
TBool CAknFepManager::IsEntryPhrase() const
    {
    return iEntryPhrase;
    }
// add for phrase creation
void CAknFepManager::EntryPhrase( TBool aEntryPhrase )
    {
    if ( iEntryPhrase != aEntryPhrase )
        {
        iEntryPhrase = aEntryPhrase;
        }
    }
// add for phrase creation
void CAknFepManager::EnableKeyStar( TBool aEnable )
    {
    if ( iKeyStar != aEnable )
        {
        iKeyStar = aEnable;
        }
    }
// add for phrase creation
TBool CAknFepManager::IsEnableKeyStar( ) const
    {
    return iKeyStar;
    }
#ifdef RD_INTELLIGENT_TEXT_INPUT
	

#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
/**
* @return   ETrue   If the number entry with long key press is supported on QWERTY keyboard.
*/
TBool CAknFepManager::LongPressNumberEntryOnQwerty() const
    {
    TBool value = EFalse;
    if ( iSharedDataInterface )
        {
        value = iSharedDataInterface->LongPressNumberEntryOnQwerty();
        }
    return value;
    }

#endif // __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__


void CAknFepManager::MultitapThroughSCTCharL(TCallBack aCallBack)
    {
    
	TInt res = GetNumericSCTResID();
    
    if(!res)
    	{
    	return;
    	}

    if(!iNumericResourceTimer)
        {        
        iNumericResourceTimer = CChrMultiTapTimer::NewL(aCallBack);
        }
    
    if(!iResourceString || iNumericResourceId !=res )
        {
        // either the resource string has not be created or the resource ID has changed
        if(iResourceString)
            {
            delete iResourceString;
            iResourceString = NULL;
            }
        iNumericResourceId = res;
        TResourceReader reader;
        CEikonEnv::Static()->CreateResourceReaderLC(reader,iNumericResourceId);
        reader.ReadInt32(); // skip sct id 
        iResourceString = reader.ReadHBufC16L();
        CleanupStack::PopAndDestroy();
        }
    if(!iResourceString)
        {
        return;
        }
    if(iNumericResourceTimer->IsActive())
        {
        if( iChrCharIndex == iResourceString->Length() )
            {
            iChrCharIndex = 0;            
            }
    
        iNumericResourceTimer->Cancel();
        iNumericResourceTimer->After(KChrKeyMultitapTimeout);        
        }
    else
        {
        iChrCharIndex = 0;
        iNumericResourceTimer->After(KChrKeyMultitapTimeout);
        }    
    TBuf<1> buf;
    buf.Append((*iResourceString)[iChrCharIndex]); 
    NewCharacterL(buf);
    iChrCharIndex++;
    	if (iResourceString->Length() == 1)
		{
		NumericResourceMultiTapTimerTimeoutL();
		}

    }
TBool CAknFepManager::NumericResourceMultiTapTimerTimeoutL()
    {
    if(iNumericResourceTimer->IsActive())
    	{
    	iNumericResourceTimer->Cancel();
    	if(IsFlagSet(CAknFepManager::EFlagInsideMultitapInlineEditingTransaction))
	        {
	        CommitInlineEditL();
	        return ETrue;
	        } 
    	}       
    return EFalse;    
    }    
void CAknFepManager::ChangeMfneAmPmL()
    {
	//trigger when touch at "AM" or "PM" in ICF
    if (iInputCapabilities.FepAwareTextEditor() && IsMfneEditor() )
        {
        MObjectProvider* mop = iInputCapabilities.ObjectProvider();
        if ( mop && iInputCapabilities.FepAwareTextEditor() )
            {
            CAknExtendedInputCapabilities* extendedInputCapabilities = 
                mop->MopGetObject( extendedInputCapabilities );
            if ( extendedInputCapabilities->Capabilities() && CAknExtendedInputCapabilities::EInputEditorAmPm )// if support AM/PM toggle
                 {
                extendedInputCapabilities->ReportEventL(
                    CAknExtendedInputCapabilities::MAknEventObserver::EChangeAmPm, NULL);
                 }
            }
        }
    }

//-------------------------------------------------------------------

//
// CChrMultiTapTimer
//

// ---------------------------------------------------------------------------
// CChrMultiTapTimer::NewL
// 
// ---------------------------------------------------------------------------
// 
CChrMultiTapTimer* CChrMultiTapTimer::NewL(TCallBack aCallback)
    {
    CChrMultiTapTimer* self = new(ELeave)CChrMultiTapTimer(aCallback);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();    // self
    return self;
    }


// ---------------------------------------------------------------------------
// CChrMultiTapTimer::CChrMultiTapTimer
// 
// ---------------------------------------------------------------------------
// 
CChrMultiTapTimer::CChrMultiTapTimer(TCallBack aCallback)
:CTimer(EPriorityStandard), iCallback(aCallback)
    {
    }

// ---------------------------------------------------------------------------
// CChrMultiTapTimer::ConstructL
// 
// ---------------------------------------------------------------------------
// 
void CChrMultiTapTimer::ConstructL()
    {
    CTimer::ConstructL();
    CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------------------------
// CChrMultiTapTimer::After
// 
// ---------------------------------------------------------------------------
// 
void CChrMultiTapTimer::After(TTimeIntervalMicroSeconds32 aInterval)
    {
    if (IsActive())
        {
        Cancel();
        }
    CTimer::After(aInterval);
    }

// ---------------------------------------------------------------------------
// CChrMultiTapTimer::RunL
// 
// ---------------------------------------------------------------------------
// 
void CChrMultiTapTimer::RunL()
    {
    iCallback.CallBack();
    }

TBool CAknFepManager::IsLanguageSupportPrediction()
    {
    TBool isLanguagugeSupportsPrediction = EFalse;
    switch(iKeyboardType)
        {
        case EPtiKeyboardHalfQwerty:
#ifdef __HALF_QWERTY_KEYPAD
            isLanguagugeSupportsPrediction = (iQwertyInputMode) & (iLanguageCapabilities.iSupportsWesternHalfQwertyPredictive); 
#endif              
            break;
        case EPtiKeyboardQwerty4x12:
        case EPtiKeyboardQwerty4x10:
        case EPtiKeyboardQwerty3x11:
        case EPtiKeyboardCustomQwerty:
            isLanguagugeSupportsPrediction = (iQwertyInputMode) & (iLanguageCapabilities.iSupportsWesternQwertyPredictive);
            break;
        case EPtiKeyboardNone:
        case EPtiKeyboard12Key:
            isLanguagugeSupportsPrediction = (!iQwertyInputMode) & (iLanguageCapabilities.iSupportsWesternPredictive);
            break;
        default:
            isLanguagugeSupportsPrediction = (!iQwertyInputMode) & (iLanguageCapabilities.iSupportsWesternPredictive);
            break;    
        }    
    return isLanguagugeSupportsPrediction;
    }
#endif //RD_INTELLIGENT_TEXT_INPUT

TBool CAknFepManager::IsAllowedKeymappingForNumberMode(TChar aChar) const
    {
    TBool response = EFalse;


    switch ( iAknEditorNumericKeymap )
        {
        case EAknEditorStandardNumberModeKeymap:
        case EAknEditorFixedDiallingNumberModeKeymap:
        case EAknEditorSATNumberModeKeymap:
        case EAknEditorSATHiddenNumberModeKeymap:
            {
            response = (aChar == '#');
            }
            break;
        case EAknEditorCalculatorNumberModeKeymap:
        case EAknEditorConverterNumberModeKeymap:
            {
            TLocale tLoc;
            response = (aChar == tLoc.DecimalSeparator());
            }
            break;
        case EAknEditorToFieldNumberModeKeymap:
            {
            response = (aChar == ';');
            }
            break;
        case EAknEditorPlainNumberModeKeymap:
        case EAknEditorReservedInternalUseKeymap:
        default:
            break;
        }

    return response;
    }
/**
  * Return wether ipnut mode is change by press shift+space. 
  * This is for half-qwerty.
  */
TBool CAknFepManager::IsChangeModeByShiftAndSpace()
    {
    return iShiftAndSpace;
    }

/**
  * Set wether ipnut mode is change by press shift+space. 
  * This is for half-qwerty.
  */
void CAknFepManager::SetChangeModeByShiftAndSpace( TBool aFlag )
    {
    iShiftAndSpace = aFlag;
    }

void CAknFepManager::HideExactWordPopUp()
	{
	iExactWordPopupContent->HidePopUp();
	TRAP_IGNORE( SendEventsToPluginManL( EPluginHideTooltip ));
	}

//========================================================================
//
// This function judge whether the texts inputted exceed 
// the max length of the editor. 
// 
//========================================================================
TBool CAknFepManager::IsTextExceedLeghthEditor( TInt aNewInlineTextLenght )
	{
    
    // Get the length of the newest text that contains uncommitted texts 
	TInt curPos = iUncommittedText.iAnchorPos + aNewInlineTextLenght;
    
    // Get the length of the text that is newly increased in uncommitted texts.
	TInt deltaCur = curPos - iUncommittedText.iCursorPos;
	
    // Get the max length of the editor
	TInt maxEdSize = iInputCapabilities.FepAwareTextEditor()->DocumentMaximumLengthForFep();

    // Get the length of current the texts in the editor. The texts contain committed and uncomitted.    
	TInt docLength = iInputCapabilities.FepAwareTextEditor()->DocumentLengthForFep();
	
      // DeltaCur is greater than zero.
      if((maxEdSize > 0) && ( deltaCur > 0 ) 
							&& (( docLength + deltaCur) > maxEdSize ) )
         {
         return ETrue;
         }
      // DeltaCur is negative. For example uncommitted texts contain candidate words.
      else if ( (maxEdSize > 0) && (curPos > maxEdSize) )
          {
          return ETrue;
          }
        else
          {

          // Not exceeding the max length of editor
          return EFalse;
          }
	}


TBool CAknFepManager::IsExactWordPopUpShown()
	{
#ifdef RD_SCALABLE_UI_V2 
	return iExactWordPopupContent->IsPopUpVisible()
           || ( iFepPluginManager && iFepPluginManager->IsTooltipOpenOnFSQ() );
#else
	return iExactWordPopupContent->IsPopUpVisible();
#endif // RD_SCALABLE_UI_V2
	}
void CAknFepManager::StopDisplayingMenuBar()
    {
    if (iEditMenuBar)
        {
        iEditMenuBar->StopDisplayingMenuBar();
        iEditMenuBar = NULL;
        }
#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__    
    // temporary solution for Entering key on virtual QWERTY not working 
    // like enter on virtual ITU-T or on external keyboard
    if ( iOptionsMenuBar && !( iFepPluginManager && 
        ( iFepPluginManager->CurrentPluginInputMode() == EPluginInputModeFSQ || 
          iFepPluginManager->CurrentPluginInputMode() == EPluginInputModePortraitFSQ ) ) )
        {
        iOptionsMenuBar->StopDisplayingMenuBar();
        iOptionsMenuBar = NULL;
        }
#endif //__ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__   
    }

void CAknFepManager::SetQwertyModeToInputcapbility()
    {
    if ( iSharedDataInterface->QwertyInputMode() )
        {
        SetExtendedInputCapabilities( ExtendedInputCapabilities() |
            CAknExtendedInputCapabilities::EInputEditorQwertyInputActive  );
        }
    else
        {
        SetExtendedInputCapabilities( ExtendedInputCapabilities() &
            ~CAknExtendedInputCapabilities::EInputEditorQwertyInputActive  );
        }
    }
void CAknFepManager::SetExtendedInputCapabilities( TUint aCapabilities)
    {
    MObjectProvider* mop = iInputCapabilities.ObjectProvider();

    if ( mop && iInputCapabilities.FepAwareTextEditor() )//IsFepAwareTextEditor()
        {
        CAknExtendedInputCapabilities* extendedInputCapabilities =
        mop->MopGetObject( extendedInputCapabilities );

        if ( extendedInputCapabilities )
            {
            extendedInputCapabilities->SetCapabilities( aCapabilities );
            }
        }

    return;
    }

void CAknFepManager::PrepareFepAfterDialogExitL(TUid aFepUid)
    {
    // This ensures that on 3rd party fep change we do not have access to other fep variables
    if (aFepUid != CCoeEnv::Static()->FepUid())
        {
		// In case the fep has changed, then leave
        User::Leave(KErrNone);
        }
	// Once the editor which launches a dialog is destroyed, this flag is set
	// In case we have any dialog launched we leave as the underneath editor is
	// destroyed
    if (IsExtendedFlagSet(EExtendedFlagEdwinEditorDestroyed))
        {
        if(!iFepAwareDialogParentEditor)
            {
			// For Eg: Messaging editor launches spell query which inturn launches
			// SCT. Then don't clear the flag once we dismiss SCT because we even
			// need to dismiss spell query dialog.
            ClearExtendedFlag(EExtendedFlagEdwinEditorDestroyed);
            }       
        User::Leave(KErrNone);
        }
    if(!iFepAwareDialogParentEditor)
        {
        ClearExtendedFlag(EExtendedFlagEdwinEditorDestroyed);
        }    
    }

void CAknFepManager::PrepareFepForFepAwareDialogLaunch()
    {
    iFepAwareDialogParentEditor = EditorState();
    SetExtendedFlag(EExtendedFlagFepAwareDialogLaunched);
    }

void CAknFepManager::PrepareFepAfterFepAwareDialogExitL(TUid aFepUid)
    {
    // This ensures that on 3rd party fep change we do not have access to other fep variables
    if (aFepUid != CCoeEnv::Static()->FepUid())
        {
        // In case the fep has changed, then leave
        User::Leave(KErrNone);
        }    
    iFepAwareDialogParentEditor = NULL;
    ClearExtendedFlag(EExtendedFlagFepAwareDialogLaunched);
    }

void CAknFepManager::ConvertCharToKey(TChar aChar, TUint16& aKey) const
    {
    aKey = 0;
	
	if (!iPtiEngine)
		return;
		
    CPtiCoreLanguage* lang = static_cast<CPtiCoreLanguage*>( iPtiEngine->CurrentLanguage() );
    if (!lang)
        return;

    const TBool vietnamese = (iLanguageCapabilities.iInputLanguageCode == ELangVietnamese);
    
    MPtiKeyMappings* map;
    TPtiKeyboardType keyboardLayout = KeyboardLayout();
	if ( keyboardLayout == EPtiKeyboardQwerty4x12 
		 || keyboardLayout == EPtiKeyboardQwerty4x10
		 || keyboardLayout == EPtiKeyboardQwerty3x11
		 || keyboardLayout == EPtiKeyboardCustomQwerty )
		{
		map = lang->GetQwertyKeymappings();
		}
	else if( keyboardLayout == EPtiKeyboardHalfQwerty )
		{
		map = lang->GetHalfQwertyKeymappings();
		}
	else    
		{
		map = lang->GetKeymappings();
		}          

    if (!map)
        {
        return;
        }   

    TUint16 ch;
    ch = (TUint16)map->KeyForCharacter(aChar); 
    if ( !ch && vietnamese 
         && !( iFepPluginManager && iFepPluginManager->IsSupportITIOnFSQ() ) )
        {
        // This may be Vietnamese tone mark or an accented character which isn't listed
        // in keymappings. 
        ch = RemapVietnameseAccentedCharacter(aChar);
        }

    aKey = ch;
    }

TBool CAknFepManager::GetSctLengthL(TInt resourceId)const
	{
	CCoeEnv* coeEnv = CCoeEnv::Static();
	TResourceReader reader;
	CEikonEnv::Static()->CreateResourceReaderLC(reader,resourceId);
	TInt component_count=reader.ReadInt16();
	TInt length = 0;
	TBool isEmpty = ETrue;
	HBufC* sctChar = NULL;
	
	for (TInt ii=0;ii<component_count;ii++)
		{
		TInt component_id=reader.ReadInt16();
		switch(component_id)
			{
			case EAknSCTLowerCase:
				{
		        sctChar = reader.ReadHBufCL();
		        length += (sctChar != NULL)? sctChar->Length(): 0;
		        break;
				}
			case EAknSCTUpperCase:
				{
		        sctChar = reader.ReadHBufCL();
		        length += (sctChar != NULL)? sctChar->Length(): 0;
		        break;
				}
			case EAknSCTNumeric:
				{
		        sctChar = reader.ReadHBufCL();
		        length += (sctChar != NULL)? sctChar->Length(): 0;
		    	break;
				}
		    case EAknSCTFullCase:
		        {
		        sctChar = reader.ReadHBufCL();
		        length += (sctChar != NULL)? sctChar->Length(): 0;
		        break;
		        }
		    case EAknSCTHalfCase:
		       	{
		        sctChar = reader.ReadHBufCL();
		        length += (sctChar != NULL)? sctChar->Length(): 0;
		        break;
		       	}
		    case EAknSCTQwerty:
		        {
		        sctChar = reader.ReadHBufCL();
		        length += (sctChar != NULL)? sctChar->Length(): 0;
		        break;
		        }
		   	default:
		        break;
			}
	    delete sctChar;
	    sctChar = NULL;
	    if(length > 0)
	    	{
            isEmpty = EFalse;
	    	break;
	    	}
		}
	CleanupStack::PopAndDestroy();  //reader 
	return isEmpty;
	}
    
// ========================================
// Update Editor State Flag
// Add editor state flags or 
// Remove editor state flags
// ========================================
void CAknFepManager::UpdateEditorStateFlags( TInt aFlag, TBool aAdd ) const
    {
    CAknEdwinState* state( EditorState() );
    if ( state )
        {
        TInt flags( state->Flags() );
        if ( aAdd )
            {
            flags |= aFlag;  // Add flag
            }
        else
            {
            flags &= ~aFlag;  // Remove flag
            }
        state->SetFlags( flags );
        }
    }


// ---------------------------------------------------------------------------
// CAknFepManager::HandleFnKeyPressMonitorCallback
// Handle fnkey press monitor callback
// static function
// ---------------------------------------------------------------------------
//
TInt CAknFepManager::HandleFnKeyPressMonitorCallback(TAny* aObj)
	{
    TRAPD(err, static_cast<CAknFepManager*>(aObj)->HandleFnKeyPressMonitor());
    if (err)
        {
        static_cast<CAknFepManager*>(aObj)->CleanUpFep();
        return KErrDied;
        }
    return KErrNone;
	}

// ---------------------------------------------------------------------------
// CAknFepManager::HandleFnKeyPressMonitor
// Handle fnkey press monitor callback
// ---------------------------------------------------------------------------
//
void CAknFepManager::HandleFnKeyPressMonitor()
	{	
	 DeactivateFnkeyPressMonitor();
	 if (IsExtendedFlagSet ( EExtendedFlagFnKeyNeedLockMode ) )
		 {
	      iFnKeyManager->SetFnKeyState(CAknFepFnKeyManager::EFnKeyLock);
		 }
	}

// ---------------------------------------------------------------------------
// CAknFepManager::ActivateFnkeyPressMonitor
// Activate Fnkey press Monitor
// ---------------------------------------------------------------------------
//
void CAknFepManager::ActivateFnkeyPressMonitor()
	{
	 if( iFnKeypressMonitor )
		 {	      
	      if ( iFnKeypressMonitor->IsActive() )
	    	  {
	           iFnKeypressMonitor->Cancel();
	    	  }
	      
		  iFnKeypressMonitor->Start(KFnKeyLongPressTimeout, KFnKeyLongPressTimeout, 
							   TCallBack(HandleFnKeyPressMonitorCallback, this));
		 }
	}

// ---------------------------------------------------------------------------
// CAknFepManager::DeactivateFnkeyPressMonitor
// Deactivate Fnkey press Monitor
// ---------------------------------------------------------------------------
//
void CAknFepManager::DeactivateFnkeyPressMonitor()
	{
	 if( iFnKeypressMonitor && iFnKeypressMonitor->IsActive())
		 {
		   iFnKeypressMonitor->Cancel();
		 }
	}

// ---------------------------------------------------------------------------
// LOCAL METHODS
// 
// ---------------------------------------------------------------------------
//     
const TInt KVietKeyMarker = 0xffff;
const TUint16 vietnameseAccents[] =
    {
    KVietKeyMarker, EPtiKey2, // For key 2, a-based    
    0x00c1,	0x00c0, 0x1ea2, 0x00c3, 0x1ea0,
    0x00e1, 0x00e0, 0x1ea3, 0x00e3, 0x1ea1,
    0x0102, 0x1eae, 0x1eb0, 0x1eb2, 0x1eb4, 0x1eb6,
    0x0103, 0x1eaf, 0x1eb1, 0x1eb3, 0x1eb5, 0x1eb7,
    0x00c2, 0x1ea4, 0x1ea6, 0x1ea8, 0x1eaa, 0x1eac,
    0x00e2, 0x1ea5, 0x1ea7, 0x1ea9, 0x1eab, 0x1ead, 

    KVietKeyMarker, EPtiKey3, // For key 3, e-based    
    0x00c9,	0x00c8, 0x1eba, 0x1ebc, 0x1eb8,
    0x00e9, 0x00e8, 0x1ebb, 0x1ebd, 0x1eb9,
    0x00ca, 0x1ebe, 0x1ec0, 0x1ec2, 0x1ec4, 0x1ec6,
    0x00ea, 0x1ebf, 0x1ec1, 0x1ec3, 0x1ec5, 0x1ec7,

    KVietKeyMarker, EPtiKey4, // For key 4, i-based    
    0x00cd,	0x00cc, 0x1ec8, 0x0128, 0x1eca,
    0x00ed, 0x00ec, 0x1ec9, 0x0129, 0x1ecb,
    
    KVietKeyMarker, EPtiKey6, // For key 6, o-based    
    0x00d3,	0x00d2, 0x1ece, 0x00d5, 0x1ecc,
    0x00f3, 0x00f2, 0x1ecf, 0x00f5, 0x1ecd,
    0x00d4, 0x1ed0, 0x1ed2, 0x1ed4, 0x1ed6, 0x1ed8,
    0x00f4, 0x1ed1, 0x1ed3, 0x1ed5, 0x1ed7, 0x1ed9,
    0x01a0, 0x1eda, 0x1edc, 0x1ede, 0x1ee0, 0x1ee2,
    0x01a1, 0x1edb, 0x1edd, 0x1edf, 0x1ee1, 0x1ee3, 
    0
    }; 


// ---------------------------------------------------------------------------
// RemapVietnameseAccentedCharacter
// 
// ---------------------------------------------------------------------------
// 
LOCAL_C TInt RemapVietnameseAccentedCharacter(TUint16 aChr)
	{
	// Check tone marks first.	
	switch (aChr)
		{
		case 0x0301:
		     return EPtiKey2;
		case 0x0300:
		     return EPtiKey3;
		case 0x0309:
		     return EPtiKey4;		     
		case 0x0303:
			 return EPtiKey5;			
		case 0x0323:
			 return EPtiKey6;						 
		default:
			break; 	
		}	 
	
	TInt ret = 0;
	
	for (const TUint16* ptr = vietnameseAccents; *ptr != 0; ptr++)
		{
		if (*ptr == KVietKeyMarker)
			{
			ret = ptr[1];
			ptr++;
			}
		else
			{
			if (*ptr == aChr)
				{
				break;
				}
			}			
		}
		
	return ret;	 		
	}

// End of file
