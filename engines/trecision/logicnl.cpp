/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "trecision/nl/extern.h"
#include "trecision/trecision.h"
#include "trecision/logic.h"
#include "trecision/nl/struct.h"
#include "trecision/nl/define.h"
#include "trecision/nl/message.h"
#include "video.h"

namespace Trecision {

LogicManager::LogicManager(TrecisionEngine *vm) : _vm(vm) {
	// panel puzzle 35
	for (int i = 0; i < 7; ++i) {
		Comb35[i] = 0;
	}
	Count35 = 0;

	// sundial puzzle 49
	for (int i = 0; i < 4; ++i) {
		Comb49[i] = 0;
	}

	for (int i = 0; i < 6; ++i) {
		Comb58[i] = 0;
		Comb4CT[i] = 0;
	}
	Count58 = 0;
}
LogicManager::~LogicManager() {}

/* ------------------------------------------------
					initScript
 --------------------------------------------------*/
void LogicManager::initScript() {
	int f = 0;

	_vm->_script[s16CARD]._firstFrame = f;
	_vm->_script[s16CARD]._flag = 0;

	_vm->_scriptFrame[f]._class = MC_CHARACTER;
	_vm->_scriptFrame[f]._event = ME_CHARACTERACTION;
	_vm->_scriptFrame[f++]._u16Param1 = a166USECREDITCARD;

	_vm->_scriptFrame[f]._class = MC_ACTION;
	_vm->_scriptFrame[f]._event = ME_MOUSEOPERATE;
	_vm->_scriptFrame[f++]._u32Param = oTICKETOFFICE16;

	_vm->_script[S16MONEY]._firstFrame = ++f;
	_vm->_script[S16MONEY]._flag = 0;

	_vm->_scriptFrame[f]._class = MC_CHARACTER;
	_vm->_scriptFrame[f]._event = ME_CHARACTERACTION;
	_vm->_scriptFrame[f++]._u16Param1 = a167USEMONEY;

	_vm->_scriptFrame[f]._class = MC_ACTION;
	_vm->_scriptFrame[f]._event = ME_MOUSEOPERATE;
	_vm->_scriptFrame[f++]._u32Param = oTICKETOFFICE16;

	_vm->_script[s19EVA]._firstFrame = ++f;
	_vm->_script[s19EVA]._flag = 0;

	_vm->_scriptFrame[f]._class = MC_MOUSE;
	_vm->_scriptFrame[f]._event = ME_MRIGHT;
	_vm->_scriptFrame[f]._u8Param = 1;
	_vm->_scriptFrame[f]._u16Param1 = 214;
	_vm->_scriptFrame[f]._u16Param2 = TOP + 140;
	_vm->_scriptFrame[f++]._u32Param = ocEVA19;

	_vm->_script[s4AHELLEN]._firstFrame = ++f;
	_vm->_script[s4AHELLEN]._flag = 0;

	_vm->_scriptFrame[f]._class = MC_CHARACTER;
	_vm->_scriptFrame[f]._event = ME_CHARACTERACTION;
	_vm->_scriptFrame[f++]._u16Param1 = a4A2;

	_vm->_scriptFrame[f]._class = MC_MOUSE;
	_vm->_scriptFrame[f]._event = ME_MLEFT;
	_vm->_scriptFrame[f]._u8Param = 1;
	_vm->_scriptFrame[f]._u16Param1 = 336;
	_vm->_scriptFrame[f]._u16Param2 = 263 + TOP;
	_vm->_scriptFrame[f++]._u32Param = 0;

	_vm->_scriptFrame[f]._class = MC_MOUSE;
	_vm->_scriptFrame[f]._event = ME_MLEFT;
	_vm->_scriptFrame[f]._u8Param = 1;
	_vm->_scriptFrame[f]._u16Param1 = 336;
	_vm->_scriptFrame[f]._u16Param2 = 260 + TOP;
	_vm->_scriptFrame[f++]._u32Param = 0;

	_vm->_scriptFrame[f]._class = MC_DIALOG;
	_vm->_scriptFrame[f]._event = ME_STARTDIALOG;
	_vm->_scriptFrame[f++]._u16Param1 = dF4A3;

	_vm->_script[s49SUNDIAL]._firstFrame = ++f;
	_vm->_script[s49SUNDIAL]._flag = 0;

	_vm->_scriptFrame[f]._class = MC_SCRIPT;
	_vm->_scriptFrame[f]._event = ME_CHANGER;
	_vm->_scriptFrame[f]._u16Param1 = r49;
	_vm->_scriptFrame[f]._u16Param2 = a496;
	_vm->_scriptFrame[f]._u8Param = 1;
	_vm->_scriptFrame[f++]._u32Param = 1;

	_vm->_scriptFrame[f]._class = MC_MOUSE;
	_vm->_scriptFrame[f]._event = ME_MLEFT;
	_vm->_scriptFrame[f]._u8Param = 1;
	_vm->_scriptFrame[f]._u16Param1 = 360;
	_vm->_scriptFrame[f]._u16Param2 = 255 + TOP;
	_vm->_scriptFrame[f++]._u32Param = 0;

	_vm->_scriptFrame[f]._class = MC_SCRIPT;
	_vm->_scriptFrame[f]._event = ME_CHANGER;
	_vm->_scriptFrame[f]._u16Param1 = r4A;
	_vm->_scriptFrame[f]._u16Param2 = 0;
	_vm->_scriptFrame[f]._u8Param = 1;
	_vm->_scriptFrame[f++]._u32Param = 1;

	//	Fast change room in double room of level 2
	_vm->_script[s21TO22]._firstFrame = ++f;
	_vm->_script[s21TO22]._flag = 0;

	_vm->_scriptFrame[f]._class = MC_MOUSE;
	_vm->_scriptFrame[f]._u16Param1 = 317;
	_vm->_scriptFrame[f]._u8Param = 1;
	_vm->_scriptFrame[f]._u16Param2 = 166;
	_vm->_scriptFrame[f]._event = ME_MRIGHT;
	_vm->_scriptFrame[f++]._u32Param = oCATENAT21;

	_vm->_scriptFrame[f]._class = MC_MOUSE;
	_vm->_scriptFrame[f]._u16Param1 = 429;
	_vm->_scriptFrame[f]._u8Param = 1;
	_vm->_scriptFrame[f]._u16Param2 = 194;
	_vm->_scriptFrame[f]._event = ME_MLEFT;
	_vm->_scriptFrame[f++]._u32Param = od21TO22;

	_vm->_script[s21TO23]._firstFrame = ++f;
	_vm->_script[s21TO23]._flag = 0;

	_vm->_scriptFrame[f]._class = MC_MOUSE;
	_vm->_scriptFrame[f]._u16Param1 = 317;
	_vm->_scriptFrame[f]._u8Param = 1;
	_vm->_scriptFrame[f]._u16Param2 = 166;
	_vm->_scriptFrame[f]._event = ME_MRIGHT;
	_vm->_scriptFrame[f++]._u32Param = oCATENAT21;

	_vm->_scriptFrame[f]._class = MC_MOUSE;
	_vm->_scriptFrame[f]._u16Param1 = 204;
	_vm->_scriptFrame[f]._u8Param = 1;
	_vm->_scriptFrame[f]._u16Param2 = 157;
	_vm->_scriptFrame[f]._event = ME_MLEFT;
	_vm->_scriptFrame[f++]._u32Param = od21TO23;

	_vm->_script[s24TO23]._firstFrame = ++f;
	_vm->_script[s24TO23]._flag = 0;
	_vm->_scriptFrame[f]._class = MC_MOUSE;
	_vm->_scriptFrame[f]._u16Param1 = 316;
	_vm->_scriptFrame[f]._u8Param = 1;
	_vm->_scriptFrame[f]._u16Param2 = 213;
	_vm->_scriptFrame[f]._event = ME_MRIGHT;
	_vm->_scriptFrame[f++]._u32Param = oPASSAGE24;

	_vm->_scriptFrame[f]._class = MC_MOUSE;
	_vm->_scriptFrame[f]._u16Param1 = 120;
	_vm->_scriptFrame[f]._u8Param = 1;
	_vm->_scriptFrame[f]._u16Param2 = 196;
	_vm->_scriptFrame[f]._event = ME_MLEFT;
	_vm->_scriptFrame[f++]._u32Param = od24TO23;

	_vm->_script[s24TO26]._firstFrame = ++f;
	_vm->_script[s24TO26]._flag = 0;

	_vm->_scriptFrame[f]._class = MC_MOUSE;
	_vm->_scriptFrame[f]._u16Param1 = 316;
	_vm->_scriptFrame[f]._u8Param = 1;
	_vm->_scriptFrame[f]._u16Param2 = 213;
	_vm->_scriptFrame[f]._event = ME_MRIGHT;
	_vm->_scriptFrame[f++]._u32Param = oPASSAGE24;

	_vm->_scriptFrame[f]._class = MC_MOUSE;
	_vm->_scriptFrame[f]._u16Param1 = 527;
	_vm->_scriptFrame[f]._u8Param = 1;
	_vm->_scriptFrame[f]._u16Param2 = 187;
	_vm->_scriptFrame[f]._event = ME_MLEFT;
	_vm->_scriptFrame[f++]._u32Param = od24TO26;

	_vm->_script[s2ETO2C]._firstFrame = ++f;
	_vm->_script[s2ETO2C]._flag = 0;

	_vm->_scriptFrame[f]._class = MC_MOUSE;
	_vm->_scriptFrame[f]._u16Param1 = 420;
	_vm->_scriptFrame[f]._u8Param = 1;
	_vm->_scriptFrame[f]._u16Param2 = 238;
	_vm->_scriptFrame[f]._event = ME_MRIGHT;
	_vm->_scriptFrame[f++]._u32Param = oCATWALKA2E;

	_vm->_scriptFrame[f]._class = MC_MOUSE;
	_vm->_scriptFrame[f]._u16Param1 = 66;
	_vm->_scriptFrame[f]._u8Param = 1;
	_vm->_scriptFrame[f]._u16Param2 = 336;
	_vm->_scriptFrame[f]._event = ME_MLEFT;
	_vm->_scriptFrame[f++]._u32Param = od2ETO2C;

	_vm->_script[s2ETO2F]._firstFrame = ++f;
	_vm->_script[s2ETO2F]._flag = 0;

	_vm->_scriptFrame[f]._class = MC_MOUSE;
	_vm->_scriptFrame[f]._u16Param1 = 420;
	_vm->_scriptFrame[f]._u8Param = 1;
	_vm->_scriptFrame[f]._u16Param2 = 238;
	_vm->_scriptFrame[f]._event = ME_MRIGHT;
	_vm->_scriptFrame[f++]._u32Param = oCATWALKA2E;

	_vm->_scriptFrame[f]._class = MC_MOUSE;
	_vm->_scriptFrame[f]._u16Param1 = 213;
	_vm->_scriptFrame[f]._u8Param = 1;
	_vm->_scriptFrame[f]._u16Param2 = 69;
	_vm->_scriptFrame[f]._event = ME_MLEFT;
	_vm->_scriptFrame[f]._u32Param = oENTRANCE2E;
}

void LogicManager::setRoom(uint16 r, bool b) {
	switch (r) {
	case r21:
		if (!b) {
			read3D("21.3d");
			_vm->_room[r21]._flag &= ~OBJFLAG_EXTRA;
			setPosition(14);
			_vm->_obj[oCATENAT21]._position = 5;
			_vm->_obj[oUSCITA21]._position = 11;

			// if we can go beyond
			if (((_vm->iconPos(iSBARRA21) != MAXICON) && ((g_vm->_choice[436]._flag & OBJFLAG_DONE) || (g_vm->_choice[466]._flag & OBJFLAG_DONE)))
				|| ((g_vm->_choice[451]._flag & OBJFLAG_DONE) || (g_vm->_choice[481]._flag & OBJFLAG_DONE))) {
				_vm->_obj[od21TO23]._flag |= OBJFLAG_ROOMOUT;
				_vm->_obj[od21TO23]._flag &= ~OBJFLAG_EXAMINE;
			} else {
				_vm->_obj[od21TO23]._flag &= ~OBJFLAG_ROOMOUT;
				_vm->_obj[od21TO23]._flag |= OBJFLAG_EXAMINE;
			}

			_vm->_obj[od21TO23]._anim = 0;
			_vm->_obj[oUSCITA21]._mode |= OBJMODE_OBJSTATUS;

			_vm->_obj[od21TO22]._flag |= OBJFLAG_ROOMOUT;
			_vm->_obj[od21TO22]._flag &= ~OBJFLAG_EXAMINE;
			_vm->_obj[od21TO22]._anim = aWALKOUT;
			_vm->_obj[oPORTAA21]._anim = a212;
			_vm->_obj[oDOORC21]._anim = a219;

			_vm->_obj[oCUNICOLO21]._mode |= OBJMODE_OBJSTATUS;
			_vm->_obj[oCARTELLONE21]._mode |= OBJMODE_OBJSTATUS;
		} else {
			read3D("212.3d");
			_vm->_room[r21]._flag |= OBJFLAG_EXTRA;
			setPosition(15);
			_vm->_obj[oCATENAT21]._position = 6;
			_vm->_obj[oUSCITA21]._position = 21;

			_vm->_obj[od21TO23]._flag |= OBJFLAG_ROOMOUT;
			_vm->_obj[od21TO23]._flag &= ~OBJFLAG_EXAMINE;
			_vm->_obj[od21TO23]._anim = aWALKOUT;
			_vm->_obj[oUSCITA21]._mode |= OBJMODE_OBJSTATUS;

			// If we can go beyond
			if (((_vm->iconPos(iSBARRA21) != MAXICON) && ((g_vm->_choice[436]._flag & OBJFLAG_DONE) || (g_vm->_choice[466]._flag & OBJFLAG_DONE)))
				|| ((g_vm->_choice[451]._flag & OBJFLAG_DONE) || (g_vm->_choice[481]._flag & OBJFLAG_DONE))) {
				_vm->_obj[od21TO22]._flag |= OBJFLAG_ROOMOUT;
				_vm->_obj[od21TO22]._flag &= ~OBJFLAG_EXAMINE;
			} else {
				_vm->_obj[od21TO22]._flag &= ~OBJFLAG_ROOMOUT;
				_vm->_obj[od21TO22]._flag |= OBJFLAG_EXAMINE;
			}
			_vm->_obj[od21TO22]._anim = 0;
			_vm->_obj[od21TO22]._examine = 335;
			_vm->_obj[od21TO22]._action = 335;
			_vm->_obj[oPORTAA21]._anim = 0;
			_vm->_obj[oDOORC21]._anim = 0;

			_vm->_obj[oCUNICOLO21]._mode &= ~OBJMODE_OBJSTATUS;
			_vm->_obj[oCARTELLONE21]._mode &= ~OBJMODE_OBJSTATUS;
		}
		break;
	case r24:
		if (!b) {
			read3D("24.3d");
			_vm->_room[r24]._flag &= ~OBJFLAG_EXTRA;
			_vm->_obj[oPASSAGE24]._position = 3;
			_vm->_obj[oMACERIE24]._position = 3;
			_vm->_obj[oDUMMY24]._mode &= ~OBJMODE_OBJSTATUS;
			_vm->_obj[oDUMMY24A]._mode |= OBJMODE_OBJSTATUS;
		} else {
			read3D("242.3d");
			_vm->_room[r24]._flag |= OBJFLAG_EXTRA;
			_vm->_obj[od24TO26]._mode |= OBJMODE_OBJSTATUS;
			_vm->_obj[oPASSAGE24]._position = 4;
			_vm->_obj[oMACERIE24]._position = 4;
			_vm->_obj[oDUMMY24A]._mode &= ~OBJMODE_OBJSTATUS;
			_vm->_obj[oDUMMY24]._mode |= OBJMODE_OBJSTATUS;
		}
		break;

	case r2A:
		if (!b) {
			read3D("2A.3d");
			_vm->_room[r2A]._flag &= ~OBJFLAG_EXTRA;
			_vm->_obj[oDUMMY2A2]._mode |= OBJMODE_OBJSTATUS;
			_vm->_obj[oDUMMY2A]._mode &= ~OBJMODE_OBJSTATUS;
		} else {
			read3D("2A2.3d");
			_vm->_room[r2A]._flag |= OBJFLAG_EXTRA;
			_vm->_obj[oDUMMY2A]._mode |= OBJMODE_OBJSTATUS;
			_vm->_obj[oDUMMY2A2]._mode &= ~OBJMODE_OBJSTATUS;
		}
		break;
	case r2B:
		if (!b) {
			read3D("2B.3d");
			_vm->_room[r2B]._flag &= ~OBJFLAG_EXTRA;
			_vm->_obj[oPORTA2B]._mode |= OBJMODE_OBJSTATUS;
			_vm->_obj[od2BALLA28]._mode &= ~OBJMODE_OBJSTATUS;
		} else {
			read3D("2B2.3d");
			_vm->_room[r2B]._flag |= OBJFLAG_EXTRA;
			_vm->_obj[oPORTA2B]._mode &= ~OBJMODE_OBJSTATUS;
			_vm->_obj[od2BALLA28]._mode |= OBJMODE_OBJSTATUS;
		}
		break;
	case r2E:
		if (!b) {
			_vm->_obj[oCATWALKA2E]._nbox = BACKGROUND;
			_vm->_obj[oCATWALKA2E]._position = 2;
			_vm->_obj[oCATWALKA2E]._anim = a2E2PRIMAPALLONTANANDO;
			read3D("2E.3d");
			_vm->_room[r2E]._flag &= ~OBJFLAG_EXTRA;
			_vm->_obj[oDUMMY2E]._mode &= ~OBJMODE_OBJSTATUS;
			_vm->_obj[oENTRANCE2E]._flag &= ~OBJFLAG_EXAMINE;
			_vm->_obj[oPASSERELLAB2E]._mode &= ~OBJMODE_OBJSTATUS;
			_vm->_obj[oCRATERE2E]._mode &= ~OBJMODE_OBJSTATUS;
			_vm->_obj[oARBUSTI2E]._mode &= ~OBJMODE_OBJSTATUS;
			_vm->_obj[oCREPACCIO2E]._position = 6;
		} else {
			_vm->_obj[oCATWALKA2E]._position = 3;
			_vm->_obj[oCATWALKA2E]._anim = a2E3PRIMAPAVVICINANDO;
			read3D("2E2.3d");
			_vm->_room[r2E]._flag |= OBJFLAG_EXTRA;
			_vm->_obj[oDUMMY2E]._mode |= OBJMODE_OBJSTATUS;
			_vm->_obj[oENTRANCE2E]._mode |= OBJMODE_OBJSTATUS;
			_vm->_obj[oENTRANCE2E]._flag |= OBJFLAG_EXAMINE;
			_vm->_obj[oPASSERELLAB2E]._mode |= OBJMODE_OBJSTATUS;
			_vm->_obj[oCRATERE2E]._mode |= OBJMODE_OBJSTATUS;
			_vm->_obj[oARBUSTI2E]._mode |= OBJMODE_OBJSTATUS;
			_vm->_obj[oCREPACCIO2E]._position = 7;
			_vm->_obj[oCATWALKA2E]._nbox = FOREGROUND;
		}
		break;
	case r2GV:
		if (!b) {
			_vm->_obj[oVIADOTTO2GV]._position = 7;
			_vm->_obj[oVIADOTTO2GV]._anim = a2G7ATTRAVERSAPONTICELLO;
			read3D("2GV.3d");
			_vm->_room[r2GV]._flag &= ~OBJFLAG_EXTRA;
			_vm->_obj[oDUMMY2GV]._mode &= ~OBJMODE_OBJSTATUS;
			_vm->_obj[oRAGAZZOS2GV]._mode &= ~OBJMODE_OBJSTATUS;
			_vm->_obj[oCOCCODRILLO2GV]._mode &= ~OBJMODE_OBJSTATUS;
		}
		break;
	default:
		break;
	}
}

void LogicManager::initInventory() {
	_vm->_inventorySize = 0;
	_vm->_inventory[_vm->_inventorySize++] = iBANCONOTE;
	_vm->_inventory[_vm->_inventorySize++] = iSAM;
	_vm->_inventory[_vm->_inventorySize++] = iCARD03;
	_vm->_inventory[_vm->_inventorySize++] = iPEN;
	_vm->_inventory[_vm->_inventorySize++] = iKEY05;
}

void LogicManager::useWithInventory(bool *updateInv, bool *printSentence) {
	*updateInv = true;
	*printSentence = true;

	switch (_vm->_useWith[USED]) {
	case iSTAGNOLA:
		if (_vm->_useWith[WITH] == iFUSE) {
			_vm->removeIcon(iSTAGNOLA);
			_vm->replaceIcon(iFUSE, iFUSES);
			StartCharacterAction(hUSEGG, 0, 0, 1441);
			*printSentence = false;
		}
		break;

	case iFUSE:
		if (_vm->_useWith[WITH] == iSTAGNOLA) {
			_vm->removeIcon(iSTAGNOLA);
			_vm->replaceIcon(iFUSE, iFUSES);
			StartCharacterAction(hUSEGG, 0, 0, 1441);
			*printSentence = false;
		}
		break;

	case iTOPO1C:
		if (_vm->_useWith[WITH] == iPATTINO) {
			_vm->removeIcon(iPATTINO);
			_vm->removeIcon(iTOPO1C);
			_vm->addIcon(iTOPO1D);
			StartCharacterAction(hUSEGG, 0, 0, 1497);
			*printSentence = false;
		}
		break;

	case iPATTINO:
		if (_vm->_useWith[WITH] == iTOPO1C) {
			_vm->removeIcon(iPATTINO);
			_vm->removeIcon(iTOPO1C);
			_vm->addIcon(iTOPO1D);
			StartCharacterAction(hUSEGG, 0, 0, 1497);
			*printSentence = false;
		}
		break;

	case iBAR11:
		if (_vm->_useWith[WITH] == iMAGNETE) {
			_vm->removeIcon(iBAR11);
			_vm->replaceIcon(iMAGNETE, iSBARRA21);
			StartCharacterAction(hUSEGG, 0, 0, 1438);
			*printSentence = false;
		}
		break;

	case iMAGNETE:
		if (_vm->_useWith[WITH] == iBAR11) {
			_vm->removeIcon(iBAR11);
			_vm->replaceIcon(iMAGNETE, iSBARRA21);
			StartCharacterAction(hUSEGG, 0, 0, 1533);
			*printSentence = false;
		}
		break;

	case iSIGARO:
		if (_vm->_useWith[WITH] == iSCOPA27) {
			_vm->removeIcon(iSCOPA27);
			_vm->replaceIcon(iSIGARO, iTORCIA32);
			StartCharacterAction(hUSEGG, 0, 0, 1575);
			*printSentence = false;
		}
		break;

	case iSCOPA27:
		if (_vm->_useWith[WITH] == iSIGARO) {
			_vm->removeIcon(iSCOPA27);
			_vm->replaceIcon(iSIGARO, iTORCIA32);
			StartCharacterAction(hUSEGG, 0, 0, 1546);
			*printSentence = false;
		}
		break;

	case iPROIETTORE31:
		if (_vm->_useWith[WITH] == iTRIPLA) {
			_vm->removeIcon(iTRIPLA);
			_vm->replaceIcon(iPROIETTORE31, iPROIETTORE35);
			StartCharacterAction(hUSEGG, 0, 0, 0);
			*printSentence = false;
		}
		break;

	case iTRIPLA:
		if (_vm->_useWith[WITH] == iPROIETTORE31) {
			_vm->removeIcon(iTRIPLA);
			_vm->replaceIcon(iPROIETTORE31, iPROIETTORE35);
			StartCharacterAction(hUSEGG, 0, 0, 0);
			*printSentence = false;
		}
		break;

	case iSALNITRO:
	case iCARBONE:
	case iZOLFO:
	case iCARSAL:
	case iCARZOL:
	case iSALZOL:
		if ((_vm->_useWith[WITH] == iSALNITRO) || (_vm->_useWith[WITH] == iCARBONE) || (_vm->_useWith[WITH] == iZOLFO) ||
			(_vm->_useWith[WITH] == iCARSAL) || (_vm->_useWith[WITH] == iCARZOL) || (_vm->_useWith[WITH] == iSALZOL)) {
			_vm->removeIcon(_vm->_useWith[USED]);
			_vm->removeIcon(_vm->_useWith[WITH]);

			if (((_vm->_useWith[USED] == iSALNITRO) && (_vm->_useWith[WITH] == iCARBONE)) ||
				((_vm->_useWith[WITH] == iSALNITRO) && (_vm->_useWith[USED] == iCARBONE)))
				_vm->addIcon(iCARSAL);
			if (((_vm->_useWith[USED] == iZOLFO) && (_vm->_useWith[WITH] == iCARBONE)) ||
				((_vm->_useWith[WITH] == iZOLFO) && (_vm->_useWith[USED] == iCARBONE)))
				_vm->addIcon(iCARZOL);
			if (((_vm->_useWith[USED] == iZOLFO) && (_vm->_useWith[WITH] == iSALNITRO)) ||
				((_vm->_useWith[WITH] == iZOLFO) && (_vm->_useWith[USED] == iSALNITRO)))
				_vm->addIcon(iSALZOL);

			if ((_vm->_useWith[USED] == iZOLFO) || (_vm->_useWith[WITH] == iZOLFO))
				_vm->addIcon(iBARATTOLO);
			if ((_vm->_useWith[USED] >= iCARSAL) || (_vm->_useWith[WITH] >= iCARSAL))
				_vm->addIcon(iPOLVERE48);
			StartCharacterAction(hUSEGG, 0, 0, 1663);
			*printSentence = false;
		}
		break;

	case iPISTOLA4B:
		if (_vm->_useWith[WITH] == iPOLVERE48) {
			_vm->replaceIcon(iPOLVERE48, iPOLVERE4P);
			_vm->replaceIcon(iPISTOLA4B, iPISTOLA4PC);
			StartCharacterAction(hUSEGG, 0, 0, 1676);
			*printSentence = false;
		} else if (_vm->_useWith[WITH] == iPOLVERE4P) {
			_vm->removeIcon(iPOLVERE4P);
			_vm->replaceIcon(iPISTOLA4B, iPISTOLA4PC);
			StartCharacterAction(hUSEGG, 0, 0, 1700);
			*printSentence = false;
		}
		break;

	case iPOLVERE48:
		if (_vm->_useWith[WITH] == iPISTOLA4B) {
			_vm->replaceIcon(iPOLVERE48, iPOLVERE4P);
			_vm->replaceIcon(iPISTOLA4B, iPISTOLA4PC);
			StartCharacterAction(hUSEGG, 0, 0, 1676);
			*printSentence = false;
		}
		break;

	case iPOLVERE4P:
		if (_vm->_useWith[WITH] == iPISTOLA4B) {
			_vm->removeIcon(iPOLVERE4P);
			_vm->replaceIcon(iPISTOLA4B, iPISTOLA4PC);
			StartCharacterAction(hUSEGG, 0, 0, 1700);
			*printSentence = false;
		}
		break;

	case iBIGLIAA:
	case iBIGLIAB:
		if ((_vm->_useWith[WITH] == iPISTOLA4PC) && !(_vm->_inventoryObj[iPISTOLA4PC]._flag & OBJFLAG_EXTRA)) {
			_vm->removeIcon(_vm->_useWith[USED]);
			_vm->replaceIcon(iPISTOLA4PC, iPISTOLA4PD);
			StartCharacterAction(hUSEGG, 0, 0, 1683);
			_vm->_inventoryObj[iPISTOLA4PC]._flag |= OBJFLAG_EXTRA;
			*printSentence = false;
		} else if (_vm->_useWith[WITH] == iPISTOLA4PC) {
			CharacterSay(1688);
			*printSentence = false;
		} else if (_vm->_useWith[WITH] == iPISTOLA4B) {
			CharacterSay(2009);
			*printSentence = false;
		}
		break;

	case iBIGLIA4U:
		if (_vm->_useWith[WITH] == iPISTOLA4PC) {
			_vm->removeIcon(iBIGLIA4U);
			_vm->replaceIcon(iPISTOLA4PC, iPISTOLA4PD);
			StartCharacterAction(hUSEGG, 0, 0, 1718);
			_vm->_inventoryObj[iPISTOLA4PD]._flag |= OBJFLAG_EXTRA;
			*printSentence = false;
		} else if (_vm->_useWith[WITH] == iPISTOLA4B) {
			CharacterSay(2011);
			*printSentence = false;
		}
		break;

	case iSIRINGA37:
		if (_vm->_useWith[WITH] == iFIALE) {
			_vm->removeIcon(iSIRINGA37);
			_vm->replaceIcon(iFIALE, iSIRINGA59);
			StartCharacterAction(hUSEGG, 0, 0, 1756);
			*printSentence = false;
		}
		break;

	case iFIALE:
		if (_vm->_useWith[WITH] == iSIRINGA37) {
			_vm->removeIcon(iSIRINGA37);
			_vm->replaceIcon(iFIALE, iSIRINGA59);
			StartCharacterAction(hUSEGG, 0, 0, 1756);
			*printSentence = false;
		}
		break;

	case iFILO:
		if (_vm->_useWith[WITH] == iGUANTI57) {
			_vm->removeIcon(iFILO);
			_vm->replaceIcon(iGUANTI57, iGUANTI5A);
			StartCharacterAction(hUSEGG, 0, 0, 1756);
			*printSentence = false;
		} else if (_vm->_useWith[WITH] == iSIRINGA59) {
			_vm->removeIcon(iFILO);
			_vm->replaceIcon(iSIRINGA59, iSIRINGA5A);
			StartCharacterAction(hUSEGG, 0, 0, 1756);
			*printSentence = false;
		}
		break;

	case iGUANTI57:
		if (_vm->_useWith[WITH] == iFILO) {
			_vm->removeIcon(iFILO);
			_vm->replaceIcon(iGUANTI57, iGUANTI5A);
			StartCharacterAction(hUSEGG, 0, 0, 1756);
			*printSentence = false;
		} else if (_vm->_useWith[WITH] == iSIRINGA5A) {
			_vm->removeIcon(iSIRINGA5A);
			_vm->replaceIcon(iGUANTI57, iARMAEVA);
			StartCharacterAction(hUSEGG, 0, 0, 1756);
			*printSentence = false;
		}
		break;

	case iSIRINGA59:
		if (_vm->_useWith[WITH] == iFILO) {
			_vm->removeIcon(iFILO);
			_vm->replaceIcon(iSIRINGA59, iSIRINGA5A);
			StartCharacterAction(hUSEGG, 0, 0, 1756);
			*printSentence = false;
		} else if (_vm->_useWith[WITH] == iGUANTI5A) {
			_vm->removeIcon(iSIRINGA59);
			_vm->replaceIcon(iGUANTI5A, iARMAEVA);
			StartCharacterAction(hUSEGG, 0, 0, 1756);
			*printSentence = false;
		}
		break;

	case iGUANTI5A:
		if (_vm->_useWith[WITH] == iSIRINGA59) {
			_vm->removeIcon(iSIRINGA59);
			_vm->replaceIcon(iGUANTI5A, iARMAEVA);
			StartCharacterAction(hUSEGG, 0, 0, 1756);
			*printSentence = false;
		}
		break;

	case iSIRINGA5A:
		if (_vm->_useWith[WITH] == iGUANTI57) {
			_vm->removeIcon(iSIRINGA5A);
			_vm->replaceIcon(iGUANTI57, iARMAEVA);
			StartCharacterAction(hUSEGG, 0, 0, 1756);
			*printSentence = false;
		}
		break;

	default:
		*updateInv = false;
		break;
	}
}

void LogicManager::useWithScreen(bool *updateinv, bool *printsent) {
	*updateinv = true;
	*printsent = true;

	switch (_vm->_useWith[USED]) {
	case iBANCONOTE:
		if (_vm->_useWith[WITH] == oDISTRIBUTORE13 && !(_vm->_obj[oDISTRIBUTORE13]._flag & OBJFLAG_EXTRA)) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a131USABANCONOTA, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
			_vm->_obj[oDISTRIBUTORE13]._flag |= OBJFLAG_EXTRA;
		} else if ((_vm->_useWith[WITH] == oDISTRIBUTORE13) && (_vm->_obj[oDISTRIBUTORE13]._flag & OBJFLAG_EXTRA) && (_vm->_obj[oLATTINA13]._mode & OBJMODE_OBJSTATUS)) {
			CharacterSay(1410);
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oDISTRIBUTORE13) && (_vm->_obj[oDISTRIBUTORE13]._flag & OBJFLAG_EXTRA)) {
			if (!(_vm->_obj[oSCOMPARTO13]._flag & OBJFLAG_EXTRA)) {
				_vm->_obj[oSCOMPARTO13]._flag |= OBJFLAG_EXTRA;
				doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a1311DABOTTADISTRIBUTORE, 0, 0, _vm->_useWith[WITH]);
			} else
				CharacterSay(1411);

			*printsent = false;
		} else if (_vm->_useWith[WITH] == oTICKETOFFICE16) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a165USABANCONOTA, 0, 0, _vm->_useWith[WITH]);
			_vm->_inventoryObj[iBANCONOTE]._flag |= OBJFLAG_EXTRA;
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == ocPOLIZIOTTO16) && (_vm->_inventoryObj[iBANCONOTE]._flag & OBJFLAG_EXTRA)) {
			_vm->_choice[62]._flag &= ~DLGCHOICE_HIDE;
			PlayDialog(dPOLIZIOTTO16);
			*printsent = false;
		}
		break;
	case iLETTER12:
		if (_vm->_useWith[WITH] == oPENPADA13) {
			if (_vm->_room[r14]._flag & OBJFLAG_DONE)
				doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a1312METTELETTERARICALCA, r14, 14, _vm->_useWith[WITH]);
			else {
				doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a135METTELETTERA, 0, 0, _vm->_useWith[WITH]);
				_vm->_obj[oLETTERA13]._mode |= OBJMODE_OBJSTATUS;
				_vm->_inventoryObj[iLETTER12]._flag |= OBJFLAG_EXTRA;
				_vm->removeIcon(iLETTER12);
				RegenRoom();
			}
			*printsent = false;
		}
		break;
	case iFUSES:
		if (_vm->_useWith[WITH] == oPANELA12) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a127USEWRAPPEDFUSED, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		}
		break;
	case iFUSE:
		if (_vm->_useWith[WITH] == oPANELA12) {
			CharacterSay(62);
			*printsent = false;
		}
		break;
	case iKEY05:
		if (_vm->_useWith[WITH] == oBOXES12) {
			if (!(_vm->_obj[oBOXES12]._flag & OBJFLAG_EXTRA)) {
				doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a121BOXTEST, 0, 0, _vm->_useWith[WITH]);
				_vm->_obj[oBOXES12]._flag |= OBJFLAG_EXTRA;
				*printsent = false;
			} else {
				CharacterSay(1426);
				*printsent = false;
			}
		} else if (_vm->_useWith[WITH] == oBOX12 && !(_vm->_inventoryObj[iLETTER12]._flag & OBJFLAG_EXTRA)) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a122APREBOX, 0, 0, oBOX12);
			_vm->_inventoryObj[iLETTER12]._flag |= OBJFLAG_EXTRA;
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oBOX12) && (_vm->_inventoryObj[iLETTER12]._flag & OBJFLAG_EXTRA)) {
			CharacterSay(1429);
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oCARA11) || (_vm->_useWith[WITH] == oCARB11) || (_vm->_useWith[WITH] == oTAKE12) || (_vm->_useWith[WITH] == oSTRONGBOXC15) || (_vm->_useWith[WITH] == oDOOR18) || (_vm->_useWith[WITH] == oPADLOCK1B) || (_vm->_useWith[WITH] == oDOORC21) || (_vm->_useWith[WITH] == oPANELC23) || (_vm->_useWith[WITH] == oDOOR2A) || (_vm->_useWith[WITH] == oDOORC33) || (_vm->_useWith[WITH] == oFRONTOFFICEC35) || (_vm->_useWith[WITH] == oCASSETTOC36) || (_vm->_useWith[WITH] == oPORTAC54) || (_vm->_useWith[WITH] == oPORTA57C55) || (_vm->_useWith[WITH] == oPORTA58C55) || (_vm->_useWith[WITH] == oPORTAS56) || (_vm->_useWith[WITH] == oPORTAS57)) {
			*printsent = false;
			CharacterSay(1426);
		}
		break;
	case iCARD03:
		if (_vm->_useWith[WITH] == oSLOT12 || _vm->_useWith[WITH] == oSLOT13 || _vm->_useWith[WITH] == oSLOT16) {
			*printsent = false;
			_vm->_inventoryObj[iCARD03]._flag |= OBJFLAG_EXTRA;
			_vm->_obj[oSLOT12]._flag |= OBJFLAG_PERSON;
			_vm->_obj[oLIFTA12]._flag |= OBJFLAG_PERSON;
			doMouseTalk(_vm->_useWith[WITH]);
		} else if ((_vm->_useWith[WITH] == oTICKETOFFICE16) || (_vm->_useWith[WITH] == oSLOT23) || (_vm->_useWith[WITH] == oFRONTOFFICEA35) || (_vm->_useWith[WITH] == oSLOTA58) || (_vm->_useWith[WITH] == oSLOTB58)) {
			*printsent = false;
			CharacterSay(1419);
		}
		break;
	case iPEN:
		*printsent = false;
		if (((_vm->_useWith[WITH] == oPENPADA13) || (_vm->_useWith[WITH] == oLETTERA13)) && (_vm->_obj[oLETTERA13]._mode & OBJMODE_OBJSTATUS))
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a137RICALCAFIRMA, r14, 14, _vm->_useWith[WITH]);
		else if ((_vm->_useWith[WITH] == oPENPADA13) && (_vm->_room[r14]._flag & OBJFLAG_DONE))
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a1312METTELETTERARICALCA, r14, 14, _vm->_useWith[WITH]);
		else if (_vm->_useWith[WITH] == oPENPADA13) {
			if (!(_vm->_obj[oBOX12]._mode & OBJMODE_OBJSTATUS)) {
				*printsent = false;
				CharacterSay(2005);
			} else
				doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a134USAMAGNETICPEN, 0, 0, _vm->_useWith[WITH]);
		} else
			*printsent = true;
		break;

	case iACIDO15:
		if (_vm->_useWith[WITH] == oBAR11) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a113USAFIALA, 0, 0, _vm->_useWith[WITH]);
			_vm->replaceIcon(iACIDO15, iFIALAMETA);
			//_vm->_animMgr->_animTab[a113USAFIALA]._atFrame[0]._index = 1483;
			*printsent = false;
		} else if (_vm->_useWith[WITH] == oPADLOCK1B) {
			if (_vm->_obj[oTOMBINOA1B]._mode & OBJMODE_OBJSTATUS)
				doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a1B2AVERSAACIDO, 0, 0, _vm->_useWith[WITH]);
			else
				doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a1B2VERSAACIDO, 0, 0, _vm->_useWith[WITH]);
			_vm->_obj[oBOTOLAC1B]._anim = a1B3APREBOTOLA;
			_vm->replaceIcon(iACIDO15, iFIALAMETA);
			_vm->_animMgr->_animTab[a113USAFIALA]._atFrame[0]._index = 1483;
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == ocGUARD18) || (_vm->_useWith[WITH] == oMANHOLEC1B)) {
			*printsent = false;
			CharacterSay(1476);
		}
		break;

	case iFIALAMETA:
		if (_vm->_useWith[WITH] == oBAR11) {
			_vm->_animMgr->_animTab[a113USAFIALA]._atFrame[0]._index = 1483;
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a113USAFIALA, 0, 0, _vm->_useWith[WITH]);
			_vm->removeIcon(iFIALAMETA);
			*printsent = false;
		} else if (_vm->_useWith[WITH] == oPADLOCK1B) {
			_vm->_animMgr->_animTab[a1B2AVERSAACIDO]._atFrame[2]._index = 1483;
			_vm->_animMgr->_animTab[a1B2VERSAACIDO]._atFrame[2]._index = 1483;
			if (_vm->_obj[oTOMBINOA1B]._mode & OBJMODE_OBJSTATUS)
				doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a1B2AVERSAACIDO, 0, 0, _vm->_useWith[WITH]);
			else
				doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a1B2VERSAACIDO, 0, 0, _vm->_useWith[WITH]);
			_vm->_obj[oBOTOLAC1B]._anim = a1B3APREBOTOLA;
			_vm->removeIcon(iFIALAMETA);
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oDOOR2A) || (_vm->_useWith[WITH] == oPORTA2B)) {
			*printsent = false;
			CharacterSay(1508);
		}
		break;

	case iKEY15:
		if (_vm->_useWith[WITH] == oSTRONGBOXC15) {
			PlayDialog(dF151);
			_vm->_obj[oSTRONGBOXC15]._mode &= ~OBJMODE_OBJSTATUS;
			_vm->_obj[oCASSAFORTEA15]._mode |= OBJMODE_OBJSTATUS;
			_vm->_obj[oSLOT13]._mode &= ~OBJMODE_OBJSTATUS;
			_vm->_obj[oASCENSOREA13]._mode &= ~OBJMODE_OBJSTATUS;
			_vm->_obj[oASCENSOREC13]._mode |= OBJMODE_OBJSTATUS;
			_vm->addIcon(iPISTOLA15);
			_vm->addIcon(iACIDO15);
			_vm->addIcon(iRAPPORTO);
			_vm->removeIcon(iKEY15);
			*printsent = false;
			*updateinv = false;
		} else if ((_vm->_useWith[WITH] == oCARA11) || (_vm->_useWith[WITH] == oCARB11) || (_vm->_useWith[WITH] == oTAKE12) || (_vm->_useWith[WITH] == oBOX12) || (_vm->_useWith[WITH] == oDOOR18) || (_vm->_useWith[WITH] == oPADLOCK1B) || (_vm->_useWith[WITH] == oDOORC21) || (_vm->_useWith[WITH] == oPANELC23) || (_vm->_useWith[WITH] == oDOOR2A) || (_vm->_useWith[WITH] == oDOORC33) || (_vm->_useWith[WITH] == oFRONTOFFICEC35) || (_vm->_useWith[WITH] == oCASSETTOC36) || (_vm->_useWith[WITH] == oPORTAC54) || (_vm->_useWith[WITH] == oPORTA57C55) || (_vm->_useWith[WITH] == oPORTA58C55) || (_vm->_useWith[WITH] == oPORTAS56) || (_vm->_useWith[WITH] == oPORTAS57)) {
			*printsent = false;
			CharacterSay(1469);
		}
		break;

	case iBAR11:
		if (_vm->_useWith[WITH] == oMANHOLEC1B) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a1B1USASBARRA, 0, 0, _vm->_useWith[WITH]);
			_vm->_obj[oBOTOLAA1B]._anim = a1B6ASCENDEBOTOLA;
			//_vm->_obj[oBOTOLAC1B]._anim = a1B3AAPREBOTOLA;
			*printsent = false;
		} else if (_vm->_useWith[WITH] == oCATENAT21) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a216, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		} else if (_vm->_useWith[WITH] == oALTOPARLANTE25) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a251, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		} else if (_vm->_useWith[WITH] == oDOORC33) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a333LOSEBAR, 0, 0, _vm->_useWith[WITH]);
			_vm->removeIcon(iBAR11);
			*printsent = false;
		} else if (_vm->_obj[_vm->_useWith[WITH]]._flag & OBJFLAG_PERSON) {
			CharacterSay(1436);
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oTAKE12) || (_vm->_useWith[WITH] == oSTRONGBOXC15) || (_vm->_useWith[WITH] == oDOOR18) || (_vm->_useWith[WITH] == oPADLOCK1B) || (_vm->_useWith[WITH] == oDOORC21) || (_vm->_useWith[WITH] == oPANELC23) || (_vm->_useWith[WITH] == oDOOR2A) || (_vm->_useWith[WITH] == oPORTA2B)) {
			*printsent = false;
			CharacterSay(1435);
		}
		break;

	case iCARD14:
		if ((_vm->_useWith[WITH] == oTICKETOFFICE16) && (_vm->_obj[oMAPPA16]._flag & OBJFLAG_EXTRA)) {
			if (_vm->_choice[49]._flag & OBJFLAG_DONE) {
				CharacterSay(1457);
				*printsent = false;
			} else {
				_vm->_choice[46]._flag |= DLGCHOICE_HIDE;
				_vm->_choice[47]._flag |= DLGCHOICE_HIDE;
				_vm->_choice[48]._flag |= DLGCHOICE_HIDE;
				_vm->_choice[49]._flag &= ~DLGCHOICE_HIDE;
				_vm->PlayScript(s16CARD);
				//					doMouseTalk( _useWith[WITH] );
				*printsent = false;
			}
		} else if (_vm->_useWith[WITH] == oSLOT23) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a2311, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oSLOT12) || (_vm->_useWith[WITH] == oSLOT13) || (_vm->_useWith[WITH] == oSLOT16) || (_vm->_useWith[WITH] == oFRONTOFFICEA35) || (_vm->_useWith[WITH] == oSLOTA58) || (_vm->_useWith[WITH] == oSLOTB58)) {
			*printsent = false;
			CharacterSay(1419);
		}
		break;

	case iMONETA13:
		if ((_vm->_useWith[WITH] == oTICKETOFFICE16) && (_vm->_obj[oMAPPA16]._flag & OBJFLAG_EXTRA)) {
			_vm->_choice[46]._flag |= DLGCHOICE_HIDE;
			_vm->_choice[47]._flag |= DLGCHOICE_HIDE;
			_vm->_choice[48]._flag |= DLGCHOICE_HIDE;
			_vm->_choice[50]._flag &= ~DLGCHOICE_HIDE;
			_vm->PlayScript(S16MONEY);
			//				doMouseTalk( _useWith[WITH] );
			*printsent = false;
			_vm->removeIcon(iMONETA13);
			_vm->_obj[oFINGERPADP16]._flag |= OBJFLAG_ROOMOUT;
		} else if (_vm->_useWith[WITH] == oTICKETOFFICE16) {
			CharacterSay(146);
			*printsent = false;
		}
		break;

	case iPLASTICA:
		if (_vm->_useWith[WITH] == oTELEFAXF17) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a172USAPLASTICA, 0, 0, _vm->_useWith[WITH]);
			_vm->_obj[oTELEFAXF17]._examine = 1486;
			*printsent = false;
		}
		break;

	case iFOTO:
		if ((_vm->_useWith[WITH] == ocBARBONE17) && (_vm->_choice[81]._flag & OBJFLAG_DONE)) {
			CharacterSay(1463);
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == ocBARBONE17) && (_vm->_choice[91]._flag & OBJFLAG_DONE)) {
			_vm->_obj[ocBARBONE17]._action = 1462;
			CharacterSay(_vm->_obj[ocBARBONE17]._action);
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == ocBARBONE17) && (!(_vm->_choice[78]._flag & OBJFLAG_DONE) || ((_vm->_choice[79]._flag & OBJFLAG_DONE) || (_vm->_choice[83]._flag & OBJFLAG_DONE) && !(_vm->_choice[92]._flag & OBJFLAG_DONE)))) {
			_vm->_choice[78]._flag &= ~DLGCHOICE_HIDE;
			if ((_vm->_choice[79]._flag & OBJFLAG_DONE) || (_vm->_choice[83]._flag & OBJFLAG_DONE)) {
				_vm->_choice[92]._flag &= ~DLGCHOICE_HIDE;
				if (_vm->_choice[80]._flag & OBJFLAG_DONE)
					_vm->_choice[121]._flag &= ~DLGCHOICE_HIDE;
				else
					_vm->_choice[122]._flag &= ~DLGCHOICE_HIDE;
			} else
				_vm->_choice[91]._flag &= ~DLGCHOICE_HIDE;
			PlayDialog(dBARBONE171);
			*updateinv = false;
			*printsent = false;
		} else if (_vm->_useWith[WITH] == ocPOLIZIOTTO16) {
			_vm->_obj[ocPOLIZIOTTO16]._flag |= OBJFLAG_EXTRA;
			CharacterSay(1461);
			if ((_vm->_choice[61]._flag & OBJFLAG_DONE) && (_vm->_choice[62]._flag & OBJFLAG_DONE) && (_vm->_obj[ocPOLIZIOTTO16]._flag & OBJFLAG_EXTRA))
				_vm->_obj[ocPOLIZIOTTO16]._mode &= ~OBJMODE_OBJSTATUS;
			*printsent = false;
		} else if (_vm->_useWith[WITH] == ocGUARD18) {
			if (_vm->_choice[152]._flag & OBJFLAG_DONE)
				CharacterSay(1465);
			else
				CharacterSay(1464);
			*printsent = false;
		} else if (_vm->_useWith[WITH] == ocNEGOZIANTE1A) {
			CharacterSay(1466);
			*printsent = false;
		} else if (_vm->_useWith[WITH] == ocEVA19) {
			CharacterSay(1465);
			*printsent = false;
		}

		break;

	case iLATTINA13:
		if (_vm->_useWith[WITH] == ocBARBONE17) {
			if ((_vm->_choice[79]._flag & OBJFLAG_DONE) || (_vm->_choice[83]._flag & OBJFLAG_DONE)) {
				_vm->_choice[80]._flag &= ~DLGCHOICE_HIDE;
				if (_vm->_choice[81]._flag & OBJFLAG_DONE) {
					_vm->_choice[81]._flag &= ~DLGCHOICE_HIDE;
					_vm->_choice[80]._flag &= ~DLGCHOICE_EXITDLG;
				}
				PlayDialog(dBARBONE171);
				*updateinv = false;
				_vm->removeIcon(iLATTINA13);
				*printsent = false;
			} else { // if( !(_choice[79]._flag & OBJFLAG_DONE) )
				_vm->_choice[79]._flag &= ~DLGCHOICE_HIDE;
				if (!(_vm->_choice[78]._flag & OBJFLAG_DONE)) {
					_vm->_choice[106]._flag &= ~DLGCHOICE_HIDE;
					_vm->_obj[ocBARBONE17]._action = 213;
				} else {
					_vm->_choice[107]._flag &= ~DLGCHOICE_HIDE;
					if (_vm->_choice[80]._flag & OBJFLAG_DONE)
						_vm->_choice[121]._flag &= ~DLGCHOICE_HIDE;
					else
						_vm->_choice[122]._flag &= ~DLGCHOICE_HIDE;
				}
				PlayDialog(dBARBONE171);
				*updateinv = false;
				_vm->removeIcon(iLATTINA13);
				*printsent = false;
			}
		}

		break;

	case iBOTTIGLIA14:
		if (_vm->_useWith[WITH] == ocBARBONE17) {
			if ((_vm->_choice[79]._flag & OBJFLAG_DONE) || (_vm->_choice[83]._flag & OBJFLAG_DONE)) {
				_vm->_choice[80]._flag &= ~DLGCHOICE_HIDE;
				if (_vm->_choice[81]._flag & OBJFLAG_DONE) {
					_vm->_choice[81]._flag &= ~DLGCHOICE_HIDE;
					_vm->_choice[80]._flag &= ~DLGCHOICE_EXITDLG;
				}
				PlayDialog(dBARBONE171);
				*updateinv = false;
				_vm->removeIcon(iBOTTIGLIA14);
				*printsent = false;
			} else {
				_vm->_choice[83]._flag &= ~DLGCHOICE_HIDE;
				if (!(_vm->_choice[78]._flag & OBJFLAG_DONE)) {
					_vm->_choice[106]._flag &= ~DLGCHOICE_HIDE;
					_vm->_obj[ocBARBONE17]._action = 213;
				} else {
					_vm->_choice[107]._flag &= ~DLGCHOICE_HIDE;
					if (_vm->_choice[80]._flag & OBJFLAG_DONE)
						_vm->_choice[121]._flag &= ~DLGCHOICE_HIDE;
					else
						_vm->_choice[122]._flag &= ~DLGCHOICE_HIDE;
				}
				PlayDialog(dBARBONE171);
				*updateinv = false;
				_vm->removeIcon(iBOTTIGLIA14);
				*printsent = false;
			}
		}

		break;

	case iBOTTIGLIA1D:
		if (_vm->_useWith[WITH] == ocNEGOZIANTE1A) {
			*printsent = false;
			if (_vm->_choice[183]._flag & OBJFLAG_DONE) {
				_vm->_choice[185]._flag &= ~DLGCHOICE_HIDE;
				_dialog[dNEGOZIANTE1A]._startLen = 0;
				PlayDialog(dNEGOZIANTE1A);
				*updateinv = false;
				_dialog[dNEGOZIANTE1A]._startLen = 1;
				_vm->replaceIcon(iBOTTIGLIA1D, iTESSERA);
			} else
				CharacterSay(2006);
		}
		break;

	case iTESSERA:
		if ((_vm->_useWith[WITH] == ocGUARD18) && !(_vm->_choice[155]._flag & OBJFLAG_DONE)) {
			_vm->_choice[155]._flag &= ~DLGCHOICE_HIDE;
			PlayDialog(dGUARDIANO18);
			*updateinv = false;
			_vm->_obj[ocGUARD18]._flag &= ~OBJFLAG_PERSON;
			_vm->_obj[oPORTAC18]._flag |= OBJFLAG_ROOMOUT;
			*printsent = false;
		} else if (_vm->_useWith[WITH] == ocGUARD18) {
			CharacterSay(1494);
			*printsent = false;
		}
		break;

	case iTOPO1D:
		if ((_vm->_useWith[WITH] == oDONNA1D) && ((mx >= _vm->_obj[oDONNA1D]._lim[0]) && (my >= _vm->_obj[oDONNA1D]._lim[1] + TOP)
		&& (mx <= _vm->_obj[oDONNA1D]._lim[2]) && (my <= _vm->_obj[oDONNA1D]._lim[3] + TOP))) {
			PlayDialog(dF1D1);
			*updateinv = false;
			_vm->removeIcon(iTOPO1D);
			read3D("1d2.3d"); // after skate
			_vm->_obj[oDONNA1D]._mode &= ~OBJMODE_OBJSTATUS;
			_vm->_room[_vm->_curRoom]._flag |= OBJFLAG_EXTRA;
			_vm->_animMgr->_animTab[aBKG1D]._flag |= SMKANIM_OFF1;
			*printsent = false;
		}
		break;

	case iPISTOLA15:
		if ((_vm->_useWith[WITH] == oDOORC21) && !(_vm->_room[r21]._flag & OBJFLAG_EXTRA)) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a211, 0, 0, _vm->_useWith[WITH]);
			_vm->_inventoryObj[iPISTOLA15]._examine = 1472;
			_vm->_inventoryObj[iPISTOLA15]._action = 1473;
			*printsent = false;
		}
		break;

	case iCACCIAVITE:
		if (_vm->_useWith[WITH] == oESSE21) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a213, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		} else if (_vm->_useWith[WITH] == oCOPERCHIOC31) {
			NLPlaySound(wCOVER31);
			_vm->_obj[oCOPERCHIOC31]._mode &= ~OBJMODE_OBJSTATUS;
			_vm->_obj[oCOPERCHIOA31]._mode |= OBJMODE_OBJSTATUS;
			RegenRoom();
			*printsent = false;
		} else if (_vm->_useWith[WITH] == oCOPERCHIOA31) {
			_vm->_obj[oCOPERCHIOA31]._mode &= ~OBJMODE_OBJSTATUS;
			_vm->_obj[oCOPERCHIOC31]._mode |= OBJMODE_OBJSTATUS;
			_vm->_obj[oPANNELLOM31]._mode &= ~OBJMODE_OBJSTATUS;
			_vm->_obj[oPANNELLOMA31]._mode |= OBJMODE_OBJSTATUS;
			if (_vm->_obj[oFILOTC31]._mode & OBJMODE_OBJSTATUS) {
				_vm->_obj[oPANNELLOM31]._anim = 0;
				_vm->_obj[oPANNELLOM31]._examine = 715;
				_vm->_obj[oPANNELLOM31]._action = 716;
				_vm->_obj[oPANNELLOM31]._flag &= ~OBJFLAG_ROOMOUT;
			}
			NLPlaySound(wCOVER31);
			RegenRoom();
			*printsent = false;
			doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, r31, a3118CHIUDEPANNELLO, 3, _vm->_curObj);
		} else if (_vm->_useWith[WITH] == oPANNELLO55) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a5512, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		} else if (_vm->_useWith[WITH] == oPANNELLOC56) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a568, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oSLOT23) || (_vm->_useWith[WITH] == oPRESA35) || (_vm->_useWith[WITH] == oSERRATURA33)) {
			*printsent = false;
			CharacterSay(1520);
		}
		break;

	case iESSE:
		if (_vm->_useWith[WITH] == oCATENA21) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a214, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		}
		break;

	case iTANICHETTA27:
		if (_vm->_useWith[WITH] == oMANIGLIONE22) {
			if (_vm->_obj[oARMADIETTORC22]._mode & OBJMODE_OBJSTATUS)
				doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a228, 0, 0, _vm->_useWith[WITH]);
			else
				doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a228A, 0, 0, _vm->_useWith[WITH]);
			_vm->removeIcon(iTANICHETTA27);
			_vm->addIcon(iTANICHETTA22);
			if (_vm->_inventoryObj[iLATTINA28]._flag & OBJFLAG_EXTRA) {
				_vm->removeIcon(iTANICHETTA22);
				_vm->replaceIcon(iLATTINA28, iLATTINE);
			}
			*printsent = false;
		} else
			break;

	case iKEY22:
		if (_vm->_useWith[WITH] == oDOOR2A) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a2A2USEKEY, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oPANELC23) || (_vm->_useWith[WITH] == oDOORC33) || (_vm->_useWith[WITH] == oFRONTOFFICEC35) || (_vm->_useWith[WITH] == oCASSETTOC36) || (_vm->_useWith[WITH] == oPORTAC54) || (_vm->_useWith[WITH] == oPORTA57C55) || (_vm->_useWith[WITH] == oPORTA58C55) || (_vm->_useWith[WITH] == oPORTAS56) || (_vm->_useWith[WITH] == oPORTAS57)) {
			*printsent = false;
			CharacterSay(1512);
		}
		break;

	case iLAMPADINA29:
		if ((_vm->_useWith[WITH] == oPORTALAMPADE2B) && !(_vm->_obj[_vm->_useWith[WITH]]._anim)) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a2B6METTELAMPADINA, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		}
		break;

	case iPIPEWRENCH:
		if (_vm->_useWith[WITH] == oPANELC23) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a233, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oDOORC33) || (_vm->_useWith[WITH] == oFRONTOFFICEC35) || (_vm->_useWith[WITH] == oCASSETTOC36) || (_vm->_useWith[WITH] == oPORTAC54) || (_vm->_useWith[WITH] == oPORTA57C55) || (_vm->_useWith[WITH] == oPORTA58C55) || (_vm->_useWith[WITH] == oPORTAS56) || (_vm->_useWith[WITH] == oPORTAS57)) {
			*printsent = false;
			CharacterSay(1525);
		}
		break;

	case iCAVI:
		if ((_vm->_useWith[WITH] == oCONTATTI23) && (_vm->_obj[oLEVAS23]._mode & OBJMODE_OBJSTATUS)) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a236, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		} else if (_vm->_useWith[WITH] == oLEVAG23) {
			CharacterSay(2015);
			*printsent = false;
		}
		break;

	case iTRONCHESE:
		if ((_vm->_useWith[WITH] == oCAVO2H) && (_vm->_obj[oCARTELLONE2H]._mode & OBJMODE_OBJSTATUS)) {
			PlayDialog(dF2H1);
			_vm->_obj[oPASSAGE24]._mode |= OBJMODE_OBJSTATUS;
			_vm->_obj[omPASSAGGIO24]._mode |= OBJMODE_OBJSTATUS;
			_vm->_obj[oCARTELLONE24]._mode &= ~OBJMODE_OBJSTATUS;
			_vm->_obj[oCARTELLONE2H]._mode &= ~OBJMODE_OBJSTATUS;
			_vm->_obj[oPASSERELLA24]._mode &= ~OBJMODE_OBJSTATUS;
			_vm->_obj[oMACERIE24]._mode &= ~OBJMODE_OBJSTATUS;
			//_vm->_obj[oPASSERELLA24]._flag &= ~OBJFLAG_ROOMOUT;
			//_vm->_obj[oPASSERELLA24]._anim = 0;
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oTUBOT34) && (_vm->_obj[oVALVOLAC34]._mode & OBJMODE_OBJSTATUS)) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a341USAPINZE, 0, 0, _vm->_useWith[WITH]);
			//_obj[oVALVOLAC34]._anim = 0;
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oTUBOT34) && (_vm->_obj[oVALVOLA34]._mode & OBJMODE_OBJSTATUS)) {
			CharacterSay(2007);
			*printsent = false;
		} else
			*printsent = true;
		break;

	case iLATTINA28:
		if ((_vm->_useWith[WITH] == oSERBATOIOA2G) && !(_vm->_inventoryObj[iLATTINA28]._flag & OBJFLAG_EXTRA)) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a2G4VERSALATTINA, 0, 0, _vm->_useWith[WITH]);
			_vm->_inventoryObj[iLATTINA28]._flag |= OBJFLAG_EXTRA;
			_vm->_inventoryObj[iLATTINA28]._examine = 1537;
			if (_vm->iconPos(iTANICHETTA22) != MAXICON) {
				_vm->removeIcon(iTANICHETTA22);
				_vm->replaceIcon(iLATTINA28, iLATTINE);
			}
			if ((_vm->_inventoryObj[iBOMBOLA]._flag & OBJFLAG_EXTRA) && (_vm->_inventoryObj[iLATTINA28]._flag & OBJFLAG_EXTRA)) {
				_vm->_obj[oSERBATOIOA2G]._examine = 670;
				_vm->_obj[oSERBATOIOA2G]._action = 671;
			} else {
				_vm->_obj[oSERBATOIOA2G]._examine = 667;
				_vm->_obj[oSERBATOIOA2G]._action = 669;
			}
			*printsent = false;
		}
		break;

	case iBOMBOLA:
		if (_vm->_useWith[WITH] == oSERBATOIOA2G) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a2G5METTEBOMBOLA, 0, 0, _vm->_useWith[WITH]);
			_vm->_inventoryObj[iBOMBOLA]._flag |= OBJFLAG_EXTRA;
			if ((_vm->_inventoryObj[iBOMBOLA]._flag & OBJFLAG_EXTRA) && (_vm->_inventoryObj[iLATTINA28]._flag & OBJFLAG_EXTRA)) {
				_vm->_obj[oSERBATOIOA2G]._examine = 670;
				_vm->_obj[oSERBATOIOA2G]._action = 671;
			} else {
				_vm->_obj[oSERBATOIOA2G]._examine = 668;
				_vm->_obj[oSERBATOIOA2G]._action = 669;
			}
			*printsent = false;
		}
		break;

	case iCANDELOTTO:
		if ((_vm->_useWith[WITH] == oSERBATOIOA2G) && (_vm->_inventoryObj[iBOMBOLA]._flag & OBJFLAG_EXTRA) && (_vm->_inventoryObj[iLATTINA28]._flag & OBJFLAG_EXTRA)) {
			doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, r2GV, 0, 0, _vm->_useWith[WITH]);
			_vm->removeIcon(iCANDELOTTO);
			*printsent = false;
		}
		break;

	case iFUCILE:
		if (_vm->_useWith[WITH] == oDINOSAURO2E) {
			PlayDialog(dF2E2);
			_vm->_obj[oDINOSAURO2E]._mode &= ~OBJMODE_OBJSTATUS;
			_vm->_obj[oCATWALKA2E]._anim = a2E2PRIMAPALLONTANANDO;
			_vm->_animMgr->_animTab[aBKG2E]._flag |= SMKANIM_OFF2;
			*printsent = false;
		}
		break;

	case iPINZA:
	case iSBARRA21:
		if (_vm->_useWith[WITH] == oCATENAT21) {
			if (_vm->_room[_vm->_curRoom]._flag & OBJFLAG_EXTRA) {
				if (_vm->_useWith[USED] == iPINZA)
					PlayDialog(dF212B);
				else
					PlayDialog(dF212);
				*printsent = false;
			} else {
				if (_vm->_useWith[USED] == iPINZA)
					PlayDialog(dF213B);
				else
					PlayDialog(dF213);
				*printsent = false;
			}
		} else if ((_vm->_useWith[WITH] == oDOORC33) && (_vm->_useWith[USED] == iSBARRA21)) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a333LOSEBAR, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oSNAKEU52) && (_vm->_useWith[USED] == iPINZA)) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a527, 0, 0, _vm->_useWith[WITH]);
			_vm->_obj[oSCAVO51]._anim = a516;
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oSERPENTEA52) && (_vm->_useWith[USED] == iPINZA)) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a528, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oSERPENTEB52) && (_vm->_useWith[USED] == iPINZA) && (_vm->iconPos(iSERPENTEA) == MAXICON)) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a523, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		}
		break;

	case iLAMPADINA2B:
		if (_vm->_useWith[WITH] == oPORTALAMPADE29) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a293AVVITALAMPADINA, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		}
		break;

	case iLATTINA27:
		if ((_vm->_useWith[WITH] == oBRACIERES28) && (_vm->_obj[oBRACIERES28]._flag & OBJFLAG_EXTRA)) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a286, 0, 0, _vm->_useWith[WITH]);
			_vm->_obj[oBRACIERES28]._examine = 456;
			*printsent = false;
		} else
			*printsent = true;
		break;

	case iTELECOMANDO2G:
		if (_vm->_useWith[WITH] == oTASTO2F) {
			if (_vm->_obj[oASCENSORE2F]._mode & OBJMODE_OBJSTATUS) {
				doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a2F10PANNELLOSICHIUDE, 0, 0, _vm->_useWith[WITH]);
				_vm->_obj[oBIDONE2F]._anim = a2F5CFRUGABIDONE;
			} else {
				if (!(_vm->_inventoryObj[iTELECOMANDO2G]._flag & OBJFLAG_EXTRA))
					doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a2F9PPANNELLOSIAPRE, 0, 0, _vm->_useWith[WITH]);
				else
					doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a2F9PANNELLOSIAPRE, 0, 0, _vm->_useWith[WITH]);
				_vm->_inventoryObj[iTELECOMANDO2G]._flag |= OBJFLAG_EXTRA;
				_vm->_obj[oBIDONE2F]._anim = a2F5FRUGABIDONE;
			}
			*printsent = false;
		}
		break;

	case iSAMROTTO:
		if (_vm->_useWith[WITH] == oSERRATURA33) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a338POSASAM, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		}
		break;

	case iTORCIA32:
		if ((_vm->_useWith[WITH] == oSENSOREV32) && (_vm->_obj[oFILOTC31]._mode & OBJMODE_OBJSTATUS)) {
			_vm->_obj[oPANNELLOMA31]._mode |= OBJMODE_OBJSTATUS;
			_vm->_obj[oPANNELLOM31]._mode &= ~OBJMODE_OBJSTATUS;
			_vm->_obj[oPANNELLOMA31]._examine = 717;
			_vm->_obj[oPANNELLOMA31]._action = 718;
			_vm->_obj[oCORPO31]._mode |= OBJMODE_OBJSTATUS;
			_vm->_obj[od31ALLA35]._mode |= OBJMODE_OBJSTATUS;
			_vm->_obj[omd31ALLA35]._mode |= OBJMODE_OBJSTATUS;
			_vm->_obj[oMONTACARICHI31]._mode |= OBJMODE_OBJSTATUS;
			_vm->_obj[oPANNELLO31]._mode &= ~OBJMODE_OBJSTATUS;
			_vm->_obj[oPANNELLON31]._mode &= ~OBJMODE_OBJSTATUS;
			_vm->_room[r32]._flag |= OBJFLAG_EXTRA;
			doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, r31, 0, 11, _vm->_useWith[WITH]);

			*printsent = false;
		}
		break;

	case iPROIETTORE31:
		if (_vm->_useWith[WITH] == oPRESA35) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a351PROVAPROIETTOREPRESA, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		} else if (_vm->_useWith[WITH] == oTRIPLA35) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a355ATTPROIETTORETRIPLAEPRESA, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oRIBELLEA35)) {
			*printsent = false;
			CharacterSay(1578);
		}
		break;

	case iPROIETTORE35:
		if (_vm->_useWith[WITH] == oPRESA35) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a352ATTPROIETTOREETRIPLAPRESA, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oRIBELLEA35)) {
			*printsent = false;
			CharacterSay(1590);
		}
		break;

	case iTRIPLA:
		if (_vm->_useWith[WITH] == oPRESA35) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a353ATTACCATRIPLAPRESA, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		}
		break;

	case iLASER35:
		if (_vm->_useWith[WITH] == oFRONTOFFICEC35) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a3511APRESPORTELLO, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		} else if (_vm->_useWith[WITH] == oSNAKEU52) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a522, 0, 0, _vm->_useWith[WITH]);
			_vm->_obj[oSCAVO51]._anim = a516;
			*printsent = false;
		} else if (_vm->_useWith[WITH] == oLUCCHETTO53) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a532, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oPORTAMC36) || (_vm->_useWith[WITH] == oPORTALC36) || (_vm->_useWith[WITH] == oSCANNERMA36) || (_vm->_useWith[WITH] == oSCANNERLA36) || (_vm->_useWith[WITH] == oCASSETTOC36) || (_vm->_useWith[WITH] == oRETE52) || (_vm->_useWith[WITH] == oTELECAMERA52) || (_vm->_useWith[WITH] == oSERPENTET52) || (_vm->_useWith[WITH] == oLAGO53)) {
			*printsent = false;
			CharacterSay(1597);
		}
		break;

	case iKEY35:
		if (_vm->_useWith[WITH] == oCASSETTOC36) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a364APRECASSETTO, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oFRONTOFFICEC35) || (_vm->_useWith[WITH] == oPORTAC54) || (_vm->_useWith[WITH] == oPORTA57C55) || (_vm->_useWith[WITH] == oPORTA58C55) || (_vm->_useWith[WITH] == oPORTAS56) || (_vm->_useWith[WITH] == oPORTAS57)) {
			*printsent = false;
			CharacterSay(1594);
		}
		break;

	case iSTETOSCOPIO:
		if (_vm->_useWith[WITH] == oPORTALC36) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a368USASTETOSCOPIO, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		}
		break;

	case iCARD36:
		if ((_vm->_useWith[WITH] == oFRONTOFFICEA35) && !(_vm->_obj[oFRONTOFFICEA35]._flag & OBJFLAG_EXTRA)) {
			doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, r35P, 0, 10, _vm->_useWith[WITH]);
			_vm->removeIcon(iCARD36);
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oFRONTOFFICEA35) && (_vm->_obj[oFRONTOFFICEA35]._flag & OBJFLAG_EXTRA)) {
			CharacterSay(1844);
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oSLOTA58) || (_vm->_useWith[WITH] == oSLOTB58)) {
			*printsent = false;
			CharacterSay(1602);
		}
		break;

	case iMONETA4L:
		if ((_vm->_useWith[WITH] == oFESSURA41) &&
			((_vm->_obj[oFUCILE42]._anim != 0) && (_vm->_obj[oFUCILE42]._anim != a428) && (_vm->_obj[oFUCILE42]._anim != a429))) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a412, 0, 0, _vm->_useWith[WITH]);
			if (_vm->_obj[oZAMPA41]._mode & OBJMODE_OBJSTATUS)
				_vm->_obj[oSLOT41]._anim = a417;
			else if (_vm->_slotMachine41Counter <= 2)
				_vm->_obj[oSLOT41]._anim = a414;
			else
				CharacterSay(2015);
			_vm->_slotMachine41Counter++;
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oFESSURA41) && ((_vm->_obj[oFUCILE42]._anim == 0) || (_vm->_obj[oFUCILE42]._anim == a428) || (_vm->_obj[oFUCILE42]._anim == a429))) {
			CharacterSay(2010);
			*printsent = false;
		} else if (_vm->_useWith[WITH] == oFESSURA42) {
			CharacterSay(924);
			*printsent = false;
		} else if (_vm->_useWith[WITH] == oCAMPANA4U) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a4U3, 0, 0, _vm->_useWith[WITH]);
			_vm->_obj[oCAMPANA4U]._flag |= OBJFLAG_EXTRA;
			if ((_vm->_obj[oCAMPANA4U]._flag & OBJFLAG_EXTRA) && (_vm->_inventoryObj[iBIGLIAA]._flag & OBJFLAG_EXTRA)) {
				_vm->_obj[oCAMPANA4U]._examine = 1202;
				_vm->_obj[oCAMPANA4U]._action = 1203;
			} else
				_vm->_obj[oCAMPANA4U]._examine = 1200;
			*printsent = false;
		}
		break;

	case iMARTELLO:
		if ((_vm->_useWith[WITH] == oRAGNO41) && !(_vm->_obj[oRAGNO41]._flag & OBJFLAG_EXTRA)) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a416, 0, 0, _vm->_useWith[WITH]);
			_vm->_obj[oRAGNO41]._flag |= OBJFLAG_EXTRA;
			_vm->_obj[oRAGNO41]._anim = 0;
			if (_vm->_obj[oSLOT41]._anim == a414)
				_vm->_obj[oSLOT41]._anim = a417;
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oSLOT41) || (_vm->_useWith[WITH] == oVETRINETTA42) || (_vm->_useWith[WITH] == oTAMBURO43) || (_vm->_useWith[WITH] == oSFIATO45) || (_vm->_useWith[WITH] == oPORTAC4A) || (_vm->_useWith[WITH] == oPORTAC4B) || (_vm->_useWith[WITH] == oSERRATURA4B) || (_vm->_useWith[WITH] == oLICANTROPO4P)) {
			*printsent = false;
			CharacterSay(1619);
		}
		break;

	case iMONETE:
		if ((_vm->_useWith[WITH] == oFESSURA42) && (_vm->_obj[oFUCILE42]._anim == a427)) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a421, 0, 0, _vm->_useWith[WITH]);
			if (_vm->_obj[oPOLTIGLIA42]._mode & OBJMODE_OBJSTATUS)
				_vm->_obj[oFUCILE42]._anim = a429;
			else
				_vm->_obj[oFUCILE42]._anim = a428;
			*printsent = false;
		}
		break;

	case iPOLTIGLIA:
		if (_vm->_useWith[WITH] == oGUIDE42) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a423, 0, 0, _vm->_useWith[WITH]);
			if (_vm->_obj[oFUCILE42]._anim != a427)
				_vm->_obj[oFUCILE42]._anim = a429;
			*printsent = false;
		}
		break;

	case iMAZZA:
		if ((_vm->_useWith[WITH] == oTAMBURO43) && !(_vm->_obj[oTAMBURO43]._flag & OBJFLAG_EXTRA)) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a435, 0, 0, _vm->_useWith[WITH]);
			_vm->_obj[oTAMBURO43]._flag |= OBJFLAG_EXTRA;
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oPORTAC4B) || (_vm->_useWith[WITH] == oSERRATURA4B) || (_vm->_useWith[WITH] == oLICANTROPO4P)) {
			*printsent = false;
			CharacterSay(1679);
		}
		break;

	case iPUPAZZO:
		if (_vm->_useWith[WITH] == oCASSETTOAA44) {
			_vm->replaceIcon(iPUPAZZO, iTELECOMANDO44);
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a442, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		}
		break;

	case iSTRACCIO:
		if (_vm->_useWith[WITH] == oMANOPOLAR45) {
			doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, r45S, 0, 2, _vm->_useWith[WITH]);
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oCAMPANA4U) && (_vm->_inventoryObj[iBIGLIAA]._flag & OBJFLAG_EXTRA) && (_vm->_obj[oCAMPANA4U]._flag & OBJFLAG_EXTRA)) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a4U5, 0, 0, _vm->_useWith[WITH]);
			_vm->_obj[oCAMPANA4U]._examine = 1204;
			_vm->_obj[oCAMPANA4U]._action = 1205;
			*printsent = false;
		} else if (_vm->_useWith[WITH] == oCAMPANA4U) {
			CharacterSay(1713);
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oRAGNO46) || (_vm->_useWith[WITH] == oLICANTROPO4P)) {
			*printsent = false;
			CharacterSay(1711);
		}
		break;

	case iTESCHIO:
		if (_vm->_useWith[WITH] == oPIASTRELLA48) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a4810, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		}
		break;

	case iTORCIA47:
		if (_vm->_useWith[WITH] == oTESCHIO48) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a4811, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oCALDAIAS45) || (_vm->_useWith[WITH] == oRAGNO46)) {
			*printsent = false;
			CharacterSay(1640);
		}
		break;

	case iFIAMMIFERO:
		if (_vm->_useWith[WITH] == oTORCIAS48) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a4812, 0, 0, _vm->_useWith[WITH]);
			_vm->_obj[oTORCIAS48]._lim[0] = 0;
			_vm->_obj[oTORCIAS48]._lim[1] = 0;
			_vm->_obj[oTORCIAS48]._lim[2] = 0;
			_vm->_obj[oTORCIAS48]._lim[3] = 0;
			*printsent = false;
		}
		break;

	case iASTA:
		if (_vm->_useWith[WITH] == oMERIDIANA49) {
			_vm->removeIcon(iASTA);
			StartCharacterAction(a491, r49M, 1, 0);
			*printsent = false;
		}
		break;

	case iPISTOLA4PD:
		if ((_vm->_useWith[WITH] == oLICANTROPO4P) && (_vm->_inventoryObj[iPISTOLA4PD]._flag & OBJFLAG_EXTRA)) {
			_vm->replaceIcon(iPISTOLA4PD, iPISTOLA4B);
			_vm->_obj[oLICANTROPO4P]._mode &= ~OBJMODE_OBJSTATUS;
			_vm->_obj[oLICANTROPOM4P]._mode |= OBJMODE_OBJSTATUS;
			_vm->_obj[oSANGUE4P]._mode |= OBJMODE_OBJSTATUS;
			_vm->_room[r4P]._flag |= OBJFLAG_EXTRA;
			read3D("4p2.3d"); // after werewolf
			_vm->_animMgr->_animTab[aBKG4P]._flag |= SMKANIM_OFF1;
			PlayDialog(dF4P2);
			*printsent = false;
		} else if (_vm->_useWith[WITH] == oLICANTROPO4P) {
			_vm->replaceIcon(iPISTOLA4PD, iPISTOLA4B);
			PlayDialog(dF4P1);
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oRAGNO46) || (_vm->_useWith[WITH] == oPORTAC4B) || (_vm->_useWith[WITH] == oSERRATURA4B)) {
			*printsent = false;
			CharacterSay(1706);
		}
		break;

	case iBARATTOLO:
		if ((_vm->_useWith[WITH] == oSANGUE4P) || (_vm->_useWith[WITH] == oLICANTROPOM4P)) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a4P7, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		}
		break;

	case iCAMPANA:
		if (_vm->_useWith[WITH] == oPOZZA4U) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a4U2, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		}
		break;

	case iBIGLIAA:
	case iBIGLIAB:
		if ((_vm->_useWith[WITH] == oCAMPANA4U) && (_vm->_inventoryObj[iBIGLIAA]._flag & OBJFLAG_EXTRA)) {
			CharacterSay(1684);
			*printsent = false;
		} else if (_vm->_useWith[WITH] == oCAMPANA4U) {
			_vm->removeIcon(_vm->_useWith[USED]);
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a4U4, 0, 0, _vm->_useWith[WITH]);
			_vm->_inventoryObj[iBIGLIAA]._flag |= OBJFLAG_EXTRA;
			if ((_vm->_obj[oCAMPANA4U]._flag & OBJFLAG_EXTRA) && (_vm->_inventoryObj[iBIGLIAA]._flag & OBJFLAG_EXTRA)) {
				_vm->_obj[oCAMPANA4U]._examine = 1202;
				_vm->_obj[oCAMPANA4U]._action = 1203;
			} else
				_vm->_obj[oCAMPANA4U]._examine = 1201;
			*printsent = false;
		}
		break;

	case iPAPAVERINA:
		if ((_vm->_useWith[WITH] == oCIOCCOLATINI4A) && ((_vm->_animMgr->_curAnimFrame[0] < 370) || (_vm->_animMgr->_curAnimFrame[0] > 480))) {
			_vm->PlayScript(s4AHELLEN);
			_vm->_obj[oPULSANTE4A]._anim = a4A3;
			*printsent = false;
		} else if (_vm->_useWith[WITH] == oCIOCCOLATINI4A) {
			_vm->_obj[oPULSANTE4A]._anim = a4A3;
			*printsent = false;
			_vm->_obj[oCIOCCOLATINI4A]._flag |= OBJFLAG_EXTRA;
		}
		break;

	case iSANGUE:
		if (_vm->_useWith[WITH] == oSERRATURA4B) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a4B4, 0, 0, _vm->_useWith[WITH]);
			_vm->_obj[oPORTAC4B]._anim = a4B5;
			*printsent = false;
		}
		break;

	case iDIAPA4B:
		if ((_vm->_useWith[WITH] == oPROIETTORE4B) && (_vm->_obj[oPROIETTORE4B]._anim < a4B9A)) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a4B6A, 0, 0, _vm->_useWith[WITH]);
			_vm->_obj[oPROIETTORE4B]._anim = a4B9A;
			*printsent = false;
		}
		break;

	case iDIAPB4B:
		if ((_vm->_useWith[WITH] == oPROIETTORE4B) && (_vm->_obj[oPROIETTORE4B]._anim < a4B9A)) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a4B6B, 0, 0, _vm->_useWith[WITH]);
			_vm->_obj[oPROIETTORE4B]._anim = a4B9B;
			*printsent = false;
		}
		break;

	case iDIAP4C:
		if ((_vm->_useWith[WITH] == oPROIETTORE4B) && (_vm->_obj[oPROIETTORE4B]._anim < a4B9A)) {
			//
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a4B6C, 0, 0, _vm->_useWith[WITH]);
			_vm->_obj[oPROIETTORE4B]._anim = a4B9C;
			*printsent = false;
		}
		break;

	case iUOVO:
		if ((_vm->_useWith[WITH] == oRETE52) || (_vm->_useWith[WITH] == oSERPENTET52)) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a521, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		}
		break;

	case iSERPENTEA:
		if (_vm->_useWith[WITH] == oLAGO53) {
			*printsent = false;
			if (!(_vm->_obj[oLUCCHETTO53]._mode & OBJMODE_OBJSTATUS)) {
				StartCharacterAction(a533, r54, 11, 0);
				_vm->removeIcon(_vm->_useWith[USED]);
			} else if (_vm->_useWith[USED] != iSERPENTEB) {
				doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a534, 0, 0, _vm->_useWith[WITH]);
				_vm->removeIcon(_vm->_useWith[USED]);
				_vm->_obj[oLAGO53]._examine = 1237;
			} else
				CharacterSay(1740);
		}
		break;

	case iSERPENTEB:
		if (_vm->_useWith[WITH] == oLAGO53) {
			*printsent = false;
			if (!(_vm->_obj[oLUCCHETTO53]._mode & OBJMODE_OBJSTATUS)) {
				StartCharacterAction(a533C, r54, 11, 0);
				_vm->removeIcon(_vm->_useWith[USED]);
			} else if (_vm->_useWith[USED] != iSERPENTEB) {
				doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a534, 0, 0, _vm->_useWith[WITH]);
				_vm->removeIcon(_vm->_useWith[USED]);
				_vm->_obj[oLAGO53]._examine = 1237;
			} else
				CharacterSay(1740);
		}
		break;

	case iSAPONE:
		if (_vm->_useWith[WITH] == oSECCHIOA54) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a543, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		}
		break;

	case iLATTINE:
		if (_vm->_useWith[WITH] == oLAVATRICEF54) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a546, 0, 0, _vm->_useWith[WITH]);
			_vm->_obj[oLAVATRICEL54]._anim = a547;
			*printsent = false;
		}
		break;

	case iCHIAVI:
		if (_vm->_useWith[WITH] == oPORTAS56) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a563, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		} else if (_vm->_useWith[WITH] == oPORTA57C55) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a551, r57, 17, _vm->_useWith[WITH]);
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oPORTA58C55) && (!(_vm->_choice[871]._flag & OBJFLAG_DONE) || (_vm->_choice[901]._flag & OBJFLAG_DONE))) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a552, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oPORTA58C55) && (_vm->_choice[871]._flag & OBJFLAG_DONE)) {
			CharacterSay(1287);
			*printsent = false;
		}
		break;

	case iMDVD:
		if ((_vm->_useWith[WITH] == oTASTIERA56) && (_vm->_choice[260]._flag & OBJFLAG_DONE) && !(_vm->_choice[262]._flag & OBJFLAG_DONE) && (_vm->_inventoryObj[iMDVD]._examine != 1752)) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a566, 0, 0, _vm->_useWith[WITH]);
			_vm->_choice[262]._flag &= ~DLGCHOICE_HIDE;
			_vm->_inventoryObj[iMDVD]._examine = 1752;
			*printsent = false;
		} else if ((_vm->_useWith[WITH] == oTASTIERA56) && (_vm->_inventoryObj[iMDVD]._examine == 1752)) {
			CharacterSay(1753);
			*printsent = false;
		} else
			*printsent = true;
		break;

	case iTESTER:
		if ((_vm->_useWith[WITH] == oPANNELLOA) && (_vm->_choice[856]._flag & OBJFLAG_DONE)) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a569, 0, 0, _vm->_useWith[WITH]);
			_vm->_obj[oPANNELLOA]._flag |= OBJFLAG_EXTRA;
			*printsent = false;
		}
		break;

	case iCUTTER:
		if ((_vm->_useWith[WITH] == oPANNELLOA) && (_vm->_obj[oPANNELLOA]._flag & OBJFLAG_EXTRA)) {
			PlayDialog(dF562);
			_vm->_obj[oPANNELLOA]._mode &= ~OBJMODE_OBJSTATUS;
			_vm->_obj[oCAVOTAGLIATO56]._mode |= OBJMODE_OBJSTATUS;
			_vm->_obj[oPORTA58C55]._mode |= OBJMODE_OBJSTATUS;
			setPosition(6);
			_vm->removeIcon(iCUTTER);
			*printsent = false;
		} else if (_vm->_useWith[WITH] == oPANNELLOA) {
			CharacterSay(2012);
			*printsent = false;
		}
		break;

	case iGUANTI59:
		if (_vm->_useWith[WITH] == oBOMBOLA57) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a575, 0, 0, _vm->_useWith[WITH]);
			*printsent = false;
		}
		break;

	case iARMAEVA:
		if ((_vm->_useWith[WITH] == oFINESTRAA5A) && (_vm->_choice[871]._flag & OBJFLAG_DONE) && !(_vm->_choice[286]._flag & OBJFLAG_DONE)) {
			_vm->removeIcon(iARMAEVA);
			PlayDialog(dC5A1);
			_vm->_obj[oFINESTRAA58]._anim = a587;
			*printsent = false;
			_vm->_room[r5A]._flag |= OBJFLAG_EXTRA;
		} else if ((_vm->_useWith[WITH] == oFINESTRAA5A) && (_vm->_choice[871]._flag & OBJFLAG_DONE)) {
			_vm->removeIcon(iARMAEVA);
			PlayDialog(dF5A1);
			_vm->_obj[oFINESTRAA58]._anim = a587;
			*printsent = false;
		}
		break;

	default:
		*updateinv = false;
		break;
	}
}

void LogicManager::roomOut(uint16 curObj, uint16 *action, uint16 *pos) {
	if (curObj == oSCALA32 && g_vm->_obj[oBOTOLAC32]._mode & OBJMODE_OBJSTATUS) {
		CharacterSay(g_vm->_obj[curObj]._action);
		g_vm->_flagMouseEnabled = true;
		*action = 0;
		*pos = 0;
	} else {
		*action = g_vm->_obj[curObj]._anim;
		*pos = g_vm->_obj[curObj]._ninv;
	}
}

bool LogicManager::mouseExamine(uint16 curObj) {
	bool retVal = false;

	switch (curObj) {
	case oMAPPA12:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a1211OSSERVAMAPPAPALAZZO, 0, 0, curObj);
		g_vm->_choice[4]._flag &= ~DLGCHOICE_HIDE;
		g_vm->_choice[18]._flag &= ~DLGCHOICE_HIDE;
		g_vm->_choice[33]._flag &= ~DLGCHOICE_HIDE;
		retVal = false;
		break;

	case oPORTAA13:
		g_vm->_obj[oBOX12]._mode |= OBJMODE_OBJSTATUS;
		retVal = true;
		break;

	case oPANELA12:
	case oFUSIBILE12:
		if (g_vm->_obj[oFUSIBILE12]._mode & OBJMODE_OBJSTATUS)
			g_vm->_obj[oFUSE12CU]._mode |= OBJMODE_OBJSTATUS;
		else
			g_vm->_obj[oFUSE12CU]._mode &= ~OBJMODE_OBJSTATUS;
		doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, r12CU, 0, 0, curObj);
		g_vm->_closeUpObj = curObj;
		break;

	case oLETTERA13:
	case oPENPADA13:
		if (g_vm->_obj[oLETTERA13]._mode & OBJMODE_OBJSTATUS)
			g_vm->_obj[oLETTER13CU]._mode |= OBJMODE_OBJSTATUS;
		else
			g_vm->_obj[oLETTER13CU]._mode &= ~OBJMODE_OBJSTATUS;
		doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, r13CU, 0, 0, curObj);
		g_vm->_closeUpObj = curObj;
		break;

	case oCUCININO14:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a1413OSSERVAPIANOCOTTURA, 0, 0, curObj);
		retVal = false;
		break;

	case oSCAFFALE14:
		if (!(g_vm->_obj[oPORTAR14]._mode & OBJMODE_OBJSTATUS))
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a145ESAMINASCAFFALE, 0, 0, curObj);
		else
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a145CESAMINASCAFFALE, 0, 0, curObj);
		retVal = false;
		break;

	case oTAVOLINOPP14:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a1412SPOSTASEDIA, 0, 0, curObj);
		retVal = false;
		break;

	case oBOCCETTE15:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a1510TOCCABOCCETTE, 0, 0, curObj);
		retVal = false;
		break;

	case oSPECCHIO15:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a1511SISPECCHIA, 0, 0, curObj);
		retVal = false;
		break;

	case oMONITORSA16:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a1614GUARDAMONITORS14, 0, 0, curObj);
		retVal = false;
		break;

	case oFINESTRAA15:
		if (g_vm->_obj[oTAPPARELLAA15]._mode & OBJMODE_OBJSTATUS)
			CharacterSay(1999);
		else
			retVal = true;
		break;

	case oMONITORSP16:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a1615GUARDAMONITORS15, 0, 0, curObj);
		retVal = false;
		break;

	case oCARTACCE16:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a1613GUARDACARTACCE, 0, 0, curObj);
		retVal = false;
		break;

	case oMAPPA16:
		if (g_vm->iconPos(iFOGLIO14) != MAXICON)
			g_vm->_obj[oMAPPA16]._flag |= OBJFLAG_EXTRA;
		retVal = true;
		break;

	case oSCATOLONE17:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a177SICHINA, 0, 0, curObj);
		retVal = false;
		break;

	case oMURALES17:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a179MUOVETESTA, 0, 0, curObj);
		retVal = false;
		break;

	case oSCHERMO18:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a181ESAMINACARTELLONE, 0, 0, curObj);
		retVal = false;
		break;

	case oVETRINA1A:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a1A1ESAMINAVETRINA, 0, 0, curObj);
		retVal = false;
		break;

	case oTESSERA1A:
		if (((g_vm->_choice[151]._flag & OBJFLAG_DONE) || (g_vm->_choice[152]._flag & OBJFLAG_DONE)) && !(g_vm->_choice[183]._flag & OBJFLAG_DONE))
			g_vm->_choice[183]._flag &= ~DLGCHOICE_HIDE;
		g_vm->_obj[oTESSERA1A]._flag |= OBJFLAG_EXTRA;
		retVal = true;
		break;

	case oCARTACCE1B:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a1B8FRUGACARTACCIE, 0, 0, curObj);
		retVal = false;
		break;

	case oBIDONE1B:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a1B10GUARDABIDONE, 0, 0, curObj);
		retVal = false;
		break;

	case oGRATA1C:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a1C2GUARDAGRATA, 0, 0, curObj);
		retVal = false;
		break;

	case oSCAFFALE1D:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a1D8SALESGABELLO, 0, 0, curObj);
		retVal = false;
		break;

	case oBARILOTTO1D:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a1D7SPINGEBARILOTTO, 0, 0, curObj);
		retVal = false;
		break;

	case oCASSA1D:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a1D10ESAMINABOTTIGLIE, 0, 0, curObj);
		retVal = false;
		break;

	case oSCATOLETTA23:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a2310, 0, 0, curObj);
		retVal = false;
		break;

	case oPALMA26:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a262, 0, 0, curObj);
		retVal = false;
		break;

	case oINSEGNA26:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a263, 0, 0, curObj);
		retVal = false;
		break;

	case oTEMPIO28:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a2810, 0, 0, curObj);
		retVal = false;
		break;

	case oSERPENTET28:
	case oSERPENTEA28:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a2811, 0, 0, curObj);
		retVal = false;
		break;

	case oSERPENTE2B:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a2B11ESAMINASERPENTE, 0, 0, curObj);
		retVal = false;
		break;

	case oLEOPARDO2B:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a2B9ESAMINALEOPARDO, 0, 0, curObj);
		retVal = false;
		break;

	case oPELLICANO2B:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a2B10ESAMINAPELLICANO, 0, 0, curObj);
		retVal = false;
		break;

	case oBACHECA2B:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a2B13OSSERVAREFARFALLE, 0, 0, curObj);
		retVal = false;
		break;

	case oROBOT2F:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a2F6TOCCADINOSAURO, 0, 0, curObj);
		retVal = false;
		break;

	case oCREPACCIO2E:
		if (g_vm->_room[r2E]._flag & OBJFLAG_EXTRA)
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a2E7GUARDACREPACCIODILA, 0, 0, curObj);
		else
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a2E6GUARDACREPACCIODIQUA, 0, 0, curObj);
		retVal = false;
		break;

	case oGENERATORE34:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a347ESAMINAGENERATORE, 0, 0, curObj);
		retVal = false;
		break;

	case oTUBOF33:
		if ((g_vm->_obj[oVALVOLA34]._mode & OBJMODE_OBJSTATUS) && g_vm->_obj[oVALVOLA34]._anim)
			CharacterSay(2000);
		else
			CharacterSay(g_vm->_obj[curObj]._examine);
		retVal = false;
		break;

	case oTUBOT33:
		if ((g_vm->_obj[oVALVOLA34]._mode & OBJMODE_OBJSTATUS) && g_vm->_obj[oVALVOLA34]._anim)
			CharacterSay(2001);
		else
			CharacterSay(g_vm->_obj[curObj]._examine);
		retVal = false;
		break;

	case oTUBOA34:
		if ((g_vm->_obj[oVALVOLA34]._mode & OBJMODE_OBJSTATUS) && g_vm->_obj[oVALVOLA34]._anim)
			CharacterSay(2002);
		else
			CharacterSay(g_vm->_obj[curObj]._examine);
		retVal = false;
		break;

	case oTUBOF34:
		if ((g_vm->_obj[oVALVOLA34]._mode & OBJMODE_OBJSTATUS) && g_vm->_obj[oVALVOLA34]._anim)
			CharacterSay(2000);
		else
			CharacterSay(g_vm->_obj[curObj]._examine);
		retVal = false;
		break;

	case oTUBOFT34:
		if ((g_vm->_obj[oVALVOLA34]._mode & OBJMODE_OBJSTATUS) && g_vm->_obj[oVALVOLA34]._anim)
			CharacterSay(2001);
		else
			CharacterSay(g_vm->_obj[curObj]._examine);
		retVal = false;
		break;

	case oCASSE35:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a3522ESAMINACASSE, 0, 0, curObj);
		retVal = false;
		break;

	case oSCAFFALE35:
		if (g_vm->_room[r35]._flag & OBJFLAG_EXTRA)
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a3517ESAMINACIANFRUSAGLIE, 0, 0, curObj);
		else
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a3517AESAMINACIANFRUSAGLIE, 0, 0, curObj);
		retVal = false;
		break;

	case oGIORNALE35:
		if (g_vm->_room[r35]._flag & OBJFLAG_EXTRA) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a3521LEGGEGIORNALE, 0, 0, curObj);
			retVal = false;
		}
		break;

	case oSCAFFALE36:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, g_vm->_obj[oSCAFFALE36]._anim, 0, 0, curObj);
		retVal = false;
		break;

	case oFESSURA41:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a411, 0, 0, curObj);
		retVal = false;
		break;

	case oCARTELLOV42:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a424, 0, 0, curObj);
		retVal = false;
		break;

	case oCARTELLOF42:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a426, 0, 0, curObj);
		retVal = false;
		break;

	case oCAMPANAT43:
		if (g_vm->_obj[oMARTELLOR43]._mode & OBJMODE_OBJSTATUS)
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a431R, 0, 0, curObj);
		else
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a431, 0, 0, curObj);
		retVal = false;
		break;

	case oTAMBURO43:
		if (g_vm->_obj[oMARTELLOR43]._mode & OBJMODE_OBJSTATUS)
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a432R, 0, 0, curObj);
		else
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a432, 0, 0, curObj);
		retVal = false;
		break;

	case oRAGNATELA45:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a451, 0, 0, curObj);
		retVal = false;
		break;

	case oQUADROS4A:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a4A5, 0, 0, curObj);
		retVal = false;
		break;

	case oCARTELLO55:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a5511, 0, 0, curObj);
		retVal = false;
		break;

	case oEXIT12CU:
		doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, g_vm->_obj[oEXIT12CU]._goRoom, 0, 0, curObj);
		break;

	case oEXIT13CU:
		doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, g_vm->_obj[oEXIT13CU]._goRoom, 0, 0, curObj);
		break;

	case oEXIT2BL:
		doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, g_vm->_obj[oEXIT2BL]._goRoom, 0, 0, curObj);
		break;

	case oEXIT36F:
		doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, g_vm->_obj[oEXIT36F]._goRoom, 0, 0, curObj);
		break;

	case oEXIT41D:
		doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, g_vm->_obj[oEXIT41D]._goRoom, 0, 0, curObj);
		break;

	case oEXIT4CT:
		doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, g_vm->_obj[oEXIT4CT]._goRoom, 0, 0, curObj);
		break;

	case oEXIT58T:
		Count58 = 0;
		for (int a = 0; a < 6; a++)
			g_vm->_obj[oLED158 + a]._mode &= ~OBJMODE_OBJSTATUS;
		doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, g_vm->_obj[oEXIT58T]._goRoom, 0, 0, curObj);
		break;

	case oEXIT58M:
		doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, g_vm->_obj[oEXIT58M]._goRoom, 0, 0, curObj);
		break;

	case oEXIT59L:
		doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, g_vm->_obj[oEXIT59L]._goRoom, 0, 0, curObj);
		break;

	default:
		retVal = true;
		break;
	}

	return retVal;
}

bool LogicManager::mouseOperate(uint16 curObj) {
	bool retVal = false;

	if (!curObj)
		warning("doMouseOperate");

	switch (curObj) {
	case oFAX17:
		if (g_vm->_obj[oSCALA16]._anim) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, g_vm->_obj[curObj]._anim, 0, 0, curObj);
			g_vm->_inventoryObj[iSAM]._action = 1416;
			retVal = false;
		}
		break;

	case ocPOLIZIOTTO16:
	case oSCALA16:
		if (!(g_vm->_obj[oSCALA16]._flag & OBJFLAG_EXTRA)) {
			g_vm->_obj[oSCALA16]._flag |= OBJFLAG_EXTRA;
			g_vm->_choice[61]._flag &= ~DLGCHOICE_HIDE;
			PlayDialog(dPOLIZIOTTO16);
			g_vm->_obj[oSCALA16]._action = 166;
			g_vm->_obj[ocPOLIZIOTTO16]._action = 166;
		} else
			retVal = true;
		break;

	case oPANNELLOC12:
		if (g_vm->_obj[oPANNELLOC12]._flag & OBJFLAG_EXTRA) {
			if (g_vm->_obj[oASCENSOREC12]._mode & OBJMODE_OBJSTATUS)
				doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a128RIUSABOTTONE, 0, 0, curObj);
			else
				CharacterSay(24);
		} else {
			g_vm->_obj[oPANNELLOC12]._flag |= OBJFLAG_EXTRA;
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, g_vm->_obj[curObj]._anim, 0, 0, curObj);
		}
		break;
	case oPANNELLO13:
		if (g_vm->_obj[oASCENSOREA13]._mode & OBJMODE_OBJSTATUS) {
			CharacterSay(48);
			retVal = false;
		} else
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, g_vm->_obj[curObj]._anim, 0, 0, curObj);

		break;

	case oPANNELLO16:
		if (g_vm->_obj[oASCENSOREA16]._mode & OBJMODE_OBJSTATUS) {
			CharacterSay(48);
			retVal = false;
		} else
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, g_vm->_obj[curObj]._anim, 0, 0, curObj);
		break;

	case oLATTINA13:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a132PRENDELATTINA, 0, 0, curObj);
		g_vm->addIcon(iLATTINA13);
		break;

	case oPORTAA13:
		if (g_vm->_room[r14]._flag & OBJFLAG_DONE) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a1312METTELETTERARICALCA, r14, 14, g_vm->_useWith[WITH]);
			retVal = false;
		} else
			retVal = true;
		break;

	case oCESTINO14:
		if (g_vm->_obj[curObj]._anim) {
			if (!(g_vm->_obj[oPORTAR14]._mode & OBJMODE_OBJSTATUS))
				doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, g_vm->_obj[curObj]._anim, 0, 0, curObj);
			else
				doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a143CPRENDECREDITCARD, 0, 0, curObj);
			g_vm->_obj[curObj]._anim = NULL;
		} else
			retVal = true;
		break;
	case oTASTOB15:
		if (g_vm->_obj[oTAPPARELLAA15]._mode & OBJMODE_OBJSTATUS) {
			CharacterSay(g_vm->_obj[oTASTOB15]._action);
			retVal = false;
		} else {
			if (!(g_vm->_obj[oNASTRO15]._flag & OBJFLAG_EXTRA))
				g_vm->_obj[oNASTRO15]._mode |= OBJMODE_OBJSTATUS;
			if (g_vm->_obj[curObj]._anim)
				doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, g_vm->_obj[curObj]._anim, 0, 0, curObj);
			retVal = false;
		}
		break;

	case oTASTOA15:
		if (!(g_vm->_obj[oTAPPARELLAA15]._mode & OBJMODE_OBJSTATUS)) {
			CharacterSay(g_vm->_obj[oTASTOA15]._action);
			retVal = false;
		} else {
			g_vm->_obj[oNASTRO15]._mode &= ~OBJMODE_OBJSTATUS;
			if (g_vm->_obj[curObj]._anim)
				doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, g_vm->_obj[curObj]._anim, 0, 0, curObj);
			retVal = false;
		}
		break;

	case oFINGERPADP16:
		retVal = true;
		break;

	case oPORTAC18:
		if (g_vm->_obj[ocGUARD18]._flag & OBJFLAG_PERSON) {
			g_vm->_choice[151]._flag |= DLGCHOICE_HIDE;
			g_vm->_choice[152]._flag &= ~DLGCHOICE_HIDE;
			PlayDialog(dGUARDIANO18);
			g_vm->_obj[ocGUARD18]._flag &= ~OBJFLAG_PERSON;
			g_vm->_obj[ocGUARD18]._action = 227;
			g_vm->_obj[oPORTAC18]._action = 220;
		} else
			retVal = true;
		break;

	case oGRATA1C:
		if (g_vm->_obj[oFAX17]._flag & OBJFLAG_EXTRA)
			doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, r21, 0, 10, curObj);
		else
			retVal = true;
		break;

	case oBOTOLAC1B:
		if ((g_vm->_obj[oBOTOLAC1B]._anim == a1B3APREBOTOLA) && (g_vm->_obj[oTOMBINOA1B]._mode & OBJMODE_OBJSTATUS))
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a1B3AAPREBOTOLA, 0, 0, curObj);
		else if (g_vm->_obj[oBOTOLAC1B]._anim == a1B3APREBOTOLA)
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a1B3APREBOTOLA, 0, 0, curObj);
		else
			retVal = true;
		break;

	case oARMADIETTORC22:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, g_vm->_obj[curObj]._anim, 0, 0, curObj);
		g_vm->_obj[oMANIGLIONE22]._anim = a227A;
		g_vm->_obj[oMANIGLIONEO22]._anim = a229A;
		g_vm->_obj[od22ALLA29]._anim = a2214A;
		g_vm->_obj[od22ALLA29I]._anim = a2215A;
		break;

	case oARMADIETTORA22:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, g_vm->_obj[curObj]._anim, 0, 0, curObj);
		g_vm->_obj[oMANIGLIONE22]._anim = a227;
		g_vm->_obj[oMANIGLIONEO22]._anim = a229;
		g_vm->_obj[od22ALLA29]._anim = a2214;
		g_vm->_obj[od22ALLA29I]._anim = a2215;
		break;

	case oCATENAT21:
		if ((g_vm->iconPos(iSBARRA21) != MAXICON) && ((g_vm->_choice[436]._flag & OBJFLAG_DONE) || (g_vm->_choice[466]._flag & OBJFLAG_DONE))) {
			if (g_vm->_room[g_vm->_curRoom]._flag & OBJFLAG_EXTRA) // va a destra
				PlayDialog(dF212);                                 // 436
			else                                                   // va a sinistra
				PlayDialog(dF213);                                 // 466
			retVal = false;
		} else if ((g_vm->_choice[451]._flag & OBJFLAG_DONE) || (g_vm->_choice[481]._flag & OBJFLAG_DONE)) {
			if (g_vm->_room[g_vm->_curRoom]._flag & OBJFLAG_EXTRA) // va a destra
				PlayDialog(dF212B);                                // 451
			else                                                   // va a sinistra
				PlayDialog(dF213B);                                // 481
			retVal = false;
		} else
			retVal = true;
		break;

	case oPULSANTEACS2D:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, g_vm->_obj[curObj]._anim, 0, 0, curObj);
		g_vm->_obj[oPULSANTEBC2D]._anim = a2D7SCHIACCIATASTO6V;
		break;

	case oPULSANTEACA2D:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, g_vm->_obj[curObj]._anim, 0, 0, curObj);
		g_vm->_obj[oPULSANTEBC2D]._anim = a2D7SCHIACCIATASTO6R;
		break;

	case oINTERRUTTORE29:
		if (g_vm->_curRoom == r29L)
			retVal = true;
		else if ((g_vm->_obj[oLAMPADINAS29]._mode & OBJMODE_OBJSTATUS)) {
			doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, r29L, 0, 0, curObj);
			retVal = false;
		} else if (!(g_vm->_obj[g_vm->_curObj]._flag & OBJFLAG_EXTRA)) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a291USAINTERRUTTORELUCE, 0, 0, curObj);
			g_vm->_obj[g_vm->_curObj]._flag |= OBJFLAG_EXTRA;
			retVal = false;
		} else
			retVal = true;
		break;

	case oLEVAS23:
		if (g_vm->_obj[oCAVI23]._mode & OBJMODE_OBJSTATUS)
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a238, 0, 0, curObj);
		else
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a234, 0, 0, curObj);
		break;

	case oLEVAG23:
		if (g_vm->_obj[oCAVIE23]._mode & OBJMODE_OBJSTATUS)
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a239, 0, 0, curObj);
		else
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a235, 0, 0, curObj);
		break;

	case oBOTOLAC25:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, g_vm->_obj[curObj]._anim, 0, 0, curObj);
		g_vm->_obj[oTRONCHESE25]._anim = a254B;
		break;

	case oBOTOLAA25:
		doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, g_vm->_obj[curObj]._anim, 0, 0, curObj);
		g_vm->_obj[oTRONCHESE25]._anim = a254;
		break;

	case oPASSAGE24:
		if (g_vm->_room[r24]._flag & OBJFLAG_EXTRA) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a244, 0, 14, curObj);
			g_vm->setRoom(r24, false);
			retVal = false;
		} else {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a243, 0, 13, curObj);
			g_vm->setRoom(r24, true);
			retVal = false;
		}
		break;

	case oPORTA26:
		if (g_vm->_obj[curObj]._anim)
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, g_vm->_obj[curObj]._anim, 0, 4, curObj);
		break;

	case oRUBINETTOC28:
		if (g_vm->_obj[curObj]._anim) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, g_vm->_obj[curObj]._anim, 0, 0, curObj);
			g_vm->_obj[oBRACIERES28]._examine = 455;
			g_vm->_obj[oBRACIERES28]._flag |= OBJFLAG_EXTRA;
		} else
			retVal = true;
		break;

	case oEXIT12CU:
		doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, g_vm->_obj[oEXIT12CU]._goRoom, 0, 0, curObj);
		break;

	case oEXIT13CU:
		doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, g_vm->_obj[oEXIT13CU]._goRoom, 0, 0, curObj);
		break;

	case oEXIT2BL:
		doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, g_vm->_obj[oEXIT2BL]._goRoom, 0, 0, curObj);
		break;

	case oEXIT36F:
		doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, g_vm->_obj[oEXIT36F]._goRoom, 0, 0, curObj);
		break;

	case oEXIT41D:
		doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, g_vm->_obj[oEXIT41D]._goRoom, 0, 0, curObj);
		break;

	case oEXIT4CT:
		doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, g_vm->_obj[oEXIT4CT]._goRoom, 0, 0, curObj);
		break;

	case oEXIT58T:
		Count58 = 0;
		for (uint8 a = 0; a < 6; a++)
			g_vm->_obj[oLED158 + a]._mode &= ~OBJMODE_OBJSTATUS;
		doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, g_vm->_obj[oEXIT58T]._goRoom, 0, 0, curObj);
		break;

	case oEXIT58M:
		doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, g_vm->_obj[oEXIT58M]._goRoom, 0, 0, curObj);
		break;

	case oEXIT59L:
		doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, g_vm->_obj[oEXIT59L]._goRoom, 0, 0, curObj);
		break;

	case oPANNELLOM2G:
		if (!(g_vm->_obj[oPANNELLOM2G]._flag & OBJFLAG_EXTRA)) {
			PlayDialog(dF2G1);
			g_vm->_obj[oCOPERCHIO2G]._mode &= ~OBJMODE_OBJSTATUS;
			//_obj[oPANNELLOM2G]._mode &= ~OBJMODE_OBJSTATUS;
			g_vm->_obj[oSERBATOIOC2G]._mode &= ~OBJMODE_OBJSTATUS;
			g_vm->_obj[oRAGAZZOP2G]._mode &= ~OBJMODE_OBJSTATUS;
			g_vm->_obj[oRAGAZZOS2G]._mode |= OBJMODE_OBJSTATUS;
			g_vm->_obj[oSERBATOIOA2G]._mode |= OBJMODE_OBJSTATUS;
			g_vm->_obj[oPANNELLOE2G]._mode |= OBJMODE_OBJSTATUS;
			g_vm->_obj[oPANNELLOM2G]._flag |= OBJFLAG_EXTRA;
			g_vm->_animMgr->_animTab[aBKG2G]._flag |= SMKANIM_OFF1;
			retVal = false;
		} else
			retVal = true;

		break;

	case oRUOTE2C:
		if (!(g_vm->_obj[od2CALLA2D]._mode & OBJMODE_OBJSTATUS)) {
			g_vm->_animMgr->stopSmkAnim(g_vm->_room[g_vm->_curRoom]._bkgAnim);
			g_vm->_animMgr->_animTab[aBKG2C]._flag |= SMKANIM_OFF1;
			g_vm->_obj[oBASERUOTE2C]._mode |= OBJMODE_OBJSTATUS;
			g_vm->_obj[omRUOTE2C]._mode |= OBJMODE_OBJSTATUS;
			g_vm->_obj[oPULSANTE2C]._mode |= OBJMODE_OBJSTATUS;
			g_vm->_obj[_vm->ruotepos[0] * 3 + 0 + oRUOTA1A2C]._mode |= OBJMODE_OBJSTATUS;
			g_vm->_obj[_vm->ruotepos[1] * 3 + 1 + oRUOTA1A2C]._mode |= OBJMODE_OBJSTATUS;
			g_vm->_obj[_vm->ruotepos[2] * 3 + 2 + oRUOTA1A2C]._mode |= OBJMODE_OBJSTATUS;
			g_vm->_obj[oCAMPO2C]._mode &= ~OBJMODE_OBJSTATUS;
			g_vm->_obj[oTEMPIO2C]._mode &= ~OBJMODE_OBJSTATUS;
			g_vm->_obj[oLEONE2C]._mode &= ~OBJMODE_OBJSTATUS;
			g_vm->_obj[od2CALLA2D]._mode &= ~OBJMODE_OBJSTATUS;
			g_vm->_obj[oSFINGE2C]._mode &= ~OBJMODE_OBJSTATUS;
			g_vm->_obj[oSTATUA2C]._mode &= ~OBJMODE_OBJSTATUS;
			g_vm->_obj[oRUOTE2C]._mode &= ~OBJMODE_OBJSTATUS;
			g_vm->_obj[od2CALLA2E]._mode &= ~OBJMODE_OBJSTATUS;
			g_vm->_obj[oCARTELLOS2C]._mode &= ~OBJMODE_OBJSTATUS;
			g_vm->_obj[oCARTELLOA2C]._mode &= ~OBJMODE_OBJSTATUS;
			g_vm->_obj[od2CALLA26]._mode &= ~OBJMODE_OBJSTATUS;
			FlagShowCharacter = false;
			ReadExtraObj2C();
			RegenRoom();
		} else
			retVal = true;
		break;

	case oCATWALKA2E:
		if (!(g_vm->_obj[oCATWALKA2E]._flag & OBJFLAG_EXTRA)) {
			PlayDialog(dF2E1);
			g_vm->_obj[oDINOSAURO2E]._mode |= OBJMODE_OBJSTATUS;
			g_vm->_obj[oCATWALKA2E]._flag |= OBJFLAG_EXTRA;
			g_vm->_animMgr->_animTab[aBKG2E]._flag &= ~SMKANIM_OFF2;
			retVal = false;
		} else if (g_vm->_obj[curObj]._anim) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, g_vm->_obj[curObj]._anim, 0, 0, curObj);
			retVal = false;
		} else
			retVal = true;
		break;

	case oBORSA22:
	case oPORTALAMPADE2B:
	case oMAPPAMONDO2B:
		if (g_vm->_obj[curObj]._anim) {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, g_vm->_obj[curObj]._anim, 0, 0, curObj);
			g_vm->_obj[curObj]._anim = 0;
		} else
			retVal = true;
		break;

	case oTUBOF34:
		if (!(g_vm->_obj[oTUBOFT34]._mode & OBJMODE_OBJSTATUS))
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, g_vm->_obj[curObj]._anim, 0, 0, curObj);
		else
			retVal = true;
		break;

	case oFILOT31:
		g_vm->_obj[oFILOT31]._mode &= ~OBJMODE_OBJSTATUS;
		g_vm->_obj[oFILOS31]._mode |= OBJMODE_OBJSTATUS;
		RegenRoom();
		break;

	case oCOPERCHIOA31:
		if (g_vm->_obj[oFILOTC31]._mode & OBJMODE_OBJSTATUS) {
			NLPlaySound(wCOVER31);
			g_vm->_obj[oPANNELLOM31]._mode &= ~OBJMODE_OBJSTATUS;
			g_vm->_obj[oPANNELLOMA31]._mode |= OBJMODE_OBJSTATUS;
			g_vm->_obj[oPANNELLOM31]._anim = 0;
			g_vm->_obj[oPANNELLOM31]._examine = 715;
			g_vm->_obj[oPANNELLOM31]._action = 716;
			g_vm->_obj[oPANNELLOM31]._flag &= ~OBJFLAG_ROOMOUT;
			doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, r31, a3118CHIUDEPANNELLO, 3, curObj);
		} else
			retVal = true;
		break;

	case oVALVOLAC34:
		if (g_vm->_obj[curObj]._anim)
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, g_vm->_obj[curObj]._anim, 0, 0, curObj);
		else
			retVal = true;
		break;

	case oVALVOLA34:
		if (g_vm->_obj[curObj]._anim)
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, g_vm->_obj[curObj]._anim, 0, 0, curObj);
		else
			retVal = true;
		break;

	case oPROIETTORE35:
		g_vm->_room[r35]._flag |= OBJFLAG_EXTRA;
		read3D("352.3d"); // dopo scossa

		g_vm->_obj[oRIBELLEA35]._mode &= ~OBJMODE_OBJSTATUS;
		g_vm->_obj[oPORTAC35]._mode &= ~OBJMODE_OBJSTATUS;
		g_vm->_obj[omPORTAC35]._mode &= ~OBJMODE_OBJSTATUS;
		g_vm->_obj[oPRESA35]._mode &= ~OBJMODE_OBJSTATUS;

		g_vm->_obj[oPORTAA35]._mode |= OBJMODE_OBJSTATUS;
		g_vm->_obj[omPORTAA35]._mode |= OBJMODE_OBJSTATUS;
		g_vm->_obj[oRIBELLES35]._mode |= OBJMODE_OBJSTATUS;
		g_vm->_obj[oSEDIA35]._mode |= OBJMODE_OBJSTATUS;
		g_vm->_obj[oMONITOR35]._mode |= OBJMODE_OBJSTATUS;
		g_vm->_obj[omPIANO35]._mode |= OBJMODE_OBJSTATUS;

		g_vm->_obj[oFRONTOFFICEC35]._anim = a356PROVASPORTELLO;
		g_vm->_obj[oASCENSORE35]._flag |= OBJFLAG_ROOMOUT;
		g_vm->_obj[oASCENSORE35]._anim = a3514ENTRAASCENSORE;

		g_vm->_animMgr->_animTab[aBKG35]._flag |= SMKANIM_OFF1;
		PlayDialog(dF351);
		setPosition(7);
		break;

	case oCOMPUTER36:
		if (!(g_vm->_choice[646]._flag & OBJFLAG_DONE)) {
			PlayDialog(dF361);
			g_vm->_obj[oCOMPUTER36]._action = 2004;
			retVal = false;
		} else
			retVal = true;
		break;

	case oPULSANTEA35:
	case oPULSANTEB35:
	case oPULSANTEC35:
	case oPULSANTED35:
	case oPULSANTEE35:
	case oPULSANTEF35:
	case oPULSANTEG35:
		g_vm->_obj[curObj]._mode &= ~OBJMODE_OBJSTATUS;
		g_vm->_obj[curObj + 7]._mode |= OBJMODE_OBJSTATUS;
		Comb35[Count35++] = curObj;
		NLPlaySound(wPAD5);
		if (Count35 == 7) {
			if (((Comb35[0] == oPULSANTEF35) && (Comb35[1] == oPULSANTED35) && (Comb35[2] == oPULSANTEC35) &&
				 (Comb35[3] == oPULSANTEG35) && (Comb35[4] == oPULSANTEB35) && (Comb35[5] == oPULSANTEA35) &&
				 (Comb35[6] == oPULSANTEE35)) ||
				((Comb35[0] == oPULSANTEE35) &&
				 (Comb35[1] == oPULSANTEA35) && (Comb35[2] == oPULSANTEB35) && (Comb35[3] == oPULSANTEG35) &&
				 (Comb35[4] == oPULSANTEC35) && (Comb35[5] == oPULSANTED35) && (Comb35[6] == oPULSANTEF35))) {
				g_vm->_obj[oPULSANTEAA35]._mode &= ~OBJMODE_OBJSTATUS;
				g_vm->_obj[oPULSANTEBA35]._mode &= ~OBJMODE_OBJSTATUS;
				g_vm->_obj[oPULSANTECA35]._mode &= ~OBJMODE_OBJSTATUS;
				g_vm->_obj[oPULSANTEDA35]._mode &= ~OBJMODE_OBJSTATUS;
				g_vm->_obj[oPULSANTEEA35]._mode &= ~OBJMODE_OBJSTATUS;
				g_vm->_obj[oPULSANTEFA35]._mode &= ~OBJMODE_OBJSTATUS;
				g_vm->_obj[oPULSANTEGA35]._mode &= ~OBJMODE_OBJSTATUS;
				g_vm->_obj[oPULSANTIV35]._mode |= OBJMODE_OBJSTATUS;
				g_vm->_obj[oLEDS35]._mode &= ~OBJMODE_OBJSTATUS;

				g_vm->_obj[oFRONTOFFICEA35]._anim = 0;
				//					_obj[oFRONTOFFICEA35]._examine = 1843;
				g_vm->_obj[oFRONTOFFICEA35]._action = 1844;
				g_vm->_obj[oFRONTOFFICEA35]._flag |= OBJFLAG_EXTRA;
				g_vm->_obj[oPORTAMC36]._flag |= OBJFLAG_ROOMOUT;
				g_vm->_obj[oPORTAMC36]._anim = a3610APREPORTA;
				g_vm->_obj[oSCAFFALE36]._anim = a3615APRENDESCAFFALE;

				g_vm->_animMgr->_animTab[aBKG36]._flag |= SMKANIM_OFF2;
				g_vm->_animMgr->_animTab[aBKG36]._flag |= SMKANIM_OFF3;
				g_vm->_obj[oSCANNERLA36]._mode &= ~OBJMODE_OBJSTATUS;
				g_vm->_obj[oSCANNERLS36]._mode |= OBJMODE_OBJSTATUS;
				g_vm->_obj[oSCANNERMA36]._mode &= ~OBJMODE_OBJSTATUS;
				g_vm->_obj[oSCANNERMS36]._mode |= OBJMODE_OBJSTATUS;

				NLPlaySound(wWIN35);
			} else {
				g_vm->_obj[oPULSANTEA35]._mode |= OBJMODE_OBJSTATUS;
				g_vm->_obj[oPULSANTEB35]._mode |= OBJMODE_OBJSTATUS;
				g_vm->_obj[oPULSANTEC35]._mode |= OBJMODE_OBJSTATUS;
				g_vm->_obj[oPULSANTED35]._mode |= OBJMODE_OBJSTATUS;
				g_vm->_obj[oPULSANTEE35]._mode |= OBJMODE_OBJSTATUS;
				g_vm->_obj[oPULSANTEF35]._mode |= OBJMODE_OBJSTATUS;
				g_vm->_obj[oPULSANTEG35]._mode |= OBJMODE_OBJSTATUS;
				g_vm->_obj[oPULSANTEAA35]._mode &= ~OBJMODE_OBJSTATUS;
				g_vm->_obj[oPULSANTEBA35]._mode &= ~OBJMODE_OBJSTATUS;
				g_vm->_obj[oPULSANTECA35]._mode &= ~OBJMODE_OBJSTATUS;
				g_vm->_obj[oPULSANTEDA35]._mode &= ~OBJMODE_OBJSTATUS;
				g_vm->_obj[oPULSANTEEA35]._mode &= ~OBJMODE_OBJSTATUS;
				g_vm->_obj[oPULSANTEFA35]._mode &= ~OBJMODE_OBJSTATUS;
				g_vm->_obj[oPULSANTEGA35]._mode &= ~OBJMODE_OBJSTATUS;
			}
			Count35 = 0;
			Comb35[0] = 0;
			Comb35[1] = 0;
			Comb35[2] = 0;
			Comb35[3] = 0;
			Comb35[4] = 0;
			Comb35[5] = 0;
			Comb35[6] = 0;
		}
		g_vm->_curObj += 7;
		RegenRoom();
		break;

	case oCARD35:
		g_vm->_obj[oPULSANTEA35]._mode |= OBJMODE_OBJSTATUS;
		g_vm->_obj[oPULSANTEB35]._mode |= OBJMODE_OBJSTATUS;
		g_vm->_obj[oPULSANTEC35]._mode |= OBJMODE_OBJSTATUS;
		g_vm->_obj[oPULSANTED35]._mode |= OBJMODE_OBJSTATUS;
		g_vm->_obj[oPULSANTEE35]._mode |= OBJMODE_OBJSTATUS;
		g_vm->_obj[oPULSANTEF35]._mode |= OBJMODE_OBJSTATUS;
		g_vm->_obj[oPULSANTEG35]._mode |= OBJMODE_OBJSTATUS;
		g_vm->_obj[oPULSANTEAA35]._mode &= ~OBJMODE_OBJSTATUS;
		g_vm->_obj[oPULSANTEBA35]._mode &= ~OBJMODE_OBJSTATUS;
		g_vm->_obj[oPULSANTECA35]._mode &= ~OBJMODE_OBJSTATUS;
		g_vm->_obj[oPULSANTEDA35]._mode &= ~OBJMODE_OBJSTATUS;
		g_vm->_obj[oPULSANTEEA35]._mode &= ~OBJMODE_OBJSTATUS;
		g_vm->_obj[oPULSANTEFA35]._mode &= ~OBJMODE_OBJSTATUS;
		g_vm->_obj[oPULSANTEGA35]._mode &= ~OBJMODE_OBJSTATUS;
		g_vm->addIcon(iCARD36);

		doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, r35, a359RITIRACARD, 6, curObj);
		break;

	case oSCAFFALE36:
		retVal = true;
		break;

	case oFOROC49:
	case oFORO849:
	case oFORO949:
	case oFORO1049:
	case oFORO1149:
	case oFORO1249:
	case oFORO149:
	case oFORO249:
	case oFORO349:
	case oFORO449:
	case oFORO549:
	case oFORO649:
	case oFORO749:
		for (int a = oASTAC49; a <= oASTA749; a++)
			g_vm->_obj[a]._mode &= ~OBJMODE_OBJSTATUS;
		g_vm->_obj[oASTAC49 + curObj - oFOROC49]._mode |= OBJMODE_OBJSTATUS;
		Comb49[3] = Comb49[2];
		Comb49[2] = Comb49[1];
		Comb49[1] = Comb49[0];
		Comb49[0] = curObj;
		NLPlaySound(wASTA49);
		RegenRoom();
		if ((Comb49[3] == oFORO749) && (Comb49[2] == oFORO849) && (Comb49[1] == oFORO449) && (Comb49[0] == oFORO549)) {
			PaintScreen(0);
			NlDelay(60);
			g_vm->_obj[oOMBRAS49]._mode |= OBJMODE_OBJSTATUS;
			g_vm->_obj[oSCOMPARTO49]._mode |= OBJMODE_OBJSTATUS;
			g_vm->_obj[oAGENDA49]._mode |= OBJMODE_OBJSTATUS;
			g_vm->_obj[oMERIDIANA49]._mode &= ~OBJMODE_OBJSTATUS;
			g_vm->_obj[oAGENDA49]._examine = 1099;
			g_vm->_obj[oAGENDA49]._action = 1100;
			FlagCharacterExist = true;
			g_vm->_curObj = oAGENDA49;
			//FlagShowCharacter=true;
			//doEvent(MC_SYSTEM,ME_CHANGEROOM,MP_SYSTEM,r4A,0,1,TheObj);
			g_vm->PlayScript(s49SUNDIAL);
		}
		retVal = false;
		break;

	case oASTAC49:
	case oASTA849:
	case oASTA949:
	case oASTA1049:
	case oASTA1149:
	case oASTA1249:
	case oASTA149:
	case oASTA249:
	case oASTA349:
	case oASTA449:
	case oASTA549:
	case oASTA649:
	case oASTA749:
		for (int a = oASTAC49; a <= oASTA749; a++)
			g_vm->_obj[a]._mode &= ~OBJMODE_OBJSTATUS;
		g_vm->_obj[oASTAC49]._mode |= OBJMODE_OBJSTATUS;
		doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, r49, a496, 1, curObj);
		retVal = false;
		break;

	case oNUMERO14C:
	case oNUMERO24C:
	case oNUMERO34C:
	case oNUMERO44C:
	case oNUMERO54C:
	case oNUMERO64C:
	case oNUMERO74C:
	case oNUMERO84C:
	case oNUMERO94C:
	case oNUMERO04C: {
		int a;
		for (a = 0; a < 6; a++) {
			if (Comb4CT[a] == 0) {
				g_vm->_obj[a + oAST14C]._mode |= OBJMODE_OBJSTATUS;
				Comb4CT[a] = curObj - oNUMERO14C + 1;
				break;
			}
		}
		NLPlaySound(wPAD1 + curObj - oNUMERO14C);
		RegenRoom();
		if (a < 5)
			break;
		PaintScreen(0);
		NlDelay(60);
		if ((Comb4CT[0] == 5) && (Comb4CT[1] == 6) && (Comb4CT[2] == 2) &&
			(Comb4CT[3] == 3) && (Comb4CT[4] == 9) && (Comb4CT[5] == 6)) {
			for (a = 0; a < 6; a++) {
				Comb4CT[a] = 0;
				g_vm->_obj[oAST14C + a]._mode &= ~OBJMODE_OBJSTATUS;
			}
			doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, r51, 0, 1, curObj);
			FlagCharacterExist = true;
			//FlagShowCharacter=true;
		} else {
			for (a = 0; a < 6; a++) {
				Comb4CT[a] = 0;
				g_vm->_obj[oAST14C + a]._mode &= ~OBJMODE_OBJSTATUS;
			}
			doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, r4C, 0, 4, curObj);
			FlagCharacterExist = true;
			//FlagShowCharacter=true;
		}
		retVal = false;
		break;
		}
	case oPORTAC4A:
		if (!(g_vm->_choice[245]._flag & OBJFLAG_DONE) && !(g_vm->_choice[766]._flag & OBJFLAG_DONE)) {
			g_vm->_choice[245]._flag &= ~DLGCHOICE_HIDE;
			PlayDialog(dC4A1);
			setPosition(14);
			g_vm->_obj[oPORTAC4A]._action = 1117;
			g_vm->_obj[oPORTAC4A]._anim = 0;
			retVal = false;
		} else
			retVal = true;
		break;

	case oPULSANTE4A:
		if (g_vm->_obj[curObj]._anim)
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, g_vm->_obj[curObj]._anim, 0, 0, curObj);
		else if (!(g_vm->_choice[244]._flag & OBJFLAG_DONE)) {
			g_vm->_choice[244]._flag &= ~DLGCHOICE_HIDE;
			PlayDialog(dC4A1);
			g_vm->_obj[oPULSANTE4A]._examine = 1108;
			g_vm->_obj[oPULSANTE4A]._action = 1109;
			retVal = false;
		} else
			retVal = true;
		break;

	case oFINESTRA56:
		if (g_vm->_obj[oPANNELLOC56]._mode & OBJMODE_OBJSTATUS)
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a5614, 0, 0, curObj);
		else
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a5614P, 0, 0, curObj);
		break;

	case oPULSANTECD:
		for (int a = oPULSANTE1AD; a <= oPULSANTE33AD; a++) {
			if ((g_vm->_obj[a]._goRoom == g_vm->_obj[oEXIT41D]._goRoom) ||
				((g_vm->_obj[a]._goRoom == r45) && (g_vm->_obj[oEXIT41D]._goRoom == r45S))) {
				CharacterSay(903);
				break;
			} else if (g_vm->_obj[a]._goRoom == 0) {
				if (g_vm->_obj[oEXIT41D]._goRoom == r45S)
					g_vm->_obj[a]._goRoom = r45;
				else
					g_vm->_obj[a]._goRoom = g_vm->_obj[oEXIT41D]._goRoom;
				g_vm->_obj[a]._mode |= OBJMODE_OBJSTATUS;
				g_vm->_obj[a - 40]._mode &= ~OBJMODE_OBJSTATUS;
				RegenRoom();
				break;
			}
		}
		retVal = false;
		break;

	case oPORTAC54:
		if (!(g_vm->_choice[826]._flag & OBJFLAG_DONE)) {
			PlayDialog(dF541);
			retVal = false;
		} else
			retVal = true;
		break;

	case oLAVATRICEL54:
		if ((g_vm->_obj[curObj]._anim) && (g_vm->_obj[oSECCHIOS54]._flag & OBJFLAG_EXTRA) && (g_vm->_obj[oGRATAC54]._mode & OBJMODE_OBJSTATUS) && !(g_vm->_choice[841]._flag & OBJFLAG_DONE))
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, g_vm->_obj[curObj]._anim, 0, 0, curObj);
		else
			retVal = true;
		break;

	case oSECCHIOS54:
		if (g_vm->_obj[oGRATAC54]._mode & OBJMODE_OBJSTATUS)
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a544G, 0, 0, curObj);
		else
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, g_vm->_obj[curObj]._anim, 0, 0, curObj);
		retVal = false;
		break;

	case oTASTIERA56:
		if (g_vm->_choice[262]._flag & OBJFLAG_DONE) {
			if (g_vm->_obj[od56ALLA59]._mode & OBJMODE_OBJSTATUS)
				CharacterSay(g_vm->_obj[curObj]._action);
			else
				doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a567, 0, 0, curObj);
			retVal = false;
		} else {
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a564, 0, 0, curObj);
			retVal = false;
		}
		break;

	case oLIBRIEG2B:
		if (g_vm->_room[r2C]._flag & OBJFLAG_DONE) { // se sono gia' stato nella 2C prendo libro
			retVal = false;
			if (g_vm->_obj[curObj]._anim)
				doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, a2B4PRENDELIBRO, 0, 0, curObj);
			else
				retVal = true;
		} else { // se non ci sono ancora stato dice che non gli serve
			CharacterSay(2014);
			retVal = false;
		}
		break;

	case oTASTIERA58:
		doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, r58T, 0, 0, curObj);
		break;

	case oLAVAGNA59:
	case oSIMBOLI59:
		doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, r59L, 0, 0, curObj);
		break;

	case oFINESTRAA5A:
		if ((g_vm->_choice[871]._flag & OBJFLAG_DONE) && !(g_vm->_choice[286]._flag & OBJFLAG_DONE)) {
			PlayDialog(dC5A1);
			retVal = false;
		} else
			retVal = true;
		break;

	case oFINESTRAB5A:
		if (!(g_vm->_choice[256]._flag & OBJFLAG_DONE)) {
			CharacterSay(1999);
			retVal = false;
		} else
			retVal = true;
		break;

	case oTASTO158:
	case oTASTO258:
	case oTASTO358:
	case oTASTO458:
	case oTASTO558:
	case oTASTO658:
	case oTASTO758:
	case oTASTO858:
	case oTASTO958:
	case oTASTO058:
		retVal = false;
		Comb58[5] = Comb58[4];
		Comb58[4] = Comb58[3];
		Comb58[3] = Comb58[2];
		Comb58[2] = Comb58[1];
		Comb58[1] = Comb58[0];
		Comb58[0] = curObj;

		NLPlaySound(wPAD1 + curObj - oTASTO158);
		g_vm->_obj[oLED158 + Count58]._mode |= OBJMODE_OBJSTATUS;
		Count58++;
		RegenRoom();
		if (Count58 < 6)
			break;

		PaintScreen(0);
		NlDelay(60);
		Count58 = 0;
		for (int a = 0; a < 6; a++)
			g_vm->_obj[oLED158 + a]._mode &= ~OBJMODE_OBJSTATUS;

		if ((Comb58[0] == oTASTO058) && (Comb58[1] == oTASTO258) && (Comb58[2] == oTASTO358) &&
			(Comb58[3] == oTASTO858) && (Comb58[4] == oTASTO558) && (Comb58[5] == oTASTO958)) {
			SoundFadOut();
			PlayDialog(dF582);
		} else
			doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, r58, 0, 2, curObj);

		for (int i = 0; i < 6; ++i)
			Comb58[i] = 0;
		break;

	default:
		if (g_vm->_obj[curObj]._anim)
			doEvent(MC_CHARACTER, ME_CHARACTERACTION, MP_DEFAULT, g_vm->_obj[curObj]._anim, 0, 0, curObj);
		else
			retVal = true;
		break;
	}

	return retVal;
}

} // End of namespace Trecision
