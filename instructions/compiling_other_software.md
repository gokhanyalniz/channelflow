# Compiling Channelflow 2.0

## Setting up the paths for the prerequisites

### Compiler locations
Please adapt the following paths according to your system. I assume C/C++/Fortran compilers are already installed somewhere, below are examples for the GNU compiler suite, but you could also work with Intel compilers. Likewise I assume OpenMPI (built using the same compiler set) is already installed.

Input the locations of various compilers:
```
# C compiler
export CC=/usr/bin/gcc
# C++ compiler
export CXXC=/usr/bin/g++
# Fortran compiler
export FC=/usr/bin/gfortran
```
Input the locations of the corresponding MPI compilers:
```
export MCC=/usr/bin/mpicc
export MCXX=/usr/bin/mpic++
export MFC=/usr/bin/mpifort
```

### Library locations
Below I provide recipes for how to build the required libraries. With them you can build the libraries yourself, or alternatively, you can try to use the preinstalled versions, if they exist / can be installed in your system.

Where the compiled libraries should be installed to:
```
export USRLOCAL=/home/gokhan/usr/local
```

You will need to update `LD_LIBRARY_PATH` for the resulting compiled libraries to be seen:
```
export LD_LIBRARY_PATH=${USRLOCAL}/lib:$LD_LIBRARY_PATH
```

### Channelflow location
Folder to install Channelflow 2.0 binaries to:
```
export SIMS=/home/gokhan/usr/local/sims
```
In the end, the binaries will be located in `${SIMS}/channelflow`.

## Building the prerequisites

All commands that follow are to be run in the folders extracted from
the downloaded archives, unless otherwise stated.

### [libzip](https://libzip.org/download/)
Create a build directory within the extracted archive and go into it.
Then run
```
cmake ../ \
-DCMAKE_C_COMPILER=$CC \
-DCMAKE_C_FLAGS="-fPIC -O3" \
-DCMAKE_INSTALL_PREFIX=$USRLOCAL
make install
```

### [FFTW](https://www.fftw.org/download.html) (with MPI)
```
./configure --prefix=$USRLOCAL --enable-mpi \
CC=$CC \
MPICC=$MCC \
CFLAGS="-fPIC -O3"
make install
```

### [HDF5](https://github.com/HDFGroup/hdf5/releases) (serial) [do not upgrade to v2.0 and higher when they get released, not for now!]
```
./configure --prefix=$USRLOCAL \
CC=$CC \
CFLAGS="-fPIC -O3 -I${USRLOCAL}/include" \
LDFLAGS=-L${USRLOCAL}/lib
make install
```

### [NetCDF-C](https://downloads.unidata.ucar.edu/netcdf/) (serial)
```
./configure --prefix=$USRLOCAL --disable-dap \
CC=$CC \
CFLAGS="-fPIC -O3 -I${USRLOCAL}/include" \
LDFLAGS=-L${USRLOCAL}/lib
make install
```

### [NetCDF-Fortran](https://downloads.unidata.ucar.edu/netcdf/) (serial)
```
./configure --prefix=$USRLOCAL \
CC=$CC \
FC=$FC \
F77=$FC \
CFLAGS="-fPIC -O3 -I${USRLOCAL}/include" \
FFLAGS="-fPIC -O3 -I${USRLOCAL}/include" \
LDFLAGS=-L${USRLOCAL}/lib
make install
```

### [Eigen](https://eigen.tuxfamily.org)
Move the folder `Eigen` within the extracted directory to `${USRLOCAL}/include/`.
In the end, the folder `${USRLOCAL}/include/Eigen/src/` should exist.

## Building [Channelflow 2.0](https://github.com/gokhanyalniz/channelflow/tree/couettechannel)
Create a build directory within the extracted archive / cloned repository and go into it. Then run
```
cmake ../ \
-DCMAKE_BUILD_TYPE=release \
-DCMAKE_C_COMPILER=$MCC \
-DCMAKE_C_FLAGS="-fPIC -O3" \
-DCMAKE_CXX_COMPILER=$MCXX \
-DCMAKE_CXX_FLAGS="-fPIC -O3" \
-DCMAKE_INSTALL_PREFIX=$CHANNELFLOW \
-DFFTW_INCLUDE_DIR=${USRLOCAL}/include \
-DFFTW_LIBRARY=${USRLOCAL}/lib/libfftw3.a \
-DFFTW_MPI_LIBRARY=${USRLOCAL}/lib/libfftw3_mpi.a \
-DWITH_NETCDF=Serial \
-DNETCDF_INCLUDE_DIR=${USRLOCAL}/include \
-DNETCDF_LIBRARY=${USRLOCAL}/lib/libnetcdf.a \
-DEIGEN3_INCLUDE_DIR=${USRLOCAL}/include \
-DWITH_GTEST=OFF
make -j install
```

To run the compiled binaries, all the aliases that were `export`ed until this point will need to be repeated each time a new terminal session is started. The easiest solution is to put all those into your `.profile` or `.bashrc` file, or into an "environment" file that you `source` when needed.