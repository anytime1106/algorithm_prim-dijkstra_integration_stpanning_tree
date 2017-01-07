#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#pragma warning (disable:4996)

#define MAX 2147483647
#define TRUE 1
#define FALSE 0
#define INPUT_FILE "indata.dat"						//�Է����� ��

#define L_CHILD(x) 2 * x							//x�� ���� �ڽ� ��� �ε���
#define R_CHILD(x) 2 * x + 1						//x�� ���� �ڽ� ��� �ε���
#define _PARENT(x) (x - 1) / 2						//x�� �θ� ����ε���

typedef struct vertex {								//vertex ����ü
	int cv;
	int data;
}vertex;

typedef struct minHeap {							//Heap ����ü
	int size;
	vertex *v;
} min_Heap;

int *selected;										//���õ� vertex ���� �迭
int *distance;										//vertex�� Ű ���� �����ϴ� �迭
int *prev;											//vertex�� ���� ��� ���� �迭


void exchange(vertex *v1, vertex *v2) {				//���ڸ� �ٲٴ� �Լ�
	vertex temp = *v1;
	*v1 = *v2;
	*v2 = temp;
}

void min_heapify(min_Heap *hp, int index) {			//�� ����� �� ������ �������� �ִ� �Լ�

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

void my_min_heapify(min_Heap *hp, int num) {		//���������� ���� ����� �ִ� �Լ�
	int i;
	for (i = num / 2; i > 0; i--) {
		min_heapify(hp, i);
	}
}

//�� ���� �Լ�
void insert_min_heap(min_Heap *hp, vertex item)		//���� ���ڸ� �Է��ϴ� �Լ�
{	
	int i = ++(hp->size);							//���� �ε���
	hp->v[i] = item;
	while (i > 1) {									//�ε����� ��Ʈ��忡 �� ������ �ݺ�
		if (hp->v[i].data < hp->v[i / 2].data) {	//�ڽ��� �θ𺸴� ũ��
			exchange(&(hp->v[i]), &(hp->v[i / 2])); //�ڽİ� �θ��� ��ġ�� �ٲ۴�.
			i /= 2;
		}
		else
			break;
	}
}
													//������ �ּҰ� �������� (ExtraMin)
vertex delete_Node(min_Heap *hp) {					
		
	int parent, child;
	vertex item, temp;
	item = hp->v[1];								//�ּ� ���� ���� ��带 ����
	temp = hp->v[(hp->size)--];						//���� ���� ū ���� ��Ʈ�� �̵�
	parent = 1;
	child = 2;
	while (child <= hp->size) {						//�ڱ� �ڸ��� ã�ư��� ����

		if ((child < hp->size) && (hp->v[child].data) > hp->v[child + 1].data)         
			child++;

		if (temp.data <= hp->v[child].data)                                       
			break;

		hp->v[parent] = hp->v[child];
		parent = child;
		child = child * 2;
	}
	hp->v[parent] = temp;
	return item;									//������ �ּ� ��带 �����Ѵ�.
}

void min_spaning_key_value(int **edge, int start, int num, min_Heap *Q, vertex *current_v, vertex *adj, int **output){
													//spanning_tree �Լ��� MST�� ������ִ� �Լ�
	int i, u, j, index;
	vertex temp;

	while (Q->size != 0){							//���� ������ ������

		*current_v = delete_Node(Q);				//�ּ� ��� ����
		u = current_v->cv - 1;
		selected[u] = 0;

		for (i = 0; i < num; i++){					//������� Ȯ��
			if (u + 1 != i + 1 && edge[u][i] != 0 && selected[i] == 1){
				temp.cv = i + 1;
				temp.data = edge[u][i];
				adj[i] = temp;
			}
		}

		for (i = 0; i < num; i++){
			for (j = 0; j < Q->size; j++){			//���� ���� ���� vertex�� �ε����� ã�Ƽ� ����
				if (adj[i].cv == Q->v[j + 1].cv){
					index = j + 1;
				}
			}

			if (adj[i].data != 0 && adj[i].data < Q->v[index].data){

				Q->v[index].data = adj[i].data;		//���� ��� Ű�� ��ȯ
				prev[i] = current_v->cv;			//���� ��� ����
				distance[i] = edge[i][u];			//Ű�� ����
			}
		}
		my_min_heapify(Q, Q->size);					//�� ����

		for (i = 0; i < num; i++){					//������� �ʱ�ȭ
			temp.cv = 0;
			temp.data = 0;
			adj[i] = temp;
		}
	}
}


void shortest_spanning_dey_value(int **edge, int start, int num, min_Heap *Q, vertex *current_v, vertex *adj, int **output){
													//spanning_tree �Լ��� MST�� ������ִ� �Լ�
	int i, u, j, index;
	vertex temp;
	while (Q->size != 0){							//���� ������ ������

		*current_v = delete_Node(Q);				//�ּ� ��� ����
		u = current_v->cv - 1;
		selected[u] = 0;

		for (i = 0; i < num; i++){					//������� Ȯ��
			if (u + 1 != i + 1 && edge[u][i] != 0 && selected[i] == 1){
				temp.cv = i + 1;
				temp.data = current_v->data + edge[u][i];//�ִܰŸ� ���ؼ� ����(relax)
				adj[i] = temp;
			}
		}

		for (i = 0; i < num; i++){
			for (j = 0; j < Q->size; j++){			//�������� ���� vertex�� �ε����� ã�Ƽ� ����
				if (adj[i].cv == Q->v[j + 1].cv){
					index = j + 1;
				}
			}

			if (adj[i].data != 0 && adj[i].data < Q->v[index].data){
				for (j = 0; j < num; j++){
					output[i][j] = output[u][j];
				}
				output[i][u] = edge[i][u];
				Q->v[index].data = adj[i].data;		//������� Ű�� ��ȯ
				prev[i] = current_v->cv;			//���� ��� ����
				distance[i] = adj[i].data;			//�ִܰŸ� ����
			}
		}
		my_min_heapify(Q, Q->size);					//�� ����

		for (i = 0; i < num; i++){					//������� �ʱ�ȭ
			temp.cv = 0;
			temp.data = 0;
			adj[i] = temp;
		}
	}
}

void print_graph(int **G, int num)					//�׷��� ��� �Լ�
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
	min_Heap Q;										//�ּ� �� ����
	vertex temp, current_v, *adj;
	int **output;
	FILE *f;

	f = fopen("output.txt", "w+");
	distance = (int *)calloc(num, sizeof(int));		//������ �迭 �����Ҵ�
	selected = (int *)calloc(num, sizeof(int));		//�湮���� �迭 �����Ҵ�
	prev = (int *)calloc(num, sizeof(int));			//������� �迭 �����Ҵ�
	adj = (vertex *)calloc(num, sizeof(vertex));	//������� �迭 �����Ҵ�
	output = (int **)calloc(num, sizeof(int *));	//��� �迭 �����Ҵ�

	for (i = 0; i < num; i++){
		output[i] = (int *)calloc(num, sizeof(int));
		selected[i] = 1;
	}

	distance[start - 1] = 0;
	prev[start - 1] = 0;

	Q.size = 0;
	Q.v = (vertex *)malloc((num + 1)*sizeof(vertex));//�� ����

	for (i = 1; i <= num; i++){						 //�� ����
		temp.cv = i;
		temp.data = MAX;
		insert_min_heap(&Q, temp);
	}

	Q.v[start].cv = start;							//������ �ʱ�ȭ
	Q.v[start].data = 0;
	current_v = Q.v[start];

	my_min_heapify(&Q, num);						//�� ����

	if (selectNum == 1){							//MST�� ������
		min_spaning_key_value(edge, start, num, &Q, &current_v, adj, output);
	}												//MST �Լ� ȣ��
	else
	{												//Shortest Path ������
		shortest_spanning_dey_value(edge, start, num, &Q, &current_v, adj, output);
	}												//Shortest Path ȣ��
		


	for (i = 0; i < num; i++){						
		if (prev[i] != 0)
			output[i][prev[i] - 1] = distance[i];
	}
	printf("\n\n�������\nvertex�� index : ");		//���� ��� ���
	for (i = 0; i < num; i++){
		printf("%3d", i+1);
	}
	printf("\nvertex�� �� :    ");
	for (i = 0; i < num; i++){
		printf("%3d", prev[i]);
	}
	
	printf("\n\n\n�Ÿ�\nvertex�� index : ");			//�Ÿ� �� ���
	for (i = 0; i < num; i++){
		printf("%3d", i + 1);
	}
	printf("\nvertex���� �� :  ");
	for (i = 0; i < num; i++){
		printf("%3d", distance[i]);
	}
	printf("\n\n ");

	print_graph(output, num);						//Ʈ�� ���

	fprintf(f, "%d\n", num);						//���Ͽ� ���
	for (i = 0; i < num; i ++){
		for (j = 0; j < num; j++){
			fprintf(f, "%3d", output[i][j]);
		}
		fprintf(f,"\n");
	}
	fclose(f);
	free(prev);										//�����Ҵ� ��ȯ
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
	fscanf_s(fp, "%d", &num);						//vertex ���� �������� �Է¹���

	edge = (int **)calloc(num, sizeof(int *));		//2���� �Է� �迭 �����Ҵ�

	for (i = 0; i < num; i++){
		edge[i] = (int *)calloc(num, sizeof(int));	//2���� �Է� �迭 �����Ҵ�
	}

	for (i = 0; i < num; i++){						//�Է°��� �迭�� ����
		for (j = 0; j < num; j++){
			fscanf_s(fp, "%d", &edge[i][j]);
		}
	}

	fclose(fp);

	print_graph(edge, num);							//�ʱ� ���� �� ���

	while (TRUE){										//�˰��� ����
		printf("\n\n\n");
		printf("1. ����(MST)\n\n2. ���ͽ�Ʈ��(Shortest Path)\n\n3. ����\n\n");
		printf("�˰����� �����Ͻÿ� : ");

		gets(input);
		selectNum = atoi(input);
		fflush(stdin);

		switch (selectNum){
		case 1:
			printf("\n���۳�带 �Է��Ͻÿ� : ");
			gets(input);
			start = atoi(input);
			fflush(stdin);

			spanning_tree(edge, start, num, selectNum);
			break;

		case 2:
			printf("\n���۳�带 �Է��Ͻÿ� : ");
			gets(input);
			start = atoi(input);
			fflush(stdin);

			spanning_tree(edge, start, num, selectNum);
			break;

		case 3:
			printf("���α׷��� ����˴ϴ�.\n");
			return;

		default:
			printf("�Է��� �߸��Ǿ����ϴ�. �ٽ��Է��Ͻÿ�\n\n");
			break;
		}
	}

	for (i = 0; i < num; i++){							//�Է� �迭 ��ȯ
		free(edge[i]);
	}
	free(edge);

	return 0;
}