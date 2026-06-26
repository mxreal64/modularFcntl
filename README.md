# modularFcntl

A blazing-fast, zero-overhead C++26 named module gateway for raw Linux file control operations. It performs a brutal macro-exorcism to purge toxic, legacy POSIX preprocessor pollution from your global namespace while delivering direct, type-safe kernel performance.

## Why It Matters

Traditional `<fcntl.h>` usage floods your codebase with unsafe global macros (`O_NONBLOCK`, `O_CREAT`, etc.) and forces error-prone integer flag masking. 

`modularFcntl` firewalls the legacy C headers inside the global module fragment and exposes strongly typed, modern C++ structures that compile down to the exact same assembly as raw system calls.

## Features

* **Zero Memory Overhead:** Pure `inline` API with zero heap allocation or runtime abstraction penalties.
* **Strong Type Safety:** Replaces loose integer flags with strict, bitmask-supported `enum class` types for file modes, open flags, and flock parameters.
* **Cooperative File Locking:** Modern C++ structural wrapper over raw kernel `flock` configurations.
* **Compile-Time Isolation:** Fully isolates POSIX macros so they can never bleed into your downstream user code.

## Quick Example

```cpp
import modularFcntl;
import std;

int main() {
    using namespace Sys::fcntl;

    // Open a file safely with strongly-typed bitmasks
    FileDescriptor fd = open_create(
        "log.txt", 
        OpenFlags::WrOnly | OpenFlags::Append | OpenFlags::NonBlock, 
        FileMode::UserRead | FileMode::UserWrite
    );

    if (fd < 0) {
        std::println(stderr, "Failed to open descriptor.");
        return 1;
    }

    // Try applying a cooperative lock
    FileLockSpec lock{
        .type = LockType::WriteLock,
        .whence = LockWhence::Set,
        .start = 0,
        .length = 0 // Lock whole file
    };

    if (try_apply_lock(fd, lock) == 0) {
        std::println("File locked successfully.");
    }

    close(fd);
    return 0;
}
```

## License

copyright mxreal64, 2026

This project is licensed under the GNU General Public License v3.0 - see the LICENSE file for details.
