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
/***********************************************************************************************/
#define MAX_WEAPON_SLOTS 13
#define MAX_WEAPONS 47

#define INVALID_WEAPON_ID -1
/**********************************************************************************************/
namespace WeaponInformation
{
	const float MAX_WEAPON_RANGE = 320.0;

	const int WeaponSlot[MAX_WEAPONS] =
	{
		0, //0 Fist
		0, //1 Brass Knuckles
		1, //2 Golf Club
		1, //3 Night Stick
		1, //4 Knife
		1, //5 Baseball bat
		1, //6 Shovel
		1, //7 Pool Cue
		1, //8 Katana
		1, //9 Chainsaw
		10, //10 Dildo
		10, //11 White Dildo
		10, //12 Vibrator
		10, //13 Another Vibrator
		10, //14 Flowers
		10, //15 Cane
		8, //16 Grenade
		8, //17 Tear Gas
		8, //18 Molotov
		INVALID_WEAPON_ID,
		INVALID_WEAPON_ID,
		INVALID_WEAPON_ID,
		2, //22 Colt 45
		2, //23 Silenced Pistol
		2, //24 Desert Eagle
		3, //25 Shotgun
		3, //26 Sawn-Off
		3, //27 Combat Shotgun
		4, //28 UZI
		4, //29 MP5
		5, //30 AK-47
		5, //31 M4
		4, //32 Tec-9
		6, //33 Rifle
		6, //34 Sniper
		7, //35 RPG
		7, //36 Heat Seeker
		7, //37 Flamethrower
		7, //38 Minigun
		8, //39 Satchel Charges
		12, //40 Detonator
		9, //41 Spray Can
		9, //42 Fire Extinguisher
		9, //43 Camera
		11, //44 Night Vision Goggles
		11, //45 Thermal Goggles
		11 //46 Parachute
	};

	const int WeaponPickupAmmo[MAX_WEAPONS] =
	{
		0, //0 Fist
		1, //1 Brass Knuckles
		1, //2 Golf Club
		1, //3 Night Stick
		1, //4 Knife
		1, //5 Baseball bat
		1, //6 Shovel
		1, //7 Pool Cue
		1, //8 Katana
		1, //9 Chainsaw
		1, //10 Dildo
		1, //11 White Dildo
		1, //12 Vibrator
		1, //13 Another Vibrator
		1, //14 Flowers
		1, //15 Cane
		8, //16 Grenade
		8, //17 Tear Gas
		8, //18 Molotov
		INVALID_WEAPON_ID,
		INVALID_WEAPON_ID,
		INVALID_WEAPON_ID,
		30, //22 Colt 45
		10, //23 Silenced Pistol
		10, //24 Desert Eagle
		15, //25 Shotgun
		10, //26 Sawn-Off
		10, //27 Combat Shotgun
		60, //28 UZI
		60, //29 MP5
		80, //30 AK-47
		80, //31 M4
		60, //32 Tec-9
		20, //33 Rifle
		10, //34 Sniper
		4, //35 RPG
		3, //36 Heat Seeker
		100, //37 Flamethrower - (10 Ammo Client Side)
		500, //38 Minigun
		5, //39 Satchel Charges
		0, //40 Detonator
		500, //41 Spray Can
		500, //42 Fire Extinguisher
		36, //43 Camera
		0, //44 Night Vision Goggles
		0, //45 Thermal Goggles
		1, //46 Parachute
	};

	const int WeaponRange[MAX_WEAPONS] =
	{
		0, //0 Fist
		0, //1 Brass knuckles
		0, //2 Golf club
		0, //3 Nitestick
		0, //4 Knife
		0, //5 Bat
		0, //6 Shovel
		0, //7 Pool cue
		0, //8 Katana
		0, //9 Chainsaw
		0, //10 Dildo
		0, //11 Dildo 2
		0, //12 Vibrator
		0, //13 Vibrator 2
		0, //14 Flowers
		0, //15 Cane
		0, //16 Grenade
		0, //17 Teargas
		0, //18 Molotov
		INVALID_WEAPON_ID,
		INVALID_WEAPON_ID,
		INVALID_WEAPON_ID,
		35, //22 Colt 45
		35, //23 Silenced
		35, //24 Deagle
		40, //25 Shotgun
		35, //26 Sawed-off
		40, //27 Spas
		35, //28 UZI
		45, //29 MP5
		70, //30 AK47
		90, //31 M4
		35, //32 Tec9
		100, //33 Cuntgun
		320, //34 Sniper
		0, //35 Rocket launcher
		0, //36 Heatseeker
		0, //37 Flamethrower
		75,  //38 Minigun
		0, //39 Satchel Charges
		0, //40 Detonator
		0, //41 Spray Can
		0, //42 Fire Extinguisher
		0, //43 Camera
		0, //44 Night Vision Goggles
		0, //45 Thermal Goggles
		0, //46 Parachute
	};

	const int WeaponFireRate[MAX_WEAPONS] =
	{
		250, //0 Fist
		250, //1 Brass Knuckles
		250, //2 Golf Club
		250, //3 Night Stick
		250, //4 Knife
		250, //5 Baseball bat
		250, //6 Shovel
		250, //7 Pool Cue
		250, //8 Katana
		30, //9 Chainsaw
		250, //10 Dildo
		250, //11 White Dildo
		250, //12 Vibrator
		250, //13 Another Vibrator
		250, //14 Flowers
		250, //15 Cane
		0, //16 Grenade
		0, //17 Tear Gas
		0, //18 Molotov
		INVALID_WEAPON_ID,
		INVALID_WEAPON_ID,
		INVALID_WEAPON_ID,
		160, //22 Colt 45
		120, //23 Silenced Pistol
		120, //24 Desert Eagle
		800, //25 Shotgun
		120, //26 Sawn-Off
		120, //27 Combat Shotgun
		50, //28 UZI
		90, //29 MP5
		90, //30 AK-47
		90, //31 M4
		70, //32 Tec-9
		800, //33 Rifle
		900, //34 Sniper
		0, //35 RPG
		0, //36 Heat Seeker
		0, //37 Flamethrower - (10 Ammo Client Side)
		20, //38 Minigun
		0, //39 Satchel Charges
		0, //40 Detonator
		10, //41 Spray Can
		10, //42 Fire Extinguisher
		0, //43 Camera
		0, //44 Night Vision Goggles
		0, //45 Thermal Goggles
		0 //46 Parachute
	};
}
