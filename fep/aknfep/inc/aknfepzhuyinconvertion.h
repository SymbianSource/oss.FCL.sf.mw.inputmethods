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












#ifndef T_AKNFEPZHUYINCONVERTION_H
#define T_AKNFEPZHUYINCONVERTION_H

#include <e32hashtab.h>
#include <badesca.h>
#include <PtiEngine.h>
#include "AknFepZhuyinAnalyser.h"

class TZhuyinSymbolsMethods
    {
public:
    /**
     * Convert aArray to a descriptor outputed through aOutDes
     * 
     * @param aArray. the converted array.
     * @param aOutDes. an output parameter for storing descriptor after converting.
     * @return. the converted number of elements for aArray.
     */
    static TInt Convert2Des ( CDesCArrayFlat* aArray, TDes& aOutDes );
    
    /**
     * Update the content in eep control.
     * 
     * @since S60 v5.0
     * @param aPtiengine. used for getting candidate list.
     * @param aAnylyser. used for analysing validity for zhuyin symbols.
     * @param aZhuyinSymbols. zhuyin symbols' array.
     * @param aChineseDisped. chinese characters' array.
     * @param aCandidateList. an output parameter which will receive updated candidates.
     * @param aOutDes. an output parameter for storing displayed text.
     * @param aPhraseCreation. an out put parameter, ETrue for phrase creation, EFalse phrase input
     * @return. whether zhuyin symbols is valid. ETrue for valid and EFalse for invalid.
     */
    static TBool UpdateEEPContent( 
        CPtiEngine* aPtiengine,
        CAknFepZhuyinAnalyser* aAnalyser,
        CDesCArrayFlat* aZhuyinSymbols, 
        CDesCArrayFlat* aChineseDisped,
        CDesCArrayFlat* aCandidateList,
        TDes& aOutDes,
        TBool& aPhraseCreation);
    
    /**
     * Update the content in eep control.
     * 
     * @since S60 v5.0
     * @param aPtiengine. used for getting candidate list.
     * @param aAnylyser. used for analysing validity for zhuyin symbols.
     * @param aZhuyinSymbols. zhuyin symbols' array.
     * @param aChineseDisped. chinese characters' array.
     * @param aCandidateList. an output parameter which will receive updated candidates.
     * @param aOutDes. an output parameter for storing displayed text.
     * @param aPhraseCreation. an out put parameter, ETrue for phrase creation, EFalse phrase input
     * @return. whether zhuyin symbols is valid. ETrue for valid and EFalse for invalid.
     */
    static TBool UpdateEEPContentL( 
        CPtiEngine* aPtiengine,
        CAknFepZhuyinAnalyser* aAnalyser,
        CDesCArrayFlat* aZhuyinSymbols, 
        CDesCArrayFlat* aChineseDisped,
        CDesCArrayFlat* aCandidateList,
        TDes& aOutDes,
        TBool& aPhraseCreation);
    
    /**
     * commit a chinese character from candidate list.
     * After appending, the CDesCArrayFlat parameters will be updated for accordance.
     * 
     * @since S60 v5.0
     * @param aChinese. the unicode for added chinese character.
     * @param aAnalyser. the analyser for cecking the validity of aZhuyinSymbols.
     * @param aZhuyinSymbols. the zhuyin symbols appeared in the eep control.
     * @param aChineseDisped. the chinese character array aChinese should be added into.
     * @param aZhuyinSymbolsForChinese. the corresponding symbols for chinese character array.
     * @return. ETrue there are already 7 characters in eep control after appending.
     *          EFalse there are less than 7 characters in eep control.
     */
    static TBool AppendChineseChar( 
        const TDes& aChinese,
        CAknFepZhuyinAnalyser* aAnalyser,
        CDesCArrayFlat* aZhuyinSymbols, 
        CDesCArrayFlat* aChineseDisped,
        CDesCArrayFlat* aZhuyinSymbolsForChinese ); 

    /**
     * Delete a chinese character from aChineseDisped array
     * and update related array in the meantime.
     * 
     * @since S60 v5.0
     * @param aZhuyinSymbols.zhuyin symbols' array.
     * @param aInputBuf. zhuyin symbols' buf.
     * @return.ETrue for successful delete, EFalse for failed delete.
     */
    static void GetZhuyinSymbolsL(
        CDesCArrayFlat* aZhuyinSymbols,
        const TDesC& aInputBuf );
    
    /**
     * Delete a chinese character from aChineseDisped array
     * and update related array in the meantime.
     * 
     * @since S60 v5.0
     * @param aZhuyinSymbols.zhuyin symbols' array.
     * @param aChineseDisped. chinese caracters' array.
     * @param aZhuyinSymbolsForChinese. corresponding zhuyin symbol sequences for 
     * chinese caracters stored in aChineseDisped.
     * @return.ETrue for successful delete, EFalse for failed delete.
     */
    static TBool DeleteChineseChar(
        CDesCArrayFlat* aZhuyinSymbols, 
        CDesCArrayFlat* aChineseDisped,
        CDesCArrayFlat* aZhuyinSymbolsForChinese );
    
    /**
     * Delete a zhuyin symbol stored in aZhuyinSymbols array.
     * if the symbol after aKeyStroke is a tonemark, the tonemark should be deleted with zhuyin symbol.
     * 
     * @since S60 v5.0
     * @param aZhuyinSymbols.zhuyin symbols' array.
     * @param aKeyStroke. the key stroke position when deleting. 
     * @return. ETrue for success. EFalse for failure.
     */
    static TBool DeleteZhuyinSymbol(
        CDesCArrayFlat* aZhuyinSymbols, 
        TInt aKeyStroke
        );
    
    /**
     * Refresh the cursor position according to the input parameters.
     * 
     * @since S60 v5.0
     * @param aChineseNum. the number of chinese characters.
     * @param aZhuyinShown. the displayed content.
     * @param aKeyStrokePos. the key stroke position.
     * @return. the new cursor position for displayed content.
     */
    static TInt RefreshCursorPos (
        TInt aChineseNum,
        const TDes& aShownContent,
        TInt aKeyStrokePos );
    
    /**
     * Get the number of zhuyin symbol group.
     * 
     * @since S60 v5.0
     * @param aShown. the displayed content.
     * @return. the number of zhuyin symbol group.
     */
    static TInt GetChineseCount ( const TDes& aShown );
    
    /**
     * Whether the aFirstPhrase is a Zhuyin symbol or not.
     * 
     * @since S60 v5.0
     * @param aFirstPhrase.Phrase content.
     * @return. ETrue for aFirstPhrase is Zhuyin symbol. EFalse for not.
     */
    static TBool IsZhuyinSymbol( const TDesC& aFirstPhrase );
    
    /**
     * Whether is a Zhuyin spelling or not.
     * 
     * @since S60 v5.0
     * @param aSymbol the first zhuyin symbol.
     * @param index the zhuyin symbol that index of zhuyin symbol array. 
     * @return. ETrue the aSymbol and the symbol that index of zhuyin symbol
     *  array is zhuyin spelling, EFalse others.
     */
    static TBool IsZhuyinSpelling( 
        const TDesC& aSymbol,
        const TInt index,
        CDesCArrayFlat* aZhuyinSymbols );
    
    };

#endif /*T_AKNFEPZHUYINCONVERTION_H*/

//End of File

