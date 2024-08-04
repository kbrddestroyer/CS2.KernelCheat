#pragma once
#include <cmath>
#include <string>
#include <list>

#include <mutex>

#define DEG2RAD (deg) (deg * 3.14f / 180)

template <typename T>
struct Vector3
{
	T x, y, z;
};

template <typename T>
struct Vector2
{
	T x, y;
};

struct ViewMatrix
{
	float matrix[4][4];
	float* operator[](int index) { return matrix[index]; }
};

typedef Vector3<int> Vector3i;
typedef Vector3<float> Vector3f;

typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;

typedef Vector3f QAngle;
typedef float angle_t;

template <typename T> 
class Exception
{
private:
	T val;
	Exception();
public:
	Exception(T);
	T err();
	operator T() const;
};

typedef Exception<std::string> StringException;
typedef Exception<uint32_t> ErrorCode;

enum BoneIndex : int8_t
{
	none = -1,
	head = 6,
	neck_0 = 5,
	spine_1 = 4,
	spine_2 = 2,
	pelvis = 0,
	arm_upper_L = 8,
	arm_lower_L = 9,
	hand_L = 20,
	arm_upper_R = 13,
	arm_lower_R = 14,
	hand_R = 15,
	leg_upper_L = 22,
	leg_lower_L = 23,
	ankle_L = 24,
	leg_upper_R = 25,
	leg_lower_R = 26,
	ankle_R = 27,
};

namespace BoneJointList
{
	inline std::list<BoneIndex> Trunk = { head, neck_0, spine_2, pelvis };
	inline std::list<BoneIndex> LeftArm = { neck_0,  arm_upper_L, arm_lower_L, hand_L};
	inline std::list<BoneIndex> RightArm = { neck_0, arm_upper_R, arm_lower_R, hand_R };
	inline std::list<BoneIndex> LeftLeg = { pelvis, leg_upper_L , leg_lower_L, ankle_L };
	inline std::list<BoneIndex> RightLeg = { pelvis, leg_upper_R , leg_lower_R, ankle_R };
	inline std::list<std::list<BoneIndex>> Skeleton = { Trunk, LeftArm, RightArm, LeftLeg, RightLeg };
}

template <typename T>
class Wrapper
{
private:
	std::mutex mLock;
	T instance;
public:
	T load()
	{
		lock();
		T destination = instance;
		unlock();

		return destination;
	}

	void set(T data)
	{
		lock();
		instance = data;
		unlock();
	}

	void lock()
	{
		mLock.lock();
	}

	void unlock()
	{
		mLock.unlock();
	}
};