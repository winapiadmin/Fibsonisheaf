import glob
import os
import random
import hashlib
import json
import matplotlib.colors as mcolors

# Combine named colors
color_list = list(mcolors.TABLEAU_COLORS.values()) + list(mcolors.CSS4_COLORS.values())

config = {}

for f in glob.glob("*.dat"):
    algo = os.path.splitext(os.path.basename(f))[0]

    # Deterministic color choice using hash
    seed = int(hashlib.sha256(algo.encode()).hexdigest(), 16)
    rng = random.Random(seed)
    color = rng.choice(color_list)

    config[algo] = {
        "path": f"{algo}.dat",
        "colour": color,
    }

# Write to JSON file
with open("legend.json", "w") as f:
    json.dump(config, f, indent=4)

print("Wrote config to legend.json")
