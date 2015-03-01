/*
 * Copyright (C) 2015 Christian Benjamin Ries
 * Website: https://github.com/cbries/utilities
 * License: MIT
 */

#pragma once

#ifndef __EXECCMD_H__
#define __EXECCMD_H__

// C/C++
#include <stdio.h>
#include <string>

std::string execcmd(const std::string & cmdline, bool *ok=NULL);

#endif // __EXECCMD_H__

