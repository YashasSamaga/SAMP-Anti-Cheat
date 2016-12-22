/************************************************************************************************
SAMP-Anti-Cheat

This file is part of SAMP-Anti-Cheat

SAMP-Anti-Cheat is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

SAMP-Anti-Cheat is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Extensive PAWN Library.  If not, see <http://www.gnu.org/licenses/>.

Copyright (C) Yashas 2015
************************************************************************************************/
#if !defined _INC_SAMP_AC_
#define _INC_SAMP_AC_
#define SAMP_AC_INC_VERSION 0x2A7B8C

enum CheatType
{
	WEAPON_CHEAT, 
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


#if defined OnPlayerSuspectedForCheat
	forward OnPlayerSuspectedForCheat(playerid, cheatid, extra_info1, extra_info2, extra_info3, Float:extra_info4);
#endif
#endinput
