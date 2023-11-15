#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

//вычисление minrun шаг 0
int getMinrun(int n)
{
	int r = 0;

	while (n >= 64)
	{
		r |= n & 1;
		n >>= 1; // == n / 2
	}

	return n + r;
}

//структура для хранения начала подмассива и его длины.
struct sRange
{
	int start;
	int length;
};
typedef sRange Range;

int min(int a, int b)
{
	int min;

	if (a < b)
	{
		min = a;
	}
	else { min = b; }

	return min;
}

// функция слияния подмассивов. 
// получает на вход массив и диапазоны подмассивов (Range x, Range y), 
// производит слияние. По размеру первого диапазона (x) создает временный массив, 
// копирует туда значения из первого диапазона, и потом, сравнивая значения из первого (x) и второго диапазона (y), 
// постепенно заполняет в отсортированном виде часть массива, равную двум диапазонам (начиная от x.start до x.length+y.length).
void merge(int* arr, int fStart, int fLength, int sStart, int sLength)
{
	int* tmp = new int[fLength];

	memcpy(tmp, &arr[fStart], sizeof(int) * (fLength));
	int fIndex = 0;
	int sIndex = sStart;
	int i = fStart;

	while (i < sStart + sLength - 1)
	{
		if (arr[sIndex] > tmp[fIndex])
		{
			arr[i] = tmp[fIndex];
			++fIndex;
			fIndex = min(fIndex, fLength);
		}
		else if (arr[sIndex] <= tmp[fIndex])
		{
			arr[i] = arr[sIndex];
			++sIndex;
			sIndex = min(sIndex, sStart + sLength);
		}
		if (sIndex == sStart + sLength)
		{
			if (fIndex != fLength)
			{
				memcpy(&arr[i + 1], &tmp[fIndex], sizeof(int) * (fLength - fIndex));
				i = sStart + sLength;
			}
		}
		if (fIndex == fLength)
		{
			i = sStart + sLength;
		}
		++i;
	}
	delete[] tmp;
}

//вставка частей
void sortInsertionPart(int* arr, int start, int end)
{
	for (int i = start + 1; i < end; ++i)
	{
		int current = arr[i];
		int j = i - 1;
		while ((j >= start) && (current < arr[j]))
		{
			swap(arr[j], arr[j + 1]);
			--j;
		}
	}
}

void timsort(int* arr, int n) 
{
	int minrun = getMinrun(n);

	vector <Range> minrunStack;

	int currentIndex = 0;

	while (currentIndex < n - 1)
	{
		int currentRunIndex = currentIndex;
		int nextIndex = currentRunIndex + 1;

		if (arr[currentRunIndex] > arr[nextIndex])
		{
			swap(arr[currentRunIndex], arr[nextIndex]);
		}

		bool goNext = true;
		while (goNext)
		{
			if (arr[currentRunIndex] > arr[nextIndex])
				goNext = false;
			if (!(goNext) && (currentRunIndex - currentIndex < minrun))
				goNext = true;
			if (currentRunIndex == n - 1)
				goNext = false;
			if (goNext)
			{
				++currentRunIndex;
				++nextIndex;
			}
		}
		sortInsertionPart(arr, currentIndex, currentRunIndex + 1);

		Range r;
		r.start = currentIndex;
		r.length = currentRunIndex - currentIndex;

		minrunStack.push_back(r);

		currentIndex = currentRunIndex;
	}

	//merge
	while (minrunStack.size() > 1)
	{
		Range x = minrunStack.at(0);
		minrunStack.erase(minrunStack.begin());
		Range y = minrunStack.at(0);
		minrunStack.erase(minrunStack.begin());
		if (x.start > y.start)
		{
			swap(x.start, y.start);
			swap(x.length, y.length);
		}
		if (y.start != x.start + x.length)
		{
			minrunStack.push_back(y);
			minrunStack.push_back(x);
			continue;
		}
		merge(arr, x.start, x.length, y.start, y.length);
		x.length = x.length + y.length;
		minrunStack.push_back(x);
	}
}

int main() {
	int n;
	cout << "vvedite razmer massiva: "; cin >> n;
	cout << endl;

	int* arr = new int[n];
	cout << "zapolnite massiv" << endl;

	system("cls");

	for (int i = 0; i < n; i++)
	{
		cout << "vvedite " << i + 1 << " chislo: ";
		cin >> arr[i];
	}

	system("cls");

	cout << "massiv sostoit iz " << n << " chisel" << endl;
	for (int i = 0; i < n; i++) {
		cout << arr[i] << " ";
	}
	cout << endl;
	cout << endl;
	cout << endl;

	timsort(arr, n);

	cout << "posle sortirovki: ";
	for (int i = 0; i < n; i++) {
		cout << arr[i] << " ";
	}

	delete[] arr;
}
