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
	msgbox Steel_Home_Text_Logan, MSGBOX_NPC
	end

Steel_Home_Madison::
	msgbox Steel_Home_Text_Madison, MSGBOX_NPC
	end

Steel_Home_Kyle::
	msgbox Steel_Home_Text_Kyle, MSGBOX_NPC
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
	setvar VAR_STEEL_OPENING, STEEL_OPENING_STARTER_CHOICE
	releaseall
	end
Steel_Home_Text_Return:
	.string "LOGAN: All three are ready now.\n"
	.string "Come talk to me when you've chosen.$"
