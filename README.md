# High-Resolution Two-Phase Electrohydrodynamic Solver for OpenFOAM

This repository hosts a solver implemented in OpenFOAM v1912 for simulating two-phase electrohydrodynamic (EHD) flows with high resolution, focusing on Taylor cone jet dynamics. The solver is based on the interIsoFoamEHD solver and incorporates the isoAdvector interface capturing method to ensure precise representation of fluid interfaces. It is capable of simulating systems involving two immiscible mediums with low conductivities.

## Features:
- **Two-phase Flow Simulation**: Capable of simulating the dynamics of two-phase flows, such as liquid-gas or liquid-liquid systems.
- **Electrohydrodynamics (EHD)**: Incorporates EHD effects to account for the influence of electric fields on fluid flow and particle motion.
- **High Resolution Interface Capture**: Utilizes the isoAdvector method for high-resolution interface capturing, ensuring accurate representation of fluid interfaces and minimizing numerical diffusion.
- **Taylor Cone Jet Dynamics**: Focused on simulating the dynamics of Taylor cone jets, a common phenomenon in electrohydrodynamics.
- **Automatic Mesh Generation**: Provides both automatic 2D axisymmetric and full 3D mesh generation with SnappyHexMesh in the tutorial folder.
- **Custom Solvers and Library**: Includes the interIsoFoamEHD solver in the `applications/solvers` folder and a library for immiscible two-phase flow in the `src/` folder.

## Usage:
1. Clone this repository to your local machine.
2. Compile the solver using the provided build scripts.
3. Prepare your case setup, including mesh generation and boundary conditions.
4. Run the simulation using standard OpenFOAM utilities.
5. Post-process the results using visualization tools compatible with OpenFOAM.

## Contribution:
Contributions to this project are welcome! Whether it's bug fixes, enhancements, or new features, feel free to submit pull requests or open issues to discuss potential improvements.

## Citation:
If you use this solver in your research or work, please cite the following papers:
- [1] S. Cândido and J. C. Páscoa, "On modal decomposition as surrogate for charge-conservative EHD modelling of Taylor Cone jets", International Journal of Engineering Science, Volume 193, 2023, 103947, ISSN 0020-7225, https://doi.org/10.1016/j.ijengsci.2023.103947.
- [2] S. Cândido and J. C. Páscoa, "Dynamics of three-dimensional electrohydrodynamic instabilities on Taylor cone jets using a numerical approach", Physics of Fluids 1 May 2023; 35 (5): 052110. https://doi.org/10.1063/5.0151109 

## Authors:
This solver was developed by Sílvio Cândido and José C. Páscoa under the academic supervision of the University of Beira Interior, with support from the C-MAST Center for Mechanical and Aerospace Science and Technologies and the FCT Portuguese Foundation for Science and Technology.


## License:
This project is licensed under the [MIT License](LICENSE), allowing for both personal and commercial use with proper attribution.


