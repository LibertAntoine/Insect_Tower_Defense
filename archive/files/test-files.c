#include <stdio.h> 
#include <string.h> 
#include <dirent.h> 

int main(void) 
{ 
  struct dirent *de;  // Pointer for directory entry 

  // opendir() returns a pointer of DIR type.  
  DIR *dr = opendir("level"); 

  if (dr == NULL)  // opendir returns NULL if couldn't open directory 
  { 
    printf("Could not open current directory" ); 
    return 0; 
  } 

  // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html 
  // for readdir() 
  while ((de = readdir(dr)) != NULL) 
    if (strstr(de->d_name, ".itd")) {
      printf("%s\n", de->d_name); 
    }

  closedir(dr);     
  return 0; 
} 
