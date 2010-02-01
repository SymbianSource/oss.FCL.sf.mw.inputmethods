/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Predective text input engine Hwr Recognizer local methods.
*
*/



// INCLUDES
#include <ecom/ecom.h>
#include "PtiHwrRecognizer.h"
#include "PtiSymbolList.h"
#include "PtiTruiEngine.h"

const TUid KCHwrInterfaceUid =  {0x1028185d};
_LIT8(KInnerSeparator, "-");
_LIT8(KDataSeparator, "||");

// ---------------------------------------------------------------------------
// CHwrRecognizer::NewL(TInt aImpId)
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C CHwrRecognizer* CHwrRecognizer::NewL(TInt aImpId)
    {
    CHwrRecognizer* interface = (CHwrRecognizer*)REComSession::CreateImplementationL (TUid::Uid(aImpId), 
                                                                                      _FOFF(CHwrRecognizer, iDtor_ID_Key));

    return interface;
    }

// ---------------------------------------------------------------------------
// CHwrRecognizer::NewL(const TLanguage aLanguage)
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C CHwrRecognizer* CHwrRecognizer::NewL(const TLanguage aLanguage, TUid& aHwrImplUid) 
    {
    RArray<TUid> uidArray;
    ListImplementationsL(aLanguage, uidArray);

    if (uidArray.Count() > 0)
    	{
        CleanupClosePushL(uidArray);
            	    
        CHwrRecognizer* interface = (CHwrRecognizer*)REComSession::CreateImplementationL (uidArray[0], 
                                                                                          _FOFF(CHwrRecognizer, iDtor_ID_Key));
        
        aHwrImplUid = uidArray[0];
        
        CleanupStack::PopAndDestroy();  // uidArray        
        
        return interface; 
    	}
    else
        {
        return NULL;        
        }     
    }

// ---------------------------------------------------------------------------
// CHwrRecognizer::~CHwrRecognizer
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C CHwrRecognizer::~CHwrRecognizer()
    {
    REComSession::DestroyedImplementation(iDtor_ID_Key);
    delete iUdmEngine;
    }

// ---------------------------------------------------------------------------
// CHwrRecognizer::ListImplementationsL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CHwrRecognizer::ListImplementationsL(const TLanguage aLanguage, RArray<TUid>& aImpIdList)
    {
    CleanupClosePushL(aImpIdList);

    RImplInfoPtrArray infoArray;
    extern void Cleanup( TAny* aAny );
    CleanupStack::PushL(TCleanupItem(Cleanup, &infoArray));
    
    REComSession::ListImplementationsL(KCHwrInterfaceUid, infoArray);
        
    for (TInt i=0; i<infoArray.Count(); i++)
        {
        if (Match(infoArray[i]->DataType(), aLanguage))
        	{
        	User::LeaveIfError(aImpIdList.Append(infoArray[i]->ImplementationUid()));
        	}
        }
    	
    CleanupStack::PopAndDestroy(); // infoArray 
    
    CleanupStack::Pop();  //  aImpIdList
    }

// ---------------------------------------------------------------------------
// CHwrRecognizer::GetRanges
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
void CHwrRecognizer::GetRanges(const TDesC8& aRangeData, 
                               TInt& aLowerValue, 
                               TInt& aUpperValue)
    {
    aLowerValue = aUpperValue = 0;
    const TInt separatorLength = KInnerSeparator().Length();
    TInt separatorPos = aRangeData.Find(KInnerSeparator); 
    TLex8 atoi;

    if(separatorPos == KErrNotFound)
        {
        atoi.Assign(aRangeData);
        atoi.Val(aLowerValue);
        aUpperValue = aLowerValue;
        }
    else
        {
        //get low bound
        atoi.Assign(aRangeData.Left(separatorPos));
        atoi.Val(aLowerValue);
        //get upper bound
        atoi.Assign(aRangeData.Mid(separatorPos + separatorLength));
        atoi.Val(aUpperValue);
        }
    }

// ---------------------------------------------------------------------------
// CHwrRecognizer::Match
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
TBool CHwrRecognizer::Match(const TDesC8& aImplementationData, TInt aValue)
	{
    TBool gotMatch = EFalse;
    TInt lower, upper;
    const TInt separatorLength = KDataSeparator().Length();
    TInt separatorPos = aImplementationData.Find(KDataSeparator);

    if(separatorPos == KErrNotFound)
        {
        GetRanges(aImplementationData, lower, upper);
        if( aValue <= upper && aValue >= lower )
            {
            gotMatch = ETrue;
            }
        }
    else
        {
        // Find the first section, up to the separator
        TInt startPos = 0;

        while( separatorPos != KErrNotFound )
            {
            TPtrC8 dataSection = aImplementationData.Mid(startPos, separatorPos);
            startPos += separatorPos + separatorLength;
            TPtrC8 remainingData = aImplementationData.Mid(startPos );
            GetRanges(dataSection, lower, upper);

            if( aValue <= upper && aValue >= lower )
                {
                gotMatch = ETrue;
                break;
                }

            separatorPos = remainingData.Find(KDataSeparator);

            if( separatorPos == KErrNotFound )
                {
                GetRanges(remainingData, lower, upper);
                if( aValue <= upper && aValue >= lower )
                    {
                    gotMatch = ETrue;
                    }
                }
            }
        }

    return gotMatch;
	}

// ---------------------------------------------------------------------------
// CHwrRecognizer::AppendMatchedLanguages
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
void CHwrRecognizer::AppendMatchedLanguagesL(const TDesC8& aImplementationData, RArray<TInt>& aResult)
	{
    TInt lower, upper, i;
    const TInt separatorLength = KDataSeparator().Length();
        
    TInt separatorPos = aImplementationData.Find(KDataSeparator);
        
    if(separatorPos == KErrNotFound)
        {
        GetRanges(aImplementationData, lower, upper);

        for (i = lower; i <= upper; i++)
          	{
            if (aResult.Find(i) == KErrNotFound)
               	{
                User::LeaveIfError(aResult.Append(i));
               	}
           	}
        }
    else
        {
        TInt startPos = 0;

        while (separatorPos != KErrNotFound)
         	{
            TPtrC8 dataSection = aImplementationData.Mid(startPos, separatorPos);
            startPos += separatorPos + separatorLength;
            TPtrC8 remainingData = aImplementationData.Mid(startPos );
            
            GetRanges(dataSection, lower, upper);

           	for (i = lower; i <= upper; i++)
           		{
           		if (aResult.Find(i) == KErrNotFound)
           			{
           			User::LeaveIfError(aResult.Append(i));
           			}
           		}
            		
           	separatorPos = remainingData.Find(KDataSeparator);

           	if (separatorPos == KErrNotFound)
           		{
           		GetRanges(remainingData, lower, upper);

                for (i = lower; i <= upper; i++)
            	    {
                    if (aResult.Find(i) == KErrNotFound)
                	    {
                	    User::LeaveIfError(aResult.Append(i));
                	    }
            	    }
            	}
            }
        }
	}

// ---------------------------------------------------------------------------
// CHwrRecognizer::ListAvailableLanguages
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CHwrRecognizer::ListAvailableLanguagesL(RArray<TInt>& aResult)
    {
    RImplInfoPtrArray infoArray;
    
    extern void Cleanup( TAny* aAny );
    CleanupStack::PushL(TCleanupItem(Cleanup, &infoArray));
    
    REComSession::ListImplementationsL(KCHwrInterfaceUid, infoArray);
        
    for (TInt i=0; i<infoArray.Count(); i++)
    	{
        AppendMatchedLanguagesL(infoArray[i]->DataType(), aResult);
        }
    	
    CleanupStack::PopAndDestroy();
    }

// ---------------------------------------------------------------------------
// CHwrRecognizer::ListGivenAvailableLanguagesL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CHwrRecognizer::ListGivenAvailableLanguagesL(TUid aImpId, RArray<TInt>& aResult)
    {
    RImplInfoPtrArray infoArray;

    extern void Cleanup( TAny* aAny );
    CleanupStack::PushL(TCleanupItem(Cleanup, &infoArray));

    REComSession::ListImplementationsL(KCHwrInterfaceUid, infoArray);
        
    for (TInt i=0; i<infoArray.Count(); i++)
    	{
    	if ( aImpId == infoArray[i]->ImplementationUid() )
    	    {
            AppendMatchedLanguagesL(infoArray[i]->DataType(), aResult);
            break;
    	    }
    	}
    	
    CleanupStack::PopAndDestroy();  // infoArray
    }

// ---------------------------------------------------------------------------
// CHwrRecognizer::SetRange
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C TInt CHwrRecognizer::SetRange(const TRecognitionRange& /*aRange*/)
    {
    return KErrNotSupported;
    }
    
// ---------------------------------------------------------------------------
// CHwrRecognizer::GetRange
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C TRecognitionRange CHwrRecognizer::GetRange()
    {
    TRecognitionRange aRange;

    aRange.iLanguage = ELangNone;
    aRange.iSubRange = 0;

    return aRange;
    }

// ---------------------------------------------------------------------------
// CHwrRecognizer::GetRange
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C TInt CHwrRecognizer::AddAuxiliaryRange(const TRecognitionRange& /*aRange*/)
    {
    return KErrNotSupported;
    }
    
// ---------------------------------------------------------------------------
// CHwrRecognizer::RemoveAllAuxRange
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CHwrRecognizer::RemoveAllAuxRange()
    {
    }

// ---------------------------------------------------------------------------
// CHwrRecognizer::RemoveAuxiliaryRange
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C TInt CHwrRecognizer::RemoveAuxiliaryRange(const TInt /*aIndex*/)
    {
    return KErrNotSupported;
    }

// ---------------------------------------------------------------------------
// CHwrRecognizer::SetCandidateNum
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CHwrRecognizer::SetCandidateNum(const TInt /*aNumber*/)
    {
    return KErrNotSupported;
    }

// ---------------------------------------------------------------------------
// CHwrRecognizer::GetCandidateNum
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C TInt CHwrRecognizer::GetCandidateNum() const
    {
    return KErrNotSupported;    
    }
    
// ---------------------------------------------------------------------------
// CHwrRecognizer::StrokeEndMark
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C TPoint CHwrRecognizer::StrokeEndMark() const
    {
    TPoint point = TPoint(-65536, -65536);
    return point;
    }

// ---------------------------------------------------------------------------
// CHwrRecognizer::Recognize
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C TInt CHwrRecognizer::Recognize(const RArray<TPoint>& /*aTraceData*/, 
                                             RPointerArray<HBufC>& /*aResult*/)
    {
    return KErrNotSupported;
    }

// ---------------------------------------------------------------------------
// CHwrRecognizer::SetInputAreaSize
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C TInt CHwrRecognizer::SetInputAreaSize(TSize& /*aSize*/)
    {
    return KErrNotSupported; 
    }
    
// ---------------------------------------------------------------------------
// CHwrRecognizer::GetEngineVersion
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C TInt CHwrRecognizer::GetEngineVersion(TDes& /*aVersion*/)
    {
    return KErrNotSupported;
    }

// ---------------------------------------------------------------------------
// CHwrRecognizer::GetDictVersion
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C TInt CHwrRecognizer::GetDictVersion(TDes& /*aVersion*/)
    {
    return KErrNotSupported;
    }

// ---------------------------------------------------------------------------
// CHwrRecognizer::EnableWordRecognition
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CHwrRecognizer::EnableWordRecognition(const TBool /*aFlag*/)
    {
    }

// ---------------------------------------------------------------------------
// CHwrRecognizer::SetFirstLetterOrder
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CHwrRecognizer::SetFirstLetterOrder(const TLatinLetterOrder /*aOrder*/)
    {
    }

// ---------------------------------------------------------------------------
// CHwrRecognizer::SetNumberMode
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CHwrRecognizer::SetNumberMode(const TAknEditorNumericKeymap /*aNumberMode*/)
    {
    }

// ---------------------------------------------------------------------------
// CHwrRecognizer::SetTopGuideLinePos
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CHwrRecognizer::GetTopGuideLinePos( TInt& /*aPos*/)
	{
	}

// ---------------------------------------------------------------------------
// CHwrRecognizer::SetBottomGuideLinePos
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CHwrRecognizer::GetBottomGuideLinePos( TInt& /*aPos*/)
	{
	}
	
// ---------------------------------------------------------------------------
// CHwrRecognizer::SetPermittedSymbolSet
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CHwrRecognizer::SetPermittedSymbolSet(const TDesC& /*aSymbolSet*/)
    {
    }	
    
// ---------------------------------------------------------------------------
// CHwrRecognizer::RecognizeWithCharSet
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C TInt CHwrRecognizer::RecognizeWithCharSet(const RArray<TPoint>& /*aTraceData*/, 
                                                   RPointerArray<HBufC>& /*aResult*/,
                                                   const TDesC& /*aCharSet*/)
    {
    return KErrNotSupported;
    }

// ---------------------------------------------------------------------------
// CHwrRecognizer::SetAuxCandidateNum
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C TInt CHwrRecognizer::SetAuxCandidateNum(const TInt /*aNumber*/)
    {
    return KErrNotSupported;
    }

// ---------------------------------------------------------------------------
// CHwrRecognizer::GetAuxCandidateNum
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C TInt CHwrRecognizer::GetAuxCandidateNum() const
    {
    return KErrNotSupported;    
    }    
    
// ---------------------------------------------------------------------------
// CHwrRecognizer::QueryUdmInterfaceL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//  
EXPORT_C MTruiEngine* CHwrRecognizer::QueryUdmInterfaceL()
    {
    if ( !IsSupportUdm() )
        {
        User::Leave( KErrNotSupported );
        return NULL;
        }
    if ( iUdmEngine == NULL )
        {
        iUdmEngine = CTruiPtiEngine::NewL(  this  );  
        }
    return static_cast<MTruiEngine*>( iUdmEngine );
    }
    
// ---------------------------------------------------------------------------
// CHwrRecognizer::BeginMcrSession
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CHwrRecognizer::McrBeginSessionL()
    {
    return;
    }

// ---------------------------------------------------------------------------
// CHwrRecognizer::McrRecognize
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CHwrRecognizer::McrAddStrokesL(const RArray<TPoint>& /*aTraceData*/, 
                                                RPointerArray<HBufC>& /*aResult*/)
    {
    return ;
    }

// ---------------------------------------------------------------------------
// CHwrRecognizer::McrEndSessionL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CHwrRecognizer::McrEndSessionL( RPointerArray<HBufC>& /*aResult*/ )
    {
    return;
    }       
// ---------------------------------------------------------------------------
// CHwrRecognizer::IsSupportUdm
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C   TBool CHwrRecognizer::IsSupportUdm() const
    {
    return EFalse;
    }

// ---------------------------------------------------------------------------
// CHwrRecognizer::LoadUdmL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C   void CHwrRecognizer::LoadUdmL( TUdmType aType  )
    {
    if ( !iUdmEngine )
        {
        iUdmEngine = CTruiPtiEngine::NewL(  this  );
        }
    iUdmEngine->LoadUdmL( aType );
    }
    
// ---------------------------------------------------------------------------
// CHwrRecognizer::UnLoadUdmL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C   void CHwrRecognizer::UnLoadUdmL( TUdmType aType )
    {
    if ( iUdmEngine )
        {
        iUdmEngine->UnLoadUdm( aType );
        }
    }
    
// ---------------------------------------------------------------------------
// CHwrRecognizer::GetModelIndexListL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//         
EXPORT_C void CHwrRecognizer::GetModelIndexListL( TUdmType aType, RArray<TInt>& aList, const THwrUdmRange& aRange )
    {
    User::LeaveIfNull( iUdmEngine );
    iUdmEngine->GetModelIndexListL( aType, aList, aRange );
    
    }
    
// ---------------------------------------------------------------------------
// CHwrRecognizer::GetSymbolModelL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C void CHwrRecognizer::GetSymbolModelL( TUdmType aType, TInt aIndex, TPtrC& aSymbolName,RArray<TPoint>& aModel, TInt& aHelpLine, TInt& aBaseLine )
    {
    User::LeaveIfNull( iUdmEngine );
    iUdmEngine->GetSymbolModelL( aType, aIndex, aSymbolName,  aModel, aHelpLine, aBaseLine );
    }

 EXPORT_C TInt CHwrRecognizer::SymbolModelValid(  const TDesC& /*aSymbolName*/, const RArray<TPoint>& /*aStrokes*/, THwrUdmRange /*aScriptType*/, TDes& /*aSimilarSymbol*/ )
    {
    return KErrNotSupported;
    }
 // ---------------------------------------------------------------------------
 // CHwrRecognizer::EnableRecognitionDictionary
 // (other items were commented in a header).
 // ---------------------------------------------------------------------------
 // 
EXPORT_C TInt CHwrRecognizer::EnableRecognitionDictionary(const TBool /* aFlag */)
    { 
    return KErrNotSupported;    	    
    }
 // ---------------------------------------------------------------------------
 // CHwrRecognizer::SetScreenSize
 // (other items were commented in a header).
 // ---------------------------------------------------------------------------
 // 
EXPORT_C TInt CHwrRecognizer::SetScreenSize(TSize& /*aSize*/) 
    {
    return KErrNotSupported;    	
    }
// ---------------------------------------------------------------------------
// CHwrRecognizer::EnableGuideline
// (other items were commented in a header).
// ---------------------------------------------------------------------------
// 
EXPORT_C TInt CHwrRecognizer::EnableGuideline(const TBool /*aFlag */)
    {
    return KErrNotSupported;        
    }
// ======== CPtiHwrRecognizer Implementation ========
CPtiHwrRecognizer::~CPtiHwrRecognizer()
    {
    delete iHwrRecognizer;
    iHwrLanguagesList.Close();
    }
    
// End Of File
