#pragma once

enum e_figure_action {
    FIGURE_ACTION_1_IMMIGRANT_CREATED = 1,
    FIGURE_ACTION_2_IMMIGRANT_ARRIVING = 2,
    FIGURE_ACTION_3_IMMIGRANT_ENTERING_HOUSE = 3,
    FIGURE_ACTION_4_EMIGRANT_CREATED = 4,
    FIGURE_ACTION_5_EMIGRANT_EXITING_HOUSE = 5,
    FIGURE_ACTION_6_EMIGRANT_LEAVING = 6,
    FIGURE_ACTION_6_HOMELESS_LEAVING = 6,
    FIGURE_ACTION_7_HOMELESS_CREATED = 7,
    FIGURE_ACTION_8_HOMELESS_GOING_TO_HOUSE = 8,
    FIGURE_ACTION_9_HOMELESS_ENTERING_HOUSE = 9,
    FIGURE_ACTION_10_HOMELESS_ENTERING_HOUSE = 10,
    FIGURE_ACTION_10_FESTIVAL_PRIEST_CREATED = 10,
    FIGURE_ACTION_10_WORKER_CREATED = 10,
    FIGURE_ACTION_10_WORKER_GOING = 10,
    FIGURE_ACTION_11_WORKER_GOING_TO_PLACE = 11,
    FIGURE_ACTION_12_WORKER_LEVELING_GROUND = 12,
    FIGURE_ACTION_10_BRIRKLAYER_CREATED = 10,
    FIGURE_ACTION_11_BRIRKLAYER_GOING = 11,
    FIGURE_ACTION_12_BRICKLAYER_GOING_TO_PLACE = 12,
    FIGURE_ACTION_13_BRICKLAYER_WAITING_RESOURCES = 13,
    FIGURE_ACTION_14_BRICKLAYER_LAY_BRICKS = 14,
    FIGURE_ACTION_15_BRICKLAYER_LOOKING_FOR_IDLE_TILE = 15,
    FIGURE_ACTION_16_BRICKLAYER_RETURN_HOME = 16,
    FIGURE_ACTION_17_BRICKLAYER_EXIT_FROM_MONUMENT = 17,
    FIGURE_ACTION_18_BRICKLAYER_RANDOM_TILE = 18,
    FIGURE_ACTION_14_CARTPUSHER_AT_PALACE = 14,
    FIGURE_ACTION_20_BRIRKLAYER_DESTROY = 20,
    FIGURE_ACTION_20_CARTPUSHER_INITIAL = 20,
    FIGURE_ACTION_21_CARTPUSHER_DELIVERING_TO_WAREHOUSE = 21,
    FIGURE_ACTION_22_CARTPUSHER_DELIVERING_TO_GRANARY = 22,
    FIGURE_ACTION_23_CARTPUSHER_DELIVERING_TO_WORKSHOP = 23,
    FIGURE_ACTION_24_CARTPUSHER_AT_WAREHOUSE = 24,
    FIGURE_ACTION_25_CARTPUSHER_AT_GRANARY = 25,
    FIGURE_ACTION_26_CARTPUSHER_AT_WORKSHOP = 26,
    FIGURE_ACTION_27_CARTPUSHER_RETURNING = 27,
    FIGURE_ACTION_40_TAX_COLLECTOR_CREATED = 40,
    FIGURE_ACTION_41_TAX_COLLECTOR_ENTERING_EXITING = 41,
    FIGURE_ACTION_42_TAX_COLLECTOR_ROAMING = 42,
    FIGURE_ACTION_43_TAX_COLLECTOR_RETURNING = 43,
    FIGURE_ACTION_50_SLED_CREATED = 50,
    FIGURE_ACTION_51_SLED_DELIVERING_RESOURCE = 51,
    FIGURE_ACTION_52_SLED_AT_DELIVERY_BUILDING = 52,
    FIGURE_ACTION_50_SLED_PULLER_CREATED = 50,
    FIGURE_ACTION_51_SLED_PULLER_DELIVERING_RESOURCE = 51,
    FIGURE_ACTION_52_SLED_PULLER_AT_DELIVERY_BUILDING = 52,
    FIGURE_ACTION_53_SLED_PULLER_DESTROY = 53,
    FIGURE_ACTION_54_SLED_PULLER_WAITING_FOR_DESTROY = 54,
    FIGURE_ACTION_50_WAREHOUSEMAN_CREATED = 50,
    FIGURE_ACTION_51_WAREHOUSEMAN_DELIVERING_RESOURCE = 51,
    FIGURE_ACTION_52_WAREHOUSEMAN_AT_DELIVERY_BUILDING = 52,
    FIGURE_ACTION_53_WAREHOUSEMAN_RETURNING_EMPTY = 53,
    FIGURE_ACTION_54_WAREHOUSEMAN_GETTING_FOOD = 54,
    FIGURE_ACTION_55_WAREHOUSEMAN_AT_GRANARY_GETTING = 55,
    FIGURE_ACTION_56_WAREHOUSEMAN_RETURNING_WITH_FOOD = 56,
    FIGURE_ACTION_57_WAREHOUSEMAN_GETTING_RESOURCE = 57,
    FIGURE_ACTION_58_WAREHOUSEMAN_AT_WAREHOUSE_GETTING_GOODS = 58,
    FIGURE_ACTION_59_WAREHOUSEMAN_RETURNING_WITH_RESOURCE = 59,
    FIGURE_ACTION_60_ENGINEER_CREATED = 60,
    FIGURE_ACTION_61_ENGINEER_ENTERING_EXITING = 61,
    FIGURE_ACTION_62_ENGINEER_ROAMING = 62,
    FIGURE_ACTION_63_ENGINEER_RETURNING = 63,
    FIGURE_ACTION_60_HERBALIST_CREATED = 60,
    FIGURE_ACTION_61_HERBALIST_ENTERING_EXITING = 61,
    FIGURE_ACTION_62_HERBALIST_ROAMING = 62,
    FIGURE_ACTION_63_HERBALIST_RETURNING = 63,
    FIGURE_ACTION_60_PHYSICIAN_CREATED = 60,
    FIGURE_ACTION_61_PHYSICIAN_ENTERING_EXITING = 61,
    FIGURE_ACTION_62_PHYSICIAN_ROAMING = 62,
    FIGURE_ACTION_63_PHYSICIAN_RETURNING = 63,
    FIGURE_ACTION_70_FIREMAN_CREATED = 70,
    FIGURE_ACTION_71_FIREMAN_ENTERING_EXITING = 71,
    FIGURE_ACTION_72_FIREMAN_ROAMING = 72,
    FIGURE_ACTION_73_FIREMAN_RETURNING = 73,
    FIGURE_ACTION_74_FIREMAN_GOING_TO_FIRE = 74,
    FIGURE_ACTION_75_FIREMAN_AT_FIRE = 75,
    FIGURE_ACTION_76_FIREMAN_GOING_TO_ENEMY = 76,
    FIGURE_ACTION_77_FIREMAN_AT_ENEMY = 77,
    FIGURE_ACTION_70_POLICEMAN_CREATED = 70,
    FIGURE_ACTION_71_POLICEMAN_ENTERING_EXITING = 71,
    FIGURE_ACTION_72_POLICEMAN_ROAMING = 72,
    FIGURE_ACTION_73_POLICEMAN_RETURNING = 73,
    FIGURE_ACTION_76_POLICEMAN_GOING_TO_ENEMY = 76,
    FIGURE_ACTION_77_POLICEMAN_AT_ENEMY = 77,
    FIGURE_ACTION_80_SOLDIER_AT_REST = 80,
    FIGURE_ACTION_81_SOLDIER_GOING_TO_FORT = 81,
    FIGURE_ACTION_82_SOLDIER_RETURNING_TO_BARRACKS = 82,
    FIGURE_ACTION_83_SOLDIER_GOING_TO_STANDARD = 83,
    FIGURE_ACTION_84_SOLDIER_AT_STANDARD = 84,
    FIGURE_ACTION_85_SOLDIER_GOING_TO_MILITARY_ACADEMY = 85,
    FIGURE_ACTION_86_SOLDIER_MOPPING_UP = 86,
    FIGURE_ACTION_87_SOLDIER_GOING_TO_DISTANT_BATTLE = 87,
    FIGURE_ACTION_88_SOLDIER_RETURNING_FROM_DISTANT_BATTLE = 88,
    FIGURE_ACTION_89_SOLDIER_AT_DISTANT_BATTLE = 89,
    FIGURE_ACTION_90_ENTERTAINER_AT_SCHOOL_CREATED = 90,
    FIGURE_ACTION_91_ENTERTAINER_EXITING_SCHOOL = 91,
    FIGURE_ACTION_92_ENTERTAINER_GOING_TO_VENUE = 92,
    FIGURE_ACTION_93_ENTERTAINER_GOING_TO_RANDOM_ROAD = 93,
    FIGURE_ACTION_94_ENTERTAINER_ROAMING = 94,
    FIGURE_ACTION_95_ENTERTAINER_RETURNING = 95,
    FIGURE_ACTION_100_TRADE_CARAVAN_CREATED = 100,
    FIGURE_ACTION_101_TRADE_CARAVAN_ARRIVING = 101,
    FIGURE_ACTION_102_TRADE_CARAVAN_TRADING = 102,
    FIGURE_ACTION_103_TRADE_CARAVAN_LEAVING = 103,
    FIGURE_ACTION_110_TRADE_SHIP_CREATED = 110,
    FIGURE_ACTION_111_TRADE_SHIP_GOING_TO_DOCK = 111,
    FIGURE_ACTION_112_TRADE_SHIP_MOORED = 112,
    FIGURE_ACTION_113_TRADE_SHIP_GOING_TO_DOCK_QUEUE = 113,
    FIGURE_ACTION_114_TRADE_SHIP_ANCHORED = 114,
    FIGURE_ACTION_115_TRADE_SHIP_LEAVING = 115,
    FIGURE_ACTION_120_GOVERNOR_CREATED = 120,
    FIGURE_ACTION_121_GOVERNOR_MOVING = 121,
    FIGURE_ACTION_120_RIOTER_CREATED = 120,
    FIGURE_ACTION_120_MUGGER_CREATED = 120,
    FIGURE_ACTION_121_MUGGER_MOVING = 121,
    FIGURE_ACTION_122_MUGGER_HIDE = 122,
    FIGURE_ACTION_123_MUGGER_LEAVING = 123,
    FIGURE_ACTION_121_RIOTER_MOVING = 121,
    FIGURE_ACTION_125_ROAMING = 125,
    FIGURE_ACTION_126_ROAMER_RETURNING = 126,
    FIGURE_ACTION_128_FLOTSAM_CREATED = 128,
    FIGURE_ACTION_129_FLOTSAM_FLOATING = 129,
    FIGURE_ACTION_130_FLOTSAM_OFF_MAP = 130,
    FIGURE_ACTION_132_DOCKER_IDLING = 132,
    FIGURE_ACTION_133_DOCKER_IMPORT_QUEUE = 133,
    FIGURE_ACTION_134_DOCKER_EXPORT_QUEUE = 134,
    FIGURE_ACTION_135_DOCKER_IMPORT_GOING_TO_WAREHOUSE = 135,
    FIGURE_ACTION_136_DOCKER_EXPORT_GOING_TO_WAREHOUSE = 136,
    FIGURE_ACTION_137_DOCKER_EXPORT_RETURNING = 137,
    FIGURE_ACTION_138_DOCKER_IMPORT_RETURNING = 138,
    FIGURE_ACTION_139_DOCKER_IMPORT_AT_WAREHOUSE = 139,
    FIGURE_ACTION_140_DOCKER_EXPORT_AT_WAREHOUSE = 140,
    FIGURE_ACTION_144_MARKET_BUYER_CREATE = 144,
    FIGURE_ACTION_145_MARKET_BUYER_GOING_TO_STORAGE = 145,
    FIGURE_ACTION_146_MARKET_BUYER_RETURNING = 146,
    FIGURE_ACTION_147_MARKET_BUYER_REROUTING = 147,
    FIGURE_ACTION_148_FLEEING = 148,
    FIGURE_ACTION_149_CORPSE = 149,
    FIGURE_ACTION_150_ATTACK = 150,
    FIGURE_ACTION_150_OSTRICH_EATING = 150,
    FIGURE_ACTION_151_ENEMY_INITIAL = 151,
    FIGURE_ACTION_152_ENEMY_WAITING = 152,
    FIGURE_ACTION_153_ENEMY_MARCHING = 153,
    FIGURE_ACTION_154_ENEMY_FIGHTING = 154,
    FIGURE_ACTION_156_NATIVE_GOING_TO_MEETING_CENTER = 156,
    FIGURE_ACTION_157_NATIVE_RETURNING_FROM_MEETING = 157,
    FIGURE_ACTION_158_NATIVE_CREATED = 158,
    FIGURE_ACTION_159_NATIVE_ATTACKING = 159,
    FIGURE_ACTION_160_NATIVE_TRADER_GOING_TO_WAREHOUSE = 160,
    FIGURE_ACTION_161_NATIVE_TRADER_RETURNING = 161,
    FIGURE_ACTION_162_NATIVE_TRADER_CREATED = 162,
    FIGURE_ACTION_163_NATIVE_TRADER_AT_WAREHOUSE = 163,
    FIGURE_ACTION_170_TOWER_SENTRY_AT_REST = 170,
    FIGURE_ACTION_171_TOWER_SENTRY_PATROLLING = 171,
    FIGURE_ACTION_172_TOWER_SENTRY_FIRING = 172,
    FIGURE_ACTION_173_TOWER_SENTRY_RETURNING = 173,
    FIGURE_ACTION_174_TOWER_SENTRY_GOING_TO_TOWER = 174,
    FIGURE_ACTION_180_BALLISTA_CREATED = 180,
    FIGURE_ACTION_181_BALLISTA_FIRING = 181,
    FIGURE_ACTION_190_FISHING_BOAT_CREATED = 190,
    FIGURE_ACTION_191_FISHING_BOAT_GOING_TO_FISH = 191,
    FIGURE_ACTION_192_FISHING_BOAT_FISHING = 192,
    FIGURE_ACTION_193_FISHING_BOAT_GOING_TO_WHARF = 193,
    FIGURE_ACTION_194_FISHING_BOAT_AT_WHARF = 194,
    FIGURE_ACTION_195_FISHING_BOAT_RETURNING_WITH_FISH = 195,
    FIGURE_ACTION_196_HERD_ANIMAL_AT_REST = 196,
    FIGURE_ACTION_196_FISHPOINT_BUBLES = 196,
    FIGURE_ACTION_197_HERD_ANIMAL_MOVING = 197,
    FIGURE_ACTION_197_FISHPOINT_JUMP = 197,
    FIGURE_ACTION_199_WOLF_ATTACKING = 199,
    FIGURE_ACTION_200_HIPPODROME_HORSE_CREATED = 200,
    FIGURE_ACTION_201_HIPPODROME_HORSE_RACING = 201,
    FIGURE_ACTION_202_HIPPODROME_HORSE_DONE = 202,

    FIGURE_ACTION_10_HIPPO_MOVING = 10,
    FIGURE_ACTION_19_HIPPO_IDLE = 19,
    FIGURE_ACTION_24_HIPPO_CREATED = 24,

    // pharaoh
    ACTION_1_ROAMING = 1,
    ACTION_2_ROAMERS_RETURNING = 2,
    //    ACTION_3,
    //    ACTION_4,
    //    ACTION_5,
    //    ACTION_6,
    //    ACTION_7,
    ACTION_8_RECALCULATE = 8,
    ACTION_9_CHASE_PREY = 9,
    ACTION_9_REED_GATHERER_GOTO_RESOURCE = 9,
    ACTION_10_REED_GATHERER_WORK = 10,
    ACTION_11_REED_GATHERER_RETURN_HOME = 11,
    ACTION_9_DELIVERING_GOODS = 9,
    ACTION_10_DELIVERING_FOOD = 10,
    ACTION_10_GOING = 10,
    ACTION_10_PICKUP_ANIMAL = 10,
    ACTION_15_HUNTER_PACK = 10,
    ACTION_11_GOING_TO_PICKUP_POINT = 11,
    ACTION_11_RETURNING_EMPTY = 11,
    ACTION_11_DELIVERING_GOLD = 11,
    ACTION_11_HUNTER_WALK = 11,
    ACTION_11_RETURNING_FROM_PATROL = 11,
    ACTION_12_GOING_HOME_AND_UNLOAD = 12,
    ACTION_12_UNLOADING1 = 12,
    ACTION_15_HUNTER_MOVE_PACKED = 12,
    ACTION_13_UNLOADING2 = 13,
    ACTION_13_WAIT_FOR_ACTION = 13,
    ACTION_13_RETURNING_TO_VENUE = 13,
    ACTION_14_RETURNING_WITH_FOOD = 14,
    ACTION_14_UNLOADING_GOLD = 14,
    ACTION_14_GATHERER_CREATED = 14,
    ACTION_15_RETURNING2 = 15,
    ACTION_15_ANIMAL_TERRIFIED = 15,
    ACTION_15_GETTING_GOODS = 15,
    ACTION_15_HUNTER_HUNT = 15,
    ACTION_16_HUNTER_INVESTIGATE = 16,
    ACTION_16_EMIGRANT_RANDOM = 16,
    ACTION_16_HOMELESS_RANDOM = 16,
    ACTION_16_FLEEING = 16,
    //    ACTION_17,
    ACTION_18_ROOSTING = 18,
    FIGURE_ACTION_19_ANIMAL_IDLE = 19,
    //    ACTION_20,
    //    ACTION_21,
    //    ACTION_22,
    //    ACTION_23,
    //    ACTION_24_SPAWNING = 24,
    ACTION_24_ANIMAL_SPAWNED = 24,
    //    ACTION_25,
    //    ACTION_26,
    //    ACTION_27,
    //    ACTION_28,
    //    ACTION_29,
    //    ACTION_30,

    //    ACTION_NONE = -1,
    FIGURE_ACTION_MAX
};

void figure_action_handle(void);
