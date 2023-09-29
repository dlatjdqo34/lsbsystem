#!/bin/sh

mkdir -p /root/kdt
cp -r /mnt/nfs/* /root/kdt

echo "Copied from NFS directory to /root/kdt"
