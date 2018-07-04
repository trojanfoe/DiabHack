/*
 * Diablo Hacking Utility
 *
 * Copyright (C)1997 Trojan Consulting Ltd.
 *
 * andy@trojanco.demon.co.uk
 *
 * $Header: /diabhack/diabhack.h 1     2/03/99 21:21 Andy $
 */

#ifndef _DIABHACK_H_
#define _DIABHACK_H_

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "resource.h"

/*
 * CProc provides a wrapper for the Win32 process open, read,
 * write and close API functions.
 */
class CProc {
private:
    static BOOL m_fReportMemIoErrors;
public:
    static HWND GetWindowHandle(LPCSTR pszWindowName);
    static HANDLE Open(LPCSTR pszWindowName);
    static void Close(HANDLE &hProc);
    static BOOL Read(HANDLE hProc, DWORD dwOffset,
        LPBYTE pBuffer, DWORD dwLen);
    static BOOL Write(HANDLE hProc, DWORD dwOffset,
        LPBYTE pBuffer, DWORD dwLen);
    static void ReportMemIoErrors(BOOL fReport) {
        m_fReportMemIoErrors = fReport;
    }
};

/*
 * CHack holds a list of CHackNodes and a description of the hack
 */
class CHackNode;        // Forward
class CHack {
public:
    enum State {
        STATE_UNKNOWN,
        STATE_ACTIVE,
        STATE_INACTIVE,
        STATE_ERROR
    };
private:
    LPSTR m_pszDesc;
    LPSTR m_pszVersion;
    CHackNode *m_pRootNode;
    CHack *m_pNext;
    State m_State;

	void Init();
	
public:
    CHack();
    CHack(LPCSTR pszDesc, LPCSTR pszVersion);
    ~CHack();

    void desc(LPCSTR pszDesc);
    LPCSTR desc() const;

    void version(LPCSTR pszVersion);
    LPCSTR version() const;

    void addNode(CHackNode *pNode);
    CHackNode *findNode(DWORD dwOffset, DWORD dwLen) const;
    CHackNode *rootNode() const;

    void next(class CHack *pNext);
    class CHack *next() const;

    void state(State State);
    State state() const;

    static BOOL IsDiabloRunning();

    BOOL getState();
    BOOL activateHack();
    BOOL deactivateHack();

    void dumpContents(LPSTR pszBuffer);
};

/*
 * CHackList manages a list of hacks
 */
class CHackList {
private:
    CHack *m_pRootHack;
public:
    CHackList();
    ~CHackList();
    CHack *rootHack() {
        return m_pRootHack;
    }
    void add(CHack *pHack);
    CHack *find(LPCSTR pszDesc);
    CHack *find(DWORD dwOffset, DWORD dwLen);
    void remove(CHack *pHack);
    void removeAll();
#ifdef _DEBUG
    void dump();
#endif // _DEBUG
};

/*
 * CHackNode holds information about a single
 * program modification.
 */
class CHackNode {
private:
    DWORD m_dwOffset;
    DWORD m_dwLen;
    LPBYTE m_pOldData;
    LPBYTE m_pNewData;
    class CHackNode *m_pNext;

public:
    CHackNode();
    CHackNode(DWORD dwOffset, DWORD dwLen, LPBYTE pOldData, LPBYTE pNewData,
        class CHackNode *pNext = NULL);
    ~CHackNode();

    void offset(DWORD dwOffset);
    DWORD offset() const;

    void len(DWORD dwLen);
    DWORD len() const;

    void oldData(LPBYTE pOldData);
    LPBYTE oldData() const;

    void newData(LPBYTE pNewData);
    LPBYTE newData() const;

    void next(class CHackNode *pNext);
    class CHackNode *next() const;

    CHack::State getState(HANDLE hDiablo);
    BOOL activateHack(HANDLE hDiablo);
    BOOL deactivateHack(HANDLE hDiablo);

    void dumpContents(LPSTR szBuffer);
};

/*
 * Image indexes used in listview window
 */
enum LVImageIndex {
    IDX_STATEUNKNOWN,
    IDX_STATEACTIVE,
    IDX_STATEINACTIVE,
    IDX_STATEERROR
};

/*
 * main.cpp
 */
extern char g_szTitle[];
extern int g_nMainX, g_nMainY, g_nMainCX, g_nMainCY;
extern int g_nLV0CX, g_nLV1CX, g_nLV2CX;
extern DWORD g_dwDiabloNotRunningNotifyFlag;
extern DWORD g_dwDangerousCommandNotifyFlag;
extern HWND g_hwndMain;
extern HINSTANCE g_hInst;
extern CHackList g_HackList;

/*
 * dialog.cpp
 */
extern CHack *g_pHackDisplayProps;
extern BOOL CALLBACK ReadDiabloMemoryDlgProc(HWND hwnd, UINT uMsg,
    WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK WriteDiabloMemoryDlgProc(HWND hwnd, UINT uMsg,
    WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK AboutDlgProc(HWND hwnd, UINT uMsg,
    WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK DisplayHackPropsDlgProc(HWND hwnd, UINT uMsg,
    WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK HelpCreatingDatfilesDlgProc(HWND hwnd, UINT uMsg,
    WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK HelpSofticeCodesDlgProc(HWND hwnd, UINT uMsg,
    WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK DiabloNotRunningDlgProc(HWND hwnd, UINT uMsg,
    WPARAM wParam, LPARAM lParam);
extern BOOL CALLBACK DangerousCommandDlgProc(HWND hwnd, UINT uMsg,
    WPARAM wParam, LPARAM lParam);


/*
 * listview.cpp
 */
extern HWND CreateListView();
extern void RefreshListView(HWND hwnd);
extern void DblClkListView(HWND hwnd);
extern void RClickListView(HWND hwnd);
extern void ColumnClickListView(HWND hwnd, int nColumn);
extern void ActivateHacks(HWND hwnd, BOOL fAll);
extern void DeactivateHacks(HWND hwnd, BOOL fAll);
extern void RemoveSelectedHacks(HWND hwnd);
extern void UpdateHackState(HWND hwnd, CHack *pHack);
extern void DisplayHackProperties(HWND hwnd);
extern void InterpretHackState(CHack *pHack, LPCSTR &pszState,
    int &nImageIndex);

/*
 * toolbar.cpp
 */
extern HWND CreateToolbar();
extern void EnableHackButtons(HWND hwnd, BOOL fEnable);
extern void EnableRemoveAllButton(HWND hwnd, BOOL fEnable);

/*
 * datfile.cpp
 */
extern BOOL LoadDatFile(LPCSTR pszFileName);
extern BOOL SaveDatFile(LPCSTR pszFileName);
extern BOOL SkipToNextHack(FILE *fp, UINT &uLine);
extern int ParseLine(LPSTR pszLine, LPSTR pszWord[], int nMaxWords);

/*
 * registry.cpp
 */
extern BOOL LoadCurrentWindowPositions();
extern BOOL SaveCurrentWindowPositions(HWND hwndMain, HWND hwndLV);
extern BOOL LoadNotifyFlags();
extern BOOL SaveNotifyFlags();
extern HKEY OpenRegistry();

/*
 * utils.cpp
 */
extern BOOL GetDlgItemHex(HWND hwndDlg, int nItemId, LPCSTR pszItemText,
    DWORD &dwValue);
extern BOOL SetDlgItemHex(HWND hwndDlg, int nItemId, DWORD dwValue);
extern int MsgBox(UINT uType, LPCSTR pszFmt, ...);
extern BOOL GetFileName(LPSTR pszBuffer, DWORD dwBufLen, LPCSTR pszFilter,
    LPCSTR pszDefExt, BOOL fSave);
extern LPSTR BinToAscii(LPBYTE pBuffer, DWORD dwBufLen);
extern BOOL ParseHexValue(LPCSTR pszString, DWORD &dwValue);
extern DWORD ParseHexData(LPCSTR pszString, LPBYTE pBuffer, DWORD dwBufLen);
extern BOOL ParseHexDigit(LPCSTR pszString, BYTE &Byte);
extern DWORD GetErrorText(DWORD dwErrorCode, LPTSTR pszBuffer, DWORD dwBufLen);
extern LPCSTR GetLastErrorText();

/*
 * debug.cpp
 */
extern void MyTrace(LPCSTR pszFmt, ...);
extern void MyAssert(LPCSTR pszFile, unsigned uLine, LPCSTR pszExpr);

#ifdef _DEBUG
#define TRACE MyTrace
#define ASSERT(f) if (!(f)) MyAssert(__FILE__, __LINE__, #f)
#define VERIFY(f) if (!(f)) MyAssert(__FILE__, __LINE__, #f)
#else /* !_DEBUG */
#define ASSERT(f) ((void)0)
#define VERIFY(f) ((void)(f))
#define TRACE 1 ? (void)0 : MyTrace
#endif /* _DEBUG */

/*
 * data.cpp
 */
extern char g_szCreatingDatfiles[];
extern char g_szSofticeCodes[];
extern char g_szDiabloNotRunningMsg[];
extern char g_szDangerousCommandMsg[];

#endif /* _DIABHACK_H_ */