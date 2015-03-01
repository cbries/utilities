/*
 * Copyright (C) 2015 Christian Benjamin Ries
 * Website: https://github.com/cbries/utilities
 * License: MIT
 */

#ifndef __DHT22_H__
#define __DHT22_H__

#define PIN_DHT 4 // Pin 7 for the Raspberry Pi B+
#define BCM2708_PERI_BASE 0x20000000
#define GPIO_BASE (BCM2708_PERI_BASE + 0x200000) // GPIO controller

// External
#include <bcm2835.h>

class Dht22
{
public:
	Dht22(const int pin);
	~Dht22();
private:
	Dht22(const Dht22 & obj) { }
	Dht22 & operator=(const Dht22 & obj) { }

public:
	bool Refresh();
	inline float Humidity() const { return humidity; }
	inline std::string HumidityUnit() const { return "%"; }
	inline float Temperature() const { return temperature; }
	inline std::string TemperatureUnit() const { return "°C"; }
private:
	int pin;
	float humidity;
	float temperature;
};

#endif // __DHT22_H__
