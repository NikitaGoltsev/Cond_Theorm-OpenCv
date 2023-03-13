#include <math.h>
#include <stdio.h>
#include <stdlib.h>

double **creater_mtr(int n, int m);
void clear(double **mas, int n);
void cicle_of_prog(double **mas, int n, int m, int iter_max, double tooles);
double max_fn(double x, double y);
void copy_to_old(double **new_m, double **old_m, int n, int m);
void copy_el(double *x, double *y);
double module_fn(double x);

int main(int argc, char *argv[]) {
  double **arr;
  int n, m;
  int iter_max;

  // Change tool
  double tool = 0.000001;
  n = 1024;
  m = 1024;
  iter_max = 1000000;
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

double **creater_mtr(int n, int m) {
  double **mas;
  mas = malloc(sizeof(double *) * n);
  // Cicle here
  for (int i = 0; i < n; i++) {
    mas[i] = malloc(sizeof(double) * m);
    for (int j = 0; j < m; j++)
      mas[i][j] = 0;
  }

  return mas;
}

void clear(double **mas, int n) {
  for (int i = 0; i < n; i++) {
    free(mas[i]);
  }
  free(mas);
}

void cicle_of_prog(double **mas, int n, int m, int iter_max, double tooles) {
  int iter = 0;
  double error_c = 1.0; // Is its init here?
  double toll = tooles;
  double **local_arr;
  local_arr = creater_mtr(n, m);

  #pragma acc enter data copyin(mas [0:n] [0:m], error_c) create(local_arr[n][m])

  mas[0][0] = 10;
  mas[0][m - 1] = 20;
  mas[n - 1][0] = 30;
  mas[n - 1][m - 1] = 20;

  #pragma acc parallel loop present(mas[0:n][0:m])
  for (int i = 1; i < n - 1; ++i) {
    mas[0][i] = 10 + ((mas[0][m - 1] - mas[0][0]) / (n - 1)) * i;
    mas[i][0] = 10 + ((mas[n - 1][0] - mas[0][0]) / (n - 1)) * i;
    mas[n - 1][i] = 10 + ((mas[n - 1][0] - mas[n - 1][m - 1]) / (n - 1)) * i;
    mas[i][m - 1] = 10 + ((mas[n - 1][m - 1] - mas[n - 1][0]) / (n - 1)) * i;
  }

  while (iter <= iter_max && error_c > toll) {
    iter += 1;
    error_c = 0.0;

    #pragma acc update device(error_c)

    #pragma acc parallel loop collapse(2) present(mas[0:n][0:m]) reduction(max : error_c)
    for (int i = 1; i < n - 1; i++) {
      for (int j = 1; j < m - 1; j++) {
        //
        local_arr[i][j] = 0.25 * (mas[i - 1][j] + mas[i][j - 1] +
                                  mas[i + 1][j] + mas[i][j + 1]);
        // add error check
        error_c = max_fn(error_c, module_fn((local_arr[i][j] - mas[i][j])));
      }
    }

    // Iter check, out of cicles
    if (iter % 100 == 0 || iter == 1)
      printf("iter:%d error:%lf\n", iter, error_c);

    copy_to_old(mas, local_arr, n, m);
  }
  clear(local_arr, n);
  printf("Iterations:%d\nError:%lf", iter, error_c);
}

double max_fn(double x, double y) {
  double res = y;
  if (x > res)
    res = x;
  return res;
}

void copy_to_old(double **new_m, double **old_m, int n, int m) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++)
      copy_el(&new_m[i][j], &old_m[i][j]);
  }
}

void copy_el(double *x, double *y) { *x = *y; }

double module_fn(double x) {
  if (x < 0)
    x *= -1;
  return x;
}