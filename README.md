# V8 Hash Collision Generator

## About

This code generates hashes that can be used to test your node.js application against a vulnerabilty to hash collisions attacks. We use the "meet in the middle" method to generate collisions very fast. [This video](https://www.youtube.com/watch?v=R2Cq3CLI6H8) describes how a hash collision attack works 

## Usage

**Use it for good, not evil!**


### 1) compile it
<code>gcc -o generateCollisions generateCollisions.c -O3</code>

### 2) generate hashes
This example generates over 9000 collisions against the hash of "teststr"<br>
<code>./generateCollisions teststr 100000</code>

### 3) test how effectiv it is
This code shows you how to generate hashes and put them into a JSON string. Me measure the time it needs to parse this JSON string again<br>
<pre>
./generateCollisions teststr 100000 > collisions.txt
./jsonify.js collisions.txt > json.txt
ls -lh json.txt # before we start, lets check how big the file is that we have generated
time ./testHashes.js json.txt # this will take a very long time
</pre>
