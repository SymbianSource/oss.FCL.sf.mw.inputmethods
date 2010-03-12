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

// User includes
#include "peninputimepluginitut.h"
#include "peninputimepluginitutdebug.h"
#include "pluginfepmanageritut.h"

const TInt KInvalidImplId = 0;

_LIT(KItutImeName,"vitut Itut");
_LIT(KSplitItutImeName, "split vitut Itut");

class TScriptPair
    {
public:
    const TInt* iData;
    TInt iLen;
    CPenInputImePluginItut::TScriptIndex iScriptIdx;
    };

const TInt LatinLangs[] = 
    {
    65, 1,   2,   3,   4,   5,   6,   7,   8,   9,   10, 
    11,  12,  13,  14,  15,  17,  18,  20,  21,  22, 
    23,  24,  25,  26,  27,  28,  39,  44,  45,  46, 
    47,  48,  49,  51,  59,  67,  68,  70,  76,  78, 
    79,  82,  83,  85,  91,  96,  102, 103, 401, 402
    };
const TInt CyrillicLangs[] = {16, 42, 93,};
const TInt GreekLangs[] = {54, 55,};
const TInt HebrewLangs[] = {57,};
const TInt ArabicLangs[] = {37,};
const TInt FarsiLangs[] = {50,};
const TInt UrduLangs[] = {94,};
const TInt ThaiLangs[] = {33,};
const TInt ChnLangs[] = {29, 30, 31,};
const TInt IndicLangs[] = {58, 72};

_LIT(KPluginResourcePath, "z:\\RESOURCE\\Plugins\\");
_LIT(KResFilePattern, "peninputitutconfig_*.rsc");
_LIT(KResFilePatternSplit, "peninputsplititutconfig_*.rsc");

_LIT(KLatinResFile, "peninputitutconfig_latin.rsc");
_LIT(KLatinResFileSplit, "peninputsplititutconfig_latin.rsc");
_LIT(KCyrillicResFile, "peninputitutconfig_cyrillic.rsc");
_LIT(KCyrillicResFileSplit, "peninputsplititutconfig_cyrillic.rsc");
_LIT(KGreekResFile, "peninputitutconfig_greek.rsc");
_LIT(KGreekResFileSplit, "peninputsplititutconfig_greek.rsc");
_LIT(KHebrewResFile, "peninputitutconfig_hebrew.rsc");
_LIT(KHebrewResFileSplit, "peninputsplititutconfig_hebrew.rsc");
_LIT(KArabicResFile, "peninputitutconfig_arabic.rsc");
_LIT(KArabicResFileSplit, "peninputsplititutconfig_arabic.rsc");
_LIT(KFarsiResFile, "peninputitutconfig_farsi.rsc");
_LIT(KFarsiResFileSplit, "peninputsplititutconfig_farsi.rsc");
_LIT(KUrduResFile, "peninputitutconfig_urdu.rsc");
_LIT(KUrduResFileSplit, "peninputsplititutconfig_urdu.rsc");
_LIT(KThaiResFile, "peninputitutconfig_thai.rsc");
_LIT(KThaiResFileSplit, "peninputsplititutconfig_thai.rsc");
_LIT(KChnResFile, "peninputitutconfig_chinese.rsc");
_LIT(KChnResFileSplit, "peninputsplititutconfig_chinese.rsc");

_LIT(KDevanagiriResFile, "peninputitutconfig_devanagari.rsc");
_LIT(KDevanagiriResFileSplit, "peninputsplititutconfig_devanagari.rsc");
const TScriptPair KSupportScripts[] = 
    {
    {LatinLangs, sizeof(LatinLangs)/sizeof(TInt),
        CPenInputImePluginItut::ELatinLang},
    {CyrillicLangs, 3, CPenInputImePluginItut::ECyrillicLang},
    {GreekLangs, 2, CPenInputImePluginItut::EGreekLang},
    {HebrewLangs, 1, CPenInputImePluginItut::EHebrewLang},
    {ArabicLangs, 1, CPenInputImePluginItut::EArabicLang},
    {FarsiLangs, 1, CPenInputImePluginItut::EFarsiLang},
    {UrduLangs, 1, CPenInputImePluginItut::EUrduLang},
    {ThaiLangs, 1, CPenInputImePluginItut::EThaiLang},
    {ChnLangs, 3, CPenInputImePluginItut::EChnLang},
    {IndicLangs, 2, CPenInputImePluginItut::EIndicLang}
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
// CPenInputImePluginItut::NewL
// Factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPenInputImePluginItut* CPenInputImePluginItut::NewL(TAny* aInitParams)
    {
    CPenInputImePluginItut* self = new ( ELeave ) CPenInputImePluginItut(
                                        static_cast<RPeninputServer*>( 
                                            aInitParams ) );
    CleanupStack::PushL( self );
    self->ConstructL(); 
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginItut::NewLC
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPenInputImePluginItut* CPenInputImePluginItut::NewLC( 
    TAny* aInitParams )
    {
    CPenInputImePluginItut* self = new ( ELeave ) CPenInputImePluginItut(
                                        static_cast<RPeninputServer*>( 
                                            aInitParams ) );
    CleanupStack::PushL( self );
    self->ConstructL(); 
    return self;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginItut::~CPenInputImePluginItut
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPenInputImePluginItut::~CPenInputImePluginItut()
    {
    DeActivate();
    iUiLayoutImpIdList.Close();
    delete iPluginUiManager;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginItut::GetInputMethodUiL
// Create layout UI interface
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MAknFepManagerInterface* CPenInputImePluginItut::GetInputMethodUiL( 
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
        case EPluginInputModeItut:
            {
            if(aIsSplitView)
                {
                layoutId.iUid = KSplitItutUiId;
                }
            else
                {
                layoutId.iUid = KItutUiId;
                }
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
        TInt errCode = iPenInputServer->SetUiLayoutId( layoutId );
        if ( errCode == KErrNone )
            {
            fepmanager->OnInit();
            return fepmanager;
            }
        }
        
    return NULL;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginItut::GetInputMethodUiL
// Create layout UI interface
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MAknFepManagerInterface* CPenInputImePluginItut::GetInputMethodUiL( 
                                    MAknFepManagerUIInterface* aFepManager,
                                    TInt aLayoutId,
                                    const TDesC8& aData )
    {
    iFepManager = aFepManager;
    CPluginFepManagerBase* fepmanager = NULL;   
    TInt i;
    TInt mode;    
    
     if ( aLayoutId == KItutUiId || aLayoutId == KSplitItutUiId)
        {
        mode = EPluginInputModeItut;
        }
    else
        {
        mode = EPluginInputModeNone;
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
// CPenInputImePluginItut::Activate
// Activate current IME plugin.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPenInputImePluginItut::Activate()
    {
    if(iPenInputServer)
        {
        iPenInputServer->ActivateLayout(ETrue);        
        }
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginItut::DeActivate
// Deactivate current IME plugin.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPenInputImePluginItut::DeActivate()
    {
    if (iPenInputServer && iPenInputServer->IsVisible())        
        {
        iPenInputServer->ActivateLayout(EFalse);
        }
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginItut::ImeImplId
// Get current IME plugin implementation ID.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPenInputImePluginItut::ImeImplId()
    {
    return KInputMethodImplementationId;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginItut::LayoutUiImplId
// Get current layout UI implementation id.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPenInputImePluginItut::LayoutUiImplId()
    {
    TInt id;
    switch ( iPenInputMode )
        {
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


// -----------------------------------------------------------------------------
// CPenInputImePluginItut::SupportModes
// Get suppored layout UI modes(VKB/HWR).
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPenInputImePluginItut::SupportModes(CPtiEngine* aPtiEngine,
                                             RArray<TImePlguinImplDetail>& aSupportList) const
    {
    TInt ret = KErrNone;
    TRAP_IGNORE( ret = SupportModesL( aPtiEngine, aSupportList ) );
    return ret;
    }
    
// -----------------------------------------------------------------------------
// CPenInputImePluginItut::SupportModesL
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
    

TInt CPenInputImePluginItut::SupportModesL(CPtiEngine* /*aPtiEngine*/,
                                             RArray<TImePlguinImplDetail>& aSupportList) const 
    {
    RImplInfoPtrArray infoArray;
    TUid id;
    id.iUid = KHwrLayoutInterfaceId;
    TBool itut = EFalse;
    TBool splititut = EFalse;
    
    CleanupStack::PushL( TCleanupItem( Cleanup, &infoArray ) );
    REComSession::ListImplementationsL(id, infoArray);

    for (TInt i = 0; i < infoArray.Count(); ++i)
        {
        if (infoArray[i]->ImplementationUid().iUid == KItutUiId)
            {
            itut = ETrue;
            }
            
        if (infoArray[i]->ImplementationUid().iUid == KSplitItutUiId)
            {
            splititut = ETrue;
            } 
        }

    TImePlguinImplDetail detail;

    detail.iImplementationId = KInputMethodImplementationId;

    if (itut)
        {
        detail.iMode = EPluginInputModeItut;
        detail.iDisplayName.Copy(KItutImeName());
        detail.iMeritValue = EImeMerit_Preferred;

        TFindFile fileFinder(CCoeEnv::Static()->FsSession());

      	CDir* dir = NULL;
      	TInt result = fileFinder.FindWildByDir( KResFilePattern, KPluginResourcePath, dir );
      	if( result == KErrNone )
      	    {
            for (TInt i = 0; i < sizeof(KSupportScripts)/sizeof(KSupportScripts[0]); i++)
                {
                if ( IsFileInDir( dir, ResFileNameByScriptIdx(KSupportScripts[i].iScriptIdx) ) )
                    {
                    TInt* data = const_cast<TInt*>(KSupportScripts[i].iData);
                    for (TInt j = 0; j < KSupportScripts[i].iLen; j++)
                        {
                        detail.iLanguage = *(data);
                        data++;
                        aSupportList.AppendL(detail);                      
                        }
                    }
                }
      	    }
  	    delete dir;
        }    

    if (splititut)
        {
        detail.iMode = EPluginInputModeItut;
        detail.iDisplayName.Copy(KSplitItutImeName());
        detail.iMeritValue = EImeMerit_Preferred;

        TFindFile fileFinder(CCoeEnv::Static()->FsSession());

      	CDir* dir = NULL;
      	TInt result = fileFinder.FindWildByDir( KResFilePatternSplit, KPluginResourcePath, dir );
      	if( result == KErrNone )
      	    {
            for (TInt i = 0; i < sizeof(KSupportScripts)/sizeof(KSupportScripts[0]); i++)
                {
                if ( IsFileInDir( dir, ResFileNameByScriptIdxForSplit(KSupportScripts[i].iScriptIdx) ) )
                    {
                    TInt* data = const_cast<TInt*>(KSupportScripts[i].iData);
                    for (TInt j = 0; j < KSupportScripts[i].iLen; j++)
                        {
                        detail.iLanguage = *(data);
                        data++;
                        aSupportList.AppendL(detail);                      
                        }
                    }
                }
      	    }
  	    delete dir;
        }    

    CleanupStack::PopAndDestroy(&infoArray); // infoArray    
    
    return 0;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginItut::CurrentMode
// Get current layout UI mode.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPenInputImePluginItut::CurrentMode() const
    {
    return iPenInputMode;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginItut::HandleServerEventL
// Handle pen input server event..
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPenInputImePluginItut::HandleServerEventL( TInt aEventId, const TDesC& aData )
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
// CPenInputImePluginItut::HandleMenuCommandL
// Handle the menu command.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPenInputImePluginItut::HandleMenuCommandL( TInt /*aCommandId*/ )
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginItut::DynInitMenuPaneL
// Dynamiclly update menu items during it displays.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPenInputImePluginItut::DynInitMenuPaneL( CAknFepUiInterfaceMenuPane* /*aMenuPane*/ )
    {
    }


// -----------------------------------------------------------------------------
// CPenInputImePluginItut::CPenInputImePluginItut
// C++ default constructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPenInputImePluginItut::CPenInputImePluginItut( RPeninputServer* aHwrServer )
    :iPenInputServer( aHwrServer ) 
    {
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginItut::ConstructL
// 2nd phase constructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPenInputImePluginItut::ConstructL()
    {
    FindUiLayoutImplementationL();
    }

// -----------------------------------------------------------------------------
// CPenInputImePluginItut::FindUiLayoutImplementationL
// Enumuate all layout UI ids.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPenInputImePluginItut::FindUiLayoutImplementationL()
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
// CPenInputImePluginItut::GetPluginUiL
// Get HWR UI interface.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPluginFepManagerBase* CPenInputImePluginItut::GetPluginUiL( TInt aMode )
    {
    switch ( aMode )
        {
        case EPluginInputModeItut:
            {
            delete iPluginUiManager;
            iPluginUiManager = NULL;
            iPluginUiManager = CPluginFepManagerItut::NewL(*this, iPenInputServer);
            return iPluginUiManager;
            }
        default:
            {
            return NULL;
            }
        }
    }
    
// -----------------------------------------------------------------------------
// CPenInputImePluginItut::ResFileNameByScriptIdx
// Get HWR UI interface.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TDesC& CPenInputImePluginItut::ResFileNameByScriptIdx(const TScriptIndex aIndex)
    {
    switch (aIndex)
        {
        case ELatinLang:
            return KLatinResFile;
        case ECyrillicLang:
            return KCyrillicResFile;
        case EGreekLang:
            return KGreekResFile;
        case EHebrewLang:
            return KHebrewResFile;
        case EArabicLang:
            return KArabicResFile;
        case EFarsiLang:
            return KFarsiResFile;
        case EUrduLang:
            return KUrduResFile;
        case EThaiLang:
            return KThaiResFile;
        case EChnLang:
            return KChnResFile;
        case EIndicLang:
        	return KDevanagiriResFile;
        default:
            break;    
        }

    return KNullDesC;
    }

const TDesC& CPenInputImePluginItut::ResFileNameByScriptIdxForSplit(const TScriptIndex aIndex)
    {
    switch (aIndex)
        {
        case ELatinLang:
            return KLatinResFileSplit;

        case ECyrillicLang:
            return KCyrillicResFileSplit;

        case EGreekLang:
            return KGreekResFileSplit;

        case EHebrewLang:
            return KHebrewResFileSplit;
                
        case EArabicLang:
            return KArabicResFileSplit;

        case EFarsiLang:
            return KFarsiResFileSplit;

        case EUrduLang:
            return KUrduResFileSplit;

        case EThaiLang:
            return KThaiResFileSplit;

        case EChnLang:
            return KChnResFileSplit;

        case EIndicLang:
            return KDevanagiriResFileSplit;

        default:
            break;    
        }

    return KNullDesC;
    }

//End Of File
