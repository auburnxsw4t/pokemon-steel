Steel_SouthWoods_MapScripts::
	.byte 0

Steel_SouthWoods_ToHomestead::
	warp MAP_STEEL_HOMESTEAD_RIDGE, 7, 25
	waitstate
	end

Steel_SouthWoods_ToAlumina::
	warp MAP_STEEL_ALUMINA_VILLAGE, 28, 29
	waitstate
	end

Steel_SouthWoods_ToSouthTrail::
	warp MAP_STEEL_SOUTH_TRAIL, 18, 2
	waitstate
	end

Steel_SouthWoods_TrainerT2::
	trainerbattle_single TRAINER_STEEL_SOUTHWOODS_FINN, Steel_SouthWoods_Text_FinnIntro, Steel_SouthWoods_Text_FinnDefeated
	msgbox Steel_SouthWoods_Text_FinnAfter, MSGBOX_AUTOCLOSE
	end

Steel_SouthWoods_TrainerT3::
	trainerbattle_single TRAINER_STEEL_SOUTHWOODS_WADE, Steel_SouthWoods_Text_WadeIntro, Steel_SouthWoods_Text_WadeDefeated
	msgbox Steel_SouthWoods_Text_WadeAfter, MSGBOX_AUTOCLOSE
	end

Steel_SouthWoods_TrainerT5::
	trainerbattle_single TRAINER_STEEL_SOUTHWOODS_JUNE, Steel_SouthWoods_Text_JuneIntro, Steel_SouthWoods_Text_JuneDefeated
	msgbox Steel_SouthWoods_Text_JuneAfter, MSGBOX_AUTOCLOSE
	end

Steel_SouthWoods_ClearingSign::
	msgbox Steel_SouthWoods_Text_Clearing, MSGBOX_SIGN
	end

Steel_SouthWoods_LoggingSign::
	msgbox Steel_SouthWoods_Text_Logging, MSGBOX_SIGN
	end

Steel_SouthWoods_Text_FinnIntro:
	.string "Every trail has its own bugs!$"
Steel_SouthWoods_Text_FinnDefeated:
	.string "They found the better path.$"
Steel_SouthWoods_Text_FinnAfter:
	.string "Grass makes a fine shortcut--\nif you're ready for company.$"
Steel_SouthWoods_Text_WadeIntro:
	.string "You made it past the clearing?\nThen let's battle!$"
Steel_SouthWoods_Text_WadeDefeated:
	.string "These woods test everybody.$"
Steel_SouthWoods_Text_WadeAfter:
	.string "The creek is south of here.\nListen for the water.$"
Steel_SouthWoods_Text_JuneIntro:
	.string "One last battle before the trail!$"
Steel_SouthWoods_Text_JuneDefeated:
	.string "You earned the way through.$"
Steel_SouthWoods_Text_JuneAfter:
	.string "South Trail leads to ROUTE 1.$"
Steel_SouthWoods_Text_Clearing:
	.string "SOUTHWOODS CENTRAL CLEARING\nHomestead west--Alumina east$"
Steel_SouthWoods_Text_Logging:
	.string "Old logging marks fade into\nthe newer forest growth.$"
