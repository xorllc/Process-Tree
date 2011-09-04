#include <stdlib.h>
#include <stdio.h>

typedef struct _node node;
struct _node {
  int pid;
  int ppid;
}

static int size;

void findSize() {
  size = 0;

  FILE *file;
  file = popen("ps -alx", "r");

  char line[130];

  while(fgets(line, sizeof line, file)) {
    size += 1;
  }

  pclose(file);
}

void parse(node *values[]) {
  FILE *file;
  file = popen("ps -alx", "r");

  char line[130];

  int index = 0;

  while(fgets(line, sizeof line, file)) {
    strtok(line, " ");
    strtok(NULL, " ");
    strtok(NULL, " ");

    node *temp = malloc(sizeof(node));
    temp->pid = atoi(strtok(NULL, " "));
    temp->ppid = atoi(strtok(NULL, " "));

    values[index] = temp;
    index++;
  }

  pclose(file);
}

void printRoot(node *values[]) {
  for(int i = 0; i < size; i++) {
    if(values[i]->pid == 0) {
      printf("%i %i\n", values[i]->pid, values[i]->ppid);
    }
  }
}

void printTree(node *values[], int currentPid, int depth) {
  for(int i = 0; i < size; i++) {
    if(values[i]->ppid == currentPid) {
      printf("%*s", depth, "-");
      printf("%i %i\n", values[i]->pid, values[i]->ppid);

      int newDepth = depth + 1;
      printTree(values, values[i]->pid, newDepth);
    }
  }
}

int main(int argc, char** argv) {
  findSize();
  node *values[size];
  parse(values);
  printRoot(values);
  printTree(values, 1, 1);
  return 0;
}
