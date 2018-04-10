CDPVERSION = 4.4
TYPE = library
PROJECTNAME = I2CIO

API_HEADERS += \
    i2cio.h \
    I2CIOBuilder.h \

HEADERS += \
    $${API_HEADERS} \
    ADS1115IOServer.h \
    ADS1115Reader.h \
    I2CAdapter.h \
    I2CDevice.h \
    I2CDeviceChannelGroup.h \
    I2CException.h \
    I2CHelpers.h \
    I2CIOServer.h

SOURCES += \
    ADS1115IOServer.cpp \
    I2CDevice.cpp \
    I2CDeviceChannelGroup.cpp \
    I2CException.cpp \
    I2CIOBuilder.cpp \
    I2CIOServer.cpp

linux*:SOURCES += \
    Linux/ADS1115Reader.cpp \
    Linux/I2CAdapter.cpp

win*:SOURCES += \
    Win/ADS1115Reader.cpp \
    Win/I2CAdapter.cpp

DISTFILES += \
    I2CIO.xml \
    QDoc/i2cio.qdocconf \
    QDoc/src/i2cio.qdoc \
    README.md \
    Templates/Models/I2CDeviceChannelGroup.xml \
    Templates/Models/I2CDevice.xml \
    Templates/Models/I2CIOServer.xml \
    Templates/Models/LSM9DS1AccelerometerGyroscope.xml \
    Templates/Models/LSM9DS1Magnetometer.xml \
    Templates/Models/I2CIO.ADS1115IOServer.xml

load(cdp)
