#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int **creater_mtr(int n, int m);
void clear(int **mas, int n);
void cicle_of_prog(int **mas, int n, int m, int iter_max, double tooles);
int max_fn(int x, int y);
void copy_to_old(int **new_m, int **old_m, int n, int m);
void copy_el(int *x, int *y);

int main(int argc, char *argv[]) {
  int **arr;
  int n, m;
  int iter_max;

  // Change tool
  double tool = 0.000001;
  n = 128;
  m = 128;
  iter_max = 100000;
  /*if (argc <= 1) {
    scanf("%d %d", &n, &m);
    scanf("%d", &iter_max);
    scanf("%d", &tool);
  } else {
  }*/
  arr = creater_mtr(n, m);

  cicle_of_prog(arr, n, m, iter_max, tool);

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

void cicle_of_prog(int **mas, int n, int m, int iter_max, double tooles) {
  int iter = 0;
  double error_c = 1.0; // Is its init here?
  double toll = tooles;
  while (iter <= iter_max && error_c > toll) {
    iter += 1;
    error_c = 0;
    int **local_arr;
    local_arr = creater_mtr(n, m);

    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        //
        if (i != 0 && j != 0 && i != n - 1 && j != m - 1)
          local_arr[i][j] = (mas[i][j] + mas[i - 1][j] + mas[i][j - 1] +
                             mas[i + 1][j] + mas[i][j + 1]);
        // add error check
        error_c = max_fn(error_c, (local_arr[i][j] - mas[i][j]));
      }
    }

    // Iter check, out of cicles
    if (iter % 100 == 0 || iter == 1)
      printf("iter:%d error:%lf\n", iter, error_c);

    copy_to_old(mas, local_arr, n, m);
    clear(local_arr, n);
    break;
  }

  printf("Iterations:%d\nError:%lf", iter, error_c);
}

int max_fn(int x, int y) {
  if (x > y)
    y = x;
  return y;
}

void copy_to_old(int **new_m, int **old_m, int n, int m) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++)
      copy_el(&new_m[i][j], &old_m[i][j]);
  }
}

void copy_el(int *x, int *y) { *x = *y; }