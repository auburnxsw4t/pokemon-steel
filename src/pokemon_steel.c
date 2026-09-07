#include "global.h"
#include "constants/flags.h"
#include "constants/pokemon_steel.h"
#include "constants/vars.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_player_avatar.h"
#include "naming_screen.h"
#include "overworld.h"
#include "string_util.h"

void SteelSetPlayerIdentity(void)
{
    // The non-cancellable roll-call menu returns MALE (0) or FEMALE (1).
    gSaveBlock2Ptr->playerGender = gSpecialVar_Result == FEMALE ? FEMALE : MALE;
    gPlayerAvatar.gender = gSaveBlock2Ptr->playerGender;
    StringCopy(gSaveBlock2Ptr->playerName,
               gPlayerAvatar.gender == FEMALE ? COMPOUND_STRING("Evelyn") : COMPOUND_STRING("Jayson"));
    ObjectEventSetGraphicsId(&gObjectEvents[gPlayerAvatar.objectEventId],
                            GetPlayerAvatarGraphicsIdByStateIdAndGender(PLAYER_AVATAR_STATE_NORMAL, gPlayerAvatar.gender));
}

void SteelNamePlayer(void)
{
    DoNamingScreen(NAMING_SCREEN_PLAYER, gSaveBlock2Ptr->playerName,
                   gSaveBlock2Ptr->playerGender, 0, 0, CB2_ReturnToFieldContinueScript);
}

// Rebuild visibility before map objects spawn. No map owns its own Kyle state.
void SteelSyncOpeningActors(void)
{
    u16 stage = VarGet(VAR_STEEL_OPENING);
    FlagSet(FLAG_HIDE_STEEL_KYLE_SCHOOL);
    FlagSet(FLAG_HIDE_STEEL_KYLE_VILLAGE);
    FlagSet(FLAG_HIDE_STEEL_KYLE_RIDGE);
    FlagSet(FLAG_HIDE_STEEL_KYLE_HOME);
    FlagSet(FLAG_HIDE_STEEL_KYLE_WOODS);
    switch (stage)
    {
    case STEEL_OPENING_ROLL_CALL:
        FlagClear(FLAG_HIDE_STEEL_KYLE_SCHOOL);
        break;
    case STEEL_OPENING_DISMISSED:
        FlagClear(FLAG_HIDE_STEEL_KYLE_RIDGE);
        break;
    default:
        FlagClear(FLAG_HIDE_STEEL_KYLE_HOME);
        break;
    }
}
