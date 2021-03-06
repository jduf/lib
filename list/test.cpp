#include "List.hpp"
#include "Vector.hpp"
#include "Rand.hpp"

class A{
	public:
		A(double a):a_(a){
			N_++;
			std::cout<<"normal A "<<N_<<" "<<a_<<std::endl;
		}

		A(A const& a):a_(a.a_){
			N_++;
			std::cout<<"copy A "<<N_<<" "<<a_<<std::endl;
		}

		A(A&& a):a_(a.a_){
			std::cout<<"move A "<<N_<<" "<<a_<<std::endl;
		}

		~A(){
			N_--;
			std::cout<<"destroyed A "<<N_<<" "<<a_<<std::endl;
		}

		A const& operator*=(double x){
			a_ *= x;
			return (*this);
		}

		void print(std::ostream& flux) const { flux<<a_; }

		static unsigned int N_;
		double a_;
};

unsigned int A::N_ = 0;

std::ostream& operator<<(std::ostream& flux, A const& a){
	a.print(flux);
	return flux;
}

int main(){
	{
		std::cout<<"#### general tests ####"<<std::endl;
		List<A> a;
		for(unsigned int i(0);i<10;i++){ a.add_end(std::make_shared<A>(i)); }
		std::cout<<a<<std::endl;
		std::cout<<A::N_<<std::endl;
		std::cout<<"size of the list "<<a.size()<<std::endl;
		std::cout<<"will remove 4th entry with pop(4)"<<std::endl;
		a.pop(4);
		std::cout<<a<<std::endl;
		std::cout<<"will remove last entry with pop(idx)"<<std::endl;
		a.pop(a.size()-1);
		std::cout<<a<<std::endl;
		std::cout<<"will remove last entry with pop_end"<<std::endl;
		a.pop_end();
		std::cout<<a<<std::endl;
		std::cout<<"will remove 1st entry with pop(0)"<<std::endl;
		a.pop(0);
		std::cout<<a<<std::endl;
		std::cout<<"will swap entry 2 end 5"<<std::endl;
		a.swap(2,5);
		std::cout<<a<<std::endl;
		std::cout<<"will add 3 in at the 3rd position"<<std::endl;
		a.add(std::make_shared<A>(3),3);
		std::cout<<a<<std::endl;
		std::cout<<"will add 3 in at the first ordered position"<<std::endl;
		auto func = [](A const& a, A const& b){ return a.a_<b.a_;};
		a.add_sort(std::make_shared<A>(3),func);
		std::cout<<a<<std::endl;
		std::cout<<"will add 10 in at the first ordered position"<<std::endl;
		a.add_sort(std::make_shared<A>(10),func);
		std::cout<<a<<std::endl;
		std::cout<<"will add 0 in at the first ordered position"<<std::endl;
		a.add_sort(std::make_shared<A>(0),func);
		std::cout<<a<<std::endl;
		std::cout<<"will add -1 in at the first ordered position"<<std::endl;
		a.add_sort(std::make_shared<A>(-1),func);
		std::cout<<a<<std::endl;
		std::cout<<"will add -0.5 in at the first ordered position"<<std::endl;
		a.add_sort(std::make_shared<A>(-0.5),func);
		std::cout<<a<<std::endl;
		std::cout<<"will add 9 in at the first ordered position"<<std::endl;
		a.add_sort(std::make_shared<A>(9),func);
		std::cout<<a<<std::endl;
		std::cout<<"will add 0 in at the first ordered position"<<std::endl;
		a.add_sort(std::make_shared<A>(0),func);
		std::cout<<a<<std::endl;
		std::cout<<"will print using get_next must be identical to the previous line"<<std::endl;
		while (a.target_next()){ std::cout<<a.get()<<" "; }
		std::cout<<std::endl;
		std::cout<<"will copy the whole first list without actually copying the value."<<std::endl;
		List<A> c;
		while (a.target_next()){ c.add_end(a.get_ptr()); }
		while (c.target_next()){ std::cout<<c.get()<<" ";}
		std::cout<<std::endl;

		std::cout<<"multiply the first entry bigger or equal to 2 by pi"<<std::endl;
		while(c.target_next() && c.get().a_ < 2.0 );
		c.get() *= M_PI;
		std::cout<<c<<std::endl;
		std::cout<<"the other list should also be affected"<<std::endl;
		std::cout<<a<<std::endl;


		std::cout<<"list copied from the first one between entry [3,7)"<<std::endl;
		List<A> b(a.sublist(3,7));/*understand why it doesn't call a copy constructor*/
		//List<A> b(std::move(a.sublist(3,7)));/*understand why it doesn't call a copy constructor*/
		std::cout<<b<<std::endl;
		std::cout<<"will remove the 2nd entry with pop(1)"<<std::endl;
		b.pop(1);
		std::cout<<b<<std::endl;
		std::cout<<"will remove the 1st entry with pop(0)"<<std::endl;
		b.pop(0);
		std::cout<<b<<std::endl;
		std::cout<<"will add 8 at the 1st entry with add_start()"<<std::endl;
		b.add_start(std::make_shared<A>(8));
		std::cout<<b<<std::endl;
		std::cout<<"will remove last entry with pop(idx)"<<std::endl;
		b.pop(b.size()-1);
		std::cout<<b<<std::endl;
		std::cout<<"will remove last entry with pop(idx)"<<std::endl;
		b.pop(b.size()-1);
		std::cout<<b<<std::endl;
		std::cout<<"will remove last entry with pop(idx)"<<std::endl;
		b.pop(b.size()-1);
		std::cout<<b<<std::endl;
		std::cout<<"will remove last entry with pop(idx)"<<std::endl;
		b.pop(b.size()-1);
		std::cout<<b<<std::endl;
		std::cout<<"will add at the last entry with add_end()"<<std::endl;
		b.add_end(std::make_shared<A>(9));
		std::cout<<b<<std::endl;
	}
	std::cout<<"#(constructor calls)-#(destructor calls)="<<A::N_<<std::endl;
	//{
		//std::cout<<"#### test add_sort with int ####"<<std::endl;
		//List<int> a;
		//Rand<int> rnd(0,100);
		//auto cmp = [](int const& a, int const& b){ return a<b;};
		//for(unsigned int i(0);i<30;i++){ a.add_sort(std::make_shared<int>(rnd()),cmp ); }
		//std::cout<<a<<std::endl;
//
		//IOFiles out("list_int.jdbin",true,false);
		//out.write("int",a);
	//}
	//{
		//std::cout<<"#### test add_sort with vector ####"<<std::endl;
		//auto func = [](Vector<int> const& a, Vector<int> const& b) {
			//unsigned int i(0);
			//while(i<a.size()){
				//if(a(i) > b(i)){ return false; }
				//if(a(i) < b(i)){ return true; }
				//if( my::are_equal(a(i),b(i)) ){ i++; }
			//}
			//return false;
		//};
		//Rand<int> rnd(0,10);
		//List<Vector<int> > a;
		//Vector<int> tmp(2);
		//for(unsigned int i(0);i<10;i++){
			//tmp(0) = rnd();
			//tmp(1) = rnd();
			//a.add_sort(std::make_shared<Vector<int> >(tmp), func);
			//std::cout<<a<<std::endl;
		//}
	//}
	//{
		//std::cout<<"#### test find_in_sorted_list/add_after_target ####"<<std::endl;
		//auto cmp_for_fuse = [](Vector<int> const& a, Vector<int> const& b) {
			//unsigned int i(0);
			//while(i<a.size()){
				//if(a(i) > b(i)){ return 0; }
				//if(a(i) < b(i)){ return 1; }
				//if( my::are_equal(a(i),b(i)) ){ i++; }
			//}
			//return 2;
		//};
		//auto fuse = [](Vector<int>& a, Vector<int> const& b) {
			//std::cout<<"should fuse "<<a<<" and "<<b<<std::endl;
		//};
		//Rand<int> rnd(0,10);
		//List<Vector<int> > a;
		//Vector<int> tmp(2);
		//std::shared_ptr<Vector<int> > tmp_shared;
		//List<Vector<int> >::Node* target(NULL);
		//for(unsigned int i(0);i<10;i++){
			//tmp(0) = rnd();
			//tmp(1) = rnd();
			//tmp_shared = std::make_shared<Vector<int> >(tmp);
			//if( a.find_in_sorted_list(tmp_shared, target, cmp_for_fuse) ){
				//fuse(*target->get(),*tmp_shared);
			//} else {
				//a.set_target(target);
				//a.add_after_target(tmp_shared);
			//}
			//std::cout<<a<<std::endl;
		//}
		//a.target_next();
		//a.target_next();
		//tmp = a.get();
		//std::cout<<tmp<<std::endl;
		//tmp_shared = std::make_shared<Vector<int> >(tmp);
		//if( a.find_in_sorted_list(tmp_shared, target, cmp_for_fuse) ){
			//fuse(*target->get(),*tmp_shared);
		//} else {
			//a.set_target(target);
			//a.add_after_target(tmp_shared);
		//}
		//std::cout<<a<<std::endl;
//
		//IOFiles out("list_vector.jdbin",true,false);
		//out.write("vector",a);
	//}
	//{
		//std::cout<<"#### read list from files ####"<<std::endl;
		//IOFiles in_vector("list_vector.jdbin",false,false);
		//List<Vector<int> > lvec;
		//in_vector>>lvec;
		//std::cout<<lvec<<std::endl;
//
		//IOFiles in_int("list_int.jdbin",false,false);
		//List<int> lint;
		//in_int>>lint;
		//std::cout<<lint<<std::endl;
//
		//std::cout<<"#### move list ####"<<std::endl;
		//List<int> move(std::move(lint));
		//std::cout<<lint<<std::endl;
		//std::cout<<move<<std::endl;
//
		//Rand<unsigned int> r(move.size()-3,move.size());
		//unsigned int s;
		//for(unsigned int i(0);i<20;i++){
			//s=r.get();
			//std::cout<<s<<" ";
			//move.set_target();
			//while(move.target_next() && --s);
			//std::cout<<move.get()<<std::endl;
		//}
	//}
	//{
		//List<unsigned int> l;
		//for(unsigned int i(0);i<20;i++){ l.add_end(std::make_shared<unsigned int>(i)); }
		//l.set_target();
		//while(l.target_next()){ std::cout<<l.get()<<" "; }
		//std::cout<<std::endl;
//
		//l.set_target();
		//while(l.target_next()){
			//if(l.get()==2){ l.pop_target();}
			//if(l.get()==3){ l.pop_target();}
			//if(l.get()==4){ l.pop_target();}
			//if(l.get()==0){ l.pop_target();}
			//if(l.get()==19){ l.pop_target();}
		//}
		//while(l.target_next()){ std::cout<<l.get()<<" "; }
		//std::cout<<std::endl;
	//}
}
