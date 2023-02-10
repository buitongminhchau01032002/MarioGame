#include "HiddenBlockKoopa.h"
#include "PlayScene.h"
void CHiddenBlockKoopa::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	LPPLAYSCENE s = (LPPLAYSCENE)(CGame::GetInstance()->GetCurrentScene());
	int blockSize = s->GetTileSize();
	l = float(lCell * blockSize + blockSize/3);
	t = float(tCell * blockSize);
	r = float(rCell * blockSize + blockSize - blockSize/3);
	b = float(bCell * blockSize + blockSize);
}