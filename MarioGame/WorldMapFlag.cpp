#include "WorldMapFlag.h"
#include "Animations.h"

void CWorldMapFlag::Render() {
	LPANIMATION ani = CAnimations::GetInstance()->Get(ID_ANI_WORLDMAP_FLAG);
	ani->Render(x, y);
}