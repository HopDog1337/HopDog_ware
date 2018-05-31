#pragma once
#include <deque>
class CResolver
{
public:
	struct CTickRecord;

	struct CValidTick {
		explicit operator CTickRecord() const;

		explicit operator bool() const noexcept {
			return m_flSimulationTime > 0.f;
		}

		float m_flPitch = 0.f;
		float m_flYaw = 0.f;
		float m_flSimulationTime = 0.f;
		CBaseEntity* m_pCBaseEntity = nullptr;
	};

	struct CTickRecord {
		CTickRecord() {}
		CTickRecord(CBaseEntity* ent, int tickcount);
		CTickRecord(CBaseEntity* ent, Vector EyeAngles, float LowerBodyYaw);

		explicit operator bool() const noexcept {
			return m_flSimulationTime > 0.f;
		}

		bool operator>(const CTickRecord& others) {
			return (m_flSimulationTime > others.m_flSimulationTime);
		}
		bool operator>=(const CTickRecord& others) {
			return (m_flSimulationTime >= others.m_flSimulationTime);
		}
		bool operator<(const CTickRecord& others) {
			return (m_flSimulationTime < others.m_flSimulationTime);
		}
		bool operator<=(const CTickRecord& others) {
			return (m_flSimulationTime <= others.m_flSimulationTime);
		}
		bool operator==(const CTickRecord& others) {
			return (m_flSimulationTime == others.m_flSimulationTime);
		}

		float m_flLowerBodyYawTarget = 0.f;
		QAngle m_angEyeAngles = QAngle(0, 0, 0);
		float m_flCycle = 0.f;
		float m_flSimulationTime = 0.f;
		int m_nSequence = 0;
		Vector m_vecOrigin = Vector(0, 0, 0);
		Vector m_vecAbsOrigin = Vector(0, 0, 0);
		Vector m_vecVelocity = Vector(0, 0, 0);
		std::array<float, 24> m_flPoseParameter = {};
		QAngle m_angAbsAngles = QAngle(0, 0, 0);
		CValidTick validtick;
		int tickcount = 0;
	};

	class CResolveInfo
	{
	public:
		std::deque<CTickRecord> m_sRecords;
	protected:

		bool	m_bEnemyShot; //priority
		bool	m_bLowerBodyYawChanged;
		bool	m_bBacktrackThisTick;
	};

	enum resolver_stages
	{
		lby_delta,
		at_bw,
		at_left,
		at_half_right,
		at_half_left,
		at_right
	};

public:
	void StoreVars(CBaseEntity* ent);
	void StoreVars(CBaseEntity* ent, Vector EyeAngles, float LowerBodyYaw);
	bool HasStaticRealAngle_Wrapper(int index, float tolerance);
	//	float ResolvePitch(CBaseEntity* pPlayer, float org_pitch);
	//	float ResolveYaw(CBaseEntity* pPlayer, float org_yaw);
	//	void Run(CBaseEntity* pPlayer);


	std::string aa_info[64];



	float resolve_infos[64];

private:
	float LatestLowerBodyYaw[64];
	bool LbyUpdated[64];
	float YawDifference[64];
	float OldYawDifference[64];
	float LatestLowerBodyYawUpdateTime[64];

	std::array<CResolveInfo, 32> m_arrInfos;




}; extern CResolver* g_Resolver;


extern bool ran_thru_resolver;

extern bool did_hs[64];
extern bool did_hit[64];
extern int hit_tick;
extern int hit_tick2;

class c_resolver {
private:
	struct CTickRecord {
		CTickRecord() {}
		CTickRecord(CBaseEntity* ent) {
			m_flLowerBodyYawTarget = ent->LowerBodyYaw();
			m_angEyeAngles = ent->GetEyeAngles();
			m_flCycle = ent->GetCycle();
			m_flSimulationTime = ent->GetSimulationTime();
			for (int i = 0; i < 15; i++)
			{
				auto *overlay = &ent->GetAnimOverlays()[i];
				Utilities::Log("Activity = %i", ent->GetSequenceActivity(overlay->m_nSequence));

				m_nSequence = overlay->m_nSequence;
			}
			m_vecOrigin = ent->GetOrigin();
			m_vecVelocity = ent->GetVelocity();
			m_flPoseParameter = ent->GetPoseParameters();
			m_angAbsAngles = ent->GetAbsAngles();
			m_vecAbsOrigin = ent->GetAbsOrigin();
			//m_flDuckSpeed = ent->GetDuckSpeed();
			//m_flDuckAmount = ent->GetDuckAmount();
		}

		explicit operator bool() const noexcept {
			return m_flSimulationTime > 0.f;
		}

		bool operator>(const CTickRecord& others) {
			return (m_flSimulationTime > others.m_flSimulationTime);
		}
		bool operator>=(const CTickRecord& others) {
			return (m_flSimulationTime >= others.m_flSimulationTime);
		}
		bool operator<(const CTickRecord& others) {
			return (m_flSimulationTime < others.m_flSimulationTime);
		}
		bool operator<=(const CTickRecord& others) {
			return (m_flSimulationTime <= others.m_flSimulationTime);
		}
		bool operator==(const CTickRecord& others) {
			return (m_flSimulationTime == others.m_flSimulationTime);
		}

		//float m_flDuckAmount = 0.f;
		//float m_flDuckSpeed = 0.f;
		float m_flLowerBodyYawTarget = 0.f;
		QAngle m_angEyeAngles = QAngle(0, 0, 0);
		float m_flCycle = 0.f;
		float m_flSimulationTime = 0.f;
		int m_nSequence = 0;
		Vector m_vecOrigin = Vector(0, 0, 0);
		Vector m_vecAbsOrigin = Vector(0, 0, 0);
		Vector m_vecVelocity = Vector(0, 0, 0);
		std::array<float, 24> m_flPoseParameter = {};
		QAngle m_angAbsAngles = QAngle(0, 0, 0);
		int tickcount = 0;
	};

	struct C_PlayerStored {
		CBaseCombatWeapon* CurrentWeapon = nullptr;
		int BulletsLeft = 0;
		bool EyeAnglesUpdated = false;
		bool IsFiring = false;
		int TickCount = 0;
		float SimulationTime = 0;
		QAngle EyeAngles = QAngle(0, 0, 0);
		CTickRecord ValidTick = CTickRecord();
	};
	struct CBacktrackInfo {
		CBacktrackInfo() : shot(false), player(C_PlayerStored()), tick_count(0), temporaryRecord(CTickRecord()) {}

		bool shot = false;
		C_PlayerStored player = C_PlayerStored();
		int tick_count = 0;
		CTickRecord temporaryRecord = CTickRecord();


		CBaseEntity* Entity;
		std::deque<CTickRecord> TickRecords;
	};
	struct ResolveInfo
	{
		float last_by;
		float last_simtime;
		bool is_breaking_lby;
	};
	//	void Resolve_Nospread(CBaseEntity * pEntity);
	//	void Resolve_Predict(CBaseEntity* pEntity);
	//	void Resolve_Smart(CBaseEntity * pEntity);
public:
	//	void Resolve(CBaseEntity* pEntity);
	void Store(CBaseEntity * Entity);
	ResolveInfo g_resolverinfo[64];
	std::array<CBacktrackInfo, 64> arr_infos;
};


extern c_resolver* g_resolver;

class CPredictable
{
public:
	float m_flLowerBodyYawTarget = 0.0f;
	float m_flSimulationTime = 0.0f;

	//	CPredictable(float lby, float sim);
};
