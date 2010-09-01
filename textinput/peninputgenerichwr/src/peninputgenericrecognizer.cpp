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
* Description:  Implementation of handwriting recognition
*
*/


// System includes
#include <PtiHwrRecognizer.h>
#include <aknfeppeninputenums.h>
#include <peninputcommonlayoutglobalenum.h>

// User includes
#include "peninputgenericrecognizer.h"


// ======== MEMBER FUNCTIONS ========

// --------------------------------------------------------------------------
// CPeninputGenericRecognizer::NewL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputGenericRecognizer* CPeninputGenericRecognizer::NewL(CPtiEngine* aPtiEngine)
    {
    CPeninputGenericRecognizer* self = 
        new ( ELeave ) CPeninputGenericRecognizer(aPtiEngine);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
  
    return self;    
    }

// --------------------------------------------------------------------------
// CPeninputGenericRecognizer::NewL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputGenericRecognizer* CPeninputGenericRecognizer::NewL( TInt aLanguage )
    {
    CPeninputGenericRecognizer* self = 
        new ( ELeave ) CPeninputGenericRecognizer( aLanguage );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;    
    }

// --------------------------------------------------------------------------
// CPeninputGenericRecognizer::CPeninputGenericRecognizer
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputGenericRecognizer::CPeninputGenericRecognizer(CPtiEngine* aPtiEngine) 
    : iPtiEngine( aPtiEngine ), iLanguage( 0 ),iMode( ERangeInvalid ),
      iCaseSensitive( EFalse ), iEnabled( EFalse ), iRecognizerInited(EFalse)
      
    {
    }

// --------------------------------------------------------------------------
// CPeninputGenericRecognizer::CPeninputGenericRecognizer
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputGenericRecognizer::CPeninputGenericRecognizer( TInt aLanguage )
    : iLanguage( aLanguage )
    {
    }
    
// --------------------------------------------------------------------------
// CPeninputGenericRecognizer::ConstructL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericRecognizer::ConstructL()
    {
    iIdle = CIdle::NewL(CActive::EPriorityIdle);
    }

// --------------------------------------------------------------------------
// CPeninputGenericRecognizer::~CPeninputGenericRecognizer
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputGenericRecognizer::~CPeninputGenericRecognizer()
    {
    delete iIdle;
    }

// --------------------------------------------------------------------------
// CPeninputGenericRecognizer::RecognizeL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TInt CPeninputGenericRecognizer::Recognize( const RArray<TPoint>& aTraceData, 
    RPointerArray<HBufC>& aResult )  
    {
    TInt err = KErrGeneral;

    if (!iRecognizerInited)
        {
      	TRAP_IGNORE(DoIdleConstructL());
        }

    if ( iHwrEngine )
        {
        aResult.ResetAndDestroy();
        err = iHwrEngine->Recognize( aTraceData, aResult );
        // Convert according the shift mode:remove when recognizer provides
        AdjustResultCase( aResult );
        }

    return err;
    }
  
// ---------------------------------------------------------------------------
// CPeninputGenericRecognizer::BackgroundConstructL
// Do background construct.
// ---------------------------------------------------------------------------
//
TInt CPeninputGenericRecognizer::BackgroundTaskL(TAny* aPtr)
    {
    CPeninputGenericRecognizer* self = static_cast<CPeninputGenericRecognizer*>(aPtr);
    self->DoIdleConstructL();
    return EFalse;
    }
    
void CPeninputGenericRecognizer::DoIdleConstructL()
    {
    iRecognizerInited = ETrue;
    InitRecognizerL();
    SetMode(iMode);	
    }
  
// --------------------------------------------------------------------------
// CPeninputGenericRecognizer::SetLanguage
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericRecognizer::SetLanguage( TInt aLanguage )
    {
    if ( iLanguage != aLanguage )
        {
        iLanguage = aLanguage;
        iRecognizerInited = EFalse;
        iHwrEngine = NULL;
        iIdle->Start(TCallBack(BackgroundTaskL,this));        
        }
    }

// --------------------------------------------------------------------------
// CPeninputGenericRecognizer::GetLanguage
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TInt CPeninputGenericRecognizer::GetLanguage( )
    {
    return iLanguage;
    }
   
// --------------------------------------------------------------------------
// CPeninputGenericRecognizer::SetCase
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericRecognizer::SetCase( TInt aCase )
    {
    if ( ( iCaseSensitive && ( iMode == ERangeNative ) ) ||
         ( iMode == ERangeEnglish ) )
        {
        TInt order = -1;
        switch ( aCase )
            {
            case ECaseUpper:
            case ECaseText:
                {
                order = ( TInt ) EUpperFirst;
                }
                break;
            case ECaseLower:
            case ECaseInverseText:
                {
                order = ( TInt ) ELowerFirst;
                }
                break;
            default:
                break;
            }
            
        if ( iHwrEngine && ( order != -1 ) )
            {
            iHwrEngine->SetFirstLetterOrder( ( TLatinLetterOrder ) order );
            }  
        
        iCase = aCase;                      
        }
    }

// --------------------------------------------------------------------------
// CPeninputGenericRecognizer::SetCaseSensitive
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericRecognizer::SetCaseSensitive( TBool aCaseSensitive )
    {
    iCaseSensitive = aCaseSensitive;
    }

// --------------------------------------------------------------------------
// CPeninputGenericRecognizer::SetMode
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
 void CPeninputGenericRecognizer::SetMode( TInt aMode )
    {
    TRecognitionRange range;

    range.iLanguage = TLanguage( iLanguage );
    switch ( aMode )
        {
        case ERangeNumber:
            {
            range.iSubRange = EPtiRangeNumber;
            }
            break;
        case ERangeSymbol:
            {
            range.iSubRange = EPtiRangeSymbol;
            }
            break;
        case ERangeEnglish:
            {
            range.iSubRange = EPtiRangeLatin;            
            }
            break;
        case ERangeNative:
            {
            range.iSubRange = EPtiRangeNative;
            }
            break;
        case ERangeNativeNumber:
            {
            range.iSubRange = EPtiRangeNativeNumber;
            }
            break;
        default:
            {
            range.iSubRange = EPtiRangeNumber;  
            }
            break;
        }

    if ( iHwrEngine )
        {
        iHwrEngine->SetRange( range );  
        }

    iMode = aMode;
    }

// --------------------------------------------------------------------------
// CPeninputGenericRecognizer::SetTopGuideLinePos
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericRecognizer::GetTopGuideLinePos( TInt& aPos )
    {
    if ( iHwrEngine )
        {
        iHwrEngine->GetTopGuideLinePos( aPos );
        }
    }

// --------------------------------------------------------------------------
// CPeninputGenericRecognizer::SetBottomGuideLinePos
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericRecognizer::GetBottomGuideLinePos( TInt& aPos )
    {
    if ( iHwrEngine )
        {
        iHwrEngine->GetBottomGuideLinePos( aPos );
        }
    }
    
// --------------------------------------------------------------------------
// CPeninputGenericRecognizer::SetBottomGuideLinePos
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericRecognizer::SetPermittedCharSet( const TDesC& /*aSymbolSet*/)
    {
    if ( iHwrEngine )
        {
        //iHwrEngine->SetPermittedSymbolSet( aSymbolSet );
        }
    }
    
// --------------------------------------------------------------------------
// CPeninputGenericRecognizer::SetInputAreaSize
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericRecognizer::SetInputAreaSize( TSize& aSize )
    {
    if ( iHwrEngine )
        {
        iHwrEngine->SetInputAreaSize( aSize );
        }
    }
    
// --------------------------------------------------------------------------
// CPeninputGenericRecognizer::EnableWordRecognition
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericRecognizer::EnableWordRecognition( const TBool aValue )
    {
    if ( iHwrEngine )
        {
        iHwrEngine->EnableWordRecognition( aValue );
        }
    iEnabled = aValue;
    }

// --------------------------------------------------------------------------
// CPeninputGenericRecognizer::SetNumberMode
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericRecognizer::SetNumberMode( 
    const TAknEditorNumericKeymap aNumberMode )
    {
    if ( iHwrEngine )
        {
        iHwrEngine->SetNumberMode( aNumberMode );
        }
    }

// --------------------------------------------------------------------------
// CPeninputGenericRecognizer::InitRecognizerL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericRecognizer::InitRecognizerL()
    {
    // Select suitable hwr recognizer according to language
    if ( iPtiEngine )
        {
        iHwrEngine = iPtiEngine->GetHwrRecognizerL( TLanguage( iLanguage ) );
        }
    }

// --------------------------------------------------------------------------
// CPeninputGenericRecognizer::AdjustResultCase
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputGenericRecognizer::AdjustResultCase( 
    RPointerArray<HBufC>& aResult )
    {
    for ( TInt i = 0; i < aResult.Count(); i++ )
        {
        HBufC* res = aResult[i];
        TPtr des = res->Des();
        
        if ( ( iCase == ECaseText ) && 
             (( iMode == ERangeEnglish ) ||
              ((iMode == ERangeNative) && 
              (iLanguage == ELangGreek || iLanguage == ELangUkrainian))))
            {
            HBufC* rep = NULL;
            
            TRAP_IGNORE( rep = HBufC::NewL( res->Length() ) );
            TPtr repp = rep->Des();
            
            TChar c;
            TBuf<1> buf;

            for ( TInt j = 0; j < res->Length(); j++ )
                {
                c = des[j];
                buf.Zero();
                buf.Append( c );
                if ( j == 0 )
                    {
                    buf.UpperCase();
                    }
                else
                    {
                    buf.LowerCase();
                    }
                repp.Append( buf );
                }
            des.Copy( *rep );
            delete rep;
            }
        }
    }
// --------------------------------------------------------------------------
// CPeninputGenericRecognizer::EnableGuideline
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TInt CPeninputGenericRecognizer::EnableGuideline(const TBool aFlag)
    {
    if(!(iLanguage == ELangThai || iLanguage == ELangVietnamese))
        {
        return KErrNotSupported;
        }
    if ( iHwrEngine )
        {
        return iHwrEngine->EnableGuideline( aFlag );
        }
    else
        {
        return KErrNotSupported;
        }
    }

//End of file
