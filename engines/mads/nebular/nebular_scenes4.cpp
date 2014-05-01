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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "common/scummsys.h"
#include "mads/mads.h"
#include "mads/scene.h"
#include "mads/nebular/nebular_scenes.h"
#include "mads/nebular/nebular_scenes4.h"

namespace MADS {

namespace Nebular {

void Scene4xx::setAAName() {
	_game._aaName = Resources::formatAAName(4);
}

void Scene4xx::setPlayerSpritesPrefix() {
	_vm->_sound->command(5);
	Common::String oldName = _game._player._spritesPrefix;

	if ((_scene->_nextSceneId == 403) || (_scene->_nextSceneId == 409))
		_game._player._spritesPrefix = "";
	else if (_globals[kSexOfRex] == REX_FEMALE)
		_game._player._spritesPrefix = "ROX";
	else
		_game._player._spritesPrefix = "RXM";

	_game._player._scalingVelocity = true;

	if (oldName != _game._player._spritesPrefix)
		_game._player._spritesChanged = true;

	_vm->_palette->setEntry(16, 10, 63, 63);
	_vm->_palette->setEntry(17, 10, 45, 45);
}

void Scene4xx::sceneEntrySound() {
	if (!_vm->_musicFlag) {
		_vm->_sound->command(2);
		return;
	}

	switch (_scene->_nextSceneId) {
	case 401:
		_vm->_sound->startQueuedCommands();
		if (_scene->_priorSceneId == 402)
			_vm->_sound->command(12, 64);
		else
			_vm->_sound->command(12, 1);
		break;

	case 402:
		_vm->_sound->startQueuedCommands();
		_vm->_sound->command(12, 127);
		break;

	case 405:
	case 407:
	case 409:
	case 410:
	case 413:
		_vm->_sound->command(10);
		break;

	case 408:
		_vm->_sound->command(52);
		break;

	default:
		break;
	}
}

/*------------------------------------------------------------------------*/

void Scene401::setup() {
	setPlayerSpritesPrefix();
	setAAName();
}

void Scene401::enter() {
	if (_scene->_priorSceneId != -2)
		_northFl = false;

	_timer = 0;

	if (_scene->_priorSceneId == 402) {
		_game._player._playerPos = Common::Point(203, 115);
		_game._player._facing = FACING_WEST;
	} else if (_scene->_priorSceneId == 354) {
		_game._player._playerPos = Common::Point(149, 90);
		_game._player._facing = FACING_SOUTH;
		_northFl = true;
	} else if (_scene->_priorSceneId != -2) {
		_game._player._playerPos = Common::Point(142, 131);
		_game._player._facing = FACING_NORTH;
	}

	_game.loadQuoteSet(0x1D4, 0);
	sceneEntrySound();
}

void Scene401::step() {
	if (_game._trigger == 70) {
		_scene->_nextSceneId = 354;
		_scene->_reloadSceneFlag = true;
	}

	if (_game._trigger == 80) {
		_game._player._priorTimer = _scene->_frameStartTime - _game._player._ticksAmount;
		_game._player._stepEnabled = true;
		_game._player._visible = true;
		_northFl = false;
		_game._player.walk(Common::Point(149, 110), FACING_SOUTH);
	}

	if (_scene->_frameStartTime >= _timer) {
		int dist = 64 - ((_vm->hypotenuse(_game._player._playerPos.x - 219, _game._player._playerPos.y - 115) * 64) / 120);

		if (dist > 64)
			dist = 64;
		else if (dist < 1)
			dist = 1;

		_vm->_sound->command(12, dist);
		_timer = _scene->_frameStartTime + _game._player._ticksAmount;
	}

}

void Scene401::preActions() {
	if (_action.isAction(0x1AD, 0x2B4)) {
		_game._player.walk(Common::Point(149, 89), FACING_NORTH);
		_northFl = false;
	}

	if (_action.isAction(0x1AD, 0x2B3) && !_northFl)
		_game._player._walkOffScreenSceneId = 405;

	if (_action.isAction(VERB_TAKE))
		_game._player._needToWalk = false;

	if (_game._player._needToWalk && _northFl) {
		if (_globals[kSexOfRex] == REX_MALE)
			_destPos = Common::Point(148, 94);
		else
			_destPos = Common::Point(149, 99);

		_game._player.walk(_destPos, FACING_SOUTH);
	}
}

void Scene401::actions() {
	if ((_game._player._playerPos == _destPos) && _northFl) {
		if (_globals[kSexOfRex] == REX_MALE) {
			_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
			_game._player._stepEnabled = false;
			_game._player._visible = false;
			_vm->_sound->command(21);
			_scene->loadAnimation(formAnimName('s', 1), 70);
			_globals[kHasBeenScanned] = true;
			_vm->_sound->command(22);
			int idx = _scene->_kernelMessages.add(Common::Point(153, 46), 0x1110, 32, 0, 60, _game.getQuote(0x1D4));
			_scene->_kernelMessages.setQuoted(idx, 4, true);
		}

		if (_globals[kSexOfRex] == REX_FEMALE) {
			_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
			_game._player._stepEnabled = false;
			_game._player._visible = false;
			_vm->_sound->command(21);
			_scene->loadAnimation(formAnimName('s', 2), 80);
			_vm->_sound->command(23);
			_globals[kHasBeenScanned] = true;
		}
	}

	if (_action.isAction(0x242, 0x241)) {
		if (!_northFl)
			_scene->_nextSceneId = 402;
	} else if (_action.isAction(0x1AD, 0x2B4))
		_scene->_nextSceneId = 354;
	else if (_action.isAction(VERB_LOOK, 0x1F3)) {
		if (_globals[kHasBeenScanned])
			_vm->_dialogs->show(40111);
		else
			_vm->_dialogs->show(40110);
	} else if (_action.isAction(VERB_LOOK, 0x241))
		_vm->_dialogs->show(40112);
	else if (_action.isAction(VERB_LOOK, 0x244))
		_vm->_dialogs->show(40113);
	else if (_action.isAction(VERB_LOOK, 0x2B3))
		_vm->_dialogs->show(40114);
	else if (_action.isAction(VERB_LOOK, 0x2B4))
		_vm->_dialogs->show(40115);
	else if (_action._lookFlag)
		_vm->_dialogs->show(40116);
	else
		return;

	_action._inProgress = false;
}

/*------------------------------------------------------------------------*/
void Scene402::setup() {
	setPlayerSpritesPrefix();
	setAAName();

	_scene->addActiveVocab(0x30E);
	_scene->addActiveVocab(0x306);
	_scene->addActiveVocab(0x72);
	_scene->addActiveVocab(0x27);
	_scene->addActiveVocab(0xD);
	_scene->addActiveVocab(0x5C);
	_scene->addActiveVocab(0x4);
	_scene->addActiveVocab(0x124);
	_scene->addActiveVocab(0xD1);
}

void Scene402::setDialogNode(int node) {
	if (node > 0)
		_bartenderDialogNode = node;

	_game._player._stepEnabled = true;

	switch (node) {
	case 0:
		_scene->_userInterface.setup(kInputBuildingSentences);
		_conversationFl = false;
		_bartenderDialogNode = 0;
		break;

	case 1:
		_dialog1.start();
		_bartenderDialogNode = 1;
		break;

	case 2:
		_dialog2.start();
		_bartenderDialogNode = 2;
		break;

	case 3:
		_dialog3.start();
		_bartenderDialogNode = 3;
		break;

	case 4:
		_dialog4.start();
		_bartenderDialogNode = 4;
		break;

	default:
		break;
	}
}

void Scene402::handleConversation1() {
	switch (_action._activeAction._verbId) {
	case 0x214: {
		int quoteId = 0;
		int quotePosX = 0;
		switch (_vm->getRandomNumber(1, 3)) {
		case 1:
			quoteId = 0x1E4;
			_bartenderCurrentQuestion = 4;
			quotePosX = 205;
			break;

		case 2:
			quoteId = 0x1E5;
			_bartenderCurrentQuestion = 5;
			quotePosX = 203;
			break;

		case 3:
			quoteId = 0x1E6;
			_bartenderCurrentQuestion = 6;
			quotePosX = 260;
			break;

		default:
			break;
		}
		_scene->_kernelMessages.reset();
		_scene->_kernelMessages.add(Common::Point(quotePosX, 41), 0xFDFC, 0, 0, 9999999, _game.getQuote(quoteId));
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_scene->_sequences.addTimer(1, 100);
		_talkTimer = 120;
		setDialogNode(2);
		}
		break;

	case 0x215:
		_scene->_kernelMessages.reset();
		_scene->_kernelMessages.add(Common::Point(260, 41), 0xFDFC, 0, 0, 9999999, _game.getQuote(0x1EC));
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_scene->_sequences.addTimer(1, 100);
		_talkTimer = 120;
		_bartenderCurrentQuestion = 1;
		setDialogNode(3);
		break;

	case 0x237:
		_scene->_kernelMessages.reset();
		_scene->_kernelMessages.add(Common::Point(208, 41), 0xFDFC, 0, 0, 100, _game.getQuote(0x1FD));
		setDialogNode(0);
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_scene->_sequences.addTimer(1, 100);
		_talkTimer = 1120;
		break;

	default:
		break;
	}
}

void Scene402::handleConversation2() {
	switch (_action._activeAction._verbId) {
	case 0x216:
		_dialog2.write(0x216, false);
		_dialog2.write(0x21D, true);
		break;

	case 0x219:
		_dialog2.write(0x219, false);
		_dialog2.write(0x220, true);
		break;

	case 0x21A:
		_dialog2.write(0x21A, false);
		_dialog2.write(0x223, true);
		break;

	case 0x21B:
		_dialog2.write(0x21B, false);
		_dialog2.write(0x224, true);
		break;

	case 0x21D:
		_dialog2.write(0x21D, false);
		_dialog2.write(0x227, true);
		break;

	case 0x220:
		_dialog2.write(0x220, false);
		_dialog2.write(0x22A, true);
		break;

	case 0x223:
		_dialog2.write(0x223, false);
		_dialog2.write(0x22D, true);
		break;

	case 0x224:
		_dialog2.write(0x224, false);
		_dialog2.write(0x230, true);
		break;

	case 0x227:
		_dialog2.write(0x227, false);
		break;

	case 0x22A:
		_dialog2.write(0x22A, false);
		break;

	case 0x22D:
		_dialog2.write(0x22D, false);
		break;

	case 0x230:
		_dialog2.write(0x230, false);
		break;

	case 0x21C:
		setDialogNode(0);
		break;

	default:
		break;

	}

	if (_action._activeAction._verbId != 0x21C) {
		switch (_vm->getRandomNumber(1, 3)) {
		case 1:
			_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
			_scene->_sequences.addTimer(1, 100);
			_talkTimer = 180;
			_scene->_kernelMessages.reset();
			_scene->_kernelMessages.add(Common::Point(198, 27), 0xFDFC, 0, 0, 9999999, _game.getQuote(0x1E7));
			_scene->_kernelMessages.add(Common::Point(201, 41), 0xFDFC, 0, 0, 9999999, _game.getQuote(0x1E8));
			_bartenderCurrentQuestion = 7;
			break;

		case 2:
			_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
			_scene->_sequences.addTimer(1, 100);
			_talkTimer = 180;
			_scene->_kernelMessages.reset();
			_scene->_kernelMessages.add(Common::Point(220, 27), 0xFDFC, 0, 0, 9999999, _game.getQuote(0x1E9));
			_scene->_kernelMessages.add(Common::Point(190, 41), 0xFDFC, 0, 0, 9999999, _game.getQuote(0x1EA));
			_bartenderCurrentQuestion = 8;
			break;

		case 3:
			_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
			_scene->_sequences.addTimer(1, 100);
			_talkTimer = 150;
			_scene->_kernelMessages.reset();
			_scene->_kernelMessages.add(Common::Point(196, 41), 0xFDFC, 0, 0, 9999999, _game.getQuote(0x1EB));
			_bartenderCurrentQuestion = 9;
			break;

		default:
			break;
		}
		_dialog2.start();
	} else {
		_scene->_kernelMessages.reset();
		_scene->_kernelMessages.add(Common::Point(208, 41), 0xFDFC, 0, 0, 100, _game.getQuote(0x1FD));
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_scene->_sequences.addTimer(1, 100);
		_talkTimer = 1120;
	}
}

void Scene402::handleConversation3() {
	switch (_action._activeAction._verbId) {
	case 0x233:
	case 0x234:
	case 0x235:
	case 0x236:
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_scene->_sequences.addTimer(1, 86);
		_scene->_kernelMessages.reset();
		_scene->_kernelMessages.add(Common::Point(188, 27), 0xFDFC, 0, 0, 9999999, _game.getQuote(0x1ED));
		_scene->_kernelMessages.add(Common::Point(199, 41), 0xFDFC, 0, 0, 9999999, _game.getQuote(0x1EE));
		setDialogNode(4);
		_bartenderCurrentQuestion = 2;
		break;

	case 0x237:
		_scene->_kernelMessages.reset();
		_scene->_kernelMessages.add(Common::Point(208, 41), 0xFDFC, 0, 0, 100, _game.getQuote(0x1FD));
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_scene->_sequences.addTimer(1, 100);
		_talkTimer = 1120;
		setDialogNode(0);
		break;

	default:
		break;
	}
}

void Scene402::handleConversation4() {
	switch (_action._activeAction._verbId) {
	case 0x238:
		_scene->_kernelMessages.reset();
		setDialogNode(0);
		_game._player._stepEnabled = false;
		_scene->_kernelMessages.add(Common::Point(196, 13), 0xFDFC, 0, 0, 180, _game.getQuote(0x1F0));
		_scene->_kernelMessages.add(Common::Point(184, 27), 0xFDFC, 0, 0, 180, _game.getQuote(0x1F1));
		_scene->_kernelMessages.add(Common::Point(200, 41), 0xFDFC, 0, 0, 180, _game.getQuote(0x1F2));
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_scene->_sequences.addTimer(1, 100);
		_talkTimer = 1100;
		_dialog4.write(0x238, false);
		_bartenderMode = 22;
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_scene->_sequences.addTimer(100, 95);
		_refuseAlienLiquor = true;
		break;

	case 0x239:
		_game._player._stepEnabled = false;
		_roxMode = 21;
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_scene->_sequences.addTimer(1, 92);
		_scene->_userInterface.setup(kInputBuildingSentences);
		_conversationFl = false;
		break;

	case 0x23A:
		setDialogNode(0);
		_game._player._stepEnabled = false;
		_scene->_kernelMessages.add(Common::Point(193, 27), 0xFDFC, 0, 0, 150, _game.getQuote(0x1F4));
		_scene->_kernelMessages.add(Common::Point(230, 41), 0xFDFC, 0, 0, 150, _game.getQuote(0x1F5));
		_dialog4.write(0x23A, false);
		_globals[kHasSaidTimer] = true;
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_scene->_sequences.addTimer(1, 100);
		_talkTimer = 1100;
		_bartenderMode = 22;
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_scene->_sequences.addTimer(100, 95);
		_refuseAlienLiquor = true;
		break;

	case 0x23D:
		setDialogNode(0);
		_game._player._stepEnabled = false;
		_scene->_kernelMessages.add(Common::Point(153, 27), 0xFDFC, 0, 0, 150, _game.getQuote(0x1F6));
		_scene->_kernelMessages.add(Common::Point(230, 41), 0xFDFC, 0, 0, 150, _game.getQuote(0x1F7));
		_dialog4.write(0x23D, false);
		_globals[kHasSaidBinocs] = true;
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_scene->_sequences.addTimer(1, 100);
		_talkTimer = 1100;
		_bartenderMode = 22;
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_scene->_sequences.addTimer(100, 95);
		_refuseAlienLiquor = true;
		break;

	case 0x23E:
		_scene->_kernelMessages.reset();
		setDialogNode(0);
		_game._player._stepEnabled = false;
		_scene->_kernelMessages.add(Common::Point(205, 41), 0xFDFC, 0, 0, 100, _game.getQuote(0x1F8));
		_bartenderMode = 22;
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_scene->_sequences.addTimer(1, 100);
		_talkTimer = 1050;
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_scene->_sequences.addTimer(50, 95);
		_refuseAlienLiquor = true;
		break;

	default:
		break;
	}
}

void Scene402::handleDialogs() {
	if (_game._trigger == 0) {
		_scene->_kernelMessages.reset();
		_game._player._stepEnabled = false;
		Common::String curQuote = _game.getQuote(_action._activeAction._verbId);
		if (_vm->_font->getWidth(curQuote, _scene->_textSpacing) > 200) {
			Common::String subQuote1, subQuote2;
			_game.splitQuote(curQuote, subQuote1, subQuote2);
			_scene->_kernelMessages.add(Common::Point(230, 42), 0x1110, 32, 0, 140, subQuote1);
			_scene->_kernelMessages.add(Common::Point(230, 56), 0x1110, 32, 0, 140, subQuote2);
			_scene->_sequences.addTimer(160, 120);
		} else {
			_scene->_kernelMessages.add(Common::Point(230, 56), 0x1110, 32, 1, 140, curQuote);
			_scene->_sequences.addTimer(160, 120);
		}
	} else if (_game._trigger == 120) {
		_game._player._stepEnabled = true;
		switch (_bartenderDialogNode) {
		case 1:
			handleConversation1();
			break;

		case 2:
			handleConversation2();
			break;

		case 3:
			handleConversation3();
			break;

		case 4:
			handleConversation4();
			break;

		default:
			break;
		}
	}
}

void Scene402::enter() {
	_globals._spriteIndexes[0] = _scene->_sprites.addSprites(formAnimName('n', -1));
	_globals._spriteIndexes[1] = _scene->_sprites.addSprites(formAnimName('x', 2));
	_globals._spriteIndexes[3] = _scene->_sprites.addSprites(formAnimName('x', 0));
	_globals._spriteIndexes[2] = _scene->_sprites.addSprites(formAnimName('x', 1));
	_globals._spriteIndexes[4] = _scene->_sprites.addSprites(formAnimName('g', 0));
	_globals._spriteIndexes[5] = _scene->_sprites.addSprites(formAnimName('g', 1));
	_globals._spriteIndexes[6] = _scene->_sprites.addSprites(formAnimName('a', 0));
	_globals._spriteIndexes[7] = _scene->_sprites.addSprites(formAnimName('a', 1));
	_globals._spriteIndexes[15] = _scene->_sprites.addSprites(formAnimName('x', 5));
	_globals._spriteIndexes[8] = _scene->_sprites.addSprites(formAnimName('x', 4));
	_globals._spriteIndexes[9] = _scene->_sprites.addSprites(formAnimName('b', 0));
	_globals._spriteIndexes[10] = _scene->_sprites.addSprites(formAnimName('b', 1));
	_globals._spriteIndexes[11] = _scene->_sprites.addSprites(formAnimName('b', 2));
	_globals._spriteIndexes[12] = _scene->_sprites.addSprites(formAnimName('b', 3));
	_globals._spriteIndexes[13] = _scene->_sprites.addSprites(formAnimName('c', 0));
	_globals._spriteIndexes[14] = _scene->_sprites.addSprites(formAnimName('l', 0));
	_globals._spriteIndexes[16] = _scene->_sprites.addSprites(formAnimName('h', 0));
	_globals._spriteIndexes[17] = _scene->_sprites.addSprites(formAnimName('z', 0));
	_globals._spriteIndexes[18] = _scene->_sprites.addSprites(formAnimName('z', 1));
	_globals._spriteIndexes[19] = _scene->_sprites.addSprites(formAnimName('z', 2));
	_globals._spriteIndexes[20] = _scene->_sprites.addSprites(formAnimName('x', 6));
	_globals._spriteIndexes[21] = _scene->_sprites.addSprites("*ROXRC_9");
	_globals._spriteIndexes[22] = _scene->_sprites.addSprites("*ROXCL_8");

	if (_scene->_priorSceneId == 401) {
		_game._player._playerPos = Common::Point(160, 150);
		_game._player._facing = FACING_NORTH;
		_roxOnStool = false;
		_bartenderDialogNode = 1;
		_conversationFl = false;
	} else if (_scene->_priorSceneId != -2) {
		_game._player._playerPos = Common::Point(160, 150);
		_game._player._facing = FACING_NORTH;
		_game._objects.addToInventory(OBJ_CREDIT_CHIP);
		_game._objects.addToInventory(OBJ_BINOCULARS);
		_game._objects.addToInventory(OBJ_TIMER_MODULE);
		_roxOnStool = false;
		_bartenderDialogNode = 1;
		_conversationFl = false;
	}

	_game.loadQuoteSet(0x1D7, 0x1D8, 0x1D9, 0x1DA, 0x1DB, 0x1DC, 0x1DD, 0x1DE, 0x1DF, 0x1E2, 0x1E3, 0x1E6, 0x1E5, 0x1E7,
		0x1E8, 0x1E9, 0x1EA, 0x1EF, 0x1F0, 0x1F1, 0x1F2, 0x1F3, 0x1F4, 0x1F5, 0x1F6, 0x1F7, 0x1F8, 0x1F9, 0x1FA, 0x1FB,
		0x1FC, 0x1EB, 0x1EC, 0x1ED, 0x1EE, 0x1E4, 0x1FD, 0x1E0, 0x1E1, 0x1FE, 0x1FF, 0x200, 0x201, 0x202, 0x203, 0x204,
		0x205, 0x206, 0x207, 0x208, 0x209, 0x20A, 0x20B, 0x20C, 0x20F, 0x20D, 0x20E, 0x210, 0x211, 0x212, 0x213, 0x214,
		0x215, 0x237, 0x216, 0x219, 0x21A, 0x21B, 0x21C, 0x21D, 0x220, 0x223, 0x224, 0x227, 0x22A, 0x22D, 0x230, 0x233,
		0x234, 0x235, 0x236, 0x238, 0x239, 0x23A, 0x23D, 0x23E, 0x23F, 0);

	_vm->_palette->setEntry(250, 47, 41, 40);
	_vm->_palette->setEntry(251, 50, 63, 55);
	_vm->_palette->setEntry(252, 38, 34, 25);
	_vm->_palette->setEntry(253, 45, 41, 35);

	_dialog1.setup(0x60, 0x214, 0x215, 0x237, 0);
	_dialog2.setup(0x61, 0x216, 0x219, 0x21A, 0x21B, 0x21D, 0x220, 0x223, 0x224, 0x227, 0x22A, 0x22D, 0x230, 0x21C, 0);
	_dialog3.setup(0x62, 0x233, 0x234, 0x235, 0x236, 0x237, -1);
	_dialog4.setup(0x63, 0x238, 0x239, 0x23A, 0x23D, 0x23E, 0);

	if (!_game._visitedScenes._sceneRevisited) {
		_dialog2.set(0x61, 0x216, 0x219, 0x21A, 0x21B, 0x21C, 0);
		_dialog4.set(0x63, 0x238, 0x23E, 0);
		_dialog1.set(0x60, 0x214, 0x215, 0x237, 0);
	}

	if (_game._objects.isInInventory(OBJ_CREDIT_CHIP))
		_dialog4.write(0x239, true);
	else
		_dialog4.write(0x239, false);

	if (_game._objects.isInInventory(OBJ_BINOCULARS) && !_globals[kHasSaidBinocs])
		_dialog4.write(0x23D, true);
	else
		_dialog4.write(0x23D, false);

	if (_game._objects.isInInventory(OBJ_TIMER_MODULE) && !_globals[kHasSaidTimer])
		_dialog4.write(0x23A, true);
	else
		_dialog4.write(0x23A, false);

	if (_dialog2.read(0) <= 1)
		_dialog1.write(0x214, false);

	if (_conversationFl) {
		switch (_bartenderDialogNode) {
		case 0:
			_scene->_userInterface.setup(kInputBuildingSentences);
			_bartenderDialogNode = 1;
			break;

		case 1:
			_dialog1.start();
			break;

		case 2:
			_dialog2.start();
			break;

		case 3:
			_dialog3.start();
			break;

		case 4:
			_dialog4.start();
			break;

		default:
			break;
		}

		switch (_bartenderCurrentQuestion) {
		case 1:
			_scene->_kernelMessages.add(Common::Point(260, 41), 0xFDFC, 0, 0, 9999999, _game.getQuote(0x1EC));
			break;

		case 2:
			_scene->_kernelMessages.add(Common::Point(188, 27), 0xFDFC, 0, 0, 9999999, _game.getQuote(0x1ED));
			_scene->_kernelMessages.add(Common::Point(199, 41), 0xFDFC, 0, 0, 9999999, _game.getQuote(0x1EE));
			break;

		case 3:
			_scene->_kernelMessages.add(Common::Point(177, 41), 0xFDFC, 0, 0, 9999999, _game.getQuote(0x1EF));
			break;

		case 4:
			_scene->_kernelMessages.add(Common::Point(205, 41), 0xFDFC, 0, 0, 9999999, _game.getQuote(0x1E4));
			break;

		case 5:
			_scene->_kernelMessages.add(Common::Point(203, 41), 0xFDFC, 0, 0, 9999999, _game.getQuote(0x1E5));
			break;

		case 6:
			_scene->_kernelMessages.add(Common::Point(260, 41), 0xFDFC, 0, 0, 9999999, _game.getQuote(0x1E6));
			break;

		case 7:
			_scene->_kernelMessages.add(Common::Point(198, 27), 0xFDFC, 0, 0, 9999999, _game.getQuote(0x1E7));
			_scene->_kernelMessages.add(Common::Point(201, 41), 0xFDFC, 0, 0, 9999999, _game.getQuote(0x1E8));
			break;

		case 8:
			_scene->_kernelMessages.add(Common::Point(220, 27), 0xFDFC, 0, 0, 9999999, _game.getQuote(0x1E9));
			_scene->_kernelMessages.add(Common::Point(190, 41), 0xFDFC, 0, 0, 9999999, _game.getQuote(0x1EA));
			break;

		case 9:
			_scene->_kernelMessages.add(Common::Point(196, 41), 0xFDFC, 0, 0, 9999999, _game.getQuote(0x1EB));
			break;

		case 10:
			_scene->_kernelMessages.add(Common::Point(198, 27), 0xFDFC, 0, 0, 9999999, _game.getQuote(0x1E2));
			_scene->_kernelMessages.add(Common::Point(199, 41), 0xFDFC, 0, 0, 9999999, _game.getQuote(0x1E3));
			break;

		default:
			break;
		}
	}

	_firstTalkToGirlInChair = false;
	_lightOn = false;
	_blowingSmoke = false;
	_leftWomanMoving = false;
	_rightWomanMoving = false;
	_ginnyLooking = false;
	_beatCounter = 0;
	_waitingGinnyMove = false;
	_bigBeatFl = true;
	_bartenderHandsHips = false;
	_bartenderSteady = true;
	_bartenderLooksLeft = false;
	_activeTeleporter = false;
	_activeArrows = false;
	_activeArrow1 = false;
	_activeArrow2 = false;
	_activeArrow3 = false;
	_cutSceneReady = false;
	_cutSceneNeeded = false;
	_helgaReady = true;
	_bartenderReady = true;
	_drinkTimer = 0;
	_bartenderTalking = false;
	_bartenderCalled = false;
	_helgaTalkMode = 0;
	_rexMode = 0;
	_refuseAlienLiquor = false;

	_scene->loadAnimation(Resources::formatName(402, 'd', 1, EXT_AA, ""));
	_scene->_activeAnimation->_resetFlag = true;

	_globals._sequenceIndexes[5] = _scene->_sequences.startCycle(_globals._spriteIndexes[5], false, 1);
	_scene->_sequences.setDepth(_globals._sequenceIndexes[5], 1);

	_globals._sequenceIndexes[13] = _scene->_sequences.startCycle(_globals._spriteIndexes[13], false, 2);
	_scene->_sequences.setDepth(_globals._sequenceIndexes[13], 8);

	if (!_game._objects.isInInventory(OBJ_REPAIR_LIST)) {
		_globals._sequenceIndexes[14] = _scene->_sequences.startCycle(_globals._spriteIndexes[14], false, 1);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[14], 7);
		_scene->_sequences.setMsgPosition(_globals._sequenceIndexes[14], Common::Point(210, 80));
		int idx = _scene->_dynamicHotspots.add(0x124, 0xD1, _globals._sequenceIndexes[14], Common::Rect(0, 0, 0, 0));
		_scene->_dynamicHotspots.setPosition(idx, Common::Point(0, 0), FACING_NONE);
	}

	{
		_globals._sequenceIndexes[9] = _scene->_sequences.startCycle(_globals._spriteIndexes[9], false, 1);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[9], 8);
		int idx = _scene->_dynamicHotspots.add(0x30E, 0xD, _globals._sequenceIndexes[9], Common::Rect(0, 0, 0, 0));
		_scene->_dynamicHotspots.setPosition(idx, Common::Point(228, 83), FACING_SOUTH);
	}

	if (!_game._objects.isInRoom(OBJ_ALIEN_LIQUOR) && !_game._objects.isInInventory(OBJ_CREDIT_CHIP)) {
		_globals._sequenceIndexes[15] = _scene->_sequences.startCycle(_globals._spriteIndexes[15], false, 1);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[15], 8);
		_scene->_sequences.setMsgPosition(_globals._sequenceIndexes[15], Common::Point(250, 80));
		int idx = _scene->_dynamicHotspots.add(0x5C, 0x4, _globals._sequenceIndexes[15], Common::Rect(0, 0, 0, 0));
		_scene->_dynamicHotspots.setPosition(idx, Common::Point(0, 0), FACING_NONE);
	}

	_globals._sequenceIndexes[20] = _scene->_sequences.startCycle(_globals._spriteIndexes[20], false, 1);
	_scene->_sequences.setDepth(_globals._sequenceIndexes[20], 7);
	_scene->_sequences.setMsgPosition(_globals._sequenceIndexes[20], Common::Point(234, 72));

	if (_globals[kBottleDisplayed]) {
		_globals._sequenceIndexes[8] = _scene->_sequences.startCycle(_globals._spriteIndexes[8], false, 4);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[8], 7);
	}

	if (_roxOnStool) {
		_globals._sequenceIndexes[6] = _scene->_sequences.startCycle(_globals._spriteIndexes[6], false, 11);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[6], 5);
		_game._player._visible = false;
	}

	sceneEntrySound();
}

void Scene402::step() {
	if (_game._trigger == 104) {
		_game._player._priorTimer = _scene->_frameStartTime + _game._player._ticksAmount;
		_game._player._visible = true;
		_scene->_sequences.remove(_globals._sequenceIndexes[15]);
		_game._objects.addToInventory(OBJ_CREDIT_CHIP);
		_vm->_dialogs->showItem(OBJ_CREDIT_CHIP, 40242);
		_game._player._stepEnabled = true;
	}

	if ((_vm->getRandomNumber(1, 1500) == 1) && (!_activeTeleporter) && (_game._player._playerPos.x < 150)) {
		_vm->_sound->command(30);
		_globals._sequenceIndexes[4] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[4], false, 13, 1, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[4], 1, 11);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[4], 14);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[4], SEQUENCE_TRIGGER_EXPIRE, 0, 48);
		_activeTeleporter = true;
		_globals[kSomeoneHasExploded] = true;
	}

	if (_game._trigger == 48)
		_activeTeleporter = false;

	if (_game._trigger == 100) {
		_bartenderReady = false;
		if (_bartenderHandsHips) {
			_scene->_sequences.remove(_globals._sequenceIndexes[10]);
			_bartenderHandsHips = false;
		} else if (_bartenderLooksLeft) {
			_scene->_sequences.remove(_globals._sequenceIndexes[11]);
			_bartenderLooksLeft = false;
		} else if (_bartenderSteady) {
			_scene->_sequences.remove(_globals._sequenceIndexes[9]);
			_bartenderSteady = false;
		}

		if (!_bartenderTalking) {
			_globals._sequenceIndexes[10] = _scene->_sequences.startReverseCycle(_globals._spriteIndexes[10], false, 7, 0, 0, 0);
			_scene->_sequences.setAnimRange(_globals._sequenceIndexes[10], 3, 4);
			_scene->_sequences.setDepth(_globals._sequenceIndexes[10], 8);
			int idx = _scene->_dynamicHotspots.add(0x30E, 0xD, _globals._sequenceIndexes[10], Common::Rect(0, 0, 0, 0));
			_scene->_dynamicHotspots.setPosition(idx, Common::Point(228, 83), FACING_SOUTH);
			_bartenderTalking = true;
			if (_talkTimer > 1000)
				_scene->_sequences.addTimer(_talkTimer - 1000, 101);
			else
				_scene->_sequences.addTimer(_talkTimer, 101);
		}
	}

	if ((_game._trigger == 101) && _bartenderTalking) {
		_scene->_sequences.remove(_globals._sequenceIndexes[10]);
		_globals._sequenceIndexes[9] = _scene->_sequences.startCycle(_globals._spriteIndexes[9], false, 1);
		int idx = _scene->_dynamicHotspots.add(0x30E, 0xD, _globals._sequenceIndexes[9], Common::Rect(0, 0, 0, 0));
		_scene->_dynamicHotspots.setPosition(idx, Common::Point(228, 83), FACING_SOUTH);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[9], 8);
		_bartenderSteady = true;
		_bartenderTalking = false;
		if (_talkTimer < 1000)
			_bartenderReady = true;
	}

	if (_game._trigger == 28)
		_game._player._stepEnabled = true;

	switch (_game._trigger) {
	case 92:
		_game._player._stepEnabled = false;
		_scene->_sequences.remove(_globals._sequenceIndexes[6]);
		_globals._sequenceIndexes[7] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[7], false, 10, 1, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[7], 1, 2);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[7], 5);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[7], SEQUENCE_TRIGGER_EXPIRE, 0, 93);
		break;

	case 93: {
		int seqIdx = _globals._sequenceIndexes[7];
		switch (_roxMode) {
		case 20:
			_vm->_sound->command(57);
			_scene->_sequences.remove(_globals._sequenceIndexes[15]);
			_game._objects.addToInventory(OBJ_CREDIT_CHIP);
			_vm->_dialogs->showItem(OBJ_CREDIT_CHIP, 40242);
			break;

		case 22:
			_vm->_sound->command(57);
			_scene->_sequences.remove(_globals._sequenceIndexes[8]);
			_game._objects.addToInventory(OBJ_ALIEN_LIQUOR);
			_globals[kBottleDisplayed] = false;
			_vm->_dialogs->showItem(OBJ_ALIEN_LIQUOR, 40241);
			break;

		case 21:
			_globals._sequenceIndexes[15] = _scene->_sequences.startCycle(_globals._spriteIndexes[15], false, 1);
			_scene->_sequences.setDepth(_globals._sequenceIndexes[15], 8);
			_scene->_sequences.setMsgPosition(_globals._sequenceIndexes[15], Common::Point(250, 80));
			break;

		default:
			break;
		}
		_globals._sequenceIndexes[7] = _scene->_sequences.addReverseSpriteCycle(_globals._spriteIndexes[7], false, 10, 1, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[7], 1, 3);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[7], 5);
		_scene->_sequences.updateTimeout(_globals._sequenceIndexes[7], seqIdx);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[7], SEQUENCE_TRIGGER_EXPIRE, 0, 94);

		if (_roxMode == 21) {
			if (_game._objects.isInInventory(OBJ_CREDIT_CHIP))
				_game._objects.setRoom (OBJ_CREDIT_CHIP, NOWHERE);

			_bartenderMode = 20;
			_scene->_sequences.addTimer(60, 95);
		}
		}
		break;

	case 94: {
		int seqIdx = _globals._sequenceIndexes[7];
		_globals._sequenceIndexes[6] = _scene->_sequences.startCycle(_globals._spriteIndexes[6], false, 11);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[6], 5);
		_scene->_sequences.updateTimeout(_globals._sequenceIndexes[6], seqIdx);
		if (_roxMode == 22) {
			_scene->_kernelMessages.reset();
			_scene->_kernelMessages.add(Common::Point(230, 56), 0x1110, 32, 0, 120, _game.getQuote(0x23F));
			_bartenderMode = 21;
			_globals[kHasPurchased] = true;
			_scene->_sequences.addTimer(140, 95);
		} else if (_roxMode == 20)
			_game._player._stepEnabled = true;

		}
		break;
	}

	switch (_game._trigger) {
	case 95:
		_bartenderReady = false;
		_game._player._stepEnabled = false;
		if (_bartenderHandsHips || _bartenderTalking) {
			_scene->_sequences.remove(_globals._sequenceIndexes[10]);
			_bartenderHandsHips = false;
			_bartenderTalking = false;
		}

		if (_bartenderLooksLeft) {
			_scene->_sequences.remove(_globals._sequenceIndexes[11]);
			_bartenderLooksLeft = false;
		}

		if (_bartenderSteady) {
			_scene->_sequences.remove(_globals._sequenceIndexes[9]);
			_bartenderSteady = false;
		}
		_globals._sequenceIndexes[12] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[12], false, 10, 1, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[12], 1, 4);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[12], 8);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[12], SEQUENCE_TRIGGER_EXPIRE, 0, 96);
		break;

	case 96: {
		int seqIdx = _globals._sequenceIndexes[12];
		_globals._sequenceIndexes[12] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[12], false, 10, 1, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[12], 6, 7);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[12], 8);
		_scene->_sequences.updateTimeout(_globals._sequenceIndexes[12], seqIdx);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[12], SEQUENCE_TRIGGER_EXPIRE, 0, 97);
		}
		break;

	case 97: {
		int seqIdx = _globals._sequenceIndexes[12];
		switch (_bartenderMode) {
		case 20:
			_scene->_sequences.remove(_globals._sequenceIndexes[15]);
			break;

		case 21: {
			_globals._sequenceIndexes[15] = _scene->_sequences.startCycle(_globals._spriteIndexes[15], false, 1);
			_scene->_sequences.setDepth(_globals._sequenceIndexes[15], 8);
			_scene->_sequences.setMsgPosition(_globals._sequenceIndexes[15], Common::Point(250, 80));
			int idx = _scene->_dynamicHotspots.add(0x5C, 0x4, _globals._sequenceIndexes[15], Common::Rect(0, 0, 0, 0));
			_scene->_dynamicHotspots.setPosition(idx, Common::Point(0, 0), FACING_NONE);
			}
			break;

		case 22:
			_scene->_sequences.remove(_globals._sequenceIndexes[8]);
			_globals[kBottleDisplayed] = false;
			break;

		default:
			break;
		}

		_globals._sequenceIndexes[12] = _scene->_sequences.startCycle(_globals._spriteIndexes[12], false, 6);
		_scene->_sequences.updateTimeout(_globals._sequenceIndexes[12], seqIdx);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[12], 8);
		_scene->_sequences.addTimer(10, 98);
		}
		break;

	case 98:
		_scene->_sequences.remove(_globals._sequenceIndexes[12]);
		_globals._sequenceIndexes[12] = _scene->_sequences.addReverseSpriteCycle(_globals._spriteIndexes[12], false, 10, 1, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[12], 1, 4);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[12], 8);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[12], SEQUENCE_TRIGGER_EXPIRE, 0, 99);
		break;

	case 99: {
		int seqIdx = _globals._sequenceIndexes[12];
		_globals._sequenceIndexes[9] = _scene->_sequences.startCycle(_globals._spriteIndexes[9], false, 1);
		_bartenderSteady = true;
		int idx = _scene->_dynamicHotspots.add(0x30E, 0xD, _globals._sequenceIndexes[9], Common::Rect(0, 0, 0, 0));
		_scene->_dynamicHotspots.setPosition(idx, Common::Point(228, 83), FACING_SOUTH);
		_scene->_sequences.updateTimeout(_globals._sequenceIndexes[9], seqIdx);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[9], 8);
		if (_bartenderMode == 20) {
			_scene->_kernelMessages.reset();
			_scene->_kernelMessages.add(Common::Point(210, 41), 0xFDFC, 0, 0, 100, _game.getQuote(0x1F3));
			_scene->_sequences.addTimer(5, 100);
			_talkTimer = 180;
			_roxMode = 22;
			_scene->_sequences.addTimer(65, 92);
		} else if ((_bartenderMode == 21) || (_bartenderMode == 22)) {
			_game._player._stepEnabled = true;
			_bartenderReady = true;
		}

		}
		break;
	}

	switch (_game._trigger) {
	case 86:
		_bartenderReady = false;
		_game._player._stepEnabled = false;
		if ((_bartenderHandsHips) || (_bartenderTalking)) {
			_scene->_sequences.remove(_globals._sequenceIndexes[10]);
			_bartenderHandsHips = false;
			_bartenderTalking = false;
		} else if (_bartenderLooksLeft) {
			_scene->_sequences.remove(_globals._sequenceIndexes[11]);
			_bartenderLooksLeft = false;
		} else if (_bartenderSteady) {
			_scene->_sequences.remove(_globals._sequenceIndexes[9]);
			_bartenderSteady = false;
		}
		_globals._sequenceIndexes[9] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[9], false, 10, 1, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[9], 1, 9);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[9], 8);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[9], SEQUENCE_TRIGGER_EXPIRE, 0, 87);
		break;

	case 87: {
		int seqIdx = _globals._sequenceIndexes[9];
		_globals._sequenceIndexes[9] = _scene->_sequences.startCycle(_globals._spriteIndexes[9], false, 9);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[9], 8);
		_scene->_sequences.updateTimeout(_globals._sequenceIndexes[9], seqIdx);
		_scene->_sequences.addTimer(10, 89);

		_globals._sequenceIndexes[8] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[8], false, 10, 1, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[8], 1, 4);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[8], 7);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[8], SEQUENCE_TRIGGER_EXPIRE, 0, 88);
		}
		break;

	case 88:
		_globals._sequenceIndexes[8] = _scene->_sequences.startCycle(_globals._spriteIndexes[8], false, 4);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[8], 7);
		_globals[kBottleDisplayed] = true;
		break;

	case 89:
		_scene->_sequences.remove(_globals._sequenceIndexes[9]);
		_globals._sequenceIndexes[9] = _scene->_sequences.startCycle(_globals._spriteIndexes[9], false, 6);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[9], 8);
		_scene->_sequences.addTimer(10, 90);
		break;

	case 90:
		_scene->_sequences.remove(_globals._sequenceIndexes[9]);
		_globals._sequenceIndexes[9] = _scene->_sequences.startCycle(_globals._spriteIndexes[9], false, 5);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[9], 8);
		_scene->_sequences.addTimer(10, 91);
		break;

	case 91: {
		_scene->_sequences.remove(_globals._sequenceIndexes[9]);
		_globals._sequenceIndexes[9] = _scene->_sequences.startCycle(_globals._spriteIndexes[9], false, 1);
		int idx = _scene->_dynamicHotspots.add(0x30E, 0xD, _globals._sequenceIndexes[9], Common::Rect(0, 0, 0, 0));
		_scene->_dynamicHotspots.setPosition(idx, Common::Point(228, 83), FACING_SOUTH);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[9], 8);
		_bartenderSteady = true;
		_game._player._stepEnabled = true;
		_bartenderReady = true;
		}
		break;
	}

	if (!_waitingGinnyMove && !_ginnyLooking) {
		_waitingGinnyMove = true;
		++ _beatCounter;
		if (_beatCounter >= 20) {
			_ginnyLooking = true;
			_beatCounter = 0;
			_scene->_sequences.addTimer(60, 54);
		} else {
			_scene->_sequences.addTimer(30, 75);
		}
	}

	switch (_game._trigger) {
	case 75:
		_scene->_sequences.remove(_globals._sequenceIndexes[5]);
		_bigBeatFl = !_bigBeatFl;

		if (_bigBeatFl) {
			_globals._sequenceIndexes[5] = _scene->_sequences.startCycle(_globals._spriteIndexes[5], false, 5);
			_scene->_sequences.addTimer(8, 130);
		} else {
			_globals._sequenceIndexes[5] = _scene->_sequences.startCycle(_globals._spriteIndexes[5], false, 5);
			_scene->_sequences.addTimer(8, 53);
		}

		_scene->_sequences.setDepth(_globals._sequenceIndexes[5], 1);
		break;

	case 130:
		_scene->_sequences.remove(_globals._sequenceIndexes[5]);
		_globals._sequenceIndexes[5] = _scene->_sequences.startCycle(_globals._spriteIndexes[5], false, 6);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[5], 1);
		_scene->_sequences.addTimer(8, 131);
		break;

	case 131:
		_scene->_sequences.remove(_globals._sequenceIndexes[5]);
		_globals._sequenceIndexes[5] = _scene->_sequences.startCycle(_globals._spriteIndexes[5], false, 5);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[5], 1);
		_scene->_sequences.addTimer(8, 53);
		break;

	case 53:
		if (_bigBeatFl) {
			_scene->_sequences.remove(_globals._sequenceIndexes[5]);
			_globals._sequenceIndexes[5] = _scene->_sequences.startCycle(_globals._spriteIndexes[5], false, 1);
		} else {
			_scene->_sequences.remove(_globals._sequenceIndexes[5]);
			_globals._sequenceIndexes[5] = _scene->_sequences.startCycle(_globals._spriteIndexes[5], false, 1);
		}
		_scene->_sequences.setDepth(_globals._sequenceIndexes[5], 1);
		_waitingGinnyMove = false;
		break;
	}

	if ((_game._trigger == 54) && _ginnyLooking) {
		++_beatCounter;
		if (_beatCounter >= 10) {
			_ginnyLooking = false;
			_waitingGinnyMove = false;
			_beatCounter = 0;
			_bigBeatFl = true;
		} else {
			_scene->_sequences.remove(_globals._sequenceIndexes[5]);
			_globals._sequenceIndexes[5] = _scene->_sequences.startCycle(_globals._spriteIndexes[5], false, _vm->getRandomNumber(1, 4));
			_scene->_sequences.setDepth(_globals._sequenceIndexes[5], 1);
			_scene->_sequences.addTimer(60, 54);
		}
	}

	if (_bartenderReady) {
		if (_vm->getRandomNumber(1, 250) == 1) {
			if (_bartenderLooksLeft) {
				_scene->_sequences.remove(_globals._sequenceIndexes[11]);
				_bartenderLooksLeft = false;
			} else if (_bartenderHandsHips) {
				_scene->_sequences.remove(_globals._sequenceIndexes[10]);
				_bartenderHandsHips = false;
			} else if (_bartenderSteady) {
				_scene->_sequences.remove(_globals._sequenceIndexes[9]);
				_bartenderSteady = false;
			}

			switch (_vm->getRandomNumber(1, 3)) {
			case 1: {
				_globals._sequenceIndexes[10] = _scene->_sequences.startCycle(_globals._spriteIndexes[10], false, 4);
				_scene->_sequences.setDepth(_globals._sequenceIndexes[10], 8);
				int idx = _scene->_dynamicHotspots.add(0x30E, 0xD, _globals._sequenceIndexes[10], Common::Rect(0, 0, 0, 0));
				_scene->_dynamicHotspots.setPosition(idx, Common::Point(228, 83), FACING_SOUTH);
				_bartenderHandsHips = true;
				}
				break;

			case 2: {
				_globals._sequenceIndexes[11] = _scene->_sequences.startCycle(_globals._spriteIndexes[11], false, 2);
				_scene->_sequences.setDepth(_globals._sequenceIndexes[11], 8);
				int idx = _scene->_dynamicHotspots.add(0x30E, 0xD, _globals._sequenceIndexes[11], Common::Rect(0, 0, 0, 0));
				_scene->_dynamicHotspots.setPosition(idx, Common::Point(228, 83), FACING_SOUTH);
				_bartenderLooksLeft = true;
				}
				break;

			case 3: {
				_globals._sequenceIndexes[9] = _scene->_sequences.startCycle(_globals._spriteIndexes[9], false, 1);
				_scene->_sequences.setDepth(_globals._sequenceIndexes[9], 8);
				int idx = _scene->_dynamicHotspots.add(0x30E, 0xD, _globals._sequenceIndexes[9], Common::Rect(0, 0, 0, 0));
				_scene->_dynamicHotspots.setPosition(idx, Common::Point(228, 83), FACING_SOUTH);
				_bartenderSteady = true;
				}
				break;

			default:
				break;
			}
		}
	}

	if (_game._trigger == 76) {
		int seqIdx = _globals._sequenceIndexes[6];
		_globals._sequenceIndexes[6] = _scene->_sequences.startCycle(_globals._spriteIndexes[6], false, 11);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[6], 5);
		_scene->_sequences.updateTimeout(_globals._sequenceIndexes[6], seqIdx);
		if (!_globals[kBeenThruHelgaScene]) {
			_game._player._stepEnabled = false;
			_cutSceneNeeded = true;
		} else {
			_game._player._stepEnabled = true;
		}
		_roxOnStool = true;
	}

	switch (_game._trigger) {
	case 77:
		_game._player._stepEnabled = false;
		_scene->_sequences.remove(_globals._sequenceIndexes[6]);
		_globals._sequenceIndexes[7] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[7], false, 10, 1, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[7], 10, 12);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[7], 5);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[7], SEQUENCE_TRIGGER_EXPIRE, 0, 78);
		break;

	case 78: {
		_vm->_sound->command(57);
		int seqIdx = _globals._sequenceIndexes[7];
		_game._objects.addToInventory(OBJ_REPAIR_LIST);
		_scene->_sequences.remove(_globals._sequenceIndexes[14]);
		_globals._sequenceIndexes[7] = _scene->_sequences.addReverseSpriteCycle(_globals._spriteIndexes[7], false, 10, 1, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[7], 10, 12);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[7], 5);
		_scene->_sequences.updateTimeout(_globals._sequenceIndexes[7], seqIdx);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[7], SEQUENCE_TRIGGER_EXPIRE, 0, 79);
		}
		break;

	case 79: {
		int seqIdx = _globals._sequenceIndexes[7];
		_globals._sequenceIndexes[6] = _scene->_sequences.startCycle(_globals._spriteIndexes[6], false, 11);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[6], 5);
		_scene->_sequences.updateTimeout(_globals._sequenceIndexes[6], seqIdx);
		_scene->_sequences.addTimer(20, 180);
		}
		break;

	case 180:
		_vm->_dialogs->showItem(OBJ_REPAIR_LIST, 40240);
		_game._player._stepEnabled = true;
		break;

	default:
		break;
	}

	if (_cutSceneNeeded && _cutSceneReady) {
		_cutSceneNeeded = false;
		_scene->_sequences.addTimer(20, 55);
		_helgaReady = false;
		_bartenderReady = false;
	}

	if (_vm->getRandomNumber(1, 25) == 1) {
		if (_lightOn) {
			_scene->_sequences.remove(_globals._sequenceIndexes[0]);
			_lightOn = false;
		} else {
			_globals._sequenceIndexes[0] = _scene->_sequences.startCycle(_globals._spriteIndexes[0], false, 1);
			_lightOn = true;
		}
	}

	if (!_blowingSmoke) {
		if (_vm->getRandomNumber(1, 300) == 1) {
			_globals._sequenceIndexes[1] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[1], false, 8, 1, 0, 0);
			_scene->_sequences.setAnimRange(_globals._sequenceIndexes[1], 1, 14);
			_scene->_sequences.setDepth(_globals._sequenceIndexes[1], 14);
			_scene->_sequences.addSubEntry(_globals._sequenceIndexes[1], SEQUENCE_TRIGGER_EXPIRE, 0, 30);
			_blowingSmoke = true;
		}
	}

	switch (_game._trigger) {
	case 30:
		_globals._sequenceIndexes[1] = _scene->_sequences.addReverseSpriteCycle(_globals._spriteIndexes[1], false, 8, 1, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[1], 1, 5);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[1], 14);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[1],SEQUENCE_TRIGGER_EXPIRE, 0, 31);
		break;

	case 31:
		_blowingSmoke = false;
		break;
	}

	if (!_leftWomanMoving) {
		if (_vm->getRandomNumber(1, 1000) == 1) {
			switch (_vm->getRandomNumber(1, 2)) {
			case 1:
				_globals._sequenceIndexes[2] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[2], false, 12, 1, 0, 0);
				_scene->_sequences.setAnimRange(_globals._sequenceIndexes[2], 1, 8);
				_scene->_sequences.addSubEntry(_globals._sequenceIndexes[2], SEQUENCE_TRIGGER_EXPIRE, 0, 33);
				_leftWomanMoving = true;
				break;

			case 2:
				_globals._sequenceIndexes[2] = _scene->_sequences.startCycle(_globals._spriteIndexes[2], false, 1);
				_scene->_sequences.addTimer(12, 35);
				_leftWomanMoving = true;
				break;

			default:
				break;
			}
		}
	}

	switch (_game._trigger) {
	case 33: {
		int seqIdx = _globals._sequenceIndexes[2];
		_globals._sequenceIndexes[2] = _scene->_sequences.startCycle(_globals._spriteIndexes[2], false, 9);
		_scene->_sequences.updateTimeout(_globals._sequenceIndexes[2], seqIdx);
		_scene->_sequences.addTimer(_vm->getRandomNumber(60, 250), 34);
		}
		break;

	case 34:
		_scene->_sequences.remove(_globals._sequenceIndexes[2]);
		_globals._sequenceIndexes[2] = _scene->_sequences.addReverseSpriteCycle(_globals._spriteIndexes[2], false, 12, 1, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[2], 1, 8);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[2], SEQUENCE_TRIGGER_EXPIRE, 0, 38);
		break;

	case 35:
		_scene->_sequences.remove(_globals._sequenceIndexes[2]);
		_globals._sequenceIndexes[2] = _scene->_sequences.startCycle(_globals._spriteIndexes[2], false, 2);
		_scene->_sequences.addTimer(_vm->getRandomNumber(60, 300), 36);
		break;

	case 36:
		_scene->_sequences.remove(_globals._sequenceIndexes[2]);
		_globals._sequenceIndexes[2] = _scene->_sequences.startCycle(_globals._spriteIndexes[2], false, 1);
		_scene->_sequences.addTimer(12, 37);
		break;

	case 37:
		_scene->_sequences.remove(_globals._sequenceIndexes[2]);
		_leftWomanMoving = false;
		break;

	case 38:
		_leftWomanMoving = false;
		break;

	default:
		break;
	}

	if (!_rightWomanMoving) {
		if (_vm->getRandomNumber(1, 300) == 1) {
			switch (_vm->getRandomNumber(1, 4)) {
			case 1:
				_globals._sequenceIndexes[3] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[3], false, 12, 1, 0, 0);
				_scene->_sequences.setAnimRange(_globals._sequenceIndexes[3], 1, 4);
				_scene->_sequences.addSubEntry(_globals._sequenceIndexes[3], SEQUENCE_TRIGGER_EXPIRE, 0, 32);
				_rightWomanMoving = true;
				break;

			case 2:
				_globals._sequenceIndexes[3] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[3], false, 12, 2, 0, 0);
				_scene->_sequences.setAnimRange(_globals._sequenceIndexes[3], 4, 5);
				_scene->_sequences.addSubEntry(_globals._sequenceIndexes[3], SEQUENCE_TRIGGER_EXPIRE, 0, 32);
				_rightWomanMoving = true;
				break;

			case 3:
				_globals._sequenceIndexes[3] = _scene->_sequences.startReverseCycle(_globals._spriteIndexes[3], false, 12, 2, 0, 0);
				_scene->_sequences.setAnimRange(_globals._sequenceIndexes[3], 4, 5);
				_scene->_sequences.addSubEntry(_globals._sequenceIndexes[3], SEQUENCE_TRIGGER_EXPIRE, 0, 32);
				_rightWomanMoving = true;
				break;

			case 4:
				_globals._sequenceIndexes[3] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[3], false, 12, 2, 0, 0);
				_scene->_sequences.setAnimRange(_globals._sequenceIndexes[3], 3, 4);
				_scene->_sequences.addSubEntry(_globals._sequenceIndexes[3], SEQUENCE_TRIGGER_EXPIRE, 0, 32);
				_rightWomanMoving = true;
				break;

			default:
				break;
			}
		}
	}

	if (_game._trigger == 32)
		_rightWomanMoving = false;

	if (_scene->_activeAnimation->getCurrentFrame() == 1) {
		switch (_vm->getRandomNumber(1, 50)) {
		case 1:
			_scene->_activeAnimation->setCurrentFrame(2);
			break;

		case 2:
			_scene->_activeAnimation->setCurrentFrame(7);
			break;

		case 3:
			_scene->_activeAnimation->setCurrentFrame(11);
			break;

		default:
			_scene->_activeAnimation->setCurrentFrame(0);
			break;
		}
	}

	if ((_scene->_activeAnimation->getCurrentFrame() == 4) && (_drinkTimer < 10)) {
		++ _drinkTimer;
		_scene->_activeAnimation->setCurrentFrame(3);
	}

	if (_drinkTimer == 10) {
		_drinkTimer = 0;
		_scene->_activeAnimation->setCurrentFrame(4);
		_scene->_activeAnimation->_currentFrame = 5;
	}


	switch (_scene->_activeAnimation->getCurrentFrame()) {
	case 6:
	case 10:
	case 14:
		_scene->_activeAnimation->setCurrentFrame(0);
		break;

	default:
		break;
	}

	switch (_game._trigger) {
	case 39:
		_scene->_kernelMessages.reset();
		_scene->_kernelMessages.add(Common::Point(89, 67), 0xFDFC, 32, 0, 120, _game.getQuote(0x1D8));
		_scene->_sequences.addTimer(150, 40);
		break;

	case 40:
		_scene->_kernelMessages.add(Common::Point(89, 67), 0xFDFC, 32, 0, 120, _game.getQuote(0x1D9));
		_scene->_sequences.addTimer(150, 41);
		break;

	case 41:
		_scene->_kernelMessages.add(Common::Point(89, 67), 0xFDFC, 32, 0, 120, _game.getQuote(0x1DA));
		_game._player._stepEnabled = true;
		break;

	case 42:
		_scene->_kernelMessages.reset();
		_scene->_kernelMessages.add(Common::Point(89, 67), 0xFDFC, 32, 0, 120, _game.getQuote(0x1DC));
		_scene->_sequences.addTimer(150, 43);
		break;

	case 43:
		_scene->_kernelMessages.add(Common::Point(89, 67), 0xFDFC, 32, 0, 120, _game.getQuote(0x1DD));
		_game._player._stepEnabled = true;
		break;

	case 44:
		_game._player._stepEnabled = true;
		break;

	default:
		break;
	}

	switch (_game._trigger) {
	case 55:
		if (_bartenderHandsHips) {
			_scene->_sequences.remove(_globals._sequenceIndexes[10]);
			_bartenderHandsHips = false;
			_globals._sequenceIndexes[9] = _scene->_sequences.startCycle(_globals._spriteIndexes[9], false, 1);
			int idx = _scene->_dynamicHotspots.add(0x30E, 0xD, _globals._sequenceIndexes[9], Common::Rect(0, 0, 0, 0));
			_scene->_dynamicHotspots.setPosition(idx, Common::Point(228, 83), FACING_SOUTH);
			_bartenderSteady = true;
			_scene->_sequences.setDepth(_globals._sequenceIndexes[9], 8);
		} else if (_bartenderLooksLeft) {
			_scene->_sequences.remove(_globals._sequenceIndexes[11]);
			_bartenderLooksLeft = false;
			_globals._sequenceIndexes[9] = _scene->_sequences.startCycle(_globals._spriteIndexes[9], false, 1);
			int idx = _scene->_dynamicHotspots.add(0x30E, 0xD, _globals._sequenceIndexes[9], Common::Rect(0, 0, 0, 0));
			_scene->_dynamicHotspots.setPosition(idx, Common::Point(228, 83), FACING_SOUTH);
			_bartenderSteady = true;
			_scene->_sequences.setDepth(_globals._sequenceIndexes[9], 8);
		}
		_game._player._stepEnabled = false;
		_scene->_kernelMessages.reset();
		_scene->_kernelMessages.add(Common::Point(180, 47), 0xFBFA, 0, 0, 100, _game.getQuote(0x1FE));
		_scene->_sequences.addTimer(120, 56);
		break;

	case 56:
		_scene->_sequences.remove(_globals._sequenceIndexes[13]);
		_globals._sequenceIndexes[13] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[13], false, 10, 1, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[13], 2, 5);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[13], 8);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[13], SEQUENCE_TRIGGER_EXPIRE, 0, 57);
		break;

	case 57: {
		int seqIdx = _globals._sequenceIndexes[13];
		_scene->_sequences.remove(_globals._sequenceIndexes[20]);
		_globals._sequenceIndexes[13] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[13], false, 10, 1, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[13], 6, 9);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[13], 8);
		_scene->_sequences.updateTimeout(_globals._sequenceIndexes[13], seqIdx);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[13], SEQUENCE_TRIGGER_EXPIRE, 0, 58);
		}
		break;

	case 58: {
		int seqIdx = _globals._sequenceIndexes[13];
		_globals._sequenceIndexes[13] = _scene->_sequences.addReverseSpriteCycle(_globals._spriteIndexes[13], false, 10, 1, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[13], 5, 9);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[13], 8);
		_scene->_sequences.updateTimeout(_globals._sequenceIndexes[13], seqIdx);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[13], SEQUENCE_TRIGGER_EXPIRE, 0, 59);
		}
		break;

	case 59: {
		int seqIdx = _globals._sequenceIndexes[13];
		_globals._sequenceIndexes[13] = _scene->_sequences.addReverseSpriteCycle(_globals._spriteIndexes[13], false, 10, 1, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[13], 1, 4);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[13], 8);
		_scene->_sequences.updateTimeout(_globals._sequenceIndexes[13], seqIdx);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[13], SEQUENCE_TRIGGER_EXPIRE, 0, 60);
		_globals._sequenceIndexes[20] = _scene->_sequences.startCycle(_globals._spriteIndexes[20], false, 1);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[20], 8);
		_scene->_sequences.setMsgPosition(_globals._sequenceIndexes[20], Common::Point(234, 72));
		}
		break;

	case 60: {
		int seqIdx = _globals._sequenceIndexes[13];
		_globals._sequenceIndexes[13] = _scene->_sequences.startCycle(_globals._spriteIndexes[13], false, 2);
		_scene->_sequences.updateTimeout(_globals._sequenceIndexes[13], seqIdx);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[13], 8);
		_scene->_sequences.addTimer(10, 61);
		}
		break;

	case 61:
		_scene->_kernelMessages.reset();
		_scene->_kernelMessages.add(Common::Point(181, 33), 0xFBFA, 0, 0, 130, _game.getQuote(0x1FF));
		_scene->_kernelMessages.add(Common::Point(171, 47), 0xFBFA, 0, 0, 130, _game.getQuote(0x200));
		_scene->_sequences.addTimer(150, 63);
		_scene->_sequences.remove(_globals._sequenceIndexes[13]);
		_globals._sequenceIndexes[13] = _scene->_sequences.startReverseCycle(_globals._spriteIndexes[13], false, 30, 4, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[13], 10, 11);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[13], 8);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[13], SEQUENCE_TRIGGER_EXPIRE, 0, 62);
		break;

	case 62: {
		int seqIdx = _globals._sequenceIndexes[13];
		_globals._sequenceIndexes[13] = _scene->_sequences.startCycle(_globals._spriteIndexes[13], false, 2);
		_scene->_sequences.updateTimeout(_globals._sequenceIndexes[13], seqIdx);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[13], 8);
		}
		break;

	case 63:
		_scene->_kernelMessages.reset();
		_scene->_kernelMessages.add(Common::Point(160, 33), 0xFBFA, 0, 0, 130, _game.getQuote(0x201));
		_scene->_kernelMessages.add(Common::Point(165, 47), 0xFBFA, 0, 0, 130, _game.getQuote(0x202));
		_scene->_sequences.addTimer(150, 64);
		break;

	case 64:
		_scene->_kernelMessages.reset();
		_scene->_kernelMessages.add(Common::Point(210, 27), 0xFDFC, 0, 0, 130, _game.getQuote(0x1E0));
		_scene->_kernelMessages.add(Common::Point(198, 41), 0xFDFC, 0, 0, 130, _game.getQuote(0x1E1));
		_scene->_sequences.addTimer(150, 65);
		_scene->_sequences.addTimer(1, 100);
		_talkTimer = 1130;
		break;

	case 65:
		_vm->_sound->command(30);
		_globals._sequenceIndexes[16] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[16], false, 10, 1, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[16], 1, 6);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[16], 9);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[16], SEQUENCE_TRIGGER_EXPIRE, 0, 66);
		break;

	case 66: {
		int seqIdx = _globals._sequenceIndexes[16];
		_globals._sequenceIndexes[16] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[16], false, 10, 1, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[16], 7, 37);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[16], 9);
		_scene->_sequences.updateTimeout(_globals._sequenceIndexes[16], seqIdx);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[16], SEQUENCE_TRIGGER_EXPIRE, 0, 68);

		_scene->_sequences.remove(_globals._sequenceIndexes[13]);
		_globals._sequenceIndexes[13] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[13], false, 10, 1, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[13], 12, 13);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[13], 8);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[13], SEQUENCE_TRIGGER_EXPIRE, 0, 67);
		}
		break;

	case 67: {
		int seqIdx = _globals._sequenceIndexes[13];
		_globals._sequenceIndexes[13] = _scene->_sequences.startCycle(_globals._spriteIndexes[13], false, 13);
		_scene->_sequences.updateTimeout(_globals._sequenceIndexes[13], seqIdx);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[13], 8);
		}
		break;

	case 68:
		_scene->_sequences.remove(_globals._sequenceIndexes[13]);
		_globals._sequenceIndexes[13] = _scene->_sequences.addReverseSpriteCycle(_globals._spriteIndexes[13], false, 10, 1, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[13], 12, 13);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[13], 8);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[13], SEQUENCE_TRIGGER_EXPIRE, 0, 69);
		break;

	case 69: {
		int seqIdx = _globals._sequenceIndexes[13];
		_globals._sequenceIndexes[13] = _scene->_sequences.startReverseCycle(_globals._spriteIndexes[13], false, 25, 4, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[13], 10, 11);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[13], 8);
		_scene->_sequences.updateTimeout(_globals._sequenceIndexes[13], seqIdx);

		_scene->_kernelMessages.reset();
		_scene->_kernelMessages.add(Common::Point(179, 33), 0xFBFA, 0, 0, 130, _game.getQuote(0x203));
		_scene->_kernelMessages.add(Common::Point(167, 47), 0xFBFA, 0, 0, 130, _game.getQuote(0x204));
		_scene->_sequences.addTimer(150, 71);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[13], SEQUENCE_TRIGGER_EXPIRE, 0, 70);
		}
		break;

	case 70:
		_globals._sequenceIndexes[13] = _scene->_sequences.startReverseCycle(_globals._spriteIndexes[13], false, 25, 4, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[13], 10, 12);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[13], 8);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[13], SEQUENCE_TRIGGER_EXPIRE, 0, 72);
		break;

	case 71:
		_scene->_sequences.addTimer(210, 73);
		_scene->_kernelMessages.reset();
		_scene->_kernelMessages.add(Common::Point(168, 33), 0xFBFA, 0, 0, 180, _game.getQuote(0x205));
		_scene->_kernelMessages.add(Common::Point(151, 47), 0xFBFA, 0, 0, 180, _game.getQuote(0x206));
		if (!_game._objects.isInInventory(OBJ_REPAIR_LIST))
			_activeArrows = true;
		break;

	case 72: {
		int seqIdx = _globals._sequenceIndexes[13];
		_globals._sequenceIndexes[13] = _scene->_sequences.startCycle(_globals._spriteIndexes[13], false, 2);
		_scene->_sequences.updateTimeout(_globals._sequenceIndexes[13], seqIdx);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[13], 8);
		}
		break;

	case 73:
		_scene->_kernelMessages.reset();
		_scene->_kernelMessages.add(Common::Point(177, 33), 0xFBFA, 0, 0, 150, _game.getQuote(0x207));
		_scene->_kernelMessages.add(Common::Point(172, 47), 0xFBFA, 0, 0, 150, _game.getQuote(0x208));
		_bartenderSteady = true;
		_game._player._stepEnabled = true;
		_helgaReady = true;
		_bartenderReady = true;
		_globals[kBeenThruHelgaScene] = true;
		_activeArrows = false;
		break;

	default:
		break;
	}

	if (_helgaReady) {
		int rndVal = _vm->getRandomNumber(1, 1000);
		if (rndVal < 6)
			switch (rndVal) {
			case 1:
				_cutSceneReady = false;
				_helgaReady = false;
				_scene->_sequences.remove(_globals._sequenceIndexes[13]);
				_globals._sequenceIndexes[13] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[13], false, 10, 1, 0, 0);
				_scene->_sequences.setAnimRange(_globals._sequenceIndexes[13], 2, 5);
				_scene->_sequences.setDepth(_globals._sequenceIndexes[13], 8);
				_scene->_sequences.addSubEntry(_globals._sequenceIndexes[13], SEQUENCE_TRIGGER_EXPIRE, 0, 82);
				break;

			case 2:
				_cutSceneReady = false;
				_helgaReady = false;
				_scene->_sequences.remove(_globals._sequenceIndexes[13]);
				_globals._sequenceIndexes[13] = _scene->_sequences.startReverseCycle(_globals._spriteIndexes[13], false, 15, 2, 0, 0);
				_scene->_sequences.setAnimRange(_globals._sequenceIndexes[13], 11, 13);
				_scene->_sequences.setDepth(_globals._sequenceIndexes[13], 8);
				_scene->_sequences.addSubEntry(_globals._sequenceIndexes[13], SEQUENCE_TRIGGER_EXPIRE, 0, 26);
				break;

			case 3:
				_cutSceneReady = false;
				_helgaReady = false;
				_scene->_sequences.remove(_globals._sequenceIndexes[13]);
				_globals._sequenceIndexes[13] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[13], false, 10, 1, 0, 0);
				_scene->_sequences.setAnimRange(_globals._sequenceIndexes[13], 10, 11);
				_scene->_sequences.setDepth(_globals._sequenceIndexes[13], 8);
				_scene->_sequences.addSubEntry(_globals._sequenceIndexes[13], SEQUENCE_TRIGGER_EXPIRE, 0, 26);
				break;

			case 4:
				_cutSceneReady = false;
				_helgaReady = false;
				_scene->_sequences.remove(_globals._sequenceIndexes[13]);
				_globals._sequenceIndexes[13] = _scene->_sequences.startReverseCycle(_globals._spriteIndexes[13], false, 15, 2, 0, 0);
				_scene->_sequences.setAnimRange(_globals._sequenceIndexes[13], 14, 15);
				_scene->_sequences.setDepth(_globals._sequenceIndexes[13], 8);
				_scene->_sequences.addSubEntry(_globals._sequenceIndexes[13], SEQUENCE_TRIGGER_EXPIRE, 0, 26);
				break;

			case 5:
				_cutSceneReady = false;
				_helgaReady = false;
				_scene->_sequences.remove(_globals._sequenceIndexes[13]);
				_globals._sequenceIndexes[13] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[13], false, 10, 1, 0, 0);
				_scene->_sequences.setAnimRange(_globals._sequenceIndexes[13], 16, 19);
				_scene->_sequences.setDepth(_globals._sequenceIndexes[13], 8);
				_scene->_sequences.addSubEntry(_globals._sequenceIndexes[13], SEQUENCE_TRIGGER_EXPIRE, 0, 80);
				break;

			default:
				break;
			}
		}

	if (_game._trigger == 80) {
		int seqIdx = _globals._sequenceIndexes[13];
		_globals._sequenceIndexes[13] = _scene->_sequences.startCycle(_globals._spriteIndexes[13], false, 19);
		_scene->_sequences.updateTimeout(_globals._sequenceIndexes[13], seqIdx);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[13], 8);
		_scene->_sequences.addTimer(_vm->getRandomNumber(60, 120), 81);
	}

	if (_game._trigger == 81) {
		_scene->_sequences.remove(_globals._sequenceIndexes[13]);
		_globals._sequenceIndexes[13] = _scene->_sequences.addReverseSpriteCycle(_globals._spriteIndexes[13], false, 10, 1, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[13], 16, 19);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[13], 8);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[13], SEQUENCE_TRIGGER_EXPIRE, 0, 26);
	}

	if (_game._trigger == 26) {
		int seqIdx = _globals._sequenceIndexes[13];
		_globals._sequenceIndexes[13] = _scene->_sequences.startCycle(_globals._spriteIndexes[13], false, 2);
		_scene->_sequences.updateTimeout(_globals._sequenceIndexes[13], seqIdx);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[13], 8);
		_cutSceneReady = true;

		if (!_cutSceneNeeded)
			_helgaReady = true;
	}

	if (_game._trigger == 82) {
		_scene->_sequences.remove(_globals._sequenceIndexes[20]);
		int seqIdx = _globals._sequenceIndexes[13];
		_globals._sequenceIndexes[13] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[13], false, 10, 1, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[13], 6, 9);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[13], 8);
		_scene->_sequences.updateTimeout(_globals._sequenceIndexes[13], seqIdx);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[13], SEQUENCE_TRIGGER_EXPIRE, 0, 83);
	}

	if (_game._trigger == 83) {
		int seqIdx = _globals._sequenceIndexes[13];
		_globals._sequenceIndexes[13] = _scene->_sequences.addReverseSpriteCycle(_globals._spriteIndexes[13], false, 10, 1, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[13], 5, 9);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[13], 8);
		_scene->_sequences.updateTimeout(_globals._sequenceIndexes[13], seqIdx);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[13], SEQUENCE_TRIGGER_EXPIRE, 0, 84);
	}

	if (_game._trigger == 84) {
		int seqIdx = _globals._sequenceIndexes[13];
		_globals._sequenceIndexes[13] = _scene->_sequences.addReverseSpriteCycle(_globals._spriteIndexes[13], false, 10, 1, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[13], 1, 4);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[13], 8);
		_scene->_sequences.updateTimeout(_globals._sequenceIndexes[13], seqIdx);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[13], SEQUENCE_TRIGGER_EXPIRE, 0, 85);

		_globals._sequenceIndexes[20] = _scene->_sequences.startCycle(_globals._spriteIndexes[20], false, 1);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[20], 8);
		_scene->_sequences.setMsgPosition(_globals._sequenceIndexes[20], Common::Point(234, 72));
	}

	if (_game._trigger == 85) {
		int seqIdx = _globals._sequenceIndexes[13];
		_globals._sequenceIndexes[13] = _scene->_sequences.startCycle(_globals._spriteIndexes[13], false, 2);
		_scene->_sequences.updateTimeout(_globals._sequenceIndexes[13], seqIdx);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[13], 8);
		_cutSceneReady = true;
		if (!_cutSceneNeeded)
			_helgaReady = true;
	}

	if (_game._trigger == 102) {
		_scene->_sequences.remove(_globals._sequenceIndexes[6]);
		_game._player._priorTimer = _scene->_frameStartTime - _game._player._ticksAmount;
		_globals._sequenceIndexes[7] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[7], false, 8, 1, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[7], 14, 18);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[7], 5);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[7], SEQUENCE_TRIGGER_EXPIRE, 0, 103);
	} else if (_game._trigger == 103) {
		_game._player._priorTimer = _scene->_frameStartTime + _game._player._ticksAmount;
		_roxOnStool = false;
		_game._player._facing = FACING_SOUTH;
		_game._player.selectSeries();
		_game._player._visible = true;
		_game._player._stepEnabled = true;
		_game._player._readyToWalk = true;
	}

	if (_activeArrows) {
		if (!_activeArrow1) {
			_globals._sequenceIndexes[17] = _scene->_sequences.startCycle(_globals._spriteIndexes[17], false, 1);
			_scene->_sequences.setDepth(_globals._sequenceIndexes[17], 1);
			_scene->_sequences.addTimer(_vm->getRandomNumber(10, 15), 49);
			_activeArrow1 = true;
		}

		if (!_activeArrow2) {
			_globals._sequenceIndexes[18] = _scene->_sequences.startCycle(_globals._spriteIndexes[18], false, 1);
			_scene->_sequences.setDepth(_globals._sequenceIndexes[18], 1);
			_scene->_sequences.addTimer(_vm->getRandomNumber(10, 15), 50);
			_activeArrow2 = true;
		}

		if (!_activeArrow3) {
			_globals._sequenceIndexes[19] = _scene->_sequences.startCycle(_globals._spriteIndexes[19], false, 1);
			_scene->_sequences.setDepth(_globals._sequenceIndexes[19], 1);
			_scene->_sequences.addTimer(_vm->getRandomNumber(10, 15), 51);
			_activeArrow3 = true;
		}
	}

	if (_game._trigger == 49) {
		_scene->_sequences.remove(_globals._sequenceIndexes[17]);
		_scene->_sequences.addTimer(_vm->getRandomNumber(10, 15), 45);
	}

	if (_game._trigger == 45)
		_activeArrow1 = false;

	if (_game._trigger == 50) {
		_scene->_sequences.remove(_globals._sequenceIndexes[18]);
		_scene->_sequences.addTimer(_vm->getRandomNumber(10, 15), 46);
	}

	if (_game._trigger == 46)
		_activeArrow2 = false;

	if (_game._trigger == 51) {
		_scene->_sequences.remove(_globals._sequenceIndexes[19]);
		_scene->_sequences.addTimer(_vm->getRandomNumber(10, 15), 47);
	}

	if (_game._trigger == 47)
		_activeArrow3 = false;
}

void Scene402::preActions() {
	if (_action.isAction(0x30B, 0x24A) && (_game._player._prepareWalkPos.x != 248))
		_game._player.walk(Common::Point(232, 112), FACING_EAST);

	if (_action.isAction(VERB_WALKTO, 0x309))
		_game._player._needToWalk = _game._player._readyToWalk;

	if (!_roxOnStool && _action.isAction(VERB_TAKE, 0x5C) && !_game._objects.isInInventory(OBJ_CREDIT_CHIP))
		_game._player.walk(Common::Point(246, 108), FACING_NORTH);

	if (_action.isAction(VERB_TAKE))
		_game._player._needToWalk = false;

	if (_action.isAction(VERB_TAKE, 0x5C) && !_roxOnStool)
		_game._player._needToWalk = true;

	if (_roxOnStool) {
		if (_action.isAction(VERB_LOOK) || _action.isAction(0x24A) || _action.isAction(VERB_TALKTO))
			_game._player._needToWalk = false;

		if (_action.isAction(VERB_TAKE, 0x124) || _action.isAction(VERB_TAKE, 0x5C))
			_game._player._needToWalk = false;

		if (_action.isAction(VERB_TALKTO, 0x30A) || _action.isAction(VERB_TALKTO, 0x30C))
			_game._player._needToWalk = _game._player._readyToWalk;

		if (_game._player._needToWalk) {
			_game._player._facing = FACING_SOUTH;
			_game._player._readyToWalk = false;
			_game._player._stepEnabled = false;
			_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
			_scene->_sequences.addTimer(1, 102);
		}
	}

	if (_action.isAction(VERB_TAKE, 0x124) && !_roxOnStool && !_game._objects.isInInventory(OBJ_REPAIR_LIST))
		_game._player.walk(Common::Point(191, 99), FACING_NORTHEAST);

	if (_action.isAction(VERB_TALKTO, 0x30E) && !_roxOnStool)
		_game._player.walk(Common::Point(228, 83), FACING_SOUTH);

	if (_action.isAction(VERB_TALKTO, 0x3AA) && !_roxOnStool)
		_game._player.walk(Common::Point(208, 102), FACING_NORTHEAST);
}

void Scene402::actions() {
	if (_action.isAction(VERB_TAKE, 0x124) && _game._objects.isInRoom(OBJ_REPAIR_LIST) && _roxOnStool) {
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_scene->_sequences.addTimer(1, 77);
		_game._player._needToWalk = false;
	} else if (_action.isAction(VERB_TAKE, 0x124) && _game._objects.isInRoom(OBJ_REPAIR_LIST) && !_roxOnStool) {
		if (_game._trigger == 0) {
			_game._player._stepEnabled = false;
			_game._player._visible = false;
			_globals._sequenceIndexes[21] = _scene->_sequences.startReverseCycle(_globals._spriteIndexes[21], false, 7, 2, 0, 0);
			_scene->_sequences.setAnimRange(_globals._sequenceIndexes[21], 1, 2);
			_scene->_sequences.setMsgLayout(_globals._sequenceIndexes[21]);
			_scene->_sequences.addSubEntry(_globals._sequenceIndexes[21], SEQUENCE_TRIGGER_SPRITE, 2, 165);
			_scene->_sequences.addSubEntry(_globals._sequenceIndexes[21], SEQUENCE_TRIGGER_EXPIRE, 0, 166);
		} else if (_game._trigger == 165) {
			_scene->_sequences.remove(_globals._sequenceIndexes[14]);
			_game._objects.addToInventory(OBJ_REPAIR_LIST);
		}
	} else if (_game._trigger == 166) {
		_game._player._priorTimer = _scene->_frameStartTime + _game._player._ticksAmount;
		_game._player._visible = true;
		_scene->_sequences.addTimer(20, 167);
	} else if (_game._trigger == 167) {
		_vm->_dialogs->showItem(OBJ_REPAIR_LIST, 40240);
		_game._player._stepEnabled = true;
	} else if (_game._screenObjects._inputMode == 1)
		handleDialogs();
	else if (_action.isAction(0x242, 0x2B3))
		_scene->_nextSceneId = 401;
	else if (_action.isAction(0x2B5, 0x248))
		; // just... nothing
	else if (_action.isAction(VERB_TALKTO, 0x3AA)) {
		switch (_game._trigger) {
		case 0: {
			_game._player._stepEnabled = false;
			int random = _vm->getRandomNumber(1, 3);
			if (_helgaTalkMode == 0)
				random = 1;

			int centerFlag;
			Common::Point centerPos;
			if (_roxOnStool) {
				centerFlag = 0;
				centerPos = Common::Point(230, 56);
			} else {
				centerFlag = 2;
				centerPos = Common::Point(0, 0);
			}

			switch (random) {
			case 1:
				_scene->_kernelMessages.add(centerPos, 0x1110, 32 | centerFlag, 0, 90, _game.getQuote(0x211));
				_scene->_sequences.addTimer(110, 25);
				break;

			case 2:
				_scene->_kernelMessages.add(centerPos, 0x1110, 32 | centerFlag, 0, 90, _game.getQuote(0x212));
				_scene->_sequences.addTimer(110, 25);
				break;

			case 3:
				_scene->_kernelMessages.add(centerPos, 0x1110, 32 | centerFlag, 0, 90, _game.getQuote(0x213));
				_scene->_sequences.addTimer(110, 25);
				break;

			default:
				break;
		}
		}
		break;

		case 25:
			switch (_helgaTalkMode) {
			case 0:
				_game._player._stepEnabled = false;
				_helgaTalkMode = 1;
				_scene->_kernelMessages.add(Common::Point(177, 33), 0xFBFA, 0, 0, 130, _game.getQuote(0x209));
				_scene->_kernelMessages.add(Common::Point(182, 47), 0xFBFA, 0, 0, 130, _game.getQuote(0x20A));
				_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
				_scene->_sequences.addTimer(130, 28);
				break;

			case 1:
				_game._player._stepEnabled = false;
				_helgaTalkMode = 2;
				_scene->_kernelMessages.add(Common::Point(157, 47), 0xFBFA, 0, 0, 100, _game.getQuote(0x20B));
				_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
				_scene->_sequences.addTimer(100, 28);
				break;

			case 2:
				_game._player._stepEnabled = false;
				_helgaTalkMode = 3;
				_scene->_kernelMessages.add(Common::Point(172, 47), 0xFBFA, 0, 0, 100, _game.getQuote(0x20C));
				_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
				_scene->_sequences.addTimer(100, 28);
				break;

			case 3:
				_game._player._stepEnabled = true;
				break;

			default:
				break;
			}
			break;
		}
	} else if (_action.isAction(VERB_TALKTO, 0x30A) && !_firstTalkToGirlInChair) {
		_scene->_kernelMessages.add(Common::Point(0, 0), 0x1110, 34, 0, 120, _game.getQuote(0x1D7));
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_scene->_sequences.addTimer(150, 39);
		_game._player._stepEnabled = false;
		_firstTalkToGirlInChair = true;
	} else if (_action.isAction(VERB_TALKTO, 0x30A) && _firstTalkToGirlInChair) {
		_scene->_kernelMessages.add(Common::Point(0, 0), 0x1110, 34, 0, 120, _game.getQuote(0x1DB));
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_scene->_sequences.addTimer(150, 42);
		_game._player._stepEnabled = false;
	} else if (_action.isAction(VERB_TALKTO, 0x30C) || _action.isAction(VERB_WALKTO, 0x30C)) {
		_scene->_kernelMessages.add(Common::Point(102, 48), 0xFBFA, 0, 0, 120, _game.getQuote(0x1DE));
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_scene->_sequences.addTimer(120, 44);
		_game._player._stepEnabled = false;
	} else if (_action.isAction(0x30B, 0x24A) && (_game._player._targetPos.x == 248)){
		_scene->_kernelMessages.add(Common::Point(0, -14), 0x1110, 34, 0, 120, _game.getQuote(0x20D));
		_scene->_kernelMessages.add(Common::Point(0, 0), 0x1110, 34, 0, 120, _game.getQuote(0x20E));
	} else if (_action.isAction(0x30B, 0x24A) && !_roxOnStool && (_game._player._targetPos.x != 248)) {
		_game._player._visible = false;
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_globals._sequenceIndexes[6] = _scene->_sequences.addSpriteCycle(_globals._spriteIndexes[6], false, 7, 1, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[6], 1, 11);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[6], 5);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[6], SEQUENCE_TRIGGER_EXPIRE, 0, 76);
		_game._player._stepEnabled = false;
	} else if (_action.isAction(VERB_TAKE, 0x5C) && !_game._objects.isInInventory(OBJ_CREDIT_CHIP) && _roxOnStool) {
		_roxMode = 20;
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_scene->_sequences.addTimer(1, 92);
	} else if (_action.isAction(VERB_TAKE, 0x5C) && !_game._objects.isInInventory(OBJ_CREDIT_CHIP) && !_roxOnStool) {
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_game._player._stepEnabled = false;
		_game._player._visible = false;
		_globals._sequenceIndexes[22] = _scene->_sequences.startReverseCycle(_globals._spriteIndexes[22], false, 7, 2, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[22], 1, 2);
		_scene->_sequences.setMsgPosition(_globals._sequenceIndexes[22], Common::Point(_game._player._playerPos.x, _game._player._playerPos.y + 1));
		_scene->_sequences.setDepth(_globals._sequenceIndexes[22], 5);
		_scene->_sequences.setScale(_globals._sequenceIndexes[22], 88);
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[22], SEQUENCE_TRIGGER_EXPIRE, 0, 104);
	} else if (_action.isAction(VERB_TALKTO, 0x30E)) {
		switch (_game._trigger) {
		case 0: {
			int centerFlag;
			Common::Point centerPos;
			if (_roxOnStool) {
				centerFlag = 0;
				centerPos = Common::Point(230, 56);
			} else {
				centerFlag = 2;
				centerPos = Common::Point(0, 0);
			}

			_game._player._stepEnabled = false;
			int quoteId;
			if (_bartenderCalled) {
				quoteId = 0x210;
			} else {
				quoteId = 0x20F;
				_bartenderCalled = true;
			}
			_scene->_kernelMessages.reset();
			_scene->_kernelMessages.add(centerPos, 0x1110, 32 | centerFlag, 0, 90, _game.getQuote(quoteId));
			_scene->_sequences.addTimer(110, 29);
			_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
			_scene->_sequences.addTimer(90, 28);
			}
			break;

		case 29:
			_scene->_kernelMessages.reset();
			if (!_roxOnStool) {
				if (_game._objects.isInRoom(OBJ_ALIEN_LIQUOR)) {
					_scene->_kernelMessages.add(Common::Point(177, 41), 0xFDFC, 0, 0, 120, _game.getQuote(0x1DF));
					_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
					_scene->_sequences.addTimer(1, 100);
					_talkTimer = 120;
				} else if (_rexMode == 0) {
					_scene->_kernelMessages.add(Common::Point(175, 13), 0xFDFC, 0, 0, 180, _game.getQuote(0x1F9));
					_scene->_kernelMessages.add(Common::Point(184, 27), 0xFDFC, 0, 0, 180, _game.getQuote(0x1FA));
					_scene->_kernelMessages.add(Common::Point(200, 41), 0xFDFC, 0, 0, 180, _game.getQuote(0x1FB));
					_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
					_scene->_sequences.addTimer(1, 100);
					_talkTimer = 180;
					_rexMode = 1;
				} else if (_rexMode == 1) {
					_scene->_kernelMessages.add(Common::Point(205, 41), 0xFDFC, 0, 0, 120, _game.getQuote(0x1FC));
					_game._player._stepEnabled = true;
					_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
					_scene->_sequences.addTimer(1, 100);
					_talkTimer = 120;
					_rexMode = 3;
				} else {
					_game._player._stepEnabled = true;
				}
			} else {
				if (_game._objects.isInRoom(OBJ_ALIEN_LIQUOR)) {
					if (!_refuseAlienLiquor) {
						_scene->_kernelMessages.reset();
						_scene->_kernelMessages.add(Common::Point(198, 27), 0xFDFC, 0, 0, 9999999, _game.getQuote(0x1E2));
						_scene->_kernelMessages.add(Common::Point(199, 41), 0xFDFC, 0, 0, 9999999, _game.getQuote(0x1E3));
						_bartenderCurrentQuestion = 10;
						_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
						_scene->_sequences.addTimer(1, 100);
						_talkTimer = 120;
						_conversationFl = true;
						_bartenderDialogNode = 1;
						if (_dialog2.read(0) <= 1)
							_dialog1.write(0x214, false);

						_dialog1.start();
					} else {
						_scene->_kernelMessages.add(Common::Point(177, 41), 0xFDFC, 0, 0, 9999999, _game.getQuote(0x1EF));
						_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
						_scene->_sequences.addTimer(1, 100);
						_talkTimer = 120;
						_bartenderCurrentQuestion = 3;
						_conversationFl = true;
						_bartenderDialogNode = 1;
						if (_dialog2.read(0) <= 1)
							_dialog1.write(0x214, false);

						_dialog1.start();
						_game._player._stepEnabled = true;
					}
				} else {
					if (_rexMode == 0) {
						_scene->_kernelMessages.add(Common::Point(175, 13), 0xFDFC, 0, 0, 180, _game.getQuote(0x1F9));
						_scene->_kernelMessages.add(Common::Point(184, 27), 0xFDFC, 0, 0, 180, _game.getQuote(0x1FA));
						_scene->_kernelMessages.add(Common::Point(200, 41), 0xFDFC, 0, 0, 180, _game.getQuote(0x1FB));
						_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
						_scene->_sequences.addTimer(1, 100);
						_talkTimer = 180;
						_rexMode = 1;
					} else if (_rexMode == 1) {
						_scene->_kernelMessages.add(Common::Point(205, 41), 0xFDFC, 0, 0, 120, _game.getQuote(0x1FC));
						_game._player._stepEnabled = true;
						_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
						_scene->_sequences.addTimer(1, 100);
						_talkTimer = 120;
						_rexMode = 3;
					} else {
						_game._player._stepEnabled = true;
					}
				}
			}
			break;

		default:
			break;
		}
	} else if (_action.isAction(VERB_LOOK, 0x248))
		_vm->_dialogs->show(40210);
	else if (_action.isAction(VERB_LOOK, 0x16C)) {
		if (_globals[kSomeoneHasExploded])
			_vm->_dialogs->show(40212);
		else
			_vm->_dialogs->show(40211);
	} else if (_action.isAction(VERB_LOOK, 0x241))
		_vm->_dialogs->show(40213);
	else if (_action.isAction(VERB_LOOK, 0x30E))
		_vm->_dialogs->show(40214);
	else if (_action.isAction(VERB_LOOK, 0x30C))
		_vm->_dialogs->show(40215);
	else if (_action.isAction(VERB_LOOK, 0x309))
		_vm->_dialogs->show(40216);
	else if (_action.isAction(VERB_LOOK, 0x30A))
		_vm->_dialogs->show(40217);
	else if (_action.isAction(VERB_LOOK, 0x3AA))
		_vm->_dialogs->show(40218);
	else if (_action.isAction(VERB_LOOK, 0x2B3))
		_vm->_dialogs->show(40219);
	else if (_action._lookFlag)
		_vm->_dialogs->show(40220);
	else if (_action.isAction(VERB_LOOK, 0x2B8))
		_vm->_dialogs->show(40221);
	else if (_action.isAction(VERB_PUSH, 0x3AA) || _action.isAction(VERB_PULL, 0x3AA))
		_vm->_dialogs->show(40222);
	else if (_action.isAction(VERB_TALKTO, 0x2B8))
		_vm->_dialogs->show(40223);
	else if (_action.isAction(VERB_TALKTO, 0x309))
		_vm->_dialogs->show(40224);
	else if (_action.isAction(VERB_LOOK, 0x249))
		_vm->_dialogs->show(40225);
	else if (_action.isAction(VERB_LOOK, 0x160))
		_vm->_dialogs->show(40226);
	else if (_action.isAction(VERB_TAKE, 0x160))
		_vm->_dialogs->show(40227);
	else if (_action.isAction(VERB_LOOK, 0x244))
		_vm->_dialogs->show(40228);
	else if (_action.isAction(VERB_TAKE, 0x244))
		_vm->_dialogs->show(40229);
	else if (_action.isAction(VERB_LOOK, 0x24A))
		_vm->_dialogs->show(40230);
	else if (_action.isAction(VERB_TAKE, 0x24A))
		_vm->_dialogs->show(40231);
	else if (_action.isAction(VERB_LOOK, 0x3B))
		_vm->_dialogs->show(40232);
	else if (_action.isAction(VERB_TAKE, 0x3B))
		_vm->_dialogs->show(40233);
	else if (_action.isAction(VERB_LOOK, 0x24E))
		_vm->_dialogs->show(40234);
	else if (_action.isAction(VERB_LOOK, 0x247))
		_vm->_dialogs->show(40235);
	else if (_action.isAction(VERB_LOOK, 0x174))
		_vm->_dialogs->show(40236);
	else if (_action.isAction(VERB_LOOK, 0x10E))
		_vm->_dialogs->show(40237);
	else if (_action.isAction(VERB_TAKE, 0x10E))
		_vm->_dialogs->show(40238);
	else if (_action.isAction(VERB_LOOK, 0x124) && _game._objects.isInRoom(OBJ_REPAIR_LIST))
		_vm->_dialogs->show(40239);
	else
		return;

	_action._inProgress = false;
}

/*------------------------------------------------------------------------*/

void Scene405::setup() {
	setPlayerSpritesPrefix();
	setAAName();
}

void Scene405::enter() {
	_globals._spriteIndexes[1] = _scene->_sprites.addSprites(formAnimName('x', 0));
	_globals._spriteIndexes[2] = _scene->_sprites.addSprites(formAnimName('x', 1));
	_globals._spriteIndexes[3] = _scene->_sprites.addSprites("*ROXCL_8");

	if (_scene->_priorSceneId == 401) {
		_game._player._playerPos = Common::Point(23, 123);
		_game._player._facing = FACING_EAST;
	} else if (_scene->_priorSceneId == 406) {
		_game._player._playerPos = Common::Point(300, 128);
		_game._player._facing = FACING_WEST;
	} else if (_scene->_priorSceneId == 408) {
		_game._player._playerPos = Common::Point(154, 109);
		_game._player._facing = FACING_SOUTH;
	} else if (_scene->_priorSceneId == 413) {
		_game._player._playerPos = Common::Point(284, 109);
		_game._player._facing = FACING_SOUTH;
	} else if (_scene->_priorSceneId != -2) {
		_game._player._playerPos = Common::Point(23, 123);
		_game._player._facing = FACING_EAST;
	}

	if (_globals[kArmoryDoorOpen])
		_globals._sequenceIndexes[2] = _scene->_sequences.startCycle(_globals._spriteIndexes[2], false, 1);
	else
		_globals._sequenceIndexes[1] = _scene->_sequences.startCycle(_globals._spriteIndexes[1], false, 1);

	if (_scene->_roomChanged) {
		_globals[kArmoryDoorOpen] = false;
		_game._objects.addToInventory(OBJ_SECURITY_CARD);
	}

	_game.loadQuoteSet(0x24F, 0);
	sceneEntrySound();
}

void Scene405::step() {
	if (_game._trigger == 80) {
		_scene->_sequences.addTimer(20, 81);
		_game._player._priorTimer = _scene->_frameStartTime + _game._player._ticksAmount;
		_game._player._visible = true;
	}

	if (_game._trigger == 81) {
		_game._player._stepEnabled = true;
		_vm->_dialogs->show(40525);
	}

	if (_game._trigger == 70) {
		_game._player._priorTimer = _scene->_frameStartTime + _game._player._ticksAmount ;
		_game._player._visible = true;
		_globals._sequenceIndexes[1] = _scene->_sequences.startReverseCycle(_globals._spriteIndexes[1], false, 6, 1, 0, 0);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[1], SEQUENCE_TRIGGER_EXPIRE, 0, 71);
		_vm->_sound->command(19);
	}

	if (_game._trigger == 71) {
		_globals._sequenceIndexes[1] = _scene->_sequences.startCycle(_globals._spriteIndexes[1], false, 1);
		_globals[kArmoryDoorOpen] = false;
		_scene->_sequences.remove(_globals._sequenceIndexes[2]);
		_game._player._stepEnabled = true;
	}

	if (_game._trigger == 75) {
		_game._player._priorTimer = _scene->_frameStartTime + _game._player._ticksAmount ;
		_game._player._visible = true;
		_scene->_sequences.remove(_globals._sequenceIndexes[1]);
		_globals._sequenceIndexes[1] = _scene->_sequences.addSpriteCycle (_globals._spriteIndexes[1], false, 6, 1, 0, 0);
		_globals[kArmoryDoorOpen] = true;
		_game._player._stepEnabled = true;
		_globals._sequenceIndexes[2] = _scene->_sequences.startCycle (_globals._spriteIndexes[2],
			false, 1);
		_vm->_sound->command(19);
	}
}

void Scene405::preActions() {
	if (_action.isAction(VERB_TAKE))
		_game._player._needToWalk = false;

	if (_action.isAction(0x1AD, 0x2BA))
		_game._player._walkOffScreenSceneId = 401;

	if (_action.isAction(0x1AD, 0x2B9))
		_game._player._walkOffScreenSceneId = 406;

	if (_action.isAction(VERB_CLOSE, 0x259) && _globals[kArmoryDoorOpen])
		_game._player.walk(Common::Point(212, 113), FACING_NORTH);
}

void Scene405::actions() {
	if (_action.isAction(0x18B, 0x6E))
		_scene->_nextSceneId = 413;
	else if (_action.isAction(0x18B, 0x259) && _globals[kArmoryDoorOpen])
		_scene->_nextSceneId = 408;
	else if (_action.isAction(0x18B, 0x259) && !_globals[kArmoryDoorOpen])
		_scene->_kernelMessages.add(Common::Point(0, 0), 0x1110, 34, 0, 60, _game.getQuote(0x24F));
	else if (_action.isAction(VERB_PUT, 0x131, 0x251) && !_globals[kArmoryDoorOpen]) {
		_game._player._stepEnabled = false;
		_game._player._visible = false;
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_globals._sequenceIndexes[3] = _scene->_sequences.startReverseCycle(_globals._spriteIndexes[3], false, 7, 2, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[3], 1, 2);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[3], SEQUENCE_TRIGGER_EXPIRE, 0, 75);
		Common::Point msgPos = Common::Point(_game._player._playerPos.x, _game._player._playerPos.y + 1);
		_scene->_sequences.setMsgPosition(_globals._sequenceIndexes[3], msgPos);
		_scene->_sequences.setScale(_globals._sequenceIndexes[3], 87);
	} else if ((_action.isAction(VERB_PUT, 0x131, 0x251) || _action.isAction(VERB_CLOSE, 0x259)) && _globals[kArmoryDoorOpen]) {
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_game._player._stepEnabled = false;
		_game._player._visible = false;
		_globals._sequenceIndexes[3] = _scene->_sequences.startReverseCycle(_globals._spriteIndexes[3], false, 7, 2, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[3], 1, 2);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[3], SEQUENCE_TRIGGER_EXPIRE, 0, 70);
		_scene->_sequences.setMsgPosition(_globals._sequenceIndexes[3], _game._player._playerPos);
		_scene->_sequences.setScale(_globals._sequenceIndexes[3], 87);
	} else if (_action.isAction(VERB_PUT, 0x251)) {
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_game._player._stepEnabled = false;
		_game._player._visible = false;
		_globals._sequenceIndexes[3] = _scene->_sequences.startReverseCycle(_globals._spriteIndexes[3], false, 7, 2, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[3], 1, 2);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[3], SEQUENCE_TRIGGER_EXPIRE, 0, 80);
		_scene->_sequences.setMsgPosition(_globals._sequenceIndexes[3], _game._player._playerPos);
		_scene->_sequences.setScale(_globals._sequenceIndexes[3], 87);
	} else if (_action.isAction(VERB_LOOK, 0x31A))
		_vm->_dialogs->show(40510);
	else if (_action.isAction(VERB_TAKE, 0x31A))
		_vm->_dialogs->show(40511);
	else if (_action.isAction(VERB_LOOK, 0x254))
		_vm->_dialogs->show(40512);
	else if (_action.isAction(VERB_LOOK, 0x258) || _action.isAction(VERB_LOOK, 0x252))
		_vm->_dialogs->show(40513);
	else if (_action.isAction(VERB_LOOK, 0xCD))
		_vm->_dialogs->show(40514);
	else if (_action.isAction(VERB_LOOK, 0x251))
		_vm->_dialogs->show(40515);
	else if (_action.isAction(VERB_LOOK, 0x2B9))
		_vm->_dialogs->show(40516);
	else if (_action.isAction(VERB_LOOK, 0x2BA))
		_vm->_dialogs->show(40517);
	else if (_action.isAction(VERB_LOOK, 0xE2))
		_vm->_dialogs->show(40518);
	else if (_action.isAction(VERB_LOOK, 0x204) || _action._lookFlag)
		_vm->_dialogs->show(40519);
	else if (_action.isAction(VERB_LOOK, 0x259)) {
		if (_globals[kArmoryDoorOpen])
			_vm->_dialogs->show(40521);
		else
			_vm->_dialogs->show(40520);
	} else if (_action.isAction(VERB_LOOK, 0x6E))
		_vm->_dialogs->show(40522);
	else if (_action.isAction(VERB_LOOK, 0x250))
		_vm->_dialogs->show(40523);
	else if (_action.isAction(VERB_LOOK, 0x257))
		_vm->_dialogs->show(40524);
	else
		return;

	_action._inProgress = false;
}

/*------------------------------------------------------------------------*/

void Scene406::setup() {
	setPlayerSpritesPrefix();
	setAAName();
}

void Scene406::enter() {
	_game._player._visible = true;
	if (_scene->_priorSceneId == 405) {
		_game._player._playerPos = Common::Point(15, 129);
		_game._player._facing = FACING_EAST;
	} else if (_scene->_priorSceneId == 407) {
		_game._player._playerPos = Common::Point(270, 127);
		_game._player._facing = FACING_WEST;
	} else if (_scene->_priorSceneId == 410) {
		_game._player._playerPos = Common::Point(30, 108);
		_game._player._facing = FACING_SOUTH;
	} else if (_scene->_priorSceneId == 411) {
		_game._player._playerPos = Common::Point(153, 108);
		_game._player._facing = FACING_SOUTH;
	} else if (_scene->_priorSceneId != -2) {
		_game._player._playerPos = Common::Point(15, 129);
		_game._player._facing = FACING_EAST;
	}

	_globals._spriteIndexes[2] = _scene->_sprites.addSprites("*ROXCL_8");
	_globals._spriteIndexes[1] = _scene->_sprites.addSprites(formAnimName('x', 0));
	_globals._spriteIndexes[3] = _scene->_sprites.addSprites(formAnimName('x', 1));

	if (_scene->_roomChanged) {
		_globals[kStorageDoorOpen] = false;
		_game._objects.addToInventory(OBJ_SECURITY_CARD);
	}

	if (!_globals[kStorageDoorOpen])
		_globals._sequenceIndexes[1] = _scene->_sequences.startCycle(_globals._spriteIndexes[1], false, 1);

	if (_scene->_priorSceneId != 411)
		_globals._sequenceIndexes[3] = _scene->_sequences.startCycle(_globals._spriteIndexes[3], false, 1);
	else {
		_game._player._stepEnabled = false;
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_globals._sequenceIndexes[3] = _scene->_sequences.startReverseCycle(_globals._spriteIndexes[3], false, 3, 1, 0, 0);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[3], SEQUENCE_TRIGGER_EXPIRE, 0, 90);
		_vm->_sound->command(19);
	}

	_game.loadQuoteSet(0x24F, 0);
	_hitStorageDoor = false;
	sceneEntrySound();
}

void Scene406::step() {
	if (_game._trigger == 90) {
		_game._player._stepEnabled = true;
		_globals._sequenceIndexes[3] = _scene->_sequences.startCycle(_globals._spriteIndexes[3], false, 1);
	}

	if (_game._trigger == 80)
		_scene->_nextSceneId = 411;

	if (_game._trigger == 100) {
		_vm->_dialogs->show(40622);
		_hitStorageDoor = true;
	}

	if (_game._trigger == 110) {
		_scene->_sequences.addTimer(20, 111);
		_game._player._priorTimer = _scene->_frameStartTime + _game._player._ticksAmount;
		_game._player._visible = true;
	}

	if (_game._trigger == 111) {
		_game._player._stepEnabled = true;
		_vm->_dialogs->show(40613);
	}

	if (_game._trigger == 70) {
		_game._player._priorTimer = _scene->_frameStartTime + _game._player._ticksAmount;
		_game._player._visible = true;
		_globals._sequenceIndexes[1] = _scene->_sequences.startReverseCycle(_globals._spriteIndexes[1], false, 4, 1, 0, 0);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[1], SEQUENCE_TRIGGER_EXPIRE, 0, 71);
		_vm->_sound->command(19);
	}

	if (_game._trigger == 71) {
		_globals._sequenceIndexes[1] = _scene->_sequences.startCycle(_globals._spriteIndexes[1], false, 1);
		_globals[kStorageDoorOpen] = false;
		_game._player._stepEnabled = true;
	}

	if (_game._trigger == 75) {
		_scene->_sequences.remove(_globals._sequenceIndexes[1]);
		_globals._sequenceIndexes[1] = _scene->_sequences.addSpriteCycle (_globals._spriteIndexes[1], false, 4, 1, 0, 0);
		_globals[kStorageDoorOpen] = true;
		_game._player._stepEnabled = true;
		_game._player._priorTimer = _scene->_frameStartTime + _game._player._ticksAmount;
		_game._player._visible = true;
		_vm->_sound->command(19);
	}
}

void Scene406::preActions() {
	if (_action.isAction(0x1AD, 0x2BA))
		_game._player._walkOffScreenSceneId = 405;

	if (_action.isAction(0x1AD, 0x2B9))
		_game._player._walkOffScreenSceneId = 407;

	if (_action.isAction(VERB_TAKE))
		_game._player._needToWalk = false;

	if (_action.isAction(VERB_LOOK, 0x244) || _action.isAction(VERB_LOOK, 0x25C))
		_game._player._needToWalk = true;
}

void Scene406::actions() {
	if (_action.isAction(0x18B, 0x6E) && (_game._player._targetPos.x> 100)) {
		_game._player._stepEnabled = false;
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_scene->_sequences.remove(_globals._sequenceIndexes[3]);
		_globals._sequenceIndexes[3] = _scene->_sequences.addSpriteCycle (_globals._spriteIndexes[3], false, 3, 1, 0, 0);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[3], SEQUENCE_TRIGGER_EXPIRE, 0, 80);
		_vm->_sound->command(19);
	} else if (_action.isAction(0x18B, 0x6E) && _globals[kStorageDoorOpen] && (_game._player._targetPos.x < 100))
		_scene->_nextSceneId = 410;
	else if (_action.isAction(0x18B, 0x6E) && !_globals[kStorageDoorOpen] && (_game._player._targetPos.x < 100)) {
		_scene->_kernelMessages.add(Common::Point(0, 0), 0x1110, 34, 0, 60, _game.getQuote(0x24F));
		if (!_hitStorageDoor) {
			_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
			_scene->_sequences.addTimer(80, 100);
		}
	} else if (_action.isAction(VERB_PUT, 0x131, 0x251) && !_globals[kStorageDoorOpen]) {
		_game._player._stepEnabled = false;
		_game._player._visible = false;
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_globals._sequenceIndexes[2] = _scene->_sequences.startReverseCycle(_globals._spriteIndexes[2], false, 7, 2, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[2], 1, 2);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[2], SEQUENCE_TRIGGER_EXPIRE, 0, 75);
		Common::Point msgPos = Common::Point(_game._player._playerPos.x, _game._player._playerPos.y + 1);
		_scene->_sequences.setMsgPosition(_globals._sequenceIndexes[2], msgPos);
		_scene->_sequences.setScale(_globals._sequenceIndexes[2], 87);
	} else if (_action.isAction(VERB_PUT, 0x131, 0x251) && _globals[kStorageDoorOpen]) {
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_game._player._stepEnabled = false;
		_game._player._visible = false;
		_globals._sequenceIndexes[2] = _scene->_sequences.startReverseCycle(_globals._spriteIndexes[2], false, 7, 2, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[2], 1, 2);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[2], SEQUENCE_TRIGGER_EXPIRE, 0, 70);
		Common::Point msgPos = Common::Point(_game._player._playerPos.x, _game._player._playerPos.y + 1);
		_scene->_sequences.setMsgPosition(_globals._sequenceIndexes[2], msgPos);
		_scene->_sequences.setScale(_globals._sequenceIndexes[2], 87);
	} else if (_action.isAction(VERB_PUT, 0x251)) {
		_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
		_game._player._stepEnabled = false;
		_game._player._visible = false;
		_globals._sequenceIndexes[2] = _scene->_sequences.startReverseCycle(_globals._spriteIndexes[2], false, 7, 2, 0, 0);
		_scene->_sequences.setAnimRange(_globals._sequenceIndexes[2], 1, 2);
		_scene->_sequences.addSubEntry(_globals._sequenceIndexes[2], SEQUENCE_TRIGGER_EXPIRE, 0, 110);
		_scene->_sequences.setMsgPosition(_globals._sequenceIndexes[2], _game._player._playerPos);
		_scene->_sequences.setScale(_globals._sequenceIndexes[2], 87);
	} else if (_action.isAction(VERB_LOOK, 0x25C))
		_vm->_dialogs->show(40610);
	else if (_action.isAction(VERB_TAKE, 0x25C))
		_vm->_dialogs->show(40611);
	else if (_action.isAction(VERB_LOOK, 0x251))
		_vm->_dialogs->show(40612);
	else if (_action.isAction(VERB_LOOK, 0x25B))
		_vm->_dialogs->show(40614);
	else if (_action.isAction(VERB_TAKE, 0x25B))
		_vm->_dialogs->show(40615);
	else if (_action.isAction(VERB_LOOK, 0x2B9))
		_vm->_dialogs->show(40616);
	else if (_action.isAction(VERB_LOOK, 0x2BA))
		_vm->_dialogs->show(40617);
	else if (_action.isAction(VERB_LOOK, 0x204) || _action._lookFlag)
		_vm->_dialogs->show(40618);
	else if (_action.isAction(VERB_LOOK, 0x18D))
		_vm->_dialogs->show(40619);
	else if (_action.isAction(VERB_LOOK, 0x6E)) {
		if (_globals[kStorageDoorOpen])
			_vm->_dialogs->show(40621);
		else
			_vm->_dialogs->show(40620);
	} else if (_action.isAction(VERB_LOOK, 0xE2))
		_vm->_dialogs->show(40623);
	else if (_action.isAction(VERB_LOOK, 0x47A))
		_vm->_dialogs->show(40624);
	else if (_action.isAction(VERB_TAKE, 0x47A))
		_vm->_dialogs->show(40625);
	else if (_action.isAction(VERB_LOOK, 0x2F))
		_vm->_dialogs->show(40626);
	else if (_action.isAction(VERB_TAKE, 0x2F))
		_vm->_dialogs->show(40627);
	else if (_action.isAction(VERB_LOOK, 0x244))
		_vm->_dialogs->show(40628);
	else if (_action.isAction(VERB_TAKE, 0x244))
		_vm->_dialogs->show(40629);
	else
		return;

	_action._inProgress = false;
}

/*------------------------------------------------------------------------*/

void Scene407::setup() {
	setPlayerSpritesPrefix();
	setAAName();
}

void Scene407::enter() {
	if (_scene->_priorSceneId != -2)
		_fromNorth = false;

	if (_scene->_priorSceneId == 318) {
		_game._player._playerPos = Common::Point(172, 92);
		_game._player._facing = FACING_SOUTH;
		_fromNorth = true;
	} else if (_scene->_priorSceneId != -2) {
		_game._player._playerPos = Common::Point(172, 132);
		_game._player._facing = FACING_NORTH;
	}

	_game.loadQuoteSet(0x250, 0);
	sceneEntrySound();
}

void Scene407::step() {
	if (_game._trigger == 70) {
		_scene->_nextSceneId = 318;
		_scene->_reloadSceneFlag = true;
	}

	if (_game._trigger == 80) {
		_game._player._priorTimer = _scene->_frameStartTime - _game._player._ticksAmount;
		_game._player._stepEnabled = true;
		_game._player._visible = true;
		_fromNorth = false;
		_game._player.walk(Common::Point(173, 104), FACING_SOUTH);
	}
}

void Scene407::preActions() {
	if (_action.isAction(VERB_TAKE))
		_game._player._needToWalk = false;

	if (_action.isAction(VERB_LOOK, 0x6E))
		_game._player._needToWalk = true;

	if (_action.isAction(0x1AD, 0x2B4)) {
		_game._player.walk(Common::Point(172, 91), FACING_NORTH);
		_fromNorth = false;
	}

	if (_game._player._needToWalk && _fromNorth) {
		if (_globals[kSexOfRex] == REX_MALE)
			destPos = Common::Point(171, 95);
		else
			destPos = Common::Point(173, 96);

		_game._player.walk(destPos, FACING_SOUTH);
	}
}

void Scene407::actions() {
	if ((_game._player._playerPos == destPos) && _fromNorth) {
		if (_globals[kSexOfRex] == REX_MALE) {
			_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
			_game._player._stepEnabled = false;
			_game._player._visible = false;
			_vm->_sound->command(21);
			_scene->loadAnimation(formAnimName('s', 1), 70);
			_globals[kHasBeenScanned] = true;
			_scene->_kernelMessages.add(Common::Point(0, 0), 0x1110, 34, 0, 60, _game.getQuote(60));
			_vm->_sound->command(22);
		}

		if (_globals[kSexOfRex] == REX_FEMALE) {
			_game._triggerSetupMode = SEQUENCE_TRIGGER_DAEMON;
			_game._player._stepEnabled = false;
			_game._player._visible = false;
			_vm->_sound->command(21);
			_scene->loadAnimation(formAnimName('s', 2), 80);
			_vm->_sound->command(23);
			_globals[kHasBeenScanned] = true;
		}
	}

	if (_action.isAction(0x1AD, 0x2B3) && !_fromNorth)
		_scene->_nextSceneId = 406;
	else if (_action.isAction(0x1AD, 0x2B4))
		_scene->_nextSceneId = 318;
	else if (_action.isAction(VERB_LOOK, 0x1F3)) {
		if (_globals[kHasBeenScanned])
			_vm->_dialogs->show(40711);
		else
			_vm->_dialogs->show(40710);
	} else if (_action.isAction(VERB_LOOK, 0x6E))
		_vm->_dialogs->show(40712);
	else if (_action.isAction(VERB_LOOK, 0x2B3))
		_vm->_dialogs->show(40713);
	else if (_action.isAction(VERB_LOOK, 0x2B4))
		_vm->_dialogs->show(40714);
	else if (_action._lookFlag)
		_vm->_dialogs->show(40715);
	else
		return;

	_action._inProgress = false;
}

/*------------------------------------------------------------------------*/

void Scene408::setup() {
	setPlayerSpritesPrefix();
	setAAName();
	_scene->addActiveVocab(0x167);
	_scene->addActiveVocab(0xD);
}

void Scene408::enter() {
	_game._player._playerPos = Common::Point(137, 150);
	_game._player._facing = FACING_NORTH;

	_globals._spriteIndexes[1] = _scene->_sprites.addSprites("*ROXRC_7");
	_globals._spriteIndexes[2] = _scene->_sprites.addSprites(formAnimName('m', -1));

	if (_game._objects.isInRoom(OBJ_TARGET_MODULE)) {
		_globals._sequenceIndexes[2] = _scene->_sequences.startCycle(_globals._spriteIndexes[2], false, 1);
		_scene->_sequences.setDepth(_globals._sequenceIndexes[2], 3);
		int idx = _scene->_dynamicHotspots.add(0x167, 0xD, _globals._sequenceIndexes[2], Common::Rect(0, 0, 0, 0));
		_scene->_dynamicHotspots.setPosition(idx, Common::Point(283, 128), FACING_NORTHEAST);
	}
	sceneEntrySound();
}

void Scene408::preActions() {
	if ((_action.isAction(VERB_TAKE) && !_action.isAction(0x167)) || _action.isAction(VERB_PULL, 0x47B) || _action.isAction(VERB_OPEN, 0x265))
		_game._player._needToWalk = false;

	if ((_action.isAction(VERB_LOOK, 0x167) && _game._objects.isInRoom(0x17)) || _action.isAction(VERB_LOOK, 0x263))
		_game._player._needToWalk = true;
}

void Scene408::actions() {
	if (_action.isAction(0x242, 0x2B3)) {
		_scene->_nextSceneId = 405;
		_vm->_sound->command(58);
	} else if (_action.isAction(VERB_TAKE, 0x167) && (_game._objects.isInRoom(OBJ_TARGET_MODULE) || _game._trigger)) {
		switch (_game._trigger) {
		case (0):
			_vm->_sound->command(57);
			_game._player._stepEnabled = false;
			_game._player._visible = false;
			_globals._sequenceIndexes[1] = _scene->_sequences.startReverseCycle(_globals._spriteIndexes[1], true, 7, 2, 0, 0);
			_scene->_sequences.setAnimRange(_globals._sequenceIndexes[1], 1, 2);
			_scene->_sequences.setMsgLayout(_globals._sequenceIndexes[1]);
			_scene->_sequences.addSubEntry(_globals._sequenceIndexes[1], SEQUENCE_TRIGGER_SPRITE, 2, 1);
			_scene->_sequences.addSubEntry(_globals._sequenceIndexes[1], SEQUENCE_TRIGGER_EXPIRE, 0, 2);
			break;

		case 1:
			_scene->_sequences.remove(_globals._sequenceIndexes[2]);
			_game._objects.addToInventory(OBJ_TARGET_MODULE);
			_vm->_dialogs->showItem(OBJ_TARGET_MODULE, 40847);
			break;

		case 2:
			_game._player._priorTimer = _game._player._ticksAmount + _scene->_frameStartTime;
			_game._player._visible = true;
			_scene->_sequences.addTimer(20, 3);
			break;

		case 3:
			_game._player._stepEnabled = true;
			break;

		default:
			break;
		}
	} else if (_action.isAction(VERB_LOOK, 0x2BC))
		_vm->_dialogs->show(40810);
	else if (_action.isAction(VERB_TAKE, 0x2BC) || _action.isAction(VERB_TAKE, 0x261) || _action.isAction(VERB_TAKE, 0x25F))
		_vm->_dialogs->show(40811);
	else if (_action.isAction(VERB_LOOK, 0x2BE))
		_vm->_dialogs->show(40812);
	else if (_action.isAction(VERB_TAKE, 0x2BE))
		_vm->_dialogs->show(40813);
	else if (_action.isAction(VERB_LOOK, 0x2C2))
		_vm->_dialogs->show(40814);
	else if (_action.isAction(VERB_TAKE, 0x2C2))
		_vm->_dialogs->show(40815);
	else if (_action.isAction(VERB_TAKE, 0x47B) || _action.isAction(VERB_PULL, 0x47B))
		_vm->_dialogs->show(40816);
	else if (_action.isAction(VERB_LOOK, 0x26D))
		_vm->_dialogs->show(40817);
	else if (_action.isAction(VERB_TAKE, 0x26D))
		_vm->_dialogs->show(40818);
	else if (_action.isAction(VERB_LOOK, 0x2BB))
		_vm->_dialogs->show(40819);
	else if (_action.isAction(VERB_TAKE, 0x2BB))
		_vm->_dialogs->show(40820);
	else if (_action.isAction(VERB_LOOK, 0x269))
		_vm->_dialogs->show(40821);
	else if (_action.isAction(VERB_TAKE, 0x269))
		_vm->_dialogs->show(40822);
	else if (_action.isAction(VERB_LOOK, 0x263)) {
		if (_game._objects.isInRoom(OBJ_TARGET_MODULE))
			_vm->_dialogs->show(40823);
		else
			_vm->_dialogs->show(40824);
	} else if (_action.isAction(VERB_TAKE, 0x263))
		_vm->_dialogs->show(40825);
	else if (_action.isAction(VERB_LOOK, 0x2BF))
		_vm->_dialogs->show(40826);
	else if (_action.isAction(VERB_TAKE, 0x2BF))
		_vm->_dialogs->show(40827);
	else if (_action.isAction(VERB_LOOK, 0x7B))
		_vm->_dialogs->show(40828);
	else if (_action.isAction(VERB_OPEN, 0x7B) || _action.isAction(VERB_PULL, 0x7B))
		_vm->_dialogs->show(40829);
	else if (_action.isAction(VERB_LOOK, 0x26B))
		_vm->_dialogs->show(40830);
	else if (_action.isAction(VERB_TAKE, 0x26B))
		_vm->_dialogs->show(40831);
	else if (_action.isAction(VERB_LOOK, 0x2C1))
		_vm->_dialogs->show(40832);
	else if (_action.isAction(VERB_TAKE, 0x2C1))
		_vm->_dialogs->show(40833);
	else if (_action.isAction(VERB_LOOK, 0x2BD))
		_vm->_dialogs->show(40834);
	else if (_action.isAction(VERB_TAKE, 0x2BD))
		_vm->_dialogs->show(40835);
	else if (_action.isAction(VERB_LOOK, 0x261))
		_vm->_dialogs->show(40836);
	else if (_action.isAction(VERB_LOOK, 0x25F))
		_vm->_dialogs->show(40837);
	else if (_action.isAction(VERB_LOOK, 0x2C0))
		_vm->_dialogs->show(40838);
	else if (_action.isAction(VERB_LOOK, 0x47C))
		_vm->_dialogs->show(40839);
	else if (_action.isAction(VERB_TAKE, 0x47C))
		_vm->_dialogs->show(40840);
	else if (_action.isAction(VERB_LOOK, 0x265))
		_vm->_dialogs->show(40841);
	else if (_action.isAction(VERB_TAKE, 0x265))
		_vm->_dialogs->show(40842);
	else if (_action.isAction(VERB_OPEN, 0x265))
		_vm->_dialogs->show(40843);
	else if (_action.isAction(VERB_LOOK, 0x2B3))
		_vm->_dialogs->show(40844);
	else if (_action._lookFlag)
		_vm->_dialogs->show(40845);
	else if (_action.isAction(VERB_LOOK, 0x167) && _game._objects.isInRoom(OBJ_TARGET_MODULE))
		_vm->_dialogs->show(40846);
	else if (_action.isAction(VERB_LOOK, 0x26C))
		_vm->_dialogs->show(40848);
	else if (_action.isAction(VERB_OPEN, 0x263))
		_vm->_dialogs->show(40849);
	else
		return;

	_action._inProgress = false;
}

/*------------------------------------------------------------------------*/

void Scene410::setup() {
	setPlayerSpritesPrefix();
	setAAName();
}

void Scene410::enter() {
	_globals._spriteIndexes[1] = _scene->_sprites.addSprites(formAnimName('y', -1));
	_globals._spriteIndexes[2] = _scene->_sprites.addSprites("*ROXRC_7");

	if (_game._objects.isInRoom(OBJ_CHARGE_CASES))
		_globals._sequenceIndexes[1] = _scene->_sequences.startCycle(_globals._spriteIndexes[1], false, 1);
	else
		_scene->_hotspots.activate(0x48, false);

	if (_scene->_priorSceneId != -2) {
		_game._player._playerPos = Common::Point(155, 150);
		_game._player._facing = FACING_NORTH;
	}

	sceneEntrySound();

	_scene->loadAnimation(Resources::formatName(410, 'r', -1, EXT_AA, ""));
	_scene->_activeAnimation->_resetFlag = true;
}

void Scene410::step() {
	if (_scene->_activeAnimation->getCurrentFrame() == 1) {
		if (_vm->getRandomNumber(1, 30) == 1)
			_scene->_activeAnimation->setCurrentFrame(2);
		else
			_scene->_activeAnimation->setCurrentFrame(0);
	}

	if (_scene->_activeAnimation->getCurrentFrame() == 9) {
		if (_vm->getRandomNumber(1, 30) == 1)
			_scene->_activeAnimation->setCurrentFrame(10);
		else
			_scene->_activeAnimation->setCurrentFrame(8);
	}

	if (_scene->_activeAnimation->getCurrentFrame() == 5) {
		if (_vm->getRandomNumber(1, 30) == 1)
			_scene->_activeAnimation->setCurrentFrame(6);
		else
			_scene->_activeAnimation->setCurrentFrame(4);
	}

	if (_scene->_activeAnimation->getCurrentFrame() == 3) {
		if (_vm->getRandomNumber(1, 2) == 1)
			_scene->_activeAnimation->setCurrentFrame(4);
		else // == 2
			_scene->_activeAnimation->setCurrentFrame(8);
	}
}

void Scene410::preActions() {
	if (_action.isAction(VERB_TAKE) && !_action.isAction(0x48))
		_game._player._needToWalk = false;

	if (_action.isAction(VERB_LOOK, 0x48) && _game._objects.isInRoom(OBJ_CHARGE_CASES))
		_game._player._needToWalk = true;

	if (_action.isAction(VERB_OPEN, 0x278) || _action.isAction(VERB_OPEN, 0x214))
		_game._player._needToWalk = false;

	if (_action.isAction(VERB_LOOK, 0x272))
		_game._player._needToWalk = true;
}

void Scene410::actions() {
	if (_action.isAction(0x242, 0x2B3))
		_scene->_nextSceneId = 406;
	else if (_action.isAction(VERB_TAKE, 0x48) && (_game._objects.isInRoom(OBJ_CHARGE_CASES) || _game._trigger)) {
		switch (_game._trigger) {
		case (0):
			_vm->_sound->command(57);
			_game._player._stepEnabled = false;
			_game._player._visible = false;  
			_globals._sequenceIndexes[2] = _scene->_sequences.startReverseCycle(_globals._spriteIndexes[2], false, 7, 2, 0, 0);
			_scene->_sequences.setAnimRange(_globals._sequenceIndexes[2], 1, 3);
			_scene->_sequences.setMsgLayout(_globals._sequenceIndexes[2]);
			_scene->_sequences.addSubEntry(_globals._sequenceIndexes[2], SEQUENCE_TRIGGER_SPRITE, 3, 1);
			_scene->_sequences.addSubEntry(_globals._sequenceIndexes[2], SEQUENCE_TRIGGER_EXPIRE, 0, 2);
			break;

		case 1:
			_scene->_sequences.remove(_globals._sequenceIndexes[1]);
			_scene->_hotspots.activate(0x48, false);
			_game._objects.addToInventory(OBJ_CHARGE_CASES);
			_vm->_dialogs->showItem(OBJ_CHARGE_CASES, 41032);
			break;

		case 2:
			_game._player._priorTimer = _game._player._ticksAmount + _scene->_frameStartTime;
			_game._player._visible = true;
			_scene->_sequences.addTimer(20, 3);
			break;

		case 3:
			_game._player._stepEnabled = true;
			break;

		default:
			break;
		}
	} else if (_action.isAction(VERB_LOOK, 0x276))
		_vm->_dialogs->show(41010);
	else if (_action.isAction(VERB_TAKE, 0x276))
		_vm->_dialogs->show(41011);
	else if (_action.isAction(VERB_OPEN, 0x276))
		_vm->_dialogs->show(41012);
	else if (_action.isAction(VERB_LOOK, 0x270))
		_vm->_dialogs->show(41013);
	else if (_action.isAction(VERB_TAKE, 0x270))
		_vm->_dialogs->show(41014);
	else if (_action.isAction(VERB_LOOK, 0x265) || _action.isAction(VERB_OPEN, 0x265)) {
		if (_game._objects.isInRoom(OBJ_CHARGE_CASES))
			_vm->_dialogs->show(41015);
		else
			_vm->_dialogs->show(41016);
	} else if (_action.isAction(VERB_LOOK, 0x26E))
		_vm->_dialogs->show(41017);
	else if (_action.isAction(VERB_TAKE, 0x26E))
		_vm->_dialogs->show(41018);
	else if (_action.isAction(VERB_LOOK, 0x278))
		_vm->_dialogs->show(41019);
	else if (_action.isAction(VERB_LOOK, 0x214))
		_vm->_dialogs->show(41019);
	else if (_action.isAction(VERB_OPEN, 0x278))
		_vm->_dialogs->show(41020);
	else if (_action.isAction(VERB_OPEN, 0x214))
		_vm->_dialogs->show(41020);
	else if (_action.isAction(VERB_LOOK, 0x277))
		_vm->_dialogs->show(41021);
	else if (_action.isAction(VERB_TAKE, 0x277))
		_vm->_dialogs->show(41022);
	else if (_action.isAction(VERB_LOOK, 0x272))
		_vm->_dialogs->show(41023);
	else if (_action.isAction(VERB_TAKE, 0x272))
		_vm->_dialogs->show(41024);
	else if (_action.isAction(VERB_LOOK, 0x48) && _game._objects.isInRoom(OBJ_CHARGE_CASES))
		_vm->_dialogs->show(41025);
	else if (_action.isAction(VERB_LOOK, 0x2C3))
		_vm->_dialogs->show(41027);
	else if (_action.isAction(VERB_LOOK, 0x47D))
		_vm->_dialogs->show(41028);
	else if (_action.isAction(VERB_LOOK, 0x275))
		_vm->_dialogs->show(41029);
	else if (_action.isAction(VERB_TAKE, 0x275))
		_vm->_dialogs->show(41030);
	else if (_action.isAction(VERB_THROW, 0x275))
		_vm->_dialogs->show(41031);
	else if (_action._lookFlag)
		_vm->_dialogs->show(41033);
	else
		return;

	_action._inProgress = false;
}

/*------------------------------------------------------------------------*/

} // End of namespace Nebular
} // End of namespace MADS
