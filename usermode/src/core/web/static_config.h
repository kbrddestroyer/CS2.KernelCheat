#pragma once

#define STRINGIZE(x) #x
#define STRINGIZE_W(x) STRINGIZE(x)

#ifndef CLIENT_ID
#pragma message("CLIENT_ID is undefined")
#else
#pragma message("CLIENT_ID" STRINGIZE_W(CLIENT_ID))
#endif
