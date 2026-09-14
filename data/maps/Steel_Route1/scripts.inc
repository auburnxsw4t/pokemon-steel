Steel_Route1_MapScripts::
	.byte 0

Steel_Route1_ToSouthTrail::
	warp MAP_STEEL_SOUTH_TRAIL, 18, 9
	waitstate
	end

Steel_Route1_TrainerT1::
	trainerbattle_single TRAINER_STEEL_ROUTE1_ELI, Steel_Route1_Text_EliIntro, Steel_Route1_Text_EliDefeated
	msgbox Steel_Route1_Text_EliAfter, MSGBOX_AUTOCLOSE
	end
Steel_Route1_TrainerT3::
	trainerbattle_single TRAINER_STEEL_ROUTE1_REED, Steel_Route1_Text_ReedIntro, Steel_Route1_Text_ReedDefeated
	msgbox Steel_Route1_Text_ReedAfter, MSGBOX_AUTOCLOSE
	end
Steel_Route1_TrainerT5::
	trainerbattle_single TRAINER_STEEL_ROUTE1_CLAY, Steel_Route1_Text_ClayIntro, Steel_Route1_Text_ClayDefeated
	msgbox Steel_Route1_Text_ClayAfter, MSGBOX_AUTOCLOSE
	end
Steel_Route1_TrainerT7::
	trainerbattle_single TRAINER_STEEL_ROUTE1_TESS, Steel_Route1_Text_TessIntro, Steel_Route1_Text_TessDefeated
	msgbox Steel_Route1_Text_TessAfter, MSGBOX_AUTOCLOSE
	end
Steel_Route1_TrainerT8::
	trainerbattle_single TRAINER_STEEL_ROUTE1_COLE, Steel_Route1_Text_ColeIntro, Steel_Route1_Text_ColeDefeated
	msgbox Steel_Route1_Text_ColeAfter, MSGBOX_AUTOCLOSE
	end

Steel_Route1_NorthSign::
	msgbox Steel_Route1_Text_NorthSign, MSGBOX_SIGN
	end
Steel_Route1_MineSign::
	msgbox Steel_Route1_Text_MineSign, MSGBOX_SIGN
	end
Steel_Route1_OakridgeSign::
	msgbox Steel_Route1_Text_OakridgeSign, MSGBOX_SIGN
	end

Steel_Route1_Text_EliIntro:
	.string "First trip down ROUTE 1?\nLet's see your new partner!$"
Steel_Route1_Text_EliDefeated:
	.string "That's a strong start.$"
Steel_Route1_Text_EliAfter:
	.string "The grass gets thicker ahead.$"
Steel_Route1_Text_ReedIntro:
	.string "The old mine trail makes\na perfect training ground!$"
Steel_Route1_Text_ReedDefeated:
	.string "Back to field notes.$"
Steel_Route1_Text_ReedAfter:
	.string "The mine is sealed tight.\nThe river route is safer.$"
Steel_Route1_Text_ClayIntro:
	.string "Crossing the river?\nEarn the bridge!$"
Steel_Route1_Text_ClayDefeated:
	.string "Go on across.$"
Steel_Route1_Text_ClayAfter:
	.string "That bridge is the only dry way.$"
Steel_Route1_Text_TessIntro:
	.string "Oakridge trainers don't coast!$"
Steel_Route1_Text_TessDefeated:
	.string "You kept your momentum.$"
Steel_Route1_Text_TessAfter:
	.string "The last field is just south.$"
Steel_Route1_Text_ColeIntro:
	.string "One more battle before Oakridge!$"
Steel_Route1_Text_ColeDefeated:
	.string "You're ready for town.$"
Steel_Route1_Text_ColeAfter:
	.string "Oakridge is next.\nThe road opens after this review.$"
Steel_Route1_Text_NorthSign:
	.string "ROUTE 1\nNorth: SOUTH TRAIL$"
Steel_Route1_Text_MineSign:
	.string "OLD FERRANE MINE\nClosed--unstable workings$"
Steel_Route1_Text_OakridgeSign:
	.string "OAKRIDGE TOWN lies south.\nEnd of the current playtest.$"
