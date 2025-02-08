
# About

A collection of scripts and C++ code to make games, demos and tools.

The project is being heavily refactored as a transition effort to the best
practices of the 2020s. The old `master` branch still exists if needed.

## Notes

### Confidential platform support

Support for private platforms need to be activated manually:

    git clone --recurse https://github.com/lolengine/lol-playstation
    git clone --recurse https://github.com/lolengine/lol-switch
    git clone --recurse https://github.com/lolengine/lol-xbox

### Build dependencies

The following packages are required on Linux:

 - for bootstrapping: `automake libtool pkg-config`
 - for building: `clang make`
