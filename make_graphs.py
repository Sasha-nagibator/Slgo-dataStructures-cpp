import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import os

data_folder = "cmake-build-debug/data"

operations = ["creation", "find", "find_random", "remove", "delete"]

data_structures = ["avl", "hash", "sort"]

for operation in operations:
    fig, ax = plt.subplots(figsize=(10, 6))

    for data_structure in data_structures:
        if operation == "remove" and data_structure == "sort":
            continue
        file_name = f"{data_structure}_{operation}"
        file_path = os.path.join(data_folder, file_name)

        with open(file_path, 'r') as file:
            data = list(map(float, file.read().split()))

        ax.plot(data, label=data_structure)

    ax.set_title(operation)
    ax.legend()

    ax.set_xlabel('Number of elements (*10^5)')
    ax.set_ylabel('sec')

    ax.xaxis.set_major_formatter(ticker.FuncFormatter(lambda x, pos: f'{(x+1)}'))

    fig.savefig(f"{operation}.png")
