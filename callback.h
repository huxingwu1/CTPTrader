#ifndef CALLBACK_H
#define CALLBACK_H
#include <vector>
#include <iostream>

#include "eventbase.h"

//反应槽基类
class slotbase
{
public:
    virtual void Execute(Event ev) = 0;
};

//T类型的类，成员函数的变量时Event类型
template<typename T>
class slotimpl : public slotbase
{
public:
    using member_function = void (T::*)(Event);        //起类型别名，using等价typedef

    slotimpl(T* pObj, member_function pMemberFunc)
    {
        m_pObj = pObj;
        m_pMemberFunc = pMemberFunc;
    }

    inline void Execute(Event ev) override
    {
        (m_pObj->*m_pMemberFunc)(ev);
    }

private:
    T* m_pObj;
    member_function m_pMemberFunc;
};

//回调类
class CallBack
{
public:
    template<typename T>
    CallBack(T* pObj, void (T::*pMemberFunc)(Event))
    {
        m_pSlotbase = new slotimpl<T>(pObj, pMemberFunc);  //T类型对象，成员函数指针
    }
    ~CallBack()
    {
        delete m_pSlotbase;
    }

    inline void Execute(Event ev)
    {
        m_pSlotbase->Execute(ev);
    }

private:
    slotbase* m_pSlotbase;
};

#endif // CALLBACK_H
