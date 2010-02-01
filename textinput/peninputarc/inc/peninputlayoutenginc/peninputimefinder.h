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
* Description:  header file of peninput ime plugin finder
*
*/



#ifndef C_CPENINPUTIMEFINDER_H
#define C_CPENINPUTIMEFINDER_H

// INCLUDES
#include <e32base.h>
#include <badesca.h> 
#include <aknfeppeninputimeplugin.h>

#include <AknFepGlobalEnums.h>

class CPtiEngine;
class RPeninputServer;
class TImePluginLangAndMode;

class TInternalImePlguinImplDetail
    {
public:
    TInt iImplementationId;
    TInt iMode;
    TInt iMeritValue;
    TInt iLanguage;
    };

/**
* CImePluginFinder class.
*
* This is main class for outside users to find 
*
* @lib feplayouteng.lib
* @since S60 V4.0
*/
class CImePluginFinder : public CBase
    {
public:
    /**
     * Two phase constructor.
     *
     * @since S60 V4.0
     * @return The layout engine instance
     */
    static CImePluginFinder* NewL();
    
    /**
     * Destructor.
     *
     * @since S60 V4.0
     */
    ~CImePluginFinder();
    
    /**
     * Initialze the finder and prepare all internal data
     *
     * @since S60 V4.0
     * @param aPtiEngine The ptiEngine instance, used by Ime plug-in 
     * @return None
     */
    void InitializeL(CPtiEngine* aPtiEngine);
    
    /**
     * Get pen support language list
     *
     * @since S60 V4.0
     * @param aLanguageLists A reference to a client owned array which 
     *                       will be filled with languages code
     * @return None                        
     */
    void SupportLanguages(RArray<TInt>& aLanguageLists);
    
    /**
     * Get IME plugin implementation list for specified language and plugin mode
     *
     * @since S60 V4.0
     * @param aLanguage The required language code.
     * @param aPluginMode The required IME plugin mode
     * @param aImplmentationIds A reference to a client owned array which 
     *                          will be filled with IME implementation code
     * @return None                        
     */
    void GetImePlugins(TInt aLanguage, TInt aPluginMode, RArray<TInt>& aImplmentationIds);
    
    /**
     * Get IME plugin implementation list for specified language 
     *
     * @since S60 V4.0
     * @param aLanguage The required language code.
     * @param aImplmentationIds A reference to a client owned array which 
     *                          will be filled with IME implementation code
     * @return None                        
     */
    void GetImePlugins(TInt aLanguage, RArray<TInt>& aImplmentationIds);

    /**
     * Test the Ime Finder initialiaztion state
     *
     * @since S60 V4.0
     * @return ETrue if initialzed, otherwise EFalse.
     */
    TBool Initialized(); 
      
    /**
     * Refresh internal data after system changes, such as disk insert/remove,
     * application install/uninstall and so on.
     * 
     * @since S60 V4.0
     * @return None
     */
    void RefreshL(CPtiEngine* aPtiEngine);

    /**
     * Is support TPluginInputMode for aLanguage. 
     *
     * @since S60 v4.0
     * @param aLanguage Value of TLanguage
     * @param aMode Value of TPluginInputMode
     * @return ETrue if supported. Otherwise EFalse.
     */
    TBool IsSupportPluginMode( TLanguage aLanguage, TPluginInputMode aMode ) ;
private:

    /**
     * Constructor
     *
     * @since S60 V4.0 
     */
    CImePluginFinder();

    /**
     * Internal version of get IME plugin implementation list 
     * for specified language and plugin mode
     *
     * @since S60 V4.0
     * @param aLanguage The required language code.
     * @param aPluginMode The required IME plugin mode
     * @param aImplmentationIds A reference to a client owned array which 
     *                          will be filled with IME implementation code
     * @return None                        
     */
    void InternalGetImePlugins(TInt aLanguage, TInt aPluginMode, RArray<TInt>& aImplmentationIds);
    
    /**
     * Free a list
     *
     * @since S60 V4.0
     * @param aList A reference to the list
     * @return None                        
     */
    void FreeList(RArray<TImePluginLangAndMode>& aList);
    
    /**
     * Reset all data and free memory
     *
     * @return None                        
     */
    void Reset();
    
    /**
     * Add a implementation into internal data structure
     *
     * @param aImplDetail A reference to IME implenmentation detail.
     * @return None                        
     */
    void AddImplementationL(const TInternalImePlguinImplDetail& aImplDetail);

private:

    /**
     * Implementation list, only contains pointer to iAllImplementations
     */
    RArray<TImePluginLangAndMode> iPluginList;
    
    /**
     * All implentation list
     */
    RArray<TInternalImePlguinImplDetail> iAllImplementations;

    /**
     * Flag of initialzed status
     */
    TBool iInitialized;
    };
     
/**
* CImePluginList class.
*
* This is class storing ime plugin prioritized
*
* @lib feplayouteng.lib
* @since S60 V4.0
*/
class CImePluginList: public CBase
    {
public:

    /**
     * Constructor
     *
     * @since S60 V4.0 
     */
    CImePluginList();

    /**
     * Destructor
     *
     * @since S60 V4.0 
     */
    ~CImePluginList();
    
    /**
     * Insert a implementation in the list and keep priority order
     *
     * @param aImplDetail A reference to IME implenmentation detail.
     * @return None                        
     */
    void Insert(const TInternalImePlguinImplDetail* aImplDetail);

    /**
     * Get Ime plugin list
     *
     * @param aImplmentationIds A reference to internal implementation list.
     * @return None                        
     */
    void GetImePlugins(RArray<TInt>& aImplmentationIds);
private:

    /**
     * Priorited Ime plugin list
     */
    RPointerArray<TInternalImePlguinImplDetail> iPriList;
    };
    
/**
* TImePluginLangAndMode structure.
*
*/
class TImePluginLangAndMode
    {
public:
    
    /**
     * Language code
     */
    TInt8 iLanguage; 
    
    /**
     * Language code
     */
    TInt16 iMode; 
    
    /**
     * Ime plugin list for speicial plugin mode and language of iLanguage
     */
    CImePluginList* iImePluginList;
    };

#endif  C_CPENINPUTIMEFINDER_H

// End of file
