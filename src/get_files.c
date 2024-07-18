#include <dirent.h>     // To read directory
#include <sys/stat.h>   // To get file properties
#include <stdlib.h>     // Create dynamic list of files
#include <string.h>

/*
 * Function: get_files
 * ----------------------
 * 
 * get's the regular files and corresponding count inside a directory
 * located on a path.
 *
 * Parameters:
 * -----------
 * 
 * path : pointer to a char that represents a string containing the
 *      : file path of the directory.
 *
 * files : pointer to a NULL char** where the new pointer will be written
 *       : that represents an array of strings where each
 *       : string is the name of the dir plus the name of the file.
 *
 * returns: SUCCESS : the file_count of regular files inside the directory.
 *
 *          ERROR : -1 in the case no regular files were found on directory or
 *                  invalid directory.
 */
int16_t get_files(char *path, char ***files)
{
    struct dirent **list_of_files;
    int16_t lines = scandir(path, &list_of_files, 0, alphasort);
    
    // No files or directory doesn't exist
    if (lines <= 0)
        return -1;

    // Current file
    struct stat properties;
    char *file_path;

    // The list of files will be resized to only accomodate regular files.
    // but initially it will be of lenght lines.
    *files = (char**)calloc(lines, sizeof(char*));
    int16_t path_len = strlen(path);
    for (int16_t line = 0, file = 0; line < lines; file++)
    {
        file_path = (char *)malloc(sizeof(char) * (path_len +
                    strlen(list_of_files[file] -> d_name) + 1));

        strcpy(file_path, path);
        strcat(file_path, list_of_files[file] -> d_name);
        stat(file_path, &properties); // Get properties of current file

        // https://www.man7.org/linux/man-pages/man7/inode.7.html
        // Remove non-regular files (e.g directories)
        if ((properties.st_mode & S_IFMT) != S_IFREG) 
        {
            free(file_path);
            lines--;
            if (lines == 0) // No regular files inside directory.
                return -1;

            *files = (char**) realloc(*files, lines * sizeof(char*));
            continue;
        }

        (*files)[line] = file_path;
        line++;
    }
    return lines;
}
