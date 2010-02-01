/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:               Predective text input engine definitions for Indic input
*
*/















#ifndef _PTI_INDIC_DEFS_H
#define _PTI_INDIC_DEFS_H

/**
	The new language is defined for supporting Phonetic input of Hindi, which is only a 
	writing language and is not a display language.
	*/
#define KLangHindiPhonetic	602

/**
@internal
Defines the state of the syllable. This is used by FEP for either committing 
the inline text or updating the inline text.
@see CPtiEngine::HandleCommandL
*/
enum TSyllableState
{
    /**
	Used by the phonetic core to notify FEP the start of a Syllable. This is not  used in the
	current implementation of FEP.
	*/
    EIndicSyllableStateFirst,
    /**
	Used by the phonetic core to notify FEP the continuation of a Syllable.
	*/
    EIndicSyllableStateInsideSyllable,
     /**
	Used by the phonetic core to notify FEP the break of a Syllable.
	*/
    EIndicSyllableStateSyllableBroken,
    /**
	Used by the phonetic core to notify FEP the end of a Syllable. Not used in the
	current implementation of FEP.
	*/
    EIndicSyllableStateLast
};

/**
@internal
It represents the phonetic argument and the response
from the Indic phonetic core in a structure.
@see CPtiEngine::HandleCommandL
*/

typedef struct
{
    /**
	The source character for which phonetic map is looked for.
	*/
    TChar iChar;
    /**
	The pointer to the destination buffer to which the phonetic map is copied. 
	The allocation is done by the client and also the client owns the pointer to
	the destination buffer.
	*/
    TDes *iDest;
    /**
	The syllable state after the input of the character iChar.
	*/
    TSyllableState iState;
}TPhoneticArg;


#endif // _PTI_INDIC_DEFS_H
// End of file

