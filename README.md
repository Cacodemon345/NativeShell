# Native Shell Improved
This is a fork of Native Shell with corrected backspace behaviour and the ability to display Windows version information.

## Requirements
Windows Driver Kit Version 7.1.0

## Build

Build command for WDK x86 Free Build Environment:

`build /g /w`

Build output is native.exe.

# Install

Copy native.exe to %systemroot%\system32\

Use add.reg from `install` directory.

# Uninstall
Use remove.reg from `install` directory.

Delete %systemroot%\system32\native.exe

