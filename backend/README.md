
### Prerequirements:
*OpenSSL

### How to run server:

```
cd backend
g++ main.cpp -L/usr/lib json.hpp ThreadData.cpp ThreadData.h GlobalData.cpp GlobalData.h Base64.cpp Base64.h WebsocketContent.cpp WebsocketContent.h -std=c++17 -pthread -Wall -lcrypto -lssl -o main.out
./main.out 127.0.0.1 8000
```

### How to run frontend (in new tab):
```
cd frontend
npm i
npm start
```