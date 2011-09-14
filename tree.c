#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct _node node;
struct _node {
  int pid;
  int ppid;
};

static int size;

void findSize() {
  size = 0;

  FILE *file;
  file = popen("ps -alx", "r");

  char line[130];

  fgets(line, sizeof line, file);
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

  fgets(line, sizeof line, file);
  while(fgets(line, sizeof line, file)) {
    node *temp = malloc(sizeof(node));

    strtok(line, " ");
    strtok(NULL, " ");
    strtok(NULL, " ");

    char *pid = NULL;
    pid = strtok(NULL, " ");
    if(strncmp((pid + 0), "(", 1) == 0) {
      temp->pid = atoi((pid + 2));
      temp->pid = temp->pid * -1;
    } else {
      temp->pid = atoi(pid);
    }

    temp->ppid = atoi(strtok(NULL, " "));

    values[index] = temp;
    index++;
  }

  pclose(file);
}

void printRoot(node *values[]) {
  for(int i = 0; i < size; i++) {
    if(values[i]->ppid == 0) {
      printf("%i %i\n", values[i]->pid, values[i]->ppid);
    }
  }
}

void printTree(node *values[], int currentPid, int depth) {
  for(int i = 0; i < size; i++) {
    if(values[i]->ppid == currentPid) {
      printf("%*s", depth, " ");
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
