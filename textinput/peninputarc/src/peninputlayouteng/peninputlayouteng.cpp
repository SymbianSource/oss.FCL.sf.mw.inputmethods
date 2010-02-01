/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for CFepLayoutEngine
*
*/


#include "peninputlayoutplugininterface.h"
#include "peninputuilayoutbase.h"
#include "peninputlayouteng.h"
#include "peninputimefinder.h"  
#include <PtiEngine.h>          //CPtiEngine

// ======== LOCAL FUNCTIONS ========

//
// Cleaup function
//
void Cleanup( TAny* aAny )
    {
    RImplInfoPtrArray* implArray = 
        reinterpret_cast< RImplInfoPtrArray*> ( aAny );
    if(implArray)
        {
        implArray->ResetAndDestroy();
        implArray->Close();            
        }
    }


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CFepLayoutEngine::NewL
// Factory function
// ---------------------------------------------------------------------------
//
EXPORT_C CFepLayoutEngine* CFepLayoutEngine::NewL()
    {
    CFepLayoutEngine *engine = new (ELeave) CFepLayoutEngine();
    CleanupStack::PushL( engine );
    engine->ConstructL();
    CleanupStack::Pop(engine); // engine
    return engine;
    }

// ---------------------------------------------------------------------------
// CFepLayoutEngine::NewL
// Factory function
// ---------------------------------------------------------------------------
//
EXPORT_C CFepLayoutEngine* CFepLayoutEngine::NewL(CPtiEngine* aPtiEngine)
    {
    CFepLayoutEngine *engine = new (ELeave) CFepLayoutEngine(aPtiEngine);
    CleanupStack::PushL( engine );
    engine->ConstructL();
    CleanupStack::Pop(engine); // engine
    return engine;
    }
// ---------------------------------------------------------------------------
// CFepLayoutEngine::CFepLayoutEngine
// Default constructor
// ---------------------------------------------------------------------------
//
CFepLayoutEngine::CFepLayoutEngine(CPtiEngine* aPtiEngine)
:iPtiEngine(aPtiEngine)
    {
    iLayoutId.iUid=0;
    }

// ---------------------------------------------------------------------------
// CFepLayoutEngine::ConstructL
// 2nd phase Constructor
// ---------------------------------------------------------------------------
//
void CFepLayoutEngine::ConstructL()
    {
    FindImeLayoutL(KHwrInputMethodId);
    iImeFinder = CImePluginFinder::NewL();
    }


// ---------------------------------------------------------------------------
// CFepLayoutEngine::~CFepLayoutEngine
// Destructor
// ---------------------------------------------------------------------------
//
CFepLayoutEngine::~CFepLayoutEngine()
    {
    iLayoutIdList.Reset();
    delete iCurInterface;
    iCurInterface = NULL;
    delete iImeFinder;
    iTouchSupportLanguages.Close();
    iImePluginIdList.Close();
    REComSession::FinalClose();
    }

// ---------------------------------------------------------------------------
// CFepLayoutEngine::GetUiLayout
// Create UI layout
// ---------------------------------------------------------------------------
//
EXPORT_C MPenUiLayoutBase* CFepLayoutEngine::GetUiLayout(TUid aInputMethodId,
                                 MLayoutOwner* aLayoutOwner,const TAny* aData)
    {
    MPenUiLayoutBase* uiLayout=NULL;
    if(aInputMethodId != iLayoutId)
        {
        iLayoutId = aInputMethodId;
        delete iCurInterface;
        iCurInterface = NULL;

        if( aInputMethodId == TUid::Null() ) 
            { 
            return NULL; 
            } 
            
        TUid destructor_ID_Key;
        TAny* pFunc = NULL;
        TRAPD(err, 
            pFunc = REComSession::CreateImplementationL(aInputMethodId, 
                                                           destructor_ID_Key)
                                                           );
        
       
        if(err != KErrNone)
            return NULL;
        else
            iCurInterface = reinterpret_cast<CFepUiLayoutInterface*>(pFunc);
                                                       
        iCurInterface->SetDestructorKey(destructor_ID_Key);
           
        }
        
    if(iCurInterface)
        {
        TRAP_IGNORE(uiLayout = iCurInterface->
                                     CreateFepUiLayoutL(aLayoutOwner,aData));
        }
                
    return uiLayout;
    }   
      
// ---------------------------------------------------------------------------
// CFepLayoutEngine::FindImeLayoutL
// Find layout for a input mode
// ---------------------------------------------------------------------------
//
void CFepLayoutEngine::FindImeLayoutL(TInt aInputMethodId)
    {
    
    if(iInputMethodId == aInputMethodId)
        return;
    
    iInputMethodId = aInputMethodId;
    
    RImplInfoPtrArray infoArray;
    TInt i;

    iLayoutIdList.Reset();

    CleanupStack::PushL( TCleanupItem( Cleanup, &infoArray ) );
    
    TUid id;
    id.iUid = aInputMethodId;
    
    REComSession::ListImplementationsL(id, infoArray);

    for (i = 0; i < infoArray.Count(); i++)
        {
        iLayoutIdList.AppendL(infoArray[i]->ImplementationUid().iUid);
        }

    CleanupStack::PopAndDestroy(&infoArray); // infoArray    
    
    }

// ---------------------------------------------------------------------------
// CFepLayoutEngine::UiLayoutListL
// Get the layout list
// ---------------------------------------------------------------------------
//
EXPORT_C const RArray<TInt>& CFepLayoutEngine::UiLayoutListL(TInt aInputMethodId)
    {
    FindImeLayoutL(aInputMethodId);
    return iLayoutIdList;
    }
    
// ---------------------------------------------------------------------------
// CFepLayoutEngine::ImePluginListL
// Get the IME plugin list by required mode
// ---------------------------------------------------------------------------
//
EXPORT_C const RArray<TInt>& CFepLayoutEngine::ImePluginListL(TInt aLanguage, TInt aPluginMode)
    {
    if( !iImeFinder->Initialized() )
        {
        iImeFinder->InitializeL(iPtiEngine);
        }
    
    iImeFinder->GetImePlugins(aLanguage, aPluginMode, iImePluginIdList);
    return iImePluginIdList;
    }

// ---------------------------------------------------------------------------
// CFepLayoutEngine::TouchSupportLanguagesL
// Get touch input language list
// ---------------------------------------------------------------------------
//
EXPORT_C const RArray<TInt>& CFepLayoutEngine::TouchSupportLanguagesL()
    {
    if( !iImeFinder->Initialized() )
        {
        iImeFinder->InitializeL(iPtiEngine);
        }
    
    iImeFinder->SupportLanguages(iTouchSupportLanguages);
    return iTouchSupportLanguages;
    }

// ---------------------------------------------------------------------------
// CFepLayoutEngine::IsSupportPluginMode
// Get touch input language list
// ---------------------------------------------------------------------------
//    
EXPORT_C TBool CFepLayoutEngine::IsSupportPluginMode( TLanguage aLanguage, TPluginInputMode aMode ) 
    {
    return iImeFinder->IsSupportPluginMode( aLanguage, aMode );
    }

// ---------------------------------------------------------------------------
// CFepLayoutEngine::InitializeL
// Initialize finder
// ---------------------------------------------------------------------------
//    
EXPORT_C void CFepLayoutEngine::InitializeL()
    {
    if(iPtiEngine && iImeFinder)
        {
        iImeFinder->InitializeL(iPtiEngine);
        }
    }
// End of file

