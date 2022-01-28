#include <bits/stdc++.h>
using namespace std;

int main() {
	vector<vector<int>> grid;

	fstream inputFile;
	inputFile.open("input-grid");
	for (int i=0; i<10; i++) {
		grid.push_back(vector<int>{});
		for (int j=0; j<17; j++) {
			int x; inputFile >> x;
			grid[i].push_back(x);
		}
	}
	inputFile.close();

	//algorithm here
	vector<vector<int>> moves;

	fstream outputFile;
	outputFile.open("output-moves", ofstream::out | ofstream::trunc); //trunc to delete previous contents

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
						outputFile << x1 << " " << y1 << " " << x2 << " " << y2 << endl;
						for (int i=x1; i<=x2; i++)
							for (int j=y1; j<=y2; j++)
								grid[j][i]=0;
					}
				}
			}
		}
	}

	outputFile.close();

	for (int i=0; i<10; i++) {
		for (int j=0; j<17; j++) {
			if (grid[i][j]!=0) cout << grid[i][j] << " ";
			else cout << "  ";
		}
		cout << endl;
	}
}
