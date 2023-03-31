#include <cmath>
#include <iostream>
#include <string>

#define TYPE double
#define ABS fabs
#define MAX fmax
#define CAST std::stod

void swap_for_tp(TYPE **x, TYPE **y);

int initArr(TYPE **A, int n) {

  A[0][0] = 10.0;
  A[0][n - 1] = 20.0;
  A[n - 1][0] = 30.0;
  A[n - 1][n - 1] = 20.0;

  for (int i{1}; i < n - 1; ++i) {
    A[0][i] = 10 + ((A[0][n - 1] - A[0][0]) / (n - 1)) * i;
    A[i][0] = 10 + ((A[n - 1][0] - A[0][0]) / (n - 1)) * i;
    A[n - 1][i] = 10 + ((A[n - 1][0] - A[n - 1][n - 1]) / (n - 1)) * i;
    A[i][n - 1] = 10 + ((A[n - 1][n - 1] - A[n - 1][0]) / (n - 1)) * i;
  }

  return 1;
}

void solution(TYPE tol, int iter_max, int n) {
  TYPE error{1.0};
  int iter{0};
  TYPE **A = new TYPE *[n], **Anew = new TYPE *[n];
  for (int i{0}; i < n; ++i) {
    A[i] = new TYPE[n];
    Anew[i] = new TYPE[n];
  }


 #pragma acc enter data copyin(A [0:n] [0:n],error) create (Anew[0:n][0:n])
  initArr(A, n);
  initArr(Anew, n);
    
  while (error > tol && iter < iter_max) {
    error = 0.0;

    #pragma acc parallel loop collapse(2) present(A[0:n][0:n]) reduction(max : error)
    for (int j{1}; j < n - 1; ++j) {
      for (int i{1}; i < n - 1; ++i) {
        Anew[j][i] =
            0.25 * (A[j][i + 1] + A[j][i - 1] + A[j - 1][i] + A[j + 1][i]);
        error = MAX(error, ABS(Anew[j][i] - A[j][i]));
      }
    }

    swap_for_tp(A, Anew);
    if (iter % 1000 == 0) {
      std::cout << "Iter num: " << iter << "\n Error on iter " << error
                << std::endl;
    }
    ++iter;

  }

  std::cout << "Iterations: " << iter << std::endl
            << "Error: " << error << std::endl;

  for (int i{0}; i < n; i++) {
    delete[] A[i];
    delete[] Anew[i];
  }
  delete[] A;
  delete[] Anew;
}

int main(int argc, char *argv[]) {
  TYPE tol{1e-6};
  int iter_max{1000000}, n{128}; // Net with zeros by default

  std::string tmpStr;
  //-t - точность
  //-n - размер сетки
  //-i - кол-во итераций
  for (int i{1}; i < argc; ++i) {
    tmpStr = argv[i];
    if (!tmpStr.compare("-t")) {
      tol = CAST(argv[i + 1]);
      ++i;
    }

    if (!tmpStr.compare("-i")) {
      iter_max = std::stoi(argv[i + 1]);
      ++i;
    }

    if (!tmpStr.compare("-n")) {
      n = std::stoi(argv[i + 1]);
      ++i;
    }
  }
  solution(tol, iter_max, n);
}

void swap_for_tp(TYPE **x, TYPE **y) {
  TYPE t;
  t = **x;
  **x = **y;
  **y = t;
}