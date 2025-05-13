# Running simulations with Channelflow 2.0 (modified to allow for homotopy between plane-Poiseuille and plane-Couette flow)

Supposing that you have no initial condition to start from, we first need to create one. A random initial condition can be created like:
```
randomfield -Nx 256 -Ny 49 -Nz 256 -Lx 40 -Lz 40 -m 0.01 ./urandom.nc
```
This creates (and saves to disk) random perturbations (read `randomfield --help` for details) that get added on top of the underlying laminar base flow at the start of a simulation (so the same random initial condition can be used between different flows in principle). `x` and `z` are the periodic directions, respectively streamwise and spanwise if `-theta 0` (`-theta`, the tilting angle in radians, is an argument of `simulateflow`, and by default it is zero.) `-Nx` and `-Nz` refer to the total number of modes *including* the zero-padded modes for 3/2-dealiasing, so in reality there are `2Nx/3` modes = gridpoints in the x direction, and likewise `2Nz/3` modes = gridpoints in the z direction. By default the `y` direction (wallnormal) is *not* dealiased, so `Ny` is the actual number of Chebyshev modes = gridpoints. `-Lx` and `-Lz` set the lengths of the x and z directions in the units of the half-gap, so intrinsically, `Ly=2`. `-m` refers to the magnitude of the perturbation, might need to be increased to trigger turbulence depending on Re for subcritically stable flows.

Given an initial condition file (which contains the geometry parameters, so you do not need to set them again), it can be run with `simulateflow` like:
```
mpirun -np 64 simulateflow -T 10000 -R 200 -mc bulkv -Ubulk 0 -Uupperwall 1 -Ulowerwall 0 -dT 1 -dt 0.01 -vdt false -e 0.0000001 -np0 8 -np1 8 -dv ./urandom.nc
```
In this command, `np = np1 * np0` refers to the total number of MPI tasks, `np1 >= np0 >= 1` set the distribution of the grid for parallelization (`mod(Nx,np0)=0` has to hold, "np1 divides the x-dimension in physical space and the z-dimension in spectral.", quoted from the [Channelflow 2.0 forum](https://discourse.channelflow.ch/t/recommendations-for-np0-and-np1/33).), theoretically it's for the best if `np1 ~ np0`, but often it is found that `np0=1` gives the best results.

`-T` sets the total integration time. The run can be aborted early if nonzero `-e` is provided, which aborts the run if the crossflow energy component goes below it.
`-R` is the Reynolds number.
`-mc bulkv` keeps the bulk velocity at `-Ubulk`.
`-Uupperwall` set the speed of the upper wall, likewise `-Ulowerwall` sets the speed for the lower wall. So `-Uupperwall 1 -Ulowerwall 0` is the `\sigma =0` case of the standard Couette-Channel homotopy, while `-Uupperwall 1 -Ulowerwall 1` would be pure plane-Poiseuille (flowing leftwards, in the frame of reference of bulk velocity, velocity scale being the bulk velocity), `-Uupperwall 1 -Ulowerwall -1` would be pure plane-Couette flow.
`-dt 0.01 -vdt false` sets constant time steps of size 0.01. You can later set `-vdt true` for dynamic adjustment of time step sizes. (See `simulateflow --help` for the CFL parameters.)
Every `-dT` state files and statistics on the flow get written to the disk.
`-dv` prints out the divergence norm of the field every so often, which is useful when you are not sure about the resolution.

The data gets recorded in a subfolder `data/`.