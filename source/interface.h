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
/************************************************************************************************/
class Interface
{
public:
	Interface(AMX * amx);
	void CheatCallback(int playerid, int cheatid, int extra_info1, int extra_info2, int extra_info3, float extra_info4);
	void DesyncCallback(int playerid);

	AMX * amx;

private:
	int CheatCallbackIndex;
	int DesyncCallbackIndex;
};
