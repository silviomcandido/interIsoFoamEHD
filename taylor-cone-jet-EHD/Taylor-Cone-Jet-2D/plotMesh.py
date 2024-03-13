"""
Taylor Cone Jet Visualization Script

This script visualizes the results of a Taylor cone jet simulation using PyVista.

Usage:
- Make sure you have PyVista installed.

- Ensure that the simulation results are available in the 'postProcessing/samples/' directory.

- Adjust the parameters as needed in the script to customize the visualization.

- Run the script to generate and display the visualization.

Author: Silvio Candido

Date: 2024

"""

import os
import pyvista as pv
import numpy as np


# Set plot theme
pv.set_plot_theme("dark")

# Time
times = os.listdir('./postProcessing/samples/')
time = str(max(map(float, times)))

# File paths
isoSurface_file = f'./postProcessing/samples/{time}/iso1.vtp'
cutPlane1_file = f'./postProcessing/samples/{time}/face.vtp'

# Create the rendering scene
p = pv.Plotter()

# Add isoSurface
isoSurface = pv.read(isoSurface_file)
p.add_mesh(isoSurface, cmap="bwr", scalars="Ue", lighting=True)

# Add planar grid
cutPlane = pv.read(cutPlane1_file)
edges = cutPlane.extract_all_edges()
p.add_mesh(edges, color="white", line_width=1)

# Add text annotation for time
p.add_text(f'Time: {float(time)*1000:.1f} ms', font_size=20)

# Set view
p.view_xy()

# Show plot
p.show()

