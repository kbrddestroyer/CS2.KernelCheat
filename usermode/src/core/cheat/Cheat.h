#pragma once

#include "ThreadController.h"

#include "../km_mailbox.h"

class BhopCheat : ThreadedObject
{
public:
	void Update(HANDLE, uintptr_t) override;
};

