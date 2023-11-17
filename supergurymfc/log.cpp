
#include "pch.h"
#include "framework.h"
#include "MainFrm.h"

#include "log.h"

std::string dayOfWeek(WORD d)
{
	switch (d)
	{
	case 1:
		return "Monday";
	case 2:
		return "Tuesday";
	case 3:
		return "Wednesday";
	case 4:
		return "Thursday";
	case 5:
		return "Friday";
	case 6:
		return "Saturday";
	case 7:
		return "Sunday";
	}
	return "";
}

void RBX::Log::writeEntry(MessageType messageType, const char* message)
{
	switch (messageType)
	{
	case MESSAGE_PRINT:
	{
		CMainFrame::mainFrame->m_wndOutput.AddText(message);
		break;
	}
	case MESSAGE_INFO:
	{

		CMainFrame::mainFrame->m_wndOutput.SetTextColor(0, 0, 255);
		CMainFrame::mainFrame->m_wndOutput.AddText(message);
		CMainFrame::mainFrame->m_wndOutput.SetTextColor(0, 0, 0);

		break;
	}
	case MESSAGE_ERROR:
	{
		CMainFrame::mainFrame->m_wndOutput.AddText(message);
		break;
	}
	}
}
