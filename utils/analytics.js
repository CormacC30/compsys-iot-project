//import { fetchHistoricalData } from "../models/firebase";

export const analytics = {

async chartData(chartData){
    
    let report = {
        tempTrend: [],
        airQualityTrend: [],
        humidityTrend: [],
        ambientLightTrend: [],
        trendLabels: []
      };
      const startIndex = Math.max(0, chartData.length - 1000);
    const endIndex = chartData.length;
    for (let i = startIndex; i < endIndex; i++){
        //const timestamp = new Date(chartData[i].timestamp).getTime() / 1000;  // Convert to Unix timestamp
        const dataPoint = chartData[i];
        report.tempTrend.push(dataPoint.temperature);
        report.airQualityTrend.push(dataPoint.airQuality);
        report.humidityTrend.push(dataPoint.humidity);
        report.ambientLightTrend.push(dataPoint.ambientLight);
        report.trendLabels.push(dataPoint.timestamp);
    }
    return report;
}

};