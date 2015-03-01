/*
 * Copyright (C) 2015 Christian Benjamin Ries
 * Website: https://github.com/cbries/utilities
 * License: MIT
 */

// weatherpi
#include <execcmd.h>

std::string execcmd(const std::string & cmdline, bool *ok)
{
	FILE *handle = popen(cmdline.c_str(), "r");
	if(handle == NULL)
	{
		if(ok != NULL) { *ok = false; }
		return "";
	}

	char buf[1024];
	size_t n = 0;
	std::string v = "";

	while ((n = fread(buf, 1, sizeof(buf), handle)) > 0) {
        v += buf;
    }
    pclose(handle);

	if(ok != NULL) { *ok = true; }
	return v;
}

