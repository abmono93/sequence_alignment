#include <iostream>
#include <vector>
#include <string>

#define UP 1
#define LEFT 2
#define DIAG 3

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

bool match(char X, char Y){
	switch (X){
		case 'A': return Y == 'T';
		case 'T': return Y == 'A';
		case 'C': return Y == 'G';
		case 'G': return Y == 'C';
	}
	return false;
}

void setNeighbors(table& scores, int i, int j, int *up, int *left, int *diag){
	*up = (i == 0) ? 0 : scores[i - 1][j];
	*left = (j == 0) ? 0 : scores[i][j - 1];
	*diag = (j == 0 || i == 0) ? 0 : scores[i - 1][j - 1];
}

void setScore(table& scores, char X, char Y, int i, int j){
	int up, left, diag;
	setNeighbors(scores, i, j, &up, &left, &diag);

	scores[i][j] = match(X, Y)? diag + 1 : max(up, left);
}

void fillTable(sequence& seq1, sequence& seq2, table& scores){
	for (int i = 0; i < scores.n; i++){
		for (int j = 0; j < scores.m; j++){
			setScore(scores, seq1[i], seq2[j], i, j);
		}
	}
}

int getDir(table& scores, int i, int j){
	if (i < 0) return LEFT;
	if (j < 0) return UP;
	int up, left, diag, maxStep;

	setNeighbors(scores, i, j, &up, &left, &diag);
	
	maxStep = max(up, left);
	maxStep = max(maxStep, diag);

	if (scores[i][j] != diag && maxStep == diag) return DIAG;
	if (maxStep == left) return LEFT;
	return UP;
}

void adjustSequences(sequence& seq1, sequence& seq2, table& scores){
	int dir;
	int i = scores.n - 1;
	int j = scores.m - 1;

	while (i >= 0 || j >= 0){
		dir = getDir(scores, i, j);
		switch (dir){
			case UP:
				seq2.insert(seq2.begin() + j + 1, '_');
				i--;
				break;
			case LEFT:
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
	for (int i = 0; i < seq1.size(); i++) cout << seq1[i] << ' ';
	cout << endl;
	for (int i = 0; i < seq2.size(); i++) cout << seq2[i] << ' ';
	cout << endl;
}

void align(sequence seq1, sequence seq2){
	int n = seq1.size();
	int m = seq2.size();
	table scores(n, m);
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
