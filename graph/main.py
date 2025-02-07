import requests
import matplotlib.pyplot as plt
from datetime import datetime

# Firebase URL
firebase_url = "https://esp32-heatmonitor-default-rtdb.europe-west1.firebasedatabase.app/sensor_data.json?auth=AIzaSyCSVIYn6qg6sfrewTAPGvifEGKtHzIx0q4"  # Replace with your Firebase URL

# Fetch data from Firebase
response = requests.get(firebase_url)
data = response.json()

# Prepare data for plotting
timestamps = []
temp1_values = []
temp2_values = []
temp3_values = []

for timestamp, readings in data.items():
    # Convert timestamp to datetime for better plotting
    dt_object = datetime.strptime(timestamp, "%Y-%m-%d_%H:%M:%S")
    timestamps.append(dt_object)
    temp1_values.append(readings.get("temperature1"))
    temp2_values.append(readings.get("temperature2"))
    temp3_values.append(readings.get("temperature3"))

# Plot the data
plt.figure(figsize=(12, 6))
plt.plot(timestamps, temp1_values, label="Temperature 1", marker="o")
plt.plot(timestamps, temp2_values, label="Temperature 2", marker="s")
plt.plot(timestamps, temp3_values, label="Temperature 3", marker="^")

# Customize the plot
plt.title("Temperature Readings Over Time")
plt.xlabel("Time")
plt.ylabel("Temperature (°C)")
plt.legend()
plt.grid(True)
plt.tight_layout()

# Show the plot
plt.show()
