//
//  ViewController.m
//  SenicExample
//
//  Created by Kai-Dominik Kuhn on 26.01.14.
//  Copyright (c) 2014 measurePhone, Inc. All rights reserved.
//

#import "ViewController.h"
#import <SenicSDK/SenicSDK.h>

@interface ViewController ()<SCBluetoothDelegate, SCDeviceDelegate> {
    SCBluetooth *bluetoothManager;
    SCDevice *senicDevice;
}

@property (nonatomic, strong) IBOutlet UILabel *lb_measurement;
@property (nonatomic, strong) IBOutlet UISlider *sl_SoundFrequenz;
@property (nonatomic, strong) IBOutlet UISlider *sl_LEDBrightness;
@property (nonatomic, strong) IBOutlet UISlider *sl_DisplayBrightness;

- (IBAction)takeMeasurementPushed:(id)sender;

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    
    bluetoothManager = [SCBluetooth defaultSCBluetooth];
    [bluetoothManager setDelegate:self];
    
    _lb_measurement.text = @"Not Connected";
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)takeMeasurementPushed:(id)sender {
    if (senicDevice != nil) {
        [senicDevice takeMeasurement];
    }
}

- (IBAction)finishedChangingSound:(UISlider *)sender {
    if (senicDevice != nil) {
        [senicDevice setSoundFrequency:sender.value];
    }
}

- (IBAction)finishedChangingLED:(UISlider *)sender {
    if (senicDevice != nil) {
        [senicDevice setLEDBrightness:sender.value];
    }
}

- (IBAction)finishedChangingDisplay:(UISlider *)sender {
    if (senicDevice != nil) {
        [senicDevice setDisplayBrightness:sender.value];
    }
}

- (void) setSlider {
    if (senicDevice != nil) {
        _sl_DisplayBrightness.minimumValue = 0;
        _sl_DisplayBrightness.maximumValue = 100;
        _sl_DisplayBrightness.value = [senicDevice displayBrightness];
        
        _sl_LEDBrightness.minimumValue = 0;
        _sl_LEDBrightness.maximumValue = 100;
        _sl_LEDBrightness.value = [senicDevice LEDBrightness];
        
        _sl_SoundFrequenz.minimumValue = 0;
        _sl_SoundFrequenz.maximumValue = 100;
        _sl_SoundFrequenz.value = [senicDevice soundFrequency];
    }
}

#pragma mark - SCBluetoothDelegate
- (void) detectedDeviceListDidChange:(NSArray *)deviceList {
    if (senicDevice == nil) {
        senicDevice = deviceList[0];
        
        NSError *e = nil;
        [senicDevice connectToSenicDevice:&e];
        if(e) {
            [self printErrorMessage:e];
            return;
        }
        
        [senicDevice setDelegate:self];
        _lb_measurement.text = @"Connected";
        
        [NSTimer scheduledTimerWithTimeInterval:3 target:self selector:@selector(setSlider) userInfo:nil repeats:FALSE];
    }
}

- (void) appleBluetoothModuleDidFail:(NSError *)error {
    senicDevice = nil;
    _lb_measurement.text = @"Not Connected";
}

- (void) senicDeviceConnectionDidEnd:(SCDevice *)device {
    senicDevice = nil;
    _lb_measurement.text = @"Not Connected";
    [bluetoothManager startScan];
}

- (void) senicDeviceConnectionDidFail:(NSError *)error {
    senicDevice = nil;
    _lb_measurement.text = @"Not Connected";
    [bluetoothManager startScan];
}

- (void) printErrorMessage:(NSError *) error {
    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Error"
                                                        message:[error description]
                                                       delegate:nil
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles:nil];
    [alertView show];
}

- (void) receivedValue:(NSNumber *)value forSensorType:(SensorTypes)sensorType {
    _lb_measurement.text = [NSString stringWithFormat:@"%f m", [value floatValue] / 1000.0f];
}

@end
