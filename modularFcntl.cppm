module;

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

export module modularFcntl;

import std;

export namespace Sys::fcntl {

    using FileDescriptor = int;

    // Access and Behavior Flags
    enum class OpenFlags : int {
        RdOnly    = O_RDONLY,
        WrOnly    = O_WRONLY,
        RdWr      = O_RDWR,
        Create    = O_CREAT,
        Truncate  = O_TRUNC,
        Append    = O_APPEND,
        NonBlock  = O_NONBLOCK,
        Direct    = O_DIRECT,
        Sync      = O_SYNC
    };

    // Explicit Permission Masks
    enum class FileMode : mode_t {
        UserRead    = S_IRUSR,
        UserWrite   = S_IWUSR,
        UserExec    = S_IXUSR,
        GroupRead   = S_IRGRP,
        AllRead     = S_IRUSR | S_IRGRP | S_IROTH
    };

    // Cooperative Locking Layouts
    enum class LockType : short {
        ReadLock  = F_RDLCK,
        WriteLock = F_WRLCK,
        Unlock    = F_UNLCK
    };

    enum class LockWhence : short {
        Set = SEEK_SET,
        Cur = SEEK_CUR,
        End = SEEK_END
    };

    struct FileLockSpec {
        LockType   type;
        LockWhence whence;
        off_t      start;
        off_t      length;
        pid_t      pid{0};
    };

    // Bitwise operators for Flags
    constexpr OpenFlags operator|(OpenFlags lhs, OpenFlags rhs) noexcept {
        return static_cast<OpenFlags>(static_cast<int>(lhs) | static_cast<int>(rhs));
    }

    // New: Bitwise operator for Modes so they can be combined
    constexpr FileMode operator|(FileMode lhs, FileMode rhs) noexcept {
        return static_cast<FileMode>(static_cast<mode_t>(lhs) | static_cast<mode_t>(rhs));
    }
    
	constexpr OpenFlags operator&(OpenFlags lhs, OpenFlags rhs) noexcept {
    	return static_cast<OpenFlags>(static_cast<int>(lhs) & static_cast<int>(rhs));
	}
	
	constexpr OpenFlags operator~(OpenFlags fh) noexcept {
    	return static_cast<OpenFlags>(~static_cast<int>(fh));
	}

    // --- Inline Core System APIs ---

    [[nodiscard]] inline FileDescriptor open(const char* pathname, OpenFlags flags) noexcept {
        return ::open(pathname, static_cast<int>(flags));
    }

    [[nodiscard]] inline FileDescriptor open_create(const char* pathname, OpenFlags flags, FileMode mode) noexcept {
        return ::open(pathname, static_cast<int>(flags | OpenFlags::Create), static_cast<mode_t>(mode));
    }

    inline int close(FileDescriptor fd) noexcept {
        return ::close(fd);
    }

    // New: Wrapped unlink to keep main.cpp entirely header-free
    inline int unlink(const char* pathname) noexcept {
        return ::unlink(pathname);
    }

    inline int try_apply_lock(FileDescriptor fd, const FileLockSpec& spec) noexcept {
        struct ::flock native_lock {
            .l_type   = static_cast<short>(spec.type),
            .l_whence = static_cast<short>(spec.whence),
            .l_start  = spec.start,
            .l_len    = spec.length,
            .l_pid    = spec.pid
        };
        return ::fcntl(fd, F_SETLK, &native_lock);
    }
}
