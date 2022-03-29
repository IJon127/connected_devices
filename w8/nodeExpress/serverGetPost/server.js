const express = require("express");
const server = express();
server.use("/", express.static("public"));

const bodyParser = require("body-parser");
server.use(bodyParser.json());
server.use(bodyParser.urlencoded({ extended: true }));

let data = new Array();

function handleGet(request, response) {
  console.log("got a GET request");
  response.json(data);
}

function handlePost(request, response) {
  console.log("got a POST request");
  let incomingData = request.body;
  console.log(incomingData);
  data.push(incomingData);
  response.json(data);
}

server.listen(process.env.PORT || 8080);
server.get("/readings", handleGet);
server.post("/data", handlePost);

/**************************************************************************
Node.js Express.js practice Uses Tom Igoe's express-basics.js:
https://github.com/tigoe/NodeExamples/blob/main/ExpressIntro/express-basics.js
created 29 March 2022 
modified 29 March 2022 by I-Jon Hsieh
**************************************************************************/
