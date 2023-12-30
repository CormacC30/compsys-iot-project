// data-controller.js

import { fetchHistoricalData } from '../models/firebase.js';
import { analytics } from '../utils/analytics.js';

export const getHistoricalData = async (req, res) => {
  try {
    // Fetch historical data from the database
    const historicalData = await fetchHistoricalData();
    
    // Log the historical data to the console
    console.log('Historical Data:', historicalData);

    // Respond with a success message or other appropriate response
    const chartData = await analytics.chartData(historicalData);
    //console.log("chart data: ", chartData);
    const viewData = {
      title: "data-controller",
      chartData: chartData,
    };
    res.render("historical-data", viewData);
    
  } catch (error) {
    console.error('Error fetching historical data:', error);
    res.status(500).send('Internal Server Error');
  }
};