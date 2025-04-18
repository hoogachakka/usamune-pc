#include "usamune.h"
#include "usamune_timer.h"
#include "usamune_settings.h"
#include "game_init.h"
#include "segment2.h"
#include "level_update.h"
#include "course_table.h"
#include "print.h"
#include "area.h"
#include "mario_animation_ids.h"

extern void render_hud_timer(void);
extern void render_hud_tex_lut(s32 x, s32 y, u8 *texture);

static void usamune_handle_misc_timer(void);

static struct uMiscTimer
{
  u16 time;        // Time displayed on screen
  s8 visibleTimer; // Time until timer disappears
  u8 trigger;
  s16 delayedSetting;
  u8 delayedTrigger;
} uMiscTimer;

static u8 uStopCastleTimer = 0;

u8 uGlobalTimerActive = FALSE;
s8 uStoppedTimerVisible = 0;
u16 uGlobalTimer = 0;
u16 uStoppedLevelTimer = 0;
u16 uIngameMenuTimer = 0;
u8 uTimerFlags = 0;
u8 uPssTimerFlags = 0;
u8 uMiscTimerTriggered = 0;
u16 DAT_80417c68 = 0;

void usamune_init_level_timer(u8 reset) {
  uDustFrameCounter = 0;
  uIngameMenuTimer = 0;
  uGlobalTimerActive = TRUE;
  uGlobalTimer = 0;
  if (reset && (uGlobalSettingsTable[DEFLT_DEFTIME])) {
    uGlobalTimer = uDefaultTime;
  }

  uStoppedLevelTimer = 0;
  uStoppedTimerVisible = 0;
  //TODO: uRealFadeTmrActive

  uTimerFlags &= 0xF1;
  usamune_set_pss_flags();
  uMiscTimer.time = 0;
  uMiscTimer.visibleTimer = 0;
  uMiscTimer.trigger = 0;
  uMiscTimer.delayedSetting = -1;
  uMiscTimer.delayedTrigger = 0;
  uStopCastleTimer = FALSE;
  
}

void usamune_init_timer_flags(void) {
  uStoppedLevelTimer = 0;
  uStoppedTimerVisible = 0;
  uStopCastleTimer = FALSE;
  uMiscTimer.time = 0;
  uMiscTimer.visibleTimer = 0;
  uMiscTimer.trigger = 0;
  uMiscTimer.delayedSetting = -1;
  uMiscTimer.delayedTrigger = 0;
  usamune_set_pss_flags();
  uTimerFlags &= 0xf3;
}

void usamune_handle_timers(void) {
  if ((uStoppedTimerVisible != 0) && (--uStoppedTimerVisible == 0)) {
    if (uStopCastleTimer) {
      if (uGlobalSettingsTable[TIMER_STOP] != 0) {
	uGlobalTimer = 0;
      }
      uGlobalTimerActive = TRUE;
      uStopCastleTimer = FALSE;
    }
    uStoppedLevelTimer = 0;
    usamune_set_pss_flags();
    uTimerFlags &= ~TIMER_VISIBLE;
  }
  
  if (uGlobalSettingsTable[MISCT_MISCTMR]) {
    usamune_handle_misc_timer();
  }
}

//causes misc timer to display
void usamune_trigger_misc_timer(u8 id, u8 val) {
  if (uGlobalSettingsTable[id]) {
    uMiscTimer.trigger = val;
  }
}

//causes misc timer to display, delayed by one frame
void usamune_trigger_misc_timer_delayed(u8 id, u8 val) {
  uMiscTimer.delayedTrigger = val;
  uMiscTimer.delayedSetting = id;
}

static void usamune_handle_misc_timer(void) {
  static u8 next_trigger = 0;
  
  if (uMiscTimer.trigger != 0 &&
      uMiscTimer.trigger != next_trigger && 
      uGlobalSettingsTable[TIMER_STOP] != 0 &&
      uGlobalSettingsTable[TIMER_DISPLAY] != 0) {
    uMiscTimer.visibleTimer = 30;
    uMiscTimer.time = (uPssTimerFlags == 0) ? gHudDisplay.timer : uGlobalTimer;
    usamune_set_pss_flags();
    uTimerFlags |= MISC_TIMER_VISIBLE;
  }
  else {
    if ((uMiscTimer.visibleTimer != 0) &&
	(--uMiscTimer.visibleTimer == 0)) {
      uMiscTimer.time = 0;
      usamune_set_pss_flags();
      uTimerFlags &= ~MISC_TIMER_VISIBLE;
    }
  }
  
  next_trigger = uMiscTimer.trigger;
  uMiscTimer.trigger = 0;
  if (uMiscTimer.delayedTrigger != 0) {
    usamune_trigger_misc_timer(uMiscTimer.delayedSetting, uMiscTimer.delayedTrigger);
    uMiscTimer.delayedTrigger = 0;
    uMiscTimer.delayedSetting = -1;
  }
}

//if frames > 0, timer will deactivate automatically in (frames) frames
//if frames == 0, timer will deactivate when prompted to manually
void usamune_trigger_timer(u8 frames) {
  if (uGlobalSettingsTable[TIMER_STOP] != 0) {
    uStoppedLevelTimer = (uPssTimerFlags == 0) ? uGlobalTimer : gHudDisplay.timer;
    uStoppedTimerVisible = frames;
  }
  if (uGlobalSettingsTable[TIMER_DISPLAY] != 0) {
    usamune_set_pss_flags();
    uStoppedTimerVisible = frames;
    uTimerFlags |= TIMER_VISIBLE;
  }
}

void usamune_reset_castle_timer(void) {
  uStoppedTimerVisible = 2;
  uStopCastleTimer = TRUE;
}

void usamune_time_star_grab(u32 noExit) {
  //TODO: nonstop
  if (uGlobalSettingsTable[MISC_NONSTOP] != 0) {
    usamune_trigger_timer(31);
  }
  else {
    if (uGlobalSettingsTable[TIMER_STOP] & 1) {
      uStoppedTimerVisible = 0;
      uStoppedLevelTimer = uGlobalTimer;
    }
    if (uGlobalSettingsTable[TIMER_DISPLAY] & 1) {
      usamune_set_pss_flags();
      if (!noExit) {
	uTimerFlags &= ~RACE_TIMER_ACTIVE;
      }
      uStoppedTimerVisible = 0;
      uTimerFlags |= TIMER_VISIBLE;
    }
  }
  //TODO: real timer
  
}

void usamune_time_star_dance(u16 actionTimer, u8 exit) {
  if (actionTimer == 0) {
    if (uGlobalSettingsTable[TIMER_STOP] & 2) {
      uStoppedTimerVisible = 0;
      uStoppedLevelTimer = uGlobalTimer;
    }
    if (uGlobalSettingsTable[TIMER_DISPLAY] != 0) {
      uTimerFlags |= TIMER_VISIBLE;
      if (exit) {
	uTimerFlags &= ~RACE_TIMER_ACTIVE;
      }
      usamune_set_pss_flags();
      uStoppedTimerVisible = 0;
    }
    //TODO: real timer, attempt stuff?
  }
  else if (actionTimer == 26) {
    uStoppedLevelTimer += DAT_80417c68;
    if (uStoppedLevelTimer > 17999) {
      uStoppedLevelTimer = 17999;
    }
  }
  else if (actionTimer == 52) {
    if (!exit) {
      uStoppedLevelTimer = 0;
      usamune_set_pss_flags();
      uTimerFlags &= ~TIMER_VISIBLE;
      //TODO: other vars i dont know nothin bout
    }
    else {
      uTimerFlags &= ~TIMER_VISIBLE;
      if (gHudDisplay.flags & HUD_DISPLAY_FLAG_TIMER) {
	uTimerFlags |= RACE_TIMER_ACTIVE;
      }
      usamune_set_pss_flags();
    }
  }
}

void usamune_time_star_dance_water(u16 actionTimer, u8 noExit) {
  if (actionTimer == 0) {
    if (uGlobalSettingsTable[TIMER_STOP] & 2) {
      uStoppedTimerVisible = 0;
      uStoppedLevelTimer = uGlobalTimer;
    }
    if (uGlobalSettingsTable[TIMER_DISPLAY] != 0) {
      usamune_set_pss_flags();
      if (!noExit) {
	uTimerFlags &= ~RACE_TIMER_ACTIVE;
      }
      uStoppedTimerVisible = 0;
      uTimerFlags |= TIMER_VISIBLE;
    }
  }
  /* else if (actionTimer == 26) { */
  /*   uStoppedLevelTimer += DAT_80417c68; */
  /*   if (uStoppedLevelTimer > 17999) { */
  /*     uStoppedLevelTimer = 17999; */
  /*   } */
  /* } */
  else if (actionTimer == 52) {
    if (noExit) {
      uStoppedLevelTimer = 0;
      usamune_set_pss_flags();
      uTimerFlags &= ~TIMER_VISIBLE;
      //TODO: other vars i dont know nothin bout
    }
    else {
      usamune_set_pss_flags();
      uTimerFlags &= ~TIMER_VISIBLE;
      if (gHudDisplay.flags & HUD_DISPLAY_FLAG_TIMER) {
	uTimerFlags |= RACE_TIMER_ACTIVE;
      }
    }
  }
}


void usamune_time_teleport_fadeout(s16 warpOp, s16 warpTimer) {
  if (uStoppedLevelTimer == 0) {
    if ((warpOp < 3 && warpTimer == 29) ||
	(warpOp == 5 && warpTimer == 19 &&
	 uGlobalSettingsTable[TIMER_FADETMR] != 0)) {      
      usamune_trigger_timer(0);
    }
  }
}

void usamune_time_warp_fadeout(void) {
  u8 inBowserCourse = ((u16)(gCurrCourseNum - 16) < 3);
  if (inBowserCourse) {
    usamune_trigger_timer(0);
    //TODO: rta
  }
  else if (uGlobalSettingsTable[TIMER_FADETMR] != 0 || gCurrCourseNum == COURSE_NONE) {
    usamune_trigger_timer(0);
  }

}

void usamune_set_pss_flags(void) {
  switch (uGlobalSettingsTable[TIMER_PSSRACE]) {
  case 0:
    //display vanilla timer instead of usamune timer
    uPssTimerFlags = (uTimerFlags >> 1) & 1;
    break;
  case 1:
    //show race time using usamune timer graphics (if timer is running)
    uPssTimerFlags = (sTimerRunning != 0) << 1; 
    break;
  default:
    uPssTimerFlags = 0;
    break;
  }
}

void usamune_render_hud_timer(void) {
  if (uPssTimerFlags == 1) {
    render_hud_timer();
  } 
  else if (uTimerFlags) { 
    u16 timerValFrames;
    if ((timerValFrames = uStoppedLevelTimer, timerValFrames == 0) &&
	(timerValFrames = uMiscTimer.time, timerValFrames == 0)) {
      timerValFrames = (uPssTimerFlags == 2) ? gHudDisplay.timer : uGlobalTimer;
    }
    usamune_print_timer(timerValFrames);
  }  
}

void usamune_menu_render_timer(void) {
  if (!gHudDisplay.flags) {
    return;
  }
  gSPDisplayList(gDisplayListHead++, dl_usamune_text_begin);
  if (uPssTimerFlags == 1) {
    usamune_render_timer(uTimerScreenPos, 204, gHudDisplay.timer);
  }
  else {
    u16 timerValFrames;
    if ((timerValFrames = uStoppedLevelTimer, timerValFrames == 0) &&
	(timerValFrames = uMiscTimer.time, timerValFrames == 0)) {
      timerValFrames = (uPssTimerFlags == 2) ? gHudDisplay.timer : uGlobalTimer;
    }
    usamune_render_timer(uTimerScreenPos, 204, timerValFrames);
  }
  gSPDisplayList(gDisplayListHead++, dl_usamune_text_end);
}

void usamune_print_timer(u16 timerValFrames) {
  u8 *(*hudLUT)[58];
  u16 timerMins;
  u16 timerSecs;
  u16 timerFracSecs;

  hudLUT = segmented_to_virtual(&main_hud_lut);

  timerMins = timerValFrames / (30 * 60);
  timerSecs = (timerValFrames - (timerMins * 1800)) / 30;
  timerFracSecs = (10 * ((timerValFrames - (timerMins * 1800) - (timerSecs * 30)) & 0xFFFF)) / 3;

  print_text_fmt_int(uTimerScreenPos, 185, "%0d", timerMins);
  print_text_fmt_int(uTimerScreenPos + 20, 185, "%02d", timerSecs);
  print_text_fmt_int(uTimerScreenPos + 54, 185, "%02d", timerFracSecs);
  
  gSPDisplayList(gDisplayListHead++, dl_hud_img_begin);
  render_hud_tex_lut(uTimerScreenPos + 10, 32, (*hudLUT)[GLYPH_APOSTROPHE]);
  render_hud_tex_lut(uTimerScreenPos + 45, 32, (*hudLUT)[GLYPH_DOUBLE_QUOTE]);
  gSPDisplayList(gDisplayListHead++, dl_hud_img_end);
}

