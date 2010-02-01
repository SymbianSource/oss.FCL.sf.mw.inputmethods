/*
* Copyright (c) 2003-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Default english multitapping core for PtiEngine
*               This core implementation is always available.
*
*/


#ifndef _PTI_DEFAULT_CORE_H
#define _PTI_DEFAULT_CORE_H

// INCLUDES
#include <e32base.h>
#include "PtiCore.h"

// FORWARD DECLARATIONS
class CPtiKeyMappings;
//class CPtiKeyMapData;
class CPtiKeyMapDataFactory;

const TInt KDefaulCoreMaximumWordLength = 100;
const TInt32 KDefaultCoreUid = 0xffffffff;
	
const TUid KPtiEnginePSUid = {0x101F8610}; // Same as PtiEngine dll
enum TPtiEnginePSKeys
    {
    EKeyMapPropertyCount,
    EKeyMapPropertyData
    };

//
// TPtiDataFactoryBinding
//
class TPtiDataFactoryBinding
    {
    public:
        inline TPtiDataFactoryBinding();
        inline TPtiDataFactoryBinding(TInt aLanguage, TInt aImplUid);        
    
    public:
        TInt iLanguage;
        TInt iFactoryImplUid;      
    };


// ---------------------------------------------------------------------------
// TPtiDataFactoryBinding::TPtiDataFactoryBinding
// 
// ---------------------------------------------------------------------------
//
inline TPtiDataFactoryBinding::TPtiDataFactoryBinding() :
		iLanguage(0), iFactoryImplUid(0)
	{    
	}

// ---------------------------------------------------------------------------
// TPtiDataFactoryBinding::TPtiDataFactoryBinding
// 
// ---------------------------------------------------------------------------
//
inline TPtiDataFactoryBinding::TPtiDataFactoryBinding(TInt aLanguage, TInt aImplUid) 
                                 : iLanguage(aLanguage), iFactoryImplUid(aImplUid)
    {    
    }


//
// CMultiTapTimer
//
NONSHARABLE_CLASS(CMultiTapTimer) : public CTimer
	{
	public:
		static CMultiTapTimer* NewL(TCallBack aCallback);
		void After(TTimeIntervalMicroSeconds32 aInterval);
	private:
		CMultiTapTimer(TCallBack aCallback);
		void ConstructL();
		// from CActive
		virtual void RunL();
	private:
		TCallBack iCallback;
	};


/**
Default multitapping & qwerty core. 	
*/
NONSHARABLE_CLASS(CPtiDefaultCore) : public CPtiCore
	{
	public:
		static CPtiDefaultCore* NewL(CPtiEngine* aOwner, TDes* aTextBuffer);
		~CPtiDefaultCore();

		 // From MPtiCore
		 void InitializeL(CPtiEngine* aOwner, TDes* aTextBuffer, CArrayPtrFlat<CPtiCoreLanguage>* aLanguageList, TBool aUseDefautUserDictionary);
		 void DropLanguages(CArrayPtrFlat<CPtiCoreLanguage>* aLanguageList);
		 TInt OpenLanguageL(CPtiCoreLanguage *aLanguage);
		 TInt CloseLanguageL();
		 TPtrC GetCurrentWord() const;
		 TInt CommitCurrentWord();
		 TInt NumberOfCandidates();
		 MPtiCoreInfo* GetCoreInfo();		 
		 TInt SetReordering(TBool aStatus);
		 void AppendKeyPress(TPtiKey aKey);
		 void DeleteKeyPress();
		 void ClearBuffer();
		 TInt SetCurrentInputSequence(TPtrC8 aInput);			
		 TInt Convert(TPtiCharConversion aType,
					  TAny* aInput,
					  TInt aInputLength,
					  TAny* aOutput);
		TInt SetExternalKeyMapL(TPtiEngineInputMode aMode,
                                TPtiKey aKey,
    						    TDesC& aKeyMap,
								TPtiTextCase aCase);
		TInt GetSpelling(TUint16 aInput, TDes& aOutput, TPtiSpelling aType);
		TInt CancelTimerActivity();
		TInt HandleCommandL(TPtiEngineCommand aCommand, TAny* aParams);
		void ListDataFactoryLanguagesL(CPtiKeyMapDataFactory* aDataFactory,
		                               TInt aImplUid,
		                               CArrayPtrFlat<CPtiCoreLanguage>* aLanguageList);		
		void LoadFactoryForLanguageL(TInt aLanguage);
   		   
	private:
		 CPtiDefaultCore();
		 CPtiDefaultCore(CPtiEngine* aOwner, TDes* aTextBuffer);
		 void ConstructL();
		 static TInt TimerExpire(TAny* aPtr);
		 TInt DoTimerExpire(); 
		 void FillCoreInfo();
		 void ClearVowelSequence();
		 void ShutDownKeymapFactory();

	private:
	     enum 
	     	{
	     	EPtiDeadKeyRootFlag = 0x01,
	     	EPtiGetAllFlag      = 0x02,
	     	EPtiFlagVowelSeq   	= 0x04,   
   			EPtiDoubleWideEntry = 0x08     		     	  	
	      	};
	     inline void SetFlag(TInt aFlag);
	     inline TBool IsFlagSet(TInt aFlag) const;
	     inline void ClearFlag(TInt aFlag); 	

	private:
		CPtiEngine*      iOwner;            // Not owned.
		TDes*            iTextBuffer;		// Not owned.
		CMultiTapTimer*  iTimer;
		TPtiCoreInfo     iCoreInfo;
		CPtiKeyMappings* iNumericMappings;
		CPtiKeyMapData*  iNumericData;		
		TInt             iFlags;
        CPtiKeyMapDataFactory* iKeyMapDataFactory;		     
        RArray<TPtiDataFactoryBinding> iFactoryBindings;  
        CArrayPtrFlat<CPtiCoreLanguage>* iLanguageList;   // Not owned                    			
	};


// ---------------------------------------------------------------------------
// CPtiDefaultCore::SetFlag
// 
// ---------------------------------------------------------------------------
//
inline void CPtiDefaultCore::SetFlag(TInt aFlag)
	{
	iFlags |= aFlag;
	}
	
// ---------------------------------------------------------------------------
// CPtiDefaultCore::IsFlagSet
// 
// ---------------------------------------------------------------------------
//	
inline TBool CPtiDefaultCore::IsFlagSet(TInt aFlag) const
	{
	return (iFlags & aFlag) != 0;	
	}
	
// ---------------------------------------------------------------------------
// CPtiDefaultCore::ClearFlag
// 
// ---------------------------------------------------------------------------
//	
inline void CPtiDefaultCore::ClearFlag(TInt aFlag)
	{
    iFlags &= ~aFlag;	
	}

#endif  // _PTI_DEFAULT_CORE_H

// End of file
