#include "SynIDCard.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <windows.h>

using namespace v8;
using namespace std;

Persistent<Function> SynIDCard::constructor;

#pragma pack(push)
#pragma pack(2)
struct CardDataMapping
{
    WCHAR name[15];
    WCHAR gender[1];
    WCHAR folk[2];
    WCHAR birthDay[8];
    WCHAR address[35];
    WCHAR code[18];
    WCHAR agency[15];
    WCHAR expireStart[8];
    WCHAR expireEnd[8];
};
#pragma pack(pop)

struct DelayBaton
{
    // required
    uv_work_t request;                       // libuv
    uv_async_t async_request;                // libuv
    Persistent<Function> cbRealDataCallback; // javascript read callback
    Persistent<Function> cbErrorCallback;    // javascript error callback

    int iPort = 1001; //阅读器端口

    int isRead = 1;   // 是否开始监听
    int cardType = 1; // 卡类型：身份证：1，IC卡：2
    int status = 1;   // 状态1:正常，0：读取失败，-1：读取异常

    CardDataMapping *cardData;
    // char *name = new char[31];       //姓名
    // char *gender = new char[3];      //性别
    // char *folk = new char[10];       //民族
    // char *birthDay = new char[9];    //生日
    // char *code = new char[19];       //身份证号码
    // char *address = new char[71];    //地址
    // char *agency = new char[31];     //签证机关
    // char *expireStart = new char[9]; //有效期起始日期
    // char *expireEnd = new char[9];   //有效期截至日期
};

DelayBaton *baton;

SynIDCard::SynIDCard()
{
}

SynIDCard::~SynIDCard()
{
}

static std::string GBKToUTF8(const std::string &strGBK)
{
    static std::string strOutUTF8 = "";
    WCHAR *str1;
    int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
    str1 = new WCHAR[n];
    MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);
    n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
    char *str2 = new char[n];
    WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
    strOutUTF8 = str2;
    delete[] str1;
    str1 = NULL;
    delete[] str2;
    str2 = NULL;
    return strOutUTF8;
}

static std::string UNICODE_to_UTF8(const WCHAR *pSrc, int stringLength)
{
    static std::string strOutUTF8 = "";

    char *buffer = new char[stringLength + 1];
    WideCharToMultiByte(CP_UTF8, NULL, pSrc, wcslen(pSrc), buffer, stringLength, NULL, NULL);
    buffer[stringLength] = '\0';

    strOutUTF8 = buffer;

    delete[] buffer;

    const string &delim = " \t";
    string r = strOutUTF8.erase(strOutUTF8.find_last_not_of(delim) + 1);
    return r.erase(0, r.find_first_not_of(delim));
}

void SynIDCard::Init(Handle<Object> exports)
{
    Isolate *isolate = Isolate::GetCurrent();

    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
    tpl->SetClassName(String::NewFromUtf8(isolate, "SynIDCard"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    // Prototype
    NODE_SET_PROTOTYPE_METHOD(tpl, "ReadCard", DR_ReadCard);
    NODE_SET_PROTOTYPE_METHOD(tpl, "StartRealReadCard", DR_Start_RealReadCard);
    NODE_SET_PROTOTYPE_METHOD(tpl, "StopRealReadCard", DR_Stop_RealReadCard);

    constructor.Reset(isolate, tpl->GetFunction());
    exports->Set(String::NewFromUtf8(isolate, "SynIDCard"),
                 tpl->GetFunction());
}

void SynIDCard::New(const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    if (args.IsConstructCall())
    {
        // Invoked as constructor: `new MyObject(...)`

        SynIDCard *obj = new SynIDCard();
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    }
    else
    {
        // Invoked as plain function `MyObject(...)`, turn into construct call.
        const int argc = 1;
        Local<Value> argv[argc] = {args[0]};
        Local<Function> cons = Local<Function>::New(isolate, constructor);
        args.GetReturnValue().Set(cons->NewInstance(argc, argv));
    }
}

void SynIDCard::DR_ReadCard(const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    int iPort = args[0]->Int32Value();

    unsigned char pucIIN[4];
    unsigned char pucSN[8];

    unsigned char pucCHMsg[256];
    unsigned char pucPHMsg[1024];

    unsigned int puiCHMsgLen;
    unsigned int puiPHMsgLen;

    int result = Syn_OpenPort(iPort);

    if (result == 0)
    {
        result = Syn_StartFindIDCard(iPort, pucIIN, 0);
        result = Syn_SelectIDCard(iPort, pucSN, 0);
        // result = Syn_ReadMsg(iPort, 0, &idcardData);
        result = Syn_ReadBaseMsg(iPort, pucCHMsg, &puiCHMsgLen, pucPHMsg, &puiPHMsgLen, 0);

        CardDataMapping *cardData = (CardDataMapping *)&pucCHMsg;

        Local<Object> readInfo = Object::New(isolate);
        readInfo->Set(String::NewFromUtf8(isolate, "name"), String::NewFromUtf8(isolate, UNICODE_to_UTF8(cardData->name, 15).c_str()));
        readInfo->Set(String::NewFromUtf8(isolate, "gender"), String::NewFromUtf8(isolate, UNICODE_to_UTF8(cardData->gender, 1).c_str()));
        readInfo->Set(String::NewFromUtf8(isolate, "folk"), String::NewFromUtf8(isolate, UNICODE_to_UTF8(cardData->folk, 2).c_str()));
        readInfo->Set(String::NewFromUtf8(isolate, "birthDay"), String::NewFromUtf8(isolate, UNICODE_to_UTF8(cardData->birthDay, 8).c_str()));
        readInfo->Set(String::NewFromUtf8(isolate, "address"), String::NewFromUtf8(isolate, UNICODE_to_UTF8(cardData->address, 63).c_str()));
        readInfo->Set(String::NewFromUtf8(isolate, "code"), String::NewFromUtf8(isolate, UNICODE_to_UTF8(cardData->code, 18).c_str()));
        readInfo->Set(String::NewFromUtf8(isolate, "agency"), String::NewFromUtf8(isolate, UNICODE_to_UTF8(cardData->agency, 27).c_str()));
        readInfo->Set(String::NewFromUtf8(isolate, "expireStart"), String::NewFromUtf8(isolate, UNICODE_to_UTF8(cardData->expireStart, 8).c_str()));
        readInfo->Set(String::NewFromUtf8(isolate, "expireEnd"), String::NewFromUtf8(isolate, UNICODE_to_UTF8(cardData->expireEnd, 8).c_str()));

        args.GetReturnValue().Set(readInfo);
    }
    else
    {
        args.GetReturnValue().Set(Number::New(isolate, 0));
    }

    if (iPort > 0)
    {
        Syn_ClosePort(iPort);
    }
}

// libuv异步线程函数
void Delay(uv_work_t *req)
{
    DelayBaton *baton = static_cast<DelayBaton *>(req->data);
    try
    {
        do
        {
            unsigned char pucIIN[4];
            unsigned char pucSN[8];

            unsigned char pucCHMsg[256];
            unsigned char pucPHMsg[1024];

            unsigned int puiCHMsgLen;
            unsigned int puiPHMsgLen;

            Syn_ClosePort(baton->iPort);

            int result = Syn_OpenPort(baton->iPort);
            if (result != 0)
                continue;

            result = Syn_StartFindIDCard(baton->iPort, pucIIN, 0);

            if (result != 0)
                continue;

            result = Syn_SelectIDCard(baton->iPort, pucSN, 0);

            if (result != 0)
                continue;

            result = Syn_ReadBaseMsg(baton->iPort, pucCHMsg, &puiCHMsgLen, pucPHMsg, &puiPHMsgLen, 0);

            if (result == 0)
            {
                baton->cardData = (CardDataMapping *)&pucCHMsg;
                baton->cardType = 1;
                baton->status = 1;
                baton->async_request.data = baton;
                uv_async_send(&baton->async_request);
            }
        } while (baton->isRead);
    }
    catch (Exception)
    {
        baton->status = -1;
        baton->async_request.data = baton;
        uv_async_send(&baton->async_request);
    }
}

// libuv异步线程完成函数
void DelayAfter(uv_work_t *req, int status)
{
    DelayBaton *baton = static_cast<DelayBaton *>(req->data);
    uv_close((uv_handle_t *)&baton->async_request, NULL);
    baton->cbRealDataCallback.Reset();
}

// libuv异步事件循环完成回调函数
void DelayAsyncAfter(uv_async_t *req, int status)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    // 获取引用上下文
    DelayBaton *baton = static_cast<DelayBaton *>(req->data);

    if (baton->status)
    {
        Local<Object> readInfo = Object::New(isolate);
        readInfo->Set(String::NewFromUtf8(isolate, "cardType"), Number::New(isolate, baton->cardType));

        if (baton->cardType == 1)
        {
            CardDataMapping *cardData = baton->cardData;
            readInfo->Set(String::NewFromUtf8(isolate, "name"), String::NewFromUtf8(isolate, UNICODE_to_UTF8(cardData->name, 15).c_str()));
            readInfo->Set(String::NewFromUtf8(isolate, "gender"), String::NewFromUtf8(isolate, UNICODE_to_UTF8(cardData->gender, 1).c_str()));
            readInfo->Set(String::NewFromUtf8(isolate, "folk"), String::NewFromUtf8(isolate, UNICODE_to_UTF8(cardData->folk, 2).c_str()));
            readInfo->Set(String::NewFromUtf8(isolate, "birthDay"), String::NewFromUtf8(isolate, UNICODE_to_UTF8(cardData->birthDay, 8).c_str()));
            readInfo->Set(String::NewFromUtf8(isolate, "address"), String::NewFromUtf8(isolate, UNICODE_to_UTF8(cardData->address, 63).c_str()));
            readInfo->Set(String::NewFromUtf8(isolate, "code"), String::NewFromUtf8(isolate, UNICODE_to_UTF8(cardData->code, 18).c_str()));
            readInfo->Set(String::NewFromUtf8(isolate, "agency"), String::NewFromUtf8(isolate, UNICODE_to_UTF8(cardData->agency, 27).c_str()));
            readInfo->Set(String::NewFromUtf8(isolate, "expireStart"), String::NewFromUtf8(isolate, UNICODE_to_UTF8(cardData->expireStart, 8).c_str()));
            readInfo->Set(String::NewFromUtf8(isolate, "expireEnd"), String::NewFromUtf8(isolate, UNICODE_to_UTF8(cardData->expireEnd, 8).c_str()));
        }

        // 获取回调函数
        Local<Function> cb = Local<Function>::New(isolate, baton->cbRealDataCallback);
        const unsigned argc = 1;
        Local<Value> argv[argc] = {
            readInfo};
        cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
    }
    else if (baton->status == 0)
    {
        Local<Function> cb = Local<Function>::New(isolate, baton->cbErrorCallback);
        const unsigned argc = 1;
        Local<Value> argv[argc] = {String::NewFromUtf8(isolate, "Read the card information failure!")};
        cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
    }
    else
    {
        Local<Function> cb = Local<Function>::New(isolate, baton->cbErrorCallback);
        const unsigned argc = 1;
        Local<Value> argv[argc] = {String::NewFromUtf8(isolate, "Read the card information exception!")};
        cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
    }
}

void SynIDCard::DR_Start_RealReadCard(const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    int iPort = args[0]->Int32Value();
    int result = Syn_ClosePort(iPort);

    if (result == 0)
    {
		result = Syn_OpenPort(iPort);

		if (result == 0)
		{
			baton = new DelayBaton;
			baton->iPort = iPort;
			baton->request.data = baton;
			baton->cbRealDataCallback.Reset(isolate, Persistent<Function>::Persistent(isolate, Local<Function>::Cast(args[1])));
			baton->cbErrorCallback.Reset(isolate, Persistent<Function>::Persistent(isolate, Local<Function>::Cast(args[2])));

			// 事件循环队列异步函数
			// node.js默认事件循环机制
			uv_loop_t *loop = uv_default_loop();
			uv_async_init(loop, &baton->async_request, (uv_async_cb)DelayAsyncAfter);
			uv_queue_work(loop, &baton->request, Delay, DelayAfter);
			uv_run(loop, UV_RUN_NOWAIT);

			args.GetReturnValue().Set(Number::New(isolate, 1));
		}
		else
		{
			args.GetReturnValue().Set(Number::New(isolate, 0));
		}
    }
    else
    {
        args.GetReturnValue().Set(Number::New(isolate, 0));
    }
}

void SynIDCard::DR_Stop_RealReadCard(const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    int result = Syn_ClosePort(baton->iPort);
    baton->isRead = 0;

    args.GetReturnValue().Set(Number::New(isolate, result == 0 ? 1 : 0));
}

void InitAll(Handle<Object> exports)
{
    SynIDCard::Init(exports);
}

NODE_MODULE(SynIDCard, InitAll)
