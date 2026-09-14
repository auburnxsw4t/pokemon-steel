Steel_SouthTrail_MapScripts::
	.byte 0

Steel_SouthTrail_ToSouthWoods::
	warp MAP_STEEL_SOUTH_WOODS, 18, 57
	waitstate
	end

Steel_SouthTrail_ToRoute1::
	warp MAP_STEEL_ROUTE1, 18, 3
	waitstate
	end

Steel_SouthTrail_Sign::
	msgbox Steel_SouthTrail_Text_Sign, MSGBOX_SIGN
	end

Steel_SouthTrail_Text_Sign:
	.string "NORTH: SOUTHWOODS\nSOUTH: ROUTE 1$"
