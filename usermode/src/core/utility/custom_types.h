#pragma once
#include <cmath>
#include <string>

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

struct BoneMatrix {
	float bones[4][2];
};

class Bone
{

};