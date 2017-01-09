/*
 * Copyright (C) 2015 Christian Benjamin Ries
 * Website: https://github.com/cbries/utilities
 * License: MIT
 */

// Linux 
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

// C/C++
#include <iostream>
#include <exception>

// weatherapi
#include <dht22.h>

#define MAXTIMINGS 100

Dht22::Dht22(const int _pin)
	: humidity(0.f)
	, temperature(0.f)
	, pin(_pin)
{
	if(!bcm2835_init())
	{
		throw new std::string("bmp180() initialization failed");
	}
}

Dht22::~Dht22()
{ }

// \see https://github.com/adafruit/Adafruit_Python_DHT
// \see https://learn.adafruit.com/dht-humidity-sensing-on-raspberry-pi-with-gdocs-logging/software-install-updated
bool Dht22::Refresh()
{
    int counter = 0;
    int laststate = HIGH;
    int j=0;

    int bits[250], data[100];
    int bitidx = 0;

    // Set GPIO pin to output
    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);

    bcm2835_gpio_write(pin, HIGH);
    usleep(500000);  // 500 ms
    bcm2835_gpio_write(pin, LOW);
    usleep(20000);

    bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_INPT);

    data[0] = data[1] = data[2] = data[3] = data[4] = 0;

	unsigned long countForTimeout = 0;

    // wait for pin to drop?
    while (bcm2835_gpio_lev(pin) == 1) {
		usleep(1);
		countForTimeout++;
		if(countForTimeout >= 45000 ) {
			std::cerr << "GPIO PIN " << pin << " does not become ready." << std::endl;
			return false;
		}
    }

    // read data!
    for (int i=0; i< MAXTIMINGS; i++) 
	{
		counter = 0;
        while ( bcm2835_gpio_lev(pin) == laststate) 
		{
        	counter++;
            if (counter == 1000)
            	break;
        }

        laststate = bcm2835_gpio_lev(pin);
        if (counter == 1000) break;
        bits[bitidx++] = counter;

        if ((i>3) && (i%2 == 0)) 
		{
        	// shove each bit into the storage bytes
            data[j/8] <<= 1;
            if (counter > 200)
			{
				data[j/8] |= 1;
            }
			j++;	
		}
    }

    if ((j >= 39) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) ) {  // yay!
        float f, h;
        h = data[0] * 256 + data[1];
        h /= 10;

        f = (data[2] & 0x7F)* 256 + data[3];
            f /= 10.0;
            if (data[2] & 0x80) {
            f *= -1;
        }

        humidity = h;
        temperature = f;
    }
    return true;
}

