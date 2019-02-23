#include "Luna-mbx.h"
#include <cstdio>



CListBase::CListBase()
{
    ReferenceCount = 1;
    Next = NULL;
    Prev = NULL;
}

CListBase::~CListBase()
{
}

long CListBase::Release( void )
{
    long NewRefCount = ReferenceCount - 1;
    ReferenceCount = NewRefCount;

    if (NewRefCount == 0)
    {
        delete this;
    }

    return NewRefCount;
}

void CListBase::AddRef( void )
{
    ReferenceCount++;
}



CList::CList()
{
    ListTop = NULL;
    ListEnd = NULL;
    Count = 0;
}

CList::~CList()
{
    LPCListBase cur = ListTop;
    while (cur != NULL)
    {
        LPCListBase next = cur->Next;
        cur->Release();
        cur = next;
    }

    ListTop = NULL;
    ListEnd = NULL;
    Count = 0;
}

void CList::Init( void )
{
    ListTop = NULL;
    ListEnd = NULL;
    Count = 0;
}

LPCListBase CList::Top( void )
{
    return ListTop;
}

LPCListBase CList::End( void )
{
    return ListEnd;
}

void CList::InsertTop( LPCListBase t )
{
    t->Prev = NULL;
    t->Next = ListTop;
    ListTop = t;
    LPCListBase next = t->Next;
    if (next != NULL) next->Prev = t;
    if (ListEnd == NULL) ListEnd = t;
    Count++;
}

void CList::InsertEnd( LPCListBase t )
{
    LPCListBase oldend = ListEnd;
    t->Prev = oldend;
    t->Next = NULL;
    ListEnd = t;
    if (oldend != NULL)
    {
        oldend->Next = t;
        Count++;
    }
    else
    {
        ListTop = t;
        Count++;
    }
}

void CList::Erase( LPCListBase t )
{
    LPCListBase oldprev = t->Prev;
    unsigned int flags = 0;
    if (oldprev == NULL) flags = 1;
    LPCListBase oldnext = t->Next;
    if (oldnext == NULL) flags |= 2;

    switch (flags)
    {
        case 0:
            oldprev->Next = oldnext;
            oldnext->Prev = oldprev;
            Count--;
            break;
        case 1:
            ListTop = oldnext;
            oldnext->Prev = NULL;
            Count--;
            break;
        case 2:
            //ListEnd = ListEnd->Prev;
            ListEnd = oldprev;
            oldprev->Next = NULL;
            Count--;
            break;
        case 3:
            ListTop = NULL;
            ListEnd = NULL;
        default:
            Count--;
            break;
    }
}

void CList::Release( void )
{
    LPCListBase cur = ListTop;
    while (cur != NULL)
    {
        LPCListBase next = cur->Next;
        cur->Release();
        cur = next;
    }

    ListTop = NULL;
    ListEnd = NULL;
    Count = 0;
}

LPCListBase CList::Data( long no )
{
    for (LPCListBase cur = ListTop; cur != NULL; cur = cur->Next)
    {
        if (no == 0) return cur;
        no--;
    }
    return NULL;
}

long CList::Size( void )
{
    return Count;
}

