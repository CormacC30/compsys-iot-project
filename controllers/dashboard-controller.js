//import { Blynk } from 'blynk-library';

import { device } from "../models/device.js";
import { sensorDataDB } from "../models/firebase.js";


// Initialize an array to store the latest sensor data
let latestSensorData = {
  temperature: null,
  humidity: null,
  airQuality: null,
  moisture: null,
  ambientLight: null,
};

// Subscribe to real-time updates
sensorDataDB.subscribe((data) => {
  latestSensorData = data;
  console.log("Real-time data updated:", latestSensorData); // Log the data for debugging
});
//const dehumidifierStatus = 1;
export const dashboardController = {

    async index(request, response) {
        
        try {
            // Call fetchData method from the device module
            const pinData = await device.fetchData();
      
            // Include the fetched data in the viewData object
            const viewData = {
              title: "Dashboard",
              
              temperature: latestSensorData?.temperature,
              humidity: latestSensorData?.humidity,
              airQuality: latestSensorData?.airQuality,
              moisture: latestSensorData?.moisture,
              ambientLight: latestSensorData?.ambientLight,
              dehumidifierStatus: pinData.v0,
              
            };
      
            console.log("rendering dashboard");
            response.render("dashboard", viewData);
          } catch (error) {
            console.error("Error fetching data:", error);
            // Handle the error as needed
            response.status(500).send("Internal Server Error");
          }
        },

// Assuming this is your route handler for updating the dehumidifier
async updateDehumidifier(request, response) {
    try {
      // Fetch current pin data
      var pinData = await device.fetchData();
      
      // Extract dehumidifier status
      var dehumidifierStatus = pinData.v0;
  
      // Toggle the dehumidifier status (0 to 1 or 1 to 0)
      var newStatus = dehumidifierStatus === 0 ? 1 : 0;
  
      // Call updatePin function from the device module
      device.updatePin("V0", newStatus);
  
      // Redirect back to the dashboard
      response.redirect("/dashboard");
    } catch (error) {
      console.error("Error updating dehumidifier status:", error);
      // Handle the error as needed
      response.status(500).send("Internal Server Error");
    }
  },
};
