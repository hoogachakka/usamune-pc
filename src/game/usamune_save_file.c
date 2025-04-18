#include "usamune.h"
#include "usamune_settings.h"
#include "save_file.h"

#define SHIFTL(setting, shift, bits) _SHIFTL((uGlobalSettingsTable[setting]), shift, bits)
#define LOAD_SETTING(byte, setting, shift, bits)			\
  uGlobalSettingsTable[setting] = (gSaveBuffer.menuData[0].usamuneSaveData[byte] >> shift) & ((1 << bits) - 1)

extern struct SaveBuffer gSaveBuffer;

extern void save_main_menu_data(void);

static void usamune_menu_load_settings(void);
static void usamune_load_save_file_4(void);

void usamune_save_file_load_all(void) {
  save_file_load_all();
  usamune_menu_load_settings();
  usamune_load_save_file_4();
}

static void usamune_menu_load_settings(void) {
  LOAD_SETTING(0, MISCT_MISCTMR, 7, 1); //apply
  LOAD_SETTING(0, MISCT_NUMDISP, 6, 1);
  LOAD_SETTING(0, MISCT_XCAM, 5, 1);
  LOAD_SETTING(0, MISCT_BSWITCH, 4, 1);
  LOAD_SETTING(0, MISCT_PSWITCH, 3, 1);
  LOAD_SETTING(0, MISCT_RIDE, 2, 1);
  LOAD_SETTING(0, MISCT_CHEST, 1, 1);
  LOAD_SETTING(0, MISCT_CRYSTAL, 0, 1);
      
  LOAD_SETTING(1, MISCT_ENEMY, 7, 1);
  LOAD_SETTING(1, MISCT_EXCLBOX, 6, 1);
  LOAD_SETTING(1, MISCT_CORKBOX, 5, 1);
  LOAD_SETTING(1, MISCT_OBJGRB, 4, 1);
  LOAD_SETTING(1, MISCT_LDGEGRB, 3, 1);
  LOAD_SETTING(1, MISCT_POLEGRB, 2, 1);
  LOAD_SETTING(1, MISCT_WK, 1, 1);
  LOAD_SETTING(1, MISCT_WALLHIT, 0, 1);

  LOAD_SETTING(2, MISCT_CLIP, 7, 1);
  LOAD_SETTING(2, MISCT_SURFACE, 6, 1);
  LOAD_SETTING(2, MISCT_FLIP, 5, 1);
  LOAD_SETTING(2, MISCT_ROLLOUT, 4, 1);
  LOAD_SETTING(2, MISCT_DIVE, 3, 1);
  LOAD_SETTING(2, MISCT_LJSLDK, 2, 1);
  LOAD_SETTING(2, MISCT_TJ, 1, 1);
  LOAD_SETTING(2, MISCT_LOAD, 0, 1);

  LOAD_SETTING(3, MISCT_BOWSER, 7, 1);
  LOAD_SETTING(3, MISCT_COIN, 6, 1);
  LOAD_SETTING(3, MISCT_1UP, 5, 1);
  LOAD_SETTING(3, TIMER_STOP, 3, 2);
  LOAD_SETTING(3, TIMER_DISPLAY, 1, 2); //apply
  LOAD_SETTING(3, TIMER_FADETMR, 0, 1);

  LOAD_SETTING(4, TIMER_SECTMR, 7, 1); //apply
  LOAD_SETTING(4, TIMER_PSSRACE, 5, 2); //apply
  LOAD_SETTING(4, TIMER_REALTMR, 3, 2);
  LOAD_SETTING(4, TIMER_RTATMR, 1, 2); //apply
  LOAD_SETTING(4, 0x1F, 0, 1);

  LOAD_SETTING(5, TIMER_TMRPOSI, 7, 1); //apply
  LOAD_SETTING(5, RESET_LBUTTON, 5, 2);
  LOAD_SETTING(5, RESET_LxR, 3, 2);
  LOAD_SETTING(5, RESET_INITBGM, 2, 1);
  LOAD_SETTING(5, RESET_SSTATES, 1, 1);
  LOAD_SETTING(5, 0x1A, 0, 1);
  
  LOAD_SETTING(6, HUD_LIFE, 5, 3);
  LOAD_SETTING(6, HUD_STAR, 2, 3);
  LOAD_SETTING(6, 0x4B, 1, 1);
  LOAD_SETTING(6, MISC_MODE, 0, 1);

  LOAD_SETTING(7, HUD_ATTEMPT, 6, 2); //apply
  LOAD_SETTING(7, 0x46, 5, 1);
  LOAD_SETTING(7, HUD_DUST, 4, 1);
  LOAD_SETTING(7, HUD_INPUT, 2, 2);
  LOAD_SETTING(7, 0x52, 1, 1);
  LOAD_SETTING(7, HUD_WKDISP, 0, 1); //apply

  LOAD_SETTING(8, STAGE_TTC, 5, 3);
  LOAD_SETTING(8, STAGE_WDW, 3, 2);
  LOAD_SETTING(8, STAGE_DDDSUB, 1, 2); //apply
  LOAD_SETTING(8, STAGE_CCMPENG, 0, 1);

  LOAD_SETTING(9, STAGE_MIPS, 6, 2);
  LOAD_SETTING(9, STAGE_TOAD, 5, 1);
  LOAD_SETTING(9, STAGE_SWITCH, 4, 1);
  LOAD_SETTING(9, MISC_MUSIC, 3, 1); //apply
  LOAD_SETTING(9, MISC_STARCOLOR, 2, 1); //apply

  //TODO: call all apply functions
}

void usamune_menu_save_settings(void) {
  gSaveBuffer.menuData[0].usamuneSaveData[0] =
    ( SHIFTL(MISCT_MISCTMR, 7, 1) |
      SHIFTL(MISCT_NUMDISP, 6, 1) |
      SHIFTL(MISCT_XCAM, 5, 1) |
      SHIFTL(MISCT_BSWITCH, 4, 1) |
      SHIFTL(MISCT_PSWITCH, 3, 1) |
      SHIFTL(MISCT_RIDE, 2, 1) |
      SHIFTL(MISCT_CHEST, 1, 1) |
      SHIFTL(MISCT_CRYSTAL, 0, 1) );

  gSaveBuffer.menuData[0].usamuneSaveData[1] =
    ( SHIFTL(MISCT_ENEMY, 7, 1) |
      SHIFTL(MISCT_EXCLBOX, 6, 1) |
      SHIFTL(MISCT_CORKBOX, 5, 1) |
      SHIFTL(MISCT_OBJGRB, 4, 1) |
      SHIFTL(MISCT_LDGEGRB, 3, 1) |
      SHIFTL(MISCT_POLEGRB, 2, 1) |
      SHIFTL(MISCT_WK, 1, 1) |
      SHIFTL(MISCT_WALLHIT, 0, 1) );

  gSaveBuffer.menuData[0].usamuneSaveData[2] =
    ( SHIFTL(MISCT_CLIP, 7, 1) |
      SHIFTL(MISCT_SURFACE, 6, 1) |
      SHIFTL(MISCT_FLIP, 5, 1) |
      SHIFTL(MISCT_ROLLOUT, 4, 1) |
      SHIFTL(MISCT_DIVE, 3, 1) |
      SHIFTL(MISCT_LJSLDK, 2, 1) |
      SHIFTL(MISCT_TJ, 1, 1) |
      SHIFTL(MISCT_LOAD, 0, 1) );

  gSaveBuffer.menuData[0].usamuneSaveData[3] =
    ( SHIFTL(MISCT_BOWSER, 7, 1) |
      SHIFTL(MISCT_COIN, 6, 1) |
      SHIFTL(MISCT_1UP, 5, 1) |
      SHIFTL(TIMER_STOP, 3, 2) |
      SHIFTL(TIMER_DISPLAY, 1, 2) |
      SHIFTL(TIMER_FADETMR, 0, 1) );

  gSaveBuffer.menuData[0].usamuneSaveData[4] =
    ( SHIFTL(TIMER_SECTMR, 7, 1) |
      SHIFTL(TIMER_PSSRACE, 5, 2) |
      SHIFTL(TIMER_REALTMR, 3, 2) |
      SHIFTL(TIMER_RTATMR, 1, 2) |
      SHIFTL(0x1F, 0, 1) );

  gSaveBuffer.menuData[0].usamuneSaveData[5] =
    ( SHIFTL(TIMER_TMRPOSI, 7, 1) |
      SHIFTL(RESET_LBUTTON, 5, 2) |
      SHIFTL(RESET_LxR, 3, 2) |
      SHIFTL(RESET_INITBGM, 2, 1) |
      SHIFTL(RESET_SSTATES, 1, 1) |
      SHIFTL(0x1A, 0, 1) );
  
  gSaveBuffer.menuData[0].usamuneSaveData[6] =
    ( SHIFTL(HUD_LIFE, 5, 3) | 
      SHIFTL(HUD_STAR, 2, 3) |
      SHIFTL(0x4B, 1, 1) |
      SHIFTL(MISC_MODE, 0, 1) );

  gSaveBuffer.menuData[0].usamuneSaveData[7] =
    ( SHIFTL(HUD_ATTEMPT, 6, 2) |
      SHIFTL(0x46, 5, 1) |
      SHIFTL(HUD_DUST, 4, 1) |
      SHIFTL(HUD_INPUT, 2, 2) |
      SHIFTL(0x52, 1, 1) |
      SHIFTL(HUD_WKDISP, 0, 1) );

  gSaveBuffer.menuData[0].usamuneSaveData[8] =
    ( SHIFTL(STAGE_TTC, 5, 3) |
      SHIFTL(STAGE_WDW, 3, 2) |
      SHIFTL(STAGE_DDDSUB, 1, 2) |
      SHIFTL(STAGE_CCMPENG, 0, 1) );

  gSaveBuffer.menuData[0].usamuneSaveData[9] =
    ( SHIFTL(STAGE_MIPS, 6, 2) |
      SHIFTL(STAGE_TOAD, 5, 1) |
      SHIFTL(STAGE_SWITCH, 4, 1) |
      SHIFTL(MISC_MUSIC, 3, 1) |
      SHIFTL(MISC_STARCOLOR, 2, 1) |
      (gSaveBuffer.menuData[0].usamuneSaveData[9] & 3) );

  gMainMenuDataModified = TRUE;
  save_main_menu_data();
}

static void usamune_load_save_file_4(void) {
  gSaveBuffer.files[3][0].capLevel = 0;
  gSaveBuffer.files[3][0].capArea = 0;
  gSaveBuffer.files[3][0].capPos[0] = 0;
  gSaveBuffer.files[3][0].capPos[1] = 0;
  gSaveBuffer.files[3][0].capPos[2] = 0;
  gSaveBuffer.files[3][0].flags =
    ( SAVE_FLAG_FILE_EXISTS |
      SAVE_FLAG_HAVE_WING_CAP |
      SAVE_FLAG_HAVE_VANISH_CAP |
      SAVE_FLAG_UNLOCKED_BASEMENT_DOOR |
      SAVE_FLAG_UNLOCKED_UPSTAIRS_DOOR |
      SAVE_FLAG_DDD_MOVED_BACK |
      SAVE_FLAG_MOAT_DRAINED |
      SAVE_FLAG_UNLOCKED_PSS_DOOR |
      SAVE_FLAG_UNLOCKED_WF_DOOR |
      SAVE_FLAG_UNLOCKED_CCM_DOOR |
      SAVE_FLAG_UNLOCKED_JRB_DOOR |
      SAVE_FLAG_UNLOCKED_BITDW_DOOR |
      SAVE_FLAG_UNLOCKED_BITFS_DOOR |
      SAVE_FLAG_UNLOCKED_50_STAR_DOOR |
      SAVE_FLAG_COLLECTED_TOAD_STAR_1 |
      SAVE_FLAG_COLLECTED_TOAD_STAR_2 |
      SAVE_FLAG_COLLECTED_TOAD_STAR_3 |
      SAVE_FLAG_COLLECTED_MIPS_STAR_1 |
      SAVE_FLAG_COLLECTED_MIPS_STAR_2 );

  gSaveBuffer.files[3][0].courseStars[0] = 0x7F;
  gSaveBuffer.files[3][0].courseStars[1] = 0xFF;
  gSaveBuffer.files[3][0].courseStars[2] = 0x7F;
  gSaveBuffer.files[3][0].courseStars[3] = 0xFF;
  gSaveBuffer.files[3][0].courseStars[4] = 0x7F;
  gSaveBuffer.files[3][0].courseStars[5] = 0x7F;
  gSaveBuffer.files[3][0].courseStars[6] = 0x7F;
  gSaveBuffer.files[3][0].courseStars[7] = 0x7F;
  gSaveBuffer.files[3][0].courseStars[8] = 0x7F;
  gSaveBuffer.files[3][0].courseStars[9] = 0x7F;
  gSaveBuffer.files[3][0].courseStars[10] = 0x7F;
  gSaveBuffer.files[3][0].courseStars[11] = 0x7F;
  gSaveBuffer.files[3][0].courseStars[12] = 0x7F;
  gSaveBuffer.files[3][0].courseStars[13] = 0x7F;
  gSaveBuffer.files[3][0].courseStars[14] = 0x7F;
  gSaveBuffer.files[3][0].courseStars[15] = 0x81;
  gSaveBuffer.files[3][0].courseStars[16] = 0x01;
  gSaveBuffer.files[3][0].courseStars[17] = 0x01;
  gSaveBuffer.files[3][0].courseStars[18] = 0x03;
  gSaveBuffer.files[3][0].courseStars[19] = 0x01;
  gSaveBuffer.files[3][0].courseStars[20] = 0x01;
  gSaveBuffer.files[3][0].courseStars[21] = 0x01;
  gSaveBuffer.files[3][0].courseStars[22] = 0x01;
  gSaveBuffer.files[3][0].courseStars[23] = 0x81;
  gSaveBuffer.files[3][0].courseStars[24] = 0x00;
  
  gSaveBuffer.files[3][0].courseCoinScores[0] = 0x92;
  gSaveBuffer.files[3][0].courseCoinScores[1] = 0x8D;
  gSaveBuffer.files[3][0].courseCoinScores[2] = 0x68;
  gSaveBuffer.files[3][0].courseCoinScores[3] = 0x9A;
  gSaveBuffer.files[3][0].courseCoinScores[4] = 0x97;
  gSaveBuffer.files[3][0].courseCoinScores[5] = 0x8B;
  gSaveBuffer.files[3][0].courseCoinScores[6] = 0x85;
  gSaveBuffer.files[3][0].courseCoinScores[7] = 0x86;
  gSaveBuffer.files[3][0].courseCoinScores[8] = 0x6A;
  gSaveBuffer.files[3][0].courseCoinScores[9] = 0x7F;
  gSaveBuffer.files[3][0].courseCoinScores[10] = 0x98;
  gSaveBuffer.files[3][0].courseCoinScores[11] = 0x89;
  gSaveBuffer.files[3][0].courseCoinScores[12] = 0xC0;
  gSaveBuffer.files[3][0].courseCoinScores[13] = 0x80;
  gSaveBuffer.files[3][0].courseCoinScores[14] = 0x92;
}
