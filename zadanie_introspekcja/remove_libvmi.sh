#!/bin/bash
pwd = $(pwd)
cd /usr/local/lib
sudo rm -f libvmi.*
sudo rm -rf pkgconfig

cd /usr/local/include
sudo rm -rf libvmi

cd $pwd