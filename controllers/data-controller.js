// data-controller.js

import { fetchHistoricalData } from '../models/firebase.js';

export const getHistoricalData = async (req, res) => {
  try {
    // Fetch historical data from the database
    const historicalData = await fetchHistoricalData();

    // Log the historical data to the console
    console.log('Historical Data:', historicalData);

    // Respond with a success message or other appropriate response
   // res.status(200).json(historicalData);
  } catch (error) {
    console.error('Error fetching historical data:', error);
    res.status(500).send('Internal Server Error');
  }
};
