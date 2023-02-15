#include "TunnelOut.h"
#include "PlayScene.h"

void CTunnelOut::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario*)(scene)->GetPlayer();

	if (mario->GetTunnelId() == id) {
		// Check in tunnel
		float mario_l, mario_t, mario_r, mario_b;
		float tunnel_l, tunnel_t, tunnel_r, tunnel_b;
		mario->GetBoundingBox(mario_l, mario_t, mario_r, mario_b);
		this->GetBoundingBox(tunnel_l, tunnel_t, tunnel_r, tunnel_b);

		// Check go to tunnel out
		if ((ny > 0 && mario_t > tunnel_b) || (ny < 0 && mario_b < tunnel_t)) {
			mario->SetState(MARIO_STATE_NONE);
		}
	}
}

void CTunnelOut::Render()
{
	RenderBoundingBox();
}
