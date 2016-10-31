# I2CIO

The I2CIO library provides an I2CIOServer that enables communicating with external devices over I2C bus.

The only supported device is STMicroelectronics LSM9DS1. LSM9DS1AccelerometerGyroscope provides access to accelerometer
and gyroscope and LSM9DS1Magnetometer provides access to magnetometer. Access is divided divided into two different
nodes because LSM9DS1 has two physical addresses - one for accelerometer and gyroscope and another for magnetometer. For
example, on Raspberry Pi Sense HAT the addresses are `0x6a` and `0x1c` respectfully.
