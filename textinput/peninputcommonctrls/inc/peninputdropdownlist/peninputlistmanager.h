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
* Description:  List manager,which is used by drop down list
*
*/


#ifndef C_LISTMANAGER_H
#define C_LISTMANAGER_H

#include <e32base.h>
#include <w32std.h>

/** class forward decalaration */
class MFepCtrlDropdownListContext;
class CList;
class CListOneRowWithoutIcon;
class CListOneRowWithoutIconWithBubble;
class CListOneRowWithIcon;
class CListOneRowWithIconWithBubble;
class CListMultiRowWithIcon;
class CListMultiRowWithIconWithBubble;
class CListMultiRowWithoutIcon;
class CListMultiRowRollWithIcon;
class CListOneRowRollWithIcon;
class CListExpandableMultiRowWithoutIcon;
class CListOneRowRollWithIconWithBubble;
class CListMultiRowRollWithIconWithBubble;

/**
 *  List manager
 *
 *  List manager used by drop down list
 *
 *  @lib FepCtrlDropdownList.lib
 *  @since S60 v3.2
 */ 
NONSHARABLE_CLASS(CListManager) : public CBase
    {
    
public:  

    /**
     * Create a object of CPages, static method
     *
     * @since S60 v3.2
     * @param aContext The context of the drop down list
     * @return CPages pointer.
     */
    static CListManager* NewL(MFepCtrlDropdownListContext* aContext);
    
    /**
     * Destructor
     *
     * @since S60 v3.2
     */     
    virtual ~CListManager();        

    /**
     * Get active list
     *
     * @since S60 v3.2
     * @return active list.
     */     
    CList* ActiveList() const;  

    /**
     * Size change
     *
     * @since S60 v3.2
     * @return None.
     */         
    void SizeChanged();

    /**
     * Move
     *
     * @since S60 v3.2
     * @param aOffset The offset of move          
     * @return None.
     */       
    void Move(const TPoint& aOffset);
    
    /**
     * Set active list
     *
     * @since S60 v3.2
     * @param aList The new active list       
     * @return None.
     */     
    void SetActiveList(CList* aList);  
               
    /**
     * Get the one row list without icon
     * @since S60 v3.2
     * @return the one row list without icon
     */
    CList* ListOneRowWithoutIcon() const;   
    
    /**
     * Get the one row list without icon with bubble
     * @since S60 v3.2
     * @return the one row list without icon
     */
    CList* ListOneRowWithoutIconWithBubble() const;  
    
    /**
     * Get the one row list with icon
     * @since S60 v3.2
     * @return the one row list with icon
     */     
    CList* ListOneRowWithIcon() const;
    
    /**
     * Get the one row list with icon with bubble
     * @since S60 v3.2
     * @return the one row list with icon
     */     
    CList* ListOneRowWithIconWithBubble() const;
    
    /**
     * Get the multi row list with icon
     * @since S60 v3.2
     * @return the multi row list with icon
     */
    CList* ListMultiRowWithIcon() const;    

    /**
     * Get the multi row list with icon with bubble
     * @since S60 v3.2
     * @return the multi row list with icon
     */
    CList* ListMultiRowWithIconWithBubble() const;
    

    /**
     * Get the multi row list without icon
     * @since S60 v3.2
     * @return the multi row list without icon
     */
    CList* ListMultiRowWithoutIcon() const;    

    CList* ListMultiRowRollWithIcon() const;
    
    CList* ListOneRowRollWithIcon() const;
    CList* ListExpandableMultiRowWithoutIcon() const;
    
    /**
     * Get the multi row roll list with icon with bubble
     * @since S60 v3.2
     * @return the multi row list with icon
     */
    CList* ListMultiRowRollWithIconWithBubble() const;
    
    /**
     * Get the one row roll list with icon with bubble
     * @since S60 v3.2
     * @return the multi row list with icon
     */
    CList* ListOneRowRollWithIconWithBubble() const;

    /**
     * Reset some data
     * @since S60 v4.0
     * @return None.
     */
    void ResetStatus();  

protected:

    /**
     * 2nd construct method
     *
     * @since S60 v3.2
     * @param aContext The context of the drop down list     
     * @return None
     */      
    void ConstructL(MFepCtrlDropdownListContext* aContext);  
    
private: // Data

    /**
     * The current active list
     * Not own
     */
    CList* iActiveList;
    
    /**
     * The one row list without icon
     * Own
     */
    CListOneRowWithoutIcon* iListOneRowWithoutIcon;

    /**
     * The one row list without icon with bubble
     * Own
     */
    CListOneRowWithoutIconWithBubble* iListOneRowWithoutIconWithBubble;
    
    /**
     * The one row list with icon
     * Own
     */
    CListOneRowWithIcon* iListOneRowWithIcon;

    /**
     * The one row list with icon with bubble
     * Own
     */
    CListOneRowWithIconWithBubble* iListOneRowWithIconWithBubble;
    
    /**
     * The multi row list with icon
     * Own
     */
    CListMultiRowWithIcon* iListMultiRowWithIcon;
    
    /**
     * The multi row list with icon with bubble
     * Own
     */
    CListMultiRowWithIconWithBubble* iListMultiRowWithIconWithBubble;

    /**
     * The multi row list without icon
     * Own
     */
    CListMultiRowWithoutIcon* iListMultiRowWithoutIcon;

    CListMultiRowRollWithIcon* iListMultiRowRollWithIcon;
    CListOneRowRollWithIcon* iListOneRowRollWithIcon;
    CListExpandableMultiRowWithoutIcon* iListExpandableMultiRowWithoutIcon;
    
    /**
     * The one row roll list with icon with bubble
     * Own
     */
    CListOneRowRollWithIconWithBubble* iListOneRowRollWithIconWithBubble;
    
    /**
     * The multi row roll list with icon with bubble
     * Own
     */
    CListMultiRowRollWithIconWithBubble* iListMultiRowRollWithIconWithBubble;

    /**
     * The dropdownlist context
     * Not Own
     */
    MFepCtrlDropdownListContext* iContext;
       
    };

#endif // C_LISTMANAGER_H

// End Of File
