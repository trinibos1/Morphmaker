#define UNICODE
#define _UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>
#include <string>
#include <vector>
#include <cmath>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "shcore.lib")
#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
#pragma comment(linker, "/ENTRY:WinMainCRTStartup")
#pragma comment(linker, "\"/manifestdependency:type='win32' "  \
    "name='Microsoft.Windows.Common-Controls' version='6.0.0.0' " \
    "processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

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

#define ID_BTN_COLORPICK      180

#define WIN_W   1060
#define WIN_H   620

#define PAD_X       8
#define CTRL_PAD    4
#define GAP_X       10
#define TITLE_PAD   12
#define BOX_PAD     3
#define ROW_H       28
#define LABEL_W     80
#define INPUT_X     88
#define TITLE_H     13
#define BORDER_CLR  2

#define COL1_GX     8
#define COL1_CX     (COL1_GX + CTRL_PAD)
#define COL1_W      290

#define COL2_GX     (COL1_GX + COL1_W + GAP_X)
#define COL2_CX     (COL2_GX + CTRL_PAD)
#define COL2_W      220

#define COL3_GX     (COL2_GX + COL2_W + GAP_X)
#define COL3_CX     (COL3_GX + CTRL_PAD)
#define COL3_W      220

#define COL4_GX     (COL3_GX + COL3_W + GAP_X)
#define COL4_CX     (COL4_GX + CTRL_PAD)
#define COL4_W      284

#define ID_TITLE_Y      20
#define ID_BOX_Y        (ID_TITLE_Y + TITLE_PAD)
#define ID_CTRL_Y       (ID_BOX_Y + BOX_PAD)
#define ID_CTRL_END     (ID_CTRL_Y + 4 * ROW_H)
#define ID_BOX_H        (ID_CTRL_END + BOX_PAD - ID_BOX_Y)
#define ID_BOX_END      (ID_BOX_Y + ID_BOX_H)

#define CL_TITLE_Y      (ID_BOX_END + BORDER_CLR + 2)
#define CL_BOX_Y        (CL_TITLE_Y + TITLE_PAD)
#define CL_CTRL_Y       (CL_BOX_Y + BOX_PAD)
#define CL_CTRL_END     (CL_CTRL_Y + 2 * ROW_H)
#define CL_BOX_H        (CL_CTRL_END + BOX_PAD - CL_BOX_Y)
#define CL_BOX_END      (CL_BOX_Y + CL_BOX_H)

#define HAT_TITLE_Y     (CL_BOX_END + BORDER_CLR + 2)
#define HAT_BOX_Y       (HAT_TITLE_Y + TITLE_PAD)
#define HAT_CTRL_Y      (HAT_BOX_Y + BOX_PAD)
#define HAT_CTRL_END    389
#define HAT_BOX_H       (HAT_CTRL_END - HAT_BOX_Y)
#define HAT_BOX_END     (HAT_BOX_Y + HAT_BOX_H)

#define RIGHT_BOX_Y     ID_BOX_Y
#define RIGHT_BOX_END   HAT_BOX_END
#define RIGHT_BOX_H     (RIGHT_BOX_END - RIGHT_BOX_Y)

#define OUT_TITLE_Y     (HAT_BOX_END + BORDER_CLR + 2)
#define OUT_BOX_Y       (OUT_TITLE_Y + TITLE_PAD)
#define OUT_CTRL_Y      (OUT_BOX_Y + BOX_PAD)
#define OUT_BOX_H       130
#define OUT_BOX_END     (OUT_BOX_Y + OUT_BOX_H)

#define BTN_Y           (OUT_BOX_END + 8)
#define BTN_H           24

#define CW_SIZE         260
#define CW_RADIUS       110
#define CW_CENTER_X     (CW_SIZE / 2)
#define CW_CENTER_Y     (CW_SIZE / 2)
#define SV_SIZE         100
#define SV_X            20
#define SV_Y            (CW_SIZE + 10)

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
static HBRUSH hColorBrush = nullptr;

static HWND hColorWheelWnd = nullptr;
static HBITMAP hWheelBmp = nullptr;
static float gHue = 0.0f, gSat = 1.0f, gVal = 1.0f;
static bool gDraggingWheel = false;
static bool gDraggingSV = false;

static void HSVtoRGB(float h, float s, float v, int& r, int& g, int& b)
{
    h = fmodf(h, 360.0f);
    if (h < 0) h += 360.0f;
    float c = v * s;
    float x = c * (1.0f - fabsf(fmodf(h / 60.0f, 2.0f) - 1.0f));
    float m = v - c;
    float rr, gg, bb;
    if (h < 60) { rr = c; gg = x; bb = 0; }
    else if (h < 120) { rr = x; gg = c; bb = 0; }
    else if (h < 180) { rr = 0; gg = c; bb = x; }
    else if (h < 240) { rr = 0; gg = x; bb = c; }
    else if (h < 300) { rr = x; gg = 0; bb = c; }
    else { rr = c; gg = 0; bb = x; }
    r = (int)((rr + m) * 255.0f + 0.5f);
    g = (int)((gg + m) * 255.0f + 0.5f);
    b = (int)((bb + m) * 255.0f + 0.5f);
}

static void RGBtoHSV(int ri, int gi, int bi, float& h, float& s, float& v)
{
    float r = ri / 255.0f, g = gi / 255.0f, b = bi / 255.0f;
    float mx = max(r, max(g, b)), mn = min(r, min(g, b));
    float d = mx - mn;
    v = mx;
    s = (mx == 0.0f) ? 0.0f : d / mx;
    if (d == 0.0f) { h = 0.0f; return; }
    if (mx == r)      h = 60.0f * fmodf((g - b) / d, 6.0f);
    else if (mx == g) h = 60.0f * ((b - r) / d + 2.0f);
    else              h = 60.0f * ((r - g) / d + 4.0f);
    if (h < 0) h += 360.0f;
}

static HBITMAP CreateWheelBitmap(HWND hwnd)
{
    HDC hdc = GetDC(hwnd);
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateCompatibleBitmap(hdc, CW_SIZE, CW_SIZE);
    SelectObject(memDC, bmp);

    HBRUSH bg = CreateSolidBrush(RGB(230, 238, 250));
    RECT rc = { 0, 0, CW_SIZE, CW_SIZE };
    FillRect(memDC, &rc, bg);
    DeleteObject(bg);

    for (int py = 0; py < CW_SIZE; py++) {
        for (int px = 0; px < CW_SIZE; px++) {
            float dx = (float)(px - CW_CENTER_X);
            float dy = (float)(py - CW_CENTER_Y);
            float dist = sqrtf(dx * dx + dy * dy);
            if (dist <= CW_RADIUS) {
                float angle = atan2f(dy, dx) * (180.0f / (float)M_PI);
                if (angle < 0) angle += 360.0f;
                float sat = dist / CW_RADIUS;
                int r, g, b;
                HSVtoRGB(angle, sat, 1.0f, r, g, b);
                SetPixel(memDC, px, py, RGB(r, g, b));
            }
        }
    }

    DeleteDC(memDC);
    ReleaseDC(hwnd, hdc);
    return bmp;
}

static void DrawSVSquare(HDC hdc)
{
    for (int py = 0; py < SV_SIZE; py++) {
        for (int px = 0; px < SV_SIZE; px++) {
            float s = (float)px / (SV_SIZE - 1);
            float v = 1.0f - (float)py / (SV_SIZE - 1);
            int r, g, b;
            HSVtoRGB(gHue, s, v, r, g, b);
            SetPixel(hdc, SV_X + px, SV_Y + py, RGB(r, g, b));
        }
    }

    HPEN pen = CreatePen(PS_SOLID, 1, RGB(80, 80, 80));
    HPEN op = (HPEN)SelectObject(hdc, pen);
    HBRUSH ob = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, SV_X, SV_Y, SV_X + SV_SIZE, SV_Y + SV_SIZE);
    SelectObject(hdc, op);
    SelectObject(hdc, ob);
    DeleteObject(pen);

    int cx = SV_X + (int)(gSat * (SV_SIZE - 1));
    int cy = SV_Y + (int)((1.0f - gVal) * (SV_SIZE - 1));
    HPEN cp = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    op = (HPEN)SelectObject(hdc, cp);
    ob = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Ellipse(hdc, cx - 5, cy - 5, cx + 5, cy + 5);
    SelectObject(hdc, op);
    SelectObject(hdc, ob);
    DeleteObject(cp);
    HPEN cp2 = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    op = (HPEN)SelectObject(hdc, cp2);
    ob = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Ellipse(hdc, cx - 6, cy - 6, cx + 6, cy + 6);
    SelectObject(hdc, op);
    SelectObject(hdc, ob);
    DeleteObject(cp2);
}

static void DrawWheelCursor(HDC hdc)
{
    float angle = gHue * (float)M_PI / 180.0f;
    int cx = CW_CENTER_X + (int)(gSat * CW_RADIUS * cosf(angle));
    int cy = CW_CENTER_Y + (int)(gSat * CW_RADIUS * sinf(angle));

    HPEN wp = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    HPEN op = (HPEN)SelectObject(hdc, wp);
    HBRUSH ob = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Ellipse(hdc, cx - 7, cy - 7, cx + 7, cy + 7);
    SelectObject(hdc, op);
    SelectObject(hdc, ob);
    DeleteObject(wp);

    HPEN bp = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    op = (HPEN)SelectObject(hdc, bp);
    ob = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Ellipse(hdc, cx - 8, cy - 8, cx + 8, cy + 8);
    SelectObject(hdc, op);
    SelectObject(hdc, ob);
    DeleteObject(bp);
}

static void DrawPreviewSwatch(HDC hdc, int wndW)
{
    int r, g, b;
    HSVtoRGB(gHue, gSat, gVal, r, g, b);
    HBRUSH br = CreateSolidBrush(RGB(r, g, b));
    int swX = SV_X + SV_SIZE + 14;
    int swY = SV_Y;
    RECT sw = { swX, swY, swX + 60, swY + SV_SIZE };
    FillRect(hdc, &sw, br);
    DeleteObject(br);

    HPEN pen = CreatePen(PS_SOLID, 1, RGB(80, 80, 80));
    HPEN op = (HPEN)SelectObject(hdc, pen);
    HBRUSH ob = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, swX, swY, swX + 60, swY + SV_SIZE);
    SelectObject(hdc, op);
    SelectObject(hdc, ob);
    DeleteObject(pen);

    SetBkColor(hdc, RGB(230, 238, 250));
    SetTextColor(hdc, RGB(30, 30, 60));

    wchar_t buf[32];
    swprintf_s(buf, L"R: %d", r);
    TextOutW(hdc, swX, swY + SV_SIZE + 6, buf, (int)wcslen(buf));
    swprintf_s(buf, L"G: %d", g);
    TextOutW(hdc, swX, swY + SV_SIZE + 22, buf, (int)wcslen(buf));
    swprintf_s(buf, L"B: %d", b);
    TextOutW(hdc, swX, swY + SV_SIZE + 38, buf, (int)wcslen(buf));
}

static void ApplyColorToMain()
{
    int r, g, b;
    HSVtoRGB(gHue, gSat, gVal, r, g, b);
    wchar_t buf[8];
    swprintf_s(buf, L"%d", r); SetWindowText(hTagR, buf);
    swprintf_s(buf, L"%d", g); SetWindowText(hTagG, buf);
    swprintf_s(buf, L"%d", b); SetWindowText(hTagB, buf);
    if (hColorBrush) DeleteObject(hColorBrush);
    hColorBrush = CreateSolidBrush(RGB(r, g, b));
    InvalidateRect(hColorPreview, nullptr, TRUE);
    UpdateWindow(hColorPreview);
}

static LRESULT CALLBACK ColorWheelProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_CREATE:
        hWheelBmp = CreateWheelBitmap(hwnd);
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        HBRUSH bg = CreateSolidBrush(RGB(230, 238, 250));
        RECT rc; GetClientRect(hwnd, &rc);
        FillRect(hdc, &rc, bg);
        DeleteObject(bg);

        if (hWheelBmp) {
            HDC memDC = CreateCompatibleDC(hdc);
            SelectObject(memDC, hWheelBmp);
            BitBlt(hdc, 0, 0, CW_SIZE, CW_SIZE, memDC, 0, 0, SRCCOPY);
            DeleteDC(memDC);
        }

        DrawWheelCursor(hdc);
        DrawSVSquare(hdc);

        RECT cr; GetClientRect(hwnd, &cr);
        DrawPreviewSwatch(hdc, cr.right);

        SetBkColor(hdc, RGB(230, 238, 250));
        SetTextColor(hdc, RGB(30, 30, 60));

        HFONT f = CreateFontW(13, 0, 0, 0, FW_BOLD, 0, 0, 0,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Segoe UI");
        HFONT of = (HFONT)SelectObject(hdc, f);
        const wchar_t* t1 = L"Hue / Saturation";
        TextOutW(hdc, CW_CENTER_X - 50, 2, t1, (int)wcslen(t1));
        const wchar_t* t2 = L"Brightness";
        TextOutW(hdc, SV_X, SV_Y - 16, t2, (int)wcslen(t2));
        SelectObject(hdc, of);
        DeleteObject(f);

        EndPaint(hwnd, &ps);
        break;
    }

    case WM_LBUTTONDOWN:
    case WM_MOUSEMOVE:
    {
        if (msg == WM_LBUTTONDOWN) SetCapture(hwnd);
        if (!(wp & MK_LBUTTON) && msg == WM_MOUSEMOVE) break;

        int mx = LOWORD(lp), my = HIWORD(lp);

        if (mx >= SV_X && mx < SV_X + SV_SIZE && my >= SV_Y && my < SV_Y + SV_SIZE) {
            if (msg == WM_LBUTTONDOWN) { gDraggingSV = true; gDraggingWheel = false; }
        }
        else {
            float dx = (float)(mx - CW_CENTER_X);
            float dy = (float)(my - CW_CENTER_Y);
            float dist = sqrtf(dx * dx + dy * dy);
            if (dist <= CW_RADIUS + 8 && msg == WM_LBUTTONDOWN) {
                gDraggingWheel = true; gDraggingSV = false;
            }
        }

        if (gDraggingSV) {
            float s = (float)(mx - SV_X) / (SV_SIZE - 1);
            float v = 1.0f - (float)(my - SV_Y) / (SV_SIZE - 1);
            gSat = max(0.0f, min(1.0f, s));
            gVal = max(0.0f, min(1.0f, v));
            ApplyColorToMain();
            InvalidateRect(hwnd, nullptr, FALSE);
        }
        else if (gDraggingWheel) {
            float dx = (float)(mx - CW_CENTER_X);
            float dy = (float)(my - CW_CENTER_Y);
            float dist = sqrtf(dx * dx + dy * dy);
            gHue = atan2f(dy, dx) * (180.0f / (float)M_PI);
            if (gHue < 0) gHue += 360.0f;
            gSat = min(dist / CW_RADIUS, 1.0f);
            ApplyColorToMain();
            InvalidateRect(hwnd, nullptr, FALSE);
        }
        break;
    }

    case WM_LBUTTONUP:
        gDraggingWheel = false;
        gDraggingSV = false;
        ReleaseCapture();
        break;

    case WM_CTLCOLORSTATIC:
    {
        SetBkColor((HDC)wp, RGB(230, 238, 250));
        SetTextColor((HDC)wp, RGB(30, 30, 60));
        static HBRUSH br = CreateSolidBrush(RGB(230, 238, 250));
        return (LRESULT)br;
    }

    case WM_DESTROY:
        if (hWheelBmp) { DeleteObject(hWheelBmp); hWheelBmp = nullptr; }
        hColorWheelWnd = nullptr;
        break;
    }
    return DefWindowProc(hwnd, msg, wp, lp);
}

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
    RoundRect(dc, x, y + TITLE_PAD, x + w, y + h, 6, 6);
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

    if (IsChecked(hChkMorph))
        cmds.push_back(L"permmorph " + user + L" remove");

    if (IsChecked(hChkClearSG))
        cmds.push_back(L"Clearstartergear " + user);

    if (IsChecked(hChkRemoveTools))
        cmds.push_back(L"Removetools " + user);

    std::wstring shirt = GetText(hShirtID);
    if (!shirt.empty())
        cmds.push_back(L"permshirt " + user + L" " + shirt);

    std::wstring pants = GetText(hPantsID);
    if (!pants.empty())
        cmds.push_back(L"permpants " + user + L" " + pants);

    auto hats = GetListItems(hHatList);
    if (!hats.empty()) {
        std::wstring hs = L"permhat " + user + L" ";
        for (size_t i = 0; i < hats.size(); i++) {
            if (i) hs += L",";
            hs += hats[i];
        }
        cmds.push_back(hs);
    }

    std::wstring rtag = GetText(hRoleTag);
    if (!rtag.empty())
        cmds.push_back(L"permrtag " + user + L" " + rtag);

    std::wstring ntag = GetText(hNtag);
    if (!ntag.empty())
        cmds.push_back(L"permntag " + user + L" " + ntag);

    cmds.push_back(L"permcrtag " + user + L" " +
        std::to_wstring(ClampRGB(GetText(hTagR))) + L" " +
        std::to_wstring(ClampRGB(GetText(hTagG))) + L" " +
        std::to_wstring(ClampRGB(GetText(hTagB))));

    int hp = (int)SendMessage(hSldHealth, TBM_GETPOS, 0, 0);
    if (hp != 100)
        cmds.push_back(L"permhealth " + user + L" " + std::to_wstring(hp));

    int dmgRaw = (int)SendMessage(hSldDamage, TBM_GETPOS, 0, 0);
    if (dmgRaw != 100) {
        wchar_t buf[16];
        swprintf_s(buf, 16, L"%.2f", dmgRaw / 100.0);
        cmds.push_back(L"permdamagemultiplier " + user + L" " + buf);
    }

    if (IsChecked(hChkCanrk))
        cmds.push_back(L"permcanrk " + user + L" true");

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

    std::wstring result = L"run ";
    for (size_t i = 0; i < cmds.size(); i++) {
        if (i > 0) result += L" & ";
        result += cmds[i];
    }
    return result;
}

static void OpenColorWheel(HWND parent, HINSTANCE hInst)
{
    if (hColorWheelWnd) {
        SetForegroundWindow(hColorWheelWnd);
        return;
    }

    int r = ClampRGB(GetText(hTagR));
    int g = ClampRGB(GetText(hTagG));
    int b = ClampRGB(GetText(hTagB));
    RGBtoHSV(r, g, b, gHue, gSat, gVal);

    int winW = CW_SIZE + 10;
    int winH = SV_Y + SV_SIZE + 80;

    hColorWheelWnd = CreateWindowW(
        L"ColorWheelClass",
        L"Pick Tag Color",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT,
        winW, winH,
        parent, nullptr, hInst, nullptr
    );

    HFONT hFont = CreateFontW(13, 0, 0, 0, FW_NORMAL, 0, 0, 0,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Segoe UI");
    EnumChildWindows(hColorWheelWnd, [](HWND h, LPARAM lp) -> BOOL {
        SendMessage(h, WM_SETFONT, lp, TRUE);
        return TRUE;
        }, (LPARAM)hFont);

    ShowWindow(hColorWheelWnd, SW_SHOW);
    UpdateWindow(hColorWheelWnd);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        int x = COL1_CX, y = ID_CTRL_Y;
        Label(hwnd, L"Username:", x, y, LABEL_W, 18);
        hUsername = Edit(hwnd, ID_USERNAME, x + INPUT_X, y, 170, 22);

        y += ROW_H;
        Label(hwnd, L"Role Tag:", x, y, LABEL_W, 18);
        hRoleTag = Edit(hwnd, ID_ROLE_TAG, x + INPUT_X, y, 170, 22, L"e.g. VCC Biohazard | Recruit");

        y += ROW_H;
        Label(hwnd, L"Name Tag:", x, y, LABEL_W, 18);
        hNtag = Edit(hwnd, ID_NTAG, x + INPUT_X, y, 170, 22, L"e.g. [LR] Oliver");

        y += ROW_H;
        Label(hwnd, L"Tag R G B:", x, y, LABEL_W, 18);
        hTagR = Edit(hwnd, ID_TAG_R, x + INPUT_X, y, 40, 22, L"255");
        hTagG = Edit(hwnd, ID_TAG_G, x + INPUT_X + 48, y, 40, 22, L"255");
        hTagB = Edit(hwnd, ID_TAG_B, x + INPUT_X + 96, y, 40, 22, L"255");
        hColorPreview = CreateWindowW(L"STATIC", nullptr,
            WS_CHILD | WS_VISIBLE | WS_BORDER,
            x + INPUT_X + 144, y, 36, 22, hwnd, (HMENU)ID_COLOR_PREVIEW, nullptr, nullptr);
        Btn(hwnd, ID_BTN_COLORPICK, L"...", x + INPUT_X + 186, y, 22, 22);

        y = CL_CTRL_Y;
        Label(hwnd, L"Shirt ID:", x, y, LABEL_W, 18);
        hShirtID = Edit(hwnd, ID_SHIRT_ID, x + INPUT_X, y, 170, 22, L"Roblox asset ID");

        y += ROW_H;
        Label(hwnd, L"Pants ID:", x, y, LABEL_W, 18);
        hPantsID = Edit(hwnd, ID_PANTS_ID, x + INPUT_X, y, 170, 22, L"Roblox asset ID");

        y = HAT_CTRL_Y;
        Label(hwnd, L"Hat/Accessory ID or name:", x, y, 210, 18);
        y += 20;
        hHatInput = Edit(hwnd, ID_HAT_INPUT, x, y, 210, 22, L"e.g. 107189833 or holster");
        Btn(hwnd, ID_HAT_ADD, L"Add", x + 220, y, 60, 22);
        y += 28;
        hHatList = ListBox(hwnd, ID_HAT_LIST, x, y, 210, 100);
        Btn(hwnd, ID_HAT_REMOVE, L"Remove", x + 220, y, 60, 22);

        int x2 = COL2_CX;
        y = ID_CTRL_Y;
        Label(hwnd, L"Gun name or ID:", x2, y, 150, 18);
        y += 20;
        hGunInput = Edit(hwnd, ID_GUN_INPUT, x2, y, 150, 22, L"e.g. pistol or m4");
        Btn(hwnd, ID_GUN_ADD, L"Add", x2 + 160, y, 60, 22);
        y += 28;
        hGunList = ListBox(hwnd, ID_GUN_LIST, x2, y, 150, 290);
        Btn(hwnd, ID_GUN_REMOVE, L"Remove", x2 + 160, y, 60, 22);

        int x3 = COL3_CX;
        y = ID_CTRL_Y;
        Label(hwnd, L"Gear/Equipment name:", x3, y, 150, 18);
        y += 20;
        hGearInput = Edit(hwnd, ID_GEAR_INPUT, x3, y, 150, 22, L"e.g. radio or medkit");
        Btn(hwnd, ID_GEAR_ADD, L"Add", x3 + 160, y, 60, 22);
        y += 28;
        hGearList = ListBox(hwnd, ID_GEAR_LIST, x3, y, 150, 290);
        Btn(hwnd, ID_GEAR_REMOVE, L"Remove", x3 + 160, y, 60, 22);

        int x4 = COL4_CX;
        y = ID_CTRL_Y;
        hChkMorph = Chk(hwnd, ID_CHK_MORPH, L"Remove existing morph", x4, y, 250, 20, true);
        y += ROW_H;
        hChkClearSG = Chk(hwnd, ID_CHK_CLEARSG, L"Clear starter gear", x4, y, 250, 20, true);
        y += ROW_H;
        hChkRemoveTools = Chk(hwnd, ID_CHK_REMOVETOOLS, L"Remove tools", x4, y, 250, 20, true);
        y += ROW_H;
        hChkCanrk = Chk(hwnd, ID_CHK_CANRK, L"permcanrk (can re-kill)", x4, y, 250, 20, false);

        y += ROW_H + 10;
        Label(hwnd, L"Max Health (75-200 HP):", x4, y, 250, 18);
        y += 22;
        hSldHealth = Slider(hwnd, ID_SLD_HEALTH, x4, y, 190, 28, 75, 200, 100);
        hLblHealth = Label(hwnd, L"100 HP", x4 + 196, y + 4, 50, 18);

        y += ROW_H + 14;
        Label(hwnd, L"Damage Multiplier (0.25x-4x):", x4, y, 250, 18);
        y += 22;
        hSldDamage = Slider(hwnd, ID_SLD_DAMAGE, x4, y, 190, 28, 25, 400, 100);
        hLblDamage = Label(hwnd, L"1.00x", x4 + 196, y + 4, 50, 18);

        y = OUT_CTRL_Y;
        Label(hwnd, L"Generated Command (paste into Roblox chat):", COL1_CX, y, 500, 18);
        y += 20;
        hOutput = Edit(hwnd, ID_OUTPUT, COL1_CX, y, WIN_W - 24, 110, L"", true);
        SendMessage(hOutput, EM_SETREADONLY, TRUE, 0);

        y = BTN_Y;
        Btn(hwnd, ID_BTN_GENERATE, L"Generate Command", COL1_CX, y, 160, BTN_H);
        Btn(hwnd, ID_BTN_COPY, L"Copy to Clipboard", COL1_CX + 170, y, 160, BTN_H);
        Btn(hwnd, ID_BTN_CLEAR, L"Clear All", COL1_CX + 340, y, 100, BTN_H);
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

    case WM_COMMAND:
    {
        int id = LOWORD(wp);
        int evt = HIWORD(wp);

        if ((id == ID_TAG_R || id == ID_TAG_G || id == ID_TAG_B) && evt == EN_KILLFOCUS) {
            if (hColorBrush) DeleteObject(hColorBrush);
            hColorBrush = CreateSolidBrush(RGB(
                ClampRGB(GetText(hTagR)),
                ClampRGB(GetText(hTagG)),
                ClampRGB(GetText(hTagB))));
            InvalidateRect(hColorPreview, nullptr, TRUE);
            UpdateWindow(hColorPreview);
            if (hColorWheelWnd) {
                int r = ClampRGB(GetText(hTagR));
                int g = ClampRGB(GetText(hTagG));
                int b = ClampRGB(GetText(hTagB));
                RGBtoHSV(r, g, b, gHue, gSat, gVal);
                InvalidateRect(hColorWheelWnd, nullptr, FALSE);
            }
        }

        if (id == ID_BTN_COLORPICK) {
            HINSTANCE hInst = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
            OpenColorWheel(hwnd, hInst);
        }
        else if (id == ID_HAT_ADD) {
            std::wstring hat = GetText(hHatInput);
            if (!hat.empty()) {
                SendMessage(hHatList, LB_ADDSTRING, 0, (LPARAM)hat.c_str());
                SetText(hHatInput, L"");
                SetFocus(hHatInput);
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
                SetFocus(hGunInput);
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
                SetFocus(hGearInput);
            }
        }
        else if (id == ID_GEAR_REMOVE) {
            int sel = (int)SendMessage(hGearList, LB_GETCURSEL, 0, 0);
            if (sel != LB_ERR) SendMessage(hGearList, LB_DELETESTRING, sel, 0);
        }
        else if (evt == EN_UPDATE && id == ID_HAT_INPUT) {
            if (GetAsyncKeyState(VK_RETURN) & 0x8000)
                PostMessage(hwnd, WM_COMMAND, MAKEWPARAM(ID_HAT_ADD, BN_CLICKED), 0);
        }
        else if (evt == EN_UPDATE && id == ID_GUN_INPUT) {
            if (GetAsyncKeyState(VK_RETURN) & 0x8000)
                PostMessage(hwnd, WM_COMMAND, MAKEWPARAM(ID_GUN_ADD, BN_CLICKED), 0);
        }
        else if (evt == EN_UPDATE && id == ID_GEAR_INPUT) {
            if (GetAsyncKeyState(VK_RETURN) & 0x8000)
                PostMessage(hwnd, WM_COMMAND, MAKEWPARAM(ID_GEAR_ADD, BN_CLICKED), 0);
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
                    wchar_t* ptr = (wchar_t*)GlobalLock(hMem);
                    if (ptr) {
                        wmemcpy(ptr, text.c_str(), text.size() + 1);
                        GlobalUnlock(hMem);
                        if (SetClipboardData(CF_UNICODETEXT, hMem))
                            MessageBoxW(hwnd, L"Command copied to clipboard!", L"Success", MB_OK | MB_ICONINFORMATION);
                        else
                            GlobalFree(hMem);
                    }
                    else {
                        GlobalFree(hMem);
                    }
                }
                CloseClipboard();
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
            gHue = 0.0f; gSat = 0.0f; gVal = 1.0f;
            if (hColorWheelWnd) InvalidateRect(hColorWheelWnd, nullptr, FALSE);
        }
        break;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        SetBkMode(hdc, TRANSPARENT);

        DrawBox(hdc, COL1_GX, ID_TITLE_Y, COL1_W, ID_BOX_H, L" Identity & Tags ");
        DrawBox(hdc, COL1_GX, CL_TITLE_Y, COL1_W, CL_BOX_H, L" Clothing ");
        DrawBox(hdc, COL1_GX, HAT_TITLE_Y, COL1_W, HAT_BOX_H, L" Hats & Accessories ");
        DrawBox(hdc, COL2_GX, ID_TITLE_Y, COL2_W, RIGHT_BOX_H, L" Guns ");
        DrawBox(hdc, COL3_GX, ID_TITLE_Y, COL3_W, RIGHT_BOX_H, L" Gear ");
        DrawBox(hdc, COL4_GX, ID_TITLE_Y, COL4_W, RIGHT_BOX_H, L" Options & Stats ");
        DrawBox(hdc, COL1_GX, OUT_TITLE_Y, WIN_W - 16, OUT_BOX_H, L" Output Command ");

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
        if ((HWND)lp == hColorPreview) {
            if (!hColorBrush)
                hColorBrush = CreateSolidBrush(RGB(255, 255, 255));
            return (LRESULT)hColorBrush;
        }
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
    SetProcessDPIAware();

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

    WNDCLASSEX cwc = {};
    cwc.cbSize = sizeof(cwc);
    cwc.style = CS_HREDRAW | CS_VREDRAW;
    cwc.lpfnWndProc = ColorWheelProc;
    cwc.hInstance = hInst;
    cwc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    cwc.hbrBackground = CreateSolidBrush(RGB(230, 238, 250));
    cwc.lpszClassName = L"ColorWheelClass";
    RegisterClassEx(&cwc);

    HWND hwnd = CreateWindowW(
        L"SCPMorphBuilder",
        L"SCP:RP Morph Builder v1.0",
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