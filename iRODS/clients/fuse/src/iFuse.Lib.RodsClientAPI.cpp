/*** Copyright (c), The Regents of the University of California            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/
/*** This code is rewritten by Illyoung Choi (iychoi@email.arizona.edu)    ***
 *** funded by iPlantCollaborative (www.iplantcollaborative.org).          ***/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <assert.h>
#include <pthread.h>
#include <list>
#include <arpa/inet.h>
#include "iFuse.Lib.hpp"
#include "iFuse.Lib.RodsClientAPI.hpp"
#include "iFuse.Lib.Util.hpp"
#include "sockComm.h"
#include "miscUtil.h"
#include "ticketAdmin.h"

typedef struct IFuseRodsClientOperation {
    time_t start;
    rcComm_t *conn;
} iFuseRodsClientOperation_t;

static pthread_rwlock_t g_RodsClientAPILock;
static pthread_rwlockattr_t g_RodsClientAPILockAttr;
static std::list<iFuseRodsClientOperation_t*> g_Operations;

static int g_RodsapiTimeoutSec = IFUSE_RODSCLIENTAPI_TIMEOUT_SEC;
static time_t g_LastRodsapiTimeoutCheck = 0;

static void _timeoutChecker() {
    std::list<iFuseRodsClientOperation_t*>::iterator it_oper;
    std::list<iFuseRodsClientOperation_t*> removeList;
    iFuseRodsClientOperation_t *oper;
    time_t currentTime;
    
    bool hasTimeout = false;
    
    //iFuseLibLog(LOG_DEBUG, "_timeoutChecker is called");
    
    currentTime = iFuseLibGetCurrentTime();
    
    if(iFuseLibDiffTimeSec(currentTime, g_LastRodsapiTimeoutCheck) > g_RodsapiTimeoutSec / 2) {
        //iFuseLibLog(LOG_DEBUG, "_timeoutChecker: checking timedout rodsAPI calls");
        pthread_rwlock_rdlock(&g_RodsClientAPILock);
        
        // iterate operation list to check timedout
        for(it_oper=g_Operations.begin();it_oper!=g_Operations.end();it_oper++) {
            oper = *it_oper;
            
            if(iFuseLibDiffTimeSec(currentTime, oper->start) >= g_RodsapiTimeoutSec) {
                hasTimeout = true;
                break;
            }
        }
        
        pthread_rwlock_unlock(&g_RodsClientAPILock);
        
        if(hasTimeout) {
            pthread_rwlock_wrlock(&g_RodsClientAPILock);
            
            // iterate operation list to check timedout
            for(it_oper=g_Operations.begin();it_oper!=g_Operations.end();it_oper++) {
                oper = *it_oper;
                
                if(iFuseLibDiffTimeSec(currentTime, oper->start) >= g_RodsapiTimeoutSec) {
                    iFuseLibLog(LOG_DEBUG, "_timeoutChecker: detected timed-out operation\n");
                    removeList.push_back(oper);
                }
            }
            
            // iterate remove list
            while(!removeList.empty()) {
                oper = removeList.front();
                removeList.pop_front();
                g_Operations.remove(oper);

                if(oper->conn != NULL) {
                    socklen_t len;
                    struct sockaddr_storage addr;
                    char ipstr[INET6_ADDRSTRLEN];
                    int port;
                    len = sizeof addr;

                    getsockname(oper->conn->sock, (struct sockaddr*)&addr, &len);

                    // deal with both IPv4 and IPv6:
                    if (addr.ss_family == AF_INET) {
                        struct sockaddr_in *s = (struct sockaddr_in *)&addr;
                        port = ntohs(s->sin_port);
                        inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
                    } else { // AF_INET6
                        struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
                        port = ntohs(s->sin6_port);
                        inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr);
                    }

                    iFuseLibLog(LOG_DEBUG, "_timeoutChecker: kill connection (Local IP address): %s:%d\n", ipstr, port);

                    // kill the connection
                    shutdown(oper->conn->sock, 2);
                }
            }
            
            pthread_rwlock_unlock(&g_RodsClientAPILock);
        }
        
        g_LastRodsapiTimeoutCheck = iFuseLibGetCurrentTime();
    }
}

static iFuseRodsClientOperation_t *_startOperationTimeout(rcComm_t *conn) {
    iFuseRodsClientOperation_t *oper = (iFuseRodsClientOperation_t *)calloc(1, sizeof(iFuseRodsClientOperation_t));
    if(oper == NULL) {
        return NULL;
    }
    
    oper->start = iFuseLibGetCurrentTime();
    oper->conn = conn;
    
    pthread_rwlock_wrlock(&g_RodsClientAPILock);
    g_Operations.push_back(oper);
    pthread_rwlock_unlock(&g_RodsClientAPILock);
    
    return oper;
}

static void _endOperationTimeout(iFuseRodsClientOperation_t *oper) {
    pthread_rwlock_wrlock(&g_RodsClientAPILock);
    g_Operations.remove(oper);
    pthread_rwlock_unlock(&g_RodsClientAPILock);
    
    free(oper);
}

/*
 * Initialize iFuse Rods Client
 */
void iFuseRodsClientInit() {
   
    if(iFuseLibGetOption()->rodsapiTimeoutSec > 0) {
        g_RodsapiTimeoutSec = iFuseLibGetOption()->rodsapiTimeoutSec;
    }
   
    pthread_rwlockattr_init(&g_RodsClientAPILockAttr);
    pthread_rwlock_init(&g_RodsClientAPILock, &g_RodsClientAPILockAttr);
    
    iFuseLibSetTimerTickHandler(_timeoutChecker);
}

/*
 * Destroy iFuse Rods Client
 */
void iFuseRodsClientDestroy() {
    iFuseLibUnsetTimerTickHandler(_timeoutChecker);
    
    pthread_rwlock_destroy(&g_RodsClientAPILock);
    pthread_rwlockattr_destroy(&g_RodsClientAPILockAttr);
}

int iFuseRodsClientReadMsgError(int status) {
    int irodsErr = getIrodsErrno( status );

    // SYS_PACK_INSTRUCT_FORMAT_ERR can be returned when network is suddenly disconnected
    if (irodsErr == SYS_READ_MSG_BODY_LEN_ERR ||
            irodsErr == SYS_HEADER_READ_LEN_ERR ||
            irodsErr == SYS_HEADER_WRITE_LEN_ERR ||
            irodsErr == SYS_PACK_INSTRUCT_FORMAT_ERR) {
        return 1;
    } else {
        return 0;
    }
}

rcComm_t *iFuseRodsClientConnect(const char *rodsHost, int rodsPort, const char *userName, const char *rodsZone, int reconnFlag, rErrMsg_t *errMsg) {
    rcComm_t *conn = NULL;
    conn = rcConnect(rodsHost, rodsPort, userName, rodsZone, reconnFlag, errMsg);
    if(conn != NULL) {
        socklen_t len;
        struct sockaddr_storage addr;
        char ipstr[INET6_ADDRSTRLEN];
        int port;
        len = sizeof addr;

        getsockname(conn->sock, (struct sockaddr*)&addr, &len);

        // deal with both IPv4 and IPv6:
        if (addr.ss_family == AF_INET) {
            struct sockaddr_in *s = (struct sockaddr_in *)&addr;
            port = ntohs(s->sin_port);
            inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
        } else { // AF_INET6
            struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
            port = ntohs(s->sin6_port);
            inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr);
        }

        iFuseLibLog(LOG_DEBUG, "iFuseRodsClientConnect: Local IP address: %s:%d\n", ipstr, port);
    }

    return conn;
}

int iFuseRodsClientLogin(rcComm_t *conn) {
    iFuseRodsClientOperation_t *oper = _startOperationTimeout(conn);
    int status;
    
    if(oper == NULL) {
        return SYS_MALLOC_ERR;
    }
    
    status = clientLogin(conn);
    _endOperationTimeout(oper);
    return status;
}

int iFuseRodsClientDisconnect(rcComm_t *conn) {
    return rcDisconnect(conn);
}

int iFuseRodsClientMakeRodsPath(const char *path, char *iRodsPath) {
    return parseRodsPathStr((char *) (path + 1), iFuseLibGetRodsEnv(), iRodsPath);
}

int iFuseRodsClientSetSessionTicket(rcComm_t *conn, ticketAdminInp_t *ticketAdminInp) {
    iFuseRodsClientOperation_t *oper = _startOperationTimeout(conn);
    int status;
    
    if(oper == NULL) {
        return SYS_MALLOC_ERR;
    }

    status = rcTicketAdmin(conn, ticketAdminInp);
    _endOperationTimeout(oper);
    return status;
}

int iFuseRodsClientDataObjOpen(rcComm_t *conn, dataObjInp_t *dataObjInp) {
    iFuseRodsClientOperation_t *oper = _startOperationTimeout(conn);
    int status;
    
    if(oper == NULL) {
        return SYS_MALLOC_ERR;
    }
    
    status = rcDataObjOpen(conn, dataObjInp);
    _endOperationTimeout(oper);
    return status;
}

int iFuseRodsClientDataObjClose(rcComm_t *conn, openedDataObjInp_t *dataObjCloseInp) {
    iFuseRodsClientOperation_t *oper = _startOperationTimeout(conn);
    int status;
    
    if(oper == NULL) {
        return SYS_MALLOC_ERR;
    }
    
    status = rcDataObjClose(conn, dataObjCloseInp);
    _endOperationTimeout(oper);
    return status;
}

int iFuseRodsClientOpenCollection(rcComm_t *conn, char *collection, int flag, collHandle_t *collHandle) {
    iFuseRodsClientOperation_t *oper = _startOperationTimeout(conn);
    int status;
    
    if(oper == NULL) {
        return SYS_MALLOC_ERR;
    }
    
    status = rclOpenCollection(conn, collection, flag, collHandle);
    _endOperationTimeout(oper);
    return status;
}

int iFuseRodsClientCloseCollection(collHandle_t *collHandle) {
    return rclCloseCollection(collHandle);
}

int iFuseRodsClientObjStat(rcComm_t *conn, dataObjInp_t *dataObjInp, rodsObjStat_t **rodsObjStatOut) {
    iFuseRodsClientOperation_t *oper = _startOperationTimeout(conn);
    int status;
    
    if(oper == NULL) {
        return SYS_MALLOC_ERR;
    }
    
    status = rcObjStat(conn, dataObjInp, rodsObjStatOut);
    _endOperationTimeout(oper);
    return status;
}

int iFuseRodsClientDataObjLseek(rcComm_t *conn, openedDataObjInp_t *dataObjLseekInp, fileLseekOut_t **dataObjLseekOut) {
    iFuseRodsClientOperation_t *oper = _startOperationTimeout(conn);
    int status;
    
    if(oper == NULL) {
        return SYS_MALLOC_ERR;
    }
    
    status = rcDataObjLseek(conn, dataObjLseekInp, dataObjLseekOut);
    _endOperationTimeout(oper);
    return status;
}

int iFuseRodsClientDataObjRead(rcComm_t *conn, openedDataObjInp_t *dataObjReadInp, bytesBuf_t *dataObjReadOutBBuf) {
    iFuseRodsClientOperation_t *oper = _startOperationTimeout(conn);
    int status;
    
    if(oper == NULL) {
        return SYS_MALLOC_ERR;
    }
    
    status = rcDataObjRead(conn, dataObjReadInp, dataObjReadOutBBuf);
    _endOperationTimeout(oper);
    return status;
}

int iFuseRodsClientDataObjWrite(rcComm_t *conn, openedDataObjInp_t *dataObjWriteInp, bytesBuf_t *dataObjWriteInpBBuf) {
    iFuseRodsClientOperation_t *oper = _startOperationTimeout(conn);
    int status;
    
    if(oper == NULL) {
        return SYS_MALLOC_ERR;
    }
    
    status = rcDataObjWrite(conn, dataObjWriteInp, dataObjWriteInpBBuf);
    _endOperationTimeout(oper);
    return status;
}

int iFuseRodsClientDataObjCreate(rcComm_t *conn, dataObjInp_t *dataObjInp) {
    iFuseRodsClientOperation_t *oper = _startOperationTimeout(conn);
    int status;
    
    if(oper == NULL) {
        return SYS_MALLOC_ERR;
    }
    
    status = rcDataObjCreate(conn, dataObjInp);
    _endOperationTimeout(oper);
    return status;
}

int iFuseRodsClientDataObjUnlink(rcComm_t *conn, dataObjInp_t *dataObjUnlinkInp) {
    iFuseRodsClientOperation_t *oper = _startOperationTimeout(conn);
    int status;
    
    if(oper == NULL) {
        return SYS_MALLOC_ERR;
    }
    
    status = rcDataObjUnlink(conn, dataObjUnlinkInp);
    _endOperationTimeout(oper);
    return status;
}

int iFuseRodsClientReadCollection(rcComm_t *conn, collHandle_t *collHandle, collEnt_t *collEnt) {
    iFuseRodsClientOperation_t *oper = _startOperationTimeout(conn);
    int status;
    
    if(oper == NULL) {
        return SYS_MALLOC_ERR;
    }
    
    status = rclReadCollection(conn, collHandle, collEnt);
    _endOperationTimeout(oper);
    return status;
}

int iFuseRodsClientCollCreate(rcComm_t *conn, collInp_t *collCreateInp) {
    iFuseRodsClientOperation_t *oper = _startOperationTimeout(conn);
    int status;
    
    if(oper == NULL) {
        return SYS_MALLOC_ERR;
    }
    
    status = rcCollCreate(conn, collCreateInp);
    _endOperationTimeout(oper);
    return status;
}

int iFuseRodsClientRmColl(rcComm_t *conn, collInp_t *rmCollInp, int vFlag) {
    iFuseRodsClientOperation_t *oper = _startOperationTimeout(conn);
    int status;
    
    if(oper == NULL) {
        return SYS_MALLOC_ERR;
    }
    
    status = rcRmColl(conn, rmCollInp, vFlag);
    _endOperationTimeout(oper);
    return status;
}

int iFuseRodsClientDataObjRename(rcComm_t *conn, dataObjCopyInp_t *dataObjRenameInp) {
    iFuseRodsClientOperation_t *oper = _startOperationTimeout(conn);
    int status;
    
    if(oper == NULL) {
        return SYS_MALLOC_ERR;
    }
    
    status = rcDataObjRename(conn, dataObjRenameInp);
    _endOperationTimeout(oper);
    return status;
}

int iFuseRodsClientDataObjTruncate(rcComm_t *conn, dataObjInp_t *dataObjInp) {
    iFuseRodsClientOperation_t *oper = _startOperationTimeout(conn);
    int status;
    
    if(oper == NULL) {
        return SYS_MALLOC_ERR;
    }
    
    status = rcDataObjTruncate(conn, dataObjInp);
    _endOperationTimeout(oper);
    return status;
}

int iFuseRodsClientModDataObjMeta(rcComm_t *conn, modDataObjMeta_t *modDataObjMetaInp) {
    iFuseRodsClientOperation_t *oper = _startOperationTimeout(conn);
    int status;
    
    if(oper == NULL) {
        return SYS_MALLOC_ERR;
    }
    
    status = rcModDataObjMeta(conn, modDataObjMetaInp);
    _endOperationTimeout(oper);
    return status;
}
