//import { fetchHistoricalData } from "../models/firebase";

export const analytics = {

async chartData(chartData){
    
    let report = {
        tempTrend: [],
        trendLabels: []
      };
      const startIndex = Math.max(0, chartData.length - 100);
    const endIndex = chartData.length;
    for (let i = startIndex; i < endIndex; i++){
        const timestamp = new Date(chartData[i].timestamp).getTime() / 1000;  // Convert to Unix timestamp
        report.tempTrend.push(chartData[i].temperature);
        report.trendLabels.push(timestamp);
    }
    return report;
}

};