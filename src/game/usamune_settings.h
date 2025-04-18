#ifndef USAMUNE_SETTINGS_H
#define USAMUNE_SETTINGS_H

#include "sm64.h"

#define DEF_SETTING(name, id) name = id,

enum uSetting {
#include "usamune_settings_defines.h"
};

//macro that initializes a setting along with a list of options for that setting

#define INIT_SETTING(txt, id, f, ...) \
  (uMenuSetting){ \
    .name = txt, \
    .currOption = &uGlobalSettingsTable[id],	\
    .numOptions = sizeof((const char*[]){ __VA_ARGS__ }) / sizeof(const char*), \
    .flags = f, \
    .menu_bhv_func = usamune_setting_##id##_menu_bhv,	\
    .render_func = usamune_setting_##id##_render,	\
    .apply_func = usamune_setting_##id##_apply,	\
    .subSetting = &usamune_##id##_subsetting, \
    .options = (const char*[]){ __VA_ARGS__ } \
  }

#define INIT_SUBSETTING(txt, stng, id, f, ...)				\
  uMenuSetting usamune_##stng##_subsetting = (uMenuSetting){ \
    .name = txt, \
    .currOption = &uGlobalSettingsTable[id],	\
    .numOptions = sizeof((const char*[]){ __VA_ARGS__ }) / sizeof(const char*), \
    .flags = f, \
    .menu_bhv_func = NULL,	\
    .render_func = NULL,	\
    .apply_func = NULL,	\
    .subSetting = NULL, \
    .options = (const char*[]){ __VA_ARGS__ } \
  }

extern u32 uDefaultCoinCount;
extern u32 uDefaultSwimStrength;
extern u8 uSwimStrengthCursorPos;
extern u8 uCoinCountCursorPos;
extern u32 uDefaultMissionSetting;
extern u32 uDefaultRedCoinCount;
extern u32 uDefaultRng;
extern u8 uDefRngCursorPos;
extern u16 uDefaultTime;
extern u8 uDefTimeCursorPos;

u32 usamune_stgtxt_get_save_flags(void);
u32 usamune_stgtxt_get_star_flags(s32 fileIndex, s32 courseIndex);


#endif
