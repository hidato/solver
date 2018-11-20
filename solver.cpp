#include <iostream>
#include <sstream>
#include <iterator>
#include <vector>

using namespace std;

struct node
{
	int val; // ���� ����� ��
	unsigned char neighbors;  // �������� �̿��� �ִ��� ���� 8�ڸ� 2���� ex) 11000001 0���� 1��° ���� 1�̸� dx[i],dy[i]�̿����� �ִٴ¶�
};   // ������ �Ѻ�� ����ü

int wid, hei, max, dx[8], dy[8]; //wid : x, hei : y, max : �ִ밪, dx,dy�̿���ġ 8��������
node* arr; //������ ���� �迭
bool* known; //�ڽ��� �̹� �˰��ִ°��� index = true�� �����ϴ� �迭

void findStart(int& x, int& y);
unsigned char getNeighbors(int x, int y);

bool search(int x, int y, int w) // wã����, x,y=(w-1)�� ��ǥ��
{


	node* n = &arr[x + y * wid];
	n->neighbors = getNeighbors(x, y);
	if (known[w]) //ã�����ϴ� ��w�� ��ġ�� �˰�������
	{
		for (int d = 0; d < 8; d++)
		{
			if (n->neighbors & (1 << d))
			{
				int a = x + dx[d], b = y + dy[d];
				if (arr[a + b * wid].val == w) {
					if (w == max) return true; //ã������ �ִ�

					if (search(a, b, w + 1)) return true; //w�� ��ǥ�� a,b�� �����ϰ� ����ã������ w+1�� �θ���
				}
			}
		}
		return false; //w-1�� �̿��� w�� ���� => Ǯ������
	}

	for (int d = 0; d < 8; d++)
	{
		if (n->neighbors & (1 << d))
		{
			int a = x + dx[d], b = y + dy[d];
			if (arr[a + b * wid].val == 0)
			{
				arr[a + b * wid].val = w; // w-1�� �̿��� �ϳ��� w�� �ְ�
				if (search(a, b, w + 1)) return true; // ����ã�� ���� recursive�ϰԺθ�
				arr[a + b * wid].val = 0; // ���� �ƴҰ�� backtracking
			}
		}
	}
	return false;
}

unsigned char getNeighbors(int x, int y) // node������ �̿����� ���� neighborhood�������� ����� return
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

void findStart(int& x, int& y) // ���񿡼� 1�̵�� �ִ� �������� ã����
{
	for (int b = 0; b < hei; b++)
		for (int a = 0; a < wid; a++)
			if (arr[a + wid * b].val == 1) { x = a; y = b; return; }
	x = y = -1;
}

bool solve(vector<string>& puzz, int max_wid)
{
	if (puzz.size() < 1) return true;
	wid = max_wid; hei = static_cast<int>(puzz.size()) / wid; // puzzle width, height ����
	int len = wid * hei, c = 0; max = 0; // len : ������ ũ��
	arr = new node[len]; memset(arr, 0, len * sizeof(node)); // arr�迭 ũ�⼳��, 0�ʱ�ȭ
	known = new bool[len + 1]; memset(known, 0, len + 1); // known�迭 ũ�⼳��, 0�ʱ�ȭ

	for (vector<string>::iterator i = puzz.begin(); i != puzz.end(); i++) // string���κ��� ������ ���� (. = ��ĭ = 0), (* = ĭ���� = -1)
	{
		if ((*i) == "*") { arr[c++].val = -1; continue; }
		arr[c].val = atoi((*i).c_str());
		if (arr[c].val > 0) known[arr[c].val] = true;
		if (max < arr[c].val) max = arr[c].val;
		c++;
	}

	bool ret = solveIt();
	c = 0;
	for (vector<string>::iterator i = puzz.begin(); i != puzz.end(); i++) // ���� ������ ��� string�������� ����
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
	//�׽�Ʈ �����
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