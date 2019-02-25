#include <iostream>
#include <vector>

using namespace std;

void insert_sort(vector<int> &x){//начальная часть отсортирована, вставляем очередной элемент на свое место в
								//отсотированной части   n  - n^2
	for(int i=1;i<x.size();i++){
		int elem = x[i];
		int loc;
		for(loc = i-1;loc>=0 && x[loc]>elem;loc--)
			x[loc+1] = x[loc];
		x[loc+1] = elem;
	}
}

void bubble_sort(vector<int> &x){//конечная часть отсортирована, элементы "всплывают"
								//на свое место , t - граница отсортированной части   n - n^2
	for(int t = x.size()-1;t>0;){
		int bound = t;
		t = 0;
		for(int i = 0;i<bound;i++){
			if(x[i+1]<x[i]){
				int q = x[i];
				x[i]=x[i+1];
				x[i+1] = q;
				t = i;
			}
		}
	}
}

void direct_choice_sort(vector<int> &x){//находим макс запись, ставим ее в конец, повторяем сначала 
										//для послед-ти без последнего эл-та   n^2  - n^2
	for(int j = x.size()-1;j>0;j--){
		int k = j;
		for(int i = k-1;i>=0;i--)
			if(x[k]<x[i])
				k = i;
		int r = x[j];
		x[j] = x[k];
		x[k] = r;
	}
}

void Heapify(vector<int> &x,int i,int n){// делаем i-ый элемент корнем пирамиды , при условии, что
									// 2*i+1  и 2*i+2 уже корни пирамиды  O(h) , h - высота пирамиды в i
	while(true){
		int l = 2*i+1,r = l+1,j=i;
		if(l<n && x[i]<x[l])
			i = l;
		if(r<n && x[i]<x[r])
			i = r;
		if(i==j)
			break;
		int w = x[i];
		x[i] = x[j];
		x[j] = w;
	}
}

void BuildHeap(vector<int> &x){ // для всех возможных некорней вызываем Heapify чтобы сделать корнями O(n)
	for(int i = x.size()/2-1;i>=0;i--)
		Heapify(x,i,x.size());
}

void heap_sort(vector<int> &x){ //Делаем пирамиду,меняем местами первый(максимальный) и последний эл-ты
								//восстанавливаем пирамиду на меньшей последовательности пока длина > 0 O(nlgn)
	BuildHeap(x);
	for(int i=x.size()-1;i>0;i--){
		int e = x[0];
		x[0]=x[i];
		x[i]=e;
		Heapify(x,0,i);
	}
}

void Merge(vector<int> &x,int k,int l,int m){//[k,l] и [l+1,m] отсортированы , сливаем их элементы чтобы получить 
								// отсортированую [k,m] , используем вспомогательную последовательность 
					// и два счетика по эл-ам из первой и второй послед-ей, добавляем наименьший из 2-х и тд
	vector<int> q;
	q.reserve(m-k+1);
	int i = k,j = l+1,h=0;
	for(;h<m-k+1;h++){
		if(j<=m && (i==l+1 || x[i]>x[j])){
			q[h] = x[j];
			j++;
		}else{
			q[h] = x[i];
			i++;
		}
	}
	for(i=0;i<h;i++)
		x[k+i] = q[i];
}

void MergeSortRec(int low,int high,vector<int> &x){ //рекурсивно рабиваем последовательность на пополам
	//и вызываем сортировку для каждой половинки, потом сливаем их вместе O(nlgn) ,доп блок памяти на n эл-ов 
	if(low<high){
		int mid = (low+high)/2;
		MergeSortRec(low,mid,x);
		MergeSortRec(mid+1,high,x);
		Merge(x,low,mid,high);
	}
}

void merge_sort(vector<int> &x){
	MergeSortRec(0,x.size()-1,x);
}

int Partition(int low,int high,vector<int> &x){ //разбиение последо-ти на две части : меньше x[high]
	// и больше или равно , возвращается граница разделения(где находится эл-т x[high])
	int i = low,h;
	for(int j = low;j<high;j++){
		if(x[j]<x[high]){
			h = x[i];
			x[i] = x[j];
			x[j] = h;
			i++;
		}
	}
	h = x[i];
	x[i] = x[high];
	x[high] = h;
	return i;
}

void QuickSortRec(int low,int high,vector<int> &x){//рекурсивное разбиение послед-ти относительно последнего
	// элемента и выполение сортировки для двух частей разбиения O(n lgn) - O(n^2) 
	if(low<high){
		int q = Partition(low,high,x);
		QuickSortRec(low,q-1,x);
		QuickSortRec(q+1,high,x);
	}
}

void quick_sort(vector<int> &x){
	QuickSortRec(0,x.size()-1,x);
}

int key_func(int numb,int div){//функция ключа для поразрядной сортировки
	return (numb/div)%10;
}

void distribution_sort(vector<int> &x,int div){//вычисляются сколько раз встречается каждый ключ (count),
	//с конца, для кадого элемента послед-ти вычисляется ключ смотрится значение в count и уменьшается
	//на 1 , это и будет индекс элемента в отсортированной последовательности O(n) при условии что m константа
	vector<int> count,dist;
	count.reserve(10);
	for(int i=0;i<10;i++)
		count[i]=0;
	for(int i=0;i<x.size();i++)
		count[key_func(x[i],div)]++;
	for(int i=1;i<10;i++)
		count[i]+=count[i-1];
	dist.reserve(x.size());
	for(int i=x.size()-1;i>=0;i--){
		int j = --count[key_func(x[i],div)];
		dist[j] = x[i];
	}
	for(int i=0;i<x.size();i++)
		x[i]=dist[i];
}

void distribution_radix_sort(vector<int> &x,int q){//для q разрядов поразрядная сортировка
	int div = 1;
	for(int i=0;i<q;i++){
		distribution_sort(x,div);
		div*=10;
	}
}

int main(){
	vector<int> q;
	q.push_back(8);
	q.push_back(4);
	q.push_back(8);
	q.push_back(1);
	q.push_back(8);
	q.push_back(6);
	q.push_back(2);
	q.push_back(7);
	q.push_back(9);
	distribution_radix_sort(q,3);
	for(auto s:q)
		cout<<s<<" ";
	cout<<endl;
	return 0;
}