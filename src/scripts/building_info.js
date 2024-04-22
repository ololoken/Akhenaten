log_info("akhenaten: building info started")

building_info = [
  // { type: "academy", help_id: 69, text_id: 86},
  { type: "library", help_id: 70, text_id: 87},
  { type: "marble_quarry", help_id:95, text_id:118},
  { help_id:93, type:"limestone_quarry", text_id:119},
  { help_id:94, type:"timber_yard", text_id:120},
  { help_id:92, type:"clay_pit", text_id:121},
  { help_id:92, type:"reed_farm", text_id:116},
  { help_id:92, type:"sandstone_quarry", text_id:194},
  { help_id:92, type:"granite_quarry", text_id:192},
  { help_id:92, type:"plainstone_quarry", text_id:118},
  { help_id:89, type:"barley_farm", text_id:181},
  { help_id:90, type:"flax_farm", text_id:115},
  { help_id:90, type:"grain_farm", text_id:112},
  { help_id:91, type:"lettuce_farm", text_id:113},
  { help_id:91, type:"pomegranades_farm", text_id:114},
  { help_id:90, type:"chickpeas_farm", text_id:182},
  { help_id:90, type:"figs_farm", text_id:183},
  { help_id:90, type:"henna_farm", text_id:306},
  { help_id:92, type:"bricklayers_guild", text_id:172},
]

building_small_statue = {
  variants : [
    {pack: PACK_GENERAL, id: 61, offset:1},
    {pack: PACK_GENERAL, id: 61, offset:5},
    {pack: PACK_EXPANSION, id: 37, offset:1},
    {pack: PACK_EXPANSION, id: 37, offset:5},
    {pack: PACK_TEMPLE_RA, id: 1, offset:27},
  ],
  window_info_height_id : 1,
}

building_medium_statue = {
  variants : [
    {pack: PACK_GENERAL, id: 8, offset:1},
    {pack: PACK_GENERAL, id: 8, offset:5},
    {pack: PACK_EXPANSION, id: 36, offset:1},
    {pack: PACK_EXPANSION, id: 36, offset:5},
  ],
  window_info_height_id : 1,
}

building_large_statue = {
  variants : [
    {pack: PACK_GENERAL, id: 7, offset:1},
    {pack: PACK_GENERAL, id: 7, offset:5},
    {pack: PACK_EXPANSION, id: 35, offset:1},
    {pack: PACK_EXPANSION, id: 35, offset:5},
  ],
  window_info_height_id : 1,
}

building_fort = {
  ghost : {
    main : [[-55, -35], [-55, -35], [-55, -35], [-60, -40]],
    ground : [[35, 35], [5, -70], [-200, -55], [-180, 46]],
    ground_check : [[3, -1], [4, -1], [4, 0], [3, 0],
                    [-1,-4], [0, -4], [0,-3], [-1,-3],
                    [-4, 0], [-3, 0], [-3,1], [-4, 1],
                    [0,  3], [1,  3], [1, 4], [0,  4]],
  },
  labor_category : LABOR_CATEGORY_MILITARY,
}

building_cattle_ranch = {
  animations : {
    preview : { pos : [0, 0], pack:PACK_GENERAL, id:105, },
    base : { pos : [0, 0], pack:PACK_GENERAL, id:105, offset:0 },
    work : { pos : [0, ], pack:PACK_GENERAL, id:105, offset:1, max_frames:12 }
  },
  labor_category : LABOR_CATEGORY_FOOD_PRODUCTION,
}

building_granary = {
  animations : {
    preview : { pack:PACK_GENERAL, id:99 },
    base : { pack:PACK_GENERAL, id:99 },
    work : { pack:PACK_SPR_AMBIENT, id:47, max_frames:23 },
    resources : {pack:PACK_GENERAL, id:99, offset:2}
  },
  labor_category : LABOR_CATEGORY_INFRASTRUCTURE,
  window_info_height_id : 1,
}

building_shrine_osiris = {
  animations : {
    preview : { pos : [0, 0], pack:PACK_GENERAL, id:75, },
    base : { pos : [0, 0], pack:PACK_GENERAL, id:75, offset:0 },
  },
  labor_category : LABOR_CATEGORY_RELIGION,
  window_info_height_id : 4,
}

building_shrine_ra = {
  animations : {
    preview : { pos : [0, 0], pack:PACK_GENERAL, id:74, },
    base : { pos : [0, 0], pack:PACK_GENERAL, id:74, offset:0 },
  },
  labor_category : LABOR_CATEGORY_RELIGION,
  window_info_height_id : 4,
}

building_shrine_ptah = {
  animations : {
    preview : { pos : [0, 0], pack:PACK_GENERAL, id:73, },
    base : { pos : [0, 0], pack:PACK_GENERAL, id:73, offset:0 },
  },
  labor_category : LABOR_CATEGORY_RELIGION,
  window_info_height_id : 4,
}

building_shrine_seth = {
  animations : {
    preview : { pos : [0, 0], pack:PACK_GENERAL, id:72, },
    base : { pos : [0, 0], pack:PACK_GENERAL, id:72, offset:0 },
  },
  labor_category : LABOR_CATEGORY_RELIGION,
  window_info_height_id : 4,
}

building_shrine_bast = {
  animations : {
    preview : { pos : [0, 0], pack:PACK_GENERAL, id:71, },
    base : { pos : [0, 0], pack:PACK_GENERAL, id:71, offset:0 },
  },
  labor_category : LABOR_CATEGORY_RELIGION,
  window_info_height_id : 4,
}

building_weaponsmith = {
  animations : {
    preview : { pos : [0, 0], pack:PACK_GENERAL, id:123, },
    base : { pos : [0, 0], pack:PACK_GENERAL, id:123, offset:0 },
    work : { pos : [60, -15], pack:PACK_GENERAL, id:123, offset:1, max_frames:12 }
  },
  labor_category : LABOR_CATEGORY_MILITARY,
  window_info_height_id : 1,
}

building_courthouse = {
  animations : {
    preview : { pos : [0, 0], pack:PACK_GENERAL, id:62, },
    base : { pos : [0, 0], pack:PACK_GENERAL, id:62, },
    work : { pos : [80, -80], pack:PACK_GENERAL, id:62, offset:1, max_frames:11 }
  },
  labor_category : LABOR_CATEGORY_INFRASTRUCTURE,
}

building_guild_stonemasons = {
  animations : {
    work : { pos : [-1, -1], anim_id: IMG_GUILD_STONEMASONS }
  },
  labor_category : LABOR_CATEGORY_INFRASTRUCTURE,
}

building_bricklayers_guild = {
  animations : {
    work : { pos : [-1, -1], anim_id: IMG_BRICKLAYERS_GUILD }
  },
  labor_category : LABOR_CATEGORY_INFRASTRUCTURE,
}

building_dock = {
  animations : {
    work : { pos : [-1, -1], anim_id: IMG_BUILDING_DOCK }
  },
  labor_category : LABOR_CATEGORY_INDUSTRY_COMMERCE,
}

building_personal_mansion = {
  animations : {
    work : { pos : [-1, -1], anim_id: IMG_PERSONAL_MANSION }
  },
  labor_category : LABOR_CATEGORY_GOVERNMENT,
}

building_temple_osiris = {
  animations : {
    work : { pos : [-1, -1], anim_id: IMG_TEMPLE_OSIRIS }
  },
  labor_category : LABOR_CATEGORY_RELIGION,
  window_info_height_id : 1,
}

building_temple_ra = {
  animations : {
    work : { pos : [-1, -1], anim_id: IMG_TEMPLE_RA }
  },
  labor_category : LABOR_CATEGORY_RELIGION,
  window_info_height_id : 1,
}

building_temple_ptah = {
  animations : {
    work : { pos : [-1, -1], anim_id: IMG_TEMPLE_PTAH }
  },
  labor_category : LABOR_CATEGORY_RELIGION,
  window_info_height_id : 1,
}

building_temple_seth = {
  animations : {
    work : { pos : [-1, -1], anim_id: IMG_TEMPLE_SETH }
  },
  labor_category : LABOR_CATEGORY_RELIGION,
  window_info_height_id : 1,
}

building_temple_bast = {
  animations : {
    work : { pos : [-1, -1], anim_id: IMG_TEMPLE_BAST }
  },
  labor_category : LABOR_CATEGORY_RELIGION,
  window_info_height_id : 1,
}

building_juggler_school = {
  animations : {
    preview : { pos : [0, 0], pack:PACK_GENERAL, id:46 },
    base : { pos : [0, 0], pack:PACK_GENERAL, id:46 },
    work : { pos : [33, -38], pack:PACK_GENERAL, id:46, offset:2, max_frames:12 }
  },
  window_info_height_id : 1,
}

building_dancer_school = {
  animations : {
    preview : { pos : [0, 0], pack:PACK_GENERAL, id:52, },
    base : { pos : [0, 0], pack:PACK_GENERAL, id:52, },
    work : { pos : [104, 0], pack:PACK_SPR_AMBIENT, id:6, offset:0, max_frames:35 }
  },
  window_info_height_id : 1,
}

building_storage_yard = {
  animations : {
    work : { pos : [23, 27], anim_id : ANIM_STORAGE_YARD_WORK, max_frames:15 }
  },
  labor_category : LABOR_CATEGORY_INDUSTRY_COMMERCE,
  building_size : 3,
}

building_bazaar = {
  animations : {
    preview : { pos : [0, 0], pack:PACK_GENERAL, id:22, },
    base : { pos : [0, 0], pack:PACK_GENERAL, id:22, },
    base_work : { pos : [0, 0], pack:PACK_GENERAL, id:22, offset:0 },
    fancy : { pos : [0, 0], pack:PACK_GENERAL, id:22, offset:12 },
    fancy_work : { pos : [0, 0], pack:PACK_GENERAL, id:22, offset:13 }
  },
  window_info_height_id : 1,
}

building_work_camp = {
  animations : {
    work : { pos : [0, 0], anim_id : IMG_WORKCAMP },
  },
  labor_category : LABOR_CATEGORY_INDUSTRY_COMMERCE,
}

building_booth = {
  animations : {
    booth : { pack:PACK_GENERAL, id:114 },
    square : { pack:PACK_GENERAL, id:112 },
    juggler : { pos : [35, 17], anim_id : IMG_JUGGLER_SHOW }
  },
  labor_category : LABOR_CATEGORY_ENTERTAINMENT,
  window_info_height_id : 2,
  building_size : 2,
}

building_apothecary = {
  animations : {
    work : { pos : [0, 0], anim_id : IMG_BUILDING_APOTHECARY }
  },
  labor_category : LABOR_CATEGORY_WATER_HEALTH,
  window_info_height_id : 1,
}

building_water_supply = {
  animations : {
    work : { pos : [0, 0], anim_id : IMG_WATER_SUPPLY }
  },
  labor_category : LABOR_CATEGORY_WATER_HEALTH,
}

building_well = {
  animations : {
    preview : { pack: PACK_GENERAL, id:23, max_frames:1 },
    base : { pack: PACK_GENERAL, id:23, max_frames:1 },
    base_work : { pack: PACK_GENERAL, id:23, max_frames:1 },
    fancy : { pack: PACK_GENERAL, id:23, offset:2, max_frames:1 },
    fancy_work : { pack: PACK_GENERAL, id:23, offset:3, max_frames:1 }
  },
  window_info_height_id : 4,
  labor_category : LABOR_CATEGORY_WATER_HEALTH,
}

building_papyrus_maker = {
  animations : {
    preview : { pos: [0, 0], pack:PACK_GENERAL, id:44 },
    base : { pos : [0, 0], pack:PACK_GENERAL, id:44 },
    work : { pos : [7, -10], pack:PACK_GENERAL, id:44, offset:1, max_frames:9 }
  },
  output_resource : RESOURCE_PAPYRUS,
  labor_category : LABOR_CATEGORY_INDUSTRY_COMMERCE,
}

building_reed_gatherer = {
  animations : {
    work : { pos : [0, 0], anim_id : IMG_BUILDING_REED_GATHERER }
  },
  labor_category : LABOR_CATEGORY_INDUSTRY_COMMERCE,
}

building_wood_cutter = {
  // todo
}

building_small_mastaba = {
  // todo
}

building_tax_collector = {
  animations : {
    preview : { pos: [0, 0], pack:PACK_GENERAL, id:63 },
    base : { pos : [0, 0], pack:PACK_GENERAL, id:63 },
    work : { pos : [60, -45], pack:PACK_GENERAL, id:63, offset:1, max_frames:11 }
  },
  labor_category : LABOR_CATEGORY_GOVERNMENT,
  window_info_height_id : 1,
}

building_tax_collector_up = {
  animations : {
    preview : { pos: [0, 0], pack:PACK_GENERAL, id:64 },
    base : { pos : [0, 0], pack:PACK_GENERAL, id:64 },
    work : { pos : [10, 10], pack:PACK_GENERAL, id:64, offset:1, max_frames:11 }
  },
  labor_category : LABOR_CATEGORY_GOVERNMENT,
  window_info_height_id : 1,
}

building_recruiter = {
  animations : {
    preview : { pos: [0, 0], pack:PACK_GENERAL, id:166 },
    base : { pos : [0, 0], pack:PACK_GENERAL, id:166 },
    work : { pos : [10, 10], pack:PACK_GENERAL, id:166, offset:1, max_frames:11 }
  },
  labor_category : LABOR_CATEGORY_MILITARY,
  window_info_height_id : 3,
}

building_bandstand = {
  animations : {
    booth : { pos:[0, 0], pack:PACK_GENERAL, id:114 },
    square : { pos:[0, 0], pack:PACK_GENERAL, id:58 },
    juggler : { pos : [35, 17], anim_id : IMG_JUGGLER_SHOW },
    stand_sn_s : { pack:PACK_GENERAL, id:92, offset:0},
    stand_sn_n : { pack:PACK_GENERAL, id:92, offset:1},
    stand_we_w: { pack:PACK_GENERAL, id:92, offset:2},
    stand_we_e: {pack:PACK_GENERAL, id:92, offset:3},
    musician_sn : { pos : [45, 7], pack:PACK_SPR_AMBIENT, id:10, max_frames : 11 },
    musician_we : { pos : [48, 4], pack:PACK_SPR_AMBIENT, id:9, max_frames : 11 }
  },
  labor_category : LABOR_CATEGORY_ENTERTAINMENT,
  window_info_height_id : 3,
  building_size : 3,
}

building_pavilion = {
  animations : {
    base : { pos:[0, 0], pack:PACK_GENERAL, id:48 },
    square : { pos:[0, 0], pack:PACK_GENERAL, id:50 },
    stand_sn_s : { pack:PACK_GENERAL, id:92, offset:0},
    stand_sn_n : { pack:PACK_GENERAL, id:92, offset:1},
    booth : { pos:[0, 0], pack:PACK_GENERAL, id:114 },
  },
  preview_dir_0 : { stand:[-30, 15], stand_b:[90, 45], stand_e:[60, 60], booth:[-60, 30] },
  preview_dir_1 : { stand:[30, 45], stand_b:[0, 0], stand_e:[-30, 15], booth:[30, 75] },
  preview_dir_2 : { stand:[0, 30], stand_b:[0, 90], stand_e:[-30, 75], booth:[90, 45], stand_b_img:2, stand_e_img:2 },
  preview_dir_3 : { stand:[-60, 60], stand_b:[90, 45], stand_e:[60, 30], booth:[0, 90], stand_b_img:2, stand_e_img:2 },
  preview_dir_4 : { stand:[0, 60], stand_b:[-60, 30], stand_e:[-90, 45], booth:[0, 90] },
  preview_dir_5 : { stand:[-60, 30], stand_b:[30, 75], stand_e:[0, 90], booth:[-90, 45] },
  preview_dir_6 : { stand:[-90, 45], stand_b:[30, 15], stand_e:[0, 0], booth:[-30, 75], stand_b_img : 2, stand_e_img : 2 },
  preview_dir_7 : { stand:[-30, 15], stand_b:[-60, 60], stand_e:[-90, 45], booth:[60, 30], stand_b_img : 2, stand_e_img : 2 },

  place_dir_0 : [
    {type: BUILDING_GARDENS, offset:[1, 2]},
    {type: BUILDING_GARDENS, offset:[3, 2]},
    {type: BUILDING_PAVILLION, offset:[0, 0]},
    {type: BUILDING_BANDSTAND, offset:[3, 0], main:true},
    {type: BUILDING_BANDSTAND, offset:[3, 1], main:false},
    {type: BUILDING_BOOTH, offset:[0, 2], main:false}
  ],

  place_dir_1 : [
    {type: BUILDING_GARDENS, offset:[2, 2]},
    {type: BUILDING_GARDENS, offset:[0, 2]},
    {type: BUILDING_PAVILLION, offset:[2, 0]},
    {type: BUILDING_BANDSTAND, offset:[0, 0], main:true},
    {type: BUILDING_BANDSTAND, offset:[0, 1], main:false},
    {type: BUILDING_BOOTH, offset:[3, 2], main:false}
  ],

  place_dir_2 : [
    {type: BUILDING_GARDENS, offset:[3, 0]},
    {type: BUILDING_GARDENS, offset:[3, 3]},
    {type: BUILDING_PAVILLION, offset:[1, 0]},
    {type: BUILDING_BANDSTAND, offset:[1, 3], main:true},
    {type: BUILDING_BANDSTAND, offset:[2, 3], main:false},
    {type: BUILDING_BOOTH, offset:[3, 1], main:false}
  ],

  place_dir_3 : [
    {type: BUILDING_GARDENS, offset:[3, 3]},
    {type: BUILDING_GARDENS, offset:[1, 0]},
    {type: BUILDING_PAVILLION, offset:[1, 2]},
    {type: BUILDING_BANDSTAND, offset:[2, 0], main:true},
    {type: BUILDING_BANDSTAND, offset:[3, 0], main:false},
    {type: BUILDING_BOOTH, offset:[3, 2], main:false}
  ],

  place_dir_4 : [
    {type: BUILDING_GARDENS, offset:[3, 3]},
    {type: BUILDING_GARDENS, offset:[0, 3]},
    {type: BUILDING_PAVILLION, offset:[2, 1]},
    {type: BUILDING_BANDSTAND, offset:[0, 1], main:true},
    {type: BUILDING_BANDSTAND, offset:[0, 2], main:false},
    {type: BUILDING_BOOTH, offset:[2, 3], main:false}
  ],

  place_dir_5 : [
    {type: BUILDING_GARDENS, offset:[1, 3]},
    {type: BUILDING_GARDENS, offset:[3, 3]},
    {type: BUILDING_PAVILLION, offset:[0, 1]},
    {type: BUILDING_BANDSTAND, offset:[3, 1], main:true},
    {type: BUILDING_BANDSTAND, offset:[3, 2], main:false},
    {type: BUILDING_BOOTH, offset:[0, 3], main:false}
  ],

  place_dir_6 : [
    {type: BUILDING_GARDENS, offset:[2, 0]},
    {type: BUILDING_GARDENS, offset:[2, 2]},
    {type: BUILDING_PAVILLION, offset:[0, 2]},
    {type: BUILDING_BANDSTAND, offset:[0, 0], main:false},
    {type: BUILDING_BANDSTAND, offset:[1, 0], main:true},
    {type: BUILDING_BOOTH, offset:[2, 3], main:false}
  ],

  place_dir_7 : [
    {type: BUILDING_GARDENS, offset:[0, 3]},
    {type: BUILDING_GARDENS, offset:[2, 1]},
    {type: BUILDING_PAVILLION, offset:[0, 0]},
    {type: BUILDING_BANDSTAND, offset:[1, 3], main:false},
    {type: BUILDING_BANDSTAND, offset:[2, 3], main:true},
    {type: BUILDING_BOOTH, offset:[2, 0], main:false}
  ],
  building_size : 4,
  window_info_height_id : 2,
}

building_festival_square = {
  building_size : 5,
}

building_ferry = {
  animations : {
    preview : { pack:PACK_TERRAIN, id:23 },
    base : { pack:PACK_TERRAIN, id:23 },
    work : { pack:PACK_TERRAIN, id:23 }
  },
  building_size : 2,
  labor_category : LABOR_CATEGORY_GOVERNMENT,
}

building_village_palace = {
  animations : {
    work : { pos : [-1, -1], anim_id: IMG_VILLAGE_PALACE }
  },
  labor_category : LABOR_CATEGORY_GOVERNMENT,
}

building_clay_pit = {
  animations : {
    work : { pos : [-1, -1], anim_id: IMG_CLAY_PIT }
  },
  output_resource : RESOURCE_CLAY,
  meta_id : "clay_pit",
  labor_category : LABOR_CATEGORY_INDUSTRY_COMMERCE,
}

building_brewery = {
  animations : {
    work : { pos : [-1, -1], base_id : ANIM_BREWERY_WORKSHOP, anim_id: ANIM_BREWERY_WORKSHOP, max_frames: 12 }
  },
  window_info_height_id : 1,
}

building_mine_copper = {
  animations : {
    work : { pos : [54, 15], anim_id: ANIM_GOLD_MINE, max_frames: 16 }
  },
  output_resource : RESOURCE_COPPER,
  info_help_id:93, info_text_id:193,
  labor_category : LABOR_CATEGORY_INDUSTRY_COMMERCE,
}

building_mine_gems = {
  animations : {
    work : { pos : [54, 15], anim_id: ANIM_GOLD_MINE, max_frames: 16 }
  },
  output_resource : RESOURCE_GEMS,
  meta_id : "gems_mine",
  labor_category : LABOR_CATEGORY_INDUSTRY_COMMERCE,
}

building_mine_gold = {
  animations : {
    work : { pos : [54, 15], anim_id: ANIM_GOLD_MINE, max_frames: 16 }
  },
  output_resource : RESOURCE_GOLD,
  info_help_id:93,
  info_text_id:162,
  labor_category : LABOR_CATEGORY_INDUSTRY_COMMERCE,
}

building_pottery = {
  animations : {
    work : { pos : [-1, -1], anim_id: IMG_POTTERY_WORKSHOP }
  },
  production_rate : 20,
  labor_category : LABOR_CATEGORY_INDUSTRY_COMMERCE,
  window_info_height_id : 1,
}

building_physician = {
  animations : {
    preview : { pack:PACK_GENERAL, id:70 },
    base : { pack:PACK_GENERAL, id:70 },
    work : { pos : [-1, -1], pack:PACK_GENERAL, id:70, offset:1, max_frames:11 }
  },
  labor_category : LABOR_CATEGORY_WATER_HEALTH,
}

building_firehouse = {
  animations : {
    work : { pos : [0, 0], anim_id: IMG_BUILDING_FIREHOUSE }
  },
  labor_category : LABOR_CATEGORY_INFRASTRUCTURE,
  window_info_height_id : 1,
}

building_architect_post = {
  animations : {
    preview : { pack:PACK_GENERAL, id:81 },
    base : { pack:PACK_GENERAL, id:81 },
    work : { pos : [20, -35], pack:PACK_GENERAL, id:81, offset:1, max_frames:11 }
  },
  labor_category : LABOR_CATEGORY_INFRASTRUCTURE,
  window_info_height_id : 1,
}

building_conservatory = {
  animations : {
    preview : { pack:PACK_GENERAL, id:51 },
    base : { pack:PACK_GENERAL, id:51 },
    work : { pos : [82, 18], pack:PACK_SPR_AMBIENT, id:10, max_frames:11 }
  },
  labor_category : LABOR_CATEGORY_ENTERTAINMENT,
  window_info_height_id : 1,
}

building_hunting_lodge = {
  animations : {
    preview : { pack:PACK_GENERAL, id:176},
    base : { pack:PACK_GENERAL, id:176},
    work : { pos:[20, -15], pack:PACK_GENERAL, id:176, offset:1, max_frames:11 }
  },
  labor_category : LABOR_CATEGORY_FOOD_PRODUCTION,
}

building_scribal_school = {
  animations : {
    preview : { pack:PACK_GENERAL, id:42},
    base : { pack:PACK_GENERAL, id:42},
    work : { pos:[20, -15], pack:PACK_GENERAL, id:42, offset:1, max_frames:11 }
  },

  info_icon_res :[32, 106],
  info_text_res :[60, 110],
  info_help_id: 68, info_text_id: 85,
  window_info_height_id : 1,

  papyrus_icon:[61, 14],
  labor_category : LABOR_CATEGORY_EDUCATION,
}
