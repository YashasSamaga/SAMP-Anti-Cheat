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
/***********************************************************************************************/
namespace Natives
{
	cell AMX_NATIVE_CALL AC_SetPlayerSpecialAction(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL AC_GivePlayerWeapon(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL AC_ResetPlayerWeapons(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL AC_SetPlayerAmmo(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL AC_SetSpawnInfo(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL AC_AddPlayerClass(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL AC_AddPlayerClassEx(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL AC_RepairVehicle(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL AC_SetVehicleHealth(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL AC_CreateVehicle(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL AC_AddStaticVehicle(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL AC_AddStaticVehicleEx(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL AC_DestroyVehicle(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL AC_SetPlayerPos(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL AC_SetPlayerPosFindZ(AMX* amx, cell* params);
	cell AMX_NATIVE_CALL AC_PutPlayerInVehicle(AMX* amx, cell* params);
}
