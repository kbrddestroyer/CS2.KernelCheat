#pragma once
#include <vector>
#include <map>
#include <cmath>
#include <atomic>

#include "../../imgui/imgui.h"

#include "ThreadController.h"
#include "GUIEntities.h"
#include "../km_mailbox.h"
#include "../utility/custom_types.h"
#include "../utility/CheatUtilities.h"

namespace cheatscore
{
#pragma region UTILITY
	namespace utility
	{
		enum CheatEntities
		{
			NONE,
			BHOP,
			ENTITY_SCAN,
			RADAR,
			TRIGGER,
			AIMBOT,
			ANTIRECOIL,
			BONE_ESP
		};

		class Cheat : public ThreadedObject
		{
		private:
			inline static std::map<CheatEntities, Cheat*> instances;
			CheatEntities desc;
		protected:
			std::atomic<bool> bState = false;
		public:
			static std::map<CheatEntities, Cheat*>& getMap() { return instances; }
			static Cheat* Instances(CheatEntities entity) {
				return instances[entity];
			}

			Cheat(CheatEntities entity = CheatEntities::NONE) {
				if (entity == NONE)
					throw "Entity type cannot be none";

				instances[entity] = this;
				desc = entity;
			}

			~Cheat() { instances.erase(desc); }

			virtual void toggle(bool bState) { this->bState.store(bState); }
			bool enabled() { return bState.load(std::memory_order_relaxed); }

			void Update(HANDLE, uintptr_t) override;
			virtual void Render() = 0;
		protected:
			virtual void CheatUpdate(HANDLE, uintptr_t) = 0;
		};
	}
#pragma endregion Utility tools for cheats

#pragma region CHEATS
	namespace core
	{
		using namespace utility;
		class BhopCheat : public Cheat
		{
		public:
			BhopCheat() : Cheat(BHOP) {}

			void CheatUpdate(HANDLE, uintptr_t) override;

			void Render() override;
		};

		class RadarHack : public Cheat
		{
		private:
			bool bShowDebugInfo = false;
			bool bInitialised = false;

			std::atomic<bool> isBusyRendering = false;

			std::vector<CSPlayerEntity> vEntities;
			CSPlayerEntity localEntity;
		public:
			RadarHack() : Cheat(RADAR) {}

			bool Initialized() { return bInitialised; }
			void CheatUpdate(HANDLE, uintptr_t) override;

			void Render() override;
		};

		class TriggerBot : public Cheat
		{
		public:
			TriggerBot() : Cheat(TRIGGER) {}

			void CheatUpdate(HANDLE, uintptr_t) override;
			void Render() override;
		};

		class AimBot : public Cheat
		{
		private:
			float fSmoothness;
		public:
			AimBot() : Cheat(AIMBOT) { fSmoothness = 1; }

			void setSmoothness(float fVal) { this->fSmoothness = fVal; }

			void CheatUpdate(HANDLE, uintptr_t) override;
			void Render() override;
		};

		class Antirecoil : public Cheat
		{
		private:
			QAngle oldPunch = { 0, 0, 0 };
			int32_t iShotsFired = 0;
		public:
			Antirecoil() : Cheat(ANTIRECOIL) {}

			void CheatUpdate(HANDLE, uintptr_t) override;
			void Render() override;
		};

		class BoneESP : public Cheat
		{
		private:
			typedef struct CSEntity
			{
				uint32_t uIndex;
				Vector3f vOrigin;
				Vector3f vHeadPosition;
				uint8_t uTeam;
				int32_t iHealth;
				Vector3f* bones;
				bool bUpdated;
			};

			Vector3f vLocalPosition = {};
			QAngle viewAngle = {};
			ViewMatrix viewMatrix = {};
			uint8_t uLocalTeam = 0;

			std::map<uint32_t, CSEntity> mEntities;
			bool bNeedFullSync = true;

			float passedTime = 0.f;
		public:
			BoneESP() : Cheat(BONE_ESP) {}

			void fullSyncRebuild(HANDLE, uintptr_t);
			void scanForBones();
			void CheatUpdate(HANDLE, uintptr_t) override;
			void Render() override;
		};
	}
#pragma endregion Cheats core classes
}