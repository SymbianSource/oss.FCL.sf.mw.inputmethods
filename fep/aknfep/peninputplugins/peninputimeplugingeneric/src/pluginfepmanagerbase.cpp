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
#include <AknUtils.h> 

#include <peninputclient.h>
#include <aknfeppeninputenums.h>
#include <AknFepInternalCRKeys.h>
#include <peninputcommonlayoutglobalenum.h>

// User includes
#include "pluginfepmanagerbase.h"
#include "peninputimeplugingeneric.h"

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
            if ( iHkbMode == ENativeNumber && !SupportNativeNumberRange() )
                {
                iHkbMode = ENumber;
                }
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
            SetNumberModeKeyMappingL((TAknEditorNumericKeymap)EKeymapFromResource);
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
			// for set icf text before enter spell mode
			}
			break;
        case ECmdPenInputPopupTooltip:
            {
            TPtrC* tooltipTextPtr = reinterpret_cast<TPtrC*>( aParam );
            HBufC8* buf8 = HBufC8::NewLC
                         ( sizeof( TInt ) + tooltipTextPtr->Size() );
            TPtr8 buf8Ptr = buf8->Des();
            RDesWriteStream writeStream;
            CleanupClosePushL(writeStream);
            writeStream.Open( buf8Ptr );
            writeStream.WriteInt32L( tooltipTextPtr->Size() );
            writeStream.WriteL( tooltipTextPtr->Ptr(), tooltipTextPtr->Length() );
            writeStream.CommitL();
            CleanupStack::PopAndDestroy( &writeStream );                        
            SendCommandToServer( aCommandId, buf8Ptr );            
            CleanupStack::PopAndDestroy( buf8 );
            break;
            }
        case ECmdPenInputPopupCandidateList:
            {
            TFepITICandidateList* candidatelist 
                               = reinterpret_cast<TFepITICandidateList*>( aParam );            
            CDesCArray* itemArray = candidatelist->iItemArray;
            if ( itemArray && itemArray->Count() > 0 )
                {           
                TInt sumSize = 0;     
                for ( TInt i = 0; i < itemArray->Count(); i++ )
                    {
                    sumSize += (*itemArray)[i].Size();
                    }
                HBufC8* buf8 = HBufC8::NewLC( sizeof(TInt) // For active index
                                             + sizeof(TInt) // For the count of item array
                                             + sizeof(TInt) // for langcode
                                             + sizeof(TInt)*( itemArray->Count() ) // For the size of each item
                                             + sumSize// For all of char data 
                                             );
                TPtr8 buf8Ptr = buf8->Des();
                RDesWriteStream writeStream;
                CleanupClosePushL(writeStream);                
                writeStream.Open( buf8Ptr );                
                writeStream.WriteInt32L( candidatelist->iActiveIndex );
                writeStream.WriteInt32L( itemArray->Count() ); 
                writeStream.WriteInt32L(candidatelist->iLangCode);
                
                for ( TInt i = 0; i < itemArray->Count(); i++  )
                    {
                    writeStream.WriteInt32L( (*itemArray)[i].Size() );
                    writeStream.WriteL( (*itemArray)[i].Ptr(), (*itemArray)[i].Length() );
                    }
                writeStream.CommitL();                 
                CleanupStack::PopAndDestroy( &writeStream );
                SendCommandToServer( aCommandId, buf8Ptr );
                CleanupStack::PopAndDestroy( buf8 );
                }            
            break;
            }
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
    if (iPenInputServer->IsVisible())
	    {
    	iPenInputServer->ActivateLayout( EFalse );  
		}
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::ActivateUI
// Activate plugin layout UI.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::ActivateUI()
    {
    if(iPenInputServer)
        {
        iPenInputServer->ActivateLayout( ETrue ); 
        }    
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

TInt CPluginFepManagerBase::InputLanguage()
    {
    return iLanguage;
    }

// -----------------------------------------------------------------------------
// CPluginFepManagerBase::SetFepAwareEditorText
// Set editor text and cursor information to layout UI ICF control.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPluginFepManagerBase::SetFepAwareEditorText( 
    const TFepInputContextFieldData& aIcfData )
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
CPluginFepManagerBase::CPluginFepManagerBase( CPenInputImePluginGeneric& aOwner,
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
    iPermittedMode = aPermittedMode;
    
    // Not derived from edwin
    if ( aPermittedMode == EAknEditorNumericInputMode ) 
        {
		if ( SupportNativeNumberRange() )
		    {
		    iPermittedRange = ERangeNativeNumber;
		    }
		iPermittedRange |= ERangeNumber;
        }
    else if(aPermittedMode == (EAknEditorNumericInputMode | EAknEditorFullWidthNumericInputMode))
        {
        if ( SupportNativeNumberRange() )
            {
            iPermittedRange = ERangeNativeNumber;
            }
        iPermittedRange |= ERangeNumber;    
        }
    else if ( aPermittedMode & EAknEditorTextInputMode )
        {
        // Derived from edwin and EAknEditorTextInputMode
        iPermittedRange = ERangeNative | 
                          ERangeNumber | 
                          ERangeEnglish | 
                          ERangeSymbol |
						  ERangeNativeNumber;
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
                          ERangeSymbol |
						  ERangeNativeNumber;
        }
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
    //calculate permitted ranges
    if(aEditorState)
        {
        if( aEditorState->Flags() & EAknEditorFlagLatinInputModesOnly )
            {
            iPermittedRange &= ~ERangeNative;
			iPermittedRange &= ~ERangeNativeNumber;
            }      
        if ( iPermittedMode != EAknEditorNumericInputMode 
            && !SupportLatinRange() 
            && !( aEditorState->Flags() & EAknEditorFlagLatinInputModesOnly ) )
            {
		    iPermittedRange &= ~ERangeEnglish;
            }
        }
    else
        {
        if ( iPermittedMode == EAknEditorNumericInputMode )
            {
            if( iHkbMode == ENumber )
                {
                iPermittedRange = ERangeNumber;
                }
            else if ( iHkbMode == ENativeNumber )
                {
                iPermittedRange = ERangeNativeNumber;
                } 
            }
        else if( iPermittedMode == 
                 (EAknEditorNumericInputMode | EAknEditorFullWidthNumericInputMode))
            {
            iPermittedRange = ERangeNumber | ERangeNativeNumber;            
            }
        }

    SendCommandToServer( ECmdPenInputPermittedRange, iPermittedRange );
    
    TInt priRange;
    
    switch( iHkbMode )
        {
        case ECangJie:
        case EStrokeFind:
        case EStroke:
        case EPinyin:
        case EZhuyin:
        case EZhuyinFind:
        case EHindi:
            {
            priRange = ERangeNative;
            }
            break;                 
        case ENumber:
            {
            priRange = ERangeNumber;
            }
            break;  
        case ENativeNumber:
            {
            priRange = ERangeNativeNumber;
            }
            break;  
        case EPRCFind:
        case ELatin:
        case ELatinText:
        case ELatinUpper:
        case ELatinLower:
            if ( iLanguage == ELangHindi)
                {
                priRange = ERangeEnglish;
                }
            else
                {
                priRange = iPermittedRange & ERangeNative ? ERangeNative : ERangeEnglish;
                }
            break;
        default:
            {
            priRange = iPermittedRange & ERangeNative ? ERangeNative : ERangeEnglish;
            }
            break;               
        }
     
    if( iSuggestedRange != ERangeInvalid && (iSuggestedRange & iPermittedRange) )
        {
        priRange = iSuggestedRange;
        iSuggestedRange = ERangeInvalid;
        }
    SendCommandToServer( ECmdPenInputRange, priRange );  
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

TBool CPluginFepManagerBase::SupportLatinRange()
    {
	if ( iLanguage == ELangRussian || iLanguage == ELangBulgarian ||
         iLanguage == ELangUkrainian || iLanguage == ELangHebrew )
		 {
		 return EFalse;
		 }
		 
	if ( iLanguage == ELangGreek )
		 {
		 return EFalse;
		 }
		 
	if ( iLanguage == ELangArabic || iLanguage == ELangFarsi ||
         iLanguage == ELangUrdu || iLanguage == ELangThai )
		 {
		 return EFalse;
		 }

	if ( iLanguage == ELangHindi || iLanguage == ELangMarathi )
		 {
		 return ETrue;
		 }	
		
    return ETrue;	 		 		 
	}
	

TBool CPluginFepManagerBase::SupportNativeNumberRange()
    {
	if ( iLanguage == ELangArabic || iLanguage == ELangFarsi ||
         iLanguage == ELangUrdu || iLanguage == ELangHindi ||
         iLanguage == ELangThai )
		 {
		 return ETrue;
		 }
		
    return EFalse;	 		 		 
	}	
 // End Of File
