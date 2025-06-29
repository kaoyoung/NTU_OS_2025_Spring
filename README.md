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

Role: Every open file or directory will have an associated in-memory inode. fstat() retrieves information from this in-memory inode to populate the stat structure.

- file (File structure): This is the kernel's abstraction for an open instance of a file. A file structure contains:

A pointer to its associated inode.

The file's read/write mode (read-only, write-only, read-write).

The file's current offset, indicating where the next read or write operation will start.

Role: The open() system call allocates a file structure and binds it to the appropriate inode. read() and write() operations use the offset within the file structure.

- proc (Process table): Each process (proc) has a file descriptor table (an ofile array), which is an array of struct file pointers.

Role: An fd (file descriptor) is essentially an index into this array. When you call open(), the kernel finds a free slot in the proc's ofile array, stores the pointer to the newly allocated file structure there, and returns the index of that slot (the fd) to the user process.
4. System Call Layer
- open(): Used to open a file or directory. It performs path lookup, allocates inode and file structures, and returns an fd.
- fstat(): Used to retrieve metadata for a file or directory based on its fd (reading from the in-memory inode), and copies it to the user-provided stat structure.
- read(): Used to read data from a file or directory. For files, it reads from data blocks. For directories, it reads struct dirent entries from the directory's data blocks.
- close(): Releases the fd and the associated file structure. When the file structure's reference count drops to zero, the reference count of its associated inode is also decremented.

### fd, stat, and dirent
1. fd(File Descriptor): Your Handle to the OS
It's a small, non-negative integer that the operating system uses to identify an open file, pipe, socket, or other I/O resource.
- read(fd, buffer, count): To read data from the resource.

- write(fd, buffer, count): To write data to the resource.

- close(fd): To release the resource when you're done.

- fstat(fd, &st): To get status information about the resource.

- dup(fd) or dup2(old_fd, new_fd): To duplicate or redirect fds.

2. struct stat (File Status): All About the File's Metadata
It's a data structure that holds detailed metadata about a file or directory. 
- stat(const char *path, struct stat *buf): Gets information about the file specified by its path.

- fstat(int fd, struct stat *buf): Gets information about the file associated with a given file descriptor (fd).

- lstat(const char *path, struct stat *buf): Similar to stat, but if path is a symbolic link, it gets information about the link itself, not the file it points to.

3. struct dirent (Directory Entry): What's Inside a Directory
It  represents a single entry within a directory. When you read the contents of a directory, you're essentially getting a list of these dirent structures, one for each file or subdirectory inside.