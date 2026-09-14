Steel_LeagueRegistration_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, Steel_Registration_OnLoad
	map_script MAP_SCRIPT_ON_FRAME_TABLE, Steel_Registration_OnFrame
	.byte 0

Steel_Registration_OnLoad:
	special SteelSyncOpeningActors
	end

Steel_Registration_OnFrame:
	map_script_2 VAR_STEEL_REGISTRATION, STEEL_REGISTRATION_ACTIVE, Steel_Registration_Scene
	.2byte 0

Steel_Registration_Scene::
	lockall
	msgbox Steel_Registration_Text_KyleAlready, MSGBOX_DEFAULT
	msgbox Steel_Registration_Text_KyleLeaves, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_STEEL_REGISTRATION_KYLE, Steel_Registration_KyleExit
	waitmovement LOCALID_STEEL_REGISTRATION_KYLE
	setvar VAR_STEEL_REGISTRATION, STEEL_REGISTRATION_KYLE_DONE
	setflag FLAG_HIDE_STEEL_KYLE_REGISTRATION
	removeobject LOCALID_STEEL_REGISTRATION_KYLE
	special SteelSyncOpeningActors
	releaseall
	end

Steel_Registration_Attendant::
	lock
	faceplayer
	goto_if_eq VAR_STEEL_REGISTRATION, STEEL_REGISTRATION_COMPLETE, Steel_Registration_AlreadyComplete
	goto_if_ne VAR_STEEL_REGISTRATION, STEEL_REGISTRATION_KYLE_DONE, Steel_Registration_NotReady
	msgbox Steel_Registration_Text_ClerkNext, MSGBOX_DEFAULT
	msgbox Steel_Registration_Text_ClerkCleared, MSGBOX_DEFAULT
	giveitem ITEM_FERRANE_TRAINER_LICENSE
	goto_if_eq VAR_RESULT, FALSE, Steel_Registration_NoBagRoom
	setvar VAR_STEEL_REGISTRATION, STEEL_REGISTRATION_COMPLETE
	setflag FLAG_STEEL_LEAGUE_REGISTERED
	msgbox Steel_Registration_Text_ClerkOakridge, MSGBOX_DEFAULT
	release
	end

Steel_Registration_NotReady::
	msgbox Steel_Registration_Text_NotReady, MSGBOX_DEFAULT
	release
	end

Steel_Registration_NoBagRoom::
	msgbox Steel_Registration_Text_NoBagRoom, MSGBOX_DEFAULT
	release
	end

Steel_Registration_AlreadyComplete::
	msgbox Steel_Registration_Text_AlreadyComplete, MSGBOX_DEFAULT
	release
	end

Steel_Registration_UpperFloorClosed::
	msgbox Steel_Registration_Text_UpperFloorClosed, MSGBOX_NPC
	end

Steel_Registration_Kyle::
	msgbox Steel_Registration_Text_KyleWaiting, MSGBOX_NPC
	end

Steel_Registration_KyleExit:
	walk_left
	walk_left
	walk_down
	walk_down
	walk_down
	walk_down
	step_end

Steel_Registration_Text_KyleAlready:
	.string "KYLE: Took you long enough.\n"
	.string "I'm already registered.$"

Steel_Registration_Text_ClerkNext:
	.string "CLERK: Next trainer, please.\n"
	.string "{PLAYER}, right?$"

Steel_Registration_Text_ClerkCleared:
	.string "CLERK: All set.\n"
	.string "You're cleared for the\l"
	.string "Ferrane Gym Challenge.$"

Steel_Registration_Text_ClerkOakridge:
	.string "CLERK: Your first Gym is in\n"
	.string "Oakridge.\p"
	.string "Head south through\n"
	.string "Longleaf Hollow.$"

Steel_Registration_Text_KyleLeaves:
	.string "KYLE: See? Easy.\n"
	.string "Try not to reach Oakridge\l"
	.string "after I win.$"

Steel_Registration_Text_NotReady:
	.string "CLERK: Registration opens after\n"
	.string "new trainers receive a partner.$"

Steel_Registration_Text_NoBagRoom:
	.string "CLERK: Please make room in your\n"
	.string "KEY ITEMS pocket.$"

Steel_Registration_Text_AlreadyComplete:
	.string "CLERK: Good luck, Trainer.$"

Steel_Registration_Text_KyleWaiting:
	.string "KYLE: Hurry up and register.$"

Steel_Registration_Text_UpperFloorClosed:
	.string "Upper offices are closed during\n"
	.string "this playtest.$"
