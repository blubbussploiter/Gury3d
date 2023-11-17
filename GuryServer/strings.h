#ifndef STRINGS_H
#define STRINGS_H

#include <Windows.h>
#include <string>

static std::string ExePath() {
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::string::size_type pos = std::string(buffer).find_last_of("\\/");
	return std::string(buffer).substr(0, pos);
}

static std::string GetFileInPath(std::string file)
{
	std::string name = ExePath() + file;
	struct stat buf;
	if (stat(name.c_str(), &buf) != -1)
	{
		return name;
	}
	return std::string();
}

static std::string ConFileInPath(std::string file)
{
	std::string name = ExePath() + file;
	return name;
}

#endif