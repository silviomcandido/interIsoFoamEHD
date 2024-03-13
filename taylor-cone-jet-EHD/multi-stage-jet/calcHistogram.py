import os
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import pyvista as pv
from sklearn.cluster import DBSCAN
from natsort import natsorted

# Set matplotlib parameters for better visualization
plt.rcParams.update({
    'text.usetex': True,
    'font.family': 'serif',
    'font.size': 20,
    'xtick.labelsize': 20,
    'ytick.labelsize': 20,
    'savefig.bbox': 'tight',
    'axes.linewidth': 1.2,
    'lines.linewidth': 3.0
})

# Function to read cloud data and perform DBSCAN clustering
def read_cloud_and_cluster(folder, time_dirs):
    cloud_droplets = pd.DataFrame()
    for time in time_dirs:
        fname = f'./{folder}/postProcessing/samples/{time}/iso1.vtp'
        DATA = pv.read(fname)
        X = np.hstack((DATA.points[:, 0].reshape(-1, 1), -DATA.points[:, 1].reshape(-1, 1)))
        model = DBSCAN(eps=1e-5, min_samples=3).fit(X)
        labels = model.labels_
        clusters = np.unique(labels)
        for cluster in clusters:
            row_ix = np.where(labels == cluster)
            if np.min(np.abs(X[row_ix, 1])) < 1e-5:
                Lj = np.max(np.abs(X[row_ix, 1])) - np.min(np.abs(X[row_ix, 1])))
            else:
                Lj = -1
            centroidx = np.mean(X[row_ix, 0])
            centroidy = np.mean(X[row_ix, 1])
            Ri = np.mean(np.sqrt((X[row_ix, 0] - centroidx)**2 + (X[row_ix, 1] - centroidy)**2))
            qE = np.sum(DATA['rhoE'][row_ix])
            sphericity = max(abs(X[row_ix, 1].max() - X[row_ix, 1].min()) / abs(X[row_ix, 0]).max(),
                             abs(X[row_ix, 0]).max() / abs(X[row_ix, 1].max()))
            droplet = pd.DataFrame({
                'Time': float(time),
                'Length': Lj,
                'Sphericity': sphericity,
                'Radius': Ri,
                'xC': centroidx,
                'yC': centroidy,
                'qE': qE
            }, index=[0])
            cloud_droplets = cloud_droplets.append(droplet, ignore_index=True)
    return cloud_droplets

# Main function to process data and generate plots
def main():
    # Get current directory
    dir_name = os.getcwd()
    QL = float(dir_name.split('\\')[-1].split('Q')[0].split('-')[1])
    postDatafolder = './'

    # Read simulation data
    time_dirs = natsorted(os.listdir('./postProcessing/samples/'))
    cloud_data = read_cloud_and_cluster(postDatafolder, time_dirs)

    # Save processed data
    cloud_data.to_csv(postDatafolder + 'cloudDroplets.dat', sep='\t', index=False)

    # Plot histogram of droplet diameters
    plt.figure(figsize=(10, 8))
    plt.hist(cloud_data['Radius'] * 1.5 * 1e6, bins=100, alpha=0.4, rwidth=0.85, label='Data')
    plt.axvline(55.102, color='k', label='$d_{exp}$: 55.102 $\mu$m')
    plt.axvline(np.mean(cloud_data['Radius']), color='r', label='$d_{num}$: %.3f $\mu$m' % np.mean(cloud_data['Radius']))
    plt.axvspan(55.102 * 0.95, 55.102 * 1.05, alpha=0.5, color='grey')
    plt.xlabel('Diameter ($\mu m$)')
    plt.ylabel('$\sum$')
    plt.xlim([0, 150])
    plt.legend()
    plt.tight_layout()
    plt.savefig('Radius_clustering.png', dpi=300)
    plt.close()

    # Plot histogram of droplet velocities
    plt.figure(figsize=(7, 8))
    plt.hist(cloud_data[cloud_data['yC'] > 10e-3]['Uy'], bins=100, alpha=1.0, color='blue', rwidth=0.85,
             label='$(z+H_1) \in [0, H_2]$: $\mu$=%.3f $m/s$' % np.mean(cloud_data[cloud_data['yC'] > 10e-3]['Uy']))
    plt.hist(cloud_data[cloud_data['yC'] < 10e-3]['Uy'], bins=100, alpha=0.2, color='red', rwidth=0.85,
             label='$z \in [0, H_1]$: $\mu$=%.3f $m/s$' % np.mean(cloud_data[cloud_data['yC'] < 10e-3]['Uy']))
    plt.xlabel('$u_y$ ($m/s$)')
    plt.ylabel('$\sum$')
    plt.xlim([2, 10])
    plt.ylim([0, 1300])
    plt.grid(alpha=0.15)
    plt.legend(loc='upper center')
    plt.savefig('Droplet_velocity.png', dpi=300)
    plt.close()

if __name__ == "__main__":
    main()
