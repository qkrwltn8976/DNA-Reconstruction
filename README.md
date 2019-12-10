# DNA-Reconstruction
eulerian path 알고리즘을 활용한 DNA sequence reconstruction

## 문제 정의
컴퓨터 알고리즘 프로젝트는 생물이 가지는 유전체, 즉 유전 정보를 담고있는 DNA sequence를 활용하 여 DNA를 reconstruct하는 것이다. 
프로젝트를 진행하기 위해서는 알파벳 ATGC로 이루어진 My genome으로부터 생성된 short reads가 필요하다. 
이러한 short reads로부터 원래의 My genome을 복 구하는 것이 이번 프로젝트의 주요 문제이다. 
My genome은 rand() 함수을 사용하여 데이터를 랜덤하게 생성하여 사용을 하였다. 
인간의 genome은 약 30억 개로 이루어져 있지만 30억 개를 모두 활용할 경우 시간이 오래 걸리고 데이터를 정확하게 찾았는지 알 수 없기 때문에 크기를 점점 늘려가고 short read의 길이는 줄여가면서 프로젝트를 진행하였다.

## 데이터 설명
My genome의 길이: 197000
Short Read의 길이: 800
Short Read의 개수: 492
Short Read로부터 분할한 Short Node의 길이: 400

## 알고리즘
선택한 알고리즘은 Denovo 방식의 그래프의 eulerian path 알고리즘이다. 
eulerian path 방식은 short read 를 k-mer 로 작게 나눈 후 그래프의 노드가 아닌 간선을 방문하는 경로를 만들기 위해 k-1-mer 로 다시 나누어 debruijn 그래프를 생성을 하고 경로를 탐색한다. 
하지만 k-1- mer로 분할을 할 경우 간선의 길이가 너무 길고 sequence의 길이가 1씩 증가되면서 복구되기 때문에 효율성이 떨어진다고 판단을 하였다. 
따라서 이를 응용하여 하나의 short read 를 k-mer 이라 설정을 하고 (k-1)-mer 가 아닌 중복되는 부분을 더 줄여 path 를 탐색할 때마다 복구되는 DNA sequence 가 더 길도록 코드를 작성하였다.

1. My genome으로부터 동일한 길이의 short reads를 중복되는 부분이 존재하도록 분할을 하고
short reads의 길이를 섞는다.
2. Short reads를 L-mer과 R-mer로 분할을 하여 각각을 그래프의 vertex로 만든다.
3. Short reads를 다시 탐색하면서 방문한 edge를 이차원배열에 저장하여 Directed Debruijn Graph를 생성한다.
4. OutDegree > InDegree조건을 사용하여 Eulerian Path의 시작 Vertex를 찾고 Debruijn Graph를 Eulerian Path 방식으로 탐색을 한다.
5. 여러 경로 중 가장 긴 경로를 Reconstructed DNA sequence로 설정을 한다.
(sequence의 길이가 길어질수록 길이가 동일한 경로는 줄어들고 최종적으로 1가지밖에 나오지않아 길이를 비교하지 않아도 된다.)
