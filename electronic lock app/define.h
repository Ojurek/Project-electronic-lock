#ifndef DEFINE_H
#define DEFINE_H



//Firebse data
//#define API_KEY
//#define LOGIN_MAIL
//#define LOGIN_PASS
#define STR_URL_EVENT   "https://projectlockraspi-default-rtdb.europe-west1.firebasedatabase.app/event.json?auth="
#define STR_URL_KEYS    "https://projectlockraspi-default-rtdb.europe-west1.firebasedatabase.app/keys.json?auth="
#define STR_URL_SIGN_IN "https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key="
#define STR_URL_REFRESH "https://securetoken.googleapis.com/v1/token?key="

//Firebase events data
#define EVENT_DOOR_STATUS       "Door"      //key in Firebase event
#define DOOR_OPENED             "Opened"    //this value set raspberry when door are opened
#define DOOR_CLOSED             "Closed"    //this value set raspberry when door are closed

#define EVENT_OPEN_REQUEST      "Request"   //key in Firebase event
#define OPEN_REQUEST            "Open"      //this value set app for trigger open of lock
#define OPEN_CONFIRMATION       "Done"      //this value set raspberry for confirm that on the door lock has been opened

#define EVENT_CONNECTION_STATUS "Status"    //key in Firebase event
#define CONNECTION_CHECK        "Check"     //this value set app for checking if raspberry is online
#define CONNECTION_OK           "Ack"        //this value set raspberry for confirm that is online

#define EVENT_RESPONSE_PREFIX   "event: patch\ndata: "  //means event triggerd by "patch" command

#define AUTH_REFRESH_TIME_SEC   3000        //period of the exchange of the refresh token

//Raspberry
//relay plate chanel1=BCM26, chanel2=BCM20, chanel3= BCM21
#define PIN_OUT                 26 // electromagnetic lock pin
#define PIN_INP                 12 // proximity sensor input pin

//keybord data
#define MAX_TEXT_LENGTH         7 // max number of digits on display
#define INPUT_READ_PERIOD_MILI_SEC  1000 //input from door position will be read with this period
#define OPEN_LOCK_TIME_SEC      10 //time that lock will be opened
#define NEGATIVE_MSG_PRESENTAION_TIME_SEC   5 //time that negative message will be presented

#define CLOSE_APP_PASS          "0000000"


#endif // DEFINE_H
