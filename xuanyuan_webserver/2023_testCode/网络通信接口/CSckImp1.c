#include"CSckImp1.h"

struct Info {
	char data[1024];
	int len;
};

//初始化
void init_CSocketProtocol(void** handle) {

	if (NULL == handle) {
		return;
	}

	struct Info* info = malloc(sizeof(struct Info));
	memset(info, 0, sizeof(struct Info));

	*handle = info;
}

//发送接口
void send_CSocketProtocol(void* handle, unsigned char* sendData, int sendLen) {
	if (NULL == handle) {
		return;
	}
	if (NULL == sendData) {
		return;
	}
	struct Info* info = (struct Info*)handle;
	
	strcpy(info->data, sendData, sendLen);
	info->len = sendLen;

}

//接收接口
void recv_CSocketProtocol(void* handle, unsigned char* recvData, int* recvLen) {
	if (NULL == handle) {
		return;
	}
	if (NULL == recvData) {
		return;
	}
	if (NULL == recvLen) {
		return;
	}

	struct Info* info = (struct Info*)handle;
	strncpy(recvData, info->data, info->len);
	*recvLen = info->len;
}

//关闭
void close_CSocketProtocol(void* handle) {
	if (NULL == handle) {
		return;
	}
	free(handle);
	handle = NULL;
}