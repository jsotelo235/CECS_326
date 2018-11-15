/* structured_file.c */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int CarFile;

struct CarEntry
{
  char Name[24];
  float costToRepair;
  int timeToRepair;
};


/*
Write empty records into the file.
numRecords is the number of records to write.
*/
void clearFile(int numRecords);

/*
Write the record into the file at the location indicated.
0 is the first record in the file.
where should be less than the numRecords use to initialize the file,
but no check is required (to simplify the assignment).
Caution, you have a pointer to the record to write here.
*/
void writeRecord(struct CarEntry *record,int where);

/*
Read a record from the file from the location indicated.
0 is the first record in the file.
where should be less than the numRecords use to initialize the file,
but no check is required (to simplify the assignment).
Caution, you have a pointer to the record to fill in here.
*/
void readRecord(struct CarEntry *record,int where);

int main()
{
  int whereTo;
  int numRecordsToClear;
  char user_input[64];
  struct CarEntry CarData;
  
  /* set up the student file */
  CarFile = open("cardata.bin",O_RDWR|O_CREAT,S_IRUSR | S_IWUSR);
 
  //printf("Size of carEntry: %d\n", sizeof(struct CarEntry));

 /* 3 records */
  while (1)
  {
    printf("a)clear file, b)write record, c)read record, d) enter Data, q)quit\n");
    fgets(user_input,sizeof(user_input)-1,stdin);

    switch (user_input[0])
    {
      case 'a':
	printf("Clear file: how many blank records should I write? ");
        fgets(user_input,sizeof(user_input)-1,stdin);
        numRecordsToClear = atoi(user_input);
        clearFile(numRecordsToClear);
        printf("\n");
        break;
      case 'b':
        printf("Current Data is: %s cost %f time %d\n",CarData.Name,
        CarData.costToRepair,CarData.timeToRepair);
        printf("Where should I write the data? ");
        fgets(user_input,sizeof(user_input)-1,stdin);
        whereTo = atoi(user_input);
        writeRecord(&CarData,whereTo);
        printf("\n");
        break;
      case 'c':
        printf("From where should I read the data? ");
        fgets(user_input,sizeof(user_input)-1,stdin);
        whereTo = atoi(user_input);
        readRecord(&CarData,whereTo);
        printf("Current Data is now: %s cost %f time %d\n",CarData.Name,
          CarData.costToRepair,CarData.timeToRepair);
        printf("\n");
        break;
      case 'd':
        printf("Name>");
        fgets(user_input,sizeof(user_input),stdin);
        /* Remove the \n */
        user_input[strlen(user_input)-1] = '\0';
        strcpy(CarData.Name,user_input);
        printf("cost>");
        fgets(user_input,sizeof(user_input),stdin);
        CarData.costToRepair = (float) atof(user_input);
        printf("time>");
        fgets(user_input,sizeof(user_input),stdin);
        CarData.timeToRepair = atoi(user_input);

        printf("Current Data is now: %s cost %f time %d\n",CarData.Name,
          CarData.costToRepair,CarData.timeToRepair);
        printf("\n");
        break;
      case 'q':
        close(CarFile);
        printf("File closed, Bye\n");
        return 0;
        break;
      default:
        printf("Unrecognized option\n");
        printf("\n");
        break;
    }
  }
  return 0;
};


/*
Write empty records into the file.
numRecords is the number of records to write.
*/
void clearFile(int numRecords)
{ 
  char buffer[numRecords*sizeof(struct CarEntry)];

  for(int i = 0; i < numRecords*sizeof(struct CarEntry); i++)
  {
    buffer[i] = '\0';
  }

  lseek(CarFile, 0, SEEK_SET);
  write(CarFile, buffer, sizeof(struct CarEntry)*numRecords);
}

void writeRecord(struct CarEntry *record,int where)
{
  if(lseek(CarFile, where*sizeof(struct CarEntry), SEEK_SET) < 0)
  {
    perror("lseek failed");
  }
  write(CarFile, record, sizeof(struct CarEntry));
}

void readRecord(struct CarEntry *record,int where)
{
  if(lseek(CarFile, where*sizeof(struct CarEntry), SEEK_SET) < 0)
  {
    perror("lseek failed");
  }
  
  read(CarFile, record, sizeof(struct CarEntry));
  
  printf ("Name: %s  Cost to repair: %f  Time to repair: %d\n",
           record->Name, record->costToRepair, record->timeToRepair);
}
