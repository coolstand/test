/*
 *This is a test
 *author: junkizou
 *
 *
 */
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <zmq.h>
#include <assert.h>
#include <unistd.h>
using namespace std;

const char* send_address="tcp://127.0.0.1:5555";
const char* recv_address="tcp://127.0.0.1:5556";

int clean(void *context, void *responder)
{
	zmq_close (responder);
	zmq_term (context);

	return 0;
}

int recv(void *context, void *responder)
{
	int nRet = 0;
	std::string strin = "";

	while(1)
	{
		zmq_msg_t request = {0};
		zmq_msg_init (&request);

		nRet = zmq_recvmsg (responder, &request, 0);
		int size = zmq_msg_size (&request);
		if(size != 0)
		{
			char *string = new char [size + 1];
			memset(string,0,size+1);
			memcpy (string, zmq_msg_data (&request), size);
			printf ("\tBBBB: %s\n",string);
			delete []string;

			zmq_msg_close (&request);

			//zmq_msg_t reply = {0};

			std::string strin = "AAAA get";
			if(strin.size() != 0)
			{
				//zmq_msg_init_size (&reply, strin.size());
				//memcpy (zmq_msg_data (&reply), strin.c_str(), strin.size());
				printf("[%s]\n", strin.c_str());

				//zmq_sendmsg (responder, &reply, 0);
				zmq_send (responder, strin.c_str(), strin.size(), 0);
			}
			//zmq_msg_close (&reply);
		}

	}

	return 0;
}

int send(void *context, void *requester)
{
	int nRet = 0;

	while(1)
	{
		//send begin
		string strin = "";
		getline(cin, strin);
		if(strin.size() != 0)
		{
			//zmq_msg_t request={0};
			//zmq_msg_init_size (&request, strin.size());
			//memcpy (zmq_msg_data (&request), strin.c_str(), strin.size());
			printf ("AAAA: %s\n", strin.c_str());

			//zmq_sendmsg (requester, &request, 0);
			zmq_send (requester, strin.c_str(), strin.size(), 0);
			//zmq_msg_close (&request);
		}
		//send end

		//recv begin
		zmq_msg_t reply={0};
		zmq_msg_init (&reply);
		zmq_recvmsg (requester, &reply, 0);

		int size = zmq_msg_size (&reply);
		if(size != 0)
		{
			char *string = new char [size + 1];
			memset(string,0,size+1);
			memcpy (string, zmq_msg_data (&reply), size);
			printf ("[%s]\n",string);
			delete []string;
		}

		zmq_msg_close (&reply);

	}

	return 0;
}

/*this is server*/
int main(int argc, char** argv)
{
	int nRet = 0;

	pid_t pid;
	pid = fork();
	printf("getpid:%d, fork pid:%d\n",getpid(), pid);

	if(pid < 0)
	{
		printf("error here111111111\n");
		return -1;
	}
	else if(pid == 0)
	{
		printf("this is child process\n");
		void *context = zmq_init (ZMQ_IO_THREADS_DFLT);
		if(!context)
		{
			printf("err occured during zmq_init():%s\n",zmq_strerror(nRet));
			return -1;
		}

		printf ("Connecting to server...\n");

		void *requester = zmq_socket (context, ZMQ_REQ );
		nRet = zmq_connect (requester, send_address);
		if(nRet != 0)
		{
			printf("err occued during connet to server:%s\n", zmq_strerror(nRet));
			return -2;
		}
		printf ("Connecting to server on port 5555 success...\n");

		send(context, requester);
		clean(context, requester);

		printf("oh no:child end============\n");
	}
	else
	{
		printf("this is parent process\n");

		void *context = zmq_init (ZMQ_IO_THREADS_DFLT);
		if(!context)
		{
			printf("err occured during zmq_init():%s\n", zmq_strerror(nRet));
			return -1;
		}
		printf("zmq_init passed===========\n");

		void *responder = zmq_socket (context, ZMQ_REP);

		nRet = zmq_bind (responder, recv_address);
		if(nRet != 0)
		{
			printf("err occured during zmq_bind():%s\n", zmq_strerror(nRet));
			return -2;
		}

		printf("binding port success.waiting client send message...res:%d\n", nRet);

		recv(context, responder);
		clean(context, responder);
		printf("oh no:parent end============\n");
	}

	return 0;
}



