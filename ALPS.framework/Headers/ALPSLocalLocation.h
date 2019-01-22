/******************************************************************************
 *  ALPS, The Acoustic Location Processing System.
 *  Copyright (C) 2017, Yodel Labs
 *  All rights reserved.
 *
 *  This software is the property of Yodel Labs. Source may
 *  be modified, but this license does not allow distribution.  Binaries built
 *  for research purposes may be freely distributed, but must acknowledge
 *  Yodel Labs.  No other use or distribution can be made
 *  without the written permission of the authors and Yodel Labs.
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

#import <Foundation/Foundation.h>

/**
 * Class containing location coordinates and accuracy. ALPS uses this class to return locations.
 */


typedef struct {
    float x;
    float y;
    float z;
} ALPSLocationCoordinate3D;

ALPSLocationCoordinate3D ALPSLocationCoordinate3DMake(double x, double y, double z);


@interface ALPSLocalLocation : NSObject

/**
 *  Initializer for ALPSLocalLocation.
 *  @param x The x coordinate.
 *  @param y The y coordinate.
 *  @param z The z coordinate.
 */
-(instancetype)initWithX:(double)x y:(double)y z:(double)z;

/**
 *  initializer for ALPSLocalLocation.
 *  @param coordinate The 3D coordinate.
 *  @param xAccuracy The accuracy of the coordinate with respect to the x axis.
 *  @param yAccuracy The accuracy of the coordinate with respect to the y axis.
 *  @param zAccuracy The accuracy of the coordinate with respect to the z axis.
 */
-(instancetype)initWithCoordinate:(ALPSLocationCoordinate3D)coordinate
                        xAccuracy:(float)xAccuracy
                        yAccuracy:(float)yAccuracy
                        zAccuracy:(float)zAccuracy;

@property (nonatomic) ALPSLocationCoordinate3D coordinate;
@property (nonatomic) float xAccuracy;
@property (nonatomic) float yAccuracy;
@property (nonatomic) float zAccuracy;


@end


