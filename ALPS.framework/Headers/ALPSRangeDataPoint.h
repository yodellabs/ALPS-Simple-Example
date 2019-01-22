/******************************************************************************
 *  ALPS, The Acoustic Location Processing System.
 *  Copyright (C) 2018, Yodel Labs Inc.
 *  All rights reserved.
 *
 *  This software is the property of Yodel Labs Inc. Source may
 *  be modified, but this license does not allow distribution.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 *  Contributing Author(s):
 *  Patrick Lazik
 *  Nick Wilkerson
 *
 *******************************************************************************/

/**
 * Class containing raw ultrasound ranging data from an ALPS beacon.
 */

#import <Foundation/Foundation.h>

@interface ALPSRangeDataPoint : NSObject

-(id)init;

/**
 *  Initializer
 *  @param bleMac The BLE MAC address of the beacon that sent the ultrasound packet.
 *  @param slot The time slot number of the beacon that sent the ultrasound packet.
 *  @param range The range to the beacon from the iOS device.
 *  @param rssi The ultrasound packet RSSI (no units).
 *  @param timeStamp The timestamp of when the ultrasound packet was received in Unix time.
 */
-(id)initWithMAC:(NSString*)bleMac
            slot:(UInt8)slot
           range:(float)range
            rssi:(double)rssi
       timeStamp:(double)timeStamp;


@property NSString *bleMac;
@property UInt8 slot;
@property float range;
@property float rssi;
@property double timeStamp;
@property NSSortDescriptor *highestToLowest;

@end
