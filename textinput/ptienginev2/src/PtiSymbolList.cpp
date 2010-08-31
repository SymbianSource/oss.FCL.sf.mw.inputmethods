/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Predective text input engine core local methods.
*
*/



#include <f32file.h>
#include <s32file.h>

#include "PtiSymbolList.h"


//=========== constant definition ==============
_LIT( KSymbolMutexName,"PtiHwrSymbol_101F8610" );
const TInt KUdmFileHeader = 0x20080808;
const TInt KUdmFileVersion = 0x00001000;
const TInt KInvalidPresetIndex = 0;


void CSymbol::InternalizeL(RReadStream& /*aStream*/)
    {
    }

void CSymbol::ExternalizeL(RWriteStream& aStream)
    {
    aStream.WriteInt32L( iHelpLine );
    aStream.WriteInt32L( iBaseLine );
    aStream.WriteInt32L( iRange.iScript );
    aStream.WriteInt32L( iRange.iRange );
    aStream.WriteUint32L( iPresetCode );
    if ( iSymbolName )
        {
        aStream.WriteInt32L( iSymbolName->Length() );
        aStream.WriteL( *iSymbolName, iSymbolName->Length() );
        }
    else
        {
        aStream.WriteInt32L( 0 );
        }

    aStream.WriteInt32L( iPointVectorLen );
    aStream.WriteL( reinterpret_cast< TUint8* >( iPointVector ), iPointVectorLen*sizeof( TPoint ) );
    	
    }

 CSymbol* CSymbol::NewL(RReadStream& aStream)
    {
    CSymbol* self  = new ( ELeave ) CSymbol();
    CleanupStack::PushL( self );	
    self->ConstructL( aStream );         
    CleanupStack::Pop( self );  
    return self; 	
    }

void CSymbol::ConstructL(RReadStream& aStream)
    {
    iHelpLine = aStream.ReadInt32L();
    iBaseLine = aStream.ReadInt32L();
    iRange.iScript = aStream.ReadInt32L();
    iRange.iRange = aStream.ReadInt32L();
    iPresetCode = aStream.ReadUint32L();
    
    TInt symbolLen = aStream.ReadInt32L();
    iSymbolName = NULL;
    if ( symbolLen )
        {
        iSymbolName = HBufC::NewL( symbolLen );
        TPtr ptr = iSymbolName->Des();
        aStream.ReadL( ptr, symbolLen );
        }

    iPointVectorLen = aStream.ReadInt32L();
    iPointVector = new ( ELeave ) TPoint[iPointVectorLen];
    aStream.ReadL( reinterpret_cast< TUint8* >( iPointVector ), iPointVectorLen*sizeof( TPoint ) );
    }

CSymbol::CSymbol()
    {
    iPresetCode = 0;
    }

 CSymbol* CSymbol::NewL(const TDesC& aText, const RArray<TPoint>& aModel, int aHelpLine, int aBaseLine,const THwrUdmRange& aRange)
    {	
    CSymbol* self =  new ( ELeave )CSymbol();
    CleanupStack::PushL( self );	
    self->ConstructL( aText, aModel, aHelpLine, aBaseLine, aRange );         
    CleanupStack::Pop( self );  
    return self; 
    }

CSymbol::~CSymbol()
    {
    delete iSymbolName;
    delete [] iPointVector;
    }

void CSymbol::ConstructL(const TDesC& aText, const RArray<TPoint>& aModel, int aHelpLine, int aBaseLine,const THwrUdmRange& aRange)
    {
    iHelpLine = aHelpLine;
    iBaseLine = aBaseLine;
    iRange = aRange;
    iSymbolName = aText.AllocL( );
    iPresetCode = KInvalidPresetIndex; 
    
    iPointVectorLen = aModel.Count();
    iPointVector = new ( ELeave ) TPoint[iPointVectorLen];
    memcpy( iPointVector, &(aModel[0]), iPointVectorLen*sizeof( TPoint ) );
    }

TInt CSymbol::SymbolOrderDescending(const CSymbol& aFirst, const CSymbol& aSecond)
    {
    return aFirst.iSymbolName->Compare( *(aSecond.iSymbolName ) );
    }

TBool CSymbol::Match ( const THwrUdmRange& aRange )
    {
    if ( aRange.iScript !=  EPtiHwrScriptAny )
        {
        if ( aRange.iScript != iRange.iScript )
            {
            return EFalse;
            }
        }
    
    if ( aRange.iRange != EPtiHwrRangeAny )
        {
        if ( aRange.iRange != iRange.iRange )
            {
            return EFalse;
            }
        }
    
    return ETrue;    
    }

CSymbolList* CSymbolList::NewL(const TDesC& aFilePath, CSymbolList* aPresetList )
    {
    CSymbolList* self  =  new ( ELeave ) CSymbolList( aPresetList );
    CleanupStack::PushL( self );	
    self->ConstructL( aFilePath );         
    CleanupStack::Pop( self );  
    return self;                
    }
    
void CSymbolList::SetSymbolModelL(const TDesC& aText, const RArray<TPoint>& aModel, TInt aHelpLine , TInt aBaseLine ,const THwrUdmRange& aRange )
    {
    if( !aText.Length() || !aModel.Count() )
        User::Leave( KErrGeneral );
    if ( CheckSymbolModel( aText , aRange ) )
        {
        User::Leave( KErrAlreadyExists );
        }
    
    CSymbol* symbol = CSymbol::NewL( aText, aModel, aHelpLine, aBaseLine, aRange );       
    CleanupStack::PushL( symbol );
    TLinearOrder<CSymbol> order( CSymbol::SymbolOrderDescending );
    // using order insertion later
    // iSymbolList.InsertInOrderL( symbol, order );
    iSymbolList.AppendL( symbol );
    CleanupStack::Pop( symbol );
    ExternalizeL();
    }

TBool CSymbolList::CheckSymbolModel(const TDesC& aChar ,const THwrUdmRange& aRange )
    {
    TInt idx = -1;
    return GetSymbolIndex( aChar, idx, aRange ) == KErrNone ? ETrue : EFalse ;
    }

void CSymbolList::GetSymbolModelL(const TDesC& aChar, RArray<TPoint>& aModel ,TUint& aUnicode, const THwrUdmRange& aRange )
    {
    TInt idx = -1;
    if ( GetSymbolIndex( aChar, idx, aRange ) != KErrNone )
        {
        User::Leave( KErrNotFound );
        }
    // decides which list the idx belongs to   
    CSymbol* symbol = NULL; 
    if ( idx >= iSymbolList.Count() && iPresetModels )
        {
        symbol = iPresetModels->iSymbolList[ idx - iSymbolList.Count() ];
        }
    else
        {
        symbol = iSymbolList[idx];
        }
        
    aUnicode = symbol->iPresetCode;
    aModel.Reset();
    for ( int i = 0; i < symbol->iPointVectorLen; i++ )
        {
        aModel.AppendL( symbol->iPointVector[i] );    
        }    
    }

void CSymbolList::DeleteSymbolModelL(const TDesC& aChar ,const THwrUdmRange& aRange )
    {
    TInt idx = -1;
    if ( GetSymbolIndex( aChar, idx, aRange ) != KErrNone )
        {
        User::Leave( KErrNotFound );
        }
    if ( idx < iSymbolList.Count() )
        {
        CSymbol* symbol = iSymbolList[idx]; 
        iSymbolList.Remove( idx );
        delete symbol;
        ExternalizeL();
        }
    else if ( iPresetModels )
        {
        // modify preset models
        TInt presetIdx = idx -iSymbolList.Count();
        CSymbol& symbol = *iPresetModels->iSymbolList[presetIdx];
        delete symbol.iSymbolName;
        symbol.iSymbolName = NULL;
        iPresetModels->ExternalizeL();
        }
    }

void CSymbolList::GetModelTextListL(RPointerArray<HBufC>& aList ,const THwrUdmRange& /*aRange*/ )
    {
    for ( int i = 0; i < iSymbolList.Count(); i++ )
        {
        aList.AppendL( (*iSymbolList[i]->iSymbolName).AllocL() );
        }
        
    // then get preset shortcut models if have
    if ( iPresetModels )
        {
        for ( int i = 0; i < iPresetModels->iSymbolList.Count(); i++ )
            {
            // if the preset model is assigned to a shourtcut, then append it to the list
            CSymbol* symbol = iPresetModels->iSymbolList[i];
            if ( symbol->iSymbolName && symbol->iPresetCode )
                {
                aList.AppendL( symbol->iSymbolName->AllocL() );
                }
            }
        }
    }

void CSymbolList::ChangeSymbolTextL(const TDesC& aOldText, const TDesC& aNewText ,const THwrUdmRange& aRange )
    {
    TInt idx = -1;
    if ( GetSymbolIndex( aOldText, idx, aRange ) != KErrNone )
        {
        User::Leave( KErrNotFound );
        } 
        
    if ( aOldText.Compare( aNewText ) == 0 )
        {
        return ;  
        }
        
    if ( CheckSymbolModel( aNewText, aRange ) )
        {
        User::Leave( KErrAlreadyExists );
        }  
    if ( idx < iSymbolList.Count() )
        {
        CSymbol& symbol = *iSymbolList[idx];
        delete symbol.iSymbolName;
        symbol.iSymbolName = aNewText.AllocL();   
        ExternalizeL();
        }
    else if ( iPresetModels )
        {
        // modify preset models
        TInt presetIdx = idx -iSymbolList.Count();
        
        CSymbol& symbol = *iPresetModels->iSymbolList[presetIdx];
        delete symbol.iSymbolName;
        symbol.iSymbolName = aNewText.AllocL(); ;
        
        iPresetModels->ExternalizeL();
        }
    }
    
void CSymbolList::GetModelIndexListL( RArray<TInt>& aList, const THwrUdmRange& aRange)
    {
    aList.Reset();
    for ( int i = 0; i < iSymbolList.Count(); i++ )
        {
        if ( iSymbolList[i]->Match( aRange ) )
            {
            aList.AppendL( i );
            }
        }
    
    // append the symbols to the array    
    if ( iPresetModels )
        {
        for ( int i = 0; i < iPresetModels->iSymbolList.Count(); i++ )
            {
            if ( iPresetModels->iSymbolList[i]->iSymbolName &&
                    iPresetModels->iSymbolList[i]->Match( aRange  ) )
                {
                aList.AppendL( i + iSymbolList.Count() );
                }
            }
        }
    }

void CSymbolList::InternalizeL(const TDesC& /*aFile*/)
    {
    // Read the data file and construct the object
    iMutex.Wait();
    CleanupStack::PushL(TCleanupItem(SignalMutex, &iMutex));
    RFile readFile;
    TInt errCode = readFile.Open( iRfs, *iFilePath, EFileRead  );
    CleanupClosePushL( readFile );
        
    if ( errCode == KErrNone  )
        {
        RFileReadStream readStream( readFile );
        CleanupClosePushL( readStream );
        
        // check file Type&Version
        TInt fileType = readStream.ReadInt32L();
        TInt fileVersion = readStream.ReadInt32L();
        if ( fileType != KUdmFileHeader || fileVersion != KUdmFileVersion )
            {
            User::Leave( KErrGeneral );
            }
        
        // construct each CSymbol object and add to array
        TInt symbolNO = readStream.ReadInt32L();
        for ( int i = 0; i < symbolNO; i++ )  
            {  
            CSymbol* symbol = CSymbol::NewL( readStream );
            iSymbolList.AppendL( symbol );
            }

        // check wether the file is valid.
        if ( symbolNO != iSymbolList.Count() )
            {
            User::Leave( KErrGeneral );    
            }    
        CleanupStack::PopAndDestroy( &readStream );  
        }
    CleanupStack::PopAndDestroy( &readFile );  
    CleanupStack::PopAndDestroy(); // TCleanupItem(SignalMutex, &iMutex)
    }

void CSymbolList::ExternalizeL()
    {
    // write a temp file and use replace.
    iMutex.Wait();
    CleanupStack::PushL(TCleanupItem(SignalMutex, &iMutex));
    
    RFile fileTemp;
    CleanupClosePushL( fileTemp );
    
    //HBufC* tempFile = HBufC::NewLC( iFilePath->Length() + 3 );
    //TPtr tempFilePtr( tempFile->Des() );
    //tempFilePtr.Copy( *iFilePath );
    //tempFilePtr.Append( KTempPathFix );
    

    TParse fileParse;
    fileParse.Set( *iFilePath, NULL, NULL );
        
    TFileName tempName;
    iRfs.MkDirAll( fileParse.DriveAndPath() );
    User::LeaveIfError ( fileTemp.Temp( iRfs, fileParse.DriveAndPath(), tempName, EFileWrite ) );
    
    RFileWriteStream writeStream( fileTemp );
    CleanupClosePushL( writeStream );

    // write file type&version
    writeStream.WriteInt32L( KUdmFileHeader );
    writeStream.WriteInt32L( KUdmFileVersion );
    
    writeStream.WriteInt32L( iSymbolList.Count() );
    for ( int i = 0; i < iSymbolList.Count(); i++ )  
        {  
        iSymbolList[i]->ExternalizeL( writeStream );
        }
    writeStream.CommitL();
    CleanupStack::PopAndDestroy( &writeStream );
    CleanupStack::PopAndDestroy( &fileTemp );
    
    iRfs.Delete( *iFilePath ) ;
    User::LeaveIfError( iRfs.Rename( tempName ,*iFilePath  ) );
    
    CleanupStack::PopAndDestroy(); // TCleanupItem(SignalMutex, &iMutex)
    }

CSymbolList::CSymbolList( CSymbolList* aPresetList )
    {
    iPresetModels = aPresetList;
    iFilePath = NULL;
    }

void CSymbolList::ConstructL(const TDesC& aFilePath)
    {
    User::LeaveIfError( iRfs.Connect() );
    iFilePath = aFilePath.AllocL();   
    
    // create mutex
    TInt error( KErrNotFound );
    while( error == KErrNotFound )
         {
         error = iMutex.CreateGlobal( KSymbolMutexName );
         if( error != KErrAlreadyExists )
             {
             break;
             }
         error = iMutex.OpenGlobal( KSymbolMutexName );
         }
     User::LeaveIfError( error );
      
    InternalizeL( aFilePath );
    }

CSymbolList::~CSymbolList()
    {
    delete iFilePath;
    for ( int i = 0; i < iSymbolList.Count(); i++ )
        {
        delete iSymbolList[i];
        }
    iSymbolList.Close();    
    iRfs.Close();
    iMutex.Close();
    }

TInt CSymbolList::GetSymbolIndex(const TDesC& aText, TInt& aIndex, const THwrUdmRange& aRange )
    {
    // using quick search later.
    for ( int i = 0; i < iSymbolList.Count(); i++ )
        {
        if ( iSymbolList[i]->iSymbolName->Compare( aText ) == 0 && iSymbolList[i]->Match( aRange ))
            {
            aIndex = i;
            return KErrNone;    
            }   
        }

    // then find in preset model        
    if ( iPresetModels )
        {
        for ( int i = 0; i < iPresetModels->iSymbolList.Count(); i++ )
            {
            CSymbol& symbol = *iPresetModels->iSymbolList[i];
            // not assigned
            if ( !symbol.iSymbolName )
                {
                continue;
                }
                
            if ( symbol.iSymbolName->Compare( aText ) == 0 && symbol.Match( aRange ))
                {
                aIndex = iSymbolList.Count() + i;
                return KErrNone;    
                }   
            }
        }
    
    return KErrNotFound;    
    }

TInt CSymbolList::GetSymbolDataRef( TInt aIndex, TPtrC& aSymbolName,RArray<TPoint>& aModel, TInt& aHelpLine, TInt& aBaseLine )
    {

    CSymbol* symbol = NULL; 
    if ( aIndex >= 0 && aIndex < iSymbolList.Count() )
        {
        symbol = iSymbolList[aIndex];
        }
    else if ( iPresetModels && aIndex >= iSymbolList.Count() 
                    && aIndex <   iSymbolList.Count() + iPresetModels->iSymbolList.Count() )
        {
        symbol = iPresetModels->iSymbolList[ aIndex - iSymbolList.Count() ];
        }
    else
        {
        return KErrNotFound;
        }
        
    aModel.Reset();    
    aModel = RArray<TPoint>( sizeof(TPoint), symbol->iPointVector, symbol->iPointVectorLen );
    aHelpLine = symbol->iHelpLine;
    aBaseLine = symbol->iBaseLine;
    aSymbolName.Set( *symbol->iSymbolName );

    return KErrNone;  
    }


void CSymbolList::SavePresetShortcutL( TUint aUnicode, const TDesC& aShortcut ) 
    {
    User::LeaveIfNull( iPresetModels );
    
    for ( int i = 0; i < iPresetModels->iSymbolList.Count(); i ++ )
        {
        CSymbol* sym = iPresetModels->iSymbolList[i];
        if ( aUnicode == sym->iPresetCode )
            {
            if ( sym-> iSymbolName )
                {
                delete sym->iSymbolName;
                sym->iSymbolName = NULL;
                }
                
            if ( aShortcut.Length() > 0 )
                {
                sym->iSymbolName = aShortcut.AllocL();
                }
            iPresetModels->ExternalizeL();    
            return;
            }
        }
        
    User::Leave( KErrNotFound );
    }

void CSymbolList::GetAllPresetSymbolsL( RArray<TUint>& aPresets )
    {
    User::LeaveIfNull( iPresetModels );
    
    for ( int i = 0; i < iPresetModels->iSymbolList.Count(); i ++ )
        {
        aPresets.AppendL( iPresetModels->iSymbolList[i]->iPresetCode );
        }
    }

void CSymbolList::GetPresetSymbolByUnicodeL( TUint aUnicode, RArray<TPoint>& aModel, TDes& aShortcut )
    {
    User::LeaveIfNull( iPresetModels );
    
    for ( int i = 0; i < iPresetModels->iSymbolList.Count(); i ++ )
        {
        CSymbol* sym = iPresetModels->iSymbolList[i];
        if ( aUnicode == sym->iPresetCode )
            {
            aShortcut = KNullDesC;
            if ( sym->iSymbolName )
                {
                aShortcut.Copy( *sym->iSymbolName );
                }
            
            for ( int i = 0; i < sym->iPointVectorLen; i++ )
                {
                aModel.AppendL( sym->iPointVector[i] );    
                }    
            return;
            }
        }
    User::Leave( KErrNotFound );    
    }
    
void CSymbolList::SignalMutex(TAny* aMutex)
   {
   STATIC_CAST( RMutex*, aMutex )->Signal();
   }







