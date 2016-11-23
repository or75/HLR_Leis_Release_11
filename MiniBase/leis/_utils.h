#pragma once

#include "../main.h"

/* START UTILS */

extern bool UpdateOneTarget;

bool CalcScreen( float *pflOrigin , float *pflVecScreen );
float CalcDistPlayerScreen( float fScreenCenter[2] , float fPlayerPosScreen[2] );
bool PathFree( Vector vInput );
bool CalcPlayerVisibility( int Index );
void VectorAngles( const float *forward , float *angles );
float AngleBetvenVectors( Vector a , Vector b );
void TraceGrenade( float *dest );
void HookOpenGL();

/* END UTILS */

/* LEIS GLOBAL */

#define ERR_MODE "[Opengl mode error]"

#define CFG_VERDANA "Verdana"
#define CFG_ARIAL_BLACK "Arial Black"

#define CFG_LEIS_MESSAGE "HLR Leis Release 11"
#define CFG_LEIS_AUTHOR "by _or_75"
#define CFG_LEIS_WEBSITE "www.csxr.ru"

#define CFG_LEIS_SAVED "Settings Saved"
#define CFG_LEIS_LOADED "Settings Loaded"

#define CFG_OPENGL32 "opengl32.dll"
#define CFG_GLBEGIN "glBegin"
#define CFG_GLCLEAR "glClear"
#define CFG_GLVERTEX3FV "glVertex3fv"
#define CFG_GLVERTEX3F "glVertex3f"
#define CFG_GLEND "glEnd"

#define CFG_SNAPSHOT "snapshot"
#define CFG_SCREENSHOT "screenshot"

/* LEIS HACK */

#define LEIS_SOUND_CHAR "*"

#define LEIS_DRAW_RADAR	"drawradar"
#define LEIS_HIDE_RADAR "hideradar"

#define SPR_REMOVE_SMOKE "black_smoke"
#define SPR_REMOVE_SPRITE ".spr"
#define SPR_REMOVE_DRAW_WEAPON "w_"

#define ESP_YOR_ARE	"you are "
#define ESP_FLAHED "% flashed"

#define ESP_IDLE "idle"
#define ESP_SHOOTING "shooting"
#define ESP_RELOADING "reloading"
#define ESP_DEAD "dead"
#define ESP_THROWING "throwing"
#define ESP_PLANTING "planting"
#define ESP_SWINGING "swinging"
#define ESP_OPEN_SHIELD "open shield"
#define ESP_CLOS_SHIELD	"closed shield"

#define ESP_STANDING "standing"
#define ESP_CROUCHING "crouching"
#define ESP_WALKING	"walking"
#define ESP_RUNNING	"running"
#define ESP_CROUCH_RUN "crouch running"
#define ESP_JUMPING	"jumping"
#define ESP_SWIMMING "swimming"

/* LEIS CONFIG */

#define CFG_LEIS_CONFIG "standard.ini"

#define CFG_AIMBOT "Aimbot"
#define CFG_ESP "Esp"
#define CFG_WALLHACK "Wallhack"
#define CFG_RADAR "Radar"
#define CFG_REMOVAL "Removal"
#define CFG_KNIFEBOT "Knifebot"
#define CFG_MISC "Misc"
#define CFG_KEY "Key"

/* LEIS MENU */

#define CFG_AIMBOT_SETTINGS "Aimbot Settings"
#define CFG_ESP_SETTINGS "ESP Settings"
#define CFG_WALLHACK_SETTINGS "Wallhack Settings"
#define CFG_RADAR_SETTINGS "Radar Settings"
#define CFG_REMOVAL_SETTINGS "Removal Settings"
#define CFG_KNIFEBOT_SETTINGS "Knifebot Settings"
#define CFG_MISC_SETTINGS "Misc Settings"

/* LEIS SETTINGS */

#define CFG_AIM_ACTIVE "aim_Active"
#define CFG_AIM_DEATHMATCH "aim_Deathmatch"
#define CFG_AIM_THROUGH "aim_Aimthrough"
#define CFG_AIM_SILENT "aim_Silent"
#define CFG_AIM_SMOOTH "aim_Smooth"
#define CFG_AIM_FOV "aim_Fov"
#define CFG_AIM_FOV_TYPE "aim_FovType"
#define CFG_AIM_FOV_DIST "aim_FovDist"
#define CFG_AIM_NOTWITCHING "aim_NoTwitching"
#define CFG_AIM_MODE "aim_Mode"
#define CFG_AIM_RECOIL_X "aim_RecoilX"
#define CFG_AIM_RECOIL_Y "aim_RecoilY"
#define CFG_AIM_ORIGIN "aim_Origin"
#define CFG_AIM_BONE "aim_Bone"
#define CFG_AIM_HITBOX "aim_Hitbox"
#define CFG_AIM_TIME "aim_Time"
#define CFG_AIM_DELAY "aim_Delay"
#define CFG_AIM_SHOT_DELAY "aim_ShotDelay"
#define CFG_AIM_KILL_DELAY "aim_KillDelay"
#define CFG_AIM_PREDICTION "aim_Prediction"
#define CFG_AIM_DISTANCE "aim_Distance"
#define CFG_AIM_DRAWAIMSPOT "aim_DrawAimspot"
#define CFG_AIM_DRAW_FOV "aim_DrawFov"

#define CFG_AIM_CROSSHAIR "aim_Crosshair"

#define CFG_AIM_SAVESETTINGS "aim_SaveSettings"
#define CFG_AIM_LOADSETTINGS "aim_LoadSettings"

#define CFG_ESP_STYLE "esp_Style"
#define CFG_ESP_SIZE "esp_Size"
#define CFG_ESP_VISIBLE "esp_Visible"
#define CFG_ESP_NAME "esp_Name"
#define CFG_ESP_WEAPON "esp_Weapon"
#define CFG_ESP_3DBOX "esp_3Dbox"
#define CFG_ESP_SOUND "esp_Sound"
#define CFG_ESP_SOUND_TYPE "esp_SoundType"
#define CFG_ESP_SEQUENCE "esp_Sequence"
#define CFG_ESP_G_SEQUENCE "esp_GaitSequence"
#define CFG_ESP_BAREL "esp_Barrel"
#define CFG_ESP_SPIKED "esp_Spiked"
#define CFG_ESP_WORLD_WPN "esp_WorldWeapons"
#define CFG_ESP_WORLD_SPR "esp_WorldSprite"
#define CFG_ESP_DRAW_FONT "esp_DrawFont"
#define CFG_ESP_MULTICOLOR "esp_MultiColor"

#define CFG_WH_DRAW_PLAYER "wh_DrawPlayer"
#define CFG_WH_DRAW_WEAPON "wh_DrawWeapon"
#define CFG_WH_COLOR_PLAYER "wh_ColorPlayer"
#define CFG_WH_COLOR_WEAPON "wh_ColorWeapon"
#define CFG_WH_COLOR_GRENADE "wh_ColorGrenade"
#define CFG_WH_TRAN_ENTITY "wh_TransEntity"
#define CFG_WH_WALL_MODE "wh_Wallmode"
#define CFG_WH_WIREFRAME "wh_Wireframe"
#define CFG_WH_WIREFRAME_R "wh_WireframeR"
#define CFG_WH_WIREFRAME_G "wh_WireframeG"
#define CFG_WH_WIREFRAME_B "wh_WireframeB"
#define CFG_WH_LOCAL_GLOW "wh_LocalGlow"
#define CFG_WH_LOCAL_GLOW_R "wh_LocalGlowR"
#define CFG_WH_LOCAL_GLOW_G "wh_LocalGlowG"
#define CFG_WH_LOCAL_GLOW_B "wh_LocalGlowB"

#define CFG_RADAR_ACTIVE "rad_Active"
#define CFG_RADAR_SIZE "rad_Size"
#define CFG_RADAR_ZOOM "rad_Zoom"
#define CFG_RADAR_TYPE "rad_Type"
#define CFG_RADAR_VISIBLE "rad_Visible"

#define CFG_REMOVE_SKY "rem_RemoveSky"
#define CFG_REMOVE_SMOKE "rem_RemoveSmoke"
#define CFG_REMOVE_FLASH "rem_RemoveFlash"
#define CFG_REMOVE_SPRITE "rem_RemoveSprite"
#define CFG_REMOVE_DRAW_WEAPON "rem_RemoveDrawWeapon"
#define CFG_REMOVE_FLASH_MSG "rem_FlashMessage"
#define CFG_REMOVE_AMXX_MSG "rem_AmxxMessage"

#define CFG_KNIFEBOT_ACTIVE "knf_Active"
#define CFG_KNIFEBOT_AIM "knf_Aim"
#define CFG_KNIFEBOT_AIM_FOV "knf_AimFov"
#define CFG_KNIFEBOT_TEAM "knf_Team"
#define CFG_KNIFEBOT_ATTACK "knf_Attack"
#define CFG_KNIFEBOT_DST_ATTACK "knf_DistAttack"
#define CFG_KNIFEBOT_DST_ATTACK2 "knf_DistAttack2"

#define CFG_MISC_BHOP "misc_Bhop"
#define CFG_MISC_CHASE_CAM "misc_ChaseCam"
#define CFG_MISC_FREELOOK "misc_FreeLook"
#define CFG_MISC_FREELOOK_SPEED "misc_FreeLookSpeed"
#define CFG_MISC_ANTI_SCREEN "misc_AntiScreen"
#define CFG_MISC_GRANADE_TRAIL "misc_GranadeTrail"
#define CFG_MISC_FASTRUN "misc_FastRun"
#define CFG_MISC_MENU_COLOR "misc_MenuColor"

#define CFG_TOGGLE_MENU "key_ToggleMenu"
#define CFG_TOGGLE_PANIC "key_TogglePanic"
#define CFG_NAV_ENTER "key_NavEnter"
#define CFG_NAV_LEAVE "key_NavLeave"
#define CFG_NAV_UP "key_NavUp"
#define CFG_NAV_DOWN "key_NavDown"
#define CFG_NAV_LEFT "key_NavLeft"
#define CFG_NAV_RIGHT "key_NavRight"