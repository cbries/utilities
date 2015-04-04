
# Publish weather data online

## Development Environment
- install `lighttpd` with PHP5 support
- Call `sudo apt-get install lighttpd php5-cgi`
- Enable fastcgi support:
  - `sudo lighty-enable-mod fastcgi`
  - `sudo lighty-enable-mod fastcgi-php`
  - `sudo service lighttpd force-reload`
