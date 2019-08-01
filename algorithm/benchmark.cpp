#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <time.h>

#define MAX_SIZE 197000
#define k 400
#define gap 400
using namespace std;

class Timer {
private:
	clock_t startTime, stopTime;
public:
	Timer() {
		startTime = 0;
		stopTime = 0;/**ID:2016112185 Name:박지수*/
	}
	void start() { startTime = clock(); }
	void stop() { stopTime = clock(); }
	clock_t elapsedTime() const { return (double)(stopTime - startTime)/1000; }

};

class Graph {
public:// Data members
	int     maxSize,          // Maximum number of vertices in the graph
		size;             // Actual number of vertices in the graph
	int maxLen;
	string maxCircuit;
	vector<string> vertexList;       // Vertex list
	vector<int> mat;
	vector<string> whole_circuit;
	Graph(int num) {
		size = 0;
		maxSize = num;
		for (int i = 0; i < maxSize; i++) {
			for (int j = 0; j < maxSize; j++) {
				mat.push_back(0); //이중 반복문 사용하여 인접행렬을 초기화
			}
		}
	}

	int index(string s, vector<string> submers) {
		int i;
		for (i = 0; i < submers.size(); i++) {
			if (submers[i] == s) break;
		}
		return i; //i 반환
	}

	int& edge(int row, int col)
	{
		int temp = row * maxSize + col;
		return temp;
	}

	void addVertex(string newVertex) {
		vertexList.push_back(newVertex); //새로운 vertexList 배열에 매개변수로 전달 받은 삽입할 값 저장
		for (int i = 0; i <= size; i++) {
			mat.push_back(0); //인접행렬의 새로 생긴 가로축을 초기화
			mat.push_back(0); //인접행렬의 새로 생긴 세로축을 초기화
		}
		size++; //배열의 크기를 1 증가
	}
	void addEdge(string v1, string v2, vector<string> submers) {
		int row = index(v1, submers); //index()함수에 v1을 전달하여 위치를 탐색해 row에 저장
		int col = index(v2, submers); //index()함수에 v2를 전달하여 위치를 탐색해 col에 저장

		mat[edge(row, col)]++; //edge() 함수에 row, col 전달하여 이차원 개념으로 변환한 후 mat의 위치로 설정해 weight 증가 
	}

	void showStructure()
	{
		int wt,         // Edge weight
			row, col;   // Loop counters

		if (size == 0)
			cout << "Empty graph" << endl;
		else
		{
			cout << endl << "Vertex list : " << endl;
			for (row = 0; row < size; row++)
				cout << row << '\t' << vertexList[row] << endl;

			cout << endl << "Edge matrix : " << endl << '\t';
			for (col = 0; col < size; col++)
				cout << vertexList[col] << '\t';
			cout << endl;
			for (row = 0; row < size; row++)
			{
				cout << vertexList[row] << '\t';
				for (col = 0; col < size; col++)
				{
					wt = mat[edge(row, col)];
					if (wt == 0)
						cout << "- \t";
					else
						cout << wt << '\t';
				}
				cout << endl;
			}
		}
	} //edge(이차원배열) 출력하는 함수
	int allVisited(string mer, vector<int> &adj)
	{
		int e = index(mer, vertexList);
		for (int i = 0; i < maxSize; i++) {
			if (adj[edge(e, i)] != 0)
				return 0;
		}
		return 1;
	} //모든 edge를 방문했는지 검사하는 함수
	void countDegree(vector<string> &startVertex)
	{
		for (int i = 0; i < vertexList.size(); i++) {
			int inDegree = 0, outDegree = 0;
			for (int j = 0; j < vertexList.size(); j++) {
				outDegree += mat[edge(i, j)];
				inDegree += mat[edge(j, i)];
			}
		
			if(outDegree > inDegree) //outDegree가 inDegree보다 큰 vertex가 start vertex
				startVertex.push_back(vertexList[i]);
		}
	} //start vertex를 찾기 위해 사용하는 함수
	void Eulerian(string mer, vector<int> adj, string circuit)
	{
		int e = index(mer, vertexList);
		vector<string> temp;

		for (int i = 0; i < maxSize; i++) {
			if (adj[edge(e, i)] != 0) {
				temp.push_back(vertexList[i]);
			}
		}
		vector<int> adj2;
		adj2 = adj;
		for (int i = 0; i < temp.size(); i++) {
			int m = index(temp[i], vertexList);

			adj[edge(e, m)]--;

			circuit += vertexList[m].substr(k - gap, gap);
			//cout << circuit.length() << endl;
			if (allVisited(vertexList[m], adj) == 1) {
				//cout << circuit << endl;
				cout << "." << endl;
				whole_circuit.push_back(circuit);
			}
			else
				Eulerian(vertexList[m], adj, circuit);

			adj[edge(e, m)]++;
			circuit = circuit.substr(gap);
		}
		return;
	} //오일러 경로 찾는 함수
};

void DeBruijnize(vector<string> submers, vector<string> kmers, Graph &graph)
{
	for (int i = 0; i < submers.size(); i++) {
		graph.addVertex(submers[i]);
	} // 모든 submers를 vertex에 추가

	for (int i = 0; i < kmers.size(); i++) {
		char *lsub = new char[k];
		char *rsub = new char[k];
		for (int j = 0; j < k; j++) {
			lsub[j] = kmers[i][j];
			rsub[j] = kmers[i][j + gap]; //eulerian 방식에서 vertices로 탐색하기 때문에 k-1 mer 생성
		}
		lsub[k] = '\0'; rsub[k] = '\0';
		graph.addEdge(lsub, rsub, submers);
	}
	return;
}

void ParseReads(char *genome, vector<string> &reads, string &remain)
{
	int cn = 0;
	
	for (int i = 0; i < strlen(genome); i += gap) {
		char *read = new char[k + gap];
		for (int j = 0; j < k + gap; j++) {
			read[j] = genome[i + j]; //short read로부터 k-mer 생성
		}
		read[k + gap] = '\0'; //문자열 배열의 마지막에 '\0'삽입
		if (strlen(read) == k + gap)
			reads.push_back(read);
		else {
			cn++;
			if (cn == 1) {
				remain = read;
				remain = remain.substr(k);
			}
		}
	}
} //My Genome을 길이가 k+gap인 스트링으로 분할

void ParseKmers(vector<string> reads, vector<string> &submers)
{
	for (int i = 0; i < reads.size(); i++) {
		char *lsub = new char[k];
		char *rsub = new char[k];
		for (int j = 0; j < k; j++) {
			lsub[j] = reads[i][j];
			rsub[j] = reads[i][j + gap]; //eulerian 방식에서 vertices로 탐색하기 때문에 k-1 mer 생성
		}
		lsub[k] = '\0'; rsub[k] = '\0';
		submers.push_back(lsub);
		submers.push_back(rsub);
	}

	sort(submers.begin(), submers.end());
	submers.erase(unique(submers.begin(), submers.end()), submers.end()); //벡터를 이용하여 submers의 중복제거
} //short read를 길이가 k인 스트링으로 분할

int main()
{
	char *ran = new char[MAX_SIZE];
	int tmp;
	srand((unsigned int)time(NULL));
	for (int i = 0; i < MAX_SIZE; i++) {
		tmp = rand() % 4;
		switch (tmp) {
		case 0: ran[i] = 'A'; break;
		case 1: ran[i] = 'C'; break;
		case 2: ran[i] = 'G'; break;
		case 3: ran[i] = 'T'; break;
		}
	}

	ofstream outFile("my_genome.txt");
	for (int i = 0; i < MAX_SIZE; i++) {
		outFile << ran[i];
	}
	outFile.close();
	delete ran;

	cout << "created genome file" << endl;

	//----------------------my genome 만들기----------------------/
	vector <string> reads;
	vector <string> submers;

	ifstream inputFile("my_genome.txt");
	if (!inputFile.is_open()) {
		cout << "파일이 존재하지 않습니다." << endl;
	}
	else {
		if (!inputFile.eof()) {
			char *genome = new char[MAX_SIZE];
			inputFile.getline(genome, MAX_SIZE);

			string remain;
			ParseReads(genome, reads, remain); //my genome을 short read로 분할

			ofstream outreads("ran_read.txt"); 
			random_shuffle(reads.begin(), reads.end()); //short read의 순서를 랜덤하게 섞음

			for (int i = 0; i < reads.size(); i++) {
				outreads << reads[i] << endl; //텍스트 파일에 short read 출력
			}
			
			ParseKmers(reads, submers); //short read를 분할
			cout << "parsed short reads" << endl;
	
			/*for (int i = 0; i < submers.size(); i++) {
			cout << submers[i] << endl;
			}*/

			Graph graph(submers.size());
			DeBruijnize(submers, reads, graph);
			cout << "created Debruijn Graph" << endl;
			//graph.showStructure();

			cout << "Eulerian Path" << endl;

			vector<int> adj;

			adj = graph.mat;
			string circuit;
			//vector<string> startVertex;
			//graph.countDegree(startVertex);

			//cout << "all vertices : " << graph.vertexList.size() << "   start vertex : " << startVertex.size() << endl;

			Timer T;
			T.start(); //경로 탐색 시간 측정

			for (int i = 0; i < graph.vertexList.size(); i++) {
				circuit = graph.vertexList[i];
				graph.Eulerian(graph.vertexList[i], adj, circuit);
			}
			T.stop(); //경로 탐색 종료
			cout << "elapsed time : " << T.elapsedTime() << endl; //경과 시간:초단위

			/*for (int i = 0; i < graph.vertexList.size(); i++) {
			circuit = graph.vertexList[i];
			graph.Eulerian(graph.vertexList[i], adj, circuit);
			}*/

			string origin = graph.whole_circuit[0];
			for (int i = 0; i < graph.whole_circuit.size(); i++) {
				if (origin.length() < graph.whole_circuit[i].length())
					origin = graph.whole_circuit[i];
			} //가장 긴 path가 original genome과 비슷하다고 판단(길이가 길어질수록 가능한 path는 거의 하나)

			origin = origin + remain;

			//cout << "original sequence" << endl << origin << endl;
			cout << "reconstructed my genome" << endl;

			int mismatch = 0;
			for (int i = 0; i < MAX_SIZE; i++) {
				if (genome[i] != origin[i])
					mismatch++;
			} //original sequence와 reconstructed sequence 비교

			cout << "number of mismatch : " << mismatch << endl;
			
			ofstream output("reconstructed_genome.txt");
			output << origin << endl;

			delete[] genome;
		}
		inputFile.close();
	}

	return 0;
}
