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

#ifndef SAGA2_ACTOR_H
#define SAGA2_ACTOR_H

#include "saga2/objects.h"
#include "saga2/tile.h"
#include "saga2/effects.h"

namespace Saga2 {

#ifdef _WIN32   //  Set structure alignment packing value to 1 byte
#pragma pack( push, 1 )
#endif

/* ===================================================================== *
   Constants
 * ===================================================================== */

class ActorAssignment;
class Band;
class MotionTask;
class TaskStack;

const int   bandingDist = sectorSize * 2;
const int   actorScriptVars = 4;

/* ===================================================================== *
   Actor character attributes
 * ===================================================================== */

//  Defines the colors of MANA

enum ActorSkillID {
	skillIDArchery      =  0,
	skillIDSwordcraft,
	skillIDShieldcraft,
	skillIDBludgeon,
	skillIDThrowing,    //  gone
	skillIDSpellcraft,
	skillIDStealth,     //  gone
	skillIDAgility,
	skillIDBrawn,
	skillIDLockpick,    //  gone
	skillIDPilfer,      //  gone
	skillIDFirstAid,    //  gone
	skillIDSpotHidden,  //  gone
	numSkills,
};

enum ArmorObjectTypes {
	SHIRT_ARMOR = 0,
	BOOT_ARMOR,
	BRACER_ARMOR,
	HELM_ARMOR,
	NECKLACE_ARMOR,
	CLOAK_ARMOR,
	RING_ARMOR,

	ARMOR_COUNT
};

/* ===================================================================== *
   ArmorAttributes struct
 * ===================================================================== */

struct ArmorAttributes {
	uint8       damageAbsorbtion,
	            damageDivider,
	            defenseBonus;
};

/* ===================================================================== *
   ActorAttributes structure
 * ===================================================================== */

// this enum acts as a layer of indirection for the unioned allSkills
// array.


//  This defines the basic skills possessed by an actor
struct ActorAttributes {
	enum skillInfo {
		skillBasePercent            = 100,
		skillFracPointsPerLevel     = 5,    // this being twenty and levels at 5
		skillLevels                 = 20,   // will make the advancement calc quick & easy
		skillMaxLevel               = skillFracPointsPerLevel * skillLevels,
	};

	enum vitalityInfo {
		vitalityLimit               = 256,
	};

	union {
		struct {

			//  Automatic skills
			uint8       archery,        //  Accuracy of missile weapons
			            swordcraft,     //  Accuracy of bladed melee weapons
			            shieldcraft,    //  Actor's ability to use a shield
			            bludgeon,       //  Accuracy of non-bladed melee weapons
			            throwing,       //  Ability to throw objects accurately
			            spellcraft,     //  Accuracy of spell combat
			            stealth,        //  Ability to remain unnoticed
			            agility,        //  Ability to dodge
			            brawn,          //  Ability to lift, and damage of weapons
			            lockpick;       //  Ability to pick locks

			//  Manual skills
			uint8       pilfer,         //  Ability to "lift" an item
			            firstAid,       //  Ability to heal recent injuries
			            spotHidden;     //  Ability to spot hidden objects


		};

		uint8   allSkills[ numSkills ]; // number of skills
	};


	//  Pad byte for alignment
	int8        pad;

	//  Hit-points
	int16       vitality;

	//  Magic energy
	union {
		struct {
			int16       redMana,
			            orangeMana,
			            yellowMana,
			            greenMana,
			            blueMana,
			            violetMana;
		};

		int16 allManas[ numManas ];
	};

	uint8 &skill(int16 id) {
		return allSkills[ id ];
	}
	int16 &mana(int16 id) {
		return allManas[ id ];
	}

	uint8 getSkillLevel(int16 id) {
		return skill(id) / skillFracPointsPerLevel + 1;
	}
};  // 28 bytes


const int   baseCarryingCapacity = 100;
const int   carryingCapacityBonusPerBrawn = 200 / ActorAttributes::skillLevels;

/* ===================================================================== *
   ResourceActorProtoExtension structure
 * ===================================================================== */

enum combatBehaviorTypes {
	behaviorHungry,
	behaviorCowardly,
	behaviorBerserk,
	behaviorSmart,
};

//  This defines the additional data fields needed for actor prototypes
struct ResourceActorProtoExtension {

	ActorAttributes     baseStats;  //  Base stats for non-player actors

	//  Defines behavior for combat tactics.
	uint8               combatBehavior;
	uint8               gruntStyle;

	uint32              baseEffectFlags;    // special effects, see EFFECTS.H

	//  Default constructor -- do nothing
	ResourceActorProtoExtension(void) {}

	//  Copy constructor
	ResourceActorProtoExtension(ResourceActorProtoExtension &ext) {
		memcpy(this, &ext, sizeof(ResourceActorProtoExtension));
	}
};  // 28 bytes

/* ===================================================================== *
   ResourceActorPrototype structure
 * ===================================================================== */

//  Defines the actor prototype data as read from the resource file
struct ResourceActorPrototype {
	ResourceObjectPrototype         proto;  //  Standard prototype data
	ResourceActorProtoExtension     ext;    //  Extended actor data
};

/* ===================================================================== *
   ActorProto prototype behavior for Actors
 * ===================================================================== */

class ActorProto : public ProtoObj, public ResourceActorProtoExtension {

private:
	enum {
		ViewableRows    = 3,
		ViewableCols    = 3,
		maxRows         = 3,
		maxCols         = 3,
	};

public:
	ActorProto(ResourceActorPrototype &a) :
		ProtoObj(a.proto),
		ResourceActorProtoExtension(a.ext) {
	}

	// returns the containment type flags for this object
	virtual uint16 containmentSet(void);

	//  returns TRUE if this object can contain another object
	virtual bool canContain(ObjectID dObj, ObjectID item);

	//  Determine if this object can contain another object at a
	//  specified slot
	virtual bool canContainAt(
	    ObjectID dObj,
	    ObjectID item,
	    const TilePoint &where);

	weaponID getWeaponID(void);

	//  use this actor
	bool useAction(ObjectID dObj, ObjectID enactor);

	//  open this actor
	bool canOpen(ObjectID dObj, ObjectID enactor);
	bool openAction(ObjectID dObj, ObjectID enactor);

	//  close this actor
	bool closeAction(ObjectID dObj, ObjectID enactor);

	bool strikeAction(
	    ObjectID dObj,
	    ObjectID enactor,
	    ObjectID item);

	bool damageAction(
	    ObjectID dObj,
	    ObjectID enactor,
	    ObjectID target);

	//  drop another object onto this actor.
	bool acceptDropAction(
	    ObjectID dObj,
	    ObjectID enactor,
	    ObjectID droppedObj,
	    int count);

	//  cause damage directly
	bool acceptDamageAction(
	    ObjectID            dObj,
	    ObjectID            enactor,
	    int8                absDamage,
	    effectDamageTypes   dType,
	    int8                dice,
	    uint8               sides,
	    int8                perDieMod);

	//  cause healing directly
	bool acceptHealingAction(ObjectID dObj, ObjectID enactor, int8 healing);

	//  Accept strike from an object (allows this actor to cause
	//  damage to the striking object).
	bool acceptStrikeAction(
	    ObjectID            dObj,
	    ObjectID            enactor,
	    ObjectID            strikingObj,
	    uint8               skillIndex);

	//  Handle the results of an object being inserted into this object
	//  at the specified slot
	bool acceptInsertionAtAction(
	    ObjectID        dObj,
	    ObjectID        enactor,
	    ObjectID        item,
	    const TilePoint &where,
	    int16           num = 1);

	//  Initiate an attack using this type of object
	virtual void initiateAttack(ObjectID attacker, ObjectID target);

	//  Given an object sound effect record, which sound should be made
	//  when this object is damaged
	virtual uint8 getDamageSound(const ObjectSoundFXs &soundFXs);

	//  Do the background processing, if needed, for this object.
	void doBackgroundUpdate(GameObject *obj);

	//  Cause the user's associated skill to grow
	void applySkillGrowth(ObjectID enactor, uint8 points = 1);

	bool greetActor(
	    ObjectID        dObj,                   // object dropped on
	    ObjectID        enactor);               // person doing dropping

public:
	virtual uint16 getViewableRows(void) {
		return ViewableRows;
	}
	virtual uint16 getViewableCols(void) {
		return ViewableCols;
	}
	virtual uint16 getMaxRows(void) {
		return maxRows;
	}
	virtual uint16 getMaxCols(void) {
		return maxCols;
	}

	virtual bool canFitBulkwise(GameObject *container, GameObject *obj);
	virtual bool canFitMasswise(GameObject *container, GameObject *obj);

	virtual uint16 massCapacity(GameObject *container);
	virtual uint16 bulkCapacity(GameObject *container);
};

/* ============================================================================ *
   Actor: Describes an instance of a character
 * ============================================================================ */

enum actorCreationFlags {
	actorPermanent = (1 << 0),
};

enum DispositionType {
	dispositionFriendly,
	dispositionEnemy,

	dispositionPlayer
};

enum actionSequenceOptions {

	//  Flags set by call to setAction
	animateRepeat   = (1 << 0),             // repeat animation when done
	animateReverse  = (1 << 1),             // animate in reverse direction
	animateAlternate = (1 << 2),            // both directions, back & forth
	animateRandom   = (1 << 3),             // pick a random frame
	animateNoRestart = (1 << 4),            // don't reset from start

	//  This flag is set if the animation has been put on hold until
	//  the actor's appearance is reloaded.
	animateOnHold   = (1 << 5),

	//  This flag is set if the final frame of the animation has
	//  been reached.
	animateFinished = (1 << 6),

	//  This flag gets set if the sprite could not be displayed
	//  because it's bank hasn't been loaded yet.
	animateNotLoaded = (1 << 7),
};

//  Various types of action sequences


enum ActorAnimationTypes {

	//  Various types of stands
	actionStand = 0,                        // standing still
	actionWaitAgressive,                    // an agressive wait cycle
	actionWaitImpatient,                    // an impatient wait cycle
	actionWaitFriendly,                     // a a friendly wait cycle

	//  Walking and running
	actionWalk,                             // walking motion
	actionRun,                              // running motion

	//  Squatting
	actionDuck,                             // stoop to dodge sword
	actionStoop,                            // stoop to pick up object

	//  Jumping
	actionFreeFall,                         // how he looks in ballistic
	actionFreeFallRunning,                  // a running leap (free fall)
	actionJumpUp,                           // begin jump straight up
	actionJumpFwd,                          // begin jump forward
	actionJumpBack,                         // begin jump back in surprise
	actionLand,                             // land after jump
	actionFallBadly,                        // after a very long fall

	//  Climbing
	actionClimbLadder,                      // climb a ladder (2 directions)

	//  Talking & interacting
	actionTalk,                             // talking
	actionGesture,                          // gesture with hands or body
	actionGiveItem,                         // give or take item

	//  Two-handed weapon use
	actionTwoHandSwingHigh,                 // full overhead swing aim high
	actionTwoHandSwingLow,                  // full overhead swing aim low
	actionTwoHandSwingLeftHigh,             // partial swing on left (high)
	actionTwoHandSwingLeftLow,              // partial swing on left (low)
	actionTwoHandSwingRightHigh,            // partial swing on rgt (high)
	actionTwoHandSwingRightLow,             // partial swing on rgt (low)
	actionTwoHandParry,                     // hold sword up to parry

	//  One-handed weapon use
	actionSwingHigh,                        // one-handed swing (high)
	actionSwingLow,                         // one-handed swing (low)
	actionParryHigh,                        // one-handed parry (high)
	actionParryLow,                         // one-handed parry (low)
	actionShieldParry,                      // parry with shield

	actionThrowObject,                      // throw

	//  Other combat actions
	actionFireBow,                          // fire an arrow
	actionCastSpell,                        // cast a magic spell
	actionUseWand,                          // cast a magic spell w/wand
	actionUseStaff,                         // cast a magic spell w/staff
	actionHit,                              // show impact of blow
	actionKnockedDown,                      // knocked down by opponent
	actionDie,                              // death agony

	//  Passive actions
	actionSleep,                            // sleeping
	actionDead,                             // dead body on ground
	actionSit,                              // sitting at table

	//  Misc actions built from other frames
	actionListenAtDoor,                     // listening at doors
	actionShoveDoor,                        // try to force a door open
	actionSpecial1,                         // special Action
	actionSpecial2,                         // special Action
	actionSpecial3,                         // special Action
	actionSpecial4,                         // special Action
	actionSpecial5,                         // special Action
	actionSpecial6,                         // special Action
	actionSpecial7,                         // special Action
	actionSpecial8,                         // special Action
};

enum ActorGoalTypes {
	actorGoalFollowAssignment,
	actorGoalPreserveSelf,
	actorGoalAttackEnemy,
	actorGoalFollowLeader,
	actorGoalAvoidEnemies,
};

//  The actor structure will be divided into two parts.  The
//  ResourceActor structure defines the data as it is stored in the
//  resource file.  The Actor structure has a copy of all of the
//  ResourceActor data members, plus data members which will be
//  initialized and used during run time.

struct ResourceActor : public ResourceGameObject {

	//  Social loyalty
	uint8           faction;                // actor's faction

	//  Appearance attribute
	uint8           colorScheme;            // indirect color map

	int32           appearanceID;           // appearnce of this actor

	//  Personality attributes
	int8            attitude,               // cooperativeness
	                mood;                   // happiness
	uint8           disposition;            // actor disposition
	// 0 = friendly, 1 = enemy,
	// 2 = Julian, 3 = Philip,
	// 4 = Kevin

	//  Character orientation
	Direction       currentFacing;          // current facing direction

	//  Tether info
	int16           tetherLocU;             // tether U coordinate
	int16           tetherLocV;             // tether V coordinate
	int16           tetherDist;             // length of tether

	//  Held objects
	ObjectID        leftHandObject,         // object held in left hand.
	                rightHandObject;        // object held in right hand.

	//  Knowledge packets
	uint16          knowledge[ 16 ];

	//  Schedule script ID
	uint16          schedule;

	//  Pad bytes
	uint8           reserved[ 18 ];

};

class Actor : public GameObject {
	friend class    ActorProto;
	friend class    MotionTask;
	friend class    Task;
	friend class    TaskStack;

public:

	//  Resource fields

	//  Social loyalty
	uint8           faction;                // actor's faction

	//  Appearance attribute
	uint8           colorScheme;            // indirect color map

	int32           appearanceID;           // appearnce of this actor

	//  Personality attributes
	int8            attitude,               // cooperativeness
	                mood;                   // happiness
	uint8           disposition;            // actor disposition
	// 0 = friendly, 1 = enemy,
	// 2 = Julian, 3 = Philip,
	// 4 = Kevin

	//  Character orientation
	Direction       currentFacing;          // current facing direction

	//  Tether info
	int16           tetherLocU;             // tether U coordinate
	int16           tetherLocV;             // tether V coordinate
	int16           tetherDist;             // length of tether

	//  Held objects
	ObjectID        leftHandObject,         // object held in left hand.
	                rightHandObject;        // object held in right hand.

	//  Knowledge packets
	uint16          knowledge[ 16 ];

	//  Schedule script ID
	uint16          schedule;

	//  Run-time fields

	uint8           conversationMemory[ 4 ];// last things talked about

	//  Sprite animation variables
	uint8           currentAnimation,       // current action sequence
	                currentPose,            // current pose in sequence
	                animationFlags;         // current posing flags

	//  Various actor flags
	enum {
		lobotomized     = (1 << 0),
		temporary       = (1 << 1),
		afraid          = (1 << 2),
		hasAssignment   = (1 << 3),
		specialAttack   = (1 << 4),
		fightStance     = (1 << 5),
	};

	uint8           flags;

	//  Contains sprite index and positioning info for the current
	//  actor state.
	ActorPose       poseInfo;               // current animation state

	//  Pointer to the appearance record (sprite array) for this actor.
	ActorAppearance *appearance;            // appearance structs

	int16           cycleCount;             // misc counter for actions
	int16           kludgeCount;            // another misc counter

	uint32          enchantmentFlags;       // flags indicating racial
	// abilities and enchantments

	//  Movement attributes
	MotionTask      *moveTask;

	//  Current task
	TaskStack       *curTask;

	//  Current goal type
	uint8           currentGoal;

	//  Used for deltayed deactivation (and also to word-align struct)
	uint8           deactivationCounter;

	//  Assignment
	uint8           assignmentBuf[ 24 ];    // memory reserved for actor
	// assignments

	//  Current effective stats
	ActorAttributes effectiveStats;

	uint8           actionCounter;          //  coordinate moves in combat

	uint16          effectiveResistance;    // resistances (see EFFECTS.H)
	uint16          effectiveImmunity;      // immunities  (see EFFECTS.H)

	int16           recPointsPerUpdate;     // fractional vitality recovery

	int16           currentRecoveryPoints;  // fraction left from last recovery

	enum vitalityRecovery {
		recPointsPerVitality        = 10,
	};


	Actor           *leader;                // This actor's leader
	Band            *followers;             // This actor's band of followers

	ObjectID        armorObjects[ ARMOR_COUNT ];    //  armor objects being worn

	GameObject      *currentTarget;

	int16           scriptVar[ actorScriptVars ];   //  scratch variables for scripter use

	//  Member functions

private:
	//  Initialize actor record
	void init(
	    int16   protoIndex,
	    uint16  nameIndex,
	    uint16  scriptIndex,
	    int32   appearanceNum,
	    uint8   colorSchemeIndex,
	    uint8   factionNum,
	    uint8   initFlags);

public:
	//  Default constructor
	Actor(void) {}

	//  Constructor - initial actor construction
	Actor(const ResourceActor &res);

	//  Reconstruct from archive buffer
	Actor(void **buf);

	//  Destructor
	~Actor(void);

	//  Return the number of bytes needed to archive this actor
	int32 archiveSize(void);

	//  Archive this actor in a buffer
	void *archive(void *buf);

	static Actor *newActor(
	    int16   protoNum,
	    uint16  nameIndex,
	    uint16  scriptIndex,
	    int32   appearanceNum,
	    uint8   colorSchemeIndex,
	    uint8   factionNum,
	    uint8   initFlags);

	//  Delete this actor
	void deleteActor(void);

private:
	//  Turn incrementally
	void turn(Direction targetDir) {
		Direction   relativeDir = (targetDir - currentFacing) & 0x7;

		currentFacing =
		    (relativeDir < 4
		     ?   currentFacing + 1
		     :   currentFacing - 1)
		    &   0x7;
	}
public:

	//  Cause the actor to stop his current motion task is he is
	//  interruptable
	void stopMoving(void);

	//  Cause this actor to die
	void die(void);

	//  Cause this actor to return from the dead
	void imNotQuiteDead(void);

	// makes the actor do a vitality change test
	void vitalityUpdate(void);

	//  Perform actor specific activation tasks
	void activateActor(void);

	//  Perform actor specific deactivation tasks
	void deactivateActor(void);

	//  De-lobotomize this actor
	void delobotomize(void);

	//  Lobotomize this actor
	void lobotomize(void);

	//  Allocate the assignment buffer for a new assignment
	void *allocAssignment(size_t sz) {
		ASSERT(sz <= sizeof(assignmentBuf));
		if (!(flags & hasAssignment)) {
			flags |= hasAssignment;
			return &assignmentBuf;
		} else
			return NULL;
	}

	//  Free the emmbedded assignment buffer
	void freeAssignment(void) {
		flags &= ~hasAssignment;
	}

	//  Return a pointer to the actor's current assignment
	ActorAssignment *getAssignment(void) {
		return  flags & hasAssignment
		        ? (ActorAssignment *)&assignmentBuf
		        :   NULL;
	}

	//  determine wether this actor has a specified property
	bool hasProperty(const ActorProperty &actorProp) {
		//  The function call operator is used explicitly because
		//  Visual C++ 4.0 doesn't like it otherwise.
		return actorProp.operator()(this);
	}

	//  Determine if specified point is within actor's arms' reach
	bool inReach(const TilePoint &tp);

	//  Determine if specified point is within an objects use range
	bool inUseRange(const TilePoint &tp, GameObject *obj);

	//  Determine if actor is dead
	bool isDead(void) {
		return effectiveStats.vitality <= 0;
	}

	//  Determine if actor is immobile (i.e. can't walk)
	bool isImmobile(void);

	//  Return a pointer to this actor's currently readied offensive
	//  object
	GameObject *offensiveObject(void);

	//  Returns pointers to this actor's readied primary defensive object
	//  and optionally their scondary defensive object
	void defensiveObject(GameObject **priPtr, GameObject **secPtr = NULL);

	//  Returns a pointer to the object with which this actor is
	//  currently blocking, if any
	GameObject *blockingObject(Actor *attacker);

	//  Return the total used armor attributes
	void totalArmorAttributes(ArmorAttributes &armorAttribs);

	//  Determine if specified point is within actor's attack range
	bool inAttackRange(const TilePoint &tp);

	//  Attack the specified object with the currently selected weapon
	void attack(GameObject *obj);

	//  Stop any attack on the specified object
	void stopAttack(GameObject *obj);

	//  Determine if this actor can block an attack with objects
	//  currently being held
	bool canDefend(void);

	//  Return a numeric value which roughly estimates this actor's
	//  offensive strength
	int16 offenseScore(void);

	//  Return a numeric value which roughly estimates this actor's
	//  defensive strenght
	int16 defenseScore(void);

	//  Handle the effect of a successful hit on an opponent in combat
	void handleSuccessfulStrike(GameObject *weapon) {
		weapon->proto()->applySkillGrowth(thisID());
	}

	//  Return the value of this actor's disposition
	int16 getDisposition(void) {
		return disposition;
	}

	//  Give the actor a new disposition
	int16 setDisposition(int16 newDisp) {
		int16   oldDisp = disposition;
		if (newDisp < dispositionPlayer) disposition = newDisp;
		return oldDisp;
	}

	//  Return a pointer to the effective stats
	ActorAttributes *getStats(void) {
		return &effectiveStats;
	}

	//  Return a pointer to this actor's base stats
	ActorAttributes *getBaseStats(void);

	//  Return the color remapping table
	void getColorTranslation(ColorTable map);

	//  Determine if this actor is interruptable
	bool isInterruptable(void) {
		return actionCounter == 0;
	}

	//  Determine if this actor is permanently uninterruptable
	bool isPermanentlyUninterruptable(void) {
		return actionCounter == maxuint8;
	}

	//  Set the inturruptability for this actor
	void setInterruptablity(bool val) {
		actionCounter = val ? 0 : maxuint8;
	}

	//  Set action time counter for this actor
	//  REM: the action points will eventually need to be scaled based
	//  upon enchantments and abilities
	void setActionPoints(uint8 points) {
		actionCounter = points;
	}

	//  Drop the all of the actor's inventory
	void dropInventory(void);

	//  Place an object into this actor's right or left hand
	void holdInRightHand(ObjectID objID);
	void holdInLeftHand(ObjectID objID);

	//  Wear a piece of armor
	void wear(ObjectID objID, uint8 where);

	//  Update the appearance of an actor with no motion task.
	void updateAppearance(int32 deltaTime);

	//  Used To Find Wait State When Preffered Not Available
	bool SetAvailableAction(int16, ...);

	//  Set the current animation sequence that the actor is doing.
	//  Returns the number of poses in the sequence, or 0 if there
	//  are no poses in the sequence.
	int16 setAction(int16 newState, int16 flags);

	//  returns TRUE if the action is available in the current
	//  direction.
	bool isActionAvailable(int16 newState, bool anyDir = FALSE);

	//  Return the number of animation frames in the specified action
	//  for the specified direction
	int16 animationFrames(int16 actionType, Direction dir);

	//  Update the current animation sequence to the next frame
	bool nextAnimationFrame(void);

	//  calculate which sprite frames to show. Return FALSE if
	//  sprite frames are not loaded.
	bool calcSpriteFrames(void);

	//  Calculate the frame list entry, given the current actor's
	//  body state, and facing direction.
//	FrameListEntry   *calcFrameState( int16 bodyState );

	//  Returns 0 if not moving, 1 if path being calculated,
	//  2 if path being followed.
	bool pathFindState(void);

	//  High level actor behavior functions
private:
	void setGoal(uint8 newGoal);

public:
	void evaluateNeeds(void);

	//  Called every frame to update the state of this actor
	void updateState(void);

	void handleTaskCompletion(TaskResult result);
	void handleOffensiveAct(Actor *attacker);
	void handleDamageTaken(uint8 damage);
	void handleSuccessfulStrike(Actor *target, int8 damage);
	void handleSuccessfulKill(Actor *target);

private:
	static bool canBlockWith(GameObject *defenseObj, Direction relativeDir);
public:
	void evaluateMeleeAttack(Actor *attacker);

	//  Banding related functions
	void bandWith(Actor *newLeader);
	void disband(void);

	bool inBandingRange(void) {
		ASSERT(leader != NULL);

		return      leader->IDParent() == IDParent()
		            && (leader->getLocation() - getLocation()).quickHDistance()
		            <=  bandingDist;
	}

private:
	bool addFollower(Actor *newBandMember);
	void removeFollower(Actor *bandMember);

	TaskStack *createFollowerTask(Actor *bandMember);
	uint8 evaluateFollowerNeeds(Actor *follower);

public:
	//  Knowledge-related member functions
	bool addKnowledge(uint16 kID);
	bool removeKnowledge(uint16 kID);
	void clearKnowledge(void);
	void useKnowledge(scriptCallFrame &scf);

	bool canSenseProtaganistIndirectly(SenseInfo &info, int16 range);
	bool canSenseSpecificActorIndirectly(
	    SenseInfo           &info,
	    int16               range,
	    Actor               *a);
	bool canSenseSpecificObjectIndirectly(
	    SenseInfo           &info,
	    int16               range,
	    ObjectID            obj);
	bool canSenseActorPropertyIndirectly(
	    SenseInfo           &info,
	    int16               range,
	    ActorPropertyID     prop);
	bool canSenseObjectPropertyIndirectly(
	    SenseInfo           &info,
	    int16               range,
	    ObjectPropertyID    prop);

	// Take mana from actor's mana pool (if possible)
	bool takeMana(ActorManaID i, int8 dMana);

	bool hasMana(ActorManaID i, int8 dMana);

	uint32 getBaseEnchantmentEffects(void);
	uint16 getBaseResistance(void);
	uint16 getBaseImmunity(void);
	uint16 getBaseRecovery(void);

	bool resists(effectResistTypes r) {
		return effectiveResistance & (1 << r);
	}
	bool isImmuneTo(effectImmuneTypes r) {
		return effectiveImmunity & (1 << r);
	}
	bool hasEffect(effectOthersTypes e) {
		return (enchantmentFlags & (1 << e)) != 0;
	}

	void setResist(effectResistTypes r, bool on) {
		effectiveResistance = on ?
		                      effectiveResistance | (1 << r) :
		                      effectiveResistance & ~(1 << r);
	}

	void setImmune(effectImmuneTypes r, bool on) {
		effectiveImmunity = on ?
		                    effectiveImmunity | (1 << r) :
		                    effectiveImmunity & ~(1 << r);
	}

	void setEffect(effectOthersTypes e, bool on) {
		enchantmentFlags = on ?
		                   enchantmentFlags | (1 << e) :
		                   enchantmentFlags & ~(1 << e);
	}

	bool makeSavingThrow(void);

	void setFightStance(bool val) {
		if (val)
			flags |= fightStance;
		else
			flags &= ~fightStance;
	}
};

inline bool isPlayerActor(Actor *a) {
	return a->disposition >= dispositionPlayer;
}

inline bool isPlayerActor(ObjectID obj) {
	return  isActor(obj)
	        &&  isPlayerActor((Actor *)GameObject::objectAddress(obj));
}

inline bool isEnemy(Actor *a) {
	return !a->isDead() && a->disposition == dispositionEnemy;
}

inline bool isEnemy(ObjectID obj) {
	return  isActor(obj)
	        &&  isEnemy((Actor *)GameObject::objectAddress(obj));
}

void updateActorStates(void);

void pauseActorStates(void);
void resumeActorStates(void);

void setCombatBehavior(bool enabled);

//  Determine if the actors are currently initialized
bool areActorsInitialized(void);

void clearEnchantments(Actor *a);
void addEnchantment(Actor *a, uint16 enchantmentID);

/* ============================================================================ *
   Actor factions table
 * ============================================================================ */

//  Let's assume 64 factions maximum for now
const int           maxFactions = 64;

enum factionTallyTypes {
	factionNumKills = 0,            // # of times faction member killed by PC
	factionNumThefts,               // # of times PC steals from faction member
	factionNumFavors,               // accumulated by SAGA script.

	factionNumColumns
};

//  Get the attitude a particular faction has for a char.
int16 GetFactionTally(int faction, enum factionTallyTypes act);

//  Increment / Decrement faction attitude
//  Whenever an actor is killed, call:
//      AddFactionAttitude( actor.faction, factionNumKills, 1 );
//  Whenever an actor is robbed, call:
//      AddFactionAttitude( actor.faction, factionNumThefts, 1 );
int16 AddFactionTally(int faction, enum factionTallyTypes act, int amt);

//  Initialize the faction tally table
void initFactionTallies(void);

//  Save the faction tallies to a save file
void saveFactionTallies(SaveFileConstructor &saveGame);

//  Load the faction tallies from a save file
void loadFactionTallies(SaveFileReader &saveGame);

//  Cleanup the faction tally table
inline void cleanupFactionTallies(void) { /* Nothing to do */ }

/* ============================================================================ *
   Magic: Actor Enchantment bits
 * ============================================================================ */

//  These are are all the enchantment effects that can be represented
//  as a single bit.



//    THESE ARE NO LONGER VALID SEE EFFECTS.H

#if 0
enum enchantmentEffects {
	actorFlamingAura        = (1 << 0),     // surrounded by damaging aura
	actorImmunePhysical     = (1 << 1),     // immune to physical weapons
	actorImmuneProjectile   = (1 << 2),     // immune to projectiles
	actorImmuneHandToHand   = (1 << 3),     // immune to non-projectiles
	actorImmuneMagicMissile = (1 << 4),     // immune to magical projectiles
	actorImmuneFire         = (1 << 5),     // immune to fire
	actorImmuneFireMagic    = (1 << 6),     // immune to magical fire
	actorImmuneLava         = (1 << 7),     // immune to hot terrain
	actorImmuneCold         = (1 << 8),     // immune to cold
	actorImmuneMental       = (1 << 9),     // immune to mental attack
	actorImmuneDirectMagic  = (1 << 10),    // immune to direct magic
	actorImmuneLifeDrain    = (1 << 11),    // immune to Life Drain
	actorLandWalking        = (1 << 12),    // can walk on land
	actorDesolidified       = (1 << 13),    // can walk through walls
	actorFloating           = (1 << 14),    // actor is floating
	actorFallSlowly         = (1 << 15),    // protected against falling
	actorLevitating         = (1 << 16),    // actor is levitating
	actorWaterWalking       = (1 << 17),    // can walk on water
	actorFlying             = (1 << 18),    // flying spell
	actorBlind              = (1 << 19),    // actor is blind
	actorPanic              = (1 << 20),    // actor runs away from danger
	actorParalyzed          = (1 << 21),    // actor can not move
	actorMoveFast           = (1 << 22),    // move faster than normal
	actorMoveSlow           = (1 << 23),    // move slower than normal
	actorAttackFast         = (1 << 24),    // attack faster than normal
	actorAttackSlow         = (1 << 25),    // attack slower than normal
	actorAsleep             = (1 << 26),    // actor goes to sleep
	actorSoulSight          = (1 << 27),    // can see invisible
	actorClairvoyant        = (1 << 28),    // player can scroll view
	actorDetectPoison       = (1 << 29),    // poison objects highlighted
	actorInvisible          = (1 << 30),    // no-one can see this actor
	actorHasNoSmell         = (1 << 31),    // can't be tracked
};
#endif

#ifdef _WIN32   //  Reset old structure alignment
#pragma pack( pop )
#endif

} // end of namespace Saga2

#endif
