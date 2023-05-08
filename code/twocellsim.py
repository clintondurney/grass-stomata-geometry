import numpy as np
import os
import globals as const 

def make_dir(directory):

    file_path = directory
    command = 'mkdir ' + file_path
    os.system(command)

def load_initial_mesh(mesh_name):

    Process.Mesh__System__Open('', mesh_name, 'No', 'No')
    Process.Mesh__Selection__Action__Clear_Selection('All')

def ref_cfg(rod, wall):

    Process.Mesh__Selection__Action__Load_Selection('rods.txt')
    Process.Model__CCF__03_Reference_Configuration('Linear Triangle', 'Triangle Element', 'False', rod)
    Process.Mesh__Selection__Action__Invert_Selection('Faces')
    Process.Model__CCF__03_Reference_Configuration('Linear Triangle', 'Triangle Element', 'False', wall)

def set_stress_strain():

    Process.Mesh__Selection__Action__Clear_Selection('All')
    Process.Mesh__Selection__Action__Select_All('Faces')
    Process.Model__CCF__04_StressStrain('Stress', 'Strain', '', '', '', '', '', 'Von Mises Stress', 'Stress', 'Strain', '1.0', 'No', '1', 'Linear Triangle', 'Triangle Element', 'TransIso Material')

def set_aniso_dir():

    Process.Mesh__Selection__Action__Clear_Selection('All')
    Process.Mesh__Selection__Action__Select_All('Faces')
    Process.Model__CCF__06_Set_Ansio_Dir('Linear Triangle', 'Triangle Element', 'E2', '0.0 1.0 0.0', 'Parallel', '1e-6')
    Process.Model__CCF__21_Visualize_Directions('Material Direction', 'Growth Direction', '1.0', 'Linear Triangle', 'Triangle Element', '1.e-6')

def material_props(E13_gc, E2_gc):
    
    # Guard Cells
    Process.Mesh__Selection__Action__Clear_Selection('All')
    Process.Mesh__Selection__Action__Load_Selection('GCs.txt')
    Process.Mesh__Selection__Faces__Select_Faces_of_Volumes()
    Process.Mesh__Selection__Action__Clear_Selection('Volumes')
    Process.Model__CCF__05_Set_Material_Properties(E13_gc, E2_gc, const.poisson_coef, '1', '2', 'TransIso Material')
    
def initial_pressure():

    Process.Mesh__Selection__Action__Clear_Selection('All')
    Process.Mesh__Selection__Action__Load_Selection('GCs.txt')
    Process.Model__CCF__90_Set_3D_Cell_Pressure(const.p_gc_i, 'Signal Name')
    Process.Model__CCF__91_Set_Face_Pressure_From_Volumes('Fem Pressure', 'Triangle Element', 'Signal Name')
    Process.Model__CCF__01_FEM_Membranes('10', '.1', '.0001', '10', '0.1', '10', '0.1', 'Backward Euler', 'Preconditioned Conjugate Gradient', '50', '1e-10', 'Yes', '10e-5', 'Model/CCF/04 StressStrain', 'Model/CCF/02 Triangle Derivs', 'Model/CCF/08 Pressure Derivs', 'Model/CCF/10 Dirichlet Derivs')

def set_gc_pressure(press):
    Process.Mesh__Selection__Action__Clear_Selection('All')
    Process.Mesh__Selection__Action__Load_Selection('GCs.txt')
    Process.Model__CCF__90_Set_3D_Cell_Pressure(press, 'Signal Name')

def run_fem():
    Process.Model__CCF__91_Set_Face_Pressure_From_Volumes('Fem Pressure', 'Triangle Element', 'Signal Name')
    Process.Model__CCF__01_FEM_Membranes('10', '.1', '.0001', '10', '0.1', '10', '0.1', 'Backward Euler', 'Preconditioned Conjugate Gradient', '50', '1e-10', 'Yes', '10e-5', 'Model/CCF/04 StressStrain', 'Model/CCF/02 Triangle Derivs', 'Model/CCF/08 Pressure Derivs', 'Model/CCF/10 Dirichlet Derivs')
    Process.Mesh__Heat_Map__Measures_3D__Geometry__Cell_Wall_Area('Cell Wall Area')
    Process.Mesh__Heat_Map__Measures_3D__Geometry__Volume('Volume')

def write_csv_save_mesh(a, press_gc, file_path):
    Process.Mesh__Structure__Center_Mesh() 
    Process.Mesh__Heat_Map__Measures_3D__Geometry__Cell_Wall_Area('Cell Wall Area')
    Process.Mesh__Heat_Map__Measures_3D__Geometry__Volume('Volume')
    if (a == 0): 
        Process.Model__CCF__103_Geometry_to_CSV('100',str(press_gc),str(0),'data.csv','True')
    else:
        Process.Model__CCF__103_Geometry_to_CSV('100',str(press_gc),str(0),'data.csv','False')
    Process.Mesh__System__Save('',file_path + '/GC_' + str(int(press_gc*10)) + '.mdxm', 'no')

def move_csv(directory):
    file_path = directory
    command = 'mv data.csv ' + file_path
    os.system(command)

def iterate_pressure(p_gc, where):
    print("Incrementing Guard Cell and Subsidiary Cell Pressure")
    for i in range(0,len(p_gc)):    
        print("P_GC = ", p_gc[i])
   
        set_gc_pressure(p_gc[i]) 
        run_fem() 
        write_csv_save_mesh(i+1, p_gc[i], where)

# begin running functions here
pressure_gc = np.linspace(const.gc_start, const.gc_end, const.num_points) 

# make directories for saving 
out_dir = const.out_dir + "E1_" + str(const.E1_E3_gc) + "_E2_" + str(const.E2_gc)
make_dir(out_dir)

# save initial template (unpressurized)
load_initial_mesh(const.initial_mesh)
write_csv_save_mesh(0, 0, out_dir)

# set material properties
ref_cfg(const.rod_thickness, const.wall_thickness)
set_stress_strain()
set_aniso_dir()
material_props(const.E1_E3_gc, const.E2_gc)

# Run simulation and save outputs
initial_pressure()
write_csv_save_mesh(1, const.p_gc_i, out_dir)
print("Incrementing Guard Cell and Subsidiary Cell Pressure")
iterate_pressure(pressure_gc, out_dir)
move_csv(out_dir)


