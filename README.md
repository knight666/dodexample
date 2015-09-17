# Overview

An example of possible optimizations when using data-oriented design.

![Screenshot](https://bitbucket.org/repo/o6dnpp/images/1731901630-Screenshot930.png)

# Compiling

Use [GYP](https://chromium.googlesource.com/external/gyp) to generate a solution and compile it using Visual Studio 2012 or above.

    set GYP_MSVS_VERSION=2012
    gyp --depth project.gyp

# Command line

    -help
        Display this help text and exit.

    -profiling [FRAME_COUNT]
        Enable profiling mode with a default frame count of 100.

    -dod
        Use data-oriented design logic.

    -ood
        Use object-oriented design logic (default).

    -scene {SMALL|MEDIUM|LARGE}
        Set the scene size on start-up.