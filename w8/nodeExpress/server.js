var express = require("express");
var server = express();
server.use("/", express.static("public"));
server.listen(process.env.PORT || 8080);

/**************************************************************************
Node.js Express.js practice 
Uses Tom Igoe's FourLineServer:
https://github.com/tigoe/NodeExamples/tree/main/ExpressIntro/FourLineServer
created 29 March 2022 
modified 29 March 2022 by I-Jon Hsieh
**************************************************************************/
