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

#include "player.h"
#include "vehicle.h"
#include "interface.h"

#include <list>
#include <string>

using namespace std;
/***********************************************************************************************/
PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerConnect(int playerid)
{
	if (IsPlayerNPC(playerid))
	{
		if (PlayerList[playerid] != nullptr) delete PlayerList[playerid];
		return true;
	}	
	PlayerList[playerid] = new Player(playerid);
	return true;
}
PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerDisconnect(int playerid, int reason)
{
	if (PlayerList[playerid] != nullptr) delete PlayerList[playerid];
	PlayerList[playerid] = nullptr;
	return true;
}
PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerUpdate(int playerid)
{
	if (PlayerList[playerid] == nullptr) return true;
	PlayerList[playerid]->NoUpdateSinceLastCycle = false;

	if (PlayerList[playerid]->IsAntiCheatEnabled(CheatType::VEHICLE_TROLL_CHEAT))
	{
		switch (PlayerList[playerid]->AntiCheatStatus[CheatType::VEHICLE_TROLL_CHEAT])
		{
			case AntiCheatCommands::CHECK:
			{
				int vehicleid = GetPlayerVehicleID(playerid);
				if (vehicleid != 0)
				{
					if (PlayerList[playerid]->vehicleid != vehicleid)
					{
						int tick = GetTickCount();
						if (tick - PlayerList[playerid]->LastVehicleChangeTick < MIN_VEHICLE_SWITCH_TIME)
						{
							if (++(PlayerList[playerid]->WarningCount[CheatType::VEHICLE_TROLL_CHEAT]) >= MaxDetectionProbes[CheatType::VEHICLE_TROLL_CHEAT])
							{
								for (list <Interface *>::iterator p = InterfaceList.begin(); p != InterfaceList.end(); p++)
									(*p)->CheatCallback(playerid, CheatType::VEHICLE_TROLL_CHEAT, 0, 0, 0, 0.0);

								PlayerList[playerid]->WarningCount[CheatType::VEHICLE_TROLL_CHEAT] = 0;
							}
						}
						PlayerList[playerid]->LastVehicleChangeTick = tick;
						PlayerList[playerid]->vehicleid = vehicleid;
					}
				}
				break;
			}
			case AntiCheatCommands::SKIP:	break;
			default: PlayerList[playerid]->AntiCheatStatus[CheatType::VEHICLE_TROLL_CHEAT]--;
		}
	}

	return true;
}
PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerSpawn(int playerid)
{	
	if (PlayerList[playerid] == nullptr) return true;
	Player * player = PlayerList[playerid];

	player->ResetVariablesForSpawn(AntiCheatCommands::SKIP_CYCLE_TWICE);

	if (player->PlayerHasWeaponsOnSpawn == true)
	{
		memcpy(player->WeaponID, player->SpawnWeaponID, MAX_WEAPON_SLOTS);

		if (player->IsAntiCheatEnabled(CheatType::AMMO_CHEAT))
			memcpy(player->WeaponAmmo, player->SpawnWeaponAmmo, MAX_WEAPON_SLOTS * 4);
	}
	else if (player->classid != INVALID_CLASSID)
	{
		if (Class_HasWeapons[player->classid] == true)
		{
			memcpy(player->WeaponID, Class_SpawnWeaponID[player->classid], MAX_WEAPON_SLOTS);
				
			if (player->IsAntiCheatEnabled(CheatType::AMMO_CHEAT))
				memcpy(player->WeaponAmmo, Class_SpawnWeaponAmmo[player->classid], MAX_WEAPON_SLOTS * 4);
		}
	}		
	return true;
}
PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerRequestClass(int playerid, int classid)
{
	if (PlayerList[playerid] == nullptr) return true;
	PlayerList[playerid]->classid = classid;
	return true;
}
PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerWeaponShot(int playerid, int weaponid, int hittype, int hitid, float fX, float fY, float fZ)
{
	if (PlayerList[playerid] == nullptr) return true;

	static int LastCalled[MAX_PLAYERS];
	static char PreviousWeapon[MAX_PLAYERS];	
	static float B2V_dist, pX, pY, pZ, hX, hY, hZ;

	Player * player = PlayerList[playerid];
	player->WeaponAmmo[WeaponInformation::WeaponSlot[weaponid]]--;

	player->BulletsShot++;
	if (hittype == BULLET_HIT_TYPE_PLAYER) player->BulletsHit++;

	if (weaponid != PreviousWeapon[playerid]) { PreviousWeapon[playerid] = weaponid;	return true; }
	if (player->NoUpdateSinceLastCycle) return true;
	if (weaponid == 38) return true;
	if (GetPlayerPing(playerid) > MAX_ALLOWED_PING_FOR_RAPID_FIRE_CHECKS) return true;

	if (player->IsAntiCheatEnabled(CheatType::RAPID_FIRE_CHEAT))
	{
		int tick = GetTickCount();
		switch (player->AntiCheatStatus[CheatType::RAPID_FIRE_CHEAT])
		{
			case AntiCheatCommands::CHECK:
			{
				if ((tick - LastCalled[playerid] + MAX_WEAPON_FIRE_RATE_VARIANCE) < WeaponInformation::WeaponFireRate[weaponid])
					if (++(player->WarningCount[CheatType::RAPID_FIRE_CHEAT]) > MaxDetectionProbes[CheatType::RAPID_FIRE_CHEAT])
					{
						for (list <Interface *>::iterator p = InterfaceList.begin(); p != InterfaceList.end(); p++)
							(*p)->CheatCallback(playerid, CheatType::RAPID_FIRE_CHEAT, weaponid, tick - LastCalled[playerid], WeaponInformation::WeaponFireRate[weaponid], 0.0);

						player->WarningCount[CheatType::RAPID_FIRE_CHEAT] = 0; 
					}
				break;
			}
			case AntiCheatCommands::SKIP:	break;
			default: player->AntiCheatStatus[CheatType::RAPID_FIRE_CHEAT]--;
		}
		LastCalled[playerid] = tick;
	}	

	if (hittype == BULLET_HIT_TYPE_PLAYER)
	{
		if (player->IsAntiCheatEnabled(CheatType::PROAIM_AIMBOT_CHEAT))
		{
			switch (player->AntiCheatStatus[CheatType::PROAIM_AIMBOT_CHEAT])
			{
				case AntiCheatCommands::CHECK:
				{
					Player * player_hit = PlayerList[hitid];
					if (player_hit == nullptr) return true;
					if (GetPlayerPing(hitid) > MAX_ALLOWED_PING_FOR_PROAIM_CHECKS) return true;
					if (IsPlayerInAnyVehicle(playerid) || IsPlayerInAnyVehicle(hitid)) return true;
					if (GetPlayerSurfingVehicleID(playerid) != INVALID_VEHICLE_ID || GetPlayerSurfingVehicleID(hitid) != INVALID_VEHICLE_ID) return true;
					if (GetPlayerSurfingObjectID(playerid) != INVALID_OBJECT_ID || GetPlayerSurfingObjectID(hitid) != INVALID_OBJECT_ID) return true;

					GetPlayerPos(hitid, &pX, &pY, &pZ);
					GetPlayerLastShotVectors(playerid, &fX, &fY, &fZ, &hX, &hY, &hZ);

					B2V_dist = (pX - hX)*(pX - hX) + (pY - hY)*(pY - hY) + (pZ - hZ)*(pZ - hZ);
					if (B2V_dist > WeaponInformation::MAX_WEAPON_RANGE*WeaponInformation::MAX_WEAPON_RANGE)
					{
						if (++(player->DesyncWarningCount) > MAX_DESYNC_PROBES)
						{
							for (list <Interface *>::iterator p = InterfaceList.begin(); p != InterfaceList.end(); p++)
								(*p)->DesyncCallback(playerid);

							player->DesyncWarningCount = 0;
						}
					}
					else if (B2V_dist > MAX_B2V_DEVIATION*MAX_B2V_DEVIATION)
					{
						if (++(player->WarningCount[CheatType::PROAIM_AIMBOT_CHEAT]) > MaxDetectionProbes[CheatType::PROAIM_AIMBOT_CHEAT])
						{
							for (list <Interface *>::iterator p = InterfaceList.begin(); p != InterfaceList.end(); p++)
								(*p)->CheatCallback(playerid, CheatType::PROAIM_AIMBOT_CHEAT, weaponid, WeaponInformation::WeaponRange[weaponid], 0, float(sqrt(B2V_dist)));

							player->WarningCount[CheatType::PROAIM_AIMBOT_CHEAT] = 0;
						}
					}
					else player->DesyncWarningCount = player->WarningCount[CheatType::PROAIM_AIMBOT_CHEAT] = 0;
				}
				case AntiCheatCommands::SKIP: break;
				default: player->AntiCheatStatus[CheatType::PROAIM_AIMBOT_CHEAT]--;
			}
		}
	}
	return true;
}
PLUGIN_EXPORT bool PLUGIN_CALL OnVehicleRespray(int playerid, int vehicleid, int color1, int color2)
{
	if (PlayerList[playerid] == nullptr) return true;
	GetVehicleHealth(vehicleid, &VehicleList[vehicleid]->health);
	return true;
}
PLUGIN_EXPORT bool PLUGIN_CALL OnGameModeExit()
{
	//for (int i = 0; i < MAX_VEHICLES; i++)
	//	if (VehicleList[i] != nullptr) delete VehicleList[i];

	return true;
}
PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerEnterVehicle(int playerid, int vehicleid, bool ispassenger)
{
	if (PlayerList[playerid] == nullptr) return true;
	PlayerList[playerid]->vehicleid = vehicleid;
	return true;
}
PLUGIN_EXPORT bool PLUGIN_CALL OnPlayerExitVehicle(int playerid, int vehicleid)
{
	if (PlayerList[playerid] == nullptr) return true;
	PlayerList[playerid]->vehicleid = INVALID_VEHICLE_ID;
	return true;
}