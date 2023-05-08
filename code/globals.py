# Global variables for stoma simulations

# Directory and File details
initial_mesh = 'baseline.mdxm'
out_dir = '/home/cdurney/curr-biol/bdmute/'
outdirSuffix = ''

# cell wall variables 
rod_thickness = 3.0 
wall_thickness = 1.0
## Guard Cell Young's Moduli
E1_E3_gc = 40 
E2_gc = 75 
## Subsidiary Cell Young's Moduli
E1_E3_sc = 40 
E2_sc = 75 
## aniso direction ('Parallel' or 'Orthogonal') for isotropic, set E1/E3=E2
anisoDir = 'Orthogonal'

poisson_coef = 0.3

# pressure variables
## Initial Pressure
p_gc_i = 0.5 
p_sc_i = 0.7 

## pressure incrementer
num_points = 5 
gc_start = 1.0 
gc_end = 5.0 
sc_start = 0.6 
sc_end = 0.2 



