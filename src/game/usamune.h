#ifndef USAMUNE_H
#define USAMUNE_H

#include "sm64.h"

void usamune_main(void);
s16 usamune_render_menus_and_dialogs(void);
void usamune_menu_main(struct Controller *controller);
void usamune_load_menu(void);
void usamune_print_text(s32 x, s32 y, const char* str);
void usamune_print_text_fmt_int(s32 x, s32 y, const char* str, s32 n);
void usamune_print_save_slot_text(void);
void usamune_render_hud(void);

enum SaveSlotText
  {
    NONE,
    TEXT_LOAD,
    TEXT_SAVE
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

extern const Texture *const usamune_sprite_lut[];
extern const Gfx dl_usamune_draw_text_bg_box[];
extern const Gfx dl_usamune_text_begin[];
extern const Gfx dl_usamune_text_end[];

#endif
