/*
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>
*/

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <tlhelp32.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
// #define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION
#define NK_GDI_IMPLEMENTATION
#include <nuklear.h>
#include <nuklear_gdi.h>

// http://stackoverflow.com/questions/865152/how-can-i-get-a-process-handle-by-its-name-in-c

HANDLE GetProcessByName(TCHAR* name)
{
    DWORD pid = 0;

    // Create toolhelp snapshot.
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 process;
    ZeroMemory(&process, sizeof(process));
    process.dwSize = sizeof(process);

    // Walkthrough all processes.
    if (Process32First(snapshot, &process))
    {
        do
        {
            // Compare process.szExeFile based on format of name, i.e., trim file path
            // trim .exe if necessary, etc.
            if (stricmp(process.szExeFile, name) == 0)
            {
                pid = process.th32ProcessID;
                break;
            }
        } while(Process32Next(snapshot, &process));
    }

    CloseHandle(snapshot);

    if (pid != 0)
    {
        return OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    }

    // Not found


    return NULL;
}

#define NUM_CHEATS 92
#define PROCESS_NAME "gta_sa.exe"

HWND hwnd;
unsigned char cheats[NUM_CHEATS];
volatile unsigned char cur_cheats[NUM_CHEATS];
volatile char ready = 0;

char* cheat_codes[NUM_CHEATS] = {
    "ThugsArmoury",
    "ProfessionalsKit",
    "NuttersToys",
    "INeedSomeHelp",
    "TurnUpTheHeat",
    "TurnDownTheHeat",
    "PleasantlyWarm",
    "TooDamnHot",
    "DullDullDay",
    "StayInAndWatchTV",
    "CantSeeWhereImGoing",
    "TimeJustFliesBy",
    "SpeedItUp",
    "SlowItDown",
    "RoughNeighbourhood",
    "StopPickingOnMe",
    "SurroundedByNutters",
    "TimeToKickAss",
    "OldSpeedDemon",
    "JQNTDMH",
    "NotForPublicRoads",
    "JustTryAndStopMe",
    "WheresTheFuneral",
    "CelebrityStatus",
    "TrueGrime",
    "18Holes",
    "AllCarsGoBoom",
    "WheelsOnlyPlease",
    "StickLikeGlue",
    "GoodbyeCruelWorld",
    "DontTryAndStopMe",
    "AllDriversAreCriminals",
    "PinkIsTheNewCool",
    "SoLongAsItsBlack",
    NULL,
    "FlyingFish",
    "WhoAteAllThePies",
    "BuffMeUp",
    "LeanAndMean",
    "BlueSuedeShoes",
    "AttackOfTheVillagePeople",
    "LifesABeach",
    "OnlyHomiesAllowed",
    "BetterStayIndoors",
    "NinjaTown",
    "LoveConquersAll",
    "EveryoneIsPoor",
    "EveryoneIsRich",
    "ChittyChittyBangBang",
    "CJPhoneHome",
    "JumpJet",
    "IWantToHover",
    "TouchMyCarYouDie",
    "SpeedFreak",
    "BubbleCars",
    "NightProwler",
    "DontBringOnTheNight",
    "ScottishSummer",
    "SandInMyEars",
    NULL,
    "Kangaroo",
    "NoOneCanHurtMe",
    "ManFromAtlantis",
    "LetsGoBaseJumping",
    "Rocketman",
    "IDoAsIPlease",
    "BringItOn",
    "StingLikeABee",
    "IAmNeverHungry",
    "StateOfEmergency",
    "CrazyTown",
    "TakeAChillPill",
    "FullClip",
    "IWannaDriveBy",
    "GhostTown",
    "HicksVille",
    "WannaBeInMyGang",
    "NoOneCanStopUs",
    "RocketMayhem",
    "WorshipMe",
    "HelloLadies",
    "ICanGoAllNight",
    "ProfessionalKiller",
    "NaturalTalent",
    "OhDude",
    "FourWheelFun",
    "HitTheRoadJack",
    "ItsAllBull",
    "FlyingToStunt",
    "MonsterMash",
    NULL,
    NULL
};

char* cheat_desc[NUM_CHEATS] = {
    "Weapon Set 1",
    "Weapon Set 2",
    "Weapon Set 3",
    "Health, Armor, $250k",
    "Wanted level 2 stars",
    "Clear wanted level",
    "Sunny weather",
    "Very sunny weather",
    "Overcast weather",
    "Rainy weather",
    "Foggy weather",
    "Faster clock",
    "Faster gameplay",
    "Slower gameplay",
    "Peds attack each other",
    "Have a bounty on your head",
    "Everyone armed",
    "Spawn Rhino",
    "Spawn Bloodring Banger",
    "Spawn Rancher",
    "Spawn Hotring A",
    "Spawn Hotring B",
    "Spawn Romero",
    "Spawn Stretch",
    "Spawn Trashmaster",
    "Spawn Caddy",
    "Blow up all cars",
    "Invisible cars",
    "Insane handling",
    "Suicide",
    "All green lights",
    "Aggressive drivers",
    "Pink traffic",
    "Black traffic",
    "Cars on water",
    "Boats fly",
    "Fat player",
    "Max muscle",
    "Skinny player",
    "Elvis is everywhere",
    "Peds attack you with rockets",
    "Beach party",
    "Gang members everywhere",
    "Gangs controls the streets",
    "Ninja theme",
    "Slut magnet",
    "Cheap cars",
    "Expensive cars",
    "Cars fly",
    "Huge bunny hop",
    "Spawn Hydra",
    "Spawn Vortex",
    "Smash n' boom",
    "All cars have nitro",
    "Cars float away when hit",
    "Always midnight",
    "Stop game clock orange sky",
    "Thunder storm",
    "Sand storm",
    "(Unknown)",
    "Mega jump",
    "Infinite health",
    "Infinite oxygen",
    "Get parachute",
    "Get jetpack",
    "Never wanted",
    "Six wanted stars",
    "Mega punch",
    "Never get hungry",
    "Riot mode",
    "Funhouse theme",
    "Adrenaline mode",
    "Infinite ammo",
    "Weapon aiming while driving",
    "Reduced traffic",
    "Country traffic",
    "Recruit anyone (9mm)",
    "Recruit anyone (AK-47)",
    "Recruit anyone (rockets)",
    "Lock respect at max",
    "Lock sex appeal at max",
    "Max stamina",
    "Hitman level for all weapons",
    "Max driving skills",
    "Spawn Hunter",
    "Spawn Quad",
    "Spawn Tanker Truck",
    "Spawn Dozer",
    "Spawn Stunt Plane",
    "Spawn Monster",
    "Prostitutes pay you",
    "All taxis have nitrous",
};

DWORD crc32t[256] = {0};

void gen_crc32t() {
    if (crc32t[1] != 0) return;
    DWORD poly = 0xEDB88320;
    for(DWORD n = 0; n < 256; n++){
        DWORD c = n;
        for(unsigned char k = 0; k < 8; k++) {
            c = ((c & 1) ? (poly ^ (c >> 1)) : (c >> 1));
        }
        crc32t[n] = c;
    }
}

DWORD cheat_crc[NUM_CHEATS] = {0};

void gen_cheat_crc() {
    if (cheat_crc[0] != 0) return;
    gen_crc32t();
    for(int i = 0 ; i < NUM_CHEATS; i++) {
        if (cheat_codes[i] != NULL) {
            char* code = cheat_codes[i];
            char* code_s = code;
            DWORD crc = 0xFFFFFFFF;
            // CRC32 values for cheat codes are calculated backwards
            while(*code != 0) code++;
            while(--code >= code_s)
            {
                char c = toupper(*code);
                crc = (crc >> 8) ^ crc32t[(crc ^ (unsigned char)c) & 0xFF];
            }
            cheat_crc[i] = crc;
        }
    }
}

DWORD get_cheat_crc(unsigned char i) {
    gen_cheat_crc();
    return cheat_crc[i];
}

unsigned char typed[30];

struct typed_code { char str[31]; };
struct typed_code typed_codes[NUM_CHEATS];

void search_typed_for_cheat(unsigned char cheat_id) {
    struct typed_code code;
    DWORD target_crc = get_cheat_crc(cheat_id);

    // start_i points to the first cheat character in typed buffer
    // which is actually the last character of the cheat code
    // as the buffer is reversed.
    // The character is always 0 - it is zeroed to prevent chaining
    // of cheat codes.
    unsigned char start_i;
    for(start_i = 0; start_i < sizeof(typed); start_i++)
        if (typed[start_i] == 0) goto cont;
    // cheat code not found
    goto not_found;

    // end_i points beyond the end of the buffer or to the previous
    // cheat code, whatever comes first
    unsigned char end_i;
cont:
    for(end_i = start_i+1; end_i < sizeof(typed); end_i++)
        if (typed[end_i] == 0) break;

    // cheat_len is the current length of the cheat code we're trying to find
    for(unsigned char cheat_len = 6; cheat_len <= end_i - start_i; cheat_len++)
    {
        // search what character was overwritten by 0
        // we expect it to be an upper case letter or a digit
        // brute forcing seems to be fast enough
        unsigned char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        for(unsigned char char_i = 0; char_i < sizeof(chars); char_i++)
        {
            DWORD crc = 0xFFFFFFFF;
            crc = (crc >> 8) ^ crc32t[(crc ^ chars[char_i]) & 0xFF];

            for(unsigned char typed_i = start_i+1; typed_i < start_i + cheat_len; typed_i++)
            {
                crc = (crc >> 8) ^ crc32t[(crc ^ typed[typed_i]) & 0xFF];
            }

            if (crc != target_crc) continue;
            code.str[cheat_len] = 0;
            code.str[cheat_len-1] = tolower(chars[char_i]);
            for(unsigned char typed_i = start_i+1; typed_i < start_i + cheat_len; typed_i++)
                code.str[start_i + cheat_len - 1 - typed_i] = tolower(typed[typed_i]);
            typed_codes[cheat_id] = code;
            return;
        }
    }
not_found:
    // cheat not found for some reason
    if (strcmp(typed_codes[cheat_id].str, "") == 0)
        sprintf(typed_codes[cheat_id].str, "?");
}

DWORD WINAPI dc_thread(LPVOID lpParameter) {
    (void)lpParameter;
    HANDLE proc;
    do {
        Sleep(250);
        while(
            (proc = GetProcessByName(PROCESS_NAME)) &&
            ReadProcessMemory(proc, (LPCVOID)0x969130, &cheats, sizeof(cheats), NULL) &&
            ReadProcessMemory(proc, (LPCVOID)0x969110, &typed, sizeof(typed), NULL)
        ) {
            char changed_cheats = 0;
            for(unsigned char i = 0; i < NUM_CHEATS; i++) {
                if (cur_cheats[i] != cheats[i]) {
                    search_typed_for_cheat(i);
                    cur_cheats[i] = cheats[i];
                    changed_cheats++;
                }
            }
            if (ready == 0 || changed_cheats > 0) {
                ready = 1;
                InvalidateRect(hwnd, NULL, TRUE);
            }
            CloseHandle(proc);
            Sleep(33);
        }
        if (ready == 1) {
            ready = 0;
            InvalidateRect(hwnd, NULL, TRUE);
        }
        if (proc) CloseHandle(proc);
    } while(1);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    if (nk_gdi_handle_event(hwnd, message, wParam, lParam))
        return 0;

    return DefWindowProc (hwnd, message, wParam, lParam);
}

int draw_label_box_offset(struct nk_context *ctx, const char* str, nk_flags align, struct nk_color color, float offset_x, float offset_y)
{
    struct nk_command_buffer *canvas = nk_window_get_canvas(ctx);

    struct nk_rect space;
    enum nk_widget_layout_states state = nk_widget(&space, ctx);
    if (!state) return 0;
    struct nk_input *input = &ctx->input;

    space.x += round(offset_x);
    space.y += round(offset_y);
    nk_fill_rect(canvas, space, 0, color);
    int len = nk_strlen(str);
    struct nk_text text;
    text.padding.x = ctx->style.text.padding.x;
    text.padding.y = ctx->style.text.padding.y;
    text.background = color;
    text.text = ctx->style.text.color;
    nk_widget_text(canvas, space, str, len, &text, align, ctx->style.font);

    if (state != NK_WIDGET_ROM)
        if (nk_input_any_mouse_click_in_rect(input, space)) return 1;
    return 0;
}

int draw_label_box(struct nk_context *ctx, const char* str, nk_flags align, struct nk_color color)
{
    return draw_label_box_offset(ctx, str, align, color, 0, 0);
}

// a box with cheat description
// automatically switches between monitored cheats

void draw_description_box(struct nk_context *ctx, unsigned char* cheats_used, int cheats_used_cnt, double* x_center, HWND hwnd, int line_height)
{
    if (cheats_used_cnt == 0) return;
    static const DWORD update_period = 5000;
    static DWORD last_update = 0;
    static int cheat_num = -1;
    static double tr_pos;
    static double tr_pos_v = 0;
    static char tr_pos_init = 0;
    DWORD tick = GetTickCount();

    // select next cheat after predefined time, or if the current
    // cheat was disabled and then deleted by clicking on its number
    if (tick - last_update >= update_period || !cheats_used[cheat_num]) {
        do {
            cheat_num++;
            cheat_num %= NUM_CHEATS;
        } while(!cheats_used[cheat_num]);
        last_update = tick;
        SetTimer(hwnd, 1, update_period, NULL);
    }

    struct nk_command_buffer *canvas = nk_window_get_canvas(ctx);

    struct nk_rect space;
    enum nk_widget_layout_states state = nk_widget(&space, ctx);
    if (!state) return;

    if (!tr_pos_init)
    {
        tr_pos = space.x + space.w / 2;
        tr_pos_init = 1;
    }
    // move pointer to the middle of the box that represent
    // currently selected cheat using simple physics
    double tr_pos_force = (x_center[cheat_num] - tr_pos) / 16;
    tr_pos_force -= tr_pos_v / 2;
    tr_pos_v += tr_pos_force;
    tr_pos += tr_pos_v;
    KillTimer(hwnd, 2);
    // animate until it reaches the center
    if (round(tr_pos) != x_center[cheat_num]) SetTimer(hwnd, 2, 33, NULL);

    // draw pointer
    struct nk_color bg = nk_rgb(64, 64, 64);
    nk_fill_triangle(canvas, 
        tr_pos, space.y,
        tr_pos + 10, space.y + 16,
        tr_pos - 10, space.y + 16,
        bg);
    space.h -= 16; space.y += 16;

    // draw description box
    nk_fill_rect(canvas, space, 0, bg);

    // draw text
    char str[38];
    struct nk_text text;
    nk_flags align = NK_TEXT_ALIGN_LEFT;

    text.padding.x = ctx->style.text.padding.x;
    text.padding.y = ctx->style.text.padding.y;
    text.background = bg;
    text.text = ctx->style.text.color;
    snprintf(str, sizeof(str), "Typed: %s", typed_codes[cheat_num].str);
    nk_widget_text(canvas, space, str, strlen(str), &text, align, ctx->style.font);
    snprintf(str, sizeof(str), "Full: %s", cheat_codes[cheat_num]);
    space.y += line_height;
    nk_widget_text(canvas, space, str, strlen(str), &text, align, ctx->style.font);
    snprintf(str, sizeof(str), "Effect:");
    space.y += line_height;
    nk_widget_text(canvas, space, str, strlen(str), &text, align, ctx->style.font);
    snprintf(str, sizeof(str), "%s", cheat_desc[cheat_num]);
    space.y += line_height;
    nk_widget_text(canvas, space, str, strlen(str), &text, align, ctx->style.font);
}

NK_API void
nk_gdi_push_font(struct nk_context *ctx, GdiFont *gdifont)
{
    struct nk_user_font *font = &gdifont->nk;
    font->userdata = nk_handle_ptr(gdifont);
    font->height = (float)gdifont->height;
    font->width = nk_gdifont_get_text_width;
    nk_style_push_font(ctx, font);
}

#define WINDOW_WIDTH 300
#define WINDOW_HEIGHT 358

int WINAPI WinMain(HINSTANCE hIns, HINSTANCE hPrev, LPSTR lpszArgument, int nCmdShow)
{
    (void)hPrev;
    (void)lpszArgument;
    char szClassName[]="GTA SA Cheat Monitor";
    WNDCLASSEX wc;
    MSG messages;

    wc.hInstance=hIns;
    wc.lpszClassName=szClassName;
    wc.lpfnWndProc = WndProc;
    wc.style = CS_DBLCLKS;
    wc.cbSize = sizeof (WNDCLASSEX);
    wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor (NULL, IDC_ARROW);
    wc.lpszMenuName = NULL;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    RegisterClassEx(&wc);
    hwnd=CreateWindow(szClassName,szClassName,WS_OVERLAPPEDWINDOW,0,0,WINDOW_WIDTH,WINDOW_HEIGHT,HWND_DESKTOP,NULL,hIns,NULL);
    ShowWindow(hwnd, nCmdShow);
    HANDLE thread = CreateThread(0, 0, dc_thread, NULL, 0, NULL);

    /* GUI */
    HDC dc = GetDC(hwnd);
    GdiFont* font = nk_gdifont_create("Arial", 30);
    GdiFont* font_names = nk_gdifont_create("Arial", 22);
    struct nk_context* ctx = nk_gdi_init(font, dc, WINDOW_WIDTH, WINDOW_HEIGHT);
    nk_style_push_style_item(ctx, &ctx->style.window.fixed_background, nk_style_item_color(nk_rgb(0, 0, 0)));
    nk_style_push_color(ctx, &ctx->style.window.background, nk_rgb(0, 0, 0));
    nk_style_push_color(ctx, &ctx->style.text.color, nk_rgb(255, 255, 255));

    int running = 1;
    int needs_refresh = 1;
    unsigned char cheats_used[NUM_CHEATS] = {0};
    unsigned char prev_cheats[NUM_CHEATS] = {0};
    DWORD change_t[NUM_CHEATS] = {0};
    unsigned char animating[NUM_CHEATS] = {0};
    struct anim_state { float x, dx, y, dy; int line; };
    struct anim_state anim_states[NUM_CHEATS];

    while (running)
    {
        /* Input */
        MSG msg;
        nk_input_begin(ctx);
        if (needs_refresh == 0) {
            if (GetMessageW(&msg, NULL, 0, 0) <= 0)
                running = 0;
            else {
                TranslateMessage(&msg);
                DispatchMessageW(&msg);
            }
            needs_refresh = 1;
        } else needs_refresh = 0;

        while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                running = 0;
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
            needs_refresh = 1;
        }
        nk_input_end(ctx);

        struct nk_rect bounds = nk_rect(0, 0, 0, 0);
        RECT clientRect;
        if (GetClientRect(hwnd, &clientRect))
            bounds = nk_rect(0, 0, clientRect.right, clientRect.bottom);

        /* GUI */
        if (nk_begin(ctx, "Cheats", bounds, NK_WINDOW_NO_SCROLLBAR))
        {
            // Scale GUI with the window
            nk_window_set_bounds(ctx, bounds);
            if (ready) {
                int cheats_used_cnt = 0;
                for (int i = 0; i < NUM_CHEATS; i++) {
                    if (cur_cheats[i] && cheat_codes[i]) cheats_used[i] = 1;
                    if (cheats_used[i]) cheats_used_cnt++;
                }
                if (cheats_used_cnt == 0) {
                    nk_layout_row_dynamic(ctx, 30, 1);
                    nk_label(ctx, "No cheats used", NK_TEXT_CENTERED);
                } else {
                    nk_layout_row_dynamic(ctx, 30, 1);
                    draw_label_box(ctx, "Cheats used:", NK_TEXT_CENTERED, nk_rgb(64, 64, 64));
                    // Draw number boxes
                    nk_layout_row_dynamic(ctx, 30, cheats_used_cnt);
                    double x_center[NUM_CHEATS];
                    for (int i = 0; i < NUM_CHEATS; i++) {
                        char cheat_num_str[4];
                        snprintf(cheat_num_str, sizeof(cheat_num_str), "#%d", i);
                        if (cheats_used[i]) {
                            struct nk_rect r = nk_widget_bounds(ctx);
                            x_center[i] = r.x + r.w / 2;
                            if (cur_cheats[i]) {
                                draw_label_box(ctx, cheat_num_str, NK_TEXT_CENTERED, nk_rgb(0, 64, 0));
                            } else {
                                nk_style_push_color(ctx, &ctx->style.text.color, nk_rgb(128, 128, 128));
                                if (draw_label_box(ctx, cheat_num_str, NK_TEXT_CENTERED, nk_rgb(64, 0, 0)))
                                {
                                    cheats_used[i] = 0;
                                    cheats_used_cnt--;
                                }
                                nk_style_pop_color(ctx);
                            }
                        }
                    }
                    // Draw description box
                    nk_gdi_push_font(ctx, font_names);
                    nk_layout_row_dynamic(ctx, 116, 1);
                    draw_description_box(ctx, cheats_used, cheats_used_cnt, x_center, hwnd, 24);
                    // Draw cheat on/off animation
                    int line = 0;
                    KillTimer(hwnd, 0);
                    for (int i = 0; i < NUM_CHEATS; i++) {
                    // If cheat state changes and its not yet animated then
                    // start the animation, otherwise just reset animation timer
                      if (cheats_used[i] && cur_cheats[i] != prev_cheats[i]) {
                          prev_cheats[i] = cur_cheats[i];
                          change_t[i] = GetTickCount();
                          if (animating[i] == 0) {
                              animating[i] = 1;
                              // start off-screen on the left
                              struct anim_state state = { -bounds.w, 10, 0, 0, line };
                              anim_states[i] = state;
                          }
                      }
                      if (animating[i]) {
                          // Draw line
                          char cheat_code[30];
                          struct anim_state state = anim_states[i];
                          snprintf(cheat_code, sizeof(cheat_code), "%s%s(%d)",
                              cur_cheats[i] ? "+" : "-", cheat_codes[i], i);
                          nk_layout_row_dynamic(ctx, 24, 1);
                          if (cur_cheats[i]) {
                              draw_label_box_offset(ctx, cheat_code, NK_TEXT_LEFT, nk_rgb(0, 64, 0), state.x, state.y);
                          } else {
                              nk_style_push_color(ctx, &ctx->style.text.color, nk_rgb(128, 128, 128));
                              draw_label_box_offset(ctx, cheat_code, NK_TEXT_LEFT, nk_rgb(64, 0, 0), state.x, state.y);
                              nk_style_pop_color(ctx);
                          }
                          // Simple physics based movement
                          float fx;
                          if (GetTickCount() - change_t[i] < 6000) {
                              // Slide in for 6s
                              fx = -state.x / 10;
                          } else {
                              // Then slide out to the right
                              fx = (bounds.w - state.x) / 10;
                              // If out of screen stop animating
                              if (state.x >= bounds.w) animating[i] = 0;
                          }
                          // Some friction to dampen oscillations
                          fx -= state.dx / 2;
                          // Update speed and position
                          state.dx += fx;
                          state.x += state.dx;

                          // If position changed due to line being
                          // added or removed, reposition to match
                          // previous position and do a smooth transition
                          if (state.line != line) {
                              state.y = (state.line - line) * 28;
                              state.line = line;
                          }

                          // Gravitate towards 0 offset from current line
                          float fy;
                          fy = -state.y / 10;
                          fy -= state.dy / 2;
                          state.dy += fy;
                          state.y += state.dy;

                          anim_states[i] = state;
                          SetTimer(hwnd, 0, 33, NULL);

                          line++;
                      }
                    }
                    nk_style_pop_font(ctx);
                }
            } else {
                nk_layout_row_dynamic(ctx, 30, 1);
                char* lines[] = { PROCESS_NAME, "process", "not found", NULL };
                for (int i = 0; lines[i]; i++)
                    nk_label(ctx, lines[i], NK_TEXT_CENTERED);
            }
        }
        nk_end(ctx);

        /* Draw */
        nk_gdi_render(nk_rgb(0,0,0));
    }

    nk_gdifont_del(font);
    ReleaseDC(hwnd, dc);

    CloseHandle(thread);
    return messages.wParam;
}

