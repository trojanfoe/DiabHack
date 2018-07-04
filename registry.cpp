/*
 * Diablo Hacking Utility
 *
 * Copyright (C)1997 Trojan Consulting Ltd.
 *
 * andy@trojanco.demon.co.uk
 *
 * $Header: /diabhack/registry.cpp 1     2/03/99 21:21 Andy $
 */

#include "diabhack.h"

BOOL
LoadCurrentWindowPositions()
{
    BOOL fReturn = FALSE;
    HKEY hk;
    if (hk = OpenRegistry()) {
        DWORD dwType, dwSize;
        dwSize = sizeof(DWORD);
        RegQueryValueEx(hk, "MainX", NULL, &dwType,
            (LPBYTE)&g_nMainX, &dwSize);
        RegQueryValueEx(hk, "MainY", NULL, &dwType,
            (LPBYTE)&g_nMainY, &dwSize);
        RegQueryValueEx(hk, "MainWidth", NULL, &dwType,
            (LPBYTE)&g_nMainCX, &dwSize);
        RegQueryValueEx(hk, "MainHeight", NULL, &dwType,
            (LPBYTE)&g_nMainCY, &dwSize);
        RegQueryValueEx(hk, "LV0Width", NULL, &dwType,
            (LPBYTE)&g_nLV0CX, &dwSize);
        RegQueryValueEx(hk, "LV1Width", NULL, &dwType,
            (LPBYTE)&g_nLV1CX, &dwSize);
        RegQueryValueEx(hk, "LV2Width", NULL, &dwType,
            (LPBYTE)&g_nLV2CX, &dwSize);
        fReturn = TRUE;
        RegCloseKey(hk);
    }
    return fReturn;
}

BOOL
SaveCurrentWindowPositions(HWND hwndMain, HWND hwndLV)
{
    BOOL fReturn = FALSE;
    HKEY hk;
    if (hk = OpenRegistry()) {
        RECT r;
        if (GetWindowRect(hwndMain, &r)) {
            g_nMainX = r.left;
            g_nMainY = r.top;
            g_nMainCX = r.right - r.left;
            g_nMainCY = r.bottom - r.top;
            RegSetValueEx(hk, "MainX", 0, REG_DWORD,
                (CONST BYTE *)&g_nMainX, sizeof(DWORD));
            RegSetValueEx(hk, "MainY", 0, REG_DWORD,
                (CONST BYTE *)&g_nMainY, sizeof(DWORD));
            RegSetValueEx(hk, "MainWidth", 0, REG_DWORD,
                (CONST BYTE *)&g_nMainCX, sizeof(DWORD));
            RegSetValueEx(hk, "MainHeight", 0, REG_DWORD,
                (CONST BYTE *)&g_nMainCY, sizeof(DWORD));
        }

        g_nLV0CX = ListView_GetColumnWidth(hwndLV, 0);
        g_nLV1CX = ListView_GetColumnWidth(hwndLV, 1);
        g_nLV2CX = ListView_GetColumnWidth(hwndLV, 2);
        RegSetValueEx(hk, "LV0Width", 0, REG_DWORD,
            (CONST BYTE *)&g_nLV0CX, sizeof(DWORD));
        RegSetValueEx(hk, "LV1Width", 0, REG_DWORD,
            (CONST BYTE *)&g_nLV1CX, sizeof(DWORD));
        RegSetValueEx(hk, "LV2Width", 0, REG_DWORD,
            (CONST BYTE *)&g_nLV2CX, sizeof(DWORD));
        fReturn = TRUE;
        RegCloseKey(hk);
    }
    return fReturn;
}

BOOL
LoadNotifyFlags()
{
    BOOL fReturn = FALSE;
    HKEY hk;
    if (hk = OpenRegistry()) {
        DWORD dwType, dwSize;
        dwSize = sizeof(DWORD);
        RegQueryValueEx(hk, "DiabloNotRunningNotifyFlag", NULL, &dwType,
            (LPBYTE)&g_dwDiabloNotRunningNotifyFlag, &dwSize);
        RegQueryValueEx(hk, "WriteMemDangerousNotifyFlag", NULL, &dwType,
            (LPBYTE)&g_dwDangerousCommandNotifyFlag, &dwSize);
        fReturn = TRUE;
        RegCloseKey(hk);
    }
    return fReturn;
}

BOOL
SaveNotifyFlags()
{
    BOOL fReturn = FALSE;
    HKEY hk;
    if (hk = OpenRegistry()) {
        RegSetValueEx(hk, "DiabloNotRunningNotifyFlag", 0, REG_DWORD,
            (CONST BYTE *)&g_dwDiabloNotRunningNotifyFlag, sizeof(DWORD));
        RegSetValueEx(hk, "WriteMemDangerousNotifyFlag", 0, REG_DWORD,
            (CONST BYTE *)&g_dwDangerousCommandNotifyFlag, sizeof(DWORD));
        fReturn = TRUE;
        RegCloseKey(hk);
    }
    return fReturn;
}

HKEY
OpenRegistry()
{
    HKEY hkReturn;
    DWORD dwDisp;
    if (RegCreateKeyEx(HKEY_CURRENT_USER,
        "Software\\Trojan Consulting\\DiabHack", 0, NULL,
        REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL,
        &hkReturn, &dwDisp) == ERROR_SUCCESS) {
        return hkReturn;
    }
    return NULL;
}
