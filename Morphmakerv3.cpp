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
#pragma comment(lib, "shcore.lib")
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


#define WIN_W   1060
#define WIN_H   620

// ── Layout Constants ────────────────────────────────────────────────
// DrawBox draws title at (gx+8, gy) and rounded rect from (gx, gy+9) to (gx+w, gy+h)
// Title text is ~13px tall, so it occupies [gy, gy+13].
// The box top edge is at gy+9, so title naturally sits 4px into the box top.
// CRITICAL: title must NOT overlap previous section's box bottom.
// Previous box bottom = prev_box_y + prev_box_h
// Next title_y must be >= prev_box_bottom + 2 (clearance)

#define PAD_X       8       // Window edge to group box
#define CTRL_PAD    4       // Group box edge to control x
#define GAP_X       10      // Horizontal gap between columns
#define TITLE_PAD   12      // Space between title text and box top edge (DPI safety margin)
#define BOX_PAD     3       // Padding inside box before controls
#define ROW_H       28      // Form row height
#define LABEL_W     80      // Label width
#define INPUT_X     88      // Input x offset (label + gap)
#define TITLE_H     13      // Approximate title text height
#define BORDER_CLR  2       // Clearance from previous box bottom

// Column positions
#define COL1_GX     8
#define COL1_CX     (COL1_GX + CTRL_PAD)          // 12
#define COL1_W      290

#define COL2_GX     (COL1_GX + COL1_W + GAP_X)    // 308
#define COL2_CX     (COL2_GX + CTRL_PAD)          // 312
#define COL2_W      220

#define COL3_GX     (COL2_GX + COL2_W + GAP_X)    // 538
#define COL3_CX     (COL3_GX + CTRL_PAD)          // 542
#define COL3_W      220

#define COL4_GX     (COL3_GX + COL3_W + GAP_X)    // 768
#define COL4_CX     (COL4_GX + CTRL_PAD)          // 772
#define COL4_W      284

// ── Vertical Layout (calculated to prevent title/box overlap) ───────
// Identity & Tags (4 rows)
#define ID_TITLE_Y      20
#define ID_BOX_Y        (ID_TITLE_Y + TITLE_PAD)      // 29
#define ID_CTRL_Y       (ID_BOX_Y + BOX_PAD)          // 32
#define ID_CTRL_END     (ID_CTRL_Y + 4 * ROW_H)       // 144
#define ID_BOX_H        (ID_CTRL_END + BOX_PAD - ID_BOX_Y)  // 118
#define ID_BOX_END      (ID_BOX_Y + ID_BOX_H)         // 147

// Clothing (2 rows) - title clears Identity box bottom + 2px
#define CL_TITLE_Y      (ID_BOX_END + BORDER_CLR + 2) // 151
#define CL_BOX_Y        (CL_TITLE_Y + TITLE_PAD)      // 160
#define CL_CTRL_Y       (CL_BOX_Y + BOX_PAD)          // 163
#define CL_CTRL_END     (CL_CTRL_Y + 2 * ROW_H)       // 219
#define CL_BOX_H        (CL_CTRL_END + BOX_PAD - CL_BOX_Y)  // 62
#define CL_BOX_END      (CL_BOX_Y + CL_BOX_H)         // 222

// Hats & Accessories - title clears Clothing box bottom + 2px
#define HAT_TITLE_Y     (CL_BOX_END + BORDER_CLR + 2) // 226
#define HAT_BOX_Y       (HAT_TITLE_Y + TITLE_PAD)     // 235
#define HAT_CTRL_Y      (HAT_BOX_Y + BOX_PAD)         // 238
// Controls: label(238) input(258) list(286) list_h=100 list_end=386
#define HAT_CTRL_END    389
#define HAT_BOX_H       (HAT_CTRL_END - HAT_BOX_Y)    // 154
#define HAT_BOX_END     (HAT_BOX_Y + HAT_BOX_H)       // 389

// Guns, Gear, Options boxes span from Identity top to Hats bottom
#define RIGHT_BOX_Y     ID_BOX_Y                      // 29
#define RIGHT_BOX_END   HAT_BOX_END                   // 389
#define RIGHT_BOX_H     (RIGHT_BOX_END - RIGHT_BOX_Y) // 360

// Output Command - title clears Hats box bottom + 2px
#define OUT_TITLE_Y     (HAT_BOX_END + BORDER_CLR + 2)  // 393
#define OUT_BOX_Y       (OUT_TITLE_Y + TITLE_PAD)     // 402
#define OUT_CTRL_Y      (OUT_BOX_Y + BOX_PAD)         // 405
#define OUT_BOX_H       130
#define OUT_BOX_END     (OUT_BOX_Y + OUT_BOX_H)       // 532

// Buttons
#define BTN_Y           (OUT_BOX_END + 8)             // 540
#define BTN_H           24


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

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        // ── Column 1: Identity & Tags ─────────────────────────────────────
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

        // ── Column 1: Clothing ────────────────────────────────────────────
        y = CL_CTRL_Y;
        Label(hwnd, L"Shirt ID:", x, y, LABEL_W, 18);
        hShirtID = Edit(hwnd, ID_SHIRT_ID, x + INPUT_X, y, 170, 22, L"Roblox asset ID");

        y += ROW_H;
        Label(hwnd, L"Pants ID:", x, y, LABEL_W, 18);
        hPantsID = Edit(hwnd, ID_PANTS_ID, x + INPUT_X, y, 170, 22, L"Roblox asset ID");

        // ── Column 1: Hats & Accessories ──────────────────────────────────
        y = HAT_CTRL_Y;
        Label(hwnd, L"Hat/Accessory ID or name:", x, y, 210, 18);
        y += 20;
        hHatInput = Edit(hwnd, ID_HAT_INPUT, x, y, 210, 22, L"e.g. 107189833 or holster");
        Btn(hwnd, ID_HAT_ADD, L"Add", x + 220, y, 60, 22);
        y += 28;
        hHatList = ListBox(hwnd, ID_HAT_LIST, x, y, 210, 100);
        Btn(hwnd, ID_HAT_REMOVE, L"Remove", x + 220, y, 60, 22);

        // ── Column 2: Guns ────────────────────────────────────────────────
        int x2 = COL2_CX;
        y = ID_CTRL_Y;
        Label(hwnd, L"Gun name or ID:", x2, y, 150, 18);
        y += 20;
        hGunInput = Edit(hwnd, ID_GUN_INPUT, x2, y, 150, 22, L"e.g. pistol or m4");
        Btn(hwnd, ID_GUN_ADD, L"Add", x2 + 160, y, 60, 22);
        y += 28;
        hGunList = ListBox(hwnd, ID_GUN_LIST, x2, y, 150, 290);
        Btn(hwnd, ID_GUN_REMOVE, L"Remove", x2 + 160, y, 60, 22);

        // ── Column 3: Gear ────────────────────────────────────────────────
        int x3 = COL3_CX;
        y = ID_CTRL_Y;
        Label(hwnd, L"Gear/Equipment name:", x3, y, 150, 18);
        y += 20;
        hGearInput = Edit(hwnd, ID_GEAR_INPUT, x3, y, 150, 22, L"e.g. radio or medkit");
        Btn(hwnd, ID_GEAR_ADD, L"Add", x3 + 160, y, 60, 22);
        y += 28;
        hGearList = ListBox(hwnd, ID_GEAR_LIST, x3, y, 150, 290);
        Btn(hwnd, ID_GEAR_REMOVE, L"Remove", x3 + 160, y, 60, 22);

        // ── Column 4: Options & Stats ─────────────────────────────────────
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

        // ── Output area ───────────────────────────────────────────────────
        y = OUT_CTRL_Y;
        Label(hwnd, L"Generated Command (paste into Roblox chat):", COL1_CX, y, 500, 18);
        y += 20;
        hOutput = Edit(hwnd, ID_OUTPUT, COL1_CX, y, WIN_W - 24, 110, L"", true);
        SendMessage(hOutput, EM_SETREADONLY, TRUE, 0);

        // ── Buttons ───────────────────────────────────────────────────────
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
            double dmg = v / 100.0;
            swprintf_s(buf, 16, L"%.2fx", dmg);
            SetText(hLblDamage, buf);
        }
        break;
    }


    case WM_DRAWITEM:
    {
        // Not used — color preview now handled in WM_CTLCOLORSTATIC
        break;
    }


    case WM_COMMAND:
    {
        int id = LOWORD(wp);
        int evt = HIWORD(wp);

        // Color preview updates on focus loss (debounced, prevents flicker during typing)
        if ((id == ID_TAG_R || id == ID_TAG_G || id == ID_TAG_B) && evt == EN_KILLFOCUS) {
            if (hColorBrush) DeleteObject(hColorBrush);
            hColorBrush = CreateSolidBrush(RGB(
                ClampRGB(GetText(hTagR)),
                ClampRGB(GetText(hTagG)),
                ClampRGB(GetText(hTagB))));
            InvalidateRect(hColorPreview, nullptr, TRUE);
            UpdateWindow(hColorPreview);
        }

        if (id == ID_HAT_ADD) {
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
        // Enter key in input boxes → trigger Add
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
                        if (SetClipboardData(CF_UNICODETEXT, hMem)) {
                            MessageBoxW(hwnd, L"Command copied to clipboard!", L"Success", MB_OK | MB_ICONINFORMATION);
                        }
                        else {
                            GlobalFree(hMem);
                        }
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
        }
        break;
    }


    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        SetBkMode(hdc, TRANSPARENT);

        // Group boxes aligned with control sections
        DrawBox(hdc, COL1_GX, ID_TITLE_Y,  COL1_W, ID_BOX_H,  L" Identity & Tags ");
        DrawBox(hdc, COL1_GX, CL_TITLE_Y,  COL1_W, CL_BOX_H,  L" Clothing ");
        DrawBox(hdc, COL1_GX, HAT_TITLE_Y, COL1_W, HAT_BOX_H, L" Hats & Accessories ");
        DrawBox(hdc, COL2_GX, ID_TITLE_Y,  COL2_W, RIGHT_BOX_H, L" Guns ");
        DrawBox(hdc, COL3_GX, ID_TITLE_Y,  COL3_W, RIGHT_BOX_H, L" Gear ");
        DrawBox(hdc, COL4_GX, ID_TITLE_Y,  COL4_W, RIGHT_BOX_H, L" Options & Stats ");
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
        // Color preview gets dynamic color brush
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
    // Enable DPI awareness for proper scaling at 125%, 150%, 200% etc.
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