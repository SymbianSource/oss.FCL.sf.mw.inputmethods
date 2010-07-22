/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for peninput server key rotator
*
*/

#include "keyrotator.h"
#include <UikonInternalPSKeys.h>
#include <PSVariables.h>
#include <aknpriv.rsg>
#include <barsread.h>
#include <barsc.h>
#include <bautils.h>
#include <w32adll.h>
#include <coemain.h>

// CONSTANTS

const TUint KAknModifiersMask = 0xFFFF0000;
// Path to wsini.ini
_LIT( KAknWsini, "z:\\system\\data\\wsini.ini" );
// Keyword for key rotator compensation.
// Syntax: S60_KEYROTATOR <value>
// where <value> is one of the following: -270, -180, -90, -0, 0, 90, 180, 270
_LIT( KAknKeyRotatorKey, "S60_KEYROTATOR" );

// Configure
//const TUint KAknRotateArrowKeys = 1;
const TUint KAknRotateInKeyboardDriver = 0;

//class CKeyRotator
    
CKeyRotator::~CKeyRotator()    
    {
    iHwRotations.Close();
    iProperty.Close();
    }

CKeyRotator* CKeyRotator::NewL()
    {
    CKeyRotator* self = new(ELeave) CKeyRotator;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
// -----------------------------------------------------------------------------
// CKeyRotator::ConstructL
// 2nd phase constructor
// -----------------------------------------------------------------------------
//
void CKeyRotator::ConstructL()
    {
    if ( KAknRotateInKeyboardDriver )
        {
        LoadHwStateRotationsL();
        }
    
    User::LeaveIfError( iProperty.Attach( KPSUidUikon, KUikLayoutState ) );
    
    TRAPD( err, iKeyRotatorCompensation = GetKeyRotatorCompensationL() );
    if ( err != KErrNone )
        {
        iKeyRotatorCompensation = 0;
        }
    }
    
//Does the reverse key rotator to CKeyRotator::DoCheckRotation
//Most of the code are copied code from CKeyRotator::DoCheckRotation in AknKeyRotator    
//
void CKeyRotator::DoReverseRotate(TRawEvent &aNewRawEvent)
    {
   
    // Current implementation is only for arrow keys
    /*
    if ( !KAknRotateArrowKeys )
        {
        return;
        }
    
    // Do not rotate external keyboard events.
    if ( aNewRawEvent.ScanCode() & EModifierKeyboardExtend )
        {
        return;
        }    
    */    
    // Also check only the arrow keys
    if ( aNewRawEvent.ScanCode() < EStdKeyLeftArrow ||
         aNewRawEvent.ScanCode() > EStdKeyDownArrow )
        {
        return;
        }
    
/*    aNewRawEvent.Set(aNewRawEvent.Type(),
                    aNewRawEvent.ScanCode() + EModifierKeyboardExtend);

    
    return;*/
    // If 'newCode' is changed something else than -1, 
    // a new event will be generated
    TInt newCode = KErrNotFound;
    
    // Check the rotation on down event. Use the same rotation for up event.
    
    // finalRotation variable at the end of this function is used to determine
    // the new scan code.
    CFbsBitGc::TGraphicsOrientation finalRotation = 
        CFbsBitGc::EGraphicsOrientationNormal;
    
    if ( aNewRawEvent.Type() == TRawEvent::EKeyUp || 
         aNewRawEvent.Type() == TRawEvent::EKeyRepeat )
        {
        // Use the same orintation for up event.
        finalRotation = iUsedRotationForDownEvent;
        }
    else // For down event, find out the rotation.
        {
        // Get SW screen rotation compared to the keyboard i.e. app orientation.
        CWsScreenDevice* device = CCoeEnv::Static()->ScreenDevice();
        TPixelsAndRotation sizeAndRotation;
        device->GetScreenModeSizeAndRotation(device->CurrentScreenMode(),sizeAndRotation);
        
        CFbsBitGc::TGraphicsOrientation swRotation = sizeAndRotation.iRotation;
            /*aAnimGeneralFunctions.ScreenDevice()->Orientation();*/
        
    
        // Get HW screen rotation
        // hardware rotate not implemented yet.
        /* 
        CFbsBitGc::TGraphicsOrientation hwRotation = 
            CFbsBitGc::EGraphicsOrientationNormal;
        TInt hwState;
        if ( KAknRotateInKeyboardDriver && 
             ( iProperty.Get(hwState) == KErrNone ) )
            {
            if ( hwState < iHwRotations.Count() )
                {
                hwRotation = iHwRotations[hwState];
                }
            }
        */
        // Calculate the difference
	    TInt finalRotationInt = swRotation*90;
	    /*
	    if ( KAknRotateInKeyboardDriver )
	        {
	        // If the rotation is also done in the driver level, 
	        // the rotation needs to be compensated so we do not 
	        // rotate twice.
	        finalRotationInt -= hwRotation*90;
	        }
	*/
        finalRotationInt += iKeyRotatorCompensation;

        // Keep the value between 0 and 270.
	    while ( finalRotationInt < 0 )
	        {
	        finalRotationInt += 360;
	        }
	    while ( finalRotationInt > 270 )
	        {
	        finalRotationInt -= 360;
	        }
	        
	    finalRotation = 
	        (CFbsBitGc::TGraphicsOrientation)( finalRotationInt / 90 );

	    iUsedRotationForDownEvent = finalRotation;
        }
	
	// Find the new scan code from the rotation.
    switch( aNewRawEvent.ScanCode() )
        {
        case EStdKeyLeftArrow:
            switch ( finalRotation )
                {
                case CFbsBitGc::EGraphicsOrientationRotated90:
                    newCode = EStdKeyUpArrow; // fixed
                    break;
                    
                case CFbsBitGc::EGraphicsOrientationRotated180:
                    newCode = EStdKeyRightArrow; //fixed
                    break;
                    
                case CFbsBitGc::EGraphicsOrientationRotated270:
                    newCode = EStdKeyDownArrow;
                    break;
                default:
                    break;
                }
            break;
        case EStdKeyDownArrow:
            switch ( finalRotation )
                {
                case CFbsBitGc::EGraphicsOrientationRotated90:
                    newCode = EStdKeyLeftArrow; //fixed
                    break;
                    
                case CFbsBitGc::EGraphicsOrientationRotated180:
                    newCode = EStdKeyUpArrow;
                    break;
                    
                case CFbsBitGc::EGraphicsOrientationRotated270:
                    newCode = EStdKeyRightArrow;
                    break;
                default:
                    break;
                }
            break;
        case EStdKeyRightArrow:
            switch ( finalRotation )
                {
                case CFbsBitGc::EGraphicsOrientationRotated90:
                    newCode = EStdKeyDownArrow; //fixed
                    break;
                    
                case CFbsBitGc::EGraphicsOrientationRotated180:
                    newCode = EStdKeyLeftArrow;
                    break;
                    
                case CFbsBitGc::EGraphicsOrientationRotated270:
                    newCode = EStdKeyUpArrow;
                    break;
                default:
                    break;
                }
            break;
        case EStdKeyUpArrow:
            switch ( finalRotation )
                {
                case CFbsBitGc::EGraphicsOrientationRotated90:
                    newCode = EStdKeyRightArrow; //fixed
                    break;
                    
                case CFbsBitGc::EGraphicsOrientationRotated180:
                    newCode = EStdKeyDownArrow;
                    break;
                    
                case CFbsBitGc::EGraphicsOrientationRotated270:
                    newCode = EStdKeyLeftArrow;
                    break;
                default:
                    break;
                }
            break;
        default:
            break;    
        }
        
    // If the 'newCode' was updated, add that value as the new scancode with existing modifiers.
    if ( newCode != KErrNotFound )
        {
        aNewRawEvent.Set(
            aNewRawEvent.Type(),
            (aNewRawEvent.ScanCode()&KAknModifiersMask) + newCode);
        }
        
    }
    
  

// -----------------------------------------------------------------------------
// CKeyRotator::GeCKeyRotatorCompensationL
// Parses wsini.ini to read key rotator compensation value.
// -----------------------------------------------------------------------------
//
TInt CKeyRotator::GetKeyRotatorCompensationL()
    {
    TInt result = 0;
    HBufC* wsiniText = GetWsiniLC();
    
    // Now look for keyword
    const TInt pos = wsiniText->Find( KAknKeyRotatorKey );
    if ( pos != KErrNotFound )
        {        
        // Keyword was found. Check that it is the beginning of line.
        // Three cases:
        // 1. Keyword could be at the beginning of the file.
        // 2. Keyword could be at the beginning of the file 
        //    after byte ordering marker.
        // 3. Previous character can be end of line marker.
        const TInt previousPos = pos - 1;
        if ( previousPos < 0 || 
             ( !previousPos && 
               IsByteOrderingMarker( (*wsiniText)[ previousPos ] ) ) || 
             IsEndOfLine( (*wsiniText)[ previousPos ] ) )
            {
            TLex text( wsiniText->Mid( pos + KAknKeyRotatorKey().Length() ) );
            
            // First, there must be at least a space after keyword.
            TBool fail = !( SkipSpaces( text ) & EAknWasSpace );
                   
            // Then follows a sequence of digits, optionally preceded by '-'.
            if ( !fail )
                {
                // Check optional -
                TBool negate = EFalse;
                if ( !text.Eos() && text.Peek() == '-' )
                    {
                    negate = ETrue;
                    text.Inc();
                    }
                    
                // Get digit sequence and convert to integer value.
                TPtrC token = GetDigits( text );
                fail = !token.Length() || 
                       ( TLex( token ).Val( result ) != KErrNone );
                
                // Handle negation
                if ( !fail && negate )
                    {
                    result = -result;
                    }
                }

            // That sequence of digits is followed by sequence of spaces until
            // end of line or end of file.
            fail = fail || ( SkipSpaces( text ) & EAknWasCharacter );
            
            // Finally, that sequence of digits must represent
            // one valid decimal value of the following: 
            // -270, -180, -90, 0, 90, 180, 270.
            fail = fail || !CheckCompensationValue( result );               
                
            // If any of above checks failed, use default value 0.
            if ( fail )
                {
                result = 0;
                }
            }
        }
        
    CleanupStack::PopAndDestroy( wsiniText );   
    return result;
    }

// -----------------------------------------------------------------------------
// CKeyRotator::IsEndOfLine
// Checks if specified character is end of line marker.
// -----------------------------------------------------------------------------
//
inline TBool CKeyRotator::IsEndOfLine( TText aChar )
    {
    return aChar == '\r' || aChar == '\n';
    }

// -----------------------------------------------------------------------------
// CKeyRotator::IsByteOrderingMarker
// Checks if specified character is byte ordering marker.
// -----------------------------------------------------------------------------
//
inline TBool CKeyRotator::IsByteOrderingMarker( TText aChar )
    {
    return aChar == 0xFEFF || aChar == 0xFFFE;
    }

// -----------------------------------------------------------------------------
// CKeyRotator::GetWsiniLC
// Reads the whole wsini.ini to memory and returns in heap descriptor.
// -----------------------------------------------------------------------------
//
HBufC* CKeyRotator::GetWsiniLC()
    {
    // Read the whole wsini.ini to memory
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );
    
    TEntry wsiniEntry;
    User::LeaveIfError( fs.Entry( KAknWsini, wsiniEntry ) );
	
    HBufC* wsiniText = HBufC::NewLC( ( wsiniEntry.iSize + 1 )/2 );
    TPtr wsiniPtr = wsiniText->Des();
    TPtr8 wsiniPtr8( (TText8*)wsiniPtr.Ptr(), 0, wsiniPtr.MaxLength()*2 );

    RFile wsiniFile;
    User::LeaveIfError( wsiniFile.Open( fs, KAknWsini, EFileRead | EFileShareReadersOnly ) );
    CleanupClosePushL( wsiniFile );
	
    User::LeaveIfError( wsiniFile.Read( wsiniPtr8, wsiniEntry.iSize ) );
    wsiniPtr.SetLength( wsiniPtr8.Length() / 2 );
	
    CleanupStack::PopAndDestroy( &wsiniFile );
    CleanupStack::Pop( wsiniText );
    CleanupStack::PopAndDestroy( &fs );
    CleanupStack::PushL( wsiniText );
    return wsiniText;
    }

// -----------------------------------------------------------------------------
// CKeyRotator::SkipSpaces
// Skips over spaces.
// -----------------------------------------------------------------------------
//
TInt CKeyRotator::SkipSpaces( TLex& aLex )
    {
    TInt flags = 0;
    // Skip spaces, but stop at end of line.
    while ( !aLex.Eos() && !IsEndOfLine( aLex.Peek() ) )
        {
        if ( aLex.Peek().IsSpace() )
            {
            // There was a space, so ok for now.
            flags |= EAknWasSpace;
            aLex.Inc();
            }
        else
            {
            flags |= EAknWasCharacter;
            break;
            }
        }
    return flags;
    }

// -----------------------------------------------------------------------------
// CKeyRotator::SkipSpaces
// Goes over digits and returns that sequence.
// -----------------------------------------------------------------------------
//
TPtrC CKeyRotator::GetDigits( TLex& aLex )
    {
    // Mark current place and go over digits.
    aLex.Mark();
    while ( !aLex.Eos() && !IsEndOfLine( aLex.Peek() ) )
        {
        if ( aLex.Peek().IsDigit() )
            {
            aLex.Inc();
            }
        else
            {
            break;
            }
        }
    return aLex.MarkedToken();
    }

// -----------------------------------------------------------------------------
// CKeyRotator::CheckCompensationValue
// Checks that value contains valid key rotator compensation value.
// -----------------------------------------------------------------------------
//
inline TBool CKeyRotator::CheckCompensationValue( TInt aValue )
    {
    // Check absolute value
    if ( aValue < 0 )
        {
        aValue = -aValue;
        }
        
    return aValue == 0 ||
           aValue == 90 ||
           aValue == 180 ||
           aValue == 270;
    }
