
# APFS FUSE Driver for FreeBSD

This project is a read-only FUSE driver for the Apple File System. Encrypted volumes and fusion drives have not been tested. Firmlinks are not supported yet.

Be aware that not all compression methods are supported yet (only the ones I have encountered so far).
Thus, the driver may return compressed files instead of uncompressed ones. Although most of the time it
should just report an error.

## Installation (as root)

```
sudo su -
pkg install fusefs-libs3 bzip2 cmake gcc git
kldload fusefs
git clone https://github.com/iairu/apfs-fuse-freebsd.git
cd apfs-fuse-freebsd
git submodule init
git submodule update
mkdir build
cd build
cmake ..
make
make install # will install apfs-fuse and apfsutil binaries
```

## Regular usage

```
mkdir -p /mnt/apfs
gpart show # find your apfs partition to mount (for me ada0 and partition no. 2)
sudo /usr/local/bin/apfs-fuse /dev/ada0p2 /mnt/apfs/
```

Will mount read only, tested by doing firefox profile copy from APFS to UFS using cp -r as fully working.

## Optional GDB script used by gemini cli during debug

```
set args /dev/ada0p2 /mnt/apfs/ # my parition is no.2 on disk ada0
run
backtrace
quit
```

ran using: `gdb -x gdb_script apfs-fuse-freebsd/build/apfs-fuse`
