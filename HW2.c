#include <stdio.h>
#include <stdlib.h>

void write_graph(FILE* fname, int** arr, int size)
{
	fprintf(fname, "graph G{\n");

	for (int i = 0; i < size; i++)
		fprintf(fname, "%d;\n", i);

	for (int j = 0; j < size; j++) {
		for (int i = j; i < size; i++)
			for (int k = 0; k < arr[i][j]; k++)
				fprintf(fname, "%d -- %d;\n", j, i);
	}

	fprintf(fname, "}");

	fclose(fname);
}

void check(int** arr, int size) {

	for (int i, j = 0; j < size; j++) {			// ребро графа приходит в ту же вершину
		i = j;
		if (arr[i][j] != 0) {
			printf("Граф не является простым циклом\n");
			return;
		}
	}

	for (int j = 0; j < size; j++)				// между двумя вершинами больше одного ребра
		for (int i = 0; i < size; i++) {
			if (arr[i][j] > 1) {
				printf("Граф не является простым циклом\n");
				return;
			}
		}

	for (int k, j = 0; j < size; j++) {

		if (size == 2)							// только 2 вершины
			break;

		k = 0;									// чтобы был цикл, необходимо, чтобы в каждом строке и каждом столбце было две единицы. k - счетчик этих единиц
		for (int i = 0; i < size; i++) {
			if (arr[i][j] != arr[j][i]) {
				printf("Матрица несимметричная\n"); // если матрица несимметричная
				break;
			}

			if (arr[i][j] == 1)
				k++;
			//printf("i=%d j=%d     k=%d\n", i, j, k);
		}

		if (k != 2) {
			printf("Граф не является простым циклом\n");
			return;
		}
	}

	int num = 1;
	for (int i = 0, j = 1; j < size; i++, j++) {		// проверка на количество циклов
		if (arr[i][j] == 0) {							// алгоритм смотри в тетради
			num++;
			//printf("num = %d\n", num);
		}
	}

	printf("Граф - простой цикл. Количество циклов: %d\n", num);
}

void free_arr(char* arr) {
	free(arr);
	arr = NULL;
	arr = (char*)malloc(sizeof(char));
}

int main(void) {
	int width;
	printf("Размер квадратной матрицы: ");
	scanf("%d", &width);


	printf("\nЗаполните строки матрицы. Нажимая \"ENTER\" вы перейдете на следующую строку. Если вы не ввели число, то оставшиеся ячейки будут заполнены '0'.\n");

	getchar();											


	int** arr = NULL;
	arr = (int**)malloc(sizeof(*arr) * width);
	for (int i = 0; i < width; i++) {
		arr[i] = (int*)malloc(sizeof(**arr) * width);
	}

	char* s, c;
	s = (char*)malloc(sizeof(char));


	for (int i = 0, m = 1, j = 0; j < width; j++) {

		while ((c = getchar()) != '\n') {

			if ((c < '0') && c != ' ') {
				printf("Введенное значение некорректное\n");
				return -1;
			}

			if (c != ' ') {
				s[m - 1] = c;

				s = (char*)realloc(s, sizeof(char) * (++m));
			}

			else {
				s[m - 1] = '\0';

				if (i >= width) {
					printf("Чисел в строке больше, чем задано\n");
					return -1;
				}

				arr[i][j] = atoi(s);      // преобразуем строку в число
				i++;
				m = 1;

				free_arr(s);

			}

		}

		s[m - 1] = '\0';

		if (i >= width) {
			printf("Чисел в строке больше, чем задано\n");
			return -1;
		}

		arr[i][j] = atoi(s);

		if (i < width) {
			++i;
			while (i < width)
				arr[i++][j] = 0;
		}

		i = 0;
		m = 1;

		free_arr(s);
	}

	free(s);
	s = NULL;

	FILE* fout;
	fout = fopen("graph.gv", "w");

	if (fout == NULL) {
		perror("fopen()");
		return EXIT_FAILURE;
	}

	write_graph(fout, arr, width);

	check(arr, width);


	for (int i = 0; i < width; i++)
		free(arr[i]);
	free(arr);
	arr = NULL;

	system("dot -Tjpg graph.gv -o HW2.jpg");
	return 0;
}