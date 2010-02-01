/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:                
*
*/




#include "aknfepzhuyinkeyhandler.h"

const TInt KBTLoopLength1 = 3;
const TInt KBTLoopLength2 = 4;
const TInt KBTLoopLength3 = 5;

const TInt KBTLoopLength = 5;

const TInt KCLKCount1 = 1;
const TInt KCLKCount2 = 2;
const TInt KCLKCount3 = 3;
const TInt KCLKCount4 = 4;
const TInt KCLKCount5 = 5;

const TInt KTimerOut = 700000;

_LIT(KPtiKey1_1, "\x3105");
_LIT(KPtiKey1_2, "\x3106");
_LIT(KPtiKey1_3, "\x3107");
_LIT(KPtiKey1_4, "\x3108");

_LIT(KPtiKey2_1, "\x3109");
_LIT(KPtiKey2_2, "\x310A");
_LIT(KPtiKey2_3, "\x310B");
_LIT(KPtiKey2_4, "\x310C");

_LIT(KPtiKey3_1, "\x310D");
_LIT(KPtiKey3_2, "\x310E");
_LIT(KPtiKey3_3, "\x310F");

_LIT(KPtiKey4_1, "\x3110");
_LIT(KPtiKey4_2, "\x3111");
_LIT(KPtiKey4_3, "\x3112");

_LIT(KPtiKey5_1, "\x3113");
_LIT(KPtiKey5_2, "\x3114");
_LIT(KPtiKey5_3, "\x3115");
_LIT(KPtiKey5_4, "\x3116");

_LIT(KPtiKey6_1, "\x3117");
_LIT(KPtiKey6_2, "\x3118");
_LIT(KPtiKey6_3, "\x3119");

_LIT(KPtiKey7_1, "\x311A");
_LIT(KPtiKey7_2, "\x311B");
_LIT(KPtiKey7_3, "\x311C");
_LIT(KPtiKey7_4, "\x311D");

_LIT(KPtiKey8_1, "\x311E");
_LIT(KPtiKey8_2, "\x311F");
_LIT(KPtiKey8_3, "\x3120");
_LIT(KPtiKey8_4, "\x3121");

_LIT(KPtiKey9_1, "\x3122");
_LIT(KPtiKey9_2, "\x3123");
_LIT(KPtiKey9_3, "\x3124");
_LIT(KPtiKey9_4, "\x3125");
_LIT(KPtiKey9_5, "\x3126");

_LIT(KPtiKey0_1, "\x3127");
_LIT(KPtiKey0_2, "\x3128");
_LIT(KPtiKey0_3, "\x3129");

_LIT(KPtiKeyStar_1, "\x0020");
_LIT(KPtiKeyStar_2, "\x02CA");
_LIT(KPtiKeyStar_3, "\x02C7");
_LIT(KPtiKeyStar_4, "\x02CB");
_LIT(KPtiKeyStar_5, "\x02D9");


// ---------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------
//
ZhuyinKeyTable::ZhuyinKeyTable()
    {
    }

// ---------------------------------------------------------
// Cancel and destroy.
// ---------------------------------------------------------
//
ZhuyinKeyTable::~ZhuyinKeyTable()
    {
    }

// ---------------------------------------------------------
// Get butten loop length.
// ---------------------------------------------------------
//
TInt ZhuyinKeyTable::GetBtLoopLength ( TPtiKey aKey )
    {
    switch ( aKey )
        {
        case EPtiKey0:
        case EPtiKey3:
        case EPtiKey4:
        case EPtiKey6:
            {
            return KBTLoopLength1;
            }
        case EPtiKey1:
        case EPtiKey2:
        case EPtiKey5:
        case EPtiKey7:
        case EPtiKey8:
            {
            return KBTLoopLength2;
            }
        case EPtiKey9:
        case EPtiKeyStar:
            {
            return KBTLoopLength3;
            }
        default:
            return 0;
        }
    }

// ---------------------------------------------------------
// Get zhuyin symbol.
// ---------------------------------------------------------
//
TBool ZhuyinKeyTable::GetUnicode ( TPtiKey aKey, TInt aClkCount, TDes& aOutput )
    {
    aOutput.Zero();
    switch ( aKey )
        {
        case EPtiKey0:
            {
            switch ( aClkCount )
                {
                case KCLKCount1:
                    aOutput.Append(KPtiKey0_1);
                    return ETrue;
                case KCLKCount2:
                    aOutput.Append(KPtiKey0_2);
                    return ETrue;
                case KCLKCount3:
                    aOutput.Append(KPtiKey0_3);
                    return ETrue;
                default:
                    return EFalse;
                }
            }
        case EPtiKey1:
            switch ( aClkCount )
                {
                case KCLKCount1:
                    aOutput.Append(KPtiKey1_1);
                    return ETrue;
                case KCLKCount2:
                    aOutput.Append(KPtiKey1_2);
                    return ETrue;
                case KCLKCount3:
                    aOutput.Append(KPtiKey1_3);
                    return ETrue;
                case KCLKCount4:
                    aOutput.Append(KPtiKey1_4);
                    return ETrue;
                default:
                    return EFalse;
                }
        case EPtiKey2:
            switch ( aClkCount )
                {
                case KCLKCount1:
                    aOutput.Append(KPtiKey2_1);
                    return ETrue;
                case KCLKCount2:
                    aOutput.Append(KPtiKey2_2);
                    return ETrue;
                case KCLKCount3:
                    aOutput.Append(KPtiKey2_3);
                    return ETrue;
                case KCLKCount4:
                    aOutput.Append(KPtiKey2_4);
                    return ETrue;
                default:
                    return EFalse;
                }
        case EPtiKey3:
            {
            switch ( aClkCount )
                {
                case KCLKCount1:
                    aOutput.Append(KPtiKey3_1);
                    return ETrue;
                case KCLKCount2:
                    aOutput.Append(KPtiKey3_2);
                    return ETrue;
                case KCLKCount3:
                    aOutput.Append(KPtiKey3_3);
                    return ETrue;
                default:
                    return EFalse;
                }
            }
        case EPtiKey4:
            {
            switch ( aClkCount )
                {
                case KCLKCount1:
                    aOutput.Append(KPtiKey4_1);
                    return ETrue;
                case KCLKCount2:
                    aOutput.Append(KPtiKey4_2);
                    return ETrue;
                case KCLKCount3:
                    aOutput.Append(KPtiKey4_3);
                    return ETrue;
                default:
                    return EFalse;
                }
            }
        case EPtiKey5:
            {
            switch ( aClkCount )
                {
                case KCLKCount1:
                    aOutput.Append(KPtiKey5_1);
                    return ETrue;
                case KCLKCount2:
                    aOutput.Append(KPtiKey5_2);
                    return ETrue;
                case KCLKCount3:
                    aOutput.Append(KPtiKey5_3);
                    return ETrue;
                case KCLKCount4:
                    aOutput.Append(KPtiKey5_4);
                    return ETrue;
                default:
                    return EFalse;
                }
            }
        case EPtiKey6:
            {
            switch ( aClkCount )
                {
                case KCLKCount1:
                    aOutput.Append(KPtiKey6_1);
                    return ETrue;
                case KCLKCount2:
                    aOutput.Append(KPtiKey6_2);
                    return ETrue;
                case KCLKCount3:
                    aOutput.Append(KPtiKey6_3);
                    return ETrue;
                default:
                    return EFalse;
                }
            }
        case EPtiKey7:
            {
            switch ( aClkCount )
                {
                case KCLKCount1:
                    aOutput.Append(KPtiKey7_1);
                    return ETrue;
                case KCLKCount2:
                    aOutput.Append(KPtiKey7_2);
                    return ETrue;
                case KCLKCount3:
                    aOutput.Append(KPtiKey7_3);
                    return ETrue;
                case KCLKCount4:
                    aOutput.Append(KPtiKey7_4);
                    return ETrue;
                default:
                    return EFalse;
                }
            }
        case EPtiKey8:
            {
            switch ( aClkCount )
                {
                case KCLKCount1:
                    aOutput.Append(KPtiKey8_1);
                    return ETrue;
                case KCLKCount2:
                    aOutput.Append(KPtiKey8_2);
                    return ETrue;
                case KCLKCount3:
                    aOutput.Append(KPtiKey8_3);
                    return ETrue;
                case KCLKCount4:
                    aOutput.Append(KPtiKey8_4);
                    return ETrue;
                default:
                    return EFalse;
                }
            }
        case EPtiKey9:
            {
            switch ( aClkCount )
                {
                case KCLKCount1:
                    aOutput.Append(KPtiKey9_1);
                    return ETrue;
                case KCLKCount2:
                    aOutput.Append(KPtiKey9_2);
                    return ETrue;
                case KCLKCount3:
                    aOutput.Append(KPtiKey9_3);
                    return ETrue;
                case KCLKCount4:
                    aOutput.Append(KPtiKey9_4);
                    return ETrue;
                case KCLKCount5:
                    aOutput.Append(KPtiKey9_5);
                    return ETrue;
                default:
                    return EFalse;
                }
            }
        case EPtiKeyStar:
            {
            switch ( aClkCount )
                {
                case KCLKCount1:
                    aOutput.Append(KPtiKeyStar_1);
                    return ETrue;
                case KCLKCount2:
                    aOutput.Append(KPtiKeyStar_2);
                    return ETrue;
                case KCLKCount3:
                    aOutput.Append(KPtiKeyStar_3);
                    return ETrue;
                case KCLKCount4:
                    aOutput.Append(KPtiKeyStar_4);
                    return ETrue;
                case KCLKCount5:
                    aOutput.Append(KPtiKeyStar_5);
                    return ETrue;
                default:
                    return EFalse;
                }
            }
        default:
            return EFalse;
        }
    }

// ---------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------
//
CZhuyinKeyHandler::CZhuyinKeyHandler( ) :
    CActive ( EPriorityStandard ), // Standard priority
    iLastKey ( EPtiKeyNone ), 
    iTimeOut ( ETrue ),
    iBtClickNum ( 0 ),
    iBtLoopLength ( 1 ),
    iState( 0 ),
    iStatusChgFlag(EInit)
    {
    }

// ---------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------
//
CZhuyinKeyHandler* CZhuyinKeyHandler::NewLC()
    {
    CZhuyinKeyHandler* self = new ( ELeave ) CZhuyinKeyHandler();
    CleanupStack::PushL( self );
    self->ConstructL( );
    return self;
    }

// ---------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------
//
CZhuyinKeyHandler* CZhuyinKeyHandler::NewL()
    {
    CZhuyinKeyHandler* self = CZhuyinKeyHandler::NewLC();
    CleanupStack::Pop( self ); // self;
    return self;
    }

// ---------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------
//
void CZhuyinKeyHandler::ConstructL( )
    {
    User::LeaveIfError( iTimer.CreateLocal( ) ); // Initialize timer
    CActiveScheduler::Add( this ); // Add to scheduler
    }

// ---------------------------------------------------------
// Cancel and destroy.
// ---------------------------------------------------------
//
CZhuyinKeyHandler::~CZhuyinKeyHandler( )
    {
    Cancel( ); // Cancel any request, if outstanding
    iTimer.Close( ); // Destroy the RTimer object
    // Delete instance variables if any
    }

// ---------------------------------------------------------
// How to cancel me.
// ---------------------------------------------------------
//
void CZhuyinKeyHandler::DoCancel( )
    {
    iTimer.Cancel( );
    }

// ---------------------------------------------------------
// Function for making the initial request.
// ---------------------------------------------------------
//
void CZhuyinKeyHandler::Start()
    {
    Cancel(); // Cancel any request, just to be sure
    iTimer.After( iStatus, KTimerOut ); // Set for later
    SetActive(); // Tell scheduler a request is active
    }

// ---------------------------------------------------------
// Get Zhuyin symbol according to the inputting key.
// ---------------------------------------------------------
//
TInt CZhuyinKeyHandler::GetZhuyinSymbol ( TPtiKey aKey, TDes& aOutBuf )
    {
    TInt ret = 0;
    Start();
    // if the same button is pressed for many times.
    if ( iLastKey == aKey
        && iTimeOut == EFalse )
        {
        if ( iBtLoopLength != 0 )
            {
            iBtClickNum++;
            if ( iBtClickNum > iBtLoopLength )
                {
                iBtClickNum = 1;
                }
            }
        // Search for the corresponding unicode and value aOutBuf.
        ZhuyinKeyTable::GetUnicode(aKey,iBtClickNum,aOutBuf);
        ret = 0;
        }
    // if the pressed button is changed or time out event is happened.
    else if ( iLastKey == aKey
        && iTimeOut != EFalse )
        {
        // Update iBtClickNum
        iBtClickNum = 1;
        // Search for the corresponding unicode and value aOutBuf.
        ZhuyinKeyTable::GetUnicode(aKey,iBtClickNum,aOutBuf);
        ret = 1;
        }
    else
        {
        // Update iBtClickNum
        iBtClickNum = 1;
        // Update iBtUnicodeNum
        iBtLoopLength = ZhuyinKeyTable::GetBtLoopLength(aKey);
        // Search for the corresponding unicode and value aOutBuf.
        ZhuyinKeyTable::GetUnicode(aKey,iBtClickNum,aOutBuf);
        if ( iBtLoopLength != 0 )
            {
            ret = 1;
            }
        else
            {
            ret = -1;
            }
        }
    iLastKey = aKey;
    iTimeOut = EFalse;
    return ret;
    }

// ---------------------------------------------------------
// Set status change flag.
// ---------------------------------------------------------
//
void CZhuyinKeyHandler::SetStatusChgFlag ( MZhuyinKeyHandler::TStatusChgFlag aChgFlag )
    {
    iStatusChgFlag = aChgFlag;
    }

// ---------------------------------------------------------
// Get the status change flag.
// ---------------------------------------------------------
//
MZhuyinKeyHandler::TStatusChgFlag CZhuyinKeyHandler::GetStatusChgFlag ()
    {
    return iStatusChgFlag;
    }

// ---------------------------------------------------------
// Reset all the member variables.
// ---------------------------------------------------------
//
void CZhuyinKeyHandler::Reset()
    {
    iLastKey = EPtiKeyNone; 
    iTimeOut = ETrue ;
    iBtClickNum = 0;
    iBtLoopLength = 1;
    }


// ---------------------------------------------------------
// Get tonemark according to symbol in front of cursor.
// ---------------------------------------------------------
//
TInt CZhuyinKeyHandler::GetToneMark ( const TDes& aBaseSymbol, TDes& aOutBuf )
    {
    TInt ret = 0;
    iBtLoopLength = KBTLoopLength;
    iTimeOut = ETrue ;
    if ( aBaseSymbol.Compare(KPtiKeyStar_1) == 0 )
        {
        iBtClickNum = KCLKCount1;
        }
    else if ( aBaseSymbol.Compare(KPtiKeyStar_2) == 0 )
        {
        iBtClickNum = KCLKCount2;
        }
    else if ( aBaseSymbol.Compare(KPtiKeyStar_3) == 0 )
        {
        iBtClickNum = KCLKCount3;
        }
    else if ( aBaseSymbol.Compare(KPtiKeyStar_4) == 0 )
        {
        iBtClickNum = KCLKCount4;
        }
    else if ( aBaseSymbol.Compare(KPtiKeyStar_5) == 0 )
        {
        iBtClickNum = KCLKCount5;
        }
    else
        {
        iBtClickNum = 0;
        ret = 1;
        }
    iBtClickNum++;
    if ( iBtClickNum > iBtLoopLength )
        {
        iBtClickNum = 1;
        }
    ZhuyinKeyTable::GetUnicode(EPtiKeyStar,iBtClickNum,aOutBuf);
    iLastKey = EPtiKeyStar;
    iTimeOut = EFalse;
    return ret;
    }

// ---------------------------------------------------------
// Get tonemark according to symbol in front of cursor.
// ---------------------------------------------------------
//
void CZhuyinKeyHandler::RunL( )
    {
    iTimeOut = ETrue;
    }

// ---------------------------------------------------------
// Override to handle leaves from RunL(). Default implementation causes.
// ---------------------------------------------------------
//
TInt CZhuyinKeyHandler::RunError( TInt aError )
    {
    return aError;
    }

// ---------------------------------------------------------
// Set whether state change from entry to spelling editing is automatical.
// ---------------------------------------------------------
//
void CZhuyinKeyHandler::SetState( TInt aState )
    {
    iState = aState;
    }

// ---------------------------------------------------------
// Get state change states.
// ---------------------------------------------------------
//
TInt CZhuyinKeyHandler::GetState()
    {
    return iState;
    }

// End of file
