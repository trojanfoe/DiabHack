/*
 * Diablo Hacking Utility
 *
 * Copyright (C)1997 Trojan Consulting Ltd.
 *
 * andy@trojanco.demon.co.uk
 *
 * $Header: /diabhack/utils.cpp 1     2/03/99 21:21 Andy $
 */

#include "diabhack.h"

BOOL
GetDlgItemHex(HWND hwndDlg, int nItemId, LPCSTR pszItemText, DWORD &dwValue)
{
    BOOL fReturn = FALSE;
    char szBuffer[256];
    HWND hwndCtrl = GetDlgItem(hwndDlg, nItemId);
    ASSERT(hwndCtrl != NULL);
    if (GetWindowText(hwndCtrl, szBuffer, sizeof(szBuffer)) > 0) {
        if (ParseHexValue(szBuffer, dwValue)) {
            fReturn = TRUE;
        } else {
            MsgBox(MB_ICONEXCLAMATION|MB_OK, "%s is invalid", pszItemText);
        }
    } else {
        MsgBox(MB_ICONEXCLAMATION|MB_OK, "%s not specified", pszItemText);
    }

    if (!fReturn) {
        SetFocus(hwndCtrl);
    }
    return fReturn;
}

BOOL
SetDlgItemHex(HWND hwndDlg, int nItemId, DWORD dwValue)
{
    BOOL fReturn = FALSE;
    char szBuffer[256];
    wsprintf(szBuffer, "%lx", dwValue);
    return SetDlgItemText(hwndDlg, nItemId, szBuffer);
}

int
MsgBox(UINT uType, LPCSTR pszFmt, ...)
{
    char szBuffer[2048];
    va_list va;
    va_start(va, pszFmt);
    wvsprintf(szBuffer, pszFmt, va);
    va_end(va);
    return MessageBox(g_hwndMain, szBuffer, g_szTitle, uType);
}

BOOL
GetFileName(LPSTR pszBuffer, DWORD dwBufLen, LPCSTR pszFilter,
    LPCSTR pszDefExt, BOOL fSave)
{
    OPENFILENAME ofn;

    memset(&ofn, 0, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = g_hwndMain;
    ofn.hInstance = g_hInst;
    ofn.lpstrFilter = pszFilter;
    ofn.lpstrFile = pszBuffer;
    ofn.nMaxFile = dwBufLen;
    if (fSave) {
        ofn.Flags = OFN_PATHMUSTEXIST|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT;
    } else {
        ofn.Flags = OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;
    }
    ofn.lpstrDefExt = pszDefExt;

    return fSave ? GetSaveFileName(&ofn) : GetOpenFileName(&ofn);
}

LPSTR
BinToAscii(LPBYTE pBuffer, DWORD dwBufLen)
{
    LPSTR pszReturn = new char [(dwBufLen * 2) + 1];
    if (pszReturn != NULL) {
        LPSTR psz = pszReturn;
        while (dwBufLen-- > 0) {
            BYTE b = (*pBuffer >> 4) & 0xf;
            if (b > 9) {
                *psz++ = b - 0x0a + 'a';
            } else {
                *psz++ = b + '0';
            }
            b = *pBuffer & 0xf;
            if (b > 9) {
                *psz++ = b - 0x0a + 'a';
            } else {
                *psz++ = b + '0';
            }
            pBuffer++;
        }
        *psz = '\0';
    }
    return pszReturn;
}

BOOL
ParseHexValue(LPCSTR pszString, DWORD &dwValue)
{
    LPSTR pszBase;
    if (pszString[0] == '0' &&
        (pszString[1] == 'x' || pszString[1] == 'X')) {
        pszString += 2;
    }
    dwValue = strtoul(pszString, &pszBase, 16);
    return *pszBase == '\0' ? TRUE : FALSE;
}

DWORD
ParseHexData(LPCSTR pszString, LPBYTE pBuffer, DWORD dwBufLen)
{
    DWORD dwLen = 0;
    while (*pszString != '\0' && dwBufLen > 0) {
        BYTE b1, b2;
        if (!ParseHexDigit(pszString, b1) ||
            !ParseHexDigit(pszString+1, b2)) {
            return 0;
        }
        *pBuffer++ = (b1 << 4) | b2;
        dwBufLen--;
        pszString += 2;
        dwLen++;
    }
    return dwLen;
}

BOOL
ParseHexDigit(LPCSTR pszString, BYTE &Byte)
{
    if (!isxdigit(*pszString)) {
        return FALSE;
    }
    if (isdigit(*pszString)) {
        Byte = *pszString - '0';
    } else if (islower(*pszString)) {
        Byte = *pszString - 'a' + 0xa;
    } else {
        Byte = *pszString - 'A' + 0xa; 
    }
    return TRUE;
}

DWORD
GetErrorText(DWORD dwErrorCode, LPTSTR pszBuffer, DWORD dwBufLen)
{
    char szBuffer[1024];
    DWORD i;
	DWORD dwReturn = FormatMessage(FORMAT_MESSAGE_IGNORE_INSERTS|
        FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_ARGUMENT_ARRAY,
    	0, dwErrorCode, LANG_NEUTRAL, szBuffer, sizeof(szBuffer), 0);
	if (dwReturn) {
		// Remove CR+LFs
        for (i = 0; i < sizeof(szBuffer) - 1 && i < dwBufLen - 1; i++) {
            if (szBuffer[i] == 0x0d && szBuffer[i + 1] == 0x0a) {
                i++;
                *pszBuffer++ = ' ';
                continue;
            }
            *pszBuffer++ = szBuffer[i];
        }
        *pszBuffer = '\0';
	}
    return dwReturn;
}

// This is a dangerous version which isn't thread-safe...
LPCSTR
GetLastErrorText()
{
    static char szBuffer[1024];
    return GetErrorText(GetLastError(), szBuffer,
        sizeof(szBuffer)) ? szBuffer : "Unknown Error";
}
