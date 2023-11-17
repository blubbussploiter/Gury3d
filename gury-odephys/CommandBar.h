#pragma once

#include "pch.h"
#include "framework.h"

// CommandBar

class CommandBar : public CEdit
{
	DECLARE_DYNAMIC(CommandBar)

public:
	CommandBar();
	virtual ~CommandBar();

protected:
	DECLARE_MESSAGE_MAP()
};


