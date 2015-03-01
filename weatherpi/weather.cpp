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

int main(int argc, char **argv)
{
	Dht22 dht22(PIN_DHT);
	if(!dht22.Refresh()) { perror("dht22.Refresh() failed"); }
	else
	{
		std::cout << "DHT22" << std::endl;
		std::cout << "Temperature = " << dht22.Temperature() << " " << dht22.TemperatureUnit() << std::endl;
		std::cout << "Humidity = " << dht22.Humidity() << " " << dht22.HumidityUnit() << std::endl;
	}

	Bmp180 bmp180;
	bool r = bmp180.Refresh();
	if(!r) { perror("bmp180.refresh() failed"); }
	else
	{
		std::cout << "BMP180" << std::endl;
		std::cout << "Temperature = " << bmp180.Temperature() << " " << bmp180.TemperatureUnit() << std::endl;
		std::cout << "Pressure = " << bmp180.Pressure() << " " << bmp180.PressureUnit() << std::endl;
		std::cout << "Altitude = " << bmp180.Altitude() << std::endl;
	}
}

