CDPVERSION = 4.0.0
TYPE = library

HEADERS += \
    I2CAdapter.h \
    I2CDevice.h \
    I2CDeviceChannelGroup.h \
    I2CException.h \
    I2CHelpers.h \
    I2CIOBuilder.h \
    i2cio.h \
    I2CIOServer.h

SOURCES += \
    I2CDevice.cpp \
    I2CDeviceChannelGroup.cpp \
    I2CException.cpp \
    I2CIOBuilder.cpp \
    I2CIOServer.cpp

linux*:SOURCES += \
    Linux/I2CAdapter.cpp

win*:SOURCES += \
    Win/I2CAdapter.cpp

DISTFILES += \
    I2CIO.xml \
    README.md \
    Templates/Models/I2CDeviceChannelGroup.xml \
    Templates/Models/I2CDevice.xml \
    Templates/Models/I2CIOServer.xml \
    Templates/Models/LSM9DS1AccelerometerGyroscope.xml \
    Templates/Models/LSM9DS1Magnetometer.xml

load(cdp)
