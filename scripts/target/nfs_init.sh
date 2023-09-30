#!/bin/sh
# run this scripts on raspberry pi to initialize nfs client

NFS_SERVER_IP="192.168.20.101"

mkdir -p /mnt/nfs
mount -t nfs -o nolock ${NFS_SERVER_IP}:/mnt/nfs /mnt/nfs

echo "Mounted NFS server directory on /mnt/nfs..!"
