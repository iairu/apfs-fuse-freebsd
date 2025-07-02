/*
This file is part of apfs-fuse, a read-only implementation of APFS
(Apple File System) for FUSE.
Copyright (C) 2017 Simon Gander

Apfs-fuse is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Apfs-fuse is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with apfs-fuse.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef __FreeBSD__

#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/disk.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include "DeviceFreeBSD.h"
#include "Global.h"

DeviceFreeBSD::DeviceFreeBSD()
{
	m_device = -1;
	m_size = 0;
}

DeviceFreeBSD::~DeviceFreeBSD()
{
	Close();
}

bool DeviceFreeBSD::Open(const char* name)
{
	m_device = open(name, O_RDONLY | O_DIRECT);

	if (m_device == -1)
	{
		std::cout << "Opening device " << name << " failed with error " << strerror(errno) << std::endl;
		return false;
	}

	struct stat st;

	fstat(m_device, &st);

	if (S_ISREG(st.st_mode))
	{
		m_size = st.st_size;
	}
	else if (S_ISCHR(st.st_mode))
	{
		ioctl(m_device, DIOCGMEDIASIZE, &m_size);
	}

	if (g_debug & Dbg_Info)
		std::cout << "Device " << name << " opened. Size is " << m_size << std::endl;

	return m_device != -1;
}

void DeviceFreeBSD::Close()
{
	if (m_device != -1)
		close(m_device);
	m_device = -1;
	m_size = 0;
}

bool DeviceFreeBSD::Read(void* data, uint64_t offs, uint64_t len)
{
	size_t nread;

	nread = pread(m_device, data, len, offs);

	// TODO: Better error handling ...
	return nread == len;
}

#endif