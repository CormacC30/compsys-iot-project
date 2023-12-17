import express from "express";
import path from "path";

import { fileURLToPath } from "url"; // New URL handling module

// Use the new URL API to get the path
const __filename = fileURLToPath(import.meta.url);
const __dirname = path.dirname(__filename);
import exphbs from "express-handlebars";
import { router } from "./routes.js";
import bodyParser from "body-parser";
import fileUpload from "express-fileupload";
import { engine } from "express-handlebars";
import axios from "axios";

const app = express();
app.use(bodyParser.urlencoded({ extended: false }));
app.use(fileUpload());

app.use(bodyParser.json());
app.engine(".hbs", engine({ extname: ".hbs"}));
app.set("view engine", ".hbs");
app.set("views", path.join(__dirname, "views"));  // Use path.join to ensure correct path
app.use("/", router);  // Place your router setup before static file serving
app.use(express.static(path.join(__dirname, "public")));  // Use path.join to ensure correct path

const listener = app.listen(process.env.PORT || 4000, function () {
    console.log(`mould-o-matic started on http://localhost:${listener.address().port}`);
});