/*
 * Copyright (C) 2011 Andrea Mazzoleni
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __PORTABLE_H
#define __PORTABLE_H

#if HAVE_CONFIG_H
#include "config.h" /* Use " to include first in the same directory of this file */
#endif

/***************************************************************************/
/* Config */

#ifdef __MINGW32__
/**
 * Enable the GNU printf functions instead of using the MSVCRT ones.
 *
 * Note that this is the default if _POSIX is also defined.
 * To disable it you have to set it to 0.
 */
#define __USE_MINGW_ANSI_STDIO 1

/*
 * Define the MSVCRT version targetting Windows XP.
 */
#define __MSVCRT_VERSION__ 0x0601

/**
 * Include Windows XP headers.
 *
 * Like for CreateHardLinkW().
 */
#define _WIN32_WINNT 0x501

#include <windows.h>
#endif

/**
 * Specify the format attribute for printf.
 */
#ifdef __MINGW32__
#if defined(__USE_MINGW_ANSI_STDIO) && __USE_MINGW_ANSI_STDIO == 1
#define attribute_printf gnu_printf /* GNU format */
#else
#define attribute_printf ms_printf /* MSVCRT format */
#endif
#else
#define attribute_printf printf /* GNU format is the default one */
#endif

/**
 * Compiler extension
 */
#ifndef __always_inline
#define __always_inline inline __attribute__((always_inline))
#endif

/**
 * Architecture.
 */
#if defined(__i386__)
#define CONFIG_X86 1
#define CONFIG_X86_32 1
#endif

#if defined(__x86_64__)
#define CONFIG_X86 1
#define CONFIG_X86_64 1
#endif

/**
 * Includes some platform specific headers.
 */
#if HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

#if HAVE_SYS_MOUNT_H
#include <sys/mount.h>
#endif

#if HAVE_SYS_VFS_H
#include <sys/vfs.h>
#endif

#if HAVE_SYS_STATFS_H
#include <sys/statfs.h>
#endif

#if HAVE_SYS_FILE_H
#include <sys/file.h>
#endif

#if HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif

#if HAVE_LINUX_FS_H
#include <linux/fs.h>
#endif

#if HAVE_LINUX_FIEMAP_H
#include <linux/fiemap.h>
#endif

/**
 * Includes some standard headers.
 */
#include <stdio.h>
#include <stdlib.h> /* On many systems (e.g., Darwin), `stdio.h' is a prerequisite. */
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <limits.h>

#if HAVE_STDINT_H
#include <stdint.h>
#endif

#if HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#if TIME_WITH_SYS_TIME
#include <sys/time.h>
#include <time.h>
#else
#if HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#include <time.h>
#endif
#endif

#if HAVE_MACH_MACH_TIME_H
#include <mach/mach_time.h>
#endif

#if HAVE_DIRENT_H
#include <dirent.h>
#define NAMLEN(dirent) strlen((dirent)->d_name)
#else
#define dirent direct
#define NAMLEN(dirent) (dirent)->d_namlen
#if HAVE_SYS_NDIR_H
#include <sys/ndir.h>
#endif
#if HAVE_SYS_DIR_H
#include <sys/dir.h>
#endif
#if HAVE_NDIR_H
#include <ndir.h>
#endif
#endif

#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#if HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#if HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif
#ifndef WEXITSTATUS
#define WEXITSTATUS(stat_val) ((unsigned)(stat_val) >> 8)
#endif
#ifndef WIFEXITED
#define WIFEXITED(stat_val) (((stat_val) & 255) == 0)
#endif

#if HAVE_GETOPT_H
#include <getopt.h>
#endif

#if HAVE_FNMATCH_H
#include <fnmatch.h>
#else
#include "fnmatch.h"
#endif

#if HAVE_PTHREAD_H
#include <pthread.h>
#endif

#if HAVE_MATH_H
#include <math.h>
#endif

/**
 * Disable case check in Windows.
 */
#ifdef _WIN32
#define FNM_CASEINSENSITIVE_FOR_WIN FNM_CASEFOLD
#else
#define FNM_CASEINSENSITIVE_FOR_WIN 0
#endif

#if HAVE_IO_H
#include <io.h>
#endif

#if HAVE_GETOPT_LONG
#define SWITCH_GETOPT_LONG(a, b) a
#else
#define SWITCH_GETOPT_LONG(a, b) b
#endif

/**
 * Enables lock file support.
 */
#if HAVE_FLOCK && HAVE_FTRUNCATE
#define HAVE_LOCKFILE 1
#endif

/**
 * Includes specific support for Windows or Linux.
 */
#ifdef __MINGW32__
#include "mingw.h"
#else
#include "unix.h"
#endif

/**
 * Include list support to have tommy_node.
 */
#include "tommyds/tommylist.h"

/**
 * Another name for link() to avoid confusion with local variables called "link".
 */
static inline int hardlink(const char* a, const char* b)
{
	return link(a, b);
}

/**
 * Get the device UUID.
 * Return 0 on success.
 */
int devuuid(uint64_t device, char* uuid, size_t size);

/**
 * Physical offset not yet read.
 */
#define FILEPHY_UNREAD_OFFSET 0

/**
 * Special value returned when the filesystem doesn't report any offset for unknown reason.
 */
#define FILEPHY_UNREPORTED_OFFSET 1

/**
 * Special value returned when the file doesn't have a real offset.
 * For example, because it's stored in the NTFS MFT.
 */
#define FILEPHY_WITHOUT_OFFSET 2

/**
 * Value indicating real offsets. All offsets greater or equal at this one are real.
 */
#define FILEPHY_REAL_OFFSET 3

/**
 * Get the physcal address of the specified file.
 * This is expected to be just a hint and not necessarely correct or unique.
 * Return 0 on success.
 */
int filephy(const char* path, uint64_t size, uint64_t* physical);

/**
 * Check if the underline filesystem support persistent inodes.
 * Return -1 on error, 0 on success.
 */
int fsinfo(const char* path, int* has_persistent_inode, uint64_t* total_space, uint64_t* free_space);

/**
 * Get the tick counter value.
 *
 * Note that the frequency is unspecified, because the time measure
 * is meant to be used to compare the ratio between usage times.
 */
uint64_t tick(void);

/**
 * Get the tick counter value in millisecond.
 */
uint64_t tick_ms(void);

/**
 * Initializes the system.
 */
void os_init(int opt);

/**
 * Deintialize the system.
 */
void os_done(void);

/**
 * Log file.
 *
 * This stream if fully buffered.
 *
 * If no log file is selected, it's 0.
 */
FILE* stdlog;

/**
 * Exit codes for testing.
 */
int exit_success;
int exit_failure;
int exit_sync_needed;
#undef EXIT_SUCCESS
#undef EXIT_FAILURE
#define EXIT_SUCCESS exit_success
#define EXIT_FAILURE exit_failure
#define EXIT_SYNC_NEEDED exit_sync_needed

/**
 * Fill memory with psudeo-random values.
 */
int randomize(void* ptr, size_t size);

/**
 * Additional SMART attributes.
 */
#define SMART_ERROR 256 /**< ATA Error count. */
#define SMART_SIZE 257 /**< Size in bytes. */
#define SMART_ROTATION_RATE 258 /**< Rotation speed. 0 for SSD. */
#define SMART_FLAGS 259 /**< Flags returned by smartctl. */

/**
 * SMART attributes count.
 */
#define SMART_COUNT 260

/**
 * Flags returned by smartctl.
 */
#define SMARTCTL_FLAG_UNSUPPORTED (1 << 0) /**< Device not recognized, requiring the -d option. */
#define SMARTCTL_FLAG_OPEN (1 << 1) /**< Device open or identification failed. */
#define SMARTCTL_FLAG_COMMAND (1 << 2) /**< Some SMART or ATA commands failed. This is a common error, also happening with full info gathering. */
#define SMARTCTL_FLAG_FAIL (1 << 3) /**< SMART status check returned "DISK FAILING". */
#define SMARTCTL_FLAG_PREFAIL (1 << 4) /**< We found prefail Attributes <= threshold. */
#define SMARTCTL_FLAG_PREFAIL_LOGGED (1 << 5) /**< SMART status check returned "DISK OK" but we found that some (usage or prefail) Attributes have been <= threshold at some time in the past. */
#define SMARTCTL_FLAG_ERROR (1 << 6) /**< The device error log contains records of errors. */
#define SMARTCTL_FLAG_ERROR_LOGGED (1 << 7) /**< The device self-test log contains records of errors. */

/**
 * SMART max attribute length.
 */
#define SMART_MAX 64

/**
 * Value for unassigned SMART attribute.
 */
#define SMART_UNASSIGNED 0xFFFFFFFFFFFFFFFFULL

/**
 * Device info entry.
 */
struct devinfo_struct {
	uint64_t device; /**< Device ID. */
	char name[PATH_MAX]; /**< Name of the disk. */
	char mount[PATH_MAX]; /**< Mount point or other contained directory. */
	char smartctl[PATH_MAX]; /**< Options for smartctl. */
	char file[PATH_MAX]; /**< File device. */
#ifdef _WIN32
	char wfile[PATH_MAX]; /**< File device in Windows format. Like \\.\PhysicalDriveX, or \\?\Volume{X}. */
#endif
	struct devinfo_struct* parent; /**< Pointer at the parent if any. */
	uint64_t smart[SMART_COUNT]; /**< SMART raw attributes. */
	char smart_serial[SMART_MAX]; /**< SMART serial number. */
#if HAVE_PTHREAD_CREATE
	pthread_t thread;
#endif
	tommy_node node;
};
typedef struct devinfo_struct devinfo_t;

#define DEVICE_LIST 0
#define DEVICE_DOWN 1
#define DEVICE_UP 2
#define DEVICE_SMART 3

/**
 * Query all the "high" level devices with the specified operation,
 * and produces a list of "low" level devices to operate on.
 *
 * The passed "low" device list must be already initialized.
 */
int devquery(tommy_list* high, tommy_list* low, int operation);

#endif

