
typedef struct {
	float latitude;
	float longitude;
	float speed;
	int flag;
} dataPoints;

int CONNECT_Server(void);

int SUB_Topic(void);

int PUB_Messag(char *Messag);

int UNSUB_Topic(void);

int Close_Server(void);

int HEX_Mode_Enable(void);

void Messag_Analysis(char *buff);

int Messag_Bispose(void);

void ONENET_MQTT(dataPoints *DP);
