#include "NetWork.h"
void NetWork::Init(data_NetWork data){
//////////////////////////////////////////////////////////////////////////////
/// Инициализация матрицы и нейроны ошибки выделяя под них память и заполняя случайными значениями матрицу весов
/// \param data_NetWork - содержит количество слоёв нейронов и количество нейронов на каждом слое
/////////////////////////////////////////////////////////////////////////////
	actFunc.set();
	srand(time(NULL));
	L = data.L;
	size = new int[L];
	for (int i = 0; i < L; i++)
		size[i] = data.size[i];

	weights = new Matrix[L - 1];
	bios = new double* [L - 1];
	for (int i = 0; i < L - 1; i++) {
		weights[i].Init(size[i + 1], size[i]);
		bios[i] = new double[size[i + 1]];
		weights[i].Rand();
		for (int j = 0; j < size[i + 1]; j++) {
			bios[i][j] = ((rand() % 50)) * 0.06 / (size[i] + 15);
		}
	}
	neurons_val = new double* [L]; neurons_err = new double* [L];
	for (int i = 0; i < L; i++) {
		neurons_val[i] = new double[size[i]]; neurons_err[i] = new double[size[i]];
	}
	neurons_bios_val = new double[L - 1];
	for (int i = 0; i < L - 1; i++)
		neurons_bios_val[i] = 1;
}
void NetWork::PrintConfig() {
//////////////////////////////////////////////////////////////////////////////
/// Выводит в консоль информацию о сети, данные структуры data_NeeWork
/////////////////////////////////////////////////////////////////////////////
	cout << "***********************************************************\n";
	cout << "NetWork has " << L << " layers\nSIZE[]: ";
	for (int i = 0; i < L; i++) {
		cout << size[i] << " ";
	}
	cout << "\n***********************************************************\n\n";
}
void NetWork::SetInput(double* values) {
//////////////////////////////////////////////////////////////////////////////
/// Задаёт значения нейронам на входном слое, закрашивает пиксели в соответсвии с имеющиемся данными
/// \param values - массив значений нейронов входного слоя
/////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < size[0]; i++) {
		neurons_val[0][i] = values[i];
	}
}

double NetWork::ForwardFeed() {
//////////////////////////////////////////////////////////////////////////////
/// Умножает значения предыдущих слоёв на вес добавляет к этому произведению нейрон ошибки
/// и ищет максимальное значение на выходном слое - у какого числа нейрон больше, то и было на картинке
/////////////////////////////////////////////////////////////////////////////
	for (int k = 1; k < L; ++k) {
		Matrix::Multi(weights[k - 1], neurons_val[k - 1], size[k - 1], neurons_val[k]);
		Matrix::SumVector(neurons_val[k], bios[k - 1], size[k]);
		actFunc.use(neurons_val[k], size[k]);
	}
	int pred = SearchMaxIndex(neurons_val[L - 1]);
	return pred;
}
int NetWork::SearchMaxIndex(double* value) {
//////////////////////////////////////////////////////////////////////////////
/// Из нейронов массива ищет максимальный элемент, вытаскивает его индекс
/// \param value - массив значений нейронов выходного слоя
/////////////////////////////////////////////////////////////////////////////
	double max = value[0];
	int prediction = 0;
	double tmp;
	for (int j = 1; j < size[L - 1]; j++) {
		tmp = value[j];
		if (tmp > max) {
			prediction = j;
			max = tmp;
		}
	}
	return prediction;
}
void NetWork::PrintValues(int L) {
//////////////////////////////////////////////////////////////////////////////
/// Выводит значения нейронов выходного слоя
/////////////////////////////////////////////////////////////////////////////
	for (int j = 0; j < size[L]; j++) {
		cout << j << " " << neurons_val[L][j] << endl;
	}
}

void NetWork::BackPropogation(double expect) {
//////////////////////////////////////////////////////////////////////////////
/// Подсчёт ошибки скрытого и выходного слоя, сравнение с эталаном
/// \param expect - "правильное число"
/////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < size[L - 1]; i++) {
		if (i != int(expect))
			neurons_err[L - 1][i] = -neurons_val[L - 1][i] * actFunc.useDer(neurons_val[L - 1][i]);
		else
			neurons_err[L - 1][i] = (1.0 - neurons_val[L - 1][i]) * actFunc.useDer(neurons_val[L - 1][i]);
	}
	for (int k = L - 2; k > 0; k--) {
		Matrix::Multi_T(weights[k], neurons_err[k + 1], size[k + 1], neurons_err[k]);
		for (int j = 0; j < size[k]; j++)
			neurons_err[k][j] *= actFunc.useDer(neurons_val[k][j]);
	}
}
void NetWork::WeightsUpdater(double lr) {
//////////////////////////////////////////////////////////////////////////////
/// Имея данные от BackPropogation, складывает значение веса с ошибкой умноженного на коэффициент обучения поданного на вход, обновляя весы для улучшения последующих результатов
/// \param lr - learning rate
/////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < L - 1; ++i) {
		for (int j = 0; j < size[i + 1]; ++j) {
			for (int k = 0; k < size[i]; ++k) {
				weights[i](j, k) += neurons_val[i][k] * neurons_err[i + 1][j] * lr;
			}
		}
	}
	for (int i = 0; i < L - 1; i++) {
		for (int k = 0; k < size[i + 1]; k++) {
			bios[i][k] += neurons_err[i + 1][k] * lr;
		}
	}
}
void NetWork::SaveWeights() {
//////////////////////////////////////////////////////////////////////////////
/// Сохраняет ранее полученную матрцу весов в текстовый файл для повторного использования
/////////////////////////////////////////////////////////////////////////////
    ofstream fout;
	fout.open("Weights.txt");
	if (!fout.is_open()) {
		cout << "Error reading the file";
		system("pause");
	}
	for (int i = 0; i < L - 1; ++i)
		fout << weights[i] << " ";

	for (int i = 0; i < L - 1; ++i) {
		for (int j = 0; j < size[i + 1]; ++j) {
			fout << bios[i][j] << " ";
		}
	}
	cout << "Weights saved \n";
	fout.close();
}
void NetWork::ReadWeights() {
//////////////////////////////////////////////////////////////////////////////
/// Читает матрицу весов из файла
/////////////////////////////////////////////////////////////////////////////
	ifstream fin;
	fin.open("99L97T.txt");
	if (!fin.is_open()) {
		cout << "Error reading the file";
		system("pause");
	}
	for (int i = 0; i < L - 1; ++i) {
		fin >> weights[i];
	}
	for (int i = 0; i < L - 1; ++i) {
		for (int j = 0; j < size[i + 1]; ++j) {
			fin >> bios[i][j];
		}
	}
	cout << "Weights readed \n";
	fin.close();
}