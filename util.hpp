#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>

inline std::string verify_slash_at_end_path (const std::string &folder)
{
	if (folder.size () == 0)
		return "";
	else if (folder [folder.size () - 1] == '/')
		return folder;
	else
		return folder + "/";
}

#endif // UTIL_HPP
