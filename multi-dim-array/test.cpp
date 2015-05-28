/* @file test.cpp */

#include "Vector.hpp"
#include "omp.h"

unsigned int iter(0);
unsigned int hypercube_element(int m, int n);
bool init(Vector<double>* A, unsigned int& i, unsigned int& j, unsigned int const& imax);
bool eval(Vector<double>* A, Vector<unsigned int>& idx);
bool eval_border(Vector<double>* A, Vector<unsigned int>& idx, unsigned int const& min0 = 0, unsigned int const& max0 = 0);
void normal();
void parallel();

int main(){
	//normal();
	//parallel();
	unsigned int N(3);
	unsigned int s(0);
	for(unsigned int i(1);i<N+1;i++){
		s += hypercube_element(N-i,N)*(i%2?1:-1)*pow(10,N-i);
	}
	std::cerr<<s<<std::endl;
}

bool init(Vector<double>* A, unsigned int& i, unsigned int& j, unsigned int const& imax){
	if(j==A[i].size()){ 
		if(++i==imax){ return false; }
		j=0;
	} else { 
		A[i](j) = j*pow(10,i);
		j += 1;
	}
	return init(A,i,j,imax);
}

bool eval(Vector<double>* A, Vector<unsigned int>& idx){
	unsigned int tmp(0);
	for(unsigned int i(0);i<idx.size();i++){ tmp += A[i](idx(i)); }
	std::cout<<idx<<" ->"<<tmp<<std::endl;

	idx(0)++;
	for(unsigned int i(0);i<idx.size();i++){
		if(idx(i) == A[i].size()){ 
			if(i+1 == idx.size()){ return false; }
			idx(i) = 0;
			idx(i+1)++;
		}
	}
	return eval(A,idx);
}

bool eval_border(Vector<double>* A, Vector<unsigned int>& idx, unsigned int const& min0, unsigned int const& max0){
	for(unsigned int l(0);l<idx.size();l++){
		if(l<idx.size() && (idx(l) == 0 || idx(l) == A[l].size()-1) ) { 
			std::cout<<idx<<std::endl;
			l = idx.size();
		}
	}

	idx(0)++;
	if(min0==0 && max0==0){
		for(unsigned int i(0);i<idx.size();i++){
			if(idx(i) == A[i].size()){ 
				if(i+1 == idx.size()){ return false; }
				idx(i) = 0;
				idx(i+1)++;
			}
		}
	} else {
		if(idx(0) == max0){ 
			if(1 == idx.size()){ return false; }
			idx(0) = min0;
			idx(1)++;
		}
		for(unsigned int i(1);i<idx.size();i++){
			if(idx(i) == A[i].size()){ 
				if(i+1 == idx.size()){ return false; }
				idx(i) = 0;
				idx(i+1)++;
			}
		}
	}

	return eval_border(A,idx,min0,max0);
}

void normal(){
	unsigned int N(3);
	Vector<double>* A(new Vector<double>[N]);
	A[0].set(10,0);
	A[1].set(10,0);
	A[2].set(10,0);
	//A[3].set(10,0);
	//for(unsigned int i(0);i<N;i++){ std::cout<<A[i].size()<<std::endl; }

	unsigned int i(0);
	unsigned int j(0);
	while(init(A,i,j,N));
	//for(unsigned int i(0);i<N;i++){ std::cout<<A[i]<<std::endl; }
	Vector<unsigned int> idx;
	//idx.set(N,0);
	//while(eval(A,idx));
	idx.set(N,0);
	while(eval_border(A,idx));

}

void parallel(){
	unsigned int N(4);
	Vector<double>* A(new Vector<double>[N]);
	A[0].set(80,0);
	A[1].set(10,0);
	A[2].set(10,0);
	A[3].set(10,0);
	//for(unsigned int i(0);i<N;i++){ std::cout<<A[i].size()<<std::endl; }

	unsigned int i(0);
	unsigned int j(0);
	while(init(A,i,j,N));
	//for(unsigned int i(0);i<N;i++){ std::cout<<A[i]<<std::endl; }
	//idx.set(N,0);
	//while(eval(A,idx));

	Vector<double> backup_A0(A[0]);
#pragma omp parallel
	{
		unsigned int min0(omp_get_thread_num()*A[0].size()/8);
		unsigned int max0((omp_get_thread_num()+1)*A[0].size()/8);
		Vector<unsigned int> idx;
		idx.set(N,0);
		idx(0) = min0;
#pragma omp critical
		eval_border(A,idx,min0,max0);
	}
}

unsigned int hypercube_element(int m, int n){
	if(m==0 && n==0){ return 1; }
	if(m<0 || n<0 || n<m){ return 0; }
	return 2*hypercube_element(m,n-1)+hypercube_element(m-1,n-1);
}
