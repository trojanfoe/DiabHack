/*
 * Diablo Hacking Utility
 *
 * Copyright (C)1997 Trojan Consulting Ltd.
 *
 * andy@trojanco.demon.co.uk
 *
 * $Header: /diabhack/hack.cpp 1     2/03/99 21:21 Andy $
 */

#include "diabhack.h"

static char g_szDiablo[] = "Diablo";

CHack::CHack()
{
	Init();
}

CHack::CHack(LPCSTR pszDesc, LPCSTR pszVersion)
{
	Init();

    desc(pszDesc);
    version(pszVersion);
}

void
CHack::Init()
{
    m_pszDesc = NULL;
	m_pszVersion = NULL;
    m_pRootNode = NULL;
    m_pNext = NULL;
    m_State = STATE_UNKNOWN;
}

CHack::~CHack()
{
    if (m_pszDesc != NULL) {
        delete [] m_pszDesc;
    }
    if (m_pszVersion != NULL) {
        delete [] m_pszVersion;
    }

    CHackNode *pNode = m_pRootNode;
    while (pNode != NULL) {
        CHackNode *pTemp = pNode;
        pNode = pNode->next();
        delete pTemp;
    }
}

void
CHack::desc(LPCSTR pszDesc)
{
    if (m_pszDesc != NULL) {
        delete [] m_pszDesc;
    }
    m_pszDesc = new char [strlen(pszDesc) + 1];
    if (m_pszDesc != NULL) {
        strcpy(m_pszDesc, pszDesc);
    }
}

LPCSTR
CHack::desc() const
{
    return m_pszDesc;
}

void
CHack::version(LPCSTR pszVersion)
{
    if (m_pszVersion != NULL) {
        delete [] m_pszVersion;
    }
    m_pszVersion = new char [strlen(pszVersion) + 1];
    if (m_pszVersion != NULL) {
        strcpy(m_pszVersion, pszVersion);
    }
}

LPCSTR
CHack::version() const
{
    return m_pszVersion;
}

void
CHack::addNode(CHackNode *pNode)
{
    if (m_pRootNode != NULL) {
        CHackNode *pListNode = m_pRootNode;
        while (pListNode->next() != NULL) {
            pListNode = pListNode->next();
        }
        pListNode->next(pNode);
    } else {
        m_pRootNode = pNode;
    }
    pNode->next(NULL);
}

CHackNode *
CHack::findNode(DWORD dwOffset, DWORD dwLen) const
{
    CHackNode *pNode = m_pRootNode;
    while (pNode != NULL) {
        if (dwOffset < pNode->offset()) {
            if (dwOffset + dwLen > pNode->offset()) {
                break;
            }
        } else if (dwOffset > pNode->offset()) {
            if (pNode->offset() + pNode->len() > dwOffset) {
                break;
            }
        } else {
            // dwOffset == pNode->offset()
            break;
        }

        pNode = pNode->next();
    }
    return pNode;
}

CHackNode *
CHack::rootNode() const
{
    return m_pRootNode;
}

void
CHack::next(class CHack *pNext)
{
    m_pNext = pNext;
}

class CHack *
CHack::next() const
{
    return m_pNext;
}

void
CHack::state(CHack::State State)
{
    m_State = State;
}

CHack::State
CHack::state() const
{
    return m_State;
}

BOOL
CHack::IsDiabloRunning()
{
    HWND hwnd = CProc::GetWindowHandle("Diablo");
    if (hwnd != NULL) {
        DWORD dwProcId;
        GetWindowThreadProcessId(hwnd, &dwProcId);
        HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcId);
        if (hProc != NULL) {
            CloseHandle(hProc);
            return TRUE;
        }
    }
    return FALSE;
}

BOOL
CHack::getState()
{
    BOOL fReturn = FALSE;
    HANDLE hDiablo;
    if (hDiablo = CProc::Open(g_szDiablo)) {
        CHackNode *pNode = m_pRootNode;
        while (pNode != NULL) {
            m_State = pNode->getState(hDiablo);
            if (m_State == STATE_ERROR) {
                break;
            }
            pNode = pNode->next();
        }
        if (m_State != STATE_ERROR) {
            fReturn = TRUE;
        }
        CProc::Close(hDiablo);
    } else {
        m_State = STATE_UNKNOWN;
    }
    return fReturn;
}

BOOL
CHack::activateHack()
{
    if (m_State == STATE_ACTIVE) {
        return TRUE;
    }
    BOOL fReturn = FALSE;
    HANDLE hDiablo;
    if (hDiablo = CProc::Open(g_szDiablo)) {
        CHackNode *pNode = m_pRootNode;
        while (pNode != NULL) {
            if (!pNode->activateHack(hDiablo)) {
                break;
            }
            pNode = pNode->next();
        }
        if (pNode == NULL && m_pRootNode != NULL) {
            m_State = STATE_ACTIVE;
            fReturn = TRUE;
        } else {
            m_State = STATE_ERROR;
        }
        CProc::Close(hDiablo);
    }
    return fReturn;
}

BOOL
CHack::deactivateHack()
{
    if (m_State == STATE_INACTIVE) {
        return TRUE;
    }
    BOOL fReturn = FALSE;
    HANDLE hDiablo;
    if (hDiablo = CProc::Open(g_szDiablo)) {
        CHackNode *pNode = m_pRootNode;
        while (pNode != NULL) {
            if (!pNode->deactivateHack(hDiablo)) {
                break;
            }
            pNode = pNode->next();
        }
        if (pNode == NULL && m_pRootNode != NULL) {
            m_State = STATE_INACTIVE;
            fReturn = TRUE;
        } else {
            m_State = STATE_ERROR;
        }
        CProc::Close(hDiablo);
    }
    return fReturn;
}

void
CHack::dumpContents(LPSTR pszBuffer)
{
    char szBuffer[128];
    LPCSTR pszState;
    switch (m_State) {
    case STATE_UNKNOWN:
        pszState = "Unknown";
        break;
    case STATE_ACTIVE:
        pszState = "Active";
        break;
    case STATE_INACTIVE:
        pszState = "Inactive";
        break;
    case STATE_ERROR:
        pszState = "Error";
        break;
    default:
        pszState = "*Invalid*";
        break;
    }

    wsprintf(szBuffer,
        "Hack Description=\"%s\" Version=\"%s\" State=%s\r\n",
        m_pszDesc, m_pszVersion, pszState);
    strcat(pszBuffer, szBuffer);
    CHackNode *pNode = m_pRootNode;
    while (pNode != NULL) {
        pNode->dumpContents(pszBuffer);
        pNode = pNode->next();
    }
}
