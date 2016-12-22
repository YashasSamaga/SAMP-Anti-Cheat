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

#define SAMPGDK_AMALGAMATION
/***********************************************************************************************/
#include "sampgdk.h"
#include "config.h"

#include "WeaponInformation.h"
#include "VehicleInformation.h"

#include <vector>
#include <list>
/***********************************************************************************************/
#define PLUGIN_INCLUDE_KEY 0x2A7B8C

#define PLUGIN_MAJOR_VERSION 1
#define PLUGIN_MINOR_VERSION 0
#define PLUGIN_PATCH_VERSION 0
/***********************************************************************************************/
#define TOTAL_CHEATS 11
#define MAX_DESYNC_PROBES 10
#define PLAYER_WEAPON_RESET 15
#define INVALID_CLASSID -1
/***********************************************************************************************/
#define TwoPowerBit(x) (1<<x) 

#define SQUARE_DISTANCE(x1,y1,z1,x2,y2,z2) ((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2))
/***********************************************************************************************/
class Interface;
class Player;
class Vehicle;

extern std::list <Interface *> InterfaceList;
extern std::vector <Player *> PlayerList;
extern std::vector <Vehicle *> VehicleList;

extern bool Class_HasWeapons[MAX_PLAYERS];
extern char Class_SpawnWeaponID[MAX_PLAYERS][MAX_WEAPON_SLOTS];
extern int Class_SpawnWeaponAmmo[MAX_PLAYERS][MAX_WEAPON_SLOTS];

extern unsigned int AntiCheatsEnabled;

extern const char MaxDetectionProbes[TOTAL_CHEATS];
/***********************************************************************************************/
enum DesyncType
{
	WEAPON_DESYNC,
	BULLET_DESYNC
};
enum CheatType
{
	WEAPON_CHEAT = 0,
	AMMO_CHEAT,
	VEHICLE_SPEED_CHEAT,
	ONFOOT_SPEED_CHEAT,
	JETPACK_CHEAT,
	RAPID_FIRE_CHEAT,
	PROAIM_AIMBOT_CHEAT,
	VEHICLE_REPAIR_CHEAT,
	VEHICLE_TROLL_CHEAT,
	AIRBREAK_CHEAT,
	FLY_CHEAT
};
enum AntiCheatCommands : signed int
{
	SKIP = -2,
	CHECK = 0,
	SKIP_CYCLE_ONCE = 1,
	SKIP_CYCLE_TWICE,
	SKIP_CYCLE_THRICE,
};
/***********************************************************************************************/

/*
,
TELEPORT_CHEAT,
,

HEALTH_REFILL,
ARMOUR_REFILL,
GODMODE_CHEAT,

MONEY_CHEAT,

TUNING_CHEAT,

FAKE_KILL_CHEAT,
AntiSPP_CHEAT,

SILENT_AIMBOT_CHEAT,

FORCE_INJECT_CHEAT,
VEHICLE_WARP_CHEAT,

CAMERA_CHEAT
*/