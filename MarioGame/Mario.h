#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"

#include "debug.h"

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.2f

#define MARIO_ACCEL_WALK_X	0.00018f
#define MARIO_ACCEL_BRACE 0.0005f
#define MARIO_ACCEL_RUN_X	0.0002f

#define MARIO_JUMP_SPEED_Y		0.38f
#define MARIO_SPEED_BOOST_FLY 0.2f
#define MARIO_SPEED_TO_FALL 0.05f // speed min to anim fall
#define MARIO_SPEED_Y_MAX 0.38f
#define MARIO_SPEED_Y_SLOW_MAX 0.036f
#define MARIO_JUMP_DEFLECT_SPEED  0.24f

#define MARIO_GRAVITY			0.0009f
#define MARIO_GRAVITY_FLYING	0.0007f

#define MARIO_RUNNING_DURATION_MAX 1300
#define MARIO_SLOWFALLING_BREAK_TIME 400
#define MARIO_FLYING_DURATION 5000

// STATE
#define MARIO_STATE_DIE				-10
#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601
#define MARIO_STATE_ACTTACK 800

// STATE X
#define MARIO_STATE_X_IDLE 0
#define MARIO_STATE_X_WALKING 100
#define MARIO_STATE_X_RUNNING 101
#define MARIO_STATE_X_WALK_STOPPING 200
#define MARIO_STATE_X_BRACING 300

// STATE Y
#define MARIO_STATE_Y_GROUND 0
#define MARIO_STATE_Y_JUMPING 100
#define MARIO_STATE_Y_FALLING 200
#define MARIO_STATE_Y_SITTING 300
// state cat
#define MARIO_STATE_Y_SLOWFALLING 201
#define MARIO_STATE_Y_FLYING 400


#pragma region ANIMATION_ID

#define ID_ANI_MARIO_IDLE_RIGHT 400
#define ID_ANI_MARIO_IDLE_LEFT 401

#define ID_ANI_MARIO_WALKING_RIGHT 500
#define ID_ANI_MARIO_WALKING_LEFT 501

#define ID_ANI_MARIO_RUNNING_RIGHT 600
#define ID_ANI_MARIO_RUNNING_LEFT 601

#define ID_ANI_MARIO_JUMP_WALK_RIGHT 700
#define ID_ANI_MARIO_JUMP_WALK_LEFT 701

#define ID_ANI_MARIO_JUMP_RUN_RIGHT 800
#define ID_ANI_MARIO_JUMP_RUN_LEFT 801

#define ID_ANI_MARIO_SIT_RIGHT 900
#define ID_ANI_MARIO_SIT_LEFT 901

#define ID_ANI_MARIO_BRACE_RIGHT 1000
#define ID_ANI_MARIO_BRACE_LEFT 1001

#define ID_ANI_MARIO_DIE 999

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1100
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1102

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1200
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1201

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1300
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1301

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1400
#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1401

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 1500
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 1501

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 1600
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 1601

// CAT
#define ID_ANI_MARIO_CAT_IDLE_RIGHT 1700
#define ID_ANI_MARIO_CAT_IDLE_LEFT 1701

#define ID_ANI_MARIO_CAT_WALKING_RIGHT 1800
#define ID_ANI_MARIO_CAT_WALKING_LEFT 1801

#define ID_ANI_MARIO_CAT_RUNNING_RIGHT 1900
#define ID_ANI_MARIO_CAT_RUNNING_LEFT 1901

#define ID_ANI_MARIO_CAT_JUMP_WALK_RIGHT 2000
#define ID_ANI_MARIO_CAT_JUMP_WALK_LEFT 2001

#define ID_ANI_MARIO_CAT_JUMP_RUN_RIGHT 2100
#define ID_ANI_MARIO_CAT_JUMP_RUN_LEFT 2101

#define ID_ANI_MARIO_CAT_SIT_RIGHT 2200
#define ID_ANI_MARIO_CAT_SIT_LEFT 2201

#define ID_ANI_MARIO_CAT_BRACE_RIGHT 2300
#define ID_ANI_MARIO_CAT_BRACE_LEFT 2301

#define ID_ANI_MARIO_CAT_FLYING_RIGHT 2400
#define ID_ANI_MARIO_CAT_FLYING_LEFT 2401

#define ID_ANI_MARIO_CAN_FLY_RIGHT 2500
#define ID_ANI_MARIO_CAN_FLY_LEFT 2501

#define ID_ANI_MARIO_ATTACK_RIGHT 2600
#define ID_ANI_MARIO_ATTACK_LEFT 2601

#pragma endregion




#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_CAT     3

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 12


#define MARIO_UNTOUCHABLE_TIME 2500

// CAT
#define MARIO_FLY_DURATION 5000
#define MARIO_FLY_SPEED 0.15f
#define MARIO_FLY_STARTUP_DURATION 1200
#define MARIO_FLY_BREAK_TIME 500
#define MARIO_ATTACK_EFFECT_DURATION 200

class CMario : public CGameObject
{
	BOOLEAN isSitting;
	bool isFlying;
	float maxVx;
	float maxVy;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 

	int level; 
	int untouchable; 
	ULONGLONG untouchable_start;
	BOOLEAN isOnPlatform;
	int coin;
	ULONGLONG flyingBreakTimerStart;
	int runningDuration;
	int flyingDuration;

	ULONGLONG slowFallingStart;
	ULONGLONG flyingStart;

	bool canFly;

	int stateX;
	int stateY;

	ULONGLONG attackStart;
	

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithGoombaPro(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithQuestionBox(LPCOLLISIONEVENT e);
	void OnCollisionWithMushroom(LPCOLLISIONEVENT e);
	void OnCollisionWithLeaf(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);

	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdCat();

public:
	CMario(float x, float y) : CGameObject(x, y)
	{
		isSitting = false;
		maxVx = 0.0f;
		maxVy = MARIO_SPEED_Y_MAX;
		ax = 0.0f;
		ay = MARIO_GRAVITY; 
		stateX = MARIO_STATE_X_IDLE;
		stateY = MARIO_STATE_Y_GROUND;

		level = MARIO_LEVEL_BIG;
		untouchable = 0;
		untouchable_start = -1;
		isOnPlatform = false;
		coin = 0;
		isFlying = false;
		flyingDuration = 0;
		canFly = false;
		flyingBreakTimerStart = GetTickCount64();
		runningDuration = -1;
		attackStart = 0;
		slowFallingStart = GetTickCount64();
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);

	int IsCollidable()
	{ 
		return (state != MARIO_STATE_DIE); 
	}

	int IsBlocking() { return 0; }
	//int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable==0); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);

	void SetLevel(int l);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void IncreaseLevel();
	void DecreaseLevel();
	int GetLevel() { return level; }
	bool GetCanFly() { return canFly; }
	void Setnx(int n) { nx = n; }
	int Getnx() { return nx; }
	int GetStateX() { return stateX; }
	int GetStateY() { return stateY; }
	void SetStateX(int state);
	void SetStateY(int state);
	bool GetIsFlying() { return isFlying; }
	int getFlyingDuration() { return flyingDuration; }
	bool IsCanFly() { return runningDuration >= MARIO_RUNNING_DURATION_MAX; }
	int GetPowerProgress() {
		return runningDuration * 7 / MARIO_RUNNING_DURATION_MAX;
	}
	void BreakJump() {
		vy = 0;
		SetStateY(MARIO_STATE_Y_FALLING);
	}

	void BoostFly() {
		if (vy > -MARIO_SPEED_BOOST_FLY) {
			vy = -MARIO_SPEED_BOOST_FLY;
		}
	}
};