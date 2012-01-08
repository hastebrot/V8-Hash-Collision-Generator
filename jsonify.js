#!/usr/bin/env node

var fs = require("fs");
var file = fs.readFileSync(process.argv[2],"utf8");
var lines = file.split("\n");

var values = [];

for(var i=0;i<lines.length;i++){
  var line = lines[i];

  //skip comment lines
  if(line.substr(0,1) == "#" || line.length == 0){
    continue;
  }
  
  line = line.substr(1);
  line = line.substr(0,line.length-1);
  line = JSON.stringify(line);
  line+= ':' + i;
  values.push(line);
}

console.log("{"+values.join(",")+"}");
