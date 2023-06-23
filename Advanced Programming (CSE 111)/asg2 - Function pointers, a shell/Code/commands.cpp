// $Id: commands.cpp,v 1.20 2021-01-11 15:52:17-08 - - $

#include "commands.h"
#include "debug.h"

inode_ptr get_argument_inode (inode_state& state, wordvec& pathname);
inode_ptr get_argument_inode2 (inode_state& state, wordvec& pathname);

command_hash cmd_hash {
   {"cat"   , fn_cat   },
   {"cd"    , fn_cd    },
   {"echo"  , fn_echo  },
   {"exit"  , fn_exit  },
   {"ls"    , fn_ls    },
   {"lsr"   , fn_lsr   },
   {"make"  , fn_make  },
   {"mkdir" , fn_mkdir },
   {"prompt", fn_prompt},
   {"pwd"   , fn_pwd   },
   {"rm"    , fn_rm    },
   {"rmr"   , fn_rmr   },
};

command_fn find_command_fn (const string& cmd) {
   // Note: value_type is pair<const key_type, mapped_type>
   // So: iterator->first is key_type (string)
   // So: iterator->second is mapped_type (command_fn)
   DEBUGF ('c', "[" << cmd << "]");
   const auto result = cmd_hash.find (cmd);
   if (result == cmd_hash.end()) {
	  exec::status(1);
      throw command_error (cmd + ": no such function");
   }
   return result->second;
}

command_error::command_error (const string& what):
            runtime_error (what) {
}

int exit_status_message() {
   int status = exec::status();
   cout << exec::execname() << ": exit(" << status << ")" << endl;
   return status;
}

void fn_cat (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   if (words.size() == 1) {
	  exec::status(1);
	  
      throw command_error("nothing to cat");
   }
   
   inode_ptr cwd = state.get_cwd();
   
   for (long long unsigned int i = 1; i < words.size(); i++) {
      string argument = words[i];
	  
	  if (cwd->inode_exists(argument)) {
	     inode_ptr argument_inode = cwd->get_inode(argument);  
		  
	     string argument_file_type = argument_inode->get_error_file_type();
		 
		 if (argument_file_type == "directory") {
		    exec::status(1); 
		 }
		 else if (argument_file_type == "plain file") {
			wordvec argument_data = argument_inode->readfile();
			
			for (long long unsigned int j = 0; j < argument_data.size(); j++) {
			   cout << argument_data[j] << " ";
		    }   
			cout << endl;
		 }
	  }
	  else {
	     exec::status(1);  
	  }
   }








   // if (words.size() == 1) {
	  // exec::status(1);
	  
      // throw command_error("nothing to cat");
   // }
   
   // for (long long unsigned int i = 1; i < words.size(); i++) {
      // string argument = words[i];
	  
	  // wordvec argument_split = split(argument, "/");
	  
	  // if (argument[0] == '/') {
	     // argument_split.insert(argument_split.begin(), "/");
	  // }
	  
	  // inode_ptr argument_inode = get_argument_inode(state, argument_split);
	  
	  // string inode_file_type = argument_inode->get_error_file_type();
	  
	  // if (inode_file_type == "directory") {
	     // exec::status(1);
		 
		 // throw command_error("trying to cat a directory");
	  // }
      
	  // wordvec argument_data = argument_inode->readfile();
   
	  // for (long long unsigned int j = 0; j < argument_data.size(); j++) {
	     // cout << argument_data[j];
	  // }   
	  // cout << endl;
   // }
}

void fn_cd (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   
   inode_ptr cwd = state.get_cwd(); // Get current directory

   if (words.size() == 1 || words[1] == "/") { // If no filename or filename is root
      inode_ptr root = state.get_root(); // Get current directory

      state.set_cwd(root);

      state.path_clear();

      state.path_push_back("/");
   }
   else { // Else if filename
      string filename = words[1];

      if (cwd->inode_exists(filename)) { // If filename exists
         inode_ptr new_cwd = cwd->get_inode(filename);

         state.set_cwd(new_cwd);

         if (filename == "..") { // If filename is parent
            state.path_pop_back();
         }
         else { // Else if filename isn't parent
            state.path_push_back(filename);
         }
      }
      else { // Else if filename doesn't exist
         throw file_error (filename + " does not exist");
      }
   }
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
   
  // if (words.size() == 1) { // If no filename
     // inode_ptr root = state.get_root(); // Get current directory
  
     // state.set_cwd(root);
  
     // state.path_clear();    
  
     // state.path_push_back("/");
   // }
   // else { // cd .
      // string argument = words[1];   
	   
	  // wordvec argument_split = split(argument, "/");
	  
	  // if (argument[0] == '/') {
	     // argument_split.insert(argument_split.begin(), "/");
	  // }

      // inode_ptr argument_inode = get_argument_inode(state, argument_split);

      // state.set_cwd(argument_inode);

	  // string argument_location = argument_split.back();	  

	  // if (argument_location == "..") { // If filename is parent
		 // state.path_pop_back();
	  // }
	  // else { // Else if filename isn't parent
		 // state.path_push_back(argument_location);  
	  // }
   // }
}

void fn_echo (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   cout << word_range (words.cbegin() + 1, words.cend()) << endl;
}

void fn_exit (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   
   if (words.size() == 1) {
      exec::status(0);
   }	   
   else {
	  string status = words[1];
	   
      for (long long unsigned int i = 0; i < status.size(); i++) {
	     if (!isdigit(status[i])) {
	         exec::status(127);
			 cout << exit_status_message() << endl;
			 return;
		 }
	  }

	  exec::status(stoi(words[1]));
   }

   cout << exit_status_message() << endl;
   
   throw ysh_exit();
}

void fn_ls (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   
   inode_ptr cwd = state.get_cwd();
   
   inode_ptr argument_inode;

   if (words.size() == 1) { // If no filename
      argument_inode = cwd;
	  
	  wordvec path = state.get_path();
	  
	  string current_directory = path.back();
	  
	  cout << current_directory << ":" << endl;
   } 
   else { // Else if filename
      string argument = words[1];

      if (argument == "/") { // If "/" filename
         argument_inode = state.get_root();
      }
      else { // Else if any other filename
         if (cwd->inode_exists(argument)) {
            argument_inode = cwd->get_inode(argument);
         } 
         else {
            exec::status(1);
			throw file_error (argument + " does not exist");
         }
      }
	  cout << argument << ":" << endl;
   }
   
   argument_inode->print_dirents();
}

void fn_lsr (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   
   for (long long unsigned int j = 1; j < words.size(); j++) {
      string pathname = words[j];
      
   inode_ptr cwd = state.get_cwd(); // Get current directory
   
   base_file_ptr cwd_contents = cwd->get_contents(); // Get current directory base file
   
   map<string,inode_ptr> cwd_dirents = cwd_contents->get_dirents(); // map of current
   
   if (words.size() == 1) { // If no filename
      wordvec path = state.get_path();
      string current_directory = path.back();
	  
	  state.lsr_path_push_back(current_directory);
	  wordvec print_path = state.get_lsr_path();
	  string thing_to_print;
	  for (long long unsigned int i = 0; i < print_path.size(); i++) {
         if (i < print_path.size() -1) { // If root
		    if(print_path[i] == "/"){
			   thing_to_print += print_path[i];
		    }else{
	           thing_to_print = print_path[i] + "/";
			}
	     }
	     else { // Else if every other term
	        thing_to_print += print_path[i];
	     }
      }	  
      cout << thing_to_print << ":" << endl;
	  
	  cwd->print_dirents();
	  
      for (auto key_value = cwd_dirents.begin(); key_value != cwd_dirents.end(); ++key_value) {
         inode_ptr dirents_inode = key_value->second;
		   
         base_file_ptr dirents_contents = dirents_inode->get_contents();
	  
         string file_type = dirents_contents->get_error_file_type();
		   
         if (file_type == "directory" && key_value->first != "."  && key_value->first != "..") {
			wordvec testing = state.get_path();
		    state.set_cwd(key_value->second);	
			
            wordvec string_to_pass = {"lsr"};	
            state.path_push_back(key_value->first); 
			//cout << "AFTER PUSH: " << key_value->first << " SIZE: " << state.path_get_size() << endl;
		    // cout << "LSR CALLED IN THIS PATH: ";
            // fn_pwd(state, pwd);
			// cout << endl;
            fn_lsr(state, string_to_pass);
			state.path_pop_back();
			state.lsr_path_pop_back();
         }
      }   
      state.set_cwd(cwd);
   } else {
	  
	  cout << pathname << ":" << endl;

	  inode_state new_state;
	  inode_ptr current_inode;

      if (pathname == "/") {
		 new_state.set_root(state.get_root());
		 new_state.set_cwd(state.get_root());
		 
		 current_inode = new_state.get_cwd();
	  }
	  else {
		 new_state.set_root(state.get_root());
		 new_state.set_cwd(state.get_cwd());
		  
         wordvec argument_split = split(pathname, "/");
  
         if (pathname[0] == '/') {
	        argument_split.insert(argument_split.begin(), "/");
         }

		 current_inode = get_argument_inode(new_state, argument_split);
	  }

	  current_inode->print_dirents();
   
      map<string,inode_ptr> current_inode_dirents = current_inode->get_dirents(); // map of current
	  
	  
      for (auto key_value = current_inode_dirents.begin(); key_value != current_inode_dirents.end(); ++key_value) {
         inode_ptr dirents_inode = key_value->second;
		   
         base_file_ptr dirents_contents = dirents_inode->get_contents();
	  
         string file_type = dirents_contents->get_error_file_type();
		   
         if (file_type == "directory" && key_value->first != "."  && key_value->first != "..") {
		    // wordvec testing = state.get_path();
		    // state.set_cwd(key_value->second);	 

            string next_directory;
			
			
			
            if (current_inode == new_state.get_root()) {
			   next_directory = pathname + key_value->first;
			}
			else {
			   next_directory = pathname + "/" + key_value->first;	
			}
			

            wordvec string_to_pass = {"lsr", next_directory};	
            // state.path_push_back(key_value->first); 
			// cout << "AFTER PUSH: " << key_value->first << " SIZE: " << state.path_get_size() << endl;
		    // cout << "LSR CALLED IN THIS PATH: ";
            // fn_pwd(state, pwd);
			// cout << endl;
            fn_lsr(new_state, string_to_pass);
			// state.path_pop_back();
         }
      }   
      // state.set_cwd(cwd);	   
   }
   }
}

void fn_make (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);

   if (words.size() == 1) {
	  exec::status(1);
      throw command_error("nothing to make");
   }
   
   string argument = words[1];
  
   wordvec argument_split = split(argument, "/");
  
   if (argument[0] == '/') {
	  argument_split.insert(argument_split.begin(), "/");
   }
  
   string argument_location = argument_split.back();
  
   argument_split.pop_back();
  
   inode_ptr argument_parent_inode = get_argument_inode(state, argument_split);
   
   inode_ptr argument_inode;
   
   if (argument_parent_inode->inode_exists(argument_location)) { // If filename exists
	  argument_inode = argument_parent_inode->get_inode(argument_location);
	  
	  string file_type = argument_inode->get_error_file_type();
	  
      if (file_type == "directory") { // If filename is directory
         exec::status(1);
		 throw file_error (argument_location + " is a directory");
	  }
   }
   else { // Else if filename doesn't exist
      argument_inode = argument_parent_inode->mkfile(argument_location); // Make new plain_file inode
	   
	  argument_parent_inode->add_inode(argument_location, argument_inode);
   }

   wordvec argument_data = words;
   argument_data.erase(argument_data.begin(), argument_data.begin() + 2);
   argument_inode->writefile(argument_data);
}

void fn_mkdir (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   
   if (words.size() == 1) {
      exec::status(1);
	  throw command_error("nothing to make");
   }
   
   string argument = words[1];
  
   wordvec argument_split = split(argument, "/");
  
   if (argument[0] == '/') {
	  argument_split.insert(argument_split.begin(), "/");
   }
  
   string argument_location = argument_split.back();
  
   argument_split.pop_back();
  
   inode_ptr argument_parent_inode = get_argument_inode(state, argument_split);

   if (argument_parent_inode->inode_exists(argument_location)) { // If filename exists
      exec::status(1);
	  throw file_error (argument_location + " already exists as a file or directory");	   
   }
   else { // Else if filename doesn't exist
	  inode_ptr argument_inode = argument_parent_inode->mkdir(argument_location); // Make new directory inode
	   
	  argument_parent_inode->add_inode(argument_location, argument_inode); 
   }
}

void fn_prompt (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   
   string new_prompt;
   int size = words.size();
   
   if (size == 1){
	   state.prompt("% ");
   }
   else {
	  for (int i = 1; i < size; i++){
         //take all the strings and make them into one
         // words[0] = prompt;
         // words[1] = this;
         // words[2] = is;
         // words[3] = a;		 
		 new_prompt = new_prompt + words[i] + " ";
	  }
	  state.prompt(new_prompt);
	  //cout << endl << "new prompt is " << new_prompt << endl << endl;
   }
}

void fn_pwd (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   
   wordvec path = state.get_path();

   for (long long unsigned int i = 0; i < path.size(); i++) {
      if (i == 0) { // If root
	     cout << "/";
	  }
	  else if (i == 1) { // Else if second term
	     cout << path[i];
	  }
	  else { // Else if every other term
	     cout << "/" << path[i];
	  }
   }
   
   cout << endl;
}

void fn_rm (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   
   if(words.size() == 1){
	  exec::status(1);
      throw command_error ("missing operand");
   }else{
      inode_ptr cwd = state.get_cwd();  // inode_ptr to current 

      string filename = words[1];

      // cout << "In rm: " << words[1] << " same as " << filename << endl;

      if (cwd->inode_exists(filename)) {
         inode_ptr rm_inode = cwd->get_inode(filename);

         string file_type = rm_inode->get_error_file_type();

         if (file_type == "directory") {
            cwd->rmdir(filename);
            state.path_pop_back();
         }else if (file_type == "plain file") {
            cwd->rmfile(filename);
         }
      }else{
		 exec::status(1);
         throw file_error (filename + " rm: file doesnt exist");
      }
   }
}

void fn_rmr (inode_state& state, const wordvec& words) {
   DEBUGF ('c', state);
   DEBUGF ('c', words);

	if(words.size() == 1){
	  exec::status(1);
	  throw command_error ("missing operand");
   }else{  
      wordvec pwd = {"pwd"};
      wordvec ls = {"ls"};	
   
      inode_ptr cwd = state.get_cwd();  // inode_ptr to current 
   
      base_file_ptr cwd_contents = cwd->get_contents(); // Get current directory base file
   
      map<string,inode_ptr> cwd_dirents = cwd_contents->get_dirents(); // map of current
   
      string filename = words[1];
   
      if (cwd->inode_exists(filename)) {
         inode_ptr rm_inode = cwd->get_inode(filename);
	  
	     string file_type = rm_inode->get_error_file_type();
	  
         if (file_type == "directory") {
		 
            inode_ptr recur = cwd->get_inode(filename);
   
            map<string,inode_ptr> recur_dirents = recur->get_dirents();
		 	 
	        // recursive call 		
			state.set_cwd(recur);
			if(words.size() == 2){
			   state.path_push_back(filename); 
			}
			// cout << endl;
			// fn_pwd(state, pwd);
			
			for (auto key_value = recur_dirents.begin(); key_value != recur_dirents.end(); ++key_value) {
			
               string curr_file_type = key_value->second->get_error_file_type();
			   
			   // cout << key_value->first << " is a " << curr_file_type << endl;
			
               if (curr_file_type == "directory" && key_value->first != "."  && key_value->first != "..") {	 
                  wordvec rmr = {"rmr", key_value->first};
				  
				  // cout << "rmr called with " << key_value->first << endl;
				  
                  fn_rmr(state, rmr);			  
               }
            }   
	     }
         // iterate the directory and delete all the plain files
 
         inode_ptr del_curr = state.get_cwd();  // inode_ptr to current 
	  
         base_file_ptr del_contents = del_curr->get_contents(); // Get current directory base file

         map<string,inode_ptr> del_dirents = del_contents->get_dirents(); // map of current
	  
	     if (del_dirents.size() == 2) {
	        // cout << endl << "rm called on empty dir: " << filename << endl;
            wordvec string_to_pass = {"rm", filename};
            // fn_pwd(state, pwd);
		 
            inode_ptr temp = rm_inode->get_inode("..");
            state.set_cwd(temp);
	  
            fn_rm(state, string_to_pass);
         }else{
		 
            for(auto del_value = del_dirents.begin(); del_value != del_dirents.end(); ++del_value){
               string del_file_type = del_value->second->get_error_file_type();
			 
               if (del_file_type == "plain file" && del_value->first != "."  && del_value->first != ".."){
                  // cout << endl << "rm plain file called on: " << del_value->first << endl;
                  // fn_pwd(state, pwd);
                  wordvec rm = {"rm", del_value->first};	
                  fn_rm(state, rm);
               }
	   	    }
		    // cout << endl << "rm called on full dir: " << filename << endl;
            wordvec string_to_pass = {"rm", filename};
            // fn_pwd(state, pwd);
   		 
            inode_ptr temp = rm_inode->get_inode("..");
            state.set_cwd(temp);
	  
            fn_rm(state, string_to_pass);
	     }
      }else {
		 exec::status(1); 
         throw file_error (filename + " rmr: file doesnt exist");  
      }   
   }
}

// inode_ptr get_argument_inode (inode_state& state, wordvec& pathname) {
   // inode_ptr current_inode;
   
   // if (pathname[0] == "/") {
      // current_inode = state.get_root();
   // }
   // else {
      // current_inode = state.get_cwd();
   // }  
   
   // for (long long unsigned int i = 1; i < pathname.size(); i++) {
	  // if (current_inode->inode_exists(pathname[i])) {
		 // current_inode = current_inode->get_inode(pathname[i]);  
	  // }
	  // else {
		 // exec::status(1);
		 // cout << "inode" << endl;
		 // throw command_error("no file/directory called " + pathname[i]);
	  // }
   // }   

   // return current_inode;
// }

inode_ptr get_argument_inode (inode_state& state, wordvec& pathname) {
   inode_ptr current_inode;
   
   if (pathname[0] == "/") {
      current_inode = state.get_root();
   }
   else {
      current_inode = state.get_cwd();
   }  
   
   for (long long unsigned int i = 0; i < pathname.size(); i++) {
	  if (pathname[i] != "/") {
	  if (current_inode->inode_exists(pathname[i])) {
		 current_inode = current_inode->get_inode(pathname[i]);  
	  }
	  else {
		 exec::status(1);
		 throw command_error("no file/directory called " + pathname[i]);
	  }
	  }
   }   

   return current_inode;
}

// wordvec split_pathname (const string& pathname) {
   // wordvec path = split(pathname, "/");	
   
   // cout << path.size();

   // return path;
// }



// wordvec vectest = split_pathname(test);

// for (long long unsigned int i = 0; i < vectest.size(); i++) {
  // cout << vectest[i] << endl;   
// }
