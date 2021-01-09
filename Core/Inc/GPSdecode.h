

typedef struct {
    unsigned int   latitude_bd;
    unsigned int   nshemi_bd;
    unsigned int   longitude_bd;
    unsigned int   ewhemi_bd;
} GPSmessage;

typedef struct {
	float      Longitude;
	float      Latitude;
} GPS_Data_TypeDef;

int NMEA_Comma_Pos (int *buf,uint8_t cx);

int NMEA_Pow (int m, int n);

int NMEA_Str2num (int *buf, int *dx);

void NMEA_BDS_GPRMC_Analysis (GPSmessage *gpsmsg, int *buf);
