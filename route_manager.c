/** @file route_manager.c
 *  @brief A pipes & filters program that uses conditionals, loops, and string processing tools in C to process airline routes.
 *  @author Felipe R.
 *  @author Hausi M.
 *  @author Jose O.
 *  @author Saasha J.
 *  @author Victoria L.
 *  @author Anthea Blais
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * Function: main
 * --------------
 * @brief The main function and entry point of the program.
 * @param argc The number of arguments passed to the program.
 * @param argv The list of arguments passed to the program.
 * @return int 0: No errors; 1: Errors produced.
 *
 */



//struct used to store each element from file row
struct Routes{
    char airline[100];
    char airline_icao[100];
    char airline_country[100];
    char src_airport[100];
    char src_airport_city[100];
    char src_airport_country[100];
    char src_airport_icao[100];
    char src_altitude[100];
    char dest_airport[100];
    char dest_airport_city[100];
    char dest_airport_country[100];
    char dest_airport_icao[100];
    char dest_altitude[100];
};

//function prototypes
void openFile(char[100],char[100],char[100],char[100]);
void getCommandLineInput(int argc, char*argv[]);
void edgeCase(FILE *file);
void output(char[100],char[100],char[100],char[100], struct Routes test, FILE *fileOpen, int);        //char[100],char[100],char[100],char[100],char[100],char[100]);

void openFile(char temp2[100], char temp3[100], char temp4[100], char temp5[100]){
    FILE *fp = fopen("airline-routes-data.csv","r");  //fopen returns a file pointer which gets stored in fp
    char row[1024]; //row buffer
    int index = 0; //used for printing first line into the output file
    int lines = 1; //used for the edge case (when no lines match input so there are no routes)
    struct Routes route;

    FILE *file;
    file = fopen("output.txt", "w");

    if(fp == NULL || file == NULL){
        printf("file cannot be opened \n");
        exit(1);        //return 1 to signal to shell something went wrong in program
    }

    //while loop to grab each row in the file
    while(fgets(row, 500, fp) != NULL){ 
        
        memset(&route, 0, sizeof(route)); //clears the struct in case of blank fields

        //puts each word into correct stuct field using a tokenizer which divides based on commas
        char *token = strtok(row, ",");       
        if(token != NULL){
            strcpy(route.airline,token);  
        }
        token = strtok(NULL, ",");
        if(token != NULL){
            strcpy(route.airline_icao,token);
        }
        token = strtok(NULL, ",");
        if(token != NULL){
            strcpy(route.airline_country,token);
        }
        token = strtok(NULL, ",");
        if(token != NULL){
            strcpy(route.src_airport,token);
        }
        token = strtok(NULL, ",");
        if(token != NULL){
            strcpy(route.src_airport_city,token);
        }
        token = strtok(NULL, ",");
        if(token != NULL){
            strcpy(route.src_airport_country,token);
        }
        token = strtok(NULL, ",");
        if(token != NULL){
            strcpy(route.src_airport_icao,token);
        }
        token = strtok(NULL, ",");
        if(token != NULL){
            strcpy(route.src_altitude,token);
        }
        token = strtok(NULL, ",");
        if(token != NULL){
            strcpy(route.dest_airport,token);
        }
        token = strtok(NULL, ",");
        if(token != NULL){
            strcpy(route.dest_airport_city,token);
        }
        token = strtok(NULL, ",");
        if(token != NULL){
            strcpy(route.dest_airport_country,token);
        }
        token = strtok(NULL, ",");
        if(token != NULL){
            strcpy(route.dest_airport_icao,token);
        }
        token = strtok(NULL, ",");
        if(token != NULL){
            strcpy(route.dest_altitude,token);
        }
        
        //compares command line input to corresponding struct field
        //case 1
        if(strcmp(route.airline_icao,temp2) == 0 && strcmp(route.dest_airport_country,temp3) == 0){
           
           //sends the matches to be outputted in the output file
            output(temp2,temp3,NULL,NULL,route,file,index);
            index++; //increases index by 1 so first line in output file is not repeated
        }
        //case 2
        else if (strcmp(route.src_airport_country,temp2)==0 &&strcmp(route.dest_airport_city,temp3) ==0&& strcmp(route.dest_airport_country,temp4) ==0){
           
            output(temp2,temp3,temp4,NULL,route,file,index);
            index++;
        }
        //case 3
        else if(strcmp(route.src_airport_city,temp2) == 0 && strcmp(route.src_airport_country,temp3) == 0 && strcmp(route.dest_airport_city,temp4) == 0 && strcmp(route.dest_airport_country,temp5) ==0){

            output(temp2,temp3,temp4,temp5,route,file,index);
            index++;
        }
        
        // case if there is no match 
        else{
            lines++; //counts each time a linedoes match match input
            if(lines >= 67663){ 
                edgeCase(file); //will output since no matches were found
            }
            
            
        }
        
    }

    //closes both the output and csv file
    fclose(file);
    fclose(fp);
}

void edgeCase(FILE *file){
    file = fopen("output.txt", "w");
    fputs("NO RESULTS FOUND.\n", file); 
    fclose(file);
}

void output(char temp2[100],char temp3[100],char temp4[100],char temp5[100], struct Routes test, FILE 
*fileOpen,int count){

    //checks for case to send tio output file
    if(temp4 == NULL && temp5 == NULL){
        if(count == 0){     //so the very first line in file is not repeated
            fprintf(fileOpen,"FLIGHTS TO %s BY %s (%s):\n",temp3,test.airline,temp2);
        }
        //prints to the output file
        fprintf(fileOpen,"FROM: %s, %s, %s TO: %s (%s), %s\n",test.src_airport_icao,test.src_airport_city,test.src_airport_country,test.dest_airport,test.dest_airport_icao,test.dest_airport_city);
    }
    else if(temp4 != NULL && temp5 == NULL){
        if (count == 0){
                fprintf(fileOpen,"FLIGHTS FROM %s TO %s, %s:\n", temp2,temp3,temp4);
            }
        fprintf(fileOpen,"AIRLINE: %s (%s) ORIGIN: %s (%s), %s\n",test.airline, test.airline_icao,test.src_airport, test.src_airport_icao, test.src_airport_city);

    }
    else if(temp5 != NULL){
        if(count == 0){
                fprintf(fileOpen,"FLIGHTS FROM %s, %s TO %s, %s:\n",temp2,temp3,temp4,temp5);
        }
        fprintf(fileOpen,"AIRLINE: %s (%s) ROUTE: %s-%s\n", test.airline, test.airline_icao, test.src_airport_icao, test.dest_airport_icao);
    }

}

void getCommandLineInput(int argc, char *argv[]){
    //argument variables 
    char arg2[100];
    char arg3[100];
    char arg4[100];
    char arg5[100];

   //check the number of arguments coming in from command line to check each case
    if(argc <= 1){
        printf("NO RESULTS FOUND.\n"); //if no airline data was supplied from command line
    }
    else if(argc == 4){
        sscanf(argv[2]+10, "%[^\t\n]",arg2);
        sscanf(argv[3]+15, "%[^\t\n]",arg3);
        openFile(arg2,arg3,NULL,NULL);      //send the input over to next function
    }

    else if(argc == 5){
        sscanf(argv[2]+14, "%[^\t\n]",arg2);
        sscanf(argv[3]+12, "%[^\t\n]",arg3);
        sscanf(argv[4]+15, "%[^\t\n]",arg4);
        openFile(arg2,arg3,arg4,NULL);
    }
    else if(argc == 6){
        sscanf(argv[2]+11, "%[^\t\n]",arg2);
        sscanf(argv[3]+14, "%[^\t\n]",arg3);
        sscanf(argv[4]+12, "%[^\t\n]",arg4);
        sscanf(argv[5]+15, "%[^\t\n]",arg5);
        openFile(arg2,arg3,arg4,arg5);
    }
}

int main(int argc, char *argv[])
{
    //sends command line input to first function
    getCommandLineInput(argc,argv);

    exit(0);

}