#ifndef SynIDCard_H
#define SynIDCard_H
#include <uv.h>
#include <node.h>
#include <node_buffer.h>
#include <node_object_wrap.h>
#include "SynPublic.h"

class SynIDCard : public node::ObjectWrap {
public:
	static void Init(v8::Handle<v8::Object> exports);

private:
	explicit SynIDCard();
	~SynIDCard();

	static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void DR_ReadCard(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void DR_Start_RealReadCard(const v8::FunctionCallbackInfo<v8::Value>& args);
	static void DR_Stop_RealReadCard(const v8::FunctionCallbackInfo<v8::Value>& args);
	static v8::Persistent<v8::Function> constructor;
};

#endif
