#pragma once

enum e_empire_object {
    EMPIRE_OBJECT_ORNAMENT = 0,
    EMPIRE_OBJECT_CITY = 1,
    EMPIRE_OBJECT_TEXT = 2,
    EMPIRE_OBJECT_BATTLE_ICON = 3,
    EMPIRE_OBJECT_LAND_TRADE_ROUTE = 4,
    EMPIRE_OBJECT_SEA_TRADE_ROUTE = 5,
    EMPIRE_OBJECT_ROMAN_ARMY = 6,
    EMPIRE_OBJECT_ENEMY_ARMY = 7,
};

enum e_empire_city {
    EMPIRE_CITY_OURS = 0,             //                EMPIRE_CITY_DISTANT_ROMAN = 0
    EMPIRE_CITY_PHARAOH_TRADING = 1,  //     EMPIRE_CITY_OURS = 1
    EMPIRE_CITY_PHARAOH = 2,          //             EMPIRE_CITY_TRADE = 2
    EMPIRE_CITY_EGYPTIAN_TRADING = 3, //    EMPIRE_CITY_FUTURE_TRADE = 3
    EMPIRE_CITY_EGYPTIAN = 4,         //            EMPIRE_CITY_DISTANT_FOREIGN = 4
    EMPIRE_CITY_FOREIGN_TRADING = 5,  //     EMPIRE_CITY_VULNERABLE = 5
    EMPIRE_CITY_FOREIGN = 6           //              EMPIRE_CITY_FUTURE_ROMAN = 6,
};

bool empire_city_type_can_trade(int type);