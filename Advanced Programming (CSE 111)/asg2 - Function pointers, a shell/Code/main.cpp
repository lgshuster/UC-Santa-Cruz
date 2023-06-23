// $Id: main.cpp,v 1.11 2020-10-28 11:37:57-07 - - $

#include <cstdlib>
#include <iostream>
#include <string>
#include <utility>
#include <unistd.h>

using namespace std;

#include "commands.h"
#include "debug.h"
#include "file_sys.h"
#include "util.h"

// scan_options
//    Options analysis:  The only option is -Dflags. 

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            debugflags::setflags (optarg);
            break;
         default:
            complain() << "-" << static_cast<char> (option)
                       << ": invalid option" << endl;
            break;
      }
   }
   if (optind < argc) {
      complain() << "operands not permitted" << endl;
   }
}


// main -
//    Main program which loops reading commands until end of file.

int main (int argc, char** argv) {
   // inode_state state;
   
   // wordvec mkdir1 = {"mkdir", "foo"};
   // wordvec mkdir2 = {"mkdir", "foo/bar"};
   // wordvec mkdir3 = {"mkdir", "foo/bar/baz"};
   // wordvec ls = {"ls"};
   
   // fn_ls(state, ls);
   
   // fn_mkdir(state, mkdir1);
   
   // fn_mkdir(state, mkdir2);
   
   // fn_mkdir(state, mkdir3);
  
   // fn_ls(state, ls);
   
   
   
   
   
   
   // // wordvec ls = {"ls"};
   // // wordvec make = {"make"};
   // // wordvec make1 = {"make", "file1", "sljkdhfglsdfksdf"};
   // // wordvec make2 = {"make", "file2"};
   // wordvec make3 = {"make", "file3"};
   // wordvec make4 = {"make", "file4"};
   // wordvec make5 = {"make", "file5"};
   // wordvec make6 = {"make", "file6"};
   // wordvec make7 = {"make", "file7"};
   // wordvec make8 = {"make", "file8"};
   // wordvec make9 = {"make", "file9"};
   // wordvec make10 = {"make", "file10"};
   // // wordvec make11 = {"make", "file11"};
   // // wordvec mkdir = {"mkdir"};
   // // wordvec mkdir1 = {"mkdir", "dir2"};
   // // wordvec pwd =  {"pwd"};
   // // wordvec cd = {"cd", "dir1"};
   // // wordvec cd1 = {"cd", "dir2"};
   // // wordvec cat = {"cat"};
   
   // wordvec make = {"make", "file1", "this", "is", "a", "test"};
   // wordvec make2 = {"make", "123123"};
   // wordvec cd1 = {"cd", "primary"};
   // wordvec cd2 = {"cd", "secondary"};
   // wordvec cd3 = {"cd", "tertiary"};
   // wordvec cd4 = {"cd"};
   // wordvec cd5 = {"cd", "."};
   // wordvec cd6 = {"cd", ".."};
   // wordvec cd7 = {"cd", "/"};
   // wordvec cd9 = {"cd", "1"};
   // wordvec cd8 = {"cd", "dir4"};
   // wordvec dir_root = {"mkdir", "1"};
   // wordvec dir = {"mkdir", "primary"};
   // wordvec dir2 = {"mkdir", "secondary"};
   // wordvec dir3 = {"mkdir", "tertiary"};
   // wordvec dir4 = {"mkdir", "no_delete"};
   // wordvec middle_dir = {"mkdir", "middle_dir"};
   // wordvec middle_dir2 = {"mkdir", "omiddle_dir2"};
   // wordvec dir5 = {"mkdir", "dir4"};
   // wordvec dir6 = {"mkdir", "dir5"};
   // wordvec dir7 = {"mkdir", "dir6"};
   // wordvec cat = {"cat", "file1"};
   // wordvec cat2 = {"cat", "123123"};
   // wordvec lsr = {"lsr"};    
   // wordvec lsr2 = {"lsr", "/"};
   // wordvec ls = {"ls"};  
   // wordvec ls1 = {"ls", "/"};  
   // wordvec rm = {"rm", "file1"};
   // wordvec rm1 = {"rm", "primary"};
   // wordvec pwd = {"pwd"};  
   // wordvec exit = {"exit", "5"};
   // wordvec rmr = {"rmr", "file1"};
   // wordvec rmr1 = {"rmr", "primary"};
   
   // fn_mkdir(state, dir_root); // mkdir in root
   
   // // fn_ls(state, ls);
   // // cout << endl << endl;
   
   // fn_cd(state, cd9); // cd into 1
   
   // fn_mkdir(state, dir); // mkdir in 1
      
   // // fn_ls(state, ls);
   // // cout << endl << endl;
   
   // fn_cd(state, cd1); // cd into primary
   
   // fn_mkdir(state, dir2); // mkdir in primary
   // fn_mkdir(state, dir4); // mkdir in primary
   // fn_mkdir(state, dir5); // mkdir in primary
   // fn_mkdir(state, middle_dir); // mkdir in primary
   // fn_mkdir(state, middle_dir2); // mkdir in primary
   
   // fn_cd(state, cd8);
   
   // fn_mkdir(state, dir6); // mkdir in dir4
   // fn_mkdir(state, dir7); // mkdir in dir4
   
   // fn_cd(state, cd6);
   
   // fn_make(state, make); // make file in root
   // fn_make(state, make2); // make file in root
   
   // fn_cd(state, cd2); // cd into secondary
   
   // fn_make(state, make3); // make file in root
   // fn_make(state, make4); // make file in root
   // fn_make(state, make5); // make file in root
      
   // fn_mkdir(state, dir3); // mkdir in secondary

   // fn_cd(state, cd3); // cd into tertiary
   
   // fn_make(state, make6); // make file in root
   // fn_make(state, make7); // make file in root
   // fn_make(state, make8); // make file in root
     
   // fn_cd(state, cd7); // cd into root
   
   // fn_cd(state, cd9); // cd into root
   
   // //fn_ls(state, ls);
   // //cout << "BEFORE LSR" << endl << endl;
   
   // fn_lsr(state, lsr2);
   
   // cout << endl;
   // cout << endl;
   // cout << endl;
   
   
   // fn_ls(state, ls);
 
   
   
   
   
   
   
   
   // fn_ls(state, ls); // List root
 
   // fn_mkdir(state, mkdir1); // Make dir1
 
   // fn_ls(state, ls); // List root
 
   // fn_cd(state, cd1);
   
   // fn_ls(state, ls1); // List root
   
   
   
   
   
   
   
   
   
   // fn_cd(state, cd); // Change to root
   
   // fn_ls(state, ls1); // List dir1
   
   
   
   // fn_ls(state, ls); // List root
   
   // fn_ls(state, ls1); // List dir1
   
   // fn_cat(state, cat5);
   
   // wordvec ls = {"ls"};
   // wordvec lsr = {"lsr"};
   // wordvec make = {"make"};
   // wordvec make1 = {"make", "file1", "sljkdhfglsdfksdf"};
   // wordvec make2 = {"make", "file2"};
   // wordvec make3 = {"make", "file3"};
   // wordvec make4= {"make", "file4"};
   // wordvec make5 = {"make", "file5"};
   // wordvec make6 = {"make", "file6"};
   // wordvec make7 = {"make", "file7"};
   // wordvec make8 = {"make", "file8"};
   // wordvec make9 = {"make", "file9"};
   // wordvec make10 = {"make", "file10"};
   // wordvec make11 = {"make", "file11"};
   // wordvec mkdir1 = {"mkdir1", "dir1"};
   // wordvec mkdir2 = {"mkdir2", "dir2"};
   // wordvec pwd =  {"pwd"};
   // wordvec cd = {"cd", "dir1"};
   // wordvec cd1 = {"cd", "dir2"};
   // wordvec cat = {"cat"};
   
   // fn_ls(state, ls); // List root
   
   // fn_make(state, make1);
   // fn_make(state, make2);
   // fn_make(state, make3);
   // fn_make(state, make4);
   // fn_make(state, make5);
   // fn_make(state, make6);
   // fn_make(state, make7);
   // fn_make(state, make8);
   // fn_make(state, make9);
   // fn_make(state, make10);
   // fn_make(state, make11);
   
   // fn_mkdir(state, mkdir1); // Make dir1
   
   // fn_lsr(state, lsr); // List root
   
   // fn_ls(state, ls); // List root
   
   // fn_pwd(state, pwd); // Print root working directory
   
   // fn_cd(state, cd); // Change to dir1
   
   // fn_ls(state, ls); // List dir1
   
   // fn_pwd(state, pwd); // Print dir1 working directory
   
   // fn_mkdir(state, mkdir1); // Make dir2
   
   // fn_ls(state, ls); // List dir1
   
   // fn_cd(state, cd1); // Change to dir2
   
   // fn_ls(state, ls); // List dir2
   
   // fn_pwd(state, pwd); // Print dir2 working directory
   
   // wordvec make = {"make", "file1", "this", "is", "a", "test"};
   // wordvec make2 = {"make", "123123"};
   // wordvec cd1 = {"cd", "primary"};
   // wordvec cd2 = {"cd", "secondary"};
   // wordvec cd3 = {"cd", "tertiary"};
   // wordvec cd4 = {"cd"};
   // wordvec cd5 = {"cd", "."};
   // wordvec cd6 = {"cd", ".."};
   // wordvec cd7 = {"cd", "/"};
   // wordvec dir = {"mkdir", "primary"};
   // wordvec dir2 = {"mkdir", "secondary"};
   // wordvec dir3 = {"mkdir", "tertiary"};
   // wordvec cat = {"cat", "file1"};
   // wordvec cat2 = {"cat", "123123"};
   // wordvec lsr = {"lsr"};    
   // wordvec lsr2 = {"lsr", "."};
   // wordvec ls = {"ls"};  
   // wordvec rm = {"rm", "file1"};
   // wordvec rm1 = {"rm", "primary"};
   // wordvec pwd = {"pwd"};  
   // wordvec exit = {"exit", "5"};
   
   // fn_mkdir(state, dir); // mkdir in root
   
   // fn_cd(state, cd1); // cd into primary
   
   // fn_mkdir(state, dir2); // mkdir in primary
   
   // fn_make(state, make); // make file in root
   
   // fn_cd(state, cd2); // cd into secondary
      
   // fn_mkdir(state, dir3); // mkdir in secondary

   // fn_cd(state, cd3); // cd into tertiary
     
   // fn_cd(state, cd7); // cd into primary

   
   // fn_lsr(state, lsr);
   
   
   
   
   
   
   // fn_ls(state, ls);
      
   // fn_pwd(state, pwd);
   
   // fn_cd(state, cd4); // cd into root
   
   // fn_ls(state, ls);
   
   // fn_pwd(state, pwd);
   
   // fn_make(state, make); // make file in root
   
   // fn_ls(state, ls);
   
   // fn_cat(state, cat); // cat file 
   
   // fn_make(state, make2); // make file in root
   
   // fn_ls(state, ls);
   
   // fn_cat(state, cat2); // cat file
   
   // fn_mkdir(state, dir); // mkdir in root
   
   // fn_ls(state, ls);
   
   // fn_rm(state, rm1);
   
   // fn_ls(state, ls);
   
   // fn_rm(state, rm);
   
   // fn_ls(state, ls);
   
   // fn_mkdir(state, dir); // mkdir in root
   
   // fn_ls(state, ls);
   
   // fn_cd(state, cd1); // cd into primary
   
   // fn_ls(state, ls);
   
   // fn_pwd(state, pwd);
   
   // fn_mkdir(state, dir2); // mkdir in primary
   
   // fn_ls(state, ls);
   
   // fn_cd(state, cd2); // cd into secondary
   
   // fn_ls(state, ls);
   
   // fn_pwd(state, pwd);
   
   // fn_mkdir(state, dir3); // mkdir in secondary
   
   // fn_ls(state, ls);
   
   // fn_cd(state, cd3); // cd into tertiary
   
   // fn_ls(state, ls);
   
   // fn_pwd(state, pwd);
   
   // fn_cd(state, cd6); // cd into primary
   
   // fn_ls(state, ls);
   
   // fn_cd(state, cd7); // cd into primary
   
   // fn_ls(state, ls);
   
   // fn_exit(state, exit);
	
	
   exec::execname (argv[0]);
   cout << boolalpha;  // Print false or true instead of 0 or 1.
   cerr << boolalpha;
   cout << argv[0] << " build " << __DATE__ << " " << __TIME__ << endl;
   scan_options (argc, argv);
   bool need_echo = want_echo();
   inode_state state;
   try {
      for (;;) {
         try {
            // Read a line, break at EOF, and echo print the prompt
            // if one is needed.
            cout << state.prompt();
            string line;
            getline (cin, line);
            if (cin.eof()) {
               if (need_echo) cout << "^D";
               cout << endl;
               DEBUGF ('y', "EOF");
               break;
            }
            if (need_echo) cout << line << endl;
   
            // Split the line into words and lookup the appropriate
            // function.  Complain or call it.
            wordvec words = split (line, " \t");
            DEBUGF ('y', "words = " << words);
			
			string temp = words[0];
					
			if (temp[0] != '#') {
               command_fn fn = find_command_fn (words.at(0));
               fn (state, words);
			}
         }catch (file_error& error) {
            complain() << error.what() << endl;
         }catch (command_error& error) {
            complain() << error.what() << endl;
         }
      }
   } catch (ysh_exit&) {
      // This catch intentionally left blank.
   }

   return exit_status_message();
}
