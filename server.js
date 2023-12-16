import express from "express";
import favicon from "serve-favicon";
import path from "path";
import bodyParser from "body-parser";
import cookieParser from "cookie-parser";
import fileUpload from "express-fileupload";
import { engine } from "express-handlebars";
import { router } from "./routes.js";
import dotenv from 'dotenv';
dotenv.config();

const app = express();
app.use(cookieParser());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(express.static("public"));
app.use(favicon(path.join("public", "images", "favicon.ico")));
app.use(fileUpload());
app.engine(".hbs", engine({ extname: ".hbs", helpers: handlebarsHelpers }));
app.set("view engine", ".hbs");
app.set("views", "./views");
app.use("/", router);

const listener = app.listen(process.env.PORT || 4000, function () {
    console.log(`mould-o-matic started on http://localhost:${listener.address().port}`);
  });