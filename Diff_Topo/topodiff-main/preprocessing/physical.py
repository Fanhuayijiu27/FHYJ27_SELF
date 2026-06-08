# Code used to generate physical fields
import numpy as np
from solidspy import solids_GUI
import matplotlib.pyplot as plt

size = 64
#Number of samples for which to generate physical fields
num_samples = 10

#Loading of file containing samples information
dict_array = np.load(f"./test_data_level_2_summary.npy", allow_pickle = True, encoding = 'latin1') #.npy filename can be replaced by any file containing samples information in the right format - e.g., all XXX_summary.npy files in dataset

for i in range(dict_array.size):
    load_nodes_i = np.empty(dict_array[i]['load_coord'].shape[0])
    for j,coord in enumerate(dict_array[i]['load_coord']):
        node = int(round(64*coord[0])*65+round(64*(1.0 - coord[1])))
        if node < 0:
            node = 0
        load_nodes_i[j] = node + 1
    dict_array[i]['load_nodes'] = load_nodes_i.astype(int)


def create_files(BC_conf, load_position, load_x_value, load_y_value, directory):
    # nodes.txt file
    # Boundary conditions of nodes
    BC_node = np.zeros(((size + 1) ** 2, 2))
    for elem in BC_conf:
        list_nodes = elem[0]
        type_bc = elem[1]
        for n in list_nodes:
            if type_bc == 1 or type_bc == 3:
                BC_node[n - 1, 0] = -1
            if type_bc == 2 or type_bc == 3:
                BC_node[n - 1, 1] = -1
    # Creating the file
    f = open(f"{directory}/nodes.txt", "w")
    for node in range(1, (size + 1) ** 2 + 1):
        # Coordinates of nodes
        x = node // (size + 1)
        r = node % (size + 1)
        if r != 0:
            y = (size + 1) - r
        else:
            x -= 1
            y = 0

        f.write(f"{node - 1}  {x:.2f}  {y:.2f}  {BC_node[node - 1, 0]:.0f}  {BC_node[node - 1, 1]:.0f}" + "\n")
    f.close()

    # eles.txt file
    f = open(f"{directory}/eles.txt", "w")
    num_elem = 0
    for node in range(1, (size + 1) ** 2 + 1):
        if node % (size + 1) != 0 and node < (size + 1) ** 2 - size:
            f.write(
                f"{num_elem}  1  0  {node - 1}  {node - 1 + 1}  {node - 1 + (size + 2)}  {node - 1 + (size + 1)}" + "\n")
            num_elem += 1
    f.close()

    # mater.txt file
    f = open(f"{directory}/mater.txt", "w")
    f.write("1.0  0.3")
    f.close()

    # loads.txt file
    f = open(f"{directory}/loads.txt", "w")
    for i, pos in enumerate(load_position):
        f.write(f"{pos - 1}  {load_x_value[i]:.1f}  {load_y_value[i]:.1f}" + "\n")
    f.close()

def resize(arr):
    res = np.empty((64,64))
    for i in range(64):
        for j in range(64):
            res[i,j] = arr[i,j]+arr[i+1,j]+arr[i,j+1]+arr[i+1,j+1]
    return res*0.25


for i in range(num_samples):
    vf = dict_array[i]['VF']
    vf_arr = vf * np.ones((64, 64))

    BC_conf = dict_array[i]['BC_conf']
    load_position = dict_array[i]['load_nodes']
    load_x_value = dict_array[i]['x_loads']
    load_y_value = dict_array[i]['y_loads']

    create_files(BC_conf, load_position, load_x_value, load_y_value, "./")
    disp, strain, stress = solids_GUI(plot_contours=False, compute_strains=True, folder="./")
    stress = stress.reshape((65, 65, 3)).swapaxes(0, 1)
    print(stress.shape)
    strain = strain.reshape((65, 65, 3)).swapaxes(0, 1)
    stress = stress.transpose([2, 0, 1])
    strain = strain.transpose([2, 0, 1])

    strain_energy_density = 0.5 * (stress[0] * strain[0] + stress[1] * strain[1] + 2 * stress[2] * strain[2])
    von_mises_stress = np.sqrt(
        np.power(stress[0], 2) - stress[0] * stress[1] + np.power(stress[1], 2) + 3 * np.power(stress[2], 2))

    res = np.transpose(np.stack([vf_arr, resize(strain_energy_density), resize(von_mises_stress)]), [1, 2, 0])
    print(f"{i + 1} physical fields computed")
    np.save(f"./cons_pf_array_{i}.npy", res)
