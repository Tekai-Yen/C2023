#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
 
#define MAXN 100
#define MAXS (MAXN * MAXN)
#define MAXC 10
#define MAXSTRLENP1 6        /* with '\0' */
 
#define NDEBUG
 
int max(int a, int b)
{
  return (a > b? a : b);
}
 
typedef struct {
  int N;
  char string[MAXN][MAXN][MAXN][MAXSTRLENP1];
  int height[MAXN][MAXN];
} Tower;

typedef struct {
  int row;
  int col;
} Position;

typedef struct {
  Position position;
  char string[MAXSTRLENP1];
} Hash;
 
void getTower(Tower *tower)
{
  int N;
  assert(scanf("%d", &N) == 1);
 
  tower->N = N;
  for (int layer = 1; layer <= N; layer++)
    for (int row = 0; row < layer; row++)
      for (int col = 0; col < layer; col++)
	assert(scanf("%s", tower->string[row][col][N - layer]) == 1);
  for (int row = 0; row < N; row++)
    for (int col = 0; col < N; col++)
      tower->height[row][col] = N - max(row, col);
}
 
int f(char s[], int K)
{
  int len = strlen(s);
  int v = 0;
  for (int i = 0; i < len; i++)
    v = v * 29 + (s[i] - 'a' + 1);
  return (v % K);
}
 
/* find key in the hash table, if found remove it and return its tower
   index, if not return (-1, -1) */
 
Position findRemove(Hash hashTable[MAXS][MAXC], char key[], int hash)
{
  for (int i = 0; i < MAXC; i++)
    if ((hashTable[hash][i].string[0] != '\0') &&
	strcmp(hashTable[hash][i].string, key) == 0) {
      hashTable[hash][i].string[0] = '\0'; /* remove the key */
      return (hashTable[hash][i].position);
    }
  Position notFound = {-1, -1};
  return notFound;            /* no match */
}
 
/* insert a key into the hash table */
 
void insert(Hash hashTable[MAXS][MAXC],
	    char key[], Position position, int hash)
{
  for (int i = 0; i < MAXC; i++)
    if (hashTable[hash][i].string[0] == '\0') { /* empty */
      strcpy(hashTable[hash][i].string, key);
      hashTable[hash][i].position = position;
      return;
    }
  printf("no space found");
  exit(-1);
}
 
char *getKey(Tower *tower, int row, int col)
{
  assert(tower->height[row][col] > 0);
  return (tower->string[row][col][tower->height[row][col] - 1]);  
}
 
void pairing(int pair[], Position match[], 
	     const Position exposed[], Tower *tower,
	     Hash hashTable[MAXS][MAXC])
{
  for (int i = 0; i < 2; i++) {
    pair[i] = 0;
    match[i].row = -1;
    if (tower->height[exposed[i].row][exposed[i].col] > 0) {
      char *key = getKey(tower, exposed[i].row, exposed[i].col);
      int hash = f(key, tower->N * tower->N);
      match[i] = findRemove(hashTable, key, hash);
      if (match[i].row == -1) /* no match */
	insert(hashTable, key, exposed[i], hash);
      else
	pair[i] = 1;
    }
  }
}
 
int main()
{
  Tower tower;
  getTower(&tower);
  int S = tower.N * tower.N;        /* hash table size */
 
  Hash hashTable[MAXS][MAXC];
  for (int i = 0; i < S; i++)
    for (int j = 0; j < MAXC; j++)
      hashTable[i][j].string[0] = '\0';
 
  int paired = 0;
  Position exposed[2];
  for (int row = 0; row < tower.N; row++)
    for (int col = 0; col < tower.N; col++) {
      	Position current = {row, col};
      char *key = getKey(&tower, row, col);
      int hash = f(key, tower.N * tower.N);
      Position match = findRemove(hashTable, key, hash);
      if (match.row == -1)    /* not found */ 
	insert(hashTable, key, current, hash);
      else {
	paired++;
	exposed[0] = match;
	exposed[1] = current;
      }
    }
  assert(paired <= 1);
 
  /* there will be a pair to remove at exposed1 and exposed2 */
  while (paired > 0) {
    paired = 0;
    char *key[2];
    for (int i = 0; i < 2; i++) {
      key[i] = getKey(&tower, exposed[i].row, exposed[i].col);
    }
    assert(strcmp(key[0], key[1]) == 0);
    printf("%s\n", key[0]);
    for (int i = 0; i < 2; i++) 
      tower.height[exposed[i].row][exposed[i].col]--;
 
    if (tower.height[exposed[0].row][exposed[0].col] > 0 &&
	tower.height[exposed[1].row][exposed[1].col] > 0 &&
	getKey(&tower, exposed[0].row, exposed[0].col) ==
	getKey(&tower, exposed[1].row, exposed[1].col)) {
      paired = 1;
      continue;
    }
 
    int pair[2];
    Position match[2];
    pairing(pair, match, exposed, &tower, hashTable);
    paired = pair[0] + pair[1];
    assert(paired <= 1);
    if (paired > 0) {
      if (pair[0] == 1) 
	exposed[1] = match[0];
      else {
	assert(pair[1] == 1);
	exposed[0] = match[1];
      }
    }
  }
  return 0;
}
