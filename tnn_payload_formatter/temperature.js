function unsignedToSignedByte(unsignedByte) {
  // Perform a bitwise operation with a signed byte range value
  return (unsignedByte << 24) >> 24;
}

function checkValue(value) {
  if (value == 0b10000000) {
    return "NaN";
  }
  
  return unsignedToSignedByte(value);
}

function decodeUplink(input) {
  return {
    data: {
      battery: checkValue(input.bytes[0]),
      humidity: checkValue(input.bytes[1]),
      temperature: checkValue(input.bytes[2])
    }
  };
}
