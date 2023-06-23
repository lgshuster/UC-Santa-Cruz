// $Id: cxi.cpp,v 1.5 2021-05-18 01:32:29-07 - - $
// Leonid Shuster lshuster
// Anirudh Sridhar asridha2

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

#include <libgen.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "debug.h"
#include "logstream.h"
#include "protocol.h"
#include "socket.h"
#include "util.h"

logstream outlog (cout);
struct cxi_exit: public exception {};

unordered_map<string,cxi_command> command_map {
   {"exit", cxi_command::EXIT},
   {"help", cxi_command::HELP},
   {"ls"  , cxi_command::LS},
   {"get"  , cxi_command::GET},
   {"put"  , cxi_command::PUT},
   {"rm"  , cxi_command::RM}
};

static const char help[] = R"||(
exit         - Exit the program.  Equivalent to EOF.
get filename - Copy remote file to local host.
help         - Print help summary.
ls           - List names of files on remote server.
put filename - Copy local file to remote host.
rm filename  - Remove file from remote server.
)||";

void cxi_help() {
   cout << help;
}

void cxi_ls (client_socket& server) {
   cxi_header header;
   header.command = cxi_command::LS;
   DEBUGF ('h', "sending header " << header << endl);
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   DEBUGF ('h', "received header " << header << endl);
   if (header.command != cxi_command::LSOUT) {
      outlog << "sent LS, server did not return LSOUT" << endl;
      outlog << "server returned " << header << endl;
   }else {
      size_t host_nbytes = ntohl (header.nbytes);
      auto buffer = make_unique<char[]> (host_nbytes + 1);
      recv_packet (server, buffer.get(), host_nbytes);
      DEBUGF ('h', "received " << host_nbytes << " bytes");
      buffer[host_nbytes] = '\0';
      cout << buffer.get();
   }
}

void cxi_get (client_socket& server, string filename) {
   cxi_header header;
   header.command = cxi_command::GET;

   for (long unsigned int i = 0; i < FILENAME_SIZE; ++i) {
      if (i <= filename.length()) {
         header.filename[i] = filename[i];
      }else {
         header.filename[i] = '\0';
      }
   }

   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   
   if (header.command != cxi_command::FILEOUT) {
      outlog << "sent GET, server did not return FILEOUT" << endl;
      outlog << "server returned " << header << endl;
   }else {
      size_t nbytes = ntohl (header.nbytes);
      auto buffer = make_unique<char[]> (nbytes + 1);
      
      recv_packet (server, buffer.get(), nbytes);

      buffer[nbytes] = '\0';

      ofstream file (filename);
      file.write(buffer.get(), nbytes);

      outlog << "File was written succesfully" << endl;
   }
}

void cxi_put (client_socket& server, string filename) {
   struct stat stat_buf;
   int status = stat (filename.c_str(), &stat_buf);
   if (status != 0) {
      outlog << filename <<  ": No such file" << endl;
   }else {
      cxi_header header;
      size_t nbytes = stat_buf.st_size;
      header.command = cxi_command::PUT;
      header.nbytes = htonl (nbytes);

      for (long unsigned int i = 0; i < FILENAME_SIZE; ++i) {
         if (i <= filename.length()) {
            header.filename[i] = filename[i];
         }else {
            header.filename[i] = '\0';
         }
      }

      send_packet (server, &header, sizeof header);
      char * buffer = new char[nbytes];
      ifstream file (filename);
      file.read(buffer, nbytes);
      file.close();

      send_packet (server, buffer, nbytes);
      recv_packet (server, &header, sizeof header);
      delete (buffer);

      if (header.command == cxi_command::NAK) {
         outlog << "sent PUT, server did not return AK" << endl;
      }else {
         outlog << "File was written succesfully" << endl;
      }
   }
}

void cxi_rm (client_socket& server, string filename) {
   cxi_header header;
   header.command = cxi_command::RM;

   for (long unsigned int i = 0; i < FILENAME_SIZE; ++i) {
      if (i <= filename.length()) {
         header.filename[i] = filename[i];
      }else {
         header.filename[i] = '\0';
      }
   }

   DEBUGF ('h', "sending header " << header << endl);
   send_packet (server, &header, sizeof header);
   recv_packet (server, &header, sizeof header);
   DEBUGF ('h', "received header " << header << endl);
   if (header.command == cxi_command::NAK) {
      outlog << "sent RM, server did not return AK" << endl;
   }else {
      outlog << "File was deleted succesfully" << endl;
   }
}




void usage() {
   cerr << "Usage: " << outlog.execname() << " host port" << endl;
   throw cxi_exit();
}

pair<string,in_port_t> scan_options (int argc, char** argv) {
   for (;;) {
      int opt = getopt (argc, argv, "@:");
      if (opt == EOF) break;
      switch (opt) {
         case '@': debugflags::setflags (optarg);
                   break;
      }
   }
   if (argc - optind != 2) usage();
   string host = argv[optind];
   in_port_t port = get_cxi_server_port (argv[optind + 1]);
   return {host, port};
}

int main (int argc, char** argv) {
   outlog.execname (basename (argv[0]));
   outlog << to_string (hostinfo()) << endl;
   try {
      auto host_port = scan_options (argc, argv);
      string host = host_port.first;
      in_port_t port = host_port.second;
      outlog << "connecting to " << host << " port " << port << endl;
      client_socket server (host, port);
      outlog << "connected to " << to_string (server) << endl;
      for (;;) {
         string line;
         getline (cin, line);
         if (cin.eof()) throw cxi_exit();
         outlog << "command " << line << endl;
         wordvec command = split(line, " ");
         const auto& itor = command_map.find (command[0]);
         cxi_command cmd = itor == command_map.end()
                         ? cxi_command::ERROR : itor->second;
         switch (cmd) {
            case cxi_command::EXIT:
               throw cxi_exit();
               break;
            case cxi_command::HELP:
               cxi_help();
               break;
            case cxi_command::LS:
               cxi_ls (server);
               break;
            case cxi_command::GET:
               cxi_get(server, command[1]);
               break;
            case cxi_command::PUT:
               cxi_put(server, command[1]);
               break;
            case cxi_command::RM:
               cxi_rm (server, command[1]);
               break;
            default:
               outlog << line << ": invalid command" << endl;
               break;
         }
      }
   }catch (socket_error& error) {
      outlog << error.what() << endl;
   }catch (cxi_exit& error) {
      DEBUGF ('x', "caught cxi_exit");
   }
   return 0;
}

