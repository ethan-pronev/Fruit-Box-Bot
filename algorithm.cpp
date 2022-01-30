#include <bits/stdc++.h>
using namespace std;



// algorithm 1: brute force
pair<vector<vector<int>>,int> algo1(vector<vector<int>> grid) {
	vector<vector<int>> moves;
	int score=0;

	for (int t=0; t<20; t++) {
		for (int x1=0; x1<17; x1++) {
			for (int x2=x1; x2<17; x2++) {
				for (int y1=0; y1<10; y1++) {
					for (int y2=y1; y2<10; y2++) {
						int sum=0;
						for (int i=x1; i<=x2; i++) {
							for (int j=y1; j<=y2; j++) {
								sum+=grid[j][i];
								if (sum>10) break;
							}
							if (sum>10) break;
						}
						if (sum!=10) continue;

						moves.push_back({x1,y1,x2,y2});
						for (int i=x1; i<=x2; i++)
							for (int j=y1; j<=y2; j++) {
								if (grid[j][i]!=0) score++;
								grid[j][i]=0;
							}
					}
				}
			}
		}
	}

	/*for (int i=0; i<10; i++) {
		for (int j=0; j<17; j++) {
			if (grid[i][j]!=0) cout << grid[i][j] << " ";
			else cout << "  ";
		}
		cout << endl;
	}
	cout << score << endl;*/

	return {moves,score};
}



// algorithm 2: bfs
struct State {
	string board;
	int score;
};
struct Parent {
	string parentBoard;
	vector<int> lastMove; // length 4
};
bool operator<(const State &a, const State &b) { return a.board < b.board; }

int TIMEOUT = 70;
int BRUTE_FORCE_CUTOFF = 135;
int PARITY_COUNT = 2; //will only branch after a chain of this many moves

pair<vector<vector<int>>,int> algo2(vector<vector<int>> grid) {
	const auto startTime = std::chrono::system_clock::now();

	vector<vector<int>> moves;
	int score=0;

	map<State,bool> visited;
	map<string,Parent> backtrack;

	int bestScore=0;
	string bestBoard; // the final state

	stack<pair<State,int>> q; //int is the parity tracker - every other level, heuristically choose only the first move that you find instead of all possible moves
	q.push({State{string(170,'1'),0}, 1});
	while (!q.empty()) {
		const auto nowTime = std::chrono::system_clock::now();
		if (std::chrono::duration<double>(nowTime - startTime).count() > TIMEOUT) break;

		State current = q.top().first;
		int parity = q.top().second;
		q.pop();


		if (visited.find(current) != visited.end()) continue;
		visited[current] = true;

		if (current.score > bestScore) {
			bestScore = current.score;
			bestBoard = current.board;

			//cout << current.score << endl;
		}


		for (int r1=0; r1<grid.size(); r1++) {
			for (int r2=r1; r2<grid.size(); r2++) {
				for (int c1=0; c1<grid[0].size(); c1++) {
					for (int c2=c1; c2<grid[0].size(); c2++) {
						int sum=0;
						for (int i=r1; i<=r2; i++) {
							for (int j=c1; j<=c2; j++) {
								sum += grid[i][j] * ( current.board[i*grid[0].size()+j] - '0');
								if (sum>10) break;
							}
							if (sum>10) break;
						}
						if (sum!=10) continue;

						State next = current;
						for (int i=r1; i<=r2; i++) {
							for (int j=c1; j<=c2; j++) {
								if (current.board[i*grid[0].size()+j]=='1') {
									next.score++;
									next.board[i*grid[0].size()+j]='0';
								}
							}
						}

						backtrack[next.board]=Parent{current.board,vector<int>{c1,r1,c2,r2}};
						q.push({next,(parity+1)%PARITY_COUNT});

						if (parity>0 && PARITY_COUNT>1) goto endLoop; //check parity count to ensure this never goes off when its one (ie. on the first move)
					}
				}
			}
		}
		endLoop:;
	}

	string backtrackBoard = bestBoard;
	while (backtrack.find(backtrackBoard) != backtrack.end()) {
		moves.push_back(backtrack[backtrackBoard].lastMove);
		backtrackBoard = backtrack[backtrackBoard].parentBoard;
	}

	vector<vector<int>> movesFinal; //to reverse moves since they are in the wrong direction
	for (int i=moves.size()-1; i>=0; i--) movesFinal.push_back(moves[i]);

	return {movesFinal, bestScore};
}



// primary main() function for bot use
int main() {
	vector<vector<int>> grid;

	// read input grid
	fstream inputFile;
	inputFile.open("input-grid");
	for (int i=0; i<10; i++) {
		grid.push_back(vector<int>{});
		for (int j=0; j<17; j++) {
			//int x; inputFile >> x;
			int x = rand()%9 + 1;
			grid[i].push_back(x);
		}
	}
	inputFile.close();

	// algorithm here
	pair<vector<vector<int>>,int> result = algo1(grid);
	if (result.second >= BRUTE_FORCE_CUTOFF) result = algo2(grid); // only if the fast brute force method gets a high score, run the slow bfs method to try and get even higher

	// output move list to file
	fstream outputFile;
	outputFile.open("output-moves", ofstream::out | ofstream::trunc); // trunc to delete previous contents
	outputFile << result.second << endl; // print the game score to the first line of file
	for (int i=0; i<result.first.size(); i++) outputFile << result.first[i][0] << " " << result.first[i][1] << " " << result.first[i][2] << " " << result.first[i][3] << endl;
	outputFile.close();
}



// for comparing different PARITY_COUNT values
int main2() {
	srand (time(NULL));
	int gameCount=0;
	while (gameCount<=20) {
		vector<vector<int>> grid;

		// generate random grid
		for (int i=0; i<10; i++) {
			grid.push_back(vector<int>{});
			for (int j=0; j<17; j++) {
				int x = rand()%9 + 1;
				grid[i].push_back(x);
			}
		}

		pair<vector<vector<int>>,int> result = algo1(grid);
		if (result.second<140) continue;
		gameCount++; cout << "game " << gameCount << "(" << result.second << ") - ";
		for (int i=1; i<=4; i++) {
			PARITY_COUNT=i;
			result = algo2(grid);
			cout << i << ": " << result.second << " ";
		}
		cout << endl;
	}
}



// for calculating good brute-force game probability
int main3() {
	srand (time(NULL));
	int aboveCutoff=0, total=0;
	int CUTOFF_VALUE=140;

	while (aboveCutoff<=100) {
		vector<vector<int>> grid;

		// generate random grid
		for (int i=0; i<10; i++) {
			grid.push_back(vector<int>{});
			for (int j=0; j<17; j++) {
				int x = rand()%9 + 1;
				grid[i].push_back(x);
			}
		}

		pair<vector<vector<int>>,int> result = algo1(grid);
		total++;
		if (result.second<CUTOFF_VALUE) continue;
		aboveCutoff++;
		cout << "|";
	}
	cout << endl;
	cout << "above " << CUTOFF_VALUE << ": " << aboveCutoff << endl;
	cout << "total: " << total << endl;
}
