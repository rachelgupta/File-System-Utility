This code defines a basic file system utility with the ability to create files, delete files, create subdirectories, and display files and directory structure. The disk is represented as an array of blocks, each with a fixed size. Files are stored within these blocks, and the allocateBlocks function is responsible for finding contiguous blocks for file storage. The freeBlocks function is called when a file is deleted to free up the blocks previously allocated to that file.
