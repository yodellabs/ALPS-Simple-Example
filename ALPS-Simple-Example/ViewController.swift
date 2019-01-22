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
 *
 *******************************************************************************/

/**
 * This simple example shows how to inialize and run ALPS in conjunction with tracking from ARKit.
 * The location calculated by ALPS will be printed to the console.
 */
 

import UIKit
import ALPS
import ARKit

class ViewController: UIViewController {
    
    /* Create an ALPS location manager object */
    var locationManager: ALPSLocationManager?
    
    /* Variable to store tracking state of ARKit */
    var cameraTrackingState: ARCamera.TrackingState = ARCamera.TrackingState.notAvailable
    
    /* Standard ARKit initialization */
    let sceneView = ARSCNView()
    let scene = SCNScene()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        /* Replace the organization identifer here */
        self.locationManager = ALPSLocationManager(remoteRealmIdentifier: "/a813dda10478e48d283b237775d5ae55/Leeroy", useVIO: true, solveIn3D: false)
        
        /* Sets the delegate of the location manager to receive location data */
        self.locationManager?.delegate = self
        
        sceneView.scene = scene
        
        /* Set ViewController as the SceneView delegate */
        sceneView.session.delegate = self
        
        /* Create a world tracking configuration */
        let configuration = ARWorldTrackingConfiguration()
        
        /* Make y the vertical direction by aligning to gravity */
        configuration.worldAlignment = .gravity
        
        /* Run the view's session*/
        sceneView.session.run(configuration)
        
        /* Tells location manager to start sending location data to its delegate */
        locationManager?.startUpdatingLocation()
        
        
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        /* Dispose of any resources that can be recreated. */
    }
    
    
}

/* ViewController conforms to the ALPSParticleLocationManagerDelegate protocol */
extension ViewController: ALPSLocationManagerDelegate {
    
    /* Delegate method for receiving location only */
    func locationManager(_ manager: ALPSLocationManager!, didUpdate location: ALPSLocation3D, andPoseTransform poseTransform: ALPSPoseTransform, fromBeacons sortedToFArray: [ALPSRangeDataPoint]!, timeStamp: Double) {
        /* Outputs location to the console */
        NSLog("x: %f, y: %f, z: %f", location.x, location.y, location.z)
    }
    
}


extension ViewController: ARSessionDelegate {
    func session(_ session: ARSession, didUpdate frame: ARFrame) {
        
        let currentTransform = frame.camera.transform
        /* Grab x, y, and z from the ar camera transform */
        let x = currentTransform[3][0]
        let y = currentTransform[3][1]
        let z = currentTransform[3][2]
        let yAngle = frame.camera.eulerAngles.y
        
        switch (cameraTrackingState) {
        case .normal:
            if let locationManager = self.locationManager {
                /* Send VIO data to the location manager to fuse with ALPS data */
                locationManager.fuseVioCoordinateX(Double(x), y: Double(y), z: Double(z), yEulerAngle: Double(yAngle), timeStamp: frame.timestamp)
                locationManager.trackingState = ALPSTrackingStateNormal
            }
             /* If ARKit tracking is limited or not available, don't send any data to the ALPS location manger and let it know about the tracking state so that it switches over to ultrasound only localization  */
        case .limited(let reason):
            switch(reason){
            case .initializing:
                locationManager?.trackingState = ALPSTrackingStateInitializing
            case .insufficientFeatures:
                locationManager?.trackingState = ALPSTrackingStateInsufficientFeatures
            case .excessiveMotion:
                locationManager?.trackingState = ALPSTrackingStateExcessiveMotion
            case .relocalizing:
                locationManager?.trackingState = ALPSTrackingStateRelocalizing
            }
        case .notAvailable:
            locationManager?.trackingState = ALPSTrackingStateNotAvailable
        }
        
    }
    
    
    
    func session(_ session: ARSession, cameraDidChangeTrackingState camera: ARCamera) {
        /* Stores camera tracking state so we know when to send VIO data to the ALPS framework */
        cameraTrackingState = camera.trackingState
    }
    
}
