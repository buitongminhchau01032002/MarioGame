#include "AttackBlock.h"
#include "Koopa.h"
#include "PlayScene.h"
#include "Chomper.h"
#include "ChomperSmall.h"
#include "LightBrick.h"
#include "QuestionBox.h"
#include "Label.h"


void CAttackBlock::AddLabel(int value) {
	LPPLAYSCENE s = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
	vector<LPGAMEOBJECT>& objects = s->GetObjects();
	objects.push_back(new CLabel(x, y, value));
}

void CAttackEffect::Render() {
	LPSPRITE sprite = CSprites::GetInstance()->Get(ID_SPRITE_ATTACK_EFFECT);
	sprite->Draw(x, y);
}

void CAttackBlock::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x - ATTACK_BLOCK_WIDTH / 2;
    top = y - ATTACK_BLOCK_HEIGHT/2 + ATTACK_BLOCK_OFFSET_Y;
    right = left + ATTACK_BLOCK_WIDTH;
    bottom = top + ATTACK_BLOCK_HEIGHT;
}

void CAttackBlock::AddEffect() {
	LPPLAYSCENE s = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
	vector<LPGAMEOBJECT>& objects = s->GetObjects();
	if (nx > 0) {
		objects.push_back(new CAttackEffect(x + ATTACK_BLOCK_WIDTH, y + ATTACK_BLOCK_OFFSET_Y));
	}
	else if (nx < 0) {
		objects.push_back(new CAttackEffect(x - ATTACK_BLOCK_WIDTH, y + ATTACK_BLOCK_OFFSET_Y));
	}
}

void CAttackBlock::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (GetTickCount64() - timer > ATTACK_BLOCK_DURATION) {
        this->Delete();
        return;
    }
	// Check overla, NOT collision
	LPPLAYSCENE s = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
	vector<LPGAMEOBJECT>& objects = s->GetObjects();
	for (int i = 0; i < objects.size(); i++) {
		float obj_l, obj_t, obj_r, obj_b;
		objects[i]->GetBoundingBox(obj_l, obj_t, obj_r, obj_b);
		if (!IsOverLap(obj_l, obj_t, obj_r, obj_b)) continue;

		if (dynamic_cast<CGoomba*>(objects[i])) OnOverlapWithGoomba(objects[i]);
		else if (dynamic_cast<CKoopa*>(objects[i])) OnOverlapWithKoopa(objects[i]);
		else if (dynamic_cast<CChomper*>(objects[i])) OnOverlapWithChomper(objects[i]);
		else if (dynamic_cast<CChomperSmall*>(objects[i])) OnOverlapWithChomperSmall(objects[i]);
		else if (dynamic_cast<CLightBrick*>(objects[i])) OnOverlapWithLightBrick(objects[i]);
		else if (dynamic_cast<CQuestionBox*>(objects[i])) OnOverlapWithQuestionBox(objects[i]);
	}
}
void CAttackBlock::Render()
{
    //RenderBoundingBox();
}

bool CAttackBlock::IsOverLap(float l, float t, float r, float b) {
	float attack_l, attack_t, attack_r, attack_b;
	this->GetBoundingBox(attack_l, attack_t, attack_r, attack_b);
	if (attack_l < r && attack_r > l && attack_b > t && attack_t < b) {
		return true;
	}
	return false;
}

void CAttackBlock::OnOverlapWithGoomba(LPGAMEOBJECT obj) {
	CGoomba* goomba = dynamic_cast<CGoomba*>(obj);
	if (goomba->GetState() != GOOMBA_STATE_DIE && goomba->GetState() != GOOMBA_STATE_DIE_STRONG) {
		this->AddEffect();
		goomba->SetState(GOOMBA_STATE_DIE_STRONG);
		goomba->SetSpeed(nx * GOOMBA_WALKING_SPEED, -GOOMBA_DEFLECT_SPEED_Y);
		AddLabel(LABEL_100);
	}
}
void CAttackBlock::OnOverlapWithKoopa(LPGAMEOBJECT obj) {
	CKoopa* koopa = dynamic_cast<CKoopa*>(obj);
	if (koopa->GetState() != KOOPA_STATE_SLEEP) {
		if (koopa->GetState() != KOOPA_STATE_SLEEPING) {
			AddLabel(LABEL_100);
		}
		this->AddEffect();
		koopa->SetState(KOOPA_STATE_SLEEPING);
		koopa->SetSpeed(nx*KOOPA_WALKING_SPEED, -KOOPA_DEFLECT_SPEED);
		koopa->SetNy(-1);
		
	}
}
void CAttackBlock::OnOverlapWithChomper(LPGAMEOBJECT obj) {
	CChomper* chomper = dynamic_cast<CChomper*>(obj);
	float chomper_show_l, chomper_show_t, chomper_show_r, chomper_show_b;
	chomper->GetShowBox(chomper_show_l, chomper_show_t, chomper_show_r, chomper_show_b);
	if (IsOverLap(chomper_show_l, chomper_show_t, chomper_show_r, chomper_show_b)) {
		if (chomper->GetState() != CHOMPER_STATE_DIE) {
			this->AddEffect();
			AddLabel(LABEL_100);
			chomper->SetState(CHOMPER_STATE_DIE);
		}
	}
}

void CAttackBlock::OnOverlapWithChomperSmall(LPGAMEOBJECT obj) {
	CChomperSmall* chomper = dynamic_cast<CChomperSmall*>(obj);
	float chomper_show_l, chomper_show_t, chomper_show_r, chomper_show_b;
	chomper->GetShowBox(chomper_show_l, chomper_show_t, chomper_show_r, chomper_show_b);
	if (IsOverLap(chomper_show_l, chomper_show_t, chomper_show_r, chomper_show_b)) {
		if (chomper->GetState() != CHOMPER_SMALL_STATE_DIE) {
			this->AddEffect();
			AddLabel(LABEL_100);
			chomper->SetState(CHOMPER_SMALL_STATE_DIE);
		}
	}
}

void CAttackBlock::OnOverlapWithLightBrick(LPGAMEOBJECT obj) {
	CLightBrick* lightbrick = dynamic_cast<CLightBrick*>(obj);
	lightbrick->Break();
}

void CAttackBlock::OnOverlapWithQuestionBox(LPGAMEOBJECT obj) {
	CQuestionBox* questionBox = dynamic_cast<CQuestionBox*>(obj);
	questionBox->Unbox();
}