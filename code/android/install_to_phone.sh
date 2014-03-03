#!/bin/bash

#uninstall the old version
adb uninstall mox.lamp.myfirstapp &&

# build the app
ant debug &&


#install the new version
adb install bin/MyFirstApp-debug.apk
