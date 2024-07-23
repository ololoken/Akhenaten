#pragma once

#include "core/tokenum.h"

enum e_building_type {
	BUILDING_NONE = 0,
	BUILDING_RESERVED_1 = 1,
	BUILDING_MENU_FARMS = 2,
	BUILDING_MENU_RAW_MATERIALS = 3,
	BUILDING_MENU_CONSTURCTION_GUILDS = 4,
	BUILDING_ROAD = 5,
	BUILDING_MUD_WALL = 6,
	BUILDING_WATER_LIFT = 7,
	BUILDING_IRRIGATION_DITCH = 8,
	BUILDING_CLEAR_LAND = 9,
	BUILDING_HOUSE_VACANT_LOT = 10,
	BUILDING_HOUSE_CRUDE_HUT = 10,
	BUILDING_HOUSE_STURDY_HUT = 11,
	BUILDING_HOUSE_MEAGER_SHANTY = 12,
	BUILDING_HOUSE_COMMON_SHANTY = 13,
	BUILDING_HOUSE_ROUGH_COTTAGE = 14,
	BUILDING_HOUSE_ORDINARY_COTTAGE = 15,
	BUILDING_HOUSE_MODEST_HOMESTEAD = 16,
	BUILDING_HOUSE_SPACIOUS_HOMESTEAD = 17,
	BUILDING_HOUSE_MODEST_APARTMENT = 18,
	BUILDING_HOUSE_SPACIOUS_APARTMENT = 19,
	BUILDING_HOUSE_COMMON_RESIDENCE = 20,
	BUILDING_HOUSE_SPACIOUS_RESIDENCE = 21,
	BUILDING_HOUSE_ELEGANT_RESIDENCE = 22,
	BUILDING_HOUSE_FANCY_RESIDENCE = 23,
	BUILDING_HOUSE_COMMON_MANOR = 24,
	BUILDING_HOUSE_SPACIOUS_MANOR = 25,
	BUILDING_HOUSE_ELEGANT_MANOR = 26,
	BUILDING_HOUSE_STATELY_MANOR = 27,
	BUILDING_HOUSE_MODEST_ESTATE = 28,
	BUILDING_HOUSE_PALATIAL_ESTATE = 29,
	BUILDING_BANDSTAND = 30,
	BUILDING_BOOTH = 31,
	BUILDING_SENET_HOUSE = 32,
	BUILDING_PAVILLION = 33,
	BUILDING_CONSERVATORY = 34,
	BUILDING_DANCE_SCHOOL = 35,
	BUILDING_JUGGLER_SCHOOL = 36,
	BUILDING_SENET_MASTER = 37,
	BUILDING_PLAZA = 38,
	BUILDING_GARDENS = 39,
	BUILDING_FORT_CHARIOTEERS = 40,
	BUILDING_SMALL_STATUE = 41,
	BUILDING_MEDIUM_STATUE = 42,
	BUILDING_LARGE_STATUE = 43,
	BUILDING_FORT_ARCHERS = 44,
	BUILDING_FORT_INFANTRY = 45,
	BUILDING_APOTHECARY = 46,
	BUILDING_MORTUARY = 47,
	BUILDING_MENU_MONUMENTS = 48,
	BUILDING_DENTIST = 49,
	BUILDING_DISTRIBUTION_CENTER_UNUSED = 50,
	BUILDING_SCRIBAL_SCHOOL = 51,
	BUILDING_MENU_WATER_CROSSINGS = 52,
	BUILDING_LIBRARY = 53,
	BUILDING_FORT_GROUND = 54,
	BUILDING_POLICE_STATION = 55,
	BUILDING_RESERVED_TRIUMPHAL_ARCH_56 = 56,
	BUILDING_MENU_FORTS = 57,
	BUILDING_MUD_GATEHOUSE = 58,
	BUILDING_MUD_TOWER = 59,
	BUILDING_TEMPLE_OSIRIS = 60,
	BUILDING_TEMPLE_RA = 61,
	BUILDING_TEMPLE_PTAH = 62,
	BUILDING_TEMPLE_SETH = 63,
	BUILDING_TEMPLE_BAST = 64,
	BUILDING_TEMPLE_COMPLEX_OSIRIS = 65,
	BUILDING_TEMPLE_COMPLEX_RA = 66,
	BUILDING_TEMPLE_COMPLEX_PTAH = 67,
	BUILDING_TEMPLE_COMPLEX_SETH = 68,
	BUILDING_TEMPLE_COMPLEX_BAST = 69,
	BUILDING_BAZAAR = 70,
	BUILDING_GRANARY = 71,
	BUILDING_STORAGE_YARD = 72,
	BUILDING_STORAGE_ROOM = 73,
	BUILDING_SHIPWRIGHT = 74,
	BUILDING_DOCK = 75,
	BUILDING_FISHING_WHARF = 76,
	BUILDING_PERSONAL_MANSION = 77,
	BUILDING_FAMILY_MANSION = 78,
	BUILDING_DYNASTY_MANSION = 79,
	BUILDING_RESERVER_MISSION_POST_80 = 80,
	BUILDING_ARCHITECT_POST = 81,
	BUILDING_LOW_BRIDGE = 82,
	BUILDING_UNUSED_SHIP_BRIDGE_83 = 83,
	BUILDING_VILLAGE_PALACE = 84,
	BUILDING_TOWN_PALACE = 85,
	BUILDING_TAX_COLLECTOR = 86,
	BUILDING_TAX_COLLECTOR_UPGRADED = 87,
	BUILDING_UNUSED_NATIVE_HUT_88 = 88,
	BUILDING_UNUSED_NATIVE_MEETING_89 = 89,
	BUILDING_RESERVED_90 = 90,
	BUILDING_MENU_BEAUTIFICATION = 91,
	BUILDING_WELL = 92,
	BUILDING_UNUSED_NATIVE_CROPS_93 = 93,
	BUILDING_MILITARY_ACADEMY = 94,
	BUILDING_RECRUITER = 95,
	BUILDING_MENU_TEMPLES = 96,
	BUILDING_MENU_TEMPLE_COMPLEX = 97,
	BUILDING_ORACLE = 98,
	BUILDING_BURNING_RUIN = 99,
	BUILDING_BARLEY_FARM = 100,
	BUILDING_FLAX_FARM = 101,
	BUILDING_GRAIN_FARM = 102,
	BUILDING_LETTUCE_FARM = 103,
	BUILDING_POMEGRANATES_FARM = 104,
	BUILDING_CHICKPEAS_FARM = 105,
	BUILDING_STONE_QUARRY = 106,
	BUILDING_LIMESTONE_QUARRY = 107,
	BUILDING_WOOD_CUTTERS = 108,
	BUILDING_CLAY_PIT = 109,
	BUILDING_BREWERY_WORKSHOP = 110,
	BUILDING_WEAVER_WORKSHOP = 111,
	BUILDING_WEAPONSMITH = 112,
	BUILDING_JEWELS_WORKSHOP = 113,
	BUILDING_POTTERY_WORKSHOP = 114,
	BUILDING_HUNTING_LODGE = 115,
	BUILDING_UNUSED_116 = 116,
	BUILDING_MENU_EDUCATION = 134,
	BUILDING_ACADEMY = 135,
	BUILDING_FERRY = 136,
	BUILDING_UNUSED_137 = 137,
	BUILDING_ROADBLOCK = 138,
	BUILDING_SHRINE_OSIRIS = 140,
	BUILDING_SHRINE_RA = 141,
	BUILDING_SHRINE_PTAH = 142,
	BUILDING_SHRINE_SETH = 143,
	BUILDING_SHRINE_BAST = 144,
	BUILDING_MENU_SHRINES = 150,
	BUILDING_BIG_TEMPLE_OSIRIS = 151,
	BUILDING_BIG_TEMPLE_RA = 152,
	BUILDING_BIG_TEMPLE_PTAH = 153,
	BUILDING_BIG_TEMPLE_SETH = 154,
	BUILDING_BIG_TEMPLE_BAST = 155,
	BUILDING_GOLD_MINE = 161,
	BUILDING_GEMSTONE_MINE = 162,
	BUILDING_ORDINARY_ROCK = 163,
	BUILDING_OREBEARING_ROCK = 164,
	BUILDING_FIREHOUSE = 167,
	BUILDING_BRICK_WALL = 169,
	BUILDING_CLAY_GATEHOUSE = 170,
	BUILDING_BRICK_GATEHOUSE = 171,
	BUILDING_BRICK_TOWER = 172,
	BUILDING_CLAY_TOWER = 173,
	BUILDING_MENU_MUD_STRUCTURES = 174,
	BUILDING_MENU_BRICK_STRUCTURES = 175,
	BUILDING_MENU_DEFENSES = 176,
	BUILDING_CARPENTERS_GUILD = 177,
	BUILDING_BRICKLAYERS_GUILD = 178,
	BUILDING_STONEMASONS_GUILD = 179,
	BUILDING_WATER_SUPPLY = 180,
	BUILDING_TRANSPORT_WHARF = 181,
	BUILDING_WARSHIP_WHARF = 182,
	BUILDING_PYRAMID = 183,
	BUILDING_COURTHOUSE = 184,
	BUILDING_MILITARY_ACADEMY_2 = 185,
	BUILDING_MILITARY_ACADEMY_3 = 186,
	BUILDING_VILLAGE_PALACE_UP = 187,
	BUILDING_TOWN_PALACE_UP = 188,
	BUILDING_CITY_PALACE = 189,
	BUILDING_BAZAAR_UP = 190,
	BUILDING_GRANARY_UP = 191,
	BUILDING_DOCK_UP = 192,
	BUILDING_STORAGE_YARD_UP = 193,
	BUILDING_CATTLE_RANCH = 194,
	BUILDING_REED_GATHERER = 195,
	BUILDING_FIGS_FARM = 196,
	BUILDING_MARSHLAND = 197,
	BUILDING_SAND_DUNES = 198,
	BUILDING_WORK_CAMP = 199,
	BUILDING_MUD_GATEHOUSE_UP = 200,
	BUILDING_BRICK_GATEHOUSE_UP = 201,
	BUILDING_MENU_GATEHOUSE = 202,
	BUILDING_PAPYRUS_WORKSHOP = 203,
	BUILDING_BRICKS_WORKSHOP = 204,
	BUILDING_CHARIOTS_WORKSHOP = 205,
	BUILDING_PHYSICIAN = 206,
	BUILDING_FESTIVAL_SQUARE = 209,
	BUILDING_SPHINX = 210,
	BUILDING_TEMPLE_COMPLEX_ALTAR = 211,
	BUILDING_TEMPLE_COMPLEX_ORACLE = 212,
	BUILDING_RESERVED_213 = 213,
	BUILDING_DISEMBARK_POINT = 214,
	BUILDING_GRANITE_QUARRY = 216,
	BUILDING_COPPER_MINE = 217,
	BUILDING_SANDSTONE_QUARRY = 221,
	BUILDING_MAUSOLEUM = 222,
	BUILDING_CLIFF = 223,
	BUILDING_HENNA_FARM = 224,
	BUILDING_ALEXANDRIA_LIBRARY = 225,
	BUILDING_ZOO = 226,
	BUILDING_CAESAREUM = 227,
	BUILDING_PHAROS_LIGHTHOUSE = 228,
	BUILDING_SMALL_ROYAL_TOMB = 229,
	BUILDING_ABU_SIMBEL = 230,
	BUILDING_ARTISANS_GUILD = 231,
	BUILDING_LAMP_WORKSHOP = 232,
	BUILDING_PAINT_WORKSHOP = 233,
	BUILDING_MEDIUM_ROYAL_TOMB = 234,
	BUILDING_LARGE_ROYAL_TOMB = 235,
	BUILDING_GRAND_ROYAL_TOMB = 236,
	BUILDING_MONUMENT_NONE = 240,
	BUILDING_SMALL_BENT_PYRAMID = 241,
	BUILDING_MEDIUM_BENT_PYRAMID = 242,
	BUILDING_SMALL_MUDBRICK_PYRAMID = 243,
	BUILDING_MEDIUM_MUDBRICK_PYRAMID = 244,
	BUILDING_LARGE_MUDBRICK_PYRAMID = 245,
	BUILDING_MUDBRICK_PYRAMID_COMPLEX = 246,
	BUILDING_GRAND_MUDBRICK_PYRAMID_COMPLEX = 247,
	BUILDING_SMALL_STEPPED_PYRAMID = 248,
	BUILDING_MEDIUM_STEPPED_PYRAMID = 249,
	BUILDING_LARGE_STEPPED_PYRAMID = 250,
	BUILDING_STEPPED_PYRAMID_COMPLEX = 251,
	BUILDING_GRAND_STEPPED_PYRAMID_COMPLEX = 252,
	BUILDING_SMALL_PYRAMID = 253,
	BUILDING_MEDIUM_PYRAMID = 254,
	BUILDING_LARGE_PYRAMID = 255,
	BUILDING_PYRAMID_COMPLEX = 256,
	BUILDING_GRAND_PYRAMID_COMPLEX = 257,
	BUILDING_SMALL_MASTABA = 258,
	BUILDING_MEDIUM_MASTABA = 259,
	BUILDING_LARGE_MASTABA = 260,
	BUILDING_SMALL_OBELISK = 262,
	BUILDING_LARGE_OBELISK = 263,
	BUILDING_SUN_TEMPLE = 264,
	BUILDING_MAUSOLEUM_0 = 265,
	BUILDING_MAUSOLEUM_1 = 266,
	BUILDING_MAUSOLEUM_2 = 266,
	BUILDING_PHAROS_LIGHTHOUSE_0 = 267,
	BUILDING_ALEXANDRIAS_LIBRARY = 268,
	BUILDING_RESERVED_269 = 269,
	BUILDING_COLOSSI = 270,
	BUILDING_TEMPLE_OF_LUXOR = 271,
	BUILDING_SMALL_ROYAL_BURIAL_TOMB = 272,
	BUILDING_MEDIUM_ROYAL_BURIAL_TOMB = 273,
	BUILDING_LARGE_ROYAL_BURIAL_TOMB = 274,
	BUILDING_GRAND_ROYAL_BURIAL_TOMB = 275,
	BUILDING_RESERVED_276 = 276,
	BUILDING_SMALL_MASTABA_SIDE = 277,
	BUILDING_SMALL_MASTABA_WALL = 278,
	BUILDING_SMALL_MASTABA_ENTRANCE = 279,
	BUILDING_MENU_VACANT_HOUSE = 280,
	BUILDING_MENU_CLEAR_LAND = 281,
	BUILDING_MENU_ROAD = 282,
	BUILDING_MENU_HEALTH = 284,
	BUILDING_MENU_RELIGION = 285,
	BUILDING_MENU_ENTERTAINMENT = 286,
	BUILDING_MENU_ADMINISTRATION = 287,
	BUILDING_MENU_SECURITY = 288,
	BUILDING_MENU_INDUSTRY = 289,
	BUILDING_MENU_GUILDS = 290,
	BUILDING_MENU_LARGE_TEMPLES = 291,
	BUILDING_MENU_FOOD = 292,
	BUILDING_MENU_DISTRIBUTION = 293,
	BUILDING_MENU_DEFENCES = 294,

	BUILDING_MAX = 295,
};
extern const token_holder<e_building_type, BUILDING_NONE, BUILDING_MAX> e_building_type_tokens;

enum e_house_level {
    HOUSE_CRUDE_HUT = 0,
    HOUSE_STURDY_HUT = 1,
    HOUSE_MEAGER_SHANTY = 2,
    HOUSE_COMMON_SHANTY = 3,
    HOUSE_ROUGH_COTTAGE = 4,
    HOUSE_ORDINARY_COTTAGE = 5,
    HOUSE_MODEST_HOMESTEAD = 6,
    HOUSE_SPACIOUS_HOMESTEAD = 7,
    HOUSE_MODEST_APARTMENT = 8,
    HOUSE_SPACIOUS_APARTMENT = 9,
    HOUSE_COMMON_RESIDENCE = 10,
    HOUSE_SPACIOUS_RESIDENCE = 11,
    HOUSE_ELEGANT_RESIDENCE = 12,
    HOUSE_FANCY_RESIDENCE = 13,
    HOUSE_COMMON_MANOR = 14,
    HOUSE_SPACIOUS_MANOR = 15,
    HOUSE_ELEGANT_MANOR = 16,
    HOUSE_STATELY_MANOR = 17,
    HOUSE_MODEST_ESTATE = 18,
    HOUSE_PALATIAL_ESTATE = 19,
};