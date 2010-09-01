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
* Description:  Pen input related enums
*
*/

#ifndef AKNFEPVKBPINYINANALYSER_H
#define AKNFEPVKBPINYINANALYSER_H


#include <e32base.h>



 
 
class TSpellBand;      
class CPinyinAnalyserDbFeed;

/**
 *  Composition no chars ui state
 *  It provides key event handling and internal event handling in composition no chars state
 *
 *  @lib peninputvkbcn.lib
 *  @since S60 v3.2
 */
class CAknFepVkbPinyinAnalyser : public CBase
    {
public:

    /**
     * Two-phased constructor.
     *
     * @since S60 v3.2
     * @param aDb The database file for pinyin or spell
     * @return The pointer point to CAknFepVkbPinyinAnalyser type object
     */
    static CAknFepVkbPinyinAnalyser* NewL( CPinyinAnalyserDbFeed* aDb );
        
    /**
     * Destructor.
     *
     * @since S60 v3.2
     */
    virtual ~CAknFepVkbPinyinAnalyser();
        
    /**
     * Process key event
     *
     * @since S60 v3.2
     * @param aData Carry information of key pressed
     * @return ETrue means event was responsed, otherwise EFalse
     */

        
    TBool AnalyzeL(const TDesC& aInputString);
    
    HBufC* LastAnalysisResult();
    
private:
    CAknFepVkbPinyinAnalyser();
    void ConstructL( CPinyinAnalyserDbFeed* aDb );
    TBool DetailAnalyse(const TDesC& aInputString);
    TBool FindSpell(const TDesC& aSpell, TInt& aSameLength);
    TInt  Compare(const TDesC& aFirst, const TDesC& aSecond, TInt& aSameLength);
  
private:
    TSpellBand* iParagraph;
    TPtrC*      iSpellLibrary;
    HBufC*      iLastResult;
    RArray<TInt> iSeperators;
    CPinyinAnalyserDbFeed* iDb;
    TBool   iAnalysisFlag;
    };













#endif // AKNFEPVKBPINYINANALYSER_H