Steel_CatchingWoods_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, Steel_Woods_OnLoad
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Steel_Woods_OnFrame
	.byte 0
Steel_Woods_OnLoad:
	special SteelSyncOpeningActors
	end
Steel_Woods_OnFrame:
	map_script_2 VAR_STEEL_OPENING, STEEL_OPENING_CATCHING, Steel_Woods_Arrival
	.2byte 0
Steel_Woods_Arrival::
	lockall
	closemessage
	applymovement LOCALID_STEEL_WOODS_LOGAN, Steel_Woods_In0
	applymovement LOCALID_STEEL_WOODS_KYLE, Steel_Woods_In1
	applymovement OBJ_EVENT_ID_PLAYER, Steel_Woods_In2
	waitmovement LOCALID_STEEL_WOODS_LOGAN
	waitmovement LOCALID_STEEL_WOODS_KYLE
	waitmovement OBJ_EVENT_ID_PLAYER
	msgbox Steel_Woods_Text_Found, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_STEEL_WOODS_LOGAN, Steel_Woods_Clearing0
	applymovement LOCALID_STEEL_WOODS_KYLE, Steel_Woods_Clearing1
	applymovement OBJ_EVENT_ID_PLAYER, Steel_Woods_Clearing2
	waitmovement LOCALID_STEEL_WOODS_LOGAN
	waitmovement LOCALID_STEEL_WOODS_KYLE
	waitmovement OBJ_EVENT_ID_PLAYER
	goto Steel_Woods_Demonstrate
Steel_Woods_Logan::
	lockall
	goto_if_eq VAR_STEEL_OPENING, STEEL_OPENING_TUTORIAL_RETRY, Steel_Woods_Demonstrate
	releaseall
	end
Steel_Woods_Demonstrate:
	closemessage
	special StartLoganTutorialBattle
	goto_if_eq VAR_RESULT, FALSE, Steel_Woods_Retry
	msgbox Steel_Woods_Text_Kyle, MSGBOX_DEFAULT
	msgbox Steel_Woods_Text_LoganReply, MSGBOX_DEFAULT
	closemessage
	setvar VAR_STEEL_OPENING, STEEL_OPENING_WOODS_RETURN
	applymovement LOCALID_STEEL_WOODS_LOGAN, Steel_Woods_Back0
	applymovement LOCALID_STEEL_WOODS_KYLE, Steel_Woods_Back1
	applymovement OBJ_EVENT_ID_PLAYER, Steel_Woods_Back2
	waitmovement LOCALID_STEEL_WOODS_LOGAN
	waitmovement LOCALID_STEEL_WOODS_KYLE
	waitmovement OBJ_EVENT_ID_PLAYER
	setvar VAR_STEEL_OPENING, STEEL_OPENING_RIDGE_RETURN
	special SteelSyncOpeningActors
	warp MAP_STEEL_HOMESTEAD_RIDGE, 4, 17
	waitstate
	releaseall
	end

Steel_Woods_Retry:
	setvar VAR_STEEL_OPENING, STEEL_OPENING_TUTORIAL_RETRY
	msgbox Steel_Woods_Text_Retry, MSGBOX_DEFAULT
	releaseall
	end
Steel_Woods_Sign::
	msgbox Steel_Woods_Text_Sign, MSGBOX_SIGN
	end
Steel_Woods_Text_Sign:
	.string "CREEKSIDE WOODS$"
Steel_Woods_Text_Found:
	.string "LOGAN: There! Watch carefully.\n"
	.string "SCIZOR, give me a hand!$"
Steel_Woods_Text_Kyle:
	.string "KYLE: I could've caught it at\n"
	.string "full health.$"
Steel_Woods_Text_LoganReply:
	.string "LOGAN: When you're buying the\n"
	.string "POKé BALLS, you can try.$"
Steel_Woods_Text_Retry:
	.string "LOGAN: Let's try that again.\n"
	.string "Talk to me when you're ready.$"

Steel_Woods_In0:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

Steel_Woods_Clearing0:
	walk_up
	walk_up
	walk_up
	face_right
	step_end

Steel_Woods_Back0:
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

Steel_Woods_In1:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

Steel_Woods_Clearing1:
	walk_up
	walk_up
	walk_up
	face_right
	step_end

Steel_Woods_Back1:
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end

Steel_Woods_In2:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

Steel_Woods_Clearing2:
	walk_up
	walk_up
	walk_up
	face_right
	step_end

Steel_Woods_Back2:
	walk_down
	walk_down
	walk_down
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	step_end
