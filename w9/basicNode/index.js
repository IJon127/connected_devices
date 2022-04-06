//load module ------------
const http = require("http");
const url = require("url");
const fs = require("fs");

//load pages & read files -------------
const homePage = fs.readFileSync(`../public/index.html`, "utf-8");
const data = fs.readFileSync(`../public/data.json`, "utf-8");

//server ----------------
const server = http.createServer((req, res) => {
  const pathName = req.url;

  if (pathName === "/" || pathName === "/home") {
    res.writeHead(200, { "Content-type": "text/html" });
    res.end(homePage);
  } else if (pathName === "/about") {
    res.end("What to learn about me?");
  } else if (pathName === "/data") {
    res.writeHead(200, { "Content-type": "application/json" });
    res.end(data);
  } else {
    res.writeHead(404, { "Content-type": "text/html" });
    res.end("<h1>Page not found!!!</h1>");
  }
});

server.listen(8000, "127.0.0.1", () => {
  console.log("Listening to requests on port 8000");
});
