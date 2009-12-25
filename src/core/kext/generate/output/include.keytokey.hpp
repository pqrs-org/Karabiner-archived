bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, toKeyCode2, toFlags2, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, toFlags2, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, toKeyCode2, toFlags2, toKeyCode3, toFlags3, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, toFlags2, toKeyCode3, toFlags3, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, toKeyCode3, toFlags3, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, toKeyCode3, toFlags3, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, toKeyCode3, toFlags3, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, toKeyCode3, toFlags3, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, toKeyCode3, toFlags3, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, toKeyCode3, toFlags3, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, toKeyCode2, toFlags2, toKeyCode3, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, toFlags2, toKeyCode3, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, toKeyCode3, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, toKeyCode3, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, toKeyCode3, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, toKeyCode3, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, toKeyCode3, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, toKeyCode3, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, toKeyCode2, toFlags2, toKeyCode3, toFlags3, toKeyCode4, toFlags4, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, toFlags2, toKeyCode3, toFlags3, toKeyCode4, toFlags4, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, toKeyCode3, toFlags3, toKeyCode4, toFlags4, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, toKeyCode3, toFlags3, toKeyCode4, toFlags4, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, toKeyCode3, toFlags3, toKeyCode4, toFlags4, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, toKeyCode3, toFlags3, toKeyCode4, toFlags4, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, toKeyCode3, toFlags3, toKeyCode4, toFlags4, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, toKeyCode3, toFlags3, toKeyCode4, toFlags4, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, toKeyCode2, toFlags2, toKeyCode3, ModifierFlag::NONE, toKeyCode4, toFlags4, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, toFlags2, toKeyCode3, ModifierFlag::NONE, toKeyCode4, toFlags4, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, toKeyCode3, ModifierFlag::NONE, toKeyCode4, toFlags4, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, toKeyCode3, ModifierFlag::NONE, toKeyCode4, toFlags4, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, toKeyCode3, ModifierFlag::NONE, toKeyCode4, toFlags4, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, toKeyCode3, ModifierFlag::NONE, toKeyCode4, toFlags4, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, toKeyCode3, ModifierFlag::NONE, toKeyCode4, toFlags4, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, toKeyCode3, ModifierFlag::NONE, toKeyCode4, toFlags4, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, toKeyCode2, toFlags2, toKeyCode3, toFlags3, toKeyCode4, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, toFlags2, toKeyCode3, toFlags3, toKeyCode4, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, toKeyCode3, toFlags3, toKeyCode4, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, toKeyCode3, toFlags3, toKeyCode4, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, toKeyCode3, toFlags3, toKeyCode4, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, toKeyCode3, toFlags3, toKeyCode4, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, toKeyCode3, toFlags3, toKeyCode4, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, toKeyCode3, toFlags3, toKeyCode4, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, toKeyCode2, toFlags2, toKeyCode3, ModifierFlag::NONE, toKeyCode4, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, toFlags2, toKeyCode3, ModifierFlag::NONE, toKeyCode4, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, toKeyCode3, ModifierFlag::NONE, toKeyCode4, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, toKeyCode3, ModifierFlag::NONE, toKeyCode4, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, toKeyCode3, ModifierFlag::NONE, toKeyCode4, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, toKeyCode3, ModifierFlag::NONE, toKeyCode4, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, toKeyCode3, ModifierFlag::NONE, toKeyCode4, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, toKeyCode3, ModifierFlag::NONE, toKeyCode4, ModifierFlag::NONE, KeyCode::VK_NONE, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5);

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, toFlags2, toKeyCode3, toFlags3, toKeyCode4, toFlags4, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, toKeyCode3, toFlags3, toKeyCode4, toFlags4, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, toKeyCode3, toFlags3, toKeyCode4, toFlags4, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, toKeyCode3, toFlags3, toKeyCode4, toFlags4, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, toKeyCode3, toFlags3, toKeyCode4, toFlags4, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, toKeyCode3, toFlags3, toKeyCode4, toFlags4, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, toKeyCode3, toFlags3, toKeyCode4, toFlags4, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, toKeyCode2, toFlags2, toKeyCode3, ModifierFlag::NONE, toKeyCode4, toFlags4, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, toFlags2, toKeyCode3, ModifierFlag::NONE, toKeyCode4, toFlags4, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, toKeyCode3, ModifierFlag::NONE, toKeyCode4, toFlags4, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, toKeyCode3, ModifierFlag::NONE, toKeyCode4, toFlags4, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, toKeyCode3, ModifierFlag::NONE, toKeyCode4, toFlags4, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, toKeyCode3, ModifierFlag::NONE, toKeyCode4, toFlags4, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, toKeyCode3, ModifierFlag::NONE, toKeyCode4, toFlags4, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, toKeyCode3, ModifierFlag::NONE, toKeyCode4, toFlags4, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, toKeyCode2, toFlags2, toKeyCode3, toFlags3, toKeyCode4, ModifierFlag::NONE, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, toFlags2, toKeyCode3, toFlags3, toKeyCode4, ModifierFlag::NONE, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, toKeyCode3, toFlags3, toKeyCode4, ModifierFlag::NONE, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, toKeyCode3, toFlags3, toKeyCode4, ModifierFlag::NONE, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, toKeyCode3, toFlags3, toKeyCode4, ModifierFlag::NONE, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, toKeyCode3, toFlags3, toKeyCode4, ModifierFlag::NONE, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, toKeyCode3, toFlags3, toKeyCode4, ModifierFlag::NONE, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, toKeyCode3, toFlags3, toKeyCode4, ModifierFlag::NONE, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, toKeyCode2, toFlags2, toKeyCode3, ModifierFlag::NONE, toKeyCode4, ModifierFlag::NONE, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, toFlags2, toKeyCode3, ModifierFlag::NONE, toKeyCode4, ModifierFlag::NONE, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, toKeyCode3, ModifierFlag::NONE, toKeyCode4, ModifierFlag::NONE, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, toKeyCode3, ModifierFlag::NONE, toKeyCode4, ModifierFlag::NONE, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, toKeyCode3, ModifierFlag::NONE, toKeyCode4, ModifierFlag::NONE, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, toKeyCode3, ModifierFlag::NONE, toKeyCode4, ModifierFlag::NONE, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, toKeyCode3, ModifierFlag::NONE, toKeyCode4, ModifierFlag::NONE, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5,  const Flags& toFlags5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, toKeyCode3, ModifierFlag::NONE, toKeyCode4, ModifierFlag::NONE, toKeyCode5, toFlags5);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, toKeyCode2, toFlags2, toKeyCode3, toFlags3, toKeyCode4, toFlags4, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, toFlags2, toKeyCode3, toFlags3, toKeyCode4, toFlags4, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, toKeyCode3, toFlags3, toKeyCode4, toFlags4, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, toKeyCode3, toFlags3, toKeyCode4, toFlags4, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, toKeyCode3, toFlags3, toKeyCode4, toFlags4, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, toKeyCode3, toFlags3, toKeyCode4, toFlags4, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, toKeyCode3, toFlags3, toKeyCode4, toFlags4, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, toKeyCode3, toFlags3, toKeyCode4, toFlags4, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, toKeyCode2, toFlags2, toKeyCode3, ModifierFlag::NONE, toKeyCode4, toFlags4, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, toFlags2, toKeyCode3, ModifierFlag::NONE, toKeyCode4, toFlags4, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, toKeyCode3, ModifierFlag::NONE, toKeyCode4, toFlags4, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, toKeyCode3, ModifierFlag::NONE, toKeyCode4, toFlags4, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, toKeyCode3, ModifierFlag::NONE, toKeyCode4, toFlags4, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, toKeyCode3, ModifierFlag::NONE, toKeyCode4, toFlags4, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, toKeyCode3, ModifierFlag::NONE, toKeyCode4, toFlags4, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,  const Flags& toFlags4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, toKeyCode3, ModifierFlag::NONE, toKeyCode4, toFlags4, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, toKeyCode2, toFlags2, toKeyCode3, toFlags3, toKeyCode4, ModifierFlag::NONE, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, toFlags2, toKeyCode3, toFlags3, toKeyCode4, ModifierFlag::NONE, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, toKeyCode3, toFlags3, toKeyCode4, ModifierFlag::NONE, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, toKeyCode3, toFlags3, toKeyCode4, ModifierFlag::NONE, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, toKeyCode3, toFlags3, toKeyCode4, ModifierFlag::NONE, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, toKeyCode3, toFlags3, toKeyCode4, ModifierFlag::NONE, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, toKeyCode3, toFlags3, toKeyCode4, ModifierFlag::NONE, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,  const Flags& toFlags3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, toKeyCode3, toFlags3, toKeyCode4, ModifierFlag::NONE, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, toKeyCode2, toFlags2, toKeyCode3, ModifierFlag::NONE, toKeyCode4, ModifierFlag::NONE, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, toFlags2, toKeyCode3, ModifierFlag::NONE, toKeyCode4, ModifierFlag::NONE, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, toKeyCode3, ModifierFlag::NONE, toKeyCode4, ModifierFlag::NONE, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,  const Flags& toFlags2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, toFlags2, toKeyCode3, ModifierFlag::NONE, toKeyCode4, ModifierFlag::NONE, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, toKeyCode3, ModifierFlag::NONE, toKeyCode4, ModifierFlag::NONE, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,  const Flags& toFlags1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, toFlags1, toKeyCode2, ModifierFlag::NONE, toKeyCode3, ModifierFlag::NONE, toKeyCode4, ModifierFlag::NONE, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode, const Flags& fromFlags,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, fromFlags, toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, toKeyCode3, ModifierFlag::NONE, toKeyCode4, ModifierFlag::NONE, toKeyCode5, ModifierFlag::NONE);
}

bool remap(RemapParams& remapParams,
           const KeyCode& fromKeyCode,
           const KeyCode& toKeyCode1,
           const KeyCode& toKeyCode2,
           const KeyCode& toKeyCode3,
           const KeyCode& toKeyCode4,
           const KeyCode& toKeyCode5) {
  return remap(remapParams, fromKeyCode, Flags(0), toKeyCode1, ModifierFlag::NONE, toKeyCode2, ModifierFlag::NONE, toKeyCode3, ModifierFlag::NONE, toKeyCode4, ModifierFlag::NONE, toKeyCode5, ModifierFlag::NONE);
}

