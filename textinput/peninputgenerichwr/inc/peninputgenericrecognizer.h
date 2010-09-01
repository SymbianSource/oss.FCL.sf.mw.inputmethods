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
* Description:  peninput generic hwr handwriting recognition class
*
*/


#ifndef C_PENINPUTGENERICRECOGNIZER_H
#define C_PENINPUTGENERICRECOGNIZER_H

// System includes
#include <e32base.h>
#include <PtiEngine.h>
#include <PtiHwrRecognizer.h>

// Forward declarations
class CPtiEngine;

/**
 *  This class uses ptiengine to recognize handwriting
 *
 *  @lib peninputgenerichwr.lib
 *  @since S60 v3.2
 */
class CPeninputGenericRecognizer : public CBase
  {

  public: 
    
    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @return The pointer to CPeninputGenericRecognizer object
     */
    static CPeninputGenericRecognizer* NewL(CPtiEngine* aPtiEngine);
  
    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @return The pointer to CPeninputGenericRecognizer object
     */
    static CPeninputGenericRecognizer* NewL( TInt aLanguage );
  
    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CPeninputGenericRecognizer();    
      
    /**
      * Do the recognition using the PtiEngine
      *
      * @since S60 v3.2
      * @param aTraceData The point array of curve
      * @param aResult The result of recognition
      * @return Error code
      */
    TInt Recognize( const RArray<TPoint>& aTraceData, 
                     RPointerArray<HBufC>& aResult );
      
    /**
     * Set language
     *
     * @since S60 v3.2
     * @param aLanguage Language id
     * @return None
     */
    void SetLanguage( TInt aLanguage );

    /**
     * Get language
     *
     * @return iLanguage
     */
    TInt GetLanguage();
       
    /**
     * Set recognition mode 
     *
     * @since S60 v3.2
     * @param aMode Recognition mode
     * @return None
     */
    void SetMode( TInt aMode );
       
    /**
     * Set recognition case
     *
     * @since S60 v3.2
     * @param aCase Recognition case 
     * @return None
     */
    void SetCase( TInt aCase );

    /**
     * Set case sensitive
     *
     * @since S60 v3.2
     * @param aCaseSensitive Sensitive flag
     * @return None
     */
    void SetCaseSensitive( TBool aCaseSensitive );

    /**
     * Set writing area size
     *
     * @since S60 v3.2
     * @param aSize Size of input area
     * @return None
     */         
    void SetInputAreaSize( TSize& aSize );
        
    /**
     * Set word recognition on/off
     *
     * @since S60 v3.2
     * @param aValue Word recognition flag
     * @return None
     */         
    void EnableWordRecognition( const TBool aValue = ETrue );
        
    /**
     * Set number mode
     *
     * @since S60 v3.2
     * @param aNumberMode Number mode to set
     * @return None
     */  
    void SetNumberMode( const TAknEditorNumericKeymap aNumberMode );
    
    /**
     * Set position of HWR top guideline
     * 
     * @param aPos Position to be set
     * @return None
     */
    void GetTopGuideLinePos( TInt& aPos );
    
    /**
     * Set position of HWR bottom guideline
     * 
     * @param aPos Position to be set
     * @return None
     */
    void GetBottomGuideLinePos( TInt& aPos );

    /**
     * Set permitted character set that can be recognized by hwr engine
     * 
     * @param aSymbolSet Permitted character set
     * @return None
     */
    void SetPermittedCharSet( const TDesC& aSymbolSet);
    
    static TInt BackgroundTaskL(TAny* aPtr);
    /**
     * Tell the HWR engine whether guideline is on.
     * 
     *
     * @param aFlag ETrue to enable guideline, and EFalse to diable.
     * @return KErrNone means operation succeed, otherwise system wide error codes
     */  
    TInt EnableGuideline(const TBool aFlag);     
private: 

    /**
     * Constructor
     *
     * @since S60 v3.2
     * @return None
     */
    CPeninputGenericRecognizer(CPtiEngine* aPtiEngine);

    /**
     * Constructor
     *
     * @since S60 v3.2
     * @param aLayoutContext The language id
     * @return None
     */
    CPeninputGenericRecognizer( TInt aLanguage );
    
    /**
     * Sencond phase constructor
     *
     * @since S60 v3.2
     * @return None
     */
    void ConstructL();
    
    /**
     * Initialize recognizer
     *
     * @since S60 v3.2
     * @return None
     */
    void InitRecognizerL();
    
    /**
     * Adjust result case
     *
     * @since S60 v3.2
     * @param aResult The recognition result array
     * @return None
     */
    void AdjustResultCase( RPointerArray<HBufC>& aResult );
    
    void DoIdleConstructL();
      
private: // Data
    
    /**
     * The pti engine
     * Not own
     */
    CPtiEngine* iPtiEngine; 
    
    /**
     * The hwr recognizer engine
     * Not own
     */
    MPtiHwrRecognizer* iHwrEngine;
    
    /**
     * The language id
     */
    TInt iLanguage;
    
    /**
     * The recognizer engine mode
     */
    TInt iMode;
    
    /**
     * The recognizer engine case
     */
    TInt iCase;
    
    /**
     * The recognizer engine case sensitive
     */
    TInt iCaseSensitive;
    
    /**
     * The recognizer engine enable/disable flag
     */
    TBool iEnabled;
    
    CIdle* iIdle;
    TBool iRecognizerInited;
  };

#endif   // C_PENINPUTGENERICRECOGNIZER_H
