#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define Max_Out (100)
struct PeoInfomation;
typedef struct PeoInfomation *PeoInfo;
struct PeoInfomation
{
	char name[10];
	int age;
	long int worth;
};
PeoInfo Median3(PeoInfo *A,int Left,int Right);
void ListOut(PeoInfo *Array[],int Num_Out,int Amin,int Amax,int NumAge[],int QueryNum,int j);
int Compare(PeoInfo P1,PeoInfo P2);
void Kth_Element(PeoInfo *A,int Left,int Right,int K);
void Qsort(PeoInfo *A,int Left,int Right);
void Sort(PeoInfo *A,int Left,int Right);
void Swap(PeoInfo *P1,PeoInfo *P2);
int main()
{
	PeoInfo *TmpArray;							
	PeoInfo *Array[201];
	int i, QueryNum, Num_Out, Amin, Amax, tail[201], NumAge[201];
	long int PeoNum, Rear;
	PeoInfo TmpP;

	scanf("%ld%d",&PeoNum,&QueryNum);	/*input the tatal number of people and Queries*/

	for(i=1;i<=200;i++)							
		tail[i]=NumAge[i]=0;			/*Initialize the array tail and NumAge*/

	/*TmpArray Store the pointer pointed to the stucture that stores people infomation */
	TmpArray=(PeoInfo *)malloc(PeoNum*sizeof(PeoInfo));	

	/*Input the information of people */
	for(i=0; i<PeoNum; i++)
	{
		TmpP=(PeoInfo)malloc(sizeof(struct PeoInfomation));
		scanf("%s%d%ld", TmpP->name, &TmpP->age, &TmpP->worth); 
		TmpArray[i]=TmpP;
		NumAge[TmpP->age]++; 					/*Count the numbers of people of same age*/
	}

	/*Allocate for the array of different ages*/
	for(i=1;i<=200;i++)
		Array[i]=(PeoInfo *)malloc(NumAge[i]*sizeof(PeoInfo)); 
	
	/*Divide the people into 200 groups according to the age*/
	for(i=0;i<PeoNum;i++)
	{
		TmpP=TmpArray[i];
		Rear=tail[TmpP->age];
		Array[TmpP->age][Rear]=TmpArray[i];	
		tail[TmpP->age]++;		/*Array tail stores the rear of the Array[Tmp->age]*/
	}

	/*Selete the first K maximum people when the people of same age more than Max_Out*/
	for(i=1;i<=200;i++)						
		if(tail[i]>Max_Out)
			Kth_Element(Array[i],0,tail[i]-1,Max_Out);
		else if(tail[i]>0)		/*Sort the people when the length less than Max_Out*/
			Qsort(Array[i],0,tail[i]-1);
	for(i=1;i<=QueryNum;i++)
	{
		scanf("%d%d%d",&Num_Out,&Amin,&Amax);   /*Input the Maxoutput and the age range*/
		printf("Case #%d:\n",i);
		ListOut(Array,Num_Out,Amin,Amax,NumAge,QueryNum,i);
	}

	return 0;
}

/*Selete the K maximum people frpm the Array[Amin] to Array[Amax]*/
void ListOut(PeoInfo *Array[],int Num_Out,int Amin,int Amax,int NumAge[],int QueryNum,int j)
{
	int i, IndexOfMax,Fornt[200],count=0;
	PeoInfo Ptr;

	for(i=1;i<=200;i++)
		Fornt[i]=0;
	while(count<Num_Out)
	{
		i=Amin;
		while(i<=Amax)
		{
			if(Fornt[i]<NumAge[i])
			{
				IndexOfMax=i;
				break;
			}
			i++;
		}
		if(i==(Amax+1))
		{
			if(count==0)
			{
				printf("None");
				if(j!=QueryNum)			/*Avoid print a blank line at the end of result*/
					printf("\n");
				return ;
			}
			else
				break;
		}
		/*Find the index of maximum*/
		for(i=Amin;i<=Amax;i++)
			if(Fornt[i]<NumAge[i]&&Fornt[IndexOfMax]<NumAge[IndexOfMax])
				if(Compare(Array[i][Fornt[i]],Array[IndexOfMax][Fornt[IndexOfMax]]))
					IndexOfMax=i;

		Ptr=Array[IndexOfMax][Fornt[IndexOfMax]];
		Fornt[IndexOfMax]++;
		printf("%s %d %ld",Ptr->name,Ptr->age,Ptr->worth);
		if(j<QueryNum||count+1<Num_Out)	/*Avoid print a blank line at the end of result*/
			printf("\n");
		count++;
	}
}

/*Return median of Left, Center and Right as the pivot*/
/*Order these and hide the pivot*/
PeoInfo Median3(PeoInfo *A,int Left,int Right)
{
	int Center=(Left+Right)/2;

	if(!Compare(A[Left],A[Center]))
		Swap(&A[Left],&A[Center]);
	if(!Compare(A[Left],A[Right]))
		Swap(&A[Left],&A[Right]);
	if(!Compare(A[Center],A[Right]))
		Swap(&A[Center],&A[Right]);

	/*Invariant: A[Left]>A[Center]>A[Right]*/

	Swap(&A[Center],&A[Right-1]);	/*Hide pivot*/
	return A[Right-1];				/*return pivot*/
}

/*Selete the first K maximun people*/
void Kth_Element(PeoInfo *A,int Left,int Right,int K)
{
	int i, j;
	PeoInfo Pivot;
	if(Left+3<=Right)
	{
		Pivot=Median3(A,Left,Right);
		i=Left;
		j=Right-1;
		for( ; ; )
		{
			while(Compare(A[++i],Pivot)){}
			while(!Compare(A[--j],Pivot)){}
			if(i<j)
				Swap(&A[i],&A[j]);
			else
				break;
		}

		Swap(&A[i],&A[Right-1]);	/*Restore the pivot*/
		if(i-Left+1==K)
			return ;
		else if(i-Left+1<K)       					
			Kth_Element(A,i+1,Right,(K+Left-i-1));
		else
			Kth_Element(A,Left,i,K);
	}
	else
		Sort(A,Left,Right);
}

void Qsort(PeoInfo *A,int Left,int Right)
{
	int i, j;
	PeoInfo Pivot;
	if(Left+3<=Right)
	{
		Pivot=Median3(A,Left,Right);
		i=Left;
		j=Right-1;
		for( ; ; )
		{
			while(Compare(A[++i],Pivot)){}
			while(!Compare(A[--j],Pivot)){}
			if(i<j)
				Swap(&A[i],&A[j]);
			else
				break;
		}

		Swap(&A[i],&A[Right-1]);			/*Restore the pivot*/

		Qsort(A,Left,i-1);
		Qsort(A,i+1,Right);
	}
	else		/*Sort the subarray*/									
		Sort(A,Left,Right);
}

/*Sort the array whose legth is less than 3*/
void Sort(PeoInfo *A,int Left,int Right)
{
	int Center;
	Center=(Left+Right)/2;

	if(!Compare(A[Left],A[Center]))
		Swap(&A[Left],&A[Center]);
	if(!Compare(A[Left],A[Right]))
		Swap(&A[Left],&A[Right]);
	if(!Compare(A[Center],A[Right]))
		Swap(&A[Center],&A[Right]);
}

/*Prioritize the one of two people according to their worth, ages and names*/
int Compare(PeoInfo P1,PeoInfo P2)
{
	if((P1->worth)>(P2->worth))
		return 1;
	else if((P1->worth)<(P2->worth))
		return 0;
	else
	{
		if((P1->age)<(P2->age))
			return 1;
		else if((P1->age)>(P2->age))
			return 0;
		else
		{
			if(strcmp(P1->name,P2->name)>=0)
				return 0;
			else
				return 1;
		}
	}

}

void Swap(PeoInfo *P1,PeoInfo *P2)
{
	PeoInfo TmpP;

	TmpP=*P1;
	*P1=*P2;
	*P2=TmpP;
}
