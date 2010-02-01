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
#ifndef CMICFCUSTOMDRAW_H
#define CMICFCUSTOMDRAW_H

#include <e32base.h>
#include <frmtlay.h>

class CFepLayoutMultiLineIcfEditor;

class CFepLayoutMIcfCustomDraw : public CBase,
                                 public MFormCustomDraw
    {
public:
    CFepLayoutMIcfCustomDraw(CFepLayoutMultiLineIcfEditor* aParentControl);
    virtual ~CFepLayoutMIcfCustomDraw();

public: 
    // functions from base class MFormCustomDraw
    void DrawBackground(const TParam& aParam,
                        const TRgb& aRgb,
                        TRect& aDrawn) const;
    
    void DrawText( const TParam& aParam, const TLineInfo& aLineInfo, 
    		       const TCharFormat& aFormat, const TDesC& aText, 
    		       const TPoint& aTextOrigin,TInt aExtraPixels ) const;
    
    void DrawText( const TParam& aParam,const TLineInfo& aLineInfo,
    		       const TCharFormat& aFormat, const TDesC& aText,
    		       const TInt aStart, const TInt aEnd, 
    		       const TPoint& aTextOrigin,TInt aExtraPixels) const;
    
private:
    mutable CFepLayoutMultiLineIcfEditor* iParentControl;
    };

class CFepLayoutMIcfCustomWrap : public CBase,
                                 public MFormCustomWrap                             
    {
public:    
    TBool LineBreakPossible(TUint aPrevClass, TUint aNextClass, TBool aHaveSpaces) const;
    TUint LineBreakClass(TUint aCode,TUint& aRangeStart,TUint& aRangeEnd) const;
    };

#endif // CMICFCUSTOMDRAW_H
