//
//  MPPeripherals.h
//  SenicSDK
//
//  Created by Kai-Dominik Kuhn on 16.12.13.
//  Copyright (c) 2013 measurePhone, Inc. All rights reserved.
//

/*
 *  @file SCDeviceHandler
 *  @framework SenicSDK
 *
 *  @Author Kai-Dominik Kuhn
 *
 *  @discussion This Class is mostly for users that do not use {@link SCBluetooth} because they need to communicate with other bluetooth low energy devices
 *
 *  @copyright 2013 MeasurePhone, Inc. All rights reserved.
 */
#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>

#import "SCDevice.h"

/*!
 *  @class SCDeviceHandler
 *
 *  @discussion Entry point to handle senic peripherals.
 */
@interface SCDeviceHandler : NSObject

/*!
 *  @method defaultSCPeripherals
 *
 *  @discussion     Since only one <code>SCDeviceHandler</code> object should exist, the <code>SCDeviceHandler</code> object should either be initiated with this mehod or receive the existing <code>SCDeviceHandler</code> object.
 */
+ (id) defaultSCPeripherals;

/*!
 *  @method checkAndCreateSenicPeripheral:peripheral:advertisementData:RSSI:
 *
 *  @param central              The central manager which provided an update.
 *  @param peripheral           A <code>CBPeripheral</code> object.
 *  @param advertisementData    A dictionary containing any advertisement and scan response data.
 *  @param RSSI                 The current RSSI of <i>peripheral</i>, in dBm. A value of <code>127</code> is reserved and indicates the RSSI
 *
 *  @return         {@link SCDevice} object is returned if the periperal is a senic device; otherwise it will return nil.
 *
 *  @discussion     If the {@link SCBluetooth} class is not used, <code>SCDeviceHandler</code> should be notified about found peripherals.
 *
 *  @seealso                    CBAdvertisementData.h

 */
- (SCDevice *) checkAndCreateSenicPeripheral:(CBCentralManager *)central
                    peripheral:(CBPeripheral *) peripheral
             advertisementData:(NSDictionary *)advertisementData
                          RSSI:(NSNumber *)RSSI;


/*!
 *  @method prepareForRescan
 *
 *  @discussion     Before a new scan is started, this method should be called.
 */
- (void) prepareForRescan;

/*!
 *  @method detectedSenicDevices
 *
 *  @return         A list of {@link SCDevice} objects
 *
 *  @discussion     A list of {@link SCDevice} objects which was detected during last scanning process.

 */
- (NSArray *) detectedSenicDevices;

/*!
 *  @method connectedSenicDevices
 *
 *  @return         A list of {@link SCDevice} objects
 *
 *  @discussion     A list of {@link SCDevice} objects which are actually connected to this device.
 */
- (NSArray *) connectedSenicDevices;

/*!
 *  @method connectToPeripheral:
 *
 *  @param peripheral   <code>CBPeripheral</code> to which a connection should be established
 *
 *  @discussion     This method establishes a connetion to the peripheral as long as it is a senic device.
 */
- (void) connectToPeripheral:(CBPeripheral *) peripheral;

/*!
 *  @method connectToSenicDevice:
 *
 *  @param device   {@link SCDevice} objecte to which a connection should be established
 *
 *  @discussion     This method establishes a connetion to the senic device.
 */
- (void) connectToSenicDevice:(SCDevice *) device;

/*!
 *  @method disconnectPeripheral:
 *
 *  @param peripheral  <code>CBPeripheral</code> object which should be disconnected
 *
 *  @discussion     This method disconnects the peripheral.
 */
- (void) disconnectPeripheral:(CBPeripheral *) peripheral;

/*!
 *  @method disconnectSenicDevice:
 *
 *  @param device  {@link SCDevice} object which should be disconnected
 *
 *  @discussion     This method disconnects the senic device.
 */
- (void) disconnectSenicDevice:(SCDevice *) device;

/*!
 *  @method didDisconnectPeripheral:
 *
 *  @param peripheral  <code>CBPeripheral</code> object which was disconnected
 *
 *  @return         {@link SCDevice} object which was disconnected
 *
 *  @discussion     This method should be called by the <code>centralManagerDelegate</code> after a peripheral was disconnected.
 */
- (SCDevice *) didDisconnectPeripheral:(CBPeripheral *) peripheral;

/*!
 *  @method didDisconnectSenicDevice:
 *
 *  @param device   {@link SCDevice} object which was disconnected
 *
 *  @return         {@link SCDevice} object which got disconnected
 *
 *  @discussion     This method should be called by the <code>centralManagerDelegate</code> after a senic device was disconnected.
 */
- (SCDevice *) didDisconnectSenicDevice:(SCDevice *)peripheral;

@end
