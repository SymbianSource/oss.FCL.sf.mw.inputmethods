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
* Description:  Defination for peninput server key rotator
*
*/


#ifndef __PENINPUT_KEY_ROTATOR__
#define __PENINPUT_KEY_ROTATOR__
#include <e32base.h>
#include <e32event.h>
#include <e32property.h>
#include <bitstd.h>
//class CKeyRotator
class CKeyRotator : public CBase
    {
    public:
        static CKeyRotator* NewL();
        void DoReverseRotate(TRawEvent &aEvent);
        void DoRatate(TRawEvent &aEvent);
        ~CKeyRotator();
    private:
        void ConstructL();
        void LoadHwStateRotationsL();
       
        // Gets key rotator compensation value from wsini.ini
        static TInt GetKeyRotatorCompensationL();
        
        // Checks if character is end of line marker.
        inline static TBool IsEndOfLine( TText aChar );
        // Checks if character is byte ordering marker.
        inline static TBool IsByteOrderingMarker( TText aChar );
        // Reads wsini.ini file to memory
        static HBufC* GetWsiniLC();
        // Flags - returned by SkipSpaces
        enum TAknSkipSpacesFlags
            {
            // There was a space character (not end of line)
            EAknWasSpace = 1,
            // There was other kind of character
            EAknWasCharacter = 2
            };
        // Skips spaces, but stops if end of line.
        static TInt SkipSpaces( TLex& aLex );
        // Goes over digit sequence and returns string.
        static TPtrC GetDigits( TLex& aLex );
        // Checks if compensation value is ok.
        inline static TBool CheckCompensationValue( TInt aValue );

    private:
        RProperty iProperty; 
        RArray<CFbsBitGc::TGraphicsOrientation> iHwRotations;
        CFbsBitGc::TGraphicsOrientation iUsedRotationForDownEvent;
        
        // Key rotator compensation value.
        TInt iKeyRotatorCompensation;

    };

#endif //__PENINPUT_KEY_ROTATOR__