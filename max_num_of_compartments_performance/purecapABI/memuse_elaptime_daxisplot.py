"""
memuse_elaptime_daxisplot.py
  Takes a csv file with metrics collects about
  - memory used by instantiated compartments
  - elapsed time to instantiate a compartment
  and builds a dualaxis plot.

Programmer: Regis Schuch (regis.schucg@unijui.edu.br)

         Carlos Molina (carlos.molina@cl.cam.ac.uk
         Computer Lab, University of Cambridge)
         edited the original ver 23 Feb 2025. 

Execute: % python memuse_elaptime_daxisplot.py

         The file max_num_compart-experiment-cheriOS22.12-results.csv
         is expected in local directory.
 
         We have tested with 
         Python 3.12.5 (v3.12.5:ff3bc82f7c9)
         on a Mac Book Air.  
"""


import pandas as pd
import matplotlib.pyplot as plt

# Load data 
data = pd.read_csv('./max_num_compart-experiment-purecapABI-results.csv')

# Read metrics from 3 columns
processes = data['Number of Processes'] # should say Number of Processes
memory_usage = data['Memory Used (MB)']
elapsed_time = data['Time Elapsed (ms)']

# Adjust the elapsed time scale to improve visualisation (e.g. divide by 1000)
elapsed_time_scaled = elapsed_time / 1000  # Converting to seconds

# Create a dual axis plot
fig, ax1 = plt.subplots(figsize=(10, 6))

# Plot Memory Usage on the left Y axis 
ax1.set_xlabel('Number of compartments instantiated')
ax1.set_ylabel('Memory used (MB)', color='blue')
ax1.plot(processes, memory_usage, label='Memory Usage (MB)', color='blue', marker='o')
ax1.tick_params(axis='y', labelcolor='blue')

# Create the right Y axis for Elapsed Time
ax2 = ax1.twinx()
ax2.set_ylabel('Elapsed Time (s)', color='green')  # Using the scale set to seconds
ax2.plot(processes, elapsed_time_scaled, label='Elapsed time (s)', color='green', marker='s')
ax2.tick_params(axis='y', labelcolor='green')

# Add title and grid
plt.title('Memory used and Elapsed time vs Number of compartments instantiated')
plt.grid(True)

# Adjust the layout and display the graphic
plt.tight_layout()
plt.show()
