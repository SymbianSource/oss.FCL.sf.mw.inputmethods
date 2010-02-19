/*
* Copyright (c) 2005,2006 Choe Hwanjin
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
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

#include "OssHangulInputContext.h"
#include "hangul.h"
#include "OssKeyboard.h"
#include "OssCombination.h"

_LIT8(KHangulInputContextEventTranslate,"translate");
_LIT8(KHangulInputContextEventTransition,"transition");

/*
LOCAL_C HBufC8* AllocStdBufLC(const TDesC8& aStr)
    {
    TInt len=aStr.Length()+1;
    HBufC8* buf=HBufC8::NewLC(len);
    TPtr8 ptr(buf->Des());
    ptr.FillZ(len);
    ptr.Copy(aStr);
    return buf;
    }*/


LOCAL_C HBufC8* AllocStdBufL(const TDesC8& aStr)
    {
    TInt len=aStr.Length()+1;
    HBufC8* buf=HBufC8::NewL(len);
    TPtr8 ptr(buf->Des());
    ptr.FillZ(len);
    ptr.Copy(aStr);
    return buf;
    }

class TLocalStdBuf
    {
    public:
        
        TLocalStdBuf(const TDesC8& aStr)
            {
            TInt len=aStr.Length()+1;
            iBuf=HBufC8::New(len);
            if (iBuf!=NULL)
                {
                TPtr8 ptr(iBuf->Des());
                ptr.FillZ(len);
                ptr.Copy(aStr);
                }
            }
        
        ~TLocalStdBuf()
            {
            delete iBuf;
            }
        
        const char* operator() ()
            {
            return (const char*) iBuf->Ptr(); 
            }
    
    private:
        
        HBufC8* iBuf;
    };


LOCAL_C TBool CBTranslate(HangulInputContext* ,int aAscII, ucschar* aStr, void* aSelf)
    {
    COssHangulInputContext* context=(COssHangulInputContext*)aSelf;
    return context->OnTranslate(aAscII,TPtrC(aStr));
    }

LOCAL_C TBool CBTransition(HangulInputContext* ,ucschar aChar, const ucschar* preedit, void* aSelf)
    {
    COssHangulInputContext* context=(COssHangulInputContext*)aSelf;
    return context->OnTransition(aChar,TPtrC(preedit));    
    }

EXPORT_C COssHangulInputContext* COssHangulInputContext::NewLC(
        const TDesC8& aKeyboardType)
    {
    COssHangulInputContext* self = 
        new (ELeave) COssHangulInputContext();
    CleanupStack::PushL(self);
    self->ConstructL(aKeyboardType);
    return self;
    }

EXPORT_C COssHangulInputContext* COssHangulInputContext::NewL(
        const TDesC8& aKeyboardType)
    {
    COssHangulInputContext* self = 
        COssHangulInputContext::NewLC(aKeyboardType);
    CleanupStack::Pop(); // self;
    return self;
    }


COssHangulInputContext::COssHangulInputContext()
    {
    }

void COssHangulInputContext::ConstructL(const TDesC8& aKeyboardType)
    {
    // Null termination string for standard library
    //iKeyboardType=AllocStdBufLC(aKeyboardType);
    iKeyboardType=AllocStdBufL(aKeyboardType);
    iHic=hangul_ic_new((const char*)iKeyboardType->Ptr());
    //CleanupStack::Pop(iKeyboardType);
    }

EXPORT_C COssHangulInputContext::~COssHangulInputContext()
    {
    hangul_ic_delete(iHic);
    delete iKeyboardType;
    }

EXPORT_C TBool COssHangulInputContext::Process(const TChar& aAscII)
    {
    return hangul_ic_process(iHic,aAscII);
    }

EXPORT_C void COssHangulInputContext::Reset()
    {
    hangul_ic_reset(iHic);
    }

EXPORT_C TBool COssHangulInputContext::BackSpace()
    {
    return hangul_ic_backspace(iHic);
    }

EXPORT_C TBool COssHangulInputContext::IsEmpty()
    {
    return hangul_ic_is_empty(iHic);
    }

EXPORT_C TBool COssHangulInputContext::HasChoseong()
    {
    return hangul_ic_has_choseong(iHic);
    }

EXPORT_C TBool COssHangulInputContext::HasJungseong()
    {
    return hangul_ic_has_jungseong(iHic);
    }

EXPORT_C TBool COssHangulInputContext::HasJongseong()
    {
    return hangul_ic_has_jongseong(iHic);
    }

EXPORT_C void COssHangulInputContext::SetOutputMode(const TInt& aMode)
    {
    hangul_ic_set_output_mode(iHic,aMode);
    }

EXPORT_C void COssHangulInputContext::SetKeyboard(const COssKeyboard& aOssKeyboard)
    {
    hangul_ic_set_keyboard(iHic,aOssKeyboard.iHangulKeyboard);
    }

EXPORT_C void COssHangulInputContext::SelectKeyboard(const TDesC8& aId)
    {
    TLocalStdBuf buf(aId);
    hangul_ic_select_keyboard(iHic,buf());
    }

EXPORT_C void COssHangulInputContext::SetCombinationL(COssCombination& aOssCombination)
    {
    aOssCombination.PopulateListL();
    hangul_ic_set_combination(iHic,aOssCombination.iHangulCombination);
    }

EXPORT_C void COssHangulInputContext::AddEventObserver(MOssHangulInputEventObserver* aObserver)
    {
    __ASSERT_ALWAYS(
        iObservers.Find(aObserver)==KErrNotFound,
        User::Panic(_L("libhangul_symbian"),__LINE__));
    
    hangul_ic_connect_callback(
        iHic,
        (const char *)KHangulInputContextEventTranslate().Ptr(),
        (void*)CBTranslate, 
        this);
    
    hangul_ic_connect_callback(
        iHic,
        (const char *)KHangulInputContextEventTransition().Ptr(),
        (void*)CBTransition, 
        this);
    
    iObservers.Append(aObserver);
    }

EXPORT_C void COssHangulInputContext::RemoveEventObserver(MOssHangulInputEventObserver* aObserver)
    {
    TInt index(iObservers.Find(aObserver));
    if (index!=KErrNotFound)
        {
        iObservers.Remove(index);        
        }
    }

EXPORT_C TPtrC COssHangulInputContext::GetPreeditString()
    {
    TPtrC ret(hangul_ic_get_preedit_string(iHic));
    return ret;
    }

EXPORT_C TPtrC COssHangulInputContext::GetCommitString()
    {
    TPtrC ret(hangul_ic_get_commit_string(iHic));
    return ret;
    }

EXPORT_C TPtrC COssHangulInputContext::Flush()
    {
    TPtrC ret(hangul_ic_flush(iHic));
    return ret;
    }

TBool COssHangulInputContext::OnTranslate(const TChar& aAscII,const TDesC& aUcsStr)
    {
    TBool ret(EFalse);
    for (TInt i=0;i<iObservers.Count();i++)
        {
        if (iObservers[i]->MOhieoOnTranslate(aAscII,aUcsStr)) ret=ETrue;
        }
    return ret;
    }

TBool COssHangulInputContext::OnTransition(const TChar& aChar,const TDesC& aPreEdit)
    {
    TBool ret(EFalse);
    for (TInt i=0;i<iObservers.Count();i++)
        {
        if (iObservers[i]->MOhieoOnTransition(aChar,aPreEdit)) ret=ETrue;
        }
    return ret;
    }

