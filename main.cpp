#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <winSock2.h>

/*******************************************************************************
* �v���g�^�C�v�錾(����Ȋ֐���p�ӂ��Ă���܂���錾/�֐��̐�����)
*******************************************************************************/
/*******************************************************************************
* �\���̒�`
*******************************************************************************/
/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
#pragma comment(lib, "ws2_32.lib")
#define hight (15)
#define width (41)
char field[hight][width] = {	// �\���pfield�f�[�^�i����+1���Ă���͕̂�����NULL�̕��j
	"****************************************",
	"*                                      *",
	"*                                      *",
	"*                                      *",
	"*                                      *",
	"*                                      *",
	"*                                      *",
	"*                                      *",
	"*                                      *",
	"*                                      *",
	"*                                      *",
	"*                                      *",
	"*                                      *",
	"*                                      *",
	"****************************************",
};

char drawfield[hight][width] = {	// �\���pfield�f�[�^�i����+1���Ă���͕̂�����NULL�̕��j
	"****************************************",
	"*                                      *",
	"*                                      *",
	"*                                      *",
	"*                                      *",
	"*                                      *",
	"*                                      *",
	"*                                      *",
	"*                                      *",
	"*                                      *",
	"*                                      *",
	"*                                      *",
	"*                                      *",
	"*                                      *",
	"****************************************",
};

int main(void)
{
	WSADATA wsa_data;

	HOSTENT *lpHost;
	IN_ADDR inaddr;
	char szBuf[256], szIP[256];

	SOCKET server_socket;
	char buff[1024];
	unsigned short port_no = 0;
	SOCKADDR_IN sock_addr_in;

	//�L�����N�^�[�̈ʒu���
	int p_x, p_y;


	// �@�|�[�g�ԍ��̐ݒ�
	printf("�|�[�g�ԍ�:");
	gets_s(buff);
	port_no = atoi(buff);
	printf("\n");

	// �AWinSock������
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		printf("WinSock�������Ɏ��s���܂���\n");
		getchar();
		return -4;
	}
	printf("wVersion = %d.%d\n",
		LOBYTE(wsa_data.wVersion),
		HIBYTE(wsa_data.wVersion));
	printf("wHighVersion = %d.%d\n",
		LOBYTE(wsa_data.wHighVersion),
		HIBYTE(wsa_data.wHighVersion));
	printf("szDescription = %s\n", wsa_data.szDescription);
	printf("szSystemStatus = %s\n", wsa_data.szSystemStatus);
	printf("iMaxSockets= %d\n", wsa_data.iMaxSockets);
	printf("iMaxUdpDg = %d\n", wsa_data.iMaxUdpDg);
	printf("WSAStartup�����ł�\n");
	gethostname(szBuf, (int)sizeof(szBuf));
	printf("\n�z�X�g��=%s\n", szBuf);
	lpHost = gethostbyname(szBuf);
	memcpy(&inaddr, lpHost->h_addr_list[0], 4);
	strcpy(szIP, inet_ntoa(inaddr));
	printf("IP�A�h���X=%s\n", szIP);


	// �B�\�P�b�g�쐬
	server_socket = socket(AF_INET, SOCK_STREAM, 0);	
	//��1����
	// �ʐM���s���h���C�����w�肷��
	// ��ނ��������邪�A��{�I�ɂ�
	// AF_INET(IPv4�C���^�[�l�b�g�v���g�R��)���w�肷��Ηǂ�
	// 
	//��2����
	//SOCK_STREAM �ڑ��^�ʐM�iTCP�j�̌`���ŒʐM���s��
	//SOCK_DGRAM ��ڑ��^�ʐM�iUDP�j
	//
	//��3����
	//�v���g�R���̎w��
	//0���w�肷���winsock�������I�ɐڑ����Ă����
	if (server_socket < 0)
	{
		printf("�\�P�b�g�I�[�v���G���[\n");
		getchar();
		WSACleanup();
		return -5;
	}

	// �C�\�P�b�g�ƃ|�[�g�̌��т�
	memset(&sock_addr_in, 0, sizeof(SOCKADDR_IN));
	sock_addr_in.sin_family = AF_INET;
	sock_addr_in.sin_port = htons(port_no);
	sock_addr_in.sin_addr.s_addr = INADDR_ANY;
	if (bind(server_socket, (SOCKADDR*)&sock_addr_in, sizeof(sock_addr_in)) ==
		SOCKET_ERROR)
	{
		printf("�o�C���h�G���[\n");
		getchar();
		closesocket(server_socket);
		WSACleanup();
		return -1;
	}

	// �D�\�P�b�g��ҋ@��ԂɕύX
	if (listen(server_socket, 0) == SOCKET_ERROR)
	{
		printf("�ҋ@��ԕύX���s\n");
		getchar();
		closesocket(server_socket);
		WSACleanup();
		return -2;
	}

	SOCKADDR_IN from;
	int from_len = sizeof(from);
	SOCKET accept_socket;

	// �E�N���C�A���g����ڑ��󂯓���
	accept_socket = accept(server_socket, (SOCKADDR*)&from, &from_len);
	if (accept_socket == INVALID_SOCKET)
	{
		printf("accept error\n");
		closesocket(server_socket);
		WSACleanup();
		return -3;
	}

	printf("%s���ڑ����Ă��܂���\n", inet_ntoa(from.sin_addr));
	printf("��M���J�n���܂�\n�I���� end �ł�\n");

	p_x = 2;
	p_y = 2;


	while (true)
	{
		// �F�f�[�^��M
		int result = recv(accept_socket, buff, sizeof(buff) - 1, 0);
		if (result == SOCKET_ERROR)
		{
			printf("��M�G���[�ł�\n");
			break;
		}

		buff[result] = '\0';

		if (strcmp(buff, "end") == 0)
		{
			printf("�N���C�A���g���ڑ���؂�܂���\n");
			break;
		}
		printf("��M:%s\n", buff);


		/* ��ʃN���A */
		system("cls");
		memcpy(drawfield, field, sizeof(field));

		//�L�����N�^�[�̈ړ�
		if ((strcmp(buff, "w") == 0) &&(p_y<hight))
		{
			p_y -= 1;
		}
		if ((strcmp(buff, "a") == 0) && (p_x<width - 1))
		{
			p_x -= 1;
		}
		if ((strcmp(buff, "s") == 0) && (p_y < hight))
		{
			p_y += 1;
		}
		if ((strcmp(buff, "d") == 0) && (p_x < width - 1))
		{
			p_x += 1;
		}

		//�ړ������ꏊ�𔽉f
		drawfield[p_y][p_x] = 'P';

		//�P�s���`��
		for (int i = 0; i < 16; i++)
		{
			printf("%s\n", &drawfield[i][0]);
		}

	}

	// �G�\�P�b�g�̑���M��~
	shutdown(accept_socket, SD_BOTH);
	// �H�\�P�b�g�����
	closesocket(accept_socket);
	closesocket(server_socket);
	// �IWinSock�I��
	WSACleanup();
	return 0;
}

