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
* Description:            Input mode plugin implementation file
*
*/












// System includes
#include <ecom.h>
#include <AknFepManagerInterface.h>
#include <eikmenup.h>
#include <avkon.hrh>
#include <aknfepuiinterface.h>
#include <aknfepuimenus.h>
#include <bldvariant.hrh>
#include <featmgr.h>

// User includes
#include "peninputimeplugincn.h"
#include "peninputimeplugincndebug.h"
#include "pluginfepmanagerhwr.h"
#include "pluginfepmanagervkb.h"
#include "aknfepuiinterface.h"



const TInt KInvalidImplId = 0;

_LIT(KFscImeName, "Chinese FSC");
_LIT(KHwrImeName, "Chinese HWR");
_LIT(KVkbImeName, "Chinese VKB");
_LIT(KFsqImeName, "Chinese FSQ");
_LIT(KFingerHwrImeName, "Chinese Fingerhwr");

//lint -esym( 960, 58, * ) lint notes: break used outside of a switch

// Implementation of Class CPenInputImePluginCn 

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

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CPenInputImePluginCn::NewL
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPenInputImePluginCn* CPenInputImePluginCn::NewL(TAny* aInitParams)
    {
    CPenInputImePluginCn* self = new(ELeave)CPenInputImePluginCn(
                                        static_cast<RPeninputServer*>(aInitParams));
    CleanupStack::PushL(self);
    self->ConstructL(); 
    CleanupStack::Pop(self);
    return self;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginCn::NewLC
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPenInputImePluginCn* CPenInputImePluginCn::NewLC(TAny* aInitParams)
    {
    CPenInputImePluginCn* self = new(ELeave) CPenInputImePluginCn(
                                        static_cast<RPeninputServer*>(aInitParams));
    CleanupStack::PushL(self);
    self->ConstructL(); 
    return self;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginCn::~CPenInputImePluginCn
// destructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPenInputImePluginCn::~CPenInputImePluginCn()
    {
    DeActivate();
    iUiLayoutImpIdList.Close();
    delete iPluginUiManager;
    REComSession::FinalClose();//cleanup ecom
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginCn::GetInputMethodUiL
// Create layout UI interface
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MAknFepManagerInterface* CPenInputImePluginCn::GetInputMethodUiL(
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
        case EPluginInputModeFSc:
            {
            layoutId.iUid = KFScUiId;
            }
            break;         
        case EPluginInputModeHwr:
            {
            layoutId.iUid = KHwrUiId;
            }
            break; 
        case EPluginInputModeVkb:
            {
            layoutId.iUid = KVkbUiId;
            }
            break;
        case EPluginInputModeFSQ:
            {
            layoutId.iUid = KFsqUiId;
            }
            break;
        case EPluginInputModeFingerHwr:
            {
            layoutId.iUid = KFingerHwrUiId;
            }
            break;            
        default:
            return NULL;
        }
    fepmanager = GetPluginUiL( aMode );
    
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
// CPenInputImePluginCn::GetInputMethodUiL
// Create layout UI interface
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MAknFepManagerInterface* CPenInputImePluginCn::GetInputMethodUiL(
                                    MAknFepManagerUIInterface* aFepManager,
                                    TInt aLayoutId,
                                    const TDesC8& /*aData*/)
    {
    iFepManager = aFepManager;
    CPluginFepManagerBase* fepmanager = NULL;   
    TInt i;
    TInt mode;    
    
    if ( aLayoutId == KHwrUiId )
        {
        mode = EPluginInputModeHwr;      
        }
    else if( aLayoutId == KVkbUiId )
        {
        mode = EPluginInputModeVkb;
        }
    else if (aLayoutId == KFScUiId)
        {
        mode = EPluginInputModeFSc;
        }
    else if( aLayoutId == KFsqUiId )
        {
        mode = EPluginInputModeFSQ;
        }
    else if( aLayoutId == KFingerHwrUiId )
        {
        mode = EPluginInputModeFingerHwr;
        }
    else
        {
        return NULL;
        }
    fepmanager = GetPluginUiL( mode );

    for ( i = 0; i < iUiLayoutImpIdList.Count(); ++i )
        {
        if ( iUiLayoutImpIdList[i] == aLayoutId )
            {
            break;
            }
        }

    if ( i < iUiLayoutImpIdList.Count() )
        {
        iPenInputMode = mode;
        iPenInputServer->SetUiLayoutId(TUid::Uid(aLayoutId));
        fepmanager->OnInit();    
        return fepmanager;
        }
        
    return NULL;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginCn::Activate
// Activate current IME plugin.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPenInputImePluginCn::Activate()
    {
    if(iPenInputServer)
        {
        iPenInputServer->ActivateLayout(ETrue);        
        }
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginCn::DeActivate
// Deactivate current IME plugin.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPenInputImePluginCn::DeActivate()
    {
    if (iPenInputServer && iPenInputServer->IsVisible())        
        {
        iPenInputServer->ActivateLayout(EFalse);
        }
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginCn::ImeImplId
// Get current IME plugin implementation ID.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPenInputImePluginCn::ImeImplId()
    {
    return KInputMethodImplementationId;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginCn::LayoutUiImplId
// Get current layout UI implementation id.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPenInputImePluginCn::LayoutUiImplId()
    {
    TInt id;
    switch ( iPenInputMode )
        {
        case EPluginInputModeHwr:
            {
            id = KHwrUiId;
            }
            break;
        case EPluginInputModeVkb:
            {
            id = KVkbUiId;
            }
            break;
        case EPluginInputModeFSc:
            {
            id = KFScUiId;
            }
            break; 
        case EPluginInputModeFSQ:
            {
            id = KFsqUiId;
            }
            break;
        case EPluginInputModeFingerHwr:
            {
            id = KFingerHwrUiId;
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


// -----------------------------------------------------------------------------
// CPenInputImePluginCn::SupportModes
// Get suppored layout UI modes(VKB/HWR).
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPenInputImePluginCn::SupportModes(CPtiEngine* aPtiEngine,
                                             RArray<TImePlguinImplDetail>& aSupportList) const
    {
    TInt ret = 0;
    TRAP_IGNORE( ret = SupportModesL( aPtiEngine, aSupportList ) );
    return ret;
    }
    
// -----------------------------------------------------------------------------
// CPenInputImePluginCn::SupportModesL
// Get suppored layout UI modes(VKB/HWR).
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPenInputImePluginCn::SupportModesL(CPtiEngine* /*aPtiEngine*/,
                                             RArray<TImePlguinImplDetail>& aSupportList) const 
    {
    RImplInfoPtrArray infoArray;
    TUid id;
    id.iUid = KHwrLayoutInterfaceId;
    TBool hwr = EFalse;
    TBool vkb = EFalse;
    TBool fsc = EFalse;
    TBool fsq = EFalse;
    TBool fingerhwr = EFalse;
    
    CleanupStack::PushL( TCleanupItem( Cleanup, &infoArray ) );
    REComSession::ListImplementationsL(id, infoArray);
        
    for (TInt i = 0; i < infoArray.Count(); ++i)
        {
        if(infoArray[i]->ImplementationUid().iUid == KHwrUiId )
            {
            hwr = ETrue;
            }

        if (infoArray[i]->ImplementationUid().iUid == KVkbUiId
                && FeatureManager::FeatureSupported(KFeatureIdVirtualKeyboardInput))
            {
            vkb = ETrue;
            }
        if(infoArray[i]->ImplementationUid().iUid == KFScUiId )
            {
            fsc = ETrue;
            }          
        if (infoArray[i]->ImplementationUid().iUid == KFsqUiId
                && FeatureManager::FeatureSupported(KFeatureIdVirtualFullscrQwertyInput))
            {
            fsq = ETrue;
            }
        if (infoArray[i]->ImplementationUid().iUid == KFingerHwrUiId )
            {
            fingerhwr = ETrue;
            }  
        }
    
    TImePlguinImplDetail detail;
    
    detail.iImplementationId = KInputMethodImplementationId;
    if( fsc )
        {
        detail.iMode = EPluginInputModeFSc;
        detail.iMeritValue = EImeMerit_Preferred;
        detail.iDisplayName.Copy(KFscImeName());
        detail.iLanguage = ELangTaiwanChinese;
        aSupportList.Append(detail);
        detail.iLanguage = ELangHongKongChinese;
        aSupportList.Append(detail);
        detail.iLanguage = ELangPrcChinese;
        aSupportList.Append(detail);
        //detail.iLanguage = ELangEnglish;
        //detail.iMeritValue = EImeMerit_NotUsed;
        //aSupportList.Append(detail);
        }
    if( hwr )
        {
        detail.iMode = EPluginInputModeHwr;
        detail.iMeritValue = EImeMerit_Preferred;
        detail.iDisplayName.Copy(KHwrImeName());
        detail.iLanguage = ELangTaiwanChinese;
        aSupportList.Append(detail);
        detail.iLanguage = ELangHongKongChinese;
        aSupportList.Append(detail);
        detail.iLanguage = ELangPrcChinese;
        aSupportList.Append(detail);
        //detail.iLanguage = ELangEnglish;
        //detail.iMeritValue = EImeMerit_NotUsed;
        //aSupportList.Append(detail);
        }

    if( vkb )
        {
        detail.iMode = EPluginInputModeVkb;
        detail.iMeritValue = EImeMerit_Preferred;
        detail.iDisplayName.Copy(KVkbImeName());
        detail.iLanguage = ELangTaiwanChinese;
        aSupportList.Append(detail);
        detail.iLanguage = ELangHongKongChinese;
        aSupportList.Append(detail);
        detail.iLanguage = ELangPrcChinese;
        aSupportList.Append(detail);
        //detail.iLanguage = ELangEnglish;
        //detail.iMeritValue = EImeMerit_NotUsed;
        //aSupportList.Append(detail);
        }
        
    if( fsq )
        {
        detail.iMode = EPluginInputModeFSQ;
        detail.iMeritValue = EImeMerit_Preferred;
        detail.iDisplayName.Copy(KFsqImeName());
        detail.iLanguage = ELangTaiwanChinese;
        aSupportList.Append(detail);
        detail.iLanguage = ELangHongKongChinese;
        aSupportList.Append(detail);
        detail.iLanguage = ELangPrcChinese;
        aSupportList.Append(detail);
        //detail.iLanguage = ELangEnglish;
        //detail.iMeritValue = EImeMerit_NotUsed;
        //aSupportList.Append(detail);
        }
    if( fingerhwr )
        {
        detail.iMode = EPluginInputModeFingerHwr;
        detail.iMeritValue = EImeMerit_Preferred;
        detail.iDisplayName.Copy(KFingerHwrImeName());
        detail.iLanguage = ELangTaiwanChinese;
        aSupportList.Append(detail);
        detail.iLanguage = ELangHongKongChinese;
        aSupportList.Append(detail);
        detail.iLanguage = ELangPrcChinese;
        aSupportList.Append(detail);
        }
        
    CleanupStack::PopAndDestroy(&infoArray); // infoArray    
    
    return 0;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginCn::CurrentMode
// Get current layout UI mode.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPenInputImePluginCn::CurrentMode() const
    {
    return iPenInputMode;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginCn::HandleServerEventL
// Handle pen input server event..
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPenInputImePluginCn::HandleServerEventL(TInt aEventId, const TDesC& aData)
    {
    TBool bHandled = EFalse;
    switch( aEventId )
        {
        case ESignalLayoutICFLengthChanged:
            {
            GetCurrentUi()->RetrieveEditorMaxLength();
            }
            break;
        case ESignalCaseMode:
            {
            GetCurrentUi()->UpdateCaseMode(*(TInt*)(aData.Ptr())); //lint !e728
            }
            break;
        default:
            break;
        }
        
    return bHandled;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginCn::HandleMenuCommandL
// Handle the menu command.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPenInputImePluginCn::HandleMenuCommandL(TInt /*aCommandId*/)
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginCn::DynInitMenuPaneL
// Dynamiclly update menu items during it displays.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPenInputImePluginCn::DynInitMenuPaneL(CAknFepUiInterfaceMenuPane* aMenuPane)
    {
    TInt index;
    
    if ((iPenInputMode == EPluginInputModeVkb ||
        iPenInputMode == EPluginInputModeFSQ) &&
        aMenuPane->MenuItemExists(EPenInputCmdSetting, index))
        {
        aMenuPane->SetItemDimmed(EPenInputCmdSetting, EFalse);   
        }
    aMenuPane->SetItemDimmed(EPenInputCmdHwrTraining, ETrue);			
    }


// -----------------------------------------------------------------------------
// CPenInputImePluginCn::CPenInputImePluginCn
// C++ default constructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPenInputImePluginCn::CPenInputImePluginCn(RPeninputServer* aHwrServer)
:iPenInputServer(aHwrServer) 
    {
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginCn::ConstructL
// 2nd phase constructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPenInputImePluginCn::ConstructL()
    {
    FindUiLayoutImplementationL();
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginCn::FindUiLayoutImplementationL
// Enumuate all layout UI ids.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPenInputImePluginCn::FindUiLayoutImplementationL()
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
// CPenInputImePluginCn::GetPluginUiL
// Get HWR UI interface.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPluginFepManagerBase* CPenInputImePluginCn::GetPluginUiL( TInt aMode )
    {
    delete iPluginUiManager;
    iPluginUiManager = NULL;
    
    switch ( aMode )
        {
        case EPluginInputModeFingerHwr:
        case EPluginInputModeFSc:
        case EPluginInputModeHwr:
            {
            return iPluginUiManager = CPluginFepManagerHwr::NewL(*this, iPenInputServer);
            }
        case EPluginInputModeVkb:
        case EPluginInputModeFSQ:
            {
            return iPluginUiManager = CPluginFepManagerVkb::NewL(*this, iPenInputServer);
            }
        default:
            {
            return NULL;
            }
        }
    }
//End Of File
