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
  var distance = (input.bytes[1] << 8 | input.bytes[2]);
  if (distance === 0) {
    distance = "NaN"
  }
  
  return {
    data: {
      battery: checkValue(input.bytes[0]),
      distance: distance
    }
  };
}
