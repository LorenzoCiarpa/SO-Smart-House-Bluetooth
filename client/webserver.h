#include <libwebsockets.h>

int webServerCreate();
int manageWebServerRequest(struct lws *wsi, char* buf);
