/*
* Copyright (c) 2200 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Pages manager,which is used by drop down list
*
*/


#ifndef C_PAGES_H
#define C_PAGES_H

#include <e32base.h>
#include <w32std.h>

/**
 *  Pages manager
 *
 *  Pages manager used by drop down list
 *
 *  @lib FepCtrlDropdownList.lib
 *  @since S60 v3.2
 */ 
NONSHARABLE_CLASS(CPages) : public CBase
    {
    
public:  

    /**
     * Create a object of CPages, static method
     *
     * @since S60 v3.2
     * @return CPages pointer.
     */
    static CPages* NewL();
    
    /**
     * Destructor
     *
     * @since S60 v3.2
     */     
    virtual ~CPages();        
 
    /**
     * Add one page into the page list
     *
     * @since S60 v3.2
     * @param aFirstIndex The first index of the page
     * @param aLastIndex The last index of the page    
     * @return None.
     */
    void AddPage(const TInt aFirstIndex, const TInt aLastIndex); 
           
    /**
     * Get previous index according the appointed last index
     *
     * @since S60 v3.2
     * @param aLastIndex The current last index 
     * @param aFirstIndexOfPrevious Returned first index of previous page
     * @param aLastIndexOfPrevious Returned last index of previous page
     * @return The find result, ETrue if success, else EFalse.
     */
    TBool GetPreviousIndex(const TInt aLastIndex, 
                           TInt& aFirstIndexOfPrevious, 
                           TInt& aLastIndexOfPrevious) const;
    
    /**
     * Reset the pages, clear all pages
     *
     * @since S60 v3.2
     * @return None.
     */
    void Reset();
    
    /**
     * Get page count in the list
     *
     * @since S60 v3.2
     * @return Page count.
     */
    TInt PageCount() const;    
    
    /**
     * Get page by appointed postion.
     *
     * @since S60 v3.2
     * @param aPos The appointed postion
     * @param aFirstIndex Returned first index of page
     * @param aLastIndex Returned last index of page
     * @return The find result, ETrue if success, else EFalse.
     */
    TBool GetPageIndexByPos(const TInt aPos, 
                            TInt& aFirstIndex, 
                            TInt& aLastIndex) const;
    
protected:

    /**
     * 2nd construct method
     *
     * @since S60 v3.2
     * @return None
     */      
    void ConstructL();  
    
private: // Data

    /**
     * the list for first indexes 
     */  
    RArray<TInt> iFirstIndexList;
    
    /**
     * the list for last indexes 
     */  
    RArray<TInt> iLastIndexList;
      
    };

#endif // C_PAGES_H

// End Of File
