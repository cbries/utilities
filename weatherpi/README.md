
# Reference
- Have a look at this website: http://blog.dioty.co/2014/12/raspberry-pi-sensors-and-dioty-mqtt.html

# Installation
- In case you have the bcm2835 and Wiring libraries already installed, just enter only `make clean && make` and afterwards call `sudo ./weatherpi` -> The following output should show up:
```bash
pi@raspberrypi ~/gitrepos/utilities/weatherpi $ sudo ./weatherpi
DHT22
 Temperature = 21.2 °C
 Humidity = 45.5 %
BMP180
 Temperature = 21.7 C
 Pressure = 983.82 hPa
 Altitude = 248.3
```

- Otherwise we recommend to follow section *Requirements*!

# Requirements
- `sudo apt-get install python g++ wget libssl-dev -y`
- `sudo apt-get install i2c-tools -y`
- `sudo apt-get install python-smbus -y`
- `sudo adduser pi i2c`
- Change the Linux kernel modules blacklist:
```
# /etc/modprobe.d/raspi-blacklist.conf
blacklist spi-bcm2708
#blacklist i2c-bcm2708
```
- Add `i2c-dev` to the end of file `/etc/modules`.
- Download and install the bcm2835 library:
```bash
## Can be changed, just have a look at http://www.airspayce.com/mikem/bcm2835/
# Option 1
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.38.tar.gz
# or use the version in this repository
# Option 2
cd thirdparty/
ls -al
##
tar zxvf bcm2835-1.xx.tar.gz
cd bcm2835-1.xx
./configure
make
sudo make check
sudo make install
```
- Install Gordon's Wiring library: `https://git.drogon.net/?p=wiringPi;a=summary` -> Click on "Snapshot" and store it where you want.

