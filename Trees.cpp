#include <algorithm>
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

class SegmentTree{//дерево отрезков с операциями построения дерева, обновления элемента, запроса к дереву
	//операция сумма элементов
public:
	SegmentTree(const vector<int> &x);
	void UpdateTree(int i,int v,int n);
	int QueryTree(int n,int l,int r);
private:
	vector<int> tree;
	void buildTree(int v,int a,int b,const vector<int> &x);
	void update(int w,int i,int v,int a,int b);
	int query(int w,int l,int r,int a,int b);
};

SegmentTree::SegmentTree(const vector<int> &x){//вычисляется размер дерева исходя из размера входного массива
	//2^(w)>x.size() tree.size()=2^(w+1) выполняется построение дерева
	int w = 1;
	while(w<x.size()){
		w*=2;
	}
	for(int i=0;i<2*w;i++)
		tree.push_back(0);
	buildTree(0,0,x.size()-1,x);
	for(int i=0;i<2*w;i++)
		cout<<tree[i]<<" ";
	cout<<endl;
}

void SegmentTree::buildTree(int v,int a,int b,const vector<int> &x){
	// если правая и левая граница совпадают ([4,4]) то на соответсвующее место в массиве(нумерация как в кучу пирамиды)
	//ставиться x[4] , иначе вычисляется середина и построение рекурсивно двух меньших отрезков, в текущую ячейку
	// заносится сумма двух меньших отрезков
	if(a==b){
		tree[v] = x[a]; 
	}else{
		int m = (a+b)/2;
		buildTree(2*v+1,a,m,x);
		buildTree(2*v+2,m+1,b,x);
		tree[v] = tree[2*v+1] + tree[2*v+2];
	}
}

void SegmentTree::UpdateTree(int i,int v,int n){
	update(0,i,v,0,n-1);
}

void SegmentTree::update(int w,int i,int v,int a,int b){
	// ищется отрезок для которого a==b==i, элемент на i месте массива, одновременно пересчитываются
	//все элемнты включающие отрезок [i,i]
	if(a==b){
		tree[w]=v;
	}else{
		int m = (a+b)/2;
		if(i<=m)
			update(2*w+1,i,v,a,m);
		else
			update(2*w+2,i,v,m+1,b);
		tree[w] = tree[2*w+1] + tree[w*2+2];
	}
}

int SegmentTree::QueryTree(int n,int l,int r){
	return query(0,l,r,0,n-1);
}

int SegmentTree::query(int w,int l,int r,int a,int b){
	//если текущиий отрезок полностью лежит в [l,r] = [a,b] , то возвращаем связанную с ним сумму
	//иначе смотрим правее или левее лежит  l r относительно текущего отрезка или возвращаем сумму и с права и слева,
	//уменьшая отрезок l r   
	if(l==a && r==b){
		return tree[w];
	}else{
		int m = (a+b)/2;
		if(r<=m)
			return query(2*w+1,l,r,a,m);
		if(l>m)
			return query(2*w+2,l,r,m+1,b);
		return query(2*w+1,l,m,a,m)+query(2*w+2,m+1,r,m+1,b);
	}
}
													//i
class FenvickTree{//массив из n элементов, tree[i] = SUM x[i]
public:											  //i&(i+1)
	FenvickTree(const vector<int> &x);
	void UpdateTree(int i,int v,int n);
	int QueryTree(int n,int l,int r);
private:
	vector<int> tree;
	int buildTree(int n,int l,int r,const vector<int> &x);
	int query(int i);
};

FenvickTree::FenvickTree(const vector<int> &x){
	int w = 1;
	while(w<x.size()){
		w*=2;
	}
	for(int i=0;i<x.size();i++)
		tree.push_back(0);
	buildTree(x.size(),0,w-1,x);
}

int FenvickTree::buildTree(int n,int l,int r,const vector<int> &x){
	int sum = 0,bound = min(r,n);
	while(l<bound){
		int m = (l+r)/2;
		sum+=buildTree(n,l,m,x);
		l = m+1;
	}
	if(r<n){
		sum+=x[r];
		tree[r] = sum;
	}
	return sum;
}

void FenvickTree::UpdateTree(int i,int v,int n){
	while(i<n){
		tree[i] = tree[i]+v;
		i = i|(i+1);
	}
}

int FenvickTree::QueryTree(int n,int l,int r){
	return query(r) - query(l-1);
}

int FenvickTree::query(int i){
	int v = 0;
	while(i>=0){
		v = v+tree[i];
		i = (i&(i+1))-1;
	}
	return v;
}

class SparseTable{
public:
	SparseTable(const vector<int> &x);
	int QueryTable(int l,int r);
private:
	void build(const vector<int> &x);
	void computeLg(int m);
	int Oper(int a,int b);
	vector<int> lg;
	int **ST;
	int n,m;
};

SparseTable::SparseTable(const vector<int> &x){
	computeLg(x.size());
	m = lg[x.size()]+1;
	n = x.size();
	ST = new int*[n];
	for(int i=0;i<n;i++)
		ST[i] = new int[m];
	build(x);
}

void SparseTable::computeLg(int mw){
	int j = 0;
	for(int i=0;i<mw;i++){
		int w = pow(2,i);
		for(;j<w;j++)
			lg.push_back(i-1);
	}
}

void SparseTable::build(const vector<int> &x){
	for(int i=0;i<n;i++)
		ST[i][0] = x[i];
	for(int j=1;j<m;j++){
		int r = pow(2,j);
		for(int i=0;i<=n-r;i++)
			ST[i][j] = Oper(ST[i][j-1],ST[i+r/2][j-1]);
	}
}

int SparseTable::Oper(int a,int b){
	return min(a,b);
}

int SparseTable::QueryTable(int l,int r){
	int j = lg[r-l+1];
	return Oper(ST[l][j],ST[r-(int)pow(2,j)+1][j]);
}

pair<int,int> Kadane_Alg(const vector<int> &x){
	int l=0,r=0,maxsum = x[0];
	int start=0,sum=0;
	for(int i=0;i<x.size();){
		sum+=x[i];
		if(sum>maxsum){
			maxsum = sum;
			l=start;
			r=i;
		}
		i++;
		if(sum<0){
			sum=0;
			start=i;
		}
	}
	pair<int,int> q;
	q.first = l;
	q.second = r;
	return q;
}

int main(){
	vector<int> q;
	q.push_back(8);
	q.push_back(4);
	q.push_back(8);
	q.push_back(1);
	q.push_back(-100);
	q.push_back(6);
	q.push_back(2);
	q.push_back(7);
	q.push_back(9);
	//SparseTable test(q);
	auto s = Kadane_Alg(q);
	cout<<s.first<<" "<<s.second<<endl;
	return 0;
}