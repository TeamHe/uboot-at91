# !/bin/sh

make 
make env

echo copy u-boot.bin to images
cp tools/env/fw_printenv /home/linux/wkdir/sys/buildroot-at91/system/skeleton/usr/bin/
cp tools/env/fw_printenv /home/linux/wkdir/sys/buildroot-at91/output/target/usr/bin/

cp u-boot.bin /home/share/images/
cp u-boot.bin /home/linux/wkdir/images/

#重新生成rootfs
/home/linux/wkdir/sys/buildroot-at91/make-insall.sh

