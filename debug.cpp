/*
 * Diablo Hacking Utility
 *
 * Copyright (C)1997 Trojan Consulting Ltd.
 *
 * andy@trojanco.demon.co.uk
 *
 * $Header: /diabhack/debug.cpp 1     2/03/99 21:21 Andy $
 */

#include "diabhack.h"

void
MyTrace(LPCSTR pszFmt, ...)
{
#ifdef _DEBUG
    char szBuffer[2048];
    va_list va;
    va_start(va, pszFmt);
    wvsprintf(szBuffer, pszFmt, va);
    va_end(va);
    OutputDebugString(szBuffer);
#endif /* _DEBUG */
}

void
MyAssert(LPCSTR pszFile, unsigned uLine, LPCSTR pszExpr)
{
#ifdef _DEBUG
    char szBuffer[2048];
    MyTrace("Assertion \"%s\" failed at %s:%u\n", pszExpr, pszFile, uLine);
    wsprintf(szBuffer,
        "Assertion \"%s\"\nFailed at %s:%u\n\nPress Cancel to stop",
        pszExpr, pszFile, uLine);
    if (MessageBox(NULL, szBuffer, "Assertion failure", MB_OKCANCEL)
        == IDCANCEL) {
        DebugBreak();
    }
#endif /* _DEBUG */
}
