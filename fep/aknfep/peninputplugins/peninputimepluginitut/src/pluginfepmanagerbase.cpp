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
* Description:            Layout UI interface base class implementation
*
*/












// System includes
#include <w32std.h>
#include <s32mem.h> 
#include <aknedsts.h>               // CAknEdwinState
#include <centralrepository.h>
#include <peninputclient.h>
#include <aknfeppeninputenums.h>
#include <AknFepInternalCRKeys.h>
#include <peninputgenericitutcustomization.h>

// User includes
#include "pluginfepmanagerbase.h"
#include "peninputimepluginitut.h"

// ======== MEMBER FUNCTIONS ========


// -----------------------------------------------------------------------------
// CPluginFepManagerBase::CPluginFepManagerBase
// Destructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPluginFepManagerBase::~CPluginFepManagerBase()
    {
    delete iRepository;
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::OnInit
// Initalize UI.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::OnInit()
    {
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::RetrieveEditorMaxLength
// Explicitly get ICF max length from layout UI .
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::RetrieveEditorMaxLength()
    {
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::UpdateCaseMode
// Update local copy of case mode.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::UpdateCaseMode( TInt aCaseMode )
    {
    iLastCase = aCaseMode; 
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SetNumberModeKeyMappingL
// Set layout UI current number mode.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::SetNumberModeKeyMappingL( 
    TAknEditorNumericKeymap aAknEditorNumericKeymap )
    {
    SendCommandToServer( ECmdPenInputEditorNumericKeyMap, 
                         aAknEditorNumericKeymap );    
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::HandleKeyL
// Handle key event.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPluginFepManagerBase::HandleKeyL( TInt aKey, TKeyPressLength aLength, TEventCode /*aEventCode*/ )
    {
    return iHkbUi->HandleKeyL( aKey, aLength );
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::HandleCommandL
// Handle command come from FEP.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::HandleCommandL( TInt aCommandId )
    {
    SendCommandToServer( aCommandId );
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::HandleCommandL
// Handle command come from FEP.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::HandleCommandL(TInt aCommandId,TInt aParam)
    {
    switch ( aCommandId )
        {
        case ECmdPenInputRange:
            {
            iHkbMode = aParam;
            }
            break;
        case ECmdPenInputPermittedRange:
            {
            iLastCase = ECaseInvalide;
            SetLayoutPermitedRanges( aParam );
            }
            break;
        case ECmdPenInputEditorState:
            {
            CAknEdwinState* state = 
                reinterpret_cast<CAknEdwinState*>( aParam );
            SetLayoutRange( state );
            }
            break;
        case ECmdPenInputSetWindowPos:
            {
            TRect* rect = reinterpret_cast<TRect*>( aParam );
            ASSERT( rect );
            SetLayoutPosition( *rect );
            }
            break;
        case ECmdPenInputCaseMode:
            //if( aParam != iLastCase )
                {
                SendCommandToServer( ECmdPenInputCase, aParam );
                iLastCase = aParam;
                }
            break;
        case ECmdPenInputEditorCustomNumericKeyMap:
            {
            TDesC* res = (TDesC*)aParam;
            TPtrC8 param;
            param.Set((TUint8*)res->Ptr(), res->Size());
            SendCommandToServer( aCommandId, param ); 
            SetNumberModeKeyMappingL
                ( ( TAknEditorNumericKeymap )EKeymapFromResource );                   
            }
            break;
        case ECmdPenInputSuggestRange:
            iSuggestedRange = aParam;
            if( iSuggestedRange == ERangeAccent )
                {
                iSuggestedRange = ERangeEnglish;
                }
            break;
        case ECmdPenInputSendEditorTextAndCurPos:
            {
            TFepInputContextFieldData* pIcfData = 
                reinterpret_cast<TFepInputContextFieldData*>( aParam );
            TInt dataSize = sizeof( TFepInputContextFieldData );
            TInt textSize = pIcfData->iText.Size();
            
			HBufC8* buf = HBufC8::NewLC(dataSize + textSize + 2*sizeof(TInt));
			TPtr8 bufPtr = buf->Des();
			
			RDesWriteStream writeStream;
			writeStream.Open(bufPtr);
			CleanupClosePushL(writeStream);
			
			writeStream.WriteInt32L(dataSize);
			writeStream.WriteInt32L(textSize);
			
			const TUint8* pData = reinterpret_cast<const TUint8*>( pIcfData );
			writeStream.WriteL( pData, dataSize );
        		
    		const TUint8* pText = reinterpret_cast<const TUint8*>( pIcfData->iText.Ptr() );
		    writeStream.WriteL( pText, textSize );
		    
            writeStream.CommitL();
        
            SendCommandToServer( aCommandId, bufPtr );
    
			CleanupStack::PopAndDestroy(&writeStream);
			CleanupStack::PopAndDestroy(buf);
            }
            break;
        case ECmdPenInputFingerMatchList:
            {
            TFepInputAllCandidates* pCanData = reinterpret_cast<TFepInputAllCandidates*>(aParam);
            TInt count = (*pCanData).iCandidates.Count();

            TInt transferSize = 2 * sizeof(TInt);
            RArray<TInt> sizeArray;
            for (TInt i = 0; i < count; i++)
                {
                sizeArray.Append((*pCanData).iCandidates[i].Size());
                transferSize += sizeArray[i] + sizeof(TInt);
                }

            HBufC8* buf = HBufC8::NewLC(transferSize);
            TPtr8 bufPtr = buf->Des();

            RDesWriteStream writeStream;
            writeStream.Open(bufPtr);
            CleanupClosePushL(writeStream);

            writeStream.WriteInt32L(count);
            writeStream.WriteInt32L(transferSize);

            for (TInt i = 0; i < count; i++)
                {
                writeStream.WriteInt32L(sizeArray[i]);
                const TUint16* piData = (*pCanData).iCandidates[i].Ptr();
                writeStream.WriteL(piData, sizeArray[i]/2);
                }
			
            writeStream.CommitL();
        
            SendCommandToServer( aCommandId, bufPtr );

            sizeArray.Close();    
			CleanupStack::PopAndDestroy(&writeStream);
			CleanupStack::PopAndDestroy(buf);
            
            }
            break;
        case ECmdPenInputSetPromptText:
            {
            TFepPromptText* pPromptData = 
                reinterpret_cast<TFepPromptText*>( aParam );
                
            TInt dataSize = sizeof( TFepPromptText );
            TInt textSize = 0;
            
            if (pPromptData->iText.Length())
                {
                textSize = pPromptData->iText.Size();
                }
            
			HBufC8* buf = HBufC8::NewLC(dataSize + textSize + 2*sizeof(TInt));
			TPtr8 bufPtr = buf->Des();
			
			RDesWriteStream writeStream;
			writeStream.Open(bufPtr);
			CleanupClosePushL(writeStream);
			
			writeStream.WriteInt32L(dataSize);
			writeStream.WriteInt32L(textSize);
			
			const TUint8* pData = reinterpret_cast<const TUint8*>( pPromptData );
			writeStream.WriteL( pData, dataSize );
        	
        	if (textSize != 0)
        	    {
                const TUint16* pText = pPromptData->iText.Ptr();
    		    writeStream.WriteL( pText, textSize/2 );
        	    }
		    
            writeStream.CommitL();
        
            SendCommandToServer( aCommandId, bufPtr );
    
			CleanupStack::PopAndDestroy(&writeStream);
			CleanupStack::PopAndDestroy(buf);
            }
            break;
        case ECmdPeninputSpellICFDisplayContent:
        	{
        	TFepSpellICFDisplayContent* pContent = 
        			reinterpret_cast<TFepSpellICFDisplayContent*>( aParam );
        	
            TInt dataSize = sizeof( TFepSpellICFDisplayContent );
            TInt icfTextSize = pContent->iICFText.Size();
            TInt promptTextSize = pContent->iPromptText.Size();
        	
        	HBufC8* buf = HBufC8::NewLC( dataSize + 
        			icfTextSize + promptTextSize + 3 * sizeof(TInt));
        	TPtr8 bufPtr = buf->Des();
        				
			RDesWriteStream writeStream;
			writeStream.Open( bufPtr );
			CleanupClosePushL(writeStream);
			
			writeStream.WriteInt32L(dataSize);
			writeStream.WriteInt32L(icfTextSize);
			writeStream.WriteInt32L(promptTextSize);
			
			const TUint8* pData = reinterpret_cast<const TUint8*>( pContent );
			writeStream.WriteL( pData, dataSize );
			
			if ( icfTextSize != 0 )
				{
				const TUint16* pIcfText = pContent->iICFText.Ptr();
				writeStream.WriteL( pIcfText, icfTextSize/2 );
				}
			
			if ( promptTextSize != 0 )
				{
				const TUint16* pPromptText = pContent->iPromptText.Ptr();
				writeStream.WriteL( pPromptText, promptTextSize/2 );
				}
			
			writeStream.CommitL();
		
			SendCommandToServer( aCommandId, bufPtr );
	
			CleanupStack::PopAndDestroy(&writeStream);
			CleanupStack::PopAndDestroy(buf);
        	}
        	break;
        case ECmdPenInputFingerMatchIndicator:
            {
            TFepIndicatorInfo* pIndicatorData = 
                reinterpret_cast<TFepIndicatorInfo*>( aParam );
            
			HBufC8* buf = HBufC8::NewLC(4 * sizeof(TInt));
			TPtr8 bufPtr = buf->Des();
			
			RDesWriteStream writeStream;
			writeStream.Open(bufPtr);
			CleanupClosePushL(writeStream);
			
            writeStream.WriteInt32L(pIndicatorData->iIndicatorImgID);
            writeStream.WriteInt32L(pIndicatorData->iIndicatorMaskID);
            writeStream.WriteInt32L(pIndicatorData->iIndicatorTextImgID);
            writeStream.WriteInt32L(pIndicatorData->iIndicatorTextMaskID);
		    
            writeStream.CommitL();
        
            SendCommandToServer( aCommandId, bufPtr );
    
			CleanupStack::PopAndDestroy(&writeStream);
			CleanupStack::PopAndDestroy(buf);			
            }
            break;
        case ECmdPenInputSendEditMenuData:
            {
            TInt* commandList = reinterpret_cast<TInt*>( aParam );
            
            HBufC8* buf = HBufC8::NewLC( (*commandList+1) * sizeof(TInt));
            TPtr8 bufPtr = buf->Des();
      
            RDesWriteStream writeStream;
            writeStream.Open(bufPtr);
            CleanupClosePushL(writeStream);
      
            writeStream.WriteInt32L(*commandList);
            for (TInt i=1;i<=*commandList;i++)
                {
                writeStream.WriteInt32L(*(commandList+i));
                }
        
            writeStream.CommitL();  
            
            SendCommandToServer( aCommandId, bufPtr );
            
            CleanupStack::PopAndDestroy(&writeStream);
            CleanupStack::PopAndDestroy(buf);                          
            }
            break;
            
        case ECmdPenInputSymbolOfHardKeyOne:
            {
            TFepSymbolOfHardwareOne* pIndicatorData = 
                reinterpret_cast<TFepSymbolOfHardwareOne*>( aParam );
            
            HBufC8* buf = HBufC8::NewLC(3 * sizeof(TInt));
            TPtr8 bufPtr = buf->Des();
            
            RDesWriteStream writeStream;
            writeStream.Open(bufPtr);
            CleanupClosePushL(writeStream);
            
            writeStream.WriteInt32L(pIndicatorData->iSymbol1);
            writeStream.WriteInt32L(pIndicatorData->iSymbol2);
            writeStream.WriteInt32L(pIndicatorData->iSymbol3);
            
            writeStream.CommitL();
        
            SendCommandToServer( aCommandId, bufPtr );
    
            CleanupStack::PopAndDestroy(&writeStream);
            CleanupStack::PopAndDestroy(buf);           
            }
            break;
        default:
            SendCommandToServer( aCommandId, aParam );
            break;
        }
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::CloseUI
// Close plugin layout UI.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::CloseUI()
    {
    iPenInputServer->ActivateLayout( EFalse ); 
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::ActivateUI
// Activate plugin layout UI.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::ActivateUI()
    {
    iPenInputServer->ActivateLayout( ETrue );    
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::EditorMaxLength
// Get max text length of layout UI ICF control.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPluginFepManagerBase::EditorMaxLength()
    {
    return iMaxEditorLength;
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SizeChanged
// Handle screen size changed.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::ResourceChanged(TInt aType)
    {
    iPenInputServer->ResourceChanged(aType);
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SetNextFepUI
// Set underlining UI.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::SetNextFepUI( MAknFepManagerInterface* aNextUi )
    {
    iHkbUi = aNextUi;    
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SupportLanguage
// Get support language in sepecfied mode.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPluginFepManagerBase::SupportLanguage( TInt /*aMode*/ ) const
    {
    return iLanguage;
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SetInputLanguageL
// Set current language.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::SetInputLanguageL( TLanguage aLanguage )
    {
    iLanguage = aLanguage;
    //maybe language check is needed.
    TInt lang = (TInt)aLanguage;
    ASSERT( lang >=0 );
    SendCommandToServer( ECmdPenInputLanguage, lang );       
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SetFepAwareEditorText
// Set editor text and cursor information to layout UI ICF control.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::SetFepAwareEditorText( 
    const TFepInputContextFieldData& /*aIcfData*/ )
    {
    }
    
// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SetMode
// Set layout UI mode.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
#ifdef RD_INTELLIGENT_TEXT_INPUT          
void CPluginFepManagerBase::SetMode( TInt aMode, TBool aPredictive, 
                                     TBool aQwertyInputMode, TInt aKeyboardType)
#else
void CPluginFepManagerBase::SetMode( TInt aMode, TBool aPredictive, 
                                     TBool aQwertyInputMode )
#endif 
    {
    iHkbUi->SetMode( aMode, aPredictive, aQwertyInputMode );
    }

#ifdef RD_INTELLIGENT_TEXT_INPUT
    TBool CPluginFepManagerBase::IsValidFnKeyPress() const
    {
    return EFalse;
    }
    TBool CPluginFepManagerBase::IsValidLongChrKeyPress() const
    {
    return EFalse;
    }
#endif
// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SetMode
// Set layout UI current case.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::SetCase( TCase aCase )
    {
    iHkbUi->SetCase( aCase );
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::ExpireMultitapTimer
// Compatibile with HKB UI.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::ExpireMultitapTimer()
    {
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::IsValidNumericLongKeyPress
// Compatibile with HKB UI.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPluginFepManagerBase::IsValidNumericLongKeyPress( TInt /*aKey*/ ) const
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::AddTextToUserDictionaryL
// Compatibile with HKB UI.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::AddTextToUserDictionaryL( const TDesC& /*aText*/ )
    {
    }
       
// -----------------------------------------------------------------------------
// CPluginFepManagerBase::GetFormatOfFepInlineText
// Compatibile with HKB UI.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::GetFormatOfFepInlineText( 
                                TCharFormat& /*aFormat*/, 
                                TInt& /*aNumberOfCharactersWithSameFormat*/, 
                                TInt /*aPositionOfCharacter*/ ) const
    {
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::IsValidShiftKeyPress
// Compatibile with HKB UI.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPluginFepManagerBase::IsValidShiftKeyPress() const
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SetEditorContext
// Compatibile with HKB UI.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::SetEditorContext( TInt /*aContext*/ )
    {
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SetEditorContext
// Compatibile with HKB UI.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::SetStarKeyFlag( TBool /*aSet*/ )
    {
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::CPluginFepManagerBase
// C++ default constructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPluginFepManagerBase::CPluginFepManagerBase( CPenInputImePluginItut& aOwner,
                                             RPeninputServer* aPenInputServer )
    :iOwner( aOwner ), iPenInputServer( aPenInputServer )
    {
    iSuggestedRange = ERangeInvalid;
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::BaseConstructL
// Symbian 2nd phase constructor.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::BaseConstructL()
    {
    iRepository = CRepository::NewL( KCRUidAknFep );
    }
    
// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SetLayoutPermitedRanges
// Set layout permited ranges.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::SetLayoutPermitedRanges( TInt aPermittedMode )
    {
    // Not derived from edwin
	SendCommandToServer( ECmdPenInputPermittedRange, aPermittedMode == EAknEditorNumericInputMode  );
	
	/*if ( aPermittedMode == EAknEditorNumericInputMode ) 
        {
        iPermittedRange = ERangeNumber;         
        }
    else if ( aPermittedMode & EAknEditorTextInputMode )
        {
        // Derived from edwin and EAknEditorTextInputMode
        iPermittedRange = ERangeNative | 
                          ERangeNumber | 
                          ERangeEnglish | 
                          ERangeSymbol;
        }
    else if ( aPermittedMode & EAknEditorSecretAlphaInputMode ) 
        {
        // Derived from EAknEditorSecretAlphaInputMode 
        iPermittedRange = ERangeNumber |
                          ERangeEnglish | 
                          ERangeSymbol;        
        }
    else
        {
        iPermittedRange = ERangeNative | 
                          ERangeNumber | 
                          ERangeEnglish | 
                          ERangeSymbol;
        }*/
    }
    
// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SetLayoutRange
// Set layout primary range.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::SetLayoutRange( 
    const CAknEdwinState* aEditorState )
    {
    if ( aEditorState )
	    {
	    SendCommandToServer( ECmdPenInputRange, 
	                         aEditorState->Flags() & EAknEditorFlagLatinInputModesOnly ); 
	    }
    }
    
// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SendCommandToServer
// Send command to pen input server utility function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::SendCommandToServer( TInt aCommandId )
    {
    iPenInputServer->HandleCommand( aCommandId );
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SendCommandToServer
// Send command to pen input server utility function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::SendCommandToServer( TInt aCommandId, TInt aParam )
    {
    TBuf8<sizeof(TInt)> buf;
    buf.Append( (TUint8*)&aParam, sizeof(TInt) );    
    SendCommandToServer( aCommandId, buf );
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SendCommandToServer
// Send command to pen input server utility function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::SendCommandToServer( TInt aCommandId, 
                                                 const TDesC8& aParam )
    {
    iPenInputServer->HandleCommand( aCommandId, aParam );        
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::LayoutLastUsedRange
// Get layout UI last used range.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPluginFepManagerBase::LayoutLastUsedRange()
    {
    TInt value = 0;
    if ( iRepository )
        {
        iRepository->Get( KAknFepLastUsedRange, value );
        }
        
    return value;
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SetLayoutPosition
// Set layout UI position.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::SetLayoutPosition( const TRect& pParam )
    {
    TBuf8<sizeof( TRect )> buf;
    buf.Append( (TUint8*)&pParam, sizeof( pParam ) );
    
    SendCommandToServer( ECmdPenInputSetWindowPos, buf );
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::GetLocalLanguage
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TLanguage CPluginFepManagerBase::GetLocalLanguage( 
    const CAknEdwinState* aEditorState ) const
    {
    TLanguage language = ELangTest;
    // Check for a local language override
    if ( aEditorState )
        {
        TInt aknEditorFlags = aEditorState->Flags()	;
        if ( aknEditorFlags & EAknEditorFlagLatinInputModesOnly )
            {
            language = ELangEnglish;
            }
        else
            {
            language = aEditorState->LocalLanguage();
            }
        }

    return language;
    }

 // End Of File
