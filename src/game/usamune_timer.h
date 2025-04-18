#ifndef USAMUNE_TIMER_H
#define USAMUNE_TIMER_H

#include "sm64.h"

extern s8 uStoppedTimerVisible;
extern u8 uTimerFlags;
extern u8 uPssTimerFlags;
extern u8 uGlobalTimerActive;
extern u16 uGlobalTimer;
extern u16 uStoppedLevelTimer;
extern u16 uIngameMenuTimer;

enum uTimerFlag {
  TIMER_ALWAYS_VISIBLE = (1 << 0),
  RACE_TIMER_ACTIVE = (1 << 1),
  TIMER_VISIBLE = (1 << 2),
  MISC_TIMER_VISIBLE = (1 << 3)
};

void usamune_init_level_timer(u8 reset);
void usamune_handle_timers(void);
void usamune_menu_render_timer(void);
void usamune_trigger_timer(u8 time);
void usamune_reset_castle_timer(void);
void usamune_trigger_misc_timer(u8 id, u8 val);
void usamune_trigger_misc_timer_delayed(u8 id, u8 val);
void usamune_print_timer(u16 timerValFrames);
void usamune_time_door_open(void);
void usamune_init_timer_flags(void);
void usamune_set_pss_flags(void);
void usamune_time_teleport_fadeout(s16 warpOp, s16 warpTimer);
void usamune_time_star_grab(u32 noExit);
void usamune_time_star_dance(u16 actionTimer, u8 exit);
void usamune_time_star_dance_water(u16 actionTimer, u8 noExit);
void usamune_time_entering_star_door(void);
void usamune_time_unlocking_key_door(void);

#endif
