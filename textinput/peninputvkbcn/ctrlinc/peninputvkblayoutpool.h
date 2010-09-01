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
* Description:  vkb layout pool header file
*
*/


#ifndef C_AKNFEPVKBLAYOUTPOOL_H
#define C_AKNFEPVKBLAYOUTPOOL_H

// INCLUDES
#include <e32base.h>
#include <w32std.h>
#include <barsread.h>
#include <peninputvkbctrllayout.h>

// FORWARD DECLARATION
class CAknFepCtrlVkbLayout;
class CPeninputVkbLayoutInfo;
class CAknFepVkbImLayout;

/**
 *  vkb layout pool class
 *
 *  This class is layout pool. Application can use this class to 
 *  manage their layout. Layout creates out side and add it to this 
 *  pool. Application can get and remove one layout based on layout
 *  id. 
 *
 *  @lib peninputvkbcn.lib
 *  @since S60 v3.2
 */ 
class CAknFepLayoutPool: public CBase
    {
    
public:

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @return Pointer to created CAknFepLayoutPool object
     */
    static CAknFepLayoutPool* NewL( CAknFepCtrlVkbLayout& aVkbLayout );
    
    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @return Pointer to created CAknFepLayoutPool object
     */
    static CAknFepLayoutPool* NewLC( CAknFepCtrlVkbLayout& aVkbLayout );

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CAknFepLayoutPool();        
  
    /**
     * Add one client layout to the poo 
     *
     * @since S60 v3.2
     * @param aImLayout Pointer to image layout     
     * @return None
     */ 
    void AddImLayout(CAknFepVkbImLayout* aImLayout); 

    /**
     * Remove one client layout from the pool based on image layout id 
     *
     * @since S60 v3.2
     * @param aImLayout Image layout id     
     * @return None
     */             
    void RemoveImLayout(const TInt aImLayout); 

    /**
     * Get one client layout from the pool based on image layout id 
     *
     * @since S60 v3.2
     * @param aImLayout image layout id     
     * @return Pointer to image layout
     */
    CAknFepVkbImLayout* GetImLayout(const TInt aImLayout) const;    

    /**
     * Add one vkb layout to the pooling
     *
     * @since S60 v3.2
     * @param aVkbLayout Pointer to image layout     
     * @return None
     */  
    void AddVkbLayout(CPeninputVkbLayoutInfo* aVkbLayout); 
 
    /**
     * Remove one client layout from the pool based on layout id 
     *
     * @since S60 v3.2
     * @param aVkbLayout Vkb layout id     
     * @return None
     */            
    void RemoveVkbLayout(const TInt aVkbLayout); 

    /**
     * Get one vkb layout from the pool based on layout id 
     *
     * @since S60 v3.2
     * @param aVkbLayout Vkb layout id     
     * @return Pointer to vkb layout
     */       
    CPeninputVkbLayoutInfo* GetVkbLayout(const TInt aVkbLayout) const;    
 
    /**
     * Delete all image layout from the pool  
     *
     * @since S60 v3.2
     * @return None
     */        
    void ResetImLayout();

    /**
     * Delete all vkb layout from the pool  
     *
     * @since S60 v3.2
     * @return None
     */  
    void ResetVkbLayout();

    /**
     * Delete all vkb and image layout from the pool  
     *
     * @since S60 v3.2
     * @return None
     */      
    void Reset();

    /**
     * Get vkb layout array  
     *
     * @since S60 v3.2
     * @return None
     */     
    inline RPointerArray<CPeninputVkbLayoutInfo>& VkbLayoutInfoList();

    /**
     * Get image layout array
     *
     * @since S60 v3.2
     * @return None
     */     
    inline RPointerArray<CAknFepVkbImLayout>& ImLayoutList();     
    
protected:
 
    /**
     * C++ default constructor
     *
     * @since S60 v3.2
     * @return None
     */  
    CAknFepLayoutPool();  

    /**
     * Contruct from resource
     *
     * @since S60 v3.2
     * @return None
     */    
 void ConstructFromResourceL( CAknFepCtrlVkbLayout& aVkbLayout );

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v3.2
     * @return None
     */ 
    void ConstructL( CAknFepCtrlVkbLayout& aVkbLayout );  
    
private: // Data

    /**
     * Image layout array
     */  
    RPointerArray<CAknFepVkbImLayout> iImLayoutList;
         
    /**
     * Vkb layout array
     */  
    RPointerArray<CPeninputVkbLayoutInfo> iVkbLayoutInfoList;     
    
    };

#include "peninputvkblayoutpool.inl"

#endif // C_AKNFEPVKBLAYOUTPOOL_H

// End Of File
