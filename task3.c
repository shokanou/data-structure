#include <stdio.h>
#include <stdlib.h>
#define Error( Str )        FatalError( Str )
#define FatalError( Str )   fprintf( stderr, "%s\n", Str ), exit( 1 )
#define MinData (-32767)

// An aov handles this problem well. All the numbers are stored in the aov in the same order as ther are stored in the hash table.
// The nagetive numbers in the hash table are also copied into the voa, but they are ignored when graph is preformed.
// The successors of a vertex are stored in a linked list.

typedef struct Successor{
	int index;
	struct Successor* next;
} Successor;

typedef struct Vertex{
	int value;
	int pre_num;
	Successor* suc;
} Vertex;

typedef struct GraphRecord{
	Vertex *array;
	int size;
} * Graph;

Graph CreatGraph(int n){    // allocate memory to the graph
	if(n<=0)
		return NULL;
	Graph G;
	int i;
	G=(Graph)malloc(sizeof(struct GraphRecord));
	if(G==NULL)
		FatalError( "Out of space!!!" );
	G->size=0;  // set size 0
	G->array=(Vertex*)malloc(n*sizeof(Vertex));
	if(G->array==NULL)
		FatalError( "Out of space!!!" );
	for(i=0;i<n;i++){
		(G->array[i]).suc=NULL;     // every vertex has no successor
		(G->array[i]).pre_num=0;    // every vertex has no predecessor
	}
	return G;
}

void AddVertex(Graph G,int val){
	(G->array[G->size++]).value=val;    // store the value into a vertex and size++
}

void AddEdge(Graph G,int pre,int suc){
	Successor *tmp,*last;
	tmp=(Successor*)malloc(sizeof(Successor));  // creat the successor record
	tmp->index=suc;
	tmp->next=NULL;
	if((G->array[pre]).suc==NULL)
		(G->array[pre]).suc=tmp;
	else{
		for(last=(G->array[pre]).suc;last->next!=NULL;last=last->next);     // found the tail in the linked list
		last->next=tmp;     // point the tail to the new successor record
	}
	(G->array[suc]).pre_num++;  // suc's number of predecessor plus one
}
int NoPredecessor(Graph G,int position){    // check if a vertex has no predecessor. If so, return 1, and 0 otherwise.
	if((G->array[position]).pre_num==0)
		return 1;
	else
		return 0;
}

int Value(Graph G,int position){    // return the value of the vertex in the array[position]
	return (G->array[position]).value;
}
int Find(Graph G,int Vertex){   // give the value of a vertex, return it's index
	int index;
	index=Vertex%G->size;
	while(Value(G,index)!=Vertex){
		index++;
		if(index>=G->size)
			index-=G->size;
	}
	return index;
}

// ruturn a non-visited successor of the given vertex.If all the successor are visited, return -1.
int NextSuccessor(Graph G,int Vertex){
	static int val=-1;  // static val is used to store the vertex passed last time
	static int index;   // static index is used to store the index of vertex passed last time
	static Successor* now;  // static now is pointed to the successor which is supposed to return this time
	int suc;
	if(Vertex!=val){    // If the vertex is different with the vertex passed last time, initialize all the static variables
		val=Vertex;
		index=Find(G,Vertex);
		now=(G->array[index]).suc;
	}
	if(now==NULL)   // if now is NULL, there is no successor to return
		return -1;
	else{
		suc=now->index;     // store the value that is supposed to return
		now=now->next;  // point now to the next successor in the linked list
		return suc;
	}
}

void DestroyGraph(Graph G){     // destruct the graph
	int i;
	Successor *next,*now;
	if(G!=NULL){
		if(G->array!=NULL){
			for (i = 0; i < G->size; ++i){
				next=(G->array[i]).suc;
				while(next!=NULL){
					now=next;
					next=next->next;
					free(now);
				}
			}
			free(G->array);
		}
		free(G);
	}
}

typedef int ElementType;

// This a standard definition of min heap and associated function, including Initialize, Insert, DeleteMin, Destroy, and etc.
// Almost all the codes are identical to the ones on the textbook.
typedef struct HeapStruct
    {
        int Capacity;
        int Size;
        ElementType *Elements;
    } *PriorityQueue;

PriorityQueue
Initialize( int MaxElements ){
    PriorityQueue H;
    H =(PriorityQueue)malloc( sizeof( struct HeapStruct ) );
    if( H ==NULL )
        FatalError( "Out of space!!!" );

    // Allocate the array plus one extra for sentinel
    H->Elements = (ElementType*)malloc( ( MaxElements + 1 )* sizeof( ElementType ) );
    if( H->Elements == NULL )
        FatalError( "Out of space!!!" );
        H->Capacity = MaxElements;
        H->Size = 0;
        H->Elements[ 0 ] = MinData;
        return H;
    }

void MakeEmpty( PriorityQueue H ){
            H->Size = 0;
        }

//H->Element[ 0 ] is a sentinel

void Insert( ElementType X, PriorityQueue H ){
    int i;
    if( IsFull( H ) ){
        Error( "Priority queue is full" );
        return;
    }
    for( i = ++H->Size; H->Elements[ i / 2 ] > X; i /= 2 )
        H->Elements[ i ] = H->Elements[ i / 2 ];
        H->Elements[ i ] = X;
}

ElementType DeleteMin( PriorityQueue H ){
    int i, Child;
    ElementType MinElement, LastElement;
    if( IsEmpty( H ) ){
        Error( "Priority queue is empty" );
        return H->Elements[ 0 ];
    }
    MinElement = H->Elements[ 1 ];
    LastElement = H->Elements[ H->Size-- ];

    for( i = 1; i * 2 <= H->Size; i = Child ){  // Find smaller child
        Child = i * 2;
        if( Child != H->Size && H->Elements[ Child + 1 ]< H->Elements[ Child ] )
            Child++;

        // Percolate one level
        if( LastElement > H->Elements[ Child ] )
            H->Elements[ i ] = H->Elements[ Child ];
        else
            break;
        }
        H->Elements[ i ] = LastElement;
        return MinElement;
}

ElementType FindMin( PriorityQueue H ){
    if( !IsEmpty( H ) )
        return H->Elements[ 1 ];
    Error( "Priority Queue is Empty" );
    return H->Elements[ 0 ];
}

int IsEmpty( PriorityQueue H ){
    return H->Size == 0;
}

int IsFull( PriorityQueue H ){
    return H->Size == H->Capacity;
}

void Destroy( PriorityQueue H ){
    free( H->Elements );
    free( H );
}

void TopologicalSort(Graph G, int Size); //This procedure finds the right order in which data are inserted to the hash table

int main (int argn,char** argv){
	int N,i,tmp,index,val;
	Graph G;
	scanf("%d",&N);
	//start to build the graph
	G=CreatGraph(N);
	for(i=0;i<N;i++){
		scanf("%d",&tmp);
		AddVertex(G,tmp);   //insert a number into the graph as a vertex
	}
	for(i=0;i<N;i++){
		val=Value(G,i);
		if(val>=0){
			//start to find the predecessor of the i-th vertex
			index=val%N;
			while(index!=i){    //all the vertices between val%N and i are predecessor of val
				AddEdge(G,index,i);     //add the directed edge <index,i> to the graph
				index++;
				if(index>=N)    //if index overflows, fix it
					index-=N;
			}
			//end finding the predecessor of the i-th vertex
		}
	}
	//end building the graph
	TopologicalSort(G,N);   //find and output result
	DestroyGraph(G);
	return 0;
}

void TopologicalSort(Graph G, int Size){
	//all the outputable vertices store in this heap. A min heap ensure that the output is always the smallest of feasible vertices
	PriorityQueue H;
	int i,vertex,suc;
	H=Initialize(Size);
	//find all the vertices that has no predecessor and insert them to heap
	for(i=0;i<Size;i++){
		vertex=Value(G,i);
		if(NoPredecessor(G,i) && vertex>=0)
			Insert(vertex,H);
	}

	while(!IsEmpty(H)){
		vertex=DeleteMin(H);    //pop the min of feasible vertices
		printf("%d",vertex);
		suc=NextSuccessor(G,vertex);
		while(suc!=-1){     //if suc==-1, that means all the successor has been visited
			(G->array[suc]).pre_num--;
			if(NoPredecessor(G,suc))    //if this successor has no predecessor, it is outputable.
				Insert(Value(G,suc),H);
			suc=NextSuccessor(G,vertex);    //suc point to another successor
		}
		if(!IsEmpty(H))     //if there are other vertices waiting to ouput, print a space to seperate the vertices.
			printf(" ");
	}
	Destroy(H);
}
