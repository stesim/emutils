const api = require('./foo');

class HeapAllocation {
  constructor(size) {
    if(size <= 0) {
      throw new Error(`Invalid allocation size`);
    }

    this.pointer_ = api._malloc(size);
  }

  free() {
    api._free(this.pointer_);
  }

  get pointer() {
    return this.pointer_;
  }
}

class UInt32 extends HeapAllocation {
  constructor() {
    super(4);
  }

  get size() {
    return 4;
  }

  get value() {
    return api.HEAPU32[this.pointer >> 2];
  }
}

class PointerArray extends HeapAllocation {
  constructor(numElements) {
    numElements = numElements || 0;

    super(4 * numElements);

    this.array_ = new Uint32Array(api.HEAPU8.buffer, this.pointer, numElements);
  }

  get size() {
    return this.array_.length * this.array_.BYTES_PER_ELEMENT;
  }

  get values() {
    return this.array_;
  }
}

class HeapStack {
  constructor() {
    this.allocations_ = [];
  }

  wrap(func) {
    return function() {
      stack.push();
      const result = func.call(this, arguments);
      stack.pop();
      return result;
    }
  }

  push() {
    this.allocations_.push([]);
  }

  pop() {
    const currentAllocations = this.allocations_.pop();
    for(const allocation of currentAllocations) {
      allocation.free();
    }
  }

  add_(allocation) {
    this.allocations_[this.allocations_.length - 1].push(allocation);
    return allocation;
  }

  UInt32() {
    return this.add_(new UInt32());
  }

  Pointer() {
    return this.add_(new Pointer());
  }

  PointerArray(size) {
    return this.add_(new PointerArray(size));
  }
}

const stack = new HeapStack();

const Pointer = UInt32;

const version = stack.wrap(function() {
  return api.Pointer_stringify(api._foo_GetVersion());
});

const foos = stack.wrap(function() {
  const numFoos = stack.UInt32();
  let error = api._foo_GetFoos(0, null, numFoos.pointer);

  if(error) {
    throw new Error(error);
  }

  const foos = stack.PointerArray(numFoos.value);
  error = api._foo_GetFoos(numFoos.value, foos.pointer, null);

  if(error) {
    throw new Error(error);
  }

  return Array.from(foos.values, handle => new Foo(handle))
});

class Foo {
  constructor(handle) {
    this.handle_ = handle;
  }
}

exports.api = api;

exports.version = version;
exports.foos = foos;
exports.Foo = Foo;
