import { Chart } from 'chart.js/auto';

// Function to fetch historical data and generate a chart
async function generateHistoricalChart() {
  let historicalChart; 
  try {
    const response = await fetch('/historical-data'); // Update the endpoint accordingly
    const historicalData = await response.json();

    // Extract temperature and time data
    const temperatureData = historicalData.map(entry => entry.temperature);
    const timeData = historicalData.map(entry => new Date(entry.timestamp)); // Parse timestamps as Date objects

    // Create a line chart
    const ctx = document.getElementById('historicalChart').getContext('2d');
    historicalChart = new Chart(ctx, {
      type: 'line',
      data: {
        labels: timeData,
        datasets: [{
          label: 'Temperature',
          data: temperatureData,
          borderColor: 'rgba(75, 192, 192, 1)',
          borderWidth: 1,
          fill: false,
        }],
      },
      options: {
        scales: {
          x: {
            type: 'time',
            time: {
              unit: 'day', 
            },
            title: {
              display: true,
              text: 'Time',
            },
          },
          y: {
            beginAtZero: true,
            title: {
              display: true,
              text: 'Temperature',
            },
          },
        },
      },
    });
  } catch (error) {
    console.error('Error fetching historical data:', error);
  }
  console.log('Chart Type:', historicalChart.config.type);

};

// Call the generateHistoricalChart function when the page loads
document.addEventListener('DOMContentLoaded', generateHistoricalChart);