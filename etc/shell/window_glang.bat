@echo off
set "GCC_PATH_ADD=D:\Program Files\LLVM\bin;C:\MinGW\bin"
if defined GCC_PATH_BASE (
  set "PATH=%GCC_PATH_BASE%"
) else (
  set "GCC_PATH_BASE=%PATH%"
)
set "PATH=%GCC_PATH_ADD%;%PATH%"
echo - clang and gnu compiler are now 64 bit
echo.
@echo on