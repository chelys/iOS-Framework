//
//  MPPeripheralSensor.h
//  SenicSDK
//
//  Created by Kai-Dominik Kuhn on 27.02.13.
//  Copyright (c) 2013 MeasurePhone, Inc. All rights reserved.
//

/*
 *  @file SCDeviceHandler
 *  @framework SenicSDK
 *
 *  @Author Kai-Dominik Kuhn
 *
 *  @discussion This class take cares of all communciations to a senic devic.
 *
 *  @copyright 2013 MeasurePhone, Inc. All rights reserved.
 */

#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>

#import "SCSensorType.h"


/*!
 *  @struct SCMagnetometerRaw
 */
typedef struct {
    NSInteger x;
    NSInteger y;
    NSInteger z;
} SCMagnetometerRaw;

/*!
 *  @struct SCAccelerometerRaw
 */
typedef struct {
    NSInteger x;
    NSInteger y;
    NSInteger z;
} SCAccelerometerRaw;

/*!
 *  @struct SCGyroscopeRaw
 */
typedef struct {
    NSInteger x;
    NSInteger y;
    NSInteger z;
} SCGyroscopeRaw;

/*!
 *  @struct SCTemperatureRaw
 */
typedef struct {
    NSInteger tmp;
} SCTemperatureRaw;


/*!
 *  @struct SCAGM_Raw
 */
typedef struct {
    SCMagnetometerRaw magnetometer;
    SCAccelerometerRaw accelerometer;
    SCTemperatureRaw temperature;
    SCGyroscopeRaw gyroscope;
} SCAGM_Raw;

/*!
 *  @struct SCTimeout
 */
typedef struct {
    NSInteger t1;
    NSInteger t2;
} SCTimeout;

/*!
 *  @enum SCSettingsValues
 */
typedef enum {
    SoundFrequency,
    VibrationStrength,
    LEDBrightness,
    DisplayBrightness,
} SCSettingsValues;

/*!
 *  @enum SCPowerTimeouts
 */
typedef enum {
    SleepTimeout,
    OffTimeout,
} SCPowerTimeouts;

/*!
 *  @enum SCPowerStatus
 */
typedef enum {
    SleepStatus,
    OffStatus,
} SCPowerStatus;

/*!
 *  @enum SCStates
 */
typedef enum {
    STATUS_SLEEPING,
    STATUS_ON,
    STATUS_OFF,
} SCStates;

/*!
 *  @protocol SCDeviceDelegate
 *
 *  @discussion     The delegate of a {@link SCDevice} object must adopt the <code>SCDeviceDelegate</code> protocol. The methods inform about incoming information from a senic device
 */
@protocol SCDeviceDelegate <NSObject>
@required

/*!
 *  @method receivedValue:forSensorType:
 *
 *  @param value        A string with contains a measurement value
 *  @param sensorType   Type of Sensor which provided the measurement
 *
 *  @discussion         This method is called after a measurement from the senic device was received
 *
 *  @see {@link SensorTypes}
 */
- (void) receivedValue:(NSNumber *) value forSensorType:(SensorTypes) sensorType;

@optional
/*!
 *  @method receivedBatteryValue:
 *
 *  @param percentage   Battery value as a percentage
 *
 *  @discussion         Method is called after the battery value changed.
 */
- (void) receivedBatteryValue:(int) percentage;

/*!
 *  @method receivedAGMValuesRaw:
 *
 *  @param value   {@link SCAGM_Raw} structure which contains the raw value of the temperature, magnetometer, accelerometer and gyroscope
 *
 *  @discussion         Method is called after a AGM value was received.
 */
- (void) receivedAGMValuesRaw:(SCAGM_Raw) value;

@end


/*!
 *  @class SCDevice
 *
 *  @discussion This class represents a senic device with all outgoing and incoming communication.
 */
@interface SCDevice : NSObject <CBPeripheralDelegate> {
}

/*!
 *  @property delegate
 *
 *  @discussion The delegate object that will receive <code>SCDevice</code> events.
 *
 */
@property (nonatomic, retain) id <SCDeviceDelegate>delegate;

/*!
 *  @property addSenicDeviceLength
 *
 *  @discussion     If the value is YES, then the device length will be added automatically for every measurement. The default value is NO.
 */
@property (nonatomic, assign) BOOL addSenicDeviceLength;

/*!
 *  @property name
 *
 *  @discussion     This property represents the name of the senic device
 */
@property (nonatomic, retain) NSString *name;

/*!
 *  @property batteryValue
 *
 *  @discussion     This property represents the last received battery value.
 */
@property (nonatomic, readonly) NSInteger batteryValue;

/*!
 *  @property displayBrightness
 *
 *  @discussion     The propery is adjustable from 0 to 100 whereas 0 means lowest power level and 100 means full power.
 */
@property (nonatomic, assign) NSInteger displayBrightness;

/*!
 *  @property soundFrequency
 *
 *  @discussion     The propery is adjustable from 0 to 100 whereas 0 means lowest power level and 100 means full power.
 */
@property (nonatomic, assign) NSInteger soundFrequency;

/*!
 *  @property LEDBrightness
 *
 *  @discussion     The propery is adjustable from 0 to 100 whereas 0 means lowest power level and 100 means full power.
 */
@property (nonatomic, assign) NSInteger LEDBrightness;

/*!
 *  @property laserTimeout
 *
 *  @discussion     The propery is adjustabel from 0 to 999 and represents seconds until the laser will be turned off.
 */
@property (nonatomic, assign) NSInteger laserTimeout;

/*!
 *  @property displayTimeout
 *
 *  @discussion     The propery is adjustabel from 0 to 999 and represents seconds until the laser will be turned off.
 */
@property (nonatomic, assign) NSInteger displayTimeout;

/*!
 *  @property deviceTimeout
 *
 *  @discussion    The propery is adjustabel from 0 to 999 and represents seconds until the laser will be turned off.
 */
@property (nonatomic, assign) NSInteger deviceTimeout;

/*!
 *  @property measurementsPerSecond
 *
 *  @discussion     The propery is adjustable from 1 to 6 and change the number of measurements per second during the continues measurement mode.
 */
@property (nonatomic, assign) NSInteger measurementsPerSecond;

/*!
 *  @property softwareVersion
 *
 *  @discussion     The property stands for the software version which runs on the senic device.
 */
@property (nonatomic, readonly) NSString *softwareVersion;

/*!
 *  @property hardwareVersion
 *
 *  @discussion     This property stands for the hardware version of the device which is currently connected.
 */
@property (nonatomic, readonly) NSString *hardwareVersion;


/*!
 *  @method isConnected
 *
 *  @return BOOL value
 *
 *  @discussion     Indicates if the current senic device is connected.
 */
- (BOOL) isConnected;

/*!
 *  @method RSSILevel
 *
 *  @param callback
 *
 *  @discussion     After the new RSSILevel was received, the callback method will be called whichs contains the new RSSI value in dBm
 */
- (void) RSSILevel:(void (^)(int rssi)) callback;

/*!
 *  @method takeMeasurement
 *
 *  @discussion     This method sends out a measure command to the senic device. If the laser is not turned on, it will first turn on the laser. The next call will then take a measurement and {@link receivedValue:forSensorType:} provide the value.
 */
- (void) takeMeasurement;

/*!
 *  @method connectToSenicDevice:
 *
 *  @param error    This parameter represents an error pointer which will be set to an <code>NSError</code> object if an error occures.
 *
 *  @discussion     This method establishes a connection to the senic device.
 */
- (void) connectToSenicDevice:(NSError **)error;

/*!
 *  @method disconnectSenicDevice
 *
    @param error    This parameter represents an error pointer which will be set to an <code>NSError</code> object if an error occures.
 *
 *  @discussion     This method disconnects the senic device.
 */
- (void) disconnectSenicDevice:(NSError **)error;


@end
