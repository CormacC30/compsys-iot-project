//import { Blynk } from 'blynk-library';

import { device } from "../models/device.js";
//const dehumidifierStatus = 1;
export const dashboardController = {

    async index(request, response) {
        
        try {
            // Call fetchData method from the device module
            const pinData = await device.fetchData();
      
            // Include the fetched data in the viewData object
            const viewData = {
              title: "Dashboard",
              temperature: pinData.v1,
              humidity: pinData.v2,
              airQuality: pinData.v3,
              moisture: pinData.v4,
              ambientLight: pinData.v5,
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
/*
var blynk = new Blynk('-8LxkAzVWEMcC1JJz4qkoRQEZLhYOKM7', {
    addr: blynk.cloud,
});

// Function to read data from Blynk and return a Promise
function readDataFromBlynk(pin) {
    return new Promise((resolve, reject) => {
        // Use Blynk's read functions here
        // For example, if you have a virtual pin V1 for temperature, you can use:
        blynk.virtualRead(pin, (value) => {
            if (value !== null) {
                resolve(value);
            } else {
                reject(new Error('Failed to read data from Blynk.'));
            }
        });
    });
}
*/