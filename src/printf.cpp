#include <stdio.h>
#include <functional>
#include <exception>
#include <string>
#include <sstream>
#include <emscripten/bind.h>
#include <emscripten.h>
#include <emscripten/val.h>

#define LOG_WARNING(msg) EM_ASM(console.warn(msg);)
#define LOG_ERROR(msg) EM_ASM(console.error(msg);)
#define LOG(...) EM_ASM(console.log(__VA_ARGS__);)

struct Blah
{
	float x, y, z;

	Blah(float x, float y, float z) : x(x), y(y), z(z) {}

	Blah(Blah *data)
	{
		Blah *myData = reinterpret_cast<Blah *>(data);
		x = myData->x;
		y = myData->y;
		z = myData->z;
	}

	emscripten::val toVal(void)
	{
		emscripten::val v = emscripten::val::object();
		v.set("x", x);
		v.set("y", y);
		v.set("z", z);
		EM_ASM_({console.log('Created data: { ' + $0 + ', ' + $1 + ', ' + $2 + ' }')}, x, y, z);
		return v;
	}
};

class BaseCallback
{
public:
	virtual void giveCallbackData(void *data) = 0;
};

class BaseCallbackWrapper : public emscripten::wrapper<BaseCallback>
{
public:
	EMSCRIPTEN_WRAPPER(BaseCallbackWrapper);

	virtual void giveCallbackData(void *data) override
	{
		Blah *b = reinterpret_cast<Blah *>(data);
		LOG("CBW giveCallbackData called!");
		return call<void>("giveCallbackData", b->toVal());
	}
};

class BS
{
public:
	struct 
	{
		float x;
		float y;
	};

	void someFunction(void)
	{
		Blah b { 1.0f, 0.0f, 3.0f };
		LOG("Woot");
	}

	void causeWhyNot(void)
	{
		LOG_ERROR("Hello, World");
		EM_ASM(console.warn('Blah!!!!'););
		LOG_WARNING("AHHHHH");
		EM_ASM("console.error('Blah!!!!');");
	}

	virtual void umm(void) {
		LOG_WARNING("umm");
	}

	virtual void yo(void) = 0;
};

class BBS : public BS
{
public:
	virtual void umm(void) override
	{
		LOG_ERROR("Umm");
	}

	virtual void yo(void) override
	{
		LOG("Sup Dude\n");
	}
};

void giveData(BaseCallback *cb)
{
	LOG("giveData begin");

	Blah *data = new Blah{ 2.0f, 1.0f, 0.0f };
	EM_ASM_({console.log('Created data: { ' + $0 + ', ' + $1 + ', ' + $2 + ' }')}, data->x, data->y, data->z);

	cb->giveCallbackData(data);

	LOG("giveData end");

	delete data;
}

typedef void (*FuncType)(void);

void someFunc(emscripten::val f)
{
	f();
}

EMSCRIPTEN_BINDINGS(BS_class)
{
	emscripten::class_<BS>("BS")
		.function("someFunction", &BS::someFunction)
		.function("causeWhyNot", &BS::causeWhyNot)
		.function("umm", &BS::umm)
		.function("yo", &BS::umm, emscripten::pure_virtual())
		;

	emscripten::class_<BBS, emscripten::base<BS>>("BBS")
		.constructor<>()
		.function("umm", &BBS::umm)
		.function("yo", &BBS::yo)
		;

	emscripten::class_<BaseCallback>("BaseCallback")
		.function("giveCallbackData", &BaseCallback::giveCallbackData, emscripten::pure_virtual(), emscripten::allow_raw_pointers())
		.allow_subclass<BaseCallbackWrapper>("BaseCallbackWrapper")
		;


	function("giveData", &giveData, emscripten::allow_raw_pointers());

	emscripten::class_<Blah>("Blah")
		.constructor<Blah *>()
		.property("x", &Blah::x)
		.property("y", &Blah::y)
		.property("z", &Blah::z)
		;

	function("someFunc", &someFunc, emscripten::allow_raw_pointers());
}

