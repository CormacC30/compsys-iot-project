import express from "express";
import { dashboardController } from "./controllers/dashboard-controller.js";
import { aboutController } from "./controllers/about-controller.js";

export const router = express.Router();

router.get("/", dashboardController.index);
router.get("/dashboard", dashboardController.index);
router.get("/about", aboutController.index);
router.post('/updateDehumidifier', dashboardController.updateDehumidifier);

//router.post("/dashboard/update-dehumidifier", dashboardController.updateDehumidifier);

router.use((req, res, next) => {
    res.status(404).send("404 - Not Found");
  });


