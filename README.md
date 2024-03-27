# GPUFluid

Primary simulation is based off this paper:
https://hal.inria.fr/inria-00596050/document

Martin Guay, Fabrice Colin, Richard Egli. Simple and Fast Fluids. GPU Pro, 2011, GPU Pro, 2,
pp.433-444. ffinria-00596050f

This is a one-pass mostly mass-conserving fluid simulation, 
which requires no GPU-based linear solver to run and only execute 5 texture reads per pixel. 
Since the shader is also effectively branchless in the main computing work, 
this algorithm runs very efficiently on even my laptop GPU.

I have also implemented the algorithm known as vorticity confinement, to make up for detail
lost to numerical inaccuracies, and the program allows the user to play with some weird values
for the constants to see what happens.

Screenshots of two different preset simulations and an example of how the UI looks:

<img width="912" alt="image" src="https://github.com/jearmstrong21/GPUFluid/assets/20100635/3b39aaea-3a29-4233-90ef-68555aed0456">
<img width="912" alt="image" src="https://github.com/jearmstrong21/GPUFluid/assets/20100635/da12e8d0-dc49-4351-ba2d-fb748b37c96e">
