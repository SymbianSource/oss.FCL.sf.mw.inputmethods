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
* Description:  Trigger string
*
*/


#ifndef __PENINPUTHWRFSCNLAFMANAGER_H__
#define __PENINPUTHWRFSCNLAFMANAGER_H__

/**
* Class  PeninputHwrfscnLAFManager
*  
* Handle the LAF data retrieving and contains a number of static functions
*
* @lib peninputhwrfscn.lib
* @since S60 v5.0
*/
class PeninputHwrfscnLAFManager
    {
public:
    /**
    * Get Control bar position
    *
    * @since S60 5.0
    * @return Control bar left top point
    */
    static TPoint ControlBarLeftTopPosition();
    
    /**
    * Get Control bar unit size
    *
    * @since S60 5.0
    * @return Control bar unit size
    */
    static TRect ControlBarUnitRect();
    
    /**
    * Get candidate unit size
    *
    * @since S60 5.0
    * @return candidate unit size
    */
    static TRect CandidateUnitRect();
    
    /**
    * Get the candidate font
    *
    * @since S60 5.0
    * @return candidate font
    */
    static const CFont* CandidateFont();
    
    /**
    * Get the scrollBar rect
    *
    * @since S60 5.0
    * @return scrollBarR rect
    */
    static TRect ScrollBarRect();
    
    /**
    * Get the candidate button down background color
    *
    * @since S60 5.0
    * @param aBtnDownColor background color
    */
    static void CandidateBtnDownBgColor( TRgb& aBtnDownColor );
    
    /**
    * Get the language text color
    *
    * @since S60 5.0
    * @param aTextColor The language text color
    */
    static void GetLangTextColor( TRgb& aTextColor );
    
    /**
    * Get the language text shadow color
    *
    * @since S60 5.0
    * @param aTextColor The language text shadow color
    */
    static void GetLangShadowTextColor( TRgb& aTextColor );
    
    /**
    * Get he language switch button shadow font
    *
    * @since S60 5.0
    * @return The font pointer for switch button shadow font
    */
    static const CFont* LangSwitchButtonShadowFont();
    
    /**
    * Get the language switch button font
    *
    * @since S60 5.0
    * @return The font pointer for switch button font
    */
    static const CFont* LangSwitchButtonFont();
    
    /**
    * Get the symbol table font
    *
    * @since S60 5.0
    * @return The font pointer for symbol table
    */
    static const CFont* SymbolTableFont();
    
    /**
    * Get the symbol char size offset used by function panel
    *
    * @since S60 5.0
    * @return The size offset 
    */
    static TSize GetSymbolTableCharSizeOffset();
    
    static TRect MainPaneRect();
    };

#endif // __PENINPUTHWRFSCNLAFMANAGER_H__

// End Of File
