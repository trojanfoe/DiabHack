/*
 * Diablo Hacking Utility
 *
 * Copyright (C)1997 Trojan Consulting Ltd.
 *
 * andy@trojanco.demon.co.uk
 *
 * $Header: /diabhack/toolbar.cpp 1     2/03/99 21:21 Andy $
 */

#include "diabhack.h"

static TBBUTTON tbButtons[] = {
    { 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0 },
    { 0, IDM_FILE_OPENDAT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
    { 1, IDM_FILE_SAVEDAT, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
    { 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0 },
    { 2, IDM_HACK_ACTIVATE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
    { 3, IDM_HACK_ACTIVATEALL, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
    { 4, IDM_HACK_DEACTIVATE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
    { 5, IDM_HACK_DEACTIVATEALL, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
    { 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0 },
    { 6, IDM_HACK_REFRESHSTATUS, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
    { 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0 },
    { 7, IDM_HACK_REMOVE, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
    { 8, IDM_HACK_REMOVEALL, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
    { 0, 0, TBSTATE_ENABLED, TBSTYLE_SEP, 0, 0 },
    { 9, IDM_UTILITY_READDIABLOMEMORY, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
    { 10, IDM_UTILITY_WRITEDIABLOMEMORY, TBSTATE_ENABLED, TBSTYLE_BUTTON, 0, 0 },
};
const int nButtons = sizeof(tbButtons) / sizeof(tbButtons[0]);
const int nBitmaps = 11;         // Don't forget to update this!!!

HWND
CreateToolbar()
{
    HWND hwndTB = CreateToolbarEx(g_hwndMain, WS_CHILD|WS_VISIBLE|TBSTYLE_TOOLTIPS,
        ID_TOOLBAR, nBitmaps, g_hInst, IDB_TOOLBAR, tbButtons, nButtons,
        16, 16, 16, 15, sizeof(TBBUTTON));
    return hwndTB;
}

// Enable/disable the hack-related buttons depending on whether diablo is
// running
void
EnableHackButtons(HWND hwnd, BOOL fEnable)
{
    SendMessage(hwnd, TB_ENABLEBUTTON, IDM_HACK_ACTIVATE,
        MAKELONG(fEnable, 0));
    SendMessage(hwnd, TB_ENABLEBUTTON, IDM_HACK_ACTIVATEALL,
        MAKELONG(fEnable, 0));
    SendMessage(hwnd, TB_ENABLEBUTTON, IDM_HACK_DEACTIVATE,
        MAKELONG(fEnable, 0));
    SendMessage(hwnd, TB_ENABLEBUTTON, IDM_HACK_DEACTIVATEALL,
        MAKELONG(fEnable, 0));
    SendMessage(hwnd, TB_ENABLEBUTTON, IDM_HACK_ACTIVATE,
        MAKELONG(fEnable, 0));
    SendMessage(hwnd, TB_ENABLEBUTTON, IDM_HACK_REFRESHSTATUS,
        MAKELONG(fEnable, 0));
    SendMessage(hwnd, TB_ENABLEBUTTON, IDM_UTILITY_READDIABLOMEMORY,
        MAKELONG(fEnable, 0));
    SendMessage(hwnd, TB_ENABLEBUTTON, IDM_UTILITY_WRITEDIABLOMEMORY,
        MAKELONG(fEnable, 0));
}

void
EnableRemoveAllButton(HWND hwnd, BOOL fEnable)
{
    SendMessage(hwnd, TB_ENABLEBUTTON, IDM_HACK_REMOVE,
        MAKELONG(fEnable, 0));
    SendMessage(hwnd, TB_ENABLEBUTTON, IDM_HACK_REMOVEALL,
        MAKELONG(fEnable, 0));
}
