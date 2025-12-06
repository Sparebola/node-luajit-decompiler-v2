## LuaJIT Decompiler v2. Node support

_LuaJIT Decompiler v2_ is a replacement tool for the old and now mostly defunct python decompiler.  
The project fixes all of the bugs and quirks the python decompiler had while also offering  
full support for gotos and stripped bytecode including locals and upvalues.

## Usage

It is recommended to run in a separate node thread

```ts
import { type DecompileOptions, decompile } from "node-luajit-decompiler-v2";

const options: DecompileOptions = {
  ignoreDebugInfo: false,
  minimizeDiffs: true,
  unrestrictedAscii: true,
};

try {
  const tmpInFile = "file.luac";
  const buffer = fs.readFileSync(tmpInFile);
  const result = decompile(buffer, options);
  fs.writeFileSync(tmpOutFile, result, "binary");
} catch (error) {
  console.error(error);
}
```

## TODO

- bytecode big endian support
- improved decompilation logic for conditional assignments

---

Original project
https://github.com/marsinator358/luajit-decompiler-v2
