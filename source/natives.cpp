/************************************************************************************************
SAMP-Anti-Cheat

This file is part of SAMP-Anti-Cheat

SAMP-Anti-Cheat is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Extensive PAWN Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Extensive PAWN Library.  If not, see <http://www.gnu.org/licenses/>.

Copyright (C) Yashas 2015
************************************************************************************************/
#include "SAMP-Anti-Cheat.h"

#include "natives.h"
#include "player.h"
#include "vehicle.h"
/************************************************************************************************/
cell AMX_NATIVE_CALL Natives::AC_SetPlayerSpecialAction(AMX* amx, cell* params) //playerid, actionid
{
	if (SetPlayerSpecialAction(params[1], params[2]))
	{
		if (PlayerList[params[1]] == nullptr) return true;

		if (params[2] == SPECIAL_ACTION_USEJETPACK)
			PlayerList[params[1]]->AntiCheatStatus[CheatType::JETPACK_CHEAT] = AntiCheatCommands::SKIP;
		return true;
	}	
	return false;
}
cell AMX_NATIVE_CALL Natives::AC_GivePlayerWeapon(AMX* amx, cell* params) //playerid, weaponid, ammo
{
	if (GivePlayerWeapon(params[1], params[2], params[3]))
	{
		if (PlayerList[params[1]] == nullptr) return true;
		
		Player * player = PlayerList[params[1]];
		int slotid = WeaponInformation::WeaponSlot[params[2]];		

		player->CheckPlayerForDesync(DesyncType::WEAPON_DESYNC);
		player->PreviousWeaponInLastModifiedSlot = player->WeaponID[slotid];
		player->LastModifiedWeaponSlot = slotid;
		player->WeaponID[slotid] = params[2];

		if (player->IsAntiCheatEnabled(CheatType::AMMO_CHEAT))
		{
			player->WeaponAmmo[slotid] += params[3];
			player->WarningCount[CheatType::AMMO_CHEAT] = 0;		
		}
		return true;
	}
	return false;
}
cell AMX_NATIVE_CALL Natives::AC_ResetPlayerWeapons(AMX* amx, cell* params) //playerid
{
	if (ResetPlayerWeapons(params[1]))
	{
		if (PlayerList[params[1]] == nullptr) return true;

		Player * player = PlayerList[params[1]];

		player->CheckPlayerForDesync(DesyncType::WEAPON_DESYNC);
		player->LastModifiedWeaponSlot = PLAYER_WEAPON_RESET;

		memcpy(player->PreviousWeaponID, player->WeaponID, MAX_WEAPON_SLOTS*sizeof(char));
		memset(player->WeaponID, INVALID_WEAPON_ID, MAX_WEAPON_SLOTS);

		if (player->IsAntiCheatEnabled(CheatType::AMMO_CHEAT))
		{
			player->WarningCount[CheatType::AMMO_CHEAT] = 0;
			memset(player->WeaponAmmo, 0, MAX_WEAPON_SLOTS * 4);
			player->AntiCheatStatus[CheatType::AMMO_CHEAT] = AntiCheatCommands::SKIP_CYCLE_THRICE;
		}
		return true;
	}	
	return false;
}
cell AMX_NATIVE_CALL Natives::AC_SetPlayerAmmo(AMX* amx, cell* params) //playerid, weaponid, ammo
{
	//SetPlayerAmmo doesn't work if the player does not have the weapon corresponding to the passed weapon id
	if (SetPlayerAmmo(params[1], params[2], params[3]))
	{
		if (PlayerList[params[1]] == nullptr) return true;

		int slotid = WeaponInformation::WeaponSlot[params[2]];
		Player * player = PlayerList[params[1]];

		if (player->IsAntiCheatEnabled(CheatType::AMMO_CHEAT))
		{
			if (player->WeaponID[slotid] == params[2])
			{
				player->WeaponAmmo[slotid] = params[3];
				player->LastModifiedWeaponSlot = slotid;
				player->WarningCount[CheatType::AMMO_CHEAT] = 0;
				player->AntiCheatStatus[CheatType::AMMO_CHEAT] = AntiCheatCommands::SKIP_CYCLE_ONCE;
			}
		}
		return true;
	}	
	return false; 
}
cell AMX_NATIVE_CALL Natives::AC_SetSpawnInfo(AMX* amx, cell* params) //playerid, team, skin, spawn_x, spawn_y, spawn_z, z_angle, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo
{
	if (params[1] < 0 && params[1] >= MAX_PLAYERS) return false;

	SetSpawnInfo(params[1], params[2], params[3], amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6]), amx_ctof(params[7]), params[8], params[9], params[10], params[11], params[12], params[13]);

	if (PlayerList[params[1]] == nullptr) return true;
	Player * player = PlayerList[params[1]];

	player->classid = INVALID_CLASSID;

	if (params[12] != 0 || params[10] != 0 || params[8] != 0)
	{			
		int weaponid = params[12];
		int slotid = WeaponInformation::WeaponSlot[weaponid];

		player->PlayerHasWeaponsOnSpawn = true;

		//If weapon3 and weapon2 have the same slot id, then weapon2 will be given 
		//Priority Order: 1>2>3

		memset(player->SpawnWeaponAmmo, 0, MAX_WEAPON_SLOTS*sizeof(int));
		memset(player->SpawnWeaponID, INVALID_WEAPON_ID, MAX_WEAPON_SLOTS*sizeof(char));

		player->SpawnWeaponID[slotid] = weaponid;
		player->SpawnWeaponAmmo[slotid] = params[13];

		weaponid = params[10];
		slotid = WeaponInformation::WeaponSlot[weaponid];

		player->SpawnWeaponID[slotid] = weaponid;
		player->SpawnWeaponAmmo[slotid] += params[11];

		weaponid = params[8];
		slotid = WeaponInformation::WeaponSlot[weaponid];

		player->SpawnWeaponID[slotid] = weaponid;
		player->SpawnWeaponAmmo[slotid] += params[9];
	}
	return true;
}
cell AMX_NATIVE_CALL Natives::AC_AddPlayerClass(AMX* amx, cell* params) //modelid, spawn_x, spawn_y, spawn_z, z_angle, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo
{
	int classid = AddPlayerClass(params[1], amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]), amx_ctof(params[5]), params[6], params[7], params[8], params[9], params[10], params[11]);

	bool classid_limit = (classid == 319);

	//If weapon3 and weapon2 have the same slot id, then weapon2 will be given 
	//Priority Order: 1>2>3

	if (params[10] != 0 || params[8] != 0 || params[6] != 0)
	{
		if (classid_limit) classid = 318;
		Class_HasWeapons[classid] = true;

		int weaponid = params[10];
		int slotid = WeaponInformation::WeaponSlot[weaponid];		

		memset(Class_SpawnWeaponAmmo[classid], 0, MAX_WEAPON_SLOTS*sizeof(int));
		memset(Class_SpawnWeaponID[classid], INVALID_WEAPON_ID, MAX_WEAPON_SLOTS*sizeof(char));

		Class_SpawnWeaponID[classid][slotid] = weaponid;
		Class_SpawnWeaponAmmo[classid][slotid] = params[11];

		weaponid = params[8];
		slotid = WeaponInformation::WeaponSlot[weaponid];

		Class_SpawnWeaponID[classid][slotid] = weaponid;
		Class_SpawnWeaponAmmo[classid][slotid] += params[9];

		weaponid = params[6];
		slotid = WeaponInformation::WeaponSlot[weaponid];

		Class_SpawnWeaponID[classid][slotid] = weaponid;
		Class_SpawnWeaponAmmo[classid][slotid] += params[7];
	
		if (classid_limit) return 319;
	}
	return classid;
}
cell AMX_NATIVE_CALL Natives::AC_AddPlayerClassEx(AMX* amx, cell* params) //teamid, modelid, spawn_x, spawn_y, spawn_z, z_angle, weapon1, weapon1_ammo, weapon2, weapon2_ammo, weapon3, weapon3_ammo
{
	int classid = AddPlayerClassEx(params[1], params[2], amx_ctof(params[3]), amx_ctof(params[4]), amx_ctof(params[5]), amx_ctof(params[6]), params[7], params[8], params[9], params[10], params[11], params[12]);

	bool classid_limit = (classid == 319);

	//If weapon3 and weapon2 have the same slot id, then weapon2 will be given 
	//Priority Order: 1>2>3

	if (params[11] != 0 || params[9] != 0 || params[7] != 0)
	{
		if (classid_limit) classid = 318;
		Class_HasWeapons[classid] = true;

		int weaponid = params[11];
		int slotid = WeaponInformation::WeaponSlot[weaponid];

		memset(Class_SpawnWeaponAmmo[classid], 0, MAX_WEAPON_SLOTS*sizeof(int));
		memset(Class_SpawnWeaponID[classid], INVALID_WEAPON_ID, MAX_WEAPON_SLOTS*sizeof(char));

		Class_SpawnWeaponID[classid][slotid] = weaponid;
		Class_SpawnWeaponAmmo[classid][slotid] = params[12];

		weaponid = params[9];
		slotid = WeaponInformation::WeaponSlot[weaponid];

		Class_SpawnWeaponID[classid][slotid] = weaponid;
		Class_SpawnWeaponAmmo[classid][slotid] += params[10];

		weaponid = params[7];
		slotid = WeaponInformation::WeaponSlot[weaponid];

		Class_SpawnWeaponID[classid][slotid] = weaponid;
		Class_SpawnWeaponAmmo[classid][slotid] += params[8];

		if (classid_limit) return 319;
	}
	return classid;
}
cell AMX_NATIVE_CALL Natives::AC_RepairVehicle(AMX* amx, cell* params) //vehicleid
{
	if (RepairVehicle(params[1]))
	{
		if (AntiCheatsEnabled & TwoPowerBit(CheatType::VEHICLE_REPAIR_CHEAT))
			VehicleList[params[1]]->health = 1000.0;
	}
	return false;
}
cell AMX_NATIVE_CALL Natives::AC_SetVehicleHealth(AMX* amx, cell* params) //vehicleid, health
{
	if (SetVehicleHealth(params[1], amx_ctof(params[2])))
	{
		if (AntiCheatsEnabled & TwoPowerBit(CheatType::VEHICLE_REPAIR_CHEAT))
			VehicleList[params[1]]->health = amx_ctof(params[2]);
		return true;
	}
	return false;
}
cell AMX_NATIVE_CALL Natives::AC_CreateVehicle(AMX* amx, cell* params)
{
	int vehicleid = CreateVehicle(params[1], amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]), amx_ctof(params[5]), params[6], params[7], params[8], !!params[9]);

	if (vehicleid == INVALID_VEHICLE_ID) return INVALID_VEHICLE_ID;

	if (VehicleList[vehicleid] != nullptr) delete VehicleList[vehicleid];
	VehicleList[vehicleid] = new Vehicle(vehicleid, params[1]);

	return vehicleid;
}
cell AMX_NATIVE_CALL Natives::AC_AddStaticVehicle(AMX* amx, cell* params)
{
	int vehicleid = AddStaticVehicle(params[1], amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]), amx_ctof(params[5]), params[6], params[7]);

	if (vehicleid == INVALID_VEHICLE_ID) return INVALID_VEHICLE_ID;

	if (VehicleList[vehicleid] != nullptr) delete VehicleList[vehicleid];
	VehicleList[vehicleid] = new Vehicle(vehicleid, params[1]);

	return vehicleid;
}
cell AMX_NATIVE_CALL Natives::AC_AddStaticVehicleEx(AMX* amx, cell* params) 
{
	int vehicleid = AddStaticVehicleEx(params[1], amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4]), amx_ctof(params[5]), params[6], params[7], params[8], !!params[9]);

	if (vehicleid == INVALID_VEHICLE_ID) return INVALID_VEHICLE_ID;
	VehicleList[vehicleid] = new Vehicle(vehicleid, params[1]);

	return vehicleid;
}
cell AMX_NATIVE_CALL Natives::AC_DestroyVehicle(AMX* amx, cell* params) //vehicleid
{
	int vehicleid = params[1];
	if (DestroyVehicle(vehicleid))
	{
		if (VehicleList[vehicleid] != nullptr)
			delete VehicleList[vehicleid];
		return true;
	}
	return false;
}
cell AMX_NATIVE_CALL Natives::AC_SetPlayerPos(AMX* amx, cell* params) //playerid, x, y, z
{
	if (SetPlayerPos(params[1], amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4])))
	{
		PlayerList[params[1]]->vehicleid = INVALID_VEHICLE_ID;
		return true;
	}		
	return false;
}
cell AMX_NATIVE_CALL Natives::AC_SetPlayerPosFindZ(AMX* amx, cell* params) //playerid, x, y, z
{
	if (SetPlayerPosFindZ(params[1], amx_ctof(params[2]), amx_ctof(params[3]), amx_ctof(params[4])))
	{
		PlayerList[params[1]]->vehicleid = INVALID_VEHICLE_ID;
		return true;
	}
	return false;
}
cell AMX_NATIVE_CALL Natives::AC_PutPlayerInVehicle(AMX* amx, cell* params) //playerid, vehicleid, seatid
{
	if (PutPlayerInVehicle(params[1], params[2], params[3]))
	{
		PlayerList[params[1]]->vehicleid = params[2];
		return true;
	}
	return false;
}