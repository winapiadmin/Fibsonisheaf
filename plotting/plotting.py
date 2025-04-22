import numpy as np
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# --- CONFIG ---
filenames = [
    #"naive.dat",
    "iterative.dat",
    "matrix_v1.dat",
    "matrix_v2.dat",
    "binet.dat",
    "iterativev2.dat"
]
labels = [
    #"Naive Recursive",
    "Iterative",
    "Matrix v1",
    "Matrix v2",
    "Binet",
    "Iterative v2"
]
# ---------------

def load_data(filename):
    # Each line is just a time value, line number = index
    times = np.loadtxt(filename)
    indices = np.arange(len(times))  # Create index array based on line number
    return indices, times

# Set up the plot
fig, ax = plt.subplots(figsize=(10, 6))

ax.set_ylabel("Runtime (seconds)")
ax.set_xlabel("Fibonacci Index (n)")
ax.set_ylim(0, 1)  # Set suitable x-axis limits for runtime (adjust as needed)
ax.set_xlim(0, 70000)  # Set suitable y-axis limits for Fibonacci index (adjust as needed)

# Load the data for all algorithms (load data once for faster access)
data = [load_data(fn) for fn in filenames]
max_lengths = [len(d[:, 0]) for d in data]  # Get the maximum length for each dataset

# Initialize plot lines for each dataset
lines = [ax.plot([], [], label=label)[0] for label in labels]

# Precompute the x and y values for all frames (avoids redundant computation during each update)
precomputed_data = [
    {
        'x': d[:, 0],  # Fibonacci index
        'y': d[:, 1],  # runtime
    }
    for d in data
]

# Define the update function for animation
def update(frame):
    # Update the plot with data up to the current frame for each dataset
    for idx, pd in enumerate(precomputed_data):
        lines[idx].set_data(pd['x'][:frame], pd['y'][:frame])  # Update line data up to the current frame
    
    return lines  # Return the updated lines for the animation

# Create the animation with blit=True for faster updates
ani = FuncAnimation(fig, update, frames=max(max_lengths), interval=1/4096, repeat=False)

# Show the plot
plt.show()
