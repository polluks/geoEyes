#include <geos.h>

#define LEFT_EX     75
#define LEFT_EY     90
#define RIGHT_EX    225
#define RIGHT_EY    90
#define EYE_RY      30
#define PUPIL_MAX   12

static const char pupil[63] = {
#include "pupil_data.c"
};

static const char eye_span[] = {
    0, 6, 9, 11, 12, 14, 15, 16, 17, 18, 19, 19, 20, 21, 21, 22,
    22, 23, 23, 23, 24, 24, 24, 24, 24, 25, 25, 25, 25, 25, 25,
};

static unsigned isqrt(unsigned long n)
{
    unsigned long r = 0;
    while (r * r < n) ++r;
    if (r * r > n) --r;
    return (unsigned)r;
}

static void draw_eye(unsigned cx, char cy)
{
    signed char dy;
    for (dy = -(char)EYE_RY; dy <= (char)EYE_RY; ++dy) {
        unsigned idx = EYE_RY - (dy < 0 ? (unsigned)-dy : (unsigned)dy);
        unsigned hw = eye_span[idx];
        if (hw > 0) {
            HorizontalLine(1, cy + dy, cx - hw, cx + hw);
        }
    }
}

static void get_pupil_pos(int mx, int my, int ex, int ey,
                           int *px, int *py)
{
    int dx = mx - ex;
    int dy = my - ey;
    unsigned adx = dx < 0 ? -(unsigned)dx : (unsigned)dx;
    unsigned ady = dy < 0 ? -(unsigned)dy : (unsigned)dy;
    int dist = isqrt((unsigned long)adx * adx + (unsigned long)ady * ady);
    if (dist > PUPIL_MAX && dist > 0) {
        dx = dx * PUPIL_MAX / dist;
        dy = dy * PUPIL_MAX / dist;
    }
    *px = ex + dx;
    *py = ey + dy;
}

int main(void)
{
    struct pixel lpos, rpos;

    FirstInit();
    InitForIO();

    ClearRam(SCREEN_BASE, SC_SIZE);

    DrawSprite(2, pupil);
    DrawSprite(3, pupil);
    DrawSprite(0, pupil);
    DrawSprite(1, pupil);

    mob2clr = 1;
    mob3clr = 1;
    mob0clr = 0;
    mob1clr = 0;

    mobx2 = 0x0c;
    moby2 = 0x0c;

    draw_eye(LEFT_EX, LEFT_EY);
    draw_eye(RIGHT_EX, RIGHT_EY);

    lpos.x = LEFT_EX - 24;
    lpos.y = LEFT_EY - 21;
    PosSprite(2, &lpos);

    lpos.x = RIGHT_EX - 24;
    lpos.y = RIGHT_EY - 21;
    PosSprite(3, &lpos);

    EnablSprite(0);
    EnablSprite(1);
    EnablSprite(2);
    EnablSprite(3);

    StartMouseMode();

    while (!(pressFlag & SET_KEYPRESS)) {
        int mx = mousePos.x;
        int my = mousePos.y;
        int px, py;

        get_pupil_pos(mx, my, LEFT_EX, LEFT_EY, &px, &py);
        lpos.x = (unsigned)(px > 12 ? px - 12 : 0);
        lpos.y = (char)(py > 10 ? py - 10 : 0);
        PosSprite(0, &lpos);

        get_pupil_pos(mx, my, RIGHT_EX, RIGHT_EY, &px, &py);
        rpos.x = (unsigned)(px > 12 ? px - 12 : 0);
        rpos.y = (char)(py > 10 ? py - 10 : 0);
        PosSprite(1, &rpos);

        Sleep(1);
    }

    DisablSprite(0);
    DisablSprite(1);
    DisablSprite(2);
    DisablSprite(3);
    ClearMouseMode();

    EnterDeskTop();
    return 0;
}
