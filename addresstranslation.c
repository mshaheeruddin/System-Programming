#include <stdio.h>
#include <stdlib.h>

int main()
{
char str[10];
FILE *fptr;

if ((fptr = fopen("/home/mshaheeruddin/Desktop/addinfo.txt","r")) == NULL){
printf("Error! opening file");

// Program exits if the file pointer returns NULL.
exit(1);
}
char c;

fscanf(fptr,"%s", &str);

printf("System name =%s", str );
fclose(fptr);

return 0;
}
