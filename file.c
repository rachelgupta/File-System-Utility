#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 100
#define MAX_FILENAME_LENGTH 50
#define MAX_FILE_CONTENT_LENGTH 1000
#define BLOCK_SIZE 512
#define DISK_SIZE 1024 * 1024 // 1MB disk size

// Struct for representing a file
typedef struct {
    char name[MAX_FILENAME_LENGTH];
    int start_block;
    int num_blocks;
} File;

// Struct for representing a directory
typedef struct {
    char name[MAX_FILENAME_LENGTH];
    File files[MAX_FILES];
    int file_count;
} Directory;

// Struct for representing disk blocks
typedef struct {
    char data[BLOCK_SIZE];
} Block;

// Global variables
Block disk[DISK_SIZE / BLOCK_SIZE];
Directory root;

// Function prototypes
void initializeDirectory(Directory *dir, const char *name);
void createFile(const char *filename, const char *content);
void displayFiles();
void deleteFile(const char *filename);
void displayDiskUsage();
int allocateBlocks(int num_blocks);
void freeBlocks(int start_block, int num_blocks);

int main() {
    initializeDirectory(&root, "root");

    createFile("file1.txt", "This is file 1.");
    createFile("file2.txt", "This is file 2.");
    createFile("file3.txt", "This is file 3.");

    displayFiles();
    displayDiskUsage();

    deleteFile("file2.txt");
    printf("\nAfter deleting file2.txt:\n");
    displayFiles();
    displayDiskUsage();

    return 0;
}

// Initialize a directory
void initializeDirectory(Directory *dir, const char *name) {
    strcpy(dir->name, name);
    dir->file_count = 0;
}

// Create a file in the root directory
void createFile(const char *filename, const char *content) {
    if (root.file_count < MAX_FILES) {
        int num_blocks_needed = (strlen(content) + BLOCK_SIZE - 1) / BLOCK_SIZE;
        int start_block = allocateBlocks(num_blocks_needed);
        if (start_block != -1) {
            File *new_file = &root.files[root.file_count];
            strcpy(new_file->name, filename);
            new_file->start_block = start_block;
            new_file->num_blocks = num_blocks_needed;
            memcpy(disk[start_block].data, content, strlen(content));
            root.file_count++;
            printf("File '%s' created successfully.\n", filename);
        } else {
            printf("Cannot create file '%s'. Not enough disk space.\n", filename);
        }
    } else {
        printf("Cannot create file '%s'. Maximum files limit reached.\n", filename);
    }
}

// Display files in the root directory
void displayFiles() {
    printf("Files in directory '%s':\n", root.name);
    for (int i = 0; i < root.file_count; ++i) {
        printf("- %s\n", root.files[i].name);
    }
}

// Delete a file from the root directory
void deleteFile(const char *filename) {
    for (int i = 0; i < root.file_count; ++i) {
        if (strcmp(root.files[i].name, filename) == 0) {
            freeBlocks(root.files[i].start_block, root.files[i].num_blocks);
            for (int j = i; j < root.file_count - 1; ++j) {
                root.files[j] = root.files[j + 1];
            }
            root.file_count--;
            printf("File '%s' deleted successfully.\n", filename);
            return;
        }
    }
    printf("File '%s' not found.\n", filename);
}

// Display disk usage
void displayDiskUsage() {
    int used_blocks = 0;
    for (int i = 0; i < root.file_count; ++i) {
        used_blocks += root.files[i].num_blocks;
    }
    printf("Disk usage: %d blocks used, %d blocks free.\n", used_blocks, (DISK_SIZE / BLOCK_SIZE) - used_blocks);
}

// Allocate blocks for file storage
int allocateBlocks(int num_blocks) {
    int consecutive_blocks = 0;
    for (int i = 0; i < DISK_SIZE / BLOCK_SIZE; ++i) {
        if (consecutive_blocks == num_blocks) {
            return i - num_blocks;
        }
        if (disk[i].data[0] == '\0') {
            consecutive_blocks++;
        } else {
            consecutive_blocks = 0;
        }
    }
    return -1;
}

// Free blocks after deleting a file
void freeBlocks(int start_block, int num_blocks) {
    for (int i = start_block; i < start_block + num_blocks; ++i) {
        memset(disk[i].data, '\0', BLOCK_SIZE);
    }
}
