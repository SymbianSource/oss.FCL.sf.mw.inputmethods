/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  implementation of inline function of CTruiEngine
*
*/


// ---------------------------------------------------------------------------
// Get enum ID of the current language script
// ---------------------------------------------------------------------------
//      
inline TInt CTruiEngine::CurrentLanguageScript() const
    {
    return iCurrentScript;
    }

// ---------------------------------------------------------------------------
// Get name list of all supported character range
// ---------------------------------------------------------------------------
//      
inline CDesCArray* CTruiEngine::GetCharacterRangeName()
    {
    return iRangeName;
    }

// ---------------------------------------------------------------------------
// Get enum ID of the current character range
// ---------------------------------------------------------------------------
//      
inline TInt CTruiEngine::GetCurrentCharacterRange()
    {
    return iCurrentRange;
    }

// ---------------------------------------------------------------------------
// Get enum ID of the supported character ranges
// ---------------------------------------------------------------------------
//
inline const RArray<TInt>& CTruiEngine::GetCurrentSupportedRangesId()
    {
    return iSupportedRange;
    }
    
// ---------------------------------------------------------------------------
// Get current character set
// ---------------------------------------------------------------------------
//  
inline const HBufC* CTruiEngine::CharacterSet()
    {
    return iCharacterSet;
    }

// ---------------------------------------------------------------------------
// Get current edited shortcut
// ---------------------------------------------------------------------------
//
inline TDes& CTruiEngine::Shortcut()
    {
    return iShortcut;
    }

inline const RArray<TInt>& CTruiEngine::SupportedLanguageScript()
    {
    return iSupportedScript;
    }

// ---------------------------------------------------------------------------
// Set edit state
// ---------------------------------------------------------------------------
//
inline void CTruiEngine::NewShortcut( TBool aNew )
    {
    iNewShortcut = aNew;
    }

// ---------------------------------------------------------------------------
// Get edit state
// ---------------------------------------------------------------------------
//
inline TBool CTruiEngine::NewShortcut()
    {
    return iNewShortcut;
    }
    
// ---------------------------------------------------------------------------
// Set if display shortcut wizard view.
// ---------------------------------------------------------------------------
//
inline void CTruiEngine::SetDisplayWizard( TBool aIsDisplay )
    {
    iIsDisplay = aIsDisplay;
    }

// ---------------------------------------------------------------------------
// Get if display shortcut wizard view
// ---------------------------------------------------------------------------
//
inline TBool CTruiEngine::DisplayWizard() const
    {
    return iIsDisplay;
    }
   
// ---------------------------------------------------------------------------
// Set the current selected index
// ---------------------------------------------------------------------------
//
inline void CTruiEngine::SetCurrentSelectedIndex( TInt aSelectedIndex )
    {
    iSelectedShortcut = aSelectedIndex;
    }
    
// ---------------------------------------------------------------------------
// Get the current selected index
// ---------------------------------------------------------------------------
//
inline TInt CTruiEngine::CurrentSelectedIndex()
    {
    return iSelectedShortcut;
    }
    
// ---------------------------------------------------------------------------
// Get shortcut list
// ---------------------------------------------------------------------------
//
inline CDesCArray* CTruiEngine::ShortcutTextList()
    {
    return iShortcutList;
    }

// ---------------------------------------------------------------------------
// Get the current input language
// ---------------------------------------------------------------------------
//
inline TLanguage CTruiEngine::InputLanguage() const
    {
    return iInputLanguage;
    }
