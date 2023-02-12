# FileToHeader
A simple handy application for converting a binary file into a C++ .h header to be included directly in an executable.

## Usage
Compile the application; this has only been tested on Windows but there are no platform-specific mechanisms or code, so compiling on Linux or Mac should be no trouble.

Each argument passed to the application is an input filename.  On Windows, you can drag one or more files directly onto the executable.  A .h file with the same name as the input file (extension included) will be generated.  This C++ header file defines `FILE_<filename>_SIZE` which exposes how large, in bytes, the file data is, as well as `FILE_<filename>_DATA[]`, an array of unsigned char's containing the file data.  When included in a C++ project, this allows you to embed any file directly into the final executible file.  This should be used very sparingly, as you'll inflate the size of your .exe with all the extra files you're packing in!  This should only be used for resources which are funamental and required, such as default fonts and shaders, backup models and textures, etc.

Generated headers should work fine in pure C as well, but it's untested.
