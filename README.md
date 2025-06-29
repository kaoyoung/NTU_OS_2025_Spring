# NTU_OS_2025_Spring

## MP0: listing all directories and files

### File System Architecture in xv6
1. Disk layer 
- Sectors:  The smallest unit for disk I/O.
- Blocks: The file system groups multiple sectors into a block
- Device Driver: Responsible for actual read/write operations with the virtual disk (usually an IDE disk emulated by QEMU).
2. File System Structure on Disk
- boot block: bootstrap code to start the operating system.
- Inode Bitmap: Used to track which inodes are allocated and which are free.
- Inode Blocks: Store all inodes. Each inode contains metadata for a file or directory
- Data Blocks: Store the actual file data or directory contents. 
...
3. In-Memory Kernel Data Structures
- inode (In-memory inode): This is the in-memory representation of an on-disk inode. When a file or directory is accessed, its corresponding on-disk inode is read into memory, becoming a struct inode. It contains all the information from the disk inode, plus some additional memory management information (like a reference count).

    - Role: Every open file or directory will have an associated in-memory inode. fstat() retrieves information from this in-memory inode to populate the stat structure.

- file (File structure): This is the kernel's abstraction for an open instance of a file. A file structure contains:

    1. A pointer to its associated inode.
    2. The file's read/write mode (read-only, write-only, read-write).
    3. The file's current offset, indicating where the next read or write operation will start.

    - Role: The open() system call allocates a file structure and binds it to the appropriate inode. read() and write() operations use the offset within the file structure.

- proc (Process table): Each process (proc) has a file descriptor table (an ofile array), which is an array of struct file pointers.

    - Role: An fd (file descriptor) is essentially an index into this array. When you call open(), the kernel finds a free slot in the proc's ofile array, stores the pointer to the newly allocated file structure there, and returns the index of that slot (the fd) to the user process.
4. System Call Layer
- open(): Used to open a file or directory. It performs path lookup, allocates inode and file structures, and returns an fd.
- fstat(): Used to retrieve metadata for a file or directory based on its fd (reading from the in-memory inode), and copies it to the user-provided stat structure.
- read(): Used to read data from a file or directory. For files, it reads from data blocks. For directories, it reads struct dirent entries from the directory's data blocks.
- close(): Releases the fd and the associated file structure. When the file structure's reference count drops to zero, the reference count of its associated inode is also decremented.

### fd, stat, and dirent
1. `fd(File Descriptor)`: Your Handle to the OS
It's a small, non-negative integer that the operating system uses to identify an open file, pipe, socket, or other I/O resource.
    - read(fd, buffer, count): To read data from the resource.

    - write(fd, buffer, count): To write data to the resource.

    - close(fd): To release the resource when you're done.

    - fstat(fd, &st): To get status information about the resource.

    - dup(fd) or dup2(old_fd, new_fd): To duplicate or redirect fds.

2. `struct stat (File Status)`: All About the File's Metadata
It's a data structure that holds detailed metadata about a file or directory. 
    - stat(const char *path, struct stat *buf): Gets information about the file specified by its path.

    - fstat(int fd, struct stat *buf): Gets information about the file associated with a given file descriptor (fd).

    - lstat(const char *path, struct stat *buf): Similar to stat, but if path is a symbolic link, it gets information about the link itself, not the file it points to.

3. `struct dirent (Directory Entry)`: What's Inside a Directory
It  represents a single entry within a directory. When you read the contents of a directory, you're essentially getting a list of these dirent structures, one for each file or subdirectory inside.

- d_name: The name of the file or subdirectory. This is the most crucial piece of information it provides. It's a character array (e.g., char d_name[256];).

- d_ino: The inode number of the file or subdirectory. (This is especially important in xv6).

- d_type: (On most modern systems, like Linux) An unsigned char indicating the file type (e.g., DT_REG for regular file, DT_DIR for directory, DT_LNK for symbolic link). This can sometimes save you a stat() call if you only need the type.


#### How they work together (typical way)
1. Open the directory: You use opendir(const char *name) to get a DIR* pointer (which internally holds an fd for the directory).

2.Read directory entries: You enter a loop, calling readdir(DIR *dirp) repeatedly. Each call returns a struct dirent * (a pointer to a dirent structure) for the next entry in the directory. You'll primarily extract the d_name from this dirent.

3.Get file/entry status: For each d_name you get, if you need more details about that specific file or subdirectory (like its size, permissions, or to confirm its exact type beyond d_type), you construct its full path (e.g., "parent_dir/file_name") and then call stat(full_path, &st) to populate a struct stat variable.

4.Process information: You then use the information in the struct stat (e.g., st.st_mode to check if it's a directory for recursive listing, or st.st_size to display its size) to perform further actions.

5.Close the directory: Once done, you call closedir(DIR *dirp) to release the resources associated with the opened directory.

### `return 0;` vs `exit(0);` in xv6

#### The Problem
When using `return 0;` in `main()` on **xv6\user\mp0.c**, you may encounter the following error:

`
usertrap(): unexpected scause 0x000000000000000f pid=5
sepc=0x00000000000007dc stval=0x0000000000001ff8`


This happens because xv6 has a **minimal runtime environment**, and `return 0;` relies on runtime behavior that isn't fully supported.

#### Why This Happens

When you use `return 0;`:
- The program attempts to return control to a **missing or broken runtime handler**
- This can result in:
  - A corrupted or invalid return address
  - A messed-up stack pointer
  - Jumps to **unmapped memory regions**

#### The Fix: Use `exit(0);`

`exit(0);` is a **system call** that directly tells the kernel to terminate the process. It avoids all the pitfalls of relying on user-space runtime code.

**Benefits of `exit(0);`:**
- Direct system call to the kernel
- Proper process cleanup and termination
- No dependency on user-space stack or return handling
- Avoids undefined behavior or crashes

#### Summary

| Action      | Result in xv6                   |
|-------------|----------------------------------|
| `return 0;` | May crash due to missing runtime |
| `exit(0);`  | Properly exits the process       |