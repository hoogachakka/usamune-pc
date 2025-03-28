#include <PR/ultratypes.h>
#include <PR/gbi.h>

#include "config.h"
#include "macros.h"
#include "types.h"
#include "game/usamune.h"

#include "make_const_nonconst.h"

// Usamune Practice Rom (US) Textures

ALIGNED8 static const Texture texture_usamune_sprite_arrow_right[] = {
#include "textures/usamune/tex.00000.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_sprite_arrow_up[] = {
#include "textures/usamune/tex.00200.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_sprite_key[] = {
#include "textures/usamune/tex.00400.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_sprite_joystick[] = {
#include "textures/usamune/tex.00600.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_sprite_star[] = {
#include "textures/usamune/tex.00800.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_sprite_mario[] = {
#include "textures/usamune/tex.00A00.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_sprite_coin[] = {
#include "textures/usamune/tex.00C00.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_sprite_times[] = {
#include "textures/usamune/tex.00E00.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_percent[] = {
#include "textures/usamune/tex.01000.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_ampersand[] = {
#include "textures/usamune/tex.01200.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_question_mark[] = {
#include "textures/usamune/tex.01400.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_double_exclamation[] = {
#include "textures/usamune/tex.01600.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_exclamation[] = {
#include "textures/usamune/tex.01800.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_double_quote[] = {
#include "textures/usamune/tex.01A00.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_apostrophe[] = {
#include "textures/usamune/tex.01C00.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_Z[] = {
#include "textures/usamune/tex.01E00.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_Y[] = {
#include "textures/usamune/tex.02000.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_X[] = {
#include "textures/usamune/tex.02200.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_W[] = {
#include "textures/usamune/tex.02400.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_V[] = {
#include "textures/usamune/tex.02600.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_U[] = {
#include "textures/usamune/tex.02800.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_T[] = {
#include "textures/usamune/tex.02A00.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_S[] = {
#include "textures/usamune/tex.02C00.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_R[] = {
#include "textures/usamune/tex.02E00.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_Q[] = {
#include "textures/usamune/tex.03000.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_P[] = {
#include "textures/usamune/tex.03200.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_O[] = {
#include "textures/usamune/tex.03400.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_N[] = {
#include "textures/usamune/tex.03600.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_M[] = {
#include "textures/usamune/tex.03800.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_L[] = {
#include "textures/usamune/tex.03A00.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_K[] = {
#include "textures/usamune/tex.03C00.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_J[] = {
#include "textures/usamune/tex.03E00.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_I[] = {
#include "textures/usamune/tex.04000.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_H[] = {
#include "textures/usamune/tex.04200.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_G[] = {
#include "textures/usamune/tex.04400.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_F[] = {
#include "textures/usamune/tex.04600.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_E[] = {
#include "textures/usamune/tex.04800.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_D[] = {
#include "textures/usamune/tex.04A00.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_C[] = {
#include "textures/usamune/tex.04C00.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_B[] = {
#include "textures/usamune/tex.04E00.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_A[] = {
#include "textures/usamune/tex.05000.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_9[] = {
#include "textures/usamune/tex.05200.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_8[] = {
#include "textures/usamune/tex.05400.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_7[] = {
#include "textures/usamune/tex.05600.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_6[] = {
#include "textures/usamune/tex.05800.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_5[] = {
#include "textures/usamune/tex.05A00.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_4[] = {
#include "textures/usamune/tex.05C00.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_3[] = {
#include "textures/usamune/tex.05E00.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_2[] = {
#include "textures/usamune/tex.06000.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_1[] = {
#include "textures/usamune/tex.06200.rgba16.inc.c"
};

ALIGNED8 static const Texture texture_usamune_char_0[] = {
#include "textures/usamune/tex.06400.rgba16.inc.c"
};

const Texture *const usamune_sprite_lut[] = {
  0x0, texture_usamune_char_exclamation, texture_usamune_char_double_quote, texture_usamune_char_double_exclamation,
  0x0, texture_usamune_char_percent, texture_usamune_char_ampersand, texture_usamune_char_apostrophe,
  0x0, 0x0, texture_usamune_sprite_times, texture_usamune_sprite_coin,
  texture_usamune_sprite_mario, texture_usamune_sprite_star, texture_usamune_sprite_joystick, texture_usamune_sprite_key,
  texture_usamune_char_0, texture_usamune_char_1, texture_usamune_char_2, texture_usamune_char_3,
  texture_usamune_char_4, texture_usamune_char_5, texture_usamune_char_6, texture_usamune_char_7,
  texture_usamune_char_8, texture_usamune_char_9, 0x0, 0x0,
  0x0, 0x0, texture_usamune_sprite_arrow_right, texture_usamune_char_question_mark,
  0x0, texture_usamune_char_A, texture_usamune_char_B, texture_usamune_char_C,
  texture_usamune_char_D, texture_usamune_char_E, texture_usamune_char_F, texture_usamune_char_G,
  texture_usamune_char_H, texture_usamune_char_I, texture_usamune_char_J, texture_usamune_char_K,
  texture_usamune_char_L, texture_usamune_char_M, texture_usamune_char_N, texture_usamune_char_O,
  texture_usamune_char_P, texture_usamune_char_Q, texture_usamune_char_R, texture_usamune_char_S,
  texture_usamune_char_T, texture_usamune_char_U, texture_usamune_char_V, texture_usamune_char_W,
  texture_usamune_char_X, texture_usamune_char_Y, texture_usamune_char_Z, 0x0,
  0x0, 0x0, texture_usamune_sprite_arrow_up, 0x0,
  0x0, texture_usamune_char_A, texture_usamune_char_B, texture_usamune_char_C,
  texture_usamune_char_D, texture_usamune_char_E, texture_usamune_char_F, texture_usamune_char_G,
  texture_usamune_char_H, texture_usamune_char_I, texture_usamune_char_J, texture_usamune_char_K,
  texture_usamune_char_L, texture_usamune_char_M, texture_usamune_char_N, texture_usamune_char_O,
  texture_usamune_char_P, texture_usamune_char_Q, texture_usamune_char_R, texture_usamune_char_S,
  texture_usamune_char_T, texture_usamune_char_U, texture_usamune_char_V, texture_usamune_char_W,
  texture_usamune_char_X, texture_usamune_char_Y, texture_usamune_char_Z, 0x0,
  0x0, 0x0, 0x0, 0x0
};

static const Vtx usamune_vertex_text_bg_box[] = {
    {{{     0,    -80,      0}, 0, {     0,      0}, {0xff, 0xff, 0xff, 0xff}}},
    {{{   130,    -80,      0}, 0, {     0,      0}, {0xff, 0xff, 0xff, 0xff}}},
    {{{   130,      0,      0}, 0, {     0,      0}, {0xff, 0xff, 0xff, 0xff}}},
    {{{     0,      0,      0}, 0, {     0,      0}, {0xff, 0xff, 0xff, 0xff}}},
};

const Gfx dl_usamune_text_begin[] = {
  gsDPPipeSync(),
  gsDPSetTexturePersp(G_TP_NONE),
  gsDPSetCombineMode(G_CC_FADEA, G_CC_FADEA),
  gsDPSetEnvColor(255, 255, 255, 255),
  gsDPSetRenderMode(G_RM_AA_XLU_SURF, G_RM_AA_XLU_SURF2),
  gsDPSetTextureFilter(G_TF_POINT),
  gsSPEndDisplayList()
};

const Gfx dl_usamune_text_end[] = {
  gsDPPipeSync(),
  gsDPSetTexturePersp(G_TP_PERSP),
  gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2),
  gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
  gsDPSetEnvColor(255, 255, 255, 255),
  gsDPSetTextureFilter(G_TF_BILERP),
  gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_OFF),
  gsSPEndDisplayList()
};

const Gfx dl_usamune_draw_text_bg_box[] = {
  gsDPPipeSync(), //E7000000 00000000
  gsSPClearGeometryMode(G_LIGHTING), //B6000000 00020000
  gsDPSetCombineMode(G_CC_FADE, G_CC_FADE), //FC42CA85 FF97FFFF
  gsDPSetRenderMode(G_RM_XLU_SURF, G_RM_XLU_SURF2), //B900031D 00504240
  gsSPVertex(usamune_vertex_text_bg_box, 4, 0), //04300040 80419230
  gsSP1Triangle(0, 1, 2, 0x0), //BF000000 00000A14
  gsSP1Triangle(0, 2, 3, 0x0), //BF000000 0000141E
  gsSPEndDisplayList() //B8000000 00000000
};
