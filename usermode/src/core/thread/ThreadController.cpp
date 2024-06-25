#include "ThreadController.h"

ThreadController::ThreadController() {}

ThreadController::~ThreadController() {}

bool ThreadController::addThreadElement(ThreadElementController& thElement)
{
	if (!availible())
		return false;

	vCallStack.push_back(std::make_shared<ThreadElementController>(thElement));
	return true;
}

void ThreadController::Update()
{
	for (std::shared_ptr<ThreadElementController> thElement : vCallStack)
	{
		if (thElement)
			thElement->Update();
	}
}

bool ThreadController::availible() { return vCallStack.size() < MAX_CALLSTACK_SIZE; }