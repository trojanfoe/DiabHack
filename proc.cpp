/*
 * Diablo Hacking Utility
 *
 * Copyright (C)1997 Trojan Consulting Ltd.
 *
 * andy@trojanco.demon.co.uk
 *
 * $Header: /diabhack/proc.cpp 1     2/03/99 21:21 Andy $
 */

#include "diabhack.h"

BOOL CProc::m_fReportMemIoErrors = TRUE;

HWND
CProc::GetWindowHandle(LPCSTR pszWindowName)
{
    return ::FindWindowEx(NULL, NULL, NULL, pszWindowName);
}

HANDLE
CProc::Open(LPCSTR pszWindowName)
{
    HANDLE hReturn = NULL;
    HWND hwnd = GetWindowHandle(pszWindowName);
    if (hwnd != NULL) {
        DWORD dwProcId;
        ::GetWindowThreadProcessId(hwnd, &dwProcId);
        hReturn = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcId);
        if (hReturn == NULL) {
            ::MsgBox(MB_ICONSTOP|MB_OK, "Unable to open process \"%s\"",
                pszWindowName);
        }
    } else {
        ::MsgBox(MB_ICONSTOP|MB_OK, "Unable to locate process \"%s\"",
            pszWindowName);
    }
    return hReturn;
}

void
CProc::Close(HANDLE &hProc)
{
    if (hProc != NULL) {
        ::CloseHandle(hProc);
    }
}

BOOL
CProc::Read(HANDLE hProc, DWORD dwOffset, LPBYTE pBuffer, DWORD dwLen)
{
    BOOL fReturn = FALSE;
    DWORD dwRead;

    if (::ReadProcessMemory(hProc, (LPCVOID)dwOffset, pBuffer, dwLen,
        &dwRead)) {
        ASSERT(dwRead == dwLen);
        fReturn = TRUE;
    } else if (m_fReportMemIoErrors) {
        if (::MsgBox(MB_ICONSTOP|MB_OKCANCEL,
            "Failed to read from offset 0x%lx, len 0x%lx:\n%s",
            dwOffset, dwLen, GetLastErrorText()) == IDCANCEL) {
            ReportMemIoErrors(FALSE);
        }
    }
    return fReturn;
}

BOOL
CProc::Write(HANDLE hProc, DWORD dwOffset, LPBYTE pBuffer, DWORD dwLen)
{
    BOOL fReturn = FALSE;
    DWORD dwWritten;

    if (::WriteProcessMemory(hProc, (LPVOID)dwOffset, pBuffer, dwLen,
        &dwWritten)) {
        ASSERT(dwWritten == dwLen);
        fReturn = TRUE;
    } else if (m_fReportMemIoErrors) {
        if (::MsgBox(MB_ICONSTOP|MB_OKCANCEL,
            "Failed to write to offset 0x%lx, len 0x%lx:\n%s",
            dwOffset, dwLen, GetLastErrorText()) == IDCANCEL) {
            ReportMemIoErrors(FALSE);
        }
    }
    return fReturn;
}
