import express from "express";
import { dashboardController } from "./controllers/dashboard-controller.js";
import { aboutController } from "./controllers/about-controller.js";

export const router = express.Router();

router.get("/", dashboardController.index);
router.get("/dashboard", dashboardController.index);
router.get("/about", aboutController.index);

router.use((req, res, next) => {
    res.status(404).send("404 - Not Found");
  });

router.post('/trigger-ifttt', async (req, res) => {
 try {
  // Trigger IFTTT event
  await axios.post('https://maker.ifttt.com/trigger/dehumidify/json/with/key/dhUEVYI8vPSvFOdJBIxPJh_IIeYmkDLINMy7egFHl0J');
  res.status(200).send('IFTTT event triggered successfully');
 } catch (error) {
  console.error('Error triggering IFTTT event:', error.message);
  res.status(500).send('Internal Server Error');
  }
 });