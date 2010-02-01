/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
 *      Provides the TZhuyinSymbolsMethods definition 
 *
*/












#include "aknfepzhuyinconvertion.h"

_LIT(KFullStop, "\x002E");
_LIT(KToneMark0, "\x0020");
_LIT(KToneMark1, "\x02CA");
_LIT(KToneMark2, "\x02C7");
_LIT(KToneMark3, "\x02CB");
_LIT(KToneMark4, "\x02D9");

const TInt KMinZhuyinUnicode = 0x3105;
const TInt KMaxZhuyinUnicode = 0x3129;
const TInt KMaxBufLen = 100;
const TInt KMaxChineseCount = 7;

// ---------------------------------------------------------
// Convert aArray to a descriptor outputed through aOutDes.
// ---------------------------------------------------------
//
TInt TZhuyinSymbolsMethods::Convert2Des ( CDesCArrayFlat* aArray, TDes& aOutDes )
    {
    TInt bufLen = 0;
    TInt i = 0;
    for ( ; i < aArray->Count(); i++ )
        {
        bufLen = bufLen + aArray->MdcaPoint(i).Length();
        if ( bufLen > KMaxBufLen ) 
            {
            return i;
            }
        aOutDes.Append(aArray->MdcaPoint(i));
        }
    return i;
    }

// ---------------------------------------------------------
// Update the content in eep control.
// ---------------------------------------------------------
//
TBool TZhuyinSymbolsMethods::UpdateEEPContent( 
    CPtiEngine* aPtiengine,
    CAknFepZhuyinAnalyser* aAnalyser,
    CDesCArrayFlat* aZhuyinSymbols, 
    CDesCArrayFlat* aChineseDisped,
    CDesCArrayFlat* aCandidateList,
    TDes& aOutDes,
    TBool& aPhraseCreation)
    {
    TBool rnt = EFalse;
    TRAPD( ret, rnt = UpdateEEPContentL( aPtiengine, 
                                         aAnalyser, 
                                         aZhuyinSymbols, 
                                         aChineseDisped, 
                                         aCandidateList, 
                                         aOutDes, 
                                         aPhraseCreation ) );
    if ( KErrNone != ret )
        {
        return EFalse;
        }
    
    return rnt;
    }

// ---------------------------------------------------------
// clean.
// ---------------------------------------------------------
//
void CleanArray( TAny* aAny )
    {
    RPointerArray<HBufC>* implArray = 
        reinterpret_cast< RPointerArray<HBufC>*> ( aAny );
    implArray->ResetAndDestroy();
    implArray->Close();
    }


// ---------------------------------------------------------
// Update the content in eep control.
// ---------------------------------------------------------
//
TBool TZhuyinSymbolsMethods::UpdateEEPContentL( 
    CPtiEngine* aPtiengine,
    CAknFepZhuyinAnalyser* aAnalyser,
    CDesCArrayFlat* aZhuyinSymbols, 
    CDesCArrayFlat* aChineseDisped,
    CDesCArrayFlat* aCandidateList,
    TDes& aOutDes,
    TBool& aPhraseCreation)
    {
    TBuf<KMaxBufLen> buf;
    Convert2Des( aZhuyinSymbols, buf );
    RPointerArray<HBufC> list;
    TCleanupItem cleanitem( CleanArray, &list );
    CleanupStack::PushL( cleanitem );
    
    HBufC* retString = aPtiengine->GetCandidatesByInputString( buf, list,
        EFalse );
    if ( buf.Length() == 0 )
        {
        Convert2Des( aChineseDisped, aOutDes );
        delete retString;
        aCandidateList->Reset();
        aCandidateList->Compress();
        CleanupStack::PopAndDestroy();
        return ETrue;
        }
    
    CleanupStack::PushL( retString );
    TBool valid = aAnalyser->AnalyzeL( buf );
       
    HBufC* showbuf = aAnalyser->LastAnalysisResult();
    aCandidateList->Reset();
    if ( retString->Compare( buf ) != 0 || 
        retString->Compare( buf ) == 0 && aChineseDisped->Count() != 0 )
        {
        // Display the candidates for the 1st chinese character group.
        aPhraseCreation = ETrue;
        list.ResetAndDestroy();
        TInt length = 0;
        TInt i = 0;
        for ( i = 0; i < showbuf->Length(); i++ )
            {
            if ( showbuf->Mid(i, 1).Compare(KFullStop) == 0 )
                {
                length = i;
                break;
                }
            if ( showbuf->Mid(i, 1).Compare(KToneMark0) == 0 
                || showbuf->Mid(i, 1).Compare(KToneMark1) == 0 
                || showbuf->Mid(i, 1).Compare(KToneMark2) == 0
                || showbuf->Mid(i, 1).Compare(KToneMark3) == 0
                || showbuf->Mid(i, 1).Compare(KToneMark4) == 0 )
                {
                length = i + 1;
                break;
                }
            }
        if ( i == showbuf->Length())
            {
            length = i;
            }
        CleanupStack::PopAndDestroy( retString );
        TPtrC tmp = showbuf->Left( length );
        retString = aPtiengine->GetCandidatesByInputString( tmp, list, EFalse );
        CleanupStack::PushL( retString );
        for (TInt ii = 0; ii < list.Count(); ii++ )
            {
            aCandidateList->AppendL( *(list[ii]) );
            }
        }
    else
        {
        aPhraseCreation = EFalse;
        for (TInt i = 0; i < list.Count(); i++ )
            {
            aCandidateList->AppendL( *(list[i]) );
            }
        }
    CleanupStack::Pop( retString );
    CleanupStack::PopAndDestroy();
    Convert2Des( aChineseDisped, aOutDes );
    if ( !valid )
        {
        aOutDes.Append(buf);
        }
    else
        aOutDes.Append(*showbuf);
    delete retString;
    
    if ( aZhuyinSymbols->Count() > 0
        && ( aZhuyinSymbols->MdcaPoint(0).Compare(KToneMark0) == 0 
        || aZhuyinSymbols->MdcaPoint(0).Compare(KToneMark1) == 0 
        || aZhuyinSymbols->MdcaPoint(0).Compare(KToneMark2) == 0
        || aZhuyinSymbols->MdcaPoint(0).Compare(KToneMark3) == 0
        || aZhuyinSymbols->MdcaPoint(0).Compare(KToneMark4) == 0 ) )
        {
        valid = EFalse;
        }
    
    if ( aCandidateList->Count() != 0 && aOutDes.Length() == 1 )
        {
        valid = ETrue;
        }
    return valid;
    }

// ---------------------------------------------------------
// Commit a chinese character from candidate list.
// ---------------------------------------------------------
//
TBool TZhuyinSymbolsMethods::AppendChineseChar( 
    const TDes& aChinese,
    CAknFepZhuyinAnalyser* aAnalyser,
    CDesCArrayFlat* aZhuyinSymbols, 
    CDesCArrayFlat* aChineseDisped,
    CDesCArrayFlat* aZhuyinSymbolsForChinese )
    {
    TBuf<KMaxBufLen> buf;
    buf.Copy(aChinese);
    if ( aChineseDisped->Count() >= KMaxChineseCount )
        {
        return ETrue;
        }
    buf.Zero();
    Convert2Des(aZhuyinSymbols, buf);
    TBool valid = EFalse;
    TRAPD( err, valid = aAnalyser->AnalyzeL( buf ) );
    if ( KErrNone != err )
        {
        return EFalse;
        }
    
    if ( !valid )
        {
        return EFalse;
        }
    else
        {
        HBufC* showbuf = aAnalyser->LastAnalysisResult();
        TInt length = 0;
        TInt i = 0;
        for ( i = 0; i < showbuf->Length(); i++ )
            {
            if ( showbuf->Mid(i, 1).Compare(KFullStop) == 0 )
                {
                length = i;
                break;
                }
            if ( showbuf->Mid(i, 1).Compare(KToneMark0) == 0 
                || showbuf->Mid(i, 1).Compare(KToneMark1) == 0 
                || showbuf->Mid(i, 1).Compare(KToneMark2) == 0
                || showbuf->Mid(i, 1).Compare(KToneMark3) == 0
                || showbuf->Mid(i, 1).Compare(KToneMark4) == 0 )
                {
                length = i + 1;
                break;
                }
            }
        if ( i == showbuf->Length())
            {
            length = i;
            }
        // update arrays
        TRAPD( ret, aChineseDisped->AppendL(aChinese) );
        if ( KErrNone != ret )
            {
            return EFalse;
            }
        
        TBuf<KMaxBufLen> tmpbuf;
        for ( TInt ii = 0; ii < length; ii++ )
            {
            tmpbuf.Append(aZhuyinSymbols->MdcaPoint(ii));
            }
        TRAPD( ret1, aZhuyinSymbolsForChinese->AppendL(tmpbuf) );
        if ( KErrNone != ret1 )
            {
            return EFalse;
            }
        aZhuyinSymbols->Delete(0, length);
        if ( aChineseDisped->Count() >= KMaxChineseCount 
            || aZhuyinSymbols->Count() == 0 )
            {
            return ETrue;
            }
        else
            {
            return EFalse;
            }
        }
    }

// ---------------------------------------------------------
// append the zhuyin symbol to the zhuyin array form zhuyin buf.
// ---------------------------------------------------------
//
void TZhuyinSymbolsMethods::GetZhuyinSymbolsL( 
    CDesCArrayFlat* aZhuyinSymbols,  
    const TDesC& aInputBuf )
    {
    for ( TInt i = 0; i < aInputBuf.Length(); i++ )
        {
        aZhuyinSymbols->InsertL(i, aInputBuf.Mid(i,1));
        }
    }

// ---------------------------------------------------------
// Delete a chinese character from aChineseDisped array
// and update related array in the meantime.
// ---------------------------------------------------------
//
TBool TZhuyinSymbolsMethods::DeleteChineseChar(
    CDesCArrayFlat* aZhuyinSymbols, 
    CDesCArrayFlat* aChineseDisped,
    CDesCArrayFlat* aZhuyinSymbolsForChinese )
    {
    if ( aChineseDisped->Count() == 0 )
        {
        return EFalse;
        }
    TBuf<KMaxBufLen> buf;
    buf.Append( aZhuyinSymbolsForChinese->MdcaPoint( 
        aZhuyinSymbolsForChinese->Count() - 1 ) );
    
    TRAPD( ret, GetZhuyinSymbolsL( aZhuyinSymbols, buf ) );
    if ( KErrNone != ret )
        {
        return EFalse;
        }
    
    aChineseDisped->Delete(aChineseDisped->Count() - 1);
    aZhuyinSymbolsForChinese->Delete(aZhuyinSymbolsForChinese->Count() - 1);
    return ETrue;
    }

// ---------------------------------------------------------
// Delete a chinese character from aChineseDisped array
// and update related array in the meantime.
// ---------------------------------------------------------
//
TBool TZhuyinSymbolsMethods::DeleteZhuyinSymbol(
    CDesCArrayFlat* aZhuyinSymbols, 
    TInt aKeyStroke
    )
    {
    TInt delCount = 0;
    if ( aKeyStroke == aZhuyinSymbols->Count() )
        {
        delCount++;
        }
    else if ( aKeyStroke < aZhuyinSymbols->Count() )
        {
        delCount++;
        }
    else
        {
        return EFalse;
        }
    aZhuyinSymbols->Delete( aKeyStroke-1, delCount );
    return ETrue;
    }

// ---------------------------------------------------------
// Refresh the cursor position according to the input parameters.
// ---------------------------------------------------------
//
TInt TZhuyinSymbolsMethods::RefreshCursorPos (
    TInt aChineseNum,
    const TDes& aShownContent,
    TInt aKeyStrokePos )
    {
    TInt cursorPos = aChineseNum;
    TInt index = 0;
    TInt count = 0;
    if ( aShownContent.Length() - aChineseNum <= 0 )
        {
        return aChineseNum;
        }
    
    if ( 0 == aKeyStrokePos )
        {
        return aChineseNum;
        }
    
    TPtrC zhuyinShown = aShownContent.Right( aShownContent.Length() - 
        aChineseNum );
    
    if ( zhuyinShown.Length() == 0 )
        {
        return aChineseNum;
        }
    
    for ( TInt i = 0; i < zhuyinShown.Length(); ++i )
        {
        if ( 0 != zhuyinShown.Mid( i, 1 ).Compare( KFullStop ) )
            {
            count = count + 1;
            }
        if ( aKeyStrokePos == count )
            {
            index = i + 1;
            break;
            }
        }

    return cursorPos + index;
    
    }

// ---------------------------------------------------------
// Get the number of zhuyin symbol group.
// ---------------------------------------------------------
//
TInt TZhuyinSymbolsMethods::GetChineseCount(const TDes& aShown)
    {
    TInt i = 0;
    TInt count = 0;
    for ( i = 0; i < aShown.Length(); i++ )
        {
        if ( aShown.Mid(i, 1).Compare(KFullStop) == 0
            || aShown.Mid(i, 1).Compare(KToneMark0) == 0 
            || aShown.Mid(i, 1).Compare(KToneMark1) == 0 
            || aShown.Mid(i, 1).Compare(KToneMark2) == 0
            || aShown.Mid(i, 1).Compare(KToneMark3) == 0
            || aShown.Mid(i, 1).Compare(KToneMark4) == 0 )
            {
            count++;
            }
        }
    if ( aShown.Right(1).Compare(KToneMark0) == 0 
        || aShown.Right(1).Compare(KToneMark1) == 0 
        || aShown.Right(1).Compare(KToneMark2) == 0
        || aShown.Right(1).Compare(KToneMark3) == 0
        || aShown.Right(1).Compare(KToneMark4) == 0 )
        {
        return count;
        }
    else
        {
        return count + 1;
        }
    }

// ---------------------------------------------------------
// Whether the aFirstPhrase is a Zhuyin symbol or not.
// ---------------------------------------------------------
//
TBool TZhuyinSymbolsMethods::IsZhuyinSymbol( const TDesC& aFirstPhrase )
    {
    if ( aFirstPhrase.Length() > 1 )
        {
        return EFalse;
        }
    
    if ( aFirstPhrase[0] >= KMinZhuyinUnicode &&
         aFirstPhrase[0] <= KMaxZhuyinUnicode )
        {
        return ETrue;
        }
    else
        {
        return EFalse;
        }
    }

// ---------------------------------------------------------
// Whether the aFirstPhrase is a Zhuyin symbol or not.
// ---------------------------------------------------------
//
TBool TZhuyinSymbolsMethods::IsZhuyinSpelling(
                                    const TDesC& /*aSymbol*/,
                                    const TInt /*index*/,
                                    CDesCArrayFlat* /*aZhuyinSymbols*/ ) 
    {
    return ETrue;
    }

// End of file
