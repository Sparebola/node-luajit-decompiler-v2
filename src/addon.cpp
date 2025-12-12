#include <napi.h>
#include <string>
#include <cstdint>
#include <stdexcept>

#include "main.h"

struct Error {
	const std::string message;
	const std::string filePath;
	const std::string function;
	const std::string source;
	const std::string line;
};

static bool isProgressBarActive = false;

void print(const std::string& message) {
}

void print_progress_bar(const double& progress, const double& total) {
}

void erase_progress_bar() {
	isProgressBarActive = false;
}

void assert(const bool& assertion, const std::string& message, const std::string& filePath, const std::string& function, const std::string& source, const uint32_t& line) {
	if (!assertion) throw Error{
		.message = message,
		.filePath = filePath,
		.function = function,
		.source = source,
		.line = std::to_string(line)
	};
}

std::string byte_to_string(const uint8_t& byte) {
	char string[] = "0x00";
	uint8_t digit;
	
	for (uint8_t i = 2; i--;) {
		digit = (byte >> i * 4) & 0xF;
		string[3 - i] = digit >= 0xA ? 'A' + digit - 0xA : '0' + digit;
	}

	return string;
}

struct DecompileOptions {
	bool ignoreDebugInfo = false;
	bool minimizeDiffs = false;
	bool unrestrictedAscii = false;
};

std::string decompileBuffer(const uint8_t* buffer, size_t bufferSize, const DecompileOptions& options) {
	try {
		if (!buffer || bufferSize == 0) {
			throw std::runtime_error("Invalid buffer: buffer is null or size is zero");
		}
		Bytecode bytecode(buffer, bufferSize);
		bytecode();
		
		Ast ast(bytecode, options.ignoreDebugInfo, options.minimizeDiffs);
		ast();
		
		Lua lua(bytecode, ast, options.minimizeDiffs, options.unrestrictedAscii);
		lua();
		
		std::string result = lua.getResult();
		if (result.empty()) {
			throw std::runtime_error("Decompilation resulted in empty output");
		}
		return result;
	} catch (const Error& customError) {
		throw std::runtime_error("Decompilation error: " + customError.message + " (in " + customError.function + " at " + customError.source + ":" + customError.line + ")");
	} catch (const std::exception& stdException) {
		throw std::runtime_error(std::string("Decompilation error: ") + stdException.what());
	} catch (...) {
		throw std::runtime_error("Unknown decompilation error");
	}
}

Napi::Value Decompile(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();
	
	if (info.Length() < 1) {
		Napi::TypeError::New(env, "Expected at least 1 argument").ThrowAsJavaScriptException();
		return env.Null();
	}
	
	if (!info[0].IsBuffer()) {
		Napi::TypeError::New(env, "First argument must be a Buffer").ThrowAsJavaScriptException();
		return env.Null();
	}
	
	Napi::Buffer<uint8_t> buffer = info[0].As<Napi::Buffer<uint8_t>>();
	const uint8_t* data = buffer.Data();
	size_t length = buffer.Length();
	
	DecompileOptions options;
	
	if (info.Length() >= 2 && info[1].IsObject()) {
		Napi::Object opts = info[1].As<Napi::Object>();
		
		if (opts.Has("ignoreDebugInfo") && opts.Get("ignoreDebugInfo").IsBoolean()) {
			options.ignoreDebugInfo = opts.Get("ignoreDebugInfo").As<Napi::Boolean>().Value();
		}
		
		if (opts.Has("minimizeDiffs") && opts.Get("minimizeDiffs").IsBoolean()) {
			options.minimizeDiffs = opts.Get("minimizeDiffs").As<Napi::Boolean>().Value();
		}
		
		if (opts.Has("unrestrictedAscii") && opts.Get("unrestrictedAscii").IsBoolean()) {
			options.unrestrictedAscii = opts.Get("unrestrictedAscii").As<Napi::Boolean>().Value();
		}
	}
	
	try {
		std::string result = decompileBuffer(data, length, options);
		if (result.size() == 0) {
			Napi::Error::New(env, "Decompilation resulted in empty output").ThrowAsJavaScriptException();
			return env.Null();
		}
		Napi::Buffer<uint8_t> outputBuffer = Napi::Buffer<uint8_t>::Copy(env, reinterpret_cast<const uint8_t*>(result.data()), result.size());
		return outputBuffer;
	} catch (const std::exception& stdException) {
		Napi::Error::New(env, stdException.what()).ThrowAsJavaScriptException();
		return env.Null();
	} catch (...) {
		Napi::Error::New(env, "Unknown error during decompilation").ThrowAsJavaScriptException();
		return env.Null();
	}
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
	exports.Set(Napi::String::New(env, "decompile"), Napi::Function::New(env, Decompile));
	return exports;
}

NODE_API_MODULE(addon, Init)
