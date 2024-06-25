#pragma once
#include <Windows.h>

#include "../client.dll.hpp"
#include "../buttons.hpp"
#include "../offsets.hpp"

#include "../thread/ThreadController.h"

class CheatBase : public ThreadElementController
{
protected:
	HANDLE		hDriver;
	uintptr_t	uClient;

	virtual void UpdateImpl() = 0;
public:
	CheatBase();
	~CheatBase();

	void Update() override;
};

