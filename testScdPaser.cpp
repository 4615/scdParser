/************************************************************************/
/* auth: luolinglu
   fun:  test sclpase functon
*/
/************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <QDateTime>
#include "scl.h"
#include "slog.h"

//static char *thisFileName;
#define debugMsg qDebug() << QString("[%1 %2: %3 %4]").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")).arg(__FILE__).arg(__FUNCTION__).arg(__LINE__)
#define warnMsg qWarning() << QString("[%1 %2: %3 %4]").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")).arg(__FILE__).arg(__FUNCTION__).arg(__LINE__)

#define errMsg qCritical() << QString("[%1 %2: %3 %4]").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")).arg(__FILE__).arg(__FUNCTION__).arg(__LINE__)
#define panicMsg  qFatal() << QString("[%1 %2: %3 %4]").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")).arg(__FILE__).arg(__FUNCTION__).arg(__LINE__)
#define LOG_FILE_NAME "scdpase.log"
#ifdef DEBUG_SISCO
SD_CONST static ST_CHAR *SD_CONST thisFileName = __FILE__;
#endif

int main(int argc, char* argv[])
{
	//thisFileName = __FILE__;

	ST_CHAR *xmlFileName = "Template.icd";//"jilindong.scd"; //"TXJD01.cid";
	ST_CHAR *iedName = "C5011";
	ST_CHAR *accessPointName = "S1";
	ST_RET rc;

	
	SCL_INFO sclInfo;
	memset(&sclInfo, 0, sizeof(SCL_INFO));

	SCL_LD* ld;
	SCL_LN* ln;
	SCL_DOI* doi;
	// SCL_DAI* dai;
	SCL_SDI* sdi;

	SCL_SUBNET *net;
	SCL_CAP *cap;
	SCL_GSE *gse;

	SCL_LNTYPE *lnt;
	SCL_DO* don;

	SCL_DOTYPE *dot;
	SCL_DA *dan;

	int i=0;

	//test for log 
	
	slog_start(SX_LOG_ALWAY, LOG_FILE_EN, LOG_FILE_NAME);
	rc = scl_parse(xmlFileName, iedName, accessPointName, &sclInfo);

#if 0
	for(ld = sclInfo.ldHead; ld!=NULL; ld = (SCL_LD*)list_get_next (sclInfo.ldHead, ld))
	{
		printf("ld:%s\n",ld->inst); //ld:MONT
		for(ln = ld->lnHead; ln!=NULL; ln = (SCL_LN*)list_get_next (ld->lnHead, ln))
		{
			printf("  Ln:%s, %s, %s\n",ln->lnClass, ln->lnType, ln->varName); // Ln:GGIO, GDNR_EVE_GGIO_NSR_3620_DS, AlmGGIO1
			for(doi = ln->doiHead; doi!=NULL; doi = (SCL_DOI*)list_get_next (ln->doiHead, doi))
			{
				printf("    DO:%s, %s\n",doi->name, doi->desc); // DO:COppm, COһ����̼Ũ��
				for(dai = doi->daiHead; dai!=NULL; dai = (SCL_DAI*)list_get_next (doi->daiHead, dai))
				{
					printf("      DAI:%s, %s, %s, %s, %d\n",dai->flattened, dai->Val, dai->desc, dai->sAddr, dai->sGroup); //DAI:mag$dU, COһ����̼��
				}
				for(sdi = doi->sdiHead; sdi!=NULL; sdi = (SCL_SDI*)list_get_next(doi->sdiHead, sdi))
				{
					printf("      SDI:%s, %s\n",sdi->flattened, sdi->desc); //SDI:mag, (null)
				}
			}

			for(SCL_DATASET* ds = ln->datasetHead; ds != NULL; ds = (SCL_DATASET*)list_get_next(ln->datasetHead, ds))
			{
				printf("  DataSet:%s, %s\n", ds->name, ds->desc); //DataSet:dsLog, 日志记录数据�?
				for(SCL_FCDA* fcda = ds->fcdaHead; fcda != NULL; fcda = (SCL_FCDA*)list_get_next(ds->fcdaHead, fcda)) 
				{
					printf("      FCDA:%s, %s, %s, %s\n",fcda->doName, fcda->daName, fcda->domName, fcda->fc); //FCDA:Alm32, , TEMPLATELD0, ST
				}
			}

			for(SCL_RCB *rcb = ln->rcbHead; rcb != NULL; rcb = (SCL_RCB *)list_get_next(ln->rcbHead, rcb))
			{
				printf("  rcb:%s, %s, %s, %d, %s\n", rcb->name, rcb->datSet, rcb->rptID, rcb->maxClient, rcb->desc); //rcb:brcbRelayEna, dsRelayEna, TEMPLATEPROT/LLN0$brcbRelayEna, 16, (null)
			}
		}
	}
#endif
	//����communication,
	
	for(net = sclInfo.subnetHead; net!=NULL; net = (SCL_SUBNET*)list_get_next (sclInfo.subnetHead, net))
	{
		SLOG_DEBUG ("net=%s type=%s",net->name, net->type);
	
		for(cap = net->capHead; cap!=NULL; cap = (SCL_CAP *)list_get_next (net->capHead, cap))
		{
			// if (cap->addr->IP == '\0' || cap->addr->IPSUBNET == '\0') continue;
			SCL_PORT *port;
			for (port = cap->portHead; port != NULL; port = (SCL_PORT *)list_get_next (cap->portHead, port)) {
				SLOG_DEBUG("Port: %s", port->portCfg);
			}
			for (gse = cap->gseHead; gse != NULL; gse = (SCL_GSE *)list_get_next (cap->gseHead, gse)) {
				SLOG_DEBUG("   cap:Name %s MAC %s APPID %d min %d max %d", gse->cbName, gse->MAC, gse->APPID, gse->minTime, gse->maxTime);
			}
			
		}
	}
	SCL_ACCESSPOINT *acPoint;
	
	for (acPoint = sclInfo.accessPointHead; acPoint != NULL; acPoint = (SCL_ACCESSPOINT *)list_get_next(sclInfo.accessPointHead, acPoint))
	{
		SLOG_DEBUG("==============================AccessPoint Start=============================");
		SLOG_DEBUG("<AccessPoint Name= %s desc=%s>", acPoint->name, acPoint->desc);
		SLOG_DEBUG("==============================LDevice Start=================================");
		SCL_LD *scl_ld;
		SCL_LN *scl_ln;
		for (scl_ld = acPoint->ldHead; scl_ld != NULL; scl_ld = (SCL_LD *)list_get_next(acPoint->ldHead, scl_ld)){
			SLOG_DEBUG("<LDevice inst= %s desc=%s apName=%s>", scl_ld->inst, scl_ld->desc, scl_ld->apName);
			
			for (scl_ln = scl_ld->lnHead; scl_ln != NULL; scl_ln = (SCL_LN *)list_get_next(scl_ld->lnHead, scl_ln)) {
				// ST_UCHAR dsCount = scl_ln->datasetCount;
				SLOG_DEBUG("  <LN:%s %s %s %s %s %s>", scl_ln->varName, scl_ln->desc, scl_ln->lnType, scl_ln->lnClass, scl_ln->prefix, scl_ln->inst);
				for (SCL_RCB *reportCtl = scl_ln->rcbHead; reportCtl != NULL; reportCtl = (SCL_RCB *)list_get_next(scl_ln->rcbHead, reportCtl)) {
					SLOG_DEBUG("    <ReportControl: name=%s dataSet=%s intgPd=%d rptID=%s confRev=%d buffered=%d bufTime=%d", 
										reportCtl->name, reportCtl->datSet, reportCtl->intgPd,
										reportCtl->rptID, reportCtl->confRev, reportCtl->buffered,
										reportCtl->bufTime);
				}
				for(SCL_DATASET* ds = scl_ln->datasetHead; ds != NULL; ds = (SCL_DATASET*)list_get_next(scl_ln->datasetHead, ds))
				{
					SLOG_DEBUG("    <DataSet: name %s Desc %s>", ds->name, ds->desc); //
					for(SCL_FCDA* fcda = ds->fcdaHead; fcda != NULL; fcda = (SCL_FCDA*)list_get_next(ds->fcdaHead, fcda)) 
					{
						SLOG_DEBUG("      <FCDA ldInst=%s prefix=%s lnClass=%s lnInst=%s doName=%s daName=%s fc=%s>", 
											fcda->ldInst,
											fcda->prefix, 
											fcda->lnClass,
											fcda->lnInst,
											fcda->doName,
											fcda->daName, 
											fcda->fc); //FCDA:Alm32, , TEMPLATELD0, ST
					}
				}		
				for (SCL_DOI *doi = scl_ln->doiHead; doi != NULL; doi = (SCL_DOI *)list_get_next(scl_ln->doiHead, doi)) {
					SLOG_DEBUG("    <DOI name=%s dest=%s>", doi->name, doi->desc);
					for (SCL_SDI *sdi = doi->sdiHead; sdi != NULL; sdi = (SCL_SDI *)list_get_next(doi->sdiHead, sdi)) {
						SLOG_DEBUG("      <SDI name=%s>", sdi->flattened);
						for (SCL_DAI *dai = sdi->sdaiHead; dai != NULL; dai = (SCL_DAI *)list_get_next(sdi->sdaiHead, dai))
						{
							SLOG_DEBUG("        <DAI name=%s sAddr=%s>", dai->flattened, dai->sAddr);
						}
						
					}	
					for (SCL_DAI *dai = doi->daiHead; dai != NULL; dai = (SCL_DAI *)list_get_next(doi->daiHead, dai)) {
						SLOG_DEBUG("      <DAI name=%s sAddr=%s val=%s>", dai->flattened, dai->sAddr, dai->Val);
					}									
				}		
			}
		}
		SLOG_DEBUG("==============================LDevice End===================================");
		SLOG_DEBUG("==============================AccessPoint End===============================");
		
	}
	
	
 #if 0
	for(dot = sclInfo.doTypeHead; dot!=NULL; dot = (SCL_DOTYPE*)list_get_next (sclInfo.doTypeHead, dot))
	{
		printf("do type:%s, %s\n",dot->id, dot->cdc);  //do type:CN_INC_Mod, INC
		for(dan = dot->daHead; dan!=NULL; dan = (SCL_DA*)list_get_next (dot->daHead, dan))
		{
			printf("  da:%s, %s, %s\n",dan->name, dan->bType, dan->fc); //da:ctlModel, Enum, CF
		}
		for(don = dot->sdoHead; don!=NULL; don = (SCL_DO*)list_get_next(dot->sdoHead, don))
		{
			printf("  do:%s, %s\n",don->name, don->type); //do:CmbuGasAlm, CN_SPS
		}
	}
#endif
	

	scl_info_destroy(&sclInfo);
	
	printf("main: parse %s, Result=%d\n", xmlFileName, rc);
	

	//slog_start(SX_LOG_ALWAY, LOG_MEM_EN, NULL);
	slog_end();
	//slog("main: parse %s, rc=%d\n", xmlFileName, rc);

#ifdef WIN32
	system("pause");
#endif

	return 0;
}