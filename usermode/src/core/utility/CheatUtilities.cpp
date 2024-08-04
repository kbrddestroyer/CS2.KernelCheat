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

Vector3f worldToScreenPoint(ViewMatrix matrix, Vector3f vPosition)
{
	float invW = matrix[3][0] * vPosition.x + matrix[3][1] * vPosition.y + matrix[3][2] * vPosition.z + matrix[3][3];
	if (invW <= 0.01f)
		return { 0, 0, 0 };

	Vector2f vPrecompPosition = {
		(matrix[0][0] * vPosition.x + matrix[0][1] * vPosition.y + matrix[0][2] * vPosition.z + matrix[0][3]) / invW,
		(matrix[1][0] * vPosition.x + matrix[1][1] * vPosition.y + matrix[1][2] * vPosition.z + matrix[1][3]) / invW,
	};

	ImVec2 vSize = ImGui::GetIO().DisplaySize;
	ImVec2 vScreenCenter = {
		vSize.x / 2,
		vSize.y / 2
	};

	ImVec2 vScreenPoint = {
		vScreenCenter.x + (vPrecompPosition.x * vScreenCenter.x),
		vScreenCenter.y - (vPrecompPosition.y * vScreenCenter.y)
	};

	return { vScreenPoint.x, vScreenPoint.y, invW };
}

Vector3f operator+(Vector3f a, Vector3f b)
{
	return { a.x + b.x, a.y + b.y, a.z + b.z };
}

Vector3f operator-(Vector3f a, Vector3f b)
{
	return { a.x - b.x, a.y - b.y, a.z - b.z };
}

Vector3f operator*(Vector3f a, float b)
{
	return { a.x * b, a.y * b, a.z * b};
}

Vector3f operator/(Vector3f a, float b)
{
	return { a.x / b, a.y / b, a.z / b };
}

Vector3f abs(Vector3f a)
{
	return { abs(a.x), abs(a.y), abs(a.z) };
}

float magnitude(Vector3f a)
{
	return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

float distance(Vector3f a, Vector3f b)
{
	return magnitude(abs(a - b));
}

Vector3f rotate(Vector3f vector, float fAngle)
{
	return { vector.x * cos(fAngle) - vector.y * sin(fAngle), vector.y * cos(fAngle) + vector.x * sin(fAngle), vector.z };
}

QAngle toAngle(Vector3f vector)
{
	return {
			std::atan2(-vector.z, std::hypot(vector.x, vector.y)) * (180.0f / 3.14f),
			std::atan2(vector.y, vector.x) * (180.0f / 3.14f),
			0.0f
	};
}