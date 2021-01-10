
typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;

typedef struct {
    u8  num;                   //Satellite code
    u8  eledeg;                //Satellite elevation
    u16 azideg;                //Satellite azimuth
    u8  sn;                    //Signal to noise ratio
} __attribute__((__packed__)) nmea_slmsg;

typedef struct {
    u16 year;
    u8  month;
    u8  date;
    u8  hour;
    u8  min;
    u8  sec;
} __attribute__((packed))  nmea_utc_time;

typedef struct {
    u8 svnum;                  //Satellite visible
    nmea_slmsg slmsg[12];      //NOTE: max 12 satellites
    nmea_utc_time utc;         //UTC time
    u32 latitude;              //Latitude * 100000
    u8 nshemi;                 //South/North
    u32 longitude;             //Longitude * 100000
    u8 ewhemi;                 //West/East
    u8 gpssta;

    /**
     * GPS status:
     * 0: no signal
     * 1: non differential positioning
     * 2: differentiation positioning
     * 6: estimating
     */

    u8 posslnum;               //Satellite counter 0~12
    u8 possl[12];              //Satellite Code locating
    u8 fixmode;

    /**
     * Locating status:
     * 1: non
     * 2: 2D
     * 3: 3D
     */

    u16 pdop;                  //Position accuracy factor     0~500, to real 0~50.0
    u16 hdop;                  //Horizontal precision factor  0~500, to real 0~50.0
    u16 vdop;                  //Vertical precision factor    0~500, to real 0~50.0
    int altitude;              //Altitude * 10
    u16 speed;                 //Speed * 1000
} __attribute__((packed)) nmea_msg;

u8  NMEA_Comma_Pos (u8 *buf, u8 cx);

u32 NMEA_Pow (u8 a, u8 n);

int NMEA_StrToNum (u8 *buf, u8*dx);

void NMEA_GPGSV_Analysis (nmea_msg *gpsx, u8 *buf);

void NMEA_GPGGA_Analysis (nmea_msg *gpsx,u8 *buf);
