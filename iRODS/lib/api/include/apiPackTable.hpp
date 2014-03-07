/*** Copyright (c), The Regents of the University of California            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/
/* apiPackTable.h - This file may be generated by a program or script
 */

#ifndef API_PACK_TABLE_HPP
#define API_PACK_TABLE_HPP

#include "rods.hpp"
#include "packStruct.hpp"
#include "rodsPackInstruct.hpp"

/* need to include a header for for each API */
#include "apiHeaderAll.hpp"

packInstruct_t MiscSvrInfo_PIG = MiscSvrInfo_PI;

packInstructArray_t api_pack_table_init[] = {
    {"DataObjInp_PI", DataObjInp_PI, irods::clearInStruct_noop},
    {"OpenedDataObjInp_PI", OpenedDataObjInp_PI, irods::clearInStruct_noop},
    {"DataCopyInp_PI", DataCopyInp_PI, irods::clearInStruct_noop},
    {"DataObjCopyInp_PI", DataObjCopyInp_PI, irods::clearInStruct_noop},
    {"fileOpenInp_PI", fileOpenInp_PI, irods::clearInStruct_noop},
    {"fileChksumInp_PI", fileChksumInp_PI, irods::clearInStruct_noop},
    {"fileChksumOut_PI", fileChksumOut_PI, irods::clearInStruct_noop},
    {"fileLseekInp_PI", fileLseekInp_PI, irods::clearInStruct_noop},
    {"fileLseekOut_PI", fileLseekOut_PI, irods::clearInStruct_noop},
    {"fileStatInp_PI", fileStatInp_PI, irods::clearInStruct_noop},
    {"fileUnlinkInp_PI", fileUnlinkInp_PI, irods::clearInStruct_noop},
    {"fileReadInp_PI", fileReadInp_PI, irods::clearInStruct_noop},
    {"fileChmodInp_PI", fileChmodInp_PI, irods::clearInStruct_noop},
    {"fileCloseInp_PI", fileCloseInp_PI, irods::clearInStruct_noop},
    {"fileGetFsFreeSpaceInp_PI", fileGetFsFreeSpaceInp_PI, irods::clearInStruct_noop},
    {"fileGetFsFreeSpaceOut_PI", fileGetFsFreeSpaceOut_PI, irods::clearInStruct_noop},
    {"fileMkdirInp_PI", fileMkdirInp_PI, irods::clearInStruct_noop},
    {"fileOpendirInp_PI", fileOpendirInp_PI, irods::clearInStruct_noop},
    {"fileReaddirInp_PI", fileReaddirInp_PI, irods::clearInStruct_noop},
    {"fileRenameInp_PI", fileRenameInp_PI, irods::clearInStruct_noop},
    {"fileRmdirInp_PI", fileRmdirInp_PI, irods::clearInStruct_noop},
    {"fileWriteInp_PI", fileWriteInp_PI, irods::clearInStruct_noop},
    {"fileClosedirInp_PI", fileClosedirInp_PI, irods::clearInStruct_noop},
    {"MiscSvrInfo_PI", MiscSvrInfo_PI, irods::clearInStruct_noop},
    {"ModDataObjMeta_PI", ModDataObjMeta_PI, irods::clearInStruct_noop},
    {"authRequestOut_PI", authRequestOut_PI, irods::clearInStruct_noop},
    {"authResponseInp_PI", authResponseInp_PI, irods::clearInStruct_noop},
    {"CollInpNew_PI", CollInpNew_PI, irods::clearInStruct_noop},
    {"ExecMyRuleInp_PI", ExecMyRuleInp_PI, irods::clearInStruct_noop},
    {"simpleQueryInp_PI", simpleQueryInp_PI, irods::clearInStruct_noop},
    {"generalAdminInp_PI", generalAdminInp_PI, irods::clearInStruct_noop},
    {"authCheckInp_PI", authCheckInp_PI, irods::clearInStruct_noop},
    {"authCheckOut_PI", authCheckOut_PI, irods::clearInStruct_noop},
    {"modAccessControlInp_PI", modAccessControlInp_PI, irods::clearInStruct_noop},
    {"ModAVUMetadataInp_PI", ModAVUMetadataInp_PI, irods::clearInStruct_noop},
    {"RULE_EXEC_MOD_INP_PI", RULE_EXEC_MOD_INP_PI, irods::clearInStruct_noop},
    {"RULE_EXEC_DEL_INP_PI", RULE_EXEC_DEL_INP_PI, irods::clearInStruct_noop},
    {"RULE_EXEC_SUBMIT_INP_PI", RULE_EXEC_SUBMIT_INP_PI, irods::clearInStruct_noop},
    {"simpleQueryOut_PI", simpleQueryOut_PI, irods::clearInStruct_noop},
    {"RegReplica_PI", RegReplica_PI, irods::clearInStruct_noop},
    {"UnregDataObj_PI", UnregDataObj_PI, irods::clearInStruct_noop},
    {"ExecCmd_PI", ExecCmd_PI, irods::clearInStruct_noop},
    {"ExecCmd241_PI", ExecCmd241_PI, irods::clearInStruct_noop},
    {"ExecCmdOut_PI", ExecCmdOut_PI, irods::clearInStruct_noop},
    {"SubStructFileFdOpr_PI", SubStructFileFdOpr_PI, irods::clearInStruct_noop},
    {"SubStructFileLseekInp_PI", SubStructFileLseekInp_PI, irods::clearInStruct_noop},
    {"SubStructFileRenameInp_PI", SubStructFileRenameInp_PI, irods::clearInStruct_noop},
    {"getTempPasswordOut_PI", getTempPasswordOut_PI, irods::clearInStruct_noop},
    {"GetXmsgTicketInp_PI", GetXmsgTicketInp_PI, irods::clearInStruct_noop},
    {"SendXmsgInp_PI", SendXmsgInp_PI, irods::clearInStruct_noop},
    {"StructFileOprInp_PI", StructFileOprInp_PI, irods::clearInStruct_noop},
    {"gsiAuthRequestOut_PI", gsiAuthRequestOut_PI, irods::clearInStruct_noop},
    {"StructFileExtAndRegInp_PI", StructFileExtAndRegInp_PI, irods::clearInStruct_noop},
    {"ChkObjPermAndStat_PI", ChkObjPermAndStat_PI, irods::clearInStruct_noop},
    {"userAdminInp_PI", userAdminInp_PI, irods::clearInStruct_noop},
    {"OpenStat_PI", OpenStat_PI, irods::clearInStruct_noop},
    {"fileStageSyncInp_PI", fileStageSyncInp_PI, irods::clearInStruct_noop},
    {"generalRowInsertInp_PI", generalRowInsertInp_PI, irods::clearInStruct_noop},
    {"generalRowPurgeInp_PI", generalRowPurgeInp_PI, irods::clearInStruct_noop},
    {"krbAuthRequestOut_PI", krbAuthRequestOut_PI, irods::clearInStruct_noop},
    {"getRescQuotaInp_PI", getRescQuotaInp_PI, irods::clearInStruct_noop},
    {"rescQuota_PI", rescQuota_PI, irods::clearInStruct_noop},
    {"BulkOprInp_PI", BulkOprInp_PI, irods::clearInStruct_noop},
    {"endTransactionInp_PI", endTransactionInp_PI, irods::clearInStruct_noop},
    {"ProcStatInp_PI", ProcStatInp_PI, irods::clearInStruct_noop},
    {"specificQueryInp_PI", specificQueryInp_PI, irods::clearInStruct_noop},
    {"ticketAdminInp_PI", ticketAdminInp_PI, irods::clearInStruct_noop},
    {"getTempPasswordForOtherInp_PI", getTempPasswordForOtherInp_PI, irods::clearInStruct_noop},
    {"getTempPasswordForOtherOut_PI", getTempPasswordForOtherOut_PI, irods::clearInStruct_noop},
    /* NETCDF PI */
#if 0 // NETCDF_CLIENT
    {"NcOpenInp_PI", NcOpenInp_PI, irods::clearInStruct_noop},
    {"NcCloseInp_PI", NcCloseInp_PI, irods::clearInStruct_noop},
    {"NcInqIdInp_PI", NcInqIdInp_PI, irods::clearInStruct_noop},
    {"NcInqWithIdOut_PI", NcInqWithIdOut_PI, irods::clearInStruct_noop},
    {"NcGetVarInp_PI", NcGetVarInp_PI, irods::clearInStruct_noop},
    {"NcGetVarOut_PI", NcGetVarOut_PI, irods::clearInStruct_noop},
    {"NcGenDimOut_PI", NcGenDimOut_PI, irods::clearInStruct_noop},
    {"NcGenAttOut_PI", NcGenAttOut_PI, irods::clearInStruct_noop},
    {"NcGenVarOut_PI", NcGenVarOut_PI, irods::clearInStruct_noop},
    {"NcInqInp_PI", NcInqInp_PI, irods::clearInStruct_noop},
    {"NcInqOut_PI", NcInqOut_PI, irods::clearInStruct_noop},
    {"NcInqGrpsInp_PI", NcInqGrpsInp_PI, irods::clearInStruct_noop},
    {"NcInqGrpsOut_PI", NcInqGrpsOut_PI, irods::clearInStruct_noop},
    {"NcRegGlobalAttrInp_PI", NcRegGlobalAttrInp_PI, irods::clearInStruct_noop},
    {"NcAggElement_PI", NcAggElement_PI, irods::clearInStruct_noop},
    {"NcAggInfo_PI", NcAggInfo_PI, irods::clearInStruct_noop},
    {"NcArchTimeSeriesInp_PI", NcArchTimeSeriesInp_PI, irods::clearInStruct_noop},
#endif
    /* endof NETCDF_CLIENT */
#ifdef OOI_CI
    {"OoiGenServReqInp_PI", OoiGenServReqInp_PI, irods::clearInStruct_noop},
    {"OoiGenServReqOut_PI", OoiGenServReqOut_PI, irods::clearInStruct_noop},
#endif

    {"pamAuthRequestInp_PI", pamAuthRequestInp_PI, irods::clearInStruct_noop},
    {"pamAuthRequestOut_PI", pamAuthRequestOut_PI, irods::clearInStruct_noop},
    {"authPlugReqInp_PI", authPlugReqInp_PI, irods::clearInStruct_noop},
    {"authPlugReqOut_PI", authPlugReqOut_PI, irods::clearInStruct_noop},
    {"getHierarchyForRescInp_PI", getHierarchyForRescInp_PI, irods::clearInStruct_noop},
    {"getHierarchyForRescOut_PI", getHierarchyForRescOut_PI, irods::clearInStruct_noop},
    {"sslStartInp_PI", sslStartInp_PI, irods::clearInStruct_noop},
    {"sslEndInp_PI", sslEndInp_PI, irods::clearInStruct_noop},
    {"getLimitedPasswordInp_PI", getLimitedPasswordInp_PI, irods::clearInStruct_noop},
    {"getLimitedPasswordOut_PI", getLimitedPasswordOut_PI, irods::clearInStruct_noop},
    {"fileSyncOut_PI", fileSyncOut_PI, irods::clearInStruct_noop},
    {"fileRenameOut_PI", fileRenameOut_PI, irods::clearInStruct_noop},
    {PACK_TABLE_END_PI, ( char * ) NULL, irods::clearInStruct_noop},
};

#endif	/* API_PACK_TABLE_H */
