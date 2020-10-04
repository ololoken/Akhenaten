#include "properties.h"
#include "core/game_environment.h"

static int BUILD_MAX[2] = {
        140,
        300 // ????? temp...
};

static building_properties properties[2][400] = {
        { // SZ FIRE GRP OFF
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {1, 0, 112, 0},
                {1, 0, 24, 26},
                {1, 0, 0,  0},
                {1, 0, 19, 2},
                {0, 0, 0, 0},
                {1, 0, 0, 0},
                {1, 0, 0, 0},
                {1, 0, 0, 0},
                {1, 0, 0, 0},
                {1, 0, 0, 0},
                {1, 0, 0, 0},
                {1, 0, 0, 0},
                {1, 0, 0, 0},
                {1, 0, 0, 0},
                {1, 0, 0, 0},
                {2, 0, 0, 0},
                {2, 0, 0, 0},
                {2, 0, 0, 0},
                {2, 0, 0, 0},
                {3, 0, 0, 0},
                {3, 0, 0, 0},
                {3, 0, 0, 0},
                {3, 0, 0, 0},
                {4, 0, 0, 0},
                {4, 0, 0, 0},
                {3, 0, 45, 0},
                {2, 0, 46, 0},
                {5, 0, 213, 0},
                {5, 0, 48, 0},
                {3, 0, 49, 0},
                {3, 0, 50, 0},
                {3, 0, 51, 0},
                {3, 0, 52, 0},
                {1, 1, 58, 0},
                {1, 1, 59, 0},
                {3, 1, 66, 0},
                {1, 1, 61, 0},
                {2, 1, 61,             1},
                {3, 1, 61,             2},
                {3, 1, 66, 0},
                {3, 1, 66, 0},
                {1, 0, 68, 0},
                {3, 0, 70, 0},
                {2, 0, 185, 0},
                {1, 0, 67, 0},
                {3, 0, 66, 0},
                {2, 0, 41, 0},
                {3, 0, 43, 0},
                {2, 0, 42, 0},
                {4, 1, 66, 1},
                {1, 0, 64, 0},
                {3, 1, 205, 0},
                {3, 1, 66, 0},
                {2, 1, 17, 1},
                {2, 1, 17, 0},
                {2, 0, 71, 0},
                {2, 0, 72, 0},
                {2, 0, 73, 0},
                {2, 0, 74, 0},
                {2, 0, 75, 0},
                {3,  0, 71, 1},
                {3,  0, 72, 1},
                {3,  0, 73, 1},
                {3,  0, 74, 1},
                {3,  0, 75, 1},
                {2, 0, 22, 0},
                {3, 0, 99, 0},
                {1, 1, 82, 0},
                {1, 1, 82, 0},
                {2, 0, 77, 0},
                {3, 0, 78, 0},
                {2, 0, 79, 0},
                {3, 0, 85, 0},
                {4, 0, 86, 0},
                {5, 0, 87, 0},
                {2, 1, 184, 0},
                {1, 1, 81, 0},
                {1, 1, 0, 0},
                {1, 1, 0, 0},
                {0, 0, 0, 0},
                {5, 0, 62, 0},
                {2, 0, 63, 0},
                {0, 0, 0,  0},
                {1, 1, 183, 0},
                {2, 1, 183, 2},
                {3, 1, 25, 0},
                {1, 1, 54, 0},
                {1, 1, 23, 0},
                {1, 1, 100, 0},
                {3, 0, 201, 0},
                {3, 0, 166, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {2, 0, 76, 0},
                {1, 1, 0, 0},
                {3, 0, 37, 0},
                {3, 0, 37, 0},
                {3, 0, 37, 0},
                {3, 0, 37, 0},
                {3, 0, 37, 0},
                {3, 0, 37, 0},
                {2, 0, 38, 0},
                {2, 0, 39, 0},
                {2, 0, 65, 0},
                {2, 0, 40, 0},
                {2, 0, 44, 0},
                {2, 0, 122, 0},
                {2, 0, 123, 0},
                {2, 0, 124, 0},
                {2, 0, 125, 0},
                {0, 0, 0,                0},
                {1, 1, 0, 0},
                {1, 1, 0, 0},
                {1, 1, 0, 0},
                {1, 1, 0, 0},
                {1, 1, 0, 0},
                {1, 1, 0, 0},
                {1, 1, 0, 0},
                {1, 1, 0, 0},
                {1, 1, 0, 0},
                {1, 1, 0, 0},
                {1, 1, 0, 0},
                {1, 1, 0, 0},
                {1, 1, 0, 0},
                {2, 1, 216, 0},
                {1, 1, 0, 0},
                {1, 1, 0, 0},
                {1, 1, 0, 0},
                {1, 1, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0,  0},
                {0, 0, 0, 0},
                {0, 0, 0,               0},
                {0, 0, 0, 0}
        },
        { // PHARAOH
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {1, 0, 112, 0}, // road
                {1, 0, 24, 26}, // wall (unused)
                {2, 1, 25, 0}, // ??????? water lift ???????
                {1, 0, 19, 2},
                {0, 0, 0, 0}, // clear land
                {1, 0, 0, 0},
                {1, 0, 0, 0},
                {1, 0, 0, 0},
                {1, 0, 0, 0},
                {1, 0, 0, 0},
                {1, 0, 0, 0},
                {1, 0, 0, 0},
                {1, 0, 0, 0},
                {1, 0, 0, 0},
                {1, 0, 0, 0},
                {2, 0, 0, 0},
                {2, 0, 0, 0},
                {2, 0, 0, 0},
                {2, 0, 0, 0},
                {3, 0, 0, 0},
                {3, 0, 0, 0},
                {3, 0, 0, 0},
                {3, 0, 0, 0},
                {4, 0, 0, 0},
                {4, 0, 0, 0}, // houses ^^^^
                {3, 0, 45, 0},
                {2, 0, 46, 0},
                {5, 0, 213, 0}, // senet
                {4, 0, 48, 0},
                {3, 0, 49, 0},
                {4, 0, 50, 0},
                {2, 0, 51, 0}, // juggler
                {3, 0, 52, 0}, // unused (senet master)
                {1, 1, 58, 0}, // plaza
                {1, 1, 59, 0}, // gardens
                {3, 1, 66, 0}, // charioteers
                {1, 1, 61, 0},
                {2, 1, 8 + 66 + 99999, 0},
                {3, 1, 7 + 66 + 99999, 0}, // statues ^^^
                {3, 1, 66, 0}, // archers
                {3, 1, 66, 0}, // infantry
                {1, 0, 68, 0}, // apothecary
                {2, 0, 70, 0}, // mortuary
                {2, 0, 185, 0},
                {1, 0, 67, 0}, // dentist
                {3, 0, 66, 0}, // unused (distribution center)
                {2, 0, 41, 0}, // school
                {3, 0, 43, 0}, // water crossings
                {3, 0, 42, 0}, // library
                {4, 1, 66, 1}, // fort yard
                {1, 0, 64, 0}, // police
                {3, 1, 205, 0},
                {3, 1, 66, 0}, // fort (main)
                {2, 1, 17, 1},
                {2, 1, 17, 0},
                {3, 0, 71, 0},
                {3, 0, 72, 0},
                {3, 0, 73, 0},
                {3, 0, 74, 0},
                {3, 0, 75, 0}, // temples ^^^^
                {-1, 0, 71, 1},
                {-1, 0, 72, 1},
                {-1, 0, 73, 1},
                {-1, 0, 74, 1},
                {-1, 0, 75, 1}, // temple complex ^^^^
                {2, 0, 22, 0}, // bazaar
                {4, 0, 99, 0}, // granary
                {1, 1, 82, 0}, // storageyard (hut)
                {1, 1, 82, 0}, // storageyard (space tile)
                {3, 0, 77, 0}, // shipwright
                {3, 0, 78, 0}, // dock
                {2, 0, 79, 0}, // wharf (fish)
                {3, 0, 85, 0},
                {4, 0, 86, 0},
                {5, 0, 87, 0}, // mansions ^^^
                {2, 1, 184, 0},
                {1, 1, 81, 0}, // architects
                {1, 1, 0, 0}, // bridge
                {1, 1, 0, 0},
                {0, 0, 0, 0}, // senate (unused?)
                {5, 0, 62, 0}, // senate2 (unused?)
                {2, 0, 63, 0},
                {2, 0, 63, 0}, // tax collectors ^^
                {1, 1, 183, 0},
                {2, 1, 183, 2},
                {2, 1, 25, 0}, // water lift
                {1, 1, 54, 0},
                {1, 1, 23, 0}, // well
                {1, 1, 100, 0},
                {4, 0, 201, 0}, // academy
                {3, 0, 166, 0}, // recruiter
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {2, 0, 76, 0},
                {1, 1, 0, 0}, // burning ruin
                {3, 0, 37, 0},
                {3, 0, 37, 0},
                {3, 0, 37, 0},
                {3, 0, 37, 0},
                {3, 0, 37, 0},
                {3, 0, 37, 0}, // farms ^^^
                {2, 0, 38, 0}, // stone
                {2, 0, 39, 0}, // limestone
                {2, 0, 65, 0}, // wood
                {2, 0, 40, 0}, // clay
                {2, 0, 44, 0}, // beer
                {2, 0, 122, 0}, // linen
                {2, 0, 123, 0}, // weapons
                {2, 0, 124, 0}, // jewels
                {2, 0, 125, 0}, // pottery

                {2, 0, 176 + 66 + 99999, 0}, // hunters
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0,   0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {4, 0, 0, 0},
                {2, 1, 23, 0}, // ferry
                {2, 0, 0, 0},
                {1, 1, 98 + 66 + 99999, 0}, // roadblock
                {0, 0, 0, 0},
                {1, 0, 75 + 66 + 99999, 0},
                {1, 0, 74 + 66 + 99999, 0},
                {1, 0, 73 + 66 + 99999, 0},
                {1, 0, 72 + 66 + 99999, 0},
                {1, 0, 71 + 66 + 99999, 0}, // shrines ^^^^
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {1, 1, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {2, 0, 185 + 66 + 99999, 0}, // gold
                {2, 0, 188 + 66 + 99999, 0}, // gemstone
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {1, 0, 78 + 66 + 99999, 0}, // firehouse
                {0, 0, 0, 0},
                {1, 1, 138 + 66 + 99999, 0}, // wall
                {0, 0, 0, 0},
                {1, 1, 161 + 66 + 99999, 0}, // gatehouse
                {0, 0, 0, 0},
                {2, 1, 135 + 66 + 99999, 0}, // tower
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {1, 1, 0, 0},
                {2, 0, 91 + 66 + 99999, 0},
                {2, 0, 88 + 66 + 99999, 0},
                {2, 0, 57 + 66 + 99999, 0}, // guilds ^^^
                {2, 1, 69 + 66 + 99999, 0}, // water supply
                {2, 1, 17, 0}, // wharf (transport)
                {3, 1, 28, 0}, // wharf (warship)
                {-1, 1, 0, 0}, // pyramid
                {3, 0, 62 + 66 + 99999, 0}, // courthouse
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {4, 0, 47 + 66 + 99999, 0},
                {5, 0, 39 + 66 + 99999, 0},
                {6, 0, 18 + 66 + 99999, 0}, // town palace ^^^
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {4, 0, 105 + 66 + 99999, 0}, // ranch
                {2, 0, 24 + 66 + 99999, 0}, // reeds
                {3, 1, 115 + 66 + 99999, 0}, // figs farm
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {2, 0, 77 + 66 + 99999, 0}, // work camp
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {1, 1, 220 + 66 + 99999, 0}, // gatehouse (2)
                {2, 0, 44 + 66 + 99999, 0}, // papyrus
                {2, 0, 124 + 66 + 99999, 0}, // bricks
                {4, 0, 52, 0}, // chariots
                {2, 0, 70 + 66 + 99999, 0}, // physician
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {5, 1, 49 + 66 + 99999, 0}, // festival square
                {-1, 1, 0, 0}, // sphynx
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {2, 0, 38 + 66 + 99999, 0}, // granite
                {2, 0, 196 + 66 + 99999, 0}, // copper
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {0, 0, 0, 0},
                {2, 0, 197 + 66 + 99999, 0}, // sandstone
                {-1, 1, 0, 0}, // mausoleum
                {0, 0, 0, 0},
                {3, 0, 0, 0}, // henna
                {-1, 1, 0, 0}, // alexandria library
        }
};


building_properties ROADBLOCK_PROPERTIES = {1, 1, 10000, 0};

const building_properties dummy_property = {0, 0, 0, 0};

const building_properties *building_properties_for_type(int type) {
    // Roadblock properties, as plaza
    if (type == 115 && GAME_ENV == ENGINE_ENV_C3)
        return &properties[GAME_ENV][38];
    if (type >= BUILD_MAX[GAME_ENV])
        return &dummy_property;
    auto p = &properties[GAME_ENV][type];
    return p;
}

