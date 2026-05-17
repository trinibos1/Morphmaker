#define UNICODE
#define _UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>
#include <string>
#include <vector>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
#pragma comment(linker, "/ENTRY:WinMainCRTStartup")
#pragma comment(linker, "\"/manifestdependency:type='win32' "  \
    "name='Microsoft.Windows.Common-Controls' version='6.0.0.0' " \
    "processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")


#define ID_USERNAME           100
#define ID_ROLE_TAG           101
#define ID_NTAG               102
#define ID_TAG_R              103
#define ID_TAG_G              104
#define ID_TAG_B              105
#define ID_COLOR_PREVIEW      106

#define ID_SHIRT_ID           110
#define ID_PANTS_ID           111

#define ID_HAT_INPUT          120
#define ID_HAT_ADD            121
#define ID_HAT_LIST           122
#define ID_HAT_REMOVE         123

#define ID_GUN_INPUT          130
#define ID_GUN_ADD            131
#define ID_GUN_LIST           132
#define ID_GUN_REMOVE         133

#define ID_GEAR_INPUT         140
#define ID_GEAR_ADD           141
#define ID_GEAR_LIST          142
#define ID_GEAR_REMOVE        143

#define ID_CHK_MORPH          150
#define ID_CHK_CLEARSG        151
#define ID_CHK_REMOVETOOLS    152
#define ID_CHK_CANRK          153

#define ID_SLD_HEALTH         160
#define ID_LBL_HEALTH         161
#define ID_SLD_DAMAGE         162
#define ID_LBL_DAMAGE         163

#define ID_BTN_GENERATE       170
#define ID_BTN_COPY           171
#define ID_BTN_CLEAR          172
#define ID_OUTPUT             173


#define WIN_W   920
#define WIN_H   950


static HWND hUsername, hRoleTag, hNtag;
static HWND hTagR, hTagG, hTagB, hColorPreview;
static HWND hShirtID, hPantsID;
static HWND hHatInput, hHatList;
static HWND hGunInput, hGunList;
static HWND hGearInput, hGearList;
static HWND hChkMorph, hChkClearSG, hChkRemoveTools, hChkCanrk;
static HWND hSldHealth, hLblHealth;
static HWND hSldDamage, hLblDamage;
static HWND hOutput;


static std::wstring GetText(HWND h)
{
    int n = GetWindowTextLength(h) + 1;
    std::vector<wchar_t> buf(n);
    GetWindowText(h, buf.data(), n);
    return std::wstring(buf.data());
}

static void SetText(HWND h, const std::wstring& s)
{
    SetWindowText(h, s.c_str());
}

static int ClampRGB(const std::wstring& s)
{
    try { int v = std::stoi(s); return v < 0 ? 0 : v > 255 ? 255 : v; }
    catch (...) { return 0; }
}

static bool IsChecked(HWND h)
{
    return SendMessage(h, BM_GETCHECK, 0, 0) == BST_CHECKED;
}

static std::vector<std::wstring> GetListItems(HWND lb)
{
    std::vector<std::wstring> items;
    int n = (int)SendMessage(lb, LB_GETCOUNT, 0, 0);
    for (int i = 0; i < n; i++) {
        int len = (int)SendMessage(lb, LB_GETTEXTLEN, i, 0);
        std::vector<wchar_t> buf(len + 1);
        SendMessage(lb, LB_GETTEXT, i, (LPARAM)buf.data());
        items.push_back(buf.data());
    }
    return items;
}


static HWND Label(HWND p, const wchar_t* t, int x, int y, int w, int h)
{
    return CreateWindowW(L"STATIC", t, WS_CHILD | WS_VISIBLE | SS_LEFT,
        x, y, w, h, p, nullptr, nullptr, nullptr);
}

static HWND Edit(HWND p, int id, int x, int y, int w, int h,
    const wchar_t* hint = L"", bool multi = false)
{
    DWORD s = WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL;
    if (multi) s |= ES_MULTILINE | WS_VSCROLL | ES_AUTOVSCROLL;
    return CreateWindowW(L"EDIT", hint, s, x, y, w, h,
        p, (HMENU)(intptr_t)id, nullptr, nullptr);
}

static HWND Btn(HWND p, int id, const wchar_t* t, int x, int y, int w, int h)
{
    return CreateWindowW(L"BUTTON", t, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        x, y, w, h, p, (HMENU)(intptr_t)id, nullptr, nullptr);
}

static HWND Chk(HWND p, int id, const wchar_t* t, int x, int y, int w, int h, bool on = false)
{
    HWND hw = CreateWindowW(L"BUTTON", t, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX,
        x, y, w, h, p, (HMENU)(intptr_t)id, nullptr, nullptr);
    if (on) SendMessage(hw, BM_SETCHECK, BST_CHECKED, 0);
    return hw;
}

static HWND ListBox(HWND p, int id, int x, int y, int w, int h)
{
    return CreateWindowW(L"LISTBOX", nullptr,
        WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_NOTIFY,
        x, y, w, h, p, (HMENU)(intptr_t)id, nullptr, nullptr);
}

static HWND Slider(HWND p, int id, int x, int y, int w, int h, int lo, int hi, int init)
{
    HWND hw = CreateWindowW(TRACKBAR_CLASS, nullptr,
        WS_CHILD | WS_VISIBLE | TBS_HORZ | TBS_AUTOTICKS,
        x, y, w, h, p, (HMENU)(intptr_t)id, nullptr, nullptr);
    SendMessage(hw, TBM_SETRANGE, TRUE, MAKELPARAM(lo, hi));
    SendMessage(hw, TBM_SETPOS, TRUE, init);
    return hw;
}

static void DrawBox(HDC dc, int x, int y, int w, int h, const wchar_t* title)
{
    HPEN pen = CreatePen(PS_SOLID, 1, RGB(110, 140, 180));
    HPEN op = (HPEN)SelectObject(dc, pen);
    HBRUSH ob = (HBRUSH)SelectObject(dc, GetStockObject(NULL_BRUSH));
    RoundRect(dc, x, y + 9, x + w, y + h, 6, 6);
    SelectObject(dc, op);
    SelectObject(dc, ob);
    DeleteObject(pen);

    SetBkColor(dc, RGB(238, 244, 252));
    SetTextColor(dc, RGB(15, 65, 155));
    HFONT f = CreateFontW(13, 0, 0, 0, FW_BOLD, 0, 0, 0,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Segoe UI");
    HFONT of = (HFONT)SelectObject(dc, f);
    TextOutW(dc, x + 8, y, title, (int)wcslen(title));
    SelectObject(dc, of);
    DeleteObject(f);
}


static std::wstring GenerateCommand()
{
    std::wstring user = GetText(hUsername);
    if (user.empty()) return L"[ERROR: Enter a username first!]";

    std::vector<std::wstring> cmds;

    // Clear morph
    if (IsChecked(hChkMorph))
        cmds.push_back(L"permmorph " + user + L" remove");

    // Clear starter gear
    if (IsChecked(hChkClearSG))
        cmds.push_back(L"Clearstartergear " + user);

    // Remove tools
    if (IsChecked(hChkRemoveTools))
        cmds.push_back(L"Removetools " + user);

    // Shirt
    std::wstring shirt = GetText(hShirtID);
    if (!shirt.empty())
        cmds.push_back(L"permshirt " + user + L" " + shirt);

    // Pants
    std::wstring pants = GetText(hPantsID);
    if (!pants.empty())
        cmds.push_back(L"permpants " + user + L" " + pants);

    // Hats
    auto hats = GetListItems(hHatList);
    if (!hats.empty()) {
        std::wstring hs = L"permhat " + user + L" ";
        for (size_t i = 0; i < hats.size(); i++) {
            if (i) hs += L",";
            hs += hats[i];
        }
        cmds.push_back(hs);
    }

    // Role tag
    std::wstring rtag = GetText(hRoleTag);
    if (!rtag.empty())
        cmds.push_back(L"permrtag " + user + L" " + rtag);

    // Ntag
    std::wstring ntag = GetText(hNtag);
    if (!ntag.empty())
        cmds.push_back(L"permntag " + user + L" " + ntag);

    // Tag colour
    cmds.push_back(L"permcrtag " + user + L" " +
        std::to_wstring(ClampRGB(GetText(hTagR))) + L" " +
        std::to_wstring(ClampRGB(GetText(hTagG))) + L" " +
        std::to_wstring(ClampRGB(GetText(hTagB))));

    // Max health
    int hp = (int)SendMessage(hSldHealth, TBM_GETPOS, 0, 0);
    if (hp != 100)
        cmds.push_back(L"permhealth " + user + L" " + std::to_wstring(hp));

    // Damage multiplier
    int dmgRaw = (int)SendMessage(hSldDamage, TBM_GETPOS, 0, 0);
    if (dmgRaw != 100) {
        wchar_t buf[16];
        swprintf_s(buf, 16, L"%.2f", dmgRaw / 100.0);
        cmds.push_back(L"permdamagemultiplier " + user + L" " + buf);
    }

    // canrk
    if (IsChecked(hChkCanrk))
        cmds.push_back(L"permcanrk " + user + L" true");

    // Starter gear (guns + gear combined)
    auto guns = GetListItems(hGunList);
    auto gear = GetListItems(hGearList);
    if (!guns.empty() || !gear.empty()) {
        std::wstring gs = L"startergear " + user + L" ";
        bool first = true;
        for (auto& x : guns) {
            if (!first) gs += L",";
            gs += x;
            first = false;
        }
        for (auto& x : gear) {
            if (!first) gs += L",";
            gs += x;
            first = false;
        }
        cmds.push_back(gs);
    }

    // Join with " & " and prefix with "run "
    std::wstring result = L"run ";
    for (size_t i = 0; i < cmds.size(); i++) {
        if (i > 0) result += L" & ";
        result += cmds[i];
    }
    return result;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        int x = 12, y = 16;
        hUsername = Edit(hwnd, ID_USERNAME, x + 100, y, 150, 22);

        Label(hwnd, L"Role Tag:", x, y + 28, 80, 18);
        hRoleTag = Edit(hwnd, ID_ROLE_TAG, x + 100, y + 28, 230, 22, L"e.g. VCC Biohazard | Recruit");

        Label(hwnd, L"Name Tag:", x, y + 56, 80, 18);
        hNtag = Edit(hwnd, ID_NTAG, x + 100, y + 56, 230, 22, L"e.g. [LR] Oliver");

        Label(hwnd, L"Tag R G B:", x, y + 84, 80, 18);
        hTagR = Edit(hwnd, ID_TAG_R, x + 100, y + 82, 40, 22, L"255");
        hTagG = Edit(hwnd, ID_TAG_G, x + 148, y + 82, 40, 22, L"255");
        hTagB = Edit(hwnd, ID_TAG_B, x + 196, y + 82, 40, 22, L"255");
        hColorPreview = CreateWindowW(L"STATIC", nullptr,
            WS_CHILD | WS_VISIBLE | SS_OWNERDRAW | WS_BORDER,
            x + 244, y + 82, 36, 22, hwnd, (HMENU)ID_COLOR_PREVIEW, nullptr, nullptr);


        y += 120;
        Label(hwnd, L"Shirt ID:", x, y, 80, 18);
        hShirtID = Edit(hwnd, ID_SHIRT_ID, x + 100, y, 150, 22, L"Roblox asset ID");

        Label(hwnd, L"Pants ID:", x, y + 28, 80, 18);
        hPantsID = Edit(hwnd, ID_PANTS_ID, x + 100, y + 28, 150, 22, L"Roblox asset ID");


        y += 68;
        Label(hwnd, L"Hat/Accessory ID or name:", x, y, 210, 18);
        hHatInput = Edit(hwnd, ID_HAT_INPUT, x, y + 20, 210, 22, L"e.g. 107189833 or holster");
        Btn(hwnd, ID_HAT_ADD, L"Add", x + 220, y + 20, 60, 22);
        hHatList = ListBox(hwnd, ID_HAT_LIST, x, y + 48, 210, 100);
        Btn(hwnd, ID_HAT_REMOVE, L"Remove", x + 220, y + 48, 60, 22);


        int x2 = x + 310;
        y = 16;
        Label(hwnd, L"Gun name or ID:", x2, y, 150, 18);
        hGunInput = Edit(hwnd, ID_GUN_INPUT, x2, y + 20, 150, 22, L"e.g. pistol or m4");
        Btn(hwnd, ID_GUN_ADD, L"Add", x2 + 160, y + 20, 60, 22);
        hGunList = ListBox(hwnd, ID_GUN_LIST, x2, y + 48, 150, 280);
        Btn(hwnd, ID_GUN_REMOVE, L"Remove", x2 + 160, y + 48, 60, 22);


        int x3 = x2 + 240;
        Label(hwnd, L"Gear/Equipment name:", x3, y, 150, 18);
        hGearInput = Edit(hwnd, ID_GEAR_INPUT, x3, y + 20, 150, 22, L"e.g. radio or medkit");
        Btn(hwnd, ID_GEAR_ADD, L"Add", x3 + 160, y + 20, 60, 22);
        hGearList = ListBox(hwnd, ID_GEAR_LIST, x3, y + 48, 150, 280);
        Btn(hwnd, ID_GEAR_REMOVE, L"Remove", x3 + 160, y + 48, 60, 22);


        y = 340;
        Label(hwnd, L"Options:", x, y, 200, 18);
        hChkMorph = Chk(hwnd, ID_CHK_MORPH, L"Remove existing morph", x, y + 22, 280, 20, true);
        hChkClearSG = Chk(hwnd, ID_CHK_CLEARSG, L"Clear starter gear", x, y + 46, 280, 20, true);
        hChkRemoveTools = Chk(hwnd, ID_CHK_REMOVETOOLS, L"Remove tools", x, y + 70, 280, 20, true);
        hChkCanrk = Chk(hwnd, ID_CHK_CANRK, L"permcanrk (can re-kill)", x, y + 94, 280, 20, false);


        y += 130;
        Label(hwnd, L"Max Health (75 - 200 HP):", x, y, 220, 18);
        hSldHealth = Slider(hwnd, ID_SLD_HEALTH, x, y + 20, 240, 28, 75, 200, 100);
        hLblHealth = Label(hwnd, L"100 HP", x + 250, y + 24, 60, 18);

        y += 50;
        Label(hwnd, L"Damage Multiplier (0.25x - 4.00x):", x, y, 280, 18);
        hSldDamage = Slider(hwnd, ID_SLD_DAMAGE, x, y + 20, 240, 28, 25, 400, 100);
        hLblDamage = Label(hwnd, L"1.00x", x + 250, y + 24, 60, 18);


        y = 630;
        Label(hwnd, L"Generated Command (paste into Roblox chat):", x, y, 500, 18);
        hOutput = Edit(hwnd, ID_OUTPUT, x, y + 20, WIN_W - 24, 120, L"", true);
        SendMessage(hOutput, EM_SETREADONLY, TRUE, 0);


        y += 130;
        Btn(hwnd, ID_BTN_GENERATE, L"Generate Command", x, y, 160, 28);
        Btn(hwnd, ID_BTN_COPY, L"Copy to Clipboard", x + 170, y, 160, 28);
        Btn(hwnd, ID_BTN_CLEAR, L"Clear All", x + 340, y, 100, 28);
        break;
    }


    case WM_HSCROLL:
    {
        if ((HWND)lp == hSldHealth) {
            int v = (int)SendMessage(hSldHealth, TBM_GETPOS, 0, 0);
            SetText(hLblHealth, std::to_wstring(v) + L" HP");
        }
        else if ((HWND)lp == hSldDamage) {
            int v = (int)SendMessage(hSldDamage, TBM_GETPOS, 0, 0);
            wchar_t buf[16];
            swprintf_s(buf, 16, L"%.2fx", v / 100.0);
            SetText(hLblDamage, buf);
        }
        break;
    }


    case WM_DRAWITEM:
    {
        auto* dis = (DRAWITEMSTRUCT*)lp;
        if (dis->CtlID == ID_COLOR_PREVIEW) {
            HBRUSH br = CreateSolidBrush(RGB(
                ClampRGB(GetText(hTagR)),
                ClampRGB(GetText(hTagG)),
                ClampRGB(GetText(hTagB))));
            FillRect(dis->hDC, &dis->rcItem, br);
            DeleteObject(br);
            return TRUE;
        }
        break;
    }


    case WM_COMMAND:
    {
        int id = LOWORD(wp);
        int evt = HIWORD(wp);

        if ((id == ID_TAG_R || id == ID_TAG_G || id == ID_TAG_B) && evt == EN_CHANGE)
            InvalidateRect(hColorPreview, nullptr, TRUE);

        if (id == ID_HAT_ADD) {
            std::wstring hat = GetText(hHatInput);
            if (!hat.empty()) {
                SendMessage(hHatList, LB_ADDSTRING, 0, (LPARAM)hat.c_str());
                SetText(hHatInput, L"");
            }
        }
        else if (id == ID_HAT_REMOVE) {
            int sel = (int)SendMessage(hHatList, LB_GETCURSEL, 0, 0);
            if (sel != LB_ERR) SendMessage(hHatList, LB_DELETESTRING, sel, 0);
        }
        else if (id == ID_GUN_ADD) {
            std::wstring gun = GetText(hGunInput);
            if (!gun.empty()) {
                SendMessage(hGunList, LB_ADDSTRING, 0, (LPARAM)gun.c_str());
                SetText(hGunInput, L"");
            }
        }
        else if (id == ID_GUN_REMOVE) {
            int sel = (int)SendMessage(hGunList, LB_GETCURSEL, 0, 0);
            if (sel != LB_ERR) SendMessage(hGunList, LB_DELETESTRING, sel, 0);
        }
        else if (id == ID_GEAR_ADD) {
            std::wstring gear = GetText(hGearInput);
            if (!gear.empty()) {
                SendMessage(hGearList, LB_ADDSTRING, 0, (LPARAM)gear.c_str());
                SetText(hGearInput, L"");
            }
        }
        else if (id == ID_GEAR_REMOVE) {
            int sel = (int)SendMessage(hGearList, LB_GETCURSEL, 0, 0);
            if (sel != LB_ERR) SendMessage(hGearList, LB_DELETESTRING, sel, 0);
        }
        else if (id == ID_BTN_GENERATE) {
            SetText(hOutput, GenerateCommand());
        }
        else if (id == ID_BTN_COPY) {
            std::wstring text = GetText(hOutput);
            if (text.empty()) {
                text = GenerateCommand();
                SetText(hOutput, text);
            }
            if (OpenClipboard(hwnd)) {
                EmptyClipboard();
                size_t sz = (text.size() + 1) * sizeof(wchar_t);
                HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, sz);
                if (hMem) {
                    memcpy(GlobalLock(hMem), text.c_str(), sz);
                    GlobalUnlock(hMem);
                    SetClipboardData(CF_UNICODETEXT, hMem);
                }
                CloseClipboard();
                MessageBoxW(hwnd, L"Command copied to clipboard!", L"Success", MB_OK | MB_ICONINFORMATION);
            }
        }
        else if (id == ID_BTN_CLEAR) {
            SetText(hUsername, L"");
            SetText(hRoleTag, L"");
            SetText(hNtag, L"");
            SetText(hTagR, L"255");
            SetText(hTagG, L"255");
            SetText(hTagB, L"255");
            SetText(hShirtID, L"");
            SetText(hPantsID, L"");
            SendMessage(hHatList, LB_RESETCONTENT, 0, 0);
            SendMessage(hGunList, LB_RESETCONTENT, 0, 0);
            SendMessage(hGearList, LB_RESETCONTENT, 0, 0);
            SendMessage(hSldHealth, TBM_SETPOS, TRUE, 100);
            SetText(hLblHealth, L"100 HP");
            SendMessage(hSldDamage, TBM_SETPOS, TRUE, 100);
            SetText(hLblDamage, L"1.00x");
            SendMessage(hChkMorph, BM_SETCHECK, BST_CHECKED, 0);
            SendMessage(hChkClearSG, BM_SETCHECK, BST_CHECKED, 0);
            SendMessage(hChkRemoveTools, BM_SETCHECK, BST_CHECKED, 0);
            SendMessage(hChkCanrk, BM_SETCHECK, BST_UNCHECKED, 0);
            SetText(hOutput, L"");
        }
        break;
    }


    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        SetBkMode(hdc, TRANSPARENT);

        DrawBox(hdc, 8, 10, 290, 107, L" Identity & Tags ");
        DrawBox(hdc, 8, 120, 290, 65, L" Clothing ");
        DrawBox(hdc, 8, 187, 290, 156, L" Hats & Accessories ");
        DrawBox(hdc, 310, 10, 180, 350, L" Guns ");
        DrawBox(hdc, 502, 10, 180, 350, L" Gear ");
        DrawBox(hdc, 694, 10, 210, 350, L" Options & Stats ");
        DrawBox(hdc, 8, 623, 900, 152, L" Output Command ");

        EndPaint(hwnd, &ps);
        break;
    }


    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORLISTBOX:
    {
        SetBkColor((HDC)wp, RGB(248, 251, 255));
        static HBRUSH br = CreateSolidBrush(RGB(248, 251, 255));
        return (LRESULT)br;
    }
    case WM_CTLCOLORSTATIC:
    {
        SetBkColor((HDC)wp, RGB(238, 244, 252));
        SetTextColor((HDC)wp, RGB(30, 30, 60));
        static HBRUSH br = CreateSolidBrush(RGB(238, 244, 252));
        return (LRESULT)br;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, msg, wp, lp);
}


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nShow)
{
    INITCOMMONCONTROLSEX icc = { sizeof(icc), ICC_WIN95_CLASSES | ICC_BAR_CLASSES };
    InitCommonControlsEx(&icc);

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(RGB(238, 244, 252));
    wc.lpszClassName = L"SCPMorphBuilder";
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    RegisterClassEx(&wc);

    HWND hwnd = CreateWindowW(
        L"SCPMorphBuilder",
        L"SCP:RP Morph Builder v2",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT,
        WIN_W, WIN_H,
        nullptr, nullptr, hInst, nullptr
    );

    HFONT hFont = CreateFontW(14, 0, 0, 0, FW_NORMAL, 0, 0, 0,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Segoe UI");
    EnumChildWindows(hwnd, [](HWND h, LPARAM lp) -> BOOL {
        SendMessage(h, WM_SETFONT, lp, TRUE);
        return TRUE;
        }, (LPARAM)hFont);

    ShowWindow(hwnd, nShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}