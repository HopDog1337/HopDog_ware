#include "sdk.h"
#include "Resolver2.h"
#include "global.h"
#include "BacktrackingHelper.h"
#include "Math.h"
#include "GameUtils.h"
#include "Menu.h"
CResolver* g_Resolver = new CResolver;
c_resolver* g_resolver = new c_resolver;



void c_resolver::Store(CBaseEntity * Entity) {
	if (!G::UserCmd || !Entity || Entity->GetIndex() < 1 || Entity->GetIndex() > 64)
		return;

	auto record = &arr_infos.at(Entity->GetIndex());

	record->Entity = Entity;

	if (record->TickRecords.size() > 1 && record->TickRecords.front().tickcount == G::UserCmd->tick_count)
		return;

	record->TickRecords.push_front(CTickRecord(Entity));

	while ((int)record->TickRecords.size() > 256)
		record->TickRecords.pop_back();
}