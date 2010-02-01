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
* Description:            Implementation of the vkb data manager
 *
*/












// system includes


// user includes
#include "AknFepZhuyinAnalyser.h"
#include "AknFepZhuyinAnalyserDb.h"

class TSpellBand
    {
public:
    int iStart;
    int iCount;
    };

class TSpellAnalysis : public TPriQueLink
    {
public:
    RArray<TInt> iSeparators;
    TSpellAnalysis( )
        {
        iPriority = -0x10000;
        }
    ~TSpellAnalysis( )
        {
        iSeparators.Close( );
        }

    TSpellAnalysis( const TSpellAnalysis& aSource )
        {
        iPriority = aSource.iPriority;
        for (TInt i = 0; i < aSource.iSeparators.Count( ); ++i )
            {
            iSeparators.Append( aSource.iSeparators[i] );
            }
        }
    void Calculate( )
        {
        iPriority = (iSeparators.Count() + 1) << 16;
        TInt multi = 12;
        for (TInt i = 0; i < iSeparators.Count(); ++i )
            {
            iPriority += ( 6 - iSeparators[i]) << multi;
            if ( multi >= 3 )
                {
                multi -= 3;
                }
            }
        iPriority = -iPriority;
        }
    TInt GetStartPos( )
        {
        if ( iSeparators.Count() == 0 )
            {
            return 0;
            }
        else
            {
            return iSeparators[ iSeparators.Count() - 1 ];
            }
        }
    void AddSeparator( TInt aPos )
        {
        iSeparators.Append( aPos );
        Calculate( );
        }
    };

// -----------------------------------------------------------------------------
// CAknFepVkbPinyinAnalyser::CAknFepVkbPinyinAnalyser
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepZhuyinAnalyser::CAknFepZhuyinAnalyser( )
    {
    iAnalysisFlag = EFalse;
    }

// -----------------------------------------------------------------------------
// CAknFepVkbPinyinAnalyser::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepZhuyinAnalyser* CAknFepZhuyinAnalyser::NewL( CZhuyinAnalyserDbFeed* aDb )
    {
    CAknFepZhuyinAnalyser* self = new ( ELeave ) CAknFepZhuyinAnalyser();
    CleanupStack::PushL( self );
    self->ConstructL( aDb );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CAknFepVkbPinyinAnalyser::~CAknFepVkbPinyinAnalyser
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAknFepZhuyinAnalyser::~CAknFepZhuyinAnalyser( )
    {
    delete []iParagraph;
    delete []iSpellLibrary;
    delete iLastResult;
    delete iDb;
    }

void CAknFepZhuyinAnalyser::ConstructL( CZhuyinAnalyserDbFeed* aDb )
    {
    ASSERT( aDb != NULL );
    iDb = aDb;

    TInt length = iDb->GetSpellCount( );

    iSpellLibrary = new(ELeave) TPtrC[length];
    for (TInt i = 0; i < length; ++i )
        {
        iSpellLibrary[i].Set( aDb->GetSpellByIndex( i ) );
        }

    TInt current = 0;
    TInt pinyinElementCount = iDb->GetSpellElementCount( );
    iParagraph = new(ELeave) TSpellBand[pinyinElementCount];
    iParagraph[current].iStart = 0;
    for (TInt i = 0; i < length; ++i )
        {
        if ( iSpellLibrary[i][0] != iDb->GetSpellElementByIndex( current ) )
            {
            iParagraph[current].iCount = i - iParagraph[current].iStart;
            current++;
            iParagraph[current].iStart = i;
            while (iSpellLibrary[i][0]
                != iDb->GetSpellElementByIndex( current ) )
                {
                iParagraph[current].iCount = 0;
                current ++;
                iParagraph[current].iStart = i;
                }
            }
        }

    iParagraph[current].iCount = length - iParagraph[current].iStart;

    }

HBufC* CAknFepZhuyinAnalyser::LastAnalysisResult( )
    {
    return iLastResult;
    }

TBool CAknFepZhuyinAnalyser::AnalyzeL( const TDesC& aInputString )
    {
    delete iLastResult;
    iLastResult = NULL;

    iLastResult = HBufC::NewL( aInputString.Length( ) * 2 );

    if ( aInputString.Length( ) < 1 )
        {
        return ETrue;
        }

    if ( aInputString.Length( ) == 1 )
        {
        iLastResult->Des().Append( aInputString[0] );
        if ( iDb->IsFirstElement( aInputString[0] ) )
            {
            return ETrue;
            }
        else
            {
            return EFalse;
            }
        }

    if ( !iDb->IsFirstElement( aInputString[0] ) )
        {
        iLastResult->Des().Append( aInputString );
        return EFalse;
        }

    int first = 0, second = 1;
    TBool seriateTonemark = EFalse;
    for (; second < aInputString.Length( ); first++, second++ )
        {
        if ( iDb->IsToneMark( aInputString[first], aInputString[second] ) )
            {
            iLastResult->Des().Append( aInputString );
            seriateTonemark = ETrue;
            break;
            }
        }

    first = 0;
    second = 1;
    int previous = 0;
    iAnalysisFlag = ETrue;
    //rough spilit
    for (; second < aInputString.Length( ) && !seriateTonemark; first++, second++ )
        {
        if ( !iDb->IsNormalElement( aInputString[first] ) )
            {
            //go to detail analysis
            if ( first > previous )
                {
                TPtrC toanalysis = aInputString.Mid( previous, first
                    - previous );
                DetailAnalyseL( toanalysis );
                }

            iLastResult->Des().Append( aInputString[first] );
            previous = first + 1;
            }
        else if ( !iDb->IsNormalElement( aInputString[second] ) )
            {
            //go to detail analysis
            if ( second > previous )
                {
                TPtrC toanalysis = aInputString.Mid( previous, second
                    - previous );
                DetailAnalyseL( toanalysis );
                }

            iLastResult->Des().Append( aInputString[second] );
            previous = second + 1;
            first ++;
            second ++;
            }
        else if ( iDb->GetSpellElementType( aInputString[first] )
            == ESpellVowel || iDb->GetSpellElementType( aInputString[second] )
            == ESpellVowel )
            {
            continue;
            }
        else if ( iDb->GetSpellElementType( aInputString[first] )
            == ESpellConsonant
            || iDb->GetSpellElementType( aInputString[second] )
                == ESpellConsonant )
            {
            //go to detail analysis
            TPtrC toanalysis = aInputString.Mid( previous, second - previous );
            DetailAnalyseL( toanalysis );
            iLastResult->Des().Append( KProgramSeparator );
            previous = second;
            }
        else
            {
            if ( (aInputString[second] == L'h'&& ( aInputString[first] == L'z' || aInputString[first] == L'c' || aInputString[first] == L's')) || //ch, sh, zh
            (aInputString[second] == L'g' && aInputString[first] == L'n') || //ng
            (aInputString[second] == L'n' && aInputString[first] == L'h') //hn 
            )
                {
                }
            else
                {
                //go to detail analysis
                TPtrC toanalysis = aInputString.Mid(previous, second - previous);
                DetailAnalyseL(toanalysis);
                iLastResult->Des().Append(KProgramSeparator);
                previous = second;
                }
            }
        }

    if( aInputString.Length() <= previous 
    		|| second> aInputString.Length() 
    		|| seriateTonemark )
        {
        return TRUE;
        }

    TPtrC toanalysis = aInputString.Mid(previous, second - previous);
    DetailAnalyseL(toanalysis);
    return iAnalysisFlag;
    }

TBool CAknFepZhuyinAnalyser::DetailAnalyseL( const TDesC& aInputString )
    {
    TPriQue<TSpellAnalysis> open;
    TSpellAnalysis* p = new(ELeave) TSpellAnalysis();

    open.Add( *p );

    int sameLength;
    int startPos;
    TBool findRes;
    TSpellAnalysis* pNext= NULL;

    while ( !open.IsEmpty( ) )
        {
        pNext = open.First( );
        pNext->Deque( );

        startPos = pNext->GetStartPos( );
        findRes = FindSpell( aInputString.Mid( startPos ), sameLength );

        if ( findRes || sameLength + startPos == aInputString.Length( ) )
            {
            //got it!
            int start = 0;
            for (TInt j = 0; j < pNext->iSeparators.Count( ); ++j )
                {
                iLastResult->Des().Append( aInputString.Mid( start, pNext->iSeparators[j]
                    - start ) );
                start = pNext->iSeparators[j];
                iLastResult->Des().Append( KProgramSeparator );
                }

            iLastResult->Des().Append( aInputString.Mid( start, aInputString.Length( )
                - start ) );
            break;
            }
        else if ( sameLength == 0 )
            {
            delete pNext;
            pNext = NULL;
            }
        else
            {
            int partLength = sameLength - 1;
            while (partLength > 0 )
                {
                int templen;
                if ( FindSpell( aInputString.Mid( startPos, partLength ),
                    templen ) )
                    {
                    TSpellAnalysis* q = new TSpellAnalysis(*pNext);
                    q->AddSeparator( startPos + partLength );
                    open.Add( *q );
                    }
                partLength --;
                }
            pNext->AddSeparator( startPos + sameLength );
            open.Add( *pNext );
            }
        }
    if ( pNext )
        {
        delete pNext;
        pNext = NULL;
        while ( !open.IsEmpty( ) )
            {
            TSpellAnalysis* p = open.First( );
            p->Deque( );
            delete p;
            }
        return true;
        }
    else
        {
        //not found
        /*
         FindSpell(aInputString, sameLength);
         int offset = 0;
         TBool rtn = sameLength > 0;
         while( sameLength > 0 )
         {
         iLastResult->Des().Append(aInputString.Mid(offset, sameLength));
         iLastResult->Des().Append(KProgramSeparator);
         offset += sameLength;
         FindSpell(aInputString.Mid(offset), sameLength);
         }
         */
        //FindSpell(aInputString, sameLength);
        int offset = 0;

        while (offset < aInputString.Length( ) )
            {
            FindSpell( aInputString.Mid( offset ), sameLength );

            if ( sameLength )
                {
                iLastResult->Des().Append( aInputString.Mid( offset, sameLength ) );
                iLastResult->Des().Append( KProgramSeparator );
                offset += sameLength;
                }
            else
                {
                iLastResult->Des().Append( aInputString.Mid( offset, 1 ) );

                iLastResult->Des().Append( KProgramSeparator );

                offset += 1;
                }
            }
        if ( iLastResult->Des()[ iLastResult->Length() - 1 ] == KProgramSeparator )
            {
            iLastResult->Des().SetLength( iLastResult->Length( ) - 1 );
            }
        iAnalysisFlag = EFalse;
        return EFalse;
        }
    }

TBool CAknFepZhuyinAnalyser::FindSpell( const TDesC& aSpell, TInt& aSameLength )
    {
    int index = aSpell[0] - iDb->GetSpellElementByIndex( 0 );

    int start = iParagraph[index].iStart;
    int end = iParagraph[index].iCount + iParagraph[index].iStart - 1;
    int mid = (start + end ) >> 1;
    int result = 0;
    aSameLength = 0;
    while (mid > start && mid < end )
        {
        result = Compare( iSpellLibrary[mid], aSpell, aSameLength );
        if ( result == 0 )
            {
            return true;
            }
        else if ( result < 0 )
            {
            start = mid;
            mid = (start + end ) >> 1;
            }
        else
            {
            end = mid;
            mid = (start + end) >> 1;
            }
        }
    int newLength;
    if ( result > 0 )
        {
        result = Compare( iSpellLibrary[start], aSpell, newLength );
        }
    else
        {
        result = Compare( iSpellLibrary[end], aSpell, newLength );
        }
    newLength > aSameLength ? aSameLength = newLength : aSameLength;

    return result == 0;
    }

TInt CAknFepZhuyinAnalyser::Compare( const TDesC& aFirst,
    const TDesC& aSecond, TInt& aSameLength )
    {
    aSameLength = 0;

    TInt length = aFirst.Length( ) > aSecond.Length( ) ? aSecond.Length( )
        : aFirst.Length( );

    for (TInt i = 0; i < length; ++i )
        {
        if ( aFirst[i] != aSecond[i] )
            {
            return aFirst[i] - aSecond[i];
            }

        aSameLength ++;
        }

    return aFirst.Length( ) - aSecond.Length( );
    }

