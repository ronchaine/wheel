# Target OS name
set(CMAKE_SYSTEM_NAME Windows)

# Compilers to use for C / C++
SET(CMAKE_C_COMPILER x86_64-linux-musl-gcc)
SET(CMAKE_CXX_COMPILER x86_64-linux-musl-g++)

# Target environment
SET(CMAKE_FIND_ROOT_PATH /usr/x86_64-linux-musl/)

# Adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search 
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
