// $Id: file_sys.h,v 1.8 2020-10-22 14:37:26-07 - - $

#ifndef __INODE_H__
#define __INODE_H__

#include <exception>
#include <iostream>
#include <memory>
#include <map>
#include <vector>
using namespace std;

#include "util.h"

// inode_t -
//    An inode is either a directory or a plain file.

enum class file_type {PLAIN_TYPE, DIRECTORY_TYPE};
class inode;
class base_file;
class plain_file;
class directory;
using inode_ptr = shared_ptr<inode>;
using base_file_ptr = shared_ptr<base_file>;
ostream& operator<< (ostream&, file_type);


// inode_state -
//    A small convenient class to maintain the state of the simulated
//    process:  the root (/), the current directory (.), and the
//    prompt.

class inode_state {
   friend class inode;
   friend ostream& operator<< (ostream& out, const inode_state&);
   private:
      inode_ptr root {nullptr};
      inode_ptr cwd {nullptr};
      string prompt_ {"% "};
	  vector<string> path;
	  vector<string> lsr_path;
   public:
      // Constructor/destructor
	  inode_state (const inode_state&) = delete; // copy ctor
      inode_state& operator= (const inode_state&) = delete; // op=
      inode_state();
	  
	  // Getters
	  inode_ptr get_root();
	  inode_ptr get_cwd();
	  void prompt (const string&);
	  
	  // Setters
      const string& prompt() const;
	  void set_root (inode_ptr new_root);
	  void set_cwd (inode_ptr new_cwd);

	  // Public functions
	  void path_clear();
	  void path_push_back (const string& dirname);
	  void path_pop_back();
	  wordvec& get_path();
	  wordvec& get_lsr_path();
      void lsr_path_push_back (const string& dirname);
      void lsr_path_pop_back();
	  
};

// class inode -
// inode ctor -
//    Create a new inode of the given type.
// get_inode_nr -
//    Retrieves the serial number of the inode.  Inode numbers are
//    allocated in sequence by small integer.
// size -
//    Returns the size of an inode.  For a directory, this is the
//    number of dirents.  For a text file, the number of characters
//    when printed (the sum of the lengths of each word, plus the
//    number of words.
//    

class inode {
   friend class inode_state;
   private:
      static size_t next_inode_nr;
      size_t inode_nr;
      base_file_ptr contents;
   public:
      // Constructor
	  inode (file_type);

	  // Getters
	  size_t get_inode_nr() const;
	  base_file_ptr get_contents();
	  map<string,inode_ptr> get_dirents();
	  inode_ptr get_inode(const string& name);
	  const string& get_error_file_type() const;
		
      // Setters
	  void set_dirents (map<string,inode_ptr> new_dirents);
	  void set_self_and_parent (inode_ptr self, inode_ptr parent);
     
	  // Public functions
	  bool inode_exists (const string& name);
	  void add_inode (const string& name, inode_ptr new_inode);
	  void print_dirents();
	  void writefile (const wordvec& newdata);
	  const wordvec& readfile() const;
	  inode_ptr mkdir (const string& dirname);
	  inode_ptr mkfile (const string& filename);
	  void rmdir (const string& dirname);
	  void rmfile (const string& filename);
};


// class base_file -
// Just a base class at which an inode can point.  No data or
// functions.  Makes the synthesized members useable only from
// the derived classes.

class file_error: public runtime_error {
   public:
      explicit file_error (const string& what);
};

class base_file {
   protected:
      base_file() = default;
      virtual const string& error_file_type() const = 0;
   public:
	  // Constructor/destructor
	  virtual ~base_file() = default;
	  base_file (const base_file&) = delete;
      base_file& operator= (const base_file&) = delete;
	  
	  // Public variables
	  weak_ptr<inode> self;
      weak_ptr<inode> parent;
	  
	  // Getters
	  virtual map<string,inode_ptr> get_dirents();
	  virtual const string& get_error_file_type() const;
	  
	  // Setters
	  virtual void set_dirents (map<string,inode_ptr> new_dirents);

	  // Public functions
      virtual size_t size() const = 0;
      virtual const wordvec& readfile() const;
      virtual void writefile (const wordvec& newdata);
      virtual void remove (const string& name);
      virtual inode_ptr mkdir (const string& dirname);
      virtual inode_ptr mkfile (const string& filename);
};


// class plain_file -
// Used to hold data.
// synthesized default ctor -
//    Default vector<string> is a an empty vector.
// readfile -
//    Returns a copy of the contents of the wordvec in the file.
// writefile -
//    Replaces the contents of a file with new contents.

class plain_file: public base_file {
   private:
      wordvec data;
      virtual const string& error_file_type() const override {
         static const string result = "plain file";
         return result;
      }
   public:
      // Getters
	  virtual const string& get_error_file_type() const override;
	  
	  
	  // Public functions
	  virtual size_t size() const override;
      virtual const wordvec& readfile() const override;
      virtual void writefile (const wordvec& newdata) override;
};

// class directory -
// Used to map filenames onto inode pointers.
// default ctor -
//    Creates a new map with keys "." and "..".
// remove -
//    Removes the file or subdirectory from the current inode.
//    Throws an file_error if this is not a directory, the file
//    does not exist, or the subdirectory is not empty.
//    Here empty means the only entries are dot (.) and dotdot (..).
// mkdir -
//    Creates a new directory under the current directory and 
//    immediately adds the directories dot (.) and dotdot (..) to it.
//    Note that the parent (..) of / is / itself.  It is an error
//    if the entry already exists.
// mkfile -
//    Create a new empty text file with the given name.  Error if
//    a dirent with that name exists.

class directory: public base_file {
   private:
      // Must be a map, not unordered_map, so printing is lexicographic
      map<string,inode_ptr> dirents;
      virtual const string& error_file_type() const override {
         static const string result = "directory";
         return result;
      }
   public:
	  // Getters
	  virtual map<string,inode_ptr> get_dirents() override;
	  virtual const string& get_error_file_type() const override;
	  
	  // Setters
	  virtual void set_dirents (map<string,inode_ptr> new_dirents) override;
	  
	  // Public functions
      virtual size_t size() const override;
      virtual void remove (const string& name) override;
      virtual inode_ptr mkdir (const string& dirname) override;
      virtual inode_ptr mkfile (const string& filename) override;
};

#endif

