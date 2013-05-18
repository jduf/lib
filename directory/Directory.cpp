#include "Directory.hpp"

Directory::Directory():
	dir("."),
	fname(0),
	path(0),
	ext(0)
{ }

void Directory::search_file(std::string const& keyword, std::string curr_dir){
	DIR* dir_point = opendir(curr_dir.c_str());
	dirent* entry = readdir(dir_point);
	while (entry){
		if (entry->d_type == DT_DIR){
			std::string dir = entry->d_name;
			if (dir != "." && dir != ".."){
				search_file(keyword,curr_dir+"/"+dir);
			}
		}
		else if (entry->d_type == DT_REG){
			std::string f = entry->d_name;
			if (f.find(keyword) != std::string::npos){
				path.push_back(curr_dir);
				split_fname(f);
			}
		}
		entry = readdir(dir_point);
	}
	closedir(dir_point);
}

void Directory::search_file_ext(std::string const& extension, std::string curr_dir){
	DIR* dir_point = opendir(curr_dir.c_str());
	dirent* entry = readdir(dir_point);
	while (entry){
		if (entry->d_type == DT_DIR){
			std::string dir = entry->d_name;
			if (dir != "." && dir != ".."){
				search_file_ext(extension,curr_dir+"/"+dir);
			}
		} else if (entry->d_type == DT_REG){
			std::string f = entry->d_name;
			if (f.find(extension,f.size()-extension.size()) != std::string::npos){
				path.push_back(curr_dir);
				split_fname(f);
			}
		}
		entry = readdir(dir_point);
	}
	closedir(dir_point);
}

void Directory::print(){
	for(unsigned int i(0);i<path.size();i++){
		std::cout<<path[i]<<"/"<<fname[i]<<ext[i]<<std::endl;
	}
}

void Directory::split_fname(std::string f){
	unsigned int pos(f.find("."));
	fname.push_back(f.substr(0,pos));
	ext.push_back(f.substr(pos));
}

void Directory::sort(){
	bool sort(true);
	while(sort){
		sort=false;
		for(unsigned int i(0); i<path.size()-1;i++){
			if(path[i]+fname[i]+ext[i] > path[i+1]+fname[i+1]+ext[i+1]) { 
				sort= true;
				std::string tmp("");
				tmp = path[i];
				path[i] = path[i+1];
				path[i+1] = tmp;
				tmp =fname[i];
				fname[i] = fname[i+1];
				fname[i+1] = tmp;
				tmp = ext[i];
				ext[i] = ext[i+1];
				ext[i+1] = tmp;
			}
		}
	}
}

