import { getDatabase, ref, onValue, get, orderByKey, limitToLast } from 'firebase/database';
import { initializeApp } from "firebase/app";
import * as firebase from "firebase/database";

const apiKey = process.env.API_KEY;
const authDomain = process.env.AUTH_DOMAIN;
const databaseURL = process.env.DATABASE_URL;
const projectId = process.env.PROJECT_ID;
const storageBucket = process.env.STORAGE_BUCKET;
const messagingSenderId = process.env.MESSAGING_SENDER_ID;
const appId = process.env.APP_ID;
console.log("API_KEY:", process.env.API_KEY);
console.log("AUTH_DOMAIN:", process.env.AUTH_DOMAIN);
export const firebaseConfig = {
  apiKey: apiKey,
  authDomain: authDomain,
  databaseURL: databaseURL,
  projectId: projectId,
  storageBucket: storageBucket,
  messagingSenderId: messagingSenderId,
  appId: appId
};
//console.log("Firebase config keys: ", firebaseConfig);

//admin.initializeApp(firebaseConfig);
const app = initializeApp(firebaseConfig);
// Reference to the Firebase Realtime Database
const db = getDatabase(app);

// Reference to the 'sensors' node
const sensorsRef = ref(db, 'sensors');

let latestSensorData = {
  temperature: null,
  humidity: null,
  airQuality: null,
  ambientLight: null,
  moisture: null,
  time: null,
};

// Real-time data listener from Firebase
onValue(sensorsRef, (snapshot) => {
  const data = snapshot.val();
  if (data) {
    // Assuming you want the latest reading, use the last child
    const latestReadingKey = Object.keys(data).pop();
    const latestReading = data[latestReadingKey];

    latestSensorData = {
      temperature: latestReading.temperature,
      humidity: latestReading.humidity,
      airQuality: latestReading.air_quality,
      ambientLight: latestReading.ambient_light,
      moisture: latestReading.moisture,
      time: latestReading.timestamp,
    };

    // Notify subscribers
    notifySubscribers(latestSensorData);
  }
});

const subscribers = [];
// Function to subscribe to real-time updates
const subscribeToSensorData = (callback) => {

  subscribers.push(callback);
};

export const sensorDataDB = {
  subscribe: subscribeToSensorData,
};

// Internal function to notify subscribers
const notifySubscribers = (data) => {
  // Call all subscribed callbacks
  subscribers.forEach((callback) => {
    callback(data);
  });
};

export const fetchHistoricalData = async () => {
  //const sensorsRef = ref(db, 'sensors');

  try {
    const snapshot = await get(sensorsRef);
    const historicalData = [];

    snapshot.forEach((childSnapshot) => {
      const data = childSnapshot.val();
      historicalData.push({
        temperature: data.temperature,
        timestamp: data.timestamp,
      });
    });

    console.log('Fetched Historical Data:', historicalData);
    return historicalData;
  } catch (error) {
    console.error('Error fetching historical data:', error);
  }
};
/*
// Example usage:
fetchHistoricalData((data) => {
  console.log('Fetched Historical Data:', data);
  // You can do something with the historical data here
});


fetchHistoricalData()
  .then((data) => {
    console.log('Fetched Historical Data:', data);
    renderChart(data); // Call your chart rendering function here
  })
  .catch((error) => console.error('Error:', error));
*/