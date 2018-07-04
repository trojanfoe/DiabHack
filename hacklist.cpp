/*
 * Diablo Hacking Utility
 *
 * Copyright (C)1997 Trojan Consulting Ltd.
 *
 * andy@trojanco.demon.co.uk
 *
 * $Header: /diabhack/hacklist.cpp 1     2/03/99 21:21 Andy $
 */

#include "diabhack.h"

CHackList::CHackList()
{
    m_pRootHack = NULL;
}

CHackList::~CHackList()
{
    removeAll();
}

void
CHackList::add(CHack *pHack)
{
    if (m_pRootHack == NULL) {
        m_pRootHack = pHack;
    } else {
        CHack *pListHack = m_pRootHack;
        while (pListHack->next() != NULL) {
            pListHack = pListHack->next();
        }
        pListHack->next(pHack);
    }
    pHack->next(NULL);
}

CHack *
CHackList::find(LPCSTR pszDesc)
{
    CHack *pHack = m_pRootHack;
    while (pHack != NULL) {
        if (stricmp(pHack->desc(), pszDesc) == 0) {
            break;
        }
        pHack = pHack->next();
    }
    return pHack;
}

CHack *
CHackList::find(DWORD dwOffset, DWORD dwLen)
{
    CHack *pHack = m_pRootHack;
    while (pHack != NULL) {
        if (pHack->findNode(dwOffset, dwLen) != NULL) {
            break;
        }
        pHack = pHack->next();
    }
    return pHack;
}

void
CHackList::remove(CHack *pHack)
{
    CHack *pListHack = m_pRootHack;
    CHack *pLast = NULL;
    while (pListHack != NULL) {
        if (pListHack == pHack) {
            if (pLast != NULL) {
                // Remove node from singly-linked list
                pLast->next(pListHack->next());
            } else {
                // Must be root node
                m_pRootHack = pListHack->next();
            }
            delete pListHack;
            return;
        }

        pLast = pListHack;
        pListHack = pListHack->next();
    }
}

void
CHackList::removeAll()
{
    while (m_pRootHack != NULL) {
        remove(m_pRootHack);
    }
}

#ifdef _DEBUG
void
CHackList::dump()
{
    char szBuffer[16384];
    OutputDebugString("**** Starting dump of hacks ****\n");
    CHack *pHack = m_pRootHack;
    while (pHack != NULL) {
        szBuffer[0] = '\0';
        pHack->dumpContents(szBuffer);
        OutputDebugString(szBuffer);
        pHack = pHack->next();
    }
    OutputDebugString("**** Ending dump of hacks ****\n");
}
#endif // _DEBUG
