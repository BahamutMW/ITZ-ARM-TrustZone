/*
 * \brief  File node
 * \author Norman Feske
 * \author Christian Helmuth
 * \date   2013-11-11
 */

#ifndef _FILE_H_
#define _FILE_H_

/* local includes */
#include <node.h>
#include <lx_util.h>


namespace File_system {
	class File;
}


class File_system::File : public Node
{
	private:

		int _fd;

		unsigned long _inode(int dir, char const *name, bool create)
		{
			int ret;

			if (create) {
				mode_t ugo = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
				ret = mknodat(dir, name, S_IFREG | ugo, 0);
				if (ret == -1 && errno != EEXIST)
					throw No_space();
			}

			struct stat s;

			ret = fstatat(dir, name, &s, 0);
			if (ret == -1)
				throw Lookup_failed();

			return s.st_ino;
		}

		unsigned long _inode_path(char const *path)
		{
			int         ret;
			struct stat s;

			ret = stat(path, &s);
			if (ret == -1)
				throw Lookup_failed();

			return s.st_ino;
		}

		int _open(int dir, char const *name, Mode mode)
		{
			int fd = openat(dir, name, access_mode(mode));
			if (fd == -1)
				throw Lookup_failed();

			return fd;
		}

		int _open_path(char const *path, Mode mode)
		{
			int fd = open(path, access_mode(mode));
			if (fd == -1)
				throw Lookup_failed();

			return fd;
		}

	public:

		File(int         dir,
		     char const *name,
		     Mode        mode,
		     bool        create)
		:
			Node(_inode(dir, name, create)),
			_fd(_open(dir, name, mode))
		{
			Node::name(name);
		}

		File(char const *path, Mode mode)
		:
			Node(_inode_path(path)),
			_fd(_open_path(path, mode))
		{
			Node::name(basename(path));
		}

		size_t read(char *dst, size_t len, seek_off_t seek_offset)
		{
			int ret = pread(_fd, dst, len, seek_offset);

			return ret == -1 ? 0 : ret;
		}

		size_t write(char const *src, size_t len, seek_off_t seek_offset)
		{
			/* should we append? */
			if (seek_offset == ~0ULL) {
				::off_t off = lseek(_fd, 0, SEEK_END);
				if (off == -1)
					return 0;
				seek_offset = off;
			}

			int ret = pwrite(_fd, src, len, seek_offset);

			return ret == -1 ? 0 : ret;
		}

		file_size_t length() const
		{
			struct stat s;

			if (fstat(_fd, &s) < 0)
				return 0;

			return s.st_size;
		}

		void truncate(file_size_t size)
		{
			if (ftruncate(_fd, size)) /* nothing */;

			mark_as_updated();
		}
};

#endif /* _FILE_H_ */
