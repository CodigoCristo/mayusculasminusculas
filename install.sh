#!/bin/bash
meson setup build 
meson setup build --reconfigure
meson compile -C build
meson install -C build
org.gtk.MayusApp

#./build/src/example
