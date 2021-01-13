
int CONNECT_Server(void);

void ADD_Object(void);

int Registered_Plant(void);

int Reply_Observe_Command(void);

void Parse_Observe_Command(char *buff,char *ret);

int Reply_Discover_Command(void);

void Parse_Discover_Command(char *buff,char *ret);

int SEND_Messag(char *messag);

int Reply_READ_Command(char *messag);

void Parse_READ_Command(char *buff,char *ret);

int LOGOUT_Device(void);

int CLOSE_Server(void);

void ONENET_LWM2M(void);
