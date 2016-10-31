CDPVERSION = 4.0.0
TYPE = library

HEADERS += \
    I2CAdapter.h \
    I2CDeviceFactory.h \
    I2CDevice.h \
    I2CIOBuilder.h \
    i2cio.h \
    I2CIOServer.h \
    LSM9DS1AccelerometerGyroscope.h \
    LSM9DS1Magnetometer.h

SOURCES += \
    I2CDevice.cpp \
    I2CDeviceFactory.cpp \
    I2CIOBuilder.cpp \
    I2CIOServer.cpp \
    LSM9DS1AccelerometerGyroscope.cpp \
    LSM9DS1Magnetometer.cpp

linux*:SOURCES += \
    Linux/I2CAdapter.cpp

win*:SOURCES += \
    Win/I2CAdapter.cpp

DISTFILES += \
    I2CIO.xml \
    README.md \
    Templates/Models/I2CDevice.xml \
    Templates/Models/I2CIOServer.xml \
    Templates/Models/LSM9DS1AccelerometerGyroscope.xml \
    Templates/Models/LSM9DS1Magnetometer.xml

load(cdp)
