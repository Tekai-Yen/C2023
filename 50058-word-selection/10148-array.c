#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#define LETTERS 26
#define MAXN 20
#define MAXSTRINGP1 51

void incCount(int count[LETTERS], const char *string)
{
  int length = strlen(string);
  for (int i = 0; i < length; i++)
    count[string[i] - 'a']++;
}
 
void decCount(int count[LETTERS], const char *string)
{
  int length = strlen(string);
  for (int i = 0; i < length; i++)
    count[string[i] - 'a']--;
}
 
bool ok(const int count[LETTERS])
{
  for (int i = 0; i < LETTERS; i++)
    if (count[i] == 0)
      return false;
 
  return true;
}
 
int min(int x, int y)
{
  return(x < y? x : y);
}
 
int select(const char word[MAXN][MAXSTRINGP1], const int cost[MAXN],
	   const int count[LETTERS], const int currentCost, 
	   const int wordIndex, const int N)
{
  if (ok(count))
    return currentCost;
 
  if (wordIndex == N)
    return INT32_MAX;
 
  incCount(count, word[wordIndex]);
  int selectCost = 
    select(word, cost, count, currentCost + cost[wordIndex], wordIndex + 1, N);
  decCount(count, word[wordIndex]);
  int notSelectCost = 
    select(word, cost, count, currentCost, wordIndex + 1, N);
 
  return min(selectCost, notSelectCost);
}


int main()
{
  int N;
  assert(scanf("%d", &N) == 1);
  assert(N <= MAXN);
 
  char word[MAXN][MAXSTRINGP1];
  int cost[MAXN];
  for (int i = 0; i < N; i++) 
    assert(scanf("%s%d", word[i], &(cost[i])) == 2);
 
  int count[LETTERS] = {0};
  printf("%d\n", select(word, cost, count, 0, 0, N));
 
  return 0;
}
