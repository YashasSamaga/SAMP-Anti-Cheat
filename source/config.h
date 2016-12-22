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

#define MAX_B2V_DEVIATION 15
#define MAX_AMMO_VARIANCE_ALLOWED 10
#define MAX_WEAPON_FIRE_RATE_VARIANCE 20

#define MIN_HEIGHT_FOR_FLY_CHEAT_CHECKS 40.0

#define MIN_VEHICLE_SWITCH_TIME 200

#define MAX_ALLOWED_PING_FOR_RAPID_FIRE_CHECKS 500
#define MAX_ALLOWED_PING_FOR_PROAIM_CHECKS 500

#define SPEED_CONVERSION_FACTOR 170

#define CHEAT_CALLBACK_STRING "OnPlayerSuspectedForCheat"
#define DESYNC_CALLBACK_STRING "OnPlayerDesync"
