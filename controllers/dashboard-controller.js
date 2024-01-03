//import { Blynk } from 'blynk-library';

import { device } from "../models/device.js";
import { sensorDataDB } from "../models/firebase.js";
import { fetchHistoricalData } from '../models/firebase.js';

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
            var tempWarning;
            var humidityWarning;
            var healthWarning;
            var moistureWarning;
            var lightWarning;
            var warning1;
            var warning2;
            var warning3;
            var warning4;

            // set the value of the boolean flags
            if (latestSensorData?.temperature >= 18 && latestSensorData?.temperature <= 30){
              tempWarning = true;
            } else {
              tempWarning = false;
            };
            if (latestSensorData?.humidity >= 70 ){
              humidityWarning = true;
            } else {
              humidityWarning = false;
            };
            if (latestSensorData?.airQuality >= 60){
              healthWarning = true;
            } else {
              healthWarning = false;
            };
            if (latestSensorData?.moisture <= 700){
              moistureWarning = true;
            } else {
              moistureWarning = false;
            };
            if (latestSensorData?.ambientLight <= 100){
              lightWarning = true;
            } else {
              lightWarning = false;
            };

            if (tempWarning + lightWarning + humidityWarning + moistureWarning == 4){
              warning4 = true;
              warning3 = false;
              warning2 = false;
              warning1 = false;
            } else if (tempWarning + lightWarning + humidityWarning + healthWarning + moistureWarning == 3) {
              warning4 = false;
              warning3 = true;
              warning2 = false;
              warning1 = false;
            } else if (tempWarning + lightWarning + humidityWarning + healthWarning + moistureWarning == 2) {
              warning4 = false;
              warning3 = false;
              warning2 = true;
              warning1 = false;
            } else if (tempWarning + lightWarning + humidityWarning + healthWarning + moistureWarning == 1){
              warning4 = false;
              warning3 = false;
              warning2 = false;
              warning1 = true;
            } else {
              warning4 = false;
              warning3 = false;
              warning2 = false;
              warning1 = false;
            }
            
            
            const viewData = {
              title: "Dashboard",
              
              temperature: latestSensorData?.temperature,
              humidity: latestSensorData?.humidity,
              airQuality: latestSensorData?.airQuality,
              moisture: latestSensorData?.moisture,
              ambientLight: latestSensorData?.ambientLight,
              dehumidifierStatus: pinData.v0,

              tempWarning: tempWarning,
              humidityWarning: humidityWarning,
              healthWarning: healthWarning,
              moistureWarning: moistureWarning,
              lightWarning: lightWarning,

              warning4: warning4,
              warning3: warning3,
              warning2: warning2,
              warning1: warning1,

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
