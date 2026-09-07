Steel_HomesteadRidge_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, Steel_Ridge_OnLoad
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Steel_Ridge_OnFrame
	.byte 0
Steel_Ridge_OnLoad:
	special SteelSyncOpeningActors
	end
Steel_Ridge_OnFrame:
	map_script_2 VAR_STEEL_OPENING, STEEL_OPENING_RIDGE_HOME, Steel_Ridge_EscortHome
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
