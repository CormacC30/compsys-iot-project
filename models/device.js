import * as https from "https";

const authToken = process.env.BLYNK_AUTH_TOKEN; 
console.log('Blynk Auth Token:', process.env.BLYNK_AUTH_TOKEN);
const pinNumber = 1; // Replace with the desired virtual pin number

const options = {
  hostname: "blynk.cloud",
  port: 443,
  path: `/external/api/getAll?token=${authToken}`, 
  method: "GET",
};

export const device = {
  fetchData: () => {
    return new Promise((resolve, reject) => {
      const req = https.request(options, (res) => {
        let data = "";

        res.on("data", (chunk) => {
          data += chunk;
        });

        res.on("end", () => {
          try {
            const pinData = JSON.parse(data);
            resolve(pinData);
          } catch (error) {
            reject(error);
          }
        });
      });

      req.on("error", (e) => {
        reject(e);
      });

      req.end();
    });
  },

  updatePin: function (pinNumber, status) {
    return new Promise((resolve, reject) => {
      // Ensure that the status is either 0 or 1
      if (status !== 0 && status !== 1) {
        reject(new Error("Invalid status value. Must be 0 or 1."));
        return;
      }

      // Construct the options for the HTTP request
      const options = {
        hostname: "blynk.cloud",
        port: 443,
        path: `/external/api/update?token=${authToken}&${pinNumber}=${status}`,
        method: "GET",
      };

      // Create the HTTP request
      const req = https.request(options, (res) => {
        let data = "";

        // A chunk of data has been received.
        res.on("data", (chunk) => {
          data += chunk;
        });

      });

      // Handle HTTP request errors
      req.on("error", (e) => {
        console.error("Error with Blynk request:", e);
        reject(new Error("Error with Blynk request."));
      });

      // End the HTTP request
      req.end();
    });
  },
};
