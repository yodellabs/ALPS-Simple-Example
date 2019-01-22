<img src="https://github.com/yodellabs/ALPS-Framework-iOS/blob/master/docs/logo.png" width="335">

# ALPS-Simple-Example

This example shows how to use the Acoustic Location Processing System (ALPS) framework to provide indoor localization services to iOS devices. ALPS uses time-of-flight ranging from ultrasound beacons at known locations to determine the location of the mobile device. This example further makes use of Visual Inertial Odometry (VIO) data from Apple's ARkit to provide relative tracking. The location of the device is output to the console in cartesian coordinates (x,y,z). The framework uses a Realm cloud database to access beacon configuration data, for which you will need to supply a Remote Realm Identifier.

A repository with the ALPS framework and documentation can be found here: https://github.com/yodellabs/ALPS-Framework-iOS
