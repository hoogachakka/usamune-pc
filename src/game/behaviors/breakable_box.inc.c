// breakable_box.c.inc

#include "game/usamune_settings.h"
#include "sm64.h"

extern void usamune_trigger_misc_timer(u8, u8);

void bhv_breakable_box_loop(void) {
  obj_set_hitbox(o, &sBreakableBoxHitbox);
  cur_obj_set_model(MODEL_BREAKABLE_BOX_SMALL);
  if (o->oTimer == 0)
    breakable_box_init();
  if (cur_obj_was_attacked_or_ground_pounded() != 0) {
    usamune_trigger_misc_timer(MISCT_CORKBOX, 17);
    obj_explode_and_spawn_coins(46.0f, 1);
    create_sound_spawner(SOUND_GENERAL_BREAK_BOX);
  }
}
