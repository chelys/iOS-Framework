//
//  MPSensorType.h
//  SenicSDK
//
//  Created by Kai-Dominik Kuhn on 23.06.13.
//  Copyright (c) 2013 MeasurePhone, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

enum SensorTypes : NSUInteger {
	//0 should stay undefined
    SensorTypeTemperature = 1,
    SensorTypeHumidity = 2,
    SensorTypeWind = 3,
    SensorTypeSpeed = 3,
    SensorTypeVolume = 4,
    SensorTypeDistance = 5,
    SensorTypeBattery = 6,
    SensorTypeMagnetometer = 7,
    SensorTypeMagnetometerDirection = 8,
    SensorTypeAcceloretorDirection = 9,
    SensorTypeAngle = 10
	};
typedef NSUInteger SensorTypes;
    
@interface SCSensorType : NSObject






@end
