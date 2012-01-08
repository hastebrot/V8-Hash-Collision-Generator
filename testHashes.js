#!/usr/bin/env node

var fs = require("fs");
var file = fs.readFileSync(process.argv[2],"utf8");

console.error("file read, lets's start parsing...");

var json = JSON.parse(file);
