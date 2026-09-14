Steel_LeagueRegistration_MapScripts::
	.byte 0

Steel_Registration_Attendant::
	lock
	faceplayer
	goto_if_eq VAR_STEEL_REGISTRATION, STEEL_REGISTRATION_COMPLETE, Steel_Registration_AlreadyComplete
	msgbox Steel_Registration_Text_TemporaryRegistration, MSGBOX_DEFAULT
	setvar VAR_STEEL_REGISTRATION, STEEL_REGISTRATION_COMPLETE
	setflag FLAG_STEEL_LEAGUE_REGISTERED
	setflag FLAG_HIDE_STEEL_KYLE_REGISTRATION
	release
	end

Steel_Registration_AlreadyComplete::
	msgbox Steel_Registration_Text_AlreadyComplete, MSGBOX_DEFAULT
	release
	end

Steel_Registration_UpperFloorClosed::
	msgbox Steel_Registration_Text_UpperFloorClosed, MSGBOX_NPC
	end

Steel_Registration_Text_TemporaryRegistration:
	.string "TEMPORARY REGISTRATION SCRIPT\n"
	.string "Your LEAGUE entry is recorded.\p"
	.string "Approved dialogue will replace\n"
	.string "this development text.$"

Steel_Registration_Text_AlreadyComplete:
	.string "Your LEAGUE registration is\n"
	.string "already recorded.$"

Steel_Registration_Text_UpperFloorClosed:
	.string "Upper offices are closed during\n"
	.string "this playtest.$"
