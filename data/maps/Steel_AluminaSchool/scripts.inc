Steel_AluminaSchool_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, Steel_AluminaSchool_SyncActors
	map_script MAP_SCRIPT_ON_TRANSITION, Steel_School_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Steel_School_OnFrame
	.byte 0

Steel_School_OnTransition:
	end

Steel_School_OnFrame:
	map_script_2 VAR_STEEL_OPENING, STEEL_OPENING_ROLL_CALL, Steel_School_RollCall
	.2byte 0

Steel_School_RollCall::
	lockall
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	msgbox Steel_School_Text_LastDay, MSGBOX_DEFAULT
Steel_School_Identity:
	message Steel_School_Text_Identity
	waitmessage
	multichoice 18, 4, MULTI_STEEL_IDENTITY, TRUE
	special SteelSetPlayerIdentity
	msgbox Steel_School_Text_UseName, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, Steel_School_ConfirmName
	fadescreen FADE_TO_BLACK
	special SteelNamePlayer
Steel_School_ConfirmName:
	msgbox Steel_School_Text_ConfirmName, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, Steel_School_Identity
	msgbox Steel_School_Text_Dismissed, MSGBOX_DEFAULT
	msgbox Steel_School_Text_Kyle, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_STEEL_SCHOOL_KYLE, Steel_School_KyleExit
	applymovement OBJ_EVENT_ID_PLAYER, Steel_School_PlayerExit
	waitmovement LOCALID_STEEL_SCHOOL_KYLE
	waitmovement OBJ_EVENT_ID_PLAYER
	removeobject LOCALID_STEEL_SCHOOL_KYLE
	setvar VAR_STEEL_OPENING, STEEL_OPENING_DISMISSED
	special SteelSyncOpeningActors
	warp MAP_STEEL_ALUMINA_VILLAGE, 32, 8
	waitstate
	releaseall
	end


Steel_School_Teacher::
	msgbox Steel_School_Text_Dismissed, MSGBOX_NPC
	end

Steel_School_Kyle::
	msgbox Steel_School_Text_Kyle, MSGBOX_NPC
	end

Steel_School_Classmate::
	msgbox Steel_School_Text_Classmate, MSGBOX_NPC
	end

Steel_School_Text_LastDay:
	.string "TEACHER: One last roll call before\n"
	.string "Alumina School lets out for summer.\p"
	.string "Kyle? Present, of course.\n"
	.string "And your younger sibling?$"

Steel_School_Text_Identity:
	.string "Which are you?$"

Steel_School_Text_UseName:
	.string "TEACHER: {PLAYER}?\n"
	.string "Is that what you'd like to be called?$"

Steel_School_Text_ConfirmName:
	.string "TEACHER: {PLAYER}.\n"
	.string "Have I got that right?$"

Steel_School_Text_Dismissed:
	.string "TEACHER: That's everyone.\n"
	.string "Class dismissed!\p"
	.string "Take care on your way home.$"

Steel_School_Text_Kyle:
	.string "KYLE: Come on, younger sibling!\n"
	.string "Dad's waiting for us at home.\p"
	.string "Nine minutes older, and already\n"
	.string "nine minutes ahead of you!$"

Steel_School_Text_Classmate:
	.string "No more homework!\n"
	.string "I'm going to miss everyone, though.$"

Steel_AluminaSchool_SyncActors:
	special SteelSyncOpeningActors
	end
Steel_School_KyleExit:
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

Steel_School_PlayerExit:
	walk_down
	walk_down
	walk_down
	walk_down
	step_end
