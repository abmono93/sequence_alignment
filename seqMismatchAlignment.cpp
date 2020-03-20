#include <iostream>
#include <vector>
#include <string>

#define LEFT 1
#define UP 2
#define DIAG 3

#define MATCH_SCORE 0
#define SPACE_PENALTY 4
#define G1_PENALTY 6
#define G2_PENALTY 7
#define G3_PENALTY 10

using namespace std;

struct table : vector<vector<int> > {
	int m;
	int n;
	table(int n, int m);
};

table::table(int n, int m) : vector<vector<int> >(n, vector<int>(m)){
	this->n = n;
	this->m = m;  
}

struct sequence : vector<char> {};

int score(char X, char Y){
	switch (X){
		case 'A':
			switch(Y){
				case 'A': return G3_PENALTY;
				case 'T': return MATCH_SCORE;
				case 'C': return G2_PENALTY;
				case 'G': return G3_PENALTY;
			}
		case 'T':
			switch(Y){
				case 'A': return MATCH_SCORE;
				case 'T': return G1_PENALTY;
				case 'C': return G1_PENALTY;
				case 'G': return G2_PENALTY;
			}
		case 'C':
			switch(Y){
				case 'C': return G1_PENALTY;
				case 'G': return MATCH_SCORE;
				case 'T': return G1_PENALTY;
				case 'A': return G2_PENALTY;
			}
		case 'G':
			switch(Y){
				case 'G': return G3_PENALTY;
				case 'C': return MATCH_SCORE;
				case 'T': return G2_PENALTY;
				case 'A': return G3_PENALTY;
			}
	}
	return -20;
}

void setNeighbors(table& scores, int i, int j, int *left, int *up, int *diag){
	*left = scores[i - 1][j];
	*up = scores[i][j - 1];
	*diag = scores[i - 1][j - 1];
}

void setScore(table& scores, char X, char Y, int i, int j){
	int left, up, diag;
	setNeighbors(scores, i, j, &left, &up, &diag);

	scores[i][j] = min(left, up) + SPACE_PENALTY;
	scores[i][j] = min(scores[i][j], diag + score(X, Y));
}

void initFirstRowAndCol(table& scores){
	scores[0][0] = 0;
	for (int i = 1; i < scores.n; i++) scores[i][0] = i * SPACE_PENALTY;
	for (int j = 1; j < scores.m; j++) scores[0][j] = j * SPACE_PENALTY;
}

void fillTable(sequence& seq1, sequence& seq2, table& scores){
	initFirstRowAndCol(scores);
	for (int i = 1; i < scores.n; i++){
		for (int j = 1; j < scores.m; j++){
			setScore(scores, seq1[i], seq2[j], i, j);
		}
	}
}

int getDir(table& scores, int i, int j){
	if (i < 1) return UP;
	if (j < 1) return LEFT;
	int left, up, diag, minStep, dir;

	setNeighbors(scores, i, j, &left, &up, &diag);

	minStep = min(left, up);
	minStep = min(minStep, diag);

	if (scores[i][j] - left == SPACE_PENALTY) dir = LEFT;
	else if (scores[i][j] - up == SPACE_PENALTY) dir = UP;
	else dir = DIAG;

	if (minStep == up) dir = UP;
	if (minStep == left) dir = LEFT;
	return dir;

}

void adjustSequences(sequence& seq1, sequence& seq2, table& scores){
	int dir;
	int i = scores.n - 1;
	int j = scores.m - 1;

	while (i >= 1 || j >= 1){
		switch (getDir(scores, i, j)){
			case LEFT:
				seq2.insert(seq2.begin() + j + 1, '_');
				i--;
				break;
			case UP:
				seq1.insert(seq1.begin() + i + 1, '_');
				j--;
				break;
			case DIAG:
				i--;
				j--;
		}
	}
}

void printSequences(sequence& seq1, sequence& seq2){
	for (int i = 1; i < seq1.size(); i++) cout << seq1[i] << ' ';
	cout << endl;
	for (int i = 1; i < seq2.size(); i++) cout << seq2[i] << ' ';
	cout << endl;
}

void align(sequence seq1, sequence seq2){
	seq1.insert(seq1.begin(), 'Z');
	seq2.insert(seq2.begin(), 'Z');
	table scores(seq1.size(), seq2.size());
	fillTable(seq1, seq2, scores);
	adjustSequences(seq1, seq2, scores);
	printSequences(seq1, seq2);
}

void readSeq(string& line, sequence& seq){
	for (char X : line){
		if (!isspace(X)) seq.push_back(X);
	}
}

int main(){
	string line;
	sequence seq1, seq2;

	getline(cin, line);
	readSeq(line, seq1);
	getline(cin, line);
	readSeq(line, seq2);

	align(seq1, seq2);
	
}
