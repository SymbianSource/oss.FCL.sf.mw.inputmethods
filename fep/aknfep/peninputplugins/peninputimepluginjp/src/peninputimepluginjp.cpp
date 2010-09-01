/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            Input mode plugin implementation file
*
*/













#include "peninputimepluginjp.h"
#include <ecom.h>
#include <AknFepManagerInterface.h>
#include <eikmenup.h>
#include <avkon.hrh>
#include <aknfepuiinterface.h>
#include <aknfepuimenus.h>
#include <featmgr.h>

#include "peninputimepluginjpdebug.h"
#include "pluginfepmanagerhwr.h"
#include "pluginfepmanagervkb.h"
#include "pluginfepmanagerbase.h"


const TInt KInvalidImplId = 0;

_LIT(KHwrImeName, "Japanese HWR");
_LIT(KVkbImeName, "Japanese VKB");

// Implementation of Class CPenInputImePluginJp

// -----------------------------------------------------------------------------
// RImplInfoPtrArray cleanup function
// -----------------------------------------------------------------------------
//
void Cleanup( TAny* aAny )
    {
    RImplInfoPtrArray* implArray =
        reinterpret_cast< RImplInfoPtrArray*> ( aAny );
    implArray->ResetAndDestroy();
    implArray->Close();
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginJp::NewL
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPenInputImePluginJp* CPenInputImePluginJp::NewL(TAny* aInitParams)
    {
    CPenInputImePluginJp* self = new(ELeave)CPenInputImePluginJp(
                                        static_cast<RPeninputServer*>(aInitParams));
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginJp::NewLC
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPenInputImePluginJp* CPenInputImePluginJp::NewLC(TAny* aInitParams)
    {
    CPenInputImePluginJp* self = new(ELeave) CPenInputImePluginJp(
                                        static_cast<RPeninputServer*>(aInitParams));
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginJp::~CPenInputImePluginJp
// destructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPenInputImePluginJp::~CPenInputImePluginJp()
    {
    DeActivate();
    iUiLayoutImpIdList.Close();
    iAllUiManagers.ResetAndDestroy();
    REComSession::FinalClose();//cleanup ecom
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginJp::GetInputMethodUiL
// Create layout UI interface
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MAknFepManagerInterface* CPenInputImePluginJp::GetInputMethodUiL(
                                        MAknFepManagerUIInterface* aFepManager,
                                        TLanguage aLanguage,
                                        TInt      aMode,
                                        const TDesC8& /*aData*/,
                                        const TBool aIsSplitView)
    {
    iFepManager = aFepManager;
    iLanguage = aLanguage;
    CPluginFepManagerBase* fepmanager = NULL;

    TUid layoutId;
    TInt i;

    switch(aMode)
        {
        case EPluginInputModeHwr:
            layoutId.iUid = KHwrUiId;
            fepmanager = GetHwrUiL();
            break;
        case EPluginInputModeVkb:
            layoutId.iUid = KVkbUiId;
            fepmanager = GetVkbUiL();
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
        TInt errCode = iPenInputServer->SetUiLayoutId(layoutId);
        if( errCode == KErrNone )
            {
            fepmanager->OnInit();
            return fepmanager;
            }
        }

    return NULL;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginJp::GetInputMethodUiL
// Create layout UI interface
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MAknFepManagerInterface* CPenInputImePluginJp::GetInputMethodUiL(
                                    MAknFepManagerUIInterface* aFepManager,
                                    TInt aLayoutId,
                                    const TDesC8& /*aData*/)
    {
    iFepManager = aFepManager;
    CPluginFepManagerBase* fepmanager = NULL;
    TInt i;
    TInt mode;

    if( aLayoutId == KHwrUiId)
        {
        mode = EPluginInputModeHwr;
        fepmanager = GetHwrUiL();
        }
    else if( aLayoutId == KVkbUiId)
        {
        mode = EPluginInputModeVkb;
        fepmanager = GetVkbUiL();
        }
    else
        {
        return NULL;
        }

    for(i = 0; i < iUiLayoutImpIdList.Count(); ++i)
        {
        if(iUiLayoutImpIdList[i] == aLayoutId )
            {
            break;
            }
        }

    if( i < iUiLayoutImpIdList.Count() )
        {
        iPenInputMode = mode;
        iPenInputServer->SetUiLayoutId(TUid::Uid(aLayoutId));
        fepmanager->OnInit();
        return fepmanager;
        }

    return NULL;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginJp::Activate
// Activate current IME plugin.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPenInputImePluginJp::Activate()
    {
    if(iPenInputServer)
        {
        iPenInputServer->ActivateLayout(ETrue);
        }
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginJp::DeActivate
// Deactivate current IME plugin.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPenInputImePluginJp::DeActivate()
    {
    if (iPenInputServer && iPenInputServer->IsVisible())
        {
        iPenInputServer->ActivateLayout(EFalse);
        }
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginJp::ImeImplId
// Get current IME plugin implementation ID.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPenInputImePluginJp::ImeImplId()
    {
    return KInputMethodImplementationId;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginJp::LayoutUiImplId
// Get current layout UI implementation id.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPenInputImePluginJp::LayoutUiImplId()
    {
    TInt id;
    switch(iPenInputMode)
        {
        case EPluginInputModeHwr:
            id = KHwrUiId;
            break;
        case EPluginInputModeVkb:
            id = KVkbUiId;
            break;
        default:
            id = KInvalidImplId;
            break;
        }
    return id;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginJp::SupportModes
// Get suppored layout UI modes(VKB/HWR).
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPenInputImePluginJp::SupportModes(CPtiEngine* aPtiEngine,
                                             RArray<TImePlguinImplDetail>& aSupportList) const
    {
    TInt ret = 0;
    TRAP_IGNORE( ret = SupportModesL( aPtiEngine, aSupportList ) );
    return ret;
    }
    
// -----------------------------------------------------------------------------
// CPenInputImePluginJp::SupportModesL
// Get suppored layout UI modes(VKB/HWR).
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPenInputImePluginJp::SupportModesL(CPtiEngine* /*aPtiEngine*/,
                                             RArray<TImePlguinImplDetail>& aSupportList) const 
    {
    RImplInfoPtrArray infoArray;
    TUid id;
    id.iUid = KHwrLayoutInterfaceId;
    TBool hwr = EFalse;
    TBool vkb = EFalse;

    CleanupStack::PushL( TCleanupItem( Cleanup, &infoArray ) );
    REComSession::ListImplementationsL(id, infoArray);

    for (TInt i = 0; i < infoArray.Count(); ++i)
        {
        if(infoArray[i]->ImplementationUid().iUid == KHwrUiId )
            {
            hwr = ETrue;
            }
        if(infoArray[i]->ImplementationUid().iUid == KVkbUiId
            && FeatureManager::FeatureSupported(KFeatureIdVirtualKeyboardInput))
            {
            vkb = ETrue;
            }
        }

    TImePlguinImplDetail detail;

    detail.iImplementationId = KInputMethodImplementationId;
    if( hwr )
        {
        detail.iMode = EPluginInputModeHwr;
        detail.iMeritValue = EImeMerit_Preferred;
        detail.iDisplayName.Copy(KHwrImeName());
        detail.iLanguage = ELangJapanese;
        aSupportList.Append(detail);
        detail.iLanguage = ELangEnglish;
        detail.iMeritValue = EImeMerit_NotUsed;
        aSupportList.Append(detail);
        }

    if( vkb )
        {
        detail.iMode = EPluginInputModeVkb;
        detail.iMeritValue = EImeMerit_Preferred;
        detail.iDisplayName.Copy(KVkbImeName());
        detail.iLanguage = ELangJapanese;
        aSupportList.Append(detail);
        detail.iLanguage = ELangEnglish;
        detail.iMeritValue = EImeMerit_NotUsed;
        aSupportList.Append(detail);
        }
    CleanupStack::PopAndDestroy(&infoArray); // infoArray

    return 0;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginJp::CurrentMode
// Get current layout UI mode.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPenInputImePluginJp::CurrentMode() const
    {
    return iPenInputMode;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginJp::HandleServerEventL
// Handle pen input server event..
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPenInputImePluginJp::HandleServerEventL(TInt aEventId, const TDesC& aData)
    {
    TBool bHandled = EFalse;
    switch( aEventId )
        {
        case ESignalLayoutICFLengthChanged:
            GetCurrentUi()->RetrieveEditorMaxLength();
            break;
        case ESignalCaseMode:
            {
            GetCurrentUi()->UpdateCaseMode(*(TInt*)(aData.Ptr()));
            }
            break;
        default:
            break;
        }

    return bHandled;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginJp::HandleMenuCommandL
// Handle the menu command.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPenInputImePluginJp::HandleMenuCommandL(TInt aCommandId)
    {
    TBool ret = EFalse;
    switch(aCommandId)
        {
        // not handle the following command ids.
        case EPenInputJpCmdFullWidth:
        case EPenInputJpCmdHalfWidth:
            break;
        case EPenInputJpCmdConversionOn:
        case EPenInputJpCmdConversionOff:
            break;
        case EPenInputJpCmdPredictiveOn:
        case EPenInputJpCmdPredictiveOff:
            break;
        case EPenInputJpCmdSaveWord:
        case EPenInputJpCmdKutenCode:
        default:
            break;
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginJp::DynInitMenuPaneL
// Dynamiclly update menu items during it displays.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPenInputImePluginJp::DynInitMenuPaneL(CAknFepUiInterfaceMenuPane* aMenuPane)
    {
    TBool dimmmenu;
    CPluginFepManagerBase* manager = GetCurrentUi();
    if (manager)
        {
        dimmmenu = (manager->IsCharWidth())? ETrue: EFalse;
        aMenuPane->SetItemDimmed(EPenInputJpCmdFullWidth, dimmmenu);
        aMenuPane->SetItemDimmed(EPenInputJpCmdHalfWidth, !dimmmenu);
        if (iPenInputMode == EPluginInputModeHwr)
            {
            dimmmenu = (manager->IsJapaneseHwrConversion())? ETrue: EFalse;
            aMenuPane->SetItemDimmed(EPenInputJpCmdConversionOn, dimmmenu);
            aMenuPane->SetItemDimmed(EPenInputJpCmdConversionOff, !dimmmenu);
            }
        else
            {
            aMenuPane->SetItemDimmed(EPenInputJpCmdConversionOn, ETrue);
            aMenuPane->SetItemDimmed(EPenInputJpCmdConversionOff, ETrue);
            }
        dimmmenu = (manager->IsJapanesePredictive())? ETrue: EFalse;
        aMenuPane->SetItemDimmed(EPenInputJpCmdPredictiveOn, dimmmenu);
        aMenuPane->SetItemDimmed(EPenInputJpCmdPredictiveOff, !dimmmenu);

        aMenuPane->SetItemDimmed(EPenInputJpCmdSaveWord, EFalse);
        aMenuPane->SetItemDimmed(EPenInputJpCmdKutenCode, EFalse);
        }
    aMenuPane->SetItemDimmed(EPenInputCmdHwrTraining, ETrue);
    }


// -----------------------------------------------------------------------------
// CPenInputImePluginJp::CPenInputImePluginJp
// C++ default constructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPenInputImePluginJp::CPenInputImePluginJp(RPeninputServer* aHwrServer)
: iPenInputServer(aHwrServer)
    {
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginJp::ConstructL
// 2nd phase constructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPenInputImePluginJp::ConstructL()
    {
    FindUiLayoutImplementationL();
    //Append NULL to save HWR manager position
    iAllUiManagers.Append(NULL);
    //Append NULL to save VKB manager position
    iAllUiManagers.Append(NULL);
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginJp::FindUiLayoutImplementationL
// Enumuate all layout UI ids.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPenInputImePluginJp::FindUiLayoutImplementationL()
    {
    RImplInfoPtrArray infoArray;
    TUid id;
    id.iUid = KHwrLayoutInterfaceId;

    iUiLayoutImpIdList.Reset();
    CleanupStack::PushL( TCleanupItem( Cleanup, &infoArray ) );
    REComSession::ListImplementationsL(id, infoArray);

    for (TInt i = 0; i < infoArray.Count(); ++i)
        {
        iUiLayoutImpIdList.AppendL(infoArray[i]->ImplementationUid().iUid);
        }

    CleanupStack::PopAndDestroy(&infoArray); // infoArray
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginJp::GetHwrUi
// Get HWR UI interface.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPluginFepManagerBase* CPenInputImePluginJp::GetHwrUiL()
    {
    if( !iAllUiManagers[KHwrUiIndex] )
        {
        iAllUiManagers[KHwrUiIndex] = CPluginFepManagerHwr::NewL(*this, iPenInputServer);
        }

    return iAllUiManagers[KHwrUiIndex];
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginJp::GetVkbUi
// Get VKB UI interface.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPluginFepManagerBase* CPenInputImePluginJp::GetVkbUiL()
    {
    if( !iAllUiManagers[KVkbUiIndex] )
        {
        iAllUiManagers[KVkbUiIndex] = CPluginFepManagerVkb::NewL(*this, iPenInputServer);
        }

    return iAllUiManagers[KVkbUiIndex];
    }

//End Of File
