#pragma once
#include "../../imgui/imgui.h"
#include "custom_types.h"

#define MAX_MAX_COORD 2000
#define MARGIN 50

ImVec2 gameToGUIPoint(Vector3f, ImVec2 = { -MAX_MAX_COORD, -MAX_MAX_COORD }, ImVec2 = {MAX_MAX_COORD, MAX_MAX_COORD});
ImVec2 getPlayerOffset(ImVec2 vLocalPlayer);
ImVec2 GUIPointToLocalObserver(Vector3f position, Vector3f localObserverPosition, float rotation);


Vector3f operator+(Vector3f, Vector3f);
Vector3f operator-(Vector3f, Vector3f);
Vector3f operator*(Vector3f, float);
Vector3f operator/(Vector3f, float);


Vector3f abs(Vector3f);
float magnitude(Vector3f);
float distance(Vector3f, Vector3f);
Vector3f rotate(Vector3f, float);
QAngle toAngle(Vector3f);