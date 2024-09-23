/*
*	Made by: KeyboardDestroyer
* Contains all cheat functionality, that's controlled from GUIController
* 
* TODO:
* - Cleanup
*/

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
		// Add new values if you're adding new Cheat class and make a constructow w/ base class constructor call, passing new enum value as an attribute: 
		// NewCheat() : Cheat(ChectEntities.NEW_CHEAT) {}
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

		/**
		* Cheat class should be base class for any functionality
		* Is being invoked automatically if registered properly
		* You should specify cheat name in CheatEntities enum and then
		* pass it in constructor
		* 
		* @param CheatUpdate(HANDLE, uintptr_t) - called in loop. Override and place memory r/w operations here
		* @param Render() - Render information on imGui. NEVER access r/w operaions from here. Just render anything you need on overlay
		*/
		class Cheat : public ThreadedObject
		{
		private:
			inline static std::map<CheatEntities, Cheat*> instances;
			CheatEntities entityKey;
			std::atomic<bool> bState = false;
		private:
			Cheat() {
				throw "Should not be accessed!";
			}
		protected:
			virtual void CheatUpdate() = 0;
		public:
			static Cheat* Instances(CheatEntities entity) {
				return instances[entity];
			}
			static std::map<CheatEntities, Cheat*>& getMap()
			{
				return instances;
			}

			Cheat(CheatEntities entity) {
				if (entity == NONE)
					throw "Entity type cannot be none";
				entityKey = entity;
				instances[entity] = this;
			}

			// Should never be accessed manually (e.g. SomeCheat.~Cheat())
			// That won't delete the cheat and break the control mapping
			~Cheat() { instances.erase(entityKey); }

			/**
			* Get uPlayerPawn pointer by index in entity list.
			* 
			* @param uEntityList uintptr_t - Index of player in dwEntityList
			* @param uIndex uint32_t Index of player in dwEntityList
			* 
			* @return uintptr_t address of uPlayerPawn or nullptr if invalid
			*/
			const uintptr_t getPlayerPawnByIndex(uintptr_t, uint32_t) noexcept;

			/**
			* Common logic of finding the local player is delegated to base class
			* 
			* @return uintptr_t address of LocalPlayerPawn
			*/
			const uintptr_t getLocalPlayerPawn() noexcept;

			/**
			* Toggle cheat state on/off
			* @param bState Current state
			*/
			virtual void toggle(bool bState) { this->bState.store(bState); }
			bool enabled() { return bState.load(std::memory_order_relaxed); }

			/*
			* This function cannot be overriden
			*/
			void Update() override final;

			virtual void Render() = 0;
		};
	}
#pragma endregion Utility tools for cheats

#pragma region CHEATS
	namespace core
	{
		using namespace utility;

		// Just test functionality, usially I use it to check the offsets
		class BhopCheat : public Cheat
		{
		public:
			BhopCheat() : Cheat(BHOP) {}

			void CheatUpdate() override;

			void Render() override;
		};

		// Creates ImGui window on overlay and renders players
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

			void fullSyncUpdate();

			bool updateEntity(CSPlayerEntity&, uintptr_t);

			bool Initialized() { return bInitialised; }
			void CheatUpdate() override;

			void Render() override;
		};

		// Triggers fire button if enemy is in crosshair
		class TriggerBot : public Cheat
		{
		public:
			TriggerBot() : Cheat(TRIGGER) {}

			void CheatUpdate() override;
			void Render() override;
		};

		// Aims to heads, can be tweaked
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

			void CheatUpdate() override;
			void Render() override;
		};

		// Controls gun recoil starting from 2nd shot via viewAngles
		class Antirecoil : public Cheat
		{
		private:
			QAngle oldPunch = { 0, 0, 0 };
			int32_t iShotsFired = 0;
		public:
			Antirecoil() : Cheat(ANTIRECOIL) {}

			void CheatUpdate() override;
			void Render() override;
		};

		// Wallhack on overlay, renders bones
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

			void fullSyncRebuild();
			void scanForBones();
			void CheatUpdate() override;
			void Render() override;
		};
	}
#pragma endregion Cheats core classes
}