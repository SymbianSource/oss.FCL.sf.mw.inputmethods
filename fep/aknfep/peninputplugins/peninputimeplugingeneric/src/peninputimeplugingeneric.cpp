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
#include <coemain.h>
#include <aknfepuiinterface.h>
#include <aknfepuimenus.h>
#include <languages.hrh>
#include <bldvariant.hrh>
#include <featmgr.h>

// User includes
#include "peninputimeplugingeneric.h"
#include "peninputimeplugingenericdebug.h"
#include "pluginfepmanagerhwr.h"
#include "pluginfepmanagervkb.h"
#include "truisupportedlanguageid.h"

const TInt KInvalidImplId = 0;

_LIT(KHwrImeName, "Generic HWR");
_LIT(KVkbImeName, "Generic VKB");
_LIT(KFSQImeName, "Generic FSQ");
_LIT(KPrtFSQImeName, "Portrait QWERTY");
_LIT(KSSQImeName, "Split View Qwerty");
_LIT(KFingerHwrImeName, "Generic Fingerhwr");
_LIT(KHwrResourceFormatPattern, "peninputhwrwindowconfiginfo_*.rsc");
_LIT(KVkbResourceFormatPattern, "peninputvkbwindowconfiginfo_*.rsc");
_LIT(KSsqResourceFormatPattern, "peninputssqwinconfiginfo_*.rsc");

_LIT(KPluginResourcePath, "z:\\RESOURCE\\Plugins\\");

class TLangMeritPair 
    {
public:
    TInt iLangStart;
    TInt iLangEnd;
    TInt iMerit;
    };
    
const TLangMeritPair KSupportLanguages[] = 
    {
        {ELangEnglish, ELangAmerican, CAknFepPenInputImePlugin::EImeMerit_Preferred}, //01-10
        {ELangPortuguese, ELangDutch, CAknFepPenInputImePlugin::EImeMerit_Preferred},//13-18
        {ELangCzech, ELangSlovenian, CAknFepPenInputImePlugin::EImeMerit_Preferred}, //25-28
        {ELangThai, ELangThai, CAknFepPenInputImePlugin::EImeMerit_Preferred},//33
        {ELangArabic, ELangArabic, CAknFepPenInputImePlugin::EImeMerit_Preferred},//37
        {ELangTagalog, ELangTagalog, CAknFepPenInputImePlugin::EImeMerit_Preferred},//39
        {ELangBulgarian, ELangBulgarian, CAknFepPenInputImePlugin::EImeMerit_Preferred},//42
        {ELangCatalan, ELangCatalan, CAknFepPenInputImePlugin::EImeMerit_Preferred},//44
        {ELangCroatian, ELangCroatian, CAknFepPenInputImePlugin::EImeMerit_Preferred},//45
        {ELangEstonian, ELangCanadianFrench, CAknFepPenInputImePlugin::EImeMerit_Preferred},//49-51
        {ELangGreek, ELangGreek, CAknFepPenInputImePlugin::EImeMerit_Preferred},//54
        {ELangHebrew, ELangIndonesian, CAknFepPenInputImePlugin::EImeMerit_Preferred},//57-59
        {ELangLatvian, ELangLithuanian, CAknFepPenInputImePlugin::EImeMerit_Preferred},//67 -68
        {ELangMalay, ELangMalay, CAknFepPenInputImePlugin::EImeMerit_Preferred},//70
#ifdef RD_MARATHI    
        {ELangMarathi, ELangMarathi, CAknFepPenInputImePlugin::EImeMerit_Preferred},//72
#endif        
        {ELangBrazilianPortuguese, ELangBrazilianPortuguese, CAknFepPenInputImePlugin::EImeMerit_Preferred},//76
        {ELangRomanian, ELangSerbian, CAknFepPenInputImePlugin::EImeMerit_Preferred},//78 -79
        {ELangLatinAmericanSpanish, ELangLatinAmericanSpanish, CAknFepPenInputImePlugin::EImeMerit_Preferred},//83
        {ELangUkrainian, ELangUrdu, CAknFepPenInputImePlugin::EImeMerit_Preferred},//93-94
        {ELangVietnamese, ELangVietnamese, CAknFepPenInputImePlugin::EImeMerit_Preferred},//96
        {KLangBasque, KLangGalician, CAknFepPenInputImePlugin::EImeMerit_Preferred},//401,402
        {ELangKorean, ELangKorean, CAknFepPenInputImePlugin::EImeMerit_Preferred} // 65 tp teleca add-on
    };
    
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

//lint -esym( 960, 58, * ) lint notes: break used outside of a switch

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CPenInputImePluginGeneric::NewL
// Factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPenInputImePluginGeneric* CPenInputImePluginGeneric::NewL(TAny* aInitParams)
    {
    CPenInputImePluginGeneric* self = new ( ELeave ) CPenInputImePluginGeneric(
                                        static_cast<RPeninputServer*>( 
                                            aInitParams ) );
    CleanupStack::PushL( self );
    self->ConstructL(); 
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginGeneric::NewLC
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPenInputImePluginGeneric* CPenInputImePluginGeneric::NewLC( 
    TAny* aInitParams )
    {
    CPenInputImePluginGeneric* self = new ( ELeave ) CPenInputImePluginGeneric(
                                        static_cast<RPeninputServer*>( 
                                            aInitParams ) );
    CleanupStack::PushL( self );
    self->ConstructL(); 
    return self;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginGeneric::~CPenInputImePluginGeneric
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPenInputImePluginGeneric::~CPenInputImePluginGeneric()
    {
    DeActivate();
    iUiLayoutImpIdList.Close();
    delete iPluginUiManager;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginGeneric::GetInputMethodUiL
// Create layout UI interface
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MAknFepManagerInterface* CPenInputImePluginGeneric::GetInputMethodUiL( 
                                        MAknFepManagerUIInterface* aFepManager,
                                        TLanguage aLanguage, 
                                        TInt aMode,
                                        const TDesC8& /*aData*/,
                                        const TBool aIsSplitView)
    {
    iFepManager = aFepManager;
    iLanguage = aLanguage;
    CPluginFepManagerBase* fepmanager = NULL;
    
    TUid layoutId;
    TInt i;

    switch ( aMode )
        {
        case EPluginInputModeHwr:
            {
            if(aLanguage == ELangArabic)
                {
                layoutId.iUid = KHwrArabicUiId;                
                }
            else
                {
                layoutId.iUid = KHwrUiId;
                }
            }
            break; 
        case EPluginInputModeVkb:
            {
            layoutId.iUid = KVkbUiId;
            }
            break;
        case EPluginInputModeFSQ:
            {
            if(aIsSplitView)
                {
                layoutId.iUid = KSsqUiId;
                }
            else
                {
                layoutId.iUid = KFsqUiId;
                }
            }
            break;
        case EPluginInputModePortraitFSQ:
            {
            // UI layout for portrait FSQ.
            // Note: Don't need to check whether portrait QWERTY feature is 
            // enabled.  Because otherwise, aMode can not have 
            // EPluginInputModePortraitFSQ.
            layoutId.iUid = KPrtFsqUiId;
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
    
    for ( i = 0; i < iUiLayoutImpIdList.Count(); ++i )
        {
        if( iUiLayoutImpIdList[i] == layoutId.iUid )
            {
            break;
            }
        }

    if ( i < iUiLayoutImpIdList.Count() )
        {
        iPenInputMode = aMode;
        TPtrC8 ptr( (TUint8*)&aMode );
        
        TInt errCode;
       	errCode = iPenInputServer->SetUiLayoutId( layoutId );	
        	
        if ( errCode == KErrNone )
            {
            fepmanager->OnInit();
            return fepmanager;
            }
        }
        
    return NULL;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginGeneric::GetInputMethodUiL
// Create layout UI interface
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MAknFepManagerInterface* CPenInputImePluginGeneric::GetInputMethodUiL( 
                                    MAknFepManagerUIInterface* aFepManager,
                                    TInt aLayoutId,
                                    const TDesC8& aData )
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
    else if( aLayoutId == KFsqUiId )
        {
        mode = EPluginInputModeFSQ;
        }
    // If full screen portrait QWERTY feature is enabled, portrait QWERTY UI 
    // layout is used.
    else if( aLayoutId == KPrtFsqUiId ) 
        {
        mode = EPluginInputModePortraitFSQ;
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
        iPenInputServer->SetUiLayoutId( TUid::Uid( aLayoutId ), aData );
        fepmanager->OnInit();    
        return fepmanager;
        }
        
    return NULL;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginGeneric::Activate
// Activate current IME plugin.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPenInputImePluginGeneric::Activate()
    {
    if(iPenInputServer)
        {
        iPenInputServer->ActivateLayout(ETrue);        
        }
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginGeneric::DeActivate
// Deactivate current IME plugin.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPenInputImePluginGeneric::DeActivate()
    {
    if (iPenInputServer && iPenInputServer->IsVisible())        
        {
        iPenInputServer->ActivateLayout(EFalse);
        }
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginGeneric::ImeImplId
// Get current IME plugin implementation ID.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPenInputImePluginGeneric::ImeImplId()
    {
    return KInputMethodImplementationId;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginGeneric::LayoutUiImplId
// Get current layout UI implementation id.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPenInputImePluginGeneric::LayoutUiImplId()
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
        case EPluginInputModeFSQ:
            {
            id = KFsqUiId;
            }
            break;
        case EPluginInputModePortraitFSQ:
            {
            id = KPrtFsqUiId;
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
// CPenInputImePluginGeneric::SupportModes
// Get suppored layout UI modes(VKB/HWR).
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPenInputImePluginGeneric::SupportModes(CPtiEngine* aPtiEngine,
                                             RArray<TImePlguinImplDetail>& aSupportList) const
    {
    TInt ret = 0;
    TRAP_IGNORE( ret = SupportModesL( aPtiEngine, aSupportList ) );
    return ret;
    }
    
// -----------------------------------------------------------------------------
// CPenInputImePluginGeneric::SupportModesL
// Get suppored layout UI modes(VKB/HWR).
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool IsFileInDir( CDir* aDir, const TDesC& aFileName )
    {
    if( aDir )
        {
        for( TInt k = 0; k < aDir->Count(); ++k )
            {
            if( aFileName.CompareF( (*aDir)[k].iName ) == 0 )
                {
                return ETrue;
                }
            }
        }
    return EFalse;
    }
  
inline void HwrResNameFromLangId( TFileName& aFileName,  TInt aLang )    
    {
    if( aLang < 10 )    
        {
        _LIT(KHwrResourceFormat1, "peninputhwrwindowconfiginfo_%02d.rsc"); 
        aFileName.Format(KHwrResourceFormat1, aLang);               
        }
    else
        {
        _LIT(KHwrResourceFormat, "peninputhwrwindowconfiginfo_%d.rsc"); 
        aFileName.Format(KHwrResourceFormat, aLang);               
        }        
    }
    
inline void VkbResNameFromLangId( TFileName& aFileName,  TInt aLang )    
    {
    if( aLang < 10 )    
        {
        _LIT(KVkbResourceFormat1, "peninputvkbwindowconfiginfo_%02d.rsc");
        aFileName.Format(KVkbResourceFormat1, aLang);               
        }
    else
        {
        _LIT(KVkbResourceFormat, "peninputvkbwindowconfiginfo_%d.rsc");
        aFileName.Format(KVkbResourceFormat, aLang);               
        }        
    }
    
inline void SsqResNameFromLangId( TFileName& aFileName,  TInt aLang )    
    {
    if( aLang < 10 )    
        {
        _LIT(KSsqResourceFormat1, "peninputssqwinconfiginfo%02d.rsc");
        aFileName.Format(KSsqResourceFormat1, aLang);               
        }
    else
        {
        _LIT(KSsqResourceFormat, "peninputssqwinconfiginfo%d.rsc");
        aFileName.Format(KSsqResourceFormat, aLang);               
        }        
    }
    
TInt CPenInputImePluginGeneric::SupportModesL(CPtiEngine* /*aPtiEngine*/,
                                             RArray<TImePlguinImplDetail>& aSupportList) const    
    {
    RImplInfoPtrArray infoArray;
    TUid id;
    id.iUid = KHwrLayoutInterfaceId;
    TBool hwr = EFalse;
    TBool vkb = EFalse;
    TBool fsq = EFalse;
    TBool prtfsq = EFalse;
    TBool ssq = EFalse;
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
            
        if (infoArray[i]->ImplementationUid().iUid == KFsqUiId
                && FeatureManager::FeatureSupported(KFeatureIdVirtualFullscrQwertyInput))
            {
            fsq = ETrue;
            }
            
        // If full screen portrait QWERTY feature is enabled, set the mode is enabled.
        if ( infoArray[i]->ImplementationUid().iUid == KPrtFsqUiId
            && FeatureManager::FeatureSupported( 
                KFeatureIdFfVirtualFullscrPortraitQwertyInput ) )
            {
            prtfsq = ETrue;
            }
            
        if (infoArray[i]->ImplementationUid().iUid == KSsqUiId
                && FeatureManager::FeatureSupported(KFeatureIdVirtualFullscrQwertyInput))
            {
            ssq = ETrue;
            } 
        if (infoArray[i]->ImplementationUid().iUid == KFingerHwrUiId )
            {
            fingerhwr = ETrue;
            }          
        }
    
    TImePlguinImplDetail detail;
    
    detail.iImplementationId = KInputMethodImplementationId;
    if( hwr )
        {
        detail.iMode = EPluginInputModeHwr;
        detail.iDisplayName.Copy(KHwrImeName());      
      	TFindFile fileFinder(CCoeEnv::Static()->FsSession());
      	TFileName resName;
      	CDir* dir = NULL;
      	TInt result = fileFinder.FindWildByDir( KHwrResourceFormatPattern, 
                                                KPluginResourcePath, dir );
      	if( result == KErrNone )
      	    {
            for(TInt i = 0; i < sizeof(KSupportLanguages)/sizeof(TLangMeritPair); ++i )
                {
                for(TInt j =  KSupportLanguages[i].iLangStart; 
                    j <=  KSupportLanguages[i].iLangEnd; ++j)
                    {
                    HwrResNameFromLangId( resName, j);
                    if( IsFileInDir( dir, resName ) )
                        {
                        detail.iLanguage = j;
                        detail.iMeritValue = KSupportLanguages[i].iMerit;
                        aSupportList.Append(detail);
                        }
                    }
                }
                
      	    }
  	    delete dir;
        }

    if( vkb || fsq || prtfsq )
        {
      	TFindFile fileFinder(CCoeEnv::Static()->FsSession());
      	TFileName resName;
      	CDir* dir = NULL;
      	TInt result = fileFinder.FindWildByDir( KVkbResourceFormatPattern, 
                                                KPluginResourcePath, dir );
      	
      	if( vkb )
      	    {
            detail.iMode = EPluginInputModeVkb;
            detail.iDisplayName.Copy(KVkbImeName());      
            for(TInt i = 0; i < sizeof(KSupportLanguages)/sizeof(TLangMeritPair); ++i )
                {
                for(TInt j =  KSupportLanguages[i].iLangStart; 
                    j <=  KSupportLanguages[i].iLangEnd; ++j)
                    {
                    VkbResNameFromLangId( resName, j);
                    if( IsFileInDir( dir, resName ) )
                        {
                        detail.iLanguage = j;
                        detail.iMeritValue = KSupportLanguages[i].iMerit;
                        aSupportList.Append(detail);
                        }
                    }
                }
      	    }
        
        if( fsq )
            {
            detail.iMode = EPluginInputModeFSQ;
            detail.iDisplayName.Copy(KFSQImeName());      
            for(TInt i = 0; i < sizeof(KSupportLanguages)/sizeof(TLangMeritPair); ++i )
                {
                for(TInt j =  KSupportLanguages[i].iLangStart; 
                    j <=  KSupportLanguages[i].iLangEnd; ++j)
                    {
                    VkbResNameFromLangId( resName, j);
                    
                    if( IsFileInDir( dir, resName ) )
                        {
                        detail.iLanguage = j;
                        detail.iMeritValue = KSupportLanguages[i].iMerit;
                        aSupportList.Append(detail);
                        }
                    }
                }
            }

        // Add portrait full qwerty as the supported mode.
        // Note: Feature flag check is not needed here because prtfsq should be
        // EFalse if the feature flag is not enabled.
        // Note: We share the same layout for portrait FSQ with landscape FSQ.
        if( prtfsq )
            {
            detail.iMode = EPluginInputModePortraitFSQ;
            // Display name is "Portrait QWERTY".
            detail.iDisplayName.Copy( KPrtFSQImeName() );
            // Add all the support languages.  They are defined in 10282358.rss.
            for ( TInt i = 0; i < sizeof( KSupportLanguages ) / sizeof( TLangMeritPair ); ++i )
                {
                for ( TInt j =  KSupportLanguages[i].iLangStart; 
                    j <=  KSupportLanguages[i].iLangEnd; ++j )
                    {
                    VkbResNameFromLangId( resName, j );

                    if( IsFileInDir( dir, resName ) )
                        {
                        detail.iLanguage = j;
                        detail.iMeritValue = KSupportLanguages[i].iMerit;
                        aSupportList.Append(detail);
                        }
                    }
                }
            }
            
        delete dir;
        }
        
    if( ssq )
        {
        detail.iMode = EPluginInputModeFSQ;
        detail.iDisplayName.Copy(KSSQImeName());      
      	TFindFile fileFinder(CCoeEnv::Static()->FsSession());
      	TFileName resName;
      	CDir* dir = NULL;
      	TInt result = fileFinder.FindWildByDir( KSsqResourceFormatPattern, 
                                                KPluginResourcePath, dir );
      
        for(TInt i = 0; i < sizeof(KSupportLanguages)/sizeof(TLangMeritPair); ++i )
            {
            for(TInt j =  KSupportLanguages[i].iLangStart; 
                j <=  KSupportLanguages[i].iLangEnd; ++j)
                {
                SsqResNameFromLangId( resName, j);
                if( IsFileInDir( dir, resName ) )
                    {
                    detail.iLanguage = j;
                    detail.iMeritValue = KSupportLanguages[i].iMerit;
                    aSupportList.Append(detail);
                    }
                }
            }
        delete dir;
        }    
    if( fingerhwr )
        {
        detail.iMode = EPluginInputModeFingerHwr;
        detail.iMeritValue = EImeMerit_Preferred;
        detail.iDisplayName.Copy(KFingerHwrImeName());
        detail.iLanguage = ELangArabic;
        aSupportList.Append(detail);
        }

    CleanupStack::PopAndDestroy(&infoArray); // infoArray    
    
    return 0;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginGeneric::CurrentMode
// Get current layout UI mode.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPenInputImePluginGeneric::CurrentMode() const
    {
    return iPenInputMode;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginGeneric::HandleServerEventL
// Handle pen input server event..
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPenInputImePluginGeneric::HandleServerEventL( TInt aEventId, const TDesC& aData )
    {
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
            ASSERT( data >= 0 );
            //lint !e728
            GetCurrentUi()->UpdateCaseMode( data ); 
            }
            break;
        default:
            break;
        }
        
    return bHandled;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginGeneric::HandleMenuCommandL
// Handle the menu command.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPenInputImePluginGeneric::HandleMenuCommandL( TInt /*aCommandId*/ )
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginGeneric::DynInitMenuPaneL
// Dynamiclly update menu items during it displays.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPenInputImePluginGeneric::DynInitMenuPaneL( CAknFepUiInterfaceMenuPane* aMenuPane )
    {
    TInt currentLanguage = iPluginUiManager->InputLanguage();
    TBool isDimTraining = ETrue;
    TInt count = sizeof(KTruiSupportedLanguages)/sizeof(TInt);
    for(TInt i = 0; i < count; i++)
        {
        if(currentLanguage == KTruiSupportedLanguages[i])
            {
            isDimTraining = EFalse;
            break;
            }
        }
    if(isDimTraining)
        {
        aMenuPane->SetItemDimmed(EPenInputCmdHwrTraining, ETrue);
        }
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginGeneric::CPenInputImePluginGeneric
// C++ default constructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPenInputImePluginGeneric::CPenInputImePluginGeneric( RPeninputServer* aHwrServer )
    :iPenInputServer( aHwrServer ) 
    {
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginGeneric::ConstructL
// 2nd phase constructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPenInputImePluginGeneric::ConstructL()
    {
    FindUiLayoutImplementationL();
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginGeneric::FindUiLayoutImplementationL
// Enumuate all layout UI ids.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPenInputImePluginGeneric::FindUiLayoutImplementationL()
    {
    RImplInfoPtrArray infoArray;
    TUid id;
    id.iUid = KHwrLayoutInterfaceId;

    iUiLayoutImpIdList.Reset();
    CleanupStack::PushL( TCleanupItem( Cleanup, &infoArray ) );
    REComSession::ListImplementationsL( id, infoArray );
        
    for ( TInt i = 0; i < infoArray.Count(); ++i )
        {
        iUiLayoutImpIdList.AppendL( infoArray[i]->ImplementationUid().iUid );
        }

    CleanupStack::PopAndDestroy( &infoArray ); // infoArray    
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginGeneric::GetPluginUiL
// Get HWR UI interface.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPluginFepManagerBase* CPenInputImePluginGeneric::GetPluginUiL( TInt aMode )
    {
    delete iPluginUiManager;
    iPluginUiManager = NULL;
    
    switch ( aMode )
        {
        case EPluginInputModeFingerHwr:        
        case EPluginInputModeHwr:
            {
            return iPluginUiManager = CPluginFepManagerHwr::NewL(*this, iPenInputServer);
            }
        case EPluginInputModeVkb:
        case EPluginInputModeFSQ:
        case EPluginInputModePortraitFSQ:
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
