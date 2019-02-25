#include <iostream>
#include <vector>

using namespace std;

int bin_search(const vector<int> &x,int q){
	int l = -1,r = x.size()-1;
	if(x[r]<q)
		return -1;
	while(r-l>1){
		int mid = (l+r)/2;
		if(x[mid]>=q){
			r = mid;
		}else{
			l = mid;
		}
	}
	return r;
}

int main(int argc, char const *argv[])
{
	vector<int> q;
	q.push_back(1);
	q.push_back(2);
	q.push_back(5);
	q.push_back(10);
	q.push_back(11);
	q.push_back(12);
	q.push_back(13);
	q.push_back(17);
	cout<<bin_search(q,4)<<endl;
	return 0;
}