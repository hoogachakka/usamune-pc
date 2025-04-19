#include "usamune.h"
#include "usamune_timer.h"
#include "usamune_settings.h"
#include "game_init.h"
#include "ingame_menu.h"
#include "area.h"
#include "level_update.h"
#include "object_list_processor.h"
#include "sound_init.h"
#include "dialog_ids.h"
#include "print.h"
#include "hud.h"
#include "gfx_dimensions.h"
#include "audio/external.h"
#include "save_file.h"
#include "moving_texture.h"
#include "envfx_snow.h"
#include <string.h>
#include <stdio.h>

#define MENU_ACTION_NUM 14
#define MENU_ACTION_PREPARE 1
#define MENU_ACTION_EXECUTE 2

#define PLAY_MODE_NORMAL 0
#define PLAY_MODE_PAUSED 2
#define PLAY_MODE_CHANGE_AREA 3
#define PLAY_MODE_CHANGE_LEVEL 4
#define PLAY_MODE_FRAME_ADVANCE 5

#define WARP_TYPE_NOT_WARPING 0
#define WARP_TYPE_CHANGE_LEVEL 1
#define WARP_TYPE_CHANGE_AREA 2
#define WARP_TYPE_SAME_AREA 3

u8 uMenuActive = 0;
u8 uSaveStatesEnabled;

char uMenuTextBuffer[24];

s8 uSaveStateSlot = 0;
s8 uLoadStateSlot = 0;
u8 uSaveSlotText = SS_TEXT_NONE;
u8 uSaveSlotTextTimer = 0;
u8 uSaveSlotTextVisible = 0;
static u32 uSaveSlotTextPosX = 22;
static u32 uSaveSlotTextPosY = 185;
s8 uWallKickDispTimer = 0;
u8 uWallKickFrame = 0;
u32 DAT_80417c48;
u32 DAT_80417c4c;
u32 DAT_80417c50;
u32 DAT_80417c54;
u32 DAT_80417c58;
u32 DAT_80417c5c;
u32 DAT_80417c60;
u32 DAT_804168e0;
u16 uMenuFadeTimer;
u8 uMenuNextSound;
u8 uInSubMenu;
u8 uInEditMode;
static u32 uMenuShowCursor = 0;
u8 uLockMenuArrowPos;
u8 uLockSubMenuArrowPos;
u8 uLockScroll;
u32 uFlavorTextDisplayed = 0;
u8 uFlavorTextTimer = 0;
u8 uCurrLevelSelection = 0;
u8 uCustomLevelEditArrowIndex = 0;
u16 uLandingDustFrames = 0;
u8 uStageTextReadArray[25];
u32 uMoatDrainFlags = 0;
u8 uDoSaveSettings = FALSE;

u8 DAT_80417c6a = 0;
u8 DAT_80417c6b = 0;
u8 DAT_80417c6c = 0;
u8 DAT_80417c44 = 0;

u8 DAT_80417bd0 = 0;
u8 DAT_80417bd2 = 0;

u8 DAT_80417bca = 0;
u8 DAT_80417bcb = 0;
u8 DAT_80417bcc = 0;
u8 DAT_80417bcd = 0;
u8 DAT_80417bce = 0;
u8 DAT_80417bcf = 0;

u8 uDustFrameCounter = 0;

s16 uPrevMenuMode = -1;
s16 uPrevDialogID = -1;
u8 uPrevWarpTransitionActive = FALSE;

s8 uAttemptCounterVisible = 0;
u8 uPrevLevelNum = 0;
struct WarpDest uNextLevelWarpDest;
uWarpDest *uNextWarpDest;

u8 uPrevWarpMode = 0;
s32 uTimerScreenPos = 229;


extern uMenuSettingCategory uMenuSettingCategories[];
uMenuSettingCategory* uMenuCurrCategory = &uMenuSettingCategories[0];

extern s16 gWarpTransDelay;
extern u8 sTransitionColorFadeCount[];
extern u8 sTransitionTextureFadeCount[];
extern void set_play_mode(s16 playMode);
extern void shade_screen(void);
extern s32 init_level(void);

extern s16 gMenuMode;
extern s16 gDialogID;
extern u8 gWarpTransRed;
extern u8 gWarpTransGreen;
extern u8 gWarpTransBlue;
extern u32 gWarpTransFBSetColor;
extern struct CreditsEntry *sDispCreditsEntry;
extern s16 sSwimStrength;
extern s16 sPowerMeterStoredHealth;
extern struct PowerMeterHUD sPowerMeterHUD;
extern s32 sPowerMeterVisibleTimer;
extern struct Object *gMarioPlatform;

extern uWarpDest uWarpDestTable[];
extern uLevelOption uLevelSelectOptions[];

extern u16 gRandomSeed16;

actionFunc (*uMenuActionTable)[16];

static void usamune_menu_act_close_menu(u8, u16, u8*);
static void usamune_menu_act_move_cursor(u8, u16, u8*);
static void usamune_menu_act_level_select_move_cursor(u8, u16, u8*);
static void usamune_menu_act_load_level(u8, u16, u8*);
static void usamune_menu_act_modify_settings(u8, u16, u8*);
static void usamune_menu_act_change_custom_warp(u8, u16, u8*);
static void usamune_menu_act_switch_page(u8, u16, u8*);
static void usamune_menu_render_options(u8, u16, u8*);
static void usamune_menu_render_level_select(u8, u16, u8*);
void usamune_menu_change_setting(uMenuSetting* setting, u8 param_2);
static void usamune_exit_menu_or_reset(u8 reset);
static void usamune_calc_sub_menu_first_text_index(u8* firstTextIndex);
static void reset_action_timers(void);
static void reset_level_vars(void);
static void usamune_reset_environmental_levels(void);
static void usamune_menu_switch_page(void);
static void usamune_print_save_slot_text(void);

u8 uMenuActionTimers[16];

actionFunc uMenuActionsPage1[] = {
  NULL,
  usamune_menu_act_close_menu,
  usamune_menu_act_switch_page,
  usamune_menu_act_switch_page,
  NULL,
  NULL,
  usamune_menu_act_move_cursor,
  usamune_menu_act_move_cursor,
  usamune_menu_act_move_cursor,
  usamune_menu_act_move_cursor,
  NULL,
  NULL,
  NULL,
  NULL,
  usamune_menu_act_modify_settings,
  usamune_menu_render_options
};

actionFunc uMenuActionsPage2[] = {
  usamune_menu_act_load_level,
  usamune_menu_act_close_menu,
  usamune_menu_act_switch_page,
  usamune_menu_act_switch_page,
  NULL,
  NULL,
  usamune_menu_act_level_select_move_cursor,
  usamune_menu_act_level_select_move_cursor,
  usamune_menu_act_level_select_move_cursor,
  usamune_menu_act_level_select_move_cursor,
  NULL,
  NULL,
  NULL,
  NULL,
  usamune_menu_act_change_custom_warp,
  usamune_menu_render_level_select
};

void usamune_main(void) {
  if (uMenuActive) {
    usamune_menu_main(gPlayer1Controller);
  }
  else if (gPlayer1Controller->buttonPressed & (D_JPAD | L_JPAD | R_JPAD | L_TRIG | R_TRIG)) {
    if (gPlayer1Controller->buttonPressed == L_JPAD ||
	gPlayer1Controller->buttonPressed == R_JPAD) {
      if (uGlobalSettingsTable[RESET_SSTATES] != 0) {
	//cycle through save slots
	if (gPlayer1Controller->buttonPressed == R_JPAD) {
	  if (uSaveSlotText == SS_TEXT_SAVE) {
	    if (gPlayer1Controller->buttonDown & Z_TRIG) {
	      uSaveStateSlot = (uSaveStateSlot + 8) % 9;
	    }
	    else {
	      uSaveStateSlot = (uSaveStateSlot + 1) % 9;
	    }
	  }
	  else {
	    uSaveSlotText = SS_TEXT_SAVE;
	  }
	  uSaveSlotTextTimer = 30;
	}
	else if (gPlayer1Controller->buttonPressed == L_JPAD) {
	  if (uSaveSlotText == SS_TEXT_LOAD) {
	    if (gPlayer1Controller->buttonDown & Z_TRIG) {
	      uLoadStateSlot = (uLoadStateSlot + 8) % 9;
	    }
	    else {
	      uLoadStateSlot = (uLoadStateSlot + 1) % 9;
	    }
	  }
	  else {
	    uSaveSlotText = SS_TEXT_LOAD;
	  }
	  uSaveSlotTextTimer = 30;
	}
      }
    }
    else if (gPlayer1Controller->buttonPressed == L_TRIG ||
	     ((gPlayer1Controller->buttonPressed & 0xffdf) == R_TRIG)) {
      
      u8 resetMode = 0;
      if ((gPlayer1Controller->buttonDown & (L_TRIG | R_TRIG)) == (L_TRIG | R_TRIG)) {
	resetMode = uGlobalSettingsTable[RESET_LxR];
      }
      if ((gPlayer1Controller->buttonDown & L_TRIG) && (resetMode == 0)) {
	resetMode = uGlobalSettingsTable[RESET_LBUTTON];
      }

      if (resetMode != 0) {
	usamune_warp_level(resetMode);
      } 
    }
    else if (gPlayer1Controller->buttonPressed == D_JPAD) {
      usamune_load_menu();
    }
  }
}

s16 usamune_render_menus_and_dialogs(void) {
  s16 ret = render_menus_and_dialogs();
  if (uMenuActive) {
    if (gWarpTransition.isActive && !gWarpTransDelay) {
      gWarpTransition.isActive = FALSE;
      gWarpTransDelay = 0;
      sTransitionColorFadeCount[0] = 0;
      sTransitionTextureFadeCount[0] = 0;
      sTransitionTextureFadeCount[1] = 0;
      uPrevWarpTransitionActive = FALSE;
    }
    if (uMenuFadeTimer < 250) {
      uMenuFadeTimer += 25;
    }
    else {
      uMenuFadeTimer = 0xff;
    }
    if ((*uMenuActionTable)[15] != NULL) {
      (*(*uMenuActionTable)[15])(0, 0, 0);
    }
    usamune_menu_render_timer();
  }
  else {    
    if (uGlobalSettingsTable[HUD_DUST] && gHudDisplay.flags) {
      usamune_print_text_fmt_int(220, 20, "%3d", uDustFrameCounter);
    }

    uSaveSlotTextVisible = 0;
    usamune_print_save_slot_text();

    if (uGlobalSettingsTable[DEFLT_DEFRNG]) {
      usamune_print_text_fmt_int(22, 185, "%05d", gRandomSeed16);
    }
    else {
      

    }
    
    gSPDisplayList(gDisplayListHead++, dl_usamune_text_end);  
  }
  return ret;
}

void usamune_menu_main(struct Controller *controller) {
  uMenuShowCursor = 0;
  if ((sCurrPlayMode == PLAY_MODE_NORMAL) && (gMarioObject != NULL)) {
    if (gCurrDemoInput == NULL) {
      set_play_mode(PLAY_MODE_FRAME_ADVANCE);
      lower_background_noise(1);
    }
    else {
      //FUN_8040cf94(1);
    }
  }
  if (uLockScroll && !(controller->buttonDown & D_JPAD)) {
    uLockScroll = FALSE;
  }
  if (uMenuActive) {
    uMenuAction actions[] = {
      { A_BUTTON,     (*uMenuActionTable)[0]  },
      { B_BUTTON,     (*uMenuActionTable)[1]  }, 
      { L_TRIG,       (*uMenuActionTable)[2]  },
      { R_TRIG,       (*uMenuActionTable)[3]  },
      { START_BUTTON, (*uMenuActionTable)[4]  },
      { Z_TRIG,       (*uMenuActionTable)[5]  },
      { U_JPAD,       (*uMenuActionTable)[6]  },
      { D_JPAD,       (*uMenuActionTable)[7]  },
      { L_JPAD,       (*uMenuActionTable)[8]  },
      { R_JPAD,       (*uMenuActionTable)[9]  },
      { U_CBUTTONS,   (*uMenuActionTable)[10] },
      { D_CBUTTONS,   (*uMenuActionTable)[11] },
      { L_CBUTTONS,   (*uMenuActionTable)[12] },
      { R_CBUTTONS,   (*uMenuActionTable)[13] }
    };

    for (int i = 0; i < MENU_ACTION_NUM; i++) {
      u8* menuActionTimer = &uMenuActionTimers[i];
      actionFunc menuAction = actions[i].action;
      u16 menuActionTrigger = actions[i].trigger;

      if (controller->buttonDown & menuActionTrigger) {
	if ((controller->buttonPressed & menuActionTrigger) && uMenuActive && menuAction) {
	  (*menuAction)(MENU_ACTION_PREPARE, menuActionTrigger, menuActionTimer);
	}
	if (*menuActionTimer != 0) {
	  (*menuActionTimer)--;
	}
	if ((*menuActionTimer == 0) && uMenuActive && menuAction) {
	  (*menuAction)(MENU_ACTION_EXECUTE, menuActionTrigger, menuActionTimer);
	}
      }
      else {
	*menuActionTimer = 0;
      }
    }
	
    if (uMenuActive && ((*uMenuActionTable)[14] != NULL)) {
      (*(*uMenuActionTable)[14])(0, 0, 0);
    }
  }

  static s32 uMenuSounds[] = {
    SOUND_MENU_MARIO_CASTLE_WARP,
    SOUND_MENU_MARIO_CASTLE_WARP2,
    SOUND_MENU_CLICK_CHANGE_VIEW,
    SOUND_MENU_MESSAGE_NEXT_PAGE,
    SOUND_MENU_CAMERA_BUZZ
  };

  s32 i = 0;
  if (uMenuNextSound) {
    while (!(uMenuNextSound & 1)) {
      uMenuNextSound >>= 1;
      i++;
    }
    play_sound(uMenuSounds[i], gGlobalSoundSource);
  }
  uMenuNextSound = 0;  
}

u8 uStarSelectActive = 0;

s16 *uEnvironmentalRegions[8];

void usamune_warp_level(u8 warpMode) {
  u8 levelNum = uNextLevelWarpDest.levelNum;
  u8 areaIdx = uNextLevelWarpDest.areaIdx;
  u8 nodeId = uNextLevelWarpDest.nodeId;
  u8 warpType;

  if (warpMode == 4 || warpMode == 5) {
    warpType = WARP_TYPE_CHANGE_LEVEL;
  }
  else {
    if (sCurrPlayMode != PLAY_MODE_NORMAL ||
	gCurrCreditsEntry != NULL) {
      return;
    }
    if (warpMode == 1) { //LRESET
      warpType = WARP_TYPE_CHANGE_AREA;
      levelNum = sWarpDest.levelNum;
      areaIdx = sWarpDest.areaIdx;
      nodeId = sWarpDest.nodeId;
    }
    else {
      warpType = (warpMode == 2) + 1;
    }
  }

  s32 i = 0;
  while (uWarpDestTable[i].levelNum != levelNum ||
	 uWarpDestTable[i].areaIdx != areaIdx ||
	 uWarpDestTable[i].nodeId != nodeId) {
    i++;
    if (i == 168) { //no valid warp dest was found
      uNextWarpDest = NULL;
      return;
    } 
  }
  uNextWarpDest = &uWarpDestTable[i];

  sWarpDest.type = warpType;
  sWarpDest.levelNum = levelNum;
  sWarpDest.areaIdx = areaIdx;
  sWarpDest.nodeId = nodeId;
  sWarpDest.arg = 5;

  if ((warpMode - 1) < 2) { //resetting level (warping to current level)
    for (s32 i = 0; i < 8; i++) {
      struct SpawnInfo* spawnInfo;
      struct Camera* c = gAreaData[i].camera;
      s16* macroObjects = gAreaData[i].macroObjects;

      /* Make sure objects are allowed to respawn */
      for (spawnInfo = gAreaData[i].objectSpawnInfos; spawnInfo != NULL; spawnInfo = spawnInfo->next) {
        spawnInfo->behaviorArg &= 0xFFFF00FF;
      }

      /* same for macro objects */
      if (macroObjects) {
	while (*macroObjects != -1) {
	  if ((*macroObjects & 0x1FF) < 31) {
	    break;
	  }
	  macroObjects[4] &= 0xFF;
	  macroObjects += 5;
	}
      }
      
      if (c) {
	c->cutscene = 0;
	c->mode = c->defMode;
      }
    }

    /* {
    /*   uintptr_t *behaviorAddr = segmented_to_virtual(bhvChainChomp); */
    /*   struct ObjectNode *listHead = &gObjectLists[get_object_list_from_behavior(behaviorAddr)]; */
    /*   struct Object *obj = (struct Object *) listHead->next; */

    /*   while (obj != (struct Object *) listHead) { */
    /* 	if (obj->behavior == behaviorAddr) { */
    /* 	  if (obj->activeFlags != ACTIVE_FLAG_DEACTIVATED) { */
	  
		
    /* 	  } */
    /* 	} */
    /* 	obj = (struct Object *) obj->header.next; */
    /*   } */
    /* } */

    /* { */
    /*   uintptr_t *behaviorAddr = segmented_to_virtual(bhvWigglerHead); */
    /*   struct ObjectNode *listHead = &gObjectLists[get_object_list_from_behavior(behaviorAddr)]; */
    /*   if (listHead) { */
    /* 	struct Object *obj = (struct Object *) listHead->next; */

    /* 	while (obj != (struct Object *) listHead) { */
    /* 	  if (obj->behavior == behaviorAddr) { */
    /* 	    if (obj->activeFlags != ACTIVE_FLAG_DEACTIVATED) { */
    /* 	      if (obj->oAction == WIGGLER_ACT_UNINITIALIZED) { */

    /* 	      } */
    /* 	      else { */

    /* 	      } */
    /* 	      break; */
    /* 	    } */
    /* 	  } */
    /* 	  obj = (struct Object *) obj->header.next; */
    /* 	} */
    /*   } */
    /* } */

    usamune_reset_environmental_levels();
    
    if (levelNum == LEVEL_TOTWC) {
      gWarpTransBlue = 0xFF;
      gWarpTransFBSetColor = 0xFFFFFFFF;  
    }
    else {
      gWarpTransBlue = (uNextWarpDest->flags & 0x12) ? (u8)-1 : 0;
      gWarpTransFBSetColor = (uNextWarpDest->flags & 0x12) ? (u32)-1 : 0x10001;
    }

    //TODO: implement attempt counter stuff here
    gMarioState->numCoins = uGlobalSettingsTable[DEFLT_DEFCOIN] ? uDefaultCoinCount : 0;
    if (uGlobalSettingsTable[DEFLT_DEFRNG]) {
      gRandomSeed16 = uDefaultRng;
    }
    gWarpTransRed = gWarpTransBlue;
    gWarpTransGreen = gWarpTransBlue;
    gHudDisplay.coins = gMarioState->numCoins;

    usamune_init_level_timer(TRUE);
    
    uSaveStateSlot = 0;
    uLoadStateSlot = 0;
  }
  else { //warping to different level
    gSavedCourseNum = 0;
    if (levelNum == LEVEL_TOTWC) {
      gWarpTransFBSetColor = 0xFFFFFFFF;
      gWarpTransRed = 0xFF;
    }
    else {
      if (levelNum == LEVEL_VCUTM) {
	gLoadedGraphNodes[55] = NULL;
      }
      //TODO: warp transition color stuff
    }

    s16 nextCourseNum = gLevelToCourseNumTable[uNextWarpDest->levelNum - 1];
    gWarpTransGreen = gWarpTransRed;
    gWarpTransBlue = gWarpTransRed;

    if (warpMode == 3) {
      gMarioState->numCoins = 0;
      gHudDisplay.coins = 0;
      set_play_mode(PLAY_MODE_CHANGE_LEVEL);
      uStarSelectActive = ((nextCourseNum - 1) < 15);
      uAttemptCounterVisible = 0;
    }
    else if (warpMode != 4 && warpMode != 5) {
      uSaveStateSlot = 0;
      uLoadStateSlot = 0;
    }
    else {
      sDispCreditsEntry = NULL;
      gCurrCreditsEntry = NULL;
      gNeverEnteredCastle = 0;
      gMarioState->numCoins = 0;
      set_play_mode(PLAY_MODE_CHANGE_LEVEL);
      uStarSelectActive = ((nextCourseNum - 1) < 15);
      uAttemptCounterVisible = 0;
      uSaveStateSlot = 0;
      uLoadStateSlot = 0;
    }


  }

  if (warpMode != 5) {
    /* switch (gPlayer1Controller->buttonDown & C_BUTTONS) { */
    /* case D_CBUTTONS: */
      
    /* } */

    reset_level_vars();
  }

  //TODO: DATA_CANNON
  //TODO: DATA_CAP
  //TODO: DEFLT_DEFCURS
  sSwimStrength = (uGlobalSettingsTable[DEFLT_DEFSWIM]) ? uDefaultSwimStrength : 280;

  if (sWarpDest.levelNum == LEVEL_WDW) {
    switch (uGlobalSettingsTable[STAGE_WDW]) {
    case 1:
      gPaintingMarioYEntry = 0.0;
      break;
    case 2:
      gPaintingMarioYEntry = 1382.4;
      break;
    case 3:
      gPaintingMarioYEntry = 1600.0;
      break;
    }
  }
  else if (sWarpDest.levelNum == LEVEL_TTC) {
    switch (uGlobalSettingsTable[STAGE_TTC]) {
    case 1:
      gTTCSpeedSetting = TTC_SPEED_STOPPED;
      break;
    case 2:
      gTTCSpeedSetting = TTC_SPEED_SLOW;
      break;
    case 3:
      gTTCSpeedSetting = TTC_SPEED_RANDOM;
      break;
    case 4:
      gTTCSpeedSetting = TTC_SPEED_FAST;
      break;
    }
  }

  //TODO: zero out all these variables of which i know nothing
  uPrevWarpMode = warpMode;
  uMoatDrainFlags = (uGlobalSettingsTable[STAGE_MOAT] == 2) << 9;
  for (int i = 0; i < 25; i++) {
    uStageTextReadArray[i] = 0;
  }
  uWallKickFrame = 0;
  uWallKickDispTimer = 0;
  uStopDoubleWarp = FALSE;
  sPowerMeterStoredHealth = 8;
  gHudDisplay.wedges = 8;
  gMarioState->prevNumStarsForDialog = gMarioState->numStars;
  gMarioState->health = 0x880;
  sPowerMeterVisibleTimer = 0;
  sPowerMeterHUD.y = 166;
  sPowerMeterHUD.animation = 0;
  gMarioPlatform = NULL;
}

static void usamune_reset_environmental_levels(void) {
  switch (sWarpDest.levelNum) {
  case LEVEL_CCM:
    if (sWarpDest.areaIdx == 1) {
      gCCMEnteredSlide = (sWarpDest.nodeId == 20);
    }
  case LEVEL_SL:
    gSnowParticleCount = 5;
    break;
      
  case LEVEL_CASTLE:
    if (uEnvironmentalRegions[3] != NULL) {
      gEnvironmentLevels[0] = -1228;
      gEnvironmentLevels[2] = -1228;
      uEnvironmentalRegions[3][6] = -1228;
      uEnvironmentalRegions[3][18] = -1228;
    }
    break;
      
  case LEVEL_WDW:
    if (uEnvironmentalRegions[1] != NULL) {
      gEnvironmentLevels[0] = 31;
      uEnvironmentalRegions[1][6] = 31;
    }
    if (uEnvironmentalRegions[2] != NULL) {
      gEnvironmentLevels[0] = -127;
      uEnvironmentalRegions[2][6] = -127;
    }
    break;

  case LEVEL_JRB:
    if (sWarpDest.areaIdx == 2) {
      gSnowParticleCount = 30;
    }
    if (uEnvironmentalRegions[2] != NULL) {
      gEnvironmentLevels[0] = 2765;
      uEnvironmentalRegions[2][6] = 2765;
    }
    break;

  case LEVEL_THI:
    gTHIWaterDrained = FALSE;
    if (uEnvironmentalRegions[1] != NULL) {
      gEnvironmentLevels[2] = 4014;
      uEnvironmentalRegions[1][18] = 4014;
    }
    if (uEnvironmentalRegions[2] != NULL) {
      gEnvironmentLevels[2] = 1204;
      uEnvironmentalRegions[2][18] = 4014;
    }
    break;

  case LEVEL_CASTLE_GROUNDS:
    if (uEnvironmentalRegions[1] != NULL) {
      gEnvironmentLevels[0] = -81;
      gEnvironmentLevels[1] = -81;
      uEnvironmentalRegions[1][6] = -81;
      uEnvironmentalRegions[1][12] = -81;
    }
    break;

  case LEVEL_SA:
    gSnowParticleCount = 30;
    break;   
  }
  gSavedCourseNum = gLevelToCourseNumTable[sWarpDest.levelNum - 1];
}

static void reset_level_vars(void) {
  level_set_transition(0, NULL);
  gDialogBoxOpenTimer = DEFAULT_DIALOG_BOX_ANGLE;
  gDialogBoxScale = DEFAULT_DIALOG_BOX_SCALE;
  gDialogBoxState = 0;
  gDialogScrollOffsetY = 0;
  gDialogBoxType = 0;
  gDialogID = DIALOG_NONE;
  gLastDialogPageStrPos = 0;
  gDialogTextPos = 0;
  gDialogLineNum = 1;
  gLastDialogResponse = 0;
  gDialogResponse = 0;
  gMenuMode = MENU_MODE_NONE;
  gDialogColorFadeTimer = 0;
  gLastDialogLineNum = 0;
  gDialogVariable = 0;
  gDialogTextAlpha = 0;
  gCourseCompleteCoinsEqual = 0;
  gCourseDoneMenuTimer = 0;
  gCourseCompleteCoins = 0;
  gHudFlash = 0;
  gWarpTransition.isActive = FALSE;
  gWarpTransDelay = 0;
  sTransitionColorFadeCount[0] = 0;
  sTransitionTextureFadeCount[0] = 0;
  sTransitionTextureFadeCount[1] = 0;
  uPrevMenuMode = -1;
  uPrevDialogID = -1;
  uPrevWarpTransitionActive = FALSE;
}


s32 usamune_init_level(void) {
  if (gCurrDemoInput != NULL || gCurrCreditsEntry != NULL) {
    
  }
  else {
    uNextLevelWarpDest.levelNum = sWarpDest.levelNum;
    uNextLevelWarpDest.areaIdx = sWarpDest.areaIdx;
    if (sWarpDest.levelNum == LEVEL_THI) {
      if (sWarpDest.areaIdx != 1) {
	uNextLevelWarpDest.areaIdx = 2;
      }
      uNextLevelWarpDest.nodeId = 10;
    }
    else {
      uNextLevelWarpDest.nodeId = sWarpDest.nodeId;
      if (sWarpDest.levelNum != LEVEL_CASTLE &&
	  sWarpDest.levelNum != LEVEL_CASTLE_GROUNDS &&
	  sWarpDest.levelNum != LEVEL_ENDING) {
	uNextLevelWarpDest.nodeId = 10;
	uNextLevelWarpDest.areaIdx = 1;
      }
    }

    if (gCurrCourseNum == COURSE_NONE) { //in hub
      
      //TODO: timer related stuff occurs here
      
      //in case text was supposed to show up when entering castle for the first time, skip it
      if (uGlobalSettingsTable[STAGE_STGTXT] == 3) {
	gNeverEnteredCastle = FALSE;
      }
    }  
  }

  usamune_init_level_timer(0);
  
  for (s32 i = 0; i < 8; i++) {
    uEnvironmentalRegions[i] = NULL;
  }
  return init_level();
}

static void usamune_menu_act_load_level(u8 state, UNUSED u16 input, UNUSED u8* timer) {
  if (state != MENU_ACTION_PREPARE) {
    return;
  }

  if (sCurrPlayMode == PLAY_MODE_FRAME_ADVANCE) {
    uWarpDest nextLevel = uWarpDestTable[uLevelSelectOptions[uCurrLevelSelection].warpID];
    uNextLevelWarpDest.levelNum = nextLevel.levelNum;
    uNextLevelWarpDest.areaIdx = nextLevel.areaIdx;
    uNextLevelWarpDest.nodeId = nextLevel.nodeId;
    usamune_exit_menu_or_reset(1);
    usamune_warp_level(4);
  }

  //TODO: flavor text stuff
}

static void usamune_menu_act_change_custom_warp(UNUSED u8 p1, UNUSED u16 p2, UNUSED u8* p3) {
  if (uCurrLevelSelection != 30) {
    return;
  }

  u8 nextId = uLevelSelectOptions[30].warpID;
  s8 nextArrowIndex = uCustomLevelEditArrowIndex;
  u16 input = gPlayer1Controller->buttonPressed & C_BUTTONS;
  switch (input) {
  case L_CBUTTONS: 
    nextArrowIndex += 1;
    if (nextArrowIndex > 2) {
      nextArrowIndex = 0;
    }
    break;
  case R_CBUTTONS:
    nextArrowIndex -= 1;
    if (nextArrowIndex < 0) {
      nextArrowIndex = 2;
    }
    break;
  case U_CBUTTONS: 
    nextId = find_next_valid_warp(nextId, uCustomLevelEditArrowIndex, 1);
    break;
  case D_CBUTTONS:
    nextId = find_next_valid_warp(nextId, uCustomLevelEditArrowIndex, -1);
    break;
  }

  if (uLevelSelectOptions[30].warpID != nextId) {
    uLevelSelectOptions[30].warpID = nextId;
    uMenuNextSound |= (1 << 3);
  }
  else if (uCustomLevelEditArrowIndex != nextArrowIndex) {
    uCustomLevelEditArrowIndex = nextArrowIndex;
    uMenuNextSound |= (1 << 3);
  }
}

void usamune_load_menu(void) {
  /* char cVar1; */
  /* uint uVar2; */
  /* uint uVar4; */
  /* ulonglong uVar3; */
  /* uint uVar5; */
  /* char *pcVar6; */
  /* char *pcVar7; */
  /* uint uVar8; */
  /* uint uVar9; */
  /* int iVar10; */
  /* char local_38 [56]; */
  
  if (sCurrPlayMode == PLAY_MODE_PAUSED) {
    return;
  }
  if (!uMenuActionTable) {
    uMenuActionTable = &uMenuActionsPage1;
  }
  uMenuActive = TRUE;
  uMenuFadeTimer = 0;

  
  
 /*  iVar10 = DAT_804168e8 + 1; */
 /*  DAT_804168ec = 0; */
 /*  DAT_80416900 = 0; */
 /*  DAT_8041691c = 0; */
 /*  uVar4 = gNumVblanks & 0xffff; */
 /*  uVar8 = gGlobalTimer & 1; */
 /*  uVar9 = gGlobalTimer & 3; */
 /*  if (DAT_80417ce4 == '\0') { */
 /*    DAT_80417ce4 = '\x01'; */
 /*    uVar2 = gGlobalTimer & 0xffff; */
 /*    if (uVar2 == 0x560a) goto LAB_8040cbc4; */
 /*  LAB_8040c958: */
 /*    uVar2 = uVar2 ^ uVar2 << 8; */
 /*    uVar5 = uVar2 & 0xffff; */
 /*    uVar5 = (uVar2 & 0xff) << 1 ^ (uVar5 << 8 | uVar5 >> 8); */
 /*    uVar2 = (uVar5 & 0xffff) >> 1; */
 /*    if ((uVar5 & 1) == 0) { */
 /*      if (uVar2 != 0x55d5) goto LAB_8040cbc8; */
 /*      uVar2 = 0; */
 /*      if (uVar8 != 0) { */
 /*        uVar4 = ~uVar4; */
 /*        uVar5 = 0; */
 /*        goto LAB_8040cbd4; */
 /*      } */
 /*      goto LAB_8040cde0; */
 /*    } */
 /*    uVar5 = uVar2 ^ 0x7e00; */
 /*    uVar4 = uVar4 | uVar5; */
 /*    if (uVar8 != 0) { */
 /*      uVar4 = ~uVar4; */
 /*    } */
 /*    if (uVar2 != 0x280a) goto LAB_8040cbd4; */
 /*    uVar2 = 0; */
 /*  LAB_8040c9ac: */
 /*    uVar2 = uVar2 ^ 0xe074; */
 /*  LAB_8040c9b0: */
 /*    uVar8 = (uVar2 & 0xff) << 8 | uVar2 >> 8; */
 /*  } */
 /*  else { */
 /*    uVar2 = (uint)DAT_80417ce2; */
 /*    if (uVar2 != 0x560a) goto LAB_8040c958; */
 /*  LAB_8040cbc4: */
 /*    uVar2 = 0; */
 /*  LAB_8040cbc8: */
 /*    uVar5 = uVar2 ^ 0xe074; */
 /*    uVar4 = uVar4 | uVar5; */
 /*    if (uVar8 != 0) { */
 /*      uVar4 = ~uVar4; */
 /*    } */
 /*  LAB_8040cbd4: */
 /*    uVar5 = uVar5 ^ uVar5 << 8; */
 /*    uVar8 = uVar5 & 0xffff; */
 /*    uVar8 = (uVar5 & 0xff) << 1 ^ (uVar8 << 8 | uVar8 >> 8); */
 /*    uVar2 = (uVar8 & 0xffff) >> 1; */
 /*    if ((uVar8 & 1) != 0) { */
 /*      uVar2 = uVar2 ^ 0x7e00; */
 /*      goto LAB_8040c9b0; */
 /*    } */
 /*  LAB_8040cde0: */
 /*    uVar8 = 0; */
 /*    if (uVar2 != 0x55d5) goto LAB_8040c9ac; */
 /*    uVar2 = 0; */
 /*  } */
 /*  DAT_80417ce2 = (ushort)uVar2; */
 /*  if (uVar9 == 2) { */
 /*    uVar8 = uVar2 ^ 0x5555; */
 /*  } */
 /*  else if ((uVar9 != 3) && (uVar8 = ~uVar2, uVar9 != 1)) { */
 /*    uVar8 = uVar2 ^ 0xaaaa; */
 /*  } */
 /*  if ((uVar8 & 8) == 0) { */
 /*    uVar8 = uVar8 << 0x10; */
 /*    uVar4 = uVar4 & 0xffff; */
 /*  } */
 /*  else { */
 /*    uVar4 = uVar4 << 0x10; */
 /*    uVar8 = uVar8 & 0xffff; */
 /*  } */
 /*  uVar4 = (uVar4 | uVar8) % 1000; */
 /*  uVar3 = (ulonglong)DAT_80417b6d; */
 /*  uVar9 = uVar4 % 0x29; */
 /*  DAT_804168e8 = iVar10; */
 /*  if (uVar3 == 0) { */
 /*    FUN_8040008c((int)((ulonglong)gNumVblanks * 25000 >> 0x20),(int)((ulonglong)gNumVblanks * 25000) */
 /*                 ,0,0x3a6d); */
 /*  } */
 /*  else { */
 /*    uVar8 = 0; */
 /*    if (uVar3 != 1) { */
 /*      uVar5 = 0; */
 /*      uVar2 = 0; */
 /*      goto LAB_8040cad8; */
 /*    } */
 /*    uVar2 = gNumVblanks * 0x19 >> 0x1e | */
 /*      ((uint)(gNumVblanks * 0x19 < gNumVblanks * 0x18) + */
 /*       (gNumVblanks * 3 >> 0x1d | */
 /* 	((uint)(gNumVblanks * 3 < gNumVblanks * 2) - ((int)gNumVblanks >> 0x1f)) * 8)) * 4; */
 /*    uVar5 = gNumVblanks * 100; */
 /*    uVar8 = uVar5 - ((uVar5 & 0xfffffff) + (uVar2 << 4 | uVar5 >> 0x1c)) % 0xf; */
 /*    uVar3 = (ulonglong) */
 /*      (int)(((uVar2 - (uVar5 < uVar8)) * -0x11111111 + uVar8 * -0x11111112 + */
 /* 	     (int)((ulonglong)uVar8 * 0xeeeeeeef >> 0x20)) * 0x40000000 | */
 /* 	    (uint)((ulonglong)uVar8 * 0xeeeeeeef) >> 2); */
 /*  } */
 /*  uVar8 = 0x3b; */
 /*  if (uVar3 < 36000000) { */
 /*    uVar2 = (uint)((uVar3 & 0xffffffff) / 6000); */
 /*    uVar5 = uVar2 / 0x3c; */
 /*    uVar8 = uVar2 % 0x3c; */
 /*  } */
 /*  else { */
 /*    uVar5 = 99; */
 /*    uVar2 = 5999; */
 /*  } */
 /* LAB_8040cad8: */
 /*  if (iVar10 == 1) { */
 /*    DAT_80416900 = 2; */
 /*    pcVar6 = &DAT_804186e8; */
 /*    pcVar7 = &DAT_80416904; */
 /*    do { */
 /*      cVar1 = *pcVar6; */
 /*      pcVar6 = pcVar6 + 1; */
 /*      *pcVar7 = cVar1; */
 /*      pcVar7 = pcVar7 + 1; */
 /*    } while (cVar1 != '\0'); */
 /*    DAT_8041691c = 0x47; */
 /*  } */
 /*  else if (uVar4 == 777) { */
 /*    DAT_80416900 = 1; */
 /*    pcVar6 = s_You_got_777_804186f0; */
 /*    pcVar7 = &DAT_80416904; */
 /*    do { */
 /*      cVar1 = *pcVar6; */
 /*      pcVar6 = pcVar6 + 1; */
 /*      *pcVar7 = cVar1; */
 /*      pcVar7 = pcVar7 + 1; */
 /*    } while (cVar1 != '\0'); */
 /*    DAT_8041691c = 0x38; */
 /*  } */
 /*  else if ((uVar9 == 0xb) || ((gControllers[0].buttonDown & 0x2000) != 0)) { */
 /*    pcVar7 = local_38; */
 /*    if (uVar2 < 0x3c) { */
 /*      sprintf(pcVar7, "Play time %d min"); */
 /*    } */
 /*    else { */
 /*      sprintf(pcVar7, "Play time %d hr %d min",uVar5,uVar8); */
 /*    } */
 /*    if ((local_38[0] != '\0') && (uVar3 = FUN_80406dc0(local_38 + 1), uVar3 < 0x17)) { */
 /*      DAT_80416900 = 1; */
 /*      pcVar6 = &DAT_80416904; */
 /*      do { */
 /*        cVar1 = *pcVar7; */
 /*        pcVar7 = pcVar7 + 1; */
 /*        *pcVar6 = cVar1; */
 /*        pcVar6 = pcVar6 + 1; */
 /*      } while (cVar1 != '\0'); */
 /*      DAT_8041691c = 0x38; */
 /*    } */
 /*  } */
 /*  else if (uVar9 < 0xb) { */
 /*    uVar4 = 0; */
 /*    if (s_Ty_for_using_Usamune_80418ea0[uVar9 * 0x45 + 0x17] != '\0') { */
 /*      uVar4 = uVar2 < 0x3c ^ 1; */
 /*    } */
 /*    if ((((&DAT_80418ece)[uVar9 * 0x45] == '\0') || */
 /* 	 (*(short *)(&DAT_80418e6c + gCurrCourseNum * 2) < 0)) || */
 /* 	(gMarioStates[0].numCoins < *(short *)(&DAT_80418e6c + gCurrCourseNum * 2))) { */
 /*      iVar10 = uVar4 * 0x17; */
 /*    } */
 /*    else { */
 /*      iVar10 = 0x2e; */
 /*    } */
 /*    iVar10 = uVar9 * 0x45 + iVar10; */
 /*    pcVar7 = s_Ty_for_using_Usamune_80418ea0 + iVar10 + 1; */
 /*    if ((s_Ty_for_using_Usamune_80418ea0[iVar10] != '\0') && */
 /* 	(uVar3 = FUN_80406dc0(pcVar7), uVar3 < 0x17)) { */
 /*      DAT_80416900 = 1; */
 /*      pcVar6 = &DAT_80416904; */
 /*      while( true ) { */
 /*        cVar1 = pcVar7[-1]; */
 /*        *pcVar6 = cVar1; */
 /*        if (cVar1 == '\0') break; */
 /*        pcVar7 = pcVar7 + 1; */
 /*        pcVar6 = pcVar6 + 1; */
 /*      } */
 /*      DAT_8041691c = 0x38; */
 /*    } */
 /*  } */
  uSaveSlotText = 0;
  DAT_80417c48 = 0;
  DAT_80417c4c = 0;
  DAT_80417c50 = 0;
  DAT_80417c54 = 0;
  DAT_80417c58 = 0;
  DAT_80417c5c = 0;
  DAT_80417c60 = 0;
  uPrevMenuMode = gMenuMode;
  uPrevDialogID = gDialogID;
  uPrevWarpTransitionActive = gWarpTransition.isActive;
  gMenuMode = MENU_MODE_NONE;
  gDialogID = DIALOG_NONE;
  gWarpTransition.isActive = FALSE;
  uLockScroll = TRUE;
  if (((sCurrPlayMode == PLAY_MODE_NORMAL) && (gMarioObject != NULL)) && (gCurrDemoInput == NULL)) {
    set_play_mode(PLAY_MODE_FRAME_ADVANCE);
    lower_background_noise(1);
    play_sound(SOUND_MENU_PAUSE_HIGHPRIO, gGlobalSoundSource);
  }
}

static void usamune_menu_act_switch_page(u8 state, UNUSED u16 input, UNUSED u8* timer) {
  if (state == MENU_ACTION_PREPARE) {
    usamune_menu_switch_page();
  }
}

static void usamune_menu_act_close_menu(u8 state, UNUSED u16 input, UNUSED u8* timer) {
  if (state == MENU_ACTION_PREPARE) {
    usamune_exit_menu_or_reset(0);
  }
}

static void usamune_menu_act_move_cursor(u8 state, u16 input, u8* timer) {
  if (uLockScroll) return;

  s8 nextOption = (uInSubMenu) ? uMenuCurrCategory->currSetting : uMenuCurrCategory - uMenuSettingCategories;
  u8 maxOption = (uInSubMenu) ? uMenuCurrCategory->numSettings : 8;
  u8 noUpdate = FALSE;
  u8 changeOption = FALSE;
  

  switch (input) {
  case U_JPAD:
    if (--nextOption == -1) nextOption = maxOption - 1;
    changeOption = TRUE;
    break;
  case D_JPAD:
    if (++nextOption == maxOption) nextOption = 0;
    changeOption = TRUE;
    break;
  case L_JPAD:
    if (!uInSubMenu) noUpdate = TRUE;
    else {
      uInSubMenu = FALSE;
      reset_action_timers();
      changeOption = FALSE;
    }
    break;
  case R_JPAD:
    if (uInSubMenu) noUpdate = TRUE;
    else {
      uInSubMenu = TRUE;
      reset_action_timers();
      changeOption = FALSE;
    }
    break;
  }

  if (!noUpdate) {
    if (changeOption) {
      if (!uInSubMenu) {
	uMenuCurrCategory = &uMenuSettingCategories[nextOption];
      }
      else {
	uMenuCurrCategory->currSetting = nextOption;
      }
      uMenuShowCursor = 1;
    }
    uInEditMode = FALSE;
    uMenuNextSound |= (1 << 3);
    uMenuActionTimers[5] = 0; //Z_TRIG
  }
  
  *timer = (state == MENU_ACTION_PREPARE) ? 10 : 3;
  if (!(input & R_JPAD)) uMenuActionTimers[9] = 0;
  if (!(input & L_JPAD)) uMenuActionTimers[8] = 0;
  if (!(input & D_JPAD)) uMenuActionTimers[7] = 0;
  if (!(input & U_JPAD)) uMenuActionTimers[6] = 0;
}

static void usamune_menu_act_level_select_move_cursor(u8 state, u16 input, u8* timer) {
  if (uLockScroll) return;

  u8 parity = uCurrLevelSelection & 1;
  s8 nextSelection = uCurrLevelSelection;

  switch(input) {
  case D_JPAD:
    nextSelection += 2;
    if (nextSelection > 30) {
      nextSelection = parity;
    }
    uMenuShowCursor = 1;
    break;
  case U_JPAD:
    nextSelection -= 2;
    if (nextSelection < 0) {
      nextSelection = 30 - parity;
    }
    uMenuShowCursor = 1;
    break;
  case L_JPAD:
    nextSelection -= 1;
    if (nextSelection < 0) {
      nextSelection = 30;
    }
    break;
  case R_JPAD:
    nextSelection += 1;
    if (nextSelection > 30) {
      nextSelection = 0;
    }
    break;
  }

  if (nextSelection != uCurrLevelSelection) {
    uCurrLevelSelection = nextSelection;
    uMenuNextSound |= (1 << 3);
    uMenuActionTimers[5] = 0;
  }

  *timer = (state == MENU_ACTION_PREPARE) ? 10 : 3;
  if (!(input & R_JPAD)) uMenuActionTimers[9] = 0;
  if (!(input & L_JPAD)) uMenuActionTimers[8] = 0;
  if (!(input & D_JPAD)) uMenuActionTimers[7] = 0;
  if (!(input & U_JPAD)) uMenuActionTimers[6] = 0;
}

static void usamune_menu_act_modify_settings(UNUSED u8 state, UNUSED u16 input, UNUSED u8* timer) {
  if (!uInSubMenu) {
    return;
  }
  uMenuSetting* currSetting = &uMenuCurrCategory->settingsTable[uMenuCurrCategory->currSetting];
  if (!uInEditMode) {
    if (gPlayer1Controller->buttonPressed & L_CBUTTONS) {
      usamune_menu_change_setting(currSetting, 1);
    }
    else if (gPlayer1Controller->buttonPressed & R_CBUTTONS) {
      usamune_menu_change_setting(currSetting, 2);
    }
  }
  if (currSetting->menu_bhv_func != NULL) {
    (*currSetting->menu_bhv_func)(currSetting);
  }
}

void usamune_menu_change_setting(uMenuSetting* setting, u8 param_2) {
  u8 prevOption = *setting->currOption;
  u8 nextOption = prevOption;
  switch (param_2) {
  case 1:
    if (--nextOption == (u8)-1) nextOption = setting->numOptions - 1;
    break;
  case 2:
    if (++nextOption == setting->numOptions) nextOption = 0;
    break;
  }
  *setting->currOption = nextOption;

  if (prevOption == nextOption) {
    uMenuNextSound |= (1 << 4);
  }
  else {
    if (setting->flags & 1) {
      uDoSaveSettings = TRUE;
    }
    uMenuNextSound |= (1 << 3);
  }

  if (setting->apply_func != NULL) {
    (*setting->apply_func)(setting, prevOption, gCurrSaveFileNum);
  }
}
    
static void reset_action_timers(void) {
  for (int i = 0; i < 16; i++) {
    uMenuActionTimers[i] = 0;
  }
}

  
  

static void usamune_menu_switch_page(void) {
  uInEditMode = FALSE;
  if (uGlobalSettingsTable[MISC_PRESET] == 0) {
    if (uDoSaveSettings) {
      usamune_menu_save_settings();
      uDoSaveSettings = FALSE;
    }
  }

  uMenuNextSound |= 4;
  reset_action_timers();
  
  if (uMenuActionTable == &uMenuActionsPage1) {
    uMenuActionTable = &uMenuActionsPage2;
  }
  else {
    uMenuActionTable = &uMenuActionsPage1;
  }
}

/* static void usamune_menu_act_soft_reset(u8 state, u16 input, u8* timer) { */
/*   if (state == MENU_ACTION_PREPARE) { */
/*     *timer = 15; */
/*   } */
/*   else if (state == MENU_ACTION_EXECUTE) { */
/*     if (sCurrPlayMode == PLAY_MODE_FRAME_ADVANCE) { */
/*       FUN_8040cf94(1); */
/*       FUN_80406884(); */
/*     } */
/*     else { */
/*       usamune_strcpy(uMenuTextBuffer, "Cannot reset here"); */
/*     } */
/*   } */
/* } */

static void usamune_menu_render_options(UNUSED u8 p1, UNUSED u16 p2, UNUSED u8* p3) {
  shade_screen();
  gSPDisplayList(gDisplayListHead++, dl_usamune_text_begin);  
  gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, uMenuFadeTimer);

  u8 firstTextIndex;

  u8 arrowIndex = uMenuCurrCategory - uMenuSettingCategories;
  if (arrowIndex < 2) {
    firstTextIndex = 0;
    uLockMenuArrowPos = FALSE;
  }
  else if (arrowIndex < 6) {
    firstTextIndex = arrowIndex - 2;
    if ((uMenuShowCursor == 1) && !uInSubMenu && uLockMenuArrowPos) {
      uMenuShowCursor = 2;
    }
    uLockMenuArrowPos = TRUE;
  }
  else {
    firstTextIndex = 3;
    uLockMenuArrowPos = FALSE;
  }
 
  usamune_print_text(22, 200, "- Menu -");
  
  s32 yPos = 171;
  u8 textIndex = firstTextIndex;
  if (uInSubMenu) {
    while (textIndex <= firstTextIndex + 4) {
      const char* str = uMenuSettingCategories[textIndex].name;
      if (textIndex != arrowIndex) {
	gDPSetEnvColor(gDisplayListHead++, 165, 165, 165, uMenuFadeTimer);
      }
      else {
	gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, uMenuFadeTimer);
      }
      usamune_print_text(33, yPos, str);
      yPos -= 29;
      textIndex++;
    }
  }
  else {
    while (textIndex <= firstTextIndex + 4) {
      const char* str = uMenuSettingCategories[textIndex].name;
      gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, uMenuFadeTimer);
      usamune_print_text(33, yPos, str);
      if (textIndex == arrowIndex && uMenuShowCursor != 2) {
	usamune_print_text(18, yPos, ">");
      }
      yPos -= 29;
      textIndex++;
    }
  }

  gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, uMenuFadeTimer);
  
  usamune_calc_sub_menu_first_text_index(&firstTextIndex);

  yPos = 157;
  arrowIndex = uMenuCurrCategory->currSetting;
  textIndex = firstTextIndex;
  while (textIndex <= firstTextIndex + 4) {
    uMenuSetting currSetting = uMenuCurrCategory->settingsTable[textIndex];
    const char* str = currSetting.options[*currSetting.currOption];
    s32 xPos = 289;
    if (*str != 0) {
      xPos -= (12 * strlen(str));
    }
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, uMenuFadeTimer);
    usamune_print_text(xPos, yPos, str);
    usamune_print_text(124, yPos, currSetting.name);
    if (uInSubMenu && textIndex == arrowIndex && uMenuShowCursor != 2) {
      usamune_print_text(109, yPos, ">");
    }
    yPos -= 22;
    textIndex++;
  }

  uMenuSetting* currSetting = &uMenuCurrCategory->settingsTable[arrowIndex];

  if (uInSubMenu && (currSetting->render_func != NULL) && (uFlavorTextDisplayed != 2)) {
    u32 rendered = (*currSetting->render_func)(currSetting);
    if (uFlavorTextTimer != 0 && !rendered) {
      //TODO: implement usamune_render_flavor_text
    }
  }
  
  gSPDisplayList(gDisplayListHead++, dl_usamune_text_end);  
}



static void usamune_menu_render_level_select(UNUSED u8 p1, UNUSED u16 p2, UNUSED u8* p3) {
  static u8 lockScroll = 0;
  u8 textStartIndex = 0;
  
  shade_screen();
  gSPDisplayList(gDisplayListHead++, dl_usamune_text_begin);  
  gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, uMenuFadeTimer);

  usamune_print_text(22, 200, "- Cur");
  usamune_print_text_fmt_int(90, 200, "%02x", sWarpDest.levelNum);
  usamune_print_text_fmt_int(120, 200, "%02x", sWarpDest.areaIdx);
  usamune_print_text_fmt_int(150, 200, "%02x", sWarpDest.nodeId);
  usamune_print_text(186, 200, "-");

  if (uCurrLevelSelection < 4) {
    textStartIndex = 0;
    lockScroll = FALSE;
  }
  else if (uCurrLevelSelection > 25) {
    textStartIndex = 20;
    lockScroll = FALSE;
  }
  else {
    if (uMenuShowCursor == 1 && lockScroll) {
      uMenuShowCursor = 2;
    }
    textStartIndex = (uCurrLevelSelection & 0xFE) - 4;
    lockScroll = TRUE;
  }

  u8 textEndIndex = textStartIndex + 11;
  if (textEndIndex > 30) textEndIndex = 30;

  s32 y = 168;
  u8 textIndex = textStartIndex;
  while (textIndex <= textEndIndex) {
    s32 x = 136 * (textIndex & 1);
    usamune_print_text(x + 60, y, uLevelSelectOptions[textIndex].name);
    if (uCurrLevelSelection == textIndex) {
      if (uMenuShowCursor != 2) {
	usamune_print_text(x + 45, y, ">");
      }
      if (uCurrLevelSelection == 30) {
	uWarpDest customDest = uWarpDestTable[uLevelSelectOptions[30].warpID];
	usamune_print_text_fmt_int(180, y, "%02x", customDest.levelNum);
	usamune_print_text_fmt_int(210, y, "%02x", customDest.areaIdx);
	usamune_print_text_fmt_int(240, y, "%02x", customDest.nodeId);
	u8 cursorPos = 246 - (30 * uCustomLevelEditArrowIndex);
	usamune_print_text(cursorPos, y - 13, "^");
      }
    }
    if (!(++textIndex & 1)) {
      y -= 23;
    }
  }

  //TODO: flavor text stuff

  gSPDisplayList(gDisplayListHead++, dl_usamune_text_end);
}

static void usamune_calc_sub_menu_first_text_index(u8* firstTextIndex) {  
  u8 numSettings = uMenuCurrCategory->numSettings;
  u8 currSetting = uMenuCurrCategory->currSetting;

  if (currSetting < 2 || numSettings <= 5) {
    *firstTextIndex = 0;
    uLockSubMenuArrowPos = FALSE;
  }
  else if ((numSettings - 3) < currSetting) {
    *firstTextIndex = numSettings - 5;
    uLockSubMenuArrowPos = FALSE;
  }
  else { //scrolling
    if ((uMenuShowCursor == 1) && uInSubMenu && uLockSubMenuArrowPos) {
      uMenuShowCursor = 2;
    }
    *firstTextIndex = currSetting - 2;
    uLockSubMenuArrowPos = TRUE;
  }
}

void usamune_print_text(s32 x, s32 y, const char* str) {
  if (str && (*str != 0)) {
    int i = 0;      
    while (TRUE) {
      x &= 0x3ff;
      const void* tex = usamune_sprite_lut[(u8)str[i] - 32];
      if (tex) {
	gDPLoadTextureBlock(gDisplayListHead++, tex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 16, 0,
			    G_TX_WRAP | G_TX_NOMIRROR, G_TX_WRAP | G_TX_NOMIRROR, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
	gSPTextureRectangle(gDisplayListHead++, (x + 16) << 2, (240 - y) << 2,
			    x << 2, (224 - y) << 2, G_TX_RENDERTILE, 0, 0, 1 << 10, 1 << 10);
      }
      x += 12;
      if (str[++i] == 0) {
	return;
      }
    }
  }
}

void usamune_print_text_fmt_int(s32 x, s32 y, const char* str, s32 n) {
  char buf[24];
  sprintf(buf, str, n);
  usamune_print_text(x, y, buf);
}

void usamune_render_timer(s32 x, s32 y, u16 time) {
  u16 timerMins;
  u16 timerSecs;
  u16 timerFracSecs;

  timerMins = time / (30 * 60);
  timerSecs = (time - (timerMins * 1800)) / 30;
  timerFracSecs = (10 * ((time - (timerMins * 1800) - (timerSecs * 30)) & 0xFFFF)) / 3;

  usamune_print_text_fmt_int(x, y, "%0d", timerMins);
  usamune_print_text_fmt_int(x + 20, y, "%02d", timerSecs);
  usamune_print_text_fmt_int(x + 54, y, "%02d", timerFracSecs);
  usamune_print_text(x + 10, y + 7, "'");
  usamune_print_text(x + 45, y + 7, "\"");
}

static void usamune_exit_menu_or_reset(u8 reset) {
  uMenuActive = FALSE;
  uInEditMode = FALSE;

  if (!uGlobalSettingsTable[MISC_PRESET]) {
    if (uDoSaveSettings) {
      usamune_menu_save_settings();
      uDoSaveSettings = FALSE;
    }
  }

  reset_action_timers();
  if (reset == 0) {
    gMenuMode = uPrevMenuMode;
    gDialogID = uPrevDialogID;
    if (sCurrPlayMode == PLAY_MODE_FRAME_ADVANCE) {
      gWarpTransition.isActive = uPrevWarpTransitionActive;
      set_play_mode(PLAY_MODE_NORMAL);
      raise_background_noise(1);
      play_sound(SOUND_MENU_PAUSE_2, gGlobalSoundSource);
    }
  }
  else {
    reset_level_vars();
    uPrevWarpTransitionActive = FALSE;
    if (sCurrPlayMode == PLAY_MODE_FRAME_ADVANCE) {
      set_play_mode(PLAY_MODE_NORMAL);
      raise_background_noise(1);
    }
  }
}

static void usamune_print_save_slot_text(void) {
  if (uSaveSlotTextTimer > 0) {
    if (uSaveSlotTextVisible == 0) {
      const char *txt;
      u8 slot;
      if (uSaveSlotText == SS_TEXT_LOAD) {
	txt = "Load";
	slot = uLoadStateSlot + 1;
      }
      else if (uSaveSlotText == SS_TEXT_SAVE) {
	txt = "Save";
	slot = uSaveStateSlot + 1;
      }
      usamune_print_text(uSaveSlotTextPosX, uSaveSlotTextPosY, txt);
      usamune_print_text_fmt_int(uSaveSlotTextPosX + 56, uSaveSlotTextPosY, "%d", slot);
      uSaveSlotTextVisible = 1;
    }
    if (--uSaveSlotTextTimer == 0) {
      uSaveSlotText = SS_TEXT_NONE;
    }
  }
}


