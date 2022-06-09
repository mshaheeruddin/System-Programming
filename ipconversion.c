#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

int main() {


char inputstr[25];

printf("Enter ip address>>");
scanf("%s", inputstr);

const char del[4] = ".";

char* token;
token = strtok(inputstr, del);

int exp;
exp = 3;
unsigned int sum=0;
while (token != NULL) {

    int itoken;
    itoken = atoi(token);     

   
   sum = sum + itoken*pow(256, exp);
   exp--;
   if (exp == -1) {
     printf("Converted Integer: %u\n",sum); 
    }
   token = strtok(NULL, del);

}

//converting back to dotted string decimal

int r;
r = 0;
unsigned int q;
q = 0;

char ip[25];
q = sum;

int arr[4];
for (int j = 0; j<4;j++){
sum = sum%256;
r = sum;
q = q/256;
sum = q;
arr[3-j] = r;

}

printf("Back to Dotted Decimal String: ");
for (int k = 0;k<4;k++) {


if(k==3) {
   sprintf(ip,"%d\n",arr[k]);
}
     else    {
              sprintf(ip,"%d.",arr[k]); 
                      }
   printf("%s",ip);
}

}
