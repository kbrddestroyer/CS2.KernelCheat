#pragma once

#include "ThreadController.h"
#include "../km_mailbox.h"

class BhopCheat : public ThreadedObject
{
public:
	void Update(HANDLE, uintptr_t) override;
};
