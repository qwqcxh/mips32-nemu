#include <stdio.h>
#include <assert.h>

int main() {
  printf("test-1 passed\n"); //debug
  FILE *fp = fopen("/share/texts/num", "r+");
  assert(fp);
  printf("test0 passed\n"); //debug
  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);
  assert(size == 5000);
  printf("test1 passed\n");//debug
  fseek(fp, 500 * 5, SEEK_SET);
  int i, n;
  for (i = 500; i < 1000; i ++) {
    fscanf(fp, "%d", &n);
    assert(n == i + 1);
  }
  printf("test2 passed\n");//debug
  fseek(fp, 0, SEEK_SET);
  for (i = 0; i < 500; i ++) {
    fprintf(fp, "%4d\n", i + 1 + 1000);
  }
  printf("test3 passed\n");//debug
  for (i = 500; i < 1000; i ++) {
    fscanf(fp, "%d", &n);
    assert(n == i + 1);
  }
  printf("test4 passed\n");//debug
  fseek(fp, 0, SEEK_SET);
  for (i = 0; i < 500; i ++) {
    fscanf(fp, "%d", &n);
    assert(n == i + 1 + 1000);
  }
  printf("test5 passed\n");//debug
  fclose(fp);

  printf("PASS!!!\n");

  return 0;
}
