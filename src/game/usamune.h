#ifndef USAMUNE_H
#define USAMUNE_H

#include "sm64.h"

void usamune_main(void);
s16 usamune_render_menus_and_dialogs(void);
void usamune_render_timer(s32 x, s32 y, u16 time);
void usamune_menu_main(struct Controller *controller);
void usamune_load_menu(void);
void usamune_print_text(s32 x, s32 y, const char* str);
void usamune_print_text_fmt_int(s32 x, s32 y, const char* str, s32 n);
void usamune_warp_level(u8 warpMode);
s32 usamune_init_level(void);
void usamune_clear_setting(u32 settingId, s16 saveFile);
u8 find_next_valid_warp(u8 currWarpId, u8 field, s8 dir);
void usamune_save_file_load_all(void);
void usamune_menu_save_settings(void);
void usamune_prevent_double_warp(void);


enum SaveSlotText
  {
    SS_TEXT_NONE,
    SS_TEXT_LOAD,
    SS_TEXT_SAVE
  };

typedef void (*actionFunc)(u8, u16, u8*);

typedef struct uMenuAction
{
  u16 trigger;
  actionFunc action;
} uMenuAction;

typedef struct uMenuSetting
{
  const char* name;
  u8* currOption;
  u8 numOptions;
  u8 flags;
  void (*menu_bhv_func)(struct uMenuSetting*);
  u32 (*render_func)(struct uMenuSetting*);
  void (*apply_func)(struct uMenuSetting*, u8, s16);
  struct uMenuSetting* subSetting;
  const char** options;
} uMenuSetting;

typedef struct uMenuSettingCategory
{
  const char* name;
  u8 currSetting;
  u8 numSettings;
  uMenuSetting* settingsTable;
} uMenuSettingCategory;

typedef struct uWarpDest
{
  u8 levelNum;
  u8 areaIdx;
  u8 nodeId;
  u8 prevLevel;
  u8 flags;
} uWarpDest;

typedef struct uLevelOption
{
  const char* name;
  u8 warpID;
} uLevelOption;

extern u8 uGlobalSettingsTable[];
extern u8 uPrevWarpMode;
extern uWarpDest* uNextWarpDest;
extern u8 DAT_80417c44;
extern u8 uMenuActive;
extern u8 uStopDoubleWarp;
extern u8 uDustFrameCounter;
extern u16 uLandingDustFrames;
extern u8 uStarSelectActive;
extern u8 uStageTextReadArray[];
extern u32 uMoatDrainFlags;
extern s32 uTimerScreenPos;

extern const Texture *const usamune_sprite_lut[];
extern const Gfx dl_usamune_draw_text_bg_box[];
extern const Gfx dl_usamune_text_begin[];
extern const Gfx dl_usamune_text_end[];

#endif
