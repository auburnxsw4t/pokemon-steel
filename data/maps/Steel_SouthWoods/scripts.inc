Steel_SouthWoods_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, Steel_SouthWoods_OnLoad
	.byte 0

Steel_SouthWoods_OnLoad:
	special SteelSyncOpeningActors
	goto_if_ge VAR_STEEL_OPENING, STEEL_OPENING_COMPLETE, Steel_SouthWoods_Open
	lockall
	msgbox Steel_SouthWoods_Text_Gated, MSGBOX_DEFAULT
	closemessage
	warp MAP_STEEL_ALUMINA_VILLAGE, 15, 28
	waitstate
	releaseall
	end

Steel_SouthWoods_Open:
	end

Steel_SouthWoods_Sign::
	msgbox Steel_SouthWoods_Text_Sign, MSGBOX_SIGN
	end

Steel_SouthWoods_Text_Gated:
	.string "The South Woods trail is still closed.\n"
	.string "Come back when the League route opens.$"

Steel_SouthWoods_Text_Sign:
	.string "SOUTH WOODS\nA shared trail into the wild.$"
