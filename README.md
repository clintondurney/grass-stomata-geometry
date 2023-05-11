# grass-stomata-geometry
MorophDynamX and MorphoMechanX code used in "Grasses exploit geometry to achieve improved guard cell dynamics" 

### (May 10, 2023: Currently being amended)

The following code was used in "Grasses exploit geometry to achieve improved guard cell dynamics" Durney et. al. 2023.  

The simulations were run on an Ubuntu 20.04 Virtual Machine [(vmware.org)](vmware.org) without use of an NVIDIA graphics card. However, the CUDA library and Thrust need to be installed for the models to compile. These can be installed [here](https://developer.nvidia.com/cuda-downloads?target_os=Linux&target_arch=x86_64&Distribution=Ubuntu&target_version=20.04&target_type=deb_local).

To install MorphoDynamX/MorphoMechanX for Ubuntu 20.04.

1. Install MorphoDynamX using the provided .deb package

    ```sudo dpkg -i MDX-2.0.2-1236-Ubuntu20.04-OMP-CellMaker-FemLib-Gmsh.deb``` 

If MorphoDynamX fails to install, the terminal should give you an indication of why. This is most likely due to a dependency issue.  Some known dependencies that could be missing at this stage are:
* g++

This can be installed using ```apt-get```. Example,

```$ sudo apt-get install g++```

With MorphoDynamX installed, we now need to compile the source code for the models to run using MorphoMechanX.

* In the terminal, navigate to the directory 'code'. Then run the following,
* ```$ make clean``` 
* ```$ make```
* ```$ make run```

If an error occurs, most likely there are missing dependencies. Once again, the terminal should give an indication of what is missing. When setting up the Virtual Machine the following dependencies were missing and were installed:
* CUDA/Thrust (see above for installation)
* libqt5opengl5-dev
* libboost-all-dev 
* libgsl-dev
* libtbb-dev 
* libqtmultimedia5
* libqtmultimediawidgets5
* libqtmultimedia5-plugins
* qtmultimedia5-dev

For a full list of potential dependencies, please see the file dependencies.txt included in this repository.

The following Python packages need to be installed:
1. __numpy__
2. (optional) __PIL__ (for making snapshots and gifs of the pressurized stoma meshes)

These can be installed using ```pip```.

Example: ```$ pip install numpy```

To run the automated models:
1. Update globals.py to set your target output directory and change any model parameters. It is necessary to change the output directory to follow your own file structure.
2. ```$ make clean``` and MorphoDynamX should open.
3. In MorphoDynamX, under /Process/Tools/Python/Python Script input ```fourcellsim.py``` or ```twocellsim.py``` for a four or two cell simulation respectively.
4. Press 'Step' (the play button) to run the simulation.
Thisl should now run a simulation. To run the model with an alternate mesh, replace baseline.mdxm and the associated *.txt files with those in the sub-folders named 'barley-meshes', 'brachy-meshes' and 'bdmute-meshes'.

To create pictures/gif of the simulation. 
* In MorphoDynamX, under /Process/Tools/Python/Python Script input ```take-pics.py```. 
* Press 'Step' (the play button) to create the pictures/gif. The output input/output directory is that specified in globals.py.
(Also see limitations below)

__Current limitations__
* Only one Python script can run with each launch of MorphoDynamX. Once one Python script run and completed, MorphoDynamX needs to be closed and re-launched using the above steps.
* To create pictures/gifs:
  * The Python script take-pics.py works by taking screenshots within MorphoDynamX. Therefore, the overlayed text will not be in the same location for each setup. The user will need to edit take-pics.py to overlay text for their images.
  *  Similarly, the heatmap is overlayed as well. If the heatmap for stress is changed from 0-75 MPa, heatmap.png will need to be replaced with a picture of the new legend.




