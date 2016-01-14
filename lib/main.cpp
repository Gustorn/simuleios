#include <iostream>

#include <algorithm>
#include <cassert>
#include "array2d.h"

using namespace std;

int main() {
  array2d<int> test = {{1, 2, 0},
                       {4, 5, 6},
                       {7, 8, 0}};

  const auto& ctest = test;

  auto crows = ctest.rows_view();
  auto ccols = ctest.columns_view();

  auto mrows = test.rows_view();
  auto pred = [](const auto& r1) {
    return r1[2] == 0;
  };
  std::stable_partition(std::begin(mrows), std::end(mrows), pred);

  std::for_each(std::begin(mrows), std::end(mrows), [&](const auto& row) {
    cout << "Row " << row.index() << endl;
    //cout << "Row pred " << pred(row) << endl;
  });

  for (auto row : test.rows_view()) {
    for (auto& cell : row) {
      cell += 1;
    }
  }

  for (auto row : test.rows_view()) {
    row[0] += 1;
    row[1] += 1;
    row[2] += 1;
  }

  for (auto col: test.columns_view()) {
    for (auto& cell : col) {
      cell += 1;
    }
  }

  for (auto col : test.columns_view()) {
    col[0] += 1;
    col[1] += 1;
    col[2] += 1;
  }

  for (auto row : crows) {
    for (const auto& cell : row) {
      cout << cell << "\t";
    }
    cout << endl;
  }

  cout << endl;

  for (auto row : crows) {
    cout << row[0] << "\t"
         << row[1] << "\t"
         << row[2] << endl;
  }

  cout << endl;

  for (auto col : ccols) {
    for (const auto& cell: col) {
      cout << cell << "\t";
    }
    cout << endl;
  }

  cout << endl;

  for (auto col : ccols) {
    cout << col[0] << "\t"
         << col[1] << "\t"
         << col[2] << endl;
  }

  cout << endl;

  return 0;
}
