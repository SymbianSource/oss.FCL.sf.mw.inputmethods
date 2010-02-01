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
* Description:  Implementation for CImePluginFinder
*
*/


#include "peninputimefinder.h"  
#include "aknfeppeninputimeplugin.h"

const TInt KImePluginInterface = 0x10207363;

// ======== EXTERNAL FUNCTIONS ========

//
// Cleaup function
//
extern void Cleanup( TAny* aAny );

// ======== LOCAL FUNCTIONS ============

//
// Binary search comaration method
//
static TInt CompareImePlguin(const TInternalImePlguinImplDetail& aIme1, 
                             const TInternalImePlguinImplDetail& aIme2)
    {
    //order is descent
    return (aIme2.iMeritValue - aIme1.iMeritValue);
    }

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CImePluginFinder::NewL
// Factory function
// ---------------------------------------------------------------------------
//
CImePluginFinder* CImePluginFinder::NewL()
    {
    return new(ELeave)CImePluginFinder();
    }

// ---------------------------------------------------------------------------
// CImePluginFinder::~CImePluginFinder
// Destructor
// ---------------------------------------------------------------------------
//
CImePluginFinder::~CImePluginFinder()
    {
    Reset();
    }

// ---------------------------------------------------------------------------
// CImePluginFinder::InitializeL
// Initialze the finder and prepare all internal data
// ---------------------------------------------------------------------------
//
void CImePluginFinder::InitializeL(CPtiEngine* aPtiEngine)
    {
    //only initialzed once
    if( iInitialized )
        {
        return;
        }
    
    //list all ime imeplementation
    RImplInfoPtrArray infoArray;
    TUid id;
    id.iUid = KImePluginInterface;
    
    TEComResolverParams params;
    CleanupStack::PushL( TCleanupItem( Cleanup, &infoArray ) );
    REComSession::ListImplementationsL(id, infoArray);
    
    TUid dtor_ID_Key;
    CAknFepPenInputImePlugin* imeInterface = NULL;
    RArray<CAknFepPenInputImePlugin::TImePlguinImplDetail> details;
    
    for(TInt i = 0; i < infoArray.Count(); ++i )
        {
        TUid imeImplId = infoArray[i]->ImplementationUid();
        TRAP_IGNORE(imeInterface = REINTERPRET_CAST(CAknFepPenInputImePlugin*, 
                                 REComSession::CreateImplementationL(imeImplId, 
                                                                     dtor_ID_Key, 
                                                                     NULL)));

        if( !imeInterface )
            {
            continue;
            }

        imeInterface->iDtor_ID_Key = dtor_ID_Key;         
        imeInterface->SupportModes(aPtiEngine, details);
        
        //store all imeplemtnation details to internal array
        //this is important, because other list use the pointer 
        //to internal data of the iAllImplementations. Once iAllImplementations
        //changes, the pointers in other array may be invalid.
        for(TInt j = 0; j < details.Count(); ++j)
            {
            TInternalImePlguinImplDetail intDetail;
            intDetail.iImplementationId = details[j].iImplementationId;
            intDetail.iMode = details[j].iMode;
            intDetail.iMeritValue = details[j].iMeritValue;
            intDetail.iLanguage = details[j].iLanguage;
            iAllImplementations.AppendL(intDetail);
            }
            
        details.Reset();
        delete imeInterface;
        }
        
    for(TInt i = 0; i < iAllImplementations.Count(); ++i)
        {
        AddImplementationL(iAllImplementations[i]);
        }
        
    CleanupStack::PopAndDestroy(&infoArray); // infoArray    
    REComSession::FinalClose();//cleanup ecom
    iInitialized = ETrue;
    }

// ---------------------------------------------------------------------------
// CImePluginFinder::SupportLanguagesL
// Get pen support language list
// ---------------------------------------------------------------------------
//
void CImePluginFinder::SupportLanguages(RArray<TInt>& aLanguageLists)
    {
    aLanguageLists.Reset();
    for( TInt i = 0; i < iPluginList.Count(); ++i )
        {
        aLanguageLists.InsertInOrder(iPluginList[i].iLanguage);
        }
    }

// ---------------------------------------------------------------------------
// CImePluginFinder::GetImePlugins
// Get IME plugin implementation list
// ---------------------------------------------------------------------------
//
void CImePluginFinder::GetImePlugins(TInt aLanguage, TInt aPluginMode, 
                                     RArray<TInt>& aImplmentationIds)
    {
    aImplmentationIds.Reset();
    InternalGetImePlugins(aLanguage, aPluginMode, aImplmentationIds);
    }

// ---------------------------------------------------------------------------
// CImePluginFinder::GetImePlugins
// Get IME plugin implementation list
// ---------------------------------------------------------------------------
//
void CImePluginFinder::GetImePlugins(TInt aLanguage, RArray<TInt>& aImplmentationIds)
    {
    aImplmentationIds.Reset();
    InternalGetImePlugins(aLanguage, EPluginInputModeHwr, aImplmentationIds);
    InternalGetImePlugins(aLanguage, EPluginInputModeVkb, aImplmentationIds);
   // InternalGetImePlugins(aLanguage, EPluginInputModeCommon, aImplmentationIds);
    }
    
// ---------------------------------------------------------------------------
// CImePluginFinder::Initialized
// Test the Ime Finder initialiaztion state
// ---------------------------------------------------------------------------
//
TBool CImePluginFinder::Initialized()
    {
    return iInitialized; 
    }
    
// ---------------------------------------------------------------------------
// CImePluginFinder::RefreshL
// Refresh internal data after system changes
// ---------------------------------------------------------------------------
//
void CImePluginFinder::RefreshL(CPtiEngine* aPtiEngine)
    {
    Reset();
    InitializeL(aPtiEngine);
    }
// ---------------------------------------------------------------------------
// CImePluginFinder::IsSupportPluginMode
// ---------------------------------------------------------------------------
//
TBool CImePluginFinder::IsSupportPluginMode( TLanguage aLanguage, TPluginInputMode aMode )
    {
    TImePluginLangAndMode ref;
    ref.iLanguage = aLanguage;
    ref.iMode = aMode,
    ref.iImePluginList = NULL;
    
    TInt index = iPluginList.FindInSignedKeyOrder(ref);
    return index != KErrNotFound ? ETrue : EFalse;
    }

// ---------------------------------------------------------------------------
// CImePluginFinder::CImePluginFinder
// Constructor
// ---------------------------------------------------------------------------
//
CImePluginFinder::CImePluginFinder()
    {
    }
    
// ---------------------------------------------------------------------------
// CImePluginFinder::InternalGetImePlugins
// Internal version of get IME plugin implementation list
// ---------------------------------------------------------------------------
//
void CImePluginFinder::InternalGetImePlugins(TInt aLanguage, TInt aPluginMode, 
                                             RArray<TInt>& aImplmentationIds)
    {
    //find language in list
    TImePluginLangAndMode ref;
    ref.iLanguage = aLanguage;
    ref.iMode = aPluginMode,
    ref.iImePluginList = NULL;
    
    TInt index = iPluginList.FindInSignedKeyOrder(ref);
    if( index != KErrNotFound )
        {
        iPluginList[index].iImePluginList->GetImePlugins(aImplmentationIds);
        }
    }
    
// ---------------------------------------------------------------------------
// CImePluginFinder::FreeList
// Free a list
// ---------------------------------------------------------------------------
//
void CImePluginFinder::FreeList(RArray<TImePluginLangAndMode>& aList)
    {
    for( TInt i = 0; i < aList.Count(); ++i )
        {
        delete aList[i].iImePluginList;
        }
    aList.Close();
    }
    
// ---------------------------------------------------------------------------
// CImePluginFinder::Reset
// Reset all data and free memory
// ---------------------------------------------------------------------------
//
void CImePluginFinder::Reset()
    {
    FreeList( iPluginList );
    iAllImplementations.Close();
    iInitialized = EFalse;
    }
 
// ---------------------------------------------------------------------------
// CImePluginFinder::AddImplementationL
// Add a implementation into internal data structure
// ---------------------------------------------------------------------------
//
void CImePluginFinder::AddImplementationL(const TInternalImePlguinImplDetail& aImplDetail)
    {
    //find language in list
    TImePluginLangAndMode ref;
    ref.iLanguage = aImplDetail.iLanguage;
    ref.iMode = aImplDetail.iMode;
    //ref.iImePluginList = NULL;
    
    TInt index = iPluginList.FindInSignedKeyOrder(ref);
    if( index != KErrNotFound )
        {
        iPluginList[index].iImePluginList->Insert(&aImplDetail);
        }
    else
        {
        ref.iImePluginList = new(ELeave) CImePluginList();
        ref.iImePluginList->Insert(&aImplDetail);
        iPluginList.InsertInSignedKeyOrder(ref);
        }
    }

// ======== class CImePluginList========

// ---------------------------------------------------------------------------
// CImePluginList::CImePluginList
// 
// ---------------------------------------------------------------------------
//
CImePluginList::CImePluginList()
    {
    }
    
// ---------------------------------------------------------------------------
// CImePluginList::~CImePluginList
// Destructor
// ---------------------------------------------------------------------------
//
CImePluginList::~CImePluginList()
    {
    iPriList.Close();
    }

// ---------------------------------------------------------------------------
// CImePluginList::Insert
// Insert a implementation in the list and keep priority order
// ---------------------------------------------------------------------------
//
void CImePluginList::Insert(const TInternalImePlguinImplDetail* aImplDetail)
    {
    //find position
    iPriList.InsertInOrder(aImplDetail, 
                           TLinearOrder<TInternalImePlguinImplDetail>(CompareImePlguin));
    }
    
// ---------------------------------------------------------------------------
// CImePluginList::GetImePlugins
// Get Ime plugin list
// ---------------------------------------------------------------------------
//
void CImePluginList::GetImePlugins(RArray<TInt>& aImplmentationIds)
    {
    for(TInt i = 0; i < iPriList.Count(); ++i )
        {
        aImplmentationIds.Append(iPriList[i]->iImplementationId);
        }
    }

// End of file

