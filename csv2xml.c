#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

char array[1024*sizeof(int)];
int column = 1, row = 1;

//for argumans
char *inputfile ="", *outputfile="";
char *arg_seperator="0", *arg_opsys="0";

//for split
char *opsys_pointer="", *seperator_pointer="";
//for number of column and row
char selected_opsys=' ', selected_seperator=' ';


void defineSelections(char *arg_seperator, char *arg_opsys); // to define seperators and opsys with argv[]
void defineRowandColumn(FILE *filePointer); //to define number of rows and columns
void splitTable(char *list_of_elements[][column]); //to split the array table with seperators
void writeToXML(char *list_of_elements[][column],char *outputfile); // to design a XML file
char* spaceCntrl(char *string_element); //this control actually for \r\n 


void main(int argc, char* argv[]){

      if(argc ==1){
            //when the code run wit CodeRunner
            printf("\n----------%s---------\n",argv[0]);
            printf("\nYou are expected to enter 4 arguments on the terminal to start the program:\n");
            printf("\n1- The name of an input file in .csv format");
            printf("\n2- Name of output file in .xml format");
            printf("\n3- Separator used in input file. You must enter a number! \n\t1 -> comma(,) \n\t2 -> tab(\\t) \n\t3 -> semicolon(;)");
            printf("\n4- The operating system where the input file will be used. You must enter a number! \n\t1 -> 'windows' \n\t2 -> 'linux' \n\t3 -> 'macos'");
            printf("\n\nThe format should be like: CVS2XML inputfile.csv outputfile.xml 1 1\n");
    }  
    else if(strcmp(argv[1],"-h")==0){
        printf("\n----------%s---------\n",argv[0]);
        printf("\nYou are expected to enter 4 arguments to start the program:");
        printf("\n1- The name of an input file in .csv format");
        printf("\n2- Name of output file in .xml format");
        printf("\n3- Separator used in input file. You must enter a number! \n\t1 -> comma(,) \n\t2 -> tab(\\t) \n\t3 -> semicolon(;)");
        printf("\n4- The operating system where the input file will be used. You must enter a number! \n\t1 -> 'windows' \n\t2 -> 'linux' \n\t3 -> 'macos'");
        printf("\n\nThe format should be like: CVS2XML inputfile.csv outputfile.xml 1 1\n");
    }
    else if(argc==7){ 
        if(strcmp(argv[3],"-seperator")==0){        
            if(strcmp(argv[5],"-opsys")==0){

                printf("\n----------%s---------\n",argv[0]);
                inputfile=argv[1];
                outputfile=argv[2];
                arg_seperator=argv[4];
                arg_opsys=argv[6];

                defineSelections(arg_seperator, arg_opsys);

                FILE *filePointer;
                filePointer = fopen(inputfile,"r");

                defineRowandColumn(filePointer);   
                
                fclose(filePointer);
            } 
            else{
            printf("please check your arguments!\n");
            }
        }
        else{
            printf("please check your arguments!\n");
        }
        
   }
   else printf("please check your arguments!\n");
   

}

void defineSelections(char *arg_seperator, char *arg_opsys){
    //seperator_pointer and opsys_pointer are defined in this function.
    //bcs. they are necessary for strsep

    if(strcmp(arg_seperator,"1")==0){//,
        selected_seperator = ',';
        seperator_pointer=",";
   }
    else if(strcmp(arg_seperator,"2")==0){//\t
        selected_seperator = '\t';
        seperator_pointer="\t";
    }
    else if(strcmp(arg_seperator,"3")==0){//;
        selected_seperator = ';';
        seperator_pointer=";";
    }


    if(strcmp(arg_opsys,"1")==0){ //windows \n
        //\r operatörünü kaldırıyorum. 
        //Zaten ikisi de her satırda var linux veya macos kontrolü gibi tek biriyle devam edebiliyorum.
        selected_opsys = '\n';
        opsys_pointer = "\n";
    }
    else if(strcmp(arg_opsys,"2")==0){ // linux \n
        selected_opsys = '\n';
        opsys_pointer = "\n";
    }
    else if(strcmp(arg_opsys,"3")==0){ //macos \r
        selected_opsys = '\r';
        opsys_pointer = "\r";
    }

}

void defineRowandColumn(FILE *filePointer){
    char ch;
    int c=0;
    if(filePointer == NULL){
        //printf("file is not available\n");
    }
    else{
    //fill array char by char with .csv file
    ch=fgetc(filePointer);
        while(ch!= EOF){
            array[c] = ch;

            //define number of rows
            //selected_opsys can be \n or \r
            if(ch==selected_opsys) row++;
            c++;
        ch=fgetc(filePointer);
        }   
    }

    for (int i = 0; i < c; i++)
    {
        //define number of columns
        //selected_seperator can be , ; or \t
        if(array[i] == selected_seperator)
            column++;
        if(array[i] == selected_opsys )break;
    }
    
    //this is necessary because csv file was design on windows
    //so, both \n and \r exists at the end of the lines
   if(strcmp(arg_opsys,"1")==0 || strcmp(arg_opsys,"2")==0){ //linux and windows
        for (int i = 0; i < c; i++)
        {
            if(array[i]=='\r'){
                array[i] = ' ';
            }
        } 
    }
    else if(strcmp(arg_opsys,"3")==0){ //mac
        for (int i = 0; i < c; i++)
        {   
            if(array[i]=='\n'){
                array[i] = ' ';
            }
        } 
    }
    
    //desinged two dimensional array like table
    char *list_of_elements[row][column];
    splitTable(list_of_elements);

}

void splitTable(char *list_of_elements[][column]){
    row=0;
    column=0;

    char *split_file = malloc(1000*sizeof(int));
    char *arrayPointer = malloc(1000*sizeof(int));
    char *split_row =  malloc(1000*sizeof(int));

    strcat(arrayPointer,array);
    
    //split first with line characters then seperators
    while((split_file = strsep(&arrayPointer,opsys_pointer)) !=NULL){
        if(split_file[0] == ' '){
            for (size_t i = 0; i < sizeof(split_file)-1; i++)
            {
                split_file[i] = split_file[i+1];
            }
            
        }
        if(split_file[0]=='\0'){
            split_file = NULL;
        }
        if(split_file!=NULL){
            column=0;
            while((split_row = strsep(&split_file,seperator_pointer)) != NULL){
                list_of_elements[row][column] = split_row;
                column++;
            }
            row++;
        }
    }
           
    free(split_file);
    free(arrayPointer);
    free(split_row);

    writeToXML(list_of_elements,outputfile);
}

void writeToXML(char *list_of_elements[][column], char *outputfile){
  // printf("\nrow: %d  column: %d",row,column);
    FILE *fp;
    fp = fopen(outputfile, "w");
    char *child[column]; //to keep column headers


    //this part to hold column headers
    for (int j = 0; j < column; j++){
        child[j] =list_of_elements[0][j];         
    }
    
    //to clean the space
    child[column-1] = spaceCntrl(child[column-1]);


    //to lowercase and space of column headers
    //First Name ---> first_name
    for (size_t i = 0; i < column; i++)
    {
        int j=0;
        while(child[i][j]!='\0'){
            if(child[i][j] == ' '){
                child[i][j]='_';
                 
            }
            else if(child[i][j] >= 'A' && child[i][j] <= 'Z'){
                child[i][j] = child[i][j] +32;
            }
            j++;
        }
    }

   //to design xml format and write on xml file
    printf("<%s>\n",inputfile);
    fprintf(fp,"<%s>\n",inputfile);
        for (int i = 1; i < row; i++)
        {
            printf("\t<row id=\"%d\">",i);
            fprintf(fp,"\t<row id=\"%d\">",i);
            for (int j = 0; j < column; j++)
            {    
                list_of_elements[i][j] = spaceCntrl(list_of_elements[i][j]);
                if(list_of_elements[i][j][0] == '\0'){
                    printf("\n\t\t<%s/>",child[j]);
                    fprintf(fp,"\n\t\t<%s/>",child[j]);
                }
                else{
                    printf("\n\t\t<%s>%s</%s>",child[j],list_of_elements[i][j],child[j]);
                    fprintf(fp,"\n\t\t<%s>%s</%s>",child[j],list_of_elements[i][j],child[j]);
                }
                
            }
           printf("\n\t</row>\n");
           fprintf(fp,"\n\t</row>\n");        
        }
    printf("</%s>",inputfile);
    fprintf(fp,"</%s>",inputfile);   

    fclose(fp);
}

char* spaceCntrl(char *string_element){
    //this part for clean the spaces in the last column
    if(string_element[strlen(string_element)-1] == ' '){
        string_element[strlen(string_element)-1] = '\0';
    }
    return string_element;
}



