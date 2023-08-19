#include "translation/common.h"
#include "translation/translation.h"

static translation_string all_strings[]
  = {{TR_NO_PATCH_TITLE, "未安裝1.0.1.0補丁"},
     {TR_NO_PATCH_MESSAGE,
      "《凱撒大帝3》未安裝1.0.1.0補丁。"
      "補丁下載連結:\n" URL_PATCHES "\n"
      "繼續遊戲風險自擔。"},
     {TR_MISSING_FONTS_TITLE, "缺失字體"},
     {TR_MISSING_FONTS_MESSAGE,
      "《凱撒大帝3》需要額外字體檔。"
      "語言包下載連結:\n" URL_PATCHES},
     {TR_NO_EDITOR_TITLE, "未安裝地圖編輯器"},
     {TR_NO_EDITOR_MESSAGE,
      "《凱撒大帝3》未檢測到地圖編輯器檔。"
      "地圖編輯器下載連結:\n" URL_EDITOR},
     {TR_INVALID_LANGUAGE_TITLE, "語言包路徑無效"},
     {TR_INVALID_LANGUAGE_MESSAGE,
      "指定路徑未檢測到有效語言包。"
      "請檢視日志查看錯誤。"},
     {TR_BUTTON_OK, "確定"},
     {TR_BUTTON_CANCEL, "取消"},
     {TR_BUTTON_RESET_DEFAULTS, "重置默認"},
     {TR_BUTTON_CONFIGURE_HOTKEYS, "熱鍵綁定"},
     {TR_CONFIG_TITLE, "配置設定"},
     {TR_CONFIG_LANGUAGE_LABEL, "語言包:"},
     {TR_CONFIG_LANGUAGE_DEFAULT, "默認"},
     {TR_CONFIG_HEADER_UI_CHANGES, "使用者介面更變"},
     {TR_CONFIG_HEADER_GAMEPLAY_CHANGES, "遊戲內容更變"},
     {TR_CONFIG_HEADER_GODS_CHANGES, "Gods changes"},
     {TR_CONFIG_SHOW_INTRO_VIDEO, "播放啓動動畫"},
     {TR_CONFIG_SIDEBAR_INFO, "控制台更多資訊"},
     {TR_CONFIG_SMOOTH_SCROLLING, "平滑視角滾動"},
     {TR_CONFIG_VISUAL_FEEDBACK_ON_DELETE, "優化清理土地視覺回饋"},
     {TR_CONFIG_ALLOW_CYCLING_TEMPLES, "允許連續建造每種神廟"},
     {TR_CONFIG_SHOW_WATER_STRUCTURE_RANGE, "建造時顯示貯水池水泉及水井覆蓋範圍"},
     {TR_CONFIG_SHOW_CONSTRUCTION_SIZE, "顯示拖動建設大小"},
     {TR_CONFIG_HIGHLIGHT_LEGIONS, "滑鼠懸停時高亮軍團"},
     {TR_CONFIG_FIX_IMMIGRATION_BUG, "修復非常困難不來人BUG"},
     {TR_CONFIG_FIX_100_YEAR_GHOSTS, "修復人口百歲仍占房BUG"},
     {TR_CONFIG_FIX_EDITOR_EVENTS, "自定遊戲修正：皇帝更位支持度重置至50；生存時間結束才勝利"},
     {TR_HOTKEY_TITLE, "熱鍵綁定"},
     {TR_HOTKEY_LABEL, "熱鍵"},
     {TR_HOTKEY_ALTERNATIVE_LABEL, "可替代鍵"},
     {TR_HOTKEY_HEADER_ARROWS, "方向鍵"},
     {TR_HOTKEY_HEADER_GLOBAL, "全域熱鍵"},
     {TR_HOTKEY_HEADER_CITY, "城市熱鍵"},
     {TR_HOTKEY_HEADER_ADVISORS, "顧問"},
     {TR_HOTKEY_HEADER_OVERLAYS, "覆層"},
     {TR_HOTKEY_HEADER_BOOKMARKS, "城市地圖視角標簽"},
     {TR_HOTKEY_HEADER_EDITOR, "編輯器"},
     {TR_HOTKEY_HEADER_BUILD, "建造熱鍵"},
     {TR_HOTKEY_ARROW_UP, "上"},
     {TR_HOTKEY_ARROW_DOWN, "下"},
     {TR_HOTKEY_ARROW_LEFT, "左"},
     {TR_HOTKEY_ARROW_RIGHT, "右"},
     {TR_HOTKEY_TOGGLE_FULLSCREEN, "全屏切換"},
     {TR_HOTKEY_CENTER_WINDOW, "中心視窗化"},
     {TR_HOTKEY_RESIZE_TO_640, "重置解析度至640x480"},
     {TR_HOTKEY_RESIZE_TO_800, "重置解析度至800x600"},
     {TR_HOTKEY_RESIZE_TO_1024, "重置解析度至1024x768"},
     {TR_HOTKEY_SAVE_SCREENSHOT, "保存截圖"},
     {TR_HOTKEY_SAVE_CITY_SCREENSHOT, "保存城市全景截圖"},
     {TR_HOTKEY_LOAD_FILE, "載入文件"},
     {TR_HOTKEY_SAVE_FILE, "保存檔"},
     {TR_HOTKEY_INCREASE_GAME_SPEED, "加快遊戲速度"},
     {TR_HOTKEY_DECREASE_GAME_SPEED, "減慢遊戲速度"},
     {TR_HOTKEY_TOGGLE_PAUSE, "暫停切換"},
     {TR_HOTKEY_CYCLE_LEGION, "切換各軍團所在視角"},
     {TR_HOTKEY_ROTATE_MAP_LEFT, "順時針旋轉地圖視角"},
     {TR_HOTKEY_ROTATE_MAP_RIGHT, "逆時針旋轉地圖視角"},
     {TR_HOTKEY_SHOW_ADVISOR_LABOR, "勞工顧問"},
     {TR_HOTKEY_SHOW_ADVISOR_MILITARY, "軍事顧問"},
     {TR_HOTKEY_SHOW_ADVISOR_IMPERIAL, "皇帝顧問"},
     {TR_HOTKEY_SHOW_ADVISOR_RATINGS, "評比顧問"},
     {TR_HOTKEY_SHOW_ADVISOR_TRADE, "貿易顧問"},
     {TR_HOTKEY_SHOW_ADVISOR_POPULATION, "人口顧問"},
     {TR_HOTKEY_SHOW_ADVISOR_HEALTH, "健康顧問"},
     {TR_HOTKEY_SHOW_ADVISOR_EDUCATION, "教育顧問"},
     {TR_HOTKEY_SHOW_ADVISOR_ENTERTAINMENT, "娛樂顧問"},
     {TR_HOTKEY_SHOW_ADVISOR_RELIGION, "宗教顧問"},
     {TR_HOTKEY_SHOW_ADVISOR_FINANCIAL, "財政顧問"},
     {TR_HOTKEY_SHOW_ADVISOR_CHIEF, "首席顧問"},
     {TR_HOTKEY_TOGGLE_OVERLAY, "當前覆層視角切換"},
     {TR_HOTKEY_SHOW_OVERLAY_WATER, "顯示供水覆層"},
     {TR_HOTKEY_SHOW_OVERLAY_FIRE, "顯示火災覆層"},
     {TR_HOTKEY_SHOW_OVERLAY_DAMAGE, "顯示損壞覆層"},
     {TR_HOTKEY_SHOW_OVERLAY_CRIME, "顯示犯罪覆層"},
     {TR_HOTKEY_SHOW_OVERLAY_PROBLEMS, "顯示問題覆層"},
     {TR_HOTKEY_GO_TO_BOOKMARK_1, "前往視角標簽 1"},
     {TR_HOTKEY_GO_TO_BOOKMARK_2, "前往視角標簽 2"},
     {TR_HOTKEY_GO_TO_BOOKMARK_3, "前往視角標簽 3"},
     {TR_HOTKEY_GO_TO_BOOKMARK_4, "前往視角標簽 4"},
     {TR_HOTKEY_SET_BOOKMARK_1, "設定視角標簽 1"},
     {TR_HOTKEY_SET_BOOKMARK_2, "設定視角標簽 2"},
     {TR_HOTKEY_SET_BOOKMARK_3, "設定視角標簽 3"},
     {TR_HOTKEY_SET_BOOKMARK_4, "設定視角標簽 4"},
     {TR_HOTKEY_EDITOR_TOGGLE_BATTLE_INFO, "戰鬥資訊切換"},
     {TR_HOTKEY_EDIT_TITLE, "按下新熱鍵"}};

void translation_traditional_chinese(const translation_string** strings, int* num_strings) {
    *strings = all_strings;
    *num_strings = sizeof(all_strings) / sizeof(translation_string);
}
