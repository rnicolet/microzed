#!/bin/bash
cd zynqPL/
./create_vivado_project.sh
cd ../zynqLNX

time petalinux-build

