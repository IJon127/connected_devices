//load module ------------
const express = require("express");
const fs = require("fs");

//set server and port ------------
const server = express();
const port = 8080;

//load pages & read files -------------
const homePage = fs.readFileSync(`../public/index.html`, "utf-8");
const data = fs.readFileSync(`../public/data.json`, "utf-8");

//server ----------------------
server.get("/", (req, res) => {
  res.status(200).send(homePage);
});
server.get("/data", (req, res) => {
  res.status(200).json(data);
});

server.listen(port, () => {
  console.log("Listening to requests on port 8080...");
});
