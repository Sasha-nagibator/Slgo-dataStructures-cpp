import matplotlib.pyplot as plt
import os

data_folder = "cmake-build-debug/data"

operations = ["creation", "find", "find_random", "delete"]

data_structures = ["avl", "hash", "sort"]

for operation in operations:
    plt.figure(figsize=(10, 6))

    for data_structure in data_structures:
        file_name = f"{data_structure}_{operation}"
        file_path = os.path.join(data_folder, file_name)
        
        with open(file_path, 'r') as file:
            data = list(map(float, file.read().split()))
        
        plt.plot(data, label=data_structure)
    
    plt.title(operation)
    plt.legend()
    plt.savefig(f"{operation}.png")

