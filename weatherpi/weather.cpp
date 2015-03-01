/*
 * Copyright (C) 2015 Christian Benjamin Ries
 * Website: https://github.com/cbries/utilities
 * License: MIT
 */

// C/C++
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Linux 
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

// weatherapi
#include <dht22.h>
#include <bmp180.h>
#include <execcmd.h>
#include <strhelper.h>

#define DELAY 60 // seconds
#define RRDTOOL "rrdtool"

Dht22 *pDht22 = NULL;
Bmp180 *pBmp180 = NULL;

void initialize()
{
	std::cout << "Initialize: " << std::flush;
	pDht22 = new Dht22(PIN_DHT);
	pBmp180 = new Bmp180();
	usleep(5 * 1000 * 1000);
	std::cout << "DONE" << std::endl;
}

void dump()
{
	std::cout << " Temperature = " << pDht22->Temperature() << " " << pDht22->TemperatureUnit() << std::endl;
	std::cout << " Humidity = " << pDht22->Humidity() << " " << pDht22->HumidityUnit() << std::endl;
	std::cout << " Temperature = " << pBmp180->Temperature() << " " << pBmp180->TemperatureUnit() << std::endl;
	std::cout << " Pressure = " << pBmp180->Pressure() << " " << pBmp180->PressureUnit() << std::endl;
	std::cout << " Altitude = " << pBmp180->Altitude() << std::endl;
}

int main(int argc, char **argv)
{
	initialize();

	while(true)
	{
		std::cout << "Query data: ";

		bool r0 = pDht22->Refresh();
		bool r1 = pBmp180->Refresh();

		if(!r0) { std::cerr << "dht22->Refresh() failed" << std::endl; }  
		if(!r1) { std::cerr << "bmp180->Refresh() failed" << std::endl; }

		if(!r0 || !r1) 
		{
			std::cout << "MISS" << std::endl;
			usleep(DELAY * 1000);
			continue;
		}
		else
		{
			std::cout << "OK" << std::endl;
		}

		std::cout << "RRD: ";

		char rrdcmd[2048] = { '\0' };
		snprintf(rrdcmd, 2048, "%s update rrdtool/weatherdata.rrd N:%3.3f:%3.3f:%3.3f:%4.3f"
			, RRDTOOL
			, pBmp180->Temperature()
			, pDht22->Humidity()
			, pBmp180->Pressure()
			, pBmp180->Altitude());

#ifdef DEBUG
		std::cout << rrdcmd << std::endl;
#endif

		bool ok;
		std::string output = execcmd(rrdcmd, &ok);
		if(!ok)
		{
			std::cout << "MISS" << std::endl;
			std::cerr << "rrdtool update failed: " << rrdcmd << std::endl;
			std::cerr << "[OUTPUT]" << std::endl;
			std::cerr << Helper::trim(output) << std::endl;
			std::cerr << "[/OUTPUT]" << std::endl;
		}
		else
		{
			std::string m = "./rrdtool/creategraphs.sh";
			output = execcmd(m, &ok);
			if(!ok)
			{
				std::cout << "MISS" << std::endl;
				std::cerr << "rrdtool graph failed: " << m << std::endl;
				std::cerr << "[OUTPUT]" << std::endl;
            	std::cerr << Helper::trim(output) << std::endl;
            	std::cerr << "[/OUTPUT]" << std::endl;
			}
			else
			{
				std::cout << "OK" << std::endl;
			
				dump();
			}
		}

		usleep(DELAY * 1000 * 1000);
	}
}

