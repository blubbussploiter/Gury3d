
#include "pch.h"
#include "framework.h"
#include "MainFrm.h"

#include "log.h"

void RBX::Log::writeEntry(const char* message)
{
	std::string format;

	_SYSTEMTIME SystemTime;
	
	GetLocalTime(&SystemTime);
	format = RBX::Format("%02u:%02u.%03u ", SystemTime.wHour, SystemTime.wMinute, SystemTime.wMilliseconds);

	format += message;
	CMainFrame::mainFrame->m_wndOutput.AddText(format);
}
