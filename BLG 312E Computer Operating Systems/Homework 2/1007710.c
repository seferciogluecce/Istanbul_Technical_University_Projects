//Ece Naz Sefercioğlu
//150130140


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>


sem_t full,empty;
int * applicants;
//int reg=4;
int peps=0;
int noOfLine;
int regDur;
int regs=0;

pthread_mutex_t mutex[3]; 
    


void* registerP() {

    while(1) {
		
		 
		
        sem_wait(&empty);
        printf("Applicant %d applied to receptionist.\n", regs+1);
        sleep(regDur);
        printf("Applicant %d's registration is done.\n",regs+1);
		regs++;
        
  		 sem_post(&full);

    sleep(1); 
     if(regs==noOfLine){
		 pthread_exit(NULL);
		 //sem_wait(&empty);
		 break;}
    }
    pthread_exit(NULL);

//return 0;
}

void* interview(void *i) {
    while(1) {
		int m=(intptr_t)i;

		
		sem_wait(&full);
		
        pthread_mutex_lock(&mutex[m]);	      
        	peps++;
        long id=(long)i+1;  
        printf("Interviewer %ld started interview with Applicant %d\n", id,peps);
       int  current=peps;
		sleep(applicants[current-1]);
        printf("Interviewer %ld finished interview with Applicant %d\n", id,current);     
     			pthread_mutex_unlock(&mutex[m]);

		sleep(1);   
			if(current==noOfLine)
			{
				//sem_post(&full);
				pthread_exit(NULL);
				break;
				}
	
				
    }
    pthread_exit(NULL);

//return 0;
}

int main(int argc, char *argv[]) {
    //********************************************************************************************
char * fname=argv[1];
//FILE *myFile = fopen("input.txt","r");
FILE *myFile = fopen(fname,"r");
regDur=atoi(argv[2]);
noOfLine=0;
char cont;
if(myFile==NULL)
printf("error");
while(!feof(myFile)){
	cont=fgetc(myFile);
	if(cont=='\n'){
		noOfLine++;
		}
	
	}
noOfLine--;
applicants=(int*)malloc(noOfLine*sizeof(int));

rewind(myFile);

int  read;
int a;
for(a=0;a<noOfLine;a++){
	
fscanf(myFile,"%d",&applicants[a]);	
}

//***************************************************************************************************
    sem_init(&full,1,0);
    sem_init(&empty,1,noOfLine);
  pthread_mutex_init(&mutex[0],NULL);
  pthread_mutex_init(&mutex[1],NULL);
  pthread_mutex_init(&mutex[2],NULL);
	
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
	
    pthread_t recepcionist, interviewer[3];
    pthread_create(&recepcionist, &attr, registerP,NULL);
    pthread_create(&interviewer[0], &attr, interview, (void*)(intptr_t)0);
    pthread_create(&interviewer[1], &attr, interview, (void*)(intptr_t)1);
    pthread_create(&interviewer[2], &attr, interview, (void*)(intptr_t)2);
   
    pthread_join(interviewer[2], NULL);
    pthread_join(interviewer[1], NULL);   
    pthread_join(interviewer[0], NULL);
    pthread_join(recepcionist, NULL);
  
  
pthread_exit(NULL);




if(regs==noOfLine)
printf("All applicants have been interviewed succesfully\n");
free(applicants);
    return 0;
}
