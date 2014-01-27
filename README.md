iOS-Framework
=============

This is the official Senic iOS framework.
There are two different ways to connect to Senic BLE devices.

1. option: If you're using Senic BLE devices as well as other BLE devices
If you intend to integrate Senic BLE devices as well as other BLE devices into your app, 
you need to create your own <code>CBCentralManager</code> object 
and set a delegate object which can handle all events.

2. option: If you're only integrating Senic BLE devices
If you're exclusively integrating Senic BLE devices into your app, 
you can simply use our Senic <code>SCBluetooth</code> implementation,
which automatically creates a <code>CBCentralManager</code> object 
and sets itself as a delegate so that it can handle all events.
