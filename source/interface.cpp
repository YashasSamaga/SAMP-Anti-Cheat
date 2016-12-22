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
*************************************************************************************************/
#pragma once

#include "SAMP-Anti-Cheat.h"

#include "interface.h"
#include "natives.h"

#include <string>
/************************************************************************************************/
#define INTERFACE_CALLBACK_INVALID -1

#define USENAMETABLE(hdr) \
	((hdr)->defsize==sizeof(AMX_FUNCSTUBNT))

#define NUMENTRIES(hdr,field,nextfield) \
	(unsigned)(((hdr)->nextfield - (hdr)->field) / (hdr)->defsize)

#define GETENTRY(hdr,table,index) \
	(AMX_FUNCSTUB *)((unsigned char*)(hdr) + (unsigned)(hdr)->table + (unsigned)index*(hdr)->defsize)

#define GETENTRYNAME(hdr,entry) \
	(USENAMETABLE(hdr) ? \
		(char *)((unsigned char*)(hdr) + (unsigned)((AMX_FUNCSTUBNT*)(entry))->nameofs) : \
		((AMX_FUNCSTUB*)(entry))->name)
/************************************************************************************************/
AMX_NATIVE_INFO NativeHooks[] =
{
	{ "GivePlayerWeapon" , Natives::AC_GivePlayerWeapon },
	{ "ResetPlayerWeapons" , Natives::AC_ResetPlayerWeapons },
	{ "SetPlayerAmmo" , Natives::AC_SetPlayerAmmo },
	{ "AddPlayerClass" , Natives::AC_AddPlayerClass },
	{ "AddPlayerClassEx" , Natives::AC_AddPlayerClassEx },
	{ "SetSpawnInfo" , Natives::AC_SetSpawnInfo },
	{ "SetPlayerSpecialAction" , Natives::AC_SetPlayerSpecialAction },
	{ "SetVehicleHealth" , Natives::AC_SetVehicleHealth },
	{ "RepairVehicle" , Natives::AC_RepairVehicle },
	{ "AddStaticVehicle" , Natives::AC_AddStaticVehicle },
	{ "AddStaticVehicleEx" , Natives::AC_AddStaticVehicleEx },
	{ "DestroyVehicle" , Natives::AC_DestroyVehicle },
	{ "CreateVehicle" , Natives::AC_CreateVehicle },
	{ "SetPlayerPos" , Natives::AC_SetPlayerPos },
	{ "SetPlayerPosFindZ" , Natives::AC_SetPlayerPosFindZ },
	{ "PutPlayerInVehicle", Natives::AC_PutPlayerInVehicle },
};
/************************************************************************************************/
Interface::Interface(AMX * amx) : amx(amx)
{
	AMX_HEADER * hdr = (AMX_HEADER *)amx->base;
	AMX_FUNCSTUB * func;	

	int num = NUMENTRIES(hdr, natives, libraries);
	for (int idx = 0; idx != num; idx++)
	{		
		func = GETENTRY(hdr, natives, idx);
		for (int i = 0; i < sizeof(NativeHooks) / sizeof(NativeHooks[0]); i++)
		{			
			if (!strcmp(NativeHooks[i].name, GETENTRYNAME(hdr, func)))
				func->address = (ucell)NativeHooks[i].func;
		}
	}
	//Hook Code was copied from sscanf

	int amx_error = amx_FindPublic(amx, CHEAT_CALLBACK_STRING, &CheatCallbackIndex);
	if (amx_error != AMX_ERR_NONE) CheatCallbackIndex = INTERFACE_CALLBACK_INVALID;

	amx_error = amx_FindPublic(amx, DESYNC_CALLBACK_STRING, &DesyncCallbackIndex);
	if (amx_error != AMX_ERR_NONE) DesyncCallbackIndex = INTERFACE_CALLBACK_INVALID;
}

void Interface::CheatCallback(int playerid, int cheatid, int extra_info1, int extra_info2, int extra_info3, float extra_info4)
{
	if (CheatCallbackIndex != INTERFACE_CALLBACK_INVALID)
	{
		amx_Push(amx, amx_ftoc(extra_info4));
		amx_Push(amx, extra_info3);
		amx_Push(amx, extra_info2);
		amx_Push(amx, extra_info1);
		amx_Push(amx, cheatid);
		amx_Push(amx, playerid);
		amx_Exec(amx, NULL, CheatCallbackIndex);
	}
}

void Interface::DesyncCallback(int playerid)
{
	if (DesyncCallbackIndex != INTERFACE_CALLBACK_INVALID)
	{
		amx_Push(amx, playerid);
		amx_Exec(amx, NULL, DesyncCallbackIndex);
	}
}