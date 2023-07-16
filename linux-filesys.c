#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>

#define MAX_FILENAME 100
#define MAX_BUFFER_SIZE 1024

// Function to create a file
void createFile(char* path, char* filename, mode_t mode) {
    char filepath[MAX_FILENAME];
    snprintf(filepath, sizeof(filepath), "%s/%s", path, filename);

    int fd = creat(filepath, mode);
    if (fd == -1) {
        perror("Error creating file");
        return;
    }
    close(fd);
    printf("File '%s' created successfully.\n", filepath);
}

// Function to delete a file
void deleteFile(char* path, char* filename) {
    char filepath[MAX_FILENAME];
    snprintf(filepath, sizeof(filepath), "%s/%s", path, filename);

    int result = unlink(filepath);
    if (result == -1) {
        perror("Error deleting file");
        return;
    }
    printf("File '%s' deleted successfully.\n", filepath);
}

// Function to read and display the contents of a file
void readFile(char* path, char* filename) {
    char filepath[MAX_FILENAME];
    snprintf(filepath, sizeof(filepath), "%s/%s", path, filename);

    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return;
    }

    char buffer[MAX_BUFFER_SIZE];
    ssize_t bytesRead;
    while ((bytesRead = read(fd, buffer, MAX_BUFFER_SIZE)) > 0) {
        write(STDOUT_FILENO, buffer, bytesRead);
    }

    if (bytesRead == -1) {
        perror("Error reading file");
    }

    close(fd);
}

// Function to write data to a file
void writeFile(char* path, char* filename, char* data, mode_t mode) {
    char filepath[MAX_FILENAME];
    snprintf(filepath, sizeof(filepath), "%s/%s", path, filename);

    int fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, mode);
    if (fd == -1) {
        perror("Error opening file");
        return;
    }

    ssize_t bytesWritten = write(fd, data, strlen(data));
    if (bytesWritten == -1) {
        perror("Error writing to file");
    } else {
        printf("File '%s' written successfully.\n", filepath);
    }

    close(fd);
}

// Function to list files in a directory
void listFiles(char* path) {
    DIR* dir = opendir(path);
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
}

int main() {
    char parentDir[MAX_FILENAME];
    char childDir[MAX_FILENAME];
    char filename[MAX_FILENAME];
    char data[MAX_BUFFER_SIZE];
    mode_t mode;

    printf("Enter parent directory name: ");
    scanf("%s", parentDir);

    printf("Enter child directory name: ");
    scanf("%s", childDir);

    // Create parent directory
    if (mkdir(parentDir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
        perror("Error creating parent directory");
        return 1;
    }

    // Create child directory
    char childPath[MAX_FILENAME];
    snprintf(childPath, sizeof(childPath), "%s/%s", parentDir, childDir);
    if (mkdir(childPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
        perror("Error creating child directory");
        return 1;
    }

    // Perform file operations within the child directory
    chdir(childPath);

    int choice;
    do {
        printf("\nFile Management Menu\n");
        printf("--------------------\n");
        printf("1. Create File\n");
        printf("2. Delete File\n");
        printf("3. Read File\n");
        printf("4. Write to File\n");
        printf("5. List Files\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter filename to create: ");
                scanf("%s", filename);
                printf("Enter file permission in octal (e.g., 0644): ");
                scanf("%o", &mode);
                createFile(childPath, filename, mode);
                break;
            case 2:
                printf("Enter filename to delete: ");
                scanf("%s", filename);
                deleteFile(childPath, filename);
                break;
            case 3:
                printf("Enter filename to read: ");
                scanf("%s", filename);
                readFile(childPath, filename);
                break;
            case 4:
                printf("Enter filename to write to: ");
                scanf("%s", filename);
                printf("Enter data to write: ");
                scanf(" %[^\n]", data);
                printf("Enter file permission in octal (e.g., 0644): ");
                scanf("%o", &mode);
                writeFile(childPath, filename, data, mode);
                break;
            case 5:
                printf("Listing files in the directory:\n");
                listFiles(childPath);
                break;
            case 6:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 6);

    return 0;
}
