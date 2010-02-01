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
* Description:            Implementation of CAknFepLanguageManager class.
*
*/












// INCLUDE FILES
#include "AknFepGlobalEnums.h"
#include "AknFepPanic.h"
#include "AknFepManager.h"                  // CAknFepManager
#include "AknFepManagerInterface.h"
#include "AknFepUIManagerStateInterface.h"  // MAknFepUIManagerStateInterface
#include "AknFepUiInputStateBase.h"         // TAknFepInputStateBase
#include "AknFepUiManagerBase.h"            // CAknFepUIManagerBase
#include "AknFepUIManagerJapanese.h"        // CAknFepUIManagerJapanese
#include "AknFepUiManagerWestern.h"         // CAknFepUIManagerWestern
#include "AknFepUiManagerChinese.h"         // CAknFepUIManagerChinese
#include "AknFepLanguageManager.h"          // this class

#include "AknFepUiManagerKorean.h"        	// CAknFepUIManagerKorean
#include <uikon.hrh>                        // TAknEditorNumericKeymap
#include <AknUtils.h>

#include <aknfeppeninputimeplugin.h>
#include  "AknFepPluginManager.h"
#include <peninputcmd.h>
#include <peninputclient.h>

void Cleanup( TAny* aAny )
	{
	RImplInfoPtrArray* implArray = 
		reinterpret_cast< RImplInfoPtrArray*> ( aAny );
	implArray->ResetAndDestroy();
	implArray->Close();
	}

/**
 * CAknFepLanguageManager
 *
 */
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAknFepLanguageManager()
// first phase construction.
// -----------------------------------------------------------------------------
//
CAknFepLanguageManager* CAknFepLanguageManager::NewL(MAknFepManagerUIInterface* aFepMan, 
                                                     CAknFepCaseManager* aCaseMan)
    {
    CAknFepLanguageManager* self = new(ELeave) CAknFepLanguageManager(aFepMan, aCaseMan);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(); // self
    return self;
    }

// -----------------------------------------------------------------------------
// ~CAknFepLanguageManager()
// Destructor.
// -----------------------------------------------------------------------------
//
CAknFepLanguageManager::~CAknFepLanguageManager()
    {
    if (iUiManArray)
        {
        iUiManArray->ResetAndDestroy();
        }
    delete iUiManArray;
    delete iCurImePlugin;
    }

// -----------------------------------------------------------------------------
// CAknFepLanguageManager::GetFepUI()
// get ui manager object
// -----------------------------------------------------------------------------
//
MAknFepManagerInterface* CAknFepLanguageManager::GetFepUI(TInt aMode, TWidthChar /*aCharacterWidth*/, TBool aPredictive)
    {
    MAknFepManagerInterface* uiMan = NULL;
    switch (aMode)
        {
        case EPinyin:
        case EZhuyin:
        case EStroke:
        case ECangJie:
        case EZhuyinFind:
        case EStrokeFind:
            uiMan = iUiManArray->At(EUiManChinese);
            break;
        case ELatin:
        case EHindi:
            if (iInputLanguage == ELangJapanese)
                {
                uiMan = iUiManArray->At((aPredictive)? EUiManWestern : EUiManJapanese);
                }
        	else if(iInputLanguage == ELangKorean)
        		{
        		uiMan = iUiManArray->At(EUiManKorean);
        		}
            else
                {
                uiMan = iUiManArray->At(EUiManWestern);
                }
            break;
        case ENumber:
        case ENativeNumber:
            if (iInputLanguage == ELangJapanese)
                {
                uiMan = iUiManArray->At(EUiManJapanese);
                }
            else if(iInputLanguage == ELangKorean)
                {
                uiMan = iUiManArray->At(EUiManKorean);
                }
            else
                {
                uiMan = iUiManArray->At(EUiManWestern);
                }
            break;
        case EHiragana:
        case EHiraganaKanji:
        case EKatakana:
            uiMan = iUiManArray->At(EUiManJapanese);
            break;
        case EHangul:
        	uiMan = iUiManArray->At(EUiManKorean);
 			break;
        default:
            AknFepPanic(EAknFepPanicNoUiManager);
            break;
        }
    return uiMan;
    }


// -----------------------------------------------------------------------------
// CAknFepLanguageManager::SetInputLanguageL()
// Set new input language.
// -----------------------------------------------------------------------------
//
void CAknFepLanguageManager::SetInputLanguageL(TInt aInputLanguage)
    {
    iInputLanguage = aInputLanguage;
    MAknFepManagerInterface* uiManager = NULL;
    TInt uiManagerId = UiManagerId();
    uiManager = iUiManArray->At(uiManagerId);
    
    if ( !uiManager )
        {
        // Ui manager does not exist for this language yet. We create it.
        ConstructUiManagerL();
        uiManager = iUiManArray->At(uiManagerId);
        }
    
    uiManager->SetInputLanguageL((TLanguage)iInputLanguage);
    
    if (uiManagerId != EUiManWestern)
        {
        // Western UI manager needs to know current input language also when 
        // Chinese or Japanese Ui manager is used.
        // Western UI manager needs to use English language if the 
        // input language is Chinese or Japanese.
        uiManager = iUiManArray->At(EUiManWestern);
        if (uiManager)
            {
            uiManager->SetInputLanguageL((TLanguage)iInputLanguage);
            }
        }
        
    }


// -----------------------------------------------------------------------------
// CAknFepLanguageManager()
// Constructor.
// -----------------------------------------------------------------------------
//
CAknFepLanguageManager::CAknFepLanguageManager(MAknFepManagerUIInterface* aFepManager, 
                                               CAknFepCaseManager* aCaseManager) 

:iInputLanguage(ELangEnglish), 
 iFepManager(aFepManager), 
 iCaseManager(aCaseManager),
 iPluginManager(NULL)
    {
    }

// -----------------------------------------------------------------------------
// CAknFepLanguageManager::ConstructL()
// Second phase construction
// -----------------------------------------------------------------------------
//
void CAknFepLanguageManager::ConstructL()
    {
    iUiManArray = new(ELeave)CArrayPtrFlat<MAknFepManagerInterface>(EUiManMax);

    for ( TInt ii = 0; ii < EUiManMax; ii++)
        {
        iUiManArray->AppendL(NULL);
        }

    // Create UI manager for English language. English exists in all variants and
    // it is default language in CAknFepLanguageManager.
    ConstructUiManagerL();
    }

void CAknFepLanguageManager::ConstructUiManagerL()
    {
    MAknFepManagerInterface* uiMan = NULL;
    TInt uiManagerId = UiManagerId();
    iUiManArray->Delete(uiManagerId);
    switch (uiManagerId)
        {
        case EUiManWestern:
            uiMan = CAknFepUIManagerWestern::NewL(iFepManager, iCaseManager, 
                                                 (TLanguage)iInputLanguage);
            break;
        case EUiManChinese:
            uiMan = CAknFepUIManagerChinese::NewL(iFepManager, iCaseManager, 
                                                 (TLanguage)iInputLanguage);
            break;
        case EUiManJapanese:
            uiMan = CAknFepUIManagerJapanese::NewL(iFepManager, iCaseManager, 
                                                  (TLanguage)iInputLanguage);
            break;
        case EUiManKorean:
        	uiMan = CAknFepUIManagerKorean::NewL(iFepManager, iCaseManager, (TLanguage)iInputLanguage);
        	break;
        default:
            AknFepPanic(EAknFepPanicNoUiManager);
            break;
        }
    CleanupStack::PushL(uiMan);
    iUiManArray->InsertL(uiManagerId, uiMan);
    CleanupStack::Pop(); // uiMan
    }

TInt CAknFepLanguageManager::UiManagerId()
    {
    TInt uiManagerId = EUiManWestern;
    switch (iInputLanguage)
        {
        case ELangPrcChinese:
        case ELangHongKongChinese:
        case ELangTaiwanChinese:
            uiManagerId = EUiManChinese;
            break;
        case ELangJapanese:
            uiManagerId = EUiManJapanese;
            break;
        case ELangKorean:
        	uiManagerId = EUiManKorean;
        	break;
        default:
            // Western UI manager is default.
            // Thai, Arabic and Hebrew uses western UI manager.
            break;
        }
    return uiManagerId;
    }

// -----------------------------------------------------------------------------
// CAknFepLanguageManager::GetPluginInputFepUiL
// Get plugin UI manager object.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MAknFepManagerInterface* CAknFepLanguageManager::GetPluginInputFepUiL(
                                                      TPluginInputMode aPreferedMode,
                                                      TInt aPenInputLang,
                                                      TInt aDisplayLang,
                                                      RPeninputServer* aPenServer)
    {

    RArray<TInt> implList;
    CleanupClosePushL(implList);
    
    TPluginInputMode secondMode = EPluginInputModeItut;
    TPluginInputMode curMode = aPreferedMode;
    TInt language = aPenInputLang;

    //pen input language and prefered mode
    aPenServer->GetImePluginIdListL(aPenInputLang, curMode, implList);
    
    if( implList.Count() == 0 )
        {
        //pen input language and other modes
        curMode = secondMode;
        aPenServer->GetImePluginIdListL(aPenInputLang, curMode, implList);
        }
        
    if( implList.Count() == 0 && aPenInputLang != aDisplayLang)
        {
        //display language and prefered modes
        language = aDisplayLang;
        curMode = aPreferedMode;
        aPenServer->GetImePluginIdListL(aDisplayLang, curMode, implList);

        if( implList.Count() == 0 )
            {
            //display language and other modes
            curMode = secondMode;
            aPenServer->GetImePluginIdListL(aDisplayLang, curMode, implList);
            }
        }
        
    CAknFepPenInputImePlugin* pIme = NULL;
    MAknFepManagerInterface* pUi = NULL;
    TInt i;
    for(i = 0; i < implList.Count(); ++i)
        {
        pIme = ActivateImePlugin(implList[i], aPenServer);
        if( pIme )
            {
            pUi = ActivateLayoutUiPluginL(pIme, language, curMode);
            if( pUi )
                {
                iCurUiPlugIn = pUi;
                if( iCurImePlugin != pIme )
                    {
                    //a new IME plugin created
                    delete iCurImePlugin;
                    iCurImePlugin = pIme;
                    }
                break;
                }
            else
                {
                //Fail create UI, delete IME plugin just created
                 if( iCurImePlugin != pIme )
                     {
                     delete pIme;
                     }
                }
            }        
        }
    
    if( i == implList.Count() )
        {
        delete iCurImePlugin;
        iCurImePlugin = NULL;
        iCurUiPlugIn = NULL;
        }
    CleanupStack::PopAndDestroy(&implList);    
    
    if ( iCurUiPlugIn )
    	{
    	aPenServer->SetInputLanguage( language );
    	}
    return iCurUiPlugIn;
    }

TBool  CAknFepLanguageManager::IsPluginInputAvaiable(TPluginInputMode aMode, 
                                                     TInt aPenInputLang,
                                                     RPeninputServer* aPenServer)
    {
    TBool bAvail = EFalse;
    TRAP_IGNORE(bAvail = IsPluginInputAvaiableL(aMode, aPenInputLang, aPenServer));
    return bAvail;
    }

TBool  CAknFepLanguageManager::IsPluginInputAvaiableL(TPluginInputMode aMode, 
                                                     TInt aPenInputLang,
                                                     RPeninputServer* aPenServer)
    {
    RArray<TInt> implList;
    CleanupClosePushL(implList);
    
    aPenServer->GetImePluginIdListL(aPenInputLang, aMode, implList);    
    TBool bAvail = implList.Count() != 0;
    CleanupStack::PopAndDestroy(&implList);    
    return bAvail;
    }

// -----------------------------------------------------------------------------
// CAknFepLanguageManager::ActivateImePlugin
// Activate IME plugin by implemenation.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepPenInputImePlugin* CAknFepLanguageManager::ActivateImePlugin(
                                                 TInt aImeImplId, 
                                                 RPeninputServer* aPenServer)
    {
    if(iCurImePlugin && iCurImePlugin->ImeImplId() == aImeImplId)
        {
        return iCurImePlugin;
        }
    else
        {
        TUid dtor_ID_Key;
        CAknFepPenInputImePlugin* pIme = NULL;

        TRAP_IGNORE(pIme = REINTERPRET_CAST(CAknFepPenInputImePlugin*, 
              REComSession::CreateImplementationL(TUid::Uid(aImeImplId), dtor_ID_Key, aPenServer)));

        if( pIme )
            {
            pIme->iDtor_ID_Key = dtor_ID_Key;           
            }

        return pIme;
        }
    }

// -----------------------------------------------------------------------------
// CAknFepLanguageManager::ActivateLayoutUiPluginL
// Activate IME plugin's UI by mode.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MAknFepManagerInterface* CAknFepLanguageManager::ActivateLayoutUiPluginL(
                                                   CAknFepPenInputImePlugin* aImePlugin, 
                                                   TInt aPenInputLang,
                                                   TPluginInputMode aMode)
    {
    MAknFepManagerInterface* pUi = NULL;

    if( aImePlugin )
        {
        pUi = aImePlugin -> GetInputMethodUiL(iFepManager,
                                            (TLanguage)aPenInputLang, 
                                            aMode,
                                            KNullDesC8(),
                                            iIsSplitView);
        //set inputlanguage                                                             
        if(pUi)
            {
            pUi->SetInputLanguageL((TLanguage)aPenInputLang);
            //pUi->ActivateUI();
            }
        }
    
    return pUi;
    }

// -----------------------------------------------------------------------------
// CAknFepLanguageManager::SetSplitView
// Activate IME plugin's UI by mode.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepLanguageManager::SetSplitView(TBool aIsSplitView)
    {
    iIsSplitView = aIsSplitView;
    }

// End of File  
