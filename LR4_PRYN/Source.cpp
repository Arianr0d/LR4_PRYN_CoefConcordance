#include <iostream>;
#include <sstream>;
#include <fstream>;
#include <vector>;
#include <algorithm>;
#include <string>;
#include <cmath>;

using namespace std;

/* 
	Поиск коэффициента конкордации для 7 экспертов и 10 альтернатив для каждой из групп. 
	Определение группы 6 экспертов с наиболее согласованным мнением и коэффициент конкордации для неё.
*/

// функция записи результата в файл
void output(float full_result, pair<float, int> &result, vector<pair<float, int>> &matrix_result) {

	ofstream out;
	out.open("output.txt");

	sort(matrix_result.begin(), matrix_result.end(),
		[](const auto& x, const auto& y) { return x.second < y.second; });
	
	if (out.is_open()) {
		out << "Ответ:";
		for (int i = 0; i < matrix_result.size(); i++) {
			out << "\nбез эксперта e" << matrix_result[i].second << ", R" << matrix_result[i].second << " = ";
			out << round(matrix_result[i].first * 1000) / 1000 << ";";
		}
		out << "\n\nкоэффициент конкордации всей группы R = "; 
		out << round(full_result * 1000) / 1000;
		out << ", наиболее согласованная группа – без эксперта e" + to_string(result.second) + ", R" + to_string(result.second) + " = ";
		out << round(result.first * 1000) / 1000;
	}
}

// функция рассчёта коэффициента конкордации
void findCoefСoncordance(
	vector<vector<int>>& matrix,
	float& R) {

	int n = matrix.size();
	int m = matrix[0].size();
	vector<float> summary;

	float sum_summary = 0.0;
	// поиск суммы всех строк по каждому столбцу
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
	// поиск отклонения от среднего значения (СКО)
	for (int i = 0; i < summary.size(); i++)
	{
		S += pow(summary[i] - (sum_summary / summary.size()), 2);
	}

	// поиск коэффициента конкордации
	R = (12.0 * S) / (n * n * (pow(m, 3) - m));
}

// функция поиска коэффициентов конкордации для каждой группы
void findCoefСoncordanceAllGroup(
	vector<vector<int>> &matrix_start,
	vector<pair<float, int>> &matrix_result,
	pair<float, int> &result,
	float &full_result) {

	// поиск коэффициента конкордации для всех экспертов
	findCoefСoncordance(matrix_start, ref(full_result));

	for (auto i = 0; i < matrix_start.size(); i++)
	{
		vector<vector<int>> copy_start_matrix(matrix_start.size(), vector<int>(matrix_start.front().size()));
		copy(matrix_start.begin(), matrix_start.end(), copy_start_matrix.begin());
		// исключение строки i-ого эксперта из таблицы
		copy_start_matrix.erase(copy_start_matrix.cbegin() + i);

		float R;
		// поиск коэффициента конкордации
		findCoefСoncordance(copy_start_matrix, ref(R));
		matrix_result.push_back(make_pair(R, i + 1));
	}

	// сортировка по убыванию значения коэффициента конкордации
	sort(matrix_result.begin(), matrix_result.end());
	reverse(matrix_result.begin(), matrix_result.end());
	result = matrix_result.front();
}

// функция считывания матрицы из файла
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
				n = stoi(str); // число экспертов
				break;
			case 1:
				m = stoi(str);  // число альтернатив
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
		
	int n = 7,		// число экспертов
		m = 10;		// число альтернатив

	vector<vector<int>> A(n, vector<int>(m));

	cout << "Поиск коэффициента конкордации для 7 экспертов и 10 альтернатив для каждой из групп.\n" <<
		"Определение группы 6 экспертов с наиболее согласованным мнением и коэффициент конкордации для неё." << endl << endl;
	cout << "Считывание данных из файла!" << endl;

	// считывание данных из файла
	importFromFile(n, m, ref(A));

	vector<pair<float, int>> matrix_result;
	pair<float, int> result;
	float full_result;
	// рассчёт коэффициентов конкордации для всех групп
	findCoefСoncordanceAllGroup(A, ref(matrix_result), ref(result), ref(full_result));

	// запись результата в файл
	output(full_result, result, matrix_result);

	cout << "Результаты успешно сохранены!" << endl;
	system("pause");
}