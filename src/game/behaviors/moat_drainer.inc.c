// moat_drainer.c.inc

extern u32 usamune_moat_get_save_flags();

void bhv_invisible_objects_under_bridge_init(void) {
    if (usamune_moat_get_save_flags() & SAVE_FLAG_MOAT_DRAINED) {
        gEnvironmentRegions[6] = -800;
        gEnvironmentRegions[12] = -800;
    }
}
