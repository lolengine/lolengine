
# About

A collection of scripts and C++ code to make games, demos and tools.

The project is being heavily refactored as a transition effort to the best
practices of the 2020s. The old `master` branch still exists if needed.

## Notes

### Build dependencies

The following packages are required on Linux:

 - for bootstrapping: `automake libtool pkg-config`
 - for building: `clang make`

### Nintendo Switch support

That code is private, it needs to be activated manually:

    git clone https://github.com/samhocevar/lol-nx.git src/private/nx
