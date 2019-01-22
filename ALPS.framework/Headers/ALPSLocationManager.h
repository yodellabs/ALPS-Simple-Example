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
 * The location manager is the main class used to interface with ALPS. It is
 * responsible for starting, stopping and receiving location updates.
 */

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "ALPSRangeDataPoint.h"

typedef struct {
    float x;
    float y;
    float z;
} ALPSLocation3D;

typedef enum ALPSTrackingState{
    ALPSTrackingStateDisabled,
    ALPSTrackingStateNotAvailable,
    ALPSTrackingStateNormal,
    ALPSTrackingStateInitializing,
    ALPSTrackingStateRelocalizing,
    ALPSTrackingStateExcessiveMotion,
    ALPSTrackingStateInsufficientFeatures
}ALPSTrackingState;

ALPSLocation3D ALPSLocation3DMake(float x, float y, float z);

typedef struct {
    float x;
    float xStdDev;
    float y;
    float yStdDev;
    float z;
    float zStdDev;
    float rotation;
    float rotationStdDev;
} ALPSPoseTransform;

ALPSPoseTransform ALPSPoseTransformMake(float x, float xStdDev, float y, float yStdDev, float z, float zStdDev, float rotation, float rotationStdDev);

@class ALPSLocationManager, ALPSAudioBeaconFlightData;

@protocol ALPSLocationManagerDelegate <NSObject>

/**
 *  Delegate method that should be used for AR applications. Supplies the current location in the ALPS coordinate system and the transform from ARKit's reference frame to ALPS's reference frame.
 *  @param manager Pointer to the location manager that sent a new location.
 *  @param location The location in the ALPS reference frame with accuracies in meters.
 *  @param poseTransform The pose transform to translate and rotate the ARKit reference from to the ALPS reference frame.
 *  @param rangeDataPoints NSArray containing ALPSRangeDataPoints from received ultrasound transmissions. These each contain a beacon MAC, time slot and range from the phone to the beacon.
 *  @param timeStamp Unix time of when the data was received.
 */
-(void)locationManager:(ALPSLocationManager *)manager
     didUpdateLocation:(ALPSLocation3D)location
      andPoseTransform:(ALPSPoseTransform)poseTransform
           fromBeacons:(NSArray<ALPSRangeDataPoint*>*)rangeDataPoints
             timeStamp:(double)timeStamp;


@optional
/**
 *  Delegate method for when the location manager has started running.
 *  @param manager Pointer to the location manager that started.
 */
-(void)locationManagerDidStartUpdatingLocation:(ALPSLocationManager *)manager;

/**
 *  Delegate method for when the location manager has stopped running.
 *  @param manager Pointer to the location manager that started.
 */
-(void)locationManagerDidStopUpdatingLocation:(ALPSLocationManager *)manager;

/**
 *  Delegate method for providing the floor plan image (if any) of where the iOS device is currently located. This image is stored in the realm database.
 *  @param floorPlanImage Image of the floor plan as an UIImage.
 *  @param scale Scale of the floor plan image in meters/pixel.
 *  @param manager Pointer to the location manager that called back.
 */
-(void)locationManager:(ALPSLocationManager *)manager
  didChangeToGridNamed:(NSString *)gridName
    withFloorPlanImage:(UIImage *)floorPlanImage
        pixelScale:(double)scale;


@end

@interface ALPSLocationManager : NSObject


/**
 *  This initializer must be called for the location manager to connect to the Realm database for retreiving beacon configuration and floor plan data.
 *  @param remoteRealmIdentifier Remote Realm Identifer in the form /userid/realmid
 *  @param solveIn3D whether to solve in 3D or 2D.  parameter only matters if useRelativeLocation is true.
 *  This initializer requires that fuseRelative location is called when new VIO data is available
 */
-(id)initWithRemoteRealmIdentifier:(NSString *)remoteRealmIdentifier solveIn3D:(BOOL)solveIn3D;


/**
 *  This initializer must be called for the location manager to connect to the Realm database for retreiving beacon configuration and floor plan data.
 *  @param remoteRealmIdentifier Remote Realm Identifer in the form /userid/realmid
 *  @param useVIO whether or not fuseRelativeLocation will be called
 *  @param solveIn3D whether to solve in 3D or 2D.  parameter only matters if useRelativeLocation is true.
 */
-(id)initWithRemoteRealmIdentifier:(NSString *)remoteRealmIdentifier useVIO:(BOOL)useVIO solveIn3D:(BOOL)solveIn3D;


/**
 *  Method to start receiving location updates from ALPS. Locations will sent through the <didUpdateLocation> delegate method.
 *  It should be called in viewDidAppear or equivalent
 */
-(void)startUpdatingLocation;

/**
 *  Method to stop location updates.
 *  Call this when app enters the background
 */
-(void)stopUpdatingLocation;

/**
 *  Call this method only after calling stopUpdatingLocation
 *
 */
-(void)resumeUpdatingLocation;

/**
 *  This method must be called with the current location coordinates and timestamp provided by ARKit. It should be called from the didUpdate frame:ARFrame delegate method from ARKit.
 *  @param x X coordinate of the current ARFrame provided by ARKit.
 *  @param y Y coordinate of the current ARFrame provided by ARKit.
 *  @param z Z coordinate of the current ARFrame provided by ARKit.
  * @param yEulerAngle y Euler angle of the current ARFrame provided by ARKit.
 *  @param timeStamp Time stamp of the current ARFrame provided by ARKit.
 */
-(void)fuseVioCoordinateX:(double)x y:(double)y z:(double)z yEulerAngle:(double)yEulerAngle timeStamp:(double)timeStamp;

/**
 *  Streams location information to UDP host and port in json format
 *  @param hostName the host name to stream to
 *  @param port the port to stream to
 */
-(void)sendLocationUpdatesToServerWithHostHame:(NSString *)hostName port:(uint16_t)port;

/**
 *  Stops the location stream
 *
 */
-(void)stopSendingLocationUdpatesToServer;


@property (nonatomic, weak) id <ALPSLocationManagerDelegate> delegate;

@property (nonatomic) BOOL isStarted;

@property(nonatomic, readonly) NSString *remoteRealmIdentifier;

@property (nonatomic, readonly) BOOL useVIO;

//@property (nonatomic, readonly) NSArray<ALPSBeaconInfo*> *allBeacons;

@property (nonatomic) BOOL generateParticleImage;

@property (nonatomic) ALPSTrackingState trackingState;

@end
