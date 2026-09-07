Steel_FamilyHome_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, Steel_FamilyHome_SyncActors
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Steel_Home_OnFrame
	.byte 0

Steel_Home_OnFrame:
	map_script_2 VAR_STEEL_OPENING, STEEL_OPENING_HOME, Steel_Home_Arrival
	.2byte 0

Steel_Home_Arrival::
	lockall
	msgbox Steel_Home_Text_Logan, MSGBOX_DEFAULT
	setvar VAR_STEEL_OPENING, STEEL_OPENING_CREEK_NEXT
	releaseall
	end

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
