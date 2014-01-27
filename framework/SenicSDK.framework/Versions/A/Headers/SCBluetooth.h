//
//  MPBluetoohConnection.h
//  SenicSDK
//
//  Created by Kai-Dominik Kuhn on 27.02.13.
//  Copyright (c) 2013 MeasurePhone, Inc. All rights reserved.
//

/*
 *  @file SCBluetooth.h
 *  @framework SenicSDK
 *
 *  @Author Kai-Dominik Kuhn
 *
 *  @discussion Handling of Bluetooth connection to Senic device
 *
 *  @copyright 2013 MeasurePhone, Inc. All rights reserved.
 */

#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>

#import "SCDevice.h"

/*!
 *  @enum SCBluetoothErrorCode
 *
 *  @discussion Represents apple bluetooth modules Errors.
 *
 */
typedef enum {
    SCBluetoothErrorUnknown = 1,
    SCBluetoothErrorUnsupported = 2,
    SCBluetoothErrorUnauthorized = 3,
    SCBluetoothErrorPoweredOff = 4
} SCBluetoothErrorCode ;

/*!
 *  @protocol SCBluetoothDelegate
 *
 *  @discussion     The delegate of a {@link SCBluetooth} object must adopt the <code>SCBluetoothDelegate</code> protocol. The methods provides informations about detected Senic devices and the connection process.
 */
@protocol SCBluetoothDelegate <NSObject>
@required

/*!
 *  @method detectedDeviceListDidChange:
 *
 *  @param deviceList   A List of {@link SCDevice} which were detected during the scanning process
 *
 *  @discussion         This method is invoked when the scan process {@link startScan} found a senic device. For every senic device which was found, this method is called.
 */
- (void) detectedDeviceListDidChange:(NSArray *) deviceList;

/*!
 *  @method appleBluetoothModuleDidFail:
 *
 *  @param error    The cause of the failure
 *
 *  @discussion     This method is invoked if either <code>SCBluetooth</code> is initiated or the bluetooth module state changes and a bluetooth low energy connections is not possible.
 *
 *  @see    {@link SCBluetoothErrorCode}
 */
- (void) appleBluetoothModuleDidFail:(NSError *) error;

/*!
 *  @method senicDeviceConnectionDidFail:
 *
 *  @param error    The cause of the failure
 *
 *  @discussion     This method is invoked if {@link SCDevice connectToSenicDevice:} initiated a connection which failed to complete. As connection attempts do not timeout, the failure of a connection is atypical and usually indicative of a transient issue.
 */
- (void) senicDeviceConnectionDidFail:(NSError *) error;

/*!
 *  @method senicDeviceConnectionDidEnd:
 *
 *  @param device       The <code>SCDevice</code> that has disconnected.
 *  @param error        If an error occurred, the cause of the failure.
 *
 *  @discussion         This method is invoked upon the disconnection of a Senic device that was connected by {@link SCDevice connectToSenicDevice:}. If the disconnection
 *                      was not initiated by {@link SCDevice disconnectSenicDevice}, the cause will be detailed in the <i>error</i> parameter. Once this method has been
 *                      called, no more methods will be invoked on <i>SCDevice</i>'s {@link SCDevice SCDeviceDeleagte}
 */
- (void) senicDeviceConnectionDidEnd:(SCDevice *) device;
@end


/*!
 *  @class SCBluetooth
 *
 *  @discussion Entry point to the bluetooth connection of a senic device.
 */
@interface SCBluetooth : NSObject <CBCentralManagerDelegate, CBPeripheralDelegate> {
    id <SCBluetoothDelegate> delegate;
}

/*!
 *  @property delegate
 *
 *  @discussion The delegate object that will receive <code>SCBluetooth</code> events.
 *
 */
@property (nonatomic, retain) id <SCBluetoothDelegate>delegate;

/*!
 *  @property scanAfterAppDidBecomeActive
 *
 *  @discussion     Indicates if <code>SCBluetooth</code> should do a rescan after the application becomes active again.
 */
@property (nonatomic, assign) BOOL scanAfterAppDidBecomeActive;

/*!
 *  @property scanTime
 *
 *  @discussion  Determines the time of a scan process before it gets stopped. The Default is 30 seconds
 *
 */
@property (nonatomic, assign) NSInteger scanTime;


/*!
 *  @property scanDuringBackgroundMode
 *
 *  @discussion     Indicates if <code>SCBluetooth</code> should continue scanning during backround mode. The Default is NO.
 *  
 *  @warning        If you set this property to YES, the power consumption of the apple device increases.
 */
@property (nonatomic, assign) BOOL scanDuringBackgroundMode;

/*!
 *  @method defaultSCBluetooth
 *
 *  @discussion     Since only one <code>SCBluetooth</code> object should exist, the <code>SCBluetooth</code> object should either be initiated with this mehod or receive the existing <code>SCBluetooth</code> object.
 */
+ (id)defaultSCBluetooth;

/*!
 *  @method startScan
 *
 *  @discussion     This method starts a scanning process. If the apple bluetooth module isn't ready, {@link appleBluetoothModuleDidFail:} is called.
 */
- (void) startScan;

/*!
 *  @method stopScan
 *
 *  @discussion     This method stops a scan process if there already is a scanning process runing which was started by {@link startScan}.
 */
- (void) stopScan;

/*!
 *  @method startScconnectedSenicDevicesan
 *
 *  @return         A list of connected of {@link SCDevice} objects is returned
 *  @discussion     This method returns a list of {@link SCDevice} objects which are already connected
 */
- (NSArray *) connectedSenicDevices;


@end
