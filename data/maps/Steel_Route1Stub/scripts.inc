Steel_Route1Stub_MapScripts::
	.byte 0

Steel_Route1_Blocker::
	lock
	faceplayer
	msgbox Steel_Route1_Text_Blocker, MSGBOX_DEFAULT
	release
	end

Steel_Route1_Sign::
	msgbox Steel_Route1_Text_Sign, MSGBOX_SIGN
	end

Steel_Route1_Text_Blocker:
	.string "ROUTE 1 is still under construction.\n"
	.string "The League road crew will open it later.$"

Steel_Route1_Text_Sign:
	.string "ROUTE 1\nEast road to the wider Ferrane region.$"
