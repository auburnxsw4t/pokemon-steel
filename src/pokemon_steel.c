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
    FlagSet(FLAG_HIDE_STEEL_KYLE_REGISTRATION);
    FlagSet(FLAG_HIDE_STEEL_WOODS_TARGET);
    switch (stage)
    {
    case STEEL_OPENING_ROLL_CALL:
        FlagClear(FLAG_HIDE_STEEL_KYLE_SCHOOL);
        break;
    case STEEL_OPENING_DISMISSED:
    case STEEL_OPENING_ALUMINA_WALK:
        FlagClear(FLAG_HIDE_STEEL_KYLE_VILLAGE);
        break;
    case STEEL_OPENING_COMPLETE:
        FlagClear(FLAG_HIDE_STEEL_KYLE_REGISTRATION);
        break;
    case STEEL_OPENING_RIDGE_HOME:
    case STEEL_OPENING_RIDGE_CREEK:
    case STEEL_OPENING_RIDGE_RETURN:
        FlagClear(FLAG_HIDE_STEEL_KYLE_RIDGE);
        break;
    case STEEL_OPENING_CATCHING:
    case STEEL_OPENING_WOODS_RETURN:
    case STEEL_OPENING_TUTORIAL_RETRY:
        FlagClear(FLAG_HIDE_STEEL_KYLE_WOODS);
        break;
    default:
        FlagClear(FLAG_HIDE_STEEL_KYLE_HOME);
        break;
    }
    FlagSet(FLAG_HIDE_STEEL_LOGAN_HOME);
    FlagSet(FLAG_HIDE_STEEL_LOGAN_RIDGE);
    FlagSet(FLAG_HIDE_STEEL_LOGAN_WOODS);
    if (stage == STEEL_OPENING_RIDGE_CREEK || stage == STEEL_OPENING_RIDGE_RETURN)
        FlagClear(FLAG_HIDE_STEEL_LOGAN_RIDGE);
    else if (stage == STEEL_OPENING_CATCHING || stage == STEEL_OPENING_WOODS_RETURN || stage == STEEL_OPENING_TUTORIAL_RETRY)
        FlagClear(FLAG_HIDE_STEEL_LOGAN_WOODS);
    else
        FlagClear(FLAG_HIDE_STEEL_LOGAN_HOME);
    if (stage == STEEL_OPENING_CATCHING)
        FlagClear(FLAG_HIDE_STEEL_WOODS_TARGET);
}
