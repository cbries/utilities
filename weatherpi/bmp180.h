/*
 * Copyright (C) 2015 Christian Benjamin Ries
 * Website: https://github.com/cbries/utilities
 * License: MIT
 */

#ifndef __BMP180_H__
#define __BMP180_H__

#define BMP180_ADDRESS 0x77

extern float BMP180_Temperature;
extern float BMP180_Pressure;
extern float BMP180_Altitude;

class Bmp180
{
public:
	Bmp180();
	~Bmp180();
private: 
	Bmp180(const Bmp180 & obj) { }
	Bmp180 & operator=(const Bmp180 & obj) { }
public:
	bool Refresh();
	inline float Temperature() const { return temperature; }
	inline std::string TemperatureUnit() const { return temperatureUnit; }
	inline float Pressure() const { return pressure; }
	inline std::string PressureUnit() const { return pressureUnit; }
	inline float Altitude() const { return altitude; }
private:
	bool parse(const std::string & values);
	float temperature;
	std::string temperatureUnit;
	float pressure;
	std::string pressureUnit;
	float altitude;
};

#endif // __BMP180_H__
