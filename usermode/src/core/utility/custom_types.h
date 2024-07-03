#pragma once
#include <cmath>
#include <string>

#define DEG2RAD (deg) (deg * 3.14f / 180)

template <typename T>
struct Vector3
{
	T x, y, z;
};

typedef Vector3<int> Vector3i;
typedef Vector3<float> Vector3f;
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
