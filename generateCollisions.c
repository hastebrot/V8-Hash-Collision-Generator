#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define DICT_LENGTH 200000

struct dictonary{
  char strings[DICT_LENGTH][4];
  uint32_t hashes[DICT_LENGTH];
};

//original v8 hash function
uint32_t v8hash_forward(char *str, uint32_t length){
  uint32_t hash = 0, i=0;
  
  for(;i<length;i++){
    hash += str[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }
  
  return hash;
}

//debug helper
void dec2bin(char *desc, long decimal)
{
  int  k = 0, n = 0;
  int  neg_flag = 0;
  int  remain;
  int  old_decimal;  // for test
  char temp[80];
  char binary[80];
  long untouchedDecimal = decimal;

  // take care of negative input
  if (decimal < 0)
  {      
    decimal = -decimal;
    neg_flag = 1;
  }
  do 
  {
    old_decimal = decimal;   // for test
    remain    = decimal % 2;
    // whittle down the decimal number
    decimal   = decimal / 2;
    // converts digit 0 or 1 to character '0' or '1'
    temp[k++] = remain + '0';
  } while (decimal > 0);

  if (neg_flag)
    temp[k++] = '-';       // add - sign
  else
    temp[k++] = ' ';       // space

  // reverse the spelling
  while (k >= 0)
    binary[n++] = temp[--k];

  binary[n-1] = 0;         // end with NULL
  
  printf("%s = %s - %lld\n", desc, binary, untouchedDecimal);
}

//first part of the v8 hash function, forward and backward
uint32_t forwardhash_1(char c, uint32_t hash){
  hash += c;
  
  return hash;
}
uint32_t backwardhash_1(char c, uint32_t hash){
  hash -= c;
  
  return hash;
}

//second part of the v8 hash function, forward and backward
uint32_t forwardhash_2(char c, uint32_t hash){
  hash += (hash << 10);
  
  return hash;
}
uint32_t backwardhash_2(char c, uint32_t hash){  
  return hash * 3222273025;
}

//third part of the v8 hash function, forward and backward
uint32_t forwardhash_3(char c, uint32_t hash){
  hash ^= (hash >> 6);
  
  return hash;
}
uint32_t backwardhash_3(char character, uint32_t hash){
  uint32_t part1, part2, part3, part4, part5, part6;
  
  part1 = hash >> 26 << 26;
  part2 = (hash ^ (part1 >> 6)) >> 20 << 26 >> 6;
  part3 = (hash ^ (part2 >> 6)) >> 14 << 26 >> 12;
  part4 = (hash ^ (part3 >> 6)) >> 8 << 26 >> 18;
  part5 = (hash ^ (part4 >> 6)) >> 2 << 26 >> 24;
  part6 = (hash ^ (part5 >> 6)) << 30 >> 30;
  hash = part1 + part2 + part3 + part4 + part5 + part6;
  
  return hash;
}

//v8 hash function backwards
uint32_t v8hash_backward(char *str, uint32_t length, uint32_t hash){
  int i=0;
  char c;
  
  for(i=length-1;i>=0;i--){
    c = str[i];
    hash = backwardhash_3(c,hash);
    hash = backwardhash_2(c,hash);
    hash = backwardhash_1(c,hash);
  }
  
  return hash;
}

void testerror_1(int part, unsigned long i, char c, uint32_t hashed, uint32_t unhashed){
  printf("error at part %d, i = %u, c = %u\n", part, i,c);
  dec2bin("hashed",hashed);
  dec2bin("unhashed",unhashed);
  exit(1);
}

void testerror_2(char *str, uint32_t hashed, uint32_t unhashed){
  printf("error at string '%s'", str);
  dec2bin("hashed",hashed);
  dec2bin("unhashed",unhashed);
  exit(1);
}

//lets test
void test(void){
  unsigned long i;
  char c=103;
  uint32_t hashed, unhashed;
  char *str1="abc", *str2="1pj",*str3="8wn";
  
  for(i=0;i<=UINT32_MAX;i++){
    //part 1
    hashed = forwardhash_1(c,(uint32_t)i);
    unhashed = backwardhash_1(c,hashed);
  
    if(unhashed != i){
      testerror_1(1,i,c,hashed,unhashed);
    }
    
    //part 2
    hashed = forwardhash_2(c,(uint32_t)i);
    unhashed = backwardhash_2(c,hashed);
  
    if(unhashed != i){
      testerror_1(2,i,c,hashed,unhashed);
    }
    
    //part 3
    hashed = forwardhash_3(c,(uint32_t)i);
    unhashed = backwardhash_3(c,hashed);
  
    if(unhashed != i){
      testerror_1(3,i,c,hashed,unhashed);
    }
  }
  
  //str 1
  hashed = v8hash_forward(str1,3);
  unhashed = v8hash_backward(str1,3,hashed);
  if(unhashed != 0){
    testerror_2(str1, hashed, unhashed);
  }

  //str 2
  hashed = v8hash_forward(str2,3);
  unhashed = v8hash_backward(str2,3,hashed);
  if(unhashed != 0){
    testerror_2(str2, hashed, unhashed);
  }
  
  //str 3
  hashed = v8hash_forward(str3,3);
  unhashed = v8hash_backward(str3,3,hashed);
  if(unhashed != 0){
    testerror_2(str3, hashed, unhashed);
  }
}

struct dictonary createDictonary(uint32_t targetHash){
  struct dictonary dict;
  
  char a,b,c, tempstr[4];
  uint32_t backwardHash, i=0, arrayInd;
  
  //init rand
  srand ( time(NULL) );
  tempstr[3]='\0';
  
  //set all hashes to 0
  for(i=0;i<DICT_LENGTH;i++){
    dict.hashes[i]=0;
    dict.strings[i][0]='\0';
  }
  
  for(i=0;i<DICT_LENGTH;i++){
    a=(char)(rand()%94+32);
    b=(char)(rand()%94+32);
    c=(char)(rand()%94+32);
    
    tempstr[0]=a;
    tempstr[1]=b;
    tempstr[2]=c;
    
    backwardHash = v8hash_backward(tempstr,3,targetHash);
    
    arrayInd = backwardHash % DICT_LENGTH;
    
    dict.hashes[arrayInd]=backwardHash;
    strncpy(dict.strings[arrayInd],tempstr,3);
  }
  
  return dict;
}

void crack(char *str, int length, int maxLength, uint32_t basisHash, struct dictonary *dict, int *amount){
  char newStr[20];
  int i=0, newLength = length+1;
  uint32_t hash, arrayInd;
  
  //copy the string and add a null
  strncpy(newStr, str, length);
  newStr[newLength] = '\0';
  
  for(i=32;i<127;i++){
    // add a new character 
    newStr[length] = i;
    
    // hash recursive 
    hash = basisHash;
    hash += i;
    hash += (hash << 10);
    hash ^= (hash >> 6);
    
    // recursive 
    if(newLength < maxLength){
      crack(newStr, newLength, maxLength, hash, dict, amount);
    } else {
      //thats the end of the recursion, lets check if we have found a collision
      arrayInd = hash % DICT_LENGTH;
      
      if((*dict).hashes[arrayInd] == hash){      
        printf("'%s%s'\n",newStr,(*dict).strings[arrayInd]);
        fflush(stdout);
        
        *amount=*amount-1;
        if(*amount <= 0){
          fprintf(stderr, "done");
          exit(0);
        }
      }
    }
  }
}

int main(int argc, char *argv[])
{
  uint32_t targetHash;
  struct dictonary dict;
  int amount;
  
  //this is a test run
  if(argc==2 && strcmp (argv[1], "--test")==0){
    test();
  } 
  //this is a normal run
  else if(argc==3){
    //try to read the amount
    sscanf(argv[2],"%d",&amount);    
    if(amount == 0){
      fprintf(stderr, "Was not able to read the parameter AMOUNT\n");
      exit(1);
    }
    
    targetHash = v8hash_forward(argv[1],strlen(argv[1]));
    dict = createDictonary(targetHash);
    crack("", 0, 7, 0, &dict, &amount);
  }
  else {
    fprintf(stderr, "Usage: %s COLLISIONSTRING AMOUNT\n", argv[0]);
    exit(1);
  }
  
  return 0;
}
