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

#include "trecision/trecision.h"
#include "trecision/dialog.h"


#include "nl/message.h"
#include "nl/proto.h"
#include "trecision/graphics.h"

#include "trecision/nl/define.h"

namespace Trecision {
DialogManager::DialogManager(TrecisionEngine *vm) : _vm(vm) {
	_curDialog = 0;
	_curChoice = 0;
	_curSubTitle = 0;
	CurDispChoice = 0;
	CurPos = 0;
	LastPos = 0;
}

DialogManager::~DialogManager() {}

void DialogManager::DialogPrint(int x, int y, int c, const char *txt) {
	SDText curChoice;
	curChoice.set(x, y, _vm->TextLength(txt, 0), 0, 0, 0, MAXX, MAXY, c, MASKCOL, txt);
	curChoice.DText();
}

void DialogManager::ShowChoices(uint16 i) {
	Dialog *d = &_dialog[i];

	int y = 5;
	int x = 10;
	CurPos = -1;
	LastPos = -1;
	memset(_vm->_screenBuffer, 0, MAXX * TOP * 2);

	for (int c = 0; c < MAXDISPCHOICES; c++)
		DispChoice[c] = 0;

	CurDispChoice = 0;
	for (int c = d->_firstChoice; c < (d->_firstChoice + d->_choiceNumb); c++) {
		if (!(_vm->_choice[c]._flag & DLGCHOICE_HIDE)) {
			DispChoice[CurDispChoice++] = c;
			DialogPrint(x, y, HWHITE, _vm->_sentence[_vm->_choice[c]._sentenceIndex]);
			y += CARHEI;
		}
	}

	_vm->_graphicsMgr->copyToScreen(0, 0, MAXX, TOP);

	_vm->_flagDialogMenuActive = true;
	_vm->showCursor();
}

void DialogManager::UpdateChoices(int16 dmx, int16 dmy) {
	if ((dmy >= MAXDISPCHOICES) && (dmy < (CARHEI * (CurDispChoice)+5)))
		CurPos = (dmy - 5) / CARHEI;
	else
		CurPos = -1;

	if ((CurPos != LastPos) && ((CurPos != -1) || (LastPos != -1))) {
		for (int c = 0; c < MAXDISPCHOICES; c++) {
			if (DispChoice[c] != 0) {
				if (c == CurPos)
					DialogPrint(10, 5 + c * CARHEI, HGREEN, _vm->_sentence[_vm->_choice[DispChoice[c]]._sentenceIndex]);
				else
					DialogPrint(10, 5 + c * CARHEI, HWHITE, _vm->_sentence[_vm->_choice[DispChoice[c]]._sentenceIndex]);
			}
		}
		_vm->_graphicsMgr->copyToScreen(0, 5, MAXX, (CurDispChoice)*CARHEI + 5);
	}
	LastPos = CurPos;
}

void DialogManager::SelectChoice(int16 dmx, int16 dmy) {
	UpdateChoices(dmx, dmy);

	if (CurPos != -1) {
		_vm->_flagDialogMenuActive = false;
		PlayChoice(DispChoice[CurPos]);
	}
}

void DialogManager::PlayDialog(uint16 i) {
	_curDialog = i;
	_vm->_flagDialogActive = true;
	_curChoice = 0;
	_curSubTitle = 0;
	_vm->_flagShowCharacter = false;

	_vm->_characterQueue.initQueue();
	_vm->_inventoryStatus = INV_OFF;
	_vm->_inventoryCounter = INVENTORY_HIDE;
	_vm->clearText();
	_vm->drawString();
	PaintScreen(1);

	memset(_vm->_screenBuffer, 0, MAXX * TOP * 2);
	_vm->_graphicsMgr->copyToScreen(0, 0, MAXX, TOP);

	_vm->_animMgr->startFullMotion((const char *)_dialog[i]._startAnim);

	int skip = 0;
	int curChoice = 0;
	for (int c = _dialog[_curDialog]._firstChoice; c < (_dialog[_curDialog]._firstChoice + _dialog[_curDialog]._choiceNumb); ++c) {
		if (!(_vm->_choice[c]._flag & DLGCHOICE_HIDE))
			curChoice++;
	}
	
	if ((_curDialog == dC581) && !(_vm->_choice[262]._flag & DLGCHOICE_HIDE))
		skip++;
	if ((_curDialog == dC581) && (curChoice == 1))
		skip++;
	if ((_curDialog == dSHOPKEEPER1A) && (curChoice == 1))
		skip++;
	// if there's a pre-dialog
	if ((_dialog[i]._startLen > 0) && !skip)
		_vm->_animMgr->playMovie(_dialog[i]._startAnim, 1, _dialog[i]._startLen);
	else {
		_vm->_animMgr->smkSoundOnOff(1, false);
		afterChoice(1);
	}
}

void DialogManager::afterChoice(int numFrame) {
	Dialog *d = &_dialog[_curDialog];

	memset(_vm->_screenBuffer, 0, MAXX * TOP * 2);
	_vm->_graphicsMgr->copyToScreen(0, 0, MAXX, TOP);

	switch (_curDialog) {
	case dTRAMP171:
		if (_curChoice == 80) {
			_vm->_obj[ocTRAMP17]._action = 213;
			_vm->_obj[ocTRAMP17]._flag &= ~kObjFlagPerson;
		} else if (_curChoice == 77) {
			_vm->_obj[ocTRAMP17]._action = 211;
			_vm->_obj[ocTRAMP17]._flag &= ~kObjFlagPerson;
		}
		break;

	case dTRAMP1714:
		if (_curChoice == 106) {
			_vm->_obj[ocTRAMP17]._action = 213;
			_vm->_obj[ocTRAMP17]._flag &= ~kObjFlagPerson;
		}
		break;

	case dTRAMP1713:
		if (_curChoice == 91) {
			_vm->_obj[ocTRAMP17]._action = 212;
			_vm->_obj[ocTRAMP17]._flag &= ~kObjFlagPerson;
		}
		break;

	case dTRAMP1716:
		if (_curChoice == 122) {
			_vm->_obj[ocTRAMP17]._action = 212;
			_vm->_obj[ocTRAMP17]._flag &= ~kObjFlagPerson;
		}
		break;

	case dTRAMP1717:
		if (_curChoice == 136) {
			_vm->_obj[ocTRAMP17]._mode &= ~OBJMODE_OBJSTATUS;
			_vm->_obj[oTRAMPD17]._mode |= OBJMODE_OBJSTATUS;
			_vm->_room[kRoom17]._bkgAnim = aBKG17B;
			_vm->addIcon(iSKATE);
		} else if (_curChoice == 137) {
			_vm->_obj[ocTRAMP17]._flag |= kObjFlagPerson;
		}
		break;

	case dGUARDIAN18:
		if ((_curChoice == 151) || (_curChoice == 152)) {
			_vm->_inventoryObj[iFOTO]._action = 1465;
			_vm->_obj[oTESSERA1A]._action = 238;
			if (_vm->_obj[oTESSERA1A]._flag & kObjFlagExtra) {
				_vm->_choice[154]._flag &= ~DLGCHOICE_HIDE;
				_vm->_choice[153]._flag |= DLGCHOICE_HIDE;
			} else
				_vm->_choice[153]._flag &= ~DLGCHOICE_HIDE;
		} else if (_curChoice == 154) {
			if (_vm->_obj[oTESSERA1A]._flag & kObjFlagExtra)
				_vm->_choice[183]._flag &= ~DLGCHOICE_HIDE;
		} else if (_curChoice == 155)
			_vm->_obj[ocGUARD18]._action = 228;
		break;
	case dF213B:
	case dF213:
		_vm->setRoom(kRoom21, true);
		break;

	case dF212B:
	case dF212:
		_vm->setRoom(kRoom21, false);
		break;

	case dF321:
		_vm->removeIcon(iTORCIA32);
		_vm->_flagShowCharacter = false;
		break;

	case dF4A3:
		_vm->_obj[oCHOCOLATES4A]._examine = 1105;
		_vm->_obj[oCHOCOLATES4A]._action = 1106;
		_vm->_obj[oDOORC4A]._action = 1118;
		_vm->_animMgr->_animTab[aBKG4A]._flag |= SMKANIM_OFF1;
		_vm->_obj[ocHELLEN4A]._mode &= ~OBJMODE_OBJSTATUS;
		_vm->_obj[oHELLENA4A]._mode |= OBJMODE_OBJSTATUS;
		break;

	case dC581:
		_vm->_obj[oWINDOWB58]._mode |= OBJMODE_OBJSTATUS;
		if (_curChoice == 262)
			_vm->_obj[oTASTIERA56]._examine = 1307;
		break;

	case dF542:
		_vm->_obj[oGRATAC54]._mode &= ~OBJMODE_OBJSTATUS;
		_vm->_obj[oDOORC54]._mode &= ~OBJMODE_OBJSTATUS;
		_vm->_obj[oLAVATRICEL54]._mode &= ~OBJMODE_OBJSTATUS;
		_vm->_obj[oLAVATRICEF54]._mode &= ~OBJMODE_OBJSTATUS;
		_vm->_obj[oGRATAA54]._mode |= OBJMODE_OBJSTATUS;
		_vm->_obj[oCHIAVI54]._mode |= OBJMODE_OBJSTATUS;
		_vm->_obj[od54ALLA55]._mode |= OBJMODE_OBJSTATUS;
		break;
	default:
		break;
	}

	// If the player chose to exit the dialog
	if (_vm->_choice[_curChoice]._flag & DLGCHOICE_EXITDLG) {
		_vm->_animMgr->stopFullMotion();

		switch (_curDialog) {
		case dPOLIZIOTTO16:
			if ((_vm->_choice[61]._flag & kObjFlagDone) && (_vm->_choice[62]._flag & kObjFlagDone) && (_vm->_obj[ocPOLIZIOTTO16]._flag & kObjFlagExtra))
				_vm->_obj[ocPOLIZIOTTO16]._mode &= ~OBJMODE_OBJSTATUS;
			break;

		case dTRAMP171:
			if (_curChoice == 77) {
				_vm->_obj[ocTRAMP17]._action = 211;
				_vm->_obj[ocTRAMP17]._flag &= ~kObjFlagPerson;
			} else if (_curChoice == 80)
				_vm->_obj[ocTRAMP17]._action = 213;
			else if (_curChoice == 122)
				_vm->_obj[ocTRAMP17]._action = 211;
			break;

		case dGUARDIAN18:
			if (_curChoice == 152)
				_vm->_obj[ocGUARD18]._mode &= ~OBJMODE_OBJSTATUS;
			else if (_curChoice == 155)
				StartCharacterAction(a184ENTRACLUB, kRoom19, 2, 0);
			break;

		case dEVA19:
			_vm->_obj[oDOORC18]._flag &= ~kObjFlagRoomOut;
			_vm->_obj[oDOORC18]._action = 221;
			_vm->_obj[ocEVA19]._action = 1999;
			_vm->_obj[ocEVA19]._flag &= ~kObjFlagPerson;
			break;

		case dSHOPKEEPER1A:
			if (_curChoice == 185) {
				doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, kRoom18, a1810ENTRADALNEGOZIO, 10, _vm->_curObj);
				_vm->_obj[oDOORN18]._flag &= ~kObjFlagRoomOut;
				_vm->_obj[oDOORN18]._action = 218;
				_vm->_obj[oDOORN18]._anim = 0;
			} else if (_curChoice == 183)
				_vm->_obj[oTESSERA1A]._action = 239;
			break;

		case dF181:
			_vm->_obj[oRETE17]._mode |= OBJMODE_OBJSTATUS;
			_vm->_obj[oDOORA17]._examine = 196;
			_vm->_obj[oDOORUA17]._examine = 187;
			_vm->_obj[oDOORUB17]._examine = 192;
			_vm->_obj[oDOORA17]._action = 188;
			_vm->_obj[oDOORUA17]._action = 193;
			_vm->_obj[oDOORUB17]._action = 197;
			_vm->_obj[oFINGERPAD17]._mode &= ~OBJMODE_OBJSTATUS;
			_vm->_room[kRoom17]._flag &= ~kObjFlagDone;
			_vm->_room[kRoom17]._flag |= kObjFlagExtra;
			break;

		case dF1C1:
			CharacterSay(334);
			break;

		case dF1D1:
			CharacterSay(1502);
			break;

		case dF2E1:
			CharacterSay(621);
			_vm->_obj[oCATWALKA2E]._action = 622;
			break;

		case dF2E2:
			CharacterSay(1561);
			_vm->_inventoryObj[iRIFLE]._examine = 1562;
			break;

		case dF231:
			_vm->_obj[od21TO23]._goRoom = kRoom23B;
			_vm->_obj[od24TO23]._goRoom = kRoom23B;
			break;

		case dF291:
			_vm->_obj[oSWITCH29]._action = 479;
			_vm->_obj[od22TO29]._goRoom = kRoom29L;
			_vm->_obj[od2ATO29]._goRoom = kRoom29L;
			_vm->_obj[od22TO29]._mode &= ~OBJMODE_OBJSTATUS;
			_vm->_obj[od22TO29I]._mode |= OBJMODE_OBJSTATUS;
			break;

		case dF2G1:
			_vm->_obj[oPANELM2G]._action = 660;
			CharacterSay(659);
			break;

		case dF2G2:
			_vm->_obj[od26TO2G]._goRoom = kRoom2GV;
			_vm->replaceIcon(iSAM, iSAMROTTO);
			break;

		case dF321:
			StartCharacterAction(a3111TRASCINA, 0, 11, 0);
			break;

		case dF331:
			_vm->_obj[oTUBET33]._lim = Common::Rect(0, 0, 0, 0);
			CharacterSay(1997);
			break;

		case dF362:
			PlayDialog(dC381);
			break;

		case dC381:
			PlayDialog(dF381);
			break;

		case dF381:
			doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, kRoom41, 0, 18, _vm->_curObj);
			memcpy(_vm->_cyberInventory, _vm->_inventory, MAXICON);
			_vm->_cyberInventorySize = _vm->_inventorySize;
			_vm->_inventorySize = 0;
			_vm->_iconBase = 0;
			memset(_vm->_inventory, iNULL, MAXICON);
			_vm->_inventory[_vm->_inventorySize++] = iDISLOCATORE;
			break;

		case dF371:
			_vm->_obj[oSCAFFALE36]._anim = a3615AAPRENDESCAFFALE;
			_vm->_animMgr->smkVolumePan(0, 1, 1);
			break;

		case dF431:
			_vm->_flagShowCharacter = true;
			StartCharacterAction(aWALKIN, 0, 11, 0);
			break;

		case dF451:
			_vm->_obj[od44ALLA45]._goRoom = kRoom45S;
			CharacterSay(1712);
			break;

		case dF491:
			for (int c = oPULSANTE1AD; c <= oPULSANTE33AD; c++) {
				if (!(_vm->_obj[c]._goRoom)) {
					_vm->_obj[c]._goRoom = kRoom4A;
					_vm->_obj[c]._mode |= OBJMODE_OBJSTATUS;
					_vm->_obj[c - 40]._mode &= ~OBJMODE_OBJSTATUS;
					break;
				}
			}
			_vm->removeIcon(iROD);
			PlayDialog(dC4A1);
			setPosition(12);
			break;

		case dC4A1:
			_vm->_flagCharacterExists = true;
			_vm->_flagShowCharacter = true;
			actorStop();
			nextStep();
			break;

		case dF4C1:
			memcpy(_vm->_inventory, _vm->_cyberInventory, MAXICON);
			_vm->_inventorySize = _vm->_cyberInventorySize;
			_vm->_iconBase = 0;
			_vm->removeIcon(iCARD03);
			_vm->removeIcon(iPEN);
			_vm->removeIcon(iKEY05);
			_vm->removeIcon(iLETTER12);
			_vm->removeIcon(iCARD14);
			_vm->removeIcon(iFOTO);
			_vm->removeIcon(iPISTOLA15);
			_vm->removeIcon(iRAPPORTO);
			_vm->removeIcon(iTESSERA);
			_vm->removeIcon(iRIFLE);
			_vm->removeIcon(iLAMPADINA29);
			_vm->removeIcon(iTELECOMANDO2G);
			_vm->removeIcon(iCARD36);
			_vm->removeIcon(iFOGLIO36);
			_vm->removeIcon(iSTETOSCOPIO);
			_vm->removeIcon(iREFERTO);
			_vm->removeIcon(iLIBRO);
			_vm->addIcon(iMAPPA50);
			_vm->addIcon(iCUTTER);
			_vm->addIcon(iWALKY);
			StartCharacterAction(a511, 0, 1, 0);
			break;

		case dF4P1:
			CharacterSay(1707);
			break;

		case dF4P2:
			CharacterSay(1708);
			break;

		case dF562:
			_vm->_obj[oDOOR58C55]._action = 1287;
			_vm->_obj[oDOOR58C55]._anim = 0;
			_vm->_obj[oWINDOW58P55]._action = 1292;
			_vm->_obj[oWINDOW58P55]._mode |= OBJMODE_OBJSTATUS;
			_vm->_obj[oWINDOW58P55]._anim = 0;
			break;

		case dF5A1:
			_vm->_obj[oDOOR58C55]._action = 1286;
			_vm->_obj[oWINDOW58P55]._action = 1291;
			_vm->_obj[oWINDOWA5A]._action = 1403;
			_vm->_obj[oGUARDIA58]._mode |= OBJMODE_OBJSTATUS;
			_vm->_choice[286]._flag |= kObjFlagDone;
			break;

		case dC581:
			if (!(_vm->_choice[886]._flag & kObjFlagDone) && (_vm->_choice[258]._flag & kObjFlagDone)) {
				setPosition(1);
				PlayDialog(dF581);
			}
			break;

		case dC582:
			_vm->_obj[oWINDOWA58]._mode |= OBJMODE_OBJSTATUS;
			_vm->addIcon(iCODICE);
			break;

		case dC5A1:
			_vm->_obj[oWINDOWA5A]._action = 1402;
			if (_vm->_room[kRoom5A]._flag & kObjFlagExtra)
				PlayDialog(dF5A1);
			break;

		case dFLOG:
			PlayDialog(dINTRO);
			break;

		case dINTRO:
			doEvent(MC_SYSTEM, ME_CHANGEROOM, MP_SYSTEM, kRoom11, 0, 20, _vm->_curObj);
			break;

		case dF582:
			PlayDialog(dFCRED);
			break;

		case dFCRED:
			doEvent(MC_SYSTEM, ME_QUIT, MP_SYSTEM, 0, 0, 0, 0);
			break;

		default:
			break;
		}
		return;
	}

	// If another dialog starts
	if (_vm->_choice[_curChoice]._nextDialog != 0) {
		_curDialog = _vm->_choice[_curChoice]._nextDialog;
		_vm->_flagDialogActive = true;
		_curChoice = 0;

		d = &_dialog[_curDialog];

		// If there is a pre-dialog
		if (_dialog[_curDialog]._startLen > 0) {
			_vm->_animMgr->playMovie(_dialog[_curDialog]._startAnim, 1, _dialog[_curDialog]._startLen);
			return;
		}
	}

	// Immediately starts the fraud choice
	for (int c = d->_firstChoice; c < (d->_firstChoice + d->_choiceNumb); ++c) {
		if ((_vm->_choice[c]._flag & DLGCHOICE_FRAUD) && (!(_vm->_choice[c]._flag & DLGCHOICE_HIDE))) {
			PlayChoice(c);
			return;
		}
	}

	// If there's only one option, show it immediately, otherwise show available choices
	int res = 0;
	for (int c = d->_firstChoice; c < (d->_firstChoice + d->_choiceNumb); c++) {
		if (!(_vm->_choice[c]._flag & DLGCHOICE_HIDE)) {
			if (_vm->_choice[c]._flag & DLGCHOICE_EXITNOW) {
				if (res == 0)
					res = c;
				else {
					res = 0;
					break;
				}
			} else {
				res = 0;
				break;
			}
		}
	}
	if (res != 0) {
		PlayChoice(res);
		return;
	}

	// If no option is visible, close the dialog
	res = 0;
	for (int c = d->_firstChoice; c < (d->_firstChoice + d->_choiceNumb); c++) {
		if (!(_vm->_choice[c]._flag & DLGCHOICE_HIDE))
			res++;
	}

	if (res == 0) {
		_vm->_animMgr->stopFullMotion();
		if (_curDialog == dC381)
			PlayDialog(dF381);
		return;
	}

	ShowChoices(_curDialog);
}

void DialogManager::DialogHandler(int numFrame) {
	if (_vm->_flagDialogActive && !_vm->_flagDialogMenuActive) {
		_vm->hideCursor();
		if (numFrame == _subTitles[_curSubTitle]._startFrame) {
			int i = _curSubTitle++;
			_vm->_sdText.x = _subTitles[i]._x;
			_vm->_sdText.y = _subTitles[i]._y;
			_vm->_sdText.tcol = _subTitles[i]._color;
			_vm->_sdText.text = _vm->_sentence[_subTitles[i]._sentence];
		}
	}
}

void DialogManager::PlayChoice(uint16 i) {
	DialogChoice *ss = &_vm->_choice[i];

	memset(_vm->_screenBuffer, 0, MAXX * TOP * 2);
	_vm->_graphicsMgr->copyToScreen(0, 0, MAXX, TOP);

	_curChoice = i;
	_curSubTitle = ss->_firstSubTitle;
	_vm->_flagDialogMenuActive = false;

	ss->_flag |= kObjFlagDone;

	// se era one time la disabilita
	if (ss->_flag & DLGCHOICE_ONETIME)
		ss->_flag |= DLGCHOICE_HIDE;

	// Disable other choices
	for (int c = 0; c < MAXDISPCHOICES; c++) {
		_vm->_choice[ss->_off[c]]._flag |= DLGCHOICE_HIDE;
		_vm->_choice[ss->_on[c]]._flag &= ~DLGCHOICE_HIDE;
	}

	int totalLength = 0;
	int subTitleCount = ss->_firstSubTitle + ss->_subTitleNumb;
	for (int c = _curSubTitle; c < subTitleCount; c++)
		totalLength += _subTitles[c]._length;

	_vm->hideCursor();
	_vm->_animMgr->playMovie(_dialog[_curDialog]._startAnim, ss->_startFrame, ss->_startFrame + totalLength - 1);
}

void DialogManager::doDialog() {
	switch (_vm->_curMessage->_event) {
	case ME_ENDCHOICE:
		afterChoice(_vm->_curMessage->_u16Param1);
		break;

	case ME_STARTDIALOG:
		PlayDialog(_vm->_curMessage->_u16Param1);
		break;

	default:
		break;
	}
}

} // End of namespace Trecision
