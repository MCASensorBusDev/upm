/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2016-2017 Intel Corporation.
 *
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include <string>
#include <vector>

#include "bma250e.hpp"
#include "bmg160.hpp"

namespace upm {

    /**
     * @library bmx055
     * @sensor bmi055
     * @comname 6DoF Sensor Module
     * @type accelerometer compass
     * @man bosch
     * @con i2c gpio spi
     * @web https://www.bosch-sensortec.com/bst/products/all_products/bmi055
     *
     * @brief API for the BMI055 6-axis Sensor Module
     *
     * The BMI055 is an inertial measurement unit (IMU) for the
     * detection of movements and rotations in 6 degrees of freedom
     * (6DoF). It reflects the full functionality of a triaxial, low-g
     * acceleration sensor and at the same time it is capable to measure
     * angular rates. Both - acceleration and angular rate - in three
     * perpendicular room dimensions, the x-, y- and z-axis.
     *
     * The BMI055 is essentially 2 separate devices in one: the BMA250E
     * Accelerometer and the BMG160 Gyroscope.  They are completely
     * independant of each other.
     *
     * This driver provides a very simple interface to these two devices.
     * If finer control is desired, you should just use the separate
     * BMA25E and BMG160 device classes directly.  This driver
     * simply initializes both devices, and provides a mechanism to
     * read accelerometer and gyroscope data from them.
     *
     * @snippet bmi055.cxx Interesting
     */

    class BMI055 {
    public:
        /**
         * BMI055 constructor.
         *
         * This device can support both I2C and SPI. For SPI, set the addr
         * to -1, and specify a positive integer representing the Chip
         * Select (CS) pin for the cs argument.  If you are using a
         * hardware CS pin (like edison with arduino breakout), then you
         * can connect the proper pin to the hardware CS pin on your MCU
         * and supply -1 for cs.  The default operating mode is I2C.
         *
         * @param accelBus I2C or SPI bus to use. -1 to skip initializing
         * this device.
         * @param accelAddr The address for this device.  -1 for SPI.
         * @param accelCS The gpio pin to use for the SPI Chip Select.  -1 for
         * I2C or for SPI with a hardware controlled pin.
         * @param gyroBus I2C or SPI bus to use. -1 to skip initializing
         * this device.
         * @param gyroAddr The address for this device.  -1 for SPI.
         * @param gyroCS The gpio pin to use for the SPI Chip Select.  -1 for
         * I2C or for SPI with a hardware controlled pin.
         */
        BMI055(int accelBus=BMA250E_DEFAULT_I2C_BUS,
               int accelAddr=BMA250E_DEFAULT_ADDR,
               int accelCS=-1,
               int gyroBus=BMG160_DEFAULT_I2C_BUS,
               int gyroAddr=BMG160_DEFAULT_ADDR,
               int gyroCS=-1);

        /**
         * BMI055 Destructor.
         */
        ~BMI055();

        /**
         * Update the internal stored values from sensor data.
         */
        void update();

        /**
         * Initialize the accelerometer and start operation.  This
         * function is called from the constructor so will not typically
         * need to be called by a user unless the device is reset or you
         * want to change these values.
         *
         * @param pwr One of the BMA250E_POWER_MODE_T values.  The default is
         * BMA250E_POWER_MODE_NORMAL.
         * @param range One of the BMA250E_RANGE_T values.  The default is
         * BMA250E_RANGE_2G.
         * @param bw One of the filtering BMA250E_BW_T values.  The default is
         * BMA250E_BW_250.
         */
        void initAccelerometer(
            BMA250E_POWER_MODE_T pwr=BMA250E_POWER_MODE_NORMAL,
            BMA250E_RANGE_T range=BMA250E_RANGE_2G,
            BMA250E_BW_T bw=BMA250E_BW_250);

        /**
         * Initialize the gyroscope and start operation.  This function is
         * called from the constructor so will not typically need to be
         * called by a user unless the device is reset or you want to
         * change these values.
         *
         * @param pwr One of the BMG160_POWER_MODE_T values.  The default is
         * BMG160_POWER_MODE_NORMAL.
         * @param range One of the BMG160_RANGE_T values.  The default is
         * BMG160_RANGE_250.
         * @param bw One of the filtering BMG160_BW_T values.  The default is
         * BMG160_BW_400_47.
         */
        void initGyroscope(BMG160_POWER_MODE_T pwr=BMG160_POWER_MODE_NORMAL,
                           BMG160_RANGE_T range=BMG160_RANGE_250,
                           BMG160_BW_T bw=BMG160_BW_400_47);

        /**
         * Return accelerometer data in gravities.  update() must have
         * been called prior to calling this method.
         *
         * @param x Pointer to a floating point value that will have the
         * current x component placed into it.
         * @param y Pointer to a floating point value that will have the
         * current y component placed into it.
         * @param z Pointer to a floating point value that will have the
         * current z component placed into it.
         */
        void getAccelerometer(float *x, float *y, float *z);

        /**
         * Return accelerometer data in gravities in the form of a
         * floating point vector.  update() must have been called prior to
         * calling this method.
         *
         * @return A floating point vector containing x, y, and z in
         * that order.
         */
        std::vector<float> getAccelerometer();

        /**
         * Return gyroscope data in degrees per second.  update() must
         * have been called prior to calling this method.
         *
         * @param x Pointer to a floating point value that will have the
         * current x component placed into it.
         * @param y Pointer to a floating point value that will have the
         * current y component placed into it.
         * @param z Pointer to a floating point value that will have the
         * current z component placed into it.
         */
        void getGyroscope(float *x, float *y, float *z);

        /**
         * Return gyroscope data in degrees per second in the form of a
         * floating point vector.  update() must have been called prior to
         * calling this method.
         *
         * @return A floating point vector containing x, y, and z in
         * that order.
         */
        std::vector<float> getGyroscope();


    protected:
        BMA250E *m_accel;
        BMG160 *m_gyro;

    private:
    };
}
