

typedef struct {
	uint32_t   latitude_bd;
	uint8_t    nshemi_bd;
	uint32_t   longitude_bd;
	uint8_t    ewhemi_bd;
} GPSmessage;

typedef struct {
	float      Longitude;
	float      Latitude;
} GPS_Data_TypeDef;

uint8_t  NMEA_Comma_Pos (uint8_t *buf, uint8_t cx);

uint32_t NMEA_Pow (uint8_t m, uint8_t n);

int NMEA_Str2num (uint8_t *buf, uint8_t *dx);

void NMEA_BDS_GPRMC_Analysis (GPSmessage *gpsmsg, uint8_t *buf);
