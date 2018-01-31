#include <stdio.h>
#include <time.h>
#define Max 10000                                       /*define the max size of the sequence*/

clock_t start1,stop1,start2,stop2;                      /*clock_t is a built-in type for processor time (ticks)*/
double duration1,duration2;             			



/*iterative function*/
void iterative_function(int data[], int num)
{
	int j;
	for(j=0; j<num; j++)                                /*for-loop*/
	{
		printf("%d\n",data[j]);                          /*print the sequence in the given order*/
	}
}


/*recursive function*/
void recursive_function(int a[], int left, int right)
{
	int middle = (left + right) / 2;
	if(left < right)
	{
		recursive_function(a, left, middle);            /*print the left parts of the sequence*/
		printf("%d\n",a[middle]);                       /*print the middle number*/
		recursive_function(a, middle+1, right);         /*print the right parts of the sequence*/
	}
}

/*main function*/
int main()
{
	int num;
	int data[Max];
	int K;
	int i,j,k;
	clock_t start1,stop1,start2,stop2;


	FILE *fp;
	if((fp=fopen("10.in","r"))==NULL)       /*ensure the file is not empty*/ 
	{
		printf("File open error!\n");
		exit(0);
	}

	fscanf(fp,"%d",&num);                    /*read in first line of file as the size of the list*/
	fscanf(fp,"%*[^\n]%*c");                 /*jump to the second line to start data input*/



	for(i=0;i<num;i++)
	{
		fscanf(fp,"%d",&data[i]);            /*get the data from the file*/
	}


	fclose(fp);

	data[i]='\0';
	/*for(j=0;j<i;j++)
		printf("%d\n",data[j]);*/

	printf("Enter how many times you want to run the function\n");
	scanf("%d",&K);

	start1 = clock();                       /*begin time counting*/

	for(k=0;k<K;k++)
		recursive_function(data,0,num);

	stop1 = clock();                       /*stop time counting*/


	duration1 = ((double)(stop1-start1))/CLK_TCK;   /* CLK_TCK is a built-in constant = ticks per second */
	
	
	start2 = clock();                       /*begin time counting*/
	
	for(j=0;j<K;j++)
	{
		iterative_function(data,num);
		printf("\n");
	}
	
	stop2 = clock();                       /*stop time counting*/
	
	duration2 = ((double)(stop2-start2))/CLK_TCK;    /* CLK_TCK is a built-in constant = ticks per second */
	
	
	printf("----------------------------------------\n");
	printf("As for recursive method:\n");
	printf("Iterations (K):%d\n",K);	   /*print K,ticks,total time and duration*/
	printf("Ticks=%.1lf\n",(double)(stop1-start1));
	printf("Total time=%lf\n",duration1);
	printf("Duration=%lf\n",duration1/K); 
	
	printf("----------------------------------------\n");
	printf("As for iterative method:\n");
	printf("Iterations (K):%d\n",K);	   /*print K,ticks,total time and duration*/
	printf("Ticks=%.1lf\n",(double)(stop2-start2));
	printf("Total time=%lf\n",duration2);
	printf("Duration=%lf\n",duration2/K); 
}