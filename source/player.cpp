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

#include <string>
/***********************************************************************************************/
Player::Player(unsigned int playerid) : playerid(playerid)
{
	DesyncChecksNeeded = false;
	NoUpdateSinceLastCycle = false;
	PlayerHasWeaponsOnSpawn = false;

	DesyncWarningCount = 0;
	LastModifiedWeaponSlot = 0;

	classid = INVALID_CLASSID;

	memset(WarningCount, 0, TOTAL_CHEATS);
	memset(AntiCheatStatus, AntiCheatCommands::SKIP, TOTAL_CHEATS);

	LastVehicleChangeTick = 0;
	vehicleid = INVALID_VEHICLE_ID;

	BulletsHit = 0;
	BulletsShot = 0;

	AntiCheatsEnabled = 0; //Bit Mask

	memset(SpawnWeaponID, 0, MAX_WEAPON_SLOTS);
	memset(SpawnWeaponAmmo, 0, MAX_WEAPON_SLOTS * sizeof(int));

	memset(WeaponID, 0, MAX_WEAPON_SLOTS);
	memset(WeaponAmmo, 0, MAX_WEAPON_SLOTS * sizeof(int));
}

void Player::ResetVariablesForSpawn(AntiCheatCommands AC_Status)
{
	NoUpdateSinceLastCycle = false;

	DesyncWarningCount = 0;
	LastModifiedWeaponSlot = 0;

	AntiCheatsEnabled = ::AntiCheatsEnabled;

	memset(WarningCount, 0, TOTAL_CHEATS);
	memset(AntiCheatStatus, AC_Status, TOTAL_CHEATS);

	vehicleid = INVALID_VEHICLE_ID;
}

bool Player::IsDesyncCheckRequired()
{
	return !!DesyncChecksNeeded;
}
bool Player::IsDesyncCheckRequired(DesyncType type)
{
	return !!(DesyncChecksNeeded & TwoPowerBit(type));
}
void Player::DesyncCheckNotRequired(DesyncType type)
{
	DesyncChecksNeeded &= ~TwoPowerBit(type);
}
void Player::CheckPlayerForDesync(DesyncType type)
{
	AntiCheatsEnabled |= TwoPowerBit(type);
}

bool Player::IsAntiCheatEnabled(CheatType AC_ID)
{
	return !!(AntiCheatsEnabled & TwoPowerBit(AC_ID));
}
void Player::EnableAntiCheat(CheatType AC_ID)
{
	AntiCheatsEnabled |= TwoPowerBit(AC_ID);
}
void Player::DisableAntiCheat(CheatType AC_ID)
{
	AntiCheatsEnabled &= ~TwoPowerBit(AC_ID);
}
