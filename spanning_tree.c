#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#pragma warning (disable:4996)

#define MAX 2147483647
#define TRUE 1
#define FALSE 0
#define INPUT_FILE "indata.dat"						//입력파일 명

#define L_CHILD(x) 2 * x							//x의 왼쪽 자식 노드 인덱스
#define R_CHILD(x) 2 * x + 1						//x의 왼쪽 자식 노드 인덱스
#define _PARENT(x) (x - 1) / 2						//x의 부모 노드인덱스

typedef struct vertex {								//vertex 구조체
	int cv;
	int data;
}vertex;

typedef struct minHeap {							//Heap 구조체
	int size;
	vertex *v;
} min_Heap;

int *selected;										//선택된 vertex 저장 배열
int *distance;										//vertex의 키 값을 저장하는 배열
int *prev;											//vertex의 이전 노드 저장 배열


void exchange(vertex *v1, vertex *v2) {				//인자를 바꾸는 함수
	vertex temp = *v1;
	*v1 = *v2;
	*v2 = temp;
}

void min_heapify(min_Heap *hp, int index) {			//힙 변경시 힙 성질을 유지시켜 주는 함수

	int smallest;

	if (L_CHILD(index) <= hp->size && hp->v[L_CHILD(index)].data < hp->v[index].data){
		smallest = L_CHILD(index);
	}
	else
		smallest = index;

	if (R_CHILD(index) <= hp->size && hp->v[R_CHILD(index)].data < hp->v[smallest].data) {
		smallest = R_CHILD(index);
	}
	if (smallest != index) {
		exchange(&(hp->v[index]), &(hp->v[smallest]));
		min_heapify(hp, smallest);
	}
}

void my_min_heapify(min_Heap *hp, int num) {		//리프노드부터 힙을 만들어 주는 함수
	int i;
	for (i = num / 2; i > 0; i--) {
		min_heapify(hp, i);
	}
}

//힙 삽입 함수
void insert_min_heap(min_Heap *hp, vertex item)		//힙에 인자를 입력하는 함수
{	
	int i = ++(hp->size);							//힙의 인덱스
	hp->v[i] = item;
	while (i > 1) {									//인덱스가 루트노드에 갈 때까지 반복
		if (hp->v[i].data < hp->v[i / 2].data) {	//자식이 부모보다 크면
			exchange(&(hp->v[i]), &(hp->v[i / 2])); //자식과 부모의 위치를 바꾼다.
			i /= 2;
		}
		else
			break;
	}
}
													//힙에서 최소값 가져오기 (ExtraMin)
vertex delete_Node(min_Heap *hp) {					
		
	int parent, child;
	vertex item, temp;
	item = hp->v[1];								//최소 값을 가진 노드를 저장
	temp = hp->v[(hp->size)--];						//힙의 가장 큰 값을 루트로 이동
	parent = 1;
	child = 2;
	while (child <= hp->size) {						//자기 자리를 찾아가는 과정

		if ((child < hp->size) && (hp->v[child].data) > hp->v[child + 1].data)         
			child++;

		if (temp.data <= hp->v[child].data)                                       
			break;

		hp->v[parent] = hp->v[child];
		parent = child;
		child = child * 2;
	}
	hp->v[parent] = temp;
	return item;									//저장한 최소 노드를 리턴한다.
}

void min_spaning_key_value(int **edge, int start, int num, min_Heap *Q, vertex *current_v, vertex *adj, int **output){
													//spanning_tree 함수를 MST로 만들어주는 함수
	int i, u, j, index;
	vertex temp;

	while (Q->size != 0){							//힙이 없어질 때까지

		*current_v = delete_Node(Q);				//최소 노드 저장
		u = current_v->cv - 1;
		selected[u] = 0;

		for (i = 0; i < num; i++){					//인접노드 확인
			if (u + 1 != i + 1 && edge[u][i] != 0 && selected[i] == 1){
				temp.cv = i + 1;
				temp.data = edge[u][i];
				adj[i] = temp;
			}
		}

		for (i = 0; i < num; i++){
			for (j = 0; j < Q->size; j++){			//인접 노드와 같은 vertex의 인덱스를 찾아서 저장
				if (adj[i].cv == Q->v[j + 1].cv){
					index = j + 1;
				}
			}

			if (adj[i].data != 0 && adj[i].data < Q->v[index].data){

				Q->v[index].data = adj[i].data;		//인접 노드 키값 변환
				prev[i] = current_v->cv;			//이전 노드 저장
				distance[i] = edge[i][u];			//키값 저장
			}
		}
		my_min_heapify(Q, Q->size);					//힙 유지

		for (i = 0; i < num; i++){					//인접노드 초기화
			temp.cv = 0;
			temp.data = 0;
			adj[i] = temp;
		}
	}
}


void shortest_spanning_dey_value(int **edge, int start, int num, min_Heap *Q, vertex *current_v, vertex *adj, int **output){
													//spanning_tree 함수를 MST로 만들어주는 함수
	int i, u, j, index;
	vertex temp;
	while (Q->size != 0){							//힙이 없어질 때까지

		*current_v = delete_Node(Q);				//최소 노드 저장
		u = current_v->cv - 1;
		selected[u] = 0;

		for (i = 0; i < num; i++){					//인접노드 확인
			if (u + 1 != i + 1 && edge[u][i] != 0 && selected[i] == 1){
				temp.cv = i + 1;
				temp.data = current_v->data + edge[u][i];//최단거리 비교해서 저장(relax)
				adj[i] = temp;
			}
		}

		for (i = 0; i < num; i++){
			for (j = 0; j < Q->size; j++){			//인접노드와 같은 vertex의 인덱스를 찾아서 저장
				if (adj[i].cv == Q->v[j + 1].cv){
					index = j + 1;
				}
			}

			if (adj[i].data != 0 && adj[i].data < Q->v[index].data){
				for (j = 0; j < num; j++){
					output[i][j] = output[u][j];
				}
				output[i][u] = edge[i][u];
				Q->v[index].data = adj[i].data;		//인접노드 키값 변환
				prev[i] = current_v->cv;			//이전 노드 저장
				distance[i] = adj[i].data;			//최단거리 저장
			}
		}
		my_min_heapify(Q, Q->size);					//힙 유지

		for (i = 0; i < num; i++){					//인접노드 초기화
			temp.cv = 0;
			temp.data = 0;
			adj[i] = temp;
		}
	}
}

void print_graph(int **G, int num)					//그래프 출력 함수
{
	int i, j, v;
	v = num;
	printf("     |");
	for (i = 1; i <= v; i++){
		printf("%6d", i);
	}
	printf("\n-----|-------------------------------------------\n");
	for (i = 1; i <= v; i++){
		printf("%4d |", i);
		for (j = 1; j <= v; j++){
			printf("%6d", G[i - 1][j - 1]);
		}
		printf("\n     |\n");

	}
}

void spanning_tree(int **edge, int start, int num, int selectNum){
	int i,j;
	min_Heap Q;										//최소 힙 선언
	vertex temp, current_v, *adj;
	int **output;
	FILE *f;

	f = fopen("output.txt", "w+");
	distance = (int *)calloc(num, sizeof(int));		//엣지값 배열 동적할당
	selected = (int *)calloc(num, sizeof(int));		//방문여부 배열 동적할당
	prev = (int *)calloc(num, sizeof(int));			//이전노드 배열 동적할당
	adj = (vertex *)calloc(num, sizeof(vertex));	//인접노드 배열 동적할당
	output = (int **)calloc(num, sizeof(int *));	//출력 배열 동적할당

	for (i = 0; i < num; i++){
		output[i] = (int *)calloc(num, sizeof(int));
		selected[i] = 1;
	}

	distance[start - 1] = 0;
	prev[start - 1] = 0;

	Q.size = 0;
	Q.v = (vertex *)malloc((num + 1)*sizeof(vertex));//힙 생성

	for (i = 1; i <= num; i++){						 //힙 구성
		temp.cv = i;
		temp.data = MAX;
		insert_min_heap(&Q, temp);
	}

	Q.v[start].cv = start;							//시작점 초기화
	Q.v[start].data = 0;
	current_v = Q.v[start];

	my_min_heapify(&Q, num);						//힙 유지

	if (selectNum == 1){							//MST를 만들라면
		min_spaning_key_value(edge, start, num, &Q, &current_v, adj, output);
	}												//MST 함수 호출
	else
	{												//Shortest Path 만들라면
		shortest_spanning_dey_value(edge, start, num, &Q, &current_v, adj, output);
	}												//Shortest Path 호출
		


	for (i = 0; i < num; i++){						
		if (prev[i] != 0)
			output[i][prev[i] - 1] = distance[i];
	}
	printf("\n\n이전노드\nvertex의 index : ");		//이전 노드 출력
	for (i = 0; i < num; i++){
		printf("%3d", i+1);
	}
	printf("\nvertex의 값 :    ");
	for (i = 0; i < num; i++){
		printf("%3d", prev[i]);
	}
	
	printf("\n\n\n거리\nvertex의 index : ");			//거리 값 출력
	for (i = 0; i < num; i++){
		printf("%3d", i + 1);
	}
	printf("\nvertex까지 값 :  ");
	for (i = 0; i < num; i++){
		printf("%3d", distance[i]);
	}
	printf("\n\n ");

	print_graph(output, num);						//트리 출력

	fprintf(f, "%d\n", num);						//파일에 출력
	for (i = 0; i < num; i ++){
		for (j = 0; j < num; j++){
			fprintf(f, "%3d", output[i][j]);
		}
		fprintf(f,"\n");
	}
	fclose(f);
	free(prev);										//동적할당 반환
	free(distance);
	free(selected);
	free(adj);
	for (i = 0; i < num; i++){
		free(output[i]);
	}
	free(output);

}

int main(){

	FILE *fp;
	int num, i, j, start = 0, selectNum=0;
	char input[10];
	int **edge = NULL;

	fp = fopen(INPUT_FILE, "r");
	fscanf_s(fp, "%d", &num);						//vertex 수와 시작점을 입력받음

	edge = (int **)calloc(num, sizeof(int *));		//2차원 입력 배열 동적할당

	for (i = 0; i < num; i++){
		edge[i] = (int *)calloc(num, sizeof(int));	//2차원 입력 배열 동적할당
	}

	for (i = 0; i < num; i++){						//입력값을 배열에 삽입
		for (j = 0; j < num; j++){
			fscanf_s(fp, "%d", &edge[i][j]);
		}
	}

	fclose(fp);

	print_graph(edge, num);							//초기 엣지 값 출력

	while (TRUE){										//알고리즘 선택
		printf("\n\n\n");
		printf("1. 프림(MST)\n\n2. 다익스트라(Shortest Path)\n\n3. 종료\n\n");
		printf("알고리즘을 선택하시오 : ");

		gets(input);
		selectNum = atoi(input);
		fflush(stdin);

		switch (selectNum){
		case 1:
			printf("\n시작노드를 입력하시오 : ");
			gets(input);
			start = atoi(input);
			fflush(stdin);

			spanning_tree(edge, start, num, selectNum);
			break;

		case 2:
			printf("\n시작노드를 입력하시오 : ");
			gets(input);
			start = atoi(input);
			fflush(stdin);

			spanning_tree(edge, start, num, selectNum);
			break;

		case 3:
			printf("프로그램이 종료됩니다.\n");
			return;

		default:
			printf("입력이 잘못되었습니다. 다시입력하시오\n\n");
			break;
		}
	}

	for (i = 0; i < num; i++){							//입력 배열 반환
		free(edge[i]);
	}
	free(edge);

	return 0;
}