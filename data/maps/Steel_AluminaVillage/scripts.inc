Steel_AluminaVillage_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, Steel_Village_OnLoad
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Steel_Village_OnFrame
	.byte 0
Steel_Village_OnLoad:
	special SteelSyncOpeningActors
	end
Steel_Village_OnFrame:
	map_script_2 VAR_STEEL_OPENING, STEEL_OPENING_DISMISSED, Steel_Village_Escort
	.2byte 0
Steel_Village_Escort::
	lockall
	msgbox Steel_Village_Text_League, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_STEEL_VILLAGE_KYLE, Steel_Village_KyleToGreen
	applymovement OBJ_EVENT_ID_PLAYER, Steel_Village_PlayerToGreen
	waitmovement LOCALID_STEEL_VILLAGE_KYLE
	waitmovement OBJ_EVENT_ID_PLAYER
	msgbox Steel_Village_Text_Race, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_STEEL_VILLAGE_KYLE, Steel_Village_KyleWest
	applymovement OBJ_EVENT_ID_PLAYER, Steel_Village_PlayerWest
	waitmovement LOCALID_STEEL_VILLAGE_KYLE
	waitmovement OBJ_EVENT_ID_PLAYER
	msgbox Steel_Village_Text_Home, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_STEEL_VILLAGE_KYLE, Steel_Village_LastSteps
	applymovement OBJ_EVENT_ID_PLAYER, Steel_Village_LastSteps
	waitmovement LOCALID_STEEL_VILLAGE_KYLE
	waitmovement OBJ_EVENT_ID_PLAYER
	removeobject LOCALID_STEEL_VILLAGE_KYLE
	setvar VAR_STEEL_OPENING, STEEL_OPENING_RIDGE_HOME
	special SteelSyncOpeningActors
	warp MAP_STEEL_HOMESTEAD_RIDGE, 33, 16
	waitstate
	releaseall
	end

Steel_Village_KyleToGreen:
	walk_down
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
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

Steel_Village_PlayerToGreen:
	walk_down
	walk_down
	walk_down
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
	walk_left
	walk_left
	walk_left
	walk_left
	walk_left
	step_end

Steel_Village_KyleWest:
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

Steel_Village_PlayerWest:
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

Steel_Village_LastSteps:
	walk_left
	walk_left
	step_end

Steel_Village_Kyle::
	msgbox Steel_Village_Text_League, MSGBOX_SIGN
	end

Steel_Village_SchoolSign::
	msgbox Steel_Village_Text_School, MSGBOX_SIGN
	end

Steel_Village_TownSign::
	msgbox Steel_Village_Text_Town, MSGBOX_SIGN
	end

Steel_Village_CenterSign::
	msgbox Steel_Village_Text_Center, MSGBOX_SIGN
	end

Steel_Village_MartSign::
	msgbox Steel_Village_Text_Mart, MSGBOX_SIGN
	end

Steel_Village_LeagueSign::
	msgbox Steel_Village_Text_Registration, MSGBOX_SIGN
	end

Steel_Village_ServiceClosed::
	msgbox Steel_Village_Text_Closed, MSGBOX_SIGN
	end

Steel_Village_Text_League:
	.string "KYLE: See that registration office?\n"
	.string "The Ferrane League starts there!\p"
	.string "First, we need our POKéMON.\n"
	.string "Dad's waiting at home.$"

Steel_Village_Text_Race:
	.string "KYLE: I'm going to take on the League\n"
	.string "before you even get warmed up.\p"
	.string "Nine minutes older, remember?\n"
	.string "Try to keep up, younger sibling!$"

Steel_Village_Text_Home:
	.string "KYLE: Homestead Ridge is just west\n"
	.string "of here. We're nearly home.$"

Steel_Village_Text_School:
	.string "ALUMINA SCHOOL$"

Steel_Village_Text_Town:
	.string "ALUMINA VILLAGE$"

Steel_Village_Text_Center:
	.string "POKéMON CENTER$"

Steel_Village_Text_Mart:
	.string "POKéMON MART$"

Steel_Village_Text_Registration:
	.string "FERRANE LEAGUE REGISTRATION$"

Steel_Village_Text_Closed:
	.string "Back soon.$"
