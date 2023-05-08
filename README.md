# grass-stomata-geometry
MorophDynamX and MorphoMechanX code used in "Grasses exploit geometry to achieve improved guard cell dynamics" 

### (May 8, 2023: Currently being amended)

The following code was used in "Grasses exploit geometry to achieve improved guard cell dynamics" Durney et. al. 2023.  

The simulations were run on Ubuntu 20.04 Virtual Machine (vmware.org) without use of an NVIDIA graphics card. 

MorphoDynamX depends on the following: 
 
* Depends: xdg-utils
*  Depends: libqt5widgets5
*  Depends: libqt5opengl5
*  Depends: libqt5xml5
*  Depends: libqt5printsupport5
*  Depends: libtbb2
*  Depends: libtriangle-1.6
*  Depends: libgsl23
*  Recommends: libtbb2

Which can be installed using apt-get.
Example:
```sudo apt-get install xdg-utils'''


To install MorphoDynamX/MorphoMechanX.

1. Install Ubuntu 20.04
2. Install necessary requirements (list...)
3. Install Python dependencies
4. Install MorphoDynamX using the provided .deb package

    ```sudo dpkg -i \*.deb``` 

To run a model.
* Open the folder 'code' in a terminal.
* Update globals.py to set your target output directory and change any model parameters.
* Run ```make clean```
* Run ```make```
* Run ```make run```
MorphoDynamX should launch.
* In MorphoDynamX, under /Process/Tools/Python/Python Script input ```fourcellsim.py``` or ```twocellsim.py``` for a four or two cell simulation respectively.
* Press 'Step' (the play button) to run the simulation.

To run the model with an alternate mesh, replace baseline.mdxm and the associated *.txt files with those in the folders named 'barley-meshes', 'brachy-meshes' and 'bdmute-meshes'.

Current limitation: Only one Python script can run with each launch of MorphoDynamX. Once one Python script run and completed, MorphoDynamX needs to be closed and re-launched using the above steps.


