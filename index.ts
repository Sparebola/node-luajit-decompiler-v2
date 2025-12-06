const addon = require("../build/Release/addon.node");

export interface DecompileOptions {
  ignoreDebugInfo?: boolean;
  minimizeDiffs?: boolean;
  unrestrictedAscii?: boolean;
}

export function decompile(
  buffer: Buffer,
  options: DecompileOptions = {}
): Buffer {
  if (!Buffer.isBuffer(buffer)) {
    throw new Error("First argument must be a Buffer");
  }

  const result = addon.decompile(buffer, options);
  if (!Buffer.isBuffer(result)) {
    throw new Error("Addon returned non-buffer result");
  }
  return result;
}
