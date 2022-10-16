#!/bin/bash
dirname=`dirname $0`
BUILDPATH="$dirname/BUILD"
if [ ! -d $BUILDPATH ] ; then
  mkdir -p $BUILDPATH
fi
CACHEPATH="$dirname/CACHE"
if [ ! -d $CACHEPATH ] ; then
  mkdir -p $CACHEPATH
fi

ide=`which arduino`
realide=`readlink -f $ide`
ARDUINOINSTALLDIR=`dirname $realide`
python3 $HOME/.arduino15/packages/esp32/tools/esptool_py/3.3.0/esptool.py \
    --chip esp32s3 --port /dev/ttyACM0 --baud 115200 --before default_reset \
    --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m \
    --flash_size 4MB 0x0 $BUILDPATH/NewBikeComputer.ino.bootloader.bin 0x8000 \
    $BUILDPATH/NewBikeComputer.ino.partitions.bin 0xe000 \
    $HOME/.arduino15/packages/esp32/hardware/esp32/2.0.4/tools/partitions/boot_app0.bin 0x10000 \
    $BUILDPATH/NewBikeComputer.ino.bin 0x2d0000 \
    $HOME/.arduino15/packages/esp32/hardware/esp32/2.0.4/variants/adafruit_feather_esp32s3_tft/tinyuf2.bin 
