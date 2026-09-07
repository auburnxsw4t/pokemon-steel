#include "global.h"
#include "event_data.h"
#include "battle.h"
#include "main.h"
#include "load_save.h"
#include "overworld.h"
#include "pokemon.h"
#include "pokemon_steel.h"
#include "script.h"
#include "task.h"
#include "constants/moves.h"
#include "constants/pokemon_steel.h"

// Keep the demonstration's borrowed team and battle side effects out of the save.
// Unlike SavePlayerParty, this does not borrow Frontier or player save storage.
struct SteelTutorialSnapshot
{
    struct Pokemon parties[MAX_BATTLE_TRAINERS][PARTY_SIZE];
    u8 counts[MAX_BATTLE_TRAINERS];
    struct Pokemon savedParty[PARTY_SIZE];
    u8 savedPartyCount;
    struct Bag bag;
    u32 encryptionKey;
    u32 money;
    u16 coins;
    struct Pokedex pokedex;
    u8 dexSeen[NUM_DEX_FLAG_BYTES];
    u8 dexCaught[NUM_DEX_FLAG_BYTES];
    u32 gameStats[NUM_GAME_STATS];
    struct SaveBlock3 save3;
    struct BattleResults results;
    u32 battleFlags;
    u8 outcome;
    enum Item lastItem;
    u16 lastBall;
    bool8 lastBallMenuPresent;
    MainCallback savedCallback;
};

// Battle initialization relocates saves and resets the entire heap. This
// snapshot must live outside that heap until the field callback restores it.
static EWRAM_DATA struct SteelTutorialSnapshot sSnapshot = {0};
static EWRAM_DATA struct SteelTutorialSnapshot *sTutorial = NULL;
EWRAM_DATA bool8 gSteelLoganTutorialActive = FALSE;
EWRAM_DATA u16 gSteelTutorialSwipeHp = 0;

bool32 SteelPrepareLoganTutorial(void)
{
    if (sTutorial != NULL)
        return FALSE;
    sTutorial = &sSnapshot;
    sTutorial->encryptionKey = gSaveBlock2Ptr->encryptionKey;
    memcpy(sTutorial->parties, gParties, sizeof(gParties));
    memcpy(sTutorial->counts, gPartiesCount, sizeof(gPartiesCount));
    memcpy(sTutorial->savedParty, gSaveBlock1Ptr->playerParty, sizeof(sTutorial->savedParty));
    sTutorial->savedPartyCount = gSaveBlock1Ptr->playerPartyCount;
    sTutorial->bag = gSaveBlock1Ptr->bag;
    sTutorial->money = gSaveBlock1Ptr->money;
    sTutorial->coins = gSaveBlock1Ptr->coins;
    sTutorial->pokedex = gSaveBlock2Ptr->pokedex;
    memcpy(sTutorial->dexSeen, gSaveBlock1Ptr->dexSeen, sizeof(sTutorial->dexSeen));
    memcpy(sTutorial->dexCaught, gSaveBlock1Ptr->dexCaught, sizeof(sTutorial->dexCaught));
    memcpy(sTutorial->gameStats, gSaveBlock1Ptr->gameStats, sizeof(sTutorial->gameStats));
    sTutorial->save3 = *gSaveBlock3Ptr;
    sTutorial->results = gBattleResults;
    sTutorial->battleFlags = gBattleTypeFlags;
    sTutorial->outcome = gBattleOutcome;
    sTutorial->lastItem = gLastUsedItem;
    sTutorial->lastBall = gLastUsedBall;
    sTutorial->lastBallMenuPresent = gLastUsedBallMenuPresent;
    sTutorial->savedCallback = gMain.savedCallback;

    ZeroPlayerPartyMons();
    ZeroEnemyPartyMons();
    // A high-level Scizor guarantees False Swipe reaches 1 HP against the
    // level-5 placeholder, without overriding the move's real damage logic.
    CreateMonWithIVs(&gParties[B_TRAINER_PLAYER][0], SPECIES_SCIZOR, 50, 0, OTID_STRUCT_PLAYER_ID, 31);
    SetMonMoveSlot(&gParties[B_TRAINER_PLAYER][0], MOVE_FALSE_SWIPE, 0);
    for (u32 i = 1; i < MAX_MON_MOVES; i++)
        SetMonMoveSlot(&gParties[B_TRAINER_PLAYER][0], MOVE_NONE, i);
    CreateMonWithIVs(&gParties[B_TRAINER_OPPONENT_A][0], STEEL_TUTORIAL_SPECIES, 5, 0, OTID_STRUCT_PLAYER_ID, 0);
    SetMonMoveSlot(&gParties[B_TRAINER_OPPONENT_A][0], MOVE_GROWL, 0);
    for (u32 i = 1; i < MAX_MON_MOVES; i++)
        SetMonMoveSlot(&gParties[B_TRAINER_OPPONENT_A][0], MOVE_NONE, i);
    gPartiesCount[B_TRAINER_PLAYER] = 1;
    gPartiesCount[B_TRAINER_OPPONENT_A] = 1;
    gSteelTutorialSwipeHp = 0;
    gSteelLoganTutorialActive = TRUE;
    return TRUE;
}

void SteelEndLoganTutorial(void)
{
    bool32 caught = gBattleOutcome == B_OUTCOME_CAUGHT && gSteelTutorialSwipeHp == 1;
    if (sTutorial != NULL)
    {
        ApplyNewEncryptionKeyToAllEncryptedData(sTutorial->encryptionKey);
        gSaveBlock2Ptr->encryptionKey = sTutorial->encryptionKey;
        memcpy(gParties, sTutorial->parties, sizeof(gParties));
        memcpy(gPartiesCount, sTutorial->counts, sizeof(gPartiesCount));
        memcpy(gSaveBlock1Ptr->playerParty, sTutorial->savedParty, sizeof(sTutorial->savedParty));
        gSaveBlock1Ptr->playerPartyCount = sTutorial->savedPartyCount;
        gSaveBlock1Ptr->bag = sTutorial->bag;
        gSaveBlock1Ptr->money = sTutorial->money;
        gSaveBlock1Ptr->coins = sTutorial->coins;
        gSaveBlock2Ptr->pokedex = sTutorial->pokedex;
        memcpy(gSaveBlock1Ptr->dexSeen, sTutorial->dexSeen, sizeof(sTutorial->dexSeen));
        memcpy(gSaveBlock1Ptr->dexCaught, sTutorial->dexCaught, sizeof(sTutorial->dexCaught));
        memcpy(gSaveBlock1Ptr->gameStats, sTutorial->gameStats, sizeof(sTutorial->gameStats));
        *gSaveBlock3Ptr = sTutorial->save3;
        gBattleResults = sTutorial->results;
        gBattleTypeFlags = sTutorial->battleFlags;
        gBattleOutcome = sTutorial->outcome;
        gLastUsedItem = sTutorial->lastItem;
        gLastUsedBall = sTutorial->lastBall;
        gLastUsedBallMenuPresent = sTutorial->lastBallMenuPresent;
        gMain.savedCallback = sTutorial->savedCallback;
        sTutorial = NULL;
    }
    gSteelLoganTutorialActive = FALSE;
    gSpecialVar_Result = caught;
    CB2_ReturnToFieldContinueScriptPlayMapMusic();
}

void SteelResumeFailedTutorial(u8 taskId)
{
    // Resume after the special's implicit waitstate, not before it executes.
    gSpecialVar_Result = FALSE;
    ScriptContext_Enable();
    DestroyTask(taskId);
}
