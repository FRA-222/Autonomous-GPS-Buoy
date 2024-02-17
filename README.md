How to build PlatformIO based project
=====================================

1. [Install PlatformIO Core](https://docs.platformio.org/page/core.html)
2. Download [development platform with examples](https://github.com/platformio/platform-renesas-ra/archive/develop.zip)
3. Extract ZIP archive
4. Run these commands:

Ceci est un test de mise à jour du Repo Git

```shell
# Change directory to example
$ cd platform-renesas-ra/examples/arduino-iot-cloud-basic

# Build project
$ pio run

# Upload firmware
$ pio run --target upload

# Clean build files
$ pio run --target clean
```