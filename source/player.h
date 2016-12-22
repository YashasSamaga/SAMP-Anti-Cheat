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
#pragma once

#include "SAMP-Anti-Cheat.h"
/************************************************************************************************/
class Player
{
public:
	Player(unsigned int playerid);
	void ResetVariablesForSpawn(AntiCheatCommands AC_Status);

	bool IsDesyncCheckRequired();
	bool IsDesyncCheckRequired(DesyncType type);
	void CheckPlayerForDesync(DesyncType type);
	void DesyncCheckNotRequired(DesyncType type);

	bool IsAntiCheatEnabled(CheatType AC_ID);
	void EnableAntiCheat(CheatType AC_ID);
	void DisableAntiCheat(CheatType AC_ID);

	bool NoUpdateSinceLastCycle;

	bool PlayerHasWeaponsOnSpawn;

	char DesyncWarningCount;
	char PreviousWeaponInLastModifiedSlot;
	char LastModifiedWeaponSlot;
	char classid;	

	float posX, posY, posZ;

	char AntiCheatStatus[TOTAL_CHEATS];
	unsigned char WarningCount[TOTAL_CHEATS];

	unsigned int LastVehicleChangeTick;

	unsigned int playerid;
	unsigned int vehicleid;

	unsigned int BulletsShot;
	unsigned int BulletsHit;			

	char PreviousWeaponID[MAX_WEAPON_SLOTS];

	char WeaponID[MAX_WEAPON_SLOTS];
	int WeaponAmmo[MAX_WEAPON_SLOTS];

	char SpawnWeaponID[MAX_WEAPON_SLOTS];
	int SpawnWeaponAmmo[MAX_WEAPON_SLOTS];

private:
	unsigned int DesyncChecksNeeded;
	unsigned int AntiCheatsEnabled;
};