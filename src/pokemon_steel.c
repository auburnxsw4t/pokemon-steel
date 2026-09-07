#include "global.h"
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
