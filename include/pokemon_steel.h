#ifndef GUARD_POKEMON_STEEL_H
#define GUARD_POKEMON_STEEL_H

extern bool8 gSteelLoganTutorialActive;
extern u16 gSteelTutorialSwipeHp;
bool32 SteelPrepareLoganTutorial(void);
void SteelEndLoganTutorial(void);
void SteelResumeFailedTutorial(u8 taskId);
void SetControllerToLogan(enum BattlerId battler);
void LoganBufferExecCompleted(enum BattlerId battler);

#endif
