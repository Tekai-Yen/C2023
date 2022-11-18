#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>
 
#define MAXN 15
 
int min(int a, int b)
{
  return ((a < b)? a : b);
}
 
int shortestCycle(int dist[MAXN][MAXN], bool visited[MAXN], int citiesLeft,
		  int cost, int now, int N)
{
  if (citiesLeft == 0)
    return(cost + dist[now][0]);
 
  int minDistance = INT_MAX;
  for (int next = 1; next < N; next++)     /* 0 is visited */
    if (!visited[next]) {
      visited[next] = true;
      int distance = shortestCycle(dist, visited, citiesLeft - 1,
				   cost + dist[now][next], next, N);
      minDistance = min(minDistance, distance);
      visited[next] = false;
    }
  return minDistance;
}
 
int main()
{
  int N;
  assert(scanf("%d", &N) == 1);
 
  int dist[MAXN][MAXN];
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      assert(scanf("%d", &(dist[i][j])) == 1);
 
  bool visited[MAXN] = {false};
  printf("%d\n", shortestCycle(dist, visited, N - 1, 0, 0, N));
  return 0;
}
