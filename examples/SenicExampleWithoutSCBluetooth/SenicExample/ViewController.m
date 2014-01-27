//
//  ViewController.m
//  SenicExample
//
//  Created by Kai-Dominik Kuhn on 26.01.14.
//  Copyright (c) 2014 measurePhone, Inc. All rights reserved.
//

#import "ViewController.h"
#import <SenicSDK/SenicSDK.h>

@interface ViewController ()<CBCentralManagerDelegate, SCDeviceDelegate> {
    CBCentralManager *centralManager;
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
    
    centralManager = [[CBCentralManager alloc] initWithDelegate:self queue:nil];
    
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

#pragma mark - BLE Central Manager Delegate

-(void)centralManagerDidUpdateState:(CBCentralManager *)central {
    [self checkCentralManagerState];
}

-(void)checkCentralManagerState {
    NSString * str;
    
    switch (centralManager.state) {
        case CBCentralManagerStateUnknown:
        str = @"Unknown\n";
        break;
        case CBCentralManagerStateUnsupported:
        str = @"Unsupported\n";
        break;
        case CBCentralManagerStateUnauthorized:
        str = @"Unauthorized\n";
        break;
        case CBCentralManagerStateResetting:
        str = @"Resetting\n";
        break;
        case CBCentralManagerStatePoweredOn:
        str = @"PoweredOn\n";
        [self doScanWithDuplicates:FALSE];
        break;
        case CBCentralManagerStatePoweredOff:
        str = @"PoweredOff\n";
        break;
    }
    NSLog(@"CentralManager State is %@",str);
}


// Start scanning for the Device
- (void)doScanWithDuplicates:(BOOL) duplicates {
    
//    #
//    #
//    #
//    Needed that we can prepare everything to handle new found senic devices
    [[SCDeviceHandler defaultSCPeripherals] prepareForRescan];
//    #
//    #
//    #
    
    NSDictionary *options = [NSDictionary dictionaryWithObjectsAndKeys:[NSNumber numberWithBool:duplicates], CBCentralManagerScanOptionAllowDuplicatesKey, nil];
    
    [centralManager scanForPeripheralsWithServices:nil options:options];
    [NSTimer scheduledTimerWithTimeInterval:30 target:self selector:@selector(stopScan) userInfo:nil repeats:FALSE];
    
    NSLog(@"Started Scanning");
}


// Found a Device, connect to it
-(void) centralManager:(CBCentralManager *)central
 didDiscoverPeripheral:(CBPeripheral *)peripheral
     advertisementData:(NSDictionary *)advertisementData
                  RSSI:(NSNumber *)RSSI{
    
    if (senicDevice != nil) return; // We going to handle only the first senic device we found
    
    //    #!!!
    //    #!!!
    //    #!!!
    //    Needed that you can get a instance for a senic device. If the function returns nil it isn't a senic device and you can continue to handle this peripheral.
    senicDevice = [[SCDeviceHandler defaultSCPeripherals] checkAndCreateSenicPeripheral:central peripheral:peripheral advertisementData:advertisementData RSSI:RSSI];
    //    #!!!
    //    #!!!
    //    #!!!
    
    
    
    if (senicDevice != nil) {
        NSError *e = nil;
        [senicDevice connectToSenicDevice:&e];
        if (e) {
            senicDevice = nil;
            [self printErrorMessage:e];
            return;
        }
        
        [senicDevice setDelegate:self];
        _lb_measurement.text = @"Connected";
        
        [NSTimer scheduledTimerWithTimeInterval:3 target:self selector:@selector(setSlider) userInfo:nil repeats:FALSE];
    }
}

// Connected to the Device, now discover the services
- (void) centralManager:(CBCentralManager *)central didConnectPeripheral:(CBPeripheral *)peripheral {
    [peripheral discoverServices:nil];
}

// Fail!
-(void) centralManager:(CBCentralManager *)central didFailToConnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error {
    NSLog(@"Connection failed:\n%@\n", [error localizedDescription]);
    
    //    #!!!
    //    #!!!
    //    #!!!
    //    We need to know that the senic device isn't connected anymore
    SCDevice *sDevice = [[SCDeviceHandler defaultSCPeripherals] didDisconnectPeripheral:peripheral];
    //    #!!!
    //    #!!!
    //    #!!!
    
    if (sDevice != nil) {
    	senicDevice = nil;
    }
    
    [self doScanWithDuplicates:FALSE];
}

// Disconnected
-(void) centralManager:(CBCentralManager *)central didDisconnectPeripheral:(CBPeripheral *)peripheral error:(NSError *)error {
	// Tell SCDeviceHandler that a peripheral disconnected
    //    #!!!
    //    #!!!
    //    #!!!
    //    We need to know that the senic device isn't connected anymore
    SCDevice *sDevice = [[SCDeviceHandler defaultSCPeripherals] didDisconnectPeripheral:peripheral];
    //    #!!!
    //    #!!!
    //    #!!!
    
    
    if (sDevice != nil) {
    	senicDevice = nil;
    }
    
    [self stopScan];
    [self checkCentralManagerState];
    
    NSLog(@"Disconnected\n");
}

- (void) stopScan {
    [centralManager stopScan];
}

- (NSArray *) connectedSenicDevices {
    return [[SCDeviceHandler defaultSCPeripherals] connectedSenicDevices];
}

#pragma Helper methods
- (void) printErrorMessage:(NSError *) error {
    UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"Error"
                                                        message:[error description]
                                                       delegate:nil
                                              cancelButtonTitle:@"OK"
                                              otherButtonTitles:nil];
    [alertView show];
}


#pragma mark - SCDeviceDelegate
- (void) receivedValue:(NSNumber *)value forSensorType:(SensorTypes)sensorType {
    _lb_measurement.text = [NSString stringWithFormat:@"%f m", [value floatValue] / 1000.0f];
}

@end
