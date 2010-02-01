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
* Description:   Language database class implementation.
*
*/


#include <ecom/ecom.h>
#include "PtiLanguageDatabase.h"
#include "PtiUids.hrh"
#include <languages.hrh>

const TInt KDefaultDatabasesGran = 3;

//
// LOCAl METHODS
//

void DBCleanup( TAny* aAny )
	{
	RImplInfoPtrArray* implArray = 
		reinterpret_cast< RImplInfoPtrArray*> ( aAny );
	implArray->ResetAndDestroy();
	implArray->Close();
	}

//
// CPtiLanguageDatabase
//

// ---------------------------------------------------------------------------
// CPtiLanguageDatabase::CreateDatabaseL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPtiLanguageDatabase* CPtiLanguageDatabase::CreateDatabaseL(TUid aImplUid)
	{
	TUid dtorIDKey;
	CPtiLanguageDatabase* iface = (CPtiLanguageDatabase*)REComSession::CreateImplementationL(aImplUid, dtorIDKey);		
	if (iface)
		{
		iface->SetDestructorKeyId(dtorIDKey.iUid);
		}
	return iface;
	}


// ---------------------------------------------------------------------------
// CPtiLanguageDatabase::ListDatabasesLC
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CArrayFix<TInt>* CPtiLanguageDatabase::ListDatabasesLC(TInt aCoreUID)
	{
	RImplInfoPtrArray infoArray;
	TInt i;

	CArrayFixFlat<TInt>* result = new (ELeave) CArrayFixFlat<TInt>(KDefaultDatabasesGran);
	CleanupStack::PushL( result );

	CleanupStack::PushL( TCleanupItem( DBCleanup, &infoArray ) );
	REComSession::ListImplementationsL(TUid::Uid(aCoreUID), infoArray);

	for (i = 0; i < infoArray.Count(); i++)
		{
		result->AppendL(infoArray[i]->ImplementationUid().iUid);
		}

	CleanupStack::PopAndDestroy(); // infoArray 

	return result;
	}


// ---------------------------------------------------------------------------
// CPtiLanguageDatabase::~CPtiLanguageDatabase
// 
// ---------------------------------------------------------------------------
//
EXPORT_C CPtiLanguageDatabase::~CPtiLanguageDatabase()
	{
	REComSession::DestroyedImplementation(TUid::Uid(iDtorKeyId));
	}


// ---------------------------------------------------------------------------
// CPtiLanguageDatabase::CoreUID
// 
// ---------------------------------------------------------------------------
//			
EXPORT_C TInt CPtiLanguageDatabase::CoreUID() const
	{
	return 0;
	}


// ---------------------------------------------------------------------------
// CPtiLanguageDatabase::NumLanguages
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiLanguageDatabase::NumLanguages() const
	{
	return 0;
	}


// ---------------------------------------------------------------------------
// CPtiLanguageDatabase::LanguageCode
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiLanguageDatabase::LanguageCode(TInt /*aIndex*/) const
	{
	return 0;
	}


// ---------------------------------------------------------------------------
// CPtiLanguageDatabase::SubLanguageCode
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiLanguageDatabase::SubLanguageCode(TInt /*aIndex*/) const
	{
	return 0;
	}


// ---------------------------------------------------------------------------
// CPtiLanguageDatabase::LangData
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt8* CPtiLanguageDatabase::LangData(TInt /*aIndex*/) const
	{
	return NULL;
	}


// ---------------------------------------------------------------------------
// CPtiLanguageDatabase::LangData
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt8* CPtiLanguageDatabase::LangData(TInt /*aIndex*/,
                                               TAny* /*aNativeParams*/) const
	{
	return NULL;
	}


// ---------------------------------------------------------------------------
// CPtiLanguageDatabase::PinyinPhraseData
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt8* CPtiLanguageDatabase::PinyinPhraseData() const
	{
	return NULL;
	}


// ---------------------------------------------------------------------------
// CPtiLanguageDatabase::ZhuyinPhraseData
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt8* CPtiLanguageDatabase::ZhuyinPhraseData() const
	{
	return NULL;
	}


// ---------------------------------------------------------------------------
// CPtiLanguageDatabase::NativeId
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiLanguageDatabase::NativeId(TInt /*aIndex*/) const
	{
	return 0;
	}


// List of known language database uids. There maybe also others,
// but we don't need to instantiate databases for uids listed here
// to find out which language is implemented in it.
const TPtiLangDatabaseMapping langUids[] = 
	{
	{EPtiLangImplUidEnglish,    ELangEnglish},		  
	{EPtiLangImplUidFrench,     ELangFrench},		
	{EPtiLangImplUidGerman,     ELangGerman},	
	{EPtiLangImplUidSpanish,    ELangSpanish},		
	{EPtiLangImplUidItalian,    ELangItalian},		
	{EPtiLangImplUidSwedish,    ELangSwedish},		
	{EPtiLangImplUidDanish,     ELangDanish},		
	{EPtiLangImplUidNorwegian,  ELangNorwegian},	       
	{EPtiLangImplUidFinnish,    ELangFinnish},	
	{EPtiLangImplUidPortuguese, ELangPortuguese},	       
	{EPtiLangImplUidTurkish,    ELangTurkish},   	      
	{EPtiLangImplUidIcelandic,  ELangIcelandic},	      
	{EPtiLangImplUidRussian,    ELangRussian}, 	      
	{EPtiLangImplUidHungarian,  ELangHungarian},	      
	{EPtiLangImplUidDutch,      ELangDutch},		  
	{EPtiLangImplUidCzech,      ELangCzech},  		  
	{EPtiLangImplUidSlovak,     ELangSlovak},		  
	{EPtiLangImplUidPolish,     ELangPolish},	      
	{EPtiLangImplUidSlovenian,  ELangSlovenian},     	  
	{EPtiLangImplUidJapanese,   ELangJapanese},	       
	{EPtiLangImplUidThai,       ELangThai},   		   
	{EPtiLangImplUidArabic,     ELangArabic},		   
	{EPtiLangImplUidTagalog,    ELangTagalog},		       
	{EPtiLangImplUidCroatian,   ELangCroatian},    	
	{EPtiLangImplUidEstonian,   ELangEstonian},	    
	{EPtiLangImplUidGreek,      ELangGreek},	
	{EPtiLangImplUidHebrew,     ELangHebrew},   
	{EPtiLangImplUidHindi,      ELangHindi},	  
	{EPtiLangImplUidIndonesian, ELangIndonesian},	  
	{EPtiLangImplUidLatvian,    ELangLatvian},  	
	{EPtiLangImplUidLithuanian, ELangLithuanian},   	   
	{EPtiLangImplUidMalay,      ELangMalay},   	
	{EPtiLangImplUidRomanian,   ELangRomanian},	       
	{EPtiLangImplUidSerbian,    ELangSerbian},	    	   
	{EPtiLangImplUidUkrainian,  ELangUkrainian},
	{EPtiLangImplUidBulgarian,  ELangBulgarian},
	{EPtiLangImplUidVietnamese, ELangVietnamese},
	{EPtiLangImplUidPrcChinese, ELangPrcChinese},
	{EPtiLangImplUidHongKongChinese,  ELangHongKongChinese},
	{EPtiLangImplUidTaiwanChinese, 	  ELangTaiwanChinese},
	{EPtiLangImplUidCatalan,    ELangCatalan},
	{EPtiLangImplUidGalician,   KLangGalician},
	{EPtiLangImplUidBasque,     KLangBasque},	
	{EPtiLangImplUidUrdu,       ELangUrdu},	
	
	// Second set begins here.
	{EPtiLangImplUidEnglish2,    ELangEnglish},
	{EPtiLangImplUidFinnish2,    ELangFinnish},
    {EPtiLangImplUidGerman2,     ELangGerman},
    {EPtiLangImplUidFrench2,     ELangFrench},
    {EPtiLangImplUidDanish2,     ELangDanish},
    {EPtiLangImplUidHindi2,      ELangHindi},
    {EPtiLangImplUidRussian2,    ELangRussian},
    {EPtiLangImplUidArabic2,     ELangArabic},
    {EPtiLangImplUidHebrew2,     ELangHebrew},
    {EPtiLangImplUidSwedish2,    ELangSwedish},
    {EPtiLangImplUidEstonian2,   ELangEstonian},
    {EPtiLangImplUidIcelandic2,  ELangIcelandic},
    {EPtiLangImplUidItalian2,    ELangItalian},
    {EPtiLangImplUidCroatian2,   ELangCroatian},
    {EPtiLangImplUidHungarian2,  ELangHungarian},        
    {EPtiLangImplUidNorwegian2,  ELangNorwegian},
    {EPtiLangImplUidDutch2,      ELangDutch},
	{EPtiLangImplUidChinesePrc2, ELangPrcChinese},
	{EPtiLangImplUidChineseTW2,  ELangTaiwanChinese},
	{EPtiLangImplUidChineseHK2,  ELangHongKongChinese},
	
//	{EPtiLangImplUidChinesePAN2,    ELangEnglish},
	{EPtiLangImplUidTagalog2,    ELangTagalog},
    {EPtiLangImplUidThai2,     ELangThai},
    {EPtiLangImplUidSlovenian2,     ELangSlovenian},
    {EPtiLangImplUidSlovak2,     ELangSlovak},
    {EPtiLangImplUidSerbian2,      ELangSerbian},
    {EPtiLangImplUidCzech2,    ELangCzech},
    {EPtiLangImplUidBulgarian2,     ELangBulgarian},
    {EPtiLangImplUidGreek2,     ELangGreek},
    {EPtiLangImplUidSpanish2,    ELangSpanish},
    {EPtiLangImplUidLithuanian2,   ELangLithuanian},
    {EPtiLangImplUidLatvian2,  ELangLatvian},
    {EPtiLangImplUidMalay2,    ELangMalay},
    
    {EPtiLangImplUidPolish2,   ELangPolish},
    {EPtiLangImplUidRomanian2,  ELangRomanian},        
    {EPtiLangImplUidUkrainian2,  ELangUkrainian},
    {EPtiLangImplUidTurkish2,      ELangTurkish},
	{EPtiLangImplUidVietnamese2, ELangVietnamese},
	
	{EPtiLangImplUidPortuguese2,  ELangPortuguese},
	{EPtiLangImplUidFarsi2,  ELangFarsi},
	{EPtiLangImplUidUrdu2,  ELangUrdu},        
    {EPtiLangImplUidMarathi2,  ELangMarathi},
    {EPtiLangImplUidCanadianFrench2, ELangCanadianFrench},
	{EPtiLangImplUidIndonesian2, ELangIndonesian},
	{EPtiLangImplUidBrazilianPortuguese2,  ELangBrazilianPortuguese},
	{EPtiLangImplUidLatinAmericanSpanish2, ELangLatinAmericanSpanish},
	{EPtiLangImplUidCatalan2,    ELangCatalan},
	{EPtiLangImplUidGalician2,   KLangGalician},
	{EPtiLangImplUidBasque2,   KLangBasque},
	// third set begins here.
	{EPtiLangImplUidEnglish3,    ELangEnglish},	
	{EPtiLangImplUidFrench3,     ELangFrench},	 
    {EPtiLangImplUidGerman3,     ELangGerman},	
    {EPtiLangImplUidSpanish3,    ELangSpanish},	
    {EPtiLangImplUidItalian3,    ELangItalian},	
    {EPtiLangImplUidSwedish3,    ELangSwedish},	
    {EPtiLangImplUidDanish3,     ELangDanish},	
    {EPtiLangImplUidNorwegian3,  ELangNorwegian},	
    {EPtiLangImplUidFinnish3,    ELangFinnish},	
    {EPtiLangImplUidPortuguese3, ELangPortuguese},	
    {EPtiLangImplUidTurkish3,    ELangTurkish},	
    {EPtiLangImplUidIcelandic3,  ELangIcelandic},	
    {EPtiLangImplUidRussian3,    ELangRussian},	
    {EPtiLangImplUidHungarian3,  ELangHungarian},	
    {EPtiLangImplUidDutch3,      ELangDutch},	
    {EPtiLangImplUidCzech3,      ELangCzech},	
    {EPtiLangImplUidSlovak3,     ELangSlovak},	
    {EPtiLangImplUidPolish3,     ELangPolish},	
    {EPtiLangImplUidSlovenian3,  ELangSlovenian},	
    {EPtiLangImplUidThai3,       ELangThai},	
    {EPtiLangImplUidArabic3,     ELangArabic},	
    {EPtiLangImplUidTagalog3,    ELangTagalog},	
    {EPtiLangImplUidCroatian3,   ELangCroatian},	
    {EPtiLangImplUidEstonian3,   ELangEstonian},	 
    {EPtiLangImplUidGreek3,      ELangGreek},	 
    {EPtiLangImplUidHebrew3,     ELangHebrew},	 
    {EPtiLangImplUidHindi3,      ELangHindi},	 
    {EPtiLangImplUidIndonesian3, ELangIndonesian},	 
    {EPtiLangImplUidLatvian3,    ELangLatvian},	 
    {EPtiLangImplUidLithuanian3, ELangLithuanian},	 
    {EPtiLangImplUidMalay3,      ELangMalay},	 
    {EPtiLangImplUidRomanian3,   ELangRomanian},	 
    {EPtiLangImplUidSerbian3,    ELangSerbian},	
    {EPtiLangImplUidUkrainian3,  ELangUkrainian},	 
    {EPtiLangImplUidVietnamese3, ELangVietnamese},	 
    {EPtiLangImplUidBulgarian3,  ELangBulgarian},	 
    {EPtiLangImplUidBasque3,     KLangBasque},	 
    {EPtiLangImplUidCatalan3,    ELangCatalan},	 
    {EPtiLangImplUidGalician3,   KLangGalician},	 
    {EPtiLangImplUidUrdu3,       ELangUrdu},	 
    {EPtiLangImplUidMarathi3,    ELangMarathi},		
	{0, 0}
	};


// ---------------------------------------------------------------------------
// CPtiLanguageDatabase::LanguageCodeForUid
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiLanguageDatabase::LanguageCodeForUid(TInt aUid)
	{
	TInt i;

	for (i = 0; ; i++)
		{
		if (langUids[i].iUid == 0)
			{
			break;
			}
		else if (langUids[i].iUid == aUid)
			{
			return langUids[i].iLangCode;
			}		
		}

	return 0;
	}


// ---------------------------------------------------------------------------
// CPtiLanguageDatabase::CreateMappingTableL
// 
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPtiLanguageDatabase::CreateMappingTableL(TInt aCoreUid, RArray<TPtiLangDatabaseMapping>& aResult)
	{
	TInt res = 0, i, lc;
	TPtiLangDatabaseMapping map;
	CArrayFix<TInt>* dbs = ListDatabasesLC(aCoreUid);

	for (i = 0; i < dbs->Count(); i++)
		{
		lc = LanguageCodeForUid(dbs->At(i));
		map.iUid = dbs->At(i);
		if (lc)
			{
			map.iLangCode = lc;
			User::LeaveIfError(aResult.Append(map));
			}	
		else
			{
			CPtiLanguageDatabase* db = CreateDatabaseL(TUid::Uid(dbs->At(i)));
			if (db)
				{
				CleanupStack::PushL(db);
				map.iLangCode = db->LanguageCode(0);				
				User::LeaveIfError(aResult.Append(map));				
				CleanupStack::PopAndDestroy(); // db
				}
			}
		}

	CleanupStack::PopAndDestroy(); // dbs
	return res;
	}


// ---------------------------------------------------------------------------
// CPtiLanguageDatabase::CreateMappingTableOpaqueL
// 
// ---------------------------------------------------------------------------
//		
EXPORT_C TInt CPtiLanguageDatabase::CreateMappingTableWithOpaqueL(TInt aCoreUid, RArray<TPtiLangDatabaseMappingOpaque>& aResult)
    {
  	TInt res = 0, lc;
	TPtiLangDatabaseMappingOpaque map;	
	RImplInfoPtrArray infoArray;
	TInt i;

	CleanupStack::PushL( TCleanupItem( DBCleanup, &infoArray ) );
	REComSession::ListImplementationsL(TUid::Uid(aCoreUid), infoArray);
	
	for (i = 0; i < infoArray.Count(); i++)
		{
		lc = LanguageCodeForUid(infoArray[i]->ImplementationUid().iUid);
		
		map.iUid = infoArray[i]->ImplementationUid().iUid;
		map.iOpaque.Copy(infoArray[i]->OpaqueData().Left(KOpaqueDataLen));				
		
		if (lc)
			{
			map.iLangCode = lc;
			User::LeaveIfError(aResult.Append(map));
			}	
		else
			{
			CPtiLanguageDatabase* db = CreateDatabaseL(infoArray[i]->ImplementationUid());
			if (db)
				{
				CleanupStack::PushL(db);
				map.iLangCode = db->LanguageCode(0);				
				User::LeaveIfError(aResult.Append(map));				
				CleanupStack::PopAndDestroy(); // db
				}
			}
		}

	CleanupStack::PopAndDestroy(); // infoArray    

	return res;
    }
	

// End of file


