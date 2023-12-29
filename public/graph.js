import Chart from 'chart.js/auto';
import { fetchHistoricalData } from '../models/firebase';
const renderChart = (historicalData) => {
  const labels = historicalData.map((entry) => new Date(entry.timestamp).toLocaleTimeString());
  const temperatures = historicalData.map((entry) => entry.temperature);

  const ctx = document.getElementById('myChart').getContext('2d');
  const chart = new Chart(ctx, {
    type: 'line',
    data: {
      labels: labels,
      datasets: [
        {
          label: 'Temperature',
          data: temperatures,
          borderColor: 'red',
          borderWidth: 1,
        },
      ],
    },
  });
};

// Fetch historical temperature data and render the chart
fetchHistoricalData()
  .then((data) => {
    console.log('Fetched Historical Data:', data);
    renderChart(data); // Call your chart rendering function here
  })
  .catch((error) => console.error('Error:', error));

/*
fetchHistoricalData().then((data) => {
    console.log('Fetched Historical Data:', data);
  }).catch((error) => {
    console.error('Error fetching historical data:', error);
  });
  */