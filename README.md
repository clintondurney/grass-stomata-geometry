# grass-stomata-geometry
MorophDynamX and MorphoMechanX code used in "Grasses exploit geometry to achieve improved guard cell dynamics" 

The following code was used in "Grasses exploit geometry to achieve improved guard cell dynamics" Durney et. al. 2023.  

The simulations were run on Ubuntu 20.04 Virtual Machine (vmware.org) without use of an NVIDIA graphics card. 


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





