# grass-stomata-geometry
MorophDynamX and MorphoMechanX code used in "Grasses exploit geometry to achieve improved guard cell dynamics" 

### (May 8, 2023: Currently being amended)

The following code was used in "Grasses exploit geometry to achieve improved guard cell dynamics" Durney et. al. 2023.  

The simulations were run on an Ubuntu 20.04 Virtual Machine [(vmware.org)](vmware.org) without use of an NVIDIA graphics card. 

To install MorphoDynamX/MorphoMechanX.

1. Install Ubuntu 20.04
2. Install MorphoDynamX using the provided .deb package

    ```sudo dpkg -i \*.deb``` 

To run the automated models, the following Python packages need to be installed:
1. __numpy__
2. __os__
3. (optional) __PIL__ (for making snapshots and gifs of the pressurized models)

These can be installed using ```apt-get```.

Example: ```sudo apt-get install numpy```

To run a model.
* Navigate to the directory 'code' in a terminal.
* Update globals.py to set your target output directory and change any model parameters.
* ```$ make clean``` 
* ```$ make```
* ```$ make run```

MorphoDynamX should launch.

* In MorphoDynamX, under /Process/Tools/Python/Python Script input ```fourcellsim.py``` or ```twocellsim.py``` for a four or two cell simulation respectively.
* Press 'Step' (the play button) to run the simulation.

To run the model with an alternate mesh, replace baseline.mdxm and the associated *.txt files with those in the sub-folders named 'barley-meshes', 'brachy-meshes' and 'bdmute-meshes'.

To create pictures/gif of the simulation. 
* In MorphoDynamX, under /Process/Tools/Python/Python Script input ```take-pics.py```. 
* Press 'Step' (the play button) to create the pictures/gif.
(Also see limitations below)

__Current limitations__
1. Only one Python script can run with each launch of MorphoDynamX. Once one Python script run and completed, MorphoDynamX needs to be closed and re-launched using the above steps.

2a. The Python script take-pics.py works by taking screenshots within MorphoDynamX. Therefore, the overlayed text will not be in the same location for each setup. The user will need to edit take-pics.py to overlay text for their images.

2b. Similarly, the heatmap is overlayed as well. If the heatmap for stress is changed from 0-75 MPa, heatmap.png will need to be replaced with a picture of the new legend.

