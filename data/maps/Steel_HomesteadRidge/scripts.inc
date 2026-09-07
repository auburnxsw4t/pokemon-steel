Steel_HomesteadRidge_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, Steel_HomesteadRidge_SyncActors
	map_script MAP_SCRIPT_ON_TRANSITION, Steel_Ridge_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Steel_Ridge_OnFrame
	.byte 0

Steel_Ridge_OnTransition:
	goto_if_ge VAR_STEEL_OPENING, STEEL_OPENING_HOME, Steel_Ridge_HideKyle
	end

Steel_Ridge_HideKyle:
	removeobject LOCALID_STEEL_RIDGE_KYLE
	end

Steel_Ridge_OnFrame:
	map_script_2 VAR_STEEL_OPENING, STEEL_OPENING_DISMISSED, Steel_Ridge_WalkHome
	.2byte 0

Steel_Ridge_WalkHome::
	lockall
	msgbox Steel_Ridge_Text_Kyle, MSGBOX_DEFAULT
	applymovement LOCALID_STEEL_RIDGE_KYLE, Steel_Ridge_Movement_KyleHome
	waitmovement 0
	removeobject LOCALID_STEEL_RIDGE_KYLE
	applymovement OBJ_EVENT_ID_PLAYER, Steel_Ridge_Movement_PlayerHome
	waitmovement 0
	setvar VAR_STEEL_OPENING, STEEL_OPENING_HOME
	special SteelSyncOpeningActors
	releaseall
	end

Steel_Ridge_Movement_KyleHome:
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	step_end

Steel_Ridge_Movement_PlayerHome:
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	walk_up
	walk_left
	step_end

Steel_Ridge_Kyle::
	msgbox Steel_Ridge_Text_Kyle, MSGBOX_NPC
	end

Steel_Ridge_Neighbor::
	msgbox Steel_Ridge_Text_Neighbor, MSGBOX_NPC
	end

Steel_Ridge_RoadClosed::
	lockall
	msgbox Steel_Ridge_Text_RoadClosed, MSGBOX_DEFAULT
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkDown
	waitmovement 0
	releaseall
	end

Steel_Ridge_Text_Kyle:
	.string "KYLE: The Ferrane League won't know\n"
	.string "what hit it when I get there!\p"
	.string "You can try to keep up.\n"
	.string "Come on, let's head home.$"

Steel_Ridge_Text_Neighbor:
	.string "Welcome home to Homestead Ridge.\n"
	.string "Logan and Madison are waiting for you.$"

Steel_Ridge_Text_RoadClosed:
	.string "I should meet up with Dad before\n"
	.string "heading out of the neighborhood.$"

Steel_HomesteadRidge_SyncActors:
	special SteelSyncOpeningActors
	end
