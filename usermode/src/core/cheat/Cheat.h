#pragma once
#include <vector>
#include <map>
#include <cmath>
#include <atomic>
#include <list>

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
			BONE_ESP,
			AUTOSTRAFE,
		};


		/**
		* Cheat - base class for every cheat functionality
		* TODO:
		*  Make this SOLID
		*  Add base entity scan for all children
		*  Make this shit readable
		* 
		*/
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
			/**
			* Get uPlayerPawn pointer by index in entity list.
			* @remark uPlayerPawn scan process became more complicated and it keeps being dublicated in every Cheat class
			* it's recommended to keep this logic common for all classes since it's the most basic thing
			* and cannot be simplified
			* Anyway it really helps in optimisation since I don't need to change this everywhere
			* 
			* @param uIndex Index of player in dwEntityList
			* @return uintptr_t pointer to uPlayerPawn or nullptr if invalid
			* @throws should not throw exceptions
			*/
			uintptr_t getPlayerPawnByIndex(HANDLE, uintptr_t, uintptr_t, uint32_t);
			uintptr_t getLocalPlayerPawn(HANDLE, uintptr_t);


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
			bool bNeedFullSync = true;
			float passedTime = 0;

			std::map<uintptr_t, CSPlayerEntity> vEntities;
			std::pair<uintptr_t, CSPlayerEntity> localEntity;
		public:
			RadarHack() : Cheat(RADAR) {}

			void fullSyncUpdate(HANDLE, uintptr_t);

			bool updateEntity(CSPlayerEntity&, HANDLE, uintptr_t, uintptr_t);

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

			struct CSData
			{
				bool bUpdated;
				bool bRender;
				Vector3f vScreenPosition;
			};

			Vector3f vWindowCenter;
			std::map<uintptr_t, CSData> mEntities;
		public:
			AimBot() : Cheat(AIMBOT) { 
				fSmoothness = 1; 

				vWindowCenter = {
					ImGui::GetIO().DisplaySize.x / 2,
					ImGui::GetIO().DisplaySize.y / 2,
					0
				};
			}

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
			typedef std::map<uint8_t, Vector3f> BoneStructure;
			typedef struct Bonearray
			{
				BoneStructure mBones;
			};

			typedef struct CSEntity
			{
				uintptr_t uPlayerPawn;
				Vector3f vOrigin = {};
				Vector3f vHeadPosition = {};
				uint8_t uTeam = 0;
				int32_t iHealth = 0;
				Bonearray bones = {};
				bool bUpdated = false;
			};

			Vector3f vLocalPosition = {};
			QAngle viewAngle = {};
			ViewMatrix viewMatrix = {};
			uint8_t uLocalTeam = 0;

			Wrapper<std::vector<CSEntity>> mEntities;
			bool bNeedFullSync = true;

			float passedTime = 0.f;
		public:
			BoneESP() : Cheat(BONE_ESP) {}

			void fullSyncRebuild(HANDLE, uintptr_t);
			void scanForBones();
			void CheatUpdate(HANDLE, uintptr_t) override;
			void Render() override;
		};

		class Autostrafe : public Cheat
		{
		public:
			Autostrafe() : Cheat(AUTOSTRAFE) {}

			void CheatUpdate(HANDLE, uintptr_t) override;
			void Render() override;
		};
	}
#pragma endregion Cheats core classes
}