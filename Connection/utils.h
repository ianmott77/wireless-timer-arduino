#ifndef UTILS_H
#define UTILS_H

#include "Connection.h"
#include "TimingEye.h"
#define DELAY_ARR 10
#define START 10

#define MONITOR 5
#define TRIGGER 6

/******Race Mode functions********/

static volatile bool receivedStartParams = false;
static bool deviceTypeSent = false;
static bool intervalLoop = false;
static int bib = -1;
static int timeCount = 0;
static int startParamData = -1;
static int deviceType = getDeviceType();
static unsigned long start = 0;
static unsigned long interval = 0;
static unsigned long nextInterval = 0;
static int ms = 0;
static bool seqStarted = false;
static int seqStatus = -1;
static uint8_t prevAdd = getPrevAddress();
static uint8_t nextAdd = getNextAddress();
static int startInfoCount = 0;
static int newRacerConf = 0;
static int racerCount = 0;
static int racerInfoCount = 0;
static bool dnfStatus = false;
static int dnfBib = 0;
static int recParamsCount = 0;
static int raceMode = 0;
static int paceInterval = 10000;
static unsigned long paceTime = 0;
static unsigned long lastTime = 1;

//general functions
int getRaceMode();
void timing_mode();
Packet * send_device_type();
void send_device_type_cb();
void rec_start_params(Packet*);
void rec_start_params_cb();
void timing_mode_init();
bool hasReceivedStartParams();
bool hasDeviceTypeSent();
int getPaceInterval();
void setPaceInterval(int);
int getPaceTime();
void setPaceTime(int);
void rec_pace_time(Packet*);
void rec_pace_time_cb();
void rec_confirm_time(Packet*);
void rec_confirm_time_cb();

//starter
void starter();
Packet * starter_init_timing_seq();
void starter_init_timing_seq_cb();
Packet * starter_send_bib();
void starter_send_bib_cb();
Packet * send_bib();
void send_bib_cb();
Packet * starter_send_start_millis();
void starter_send_start_millis_cb();
Packet * starter_send_race_type();
void starter_send_race_type_cb();
void rec_seq_started(Packet *);
void rec_seq_started_cb();
void rec_millis_int(Packet *);
void rec_millis_int_cb();
void setStartTime(unsigned long);
unsigned long catch_start();
unsigned long getStartMillis();
unsigned long getNextInterval();

//interval
void interval_timer();
void interval_seq_handle_rec(Packet*);
void interval_seq_handle_rec_cb();
Packet * interval_seq_handle_sen();
void interval_seq_handle_sen_cb();
void interval_rec_millis(Packet*);
void interval_rec_millis_cb();
void interval_rec_bib(Packet*);
void interval_rec_bib_cb();
void interval_rec_race_type(Packet *);
void interval_rec_race_type_cb();
Packet * send_interval();
void send_interval_cb();
Packet * send_time_init();
void send_time();
void send_time_cb();
Packet * send_prev_interval();
void send_prev_interval_cb();
void new_racer(Packet *);
void new_racer_cb();
void interval_racer_init();
void rec_dnf(Packet *);
void rec_dnf_cb();
void catch_interval();
Packet * send_pace_time();
void send_pace_time_cb();
void send_pace_time_init();

/******Timing Eye********************/
static TimingEye * eye = 0;
static int confDistData = 0;
volatile static int lastDistance = 0;
volatile static bool sendingDistance = false;
volatile static bool recData = false;
static int finishDistance = 0;
static bool foundDistance = false;

bool hasFoundDistance();
void initEye();
void find_timing_distance();
bool isSendingDistance();
bool isReceivingData();
TimingEye * getEye();
Packet * send_distance();
void rec_confirm_distance(Packet*);
void rec_confirm_distance_cb();
void send_distance_cb();
int getFinishDistance();
void setHasFoundDistance(bool);

/******Send Choice to RPi functions********/

static int choice = -1;
static bool csent = false;
void make_choice();
void make_choice_callback();
Packet * make_choice_init();
int getChoice();
bool wasChoiceSent();
void setChoice(int);
void setChoiceSent(bool);

/******Send Wake Up Signal to RPi functions********/

static int wake_up = 1;
static int ack = -1;
static bool piAwake = false;
void wake_up_pi();
void sen_wake_up_callback();
void rec_confirm_callback();
void rec_confirm(Packet *);
Packet * sen_wake_up();
bool isPiAwake();
void setPiAwake(bool);


/******Add New Device functions********/

static int addData = -1;
static int addDev = -1;
static int addPos = -1;
static int addCount = 0;
static int senCount = 0;
static bool recv = false;
void get_add_device_parameters();
void rec_device_parameters_cb();
void rec_device_parameters(Packet *);
void rec_data_cb();
void rec_data(Packet *);
bool hasRecv();
void setRecv(bool);
int getAddDevice();
int getAddPosition();
void sen_dev_params_cb();
void send_dev_params();
void sen_dev_params_err(int);
Packet * sen_dev_params_init();
Packet * sen_dev_error();
void sen_dev_error_cb();

/******Send Device Info functions********/

static int senDevInfoC = 0;
static bool devInfoSent = false;
void send_dev_info();
Packet * send_dev_info_init();
void send_dev_info_cb();
bool hasDevInfoSent();

/******Sychronization functions********/

static unsigned long startMils = 0;
static unsigned long endMils = 0;
static unsigned long first = 0;
static unsigned long last = 0;
static unsigned long difs[DELAY_ARR];
static long del = 0;
static long dif = 0;
static unsigned long now = 0;
static unsigned long them = 0;
static unsigned long adjusted = 0;
static float factor = 0.00;
static int drift = 0;
static int c = 0 ;
static int i = 0;
static int o = 1;
static int g = 0;
static int q = 0;
static int syncCount = 0;
static int syncFunction = 0;
static bool resetStat = false;

bool getResetStatus();
void setResetStatus(bool);
void sync();
void sync_rec();
Packet * sync_init();
void sync_init_cb();
void sync_rec_init(Packet *);
void sync_rec_init_cb();
void sync_relay_rec(Packet*);
void sync_relay_rec_cb();
Packet * sync_relay_sen();
void sync_relay_sen_cb();
void sync_master_rec(Packet*);
void sync_master_rec_millis(Packet*);
void sync_master_rec_cb();
Packet * sync_master_sen();
Packet * sync_master_sen_millis();
void sync_master_sen_cb();
unsigned long getTime();

static int currentError = 0;
static func resetFunc  ='\0';
static func tempFunc = 0;
void setResetFunction(func);
void errorBlink();
int getCurrentError();
void setCurrentError(int);
#endif;
