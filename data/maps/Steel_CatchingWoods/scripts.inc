Steel_CatchingWoods_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, Steel_Woods_OnLoad
	.byte 0
Steel_Woods_OnLoad:
	special SteelSyncOpeningActors
	end
Steel_Woods_Sign::
	msgbox Steel_Woods_Text_Sign, MSGBOX_SIGN
	end
Steel_Woods_Text_Sign:
	.string "CREEKSIDE WOODS$"
