"use strict";

const colNum = 8;
const rowNum = 8;
const cellNum = colNum * rowNum;
let cells = [];
let cellsContainer = document.getElementById("container");
let treasureLoc;
let bombLoc;
let currentLoc = 0;

const makeCells = function () {
  for (let i = 0; i < cellNum; i++) {
    cells[i] = document.createElement("div");
    cells[i].className = "cell";
    cellsContainer.appendChild(cells[i]);
  }
};

const placeYourself = function () {
  cells[currentLoc].classList.add("current_location");
};

const placeTreasure = function () {
  treasureLoc = Math.trunc(Math.random() * cellNum);
  cells[treasureLoc].classList.add("treasure");
};

const placeBomb = function () {
  let tempLoc = Math.abs(Math.trunc(Math.random() * cellNum) - 1);
  if (tempLoc !== treasureLoc) {
    bombLoc = tempLoc;
  } else {
    bombLoc = tempLoc - 1;
  }
  cells[bombLoc].classList.add("bomb");
};

const leftLocation = function () {
  cells[currentLoc].classList.remove("current_location");
};

const getTreasure = function () {
  if (currentLoc === treasureLoc) {
    cells[treasureLoc].classList.remove("treasure");
    cells[bombLoc].classList.remove("bomb");
    placeTreasure();
    placeBomb();
  }
};

const reachBomb = function () {
  if (currentLoc === bombLoc) {
    document.getElementById("explosion").classList.remove("hidden");
  }
};

makeCells();
placeYourself();
placeTreasure();
placeBomb();

window.addEventListener("keydown", (event) => {
  leftLocation();
  if (event.key === "ArrowUp") {
    if (currentLoc >= colNum) {
      currentLoc -= colNum;
    }
  }
  if (event.key === "ArrowDown") {
    if (currentLoc < cellNum - colNum) {
      currentLoc += colNum;
    }
  }
  if (event.key === "ArrowLeft") {
    if (currentLoc % colNum !== 0) {
      currentLoc--;
    }
  }
  if (event.key === "ArrowRight") {
    if (currentLoc % colNum !== colNum - 1) {
      currentLoc++;
    }
  }
  placeYourself();
  getTreasure();
  reachBomb();
});
