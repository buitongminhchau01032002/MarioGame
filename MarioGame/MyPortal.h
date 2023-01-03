#pragma once
#include "Portal.h"
#include "Sprites.h"
class CMyPortal : public CPortal
{
private:
	int spriteId;
public:
	CMyPortal(float l, float t, float r, float b, int scene_id, int spriteId) :CPortal(l, t, r, b, scene_id) {
		this->spriteId = spriteId;
	}
	void Render() {
		CSprites* s = CSprites::GetInstance();
		s->Get(this->spriteId)->Draw(x, y);
		RenderBoundingBox();
	}
};

