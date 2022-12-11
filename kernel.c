//Omar and Alex worked on Step 0 to Step 6 equally 


void printChar(char);
void printString(char*);
void readString(char*);
void readSector(char*,int);
void handleInterrupt21(int ax, int bx, int cx, int dx);
void readFile(char*, char*, char*);
void executeProgram(char*);
void terminate();
void writeSector(char*, char*);
void deleteFile(char*);
void writeFile(char*, char*, int);
void makeTimerInterrupt();
void handleTimerInterrupt(int segment, int sp);
void returnFromTimer(int segment, int sp);
void initializeProgram(int segment); // step 3


int processActive[8];//step 2 
int processStackPointer[8];//step 2
int currentProcess;//step 2, keep track of whats running currently

//Thus, given a table entry, you can find the segment by adding two and multiplying by 0x1000.

//print string cant be used to debug now in kernel, becuase its inaccesible when it goes to kernel, when we call it back it is still inaccesable nov 10:
//use printChar instead this works because its only putting a value not an adress for that value
void main() {
    /*char line[80];
    char buffer[13312];
    int sectorsRead;
    printString("what file: \0");
    readString(line);
    printString(line);
    readSector(buffer, 30);
    printString(buffer);
    readFile("messag", buffer,&sectorsRead);
    printString(buffer);
    makeInterrupt21();
    //interrupt(0x21, 4, "tstpr1", 0, 0);
    //interrupt(0x21, 4, "tstpr2", 0, 0);
    
    
    handleInterrupt21(0x21, 0,0,0,0);
    interrupt(0x21,0,line,0,0);
    interrupt(0x21,1,line,0,0);
    //printString("Hello \0");
    interrupt(0x21,3,"messag",buffer, &sectorsRead);  // read the file into buffer
    if(sectorsRead>0)
      interrupt(0x21,0,buffer,0,0);  //print out the file
    else
        interrupt(0x21, 0,"messag not found in this sector\r\n",0,0);
*/
    makeInterrupt21();
    processActive=0;
    processStackPointer=0xff00;//where stacks start
    currentProcess=-1;//no user processes yet, so set to -1
    makeTimerInterrupt();
    //interrupt(0x21,8,"this is a test message","testmg",3);
    interrupt(0x21,4,"shell",0,0);
    while(1);

}



void printChar( char letter){
     interrupt(0x10,0xe*256+letter,0,0,0);
  }
void printString(char *letter){
      char al = '\n';
       while (*letter != '\0'){
          al = *letter;
         interrupt(0x10,0x0e*256+al,0,0,0);
         letter++;
    }
}

void readString(char* line){
    int i = 0;
      char temp = 0;
      while (1)
      {
        /* the AH must equal 0 and AL doesn't matter */
            temp = interrupt (0x16, 0, 0, 0, 0);
            
            /* only if the input is "ENTER", the loop will end*/
            if (temp == 0xd)
                  break;

            /* if input is backspace, it will only diplay not store and index will decrement minus one */
            if (temp == 0x8) //0x8 being backspace button
            {
                  if (i > 0) //if it isnt the first to sort index of 0
                  {
                        interrupt (0x10, 0x0e * 256 + temp, 0, 0, 0);
                        i--;//decrement index position in array
                        interrupt (0x10, 0x0e * 256 + 0x20, 0, 0, 0); //prints space
                        interrupt (0x10, 0x0e * 256 + temp, 0, 0, 0); //prints backspace
               
                  }
                  else
                        continue; //if i was at 0 dont do anything pressed keep going
            }

            /* for any other charcters normal*/
            else
            {
                  line[i++] = temp;
                  interrupt (0x10, 0x0e * 256 + temp, 0, 0, 0);   
            }
      }

      /* remeber to set the last two characters as 0xa and 0x0 */
      if (i > line - 2)
            i = line - 2;
      line[i++] = 0xa; //next line
      line[i] = 0x0; //null terminator
   
      return;
}

void readSector(char* buffer, int sector){
    int AX = 2*256+1;
    int CX = 0*256+sector+1;
    int DX = 0*256+0x80; 
    interrupt(0x13,AX,buffer,CX,DX);
}

void handleInterrupt21(int ax, int bx, int cx, int dx){

    //printString("Hello World");
    if(ax == 0){
    printString(bx);

    }
    else if(ax ==1){
        printString("Enter File:");
        readString(bx);
    }
    else if(ax ==2){
        readSector(bx,cx);
    }
    else if(ax==3) {

        readFile(bx, cx, dx);
    }
    else if(ax==4){
        executeProgram(bx);
    }
    else if(ax==5){
        terminate();
    }
    else if( ax==6){
    	writeSector(bx,cx);
    }

    else if( ax==7){

   	deleteFile(bx);
    }

   else if(ax==8){
  	writeFile(bx,cx,dx);
    }
   else{

    printString("An error occurred when ax>8"); // terminating the prgram
    }
    
}


void readFile( char*filename, char*buffer){
    int *sectorsred;
    char dir [512];
    int fileentry;
    int sector;
    int foundfile;
    int entry;
    int i;


     readSector(dir,2);
   for(entry=0; entry<512; entry+=32){
       foundfile=1;
      for(fileentry=0; fileentry<6; fileentry++){

             //   printChar(dir[fileentry+entry]);

           //  printChar(filename[fileentry]);
      if(filename[fileentry]!=dir[fileentry+entry]){
        foundfile=0;
      //   entry+=6;
        //printChar(fileentry);
       break;
         }
      }
     if(foundfile==1){

break;

}
   }
//handle match=0, set to 0 and ret
//handle match=1 case
       if(foundfile==0){
     *sectorsred=0;
      return;
}
       for(i=6; i<26; i++){
          sector=dir[entry+i];
        if(sector==0)
      break;
    readSector(buffer, sector);
    buffer=buffer+512;
   *sectorsred=*sectorsred+1;
   }

}

//void executeProgram(char*name){
 //      char buffer[13312];
  //      int i=0;
    //    int segment =0x2000;
      //     readFile(name,buffer);
        //while(i<13312){
     	  //  putInMemory(segment,i,buffer[i]);
    	    //i++;
	
  //}
  	//step 3
    //launchProgram(segment);
    

}

void executeProgram(char* name){ //new executeProgram
    readFile(name,buffer);//step 1: read file into buffer
    //step through processAvtive array looking for free entry
    //determine segment,take entry add 2 *0x1000
    putInMemory(segment,i,buffer[i]);//copy buffer into segment with putInMemory
    intitializeProgram(segment);//call init program
    processActive=1;//set process active to 1 step 6
    processStackPointer=0xff00; //set stack pointer to 0xff00: step 6
    
    
}
void terminate(){

	char shellname[6];
	shellname[0]= 's';
	shellname[1] = 'h';
	shellname[2]='e';
	shellname[3]='l';
	shellname[4]='l';
	shellname[5]='\0';
	executeProgram(shellname);
    
}    
void writeSector(char* buffer, int sector){
   int AX = 3*256 +1;
   int CX = 0*256+sector+1;
   int DX = 0*256+0x80;
   interrupt(0x13,AX,buffer,CX,DX);
 }


void deleteFile(char* filename){
   char dir[512];
   char map[512];
   int i =0;
   dir[0] = '\0';  
    for (i=0; i<13312; i++){
      dir[0]='\0';
      if(filename[i]!=dir[0]){ 
       	map[i] = 0;              
      }
    }
}
 void writeFile(char* buffer, char* filename, int numberOfSectors){
          char dir[512];
          char map[512];
          int i=0;
          int k = 0;
          int y = 0;
          int count =0;
          int empty = 0;

         readSector(dir, 2);     // read on the directory
         readSector(map, 1);     // map

       for(i=0; i<512; i=i+32){  // for each iteration  junp 32 times
        if(dir[i]==0){
          dir[i] = filename[0];
          dir[i+1] = filename[1];
          dir[i+2] = filename[2];
          dir[i+3] = filename[3];
          dir[i+4] = filename[4];
          dir[i+5] = filename[5];
            break;
     }
  }
        for(y=0; y<numberOfSectors; y++){
              for(count=3;  k!=0; count++){
               k= map[count];
                empty = count;
     }

            map[empty] = 0xff;
            dir[i+6+y] = empty;
    }

          writeSector(dir, 2);
          writeSector(map, 1);
          
       
}   


void handleTimerInterrupt(int segment, int sp){
	//printChar('T');
	//printChar('I');
	//printChar('C');
    dataseg=setKernelDataSegment
	returnFromTimer(segment,sp);
}




