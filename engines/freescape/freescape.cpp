#include "common/scummsys.h"

#include "common/config-manager.h"
#include "common/debug-channels.h"
#include "common/debug.h"
#include "common/error.h"
#include "common/events.h"
#include "common/file.h"
#include "common/fs.h"
#include "common/system.h"

#include "engines/util.h"

#include "graphics/renderer.h"

#include "freescape/freescape.h"
#include "freescape/gfx.h"

#include "freescape/loaders/16bitBinaryLoader.h"
#include "freescape/loaders/8bitBinaryLoader.h"


namespace Freescape {

FreescapeEngine *g_freescape = NULL;

FreescapeEngine::FreescapeEngine(OSystem *syst)
	: Engine(syst), _screenW(320), _screenH(200), _border(nullptr), _gfx(nullptr) {
	// Put your engine in a sane state, but do nothing big yet;
	// in particular, do not load data from files; rather, if you
	// need to do such things, do them from run().

	g_freescape = this;
	// Do not initialize graphics here
	// Do not initialize audio devices here
	_hasReceivedTime = false;

	_rotation = Math::Vector3d(0.f, 0.f, 0.f);
	_position = Math::Vector3d(0.f, 0.f, 0.f);
	_velocity = Math::Vector3d(0.f, 0.f, 0.f);
	_cameraFront = Math::Vector3d(0.f, 0.f, 0.f);
	_cameraRight = Math::Vector3d(0.f, 0.f, 0.f);
	_movementSpeed = 4.5f;
	_mouseSensitivity = 0.1f;
	_scale = Math::Vector3d(0, 0, 0);
	_borderTexture = nullptr;

	// Here is the right place to set up the engine specific debug channels
	DebugMan.addDebugChannel(kFreescapeDebug, "example", "this is just an example for a engine specific debug channel");
	DebugMan.addDebugChannel(kFreescapeDebug2, "example2", "also an example");

	// Don't forget to register your random source
	_rnd = new Common::RandomSource("freescape");

	debug("FreescapeEngine::FreescapeEngine");
}

FreescapeEngine::~FreescapeEngine() {
	debug("FreescapeEngine::~FreescapeEngine");

	// Dispose your resources here
	delete _rnd;
	//delete _areasByAreaID;
	delete _border;
	delete _gfx;

	// Remove all of our debug levels here
	//DebugMan.clearAllDebugChannels();
}

void FreescapeEngine::drawBorder() {
	if (_border == nullptr)
		return;

	if (!_borderTexture)
		_borderTexture = _gfx->createTexture(_border);
	const Common::Rect rect(0, 0, _screenW, _screenH);

	_gfx->drawTexturedRect2D(rect, rect, _borderTexture, 1.0, false);
	// _gfx->flipBuffer();
	// _system->updateScreen();
	// _gfx->freeTexture(t);
}

void FreescapeEngine::loadAssets() {
	Common::String renderMode = "";
	Common::SeekableReadStream *file = nullptr;
	Common::String path = ConfMan.get("path");
	Common::FSDirectory gameDir(path);

	if (_targetName.hasPrefix("3dkit") || _targetName == "menace") {
		Common::ArchiveMemberList files;
        gameDir.listMatchingMembers(files, "*.RUN");

		if (files.size() == 0) {
			error("No .RUN was found in %s", path.c_str());
		} else if (files.size() > 1) {
			warning("More than one .RUN file found, only the first one will be used!");
		}

		file = files.begin()->get()->createReadStream();
		load16bitBinary(file);
	} else if (_targetName == "Driller") {
		if (!ConfMan.hasKey("render_mode"))
			renderMode = "ega";
		else
			renderMode = ConfMan.get("render_mode");

		Common::File exe;
		debug("renderMode: %s", renderMode.c_str());
		bool success = false;
		if (renderMode == "ega") {
			file = gameDir.createReadStreamForMember("DRILLE.EXE");

			if (file == nullptr)
				error("Failed to open DRILLE.EXE");

			load8bitBinary(file, 0x9b40, 16);
		} else if (renderMode == "cga") {
			file = gameDir.createReadStreamForMember("DRILLC.EXE");

			if (file == nullptr)
				error("Failed to open DRILLC.EXE");
			load8bitBinary(file, 0x7bb0, 4);
		} else
			error("Invalid render mode %s for Driller", renderMode.c_str());

	   } else if (_targetName == "Castle") {
			error("Unsupported game");
	   } else
		error("'%s' is an invalid game", _targetName.c_str());

}

void FreescapeEngine::drawFrame(Area *area) {
	_gfx->updateProjectionMatrix(60.0, _nearClipPlane, _farClipPlane);
	_gfx->positionCamera(_position, _position + _cameraFront);
	_gfx->scale(_scaleVector);
	area->draw(_gfx);
	//drawBorder();
	_gfx->flipBuffer();
}

void FreescapeEngine::processInput() {
	float currentFrame = g_system->getMillis();
	float deltaTime = currentFrame - _lastFrame;
	_lastFrame = currentFrame;
	Common::Event event;
	while (g_system->getEventManager()->pollEvent(event)) {
		Common::Point mousePos = g_system->getEventManager()->getMousePos();

		switch (event.type) {
		case Common::EVENT_KEYDOWN:
			if (event.kbd.keycode == Common::KEYCODE_w || event.kbd.keycode == Common::KEYCODE_UP)
				move(FORWARD, _scaleVector.x(), deltaTime);
			else if (event.kbd.keycode == Common::KEYCODE_s || event.kbd.keycode == Common::KEYCODE_DOWN)
				move(BACKWARD, _scaleVector.x(), deltaTime);
			else if (event.kbd.keycode == Common::KEYCODE_a || event.kbd.keycode == Common::KEYCODE_LEFT)
				move(LEFT, _scaleVector.y(), deltaTime);
			else if (event.kbd.keycode == Common::KEYCODE_d || event.kbd.keycode == Common::KEYCODE_RIGHT)
				move(RIGHT, _scaleVector.y(), deltaTime);
			break;

		case Common::EVENT_QUIT:
		case Common::EVENT_RETURN_TO_LAUNCHER:
			quitGame();
			return;
			break;

		case Common::EVENT_MOUSEMOVE:
			rotate(_lastMousePos, mousePos);
			_lastMousePos = mousePos;
			if (mousePos.x <= 5 || mousePos.x >= 315) {
				g_system->warpMouse(_screenW/2, mousePos.y);
				_lastMousePos.x = _screenW/2;
				_lastMousePos.y = mousePos.y;
			}
			break;
		default:
			break;

		}
	}

}

Common::Error FreescapeEngine::run() {
	// Initialize graphics:
	_gfx = Freescape::createRenderer(_system);
	_gfx->init();
	_gfx->clear();
	loadAssets();
	Area *area = nullptr;
	Entrance *entrance = nullptr;
	assert(_areasByAreaID);
	if (_startArea == 14)
		_startArea = 1;
	assert(_areasByAreaID->contains(_startArea));
	area = (*_areasByAreaID)[_startArea];
	assert(area);
	entrance = (Entrance*) area->entranceWithID(_startEntrance);
	area->show();
	assert(entrance);
	_position = entrance->getOrigin();
	if (_scale == Math::Vector3d(0, 0, 0)) {
		uint8 scale = area->getScale();
		_scaleVector = Math::Vector3d(scale, scale, scale);
	} else
		_scaleVector = _scale;
	debug("entrace position: %f %f %f", _position.x(), _position.y(), _position.z());
	debug("player height: %d", _playerHeight);
	_position.setValue(1, _position.y() + _playerHeight);

	Math::Vector3d rotation = entrance->getRotation();
	_pitch = rotation.x() - 180.f;
	_yaw = rotation.y() - 180.f;

	debug("FreescapeEngine::init");
	// Simple main event loop
	_lastMousePos = Common::Point(0, 0);
	_lastFrame = 0.f;
	// used to create a projection matrix;
	_nearClipPlane = 1.f;

	if (_binaryBits == 16) {
		// the 16-bit kit permits the range 0-8192 to be used along all three axes and from that comes the far plane distance of 14189.
		_farClipPlane = 14189.f;
	} else {
		_farClipPlane = 4096.f; // wild guess
	}

	//g_system->lockMouse(true);

	while (!shouldQuit()) {
		processInput();
		drawFrame(area);
		g_system->updateScreen();
		g_system->delayMillis(10);
	}

	return Common::kNoError;
}

void FreescapeEngine::rotate(Common::Point lastMousePos, Common::Point mousePos) {
	//debug("x: %d, y: %d", mousePos.x, mousePos.y);
	float xoffset = mousePos.x - lastMousePos.x;
	float yoffset = mousePos.y - lastMousePos.y;

	xoffset *= _mouseSensitivity;
	yoffset *= _mouseSensitivity;

	_yaw += xoffset;
	_pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (_pitch > 180.0f)
		_pitch = 180.0f;
	if (_pitch < -180.0f)
		_pitch = -180.0f;

	Math::Vector3d v;
	float x = cos(_yaw  * M_PI / 180.0) * cos(_pitch  * M_PI / 180.0);
	float y = sin(_pitch * M_PI / 180.0);
	float z = sin(_yaw * M_PI / 180.0) * cos(_pitch * M_PI / 180.0);
	v.set(x, y, z);
	v.normalize();
	_cameraFront = v;

	// // _right = _front x _up;
	Math::Vector3d up(0, 1, 0); // this should be const
	v = Math::Vector3d::crossProduct(_cameraFront, up);
	v.normalize();
	_cameraRight = v;
}

void FreescapeEngine::move(CameraMovement direction, uint8 scale, float deltaTime) {
	float velocity = _movementSpeed * deltaTime;
	float positionY = _position.y();
	switch (direction) {
	case FORWARD:
		_position = _position + _cameraFront * velocity;
		break;
	case BACKWARD:
		_position = _position - _cameraFront * velocity;
		break;
	case RIGHT:
		_position = _position + _cameraRight * velocity;
		break;
	case LEFT:
		_position = _position - _cameraRight * velocity;
		break;
	}
	// Make sure the user stays at the ground level
	// this one-liner keeps the user at the ground level (xz plane)
	_position.set(_position.x(), positionY, _position.z());
	debug("player position: %f, %f, %f", _position.x(), _position.y(), _position.z());
}

bool FreescapeEngine::hasFeature(EngineFeature f) const {
	return (f == kSupportsReturnToLauncher) ||
		   (f == kSupportsLoadingDuringRuntime) ||
		   (f == kSupportsSavingDuringRuntime);
}

Common::Error FreescapeEngine::loadGameStream(Common::SeekableReadStream *stream) {
	Common::Serializer s(stream, nullptr);
	syncGameStream(s);
	return Common::kNoError;
}

Common::Error FreescapeEngine::saveGameStream(Common::WriteStream *stream, bool isAutosave) {
	Common::Serializer s(nullptr, stream);
	syncGameStream(s);
	return Common::kNoError;
}

void FreescapeEngine::syncGameStream(Common::Serializer &s) {
	// Use methods of Serializer to save/load fields
	int dummy = 0;
	s.syncAsUint16LE(dummy);
}

} // namespace Freescape
