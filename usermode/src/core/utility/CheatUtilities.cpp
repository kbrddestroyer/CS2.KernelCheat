#include "CheatUtilities.h"

ImVec2 gameToGUIPoint(Vector3f point, ImVec2 min, ImVec2 max)
{
	ImVec2 vMargin = { MARGIN, MARGIN };
	ImVec2 vSize = ImGui::GetWindowSize();
	ImVec2 vPos = ImGui::GetWindowPos();

	ImVec2 vGameScale = { min.x - max.x, min.y - max.y };
	ImVec2 vGamePos = { min.x - point.x, min.y - point.y };

	ImVec2 vScale = { (vSize.x - 2 * vMargin.x) / vGameScale.x, (vSize.y - 2 * vMargin.y) / vGameScale.y };

	return { vPos.x + vGamePos.x * vScale.x + vMargin.x, vPos.y + vGamePos.y * vScale.y + vMargin.y };
}

ImVec2 getPlayerOffset(ImVec2 vLocalPlayer)
{
	ImVec2 vSize = ImGui::GetWindowSize();
	return { vSize.x / 2 - vLocalPlayer.x, vSize.x / 2 - vLocalPlayer.y };
}

ImVec2 rotateOnPoint(ImVec2 vOffset, float angle)
{
	return { vOffset.x - cos(angle), vOffset.y + sin(angle) };
}

ImVec2 GUIPointToLocalObserver(Vector3f position, Vector3f localObserverPosition, float rotation)
{
	ImVec2 vWndPosition = ImGui::GetWindowPos();
	ImVec2 vSize = ImGui::GetWindowSize();

	ImVec2 root = { vWndPosition.x + vSize.x / 2, vWndPosition.y + vSize.y / 2 };

	ImVec2 vPosition = gameToGUIPoint({ position.x - localObserverPosition.x, position.y - localObserverPosition.y });
	ImVec2 vDistance = { vPosition.x - root.x, vPosition.y - root.y };
	float distance = sqrt(vDistance.x * vDistance.x + vDistance.y * vDistance.y);
	rotation *= 3.14 / 180;

	ImVec2 vOffset = { vDistance.x + cos(rotation) * distance, vDistance.y + sin(rotation) * distance };

	return vOffset;
}

Vector3f operator+(Vector3f a, Vector3f b)
{
	return { a.x + b.x, a.y + b.y };
}

Vector3f operator-(Vector3f a, Vector3f b)
{
	return { a.x - b.x, a.y - b.y };
}

Vector3f operator*(Vector3f a, float b)
{
	return { a.x - b, a.y - b };
}

Vector3f operator/(Vector3f a, float b)
{
	return { a.x - b, a.y - b };
}

Vector3f abs(Vector3f a)
{
	return { abs(a.x), abs(a.y) };
}

float magnitude(Vector3f a)
{
	return sqrt(a.x * a.x + a.y * a.y);
}

Vector3f rotate(Vector3f vector, float fAngle)
{
	return { vector.x * cos(fAngle) - vector.y * sin(fAngle), vector.y * cos(fAngle) + vector.x * sin(fAngle) };
}