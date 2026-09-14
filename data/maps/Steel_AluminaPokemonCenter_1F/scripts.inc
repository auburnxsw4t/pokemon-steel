Steel_AluminaPokemonCenter_1F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Steel_AluminaCenter_OnTransition
	.byte 0

Steel_AluminaCenter_OnTransition:
	setrespawn HEAL_LOCATION_STEEL_ALUMINA_VILLAGE
	end

Steel_AluminaCenter_Nurse::
	setvar VAR_0x800B, LOCALID_STEEL_ALUMINA_NURSE
	call Common_EventScript_PkmnCenterNurse
	waitmessage
	waitbuttonpress
	release
	end
