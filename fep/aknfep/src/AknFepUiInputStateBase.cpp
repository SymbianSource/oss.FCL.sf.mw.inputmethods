/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStateBase methods.
*
*/












// INCLUDE FILES
#include "AknFepUiInputStateBase.h"
#include "AknFepPanic.h"

// CONSTANTS

/**
 *  TAknFepInputStateBase class.
 * 
 */
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepInputStateBase::TAknFepInputStateBase
// Constructor
// ---------------------------------------------------------------------------
//
TAknFepInputStateBase::TAknFepInputStateBase(MAknFepUIManagerStateInterface* aOwner)
    :iOwner(aOwner),
    iState(ENull),
    iData(0)
    {
    }

/**
 * MPtiObserver interface.
 */
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepInputStateBase::KeyTimerExpired
// 
// ---------------------------------------------------------------------------
//
void TAknFepInputStateBase::KeyTimerExpired()
    {
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateBase::LastWordInSelectionList
// 
// ---------------------------------------------------------------------------
//
void TAknFepInputStateBase::LastWordInSelectionList()
    {
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateBase::FirstWordInSelectionList
// 
// ---------------------------------------------------------------------------
//
void TAknFepInputStateBase::FirstWordInSelectionList()
    {
    }

/**
 *  TAknFepInputStateBase class.
 * 
 */
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepInputStateBase::HandleKeyL
// Handling Keypress events
// ---------------------------------------------------------------------------
//
TBool TAknFepInputStateBase::HandleKeyL(TInt /*aKey*/, TKeyPressLength /*aLength*/)
    {
    return EFalse;
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateBase::HandleCommandL
// Handling Command
// ---------------------------------------------------------------------------
//
void TAknFepInputStateBase::HandleCommandL(TInt /*aCommandId*/)
    {
    }
void TAknFepInputStateBase::HandleCommandL(TInt aCommandId, TInt /*aParam*/)
    {
    HandleCommandL(aCommandId);
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateBase::State
// 
// ---------------------------------------------------------------------------
//
TUIState TAknFepInputStateBase::State() const
    {
    return iState;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateJapaneseBase::CloseUI
// 
// ---------------------------------------------------------------------------
//
void TAknFepInputStateBase::CloseUI()
    {
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateJapaneseBase::InitializeStateL
// 
// ---------------------------------------------------------------------------
//
void TAknFepInputStateBase::InitializeStateL()
    {
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateBase::operator=
// 
// ---------------------------------------------------------------------------
//
void TAknFepInputStateBase::operator=(const TAknFepInputStateBase& aState)
    {
    Mem::Copy(this, &aState, sizeof(TAknFepInputStateBase));
    }

// ---------------------------------------------------------------------------
// TAknFepInputStateBase::SetStarKeyFlag
// 
// ---------------------------------------------------------------------------
//
void TAknFepInputStateBase::SetStarKeyFlag( TBool aSet )
	{
	iIsStarKeyPressed = aSet;
	}
#ifdef RD_HINDI_PHONETIC_INPUT 
// ---------------------------------------------------------------------------
// TAknFepInputStateInitialIndicPhoneticMultitap::GetPhoneticLatinChar
// 
// ---------------------------------------------------------------------------
//
void TAknFepInputStateBase::GetPhoneticLatinChar(TChar& aChar)const
	{
	}
#endif
#ifdef RD_INTELLIGENT_TEXT_INPUT	
// ---------------------------------------------------------------------------
// TAknFepInputStateBase::GetShortKeyPressTxtLength
// 
// ---------------------------------------------------------------------------
//
TInt TAknFepInputStateBase::GetShortKeyPressTxtLength()
	{
	return iShortKeyPressText.Length();
	}

// ---------------------------------------------------------------------------
// TAknFepInputStateBase::SetShortKeyPressText
// 
// ---------------------------------------------------------------------------
//
void TAknFepInputStateBase::SetShortKeyPressText(const TDesC& aText)
	{
	iShortKeyPressText.Zero();
	iShortKeyPressText.Copy(aText);
	}	
#endif // RD_INTELLIGENT_TEXT_INPUT        

// End of file
