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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *
 * Based on the original sources
 *   Faery Tale II -- The Halls of the Dead
 *   (c) 1993-1996 The Wyrmkeep Entertainment Co.
 */

#ifndef SAGA2_MISSION_H
#define SAGA2_MISSION_H

#include "saga2/objects.h"
#include "saga2/actor.h"
#include "saga2/savefile.h"

namespace Saga2 {

class ActiveMission {

	friend void initMissions(void);
	friend void saveMissions(SaveFileConstructor &saveGame);
	friend void loadMissions(SaveFileReader &saveGame);
	friend void cleanupMissions(void);

private:

	//  Used to store a record of mission knowledge that
	//  has been added to an actor, and will be deleted after
	//  the mission is destructed.
	struct KnowledgeID {
		ObjectID    id;
		uint16      kID;
	};

	//  Store the unique ID of this active mission, and the
	//  object ID of the generator.
	uint16          missionID;          // ID of this instance
	ObjectID        generatorID;        // ObjectID of generator instance
	uint16          missionScript;      // script for this mission
	uint16          missionFlags;       // various mission flags

	//  Mission flags
	enum missionFlags {
		inUse = (1 << 0),               // this mission struct in use
	};

	//  Specific variables relating to the mission which can
	//  be defined by the script writer.
	uint8           missionVars[ 32 ];

	//  Record what resources were created for this mission
	ObjectID        missionObjectList[ 32 ];
	KnowledgeID     missionKnowledgeList[ 32 ];
	uint16          numObjectIDs,
	                numKnowledgeIDs;

public:
	static ActiveMission *newMission(ObjectID genID, uint16 script);
	static int  findMission(ObjectID genID);
	static ActiveMission *missionAddress(int index);

	void cleanup(void);

	bool spaceForObject(void) {
		return numObjectIDs < elementsof(missionObjectList);
	}

	//  Add record of object creation to mission
	bool addObjectID(ObjectID objID);

	//  Add record of object creation to mission
	bool removeObjectID(ObjectID objID);

	//  Add record of knowledge creation to mission
	bool addKnowledgeID(ObjectID actor, uint16 knowledgeID);

	//  Add record of knowledge creation to mission
	bool removeKnowledgeID(ObjectID actor, uint16 knowledgeID);

	int16 getMissionID(void) {
		return missionID;
	}

	uint16 getScript(void) {
		return missionScript;
	}
};

//  Initialize the active mission list
void initMissions(void);

//  Save the active missions
void saveMissions(SaveFileConstructor &saveGame);

//  Restore the active missions
void loadMissions(SaveFileReader &saveGame);

//  Cleanup the active mission list
inline void cleanupMissions(void) { /* do nothing */ }

} // end of namespace Saga2

#endif
