void enableInterrupts();

void main(){
    char buffer[13312];
    char line[80];
    char dir[512];
    char map[512];
    char filename[20];
    char command[20];
    int sectorReader;
    int y;
    int i;
    int k;
    int j=1;
    int count=0;
    char copyFrom[20];
    char copyTo[20];
    char file[100];
    
    
    while(1){
        syscall(0, "shell>");
        syscall(1, line);
       
        if (line[0] == 't' && line[1] == 'y' && line[2] == 'p' && line[3] == 'e'){
            syscall(3, line + 5, buffer);
            if (buffer[0] == '\0'){
                syscall(0, " file not found\r\n"); // the file does not exist
            } 
            
            else  {
                syscall(0, buffer);
                buffer[0] = '\0';
        } }
        
        
        else if (line[0] == 'e' && line[1] == 'x' && line[2] == 'e' && line[3] == 'c'){
            syscall(4, line+5);
          }  
        
        else if (line[0] == 'k' && line[1] == 'i' && line[2] == 'l' && line[3] == 'l'){
       	    syscall(9, line+5);
       	    }
        
        else if (line[0] == 'd' && line[1] == 'i' && line[2] == 'r') {
 
            syscall(2, dir, 2);
            for (y = 0; y < 512; y+=32){
                if (dir[y] == '\0'){
                    return;
                } else {
                    for (i = 0; i < 6; i++){
                        if (filename[i] =! '\0'){ 
                            filename[i] = dir[y+i];
                        }
                           else {
                            break;
                        }

                    }
                    syscall(0, filename);
                    syscall(0, "\r\n");
                }
          }

        }
        
        else if(line[0]=='d' && line[1]=='e' && line[2]=='l'){
          syscall(3,command, buffer, &sectorReader);
          syscall(2, dir, 2);
          syscall(2,map,1);
          if(sectorReader){
            syscall(0,"File was deleted");
         }
         for(k=0; k<512; k=k+32){
           if(dir[k]!=0){
                    if(command[0]==dir[k] && command[1]==dir[k+1] && command[2]== dir[k+2]
                       && command[3]== dir[k+3] && command[4] ==dir[k+4] && command[5]==dir[k+5]){
                    dir[k] ='\0';
                    syscall(0, &dir[k]);
                    syscall(6, dir,2);
                  for(count = 0; j!='\0'; count++){

                  j=dir[k+6+count];

                  map[j]='\0';

                }
                syscall(7, map,1);
           }


           }
         }
        }
        else if(line[0]=='c' && line[1]=='o' && line[2]=='p' && line[3]=='y'){
        	int marker =5;
        	int i =0;
       while (line[marker] !=' '){
        copyFrom[i] = line[marker];
       i++;
      marker++;

      }
      i++;
      marker++;
     while(line[marker] !='\0'){
     copyTo[i] = line[marker];

     i++;
    marker++;
   }
    syscall(3, copyFrom, buffer, sectorReader);
    if(buffer[0] =='\0'){
    syscall(0, "could no locate file to copy\r\n");
  }
     else{
   syscall(8, copyTo, sectorReader);

     }

        } else if(line[0]=='t' && line[1]=='e' && line[2]=='x' && line[3]=='t' && line[4]=='f' && line[5]=='l'){
           syscall(0, "File Created\n\r");


    

     } else {
            syscall(0, "Cannot be found\r\n");
          }
        }
     }
