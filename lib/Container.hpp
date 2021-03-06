#ifndef DEF_CONTAINER
#define DEF_CONTAINER

#include "Miscellaneous.hpp"

/*{Container*/
/*!Contains a vector of Variable*, can store diffrent GenericVariable*/
class Container{
	public:
		/*!Default constructor*/
		Container() = default;
		/*!Copy constructor*/
		Container(Container const& c){
			for(auto const& d:c.data_){ data_.push_back(d->clone()); }
		}
		/*!Destructor*/
		~Container(){
			for(auto& d:data_){
				delete d;
				d = NULL;
			}
		}
		/*{Forbidden*/
		Container(Container&&) = delete;
		Container& operator=(Container) = delete;
		/*}*/

		/*!Add one GenericVariable<Type> of value t and named name*/
		template<typename Type>
			void set(std::string const& name, Type const& t);

		/*!Returns the value GenericVariable<Type>::t_ named name*/
		template<typename Type>
			Type get(std::string const& name) const;
		/*!Set the GenericVariable<Type> named name to t=GenericVariable<Type>::t_ */
		template<typename Type>
			void get(std::string const& name, Type& t) const;
		/*!Sets t to data_[i].get_val()*/
		template<typename Type>
			void get(unsigned int const& i, Type& t) const;
		/*!Returns data_[i].get_val()*/
		template<typename Type>
			Type get(unsigned int const& i) const;

		/*!Returns true if patter_==val_[i] and set i to that index*/
		virtual bool find(std::string const& pattern, unsigned int& i, bool const& lock_iffail) const {
			(void)(lock_iffail);
			i = 0;
			while(i<data_.size()){
				if(data_[i]->name_==pattern){ return true; }
				else { i++; }
			}
			return false;
		}

		/*!Returns the number of GenericVariable stored*/
		unsigned int size() const { return data_.size(); }

	private:
		/*{Variable*/
		/*!Base class that will never be instantiate. It provides a way to create
		 * GenericVariable with defferent type.*/
		class Variable{
			public:
				/*!Constructor*/
				Variable(std::string const& name):name_(name){}
				/*!Destructor*/
				virtual ~Variable() = default;
				/*{Forbidden*/
				Variable() = delete;
				Variable(Variable&&) = delete;
				Variable& operator=(Variable const&) = delete;
				/*}*/

				/*!Method that allows a copy of the derived class*/
				virtual Variable* clone() const = 0;

				std::string name_;//!< Name of the (Generic)Variable

			protected:
				/*!Default copy constructor*/
				Variable(Variable const&) = default;
		};
		/*}*/

		/*{GenericVariable*/
		/*!Derived class that can contain any value t_ of any type and of name
		 * Variable::name_*/
		template<typename Type>
			class GenericVariable : public Variable{
				public:
					/*!Constructor*/
					GenericVariable(std::string const& name, Type t):Variable(name),t_(t){}
					/*!Default destructor*/
					~GenericVariable() = default;
					/*!Returns a copy of*/
					GenericVariable<Type>* clone() const { return new GenericVariable<Type>(*this); }

					Type t_;//!< Value of the GenericVariable

				private:
					/*!Default copy constructor accessible via clone()*/
					GenericVariable(GenericVariable const&) = default;
			};
		/*}*/

	protected:
		std::vector<Variable*> data_;
};

template<typename Type>
void Container::set(std::string const& name, Type const& t){
	unsigned int i(0);
	if(!find(name,i,false)){ data_.push_back(new GenericVariable<Type>(name,t)); }
	else { std::cerr<<__PRETTY_FUNCTION__<<" : can't store two values with the same name ("<< name<<")" <<std::endl; }
}

template<typename Type>
Type Container::get(std::string const& name) const {
	unsigned int i(0);
	if(find(name,i,true)){ return static_cast<GenericVariable<Type>*>(data_[i])->t_; }
	else {
		std::cerr<<__PRETTY_FUNCTION__<<" : no variable with name "<<name<<std::endl;
		return Type();
	}
}

template<typename Type>
void Container::get(std::string const& name, Type& t) const {
	unsigned int i(0);
	if(find(name,i,true)){ t = static_cast< GenericVariable<Type> *>(data_[i])->t_; }
	else { std::cerr<<__PRETTY_FUNCTION__<<" : no data with name '"<<name<<"' thus the value is unchanged : "<< t <<std::endl; }
}

template<typename Type>
void Container::get(unsigned int const& i, Type& t) const {
	if(i<data_.size()){ t = static_cast< GenericVariable<Type> *>(data_[i])->t_; }
	else { std::cerr<<__PRETTY_FUNCTION__<<" : trying to get an unindexed value ("<<i<<"<"<< data_.size()<<"?)"<<std::endl; }
}

template<typename Type>
Type Container::get(unsigned int const& i) const {
	if(i<data_.size()){ return static_cast< GenericVariable<Type> *>(data_[i])->t_; }
	else {
		std::cerr<<__PRETTY_FUNCTION__<<" : "<<i<<"<"<< data_.size()<<"?" <<std::endl;
		return Type();
	}
}
/*}*/
#endif
