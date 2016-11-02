#pragma once
#pragma pack(1)
typedef struct tagIDCardData{
	char Name[32];		//姓名       
	char Sex[6];		//性别
	char Nation[20];		//名族
	char Born[18];		//出生日期
	char Address[72];	//住址
	char IDCardNo[38];	//身份证号
	char GrantDept[32]; //发证机关
	char UserLifeBegin[18];	// 有效开始日期
	char UserLifeEnd[18];	// 有效截止日期
	char reserved[38];		// 保留
	char PhotoFileName[255];// 照片路径
}IDCardData;
typedef struct tagIDCardData2{
	char Name[32]; //姓名       
	char Sex[4];   //性别
	char Nation[6]; //名族
	char Born[18]; //出生日期
	char Address[72]; //住址
	char IDCardNo[38]; //身份证号
	char GrantDept[32]; //发证机关
	char UserLifeBegin[18]; // 有效开始日期
	char UserLifeEnd[18];  // 有效截止日期
	char reserved[38]; // 保留
	char PhotoWLT[1024]; // WLT照片
}IDCardData2;
#pragma pack()
extern "C"{
	//////////////////////////////////////////////////////////////////////////
	//				SAM端口函数
	//
	//////////////////////////////////////////////////////////////////////////
	int _stdcall Syn_GetCOMBaud ( int iPort, unsigned int *  puiBaudRate );
	int _stdcall Syn_SetCOMBaud ( int iPort, unsigned int uiCurrBaud, unsigned int uiSetBaud );
	int _stdcall Syn_OpenPort( int iPort );
	int _stdcall Syn_ClosePort( int iPort );
	//////////////////////////////////////////////////////////////////////////
	//				SAM类函数
	//
	//////////////////////////////////////////////////////////////////////////
	int _stdcall Syn_SetMaxRFByte (	int	iPort, unsigned char ucByte,int	bIfOpen );
	int _stdcall Syn_ResetSAM ( int  iPort,	int	iIfOpen	);
	int _stdcall Syn_GetSAMStatus (	int iPort, int iIfOpen );
	int _stdcall Syn_GetSAMID (	int iPort, unsigned char *	pucSAMID, int iIfOpen );
	int _stdcall Syn_GetSAMIDToStr ( int iPort,	char *	pcSAMID, int iIfOpen );
	//////////////////////////////////////////////////////////////////////////
	//				身份证卡类函数
	//
	//////////////////////////////////////////////////////////////////////////
	int _stdcall Syn_StartFindIDCard ( int iPort , unsigned char *	pucIIN,	int	iIfOpen	);
	int _stdcall Syn_SelectIDCard ( int iPort , unsigned char * pucSN,	int iIfOpen	);
	int _stdcall Syn_ReadBaseMsg ( 
		int				iPort , 
		unsigned char * pucCHMsg , 
		unsigned int  * puiCHMsgLen , 
		unsigned char * pucPHMsg , 
		unsigned int  *	puiPHMsgLen , 
		int				iIfOpen);
	int _stdcall Syn_ReadBaseMsgToFile (
		int 			iPort,
		char * 			pcCHMsgFileName,
		unsigned int *	puiCHMsgFileLen,
		char * 			pcPHMsgFileName,
		unsigned int  *	puiPHMsgFileLen,
		int				iIfOpen
		);
	int	_stdcall Syn_ReadBaseFPMsg(
		int				iPort, 
		unsigned char *	pucCHMsg, 
		unsigned int  *	puiCHMsgLen, 
		unsigned char *	pucPHMsg, 
		unsigned int  *	puiPHMsgLen, 
		unsigned char *	pucFPMsg, 
		unsigned int  *	puiFPMsgLen, 
		int				iIfOpen
		);
	int	_stdcall Syn_ReadBaseFPMsgToFile(
		int				iPort, 
		char *			pcCHMsgFileName, 
		unsigned int *	puiCHMsgFileLen, 
		char *			pcPHMsgFileName, 
		unsigned int *	puiPHMsgFileLen, 
		char *			pcFPMsgFileName, 
		unsigned int *	puiFPMsgFileLen, 
		int				iIfOpen
		);
	int _stdcall Syn_ReadNewAppMsg(int iPort , unsigned char * pucAppMsg , unsigned int * puiAppMsgLen , int iIfOpen);
	int _stdcall Syn_GetBmp( int iPort , char * Wlt_File );
	
	int _stdcall Syn_ReadMsg( int iPort,int iIfOpen,IDCardData *pIDCardData );
	int _stdcall Syn_ReadFPMsg( int iPort,int iIfOpen,IDCardData *pIDCardData,char * cFPhotoname );

	int _stdcall Syn_FindReader();
	int _stdcall Syn_FindUSBReader();

	int _stdcall Syn_BmpToJpeg( char * cBmpName , char * cJpegName);
	int _stdcall Syn_PhotoToStrBase64(char * cBase64,int * iLen, char *cPhotoName);
	int _stdcall Syn_StrBase64ToPhoto(char * cBase64,int iLen,char *cPhotoName);
	//////////////////////////////////////////////////////////////////////////
	//				设置附加功能函数
	//
	//////////////////////////////////////////////////////////////////////////
	int _stdcall Syn_SetPhotoPath( int iOption , char * cPhotoPath );
	int _stdcall Syn_SetPhotoType( int iType );
	int _stdcall Syn_SetPhotoName( int iType );
	int _stdcall Syn_SetPhotoNameEx( int iType ,char * cPhotoname);	
	int _stdcall Syn_SetSexType( int iType );
	int _stdcall Syn_SetNationType( int iType );
	int _stdcall Syn_SetBornType( int iType );
	int _stdcall Syn_SetUserLifeBType( int iType );
	int _stdcall Syn_SetUserLifeEType( int iType ,int iOption);

	//////////////////////////////////////////////////////////////////////////
	//				USBHID函数
	//
	//////////////////////////////////////////////////////////////////////////

	int _stdcall Syn_USBHIDM1Reset( int iPort, DWORD * pdwCardSN, unsigned char * pbSize );
	int _stdcall Syn_USBHIDM1AuthenKey( int iPort, unsigned char KeyType, unsigned char BlockNo, unsigned char * pKey);
	int _stdcall Syn_USBHIDM1ReadBlock( int iPort, unsigned char BlockNo, unsigned char * pBlock );
	int _stdcall Syn_USBHIDM1WriteBlock( int iPort, unsigned char BlockNo, unsigned char * pBlock );
	int _stdcall Syn_USBHIDM1Halt( int iPort );
	int _stdcall Syn_USBHIDReadFactoryInfo( 
		int iPort ,
		unsigned char * pFactory,
		unsigned char * pVol,
		unsigned char * pSerialNo,
		unsigned char * pReserved);
	int _stdcall Syn_USBHIDWriteFactoryInfo(
		int iPort,
		unsigned char * pFactory,
		unsigned char * pVol,
		unsigned char * pSerialNo,
		unsigned char * pReserved);
	int _stdcall Syn_USBHIDReadUserInfo( int iPort,unsigned char *pInfo);
	int _stdcall Syn_USBHIDWriteUserInfo( int iPort,unsigned char *pInfo);
	int _stdcall Syn_USBHIDGetMaxByte( int iPort,unsigned char *pSAMMaxByte , unsigned char *pARMVol);
	int _stdcall Syn_USBHIDSendSound( int iPort, unsigned char ucSound);
	int _stdcall Syn_USBHIDControlLED( int iPort,unsigned char ucLed);
	//////////////////////////////////////////////////////////////////////////
	//				USBReade.dll USBHID函数
	//
	//////////////////////////////////////////////////////////////////////////
	int _stdcall Syn_USBOpenPort( int iPort );
	int _stdcall Syn_USBClosePort( int iPort );
	int _stdcall Syn_USBSetMaxRFByte (	int	iPort, unsigned char ucByte,int	bIfOpen );
	int _stdcall Syn_USBStartFindIDCard ( int iPort , unsigned char *	pucIIN,	int	iIfOpen	);
	int _stdcall Syn_USBSelectIDCard ( int iPort , unsigned char * pucSN,	int iIfOpen	);
	int _stdcall Syn_USBReadBaseMsg ( 
		int				iPort , 
		unsigned char * pucCHMsg , 
		unsigned int  * puiCHMsgLen , 
		unsigned char * pucPHMsg , 
		unsigned int  *	puiPHMsgLen , 
		int				iIfOpen);
	int	_stdcall Syn_USBReadBaseFPMsg(
		int				iPort, 
		unsigned char *	pucCHMsg, 
		unsigned int  *	puiCHMsgLen, 
		unsigned char *	pucPHMsg, 
		unsigned int  *	puiPHMsgLen, 
		unsigned char *	pucFPMsg, 
		unsigned int  *	puiFPMsgLen, 
		int				iIfOpen
		);
	int _stdcall Syn_USBReadNewAppMsg(int iPort , unsigned char * pucAppMsg , unsigned int * puiAppMsgLen , int iIfOpen);

	int _stdcall Syn_USBGetSAMID (	int iPort, unsigned char *	pucSAMID, int iIfOpen );
	int _stdcall Syn_USBGetSAMIDToStr ( int iPort,	char *	pcSAMID, int iIfOpen );

	int _stdcall Syn_USBM1Reset( int iPort, DWORD * pdwCardSN, unsigned char * pbSize );
	int _stdcall Syn_USBM1AuthenKey( int iPort, unsigned char KeyType, unsigned char BlockNo, unsigned char * pKey);
	int _stdcall Syn_USBM1ReadBlock( int iPort, unsigned char BlockNo, unsigned char * pBlock );
	int _stdcall Syn_USBM1WriteBlock( int iPort, unsigned char BlockNo, unsigned char * pBlock );
	int _stdcall Syn_USBM1Halt( int iPort );
	int _stdcall Syn_USBReadFactoryInfo( 
		int iPort ,
		unsigned char * pFactory,
		unsigned char * pVol,
		unsigned char * pSerialNo,
		unsigned char * pReserved);
	int _stdcall Syn_USBWriteFactoryInfo(
		int iPort,
		unsigned char * pFactory,
		unsigned char * pVol,
		unsigned char * pSerialNo,
		unsigned char * pReserved);
	int _stdcall Syn_USBReadUserInfo( int iPort,unsigned char *pInfo);
	int _stdcall Syn_USBWriteUserInfo( int iPort,unsigned char *pInfo);
	int _stdcall Syn_USBGetMaxByte( int iPort,unsigned char *pSAMMaxByte , unsigned char *pARMVol);
	int _stdcall Syn_USBSendSound( int iPort, unsigned char ucSound);
	int _stdcall Syn_USBControlLED( int iPort,unsigned char ucLed);
}