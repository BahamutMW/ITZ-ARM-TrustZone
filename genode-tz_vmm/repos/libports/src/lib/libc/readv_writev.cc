/*
 * \brief  'readv()' and 'writev()' implementations
 * \author Josef Soentgen
 * \author Christian Prochaska
 * \date   2012-04-10
 */

/*
 * Copyright (C) 2012-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/lock.h>

/* libc includes */
#include <sys/uio.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>


static Genode::Lock rw_lock;


struct Read
{
     ssize_t operator()(int fd, void *buf, size_t count)
     {
    	 return read(fd, buf, count);
     }
};


struct Write
{
     ssize_t operator()(int fd, const void *buf, size_t count)
     {
    	 return write(fd, buf, count);
     }
};


template <typename Rw_func>
static ssize_t readv_writev_impl(Rw_func rw_func, int fd, const struct iovec *iov, int iovcnt)
{
	Genode::Lock_guard<Genode::Lock> rw_lock_guard(rw_lock);

	char *v;
	ssize_t bytes_transfered_total = 0;
	size_t v_len = 0;
	int i;

	if (iovcnt < 1 || iovcnt > IOV_MAX) {
		errno = EINVAL;
		return -1;
	}

	for (i = 0; i < iovcnt; i++)
		v_len += iov->iov_len;

	if (v_len > SSIZE_MAX) {
		errno = EINVAL;
		return -1;
	}

	while (iovcnt > 0) {
		v = static_cast<char *>(iov->iov_base);
		v_len = iov->iov_len;

		while (v_len > 0) {
			ssize_t bytes_transfered = rw_func(fd, v, v_len);

			if (bytes_transfered == -1)
				return -1;

			if (bytes_transfered == 0)
				return bytes_transfered_total;

			v_len -= bytes_transfered;
			v += bytes_transfered;
			bytes_transfered_total += bytes_transfered;
		}

		iov++;
		iovcnt--;
	}

	return bytes_transfered_total;
}


extern "C" ssize_t _readv(int fd, const struct iovec *iov, int iovcnt)
{
	return readv_writev_impl(Read(), fd, iov, iovcnt);
}


extern "C" ssize_t readv(int fd, const struct iovec *iov, int iovcnt)
{
	return _readv(fd, iov, iovcnt);
}


extern "C" ssize_t _writev(int fd, const struct iovec *iov, int iovcnt)
{
	return readv_writev_impl(Write(), fd, iov, iovcnt);
}


extern "C" ssize_t writev(int fd, const struct iovec *iov, int iovcnt)
{
	return _writev(fd, iov, iovcnt);
}
