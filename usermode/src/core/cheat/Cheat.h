#pragma once
#include <vector>
#include <map>
#include <cmath>
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
			AIMBOT
		};

		class Cheat : public ThreadedObject
		{
		private:
			inline static std::map<CheatEntities, Cheat*> instances;
			CheatEntities desc;
		protected:
			bool bState = false;
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

			virtual void toggle(bool bState) { this->bState = bState; }
			void Update(HANDLE, uintptr_t) override;
			virtual void Render(ImDrawList*) = 0;
		protected:
			virtual void CheatUpdate(HANDLE, uintptr_t) = 0;
		};

		class EntityScannerDependency : public Cheat
		{
		public:
			EntityScannerDependency(CheatEntities entity = NONE);
			~EntityScannerDependency();

			void toggle(bool bState) override;
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

			void Render(ImDrawList*) override;
		};

		class EntityScanner : public Cheat
		{
		private:
			inline static EntityScanner* instance;
			uint8_t callCount = 0;
			std::vector<CSPlayerEntity> vEntities;
			CSPlayerEntity localEntity;
		public:
			EntityScanner() : Cheat(ENTITY_SCAN) { instance = this; bState = true; }
			static EntityScanner* getInstance() { return instance; }

			void add() { callCount++; }
			void remove() { if (callCount > 0) callCount--; }

			std::vector<CSPlayerEntity> getEntities() { return vEntities; }
			CSPlayerEntity getLocalEntity() { return localEntity; }

			void CheatUpdate(HANDLE, uintptr_t) override;
			void Render(ImDrawList*) override;
		};

		class RadarHack : public EntityScannerDependency
		{
		private:
			bool bShowDebugInfo = false;
			bool bInitialised = false;
		public:
			RadarHack() : EntityScannerDependency(RADAR) {}

			bool Initialized() { return bInitialised; }
			void CheatUpdate(HANDLE, uintptr_t) override;

			void Render(ImDrawList*) override;
		};

		class TriggerBot : public Cheat
		{
		public:
			TriggerBot() : Cheat(TRIGGER) {}

			void CheatUpdate(HANDLE, uintptr_t) override;
			void Render(ImDrawList*) override;
		};

		class AimBot : public EntityScannerDependency
		{
		private:
			float fSmoothness;
		private:
			CSPlayerEntity& closest(std::vector<CSPlayerEntity>&, CSPlayerEntity);
		public:
			AimBot() : EntityScannerDependency(AIMBOT) { fSmoothness = 1; }

			void setSmoothness(float fVal) { this->fSmoothness = fVal; }

			void CheatUpdate(HANDLE, uintptr_t) override;
			void Render(ImDrawList*) override;
		};
	}
#pragma endregion Cheats core classes
}