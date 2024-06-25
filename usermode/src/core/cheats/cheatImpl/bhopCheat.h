#pragma once

#include "../CheatBase.h"

#include "../client.dll.hpp"
#include "../buttons.hpp"
#include "../offsets.hpp"

#include "../core/constants.h"

class bhopCheat : public CheatBase
{
protected:
	void UpdateImpl() override;
};

