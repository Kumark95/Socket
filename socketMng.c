#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define ADDRESS     "mysocket"  /* addr to connect */

// Create a socket and initialize it to be able to accept 
// connections.
// It returns the virtual device associated to the socket that should be used 
// in the accept system call, to get the virtual device associated to 
// the connection
//

int createServerSocket (int port)
{
	/* Crea el socket */
	int sock = socket(PF_UNIX,SOCK_STREAM,0);
	if (sock < 0) {
		perror("socket");
		exit(EXIT_FAILURE);
	}


//http://troydhanson.github.io/network/Unix_domain_sockets.html

	struct sockaddr_un addrSock;
	memset(&addrSock, 0, sizeof(addrSock));
	addrSock.sun_family = AF_UNIX;
	strcpy(addrSock.sun_path, "socket", sizeof(addrSock.sun_path)-1);

	/* The size of the address is
	 the offset of the start of the filename,
	 plus its length (not including the terminating null byte).
	 Alternatively you can just do:
	 size = SUN_LEN (&name);
	*/
	//size_t size = (offsetof (struct sockaddr_un, sun_path)
		  //+ strlen (addrSock.sun_path));

	if (bind (sock, (struct sockaddr *) &addrSock, sizeof(addrSock)) < 0)
	{
	  perror ("bind");
	  exit (EXIT_FAILURE);
	}

	if (listen(sock, 5) < 0) {
	  perror ("listen");
	  exit (EXIT_FAILURE);
	}

	return accept(sock, NULL, NULL);
	
}


// Returns the file descriptor associated to the connection.
// accept system call will fill the socketAddr parameter
// with the address of the socket for the client which is requesting the
// connection, and the addrSize parameter with the size of that address.

int
acceptNewConnections (int socket_fd)
{

}

// Returns the socket virtual device that the client should use to access 
// the socket, if the connection is successfully established 
// and -1 in case of error
//
// Connect system call requires the address of the 
// server socket to request the connection and the size of that address.
//

int
clientConnection (char *host_name, int port)
{

  struct sockaddr_in serv_addr;
  struct hostent * hent;
  int socket_fd;
  int ret;

  //creates the virtual device for accessing the socket
  socket_fd = socket (AF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0)
    return socket_fd;

  memset((char *) &serv_addr, 0, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  hent = gethostbyname(host_name);
  if (hent == NULL) {
        close (socket_fd);
	return -1;

  }
  memcpy((char *)&serv_addr.sin_addr.s_addr, (char *) hent->h_addr, hent->h_length);
  serv_addr.sin_port = htons(port);
  serv_addr.sin_family = PF_INET; 

  ret = connect (socket_fd, (struct sockaddr *) &serv_addr, sizeof (serv_addr));
  if (ret < 0)
  {
	  close (socket_fd);
	  return (ret);
  } 

  return socket_fd;

}


int deleteSocket (int socket_fd)
{
  close (socket_fd);
}

