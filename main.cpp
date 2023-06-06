#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <winSock2.h>

/*******************************************************************************
* プロトタイプ宣言(こんな関数を用意してありますよ宣言/関数の説明書)
*******************************************************************************/
/*******************************************************************************
* 構造体定義
*******************************************************************************/
/*******************************************************************************
* グローバル変数
*******************************************************************************/
#pragma comment(lib, "ws2_32.lib")
#define hight (15)
#define width (41)
char field[hight][width] = {	// 表示用fieldデータ（横を+1しているのは文末のNULLの分）
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

char drawfield[hight][width] = {	// 表示用fieldデータ（横を+1しているのは文末のNULLの分）
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

	//キャラクターの位置情報
	int p_x, p_y;


	// ①ポート番号の設定
	printf("ポート番号:");
	gets_s(buff);
	port_no = atoi(buff);
	printf("\n");

	// ②WinSock初期化
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		printf("WinSock初期化に失敗しました\n");
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
	printf("WSAStartup成功です\n");
	gethostname(szBuf, (int)sizeof(szBuf));
	printf("\nホスト名=%s\n", szBuf);
	lpHost = gethostbyname(szBuf);
	memcpy(&inaddr, lpHost->h_addr_list[0], 4);
	strcpy(szIP, inet_ntoa(inaddr));
	printf("IPアドレス=%s\n", szIP);


	// ③ソケット作成
	server_socket = socket(AF_INET, SOCK_STREAM, 0);	
	//第1引数
	// 通信を行うドメインを指定する
	// 種類が複数あるが、基本的には
	// AF_INET(IPv4インターネットプロトコル)を指定すれば良い
	// 
	//第2引数
	//SOCK_STREAM 接続型通信（TCP）の形式で通信を行う
	//SOCK_DGRAM 非接続型通信（UDP）
	//
	//第3引数
	//プロトコルの指定
	//0を指定するとwinsockが自動的に接続してくれる
	if (server_socket < 0)
	{
		printf("ソケットオープンエラー\n");
		getchar();
		WSACleanup();
		return -5;
	}

	// ④ソケットとポートの結びつけ
	memset(&sock_addr_in, 0, sizeof(SOCKADDR_IN));
	sock_addr_in.sin_family = AF_INET;
	sock_addr_in.sin_port = htons(port_no);
	sock_addr_in.sin_addr.s_addr = INADDR_ANY;
	if (bind(server_socket, (SOCKADDR*)&sock_addr_in, sizeof(sock_addr_in)) ==
		SOCKET_ERROR)
	{
		printf("バインドエラー\n");
		getchar();
		closesocket(server_socket);
		WSACleanup();
		return -1;
	}

	// ⑤ソケットを待機状態に変更
	if (listen(server_socket, 0) == SOCKET_ERROR)
	{
		printf("待機状態変更失敗\n");
		getchar();
		closesocket(server_socket);
		WSACleanup();
		return -2;
	}

	SOCKADDR_IN from;
	int from_len = sizeof(from);
	SOCKET accept_socket;

	// ⑥クライアントから接続受け入れ
	accept_socket = accept(server_socket, (SOCKADDR*)&from, &from_len);
	if (accept_socket == INVALID_SOCKET)
	{
		printf("accept error\n");
		closesocket(server_socket);
		WSACleanup();
		return -3;
	}

	printf("%sが接続してきました\n", inet_ntoa(from.sin_addr));
	printf("受信を開始します\n終了は end です\n");

	p_x = 2;
	p_y = 2;


	while (true)
	{
		// ⑦データ受信
		int result = recv(accept_socket, buff, sizeof(buff) - 1, 0);
		if (result == SOCKET_ERROR)
		{
			printf("受信エラーです\n");
			break;
		}

		buff[result] = '\0';

		if (strcmp(buff, "end") == 0)
		{
			printf("クライアントが接続を切りました\n");
			break;
		}
		printf("受信:%s\n", buff);


		/* 画面クリア */
		system("cls");
		memcpy(drawfield, field, sizeof(field));

		//キャラクターの移動
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

		//移動した場所を反映
		drawfield[p_y][p_x] = 'P';

		//１行ずつ描画
		for (int i = 0; i < 16; i++)
		{
			printf("%s\n", &drawfield[i][0]);
		}

	}

	// ⑧ソケットの送受信停止
	shutdown(accept_socket, SD_BOTH);
	// ⑨ソケットを閉じる
	closesocket(accept_socket);
	closesocket(server_socket);
	// ⑩WinSock終了
	WSACleanup();
	return 0;
}

