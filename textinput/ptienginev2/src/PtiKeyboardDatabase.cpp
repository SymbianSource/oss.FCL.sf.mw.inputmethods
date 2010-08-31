/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Keyboard database class implementation.
*
*/


#include <ecom/ecom.h>
#include <PtiKeyboardDatabase.h>
#include "PtiUids.hrh"
#include <languages.hrh>

//
// LOCAl METHODS
//

void KDB_DBCleanup( TAny* aAny )
	{
	RImplInfoPtrArray* implArray = 
		reinterpret_cast< RImplInfoPtrArray*> ( aAny );
	implArray->ResetAndDestroy();
	implArray->Close();
	}
				
// ---------------------------------------------------------------------------
// CPtiKeyboardDatabase::KdbData
// 
// ---------------------------------------------------------------------------
// 				
EXPORT_C TInt8* CPtiKeyboardDatabase::KdbData(TInt /*aNativeKdbId*/, TAny* /*aNativeParams*/) const
    {    
    return NULL;
    }
		
		
// ---------------------------------------------------------------------------
// CPtiKeyboardDatabase::LanguageCode
// 
// ---------------------------------------------------------------------------
// 	
EXPORT_C TInt CPtiKeyboardDatabase::LanguageCode() const
    {
    return ELangNone;
    }	
	
	
// ---------------------------------------------------------------------------
// CPtiKeyboardDatabase::LanguageCode
// 
// ---------------------------------------------------------------------------
// 		
EXPORT_C TInt CPtiKeyboardDatabase::NativeId() const
    {
    return 0;
    }			
	
	
// ---------------------------------------------------------------------------
// Reserved ordinals
// 
// ---------------------------------------------------------------------------
// 			
EXPORT_C void CPtiKeyboardDatabase::Reserved_1()
    {    
    }
        
EXPORT_C void CPtiKeyboardDatabase::Reserved_2()
    {    
    }


// ---------------------------------------------------------------------------
// CPtiKeybaordDatabaseFactory::CreateMappingTableWithOpaqueL
// 
// ---------------------------------------------------------------------------
// 	
EXPORT_C TInt CPtiKeyboardDatabaseFactory::CreateMappingTableWithOpaqueL(TInt aInterfaceUid,
                                                  RArray<TPtiKeyboardDatabaseMappingOpaque>& aResult)
    {
	TInt res = 0;
	TPtiKeyboardDatabaseMappingOpaque map;	
	RImplInfoPtrArray infoArray;
	TInt i;

	CleanupStack::PushL( TCleanupItem( KDB_DBCleanup, &infoArray ) );
	REComSession::ListImplementationsL(TUid::Uid(aInterfaceUid), infoArray);
	
	for (i = 0; i < infoArray.Count(); i++)
		{		
		map.iUid = infoArray[i]->ImplementationUid().iUid;
		map.iOpaque.Copy(infoArray[i]->OpaqueData().Left(KKdbOpaqueDataLen));					
			
		User::LeaveIfError(aResult.Append(map));
		}
		
	CleanupStack::PopAndDestroy(); // infoArray    

	return res;    
    }
		
// ---------------------------------------------------------------------------
// CPtiKeybaordDatabaseFactory::CreateImplementationL
// 
// ---------------------------------------------------------------------------
// 		
EXPORT_C CPtiKeyboardDatabaseFactory* CPtiKeyboardDatabaseFactory::CreateImplementationL(const TUid aImplUid)
    {
	TUid dtorIDKey;
	CPtiKeyboardDatabaseFactory* iface = (CPtiKeyboardDatabaseFactory*)REComSession::CreateImplementationL(aImplUid, dtorIDKey);		
	if (iface)
		{
		iface->SetDestructorKeyId(dtorIDKey.iUid);
		iface->SetImplementationUid(aImplUid.iUid);
		}
	return iface;           
    }

// ---------------------------------------------------------------------------
// CPtiKeybaordDatabaseFactory::ListImplementationsL
// 
// ---------------------------------------------------------------------------
// 			
EXPORT_C void CPtiKeyboardDatabaseFactory::ListImplementationsL(TInt aCoreUid, 
                                                                RArray<TInt>& aResult)
    {
	RImplInfoPtrArray infoArray;
	TInt i;

	CleanupStack::PushL( TCleanupItem( KDB_DBCleanup, &infoArray ) );
	REComSession::ListImplementationsL(TUid::Uid(aCoreUid), infoArray);

	for (i = 0; i < infoArray.Count(); i++)
		{
		aResult.AppendL(infoArray[i]->ImplementationUid().iUid);
		}

	CleanupStack::PopAndDestroy(); // infoArray     
    }
        
// ---------------------------------------------------------------------------
// CPtiKeybaordDatabaseFactory::~CPtiKeybaordDatabaseFactory
// 
// ---------------------------------------------------------------------------
// 	      
EXPORT_C CPtiKeyboardDatabaseFactory::~CPtiKeyboardDatabaseFactory()
    {
	REComSession::DestroyedImplementation(TUid::Uid(iDTorId));    
    }

// ---------------------------------------------------------------------------
// Reserved ordinals
// 
// ---------------------------------------------------------------------------
// 	        
EXPORT_C void CPtiKeyboardDatabaseFactory::Reserved_1()
    {    
    }
	    
	    
EXPORT_C void CPtiKeyboardDatabaseFactory::Reserved_2()
    {     
    }

// End of file


