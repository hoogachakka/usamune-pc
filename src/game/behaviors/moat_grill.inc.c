// moat_grill.c.inc

extern u32 usamune_moat_get_save_flags();

void bhv_moat_grills_loop(void) {
    if (usamune_moat_get_save_flags() & SAVE_FLAG_MOAT_DRAINED)
        cur_obj_set_model(MODEL_NONE);
    else
        load_object_collision_model();
}
