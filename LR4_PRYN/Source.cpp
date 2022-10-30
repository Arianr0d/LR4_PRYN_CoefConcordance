#include <iostream>;
#include <sstream>;
#include <fstream>;
#include <vector>;
#include <algorithm>;
#include <string>;
#include <cmath>;

using namespace std;

/* 
	����� ������������ ����������� ��� 7 ��������� � 10 ����������� ��� ������ �� �����. 
	����������� ������ 6 ��������� � �������� ������������� ������� � ����������� ����������� ��� ��.
*/

// ������� ������ ���������� � ����
void output(float full_result, pair<float, int> &result, vector<pair<float, int>> &matrix_result) {

	ofstream out;
	out.open("output.txt");

	sort(matrix_result.begin(), matrix_result.end(),
		[](const auto& x, const auto& y) { return x.second < y.second; });
	
	if (out.is_open()) {
		out << "�����:";
		for (int i = 0; i < matrix_result.size(); i++) {
			out << "\n��� �������� e" << matrix_result[i].second << ", R" << matrix_result[i].second << " = ";
			out << round(matrix_result[i].first * 1000) / 1000 << ";";
		}
		out << "\n\n����������� ����������� ���� ������ R = "; 
		out << round(full_result * 1000) / 1000;
		out << ", �������� ������������� ������ � ��� �������� e" + to_string(result.second) + ", R" + to_string(result.second) + " = ";
		out << round(result.first * 1000) / 1000;
	}
}

// ������� �������� ������������ �����������
void findCoef�oncordance(
	vector<vector<int>>& matrix,
	float& R) {

	int n = matrix.size();
	int m = matrix[0].size();
	vector<float> summary;

	float sum_summary = 0.0;
	// ����� ����� ���� ����� �� ������� �������
	for (int i = 0; i < m; i++)
	{
		float sum = 0.0;
		for (int j = 0; j < n; j++)
		{
			sum += matrix[j][i];
		}
		summary.push_back(sum);
		sum_summary += sum;
	}

	float S = 0.0;
	// ����� ���������� �� �������� �������� (���)
	for (int i = 0; i < summary.size(); i++)
	{
		S += pow(summary[i] - (sum_summary / summary.size()), 2);
	}

	// ����� ������������ �����������
	R = (12.0 * S) / (n * n * (pow(m, 3) - m));
}

// ������� ������ ������������� ����������� ��� ������ ������
void findCoef�oncordanceAllGroup(
	vector<vector<int>> &matrix_start,
	vector<pair<float, int>> &matrix_result,
	pair<float, int> &result,
	float &full_result) {

	// ����� ������������ ����������� ��� ���� ���������
	findCoef�oncordance(matrix_start, ref(full_result));

	for (auto i = 0; i < matrix_start.size(); i++)
	{
		vector<vector<int>> copy_start_matrix(matrix_start.size(), vector<int>(matrix_start.front().size()));
		copy(matrix_start.begin(), matrix_start.end(), copy_start_matrix.begin());
		// ���������� ������ i-��� �������� �� �������
		copy_start_matrix.erase(copy_start_matrix.cbegin() + i);

		float R;
		// ����� ������������ �����������
		findCoef�oncordance(copy_start_matrix, ref(R));
		matrix_result.push_back(make_pair(R, i + 1));
	}

	// ���������� �� �������� �������� ������������ �����������
	sort(matrix_result.begin(), matrix_result.end());
	reverse(matrix_result.begin(), matrix_result.end());
	result = matrix_result.front();
}

// ������� ���������� ������� �� �����
void importFromFile(
	int& n,
	int& m,
	vector<vector<int>>& matrix) {

	string str;
	ifstream in("input.txt");

	if (in.is_open()) {
		int i = 0, k = 0;
		while (getline(in, str)) {

			if (str.size() == 0) continue;

			switch (k) {
			case 0:
				n = stoi(str); // ����� ���������
				break;
			case 1:
				m = stoi(str);  // ����� �����������
				break;

			default:
				string row;
				istringstream ist(str);
				int j = 0;

				while (ist >> row) {
					matrix[i][j] = stoi(row);
					j++;
				}
				i++;
				break;
			}
			k++;
		}
	}
	in.close();
}

int main() {

	setlocale(LC_ALL, "ru");
		
	int n = 7,		// ����� ���������
		m = 10;		// ����� �����������

	vector<vector<int>> A(n, vector<int>(m));

	cout << "����� ������������ ����������� ��� 7 ��������� � 10 ����������� ��� ������ �� �����.\n" <<
		"����������� ������ 6 ��������� � �������� ������������� ������� � ����������� ����������� ��� ��." << endl << endl;
	cout << "���������� ������ �� �����!" << endl;

	// ���������� ������ �� �����
	importFromFile(n, m, ref(A));

	vector<pair<float, int>> matrix_result;
	pair<float, int> result;
	float full_result;
	// ������� ������������� ����������� ��� ���� �����
	findCoef�oncordanceAllGroup(A, ref(matrix_result), ref(result), ref(full_result));

	// ������ ���������� � ����
	output(full_result, result, matrix_result);

	cout << "���������� ������� ���������!" << endl;
	system("pause");
}