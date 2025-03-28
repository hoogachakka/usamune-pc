#include "usamune.h"
//#include "usamune_settings.h"
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

u8 uMenuActive = 0;
u8 uSaveStatesEnabled;

s8 uSaveStateSlot;
s8 uLoadStateSlot;
u8 uSaveSlotText;
u8 uSaveSlotTextTimer;
u32 DAT_80417c48;
u32 DAT_80417c4c;
u32 DAT_80417c50;
u32 DAT_80417c54;
u32 DAT_80417c58;
u32 DAT_80417c5c;
u32 DAT_80417c60;
u32 DAT_80416920;
u32 DAT_80416922;
u32 DAT_80416924;
u32 DAT_804168e0;
u16 uMenuFadeTimer;
u8 uMenuNextSound;
u8 uInSubMenu;
u8 uInEditMode;
u32 u32_804168e0;
u8 uLockMenuArrowPos;
u8 uLockSubMenuArrowPos;
u8 uLockScroll;
u32 uFlavorTextDisplayed;
u8 uFlavorTextTimer;

u8 DAT_80417c6a = 0;
u8 DAT_80417c6b = 0;
u8 DAT_80417c6c = 0;

u8 DAT_80417bd0 = 0;
u8 DAT_80417bd2 = 0;

u8 DAT_80417bca = 0;
u8 DAT_80417bcb = 0;
u8 DAT_80417bcc = 0;
u8 DAT_80417bcd = 0;
u8 DAT_80417bce = 0;
u8 DAT_80417bcf = 0;



extern uMenuSettingCategory uMenuSettingCategories[];
uMenuSettingCategory* uMenuCurrCategory = &uMenuSettingCategories[0];

extern s16 gWarpTransDelay;
extern u8 sTransitionColorFadeCount[];
extern u8 sTransitionTextureFadeCount[];
extern void set_play_mode(s16 playMode);
extern void shade_screen(void);

extern s16 gMenuMode;
extern s16 gDialogID;

actionFunc (*uMenuActionTable)[16];

static void usamune_menu_act_close_menu(u8, u16, u8*);
static void usamune_menu_act_move_cursor(u8, u16, u8*);
static void usamune_menu_act_modify_settings(u8, u16, u8*);
static void usamune_menu_render_options(u8, u16, u8*);
void usamune_menu_change_setting(uMenuSetting* setting, u8 param_2);
static void FUN_80410d70(void);
static void FUN_80409630(u32 param_1);
static void usamune_exit_menu_or_reset(u8 reset);
static void usamune_calc_sub_menu_first_text_index(u8* firstTextIndex);
static void reset_action_timers(void);

u8 uMenuActionTimers[16];

actionFunc uMenuActionsPage1[] = {
  NULL,
  usamune_menu_act_close_menu,
  NULL,
  NULL,
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

void usamune_main(void) {
  if (uMenuActive) {
    usamune_menu_main(gPlayer1Controller);
  }
  /* else if (gPlayer1Controller->buttonPressed & (D_JPAD | L_JPAD | R_JPAD | L_TRIG | R_TRIG)) { */
  /*   //cycle through save slots */
  /*   if (uSaveStatesEnabled) { */
  /*     if (gPlayer1Controller->buttonPressed == R_JPAD) { */
  /* 	if (uSaveSlotText == TEXT_SAVE) { */
  /* 	  if (gPlayer1Controller->buttonDown & Z_TRIG) { */
  /* 	    uSaveStateSlot += 8; */
  /* 	  } */
  /* 	  else { */
  /* 	    uSaveStateSlot += 1; */
  /* 	  } */
  /* 	  uSaveStateSlot -= 9 * (uSaveStateSlot / 9); */
  /* 	} */
  /* 	else { */
  /* 	  uSaveSlotText = TEXT_SAVE; */
  /* 	} */
  /* 	uSaveSlotTextTimer = 30; */
  /*     } */
  /*     else if (gPlayer1Controller->buttonPressed == L_JPAD) { */
  /* 	if (uSaveSlotText == TEXT_LOAD) { */
  /* 	  if (gPlayer1Controller->buttonDown & Z_TRIG) { */
  /* 	    uLoadStateSlot += 8; */
  /* 	  } */
  /* 	  else { */
  /* 	    uLoadStateSlot += 1; */
  /* 	  } */
  /* 	  uLoadStateSlot -= 9 * (uLoadStateSlot / 9); */
  /* 	} */
  /* 	else { */
  /* 	  uSaveSlotText = TEXT_SAVE; */
  /* 	} */
  /* 	uSaveSlotTextTimer = 30; */
  /*     } */
  /*   } */
   
    /* else if (gPlayer1Controller->buttonPressed < 0x101) { */
    /*   if ((gPlayer1Controller->buttonPressed == L_TRIG) || */
    /* 	  ((gPlayer1Controller->buttonPressed & 0xffdf) == 0x10)) { */
    /* 	if ((gPlayer1Controller->buttonDown & 0x30) == 0x30) { */
    /* 	  if ((DAT_80417b65 != '\x02') && (DAT_80417b65 != '\x03')) { */
    /* 	    if (DAT_80417b65 == '\x01') { */
    /* 	      FUN_8040a518(); */
    /* 	      goto LAB_8040d6fc; */
    /* 	    } */
    /* 	    goto LAB_8040dc2c; */
    /* 	  } */
    /* 	} */
    /* 	else { */
    /* 	LAB_8040dc2c: */
    /* 	  if (((gPlayer1Controller->buttonDown & L_TRIG) == 0) || */
    /* 	      (((DAT_80417b4c != '\x02' && (DAT_80417b4c != '\x03')) && (DAT_80417b4c != '\x01')))) */
    /* 	    goto LAB_8040d6fc; */
    /* 	} */
    /* 	FUN_8040a518(); */
    /*   } */
    /* } */
    else if (gPlayer1Controller->buttonPressed & D_JPAD) {
      usamune_load_menu();
    }
  // }
 /* LAB_8040d6fc: */
 /*  if ((DAT_80417c43 == '\0') || (DAT_80417c43 = DAT_80417c43 + -1, DAT_80417c43 != '\0')) { */
 /*    iVar5 = (DAT_80417bdc + -2) - DAT_80417be0; */
 /*  } */
 /*  else { */
 /*    iVar5 = (DAT_80417bdc + -2) - DAT_80417be0; */
 /*    DAT_80417c3c = DAT_80417be4; */
 /*    DAT_80417c38 = gGlobalTimer; */
 /*  } */
 /*  DAT_80417be4 = iVar5 + DAT_80417be4; */
 /*  DAT_80417be0 = DAT_80417bdc; */
 /*  if (DAT_80417c60._0_1_ != '\0') { */
 /*    uVar1 = (ulonglong)DAT_80417be4; */
 /*    DAT_80417c60._0_1_ = DAT_80417c60._0_1_ + -1; */
 /*    uVar6 = iVar5 + DAT_80417c54; */
 /*    if (DAT_80417b6d == '\0') { */
 /*      if (0x57a37 < uVar6) { */
 /*        DAT_80417c48._3_1_ = 0; */
 /*        DAT_80417c48._0_2_ = 0x633b; */
 /*        DAT_80417c48._2_1_ = 0x62; */
 /*        DAT_80417c54 = 0x57a37; */
 /*        goto LAB_8040d738; */
 /*      } */
 /*      FUN_8040008c((int)((ulonglong)uVar6 * 25000 >> 0x20),(int)((ulonglong)uVar6 * 25000),0,0x3a6d); */
 /*    } */
 /*    else { */
 /*      if (359999 < uVar6) { */
 /*        DAT_80417c48._3_1_ = 0; */
 /*        DAT_80417c48._0_2_ = 0x633b; */
 /*        DAT_80417c48._2_1_ = 0x62; */
 /*        DAT_80417c54 = 359999; */
 /*        goto LAB_8040d738; */
 /*      } */
 /*      uVar2 = uVar6 * 0x19 >> 0x1e | */
 /* 	((uint)(uVar6 * 0x19 < uVar6 * 0x18) + */
 /* 	 (uVar6 * 3 >> 0x1d | ((uint)(uVar6 * 3 < uVar6 * 2) - ((int)uVar6 >> 0x1f)) * 8)) * 4; */
 /*      uVar3 = uVar6 * 100; */
 /*      uVar4 = uVar3 - ((uVar3 & 0xfffffff) + (uVar2 << 4 | uVar3 >> 0x1c)) % 0xf; */
 /*      uVar1 = (ulonglong) */
 /* 	(int)(((uVar2 - (uVar3 < uVar4)) * -0x11111111 + uVar4 * -0x11111112 + */
 /* 	       (int)((ulonglong)uVar4 * 0xeeeeeeef >> 0x20)) * 0x40000000 | */
 /* 	      (uint)((ulonglong)uVar4 * 0xeeeeeeef) >> 2); */
 /*    } */
 /*    uVar2 = (int)uVar1 + (int)((uVar1 & 0xffffffff) / 6000) * -6000; */
 /*    DAT_80417c48._2_1_ = (undefined1)(uVar2 % 100); */
 /*    DAT_80417c48._0_2_ = */
 /*      (ushort)(((uint)((uVar1 & 0xffffffff) / 6000) & 0xff) << 8) | (ushort)(uVar2 / 100) & 0xff; */
 /*    DAT_80417c54 = uVar6; */
 /*  } */

  //timer stuff (presumeably)
 /* LAB_8040d738: */
 /*  if (DAT_80417c42 != '\0') { */
 /*    DAT_80417c42 = DAT_80417c42 + -1; */
 /*    DAT_80417c34 = iVar5 + DAT_80417c34; */
 /*    if (DAT_80417b6d == '\0') { */
 /*      uVar6 = DAT_80417c34 * 0x3e9; */
 /*      uVar4 = (uint)(uVar6 < DAT_80417c34 * 1000) + */
 /* 	(DAT_80417c34 * 0x7d >> 0x1d | */
 /* 	 ((uint)(DAT_80417c34 * 0x7d < DAT_80417c34 * 0x7c) + */
 /* 	  (DAT_80417c34 * 0x1f >> 0x1e | */
 /* 	   ((DAT_80417c34 >> 0x1b) - (uint)(DAT_80417c34 * 0x20 < DAT_80417c34 * 0x1f)) * 4)) * 8 */
 /* 	 ); */
 /*      uVar2 = uVar6 - ((uVar6 & 0xfffff) + (uVar4 * 0x1000 & 0xfffff | uVar6 >> 0x14) + */
 /*                       (uVar4 >> 8 & 0xfffff) + (uVar4 >> 0x1c)) % 0x4b; */
 /*      uVar6 = ((uVar4 - (uVar6 < uVar2)) * -0x69d0369d + uVar2 * 0x2fc962fc + */
 /* 	       (int)((ulonglong)uVar2 * 0x962fc963 >> 0x20)) * 0x20000000 | */
 /* 	(uint)((ulonglong)uVar2 * 0x962fc963) >> 3; */
 /*    } */
 /*    else { */
 /*      uVar6 = DAT_80417c34 * 0x19 >> 0x1e | */
 /* 	((uint)(DAT_80417c34 * 0x19 < DAT_80417c34 * 0x18) + */
 /* 	 (DAT_80417c34 * 3 >> 0x1d | */
 /* 	  ((uint)(DAT_80417c34 * 3 < DAT_80417c34 * 2) - ((int)DAT_80417c34 >> 0x1f)) * 8)) * 4; */
 /*      uVar4 = DAT_80417c34 * 100; */
 /*      uVar2 = uVar4 - ((uVar4 & 0xfffffff) + (uVar6 << 4 | uVar4 >> 0x1c)) % 0xf; */
 /*      uVar6 = ((uVar6 - (uVar4 < uVar2)) * -0x11111111 + uVar2 * -0x11111112 + */
 /* 	       (int)((ulonglong)uVar2 * 0xeeeeeeef >> 0x20)) * 0x40000000 | */
 /* 	(uint)((ulonglong)uVar2 * 0xeeeeeeef) >> 2; */
 /*    } */
 /*    uVar2 = (uVar6 % 6000) / 100; */
 /*    DAT_80417c30 = (ushort)((uVar6 / 6000 & 0xff) << 8) | (ushort)uVar2; */
 /*    DAT_80417c32 = (char)(uVar6 % 6000) + (char)uVar2 * -100; */
 /*  } */
}

s16 usamune_render_menus_and_dialogs(void) {
  s16 ret = render_menus_and_dialogs();
  if (uMenuActive) {
    if (gWarpTransition.isActive && !gWarpTransDelay) {
      gWarpTransition.isActive = 0;
      gWarpTransDelay = 0;
      sTransitionColorFadeCount[0] = 0;
      sTransitionTextureFadeCount[0] = 0;
      sTransitionTextureFadeCount[1] = 0;
      DAT_80416924 = 0;
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
    FUN_80410d70();
  }
  return ret;
}

void usamune_menu_main(struct Controller *controller) {
  u32_804168e0 = 0;
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
  
  if (uMenuNextSound & 1) {
    play_sound(SOUND_MENU_MARIO_CASTLE_WARP, gGlobalSoundSource);
  }
  else if (uMenuNextSound & 2) {
    play_sound(SOUND_MENU_MARIO_CASTLE_WARP2, gGlobalSoundSource);
  }
  else if (uMenuNextSound & 4) {
    play_sound(SOUND_MENU_CLICK_CHANGE_VIEW, gGlobalSoundSource);
  }
  else if (uMenuNextSound & 8) {
    play_sound(SOUND_MENU_MESSAGE_NEXT_PAGE, gGlobalSoundSource);
  }
  else if (uMenuNextSound & 0x10) {
    play_sound(SOUND_MENU_CAMERA_BUZZ, gGlobalSoundSource);
  }
  uMenuNextSound = 0;
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
  DAT_80416920 = gMenuMode;
  DAT_80416922 = gDialogID;
  DAT_80416924 = gWarpTransition.isActive;
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

/* void usamune_print_save_slot_text(void) { */
/*   if (uSaveSlotTextTimer == 0) { */
/*     return; */
/*   } */
/* } */

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
      u32_804168e0 = 1;
    } 
    uMenuNextSound |= (1 << 3);
    uMenuActionTimers[5] = 0; //Z_TRIG
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

  if (setting->apply_func != NULL) {
    (*setting->apply_func)(setting, prevOption, gCurrSaveFileNum);
  }

  if (prevOption == nextOption) {
    uMenuNextSound |= (1 << 4);
  }
  else {
    uMenuNextSound |= (1 << 3);
  }
}
    
static void reset_action_timers(void) {
  for (int i = 0; i < 16; i++) {
    uMenuActionTimers[i] = 0;
  }
}

  
  

/* static void usamune_menu_act_switch_page(u8 state, u16 input, u8* timer) { */
/*   if (state == MENU_ACTION_PREPARE) { */
    

/*   } */
/* } */

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
    if ((u32_804168e0 == 1) && !uInSubMenu && uLockMenuArrowPos) {
      u32_804168e0 = 2;
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
      if (textIndex == arrowIndex && u32_804168e0 != 2) {
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
    if (uInSubMenu && textIndex == arrowIndex && u32_804168e0 != 2) {
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
    if ((u32_804168e0 == 1) && uInSubMenu && uLockSubMenuArrowPos) {
      u32_804168e0 = 2;
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
      void* tex = usamune_sprite_lut[(u8)str[i] - 32];
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
  char buf[20];
  sprintf(buf, str, n);
  usamune_print_text(x, y, buf);
}

void usamune_render_hud(void) {
  if (!uMenuActive) {
    render_hud();
  }
}

static void FUN_80410d70(void) {
  if (!gHudDisplay.flags) {
    return;
  }
  
}

static void usamune_exit_menu_or_reset(u8 reset) {
  uMenuActive = FALSE;

  for (int i = 0; i < 16; i++) {
    uMenuActionTimers[i] = 0;
  }

  if (reset == 0) {
    gMenuMode = DAT_80416920;
    gDialogID = DAT_80416922;
    if (sCurrPlayMode == PLAY_MODE_FRAME_ADVANCE) {
      gWarpTransition.isActive = DAT_80416924;
      set_play_mode(PLAY_MODE_NORMAL);
      raise_background_noise(1);
      play_sound(SOUND_MENU_PAUSE_2, gGlobalSoundSource);
      return;
    }
  }
}


