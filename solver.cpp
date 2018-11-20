#include <iostream>
#include <sstream>
#include <iterator>
#include <vector>

using namespace std;

struct node
{
	int val; // 퍼즐 블록의 값
	unsigned char neighbors;  // 퍼즐블록의 이웃이 있는지 저장 8자리 2진수 ex) 11000001 0부터 1번째 수가 1이면 dx[i],dy[i]이웃값이 있다는뜻
};   // 퍼즐의 한블록 구조체

int wid, hei, max, dx[8], dy[8]; //wid : x, hei : y, max : 최대값, dx,dy이웃위치 8방향저장
node* arr; //퍼즐을 만들 배열
bool* known; //자신이 이미 알고있는값의 index = true로 저장하는 배열

void findStart(int& x, int& y);
unsigned char getNeighbors(int x, int y);

bool search(int x, int y, int w) // w찾을값, x,y=(w-1)의 좌표값
{


	node* n = &arr[x + y * wid];
	n->neighbors = getNeighbors(x, y);
	if (known[w]) //찾고자하는 값w의 위치를 알고있을때
	{
		for (int d = 0; d < 8; d++)
		{
			if (n->neighbors & (1 << d))
			{
				int a = x + dx[d], b = y + dy[d];
				if (arr[a + b * wid].val == w) {
					if (w == max) return true; //찾을값이 최대

					if (search(a, b, w + 1)) return true; //w의 좌표를 a,b에 저장하고 다음찾을값인 w+1을 부른다
				}
			}
		}
		return false; //w-1의 이웃에 w가 없다 => 풀수없음
	}

	for (int d = 0; d < 8; d++)
	{
		if (n->neighbors & (1 << d))
		{
			int a = x + dx[d], b = y + dy[d];
			if (arr[a + b * wid].val == 0)
			{
				arr[a + b * wid].val = w; // w-1의 이웃중 하나에 w를 넣고
				if (search(a, b, w + 1)) return true; // 다음찾을 값을 recursive하게부름
				arr[a + b * wid].val = 0; // 답이 아닐경우 backtracking
			}
		}
	}
	return false;
}

unsigned char getNeighbors(int x, int y) // node에대한 이웃값을 위의 neighborhood형식으로 만들어 return
{
	unsigned char c = 0; int m = -1, a, b;
	for (int yy = -1; yy < 2; yy++)
		for (int xx = -1; xx < 2; xx++)
		{
			if (!yy && !xx) continue;
			m++; a = x + xx, b = y + yy;
			if (a < 0 || b < 0 || a >= wid || b >= hei) continue;
			if (arr[a + b * wid].val > -1) c |= (1 << m);
		}
	return c;
}

bool solveIt()
{
	int x, y; findStart(x, y);
	if (x < 0) { cout << "\nCan't find start point!\n"; return false; }
	return search(x, y, 2);
}

void findStart(int& x, int& y) // 퍼즐에서 1이들어 있는 시작점을 찾아줌
{
	for (int b = 0; b < hei; b++)
		for (int a = 0; a < wid; a++)
			if (arr[a + wid * b].val == 1) { x = a; y = b; return; }
	x = y = -1;
}

bool solve(vector<string>& puzz, int max_wid)
{
	if (puzz.size() < 1) return true;
	wid = max_wid; hei = static_cast<int>(puzz.size()) / wid; // puzzle width, height 설정
	int len = wid * hei, c = 0; max = 0; // len : 퍼즐의 크기
	arr = new node[len]; memset(arr, 0, len * sizeof(node)); // arr배열 크기설정, 0초기화
	known = new bool[len + 1]; memset(known, 0, len + 1); // known배열 크기설정, 0초기화

	for (vector<string>::iterator i = puzz.begin(); i != puzz.end(); i++) // string으로부터 퍼즐을 만듬 (. = 빈칸 = 0), (* = 칸없음 = -1)
	{
		if ((*i) == "*") { arr[c++].val = -1; continue; }
		arr[c].val = atoi((*i).c_str());
		if (arr[c].val > 0) known[arr[c].val] = true;
		if (max < arr[c].val) max = arr[c].val;
		c++;
	}

	bool ret = solveIt();
	c = 0;
	for (vector<string>::iterator i = puzz.begin(); i != puzz.end(); i++) // 만들어낸 퍼즐을 출력 string형식으로 저장
	{
		if ((*i) == ".")
		{
			ostringstream o; o << arr[c].val;
			(*i) = o.str();
		}
		c++;
	}
	delete[] arr;
	delete[] known;

	return ret;
}

int main(int argc, char* argv[])
{
	dx[0] = -1; dx[1] = 0; dx[2] = 1; dx[3] = -1; dx[4] = 1; dx[5] = -1; dx[6] = 0; dx[7] = 1;
	dy[0] = -1; dy[1] = -1; dy[2] = -1; dy[3] = 0; dy[4] = 0; dy[5] = 1; dy[6] = 1; dy[7] = 1;

	int wid;
	//테스트 퍼즐들
	//string p = ". 33 35 . . * * * . . 24 22 . * * * . . . 21 . . * * . 26 . 13 40 11 * * 27 . . . 9 . 1 * * * . . 18 . . * * * * * . 7 . . * * * * * * 5 ."; wid = 8;
	//string p = "54 . 60 59 . 67 . 69 . . 55 . . 63 65 . 72 71 51 50 56 62 . * * * * . . . 14 * * 17 . * 48 10 11 * 15 . 18 . 22 . 46 . * 3 . 19 23 . . 44 . 5 . 1 33 32 . . 43 7 . 36 . 27 . 31 42 . . 38 . 35 28 . 30"; wid = 9;
	//string p = ". 58 . 60 . . 63 66 . 57 55 59 53 49 . 65 . 68 . 8 . . 50 . 46 45 . 10 6 . * * * . 43 70 . 11 12 * * * 72 71 . . 14 . * * * 30 39 . 15 3 17 . 28 29 . . 40 . . 19 22 . . 37 36 . 1 20 . 24 . 26 . 34 33"; wid = 9;
	string p = "8 . . . . . 45 . 1 . 3 . 47 . . 11 . . . . 49 12 . . 24 . . 40 . . 25 . . . 39 . . . . 19 . 35 . . 16 . . . ."; wid = 7;
	//string p = "* 1 * * 14 . 3 * . * . 6 . * 5 . . . . * 10 * * *"; wid = 4;
	//string p = "5 1 * . 3 ."; wid = 3;

	istringstream iss(p); vector<string> puzz;
	copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter<vector<string> >(puzz));

	int c = 0;
	cout << "hidato puzzle" << endl << endl;
	for (vector<string>::iterator i = puzz.begin(); i != puzz.end(); i++)
	{
		if ((*i) == ".")
			cout << "00 ";
		else if ((*i) != "*" && (*i) != ".")
		{
			if (atoi((*i).c_str()) < 10) cout << "0";
			cout << (*i) << " ";
		}
		else cout << "   ";
		if (++c >= wid) { cout << endl; c = 0; }
	}
	cout << endl << endl;
	if (solve(puzz, wid)) {

		c = 0;
		cout << "solution" << endl << endl;
		for (vector<string>::iterator i = puzz.begin(); i != puzz.end(); i++)
		{
			if ((*i) != "*" && (*i) != ".")
			{
				if (atoi((*i).c_str()) < 10) cout << "0";
				cout << (*i) << " ";
			}
			else cout << "   ";
			if (++c >= wid) { cout << endl; c = 0; }
		}
		cout << endl << endl;
	}

	else {
		cout << "No solution" << endl << endl;
	}


	return system("pause");
}