#ifndef UDPSOCKET_PROTOCOL_H
#define UDPSOCKET_PROTOCOL_H
typedef enum {
    PKT_LOGIN = 1,
    PKT_INPUT = 2,
    PKT_PING = 3,
    PKT_STATE = 4,
    PKT_LOGIN_ACK = 5,
    PKT_LOGOUT = 6,
} PacketType;

typedef struct {
    uint8_t type;
} PacketHeader;

typedef struct {
    uint8_t type;
    char email[16];
    char password[64];
} PacketLogin;

typedef struct {
    uint8_t type;
    int id;
} PacketLoginACK;

typedef struct {
    uint8_t type;
    int id;
}PacketLogout;

typedef struct {
    uint8_t type;
    int dx, dy;
} PacketInput;

typedef struct {
    uint8_t type;
    int player_id;
    float x;
    float y;
} PacketState;
#endif //UDPSOCKET_PROTOCOL_H