Steel_HomesteadRidge_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, Steel_Ridge_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, Steel_Ridge_Positions
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Steel_Ridge_OnFrame
	.byte 0
Steel_Ridge_OnLoad:
	special SteelSyncOpeningActors
	end
Steel_Ridge_OnFrame:
	map_script_2 VAR_STEEL_OPENING, STEEL_OPENING_RIDGE_HOME, Steel_Ridge_EscortHome
	map_script_2 VAR_STEEL_OPENING, STEEL_OPENING_RIDGE_CREEK, Steel_Ridge_ToCreek
	map_script_2 VAR_STEEL_OPENING, STEEL_OPENING_RIDGE_RETURN, Steel_Ridge_Return
	.2byte 0
Steel_Ridge_EscortHome::
	lockall
	closemessage
	applymovement LOCALID_STEEL_RIDGE_KYLE, Steel_Ridge_KyleToHouse
	applymovement OBJ_EVENT_ID_PLAYER, Steel_Ridge_PlayerToHouse
	waitmovement LOCALID_STEEL_RIDGE_KYLE
	waitmovement OBJ_EVENT_ID_PLAYER
	msgbox Steel_Ridge_Text_Home, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_STEEL_RIDGE_KYLE, Steel_Ridge_KyleToDoor
	applymovement OBJ_EVENT_ID_PLAYER, Steel_Ridge_PlayerToDoor
	waitmovement LOCALID_STEEL_RIDGE_KYLE
	waitmovement OBJ_EVENT_ID_PLAYER
	opendoor 8, 11
	waitdooranim
	applymovement LOCALID_STEEL_RIDGE_KYLE, Common_Movement_WalkUp
	waitmovement LOCALID_STEEL_RIDGE_KYLE
	removeobject LOCALID_STEEL_RIDGE_KYLE
	applymovement OBJ_EVENT_ID_PLAYER, Common_Movement_WalkLeft
	waitmovement OBJ_EVENT_ID_PLAYER
	setvar VAR_STEEL_OPENING, STEEL_OPENING_HOME
	special SteelSyncOpeningActors
	warp MAP_STEEL_FAMILY_HOME, 9, 7
	waitstate
	releaseall
	end

Steel_Ridge_KyleToHouse:
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
	walk_left
	walk_left
	walk_left
	step_end

Steel_Ridge_PlayerToHouse:
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
	walk_left
	walk_left
	walk_left
	step_end

Steel_Ridge_KyleToDoor:
	walk_up
	walk_up
	walk_up
	walk_left
	step_end

Steel_Ridge_PlayerToDoor:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_left
	step_end

Steel_Ridge_Kyle::
	msgbox Steel_Ridge_Text_Home, MSGBOX_SIGN
	end

Steel_Ridge_HomeSign::
	msgbox Steel_Ridge_Text_Family, MSGBOX_SIGN
	end

Steel_Ridge_GardenSign::
	msgbox Steel_Ridge_Text_Garden, MSGBOX_SIGN
	end

Steel_Ridge_WoodsSign::
	msgbox Steel_Ridge_Text_Woods, MSGBOX_SIGN
	end

Steel_Ridge_VillageSign::
	msgbox Steel_Ridge_Text_Village, MSGBOX_SIGN
	end

Steel_Ridge_Text_Home:
	.string "KYLE: There's our house.\n"
	.string "Come on. Let's see what Dad caught!$"

Steel_Ridge_Text_Family:
	.string "LOGAN and MADISON's home$"

Steel_Ridge_Text_Garden:
	.string "HOMESTEAD COMMUNITY GARDEN$"

Steel_Ridge_Text_Woods:
	.string "West: CREEKSIDE WOODS$"

Steel_Ridge_Text_Village:
	.string "East: ALUMINA VILLAGE$"

Steel_Ridge_Positions:
	goto_if_eq VAR_STEEL_OPENING, STEEL_OPENING_RIDGE_CREEK, Steel_Ridge_OutPositions
	goto_if_eq VAR_STEEL_OPENING, STEEL_OPENING_RIDGE_RETURN, Steel_Ridge_BackPositions
	end
Steel_Ridge_OutPositions:
	setobjectxyperm LOCALID_STEEL_RIDGE_KYLE, 8, 12
	setobjectxyperm LOCALID_STEEL_RIDGE_LOGAN, 7, 12
	end
Steel_Ridge_BackPositions:
	setobjectxyperm LOCALID_STEEL_RIDGE_LOGAN, 2, 15
	setobjectxyperm LOCALID_STEEL_RIDGE_KYLE, 3, 16
	end
Steel_Ridge_ToCreek::
	lockall
	closemessage
	applymovement LOCALID_STEEL_RIDGE_LOGAN, Steel_Ridge_Out0
	applymovement LOCALID_STEEL_RIDGE_KYLE, Steel_Ridge_Out1
	applymovement OBJ_EVENT_ID_PLAYER, Steel_Ridge_Out2
	waitmovement LOCALID_STEEL_RIDGE_LOGAN
	waitmovement LOCALID_STEEL_RIDGE_KYLE
	waitmovement OBJ_EVENT_ID_PLAYER
	msgbox Steel_Ridge_Text_Close, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_STEEL_RIDGE_LOGAN, Steel_Ridge_West0
	applymovement LOCALID_STEEL_RIDGE_KYLE, Steel_Ridge_West1
	applymovement OBJ_EVENT_ID_PLAYER, Steel_Ridge_West2
	waitmovement LOCALID_STEEL_RIDGE_LOGAN
	waitmovement LOCALID_STEEL_RIDGE_KYLE
	waitmovement OBJ_EVENT_ID_PLAYER
	setvar VAR_STEEL_OPENING, STEEL_OPENING_CATCHING
	special SteelSyncOpeningActors
	warp MAP_STEEL_CATCHING_WOODS, 24, 17
	waitstate
	releaseall
	end

Steel_Ridge_Out0:
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

Steel_Ridge_Out1:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

Steel_Ridge_Out2:
	walk_down
	walk_down
	walk_down
	walk_down
	walk_down
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

Steel_Ridge_West0:
	walk_left
	step_end

Steel_Ridge_West1:
	walk_left
	step_end

Steel_Ridge_West2:
	walk_left
	step_end

Steel_Ridge_Return::
	lockall
	closemessage
	applymovement LOCALID_STEEL_RIDGE_LOGAN, Steel_Ridge_Back0
	applymovement LOCALID_STEEL_RIDGE_KYLE, Steel_Ridge_Back1
	applymovement OBJ_EVENT_ID_PLAYER, Steel_Ridge_Back2
	waitmovement LOCALID_STEEL_RIDGE_LOGAN
	waitmovement LOCALID_STEEL_RIDGE_KYLE
	waitmovement OBJ_EVENT_ID_PLAYER
	msgbox Steel_Ridge_Text_Return, MSGBOX_DEFAULT
	closemessage
	setvar VAR_STEEL_OPENING, STEEL_OPENING_HOME_RETURN
	special SteelSyncOpeningActors
	warp MAP_STEEL_FAMILY_HOME, 9, 7
	waitstate
	releaseall
	end

Steel_Ridge_Back0:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_up
	walk_up
	walk_up
	step_end

Steel_Ridge_Back1:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

Steel_Ridge_Back2:
	walk_right
	walk_right
	walk_right
	walk_right
	walk_right
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	step_end

Steel_Ridge_Text_Close:
	.string "LOGAN: Stay close, both of you.\n"
	.string "The clearing is just ahead.$"
Steel_Ridge_Text_Return:
	.string "LOGAN: Let's head inside.$"
