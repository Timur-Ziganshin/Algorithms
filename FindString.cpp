#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

void Prefix(string &s,vector<int> &pi){//для строчки формирует последо-ть pi , обозначающую размер наибольшего
	// префикса совпадающего с суфиксом для подстроки, оканчивающейся символом с текущим номером
								//a b r a c a d a b r a 
	pi.reserve(s.length());		//0 0 0 1 0 1 0 1 2 3 4
	auto beg = pi.begin();
	pi.insert(beg,0);
	int t = 0;
	for(int i=1;i<s.length();i++){
		while (t > 0 && s[t]!=s[i])
			t = pi[t-1];
		if (s[t]==s[i])
			t++;
		pi.insert(beg+i,t);
	}
}

int KMPSubstr(string s,string t){//s#t - рассматриваемая строка, находим функцию префикса для s 
	//динамически вычисляем значения функции префиксов для t , когда значения = длине s , мы нашли вхождение
	//отнимаем длину s для возрата индекса первого символа вхождения. O(s.length()+t.length()) = O(n)
	vector<int> pi;
	Prefix(s,pi);
	int q=0;
	for(int i=0;i<t.length();i++){
		while(q>0 && s[q]!=t[i])
			q = pi[q-1];
		if(s[q]==t[i])
			q++;
		if(q == s.length()){
			i-=s.length()-1;
			return i;
		}
	}
}

int KMPSubstrReverse(string s,string t){//s#t - рассматриваемая строка, находим функцию префикса для s 
	//динамически вычисляем значения функции префиксов для t , когда значения = длине s , мы нашли вхождение
	//отнимаем длину s для возрата индекса первого символа вхождения. O(s.length()+t.length()) = O(n)
	reverse(s.begin(),s.end()); 
	reverse(t.begin(),t.end()); 
	vector<int> pi;
	Prefix(s,pi);
	int q=0;
	for(int i=0;i<t.length();i++){
		while(q>0 && s[q]!=t[i])
			q = pi[q-1];
		if(s[q]==t[i])
			q++;
		if(q == s.length()){
			i-=s.length()-1;
			return t.length()-i-s.length();
		}
	}
}

int main(){
	string s = "ljahsduwuyhoijwmlqznwuyhoijwlf";
	string ss = "hoi";
	string ssq = "abracadabra";
	cout<<KMPSubstr(ss,s)<<endl;
	/*for(auto g:pi)
		cout<<g<<" ";
	cout<<endl;*/
	return 0;
}