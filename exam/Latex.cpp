#include "Latex.hpp"

Latex::Latex(std::string const& path, std::string const& filename):
	path_(my::ensure_trailing_slash(path)),
	filename_(filename),
	latex_("")
{}

void Latex::documentclass(std::string const& s, std::string const& options){
	latex_ ="\\documentclass[" + options + "]{" + s + "}\n"; 
}
void Latex::package(std::string const& s, std::string const& options){
	latex_+="\\usepackage[" + options + "]{" + s + "}\n"; 
}
void Latex::begin(std::string const& s, std::string const& options){
	latex_+="\\begin{" + s + "}" + options + "\n"; 
}
void Latex::end(std::string const& s){
	latex_+="\\end{" + s + "}\n"; 
}
void Latex::command(std::string const& s){
	latex_+="\\" + s + "\n"; 
}
void Latex::item(std::string const& s){
	latex_+="\\item " + s + "\n"; 
}

void Latex::operator=(std::string const& s){ latex_ = s + "\n"; }
void Latex::operator+=(std::string const& s){ latex_ += s + "\n"; }

void Latex::save_file(){
	IOFiles w_latex(path_ + filename_+".tex",true,false);
	w_latex<<latex_<<IOFiles::endl;
}

void Latex::pdflatex(bool const& silent){
	Linux command;
	command(Linux::pdflatex(path_,filename_),silent);
	if(command.status()){ std::cerr<<__PRETTY_FUNCTION__<<" : Linux::pdflatex("<<path_<<","<<filename_<<") returned an error ("<<command.status()<<")"<<std::endl; }
}
