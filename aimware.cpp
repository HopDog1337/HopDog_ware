#pragma once
//#include "aimware.h"  lel big anti-pasta
#include "global.h"
#include "GameUtils.h"
using namespace std;
#include <ctime>
#include <iostream>
#include "Render.h"
#include "Draw.h"
#include <chrono>
#include "sdk.h"
#include <iostream>
#include "Render.h"
#include "Draw.h"
using namespace std::chrono;
int64 GetEpochMS() {
	int64 now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	return now;
}

CAW_Hitmarker* aw_hitmarker = new CAW_Hitmarker();

// establish listeners that will trigger our FireGameEvent function
void CAW_Hitmarker::initialize() {
	g_pGameEventManager->AddListener(this, "player_hurt", false);
	g_pGameEventManager->AddListener(this, "bullet_impact", false);
}

// iterates through hitmarkers and draws/adjusts them (call before ur esp or w/e)
void CAW_Hitmarker::paint() {

	if (!Clientvariables->Visuals.aimware_hitmarker)
		return;

	if (!g_pEngine->IsConnected() || !g_pEngine->IsInGame() || !G::LocalPlayer) {
		if (!impacts.empty())
			impacts.clear();
		if (!hitmarkers.empty())
			hitmarkers.clear();
		return;
	}

	long long time = GetEpochMS();

	std::vector<hitmarker_info>::iterator iter;
	for (iter = hitmarkers.begin(); iter != hitmarkers.end();) {

		// handle expiration / fade out
		bool expired = time > iter->impact.time + 2000;
		static int alpha_interval = 255 / 50;
		if (expired) iter->alpha -= alpha_interval;
		if (expired && iter->alpha <= 0) { // 25 ms expiration
			iter = hitmarkers.erase(iter);
			continue;
		}

		// get the location of the hit on our screen
		Vector pos3D = Vector(iter->impact.x, iter->impact.y, iter->impact.z), pos2D;
		if (!GameUtils::WorldToScreen(pos3D, pos2D)) {
			++iter;
			continue;
		}

		// get color of the hitmarker (based on damage)
		Color c = get_hitmarker_color(*iter);
		Color c_2 = Color(0, 0, 0, 255);
		c.SetAlpha(iter->alpha);
	
		// draw it
		int lineSize = Clientvariables->Visuals.hitmarker_size;
	//	g_Draw.ColoredCircle(pos2D.x, pos2D.y, 12, 255, 255, 255, 240);  gey test (circle)
		g_Draw.DrawLine(pos2D.x - lineSize, pos2D.y - lineSize, pos2D.x - (lineSize / 4), pos2D.y - (lineSize / 4), c);
		g_Draw.DrawLine(pos2D.x + lineSize, pos2D.y - lineSize, pos2D.x + (lineSize / 4), pos2D.y - (lineSize / 4), c);
		g_Draw.DrawLine(pos2D.x - lineSize, pos2D.y + lineSize, pos2D.x - (lineSize / 4), pos2D.y + (lineSize / 4), c);
		g_Draw.DrawLine(pos2D.x + lineSize, pos2D.y + lineSize, pos2D.x + (lineSize / 4), pos2D.y + (lineSize / 4), c);

		++iter;

	}

}

void CAW_Hitmarker::player_hurt(IGameEvent *event) {

	CBaseEntity* attacker = get_player(event->GetInt("attacker"));
	CBaseEntity* victim = get_player(event->GetInt("userid"));
	int damage = event->GetInt("dmg_health");

	if (!attacker || !victim || attacker != G::LocalPlayer)
		return;

	Vector enemyPosition = victim->GetOrigin();
	impact_info best_impact;
	float best_impact_distance = -1;
	long long time = GetEpochMS();

	std::vector<impact_info>::iterator iter;
	for (iter = impacts.begin(); iter != impacts.end();) {

		// expire in 25 ms
		if (time > iter->time + 25) {
			iter = impacts.erase(iter);
			continue;
		}

		// get the best impact (closest to hurt player)
		Vector position = Vector(iter->x, iter->y, iter->z);
		float distance = position.Dist(enemyPosition);
		if (distance < best_impact_distance || best_impact_distance == -1) {
			best_impact_distance = distance;
			best_impact = *iter;
		}

		++iter;

	}

	if (best_impact_distance == -1)
		return;

	// establish new hitmarker, add to logs
	hitmarker_info info;
	info.impact = best_impact;
	info.alpha = 255;
	info.damage = damage;
	hitmarkers.push_back(info);

}

void CAW_Hitmarker::bullet_impact(IGameEvent *event) {

	CBaseEntity* shooter = get_player(event->GetInt("userid"));

	if (!shooter || shooter != G::LocalPlayer)
		return;

	// establish bullet impact, add to logs
	impact_info info;
	info.x = event->GetFloat("x");
	info.y = event->GetFloat("y");
	info.z = event->GetFloat("z");
	info.time = GetEpochMS();
	impacts.push_back(info);

}

Color CAW_Hitmarker::get_hitmarker_color(hitmarker_info hitmarker) {

	switch (!Clientvariables->Visuals.aimware_hitmarker_color) {
	case 0:		return Color(200, 200, 200, 255);	break; // White
	case 1:		return Color(255, 0, 0, 255);		break; // Red
	case 2:		return Color(218, 21, 193, 255);	break; // Pink
	case 3:		return Color(0, 0, 255, 255);		break; // Blue
	case 4:		return Color(0, 255, 0, 255);		break; // Green
	case 5:		return Color(218, 146, 21, 255);	break; // Orange
	case 6:		return Color(218, 211, 21, 255);	break; // Yellow
	case 7:		return Color(0, 0, 0, 255);			break; // Black
	case 8:		return hitmarker.random_color;		break; // Random
	default:	return Color(200, 200, 200, 255);	break; // White (default)
	}
}

// function to simply get a player entity from a userid (provided in events)
CBaseEntity* CAW_Hitmarker::get_player(int userid) {
	int index = g_pEngine->GetPlayerForUserID(userid);
	CBaseEntity* entity = g_pEntitylist->GetClientEntity(index);
	return entity;
}

// call the corresponding function when an event is triggered
void CAW_Hitmarker::FireGameEvent(IGameEvent *event) {

	if (!Clientvariables->Visuals.aimware_hitmarker)
		return;

	if (!event || !G::LocalPlayer)
		return;

	if (!strcmp(event->GetName(), "player_hurt"))
		player_hurt(event);

	if (!strcmp(event->GetName(), "bullet_impact"))
		bullet_impact(event);

}

int CAW_Hitmarker::GetEventDebugID(void) {
	return 0x2A;
}
