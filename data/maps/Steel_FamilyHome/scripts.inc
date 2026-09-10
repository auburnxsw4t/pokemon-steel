Steel_FamilyHome_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, Steel_FamilyHome_SyncActors
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Steel_Home_OnFrame
	.byte 0

Steel_Home_OnFrame:
	map_script_2 VAR_STEEL_OPENING, STEEL_OPENING_HOME, Steel_Home_Arrival
	map_script_2 VAR_STEEL_OPENING, STEEL_OPENING_CREEK_NEXT, Steel_Home_Leave
	map_script_2 VAR_STEEL_OPENING, STEEL_OPENING_HOME_RETURN, Steel_Home_Return
	.2byte 0

Steel_Home_Arrival::
	lockall
	msgbox Steel_Home_Text_Logan, MSGBOX_DEFAULT
	goto Steel_Home_Leave

Steel_Home_Logan::
	goto_if_ge VAR_STEEL_OPENING, STEEL_OPENING_KYLE_LEAVES, Steel_Home_LoganTrainingAdvice
	goto_if_ge VAR_STEEL_OPENING, STEEL_OPENING_STARTER_CHOICE, Steel_Home_LoganChooseStarter
	msgbox Steel_Home_Text_Logan, MSGBOX_NPC
	end

Steel_Home_Madison::
	goto_if_eq VAR_STEEL_OPENING, STEEL_OPENING_KYLE_LEAVES, Steel_Home_MadisonRetryScarf
	goto_if_ge VAR_STEEL_OPENING, STEEL_OPENING_COMPLETE, Steel_Home_MadisonScarfReminder
	msgbox Steel_Home_Text_Madison, MSGBOX_NPC
	end

Steel_Home_Kyle::
	goto_if_eq VAR_STEEL_OPENING, STEEL_OPENING_STARTER_CHOICE, Steel_Home_KyleChooseStarter
	goto_if_eq VAR_STEEL_OPENING, STEEL_OPENING_KYLE_LEAVES, Steel_Home_KyleLeavingSoon
	msgbox Steel_Home_Text_Kyle, MSGBOX_NPC
	end

Steel_Home_LoganChooseStarter::
	msgbox Steel_Home_Text_ChooseStarter, MSGBOX_NPC
	end

Steel_Home_LoganTrainingAdvice::
	msgbox Steel_Home_Text_LoganAdvice, MSGBOX_NPC
	end

Steel_Home_MadisonRetryScarf::
	lockall
	goto Steel_Home_GiveSilkScarf
	end

Steel_Home_MadisonScarfReminder::
	msgbox Steel_Home_Text_ScarfReminder, MSGBOX_NPC
	end

Steel_Home_KyleChooseStarter::
	msgbox Steel_Home_Text_KyleChoose, MSGBOX_NPC
	end

Steel_Home_KyleLeavingSoon::
	msgbox Steel_Home_Text_KyleLeavingSoon, MSGBOX_NPC
	end

Steel_Home_PosskitBall::
	lockall
	goto_if_ne VAR_STEEL_OPENING, STEEL_OPENING_STARTER_CHOICE, Steel_Home_PosskitRemains
	showmonpic STEEL_STARTER_POSSKIT, 10, 3
	msgbox Steel_Home_Text_InspectPosskit, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Steel_Home_ChoosePosskit
	hidemonpic
	releaseall
	end

Steel_Home_SheldoBall::
	lockall
	goto_if_ne VAR_STEEL_OPENING, STEEL_OPENING_STARTER_CHOICE, Steel_Home_SheldoRemains
	showmonpic STEEL_STARTER_SHELDO, 10, 3
	msgbox Steel_Home_Text_InspectSheldo, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Steel_Home_ChooseSheldo
	hidemonpic
	releaseall
	end

Steel_Home_MimbriBall::
	lockall
	goto_if_ne VAR_STEEL_OPENING, STEEL_OPENING_STARTER_CHOICE, Steel_Home_MimbriRemains
	showmonpic STEEL_STARTER_MIMBRI, 10, 3
	msgbox Steel_Home_Text_InspectMimbri, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Steel_Home_ChooseMimbri
	hidemonpic
	releaseall
	end

Steel_Home_PosskitRemains::
	msgbox Steel_Home_Text_PosskitRemains, MSGBOX_DEFAULT
	releaseall
	end

Steel_Home_SheldoRemains::
	msgbox Steel_Home_Text_SheldoRemains, MSGBOX_DEFAULT
	releaseall
	end

Steel_Home_MimbriRemains::
	msgbox Steel_Home_Text_MimbriRemains, MSGBOX_DEFAULT
	releaseall
	end

Steel_Home_ChoosePosskit::
	hidemonpic
	setvar VAR_STEEL_STARTER, STEEL_STARTER_CHOICE_POSSKIT
	setvar VAR_STEEL_KYLE_STARTER, STEEL_STARTER_CHOICE_MIMBRI
	setvar VAR_STARTER_MON, 0
	setflag FLAG_SYS_POKEMON_GET
	givemon STEEL_STARTER_POSSKIT, 5
	setflag FLAG_HIDE_STEEL_STARTER_POSSKIT
	setflag FLAG_HIDE_STEEL_STARTER_MIMBRI
	removeobject LOCALID_STEEL_HOME_POSSKIT_BALL
	removeobject LOCALID_STEEL_HOME_MIMBRI_BALL
	msgbox Steel_Home_Text_ReceivedPosskit, MSGBOX_DEFAULT
	msgbox Steel_Home_Text_KyleTakesMimbri, MSGBOX_DEFAULT
	goto Steel_Home_StartKyleBattle
	end

Steel_Home_ChooseSheldo::
	hidemonpic
	setvar VAR_STEEL_STARTER, STEEL_STARTER_CHOICE_SHELDO
	setvar VAR_STEEL_KYLE_STARTER, STEEL_STARTER_CHOICE_POSSKIT
	setvar VAR_STARTER_MON, 1
	setflag FLAG_SYS_POKEMON_GET
	givemon STEEL_STARTER_SHELDO, 5
	setflag FLAG_HIDE_STEEL_STARTER_SHELDO
	setflag FLAG_HIDE_STEEL_STARTER_POSSKIT
	removeobject LOCALID_STEEL_HOME_SHELDO_BALL
	removeobject LOCALID_STEEL_HOME_POSSKIT_BALL
	msgbox Steel_Home_Text_ReceivedSheldo, MSGBOX_DEFAULT
	msgbox Steel_Home_Text_KyleTakesPosskit, MSGBOX_DEFAULT
	goto Steel_Home_StartKyleBattle
	end

Steel_Home_ChooseMimbri::
	hidemonpic
	setvar VAR_STEEL_STARTER, STEEL_STARTER_CHOICE_MIMBRI
	setvar VAR_STEEL_KYLE_STARTER, STEEL_STARTER_CHOICE_SHELDO
	setvar VAR_STARTER_MON, 2
	setflag FLAG_SYS_POKEMON_GET
	givemon STEEL_STARTER_MIMBRI, 5
	setflag FLAG_HIDE_STEEL_STARTER_MIMBRI
	setflag FLAG_HIDE_STEEL_STARTER_SHELDO
	removeobject LOCALID_STEEL_HOME_MIMBRI_BALL
	removeobject LOCALID_STEEL_HOME_SHELDO_BALL
	msgbox Steel_Home_Text_ReceivedMimbri, MSGBOX_DEFAULT
	msgbox Steel_Home_Text_KyleTakesSheldo, MSGBOX_DEFAULT
	goto Steel_Home_StartKyleBattle
	end

Steel_Home_StartKyleBattle::
	setvar VAR_STEEL_OPENING, STEEL_OPENING_KYLE_BATTLE
	applymovement LOCALID_STEEL_HOME_KYLE, Common_Movement_FacePlayer
	waitmovement LOCALID_STEEL_HOME_KYLE
	msgbox Steel_Home_Text_KyleChallenge, MSGBOX_DEFAULT
	closemessage
	switch VAR_STEEL_KYLE_STARTER
	case STEEL_STARTER_CHOICE_POSSKIT, Steel_Home_BattleKylePosskit
	case STEEL_STARTER_CHOICE_SHELDO, Steel_Home_BattleKyleSheldo
	case STEEL_STARTER_CHOICE_MIMBRI, Steel_Home_BattleKyleMimbri
	end

Steel_Home_BattleKylePosskit::
	trainerbattle_earlyrival TRAINER_KYLE_POSSKIT, RIVAL_BATTLE_HEAL_AFTER, Steel_Home_Text_KyleDefeated, Steel_Home_Text_KyleVictory
	goto Steel_Home_AfterKyleBattle
	end

Steel_Home_BattleKyleSheldo::
	trainerbattle_earlyrival TRAINER_KYLE_SHELDO, RIVAL_BATTLE_HEAL_AFTER, Steel_Home_Text_KyleDefeated, Steel_Home_Text_KyleVictory
	goto Steel_Home_AfterKyleBattle
	end

Steel_Home_BattleKyleMimbri::
	trainerbattle_earlyrival TRAINER_KYLE_MIMBRI, RIVAL_BATTLE_HEAL_AFTER, Steel_Home_Text_KyleDefeated, Steel_Home_Text_KyleVictory
	goto Steel_Home_AfterKyleBattle
	end

Steel_Home_AfterKyleBattle::
	special HealPlayerParty
	msgbox Steel_Home_Text_LoganAdvice, MSGBOX_DEFAULT
	setvar VAR_STEEL_OPENING, STEEL_OPENING_KYLE_LEAVES
	goto Steel_Home_GiveSilkScarf
	end

Steel_Home_GiveSilkScarf::
	goto_if_set FLAG_RECEIVED_STEEL_SILK_SCARF, Steel_Home_KyleDeparts
	msgbox Steel_Home_Text_MadisonGift, MSGBOX_DEFAULT
	giveitem ITEM_SILK_SCARF
	goto_if_eq VAR_RESULT, FALSE, Steel_Home_NoRoomForScarf
	setflag FLAG_RECEIVED_STEEL_SILK_SCARF
	goto Steel_Home_KyleDeparts
	end

Steel_Home_NoRoomForScarf::
	msgbox Steel_Home_Text_NoRoomForScarf, MSGBOX_DEFAULT
	releaseall
	end

Steel_Home_KyleDeparts::
	msgbox Steel_Home_Text_KyleDeparts, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_STEEL_HOME_KYLE, Steel_Home_KyleExitForLeague
	waitmovement LOCALID_STEEL_HOME_KYLE
	removeobject LOCALID_STEEL_HOME_KYLE
	setvar VAR_STEEL_OPENING, STEEL_OPENING_COMPLETE
	special SteelSyncOpeningActors
	releaseall
	end

Steel_Home_TrophySign::
	msgbox Steel_Home_Text_Trophies, MSGBOX_SIGN
	end

Steel_Home_Text_Logan:
	.string "LOGAN: There you two are!\n"
	.string "Ready for your first POKéMON?\p"
	.string "Two are ready, but I still need\n"
	.string "to catch the third.\p"
	.string "We'll head to the creek together.\n"
	.string "SCIZOR will give me a hand.$"

Steel_Home_Text_Madison:
	.string "MADISON: Last day of school, and\n"
	.string "already thinking about your journey.\p"
	.string "Listen to your father out there.$"

Steel_Home_Text_Kyle:
	.string "KYLE: Don't take all day choosing.\n"
	.string "I'm ready for our first battle!$"

Steel_Home_Text_ChooseStarter:
	.string "LOGAN: Look over all three.\n"
	.string "Choose the partner you trust.$"

Steel_Home_Text_KyleChoose:
	.string "KYLE: You choose first.\n"
	.string "I'll take the one that beats\l"
	.string "yours.$"

Steel_Home_Text_KyleLeavingSoon:
	.string "KYLE: Make room for Mom's\n"
	.string "gift. Registration can wait.$"

Steel_Home_Text_InspectPosskit:
	.string "POSSKIT is playful.\n"
	.string "It never gives up.\p"
	.string "Choose POSSKIT?$"

Steel_Home_Text_InspectSheldo:
	.string "SHELDO is sturdy.\n"
	.string "It stands its ground.\p"
	.string "Choose SHELDO?$"

Steel_Home_Text_InspectMimbri:
	.string "MIMBRI is quick.\n"
	.string "It watches everything.\p"
	.string "Choose MIMBRI?$"

Steel_Home_Text_ReceivedPosskit:
	.string "POSSKIT joined your team!$"

Steel_Home_Text_ReceivedSheldo:
	.string "SHELDO joined your team!$"

Steel_Home_Text_ReceivedMimbri:
	.string "MIMBRI joined your team!$"

Steel_Home_Text_KyleTakesPosskit:
	.string "KYLE: Then POSSKIT is mine.\n"
	.string "That gives me the edge.$"

Steel_Home_Text_KyleTakesSheldo:
	.string "KYLE: Then SHELDO is mine.\n"
	.string "That gives me the edge.$"

Steel_Home_Text_KyleTakesMimbri:
	.string "KYLE: Then MIMBRI is mine.\n"
	.string "That gives me the edge.$"

Steel_Home_Text_KyleChallenge:
	.string "KYLE: Let's settle this\n"
	.string "right now!$"

Steel_Home_Text_KyleDefeated:
	.string "KYLE: That was luck.$"

Steel_Home_Text_KyleVictory:
	.string "KYLE:\n"
	.string "That's about what I expected.$"

Steel_Home_Text_LoganAdvice:
	.string "LOGAN: Battles build\n"
	.string "experience.\p"
	.string "Train together and learn\n"
	.string "your partner's strengths.\p"
	.string "A strong bond matters as\n"
	.string "much as any move.$"

Steel_Home_Text_MadisonGift:
	.string "MADISON: Take this\n"
	.string "SILK SCARF.\p"
	.string "It helped me early in my\n"
	.string "journey.$"

Steel_Home_Text_NoRoomForScarf:
	.string "MADISON: Make room in your\n"
	.string "BAG.\p"
	.string "I'll keep the SILK SCARF safe.$"

Steel_Home_Text_ScarfReminder:
	.string "MADISON: The SILK SCARF\n"
	.string "strengthens NORMAL-type moves.$"

Steel_Home_Text_KyleDeparts:
	.string "KYLE: I'm going to Ferrane\n"
	.string "League Registration.\p"
	.string "Try to keep up!$"

Steel_Home_Text_PosskitRemains:
	.string "POSSKIT will stay with Logan.$"

Steel_Home_Text_SheldoRemains:
	.string "SHELDO will stay with Logan.$"

Steel_Home_Text_MimbriRemains:
	.string "MIMBRI will stay with Logan.$"

Steel_Home_Text_Trophies:
	.string "A few old trophies and framed badges.\n"
	.string "Logan and Madison's journeys fill the walls.$"

Steel_FamilyHome_SyncActors:
	special SteelSyncOpeningActors
	end

Steel_Home_Leave::
	lockall
	setvar VAR_STEEL_OPENING, STEEL_OPENING_LEAVING_HOME
	closemessage
	applymovement LOCALID_STEEL_HOME_LOGAN, Steel_Home_LoganExit
	waitmovement LOCALID_STEEL_HOME_LOGAN
	applymovement LOCALID_STEEL_HOME_KYLE, Common_Movement_WalkDown
	waitmovement LOCALID_STEEL_HOME_KYLE
	applymovement LOCALID_STEEL_HOME_LOGAN, Steel_Home_Exit0
	applymovement LOCALID_STEEL_HOME_KYLE, Steel_Home_Exit1
	applymovement OBJ_EVENT_ID_PLAYER, Steel_Home_Exit2
	waitmovement LOCALID_STEEL_HOME_LOGAN
	waitmovement LOCALID_STEEL_HOME_KYLE
	waitmovement OBJ_EVENT_ID_PLAYER
	removeobject LOCALID_STEEL_HOME_LOGAN
	removeobject LOCALID_STEEL_HOME_KYLE
	setvar VAR_STEEL_OPENING, STEEL_OPENING_RIDGE_CREEK
	special SteelSyncOpeningActors
	warp MAP_STEEL_HOMESTEAD_RIDGE, 9, 12
	waitstate
	releaseall
	end

Steel_Home_LoganExit:
	walk_right
	walk_down
	walk_right
	walk_right
	step_end

Steel_Home_Exit0:
	walk_down
	step_end

Steel_Home_Exit1:
	delay_16
	walk_right
	step_end

Steel_Home_Exit2:
	walk_down
	step_end

Steel_Home_Return::
	lockall
	msgbox Steel_Home_Text_Return, MSGBOX_DEFAULT
	clearflag FLAG_HIDE_STEEL_STARTER_POSSKIT
	clearflag FLAG_HIDE_STEEL_STARTER_SHELDO
	clearflag FLAG_HIDE_STEEL_STARTER_MIMBRI
	setvar VAR_STEEL_STARTER, STEEL_STARTER_NONE
	setvar VAR_STEEL_KYLE_STARTER, STEEL_STARTER_NONE
	setvar VAR_STEEL_OPENING, STEEL_OPENING_STARTER_CHOICE
	releaseall
	end
Steel_Home_Text_Return:
	.string "LOGAN: All three are ready.\n"
	.string "Look them over, then choose.$"

Steel_Home_KyleExitForLeague:
	walk_down
	walk_right
	walk_down
	step_end
