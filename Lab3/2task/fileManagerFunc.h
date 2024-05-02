#ifndef FM_FUNCTIONS_H
#define FM_FUNCTIONS_H

#define DIR_MODE 0755
#define FILE_MODE 644

#include <fcntl.h>

/// @brief Make new directory.
/// @param pathname directory name.
void fm_mkdir(const char* pathname, mode_t mode);

/// @brief List directory contents.
/// @param pathname directory name.
void fm_ls(const char* pathname);

/// @brief Remove directory recursively.
/// @param pathname directory name.
void fm_rmdir_r(const char* pathname);

/// @brief Create new regular file.
/// @param filename file to create.
void fm_create(const char* filename, mode_t mode);

/// @brief Print file contents.
/// @param filename file to print.
void fm_cat(const char* filename);

/// @brief Remove file.
/// @param filename file to be deleted.
void fm_unlink(const char* filename);

/// @brief Create symbolic link.
/// @param target file to associate with the link.
/// @param link link name.
void fm_symlink(const char* target, const char* link);

/// @brief Read contents of symbolic link.
/// @param link symbolic link.
void fm_readlink(const char* link);

/// @brief Create hard link.
/// @param oldpath name of the original file.
/// @param newpath link name.
void fm_link(const char* oldpath, const char* newpath);

/// @brief Print file permissions and link count.
/// @param filename file to examine.
void fm_stat(const char* filename);

/// @brief Set file mode.
/// @param pathname file name.
/// @param mode new mode.
void fm_chmod(const char* pathname, mode_t mode);

#endif