#include <geos.h>

#define LEFT_EX     75
#define LEFT_EY     90
#define RIGHT_EX    225
#define RIGHT_EY    90
#define EYE_RX      25
#define EYE_RY      30
#define PUPIL_MAX   12

/* #define SPR_EXP_Y */  /* undef = full res 21 rows, def = half res via Y-expand */

#ifndef SPR_EXP_Y
static const char pupil_data[63] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x55, 0x00, 0x05, 0x55, 0x40, 0x15, 0xfd, 0x50,
    0x57, 0xff, 0x54, 0x5f, 0xff, 0xd4, 0x5f, 0xff, 0xd4, 0x5f, 0xff, 0xd4,
    0x57, 0xff, 0x54, 0x15, 0xfd, 0x50, 0x05, 0x55, 0x40, 0x01, 0x55, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00,
};
#else
static const char pupil_comp[33] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x55, 0x40,
    0x57, 0xff, 0x54, 0x5f, 0xff, 0xd4, 0x57, 0xff, 0x54, 0x05, 0x55, 0x40,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static char pupil_buf[64];
#endif /* SPR_EXP_Y */

static const char eye_span[] = {
    0, 6, 9, 11, 12, 14, 15, 16, 17, 18, 19, 19, 20, 21, 21, 22,
    22, 23, 23, 23, 24, 24, 24, 24, 24, 25, 25, 25, 25, 25, 25,
};

static unsigned isqrt(unsigned n)
{
    unsigned r = 0;
    while (r * r < n) ++r;
    if (r * r > n) --r;
    return r;
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
    int dist = isqrt((unsigned)(dx*dx + dy*dy));
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

#ifdef SPR_EXP_Y
    {
        unsigned i;
        for (i = 0; i < 11; ++i) {
            pupil_buf[i * 6 + 0] = pupil_comp[i * 3 + 0];
            pupil_buf[i * 6 + 1] = pupil_comp[i * 3 + 1];
            pupil_buf[i * 6 + 2] = pupil_comp[i * 3 + 2];
            pupil_buf[i * 6 + 3] = pupil_comp[i * 3 + 0];
            pupil_buf[i * 6 + 4] = pupil_comp[i * 3 + 1];
            pupil_buf[i * 6 + 5] = pupil_comp[i * 3 + 2];
        }
    }
    DrawSprite(1, pupil_buf);
    DrawSprite(2, pupil_buf);
    moby2 |= 0x06;
#else
    DrawSprite(1, pupil_data);
    DrawSprite(2, pupil_data);
#endif

    mcmclr0 = 1;
    mcmclr1 = 11;
    mob1clr = 0;
    mob2clr = 0;
    mobmcm |= 0x06;

    draw_eye(LEFT_EX, LEFT_EY);
    draw_eye(RIGHT_EX, RIGHT_EY);

    EnablSprite(1);
    EnablSprite(2);

    StartMouseMode();

    while (!(pressFlag & SET_KEYPRESS)) {
        int mx = mousePos.x;
        int my = mousePos.y;
        int px, py;

        get_pupil_pos(mx, my, LEFT_EX, LEFT_EY, &px, &py);
        lpos.x = (unsigned)(px > 12 ? px - 12 : 0);
        lpos.y = (char)(py > 10 ? py - 10 : 0);
        PosSprite(1, &lpos);

        get_pupil_pos(mx, my, RIGHT_EX, RIGHT_EY, &px, &py);
        rpos.x = (unsigned)(px > 12 ? px - 12 : 0);
        rpos.y = (char)(py > 10 ? py - 10 : 0);
        PosSprite(2, &rpos);

        Sleep(1);
    }

    DisablSprite(1);
    DisablSprite(2);
    ClearMouseMode();

    EnterDeskTop();
    return 0;
}
