#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int **creater_mtr(int n, int m);
void clear(int **mas, int n);
void cicle_of_prog(int **mas, int n, int m, int iter_max);

int main(int argc, char *argv[]) {
  int **arr;
  int n, m;
  int iter_max;

  if (argc <= 1) {
    scanf("%d %d", &n, &m);
    scanf("%d", &iter_max);
  } else {
  }
  arr = creater_mtr(n, m);

  clear(arr, n);
  return 0;
}

int **creater_mtr(int n, int m) {
  int **mas;
  mas = malloc(sizeof(int *) * n);
  // Cicle here
  for (int i = 0; i < n; i++) {
    mas[i] = malloc(sizeof(int) * m);
    for (int j = 0; j < m; j++)
      mas[i][j] = 0;
  }
  mas[0][0] = 10;
  mas[0][m - 1] = 20;
  mas[n - 1][0] = 30;
  mas[n - 1][m - 1] = 20;

  return mas;
}

void clear(int **mas, int n) {
  for (int i = 0; i < n; i++) {
    free(mas[i]);
  }
  free(mas);
}

void cicle_of_prog(int **mas, int n, int m, int iter_max) {
  int iter = 0;

  while (iter <= iter_max) {
    iter += 1;
    int **local_arr;
    local_arr = creater_mtr(n, m);

    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        // local_arr[i][j] = mas[][] + mas[][] + mas[][] + mas[] + mas[][];
      }
    }

    clear(local_arr, n);
    break;
  }
}
