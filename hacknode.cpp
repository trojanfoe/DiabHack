/*
 * Diablo Hacking Utility
 *
 * Copyright (C)1997 Trojan Consulting Ltd.
 *
 * andy@trojanco.demon.co.uk
 *
 * $Header: /diabhack/hacknode.cpp 1     2/03/99 21:21 Andy $
 */

#include "diabhack.h"

CHackNode::CHackNode()
{
    m_dwOffset = 0;
    m_dwLen = 0;
    m_pOldData = NULL;
    m_pNewData = NULL;
    m_pNext = NULL;
}

CHackNode::CHackNode(DWORD dwOffset, DWORD dwLen, LPBYTE pOldData,
    LPBYTE pNewData, class CHackNode *pNext /*=NULL*/)
{
    m_dwOffset = dwOffset;
    m_dwLen = dwLen;
    m_pOldData = NULL;
    oldData(pOldData);
    m_pNewData = NULL;
    newData(pNewData);
    m_pNext = pNext;
}

CHackNode::~CHackNode()
{
    if (m_pOldData != NULL) {
        delete [] m_pOldData;
    }
    if (m_pNewData != NULL) {
        delete [] m_pNewData;
    }
}

void
CHackNode::offset(DWORD dwOffset)
{
    m_dwOffset = dwOffset;
}

DWORD
CHackNode::offset() const
{
    return m_dwOffset;
}

void
CHackNode::len(DWORD dwLen)
{
    m_dwLen = dwLen;
}

DWORD
CHackNode::len() const
{
    return m_dwLen;
}

void
CHackNode::oldData(LPBYTE pOldData)
{
    if (m_pOldData != NULL) {
        delete [] m_pOldData;
    }
    m_pOldData = new BYTE [m_dwLen];
    if (m_pOldData != NULL) {
        memcpy(m_pOldData, pOldData, m_dwLen);
    }
}

LPBYTE
CHackNode::oldData() const
{
    return m_pOldData;
}

void
CHackNode::newData(LPBYTE pNewData)
{
    if (m_pNewData != NULL) {
        delete [] m_pNewData;
    }
    m_pNewData = new BYTE [m_dwLen];
    if (m_pNewData != NULL) {
        memcpy(m_pNewData, pNewData, m_dwLen);
    }
}

LPBYTE
CHackNode::newData() const
{
    return m_pNewData;
}

void
CHackNode::next(class CHackNode *pNext)
{
    m_pNext = pNext;
}

class CHackNode *
CHackNode::next() const
{
    return m_pNext;
}

CHack::State
CHackNode::getState(HANDLE hDiablo)
{
    CHack::State State = CHack::STATE_ERROR;

    LPBYTE pBuffer = new BYTE [m_dwLen];
    if (pBuffer != NULL) {
        if (CProc::Read(hDiablo, m_dwOffset, pBuffer, m_dwLen)) {
            if (memcmp(pBuffer, m_pOldData, m_dwLen) == 0) {
                State = CHack::STATE_INACTIVE;
            } else if (memcmp(pBuffer, m_pNewData, m_dwLen) == 0) {
                State = CHack::STATE_ACTIVE;
            }
        }
        delete pBuffer;
    } else {
        MsgBox(MB_ICONSTOP|MB_OK, "Memory allocation failure");
    }
    return State;
}

BOOL
CHackNode::activateHack(HANDLE hDiablo)
{
    BOOL fReturn = FALSE;

    LPBYTE pBuffer = new BYTE [m_dwLen];
    if (pBuffer != NULL) {
        if (CProc::Read(hDiablo, m_dwOffset, pBuffer, m_dwLen)) {
            if (memcmp(pBuffer, m_pOldData, m_dwLen) == 0) {
                // Not yet active
                if (CProc::Write(hDiablo, m_dwOffset, m_pNewData, m_dwLen)) {
                    fReturn = TRUE;
                }
            } else if (memcmp(pBuffer, m_pNewData, m_dwLen) == 0) {
                // Already active
                fReturn = TRUE;
            }
        }
        delete pBuffer;
    } else {
        MsgBox(MB_ICONSTOP|MB_OK, "Memory allocation failure");
    }
    return fReturn;
}

BOOL
CHackNode::deactivateHack(HANDLE hDiablo)
{
    BOOL fReturn = FALSE;

    LPBYTE pBuffer = new BYTE [m_dwLen];
    if (pBuffer != NULL) {
        if (CProc::Read(hDiablo, m_dwOffset, pBuffer, m_dwLen)) {
            if (memcmp(pBuffer, m_pNewData, m_dwLen) == 0) {
                // Currently active
                if (CProc::Write(hDiablo, m_dwOffset, m_pOldData, m_dwLen)) {
                    fReturn = TRUE;
                }
            } else if (memcmp(pBuffer, m_pOldData, m_dwLen) == 0) {
                // Already inactive
                fReturn = TRUE;
            }
        }
        delete pBuffer;
    } else {
        MsgBox(MB_ICONSTOP|MB_OK, "Memory allocation failure");
    }
    return fReturn;
}

void
CHackNode::dumpContents(LPSTR pszBuffer)
{
    char szBuffer[1024];
    LPSTR pszOldData = BinToAscii(m_pOldData, m_dwLen);
    LPSTR pszNewData = BinToAscii(m_pNewData, m_dwLen);
    wsprintf(szBuffer,
        "Offset=0x%lx Len=0x%lx OldData=%s NewData=%s\r\n",
        m_dwOffset, m_dwLen, pszOldData, pszNewData);
    delete pszOldData;
    delete pszNewData;
    strcat(pszBuffer, szBuffer);
}
