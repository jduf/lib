#ifndef DEF_TIME
#define DEF_TIME

#include <ctime> 
#include <string> 

class Time{
	public:
		/*!Constructor*/
		Time(){set();}
		/*!Default destructor*/
		~Time() = default;
		/*{Forbidden*/
		Time(Time const&) = delete;
		Time(Time&&) = delete;
		Time& operator=(Time) = delete;
		/*}*/

		/*!Set to present time*/
		void set(){
			rawtime_ = lastcall_ = time(0);
			time_ = localtime(&rawtime_);
		}

		/*!Returns the current day*/
		int day() const { return time_->tm_mday;}
		/*!Returns the current month*/
		int month() const { return time_->tm_mon+1;}
		/*!Returns the current year*/
		int year() const { return time_->tm_year+1900;}
		/*!Returns the current hour*/
		int hour() const { return time_->tm_hour;}
		/*!Returns the current min*/
		int min() const { return time_->tm_min;}
		/*!Returns the current sec*/
		int sec() const { return time_->tm_sec;}
		/*!Returns the date*/
		std::string date(){ 
			char tmp[20];
			//std::strftime(tmp,20,"%G-%m-%d_%H:%M:%S",localtime(&rawtime_));
			std::strftime(tmp,20,"%F_%T",localtime(&rawtime_));
			return tmp;
		}

		/*!Returns true if time limit (in second) has been reached*/
		bool limit_reached(time_t const& limit) const 
		{ return time(0)>limit+rawtime_; }
		/*!Returns true if time limit (in second) has been reached*/
		bool progress(time_t const& every_s_seconds) {
			if(time(0) > (every_s_seconds+lastcall_)){
				lastcall_ = time(0); 
				return true;
			} else {
				return false;
			}
		}

		/*!Returns the elapsed from the instantiation or last call of set*/
		time_t elapsed() const { return time(0)-rawtime_; }

	private:
		time_t rawtime_; //!< return value of time(0)
		time_t lastcall_;//!< last call to Time::progress(...)  
		struct tm* time_;//!< return value of localtime(time(0))
};
#endif
