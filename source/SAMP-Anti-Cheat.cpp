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
along with SAMP Anti Cheat.  If not, see <http://www.gnu.org/licenses/>.

Copyright (C) Yashas 2013
************************************************************************************************/
#include "SAMP-Anti-Cheat.h"

#include "interface.h"
#include "player.h"
#include "vehicle.h"
#include "natives.h"

#include <list>
#include <vector>
/**********************************************************************************************************************************/
using namespace std;
using sampgdk::logprintf;

extern void *pAMXFunctions;
/**********************************************************************************************************************************/
list <Interface *> InterfaceList;

vector <Player *> PlayerList(MAX_PLAYERS,nullptr);
vector <Vehicle *> VehicleList(MAX_VEHICLES + 1,nullptr);

bool weapon_pickups_enabled = false;

bool Class_HasWeapons[MAX_PLAYERS];
char Class_SpawnWeaponID[MAX_PLAYERS][MAX_WEAPON_SLOTS];
int Class_SpawnWeaponAmmo[MAX_PLAYERS][MAX_WEAPON_SLOTS];

unsigned int AntiCheatsEnabled = (2<<TOTAL_CHEATS) -1;
/**********************************************************************************************************************************/
const char MaxDetectionProbes[TOTAL_CHEATS] =
{
	4, //Weapon Cheat
	4, //Ammo Cheat
	2, //Vehicle Speed Cheat
	3, //On Foot Speed Cheat
	5, //Jetpack Cheat
	60, //Rapid Fire Cheat
	4,  //Proaim Aimbot Cheat
	1, //Vehicle Repair Cheat
	6, //Vehicle Troll Cheat
	2, //Airbreak Cheat
	2, //Fly Cheat
};
/**********************************************************************************************************************************/
PLUGIN_EXPORT void PLUGIN_CALL ProcessTick()
{
	static int TickCount = 0;
	static Player * player;

	static int weaponid, ammo;

	TickCount++;
	
	if (TickCount%200 == 0)
	{
		for (int playerid = GetMaxPlayers(); playerid >= 0; playerid--)
		{
			player = PlayerList[playerid];
			if (player == nullptr) continue;

			//Player did not send any update since the last cheat check was done
			if (player->NoUpdateSinceLastCycle) continue;

			player->NoUpdateSinceLastCycle = true;
			
			int player_state = GetPlayerState(playerid);

			//Check if the player has been spawned
			if (player_state == PLAYER_STATE_NONE ||
				player_state == PLAYER_STATE_WASTED ||
				player_state == PLAYER_STATE_SPAWNED) continue;
		
			//Do desync checks if pending or re-check was needed
			if (player->IsDesyncCheckRequired())
			{
				if (player->IsDesyncCheckRequired(DesyncType::WEAPON_DESYNC))
				{
					if (player->LastModifiedWeaponSlot == PLAYER_WEAPON_RESET)
					{
						if (weapon_pickups_enabled = true)
						{
							char WeaponCount = 0;
							for (int i = 0; i < (MAX_WEAPON_SLOTS - 2); i++) //MAX_WEAPON_SLOTS - 1 ; Ignoring Slot 12 and Slot 11
							{
								GetPlayerWeaponData(playerid, i, &weaponid, &ammo);
								if (weaponid != 0) WeaponCount++;
							}
							if (WeaponCount > 0) player->DesyncWarningCount++;
						}
						else
						{
							char Weapons[MAX_WEAPON_SLOTS];
							char WeaponCount = 0;

							for (int i = 0; i < (MAX_WEAPON_SLOTS - 2); i++) //MAX_WEAPON_SLOTS - 1 ; Ignoring Slot 12 and Slot 11
							{
								GetPlayerWeaponData(playerid, i, &weaponid, &ammo);
								Weapons[i] = weaponid;
								if (weaponid != 0) WeaponCount++;
							}

							if (WeaponCount != 0 && memcmp(Weapons, player->PreviousWeaponID, (MAX_WEAPON_SLOTS - 2)*sizeof(char)) == 0) player->DesyncWarningCount++;
							else
							{
								player->DesyncWarningCount = 0;
								player->DesyncCheckNotRequired(DesyncType::WEAPON_DESYNC);
							}
						}						
					}
					else
					{
						GetPlayerWeaponData(playerid, player->LastModifiedWeaponSlot, &weaponid, &ammo);
						if (weaponid != player->WeaponID[player->LastModifiedWeaponSlot])
							if(weaponid == player->PreviousWeaponInLastModifiedSlot) player->DesyncWarningCount++;
					}
				}
				if (player->DesyncWarningCount >= MAX_DESYNC_PROBES)
				{
					for (list <Interface *>::iterator p = InterfaceList.begin(); p != InterfaceList.end(); p++)
						(*p)->DesyncCallback(playerid);

					player->DesyncWarningCount = 0;
				}
				continue;
			}

			//If there is a chance that the player is desynced then don't check the player for cheats
			if (player->DesyncWarningCount) continue; 
			
			//Cheat Checks
			int action = GetPlayerSpecialAction(playerid);

			int weaponid = GetPlayerWeapon(playerid);
			int vehicleid = GetPlayerVehicleID(playerid);

			int slotid = WeaponInformation::WeaponSlot[weaponid];

			int anim_idx = GetPlayerAnimationIndex(playerid);

			float speedX, speedY, speedZ;
			float posX, posY, posZ;

			GetPlayerPos(playerid, &posX, &posY, &posZ);

			if (player_state == PLAYER_STATE_DRIVER) GetVehicleVelocity(vehicleid, &speedX, &speedY, &speedZ);
			else if (player_state == PLAYER_STATE_ONFOOT) GetPlayerVelocity(playerid, &speedX, &speedY, &speedZ);

			if (player->IsAntiCheatEnabled(CheatType::WEAPON_CHEAT))
			{
				if (player_state == PLAYER_STATE_ONFOOT)
				{
					switch (player->AntiCheatStatus[CheatType::WEAPON_CHEAT])
					{
						case AntiCheatCommands::CHECK:
						{
							if (weaponid != player->WeaponID[slotid])
							{								
								if (0 < weaponid && weaponid < 46 && weaponid != 40)
								{								
									if (++(player->WarningCount[CheatType::WEAPON_CHEAT]) >= MaxDetectionProbes[CheatType::WEAPON_CHEAT])
									{
										for (list <Interface *>::iterator p = InterfaceList.begin(); p != InterfaceList.end(); p++)
											(*p)->CheatCallback(playerid, CheatType::WEAPON_CHEAT, weaponid, 0, 0, 0);

										player->WarningCount[CheatType::WEAPON_CHEAT] = 0;
									}
								}
							}
							break;
						}
						case AntiCheatCommands::SKIP: break;
						default: player->AntiCheatStatus[CheatType::WEAPON_CHEAT]--; break;
					}
				}
			}
			if (player->IsAntiCheatEnabled(CheatType::AMMO_CHEAT))
			{
				if (player_state == PLAYER_STATE_ONFOOT)
				{
					if (0 < weaponid && weaponid < 46 && weaponid != 40)
					{
						switch (player->AntiCheatStatus[CheatType::AMMO_CHEAT])
						{
							case AntiCheatCommands::CHECK:
							{
								if (GetPlayerAmmo(playerid) - player->WeaponAmmo[slotid] > MAX_AMMO_VARIANCE_ALLOWED)
								{
									if (++(player->WarningCount[CheatType::AMMO_CHEAT]) >= MaxDetectionProbes[CheatType::AMMO_CHEAT])
									{
										for (list <Interface *>::iterator p = InterfaceList.begin(); p != InterfaceList.end(); p++)
											(*p)->CheatCallback(playerid, CheatType::AMMO_CHEAT, weaponid, player->WeaponAmmo[slotid], GetPlayerAmmo(playerid), 0.0);

										player->WarningCount[CheatType::AMMO_CHEAT] = 0;
									}
								}
								else player->WarningCount[CheatType::AMMO_CHEAT] = 0;
							}
							case AntiCheatCommands::SKIP: break;
							default: player->AntiCheatStatus[CheatType::AMMO_CHEAT]--;
						}
					}
				}
			}			
			if (player->IsAntiCheatEnabled(CheatType::VEHICLE_SPEED_CHEAT))
			{				
				if (player_state == PLAYER_STATE_DRIVER)
				{
					switch (player->AntiCheatStatus[CheatType::VEHICLE_SPEED_CHEAT])
					{
						case AntiCheatCommands::CHECK:
						{
							float speed;	

							if (speedZ > 0.2 || speedZ < -0.2) 
								speed = float(sqrt((speedX*speedX) + (speedY*speedY)));
							else 
								speed = float(sqrt((speedX*speedX) + (speedY*speedY) + (speedZ*speedZ)));

							if (speed > VehicleInformation::VehicleMaxSpeed[VehicleList[vehicleid]->modelid - 400])
							{
								if (++(player->WarningCount[CheatType::VEHICLE_SPEED_CHEAT]) >= MaxDetectionProbes[CheatType::VEHICLE_SPEED_CHEAT])
								{
									for (list <Interface *>::iterator p = InterfaceList.begin(); p != InterfaceList.end(); p++)
										(*p)->CheatCallback(playerid, CheatType::VEHICLE_SPEED_CHEAT, VehicleList[vehicleid]->modelid, int(speed * SPEED_CONVERSION_FACTOR), int(VehicleInformation::VehicleMaxSpeed[VehicleList[vehicleid]->modelid - 400] * SPEED_CONVERSION_FACTOR), 0.0);

									player->WarningCount[CheatType::VEHICLE_SPEED_CHEAT] = 0;
								}
							}
						}
						case AntiCheatCommands::SKIP:	break;
						default: player->AntiCheatStatus[CheatType::VEHICLE_SPEED_CHEAT]--;
					}
				}
			}
			if (player->IsAntiCheatEnabled(CheatType::ONFOOT_SPEED_CHEAT))
			{
				if(player_state == PLAYER_STATE_ONFOOT && action != SPECIAL_ACTION_USEJETPACK)
				{			
					switch (player->AntiCheatStatus[CheatType::ONFOOT_SPEED_CHEAT])
					{
						case AntiCheatCommands::CHECK:
						{					
							float speed;

							if (speedZ > 0.1 || speedZ < -0.1) 
								speed = (speedX*speedX) + (speedY*speedY);
							else
								speed = (speedX*speedX) + (speedY*speedY) + (speedZ*speedZ);

							//,1231,1266 --- 959,958,965,1130
							switch (anim_idx)
							{
								case 959: //PARA DIV - FALLTHROUGH
								case 958: //PARA ACCL - FALLTHROUGH
								case 1130: //FALL - FALLTHROUGH
								case 1132: //GLIDE - FALLTHROUGH
								break;

								case 1539:
								case 1538:
								case 1250:
								{
									if (speed > 0.3)
									{
										if (++(player->WarningCount[CheatType::ONFOOT_SPEED_CHEAT]) >= MaxDetectionProbes[CheatType::ONFOOT_SPEED_CHEAT])
										{
											for (list <Interface *>::iterator p = InterfaceList.begin(); p != InterfaceList.end(); p++)
												(*p)->CheatCallback(playerid, CheatType::ONFOOT_SPEED_CHEAT, 0, 0, 0, 0.0);

											player->WarningCount[CheatType::ONFOOT_SPEED_CHEAT] = 0;
										}
									}
									else player->WarningCount[CheatType::ONFOOT_SPEED_CHEAT] = 0;
									break;
									break;
								}
								default:
								{
									if (speed > 0.065)
									{
										if (++(player->WarningCount[CheatType::ONFOOT_SPEED_CHEAT]) >= MaxDetectionProbes[CheatType::ONFOOT_SPEED_CHEAT])
										{
											for (list <Interface *>::iterator p = InterfaceList.begin(); p != InterfaceList.end(); p++)
												(*p)->CheatCallback(playerid, CheatType::ONFOOT_SPEED_CHEAT, 0, 0, 0, 0.0);

											player->WarningCount[CheatType::ONFOOT_SPEED_CHEAT] = 0;
										}
									}
									else player->WarningCount[CheatType::ONFOOT_SPEED_CHEAT] = 0;
									break;
								}
							}							
							break;
						}
						case AntiCheatCommands::SKIP:	break;
						default: player->AntiCheatStatus[CheatType::ONFOOT_SPEED_CHEAT]--;
					}
				}
			}
			if (player->IsAntiCheatEnabled(CheatType::JETPACK_CHEAT))
			{	
				switch (player->AntiCheatStatus[CheatType::JETPACK_CHEAT])
				{
					case AntiCheatCommands::CHECK:
					{	
						if (action == SPECIAL_ACTION_USEJETPACK)
							if (++(player->WarningCount[CheatType::JETPACK_CHEAT]) >= MaxDetectionProbes[CheatType::JETPACK_CHEAT])
							{
								for (list <Interface *>::iterator p = InterfaceList.begin(); p != InterfaceList.end(); p++)
									(*p)->CheatCallback(playerid, CheatType::JETPACK_CHEAT, 0, 0, 0, 0.0);

								player->WarningCount[CheatType::JETPACK_CHEAT] = 0;
							}
						break;
					}
					case AntiCheatCommands::SKIP:	break;
					default: player->AntiCheatStatus[CheatType::JETPACK_CHEAT]--;
				}
			}	
			if (player->IsAntiCheatEnabled(CheatType::VEHICLE_REPAIR_CHEAT))
			{
				if (player_state == PLAYER_STATE_DRIVER)
				{
					switch (player->AntiCheatStatus[CheatType::VEHICLE_REPAIR_CHEAT])
					{
						case AntiCheatCommands::CHECK:
						{
							float health;							
							
							GetVehicleHealth(vehicleid, &health);

							if (health > VehicleList[vehicleid]->health)
							{
								bool nearsprayshop = false;
								float pX, pY, pZ;

								GetPlayerPos(playerid, &pX, &pY, &pZ);

								for (int i = 0; i < (sizeof(VehicleInformation::PayNSpray) / sizeof(float)); i++)
								{
									if (SQUARE_DISTANCE(pX, pY, pZ, VehicleInformation::PayNSpray[i][0], VehicleInformation::PayNSpray[i][1], VehicleInformation::PayNSpray[i][2]) < 10)
									{
										nearsprayshop = true; 
										break;
									}
								}
								if (!nearsprayshop)
								{
									if (++(player->WarningCount[CheatType::VEHICLE_REPAIR_CHEAT]) >= MaxDetectionProbes[CheatType::VEHICLE_REPAIR_CHEAT])
									{
										for (list <Interface *>::iterator p = InterfaceList.begin(); p != InterfaceList.end(); p++)
											(*p)->CheatCallback(playerid, CheatType::VEHICLE_REPAIR_CHEAT, vehicleid, 0 , 0, 0.0);

										player->WarningCount[CheatType::VEHICLE_REPAIR_CHEAT] = 0;
									}
								}
							}
							VehicleList[vehicleid]->health = health;
							break;
						}
						case AntiCheatCommands::SKIP:	break;
						default: player->AntiCheatStatus[CheatType::VEHICLE_REPAIR_CHEAT]--;
					}
				}
			}
			if (player->IsAntiCheatEnabled(CheatType::AIRBREAK_CHEAT))
			{
				switch (player->AntiCheatStatus[CheatType::AIRBREAK_CHEAT])
				{
					case AntiCheatCommands::CHECK:
					{
						if (player_state == PLAYER_STATE_DRIVER || player_state == PLAYER_STATE_ONFOOT)
						{
							if (speedX < 0.0005 && speedY < 0.0005 && speedZ < 0.0005 && speedZ > -0.0005)
							{
								if (abs(posX - player->posX) > 8 || abs(posZ - player->posZ) > 8 || abs(posZ - player->posZ) > 10)
								{
									if (++(player->WarningCount[CheatType::AIRBREAK_CHEAT]) >= MaxDetectionProbes[CheatType::AIRBREAK_CHEAT])
									{
										for (list <Interface *>::iterator p = InterfaceList.begin(); p != InterfaceList.end(); p++)
											(*p)->CheatCallback(playerid, CheatType::AIRBREAK_CHEAT, player_state, 0, 0, 0.0);

										player->WarningCount[CheatType::AIRBREAK_CHEAT] = 0;
									}
								}
								else player->WarningCount[CheatType::AIRBREAK_CHEAT] = 0;
							}
						}
					}
					case AntiCheatCommands::SKIP: break;
					default: player->AntiCheatStatus[CheatType::AIRBREAK_CHEAT]--;
				}
			}		
			if (player->IsAntiCheatEnabled(CheatType::FLY_CHEAT))
			{
				switch (player->AntiCheatStatus[CheatType::FLY_CHEAT])
				{
					case AntiCheatCommands::CHECK:
					{
						if (player_state == PLAYER_STATE_ONFOOT)
						{
							if (GetPlayerInterior(playerid) == 0)
							{
								if (posZ > MIN_HEIGHT_FOR_FLY_CHEAT_CHECKS)
								{
									switch (anim_idx)
									{
										case 1539:
										case 1538:
										case 1250:
										{
											if (++(player->WarningCount[CheatType::FLY_CHEAT]) >= MaxDetectionProbes[CheatType::FLY_CHEAT])
											{
												for (list <Interface *>::iterator p = InterfaceList.begin(); p != InterfaceList.end(); p++)
													(*p)->CheatCallback(playerid, CheatType::FLY_CHEAT, player_state, 0, 0, 0.0);

												player->WarningCount[CheatType::FLY_CHEAT] = 0;
											}
											else player->WarningCount[CheatType::FLY_CHEAT] = 0;
											break;
										}
										case 959:
										{
											if (speedZ > 0.08 || (speedX*speedX + speedY*speedY) > 0.9)
											{
												if (++(player->WarningCount[CheatType::FLY_CHEAT]) >= MaxDetectionProbes[CheatType::FLY_CHEAT])
												{
													for (list <Interface *>::iterator p = InterfaceList.begin(); p != InterfaceList.end(); p++)
														(*p)->CheatCallback(playerid, CheatType::FLY_CHEAT, player_state, 0, 0, 0.0);

													player->WarningCount[CheatType::FLY_CHEAT] = 0;
												}												
											}
											else player->WarningCount[CheatType::FLY_CHEAT] = 0;
											break;
										}
									}								
								}							
							}
						}
					}
					case AntiCheatCommands::SKIP: break;
					default: player->AntiCheatStatus[CheatType::FLY_CHEAT]--;
				}
			}
			player->posX = posX;
			player->posY = posY;
			player->posZ = posZ;

			//1539,1538,1250  - 959
		}
	}
	sampgdk::ProcessTick();
}
/**********************************************************************************************************************************/
AMX_NATIVE_INFO PluginNatives[] =
{
	{ 0, 0 }
};
/**********************************************************************************************************************************/
PLUGIN_EXPORT int PLUGIN_CALL AmxLoad(AMX *amx)
{	
	InterfaceList.push_back(new Interface(amx));
	return amx_Register(amx, PluginNatives, -1);
}
PLUGIN_EXPORT int PLUGIN_CALL AmxUnload(AMX *amx)
{
	list<Interface *>::iterator itr = InterfaceList.begin();
	while ((*itr)->amx != amx) itr++;
	
	InterfaceList.erase(itr);

	return AMX_ERR_NONE;
}
/**********************************************************************************************************************************/
PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports()
{
	return sampgdk::Supports() | SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES | SUPPORTS_PROCESS_TICK;
}
PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData)
{
	pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
	logprintf("\n\nSAMP-Anti-Cheat v1.0\nCopyright (C) 2015 Yashas\n\n");
	return sampgdk::Load(ppData);
}
PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
	logprintf("\nSAMP-Anti-Cheat Unloaded\n");
	sampgdk::Unload();
}
/**********************************************************************************************************************************/