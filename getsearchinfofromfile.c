#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
int num =0;
char word[2000];
char string[50];
char student[100] = {0};

while(student[0]!= '0')
{
FILE *infile;
infile = fopen("/home/mshaheeruddin/Desktop/addinfo.txt",­ "r");
if (infile == NULL)
{
printf("Error file missing\n");
exit(-1);
}

printf("please enter a system name: \n");
scanf("%s", student);
while ( fscanf(infile,"%s",­ string) == 1)
{
//Add a for loop till strstr(string, student) does-not returns null.
if(strstr(string, student)!=0) {//if match found
num++;
}
}
printf("we found the word %s in the file %d times\n",student,num­);
num = 0;
fclose(infile);
}
return 0;
}
