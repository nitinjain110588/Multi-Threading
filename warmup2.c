#include <math.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include "my402list.h"
#include "my402list.c"
#include <sys/stat.h>
#include <pthread.h>
#include <signal.h>


typedef struct packet
{
	int packet_no;
    	int token_req;
    	float lambda_rate;
    	float mu_rate;
    	long int e_Q1;
    	long int l_Q1;
    	long int e_Q2;
     
}packet;

long int My402Time()
{
    	long int time;
    	struct timeval time1;
    	gettimeofday(&time1,0);
    	time=time1.tv_sec*1000000 + time1.tv_usec;
    	return(time);
}
int token_count=0,flag=0,flag1=0,flag2=0,flag3=0,flag5=0,den=0,den3=0;
float mu,lambda,r;
int B,P,num;
My402List *ancGP,*ancSP;
FILE* fp;
int packet_count=1,i,k=0,q=0,u=0,check=0,check3=0,check4=0,check5=0,total_token_produced=0,dropped_token=0;
int total_packet_produced=0,dropped_packet=0;
packet *obj,*nit1,*temp,*nit2,*nit3,*nit4,*nit5,*nit6,*nit7,*nit8,*nit9;
My402ListElem *last,*first,*first_Q1,*first_Q3,*f_Q1,*fi_Q1,*last_eQ1,*las_Q2,*la_Q2;
long int start,previous,interarrival,tok_arr_time,enterQ1,leftQ1,Emulation_time;
long int sleep_chk1,sleep_chk2,sleep_chk3,start_service,serviced;
long int avg_service_time=0,avg_nop_Q1=0,avg_nop_Q2=0,avg_nop_S=0,avg_time_in_sys=0;
long int avg_inter_arrival=0;
long int arrival,enterQ2,time_in_Q1;
double sd=0,den4=0,den5=0,prob_packet,prob_token,sd1=0,sd2=0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;

void *Arrival()
{
    
    
	int lam,muu;
    	char *pch,buf[100];
    	previous=0;
    	fprintf(stdout,"\n%012.3fms: emulation begins",(double)(start-start));

    	if(flag1 == 1)
    	{    

        	if(fgets(buf,sizeof(buf),fp) != NULL)    
        	{
            		pch=strtok(buf," \t");
            		num=atoi(pch);
        	}
        
    	}

    while(1)
    {
    
    k=k+1;
    if(k == num+1)
    {
        flag2=1;
        pthread_exit(0);
    }

    last=(My402ListElem*)malloc(sizeof(My402ListElem));
    first_Q1=(My402ListElem*)malloc(sizeof(My402ListElem));
    f_Q1=(My402ListElem*)malloc(sizeof(My402ListElem));
    last_eQ1=(My402ListElem*)malloc(sizeof(My402ListElem));
    las_Q2=(My402ListElem*)malloc(sizeof(My402ListElem));
    obj=(packet *)malloc(sizeof(packet));
    temp=(packet *)malloc(sizeof(packet));
    nit5=(packet *)malloc(sizeof(packet));
    nit7=(packet *)malloc(sizeof(packet));
    nit8=(packet *)malloc(sizeof(packet));

    
    if(flag1 == 1)
    {

        if(fgets(buf,sizeof(buf),fp) != NULL)
        {
            pch=strtok(buf," \t");
           
            lam=atoi(pch);
            lambda=(float)1000/lam;
           
            pch=strtok(NULL," \t");
            P=atoi(pch);
           
            pch=strtok(NULL," \t");
            muu=atoi(pch);
            mu=(float)1000/muu;
        }
    }

    
    sleep_chk1=My402Time();
    sleep_chk1=sleep_chk1-start;
        
    if(k == 1)
    {        
        usleep(1000000/lambda);
    }
    
    else
    {
        if((sleep_chk1 - previous) > ((1000000/lambda)))
        {
            
        }
        else
        {
            usleep(((1000000/lambda)) - ((sleep_chk1 - previous)));
        }
    }
    if(flag == 1)
    {
        pthread_exit(0);
    }
    
    arrival=My402Time();
    arrival=arrival-start;
    interarrival=arrival-previous;
    previous=arrival;
    avg_inter_arrival=avg_inter_arrival + interarrival;
    check=check+1;
        
    if(P <= B)
    {
        if(P > 1)
        {
           
            fprintf(stdout,"\n%012.3fms: p%d arrives,needs %d tokens, inter-arrival time = %0.3fms",(double)arrival/1000,k,P,(double)interarrival/1000);
        }
        else
        {
           
fprintf(stdout,"\n%012.3fms: p%d arrives,needs %d token, inter-arrival time = %0.3fms",(double)arrival/1000,k,P,(double)interarrival/1000);
        }
    
    
    obj->packet_no = packet_count;
    packet_count=packet_count+1;
    obj->token_req=P;
    obj->lambda_rate=lambda;
    obj->mu_rate=mu;
    
    pthread_mutex_lock(&m);
    
    if(My402ListAppend(ancGP,obj) == FALSE)
    {
        fprintf(stderr,"Error appending to list:%d",errno);
    }
    enterQ1=My402Time();
    enterQ1=enterQ1-start;
    last_eQ1=My402ListLast(ancGP);
    nit8=(packet *)last_eQ1->obj;
    nit8->e_Q1=enterQ1;
    fprintf(stdout,"\n%012.3fms: p%d enters Q1",(double)enterQ1/1000,k);

    if(P > token_count)
    {
           
    }    
    else
    {
        f_Q1=My402ListFirst(ancGP);
        nit5=(packet *)f_Q1->obj;
        q=nit5->packet_no;
        enterQ1=nit5->e_Q1;
        token_count=token_count - P;

        first_Q1=My402ListFirst(ancGP);
        temp=(packet *)first_Q1->obj;

        My402ListUnlink(ancGP,first_Q1);
        leftQ1=My402Time();
        leftQ1=leftQ1-start;
        time_in_Q1=leftQ1 - enterQ1;
        
        if(token_count < 2)
        {
        fprintf(stdout,"\n%012.3fms: p%d leaves Q1, time in Q1 = %0.3fms, token bucket now has %d token",(double)leftQ1/1000,q,(double)time_in_Q1/1000,token_count);
        }
        else
        {
        fprintf(stdout,"\n%012.3fms: p%d leaves Q1, time in Q1 = %0.3fms, token bucket now has %d tokens",(double)leftQ1/1000,q,(double)time_in_Q1/1000,token_count);
        }
        avg_nop_Q1=avg_nop_Q1 + (leftQ1-enterQ1);
        check4=check4+1;
        if(My402ListAppend(ancSP,temp) == FALSE)
        {
            fprintf(stderr,"Error appending to list:%d",errno);
        }
    	las_Q2=My402ListLast(ancSP);
    	nit7=(packet *)las_Q2->obj;
    	nit7->l_Q1=leftQ1;
        enterQ2=My402Time();
        enterQ2=enterQ2-start;
        nit7->e_Q2=enterQ2;
        fprintf(stdout,"\n%012.3fms: p%d enters Q2",(double)enterQ2/1000,q);
       
       
    }
    }

    else
    {
       
        fprintf(stdout,"\n%012.3fms: packet p%d arrives,needs %d tokens, dropped",(double)arrival/1000,k,P);
        packet_count=packet_count+1;
        dropped_packet=dropped_packet+1;
    
    }
    

    if(My402ListEmpty(ancSP) == FALSE)
    {
        pthread_cond_signal(&cv);
    }

    pthread_mutex_unlock(&m);
    
    }
    
    return (0);
}

void *Bucket()
{
    int l=0;
    while(1)
    {
    l=l+1;
    
    first_Q1=(My402ListElem*)malloc(sizeof(My402ListElem));
    fi_Q1=(My402ListElem*)malloc(sizeof(My402ListElem));
    la_Q2=(My402ListElem*)malloc(sizeof(My402ListElem));
    nit2=(packet *)malloc(sizeof(packet));
    nit1=(packet *)malloc(sizeof(packet));
    nit6=(packet *)malloc(sizeof(packet));
    nit9=(packet *)malloc(sizeof(packet));
    sleep_chk2=My402Time();
    sleep_chk2=sleep_chk2-start;
    
    if(l == 1)
    {
        usleep(1000000/r);
    }
    
    else
    {
        if((sleep_chk2 - sleep_chk3) > (1000000/r))
        {
            
        }
        else
        {
            usleep((1000000/r) - ((sleep_chk2 - sleep_chk3)));
        }
    }
    sleep_chk3=My402Time();
    sleep_chk3=sleep_chk3-start;
    
    pthread_mutex_lock(&m);
    if((flag2 == 1) && (ancGP->num_members == 0))
    {
        pthread_mutex_unlock(&m);
        pthread_exit(0);
    }
    total_token_produced=total_token_produced+1;
    if(flag == 1)
    {
	
        pthread_mutex_unlock(&m);
        pthread_exit(0);
    }
    if(token_count < B)
    {
    token_count=token_count+1;

    tok_arr_time=My402Time();    
    tok_arr_time=tok_arr_time - start;

    if(token_count == 1)
    {
    fprintf(stdout,"\n%012.3fms: token t%d arrives, token bucket now has %d token",(double)tok_arr_time/1000,l,token_count);
    }
    else
    fprintf(stdout,"\n%012.3fms: token t%d arrives, token bucket now has %d tokens",(double)tok_arr_time/1000,l,token_count);
    }

    else
    {
        tok_arr_time=My402Time();
        tok_arr_time=tok_arr_time - start;
        fprintf(stdout,"\n%012.3fms: token t%d arrives, dropped",(double)tok_arr_time/1000,l);
        dropped_token=dropped_token+1;        
    }
    
    if(ancGP->num_members > 0)
    {
        first_Q1=My402ListFirst(ancGP);
        nit1=(packet *)first_Q1->obj;
        P=nit1->token_req;

        if(P <= token_count)
        {
            fi_Q1=My402ListFirst(ancGP);
            nit6=(packet *)fi_Q1->obj;
            q=nit6->packet_no;
            enterQ1=nit6->e_Q1;
            token_count=token_count - P;

            nit2=(packet *)first_Q1->obj;
            My402ListUnlink(ancGP,first_Q1);
            leftQ1=My402Time();
            leftQ1=leftQ1-start;
           
            if(token_count < 2)
            {
            fprintf(stdout,"\n%012.3fms: p%d leaves Q1, time in Q1 = %0.3fms, token bucket now has %d token",(double)leftQ1/1000,q,(double)(leftQ1 - enterQ1)/1000,token_count);
            }
            else
            {
            fprintf(stdout,"\n%012.3fms: p%d leaves Q1, time in Q1 = %0.3fms, token bucket now has %d tokens",(double)leftQ1/1000,q,(double)(leftQ1 - enterQ1)/1000,token_count);
            }
            avg_nop_Q1=avg_nop_Q1 + (leftQ1-enterQ1);
            check5=check5+1;
            if(My402ListAppend(ancSP,nit2) == FALSE)
            {
                fprintf(stderr,"Error appending to list:%d",errno);
            }
        
	    la_Q2=My402ListLast(ancSP);
            nit9=(packet *)la_Q2->obj;
            nit9->l_Q1=leftQ1;
            enterQ2=My402Time();
            enterQ2=enterQ2-start;
            nit9->e_Q2=enterQ2;
            fprintf(stdout,"\n%012.3fms: p%d enters Q2",(double)enterQ2/1000,q);
            if(My402ListEmpty(ancSP) == FALSE)
            {
                pthread_cond_signal(&cv);
            }
           

        }
    }
    pthread_mutex_unlock(&m);
    
    }
    
    return(0);
}
void *Server()
{
    
    if(flag5 == 1)
    {
        pthread_exit(0);
    }
    while(1)
    {
    
    nit3=(packet *)malloc(sizeof(packet));
    nit4=(packet *)malloc(sizeof(packet));
    My402ListElem *first_Q2;
    first_Q2=(My402ListElem *)malloc(sizeof(My402ListElem));
    
    pthread_mutex_lock(&m);
    
    while(ancSP->num_members == 0)
    {
        pthread_cond_wait(&cv,&m);
	if(flag == 1)
	{
		flag3=1;
		pthread_mutex_unlock(&m);
		pthread_exit(0);
	}
    }
    
    first_Q2=My402ListFirst(ancSP);
    nit3=(packet *)first_Q2->obj;
    mu=nit3->mu_rate;
    u=nit3->packet_no;
   
    My402ListUnlink(ancSP,first_Q2);
    pthread_mutex_unlock(&m);

    start_service=My402Time();
    start_service=start_service-start;
    fprintf(stdout,"\n%012.3fms: p%d begin service at S, time in Q2 = %0.3fms",(double)start_service/1000,u,(double)(start_service-nit3->e_Q2)/1000);
    
    usleep(1000000/mu);
    
    serviced=My402Time();
    serviced=serviced-start;
    
    fprintf(stdout,"\n%012.3fms: p%d departs from S, service time = %0.3fms, time in system = %0.3fms",(double)serviced/1000,u,(double)(serviced-start_service)/1000,(double)(serviced-nit3->e_Q1)/1000);
    
    avg_service_time=avg_service_time + (serviced-start_service);
    avg_nop_S=avg_service_time;
    avg_nop_Q2=avg_nop_Q2 + (start_service-nit3->e_Q2);
    avg_time_in_sys=avg_time_in_sys + (serviced - nit3->e_Q1);
    sd1=sd1+(((double)(serviced - nit3->e_Q1)/1000000)*((double)(serviced - nit3->e_Q1)/1000000));
    sd2=sd2+(double)(serviced - nit3->e_Q1)/1000000;
    check3=check3+1;

    if((u == num) || (flag == 1))
    {
        flag3=1;
        pthread_exit(0);
    }
    }

}
int main(int argc,char *argv[])
{
    pthread_t Athread,Tthread,Sthread;
    int error,w;
    char *f_name;
    ancGP=(My402List*)malloc(sizeof(My402List));
    ancSP=(My402List*)malloc(sizeof(My402List));
    
    My402ListInit(ancGP);
    My402ListInit(ancSP);
    
    lambda=0.5; mu=0.35; r=1.5; B=10; P=3; num=20;
    
    for(w=1;w<argc;w++)
    {
      
        if(strcmp(argv[w],"-lambda") == 0)
        {
            lambda=atof(argv[w+1]);
        }
        if(strcmp(argv[w],"-mu") == 0)
        {
            mu=atof(argv[w+1]);
        }
        if(strcmp(argv[w],"-r") == 0)
        {
            r=atof(argv[w+1]);
        }
        if(strcmp(argv[w],"-B") == 0)
        {
            B=atoi(argv[w+1]);
        }
        if(strcmp(argv[w],"-P") == 0)
        {
            P=atoi(argv[w+1]);
        }
        if(strcmp(argv[w],"-n") == 0)
        {
            num=atoi(argv[w+1]);
        }
        if(strcmp(argv[w],"-t") == 0)
        {
                   
            fp = fopen(argv[w+1],"r");
            f_name=argv[w+1];
            if(fp == NULL)
            {
                perror("\nCannot open file");
                exit(1);
            }

            flag1=1;
        }
       
    }
        if((1/lambda) > 10)
        {
            lambda=0.1;
        }
        if((1/mu) > 10)
        {
            mu=0.1;
        }
        if(B > 2147483647)
        {
            fprintf(stderr,"\n Value of B cannot exceed 2147483647");
            exit(1);
        }
        if(P > 2147483647)
        {
            fprintf(stderr,"\n Value of P cannot exceed 2147483647");
            exit(1);
        }
        if(num > 2147483647)
        {
            fprintf(stderr,"\n Value of num cannot exceed 2147483647");
            exit(1);
        }    
        if(lambda < 0)
        {
            fprintf(stderr,"\n Value of lambda cannot be negative");
            exit(1);
        }
        if(mu < 0)
        {
            fprintf(stderr,"\n Value of mu cannot be negative");
            exit(1);
        }
        if(r < 0)
        {
            fprintf(stderr,"\n Value of r cannot be negative");
            exit(1);
        }
        fprintf(stdout,"\nEmulation Parameters:");
        if(flag1 == 1)
        {
	    fprintf(stdout,"\n    r = %g",r);
            fprintf(stdout,"\n    B = %d",B);            
            fprintf(stdout,"\n    tsfile = %s",f_name);
        }
        else
        {
            fprintf(stdout,"\n    lambda = %g",lambda);
            fprintf(stdout,"\n    mu = %g",mu);
            fprintf(stdout,"\n    r = %g",r);
            fprintf(stdout,"\n    B = %d",B);
            fprintf(stdout,"\n    P = %d",P);
            fprintf(stdout,"\n    number to arrive = %d",num);
            if(P > B)
            {
            	flag5=1;
            }
       }

    start=My402Time();
    if((error=pthread_create(&Athread,0,Arrival,0)))
    {
        fprintf(stderr,"pthread_create: %s",strerror(error));
        exit(1);
    }
    if((error=pthread_create(&Tthread,0,Bucket,0)))
    {
        fprintf(stderr,"pthread_create: %s",strerror(error));
        exit(1);
    }
    if((error=pthread_create(&Sthread,0,Server,0)))
    {
        fprintf(stderr,"pthread_create: %s",strerror(error));
        exit(1);
    }
    void handler(int);
    sigset(SIGINT,handler);
 
    pthread_join(Athread,0);
    pthread_join(Tthread,0);
    pthread_join(Sthread,0);  
    Emulation_time=My402Time();
    Emulation_time=Emulation_time-start; 
    den=check*1000000;
    den3=check3*1000000;
    prob_token=dropped_token/total_token_produced;
    prob_packet=dropped_packet/k;
    sd1=sd1/check3;
    sd2=sd2/check3;
    sd2=sd2*sd2;
    sd=sd1-sd2;
    sd=sqrt(sd); 
    if(flag == 0)
    {
	fprintf(stdout,"\n    average packet inter-arrival time = %0.6g",(double)avg_inter_arrival/den);
    	fprintf(stdout,"\n    average packet service time = %0.6g\n",(double)avg_service_time/den3);
    	fprintf(stdout,"\n    average number of packets in Q1 = %0.6g",(double)avg_nop_Q1/Emulation_time);
    	fprintf(stdout,"\n    average number of packets in Q2 = %0.6g",(double)avg_nop_Q2/Emulation_time);
    	fprintf(stdout,"\n    average number of packets at S = %0.6g\n",(double)avg_nop_S/Emulation_time);
    	fprintf(stdout,"\n    average time a packet spent in system = %0.6g",(double)avg_time_in_sys/den3);
    	fprintf(stdout,"\n    standard deviation for time spent in system = %0.6g\n",sd);
    	fprintf(stdout,"\n    token drop probability = %0.6g",prob_token);
    	fprintf(stdout,"\n    packet drop probability = %0.6g",prob_packet); 
    }
return (0);
}
void handler(int signo)
{
    Emulation_time=My402Time();
    Emulation_time=Emulation_time-start;
    
    flag=1;
    pthread_cond_signal(&cv);
    while((flag3 == 0) && (flag5 == 0))
    {
    }
    den=check*1000000;
    den3=check3*1000000;
    prob_token=dropped_token/total_token_produced;
    prob_packet=dropped_packet/k;
    sd1=sd1/check3;
    sd2=sd2/check3;
    sd2=sd2*sd2;
    sd=sd1-sd2;
    sd=sqrt(sd);

    fprintf(stdout,"\nStatistics:\n");
    if(dropped_packet == (packet_count-1))
    {
    fprintf(stdout,"\n    average packet inter-arrival time = N/A, no packet arrived at the facility");
    fprintf(stdout,"\n    average packet service time = N/A, no packet arrived at the facility");
    fprintf(stdout,"\n    average number of packets in Q1 = N/A, no packet arrived at the facility");
    fprintf(stdout,"\n    average number of packets in Q2 =  N/A, no packet arrived at the facility");
    fprintf(stdout,"\n    average number of packets at S = N/A, no packet arrived at the facility");
    fprintf(stdout,"\n    average time a packet spent in system =  N/A, no packet arrived at the facility");
    fprintf(stdout,"\n    standard deviation for time spent in system =  N/A, no packet arrived at the facility");
    fprintf(stdout,"\n    token drop probability = %0.6g",prob_token);
    fprintf(stdout,"\n    packet drop probability = %0.6g",prob_packet);    
    }
    else if((check4 == 0) && (check5 == 0))
    {
    fprintf(stdout,"\n    average packet inter-arrival time = %0.6g",(double)avg_inter_arrival/den);
    fprintf(stdout,"\n    average packet service time = N/A, no packet arrived at the facility");
    fprintf(stdout,"\n    average number of packets in Q1 = N/A, packet remained in Q1");
    fprintf(stdout,"\n    average number of packets in Q2 =  N/A, no packet arrived at the facility");
    fprintf(stdout,"\n    average number of packets at S = N/A, no packet arrived at the facility");
    fprintf(stdout,"\n    average time a packet spent in system =  N/A, packet remained in Q1");
    fprintf(stdout,"\n    standard deviation for time spent in system =  N/A, packet remained in Q1");
    fprintf(stdout,"\n    token drop probability = %0.6g",prob_token);
    fprintf(stdout,"\n    packet drop probability = %0.6g",prob_packet);    
    }
    else
    {
    fprintf(stdout,"\n    average packet inter-arrival time = %0.6g",(double)avg_inter_arrival/den);
    fprintf(stdout,"\n    average packet service time = %0.6g\n",(double)avg_service_time/den3);
    fprintf(stdout,"\n    average number of packets in Q1 = %0.6g",(double)avg_nop_Q1/Emulation_time);
    fprintf(stdout,"\n    average number of packets in Q2 = %0.6g",(double)avg_nop_Q2/Emulation_time);
    fprintf(stdout,"\n    average number of packets at S = %0.6g\n",(double)avg_nop_S/Emulation_time);
    fprintf(stdout,"\n    average time a packet spent in system = %0.6g",(double)avg_time_in_sys/den3);
    fprintf(stdout,"\n    standard deviation for time spent in system = %0.6g\n",sd);
    fprintf(stdout,"\n    token drop probability = %0.6g",prob_token);
    fprintf(stdout,"\n    packet drop probability = %0.6g",prob_packet);  
    }    
}
