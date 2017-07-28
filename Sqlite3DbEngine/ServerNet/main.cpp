#include "ServerNet.h"




int main()
{
	CServerNet serverNet;
	int iRlt = serverNet.Init("127.0.0.1",8888);
	if (iRlt == 0)
	{
		printf("init ok...\n");
		serverNet.Run();
	}
	else
		printf("serverNet init failed with error : %d\n",iRlt);
	system("pause");
	return 0;
}