/*
 * Copyright (C) 2015 Christian Benjamin Ries
 * Website: https://github.com/cbries/utilities
 * License: MIT
 */

// C/C++
#include <iostream>
#include <iomanip>
#include <cstdint>
#include <stdint.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <sstream>

// weatherapi
#include <bmp180.h>
#include <strhelper.h>

Bmp180::Bmp180()
{
	temperature = 0.f;
	pressure = 0.f;
	altitude = 0.f;
}

Bmp180::~Bmp180()
{ }

bool Bmp180::Refresh()
{
	FILE *handle = popen("python thirdparty/Adafruit/Adafruit_BMP085/Adafruit_BMP085_Query.py", "r");
	if(handle == NULL) { return false; }
	char buf[1024];
	size_t n = 0;
	std::string v = "";
	while ((n = fread(buf, 1, sizeof(buf), handle)) > 0) 
	{
		//fwrite(buf, 1, n, stdout);
		v += buf;
	}
	pclose(handle);
	
	return parse(v);
}

bool Bmp180::parse(const std::string & values)
{
	auto parts = Helper::split(values, '\n');
	for(int i=0; i < parts.size(); ++i)
	{
		if(parts[i].length() <= 0) continue;
		auto keyvalue = Helper::split(parts[i], '=');
		if(keyvalue[0] == "Temperature")
		{
			auto v = Helper::split(keyvalue[1], ',');
			temperature = std::stof(Helper::trim(v[0]));
			temperatureUnit = Helper::trim(v[1]); 
		}
		else if(keyvalue[0] == "Pressure")
		{
			auto v = Helper::split(keyvalue[1], ',');
			pressure = std::stof(Helper::trim(v[0]));
			pressureUnit = Helper::trim(v[1]);
		}
		else if(keyvalue[0] == "Altitude")
		{
			altitude = stof(Helper::trim(keyvalue[1]));
		}
		else
		{
			//std::cerr << "Unknown format: " << parts[i];
		}
	}
	return true;
}

