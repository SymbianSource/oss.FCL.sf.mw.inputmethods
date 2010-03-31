/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
*
*/

#include <featmgr.h>
#include <implementationinformation.h>
#include <aknfeppeninputenums.h>
#include <aknfepuiinterface.h>
#include <aknfepuimenus.h>
#include <aknlistquerydialog.h>

#include <coemain.h>
#include <bautils.h>
#include <StringLoader.h>

#include <peninputcmd.h>
#include <PeninputPluginKrRes.rsg>
#include <peninputclient.h>
#include <AknFepInternalCRKeys.h>



#include "PeninputPluginKrDbg.h"
#include "PeninputPluginKr.hrh"   // CPeninputPluginKr
#include "PeninputPluginKr.h"	// CPeninputPluginKr
#include "PeninputPluginKr.pan"	  	// panic codes
#include "PluginKrFepManagerBase.h"     // panic codes
#include "PluginKrFepManagerHwr.h"
#include "PluginKrFepManagerVkb.h"
#include "PluginKrFepManagerItut.h"

//_LIT(KFscImeName, "Korean FSC");
//_LIT(KHwrImeName, "Korean HWR");
_LIT(KVkbImeName, "Korean VKB");
_LIT(KFsqImeName, "Korean FSQ");
//_LIT(KFsqImeNameEng, "English FSQ");
_LIT(KItutImeName, "Korean ITUT");

_LIT(KPeninputPluginKrResourceFileName, "z:\\resource\\fep\\PeninputPluginKrRes.rsc");

const TInt KItutUiIndex = 0;
const TInt KVkbUiIndex = 1;
const TInt KFSQUiIndex = 2;
//const TInt KFSQUiEngIndex = 3;
const TInt KInvalidImplId = 0;

enum TPeninputKoreanWordCombineFlags
    {
    EKoreanWordCombineVersion = 0x00000001,
    EKoreanWordCombineConsonantSsangKiyeok = 0x00000002,
    EKoreanWordCombineConsonantSsangTikeut = 0x00000004,
    EKoreanWordCombineConsonantSsangPieup =  0x00000008,
    EKoreanWordCombineConsonantSsangSios =   0x00000010,
    EKoreanWordCombineConsonantCieuc =       0x00000020,
    EKoreanWordCombineConsonantYaPlusI =     0x00000040,
    EKoreanWordCombineConsonantYuPlusI =     0x00000080,
    EKoreanWordCombineConsonantOnSetting =   0x00000100
    };

LOCAL_C void Cleanup( TAny* aAny )
    {
    RImplInfoPtrArray* implArray = 
        reinterpret_cast< RImplInfoPtrArray*> ( aAny );
    implArray->ResetAndDestroy();
    implArray->Close();
    }

// ---------------------------------------------------------------------------
// Constructors & Destructors
// ---------------------------------------------------------------------------
//

CPeninputPluginKr* CPeninputPluginKr::NewLC(TAny* aInitParams)
    {
    CPeninputPluginKr* self = new (ELeave) CPeninputPluginKr(
        static_cast<RPeninputServer*>(aInitParams));
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

CPeninputPluginKr* CPeninputPluginKr::NewL(TAny* aInitParams)
    {
    CPeninputPluginKr* self = CPeninputPluginKr::NewLC(aInitParams);
    CleanupStack::Pop(self);
    return self;
    }

CPeninputPluginKr::CPeninputPluginKr(RPeninputServer* aServer)
    : iPeninputServer(*aServer)
    {
    }

void CPeninputPluginKr::ConstructL()
    {
    /*
    LOG("FEP.IMEPLG.KR.ConstructL");
    FindUiLayoutImplementationL();
    */
    FindUiLayoutImplementationL();
    //Append NULL to save HWR manager position
    iAllUiManagers.Append(NULL);
    //Append NULL to save VKB manager position
    iAllUiManagers.Append(NULL);
    //Append NULL to save ITUT manager position
    iAllUiManagers.Append(NULL);
    //Append NULL to save Mini ITUT manager position
    iAllUiManagers.Append(NULL);
    //Append NULL to save FSQ manager position
    iAllUiManagers.Append(NULL);
    
    TFileName resourceName(KPeninputPluginKrResourceFileName);
    CCoeEnv* coeEnv = CCoeEnv::Static();
    BaflUtils::NearestLanguageFile(coeEnv->FsSession(), resourceName);
    iResId = coeEnv->AddResourceFileL(resourceName);
    
    iRepository=CRepository::NewL(KCRUidAknFep);
    
    }

CPeninputPluginKr::~CPeninputPluginKr()
    {
    delete iRepository;
    CCoeEnv::Static()->DeleteResourceFile(iResId);
    DeActivate();
    iUiLayoutImpIdList.Close();
    delete iPluginKrFepManager;
    iAllUiManagers.ResetAndDestroy();
    REComSession::FinalClose();//cleanup ecom
    }

// ---------------------------------------------------------------------------
// from CAknFepPenInputImePlugin
// ---------------------------------------------------------------------------
//

MAknFepManagerInterface* CPeninputPluginKr::GetInputMethodUiL(
        MAknFepManagerUIInterface* aFepManager,
        TLanguage aLanguage, 
        TInt aMode,
        const TDesC8& /*aData*/,
        const TBool /*aIsSplitView*/ )
    {
    iFepManager = aFepManager;
    iLanguage = aLanguage;
    CPluginKrFepManagerBase* fepmanager = NULL;

    TUid layoutId;
    TInt i;

    switch(aMode)
        {
        case EPluginInputModeVkb:
            layoutId.iUid = KVkbUiId;
            fepmanager = GetVkbUiL();
            break;
        /*   
        case EPluginInputModeItut:
            layoutId.iUid = KItutUiId;
            fepmanager = GetItutUiL();
            break;*/
            
        case EPluginInputModeFSQ:
        	{
        	/*if(iLanguage == ELangEnglish)
        		{
        		layoutId.iUid = KFsqUiEngId;
        		}
        	else*/
        		{
        		layoutId.iUid = KFsqUiId;
        		}
        	fepmanager = GetFSQUiL(aLanguage);
        	}
            break;
        default:
            return NULL;
        }
    
    for(i = 0; i < iUiLayoutImpIdList.Count(); ++i)
        {
        if(iUiLayoutImpIdList[i] == layoutId.iUid )
            {
            break;
            }
        }

    if( i < iUiLayoutImpIdList.Count() )
        {
        iPenInputMode = aMode;
        TInt errCode = iPeninputServer.SetUiLayoutId(layoutId);
        if( errCode == KErrNone )
            {
            fepmanager->OnInit();
            return fepmanager;
            }
        }
        
    return NULL;
    }

MAknFepManagerInterface* CPeninputPluginKr::GetInputMethodUiL(
        MAknFepManagerUIInterface* /*aFepManager*/,
        TInt /*aLayoutId*/,
        const TDesC8& /*aData*/)
    {
    LOG("FEP.IMEPLG.KR.GetInputMethodUiL");
    MAknFepManagerInterface* ret(NULL);
    return ret;    
    }

void CPeninputPluginKr::Activate()
    {
    LOG("FEP.IMEPLG.KR.Activate");
    iPeninputServer.ActivateLayout(ETrue);        
    }

void CPeninputPluginKr::DeActivate()
    {
    LOG("FEP.IMEPLG.KR.DeActivate");
    if (&iPeninputServer && iPeninputServer.IsVisible())        
        {
        iPeninputServer.ActivateLayout(EFalse);
        }
    }

TInt CPeninputPluginKr::ImeImplId()
    {
    LOG("FEP.IMEPLG.KR.ImeImplId");
    return KAknFepImePluginImplementationId;
    }

TInt CPeninputPluginKr::LayoutUiImplId()
    {
    LOG("FEP.IMEPLG.KR.LayoutUiImplId");
    TInt id;
    switch ( iPenInputMode )
        {
        case EPluginInputModeVkb:
            {
            id = KVkbUiId;
            }
            break;
        case EPluginInputModeFSQ:
            {
            id = KFsqUiId;
            }
            break;
        case EPluginInputModeItut:
        	{
        	id = KItutUiId;
        	}
        	
        	break;
        default:
            {
            id = KInvalidImplId;
            }
            break;
        }
    return id;
    }

TBool CPeninputPluginKr::HandleServerEventL(
        TInt aEventId, 
        const TDesC& aData)
    {
    LOG1("FEP.IMEPLG.KR.HandleServerEventL %d",aEventId);
    TBool bHandled = EFalse;
    switch ( aEventId )
        {
        case ESignalLayoutICFLengthChanged:
            {
            GetCurrentUi()->RetrieveEditorMaxLength();
            }
            break;
        case ESignalCaseMode:
            {
            TInt data = *(TInt*)( aData.Ptr() );
            if (data>=ECaseInvalide)
                {
                GetCurrentUi()->UpdateCaseMode( data );                 
                }
            }
            break;
        default:
            break;
        }
        
    return bHandled;
    }

TInt CPeninputPluginKr::SupportModes(
        CPtiEngine* aPtiEngine, 
        RArray<TImePlguinImplDetail>& aSupportList) const
    {
    LOG("FEP.IMEPLG.KR.SupportModes");
    TRAPD( ret, SupportModesL( aPtiEngine, aSupportList ) );
    return ret;
    }

TInt CPeninputPluginKr::CurrentMode() const
    {
    LOG("FEP.IMEPLG.KR.CurrentMode");
    return iPenInputMode;
    }

TBool CPeninputPluginKr::HandleMenuCommandL(TInt aCommandId)
    {
    LOG("FEP.IMEPLG.KR.HandleMenuCommandL");
    TInt ret(EFalse);
    if (aCommandId==EPeninputPluginKrDoubleConsonentCmd)
        {
        TInt settingValue;
        iRepository->Get(KAknFepKoreanCombineWordFlag,settingValue);
        settingValue |= EKoreanWordCombineConsonantOnSetting;
        iRepository->Set(EKoreanWordCombineConsonantOnSetting,settingValue);
        TRAP_IGNORE(ShowListQueryL(R_AKNEXQUERY_MULTI_SELECTION_LIST_QUERY));
        iRepository->Get(KAknFepKoreanCombineWordFlag,settingValue);
        settingValue &= 0xFFFFFEFF;
        iRepository->Set(EKoreanWordCombineConsonantOnSetting,settingValue);
        ret=ETrue;
        }
    return ret;
    }

void CPeninputPluginKr::DynInitMenuPaneL(
        CAknFepUiInterfaceMenuPane* aMenuPane)
    {
    LOG("FEP.IMEPLG.KR.DynInitMenuPaneL");
    TInt index;
    /*
    if ((iPenInputMode == EPluginInputModeVkb ||
        iPenInputMode == EPluginInputModeFSQ) &&
        aMenuPane->MenuItemExists(EPenInputCmdSetting, index))
        {
        aMenuPane->SetItemDimmed(EPenInputCmdSetting, EFalse);   
        }
    aMenuPane->SetItemDimmed(EPenInputCmdHwrTraining, ETrue);
    */           
    if (
        ( iLanguage == ELangKorean ) &&
        (iPenInputMode == EPluginInputModeVkb ||
        iPenInputMode == EPluginInputModeFSQ))
        {
        aMenuPane->MenuItemExists(EFepInputCmdHelp, index);
        if (index!=KErrNotFound)
            {
            TBuf<255> menuStr;
            StringLoader::Load(menuStr, R_PENINPUT_PLUGIN_KR_DOUBLECONSONENT_SETTING);
            CAknFepUiInterfaceMenuPane::SItemData data;
            data.iCommandId=EPeninputPluginKrDoubleConsonentCmd;
            data.iCascadeId=0;
            data.iFlags=0;
            data.iText=menuStr.Mid(1, menuStr.Length() - 1);
            aMenuPane->InsertMenuItemL(data,index);
            }
        }
    }

// ---------------------------------------------------------------------------
// Internal Utilities
// ---------------------------------------------------------------------------
//

void CPeninputPluginKr::FindUiLayoutImplementationL()
    {
    RImplInfoPtrArray infoArray;
    TUid id;
    id.iUid = KHwrLayoutInterfaceId;
    
    iUiLayoutImpIdList.Reset();
    CleanupStack::PushL( TCleanupItem( Cleanup, &infoArray ) );
    REComSession::ListImplementationsL(id, infoArray);

    LOG1("FEP.IMEPLG.KR.FindUiLayoutImplementationL %d",infoArray.Count());
    for (TInt i = 0; i < infoArray.Count(); ++i)
        {
        LOG1("FEP.IMEPLG.KR.  0x%08X",infoArray[i]->ImplementationUid().iUid);
#ifdef _DEBUG
        TPtrC ptr(infoArray[i]->DisplayName());
        LOG1("FEP.IMEPLG.KR.  %S",&ptr);
#endif
        iUiLayoutImpIdList.AppendL(infoArray[i]->ImplementationUid().iUid);
        
        }

    CleanupStack::PopAndDestroy(&infoArray); // infoArray    
    }

CPluginKrFepManagerBase* CPeninputPluginKr::GetPluginUiL( TInt /*aMode*/ )
    {
    return NULL;
    }

TInt CPeninputPluginKr::SupportModesL(
        CPtiEngine* /*aPtiEngine*/,
        RArray<TImePlguinImplDetail>& aSupportList) const
    {
    RImplInfoPtrArray infoArray;
    TUid id;
    id.iUid = KHwrLayoutInterfaceId;
    //TBool hwr = EFalse;
    TBool vkb = EFalse;
    TBool itut = EFalse;
    //TBool miniitut = EFalse;
    TBool fsq = EFalse;
    //TBool fsqEng = EFalse;

    CleanupStack::PushL( TCleanupItem( Cleanup, &infoArray ) );
    REComSession::ListImplementationsL(id, infoArray);

    for (TInt i = 0; i < infoArray.Count(); ++i)
        {
        if(infoArray[i]->ImplementationUid().iUid == KVkbUiId 
        		&& FeatureManager::FeatureSupported(KFeatureIdVirtualKeyboardInput))
            {
            vkb = ETrue;
            }
        /*    
        if(infoArray[i]->ImplementationUid().iUid == KItutUiId )
            {
            itut = ETrue;
            }*/
        if(infoArray[i]->ImplementationUid().iUid == KFsqUiId
        		&& FeatureManager::FeatureSupported(KFeatureIdVirtualFullscrQwertyInput))
            {
            fsq = ETrue;
            }
        /*if(infoArray[i]->ImplementationUid().iUid == KFsqUiEngId )
            {
            fsqEng = ETrue;
            }*/
        }

    TImePlguinImplDetail detail;

    detail.iImplementationId = KAknFepImePluginImplementationId;

    if( vkb )
        {
        detail.iMode = EPluginInputModeVkb;
        detail.iMeritValue = EImeMerit_Preferred;
        detail.iDisplayName.Copy(KVkbImeName());
        detail.iLanguage = ELangKorean;
        aSupportList.Append(detail);
        }

    if( itut )
        {
        detail.iMode = EPluginInputModeItut;
        detail.iMeritValue = EImeMerit_Preferred;
        detail.iDisplayName.Copy(KItutImeName());
        detail.iLanguage = ELangKorean;
        aSupportList.Append(detail);
        }

    if( fsq )
        {
        detail.iMode = EPluginInputModeFSQ;
        detail.iMeritValue = EImeMerit_Preferred;
        detail.iDisplayName.Copy(KFsqImeName());
        detail.iLanguage = ELangKorean;
        aSupportList.Append(detail);
        }
     /*if( fsqEng )
        {
        detail.iMode = EPluginInputModeFSQ;
        detail.iMeritValue = EImeMerit_Preferred;
        detail.iDisplayName.Copy(KFsqImeNameEng());
        detail.iLanguage = ELangEnglish;
        aSupportList.Append(detail);
        }*/

    CleanupStack::PopAndDestroy(&infoArray); // infoArray

    return 0;
    }

inline CPluginKrFepManagerBase* CPeninputPluginKr::GetCurrentUi()
    {
    switch ( iPenInputMode )
        {
        case EPluginInputModeVkb:
            return iAllUiManagers[KVkbUiIndex];
        case EPluginInputModeItut:
            return iAllUiManagers[KItutUiIndex];
        case EPluginInputModeFSQ:
        	{
        	/*if(iLanguage == ELangEnglish)
        		{
				return iAllUiManagers[KFSQUiEngIndex];
        		}
        	else*/
        		{
        		return iAllUiManagers[KFSQUiIndex];	
        		}
        	}
        default:
            return NULL;
        }
    }


// -----------------------------------------------------------------------------
// CPeninputPluginKr::GetVkbUi
// Get VKB UI interface.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPluginKrFepManagerBase* CPeninputPluginKr::GetVkbUiL()
    {
    if( !iAllUiManagers[KVkbUiIndex] )
        {
        iAllUiManagers[KVkbUiIndex] = CPluginKrFepManagerVkb::NewL(*this, iPeninputServer);
        }

    return iAllUiManagers[KVkbUiIndex];
    }

// -----------------------------------------------------------------------------
// CPeninputPluginKr::GetItutUi
// Get Itut UI interface.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPluginKrFepManagerBase* CPeninputPluginKr::GetItutUiL()
    {
    if( iAllUiManagers[KItutUiIndex] == NULL )
        {
        iAllUiManagers[KItutUiIndex] = CPluginKrFepManagerItut::NewL(*this, iPeninputServer);
        }

    return iAllUiManagers[KItutUiIndex];
    }

// -----------------------------------------------------------------------------
// CPeninputPluginKr::GetFSQUi
// Get FSQ UI interface.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPluginKrFepManagerBase* CPeninputPluginKr::GetFSQUiL(TInt aLang)
    {
    if( iAllUiManagers[KFSQUiIndex] == NULL )
        {
        iAllUiManagers[KFSQUiIndex] = CPluginKrFepManagerVkb::NewL(*this, iPeninputServer);
        }
        
   /* if( iAllUiManagers[KFSQUiEngIndex] == NULL )
        {
        iAllUiManagers[KFSQUiEngIndex] = CPluginKrFepManagerVkb::NewL(*this, iPeninputServer);
        }*/
	
	/*if (aLang == ELangEnglish)
		{
		return iAllUiManagers[KFSQUiEngIndex];
		}
	else*/
		{
		return iAllUiManagers[KFSQUiIndex];
		}
    }

void CPeninputPluginKr::ShowListQueryL(const TInt aResourceId)
    {
    CArrayFixFlat<TInt>* arraySelected=
        new (ELeave) CArrayFixFlat<TInt>(7);
    CleanupStack::PushL(arraySelected);
    CAknListQueryDialog* dlg=
        new (ELeave) CAknListQueryDialog(arraySelected);
    dlg->PrepareLC(aResourceId);
    

    TInt settingValue;

    if( iRepository->Get( KAknFepKoreanCombineWordFlag, settingValue ) == KErrNone )
        {
        if( settingValue & EKoreanWordCombineConsonantSsangKiyeok )
            {
            arraySelected->AppendL(0);
            }


        if( settingValue & EKoreanWordCombineConsonantSsangTikeut )
            {
            arraySelected->AppendL(1);
            }


        if( settingValue & EKoreanWordCombineConsonantSsangPieup )
            {
            arraySelected->AppendL(2);
            }


        if( settingValue & EKoreanWordCombineConsonantSsangSios )
            {
            arraySelected->AppendL(3);
            }


        if( settingValue & EKoreanWordCombineConsonantCieuc )
            {
            arraySelected->AppendL(4);
            }


        if( settingValue & EKoreanWordCombineConsonantYaPlusI )
            {
            arraySelected->AppendL(5);
            }


        if( settingValue & EKoreanWordCombineConsonantYuPlusI )
            {
            arraySelected->AppendL(6);
            }
        }
    dlg->ListBox()->SetSelectionIndexesL(arraySelected);
    
    if (dlg->RunLD())
        {

        settingValue &= 0xFFFFFF01; 
        iRepository->Set( KAknFepKoreanCombineWordFlag, settingValue );
        for (TInt i=0;i<arraySelected->Count();i++)
            {
            switch (arraySelected->At(i))
                {
                case 0:
                    settingValue |= EKoreanWordCombineConsonantSsangKiyeok; 
                    break;
                case 1:
                    settingValue |= EKoreanWordCombineConsonantSsangTikeut; 
                    break;
                case 2:
                    settingValue |= EKoreanWordCombineConsonantSsangPieup;
                    break;
                case 3:
                    settingValue |= EKoreanWordCombineConsonantSsangSios;
                    break;
                case 4:
                    settingValue |= EKoreanWordCombineConsonantCieuc;
                    break;
                case 5:
                    settingValue |= EKoreanWordCombineConsonantYaPlusI;
                    break;
                case 6:
                    settingValue |= EKoreanWordCombineConsonantYuPlusI;
                    break;
                default:
                    break;
                }
            }
        }

    iRepository->Set( KAknFepKoreanCombineWordFlag, settingValue );
    CleanupStack::PopAndDestroy(arraySelected);
    }

//End Of File
