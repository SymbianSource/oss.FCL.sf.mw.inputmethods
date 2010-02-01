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
* Description:  Model inline function for gspeninputplugin.
*
*/


inline TInt CGSPenInputModel::InputLanguageIndex() const
    {
    return iLanguageIndex;
    }

inline void CGSPenInputModel::SetInputLanguageIndex(TInt aIndex)
    {
    iLanguageIndex = aIndex;
    }

inline TBool CGSPenInputModel::ChineseLanguage()
    {
    if (iInputLanguage == ELangPrcChinese ||
        iInputLanguage == ELangHongKongChinese ||
        iInputLanguage == ELangTaiwanChinese)
        { 
        return ETrue;
        }
        
    return EFalse;    
    }
    
inline TBool CGSPenInputModel::JapaneseLanguage()
    {
    if (iInputLanguage == ELangJapanese)
        { 
        return ETrue;
        }
        
    return EFalse;    
    }    
    
inline TBool CGSPenInputModel::AvailableTrailColor(TRgb color)
    {
    if (color == KRgbBlack || color == KRgbDarkGray || color ==KRgbDarkRed ||
        color == KRgbDarkGreen || color == KRgbDarkYellow || color == KRgbDarkBlue ||
        color == KRgbDarkMagenta || color == KRgbDarkCyan || color == KRgbRed ||
        color == KRgbGreen || color == KRgbYellow || color == KRgbBlue || 
        color == KRgbMagenta || color == KRgbCyan || color == KRgbGray || color == KRgbWhite)
        {
        return ETrue;
        }
        
    return EFalse;
    }
    
inline void CGSPenInputModel::SetOwner(MGsCenRepNotifyHandlerCallback* aOwner)
    {
    iOwner = aOwner;
    }    
    
// End Of File
