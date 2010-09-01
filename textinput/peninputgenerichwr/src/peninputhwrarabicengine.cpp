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
#include "peninputhwrarabicengine.h"


// ======== MEMBER FUNCTIONS ========

// --------------------------------------------------------------------------
// CPeninputHwrBxAbEngine::NewL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputHwrBxAbEngine* CPeninputHwrBxAbEngine::NewL(CPtiEngine* aPtiEngine)
    {
    CPeninputHwrBxAbEngine* self = 
        new ( ELeave ) CPeninputHwrBxAbEngine(aPtiEngine);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
  
    return self;    
    }

// --------------------------------------------------------------------------
// CPeninputHwrBxAbEngine::NewL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputHwrBxAbEngine* CPeninputHwrBxAbEngine::NewL( TInt aLanguage )
    {
    CPeninputHwrBxAbEngine* self = 
        new ( ELeave ) CPeninputHwrBxAbEngine( aLanguage );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;    
    }

// --------------------------------------------------------------------------
// CPeninputHwrBxAbEngine::CPeninputHwrBxAbEngine
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputHwrBxAbEngine::CPeninputHwrBxAbEngine(CPtiEngine* aPtiEngine) 
    : iPtiEngine( aPtiEngine ), iLanguage( 0 ),iMode( ERangeInvalid ),
      iCaseSensitive( EFalse ), iEnabled( EFalse ), iRecognizerInited(EFalse)
      
    {
    }

// --------------------------------------------------------------------------
// CPeninputHwrBxAbEngine::CPeninputHwrBxAbEngine
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputHwrBxAbEngine::CPeninputHwrBxAbEngine( TInt aLanguage )
    : iLanguage( aLanguage )
    {
    }
    
// --------------------------------------------------------------------------
// CPeninputHwrBxAbEngine::ConstructL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbEngine::ConstructL()
    {
    iIdle = CIdle::NewL(CActive::EPriorityIdle);
    }

// --------------------------------------------------------------------------
// CPeninputHwrBxAbEngine::~CPeninputHwrBxAbEngine
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
CPeninputHwrBxAbEngine::~CPeninputHwrBxAbEngine()
    {
    delete iIdle;
    }

// --------------------------------------------------------------------------
// CPeninputHwrBxAbEngine::RecognizeL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TInt CPeninputHwrBxAbEngine::Recognize( const RArray<TPoint>& aTraceData, 
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
// CPeninputHwrBxAbEngine::BackgroundConstructL
// Do background construct.
// ---------------------------------------------------------------------------
//
TInt CPeninputHwrBxAbEngine::BackgroundTaskL(TAny* aPtr)
    {
    CPeninputHwrBxAbEngine* self = static_cast<CPeninputHwrBxAbEngine*>(aPtr);
    self->DoIdleConstructL();
    return EFalse;
    }
    
void CPeninputHwrBxAbEngine::DoIdleConstructL()
    {
    iRecognizerInited = ETrue;
    InitRecognizerL();
    SetMode(iMode); 
    }
  
// --------------------------------------------------------------------------
// CPeninputHwrBxAbEngine::SetLanguage
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbEngine::SetLanguage( TInt aLanguage )
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
// CPeninputHwrBxAbEngine::GetLanguage
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TInt CPeninputHwrBxAbEngine::GetLanguage( )
    {
    return iLanguage;
    }
   
// --------------------------------------------------------------------------
// CPeninputHwrBxAbEngine::SetCase
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbEngine::SetCase( TInt aCase )
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
// CPeninputHwrBxAbEngine::SetCaseSensitive
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbEngine::SetCaseSensitive( TBool aCaseSensitive )
    {
    iCaseSensitive = aCaseSensitive;
    }

// --------------------------------------------------------------------------
// CPeninputHwrBxAbEngine::SetMode
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
 void CPeninputHwrBxAbEngine::SetMode( TInt aMode )
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
// CPeninputHwrBxAbEngine::SetTopGuideLinePos
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbEngine::GetTopGuideLinePos( TInt& aPos )
    {
    if ( iHwrEngine )
        {
        iHwrEngine->GetTopGuideLinePos( aPos );
        }
    }

// --------------------------------------------------------------------------
// CPeninputHwrBxAbEngine::SetBottomGuideLinePos
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbEngine::GetBottomGuideLinePos( TInt& aPos )
    {
    if ( iHwrEngine )
        {
        iHwrEngine->GetBottomGuideLinePos( aPos );
        }
    }
    
// --------------------------------------------------------------------------
// CPeninputHwrBxAbEngine::SetBottomGuideLinePos
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbEngine::SetPermittedCharSet( const TDesC& /*aSymbolSet*/)
    {
    if ( iHwrEngine )
        {
        //iHwrEngine->SetPermittedSymbolSet( aSymbolSet );
        }
    }
    
// --------------------------------------------------------------------------
// CPeninputHwrBxAbEngine::SetInputAreaSize
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbEngine::SetInputAreaSize( TSize& aSize )
    {
    if ( iHwrEngine )
        {
        iHwrEngine->SetInputAreaSize( aSize );
        }
    }
    
// --------------------------------------------------------------------------
// CPeninputHwrBxAbEngine::EnableWordRecognition
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbEngine::EnableWordRecognition( const TBool aValue )
    {
    if ( iHwrEngine )
        {
        iHwrEngine->EnableWordRecognition( aValue );
        }
    iEnabled = aValue;
    }

// --------------------------------------------------------------------------
// CPeninputHwrBxAbEngine::SetNumberMode
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbEngine::SetNumberMode( 
    const TAknEditorNumericKeymap aNumberMode )
    {
    if ( iHwrEngine )
        {
        iHwrEngine->SetNumberMode( aNumberMode );
        }
    }

// --------------------------------------------------------------------------
// CPeninputHwrBxAbEngine::InitRecognizerL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbEngine::InitRecognizerL()
    {
    // Select suitable hwr recognizer according to language
    if ( iPtiEngine )
        {
        iHwrEngine = iPtiEngine->GetHwrRecognizerL( TLanguage( iLanguage ) );
        }
    }

// --------------------------------------------------------------------------
// CPeninputHwrBxAbEngine::AdjustResultCase
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CPeninputHwrBxAbEngine::AdjustResultCase( 
    RPointerArray<HBufC>& aResult )
    {
    for ( TInt i = 0; i < aResult.Count(); i++ )
        {
        HBufC* res = aResult[i];
        TPtr des = res->Des();
        
        if ( ( iCase == ECaseText ) && 
             ( iMode == ERangeEnglish ))
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
//End of File
