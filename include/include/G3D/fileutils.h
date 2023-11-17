/**
 @file fileutils.h
 
 @maintainer Morgan McGuire, matrix@graphics3d.com
 
 @author  2002-06-06
 @edited  2006-02-04

 Copyright 2000-2006, Morgan McGuire.
 All rights reserved.
 */

#ifndef G3D_FILEUTILS_H
#define G3D_FILEUTILS_H

#include "G3D/platform.h"
#include <string>
#include <stdio.h>
#include "G3D/Array.h"
#include "G3D/g3dmath.h"

namespace G3D {

    namespace _internal {
        extern Array<std::string> currentFilesUsed;
    }

/** Returns all the files used by G3D and GLG3D during the current execution. */
Array<std::string> filesUsed();
    
std::string readFileAsString(
    const std::string&          filename);

/**
 @param flush If true (default), the file is ready for reading as soon
 as the function returns.  If false, the function returns immediately and
 writes the file in the background.
 */
void writeStringToFile(
    const std::string&          str,
    const std::string&          filename,
    bool                        flush = true);

/**
 Creates the directory (which may optionally end in a /)
 and any parents needed to reach it.
 */
void createDirectory(
    const std::string&          dir);

/**
 Fully qualifies a filename.  The filename may contain wildcards,
 in which case the wildcards will be preserved in the returned value.
 */
std::string resolveFilename(const std::string& filename);

/**
 Appends all files matching filespec to the files array.  The names
 will not contain paths unless includePath == true.  These may be
 relative to the current directory unless the filespec is fully qualified
 (can be done with resolveFilename).
 Wildcards can only appear to the right of the last slash in filespec.
 */
void getFiles(
	const std::string&			filespec,
	Array<std::string>&			files,
	bool						includePath    = false);

/**
 Appends all directories matching filespec to the files array. The names
 will not contain paths unless includePath == true.  These may be
 relative to the current directory unless the filespec is fully qualified
 (can be done with resolveFilename).
 Does not append special directories "." or "..".
 */
void getDirs(
	const std::string&			filespec,
	Array<std::string>&			files,
	bool						includePath = false);


/** Returns true if the specified path exists and is a directory */
bool isDirectory(const std::string& filespec);

/** Returns the length of the file, -1 if it does not exist */
int64 fileLength(const std::string& filename);

/**
 Copies the file
 */
void copyFile(
    const std::string&          source,
    const std::string&          dest);

/** Returns a temporary file that is open for read/write access.  This
    tries harder than the ANSI tmpfile, so it may succeed when that fails. */
FILE* createTempFile();

/**
 Returns true if the given file (or directory) exists.
 Must not end in a trailing slash.
 */
bool fileExists(
    const std::string&          filename);

/**
  Parses a filename into four useful pieces.

  Examples:

  c:\a\b\d.e   
    root  = "c:\"
    path  = "a" "b"
    base  = "d"
    ext   = "e"
 
  /a/b/d.e
    root = "/"
    path  = "a" "b"
    base  = "d"
    ext   = "e"

  /a/b
    root  = "/"
    path  = "a"
    base  = "b"
    ext   = "e"

 */
void parseFilename(
    const std::string&  filename,
    std::string&        drive,    
    Array<std::string>& path,
    std::string&        base,
    std::string&        ext);


/**
 Returns the part of the filename that includes the base and ext from
 parseFilename (i.e. everything to the right of the path).
 */
std::string filenameBaseExt(const std::string& filename);

/**
 Returns the extension on a filename.
 */
std::string filenameExt(const std::string& filename);

/** 
 Returns the drive (if Win32) and path from a filename, including 
 a slash if there was one.
 <CODE>filenamePath(f) + filenameBaseExt(f) == f</CODE>
 */
std::string filenamePath(const std::string& filename);

/** Returns true if '*' or '?' appears in the string */
bool filenameContainsWildcards(const std::string& filename);

/** Returns true if dst does not exist or src is newer than dst.  Works on both files and directories. */
bool fileIsNewer(const std::string& src, const std::string& dst);

} // namespace

#endif

