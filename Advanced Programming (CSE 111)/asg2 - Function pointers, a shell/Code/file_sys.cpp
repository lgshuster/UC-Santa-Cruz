// $Id: file_sys.cpp,v 1.10 2021-04-10 14:23:40-07 - - $

#include <cassert>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <map>

using namespace std;

#include "debug.h"
#include "file_sys.h"

size_t inode::next_inode_nr {1};

ostream& operator<< (ostream& out, file_type type) {
   switch (type) {
      case file_type::PLAIN_TYPE: out << "PLAIN_TYPE"; break;
      case file_type::DIRECTORY_TYPE: out << "DIRECTORY_TYPE"; break;
      default: assert (false);
   };
   return out;
}

///////////////////////////// inode_state /////////////////////////////
// Constructor

inode_state::inode_state() {
   DEBUGF ('i', "root = " << root << ", cwd = " << cwd
          << ", prompt = \"" << prompt() << "\"");
		  
   root = make_shared<inode>(file_type::DIRECTORY_TYPE);
   root->set_self_and_parent(root, root);
 
   path_push_back("/");
 
   cwd = root;  
}

// Getters

inode_ptr inode_state::get_root() { 
   return root; 
}
	
inode_ptr inode_state::get_cwd() { 
   return cwd; 
}	

void inode_state::prompt (const string& prompt) { 
   prompt_ = prompt; 
}


// Setters

const string& inode_state::prompt() const { 
   return prompt_; 
}

void inode_state::set_root (inode_ptr new_root) {
   root = new_root;  
}


void inode_state::set_cwd (inode_ptr new_cwd) { 
   cwd = new_cwd; 
}


// Public functions

void inode_state::path_clear() { 
   path.clear(); 
}

void inode_state::path_push_back (const string& dirname) { 
   path.push_back(dirname); 
}

void inode_state::path_pop_back() { 
   path.pop_back(); 
}

wordvec& inode_state::get_path() { 
   return path; 
}

void inode_state::lsr_path_pop_back() { 
   lsr_path.pop_back(); 
}

void inode_state::lsr_path_push_back (const string& dirname) { 
   lsr_path.push_back(dirname); 
}

wordvec& inode_state::get_lsr_path() { 
   return lsr_path; 
}


ostream& operator<< (ostream& out, const inode_state& state) {
   out << "inode_state: root = " << state.root
       << ", cwd = " << state.cwd;
   return out;
}





///////////////////////////// inode /////////////////////////////
// Constructor

inode::inode(file_type type): inode_nr (next_inode_nr++) {
   switch (type) {
      case file_type::PLAIN_TYPE:
           contents = make_shared<plain_file>();
           break;
      case file_type::DIRECTORY_TYPE:
           contents = make_shared<directory>();
           break;
      default: assert (false);
   }
   DEBUGF ('i', "inode " << inode_nr << ", type = " << type);
}


// Getters

size_t inode::get_inode_nr() const {
   DEBUGF ('i', "inode = " << inode_nr);
   
   return inode_nr;
}

base_file_ptr inode::get_contents() {
   return contents;	
}

map<string,inode_ptr> inode::get_dirents() {
   return contents->get_dirents();
}

inode_ptr inode::get_inode (const string& name) {
   	map<string,inode_ptr> inode_dirents = get_dirents();
	
	return inode_dirents[name];
}

const string& inode::get_error_file_type() const {
   return contents->get_error_file_type();
}



// Setters

void inode::set_dirents (map<string,inode_ptr> new_dirents) {
   contents->set_dirents(new_dirents);
}

void inode::set_self_and_parent (inode_ptr self, inode_ptr parent) {
   contents->self = self;
   contents->parent = parent;
   
   map<string,inode_ptr> new_dirents = get_dirents();
   
   new_dirents["."] = self;
   new_dirents[".."] = parent;
   
   set_dirents(new_dirents);
}



// Public functions

bool inode::inode_exists (const string& name) {
   map<string,inode_ptr> inode_dirents = get_dirents();  

   if (inode_dirents.find(name) != inode_dirents.end()) {
      return true;   
   } 
   else {
      return false;   
   }
}

void inode::add_inode (const string& name, inode_ptr new_inode) {
   	map<string,inode_ptr> inode_dirents = get_dirents();
	
	inode_dirents[name] = new_inode;
}

void inode::print_dirents() {
   // Print directory contents
   map<string,inode_ptr> inode_dirents = get_dirents();
   
   for (auto string_inode_pair = inode_dirents.begin(); string_inode_pair != inode_dirents.end(); ++string_inode_pair) {
	   inode_ptr current_inode = string_inode_pair->second;
	   
	   size_t current_inode_nr = current_inode->get_inode_nr();
	   
	   base_file_ptr inode_contents = current_inode->get_contents();
	   
	   string file_type = inode_contents->get_error_file_type();
	   
	   size_t contents_size = inode_contents->size();
	   
	   if (file_type == "directory") {
		  cout << right << setw(6) << current_inode_nr << right << setw(6) << contents_size << "  " << string_inode_pair->first << "/" << endl;  
	   } else {
		  cout << right << setw(6) << current_inode_nr << right << setw(6) << contents_size << "  " << string_inode_pair->first << endl;
	   }
   }	
}

void inode::writefile (const wordvec& newdata) {
   contents->writefile(newdata);	
}

const wordvec& inode::readfile() const {
   	return contents->readfile();
}

inode_ptr inode::mkdir (const string& dirname) {
	return contents->mkdir(dirname);
}

inode_ptr inode::mkfile (const string& filename) {
   return contents->mkfile(filename);	
}

void inode::rmdir (const string& dirname) {
   inode_ptr inode_child = get_inode(dirname);
   
   map<string,inode_ptr> inode_child_dirents = inode_child->get_dirents();
   
   // cout << inode_child_dirents.size() << endl;
	
   if (inode_child_dirents.size() != 2) {
      throw file_error (dirname + " rm: dir not empty");
   }
   else {
	  contents->remove(dirname);
   }
}

void inode::rmfile (const string& filename) {
   	contents->remove(filename);
}






///////////////////////////// file_error /////////////////////////////

file_error::file_error (const string& what):
            runtime_error (what) {
}





///////////////////////////// base_file /////////////////////////////
// Getters

map<string,inode_ptr> base_file::get_dirents() {
   throw file_error ("is a " + error_file_type());
}

const string& base_file::get_error_file_type() const {
	throw file_error ("is a " + error_file_type());
}


// Setters

void base_file::set_dirents (map<string,inode_ptr>) {
   throw file_error ("is a " + error_file_type());
}


// Public functions

const wordvec& base_file::readfile() const {
   throw file_error ("is a " + error_file_type());
}

void base_file::writefile (const wordvec&) {
   throw file_error ("is a " + error_file_type());
}

void base_file::remove (const string&) {
   throw file_error ("is a " + error_file_type());
}

inode_ptr base_file::mkdir (const string&) {
   throw file_error ("is a " + error_file_type());
}

inode_ptr base_file::mkfile (const string&) {
   throw file_error ("is a " + error_file_type());
}








///////////////////////////// plain_file /////////////////////////////
// Getters
const string& plain_file::get_error_file_type() const{
	return error_file_type();
}

// Public functions
size_t plain_file::size() const {
   size_t size {0};
   
   // for (vector<string>::iterator word = data.begin(); word != data.end(); word++) {
	     // size += word.length();
   // }
   for(long unsigned int i = 0; i < data.size(); i++){
      size += data[i].length();
   }
   
   DEBUGF ('i', "size = " << size);
   return size;
}

const wordvec& plain_file::readfile() const {
   DEBUGF ('i', data);
   return data;
}

void plain_file::writefile (const wordvec& words) {
   DEBUGF ('i', words);
   
   data = words;
}







///////////////////////////// directory /////////////////////////////
// Getters
map<string,inode_ptr> directory::get_dirents() { 
   return dirents;
}

const string& directory::get_error_file_type() const {
	return error_file_type();
}




// Setters
void directory::set_dirents (map<string,inode_ptr> new_dirents) { 
   dirents = new_dirents;
}



// Public functions

size_t directory::size() const {
   size_t size {0};
   
   size = dirents.size();
   
   DEBUGF ('i', "size = " << size);
   return size;
}

void directory::remove (const string& name) {
   DEBUGF ('i', name);
   
   dirents.erase(name);
}

inode_ptr directory::mkdir (const string& dirname) {
   DEBUGF ('i', dirname);
   
   inode_ptr dir = make_shared<inode>(file_type::DIRECTORY_TYPE);
   
   dir->set_self_and_parent(dir, self.lock());

   dirents[dirname] = dir;
 
   return dir;
}

inode_ptr directory::mkfile (const string& filename) {
   DEBUGF ('i', filename);
   
   inode_ptr plain_file = make_shared<inode>(file_type::PLAIN_TYPE);
   
   dirents[filename] = plain_file;

   return plain_file;
}


