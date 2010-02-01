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
* Description:  candidate stands for every cell in drop down list
*
*/


#ifndef C_CANDIDATE_H
#define C_CANDIDATE_H

#include <e32base.h>
#include <w32std.h>

// class forward decalaration
class CFont;
class MFepCtrlDropdownListContext;

/**
 *  The following class packs the info and operation of every cell
 *
 *  the class encapsulates all properties and methods of the cell which
 *  is the basic unit the drop down list
 *
 *  @lib FepCtrlDropdownList.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS(CCandidate) : public CBase
    {

public:  

    static CCandidate* NewL(const TDesC& aString,
                            MFepCtrlDropdownListContext* aOwner);

    /**
     * Destructor
     *
     * @since S60 v3.2
     */        
    virtual ~CCandidate();        

    /**
     * Get Candidate
     *
     * @since S60 v3.2
     * @param aString Candidate string
     * @return Candidate string
     */        
    const TDesC& GetCandidate();        

    /**
     * Get rectangle of the candidate
     *
     * @since S60 v3.2
     * @return Rectangle occupied by candidate string.
     */      
    const TRect& GetRect() const;


    /**
     * Move rectangle of the candidate
     *
     * @since S60 v3.2
     * @return None.
     */      
    void Move(const TPoint& aOffsetPoint);
    
    /**
     * Set position of the candidate rectangle
     *
     * @since S60 v3.2
     * @param aLeftTopPosition the left top point
     * @param aWidthForOneUnit the width of one cell
     * @param aHeight the height of one cell
     * @param aTextFont the font which will be used for draw text
     * @return the columns the cell occupies.
     */     
    TInt SetPositionL(const TPoint& aLeftTopPosition, 
                      TInt aWidthForOneUnit, 
                      TInt aHeight, 
                      const CFont* aTextFont,
                      TInt aCellHorizontalMargin);

    /**
     * draw the text for the candidate
     *
     * @since S60 v3.2
     * @param aGc graphics context
     * @param aAlignment the alighment style, can be any of ECenter, ELeft, ERight
     * @param aTextColor the color for the text
     * @param aSpecialTextColor the color for special text
     * @param aFixedText the fixed text, it has default value of KNullDesC16
     * @return the columns the cell occupies.
     */       
    virtual void DrawText(CFbsBitGc& aGc, 
                          const CGraphicsContext::TTextAlign aAlignment, 
                          const TInt aTextMargin, 
                          TRgb aTextColor, 
                          TRgb aSpecialTextColor, 
                          const TDesC& aFixedText = KNullDesC16);
    
    TInt CalculateDisplayTextL(const TInt aWidthForOneUnit, 
                               const CFont* aTextFont,
                               TInt aColNum);     
private: // Method     
    /**
     * Constructor
     *
     * @since S60 v3.2
     * @param aString Candidate string
     */        
    CCandidate(const TDesC& aString, MFepCtrlDropdownListContext* aOwner);
    void ConstructL();
    /**
     * draw the text for the candidate
     *
     * @since S60 v3.2
     * @param aWidthForOneUnit the width of one cell
     * @param aTextFont the font which will be used for drawing the text
     * @return the columns the cell occupies.
     */ 
    TInt CalculateCells(const TInt aWidthForOneUnit, 
                        const CFont* aTextFont,
                        const TDesC& aText);

//    TInt CalculateDisplayTextL(const TInt aWidthForOneUnit, 
//                               const CFont* aTextFont,
//                               TInt aColNum);

private: // Data

    /**
     * the candidate text
     */
    const TDesC& iCandidate;
    // used when candidate may be truncated
    HBufC* iDisplayText;

    /**
     * the baseline offset used for drawing the text
     */    
    TInt iBaselineOffset;             

    /**
     * the rectangle of the candidate
     */    
    TRect iRect; 
    
    /**
     * the font which will be used for drawing the text
     * Not own.
     */
    const CFont* iTextFont;
    
    // not own
    MFepCtrlDropdownListContext* iOwner;
    };

#endif // C_CANDIDATE_H

// End Of File
