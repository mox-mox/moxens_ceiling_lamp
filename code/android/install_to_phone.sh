#!/bin/bash

# build the app
ant debug

#uninstall the old version
adb uninstall mox.lamp.myfirstapp

#install the new version
adb install bin/MyFirstApp-debug.apk
